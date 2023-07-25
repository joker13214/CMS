#pragma once
#include "afxwin.h"
#include "XpStyle/ButtonXP.h"
#include "XpStyle/ComboBoxXP.h"
#include "GridCtrl/GridCtrl.h"

// CRtuAttrWorkMode 对话框
class CRtuPropertyDlg;
class CRtuAttrWorkMode : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrWorkMode)

public:
	CRtuAttrWorkMode(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRtuAttrWorkMode();

// 对话框数据
	enum { IDD = IDD_RTU_WORKMODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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

	CRtuPropertyDlg* m_pParentDlg;				//TabCtrl父窗口
	CRtuStation*     m_pSubStation;				//当前的RtuStation对象
	BOOL      m_blResult;                       //召/设置参数成功与否标志

	void  SetWndsEnable(BOOL blEnable = TRUE);  //设置按钮是否可用
public:
	virtual BOOL OnInitDialog();
	void SetParentDlg(CRtuPropertyDlg* pDlg);
	void Update();                           //更新界面的显示信息
	void UpdateDlg(int attrType,BOOL blResult);  //接收到报文后、更新界面
	void KillAllTimers();

public:
	afx_msg void OnBnClickedYcqueryCall();
	afx_msg void OnBnClickedYcquerySet();
	afx_msg void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
