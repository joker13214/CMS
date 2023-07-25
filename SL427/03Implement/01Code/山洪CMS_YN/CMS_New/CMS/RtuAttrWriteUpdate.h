#pragma once


// CRtuAttrWriteUpdate 对话框

class CRtuAttrWriteUpdate : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrWriteUpdate)

public:
	CRtuAttrWriteUpdate(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRtuAttrWriteUpdate();

// 对话框数据
	enum { IDD = IDD_RTU_WRITEUPDATE };
public:
	CRtuPropertyDlg* m_pParentDlg;                      //TabCtrl的父窗口
	CRtuStation*     m_pSubStation;						//此窗口当前的RtuStation对象
	BOOL			 m_blResult;                        //召.设置相关参数是否成功的标志

	CButtonXP m_StartBtn;
	CButtonXP m_ReadFileBtn;
	CButtonXP m_SendBtn;
	CButtonXP m_CancleBtn;
	CButton m_CheckBox;
	CGridCtrl m_GridCtrl;
	CEdit     m_FileAddress;
	int m_nAttType;
	int m_nRepeatCnt;
	int m_nRepeatFailCnt;//重发失败计数器
	int m_nPackCntSum;
	bool m_IsStartBtnClicked;
	bool m_HasUpdateFile;
	bool m_bCheckBoxChecked;
	int m_nComInterface;
	bool m_bIsUpdating;



public:
	void SetParentDlg(CRtuPropertyDlg* pDlg);
	void Update();										//更新界面的显示信息
	void UpdateDlg(int attrType, int btnCmd, BOOL blResult, int CommInterface);			//接收到报文后、更新界面
	void KillAllTimers();
	void SetWndsEnable(BOOL blEnable, int ret);					//设置按钮是否可用
	void InitGridItems();								//GridCtrl显示

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnRead();
	afx_msg void OnBnClickedBtnSend();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedUpdate();
public:
	afx_msg void OnBnClickedCancle();
};
