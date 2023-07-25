#pragma once



// CRtuAttrSyqcfg3 对话框

class CRtuAttrSyqcfg3 : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrSyqcfg3)

public:
	CRtuAttrSyqcfg3(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRtuAttrSyqcfg3();


	virtual BOOL OnInitDialog();
	void SetParentDlg(CRtuPropertyDlg* pDlg);
	void Update();

// 对话框数据
	enum { IDD = IDD_DLG_SYQ_CFG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_cfg_40;
	CEdit m_cfg_41;
	CEdit m_cfg_42;
	CEdit m_cfg_43;	
	CEdit m_cfg_69;
	CButtonXP m_ReadBtn;
	CButtonXP m_WriteBtn;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);

public:
	CRtuPropertyDlg* m_pParentDlg;				//TabCtrl父窗口
	CRtuStation*     m_pSubStation;				//当前的RtuStation对象

	BOOL  m_bResult;
	void  SetEnable(bool b);
	void  UpdateDlgMessage(int attr, bool result);
	void StringToChar(CString str,char *buf, int len1, int len2);

	afx_msg void OnBnClickedReadbtn();
	afx_msg void OnBnClickedWritebtn();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedChecktimeBtn();
	CEdit m_Static;
	CEdit m_Static2;

public:
	CButtonXP m_Btn_Cancel;
public:
	CButtonXP m_BtnCheckTime;

};
