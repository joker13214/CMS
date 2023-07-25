// CMSView.h : CCMSView ��Ľӿ�
//


#pragma once

#include "GridCtrl/GridCtrl.h"

class CCMSView : public CView
{
protected: // �������л�����
	CCMSView();
	DECLARE_DYNCREATE(CCMSView)

// ����
public:
	CCMSDoc* GetDocument() const;

// ����
public:
	//CGridCtrl m_wndGridCtrl;
	//CBtnDataBase	m_DBClockCheck[10];

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CCMSView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // CMSView.cpp �еĵ��԰汾
inline CCMSDoc* CCMSView::GetDocument() const
   { return reinterpret_cast<CCMSDoc*>(m_pDocument); }
#endif

