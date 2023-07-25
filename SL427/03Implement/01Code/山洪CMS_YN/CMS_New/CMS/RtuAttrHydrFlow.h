#pragma once


// CRtuAttrHydrFlow �Ի���

class CRtuAttrHydrFlow : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrHydrFlow)

public:
	CRtuAttrHydrFlow(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRtuAttrHydrFlow();

// �Ի�������
	enum { IDD = IDD_RTU_FLOW_HYDR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
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
	void InitGridItems();		
public:
	CComboBoxXP m_wndSensorNoCbx;                        //�������ŵ�Combox�ؼ�
	CComboBoxXP m_wndTimeTypeCbx;                        //ʱ�����͵�Combox�ؼ�
	CDateTimeCtrl m_wndDateStartDtc;                     //��ʼ���ڵ�DateTime�ؼ�
	CDateTimeCtrl m_wndDateEndDtc;                       //�������ڵ�DateTime�ؼ�
	CStatic       m_wndTimeStartLabel;                   //��ʼ���ڵ�Static�ı���ǩ
	CStatic       m_wndTimeEndLabel;                     //�������ڵ�Static�ı���ǩ
	CGridCtrl   m_wndGridCtrl;                           //���ӿؼ�

public:
	afx_msg void OnCbnSelChangeTimeType();                       //time type cbx change
	afx_msg void OnCbnSelChangeSensorNo();
	afx_msg void OnBnClickQueryWaterLevel();
	afx_msg void OnBnClickSetWaterLevel();
	afx_msg void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
