#pragma once


// CUdfStartUpDlg �Ի���

class CUdfStartUpDlg : public CDialog
{
	DECLARE_DYNAMIC(CUdfStartUpDlg)

public:
	CUdfStartUpDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUdfStartUpDlg();
	CBitmap m_bitmap;

// �Ի�������
	enum { IDD = IDD_DLG_START };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnPaint();
};
