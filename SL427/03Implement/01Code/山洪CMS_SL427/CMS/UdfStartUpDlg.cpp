// UdfStartUpDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "UdfStartUpDlg.h"


// CUdfStartUpDlg 对话框

IMPLEMENT_DYNAMIC(CUdfStartUpDlg, CDialog)

CUdfStartUpDlg::CUdfStartUpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUdfStartUpDlg::IDD, pParent)
{

}

CUdfStartUpDlg::~CUdfStartUpDlg()
{
}

void CUdfStartUpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUdfStartUpDlg, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CUdfStartUpDlg 消息处理程序

BOOL CUdfStartUpDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ModifyStyleEx(0,WS_EX_STATICEDGE);
	m_bitmap.LoadBitmap(IDB_BMP_START);		
	CenterWindow();
	SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CUdfStartUpDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()

	CBrush brush;
	brush.CreatePatternBrush(&m_bitmap);
	CRect rect;
	GetClientRect(rect);

	dc.FillRect(&rect, &brush);	
}
