// UdfAddYxObjectDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CMS.h"
#include "UdfAddYxObjectDlg.h"


// CUdfAddYxObjectDlg �Ի���

IMPLEMENT_DYNAMIC(CUdfAddYxObjectDlg, CDialog)

CUdfAddYxObjectDlg::CUdfAddYxObjectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUdfAddYxObjectDlg::IDD, pParent)
	, m_stName(_T(""))
	, m_strYxName(_T(""))
	, m_dwYxCode(0)
{
	m_pCurStation = NULL;
	m_pCurYxObject = NULL;
	m_strCaption = "";

	m_OperateFlag = RECORD_NEW;
}

CUdfAddYxObjectDlg::~CUdfAddYxObjectDlg()
{
}

void CUdfAddYxObjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_STNAME, m_stName);
	DDV_MaxChars(pDX, m_stName, 50);
	DDX_Text(pDX, IDC_EDIT_YXNAME, m_strYxName);
	DDV_MaxChars(pDX, m_strYxName, 50);
	DDX_Text(pDX, IDC_EDIT_YXCODE, m_dwYxCode);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CUdfAddYxObjectDlg, CDialog)
END_MESSAGE_MAP()


// CUdfAddYxObjectDlg ��Ϣ�������

BOOL CUdfAddYxObjectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	
	if (m_OperateFlag == RECORD_EDIT && !m_strCaption.IsEmpty())
	{
		SetWindowText(m_strCaption);
	}

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CUdfAddYxObjectDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	UpdateData(TRUE);
	m_strYxName.Trim();
	if (m_strYxName.IsEmpty())
	{
		MessageBox("ң�����Ʋ���Ϊ��,������!","",MB_OK|MB_ICONERROR);
		return;
	}
	if (m_dwYxCode <= 0)
	{
		MessageBox("ң���벻��С�ڻ������,����������!","",MB_OK|MB_ICONERROR);
		return;
	}


	CDialog::OnOK();
}


void CUdfAddYxObjectDlg::Update(CRtuYxObject *pYx)
{
	if (pYx)
	{
		m_stName = pYx->GetParent()->GetName();
		m_strYxName = pYx->GetName();
		m_dwYxCode = pYx->GetCode();

		m_strCaption.Format("%s [������Ϣ]",pYx->GetName());
		m_pCurYxObject = pYx;
		m_pCurStation = pYx->GetParent();
		m_OperateFlag = RECORD_EDIT;

	}
}


void CUdfAddYxObjectDlg::SetRtuStation(CRtuStation* pStation)
{
	m_pCurStation = pStation;
	if (pStation)
		m_stName = pStation->GetName();

}