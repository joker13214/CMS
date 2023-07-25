#pragma once
#include "afxwin.h"
#include "GridCtrl/GridCtrl.h"
#include "CommServer.h"
#include "XpStyle/ComboBoxXP.h"
#include "XpStyle/ButtonXP.h"

// CommServer �����߳����� �Ի���
// ��ӡ��޸��߳���Ϥʱʹ��

class CCommThreadProDlg : public CDialog
{
	DECLARE_DYNAMIC(CCommThreadProDlg)

public:
	CCommThreadProDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCommThreadProDlg();

// �Ի�������
	enum { IDD = IDD_DLG_COMMTHREADPRO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString		m_strThreadInfos;
	CComboBoxXP m_wndCmbCmType;
	CComboBoxXP m_wndCmbCmInterface;
	CGridCtrl   m_wndCmOptGridCtrl;	
	CComboBoxXP	m_wndCmbSccj;				// �����ṩ����
	CComboBoxXP m_wndCmbTextMsg;            // �����ṩ����
	CStatic		m_wndWxcjStatic;
	CStatic     m_wndTextMsgStatic;  
	HBRUSH      m_brMine;

	void SetCommThreadInfo(CCommThreadInfo* pThreadInfo);
protected:
	CCommThreadInfo* m_pCurCommThreadInfo;

	void  InitGridCtrl_ComInfo(BYTE CmType = 0);		//������ϢGridCtrl��Ϣ��ʾ����
	void  InitGridCtrl_NetInfo();						//������ϢGridCtrl��Ϣ��ʾ����
	void  OnCellNormal(int Row,int Col);
	void  OnCellReadOnly(int Row,int Col);

	void  InitComGridItem11();
	void  InitComGridItem21();
	void  InitComGridItem31();
	void  InitComGridItem41();
	void  InitComGridItem51();
	void  InitComGridItem61();

	void UpdateThreadInfo();   //��ȡ��ǰ���̵���Ϣ
	const CString GetCellText(int Row,int Col);
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCbnSelchangeCmbInterface();
	afx_msg void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//void UpdateAllDate(int bCmType,int bCmInterface);              //�������б�������Ϣ
public:
   COM_INFO m_Com_Info;
   NET_INFO m_Net_Info;
   SC_COMM_INFO m_Sc_Comm_Info;
   _SC_TEXTMSG_INFO_ m_Tm_Comm_Info;
   BYTE     m_bCom_Type;
   BYTE     m_bCom_Interface;
   BYTE     m_bProtocolType;
protected:
	CButtonXP m_btnOK;
	CButtonXP m_btnCancel;
public:
	CStatic m_wndGridParent;
public:
	afx_msg void OnCbnSelchangeCmbSccj();
	afx_msg void OnCbnSelchangeCmbCmtype();
};
