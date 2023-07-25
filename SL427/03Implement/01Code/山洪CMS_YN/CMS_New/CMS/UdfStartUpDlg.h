#pragma once


// CUdfStartUpDlg 对话框

class CUdfStartUpDlg : public CDialog
{
	DECLARE_DYNAMIC(CUdfStartUpDlg)

public:
	CUdfStartUpDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUdfStartUpDlg();
	CBitmap m_bitmap;

// 对话框数据
	enum { IDD = IDD_DLG_START };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnPaint();
};
