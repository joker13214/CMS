#pragma once
#include "XpStyle/ButtonXP.h"
#include "XpStyle/ComboBoxXP.h"
#include "GridCtrl/GridCtrl.h"

// CRtuAttrWaterLevel �Ի���
class CRtuPropertyDlg;
class CRtuAttrWaterLevel : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrWaterLevel)

public:
	CRtuAttrWaterLevel(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRtuAttrWaterLevel();

// �Ի�������
	enum { IDD = IDD_RTU_WATERLEVEL };
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
	void SetWndsEnable(BOOL blEnable);					//���ð�ť�Ƿ����
	void InitGridItems();								//GridCtrl��ʾ
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_Edit_WaterLevelCount;
	CComboBoxXP m_wndWaterLevelCmb;
	CGridCtrl   m_wndGridCtrl;

	CButtonXP m_btnQueryWaterLevel;
	CButtonXP m_btnSetWaterLevel;
	CButtonXP m_btnQueryWaterLevelData;
	CButtonXP m_btnWaterLevelOK;

	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickQueryWaterLevel();
	afx_msg void OnBnClickQueryWaterLevelData();
	afx_msg void OnBnClickSetWaterLevel();
	afx_msg void OnBnClickWaterLevelOK();
	afx_msg void OnCbnSelchangeCmbWaterLevel();
	afx_msg void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
