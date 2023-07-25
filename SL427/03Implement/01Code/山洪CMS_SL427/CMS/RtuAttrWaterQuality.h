#pragma once
#include "afxwin.h"
#include "XpStyle/ButtonXP.h"
#include "XpStyle/ComboBoxXP.h"
#include "GridCtrl/GridCtrl.h"

// CRtuAttrWaterQuality �Ի���

class CRtuPropertyDlg;
class CRtuAttrWaterQuality : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrWaterQuality)

public:
	CRtuAttrWaterQuality(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRtuAttrWaterQuality();

// �Ի�������
	enum { IDD = IDD_RTU_WQUALITY };
protected:
	//���е�Tabҳ���ڶ�����߱��ĳ�Ա�����Ͷ���
	CRtuPropertyDlg* m_pParentDlg;                      //TabCtrl�ĸ�����
	CRtuStation*     m_pSubStation;						//�˴��ڵ�ǰ��RtuStation����
	BOOL			m_blResult;                         //��.������ز����Ƿ�ɹ��ı�־

	void  InitGridItems();                              //��ʼ��GridCtrl��Items
	void  SetWndsEnable(BOOL blEnable = TRUE);			//���ð�ť�Ƿ����
public:
	void SetParentDlg(CRtuPropertyDlg* pDlg);
	void Update();										//���½������ʾ��Ϣ
	void UpdateDlg(int attrType,BOOL blResult);			//���յ����ĺ󡢸��½���
	void KillAllTimers();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CComboBoxXP m_wndWQualityTypeCmd;
	CButtonXP m_btnWQUpCall;
	CButtonXP m_btnWQUpSet;
	CButtonXP m_btnWQDownCall;
	CButtonXP m_btnWQDownSet;
	CButtonXP m_btnWQDataCall;
	CGridCtrl m_wndGridCtrl;
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnCbnSelchangeCmbWquality();
	afx_msg void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnBnClickedBtnWqupcall();
	afx_msg void OnBnClickedBtnWqupset();
	afx_msg void OnBnClickedBtnWqdowncall();
	afx_msg void OnBnClickedBtnWqdownset();
	afx_msg void OnBnClickedBtnWqdatacall();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
