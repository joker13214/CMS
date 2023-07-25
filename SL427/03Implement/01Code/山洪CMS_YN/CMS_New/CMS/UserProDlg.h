#pragma once
#include "afxwin.h"
#include "XpStyle/ButtonXP.h"
#include "XpStyle/ComboBoxXP.h"
#include "UserObject.h"


// CUserProDlg 对话框
class CUserProDlg : public CDialog
{
	DECLARE_DYNAMIC(CUserProDlg)

public:
	CUserProDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUserProDlg();

// 对话框数据
	enum { IDD = IDD_DLG_USERPRO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CButtonXP m_btnOK;
	CButtonXP m_btnCancel;
	CComboBoxXP m_wndUserTypeCmb;
	CComboBoxXP m_wndUserOnDutyCmb;
	CString m_sName;
	CString m_sID;
	CString m_sPwd;
	BYTE    m_bType;
	BYTE    m_bOnDuty;
protected:
	CUserObject* m_pUserObj;
public:
	virtual BOOL OnInitDialog();
	bool         CheckUserName(CString _Name);
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnCbnSelchangeCmbUsertype();
	afx_msg void OnCbnSelchangeCmbUseronduty();
public:
	void SetUserObject(CUserObject* pUser);
public:
	afx_msg void OnEnChangeEditUsername();
	afx_msg void OnEnChangeEditUserid();
};
