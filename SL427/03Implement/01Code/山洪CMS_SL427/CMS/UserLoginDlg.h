#pragma once
#include "afxwin.h"
#include "XpStyle/ButtonXP.h"
#include "XpStyle/ComboBoxXP.h"

// CUserLoginDlg �Ի���
class CUserLoginDlg : public CDialog
{
	DECLARE_DYNAMIC(CUserLoginDlg)

public:
	CUserLoginDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUserLoginDlg();

// �Ի�������
	enum { IDD = IDD_DLG_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
