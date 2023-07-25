#pragma once
#include "afxwin.h"
#include "GridCtrl/GridCtrl.h"
#include "CommServer.h"
#include "XpStyle/ComboBoxXP.h"
#include "XpStyle/ButtonXP.h"

// CommServer 服务线程属性 对话框
// 添加、修改线程熟悉时使用

class CCommThreadProDlg : public CDialog
{
	DECLARE_DYNAMIC(CCommThreadProDlg)

public:
	CCommThreadProDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCommThreadProDlg();

// 对话框数据
	enum { IDD = IDD_DLG_COMMTHREADPRO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString		m_strThreadInfos;
	CComboBoxXP m_wndCmbCmType;
	CComboBoxXP m_wndCmbCmInterface;
	CGridCtrl   m_wndCmOptGridCtrl;	
	CComboBoxXP	m_wndCmbSccj;				// 卫星提供厂家
	CComboBoxXP m_wndCmbTextMsg;            // 短信提供厂家
	CStatic		m_wndWxcjStatic;
	CStatic     m_wndTextMsgStatic;  
	HBRUSH      m_brMine;

	void SetCommThreadInfo(CCommThreadInfo* pThreadInfo);
protected:
	CCommThreadInfo* m_pCurCommThreadInfo;

	void  InitGridCtrl_ComInfo(BYTE CmType = 0);		//串口信息GridCtrl信息显示设置
	void  InitGridCtrl_NetInfo();						//网卡信息GridCtrl信息显示设置
	void  OnCellNormal(int Row,int Col);
	void  OnCellReadOnly(int Row,int Col);

	void  InitComGridItem11();
	void  InitComGridItem21();
	void  InitComGridItem31();
	void  InitComGridItem41();
	void  InitComGridItem51();
	void  InitComGridItem61();

	void UpdateThreadInfo();   //获取当前进程的信息
	const CString GetCellText(int Row,int Col);
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCbnSelchangeCmbInterface();
	afx_msg void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//void UpdateAllDate(int bCmType,int bCmInterface);              //更新所有变量的信息
public:
   COM_INFO m_Com_Info;
   NET_INFO m_Net_Info;
   SC_COMM_INFO m_Sc_Comm_Info;
   _SC_TEXTMSG_INFO_ m_Tm_Comm_Info;
   BYTE     m_bCom_Type;
   BYTE     m_bCom_Interface;
   BYTE     m_bProtocolType;
protected:
	CButtonXP m_btnOK;
	CButtonXP m_btnCancel;
public:
	CStatic m_wndGridParent;
public:
	afx_msg void OnCbnSelchangeCmbSccj();
	afx_msg void OnCbnSelchangeCmbCmtype();
};
