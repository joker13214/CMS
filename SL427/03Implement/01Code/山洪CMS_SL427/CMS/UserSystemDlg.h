#pragma once
#include "afxwin.h"
#include "GridCtrl/GridCtrl.h"
#include "XpStyle/ButtonXP.h"
#include "XpStyle/ComboBoxXP.h"
#include "UserObject.h"

// CUserSystemDlg �Ի���
class CUserSystemDlg : public CDialog
{
	DECLARE_DYNAMIC(CUserSystemDlg)

public:
	CUserSystemDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUserSystemDlg();

// �Ի�������
	enum { IDD = IDD_DLG_USERMN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void    InitUserGridCtrl();
	void    ShowAllUsers();
	void    ShowOneUserInfo(CUserObject* pUser,int ItemRow);
public:
	CButtonXP m_btnAddUser;
	CButtonXP m_btnBackUp;
protected:
	CGridCtrl m_wndGridCtrl;
public:
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtnAdduser();
	afx_msg void OnBnClickedBtnBackup();
	afx_msg void OnLClickGridCtrl(NMHDR *pNotifyStruct,LRESULT *pResult);
};
