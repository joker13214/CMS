#pragma once
#include <afxdb.h>
#include "UdfDataInDbLogDlg.h"
#include "RtuStation.h"
#include "WaitTimer.h"

#include "DataTran_Base.h"
#include "DataTranSQLite_Local.h"


//单条需要存储数据信息
class COneData_INFO 
{
public:
	enum _Data_Type
	{
		OPERATE_RECORD	= 0x0001,					//操作日志
		WATER_L_DATA	= 0x0002,					//水位采集数据
		WATER_P_DATA	= 0x0003,					//水压采集数据
		FLOW_V_DATA		= 0x0004,					//流量采集数据
		WATER_Q_DATA	= 0x0005,					//水质采集数据
		RAIN_DATA		= 0x0006,					//雨量采集数据
		ANALOG_DATA		= 0x0007,					//模拟量采集数据

		ALARM_RECORD    = 0x0008,					//报警记录
		PULSE_DATA      = 0x0009,					//电度量采集数据
		PEACE_REPORT	= 0x000A,					//平安报信息
		DISPLACE_DATA	= 0x000B,					//深度位移量采集数据
		TEMPERA_DATA	= 0x000C					//温度量采集数据
	};
	enum _Alarm_Type
	{
		ALARM_WATERLEVEL   =  2,                    //水位超限报警
		ALARM_WATERPRESS   =  3,					//水压超限报警
		ALARM_WATERQUALITY =  4,					//水质超限报警
		ALARM_FLOW		   =  7,					//剩余水量下限报警
		ALARM_RAIN		   = 16,					//雨量超限报警
		ALARM_ANALOG	   = 17,					//模拟量超限报警
		ALARM_DISPLACE	   = 18,					//深度位移量超限报警
		ALARM_TEMPERA	   = 19						//温度量超限报警
	};

	BYTE	mProvinceCode;								//省码 0 - 99
	BYTE	mRegionCode;								//区码 0 - 99
	BYTE	mCountyCode;								//县码 0 - 99
	DWORD	mA2;										//遥测终端地址
	//int		mRtuNo										//遥测终端传感器序号
	_Data_Type mDataType;								//数据类型
	int     mAlarmType;									//事件报警类型
	CString mOperateDESC;								//操作描述
	CString mOperateAccount;							//操作帐号

	SYSTEMTIME mCollectTime;							//采集时间，报警时间
	SYSTEMTIME mOperateTime;							//操作时间
	SYSTEMTIME mRecvTime;								//接收到报文时间
	_TP		   mTP;										//TP信息，当报文含有TP信息时又用

	CList<CWaterLevel_PAR_PER*> m_lstWaterLevel;		//N个水位数据
	CList<CWaterPressure_PARA_PER*> m_lstWaterPress;	//N个水压数据
	CList<CFlowInitValume_PARA_PER*> m_lstFlow;			//N个流量数据
	CList<CWaterQuality_PARA_PER*> m_lstWaterQuality;	//N个水质数据
	CList<CRainAlert_PARA_PER*>	m_lstRain;				//N个雨量数据
	CList<CAnalogLimit_PARA_PER*> m_lstAnalog;			//N个模拟量数据
	CList<CDisplace_PARA_PER*> m_lstDisplace;		//N个深度位移量数据
	CList<CTempera_PARA_PER*> m_lstTempera;		//N个温度量数据
	CList<CPulsePara_PER*>		  m_lstPulses;			//N个电度量数据

	CString   sPeaceReport;								//平安报信息

private:
	bool m_blErrData;									//是否为异常数据 false：不是异常数据（默认） ture：异常数据 add 20140812
public:
	COneData_INFO();
	COneData_INFO(COneData_INFO* pSourceData);			//拷贝
	~COneData_INFO();
public:
	//设置异常数据标志 add 20140812
	void SetErrorFlag(bool errFlag){m_blErrData  = errFlag;}  
	//返回异常数据标志 add 20140812
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
	CString m_RtuID;								   //ID,存库需要
	CString m_CollectTimeString;                       //采集时间
	CString m_RecvTimeString;						   //接收报文时间
	CString m_OperateTimeString;					   //操作时间
	CString m_Alarm_PID;                               //报警ID
public:
	const CString Get_RTU_ID();                        //存库时所需RTU的ID
	const CString Get_CollectTime_String();			   //采集时间字符串
	const CString Get_RecvTime_String();			   //接收报文时间字符串
	const CString Get_OperateTime_String();			   //操作时间字符串
	const CString Get_Alarm_PID();					   //报警生产主键需要
};


//Rtu相关数据信息实时入库、转发、转存的Server
//管理各类数据接口的线程
class CUdfRtuDataSave_Server
{
public:
	CUdfRtuDataSave_Server(void);
	CUdfRtuDataSave_Server(CString strDataSource);
public:
	virtual ~CUdfRtuDataSave_Server(void);
protected:
	//CDatabase m_Database;																						//Connect System Database  (SQL Server 2000/2005/2008)
	CString   m_strDatasource;																					//数据源连接字符串 ODBC 
public:
	void SetDataSource(CString _dataSource);																	//设置数据源
	void SetDataSource(CString _Dsn,CString _Uid,CString _Pwd);													//设置数据源
	bool Initialization();																						//初始化RTU数据实时入库模块，包括启动实时存储数据线程
protected:
	static unsigned long __stdcall ThreadFunction(void* pPARAM);												//数据存储线程函数
	void   StartThread();																						//启动数据存储线程

	CUdfDataInDbLogDlg* m_pWndLogdlg;																			//实时显示RTU实时入库数据的信息窗口	
	
	//2012-06-29 add
	CDataTran_Base*         m_pDataTranObject;				  //SC统计数据										//
public:
	CDataTranSQLite_Local*  m_pDataTranSQLiteLocal;	          //本地SQLite数据库
public:
	void      StatisticsData(CDbSQLite* sqlite,COneData_INFO* pData=NULL);										//2012-06-29 added 由local数据存储线程触发在存储完SQLite后调用数据统计入省标（四川省标库入库统计）
	void      LocalOnTimerFunction(bool blLocalSQLiteTransStart);												//本地SQLiteOnTimer的函数（四川省标库入库统计）
	void      CurStatisticsState(int nCount,SYSTEMTIME& collTime,SYSTEMTIME& recvTime);							//监测当前是否有挤压报文（四川省标库入库统计）
public:
	void       SetDataInDbLogWnd(CUdfDataInDbLogDlg* pDlg) {m_pWndLogdlg = pDlg;}								//设置数据实时入库日志窗口的指针
	CUdfDataInDbLogDlg* GetDataInDbLogWnd()				   {return m_pWndLogdlg;}								//获取数据实时入库显示日志窗口的指针
public:
	void       InsertOneData(COneData_INFO* pData);   //插入一条需要存储的数据

public:
	//向窗口添加日志信息
	void     AddLog_To_LogFrame(CString message,
								_Log_level_type xType = Normal_Log_Message,
								PSYSTEMTIME st=NULL); 

	//添加一条日志信息（存储到日志文件里头）
	void     AddLog_To_LogSystem(_LOG_MESSAGE_TYPE LogType,
								 CString LogMsg,
								 _Log_level_type LogLevel = Normal_Log_Message,
								 PSYSTEMTIME pTIME = NULL);
protected:
	CString m_str_LastMessage_LogSys;						   //最后一条日志信息
	CString m_str_LastMessage_LogDlg;						   //最后一条日志信息

	CCriticalSection m_cs_logframe;
	void Clear();
public:
	CList<CDataTran_Base*>	m_lstDataTranThreads;			   //线程链表
	CCriticalSection		m_cs_DataTranThreads;	

	bool CreateDataTranThread(CDataTran_Base* pThreadModule);
	bool AddDataTranThread(CDataTran_Base* pThreadModule);
	bool ResumeDataTranThread(CDataTran_Base* pThreadModule);
	bool StartDataTranThread(CDataTran_Base* pThreadModule);
	bool StopDataTranThread(CDataTran_Base* pThreadModule);
	bool DeleteDataTranThread(CDataTran_Base* pThreadModule);
};