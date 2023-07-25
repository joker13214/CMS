#pragma once
#include "afxcmn.h"
#include "UdfListCtrl.h"
#include "UdfLogSystem.h"
#include "XpStyle/ButtonXP.h"

// CCommServerMnDlg �Ի���

class CCommServerMnDlg : public CDialog
{
	DECLARE_DYNAMIC(CCommServerMnDlg)

public:
	CCommServerMnDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCommServerMnDlg();

	CImageList	m_wndImagelist;
	CGridCtrl	m_wndThreadsGrid;
	CUdfListCtrl m_wndThreadLog;

// �Ի�������
	enum { IDD = IDD_DLG_COMMSERVER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnRefresh();
	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnLClickThreadGrid(NMHDR *pNotifyStruct,LRESULT *pResult);
	afx_msg void OnBnClickedButton1();							//������߳�
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	virtual BOOL OnInitDialog();

public:
	bool InitGridHeader();
	void InitListCtrlHeader();
	bool ShowAllThreadsInfo();     //��ʾ���е��߳���Ϣ
	void ShowOneThreadInfo(CCommThreadInfo* pThread,int ItemRow);
	void UpdateThreadState(CCommThreadInfo* pThread,int ItemRow); //����GridCtrl���̵߳�����״̬��Ϣ
	void InsertLog(CString strLog,_Log_level_type xLevle=Normal_Log_Message,PSYSTEMTIME pST=NULL);		  //������־��Ϣ
    void SetTextMsgDlg(CRtuAttrTextMsg *pDlg);

protected:
	// �̲߳��������־��Ϣ
	CRtuAttrTextMsg *m_pTextMsgDlg;
	
public:
private:
	bool ModifyCommThreadPro(CCommThreadInfo* pThread);        //�޸��߳�  ����ֵ false δ�޸ġ��޸�ʧ��, true ���޸ġ����޸ĳɹ�
	bool DeleteCommThreadPro(CCommThreadInfo* pTHread);        //ɾ���߳�
	bool InitMsgCommThreadPro(CCommThreadInfo* pThread);       //��ʼ����������

	CButtonXP m_btnAdd;
	CButtonXP m_btnRefresh;
	CButtonXP m_btnOK;
};

