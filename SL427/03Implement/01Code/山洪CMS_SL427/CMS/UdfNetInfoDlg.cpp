// UdfNetInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "UdfNetInfoDlg.h"



// CUdfNetInfoDlg 对话框

IMPLEMENT_DYNAMIC(CUdfNetInfoDlg, CDialog)

CUdfNetInfoDlg::CUdfNetInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUdfNetInfoDlg::IDD, pParent)
	, m_strIPAddr(_T(""))
	, m_nPort(0)
{

}

CUdfNetInfoDlg::~CUdfNetInfoDlg()
{
}

void CUdfNetInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NETIPADDR, m_strIPAddr);
	DDV_MaxChars(pDX, m_strIPAddr, 20);
	DDX_Text(pDX, IDC_EDIT_NETPROT, m_nPort);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDV_MinMaxInt(pDX, m_nPort, 1, 65535);
}


BEGIN_MESSAGE_MAP(CUdfNetInfoDlg, CDialog)
END_MESSAGE_MAP()


// CUdfNetInfoDlg 消息处理程序

BOOL CUdfNetInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CUdfNetInfoDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	UpdateData(TRUE);
	m_strIPAddr.TrimLeft();
	m_strIPAddr.TrimRight();
	if (!CUdfGlobalFUNC::IpAddressCheck(m_strIPAddr))
	{
		MessageBox("IP地址不正确或为空,请重新输入","错误",MB_OK|MB_ICONERROR);
		return;
	}
	
	if (m_nPort == 0)
	{
		MessageBox("端口号不能为零,请重新输入","错误",MB_OK|MB_ICONERROR);
		return;
	}

	CDialog::OnOK();
}
