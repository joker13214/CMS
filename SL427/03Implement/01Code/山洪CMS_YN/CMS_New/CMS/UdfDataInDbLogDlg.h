#pragma once
#include "UdfLogSystem.h"
#include "afxcmn.h"
#include "UdfListCtrl.h"

// CUdfDataInDbLogDlg 对话框

class CUdfDataInDbLogDlg : public CDialog
{
	DECLARE_DYNAMIC(CUdfDataInDbLogDlg)

public:
	CUdfDataInDbLogDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUdfDataInDbLogDlg();

// 对话框数据
	enum { IDD = IDD_DLG_DATAINDBLOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CUdfListCtrl m_wndListCtrl;
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);

public:
	//添加日志信息到ListCtrl
	void  AddOneMessage(CString strMessage,PSYSTEMTIME pTime = NULL,_Log_level_type _xLevel = Normal_Log_Message);
	void  AddOneMessage(CString strMessage,CTime wTime,_Log_level_type _xLevel = Normal_Log_Message);
	void  AddOneMessage(CString strMessage,CString strTime,_Log_level_type _xLevel = Normal_Log_Message);

public:
	afx_msg void OnClose();
	afx_msg void OnNMRclickListDataindblog(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClearLogList();
	afx_msg void OnSaveAsLogList();   //保存日志信息到其他文件
public:
	afx_msg void OnNMDblclkListDataindblog(NMHDR *pNMHDR, LRESULT *pResult);
};
