#pragma once
#include "afxwin.h"
#include "XpStyle/ButtonXP.h"
#include "XpStyle/ComboBoxXP.h"
#include "GridCtrl/GridCtrl.h"

// CRtuAttrWorkMode �Ի���
class CRtuPropertyDlg;
class CRtuAttrWorkMode : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrWorkMode)

public:
	CRtuAttrWorkMode(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRtuAttrWorkMode();

// �Ի�������
	enum { IDD = IDD_RTU_WORKMODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
protected:

	CButtonXP   m_btnYcQueryCall;
	CButtonXP   m_btnYcQuerySet;
	CGridCtrl   m_wndGridCtrl;
	void      InitGridCtrl();
	void	  OnCellNormal(int Row,int Col);
	void	  OnCellReadOnly(int Row,int Col);
	void      InitSetYcQuery(int Row,int Col);
	CString   GetCellText(int Row,int Col);

	CRtuPropertyDlg* m_pParentDlg;				//TabCtrl������
	CRtuStation*     m_pSubStation;				//��ǰ��RtuStation����
	BOOL      m_blResult;                       //��/���ò����ɹ�����־

	void  SetWndsEnable(BOOL blEnable = TRUE);  //���ð�ť�Ƿ����
public:
	virtual BOOL OnInitDialog();
	void SetParentDlg(CRtuPropertyDlg* pDlg);
	void Update();                           //���½������ʾ��Ϣ
	void UpdateDlg(int attrType,BOOL blResult);  //���յ����ĺ󡢸��½���
	void KillAllTimers();

public:
	afx_msg void OnBnClickedYcqueryCall();
	afx_msg void OnBnClickedYcquerySet();
	afx_msg void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
