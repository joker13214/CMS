// RtuAttrHydrInTGW.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuAttrHydrInTGW.h"


// CRtuAttrHydrInTGW �Ի���
#define W_TIMER_WATERLEVEL_PARA_QUERY 0x01 //ˮλ������ѯ
#define W_TIMER_WATERLEVEL_REAL_QUERY 0x02  //ˮλʵʱ���ݲ�ѯ
IMPLEMENT_DYNAMIC(CRtuAttrHydrInTGW, CDialog)

CRtuAttrHydrInTGW::CRtuAttrHydrInTGW(CWnd* pParent /*=NULL*/)
	: CDialog(CRtuAttrHydrInTGW::IDD, pParent)
{
	m_blResult = FALSE;
	m_pSubStation = NULL;
	m_pParentDlg = NULL;

}

CRtuAttrHydrInTGW::~CRtuAttrHydrInTGW()
{
}

void CRtuAttrHydrInTGW::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_CBX_TIMETYPE ,m_wndTimeTypeCbx);	
	DDX_Control(pDX,IDC_D_START ,m_wndDateStartDtc);
	DDX_Control(pDX,IDC_D_END ,m_wndDateEndDtc);
	DDX_Control(pDX,IDC_STATIC_START ,m_wndTimeStartLabel);
	DDX_Control(pDX,IDC_STATIC_END ,m_wndTimeEndLabel);
}

BOOL CRtuAttrHydrInTGW::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;
}


BEGIN_MESSAGE_MAP(CRtuAttrHydrInTGW, CDialog)
	ON_CBN_SELCHANGE(IDC_CBX_TIMETYPE,      &CRtuAttrHydrInTGW::OnCbnSelChangeTimeType)
	ON_BN_CLICKED(IDC_BTN_QUERYGWT,		&CRtuAttrHydrInTGW::OnBtnClickedQueryGWT)
    ON_WM_TIMER()
END_MESSAGE_MAP()


// CRtuAttrHydrInTGW ��Ϣ�������
void CRtuAttrHydrInTGW::SetParentDlg(CRtuPropertyDlg* pDlg)
{
	m_pParentDlg = pDlg;

}

void CRtuAttrHydrInTGW::Update()
{

}

void CRtuAttrHydrInTGW::UpdateDlg(int attrType,BOOL blResult)			//���յ����ĺ󡢸��½���
{
	m_blResult = blResult;
	if (attrType == rtu_attr_waterlevel_data_call)
		SendMessage(WM_TIMER,W_TIMER_WATERLEVEL_REAL_QUERY,NULL);
	else if (attrType == rtu_attr_waterlevel_call)
		SendMessage(WM_TIMER,W_TIMER_WATERLEVEL_PARA_QUERY,NULL);

}

void CRtuAttrHydrInTGW::KillAllTimers()
{
	KillTimer(W_TIMER_WATERLEVEL_PARA_QUERY);
	KillTimer(W_TIMER_WATERLEVEL_REAL_QUERY);
	SetWndsEnable(TRUE);
}


void CRtuAttrHydrInTGW::SetWndsEnable(BOOL blEnable)
{
}

void CRtuAttrHydrInTGW::InitGridItems()
{

}

void CRtuAttrHydrInTGW::OnCbnSelChangeTimeType()
{
	if (m_wndTimeTypeCbx.GetSafeHwnd())
	{
		SYSTEMTIME sysTime;
		int nSel = m_wndTimeTypeCbx.GetCurSel();
		if (CB_ERR == nSel) return;

		switch(nSel)
		{
		case 0://��ǰʱ��
			m_wndTimeStartLabel.SetWindowText("��ǰʱ��");
			m_wndTimeEndLabel.ShowWindow(SW_HIDE);
			m_wndDateEndDtc.ShowWindow(SW_HIDE);

			m_wndDateStartDtc.SetFormat("yyyy��MM��dd�� HHʱmm��ss��");
			memset(&sysTime, 0, sizeof(sysTime));
			::GetLocalTime(&sysTime);
			VERIFY(m_wndDateStartDtc.SetTime(&sysTime));
			break;
		case 1://�趨ʱ��
			m_wndTimeStartLabel.SetWindowText("ָ��ʱ��");
			m_wndTimeEndLabel.ShowWindow(SW_HIDE);
			m_wndDateEndDtc.ShowWindow(SW_HIDE);

			m_wndDateStartDtc.SetFormat("yyyy��MM��dd�� HHʱmm��ss��");	
			memset(&sysTime, 0, sizeof(sysTime));
			::GetLocalTime(&sysTime);
			VERIFY(m_wndDateStartDtc.SetTime(&sysTime));
			break;
		case 2://��ʷ��ѯ
			m_wndTimeStartLabel.SetWindowText("��ʼʱ��");
			m_wndTimeEndLabel.SetWindowText("����ʱ��");
			m_wndTimeEndLabel.ShowWindow(SW_SHOW);
			m_wndDateEndDtc.ShowWindow(SW_SHOW);

			m_wndDateStartDtc.SetFormat("yyyy��MM��dd�� HHʱmm��ss��");
			m_wndDateEndDtc.SetFormat("yyyy��MM��dd�� HHʱmm��ss��");
			memset(&sysTime, 0, sizeof(sysTime));
			::GetLocalTime(&sysTime);
			VERIFY(m_wndDateStartDtc.SetTime(&sysTime));
			VERIFY(m_wndDateEndDtc.SetTime(&sysTime));
			break;
		case 3://��С��ֵ
			m_wndTimeStartLabel.SetWindowText("��С��ֵ");
			m_wndTimeEndLabel.ShowWindow(SW_HIDE);
			m_wndDateEndDtc.ShowWindow(SW_HIDE);

			m_wndDateStartDtc.SetFormat("yyyy��MM��dd�� HHʱmm��ss��");
			memset(&sysTime, 0, sizeof(sysTime));
			::GetLocalTime(&sysTime);
			VERIFY(m_wndDateStartDtc.SetTime(&sysTime));
			break;
		case 4://���ֵ
			m_wndTimeStartLabel.SetWindowText("���ֵ");
			m_wndTimeEndLabel.ShowWindow(SW_HIDE);
			m_wndDateEndDtc.ShowWindow(SW_HIDE);

			m_wndDateStartDtc.SetFormat("yyyy��MM��dd�� HHʱmm��ss��");
			memset(&sysTime, 0, sizeof(sysTime));
			::GetLocalTime(&sysTime);
			VERIFY(m_wndDateStartDtc.SetTime(&sysTime));
			break;

		}
	}
}

void CRtuAttrHydrInTGW::OnBtnClickedQueryGWT()
{
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CHydrSendFrame appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //���ɳ�ʼ���·�֡����֡������Ϣ
			m_pSubStation->GetSysDataTrans_Hydr()->SCSW_hydrdata_EN_C(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_WATERLEVEL_PARA_QUERY, nElapse, NULL);
		}
	}
}


void CRtuAttrHydrInTGW::OnTimer(UINT_PTR nIDEvent)
{
	CString strLogMessge = "";
	_Log_level_type _logLevle = Normal_Log_Message;

	switch (nIDEvent)
	{
	case W_TIMER_WATERLEVEL_REAL_QUERY:
		{
			if (m_blResult == TRUE)
				strLogMessge = "��ѯң���ն˵ĵ���ˮ˲ʱ�¶�ֵ �ɹ�!";
			else if( m_blResult == FALSE)
			{
				strLogMessge = "��ѯң���ն˵ĵ���ˮ˲ʱ�¶�ֵ ��ʱ!";
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