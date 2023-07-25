#pragma once


// CRtuAttrDataShow �Ի���
class CRtuPropertyDlg;

class CRtuAttrDataShow : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrDataShow)

public:
	CRtuAttrDataShow(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRtuAttrDataShow();
	void  Update();							  //��ʾ��Ϣ����		
	void  SetParentDlg(CRtuPropertyDlg* pParent) {m_pParentDlg = pParent;}
	void  AddMessageToWindow(CString strTime,CString strMessage);   //�����Ϣ��GridCtrl��ʾ
	void  AddMessageToWindow(CString strMessage,PSYSTEMTIME pTime = NULL);

// �Ի�������
	enum { IDD = IDD_RTU_DATASHOW };
protected:
	CRtuPropertyDlg* m_pParentDlg;                      //TabCtrl�ĸ�����
	CRtuStation*     m_pSubStation;						//�˴��ڵ�ǰ��RtuStation����

	int				 m_nRowsMax;						//��ʾ������Ϣ�����������Ĭ��Ϊ100
public:
	CGridCtrl        m_wndGridCtrl;                     //��ʾ������Ϣ��GridCtrl
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	BOOL    OnInitGridCtrl();
public:
	void    InitGridHeader();							//��ʼ��GridCtrl��headers

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

public:
	afx_msg void OnRClickGridCtrl(NMHDR* pNotifyStruct,LRESULT *pResutl);//GridCtrl���Ҽ��¼�
	afx_msg void OnClearMessage();						//���������Ϣ
	afx_msg void OnSaveAsMessage();						//����������Ϣ

};
