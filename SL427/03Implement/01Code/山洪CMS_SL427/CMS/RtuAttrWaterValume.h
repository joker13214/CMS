#pragma once
#include "XpStyle/ButtonXP.h"
#include "XpStyle/ComboBoxXP.h"

// CRtuAttrWaterValume 对话框
class CRtuPropertyDlg;
class CRtuAttrWaterValume : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrWaterValume)

public:
	CRtuAttrWaterValume(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRtuAttrWaterValume();

// 对话框数据
	enum { IDD = IDD_RTU_WATERVALUME };

public:
	CEdit m_Edit_Czl;
	CEdit m_Edit_Remain;
	CEdit m_Edit_Alert;

	

	CButtonXP m_btnQueryCzl;
	CButtonXP m_btnSetCzl;
	CButtonXP m_btnQueryRemainAlert;
	CButtonXP m_btnSetRemainAlert;
protected:
	//所有的Tab页窗口都必须具备的成员函数和对象
	CRtuPropertyDlg* m_pParentDlg;                      //TabCtrl的父窗口
	CRtuStation*     m_pSubStation;						//此窗口当前的RtuStation对象
	BOOL			 m_blResult;                        //召.设置相关参数是否成功的标志
public:
	virtual BOOL OnInitDialog();
	void SetParentDlg(CRtuPropertyDlg* pDlg);
	void Update();										//更新界面的显示信息
	void UpdateDlg(int attrType,BOOL blResult);			//接收到报文后、更新界面
	void KillAllTimers();
	void SetWndsEnable(BOOL blEnable);					//设置按钮是否可用
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickQueryCzl();
	afx_msg void OnBnClickSetCzl();
	afx_msg void OnBnClickQueryRemainAlert();
	afx_msg void OnBnClickSetRemainAlert();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
