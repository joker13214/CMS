// RtuAttrPulse.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuAttrPulse.h"


#define  W_TIMER_QUERY_PULSEDATA     0x02      //查询雨量实时数据

// CRtuAttrPulse 对话框
IMPLEMENT_DYNAMIC(CRtuAttrPulse, CDialog)

CRtuAttrPulse::CRtuAttrPulse(CWnd* pParent /*=NULL*/)
	: CDialog(CRtuAttrPulse::IDD, pParent)
	, m_nPulseCount(0)
{
	m_pSubStation = NULL;
	m_pParentDlg =  NULL;
	m_blResult = FALSE;
}

CRtuAttrPulse::~CRtuAttrPulse()
{
}

void CRtuAttrPulse::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_QUERYPULSE, m_btnPulseQuery);
	DDX_Control(pDX, IDC_COMBO_PULSEPARA, m_wndPulseCmb);
	DDX_Text(pDX, IDC_EDIT_PULSECOUNT, m_nPulseCount);
	DDX_Control(pDX, IDC_EDIT_PULSECOUNT, m_wndPulseCountEdit);
	DDX_Control(pDX, IDC_STATIC_PULSECAPTION, m_wndPulseCaption);
}

BEGIN_MESSAGE_MAP(CRtuAttrPulse, CDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_QUERYPULSE, &CRtuAttrPulse::OnBnClickedBtnQuerypulse)
	ON_CBN_SELCHANGE(IDC_COMBO_PULSEPARA, &CRtuAttrPulse::OnCbnSelchangeComboPulsepara)
END_MESSAGE_MAP()

void CRtuAttrPulse::SetParentDlg( CRtuPropertyDlg* pDlg )
{
	m_pParentDlg = pDlg;
}

void CRtuAttrPulse::Update()
{
	if (m_pParentDlg)
		m_pSubStation = m_pParentDlg->GetRtuStation();

	if (m_pSubStation)
	{
		int nPulseCount = (int)m_pSubStation->GetPulseParasMap()->size();

		CString strText ="";
		strText.Format("%d",nPulseCount);
		m_wndPulseCountEdit.SetWindowText(strText);

		int idx = 0;
		if (m_wndPulseCmb.GetSafeHwnd())
		{
			m_wndPulseCmb.ResetContent();

			std::map<UINT64,CPulsePara_PER*>::iterator It;
			for (It = m_pSubStation->GetPulseParasMap()->begin();
				 It != m_pSubStation->GetPulseParasMap()->end();
				 It ++)
			{
				if (It->second)
				{
					idx ++;
					strText.Format("第 %-2d 个电度量记",idx);
					int Index = m_wndPulseCmb.AddString(strText);
					m_wndPulseCmb.SetItemDataPtr(Index,It->second);
				}
			}

			if (m_wndPulseCmb.GetCount())
			{
				m_wndPulseCmb.SetCurSel(0);
			}
			else
			{
				m_wndPulseCmb.SetCurSel(-1);
			}
			OnCbnSelchangeComboPulsepara();
		}
	}

}

void CRtuAttrPulse::UpdateDlg( int attrType,BOOL blResult )
{
	m_blResult = blResult;
	switch (attrType)
	{
	case rtu_attr_pulsepara_call:
		SendMessage(WM_TIMER,W_TIMER_QUERY_PULSEDATA,NULL);
		break;
	}

	Update();
}

void CRtuAttrPulse::KillAllTimers()
{
	KillTimer(W_TIMER_QUERY_PULSEDATA);
	SetWndCtrlEnable(TRUE);
}

// CRtuAttrPulse 消息处理程序
void CRtuAttrPulse::OnBnClickedBtnQuerypulse()
{
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{
			SetWndCtrlEnable(FALSE);
			m_blResult = FALSE;

			CPacketFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);						//生成初始的下发帧报文帧数据信息
			appFrame.ControlC._Command = C_CMD_13_PULSEPARA;
			m_pSubStation->SetQueryRealDataCmd(appFrame.ControlC._Command);

			if (m_pSubStation->GetAppProVer() == APP_PROTOCOL_V1_0)
				m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_67H(&appFrame);
			else if (m_pSubStation->GetAppProVer() == APP_PROTOCOL_V2_0)
				m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_68H(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_QUERY_PULSEDATA,nElapse,NULL);
		}
	}
}

void CRtuAttrPulse::OnCbnSelchangeComboPulsepara()
{
	m_wndGridCtrl.DeleteAllItems();
	m_wndGridCtrl.Refresh();
	int nSel = m_wndPulseCmb.GetCurSel();
	if (nSel != CB_ERR)
	{
		CString strTemp="",strCaption = "";
        m_wndPulseCmb.GetWindowText(strTemp);
		strCaption.Format("%s 的实时值",strTemp);

		m_wndPulseCaption.SetWindowText(strCaption);

		CPulsePara_PER* pPulseObject = (CPulsePara_PER*)m_wndPulseCmb.GetItemDataPtr(nSel);
		if (pPulseObject)
		{
			InitGridCtrl();

			CString strText ="";
			strText.Format("%d",pPulseObject->GetCode());
			m_wndGridCtrl.SetItemText(1,1,strText);

			strText.Format("%.2f kWh",pPulseObject->GetValue1());
			m_wndGridCtrl.SetItemText(2,1,strText);

			strText.Format("%.2f kWh",pPulseObject->GetValue2());
			m_wndGridCtrl.SetItemText(3,1,strText);

			strText.Format("%.2f kWh",pPulseObject->GetValue3());
			m_wndGridCtrl.SetItemText(4,1,strText);

			strText.Format("%.2f kWh",pPulseObject->GetValue4());
			m_wndGridCtrl.SetItemText(5,1,strText);
		}

		m_wndGridCtrl.Refresh();
	}
	else
	{
		m_wndGridCtrl.DeleteAllItems();
		m_wndGridCtrl.Refresh();
	}
}

BOOL CRtuAttrPulse::OnInitDialog()
{
	CDialog::OnInitDialog();

	CWnd* pWnd = GetDlgItem(IDC_STATIC_PULSE);
	CRect rect1, rect2;
	int captionHeight = 0; // ::GetSystemMetrics(SM_CYCAPTION);
	int cxframe = GetSystemMetrics(SM_CXFRAME);
	int cyframe = GetSystemMetrics(SM_CYFRAME);
	this->GetWindowRect(&rect2);
	if (pWnd)
		pWnd->GetWindowRect(&rect1);
	if (!m_wndGridCtrl.Create(CRect(rect1.left - rect2.left - cxframe, 
		rect1.top-rect2.top - cyframe-captionHeight, 
		rect1.left + rect1.Width() - rect2.left, 
		rect1.top + rect1.Height() - rect2.top - captionHeight), 
		this, IDC_PULSE_GRID,WS_CHILD|WS_VISIBLE))
	{
		TRACE("Failed to create threads  grid!\n");
		return FALSE;
	}

	m_wndGridCtrl.SetBkColor(RGB(255,255,255));
	m_wndGridCtrl.SetListMode(TRUE);
	m_wndGridCtrl.SetSingleColSelection(TRUE);
	m_wndGridCtrl.SetSingleRowSelection(TRUE);
	m_wndGridCtrl.SetEditable(FALSE);

	return TRUE;  
}

void CRtuAttrPulse::OnTimer( UINT_PTR nIDEvent )
{
	CString strLog= "";
	_Log_level_type LogLevel = Normal_Log_Message;

	switch (nIDEvent)
	{
	case  W_TIMER_QUERY_PULSEDATA:
		{
			if (m_blResult == TRUE)
				strLog = "查询遥测终端电度量参数实时数据 成功!";
			else
			{
				strLog = "查询遥测终端电度量参数实时数据 超时!";
				LogLevel = Warning_log_Message;
			}
		}
	}

	KillTimer(nIDEvent);
	SetWndCtrlEnable(TRUE);
	if (m_pParentDlg && m_pParentDlg->GetSafeHwnd())
		m_pParentDlg->AddLog(strLog,(PSYSTEMTIME)NULL,LogLevel);

	if (m_pSubStation)
		m_pSubStation->InsertLogMessage(strLog,LogLevel,NULL);
	CDialog::OnTimer(nIDEvent);
}

void CRtuAttrPulse::SetWndCtrlEnable( BOOL blEnable )
{

	m_wndPulseCmb.EnableWindow(blEnable);
	m_wndGridCtrl.EnableWindow(blEnable);
	m_btnPulseQuery.EnableWindow(blEnable);
}

void CRtuAttrPulse::InitGridCtrl()
{
	if (m_wndGridCtrl.GetSafeHwnd())
	{
		m_wndGridCtrl.DeleteAllItems();
		m_wndGridCtrl.SetRowCount(6);
		m_wndGridCtrl.SetColumnCount(2);
		m_wndGridCtrl.SetFixedRowCount(1);

		CString strHeaders[]={_T("选项"),_T("值")};
		int colWidth[] = {110,80};

		m_wndGridCtrl.SetItemText(0,0,strHeaders[0]);
		m_wndGridCtrl.SetItemText(0,1,strHeaders[1]);

		m_wndGridCtrl.SetColumnWidth(0,colWidth[0]);
		m_wndGridCtrl.SetColumnWidth(1,colWidth[1]);
		m_wndGridCtrl.SetRowHeight(0,MFC_GRIDCTRL_HEIGHT);

		m_wndGridCtrl.SetItemText(1,0,"地址码");
		m_wndGridCtrl.SetRowHeight(1,MFC_GRIDCTRL_HEIGHT);

		m_wndGridCtrl.SetItemText(2,0,"正向有功");
		m_wndGridCtrl.SetRowHeight(2,MFC_GRIDCTRL_HEIGHT);

		m_wndGridCtrl.SetItemText(3,0,"正向无功");
		m_wndGridCtrl.SetRowHeight(3,MFC_GRIDCTRL_HEIGHT);

		m_wndGridCtrl.SetItemText(4,0,"反向有功");
		m_wndGridCtrl.SetRowHeight(4,MFC_GRIDCTRL_HEIGHT);

		m_wndGridCtrl.SetItemText(5,0,"反向无功");
		m_wndGridCtrl.SetRowHeight(5,MFC_GRIDCTRL_HEIGHT);
	}
}
