// RtuAttrHydrEvaporate.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuAttrHydrEvaporate.h"


// CRtuAttrHydrEvaporate 对话框
#define W_TIMER_WATERLEVEL_PARA_QUERY 0x01 //水位参数查询
#define W_TIMER_WATERLEVEL_REAL_QUERY 0x02  //水位实时数据查询

IMPLEMENT_DYNAMIC(CRtuAttrHydrEvaporate, CDialog)

CRtuAttrHydrEvaporate::CRtuAttrHydrEvaporate(CWnd* pParent /*=NULL*/)
	: CDialog(CRtuAttrHydrEvaporate::IDD, pParent)
{

}

CRtuAttrHydrEvaporate::~CRtuAttrHydrEvaporate()
{
}

void CRtuAttrHydrEvaporate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_CBX_TIMETYPE ,m_wndTimeTypeCbx);	
	DDX_Control(pDX,IDC_D_START ,m_wndDateStartDtc);
	DDX_Control(pDX,IDC_D_END ,m_wndDateEndDtc);
	DDX_Control(pDX,IDC_STATIC_START ,m_wndTimeStartLabel);
	DDX_Control(pDX,IDC_STATIC_END ,m_wndTimeEndLabel);
}

BOOL CRtuAttrHydrEvaporate::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;
}


BEGIN_MESSAGE_MAP(CRtuAttrHydrEvaporate, CDialog)
	ON_CBN_SELCHANGE(IDC_CBX_TIMETYPE,      &CRtuAttrHydrEvaporate::OnCbnSelChangeTimeType)
	ON_BN_CLICKED(IDC_BTN_QUERYEVAPORATE,		&CRtuAttrHydrEvaporate::OnBtnClickedQueryEvaporate)
    ON_WM_TIMER()
END_MESSAGE_MAP()


// CRtuAttrHydrEvaporate 消息处理程序
void CRtuAttrHydrEvaporate::SetParentDlg(CRtuPropertyDlg* pDlg)
{
	m_pParentDlg = pDlg;

}

void CRtuAttrHydrEvaporate::Update()
{
}

void CRtuAttrHydrEvaporate::UpdateDlg(int attrType,BOOL blResult)			//接收到报文后、更新界面
{
	m_blResult = blResult;
	if (attrType == rtu_attr_waterlevel_data_call)
		SendMessage(WM_TIMER,W_TIMER_WATERLEVEL_REAL_QUERY,NULL);
	else if (attrType == rtu_attr_waterlevel_call)
		SendMessage(WM_TIMER,W_TIMER_WATERLEVEL_PARA_QUERY,NULL);

}

void CRtuAttrHydrEvaporate::KillAllTimers()
{
	KillTimer(W_TIMER_WATERLEVEL_PARA_QUERY);
	KillTimer(W_TIMER_WATERLEVEL_REAL_QUERY);
	SetWndsEnable(TRUE);
}

void CRtuAttrHydrEvaporate::SetWndsEnable(BOOL blEnable)
{
}

void CRtuAttrHydrEvaporate::InitGridItems()
{

}

void CRtuAttrHydrEvaporate::OnCbnSelChangeTimeType()
{
	if (m_wndTimeTypeCbx.GetSafeHwnd())
	{
		SYSTEMTIME sysTime;
		int nSel = m_wndTimeTypeCbx.GetCurSel();
		if (CB_ERR == nSel) return;

		switch(nSel)
		{
		case 0://当前时间
			m_wndTimeStartLabel.SetWindowText("当前时间");
			m_wndTimeEndLabel.ShowWindow(SW_HIDE);
			m_wndDateEndDtc.ShowWindow(SW_HIDE);

			m_wndDateStartDtc.SetFormat("yyyy年MM月dd日 HH时mm分ss秒");
			memset(&sysTime, 0, sizeof(sysTime));
			::GetLocalTime(&sysTime);
			VERIFY(m_wndDateStartDtc.SetTime(&sysTime));
			break;
		case 1://设定时间
			m_wndTimeStartLabel.SetWindowText("指定时间");
			m_wndTimeEndLabel.ShowWindow(SW_HIDE);
			m_wndDateEndDtc.ShowWindow(SW_HIDE);

			m_wndDateStartDtc.SetFormat("yyyy年MM月dd日 HH时mm分ss秒");	
			memset(&sysTime, 0, sizeof(sysTime));
			::GetLocalTime(&sysTime);
			VERIFY(m_wndDateStartDtc.SetTime(&sysTime));
			break;
		case 2://历史查询
			m_wndTimeStartLabel.SetWindowText("起始时间");
			m_wndTimeEndLabel.SetWindowText("结束时间");
			m_wndTimeEndLabel.ShowWindow(SW_SHOW);
			m_wndDateEndDtc.ShowWindow(SW_SHOW);

			m_wndDateStartDtc.SetFormat("yyyy年MM月dd日 HH时mm分ss秒");
			m_wndDateEndDtc.SetFormat("yyyy年MM月dd日 HH时mm分ss秒");
			memset(&sysTime, 0, sizeof(sysTime));
			::GetLocalTime(&sysTime);
			VERIFY(m_wndDateStartDtc.SetTime(&sysTime));
			VERIFY(m_wndDateEndDtc.SetTime(&sysTime));
			break;
		case 3://最小极值
			m_wndTimeStartLabel.SetWindowText("最小极值");
			m_wndTimeEndLabel.ShowWindow(SW_HIDE);
			m_wndDateEndDtc.ShowWindow(SW_HIDE);

			m_wndDateStartDtc.SetFormat("yyyy年MM月dd日 HH时mm分ss秒");
			memset(&sysTime, 0, sizeof(sysTime));
			::GetLocalTime(&sysTime);
			VERIFY(m_wndDateStartDtc.SetTime(&sysTime));
			break;
		case 4://最大极值
			m_wndTimeStartLabel.SetWindowText("最大极值");
			m_wndTimeEndLabel.ShowWindow(SW_HIDE);
			m_wndDateEndDtc.ShowWindow(SW_HIDE);

			m_wndDateStartDtc.SetFormat("yyyy年MM月dd日 HH时mm分ss秒");
			memset(&sysTime, 0, sizeof(sysTime));
			::GetLocalTime(&sysTime);
			VERIFY(m_wndDateStartDtc.SetTime(&sysTime));
			break;

		}
	}
}

void CRtuAttrHydrEvaporate::OnBtnClickedQueryEvaporate()
{
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CHydrSendFrame appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息
			m_pSubStation->GetSysDataTrans_Hydr()->SCSW_hydrdata_EN_ED(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_WATERLEVEL_PARA_QUERY, nElapse, NULL);
		}
	}

}

void CRtuAttrHydrEvaporate::OnTimer(UINT_PTR nIDEvent)
{
	CString strLogMessge = "";
	_Log_level_type _logLevle = Normal_Log_Message;

	switch (nIDEvent)
	{
	case W_TIMER_WATERLEVEL_REAL_QUERY:
		{
			if (m_blResult == TRUE)
				strLogMessge = "查询遥测终端的蒸发值 成功!";
			else if( m_blResult == FALSE)
			{
				strLogMessge = "查询遥测终端的蒸发值 超时!";
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