#pragma once


// CCMSLeftView 视图

class CCMSLeftView : public CView
{
	DECLARE_DYNCREATE(CCMSLeftView)

protected:
	CCMSLeftView();           // 动态创建所使用的受保护的构造函数
	virtual ~CCMSLeftView();

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
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLeftClickTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRightClickTree(NMHDR* pNMHDR,LRESULT* pResult);
protected:
	CStatic		m_wndCaption;
	CStatic		m_wndBackGroud;
	CImageList	m_wndImageList;
public:
	CTreeCtrl	m_wndTreeCtrl;
	BOOL		InitTreeItem();			//生成树对象
	BOOL        RedrawTreeCtrl();     
};


