#pragma once

#include "afxwin.h"
#include "XpStyle/ButtonXP.h"
#include "XpStyle/ComboBoxXP.h"
#include "GridCtrl/GridCtrl.h"
// CRtuAttrHydrWorkCondition 对话框

class CRtuAttrHydrWorkCondition : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrHydrWorkCondition)

public:
	CRtuAttrHydrWorkCondition(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRtuAttrHydrWorkCondition();

// 对话框数据
	enum { IDD = IDD_RTU_QUERY_WORKCONDITION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

protected:

	CButtonXP   m_btn_QuerySensorVar;//查询传感器水位读数按钮
	CButtonXP   m_btn_QueryMemoryMargin;//查询存储余量按钮
	CButtonXP   m_btn_QueryEvoirTemperature;//查询环境温度按钮
	CButtonXP   m_btn_QuerySolarCellVoltage;//查询太阳能电池电压按钮
	CButtonXP   m_btn_QueryStorageVoltage;//蓄电池电压
	CButtonXP   m_btn_QueryStoragePtr;//查询数据存储指针
	CButtonXP   m_btn_RainCnt;//查询雨量计数器累计值
	CButtonXP   m_btn_SetupStatus;//查询传感器安装状态
	CComboBoxXP m_cbx_QuerySensorNO;//传感器号
	CEdit       m_let_SensorVar;
	CEdit       m_let_MemoryMargin;
	CEdit       m_let_EvoirTemperature;
	CEdit       m_let_SolarCellVoltage;
	CEdit       m_let_StorageVoltage;
	CEdit       m_let_RainCnt;
	CEdit       m_let_SetupStatus;
	CEdit       m_let_QueryStoragePtr;

	CString     m_str_SensorVar;
	CString     m_str_MemoryMargin;
	CString     m_str_EvoirTemperature;
	CString     m_str_SolarCellVoltage;
	CString     m_str_StorageVoltage;
	CString     m_str_RainCnt;
	CString     m_str_SetupStatus;
	CString     m_str_QueryStoragePtr;


	

	CRtuPropertyDlg* m_pParentDlg;				//TabCtrl父窗口
	CRtuStation*     m_pSubStation;				//当前的RtuStation对象
	BOOL      m_blResult;                       //召/设置参数成功与否标志

	void  SetWndsEnable(BOOL blEnable = TRUE);  //设置按钮是否可用
public:
	virtual BOOL OnInitDialog();
	void SetParentDlg(CRtuPropertyDlg* pDlg);
	void Update();                           //更新界面的显示信息
	void Update(int ret);
	void UpdateDlg(int attrType,BOOL blResult);  //接收到报文后、更新界面
	void KillAllTimers();

public:
	afx_msg void OnBnClickedBtnSensorvar();
	afx_msg void OnBnClickedBtnMemory();
	afx_msg void OnBnClickedBtnTemperation();
	afx_msg void OnBnClickedBtnSolarv();
	afx_msg void OnBnClickedBtnStorage();
	afx_msg void OnSelSensorNoChange();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	afx_msg void OnBnClickedBtnMemoryptr();
public:
	afx_msg void OnBnClickedBtnRaincnt();
public:
	afx_msg void OnBnClickedBtnSetupstatus();
};
