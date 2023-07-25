#pragma once


// CRtuAttrWaterPress 对话框
class CRtuPropertyDlg;
class CRtuAttrWaterPress : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrWaterPress)

public:
	CRtuAttrWaterPress(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRtuAttrWaterPress();

// 对话框数据
	enum { IDD = IDD_RTU_WATERPRESS };
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
	void SetWndsEnable(BOOL blEnable);					//设置按钮是否可用
	void InitGridItems();								//GridCtrl显示
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CEdit		m_Edit_WPressCount;
	CComboBoxXP m_wndWaterPressCmb;
	CGridCtrl   m_wndGridCtrl;

	CButtonXP m_btnQueryWPress;
	CButtonXP m_btnSetWPress;
	CButtonXP m_btnQueryWPressData;
	CButtonXP m_btnWPressOK;

	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickQueryWPress();
	afx_msg void OnBnClickQueryWPressData();
	afx_msg void OnBnClickSetWPress();
	afx_msg void OnBnClickWPressOK();
	afx_msg void OnCbnSelchangeCmbWPress();
	afx_msg void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
