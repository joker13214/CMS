#pragma once
#include "afxwin.h"
#include "XpStyle/ButtonXP.h"
#include "XpStyle/ComboBoxXP.h"


// CRtuAttrAlarmState 对话框
class CRtuPropertyDlg;
class CRtuAttrAlarmState : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrAlarmState)

public:
	CRtuAttrAlarmState(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRtuAttrAlarmState();

// 对话框数据
	enum { IDD = IDD_RTU_ALARMSTATE };
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
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void  SetWndsEnable(BOOL blEnable = TRUE);			//设置控件等是否可用
public:
	CComboBoxXP m_wndExAlarmCmb;
	CComboBoxXP m_wndRtuStateCmb;
	CComboBoxXP m_wndAlarmCmb;
	CButtonXP m_btnAlarmCall;
	CButtonXP m_btnExAlarmCall;
	CEdit     m_Edit_AlarmValue;
	CEdit     m_Edit_ExAlarmValue;
	CEdit     m_Edit_StateValue;
public:
	afx_msg void OnCbnSelchangeCmbRtualarm();
	afx_msg void OnCbnSelchangeCmbExalarm();
	afx_msg void OnCbnSelchangeCmbRtustate();
	afx_msg void OnBnClickExAlarmCall();
	afx_msg void OnBnClickAlarmCall();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
