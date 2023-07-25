#pragma once
#include "RtuStation.h"
#include "RtuYcObject.h"

// CUdfRtuCfgLeftView ��ͼ

class CUdfRtuCfgLeftView : public CView
{
	DECLARE_DYNCREATE(CUdfRtuCfgLeftView)

protected:
	CUdfRtuCfgLeftView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CUdfRtuCfgLeftView();

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

	//return   true:rtuվitem  false:����rtuվitem 
    bool blRtuStTreeItem(HTREEITEM pItem);   //�ж�Item�Ƿ���rtuվ���� 
	bool blSelectItemRtuSt();                //�ж���ѡ���Item�Ƿ���rtuվ����


protected:
	HTREEITEM m_pTreeItemRoot; 
	CWnd*     m_pRtuCfgFrame;
	CRtuStation* m_pCurStation; //��ǰ���ڲ�����RTUվ����
public:
		HTREEITEM m_pSelectItem;    //��ǰѡ���Tree item
};


