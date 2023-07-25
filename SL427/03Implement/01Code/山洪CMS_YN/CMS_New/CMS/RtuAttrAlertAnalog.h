#pragma once
#include "afxwin.h"
#include "XpStyle/ButtonXP.h"
#include "XpStyle/ComboBoxXP.h"
#include "GridCtrl/GridCtrl.h"


// CRtuAttrAlertAnalog �Ի���
class CRtuStation;
class CRtuPropertyDlg;
class CRtuAttrAlertAnalog : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrAlertAnalog)

public:
	CRtuAttrAlertAnalog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRtuAttrAlertAnalog();

// �Ի�������
	enum { IDD = IDD_RTU_ALERTMONI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

protected:
	CRtuPropertyDlg* m_pParentDlg;                      //TabCtrl�ĸ�����
	CRtuStation*     m_pSubStation;						//�˴��ڵ�ǰ��RtuStation����
	BOOL      m_blResult;                       //��/���ò����ɹ�����־
public:
	void  Update();							  //��ʾ��Ϣ����		
	void  SetParentDlg(CRtuPropertyDlg* pParent) {m_pParentDlg = pParent;}
	void  UpdateDlg(int attrType,BOOL blResult);
	void  KillAllTimers();
public:


	CEdit m_wndAnalogPCount;
	CComboBoxXP m_wndAnalogCmb;
	CButtonXP m_btnAnalogLCall;
	CButtonXP m_btnAnalogLSet;
	CStatic m_wndAnalogCaption;
	CButtonXP m_btnAnalogOK;
	int m_nAnalogCount;
	CGridCtrl m_wndAnalogGrid;
	CButtonXP m_btnQueryAReal;
public:
	virtual BOOL OnInitDialog();
	void    FillAnalogGridCtrl();
	void    SetWndCtrlEnable(BOOL blEnable);
public:
	afx_msg void OnCbnSelchangeComboAnalohpara();
	afx_msg void OnBnClickedBtnAnaloglcall();
	afx_msg void OnBnClickedBtnAnaloglset();
	afx_msg void OnBnClickedBtnAnalogok();
	afx_msg void OnBnClickBtnQueryAReal();
	afx_msg void OnEndEditAnalogGrid(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	
};
