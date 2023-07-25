// RtuAttrControlCmd.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuAttrControlCmd.h"

#define W_TIMER_PUMPGATE_START     0x01   //启动水泵或闸门/阀门 
#define W_TIMER_PUMPGATE_CLOSE     0x02   //关闭水泵或闸门/阀门 
#define W_TIMER_ICCARD_ENABLE      0x03   //置IC卡功能有效
#define W_TIMER_ICCARD_DISENABLE   0x04   //置IC卡功能无效
#define W_TIMER_ZD_INPUT		   0x05   //定值功能投入
#define W_TIMER_ZD_EXIT			   0x06   //定值功能退出
#define W_TIMER_CLEAR_HISDATA      0x07	  //清除遥测终端历史单元数据
#define W_TIMER_RESET_RTU          0x08   //复位遥测终端参数和状态


// CRtuAttrControlCmd 对话框
IMPLEMENT_DYNAMIC(CRtuAttrControlCmd, CDialog)

CRtuAttrControlCmd::CRtuAttrControlCmd(CWnd* pParent /*=NULL*/)
	: CDialog(CRtuAttrControlCmd::IDD, pParent)
{
	m_blResult = FALSE;
	m_pSubStation = NULL;
	m_pParentDlg = NULL;
}

CRtuAttrControlCmd::~CRtuAttrControlCmd()
{
}

void CRtuAttrControlCmd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_PGTYPE, m_wndPGTypeCbm);
	DDX_Control(pDX, IDC_BTN_PGSTART, m_btmPGStart);
	DDX_Control(pDX, IDC_BTN_PGCLOSE, m_btnPGClose);
	DDX_Control(pDX, IDC_BTN_ICENABLE, m_btnICEnable);
	DDX_Control(pDX, IDC_BTN_ICDISENABLE, m_btnICDisEnable);
	DDX_Control(pDX, IDC_BTN_ZDINPUT, m_btnZdInput);
	DDX_Control(pDX, IDC_BTN_ZDEXIT, m_btnZdExit);
	DDX_Control(pDX, IDC_CMB_RESETCMD, m_wndResetCmdCmb);
	DDX_Control(pDX, IDC_BTN_RESET, m_btnReset);
	DDX_Control(pDX, IDC_BTN_CLEARHIS, m_btnHisDataClear);
	DDX_Control(pDX, IDC_EDIT_PGCODE, m_EditPgCode);
	DDX_Control(pDX, IDC_CHECK_CLEAR0,m_CheckClear0);
	DDX_Control(pDX, IDC_CHECK_CLEAR1,m_CheckClear1);
	DDX_Control(pDX, IDC_CHECK_CLEAR2,m_CheckClear2);
	DDX_Control(pDX, IDC_CHECK_CLEAR3,m_CheckClear3);
}


BEGIN_MESSAGE_MAP(CRtuAttrControlCmd, CDialog)
	ON_CBN_SELCHANGE(IDC_CMB_PGTYPE, &CRtuAttrControlCmd::OnCbnSelchangeCmbPgtype)
	ON_CBN_SELCHANGE(IDC_CMB_RESETCMD, &CRtuAttrControlCmd::OnCbnSelchangeCmbResetcmd)
	ON_BN_CLICKED(IDC_BTN_PGSTART, &CRtuAttrControlCmd::OnBnClickedBtnPgstart)
	ON_BN_CLICKED(IDC_BTN_PGCLOSE, &CRtuAttrControlCmd::OnBnClickedBtnPgclose)
	ON_BN_CLICKED(IDC_BTN_ICENABLE, &CRtuAttrControlCmd::OnBnClickedBtnIcenable)
	ON_BN_CLICKED(IDC_BTN_ICDISENABLE, &CRtuAttrControlCmd::OnBnClickedBtnIcdisenable)
	ON_BN_CLICKED(IDC_BTN_ZDINPUT,  &CRtuAttrControlCmd::OnBnClickedBtnZdinput)
	ON_BN_CLICKED(IDC_BTN_ZDEXIT,   &CRtuAttrControlCmd::OnBnClickedBtnZdexit)
	ON_BN_CLICKED(IDC_BTN_RESET,    &CRtuAttrControlCmd::OnBnClickedBtnReset)
	ON_BN_CLICKED(IDC_BTN_CLEARHIS, &CRtuAttrControlCmd::OnBnClickedBtnClearhis)
	ON_BN_CLICKED(IDC_CHECK_CLEAR0, &CRtuAttrControlCmd::OnBnClickedCheckClear0)
	ON_BN_CLICKED(IDC_CHECK_CLEAR1, &CRtuAttrControlCmd::OnBnClickedCheckClear1)
	ON_BN_CLICKED(IDC_CHECK_CLEAR2, &CRtuAttrControlCmd::OnBnClickedCheckClear2)
	ON_BN_CLICKED(IDC_CHECK_CLEAR3, &CRtuAttrControlCmd::OnBnClickedCheckClear3)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CRtuAttrControlCmd 消息处理程序


void CRtuAttrControlCmd::SetParentDlg(CRtuPropertyDlg* pDlg)
{
	m_pParentDlg = pDlg;

}

void CRtuAttrControlCmd::Update()
{
	if (m_pParentDlg != NULL)
		m_pSubStation = m_pParentDlg->GetRtuStation();

	if (m_pSubStation)
	{
		int rPos = 0;
		if (m_wndResetCmdCmb.GetSafeHwnd() && m_wndResetCmdCmb.GetCount())
		{
			for (int n= 0 ;n < m_wndResetCmdCmb.GetCount(); n ++)
			{
				BYTE ItemData = (BYTE)m_wndResetCmdCmb.GetItemData(n);
				if (ItemData == m_pSubStation->Get_RESET_Cmd())
				{
					rPos = n;
					break;
				}
			}
			m_wndResetCmdCmb.SetCurSel(rPos);
			OnCbnSelchangeCmbResetcmd();
		}
		if (m_wndPGTypeCbm.GetSafeHwnd() && m_wndPGTypeCbm.GetCount())
		{
			rPos = 0;
			for (int n =0 ;n <m_wndPGTypeCbm.GetCount() ;n ++)
			{
				BYTE ItemData = (BYTE)m_wndPGTypeCbm.GetItemData(n);
				if (ItemData == m_pSubStation->GetCurPumpGateObjecte()->GetPumpGate_Type())
				{
					rPos  = n;
					break;
				}
			}

			m_wndPGTypeCbm.SetCurSel(rPos);
			OnCbnSelchangeCmbPgtype();
		}

		CString strTemp = "";
		strTemp.Format("%d",m_pSubStation->GetCurPumpGateObjecte()->GetPumpGate_Code());
		m_EditPgCode.SetWindowText(strTemp);


		m_CheckClear0.SetCheck((m_pSubStation->GetClearHistoryDataPara()->GetBitValue(0)==true?TRUE:FALSE));
		m_CheckClear1.SetCheck((m_pSubStation->GetClearHistoryDataPara()->GetBitValue(1)==true?TRUE:FALSE));
		m_CheckClear2.SetCheck((m_pSubStation->GetClearHistoryDataPara()->GetBitValue(2)==true?TRUE:FALSE));
		m_CheckClear3.SetCheck((m_pSubStation->GetClearHistoryDataPara()->GetBitValue(3)==true?TRUE:FALSE));
	}
}

void CRtuAttrControlCmd::UpdateDlg()
{
	if (m_pSubStation != NULL)
	{
		BYTE ykResult = m_pSubStation->Get_YK_Op_Result();
		if (ykResult == YKStart_PumpGate_ERR ||ykResult == YKStart_PumpGate_SUC)
		{
			if (ykResult == YKStart_PumpGate_SUC)
				m_blResult = TRUE;
			else if (YKStart_PumpGate_ERR == ykResult) 
				m_blResult = FALSE;
			SendMessage(WM_TIMER,W_TIMER_PUMPGATE_START,NULL);
		}
		else if (ykResult == YKClose_PumpGate_ERR || ykResult == YKClose_PumpGate_SUC)
		{
			if (ykResult == YKClose_PumpGate_ERR)
				m_blResult = FALSE;
			else if (YKClose_PumpGate_SUC == ykResult)
				m_blResult = TRUE;
			SendMessage(WM_TIMER,W_TIMER_PUMPGATE_CLOSE,NULL);

		}
		else if(ykResult == YKICCard_Enable_ERR || ykResult == YKICCard_Enable_SUC)
		{
			if (ykResult == YKICCard_Enable_SUC)
				m_blResult = TRUE;
			else if (YKICCard_Enable_ERR == ykResult)
				m_blResult = FALSE;
			SendMessage(WM_TIMER,W_TIMER_ICCARD_ENABLE,NULL);
		}
		else if (ykResult == YKICCard_DisEnable_ERR || ykResult == YKICCard_DisEnable_SUC)
		{
			if (ykResult == YKICCard_DisEnable_SUC)
				m_blResult = TRUE;
			else if (YKICCard_DisEnable_ERR == ykResult)
				m_blResult = FALSE;

			SendMessage(WM_TIMER,W_TIMER_ICCARD_DISENABLE,NULL);
		}
		else if (ykResult == YKZD_Input_ERR || ykResult == YKZD_Input_SUC)
		{
			if (ykResult == YKZD_Input_SUC)
				m_blResult = TRUE;
			else if (YKZD_Input_ERR == ykResult)
				m_blResult = FALSE;

			SendMessage(WM_TIMER,W_TIMER_ZD_INPUT,NULL);
		}
		else if (ykResult == YKZD_Exit_ERR || ykResult == YKZD_Exit_SUC)
		{
			if (ykResult == YKZD_Exit_SUC)
				m_blResult = TRUE;
			else if (YKZD_Exit_ERR == ykResult)
				m_blResult = FALSE;
			SendMessage(WM_TIMER,W_TIMER_ZD_EXIT,NULL);
		}
		else if (ykResult == YKReset_Rtu_ERR || ykResult == YKReset_Rtu_SUC)
		{
			if (ykResult == YKReset_Rtu_SUC)
				m_blResult = TRUE;
			else if (YKReset_Rtu_ERR == ykResult)
				m_blResult = FALSE;

			SendMessage(WM_TIMER,W_TIMER_RESET_RTU,NULL);
		}
		else if (ykResult == YKClear_his_ERR || ykResult == YKClear_His_SUC)
		{
			if (ykResult == YKClear_His_SUC)
				m_blResult = TRUE;
			else if (YKClear_his_ERR == ykResult)
				m_blResult = FALSE;

			SendMessage(WM_TIMER,W_TIMER_CLEAR_HISDATA,NULL);
		}

	}
}


BOOL CRtuAttrControlCmd::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if (m_wndResetCmdCmb.GetSafeHwnd())
	{
		m_wndResetCmdCmb.ResetContent();
		
		int Index = m_wndResetCmdCmb.AddString("重新启动,参数不变");
		m_wndResetCmdCmb.SetItemData(Index,1);

		Index = m_wndResetCmdCmb.AddString("重新启动,参数恢复到出厂默认值");
		m_wndResetCmdCmb.SetItemData(Index,2);
		if (m_wndResetCmdCmb.GetCount())
			m_wndResetCmdCmb.SetCurSel(0);
		OnCbnSelchangeCmbResetcmd();

	}

	if (m_wndPGTypeCbm.GetSafeHwnd())
	{
		m_wndPGTypeCbm.ResetContent();

		int Index = m_wndPGTypeCbm.AddString("水泵");
		m_wndPGTypeCbm.SetItemData(Index,0);

		Index = m_wndPGTypeCbm.AddString("阀门/闸门");
		m_wndPGTypeCbm.SetItemData(Index,15);

		if (m_wndPGTypeCbm.GetCount())
			m_wndPGTypeCbm.SetCurSel(0);

		OnCbnSelchangeCmbPgtype();
	}

	return TRUE;  
	// return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CRtuAttrControlCmd::OnCbnSelchangeCmbPgtype()
{
	int nSel = m_wndPGTypeCbm.GetCurSel();
	if (nSel != CB_ERR)
	{
		BYTE ItemData = (BYTE)m_wndPGTypeCbm.GetItemData(nSel);
		if (m_pSubStation != NULL)
		{
			m_pSubStation->GetCurPumpGateObjecte()->SetPumpGate_Type(ItemData);
		}
	}
}

void CRtuAttrControlCmd::OnCbnSelchangeCmbResetcmd()
{
	int nSel = m_wndResetCmdCmb.GetCurSel();
	if (nSel != CB_ERR)
	{
		BYTE ItemData = (BYTE)m_wndResetCmdCmb.GetItemData(nSel);
		if(m_pSubStation)
		{
			m_pSubStation->Set_RESET_Cmd(ItemData);
		}
	}
}

void CRtuAttrControlCmd::SetWndsEnable(BOOL blEnable /* = TRUE */)
{
	 m_wndPGTypeCbm.EnableWindow(blEnable);
	 m_btmPGStart.EnableWindow(blEnable);
	 m_btnPGClose.EnableWindow(blEnable);
	 m_btnICEnable.EnableWindow(blEnable);
	 m_btnICDisEnable.EnableWindow(blEnable);
	 m_btnZdInput.EnableWindow(blEnable);
	 m_btnZdExit.EnableWindow(blEnable);
	 m_wndResetCmdCmb.EnableWindow(blEnable);
	 m_btnReset.EnableWindow(blEnable);
	 m_btnHisDataClear.EnableWindow(blEnable);
	 m_EditPgCode.EnableWindow(blEnable);
	 m_CheckClear0.EnableWindow(blEnable);
	 m_CheckClear1.EnableWindow(blEnable);
	 m_CheckClear2.EnableWindow(blEnable);
	 m_CheckClear3.EnableWindow(blEnable);
}

void CRtuAttrControlCmd::OnBnClickedBtnPgstart()
{
	UpdateData(TRUE);


	if( m_pSubStation)
	{
		m_pSubStation->Set_YK_Op_Result(0);

		if (m_pSubStation->blEnbaleTrans() == true)
		{

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CString strTemp = "";
			m_EditPgCode.GetWindowText(strTemp);
			BYTE PgCode = (BYTE)atoi(strTemp);
			m_pSubStation->GetCurPumpGateObjecte()->SetPumpGate_Code(PgCode);

			CPacketFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息

			m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_92H(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_PUMPGATE_START,nElapse,NULL);

			CString strLog="";
			strLog.Format("%s 启动水泵或闸门/阀门(code:%d)操作!",
				m_pSubStation->Get_Log_RtuInfo(),PgCode);
			m_pSubStation->AddYkOperateLog(strLog);
			
		}
	}
}

void CRtuAttrControlCmd::OnBnClickedBtnPgclose()
{
	UpdateData(TRUE);


	if( m_pSubStation)
	{
		m_pSubStation->Set_YK_Op_Result(0);

		if (m_pSubStation->blEnbaleTrans() == true)
		{

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CString strTemp = "";
			m_EditPgCode.GetWindowText(strTemp);
			BYTE PgCode = (BYTE)atoi(strTemp);
			m_pSubStation->GetCurPumpGateObjecte()->SetPumpGate_Code(PgCode);

			CPacketFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息

			m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_93H(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_PUMPGATE_CLOSE,nElapse,NULL);


			CString strLog="";
			strLog.Format("%s 关闭水泵或闸门/阀门(code:%d)操作!",
				m_pSubStation->Get_Log_RtuInfo(),PgCode);
			m_pSubStation->AddYkOperateLog(strLog);
		}
	}
}

void CRtuAttrControlCmd::OnBnClickedBtnIcenable()
{
	if( m_pSubStation)
	{
		m_pSubStation->Set_YK_Op_Result(0);

		if (m_pSubStation->blEnbaleTrans() == true)
		{
			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CPacketFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息

			m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_30H(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_ICCARD_ENABLE,nElapse,NULL);

			CString strLog="";
			strLog.Format("%s 置遥测终端IC卡功能有效操作!",
				m_pSubStation->Get_Log_RtuInfo());
			m_pSubStation->AddYkOperateLog(strLog);
		}
	}
}

void CRtuAttrControlCmd::OnBnClickedBtnIcdisenable()
{
	if( m_pSubStation)
	{
		m_pSubStation->Set_YK_Op_Result(0);

		if (m_pSubStation->blEnbaleTrans() == true)
		{

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CPacketFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息

			m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_31H(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_ICCARD_DISENABLE,nElapse,NULL);

			CString strLog="";
			strLog.Format("%s 取消遥测终端IC卡功能操作!",
				m_pSubStation->Get_Log_RtuInfo());
			m_pSubStation->AddYkOperateLog(strLog);
		}
	}
}

void CRtuAttrControlCmd::OnBnClickedBtnZdinput()
{
	if( m_pSubStation)
	{
		m_pSubStation->Set_YK_Op_Result(0);

		if (m_pSubStation->blEnbaleTrans() == true)
		{

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CPacketFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息

			m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_32H(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_ZD_INPUT,nElapse,NULL);

			CString strLog="";
			strLog.Format("%s 遥测终端定值功能投入操作!",
				m_pSubStation->Get_Log_RtuInfo());
			m_pSubStation->AddYkOperateLog(strLog);
		}
	}
}

void CRtuAttrControlCmd::OnBnClickedBtnZdexit()
{
	if( m_pSubStation)
	{
		m_pSubStation->Set_YK_Op_Result(0);

		if (m_pSubStation->blEnbaleTrans() == true)
		{

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CPacketFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息

			m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_33H(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_ZD_EXIT,nElapse,NULL);

			CString strLog="";
			strLog.Format("%s 遥测终端定值功能退出操作!",
				m_pSubStation->Get_Log_RtuInfo());
			m_pSubStation->AddYkOperateLog(strLog);
		}
	}
}

void CRtuAttrControlCmd::OnBnClickedBtnReset()
{
	if( m_pSubStation)
	{
		m_pSubStation->Set_YK_Op_Result(0);

		if (m_pSubStation->blEnbaleTrans() == true)
		{

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CPacketFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息

			m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_90H(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_RESET_RTU,nElapse,NULL);


			CString strLog="";
			strLog.Format("%s 遥测终端复位操作!",
				m_pSubStation->Get_Log_RtuInfo());
			m_pSubStation->AddYkOperateLog(strLog);
		}
	}
}

void CRtuAttrControlCmd::OnBnClickedBtnClearhis()
{
	if( m_pSubStation)
	{
		m_pSubStation->Set_YK_Op_Result(0);

		if (m_pSubStation->blEnbaleTrans() == true)
		{

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CPacketFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息

			m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_91H(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_CLEAR_HISDATA,nElapse,NULL);

			CString strLog="";
			strLog.Format("%s 遥测终端清除历史数据单元操作!",
				m_pSubStation->Get_Log_RtuInfo());
			m_pSubStation->AddYkOperateLog(strLog);
		}
	}
}

void CRtuAttrControlCmd::OnBnClickedCheckClear0()
{
	UpdateData(TRUE);
	if (m_pSubStation)
	{
		bool blCheck = false;
		BOOL blChecked = m_CheckClear0.GetCheck();
		blCheck = (blChecked? true:false);

		m_pSubStation->GetClearHistoryDataPara()->SetBitValue(0,blCheck);
	}
}

void CRtuAttrControlCmd::OnBnClickedCheckClear1()
{
	UpdateData(TRUE);
	if (m_pSubStation)
	{
		bool blCheck = false;
		BOOL blChecked = m_CheckClear1.GetCheck();
		blCheck = (blChecked? true:false);

		m_pSubStation->GetClearHistoryDataPara()->SetBitValue(1,blCheck);
	}
}

void CRtuAttrControlCmd::OnBnClickedCheckClear2()
{
	UpdateData(TRUE);
	if (m_pSubStation)
	{
		bool blCheck = false;
		BOOL blChecked = m_CheckClear2.GetCheck();
		blCheck = (blChecked? true:false);

		m_pSubStation->GetClearHistoryDataPara()->SetBitValue(2,blCheck);
	}
}

void CRtuAttrControlCmd::OnBnClickedCheckClear3()
{
	UpdateData(TRUE);
	if (m_pSubStation)
	{
		bool blCheck = false;
		BOOL blChecked = m_CheckClear3.GetCheck();
		blCheck = (blChecked? true:false);

		m_pSubStation->GetClearHistoryDataPara()->SetBitValue(3,blCheck);
	}
}


void CRtuAttrControlCmd::OnTimer(UINT_PTR nIDEvent)
{
	CString strLog ="";
	_Log_level_type logLevel = Normal_Log_Message;
	
	BYTE YK_Result = 0;
	if (m_pSubStation)
		YK_Result = m_pSubStation->Get_YK_Op_Result();

	switch (nIDEvent)
	{
	case W_TIMER_PUMPGATE_START:  
		{
			if (m_blResult == TRUE)
				strLog = "遥控启动水泵或阀门闸门 成功!";
			else if (m_blResult == FALSE)
			{
				if (YK_Result == YKStart_PumpGate_ERR)
					strLog = "遥控启动水泵或阀门闸门 失败!";
				else
					strLog = "遥控启动水泵或阀门闸门 超时!";
				logLevel = Warning_log_Message;
			}
		}
		break;
	case W_TIMER_PUMPGATE_CLOSE:  
		{
			if (m_blResult == TRUE)
				strLog = "遥控关闭水泵或阀门闸门 成功!";
			else if (m_blResult == FALSE)
			{
				if (YK_Result == YKClose_PumpGate_ERR)
					strLog = "遥控关闭水泵或阀门闸门 失败!";
				else 
					strLog = "遥控关闭水泵或阀门闸门 超时!";
				logLevel = Warning_log_Message;
			}
		}
		break; 
	case W_TIMER_ICCARD_ENABLE: 
		{
			if (m_blResult == TRUE)
				strLog ="置遥测终端IC卡功能有效 成功!";
			else if (m_blResult == FALSE)
			{
				if (YK_Result == YKICCard_Enable_ERR)
					strLog = "置遥测终端IC卡功能有限 失败!";
				else
					strLog = "置遥测终端IC卡功能有限 超时!";
				logLevel = Warning_log_Message;
			}
		}
		break;     
	case W_TIMER_ICCARD_DISENABLE:
		{
			if (m_blResult == TRUE)
				strLog = "取消遥测终端IC卡功能 成功!";
			else if (m_blResult == FALSE)
			{
				if (YK_Result == YKICCard_DisEnable_ERR)
					strLog = "取消遥测终端IC卡功能 失败!";
				else
					strLog = "取消遥测终端IC卡功能 超时!";
				logLevel = Warning_log_Message;
			}
		}
		break;  
	case W_TIMER_ZD_INPUT:	
		{
			if (m_blResult == TRUE)
				strLog = "定值控制功能投入 成功!";
			else if (m_blResult == FALSE)
			{
				if (YK_Result == YKZD_Input_ERR)
					strLog = "定值控制功能投入 失败!";
				else
					strLog = "定值控制功能投入 超时!";
				logLevel = Warning_log_Message;
			}
		}
		break;
	case W_TIMER_ZD_EXIT:	
		{
			if (m_blResult == TRUE)
				strLog = "定值控制功能退出 成功!";
			else if (m_blResult == FALSE)
			{  
				if (YK_Result == YKZD_Exit_ERR)
					strLog = "定值控制功能退出 失败";
				else
					strLog = "定值控制功能退出 超时";
				logLevel = Warning_log_Message;
			}
		}
		break;	
	case W_TIMER_RESET_RTU:
		{
			if (m_blResult == TRUE)
				strLog = "复位遥测终端参数和状态 成功!";
			else if (m_blResult == FALSE)
			{
				if (YK_Result == YKReset_Rtu_ERR)
					strLog = "复位遥测终端参数和状态 失败!";
				else
					strLog = "复位遥测终端参数和状态 超时!";
				logLevel = Warning_log_Message;
			}
		}
		break;
	case W_TIMER_CLEAR_HISDATA:    
		{
			if (m_blResult == TRUE)
				strLog = "清空遥测终端的历史数据单元 成功!";
			else if (m_blResult == FALSE)
			{
				if (YK_Result == YKClear_his_ERR)
					strLog = "清空遥测终端的历史数据单元 失败!";
				else
					strLog = "清空遥测终端的历史数据单元 超时!";
				logLevel = Warning_log_Message;
			}
		}
		break;
	}

	KillTimer(nIDEvent);
	SetWndsEnable(TRUE);
	if (m_pParentDlg && m_pParentDlg->GetSafeHwnd() && !strLog.IsEmpty())
		m_pParentDlg->AddLog(strLog,(PSYSTEMTIME)NULL,logLevel);

	if (m_pSubStation)
		m_pSubStation->InsertLogMessage(strLog,logLevel,NULL);

	//插入日志信息
	if (m_pSubStation != NULL)
	{
		CString strMsg="";
		strMsg.Format("%s %s",m_pSubStation->Get_Log_RtuInfo(),strLog);
		m_pSubStation->AddYkOperateLog(strMsg,logLevel);
	}

	CDialog::OnTimer(nIDEvent);
}


void CRtuAttrControlCmd::KillAllTimers()
{
	KillTimer(W_TIMER_PUMPGATE_START);
	KillTimer(W_TIMER_PUMPGATE_CLOSE);
	KillTimer(W_TIMER_ICCARD_ENABLE);
	KillTimer(W_TIMER_ICCARD_DISENABLE);
	KillTimer(W_TIMER_ZD_INPUT);
	KillTimer(W_TIMER_ZD_EXIT);
	KillTimer(W_TIMER_CLEAR_HISDATA);
	KillTimer(W_TIMER_RESET_RTU);

	SetWndsEnable(TRUE);
}