#pragma once
#include "afxwin.h"


// CExitDlg 对话框

class CExitDlg : public CDialog
{
	DECLARE_DYNAMIC(CExitDlg)

public:
	CExitDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CExitDlg();

// 对话框数据
	enum { IDD = IDD_DLG_EXIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CButtonXP m_btnOK;
	CButtonXP m_btnCancel;
public:
	virtual BOOL OnInitDialog();
};
