// UdfRtuCfgFrame.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "UdfRtuCfgFrame.h"
#include "UdfAddRtuStDlg.h"
#include "XpStyle/MenuXP.h"

static UINT rtucfgframe_indicators[] =
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

// CUdfRtuCfgFrame

IMPLEMENT_DYNCREATE(CUdfRtuCfgFrame, CFrameWnd)
IMPLEMENT_MENUXP(CUdfRtuCfgFrame, CFrameWnd)

CUdfRtuCfgFrame::CUdfRtuCfgFrame()
{
	m_pRtuCfgLeftView = NULL;
	m_pRtuCfgRightView = NULL;
}

CUdfRtuCfgFrame::~CUdfRtuCfgFrame()
{
}


BEGIN_MESSAGE_MAP(CUdfRtuCfgFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_MENUXP_MESSAGES()
	ON_COMMAND(IDM_RTUCFG_EXIT, &CUdfRtuCfgFrame::OnRtuCfgExit)
	ON_COMMAND(IDM_RTUCFG_RTUSTADD, &CUdfRtuCfgFrame::OnRtuCfgRtuStAdd)
	ON_COMMAND(IDM_RTUCFG_YCADD,&CUdfRtuCfgFrame::OnRtuCfgYcAdd)
	ON_UPDATE_COMMAND_UI(IDM_RTUCFG_YCADD,&CUdfRtuCfgFrame::OnUpdateUIRtuCfgYcAdd)
	ON_COMMAND(IDM_RTUCFG_YXADD,&CUdfRtuCfgFrame::OnRtuCfgYxAdd)
	ON_UPDATE_COMMAND_UI(IDM_RTUCFG_YXADD,&CUdfRtuCfgFrame::OnUpdateUIRtuCfgYxAdd)
	ON_COMMAND(IDM_RTUCFG_YKADD,&CUdfRtuCfgFrame::OnRtuCfgYkAdd)
	ON_UPDATE_COMMAND_UI(IDM_RTUCFG_YKADD,&CUdfRtuCfgFrame::OnUpdateUIRtuCfgYkAdd)
	ON_COMMAND(IDM_RTUCFG_ZDADD,&CUdfRtuCfgFrame::OnRtuCfgZdAdd)
	ON_UPDATE_COMMAND_UI(IDM_RTUCFG_ZDADD,&CUdfRtuCfgFrame::OnUpdateUIRtuCfgZdAdd)

	ON_COMMAND(IDM_RTUCFG_NONE,&CUdfRtuCfgFrame::OnRtuCfgGridNone)
	ON_COMMAND(IDM_RTUCFG_HORZ,&CUdfRtuCfgFrame::OnRtuCfgGridHorz)
	ON_COMMAND(IDM_RTUCFG_VERT,&CUdfRtuCfgFrame::OnRtuCfgGridVert)
	ON_COMMAND(IDM_RTUCFG_BOTH,&CUdfRtuCfgFrame::OnRtuCfgGridBoth)
	ON_UPDATE_COMMAND_UI(IDM_RTUCFG_NONE,&CUdfRtuCfgFrame::OnUpdateUIRtuCfgGridNone)
	ON_UPDATE_COMMAND_UI(IDM_RTUCFG_HORZ,&CUdfRtuCfgFrame::OnUpdateUIRtuCfgGridHorz)
	ON_UPDATE_COMMAND_UI(IDM_RTUCFG_VERT,&CUdfRtuCfgFrame::OnUpdateUIRtuCfgGridVert)
	ON_UPDATE_COMMAND_UI(IDM_RTUCFG_BOTH,&CUdfRtuCfgFrame::OnUpdateUIRtuCfgGridBoth)

END_MESSAGE_MAP()


// CUdfRtuCfgFrame 消息处理程序

int CUdfRtuCfgFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//Show Destop bar
	ModifyStyleEx(WS_EX_APPWINDOW,0); 
	//LONG style = ::GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE);	//获得窗体扩展风格
	//style &= ~WS_EX_APPWINDOW;									//取消WS_EX_APPWINDOW
	//::SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, style);			//设置窗体扩展风格

	HICON hicon;
	hicon = AfxGetApp()->LoadIcon(IDI_RTUCFG_FRAME);
	SetIcon(hicon,TRUE);

	CMenuXP::SetXPLookNFeel(this);
	CMenuXP::UpdateMenuBar(this);

	m_wndFrameMenu.LoadMenu(IDR_MENU_RTYCFG);
	if (m_wndFrameMenu.GetSafeHmenu())
		SetMenu(&m_wndFrameMenu);



	SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	return 0;
}


BOOL CUdfRtuCfgFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (!m_wndSplitterWnd.CreateStatic(this,1,2))
	{
		TRACE("CUdfRtuCfgFrame Create Clieent View ERROR\r\n");
		return FALSE;
	}

	CRect cr;
	GetClientRect(&cr);

	if (!m_wndSplitterWnd.CreateView(0,0,RUNTIME_CLASS(CUdfRtuCfgLeftView),CSize(160,cr.Height()),pContext))
	{
		TRACE("CUdfRtuCfgFrame Create Left View Error\r\n");
		return FALSE;
	}

	if (!m_wndSplitterWnd.CreateView(0,1,RUNTIME_CLASS(CUdfRtuCfgRightView),CSize(0,0),pContext))
	{
		TRACE("CUdfRtuCfgFrame Create Right View Error\r\n");
		return FALSE;
	}

	m_pRtuCfgLeftView = (CUdfRtuCfgLeftView*)m_wndSplitterWnd.GetPane(0,0);
	if (m_pRtuCfgLeftView->GetSafeHwnd())
	{
		m_pRtuCfgLeftView->SetRtuCfgFrame(this);
	}

	m_pRtuCfgRightView = (CUdfRtuCfgRightView*)m_wndSplitterWnd.GetPane(0,1);
	if (m_pRtuCfgRightView->GetSafeHwnd())
	{
		m_pRtuCfgRightView->SetRtuCfgFrame(this);
	}

	SetActiveView(m_pRtuCfgRightView);

	return CFrameWnd::OnCreateClient(lpcs, pContext);
}

void CUdfRtuCfgFrame::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ShowWindow(SW_HIDE);

	//CFrameWnd::OnClose();
}


void CUdfRtuCfgFrame::OnRtuCfgExit()
{
	SendMessage(WM_CLOSE,NULL,NULL);
}


void CUdfRtuCfgFrame::OnRtuCfgRtuStAdd()
{
	//CUdfAddRtuStDlg dlg(this);
	//dlg.DoModal();
	if (m_pRtuCfgLeftView && m_pRtuCfgLeftView->GetSafeHwnd())
		m_pRtuCfgLeftView->SendMessage(WM_COMMAND,IDM_RTUCFG_RTUSTADD,NULL);
	
}

void CUdfRtuCfgFrame::OnRtuCfgYcAdd()
{
	if (m_pRtuCfgLeftView && m_pRtuCfgLeftView->GetSafeHwnd())
		m_pRtuCfgLeftView->SendMessage(WM_COMMAND,IDM_RTUCFG_YCADD);
}

void CUdfRtuCfgFrame::OnRtuCfgYxAdd()
{
	if (m_pRtuCfgLeftView && m_pRtuCfgLeftView->GetSafeHwnd())
		m_pRtuCfgLeftView->SendMessage(WM_COMMAND,IDM_RTUCFG_YXADD);
}

void CUdfRtuCfgFrame::OnRtuCfgYkAdd()
{
	if (m_pRtuCfgLeftView && m_pRtuCfgLeftView->GetSafeHwnd())
		m_pRtuCfgLeftView->SendMessage(WM_COMMAND,IDM_RTUCFG_YKADD);
}

void CUdfRtuCfgFrame::OnRtuCfgZdAdd()
{
	if (m_pRtuCfgLeftView && m_pRtuCfgLeftView->GetSafeHwnd())
		m_pRtuCfgLeftView->SendMessage(WM_COMMAND,IDM_RTUCFG_ZDADD);
}



void CUdfRtuCfgFrame::SetStatusBarText(int barIndex,CString strText)
{
	//if (m_wndStatusBar.GetSafeHwnd())
	//	m_wndStatusBar.SetPaneText(barIndex,strText);
}


void CUdfRtuCfgFrame::OnUpdateUIRtuCfgYcAdd(CCmdUI* pCmdUI)
{
	if (m_pRtuCfgLeftView && 
		(m_pRtuCfgLeftView->blSelectItemRtuSt() == true||
		 m_pRtuCfgLeftView->m_wndTreeCtrl.GetItemText(m_pRtuCfgLeftView->m_pSelectItem).Find("遥测") != -1))
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable( FALSE);
	}
}

void CUdfRtuCfgFrame::OnUpdateUIRtuCfgYxAdd(CCmdUI* pCmdUI)
{
	if (m_pRtuCfgLeftView && 
		(m_pRtuCfgLeftView->blSelectItemRtuSt() == true ||
		m_pRtuCfgLeftView->m_wndTreeCtrl.GetItemText(m_pRtuCfgLeftView->m_pSelectItem).Find("遥信") != -1))
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable( FALSE);
	}
}

void CUdfRtuCfgFrame::OnUpdateUIRtuCfgYkAdd(CCmdUI* pCmdUI)
{
	if (m_pRtuCfgLeftView && 
		(m_pRtuCfgLeftView->blSelectItemRtuSt() == true || 
		 m_pRtuCfgLeftView->m_wndTreeCtrl.GetItemText(m_pRtuCfgLeftView->m_pSelectItem).Find("遥控") != -1) )
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable( FALSE);
	}
}

void CUdfRtuCfgFrame::OnUpdateUIRtuCfgZdAdd(CCmdUI* pCmdUI)
{
	if (m_pRtuCfgLeftView && 
		(m_pRtuCfgLeftView->blSelectItemRtuSt() == true||
		m_pRtuCfgLeftView->m_wndTreeCtrl.GetItemText(m_pRtuCfgLeftView->m_pSelectItem).Find("定值") != -1))
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable( FALSE);
	}
}



void CUdfRtuCfgFrame::OnRtuCfgGridNone()
{
	if (m_pRtuCfgRightView->GetSafeHwnd() && 
		m_pRtuCfgRightView->m_wndGridCtrl.GetSafeHwnd())
	{
		m_pRtuCfgRightView->m_wndGridCtrl.SetGridLines(GVL_NONE);
	}
}

void CUdfRtuCfgFrame::OnRtuCfgGridHorz()
{
	if (m_pRtuCfgRightView->GetSafeHwnd() && 
		m_pRtuCfgRightView->m_wndGridCtrl.GetSafeHwnd())
	{
		m_pRtuCfgRightView->m_wndGridCtrl.SetGridLines(GVL_HORZ);
	}
}

void CUdfRtuCfgFrame::OnRtuCfgGridVert()
{
	if (m_pRtuCfgRightView->GetSafeHwnd() && 
		m_pRtuCfgRightView->m_wndGridCtrl.GetSafeHwnd())
	{
		m_pRtuCfgRightView->m_wndGridCtrl.SetGridLines(GVL_VERT);
	}
}

void CUdfRtuCfgFrame::OnRtuCfgGridBoth()
{
	if (m_pRtuCfgRightView->GetSafeHwnd() && 
		m_pRtuCfgRightView->m_wndGridCtrl.GetSafeHwnd())
	{
		m_pRtuCfgRightView->m_wndGridCtrl.SetGridLines(GVL_BOTH);
	}
}

void CUdfRtuCfgFrame::OnUpdateUIRtuCfgGridNone(CCmdUI* pCmdUI)
{
	if (m_pRtuCfgRightView->GetSafeHwnd() && 
		m_pRtuCfgRightView->m_wndGridCtrl.GetSafeHwnd())
	{
		if (m_pRtuCfgRightView->m_wndGridCtrl.GetGridLines() == GVL_NONE)
		{
			pCmdUI->SetCheck(1);
		}
		else
		{
			pCmdUI->SetCheck(0);
		}
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

void CUdfRtuCfgFrame::OnUpdateUIRtuCfgGridHorz(CCmdUI* pCmdUI)
{
	if (m_pRtuCfgRightView->GetSafeHwnd() && 
		m_pRtuCfgRightView->m_wndGridCtrl.GetSafeHwnd())
	{
		if (m_pRtuCfgRightView->m_wndGridCtrl.GetGridLines() == GVL_HORZ)
		{
			pCmdUI->SetCheck(1);
		}
		else
		{
			pCmdUI->SetCheck(0);
		}
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

void CUdfRtuCfgFrame::OnUpdateUIRtuCfgGridVert(CCmdUI* pCmdUI)
{
	if (m_pRtuCfgRightView->GetSafeHwnd() && 
		m_pRtuCfgRightView->m_wndGridCtrl.GetSafeHwnd())
	{
		if (m_pRtuCfgRightView->m_wndGridCtrl.GetGridLines() == GVL_VERT)
		{
			pCmdUI->SetCheck(1);
		}
		else
		{
			pCmdUI->SetCheck(0);
		}
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

void CUdfRtuCfgFrame::OnUpdateUIRtuCfgGridBoth(CCmdUI* pCmdUI)
{
	if (m_pRtuCfgRightView->GetSafeHwnd() && 
		m_pRtuCfgRightView->m_wndGridCtrl.GetSafeHwnd())
	{
		if (m_pRtuCfgRightView->m_wndGridCtrl.GetGridLines() == GVL_BOTH)
		{
			pCmdUI->SetCheck(1);
		}
		else
		{
			pCmdUI->SetCheck(0);
		}
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}


void CUdfRtuCfgFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (nType ==SIZE_MINIMIZED)
	{
		ShowWindow(SW_HIDE);

	}
}
void CUdfRtuCfgFrame::OnUpdateFrameMenu(HMENU hMenuAlt)
{
	// TODO: 在此添加专用代码和/或调用基类

	
	CFrameWnd::OnUpdateFrameMenu(hMenuAlt);
}

BOOL CUdfRtuCfgFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle , CWnd* pParentWnd , CCreateContext* pContext)
{
	// TODO: 在此添加专用代码和/或调用基类

	if ( !CFrameWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}
	CMenuXP::UpdateMenuBar(this);

	return TRUE;
}

void CUdfRtuCfgFrame::OnDestroy()
{
	if (m_pRtuCfgLeftView && m_pRtuCfgLeftView->GetSafeHwnd())
		m_pRtuCfgLeftView->DestroyWindow();
	if (m_pRtuCfgRightView && m_pRtuCfgRightView->GetSafeHwnd())
		m_pRtuCfgRightView->DestroyWindow();

	CFrameWnd::OnDestroy();
}
