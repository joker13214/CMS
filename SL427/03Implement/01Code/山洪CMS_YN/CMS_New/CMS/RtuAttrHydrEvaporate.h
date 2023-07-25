#pragma once


// CRtuAttrHydrEvaporate �Ի���

class CRtuAttrHydrEvaporate : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrHydrEvaporate)

public:
	CRtuAttrHydrEvaporate(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRtuAttrHydrEvaporate();

// �Ի�������
	enum { IDD = IDD_RTU_EVAPORATE_HYDR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	void SetParentDlg(CRtuPropertyDlg* pDlg);
	void Update();										//���½������ʾ��Ϣ
	void UpdateDlg(int attrType,BOOL blResult);			//���յ����ĺ󡢸��½���
	void KillAllTimers();
	void SetWndsEnable(BOOL blEnable);					//���ð�ť�Ƿ����
	void InitGridItems();								//GridCtrl��ʾ

private:
	CRtuPropertyDlg *m_pParentDlg;
	CRtuStation*     m_pSubStation;						//�˴��ڵ�ǰ��RtuStation����
	BOOL			 m_blResult;                        //��.������ز����Ƿ�ɹ��ı�־
	CComboBoxXP m_wndTimeTypeCbx;                        //ʱ�����͵�Combox�ؼ�
	CDateTimeCtrl m_wndDateStartDtc;                     //��ʼ���ڵ�DateTime�ؼ�
	CDateTimeCtrl m_wndDateEndDtc;                       //�������ڵ�DateTime�ؼ�
	CStatic       m_wndTimeStartLabel;                   //��ʼ���ڵ�Static�ı���ǩ
	CStatic       m_wndTimeEndLabel;                     //�������ڵ�Static�ı���ǩ
	CGridCtrl   m_wndGridCtrl;                           //���ӿؼ�

private:
	afx_msg void OnCbnSelChangeTimeType();
	afx_msg void OnBtnClickedQueryEvaporate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
