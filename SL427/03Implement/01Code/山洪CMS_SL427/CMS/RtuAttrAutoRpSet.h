#pragma once
#include "XpStyle/ButtonXP.h"
#include "XpStyle/ComboBoxXP.h"
#include "GridCtrl/GridCtrl.h"
#include "afxwin.h"

// CRtuAttrAutoRpSet �Ի���
class CRtuPropertyDlg;
class CRtuAttrAutoRpSet : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrAutoRpSet)

public:
	CRtuAttrAutoRpSet(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRtuAttrAutoRpSet();

// �Ի�������
	enum { IDD = IDD_RTU_AUTOSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CButtonXP m_btnAutoSet;
	CButtonXP m_btnAutoQuery;
	CGridCtrl m_wndGridCtrl;

	void SetParentDlg(CRtuPropertyDlg* pDlg) {m_pParentDlg = pDlg;}
	void Update();   
	void UpdateDlg(int attrType,BOOL blResult);  //���յ����ĺ󡢸��½���
	void KillAllTimers();
protected:
	CRtuPropertyDlg* m_pParentDlg;				//TabCtrl������
	CRtuStation*     m_pSubStation;				//��ǰ��RtuStation����

	BOOL      m_blResult;                       //��/���ò����ɹ�����־


	void	InitGridCtrl();						//��ʼ��GridCtrl��ʾ
	void	OnCellNormal(int Row,int Col);
	void	OnCellReadOnly(int Row,int Col);
	void	InitSetCellAuto(int Row,int Col);    //�����Զ��ϱ�/���ϱ���ʽ
	CString GetCellText(int Row,int Col);		 //��ȡCell��������Ϣ
	void  SetWndsEnable(BOOL blEnable = TRUE);  //���ð�ť�Ƿ����
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedBtnAutoset();
	afx_msg void OnBnClickedBtnAutoquery();
	afx_msg void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
