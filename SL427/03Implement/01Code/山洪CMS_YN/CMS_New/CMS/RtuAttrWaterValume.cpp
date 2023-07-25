// RtuAttrWaterValume.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuAttrWaterValume.h"

#define W_TIMER_CLZ_QUERY    0x01      //��ֵ����ѯ
#define W_TIMER_ALART_QUERY  0x02      //ʣ��ˮ������ֵ��ѯ

// CRtuAttrWaterValume �Ի���
IMPLEMENT_DYNAMIC(CRtuAttrWaterValume, CDialog)
CRtuAttrWaterValume::CRtuAttrWaterValume(CWnd* pParent /*=NULL*/)
	: CDialog(CRtuAttrWaterValume::IDD, pParent)
{
	m_blResult = FALSE;
	m_pSubStation = NULL;
	m_pParentDlg = NULL;
}

CRtuAttrWaterValume::~CRtuAttrWaterValume()
{

}

void CRtuAttrWaterValume::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_EDIT_CZL,m_Edit_Czl);
	DDX_Control(pDX,IDC_EDIT_REMAIN,m_Edit_Remain);
	DDX_Control(pDX,IDC_EDIT_ALERTV,m_Edit_Alert);
	DDX_Control(pDX,IDC_BTN_QUERYCZL,m_btnQueryCzl);
	DDX_Control(pDX,IDC_BTN_SETCZL,m_btnSetCzl);
	DDX_Control(pDX,IDC_BTN_QUERYSYALARM,m_btnQueryRemainAlert);
	DDX_Control(pDX,IDC_BTN_SETSYALARM,m_btnSetRemainAlert);
}


BEGIN_MESSAGE_MAP(CRtuAttrWaterValume, CDialog)
	ON_BN_CLICKED(IDC_BTN_QUERYCZL,&CRtuAttrWaterValume::OnBnClickQueryCzl)
	ON_BN_CLICKED(IDC_BTN_SETCZL,&CRtuAttrWaterValume::OnBnClickSetCzl)
	ON_BN_CLICKED(IDC_BTN_QUERYSYALARM,&CRtuAttrWaterValume::OnBnClickQueryRemainAlert)
	ON_BN_CLICKED(IDC_BTN_SETSYALARM,&CRtuAttrWaterValume::OnBnClickSetRemainAlert)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CRtuAttrWaterValume ��Ϣ�������
void CRtuAttrWaterValume::SetParentDlg(CRtuPropertyDlg* pDlg)
{
	m_pParentDlg = pDlg;
}

BOOL CRtuAttrWaterValume::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;
}

void CRtuAttrWaterValume::Update()										//���½������ʾ��Ϣ
{
	if (m_pParentDlg)
		m_pSubStation = m_pParentDlg->GetRtuStation();

	if (m_pSubStation != NULL)
	{
		DWORD tValue = m_pSubStation->GetWaterValumePara()->GetRechargeValume();
		CString strText = "";
		
		strText.Format("%d",tValue);
		m_Edit_Czl.SetWindowText(strText);

		UINT64 rValue = m_pSubStation->GetWaterValumePara()->GetRemainValume();
		BYTE   bSigFlag = m_pSubStation->GetWaterValumePara()->GetSignBit();
		if (bSigFlag != 0)
			strText.Format("-%d",rValue);
		else
			strText.Format("%d",rValue);

		m_Edit_Remain.SetWindowText(strText);

		tValue = m_pSubStation->GetWaterValumePara()->GetRemainAlertValume();
		strText.Format("%d",tValue);
		m_Edit_Alert.SetWindowText(strText);

	}
}

void CRtuAttrWaterValume::UpdateDlg(int attrType,BOOL blResult)									//���յ����ĺ󡢸��½���
{
	m_blResult = blResult;
	if (attrType == rtu_attr_water_alert_call)
		SendMessage(WM_TIMER,W_TIMER_ALART_QUERY,NULL);
	else if (attrType == rtu_attr_water_recharge_call)
		SendMessage(WM_TIMER,W_TIMER_CLZ_QUERY,NULL);
}


void CRtuAttrWaterValume::OnTimer(UINT_PTR nIDEvent)
{

	CString strLogMessge = "";
	_Log_level_type _logLevle = Normal_Log_Message;

	switch (nIDEvent)
	{
	case W_TIMER_CLZ_QUERY:
		{
			if (m_blResult == TRUE)
				strLogMessge = "��ѯң���ն�����ɹ���ֵ��������ʣ��ˮ�� �ɹ�!";
			else if( m_blResult == FALSE)
			{
				strLogMessge = "��ѯң���ն�����ɹ���ֵ��������ʣ��ˮ�� ��ʱ!";
				_logLevle = Warning_log_Message;
			}
		}
		break;
	case W_TIMER_ALART_QUERY:
		{
			if (m_blResult == TRUE)
				strLogMessge = "��ѯң���ն˵�ʣ��ˮ���ͱ���ֵ �ɹ�!";
			else if( m_blResult == FALSE)
			{
				strLogMessge = "��ѯң���ն˵�ʣ��ˮ���ͱ���ֵ ��ʱ!";
				_logLevle = Warning_log_Message;
			}
		}
		break;
	}
	KillTimer(nIDEvent);
	SetWndsEnable(TRUE);

	if (m_pParentDlg && m_pParentDlg->GetSafeHwnd())
		m_pParentDlg->AddLog(strLogMessge,NULL,_logLevle);

	if (m_pSubStation)
		m_pSubStation->InsertLogMessage(strLogMessge,_logLevle,NULL);

	CDialog::OnTimer(nIDEvent);
}

void CRtuAttrWaterValume::KillAllTimers()
{
	KillTimer(W_TIMER_ALART_QUERY);
	KillTimer(W_TIMER_CLZ_QUERY);
	SetWndsEnable(TRUE);
}

void CRtuAttrWaterValume::SetWndsEnable(BOOL blEnable)
{
	m_btnQueryCzl.EnableWindow(blEnable);
	m_btnSetCzl.EnableWindow(blEnable);
	m_btnQueryRemainAlert.EnableWindow(blEnable);
	m_btnSetRemainAlert.EnableWindow(blEnable);

	m_Edit_Czl.EnableWindow(blEnable);
	m_Edit_Alert.EnableWindow(blEnable);
}

void CRtuAttrWaterValume::OnBnClickQueryCzl()
{
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CPacketFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //���ɳ�ʼ���·�֡����֡������Ϣ

			m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_55H(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_CLZ_QUERY,nElapse,NULL);
		}
	}
}

void CRtuAttrWaterValume::OnBnClickSetCzl()
{
	UpdateData(TRUE);
	CString strCZl = "",strMsg="";
    m_Edit_Czl.GetWindowText(strCZl);

	DWORD dwCzl = (DWORD)atol(strCZl);
	if (dwCzl<0 || dwCzl> 99999999)
	{
		strMsg.Format("����ĳ�ֵ��ֵ������Χ(0-99999999)������������!");
		MessageBox(strMsg,"��ʾ",MB_OK|MB_ICONWARNING);

		m_Edit_Czl.SetFocus();
		return;
	}

	if (m_pSubStation)
	{
		m_pSubStation->GetWaterValumePara()->SetRechargeValume(dwCzl);

		if (m_pSubStation->blEnbaleTrans() == true)
		{
			CPacketFrameFormat appFrame ;
			m_pSubStation->BuildBasicPacket(appFrame);

			
			m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_15H(&appFrame);
		}
	}
}

void CRtuAttrWaterValume::OnBnClickQueryRemainAlert()
{
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CPacketFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //���ɳ�ʼ���·�֡����֡������Ϣ

			m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_56H(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_ALART_QUERY,nElapse,NULL);
		}
	}
}

void CRtuAttrWaterValume::OnBnClickSetRemainAlert()
{
	UpdateData(TRUE);
	CString strAlert = "",strMsg="";
	m_Edit_Alert.GetWindowText(	strAlert);

	DWORD dwAlert = (DWORD)atol(strAlert);
	if (dwAlert<0 || dwAlert> 999999)
	{
		strMsg.Format("����ĳ�ֵ��ֵ������Χ(0-999999)������������!");
		MessageBox(strMsg,"��ʾ",MB_OK|MB_ICONWARNING);

		m_Edit_Alert.SetFocus();
		return;
	}

	if (m_pSubStation)
	{
		m_pSubStation->GetWaterValumePara()->SetRemainAlertValue(dwAlert);

		if (m_pSubStation->blEnbaleTrans() == true)
		{
			CPacketFrameFormat appFrame ;
			m_pSubStation->BuildBasicPacket(appFrame);

			m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_16H(&appFrame);
		}
	}

}