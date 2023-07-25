#include "StdAfx.h"
#include <afxdb.h>
#include "UdfRtuDataSave.h"
#include "CMS.h"
#include "MainFrm.h"
#include "DataTranSQLite_Local.h"
#include "DataTranSQLServer_GM.h"
#include "DataTranSQLServer_SC.h"

//COneData_INFO类的函数实现
COneData_INFO::COneData_INFO()
{
	mProvinceCode = 0;
	mRegionCode = 0;
	mCountyCode = 0;
	mA2 = 0;

	m_protocoltype = 0;

	mDataType = (_Data_Type)0;
	mAlarmType = 0;
	mOperateDESC = "";
	mOperateAccount = "";

	m_RtuID = "";
	m_OperateTimeString = "";
	m_RecvTimeString = "";
	m_Alarm_PID = "";
	m_CollectTimeString = "";
	sPeaceReport = "";

	GetLocalTime(&mCollectTime);
	GetLocalTime(&mRecvTime);
	GetLocalTime(&mOperateTime);
}

COneData_INFO::COneData_INFO( COneData_INFO* pSourceData )
{
	if (pSourceData != NULL)  
	{
		//Copy data
		if(pSourceData->m_protocoltype==0)
		{
			mProvinceCode = pSourceData->mProvinceCode;
			mRegionCode = pSourceData->mRegionCode;
			mCountyCode = pSourceData->mCountyCode;
			mA2 = pSourceData->mA2;

			mDataType = pSourceData->mDataType;
			mAlarmType = pSourceData->mAlarmType;
			mOperateDESC = pSourceData->mOperateDESC;
			mOperateAccount = pSourceData->mOperateAccount;

			mCollectTime = pSourceData->mCollectTime;
			mRecvTime = pSourceData->mRecvTime;
			mOperateTime = pSourceData->mOperateTime;
			mTP = pSourceData->mTP;

			sPeaceReport = pSourceData->sPeaceReport;
		}
		else
		{
			m_protocoltype = pSourceData->m_protocoltype;
			mA2 = pSourceData->mA2;
			
			m_syqSendTime.SetTime(pSourceData->m_syqSendTime);
			m_syqGetTime.month =pSourceData->m_syqGetTime.month;
			m_syqGetTime.day = pSourceData->m_syqGetTime.day;
			m_syqGetTime.hour =pSourceData->m_syqGetTime.hour;
			m_syqGetTime.min =pSourceData->m_syqGetTime.min;
		}



		

		CopyLevelList(pSourceData);
		CopyPressList(pSourceData);
		CopyFlowList(pSourceData);
		CopyQualityList(pSourceData);
		CopyRainList(pSourceData);
		CopyAnalogList(pSourceData);
		CopyPulseList(pSourceData);
	}
	else
	{
		mProvinceCode = 0;
		mRegionCode = 0;
		mCountyCode = 0;
		mA2 = 0;

		mDataType = (_Data_Type)0;
		mAlarmType = 0;
		mOperateDESC = "";
		mOperateAccount = "";

		m_RtuID = "";
		m_OperateTimeString = "";
		m_RecvTimeString = "";
		m_Alarm_PID = "";
		m_CollectTimeString = "";

		GetLocalTime(&mCollectTime);
		GetLocalTime(&mRecvTime);
		GetLocalTime(&mOperateTime);
	}
}

COneData_INFO::~COneData_INFO()
{
	Clear();
}

void COneData_INFO::Clear()
{
	ClearLevelList();
	ClearPressList();
	ClearQualityList();
	ClearFlowList();
	ClearRainList();
	ClearAnalogList();
	ClearPulseList();
}

void COneData_INFO::ClearLevelList()
{
	while (!m_lstWaterLevel.IsEmpty())
	{
		CWaterLevel_PAR_PER* pLevel = m_lstWaterLevel.RemoveHead();
		if (pLevel != NULL)
		{
			delete pLevel;
			pLevel = NULL;
		}
	}
}

void COneData_INFO::ClearPressList()
{
	while (!m_lstWaterPress.IsEmpty())
	{
		CWaterPressure_PARA_PER* pPress = m_lstWaterPress.RemoveHead();
		if (pPress != NULL)
		{
			delete pPress;
			pPress = NULL;
		}
	}
}

void COneData_INFO::ClearFlowList()
{
	while (!m_lstFlow.IsEmpty())
	{
		CFlowInitValume_PARA_PER* pFlow = m_lstFlow.RemoveHead();
		if (pFlow != NULL)
		{
			delete pFlow;
			pFlow = NULL;
		}
	}
}

void COneData_INFO::ClearQualityList()
{
	while (! m_lstWaterQuality.IsEmpty())
	{
		CWaterQuality_PARA_PER* pQuality = m_lstWaterQuality.RemoveHead();
		if (pQuality != NULL)
		{
			delete pQuality;
			pQuality = NULL;
		}
	}
}

void COneData_INFO::ClearRainList()
{
	while (! m_lstRain.IsEmpty())
	{
		CRainAlert_PARA_PER* pRain = m_lstRain.RemoveHead();
		if (pRain != NULL)
		{
			delete pRain;
			pRain = NULL;
		}
	}
}

void COneData_INFO::ClearAnalogList()
{
	while (! m_lstAnalog.IsEmpty())
	{
		CAnalogLimit_PARA_PER* pAnalog = m_lstAnalog.RemoveHead();
		if (pAnalog != NULL)
		{
			delete pAnalog;
			pAnalog = NULL;
		}
	}
}

void COneData_INFO::ClearPulseList()
{
	while (! m_lstPulses.IsEmpty())
	{
		CPulsePara_PER* pPulse = m_lstPulses.RemoveHead();
		if (pPulse)
		{
			delete pPulse;
			pPulse = NULL;
		}
	}
}

const CString COneData_INFO::Get_RTU_ID()
{
	CString strTemp = "";

	if (m_RtuID.IsEmpty())
	{
		strTemp.Format("%02d%02d%02d_%08d",
			mProvinceCode,
			mRegionCode,
			mCountyCode,
			mA2);

		m_RtuID = strTemp;
	}
	
	return m_RtuID;
}

const CString COneData_INFO::Get_CollectTime_String()
{
	CString strTemp = "";

	if (m_CollectTimeString.IsEmpty())
	{
		strTemp.Format("%04d-%02d-%02d %02d:%02d:%02d.%03d",
			mCollectTime.wYear,
			mCollectTime.wMonth,
			mCollectTime.wDay,
			mCollectTime.wHour,
			mCollectTime.wMinute,
			mCollectTime.wSecond,
			mCollectTime.wMilliseconds);
		m_CollectTimeString = strTemp;
	}	

	return m_CollectTimeString;
}

const CString COneData_INFO::Get_OperateTime_String()
{
	CString strTemp = "";
	if (m_OperateTimeString.IsEmpty())
	{
		strTemp.Format("%04d-%02d-%02d %02d:%02d:%02d.%03d",
			mOperateTime.wYear,
			mOperateTime.wMonth,
			mOperateTime.wDay,
			mOperateTime.wHour,
			mOperateTime.wMinute,
			mOperateTime.wSecond,
			mOperateTime.wMilliseconds);

		m_OperateTimeString = strTemp;
	}


	return m_OperateTimeString;
}

const CString COneData_INFO::Get_RecvTime_String()
{
	CString strTemp = "";

	if (m_RecvTimeString.IsEmpty())
	{
		strTemp.Format("%04d-%02d-%02d %02d:%02d:%02d.%03d",
			mRecvTime.wYear,
			mRecvTime.wMonth,
			mRecvTime.wDay,
			mRecvTime.wHour,
			mRecvTime.wMinute,
			mRecvTime.wSecond,
			mRecvTime.wMilliseconds);
		m_RecvTimeString = strTemp;
	}


	return m_RecvTimeString;
}



const CString COneData_INFO::Get_SYQ_RTU_ID()
{
	CString strTemp = "";

	if (m_RtuID.IsEmpty())
	{
		strTemp.Format("08d",mA2);

		m_RtuID = strTemp;
	}

	return m_RtuID;
}

const CString COneData_INFO::Get_SYQ_CollectTime_String()
{
	CString strTemp = "";
	SYSTEMTIME st;
	GetLocalTime(&st);

	if (m_CollectTimeString.IsEmpty())
	{
		strTemp.Format("%04d-%02d-%02d %02d:%02d",
			st.wYear,
			m_syqGetTime.month,
			m_syqGetTime.day,
			m_syqGetTime.hour,
			m_syqGetTime.min);
		m_CollectTimeString = strTemp;
	}	

	return m_CollectTimeString;
}

const CString COneData_INFO::Get_SYQ_RecvTime_String()
{
	CString strTemp = "";

	if (m_RecvTimeString.IsEmpty())
	{
		strTemp.Format("%04d-%02d-%02d %02d:%02d:%02d",
			m_syqSendTime.year,
			m_syqSendTime.month,
			m_syqSendTime.day,
			m_syqSendTime.hour,
			m_syqSendTime.min,
			m_syqSendTime.second);

		m_RecvTimeString = strTemp;
	}


	return m_RecvTimeString;
}

const CString COneData_INFO::Get_Alarm_PID()
{
	if (m_Alarm_PID.IsEmpty())
	{
		m_Alarm_PID.Format("%s_%04d%02d%02d%02d%02d%02d%03d",
			Get_RTU_ID(),		
			mCollectTime.wYear,
			mCollectTime.wMonth,
			mCollectTime.wDay,
			mCollectTime.wHour,
			mCollectTime.wMinute,
			mCollectTime.wSecond,
			mCollectTime.wMilliseconds
		);
	}

	return m_Alarm_PID;
}


//数据拷贝
void COneData_INFO::CopyLevelList(COneData_INFO* pSourceData)
{
	if (pSourceData != NULL)
	{
		POSITION rPos = pSourceData->m_lstWaterLevel.GetHeadPosition();
		while(rPos != NULL)
		{
			CWaterLevel_PAR_PER* pObj_InLst = pSourceData->m_lstWaterLevel.GetNext(rPos);

			//Copy data
			if (pObj_InLst != NULL)
			{
				CWaterLevel_PAR_PER* pNew = new CWaterLevel_PAR_PER(pObj_InLst);
				this->m_lstWaterLevel.AddTail(pNew);
			}			
		}
	}
}

void COneData_INFO::CopyPressList(COneData_INFO* pSourceData)
{
	if (pSourceData != NULL)
	{
		POSITION rPos = pSourceData->m_lstWaterPress.GetHeadPosition();
		while (rPos != NULL)
		{
			CWaterPressure_PARA_PER* pObj_InLst = pSourceData->m_lstWaterPress.GetNext(rPos);

			//Copy data
			if (pObj_InLst != NULL)
			{
				CWaterPressure_PARA_PER* pNew = new CWaterPressure_PARA_PER(pObj_InLst);
				this->m_lstWaterPress.AddTail(pNew);
			}
		}
	}
}

void COneData_INFO::CopyFlowList(COneData_INFO* pSourceData)
{
	if (pSourceData != NULL)
	{
		POSITION rPos = pSourceData->m_lstFlow.GetHeadPosition();
		while (rPos != NULL)
		{
			CFlowInitValume_PARA_PER* pObj_InLst = pSourceData->m_lstFlow.GetNext(rPos);

			//Copy data
			if (pObj_InLst != NULL)
			{
				CFlowInitValume_PARA_PER* pNew = new CFlowInitValume_PARA_PER(pObj_InLst);
				this->m_lstFlow.AddTail(pNew);
			}
		}
	}
}

void COneData_INFO::CopyQualityList(COneData_INFO* pSourceData)
{
	if (pSourceData != NULL)
	{
		POSITION rPos = pSourceData->m_lstWaterQuality.GetHeadPosition();
		while (rPos != NULL)
		{
			CWaterQuality_PARA_PER* pObj_InLst = pSourceData->m_lstWaterQuality.GetNext(rPos);

			//Copy data
			if (pObj_InLst != NULL)
			{
				CWaterQuality_PARA_PER* pNew =  new CWaterQuality_PARA_PER(pObj_InLst);
				this->m_lstWaterQuality.AddTail(pNew);
			}
		}
	}
}

void COneData_INFO::CopyRainList(COneData_INFO* pSourceData)
{
	if (pSourceData != NULL)
	{
		POSITION rPos = pSourceData->m_lstRain.GetHeadPosition();
		while (rPos != NULL)
		{
			CRainAlert_PARA_PER* pObj_InLst = pSourceData->m_lstRain.GetNext(rPos);

			//Copy data
			if (pObj_InLst != NULL)
			{
				CRainAlert_PARA_PER* pNew = new CRainAlert_PARA_PER(pObj_InLst);
				this->m_lstRain.AddTail(pNew);
			}
		}
	}
}

void COneData_INFO::CopyAnalogList(COneData_INFO* pSourceData)
{
	if (pSourceData != NULL)
	{
		POSITION rPos = pSourceData->m_lstAnalog.GetHeadPosition();
		while (rPos != NULL)
		{
			CAnalogLimit_PARA_PER* pObj_InLst = pSourceData->m_lstAnalog.GetNext(rPos);

			//Copy Data
			if (pObj_InLst != NULL)
			{
				CAnalogLimit_PARA_PER* pNew = new CAnalogLimit_PARA_PER(pObj_InLst);
				this->m_lstAnalog.AddTail(pNew);
			}
		}
	}
}

void COneData_INFO::CopyPulseList(COneData_INFO* pSourceData)
{
	if (pSourceData != NULL)
	{
		POSITION rPos = pSourceData->m_lstPulses.GetHeadPosition();
		while (rPos != NULL)
		{
			CPulsePara_PER* pObj_InLst = pSourceData->m_lstPulses.GetNext(rPos);

			//Copy Data
			if (pObj_InLst != NULL)
			{
				CPulsePara_PER* pNew = new CPulsePara_PER(pObj_InLst);
				this->m_lstPulses.AddTail(pNew);
			}
		}
	}
}

CUdfRtuDataSave_Server::CUdfRtuDataSave_Server(void)
{
	m_strDatasource = "";

	m_pWndLogdlg = NULL;
	m_str_LastMessage_LogDlg = "";
	m_str_LastMessage_LogSys = "";

	m_pDataTranObject = NULL;

}

CUdfRtuDataSave_Server::CUdfRtuDataSave_Server(CString strDataSource)
{
	m_strDatasource = strDataSource;

	m_pWndLogdlg = NULL;
	m_str_LastMessage_LogDlg = "";
	m_str_LastMessage_LogSys = "";

	m_pDataTranObject = NULL;

}

CUdfRtuDataSave_Server::~CUdfRtuDataSave_Server(void)
{
	//停止所以运行的线程
	Clear();


	if (m_pDataTranObject != NULL)
	{
		_DELETE(m_pDataTranObject);
	}
}

bool CUdfRtuDataSave_Server::Initialization()
{	
	//Start Thread 启动数据处理线程模块
    StartThread();
	return true;
}

void  CUdfRtuDataSave_Server::StartThread()
{
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();

	CDataTran_Base* pThreadModule = NULL;

	if(pApp->m_GlobalOptions.m_DataInterface == DATAINTERFACE_GM)
	{
		pThreadModule = new CDataTranSQLServer_GM();
		pThreadModule->SetParent(this);
		pThreadModule->SetDataSource(m_strDatasource);
		AddDataTranThread(pThreadModule);
	}
	else if (pApp->m_GlobalOptions.m_DataInterface == DATAINTERFACE_SC)
	{
		//pThreadModule = new CDataTranSQLServer_SC();
		//pThreadModule->SetParent(this);
		//pThreadModule->SetDataSource(m_strDatasource);
		//AddDataTranThread(pThreadModule);

		m_pDataTranObject = new CDataTranSQLServer_SC();
		m_pDataTranObject->SetParent(this);
		m_pDataTranObject->SetDataSource(m_strDatasource);
		((CDataTranSQLServer_SC*)m_pDataTranObject)->Initialization();

	}
	else if (pApp->m_GlobalOptions.m_DataInterface == DATAINTERFACE_YN)//BY zwb
	{
		//pThreadModule = new CDataTranSQLServer_SC();
		//pThreadModule->SetParent(this);
		//pThreadModule->SetDataSource(m_strDatasource);
		//AddDataTranThread(pThreadModule);

		m_pDataTranObject = new CDataTranSQLServer_SC();
		m_pDataTranObject->SetParent(this);
		m_pDataTranObject->SetDataSource(m_strDatasource);
		((CDataTranSQLServer_SC*)m_pDataTranObject)->Initialization();

	}
	


	pThreadModule = new CDataTranSQLite_Local();
	pThreadModule->SetParent(this);
	pThreadModule->SetDataSource(pApp->m_GlobalOptions.m_strLocalDbName);
	AddDataTranThread(pThreadModule);
}

unsigned long CUdfRtuDataSave_Server::ThreadFunction(void* pPARAM)
{
	CDataTran_Base* pModule = (CDataTran_Base*)pPARAM;
	if (pModule != NULL)
		return pModule->ThreadFunction();

	return 0;
}

void CUdfRtuDataSave_Server::SetDataSource(CString _dataSource) 
{
	m_strDatasource = _dataSource;
}

void CUdfRtuDataSave_Server::SetDataSource(CString _Dsn,CString _Uid,CString _Pwd)
{
	CString strDsn= _T("");
	strDsn.Format("DSN=%s;UID=%s;PWD=%s;",_Dsn,_Uid,_Pwd);
	SetDataSource(strDsn);
}

void CUdfRtuDataSave_Server::AddLog_To_LogSystem(_LOG_MESSAGE_TYPE LogType,
												 CString LogMsg,
												 _Log_level_type LogLevel /* = Normal_Log_Message */,
												 PSYSTEMTIME pTIME /* = NULL */)
{
	if (m_str_LastMessage_LogSys == LogMsg)
		return;

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	if (pApp->m_pLogSystem != NULL)
		pApp->m_pLogSystem->InsertLogMessage(LogType,LogMsg,LogLevel,pTIME);
	m_str_LastMessage_LogSys = LogMsg;

}

void CUdfRtuDataSave_Server::AddLog_To_LogFrame(CString message,
												_Log_level_type xType /* = Normal_Log_Message */,
												PSYSTEMTIME st/* =NULL */)
{
		if (m_str_LastMessage_LogDlg != message)
		{	
			m_cs_logframe.Lock();
			CUdfDataInDbLogDlg* pWnd = GetDataInDbLogWnd();
			if (pWnd && pWnd->GetSafeHwnd())
			{
				pWnd->AddOneMessage(message,st,xType);
			}
			m_cs_logframe.Unlock();
			m_str_LastMessage_LogDlg = message;
		}
}

void CUdfRtuDataSave_Server::InsertOneData(COneData_INFO* pData)
{
	if (pData != NULL)
	{
		m_cs_DataTranThreads.Lock();
		if (!m_lstDataTranThreads.IsEmpty())
		{
			POSITION rPos = m_lstDataTranThreads.GetHeadPosition();
			while (rPos != NULL)
			{
				CDataTran_Base* pThreadModule = m_lstDataTranThreads.GetNext(rPos);
				if (pThreadModule != NULL)
				{
					COneData_INFO* pCopyData = new COneData_INFO(pData);
					pThreadModule->AddOneData(pCopyData);
				}
			}
		}
		m_cs_DataTranThreads.Unlock();
		delete pData;
		pData = NULL;
	}
}

bool CUdfRtuDataSave_Server::CreateDataTranThread( CDataTran_Base* pThreadModule )
{
	if (!pThreadModule) return false;

	//thread is Running
	if (pThreadModule->GetThreadHandle() != NULL && 
		pThreadModule->GetThreadID() != 0 &&
		pThreadModule->GetThreadState() == THREAD_STATE_RUNNING)
		return true;

	//thread is suspend
	if (pThreadModule->GetThreadHandle() != NULL && 
		pThreadModule->GetThreadID() != 0 &&
		pThreadModule->GetThreadState() == THREAD_STATE_PAUSE)
		return ResumeDataTranThread(pThreadModule);

	//Create New thread
	pThreadModule->SetThreadControl(true);
	DWORD threadID = 0;
	HANDLE threadHandle = ::CreateThread(NULL,0,ThreadFunction,pThreadModule,0,&threadID);
	if (threadHandle == NULL)
		return false;

	pThreadModule->SetThreadHandle(threadHandle);
	pThreadModule->SetThreadID(threadID);

	return false;
}

bool CUdfRtuDataSave_Server::AddDataTranThread( CDataTran_Base* pThreadModule )
{
	if (!pThreadModule) return false;

	if (CreateDataTranThread(pThreadModule))
	{
		CWaitTimer waittimer;
		waittimer.SetWaitTimer(3000);   //wait 3s

		while(pThreadModule->GetThreadState() != THREAD_STATE_RUNNING)
		{
			if (waittimer.WaitTimerState(100))
				break;

			Sleep(0);
		}
	}

	POSITION rPos =NULL;
	m_cs_DataTranThreads.Lock();
	rPos = m_lstDataTranThreads.AddTail(pThreadModule);
	m_cs_DataTranThreads.Unlock();

	if (rPos != NULL)
		return true;
   
	return false;
}

bool CUdfRtuDataSave_Server::ResumeDataTranThread( CDataTran_Base* pThreadModule )
{
	if (!pThreadModule) return false;
	if (pThreadModule->GetThreadHandle() != NULL && 
		pThreadModule->GetThreadID() != 0 &&
		pThreadModule->GetThreadState() == THREAD_STATE_RUNNING)
		return true;

	if (pThreadModule->GetThreadHandle() != NULL && 
		pThreadModule->GetThreadID() != 0 && 
		pThreadModule->GetThreadState() == THREAD_STATE_PAUSE)
	{
		DWORD ret = ::ResumeThread(pThreadModule->GetThreadHandle());
		if(ret == 0xFFFFFFFF)
			return false;
		else
		{
			pThreadModule->SetThreadState(THREAD_STATE_RUNNING);
			return true;
		}
	}

	return false;
}


bool CUdfRtuDataSave_Server::StartDataTranThread( CDataTran_Base* pThreadModule )
{
	if (!pThreadModule) return false;

	if (pThreadModule->GetThreadHandle() != NULL &&
		pThreadModule->GetThreadID() != 0 && 
		pThreadModule->GetThreadState() == THREAD_STATE_RUNNING)
		return true;

	if (pThreadModule->GetThreadState() != NULL && 
		pThreadModule->GetThreadID() != 0 && 
		pThreadModule->GetThreadState() == THREAD_STATE_PAUSE)
		return ResumeDataTranThread(pThreadModule);

	bool ret = CreateDataTranThread(pThreadModule);
	if (ret == true)
	{
		CWaitTimer waiterTimer;
		waiterTimer.SetWaitTimer(3000);   //wait 3s

		while (pThreadModule->GetThreadState() != THREAD_STATE_RUNNING)
		{
			if (waiterTimer.WaitTimerState(100) == true)
				break;

			Sleep(0);
		}

		if (pThreadModule->GetThreadState() == THREAD_STATE_RUNNING)
			return true;
	}

	return false;
}

bool CUdfRtuDataSave_Server::StopDataTranThread( CDataTran_Base* pThreadModule )
{
	if (!pThreadModule) return false;

	if (pThreadModule->GetThreadHandle() == NULL ||
		pThreadModule->GetThreadID() == 0 ||
		pThreadModule->GetThreadState() != THREAD_STATE_RUNNING)
		return true;

	CWaitTimer waittimer;
	waittimer.SetWaitTimer(3000);

	pThreadModule->SetThreadControl(false);

	while (pThreadModule->GetThreadState() == THREAD_STATE_RUNNING)
	{
		if (waittimer.WaitTimerState(100) == true)
			break;

		Sleep(0);
	}

	if (pThreadModule->GetThreadState() == THREAD_STATE_STOP)
	{
		pThreadModule->ResetThread();
		
		return true;
	}

	return false;
}

bool CUdfRtuDataSave_Server::DeleteDataTranThread( CDataTran_Base* pThreadModule )
{
	if (!pThreadModule)	return false;
	if (!StopDataTranThread(pThreadModule)) return false;


	//Delete from list
	POSITION rPos =NULL,FindPos = NULL;
	bool blFinded =false;
	m_cs_DataTranThreads.Lock();
	rPos = m_lstDataTranThreads.GetHeadPosition();
	while (rPos != NULL)
	{
		CDataTran_Base* pObj = m_lstDataTranThreads.GetAt(rPos);
		if (pObj && pObj == pThreadModule)
		{
			blFinded = true;
			FindPos = rPos;
			break;
		}
		m_lstDataTranThreads.GetNext(rPos);
	}
	m_cs_DataTranThreads.Unlock();

	if (blFinded == true && FindPos != NULL )
	{
		m_cs_DataTranThreads.Lock();
		m_lstDataTranThreads.RemoveAt(FindPos);
		m_cs_DataTranThreads.Unlock();

		delete pThreadModule;
		pThreadModule = NULL;

		return true;
	}

	return false;
}

void CUdfRtuDataSave_Server::Clear()
{
	//Stop thread and delete thread's object
	while (!m_lstDataTranThreads.IsEmpty())
	{
		CDataTran_Base* pThreadModule = m_lstDataTranThreads.RemoveHead();

		if (pThreadModule != NULL)
		{
			StopDataTranThread(pThreadModule);

			//wait for thread stopped
			while (pThreadModule->GetThreadState()  != THREAD_STATE_STOP && 
				   pThreadModule->GetThreadHandle() != NULL)
			{
				Sleep(0);
			}

			delete pThreadModule;
			pThreadModule = NULL;
		}
	}
}

void CUdfRtuDataSave_Server::StatisticsData(CDbSQLite* sqlite,COneData_INFO* pData/*=NULL*/ )
{
	if (m_pDataTranObject != NULL)
	{
		if(m_pDataTranObject->GetDataTran_ThreadType() == DataTran_SQLServer_SC)
		{
			CDataTranSQLServer_SC* pObj = (CDataTranSQLServer_SC*)m_pDataTranObject;
			pObj->SetSqlite(sqlite);

			pObj->StatisticsRain_1Day_OnTimer();

			if (pData != NULL)
				pObj->Statistics_Data_OnRealTime(pData);

		}
		else if(m_pDataTranObject->GetDataTran_ThreadType() == DataTran_SQLServer_YN)//ZWB
		{
			CDataTranSQLServer_YN* pObj = (CDataTranSQLServer_YN*)m_pDataTranObject;

			pObj->SetSqlite(sqlite);

			pObj->StatisticsRain_1Day_OnTimer();

			if (pData != NULL)
				pObj->Statistics_Data_OnRealTime(pData);
		}


	}
}



