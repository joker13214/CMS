// UdfStartUpDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CMS.h"
#include "UdfStartUpDlg.h"


// CUdfStartUpDlg �Ի���

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


// CUdfStartUpDlg ��Ϣ�������

BOOL CUdfStartUpDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	ModifyStyleEx(0,WS_EX_STATICEDGE);
	m_bitmap.LoadBitmap(IDB_BMP_START);		
	CenterWindow();
	SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CUdfStartUpDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()

	CBrush brush;
	brush.CreatePatternBrush(&m_bitmap);
	CRect rect;
	GetClientRect(rect);

	dc.FillRect(&rect, &brush);	
}
