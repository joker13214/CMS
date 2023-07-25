#include "StdAfx.h"
#include "UdfRtuDataSave.h"
#include "DataTran_Base.h"


//数据接口线程Class
//基类:CDataTranBase
CDataTran_Base::CDataTran_Base()
{
	m_nMaxData		= 5000;
	m_nMaxLogs		= 200;
	m_strLastLogs = "";
	//m_strDataSource = "";
	m_pParent = NULL;

	m_ThreadType = DataTran_BaseClass;
	
	ResetThread();
}

CDataTran_Base::~CDataTran_Base()
{
	Clear();
}

void CDataTran_Base::AddOneData( COneData_INFO* pData )
{
	if (!pData)
		return;

	if (!m_bNeedData)
	{
		delete pData;
		return ;
	}

	m_cs_DataLst.Lock();
	if (m_lstData.GetCount() >= m_nMaxData)
	{
		COneData_INFO* pDel = m_lstData.RemoveHead();
		if (pDel)
		{
			delete pDel;
			pDel = NULL;
		}
	}
	m_lstData.AddTail(pData);
	m_cs_DataLst.Unlock();
}

void CDataTran_Base::AddThreadLog( Log_Item* pLog )
{
	if (!pLog)
		return ;
	m_cs_LogsLst.Lock();
	if( m_lstLogs.GetCount() >= m_nMaxLogs)
	{
		Log_Item* pDel = m_lstLogs.RemoveHead();
		if (pDel)
		{
			delete pDel;
			pDel  = NULL;
		}
	}

	m_lstLogs.AddTail(pLog);
	m_cs_LogsLst.Unlock();
}

void CDataTran_Base::AddThreadLog(CString logMessage, 
								  _Log_level_type xLevel /* = Normal_Log_Message */, 
								  bool bShow /* = true */, 
								  PSYSTEMTIME pTime /* = NULL */)
{
	logMessage.Trim();
	if (logMessage.IsEmpty())
		return;

	if (logMessage == m_strLastLogs)
		return;

	m_strLastLogs = logMessage;

	Log_Item* pLog = new Log_Item();
	pLog->m_Level = xLevel;
	pLog->m_Message = logMessage;
	if (pTime)
		pLog->m_Time = (*pTime);
	else
		GetLocalTime(&(pLog->m_Time));

	AddThreadLog(pLog);

	//本地入库日志信息不显示在窗口上
	if (m_ThreadType != DataTran_SQLiteLocal &&
		bShow == true)
	{
		AddLog_To_LogFrame(logMessage,xLevel,pTime);
	}

	AddLog_To_LogSystem(logMessage,xLevel,pTime);
}

COneData_INFO* CDataTran_Base::GetOneData()
{
	COneData_INFO* pData = NULL;
	m_cs_DataLst.Lock();
	if (!m_lstData.IsEmpty())
		pData = m_lstData.RemoveHead();
	m_cs_DataLst.Unlock();
	return pData;
}

void CDataTran_Base::Clear()
{
	ClearDataList();
	ClearLogsList();
}

void CDataTran_Base::ClearDataList()
{
	m_cs_DataLst.Lock();
	while (!m_lstData.IsEmpty())
	{
		COneData_INFO* pData = m_lstData.RemoveHead();
		if (pData)
		{
			delete pData;
			pData = NULL;
		}
	}
	m_cs_DataLst.Unlock();
}

void CDataTran_Base::ClearLogsList()
{
	m_cs_LogsLst.Lock();
	while (! m_lstLogs.IsEmpty())
	{
		Log_Item* pLog = m_lstLogs.RemoveHead();
		if (pLog)
		{
			delete pLog;
			pLog = NULL;
		}
	}
	m_cs_LogsLst.Unlock();
}

const int CDataTran_Base::GetDataCount()
{
	int nCount = 0;
	m_cs_DataLst.Lock();
	nCount = m_lstData.GetCount();
	m_cs_DataLst.Unlock();
	return nCount;
}

void CDataTran_Base::ResetThread()
{
	SetThreadHandle(NULL);
	SetThreadID(0);
	SetThreadState(THREAD_STATE_STOP);
	SetThreadControl(false);
}

//void CDataTran_Base::SetDataSource( CString strSource )
//{
//	m_strDataSource = strSource;
//}

void CDataTran_Base::SetParent( CUdfRtuDataSave_Server* pParent )
{
	m_pParent = pParent;
}

CUdfRtuDataSave_Server* CDataTran_Base::GetParent()
{
	return m_pParent;
}

void CDataTran_Base::AddLog_To_LogSystem( CString logMessgae, 
										_Log_level_type xLevel /*= Normal_Log_Message*/, 
										PSYSTEMTIME pTime /*= NULL*/, 
										_LOG_MESSAGE_TYPE LogType /*= DATAIN_ERROR_LOG*/ )
{
	if (m_pParent != NULL)
	{
		m_pParent->AddLog_To_LogSystem(LogType,logMessgae,xLevel,pTime);
	}
}

void CDataTran_Base::AddLog_To_LogFrame( CString LogMessage, 
										_Log_level_type xLevel /*= Normal_Log_Message*/, 
										PSYSTEMTIME pTime /*= NULL*/ )
{
	if (m_pParent != NULL)
	{
		m_pParent->AddLog_To_LogFrame(LogMessage,xLevel,pTime);
	}
}



CDataTranSQLServer_Base::CDataTranSQLServer_Base()
{
	m_strDataSource = "";
	CloseDatabase();
}

CDataTranSQLServer_Base::~CDataTranSQLServer_Base()
{
	CloseDatabase();
}


CDatabase* CDataTranSQLServer_Base::GetDatabase()
{
	return &m_Database;
}

bool CDataTranSQLServer_Base::CloseDatabase()
{
	if (m_Database.IsOpen())
	{
		try
		{
			m_Database.Close();
		}
		catch (CDBException* e)
		{
			TRACE("Close RtuDataSave database Error:%s\r\n",e->m_strError);
			e->Delete();
			return false;
		}
	}
	return true;
}


