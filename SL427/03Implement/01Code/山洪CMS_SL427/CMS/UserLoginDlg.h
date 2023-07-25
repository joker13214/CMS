#pragma once
#include "afxwin.h"
#include "XpStyle/ButtonXP.h"
#include "XpStyle/ComboBoxXP.h"

// CUserLoginDlg 对话框
class CUserLoginDlg : public CDialog
{
	DECLARE_DYNAMIC(CUserLoginDlg)

public:
	CUserLoginDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUserLoginDlg();

// 对话框数据
	enum { IDD = IDD_DLG_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBoxXP m_wndUsersCmb;
	CString m_sUserID;
	CString m_sPwd;
	CButtonXP m_btnOK;
	CButtonXP m_btnCancel;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnCbnSelchangeCmbUsers();
public:
	virtual BOOL OnInitDialog();
	void    UpdateDailog();
public:
	CEdit m_EditUserID;
	CEdit m_EditPwd;
};
