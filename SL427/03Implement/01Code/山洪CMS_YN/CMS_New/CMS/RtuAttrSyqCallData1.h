#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CRtuAttrSyqCallData1 对话框

class CRtuAttrSyqCallData1 : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrSyqCallData1)

public:
	CRtuAttrSyqCallData1(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRtuAttrSyqCallData1();
	virtual BOOL OnInitDialog();
	void SetParentDlg(CRtuPropertyDlg* pDlg);
	void Update();

	void  UpdateDlgMessage(int attr, bool result);
	void  FillDataList1();
	void  FillDataList2();
	void  SetEnable(bool b);

	CRtuPropertyDlg* m_pParentDlg;				//TabCtrl父窗口
	CRtuStation*     m_pSubStation;				//当前的RtuStation对象
	BOOL m_bResult;

// 对话框数据
	enum { IDD = IDD_DLG_SYQ_CALL_DATA1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
