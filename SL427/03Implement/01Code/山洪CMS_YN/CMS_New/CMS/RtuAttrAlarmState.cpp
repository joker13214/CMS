// RtuAttrAlarmState.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuAttrAlarmState.h"


#define  W_TIMER_EXALARM_CALL 0x01             //查询遥测终端扩展报警状态
#define  W_TIMER_ALARM_CALL   0x02             //查询遥测终端的报警状态

// CRtuAttrAlarmState 对话框
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


// CRtuAttrAlarmState 消息处理程序


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
			
			int Index = m_wndExAlarmCmb.AddString("雨量报警状态");
			m_wndExAlarmCmb.SetItemData(Index,0);
	
			Index = m_wndExAlarmCmb.AddString("模拟量报警状态");
			m_wndExAlarmCmb.SetItemData(Index,1);
		}
	
		if (m_wndRtuStateCmb.GetSafeHwnd())
		{
			m_wndRtuStateCmb.ResetContent();
	
			int Index = 0 ;
			Index = m_wndRtuStateCmb.AddString("终端工作模式");
			m_wndRtuStateCmb.SetItemData(Index,1);
	
			//Index = m_wndRtuStateCmb.AddString("IC卡功能是否有效");
			//m_wndRtuStateCmb.SetItemData(Index,2);
			
			//Index = m_wndRtuStateCmb.AddString("定值控制是否投入");
			//m_wndRtuStateCmb.SetItemData(Index,3);
	
			//Index = m_wndRtuStateCmb.AddString("水泵工作状态");
			//m_wndRtuStateCmb.SetItemData(Index,4);
	
			//Index = m_wndRtuStateCmb.AddString("终端箱门状态");
			//m_wndRtuStateCmb.SetItemData(Index,5);
	
			Index = m_wndRtuStateCmb.AddString("电源工作状态");
			m_wndRtuStateCmb.SetItemData(Index,6);
		}

		if (m_wndAlarmCmb.GetSafeHwnd())
		{
			m_wndAlarmCmb.ResetContent();

			int Index = 0 ;
			//Index = m_wndAlarmCmb.AddString("交流电状态");
			//m_wndAlarmCmb.SetItemData(Index,0);

			Index = m_wndAlarmCmb.AddString("蓄电池电压状态");
			m_wndAlarmCmb.SetItemData(Index,1);

			Index = m_wndAlarmCmb.AddString("水位超限报警状态");
			m_wndAlarmCmb.SetItemData(Index,2);

			Index = m_wndAlarmCmb.AddString("水压超限报警状态");
			m_wndAlarmCmb.SetItemData(Index,3);

			Index = m_wndAlarmCmb.AddString("水质超限报警状态");
			m_wndAlarmCmb.SetItemData(Index,4);

			//Index = m_wndAlarmCmb.AddString("水表传感器状态");
			//m_wndAlarmCmb.SetItemData(Index,5);

			//Index = m_wndAlarmCmb.AddString("水泵状态");
			//m_wndAlarmCmb.SetItemData(Index,6);

			//Index = m_wndAlarmCmb.AddString("剩余水量的下限报警状态");
			//m_wndAlarmCmb.SetItemData(Index,7);
		}

	return TRUE;  
	// return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
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
				strValue = "正常";
			else
				strValue = "停电";

		}
		else if (ItemData == 1)
		{
			if (false == bitValue)
				strValue = "正常";
			else
				strValue = "电压低";
		}
		else if (ItemData == 2)
		{
			if (true == bitValue)
				strValue = "报警";
			else
				strValue = "正常";
		}
		else if (ItemData == 3)
		{
			if (true == bitValue)
				strValue = "报警";
			else
				strValue = "正常";
		}
		else if (ItemData == 4)
		{
			if (true == bitValue)
				strValue = "报警";
			else
				strValue = "正常";
		}
		else if (ItemData == 5)
		{
			if (true == bitValue)
				strValue = "故障";
			else
				strValue = "正常";
		}
		else if (ItemData == 6)
		{
			if (true == bitValue)
				strValue = "故障";
			else
				strValue = "正常";
		}
		else if (ItemData == 7)
		{
			if (true == bitValue)
				strValue = "报警";
			else
				strValue = "正常";
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
				strValue = "正常";
			else
				strValue = "报警";

		}
		else if (ItemData == 1)
		{
			if (false == bitValue)
				strValue = "正常";
			else
				strValue = "报警";
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
				strValue = "兼容工作模式";
			else if (workMode == 1)
				strValue = "自报工作模式";
			else if (workMode== 2)
				strValue = "查询/应答工作模式";
			else if (workMode == 3)
				strValue = "调试/维修模式";
			else if (workMode == 4)
				strValue = "间歇工作模式";
		}
		//else if (ItemData == 2)
		//{
		//	if (true == bitValue)
		//		strValue = "有效";
		//	else
		//		strValue = "无效";
		//}
		else if (ItemData == 3)
		{
			if (true == bitValue)
				strValue = "投入";
			else
				strValue = "退出";
		}
		else if (ItemData == 4)
		{
			if (true == bitValue)
				strValue = "停止";
			else
				strValue = "启动";
		}
		else if (ItemData == 5)
		{
			if (true == bitValue)
				strValue = "关闭";
			else
				strValue = "开启";
		}
		else if (ItemData == 6)
		{
			if (true == bitValue)
				strValue = "蓄电池供电";
			else
				strValue = "外部供电";
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
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息

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
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息

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
				strLogMessge = "查询遥测终端报警状态和终端状态 成功!";
			else if( m_blResult == FALSE)
			{
				strLogMessge = "查询遥测终端报警状态和终端状态 超时!";
				_logLevle = Warning_log_Message;
			}
		}
		break;
	case W_TIMER_EXALARM_CALL:
		{
			if (m_blResult == TRUE)
				strLogMessge = "查询遥测终端扩展报警状态和终端状态 成功!";
			else if( m_blResult == FALSE)
			{
				strLogMessge = "查询遥测终端扩展报警状态和终端状态 超时!";
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