#pragma once
#include "XpStyle/ButtonXP.h"
#include "XpStyle/ComboBoxXP.h"
#include "GridCtrl/GridCtrl.h"

// CRtuAttrWaterLevel 对话框
class CRtuPropertyDlg;
class CRtuAttrWaterLevel : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrWaterLevel)

public:
	CRtuAttrWaterLevel(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRtuAttrWaterLevel();

// 对话框数据
	enum { IDD = IDD_RTU_WATERLEVEL };
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
	void SetWndsEnable(BOOL blEnable);					//设置按钮是否可用
	void InitGridItems();								//GridCtrl显示
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_Edit_WaterLevelCount;
	CComboBoxXP m_wndWaterLevelCmb;
	CGridCtrl   m_wndGridCtrl;

	CButtonXP m_btnQueryWaterLevel;
	CButtonXP m_btnSetWaterLevel;
	CButtonXP m_btnQueryWaterLevelData;
	CButtonXP m_btnWaterLevelOK;

	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickQueryWaterLevel();
	afx_msg void OnBnClickQueryWaterLevelData();
	afx_msg void OnBnClickSetWaterLevel();
	afx_msg void OnBnClickWaterLevelOK();
	afx_msg void OnCbnSelchangeCmbWaterLevel();
	afx_msg void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
