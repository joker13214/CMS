#pragma once


// CUdfShowDetailLog 对话框

class CUdfShowDetailLog : public CDialog
{
	DECLARE_DYNAMIC(CUdfShowDetailLog)

public:
	CUdfShowDetailLog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUdfShowDetailLog();
	HBRUSH m_brMine;
	void Show(bool blShow=true);

// 对话框数据
	enum { IDD = IDD_DLG_SHOWLOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strDatetime;
	CString m_strLogLevel;
	CString m_strLogType;
	CString m_strLogMessage;
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClose();
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};
