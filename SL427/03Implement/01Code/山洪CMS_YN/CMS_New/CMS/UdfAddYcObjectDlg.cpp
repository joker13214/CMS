// UdfAddYcObjectDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CMS.h"
#include "UdfAddYcObjectDlg.h"


// CUdfAddYcObjectDlg �Ի���

IMPLEMENT_DYNAMIC(CUdfAddYcObjectDlg, CDialog)

CUdfAddYcObjectDlg::CUdfAddYcObjectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUdfAddYcObjectDlg::IDD, pParent)
	, m_stName(_T(""))
	, m_sYCName(_T(""))
	, m_dwYcCode(0)
	, m_DisplayBits(7)
	, m_PointBits(1)
	, m_sUnit(_T(""))
	, m_fScaleMax(0)
	, m_fScaleMin(0)
{
	m_pCurYcObject = NULL;
	m_pCurStation = NULL;
	m_OperateFlag  = RECORD_NEW;
	m_YcType = 0;

}

CUdfAddYcObjectDlg::~CUdfAddYcObjectDlg()
{
}

void CUdfAddYcObjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_STNAME, m_stName);
	DDV_MaxChars(pDX, m_stName, 50);
	DDX_Text(pDX, IDC_EDIT_YCNAME, m_sYCName);
	DDV_MaxChars(pDX, m_sYCName, 50);
	DDX_Text(pDX, IDC_EDIT_YCCODE, m_dwYcCode);
	DDX_Control(pDX, IDC_CMB_YCTYPE, m_wndCmbYcType);
	DDX_Text(pDX, IDC_EDIT_DISPLAYBITS, m_DisplayBits);
	DDX_Text(pDX, IDC_EDIT_POINTBITS, m_PointBits);
	DDX_Text(pDX, IDC_EDIT_UNIT, m_sUnit);
	DDX_Text(pDX, IDC_EDIT_SCALEMAX, m_fScaleMax);
	DDX_Text(pDX, IDC_EDIT_SCALEMIN, m_fScaleMin);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CUdfAddYcObjectDlg, CDialog)
END_MESSAGE_MAP()


// CUdfAddYcObjectDlg ��Ϣ�������

BOOL CUdfAddYcObjectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	if(m_wndCmbYcType.GetSafeHwnd())
	{
		m_wndCmbYcType.ResetContent();

		int nIdx  = 0;
		nIdx = m_wndCmbYcType.AddString("��ѡ��");
		m_wndCmbYcType.SetItemData(nIdx,0);

		nIdx = m_wndCmbYcType.AddString("ˮλ");
		m_wndCmbYcType.SetItemData(nIdx,1);

		nIdx = m_wndCmbYcType.AddString("����");
		m_wndCmbYcType.SetItemData(nIdx,2);

		nIdx = m_wndCmbYcType.AddString("ˮѹ");
		m_wndCmbYcType.SetItemData(nIdx,3);

		nIdx = m_wndCmbYcType.AddString("ˮ��");
		m_wndCmbYcType.SetItemData(nIdx,4);

		nIdx = m_wndCmbYcType.AddString("�ۺ�");
		m_wndCmbYcType.SetItemData(nIdx,5);

		nIdx = m_wndCmbYcType.AddString("����");
		m_wndCmbYcType.SetItemData(nIdx,6);


		bool bFind = false;
		int nSel = 0;
		for (int n= 0; n < m_wndCmbYcType.GetCount(); n ++)
		{
			if (m_wndCmbYcType.GetItemData(n) == m_YcType)
			{
				nSel = n;
				bFind = true;
				break;
			}
		}

		if (bFind)
			m_wndCmbYcType.SetCurSel(nSel);
		else
			m_wndCmbYcType.SetCurSel(0);
		
	}
	if (m_OperateFlag == RECORD_EDIT && !m_strCaption.IsEmpty())
	{
		SetWindowText(m_strCaption);
	}

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CUdfAddYcObjectDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
    UpdateData(TRUE);


	m_sYCName.Trim();
	if (m_sYCName.IsEmpty())
	{
		MessageBox("ң�����Ʋ���Ϊ��,������!","",MB_OK|MB_ICONERROR);
		return;

	}

	if (m_dwYcCode <= 0)
	{
		MessageBox("ң���벻��Ϊ���С����,����������!","",MB_OK|MB_ICONERROR);
		return;
	}


	if (m_wndCmbYcType.GetSafeHwnd() && m_wndCmbYcType.GetCurSel() != CB_ERR)
	{
		int nSel = m_wndCmbYcType.GetCurSel();
		m_YcType = (BYTE)m_wndCmbYcType.GetItemData(nSel);
	}

	

	CDialog::OnOK();
}


void CUdfAddYcObjectDlg::Update(CRtuYcObject* pYc)
{
	if (pYc)
	{
		m_fScaleMax = pYc->GetMaxValue();
		m_fScaleMin = pYc->GetMinValue();
		m_stName = pYc->GetParent()->GetName();
		m_sYCName = pYc->GetName();
		m_dwYcCode = pYc->GetCode();
		m_DisplayBits = pYc->GetDisplayBits();
		m_PointBits = pYc->GetPointBits();
		m_sUnit = pYc->GetUnit();
		m_YcType = pYc->GetXType();

		m_pCurYcObject = pYc;
		m_pCurStation = pYc->GetParent();
		m_OperateFlag = RECORD_EDIT;
		m_strCaption.Format("%s [������Ϣ]",pYc->GetName());

	}
}

void CUdfAddYcObjectDlg::SetRtuStation(CRtuStation* pStation)
{
	m_pCurStation = pStation;
	if (pStation)
		m_stName = pStation->GetName();
}