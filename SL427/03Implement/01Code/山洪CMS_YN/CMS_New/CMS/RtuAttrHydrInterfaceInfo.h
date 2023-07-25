#pragma once


// CRtuAttrHydrInterfaceInfo �Ի���

class CRtuAttrHydrInterfaceInfo : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrHydrInterfaceInfo)

public:
	CRtuAttrHydrInterfaceInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRtuAttrHydrInterfaceInfo();

// �Ի�������
	enum { IDD = IDD_RTU_QUERY_INTERFACE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:

	CRtuPropertyDlg* m_pParentDlg;				//TabCtrl������
	CRtuStation*     m_pSubStation;				//��ǰ��RtuStation����
	BOOL      m_blResult;                       //��/���ò����ɹ�����־

	CButtonXP m_btn_QueryGSM;
	CButtonXP m_btn_SetGSM;
	CButtonXP m_btn_DelGSM;
	CComboBoxXP m_Cbx_GSM;
	CEdit m_Let_GSM;
	CString m_str_GSM;

	CButtonXP m_btn_QueryBD;
	CButtonXP m_btn_SetBD;
	CButtonXP m_btn_DelBD;
	CComboBoxXP m_Cbx_BD;
	CEdit m_Let_BD;
	CString m_str_BD;

	CButtonXP m_btn_QueryIP;
	CButtonXP m_btn_SetIP;
	CButtonXP m_btn_DelIP;
	CButtonXP m_btn_QueryIP2;
	CButtonXP m_btn_SetIP2;
	CButtonXP m_btn_DelIP2;
	CComboBoxXP m_Cbx_IP;
	CComboBoxXP m_Cbx_IP2;
	CEdit m_Let_IP;
	CEdit m_Let_IP2;
	CEdit m_Let_PSW;
	CString m_str_psw;
	CString m_str_IP;
	CString m_str_IP2;

	CButtonXP m_btn_QueryPort;
	CButtonXP m_btn_SetPort;
	CComboBoxXP m_Cbx_Port;
	CEdit m_Let_Port;
	CString m_str_Port;

	void  SetWndsEnable(BOOL blEnable = TRUE);  //���ð�ť�Ƿ����
public:
	virtual BOOL OnInitDialog();
	void SetParentDlg(CRtuPropertyDlg* pDlg);
	void Update(); //���½������ʾ��Ϣ
	void Update(int ret);
	void UpdateDlg(int attrType,BOOL blResult);  //���յ����ĺ󡢸��½���
	void KillAllTimers();


public:
	afx_msg void OnBnClickedBtnQueryGSM();
	afx_msg void OnBnClickedBtnDelGSM();
	afx_msg void OnBnClickedBtnSetGSM();
	afx_msg void OnBnClickedBtnQueryBD();
	afx_msg void OnBnClickedBtnDelBD();
	afx_msg void OnBnClickedBtnSetBD();
	afx_msg void OnBnClickedBtnQueryIP();
	afx_msg void OnBnClickedBtnDelIP();
	afx_msg void OnBnClickedBtnSetIP();
	afx_msg void OnBnClickedBtnQueryPort();
	afx_msg void OnBnClickedBtnSetPort();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	afx_msg void OnBnClickedBtnQueryIp2();
public:
	afx_msg void OnBnClickedBtnDelIp2();
public:
	afx_msg void OnBnClickedBtnSetIp2();
};
