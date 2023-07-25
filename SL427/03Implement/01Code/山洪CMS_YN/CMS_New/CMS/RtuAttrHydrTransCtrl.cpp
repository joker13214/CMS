// RtuAttrHydrTransCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuAttrHydrTransCtrl.h"


// RtuAttrHydrTransCtrl 对话框

#define W_TIMER_Q_RAINSTARDARD    0x01
#define W_TIMER_S_RAINSTARDARD    0x02
#define W_TIMER_Q_RAINSENSORDPI   0x03
#define W_TIMER_S_RAINSENSORDPI   0x04
#define W_TIMER_Q_RAINSENSORBASE  0x05
#define W_TIMER_S_RAINSENSORBASE  0x06
#define W_TIMER_Q_RAINSENSORPARA  0x07
#define W_TIMER_S_RAINSENSORPARA  0x08
#define W_TIMER_Q_RAINDEFINE      0x09
#define W_TIMER_S_RAINDEFINE      0x10
#define W_TIMER_Q_RAINTRALSTARDARD  0x11
#define W_TIMER_S_RAINTRALSTARDARD  0x12
#define W_TIMER_Q_RAINTIMESPAN      0x13
#define W_TIMER_S_RAINTIMESPAN      0x14
#define W_TIMER_S_TIME              0x15

#define W_TIMER_ER_RAINSTARDARD    0x101
#define W_TIMER_ER_RAINSENSORDPI   0x103
#define W_TIMER_ER_RAINSENSORBASE  0x105
#define W_TIMER_ER_RAINSENSORPARA  0x107
#define W_TIMER_ER_RAINDEFINE      0x109
#define W_TIMER_ER_RAINTRALSTARDARD  0x111
#define W_TIMER_ER_RAINTIMESPAN      0x113




IMPLEMENT_DYNAMIC(RtuAttrHydrTransCtrl, CDialog)

RtuAttrHydrTransCtrl::RtuAttrHydrTransCtrl(CWnd* pParent /*=NULL*/)
	: CDialog(RtuAttrHydrTransCtrl::IDD, pParent)
{
	m_pParentDlg = NULL;
	m_pSubStation = NULL;
	m_strPsw = "";
}

RtuAttrHydrTransCtrl::~RtuAttrHydrTransCtrl()
{
}

void RtuAttrHydrTransCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_QUERY_RAINSTARDARD, m_btnQuery_rainStardard);
	DDX_Control(pDX, IDC_BTN_SET_RAINSTARDARD, m_btnSet_rainStardard);
	DDX_Control(pDX, IDC_BTN_QUERY_RAINSENSORDPI, m_btnQuery_rainSensorDpi);
	DDX_Control(pDX, IDC_BTN_SET_RAINSENSORDPI, m_btnSet_rainSensorDpi);
	DDX_Control(pDX, IDC_BTN_QUERY_SENSORBASE, m_btnQuery_sensorBase);
	DDX_Control(pDX, IDC_BTN_SET_SENSORBASE, m_btnSet_sensorBase);
	DDX_Control(pDX, IDC_BTN_QUERY_SENSORPARA, m_btnQuery_sensorPara);
	DDX_Control(pDX, IDC_BTN_SET_SENSORPARA, m_btnSet_sensorPara);
	DDX_Control(pDX, IDC_BTN_QUERY_SENSORDEF, m_btnQuery_sensorDef);
	DDX_Control(pDX, IDC_BTN_SET_SENSORDEF, m_btnSet_SensorDef);
	DDX_Control(pDX, IDC_BTN_QUERY_HYDRSENDCTRL, m_btnQuery_hydrSendCtrl);
	DDX_Control(pDX, IDC_BTN_SET_HYDRSENDCTRL, m_btnSet_hydrSendCtrl);
	DDX_Control(pDX, IDC_BTN_QUERY_HYDRTIMESPAN, m_btnQuery_hydrTimeSpan);
	DDX_Control(pDX, IDC_BTN_SET_HYDRTIMESPAN, m_btnSet_hydrTimeSpan);

	DDX_Control(pDX, IDC_LET_RAINSTARDARD, m_let_rainStardard);
	DDX_Control(pDX, IDC_LET_RAINSENSORDPI, m_let_rainSensorDpi);
	DDX_Control(pDX, IDC_CBX_SENSORBASE, m_Cbx_SensorNo_rainbase);
	DDX_Control(pDX, IDC_LET_SENSORBASE, m_let_rainBaseVar);
	DDX_Control(pDX, IDC_CBX_SENSORPARA, m_Cbx_SensorNo_rainPara);
	DDX_Control(pDX, IDC_LET_SENSORPARA, m_let_rainParaVar);
	DDX_Control(pDX, IDC_CBX_SENSORDEF, m_Cbx_SensorNo_rainDef);
	DDX_Control(pDX, IDC_CBX_DEF, m_cbx_rainDefVar);
	DDX_Control(pDX, IDC_CBX_HYDRSENDCTRL, m_Cbx_SensorNo_sendStardard);
	DDX_Control(pDX, IDC_CBX_X_HYDRSENDCTRL, m_Cbx_Level_sendStardard);
	DDX_Control(pDX, IDC_LET_HYDRSENDCTRL, m_let_SendStardardVar);
	DDX_Control(pDX, IDC_CBX_HYDRTIMESPAN, m_Cbx_SensorNo_sendTimeSpan);
	DDX_Control(pDX, IDC_CBX_X_HYDRTIMESPAN, m_Cbx_Level_sendTimeSpan);
	DDX_Control(pDX, IDC_LET_HYDRTIMESPAN, m_let_SendTimeSpanVar);

	DDX_Control(pDX, IDC_BTN_CHECKTIME, m_btn_CheckTime);

	DDX_Control(pDX, IDC_LET_PSW, m_PswEdit);
	DDX_Text(pDX,IDC_LET_PSW, m_strPsw);
	DDV_MaxChars(pDX, m_strPsw, 6);

	DDX_Text(pDX,IDC_LET_RAINSTARDARD, m_str_rainStardard);
	DDV_MaxChars(pDX, m_str_rainStardard, 7);

	DDX_Text(pDX,IDC_LET_RAINSENSORDPI, m_str_rainSensorDpi);
	DDV_MaxChars(pDX, m_str_rainSensorDpi, 7);

	DDX_Text(pDX,IDC_LET_SENSORBASE, m_str_rainBaseVar);
	DDV_MaxChars(pDX, m_str_rainBaseVar, 7);

	DDX_Text(pDX,IDC_LET_SENSORPARA, m_str_rainParaVar);
	DDV_MaxChars(pDX, m_str_rainParaVar, 7);

	DDX_Text(pDX,IDC_LET_HYDRSENDCTRL, m_str_SendStardardVar);
	DDV_MaxChars(pDX, m_str_SendStardardVar, 7);

	DDX_Text(pDX,IDC_LET_HYDRTIMESPAN, m_str_SendTimeSpanVar);
	DDV_MaxChars(pDX, m_str_SendTimeSpanVar, 7);
}


BEGIN_MESSAGE_MAP(RtuAttrHydrTransCtrl, CDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_QUERY_RAINSTARDARD, &RtuAttrHydrTransCtrl::OnBnClickedBtnQueryRainstardard)
	ON_BN_CLICKED(IDC_BTN_SET_RAINSTARDARD, &RtuAttrHydrTransCtrl::OnBnClickedBtnSetRainstardard)
	ON_BN_CLICKED(IDC_BTN_QUERY_RAINSENSORDPI, &RtuAttrHydrTransCtrl::OnBnClickedBtnQueryRainsensordpi)
	ON_BN_CLICKED(IDC_BTN_SET_RAINSENSORDPI, &RtuAttrHydrTransCtrl::OnBnClickedBtnSetRainsensordpi)
	ON_BN_CLICKED(IDC_BTN_QUERY_SENSORBASE, &RtuAttrHydrTransCtrl::OnBnClickedBtnQuerySensorbase)
	ON_BN_CLICKED(IDC_BTN_SET_SENSORBASE, &RtuAttrHydrTransCtrl::OnBnClickedBtnSetSensorbase)
	ON_BN_CLICKED(IDC_BTN_QUERY_SENSORPARA, &RtuAttrHydrTransCtrl::OnBnClickedBtnQuerySensorpara)
	ON_BN_CLICKED(IDC_BTN_SET_SENSORPARA, &RtuAttrHydrTransCtrl::OnBnClickedBtnSetSensorpara)
	ON_BN_CLICKED(IDC_BTN_QUERY_SENSORDEF, &RtuAttrHydrTransCtrl::OnBnClickedBtnQuerySensordef)
	ON_BN_CLICKED(IDC_BTN_SET_SENSORDEF, &RtuAttrHydrTransCtrl::OnBnClickedBtnSetSensordef)
	ON_BN_CLICKED(IDC_BTN_QUERY_HYDRSENDCTRL, &RtuAttrHydrTransCtrl::OnBnClickedBtnQueryHydrsendctrl)
	ON_BN_CLICKED(IDC_BTN_SET_HYDRSENDCTRL, &RtuAttrHydrTransCtrl::OnBnClickedBtnSetHydrsendctrl)
	ON_BN_CLICKED(IDC_BTN_QUERY_HYDRTIMESPAN, &RtuAttrHydrTransCtrl::OnBnClickedBtnQueryHydrtimespan)
	ON_BN_CLICKED(IDC_BTN_SET_HYDRTIMESPAN, &RtuAttrHydrTransCtrl::OnBnClickedBtnSetHydrtimespan)
	ON_EN_CHANGE(IDC_LET_PSW, &RtuAttrHydrTransCtrl::OnEnChangeLetPsw)
	ON_BN_CLICKED(IDC_BTN_CHECKTIME, &RtuAttrHydrTransCtrl::OnBnClickedBtnChecktime)
END_MESSAGE_MAP()


// RtuAttrHydrTransCtrl 消息处理程序

BOOL RtuAttrHydrTransCtrl::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Cbx_SensorNo_rainbase.SetCurSel(0);
	m_Cbx_SensorNo_rainPara.SetCurSel(0);
	m_Cbx_SensorNo_rainDef.SetCurSel(0);
	m_Cbx_SensorNo_sendStardard.SetCurSel(0);
	m_Cbx_Level_sendStardard.SetCurSel(0);
	m_Cbx_SensorNo_sendTimeSpan.SetCurSel(0);
	m_Cbx_Level_sendTimeSpan.SetCurSel(0);

	return TRUE;  
}

void RtuAttrHydrTransCtrl::SetParentDlg(CRtuPropertyDlg* pDlg)
{
	m_pParentDlg = pDlg;	
}


void RtuAttrHydrTransCtrl::Update()
{
	CHydrTransCtrlData *pData = NULL;
	if (m_pParentDlg)
		m_pSubStation = m_pParentDlg->GetRtuStation();	

	if (m_pSubStation != NULL)
	{
		pData = m_pSubStation->GetRainStardard();
		if (pData != NULL)
		{
			CString str = "";
			str.Format("%d", pData->GetCtrlData());
			m_let_rainStardard.SetWindowText(str);

		}

		pData = NULL;
		pData = m_pSubStation->GetRainSensorDpi();
		if (pData != NULL)
		{
			CString str = "";
			str.Format("%d", pData->GetCtrlData());
			m_let_rainSensorDpi.SetWindowText(str);

		}

		pData = NULL;
		pData = m_pSubStation->GetRainSensorBase();
		if (pData != NULL)
		{
			CString str = "";
			str.Format("%d", pData->GetCtrlData());
			m_let_rainBaseVar.SetWindowText(str);

		}

		pData = NULL;
		pData = m_pSubStation->GetRainSensorPara();
		if (pData != NULL)
		{
			CString str = "";
			str.Format("%d", pData->GetCtrlData());
			m_let_rainParaVar.SetWindowText(str);

		}

		pData = NULL;
		pData = m_pSubStation->GetRainDefine();
		if (pData != NULL)
		{
			int data = (int)pData->GetCtrlData();
			switch (data)
			{
			case 1:
				m_cbx_rainDefVar.SetCurSel(0);
				break;
			case 2:
				m_cbx_rainDefVar.SetCurSel(1);
				break;
			}
		}

		pData = NULL;
		pData = m_pSubStation->GetRainSendStardard();
		if (pData != NULL)
		{
			CString str = "";
			str.Format("%d", pData->GetCtrlData());
			m_let_SendStardardVar.SetWindowText(str);

		}

		pData = NULL;
		pData = m_pSubStation->GetRainSendTimeSpan();
		if (pData != NULL)
		{
			CString str = "";
			str.Format("%d", pData->GetCtrlData());
			m_let_SendTimeSpanVar.SetWindowText(str);

		}


	}
}

void RtuAttrHydrTransCtrl::Update(int ret)
{
	CHydrTransCtrlData *pData = NULL;
	if (m_pParentDlg)
		m_pSubStation = m_pParentDlg->GetRtuStation();	

	if (m_pSubStation != NULL)
	{
		switch(ret)
		{
		case scsw_waterRainStardard:
			{
				pData = m_pSubStation->GetRainStardard();
				if (pData != NULL)
				{
					CString str = "";
					str.Format("%.2f", pData->GetCtrlData());
					m_let_rainStardard.SetWindowText(str);

				}
			}
			break;
		case scsw_waterRainSensorDpi:
			{
				pData = m_pSubStation->GetRainSensorDpi();
				if (pData != NULL)
				{
					CString str = "";
					str.Format("%.2f", pData->GetCtrlData());
					m_let_rainSensorDpi.SetWindowText(str);

				}
			}
			break;
		case scsw_waterRainSensorBase:
			{
				pData = m_pSubStation->GetRainSensorBase();
				if (pData != NULL)
				{
					CString str = "";
					str.Format("%.2f", pData->GetCtrlData());
					m_let_rainBaseVar.SetWindowText(str);

				}
			}
			break;
		case scsw_waterRainSensorPara:
			{
				pData = m_pSubStation->GetRainSensorPara();
				if (pData != NULL)
				{
					CString str = "";
					str.Format("%.2f", pData->GetCtrlData());
					m_let_rainParaVar.SetWindowText(str);

				}
			}
			break;
		case scsw_waterDefine:
			{
				pData = m_pSubStation->GetRainDefine();
				if (pData != NULL)
				{
					int temp = (int)pData->GetCtrlData();
					if (1 == temp)
						m_cbx_rainDefVar.SetCurSel(0);
					else if (2 == temp)
						m_cbx_rainDefVar.SetCurSel(1);
				}
			}
			break;
		case scsw_waterrainTransStardard:
			{
				pData = m_pSubStation->GetRainSendStardard();
				if (pData != NULL)
				{
					CString str = "";
					str.Format("%.2f", pData->GetCtrlData());
					m_let_SendStardardVar.SetWindowText(str);

				}
			}
			break;
		case scsw_waterrainTimespan:
			{
				pData = m_pSubStation->GetRainSendTimeSpan();
				if (pData != NULL)
				{
					CString str = "";
					str.Format("%.2f", pData->GetCtrlData());
					m_let_SendTimeSpanVar.SetWindowText(str);

				}
			}
			break;
		}
	}


}



void RtuAttrHydrTransCtrl::OnTimer(UINT_PTR nIDEvent)
{
	CString strLog= "";
	_Log_level_type LogLevel = Normal_Log_Message;


	switch (nIDEvent)
	{
	case W_TIMER_Q_RAINSTARDARD:
		{
			if (m_blResult == TRUE)
				strLog = "查询雨量加报控制标准 成功!";
			else if ( m_blResult == FALSE)
			{
				strLog = "查询雨量加报控制标准 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_blResult == 0xff)
			{
				strLog = "查询雨量加报控制标准 错误!";
				LogLevel = Warning_log_Message;
			}
		}
		break;
	case W_TIMER_S_RAINSTARDARD:
		{
			if (m_blResult == TRUE)
				strLog = "设置雨量加报控制标准 成功!";
			else if ( m_blResult == FALSE)
			{
				strLog = "设置雨量加报控制标准 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_blResult == 0xff)
			{
				strLog = "设置雨量加报控制标准 错误!";
				LogLevel = Warning_log_Message;
			}

		}
		break;
	case W_TIMER_Q_RAINSENSORDPI:
		{
			if (m_blResult == TRUE)
				strLog = "查询雨量传感器分辨力 成功!";
			else if ( m_blResult == FALSE)
			{
				strLog = "查询雨量传感器分辨力 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_blResult == 0xff)
			{
				strLog = "查询雨量传感器分辨力 错误!";
				LogLevel = Warning_log_Message;
			}

		}
		break;
	case W_TIMER_S_RAINSENSORDPI:
		{
			if (m_blResult == TRUE)
				strLog = "设置雨量传感器分辨力 成功!";
			else if ( m_blResult == FALSE)
			{
				strLog = "设置雨量传感器分辨力 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_blResult == 0xff)
			{
				strLog = "设置雨量传感器分辨力 错误!";
				LogLevel = Warning_log_Message;
			}

		}
		break;
	case W_TIMER_Q_RAINSENSORBASE:
		{
			if (m_blResult == TRUE)
				strLog = "查询水位传感器基数 成功!";
			else if ( m_blResult == FALSE)
			{
				strLog = "查询水位传感器基数 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_blResult == 0xff)
			{
				strLog = "查询水位传感器基数 错误!";
				LogLevel = Warning_log_Message;
			}

		}
		break;
	case W_TIMER_S_RAINSENSORBASE:
		{
			if (m_blResult == TRUE)
				strLog = "设置水位传感器基数 成功!";
			else if ( m_blResult == FALSE)
			{
				strLog = "设置水位传感器基数 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_blResult == 0xff)
			{
				strLog = "设置水位传感器基数 错误!";
				LogLevel = Warning_log_Message;
			}

		}
		break;
	case W_TIMER_Q_RAINSENSORPARA:
		{
			if (m_blResult == TRUE)
				strLog = "查询水位传感器读数转换系数 成功!";
			else if ( m_blResult == FALSE)
			{
				strLog = "查询水位传感器读数转换系数 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_blResult == 0xff)
			{
				strLog = "查询水位传感器读数转换系数 错误!";
				LogLevel = Warning_log_Message;
			}


		}
		break;
	case W_TIMER_S_RAINSENSORPARA:
		{
			if (m_blResult == TRUE)
				strLog = "设置水位传感器读数转换系数 成功!";
			else if ( m_blResult == FALSE)
			{
				strLog = "设置水位传感器读数转换系数 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_blResult == 0xff)
			{
				strLog = "设置水位传感器读数转换系数 错误!";
				LogLevel = Warning_log_Message;
			}

		}
		break; 
	case W_TIMER_Q_RAINDEFINE:
		{
			if (m_blResult == TRUE)
				strLog = "查询水位计算公式定义 成功!";
			else if ( m_blResult == FALSE)
			{
				strLog = "查询水位计算公式定义 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_blResult == 0xff)
			{
				strLog = "查询水位计算公式定义 错误!";
				LogLevel = Warning_log_Message;
			}

		}
		break;
	case W_TIMER_S_RAINDEFINE:
		{
			if (m_blResult == TRUE)
				strLog = "设置水位计算公式定义 成功!";
			else if ( m_blResult == FALSE)
			{
				strLog = "设置水位计算公式定义 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_blResult == 0xff)
			{
				strLog = "设置水位计算公式定义 错误!";
				LogLevel = Warning_log_Message;
			}

		}
		break;
	case W_TIMER_Q_RAINTRALSTARDARD:
		{
			if (m_blResult == TRUE)
				strLog = "查询水位发送控制标准 成功!";
			else if ( m_blResult == FALSE)
			{
				strLog = "查询水位发送控制标准 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_blResult == 0xff)
			{
				strLog = "查询水位发送控制标准 错误!";
				LogLevel = Warning_log_Message;
			}


		}
		break;
	case W_TIMER_S_RAINTRALSTARDARD:
		{
			if (m_blResult == TRUE)
				strLog = "设置水位发送控制标准 成功!";
			else if ( m_blResult == FALSE)
			{
				strLog = "设置水位发送控制标准 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_blResult == 0xff)
			{
				strLog = "设置水位发送控制标准 错误!";
				LogLevel = Warning_log_Message;
			}

		}
		break;
	case W_TIMER_Q_RAINTIMESPAN:
		{
			if (m_blResult == TRUE)
				strLog = "查询水位发送时间间隔控制 成功!";
			else if ( m_blResult == FALSE)
			{
				strLog = "查询水位发送时间间隔控制 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_blResult == 0xff)
			{
				strLog = "查询水位发送时间间隔控制 错误!";
				LogLevel = Warning_log_Message;
			}


		}
		break;
	case W_TIMER_S_RAINTIMESPAN:
		{
			if (m_blResult == TRUE)
				strLog = "设置水位发送时间间隔控制 成功!";
			else if ( m_blResult == FALSE)
			{
				strLog = "设置水位发送时间间隔控制 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_blResult == 0xff)
			{
				strLog = "设置水位发送时间间隔控制 错误!";
				LogLevel = Warning_log_Message;
			}

		}
		break;
	case W_TIMER_S_TIME:
		{
			if (m_blResult == TRUE)
				strLog = "校时 成功!";
			else if ( m_blResult == FALSE)
			{
				strLog = "校时 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_blResult == 0xff)
			{
				strLog = "校时 错误!";
				LogLevel = Warning_log_Message;
			}

		}
	}

	KillTimer(nIDEvent);
	SetWndsEnable(TRUE);
	if (m_pParentDlg && m_pParentDlg->GetSafeHwnd())
		m_pParentDlg->AddLog(strLog,(PSYSTEMTIME)NULL,LogLevel);

	if (m_pSubStation)
		m_pSubStation->InsertLogMessage(strLog,LogLevel,NULL);

	CDialog::OnTimer(nIDEvent);
}

void RtuAttrHydrTransCtrl::SetWndsEnable(BOOL blEnable /* = TRUE */)
{
	m_btnQuery_rainStardard.EnableWindow(blEnable);
	m_btnSet_rainStardard.EnableWindow(blEnable);
	m_btnQuery_rainSensorDpi.EnableWindow(blEnable);
	m_btnSet_rainSensorDpi.EnableWindow(blEnable);
	m_btnQuery_sensorBase.EnableWindow(blEnable);
	m_btnSet_sensorBase.EnableWindow(blEnable);
	m_btnQuery_sensorPara.EnableWindow(blEnable);
	m_btnSet_sensorPara.EnableWindow(blEnable);
	m_btnQuery_sensorDef.EnableWindow(blEnable);
	m_btnSet_SensorDef.EnableWindow(blEnable);
	m_btnQuery_hydrSendCtrl.EnableWindow(blEnable);
	m_btnSet_hydrSendCtrl.EnableWindow(blEnable);
	m_btnQuery_hydrTimeSpan.EnableWindow(blEnable);
	m_btnSet_hydrTimeSpan.EnableWindow(blEnable);
	m_let_rainStardard.EnableWindow(blEnable);
	m_let_rainSensorDpi.EnableWindow(blEnable);
	m_Cbx_SensorNo_rainbase.EnableWindow(blEnable);
	m_let_rainBaseVar.EnableWindow(blEnable);
	m_Cbx_SensorNo_rainPara.EnableWindow(blEnable);
	m_let_rainParaVar.EnableWindow(blEnable);
	m_Cbx_SensorNo_rainDef.EnableWindow(blEnable);
	m_cbx_rainDefVar.EnableWindow(blEnable);
	m_Cbx_SensorNo_sendStardard.EnableWindow(blEnable);
	m_Cbx_Level_sendStardard.EnableWindow(blEnable);
	m_let_SendStardardVar.EnableWindow(blEnable);
	m_Cbx_SensorNo_sendTimeSpan.EnableWindow(blEnable);
	m_Cbx_Level_sendTimeSpan.EnableWindow(blEnable);
	m_let_SendTimeSpanVar.EnableWindow(blEnable);
	m_btn_CheckTime.EnableWindow(blEnable);
}


void RtuAttrHydrTransCtrl::UpdateDlg(int attrType,BOOL blResult)
{
	m_blResult = blResult;
	if (attrType == scsw_waterRainStardard )
	{
		SendMessage(WM_TIMER,W_TIMER_Q_RAINSTARDARD, NULL);
	}
	else if (attrType == scsw_waterRainSensorDpi )
	{
		SendMessage(WM_TIMER,W_TIMER_Q_RAINSENSORDPI,NULL);
	}
	else if (attrType == scsw_waterRainSensorBase )
	{
		SendMessage(WM_TIMER,W_TIMER_Q_RAINSENSORBASE,NULL);
	}
	else if (attrType == scsw_waterRainSensorPara )
	{
		SendMessage(WM_TIMER,W_TIMER_Q_RAINSENSORPARA,NULL);
	}
	else if (attrType == scsw_waterDefine )
	{
		SendMessage(WM_TIMER,W_TIMER_Q_RAINDEFINE,NULL);
	}
	else if (attrType == scsw_waterrainTransStardard  )
	{
		SendMessage(WM_TIMER,W_TIMER_Q_RAINTRALSTARDARD,NULL);
	}
	else if (attrType == scsw_waterrainTimespan  )
	{
		SendMessage(WM_TIMER,W_TIMER_Q_RAINTIMESPAN,NULL);
	}
	else if (attrType == scsw_waterRainStardard_set)
	{
		SendMessage(WM_TIMER,W_TIMER_Q_RAINTIMESPAN,NULL);
	}
	else if (attrType == scsw_waterRainSensorDpi_set )
	{
		SendMessage(WM_TIMER,W_TIMER_S_RAINSENSORDPI   ,NULL);
	}
	else if (attrType == scsw_waterRainSensorBase_set )
	{
		SendMessage(WM_TIMER,W_TIMER_S_RAINSENSORBASE,NULL);
	}
	else if (attrType == scsw_waterRainSensorPara_set )
	{
		SendMessage(WM_TIMER,W_TIMER_S_RAINSENSORPARA,NULL);
	}
	else if (attrType == scsw_waterDefine_set )
	{
		SendMessage(WM_TIMER,W_TIMER_S_RAINDEFINE,NULL);
	}
	else if (attrType == scsw_waterrainTransStardard_set  )
	{
		SendMessage(WM_TIMER,W_TIMER_S_RAINTRALSTARDARD,NULL);
	}
	else if (attrType == scsw_waterrainTimespan_set  )
	{
		SendMessage(WM_TIMER,W_TIMER_S_RAINTIMESPAN,NULL);
	}
	else if (attrType == scsw_waterCheckTime)
	{
		SendMessage(WM_TIMER, W_TIMER_S_TIME, NULL);
	}
}

void RtuAttrHydrTransCtrl::KillAllTimers()
{
	KillTimer(W_TIMER_Q_RAINSTARDARD);
	KillTimer(W_TIMER_S_RAINSTARDARD);
	KillTimer(W_TIMER_Q_RAINSENSORDPI);
	KillTimer(W_TIMER_S_RAINSENSORDPI);
	KillTimer(W_TIMER_Q_RAINSENSORBASE);
	KillTimer(W_TIMER_S_RAINSENSORBASE); 
	KillTimer(W_TIMER_Q_RAINSENSORPARA);
	KillTimer(W_TIMER_S_RAINSENSORPARA);
	KillTimer(W_TIMER_Q_RAINDEFINE);
	KillTimer(W_TIMER_S_RAINDEFINE);
	KillTimer(W_TIMER_Q_RAINTRALSTARDARD); 
	KillTimer(W_TIMER_S_RAINTRALSTARDARD);
	KillTimer(W_TIMER_Q_RAINTIMESPAN);
	KillTimer(W_TIMER_S_RAINTIMESPAN);
	SetWndsEnable(TRUE);
}


void RtuAttrHydrTransCtrl::OnBnClickedBtnQueryRainstardard()
{
	// TODO: 在此添加控件通知处理程序代码
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CHydrSendFrame appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息
			m_pSubStation->GetSysDataTrans_Hydr()->SCSW_hydrdata_EN_PI(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_Q_RAINSTARDARD, nElapse, NULL);
		}
	}
}

void RtuAttrHydrTransCtrl::OnBnClickedBtnSetRainstardard()
{
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{
			CHydrSendFrame appFrame;

			m_PswEdit.GetWindowText(m_strPsw);
			if (m_strPsw.GetLength() == 6)
			{
				appFrame.m_strPsw = m_strPsw;
			}
			else
			{
				MessageBox("密码必须为6位字符","错误",MB_OK|MB_ICONERROR);
				return;
			}

			SetWndsEnable(FALSE);
			m_blResult = FALSE;
	
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息
			CString var = "";
			m_let_rainStardard.GetWindowText(var);
			appFrame.m_setVar = atof(var);
			m_pSubStation->GetSysDataTrans_Hydr()->SCSW_hydrdata_EN_SET_PI(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_S_RAINSTARDARD, nElapse, NULL);
		}
	}
	
}

void RtuAttrHydrTransCtrl::OnBnClickedBtnQueryRainsensordpi()
{
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CHydrSendFrame appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息
			m_pSubStation->GetSysDataTrans_Hydr()->SCSW_hydrdata_EN_RD(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_Q_RAINSENSORDPI, nElapse, NULL);
		}
	}
}

void RtuAttrHydrTransCtrl::OnBnClickedBtnSetRainsensordpi()
{
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{

			CHydrSendFrame appFrame;
			m_PswEdit.GetWindowText(m_strPsw);
			if (m_strPsw.GetLength() == 6)
			{
				appFrame.m_strPsw = m_strPsw;
			}
			else
			{
				MessageBox("密码必须为6位字符","错误",MB_OK|MB_ICONERROR);
				return;
			}

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			
	
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息
			CString var = "";
			m_let_rainSensorDpi.GetWindowText(var);
			appFrame.m_setVar = atof(var);
			m_pSubStation->GetSysDataTrans_Hydr()->SCSW_hydrdata_EN_SET_RD(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_S_RAINSENSORDPI, nElapse, NULL);
		}
	}
}

void RtuAttrHydrTransCtrl::OnBnClickedBtnQuerySensorbase()
{
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CHydrSendFrame appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息
			
			CString strSensorNo = "";
			int sel = m_Cbx_SensorNo_rainbase.GetCurSel();
			if (sel == CB_ERR) return;
			m_Cbx_SensorNo_rainbase.GetLBText(sel, strSensorNo);
			appFrame.m_LPARME = atoi(strSensorNo);
			m_pSubStation->GetSysDataTrans_Hydr()->SCSW_hydrdata_EN_ZJ(&appFrame);


			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_Q_RAINSENSORBASE, nElapse, NULL);
		}
	}
}

void RtuAttrHydrTransCtrl::OnBnClickedBtnSetSensorbase()
{
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{
			CHydrSendFrame appFrame;

			m_PswEdit.GetWindowText(m_strPsw);
			if (m_strPsw.GetLength() == 6)
			{
				appFrame.m_strPsw = m_strPsw;
			}
			else
			{
				MessageBox("密码必须为6位字符","错误",MB_OK|MB_ICONERROR);
				return;
			}

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			

			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息
			CString var = "";
			m_let_rainBaseVar.GetWindowText(var);
			appFrame.m_setVar = atof(var);

			CString strSensorNo = "";
			int sel = m_Cbx_SensorNo_rainbase.GetCurSel();
			if (sel == CB_ERR) return;
			m_Cbx_SensorNo_rainbase.GetLBText(sel, strSensorNo);
			appFrame.m_LPARME = atoi(strSensorNo);

			m_pSubStation->GetSysDataTrans_Hydr()->SCSW_hydrdata_EN_SET_ZJ(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_S_RAINSENSORBASE, nElapse, NULL);
		}
	}
}

void RtuAttrHydrTransCtrl::OnBnClickedBtnQuerySensorpara()
{
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CHydrSendFrame appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息

			CString strSensorNo = "";
			int sel = m_Cbx_SensorNo_rainPara.GetCurSel();
			if (sel == CB_ERR) return;
			m_Cbx_SensorNo_rainPara.GetLBText(sel, strSensorNo);
			appFrame.m_LPARME = atoi(strSensorNo);

			m_pSubStation->GetSysDataTrans_Hydr()->SCSW_hydrdata_EN_ZT(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_Q_RAINSENSORPARA, nElapse, NULL);
		}
	}
}

void RtuAttrHydrTransCtrl::OnBnClickedBtnSetSensorpara()
{
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{

			CHydrSendFrame appFrame;
			m_PswEdit.GetWindowText(m_strPsw);
			if (m_strPsw.GetLength() == 6)
			{
				appFrame.m_strPsw = m_strPsw;
			}
			else
			{
				MessageBox("密码必须为6位字符","错误",MB_OK|MB_ICONERROR);
				return;
			}
			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息
			CString var = "";
			m_let_rainParaVar.GetWindowText(var);
			appFrame.m_setVar = atof(var);

			CString strSensorNo = "";
			int sel = m_Cbx_SensorNo_rainPara.GetCurSel();
			if (sel == CB_ERR) return;
			m_Cbx_SensorNo_rainPara.GetLBText(sel, strSensorNo);
			appFrame.m_LPARME = atoi(strSensorNo);

			m_pSubStation->GetSysDataTrans_Hydr()->SCSW_hydrdata_EN_SET_ZT(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_S_RAINSENSORPARA, nElapse, NULL);
		}
	}
}

void RtuAttrHydrTransCtrl::OnBnClickedBtnQuerySensordef()
{
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CHydrSendFrame appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息
			
			CString strSensorNo = "";
			int sel = m_Cbx_SensorNo_rainDef.GetCurSel();
			if (sel == CB_ERR) return;
			m_Cbx_SensorNo_rainDef.GetLBText(sel, strSensorNo);
			appFrame.m_LPARME = atoi(strSensorNo);

			m_pSubStation->GetSysDataTrans_Hydr()->SCSW_hydrdata_EN_ZF(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_Q_RAINDEFINE, nElapse, NULL);
		}
	}
}

void RtuAttrHydrTransCtrl::OnBnClickedBtnSetSensordef()
{
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{
			int nsel = m_cbx_rainDefVar.GetCurSel();
			if (CB_ERR == nsel)
			{
				MessageBox("请选择设置的公式定义参数","提示",MB_OK|MB_ICONERROR);
				return;
			}

			CHydrSendFrame appFrame;
			m_PswEdit.GetWindowText(m_strPsw);

			if (m_strPsw.GetLength() == 6)
			{
				appFrame.m_strPsw = m_strPsw;
			}
			else
			{
				MessageBox("密码必须为6位字符","错误",MB_OK|MB_ICONERROR);
				return;
			}

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			

			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息
	
			CString var = "";
			m_cbx_rainDefVar.GetWindowText(var);

			if ('+' == var)
			{
				appFrame.m_setVar = 1;
			}
			else if ('-' == var)
			{
				appFrame.m_setVar = 2;
			}

			CString strSensorNo = "";
			m_Cbx_SensorNo_rainDef.GetWindowText(strSensorNo);
			appFrame.m_LPARME = atoi(strSensorNo);

			m_pSubStation->GetSysDataTrans_Hydr()->SCSW_hydrdata_EN_SET_ZF(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_S_RAINDEFINE, nElapse, NULL);
		}
	}
}

void RtuAttrHydrTransCtrl::OnBnClickedBtnQueryHydrsendctrl()
{
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CHydrSendFrame appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息

			CString strSensorNo = "";
			int sel = m_Cbx_SensorNo_rainDef.GetCurSel();
			if (sel == CB_ERR) return;
			m_Cbx_SensorNo_rainDef.GetLBText(sel, strSensorNo);
			appFrame.m_LPARME = atoi(strSensorNo);

			CString level = "";
	        sel = m_Cbx_Level_sendStardard.GetCurSel();
			m_Cbx_Level_sendStardard.GetLBText(sel, level);
			const char* chLevel = (LPCTSTR)level;
			appFrame.m_RPARME = (int)(chLevel[0]);
			m_pSubStation->GetSysDataTrans_Hydr()->SCSW_hydrdata_EN_Zx(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_Q_RAINTRALSTARDARD, nElapse, NULL);
		}
	}
}

void RtuAttrHydrTransCtrl::OnBnClickedBtnSetHydrsendctrl()
{
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{
			CHydrSendFrame appFrame;
			m_PswEdit.GetWindowText(m_strPsw);

			if (m_strPsw.GetLength() == 6)
			{
				appFrame.m_strPsw = m_strPsw;
			}
			else
			{
				MessageBox("密码必须为6位字符","错误",MB_OK|MB_ICONERROR);
				return;
			}

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			

			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息
			CString var = "";
			m_let_SendStardardVar.GetWindowText(var);
			appFrame.m_setVar = atof(var);

			CString strSensorNo = "";
			int sel = m_Cbx_SensorNo_sendStardard.GetCurSel();
			if (sel == CB_ERR) return;
			m_Cbx_SensorNo_sendStardard.GetLBText(sel, strSensorNo);
			appFrame.m_LPARME = atoi(strSensorNo);

			CString level = "";
			sel = m_Cbx_Level_sendStardard.GetCurSel();
			m_Cbx_Level_sendStardard.GetLBText(sel, level);
			const char* chLevel = (LPCTSTR)level;
			appFrame.m_RPARME = (int)(chLevel[0]);

			m_pSubStation->GetSysDataTrans_Hydr()->SCSW_hydrdata_EN_SET_Zx(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_S_RAINTRALSTARDARD, nElapse, NULL);
		}
	}
}

void RtuAttrHydrTransCtrl::OnBnClickedBtnQueryHydrtimespan()
{
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CHydrSendFrame appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息
			CString strSensorNo = "";
			int sel = m_Cbx_SensorNo_rainDef.GetCurSel();
			if (sel == CB_ERR) return;
			m_Cbx_SensorNo_sendTimeSpan.GetLBText(sel, strSensorNo);
			appFrame.m_LPARME = atoi(strSensorNo);

			CString level = "";
			sel = m_Cbx_Level_sendTimeSpan.GetCurSel();
			m_Cbx_Level_sendTimeSpan.GetLBText(sel, level);
			const char* chLevel = (LPCTSTR)level;
			appFrame.m_RPARME = (int)(chLevel[0]);

			m_pSubStation->GetSysDataTrans_Hydr()->SCSW_hydrdata_EN_DZx(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_Q_RAINTIMESPAN, nElapse, NULL);
		}
	}
}

void RtuAttrHydrTransCtrl::OnBnClickedBtnSetHydrtimespan()
{
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{
			CHydrSendFrame appFrame;
			m_PswEdit.GetWindowText(m_strPsw);

			if (m_strPsw.GetLength() == 6)
			{
				appFrame.m_strPsw = m_strPsw;
			}
			else
			{
				MessageBox("密码必须为6位字符","错误",MB_OK|MB_ICONERROR);
				return;
			}

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			

			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息
			CString var = "";
			m_let_SendTimeSpanVar.GetWindowText(var);
			appFrame.m_setVar = atof(var);

			CString strSensorNo = "";
			int sel = m_Cbx_SensorNo_sendTimeSpan.GetCurSel();
			if (sel == CB_ERR) return;
			m_Cbx_SensorNo_sendTimeSpan.GetLBText(sel, strSensorNo);
			appFrame.m_LPARME = atoi(strSensorNo);

			CString level = "";
			sel = m_Cbx_Level_sendTimeSpan.GetCurSel();
			m_Cbx_Level_sendTimeSpan.GetLBText(sel, level);
			const char* chLevel = (LPCTSTR)level;
			appFrame.m_RPARME = (int)(chLevel[0]);

			m_pSubStation->GetSysDataTrans_Hydr()->SCSW_hydrdata_EN_SET_DZx(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_S_RAINTIMESPAN, nElapse, NULL);
		}
	}
}

void RtuAttrHydrTransCtrl::OnEnChangeLetPsw()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void RtuAttrHydrTransCtrl::OnBnClickedBtnChecktime()
{
	// TODO: 在此添加控件通知处理程序代码
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{

			CHydrSendFrame appFrame;
			m_PswEdit.GetWindowText(m_strPsw);
			if (m_strPsw.GetLength() == 6)
			{
				appFrame.m_strPsw = m_strPsw;
			}
			else
			{
				MessageBox("密码必须为6位字符","错误",MB_OK|MB_ICONERROR);
				return;
			}

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息		
			m_pSubStation->GetSysDataTrans_Hydr()->SCSW_hydrdata_EN_SET_TT(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_S_TIME, nElapse, NULL);
		}
	}
}
