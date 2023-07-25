#pragma once
#include "afxwin.h"
#include "XpStyle/ButtonXP.h"
#include "XpStyle/ComboBoxXP.h"
#include "GridCtrl/GridCtrl.h"

// CRtuAttrWaterQuality 对话框

class CRtuPropertyDlg;
class CRtuAttrWaterQuality : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrWaterQuality)

public:
	CRtuAttrWaterQuality(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRtuAttrWaterQuality();

// 对话框数据
	enum { IDD = IDD_RTU_WQUALITY };
protected:
	//所有的Tab页窗口都必须具备的成员函数和对象
	CRtuPropertyDlg* m_pParentDlg;                      //TabCtrl的父窗口
	CRtuStation*     m_pSubStation;						//此窗口当前的RtuStation对象
	BOOL			m_blResult;                         //召.设置相关参数是否成功的标志

	void  InitGridItems();                              //初始化GridCtrl的Items
	void  SetWndsEnable(BOOL blEnable = TRUE);			//设置按钮是否可用
public:
	void SetParentDlg(CRtuPropertyDlg* pDlg);
	void Update();										//更新界面的显示信息
	void UpdateDlg(int attrType,BOOL blResult);			//接收到报文后、更新界面
	void KillAllTimers();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBoxXP m_wndWQualityTypeCmd;
	CButtonXP m_btnWQUpCall;
	CButtonXP m_btnWQUpSet;
	CButtonXP m_btnWQDownCall;
	CButtonXP m_btnWQDownSet;
	CButtonXP m_btnWQDataCall;
	CGridCtrl m_wndGridCtrl;
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnCbnSelchangeCmbWquality();
	afx_msg void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnBnClickedBtnWqupcall();
	afx_msg void OnBnClickedBtnWqupset();
	afx_msg void OnBnClickedBtnWqdowncall();
	afx_msg void OnBnClickedBtnWqdownset();
	afx_msg void OnBnClickedBtnWqdatacall();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
