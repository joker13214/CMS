#pragma once
#include "afxwin.h"


// CUdfComInfoDlg 对话框
//串口信息配置窗口
class CUdfComInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CUdfComInfoDlg)

public:
	CUdfComInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUdfComInfoDlg();

// 对话框数据
	enum { IDD = IDD_DLG_CFGCOMINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
