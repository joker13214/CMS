// RtuAttrSyqCallData2.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuAttrSyqCallData2.h"


#define W_TIMER_CALLDATA   0x01

// CRtuAttrSyqCallData2 对话框

IMPLEMENT_DYNAMIC(CRtuAttrSyqCallData2, CDialog)

CRtuAttrSyqCallData2::CRtuAttrSyqCallData2(CWnd* pParent /*=NULL*/)
	: CDialog(CRtuAttrSyqCallData2::IDD, pParent)
{

}

CRtuAttrSyqCallData2::~CRtuAttrSyqCallData2()
{
}

void CRtuAttrSyqCallData2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_YAOSU_DATA, m_listdata);
	DDX_Control(pDX, IDC_LIST_YAOSU, m_yaosulist1);
	DDX_Control(pDX, IDC_LIST_YAOSU2, m_yaosulist2);
	DDX_Control(pDX, IDC_BTN_ADD, m_btn_add);
	DDX_Control(pDX, IDC_BTN_DEL, m_btn_del);
	DDX_Control(pDX, IDC_BTN_QUERY, m_btn_query);

}


BEGIN_MESSAGE_MAP(CRtuAttrSyqCallData2, CDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_ADD, &CRtuAttrSyqCallData2::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL, &CRtuAttrSyqCallData2::OnBnClickedBtnDel)
	ON_BN_CLICKED(IDC_BTN_QUERY, &CRtuAttrSyqCallData2::OnBnClickedBtnQuery)
	ON_LBN_DBLCLK(IDC_LIST_YAOSU, &CRtuAttrSyqCallData2::OnLbnDblclkListYaosu)
	ON_LBN_DBLCLK(IDC_LIST_YAOSU2, &CRtuAttrSyqCallData2::OnLbnDblclkListYaosu2)
END_MESSAGE_MAP()


// CRtuAttrSyqCallData2 消息处理程序

BOOL CRtuAttrSyqCallData2::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_bResult = FALSE;

	if (m_yaosulist1.GetSafeHwnd())
	{
		m_yaosulist1.InsertString(0,"1min时段降水量");//PN01

		m_yaosulist1.InsertString(1,"5min时段降水量");//PN05

		m_yaosulist1.InsertString(2,"10min时段降水量");//PN10

		m_yaosulist1.InsertString(3,"30min时段降水量");//PN30

		m_yaosulist1.InsertString(4,"1h时段降水量");//P1

		m_yaosulist1.InsertString(5,"2h时段降水量");//P2

		m_yaosulist1.InsertString(6,"3h时段降水量");//P3

		m_yaosulist1.InsertString(7,"6h时段降水量");//P6

		m_yaosulist1.InsertString(8,"12h时段降水量");//P12

		m_yaosulist1.InsertString(9,"日降水量");//PD

		m_yaosulist1.InsertString(10,"月降水量");//PM

		m_yaosulist1.InsertString(11,"当日雨量");//PJ

		m_yaosulist1.InsertString(12,"暴雨量");//PR

		m_yaosulist1.InsertString(13,"降水量累计值");//PT

		m_yaosulist1.InsertString(14,"瞬时河道水位、潮位");//Z

		m_yaosulist1.InsertString(15,"日蒸发量");//ED

		m_yaosulist1.InsertString(16,"瞬时流量、抽水流量");//Q
	}
	

     m_yaosulist1.SetCurSel(0);

	 DWORD dwStyle = m_listdata.GetExtendedStyle();
	 dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	 dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	 //dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	 m_listdata.SetExtendedStyle(dwStyle); //设置扩展风格

	m_listdata.InsertColumn(0,_T("要素名称"),LVCFMT_LEFT,150);
	m_listdata.InsertColumn(1,_T("数据"),LVCFMT_LEFT,120);


	return TRUE;
}

void CRtuAttrSyqCallData2::SetParentDlg(CRtuPropertyDlg* pDlg)
{
	m_pParentDlg = pDlg;	
}

void CRtuAttrSyqCallData2::Update()
{
	if(m_pParentDlg)
		m_pSubStation = m_pParentDlg->GetRtuStation();	
}


void CRtuAttrSyqCallData2::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CString strLog= "";
	_Log_level_type LogLevel = Normal_Log_Message;

	switch (nIDEvent)
	{
	case W_TIMER_CALLDATA:
		{
			if (m_bResult == TRUE)
			{
				FillDataList();
				strLog = "查询遥测站指定要素数据 成功!";
			}
			else if ( m_bResult == FALSE)
			{
				strLog = "查询遥测站指定要素数据 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_bResult == 0xff)
			{
				strLog = "查询遥测站指定要素数据 错误!";
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


void CRtuAttrSyqCallData2::OnBnClickedBtnAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	int sel = m_yaosulist1.GetCurSel();
	CString str = "";
	m_yaosulist1.GetText(sel,str);

	if(str !="")
	{
		if(m_yaosulist2.GetCurSel() == -1)
		{
			m_yaosulist2.AddString(str);
			m_yaosulist2.SetCurSel(0);
		}
		else
		{
			int n = m_yaosulist2.FindString(0,str);
			if(n >=0)
			{
				AfxMessageBox("要素已存在！");
			}
			else
			{
				m_yaosulist2.AddString(str);
				m_yaosulist2.SetCurSel(m_yaosulist2.GetCurSel()+1);
			}

		}

		m_yaosulist1.SetCurSel(m_yaosulist1.GetCurSel()+1);

		
	}

}

void CRtuAttrSyqCallData2::OnBnClickedBtnDel()
{
	// TODO: 在此添加控件通知处理程序代码
	int sel = m_yaosulist2.GetCurSel();
	m_yaosulist2.DeleteString(sel);
	if(sel>=1)
		m_yaosulist2.SetCurSel(sel-1);
	else
	{
		if(m_yaosulist2.GetCount()>0)
		{
			sel = m_yaosulist2.GetCount();
			m_yaosulist2.SetCurSel(sel -1);
		}
	}


}



void CRtuAttrSyqCallData2::OnLbnDblclkListYaosu()
{
	// TODO: 在此添加控件通知处理程序代码

	int sel = m_yaosulist1.GetCurSel();
	CString str = "";
	m_yaosulist1.GetText(sel,str);

	if(str !="")
	{
		
		int n = m_yaosulist2.FindString(0,str);
			if(n >=0)
			{
				AfxMessageBox("要素已存在！");
			}
			else
			{
				m_yaosulist2.AddString(str);
				m_yaosulist2.SetCurSel(m_yaosulist2.GetCurSel()+1);
			}
	}
}

void CRtuAttrSyqCallData2::OnLbnDblclkListYaosu2()
{
	// TODO: 在此添加控件通知处理程序代码
	int sel = m_yaosulist2.GetCurSel();
	m_yaosulist2.DeleteString(sel);
	if(sel>=1)
		m_yaosulist2.SetCurSel(sel-1);
	else
	{
		if(m_yaosulist2.GetCount()>0)
			m_yaosulist2.SetCurSel(m_yaosulist2.GetCount()-1);
	}

}

void CRtuAttrSyqCallData2::OnBnClickedBtnQuery()
{
	// TODO: 在此添加控件通知处理程序代码
	int num =m_yaosulist2.GetCount();
	CString str1 = "",str="";
	if(num == 0)
	{
			AfxMessageBox("无指定要素，请选择！");
			return;
	}
	else
	{
		for (int i =0; i<num; i++)
		{
			m_yaosulist2.GetText(i,str);
			std::map<CString, SYQ_YS_STRUCT>::iterator it_map;
			//it_map = SYQ_TypeMap::GetInstance()->m_SYQTypeMap.find(tempstr);
			//if (it_map != SYQ_TypeMap::GetInstance()->m_SYQTypeMap.end())
			for(it_map = SYQ_TypeMap::GetInstance()->m_SYQTypeMap.begin();it_map != SYQ_TypeMap::GetInstance()->m_SYQTypeMap.end();it_map++)
			{
				if(it_map->second.name == str)
					break;
			}

			str = str +it_map->second.ename;


		}

		if (m_pSubStation->blEnbaleTrans())
		{
			CSYQFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息
			//添加数据打包
			m_pSubStation->GetSysDataTrans_Syq()->SYQ_EN_37H(&appFrame,str);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_CALLDATA, nElapse, NULL);
			SetEnable(FALSE);
		}


	}

}

void CRtuAttrSyqCallData2::SetEnable(bool b)
{
	m_yaosulist1.EnableWindow(b);
	m_yaosulist2.EnableWindow(b);
	m_btn_add.EnableWindow(b);
	m_btn_del.EnableWindow(b);
	m_btn_query.EnableWindow(b);
}


void CRtuAttrSyqCallData2::UpdateDlgMessage(int attr, bool result)
{
	m_bResult = result;

	if(attr == syq_call_data)
	{ 
		SendMessage(WM_TIMER,W_TIMER_CALLDATA, NULL);
	}

}

void CRtuAttrSyqCallData2::FillDataList()
{
	m_listdata.DeleteAllItems();

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
			m_listdata.InsertItem(&Item);


			Item.mask = LVIF_TEXT;
			Item.iItem = index;
			Item.iSubItem =1; //设置分项目,0代表是主项目   
			Item.pszText=(LPSTR)(LPCTSTR)((*itr)->strdata); 
			m_listdata.SetItem(&Item);

			index ++;

		}
	}
}
