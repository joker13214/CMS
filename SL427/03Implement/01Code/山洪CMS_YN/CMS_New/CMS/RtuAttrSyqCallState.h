#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CRtuAttrSyqCallState �Ի���

class CRtuAttrSyqCallState : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrSyqCallState)

public:
	CRtuAttrSyqCallState(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRtuAttrSyqCallState();

	virtual BOOL OnInitDialog();
	void SetParentDlg(CRtuPropertyDlg* pDlg);
	void Update();
	void UpdateDlgMessage(int attr, bool result);
	void FillList(int index,CString strname,CString strstate);
	void FillState();
	void  SetEnable(bool b);

	CRtuPropertyDlg* m_pParentDlg;				//TabCtrl������
	CRtuStation*     m_pSubStation;				//��ǰ��RtuStation����
	BOOL m_bResult;

// �Ի�������
	enum { IDD = IDD_DLG_SYQ_STATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CButtonXP m_btn_query;
public:
	afx_msg void OnBnClickedBtnQuery();
public:
	CListCtrl m_list;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
