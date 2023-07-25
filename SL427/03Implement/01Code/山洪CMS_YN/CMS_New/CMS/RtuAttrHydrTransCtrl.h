#pragma once


// RtuAttrHydrTransCtrl 对话框

class RtuAttrHydrTransCtrl : public CDialog
{
	DECLARE_DYNAMIC(RtuAttrHydrTransCtrl)

public:
	RtuAttrHydrTransCtrl(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~RtuAttrHydrTransCtrl();

// 对话框数据
	enum { IDD = IDD_RTU_QUERY_TRANSCTRL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

protected:
	CRtuPropertyDlg* m_pParentDlg;				//TabCtrl父窗口
	CRtuStation*     m_pSubStation;				//当前的RtuStation对象
	BOOL      m_blResult;                       //召/设置参数成功与否标志
	CButtonXP m_btnQuery_rainStardard;
	CButtonXP m_btnSet_rainStardard;
	CButtonXP m_btnQuery_rainSensorDpi;
	CButtonXP m_btnSet_rainSensorDpi;
	CButtonXP m_btnQuery_sensorBase;
	CButtonXP m_btnSet_sensorBase;
	CButtonXP m_btnQuery_sensorPara;
	CButtonXP m_btnSet_sensorPara;
	CButtonXP m_btnQuery_sensorDef;
	CButtonXP m_btnSet_SensorDef;
	CButtonXP m_btnQuery_hydrSendCtrl;
	CButtonXP m_btnSet_hydrSendCtrl;
	CButtonXP m_btnQuery_hydrTimeSpan;
	CButtonXP m_btnSet_hydrTimeSpan;

	CEdit m_let_rainStardard;
	CEdit m_let_rainSensorDpi;

	CComboBoxXP m_Cbx_SensorNo_rainbase;
	CEdit m_let_rainBaseVar;

	CComboBoxXP m_Cbx_SensorNo_rainPara;
	CEdit m_let_rainParaVar;

	CComboBoxXP m_Cbx_SensorNo_rainDef;
	//CEdit m_let_rainDefVar;
	CComboBoxXP m_cbx_rainDefVar;

	CComboBoxXP m_Cbx_SensorNo_sendStardard;
	CComboBoxXP m_Cbx_Level_sendStardard;
	CEdit m_let_SendStardardVar;

	CComboBoxXP m_Cbx_SensorNo_sendTimeSpan;
	CComboBoxXP m_Cbx_Level_sendTimeSpan;
	CEdit m_let_SendTimeSpanVar;

	CButtonXP m_btn_CheckTime;
	CEdit m_PswEdit;
	CString m_strPsw;

	CString m_str_rainStardard;
	CString m_str_rainSensorDpi;
	CString m_str_rainBaseVar;
	CString m_str_rainParaVar;
	CString m_str_SendStardardVar;
	CString m_str_SendTimeSpanVar;

	

	void  SetWndsEnable(BOOL blEnable = TRUE);  //设置按钮是否可用
public:
	virtual BOOL OnInitDialog();
	void SetParentDlg(CRtuPropertyDlg* pDlg);
	void Update(); //更新界面的显示信息
	void Update(int ret);
	void UpdateDlg(int attrType,BOOL blResult);  //接收到报文后、更新界面
	void KillAllTimers();

public:
	afx_msg void OnBnClickedBtnQueryRainstardard();
	afx_msg void OnBnClickedBtnSetRainstardard();
	afx_msg void OnBnClickedBtnQueryRainsensordpi();
	afx_msg void OnBnClickedBtnSetRainsensordpi();
	afx_msg void OnBnClickedBtnQuerySensorbase();
	afx_msg void OnBnClickedBtnSetSensorbase();
	afx_msg void OnBnClickedBtnQuerySensorpara();
	afx_msg void OnBnClickedBtnSetSensorpara();
	afx_msg void OnBnClickedBtnQuerySensordef();
	afx_msg void OnBnClickedBtnSetSensordef();
	afx_msg void OnBnClickedBtnQueryHydrsendctrl();
	afx_msg void OnBnClickedBtnSetHydrsendctrl();
	afx_msg void OnBnClickedBtnQueryHydrtimespan();
	afx_msg void OnBnClickedBtnSetHydrtimespan();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	afx_msg void OnEnChangeLetPsw();
public:
	afx_msg void OnBnClickedBtnChecktime();
};
