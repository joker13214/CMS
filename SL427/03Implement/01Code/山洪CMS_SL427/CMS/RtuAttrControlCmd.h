#pragma once
#include "afxwin.h"
#include "XpStyle/ButtonXP.h"
#include "XpStyle/ComboBoxXP.h"

// CRtuAttrControlCmd 对话框
class CRtuPropertyDlg;
class CRtuAttrControlCmd : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrControlCmd)

public:
	CRtuAttrControlCmd(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRtuAttrControlCmd();

// 对话框数据
	enum { IDD = IDD_RTU_CONTROLCMD };
protected:
	//所有的Tab页窗口都必须具备的成员函数和对象
	CRtuPropertyDlg* m_pParentDlg;                      //TabCtrl的父窗口
	CRtuStation*     m_pSubStation;						//此窗口当前的RtuStation对象
	BOOL			 m_blResult;                        //召.设置相关参数是否成功的标志
public:
	void SetParentDlg(CRtuPropertyDlg* pDlg);
	void Update();										//更新界面的显示信息
	void UpdateDlg();									//接收到报文后、更新界面
	void KillAllTimers();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void  SetWndsEnable(BOOL blEnable = TRUE);			//设置按钮是否可用

	DECLARE_MESSAGE_MAP()
public:
	CComboBoxXP m_wndPGTypeCbm;
	CButtonXP m_btmPGStart;
	CButtonXP m_btnPGClose;
	CButtonXP m_btnICEnable;
	CButtonXP m_btnICDisEnable;
	CButtonXP m_btnZdInput;
	CButtonXP m_btnZdExit;
	CComboBoxXP m_wndResetCmdCmb;
	CButtonXP m_btnReset;
	CButtonXP m_btnHisDataClear;

	CEdit     m_EditPgCode;

	CButton m_CheckClear0;
	CButton m_CheckClear1;
	CButton m_CheckClear2;
	CButton m_CheckClear3;
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnCbnSelchangeCmbPgtype();
	afx_msg void OnCbnSelchangeCmbResetcmd();
	afx_msg void OnBnClickedBtnPgstart();				//启动水泵或闸门/阀门
	afx_msg void OnBnClickedBtnPgclose();				//关闭水泵或闸门/阀门
	afx_msg void OnBnClickedBtnIcenable();				//置IC卡有效
	afx_msg void OnBnClickedBtnIcdisenable();			//值IC卡无效
	afx_msg void OnBnClickedBtnZdinput();				//定值功能投入
	afx_msg void OnBnClickedBtnZdexit();				//定值功能退出
	afx_msg void OnBnClickedBtnReset();					//遥测终端复位
	afx_msg void OnBnClickedBtnClearhis();				//清除历史数据单元
	afx_msg void OnBnClickedCheckClear0();
	afx_msg void OnBnClickedCheckClear1();
	afx_msg void OnBnClickedCheckClear2();
	afx_msg void OnBnClickedCheckClear3();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
