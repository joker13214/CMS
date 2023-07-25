#pragma once
#include "afxwin.h"
#include "RealTimeDB.h"


// CUdfAddYcObjectDlg �Ի���

class CUdfAddYcObjectDlg : public CDialog
{
	DECLARE_DYNAMIC(CUdfAddYcObjectDlg)

public:
	CUdfAddYcObjectDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUdfAddYcObjectDlg();

// �Ի�������
	enum { IDD = IDD_DLG_ADDYC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

	DB_RECORD_OPERATION_TYPE m_OperateFlag;     //������־
	CRtuYcObject* m_pCurYcObject;				//��ǰ�޸ĵ�Yc����
	CRtuStation*  m_pCurStation;                //��ǰվRtu����
	CString m_strCaption;                       //���ڱ�����

public:
	void Update(CRtuYcObject* pYc);
	void SetRtuStation(CRtuStation* pStation);

public:
	CButtonXP m_btnOk;
	CButtonXP m_btnCancel;
};
