#pragma once


// CUdfLogLeftView ��ͼ

class CUdfLogLeftView : public CView
{
	DECLARE_DYNCREATE(CUdfLogLeftView)

public:
	CUdfLogLeftView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CUdfLogLeftView();

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
	CTreeCtrl	m_pTreeCtrl;
	CImageList* m_pImageList;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnClkTreeCtrl(NMHDR* pNMHDR, LRESULT* pResult);


public:
	BOOL InitTreeView();

public:
	_LOG_MESSAGE_TYPE n_CurentType;

protected:
	CWnd* m_pFrameWnd;     //ָ��CUdfLogFrame;


public:
	void  SetFrameWnd(CWnd* pFrameWnd);
	CWnd* GetFrameWnd();
};


