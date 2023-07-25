#pragma once
#include "RtuStation.h"
#include "RtuYcObject.h"

// CUdfRtuCfgLeftView 视图

class CUdfRtuCfgLeftView : public CView
{
	DECLARE_DYNCREATE(CUdfRtuCfgLeftView)

protected:
	CUdfRtuCfgLeftView();           // 动态创建所使用的受保护的构造函数
	virtual ~CUdfRtuCfgLeftView();

public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_MENUXP()
public:
	CTreeCtrl m_wndTreeCtrl;
	CImageList m_wndImageList;

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRtuTreeClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRtuTreeRClick(NMHDR* pNMHDR,LRESULT* pResult);
	afx_msg void OnRtuCfgAddRTUSt();
	afx_msg void OnRtuCfgYcAdd();
	afx_msg void OnRtuCfgYxAdd();
	afx_msg void OnRtuCfgYkAdd();
	afx_msg void OnRtuCfgZdAdd();
	afx_msg void OnRtuCfgDelRtuSt();
	afx_msg void OnRtuCfgRtuStPro();

public:
	bool InitTreeCtrlItem();
	void SetRtuCfgFrame(CWnd* pFrame);
	CWnd* GetRtuCfgFrame();
	bool AddStTreeItem(HTREEITEM pParent,CRtuStation* pStation);

	//return   true:rtu站item  false:不是rtu站item 
    bool blRtuStTreeItem(HTREEITEM pItem);   //判断Item是否是rtu站对象 
	bool blSelectItemRtuSt();                //判断已选择的Item是否是rtu站对象


protected:
	HTREEITEM m_pTreeItemRoot; 
	CWnd*     m_pRtuCfgFrame;
	CRtuStation* m_pCurStation; //当前正在操作的RTU站对象
public:
		HTREEITEM m_pSelectItem;    //当前选择的Tree item
};


