#pragma once


// CRtuAttrWriteUpdate �Ի���

class CRtuAttrWriteUpdate : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrWriteUpdate)

public:
	CRtuAttrWriteUpdate(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRtuAttrWriteUpdate();

// �Ի�������
	enum { IDD = IDD_RTU_WRITEUPDATE };
public:
	CRtuPropertyDlg* m_pParentDlg;                      //TabCtrl�ĸ�����
	CRtuStation*     m_pSubStation;						//�˴��ڵ�ǰ��RtuStation����
	BOOL			 m_blResult;                        //��.������ز����Ƿ�ɹ��ı�־

	CButtonXP m_StartBtn;
	CButtonXP m_ReadFileBtn;
	CButtonXP m_SendBtn;
	CButtonXP m_CancleBtn;
	CButton m_CheckBox;
	CGridCtrl m_GridCtrl;
	CEdit     m_FileAddress;
	int m_nAttType;
	int m_nRepeatCnt;
	int m_nRepeatFailCnt;//�ط�ʧ�ܼ�����
	int m_nPackCntSum;
	bool m_IsStartBtnClicked;
	bool m_HasUpdateFile;
	bool m_bCheckBoxChecked;
	int m_nComInterface;
	bool m_bIsUpdating;



public:
	void SetParentDlg(CRtuPropertyDlg* pDlg);
	void Update();										//���½������ʾ��Ϣ
	void UpdateDlg(int attrType, int btnCmd, BOOL blResult, int CommInterface);			//���յ����ĺ󡢸��½���
	void KillAllTimers();
	void SetWndsEnable(BOOL blEnable, int ret);					//���ð�ť�Ƿ����
	void InitGridItems();								//GridCtrl��ʾ

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnRead();
	afx_msg void OnBnClickedBtnSend();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedUpdate();
public:
	afx_msg void OnBnClickedCancle();
};
