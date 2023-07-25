#pragma once
#include "afxdtctl.h"
#include "afxwin.h"


// CRtuAttrHisQuery 对话框

class CRtuAttrHisQuery : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrHisQuery)

public:
	CRtuAttrHisQuery(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRtuAttrHisQuery();

	virtual void OnFinalRelease();
	void  Update();							  //显示信息更新		
	void  SetParentDlg(CRtuPropertyDlg* pParent);

// 对话框数据
	enum { IDD = IDD_RTU_HISQUERY };
protected:
	CRtuPropertyDlg* m_pParentDlg;                      //TabCtrl的父窗口
	CRtuStation*     m_pSubStation;						//此窗口当前的RtuStation对象
	CGridCtrl        m_wndGridCtrl;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	virtual BOOL OnInitDialog();
	void InitGridHeader();
public:
	CDateTimeCtrl m_wndBeginTimeCtrl;
	CDateTimeCtrl m_wndEndTimeCtrl;
	CComboBoxXP m_wndQueryTypeCtrl;
	CButtonXP m_BtnQuery;
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnCbnSelchangeComboType();
	void UpdateQueryTime();
private:
	CString m_sBegin;
	CString m_sEnd;
	void AddMessageToGridCtrl(CString strTime,CString strReport);
};
