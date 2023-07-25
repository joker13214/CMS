#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include "XpStyle/ButtonXP.h"
#include "XpStyle/ComboBoxXP.h"
#include "GridCtrl/GridCtrl.h"

// CRtuAttrDayHisQuery �Ի���
class CRtuPropertyDlg;
class CRtuAttrDayHisQuery : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrDayHisQuery)

public:
	CRtuAttrDayHisQuery(CWnd* pParent = NULL);			// ��׼���캯��
	virtual ~CRtuAttrDayHisQuery();

// �Ի�������
	enum { IDD = IDD_RTU_DAYHISQUERY };
protected:
	CRtuPropertyDlg* m_pParentDlg;                      //TabCtrl�ĸ�����
	CRtuStation*     m_pSubStation;						//�˴��ڵ�ǰ��RtuStation����
	BOOL			 m_blResult;                        //��.������ز����Ƿ�ɹ��ı�־
public:
	void SetParentDlg(CRtuPropertyDlg* pDlg);
	void Update();										//���½������ʾ��Ϣ
	void UpdateDlg(BOOL blResult,CString strData);		//���յ����ĺ󡢸��½���
	void KillAllTimers();
	void SetWndsEnable(BOOL blEnable);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CComboBoxXP m_wndZBTypeCmb;						    //����/���ѡ��
	CDateTimeCtrl m_wndQueryDate;						//��ѯʱ��
	CComboBoxXP m_wndDataTypeCmb;						//��ѯ��������
	CComboBoxXP m_wndQueryNoCmb;						//��ѯ���
	CButtonXP	m_btnQueryHis;
	//CGridCtrl	m_wndGridCtrl;
protected:
	BYTE m_bZBtype;										//�����ֵ D7-D6 '10', 2 ���� '01' 1 �������
	BYTE m_bDataType;									//��������
	BYTE m_bQueryNo;									//��ѯ���
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedBtnQueryhis();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnCbnSelchangeCmbZbtype();
	afx_msg void OnCbnSelchangeCmbDatatype();
	afx_msg void OnCbnSelchangeCmbQueryno();
public:
	CEdit m_EditDataInfo;
};
