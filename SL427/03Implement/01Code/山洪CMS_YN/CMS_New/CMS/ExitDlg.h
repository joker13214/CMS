#pragma once
#include "afxwin.h"


// CExitDlg �Ի���

class CExitDlg : public CDialog
{
	DECLARE_DYNAMIC(CExitDlg)

public:
	CExitDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CExitDlg();

// �Ի�������
	enum { IDD = IDD_DLG_EXIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CButtonXP m_btnOK;
	CButtonXP m_btnCancel;
public:
	virtual BOOL OnInitDialog();
};
