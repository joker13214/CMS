#pragma once
#include "UdfLogSystem.h"
#include "afxcmn.h"
#include "UdfListCtrl.h"

// CUdfDataInDbLogDlg �Ի���

class CUdfDataInDbLogDlg : public CDialog
{
	DECLARE_DYNAMIC(CUdfDataInDbLogDlg)

public:
	CUdfDataInDbLogDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUdfDataInDbLogDlg();

// �Ի�������
	enum { IDD = IDD_DLG_DATAINDBLOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CUdfListCtrl m_wndListCtrl;
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);

public:
	//�����־��Ϣ��ListCtrl
	void  AddOneMessage(CString strMessage,PSYSTEMTIME pTime = NULL,_Log_level_type _xLevel = Normal_Log_Message);
	void  AddOneMessage(CString strMessage,CTime wTime,_Log_level_type _xLevel = Normal_Log_Message);
	void  AddOneMessage(CString strMessage,CString strTime,_Log_level_type _xLevel = Normal_Log_Message);

public:
	afx_msg void OnClose();
	afx_msg void OnNMRclickListDataindblog(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClearLogList();
	afx_msg void OnSaveAsLogList();   //������־��Ϣ�������ļ�
public:
	afx_msg void OnNMDblclkListDataindblog(NMHDR *pNMHDR, LRESULT *pResult);
};
