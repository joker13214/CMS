#pragma once
#include "afxwin.h"


// CUserEditPwdDlg 对话框

class CUserEditPwdDlg : public CDialog
{
	DECLARE_DYNAMIC(CUserEditPwdDlg)

public:
	CUserEditPwdDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUserEditPwdDlg();

// 对话框数据
	enum { IDD = IDD_DLG_EDITPWD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancel();
public:
	CButtonXP m_btnOK;
	CButtonXP m_BtnCancel;
	CString m_strName;
	CString m_strOldPwd;
	CString m_strNewPwd;
	CString m_strReNewPwd;
};
