#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CShowLogDlg �Ի���

class CShowLogDlg : public CDialog
{
	DECLARE_DYNAMIC(CShowLogDlg)

public:
	CShowLogDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CShowLogDlg();
	HBRUSH m_brMine;

// �Ի�������
	enum { IDD = IDD_DLG_LOGMSG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	//CRichEditCtrl m_wndRichEditCtrl;
	CEdit m_wndEditDate;
	CEdit m_wndEditMessage;
public:
	afx_msg void OnClose();	
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	virtual BOOL OnInitDialog();
	void    ShowLog(CString strDate,CString strLog);
public:	
};
