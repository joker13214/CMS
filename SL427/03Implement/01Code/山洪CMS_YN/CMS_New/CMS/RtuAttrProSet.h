#pragma once
#include "afxcmn.h"
#include "XTabCtrl.h"
#include "RtuAttrWorkMode.h"
#include "RtuAttrAutoRpSet.h"
#include "RtuAttrAlertAnalog.h"
#include "RtuAttrWaterQuality.h"
#include "RtuAttrAlarmState.h"
#include "RtuAttrWaterValume.h"
#include "RtuAttrWaterLevel.h"
#include "RtuAttrWaterPress.h"
#include "RtuAttrFlow.h"
#include "RtuAttrRainValume.h"
#include "RtuAttrQueryEvent.h"
#include "RtuAttrDayHisQuery.h"
#include "RtuAttrPulse.h"

// CRtuAttrProSet 对话框
class CRtuStation;
class CRtuPropertyDlg;
class CRtuAttrProSet : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrProSet)

public:
	CRtuAttrProSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRtuAttrProSet();

// 对话框数据
	enum { IDD = IDD_RTU_PROSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
protected:
	CRtuStation*		m_pSubStation;
	CRtuPropertyDlg*	m_pParentDlg;

	CRtuAttrWorkMode*  m_pTabAttrWorkMode;				                        //遥信终端的工作模式设置窗口
	CRtuAttrAutoRpSet* m_pTabAttrAutoRpSet;				                        //自报信息展示/设置窗口
	CRtuAttrAlertAnalog* m_pTabAttrAlertAnalog;                                 //模拟量
	CRtuAttrWaterQuality* m_pTabAttrWaterQuality;								//水质参数
	CRtuAttrAlarmState* m_pTabAttrAlarmState;									//遥测终端的报警状态和终端状态

	CRtuAttrWaterValume* m_pTabAttrWaterValume;									//充值量和水量
	CRtuAttrWaterLevel*  m_pTabAttrWaterLevel;									//水位
	CRtuAttrWaterPress*  m_pTabAttrWaterPress;									//水压
	CRtuAttrFlow*		 m_pTabAttrFlow;										//流量
	CRtuAttrRainValume*  m_pTabAttrRainValume;									//雨量
	CRtuAttrPulse*		 m_pTabAttrPulse;										//电度

	CRtuAttrQueryEvent* m_pTabAttrQueryEvent;                                   //事件记录 查询
	CRtuAttrDayHisQuery* m_pTabAttrDayHisQuery;									//历史日记录查询
	
	CXTabCtrl m_wndProTabCtrl;

public:
	void SetRtuStation(CRtuStation* pSub);
	void SetParentDlg(CRtuPropertyDlg* pParentDlg){m_pParentDlg = pParentDlg;}
	virtual BOOL OnInitDialog();
	CRtuAttrWorkMode*		GetTab_AttrWorkMode()			{return m_pTabAttrWorkMode;}
	CRtuAttrAutoRpSet*		GetTab_AttrAutoRpSet()			{return m_pTabAttrAutoRpSet;}
	CRtuAttrAlertAnalog*	GetTab_AttrAlertAnalog()		{return m_pTabAttrAlertAnalog;}
	CRtuAttrWaterQuality*   GetTab_AttrWaterQuality()		{return m_pTabAttrWaterQuality;}
	CRtuAttrAlarmState*     GetTab_AttrAlarmState()			{return m_pTabAttrAlarmState;}

	CRtuAttrWaterValume*    GetTab_AttrWaterValume()		{return m_pTabAttrWaterValume;}
	CRtuAttrWaterLevel*     GetTab_AttrWaterLevel()			{return m_pTabAttrWaterLevel;}
	CRtuAttrWaterPress*     GetTab_AttrWaterPress()			{return m_pTabAttrWaterPress;}
	CRtuAttrFlow*			GetTab_AttrFlow()				{return m_pTabAttrFlow;}
	CRtuAttrRainValume*     GetTab_AttrRainValume()			{return m_pTabAttrRainValume;}
	CRtuAttrQueryEvent*     GetTab_AttrQueryEvent()			{return m_pTabAttrQueryEvent;}
	CRtuAttrDayHisQuery*	GetTab_AttrDayHisQuery()		{return m_pTabAttrDayHisQuery;}
	CRtuAttrPulse*			GetTab_AttrPulse()				{return m_pTabAttrPulse;}

	void Update();

public:
	afx_msg void OnDestroy();
};
