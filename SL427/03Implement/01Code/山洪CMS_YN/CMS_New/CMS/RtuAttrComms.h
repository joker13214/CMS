#pragma once
#include "afxcmn.h"
#include "UdfListCtrl.h"

// CRtuAttrComms �Ի���
class CRtuPropertyDlg;
class CRtuAttrComms : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrComms)

public:
	CRtuAttrComms(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRtuAttrComms();

// �Ի�������
	enum { IDD = IDD_RTU_COMMS };

	void  Update();							  //��ʾ��Ϣ����		
	void  SetParentDlg(CRtuPropertyDlg* pParent);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CUdfListCtrl m_wndCommsList;
protected:
	CRtuPropertyDlg* m_pParentDlg;                      //TabCtrl�ĸ�����
	CRtuStation*     m_pSubStation;						//�˴��ڵ�ǰ��RtuStation����

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void AddMessageToWindow( CString m_Message, _Log_level_type m_Level, SYSTEMTIME m_Time );
public:
	virtual BOOL OnInitDialog();
	void InitListCtrlHeader();
};
