#pragma once
#include "afxwin.h"
#include "XpStyle/ButtonXP.h"
#include "XpStyle/ComboBoxXP.h"
#include "GridCtrl/GridCtrl.h"


// CRtuAttrTemperature �Ի���
class CRtuStation;
class CRtuPropertyDlg;
class CRtuAttrTemperature : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrTemperature)

public:
	CRtuAttrTemperature(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRtuAttrTemperature();

// �Ի�������
	enum { IDD = IDD_RTU_TEMPERATRUE };

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


	CEdit m_wndTemperaPCount;
	CComboBoxXP m_wndTemperaCmb;
	CButtonXP m_btnTemperaLCall;
	CButtonXP m_btnTemperaLSet;
	CStatic m_wndTemperaCaption;
	CButtonXP m_btnTemperaOK;
	int m_nTemperaCount;
	CGridCtrl m_wndTemperaGrid;
	CButtonXP m_btnQueryAReal;
public:
	virtual BOOL OnInitDialog();
	void    FillTemperaGridCtrl();
	void    SetWndCtrlEnable(BOOL blEnable);
public:
	afx_msg void OnCbnSelchangeComboTemperapara();
	afx_msg void OnBnClickedBtnTemperalcall();
	afx_msg void OnBnClickedBtnTemperalset();
	afx_msg void OnBnClickedBtnTemperaok();
	afx_msg void OnBnClickBtnQueryAReal();
	afx_msg void OnEndEditTemperaGrid(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	
};
