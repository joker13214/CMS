#pragma once
#include "afxwin.h"


// CUdfCfgODBCDlg �Ի���
class CUdfCfgODBCDlg : public CDialog
{
	DECLARE_DYNAMIC(CUdfCfgODBCDlg)

public:
	CUdfCfgODBCDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUdfCfgODBCDlg();

// �Ի�������
	enum { IDD = IDD_DLG_CFGDSN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	// ODBC����Դ����
	CString m_strDSN;

	// ODBC����Դ��¼�û�ID
	CString m_strUID;

	// ODBC����Դ��¼����
	CString m_strPWD;

	//Data Interface
	BYTE    m_nDataInterface;

	BYTE    m_nDataIType;

	BYTE    m_nDatabase;

protected:
	BOOL SaveOdbcOptions();              //����Odbc������Ϣ
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
