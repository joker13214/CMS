#pragma once
#include "afxwin.h"


// CUdfNetInfoDlg �Ի���

class CUdfNetInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CUdfNetInfoDlg)

public:
	CUdfNetInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUdfNetInfoDlg();

// �Ի�������
	enum { IDD = IDD_DLG_CFGNETINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
