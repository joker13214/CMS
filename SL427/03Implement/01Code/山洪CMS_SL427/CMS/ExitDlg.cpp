// ExitDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CMS.h"
#include "ExitDlg.h"


// CExitDlg �Ի���

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


// CExitDlg ��Ϣ�������

BOOL CExitDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
    CenterWindow();
	SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
