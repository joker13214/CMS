#pragma once


// CUdfLogRightView ��ͼ
#include "UdfListCtrl.h"
#include "UdfLogQueryDlg.h"

class CUdfLogRightView : public CView
{
	DECLARE_DYNCREATE(CUdfLogRightView)

public:
	CUdfLogRightView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CUdfLogRightView();

public:
	virtual void OnFinalRelease();
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_MENUXP()
public:
	CUdfListCtrl  m_pListCtrl;
	CStatic       m_wndBackGroud;
	CStatic       m_wndCaption;

public:
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnDBClickLogMessage(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRButtonClickLogMessage(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLClickLogMessage(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdLogQuery();
	afx_msg void OnCmdLogDelete();
	afx_msg void OnCmdLogExport();
	afx_msg void OnCmdLogDeleteAll();
	afx_msg void OnCmdLogDelete1MonthPre();
	afx_msg void OnCmdLogDelete2MonthPre();
	afx_msg void OnCmdLogDeleteCustom();
	afx_msg void OnCmdLogRefresh();
public:
	//��ʾ��־��Ϣ
	BOOL ShowLogInformation(_LOG_MESSAGE_TYPE _nType,CUdfLogQueryDlg::QUERY_LOG_TYPE queryLogType= CUdfLogQueryDlg::QUERY_LOGTYPE_ALL,PSYSTEMTIME _beginTime=NULL, PSYSTEMTIME _endTime = NULL);

public:
	//��ѯ��ʼ������ʱ��
   SYSTEMTIME m_queryBeginTime; 
   SYSTEMTIME m_queryEndTime;
   CUdfLogQueryDlg::QUERY_LOG_TYPE m_queryLogType;
protected:
	CWnd* m_pFrameWnd;     //ָ��CUdfLogFrame;


public:
	void  SetFrameWnd(CWnd* pFrameWnd);
	CWnd* GetFrameWnd();
	void  SetCaption(CString strCaption);  //���ñ�������ʾ����Ϣ
	BOOL  InvalidateListCtrl();


};


