#pragma once
#include <afxmt.h>
#include <afxdb.h>
#include "UdfLogSystem.h"


enum DataTran_ThreadType
{
	DataTran_BaseClass		=   0,
	DataTran_SQLiteLocal	=   1,
	DataTran_SQLServer_GM	=	2,
	DataTran_SQLServer_SC	=   3,
};

class COneData_INFO;
class CUdfRtuDataSave_Server;


//数据转发基类,抽象基类
class CDataTran_Base
{
public:
	CDataTran_Base();
	virtual ~CDataTran_Base();
public:
	virtual long ThreadFunction() = 0;                 //线程处理函数
	virtual void SetDataSource(CString strSource) = 0;
public:
	void AddOneData(COneData_INFO* pData);	
	void AddThreadLog(CString logMessage,
		_Log_level_type xLevel = Normal_Log_Message,
		bool   bShow = true,
		PSYSTEMTIME pTime = NULL);					   //添加日志信息

public:
	HANDLE GetThreadHandle()					{return m_handle_Thread;}					//获取线程句柄(HANDLE)  
	const DWORD  GetThreadID()					{return m_dwThreadID;}						//获取线程的ID
	const BYTE GetThreadState()					{return m_bThreadState;}					//获取线程的状态
	const bool GetThreadControl()				{return m_bThreadControl;}					//获取运行停止控制标志，true:运行，false停止

	void SetThreadHandle(HANDLE newThreadHandle){m_handle_Thread = newThreadHandle;}		//设置线程句柄(HANDLE)
	void SetThreadID(DWORD newThreadID)			{m_dwThreadID = newThreadID;}				//设置线程的ID
	void SetThreadState(BYTE threadState)		{m_bThreadState = threadState;}				//设置线程的状态
	void SetThreadControl(bool nValue)			{m_bThreadControl = nValue;}				//设置运行停止控制标志，true:运行，false停止
	void ResetThread();
	void SetParent(CUdfRtuDataSave_Server* pParent);
	CUdfRtuDataSave_Server* GetParent();

	DataTran_ThreadType GetDataTran_ThreadType(){return m_ThreadType;}
	CList<Log_Item*>* GetLogListPointer()		{return &m_lstLogs;}
protected:
	CCriticalSection		m_cs_DataLst;		
	CList<COneData_INFO*>	m_lstData;
	bool                    m_bNeedData;				//是否需要原始采集数据

	CCriticalSection		m_cs_LogsLst;
	CList<Log_Item*>		m_lstLogs;

	int						m_nMaxData;					//内存中最大数据保存，默认:5000;
	int						m_nMaxLogs;					//内存中最大日志保存，默认:200;

	HANDLE	m_handle_Thread;																//线程句柄
	DWORD	m_dwThreadID;																	//线程ID
	BYTE	m_bThreadState;																	//线程状态
	bool	m_bThreadControl;																//线程控制; true 启动  false 停止线程

	CString  m_strLastLogs;																	//last log message
	//CString	 m_strDataSource;															//数据源		

	DataTran_ThreadType     m_ThreadType;
	CUdfRtuDataSave_Server* m_pParent;
protected:
	COneData_INFO* GetOneData();
	void Clear();
	void ClearDataList();
	void ClearLogsList();
	void AddThreadLog(Log_Item* pLog);
	const int GetDataCount();		//数据计数
protected:
	//添加日志信息到日志系统,由日志系统的保存
	void AddLog_To_LogSystem(CString logMessgae,
		                    _Log_level_type		xLevel = Normal_Log_Message,
							PSYSTEMTIME			pTime = NULL,
							_LOG_MESSAGE_TYPE	LogType = DATAIN_ERROR_LOG);

	//添加日志信息到日志窗口显示
	void AddLog_To_LogFrame(CString LogMessage,
							_Log_level_type		xLevel = Normal_Log_Message,
							PSYSTEMTIME			pTime = NULL);
protected:
};

//SQLServer base class
class CDataTranSQLServer_Base
{
public:
	CDataTranSQLServer_Base();
	~CDataTranSQLServer_Base();
protected:
	CString m_strDataSource;
	CDatabase m_Database;
protected:
	bool CloseDatabase();
	CDatabase* GetDatabase();
};