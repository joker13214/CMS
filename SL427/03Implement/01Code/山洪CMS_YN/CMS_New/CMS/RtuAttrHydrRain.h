#pragma once


// CRtuAttrHydrRain 对话框

class CRtuAttrHydrRain : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrHydrRain)

public:
	CRtuAttrHydrRain(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRtuAttrHydrRain();

// 对话框数据
	enum { IDD = IDD_RTU_RAIN_HYDR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

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
	virtual BOOL OnInitDialog();

public:
	afx_msg void OnBnClickedBtnRainacall();
	afx_msg void OnSelComboxChangeTimeInterval();
	afx_msg void OnCbnSelChangeTimeType();
	afx_msg void OnEndEditRainGrid(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	CGridCtrl m_wndRainGrid;
	CComboBoxXP m_wndComboBox;                           //TimeInterval
	CComboBoxXP m_wndTimeTypeCbx;
	CDateTimeCtrl m_wndDateStartDtc;                     //起始日期的DateTime控件
	CDateTimeCtrl m_wndDateEndDtc;                       //结束日期的DateTime控件
	CButtonXP m_btnQueryRain;
	CStatic       m_wndTimeStartLabel;                   //起始日期的Static文本标签
	CStatic       m_wndTimeEndLabel;                     //结束日期的Static文本标签
	SYSTEMTIME m_timeA;
	SYSTEMTIME m_timeB;
	afx_msg void OnStartDataTimeChange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEndDataTimeChange(NMHDR *pNMHDR, LRESULT *pResult);
};
