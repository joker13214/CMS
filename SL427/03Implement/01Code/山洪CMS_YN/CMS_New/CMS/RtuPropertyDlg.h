#pragma once
#include "afxcmn.h"
#include "XTabCtrl.h"
#include "RtuAttrBasicInfo.h"
#include "RtuAttrWorkMode.h"
#include "RtuAttrAutoRpSet.h"
#include "RtuAttrDataShow.h"
#include "RtuAttrProSet.h"
#include "RtuAttrAlertAnalog.h"
#include "RtuAttrWaterQuality.h"
#include "RtuAttrAlarmState.h"
#include "RtuAttrControlCmd.h"
#include "RtuAttrComms.h"
#include "RtuAttrHisQuery.h"
#include "RtuAttrHydrProSet.h"
#include "RtuAttrWriteUpdate.h"
#include "RtuAttrSyqProSet.h"
#include "RtuStation.h"
#include "UdfListCtrl.h"

// CRtuPropertyDlg 对话框

class CRtuPropertyDlg : public CDialog
{
	DECLARE_DYNAMIC(CRtuPropertyDlg)

public:
	CRtuPropertyDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRtuPropertyDlg();

// 对话框数据
	enum { IDD = IDD_DLG_RTUPRO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CXTabCtrl m_wndTabCtrl;
protected:
	//Tab页窗口
	CRtuAttrBasicInfo* m_pTabAttrBasicInfo;				                        //遥测终端基本信息窗口
	CRtuAttrDataShow*  m_pTabAttrDataShow;										//数据信息显示窗口
	CRtuAttrProSet*	   m_pTabAttrProSet;										//遥测终端参数设置Tab页窗口
	CRtuAttrControlCmd* m_pTabAttrControlCmd;									//遥测终端控制命令Tab页窗口
	CRtuAttrComms*     m_pTabAttrComms;											//通信状态改变流水记录（100条）
	CRtuAttrHisQuery*  m_pTabAttrHisQuery;										//本地历史数据查询
	CRtuAttrHydrProSet* m_pTabHydrProSet;                                        //水文记录的相关属性页，适应于规约SCSW008
	CRtuAttrSyqProSet * m_pTabSyqProSet;                                        //水雨情遥测系统数据通信规约
	CRtuAttrWriteUpdate* m_pTabWriteUpdate;                                      //烧写升级

	CUdfListCtrl	   m_wndListCtrl;	         		                        //日志信息窗口
	CRtuStation*	   m_pCurRtuStation;										//当前操作的RTUStation对象
	int				   m_nlogRowsMax;											//显示日志信息的最大行数，默认为100
public:
	CRtuAttrSyqProSet*     GetRtuAttrSyqProSet()   {return m_pTabSyqProSet;}
	CRtuAttrHydrProSet*     GetRtuAttrHydrrProSet()   {return m_pTabHydrProSet;}
	CRtuAttrBasicInfo*		GetTab_AttrBasicInfo()	{return m_pTabAttrBasicInfo;}
	CRtuAttrDataShow*		GetTab_AttrDataShow()	{return m_pTabAttrDataShow;}
	CRtuAttrControlCmd*     GetTab_AttrControlCmd()	{return m_pTabAttrControlCmd;}
	CRtuAttrComms*			GetTab_AttrComms()		{return m_pTabAttrComms;}
	CRtuAttrHisQuery*		GetTab_AttrHisQuery()	{return m_pTabAttrHisQuery;}
	CRtuAttrWriteUpdate*    GetTab_AttrWriteUpdate(){return m_pTabWriteUpdate;}
	CRtuAttrWorkMode*		GetTab_AttrWorkMode();	
	CRtuAttrAutoRpSet*		GetTab_AttrAutoRpSet();	
	CRtuAttrAlertAnalog*    GetTab_AttrAlertAnalog();
	CRtuAttrWaterQuality*   GetTab_AttrWaterQuality();
	CRtuAttrAlarmState*     GetTab_AttrAlarmState();
	CRtuAttrRainValume*     GetTab_AttrRainValume();
	CRtuAttrWaterValume*    GetTab_AttrWaterValume();
	CRtuAttrWaterLevel*     GetTab_AttrWaterLevel();
	CRtuAttrWaterPress*     GetTab_AttrWaterPress();
	CRtuAttrFlow*			GetTab_AttrFlow();
	CRtuAttrQueryEvent*     GetTab_AttrQueryEvent();
	CRtuAttrDayHisQuery*	GetTab_AttrDayHisQuery();
	CRtuAttrPulse*			GetTab_AttrPulse();

public:
	virtual BOOL OnInitDialog();
	void         InitLogListCtrl();                 
	void		 SetRtuStation(CRtuStation* pRtu);								//设置当前操作的RTUStation对象
	CRtuStation* GetRtuStation(){return m_pCurRtuStation;}						//获取当前操作的RTUStation对象
	void		 Show(int nCmdShow,CWnd* pParent =  NULL);                      //窗口显示/隐藏
	void		 Update();														//更新界面显示的信息
	void         UpdateCaption();												//更新窗口标题栏

	//插入Rtu相关日志信息
	void         AddLog(CString strLogMessage,									
		                PSYSTEMTIME logTime = NULL,
						_Log_level_type logLevel = Normal_Log_Message);
	void         AddLog(CString strLogMessage,CString strLogTime,_Log_level_type LogLevel);
public:
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNMRclickRtuLoglist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClearLogList();
	afx_msg void OnSaveAsLogList();   //保存日志信息到其他文件
	afx_msg void OnNMDblclkRtuLoglist(NMHDR *pNMHDR, LRESULT *pResult);
};
