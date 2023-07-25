#pragma once
#include "afxwin.h"
#include "GridCtrl/GridCtrl.h"


// CUdfCfgLogOptDlg 对话框

class CUdfCfgLogOptDlg : public CDialog
{
	DECLARE_DYNAMIC(CUdfCfgLogOptDlg)

public:
	CUdfCfgLogOptDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUdfCfgLogOptDlg();

// 对话框数据
	enum { IDD = IDD_DLG_CFGLOGOPT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
	void         Show(int nCmdShow);
protected:
	virtual void OnCancel();
	BOOL         SaveLogOpts();                        //存储日志系统相关参数信息
	void         InitGridCtrl();					   //初始化GridCtrl
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
