// UdfLogFrame.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "UdfLogFrame.h"
#include "MainFrm.h"


static UINT logframe_indicators[] =
{
	/*
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
	*/
	IDM_LOG_SYSTEM,
	//IDM_LOG_SYSTEM,
};

// CUdfLogFrame

IMPLEMENT_DYNCREATE(CUdfLogFrame, CFrameWnd)
IMPLEMENT_MENUXP(CUdfLogFrame, CFrameWnd);

CUdfLogFrame::CUdfLogFrame()
{

	m_pLeftView = NULL;
	m_pRightView = NULL;
	EnableAutomation();
}

CUdfLogFrame::~CUdfLogFrame()
{

}

void CUdfLogFrame::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。基类将自动
	// 删除该对象。在调用该基类之前，请添加您的
	// 对象所需的附加清除代码。

	CFrameWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CUdfLogFrame, CFrameWnd)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_MENUXP_MESSAGES()
	ON_WM_DESTROY()
	ON_WM_ACTIVATE()
	ON_COMMAND(IDM_LOGFRAME_EXIT,&CUdfLogFrame::OnLogFrameExit)
	ON_COMMAND(IDM_LOG_QUERY,&CUdfLogFrame::OnCmdLogQuery)
	ON_COMMAND(IDM_LOG_EXPORT,&CUdfLogFrame::OnCmdLogExport)
	ON_COMMAND(IDM_LOG_DELALL,&CUdfLogFrame::OnCmdLogDeleteAll)
	ON_COMMAND(IDM_LOG_DEL1MONTH,&CUdfLogFrame::OnCmdLogDelete1MonthPre)
	ON_COMMAND(IDM_LOG_DEL2MONTH,&CUdfLogFrame::OnCmdLogDelete2MonthPre)
	ON_COMMAND(IDM_LOG_REFRESH,&CUdfLogFrame::OnCmdLogRefresh)
	ON_COMMAND(IDM_LOGOPT_CFG,&CUdfLogFrame::OnLogOptCfg)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CUdfLogFrame, CFrameWnd)
END_DISPATCH_MAP()

// 注意: 我们添加 IID_IUdfLogFrame 支持
//  以支持来自 VBA 的类型安全绑定。此 IID 必须同附加到 .IDL 文件中的
//  调度接口的 GUID 匹配。

// {1E057176-AB92-4355-BD41-8B504C406E5A}
static const IID IID_IUdfLogFrame =
{ 0x1E057176, 0xAB92, 0x4355, { 0xBD, 0x41, 0x8B, 0x50, 0x4C, 0x40, 0x6E, 0x5A } };

BEGIN_INTERFACE_MAP(CUdfLogFrame, CFrameWnd)
	INTERFACE_PART(CUdfLogFrame, IID_IUdfLogFrame, Dispatch)
END_INTERFACE_MAP()


// CUdfLogFrame 消息处理程序

BOOL CUdfLogFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (!m_wndSplitter.CreateStatic(this,1,2))
	{
		return FALSE;
	}
	CRect rc;
	GetClientRect(&rc);
	if (!m_wndSplitter.CreateView(0,0,RUNTIME_CLASS(CUdfLogLeftView),CSize(160,rc.Height()),pContext))
	{
		return FALSE;
	}

	if (!m_wndSplitter.CreateView(0,1,RUNTIME_CLASS(CUdfLogRightView),CSize(0,0),pContext))
	{
		return FALSE;
	}

	m_pLeftView = (CUdfLogLeftView*)m_wndSplitter.GetPane(0,0);
	if(m_pLeftView && m_pLeftView->GetSafeHwnd())
	{
		m_pLeftView->InitTreeView();
		m_pLeftView->SetFrameWnd(this);
	}
	m_pRightView = (CUdfLogRightView*)m_wndSplitter.GetPane(0,1);
	if (m_pRightView->GetSafeHwnd())
	{
		m_pRightView->SetFrameWnd(this);
	}

	SetActiveView(m_pRightView);

	return CFrameWnd::OnCreateClient(lpcs, pContext);
}

void CUdfLogFrame::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//CFrameWnd::OnClose();
	ShowWindow(SW_HIDE);
}

void CUdfLogFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (nType ==SIZE_MINIMIZED)
	{
		ShowWindow(SW_HIDE);

	}
}

int CUdfLogFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(logframe_indicators,
		sizeof(logframe_indicators)/sizeof(UINT)))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}

	m_wndStatusBar.SetPaneInfo(0,IDM_LOG_SYSTEM,SBPS_NOBORDERS|SBPS_STRETCH,200);
    m_wndStatusBar.SetPaneText(0,"");
	
	//Show Destop bar
	ModifyStyleEx(WS_EX_APPWINDOW,0); 
	

	HICON hicon;
	hicon = AfxGetApp()->LoadIcon(IDI_ICON_LOGINFO);
	SetIcon(hicon,TRUE);

	CMenuXP::SetXPLookNFeel(this);

	m_wndMenu.LoadMenu(IDR_MENU_LOGFRAME);
	SetMenu(&m_wndMenu);

	
	return 0;
}

void CUdfLogFrame::OnDestroy()
{
	if (m_pLeftView && m_pLeftView->GetSafeHwnd())
		m_pLeftView->DestroyWindow();
	if (m_pRightView && m_pRightView->GetSafeHwnd())
		m_pRightView->DestroyWindow();

	CFrameWnd::OnDestroy();
}


void CUdfLogFrame::SetStatusBarText(int indexBar,CString strText)
{
	if (m_wndStatusBar.GetSafeHwnd())
	{
		m_wndStatusBar.SetPaneText(indexBar,strText);
	}
}


void CUdfLogFrame::OnCmdLogQuery()
{
	if ( m_pRightView->GetSafeHwnd())
		m_pRightView->SendMessage(WM_COMMAND,IDM_LOG_QUERY);
}

void CUdfLogFrame::OnCmdLogExport()
{
	if (m_pRightView->GetSafeHwnd())
		m_pRightView->SendMessage(WM_COMMAND,IDM_LOG_EXPORT);
}

void CUdfLogFrame::OnCmdLogDeleteAll()
{
	if (m_pRightView->GetSafeHwnd())
		m_pRightView->SendMessage(WM_COMMAND,IDM_LOG_DELALL);
}

void CUdfLogFrame::OnCmdLogDelete1MonthPre()
{
	if (m_pRightView->GetSafeHwnd())
		m_pRightView->SendMessage(WM_COMMAND,IDM_LOG_DEL1MONTH);
}

void CUdfLogFrame::OnCmdLogDelete2MonthPre()
{
	if (m_pRightView->GetSafeHwnd())
		m_pRightView->SendMessage(WM_COMMAND,IDM_LOG_DEL2MONTH);
}

void CUdfLogFrame::OnCmdLogRefresh()
{
	if (m_pRightView->GetSafeHwnd())
		m_pRightView->SendMessage(WM_COMMAND,IDM_LOG_REFRESH);
}

void CUdfLogFrame::OnLogFrameExit()
{
	ShowWindow(SW_HIDE);
}


void CUdfLogFrame::OnLogOptCfg()
{
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	CMainFrame* pMainFrame = (CMainFrame*)pApp->m_pMainWnd;
	if (pMainFrame && pMainFrame->m_wndCfgLogOptDlg.GetSafeHwnd())
	{
		pMainFrame->m_wndCfgLogOptDlg.Show(SW_SHOW);
	}
}
void CUdfLogFrame::OnUpdateFrameMenu(HMENU hMenuAlt)
{
	// TODO: 在此添加专用代码和/或调用基类
	

	CFrameWnd::OnUpdateFrameMenu(hMenuAlt);
}

BOOL CUdfLogFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle , CWnd* pParentWnd , CCreateContext* pContext)
{
	// TODO: 在此添加专用代码和/或调用基类

	if (! CFrameWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}

	//CMenuXP::UpdateMenuBar(this);
	return TRUE;
}


void CUdfLogFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{

	if (m_pLeftView && m_pLeftView->GetSafeHwnd())
		m_pLeftView->Invalidate(TRUE);

}