#pragma once


// CRtuAttrHydrWaterLevel �Ի���

class CRtuAttrHydrWaterLevel : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrHydrWaterLevel)

public:
	CRtuAttrHydrWaterLevel(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRtuAttrHydrWaterLevel();

// �Ի�������
	enum { IDD = IDD_RTU_WATERLEVEL_HYDR };

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
public:
	CComboBoxXP m_wndSensorNoCbx;                        //�������ŵ�Combox�ؼ�
	CComboBoxXP m_wndTimeTypeCbx;                        //ʱ�����͵�Combox�ؼ�
	CDateTimeCtrl m_wndDateStartDtc;                     //��ʼ���ڵ�DateTime�ؼ�
	CDateTimeCtrl m_wndDateEndDtc;                       //�������ڵ�DateTime�ؼ�
	CStatic       m_wndTimeStartLabel;                   //��ʼ���ڵ�Static�ı���ǩ
	CStatic       m_wndTimeEndLabel;                     //�������ڵ�Static�ı���ǩ
	CGridCtrl   m_wndGridCtrl;                           //���ӿؼ�

	CButtonXP m_btnQueryWaterLevel;
	CButtonXP m_btnSetWaterLevel;
	CButtonXP m_btnQueryWaterLevelData;
	CButtonXP m_btnWaterLevelOK;
	SYSTEMTIME m_timeA;
	SYSTEMTIME m_timeB;

	virtual BOOL OnInitDialog();

public:
	afx_msg void OnCbnSelChangeTimeType();                       //time type cbx change
	afx_msg void OnCbnSelChangeSensorNo();
	afx_msg void OnBnClickQueryWaterLevel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnStartDataTimeChange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEndDataTimeChange(NMHDR *pNMHDR, LRESULT *pResult);
};
