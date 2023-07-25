#pragma once
#include "afxwin.h"
#include "XpStyle/ButtonXP.h"
#include "XpStyle/ComboBoxXP.h"
#include "GridCtrl/GridCtrl.h"


// CRtuAttrTemperature 对话框
class CRtuStation;
class CRtuPropertyDlg;
class CRtuAttrTemperature : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrTemperature)

public:
	CRtuAttrTemperature(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRtuAttrTemperature();

// 对话框数据
	enum { IDD = IDD_RTU_TEMPERATRUE };

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


	CEdit m_wndTemperaPCount;
	CComboBoxXP m_wndTemperaCmb;
	CButtonXP m_btnTemperaLCall;
	CButtonXP m_btnTemperaLSet;
	CStatic m_wndTemperaCaption;
	CButtonXP m_btnTemperaOK;
	int m_nTemperaCount;
	CGridCtrl m_wndTemperaGrid;
	CButtonXP m_btnQueryAReal;
public:
	virtual BOOL OnInitDialog();
	void    FillTemperaGridCtrl();
	void    SetWndCtrlEnable(BOOL blEnable);
public:
	afx_msg void OnCbnSelchangeComboTemperapara();
	afx_msg void OnBnClickedBtnTemperalcall();
	afx_msg void OnBnClickedBtnTemperalset();
	afx_msg void OnBnClickedBtnTemperaok();
	afx_msg void OnBnClickBtnQueryAReal();
	afx_msg void OnEndEditTemperaGrid(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	
};
