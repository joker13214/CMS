#pragma once
#include "afxwin.h"


// CUdfCfgODBCDlg 对话框
class CUdfCfgODBCDlg : public CDialog
{
	DECLARE_DYNAMIC(CUdfCfgODBCDlg)

public:
	CUdfCfgODBCDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUdfCfgODBCDlg();

// 对话框数据
	enum { IDD = IDD_DLG_CFGDSN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	// ODBC数据源名称
	CString m_strDSN;

	// ODBC数据源登录用户ID
	CString m_strUID;

	// ODBC数据源登录口令
	CString m_strPWD;

	//Data Interface
	BYTE    m_nDataInterface;

	BYTE    m_nDataIType;

	BYTE    m_nDatabase;

protected:
	BOOL SaveOdbcOptions();              //保存Odbc配置信息
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	void         Show(int nCmdShow);
public:
	afx_msg void OnClose();
	afx_msg void OnCbnSelchangeCmbDatainterface();
	afx_msg void OnCbnSelchangeCmbItype();
public:
	CButtonXP m_BtnOK;
	CButtonXP m_btnCancel;
	CComboBoxXP m_wndCbmDataInterface;
	CComboBoxXP m_wndComboBoxIType;
	CStatic m_wndStaticIType;
	CStatic m_wndStaticIName;
	CStatic m_wndStaticDatabase;
	CComboBoxXP m_wndCmbDatabase;
};
