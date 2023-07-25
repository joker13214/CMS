#pragma once


// CRtuAttrHydrInDepthGW 对话框
//地下水瞬时埋深
class CRtuAttrHydrInDepthGW : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrHydrInDepthGW)

public:
	CRtuAttrHydrInDepthGW(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRtuAttrHydrInDepthGW();

// 对话框数据
	enum { IDD = IDD_RTU_INSTANTANEOUS_DEPTH_GROUNDWATER_HYDR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	void SetParentDlg(CRtuPropertyDlg* pDlg);
	void Update();
	void UpdateDlg(int attrType,BOOL blResult);			//接收到报文后、更新界面
	void KillAllTimers();
	void SetWndsEnable(BOOL blEnable);					//设置按钮是否可用
	void InitGridItems();								//GridCtrl显示

private:
	CRtuPropertyDlg *m_pParentDlg;
	CRtuStation*     m_pSubStation;						//此窗口当前的RtuStation对象
	BOOL			 m_blResult;                        //召.设置相关参数是否成功的标志
	CComboBoxXP m_wndTimeTypeCbx;                        //时间类型的Combox控件
	CDateTimeCtrl m_wndDateStartDtc;                     //起始日期的DateTime控件
	CDateTimeCtrl m_wndDateEndDtc;                       //结束日期的DateTime控件
	CStatic       m_wndTimeStartLabel;                   //起始日期的Static文本标签
	CStatic       m_wndTimeEndLabel;                     //结束日期的Static文本标签
	CGridCtrl   m_wndGridCtrl;                           //格子控件

private:
	afx_msg void OnCbnSelChangeTimeType();
	afx_msg void OnBtnClickedQueryGWD();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
