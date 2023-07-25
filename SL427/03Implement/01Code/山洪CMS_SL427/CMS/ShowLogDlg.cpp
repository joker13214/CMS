// ShowLogDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "ShowLogDlg.h"


// CShowLogDlg 对话框

IMPLEMENT_DYNAMIC(CShowLogDlg, CDialog)

CShowLogDlg::CShowLogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShowLogDlg::IDD, pParent)
{

}

CShowLogDlg::~CShowLogDlg()
{
}

void CShowLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_RICHEDIT_LOGMSG, m_wndRichEditCtrl);
	DDX_Control(pDX, IDC_EDIT_DATE, m_wndEditDate);
	DDX_Control(pDX, IDC_EDIT_DMASSAGE, m_wndEditMessage);
}


BEGIN_MESSAGE_MAP(CShowLogDlg, CDialog)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CShowLogDlg 消息处理程序

void CShowLogDlg::OnClose()
{

	//CDialog::OnClose();
	ShowWindow(SW_HIDE);
}

BOOL CShowLogDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CShowLogDlg::ShowLog( CString strDate,CString strLog )
{
	if (m_wndEditDate.GetSafeHwnd())
		m_wndEditDate.SetWindowText(strDate);
	//if (m_wndRichEditCtrl.GetSafeHwnd())
	//	m_wndRichEditCtrl.SetWindowText(strLog);
	if (m_wndEditMessage.GetSafeHwnd())
		m_wndEditMessage.SetWindowText(strLog);

	ShowWindow(SW_SHOW);
}

HBRUSH CShowLogDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	if ((pWnd->GetDlgCtrlID() == IDC_EDIT_DMASSAGE) /*&& (nCtlColor == CTLCOLOR_EDIT)*/)
	{
		COLORREF clr = RGB(0,0,0);
		pDC->SetTextColor(clr);   //设置文本的现实颜色
		clr = RGB(255,255,255);
		pDC->SetBkColor(clr);     //设置背景色
		m_brMine = ::CreateSolidBrush(clr);
		return m_brMine;   //作为约定，返回背景色对应的刷子句柄
	}
	else
	{
		HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		return hbr;
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
