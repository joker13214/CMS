// UdfAddZdObjectDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CMS.h"
#include "UdfAddZdObjectDlg.h"


// CUdfAddZdObjectDlg �Ի���

IMPLEMENT_DYNAMIC(CUdfAddZdObjectDlg, CDialog)

CUdfAddZdObjectDlg::CUdfAddZdObjectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUdfAddZdObjectDlg::IDD, pParent)
	, m_stName(_T(""))
	, m_strZdName(_T(""))
	, m_dwZdCode(0)
	, m_bSizeof(16)
	, m_OrderInItem(0)
	, m_strUnit(_T(""))
	, m_fScaleMax(0)
	, m_fScaleMin(0)
{
	m_strCaption = "";
	m_OperateFlag = RECORD_NEW;

	m_pCurZdObject= NULL;
	m_pCurStation = NULL;
}

CUdfAddZdObjectDlg::~CUdfAddZdObjectDlg()
{
}

void CUdfAddZdObjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_STNAME, m_stName);
	DDV_MaxChars(pDX, m_stName, 50);
	DDX_Text(pDX, IDC_EDIT_ZDNAME, m_strZdName);
	DDX_Text(pDX, IDC_EDIT_ZDCODE, m_dwZdCode);
	DDX_Text(pDX, IDC_EDIT_SIZEOF, m_bSizeof);
	DDX_Text(pDX, IDC_EDIT_ORDERINITEM, m_OrderInItem);
	DDX_Text(pDX, IDC_EDIT_UNIT, m_strUnit);
	DDX_Text(pDX, IDC_EDIT_SCALEMAX, m_fScaleMax);
	DDX_Text(pDX, IDC_EDIT_SCALEMIN, m_fScaleMin);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CUdfAddZdObjectDlg, CDialog)
END_MESSAGE_MAP()


// CUdfAddZdObjectDlg ��Ϣ�������

BOOL CUdfAddZdObjectDlg::OnInitDialog()
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

void CUdfAddZdObjectDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	UpdateData(TRUE);
	m_strZdName.Trim();
	if (m_strZdName.IsEmpty())
	{
		MessageBox("���������Ʋ���Ϊ��,������!","",MB_OK|MB_ICONERROR);
		return;
	}

	if (m_dwZdCode <= 0)
	{
		MessageBox("��ֵ����������,����������!","",MB_OK|MB_ICONERROR);
		return;
	}

	CDialog::OnOK();
}


void CUdfAddZdObjectDlg::Update(CRtuZdObject* pZd)
{
	if (pZd)
	{
		m_stName = pZd->GetParent()->GetName();
		m_strZdName= pZd->GetName();
		m_dwZdCode = pZd->GetCode();
		m_bSizeof = pZd->GetSizeof();
		m_OrderInItem = pZd->GetOrderInItem();
		m_strUnit = pZd->GetUnit();
		m_fScaleMax = pZd->GetScaleMax();
		m_fScaleMin = pZd->GetScaleMin();

		m_pCurStation = pZd->GetParent();
		m_pCurZdObject  = pZd;
		m_OperateFlag = RECORD_EDIT;
		m_strCaption.Format("%s [������Ϣ]",pZd->GetName());

	}
}

void CUdfAddZdObjectDlg::SetRtuStation(CRtuStation *pStation)
{
	m_pCurStation = pStation;
	if (pStation)
		m_stName = pStation->GetName();
}