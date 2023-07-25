// RtuPropertyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuPropertyDlg.h"
#include "MainFrm.h"

// CRtuPropertyDlg 对话框
IMPLEMENT_DYNAMIC(CRtuPropertyDlg, CDialog)
CRtuPropertyDlg::CRtuPropertyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRtuPropertyDlg::IDD, pParent)
{
	m_pCurRtuStation    = NULL;
	m_nlogRowsMax		= 100;

	m_pTabAttrBasicInfo = NULL;
	m_pTabAttrDataShow  = NULL;
	m_pTabAttrControlCmd = NULL;
	m_pTabAttrProSet    = NULL;
	m_pTabAttrComms = NULL;
	m_pTabAttrHisQuery = NULL;
	m_pTabAttrTextMsg = NULL;
}

CRtuPropertyDlg::~CRtuPropertyDlg()
{
}

void CRtuPropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_RTUPRO, m_wndTabCtrl);
	DDX_Control(pDX, IDC_RTU_LOGLIST, m_wndListCtrl);
}


BEGIN_MESSAGE_MAP(CRtuPropertyDlg, CDialog)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_NOTIFY(NM_RCLICK, IDC_RTU_LOGLIST, &CRtuPropertyDlg::OnNMRclickRtuLoglist)
	ON_COMMAND(IDM_OP_CLEAR,&CRtuPropertyDlg::OnClearLogList)
	ON_COMMAND(IDM_OP_SAVEAS,&CRtuPropertyDlg::OnSaveAsLogList)
	ON_NOTIFY(NM_DBLCLK, IDC_RTU_LOGLIST, &CRtuPropertyDlg::OnNMDblclkRtuLoglist)
END_MESSAGE_MAP()


// CRtuPropertyDlg 消息处理程序
BOOL CRtuPropertyDlg::OnInitDialog()
{
	CDialog::OnInitDialog(); 
	//初始化TabCtrl控件相关信息
	m_pTabAttrBasicInfo = new CRtuAttrBasicInfo(&m_wndTabCtrl);
	m_pTabAttrBasicInfo->SetParentDlg(this);
	m_pTabAttrBasicInfo->Create(CRtuAttrBasicInfo::IDD, &m_wndTabCtrl);
	m_wndTabCtrl.AddTab(m_pTabAttrBasicInfo, "基本信息");


	m_pTabAttrProSet = new CRtuAttrProSet(&m_wndTabCtrl);
	m_pTabAttrProSet->SetParentDlg(this);
	m_pTabAttrProSet->Create(CRtuAttrProSet::IDD,&m_wndTabCtrl);
	m_wndTabCtrl.AddTab(m_pTabAttrProSet,"参数设置/查询");

	m_pTabAttrControlCmd = new CRtuAttrControlCmd(&m_wndTabCtrl);
	m_pTabAttrControlCmd->SetParentDlg( this);
	m_pTabAttrControlCmd->Create(CRtuAttrControlCmd::IDD,&m_wndTabCtrl);
	m_wndTabCtrl.AddTab(m_pTabAttrControlCmd,"控制命令");

	m_pTabAttrDataShow = new CRtuAttrDataShow(&m_wndTabCtrl);
	m_pTabAttrDataShow->SetParentDlg(this);
	m_pTabAttrDataShow->Create(CRtuAttrDataShow::IDD,&m_wndTabCtrl);
	m_wndTabCtrl.AddTab(m_pTabAttrDataShow,"自报数据信息");

	m_pTabAttrComms = new CRtuAttrComms(&m_wndTabCtrl);
	m_pTabAttrComms->SetParentDlg(this);
	m_pTabAttrComms->Create(CRtuAttrComms::IDD,&m_wndTabCtrl);
	m_wndTabCtrl.AddTab(m_pTabAttrComms,"通信状态");

	m_pTabAttrHisQuery = new CRtuAttrHisQuery(&m_wndTabCtrl);
	m_pTabAttrHisQuery->SetParentDlg(this);
	m_pTabAttrHisQuery->Create(CRtuAttrHisQuery::IDD,&m_wndTabCtrl);
	m_wndTabCtrl.AddTab(m_pTabAttrHisQuery,"历史记录");

	if (GETAPP()->m_GlobalOptions.m_nGSMFunctionEnable == 1)
	{
		m_pTabAttrTextMsg = new CRtuAttrTextMsg(&m_wndTabCtrl);
		m_pTabAttrTextMsg->SetParentDlg(this);
		m_pTabAttrTextMsg->Create(CRtuAttrTextMsg::IDD,&m_wndTabCtrl);
		m_wndTabCtrl.AddTab(m_pTabAttrTextMsg,"短信服务");
	}

	InitLogListCtrl();
	SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_RTUPRO),TRUE);
	return TRUE;  
}

//销毁相关的窗口信息
void CRtuPropertyDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if (m_pTabAttrBasicInfo)
		delete m_pTabAttrBasicInfo;

	if (m_pTabAttrDataShow)
		delete m_pTabAttrDataShow;

	if (m_pTabAttrProSet)
		delete m_pTabAttrProSet;

	if (m_pTabAttrControlCmd)
		delete m_pTabAttrControlCmd;

	if (m_pTabAttrComms)
		delete m_pTabAttrComms;

	if (m_pTabAttrHisQuery)
		delete m_pTabAttrHisQuery;

	if (m_pTabAttrTextMsg)
		delete m_pTabAttrTextMsg;

}

void CRtuPropertyDlg::UpdateCaption()
{
	if (m_pCurRtuStation)
	{
		CString strCommState="";
		if (m_pCurRtuStation->GetCommState() ==Connected_State)
			strCommState = "通信正常";
		else if ( m_pCurRtuStation->GetCommState() == Disconnect_State)
			strCommState = "通信断开";
		else
			strCommState = "未知";

		CString strCaption = "";
		strCaption.Format("[%s] 属性    [通信状态:%s ]",m_pCurRtuStation->GetName(),
			strCommState);
		SetWindowText(strCaption);
	}
}

void CRtuPropertyDlg::Update()
{

	UpdateCaption();

	if (m_pTabAttrBasicInfo && m_pTabAttrBasicInfo->GetSafeHwnd())
		m_pTabAttrBasicInfo->Update();

	if (m_pTabAttrProSet && m_pTabAttrProSet->GetSafeHwnd())
		m_pTabAttrProSet->Update();

	if (m_pTabAttrControlCmd && m_pTabAttrControlCmd->GetSafeHwnd())
		m_pTabAttrControlCmd->Update();
	
	if (m_pTabAttrDataShow && m_pTabAttrDataShow->GetSafeHwnd())
		m_pTabAttrDataShow->Update();

	if (m_pTabAttrComms && m_pTabAttrComms->GetSafeHwnd())
		m_pTabAttrComms->Update();

	if (m_pTabAttrHisQuery && m_pTabAttrHisQuery->GetSafeHwnd())
		m_pTabAttrHisQuery->Update();

	if (m_pTabAttrTextMsg && m_pTabAttrTextMsg->GetSafeHwnd())
		m_pTabAttrTextMsg->Update();
}

void CRtuPropertyDlg::OnClose()
{
	ShowWindow(SW_HIDE);
}

void CRtuPropertyDlg::Show(int nCmdShow,CWnd* pParent /* = NULL */)
{
	Update();
	ShowWindow(nCmdShow);
}

void CRtuPropertyDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
}

void CRtuPropertyDlg::SetRtuStation(CRtuStation* pRtu)
{
	if (pRtu != m_pCurRtuStation)
	{
		m_pCurRtuStation = pRtu;
		
		if (m_pTabAttrBasicInfo && m_pTabAttrBasicInfo->GetSafeHwnd())
			m_pTabAttrBasicInfo->KillAllTimers();

		if (m_pTabAttrDataShow && m_pTabAttrDataShow->GetSafeHwnd())
		{
			if (m_pTabAttrDataShow->m_wndGridCtrl.GetSafeHwnd())
			{
				m_pTabAttrDataShow->InitGridHeader();
			}

			if (m_pCurRtuStation)
			{
				CList<Log_Item*>* pList = m_pCurRtuStation->GetDataList();
				POSITION rPos = pList->GetHeadPosition();
				while (rPos)
				{
					Log_Item* pItem = pList->GetNext(rPos);
					if (pItem)
					{
						m_pTabAttrDataShow->AddMessageToWindow(pItem->m_Message,&pItem->m_Time);
					}
				}
			}
		}

		if (m_pTabAttrComms && m_pTabAttrComms->GetSafeHwnd())
		{
			if (m_pTabAttrComms->m_wndCommsList.GetSafeHwnd())
				m_pTabAttrComms->m_wndCommsList.DeleteAllItems();

			if (m_pCurRtuStation)
			{
				CList<Log_Item*>* pList = m_pCurRtuStation->GetCommList();
				POSITION rPos = pList->GetHeadPosition();
				while (rPos != NULL)
				{
					Log_Item* pItem = pList->GetNext(rPos);
					if (pItem)
						m_pTabAttrComms->AddMessageToWindow(pItem->m_Message,pItem->m_Level,pItem->m_Time);
				}
			}
		}

		if (m_pTabAttrHisQuery && m_pTabAttrHisQuery->GetSafeHwnd())
			m_pTabAttrHisQuery->InitGridHeader();

		if (m_pTabAttrControlCmd && m_pTabAttrControlCmd->GetSafeHwnd())
			m_pTabAttrControlCmd->KillAllTimers();

		if (m_pTabAttrTextMsg && m_pTabAttrTextMsg->GetSafeHwnd())
			m_pTabAttrTextMsg->KillAllTimers();

		if (GetTab_AttrWorkMode() && GetTab_AttrWorkMode()->GetSafeHwnd())
			GetTab_AttrWorkMode()->KillAllTimers();

		if (GetTab_AttrAutoRpSet() && GetTab_AttrAutoRpSet()->GetSafeHwnd())
			GetTab_AttrAutoRpSet()->KillAllTimers();

		if (GetTab_AttrAlertAnalog() && GetTab_AttrAlertAnalog()->GetSafeHwnd())
			GetTab_AttrAlertAnalog()->KillAllTimers();
		
		if (GetTab_AttrDisplacement() && GetTab_AttrDisplacement()->GetSafeHwnd())
			GetTab_AttrDisplacement()->KillAllTimers();
		
		if (GetTab_AttrTemperature() && GetTab_AttrTemperature()->GetSafeHwnd())
			GetTab_AttrTemperature()->KillAllTimers();

		if (GetTab_AttrWaterQuality() && GetTab_AttrWaterQuality()->GetSafeHwnd())
			GetTab_AttrWaterQuality()->KillAllTimers();

		if (GetTab_AttrAlarmState() && GetTab_AttrAlarmState()->GetSafeHwnd())
			GetTab_AttrAlarmState()->KillAllTimers();

		if (GetTab_AttrRainValume() && GetTab_AttrRainValume()->GetSafeHwnd())
			GetTab_AttrRainValume()->KillAllTimers();

		if (GetTab_AttrWaterValume() && GetTab_AttrWaterValume()->GetSafeHwnd())
			GetTab_AttrWaterValume()->KillAllTimers();

		if (GetTab_AttrWaterLevel() && GetTab_AttrWaterLevel()->GetSafeHwnd())
			GetTab_AttrWaterLevel()->KillAllTimers();

		if (GetTab_AttrWaterPress() && GetTab_AttrWaterPress()->GetSafeHwnd())
			GetTab_AttrWaterPress()->KillAllTimers();

		if (GetTab_AttrFlow() && GetTab_AttrFlow()->GetSafeHwnd())
			GetTab_AttrFlow()->KillAllTimers();

		if (GetTab_AttrQueryEvent() && GetTab_AttrQueryEvent()->GetSafeHwnd())
			GetTab_AttrQueryEvent()->KillAllTimers();

		if (GetTab_AttrDayHisQuery() && GetTab_AttrDayHisQuery()->GetSafeHwnd())
			GetTab_AttrDayHisQuery()->KillAllTimers();

		if (m_wndListCtrl.GetSafeHwnd())
		{
			m_wndListCtrl.DeleteAllItems();
		}

		//添加日志信息显示
		if (m_pCurRtuStation)
		{
			CList<Log_Item*> *pLst = m_pCurRtuStation->GetLogList();
			if (pLst)
			{
				POSITION rPos = pLst->GetHeadPosition();
				while(rPos)
				{
					Log_Item* pItem = pLst->GetNext(rPos);
					if (pItem)
					{
						AddLog(pItem->m_Message,&pItem->m_Time,pItem->m_Level);
					}
				}
			}
		}
	}
}

void CRtuPropertyDlg::InitLogListCtrl()
{
	if (m_wndListCtrl.GetSafeHwnd())
	{
		DWORD dw = m_wndListCtrl.GetExtendedStyle();
		dw |= LVS_EX_FULLROWSELECT;
		m_wndListCtrl.SetExtendedStyle(dw);

		m_wndListCtrl.DeleteAllItems();
		m_wndListCtrl.InsertColumn(0,"时间",LVCFMT_LEFT,140);
		m_wndListCtrl.InsertColumn(1,"日志信息",LVCFMT_LEFT,560);
	}
}


void CRtuPropertyDlg::AddLog(CString strLogMessage,PSYSTEMTIME logTime /*= NULL*/,_Log_level_type logLevel /*= Normal_Log_Message*/)
{
	strLogMessage.Trim();
	if (strLogMessage.IsEmpty()) return;

	CString strTime = "";
	
	SYSTEMTIME st;
	if (logTime == NULL)
		GetLocalTime(&st);
	else 
	{
		st.wDay				= logTime->wDay;
		st.wDayOfWeek		= logTime->wDayOfWeek;
		st.wHour			= logTime->wHour;
		st.wMilliseconds	= logTime->wMilliseconds;
		st.wMinute			= logTime->wMinute;
		st.wMonth			= logTime->wMonth;
		st.wSecond			= logTime->wSecond;
		st.wYear			= logTime->wYear;
	}

	strTime.Format("%04d-%02d-%02d %02d:%02d:%02d.%03d",
		st.wYear,
		st.wMonth,
		st.wDay,
		st.wHour,
		st.wMinute,
		st.wSecond,
		st.wMilliseconds);
	AddLog(strLogMessage,strTime,logLevel);
}

void CRtuPropertyDlg::AddLog(CString strLogMessage,CString strLogTime,_Log_level_type LogLevel)
{
	if (m_wndListCtrl.GetSafeHwnd())
	{
		int nInsertIdx = m_wndListCtrl.InsertItem(0,strLogTime);
		if (nInsertIdx != -1)
		{
			m_wndListCtrl.SetItemText(nInsertIdx,1,strLogMessage);
			m_wndListCtrl.SetItemData(nInsertIdx,(DWORD_PTR)LogLevel);
		}

		if (m_wndListCtrl.GetItemCount() >  m_nlogRowsMax)
			m_wndListCtrl.DeleteItem(m_wndListCtrl.GetItemCount() -1);
	}
}

//弹出右键菜单
void CRtuPropertyDlg::OnNMRclickRtuLoglist(NMHDR *pNMHDR, LRESULT *pResult)
{
	CMenu menu;
	menu.LoadMenu(IDR_MENU_POP);
	CPoint cursorPos;
	GetCursorPos(&cursorPos);

	if (menu.GetSafeHmenu())
	{
		CMenu* popupMenu = menu.GetSubMenu(10);
		if (popupMenu && popupMenu->GetSafeHmenu())
			popupMenu->TrackPopupMenu(TPM_LEFTALIGN,cursorPos.x,cursorPos.y,this);

	}
	*pResult = 0;
}


void CRtuPropertyDlg::OnClearLogList()
{
	int ret = MessageBox("确定清空所有信息?","提示",MB_OKCANCEL|MB_ICONQUESTION);
	if (ret == IDCANCEL)
		return;

	if (m_wndListCtrl.GetSafeHwnd())
	{
		m_wndListCtrl.DeleteAllItems();
	}

	if (m_pCurRtuStation)
	{
		m_pCurRtuStation->ClearLogList();
	}
}

//导出日志信息存储到其他文件
void CRtuPropertyDlg::OnSaveAsLogList()
{
	if (m_wndListCtrl.GetSafeHwnd()  && m_wndListCtrl.GetItemCount() > 0)
	{
		CFileDialog fileDlg(FALSE, _T("txt"), _T("日志信息"),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||"),this);
		if (fileDlg.DoModal() == IDOK)
		{
			CString sFilename = fileDlg.GetPathName();

			try
			{
				CStdioFile file(sFilename,CFile::modeCreate|CFile::modeWrite);

				int nItemCount = m_wndListCtrl.GetItemCount();
				for (int idx = 0; idx < nItemCount; idx ++)
				{
					CString strDatetime="", strMessages = "";
					strDatetime	= m_wndListCtrl.GetItemText(idx,0);
					strMessages	= m_wndListCtrl.GetItemText(idx,1);


					CString strWrite ="";
					strWrite.Format("%s,%s\n",
									strDatetime,
									strMessages);

					file.WriteString(strWrite);
				}
				file.Flush();

				m_wndListCtrl.Invalidate();
			}
			catch (CFileException* e)
			{
				e->ReportError(MB_OK);
				e->Delete();
				return;
			}
		}
	}
}


CRtuAttrAutoRpSet* CRtuPropertyDlg::GetTab_AttrAutoRpSet()
{
	if (m_pTabAttrProSet && m_pTabAttrProSet->GetSafeHwnd())
		return m_pTabAttrProSet->GetTab_AttrAutoRpSet();

	return NULL;
}

CRtuAttrWorkMode* CRtuPropertyDlg::GetTab_AttrWorkMode()
{
	if (m_pTabAttrProSet && m_pTabAttrProSet->GetSafeHwnd())
		return m_pTabAttrProSet->GetTab_AttrWorkMode();

	return NULL;
}

CRtuAttrAlertAnalog* CRtuPropertyDlg::GetTab_AttrAlertAnalog()
{
	if (m_pTabAttrProSet && m_pTabAttrProSet->GetSafeHwnd())
		return m_pTabAttrProSet->GetTab_AttrAlertAnalog();

	return NULL;
}

CRtuAttrDisplacement* CRtuPropertyDlg::GetTab_AttrDisplacement()
{
	if (m_pTabAttrProSet && m_pTabAttrProSet->GetSafeHwnd())
		return m_pTabAttrProSet->GetTab_AttrDisplacement();

	return NULL;
}

CRtuAttrTemperature* CRtuPropertyDlg::GetTab_AttrTemperature()
{
	if (m_pTabAttrProSet && m_pTabAttrProSet->GetSafeHwnd())
		return m_pTabAttrProSet->GetTab_AttrTemperature();

	return NULL;
}

CRtuAttrWaterQuality* CRtuPropertyDlg::GetTab_AttrWaterQuality()
{
	if (m_pTabAttrProSet && m_pTabAttrProSet->GetSafeHwnd())
		return m_pTabAttrProSet->GetTab_AttrWaterQuality();

	return NULL;
}

CRtuAttrAlarmState* CRtuPropertyDlg::GetTab_AttrAlarmState()
{
	if (m_pTabAttrProSet && m_pTabAttrProSet->GetSafeHwnd())
		return m_pTabAttrProSet->GetTab_AttrAlarmState();

	return NULL;
}

CRtuAttrRainValume* CRtuPropertyDlg::GetTab_AttrRainValume()
{
	if (m_pTabAttrProSet && m_pTabAttrProSet->GetSafeHwnd())
		return m_pTabAttrProSet->GetTab_AttrRainValume();

	return NULL;
}

CRtuAttrWaterValume* CRtuPropertyDlg::GetTab_AttrWaterValume()
{
	if (m_pTabAttrProSet && m_pTabAttrProSet->GetSafeHwnd())
		return m_pTabAttrProSet->GetTab_AttrWaterValume();

	return NULL;
}

CRtuAttrWaterLevel*  CRtuPropertyDlg::GetTab_AttrWaterLevel()
{
	if (m_pTabAttrProSet && m_pTabAttrProSet->GetSafeHwnd())
		return m_pTabAttrProSet->GetTab_AttrWaterLevel();

	return NULL;
}

CRtuAttrWaterPress* CRtuPropertyDlg::GetTab_AttrWaterPress()
{
	if (m_pTabAttrProSet && m_pTabAttrProSet->GetSafeHwnd())
		return m_pTabAttrProSet->GetTab_AttrWaterPress();

	return NULL;
}

CRtuAttrFlow* CRtuPropertyDlg::GetTab_AttrFlow()
{
	if (m_pTabAttrProSet && m_pTabAttrProSet->GetSafeHwnd())
		return m_pTabAttrProSet->GetTab_AttrFlow();

	return NULL;
}

CRtuAttrQueryEvent* CRtuPropertyDlg::GetTab_AttrQueryEvent()
{
	if (m_pTabAttrProSet && m_pTabAttrProSet->GetSafeHwnd())
		return m_pTabAttrProSet->GetTab_AttrQueryEvent();
	return NULL;
}

CRtuAttrDayHisQuery* CRtuPropertyDlg::GetTab_AttrDayHisQuery()
{
	if (m_pTabAttrProSet && m_pTabAttrProSet->GetSafeHwnd())
		return m_pTabAttrProSet->GetTab_AttrDayHisQuery();
	return NULL;
}

CRtuAttrPulse* CRtuPropertyDlg::GetTab_AttrPulse()
{
	if (m_pTabAttrProSet && m_pTabAttrProSet->GetSafeHwnd())
		return m_pTabAttrProSet->GetTab_AttrPulse();
	return NULL;
}

void CRtuPropertyDlg::OnNMDblclkRtuLoglist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pItem = ( LPNMITEMACTIVATE )pNMHDR;
	if (pItem->iItem != -1)
	{
		CString strDate = m_wndListCtrl.GetItemText(pItem->iItem,0);
		CString strLog = m_wndListCtrl.GetItemText(pItem->iItem,1);

		SHOW_LOG_MESSAGE(strDate,strLog);
	}

	*pResult = 0;
}
