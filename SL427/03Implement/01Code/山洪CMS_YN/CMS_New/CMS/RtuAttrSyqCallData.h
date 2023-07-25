#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include "afxcmn.h"


// CRtuAttrSyqCalldata �Ի���

class CRtuAttrSyqCalldata : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrSyqCalldata)

public:
	CRtuAttrSyqCalldata(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRtuAttrSyqCalldata();

	virtual BOOL OnInitDialog();
	void SetParentDlg(CRtuPropertyDlg* pDlg);
	void Update();
// �Ի�������
	enum { IDD = IDD_DLG_SYQ_CALLDATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CButtonXP m_Btn_Query;
	CComboBoxXP m_Yaosu;
	CDateTimeCtrl m_DtBegin;
	CDateTimeCtrl m_DtEnd;


	CRtuPropertyDlg* m_pParentDlg;				//TabCtrl������
	CRtuStation*     m_pSubStation;				//��ǰ��RtuStation����

	void  UpdateDlgMessage(int attr, bool result);
	void  FillDataList();
public:
	afx_msg void OnBnClickedQuery();

	void  SetEnable(bool b);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	BOOL m_bResult;
public:
	CComboBoxXP	 m_time_step;
public:
	CStatic m_text;
public:
	afx_msg void OnCbnSelchangeComboTimeStep();
public:
	int m_nNumber;
public:
	CListCtrl m_listdata;
};
