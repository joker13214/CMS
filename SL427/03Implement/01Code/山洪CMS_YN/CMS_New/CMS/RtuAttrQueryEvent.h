#pragma once
#include "XpStyle/ButtonXP.h"
#include "GridCtrl/GridCtrl.h"

// CRtuAttrQueryEvent �Ի���
class CRtuPropertyDlg;
class CRtuAttrQueryEvent : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrQueryEvent)

public:
	CRtuAttrQueryEvent(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRtuAttrQueryEvent();

// �Ի�������
	enum { IDD = IDD_RTU_QUERYEVENT };
	CRtuPropertyDlg* m_pParentDlg;                      //TabCtrl�ĸ�����
	CRtuStation*     m_pSubStation;						//�˴��ڵ�ǰ��RtuStation����
	BOOL			 m_blResult;                        //��.������ز����Ƿ�ɹ��ı�־
public:
	void SetParentDlg(CRtuPropertyDlg* pDlg);
	void Update();										//���½������ʾ��Ϣ
	void UpdateDlg(int attrType,BOOL blResult);			//���յ����ĺ󡢸��½���
	void KillAllTimers();
	void SetWndsEnable(BOOL blEnable);					//���ð�ť�Ƿ����
	void InitGridCtrl();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	CGridCtrl m_wndGridCtrl;
	CButtonXP m_btnQueryEvent;

	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickBtnQueryEvent();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
