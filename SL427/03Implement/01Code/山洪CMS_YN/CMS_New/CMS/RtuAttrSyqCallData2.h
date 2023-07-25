#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CRtuAttrSyqCallData2 �Ի���

class CRtuAttrSyqCallData2 : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrSyqCallData2)

public:
	CRtuAttrSyqCallData2(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRtuAttrSyqCallData2();

	virtual BOOL OnInitDialog();
	void SetParentDlg(CRtuPropertyDlg* pDlg);
	void Update();

	void  UpdateDlgMessage(int attr, bool result);
	void  FillDataList();
	void  SetEnable(bool b);

	CRtuPropertyDlg* m_pParentDlg;				//TabCtrl������
	CRtuStation*     m_pSubStation;				//��ǰ��RtuStation����
	BOOL m_bResult;

// �Ի�������
	enum { IDD = IDD_DLG_SYQ_CALL_DATA2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	CListCtrl m_listdata;
	CListBox m_yaosulist1;
	CListBox m_yaosulist2;
	CButtonXP m_btn_add;
	CButtonXP m_btn_del;
	CButtonXP m_btn_query;
public:
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnDel();
	afx_msg void OnBnClickedBtnQuery();
	afx_msg void OnLbnDblclkListYaosu();
	afx_msg void OnLbnDblclkListYaosu2();
};
