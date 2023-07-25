// ExitDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "ExitDlg.h"


// CExitDlg 对话框

IMPLEMENT_DYNAMIC(CExitDlg, CDialog)

CExitDlg::CExitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExitDlg::IDD, pParent)
{

}

CExitDlg::~CExitDlg()
{
}

void CExitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CExitDlg, CDialog)
END_MESSAGE_MAP()


// CExitDlg 消息处理程序

BOOL CExitDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
    CenterWindow();
	SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
