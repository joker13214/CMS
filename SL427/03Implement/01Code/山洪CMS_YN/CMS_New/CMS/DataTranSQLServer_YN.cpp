#include "StdAfx.h"
#include "DataTranSQLServer_YN.h"



const CString Table_ST_PPTN_R_Record::ToString()
{
	CString sResult = "";
	sResult.Format("STCD=%s,TM=%s,DRP=%f,INTV=%.2f,PRD=%.2f,DYP=%f,WTH=%s",
		STCD,TM,DRP,INTV,PRD,DYP,WTH);
	return sResult;
}

const CString Table_ST_STORM_R_Record::ToString()
{
	CString sResult = "";
	sResult.Format("STCD=%s,TM=%s,STRMDR=%.2f,STRMP=%f,WTH=%s",
		STCD,TM,STRMDR,STRMP,WTH);
	return sResult;
}


CDataTranSQLServer_YN::CDataTranSQLServer_YN(void)
{
}

CDataTranSQLServer_YN::~CDataTranSQLServer_YN(void)
{
}

long CDataTranSQLServer_YN::ThreadFunction()
{
	Initialization();

	SetThreadControl(true);
	SetThreadState(THREAD_STATE_RUNNING);
	while (GetThreadControl() != false)
	{		
	//	StatisticsRain_1Day_OnTimer();

		int nDataCount = GetDataCount();
		if (nDataCount <=0 )
		{
			Sleep(1);
			continue;
		}

		if (GetConnectionState() != true)
		{
			if (OpenDatabase() == true)
			{
				AddThreadLog("连接主站数据库成功.");
				CheckDatabase();
			}
		}
		if (GetConnectionState() != true)
		{
			Sleep(1);
			continue;
		}

		COneData_INFO* pData = GetOneData();
		Statistics_Data_OnRealTime(pData);
		Sleep(0);
	}

	ResetThread();

	return 0;
}




void CDataTranSQLServer_YN::Initialization()
{
	AddThreadLog("正在启动数据接口模块云南水雨情数据接口");

	if (OpenDatabase() == true)
	{
		AddThreadLog("连接主站数据库成功.");
		CheckDatabase();
	}
}
void CDataTranSQLServer_YN::SetDataSource( CString strSource )
{
	m_strDataSource = strSource;
}


bool CDataTranSQLServer_YN::OpenDatabase()
{
	return OpenDatabase(m_strDataSource);
}
bool CDataTranSQLServer_YN::OpenDatabase( CString dataSource )
{
	m_Database.SetLoginTimeout(30);
	if (m_Database.IsOpen())
		return true;
	try
	{
		return m_Database.OpenEx(dataSource, CDatabase::noOdbcDialog) == TRUE ? true : false;
	}
	catch(CDBException *pe)
	{
		TRACE("Error:%s", pe->m_strError);
		CString logmassage = "";
		logmassage.Format("数据源:%s Error:%s",dataSource,pe->m_strError);
		pe->Delete();

		AddThreadLog(logmassage,Error_Log_message);		
		return false;
	}
	return true;
}
void CDataTranSQLServer_YN::SetSqlite( CDbSQLite* sqlite )
{
	if (sqlite )
		m_sqlite = sqlite;
}
bool CDataTranSQLServer_YN::GetConnectionState()
{
	if (!m_Database.IsOpen())
		return false;
	try
	{
		CString strSQL="--";
		m_Database.ExecuteSQL(strSQL);
	}
	catch (CDBException* e)
	{
		CString strMessage = "";
		strMessage.Format("与主站数据库连接断开,Error:%s",e->m_strError);
		AddThreadLog(strMessage,Error_Log_message);
		e->Delete();
		m_Database.Close();
		return false;
	}
	return true;
}


bool CDataTranSQLServer_YN::ExcuteNoQuery( CString sSQL )
{
	bool ret = true;

	if (sSQL.Trim() == "")
		return true;

	CString sLog = "";
	try
	{
		m_Database.ExecuteSQL(sSQL);

#ifdef DEBUG
		TRACE("ExcuteNoQuery:%s\r\n",sSQL);
#endif
	}
	catch (CDBException* e)
	{
		sLog.Format("执行SQL:%s,Error:%s",sSQL,e->m_strError);
		AddThreadLog(sLog,Error_Log_message);
		e->Delete();
		ret = false;
	}

	return ret;
}

void CDataTranSQLServer_YN::CheckDatabase()
{
	int nCount = 5;
	CString checkTables[]=
	{
		"ST_STBPRP_B",
		"ST_PPTN_R",
		"ST_STORM_R",
		"ST_RIVER_R",
		"ST_RSVR_R"
	};
	CString checkTable_Names[]=
	{
		"测站基本属性表",
		"降水量表",
		"暴雨加报表",
		"河道水情表",
		"水库水情表"
	};

	CString strLog = "";

	for (int idx = 0; idx < nCount; idx ++)
	{
		if (CheckTableExist(checkTables[idx]) == false)
		{
			strLog.Format("主站表检查,表:%s(%s)不存在.",checkTable_Names[idx],
				checkTables[idx]);
			AddThreadLog(strLog,Warning_log_Message);
		}
	}
}



void CDataTranSQLServer_YN::Statistics_Data_OnRealTime(COneData_INFO* pData)//.....
{
	if (pData !=  NULL)
	{
		if (GetConnectionState() != true)
		{
			if (OpenDatabase() == true)
			{
				AddThreadLog("连接主站数据库成功.");
				CheckDatabase();
			}
		}
		if (GetConnectionState() == true)
		{
			CString sRTUID = pData->Get_SYQ_RTU_ID();
			if (pData->mDataType == COneData_INFO::RAIN_DATA)
			{
				//雨量数据实时统计
				if (! pData->m_lstRain.IsEmpty())
				{
					CRainAlert_PARA_PER* pRain = pData->m_lstRain.RemoveHead();
					if (pRain)
					{

						CRainData_Item* pRecvData  = new CRainData_Item(pRain->GetDataType(),pRain->GetRealTimeValue(),pRain->GetHalfHourValue(),pRain->GetTotalValue(),pData->Get_CollectTime_String());
						CalcRainData_RealTime(sRTUID,pRecvData);

						_DELETE(pRecvData);
						_DELETE(pRain);
					}
				}
			}
			else if ( pData->mDataType == COneData_INFO::WATER_L_DATA ||
				(pData->mDataType == COneData_INFO::ALARM_RECORD && pData->mAlarmType == COneData_INFO::ALARM_WATERLEVEL))
			{
				//水位数据实时统计
				if (!pData->m_lstWaterLevel.IsEmpty())
				{
					CWaterLevel_PAR_PER* pWaterLevel = pData->m_lstWaterLevel.RemoveHead();
					if (pWaterLevel)
					{
						CWaterL_DataItem* pRecvData = new CWaterL_DataItem(pWaterLevel->GetRealValue(),pData->mCollectTime);
						CalcWaterLevel_RealTime(sRTUID,pRecvData);

						_DELETE(pRecvData);
						_DELETE(pWaterLevel);
					}
				}
			}
		}
		//----------------------
		_DELETE(pData);
	}
}


void CDataTranSQLServer_YN::StatisticsRain_1Day_OnTimer()
{
	SYSTEMTIME nowTime;
	GetLocalTime(&nowTime);

	if (nowTime.wHour == 8 && nowTime.wMinute == 2 && m_blStartCalc == false)
	{
		m_blStartCalc = true;
		m_blCalc = false;
#ifdef DEBUG
		TRACE("%s START 8点定时统计.\r\n",
			CSystemTimeOp::ConvertSystemTimeToString(&nowTime,true));
#endif
	}

	if (m_blStartCalc == true)
	{
		//统计8点整点数据(雨量)
		if (m_blCalc == false)
		{
			CTime ctimenow(nowTime);
			CTimeSpan _tspan (1,0,0,0);
			CTime ctimebegin = ctimenow - _tspan;

			CString strBegin = "",strEnd = "",strPoint= "";

			strBegin.Format("%04d-%02d-%02d 08:00:01.000",
				ctimebegin.GetYear(),ctimebegin.GetMonth(),ctimebegin.GetDay());
			strEnd.Format("%04d-%02d-%02d 08:00:01.000",
				nowTime.wYear,nowTime.wMonth,nowTime.wDay);
			strPoint.Format("%04d-%02d-%02d 08:00",
				nowTime.wYear,nowTime.wMonth,nowTime.wDay);

			std::list<CAddrMap_Item*>::iterator Itr = m_mapAddressMap.end();
			for ( Itr = m_mapAddressMap.begin();
				Itr != m_mapAddressMap.end();
				Itr ++)
			{
				CAddrMap_Item* second = (*Itr);
				if (second != NULL && second->GetType() == CZ_Type_RAIN)
				{
					Table_ST_PPTN_R_Record findrecord;

					CString sSQL ="";
					sSQL.Format("SELECT STCD, TM, DRP, INTV, PDR, DYP, WTH FROM ST_PPTN_R WHERE STCD='%s' AND TM='%s'",
						/*Itr->second->Get_STCD()*/second->Get_STCD(),strPoint);

					bool blfinded  = GetRainfallTableRecord(sSQL,findrecord);
					if (blfinded == false)
					{
						//double Rain1Day = StatisticsRain_1Day(/*Itr->second*/second,strBegin,strEnd);
						//if (Rain1Day < 0.0)
						//	Rain1Day = 0.0;

						//if (Rain1Day >= m_dCHECK_RAIN_DAY)
						//{
						//	SYSTEMTIME specialtime = nowTime;
						//	Rain1Day = StatisticsRain_1Day_special(/*Itr->second*/second,specialtime);
						//}
						//if (Rain1Day < 0.0 || Rain1Day >= m_dCHECK_RAIN_DAY)
						//{
						//	Rain1Day = 0.0;
						//}


						double Rain1Day = 0.0;
						bool   isNull = false;
						Rain1Day = CalRainData_OnTimeRange(second,strBegin,strEnd,Rain_statistics_AM8Point,isNull);
						if (Rain1Day <=0.0 || Rain1Day > m_dCHECK_RAIN_DAY)
							Rain1Day = 0.0;

						Table_ST_PPTN_R_Record AM8Record ;
						AM8Record.STCD = second->Get_STCD();//Itr->second->Get_STCD();
						AM8Record.TM = strPoint;
						AM8Record.DYP = Rain1Day;
						AM8Record.WTH = "8";
						UpdateRainfallTableRecord(AM8Record,Rain_statistics_AM8Point);
					}
				}
			}

			m_blCalc = true;
		}

		//Stop calc timer
		if ( (nowTime.wHour == 8 && nowTime.wMinute == 2 ) != true && m_blCalc == true)
		{
			m_blStartCalc = false;
#ifdef DEBUG
			TRACE("%s STOP 8点定时统计.\r\n",
				CSystemTimeOp::ConvertSystemTimeToString(&nowTime,true));
#endif
		}
	}
}
