#pragma once

#include "RealTimeDB.h"
#include "UdfGlobalFUNC.h"
#include "afxwin.h"
// CUdfAddYxObjectDlg 对话框

class CUdfAddYxObjectDlg : public CDialog
{
	DECLARE_DYNAMIC(CUdfAddYxObjectDlg)

public:
	CUdfAddYxObjectDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUdfAddYxObjectDlg();

// 对话框数据
	enum { IDD = IDD_DLG_ADDYX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_stName;
	CString m_strYxName;
	DWORD m_dwYxCode;

	DB_RECORD_OPERATION_TYPE m_OperateFlag;     //操作标志
	CRtuYxObject* m_pCurYxObject;				//当前修改的Yc对象
	CRtuStation*  m_pCurStation;                //当前站Rtu对象
	CString       m_strCaption;                 //窗口标题栏
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
public:
	void Update(CRtuYxObject* pYx);
	void SetRtuStation(CRtuStation* pStation);
public:
	CButtonXP m_btnOk;
	CButtonXP m_btnCancel;
};
