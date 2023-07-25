#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CRtuAttrSyqcfg1 对话框

class CRtuAttrSyqcfg1 : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrSyqcfg1)

public:
	CRtuAttrSyqcfg1(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRtuAttrSyqcfg1();

// 对话框数据
	enum { IDD = IDD_DLG_SYQ_CFG1 };

	virtual BOOL OnInitDialog();
	void SetParentDlg(CRtuPropertyDlg* pDlg);
	void Update();
	void Updatedata(int sel);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBoxXP m_commtype1;
	CComboBoxXP m_commtype2;
	CComboBoxXP m_commtype3;
	CComboBoxXP m_commtype4;
	//CIPAddressCtrl m_str_ipaddress1;
	CEdit m_Address1;
	CEdit m_Address2;
	CEdit m_Address3;
	CEdit m_Address4;
	CEdit m_Password;
public:
//	CString m_strAddress1;
//	CString m_strAddress2;
	//CString m_strAddress3;
//	CString m_strAddress4;

public:
	afx_msg void OnBnClickedBtnCfg1();
	afx_msg void OnBnClickedBtnRead1();
	afx_msg void OnBnClickedBtnCfg2();
	afx_msg void OnBnClickedBtnRead2();
	afx_msg void OnBnClickedBtnCfg3();
	afx_msg void OnBnClickedBtnRead3();
	afx_msg void OnBnClickedBtnCfg4();
	afx_msg void OnBnClickedBtnRead4();
	afx_msg void OnEnChangeEditAddress1();
	afx_msg void OnBnClickedBtnWritePassword();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	CButtonXP m_BTN_Read1;
	CButtonXP m_BTN_Read2;
	CButtonXP m_BTN_Read3;
	CButtonXP m_BTN_Read4;
	CButtonXP m_BTN_Cfg1;
	CButtonXP m_BTN_Cfg2;
	CButtonXP m_BTN_Cfg3;
	CButtonXP m_BTN_Cfg4;
	CButtonXP m_BTN_WritePassword;

public:
	CRtuPropertyDlg* m_pParentDlg;				//TabCtrl父窗口
	CRtuStation*     m_pSubStation;				//当前的RtuStation对象
	//int m_iPassword;
	//short m_centercode;
	//DWORD m_syqStationCode;
	
	BOOL  m_bResult;

public:
	void  SetEnable(bool b);
    void  UpdateDlgMessage(int attr, bool result);

public:
	CEdit m_ECenterCode;
	CEdit m_EStCode;
};
