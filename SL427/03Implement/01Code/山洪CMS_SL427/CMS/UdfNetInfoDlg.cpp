// UdfNetInfoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CMS.h"
#include "UdfNetInfoDlg.h"



// CUdfNetInfoDlg �Ի���

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


// CUdfNetInfoDlg ��Ϣ�������

BOOL CUdfNetInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CUdfNetInfoDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	UpdateData(TRUE);
	m_strIPAddr.TrimLeft();
	m_strIPAddr.TrimRight();
	if (!CUdfGlobalFUNC::IpAddressCheck(m_strIPAddr))
	{
		MessageBox("IP��ַ����ȷ��Ϊ��,����������","����",MB_OK|MB_ICONERROR);
		return;
	}
	
	if (m_nPort == 0)
	{
		MessageBox("�˿ںŲ���Ϊ��,����������","����",MB_OK|MB_ICONERROR);
		return;
	}

	CDialog::OnOK();
}
