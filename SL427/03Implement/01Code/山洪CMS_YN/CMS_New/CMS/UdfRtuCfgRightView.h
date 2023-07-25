#pragma once
#include "resource.h"
#include "GridCtrl/GridCtrl.h"
#include "UdfGlobalFUNC.h"
#include "RtuStation.h"

// CUdfRtuCfgRightView 视图

class CUdfRtuCfgRightView : public CView
{
	DECLARE_DYNCREATE(CUdfRtuCfgRightView)

protected:
	CUdfRtuCfgRightView();				// 动态创建所使用的受保护的构造函数
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
protected:
	CWnd* m_pRtuCfgFrame;
public:
	void SetRtuCfgFrame(CWnd* pFrame);
	CWnd* GetRtuCfgFrame();
	void SetCaption(CString strCaption);      //设置标题栏显示信息
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
	void InitGridCtrlHeaders(GRIDCTRL_STYLE xType);   //初始化MFCGridCtrl的标题栏 xType为显示数据模式
	void InitShowStInfoHeaders();
	void InitShowYcInfoHeaders();
	void InitShowYxInfoHeaders();
	void InitShowYkInfoHeaders();
	void InitShowZdInfoHeaders();

    //显示Rtu站点信息
	void ShowRtuInfor(GRIDCTRL_STYLE xType, CRtuStation* pStation = NULL);

	//详细显示RTU站点信息，nRow指定需要显示的行
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
	
	void SelectAllItem();							//点标题栏（0，0） 选择或取消选择全部对象
	void SelectAllItem(BOOL blSeled);				//全选或者全取消 由参数blSeled的值来决定
	void SelectOneItem(int nRow);					//选择或取消一个对象
public:
	CRtuStation* m_pCurStation;						//当前操作的站对象
	GRIDCTRL_STYLE m_xGridCtrlStyle;				//当前GridCtrl 显示模式style

	void SetGridCtrlStyle(GRIDCTRL_STYLE xType);
	GRIDCTRL_STYLE GetGridCtrlStyle();


	//GUI界面,操作各种对象

	bool UpdateRtuObject(CRtuStation* pSub);	//修改RTU站对象
	bool UpdateYcObject(CRtuYcObject* pYc);		//修改Yc对象
	bool UpdateYxObject(CRtuYxObject* pYx);		//修改Yx对象
	bool UpdateYkObject(CRtuYkObject* pYk);		//修改Yk对象
	bool UpdateZdObject(CRtuZdObject* pZd);		//修改Zd对象

	//GUI界面,操作各种对象

	bool DeleteRtuObject(CRtuStation* pSub);	//删除RTU站对象
	bool DeleteYcObject(CRtuYcObject* pYc);		//删除Yc对象
	bool DeleteYxObject(CRtuYxObject* pYx);		//删除Yx对象
	bool DeleteYkObject(CRtuYkObject* pYk);		//删除Yk对象
	bool DeleteZdObject(CRtuZdObject* pZd);		//删除Zd对象

	//GUI界面,操作各种对象

	bool AddRtuObject();						//添加RTU站对象
	bool AddYcObject(CRtuStation* pSub);		//向站Rtu添加yc对象
	bool AddYxObject(CRtuStation* pSub);		//向站Rtu添加yx对象
	bool AddYkObject(CRtuStation* pSub);		//向站Rtu添加yk对象
	bool AddZdObject(CRtuStation* pSub);		//向站Rtu添加Zd对象

private:
	CRtuStation*	m_pSel_RtuStation;           //当前选择的rtu站对象
	CRtuYcObject*	m_pSel_YcObject;             //当前选择的yc对象
	CRtuYxObject*   m_pSel_YxObject;             //当前选择的Yx对象
    CRtuYkObject*   m_pSel_YkObject;             //当前选择的Yk对象
	CRtuZdObject*   m_pSel_ZdObject;             //当前选择的Zd对象
	void ClearSelectObject();                    //清空选择对象的指针，全设置为NULL  
	int  GetSelectedItemsCount();                //获取已经选择的项个数
};


