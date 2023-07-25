#pragma once
#include "afxwin.h"


// CUdfNetInfoDlg 对话框

class CUdfNetInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CUdfNetInfoDlg)

public:
	CUdfNetInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUdfNetInfoDlg();

// 对话框数据
	enum { IDD = IDD_DLG_CFGNETINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
public:
	CString m_strIPAddr;
	int m_nPort;
public:
	CButtonXP m_btnOK;
public:
	CButtonXP m_btnCancel;
};
