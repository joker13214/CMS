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

// CRtuAttrProSet �Ի���
class CRtuStation;
class CRtuPropertyDlg;
class CRtuAttrProSet : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrProSet)

public:
	CRtuAttrProSet(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRtuAttrProSet();

// �Ի�������
	enum { IDD = IDD_RTU_PROSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
protected:
	CRtuStation*		m_pSubStation;
	CRtuPropertyDlg*	m_pParentDlg;

	CRtuAttrWorkMode*  m_pTabAttrWorkMode;				                        //ң���ն˵Ĺ���ģʽ���ô���
	CRtuAttrAutoRpSet* m_pTabAttrAutoRpSet;				                        //�Ա���Ϣչʾ/���ô���
	CRtuAttrAlertAnalog* m_pTabAttrAlertAnalog;                                 //ģ����
	CRtuAttrWaterQuality* m_pTabAttrWaterQuality;								//ˮ�ʲ���
	CRtuAttrAlarmState* m_pTabAttrAlarmState;									//ң���ն˵ı���״̬���ն�״̬

	CRtuAttrWaterValume* m_pTabAttrWaterValume;									//��ֵ����ˮ��
	CRtuAttrWaterLevel*  m_pTabAttrWaterLevel;									//ˮλ
	CRtuAttrWaterPress*  m_pTabAttrWaterPress;									//ˮѹ
	CRtuAttrFlow*		 m_pTabAttrFlow;										//����
	CRtuAttrRainValume*  m_pTabAttrRainValume;									//����
	CRtuAttrPulse*		 m_pTabAttrPulse;										//���

	CRtuAttrQueryEvent* m_pTabAttrQueryEvent;                                   //�¼���¼ ��ѯ
	CRtuAttrDayHisQuery* m_pTabAttrDayHisQuery;									//��ʷ�ռ�¼��ѯ
	
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
