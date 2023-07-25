// RtuAttrHydrInterfaceInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuAttrHydrInterfaceInfo.h"


// CRtuAttrHydrInterfaceInfo 对话框
#define W_TIMER_Q_GSM   0x01
#define W_TIMER_S_GSM   0x02
#define W_TIMER_D_GSM   0x03

#define W_TIMER_Q_BD   0x04
#define W_TIMER_S_BD   0x05
#define W_TIMER_D_BD   0x06

#define W_TIMER_Q_IP   0x07
#define W_TIMER_S_IP   0x08
#define W_TIMER_D_IP   0x09

#define W_TIMER_Q_PORT  0x10
#define W_TIMER_S_PORT  0x11

#define W_TIMER_Q_IP2   0x17
#define W_TIMER_S_IP2   0x18
#define W_TIMER_D_IP2   0x19

IMPLEMENT_DYNAMIC(CRtuAttrHydrInterfaceInfo, CDialog)

CRtuAttrHydrInterfaceInfo::CRtuAttrHydrInterfaceInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CRtuAttrHydrInterfaceInfo::IDD, pParent)
{
	m_pParentDlg = NULL;
	m_pSubStation = NULL;
	m_blResult = TRUE;
}

CRtuAttrHydrInterfaceInfo::~CRtuAttrHydrInterfaceInfo()
{
}

void CRtuAttrHydrInterfaceInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_QUERY_GSM, m_btn_QueryGSM);
	DDX_Control(pDX, IDC_BTN_DEL_GSM, m_btn_DelGSM);
	DDX_Control(pDX, IDC_BTN_SET_GSM, m_btn_SetGSM);
	DDX_Control(pDX, IDC_CBX_GSM, m_Cbx_GSM);
	DDX_Control(pDX, IDC_LET_GSM, m_Let_GSM);
	DDX_Control(pDX, IDC_PSW2, m_Let_PSW);

	DDX_Control(pDX, IDC_CBX_BD, m_Cbx_BD);
	DDX_Control(pDX, IDC_LET_BD, m_Let_BD);
	DDX_Control(pDX, IDC_BTN_QUERY_BD, m_btn_QueryBD);
	DDX_Control(pDX, IDC_BTN_DEL_BD, m_btn_DelBD);
	DDX_Control(pDX, IDC_BTN_SET_BD, m_btn_SetBD);

	DDX_Control(pDX, IDC_CBX_IP, m_Cbx_IP);
	DDX_Control(pDX, IDC_LET_IP, m_Let_IP);
	DDX_Control(pDX, IDC_CBX_IP2, m_Cbx_IP2);
	DDX_Control(pDX, IDC_LET_IP2, m_Let_IP2);
	DDX_Control(pDX, IDC_BTN_QUERY_IP, m_btn_QueryIP);
	DDX_Control(pDX, IDC_BTN_DEL_IP, m_btn_DelIP);
	DDX_Control(pDX, IDC_BTN_SET_IP, m_btn_SetIP);

	DDX_Control(pDX, IDC_BTN_QUERY_IP2, m_btn_QueryIP2);
	DDX_Control(pDX, IDC_BTN_DEL_IP2, m_btn_DelIP2);
	DDX_Control(pDX, IDC_BTN_SET_IP2, m_btn_SetIP2);


	DDX_Control(pDX, IDC_CBX_PORT, m_Cbx_Port);
	DDX_Control(pDX, IDC_LET_PORT, m_Let_Port);
	DDX_Control(pDX, IDC_BTN_QUERY_PORT, m_btn_QueryPort);
	DDX_Control(pDX, IDC_BTN_SET_PORT, m_btn_SetPort);

	DDX_Text(pDX, IDC_PSW2, m_str_psw);
	DDV_MaxChars(pDX, m_str_psw, 6);

	DDX_Text(pDX, IDC_LET_GSM, m_str_GSM);
	DDV_MaxChars(pDX, m_str_GSM, 30);

	DDX_Text(pDX, IDC_LET_BD, m_str_BD);
	DDV_MaxChars(pDX, m_str_BD, 30);

	DDX_Text(pDX, IDC_LET_IP, m_str_IP);
	DDV_MaxChars(pDX, m_str_IP, 40);

	DDX_Text(pDX, IDC_LET_IP2, m_str_IP2);
	DDV_MaxChars(pDX, m_str_IP2, 30);

	DDX_Text(pDX, IDC_LET_PORT, m_str_Port);
	DDV_MaxChars(pDX, m_str_Port, 7);
}


BEGIN_MESSAGE_MAP(CRtuAttrHydrInterfaceInfo, CDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_QUERY_GSM, &CRtuAttrHydrInterfaceInfo::OnBnClickedBtnQueryGSM)
	ON_BN_CLICKED(IDC_BTN_DEL_GSM, &CRtuAttrHydrInterfaceInfo::OnBnClickedBtnDelGSM)
	ON_BN_CLICKED(IDC_BTN_SET_GSM, &CRtuAttrHydrInterfaceInfo::OnBnClickedBtnSetGSM)
	ON_BN_CLICKED(IDC_BTN_QUERY_BD, &CRtuAttrHydrInterfaceInfo::OnBnClickedBtnQueryBD)
	ON_BN_CLICKED(IDC_BTN_DEL_BD, &CRtuAttrHydrInterfaceInfo::OnBnClickedBtnDelBD)
	ON_BN_CLICKED(IDC_BTN_SET_BD, &CRtuAttrHydrInterfaceInfo::OnBnClickedBtnSetBD)
	ON_BN_CLICKED(IDC_BTN_QUERY_IP, &CRtuAttrHydrInterfaceInfo::OnBnClickedBtnQueryIP)
	ON_BN_CLICKED(IDC_BTN_DEL_IP, &CRtuAttrHydrInterfaceInfo::OnBnClickedBtnDelIP)
	ON_BN_CLICKED(IDC_BTN_SET_IP, &CRtuAttrHydrInterfaceInfo::OnBnClickedBtnSetIP)
	ON_BN_CLICKED(IDC_BTN_QUERY_PORT, &CRtuAttrHydrInterfaceInfo::OnBnClickedBtnQueryPort)
	ON_BN_CLICKED(IDC_BTN_SET_PORT, &CRtuAttrHydrInterfaceInfo::OnBnClickedBtnSetPort)
	ON_BN_CLICKED(IDC_BTN_QUERY_IP2, &CRtuAttrHydrInterfaceInfo::OnBnClickedBtnQueryIp2)
	ON_BN_CLICKED(IDC_BTN_DEL_IP2, &CRtuAttrHydrInterfaceInfo::OnBnClickedBtnDelIp2)
	ON_BN_CLICKED(IDC_BTN_SET_IP2, &CRtuAttrHydrInterfaceInfo::OnBnClickedBtnSetIp2)
END_MESSAGE_MAP()

void CRtuAttrHydrInterfaceInfo::SetWndsEnable(BOOL blEnable)
{
	m_btn_QueryGSM.EnableWindow(blEnable);
	m_btn_SetGSM.EnableWindow(blEnable);
	m_btn_DelGSM.EnableWindow(blEnable);
	m_Cbx_GSM.EnableWindow(blEnable);
	m_Let_GSM.EnableWindow(blEnable);

	m_btn_QueryBD.EnableWindow(blEnable);
	m_btn_SetBD.EnableWindow(blEnable);
	m_btn_DelBD.EnableWindow(blEnable);
	m_Cbx_BD.EnableWindow(blEnable);
	m_Let_BD.EnableWindow(blEnable);

	m_btn_QueryIP.EnableWindow(blEnable);
	m_btn_SetIP.EnableWindow(blEnable);
	m_btn_DelIP.EnableWindow(blEnable);

	m_btn_QueryIP2.EnableWindow(blEnable);
	m_btn_SetIP2.EnableWindow(blEnable);
	m_btn_DelIP2.EnableWindow(blEnable);

	m_Cbx_IP.EnableWindow(blEnable);
	m_Cbx_IP2.EnableWindow(blEnable);
	m_Let_IP.EnableWindow(blEnable);
	m_Let_IP2.EnableWindow(blEnable);

	m_btn_QueryPort.EnableWindow(blEnable);
	m_btn_SetPort.EnableWindow(blEnable);
	m_Cbx_Port.EnableWindow(blEnable);
	m_Let_Port.EnableWindow(blEnable);
	m_Let_PSW.EnableWindow(blEnable);
}

BOOL CRtuAttrHydrInterfaceInfo::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_Cbx_GSM.SetCurSel(0);
	m_Cbx_BD.SetCurSel(0);
	m_Cbx_IP.SetCurSel(0);
	m_Cbx_IP2.SetCurSel(0);
	m_Cbx_Port.SetCurSel(0);

	return TRUE;

}

void CRtuAttrHydrInterfaceInfo::SetParentDlg(CRtuPropertyDlg* pDlg)
{
	m_pParentDlg = pDlg;
}

void CRtuAttrHydrInterfaceInfo::Update()
{
	CInterfaceInfo *pData = NULL;
	if (m_pParentDlg)
		m_pSubStation = m_pParentDlg->GetRtuStation();	

	if (m_pSubStation != NULL)
	{
		pData = m_pSubStation->GetGSMValue();
		if (pData != NULL)
		{
			m_Let_GSM.SetWindowText(pData->GetData());
		}

		pData = NULL;
		pData = m_pSubStation->GetBDValue();
		if (pData != NULL)
		{
			m_Let_BD.SetWindowText(pData->GetData());
		}

		pData = NULL;
		pData = m_pSubStation->GetIPValue();
		if (pData != NULL)
		{
			m_Let_IP.SetWindowText(pData->GetData());
		}

		pData = NULL;
		pData = m_pSubStation->GetIPValue2();
		if (pData != NULL)
		{
			m_Let_IP2.SetWindowText(pData->GetData());
		}

		pData = NULL;
		pData = m_pSubStation->GetPortValue();
		if (pData != NULL)
		{
			m_Let_Port.SetWindowText(pData->GetData());

		}
	}
}

void CRtuAttrHydrInterfaceInfo::Update(int ret)
{

	CInterfaceInfo *pData = NULL;
	if (m_pParentDlg)
		m_pSubStation = m_pParentDlg->GetRtuStation();	

	if (NULL == m_pSubStation)
		return;

	switch(ret)
	{
	case scsw_interface_GSM:
		{
			pData = m_pSubStation->GetGSMValue();
			if (pData != NULL)
			{
				m_Let_GSM.SetWindowText(pData->GetData());
			}
		}
		break;
	case scsw_interface_BD:
		{
			pData = NULL;
			pData = m_pSubStation->GetBDValue();
			if (pData != NULL)
			{
				m_Let_BD.SetWindowText(pData->GetData());
			}
		}
		break;
	case scsw_interface_IP:
		{
			pData = NULL;
			pData = m_pSubStation->GetIPValue();
			if (pData != NULL)
			{
				m_Let_IP.SetWindowText(pData->GetData());
			}
		}
		break;
	case scsw_interface_IP2:
		{
			pData = NULL;
			pData = m_pSubStation->GetIPValue2();
			if (pData != NULL)
			{
				m_Let_IP2.SetWindowText(pData->GetData());
			}
		}
		break;
	case scsw_interface_PORT:
		{
			pData = NULL;
			pData = m_pSubStation->GetPortValue();
			if (pData != NULL)
			{
				m_Let_Port.SetWindowText(pData->GetData());

			}
		}
		break;
	}
}

void CRtuAttrHydrInterfaceInfo::UpdateDlg(int attrType,BOOL blResult)
{
	m_blResult = blResult;
	if (attrType == scsw_interface_GSM )
	{
		SendMessage(WM_TIMER,W_TIMER_Q_GSM, NULL);
	}
	else if (attrType == scsw_interface_BD )
	{
		SendMessage(WM_TIMER,W_TIMER_Q_BD,NULL);
	}
	else if (attrType == scsw_interface_IP )
	{
		SendMessage(WM_TIMER,W_TIMER_Q_IP,NULL);
	}
	else if (attrType == scsw_interface_IP2)
	{
		SendMessage(WM_TIMER, W_TIMER_Q_IP2, NULL);
	}
	else if (attrType == scsw_interface_PORT )
	{
		SendMessage(WM_TIMER,W_TIMER_Q_PORT,NULL);
	}
	else if (attrType == scsw_interface_GSM_Set )
	{
		SendMessage(WM_TIMER,W_TIMER_S_GSM,NULL);
	}
	else if (attrType == scsw_interface_BD_Set  )
	{
		SendMessage(WM_TIMER,W_TIMER_S_BD,NULL);
	}
	else if (attrType == scsw_interface_IP_Set  )
	{
		SendMessage(WM_TIMER,W_TIMER_S_IP,NULL);
	}
	else if (attrType == scsw_interface_PORT_Set)
	{
		SendMessage(WM_TIMER,W_TIMER_S_PORT,NULL);
	}
	else if (attrType == scsw_interface_GSM_Del )
	{
		SendMessage(WM_TIMER,W_TIMER_D_GSM,NULL);
	}
	else if (attrType == scsw_interface_BD_Del )
	{
		SendMessage(WM_TIMER,W_TIMER_D_BD,NULL);
	}
	else if (attrType == scsw_interface_IP_Del )
	{
		SendMessage(WM_TIMER,W_TIMER_D_IP,NULL);
	}
	
}

void CRtuAttrHydrInterfaceInfo::KillAllTimers()
{
	KillTimer(W_TIMER_Q_GSM);
	KillTimer(W_TIMER_S_GSM);
	KillTimer(W_TIMER_D_GSM);
	KillTimer(W_TIMER_Q_BD);
	KillTimer(W_TIMER_S_BD);
	KillTimer(W_TIMER_D_BD); 
	KillTimer(W_TIMER_Q_IP);
	KillTimer(W_TIMER_S_IP);
	KillTimer(W_TIMER_D_IP);
	KillTimer(W_TIMER_Q_IP2);
	KillTimer(W_TIMER_S_IP2);
	KillTimer(W_TIMER_D_IP2);
	KillTimer(W_TIMER_Q_PORT);
	KillTimer(W_TIMER_S_PORT); 
	SetWndsEnable(TRUE);
}

// CRtuAttrHydrInterfaceInfo 消息处理程序

void CRtuAttrHydrInterfaceInfo::OnBnClickedBtnQueryGSM()
{
	// TODO: 在此添加控件通知处理程序代码
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{
			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CHydrSendFrame appFrame;
			CString strSensor = "";
			m_Cbx_GSM.GetWindowText(strSensor);
			appFrame.m_LPARME = atoi(strSensor);

			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息
			m_pSubStation->GetSysDataTrans_Hydr()->SCSW_hydrdata_EN_GSM(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_Q_GSM, nElapse, NULL);
		}
	}
}

void CRtuAttrHydrInterfaceInfo::OnBnClickedBtnDelGSM()
{
	// TODO: 在此添加控件通知处理程序代码
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{
			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CHydrSendFrame appFrame;

			CString strSensor = "";
			m_Cbx_GSM.GetWindowText(strSensor);
			appFrame.m_LPARME = atoi(strSensor);

			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息
			m_pSubStation->GetSysDataTrans_Hydr()->SCSW_hydrdata_EN_DEL_GSM(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_D_GSM, nElapse, NULL);
		}
	}
}

void CRtuAttrHydrInterfaceInfo::OnBnClickedBtnSetGSM()
{
	// TODO: 在此添加控件通知处理程序代码
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{
			CHydrSendFrame appFrame;

			m_Let_PSW.GetWindowText(m_str_psw);
			if (m_str_psw.GetLength() == 6)
			{
				appFrame.m_strPsw = m_str_psw;
			}
			else
			{
				MessageBox("密码必须为6位字符","错误",MB_OK|MB_ICONERROR);
				return;
			}

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			
			CString strSensor = "";
			m_Cbx_GSM.GetWindowText(strSensor);
			appFrame.m_LPARME = atoi(strSensor);

			CString strSetVar = "";
			m_Let_GSM.GetWindowText(strSetVar);
			appFrame.m_strSetVar = strSetVar;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息
			m_pSubStation->GetSysDataTrans_Hydr()->SCSW_hydrdata_EN_SET_GSM(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_S_GSM, nElapse, NULL);
		}
	}
}

void CRtuAttrHydrInterfaceInfo::OnBnClickedBtnQueryBD()
{
	// TODO: 在此添加控件通知处理程序代码
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CHydrSendFrame appFrame;
			CString strSensor = "";
			m_Cbx_BD.GetWindowText(strSensor);
			appFrame.m_LPARME = atoi(strSensor);
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息

			m_pSubStation->GetSysDataTrans_Hydr()->SCSW_hydrdata_EN_BD(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_Q_BD, nElapse, NULL);
		}
	}
}

void CRtuAttrHydrInterfaceInfo::OnBnClickedBtnDelBD()
{
	// TODO: 在此添加控件通知处理程序代码
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CHydrSendFrame appFrame;
			CString strSensor = "";
			m_Cbx_BD.GetWindowText(strSensor);
			appFrame.m_LPARME = atoi(strSensor);

			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息
			m_pSubStation->GetSysDataTrans_Hydr()->SCSW_hydrdata_EN_DEL_BD(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_D_BD, nElapse, NULL);
		}
	}
}

void CRtuAttrHydrInterfaceInfo::OnBnClickedBtnSetBD()
{
	// TODO: 在此添加控件通知处理程序代码
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{
			CHydrSendFrame appFrame;
			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			m_Let_PSW.GetWindowText(m_str_psw);
			if (m_str_psw.GetLength() == 6)
			{
				appFrame.m_strPsw = m_str_psw;
			}
			else
			{
				MessageBox("密码必须为6位字符","错误",MB_OK|MB_ICONERROR);
				return;
			}

			CString strSensor = "";
			m_Cbx_BD.GetWindowText(strSensor);
			appFrame.m_LPARME = atoi(strSensor);

			CString strSetVar = "";
			m_Let_BD.GetWindowText(strSetVar);
			appFrame.m_strSetVar = strSetVar;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息
			m_pSubStation->GetSysDataTrans_Hydr()->SCSW_hydrdata_EN_SET_BD(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_S_BD, nElapse, NULL);
		}
	}
}

void CRtuAttrHydrInterfaceInfo::OnBnClickedBtnQueryIP()
{
	// TODO: 在此添加控件通知处理程序代码
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CHydrSendFrame appFrame;
			CString strSensor = "";
			m_Cbx_IP.GetWindowText(strSensor);
			appFrame.m_LPARME = atoi(strSensor);
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息
			m_pSubStation->GetSysDataTrans_Hydr()->SCSW_hydrdata_EN_IP(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_Q_IP, nElapse, NULL);
		}
	}
}

void CRtuAttrHydrInterfaceInfo::OnBnClickedBtnDelIP()
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
			CString strSensor = "";
			m_Cbx_IP.GetWindowText(strSensor);
			appFrame.m_LPARME = atoi(strSensor);
			m_pSubStation->GetSysDataTrans_Hydr()->SCSW_hydrdata_EN_DEL_IP(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_D_IP, nElapse, NULL);
		}
	}
}

void CRtuAttrHydrInterfaceInfo::OnBnClickedBtnSetIP()
{
	// TODO: 在此添加控件通知处理程序代码
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{

			CHydrSendFrame appFrame;
			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			m_Let_PSW.GetWindowText(m_str_psw);
			if (m_str_psw.GetLength() == 6)
			{
				appFrame.m_strPsw = m_str_psw;
			}
			else
			{
				MessageBox("密码必须为6位字符","错误",MB_OK|MB_ICONERROR);
				return;
			}

			CString strSensor = "";
			m_Cbx_IP.GetWindowText(strSensor);
			appFrame.m_LPARME = atoi(strSensor);
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息


			CString strSetVar = "";
			m_Let_IP.GetWindowText(strSetVar);
			appFrame.m_strSetVar = strSetVar;
			m_pSubStation->GetSysDataTrans_Hydr()->SCSW_hydrdata_EN_SET_IP(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_S_IP, nElapse, NULL);
		}
	}
}

void CRtuAttrHydrInterfaceInfo::OnBnClickedBtnQueryPort()
{
	// TODO: 在此添加控件通知处理程序代码
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CHydrSendFrame appFrame;
			CString strSensor = "";
			m_Cbx_Port.GetWindowText(strSensor);
			appFrame.m_LPARME = atoi(strSensor);
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息

			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息
			m_pSubStation->GetSysDataTrans_Hydr()->SCSW_hydrdata_EN_PORT(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_Q_PORT, nElapse, NULL);
		}
	}
}

void CRtuAttrHydrInterfaceInfo::OnBnClickedBtnSetPort()
{
	// TODO: 在此添加控件通知处理程序代码
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{

			CHydrSendFrame appFrame;
			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			m_Let_PSW.GetWindowText(m_str_psw);
			if (m_str_psw.GetLength() == 6)
			{
				appFrame.m_strPsw = m_str_psw;
			}
			else
			{
				MessageBox("密码必须为6位字符","错误",MB_OK|MB_ICONERROR);
				return;
			}

			CString strSensor = "";
			m_Cbx_Port.GetWindowText(strSensor);
			appFrame.m_LPARME = atoi(strSensor);
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息


			CString strSetVar = "";
			m_Let_Port.GetWindowText(strSetVar);
			appFrame.m_strSetVar = strSetVar;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息
			m_pSubStation->GetSysDataTrans_Hydr()->SCSW_hydrdata_EN_SET_PORT(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_S_PORT, nElapse, NULL);
		}
	}
}


void CRtuAttrHydrInterfaceInfo::OnTimer(UINT_PTR nIDEvent)
{
	CString strLog= "";
	_Log_level_type LogLevel = Normal_Log_Message;


	switch (nIDEvent)
	{
	case W_TIMER_Q_GSM:
		{
			if (m_blResult == TRUE)
				strLog = "GSM目标地址查询 成功!";
			else if ( m_blResult == FALSE)
			{
				strLog = "GSM目标地址查询 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_blResult == 0xff)
			{
				strLog = "GSM目标地址查询 错误!";
				LogLevel = Warning_log_Message;
			}
		}
		break;
	case W_TIMER_S_GSM:
		{
			if (m_blResult == TRUE)
				strLog = "GSM目标地址设置 成功!";
			else if ( m_blResult == FALSE)
			{
				strLog = "GSM目标地址设置 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_blResult == 0xff)
			{
				strLog = "GSM目标地址设置 错误!";
				LogLevel = Warning_log_Message;
			}

		}
		break;
	case W_TIMER_D_GSM:
		{
			if (m_blResult == TRUE)
				strLog = "GSM目标地址撤销 成功!";
			else if ( m_blResult == FALSE)
			{
				strLog = "GSM目标地址撤销 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_blResult == 0xff)
			{
				strLog = "GSM目标地址撤销 错误!";
				LogLevel = Warning_log_Message;
			}

		}
		break;
	case W_TIMER_Q_BD:
		{
			if (m_blResult == TRUE)
				strLog = "查询“北斗”卫星目标地址 成功!";
			else if ( m_blResult == FALSE)
			{
				strLog = "查询“北斗”卫星目标地址 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_blResult == 0xff)
			{
				strLog = "查询“北斗”卫星目标地址 错误!";
				LogLevel = Warning_log_Message;
			}

		}
		break;
	case W_TIMER_S_BD:
		{
			if (m_blResult == TRUE)
				strLog = "设置“北斗”卫星目标地址 成功!";
			else if ( m_blResult == FALSE)
			{
				strLog = "设置“北斗”卫星目标地址 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_blResult == 0xff)
			{
				strLog = "设置“北斗”卫星目标地址 错误!";
				LogLevel = Warning_log_Message;
			}

		}
		break;
	case W_TIMER_D_BD:
		{
			if (m_blResult == TRUE)
				strLog = "撤销“北斗”卫星目标地址 成功!";
			else if ( m_blResult == FALSE)
			{
				strLog = "撤销“北斗”卫星目标地址 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_blResult == 0xff)
			{
				strLog = "撤销“北斗”卫星目标地址 错误!";
				LogLevel = Warning_log_Message;
			}

		}
		break;
	case W_TIMER_Q_IP:
		{
			if (m_blResult == TRUE)
				strLog = "查询互联网域名地址 成功!";
			else if ( m_blResult == FALSE)
			{
				strLog = "查询互联网域名地址 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_blResult == 0xff)
			{
				strLog = "查询互联网域名地址 错误!";
				LogLevel = Warning_log_Message;
			}


		}
		break;
	case W_TIMER_S_IP:
		{
			if (m_blResult == TRUE)
				strLog = "设置互联网域名地址 成功!";
			else if ( m_blResult == FALSE)
			{
				strLog = "设置互联网域名地址 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_blResult == 0xff)
			{
				strLog = "设置互联网域名地址 错误!";
				LogLevel = Warning_log_Message;
			}
		}
		break; 
	case W_TIMER_D_IP:
		{
			if (m_blResult == TRUE)
				strLog = "撤销互联网域名地址 成功!";
			else if ( m_blResult == FALSE)
			{
				strLog = "撤销互联网域名地址 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_blResult == 0xff)
			{
				strLog = "撤销互联网域名地址 错误!";
				LogLevel = Warning_log_Message;
			}

		}
		break;
	case W_TIMER_Q_IP2:
		{
			if (m_blResult == TRUE)
				strLog = "查询互联网IP地址 成功!";
			else if ( m_blResult == FALSE)
			{
				strLog = "查询互联网IP地址 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_blResult == 0xff)
			{
				strLog = "查询互联网IP地址 错误!";
				LogLevel = Warning_log_Message;
			}


		}
		break;
	case W_TIMER_S_IP2:
		{
			if (m_blResult == TRUE)
				strLog = "设置互联网IP地址 成功!";
			else if ( m_blResult == FALSE)
			{
				strLog = "设置互联网IP地址 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_blResult == 0xff)
			{
				strLog = "设置互联网IP地址 错误!";
				LogLevel = Warning_log_Message;
			}
		}
		break; 
	case W_TIMER_D_IP2:
		{
			if (m_blResult == TRUE)
				strLog = "撤销互联网IP地址 成功!";
			else if ( m_blResult == FALSE)
			{
				strLog = "撤销互联网IP地址 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_blResult == 0xff)
			{
				strLog = "撤销互联网IP地址 错误!";
				LogLevel = Warning_log_Message;
			}

		}
		break;
	case W_TIMER_Q_PORT:
		{
			if (m_blResult == TRUE)
				strLog = "查询互联网端口号 成功!";
			else if ( m_blResult == FALSE)
			{
				strLog = "查询互联网端口号 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_blResult == 0xff)
			{
				strLog = "查询互联网端口号 错误!";
				LogLevel = Warning_log_Message;
			}

		}
		break;
	case W_TIMER_S_PORT:
		{
			if (m_blResult == TRUE)
				strLog = "设置互联网端口号 成功!";
			else if ( m_blResult == FALSE)
			{
				strLog = "设置互联网端口号 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_blResult == 0xff)
			{
				strLog = "设置互联网端口号 错误!";
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


void CRtuAttrHydrInterfaceInfo::OnBnClickedBtnQueryIp2()
{
	// TODO: 在此添加控件通知处理程序代码
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CHydrSendFrame appFrame;
			CString strSensor = "";
			m_Cbx_IP2.GetWindowText(strSensor);
			appFrame.m_LPARME = atoi(strSensor);
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息
			m_pSubStation->GetSysDataTrans_Hydr()->SCSW_hydrdata_EN_IP2(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_Q_IP, nElapse, NULL);
		}
	}
}

void CRtuAttrHydrInterfaceInfo::OnBnClickedBtnDelIp2()
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
			CString strSensor = "";
			m_Cbx_IP2.GetWindowText(strSensor);
			appFrame.m_LPARME = atoi(strSensor);
			m_pSubStation->GetSysDataTrans_Hydr()->SCSW_hydrdata_EN_DEL_IP2(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_D_IP, nElapse, NULL);
		}
	}
}

void CRtuAttrHydrInterfaceInfo::OnBnClickedBtnSetIp2()
{
	// TODO: 在此添加控件通知处理程序代码
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{

			CHydrSendFrame appFrame;

			SetWndsEnable(FALSE);
			m_blResult = FALSE;
	
			m_Let_PSW.GetWindowText(m_str_psw);
			if (m_str_psw.GetLength() == 6)
			{
				appFrame.m_strPsw = m_str_psw;
			}
			else
			{
				MessageBox("密码必须为6位字符","错误",MB_OK|MB_ICONERROR);
				return;
			}

			CString strSensor = "";
			m_Cbx_IP2.GetWindowText(strSensor);
			appFrame.m_LPARME = atoi(strSensor);
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息


			CString strSetVar = "";
			CString strPsw = "";
			m_Let_IP2.GetWindowText(strSetVar);
			m_Let_PSW.GetWindowText(strPsw);
			appFrame.m_strSetVar = strSetVar;
			appFrame.m_strPsw = strPsw;
			m_pSubStation->GetSysDataTrans_Hydr()->SCSW_hydrdata_EN_SET_IP2(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_S_IP, nElapse, NULL);
		}
	}
}
