#pragma once

#include <afxdao.h>
#include <afxdb.h>
#include <afxtempl.h>
#include <afxmt.h>
#include "UdfLogQueryDlg.h"

enum _LOG_MESSAGE_TYPE
{
	RTU_CM_STATUS_LOG = 1,  //RTU 通信 状态日志
	RTU_YX_CHANGE_LOG = 2,  //RTU 遥信 变位日志
	RTU_YC_CHANGE_LOG = 3,  //RTU 遥测 变位日志
    RTU_CM_BUFFER_LOG = 4,  //RTU 通信 报文日志
	RTU_YK_OPERAT_LOG = 5,  //RTU 遥控 操作日志
	RTU_ZD_OPERAT_LOG = 6,  //RTU 定值 操作日志
	SYS_RUNNING_S_LOG = 7,  //系统 运行时 日志
	SYS_OTHER_RUN_LOG = 8,  //其他日志信息    
	DATAIN_ERROR_LOG  = 9,  //数据实时入库错误日志
};




class CUdfLogMessage_Item
{
public:
	_LOG_MESSAGE_TYPE m_bLogType; //日志类型

	_Log_level_type   m_level_type; 

	//log信息
	SYSTEMTIME m_smtLogDateTime; //日期时间信息
	CString    m_strLogMessage;  //日志相信信息
	CString    m_strDatetime;
	CUdfLogMessage_Item()
	{
		GetLocalTime(&m_smtLogDateTime);
		m_bLogType = SYS_OTHER_RUN_LOG;
		m_strLogMessage = "";
		m_strDatetime = "";
	}
};

//通信管理系统-日志系统
class CUdfLogSystem
{
public:
	CUdfLogSystem(void);
	CUdfLogSystem(CString _sFile);
public:
	~CUdfLogSystem(void);
public:
	//初始化日子系统
	BOOL InitLogSystem(void);   

	//创建日志记录表
	BOOL CreateLogTables();

	//Connect Database
	BOOL ConnectLogDB();

	//定时删除日志信息
	BOOL DeleteLogOnTimer();


	//Insert 日志信息
	BOOL InsertLogMessage(CUdfLogMessage_Item* _plogItem);
	BOOL InsertLogMessage(_LOG_MESSAGE_TYPE LogType,CString LogMessage,_Log_level_type _levelType = Normal_Log_Message,SYSTEMTIME* pLogTime=NULL);


	BOOL GetLogInfoSaveFlag(_LOG_MESSAGE_TYPE _xType);   //获取日志信息的是/否存储的标志

	//get Log messages list
	CPtrList* GetLogList();
	CDatabase* GetDatabase();  //Get database pointer

	//日志处理线程
    static unsigned long __stdcall ThreadFunction(void *pData);
	void StartLogThread();								//启动线程
	bool m_blThreadRunFlag;								//线程 启动运行/停止 操作标志
	bool m_blThreadRuning;								//线程 运行/停止 状态

protected:
	//日志存储文件 注意:绝对路径 
	CString m_strLogFile;

	//链接日志数据库连接
	CDatabase m_DataBase;
	CDatabase m_QueryDataBase;
	//日系信息链表
	CPtrList  m_lstLogmessages;

	BOOL InsertLogMessageToLogDB(CUdfLogMessage_Item* _plogItem);

public:
	//互斥锁
	//CMutex m_LogMutex;   //log Lst Mutex
	//CMutex m_LogDBMutex; //Log db  Mutex

	CCriticalSection m_LogMutex;   //log Lst Mutex
	CCriticalSection m_LogDBMutex; //Log db  Mutex

public:
	//Search Log informations
	CPtrList* SearchLogInformation(_LOG_MESSAGE_TYPE _nType,CUdfLogQueryDlg::QUERY_LOG_TYPE queryLogType=CUdfLogQueryDlg::QUERY_LOGTYPE_ALL,PSYSTEMTIME _beginTime=NULL, PSYSTEMTIME _endTime = NULL);
    
	//查询日志信息
	int       QueryLogInformation(_LOG_MESSAGE_TYPE _nType,CUdfLogQueryDlg::QUERY_LOG_TYPE queryLogType=CUdfLogQueryDlg::QUERY_LOGTYPE_ALL,PSYSTEMTIME _beginTime=NULL, PSYSTEMTIME _endTime = NULL);

	//Save Log Search result
    CPtrList  m_lstSearchResult;

	//手动删除日志信息
	bool    DeleteLogByHand(_LOG_MESSAGE_TYPE _nType,PSYSTEMTIME _pBeginTime=NULL,PSYSTEMTIME pEndTime=NULL);
	bool    DeleteLogByHand(_LOG_MESSAGE_TYPE _nType,CString strBeginTime,CString strEndTime);

	//Get name
	static CString GetLogTableName(_LOG_MESSAGE_TYPE _nType);    //Get log database's table name
	static CString GetLogTypeName(_LOG_MESSAGE_TYPE _nType);     //Get log type name

protected:
	CWnd*  m_pLogFrame;
	HANDLE m_thread_HANDLE;                                      //线程句柄;

public:
	void	SetLogFrame(CWnd* pFrame){ m_pLogFrame = pFrame;}
	CWnd*   GetLogFrame(){return m_pLogFrame;}
};
