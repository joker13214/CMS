#pragma once
#include "RealTimeDB.h"
#include "UdfGlobalFUNC.h"
#include "afxwin.h"

// CUdfAddZdObjectDlg �Ի���

class CUdfAddZdObjectDlg : public CDialog
{
	DECLARE_DYNAMIC(CUdfAddZdObjectDlg)

public:
	CUdfAddZdObjectDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUdfAddZdObjectDlg();

// �Ի�������
	enum { IDD = IDD_DLG_ADDZD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
	CRtuZdObject* m_pCurZdObject;				//��ǰ�޸ĵ�Yc����
	CRtuStation*  m_pCurStation;                //��ǰվRtu����
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
