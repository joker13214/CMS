#include "StdAfx.h"
#include "DataTranSQLite_Local.h"
#include "UdfRtuDataSave.h"
#include "SystemTimeOp.h"
#include "CMS.h"

CDataTranSQLite_Local::CDataTranSQLite_Local()
{
	m_ThreadType = DataTran_SQLiteLocal;
	m_nSaveMonth = 6/*1*/;
	m_strDataSource = "";
	m_bNeedData = true;
	m_dwPreBeginTcTransaction = 0;
	m_dwLocalSaveInterval = GETAPP()->m_GlobalOptions.m_nLocalSQLiteSaveInterval;
	
	m_blStartTransaction = false;

}

CDataTranSQLite_Local::~CDataTranSQLite_Local()
{
	//线程已经停止
	//存储没有存储的数据信息到SQLite
	SQLiteBeginTransaction(); //20140901 启动数据库事务
	while (!m_lstData.IsEmpty())
	{
		COneData_INFO* pData = m_lstData.RemoveHead();
		SaveDataToSQLite(pData);
	}
	SQLiteCommitTransaction(); //20140901 提交数据库事务
}

void CDataTranSQLite_Local::Initialization()
{
	InitSQLiteDataBase();
	DeleteDataOnTimer();
}


long CDataTranSQLite_Local::ThreadFunction()
{

	SetThreadControl(true);
	SetThreadState(THREAD_STATE_RUNNING);

	//InitSQLiteDataBase();
	//DeleteDataOnTimer();

	DWORD preStartTickCount= 0;
	bool  bRuningDeleted = false; //定时数据删除标志
	while (GetThreadControl() != false)
	{
		//实时存储采集数据
		COneData_INFO* pData = GetOneData();
		COneData_INFO* pCopyData = NULL;
		bool blNull = true;
		if (pData != NULL)
		{
			//检查是否启动事务
			if (m_blStartTransaction == false)
			{
				SQLiteBeginTransaction();			//启动数据库事务
				preStartTickCount = GetTickCount();
			}
			
			blNull = false;
			if (pData->GetErrorFlag() == false)		//异常数据不进行统计（不拷贝至统计中心）
				pCopyData = new COneData_INFO(pData);

			SaveDataToSQLite(pData);
		}


		//定期提交数据库事务
		if (m_blStartTransaction == true && GetTickCount() - preStartTickCount >= m_dwLocalSaveInterval)
		{
			SQLiteCommitTransaction();
		}


		if (m_pParent != NULL )
		{
			if (pCopyData != NULL)
				m_pParent->StatisticsData(&m_sqlite,pCopyData);

			//统计是否有报文挤压
			int waitbuffercount = GetDataCount();
			SYSTEMTIME colltime,recvtime;
			CSystemTimeOp::ResetSystemTime(recvtime);
			CSystemTimeOp::ResetSystemTime(colltime);
			if (pCopyData != NULL)
			{
				colltime = pCopyData->mCollectTime;
				recvtime = pCopyData->mRecvTime;
			}
			m_pParent->CurStatisticsState(waitbuffercount,colltime,recvtime);

			//其他定时任务
			m_pParent->LocalOnTimerFunction(m_blStartTransaction);
		}
		
		//定期删除之前的数据,
		//默认只保存最新6个月的数据
		SYSTEMTIME nowTime;
		GetLocalTime(&nowTime);
		if (nowTime.wDay == 1 && bRuningDeleted == false)
		{
			DeleteDataOnTimer();
			bRuningDeleted = true;
		}
		if (nowTime.wDay != 1 && bRuningDeleted == true)
		{
			bRuningDeleted = false;
		}

		if (blNull == true)
			Sleep(1);
		else if (blNull == false)
			Sleep(0);
	}
	//退出线程
	ResetThread();

	//如果已经启动事务，在退出线程时需要提交事务
	if (m_blStartTransaction == true)
		SQLiteCommitTransaction();

	return 0;
}

bool CDataTranSQLite_Local::OpenSqlite()
{
	CHAR OLD_PATH[MAX_PATH];
	memset(OLD_PATH,0,sizeof(OLD_PATH));
	::GetCurrentDirectory(MAX_PATH,OLD_PATH);
	::SetCurrentDirectory(CUdfGlobalFUNC::GetRuningPath());

	bool ret = m_sqlite.Open((LPCTSTR)m_strDataSource);	
	::SetCurrentDirectory(OLD_PATH);

	return ret;
}

void CDataTranSQLite_Local::InitSQLiteDataBase()
{
	if (OpenSqlite() == true)
	{
		PragmaSQLite();
		CheckSQLiteDatabase();
	}
}

bool CDataTranSQLite_Local::SaveDataToSQLite( COneData_INFO* pData )
{
	bool ret = true;
	if (pData != NULL)  
	{
		switch (pData->mDataType)
		{
		case COneData_INFO::OPERATE_RECORD:
			ret = SaveOperateRecord(pData);
			break;
		case COneData_INFO::WATER_L_DATA:
			ret = SaveWaterLevelData(pData);
			break;	
		case COneData_INFO::WATER_P_DATA:	
			ret = SaveWaterPressData(pData);
			break;
		case COneData_INFO::FLOW_V_DATA:		
			ret = SaveWaterFlowData(pData);
			break;
		case COneData_INFO::WATER_Q_DATA:
			ret = SaveWaterQualityData(pData);
			break;	
		case COneData_INFO::RAIN_DATA:		
			ret = SaveRainData(pData);
			break;
		case COneData_INFO::ANALOG_DATA:
			ret = SaveAnalogData(pData);
			break;		
		case COneData_INFO::PULSE_DATA:
			ret = SavePulseData(pData);
			break;
		case COneData_INFO::ALARM_RECORD:  
			ret = SaveAlarmRecord(pData);
			break;
		case  COneData_INFO::PEACE_REPORT:
			ret = SavePeaceReport(pData);
			break;
		case COneData_INFO::DISPLACE_DATA:
			ret = SaveDisplaceData(pData);
			break;
		case COneData_INFO::TEMPERA_DATA:
			ret = SaveTemperaData(pData);
			break;
		}

		//删除pData
		delete pData;
		pData = NULL;
	}
	return ret;
}

bool CDataTranSQLite_Local::IsExistTable( CString strTable )
{
	int nCount = 0;
	try
	{
		CString strSQL = "";
		strSQL.Format("SELECT COUNT(*) as nCount FROM sqlite_master WHERE type='table' AND NAME='%s'",strTable);

		CSqlStatement* stmt = m_sqlite.Statement(strSQL);
		
		if (stmt != NULL)
		{
			if (stmt->NextRow())
			{
				CString szText = stmt->ValueString(0);
				nCount = atoi(szText);
			}

			delete stmt;
			stmt = NULL;
		}	
	}
	catch (CException* e)
	{
		e->Delete();
		return false;
	}
	if(nCount > 0)
		return true;

	return false;
}

bool CDataTranSQLite_Local::SaveWaterLevelData( COneData_INFO* pData )
{
	if (!pData) return false;

	CString strSQL="";
	bool blResult = true;
	try
	{
		int nSensorsNo = 0;
		while (!pData->m_lstWaterLevel.IsEmpty())
		{
			CWaterLevel_PAR_PER* pLevel = pData->m_lstWaterLevel.RemoveHead();
			if (pLevel)
			{
				if (pData->GetErrorFlag() == false)
				{
					strSQL.Format("INSERT INTO WATERL_DATA (RTU,SENSORS_NO,CURRENT_VAL,COLLECT_TIME,SAVE_TIME) VALUES('%s','%02d','%.02f','%s','%s')",
						pData->Get_RTU_ID(),
						nSensorsNo,
						pLevel->GetRealValue(),
						pData->Get_CollectTime_String(),
						pData->Get_RecvTime_String());
				}
				else if (pData->GetErrorFlag() == true)
				{
					strSQL.Format("INSERT INTO WATERL_DATA_ERR (RTU,SENSORS_NO,CURRENT_VAL,COLLECT_TIME,SAVE_TIME) VALUES('%s','%02d','%.02f','%s','%s')",
						pData->Get_RTU_ID(),
						nSensorsNo,
						pLevel->GetRealValue(),
						pData->Get_CollectTime_String(),
						pData->Get_RecvTime_String());
				}



				ExcuteNoQuery(strSQL);

				delete pLevel;
				pLevel = NULL;
				nSensorsNo ++;
			}
		}
	}
	catch (CException* pe)
	{
		blResult = false;
		pe->Delete();	
	}

	return blResult;
}

bool CDataTranSQLite_Local::SaveWaterPressData( COneData_INFO* pData )
{
	if (!pData) return false;

	CString strSQL="";
	bool blResult = true;
	try
	{
		int nSensorsNo = 0;
		while ( !pData->m_lstWaterPress.IsEmpty())
		{
			CWaterPressure_PARA_PER* pPress = pData->m_lstWaterPress.RemoveHead();
			if (pPress)
			{
				strSQL.Format("INSERT INTO WATERP_DATA (RTU,SENSORS_NO,CURRENT_VAL,COLLECT_TIME,SAVE_TIME) VALUES('%s','%02d','%.02f','%s','%s')",
					pData->Get_RTU_ID(),
					nSensorsNo,
					pPress->GetRealValue(),
					pData->Get_CollectTime_String(),
					pData->Get_RecvTime_String());

				ExcuteNoQuery(strSQL);

				nSensorsNo ++;
				delete pPress;
				pPress = NULL;
			}
		}
	}
	catch (CException* pe)
	{
		blResult= false;
		pe->Delete();
	}

	return blResult;
}

bool CDataTranSQLite_Local::SaveWaterQualityData( COneData_INFO* pData )
{
	if (!pData) return false;

	CString strSQL="";
	bool blResult = true;
	try
	{
		while (! pData->m_lstWaterQuality.IsEmpty())
		{
			CWaterQuality_PARA_PER* pQuality = pData->m_lstWaterQuality.RemoveHead();
			if (pQuality != NULL)
			{
				strSQL.Format("INSERT INTO WATERQ_DATA (RTU,SPECIES,CURRENT_VAL,COLLECT_TIME,SAVE_TIME) VALUES('%s','D%d','%.*f','%s','%s')",
					pData->Get_RTU_ID(),
					pQuality->m_BitPos,
					pQuality->m_PointBits,
					pQuality->m_CurValue,
					pData->Get_CollectTime_String(),
					pData->Get_RecvTime_String());

				ExcuteNoQuery(strSQL);

				delete pQuality;
				pQuality = NULL;
			}
		}
	}
	catch (CException* pe)
	{
		blResult= false;
		pe->Delete();
	}

	return blResult;
}

bool CDataTranSQLite_Local::SaveWaterFlowData( COneData_INFO* pData )
{
	if (!pData) return false;

	CString strSQL="";
	bool blResult = true;
	try
	{
		int nSensorsNo = 0;
		while (! pData->m_lstFlow.IsEmpty())
		{
			CFlowInitValume_PARA_PER* pFLow = pData->m_lstFlow.RemoveHead();
			if (pFLow)
			{
				strSQL.Format("INSERT INTO FLOWV_DATA (RTU,SENSORS_NO,TOTAL_WATER,ANYTIME_FLOW,COLLECT_TIME,SAVE_TIME) VALUES('%s','%02d',%d,%d,'%s','%s')",
					pData->Get_RTU_ID(),
					nSensorsNo,
					pFLow->GetCalTakeValume(),
					pFLow->GetInsFlowValume(),
					pData->Get_CollectTime_String(),
					pData->Get_RecvTime_String());

				ExcuteNoQuery(strSQL);

				nSensorsNo ++ ;
				delete pFLow;
				pFLow = NULL;
			}
		}
	}
	catch (CException* pe)
	{
		blResult= false;
		pe->Delete();
	}

	return blResult;
}

bool CDataTranSQLite_Local::SaveRainData( COneData_INFO* pData )
{
	if (!pData) return false;

	CString strSQL="";
	bool blResult = true;
	try
	{
		int nSensorsNo = 0;
		while (! pData->m_lstRain.IsEmpty())
		{
			CRainAlert_PARA_PER* pRain = pData->m_lstRain.RemoveHead();
			if (pRain)
			{

				if (pData->GetErrorFlag() == false)
				{
					strSQL.Format("INSERT INTO RAIN_DATA (RTU,SENSORS_NO,DATA_TYPE,CURRENT_VAL,HALFH_VAL,TOTAL_VAL,COLLECT_TIME,SAVE_TIME) VALUES ('%s','%02d','%02d','%.1f','%.1f','%.1f','%s','%s')",
						pData->Get_RTU_ID(),
						nSensorsNo,
						pRain->GetDataType(),
						pRain->GetRealTimeValue(),
						pRain->GetHalfHourValue(),
						pRain->GetTotalValue(),
						pData->Get_CollectTime_String(),
						pData->Get_RecvTime_String());
				}
				else if (pData->GetErrorFlag() == true)
				{
					strSQL.Format("INSERT INTO RAIN_DATA_ERR (RTU,SENSORS_NO,DATA_TYPE,CURRENT_VAL,HALFH_VAL,TOTAL_VAL,COLLECT_TIME,SAVE_TIME) VALUES ('%s','%02d','%02d','%.1f','%.1f','%.1f','%s','%s')",
						pData->Get_RTU_ID(),
						nSensorsNo,
						pRain->GetDataType(),
						pRain->GetRealTimeValue(),
						pRain->GetHalfHourValue(),
						pRain->GetTotalValue(),
						pData->Get_CollectTime_String(),
						pData->Get_RecvTime_String());
				}



				ExcuteNoQuery(strSQL);

				nSensorsNo ++;
				delete pRain;
				pRain = NULL;
			}
		}
	}
	catch (CException* pe)
	{
		blResult= false;
		pe->Delete();
	}

	return blResult;
}

bool CDataTranSQLite_Local::SaveOperateRecord( COneData_INFO* pData )
{
	if (!pData) return false;

	CString strSQL="";
	bool blResult = true;
	try
	{
		strSQL.Format("INSERT INTO OPERATE_RECORD (RTU,OPERATE_ACCOUNT,OPERATE_TIME,OPERATE_DESC) VALUES ('%s','%s','%s','%s')",
			pData->Get_RTU_ID(),
			pData->mOperateAccount,
			pData->Get_OperateTime_String(),
			pData->mOperateDESC);

		ExcuteNoQuery(strSQL);
	}
	catch (CException* pe)
	{
		blResult= false;
		pe->Delete();
	}

	return blResult;
}

bool CDataTranSQLite_Local::SaveAnalogData( COneData_INFO* pData )
{
	if (!pData) return false;

	CString strSQL="";
	bool blResult = true;
	try
	{
		int nAnalogNo = 0;
		while (! pData->m_lstAnalog.IsEmpty())
		{
			CAnalogLimit_PARA_PER* pAnalog = pData->m_lstAnalog.RemoveHead();
			if (pAnalog)
			{
				strSQL.Format("INSERT INTO ANALOG_DATA (RTU,ANALOG_NO,CURRENT_VAL,COLLECT_TIME,SAVE_TIME) VALUES ('%s','%02d','%d','%s','%s')",
					pData->Get_RTU_ID(),
					nAnalogNo,
					pAnalog->GetRealTimeValue(),
					pData->Get_CollectTime_String(),
					pData->Get_RecvTime_String());

				ExcuteNoQuery(strSQL);

				delete pAnalog;
				pAnalog = NULL;
				nAnalogNo ++;
			}
		}
	}
	catch (CException* pe)
	{
		blResult= false;
		pe->Delete();
	}

	return blResult;
}

bool CDataTranSQLite_Local::SaveDisplaceData( COneData_INFO* pData )
{
	if (!pData) return false;

	CString strSQL="";
	bool blResult = true;
	try
	{
		int nDisplaceNo = 1;
		while (! pData->m_lstDisplace.IsEmpty())
		{
			CDisplace_PARA_PER* pDisplace = pData->m_lstDisplace.RemoveHead();
			if (pDisplace)
			{
				strSQL.Format("INSERT INTO DISPLACE_DATA (RTU,SENSORS_NO,CURRENT_XVAL,CURRENT_YVAL,COLLECT_TIME,SAVE_TIME) VALUES ('%s','%02d','%f','%f','%s','%s')",
					pData->Get_RTU_ID(),
					nDisplaceNo,
					pDisplace->GetRealTimeXValue(),
					pDisplace->GetRealTimeYValue(),
					pData->Get_CollectTime_String(),
					pData->Get_RecvTime_String());

				ExcuteNoQuery(strSQL);

				delete pDisplace;
				pDisplace = NULL;
				nDisplaceNo ++;
			}
		}
	}
	catch (CException* pe)
	{
		blResult= false;
		pe->Delete();
	}

	return blResult;
}

bool CDataTranSQLite_Local::SaveTemperaData( COneData_INFO* pData )
{
	if (!pData) return false;

	CString strSQL="";
	bool blResult = true;
	try
	{
		int nTemperaNo = 0;
		while (! pData->m_lstTempera.IsEmpty())
		{
			CTempera_PARA_PER* pTempera = pData->m_lstTempera.RemoveHead();
			if (pTempera)
			{
				strSQL.Format("INSERT INTO TEMPERA_DATA (RTU,SENSORS_NO,CURRENT_VAL,COLLECT_TIME,SAVE_TIME) VALUES ('%s','%02d','%f','%s','%s')",
					pData->Get_RTU_ID(),
					nTemperaNo,
					pTempera->GetRealTimeValue(),
					pData->Get_CollectTime_String(),
					pData->Get_RecvTime_String());

				ExcuteNoQuery(strSQL);

				delete pTempera;
				pTempera = NULL;
				nTemperaNo ++;
			}
		}
	}
	catch (CException* pe)
	{
		blResult= false;
		pe->Delete();
	}

	return blResult;
}


bool CDataTranSQLite_Local::SavePulseData( COneData_INFO* pData )
{
	if (!pData) return false;

	CString strSQL="";
	bool blResult = true;
	try
	{
		while (! pData->m_lstPulses.IsEmpty())
		{
			CPulsePara_PER* pPulse = pData->m_lstPulses.RemoveHead();
			if (pPulse)
			{	
				strSQL.Format("INSERT INTO PULSE_DATA (RTU,CODE,VALUE1,VALUE2,VALUE3,VALUE4,COLLECT_TIME,SAVE_TIME) VALUES ('");

				CString strTemp = "";
				strTemp.Format("%s',",pData->Get_RTU_ID());
				strSQL += strTemp;

				strTemp.Format("'%s',",pPulse->GetCodeString());
				strSQL += strTemp;

				strTemp.Format("'%.2f',",pPulse->GetValue1());
				strSQL += strTemp;

				strTemp.Format("'%.2f',",pPulse->GetValue2());
				strSQL += strTemp;

				strTemp.Format("'%.2f',",pPulse->GetValue3());
				strSQL += strTemp;

				strTemp.Format("'%.2f',",pPulse->GetValue4());
				strSQL += strTemp;


				strTemp.Format("'%s',",pData->Get_CollectTime_String());
				strSQL += strTemp;

				strTemp.Format("'%s')",pData->Get_RecvTime_String());
				strSQL += strTemp;

				ExcuteNoQuery(strSQL);

				delete pPulse;
				pPulse = NULL;
			}
		}
	}
	catch (CException* pe)
	{
		blResult= false;
		pe->Delete();
	}

	return blResult;
}

bool CDataTranSQLite_Local::SaveAlarmRecord( COneData_INFO* pData )
{
	CString strSQL = "";
	bool blResult = true;
	if (pData != NULL)
	{
		//存储报警信息到ALARM_RECORD表

		try
		{
			strSQL.Format("INSERT INTO ALARM_RECORD (P_ID,RTU,W_TYPE,WARNING_TIME,SAVE_TIME) VALUES ('%s','%s',%d,'%s','%s')",
				pData->Get_Alarm_PID(),
				pData->Get_RTU_ID(),
				pData->mAlarmType,
				pData->Get_CollectTime_String(),
				pData->Get_RecvTime_String());

			ExcuteNoQuery(strSQL);
		}
		catch (CException* pe)
		{
			blResult = false;
			pe->Delete();
		}


		//存储响应报警的附加信息
		bool blSaveOther = true;
		if (pData->mAlarmType == (int)COneData_INFO::ALARM_WATERLEVEL)
		{
			blSaveOther = Save_Alarm_WaterLevel(pData);
		}
		else if (pData->mAlarmType == (int)COneData_INFO::ALARM_WATERPRESS)
		{
			blSaveOther = Save_Alarm_WaterPress(pData);
		}
		else if (pData->mAlarmType == (int)COneData_INFO::ALARM_WATERQUALITY)
		{
			blSaveOther = Save_Alarm_WaterQuality(pData);
		}
		else if (pData->mAlarmType == (int)COneData_INFO::ALARM_FLOW)
		{
			blSaveOther = Save_Alarm_Flow(pData);
		}
		else if (pData->mAlarmType == (int)COneData_INFO::ALARM_RAIN)
		{
			blSaveOther = Save_Alarm_Rain(pData);
		}
		else if (pData->mAlarmType == (int)COneData_INFO::ALARM_ANALOG)
		{
			blSaveOther = Save_Alarm_Analog(pData);
		}
		else if (pData->mAlarmType == (int)COneData_INFO::ALARM_DISPLACE)
		{
			blSaveOther = Save_Alarm_Displace(pData);
		}
		else if (pData->mAlarmType == (int)COneData_INFO::ALARM_TEMPERA)
		{
			blSaveOther = Save_Alarm_Tempera(pData);
		}

		if (blResult==false || blSaveOther == false) 
			blResult = false;
		else 
			blResult = true;
	}
	return blResult;
}

bool CDataTranSQLite_Local::Save_Alarm_WaterLevel( COneData_INFO* pData )
{
	CString strSQL = "";
	bool blResult = true;
	try
	{
		int nSensorsNo = 0;
		while (!pData->m_lstWaterLevel.IsEmpty())
		{
			CWaterLevel_PAR_PER* pLevel = pData->m_lstWaterLevel.RemoveHead();
			if (pLevel)
			{
				strSQL.Format("INSERT INTO ALARM_WL (WARNING_ID,SENSORS_NO,CURRENT_VAL) VALUES ('%s','%02d','%.02f')",
					pData->Get_Alarm_PID(),
					nSensorsNo,
					pLevel->GetRealValue());

				ExcuteNoQuery(strSQL);

				strSQL.Format("INSERT INTO WATERL_DATA (RTU,SENSORS_NO,CURRENT_VAL,COLLECT_TIME,SAVE_TIME) VALUES('%s','%02d','%.02f','%s','%s')",
					pData->Get_RTU_ID(),
					nSensorsNo,
					pLevel->GetRealValue(),
					pData->Get_CollectTime_String(),
					pData->Get_RecvTime_String());
				ExcuteNoQuery(strSQL);

				delete pLevel;
				pLevel = NULL;
				nSensorsNo ++;
			}
		}
	}
	catch (CException* pe)
	{
		blResult = false;
		pe->Delete();
	}

	return blResult;
}

bool CDataTranSQLite_Local::Save_Alarm_WaterPress( COneData_INFO* pData )
{
	CString strSQL = "";
	bool blResult = true;
	try
	{
		int nSensorsNo = 0;
		while (!pData->m_lstWaterPress.IsEmpty())
		{
			CWaterPressure_PARA_PER* pPress = pData->m_lstWaterPress.RemoveHead();
			if (pPress)
			{
				strSQL.Format("INSERT INTO ALARM_WP (WARNING_ID,SENSORS_NO,CURRENT_VAL) VALUES ('%s','%02d','%.02f')",
					pData->Get_Alarm_PID(),
					nSensorsNo,
					pPress->GetRealValue());

				ExcuteNoQuery(strSQL);

				delete pPress;
				pPress = NULL;
				nSensorsNo ++ ;
			}
		}
	}
	catch (CException* pe)
	{
		blResult = false;
		pe->Delete();
	}

	return blResult;
}

bool CDataTranSQLite_Local::Save_Alarm_WaterQuality( COneData_INFO* pData )
{
	CString strSQL = "";
	bool blResult = true;

	try
	{
		while (!pData->m_lstWaterQuality.IsEmpty())
		{
			CWaterQuality_PARA_PER* pQuality = pData->m_lstWaterQuality.RemoveHead();
			if (pQuality)
			{
				strSQL.Format("INSERT INTO ALARM_WQ (WARNING_ID,SPECIES,CURRENT_VAL) VALUES ('%s','D%d','%.*f')",
					pData->Get_Alarm_PID(),
					pQuality->m_BitPos,
					pQuality->m_PointBits,
					pQuality->m_CurValue);

				ExcuteNoQuery(strSQL);

				delete pQuality;
				pQuality = NULL;
			}
		}
	}
	catch (CException* pe)
	{
		blResult = false;
		pe->Delete();
	}

	return blResult;
}

bool CDataTranSQLite_Local::Save_Alarm_Flow( COneData_INFO* pData )
{
	CString strSQL = "";
	bool blResult = true;

	try
	{
		int nSensorsNo = 0;
		while (!pData->m_lstFlow.IsEmpty())
		{
			CFlowInitValume_PARA_PER* pFlow = pData->m_lstFlow.RemoveHead();
			if (pFlow)
			{
				strSQL.Format("INSERT INTO ALARM_FL (WARNING_ID,SENSORS_NO,TOTAL_WATER,ANYTIME_FLOW) VALUES ('%s','%02d',%d,%d)",
					pData->Get_Alarm_PID(),
					nSensorsNo,
					pFlow->GetCalTakeValume(),
					pFlow->GetInsFlowValume());

				ExcuteNoQuery(strSQL);

				delete pFlow;
				pFlow = NULL;
				nSensorsNo ++;
			}
		}
	}
	catch (CException* pe)
	{
		blResult = false;
		pe->Delete();
	}

	return blResult;
}

bool CDataTranSQLite_Local::Save_Alarm_Rain( COneData_INFO* pData )
{
	CString strSQL = "";
	bool blResult = true;
	try
	{
		int nSensorsNo = 0;
		while (! pData->m_lstRain.IsEmpty())
		{
			CRainAlert_PARA_PER* pRain  = pData->m_lstRain.RemoveHead();
			if (pRain)
			{
				strSQL.Format("INSERT INTO ALARM_RAIN (WARNING_ID,SENSORS_NO,CURRENT_VAL,PER_TIME) VALUES ('%s','%02d','%.1f',%d)",
					pData->Get_Alarm_PID(),
					nSensorsNo,
					pRain->GetRealTimeValue(),
					pRain->GetPerTime());

				ExcuteNoQuery(strSQL);

				delete pRain;
				pRain = NULL;
				nSensorsNo ++;
			}
		}
	}
	catch (CException* pe)
	{
		blResult = false;
		pe->Delete();
	}

	return blResult;
}

bool CDataTranSQLite_Local::Save_Alarm_Analog( COneData_INFO* pData )
{
	CString strSQL = "";
	bool blResult = true;

	try
	{
		int nAnalog = 0;
		while (! pData->m_lstAnalog.IsEmpty())
		{
			CAnalogLimit_PARA_PER* pAnalog = pData->m_lstAnalog.RemoveHead();
			if (pAnalog)
			{
				strSQL.Format("INSERT INTO ALARM_ANALOG (WARNING_ID,ANALOG_NO,CURRENT_VAL) VALUES ('%s','%02d',%d)",
					pData->Get_Alarm_PID(),
					nAnalog,
					pAnalog->GetRealTimeValue());

				ExcuteNoQuery(strSQL);

				delete pAnalog;
				pAnalog = NULL;
				nAnalog ++;
			}
		}

	}
	catch (CException* pe)
	{
		blResult = false;
		pe->Delete();
	}

	return blResult;
}

bool CDataTranSQLite_Local::Save_Alarm_Displace( COneData_INFO* pData )
{
	CString strSQL = "";
	bool blResult = true;

	try
	{
		int nDisplace = 1;
		while (! pData->m_lstDisplace.IsEmpty())
		{
			CDisplace_PARA_PER* pDisplace = pData->m_lstDisplace.RemoveHead();
			if (pDisplace)
			{
				strSQL.Format("INSERT INTO ALARM_ANALOG (WARNING_ID,SENSORS_NO,CURRENT_XVAL,CURRENT_YVAL) VALUES ('%s','%02d',%d,%d)",
					pData->Get_Alarm_PID(),
					nDisplace,
					pDisplace->GetRealTimeXValue(),
					pDisplace->GetRealTimeYValue());


				ExcuteNoQuery(strSQL);

				delete pDisplace;
				pDisplace = NULL;
				nDisplace ++;
			}
		}

	}
	catch (CException* pe)
	{
		blResult = false;
		pe->Delete();
	}

	return blResult;
}

bool CDataTranSQLite_Local::Save_Alarm_Tempera( COneData_INFO* pData )
{
	CString strSQL = "";
	bool blResult = true;

	try
	{
		int nTempera = 0;
		while (! pData->m_lstTempera.IsEmpty())
		{
			CTempera_PARA_PER* pTempera = pData->m_lstTempera.RemoveHead();
			if (pTempera)
			{
				strSQL.Format("INSERT INTO ALARM_ANALOG (WARNING_ID,SENSORS_NO,CURRENT_VAL) VALUES ('%s','%02d',%d)",
					pData->Get_Alarm_PID(),
					nTempera,
					pTempera->GetRealTimeValue());

				ExcuteNoQuery(strSQL);

				delete pTempera;
				pTempera = NULL;
				nTempera ++;
			}
		}

	}
	catch (CException* pe)
	{
		blResult = false;
		pe->Delete();
	}

	return blResult;
}

bool CDataTranSQLite_Local::SavePeaceReport( COneData_INFO* pData )
{
	CString sSQL = "";
	bool    blResult = true;

	if (!pData)
		return false;

	try
	{
		if (pData->mDataType == COneData_INFO::PEACE_REPORT)
		{
			sSQL.Format("INSERT INTO PEACE_REPORT (RTU,REPORTTIME,REPORT) VALUES ('%s','%s','%s')",
				pData->Get_RTU_ID(),pData->Get_CollectTime_String(),pData->sPeaceReport);
			ExcuteNoQuery(sSQL);
		}
	}
	catch (CException* e)
	{
		blResult = false;
		e->Delete();
	}
	return blResult;
}


void CDataTranSQLite_Local::CheckSQLiteDatabase()
{
	try
	{
		CString strSQL = "";
		if (IsExistTable("OPERATE_RECORD") == false)
		{
			strSQL.Format("CREATE TABLE OPERATE_RECORD (P_ID  integer PRIMARY KEY autoincrement,RTU VARCHAR(16),OPERATE_ACCOUNT VARCHAR(64),OPERATE_TIME DATETIME,OPERATE_DESC VARCHAR(2000))");
			ExcuteNoQuery(strSQL);
		}

		if (IsExistTable("WATERL_DATA") == false)
		{
			strSQL.Format("CREATE TABLE WATERL_DATA (P_ID integer PRIMARY KEY autoincrement,RTU VARCHAR(16),SENSORS_NO VARCHAR(10),CURRENT_VAL VARCHAR(10),COLLECT_TIME DATETIME,SAVE_TIME DATETIME )");
			ExcuteNoQuery(strSQL);
		}

		//异常水位数据
		if (IsExistTable("WATERL_DATA_ERR") == false)
		{
			strSQL.Format("CREATE TABLE WATERL_DATA_ERR (P_ID integer PRIMARY KEY autoincrement,RTU VARCHAR(16),SENSORS_NO VARCHAR(10),CURRENT_VAL VARCHAR(10),COLLECT_TIME DATETIME,SAVE_TIME DATETIME )");
			ExcuteNoQuery(strSQL);
		}

		if (IsExistTable("WATERP_DATA") == false)
		{
			strSQL.Format("CREATE TABLE WATERP_DATA (P_ID integer PRIMARY KEY autoincrement,RTU VARCHAR(16),SENSORS_NO VARCHAR(10),CURRENT_VAL VARCHAR(10),COLLECT_TIME DATETIME,SAVE_TIME DATETIME )");
			ExcuteNoQuery(strSQL);
		}

		if (IsExistTable("FLOWV_DATA") == false)
		{
			strSQL.Format("CREATE TABLE FLOWV_DATA (P_ID integer PRIMARY KEY autoincrement,RTU VARCHAR(16),SENSORS_NO VARCHAR(10),TOTAL_WATER INT,ANYTIME_FLOW INT,COLLECT_TIME DATETIME,SAVE_TIME DATETIME )");
			ExcuteNoQuery(strSQL);
		}

		if (IsExistTable("WATERQ_DATA") == false)
		{
			strSQL.Format("CREATE TABLE WATERQ_DATA (P_ID integer PRIMARY KEY autoincrement,RTU VARCHAR(16),SPECIES VARCHAR(3),CURRENT_VAL VARCHAR(10),COLLECT_TIME DATETIME,SAVE_TIME DATETIME )");
			ExcuteNoQuery(strSQL);
		}

		if (IsExistTable("RAIN_DATA") == false)
		{
			strSQL.Format("CREATE TABLE RAIN_DATA (P_ID integer PRIMARY KEY autoincrement,RTU VARCHAR(16),SENSORS_NO VARCHAR(10),DATA_TYPE VARCHAR(5),CURRENT_VAL VARCHAR(10),HALFH_VAL VARCHAR(10),TOTAL_VAL VARCHAR(10), COLLECT_TIME DATETIME,SAVE_TIME DATETIME )");
			ExcuteNoQuery(strSQL);
		}

		//异常雨量数据
		if (IsExistTable("RAIN_DATA_ERR") == false)
		{
			strSQL.Format("CREATE TABLE RAIN_DATA_ERR (P_ID integer PRIMARY KEY autoincrement,RTU VARCHAR(16),SENSORS_NO VARCHAR(10),DATA_TYPE VARCHAR(5),CURRENT_VAL VARCHAR(10),HALFH_VAL VARCHAR(10),TOTAL_VAL VARCHAR(10), COLLECT_TIME DATETIME,SAVE_TIME DATETIME )");
			ExcuteNoQuery(strSQL);
		}

		if (IsExistTable("ANALOG_DATA") == false)
		{
			strSQL.Format("CREATE TABLE ANALOG_DATA (P_ID integer PRIMARY KEY autoincrement,RTU VARCHAR(16),ANALOG_NO VARCHAR(10),CURRENT_VAL VARCHAR(10),COLLECT_TIME DATETIME,SAVE_TIME DATETIME )");
			ExcuteNoQuery(strSQL);
		}
	
		if (IsExistTable("DISPLACE_DATA") == false)
		{
			strSQL.Format("CREATE TABLE DISPLACE_DATA (P_ID integer PRIMARY KEY autoincrement,RTU VARCHAR(16),SENSORS_NO VARCHAR(10),CURRENT_XVAL VARCHAR(10),CURRENT_YVAL VARCHAR(10),COLLECT_TIME DATETIME,SAVE_TIME DATETIME )");
			ExcuteNoQuery(strSQL);
		}
		
		if (IsExistTable("TEMPERA_DATA") == false)
		{
			strSQL.Format("CREATE TABLE TEMPERA_DATA (P_ID integer PRIMARY KEY autoincrement,RTU VARCHAR(16),SENSORS_NO VARCHAR(10),CURRENT_VAL VARCHAR(10),COLLECT_TIME DATETIME,SAVE_TIME DATETIME )");
			ExcuteNoQuery(strSQL);
		}
		if (IsExistTable("PULSE_DATA") == false)
		{
			strSQL.Format("CREATE TABLE PULSE_DATA (P_ID integer PRIMARY KEY autoincrement,RTU VARCHAR(16),CODE VARCHAR(16),VALUE1 VARCHAR(10),VALUE2 VARCHAR(10),VALUE3 VARCHAR(10),VALUE4 VARCHAR(10),COLLECT_TIME DATETIME,SAVE_TIME DATETIME )");
			ExcuteNoQuery(strSQL);
		}

		if (IsExistTable("ALARM_RECORD") == false)
		{
			strSQL.Format("CREATE TABLE ALARM_RECORD (P_ID VARCHAR(64) PRIMARY KEY,RTU VARCHAR(16),W_TYPE INT,WARNING_TIME DATETIME,SAVE_TIME DATETIME)");
			ExcuteNoQuery(strSQL);
		}

		if (IsExistTable("ALARM_WQ") == false)
		{
			strSQL.Format("CREATE TABLE ALARM_WQ (P_ID integer PRIMARY KEY autoincrement,WARNING_ID VARCHAR(64),SPECIES VARCHAR(3),CURRENT_VAL VARCHAR(10))");
			ExcuteNoQuery(strSQL);
		}

		if (IsExistTable("ALARM_WL") ==false)
		{
			strSQL.Format("CREATE TABLE ALARM_WL (P_ID integer PRIMARY KEY autoincrement,WARNING_ID VARCHAR(64),SENSORS_NO VARCHAR(10),CURRENT_VAL VARCHAR(10))");
			ExcuteNoQuery(strSQL);
		}

		if (IsExistTable("ALARM_WP") == false)
		{
			strSQL.Format("CREATE TABLE ALARM_WP (P_ID integer PRIMARY KEY autoincrement,WARNING_ID VARCHAR(64),SENSORS_NO VARCHAR(10),CURRENT_VAL VARCHAR(10))");
			ExcuteNoQuery(strSQL);
		}

		if (IsExistTable("ALARM_FL") == false)
		{
			strSQL.Format("CREATE TABLE ALARM_FL (P_ID integer PRIMARY KEY autoincrement,WARNING_ID VARCHAR(64),SENSORS_NO VARCHAR(10),TOTAL_WATER INT,ANYTIME_FLOW INT)");
			ExcuteNoQuery(strSQL);
		}

		if (IsExistTable("ALARM_RAIN") == false)
		{
			strSQL.Format("CREATE TABLE ALARM_RAIN (P_ID integer PRIMARY KEY autoincrement,WARNING_ID VARCHAR(64),SENSORS_NO VARCHAR(10),CURRENT_VAL VARCHAR(10),PER_TIME INT)");
			ExcuteNoQuery(strSQL);
		}

		if (IsExistTable("ALARM_DISPLACE") == false)
		{
			strSQL.Format("CREATE TABLE ALARM_DISPLACE (P_ID integer PRIMARY KEY autoincrement,WARNING_ID VARCHAR(64),SENSORS_NO VARCHAR(10),CURRENT_XVAL VARCHAR(10),CURRENT_YVAL VARCHAR(10))");
			ExcuteNoQuery(strSQL);
		}

		if (IsExistTable("ALARM_TEMPERA") == false)
		{
			strSQL.Format("CREATE TABLE ALARM_TEMPERA (P_ID integer PRIMARY KEY autoincrement,WARNING_ID VARCHAR(64),SENSORS_NO VARCHAR(10),CURRENT_VAL VARCHAR(10))");
			ExcuteNoQuery(strSQL);
		}

		if (IsExistTable("ALARM_ANALOG") == false)
		{
			strSQL.Format("CREATE TABLE ALARM_ANALOG (P_ID integer PRIMARY KEY autoincrement,WARNING_ID VARCHAR(64),ANALOG_NO VARCHAR(10),CURRENT_VAL VARCHAR(10))");
			ExcuteNoQuery(strSQL);
		}
		if (IsExistTable("PEACE_REPORT") == false)
		{
			strSQL.Format("CREATE TABLE PEACE_REPORT (P_ID integer PRIMARY KEY autoincrement,RTU VARCHAR(16),REPORTTIME DATETIME, REPORT TEXT)");
			ExcuteNoQuery(strSQL);
		}
	}
	catch (CException* e)
	{
		e->Delete();
	}
}

bool CDataTranSQLite_Local::ExcuteNoQuery( CString strSQL )
{
	bool ret = true;

#ifdef DEBUG
	SYSTEMTIME dbtime;
	TRACE("SQLite数据库操作:%s\r\n",strSQL);
	GetLocalTime(&dbtime);
	TRACE("\t\tExecute Begin: %s\r\n",
		CSystemTimeOp::ConvertSystemTimeToString(&dbtime,true));
#endif

	bool blLocked = false;
	try
	{
		strSQL.Trim();
		if (!strSQL.IsEmpty())
		{
			 SQLiteCS_Lock();
			 blLocked = true;
			 sqlite_ex_result r = m_sqlite.DirectStatement(strSQL);
			 SQLiteCS_UnLock();
			 blLocked = false;

			 ret = r.blResult;

			 if (r.blResult == false)
			 {
				 CString sLog = "";
				 _Log_level_type xLevel = Warning_log_Message;
				 sLog.Format("SQLite error:%s,SQL:%s",r.sMessage,strSQL);
				 AddThreadLog(sLog);
			 }
		}
	}
	catch (CException* e)
	{
		CString sLog = "";
		_Log_level_type xLevel = Warning_log_Message;
		sLog.Format("SQLite error,SQL:%s",strSQL);
		AddThreadLog(sLog);
		if (blLocked == true)
			SQLiteCS_UnLock();

		e->Delete();
		ret = false;
	}

#ifdef DEBUG
	//TRACE("SQLite数据库操作:%s\r\n",strSQL);
	GetLocalTime(&dbtime);
	TRACE("\t\tExecute end: %s\r\n",
		CSystemTimeOp::ConvertSystemTimeToString(&dbtime,true));
#endif

	return ret;
}

/*
CSqlStatement* CDataTranSQLite_Local::ExcuteQuery( CString strSQL )
{
	CSqlStatement* stmt =NULL;
	try
	{
		stmt = m_sqlite.Statement(strSQL);
	}
	catch (CException* e)
	{
		CString sLog = "";
		_Log_level_type xLevel = Warning_log_Message;
		sLog.Format("SQLite error,SQL:%s",strSQL);
		AddThreadLog(sLog);

		e->Delete();
		stmt = NULL;
	}

	return stmt;		
}
*/


bool CDataTranSQLite_Local::DeleteDataOnTimer()
{
	SYSTEMTIME st;
	GetLocalTime(&st);

	long TotalMonths = st.wYear * 12 + st.wMonth;

	long _logEndMonth = TotalMonths - m_nSaveMonth;

	CString strDelEndTime = "";
	if ( _logEndMonth % 12 == 0 )
		strDelEndTime.Format("%04d-12-01 00:00:00.000",  long(_logEndMonth /12 ) - 1);
	else
		strDelEndTime.Format("%04d-%02d-01 00:00:00.000",long(_logEndMonth /12 ),long(_logEndMonth % 12));

	bool ret = true;
	try
	{
		CString sSQL = "";

		//Delete Data 
		sSQL.Format("DELETE FROM OPERATE_RECORD WHERE OPERATE_TIME < '%s'",strDelEndTime);
		ret = ExcuteNoQuery(sSQL);

		sSQL.Format("DELETE FROM WATERL_DATA WHERE COLLECT_TIME < '%s'",strDelEndTime);
		ret = ExcuteNoQuery(sSQL);

		sSQL.Format("DELETE FROM WATERP_DATA WHERE COLLECT_TIME < '%s'",strDelEndTime);
		ret = ExcuteNoQuery(sSQL);

		sSQL.Format("DELETE FROM FLOWV_DATA WHERE COLLECT_TIME < '%s'",strDelEndTime);
		ret = ExcuteNoQuery(sSQL);

		sSQL.Format("DELETE FROM WATERQ_DATA WHERE COLLECT_TIME < '%s'",strDelEndTime);
		ret = ExcuteNoQuery(sSQL);

		sSQL.Format("DELETE FROM RAIN_DATA WHERE COLLECT_TIME < '%s'",strDelEndTime);
		ret = ExcuteNoQuery(sSQL);

		sSQL.Format("DELETE FROM ANALOG_DATA WHERE COLLECT_TIME < '%s'",strDelEndTime);
		ret = ExcuteNoQuery(sSQL);

		sSQL.Format("DELETE FROM DISPLACE_DATA WHERE COLLECT_TIME < '%s'",strDelEndTime);
		ret = ExcuteNoQuery(sSQL);

		sSQL.Format("DELETE FROM TEMPERA_DATA WHERE COLLECT_TIME < '%s'",strDelEndTime);
		ret = ExcuteNoQuery(sSQL);

		sSQL.Format("DELETE FROM PULSE_DATA WHERE COLLECT_TIME < '%s'",strDelEndTime);
		ret = ExcuteNoQuery(sSQL);

		sSQL.Format("DELETE FROM PEACE_REPORT WHERE REPORTTIME < '%s'",strDelEndTime);
		ret = ExcuteNoQuery(sSQL);

		//Delete ALARM_RECORD data before
		sSQL.Format("DELETE FROM ALARM_WQ WHERE WARNING_ID IN (SELECT P_ID FROM ALARM_RECORD  WHERE WARNING_TIME < '%s')",strDelEndTime);
		ret = ExcuteNoQuery(sSQL);

		sSQL.Format("DELETE FROM ALARM_WL WHERE WARNING_ID IN (SELECT P_ID FROM ALARM_RECORD  WHERE WARNING_TIME < '%s')",strDelEndTime);
		ret = ExcuteNoQuery(sSQL);

		sSQL.Format("DELETE FROM ALARM_WP WHERE WARNING_ID IN (SELECT P_ID FROM ALARM_RECORD  WHERE WARNING_TIME < '%s')",strDelEndTime);
		ret = ExcuteNoQuery(sSQL);

		sSQL.Format("DELETE FROM ALARM_FL WHERE WARNING_ID IN (SELECT P_ID FROM ALARM_RECORD  WHERE WARNING_TIME < '%s')",strDelEndTime);
		ret = ExcuteNoQuery(sSQL);

		sSQL.Format("DELETE FROM ALARM_RAIN WHERE WARNING_ID IN (SELECT P_ID FROM ALARM_RECORD  WHERE WARNING_TIME < '%s')",strDelEndTime);
		ret = ExcuteNoQuery(sSQL);

		sSQL.Format("DELETE FROM ALARM_ANALOG WHERE WARNING_ID IN (SELECT P_ID FROM ALARM_RECORD  WHERE WARNING_TIME < '%s')",strDelEndTime);
		ret = ExcuteNoQuery(sSQL);

		sSQL.Format("DELETE FROM ALARM_DISPLACE WHERE WARNING_ID IN (SELECT P_ID FROM ALARM_RECORD  WHERE WARNING_TIME < '%s')",strDelEndTime);
		ret = ExcuteNoQuery(sSQL);

		sSQL.Format("DELETE FROM ALARM_TEMPERA WHERE WARNING_ID IN (SELECT P_ID FROM ALARM_RECORD  WHERE WARNING_TIME < '%s')",strDelEndTime);
		ret = ExcuteNoQuery(sSQL);

		sSQL.Format("DELETE FROM ALARM_RECORD WHERE WARNING_TIME < '%s'",strDelEndTime);
		ret = ExcuteNoQuery(sSQL);
		//Delete ALARM_RECORD

		//Delete Other data
	}
	catch (CException* e)
	{
		e->Delete();
		ret = false;
	}

	return ret;
}

void CDataTranSQLite_Local::SetDataSource( CString strSource )
{
	m_strDataSource = strSource;
}

void CDataTranSQLite_Local::SQLiteCS_Lock()
{
	m_cs_Sqlite.Lock();
}

void CDataTranSQLite_Local::SQLiteCS_UnLock()
{
	m_cs_Sqlite.Unlock();
}

void CDataTranSQLite_Local::SQLiteBeginTransaction()
{
	CString sSQL = "BEGIN";
	ExcuteNoQuery(sSQL);
	m_blStartTransaction = true;
	GetLocalTime(&m_stBeginTransaction);
}

void CDataTranSQLite_Local::SQLiteCommitTransaction()
{
	CString sSQL = "COMMIT";
	ExcuteNoQuery(sSQL);
	m_blStartTransaction = false;
}

void CDataTranSQLite_Local::PragmaSQLite()
{
	//CString sSQL = "";
	//sSQL.Format("PRAGMA cache_size=50000;");
	//ExcuteNoQuery(sSQL);
	//sSQL.Format("PRAGMA temp_store=MEMORY;");
	//ExcuteNoQuery(sSQL);
}


