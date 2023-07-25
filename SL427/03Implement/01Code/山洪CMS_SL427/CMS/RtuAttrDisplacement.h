#pragma once
#include "afxwin.h"
#include "XpStyle/ButtonXP.h"
#include "XpStyle/ComboBoxXP.h"
#include "GridCtrl/GridCtrl.h"


// CRtuAttrDisplacement �Ի���
class CRtuStation;
class CRtuPropertyDlg;
class CRtuAttrDisplacement : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrDisplacement)

public:
	CRtuAttrDisplacement(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRtuAttrDisplacement();

// �Ի�������
	enum { IDD = IDD_RTU_DISPLACEMENT };

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


	CEdit m_wndDisplacePCount;
	CComboBoxXP m_wndDisplaceCmb;
	CButtonXP m_btnDisplaceLCall;
	CButtonXP m_btnDisplaceLSet;
	CStatic m_wndDisplaceCaption;
	CButtonXP m_btnDisplaceOK;
	int m_nDisplaceCount;
	CGridCtrl m_wndDisplaceGrid;
	CButtonXP m_btnQueryAReal;
public:
	virtual BOOL OnInitDialog();
	void    FillDisplaceGridCtrl();
	void    SetWndCtrlEnable(BOOL blEnable);
public:
	afx_msg void OnCbnSelchangeComboDisplacepara();
	afx_msg void OnBnClickedBtnDisplacelcall();
	afx_msg void OnBnClickedBtnDisplacelset();
	afx_msg void OnBnClickedBtnDisplaceok();
	afx_msg void OnBnClickBtnQueryAReal();
	afx_msg void OnEndEditDisplaceGrid(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	
};
