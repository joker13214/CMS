#pragma once
#include "RealTimeDB.h"
#include "UdfGlobalFUNC.h"
#include "afxwin.h"

// CUdfAddZdObjectDlg 对话框

class CUdfAddZdObjectDlg : public CDialog
{
	DECLARE_DYNAMIC(CUdfAddZdObjectDlg)

public:
	CUdfAddZdObjectDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUdfAddZdObjectDlg();

// 对话框数据
	enum { IDD = IDD_DLG_ADDZD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_stName;
	CString m_strZdName;
	DWORD m_dwZdCode;
	BYTE m_bSizeof;
	BYTE m_OrderInItem;
	CString m_strUnit;
	float m_fScaleMax;
	float m_fScaleMin;

	CString m_strCaption;
	DB_RECORD_OPERATION_TYPE m_OperateFlag;
	CRtuZdObject* m_pCurZdObject;				//当前修改的Yc对象
	CRtuStation*  m_pCurStation;                //当前站Rtu对象
public:
	void Update(CRtuZdObject* pZd);
	void SetRtuStation(CRtuStation* pStation);

public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	CButtonXP m_btnOk;
	CButtonXP m_btnCancel;
};
