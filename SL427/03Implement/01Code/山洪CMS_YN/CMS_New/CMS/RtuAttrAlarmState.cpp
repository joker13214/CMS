// RtuAttrAlarmState.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuAttrAlarmState.h"


#define  W_TIMER_EXALARM_CALL 0x01             //��ѯң���ն���չ����״̬
#define  W_TIMER_ALARM_CALL   0x02             //��ѯң���ն˵ı���״̬

// CRtuAttrAlarmState �Ի���
IMPLEMENT_DYNAMIC(CRtuAttrAlarmState, CDialog)
CRtuAttrAlarmState::CRtuAttrAlarmState(CWnd* pParent /*=NULL*/)
	: CDialog(CRtuAttrAlarmState::IDD, pParent)
{
	m_pSubStation = NULL;
	m_blResult = FALSE;
	m_pParentDlg = NULL;
}

CRtuAttrAlarmState::~CRtuAttrAlarmState()
{
}

void CRtuAttrAlarmState::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_EXALARM, m_wndExAlarmCmb);
	DDX_Control(pDX, IDC_CMB_RTUSTATE, m_wndRtuStateCmb);
	DDX_Control(pDX, IDC_CMB_RTUALARM, m_wndAlarmCmb);
	DDX_Control(pDX, IDC_BTN_ALARMCALL, m_btnAlarmCall);
	DDX_Control(pDX, IDC_BTN_EXALARMCALL, m_btnExAlarmCall);
	DDX_Control(pDX, IDC_EDIT_ALARMVALUE, m_Edit_AlarmValue);
	DDX_Control(pDX, IDC_EDIT_EXALARMVALUE, m_Edit_ExAlarmValue);
	DDX_Control(pDX, IDC_EDIT_STATEVALUE, m_Edit_StateValue);
}


BEGIN_MESSAGE_MAP(CRtuAttrAlarmState, CDialog)
	ON_CBN_SELCHANGE(IDC_CMB_RTUALARM, &CRtuAttrAlarmState::OnCbnSelchangeCmbRtualarm)
	ON_CBN_SELCHANGE(IDC_CMB_EXALARM, &CRtuAttrAlarmState::OnCbnSelchangeCmbExalarm)
	ON_CBN_SELCHANGE(IDC_CMB_RTUSTATE, &CRtuAttrAlarmState::OnCbnSelchangeCmbRtustate)
	ON_BN_CLICKED(IDC_BTN_EXALARMCALL,&CRtuAttrAlarmState::OnBnClickExAlarmCall)
	ON_BN_CLICKED(IDC_BTN_ALARMCALL,&CRtuAttrAlarmState::OnBnClickAlarmCall)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CRtuAttrAlarmState ��Ϣ�������


void CRtuAttrAlarmState::Update()
{
	if (m_pParentDlg)
		m_pSubStation = m_pParentDlg->GetRtuStation();

	if (m_wndAlarmCmb.GetSafeHwnd())
	{
		m_wndAlarmCmb.SetCurSel(-1);
		OnCbnSelchangeCmbRtualarm();
	}

	if (m_wndExAlarmCmb.GetSafeHwnd())
	{
		m_wndExAlarmCmb.SetCurSel(-1);
		OnCbnSelchangeCmbExalarm();
	}

	if (m_wndRtuStateCmb.GetSafeHwnd())
	{
		m_wndRtuStateCmb.SetCurSel(-1);
		OnCbnSelchangeCmbRtustate();
	}

	if (m_pSubStation != NULL)
	{
		BYTE nAppProVer = m_pSubStation->GetAppProVer();
		if (nAppProVer == APP_PROTOCOL_V1_0)
		{
			m_btnExAlarmCall.ShowWindow(SW_SHOW);
		}
		else /*if (nAppProVer == APP_PROTOCOL_V2_0)*/
		{
			m_btnExAlarmCall.ShowWindow(SW_HIDE);
		}			 
	}
}



void CRtuAttrAlarmState::SetParentDlg(CRtuPropertyDlg* pDlg)
{
	m_pParentDlg = pDlg;
}
BOOL CRtuAttrAlarmState::OnInitDialog()
{
	CDialog::OnInitDialog();

	
	if (m_wndExAlarmCmb.GetSafeHwnd())
		{
			m_wndExAlarmCmb.ResetContent();
			
			int Index = m_wndExAlarmCmb.AddString("��������״̬");
			m_wndExAlarmCmb.SetItemData(Index,0);
	
			Index = m_wndExAlarmCmb.AddString("ģ��������״̬");
			m_wndExAlarmCmb.SetItemData(Index,1);
		}
	
		if (m_wndRtuStateCmb.GetSafeHwnd())
		{
			m_wndRtuStateCmb.ResetContent();
	
			int Index = 0 ;
			Index = m_wndRtuStateCmb.AddString("�ն˹���ģʽ");
			m_wndRtuStateCmb.SetItemData(Index,1);
	
			//Index = m_wndRtuStateCmb.AddString("IC�������Ƿ���Ч");
			//m_wndRtuStateCmb.SetItemData(Index,2);
			
			//Index = m_wndRtuStateCmb.AddString("��ֵ�����Ƿ�Ͷ��");
			//m_wndRtuStateCmb.SetItemData(Index,3);
	
			//Index = m_wndRtuStateCmb.AddString("ˮ�ù���״̬");
			//m_wndRtuStateCmb.SetItemData(Index,4);
	
			//Index = m_wndRtuStateCmb.AddString("�ն�����״̬");
			//m_wndRtuStateCmb.SetItemData(Index,5);
	
			Index = m_wndRtuStateCmb.AddString("��Դ����״̬");
			m_wndRtuStateCmb.SetItemData(Index,6);
		}

		if (m_wndAlarmCmb.GetSafeHwnd())
		{
			m_wndAlarmCmb.ResetContent();

			int Index = 0 ;
			//Index = m_wndAlarmCmb.AddString("������״̬");
			//m_wndAlarmCmb.SetItemData(Index,0);

			Index = m_wndAlarmCmb.AddString("���ص�ѹ״̬");
			m_wndAlarmCmb.SetItemData(Index,1);

			Index = m_wndAlarmCmb.AddString("ˮλ���ޱ���״̬");
			m_wndAlarmCmb.SetItemData(Index,2);

			Index = m_wndAlarmCmb.AddString("ˮѹ���ޱ���״̬");
			m_wndAlarmCmb.SetItemData(Index,3);

			Index = m_wndAlarmCmb.AddString("ˮ�ʳ��ޱ���״̬");
			m_wndAlarmCmb.SetItemData(Index,4);

			//Index = m_wndAlarmCmb.AddString("ˮ������״̬");
			//m_wndAlarmCmb.SetItemData(Index,5);

			//Index = m_wndAlarmCmb.AddString("ˮ��״̬");
			//m_wndAlarmCmb.SetItemData(Index,6);

			//Index = m_wndAlarmCmb.AddString("ʣ��ˮ�������ޱ���״̬");
			//m_wndAlarmCmb.SetItemData(Index,7);
		}

	return TRUE;  
	// return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CRtuAttrAlarmState::OnCbnSelchangeCmbRtualarm()
{
	m_Edit_AlarmValue.SetWindowText("");

	if (!m_pSubStation) return;

	int nSel = m_wndAlarmCmb.GetCurSel();
	if (nSel != CB_ERR)
	{
		BYTE ItemData = (BYTE)m_wndAlarmCmb.GetItemData(nSel);
		CRTUAlarm_State*  pAlarmOBJ = m_pSubStation->GetRtuAlarm_State();

		bool bitValue = false;
		CString strValue = "";
		bitValue = pAlarmOBJ->GetBitValue(ItemData);
		if (ItemData == 0)
		{
			if (bitValue == false)
				strValue = "����";
			else
				strValue = "ͣ��";

		}
		else if (ItemData == 1)
		{
			if (false == bitValue)
				strValue = "����";
			else
				strValue = "��ѹ��";
		}
		else if (ItemData == 2)
		{
			if (true == bitValue)
				strValue = "����";
			else
				strValue = "����";
		}
		else if (ItemData == 3)
		{
			if (true == bitValue)
				strValue = "����";
			else
				strValue = "����";
		}
		else if (ItemData == 4)
		{
			if (true == bitValue)
				strValue = "����";
			else
				strValue = "����";
		}
		else if (ItemData == 5)
		{
			if (true == bitValue)
				strValue = "����";
			else
				strValue = "����";
		}
		else if (ItemData == 6)
		{
			if (true == bitValue)
				strValue = "����";
			else
				strValue = "����";
		}
		else if (ItemData == 7)
		{
			if (true == bitValue)
				strValue = "����";
			else
				strValue = "����";
		}

		m_Edit_AlarmValue.SetWindowText(strValue);
	}
}

void CRtuAttrAlarmState::OnCbnSelchangeCmbExalarm()
{
	m_Edit_ExAlarmValue.SetWindowText("");
	if (!m_pSubStation) return;

	int nSel = m_wndExAlarmCmb.GetCurSel();
	if (nSel != CB_ERR)
	{
		BYTE ItemData = (BYTE)m_wndExAlarmCmb.GetItemData(nSel);
		CRTU_EXAlarm_State*  pAlarmOBJ = m_pSubStation->GetRtuExAlarm_State();

		bool bitValue = false;
		CString strValue = "";
		bitValue = pAlarmOBJ->GetBitValue(ItemData);
		if (ItemData == 0)
		{
			if (bitValue == false)
				strValue = "����";
			else
				strValue = "����";

		}
		else if (ItemData == 1)
		{
			if (false == bitValue)
				strValue = "����";
			else
				strValue = "����";
		}
		m_Edit_ExAlarmValue.SetWindowText(strValue);
	}
}

void CRtuAttrAlarmState::OnCbnSelchangeCmbRtustate()
{
	m_Edit_StateValue.SetWindowText("");
	if (!m_pSubStation) return;

	int nSel = m_wndRtuStateCmb.GetCurSel();
	if (nSel != CB_ERR)
	{
		BYTE ItemData = (BYTE)m_wndRtuStateCmb.GetItemData(nSel);
		CRTUTerminal_State* pTStateOBJ = m_pSubStation->GetRtuTerminal_State();

		bool bitValue = false;
		CString strValue = "";
		bitValue = pTStateOBJ->GetBitValue(ItemData);

		if (ItemData == 1)
		{
			BYTE workMode = (pTStateOBJ->GetBitValue(0) == true?1:0)  +
				(pTStateOBJ->GetBitValue(1) == true ? 2:0) +
				(pTStateOBJ->GetBitValue(2)== true ? 4:0);

			if (workMode == 0)
				strValue = "���ݹ���ģʽ";
			else if (workMode == 1)
				strValue = "�Ա�����ģʽ";
			else if (workMode== 2)
				strValue = "��ѯ/Ӧ����ģʽ";
			else if (workMode == 3)
				strValue = "����/ά��ģʽ";
			else if (workMode == 4)
				strValue = "��Ъ����ģʽ";
		}
		//else if (ItemData == 2)
		//{
		//	if (true == bitValue)
		//		strValue = "��Ч";
		//	else
		//		strValue = "��Ч";
		//}
		else if (ItemData == 3)
		{
			if (true == bitValue)
				strValue = "Ͷ��";
			else
				strValue = "�˳�";
		}
		else if (ItemData == 4)
		{
			if (true == bitValue)
				strValue = "ֹͣ";
			else
				strValue = "����";
		}
		else if (ItemData == 5)
		{
			if (true == bitValue)
				strValue = "�ر�";
			else
				strValue = "����";
		}
		else if (ItemData == 6)
		{
			if (true == bitValue)
				strValue = "���ع���";
			else
				strValue = "�ⲿ����";
		}
		m_Edit_StateValue.SetWindowText(strValue);
	}
}


void CRtuAttrAlarmState::OnBnClickAlarmCall()
{
	Update();
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CPacketFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //���ɳ�ʼ���·�֡����֡������Ϣ

			if (m_pSubStation->GetAppProVer() == APP_PROTOCOL_V1_0)
				m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_5EH(&appFrame);
			else if (m_pSubStation->GetAppProVer() == APP_PROTOCOL_V2_0)
				m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_69H(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			if (m_pSubStation->GetAppProVer() == APP_PROTOCOL_V1_0)
				SetTimer(W_TIMER_ALARM_CALL,nElapse,NULL);
			else if (m_pSubStation->GetAppProVer() == APP_PROTOCOL_V2_0)
				SetTimer(W_TIMER_EXALARM_CALL,nElapse,NULL);
		}
	}
}

void CRtuAttrAlarmState::OnBnClickExAlarmCall()
{
	Update();
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CPacketFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //���ɳ�ʼ���·�֡����֡������Ϣ

			m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_66H(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_EXALARM_CALL,nElapse,NULL);
		}
	}
}

void CRtuAttrAlarmState::SetWndsEnable(BOOL blEnable /* = TRUE */)
{
	if (m_btnAlarmCall.GetSafeHwnd())
		m_btnAlarmCall.EnableWindow(blEnable);
	if (m_btnExAlarmCall.GetSafeHwnd())
		m_btnExAlarmCall.EnableWindow(blEnable);

}

void CRtuAttrAlarmState::OnTimer(UINT_PTR nIDEvent)
{
	CString strLogMessge = "";
	_Log_level_type _logLevle = Normal_Log_Message;

	switch (nIDEvent)
	{
	case W_TIMER_ALARM_CALL:
		{
			if (m_blResult == TRUE)
				strLogMessge = "��ѯң���ն˱���״̬���ն�״̬ �ɹ�!";
			else if( m_blResult == FALSE)
			{
				strLogMessge = "��ѯң���ն˱���״̬���ն�״̬ ��ʱ!";
				_logLevle = Warning_log_Message;
			}
		}
		break;
	case W_TIMER_EXALARM_CALL:
		{
			if (m_blResult == TRUE)
				strLogMessge = "��ѯң���ն���չ����״̬���ն�״̬ �ɹ�!";
			else if( m_blResult == FALSE)
			{
				strLogMessge = "��ѯң���ն���չ����״̬���ն�״̬ ��ʱ!";
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


void CRtuAttrAlarmState::UpdateDlg(int attrType,BOOL blResult)
{
	m_blResult = blResult;
	if (attrType == rtu_attr_alarmstate_call)
		SendMessage(WM_TIMER,W_TIMER_ALARM_CALL);
	else if (attrType == rtu_attr_exalarmstate_call)
		SendMessage(WM_TIMER,W_TIMER_EXALARM_CALL);
	Update();

}

void CRtuAttrAlarmState::KillAllTimers()
{
	KillTimer(W_TIMER_EXALARM_CALL);
	KillTimer(W_TIMER_ALARM_CALL);
	SetWndsEnable(TRUE);
}