#pragma once


// CUdfLogFrame 框架
#include "UdfLogLeftView.h"
#include "UdfLogRightView.h"
#include "UdfSplitterWnd.h"
#include "XpStyle/StatusBarXP.h"

class CUdfLogFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CUdfLogFrame)
public:
	CUdfLogFrame();           // 动态创建所使用的受保护的构造函数
	virtual ~CUdfLogFrame();

public:
	virtual void OnFinalRelease();

	CStatusBarXP  m_wndStatusBar;
protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_MENUXP()
public:
	CUdfLogRightView* m_pRightView;
	CUdfLogLeftView*  m_pLeftView;
	CUdfSplitterWnd   m_wndSplitter;
	CMenu			  m_wndMenu;	
protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnCmdLogQuery();
	afx_msg void OnCmdLogExport();
	afx_msg void OnCmdLogDeleteAll();
	afx_msg void OnCmdLogDelete1MonthPre();
	afx_msg void OnCmdLogDelete2MonthPre();
	afx_msg void OnCmdLogRefresh();
	afx_msg void OnLogFrameExit();
	afx_msg void OnLogOptCfg();						//日志系统参数配置
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
public:
	void SetStatusBarText(int indexBar,CString strText);

protected:
	virtual void OnUpdateFrameMenu(HMENU hMenuAlt);
public:
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
};


