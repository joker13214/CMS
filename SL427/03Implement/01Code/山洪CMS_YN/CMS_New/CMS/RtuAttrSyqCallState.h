#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CRtuAttrSyqCallState 对话框

class CRtuAttrSyqCallState : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrSyqCallState)

public:
	CRtuAttrSyqCallState(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRtuAttrSyqCallState();

	virtual BOOL OnInitDialog();
	void SetParentDlg(CRtuPropertyDlg* pDlg);
	void Update();
	void UpdateDlgMessage(int attr, bool result);
	void FillList(int index,CString strname,CString strstate);
	void FillState();
	void  SetEnable(bool b);

	CRtuPropertyDlg* m_pParentDlg;				//TabCtrl父窗口
	CRtuStation*     m_pSubStation;				//当前的RtuStation对象
	BOOL m_bResult;

// 对话框数据
	enum { IDD = IDD_DLG_SYQ_STATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
