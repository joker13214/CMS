#pragma once
#include "RealTimeDB.h"
#include "UdfGlobalFUNC.h"
#include "afxwin.h"

// CUdfAddYkObjectDlg �Ի���

class CUdfAddYkObjectDlg : public CDialog
{
	DECLARE_DYNAMIC(CUdfAddYkObjectDlg)

public:
	CUdfAddYkObjectDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUdfAddYkObjectDlg();

// �Ի�������
	enum { IDD = IDD_DLG_ADDYK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_stName;
	CString m_strYkName;
	DWORD m_wYkCode;

	CString m_strCaption;
	DB_RECORD_OPERATION_TYPE m_OperateFlag;
	CRtuYkObject* m_pCurYkObject;				//��ǰ�޸ĵ�Yc����
	CRtuStation*  m_pCurStation;                //��ǰվRtu����

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
