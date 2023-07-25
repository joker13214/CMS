#pragma once
#include "RealTimeDB.h"
#include "UdfGlobalFUNC.h"
#include "afxwin.h"

// CUdfAddYkObjectDlg 对话框

class CUdfAddYkObjectDlg : public CDialog
{
	DECLARE_DYNAMIC(CUdfAddYkObjectDlg)

public:
	CUdfAddYkObjectDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUdfAddYkObjectDlg();

// 对话框数据
	enum { IDD = IDD_DLG_ADDYK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_stName;
	CString m_strYkName;
	DWORD m_wYkCode;

	CString m_strCaption;
	DB_RECORD_OPERATION_TYPE m_OperateFlag;
	CRtuYkObject* m_pCurYkObject;				//当前修改的Yc对象
	CRtuStation*  m_pCurStation;                //当前站Rtu对象

public:
	void Update(CRtuYkObject* pYk);
	void SetRtuStation(CRtuStation* pStation);

public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	CButtonXP m_btnOK;
	CButtonXP m_btnCancel;
};
