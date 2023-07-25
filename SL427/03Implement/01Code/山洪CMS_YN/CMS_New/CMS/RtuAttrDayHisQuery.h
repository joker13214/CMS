#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include "XpStyle/ButtonXP.h"
#include "XpStyle/ComboBoxXP.h"
#include "GridCtrl/GridCtrl.h"

// CRtuAttrDayHisQuery 对话框
class CRtuPropertyDlg;
class CRtuAttrDayHisQuery : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrDayHisQuery)

public:
	CRtuAttrDayHisQuery(CWnd* pParent = NULL);			// 标准构造函数
	virtual ~CRtuAttrDayHisQuery();

// 对话框数据
	enum { IDD = IDD_RTU_DAYHISQUERY };
protected:
	CRtuPropertyDlg* m_pParentDlg;                      //TabCtrl的父窗口
	CRtuStation*     m_pSubStation;						//此窗口当前的RtuStation对象
	BOOL			 m_blResult;                        //召.设置相关参数是否成功的标志
public:
	void SetParentDlg(CRtuPropertyDlg* pDlg);
	void Update();										//更新界面的显示信息
	void UpdateDlg(BOOL blResult,CString strData);		//接收到报文后、更新界面
	void KillAllTimers();
	void SetWndsEnable(BOOL blEnable);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBoxXP m_wndZBTypeCmb;						    //整点/半点选择
	CDateTimeCtrl m_wndQueryDate;						//查询时间
	CComboBoxXP m_wndDataTypeCmb;						//查询参数类型
	CComboBoxXP m_wndQueryNoCmb;						//查询序号
	CButtonXP	m_btnQueryHis;
	//CGridCtrl	m_wndGridCtrl;
protected:
	BYTE m_bZBtype;										//整半点值 D7-D6 '10', 2 整点 '01' 1 半点数据
	BYTE m_bDataType;									//数据类型
	BYTE m_bQueryNo;									//查询编号
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedBtnQueryhis();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnCbnSelchangeCmbZbtype();
	afx_msg void OnCbnSelchangeCmbDatatype();
	afx_msg void OnCbnSelchangeCmbQueryno();
public:
	CEdit m_EditDataInfo;
};
