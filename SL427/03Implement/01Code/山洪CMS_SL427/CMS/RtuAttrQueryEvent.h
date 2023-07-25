#pragma once
#include "XpStyle/ButtonXP.h"
#include "GridCtrl/GridCtrl.h"

// CRtuAttrQueryEvent 对话框
class CRtuPropertyDlg;
class CRtuAttrQueryEvent : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrQueryEvent)

public:
	CRtuAttrQueryEvent(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRtuAttrQueryEvent();

// 对话框数据
	enum { IDD = IDD_RTU_QUERYEVENT };
	CRtuPropertyDlg* m_pParentDlg;                      //TabCtrl的父窗口
	CRtuStation*     m_pSubStation;						//此窗口当前的RtuStation对象
	BOOL			 m_blResult;                        //召.设置相关参数是否成功的标志
public:
	void SetParentDlg(CRtuPropertyDlg* pDlg);
	void Update();										//更新界面的显示信息
	void UpdateDlg(int attrType,BOOL blResult);			//接收到报文后、更新界面
	void KillAllTimers();
	void SetWndsEnable(BOOL blEnable);					//设置按钮是否可用
	void InitGridCtrl();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CGridCtrl m_wndGridCtrl;
	CButtonXP m_btnQueryEvent;

	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickBtnQueryEvent();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
