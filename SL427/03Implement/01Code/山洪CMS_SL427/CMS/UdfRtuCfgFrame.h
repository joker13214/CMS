#pragma once


// CUdfRtuCfgFrame 框架
#include "UdfSplitterWnd.h"
#include "UdfRtuCfgLeftView.h"
#include "UdfRtuCfgRightView.h"


class CUdfRtuCfgFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CUdfRtuCfgFrame)
public:
	CUdfRtuCfgFrame();           // 动态创建所使用的受保护的构造函数
	virtual ~CUdfRtuCfgFrame();

	CUdfSplitterWnd m_wndSplitterWnd;
	//CStatusBar      m_wndStatusBar;
	CMenu           m_wndFrameMenu;

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_MENUXP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	CUdfRtuCfgLeftView*		m_pRtuCfgLeftView;
	CUdfRtuCfgRightView*	m_pRtuCfgRightView;
public:
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnRtuCfgExit();
	afx_msg void OnRtuCfgRtuStAdd();
	afx_msg void OnRtuCfgYcAdd();
	afx_msg void OnRtuCfgYxAdd();
	afx_msg void OnRtuCfgYkAdd();
	afx_msg void OnRtuCfgZdAdd();
	afx_msg void OnUpdateUIRtuCfgYcAdd(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUIRtuCfgYxAdd(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUIRtuCfgYkAdd(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUIRtuCfgZdAdd(CCmdUI* pCmdUI);

	afx_msg void OnRtuCfgGridNone();
	afx_msg void OnRtuCfgGridHorz();
	afx_msg void OnRtuCfgGridVert();
	afx_msg void OnRtuCfgGridBoth();
	afx_msg void OnUpdateUIRtuCfgGridNone(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUIRtuCfgGridHorz(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUIRtuCfgGridVert(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUIRtuCfgGridBoth(CCmdUI* pCmdUI);
public:
	void SetStatusBarText(int barIndex,CString strText);  //设置Status bar 显示信息

protected:
	virtual void OnUpdateFrameMenu(HMENU hMenuAlt);
public:
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
};


