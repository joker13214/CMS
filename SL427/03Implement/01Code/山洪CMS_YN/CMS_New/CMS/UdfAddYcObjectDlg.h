#pragma once
#include "afxwin.h"
#include "RealTimeDB.h"


// CUdfAddYcObjectDlg 对话框

class CUdfAddYcObjectDlg : public CDialog
{
	DECLARE_DYNAMIC(CUdfAddYcObjectDlg)

public:
	CUdfAddYcObjectDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUdfAddYcObjectDlg();

// 对话框数据
	enum { IDD = IDD_DLG_ADDYC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();

public:
	CString m_stName;
	CString m_sYCName;
	DWORD m_dwYcCode;
	CComboBoxXP m_wndCmbYcType;
	BYTE m_YcType;
	BYTE m_DisplayBits;
	BYTE m_PointBits;
	CString m_sUnit;
	float m_fScaleMax;
	float m_fScaleMin;

	DB_RECORD_OPERATION_TYPE m_OperateFlag;     //操作标志
	CRtuYcObject* m_pCurYcObject;				//当前修改的Yc对象
	CRtuStation*  m_pCurStation;                //当前站Rtu对象
	CString m_strCaption;                       //窗口标题栏

public:
	void Update(CRtuYcObject* pYc);
	void SetRtuStation(CRtuStation* pStation);

public:
	CButtonXP m_btnOk;
	CButtonXP m_btnCancel;
};
