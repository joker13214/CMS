#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include "resource.h"

// CUdfLogQueryDlg 对话框

class CUdfLogQueryDlg : public CDialog
{
	DECLARE_DYNAMIC(CUdfLogQueryDlg)

public:
	CUdfLogQueryDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUdfLogQueryDlg();

// 对话框数据
	enum { IDD = IDD_DLG_LOGQUERY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	enum QUERYTYPE{
		QUERYTYPE_ANYTIME,			//任意时间段	
		QUERYTYPE_MINUTE,           //按分钟查询
		QUERYTYPE_HOUR,             //按小时
		QUERYTYPE_DAY,              //按天
		QUERYTYPE_MONTH             //按月查询
	};
	enum QUERY_LOG_TYPE{
		QUERY_LOGTYPE_ALL = 0,			//全部
		QUERY_LOGTYPE_WORNING,			//警告
		QUERY_LOGTYPE_ERROR,			//错误
		QUERY_LOGTYPE_WORNINGANDERROR	//警告和错误
	};

public:
	CStatic m_wndBeginTextCtrl;
	CStatic m_wndEndTextCtrl;
	CDateTimeCtrl m_wndEndTimeCtrl;
	CDateTimeCtrl m_wndBeginTimeCtrl;
	CComboBoxXP m_wndQueryTypeCtrl;
	CComboBoxXP m_wndQueryLogTypeCtrl;
public:
	virtual BOOL OnInitDialog();
	void SetQueryType(QUERYTYPE queryType);
public:
	afx_msg void OnCbnSelchangeQueryType();
	afx_msg void OnCbnSelchangeLogType();

public:
	QUERYTYPE m_queryType;
	QUERY_LOG_TYPE m_queryLogType;

	//返回值

	SYSTEMTIME m_timeBegin;
	SYSTEMTIME m_timeEnd;


public:
	afx_msg void OnBnClickedOk();
public:
	CButtonXP m_btnOK;
public:
	CButtonXP m_btnCancel;
};
