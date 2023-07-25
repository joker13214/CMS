// RtuAttrRainValume.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuAttrRainValume.h"


#define  W_TIMER_RAINALERT_CALL		0x01      //雨量数据报警值查询Timer
#define  W_TIMER_QUERY_RAINDATA     0x02      //查询雨量实时数据

// CRtuAttrRainValume 对话框
IMPLEMENT_DYNAMIC(CRtuAttrRainValume, CDialog)

CRtuAttrRainValume::CRtuAttrRainValume(CWnd* pParent /*=NULL*/)
	: CDialog(CRtuAttrRainValume::IDD, pParent)
	, m_nRainCount(0)
{
	m_pSubStation = NULL;
	m_pParentDlg =  NULL;
	m_blResult = FALSE;
}

CRtuAttrRainValume::~CRtuAttrRainValume()
{
}

void CRtuAttrRainValume::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_RAINCOUNT, m_wndRainPCount);
	DDX_Control(pDX, IDC_COMBO_RAINPARA, m_wndRainCmb);
	DDX_Control(pDX, IDC_BTN_RAINACALL, m_btnRainACall);
	DDX_Control(pDX, IDC_BTN_RAINASET, m_btnRainASet);
	DDX_Control(pDX, IDC_STATIC_RAINCAPTION, m_wndRainCaption);
	DDX_Control(pDX, IDC_BTN_RAINOK, m_btnRainOK);
	DDX_Control(pDX, IDC_BTN_QUERYRAINREAL,m_btnQueryRainReal);
	DDX_Text(pDX, IDC_EDIT_RAINCOUNT, m_nRainCount);

}


BEGIN_MESSAGE_MAP(CRtuAttrRainValume, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_RAINPARA, &CRtuAttrRainValume::OnCbnSelchangeComboRainpara)
	ON_BN_CLICKED(IDC_BTN_RAINACALL, &CRtuAttrRainValume::OnBnClickedBtnRainacall)
	ON_BN_CLICKED(IDC_BTN_RAINASET, &CRtuAttrRainValume::OnBnClickedBtnRainaset)
	ON_BN_CLICKED(IDC_BTN_RAINOK, &CRtuAttrRainValume::OnBnClickedBtnRainok)
	ON_BN_CLICKED(IDC_BTN_QUERYRAINREAL,&CRtuAttrRainValume::OnBnClickBtnQueryRainReal)
	ON_NOTIFY(GVN_ENDLABELEDIT,IDC_RAINALERT_GRID,&CRtuAttrRainValume::OnEndEditRainGrid)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CRtuAttrRainValume 消息处理程序
void CRtuAttrRainValume::SetParentDlg(CRtuPropertyDlg* pDlg)
{
	m_pParentDlg = pDlg;

}
void CRtuAttrRainValume::Update()										//更新界面的显示信息
{
	if (m_pParentDlg)
		m_pSubStation = m_pParentDlg->GetRtuStation();

	if (m_pSubStation)
	{
		int nRainCount = (int)m_pSubStation->GetRainAlertParasList()->GetCount();

		CString strText ="";
		strText.Format("%d",nRainCount);
		m_wndRainPCount.SetWindowText(strText);

		int idx = 0;
		if (m_wndRainCmb.GetSafeHwnd())
		{
			m_wndRainCmb.ResetContent();

			for (idx = 0; idx < nRainCount; idx ++)
			{
				strText.Format("第 %-2d 个雨量点",idx + 1);
				int Index = m_wndRainCmb.AddString(strText);
				m_wndRainCmb.SetItemData(Index,idx);
			}

			if (m_wndRainCmb.GetCount())
			{
				m_wndRainCmb.SetCurSel(0);
			}
			else
			{
				m_wndRainCmb.SetCurSel(-1);
			}
			OnCbnSelchangeComboRainpara();
		}
	}

}

void CRtuAttrRainValume::UpdateDlg(int attrType,BOOL blResult)									//接收到报文后、更新界面
{
	m_blResult = blResult;
	switch (attrType)
	{
	case  rtu_attr_rainalert_call:
		SendMessage(WM_TIMER,W_TIMER_RAINALERT_CALL,NULL);
		break;
	case rtu_attr_rain_data_call:
		SendMessage(WM_TIMER,W_TIMER_QUERY_RAINDATA,NULL);
		break;
	}
}

BOOL CRtuAttrRainValume::OnInitDialog()
{
	CDialog::OnInitDialog();

	CWnd* pWnd = GetDlgItem(IDC_STATIC_RAINPARA);
	CRect rect1, rect2;
	int captionHeight = 0; // ::GetSystemMetrics(SM_CYCAPTION);
	int cxframe = GetSystemMetrics(SM_CXFRAME);
	int cyframe = GetSystemMetrics(SM_CYFRAME);
	this->GetWindowRect(&rect2);
	if (pWnd)
		pWnd->GetWindowRect(&rect1);
	if (!m_wndRainGrid.Create(CRect(rect1.left - rect2.left - cxframe, 
		rect1.top-rect2.top - cyframe-captionHeight, 
		rect1.left + rect1.Width() - rect2.left, 
		rect1.top + rect1.Height() - rect2.top - captionHeight), 
		this, IDC_RAINALERT_GRID,WS_CHILD|WS_VISIBLE))
	{
		TRACE("Failed to create threads  grid!\n");
		return FALSE;
	}

	m_wndRainGrid.SetBkColor(RGB(255,255,255));
	m_wndRainGrid.SetListMode(TRUE);
	m_wndRainGrid.SetSingleColSelection(TRUE);
	m_wndRainGrid.SetSingleRowSelection(TRUE);

	return TRUE;  
}

void CRtuAttrRainValume::FillRainGridCtrl()
{
	if (m_wndRainGrid.GetSafeHwnd())
	{
		int nRowsCount = 4;

		if (m_pSubStation && m_pSubStation->GetAppProVer() == APP_PROTOCOL_V2_0)
			nRowsCount = 5;

		m_wndRainGrid.DeleteAllItems();
		m_wndRainGrid.SetRowCount(nRowsCount);
		m_wndRainGrid.SetColumnCount(2);
		m_wndRainGrid.SetFixedRowCount(1);

		CString strHeaders[]={_T("选项"),_T("值")};
		int colWidth[] = {110,80};

		m_wndRainGrid.SetItemText(0,0,strHeaders[0]);
		m_wndRainGrid.SetItemText(0,1,strHeaders[1]);

		m_wndRainGrid.SetColumnWidth(0,colWidth[0]);
		m_wndRainGrid.SetColumnWidth(1,colWidth[1]);
		m_wndRainGrid.SetRowHeight(0,MFC_GRIDCTRL_HEIGHT);


		m_wndRainGrid.SetItemText(1,0,"报警值(0~999.9)");
		CUdfGlobalFUNC::SetGridCellReadOnly(&m_wndRainGrid,1,0);
		CUdfGlobalFUNC::SetGridCellNormal(&m_wndRainGrid,1,1);
		m_wndRainGrid.SetRowHeight(1,MFC_GRIDCTRL_HEIGHT);

		m_wndRainGrid.SetItemText(2,0,"单位时间(min)");
		CUdfGlobalFUNC::SetGridCellReadOnly(&m_wndRainGrid,2,0);
		CUdfGlobalFUNC::SetGridCellNormal(&m_wndRainGrid,2,1);
		m_wndRainGrid.SetRowHeight(2,MFC_GRIDCTRL_HEIGHT);

		m_wndRainGrid.SetItemText(3,0,"实时值(mm/h)");
		CUdfGlobalFUNC::SetGridCellReadOnly(&m_wndRainGrid,3,0);
		CUdfGlobalFUNC::SetGridCellReadOnly(&m_wndRainGrid,3,1);
		m_wndRainGrid.SetRowHeight(3,MFC_GRIDCTRL_HEIGHT);

		if (nRowsCount >= 5)
		{
			m_wndRainGrid.SetItemText(4,0,"累计值(mm)");
			CUdfGlobalFUNC::SetGridCellReadOnly(&m_wndRainGrid,4,0);
			CUdfGlobalFUNC::SetGridCellReadOnly(&m_wndRainGrid,4,1);
			m_wndRainGrid.SetRowHeight(4,MFC_GRIDCTRL_HEIGHT);
		}

	}
}

void CRtuAttrRainValume::OnCbnSelchangeComboRainpara()
{
	m_wndRainGrid.DeleteAllItems();
	m_wndRainGrid.Refresh();
	int nSel = m_wndRainCmb.GetCurSel();
	if (nSel != CB_ERR)
	{
		int nItemData = (int)m_wndRainCmb.GetItemData(nSel);
		CString strCaption = "";
		strCaption.Format("第 %-2d 个雨量点的雨量数据报警值及实时值",nItemData + 1);
		m_wndRainCaption.SetWindowText(strCaption);

		if (m_pSubStation)
		{
			CRainAlert_PARA_PER* pRain_PARA = m_pSubStation->GetRainAlartPara(nItemData);
			if (pRain_PARA)
			{
				FillRainGridCtrl();

				CString strText ="";
				strText.Format("%.01f",pRain_PARA->GetRainAlertValue());
				m_wndRainGrid.SetItemText(1,1,strText);

				strText.Format("%d",pRain_PARA->GetPerTime());
				m_wndRainGrid.SetItemText(2,1,strText);

				strText.Format("%.1f",pRain_PARA->GetRealTimeValue());
				m_wndRainGrid.SetItemText(3,1,strText);

				if(m_pSubStation->GetAppProVer() == APP_PROTOCOL_V2_0)
				{
					strText.Format("%.1f",pRain_PARA->GetTotalValue());
					m_wndRainGrid.SetItemText(4,1,strText);
				}
			}
		}

		m_wndRainGrid.Refresh();

	}
}

void CRtuAttrRainValume::OnBnClickedBtnRainacall()
{
	if (m_pSubStation)
	{

		if (m_pSubStation->blEnbaleTrans() == true)
		{
			SetWndCtrlEnable(FALSE);

			m_blResult = FALSE;
			CPacketFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);

			m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_64H(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)  *1000;
			SetTimer(W_TIMER_RAINALERT_CALL,nElapse,NULL);
		}
	}
}

void CRtuAttrRainValume::OnBnClickedBtnRainaset()
{
	if (m_pSubStation)
	{

		if (m_pSubStation->blEnbaleTrans() == true)
		{
			CPacketFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);

			m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_1FH(&appFrame);
		}
	}
}

void CRtuAttrRainValume::OnBnClickBtnQueryRainReal()
{
	Update();
	if( m_pSubStation)
	{

		if (m_pSubStation->blEnbaleTrans() == true)
		{
			SetWndCtrlEnable(FALSE);
			m_blResult = FALSE;

			CPacketFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息

			appFrame.ControlC._Command = C_CMD_11_RAINPARA;
			m_pSubStation->SetQueryRealDataCmd(appFrame.ControlC._Command);

			if (m_pSubStation->GetAppProVer() == APP_PROTOCOL_V1_0)
				m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_67H(&appFrame);
			else if (m_pSubStation->GetAppProVer() == APP_PROTOCOL_V2_0)
				m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_68H(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_QUERY_RAINDATA,nElapse,NULL);
		}
	}
}

void CRtuAttrRainValume::SetWndCtrlEnable(BOOL blEnable)
{
	m_wndRainPCount.EnableWindow(blEnable);
	m_wndRainCmb.EnableWindow(blEnable);
	m_wndRainGrid.EnableWindow(blEnable);

	m_btnRainACall.EnableWindow(blEnable);
	m_btnRainASet.EnableWindow(blEnable);
	m_btnRainOK.EnableWindow(blEnable);
	m_btnQueryRainReal.EnableWindow(blEnable);
}

void CRtuAttrRainValume::OnTimer(UINT_PTR nIDEvent)
{
	CString strLog= "";
	_Log_level_type LogLevel = Normal_Log_Message;

	switch (nIDEvent)
	{
	case W_TIMER_RAINALERT_CALL:
		{
			if (m_blResult == TRUE)
				strLog = "查询遥测站雨量数据报警值 成功!";
			else if (m_blResult == FALSE)
			{
				strLog = "查询遥测站雨量数据报警值 超时!";
				LogLevel = Warning_log_Message;
			}
		}
		break;
	case  W_TIMER_QUERY_RAINDATA:
		{
			if (m_blResult == TRUE)
				strLog = "查询遥测终端雨量实时数据 成功!";
			else
			{
				strLog = "查询遥测终端雨量实时数据 超时!";
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

void CRtuAttrRainValume::OnEndEditRainGrid(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	if (!pItem  || pItem->iRow <=0 || pItem->iColumn <= 0) return;
	if (!m_pSubStation) return;

	int nRainSel = m_wndRainCmb.GetCurSel();
	if (nRainSel != CB_ERR)
	{
		int nItemData = (int)m_wndRainCmb.GetItemData(nRainSel);

		CString strMessage = "";
		CString strText ="";
		CRainAlert_PARA_PER* pRain_PARA = m_pSubStation->GetRainAlartPara(nItemData);
		if (pRain_PARA)
		{
			if (pItem->iRow == 1 && pItem->iColumn == 1)
			{
				strText = CUdfGlobalFUNC::GetGridCellText(&m_wndRainGrid,1,1);
				//float RainAlert = (float)atof(strText);
				double RainAlert = atof(strText);


				if (RainAlert < 0 || RainAlert > 999.9)
				{
					strMessage.Format("输入的雨量报警值超出范围（0～999.9）,请重新输入!");
					MessageBox(strMessage,"提示",MB_ICONWARNING|MB_OK);
				}
				else
					pRain_PARA->SetRainAlertValue(RainAlert);

				
				strText.Format("%.01f",pRain_PARA->GetRainAlertValue());
				m_wndRainGrid.SetItemText(1,1,strText);


			}
			else if (pItem->iRow ==2 && pItem->iColumn == 1)
			{
				strText = CUdfGlobalFUNC::GetGridCellText(&m_wndRainGrid,2,1);
				int PerTime = atoi(strText);
				if (PerTime < 0 ||PerTime> 9999 )
				{
					strMessage.Format("输入的单位时间值超出范围（0～9999）,请重新输入!");
					MessageBox(strMessage,"提示",MB_ICONWARNING|MB_OK);
				}
				else
					pRain_PARA->SetPerTime(PerTime);

				strText.Format("%d",pRain_PARA->GetPerTime());
				m_wndRainGrid.SetItemText(2,1,strText);

			}
		}
	}
}


void CRtuAttrRainValume::OnBnClickedBtnRainok()
{
	UpdateData( TRUE);
	if (m_nRainCount >0)
	{
		CString strText = "";

		if (m_wndRainCmb.GetSafeHwnd())
		{
			m_wndRainCmb.ResetContent();

			for (int i = 0 ; i < m_nRainCount ; i ++)
			{
				strText.Format("第 %-2d 个雨量点",i + 1);
				int idx = m_wndRainCmb.AddString(strText);
				m_wndRainCmb.SetItemData(idx,i);
			}
		}

		if (m_pSubStation)
		{
			if (m_nRainCount !=  m_pSubStation->GetRainAlertParasList()->GetCount())
			{
				m_pSubStation->ClearRainAlartParaList();
				
				for ( int i = 0; i < m_nRainCount; i ++)
				{
					CRainAlert_PARA_PER* pRain_PARA = new CRainAlert_PARA_PER();
					m_pSubStation->GetRainAlertParasList()->AddTail(pRain_PARA);
				}
			}
		}

		if (m_wndRainCmb.GetCount())
		{
			m_wndRainCmb.SetCurSel(0);
			OnCbnSelchangeComboRainpara();
		}

		m_wndRainCmb.Invalidate();
	}
}


void CRtuAttrRainValume::KillAllTimers()
{
	KillTimer(W_TIMER_QUERY_RAINDATA);
	KillTimer(W_TIMER_RAINALERT_CALL);
	SetWndCtrlEnable(TRUE);
}