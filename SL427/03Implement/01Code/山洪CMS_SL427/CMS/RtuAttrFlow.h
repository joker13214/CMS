#pragma once


// CRtuAttrFlow �Ի���
class CRtuPropertyDlg;
class CRtuAttrFlow : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrFlow)

public:
	CRtuAttrFlow(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRtuAttrFlow();

// �Ի�������
	enum { IDD = IDD_RTU_FLOW };

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
	CEdit		m_Edit_FlowCount;
	CComboBoxXP m_wndFlowCmb;
	CGridCtrl   m_wndGridCtrl;

	CButtonXP   m_btnFlowOK;
	CButtonXP   m_btnSetFlowPara;
	CButtonXP   m_btnQueryFlowData;

	virtual BOOL OnInitDialog();

public:
	afx_msg void OnBnClickQueryFlowData();
	afx_msg void OnBnClickSetFlowPara();
	afx_msg void OnBnClickFlowOK();
	afx_msg void OnCbnSelchangeCmbFlow();
	afx_msg void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
