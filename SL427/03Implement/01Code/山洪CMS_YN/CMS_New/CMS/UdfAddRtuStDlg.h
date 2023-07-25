#pragma once
#include "afxwin.h"
#include "UdfGlobalFUNC.h"
#include "RtuStation.h"
#include "UdfGlobalFUNC.h"
#include "GridCtrl/GridCtrl.h"


// CUdfAddRtuStDlg 对话框

class CUdfAddRtuStDlg : public CDialog
{
	DECLARE_DYNAMIC(CUdfAddRtuStDlg)

public:
	CUdfAddRtuStDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUdfAddRtuStDlg();

// 对话框数据
	enum { IDD = IDD_DLG_ADDST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBoxXP m_wndCmbProvince;
	CComboBoxXP m_wndCmbRegion;
	CComboBoxXP m_wndCmbCounty;
	CComboBoxXP m_wndCmbA3;
	CComboBoxXP m_wndCmbCmType;
	CComboBoxXP m_wndCmbInterface;
	CComboBoxXP m_wndCmbAppProVer;
	CComboBoxXP m_wndCmbAppProType;//应用层协议类型
	CButtonXP m_wndBtnInterfaceCfg;

	CStatic m_wndGridParent;
	CStatic m_wndStaticScFactory;
	CStatic m_wndStaticSCSW008;
	CStatic m_wndStaticSYQCENTER;
	CStatic m_wndStaticSYQADDR;
	CEdit   m_wndLetSCSW008;
	CEdit   m_wndLetA2;
	CEdit   m_wndLetSYQCenter;
	CEdit   m_wndLetSYQAddr;

	CComboBoxXP m_wndCmbScFactory;

	DWORD m_dwA2;
	int   m_syqcenter;
	DWORD m_strSYQADDR;
	CString m_strSCSW008Code;
	CString m_strStCode;
	CString m_sName;
	CString m_strCaption;   //窗口标题
	BYTE m_ProvinceCode; 
	BYTE m_RegionCode;
	BYTE m_CountyCode;
	BYTE m_A3Code;
	BYTE m_CmType;
	BYTE m_Interface;
	COM_INFO m_ComInfo;    //串口信息
	NET_INFO m_NetInfo;    //网络信息

    bool     m_bFirstCfg;						//第一次配置 
	DB_RECORD_OPERATION_TYPE m_OperateFlag;     //操作标志
	CRtuStation* m_pCurRtuStation;				//当前修改的rtu站对象

	//Add 2012-04-24
	BYTE m_nAppProVer;
	int  m_nScFactoryType;
	UINT32 m_nScAddress;
	BYTE m_nAppProtoType;
public:
	afx_msg void OnCbnSelchangeCmbRrovince();
	afx_msg void OnCbnSelchangeCmbRegion();
	afx_msg void OnBnClickedBtnInterfacecfg();
	afx_msg void OnCbnSelchangeCmbInterface();
	afx_msg void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	void    Update(CRtuStation* pStaion);
	
public:
	CButtonXP m_btnOK;
	CButtonXP m_btnCancel;
	CGridCtrl   m_wndCmOptGridCtrl;

protected:
	void  InitGridCtrl();
	void  InitGridCtrl_ComInfo();		//串口信息设置
	void  InitGridCtrl_NetInfo();		//网卡信息设计
	void  OnCellNormal(int Row,int Col);
	void  OnCellReadOnly(int Row,int Col);

	void  InitComGridItem11();
	void  InitComGridItem21();
	void  InitComGridItem31();
	void  InitComGridItem41();
	void  InitComGridItem51();
	void  InitComGridItem61();
	const CString GetCellText(int Row,int Col);
public:

public:
	afx_msg void OnCbnSelchangeCmbCmtype();
	afx_msg void OnCbnSelchangeAppProType();
public:
	afx_msg void OnEnChangeEditAddvcode();
public:
	CEdit m_wndEditAddvCode;
protected:
	void  ShowAddvCode();
public:
	afx_msg void OnCbnSelchangeCmbCounty();
public:
	afx_msg void OnBnClickedOk();
};
