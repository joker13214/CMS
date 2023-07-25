#pragma once

#include <afxdao.h>
#include <afxdb.h>
#include <afxtempl.h>
#include <afxmt.h>
#include "UdfLogQueryDlg.h"

enum _LOG_MESSAGE_TYPE
{
	RTU_CM_STATUS_LOG = 1,  //RTU ͨ�� ״̬��־
	RTU_YX_CHANGE_LOG = 2,  //RTU ң�� ��λ��־
	RTU_YC_CHANGE_LOG = 3,  //RTU ң�� ��λ��־
    RTU_CM_BUFFER_LOG = 4,  //RTU ͨ�� ������־
	RTU_YK_OPERAT_LOG = 5,  //RTU ң�� ������־
	RTU_ZD_OPERAT_LOG = 6,  //RTU ��ֵ ������־
	SYS_RUNNING_S_LOG = 7,  //ϵͳ ����ʱ ��־
	SYS_OTHER_RUN_LOG = 8,  //������־��Ϣ    
	DATAIN_ERROR_LOG  = 9,  //����ʵʱ��������־
};




class CUdfLogMessage_Item
{
public:
	_LOG_MESSAGE_TYPE m_bLogType; //��־����

	_Log_level_type   m_level_type; 

	//log��Ϣ
	SYSTEMTIME m_smtLogDateTime; //����ʱ����Ϣ
	CString    m_strLogMessage;  //��־������Ϣ
	CString    m_strDatetime;
	CUdfLogMessage_Item()
	{
		GetLocalTime(&m_smtLogDateTime);
		m_bLogType = SYS_OTHER_RUN_LOG;
		m_strLogMessage = "";
		m_strDatetime = "";
	}
};

//ͨ�Ź���ϵͳ-��־ϵͳ
class CUdfLogSystem
{
public:
	CUdfLogSystem(void);
	CUdfLogSystem(CString _sFile);
public:
	~CUdfLogSystem(void);
public:
	//��ʼ������ϵͳ
	BOOL InitLogSystem(void);   

	//������־��¼��
	BOOL CreateLogTables();

	//Connect Database
	BOOL ConnectLogDB();

	//��ʱɾ����־��Ϣ
	BOOL DeleteLogOnTimer();


	//Insert ��־��Ϣ
	BOOL InsertLogMessage(CUdfLogMessage_Item* _plogItem);
	BOOL InsertLogMessage(_LOG_MESSAGE_TYPE LogType,CString LogMessage,_Log_level_type _levelType = Normal_Log_Message,SYSTEMTIME* pLogTime=NULL);


	BOOL GetLogInfoSaveFlag(_LOG_MESSAGE_TYPE _xType);   //��ȡ��־��Ϣ����/��洢�ı�־

	//get Log messages list
	CPtrList* GetLogList();
	CDatabase* GetDatabase();  //Get database pointer

	//��־�����߳�
    static unsigned long __stdcall ThreadFunction(void *pData);
	void StartLogThread();								//�����߳�
	bool m_blThreadRunFlag;								//�߳� ��������/ֹͣ ������־
	bool m_blThreadRuning;								//�߳� ����/ֹͣ ״̬

protected:
	//��־�洢�ļ� ע��:����·�� 
	CString m_strLogFile;

	//������־���ݿ�����
	CDatabase m_DataBase;
	CDatabase m_QueryDataBase;
	//��ϵ��Ϣ����
	CPtrList  m_lstLogmessages;

	BOOL InsertLogMessageToLogDB(CUdfLogMessage_Item* _plogItem);

public:
	//������
	//CMutex m_LogMutex;   //log Lst Mutex
	//CMutex m_LogDBMutex; //Log db  Mutex

	CCriticalSection m_LogMutex;   //log Lst Mutex
	CCriticalSection m_LogDBMutex; //Log db  Mutex

public:
	//Search Log informations
	CPtrList* SearchLogInformation(_LOG_MESSAGE_TYPE _nType,CUdfLogQueryDlg::QUERY_LOG_TYPE queryLogType=CUdfLogQueryDlg::QUERY_LOGTYPE_ALL,PSYSTEMTIME _beginTime=NULL, PSYSTEMTIME _endTime = NULL);
    
	//��ѯ��־��Ϣ
	int       QueryLogInformation(_LOG_MESSAGE_TYPE _nType,CUdfLogQueryDlg::QUERY_LOG_TYPE queryLogType=CUdfLogQueryDlg::QUERY_LOGTYPE_ALL,PSYSTEMTIME _beginTime=NULL, PSYSTEMTIME _endTime = NULL);

	//Save Log Search result
    CPtrList  m_lstSearchResult;

	//�ֶ�ɾ����־��Ϣ
	bool    DeleteLogByHand(_LOG_MESSAGE_TYPE _nType,PSYSTEMTIME _pBeginTime=NULL,PSYSTEMTIME pEndTime=NULL);
	bool    DeleteLogByHand(_LOG_MESSAGE_TYPE _nType,CString strBeginTime,CString strEndTime);

	//Get name
	static CString GetLogTableName(_LOG_MESSAGE_TYPE _nType);    //Get log database's table name
	static CString GetLogTypeName(_LOG_MESSAGE_TYPE _nType);     //Get log type name

protected:
	CWnd*  m_pLogFrame;
	HANDLE m_thread_HANDLE;                                      //�߳̾��;

public:
	void	SetLogFrame(CWnd* pFrame){ m_pLogFrame = pFrame;}
	CWnd*   GetLogFrame(){return m_pLogFrame;}
};
