#pragma once
#include "afxwin.h"
#include "GridCtrl/GridCtrl.h"


// CUdfCfgLogOptDlg �Ի���

class CUdfCfgLogOptDlg : public CDialog
{
	DECLARE_DYNAMIC(CUdfCfgLogOptDlg)

public:
	CUdfCfgLogOptDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUdfCfgLogOptDlg();

// �Ի�������
	enum { IDD = IDD_DLG_CFGLOGOPT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
	void         Show(int nCmdShow);
protected:
	virtual void OnCancel();
	BOOL         SaveLogOpts();                        //�洢��־ϵͳ��ز�����Ϣ
	void         InitGridCtrl();					   //��ʼ��GridCtrl
	void         SetGridCellComboBox(int row,int col);
public:
	BOOL m_blCmStatus;
	BOOL m_blCmBuffer;
	BOOL m_blYkOperate;
	BOOL m_blYcChange;
	BOOL m_blYxChange;
	BOOL m_blZdOperate;
	BOOL m_blSysRunning;
	BOOL m_blOthersRun;
	BOOL m_blDataInErr;
	int m_nSaveMonths;
public:
	afx_msg void OnClose();
	afx_msg void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);
public:
	CButtonXP m_btnOK;
	CButtonXP m_btnCancel;
	CGridCtrl m_wndGridCtrl;
};
