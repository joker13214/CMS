// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "CMS.h"

#include "MainFrm.h"
#include "ExitDlg.h"
#include "SystemTimeOp.h"
#include "UserEditPwdDlg.h"
#include "RtuAttrTextMsg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_SHOW_SYSTEM_TASK (WM_USER +118)


#define WTIMER_SHOWDATETIME   0x01      //显示时间日期
#define WTIMER_CHECKRTUSTATE  0x02      //检查遥测终端状态时钟

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)
IMPLEMENT_MENUXP(CMainFrame, CFrameWnd);

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_MENUXP_MESSAGES()
	ON_MESSAGE(WM_SHOW_SYSTEM_TASK,OnShowSystemTask)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_COMMAND(IDM_LOG_SYSTEM,				&CMainFrame::OnLogSystem)
	ON_COMMAND(IDM_USERSSYSTEM,				&CMainFrame::OnUsersSystem)
	ON_COMMAND(IDM_USER_LOGIN,				&CMainFrame::OnUserLogin)
	ON_COMMAND(IDM_USER_EDITPWD,			&CMainFrame::OnUserEditPwd)
	ON_COMMAND(IDM_RTUOPTIONS_CFG,			&CMainFrame::OnRtuOptionsCfg)
	ON_COMMAND(IDM_POP_HIDEFRAME,			&CMainFrame::OnPopHideFrame)
	ON_COMMAND(IDM_POP_SHOWFRAME,			&CMainFrame::OnPopShowFrame)
	ON_COMMAND(IDM_SYSOPTION,				&CMainFrame::OnSysOption)
	ON_COMMAND(IDM_LOGOPT_CFG,				&CMainFrame::OnLogOptCfg)
	ON_COMMAND(IDM_DBOPT_CFG,				&CMainFrame::OnDbOptCfg)
	ON_COMMAND(IDM_DATAINDB,				&CMainFrame::OnRtuDataInDb)
	ON_COMMAND(IDM_COMMSERVERCFG,			&CMainFrame::OnCommServerCfg)
	ON_COMMAND(IDM_MAINGRID_NONE,			&CMainFrame::OnGridLineNone)
	ON_COMMAND(IDM_MAINGRID_VERT,			&CMainFrame::OnGridLineVert)
	ON_COMMAND(IDM_MAINGRID_HORZ,			&CMainFrame::OnGridLineHorz)
	ON_COMMAND(IDM_MAINGRID_BOTH,			&CMainFrame::OnGridLineBoth)
	ON_UPDATE_COMMAND_UI(IDM_MAINGRID_BOTH,	&CMainFrame::OnUpdateUIGridLineBoth)
	ON_UPDATE_COMMAND_UI(IDM_MAINGRID_HORZ,	&CMainFrame::OnUpdateUIGridLineHorz)
	ON_UPDATE_COMMAND_UI(IDM_MAINGRID_NONE,	&CMainFrame::OnUpdateUIGridLineNone)
	ON_UPDATE_COMMAND_UI(IDM_MAINGRID_VERT,	&CMainFrame::OnUpdateUIGridLineVert)
	ON_UPDATE_COMMAND_UI(IDM_USER_EDITPWD,  &CMainFrame::OnUpdateUIEditPwd)
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_NAMECAP,
	ID_INDICATOR_USERNAME,
	ID_INDICATOR_USERTYPE,
	ID_INDICATOR_DATETIME
};

//CMainFrame 构造/析构
CMainFrame::CMainFrame()
{
	m_pShowDetailLogDlg = NULL;
	m_pLeftView = NULL;
	m_pRightView = NULL;
}

CMainFrame::~CMainFrame()
{
	DeleteIconTray();

	if (m_pShowDetailLogDlg != NULL)
	{
		delete m_pShowDetailLogDlg;
		m_pShowDetailLogDlg = NULL;
	}
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CMenuXP::SetXPLookNFeel (this);

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      
	}	

	UpdateStatusBarText();
	 
	if (CreateToolbar() == FALSE)
	{
		return -1;
	}

	if (CreateOtherFrame() == FALSE)
	{
		return FALSE;
	}

	if (InitRtuPropertyDlg() == false)
		return FALSE;
	
	IconToTray();

	SetTimer(WTIMER_SHOWDATETIME,1000,NULL);
	InitCheckRtuStateTimer();
	return 0;
}

void CMainFrame::UpdateStatusBarText()
{
	if (m_wndStatusBar.GetSafeHwnd())
	{
		CString strText = "";
		CCMSApp* pApp = (CCMSApp*)AfxGetApp();
		if (pApp->GetLoginFlag() == true && 
			pApp->GetLoginUser() != NULL)
		{
			m_wndStatusBar.SetPaneText(2,pApp->GetLoginUser()->GetName());

			if( pApp->GetLoginUser()->GetType() == 1)
				strText = "系统管理员";
			else if (pApp->GetLoginUser()->GetType() == 2)
				strText = "操作员";

			m_wndStatusBar.SetPaneText(3,strText);
		}
		else
		{
			m_wndStatusBar.SetPaneText(2,"未登录");
			m_wndStatusBar.SetPaneText(3,"");
		}
	}
}

BOOL CMainFrame::CreateToolbar()
{
   
	if (!m_wndToolBar.CreateEx(this,  TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))	
	{
		TRACE0("未能创建工具栏\n");
		return FALSE;      // 未能创建
	}

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	m_wndToolBar.ShowWindow(SW_HIDE);

	return TRUE;
}

BOOL CMainFrame::InitCheckRtuStateTimer()
{
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	if (pApp->m_GlobalOptions.m_nCheckRtuInterval > 0 )
	{
		UINT nElapse = pApp->m_GlobalOptions.m_nCheckRtuInterval * 60 * 1000;
		SetTimer(WTIMER_CHECKRTUSTATE,nElapse,NULL);
	}
	return TRUE;
}

BOOL CMainFrame::CreateOtherFrame()
{
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();

	if (!m_wndLogFrame.Create(NULL, "日志系统", WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_SIZEBOX|WS_MAXIMIZEBOX|WS_MINIMIZEBOX,
		CRect(0, 0, 960, 420), NULL))
	{
		TRACE("Failed to create Log message view frame\r\n");
		return FALSE;
	}
	m_wndLogFrame.ShowWindow(SW_HIDE);
	m_wndLogFrame.ModifyStyleEx(WS_EX_APPWINDOW, NULL);

	if (m_pRightView && m_pRightView->GetSafeHwnd())
		m_wndLogFrame.CenterWindow(m_pRightView);
	else
		m_wndLogFrame.CenterWindow();

	if (pApp && pApp->m_pLogSystem)
	{
		pApp->m_pLogSystem->SetLogFrame(&m_wndLogFrame);
	}

	if (!m_wndRtuCfgFrame.Create(NULL,"遥测终端信息配置",WS_POPUP|WS_CAPTION|WS_SYSMENU|WS_SIZEBOX|WS_MAXIMIZEBOX|WS_MINIMIZEBOX,
		CRect(0,0,1000/*960*/,420),NULL))
	{
		TRACE("Failed to create Rtu Configuration Frame\r\n");
		return FALSE;
	}
	m_wndRtuCfgFrame.ShowWindow(SW_HIDE);
	m_wndRtuCfgFrame.ModifyStyleEx(WS_EX_APPWINDOW, NULL);
	if (m_pRightView && m_pRightView->GetSafeHwnd())
		m_wndRtuCfgFrame.CenterWindow(m_pRightView);
	else
		m_wndRtuCfgFrame.CenterWindow();

	m_pShowDetailLogDlg = new CUdfShowDetailLog(&m_wndLogFrame);
	if (!m_pShowDetailLogDlg->Create(IDD_DLG_SHOWLOG))
	{
		return FALSE;
	}
	m_pShowDetailLogDlg->ShowWindow(SW_HIDE);
	m_pShowDetailLogDlg->CenterWindow(&m_wndLogFrame);

	if(!m_wndCfgOdbcDlg.Create(IDD_DLG_CFGDSN,this))
	{
		return FALSE;
	}
	m_wndCfgOdbcDlg.ShowWindow(SW_HIDE);
	m_wndCfgOdbcDlg.CenterWindow();


	if(!m_wndCfgLogOptDlg.Create(IDD_DLG_CFGLOGOPT,this))
	{
		return FALSE;
	}
	m_wndCfgLogOptDlg.Show(SW_HIDE);
	m_wndCfgLogOptDlg.CenterWindow();


	if (!m_wndDataInDbLogDlg.Create(IDD_DLG_DATAINDBLOG,this))
	{
		return FALSE;
	}
	m_wndDataInDbLogDlg.ShowWindow(SW_HIDE);


	if(!m_wndCommServerMnDlg.Create(IDD_DLG_COMMSERVER,this))
	{
		return FALSE;
	}
	m_wndCommServerMnDlg.CenterWindow(this);
	m_wndCommServerMnDlg.ShowWindow(SW_HIDE);

	if (!m_wndUserSystemDlg.Create(CUserSystemDlg::IDD,this))
	{
		return FALSE;
	}
	m_wndUserSystemDlg.CenterWindow(this);
	m_wndUserSystemDlg.ShowWindow(SW_HIDE);

	if(!m_wndUserLogin.Create(CUserLoginDlg::IDD,this))
	{
		return FALSE;
	}
	m_wndUserLogin.CenterWindow(this);
	m_wndUserLogin.UpdateDailog();
	m_wndUserLogin.ShowWindow(SW_HIDE);


	if(!m_wndShowLogDlg.Create(CShowLogDlg::IDD,this))
	{
		return FALSE;
	}
	m_wndShowLogDlg.CenterWindow(this);
	m_wndShowLogDlg.ShowWindow(SW_HIDE);
	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}


// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// 在托盘区添加软件图标
void CMainFrame::IconToTray(void)
{
	NOTIFYICONDATA nid;     
	nid.cbSize=(DWORD)sizeof(NOTIFYICONDATA);     
	nid.hWnd=this->m_hWnd;     
	nid.uID =IDR_MAINFRAME;     
	nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP ;     
	nid.uCallbackMessage=WM_SHOW_SYSTEM_TASK;										//托盘自定义的消息名称     
	nid.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));     
	//strcpy_s(nid.szTip,"CMS - 通信管理服务器 (Communications Management Server) ");	//信息提示条     
	strcpy_s(nid.szTip,"CMS - 通信前置机");											//信息提示条 
	Shell_NotifyIcon(NIM_ADD,&nid);													//在托盘区添加图标  
}


//wParam接收的是图标的ID，而lParam接收的是鼠标的行为 
LRESULT CMainFrame::OnShowSystemTask(WPARAM wParam,LPARAM lParam) 
{     
	if(wParam!=IDR_MAINFRAME)        
		return 1; 

	switch(lParam)    
	{            
	case WM_RBUTTONUP:																		//右键起来时弹出快捷菜单，这里只有一个“关闭”         
		{   
			CPoint lpoint;             
			::GetCursorPos(&lpoint);														//得到鼠标位置    

			CMenu menu;             
			//弹出菜单
			menu.LoadMenu(IDR_MENU_POP);
			CMenu* pupMenu = menu.GetSubMenu(0);
			if (pupMenu)
				pupMenu->TrackPopupMenu(TPM_LEFTALIGN,lpoint.x,lpoint.y,this);               
		}            
		break;         
	case WM_LBUTTONDBLCLK://双击左键的处理         
		OnPopShowFrame();																	//简单的显示主窗口完事儿                      
		break;         
	default:           
		break;
	}     
	return 0; 
} 


void CMainFrame::DeleteIconTray()
{  
	NOTIFYICONDATA nid;   
	nid.cbSize=(DWORD)sizeof(NOTIFYICONDATA);   
	nid.hWnd=this->m_hWnd;   
	nid.uID=IDR_MAINFRAME;   
	nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP ;   
	nid.uCallbackMessage=WM_SHOW_SYSTEM_TASK;												//自定义的消息名称   
	nid.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));   
	strcpy_s(nid.szTip,"CMS - 通信管理服务器 (Communications Management Server) ");			//信息提示条为“计划任务提醒”   
	Shell_NotifyIcon(NIM_DELETE,&nid);														//在托盘区删除图标 
}


void CMainFrame::OnClose()
{

	//ShowOtherFrame(SW_HIDE);

	CExitDlg dlg (this);
	if (dlg.DoModal() != IDOK)
	{
		return;
	}

    DeleteIconTray();
	CFrameWnd::OnClose();
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);
	CRect rc;
	GetClientRect(&rc);
}




void CMainFrame::OnDestroy()
{
	CFrameWnd::OnDestroy();		

}

void CMainFrame::OnSysOption()
{

}

void CMainFrame::UpdateClientViews()
{
	if (m_pLeftView && m_pLeftView->GetSafeHwnd())
		m_pLeftView->Invalidate(TRUE);
}

void CMainFrame::OnPopShowFrame()
{
	ShowWindow(SW_SHOWMAXIMIZED); 
}

void CMainFrame::OnPopHideFrame()
{
	ShowWindow(SW_HIDE);
}


bool CMainFrame::GetLoginFlag(BYTE& uType)
{
	//测试时使用，不用用户登录
	//--------------------------------------
#ifdef DEBUG
	uType = 1;
	return true;
#endif
	//--------------------------------------

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	if (pApp->GetLoginFlag() == false ||
		pApp->GetLoginUser() == NULL)
	{
		uType = 0;
		ShowOtherFrame(SW_HIDE);
		MessageBox("用户未登录,请登录!","提示",MB_OK|MB_ICONERROR);
		return false;
	}
	else
	{
		uType = pApp->GetLoginUser()->GetType();
		return true;
	}
}

void CMainFrame::OnLogSystem()
{
	BYTE utype = 0;
	if (GetLoginFlag(utype) == false)
		return;


	if (m_wndLogFrame.GetSafeHwnd())
	{
		if (!m_wndLogFrame.IsWindowVisible())
			m_wndLogFrame.ShowWindow(SW_SHOW);
		m_wndLogFrame.ActivateFrame();
	}

	UpdateClientViews();
}


void CMainFrame::OnRtuOptionsCfg()
{
	BYTE utype = 0;
	if (GetLoginFlag(utype) == false)
		return;
  
  if (m_wndRtuCfgFrame.GetSafeHwnd())
  {
	   if(!m_wndRtuCfgFrame.IsWindowVisible())
		   m_wndRtuCfgFrame.ShowWindow(SW_SHOW);
	   m_wndRtuCfgFrame.ActivateFrame();
  }

  UpdateClientViews();
}

void CMainFrame::OnCommServerCfg()
{
	BYTE utype = 0;
	if (GetLoginFlag(utype) == false)
		return;

	if (m_wndCommServerMnDlg.GetSafeHwnd())
	{
		m_wndCommServerMnDlg.ShowWindow(SW_SHOWNORMAL);
	}

	UpdateClientViews();
}


void CMainFrame::OnDbOptCfg()
{
	BYTE utype = 0;
	if (GetLoginFlag(utype) == false)
		return;

	if (m_wndCfgOdbcDlg.GetSafeHwnd())
	{
		m_wndCfgOdbcDlg.Show(SW_SHOW);
	}

	UpdateClientViews();
}

void CMainFrame::OnLogOptCfg()
{
	BYTE utype = 0;
	if (GetLoginFlag(utype) == false)
		return;

	if (m_wndCfgLogOptDlg.GetSafeHwnd())
	{
		m_wndCfgLogOptDlg.Show(SW_SHOW);
	}

	UpdateClientViews();
}

void CMainFrame::OnUsersSystem()
{
	BYTE utype = 0;
	if (GetLoginFlag(utype) == false)
		return;

	if (utype != 1)
	{
		ShowOtherFrame(SW_HIDE);
		MessageBox("只有'系统管理员'才能操作用户系统!","提示",MB_OK|MB_ICONERROR);
		return;
	}

	if (m_wndUserSystemDlg.GetSafeHwnd())
		m_wndUserSystemDlg.ShowWindow(SW_SHOWNORMAL);


	UpdateClientViews();
}

void CMainFrame::OnUserLogin()
{
	if (m_wndUserLogin.GetSafeHwnd())
	{
		m_wndUserLogin.UpdateDailog();
		m_wndUserLogin.ShowWindow(SW_SHOW);
	}

	UpdateClientViews();
}

void CMainFrame::OnUserEditPwd()
{
	CUserEditPwdDlg dlg (this);
	dlg.DoModal();
	UpdateClientViews();
}

void CMainFrame::OnUpdateUIEditPwd(CCmdUI* pCmdUI)
{
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	if (pApp->GetLoginUser() != NULL &&
		pApp->GetLoginFlag() == true)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	if (!m_wndSplitterWnd.CreateStatic(this,1,2))
	{
		TRACE("CMainframe Create Client View ERROR\r\n");
		return FALSE;
	}

	CRect cr;
	GetClientRect(&cr);

	if (!m_wndSplitterWnd.CreateView(0,0,RUNTIME_CLASS(CCMSLeftView),CSize(240,cr.Height()),pContext))
	{
		TRACE("CMainframe Create Left View Error\r\n");
		return FALSE;
	}

	if (!m_wndSplitterWnd.CreateView(0,1,RUNTIME_CLASS(CCMSRightView),CSize(0,0),pContext))
	{
		TRACE("CMainframe Create Right View Error\r\n");
		return FALSE;
	}

	m_pLeftView = (CCMSLeftView*)m_wndSplitterWnd.GetPane(0,0);
	if (m_pLeftView->GetSafeHwnd())
	{
		m_pLeftView->InitTreeItem();
	}

	m_pRightView = (CCMSRightView*)m_wndSplitterWnd.GetPane(0,1);
	if (m_pRightView->GetSafeHwnd())
	{
		if (m_wndLogFrame.GetSafeHwnd())
			m_wndLogFrame.CenterWindow(m_pRightView);

		if (m_wndRtuCfgFrame.GetSafeHwnd())
			m_wndRtuCfgFrame.CenterWindow(m_pRightView);
	}
	if (m_wndSplitterWnd.GetSafeHwnd())
	m_wndSplitterWnd.RecalcLayout();

	return TRUE;//CFrameWnd::OnCreateClient(lpcs, pContext);
}


void CMainFrame::ShowOtherFrame(int nCmdShow)
{
	//if (m_wndCfgLogOptDlg.GetSafeHwnd())
	//{   
	//	m_wndCfgLogOptDlg.ShowWindow(nCmdShow);
	//}
	//if (m_pShowDetailLogDlg && m_pShowDetailLogDlg->GetSafeHwnd())
	//{
	//	m_pShowDetailLogDlg->ShowWindow(nCmdShow);
	//}
	//if (m_wndLogFrame.GetSafeHwnd())
	//{
	//	m_wndLogFrame.ShowWindow(nCmdShow);
	//}
	//if (m_wndRtuCfgFrame.GetSafeHwnd())
	//{
	//	m_wndRtuCfgFrame.ShowWindow(nCmdShow);
	//}
	//if (m_wndCfgOdbcDlg.GetSafeHwnd())
	//{
	//	m_wndCfgOdbcDlg.Show(nCmdShow);
	//}
	//if (m_wndDataInDbLogDlg.GetSafeHwnd())
	//{
	//	m_wndDataInDbLogDlg.ShowWindow(nCmdShow);
	//}
	//if (m_wndCommServerMnDlg.GetSafeHwnd())
	//{
	//	m_wndCommServerMnDlg.ShowWindow(nCmdShow);
	//}
	//if (m_wndUserSystemDlg.GetSafeHwnd())
	//	m_wndUserSystemDlg.ShowWindow(nCmdShow);
}


void CMainFrame::UpdateFrameOnLogin()
{

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	if (pApp->GetLoginUser() == NULL || pApp->GetLoginFlag() == false)
	{
		int nCmdShow = SW_HIDE;

		if (m_wndCfgLogOptDlg.GetSafeHwnd())
			m_wndCfgLogOptDlg.ShowWindow(nCmdShow);
		if (m_pShowDetailLogDlg && m_pShowDetailLogDlg->GetSafeHwnd())
			m_pShowDetailLogDlg->ShowWindow(nCmdShow);

		if (m_wndLogFrame.GetSafeHwnd())
			m_wndLogFrame.ShowWindow(nCmdShow);

		if (m_wndRtuCfgFrame.GetSafeHwnd())
			m_wndRtuCfgFrame.ShowWindow(nCmdShow);

		if (m_wndCfgOdbcDlg.GetSafeHwnd())
			m_wndCfgOdbcDlg.Show(nCmdShow);

		if (m_wndCommServerMnDlg.GetSafeHwnd())
			m_wndCommServerMnDlg.ShowWindow(nCmdShow);

		if (m_wndUserSystemDlg.GetSafeHwnd())
			m_wndUserSystemDlg.ShowWindow(nCmdShow); 

		//if (pApp->m_pReadDB != NULL)
		//{
		//	POSITION rPos = pApp->m_pReadDB->GetRtuStList()->GetHeadPosition();
		//	while (rPos != NULL)
		//	{
		//		CRtuStation* pSub = (CRtuStation*)pApp->m_pReadDB->GetRtuStList()->GetNext(rPos);
		//		if (pSub)
		//			pSub->ShowPropertyDlg(nCmdShow);
		//	}
		//}
		
		GetRtuPropertyDlg()->Show(SW_HIDE);
	}



}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle , CWnd* pParentWnd , CCreateContext* pContext)
{
	if ( !CFrameWnd::LoadFrame (nIDResource, dwDefaultStyle, pParentWnd, pContext) )
	{
		return FALSE;
	}
	//CMenuXP::UpdateMenuBar (this);

	return TRUE;
}


void CMainFrame::OnRtuDataInDb()
{
	if (m_wndDataInDbLogDlg.GetSafeHwnd())
		m_wndDataInDbLogDlg.ShowWindow(SW_SHOWNORMAL);
}




void CMainFrame::OnGridLineNone()
{
	if (m_pRightView && m_pRightView->m_wndListCtrl.GetSafeHwnd())
	{
		m_pRightView->m_wndListCtrl.SetGridLines(GVL_NONE);
	}
}

void CMainFrame::OnGridLineVert()
{
	if (m_pRightView && m_pRightView->m_wndListCtrl.GetSafeHwnd())
	{
		m_pRightView->m_wndListCtrl.SetGridLines(GVL_VERT);
	}
}

void CMainFrame::OnGridLineHorz()
{
	if (m_pRightView && m_pRightView->m_wndListCtrl.GetSafeHwnd())
	{
		m_pRightView->m_wndListCtrl.SetGridLines(GVL_HORZ);
	}
}

void CMainFrame::OnGridLineBoth()
{
	if (m_pRightView && m_pRightView->m_wndListCtrl.GetSafeHwnd())
	{
		m_pRightView->m_wndListCtrl.SetGridLines(GVL_BOTH);
	}
}


void CMainFrame::OnUpdateUIGridLineBoth(CCmdUI* pCmdUI)
{
	if (m_pRightView && m_pRightView->m_wndListCtrl.GetSafeHwnd())
	{
		int nGirdLinesStyle = m_pRightView->m_wndListCtrl.GetGridLines();
		if (nGirdLinesStyle == GVL_BOTH)
			pCmdUI->SetCheck(TRUE);
		else
			pCmdUI->SetCheck(FALSE);
	}
}

void CMainFrame::OnUpdateUIGridLineHorz(CCmdUI* pCmdUI)
{
	if (m_pRightView && m_pRightView->m_wndListCtrl.GetSafeHwnd())
	{
		int nGirdLinesStyle = m_pRightView->m_wndListCtrl.GetGridLines();
		if (nGirdLinesStyle == GVL_HORZ)
			pCmdUI->SetCheck(TRUE);
		else
			pCmdUI->SetCheck(FALSE);
	}
}


void CMainFrame::OnUpdateUIGridLineNone(CCmdUI* pCmdUI)
{
	if (m_pRightView && m_pRightView->m_wndListCtrl.GetSafeHwnd())
	{
		int nGirdLinesStyle = m_pRightView->m_wndListCtrl.GetGridLines();
		if (nGirdLinesStyle == GVL_NONE)
			pCmdUI->SetCheck(TRUE);
		else
			pCmdUI->SetCheck(FALSE);
	}
}

void CMainFrame::OnUpdateUIGridLineVert(CCmdUI* pCmdUI)
{
	if (m_pRightView && m_pRightView->m_wndListCtrl.GetSafeHwnd())
	{
		int nGirdLinesStyle = m_pRightView->m_wndListCtrl.GetGridLines();
		if (nGirdLinesStyle == GVL_VERT)
			pCmdUI->SetCheck(TRUE);
		else
			pCmdUI->SetCheck(FALSE);
	}
}
void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case  WTIMER_SHOWDATETIME:
		{
			CTime _c_tm = CTime::GetCurrentTime();
			CString strTime = "";
			strTime.Format("时间 %02d:%02d:%02d",
				_c_tm.GetHour(),
				_c_tm.GetMinute(),
				_c_tm.GetSecond());

			if(m_wndStatusBar.GetSafeHwnd())
				m_wndStatusBar.SetPaneText(4,strTime);
		}
		break;
	case WTIMER_CHECKRTUSTATE:
		{
			CCMSApp* pApp = (CCMSApp*)AfxGetApp();

			if (pApp->m_pReadDB != NULL)
			{
				CPtrList* pLst = pApp->m_pReadDB->GetRtuStList();

				if (pLst != NULL)
				{
					POSITION rPos = pLst->GetHeadPosition();
					while (rPos != NULL)
					{
						CRtuStation* pSub = (CRtuStation*)pLst->GetNext(rPos);
						if (  pSub  && pSub->GetCommState() == Connected_State &&
							 (pSub->GetInterface() == Serial_CommInterface ||
							 (pSub->GetInterface() == Ethernet_CommInerface && pSub->GetProtocolType() == UDP_Protocol)))
						{
							bool blModified  = false;
							SYSTEMTIME st; 
							GetLocalTime(&st);

							UINT64 Minus = CSystemTimeOp::MinusSystemTime(st,*(pSub->GetLastRecvTime()));

							UINT64 m = Minus /60000;
							if (m >= pApp->m_GlobalOptions.m_nCheckRtuInterval)
							{
								blModified = pSub->SetCommState(CommUnknown_State);
								if (blModified ==true)
								{
									if ( m_pRightView && m_pRightView->GetSafeHwnd())
										m_pRightView->UpdateCommState(pSub);
								}
							}
						}
					}
				}			
			}
		}
	}

	CFrameWnd::OnTimer(nIDEvent);
}


bool CMainFrame::InitRtuPropertyDlg()															//初始化属性操作窗口
{
	m_wndRtuPropertyDlg.SetRtuStation(NULL);
	if (!m_wndRtuPropertyDlg.Create(CRtuPropertyDlg::IDD,this))
		return false;
	m_wndRtuPropertyDlg.ShowWindow(SW_HIDE);

	m_wndCommServerMnDlg.SetTextMsgDlg(m_wndRtuPropertyDlg.GetTab_AttrTextMsg());
	return true;
}

CRtuPropertyDlg* CMainFrame::GetRtuPropertyDlg()
{

	return &m_wndRtuPropertyDlg;
}

void CMainFrame::ShowLog( CString strDate,CString strLog )
{
	if (m_wndShowLogDlg.GetSafeHwnd())
	{
		m_wndShowLogDlg.ShowWindow(SW_HIDE);
		m_wndShowLogDlg.ShowLog(strDate,strLog);
	}
}
