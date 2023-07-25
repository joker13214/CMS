#pragma once

#include "RealTimeDB.h"
#include "UdfGlobalFUNC.h"
#include "afxwin.h"
// CUdfAddYxObjectDlg �Ի���

class CUdfAddYxObjectDlg : public CDialog
{
	DECLARE_DYNAMIC(CUdfAddYxObjectDlg)

public:
	CUdfAddYxObjectDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUdfAddYxObjectDlg();

// �Ի�������
	enum { IDD = IDD_DLG_ADDYX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_stName;
	CString m_strYxName;
	DWORD m_dwYxCode;

	DB_RECORD_OPERATION_TYPE m_OperateFlag;     //������־
	CRtuYxObject* m_pCurYxObject;				//��ǰ�޸ĵ�Yc����
	CRtuStation*  m_pCurStation;                //��ǰվRtu����
	CString       m_strCaption;                 //���ڱ�����
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
