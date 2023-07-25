#pragma once
#include "afxwin.h"


// CRtuAttrPulse �Ի���
class CRtuPropertyDlg;
class CRtuAttrPulse : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrPulse)

public:
	CRtuAttrPulse(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRtuAttrPulse();

// �Ի�������
	enum { IDD = IDD_RTU_PULSE };
protected:
	//���е�Tabҳ���ڶ�����߱��ĳ�Ա�����Ͷ���
	CRtuPropertyDlg* m_pParentDlg;                      //TabCtrl�ĸ�����
	CRtuStation*     m_pSubStation;						//�˴��ڵ�ǰ��RtuStation����
	BOOL			 m_blResult;                        //��.������ز����Ƿ�ɹ��ı�־

public:
	void SetParentDlg(CRtuPropertyDlg* pDlg);
	void Update();										//���½������ʾ��Ϣ
	void UpdateDlg(int attrType,BOOL blResult);			//���յ����ĺ󡢸��½���
	void KillAllTimers();

	void SetWndCtrlEnable(BOOL blEnable);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CButtonXP m_btnPulseQuery;
	CComboBoxXP m_wndPulseCmb;
	CGridCtrl m_wndGridCtrl;
public:
	afx_msg void OnBnClickedBtnQuerypulse();
	afx_msg void OnCbnSelchangeComboPulsepara();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	virtual BOOL OnInitDialog();
	void InitGridCtrl();
public:
	int m_nPulseCount;
	CEdit m_wndPulseCountEdit;
	CStatic m_wndPulseCaption;
};
