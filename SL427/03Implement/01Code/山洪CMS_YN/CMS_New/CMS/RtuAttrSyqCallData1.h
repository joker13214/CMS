#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CRtuAttrSyqCallData1 �Ի���

class CRtuAttrSyqCallData1 : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrSyqCallData1)

public:
	CRtuAttrSyqCallData1(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRtuAttrSyqCallData1();
	virtual BOOL OnInitDialog();
	void SetParentDlg(CRtuPropertyDlg* pDlg);
	void Update();

	void  UpdateDlgMessage(int attr, bool result);
	void  FillDataList1();
	void  FillDataList2();
	void  SetEnable(bool b);

	CRtuPropertyDlg* m_pParentDlg;				//TabCtrl������
	CRtuStation*     m_pSubStation;				//��ǰ��RtuStation����
	BOOL m_bResult;

// �Ի�������
	enum { IDD = IDD_DLG_SYQ_CALL_DATA1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listdata1;
	CListCtrl m_listdata2;
	CButtonXP m_btn_calldata1;
	CButtonXP m_btn_calldata2;
public:
	afx_msg void OnBnClickedCalldataBtn1();
public:
	afx_msg void OnBnClickedCalldataBtn2();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
