#pragma once
#include "afxwin.h"


// CUdfComInfoDlg �Ի���
//������Ϣ���ô���
class CUdfComInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CUdfComInfoDlg)

public:
	CUdfComInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUdfComInfoDlg();

// �Ի�������
	enum { IDD = IDD_DLG_CFGCOMINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CComboBoxXP m_wndCmbComNo;
	CComboBoxXP m_wndCmbBaund;
	CComboBoxXP m_wndCmbDataBits;
	CComboBoxXP m_wndCmbStopBits;
	CComboBoxXP m_wndCmbParirty;


	CString m_strComNo;
	int     m_nBaund ;
	BYTE    m_nDataBits;
	BYTE    m_nStopBits;
	BYTE    m_nParity;
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
public:
	CButtonXP m_btnOk;
public:
	CButtonXP m_btnCancel;
};
