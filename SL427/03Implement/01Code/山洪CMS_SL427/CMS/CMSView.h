// CMSView.h : CCMSView 类的接口
//


#pragma once

#include "GridCtrl/GridCtrl.h"

class CCMSView : public CView
{
protected: // 仅从序列化创建
	CCMSView();
	DECLARE_DYNCREATE(CCMSView)

// 属性
public:
	CCMSDoc* GetDocument() const;

// 操作
public:
	//CGridCtrl m_wndGridCtrl;
	//CBtnDataBase	m_DBClockCheck[10];

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CCMSView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // CMSView.cpp 中的调试版本
inline CCMSDoc* CCMSView::GetDocument() const
   { return reinterpret_cast<CCMSDoc*>(m_pDocument); }
#endif

