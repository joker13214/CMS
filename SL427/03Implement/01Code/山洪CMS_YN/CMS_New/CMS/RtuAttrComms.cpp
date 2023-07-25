// RtuAttrComms.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuAttrComms.h"
#include "RtuPropertyDlg.h"


// CRtuAttrComms 对话框

IMPLEMENT_DYNAMIC(CRtuAttrComms, CDialog)

CRtuAttrComms::CRtuAttrComms(CWnd* pParent /*=NULL*/)
	: CDialog(CRtuAttrComms::IDD, pParent)
{
	m_pParentDlg	= NULL;
	m_pSubStation	= NULL;
}

CRtuAttrComms::~CRtuAttrComms()
{

}

void CRtuAttrComms::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_COMMS, m_wndCommsList);
}


BEGIN_MESSAGE_MAP(CRtuAttrComms, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CRtuAttrComms 消息处理程序

void CRtuAttrComms::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	//if (m_wndCommsList.GetSafeHwnd())
	//{
	//	m_wndCommsList.MoveWindow(5,5,cx-10,cy-10);
	//}
}

void CRtuAttrComms::Update()
{
	if (!m_pParentDlg) return;
	if (!m_pParentDlg->GetRtuStation()) return;

	m_pSubStation = m_pParentDlg->GetRtuStation();
}

void CRtuAttrComms::SetParentDlg( CRtuPropertyDlg* pParent )
{
	m_pParentDlg = pParent;
}

void CRtuAttrComms::AddMessageToWindow( CString m_Message, _Log_level_type m_Level, SYSTEMTIME m_Time )
{
	m_Message.Trim();
	if (m_Message.IsEmpty()) return;

	CString strTime = "";
	strTime.Format("%04d-%02d-%02d %02d:%02d:%02d.%03d",
		m_Time.wYear,
		m_Time.wMonth,
		m_Time.wDay,
		m_Time.wHour,
		m_Time.wMinute,
		m_Time.wSecond,
		m_Time.wMilliseconds);

	if (m_wndCommsList.GetSafeHwnd())
	{
		int nCount = m_wndCommsList.GetItemCount();
		int nInsertIdx = m_wndCommsList.InsertItem(nCount,strTime);
		if (nInsertIdx != -1)
		{
			m_wndCommsList.SetItemText(nInsertIdx,1,m_Message);
			m_wndCommsList.SetItemData(nInsertIdx,(DWORD_PTR)m_Level);
		}

		if (m_wndCommsList.GetItemCount() >  MAX_LogCount)
			m_wndCommsList.DeleteItem(0);
	}
}

BOOL CRtuAttrComms::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitListCtrlHeader();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CRtuAttrComms::InitListCtrlHeader()
{
	if (m_wndCommsList.GetSafeHwnd())
	{
		DWORD dw = m_wndCommsList.GetExtendedStyle();
		dw |= LVS_EX_FULLROWSELECT;
		m_wndCommsList.SetExtendedStyle(dw);

		m_wndCommsList.DeleteAllItems();
		m_wndCommsList.InsertColumn(0,"时间",LVCFMT_LEFT,140);
		m_wndCommsList.InsertColumn(1,"信息",LVCFMT_LEFT,520);
	}
}
