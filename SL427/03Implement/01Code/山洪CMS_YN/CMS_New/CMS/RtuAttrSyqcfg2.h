#pragma once
#include "afxwin.h"


// CRtuAttrSyqcfg2 �Ի���

class CRtuAttrSyqcfg2 : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrSyqcfg2)

public:
	CRtuAttrSyqcfg2(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRtuAttrSyqcfg2();

// �Ի�������
	enum { IDD = IDD_DLG_SYQ_CFG2 };
	virtual BOOL OnInitDialog();
	void SetParentDlg(CRtuPropertyDlg* pDlg);
	void Update();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CRtuPropertyDlg* m_pParentDlg;				//TabCtrl������
	CRtuStation*     m_pSubStation;				//��ǰ��RtuStation����
public:
	// //��ʱ�����
	CEdit m_cfg_20;
	// //��ʱ��ʱ����
	CEdit m_cfg_21;
	CComboBoxXP m_cfg_22;
	CComboBoxXP m_cfg_23;
	CEdit m_cfg_24;
	CEdit m_cfg_25;
	CEdit m_cfg_26;
	CEdit m_cfg_27;
	CEdit m_cfg_28;
	CEdit m_cfg_29;
	CComboBoxXP m_cfg_2A;
	CEdit m_cfg_2B;
	CEdit m_cfg_2C;
	CEdit m_cfg_2D;
	CEdit m_cfg_2E;
	CEdit m_cfg_2F;
public:
	afx_msg void OnBnClickedBtnRead();
	afx_msg void OnBnClickedBtnWrite();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	CButtonXP m_ReadBtn;
	CButtonXP m_WriteBtn;


	BOOL  m_bResult;


	void  SetEnable(bool b);
	void  UpdateDlgMessage(int attr, bool result);
};
