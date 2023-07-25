#pragma once
#include "RtuStation.h"
#include "afxwin.h"
#include "XpStyle/ButtonXP.h"
#include "XpStyle/ComboBoxXP.h"

// CRtuAttrBasicInfo �Ի���
class CRtuPropertyDlg;
class CRtuAttrBasicInfo : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrBasicInfo)
public:
	CRtuAttrBasicInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRtuAttrBasicInfo();

// �Ի�������
	enum { IDD = IDD_RTU_BAINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

protected:
	
	CGridCtrl        m_wndCmOptGridCtrl;				//��ʾͨ�Žӿڵ���ϸ��Ϣ

    //���е�Tabҳ���ڶ�����߱��ĳ�Ա�����Ͷ���
	CRtuPropertyDlg* m_pParentDlg;                      //TabCtrl�ĸ�����
	CRtuStation*     m_pSubStation;						//�˴��ڵ�ǰ��RtuStation����	

	BOOL			m_blResult;                         //��.������ز����Ƿ�ɹ��ı�־
public:
	void SetParentDlg(CRtuPropertyDlg* pDlg) {m_pParentDlg = pDlg;}
	void Update();								 //���½������ʾ��Ϣ
	void UpdateDlg(int attrType,BOOL blResult);  //���յ����ĺ󡢸��½���
	void UpdateCommInfo();						 //����ͨ����ص���Ϣ
	void SetDateTime(CString strDateTime);

	void KillAllTimers();
public:
	virtual BOOL OnInitDialog();
protected:
	void  InitGridCtrl_ComInfo();		//������Ϣ����
	void  InitGridCtrl_NetInfo();		//������Ϣ���
	void  OnCellNormal(int Row,int Col);
	void  OnCellReadOnly(int Row,int Col);
	void  SetWndsEnable(BOOL blEnable = TRUE);  //���ÿؼ��Ƿ����
public:
	CComboBoxXP m_wndCmbWorkMode;
	CButtonXP   m_btnWorkModeCall;
	CButtonXP   m_btnWorkModeSet;

	CStatic m_wndGridParent;
	CString m_strName;
	CString m_strPronvince;
	CString m_strRegion;
	CString m_strCounty;
	CString m_strA2;
	//CString m_strCommType;
	//CString m_strCommInterface;
	CComboBoxXP m_wndCmbConnType;
	CButtonXP m_btnConnCheck;
	CComboBoxXP m_wndCmbThreads;
	CButtonXP m_btnSelThread;
	CButtonXP m_btnDtCall;
	CButtonXP m_btnDtSet;
	CString	  m_strDt;					//ң���ն˻��м�վʱ����Ϣ
	CString   m_strBaCommThreadInfo;	//�߳���Ϣ
	CEdit     m_EditRecvTime;
public:
	afx_msg void OnBnClickedConnCheck();
	afx_msg void OnBnClickedBtnSelthread();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnBnClickedDtCall();
	afx_msg void OnBnClickedDtSet();
	afx_msg void OnCbnSelchangeCmbWorkmode();
	afx_msg void OnBnClickedWorkmodeCall();
	afx_msg void OnBnClickedWorkmodeSet();
	afx_msg void OnTimer(UINT_PTR nIDEvent);



	
public:
	CEdit m_wndCmTypeEdit;
	CEdit m_wndInterfaceEdit;
};
