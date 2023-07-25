#pragma once
#include "RtuStation.h"
#include "afxwin.h"
#include "XpStyle/ButtonXP.h"
#include "XpStyle/ComboBoxXP.h"

// CRtuAttrBasicInfo 对话框
class CRtuPropertyDlg;
class CRtuAttrBasicInfo : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrBasicInfo)
public:
	CRtuAttrBasicInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRtuAttrBasicInfo();

// 对话框数据
	enum { IDD = IDD_RTU_BAINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

protected:
	
	CGridCtrl        m_wndCmOptGridCtrl;				//显示通信接口的详细信息

    //所有的Tab页窗口都必须具备的成员函数和对象
	CRtuPropertyDlg* m_pParentDlg;                      //TabCtrl的父窗口
	CRtuStation*     m_pSubStation;						//此窗口当前的RtuStation对象	

	BOOL			m_blResult;                         //召.设置相关参数是否成功的标志
public:
	void SetParentDlg(CRtuPropertyDlg* pDlg) {m_pParentDlg = pDlg;}
	void Update();								 //更新界面的显示信息
	void UpdateDlg(int attrType,BOOL blResult);  //接收到报文后、更新界面
	void UpdateCommInfo();						 //更新通信相关的信息
	void SetDateTime(CString strDateTime);

	void KillAllTimers();
public:
	virtual BOOL OnInitDialog();
protected:
	void  InitGridCtrl_ComInfo();		//串口信息设置
	void  InitGridCtrl_NetInfo();		//网卡信息设计
	void  OnCellNormal(int Row,int Col);
	void  OnCellReadOnly(int Row,int Col);
	void  SetWndsEnable(BOOL blEnable = TRUE);  //设置控件是否可用
public:
	CComboBoxXP m_wndCmbWorkMode;
	CButtonXP   m_btnWorkModeCall;
	CButtonXP   m_btnWorkModeSet;

	CStatic m_wndGridParent;
	CString m_strName;
	CString m_strPronvince;
	CString m_strRegion;
	CString m_strCounty;
	CString m_strA2;
	//CString m_strCommType;
	//CString m_strCommInterface;
	CComboBoxXP m_wndCmbConnType;
	CButtonXP m_btnConnCheck;
	CComboBoxXP m_wndCmbThreads;
	CButtonXP m_btnSelThread;
	CButtonXP m_btnDtCall;
	CButtonXP m_btnDtSet;
	CString	  m_strDt;					//遥测终端或中继站时钟信息
	CString   m_strBaCommThreadInfo;	//线程信息
	CEdit     m_EditRecvTime;
public:
	afx_msg void OnBnClickedConnCheck();
	afx_msg void OnBnClickedBtnSelthread();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnBnClickedDtCall();
	afx_msg void OnBnClickedDtSet();
	afx_msg void OnCbnSelchangeCmbWorkmode();
	afx_msg void OnBnClickedWorkmodeCall();
	afx_msg void OnBnClickedWorkmodeSet();
	afx_msg void OnTimer(UINT_PTR nIDEvent);



	
public:
	CEdit m_wndCmTypeEdit;
	CEdit m_wndInterfaceEdit;
};
