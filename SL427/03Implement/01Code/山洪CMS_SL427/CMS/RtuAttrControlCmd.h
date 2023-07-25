#pragma once
#include "afxwin.h"
#include "XpStyle/ButtonXP.h"
#include "XpStyle/ComboBoxXP.h"

// CRtuAttrControlCmd �Ի���
class CRtuPropertyDlg;
class CRtuAttrControlCmd : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrControlCmd)

public:
	CRtuAttrControlCmd(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRtuAttrControlCmd();

// �Ի�������
	enum { IDD = IDD_RTU_CONTROLCMD };
protected:
	//���е�Tabҳ���ڶ�����߱��ĳ�Ա�����Ͷ���
	CRtuPropertyDlg* m_pParentDlg;                      //TabCtrl�ĸ�����
	CRtuStation*     m_pSubStation;						//�˴��ڵ�ǰ��RtuStation����
	BOOL			 m_blResult;                        //��.������ز����Ƿ�ɹ��ı�־
public:
	void SetParentDlg(CRtuPropertyDlg* pDlg);
	void Update();										//���½������ʾ��Ϣ
	void UpdateDlg();									//���յ����ĺ󡢸��½���
	void KillAllTimers();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	void  SetWndsEnable(BOOL blEnable = TRUE);			//���ð�ť�Ƿ����

	DECLARE_MESSAGE_MAP()
public:
	CComboBoxXP m_wndPGTypeCbm;
	CButtonXP m_btmPGStart;
	CButtonXP m_btnPGClose;
	CButtonXP m_btnICEnable;
	CButtonXP m_btnICDisEnable;
	CButtonXP m_btnZdInput;
	CButtonXP m_btnZdExit;
	CComboBoxXP m_wndResetCmdCmb;
	CButtonXP m_btnReset;
	CButtonXP m_btnHisDataClear;

	CEdit     m_EditPgCode;

	CButton m_CheckClear0;
	CButton m_CheckClear1;
	CButton m_CheckClear2;
	CButton m_CheckClear3;
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnCbnSelchangeCmbPgtype();
	afx_msg void OnCbnSelchangeCmbResetcmd();
	afx_msg void OnBnClickedBtnPgstart();				//����ˮ�û�բ��/����
	afx_msg void OnBnClickedBtnPgclose();				//�ر�ˮ�û�բ��/����
	afx_msg void OnBnClickedBtnIcenable();				//��IC����Ч
	afx_msg void OnBnClickedBtnIcdisenable();			//ֵIC����Ч
	afx_msg void OnBnClickedBtnZdinput();				//��ֵ����Ͷ��
	afx_msg void OnBnClickedBtnZdexit();				//��ֵ�����˳�
	afx_msg void OnBnClickedBtnReset();					//ң���ն˸�λ
	afx_msg void OnBnClickedBtnClearhis();				//�����ʷ���ݵ�Ԫ
	afx_msg void OnBnClickedCheckClear0();
	afx_msg void OnBnClickedCheckClear1();
	afx_msg void OnBnClickedCheckClear2();
	afx_msg void OnBnClickedCheckClear3();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
