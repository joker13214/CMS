#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include "resource.h"

// CUdfLogQueryDlg �Ի���

class CUdfLogQueryDlg : public CDialog
{
	DECLARE_DYNAMIC(CUdfLogQueryDlg)

public:
	CUdfLogQueryDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUdfLogQueryDlg();

// �Ի�������
	enum { IDD = IDD_DLG_LOGQUERY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	enum QUERYTYPE{
		QUERYTYPE_ANYTIME,			//����ʱ���	
		QUERYTYPE_MINUTE,           //�����Ӳ�ѯ
		QUERYTYPE_HOUR,             //��Сʱ
		QUERYTYPE_DAY,              //����
		QUERYTYPE_MONTH             //���²�ѯ
	};
	enum QUERY_LOG_TYPE{
		QUERY_LOGTYPE_ALL = 0,			//ȫ��
		QUERY_LOGTYPE_WORNING,			//����
		QUERY_LOGTYPE_ERROR,			//����
		QUERY_LOGTYPE_WORNINGANDERROR	//����ʹ���
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

	//����ֵ

	SYSTEMTIME m_timeBegin;
	SYSTEMTIME m_timeEnd;


public:
	afx_msg void OnBnClickedOk();
public:
	CButtonXP m_btnOK;
public:
	CButtonXP m_btnCancel;
};
