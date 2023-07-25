// RtuAttrHydrWaterLevel.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuAttrHydrWaterLevel.h"

#define W_TIMER_WATERLEVEL_DATA_QUERY 0x01 //水位实时数据查询
#define W_TIMER_WATERLEVEL_DATA_QUERY_ER 0x02 //水位实时数据查询
// CRtuAttrHydrWaterLevel 对话框

IMPLEMENT_DYNAMIC(CRtuAttrHydrWaterLevel, CDialog)

CRtuAttrHydrWaterLevel::CRtuAttrHydrWaterLevel(CWnd* pParent /*=NULL*/)
	: CDialog(CRtuAttrHydrWaterLevel::IDD, pParent)
{
	m_blResult = FALSE;
	m_pSubStation = NULL;
	m_pParentDlg = NULL;
}

CRtuAttrHydrWaterLevel::~CRtuAttrHydrWaterLevel()
{
}

void CRtuAttrHydrWaterLevel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_CBX_SENSORNO ,m_wndSensorNoCbx);
	DDX_Control(pDX,IDC_CBX_TIMETYPE ,m_wndTimeTypeCbx);	
	DDX_Control(pDX,IDC_D_START ,m_wndDateStartDtc);
	DDX_Control(pDX,IDC_D_END ,m_wndDateEndDtc);
	DDX_Control(pDX,IDC_STATIC_START ,m_wndTimeStartLabel);
	DDX_Control(pDX,IDC_STATIC_END ,m_wndTimeEndLabel);
 	DDX_Control(pDX,IDC_BTN_QUERYWATERLEVELDATA,m_btnWaterLevelOK);
}


BEGIN_MESSAGE_MAP(CRtuAttrHydrWaterLevel, CDialog)
	ON_BN_CLICKED(IDC_BTN_QUERYWATERLEVELDATA,          &CRtuAttrHydrWaterLevel::OnBnClickQueryWaterLevel)
	ON_CBN_SELCHANGE(IDC_CBX_TIMETYPE,                  &CRtuAttrHydrWaterLevel::OnCbnSelChangeTimeType)
	ON_CBN_SELCHANGE(IDC_CBX_SENSORNO,                  &CRtuAttrHydrWaterLevel::OnCbnSelChangeSensorNo)
	ON_WM_TIMER()
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_D_START, &CRtuAttrHydrWaterLevel::OnStartDataTimeChange)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_D_END, &CRtuAttrHydrWaterLevel::OnEndDataTimeChange)
END_MESSAGE_MAP()


//CRtuAttrHydrWaterLevel 消息处理程序
void CRtuAttrHydrWaterLevel::SetParentDlg(CRtuPropertyDlg* pDlg)
{
	m_pParentDlg = pDlg;

}

 void CRtuAttrHydrWaterLevel::Update()										//更新界面的显示信息
{
	if (m_pParentDlg)
		m_pSubStation = m_pParentDlg->GetRtuStation();	


	if (m_wndGridCtrl.GetSafeHwnd())
	{
		m_wndGridCtrl.DeleteAllItems();
		m_wndGridCtrl.Refresh();
	}

	if (m_pSubStation != NULL)
	{
		CHydrWaterLevel *pWaterLevel = m_pSubStation->GetHydrWaterLevel();
		if (NULL == pWaterLevel)
		{
			return;
		}

		if (m_wndSensorNoCbx.GetSafeHwnd())
		{
			int nSel = m_wndSensorNoCbx.GetCurSel();
			if (CB_ERR == nSel)
				return;

			CString strText = "";
			m_wndSensorNoCbx.GetLBText(nSel, strText);
			
			if (pWaterLevel->GetSensorNo() == atoi(strText))
			{
				OnCbnSelChangeSensorNo();
			}
			
		}
	}
 }

void CRtuAttrHydrWaterLevel::UpdateDlg(int attrType,BOOL blResult)			//接收到报文后、更新界面
{
	m_blResult = blResult;
	if (attrType == scsw_waterLevel_call)
		SendMessage(WM_TIMER, W_TIMER_WATERLEVEL_DATA_QUERY, NULL);
}


void CRtuAttrHydrWaterLevel::KillAllTimers()
{
	KillTimer(W_TIMER_WATERLEVEL_DATA_QUERY);
	SetWndsEnable(TRUE);
}

void CRtuAttrHydrWaterLevel::SetWndsEnable(BOOL blEnable)
{
	m_wndSensorNoCbx.EnableWindow(blEnable);
	m_wndTimeTypeCbx.EnableWindow(blEnable);
	m_wndDateStartDtc.EnableWindow(blEnable);
	m_wndDateEndDtc.EnableWindow(blEnable);
	m_wndGridCtrl.EnableWindow(blEnable);
	m_btnWaterLevelOK.EnableWindow(blEnable);
}

BOOL CRtuAttrHydrWaterLevel::OnInitDialog()
{
	CDialog::OnInitDialog();
	CWnd* pWnd = GetDlgItem(IDC_STATIC_WATERLEVELGRID);
	CRect rect1, rect2;
	int captionHeight = 0; 
	int cxframe = GetSystemMetrics(SM_CXFRAME);
	int cyframe = GetSystemMetrics(SM_CYFRAME);
	this->GetWindowRect(&rect2);
	if (pWnd)
		pWnd->GetWindowRect(&rect1);
	if (!m_wndGridCtrl.Create(CRect(rect1.left - rect2.left - cxframe, 
		rect1.top-rect2.top - cyframe-captionHeight, 
		rect1.left + rect1.Width() - rect2.left, 
		rect1.top + rect1.Height() - rect2.top - captionHeight), 
		this, IDC_WATERLEVEL_GRIDCTRL,WS_CHILD|WS_VISIBLE))
	{
		TRACE("Failed to create threads  grid!\n");
		return FALSE;
	}

	m_wndGridCtrl.SetBkColor(RGB(255,255,255));
	m_wndGridCtrl.SetListMode(TRUE);
	m_wndGridCtrl.SetSingleColSelection(TRUE);
	m_wndGridCtrl.SetSingleRowSelection(TRUE);

	m_wndSensorNoCbx.SetCurSel(0);
	m_wndTimeTypeCbx.SetCurSel(0);
	OnCbnSelChangeTimeType();

	return TRUE;
}

void CRtuAttrHydrWaterLevel::OnBnClickQueryWaterLevel()
{
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CHydrSendFrame appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息

			int nsel = m_wndTimeTypeCbx.GetCurSel();
			
			if (0==nsel)
			{
				appFrame.m_nTimeType = TIMETYPE_CURR;
				appFrame.t_a = m_timeA;
			}
		    else if (1==nsel)
			{
				appFrame.m_nTimeType = TIMETYPE_TB;
				appFrame.t_a = m_timeA;
				appFrame.t_b = m_timeB;
			}
			else
			{
				appFrame.m_nTimeType = TIMETYPE_TT;
				appFrame.t_a = m_timeA;
			}
			
			
			CString strSensor = "";
			m_wndSensorNoCbx.GetWindowText(strSensor);
			appFrame.m_LPARME = atoi(strSensor);
			m_pSubStation->GetSysDataTrans_Hydr()->SCSW_hydrdata_EN_Z(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_WATERLEVEL_DATA_QUERY, nElapse, NULL);
		}
	}
}


void CRtuAttrHydrWaterLevel::InitGridItems()
{
	int nSel = 0;
	if (m_wndGridCtrl.GetSafeHwnd())
	{
		m_wndGridCtrl.DeleteAllItems();
		nSel = m_wndSensorNoCbx.GetCurSel();
		if (nSel == CB_ERR) return;
		if (!m_pSubStation) return;

		CString strHeader[] = {
			"传感器号",
			"时间",
			"实时值",
			"单位"
		};

		m_wndGridCtrl.SetColumnCount(2);
		m_wndGridCtrl.SetRowCount(4);
		m_wndGridCtrl.SetColumnWidth(0,70);
		m_wndGridCtrl.SetColumnWidth(1,120);

		for (int i=0; i<4; i++)
		{
			m_wndGridCtrl.SetItemText(i,0,strHeader[i]);
			CUdfGlobalFUNC::SetGridCellReadOnly(&m_wndGridCtrl,i,0);
			CUdfGlobalFUNC::SetGridCellReadOnly(&m_wndGridCtrl,i,1);
			m_wndGridCtrl.SetItemBkColour(i,0,RGB(128,128,128));
			m_wndGridCtrl.SetRowHeight(i,MFC_GRIDCTRL_HEIGHT);
		}

		CString strSensorNo = "";
		m_wndSensorNoCbx.GetWindowText(strSensorNo);
		int nCurrentSensorNo = atoi(strSensorNo);
		CHydrWaterLevel *pData = m_pSubStation->GetHydrWaterLevel();
		if (NULL == pData) return;

		CString strText = "";
		strText.Format("%d", nCurrentSensorNo);
		m_wndGridCtrl.SetItemText(0, 1, strText);

		strText.Format("%-2d月%-2d日%-2d时%-2d分",
			pData->GetTime().tm_mon,
			pData->GetTime().tm_mday,
			pData->GetTime().tm_hour,
			pData->GetTime().tm_min);
		m_wndGridCtrl.SetItemText(1, 1, strText);

		strText.Format("%.2f", pData->GetRealValue());
		m_wndGridCtrl.SetItemText(2, 1, strText);

		m_wndGridCtrl.SetItemText(3, 1, "m");

		m_wndGridCtrl.Refresh();
	}
}

void CRtuAttrHydrWaterLevel::OnTimer(UINT_PTR nIDEvent)
{
	CString strLogMessge = "";
	_Log_level_type _logLevle = Normal_Log_Message;

	switch (nIDEvent)
	{
	case W_TIMER_WATERLEVEL_DATA_QUERY:
		{
			if (m_blResult == TRUE)
				strLogMessge = "查询遥测终端水位值 成功!";
			else if( m_blResult == FALSE)
			{
				strLogMessge = "查询遥测终端水位值 超时!";
				_logLevle = Warning_log_Message;
			}
			else if (m_blResult == 0xff)
			{
				strLogMessge = "查询遥测终端水位值 错误!";
				_logLevle = Warning_log_Message;

			}
		}
		break;
	}
	KillTimer(nIDEvent);
	SetWndsEnable(TRUE);

	if (m_pParentDlg && m_pParentDlg->GetSafeHwnd())
		m_pParentDlg->AddLog(strLogMessge, NULL, _logLevle);

	if (m_pSubStation)
		m_pSubStation->InsertLogMessage(strLogMessge, _logLevle, NULL);

	CDialog::OnTimer(nIDEvent);
}

void CRtuAttrHydrWaterLevel::OnCbnSelChangeSensorNo()
{
	InitGridItems();
}

void CRtuAttrHydrWaterLevel::OnCbnSelChangeTimeType()
{
	if (m_wndTimeTypeCbx.GetSafeHwnd())
	{
		SYSTEMTIME sysTime;
		int nSel = m_wndTimeTypeCbx.GetCurSel();
		if (CB_ERR == nSel) return;

		switch(nSel)
		{
		case 0://当前时间
			m_wndTimeStartLabel.ShowWindow(SW_HIDE);
			m_wndDateStartDtc.ShowWindow(SW_HIDE);
			m_wndTimeEndLabel.ShowWindow(SW_HIDE);
			m_wndDateEndDtc.ShowWindow(SW_HIDE);
			
			m_wndDateStartDtc.SetFormat("yyyy年MM月dd日 HH时mm分ss秒");
			memset(&sysTime, 0, sizeof(sysTime));
			::GetLocalTime(&sysTime);
			VERIFY(m_wndDateStartDtc.SetTime(&sysTime));
			m_timeA = sysTime;
			break;
		case 1://历史查询
			m_wndTimeStartLabel.SetWindowText("起始时间");
			m_wndTimeEndLabel.SetWindowText("结束时间");
			m_wndTimeEndLabel.ShowWindow(SW_SHOW);
			m_wndDateEndDtc.ShowWindow(SW_SHOW);
			m_wndDateStartDtc.ShowWindow(SW_SHOW);
			m_wndTimeStartLabel.ShowWindow(SW_SHOW);

			m_wndDateStartDtc.SetFormat("yyyy年MM月dd日 HH时mm分ss秒");
			m_wndDateEndDtc.SetFormat("yyyy年MM月dd日 HH时mm分ss秒");
			memset(&sysTime, 0, sizeof(sysTime));
			::GetLocalTime(&sysTime);
			VERIFY(m_wndDateStartDtc.SetTime(&sysTime));
			VERIFY(m_wndDateEndDtc.SetTime(&sysTime));
			m_timeA = sysTime;
			m_timeB = sysTime;
			break;
		}
	}

}


void CRtuAttrHydrWaterLevel::OnStartDataTimeChange(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_timeA = pDTChange->st;
	*pResult = 0;
}

void CRtuAttrHydrWaterLevel::OnEndDataTimeChange(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_timeB = pDTChange->st;
	*pResult = 0;
}
