// RtuAttrSyqCallData1.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuAttrSyqCallData1.h"

#define W_TIMER_CALLDATA1   0x01
#define W_TIMER_CALLDATA2  0x02
// CRtuAttrSyqCallData1 对话框

IMPLEMENT_DYNAMIC(CRtuAttrSyqCallData1, CDialog)

CRtuAttrSyqCallData1::CRtuAttrSyqCallData1(CWnd* pParent /*=NULL*/)
	: CDialog(CRtuAttrSyqCallData1::IDD, pParent)
{

}

CRtuAttrSyqCallData1::~CRtuAttrSyqCallData1()
{
}

void CRtuAttrSyqCallData1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DATA2, m_listdata1);
	DDX_Control(pDX, IDC_LIST_DATA1, m_listdata2);
	DDX_Control(pDX, IDC_CALLDATA_BTN1, m_btn_calldata1);
	DDX_Control(pDX, IDC_CALLDATA_BTN2, m_btn_calldata2);
}


BEGIN_MESSAGE_MAP(CRtuAttrSyqCallData1, CDialog)
	ON_BN_CLICKED(IDC_CALLDATA_BTN1, &CRtuAttrSyqCallData1::OnBnClickedCalldataBtn1)
	ON_BN_CLICKED(IDC_CALLDATA_BTN2, &CRtuAttrSyqCallData1::OnBnClickedCalldataBtn2)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CRtuAttrSyqCallData1 消息处理程序
BOOL CRtuAttrSyqCallData1::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_bResult = FALSE;

	DWORD dwStyle = m_listdata1.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	//dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	m_listdata1.SetExtendedStyle(dwStyle); //设置扩展风格

	 dwStyle = m_listdata2.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	//dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	m_listdata2.SetExtendedStyle(dwStyle); //设置扩展风格

	m_listdata1.InsertColumn(0,_T("要素名称"),LVCFMT_LEFT,150);
	m_listdata1.InsertColumn(1,_T("数据"),LVCFMT_LEFT,120);

	m_listdata2.InsertColumn(0,_T("要素名称"),LVCFMT_LEFT,150);
	m_listdata2.InsertColumn(1,_T("数据"),LVCFMT_LEFT,120);

	return TRUE;
}

void CRtuAttrSyqCallData1::SetParentDlg(CRtuPropertyDlg* pDlg)
{
	m_pParentDlg = pDlg;	
}

void CRtuAttrSyqCallData1::Update()
{
	if(m_pParentDlg)
		m_pSubStation = m_pParentDlg->GetRtuStation();	
}


void CRtuAttrSyqCallData1::OnBnClickedCalldataBtn1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pSubStation->blEnbaleTrans())
	{
		SetEnable(FALSE);

		CSYQFrameFormat appFrame;
		m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息
		//添加数据打包
		m_pSubStation->GetSysDataTrans_Syq()->SYQ_EN_COMMON(&appFrame,0x36,TRUE);

		UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
		SetTimer(W_TIMER_CALLDATA1, nElapse, NULL);
	}
}

void CRtuAttrSyqCallData1::OnBnClickedCalldataBtn2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pSubStation->blEnbaleTrans())
	{
		SetEnable(FALSE);

		CSYQFrameFormat appFrame;
		m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息
		//添加数据打包
		m_pSubStation->GetSysDataTrans_Syq()->SYQ_EN_COMMON(&appFrame,0x39,TRUE);

		UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
		SetTimer(W_TIMER_CALLDATA2, nElapse, NULL);
	}
}
void CRtuAttrSyqCallData1::UpdateDlgMessage(int attr, bool result)
{
	m_bResult = result;

	if(attr == syq_call_data1)
	{ 
		SendMessage(WM_TIMER,W_TIMER_CALLDATA1, NULL);
	}
	else if(attr == syq_call_data2)
	{ 
		SendMessage(WM_TIMER,W_TIMER_CALLDATA2, NULL);
	}
}

void CRtuAttrSyqCallData1::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString strLog= "";
	_Log_level_type LogLevel = Normal_Log_Message;

	switch (nIDEvent)
	{
	case W_TIMER_CALLDATA1:
		{
			if (m_bResult == TRUE)
			{
				FillDataList1();
				strLog = "查询遥测站实时要素数据 成功!";
			}
			else if ( m_bResult == FALSE)
			{
				strLog = "查询遥测站实时要素数据 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_bResult == 0xff)
			{
				strLog = "查询遥测站实时要素数据 错误!";
				LogLevel = Warning_log_Message;
			}
		}
		break;
	case W_TIMER_CALLDATA2:
		{
			if (m_bResult == TRUE)
			{
				FillDataList2();
				strLog = "查询遥测站人工置数 成功!";
			}
			else if ( m_bResult == FALSE)
			{
				strLog = "查询遥测站人工置数 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_bResult == 0xff)
			{
				strLog = "查询遥测站人工置数 错误!";
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


void CRtuAttrSyqCallData1::SetEnable(bool b)
{
	m_btn_calldata1.EnableWindow(b);
	m_btn_calldata2.EnableWindow(b);
}


void CRtuAttrSyqCallData1::FillDataList1()
{
	m_listdata1.DeleteAllItems();

	if(m_pSubStation->m_syqdatadiscribelist.size()>0)
	{
		std::list<_SYQ_DATA_DESCRIBE*>::iterator itr;
		int index = 0;
		LVITEM Item;
		for(itr =m_pSubStation->m_syqdatadiscribelist.begin(); itr != m_pSubStation->m_syqdatadiscribelist.end();itr++ )
		{		
			Item.mask = LVIF_TEXT;
			Item.iItem = index;
			Item.iSubItem =0; //设置分项目,0代表是主项目   
			Item.pszText=(LPSTR)(LPCTSTR)((*itr)->strname); 
			m_listdata1.InsertItem(&Item);


			Item.mask = LVIF_TEXT;
			Item.iItem = index;
			Item.iSubItem =1; //设置分项目,0代表是主项目   
			Item.pszText=(LPSTR)(LPCTSTR)((*itr)->strdata); 
			m_listdata1.SetItem(&Item);

			index ++;

		}
	}
}



void CRtuAttrSyqCallData1::FillDataList2()
{
	m_listdata2.DeleteAllItems();

	if(m_pSubStation->m_syqdatadiscribelist.size()>0)
	{
		std::list<_SYQ_DATA_DESCRIBE*>::iterator itr;
		int index = 0;
		LVITEM Item;
		for(itr =m_pSubStation->m_syqdatadiscribelist.begin(); itr != m_pSubStation->m_syqdatadiscribelist.end();itr++ )
		{		
			Item.mask = LVIF_TEXT;
			Item.iItem = index;
			Item.iSubItem =0; //设置分项目,0代表是主项目   
			Item.pszText=(LPSTR)(LPCTSTR)((*itr)->strname); 
			m_listdata2.InsertItem(&Item);


			Item.mask = LVIF_TEXT;
			Item.iItem = index;
			Item.iSubItem =1; //设置分项目,0代表是主项目   
			Item.pszText=(LPSTR)(LPCTSTR)((*itr)->strdata); 
			m_listdata2.SetItem(&Item);

			index ++;

		}
	}
}
