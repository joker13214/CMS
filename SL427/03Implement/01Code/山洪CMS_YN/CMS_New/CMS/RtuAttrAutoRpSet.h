#pragma once
#include "XpStyle/ButtonXP.h"
#include "XpStyle/ComboBoxXP.h"
#include "GridCtrl/GridCtrl.h"
#include "afxwin.h"

// CRtuAttrAutoRpSet 对话框
class CRtuPropertyDlg;
class CRtuAttrAutoRpSet : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrAutoRpSet)

public:
	CRtuAttrAutoRpSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRtuAttrAutoRpSet();

// 对话框数据
	enum { IDD = IDD_RTU_AUTOSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CButtonXP m_btnAutoSet;
	CButtonXP m_btnAutoQuery;
	CGridCtrl m_wndGridCtrl;

	void SetParentDlg(CRtuPropertyDlg* pDlg) {m_pParentDlg = pDlg;}
	void Update();   
	void UpdateDlg(int attrType,BOOL blResult);  //接收到报文后、更新界面
	void KillAllTimers();
protected:
	CRtuPropertyDlg* m_pParentDlg;				//TabCtrl父窗口
	CRtuStation*     m_pSubStation;				//当前的RtuStation对象

	BOOL      m_blResult;                       //召/设置参数成功与否标志


	void	InitGridCtrl();						//初始化GridCtrl显示
	void	OnCellNormal(int Row,int Col);
	void	OnCellReadOnly(int Row,int Col);
	void	InitSetCellAuto(int Row,int Col);    //设置自动上报/不上报格式
	CString GetCellText(int Row,int Col);		 //获取Cell的文字信息
	void  SetWndsEnable(BOOL blEnable = TRUE);  //设置按钮是否可用
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedBtnAutoset();
	afx_msg void OnBnClickedBtnAutoquery();
	afx_msg void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
