#pragma once

#include "afxwin.h"
#include "XpStyle/ButtonXP.h"
#include "XpStyle/ComboBoxXP.h"
#include "GridCtrl/GridCtrl.h"
// CRtuAttrHydrWorkCondition �Ի���

class CRtuAttrHydrWorkCondition : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrHydrWorkCondition)

public:
	CRtuAttrHydrWorkCondition(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRtuAttrHydrWorkCondition();

// �Ի�������
	enum { IDD = IDD_RTU_QUERY_WORKCONDITION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

protected:

	CButtonXP   m_btn_QuerySensorVar;//��ѯ������ˮλ������ť
	CButtonXP   m_btn_QueryMemoryMargin;//��ѯ�洢������ť
	CButtonXP   m_btn_QueryEvoirTemperature;//��ѯ�����¶Ȱ�ť
	CButtonXP   m_btn_QuerySolarCellVoltage;//��ѯ̫���ܵ�ص�ѹ��ť
	CButtonXP   m_btn_QueryStorageVoltage;//���ص�ѹ
	CButtonXP   m_btn_QueryStoragePtr;//��ѯ���ݴ洢ָ��
	CButtonXP   m_btn_RainCnt;//��ѯ�����������ۼ�ֵ
	CButtonXP   m_btn_SetupStatus;//��ѯ��������װ״̬
	CComboBoxXP m_cbx_QuerySensorNO;//��������
	CEdit       m_let_SensorVar;
	CEdit       m_let_MemoryMargin;
	CEdit       m_let_EvoirTemperature;
	CEdit       m_let_SolarCellVoltage;
	CEdit       m_let_StorageVoltage;
	CEdit       m_let_RainCnt;
	CEdit       m_let_SetupStatus;
	CEdit       m_let_QueryStoragePtr;

	CString     m_str_SensorVar;
	CString     m_str_MemoryMargin;
	CString     m_str_EvoirTemperature;
	CString     m_str_SolarCellVoltage;
	CString     m_str_StorageVoltage;
	CString     m_str_RainCnt;
	CString     m_str_SetupStatus;
	CString     m_str_QueryStoragePtr;


	

	CRtuPropertyDlg* m_pParentDlg;				//TabCtrl������
	CRtuStation*     m_pSubStation;				//��ǰ��RtuStation����
	BOOL      m_blResult;                       //��/���ò����ɹ�����־

	void  SetWndsEnable(BOOL blEnable = TRUE);  //���ð�ť�Ƿ����
public:
	virtual BOOL OnInitDialog();
	void SetParentDlg(CRtuPropertyDlg* pDlg);
	void Update();                           //���½������ʾ��Ϣ
	void Update(int ret);
	void UpdateDlg(int attrType,BOOL blResult);  //���յ����ĺ󡢸��½���
	void KillAllTimers();

public:
	afx_msg void OnBnClickedBtnSensorvar();
	afx_msg void OnBnClickedBtnMemory();
	afx_msg void OnBnClickedBtnTemperation();
	afx_msg void OnBnClickedBtnSolarv();
	afx_msg void OnBnClickedBtnStorage();
	afx_msg void OnSelSensorNoChange();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	afx_msg void OnBnClickedBtnMemoryptr();
public:
	afx_msg void OnBnClickedBtnRaincnt();
public:
	afx_msg void OnBnClickedBtnSetupstatus();
};
