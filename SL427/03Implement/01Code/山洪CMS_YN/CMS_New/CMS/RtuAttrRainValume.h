#pragma once


// CRtuAttrRainValume �Ի���
class CRtuPropertyDlg;
class CRtuAttrRainValume : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrRainValume)

public:
	CRtuAttrRainValume(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRtuAttrRainValume();

// �Ի�������
	enum { IDD = IDD_RTU_RAINVALUME };
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
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_wndRainPCount;
	CComboBoxXP m_wndRainCmb;
	CButtonXP m_btnRainACall;
	CButtonXP m_btnRainASet;
	CStatic m_wndRainCaption;
	CButtonXP m_btnRainOK;
	CButtonXP m_btnQueryRainReal;
	int m_nRainCount;
	CGridCtrl m_wndRainGrid;
public:
	virtual BOOL OnInitDialog();
	void		FillRainGridCtrl();
	void		SetWndCtrlEnable(BOOL blEnable);

public:
	afx_msg void OnCbnSelchangeComboRainpara();
	afx_msg void OnBnClickedBtnRainacall();
	afx_msg void OnBnClickedBtnRainaset();
	afx_msg void OnBnClickedBtnRainok();
	afx_msg void OnBnClickBtnQueryRainReal();
	afx_msg void OnEndEditRainGrid(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
