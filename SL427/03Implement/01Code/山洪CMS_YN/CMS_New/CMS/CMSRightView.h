#pragma once
#include "GridCtrl/GridCtrl.h"

// CCMSRightView ��ͼ

class CCMSRightView : public CView
{
	DECLARE_DYNCREATE(CCMSRightView)

protected:
	CCMSRightView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CCMSRightView();

public:
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ
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
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLClickGridCtrl(NMHDR *pNotifyStruct,LRESULT *pResult);
	afx_msg void OnLDbClickGridCtrl(NMHDR *pNotifyStruct,LRESULT *pResult);
	afx_msg void OnRClickGridCtrl(NMHDR* pNotifyStruct,LRESULT *pResutl);
	afx_msg void OnGridLineNone();
	afx_msg void OnGridLineVert();
	afx_msg void OnGridLineHorz();
	afx_msg void OnGridLineBoth();

	afx_msg void OnUpdateUIGridLineNone(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUIGridLineVert(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUIGridLineHorz(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUIGridLineBoth(CCmdUI* pCmdUI);

protected:
	CStatic		m_wndCaption;
	CStatic		m_wndBackGroud;
	CImageList	m_wndImageList;
	CMutex		m_UpdateComm_Mutex;
public:
	CGridCtrl m_wndListCtrl;
	void     InitGridCtrl();
	void     SetCaption(CString strCaption);          //���ñ�������Ϣ
	void     UpdateCommState(CRtuStation* pSub);
};


