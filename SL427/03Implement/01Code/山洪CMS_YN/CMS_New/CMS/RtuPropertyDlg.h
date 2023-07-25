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

// CRtuPropertyDlg �Ի���

class CRtuPropertyDlg : public CDialog
{
	DECLARE_DYNAMIC(CRtuPropertyDlg)

public:
	CRtuPropertyDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRtuPropertyDlg();

// �Ի�������
	enum { IDD = IDD_DLG_RTUPRO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CXTabCtrl m_wndTabCtrl;
protected:
	//Tabҳ����
	CRtuAttrBasicInfo* m_pTabAttrBasicInfo;				                        //ң���ն˻�����Ϣ����
	CRtuAttrDataShow*  m_pTabAttrDataShow;										//������Ϣ��ʾ����
	CRtuAttrProSet*	   m_pTabAttrProSet;										//ң���ն˲�������Tabҳ����
	CRtuAttrControlCmd* m_pTabAttrControlCmd;									//ң���ն˿�������Tabҳ����
	CRtuAttrComms*     m_pTabAttrComms;											//ͨ��״̬�ı���ˮ��¼��100����
	CRtuAttrHisQuery*  m_pTabAttrHisQuery;										//������ʷ���ݲ�ѯ
	CRtuAttrHydrProSet* m_pTabHydrProSet;                                        //ˮ�ļ�¼���������ҳ����Ӧ�ڹ�ԼSCSW008
	CRtuAttrSyqProSet * m_pTabSyqProSet;                                        //ˮ����ң��ϵͳ����ͨ�Ź�Լ
	CRtuAttrWriteUpdate* m_pTabWriteUpdate;                                      //��д����

	CUdfListCtrl	   m_wndListCtrl;	         		                        //��־��Ϣ����
	CRtuStation*	   m_pCurRtuStation;										//��ǰ������RTUStation����
	int				   m_nlogRowsMax;											//��ʾ��־��Ϣ�����������Ĭ��Ϊ100
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
	void		 SetRtuStation(CRtuStation* pRtu);								//���õ�ǰ������RTUStation����
	CRtuStation* GetRtuStation(){return m_pCurRtuStation;}						//��ȡ��ǰ������RTUStation����
	void		 Show(int nCmdShow,CWnd* pParent =  NULL);                      //������ʾ/����
	void		 Update();														//���½�����ʾ����Ϣ
	void         UpdateCaption();												//���´��ڱ�����

	//����Rtu�����־��Ϣ
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
	afx_msg void OnSaveAsLogList();   //������־��Ϣ�������ļ�
	afx_msg void OnNMDblclkRtuLoglist(NMHDR *pNMHDR, LRESULT *pResult);
};
