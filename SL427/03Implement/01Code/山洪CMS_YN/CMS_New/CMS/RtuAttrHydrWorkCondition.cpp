// RtuAttrHydrWorkCondition.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuAttrHydrWorkCondition.h"

#define  W_TIMER_Q_SENSORVAR            0x01		
#define  W_TIMER_Q_MEMORYMARGIN         0x02
#define  W_TIMER_Q_EVIORTEMPERATION     0x03		
#define  W_TIMER_Q_SOLARCELLVOLTAGE     0x04	
#define  W_TIMER_Q_STORAGEVOLTAGE       0x05		
#define  W_TIMER_Q_DATASTORAGEPTR       0x06		
#define  W_TIMER_Q_RAINCNT              0x07 //余量累计值	
#define  W_TIMER_Q_SENSORINSTALLSTATUS  0x08

#define  W_TIMER_ER_SENSORVAR            0x101		
#define  W_TIMER_ER_MEMORYMARGIN         0x102
#define  W_TIMER_ER_EVIORTEMPERATION     0x103		
#define  W_TIMER_ER_SOLARCELLVOLTAGE     0x104	
#define  W_TIMER_ER_STORAGEVOLTAGE       0x105		
#define  W_TIMER_ER_DATASTORAGEPTR       0x106		
#define  W_TIMER_ER_RAINCNT              0x107 //余量累计值	
#define  W_TIMER_ER_SENSORINSTALLSTATUS  0x108
// CRtuAttrHydrWorkCondition 对话框


IMPLEMENT_DYNAMIC(CRtuAttrHydrWorkCondition, CDialog)

CRtuAttrHydrWorkCondition::CRtuAttrHydrWorkCondition(CWnd* pParent /*=NULL*/)
	: CDialog(CRtuAttrHydrWorkCondition::IDD, pParent)
{
	m_pParentDlg = NULL;
	m_pSubStation = NULL;

}

CRtuAttrHydrWorkCondition::~CRtuAttrHydrWorkCondition()
{
}

void CRtuAttrHydrWorkCondition::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CBX_SENSORNO, m_cbx_QuerySensorNO);
	DDX_Control(pDX, IDC_LET_SENSORVAR,  m_let_SensorVar);
	DDX_Control(pDX, IDC_BTN_SENSORVAR, m_btn_QuerySensorVar);
	DDX_Control(pDX, IDC_LET_MEMORY, m_let_MemoryMargin);
	
	DDX_Control(pDX, IDC_BTN_MEMORY, m_btn_QueryMemoryMargin);
	DDX_Control(pDX, IDC_LET_TEMPERATURE, m_let_EvoirTemperature);
	DDX_Control(pDX, IDC_BTN_TEMPERATION, m_btn_QueryEvoirTemperature);
	DDX_Control(pDX, IDC_LET_SOLARV, m_let_SolarCellVoltage);
	DDX_Control(pDX, IDC_BTN_SOLARV, m_btn_QuerySolarCellVoltage);
	DDX_Control(pDX, IDC_LET_STORAGE, m_let_StorageVoltage);
	DDX_Control(pDX, IDC_BTN_STORAGE, m_btn_QueryStorageVoltage);

	DDX_Control(pDX, IDC_BTN_RAINCNT, m_btn_RainCnt);
	DDX_Control(pDX, IDC_LET_RAINCNT, m_let_RainCnt);
	DDX_Control(pDX, IDC_BTN_SETUPSTATUS, m_btn_SetupStatus);
	DDX_Control(pDX, IDC_LET_SETUPSTATUS, m_let_SetupStatus);
	DDX_Control(pDX, IDC_BTN_MEMORYPTR, m_btn_QueryStoragePtr);
	DDX_Control(pDX, IDC_LET_MEMORYPTR, m_let_QueryStoragePtr);

	DDX_Text(pDX, IDC_LET_SENSORVAR, m_str_SensorVar);
	DDV_MaxChars(pDX, m_str_SensorVar, 7);

 	DDX_Text(pDX, IDC_LET_MEMORY, m_str_MemoryMargin);
	DDV_MaxChars(pDX, m_str_MemoryMargin, 7);

	DDX_Text(pDX, IDC_LET_TEMPERATURE, m_str_EvoirTemperature);
	DDV_MaxChars(pDX, m_str_EvoirTemperature, 7);

	DDX_Text(pDX, IDC_LET_SOLARV, m_str_SolarCellVoltage);
	DDV_MaxChars(pDX, m_str_SolarCellVoltage, 7);

	DDX_Text(pDX, IDC_LET_STORAGE, m_str_StorageVoltage);
	DDV_MaxChars(pDX, m_str_StorageVoltage, 7);

	DDX_Text(pDX, IDC_LET_RAINCNT, m_str_RainCnt);
	DDV_MaxChars(pDX, m_str_RainCnt, 7);

	DDX_Text(pDX, IDC_LET_SETUPSTATUS, m_str_SetupStatus);
	DDV_MaxChars(pDX, m_str_SetupStatus, 7);

	DDX_Text(pDX, IDC_LET_MEMORYPTR, m_str_QueryStoragePtr);	
	DDV_MaxChars(pDX, m_str_QueryStoragePtr, 7);

}


BEGIN_MESSAGE_MAP(CRtuAttrHydrWorkCondition, CDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_SENSORVAR, &CRtuAttrHydrWorkCondition::OnBnClickedBtnSensorvar)
	ON_BN_CLICKED(IDC_BTN_MEMORY, &CRtuAttrHydrWorkCondition::OnBnClickedBtnMemory)
	ON_BN_CLICKED(IDC_BTN_TEMPERATION, &CRtuAttrHydrWorkCondition::OnBnClickedBtnTemperation)
	ON_BN_CLICKED(IDC_BTN_SOLARV, &CRtuAttrHydrWorkCondition::OnBnClickedBtnSolarv)
	ON_BN_CLICKED(IDC_BTN_STORAGE, &CRtuAttrHydrWorkCondition::OnBnClickedBtnStorage)
	ON_CBN_SELCHANGE(IDC_CBX_SENSORNO, &CRtuAttrHydrWorkCondition::OnSelSensorNoChange)
	ON_BN_CLICKED(IDC_BTN_MEMORYPTR, &CRtuAttrHydrWorkCondition::OnBnClickedBtnMemoryptr)
	ON_BN_CLICKED(IDC_BTN_RAINCNT, &CRtuAttrHydrWorkCondition::OnBnClickedBtnRaincnt)
	ON_BN_CLICKED(IDC_BTN_SETUPSTATUS, &CRtuAttrHydrWorkCondition::OnBnClickedBtnSetupstatus)
END_MESSAGE_MAP()


// CRtuAttrHydrWorkCondition 消息处理程序

BOOL CRtuAttrHydrWorkCondition::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_cbx_QuerySensorNO.SetCurSel(0);
	return TRUE;  
}

void CRtuAttrHydrWorkCondition::SetParentDlg(CRtuPropertyDlg* pDlg)
{
	m_pParentDlg = pDlg;	
}


void CRtuAttrHydrWorkCondition::Update()
{
	CWorkCondition *pData = NULL;
	if (m_pParentDlg)
		m_pSubStation = m_pParentDlg->GetRtuStation();	

	if (m_pSubStation != NULL)
	{
		pData = m_pSubStation->GetSensorVar();
		if (pData != NULL)
		{
			CString str = "";
			str.Format("%d", pData->GetData());
			m_let_SensorVar.SetWindowText(str);

		}

		pData = NULL;
		pData = m_pSubStation->GetMemoryMargin();
		if (pData != NULL)
		{
			CString str = "";
			str.Format("%d", pData->GetData());
			m_let_MemoryMargin.SetWindowText(str);

		}

		pData = NULL;
		pData = m_pSubStation->GetEviorTemperation();
		if (pData != NULL)
		{
			CString str = "";
			str.Format("%d", pData->GetData());
			m_let_EvoirTemperature.SetWindowText(str);

		}

		pData = NULL;
		pData = m_pSubStation->GetSolarCellVoltage();
		if (pData != NULL)
		{
			CString str = "";
			str.Format("%d", pData->GetData());
			m_let_SolarCellVoltage.SetWindowText(str);

		}

		pData = NULL;
		pData = m_pSubStation->GetStorageVoltage();
		if (pData != NULL)
		{
			CString str = "";
			str.Format("%d", pData->GetData());
			m_let_StorageVoltage.SetWindowText(str);

		}

		pData = NULL;
		pData = m_pSubStation->GetRainCntVar();
		if (pData != NULL)
		{
			CString str = "";
			str.Format("%d", pData->GetData());
			m_let_RainCnt.SetWindowText(str);

		}

		pData = NULL;
		pData = m_pSubStation->GetSensorInstallStatus();
		if (pData != NULL)
		{
			CString str = "";
			str.Format("%d", pData->GetData());
			m_let_SetupStatus.SetWindowText(str);

		}

		pData = NULL;
		pData = m_pSubStation->GetDataStoragePtr();
		if (pData != NULL)
		{
			CString str = "";
			str.Format("%d", pData->GetData());
			m_let_QueryStoragePtr.SetWindowText(str);

		}

	}

}

void CRtuAttrHydrWorkCondition::Update(int ret)
{
	CWorkCondition *pData = NULL;
	if (m_pParentDlg)
		m_pSubStation = m_pParentDlg->GetRtuStation();	

	if (m_pSubStation != NULL)
	{
		switch (ret)
		{
		case scsw_workconditionSensorVar:
			{
				pData = m_pSubStation->GetSensorVar();
				if (pData != NULL)
				{
					CString str = "";
					str.Format("%.2f", pData->GetData());
					m_let_SensorVar.SetWindowText(str);

				}
			}
			break;
		case scsw_workconditionMemoryMargin:
			{
				pData = m_pSubStation->GetMemoryMargin();
				if (pData != NULL)
				{
					CString str = "";
					str.Format("%.2f", pData->GetData());
					m_let_MemoryMargin.SetWindowText(str);

				}

			}
			break;
		case scsw_workconditionEvoirTemperature:
			{
				pData = m_pSubStation->GetEviorTemperation();
				if (pData != NULL)
				{
					CString str = "";
					str.Format("%.2f", pData->GetData());
					m_let_EvoirTemperature.SetWindowText(str);

				}

			}
			break;
		case scsw_workconditionSolarCellVoltage:
			{
				pData = m_pSubStation->GetSolarCellVoltage();
				if (pData != NULL)
				{
					CString str = "";
					str.Format("%.2f", pData->GetData());
					m_let_SolarCellVoltage.SetWindowText(str);

				}

			}
			break;
		case scsw_workconditionStorageVoltage:
			{
				pData = m_pSubStation->GetStorageVoltage();
				if (pData != NULL)
				{
					CString str = "";
					str.Format("%.2f", pData->GetData());
					m_let_StorageVoltage.SetWindowText(str);

				}

			}
			break;
		case scsw_workconditionRainCntVar:
			{
				pData = m_pSubStation->GetRainCntVar();
				if (pData != NULL)
				{
					CString str = "";
					str.Format("%.2f", pData->GetData());
					m_let_RainCnt.SetWindowText(str);

				}

			}
			break;
		case scsw_workconditionSensorInstallStatus:
			{
				pData = m_pSubStation->GetSensorInstallStatus();
				if (pData != NULL)
				{
					CString str = "";
					str.Format("%.2f", pData->GetData());
					m_let_SetupStatus.SetWindowText(str);

				}

			}
			break;
		case scsw_workconditionMemoryPtr:
			{
				pData = m_pSubStation->GetDataStoragePtr();
				if (pData != NULL)
				{
					CString str = "";
					str.Format("%.2f", pData->GetData());
					m_let_QueryStoragePtr.SetWindowText(str);

				}

			}
			break;
		}
	}
}


void CRtuAttrHydrWorkCondition::OnTimer(UINT_PTR nIDEvent)
{
	CString strLog= "";
	_Log_level_type LogLevel = Normal_Log_Message;


	switch (nIDEvent)
	{
	case W_TIMER_Q_SENSORVAR:            
		{
			if (m_blResult == TRUE)
				strLog = "查询水位传感器读数 成功!";
			else if ( m_blResult == FALSE)
			{
				strLog = "查询水位传感器读数 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_blResult == 0xff)
			{
				strLog = "查询水位传感器读数 错误!";
				LogLevel = Warning_log_Message;
			}

		}
		break;
	case W_TIMER_Q_MEMORYMARGIN:
		{
			if (m_blResult == TRUE)
				strLog = "查询数据存储器余量 成功!";
			else if (m_blResult == FALSE)
			{
				strLog = "查询数据存储器余量 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_blResult == 0xff)
			{
				strLog = "查询数据存储器余量 错误!";
				LogLevel = Warning_log_Message;
			}
		}
		break;
	case W_TIMER_Q_EVIORTEMPERATION:
		{
			if (m_blResult == TRUE)
				strLog = "查询环境温度 成功!";
			else if (m_blResult == FALSE)
			{
				strLog = "查询环境温度 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_blResult == 0xff)
			{
				strLog = "查询环境温度 错误!";
				LogLevel = Warning_log_Message;
			}
		}
		break;
	case W_TIMER_Q_SOLARCELLVOLTAGE:
		{
			if (m_blResult == TRUE)
				strLog = "查询太阳能电池电压 成功!";
			else if (m_blResult == FALSE)
			{
				strLog = "查询太阳能电池电压 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_blResult == 0xff)
			{
				strLog = "查询太阳能电池电压 错误!";
				LogLevel = Warning_log_Message;
			}
		}
		break;
	case W_TIMER_Q_STORAGEVOLTAGE:
		{
			if (m_blResult == TRUE)
				strLog = "查询蓄电池电压 成功!";
			else if (m_blResult == FALSE)
			{
				strLog = "查询蓄电池电压 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_blResult == 0xff)
			{
				strLog = "查询蓄电池电压 错误!";
				LogLevel = Warning_log_Message;
			}
		}
		break;
	case W_TIMER_Q_DATASTORAGEPTR:
		{
			if (m_blResult == TRUE)
				strLog = "数据存储器指针 成功!";
			else if (m_blResult == FALSE)
			{
				strLog = "数据存储器指针 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_blResult == 0xff)
			{
				strLog = "数据存储器指针 错误!";
				LogLevel = Warning_log_Message;
			}
		}
		break;
	case W_TIMER_Q_RAINCNT:
		{
			if (m_blResult == TRUE)
				strLog = "查询雨量计数器累计值 成功!";
			else if (m_blResult == FALSE)
			{
				strLog = "查询雨量计数器累计值 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_blResult == 0xff)
			{
				strLog = "查询雨量计数器累计值 错误!";
				LogLevel = Warning_log_Message;
			}
		}
		break;
	case W_TIMER_Q_SENSORINSTALLSTATUS:
		{
			if (m_blResult == TRUE)
				strLog = "查询传感器安装状态 成功!";
			else if (m_blResult == FALSE)
			{
				strLog = "查询传感器安装状态 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_blResult == 0xff)
			{
				strLog = "查询传感器安装状态 错误!";
				LogLevel = Warning_log_Message;
			}
		}
		break;
	}
	KillTimer(nIDEvent);
	SetWndsEnable(TRUE);
	if (m_pParentDlg && m_pParentDlg->GetSafeHwnd())
		m_pParentDlg->AddLog(strLog,(PSYSTEMTIME)NULL,LogLevel);

	if (m_pSubStation)
		m_pSubStation->InsertLogMessage(strLog,LogLevel,NULL);

	CDialog::OnTimer(nIDEvent);
}

void CRtuAttrHydrWorkCondition::SetWndsEnable(BOOL blEnable /* = TRUE */)
{
	m_btn_QuerySensorVar.EnableWindow(blEnable);
	m_btn_QueryMemoryMargin.EnableWindow(blEnable);
	m_btn_QueryEvoirTemperature.EnableWindow(blEnable);
	m_btn_QuerySolarCellVoltage.EnableWindow(blEnable);
	m_btn_QueryStorageVoltage.EnableWindow(blEnable);
	m_btn_QueryStoragePtr.EnableWindow(blEnable);
	m_btn_RainCnt.EnableWindow(blEnable);
	m_btn_SetupStatus.EnableWindow(blEnable);
	m_cbx_QuerySensorNO.EnableWindow(blEnable);
	m_let_SensorVar.EnableWindow(blEnable);
	m_let_MemoryMargin.EnableWindow(blEnable);
	m_let_EvoirTemperature.EnableWindow(blEnable);
	m_let_SolarCellVoltage.EnableWindow(blEnable);
	m_let_StorageVoltage.EnableWindow(blEnable);
	m_let_RainCnt.EnableWindow(blEnable);
	m_let_SetupStatus.EnableWindow(blEnable);
	m_let_QueryStoragePtr.EnableWindow(blEnable);
}


void CRtuAttrHydrWorkCondition::UpdateDlg(int attrType,BOOL blResult)
{
	m_blResult = blResult;
	if (attrType == scsw_workconditionSensorVar )
	{
		SendMessage(WM_TIMER,W_TIMER_Q_SENSORVAR,NULL);
	}
	else if (attrType == scsw_workconditionMemoryMargin)
	{
		SendMessage(WM_TIMER,W_TIMER_Q_MEMORYMARGIN,NULL);
	}
	else if (attrType == scsw_workconditionEvoirTemperature)
	{
		SendMessage(WM_TIMER,W_TIMER_Q_EVIORTEMPERATION,NULL);
	}
	else if (attrType == scsw_workconditionSolarCellVoltage)
	{
		SendMessage(WM_TIMER,W_TIMER_Q_SOLARCELLVOLTAGE,NULL);
	}
	else if (attrType == scsw_workconditionStorageVoltage)
	{
		SendMessage(WM_TIMER,W_TIMER_Q_STORAGEVOLTAGE,NULL);
	}
	else if (attrType == scsw_workconditionRainCntVar)
	{
		SendMessage(WM_TIMER,W_TIMER_Q_RAINCNT,NULL);
	}
	else if (attrType == scsw_workconditionSensorInstallStatus)
	{
		SendMessage(WM_TIMER,W_TIMER_Q_SENSORINSTALLSTATUS,NULL);
	}
	else if (attrType == scsw_workconditionMemoryPtr)
	{
		SendMessage(WM_TIMER,W_TIMER_Q_DATASTORAGEPTR,NULL);
	}
}

void CRtuAttrHydrWorkCondition::KillAllTimers()
{
	KillTimer(W_TIMER_Q_SENSORVAR);
	KillTimer(W_TIMER_Q_MEMORYMARGIN);
	KillTimer(W_TIMER_Q_EVIORTEMPERATION);
	KillTimer(W_TIMER_Q_SOLARCELLVOLTAGE);
	KillTimer(W_TIMER_Q_STORAGEVOLTAGE);
	KillTimer(W_TIMER_Q_DATASTORAGEPTR);
	KillTimer(W_TIMER_Q_RAINCNT);
	KillTimer(W_TIMER_Q_SENSORINSTALLSTATUS);
	SetWndsEnable(TRUE);
}

void CRtuAttrHydrWorkCondition::OnBnClickedBtnSensorvar()
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
			CString var = "";
			m_let_SensorVar.GetWindowText(var);
			appFrame.m_setVar = atof(var);

			CString strSensor = "";
			m_cbx_QuerySensorNO.GetWindowText(strSensor);
			if (strSensor.IsEmpty())
			{
				strSensor = "99";
			}
			appFrame.m_LPARME = atoi(strSensor);
			m_pSubStation->GetSysDataTrans_Hydr()->SCSW_hydrdata_EN_ZR(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_Q_SENSORVAR, nElapse, NULL);
		}
	}
}

void CRtuAttrHydrWorkCondition::OnBnClickedBtnMemory()
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
			CString var = "";
			m_let_MemoryMargin.GetWindowText(var);
			appFrame.m_setVar = atof(var);
			m_pSubStation->GetSysDataTrans_Hydr()->SCSW_hydrdata_EN_MF(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_Q_MEMORYMARGIN, nElapse, NULL);
		}
	}
}

void CRtuAttrHydrWorkCondition::OnBnClickedBtnTemperation()
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
			CString var = "";
			m_let_EvoirTemperature.GetWindowText(var);
			appFrame.m_setVar = atof(var);
			m_pSubStation->GetSysDataTrans_Hydr()->SCSW_hydrdata_EN_ET(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_Q_EVIORTEMPERATION, nElapse, NULL);
		}
	}
}

void CRtuAttrHydrWorkCondition::OnBnClickedBtnSolarv()
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
			CString var = "";
			m_let_SolarCellVoltage.GetWindowText(var);
			appFrame.m_setVar = atof(var);
			m_pSubStation->GetSysDataTrans_Hydr()->SCSW_hydrdata_EN_SV(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_Q_SOLARCELLVOLTAGE, nElapse, NULL);
		}
	}
}

void CRtuAttrHydrWorkCondition::OnBnClickedBtnStorage()
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
			CString var = "";
			m_let_StorageVoltage.GetWindowText(var);
			appFrame.m_setVar = atof(var);
			m_pSubStation->GetSysDataTrans_Hydr()->SCSW_hydrdata_EN_BV(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_Q_STORAGEVOLTAGE, nElapse, NULL);
		}
	}
}

void CRtuAttrHydrWorkCondition::OnSelSensorNoChange()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CRtuAttrHydrWorkCondition::OnBnClickedBtnMemoryptr()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CHydrSendFrame appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息
			CString var = "";
			m_let_QueryStoragePtr.GetWindowText(var);
			appFrame.m_setVar = atof(var);
			m_pSubStation->GetSysDataTrans_Hydr()->SCSW_hydrdata_EN_ML(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_Q_DATASTORAGEPTR, nElapse, NULL);
		}
	}
}

void CRtuAttrHydrWorkCondition::OnBnClickedBtnRaincnt()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CHydrSendFrame appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息
			CString var = "";
			m_let_RainCnt.GetWindowText(var);
			appFrame.m_setVar = atof(var);
			m_pSubStation->GetSysDataTrans_Hydr()->SCSW_hydrdata_EN_RC(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_Q_RAINCNT, nElapse, NULL);
		}
	}
}

void CRtuAttrHydrWorkCondition::OnBnClickedBtnSetupstatus()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CHydrSendFrame appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息
			CString var = "";
			m_let_SetupStatus.GetWindowText(var);
			appFrame.m_setVar = atof(var);
			m_pSubStation->GetSysDataTrans_Hydr()->SCSW_hydrdata_EN_SS(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_Q_SENSORINSTALLSTATUS, nElapse, NULL);
		}
	}
}
