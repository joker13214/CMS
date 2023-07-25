// UdfShowDetailLog.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "UdfShowDetailLog.h"


// CUdfShowDetailLog 对话框

IMPLEMENT_DYNAMIC(CUdfShowDetailLog, CDialog)

CUdfShowDetailLog::CUdfShowDetailLog(CWnd* pParent /*=NULL*/)
	: CDialog(CUdfShowDetailLog::IDD, pParent)
	, m_strDatetime(_T(""))
	, m_strLogLevel(_T(""))
	, m_strLogType(_T(""))
	, m_strLogMessage(_T(""))
{

}

CUdfShowDetailLog::~CUdfShowDetailLog()
{
}

void CUdfShowDetailLog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_DATETIME, m_strDatetime);
	DDX_Text(pDX, IDC_STATIC_LOGLEVEL, m_strLogLevel);
	DDX_Text(pDX, IDC_STATIC_LOGTYPE, m_strLogType);
	DDX_Text(pDX, IDC_EDIT_LOGMESSAGE, m_strLogMessage);
}


BEGIN_MESSAGE_MAP(CUdfShowDetailLog, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// CUdfShowDetailLog 消息处理程序

HBRUSH CUdfShowDetailLog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性

	if ((pWnd->GetDlgCtrlID() == IDC_EDIT_LOGMESSAGE) /*&& (nCtlColor == CTLCOLOR_EDIT)*/)
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

void CUdfShowDetailLog::OnClose()
{
	ShowWindow(SW_HIDE);
}

void CUdfShowDetailLog::Show(bool blShow)
{
	UpdateData(FALSE);
	if (blShow)
	{
		//CenterWindow(GetParent());
		ShowWindow(SW_SHOW);
		SetActiveWindow();
	}
	else
	{
		ShowWindow(SW_HIDE);
	}
}
BOOL CUdfShowDetailLog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ModifyStyleEx(0,WS_EX_STATICEDGE); 

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CUdfShowDetailLog::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	// TODO: 在此处添加消息处理程序代码
	if (nState == WA_INACTIVE)
		ShowWindow(SW_HIDE);
}
