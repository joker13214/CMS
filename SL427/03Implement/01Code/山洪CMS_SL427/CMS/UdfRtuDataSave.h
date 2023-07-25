#pragma once
#include <afxdb.h>
#include "UdfDataInDbLogDlg.h"
#include "RtuStation.h"
#include "WaitTimer.h"

#include "DataTran_Base.h"
#include "DataTranSQLite_Local.h"


//������Ҫ�洢������Ϣ
class COneData_INFO 
{
public:
	enum _Data_Type
	{
		OPERATE_RECORD	= 0x0001,					//������־
		WATER_L_DATA	= 0x0002,					//ˮλ�ɼ�����
		WATER_P_DATA	= 0x0003,					//ˮѹ�ɼ�����
		FLOW_V_DATA		= 0x0004,					//�����ɼ�����
		WATER_Q_DATA	= 0x0005,					//ˮ�ʲɼ�����
		RAIN_DATA		= 0x0006,					//�����ɼ�����
		ANALOG_DATA		= 0x0007,					//ģ�����ɼ�����

		ALARM_RECORD    = 0x0008,					//������¼
		PULSE_DATA      = 0x0009,					//������ɼ�����
		PEACE_REPORT	= 0x000A,					//ƽ������Ϣ
		DISPLACE_DATA	= 0x000B,					//���λ�����ɼ�����
		TEMPERA_DATA	= 0x000C					//�¶����ɼ�����
	};
	enum _Alarm_Type
	{
		ALARM_WATERLEVEL   =  2,                    //ˮλ���ޱ���
		ALARM_WATERPRESS   =  3,					//ˮѹ���ޱ���
		ALARM_WATERQUALITY =  4,					//ˮ�ʳ��ޱ���
		ALARM_FLOW		   =  7,					//ʣ��ˮ�����ޱ���
		ALARM_RAIN		   = 16,					//�������ޱ���
		ALARM_ANALOG	   = 17,					//ģ�������ޱ���
		ALARM_DISPLACE	   = 18,					//���λ�������ޱ���
		ALARM_TEMPERA	   = 19						//�¶������ޱ���
	};

	BYTE	mProvinceCode;								//ʡ�� 0 - 99
	BYTE	mRegionCode;								//���� 0 - 99
	BYTE	mCountyCode;								//���� 0 - 99
	DWORD	mA2;										//ң���ն˵�ַ
	//int		mRtuNo										//ң���ն˴��������
	_Data_Type mDataType;								//��������
	int     mAlarmType;									//�¼���������
	CString mOperateDESC;								//��������
	CString mOperateAccount;							//�����ʺ�

	SYSTEMTIME mCollectTime;							//�ɼ�ʱ�䣬����ʱ��
	SYSTEMTIME mOperateTime;							//����ʱ��
	SYSTEMTIME mRecvTime;								//���յ�����ʱ��
	_TP		   mTP;										//TP��Ϣ�������ĺ���TP��Ϣʱ����

	CList<CWaterLevel_PAR_PER*> m_lstWaterLevel;		//N��ˮλ����
	CList<CWaterPressure_PARA_PER*> m_lstWaterPress;	//N��ˮѹ����
	CList<CFlowInitValume_PARA_PER*> m_lstFlow;			//N����������
	CList<CWaterQuality_PARA_PER*> m_lstWaterQuality;	//N��ˮ������
	CList<CRainAlert_PARA_PER*>	m_lstRain;				//N����������
	CList<CAnalogLimit_PARA_PER*> m_lstAnalog;			//N��ģ��������
	CList<CDisplace_PARA_PER*> m_lstDisplace;		//N�����λ��������
	CList<CTempera_PARA_PER*> m_lstTempera;		//N���¶�������
	CList<CPulsePara_PER*>		  m_lstPulses;			//N�����������

	CString   sPeaceReport;								//ƽ������Ϣ

private:
	bool m_blErrData;									//�Ƿ�Ϊ�쳣���� false�������쳣���ݣ�Ĭ�ϣ� ture���쳣���� add 20140812
public:
	COneData_INFO();
	COneData_INFO(COneData_INFO* pSourceData);			//����
	~COneData_INFO();
public:
	//�����쳣���ݱ�־ add 20140812
	void SetErrorFlag(bool errFlag){m_blErrData  = errFlag;}  
	//�����쳣���ݱ�־ add 20140812
	const bool GetErrorFlag(){return m_blErrData;}
protected:
	void Clear();
	void ClearLevelList();
	void ClearPressList();
	void ClearFlowList();
	void ClearQualityList();
	void ClearRainList();
	void ClearAnalogList();
	void ClearPulseList();
	void ClearDisplaceList();
	void ClearTemperaList();

	//copy
	void CopyLevelList(COneData_INFO* pSourceData);							   
	void CopyPressList(COneData_INFO* pSourceData);
	void CopyFlowList(COneData_INFO* pSourceData);
	void CopyQualityList(COneData_INFO* pSourceData);
	void CopyRainList(COneData_INFO* pSourceData);
	void CopyAnalogList(COneData_INFO* pSourceData);
	void CopyPulseList(COneData_INFO* pSourceData);
	void CopyDisplaceList(COneData_INFO* pSourceData);
	void CopyTemperaList(COneData_INFO* pSourceData);
protected:
	CString m_RtuID;								   //ID,�����Ҫ
	CString m_CollectTimeString;                       //�ɼ�ʱ��
	CString m_RecvTimeString;						   //���ձ���ʱ��
	CString m_OperateTimeString;					   //����ʱ��
	CString m_Alarm_PID;                               //����ID
public:
	const CString Get_RTU_ID();                        //���ʱ����RTU��ID
	const CString Get_CollectTime_String();			   //�ɼ�ʱ���ַ���
	const CString Get_RecvTime_String();			   //���ձ���ʱ���ַ���
	const CString Get_OperateTime_String();			   //����ʱ���ַ���
	const CString Get_Alarm_PID();					   //��������������Ҫ
};


//Rtu���������Ϣʵʱ��⡢ת����ת���Server
//����������ݽӿڵ��߳�
class CUdfRtuDataSave_Server
{
public:
	CUdfRtuDataSave_Server(void);
	CUdfRtuDataSave_Server(CString strDataSource);
public:
	virtual ~CUdfRtuDataSave_Server(void);
protected:
	//CDatabase m_Database;																						//Connect System Database  (SQL Server 2000/2005/2008)
	CString   m_strDatasource;																					//����Դ�����ַ��� ODBC 
public:
	void SetDataSource(CString _dataSource);																	//��������Դ
	void SetDataSource(CString _Dsn,CString _Uid,CString _Pwd);													//��������Դ
	bool Initialization();																						//��ʼ��RTU����ʵʱ���ģ�飬��������ʵʱ�洢�����߳�
protected:
	static unsigned long __stdcall ThreadFunction(void* pPARAM);												//���ݴ洢�̺߳���
	void   StartThread();																						//�������ݴ洢�߳�

	CUdfDataInDbLogDlg* m_pWndLogdlg;																			//ʵʱ��ʾRTUʵʱ������ݵ���Ϣ����	
	
	//2012-06-29 add
	CDataTran_Base*         m_pDataTranObject;				  //SCͳ������										//
public:
	CDataTranSQLite_Local*  m_pDataTranSQLiteLocal;	          //����SQLite���ݿ�
public:
	void      StatisticsData(CDbSQLite* sqlite,COneData_INFO* pData=NULL);										//2012-06-29 added ��local���ݴ洢�̴߳����ڴ洢��SQLite���������ͳ����ʡ�꣨�Ĵ�ʡ������ͳ�ƣ�
	void      LocalOnTimerFunction(bool blLocalSQLiteTransStart);												//����SQLiteOnTimer�ĺ������Ĵ�ʡ������ͳ�ƣ�
	void      CurStatisticsState(int nCount,SYSTEMTIME& collTime,SYSTEMTIME& recvTime);							//��⵱ǰ�Ƿ��м�ѹ���ģ��Ĵ�ʡ������ͳ�ƣ�
public:
	void       SetDataInDbLogWnd(CUdfDataInDbLogDlg* pDlg) {m_pWndLogdlg = pDlg;}								//��������ʵʱ�����־���ڵ�ָ��
	CUdfDataInDbLogDlg* GetDataInDbLogWnd()				   {return m_pWndLogdlg;}								//��ȡ����ʵʱ�����ʾ��־���ڵ�ָ��
public:
	void       InsertOneData(COneData_INFO* pData);   //����һ����Ҫ�洢������

public:
	//�򴰿������־��Ϣ
	void     AddLog_To_LogFrame(CString message,
								_Log_level_type xType = Normal_Log_Message,
								PSYSTEMTIME st=NULL); 

	//���һ����־��Ϣ���洢����־�ļ���ͷ��
	void     AddLog_To_LogSystem(_LOG_MESSAGE_TYPE LogType,
								 CString LogMsg,
								 _Log_level_type LogLevel = Normal_Log_Message,
								 PSYSTEMTIME pTIME = NULL);
protected:
	CString m_str_LastMessage_LogSys;						   //���һ����־��Ϣ
	CString m_str_LastMessage_LogDlg;						   //���һ����־��Ϣ

	CCriticalSection m_cs_logframe;
	void Clear();
public:
	CList<CDataTran_Base*>	m_lstDataTranThreads;			   //�߳�����
	CCriticalSection		m_cs_DataTranThreads;	

	bool CreateDataTranThread(CDataTran_Base* pThreadModule);
	bool AddDataTranThread(CDataTran_Base* pThreadModule);
	bool ResumeDataTranThread(CDataTran_Base* pThreadModule);
	bool StartDataTranThread(CDataTran_Base* pThreadModule);
	bool StopDataTranThread(CDataTran_Base* pThreadModule);
	bool DeleteDataTranThread(CDataTran_Base* pThreadModule);
};