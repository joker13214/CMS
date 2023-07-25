// RtuAttrDayHisQuery.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuAttrDayHisQuery.h"


#define W_TIMER_QUERYDAYHIS 0x01     //��ѯң���ն���ʷ�ռ�¼

// CRtuAttrDayHisQuery �Ի���
IMPLEMENT_DYNAMIC(CRtuAttrDayHisQuery, CDialog)
CRtuAttrDayHisQuery::CRtuAttrDayHisQuery(CWnd* pParent /*=NULL*/)
	: CDialog(CRtuAttrDayHisQuery::IDD, pParent)
{
	m_blResult = FALSE;
	m_pSubStation = NULL;
	m_pParentDlg = NULL;

	m_bDataType = 0;
	m_bZBtype = 0;
	m_bQueryNo = 0;
}

CRtuAttrDayHisQuery::~CRtuAttrDayHisQuery()
{

}

void CRtuAttrDayHisQuery::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_ZBTYPE, m_wndZBTypeCmb);
	DDX_Control(pDX, IDC_DT_QUERY, m_wndQueryDate);
	DDX_Control(pDX, IDC_CMB_DATATYPE, m_wndDataTypeCmb);
	DDX_Control(pDX, IDC_CMB_QUERYNO, m_wndQueryNoCmb);
	DDX_Control(pDX, IDC_BTN_QUERYHIS, m_btnQueryHis);
	DDX_Control(pDX, IDC_EDIT_DATAINFO, m_EditDataInfo);
}


BEGIN_MESSAGE_MAP(CRtuAttrDayHisQuery, CDialog)
	ON_BN_CLICKED(IDC_BTN_QUERYHIS, &CRtuAttrDayHisQuery::OnBnClickedBtnQueryhis)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_CMB_ZBTYPE, &CRtuAttrDayHisQuery::OnCbnSelchangeCmbZbtype)
	ON_CBN_SELCHANGE(IDC_CMB_DATATYPE, &CRtuAttrDayHisQuery::OnCbnSelchangeCmbDatatype)
	ON_CBN_SELCHANGE(IDC_CMB_QUERYNO, &CRtuAttrDayHisQuery::OnCbnSelchangeCmbQueryno)
END_MESSAGE_MAP()


// CRtuAttrDayHisQuery ��Ϣ�������
void CRtuAttrDayHisQuery::SetParentDlg(CRtuPropertyDlg* pDlg)
{
	m_pParentDlg = pDlg;
}

void CRtuAttrDayHisQuery::Update()										//���½������ʾ��Ϣ
{
	if (m_pParentDlg)
		m_pSubStation = m_pParentDlg->GetRtuStation();

	if (m_wndQueryDate.GetSafeHwnd())
	{
		m_wndQueryDate.SetFormat("yyyy��MM��dd��");
		SYSTEMTIME st;
		GetLocalTime(&st);
		m_wndQueryDate.SetTime(&st);
	}

	if (m_pSubStation != NULL)
	{

	}
}

void CRtuAttrDayHisQuery::UpdateDlg(BOOL blResult,CString strData)			//���յ����ĺ󡢸��½���
{
	m_blResult = blResult;

	if (m_blResult == TRUE)
		SendMessage(WM_TIMER,W_TIMER_QUERYDAYHIS);

	m_EditDataInfo.SetWindowText(strData);
	m_EditDataInfo.Invalidate(TRUE);
}

void CRtuAttrDayHisQuery::KillAllTimers()
{
	KillTimer(W_TIMER_QUERYDAYHIS);
	SetWndsEnable(TRUE);

	if (m_EditDataInfo.GetSafeHwnd())
		m_EditDataInfo.SetWindowText("");
}


void CRtuAttrDayHisQuery::OnBnClickedBtnQueryhis()
{
	UpdateData(TRUE);
	if(m_pSubStation)
	{
		m_pSubStation->GetDayHistoryObject()->SetDataType(m_bDataType);
		m_pSubStation->GetDayHistoryObject()->SetQueryNo(m_bQueryNo);
		m_pSubStation->GetDayHistoryObject()->SetZBType(m_bZBtype);

		SYSTEMTIME st;
		m_wndQueryDate.GetTime(&st);
		m_pSubStation->GetDayHistoryObject()->SetDay(st.wDay);
		m_pSubStation->GetDayHistoryObject()->SetMonth(st.wMonth);
		m_pSubStation->GetDayHistoryObject()->SetYear(st.wYear);


		if (m_pSubStation->blEnbaleTrans() == true)
		{

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			//���ɳ�ʼ���·�֡����֡������Ϣ
			CPacketFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame); 

			m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_5CH(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_QUERYDAYHIS,nElapse,NULL);
		}
	}
}

BOOL CRtuAttrDayHisQuery::OnInitDialog()
{
	CDialog::OnInitDialog();

	int ItemIdx = 0;
	if (m_wndZBTypeCmb.GetSafeHwnd())
	{
		m_wndZBTypeCmb.ResetContent();
		ItemIdx = m_wndZBTypeCmb.AddString("��������");
		m_wndZBTypeCmb.SetItemData(ItemIdx,(DWORD_PTR)2);

		ItemIdx = m_wndZBTypeCmb.AddString("�������");
		m_wndZBTypeCmb.SetItemData(ItemIdx,(DWORD_PTR)1);

		if (m_wndZBTypeCmb.GetCount())
			m_wndZBTypeCmb.SetCurSel(0);

		OnCbnSelchangeCmbZbtype();
	}

	if (m_wndDataTypeCmb.GetSafeHwnd())
	{
		m_wndDataTypeCmb.ResetContent();

		//ˮλ;����;ˮѹ;ˮ��;����;ģ����;
		ItemIdx = m_wndDataTypeCmb.AddString("ˮλ");
		m_wndDataTypeCmb.SetItemData(ItemIdx,(DWORD_PTR)C_CMD_06_WLPARA);

		ItemIdx = m_wndDataTypeCmb.AddString("����");
		m_wndDataTypeCmb.SetItemData(ItemIdx,(DWORD_PTR)C_CMD_07_FLOWPARA);

		ItemIdx = m_wndDataTypeCmb.AddString("ˮѹ");
		m_wndDataTypeCmb.SetItemData(ItemIdx,(DWORD_PTR)C_CMD_08_WPPARA);

		ItemIdx = m_wndDataTypeCmb.AddString("ˮ��");
		m_wndDataTypeCmb.SetItemData(ItemIdx,(DWORD_PTR)C_CMD_09_WQPARA);

		ItemIdx = m_wndDataTypeCmb.AddString("����");
		m_wndDataTypeCmb.SetItemData(ItemIdx,(DWORD_PTR)C_CMD_11_RAINPARA);

		ItemIdx = m_wndDataTypeCmb.AddString("ģ����");
		m_wndDataTypeCmb.SetItemData(ItemIdx,(DWORD_PTR)C_CMD_12_MONIPARA);

		if (m_wndDataTypeCmb.GetCount())
			m_wndDataTypeCmb.SetCurSel(0);

		OnCbnSelchangeCmbDatatype();
	}


	if (m_wndQueryNoCmb.GetSafeHwnd())
	{
		m_wndQueryNoCmb.ResetContent();

		for (int i = 0; i <= 7 ;i ++)
		{
			CString strTemp = "";
			strTemp.Format("%d",i);
			ItemIdx = m_wndQueryNoCmb.AddString(strTemp);
			m_wndQueryNoCmb.SetItemData(ItemIdx,(DWORD_PTR)i);
		}
		if (m_wndQueryNoCmb.GetCount())
			m_wndQueryNoCmb.SetCurSel(0);

		OnCbnSelchangeCmbQueryno();
	}

	return TRUE;  
	// return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CRtuAttrDayHisQuery::OnTimer(UINT_PTR nIDEvent)
{
	CString strLogMessge = "";
	_Log_level_type _logLevle = Normal_Log_Message;

	if (nIDEvent == W_TIMER_QUERYDAYHIS)
	{
		if (m_blResult == TRUE)
			strLogMessge = "��ѯң���ն˵���ʷ�ռ�¼ �ɹ�!";
		else if( m_blResult == FALSE)
		{
			strLogMessge = "��ѯң���ն˵���ʷ�ռ�¼ ��ʱ!";
			_logLevle = Warning_log_Message;
		}
	}
	KillTimer(nIDEvent);
	SetWndsEnable(TRUE);

	if (m_pParentDlg && m_pParentDlg->GetSafeHwnd())
		m_pParentDlg->AddLog(strLogMessge,NULL,_logLevle);

	if (m_pSubStation)
		m_pSubStation->InsertLogMessage(strLogMessge,_logLevle,NULL);

	CDialog::OnTimer(nIDEvent);
}


void CRtuAttrDayHisQuery::SetWndsEnable(BOOL blEnable)
{
	if (m_wndDataTypeCmb.GetSafeHwnd())
		m_wndDataTypeCmb.EnableWindow(blEnable);
	if (m_wndZBTypeCmb.GetSafeHwnd())
		m_wndZBTypeCmb.EnableWindow(blEnable);
	if (m_wndQueryNoCmb.GetSafeHwnd())
		m_wndQueryNoCmb.EnableWindow(blEnable);
	if (m_wndQueryDate.GetSafeHwnd())
		m_wndQueryDate.EnableWindow(blEnable);
	if (m_btnQueryHis.GetSafeHwnd())
		m_btnQueryHis.EnableWindow(blEnable);

	
}

void CRtuAttrDayHisQuery::OnCbnSelchangeCmbZbtype()
{
	int nSel = m_wndZBTypeCmb.GetCurSel();
	if (nSel != CB_ERR)
		m_bZBtype = (BYTE)m_wndZBTypeCmb.GetItemData(nSel);

}

void CRtuAttrDayHisQuery::OnCbnSelchangeCmbDatatype()
{
	int nSel = m_wndDataTypeCmb.GetCurSel();
	if (nSel != CB_ERR)
		m_bDataType = (BYTE)m_wndDataTypeCmb.GetItemData(nSel);
}

void CRtuAttrDayHisQuery::OnCbnSelchangeCmbQueryno()
{
	int nSel = m_wndQueryNoCmb.GetCurSel();
	if (nSel != CB_ERR)
		m_bQueryNo = (BYTE)m_wndQueryNoCmb.GetItemData(nSel);
}
