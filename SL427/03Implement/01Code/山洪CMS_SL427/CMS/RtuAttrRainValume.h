#pragma once


// CRtuAttrRainValume 对话框
class CRtuPropertyDlg;
class CRtuAttrRainValume : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrRainValume)

public:
	CRtuAttrRainValume(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRtuAttrRainValume();

// 对话框数据
	enum { IDD = IDD_RTU_RAINVALUME };
protected:
	//所有的Tab页窗口都必须具备的成员函数和对象
	CRtuPropertyDlg* m_pParentDlg;                      //TabCtrl的父窗口
	CRtuStation*     m_pSubStation;						//此窗口当前的RtuStation对象
	BOOL			 m_blResult;                        //召.设置相关参数是否成功的标志

	
public:
	void SetParentDlg(CRtuPropertyDlg* pDlg);
	void Update();										//更新界面的显示信息
	void UpdateDlg(int attrType,BOOL blResult);			//接收到报文后、更新界面
	void KillAllTimers();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_wndRainPCount;
	CComboBoxXP m_wndRainCmb;
	CButtonXP m_btnRainACall;
	CButtonXP m_btnRainASet;
	CStatic m_wndRainCaption;
	CButtonXP m_btnRainOK;
	CButtonXP m_btnQueryRainReal;
	int m_nRainCount;
	CGridCtrl m_wndRainGrid;
public:
	virtual BOOL OnInitDialog();
	void		FillRainGridCtrl();
	void		SetWndCtrlEnable(BOOL blEnable);

public:
	afx_msg void OnCbnSelchangeComboRainpara();
	afx_msg void OnBnClickedBtnRainacall();
	afx_msg void OnBnClickedBtnRainaset();
	afx_msg void OnBnClickedBtnRainok();
	afx_msg void OnBnClickBtnQueryRainReal();
	afx_msg void OnEndEditRainGrid(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
