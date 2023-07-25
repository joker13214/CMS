#pragma once
#include "resource.h"
#include "GridCtrl/GridCtrl.h"
#include "UdfGlobalFUNC.h"
#include "RtuStation.h"

// CUdfRtuCfgRightView ��ͼ

class CUdfRtuCfgRightView : public CView
{
	DECLARE_DYNCREATE(CUdfRtuCfgRightView)

protected:
	CUdfRtuCfgRightView();				// ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CUdfRtuCfgRightView();
	enum GRID_HERDERS_COUNT
	{
		RTU_HEADER_COUNT = 10/*12*/,
		YC_HEADER_COUNT  = 11,
		YX_HEADER_COUNT  =  5,
		YK_HEADER_COUNT  =  5,
		ZD_HEADER_COUNT  = 10,
		
	};

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
protected:
	CWnd* m_pRtuCfgFrame;
public:
	void SetRtuCfgFrame(CWnd* pFrame);
	CWnd* GetRtuCfgFrame();
	void SetCaption(CString strCaption);      //���ñ�������ʾ��Ϣ
public:
	CGridCtrl m_wndGridCtrl;
	CStatic   m_wndCaption;
	CStatic   m_wndBackGroud;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLClickGridCtrl(NMHDR *pNotifyStruct,LRESULT *pResult);
	afx_msg void OnLDbClickGridCtrl(NMHDR *pNotifyStruct,LRESULT *pResult);
	afx_msg void OnRClickGridCtrl(NMHDR *pNotifyStruct,LRESULT *pResult);

	afx_msg void OnRtuSelAllItem();
	afx_msg void OnRtuNoSelAllItem();
	afx_msg void OnRtuAddOneItem();
	afx_msg void OnRtuDelItem();
	afx_msg void OnRtuDelSelItem();
	afx_msg void OnUpdateUIRtuDelSelItem(CCmdUI* pCmdUI);
	afx_msg void OnRtuProItem();

	afx_msg void  OnYcSelAllItem();
	afx_msg void  OnYcNoSelAllItem();
	afx_msg void  OnYcAddOneItem();
	afx_msg void  OnYcDelSelItem();
	afx_msg void  OnYcDelItem();
	afx_msg void  OnYcProItem();

	afx_msg void  OnYxSelAllItem();
	afx_msg void  OnYxNoSelAllItem();
	afx_msg void  OnYxAddOneItem();
	afx_msg void  OnYxDelSelItem();
	afx_msg void  OnYxDelItem();
	afx_msg void  OnYxProItem();

	afx_msg void  OnYkSelAllItem();
	afx_msg void  OnYkNoSelAllItem();
	afx_msg void  OnYkAddOneItem();
	afx_msg void  OnYkDelSelItem();
	afx_msg void  OnYkDelItem();
	afx_msg void  OnYkProItem();


	afx_msg void  OnZdSelAllItem();
	afx_msg void  OnZdNoSelAllItem();
	afx_msg void  OnZdAddOneItem();
	afx_msg void  OnZdDelSelItem();
	afx_msg void  OnZdDelItem();
	afx_msg void  OnZdProItem();

public:
	void InitGridCtrlHeaders(GRIDCTRL_STYLE xType);   //��ʼ��MFCGridCtrl�ı����� xTypeΪ��ʾ����ģʽ
	void InitShowStInfoHeaders();
	void InitShowYcInfoHeaders();
	void InitShowYxInfoHeaders();
	void InitShowYkInfoHeaders();
	void InitShowZdInfoHeaders();

    //��ʾRtuվ����Ϣ
	void ShowRtuInfor(GRIDCTRL_STYLE xType, CRtuStation* pStation = NULL);

	//��ϸ��ʾRTUվ����Ϣ��nRowָ����Ҫ��ʾ����
	void ShowRtuInfor(CRtuStation*  pSub,int nRow);
	void ShowRtuInfor(CRtuYxObject* pYx, int nRow);
	void ShowRtuInfor(CRtuYcObject* pYc, int nRow);
	void ShowRtuInfor(CRtuYkObject* pYk, int nRow);
	void ShowRtuInfor(CRtuZdObject* pZd, int nRow);

	//Left Mouse button Click
    void OnRtuObjectLClick(NM_GRIDVIEW* pItem);
	void OnYcObjectLClick(NM_GRIDVIEW* pItem);
	void OnYxObjectLClick(NM_GRIDVIEW* pItem);
	void OnYkObjectLClick(NM_GRIDVIEW* pItem);
	void OnZdObjectLClick(NM_GRIDVIEW* pItem);

	//Left mouse button double click
	void OnRtuObjectDbClick(NM_GRIDVIEW* pItem);
	void OnYcObjectDbClick(NM_GRIDVIEW* pItem);
	void OnYxObjectDbClick(NM_GRIDVIEW* pItem);
	void OnYkObjectDbClick(NM_GRIDVIEW* pItem);
	void OnZdObjectDbClick(NM_GRIDVIEW* pItem);

	//Right mouse button click
	void OnRtuObjectRClick(NM_GRIDVIEW* pItem);
	void OnYcObjectRClick(NM_GRIDVIEW* pItem);
	void OnYxObjectRClick(NM_GRIDVIEW* pItem);
	void OnYkObjectRClick(NM_GRIDVIEW* pItem);
	void OnZdObjectRClick(NM_GRIDVIEW* pItem);
	
	void SelectAllItem();							//���������0��0�� ѡ���ȡ��ѡ��ȫ������
	void SelectAllItem(BOOL blSeled);				//ȫѡ����ȫȡ�� �ɲ���blSeled��ֵ������
	void SelectOneItem(int nRow);					//ѡ���ȡ��һ������
public:
	CRtuStation* m_pCurStation;						//��ǰ������վ����
	GRIDCTRL_STYLE m_xGridCtrlStyle;				//��ǰGridCtrl ��ʾģʽstyle

	void SetGridCtrlStyle(GRIDCTRL_STYLE xType);
	GRIDCTRL_STYLE GetGridCtrlStyle();


	//GUI����,�������ֶ���

	bool UpdateRtuObject(CRtuStation* pSub);	//�޸�RTUվ����
	bool UpdateYcObject(CRtuYcObject* pYc);		//�޸�Yc����
	bool UpdateYxObject(CRtuYxObject* pYx);		//�޸�Yx����
	bool UpdateYkObject(CRtuYkObject* pYk);		//�޸�Yk����
	bool UpdateZdObject(CRtuZdObject* pZd);		//�޸�Zd����

	//GUI����,�������ֶ���

	bool DeleteRtuObject(CRtuStation* pSub);	//ɾ��RTUվ����
	bool DeleteYcObject(CRtuYcObject* pYc);		//ɾ��Yc����
	bool DeleteYxObject(CRtuYxObject* pYx);		//ɾ��Yx����
	bool DeleteYkObject(CRtuYkObject* pYk);		//ɾ��Yk����
	bool DeleteZdObject(CRtuZdObject* pZd);		//ɾ��Zd����

	//GUI����,�������ֶ���

	bool AddRtuObject();						//���RTUվ����
	bool AddYcObject(CRtuStation* pSub);		//��վRtu���yc����
	bool AddYxObject(CRtuStation* pSub);		//��վRtu���yx����
	bool AddYkObject(CRtuStation* pSub);		//��վRtu���yk����
	bool AddZdObject(CRtuStation* pSub);		//��վRtu���Zd����

private:
	CRtuStation*	m_pSel_RtuStation;           //��ǰѡ���rtuվ����
	CRtuYcObject*	m_pSel_YcObject;             //��ǰѡ���yc����
	CRtuYxObject*   m_pSel_YxObject;             //��ǰѡ���Yx����
    CRtuYkObject*   m_pSel_YkObject;             //��ǰѡ���Yk����
	CRtuZdObject*   m_pSel_ZdObject;             //��ǰѡ���Zd����
	void ClearSelectObject();                    //���ѡ������ָ�룬ȫ����ΪNULL  
	int  GetSelectedItemsCount();                //��ȡ�Ѿ�ѡ��������
};


