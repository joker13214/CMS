#pragma once
#include "afxcmn.h"
#include "UdfListCtrl.h"

// CRtuAttrComms 对话框
class CRtuPropertyDlg;
class CRtuAttrComms : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrComms)

public:
	CRtuAttrComms(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRtuAttrComms();

// 对话框数据
	enum { IDD = IDD_RTU_COMMS };

	void  Update();							  //显示信息更新		
	void  SetParentDlg(CRtuPropertyDlg* pParent);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CUdfListCtrl m_wndCommsList;
protected:
	CRtuPropertyDlg* m_pParentDlg;                      //TabCtrl的父窗口
	CRtuStation*     m_pSubStation;						//此窗口当前的RtuStation对象

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void AddMessageToWindow( CString m_Message, _Log_level_type m_Level, SYSTEMTIME m_Time );
public:
	virtual BOOL OnInitDialog();
	void InitListCtrlHeader();
};
