#pragma once


// CRtuAttrWaterPress �Ի���
class CRtuPropertyDlg;
class CRtuAttrWaterPress : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrWaterPress)

public:
	CRtuAttrWaterPress(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRtuAttrWaterPress();

// �Ի�������
	enum { IDD = IDD_RTU_WATERPRESS };
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
	CEdit		m_Edit_WPressCount;
	CComboBoxXP m_wndWaterPressCmb;
	CGridCtrl   m_wndGridCtrl;

	CButtonXP m_btnQueryWPress;
	CButtonXP m_btnSetWPress;
	CButtonXP m_btnQueryWPressData;
	CButtonXP m_btnWPressOK;

	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickQueryWPress();
	afx_msg void OnBnClickQueryWPressData();
	afx_msg void OnBnClickSetWPress();
	afx_msg void OnBnClickWPressOK();
	afx_msg void OnCbnSelchangeCmbWPress();
	afx_msg void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
