#pragma once
#include "afxwin.h"


// CUserEditPwdDlg �Ի���

class CUserEditPwdDlg : public CDialog
{
	DECLARE_DYNAMIC(CUserEditPwdDlg)

public:
	CUserEditPwdDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUserEditPwdDlg();

// �Ի�������
	enum { IDD = IDD_DLG_EDITPWD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
