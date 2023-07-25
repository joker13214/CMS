#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CRtuAttrSyqCallData2 对话框

class CRtuAttrSyqCallData2 : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrSyqCallData2)

public:
	CRtuAttrSyqCallData2(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRtuAttrSyqCallData2();

	virtual BOOL OnInitDialog();
	void SetParentDlg(CRtuPropertyDlg* pDlg);
	void Update();

	void  UpdateDlgMessage(int attr, bool result);
	void  FillDataList();
	void  SetEnable(bool b);

	CRtuPropertyDlg* m_pParentDlg;				//TabCtrl父窗口
	CRtuStation*     m_pSubStation;				//当前的RtuStation对象
	BOOL m_bResult;

// 对话框数据
	enum { IDD = IDD_DLG_SYQ_CALL_DATA2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
