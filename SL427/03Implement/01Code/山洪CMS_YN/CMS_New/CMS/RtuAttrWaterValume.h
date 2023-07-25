#pragma once
#include "XpStyle/ButtonXP.h"
#include "XpStyle/ComboBoxXP.h"

// CRtuAttrWaterValume �Ի���
class CRtuPropertyDlg;
class CRtuAttrWaterValume : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrWaterValume)

public:
	CRtuAttrWaterValume(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRtuAttrWaterValume();

// �Ի�������
	enum { IDD = IDD_RTU_WATERVALUME };

public:
	CEdit m_Edit_Czl;
	CEdit m_Edit_Remain;
	CEdit m_Edit_Alert;

	

	CButtonXP m_btnQueryCzl;
	CButtonXP m_btnSetCzl;
	CButtonXP m_btnQueryRemainAlert;
	CButtonXP m_btnSetRemainAlert;
protected:
	//���е�Tabҳ���ڶ�����߱��ĳ�Ա�����Ͷ���
	CRtuPropertyDlg* m_pParentDlg;                      //TabCtrl�ĸ�����
	CRtuStation*     m_pSubStation;						//�˴��ڵ�ǰ��RtuStation����
	BOOL			 m_blResult;                        //��.������ز����Ƿ�ɹ��ı�־
public:
	virtual BOOL OnInitDialog();
	void SetParentDlg(CRtuPropertyDlg* pDlg);
	void Update();										//���½������ʾ��Ϣ
	void UpdateDlg(int attrType,BOOL blResult);			//���յ����ĺ󡢸��½���
	void KillAllTimers();
	void SetWndsEnable(BOOL blEnable);					//���ð�ť�Ƿ����
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickQueryCzl();
	afx_msg void OnBnClickSetCzl();
	afx_msg void OnBnClickQueryRemainAlert();
	afx_msg void OnBnClickSetRemainAlert();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
