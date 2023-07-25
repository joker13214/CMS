#pragma once


// CRtuAttrHydrRain �Ի���

class CRtuAttrHydrRain : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrHydrRain)

public:
	CRtuAttrHydrRain(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRtuAttrHydrRain();

// �Ի�������
	enum { IDD = IDD_RTU_RAIN_HYDR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

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
	virtual BOOL OnInitDialog();

public:
	afx_msg void OnBnClickedBtnRainacall();
	afx_msg void OnSelComboxChangeTimeInterval();
	afx_msg void OnCbnSelChangeTimeType();
	afx_msg void OnEndEditRainGrid(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	CGridCtrl m_wndRainGrid;
	CComboBoxXP m_wndComboBox;                           //TimeInterval
	CComboBoxXP m_wndTimeTypeCbx;
	CDateTimeCtrl m_wndDateStartDtc;                     //��ʼ���ڵ�DateTime�ؼ�
	CDateTimeCtrl m_wndDateEndDtc;                       //�������ڵ�DateTime�ؼ�
	CButtonXP m_btnQueryRain;
	CStatic       m_wndTimeStartLabel;                   //��ʼ���ڵ�Static�ı���ǩ
	CStatic       m_wndTimeEndLabel;                     //�������ڵ�Static�ı���ǩ
	SYSTEMTIME m_timeA;
	SYSTEMTIME m_timeB;
	afx_msg void OnStartDataTimeChange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEndDataTimeChange(NMHDR *pNMHDR, LRESULT *pResult);
};
