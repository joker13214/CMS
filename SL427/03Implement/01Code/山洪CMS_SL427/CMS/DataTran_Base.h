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


//����ת������,�������
class CDataTran_Base
{
public:
	CDataTran_Base();
	virtual ~CDataTran_Base();
public:
	virtual long ThreadFunction() = 0;                 //�̴߳�����
	virtual void SetDataSource(CString strSource) = 0;
public:
	void AddOneData(COneData_INFO* pData);	
	void AddThreadLog(CString logMessage,
		_Log_level_type xLevel = Normal_Log_Message,
		bool   bShow = true,
		PSYSTEMTIME pTime = NULL);					   //�����־��Ϣ

public:
	HANDLE GetThreadHandle()					{return m_handle_Thread;}					//��ȡ�߳̾��(HANDLE)  
	const DWORD  GetThreadID()					{return m_dwThreadID;}						//��ȡ�̵߳�ID
	const BYTE GetThreadState()					{return m_bThreadState;}					//��ȡ�̵߳�״̬
	const bool GetThreadControl()				{return m_bThreadControl;}					//��ȡ����ֹͣ���Ʊ�־��true:���У�falseֹͣ

	void SetThreadHandle(HANDLE newThreadHandle){m_handle_Thread = newThreadHandle;}		//�����߳̾��(HANDLE)
	void SetThreadID(DWORD newThreadID)			{m_dwThreadID = newThreadID;}				//�����̵߳�ID
	void SetThreadState(BYTE threadState)		{m_bThreadState = threadState;}				//�����̵߳�״̬
	void SetThreadControl(bool nValue)			{m_bThreadControl = nValue;}				//��������ֹͣ���Ʊ�־��true:���У�falseֹͣ
	void ResetThread();
	void SetParent(CUdfRtuDataSave_Server* pParent);
	CUdfRtuDataSave_Server* GetParent();

	DataTran_ThreadType GetDataTran_ThreadType(){return m_ThreadType;}
	CList<Log_Item*>* GetLogListPointer()		{return &m_lstLogs;}
protected:
	CCriticalSection		m_cs_DataLst;		
	CList<COneData_INFO*>	m_lstData;
	bool                    m_bNeedData;				//�Ƿ���Ҫԭʼ�ɼ�����

	CCriticalSection		m_cs_LogsLst;
	CList<Log_Item*>		m_lstLogs;

	int						m_nMaxData;					//�ڴ���������ݱ��棬Ĭ��:5000;
	int						m_nMaxLogs;					//�ڴ��������־���棬Ĭ��:200;

	HANDLE	m_handle_Thread;																//�߳̾��
	DWORD	m_dwThreadID;																	//�߳�ID
	BYTE	m_bThreadState;																	//�߳�״̬
	bool	m_bThreadControl;																//�߳̿���; true ����  false ֹͣ�߳�

	CString  m_strLastLogs;																	//last log message
	//CString	 m_strDataSource;															//����Դ		

	DataTran_ThreadType     m_ThreadType;
	CUdfRtuDataSave_Server* m_pParent;
protected:
	COneData_INFO* GetOneData();
	void Clear();
	void ClearDataList();
	void ClearLogsList();
	void AddThreadLog(Log_Item* pLog);
	const int GetDataCount();		//���ݼ���
protected:
	//�����־��Ϣ����־ϵͳ,����־ϵͳ�ı���
	void AddLog_To_LogSystem(CString logMessgae,
		                    _Log_level_type		xLevel = Normal_Log_Message,
							PSYSTEMTIME			pTime = NULL,
							_LOG_MESSAGE_TYPE	LogType = DATAIN_ERROR_LOG);

	//�����־��Ϣ����־������ʾ
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