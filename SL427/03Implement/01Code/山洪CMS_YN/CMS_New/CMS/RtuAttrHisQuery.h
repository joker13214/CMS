#pragma once
#include "afxdtctl.h"
#include "afxwin.h"


// CRtuAttrHisQuery �Ի���

class CRtuAttrHisQuery : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrHisQuery)

public:
	CRtuAttrHisQuery(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRtuAttrHisQuery();

	virtual void OnFinalRelease();
	void  Update();							  //��ʾ��Ϣ����		
	void  SetParentDlg(CRtuPropertyDlg* pParent);

// �Ի�������
	enum { IDD = IDD_RTU_HISQUERY };
protected:
	CRtuPropertyDlg* m_pParentDlg;                      //TabCtrl�ĸ�����
	CRtuStation*     m_pSubStation;						//�˴��ڵ�ǰ��RtuStation����
	CGridCtrl        m_wndGridCtrl;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	virtual BOOL OnInitDialog();
	void InitGridHeader();
public:
	CDateTimeCtrl m_wndBeginTimeCtrl;
	CDateTimeCtrl m_wndEndTimeCtrl;
	CComboBoxXP m_wndQueryTypeCtrl;
	CButtonXP m_BtnQuery;
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnCbnSelchangeComboType();
	void UpdateQueryTime();
private:
	CString m_sBegin;
	CString m_sEnd;
	void AddMessageToGridCtrl(CString strTime,CString strReport);
};
