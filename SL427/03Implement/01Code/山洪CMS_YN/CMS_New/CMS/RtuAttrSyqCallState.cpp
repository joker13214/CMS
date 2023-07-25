// RtuAttrSyqCallState.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuAttrSyqCallState.h"


#define W_TIMER_CALLSTATE   0x01
// CRtuAttrSyqCallState 对话框

IMPLEMENT_DYNAMIC(CRtuAttrSyqCallState, CDialog)

CRtuAttrSyqCallState::CRtuAttrSyqCallState(CWnd* pParent /*=NULL*/)
	: CDialog(CRtuAttrSyqCallState::IDD, pParent)
{

}

CRtuAttrSyqCallState::~CRtuAttrSyqCallState()
{
}

void CRtuAttrSyqCallState::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_QUERY, m_btn_query);
	DDX_Control(pDX, IDC_LIST, m_list);
}


BEGIN_MESSAGE_MAP(CRtuAttrSyqCallState, CDialog)
	ON_BN_CLICKED(IDC_BTN_QUERY, &CRtuAttrSyqCallState::OnBnClickedBtnQuery)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CRtuAttrSyqCallState 消息处理程序

// CRtuAttrSyqCallData1 消息处理程序
BOOL CRtuAttrSyqCallState::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_bResult = FALSE;


	DWORD dwStyle = m_list.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	//dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	m_list.SetExtendedStyle(dwStyle); //设置扩展风格

	m_list.InsertColumn(0,_T("名称"),LVCFMT_LEFT,150);
	m_list.InsertColumn(1,_T("状态"),LVCFMT_LEFT,120);


	FillList(0,"交流电充电状态","");
	FillList(1,"蓄电池电压状态","");
	FillList(2,"水位超限报警状态","");
	FillList(3,"流量超限报警状态","");
	FillList(4,"水质超限报警状态","");
	FillList(5,"流量仪表状态","");
	FillList(6,"水位仪表状态","");
	FillList(7,"终端箱门状态","");
	FillList(8,"存储器状态","");
	FillList(9,"IC卡功能有效性","");
	FillList(10,"水泵工作状态","");



	return TRUE;
}

void CRtuAttrSyqCallState::FillList(int index,CString strname,CString strstate)
{
	LVITEM Item;
	Item.mask = LVIF_TEXT;
	Item.iItem = index;
	Item.iSubItem =0; //设置分项目,0代表是主项目   
	Item.pszText=(LPSTR)(LPCTSTR)(strname); 
	m_list.InsertItem(&Item);


	Item.mask = LVIF_TEXT;
	Item.iItem = index;
	Item.iSubItem =1; //设置分项目,0代表是主项目   
	Item.pszText=(LPSTR)(LPCTSTR)(strstate); 
	m_list.SetItem(&Item);
}

void CRtuAttrSyqCallState::SetParentDlg(CRtuPropertyDlg* pDlg)
{
	m_pParentDlg = pDlg;	
}

void CRtuAttrSyqCallState::Update()
{
	if(m_pParentDlg)
		m_pSubStation = m_pParentDlg->GetRtuStation();	
}


void CRtuAttrSyqCallState::OnBnClickedBtnQuery()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pSubStation->blEnbaleTrans())
	{
		SetEnable(FALSE);

		m_pSubStation->m_syqState = -1;

		CSYQFrameFormat appFrame;
		m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息
		//添加数据打包
		m_pSubStation->GetSysDataTrans_Syq()->SYQ_EN_COMMON(&appFrame,0x48,TRUE);

		UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
		SetTimer(W_TIMER_CALLSTATE, nElapse, NULL);
	}
//	m_pSubStation->m_syqState = 30;
	//FillState();

}

void CRtuAttrSyqCallState::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString strLog= "";
	_Log_level_type LogLevel = Normal_Log_Message;

	switch (nIDEvent)
	{
	case W_TIMER_CALLSTATE:
		{
			if (m_bResult == TRUE)
			{
			//	FillDataList1();
				strLog = "查询遥测站状态和报警信息 成功!";
			}
			else if ( m_bResult == FALSE)
			{
				strLog = "查询遥测站状态和报警信息 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_bResult == 0xff)
			{
				strLog = "查询遥测站状态和报警信息 错误!";
				LogLevel = Warning_log_Message;
			}
		}
		break;
	default:
		break;;
	}

	KillTimer(nIDEvent);
	SetEnable(TRUE);
	if (m_pParentDlg && m_pParentDlg->GetSafeHwnd())
		m_pParentDlg->AddLog(strLog,(PSYSTEMTIME)NULL,LogLevel);

	if (m_pSubStation)
		m_pSubStation->InsertLogMessage(strLog,LogLevel,NULL);

	CDialog::OnTimer(nIDEvent);

}
void CRtuAttrSyqCallState::SetEnable(bool b)
{
	m_btn_query.EnableWindow(b);
}

void CRtuAttrSyqCallState::UpdateDlgMessage(int attr, bool result)
{
	m_bResult = result;

	if(attr == syq_call_state)
		SendMessage(WM_TIMER,W_TIMER_CALLSTATE, NULL);
	
}

void CRtuAttrSyqCallState::FillState()
{
	m_list.DeleteAllItems();

	CString strname = "", strstate = "";
	int temp;
	if(m_pSubStation->m_syqState != -1)
	{
		for (int i = 0; i<10; i++)
		{
			switch(i)
			{
			case 0:
				{
					strname = "交流电充电状态";
					temp = m_pSubStation->m_syqState&0x01;
					if(temp == 0)
						strstate = "正常";
					else if(temp == 1)
						strstate = "停电";
				}
				break;
			case 1:
			{
				strname = "蓄电池电压状态";
				temp = (m_pSubStation->m_syqState&0x02)>>1;
				if(temp == 0)
					strstate = "正常";
				else if(temp == 1)
					strstate = "电压低";
			}
			break;
			case 2:
				{
					strname = "水位超限报警状态";
					temp = (m_pSubStation->m_syqState&0x04)>>2;
					if(temp == 0)
						strstate = "正常";
					else if(temp == 1)
						strstate = "报警";
				}
				break;
			case 3:
				{
					strname = "流量超限报警状态";
					temp = (m_pSubStation->m_syqState&0x08)>>3;
					if(temp == 0)
						strstate = "正常";
					else if(temp == 1)
						strstate = "报警";
				}
				break;
			case 4:
				{
					strname = "水质超限报警状态";
					temp = (m_pSubStation->m_syqState&0x10)>>4;
					if(temp == 0)
						strstate = "正常";
					else if(temp == 1)
						strstate = "报警";
				}
				break;
			case 5:
				{
					strname = "流量仪表状态";
					temp = (m_pSubStation->m_syqState&0x20)>>5;
					if(temp == 0)
						strstate = "正常";
					else if(temp == 1)
						strstate = "故障";
				}
				break;
			case 6:
				{
					strname = "水位仪表状态";
					temp = (m_pSubStation->m_syqState&0x40)>>6;
					if(temp == 0)
						strstate = "正常";
					else if(temp == 1)
						strstate = "故障";
				}
				break;
			case 7:
				{
					strname = "终端箱门状态";
					temp = (m_pSubStation->m_syqState&0x80)>>7;
					if(temp == 0)
						strstate = "开启";
					else if(temp == 1)
						strstate = "关闭";
				}
				break;
			case 8:
				{
					strname = "存储器状态";
					temp = (m_pSubStation->m_syqState&0x100)>>8;
					if(temp == 0)
						strstate = "正常";
					else if(temp == 1)
						strstate = "异常";
				}
				break;
			case 9:
				{
					strname = "卡功能有效性";
					temp = (m_pSubStation->m_syqState&0x200)>>9;
					if(temp == 0)
						strstate = "IC卡有效";
					else if(temp == 1)
						strstate = "关闭";
				}
				break;
			case 10:
				{
					strname = "卡功能有效性";
					temp = (m_pSubStation->m_syqState&0x400)>>10;
					if(temp == 0)
						strstate = "IC水泵工作";
					else if(temp == 1)
						strstate = "水泵停机";
				}
				break;

			}

			FillList(i,strname,strstate);
		}
	}
}