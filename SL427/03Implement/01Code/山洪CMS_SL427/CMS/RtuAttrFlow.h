#pragma once


// CRtuAttrFlow 对话框
class CRtuPropertyDlg;
class CRtuAttrFlow : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrFlow)

public:
	CRtuAttrFlow(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRtuAttrFlow();

// 对话框数据
	enum { IDD = IDD_RTU_FLOW };

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
	CEdit		m_Edit_FlowCount;
	CComboBoxXP m_wndFlowCmb;
	CGridCtrl   m_wndGridCtrl;

	CButtonXP   m_btnFlowOK;
	CButtonXP   m_btnSetFlowPara;
	CButtonXP   m_btnQueryFlowData;

	virtual BOOL OnInitDialog();

public:
	afx_msg void OnBnClickQueryFlowData();
	afx_msg void OnBnClickSetFlowPara();
	afx_msg void OnBnClickFlowOK();
	afx_msg void OnCbnSelchangeCmbFlow();
	afx_msg void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
