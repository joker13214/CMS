#include "StdAfx.h"
#include "CMS.h"
#include "UdfLogSystem.h"
#include "UdfLogFrame.h"
#pragma warning(disable:4995)

CUdfLogSystem::CUdfLogSystem(void)
{
	m_strLogFile.Empty();
	m_blThreadRunFlag = false;
	m_blThreadRuning = false;
	m_pLogFrame = NULL;

	m_thread_HANDLE = NULL;

}


CUdfLogSystem::CUdfLogSystem(CString _sFile)
{
	m_strLogFile = _sFile;
	m_blThreadRunFlag = false;
	m_blThreadRuning = false;
	m_pLogFrame = NULL;

	m_thread_HANDLE = NULL;
}


CUdfLogSystem::~CUdfLogSystem(void)
{

	//�����߳�ֹͣ���б�־
	m_blThreadRunFlag = false;

    //�ȴ��˳��߳�
	while(m_blThreadRuning == true)
	{
		Sleep(0);
	}

    while(!m_lstLogmessages.IsEmpty())
	{
		CUdfLogMessage_Item* pObj = (CUdfLogMessage_Item*)m_lstLogmessages.RemoveHead();

		if (pObj)
		{
			InsertLogMessageToLogDB(pObj);
			delete pObj;
			pObj =NULL;
		}
	}


	if (m_DataBase.IsOpen())
		m_DataBase.Close();


	if (m_QueryDataBase.IsOpen())
		m_QueryDataBase.Close();
}



BOOL CUdfLogSystem::InitLogSystem(void)
{

	//Check file is exist;
	bool blLogDBExist = true;
	if (CUdfGlobalFUNC::IsFileExist(m_strLogFile) == false)  //��־�����ļ�������
	{
		blLogDBExist = false;
		int nCount = 0;

		//������־���ݿ��ļ�,����3��ʧ�ܱ��������ļ�ʧ��
        while(nCount<3)
		{
			CDaoDatabase tmpDaoDB;
			tmpDaoDB.Create(m_strLogFile);

			if (CUdfGlobalFUNC::IsFileExist(m_strLogFile) == true)
				break;

			nCount ++;
		}
        if(nCount >= 3)
		{
			if (CUdfGlobalFUNC::IsFileExist(m_strLogFile) == false)
			{
				AfxMessageBox("������־���ݿ��ļ�ʧ��!",MB_OK|MB_ICONERROR);
				return FALSE;
			}
		}
	}

	//����־���ݿ⽨������
	if (!ConnectLogDB())
	{
		return FALSE;
	}

	if (m_DataBase.IsOpen() && blLogDBExist == false)
	{
		CreateLogTables();
	}


	DeleteLogOnTimer();		//ɾ������Ҫ��¼����־��Ϣ
	StartLogThread();		//������־�߳�

	return TRUE;
}

BOOL CUdfLogSystem::CreateLogTables()
{

	try
	{
		CString strSQL="";

		strSQL.Format("CREATE TABLE objects (name varchar(50),xtype varchar(20),content varchar(50))");

		if (m_DataBase.IsOpen())
			m_DataBase.ExecuteSQL(strSQL);

		//ͨ��״̬��¼��
		strSQL.Format("CREATE TABLE RTU_CM_STATUS_LOG (IDX autoincrement(1,1) primary key,recordtime datetime,logtype int, xlevel int,messages memo)");
	    m_DataBase.ExecuteSQL(strSQL);


		strSQL.Format("INSERT INTO objects(name,xtype,content) VALUES ('RTU_CM_STATUS_LOG','U','ͨ��״̬��¼��')");
		m_DataBase.ExecuteSQL(strSQL);


		//ͨ�ű��ļ�¼��
		strSQL.Format("CREATE TABLE RTU_RS_BUFFER_LOG (IDX autoincrement(1,1) primary key,recordtime datetime,logtype int, xlevel int,messages memo)");
		m_DataBase.ExecuteSQL(strSQL);


		strSQL.Format("INSERT INTO objects(name,xtype,content) VALUES ('RTU_RS_BUFFER_LOG','U','ͨ�ű��ļ�¼��')");
		m_DataBase.ExecuteSQL(strSQL);


        //ң�ز�����¼
		strSQL.Format("CREATE TABLE RTU_YK_OP_LOG (IDX autoincrement(1,1) primary key,recordtime datetime,logtype int, xlevel int,messages memo)");
		m_DataBase.ExecuteSQL(strSQL);


		strSQL.Format("INSERT INTO objects(name,xtype,content) VALUES ('RTU_YK_OP_LOG','U','ң�ز�����¼')");
		m_DataBase.ExecuteSQL(strSQL);

        //ң�ű�λ��¼
		strSQL.Format("CREATE TABLE RTU_YX_CHANGE_LOG (IDX autoincrement(1,1) primary key,recordtime datetime,logtype int, xlevel int,messages memo)");
        m_DataBase.ExecuteSQL(strSQL);
	

		strSQL.Format("INSERT INTO objects(name,xtype,content) VALUES ('RTU_YX_CHANGE_LOG','U','ң�ű�λ��¼')");
		m_DataBase.ExecuteSQL(strSQL);


		//ң���λ��¼
		strSQL.Format("CREATE TABLE RTU_YC_CHANGE_LOG (IDX autoincrement(1,1) primary key,recordtime datetime,logtype int, xlevel int,messages memo)");
		m_DataBase.ExecuteSQL(strSQL);


		strSQL.Format("INSERT INTO objects(name,xtype,content) VALUES ('RTU_YC_CHANGE_LOG','U','ң���λ��¼')");
		m_DataBase.ExecuteSQL(strSQL);


		//�����޸ļ�¼
		strSQL.Format("CREATE TABLE RTU_ZD_MODIFY_LOG (IDX autoincrement(1,1) primary key,recordtime datetime,logtype int, xlevel int,messages memo)");
		m_DataBase.ExecuteSQL(strSQL);


		strSQL.Format("INSERT INTO objects(name,xtype,content) VALUES ('RTU_ZD_MODIFY_LOG','U','�����޸ļ�¼')");
		m_DataBase.ExecuteSQL(strSQL);		


		//������־��Ϣ
		strSQL.Format("CREATE TABLE SYS_OTHER_RUN_LOG (IDX autoincrement(1,1) primary key,recordtime datetime,logtype int, xlevel int,messages memo)");
		m_DataBase.ExecuteSQL(strSQL);


		strSQL.Format("INSERT INTO objects(name,xtype,content) VALUES ('SYS_OTHER_RUN_LOG','U','������־��Ϣ')");
		m_DataBase.ExecuteSQL(strSQL);	


		//ϵͳ ����ʱ ��־
		strSQL.Format("CREATE TABLE SYS_RUNNING_S_LOG (IDX autoincrement(1,1) primary key,recordtime datetime,logtype int, xlevel int,messages memo)");
		m_DataBase.ExecuteSQL(strSQL);


		strSQL.Format("INSERT INTO objects(name,xtype,content) VALUES ('SYS_RUNNING_S_LOG','U','ϵͳ����ʱ��־')");
		m_DataBase.ExecuteSQL(strSQL);	


		//����ʵʱ��������Ϣ
		strSQL.Format("CREATE TABLE DATAIN_ERR_LOG (IDX autoincrement(1,1) primary key,recordtime datetime,logtype int, xlevel int,messages memo)");
		m_DataBase.ExecuteSQL(strSQL);


		strSQL.Format("INSERT INTO objects(name,xtype,content) VALUES ('DATAIN_ERR_LOG','U','����ʵʱ��������Ϣ')");
		m_DataBase.ExecuteSQL(strSQL);	

	}
	catch (CDBException* e)
	{
		//e->ReportError(MB_OK);
		TRACE("CreateLogTables ERROR:%s\r\n",e->m_strError);
		e->Delete();
		return FALSE;
	}

	return TRUE;
}


BOOL CUdfLogSystem::ConnectLogDB()
{

	try
	{
		CString sDsn = "";
		CString sDriver = "MICROSOFT ACCESS DRIVER (*.mdb)";


		sDsn.Format("ODBC;DRIVER={%s};DSN='';DBQ=%s",sDriver,m_strLogFile);

		if (m_DataBase.IsOpen())
			m_DataBase.Close();




		if (!m_DataBase.IsOpen())
		{
			//m_DataBase.Open(NULL,FALSE,FALSE,sDsn);
			if (!m_DataBase.Open(NULL,FALSE,FALSE,sDsn))
			{
				return FALSE;
			}
		}
		else
		{
			return TRUE;
		}


		if (m_QueryDataBase.IsOpen())
			m_QueryDataBase.Close();


		if (!m_QueryDataBase.IsOpen())
		{
			//m_DataBase.Open(NULL,FALSE,FALSE,sDsn);
			if (!m_QueryDataBase.Open(NULL,FALSE,FALSE,sDsn))
			{
				return FALSE;
			}
		}
		else
		{
			return TRUE;
		}

	}
	catch (CDBException* e)
	{
		//e->ReportError(MB_OK|MB_ICONERROR);
		TRACE("ConnectLogDB error:%s\r\n",e->m_strError);
		e->Delete();
		return FALSE;
	}
	return TRUE;
}


BOOL CUdfLogSystem::DeleteLogOnTimer()
{
	SYSTEMTIME st;
	GetLocalTime(&st);

	long TotalMonths = st.wYear * 12 + st.wMonth;


	CCMSApp* pApp = (CCMSApp*)AfxGetApp();

	
	long _logEndMonth = TotalMonths - pApp->m_GlobalOptions.m_dwLogSaveMonths;

	CString strDelEndTime = "";
	if ( _logEndMonth % 12 == 0 )
		strDelEndTime.Format("%04d-12-01 00:00:00",  long(_logEndMonth /12 ) -1);
	else
		strDelEndTime.Format("%04d-%02d-01 00:00:00",long(_logEndMonth /12 ),long(_logEndMonth % 12));

	try
	{
		CString strSQL="";


		strSQL.Format("DELETE FROM RTU_CM_STATUS_LOG WHERE recordtime < #%s#",strDelEndTime);
		m_LogDBMutex.Lock();
		m_DataBase.ExecuteSQL(strSQL);
		m_LogDBMutex.Unlock();

		strSQL.Format("DELETE FROM RTU_RS_BUFFER_LOG WHERE recordtime < #%s#",strDelEndTime);
		m_LogDBMutex.Lock();
		m_DataBase.ExecuteSQL(strSQL);
		m_LogDBMutex.Unlock();

		strSQL.Format("DELETE FROM RTU_YK_OP_LOG WHERE recordtime <  #%s#",strDelEndTime);
		m_LogDBMutex.Lock();
		m_DataBase.ExecuteSQL(strSQL);
		m_LogDBMutex.Unlock();

		strSQL.Format("DELETE FROM RTU_YX_CHANGE_LOG WHERE recordtime < #%s#",strDelEndTime);
		m_LogDBMutex.Lock();
		m_DataBase.ExecuteSQL(strSQL);
		m_LogDBMutex.Unlock();

		strSQL.Format("DELETE FROM RTU_YC_CHANGE_LOG WHERE recordtime < #%s#",strDelEndTime);
		m_LogDBMutex.Lock();
		m_DataBase.ExecuteSQL(strSQL);
		m_LogDBMutex.Unlock();

		strSQL.Format("DELETE FROM RTU_ZD_MODIFY_LOG WHERE recordtime < #%s#",strDelEndTime);
		m_LogDBMutex.Lock();
		m_DataBase.ExecuteSQL(strSQL);
		m_LogDBMutex.Unlock();

		strSQL.Format("DELETE FROM SYS_OTHER_RUN_LOG WHERE recordtime < #%s#",strDelEndTime);
		m_LogDBMutex.Lock();
		m_DataBase.ExecuteSQL(strSQL);
		m_LogDBMutex.Unlock();

		strSQL.Format("DELETE FROM SYS_RUNNING_S_LOG WHERE recordtime < #%s#",strDelEndTime);
		m_LogDBMutex.Lock();
		m_DataBase.ExecuteSQL(strSQL);
		m_LogDBMutex.Unlock();

		strSQL.Format("DELETE FROM DATAIN_ERR_LOG WHERE recordtime < #%s#",strDelEndTime);
		m_LogDBMutex.Lock();
		m_DataBase.ExecuteSQL(strSQL);
		m_LogDBMutex.Unlock();
	}
	catch (CDBException* e)
	{
		m_LogDBMutex.Unlock();
		TRACE("DeleteLogOnTimer Error:%s\r\n",e->m_strError);
		e->Delete();
		return FALSE;

	}

	return TRUE;
}



BOOL CUdfLogSystem::InsertLogMessage(CUdfLogMessage_Item* _plogItem)
{

	if (!_plogItem)
		return FALSE;

	if (!GetLogInfoSaveFlag(_plogItem->m_bLogType))
	{
		delete _plogItem;
		_plogItem = NULL;
		return FALSE;
	}

	m_LogMutex.Lock();
	m_lstLogmessages.AddTail((void*)_plogItem);
	if (m_lstLogmessages.GetCount() > 5000)
	{
		CUdfLogMessage_Item* pObj = (CUdfLogMessage_Item*)m_lstLogmessages.RemoveHead();
		if (pObj)
		{
			delete pObj;
			pObj = NULL;
		}
	}
	m_LogMutex.Unlock();

	return TRUE;
}


BOOL CUdfLogSystem::InsertLogMessage(_LOG_MESSAGE_TYPE LogType,CString LogMessage,_Log_level_type _levelType,SYSTEMTIME* pLogTime/*=NULL*/)
{
	if (!GetLogInfoSaveFlag(LogType))
		return FALSE;

	if (LogMessage.IsEmpty())
		return FALSE;

	LogMessage.Replace("'","\"");  //ɾ����־��Ϣ�е�"�ַ�,�����ܴ��

	CUdfLogMessage_Item* pObj = NULL;
	pObj = new CUdfLogMessage_Item();

	BOOL blResult = FALSE;
	if (pObj)
	{
		if (pLogTime != NULL)
		{
			pObj->m_smtLogDateTime.wDay				= pLogTime->wDay;
			pObj->m_smtLogDateTime.wDayOfWeek		= pLogTime->wDayOfWeek;
			pObj->m_smtLogDateTime.wHour			= pLogTime->wHour;
			pObj->m_smtLogDateTime.wMilliseconds	= pLogTime->wMilliseconds;
			pObj->m_smtLogDateTime.wMinute			= pLogTime->wMinute;
			pObj->m_smtLogDateTime.wMonth			= pLogTime->wMonth;
			pObj->m_smtLogDateTime.wSecond			= pLogTime->wSecond;
			pObj->m_smtLogDateTime.wYear			= pLogTime->wYear;
		}
		else
		{
			GetLocalTime(&pObj->m_smtLogDateTime);
		}
		pObj->m_bLogType = LogType;
		pObj->m_strLogMessage = LogMessage;
		pObj->m_level_type = _levelType;
		
		blResult = InsertLogMessage(pObj);
	}

	return blResult;
}



void CUdfLogSystem::StartLogThread()
{
    m_blThreadRunFlag = true;
	DWORD threadID = 0;
	m_thread_HANDLE = ::CreateThread(NULL,0,ThreadFunction,this,0,&threadID);
	if (m_thread_HANDLE == NULL)
	{
		TRACE("Error:������־ϵͳ�߳�ʧ��!\r\n");
	}
	else
	{
		//DWORD threadID = ::GetThreadId(m_thread_HANDLE);
		TRACE("��־ϵͳ��̨�߳�ID:%d\r\n",threadID);
		//CString logMessage="";
		//logMessage.Format("�ɹ�������־ϵͳ�߳�,�߳�ID:%d",threadID);
		//InsertLogMessage(SYS_OTHER_RUN_LOG,logMessage,Normal_Log_Message);
	}
}



//�̺߳���
unsigned long CUdfLogSystem::ThreadFunction(void *pData)
{

	bool  blRuningDeleteFlag = false;

	CUdfLogSystem* pModule = (CUdfLogSystem*)pData;
	if(pModule != NULL)
	{
		pModule->m_blThreadRuning = true;
		while(pModule->m_blThreadRunFlag != false )
		{
			//�洢��־
			CUdfLogMessage_Item* pLogItem  = NULL;

			pModule->m_LogMutex.Lock();
			CPtrList* plogLst = pModule->GetLogList();
			if (plogLst && !plogLst->IsEmpty())
				pLogItem = (CUdfLogMessage_Item*)plogLst->RemoveHead();
			pModule->m_LogMutex.Unlock();

			if (pLogItem)
			{
				pModule->InsertLogMessageToLogDB(pLogItem);	

				delete pLogItem;
				pLogItem = NULL;
			}

			SYSTEMTIME systime;
			GetLocalTime(&systime);

			//��ʱɾ����־
			if (systime.wDay == 1 &&(blRuningDeleteFlag == false))
			{
				pModule->DeleteLogOnTimer();
				blRuningDeleteFlag = true;
			}
		    if (systime.wDay != 1 && blRuningDeleteFlag == true)
			{
				blRuningDeleteFlag = false;
			}
		    Sleep(1);
		}
		pModule->m_blThreadRuning = false;
	}


	return 0;
}


CPtrList* CUdfLogSystem::GetLogList()
{
	return &m_lstLogmessages;
}


CDatabase* CUdfLogSystem::GetDatabase()
{
	return &m_DataBase;
}

BOOL CUdfLogSystem::InsertLogMessageToLogDB(CUdfLogMessage_Item* _plogItem)
{
	try
	{
		if (!_plogItem)
			return FALSE;


		CString _sSQL="";
		CString _sTableName="";

		_sTableName = GetLogTableName(_plogItem->m_bLogType);

		_sTableName.Trim();
		if (_sTableName.IsEmpty())
			return FALSE;

		CString _strTime="";
		_strTime.Format("%04d-%02d-%02d %02d:%02d:%02d",
			_plogItem->m_smtLogDateTime.wYear,
			_plogItem->m_smtLogDateTime.wMonth,
			_plogItem->m_smtLogDateTime.wDay,
			_plogItem->m_smtLogDateTime.wHour,
			_plogItem->m_smtLogDateTime.wMinute,
			_plogItem->m_smtLogDateTime.wSecond);



		_sSQL.Format("INSERT INTO %s (recordtime,logtype,xlevel,messages) VALUES ('%s',%d, %d,'%s')",
			         _sTableName,
					 _strTime,
					 (int)_plogItem->m_bLogType,
					 (int)_plogItem->m_level_type,
					 _plogItem->m_strLogMessage);


		m_LogDBMutex.Lock();
		m_DataBase.ExecuteSQL(_sSQL);
		m_LogDBMutex.Unlock();

	}
	catch (CDBException* e)
	{
		m_LogDBMutex.Unlock();
		TRACE("InsertLogMessageToLogDB error:%s\r\n",e->m_strError);
		e->Delete();
		return FALSE;
	}

	return TRUE;
}


CPtrList* CUdfLogSystem::SearchLogInformation(_LOG_MESSAGE_TYPE _nType,CUdfLogQueryDlg::QUERY_LOG_TYPE queryLogType,PSYSTEMTIME _beginTime/* =NULL */, PSYSTEMTIME _endTime /* = NULL */)
{
	if (!m_lstSearchResult.IsEmpty())
		m_lstSearchResult.RemoveAll();

	try
	{


		CRecordset rs;

		CString _sSQL="";
		CString _sTableName="";

		_sTableName = GetLogTableName(_nType);

		if (_sTableName.IsEmpty())
			return NULL;

		CString strWhereLogType="";
		if (queryLogType == CUdfLogQueryDlg::QUERY_LOGTYPE_ALL)
			strWhereLogType = "";
		else if (queryLogType == CUdfLogQueryDlg::QUERY_LOGTYPE_WORNING)
			strWhereLogType = " xlevel = 2  ";
		else if (queryLogType == CUdfLogQueryDlg::QUERY_LOGTYPE_ERROR)
			strWhereLogType = " xlevel = 3  ";
		else if (queryLogType == CUdfLogQueryDlg::QUERY_LOGTYPE_WORNINGANDERROR)
			strWhereLogType = " ( xlevel = 2 or xlevel = 3)  ";


		if (_beginTime ==  NULL || _endTime == NULL)
		{
			if (strWhereLogType.IsEmpty())
				_sSQL.Format("SELECT IDX,recordtime,logtype,xlevel,messages FROM %s ORDER BY IDX DESC",_sTableName);
			else
				_sSQL.Format("SELECT IDX,recordtime,logtype,xlevel,messages FROM %s WHERE %s ORDER BY IDX DESC",_sTableName,strWhereLogType);
		}
		else
		{
			CString _strBeginTime="",_strEndTime="";

			_strBeginTime.Format("%04d-%02d-%02d %02d:%02d:%02d",
				_beginTime->wYear,
				_beginTime->wMonth,
				_beginTime->wDay,
				_beginTime->wHour,
				_beginTime->wMinute,
				_beginTime->wSecond);
			_strEndTime.Format("%04d-%02d-%02d %02d:%02d:%02d",
				_endTime->wYear,
				_endTime->wMonth,
				_endTime->wDay,
				_endTime->wHour,
				_endTime->wMinute,
				_endTime->wSecond);
			if (strWhereLogType.IsEmpty())
				_sSQL.Format("SELECT recordtime,logtype,xlevel,messages FROM %s  WHERE recordtime between #%s# and #%s# ORDER BY recordtime DESC",
				_sTableName,
				_strBeginTime,
				_strEndTime);
			else
				_sSQL.Format("SELECT recordtime,logtype,xlevel,messages FROM %s  WHERE %s AND recordtime BETWEEN #%s# AND #%s# ORDER BY recordtime DESC",
				_sTableName,
				strWhereLogType,
				_strBeginTime,
				_strEndTime);
		}

		//m_LogDBMutex.Lock();
		rs.m_pDatabase = &m_QueryDataBase;
		rs.Open(CRecordset::forwardOnly,_sSQL);
		//m_LogDBMutex.Unlock();

		while(!rs.IsEOF())
		{
			CString recordtime ="",logtype="",xLevel="",messages="";

			rs.GetFieldValue("recordtime",recordtime);
			rs.GetFieldValue("logtype",logtype);
			rs.GetFieldValue("xlevel",xLevel);
			rs.GetFieldValue("messages",messages);

			_Log_level_type _loglevel = (_Log_level_type)atoi(xLevel);
			_LOG_MESSAGE_TYPE _LOGTYPE = (_LOG_MESSAGE_TYPE)atoi(logtype);

			CUdfLogMessage_Item* pObj = new CUdfLogMessage_Item();
			messages.Replace("\"","'");
			pObj->m_bLogType = _LOGTYPE;
			pObj->m_level_type = _loglevel;
			pObj->m_strDatetime = recordtime;
			pObj->m_strLogMessage = messages;

			m_lstSearchResult.AddTail(pObj);

			rs.MoveNext();
		}
		rs.Close();

	}
	catch (CDBException* e)
	{
		TRACE("SearchLogInformation ERROR:%s\n",e->m_strError);
		e->Delete();

		m_LogDBMutex.Unlock();
		return NULL;
	}

	return &m_lstSearchResult;
}


//************************************
// Method:    QueryLogInformation
// FullName:  CUdfLogSystem::QueryLogInformation
// Access:    public 
// Returns:   int ��ѯ��־��¼��
// Qualifier: ��־ϵͳ��ѯ����ʾ��־��Ϣ
// Parameter: _LOG_MESSAGE_TYPE _nType                        ��ѯ��־����
// Parameter: CUdfLogQueryDlg::QUERY_LOG_TYPE queryLogType    ��ѯ��Χ(ȫ��,����,����,�����ʹ���)
// Parameter: PSYSTEMTIME _beginTime						  ��ѯ��ʼʱ��
// Parameter: PSYSTEMTIME _endTime							  ��ѯ����ʱ��
//************************************
int CUdfLogSystem::QueryLogInformation(_LOG_MESSAGE_TYPE _nType,CUdfLogQueryDlg::QUERY_LOG_TYPE queryLogType/* =CUdfLogQueryDlg::QUERY_LOGTYPE_ALL */,PSYSTEMTIME _beginTime/* =NULL */, PSYSTEMTIME _endTime /* = NULL */)
{
	int nLogsCount = 0;
	CUdfLogFrame* plogFrame = (CUdfLogFrame*)GetLogFrame();
	if (!plogFrame  || !plogFrame->m_pRightView  ||
		!plogFrame->m_pRightView->GetSafeHwnd()   ||
		!plogFrame->m_pRightView->m_pListCtrl.GetSafeHwnd())
		return 0;

	CListCtrl* pListCtrl = &(plogFrame->m_pRightView->m_pListCtrl);
	try
	{
		CRecordset rs;

		CString _sSQL="";
		CString _sTableName="";

		_sTableName = GetLogTableName(_nType);

		if (_sTableName.IsEmpty())
			return NULL;

		CString strWhereLogType="";
		if (queryLogType == CUdfLogQueryDlg::QUERY_LOGTYPE_ALL)
			strWhereLogType = "";
		else if (queryLogType == CUdfLogQueryDlg::QUERY_LOGTYPE_WORNING)
			strWhereLogType = " xlevel = 2  ";
		else if (queryLogType == CUdfLogQueryDlg::QUERY_LOGTYPE_ERROR)
			strWhereLogType = " xlevel = 3  ";
		else if (queryLogType == CUdfLogQueryDlg::QUERY_LOGTYPE_WORNINGANDERROR)
			strWhereLogType = " ( xlevel = 2 or xlevel = 3)  ";


		if (_beginTime ==  NULL || _endTime == NULL)
		{
			//if (strWhereLogType.IsEmpty())
			//	_sSQL.Format("SELECT IDX,recordtime,logtype,xlevel,messages FROM %s ORDER BY IDX ASC",_sTableName);
			//else
			//	_sSQL.Format("SELECT IDX,recordtime,logtype,xlevel,messages FROM %s WHERE %s ORDER BY IDX ASC",_sTableName,strWhereLogType);
			if (strWhereLogType.IsEmpty())
				_sSQL.Format("SELECT TOP 1000 IDX,recordtime,logtype,xlevel,messages FROM %s ORDER BY IDX DESC",_sTableName);
			else
				_sSQL.Format("SELECT TOP 1000 IDX,recordtime,logtype,xlevel,messages FROM %s WHERE %s ORDER BY IDX DESC",_sTableName,strWhereLogType);
		}
		else
		{
			CString _strBeginTime="",_strEndTime="";

			_strBeginTime.Format("%04d-%02d-%02d %02d:%02d:%02d",
				_beginTime->wYear,
				_beginTime->wMonth,
				_beginTime->wDay,
				_beginTime->wHour,
				_beginTime->wMinute,
				_beginTime->wSecond);
			_strEndTime.Format("%04d-%02d-%02d %02d:%02d:%02d",
				_endTime->wYear,
				_endTime->wMonth,
				_endTime->wDay,
				_endTime->wHour,
				_endTime->wMinute,
				_endTime->wSecond);
			//if (strWhereLogType.IsEmpty())
			//	_sSQL.Format("SELECT recordtime,logtype,xlevel,messages FROM %s  WHERE recordtime between #%s# and #%s# ORDER BY recordtime DESC",
			//	_sTableName,
			//	_strBeginTime,
			//	_strEndTime);
			//else
			//	_sSQL.Format("SELECT recordtime,logtype,xlevel,messages FROM %s  WHERE %s AND recordtime BETWEEN #%s# AND #%s# ORDER BY recordtime DESC",
			//	_sTableName,
			//	strWhereLogType,
			//	_strBeginTime,
			//	_strEndTime);
			if (strWhereLogType.IsEmpty())
				_sSQL.Format("SELECT TOP 1000 recordtime,logtype,xlevel,messages FROM %s  WHERE recordtime between #%s# and #%s# ORDER BY recordtime DESC",
				_sTableName,
				_strBeginTime,
				_strEndTime);
			else
				_sSQL.Format("SELECT  TOP 1000 recordtime,logtype,xlevel,messages FROM %s  WHERE %s AND recordtime BETWEEN #%s# AND #%s# ORDER BY recordtime DESC",
				_sTableName,
				strWhereLogType,
				_strBeginTime,
				_strEndTime);
		}

		//m_LogDBMutex.Lock();
		rs.m_pDatabase = &m_QueryDataBase;
		rs.Open(CRecordset::forwardOnly,_sSQL);
		//m_LogDBMutex.Unlock();

		while(!rs.IsEOF())
		{
			CString recordtime ="",logtype="",xLevel="",messages="";

			rs.GetFieldValue("recordtime",recordtime);
			rs.GetFieldValue("logtype",logtype);
			rs.GetFieldValue("xlevel",xLevel);
			rs.GetFieldValue("messages",messages);

			_Log_level_type _loglevel = (_Log_level_type)atoi(xLevel);
			_LOG_MESSAGE_TYPE _LOGTYPE = (_LOG_MESSAGE_TYPE)atoi(logtype);

			messages.Replace("\"","'");
			CString strXLevel="";

			switch (_loglevel)
			{
			case Normal_Log_Message:
				strXLevel ="��Ϣ";
				break;
			case Warning_log_Message:
				strXLevel = "����";
				break;
			case Error_Log_message:
				strXLevel ="����";
				break;
			}

			CString strNo="";
			//strNo.Format("%d",nLogsCount);
			//int itemIdx = pListCtrl->InsertItem(nLogsCount,strNo);
			int itemIdx = pListCtrl->InsertItem(0,strNo);
			pListCtrl->SetItemData(itemIdx,_loglevel);


			pListCtrl->SetItemText(itemIdx,1,strXLevel);
			pListCtrl->SetItemText(itemIdx,2,recordtime);

			CString strLogType ="";
			strLogType  = CUdfLogSystem::GetLogTypeName(_nType);

			pListCtrl->SetItemText(itemIdx,3,strLogType);
			pListCtrl->SetItemText(itemIdx,4,messages);

			nLogsCount ++;

			rs.MoveNext();

			Sleep(0);
		}
		rs.Close();

	}
	catch (CDBException* e)
	{
		TRACE("SearchLogInformation ERROR:%s\n",e->m_strError);
		e->Delete();

		m_LogDBMutex.Unlock();
		return 0;
	}

	return nLogsCount;
}


//************************************
// Method:    DeleteLogByHand
// FullName:  CUdfLogSystem::DeleteLogByHand
// Access:    public 
// Returns:   bool
// Qualifier: �ֶ�ɾ����־��Ϣ
// Parameter: _LOG_MESSAGE_TYPE _nType  ɾ����־������
// Parameter: PSYSTEMTIME _pBeginTime   ɾ����־����ʼʱ��
// Parameter: PSYSTEMTIME pEndTime      ɾ����־�Ľ���ʱ��
//************************************
bool CUdfLogSystem::DeleteLogByHand(_LOG_MESSAGE_TYPE _nType,PSYSTEMTIME _pBeginTime/* =NULL */,PSYSTEMTIME pEndTime/* =NULL */)
{
	CString strBegintime="",strEndtime="";

	if (_pBeginTime != NULL )
	{
		strBegintime.Format("%04d-%02d-%02d %02d:%02d:%02d",
			_pBeginTime->wYear,
			_pBeginTime->wMonth,
			_pBeginTime->wDay,
			_pBeginTime->wHour,
			_pBeginTime->wMinute,
			_pBeginTime->wSecond);
	}

	if (pEndTime != NULL)
	{
		strEndtime.Format("%04d-%02d-%02d %02d:%02d:%02d",
			pEndTime->wYear,
			pEndTime->wMonth,
			pEndTime->wDay,
			pEndTime->wHour,
			pEndTime->wMinute,
			pEndTime->wSecond);
	}

	return DeleteLogByHand(_nType,strBegintime,strEndtime);
}

//************************************
// Method:    DeleteLogByHand
// FullName:  CUdfLogSystem::DeleteLogByHand
// Access:    public 
// Returns:   bool
// Qualifier: �ֶ�ɾ����־��Ϣ
// Parameter: _LOG_MESSAGE_TYPE _nType  ɾ����־������
// Parameter: CString strBeginTime      ɾ����־����ʼʱ��
// Parameter: CString strEndTime	    ɾ����־�Ľ���ʱ��
//************************************
bool CUdfLogSystem::DeleteLogByHand(_LOG_MESSAGE_TYPE _nType,CString strBeginTime,CString strEndTime)
{
	CString strSQL="";
	CString _sTableName="";

    _sTableName = GetLogTableName(_nType);

	if (_sTableName.IsEmpty())
		return NULL;

	strBeginTime.TrimLeft();
	strBeginTime.TrimRight();
	strEndTime.TrimLeft();
	strEndTime.TrimRight();

	if (strBeginTime.IsEmpty() && strEndTime.IsEmpty())
	{
		strSQL.Format("DELETE FROM %s", _sTableName);
	}

	if (strBeginTime.IsEmpty() && (!strEndTime.IsEmpty()))
	{
		strSQL.Format("DELETE FROM %s WHERE recordtime < #%s#",_sTableName,strEndTime);
	}

	if ((!strBeginTime.IsEmpty()) && (!strEndTime.IsEmpty()))
	{
		strSQL.Format("DELETE FROM %s WHERE recordtime BETWEEN  #%s# AND #%s#",_sTableName,strBeginTime,strEndTime);
	}


	if (!strSQL.IsEmpty())
	{
		try
		{
			m_LogDBMutex.Lock();
			m_DataBase.ExecuteSQL(strSQL);
			m_LogDBMutex.Unlock();
		}
		catch (CDBException* e)
		{
			m_LogDBMutex.Unlock();
			TRACE("DeleteLogByHand ERROR:%s\r\n",e->m_strError);
			e->Delete();
			return false;
		}
	}

	return true;
}




CString CUdfLogSystem::GetLogTableName(_LOG_MESSAGE_TYPE _nType)
{
	CString _sTableName="";

	switch (_nType)
	{
	case RTU_CM_STATUS_LOG:
		{
			_sTableName="RTU_CM_STATUS_LOG";
		}
		break;
	case RTU_CM_BUFFER_LOG:
		{
			_sTableName="RTU_RS_BUFFER_LOG";
		}
		break;
	case  RTU_YK_OPERAT_LOG:
		{
			_sTableName="RTU_YK_OP_LOG";
		}
		break;
	case RTU_YC_CHANGE_LOG:
		{
			_sTableName="RTU_YC_CHANGE_LOG";
		}
		break;;
	case  RTU_YX_CHANGE_LOG:
		{
			_sTableName="RTU_YX_CHANGE_LOG";
		}
		break;
	case  RTU_ZD_OPERAT_LOG:
		{
			_sTableName="RTU_ZD_MODIFY_LOG";
		}
		break;
	case  SYS_OTHER_RUN_LOG:
		{
			_sTableName="SYS_OTHER_RUN_LOG";
		}
		break;
	case  SYS_RUNNING_S_LOG:
		{
			_sTableName="SYS_RUNNING_S_LOG";
		}
		break;
	case DATAIN_ERROR_LOG:
		{
			_sTableName = "DATAIN_ERR_LOG";
		}
	}

	return _sTableName;
}



CString CUdfLogSystem::GetLogTypeName(_LOG_MESSAGE_TYPE _nType)
{
	CString strLogType ="";
	switch (_nType)
	{
	case RTU_CM_STATUS_LOG:
		{
			strLogType="ͨ��״̬��־";
		}
		break;
	case RTU_CM_BUFFER_LOG:
		{
			strLogType="ͨ�ű�����־";
		}
		break;
	case  RTU_YK_OPERAT_LOG:
		{
			strLogType="ң�ز�����־";
		}
		break;
	case RTU_YC_CHANGE_LOG:
		{
			strLogType="ң���λ��־";
		}
		break;;
	case  RTU_YX_CHANGE_LOG:
		{
			strLogType="ң�ű�λ��־";
		}
		break;
	case  RTU_ZD_OPERAT_LOG:
		{
			strLogType="��ֵ������־";
		}
		break;
	case  SYS_OTHER_RUN_LOG:
		{
			strLogType="������־��Ϣ";
		}
		break;
	case  SYS_RUNNING_S_LOG:
		{
			strLogType="ϵͳ����ʱ��־";
		}
		break;
	case  DATAIN_ERROR_LOG:
		{
			strLogType ="���ݴ�������־";
		}
	}
	return strLogType;
}



BOOL CUdfLogSystem::GetLogInfoSaveFlag(_LOG_MESSAGE_TYPE _xType)
{
	BOOL result = FALSE;
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();

	switch(_xType)
	{
	case RTU_CM_STATUS_LOG:
		{
			result = (pApp->m_GlobalOptions.m_LogOpts.CmStatusSave_Flag ==0? FALSE:TRUE);
		}
		break;
	case RTU_CM_BUFFER_LOG:
		{
			result = (pApp->m_GlobalOptions.m_LogOpts.CmBufferSave_Flag ==0? FALSE:TRUE);
		}
		break;
	case  RTU_YK_OPERAT_LOG:
		{
			result = (pApp->m_GlobalOptions.m_LogOpts.YkOperatSave_Flag ==0? FALSE:TRUE);
		}
		break;
	case RTU_YC_CHANGE_LOG:
		{
			result = (pApp->m_GlobalOptions.m_LogOpts.YcChangeSave_Flag ==0? FALSE:TRUE);
		}
		break;;
	case  RTU_YX_CHANGE_LOG:
		{
			result = (pApp->m_GlobalOptions.m_LogOpts.YxChangeSave_Flag ==0? FALSE:TRUE);
		}
		break;
	case  RTU_ZD_OPERAT_LOG:
		{
			result = (pApp->m_GlobalOptions.m_LogOpts.ZdOperatSave_Flag ==0? FALSE:TRUE);
		}
		break;
	case  SYS_OTHER_RUN_LOG:
		{
			result = (pApp->m_GlobalOptions.m_LogOpts.OhterRunSave_FLag ==0? FALSE:TRUE);
		}
		break;
	case  SYS_RUNNING_S_LOG:
		{
			result = (pApp->m_GlobalOptions.m_LogOpts.RunningSSave_Flag ==0? FALSE:TRUE);
		}
		break;
	case  DATAIN_ERROR_LOG:
		{
			result = (pApp->m_GlobalOptions.m_LogOpts.DataInErrSave_Flag ==0? FALSE:TRUE);
		}
	}
	return result;
}