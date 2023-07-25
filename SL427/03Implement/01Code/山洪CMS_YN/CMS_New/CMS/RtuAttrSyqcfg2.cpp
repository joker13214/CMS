// RtuAttrSyqcfg2.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuAttrSyqcfg2.h"
#include "UdfGlobalFUNC.h"


#define W_TIMER_R   0x01
#define W_TIMER_W   0x02
// CRtuAttrSyqcfg2 对话框

IMPLEMENT_DYNAMIC(CRtuAttrSyqcfg2, CDialog)

CRtuAttrSyqcfg2::CRtuAttrSyqcfg2(CWnd* pParent /*=NULL*/)
	: CDialog(CRtuAttrSyqcfg2::IDD, pParent)
{

}

CRtuAttrSyqcfg2::~CRtuAttrSyqcfg2()
{
}

void CRtuAttrSyqcfg2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_DINGSHI_MESSAGE, m_cfg_20);
	DDX_Control(pDX, IDC_EDIT_DINGSHI_MESSAGE2, m_cfg_21);
	DDX_Control(pDX, IDC_COMBO_RAIN_FENBIANLI, m_cfg_22);
	DDX_Control(pDX, IDC_COMBO_WATERLEVEL_FENBIANLI, m_cfg_23);
	DDX_Control(pDX, IDC_EDIT_RIAN_JIABAOFAZHI, m_cfg_24);
	DDX_Control(pDX, IDC_EDIT_WATER_JIABAOLEVEL1, m_cfg_25);
	DDX_Control(pDX, IDC_EDIT_WATER_JIABAOLEVEL2, m_cfg_26);
	DDX_Control(pDX, IDC_EDIT_WATER_JIABAOLEVEL3, m_cfg_27);
	DDX_Control(pDX, IDC_EDIT_START_HOUR, m_cfg_28);
	DDX_Control(pDX, IDC_EDIT_INTERVAL, m_cfg_29);
	DDX_Control(pDX, IDC_COMBO_WORKWAY, m_cfg_2A);
	DDX_Control(pDX, IDC_EDIT_YC_WATERLEVEL_JIZHUN1, m_cfg_2B);
	DDX_Control(pDX, IDC_EDIT_YC_WATERLEVEL_JIZHUN2, m_cfg_2C);
	DDX_Control(pDX, IDC_EDIT_WATERLEVEL_DATA_INTERVAL, m_cfg_2D);
	DDX_Control(pDX, IDC_EDIT_LIULIANG_JIABAO_FAZHI, m_cfg_2E);
	DDX_Control(pDX, IDC_EDIT_LIUSU_JIABAO_FAZHI, m_cfg_2F);
	DDX_Control(pDX, IDC_BTN_READ, m_ReadBtn);
	DDX_Control(pDX, IDC_BTN_WRITE, m_WriteBtn);
}


BEGIN_MESSAGE_MAP(CRtuAttrSyqcfg2, CDialog)
	ON_BN_CLICKED(IDC_BTN_READ, &CRtuAttrSyqcfg2::OnBnClickedBtnRead)
	ON_BN_CLICKED(IDC_BTN_WRITE, &CRtuAttrSyqcfg2::OnBnClickedBtnWrite)
	ON_WM_TIMER()
END_MESSAGE_MAP()



BOOL CRtuAttrSyqcfg2::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (m_cfg_22.GetSafeHwnd())
	{
		m_cfg_22.ResetContent();
		int appType_idx = 0;
		appType_idx = m_cfg_22.AddString("1mm");
		m_cfg_22.SetItemData(appType_idx, (BYTE)1);

		appType_idx = m_cfg_22.AddString("0.5mm ");
		m_cfg_22.SetItemData(appType_idx, (BYTE)2);

		appType_idx = m_cfg_22.AddString("0.2mm");
		m_cfg_22.SetItemData(appType_idx, (BYTE)3);

		appType_idx = m_cfg_22.AddString("0.1mm");
		m_cfg_22.SetItemData(appType_idx, (BYTE)4);

	}
	if (m_cfg_23.GetSafeHwnd())
	{
		m_cfg_23.ResetContent();
		int appType_idx = 0;
		appType_idx = m_cfg_23.AddString("1mm");
		m_cfg_23.SetItemData(appType_idx, (BYTE)1);

		appType_idx = m_cfg_23.AddString("0.5mm ");
		m_cfg_23.SetItemData(appType_idx, (BYTE)2);

		appType_idx = m_cfg_23.AddString("0.1mm");
		m_cfg_23.SetItemData(appType_idx, (BYTE)3);
	}
	if (m_cfg_2A.GetSafeHwnd())
	{
		m_cfg_2A.ResetContent();
		int appType_idx = 0;
		appType_idx = m_cfg_2A.AddString("自报工作状态");
		m_cfg_2A.SetItemData(appType_idx, (BYTE)1);

		appType_idx = m_cfg_2A.AddString("自报确认工作状态 ");
		m_cfg_2A.SetItemData(appType_idx, (BYTE)2);

		appType_idx = m_cfg_2A.AddString("查询/应答工作状态");
		m_cfg_2A.SetItemData(appType_idx, (BYTE)3);

		appType_idx = m_cfg_2A.AddString("调试或者维修状态");
		m_cfg_2A.SetItemData(appType_idx, (BYTE)4);

	}

	m_cfg_22.SetCurSel(0);
	m_cfg_23.SetCurSel(0);
	m_cfg_2A.SetCurSel(0);


	return TRUE;
}


// CRtuAttrSyqcfg2 消息处理程序
void CRtuAttrSyqcfg2::SetParentDlg(CRtuPropertyDlg* pDlg)
{
	m_pParentDlg = pDlg;	
}
void CRtuAttrSyqcfg2::Update()
{
	CString temp ="";
	if(m_pParentDlg)
		m_pSubStation = m_pParentDlg->GetRtuStation();		
	if(m_pSubStation)
	{
		temp.Format("%d",m_pSubStation->m_syqconfig2data.CFG_20);
		m_cfg_20.SetWindowText(temp);
		temp.Format("%d",m_pSubStation->m_syqconfig2data.CFG_21);
		m_cfg_21.SetWindowText(temp);

		m_cfg_22.SetCurSel(m_pSubStation->m_syqconfig2data.CFG_22-1);
		m_cfg_23.SetCurSel(m_pSubStation->m_syqconfig2data.CFG_23-1);
		m_cfg_2A.SetCurSel(m_pSubStation->m_syqconfig2data.CFG_2A-1);

		temp.Format("%d",m_pSubStation->m_syqconfig2data.CFG_24);
		m_cfg_24.SetWindowText(temp);

		temp.Format("%d",m_pSubStation->m_syqconfig2data.CFG_25);
		m_cfg_25.SetWindowText(temp);

		temp.Format("%d",m_pSubStation->m_syqconfig2data.CFG_26);
		m_cfg_26.SetWindowText(temp);

		temp.Format("%d",m_pSubStation->m_syqconfig2data.CFG_27);
		m_cfg_27.SetWindowText(temp);

		temp.Format("%d",m_pSubStation->m_syqconfig2data.CFG_28);
		m_cfg_28.SetWindowText(temp);

		temp.Format("%d",m_pSubStation->m_syqconfig2data.CFG_29);
		m_cfg_29.SetWindowText(temp);

		temp.Format("%d",m_pSubStation->m_syqconfig2data.CFG_2B);
		m_cfg_2B.SetWindowText(temp);

		temp.Format("%d",m_pSubStation->m_syqconfig2data.CFG_2C);
		m_cfg_2C.SetWindowText(temp);

		temp.Format("%d",m_pSubStation->m_syqconfig2data.CFG_2D);
		m_cfg_2D.SetWindowText(temp);

		temp.Format("%u",m_pSubStation->m_syqconfig2data.CFG_2E);
		m_cfg_2E.SetWindowText(temp);

		temp.Format("%u",m_pSubStation->m_syqconfig2data.CFG_2F);
		m_cfg_2F.SetWindowText(temp);
	}

}
void CRtuAttrSyqcfg2::OnBnClickedBtnRead()
{
	// TODO: 在此添加控件通知处理程序代码
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans())
		{
			m_bResult = FALSE;
			SetEnable(FALSE);
			_SYQ_CONFIG_PACKET2  syqCfgPack;

			CString tempstr = "";
			int  temp;
			m_cfg_20.GetWindowText(tempstr);
			temp = _ttoi(tempstr);
				syqCfgPack.CFG_20 = temp;

				m_cfg_21.GetWindowText(tempstr);
				temp = _ttoi(tempstr);
				syqCfgPack.CFG_21 = temp;

				syqCfgPack.CFG_22 = m_cfg_22.GetCurSel()+1;
				syqCfgPack.CFG_23 = m_cfg_23.GetCurSel()+1;
				syqCfgPack.CFG_2A = m_cfg_2A.GetCurSel()+1;


				m_cfg_24.GetWindowText(tempstr);
				temp = _ttoi(tempstr);
				syqCfgPack.CFG_24 = temp;

				m_cfg_25.GetWindowText(tempstr);
				temp = _ttoi(tempstr);
				syqCfgPack.CFG_25 = temp;

				m_cfg_26.GetWindowText(tempstr);
				temp = _ttoi(tempstr);
				syqCfgPack.CFG_26 = temp;

				m_cfg_27.GetWindowText(tempstr);
				temp = _ttoi(tempstr);
				syqCfgPack.CFG_27 = temp;

				m_cfg_28.GetWindowText(tempstr);
				temp = _ttoi(tempstr);
				syqCfgPack.CFG_28 = temp;


				m_cfg_29.GetWindowText(tempstr);
				temp = _ttoi(tempstr);
				syqCfgPack.CFG_29 = temp;


				m_cfg_2B.GetWindowText(tempstr);
				temp = _ttoi(tempstr);
				syqCfgPack.CFG_2B = temp;

				m_cfg_2C.GetWindowText(tempstr);
				temp = _ttoi(tempstr);
				syqCfgPack.CFG_2C = temp;

				m_cfg_2D.GetWindowText(tempstr);
				temp = _ttoi(tempstr);
				syqCfgPack.CFG_2D = temp;

				m_cfg_2E.GetWindowText(tempstr);
				DWORD temp1 = _ttol(tempstr);
				syqCfgPack.CFG_2E = temp1;

				m_cfg_2F.GetWindowText(tempstr);
				temp = _ttoi(tempstr);
				syqCfgPack.CFG_2F = temp;

				//	short comtype;
				//comtype = m_commtype1.GetCurSel()+1;

				CSYQFrameFormat appFrame;
				m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息
				//添加数据打包
				m_pSubStation->GetSysDataTrans_Syq()->SYQ_EN_43H(&appFrame,syqCfgPack);

				UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
				SetTimer(W_TIMER_R, nElapse, NULL);
			}
		}


}

void CRtuAttrSyqcfg2::OnBnClickedBtnWrite()
{
	// TODO: 在此添加控件通知处理程序代码

	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans())
		{
			m_bResult = FALSE;
			SetEnable(FALSE);
			_SYQ_CONFIG_PACKET2  syqCfgPack;
			
			
			CString tempstr = "";
			int  temp;
			m_cfg_20.GetWindowText(tempstr);
			temp = _ttoi(tempstr);
			syqCfgPack.CFG_20 = temp;

			m_cfg_21.GetWindowText(tempstr);
			temp = _ttoi(tempstr);
			syqCfgPack.CFG_21 = temp;

			syqCfgPack.CFG_22 = m_cfg_22.GetCurSel()+1;
			syqCfgPack.CFG_23 = m_cfg_23.GetCurSel()+1;
			syqCfgPack.CFG_2A = m_cfg_2A.GetCurSel()+1;


			m_cfg_24.GetWindowText(tempstr);
			temp = _ttoi(tempstr);
			syqCfgPack.CFG_24 = temp;

			m_cfg_25.GetWindowText(tempstr);
			temp = _ttoi(tempstr);
			syqCfgPack.CFG_25 = temp;

			m_cfg_26.GetWindowText(tempstr);
			temp = _ttoi(tempstr);
			syqCfgPack.CFG_26 = temp;

			m_cfg_27.GetWindowText(tempstr);
			temp = _ttoi(tempstr);
			syqCfgPack.CFG_27 = temp;

			m_cfg_28.GetWindowText(tempstr);
			temp = _ttoi(tempstr);
			syqCfgPack.CFG_28 = temp;


			m_cfg_29.GetWindowText(tempstr);
			temp = _ttoi(tempstr);
			syqCfgPack.CFG_29 = temp;


			m_cfg_2B.GetWindowText(tempstr);
			temp = _ttoi(tempstr);
			syqCfgPack.CFG_2B = temp;

			m_cfg_2C.GetWindowText(tempstr);
			temp = _ttoi(tempstr);
			syqCfgPack.CFG_2C = temp;

			m_cfg_2D.GetWindowText(tempstr);
			temp = _ttoi(tempstr);
			syqCfgPack.CFG_2D = temp;

			m_cfg_2E.GetWindowText(tempstr);
			DWORD temp1 = _ttol(tempstr);
			syqCfgPack.CFG_2E = temp1;

			m_cfg_2F.GetWindowText(tempstr);
			temp = _ttoi(tempstr);
			syqCfgPack.CFG_2F = temp;

			CSYQFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息
			//添加数据打包
			m_pSubStation->GetSysDataTrans_Syq()->SYQ_EN_42H(&appFrame,syqCfgPack);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_W, nElapse, NULL);
		}
	}

}


void CRtuAttrSyqcfg2::UpdateDlgMessage(int attr, bool result)
{
	m_bResult = result;

	if(attr == syq_config2_Write)
	{
		SendMessage(WM_TIMER,W_TIMER_W, NULL);
	}
	else if(attr == syq_config2_Read)
	{
		SendMessage(WM_TIMER,W_TIMER_R, NULL);
	}


}

void CRtuAttrSyqcfg2::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString strLog= "";
	_Log_level_type LogLevel = Normal_Log_Message;

	switch (nIDEvent)
	{
	case W_TIMER_R:
		{
			if (m_bResult == TRUE)
				strLog = "查询遥测站参数配置表2 成功!";
			else if ( m_bResult == FALSE)
			{
				strLog = "查询遥测站参数配置表2 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_bResult == 0xff)
			{
				strLog = "查询遥测站参数配置表2 错误!";
				LogLevel = Warning_log_Message;
			}
		}
		break;
	case W_TIMER_W:
		{
			if (m_bResult == TRUE)
				strLog = "修改遥测站参数配置表2 成功!";
			else if ( m_bResult == FALSE)
			{
				strLog = "修改遥测站参数配置表2 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_bResult == 0xff)
			{
				strLog = "修改查询遥测站参数配置表2 错误!";
				LogLevel = Warning_log_Message;
			}
		}
		break;
	
	}

	KillTimer(nIDEvent);
	SetEnable(TRUE);
	if (m_pParentDlg && m_pParentDlg->GetSafeHwnd())
		m_pParentDlg->AddLog(strLog,(PSYSTEMTIME)NULL,LogLevel);

	if (m_pSubStation)
		m_pSubStation->InsertLogMessage(strLog,LogLevel,NULL);

	CDialog::OnTimer(nIDEvent);
}





void CRtuAttrSyqcfg2::SetEnable(bool b)
{
	m_cfg_20.EnableWindow(b);
	m_cfg_21.EnableWindow(b);
	m_cfg_22.EnableWindow(b);
	m_cfg_23.EnableWindow(b);
	m_cfg_24.EnableWindow(b);
	m_cfg_25.EnableWindow(b);
	m_cfg_26.EnableWindow(b);
	m_cfg_27.EnableWindow(b);
	m_cfg_28.EnableWindow(b);
	m_cfg_29.EnableWindow(b);
	m_cfg_2A.EnableWindow(b);
	m_cfg_2B.EnableWindow(b);
	m_cfg_2C.EnableWindow(b);
	m_cfg_2D.EnableWindow(b);
	m_cfg_2E.EnableWindow(b);
	m_cfg_2F.EnableWindow(b);

	m_ReadBtn.EnableWindow(b);
	m_WriteBtn.EnableWindow(b);

}