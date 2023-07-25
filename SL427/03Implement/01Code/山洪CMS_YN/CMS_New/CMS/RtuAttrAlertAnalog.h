#pragma once
#include "afxwin.h"
#include "XpStyle/ButtonXP.h"
#include "XpStyle/ComboBoxXP.h"
#include "GridCtrl/GridCtrl.h"


// CRtuAttrAlertAnalog 对话框
class CRtuStation;
class CRtuPropertyDlg;
class CRtuAttrAlertAnalog : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrAlertAnalog)

public:
	CRtuAttrAlertAnalog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRtuAttrAlertAnalog();

// 对话框数据
	enum { IDD = IDD_RTU_ALERTMONI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

protected:
	CRtuPropertyDlg* m_pParentDlg;                      //TabCtrl的父窗口
	CRtuStation*     m_pSubStation;						//此窗口当前的RtuStation对象
	BOOL      m_blResult;                       //召/设置参数成功与否标志
public:
	void  Update();							  //显示信息更新		
	void  SetParentDlg(CRtuPropertyDlg* pParent) {m_pParentDlg = pParent;}
	void  UpdateDlg(int attrType,BOOL blResult);
	void  KillAllTimers();
public:


	CEdit m_wndAnalogPCount;
	CComboBoxXP m_wndAnalogCmb;
	CButtonXP m_btnAnalogLCall;
	CButtonXP m_btnAnalogLSet;
	CStatic m_wndAnalogCaption;
	CButtonXP m_btnAnalogOK;
	int m_nAnalogCount;
	CGridCtrl m_wndAnalogGrid;
	CButtonXP m_btnQueryAReal;
public:
	virtual BOOL OnInitDialog();
	void    FillAnalogGridCtrl();
	void    SetWndCtrlEnable(BOOL blEnable);
public:
	afx_msg void OnCbnSelchangeComboAnalohpara();
	afx_msg void OnBnClickedBtnAnaloglcall();
	afx_msg void OnBnClickedBtnAnaloglset();
	afx_msg void OnBnClickedBtnAnalogok();
	afx_msg void OnBnClickBtnQueryAReal();
	afx_msg void OnEndEditAnalogGrid(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	
};
