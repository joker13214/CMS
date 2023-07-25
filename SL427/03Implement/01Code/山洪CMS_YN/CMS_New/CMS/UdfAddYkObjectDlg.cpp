// UdfAddYkObjectDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CMS.h"
#include "UdfAddYkObjectDlg.h"


// CUdfAddYkObjectDlg �Ի���

IMPLEMENT_DYNAMIC(CUdfAddYkObjectDlg, CDialog)

CUdfAddYkObjectDlg::CUdfAddYkObjectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUdfAddYkObjectDlg::IDD, pParent)
	, m_stName(_T(""))
	, m_strYkName(_T(""))
{
	m_strCaption = "";
	m_OperateFlag = RECORD_NEW;

	m_pCurYkObject= NULL;
	m_pCurStation = NULL;
	m_wYkCode = 0;
}

CUdfAddYkObjectDlg::~CUdfAddYkObjectDlg()
{
}

void CUdfAddYkObjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_STNAME, m_stName);
	DDV_MaxChars(pDX, m_stName, 50);
	DDX_Text(pDX, IDC_EDIT_YKNAME, m_strYkName);
	DDV_MaxChars(pDX, m_strYkName, 50);
	DDX_Text(pDX, IDC_EDIT_YKCODE, m_wYkCode);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CUdfAddYkObjectDlg, CDialog)
END_MESSAGE_MAP()


// CUdfAddYkObjectDlg ��Ϣ�������


void CUdfAddYkObjectDlg::Update(CRtuYkObject* pYk)
{
	if (pYk)
	{
		m_stName = pYk->GetParent()->GetName();
		m_strYkName = pYk->GetName();
		m_wYkCode = pYk->GetCode();

		m_pCurYkObject = pYk;
		m_pCurStation = pYk->GetParent();

		m_OperateFlag = RECORD_EDIT;
		m_strCaption.Format("%s [������Ϣ]",pYk->GetName());

	}
}

BOOL CUdfAddYkObjectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	

	if (!m_strCaption.IsEmpty())
	{
		SetWindowText(m_strCaption);

	}

    UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CUdfAddYkObjectDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	UpdateData(TRUE);

	m_strYkName.Trim();
	if (m_strYkName.IsEmpty())
	{
		MessageBox("ң�����Ʋ���Ϊ��,������!","",MB_OK|MB_ICONERROR);
		return;
	}
	if (m_wYkCode <= 0)
	{
		MessageBox("ң���벻��С�ڻ������,����������!","",MB_OK|MB_ICONERROR);
		return;
	}

	CDialog::OnOK();
}


void CUdfAddYkObjectDlg::SetRtuStation(CRtuStation* pStation)
{
	m_pCurStation = pStation;
	if (pStation)
		m_stName = pStation->GetName();
}