#pragma once
#include "afxwin.h"


// CRtuAttrPulse 对话框
class CRtuPropertyDlg;
class CRtuAttrPulse : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrPulse)

public:
	CRtuAttrPulse(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRtuAttrPulse();

// 对话框数据
	enum { IDD = IDD_RTU_PULSE };
protected:
	//所有的Tab页窗口都必须具备的成员函数和对象
	CRtuPropertyDlg* m_pParentDlg;                      //TabCtrl的父窗口
	CRtuStation*     m_pSubStation;						//此窗口当前的RtuStation对象
	BOOL			 m_blResult;                        //召.设置相关参数是否成功的标志

public:
	void SetParentDlg(CRtuPropertyDlg* pDlg);
	void Update();										//更新界面的显示信息
	void UpdateDlg(int attrType,BOOL blResult);			//接收到报文后、更新界面
	void KillAllTimers();

	void SetWndCtrlEnable(BOOL blEnable);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CButtonXP m_btnPulseQuery;
	CComboBoxXP m_wndPulseCmb;
	CGridCtrl m_wndGridCtrl;
public:
	afx_msg void OnBnClickedBtnQuerypulse();
	afx_msg void OnCbnSelchangeComboPulsepara();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	virtual BOOL OnInitDialog();
	void InitGridCtrl();
public:
	int m_nPulseCount;
	CEdit m_wndPulseCountEdit;
	CStatic m_wndPulseCaption;
};
