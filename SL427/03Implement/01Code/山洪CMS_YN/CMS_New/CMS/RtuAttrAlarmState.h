#pragma once
#include "afxwin.h"
#include "XpStyle/ButtonXP.h"
#include "XpStyle/ComboBoxXP.h"


// CRtuAttrAlarmState �Ի���
class CRtuPropertyDlg;
class CRtuAttrAlarmState : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrAlarmState)

public:
	CRtuAttrAlarmState(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRtuAttrAlarmState();

// �Ի�������
	enum { IDD = IDD_RTU_ALARMSTATE };
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
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void  SetWndsEnable(BOOL blEnable = TRUE);			//���ÿؼ����Ƿ����
public:
	CComboBoxXP m_wndExAlarmCmb;
	CComboBoxXP m_wndRtuStateCmb;
	CComboBoxXP m_wndAlarmCmb;
	CButtonXP m_btnAlarmCall;
	CButtonXP m_btnExAlarmCall;
	CEdit     m_Edit_AlarmValue;
	CEdit     m_Edit_ExAlarmValue;
	CEdit     m_Edit_StateValue;
public:
	afx_msg void OnCbnSelchangeCmbRtualarm();
	afx_msg void OnCbnSelchangeCmbExalarm();
	afx_msg void OnCbnSelchangeCmbRtustate();
	afx_msg void OnBnClickExAlarmCall();
	afx_msg void OnBnClickAlarmCall();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
