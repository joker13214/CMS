// RtuAttrAlertAnalog.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuAttrAlertAnalog.h"
#include "RtuStation.h"
#include "RtuPropertyDlg.h"

// CRtuAttrAlertAnalog 对话框
#define  W_TIMER_ANALOGLIMIT_CALL		0x01      //模拟量上下限值查询Timer
#define  W_TIMER_QUERY_ANALOGDATA       0x02      //模拟量实时值查询




IMPLEMENT_DYNAMIC(CRtuAttrAlertAnalog, CDialog)

CRtuAttrAlertAnalog::CRtuAttrAlertAnalog(CWnd* pParent /*=NULL*/)
	: CDialog(CRtuAttrAlertAnalog::IDD, pParent)
	, m_nAnalogCount(0)
{
	m_pSubStation = NULL;
	m_pParentDlg =  NULL;
	m_blResult = FALSE;
}

CRtuAttrAlertAnalog::~CRtuAttrAlertAnalog()
{
}

void CRtuAttrAlertAnalog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ANALOGCOUNT, m_wndAnalogPCount);
	DDX_Control(pDX, IDC_COMBO_ANALOHPARA, m_wndAnalogCmb);
	DDX_Control(pDX, IDC_BTN_ANALOGLCALL, m_btnAnalogLCall);
	DDX_Control(pDX, IDC_BTN_ANALOGLSET, m_btnAnalogLSet);
	DDX_Control(pDX, IDC_STATIC_ANALOGCAPTION, m_wndAnalogCaption);
	DDX_Control(pDX, IDC_BTN_ANALOGOK, m_btnAnalogOK);
	DDX_Text(pDX, IDC_EDIT_ANALOGCOUNT, m_nAnalogCount);
	DDX_Control(pDX, IDC_BTN_QUERYAREAL, m_btnQueryAReal);
}


BEGIN_MESSAGE_MAP(CRtuAttrAlertAnalog, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_ANALOHPARA, &CRtuAttrAlertAnalog::OnCbnSelchangeComboAnalohpara)
	ON_BN_CLICKED(IDC_BTN_ANALOGLCALL, &CRtuAttrAlertAnalog::OnBnClickedBtnAnaloglcall)
	ON_BN_CLICKED(IDC_BTN_ANALOGLSET, &CRtuAttrAlertAnalog::OnBnClickedBtnAnaloglset)
	ON_BN_CLICKED(IDC_BTN_ANALOGOK, &CRtuAttrAlertAnalog::OnBnClickedBtnAnalogok)
	ON_BN_CLICKED(IDC_BTN_QUERYAREAL,&CRtuAttrAlertAnalog::OnBnClickBtnQueryAReal)
	ON_NOTIFY(GVN_ENDLABELEDIT,IDC_ANALOGLIMIT_GRID,&CRtuAttrAlertAnalog::OnEndEditAnalogGrid)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CRtuAttrAlertAnalog 消息处理程序
BOOL CRtuAttrAlertAnalog::OnInitDialog()
{
	CDialog::OnInitDialog();

	CWnd* pWnd = NULL;
	CRect rect1, rect2;
	int captionHeight = 0; // ::GetSystemMetrics(SM_CYCAPTION);
	int cxframe = GetSystemMetrics(SM_CXFRAME);
	int cyframe = GetSystemMetrics(SM_CYFRAME);
	this->GetWindowRect(&rect2);

	pWnd = GetDlgItem(IDC_STATIC_ANALOGPRAR);
	if (pWnd)
		pWnd->GetWindowRect(&rect1);
	if (!m_wndAnalogGrid.Create(CRect(rect1.left - rect2.left - cxframe, 
		rect1.top-rect2.top - cyframe-captionHeight, 
		rect1.left + rect1.Width() - rect2.left, 
		rect1.top + rect1.Height() - rect2.top - captionHeight), 
		this, IDC_ANALOGLIMIT_GRID,WS_CHILD|WS_VISIBLE))
	{
		TRACE("Failed to create threads  grid!\n");
		return FALSE;
	}

	m_wndAnalogGrid.SetBkColor(RGB(255,255,255));
	m_wndAnalogGrid.SetListMode(TRUE);
	m_wndAnalogGrid.SetSingleColSelection(TRUE);
	m_wndAnalogGrid.SetSingleRowSelection(TRUE);


	return TRUE;  
}


void CRtuAttrAlertAnalog::FillAnalogGridCtrl()
{
	if (m_wndAnalogGrid.GetSafeHwnd())
	{
		m_wndAnalogGrid.DeleteAllItems();
		m_wndAnalogGrid.SetRowCount(4);
		m_wndAnalogGrid.SetColumnCount(2);
		m_wndAnalogGrid.SetFixedRowCount(1);

		CString strHeaders[]={_T("选项"),_T("值")};
		int colWidth[] = {110,80};

		m_wndAnalogGrid.SetItemText(0,0,strHeaders[0]);
		m_wndAnalogGrid.SetItemText(0,1,strHeaders[1]);

		m_wndAnalogGrid.SetColumnWidth(0,colWidth[0]);
		m_wndAnalogGrid.SetColumnWidth(1,colWidth[1]);
		m_wndAnalogGrid.SetRowHeight(0,MFC_GRIDCTRL_HEIGHT);

		m_wndAnalogGrid.SetItemText(1,0,"上限值(0~99999)");
		CUdfGlobalFUNC::SetGridCellReadOnly(&m_wndAnalogGrid,1,0);
		CUdfGlobalFUNC::SetGridCellNormal(&m_wndAnalogGrid,1,1);
		m_wndAnalogGrid.SetRowHeight(0,MFC_GRIDCTRL_HEIGHT);

		m_wndAnalogGrid.SetItemText(2,0,"下限值(0~99999)");
		CUdfGlobalFUNC::SetGridCellReadOnly(&m_wndAnalogGrid,2,0);
		CUdfGlobalFUNC::SetGridCellNormal(&m_wndAnalogGrid,2,1);
		m_wndAnalogGrid.SetRowHeight(2,MFC_GRIDCTRL_HEIGHT);

		m_wndAnalogGrid.SetItemText(3,0,"实时值");
		CUdfGlobalFUNC::SetGridCellReadOnly(&m_wndAnalogGrid,3,0);
		CUdfGlobalFUNC::SetGridCellReadOnly(&m_wndAnalogGrid,3,1);
		m_wndAnalogGrid.SetRowHeight(3,MFC_GRIDCTRL_HEIGHT);

	}
}


void CRtuAttrAlertAnalog::OnCbnSelchangeComboAnalohpara()
{
	m_wndAnalogGrid.DeleteAllItems();
	m_wndAnalogGrid.Refresh();
	int nSel = m_wndAnalogCmb.GetCurSel();
	if (nSel != CB_ERR)
	{
		int nItemData = (int)m_wndAnalogCmb.GetItemData(nSel);
		CString strCaption = "";
		strCaption.Format("第 %-2d 个模拟量点的模拟量的详细信息",nItemData + 1);
		m_wndAnalogCaption.SetWindowText(strCaption);

		if (m_pSubStation)
		{
			CAnalogLimit_PARA_PER* pAnalog_PARA = m_pSubStation->GetAnalogLimitPara(nItemData);
			if (pAnalog_PARA)
			{
				FillAnalogGridCtrl();

				CString strText ="";
				strText.Format("%d",pAnalog_PARA->GetAnalogUpLimit());

				m_wndAnalogGrid.SetItemText(1,1,strText);

				strText.Format("%d",pAnalog_PARA->GetAnalogDownLimit());
				m_wndAnalogGrid.SetItemText(2,1,strText);

				strText.Format("%d",pAnalog_PARA->GetRealTimeValue());
				m_wndAnalogGrid.SetItemText(3,1,strText);
			}
		}

		m_wndAnalogGrid.Refresh();
	}
}



void CRtuAttrAlertAnalog::OnBnClickedBtnAnaloglcall()
{
	if (m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{
			SetWndCtrlEnable(FALSE);

			m_blResult = FALSE;

			CPacketFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);

			m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_65H(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)  *1000;
			SetTimer(W_TIMER_ANALOGLIMIT_CALL,nElapse,NULL);
		}
	}
}

void CRtuAttrAlertAnalog::OnBnClickedBtnAnaloglset()
{
	if (m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{
			CPacketFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);

			m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_20H(&appFrame);
		}
	}
}


void CRtuAttrAlertAnalog::OnBnClickedBtnAnalogok()
{
	UpdateData( TRUE);
	if (m_nAnalogCount >0)
	{
		CString strText = "";

		if (m_wndAnalogCmb.GetSafeHwnd())
		{
			m_wndAnalogCmb.ResetContent();

			for (int i = 0 ; i < m_nAnalogCount ; i ++)
			{
				strText.Format("第 %-2d 个模拟量点",i + 1);
				int idx = m_wndAnalogCmb.AddString(strText);
				m_wndAnalogCmb.SetItemData(idx,i);
			}
		}

		if (m_pSubStation != NULL)
		{
			if (m_nAnalogCount != m_pSubStation->GetAnalogLimitParasList()->GetCount())
			{
				m_pSubStation->ClearAnalogLimitParaList();

				for (int i= 0; i <m_nAnalogCount; i ++)
				{
					CAnalogLimit_PARA_PER* pAnalog_PARA = new CAnalogLimit_PARA_PER();
					m_pSubStation->GetAnalogLimitParasList()->AddTail(pAnalog_PARA);
				}
			}
		}

		if (m_wndAnalogCmb.GetCount())
		{
			m_wndAnalogCmb.SetCurSel(0);
			OnCbnSelchangeComboAnalohpara();
		}

		m_wndAnalogCmb.Invalidate();
	}
}

void CRtuAttrAlertAnalog::OnBnClickBtnQueryAReal()
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

			appFrame.ControlC._Command = C_CMD_12_MONIPARA;
			m_pSubStation->SetQueryRealDataCmd(appFrame.ControlC._Command);

			if (m_pSubStation->GetAppProVer() == APP_PROTOCOL_V1_0)
				m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_67H(&appFrame);
			else if (m_pSubStation->GetAppProVer() == APP_PROTOCOL_V2_0)
				m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_68H(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_QUERY_ANALOGDATA,nElapse,NULL);
		}
	}
}

void CRtuAttrAlertAnalog::OnEndEditAnalogGrid(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct; 
	if (!pItem  || pItem->iRow <=0 || pItem->iColumn <= 0) return;
	if (!m_pSubStation) return;

	int nAnalogSel = m_wndAnalogCmb.GetCurSel();
	if (nAnalogSel != CB_ERR)
	{
		int nItemData = (int)m_wndAnalogCmb.GetItemData(nAnalogSel);

		CString strMessage = "";
		CString strText ="";

		CAnalogLimit_PARA_PER* pAnalog_PARA = m_pSubStation->GetAnalogLimitPara(nItemData);
		if (pAnalog_PARA)
		{
			if (pItem->iRow == 1 && pItem->iColumn == 1)
			{
				strText = CUdfGlobalFUNC::GetGridCellText(&m_wndAnalogGrid,1,1);
				int UpLimit = atoi (strText);
				if (UpLimit < 0 || UpLimit > 99999)
				{
					strMessage.Format("输入的上限值超出范围(0~99999),请重新输入!");
					MessageBox(strMessage,"提示",MB_ICONWARNING|MB_OK);
				}
				else
					pAnalog_PARA->SetAnalogUpLimit(UpLimit);


				strText.Format("%d",pAnalog_PARA->GetAnalogUpLimit());
				m_wndAnalogGrid.SetItemText(1,1,strText);
			}
			else if (pItem->iRow ==2 && pItem->iColumn == 1)
			{
				CString strText = CUdfGlobalFUNC::GetGridCellText(&m_wndAnalogGrid,2,1);
				int DownLimit = atoi(strText);
				if (DownLimit < 0 || DownLimit > 99999)
				{
					strMessage.Format("输入的下限值超出范围(0~99999),请重新输入!");
					MessageBox(strMessage,"提示",MB_ICONWARNING|MB_OK);
				}
				else
					pAnalog_PARA->SetAnalogDownLimit(atol(strText));

				strText.Format("%d",pAnalog_PARA->GetAnalogDownLimit());
				m_wndAnalogGrid.SetItemText(2,1,strText);
			}
		}
	}
}

void CRtuAttrAlertAnalog::Update()
{
	if (!m_pParentDlg) return;
	if (!m_pParentDlg->GetRtuStation()) return;

	m_pSubStation = m_pParentDlg->GetRtuStation();

	if (m_wndAnalogGrid.GetSafeHwnd())
	{
		m_wndAnalogGrid.DeleteAllItems();
		m_wndAnalogGrid.Refresh();
	}

	if (m_pSubStation)
	{
		CString strText ="";
		int idx = 0;

		int nAnalogCount = (int)m_pSubStation->GetAnalogLimitParasList()->GetCount();
		strText.Format("%d",nAnalogCount);
		m_wndAnalogPCount.SetWindowText(strText);

		if (m_wndAnalogCmb.GetSafeHwnd())
		{
			m_wndAnalogCmb.ResetContent();

			for (idx =0; idx < nAnalogCount; idx ++)
			{
				strText.Format("第 %-2d 个模拟量点",idx + 1);

				int Index = m_wndAnalogCmb.AddString(strText);
				m_wndAnalogCmb.SetItemData(Index,idx);
			}

			if (m_wndAnalogCmb.GetCount())
			{
				m_wndAnalogCmb.SetCurSel(0);
			}
			else
			{
				m_wndAnalogCmb.SetCurSel(-1);
			}
			OnCbnSelchangeComboAnalohpara();
		}
	}
}


void CRtuAttrAlertAnalog::OnTimer(UINT_PTR nIDEvent)
{
	CString strLog= "";
	_Log_level_type LogLevel = Normal_Log_Message;

	switch (nIDEvent)
	{

	case W_TIMER_ANALOGLIMIT_CALL:
		{
			if (m_blResult == TRUE)
				strLog = "查询遥测站模拟量数据上下限 成功!";
			else if (m_blResult == FALSE)
			{
				strLog = "查询遥测站模拟量数据上下限 超时!";
				LogLevel = Warning_log_Message;
			}
		}
		break;
	case W_TIMER_QUERY_ANALOGDATA:
		{
			if (m_blResult == TRUE)
				strLog = "查询遥测站模拟量实时数据 成功!";
			else if (m_blResult == FALSE)
			{
				strLog = "查询遥测站模拟量实时数据 超时!";
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

void CRtuAttrAlertAnalog::SetWndCtrlEnable(BOOL blEnable)
{
	m_wndAnalogPCount.EnableWindow(blEnable);
	m_wndAnalogCmb.EnableWindow(blEnable);
	m_wndAnalogGrid.EnableWindow(blEnable);

	m_btnAnalogLCall.EnableWindow(blEnable);
	m_btnAnalogLSet.EnableWindow(blEnable);
	m_btnAnalogOK.EnableWindow(blEnable);
	m_btnQueryAReal.EnableWindow(blEnable);
}

void CRtuAttrAlertAnalog::UpdateDlg(int attrType,BOOL blResult)
{
	m_blResult = blResult;
	switch (attrType)
	{
	case  rtu_attr_analogminmax_call:
		SendMessage(WM_TIMER,W_TIMER_ANALOGLIMIT_CALL,NULL);
		break;
	case rtu_attr_analog_data_call:
		SendMessage(WM_TIMER,W_TIMER_QUERY_ANALOGDATA,NULL);
		break;
	}
}


void CRtuAttrAlertAnalog::KillAllTimers()
{
	KillTimer(W_TIMER_QUERY_ANALOGDATA);
	KillTimer(W_TIMER_ANALOGLIMIT_CALL);
	SetWndCtrlEnable(TRUE);
}