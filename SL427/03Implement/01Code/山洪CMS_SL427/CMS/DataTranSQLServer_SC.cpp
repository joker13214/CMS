#include "StdAfx.h"
#include "DataTranSQLServer_SC.h"
#include "UdfRtuDataSave.h"
#include "SystemTimeOp.h"
#include "CMS.h"
#include <math.h>


CDataTranSQLServer_SC::CDataTranSQLServer_SC()
{
	m_ThreadType	= DataTran_SQLServer_SC;
	m_blStartCalc	= false;
	m_bNeedData		= false;

	//m_bNeedData = true;    //修改为来点就统计
	//修改为Local调用 由于SQLite对并发处理较弱

	m_dCHECK_RAIN_PER		= GETAPP()->m_GlobalOptions.m_dRainCheckPer;
	m_dCHECK_RAIN_DAY		= GETAPP()->m_GlobalOptions.m_dRainCheckDay;

	m_nCheck5Minutes_MNT	= GETAPP()->m_GlobalOptions.m_nCheck5Minutes_Count;
	m_nCheck15Minutes_MNT	= GETAPP()->m_GlobalOptions.m_nCheck15Minutes_Count;
	m_nCheck1Hour_MNT		= GETAPP()->m_GlobalOptions.m_nCheck1Hour_Count;
	m_nChcek1Day_MNT		= GETAPP()->m_GlobalOptions.m_nCheck1Day_Count;
	m_nWaterLevaeIType      = GETAPP()->m_GlobalOptions.m_WatarLevelIType;
	m_RainPRDFlag			= GETAPP()->m_GlobalOptions.m_RainPRDFlag;

	m_nDatabaseType			= GETAPP()->m_GlobalOptions.m_DataBaseType;

	m_blCalc = false;
	m_local_sqlite = NULL;

	m_strScCacheDBName = GETAPP()->m_GlobalOptions.m_strScCacheDbName;
	m_blOpenScCacheSucc = false;

	m_dwInSQLServer_Interval = GETAPP()->m_GlobalOptions.m_nScCacheCheckOnTimer;
	m_dwSaveToSQLite_Interval = GETAPP()->m_GlobalOptions.m_nScCacheSaveOnTimer;
	m_dwChcekInSecond  = GETAPP()->m_GlobalOptions.m_nScCacheCheckInSecond;
	m_dwPreInSQLServerTC = 0;
	m_dwPreSaveToSQLiteTC = 0;

	//监测当前是否有挤压报文 
	m_nWaittingBufferCount = 0;

	m_ScStatisticsTimer.m_nInterval = GETAPP()->m_GlobalOptions.m_nScStatisticsCheckInterval;
	m_ScStatisticsTimer.m_nPreTickCount = 0;
	m_ScUpdataCacheTimer.m_nInterval = GETAPP()->m_GlobalOptions.m_nScUpdataCacheCheckInterval;
	m_ScUpdataCacheTimer.m_nPreTickCount = 0;
	m_blLocalSQliteTransStartFlag = true;

	m_RainStcEarly.m_nScStatisticsCheck5mEarly = GETAPP()->m_GlobalOptions.m_nScStatisticsCheck5mEarly;
	m_RainStcEarly.m_nScStatisticsCheck15mEarly = GETAPP()->m_GlobalOptions.m_nScStatisticsCheck15mEarly;
	m_RainStcEarly.m_nScStatisticsCheck1hEarly = GETAPP()->m_GlobalOptions.m_nScStatisticsCheck1hEarly;
	m_RainStcEarly.m_nScStatisticsCheck1dEarly = GETAPP()->m_GlobalOptions.m_nScStatisticsCheck1dEarly;

	m_localTrans = NULL;
}

CDataTranSQLServer_SC::~CDataTranSQLServer_SC()
{

	//线程已经停止
	while(GetDataCount() > 0 && GetConnectionState() == true)
	{
		COneData_INFO* pData = GetOneData();
		if (pData !=  NULL)
		{
			CString sRTUID = pData->Get_RTU_ID();
			if (pData->mDataType == COneData_INFO::RAIN_DATA)
			{
				//雨量数据实时统计
				if (! pData->m_lstRain.IsEmpty())
				{
					CRainAlert_PARA_PER* pRain = pData->m_lstRain.RemoveHead();
					if (pRain)
					{
						CRainData_Item* pRecvData  = new CRainData_Item(pRain->GetDataType(),
																		pRain->GetRealTimeValue(),
																		pRain->GetHalfHourValue(),
																		pRain->GetTotalValue(),
																		pData->Get_CollectTime_String());
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
			else if ( pData->mDataType == COneData_INFO::DISPLACE_DATA ||
				(pData->mDataType == COneData_INFO::ALARM_RECORD && pData->mAlarmType == COneData_INFO::ALARM_DISPLACE))
			{
				//水位数据实时统计
				if (!pData->m_lstDisplace.IsEmpty())
				{
					CDisplace_PARA_PER* pDisplace = pData->m_lstDisplace.RemoveHead();
					if (pDisplace)
					{
						//CDisplace_DataItem* pRecvData = new CDisplace_DataItem(pDisplace->GetRealTimeXValue(),pDisplace->GetRealTimeYValue(),pDisplace->GetDisplaceNo(),pData->mCollectTime);
						CDisplace_DataItem* pRecvData = new CDisplace_DataItem(pDisplace->GetRealTimeXValue(),pDisplace->GetRealTimeYValue(),pData->mCollectTime);
						CalcDisplace_RealTime(sRTUID,pDisplace->GetDisplaceNo(),pRecvData);

						_DELETE(pRecvData);
						_DELETE(pDisplace);
					}
				}
			}
			else if ( pData->mDataType == COneData_INFO::TEMPERA_DATA ||
				(pData->mDataType == COneData_INFO::ALARM_RECORD && pData->mAlarmType == COneData_INFO::ALARM_TEMPERA))
			{
				//水位数据实时统计
				if (!pData->m_lstTempera.IsEmpty())
				{
					CTempera_PARA_PER* pTempera = pData->m_lstTempera.RemoveHead();
					if (pTempera)
					{
						CTempera_DataItem* pRecvData = new CTempera_DataItem(pTempera->GetRealTimeValue(),pData->mCollectTime);
						CalcTempera_RealTime(sRTUID,pRecvData);

						_DELETE(pRecvData);
						_DELETE(pTempera);
					}
				}
			}
			else if (pData->mDataType == COneData_INFO::FLOW_V_DATA || (pData->mDataType == COneData_INFO::ALARM_RECORD)) 
			{
				//流量数据实时统计
				if (!pData->m_lstFlow.IsEmpty())
				{
					CFlowInitValume_PARA_PER* pFlow = pData->m_lstFlow.RemoveHead();
					if (pFlow)
					{
						CFlow_DataItem* pRecvData = new CFlow_DataItem((double)pFlow->GetInsFlowValume()/1000.0,(double)pFlow->GetCalTakeValume()/10.0, pData->mCollectTime);
						CFlow_RealTime(sRTUID, pRecvData);

						_DELETE(pRecvData);
						_DELETE(pFlow);
					}
				}
			}
		}
	}

	//cms退出时
	ScCacheInSQLServer();
	ScCacheSaveToSQLite();
	
	ClearAddrMap();
}


void CDataTranSQLServer_SC::Initialization()
{
	AddThreadLog("正在启动数据接口模块:四川省山洪灾害数据接口");  //日志信息函数

	if (!InitAddressMap())
		return ;

	if (OpenScCacheSQLite())
		CheckScCacheSQLite();

	CheckAddressMap_ForRtu();

	//将本地SQLite的数据读取到上传缓存中
	ReadLocalSQLiteTOUpDataCache();

	//启动时读取sc.cache.db库数据
	ReadScCacheSQLiteByStart();

	if (OpenDatabase() == true)
	{
		AddThreadLog("连接主站数据库成功.");
		if (m_nDatabaseType == SQLServer_Database)
			CheckDatabase();
	}
}

void CDataTranSQLServer_SC::StatisticsRain_1Day_OnTimer()
{
	SYSTEMTIME nowTime;
	GetLocalTime(&nowTime);

	//if (nowTime.wHour == 8 && nowTime.wMinute == 2 && m_blStartCalc == false)
	if (nowTime.wHour == 8 && nowTime.wMinute >= 2 && nowTime.wMinute <= 15 && m_blStartCalc == false)
	{
		m_blStartCalc = true;
		m_blCalc = false;

		#ifdef DEBUG
		TRACE("%s START 8点定时统计.\r\n",CSystemTimeOp::ConvertSystemTimeToString(&nowTime,true));
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
			for ( Itr = m_mapAddressMap.begin();Itr != m_mapAddressMap.end();Itr ++)
			{
				CAddrMap_Item* second = (*Itr);
				if (second != NULL && second->GetType() == CZ_Type_RAIN)
				{
					Table_ST_PPTN_R_Record findrecord;

					CString sSQL ="";
					if (m_nDatabaseType == SQLServer_Database)
						sSQL.Format("SELECT STCD, TM, DRP, INTV, PDR, DYP, WTH FROM ST_PPTN_R WHERE STCD='%s' AND TM='%s'",
						/*Itr->second->Get_STCD()*/second->Get_STCD(),strPoint);
					else if (m_nDatabaseType == Oracle_Database)
						sSQL.Format("SELECT STCD, TM, DRP, INTV, PDR, DYP, WTH FROM ST_PPTN_R WHERE STCD='%s' AND TM=to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3')",
						/*Itr->second->Get_STCD()*/second->Get_STCD(),strPoint);

					bool blfinded  = GetRainfallTableRecord(sSQL,findrecord);
					if (blfinded == false)
					{
						double Rain1Day = 0.0;
						bool   isDayNull = false;
						Rain1Day = CalRainData_OnTimeRange(second,strBegin,strEnd,Rain_statistics_AM8Point,isDayNull);
						if (Rain1Day <=0.0 || Rain1Day > m_dCHECK_RAIN_DAY)
							Rain1Day = 0.0;

						Table_ST_PPTN_R_Record AM8Record ;
						AM8Record.STCD = second->Get_STCD();//Itr->second->Get_STCD();
						AM8Record.TM   = strPoint;
						AM8Record.DYP  = Rain1Day;
						AM8Record.WTH  = "8";

						//CALC 07:00-08:00's Rain
						if (false == isDayNull)
						{
							CString str7HourBegin = "",str7HourEnd = "";
							str7HourEnd = strEnd;
							str7HourBegin.Format("%04d-%02d-%02d 07:00:01.000",nowTime.wYear,nowTime.wMonth,nowTime.wDay);

							double hour7Rain = 0.0f;
							bool   is7HourNull = false;
							hour7Rain = CalRainData_OnTimeRange(second,str7HourBegin,str7HourEnd,Rain_statistics_1hour,is7HourNull);
							if (is7HourNull == false)
							{
								if (hour7Rain<=0.0 || hour7Rain > m_dCHECK_RAIN_PER)
									hour7Rain = 0.0;

								int HourRainMinutes = 0;
								if (hour7Rain >= 0.1)
								{

									if(m_RainPRDFlag != 0)
									{
										HourRainMinutes = GetRainMinutes(second,str7HourBegin,str7HourEnd);
										if (HourRainMinutes <= 0 || HourRainMinutes > 60)
											HourRainMinutes = 60;

										AM8Record.DRP = hour7Rain;
										AM8Record.INTV = 1.00;
										AM8Record.WTH = "7";
										if (HourRainMinutes >= 60 || HourRainMinutes <= 0 )
											AM8Record.PRD = 0.00;
										else
											AM8Record.PRD = (double)HourRainMinutes /100;
									}
									else 
									{

										AM8Record.DRP = hour7Rain;
										AM8Record.INTV = 1.00;
										AM8Record.WTH = "7";
										AM8Record.PRD = 0.00;
									}

								}

							}//end if (is7HourNull == false)	
						}//end if (false == isNull)
						
						UpdateRainfallTableRecord(AM8Record,Rain_statistics_AM8Point);
					}//end if (blfinded == false)
				}
			}

			m_blCalc = true;
		}

		//Stop calc timer
		if ( (nowTime.wHour == 8 && nowTime.wMinute >= 2 && nowTime.wMinute <= 15) != true && m_blCalc == true)
		{
			m_blStartCalc = false;
#ifdef DEBUG
			TRACE("%s STOP 8点定时统计.\r\n",
				CSystemTimeOp::ConvertSystemTimeToString(&nowTime,true));
#endif
		}
	}
}
//省标库处理遥测终端机上传的数据
void CDataTranSQLServer_SC::Statistics_Data_OnRealTime(COneData_INFO* pData)
{
	if (pData !=  NULL)
	{
		if (GetConnectionState() != true)
		{
			if (OpenDatabase() == true)
			{
				AddThreadLog("连接主站数据库成功.");
				if (SQLServer_Database == m_nDatabaseType)
					CheckDatabase();
			}
		}
		if (GetConnectionState() == true)
		{
			CString sRTUID = pData->Get_RTU_ID();
			if (pData->mDataType == COneData_INFO::RAIN_DATA)
			{
				//雨量数据实时统计
				if (! pData->m_lstRain.IsEmpty())
				{
					CRainAlert_PARA_PER* pRain = pData->m_lstRain.RemoveHead();
					if (pRain)
					{

						CRainData_Item* pRecvData  = new CRainData_Item(pRain->GetDataType(),
																		pRain->GetRealTimeValue(),
																		pRain->GetHalfHourValue(),
																		pRain->GetTotalValue(),
																		pData->Get_CollectTime_String());
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
			else if ( pData->mDataType == COneData_INFO::DISPLACE_DATA ||
				(pData->mDataType == COneData_INFO::ALARM_RECORD && pData->mAlarmType == COneData_INFO::ALARM_DISPLACE))
			{
				//位移数据实时统计
				//if (!pData->m_lstDisplace.IsEmpty())
				while (!pData->m_lstDisplace.IsEmpty())
				{
					CDisplace_PARA_PER* pDisplace = pData->m_lstDisplace.RemoveHead();
					if (pDisplace)
					{
						//CDisplace_DataItem* pRecvData = new CDisplace_DataItem(pDisplace->GetRealTimeXValue(),pDisplace->GetRealTimeYValue(),pDisplace->GetDisplaceNo(),pData->mCollectTime);
						CDisplace_DataItem* pRecvData = new CDisplace_DataItem(pDisplace->GetRealTimeXValue(),pDisplace->GetRealTimeYValue(),pData->mCollectTime);
						CalcDisplace_RealTime(sRTUID,pDisplace->GetDisplaceNo(),pRecvData);
						_DELETE(pRecvData);
						_DELETE(pDisplace);
					}
				}
			}
			else if ( pData->mDataType == COneData_INFO::TEMPERA_DATA ||
				(pData->mDataType == COneData_INFO::ALARM_RECORD && pData->mAlarmType == COneData_INFO::ALARM_TEMPERA))
			{
				//位移数据实时统计
				if (!pData->m_lstTempera.IsEmpty())
				{
					CTempera_PARA_PER* pTempera = pData->m_lstTempera.RemoveHead();
					if (pTempera)
					{
						CTempera_DataItem* pRecvData = new CTempera_DataItem(pTempera->GetRealTimeValue(),pData->mCollectTime);
						CalcTempera_RealTime(sRTUID,pRecvData);
						_DELETE(pRecvData);
						_DELETE(pTempera);
					}
				}
			}
			else if (pData->mDataType == COneData_INFO::FLOW_V_DATA || (pData->mDataType == COneData_INFO::ALARM_RECORD))
			{
				//流量数据实时统计
				if (!pData->m_lstFlow.IsEmpty())
				{
					CFlowInitValume_PARA_PER* pFlow = pData->m_lstFlow.RemoveHead();
					if (pFlow)
					{
						CFlow_DataItem* pRecvData = new CFlow_DataItem((double)pFlow->GetInsFlowValume()/1000.0,(double)pFlow->GetCalTakeValume()/10.0, pData->mCollectTime);
						CFlow_RealTime(sRTUID, pRecvData);

						_DELETE(pRecvData);
						_DELETE(pFlow);
					}
				}
			}
		}
		//----------------------
		_DELETE(pData);
	}
}


long CDataTranSQLServer_SC::ThreadFunction()
{
/*
	Initialization();

	SetThreadControl(true);
	SetThreadState(THREAD_STATE_RUNNING);
	while (GetThreadControl() != false)
	{		
		StatisticsRain_1Day_OnTimer();

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
				if (SQLServer_Database == m_nDatabaseType)
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
	*/

	return 0;
}

void CDataTranSQLServer_SC::SetDataSource( CString strSource )
{
	m_strDataSource = strSource;
}

bool CDataTranSQLServer_SC::OpenDatabase()
{
	return OpenDatabase(m_strDataSource);
}

bool CDataTranSQLServer_SC::OpenDatabase( CString dataSource )
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

bool CDataTranSQLServer_SC::GetConnectionState()
{
	if (!m_Database.IsOpen())
		return false;
	try
	{
		if (SQLServer_Database == m_nDatabaseType)
		{
			CString strSQL="--";
			m_Database.ExecuteSQL(strSQL);
		}
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

bool CDataTranSQLServer_SC::CheckTableExist( CString sTableName )
{
	bool ret = true;
	CString sLog ="",sSQL ="";
	try
	{
		sSQL.Format("SELECT COUNT(*) AS ncount FROM sysobjects WHERE name='%s' AND type='U'",sTableName);
		CRecordset rs(&m_Database);

		rs.Open(AFX_DB_USE_DEFAULT_TYPE,sSQL);

		int nCount = 0;
		if (! rs.IsEOF())
		{
			CString sTemp = "";
			rs.GetFieldValue("ncount",sTemp);
			nCount = atoi(sTemp);
		}

		rs.Close();

		if (nCount == 0 )
			ret = false;
		else
			ret = true;
	}
	catch (CDBException* e)
	{
		sLog.Format("执行SQL:%s,Error:%s",sSQL,e->m_strError);
		e->Delete();
		AddThreadLog(sLog,Error_Log_message);
		ret = false;
	}

	return ret;
}

bool CDataTranSQLServer_SC::ExcuteNoQuery( CString sSQL )
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

		m_Database.Close();
		OpenDatabase();
		//m_Database.ExecuteSQL(sSQL);

		ret = false;
	}

	return ret;
}

void CDataTranSQLServer_SC::CheckDatabase()
{
	int nCount = 7;
	CString checkTables[]=
	{
		"ST_STBPRP_B",
		"ST_PPTN_R",
		"ST_STORM_R",
		"ST_RIVER_R",
		"ST_RSVR_R",
		"ST_DISP_R",
		"ST_TEMP_R"
	};
	CString checkTable_Names[]=
	{
		"测站基本属性表",
		"降水量表",
		"暴雨加报表",
		"河道水情表",
		"水库水情表",
		"深度位移表",
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

//bool CDataTranSQLServer_SC::OpenSqlite()
//{
//	bool ret = false;
//	/*
//	try
//	{
//		CCMSApp* pApp = (CCMSApp*)AfxGetApp();
//		CString SqliteFile = pApp->m_GlobalOptions.m_strLocalDbName;
//
//		CHAR OLD_PATH[MAX_PATH];
//		memset(OLD_PATH,0,sizeof(OLD_PATH));
//		::GetCurrentDirectory(MAX_PATH,OLD_PATH);
//		::SetCurrentDirectory(CUdfGlobalFUNC::GetRuningPath());
//
//		ret = m_sqlite.Open((LPCTSTR)SqliteFile);	
//		::SetCurrentDirectory(OLD_PATH);
//
//		if (ret == false)
//			AddThreadLog("连接本地数据库失败.",Error_Log_message);
//	}
//	catch (...)
//	{
//		ret = false;
//	}*/
//
//
//	return ret;
//}

CSqlStatement* CDataTranSQLServer_SC::ExcuteSQLiteQuery( CString strSQL )
{
	CSqlStatement* stmt =NULL;

#ifdef DEBUG
	SYSTEMTIME dbtime;
	TRACE("SQLite数据库操作:%s\r\n",strSQL);
	GetLocalTime(&dbtime);
	TRACE("\t\tExecute Begin: %s\r\n",
		CSystemTimeOp::ConvertSystemTimeToString(&dbtime,true));
#endif

	try
	{

		if (m_local_sqlite != NULL)
			stmt = m_local_sqlite->Statement(strSQL);
		//stmt = m_sqlite.Statement(strSQL);
	}
	catch (CException* e)
	{
		e->Delete();
		stmt = NULL;
	}

#ifdef DEBUG
	//TRACE("SQLite数据库操作:%s\r\n",strSQL);
	GetLocalTime(&dbtime);
	TRACE("\t\tExecute end: %s\r\n",
		CSystemTimeOp::ConvertSystemTimeToString(&dbtime,true));
#endif

	return stmt;	
}

bool CDataTranSQLServer_SC::InitAddressMap()
{
	CString strLog = "";
	bool ret = true;
	try
	{
		CString sheetName="地址映射表";
		CDatabase database;
		CString sDriver = _T("MICROSOFT EXCEL DRIVER (*.XLS)"); // exactly the same name as in the ODBC-Manager
		CString sExcelFile =  ""; 
		CString sSql = "";
		sExcelFile.Format("%s\\地址映射.xls",CUdfGlobalFUNC::GetRuningPath());
		
		sSql.Format(_T("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s"),
			sDriver, sExcelFile, sExcelFile);

		//Open database (i.e. Excel sheet)
		if( database.OpenEx(sSql,CDatabase::noOdbcDialog) )
		{
			CRecordset rs(&database);
			sSql.Format(_T("SELECT 测站编码,类型,水位站类型,行政区划码,终端地址,传感器序号 FROM [%s$]"),sheetName);
			rs.Open(AFX_DB_USE_DEFAULT_TYPE,sSql);
		
			CString sTemp = "";
			CString sSTCD="",sADDVCode="";
			int     nType = 0, nwlType = 0,nRtuAddr = 0,nRtuNo = 0;

			while (!rs.IsEOF())
			{
				rs.GetFieldValue("测站编码",sTemp);
				TRACE("%s\r\n",sTemp);
				sSTCD = sTemp.Trim();

				rs.GetFieldValue("类型",sTemp);
				TRACE("%s\r\n",sTemp);
				nType = atoi(sTemp);

				rs.GetFieldValue("水位站类型",sTemp);
				TRACE("%s\r\n",sTemp);
				nwlType = atoi(sTemp);

				rs.GetFieldValue("行政区划码",sTemp);
				TRACE("%s\r\n",sTemp);
				sADDVCode.Format("%d",atoi(sTemp));

				rs.GetFieldValue("终端地址",sTemp);
				TRACE("%s\r\n",sTemp);
				nRtuAddr = atoi(sTemp);

				rs.GetFieldValue("传感器序号",sTemp);
				TRACE("%s\r\n",sTemp);
				nRtuNo = atoi(sTemp);

				//std::map<CString,CAddrMap_Item*>::iterator findItr = m_mapAddressMap.end();
				//findItr  = m_mapAddressMap.find(sSTCD);
				//if (findItr != m_mapAddressMap.end())
				//{
				//	strLog.Format("读取地址映射表,警告:测站编码'%s'有重复.",sSTCD);
				//	AddThreadLog(strLog,Warning_log_Message);
				//}
				//else
				//{
				//	CAddrMap_Item* pAddr = new CAddrMap_Item(sSTCD,nType,nwlType,sADDVCode,nRtuAddr);
				//	m_mapAddressMap.insert(std::pair<CString,CAddrMap_Item*>(sSTCD,pAddr));
				//}


				CAddrMap_Item* pAddr = new CAddrMap_Item(sSTCD,nType,nwlType,sADDVCode,nRtuAddr,nRtuNo);
				m_mapAddressMap.push_back(pAddr);

				rs.MoveNext();
			}
			rs.Close();
		}

		strLog.Format("读取地址映射表结束,Rows:%d",m_mapAddressMap.size());
		AddThreadLog(strLog);

		if (database.IsOpen())
			database.Close();
	}
	catch (CDBException* e)
	{
		strLog.Format("读取地址映射表,错误:%s",e->m_strError);
		AddThreadLog(strLog,Warning_log_Message);

		e->Delete();
		ret = false;
	}
	return true;
}

void CDataTranSQLServer_SC::ClearAddrMap()
{
	if (m_mapAddressMap.size() > 0)
	{
		std::list<CAddrMap_Item*>::iterator itr = m_mapAddressMap.end();
		for (itr = m_mapAddressMap.begin();
			 itr != m_mapAddressMap.end();
			 itr ++)
		{

			if ((*itr) != NULL)
			{
				CAddrMap_Item* pObj = (*itr);
				_DELETE(pObj);
			}
		}

		m_mapAddressMap.clear();
	}
}

void CDataTranSQLServer_SC::CheckAddressMap_ForRtu()
{
	CString strLog= "";
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	if (! pApp->m_pReadDB)
		return ;
	
	//strLog= "检查地址映射表与遥测终端机映射的正确性...";
	//AddThreadLog(strLog);

	if (m_mapAddressMap.size() > 0 )
	{
		//std::map<CString,CAddrMap_Item*>::iterator Itr;
		std::list<CAddrMap_Item*>::iterator Itr = m_mapAddressMap.end();
		for (Itr = m_mapAddressMap.begin();Itr != m_mapAddressMap.end();Itr++)
		{
			//if (Itr->second != NULL)
			CAddrMap_Item* pAddrItem = (*Itr);
			if (pAddrItem != NULL)
			{
				bool blFinded = false;
				int ncount = pApp->m_pReadDB->GetRtuStationCount();
				for (int idx = 0; idx < ncount; idx++)
				{
					CRtuStation* pStation = pApp->m_pReadDB->GetRtuStation(idx);
					if (pStation)
					{
						CString sRTUID = "";
						sRTUID.Format("%02d%02d%02d_%08d",pStation->GetProvinceCode(),pStation->GetRegionCode(),pStation->GetCountyCode(),pStation->GetAddrA2());
						if (sRTUID == pAddrItem->Get_RTU_ID())
						{
							blFinded = true;
							break;
						}
					}
				}
				if (false == blFinded)
				{
					strLog.Format("地址映射检查,警告:测站编码'%s'没有与之对应的遥测终端机(行政区划码:%s,终端地址:%d)",
						pAddrItem->Get_STCD(),
						pAddrItem->Get_ADDVCode(),
						pAddrItem->Get_RtuAddr());
					AddThreadLog(strLog,Warning_log_Message);
				}
			}
		}
	}
}

void CDataTranSQLServer_SC::CheckAddressMap_ForMaster()
{
}

CWaterL_DataItem* CDataTranSQLServer_SC::GetLatestWaterLevelData( CAddrMap_Item* pAddrItem,CString strPoint)
{
	CWaterL_DataItem* pWaterL = NULL;
	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_WL && strPoint != "")
	{
		SYSTEMTIME curTime ;
		CSystemTimeOp::ConvertStringToSysemTime(strPoint,&curTime);

		CTime nowTime(curTime);
		CTimeSpan addhour(0,1,0,0);
		CTime endtime = nowTime + addhour;
		
		CString strBegin="", strEnd = "";
		strBegin.Format("%04d-%02d-%02d %02d:00:01.000",
			nowTime.GetYear(),nowTime.GetMonth(),nowTime.GetDay(),nowTime.GetHour());

		//strEnd.Format("%04d-%02d-%02d %02d:00:00.000",
		//	endtime.GetYear(),endtime.GetMonth(),endtime.GetDay(),endtime.GetHour());
		strEnd.Format("%04d-%02d-%02d %02d:00:00.999",
			endtime.GetYear(),endtime.GetMonth(),endtime.GetDay(),endtime.GetHour());

#ifdef DEBUG
		SYSTEMTIME debugtime;
		GetLocalTime(&debugtime);
		TRACE("%s 获取小时水位数据, POINT: %s ,[%s ~ %s]\r\n",
			CSystemTimeOp::ConvertSystemTimeToString(&debugtime,true),strPoint,strBegin,strEnd);
#endif

		CString sSQL = "";
		sSQL.Format("SELECT DISTINCT CURRENT_VAL,COLLECT_TIME FROM WATERL_DATA WHERE RTU='%s' AND SENSORS_NO='00' AND COLLECT_TIME  BETWEEN '%s' AND '%s' ORDER BY COLLECT_TIME DESC LIMIT 1",
			pAddrItem->Get_RTU_ID(),strBegin,strEnd);

		CSqlStatement* stmt = ExcuteSQLiteQuery(sSQL);
#ifdef DEBUG
		GetLocalTime(&debugtime);
		TRACE("Begin read: %s\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugtime,true));
#endif
		if (stmt != NULL)
		{
			int Rows = 0;    //rows
			double dValue = 0.0;
			CString sCollectTime = "",sTemp="";
			if (stmt->NextRow())
			{
				sTemp= stmt->ValueString(0);
				dValue = atof(sTemp);

				sTemp = stmt->ValueString(1);
				sCollectTime = sTemp.Trim();

				Rows ++;
			}

			delete stmt;
			stmt = NULL;

			if (Rows > 0)
			{
				SYSTEMTIME collectTime;
				CSystemTimeOp::ConvertStringToSysemTime(sCollectTime,&collectTime);

				pWaterL = new CWaterL_DataItem(dValue,collectTime);

#ifdef DEBUG
				GetLocalTime(&debugtime);
				TRACE("%s 获取小时水位数据,%s=%f\r\n",
					CSystemTimeOp::ConvertSystemTimeToString(&debugtime,true),sCollectTime,dValue);
#endif
			}
		}
	}

	return pWaterL;
}


//水位数据统计
void CDataTranSQLServer_SC::CalcWaterLevel_RealTime( CString Rtu_ID,CWaterL_DataItem* pRecvData )
{
	if (Rtu_ID != "" && pRecvData != NULL)
	{
		std::list<CAddrMap_Item*>::iterator Itr = m_mapAddressMap.end();
		for (Itr = m_mapAddressMap.begin(); 
			Itr != m_mapAddressMap.end();
			Itr ++)
		{

			CAddrMap_Item* pAddrItem = (*Itr);

			if ( pAddrItem != NULL &&
				pAddrItem->GetType() == CZ_Type_WL &&
				pAddrItem->Get_RTU_ID() == Rtu_ID)
			{ 
				if(m_nWaterLevaeIType == W_L_IType_SBK)
				{
					//OLD
					//StatisticsWaterLevel_RealTime_SBK(pAddrItem,pRecvData);

					//NEW1
					//StatisticsWaterLevel_Optimize1_RealTime_SBK(pAddrItem,pRecvData);

					//New2 
					StatisticsWaterLevel_Optimize2_Enter(pAddrItem,pRecvData);
				}
				else//(m_nWaterLevaeIType == W_L_IType_Default)
					StatisticsWaterLevel_RealTime_Default(pAddrItem,pRecvData);
				
			}
		}
	}
}

//某水位监测点数据实时统计
void CDataTranSQLServer_SC::StatisticsWaterLevel_RealTime_Default( CAddrMap_Item* pAddrItem,CWaterL_DataItem* pRecvData )
{
	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_WL  && pRecvData != NULL)
	{
		//检测是否是整点数据
		if (pRecvData->m_RecordTime.wMinute == 0 && 
			pRecvData->m_RecordTime.wSecond == 0)
		{
			//更新整点数据
			CString strTM="";
			strTM.Format("%04d-%02d-%02d %02d:00",
				         pRecvData->m_RecordTime.wYear,pRecvData->m_RecordTime.wMonth,
						 pRecvData->m_RecordTime.wDay,pRecvData->m_RecordTime.wHour);

			//new 20140816
			CString strFTM = strTM + ":00.000";
			SYSTEMTIME FTM ;
			CSystemTimeOp::ConvertStringToSysemTime(strFTM,&FTM);
			int comp = CSystemTimeOp::CompareSystemTime(&FTM);
			if (comp <= 0)
				UpdateWaterLevelData(pAddrItem,strTM,pRecvData->m_dValue);
			else
			{
				Table_ST_WL_R_Record record;
				record.STCD = pAddrItem->Get_STCD();
				record.TYPE = (int)pAddrItem->GetWlType();
				record.TM = strTM;
				record.FTM = FTM;
				record.PV = pRecvData->m_dValue;
				GetLocalTime(&(record.CAL_TM));

				pAddrItem->UpdateST_WL_R_Record(&record);
			}

			//old
			//UpdateWaterLevelData(pAddrItem,strTM,pRecvData->m_dValue);
		}
		else
		{
			//更新当前点
			CString strTM="";
			strTM.Format("%04d-%02d-%02d %02d:%02d",
				pRecvData->m_RecordTime.wYear,pRecvData->m_RecordTime.wMonth,
				pRecvData->m_RecordTime.wDay,pRecvData->m_RecordTime.wHour,pRecvData->m_RecordTime.wMinute);

			//New 20140816
			CString strFTM = strTM + ":00.000";
			SYSTEMTIME FTM;
			CSystemTimeOp::ConvertStringToSysemTime(strFTM,&FTM);
			int comp = CSystemTimeOp::CompareSystemTime(&FTM);
			if (comp <= 0)
				UpdateWaterLevelData(pAddrItem,strTM,pRecvData->m_dValue);
			else
			{
				Table_ST_WL_R_Record record;
				record.STCD = pAddrItem->Get_STCD();
				record.TYPE = (int)pAddrItem->GetWlType();
				record.TM = strTM;
				record.FTM = FTM;
				record.PV = pRecvData->m_dValue;
				GetLocalTime(&(record.CAL_TM));

				pAddrItem->UpdateST_WL_R_Record(&record);
			}

			//OLD
			//UpdateWaterLevelData(pAddrItem,strTM,pRecvData->m_dValue);


			//更新整点数据
			CTime nowTime(pRecvData->m_RecordTime);
			CTimeSpan ttspan(0,1,0,0);
			CTime HourTime = nowTime + ttspan;
			double HourValue = 0.0;

			strTM.Format("%04d-%02d-%02d %02d:00",
				HourTime.GetYear(),HourTime.GetMonth(),HourTime.GetDay(),
				HourTime.GetHour());

			//new 20140816
			strFTM = strTM + ":00.000";
			CSystemTimeOp::ConvertStringToSysemTime(strFTM,&FTM);
		    comp = CSystemTimeOp::CompareSystemTime(&FTM);

			CWaterL_DataItem* HourWL = GetLatestWaterLevelData(pAddrItem,CSystemTimeOp::ConvertSystemTimeToString(&(pRecvData->m_RecordTime),true));
			if (HourWL != NULL)
			{
				INT64  muls = CSystemTimeOp::MinusSystemTime(HourWL->m_RecordTime,pRecvData->m_RecordTime);
			
				if (muls > 0 )   //查到的值比当前值新
					HourValue = HourWL->m_dValue;
				else
					HourValue = pRecvData->m_dValue;

				//new 20140816
				if (comp <= 0)
					UpdateWaterLevelData(pAddrItem,strTM,HourValue);
				else
				{
					Table_ST_WL_R_Record record;
					record.STCD = pAddrItem->Get_STCD();
					record.TYPE = (int)pAddrItem->GetWlType();
					record.TM = strTM;
					record.FTM = FTM;
					record.PV = HourValue;
					GetLocalTime(&(record.CAL_TM));

					pAddrItem->UpdateST_WL_R_Record(&record);
				}

				//Old
				//UpdateWaterLevelData(pAddrItem,strTM,HourValue);

				_DELETE(HourWL);
			}
			else if (HourWL == NULL)
			{
				HourValue = pRecvData->m_dValue;

				//New 20140816
				if (comp <= 0)
					UpdateWaterLevelData(pAddrItem,strTM,HourValue);
				else
				{
					Table_ST_WL_R_Record record;
					record.STCD = pAddrItem->Get_STCD();
					record.TYPE = (int)pAddrItem->GetWlType();
					record.TM = strTM;
					record.FTM = FTM;
					record.PV = HourValue;
					GetLocalTime(&(record.CAL_TM));

					pAddrItem->UpdateST_WL_R_Record(&record);
				}

				//Old
				//UpdateWaterLevelData(pAddrItem,strTM,HourValue);
			}
		}
	}
}


//某水位监测点数据实时统计
void CDataTranSQLServer_SC::StatisticsWaterLevel_RealTime_SBK( CAddrMap_Item* pAddrItem,CWaterL_DataItem* pRecvData )
{
#ifdef DEBUG
	SYSTEMTIME debugst;
#endif

	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_WL  && pRecvData != NULL)
	{
		//检测是否是整点数据
		if (pRecvData->m_RecordTime.wMinute == 0 && 
			pRecvData->m_RecordTime.wSecond == 0)
		{
			//更新整点数据
			CString strTM="";
			strTM.Format("%04d-%02d-%02d %02d:00",
				pRecvData->m_RecordTime.wYear,pRecvData->m_RecordTime.wMonth,
				pRecvData->m_RecordTime.wDay,pRecvData->m_RecordTime.wHour);

			//new 20140816
			int ret= 0;
			CString strFTM = strTM + ":00.000";
			SYSTEMTIME FTM;
			CSystemTimeOp::ConvertStringToSysemTime(strFTM,&FTM);
			int comp = CSystemTimeOp::CompareSystemTime(&FTM);
			if (comp <= 0)
			{
				ret = UpdateWaterLevelData(pAddrItem,strTM,pRecvData->m_dValue);
			}
			else
			{
				Table_ST_WL_R_Record record;
				record.STCD = pAddrItem->Get_STCD();
				record.TYPE = (int)pAddrItem->GetWlType();
				record.TM = strTM;
				record.FTM = FTM;
				record.PV = pRecvData->m_dValue;
				GetLocalTime(&(record.CAL_TM));

				ret = pAddrItem->UpdateST_WL_R_Record(&record);
			}

			//old
			//UpdateWaterLevelData(pAddrItem,strTM,pRecvData->m_dValue);



			//后1个小时的加报数据统计
			if (ret == 1 || 2 == ret)
			{
				SYSTEMTIME nexthour_begin ,nexthour_end;
				CString temp = strTM + ":01.000";
				CSystemTimeOp::ConvertStringToSysemTime(temp,&nexthour_begin);
	
				temp.Format("%04d-%02d-%02d %02d:59:59.999",nexthour_begin.wYear,nexthour_begin.wMonth,nexthour_begin.wDay,nexthour_begin.wHour);
				CSystemTimeOp::ConvertStringToSysemTime(temp,&nexthour_end);

				SYSTEMTIME curST;
				GetLocalTime(&curST);
				int comp = CSystemTimeOp::MinusSystemTime(nexthour_begin,curST);
#ifdef DEBUG
				GetLocalTime(&debugst);
				TRACE("%s Next Hour: %s - %s \r\n",
					CSystemTimeOp::ConvertSystemTimeToString(&debugst,true),
					CSystemTimeOp::ConvertSystemTimeToString(&nexthour_begin,true),
					CSystemTimeOp::ConvertSystemTimeToString(&nexthour_end,true));
#endif
				if (comp <= 0)
				{
				    StatisticsWaterLevel_Hour_AppendReport(pAddrItem,nexthour_begin,nexthour_end);
				}
			}

		}
		else
		{
			//更新当前点
			CString strTM="";
			strTM.Format("%04d-%02d-%02d %02d:%02d",
				pRecvData->m_RecordTime.wYear,pRecvData->m_RecordTime.wMonth,
				pRecvData->m_RecordTime.wDay,pRecvData->m_RecordTime.wHour,pRecvData->m_RecordTime.wMinute);
			//UpdateWaterLevelData(pAddrItem,strTM,pRecvData->m_dValue);
			
			//从接口数据库里获取基准点的数据
			//未找到基准点将不存储数据
            CWaterL_DataItem* pWData = GetWaterLevelBasePoint(pAddrItem,pRecvData);
			//if ( (pWData != NULL &&  (int)(fabs(pWData->m_dValue * 100 - pRecvData->m_dValue * 100)) >= 5))
			if ( pWData != NULL &&  CUdfGlobalFUNC::CompareDoubleChange(pWData->m_dValue,pRecvData->m_dValue,(double)0.05,7,2) >= 0)
			{
				//new 20140816
				CString strFTM = strTM + ":00.000";
				SYSTEMTIME FTM;
				CSystemTimeOp::ConvertStringToSysemTime(strFTM,&FTM);
				int comp = CSystemTimeOp::CompareSystemTime(&FTM);
				if (comp <= 0)
					UpdateWaterLevelData(pAddrItem,strTM,pRecvData->m_dValue);
				else
				{
					Table_ST_WL_R_Record record;
					record.STCD = pAddrItem->Get_STCD();
					record.TYPE = (int)pAddrItem->GetWlType();
					record.TM = strTM;
					record.FTM = FTM;
					record.PV = pRecvData->m_dValue;
					GetLocalTime(&(record.CAL_TM));

					pAddrItem->UpdateST_WL_R_Record(&record);
				}

				//Old
				//UpdateWaterLevelData(pAddrItem,strTM,pRecvData->m_dValue);
			}
			_DELETE(pWData);


			//检查是否为补报数据并进行修复
			//对一个小时内的数据重新进行修改入库
			//added by 2012-12-04
			//------------------------------------------------------
			try
			{
				CList<CWaterL_DataItem*> tempList ;
				
				CString sSQL = "",_strBegin="",_strEnd="";
				_strBegin = CSystemTimeOp::ConvertSystemTimeToString(&(pRecvData->m_RecordTime),true);
				_strEnd.Format("%04d-%02d-%02d %02d:59:59.999",pRecvData->m_RecordTime.wYear,pRecvData->m_RecordTime.wMonth,pRecvData->m_RecordTime.wDay,pRecvData->m_RecordTime.wHour);
				sSQL.Format("SELECT DISTINCT CURRENT_VAL,COLLECT_TIME FROM WATERL_DATA WHERE RTU='%s' AND SENSORS_NO='00' AND COLLECT_TIME  > '%s' AND COLLECT_TIME < '%s' ORDER BY COLLECT_TIME",
					pAddrItem->Get_RTU_ID(),_strBegin,_strEnd);

				CSqlStatement* stmt = ExcuteSQLiteQuery(sSQL);
#ifdef DEBUG
				GetLocalTime(&debugst);
				TRACE("Begin Read: %s \r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
#endif
				if (stmt != NULL)
				{
					//int Rows = 0;    //rows
					double dValue = 0.0;
					CString sCollectTime = "",sTemp="";
					while (stmt->NextRow())
					{
						sTemp= stmt->ValueString(0);
						dValue = atof(sTemp);

						sTemp = stmt->ValueString(1);
						sCollectTime = sTemp.Trim();

						SYSTEMTIME collectTime;
						CSystemTimeOp::ConvertStringToSysemTime(sCollectTime,&collectTime);

						CWaterL_DataItem* pTempW = new CWaterL_DataItem(dValue,collectTime);
						if (pTempW != NULL)
							tempList.AddTail(pTempW);
					}

					delete stmt;
					stmt = NULL;
				}
#ifdef DEBUG
				GetLocalTime(&debugst);
				TRACE("End Read: %s \r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
#endif

				if (!tempList.IsEmpty())
				{
					if (pAddrItem->GetWlType() == WL_Type_HLSW)
					{
						if (m_nDatabaseType == SQLServer_Database)
							sSQL.Format("DELETE FROM ST_RIVER_R WHERE TM > '%s' AND TM < '%s' AND STCD='%s'",_strBegin,_strEnd,pAddrItem->Get_STCD());
						else if (m_nDatabaseType == Oracle_Database)
							sSQL.Format("DELETE FROM ST_RIVER_R WHERE TM > to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3') AND TM < to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3') AND STCD='%s'",_strBegin,_strEnd,pAddrItem->Get_STCD());
					}
					else
					{
						if (m_nDatabaseType == SQLServer_Database)
							sSQL.Format("DELETE FROM ST_RSVR_R WHERE TM > '%s' AND TM < '%s' AND STCD='%s'",_strBegin,_strEnd,pAddrItem->Get_STCD());
						else if (m_nDatabaseType == Oracle_Database)
							sSQL.Format("DELETE FROM ST_RSVR_R WHERE TM > to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3') AND TM < to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3') AND STCD='%s'",_strBegin,_strEnd,pAddrItem->Get_STCD());
					}
					ExcuteNoQuery(sSQL);

					//new 20140816
					pAddrItem->RemoveST_WL_R_Record(_strBegin,_strEnd);

					//更新补报点
					while(tempList.IsEmpty() != TRUE)
					{
						CWaterL_DataItem* pTemp = tempList.RemoveHead();
						if (pTemp != NULL)
						{
							CString _tempStr = "";
							_tempStr.Format("%04d-%02d-%02d %02d:%02d",
								pTemp->m_RecordTime.wYear,pTemp->m_RecordTime.wMonth,
								pTemp->m_RecordTime.wDay,pTemp->m_RecordTime.wHour,pTemp->m_RecordTime.wMinute);

							CWaterL_DataItem* pBasic= GetWaterLevelBasePoint(pAddrItem,pTemp);
							//if (pBasic != NULL && (int)(fabs(pTemp->m_dValue * 100 - pBasic->m_dValue * 100)) >= 5)
							if (pBasic != NULL && CUdfGlobalFUNC::CompareDoubleChange(pTemp->m_dValue,pBasic->m_dValue,(double)0.05,7,2) >= 0)
							{
								CString strFTM = _tempStr + ":00.000";
								SYSTEMTIME FTM ;
								CSystemTimeOp::ConvertStringToSysemTime(strFTM,&FTM);
								int comp = CSystemTimeOp::CompareSystemTime(&FTM);
								if (comp <= 0)
									UpdateWaterLevelData(pAddrItem,_tempStr,pTemp->m_dValue);
								else
								{
									Table_ST_WL_R_Record record;
									record.STCD = pAddrItem->Get_STCD();
									record.TYPE = (int)pAddrItem->GetWlType();
									record.TM = _tempStr;
									record.FTM = FTM;
									record.PV = pTemp->m_dValue;
									GetLocalTime(&(record.CAL_TM));

									pAddrItem->UpdateST_WL_R_Record(&record);
								}
								
								//Old
								//UpdateWaterLevelData(pAddrItem,_tempStr,pTemp->m_dValue);
							}

							_DELETE(pBasic);
							_DELETE(pTemp);
						}
					}
				}
			}
			catch(...)
			{
			}
			//------------------------------------------------------


			//更新整点数据
			CTime nowTime(pRecvData->m_RecordTime);
			CTimeSpan ttspan(0,1,0,0);
			CTime HourTime = nowTime + ttspan;
			double HourValue = 0.0;

			strTM.Format("%04d-%02d-%02d %02d:00",
				HourTime.GetYear(),HourTime.GetMonth(),HourTime.GetDay(),
				HourTime.GetHour());

			CString strFTM = strTM + ":00.000";
			SYSTEMTIME FTM;
			CSystemTimeOp::ConvertStringToSysemTime(strFTM,&FTM);
			int comp = CSystemTimeOp::CompareSystemTime(&FTM);

			CWaterL_DataItem* HourWL = GetLatestWaterLevelData(pAddrItem,CSystemTimeOp::ConvertSystemTimeToString(&(pRecvData->m_RecordTime),true));
			if (HourWL != NULL)
			{
				INT64  muls = CSystemTimeOp::MinusSystemTime(HourWL->m_RecordTime,pRecvData->m_RecordTime);

				if (muls > 0 )   //查到的值比当前值新 (不需要更新库，之前已经存库了)
					HourValue = HourWL->m_dValue;
				else  // <= 0 的情况 即查找到的数据都没有当前的新
				{
					HourValue = pRecvData->m_dValue;

					//new 20140816
					int ret = 0;
					if (comp <= 0)
					{
						ret = UpdateWaterLevelData(pAddrItem,strTM,HourValue);
					}
					else
					{
						Table_ST_WL_R_Record record;
						record.STCD = pAddrItem->Get_STCD();
						record.TYPE = (int)pAddrItem->GetWlType();
						record.TM = strTM;
						record.FTM = FTM;
						record.PV = HourValue;
						GetLocalTime(&(record.CAL_TM));

						ret = pAddrItem->UpdateST_WL_R_Record(&record);
					}


					//后1个小时的加报数据统计
					if (ret == 1 || ret == 2)
					{
						SYSTEMTIME nexthour_begin ,nexthour_end;
						CString temp = strTM + ":01.000";
						CSystemTimeOp::ConvertStringToSysemTime(temp,&nexthour_begin);

						temp.Format("%04d-%02d-%02d %02d:59:59.999",nexthour_begin.wYear,nexthour_begin.wMonth,nexthour_begin.wDay,nexthour_begin.wHour);
						CSystemTimeOp::ConvertStringToSysemTime(temp,&nexthour_end);

						SYSTEMTIME curST;
						GetLocalTime(&curST);
						int comp = CSystemTimeOp::MinusSystemTime(nexthour_begin,curST);
#ifdef DEBUG
						GetLocalTime(&debugst);
						TRACE("%s Next Hour: %s - %s \r\n",
							CSystemTimeOp::ConvertSystemTimeToString(&debugst,true),
							CSystemTimeOp::ConvertSystemTimeToString(&nexthour_begin,true),
							CSystemTimeOp::ConvertSystemTimeToString(&nexthour_end,true));
#endif
						if (comp <= 0)
						{
							StatisticsWaterLevel_Hour_AppendReport(pAddrItem,nexthour_begin,nexthour_end);
						}
					}

				}

				//old
				//UpdateWaterLevelData(pAddrItem,strTM,HourValue);

				_DELETE(HourWL);
			}
			else if (HourWL == NULL)
			{
				HourValue = pRecvData->m_dValue;
				
				//new 20140816
				if (comp <= 0)
					UpdateWaterLevelData(pAddrItem,strTM,HourValue);
				else
				{
					Table_ST_WL_R_Record record;
					record.STCD = pAddrItem->Get_STCD();
					record.TYPE = (int)pAddrItem->GetWlType();
					record.TM = strTM;
					record.FTM = FTM;
					record.PV = HourValue;
					GetLocalTime(&(record.CAL_TM));

					pAddrItem->UpdateST_WL_R_Record(&record);
				}


				//old
				//UpdateWaterLevelData(pAddrItem,strTM,HourValue);
			}
		}
	}
}


CWaterL_DataItem* CDataTranSQLServer_SC::GetWaterLevelBasePoint( CAddrMap_Item* pAddrItem, CWaterL_DataItem* pRecvData )
{
	CWaterL_DataItem* pResult = NULL;
	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_WL && pRecvData != NULL)
	{
		CString strBegin= "",strEnd = "";
		strBegin.Format("%04d-%02d-%02d %02d:00:00.000",
			pRecvData->m_RecordTime.wYear,
			pRecvData->m_RecordTime.wMonth,
			pRecvData->m_RecordTime.wDay,
			pRecvData->m_RecordTime.wHour);

		strEnd = CSystemTimeOp::ConvertSystemTimeToString(&(pRecvData->m_RecordTime),true);

		CString sSQL = "";
		if (pAddrItem->GetWlType() == WL_Type_HLSW)
		{
			if (m_nDatabaseType == SQLServer_Database)
			{
				sSQL.Format("SELECT TOP 1 Z AS L,TM  AS T FROM ST_RIVER_R WHERE STCD='%s' AND TM BETWEEN '%s' and '%s' ORDER BY TM DESC",pAddrItem->Get_STCD(),strBegin,strEnd);
			}
			else if (m_nDatabaseType == Oracle_Database)
			{
				sSQL.Format("SELECT Z AS L,TO_CHAR(TM,'YYYY-MM-DD HH24:MI:SS.ff3') AS T FROM ST_RIVER_R WHERE STCD='%s' AND TM BETWEEN to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3') and to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3') ORDER BY TM DESC",pAddrItem->Get_STCD(),strBegin,strEnd);
			}
		}
		else if (pAddrItem->GetWlType() == WL_Type_SKSSW)
		{
			if (m_nDatabaseType == SQLServer_Database)
			{
				sSQL.Format("SELECT TOP 1 RZ AS L,TM AS T FROM ST_RSVR_R WHERE STCD = '%s' AND TM BETWEEN '%s' and '%s' ORDER BY TM DESC",
					pAddrItem->Get_STCD(),strBegin,strEnd);
			}
			else if (m_nDatabaseType == Oracle_Database)
			{
				sSQL.Format("SELECT RZ AS L,TO_CHAR(TM,'YYYY-MM-DD HH24:MI:SS.ff3') AS T FROM ST_RSVR_R WHERE STCD = '%s' AND TM BETWEEN to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3') and to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3') ORDER BY TM DESC",
					pAddrItem->Get_STCD(),strBegin,strEnd);
			}

		}
		else if (pAddrItem->GetWlType() == WL_Type_SKXSW)
		{
			if (m_nDatabaseType == SQLServer_Database)
			{
				sSQL.Format("SELECT TOP 1 BLRZ AS L,TM AS T FROM ST_RSVR_R WHERE STCD = '%s' AND TM BETWEEN '%s' and '%s' ORDER BY TM DESC",
					pAddrItem->Get_STCD(),strBegin,strEnd);
			}
			else if (m_nDatabaseType == Oracle_Database)
			{
				sSQL.Format("SELECT BLRZ AS L,TO_CHAR(TM,'YYYY-MM-DD HH24:MI:SS.ff3') AS T FROM ST_RSVR_R WHERE STCD = '%s' AND TM BETWEEN to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3') and to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3') ORDER BY TM DESC",
					pAddrItem->Get_STCD(),strBegin,strEnd);
			}
		}

		if (sSQL != "")
		{
			try
			{

#ifdef DEBUG
				SYSTEMTIME debugTime;
				GetLocalTime(&debugTime);
				TRACE("%s 查找水位数据,SQL:%s\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugTime,true),sSQL);
#endif
				CRecordset rs(&m_Database);

				rs.Open(AFX_DB_USE_DEFAULT_TYPE,sSQL);

				CString sTemp = "";
				if(! rs.IsEOF())
				{
					double WaterLevelData= 0.0;
					SYSTEMTIME st;

					rs.GetFieldValue("L",sTemp);
					WaterLevelData = atof(sTemp);

					rs.GetFieldValue("T",sTemp);
					CSystemTimeOp::ConvertStringToSysemTime(sTemp,&st);

					pResult = new CWaterL_DataItem(WaterLevelData,st);
#ifdef DEBUG
					GetLocalTime(&debugTime);
					TRACE("%s 查找到水位数据:%s\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugTime,true),pResult->ToString());
#endif
					
				}
				rs.Close();
			}
			catch (CDBException* e)
			{
				CString sLog = "";
				sLog.Format("执行SQL:%s,信息:%s",sSQL,e->m_strError);
				e->Delete();
				AddThreadLog(sLog,Warning_log_Message);
				_DELETE(pResult);

			}

		}
	}
	return pResult;
}
int CDataTranSQLServer_SC::UpdateFlowData1(CAddrMap_Item* pAddrItem,double value, CString strTM)
{
	int ret = 0;
	if (pAddrItem != NULL)
		//if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_WL && strTM != "" && pointValue > 0)
	{
		CString sSQL = "";
		bool isExist = false;       //Record whether exist
		double ValueInDb = 0.0;     //Data In Database
	
		if (m_nDatabaseType == SQLServer_Database) {
			sSQL.Format("UPDATE ST_RIVER_R SET Q = % .3f WHERE STCD = '%s' AND TM = '%s'",value, pAddrItem->Get_STCD(), strTM);
			ret = ExcuteNoQuery(sSQL);
		}
		
	}
	return ret;
}
int CDataTranSQLServer_SC::UpdateFlowData2(CAddrMap_Item* pAddrItem, double value, CString strTM)
{
	int ret = 0;
	if (pAddrItem != NULL)
		//if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_WL && strTM != "" && pointValue > 0)
	{
		CString sSQL = "";
		bool isExist = false;       //Record whether exist
		double ValueInDb = 0.0;     //Data In Database

		if (m_nDatabaseType == SQLServer_Database) {
			sSQL.Format("INSERT INTO ST_RSVR_R (STCD,TM, W) VALUES ('%s','%s',%.3f)",pAddrItem->Get_STCD(), strTM, value);
			//sSQL.Format("UPDATE ST_RSVR_R SET W = % .3f WHERE STCD = '%s' AND TM = '%s'",value1, pAddrItem->Get_STCD(), strTM);
			ret = ExcuteNoQuery(sSQL);
		}

	}
	return ret;
}
/*
* 更新 strTM点的数据
* 查找到数据 更新 Update
* 未查找到数据 添加 Insert
* 返回： 1: Insert  2: update  3: exist (not Update)
*/
//bool CDataTranSQLServer_SC::UpdateWaterLevelData( CAddrMap_Item* pAddrItem, CString strTM, double pointValue )
int  CDataTranSQLServer_SC::UpdateWaterLevelData( CAddrMap_Item* pAddrItem, CString strTM, double pointValue )
{
	//bool ret = true;
	int ret = 0;
	//change by yhp 20160722 如果采集的值为零也上写省标库
	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_WL && strTM != "" && pointValue >= 0.0)
	//if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_WL && strTM != "" && pointValue > 0)
	{
		CString sSQL = "";
		bool isExist = false;       //Record whether exist
		double ValueInDb = 0.0;     //Data In Database
		if (pAddrItem->GetWlType() == WL_Type_HLSW)
		{
			//查询
			if (m_nDatabaseType == SQLServer_Database)
			{
				sSQL.Format("SELECT Z AS L FROM ST_RIVER_R WHERE STCD='%s' AND TM = '%s'",
					pAddrItem->Get_STCD(),strTM);
			}
			else if (m_nDatabaseType == Oracle_Database)
			{
				sSQL.Format("SELECT Z AS L FROM ST_RIVER_R WHERE STCD='%s' AND TM = to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3')",
					pAddrItem->Get_STCD(),strTM);
			}


			isExist = GetWaterLevelData(sSQL,ValueInDb);

			if (false == isExist)     //INSERT
			{
				ret = 1;
				if (m_nDatabaseType == SQLServer_Database)
				{
					sSQL.Format("INSERT INTO ST_RIVER_R (STCD ,TM, Z, WPTN) VALUES ('%s','%s',%.2f,6)",
						pAddrItem->Get_STCD(),strTM,pointValue);
				}
				else if (m_nDatabaseType == Oracle_Database)
				{
					sSQL.Format("INSERT INTO ST_RIVER_R (STCD ,TM, Z, WPTN) VALUES ('%s',to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3'),%.2f,6)",
						pAddrItem->Get_STCD(),strTM,pointValue);
				}

				ret = ExcuteNoQuery(sSQL);
			}
			else if (true == isExist) //UPDATE
			{
				ret = 3;
				//if (ValueInDb != pointValue)
				if (fabs(ValueInDb - pointValue) >= 0.01 )
				{
					ret = 2;
#ifdef DEBUG
					TRACE("DB:%f\r\nPV:%f\r\n",ValueInDb,pointValue);
#endif

					if (m_nDatabaseType == SQLServer_Database)
					{
						sSQL.Format("UPDATE ST_RIVER_R SET Z = %.2f WHERE STCD='%s' AND TM = '%s'",
							pointValue,pAddrItem->Get_STCD(),strTM);
					}
					else if (m_nDatabaseType == Oracle_Database)
					{
						sSQL.Format("UPDATE ST_RIVER_R SET Z = %.2f WHERE STCD='%s' AND TM = to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3')",
							pointValue,pAddrItem->Get_STCD(),strTM);
					}


					ret = ExcuteNoQuery(sSQL);
				}
			}
		}
		else if (pAddrItem->GetWlType() == WL_Type_SKSSW)
		{
			//查询
			if (m_nDatabaseType == SQLServer_Database)
			{
				sSQL.Format("SELECT RZ AS L FROM ST_RSVR_R WHERE STCD = '%s' AND TM = '%s'",
					pAddrItem->Get_STCD(),strTM);
			}
			else if (m_nDatabaseType == Oracle_Database)
			{
				sSQL.Format("SELECT RZ AS L FROM ST_RSVR_R WHERE STCD = '%s' AND TM = to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3')",
					pAddrItem->Get_STCD(),strTM);
			}

			isExist = GetWaterLevelData(sSQL,ValueInDb);

			if (false == isExist)  //INSERT
			{
				ret = 1;

				if (m_nDatabaseType == SQLServer_Database)
				{
					sSQL.Format("INSERT INTO ST_RSVR_R (STCD,TM, RZ) VALUES ('%s','%s',%.2f)",
						pAddrItem->Get_STCD(),strTM,pointValue);
				}
				else if (m_nDatabaseType == Oracle_Database)
				{
					sSQL.Format("INSERT INTO ST_RSVR_R (STCD,TM, RZ) VALUES ('%s',to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3'),%.2f)",
						pAddrItem->Get_STCD(),strTM,pointValue);
				}


				ret = ExcuteNoQuery(sSQL);
			}
			else if (true == isExist) //UPDATE
			{
				ret = 3;
				//if (ValueInDb != pointValue)
				if (fabs(ValueInDb - pointValue) >= 0.01 )
				{
					ret = 2;
#ifdef DEBUG
					TRACE("DB:%f\r\nPV:%f\r\n",ValueInDb,pointValue);
#endif
					if (m_nDatabaseType == SQLServer_Database)
					{
						sSQL.Format("UPDATE ST_RSVR_R SET RZ = %.2f WHERE STCD='%s' AND TM = '%s'",
							pointValue,pAddrItem->Get_STCD(),strTM);
					}
					else if (Oracle_Database == m_nDatabaseType)
					{
						sSQL.Format("UPDATE ST_RSVR_R SET RZ = %.2f WHERE STCD='%s' AND TM = to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3')",
							pointValue,pAddrItem->Get_STCD(),strTM);
					}

					ret = ExcuteNoQuery(sSQL);
				}
			}
		}
		else if (pAddrItem->GetWlType() == WL_Type_SKXSW)
		{
			//查询
			if (SQLServer_Database == m_nDatabaseType)
			{
				sSQL.Format("SELECT BLRZ AS L FROM ST_RSVR_R WHERE STCD = '%s' AND TM = '%s'",
					pAddrItem->Get_STCD(),strTM);
			}
			else if (Oracle_Database == m_nDatabaseType)
			{
				sSQL.Format("SELECT BLRZ AS L FROM ST_RSVR_R WHERE STCD = '%s' AND TM = to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3')",
					pAddrItem->Get_STCD(),strTM);
			}

			isExist = GetWaterLevelData(sSQL,ValueInDb);

			if (false == isExist)  //INSERT
			{
				ret = 1;
				if (SQLServer_Database == m_nDatabaseType)
				{
					sSQL.Format("INSERT INTO ST_RSVR_R (STCD,TM, BLRZ) VALUES ('%s','%s',%.2f)",
						pAddrItem->Get_STCD(),strTM,pointValue);
				}
				else if (Oracle_Database == m_nDatabaseType)
				{
					sSQL.Format("INSERT INTO ST_RSVR_R (STCD,TM, BLRZ) VALUES ('%s',to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3'),%.2f)",
						pAddrItem->Get_STCD(),strTM,pointValue);
				}


				ret = ExcuteNoQuery(sSQL);
			}
			else if (true == isExist) //UPDATE
			{
				ret = 3;
				//if (ValueInDb != pointValue)
				if (fabs(ValueInDb - pointValue) >= 0.01 )
				{
					ret = 2;
#ifdef DEBUG
					TRACE("DB:%f\r\nPV:%f\r\n",ValueInDb,pointValue);
#endif
					if (SQLServer_Database == m_nDatabaseType)
					{
						sSQL.Format("UPDATE ST_RSVR_R SET BLRZ = %.2f WHERE STCD='%s' AND TM = '%s'",
							pointValue,pAddrItem->Get_STCD(),strTM);
					}
					else if (Oracle_Database == m_nDatabaseType)
					{
						sSQL.Format("UPDATE ST_RSVR_R SET BLRZ = %.2f WHERE STCD='%s' AND TM = to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3')",
							pointValue,pAddrItem->Get_STCD(),strTM);
					}

					ret = ExcuteNoQuery(sSQL);
				}
			}
		}
	}
	
	return ret;
}

//在主站SQL server 接口数据库中查询对应时间点某测站的水位数据
bool CDataTranSQLServer_SC::GetWaterLevelData( CString sSQL,double& WaterLevelData )
{
	bool ret = false;
	WaterLevelData = 0.0;
	CString sLog = "";
	try
	{

#ifdef DEBUG
		SYSTEMTIME debugTime;
		GetLocalTime(&debugTime);
		TRACE("%s 查找水位数据,SQL:%s\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugTime,true),sSQL);
#endif
		CRecordset rs(&m_Database);

		rs.Open(AFX_DB_USE_DEFAULT_TYPE,sSQL);

		CString sTemp = "";
		if(! rs.IsEOF())
		{
			rs.GetFieldValue("L",sTemp);
			WaterLevelData = atof(sTemp);

#ifdef DEBUG
			GetLocalTime(&debugTime);
			TRACE("%s 查找到水位数据:%f\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugTime,true),WaterLevelData);
#endif
			ret = true;
		}
		rs.Close();
	}
	catch (CDBException* e)
	{
		sLog.Format("执行SQL:%s,信息:%s",sSQL,e->m_strError);
		e->Delete();
		//AddThreadLog(sLog,Error_Log_message);
		AddThreadLog(sLog,Warning_log_Message);
		ret = false;
	}

	return ret;
}



void CDataTranSQLServer_SC::CalcRainData_RealTime( CString Rtu_ID,CRainData_Item* pRecvData )
{
	if (Rtu_ID !="" && pRecvData != NULL)
	{
		std::list<CAddrMap_Item*>::iterator Itr = m_mapAddressMap.end();
		for ( Itr = m_mapAddressMap.begin();
			  Itr != m_mapAddressMap.end();
			  Itr ++)
		{
			CAddrMap_Item* pAddrItem = (*Itr);
			if (pAddrItem != NULL && 
				pAddrItem->GetType() == CZ_Type_RAIN && 
				pAddrItem->Get_RTU_ID() == Rtu_ID)
			{
				//OLD
				//StatisticsRain_RealTime(/*Itr->second*/pAddrItem,pRecvData);

				//new1
				//StatisticsRain_Optimize1_RealTime(pAddrItem,pRecvData);

				//new2
				StatisticsRain_Optimize2_Enter(pAddrItem,pRecvData);
			}
		}
	}
}

void CDataTranSQLServer_SC::CalcDisplace_RealTime( CString Rtu_ID,int Rtu_No,CDisplace_DataItem* pRecvData )
{
	if (Rtu_ID !="" && pRecvData != NULL)
	{
		std::list<CAddrMap_Item*>::iterator Itr = m_mapAddressMap.end();
		for ( Itr = m_mapAddressMap.begin();
			  Itr != m_mapAddressMap.end();
			  Itr ++)
		{
			CAddrMap_Item* pAddrItem = (*Itr);
			if (pAddrItem != NULL && 
				pAddrItem->GetType() == CZ_Type_DISP && 
				pAddrItem->Get_RTU_ID() == Rtu_ID && 
				pAddrItem->Get_RtuNo() == Rtu_No)
			{
				StatisticsDisplace_Optimize2_Enter(pAddrItem,pRecvData);
			}
		}
	}
}

void CDataTranSQLServer_SC::CalcTempera_RealTime( CString Rtu_ID,CTempera_DataItem* pRecvData )
{
	if (Rtu_ID !="" && pRecvData != NULL)
	{
		std::list<CAddrMap_Item*>::iterator Itr = m_mapAddressMap.end();
		for ( Itr = m_mapAddressMap.begin();
			  Itr != m_mapAddressMap.end();
			  Itr ++)
		{
			CAddrMap_Item* pAddrItem = (*Itr);
			if (pAddrItem != NULL && 
				pAddrItem->GetType() == CZ_Type_TEMP && 
				pAddrItem->Get_RTU_ID() == Rtu_ID)
			{
				StatisticsTempera_Optimize2_Enter(pAddrItem,pRecvData);
			}
		}
	}
}

void CDataTranSQLServer_SC::StatisticsRain_RealTime( CAddrMap_Item* pAddrItem,CRainData_Item* pRecvData )
{
	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_RAIN && pRecvData != NULL)
	{
		//步骤一: 统计5分钟内降雨量
		StatisticsRain_5Minutes(pAddrItem,pRecvData);

		//步骤二: 统计15分钟内的降雨量,判断是否加报(暴雨加报)
		StatisticsRain_15Minutes(pAddrItem,pRecvData);

		//步骤三: 统计1个小时内的降雨量
		StatisticsRain_1Hour(pAddrItem,pRecvData);

		//步骤四: 统计1day内的降雨量
		StatisticsRain_1Day(pAddrItem,pRecvData);
	}
}

void CDataTranSQLServer_SC::StatisticsRain_5Minutes( CAddrMap_Item* pAddrItem,CRainData_Item* pRecvData )
{
	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_RAIN && pRecvData != NULL)
	{

		//new 20140816
		STRUCT_BEGINEND_AREA now5Area = Get5MinuteArea(); 

		CString strBegin = "",strEnd = "";
		CString strPoint = "";      //接口数据库TM 字段的值

		SYSTEMTIME colltime= pRecvData->m_tCollectTime;
		SYSTEMTIME _tempsystemtime = colltime;
		_tempsystemtime.wMinute = 0;
		_tempsystemtime.wSecond = 1;
		_tempsystemtime.wMilliseconds = 0;

		WORD mut = colltime.wMinute / 5;
		WORD mod = colltime.wMinute % 5;

		CTime _beginHour_ctime(_tempsystemtime);
		CTimeSpan _t_add_span(0,0,5 * mut,0);
		CTimeSpan _t_add5_span(0,0,5,0);

		SYSTEMTIME begintime,endtime;
		if (mod == 0 && colltime.wSecond == 0)
		{
			CTime _tempctime_end = _beginHour_ctime + _t_add_span;
			CTime _tempctime_begin  = _tempctime_end - _t_add5_span;
			CSystemTimeOp::ConvertCTimeToSystemtime(&_tempctime_end,&endtime);
			CSystemTimeOp::ConvertCTimeToSystemtime(&_tempctime_begin,&begintime);
		}
		else
		{
			CTime _tempctime_begin = _beginHour_ctime + _t_add_span;
			CTime _tempctime_end   = _tempctime_begin + _t_add5_span;
			CSystemTimeOp::ConvertCTimeToSystemtime(&_tempctime_begin,&begintime);
			CSystemTimeOp::ConvertCTimeToSystemtime(&_tempctime_end,&endtime);
		}

		strBegin = CSystemTimeOp::ConvertSystemTimeToString(&begintime,true);
		strEnd   = CSystemTimeOp::ConvertSystemTimeToString(&endtime,true);
		strPoint.Format("%04d-%02d-%02d %02d:%02d:00.005",
			endtime.wYear,endtime.wMonth,endtime.wDay,endtime.wHour,endtime.wMinute);

#ifdef DEBUG
		SYSTEMTIME _dbtime;
		GetLocalTime(&_dbtime);
		TRACE("%s 5 minutes,rain data[%s]\r\n",CSystemTimeOp::ConvertSystemTimeToString(&_dbtime,true),pRecvData->ToString());
		TRACE("%s 5 minutes,begin:%s,end:%s\r\n",CSystemTimeOp::ConvertSystemTimeToString(&_dbtime,true),strBegin,strEnd);
		TRACE("%s 5 minutes,point:%s\r\n",CSystemTimeOp::ConvertSystemTimeToString(&_dbtime,true),strPoint);
#endif
		
		double Minus5Minutes = 0.0;  //5分钟降雨量
		bool   isNull = false;
		Minus5Minutes = CalRainData_OnTimeRange(pAddrItem,strBegin,strEnd,Rain_statistics_5minutes,isNull);

		if (Minus5Minutes > m_dCHECK_RAIN_PER ||
			Minus5Minutes <= 0.0)
			Minus5Minutes = 0.0;

		if (false == isNull  && Minus5Minutes >= (double)0.1)
		{
				Table_ST_PPTN_R_Record data_record;
				data_record.STCD = pAddrItem->Get_STCD();
				data_record.TM   = strPoint;
				data_record.DRP  = Minus5Minutes;

				//其他字段的信息不需要
				data_record.INTV = 0.05;
				data_record.PRD  = 0.05;
				data_record.DYP  = 0.00;
				data_record.WTH  = "7";

				//new 20140816
				data_record.TYPE = Rain_statistics_5minutes;
				CSystemTimeOp::ConvertStringToSysemTime(strPoint,&(data_record.FTM));
				int comp = CSystemTimeOp::CompareSystemTime(&(data_record.FTM));
				if (comp <= 0)
					UpdateRainfallTableRecord(data_record,Rain_statistics_5minutes);
				else
					pAddrItem->UpdateST_PPTN_R_Record(&data_record);

				//Old
				//UpdateRainfallTableRecord(data_record,Rain_statistics_5minutes);
		}
		else 
		{
			CString strSQL = "";
			if (SQLServer_Database == m_nDatabaseType)
			{
				strSQL.Format("DELETE FROM ST_PPTN_R WHERE STCD='%s' AND TM='%s' AND INTV=0.05",pAddrItem->Get_STCD(),strPoint);
			}
			else if (Oracle_Database == m_nDatabaseType)
			{
				strSQL.Format("DELETE FROM ST_PPTN_R WHERE STCD='%s' AND TM=to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3') AND INTV=0.05",pAddrItem->Get_STCD(),strPoint);
			}
			ExcuteNoQuery(strSQL);

			//new 20140816
			pAddrItem->RemoveST_PPTN_R_Record(strPoint);
		}

		//update by repair data
		SYSTEMTIME repair_begintime, repair_endtime;
		CTime basic_ctime(endtime);

		repair_begintime = endtime;
		CTime _temp_ctime = basic_ctime + _t_add5_span;
		CSystemTimeOp::ConvertCTimeToSystemtime(&_temp_ctime,&repair_endtime);

        UINT _t_repair_check = 0;

		//修改修复数据算法 2013-02-05
		//while (_t_repair_check <= m_nCheck5Minutes_MNT)
		while(true)
		{
			CString str_repair_point= "";
			CString str_repair_begin = "", str_repair_end = "";
			double  repair_5minute_rain = 0.0;
			str_repair_begin = CSystemTimeOp::ConvertSystemTimeToString(&repair_begintime,true);
			str_repair_end = CSystemTimeOp::ConvertSystemTimeToString(&repair_endtime,true);
			str_repair_point.Format("%04d-%02d-%02d %02d:%02d:00.005",
				repair_endtime.wYear,
				repair_endtime.wMonth,
				repair_endtime.wDay,
				repair_endtime.wHour,
				repair_endtime.wMinute);

			//new 20140816
			//5分钟开始比当前五分钟的点还晚的 不统计
			//INT64 mus  = CSystemTimeOp::MinusSystemTime(/*repair_begintime*/repair_endtime,now5Area.stEnd);
			//if (mus > 2 * 60 * 1000)  
			//	break;
			SYSTEMTIME curST;
			GetLocalTime(&curST);
			INT64 mus = CSystemTimeOp::MinusSystemTime(repair_begintime,curST);
			if (mus >= 60 * 1000)
				break;


            bool repair_isNull = false;
			repair_5minute_rain = CalRainData_OnTimeRange(pAddrItem,str_repair_begin,str_repair_end,Rain_statistics_5minutes,repair_isNull);

			if (repair_5minute_rain > m_dCHECK_RAIN_PER ||
				repair_5minute_rain < 0.0)
				repair_5minute_rain = 0.0;

			if (false == repair_isNull && repair_5minute_rain >= (double)0.1 )
			{
				Table_ST_PPTN_R_Record data_record;
				data_record.STCD = pAddrItem->Get_STCD();
				data_record.TM   = str_repair_point;
				data_record.DRP  = repair_5minute_rain;

				//其他字段的信息不需要
				data_record.INTV = 0.05;
				data_record.PRD  = 0.05;
				data_record.DYP  = 0.00;
				data_record.WTH  = "7";

				//new 20140816
				data_record.TYPE = Rain_statistics_5minutes;
				CSystemTimeOp::ConvertStringToSysemTime(str_repair_point,&data_record.FTM);
				int comp = CSystemTimeOp::CompareSystemTime(&(data_record.FTM));
				if (comp <= 0)
					UpdateRainfallTableRecord(data_record,Rain_statistics_5minutes);
				else
					pAddrItem->UpdateST_PPTN_R_Record(&data_record);


				//OLD
				//UpdateRainfallTableRecord(data_record,Rain_statistics_5minutes);
			}
			else  //delete record
			{
				CString strSQL = "";
				if (SQLServer_Database == m_nDatabaseType)
				{
					strSQL.Format("DELETE FROM ST_PPTN_R WHERE STCD='%s' AND TM='%s' AND INTV=0.05",pAddrItem->Get_STCD(),str_repair_point);
				}
				else if (Oracle_Database == m_nDatabaseType)
				{
					strSQL.Format("DELETE FROM ST_PPTN_R WHERE STCD='%s' AND TM=to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3') AND INTV=0.05",pAddrItem->Get_STCD(),str_repair_point);
				}
				ExcuteNoQuery(strSQL);

				//new 20140816
				pAddrItem->RemoveST_PPTN_R_Record(str_repair_point);
			}
			
			//Next calc
			repair_begintime = repair_endtime;
			CTime repair_begin_ctime(repair_begintime);
			CTime repair_end_ctime = repair_begin_ctime + _t_add5_span;
			CTimeSpan _repair_tspan = repair_begin_ctime - basic_ctime;
			_t_repair_check = _repair_tspan.GetTotalMinutes();
			CSystemTimeOp::ConvertCTimeToSystemtime(&repair_end_ctime,&repair_endtime);

			//2013-02-05
			if (_t_repair_check > m_nCheck5Minutes_MNT && 
				(repair_begin_ctime.GetYear() != basic_ctime.GetYear() || 
				 repair_begin_ctime.GetMonth() != basic_ctime.GetMonth() ||
				 repair_begin_ctime.GetDay() != basic_ctime.GetDay() ||
				 repair_begin_ctime.GetHour() !=  basic_ctime.GetHour()))
				 break;
		}
	}
}

double CDataTranSQLServer_SC::CalRainData_OnTimeRange(CAddrMap_Item* pAddrItem, 
													  CString strBegin, 
													  CString strEnd, 
													  RIANSTATISTICS_TYPE_Enum xType, 
													  bool& isNull)
{
#ifdef DEBUG
		SYSTEMTIME _debug_systemtime;
		GetLocalTime(&_debug_systemtime);
		TRACE("%s Function:CalRainData_OnTimeRange\r\n",
			CSystemTimeOp::ConvertSystemTimeToString(&_debug_systemtime,true));
#endif

	double _RainData = 0.0;
	if (pAddrItem && pAddrItem->GetType() == CZ_Type_RAIN && strBegin != "" && strEnd != "")
	{

#ifdef DEBUG
		CString calcTypeName[] ={"5分钟统计",
							     "15分钟统计",
							     "小时统计",
							     "日统计"};
		TRACE("%s Function:CalRainData_OnTimeRange\r\n",CSystemTimeOp::ConvertSystemTimeToString(&_debug_systemtime,true));		
		TRACE("\t\tType :%d,Type's Name:%s\r\n",(int)xType,calcTypeName[int(xType)]);
		TRACE("\t\tBegin:%s\r\n",strBegin);
		TRACE("\t\tEnd  :%s\r\n",strEnd);
		TRACE("\t\tSTCD:%s,RTU:%s\r\n",pAddrItem->Get_STCD(),pAddrItem->Get_RTU_ID());
#endif

		SYSTEMTIME begintime,endtime;
		CSystemTimeOp::ConvertStringToSysemTime(strBegin,&begintime);
		CSystemTimeOp::ConvertStringToSysemTime(strEnd,&endtime);

		UINT check_time_value = 0;
		double check_data_value = 0.0;

		switch(xType)
		{
		case Rain_statistics_5minutes:
			check_time_value = m_nCheck5Minutes_MNT;
			check_data_value = m_dCHECK_RAIN_PER;
			break;
		case Rain_statistics_15minutes:
			check_time_value = m_nCheck15Minutes_MNT;
			check_data_value = m_dCHECK_RAIN_PER;
			break;
		case Rain_statistics_1hour:
			check_time_value = m_nCheck1Hour_MNT;
			check_data_value = m_dCHECK_RAIN_PER;
			break;
		case  Rain_statistics_AM8Point:
			check_time_value = m_nChcek1Day_MNT;
			check_data_value = m_dCHECK_RAIN_DAY;
			break;
		}

		CString sSQL = "";
		sSQL.Format("SELECT DISTINCT DATA_TYPE,CURRENT_VAL,HALFH_VAL,TOTAL_VAL,COLLECT_TIME FROM RAIN_DATA WHERE RTU='%s' AND SENSORS_NO='00' AND COLLECT_TIME >= '%s' AND COLLECT_TIME < '%s' ORDER BY COLLECT_TIME",
			pAddrItem->Get_RTU_ID(),strBegin,strEnd);

		CRainData_Item* pFirstRow = NULL;
		CRainData_Item* pLastRow = NULL;
		CRainData_Item* pBeforeRow = NULL;

		//add 2012-09-20
		CRainData_Item* pCopyBeforeRow = NULL;  //Copy data
		int nRowsCount = 0;

		CSqlStatement* sqlite_rs = ExcuteSQLiteQuery(sSQL);
#ifdef DEBUG
		//SYSTEMTIME _debug_systemtime;
		GetLocalTime(&_debug_systemtime);
		TRACE("\t\tread data begin:%s Function:CalRainData_OnTimeRange\r\n",
			CSystemTimeOp::ConvertSystemTimeToString(&_debug_systemtime,true));
#endif
		if (sqlite_rs != NULL)
		{
			while(sqlite_rs->NextRow())
			{
				CString sTemp = "";
				BYTE DataType =0;
				double CurrentValue =0.0,HalfHourValue = 0.0,TotalValue =0.0;
				CString CollTimeString = "";

				sTemp = sqlite_rs->ValueString(0);
				DataType = (BYTE)atoi(sTemp);

				sTemp = sqlite_rs->ValueString(1);
				CurrentValue = atof(sTemp);

				sTemp = sqlite_rs->ValueString(2);
				HalfHourValue = atof(sTemp);

				sTemp = sqlite_rs->ValueString(3);
				TotalValue =atof(sTemp);

				sTemp = sqlite_rs->ValueString(4);
				CollTimeString = sTemp;

				if (nRowsCount == 0)
					pFirstRow =new  CRainData_Item(DataType,CurrentValue,HalfHourValue,TotalValue,CollTimeString);

				_DELETE(pLastRow);
				pLastRow = new  CRainData_Item(DataType,CurrentValue,HalfHourValue,TotalValue,CollTimeString);

				nRowsCount ++;
			}
		}
		_DELETE(sqlite_rs);
#ifdef DEBUG
		//SYSTEMTIME _debug_systemtime;
		GetLocalTime(&_debug_systemtime);
		TRACE("\t\tread data end:%s Function:CalRainData_OnTimeRange\r\n",
			CSystemTimeOp::ConvertSystemTimeToString(&_debug_systemtime,true));
#endif

		if (nRowsCount == 0)
		{
			isNull = true;
			_RainData = 0.0;
		}
		else
		{
			isNull = false;
			pBeforeRow = GetEndTimeRainData(pAddrItem,strBegin);

			if (pBeforeRow != NULL)
			{
				//Copy data
				pCopyBeforeRow = new CRainData_Item(pBeforeRow);

				CTime begin_ctime(begintime);
				CTime before_ctime(pBeforeRow->m_tCollectTime);

				CTimeSpan muts = begin_ctime - before_ctime;

				//OLD 针对0.5的误差进行修改(无漏报包时可以修复0.5的误差) 2012-12-11
				//if (muts.GetTotalMinutes() > check_time_value)
				//{
				//	_DELETE(pBeforeRow);
				//	pBeforeRow = NULL;
				//}

				if (muts.GetTotalMinutes() > check_time_value)
				{
					if (xType == Rain_statistics_5minutes ||
						xType == Rain_statistics_15minutes)
					{
						CTime end_ctime(pLastRow->m_tCollectTime);
						CTimeSpan muts_end = end_ctime - before_ctime;

						if (pBeforeRow->m_tCollectTime.wYear	== pFirstRow->m_tCollectTime.wYear && 
							pBeforeRow->m_tCollectTime.wMonth	== pFirstRow->m_tCollectTime.wMonth && 
							pBeforeRow->m_tCollectTime.wDay		== pFirstRow->m_tCollectTime.wDay && 
							pBeforeRow->m_tCollectTime.wHour	== pFirstRow->m_tCollectTime.wHour)
						{
							double _mut = pFirstRow->TotalValue() - pBeforeRow->TotalValue();

							if ( muts_end.GetTotalMinutes() > 15 &&(_mut < 0.0 ||  _mut > 5.0))
							{
								_DELETE(pBeforeRow);
								pBeforeRow = NULL;
							}
						}
						else 
						{
							if (muts_end.GetTotalMinutes() > 15)
							{
								_DELETE(pBeforeRow);
								pBeforeRow = NULL;
							}
						}
					}
					else 
					{
						_DELETE(pBeforeRow);
						pBeforeRow = NULL;
					}
				}
			}

			//检查first 和 last 记录的时间戳是否相同(2012-09-20)
			if (nRowsCount > 1 )
			{
				if (pFirstRow->m_tCollectTime.wYear			==	pLastRow->m_tCollectTime.wYear && 
					pFirstRow->m_tCollectTime.wMonth		==	pLastRow->m_tCollectTime.wMonth &&
					pFirstRow->m_tCollectTime.wDay			==	pLastRow->m_tCollectTime.wDay &&
					pFirstRow->m_tCollectTime.wHour			==	pLastRow->m_tCollectTime.wHour && 
					pFirstRow->m_tCollectTime.wMinute		==	pLastRow->m_tCollectTime.wMinute && 
					pFirstRow->m_tCollectTime.wSecond		==  pLastRow->m_tCollectTime.wSecond)
				{
					_DELETE(pFirstRow);
					nRowsCount = 1;
				}
			}

			if (nRowsCount == 1)
			{
				if (pBeforeRow != NULL)
				{
					if (pLastRow->TotalValue() >= pBeforeRow->TotalValue())
						_RainData = pLastRow->TotalValue() - pBeforeRow->TotalValue();
					else 
						_RainData = RainFullScale_CONST - pBeforeRow->TotalValue() + pLastRow->TotalValue();
				}
				else  //pBeforeRow == null
				{
					//Old 
					//_RainData = pLastRow->SumHalfHour();

					//采用SumHalf Rain进行修复(2012-09-20)
					if (xType == Rain_statistics_15minutes ||
						xType == Rain_statistics_1hour     ||
						xType == Rain_statistics_AM8Point)
						_RainData = pLastRow->SumHalfHour();
					else if (xType == Rain_statistics_5minutes)
					{
						if (pCopyBeforeRow == NULL)
							_RainData = pLastRow->SumHalfHour();
						
						else  //pCopyBeforeRow != null
						{
							_RainData = 0;
							if ((endtime.wMinute>0 && endtime.wMinute <= 15)||
								(endtime.wMinute>30 && endtime.wMinute <= 45))
								_RainData = pLastRow->SumHalfHour();
						}
					}
				}
			}
			else //nRowsCount > 1
			{
				if (pBeforeRow != NULL)
				{
					if (pLastRow->TotalValue() >= pBeforeRow->TotalValue())
						_RainData = pLastRow->TotalValue() - pBeforeRow->TotalValue();
					else
						_RainData = RainFullScale_CONST - pBeforeRow->TotalValue() + pLastRow->TotalValue();
				}
				else
				{
					//Old
					//if (pLastRow->TotalValue() >= pFirstRow->TotalValue())
					//	_RainData = pLastRow->TotalValue() - pFirstRow->TotalValue();
					//else
					//	_RainData = RainFullScale_CONST - pFirstRow->TotalValue() + pLastRow->TotalValue();
		
					//采用SumHalf Rain进行修复(2012-09-20)
					if (xType == Rain_statistics_15minutes ||
						xType == Rain_statistics_1hour     ||
						xType == Rain_statistics_AM8Point)
					{
						if (pLastRow->TotalValue() >= pFirstRow->TotalValue())
							_RainData = pLastRow->TotalValue() - pFirstRow->TotalValue() + pFirstRow->SumHalfHour();
						else
							_RainData = RainFullScale_CONST - pFirstRow->TotalValue() + pLastRow->TotalValue() + pFirstRow->SumHalfHour();
					}
					else if (xType == Rain_statistics_5minutes)
					{
						if (pLastRow->TotalValue() >= pFirstRow->TotalValue())
							_RainData = pLastRow->TotalValue() - pFirstRow->TotalValue() ;
						else
							_RainData = RainFullScale_CONST - pFirstRow->TotalValue() + pLastRow->TotalValue();
					}
				}
			}	

			if (_RainData > check_data_value )
			{
				if (xType == Rain_statistics_5minutes ||xType == Rain_statistics_15minutes)
					_RainData = pLastRow->SumHalfHour();
				else if (xType == Rain_statistics_1hour)
				{
					SYSTEMTIME begintime;
					CSystemTimeOp::ConvertStringToSysemTime(strBegin,&begintime);
					_RainData = StatisticsRain_1Hour_special(pAddrItem,begintime,NULL);
				}
				else if (xType == Rain_statistics_AM8Point)
				{
					SYSTEMTIME daytime;
					CSystemTimeOp::ConvertStringToSysemTime(strEnd,&daytime);
					_RainData = StatisticsRain_1Day_special(pAddrItem,daytime,NULL);
				}
			}
		}

		_DELETE(pFirstRow);
		_DELETE(pLastRow);
		_DELETE(pBeforeRow);
		_DELETE(pCopyBeforeRow);

#ifdef DEBUG
		GetLocalTime(&_debug_systemtime);
		TRACE("\t\tReturn:%f,isNull:%s,time:%s\r\n",_RainData,
			isNull?"true":"false",
			CSystemTimeOp::ConvertSystemTimeToString(&_debug_systemtime,true));
#endif
	}

	return _RainData;
}

void CDataTranSQLServer_SC::StatisticsRain_15Minutes( CAddrMap_Item* pAddrItem,CRainData_Item* pRecvData )
{
	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_RAIN && pRecvData != NULL)
	{

		//new 20140816
		STRUCT_BEGINEND_AREA now15Area = Get15MinuteArea(); 

		CString strBegin ="",strEnd = "";
		CString strPoint = "";      //接口数据库TM 字段的值

		SYSTEMTIME collectTime = pRecvData->m_tCollectTime;

		SYSTEMTIME _tempsystemtime = collectTime;
		_tempsystemtime.wMinute = 0;
		_tempsystemtime.wSecond = 1;
		_tempsystemtime.wMilliseconds = 0;

		WORD mut = collectTime.wMinute / 15;
		WORD mod = collectTime.wMinute % 15;

		CTime _beginhour(_tempsystemtime);
		CTimeSpan _tspan_add(0,0,15 * mut,0);
		CTimeSpan _tspan_15(0,0,15,0);

		SYSTEMTIME begintime,endtime;
		if (mod == 0 && collectTime.wSecond == 0)
		{
			CTime _end_ctime = _beginhour + _tspan_add;
			CTime _begin_ctime = _end_ctime - _tspan_15;
			CSystemTimeOp::ConvertCTimeToSystemtime(&_begin_ctime,&begintime);
			CSystemTimeOp::ConvertCTimeToSystemtime(&_end_ctime,&endtime);
		}
		else
		{
			CTime _begin_ctime = _beginhour + _tspan_add;
			CTime _end_ctime = _begin_ctime + _tspan_15;
			CSystemTimeOp::ConvertCTimeToSystemtime(&_begin_ctime,&begintime);
			CSystemTimeOp::ConvertCTimeToSystemtime(&_end_ctime,&endtime);
		}

		strBegin = CSystemTimeOp::ConvertSystemTimeToString(&begintime,true);
		strEnd = CSystemTimeOp::ConvertSystemTimeToString(&endtime,true);
		strPoint.Format("%04d-%02d-%02d %02d:%02d",
			endtime.wYear,endtime.wMonth,endtime.wDay,endtime.wHour,endtime.wMinute);

#ifdef DEBUG
		SYSTEMTIME _dbtime;
		GetLocalTime(&_dbtime);
		TRACE("%s 15 minutes,rain data[%s]\r\n",CSystemTimeOp::ConvertSystemTimeToString(&_dbtime,true),pRecvData->ToString());
		TRACE("%s 15 minutes,begin:%s,end:%s\r\n",CSystemTimeOp::ConvertSystemTimeToString(&_dbtime,true),strBegin,strEnd);
		TRACE("%s 15 minutes,Point:%s\r\n",CSystemTimeOp::ConvertSystemTimeToString(&_dbtime,true),strPoint);
#endif

		double Minus15Minutes = 0.0;  //15分钟降雨量
		bool   isNull = false;

		Minus15Minutes = CalRainData_OnTimeRange(pAddrItem,strBegin,strEnd,Rain_statistics_15minutes,isNull);

		if (Minus15Minutes > m_dCHECK_RAIN_PER ||
			Minus15Minutes < 5.0)
			Minus15Minutes = 0.0;

		if (false == isNull && Minus15Minutes >= (double)5.0)
		{
			Table_ST_STORM_R_Record sRecord;

			sRecord.STCD	= pAddrItem->Get_STCD();
			sRecord.TM		= strPoint;
			sRecord.STRMP	= Minus15Minutes;

			sRecord.STRMDR	= 0.15;
			sRecord.WTH		= "7";

			//new 20140816
			CString strFTM = strPoint + ":00.000";
			CSystemTimeOp::ConvertStringToSysemTime(strFTM,&(sRecord.FTM));
			int comp = CSystemTimeOp::CompareSystemTime(&(sRecord.FTM));
			if (comp <= 0)
				UpdateRainstormTableRecord(sRecord);
			else
				pAddrItem->UpdateST_STROM_R_Record(&sRecord);


			//Old
			//UpdateRainstormTableRecord(sRecord);
		}
		else // delete
		{
			CString strSQL ="";
			if (SQLServer_Database == m_nDatabaseType)
			{
				strSQL.Format("DELETE FROM ST_STORM_R WHERE STCD='%s' AND TM='%s' AND STRMDR = 0.15",
					pAddrItem->Get_STCD(),strPoint);
			}
			else if (Oracle_Database == m_nDatabaseType)
			{
				strSQL.Format("DELETE FROM ST_STORM_R WHERE STCD='%s' AND TM=to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3') AND STRMDR = 0.15",
					pAddrItem->Get_STCD(),strPoint);
			}
			ExcuteNoQuery(strSQL);

			//new 20140816
			pAddrItem->RemoveST_STROM_R_Record(strPoint);
		}
		

		//update by repair data
		SYSTEMTIME repair_begintime, repair_endtime;
		CTime basic_ctime(endtime);

		repair_begintime = endtime;
		CTime _temp_ctime = basic_ctime + _tspan_15;
		CSystemTimeOp::ConvertCTimeToSystemtime(&_temp_ctime,&repair_endtime);

		UINT _t_repair_check = 0;

		//Modified 2013-02-05
		//while (_t_repair_check <= m_nCheck15Minutes_MNT)
		while(true)
		{
			CString str_repair_point= "";
			CString str_repair_begin = "", str_repair_end = "";
			double  repair_15minute_rain = 0.0;
			str_repair_begin = CSystemTimeOp::ConvertSystemTimeToString(&repair_begintime,true);
			str_repair_end = CSystemTimeOp::ConvertSystemTimeToString(&repair_endtime,true);
			str_repair_point.Format("%04d-%02d-%02d %02d:%02d",
				repair_endtime.wYear,
				repair_endtime.wMonth,
				repair_endtime.wDay,
				repair_endtime.wHour,
				repair_endtime.wMinute);

			//new 20140816
			//15分钟开始比当前12分钟的点还晚的 不统计
			//INT64 mus  = CSystemTimeOp::MinusSystemTime(/*repair_begintime*/repair_endtime,now15Area.stEnd);
			//if (mus > 3 * 60 * 1000)  
			//	break;
			SYSTEMTIME curST;
			GetLocalTime(&curST);
			INT64 mus = CSystemTimeOp::MinusSystemTime(repair_begintime,curST);
			if (mus >= 60 * 1000 )
				break;

			bool repair_isNull = false;
			repair_15minute_rain = CalRainData_OnTimeRange(pAddrItem,str_repair_begin,str_repair_end,Rain_statistics_15minutes,repair_isNull);

			if (repair_15minute_rain > m_dCHECK_RAIN_PER ||
				repair_15minute_rain < 5.0)
				repair_15minute_rain = 0.0;

			if (false == repair_isNull && repair_15minute_rain >= (double)5.0)
			{
				Table_ST_STORM_R_Record sRecord;

				sRecord.STCD	= pAddrItem->Get_STCD();
				sRecord.TM		= str_repair_point;
				sRecord.STRMP	= repair_15minute_rain;

				sRecord.STRMDR	= 0.15;
				sRecord.WTH		= "7";

				//new 20140816
				CString strFTM = str_repair_point + ":00.000";
				CSystemTimeOp::ConvertStringToSysemTime(strFTM,&(sRecord.FTM));
				int comp = CSystemTimeOp::CompareSystemTime(&(sRecord.FTM));
				if (comp <= 0)
					UpdateRainstormTableRecord(sRecord);
				else
					pAddrItem->UpdateST_STROM_R_Record(&sRecord);
				
				//OLD
				//UpdateRainstormTableRecord(sRecord);
			}
			else // delete record
			{
				CString strSQL ="";
				if (SQLServer_Database == m_nDatabaseType)
				{
					strSQL.Format("DELETE FROM ST_STORM_R WHERE STCD='%s' AND TM='%s' AND STRMDR = 0.15",
						pAddrItem->Get_STCD(),str_repair_point);
				}
				else if (Oracle_Database == m_nDatabaseType)
				{
					strSQL.Format("DELETE FROM ST_STORM_R WHERE STCD='%s' AND TM=to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3') AND STRMDR = 0.15",
						pAddrItem->Get_STCD(),str_repair_point);
				}


				ExcuteNoQuery(strSQL);

				//new 20140816
				pAddrItem->RemoveST_STROM_R_Record(str_repair_point);
			}
			
			//Next Calc
			repair_begintime		= repair_endtime;
			CTime repair_begin_ctime(repair_begintime);
			CTime repair_end_ctime	= repair_begin_ctime + _tspan_15;
			CTimeSpan _repair_tspan = repair_begin_ctime - basic_ctime;
			_t_repair_check			= _repair_tspan.GetTotalMinutes();
			CSystemTimeOp::ConvertCTimeToSystemtime(&repair_end_ctime,&repair_endtime);

			if (_t_repair_check > m_nCheck15Minutes_MNT &&
				(basic_ctime.GetYear() != repair_begin_ctime.GetYear() ||
				 basic_ctime.GetMonth() != repair_begin_ctime.GetMonth() || 
				 basic_ctime.GetDay() != repair_begin_ctime.GetDay() ||
				 basic_ctime.GetHour() != repair_begin_ctime.GetHour()) )
				break;
		}
	}
}

void CDataTranSQLServer_SC::StatisticsRain_1Hour( CAddrMap_Item* pAddrItem,CRainData_Item* pRecvData )
{
	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_RAIN && pRecvData != NULL)
	{
		//new 20140816
		STRUCT_BEGINEND_AREA now1HArea = Get1HourArea(); 

		CString strBegin ="",strEnd = "";
		CString strPoint = "";      //接口数据库TM 字段的值

		SYSTEMTIME collectTime = pRecvData->m_tCollectTime;

		SYSTEMTIME _tempsystemtime = collectTime;
		_tempsystemtime.wMinute = 0;
		_tempsystemtime.wSecond = 1;
		_tempsystemtime.wMilliseconds = 0;

		CTime _beginhour(_tempsystemtime);
		CTimeSpan _tspan_1hour(0,1,0,0);

		SYSTEMTIME begintime,endtime;
	     
		if (collectTime.wMinute == 0 &&
			collectTime.wSecond == 0)      //整点数据
		{
			endtime = _tempsystemtime;
			CTime _temp_begin_ctime = _beginhour - _tspan_1hour;
			CSystemTimeOp::ConvertCTimeToSystemtime(&_temp_begin_ctime,&begintime);
		}
		else									
		{
			begintime = _tempsystemtime;
			CTime _temp_end_ctime = _beginhour + _tspan_1hour;
			CSystemTimeOp::ConvertCTimeToSystemtime(&_temp_end_ctime,&endtime);
		}

		strBegin = CSystemTimeOp::ConvertSystemTimeToString(&begintime,true);
		strEnd = CSystemTimeOp::ConvertSystemTimeToString(&endtime,true);
		strPoint.Format("%04d-%02d-%02d %02d:00",
			endtime.wYear,endtime.wMonth,endtime.wDay,endtime.wHour);

#ifdef DEBUG
		SYSTEMTIME _dbtime;
		GetLocalTime(&_dbtime);
		TRACE("%s 1 hour,rain data[%s]\r\n",CSystemTimeOp::ConvertSystemTimeToString(&_dbtime,true),pRecvData->ToString());
		TRACE("%s 1 hour,begin:%s,end:%s\r\n",CSystemTimeOp::ConvertSystemTimeToString(&_dbtime,true),strBegin,strEnd);
		TRACE("%s 1 hour,Point:%s\r\n",CSystemTimeOp::ConvertSystemTimeToString(&_dbtime,true),strPoint);
#endif

		double Minus1Hour = 0.0; //小时降雨量
		bool   isHourNull = false;
		Minus1Hour = CalRainData_OnTimeRange(pAddrItem,strBegin,strEnd,Rain_statistics_1hour,isHourNull);

		//calc rain minutes 
		if (false == isHourNull)
		{
			if (Minus1Hour >  m_dCHECK_RAIN_PER ||
				Minus1Hour <= 0.0)
				Minus1Hour = 0.0f;

			int RainMinutes = 0;
			if (Minus1Hour >= (double)0.1)
			{
				if (m_RainPRDFlag != 0)
				{
					RainMinutes = GetRainMinutes(pAddrItem,strBegin,strEnd);
					if (RainMinutes <= 0 || RainMinutes > 60)
						RainMinutes = 60;	
				}

			}

			//判断是否8点,如果是8点时,需要添加8点数据(日统计、平安报数据)
			if (endtime.wHour != 8)
			{
				if (Minus1Hour >= (double)0.1)
				{
					Table_ST_PPTN_R_Record sRecord;

					sRecord.STCD = pAddrItem->Get_STCD();
					sRecord.TM = strPoint;
					sRecord.DRP = Minus1Hour;
					sRecord.INTV = 1.00;
					sRecord.WTH = "7";

					if (m_RainPRDFlag != 0)
					{
						if (RainMinutes >= 60 || RainMinutes <= 0 )
							sRecord.PRD = 0.00;
						else
							sRecord.PRD = (double)RainMinutes /100;
					}
					else
					{
						sRecord.PRD = 0.00;
					}

					//new 20140816
					sRecord.TYPE = Rain_statistics_1hour;
					CString strFTM = strPoint + ":00.000";
					CSystemTimeOp::ConvertStringToSysemTime(strFTM,&(sRecord.FTM));
					int comp = CSystemTimeOp::CompareSystemTime(&(sRecord.FTM));
					if (comp <= 0)
						UpdateRainfallTableRecord(sRecord,Rain_statistics_1hour);
					else
						pAddrItem->UpdateST_PPTN_R_Record(&sRecord);

					//OLD
					//UpdateRainfallTableRecord(sRecord,Rain_statistics_1hour);
				}
				else
				{
					CString sSQL = "";
					if (SQLServer_Database == m_nDatabaseType)
					{
						sSQL.Format("DELETE FROM ST_PPTN_R WHERE STCD='%s' AND TM='%s' AND INTV=1.00",
							pAddrItem->Get_STCD(),
							strPoint);
					}
					else if (Oracle_Database == m_nDatabaseType)
					{
						sSQL.Format("DELETE FROM ST_PPTN_R WHERE STCD='%s' AND TM=to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3') AND INTV=1.00",
							pAddrItem->Get_STCD(),
							strPoint);
					}

					ExcuteNoQuery(sSQL);

					//new 20140816
					pAddrItem->RemoveST_PPTN_R_Record(strPoint);
				}
			}
			else if (endtime.wHour == 8)
			{
				//calc AM8点数据
				CTime ctimeEnd(endtime);
				CTimeSpan _tspan (1,0,0,0);
				CTime ctime1dayBegin= ctimeEnd - _tspan;

				CString str1DayBegin ="";
				str1DayBegin.Format("%04d-%02d-%02d 08:00:01.000",ctime1dayBegin.GetYear(),ctime1dayBegin.GetMonth(),ctime1dayBegin.GetDay());

				//double Rain1Day = StatisticsRain_1Day(pAddrItem,str1DayBegin,strEnd,pRecvData);
				double Rain1Day = 0.0f;
				bool   isNullDay = false;
				Rain1Day = CalRainData_OnTimeRange(pAddrItem,str1DayBegin,strEnd,Rain_statistics_AM8Point,isNullDay);

				//遇到大数据时
				if (Rain1Day > m_dCHECK_RAIN_DAY)
					Rain1Day =0.0f;

				Table_ST_PPTN_R_Record srecord;
				srecord.STCD = pAddrItem->Get_STCD();
				srecord.TM	 = strPoint;
				srecord.DYP  = Rain1Day;
				if (Minus1Hour >= 0.1)
				{
					srecord.DRP = Minus1Hour;
					srecord.INTV = 1.00;
					srecord.WTH = "7";
					if (m_RainPRDFlag != 0)
					{
						if (RainMinutes >= 60 || RainMinutes <= 0 )
							srecord.PRD = 0.00;
						else
							srecord.PRD = (double)RainMinutes /100;
					}
					else 
					{
						srecord.PRD = 0.00;
					}

				}
				else if (Minus1Hour < (double)0.1)
					srecord.WTH = "8";


				//new 20140816
				srecord.TYPE = Rain_statistics_AM8Point;
				CString strFTM = strPoint + ":00.000";
				CSystemTimeOp::ConvertStringToSysemTime(strFTM,&(srecord.FTM));
				int comp = CSystemTimeOp::CompareSystemTime(&(srecord.FTM));
				if (comp <= 0 )
					UpdateRainfallTableRecord(srecord,Rain_statistics_AM8Point);
				else
					pAddrItem->UpdateST_PPTN_R_Record(&srecord);

				//OLD
				//UpdateRainfallTableRecord(srecord,Rain_statistics_AM8Point);
			}
		}

		//update by repair data(for 1 hour)
		SYSTEMTIME repair_begin_time,repair_end_time;
		CTime      basic_ctime(endtime);
		repair_begin_time = endtime;

		CTime      _temp_repair_end_ctime = basic_ctime + _tspan_1hour;
		CSystemTimeOp::ConvertCTimeToSystemtime(&_temp_repair_end_ctime,&repair_end_time);

		UINT repair_time_check = 0;
		while (repair_time_check <= m_nCheck1Hour_MNT)
		{
			CString str_repair_begin="",str_repair_end = "",str_repair_point = "";
			str_repair_begin = CSystemTimeOp::ConvertSystemTimeToString(&repair_begin_time,true);
			str_repair_end = CSystemTimeOp::ConvertSystemTimeToString(&repair_end_time,true);
			str_repair_point.Format("%04d-%02d-%02d %02d:00",
				repair_end_time.wYear,repair_end_time.wMonth,repair_end_time.wDay,repair_end_time.wHour);

			//new 20140816
			//在当前时间之后的不统计
			//INT64 mus  = CSystemTimeOp::MinusSystemTime(/*repair_begin_time*/repair_end_time,now1HArea.stEnd);
			//if (mus >= 60 * 1000)  
			//	break;
			SYSTEMTIME curST;
			GetLocalTime(&curST);
			INT64 mus = CSystemTimeOp::MinusSystemTime(repair_begin_time,curST);
			if (mus >= 60 * 1000 )
				break;

			double repair_rain_data =0.0;
			bool repair_isNull = false;

			repair_rain_data = CalRainData_OnTimeRange(pAddrItem,str_repair_begin,str_repair_end,Rain_statistics_1hour,repair_isNull);
			if (false == repair_isNull)
			{
				if (repair_rain_data >  m_dCHECK_RAIN_PER ||
					repair_rain_data <= 0.0)
					repair_rain_data = 0.0f;

				int repair_RainMinutes = 0;
				if (repair_rain_data >= (double)0.1)
				{
					if (m_RainPRDFlag != 0)
					{
						repair_RainMinutes = GetRainMinutes(pAddrItem,str_repair_begin,str_repair_end);
						if (repair_RainMinutes <= 0 || repair_RainMinutes > 60)
							repair_RainMinutes = 60;	
					}
				}

				//判断是否8点,如果是8点时,需要添加8点数据(日统计、平安报数据)
				if (repair_end_time.wHour != 8)
				{
					if (repair_rain_data >= (double)0.1)
					{
						Table_ST_PPTN_R_Record sRecord;

						sRecord.STCD = pAddrItem->Get_STCD();
						sRecord.TM  = str_repair_point;
						sRecord.DRP = repair_rain_data;
						sRecord.INTV = 1.00;
						sRecord.WTH = "7";

						if (m_RainPRDFlag != 0)
						{
							if (repair_RainMinutes >= 60 || repair_RainMinutes <= 0 )
								sRecord.PRD = 0.00;
							else
								sRecord.PRD = (double)repair_RainMinutes /100;
						}
						else
						{
							sRecord.PRD = 0.00;
						}


						//new 20140816
						sRecord.TYPE = Rain_statistics_1hour;
						CString strFTM = str_repair_point + ":00.000";
						CSystemTimeOp::ConvertStringToSysemTime(strFTM,&(sRecord.FTM));
						int comp = CSystemTimeOp::CompareSystemTime(&(sRecord.FTM));
						if (comp <= 0)
							UpdateRainfallTableRecord(sRecord,Rain_statistics_1hour);
						else
							pAddrItem->UpdateST_PPTN_R_Record(&sRecord);

						//OLD
						//UpdateRainfallTableRecord(sRecord,Rain_statistics_1hour);
					}
					else
					{
						CString sSQL = "";
						if (SQLServer_Database == m_nDatabaseType)
						{
							sSQL.Format("DELETE FROM ST_PPTN_R WHERE STCD='%s' AND TM='%s' AND INTV=1.00",pAddrItem->Get_STCD(),str_repair_point);
						}
						else if (Oracle_Database == m_nDatabaseType)
						{
							sSQL.Format("DELETE FROM ST_PPTN_R WHERE STCD='%s' AND TM=to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3') AND INTV=1.00",pAddrItem->Get_STCD(),str_repair_point);
						}
						
						ExcuteNoQuery(sSQL);

						//new 20140816
						pAddrItem->RemoveST_PPTN_R_Record(str_repair_point);
					}
				}
				else if (repair_end_time.wHour == 8)
				{
					//calc AM8点数据
					CTime ctimeEnd(repair_end_time);
					CTimeSpan _tspan (1,0,0,0);
					CTime ctime1dayBegin= ctimeEnd - _tspan;

					CString str1DayBegin ="";
					str1DayBegin.Format("%04d-%02d-%02d 08:00:01.000",ctime1dayBegin.GetYear(),ctime1dayBegin.GetMonth(),ctime1dayBegin.GetDay());

					double Rain1Day = 0.0f;
					bool   isNullDay = false;
					Rain1Day = CalRainData_OnTimeRange(pAddrItem,str1DayBegin,str_repair_end,Rain_statistics_AM8Point,isNullDay);

					//遇到大数据时
					if (Rain1Day > m_dCHECK_RAIN_DAY ||
						Rain1Day <= 0.0)
						Rain1Day =0.0f;

					Table_ST_PPTN_R_Record srecord;
					srecord.STCD = pAddrItem->Get_STCD();
					srecord.TM	 = str_repair_point;
					srecord.DYP  = Rain1Day;
					if (repair_rain_data >= 0.1)
					{
						srecord.DRP = repair_rain_data;
						srecord.INTV = 1.00;
						srecord.WTH = "7";
						if (m_RainPRDFlag != 0)
						{
							if (repair_RainMinutes >= 60 || repair_RainMinutes <= 0 )
								srecord.PRD = 0.00;
							else
								srecord.PRD = (double)repair_RainMinutes /100;
						}
						else
						{
							srecord.PRD = 0.00;
						}
					}
					else if (repair_rain_data < (double)0.1)
						srecord.WTH = "8";

					//new 20140816
					srecord.TYPE = Rain_statistics_AM8Point;
					CString strFTM = str_repair_point + ":00.000";
					CSystemTimeOp::ConvertStringToSysemTime(strFTM,&(srecord.FTM));
					int comp = CSystemTimeOp::CompareSystemTime(&(srecord.FTM));
					if (comp <= 0)
						UpdateRainfallTableRecord(srecord,Rain_statistics_AM8Point);
					else
						pAddrItem->UpdateST_PPTN_R_Record(&srecord);

					//OLD
					//UpdateRainfallTableRecord(srecord,Rain_statistics_AM8Point);
				}
			}

			//next
			repair_begin_time = repair_end_time;
			CTime repair_begin_ctime(repair_begin_time);
			CTime repare_end_ctime = repair_begin_ctime + _tspan_1hour;
			CSystemTimeOp::ConvertCTimeToSystemtime(&repare_end_ctime,&repair_end_time);
			CTimeSpan repair_tspan = repair_begin_ctime - basic_ctime;
			repair_time_check = repair_tspan.GetTotalMinutes();
		}
	}
}

void CDataTranSQLServer_SC::StatisticsRain_1Day(CAddrMap_Item* pAddrItem,CRainData_Item* pRecvData)
{
	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_RAIN && pRecvData != NULL)
	{
		//new 20140816
		STRUCT_BEGINEND_AREA now1DayArea = Get1DayArea(); 

		CString strBegin="",strEnd="",strPoint = "";
		CTime ctimeCur(pRecvData->m_tCollectTime);
		CTimeSpan tspan(1,0,0,0);
		SYSTEMTIME collecttime = pRecvData->m_tCollectTime;

		if ( (collecttime.wHour == 8 && collecttime.wMinute == 0 && collecttime.wSecond >= 1) ||
			 (collecttime.wHour == 8 && collecttime.wMinute != 0) ||
			 (collecttime.wHour >8 && collecttime.wHour <= 23))
		{
			strBegin.Format("%04d-%02d-%02d 08:00:01.000",pRecvData->m_tCollectTime.wYear,pRecvData->m_tCollectTime.wMonth,pRecvData->m_tCollectTime.wDay);
			CTime t = ctimeCur + tspan;
			strEnd.Format("%04d-%02d-%02d 08:00:01.000",t.GetYear(),t.GetMonth(),t.GetDay());
			strPoint.Format("%04d-%02d-%02d 08:00",t.GetYear(),t.GetMonth(),t.GetDay());
		}
		else if ((collecttime.wHour == 8 && collecttime.wMinute == 0 && collecttime.wSecond == 0) ||
			     (collecttime.wHour >= 0 && collecttime.wHour < 8 ))
		{
			CTime t = ctimeCur - tspan;
			strBegin.Format("%04d-%02d-%02d 08:00:01.000",t.GetYear(),t.GetMonth(),t.GetDay());
			strEnd.Format("%04d-%02d-%02d 08:00:01.000",pRecvData->m_tCollectTime.wYear,pRecvData->m_tCollectTime.wMonth,pRecvData->m_tCollectTime.wDay);
			strPoint.Format("%04d-%02d-%02d 08:00",pRecvData->m_tCollectTime.wYear,pRecvData->m_tCollectTime.wMonth,pRecvData->m_tCollectTime.wDay);
		}

		SYSTEMTIME begintime,endtime;
		CSystemTimeOp::ConvertStringToSysemTime(strBegin,&begintime);
		CSystemTimeOp::ConvertStringToSysemTime(strEnd,&endtime);

		CTime basic_ctime(begintime);
		UINT  m_check_time = 0;

		while (m_check_time <= m_nChcek1Day_MNT + tspan.GetTotalMinutes())
		{
			strBegin = CSystemTimeOp::ConvertSystemTimeToString(&begintime,true);
			strEnd   = CSystemTimeOp::ConvertSystemTimeToString(&endtime,true);
			strPoint.Format("%04d-%02d-%02d 08:00",endtime.wYear,endtime.wMonth,endtime.wDay);


			//new 20140816
			//在当前时间之后的不统计
			//INT64 mus  = CSystemTimeOp::MinusSystemTime(/*begintime*/endtime,now1DayArea.stEnd);
			//if (mus > 5 * 60 * 1000)  
			//	break;
			SYSTEMTIME curST;
			GetLocalTime(&curST);
			INT64 mus = CSystemTimeOp::MinusSystemTime(begintime,curST);
			if (mus >=  60 * 1000)
				break;


			double day_rain = 0;
			bool  isNullDay = false;
			day_rain = CalRainData_OnTimeRange(pAddrItem,strBegin,strEnd,Rain_statistics_AM8Point,isNullDay);

			if (false == isNullDay)
			{
				if (day_rain <=0.0 || day_rain > m_dCHECK_RAIN_DAY)
					day_rain = 0.0;

				//1 hour begin time and end time
				CString strHourBegin="",strHourEnd = "";
				strHourEnd = strEnd;
				strHourBegin.Format("%04d-%02d-%02d 07:00:01.000",endtime.wYear,endtime.wMonth,endtime.wDay);

				double hour_rain = 0.0;
				bool isNullHour = false;
				hour_rain = CalRainData_OnTimeRange(pAddrItem,strHourBegin,strHourEnd,Rain_statistics_1hour,isNullHour);

				if (false == isNullHour)
				{
					if (hour_rain <= 0.0 || hour_rain > m_dCHECK_RAIN_PER)
						hour_rain = 0.0f;

					int HourRainMinutes = 0;
					if (hour_rain >= (double)0.1)
					{
						if (m_RainPRDFlag != 0)
						{
							HourRainMinutes = GetRainMinutes(pAddrItem,strHourBegin,strHourEnd);
							if (HourRainMinutes <= 0 || HourRainMinutes > 60)
								HourRainMinutes = 60;	
						}

					}

					Table_ST_PPTN_R_Record srecord;
					srecord.STCD = pAddrItem->Get_STCD();
					srecord.TM	 = strPoint;
					srecord.DYP  = day_rain;
					if (hour_rain >= 0.1)
					{
						srecord.DRP = hour_rain;
						srecord.INTV = 1.00;
						srecord.WTH = "7";

						if (m_RainPRDFlag != 0)
						{
							if (HourRainMinutes >= 60 || HourRainMinutes <= 0 )
								srecord.PRD = 0.00;
							else
								srecord.PRD = (double)HourRainMinutes /100;
						}
						else
						{
							srecord.PRD = 0.00;
						}

					}
					else if (hour_rain < (double)0.1)
						srecord.WTH = "8";

					//new 20140816
					srecord.TYPE = Rain_statistics_AM8Point;
					CString strFTM = strPoint + ":00.000";
					CSystemTimeOp::ConvertStringToSysemTime(strFTM,&(srecord.FTM));
					int comp = CSystemTimeOp::CompareSystemTime(&(srecord.FTM));
					if (comp <= 0)
						UpdateRainfallTableRecord(srecord,Rain_statistics_AM8Point);
					else
						pAddrItem->UpdateST_PPTN_R_Record(&srecord);

					//OLD
					//UpdateRainfallTableRecord(srecord,Rain_statistics_AM8Point);

				}
				else if (true == isNullHour)
				{
					Table_ST_PPTN_R_Record srecord;
					srecord.STCD = pAddrItem->Get_STCD();
					srecord.TM	 = strPoint;
					srecord.DYP  = day_rain;
					srecord.WTH = "8";

					//new 20140816
					srecord.TYPE = Rain_statistics_AM8Point;
					CString strFTM = strPoint + ":00.000";
					CSystemTimeOp::ConvertStringToSysemTime(strFTM,&(srecord.FTM));
					int comp = CSystemTimeOp::CompareSystemTime(&(srecord.FTM));
					if (comp <= 0)
						UpdateRainfallTableRecord(srecord,Rain_statistics_AM8Point);
					else
						pAddrItem->UpdateST_PPTN_R_Record(&srecord);

					//UpdateRainfallTableRecord(srecord,Rain_statistics_AM8Point);
				}
			}

			//next row
			begintime = endtime;
			CTime temp_ctime(begintime);
			CTime tempend_ctime = temp_ctime + tspan;
			CSystemTimeOp::ConvertCTimeToSystemtime(&tempend_ctime,&endtime);
			CTimeSpan mus_tspan = temp_ctime - basic_ctime;
			m_check_time = mus_tspan.GetTotalMinutes();
		}
	}
}


CRainData_Item* CDataTranSQLServer_SC::GetEndTimeRainData( CAddrMap_Item* pAddrItem,CString strEnd )
{
	CRainData_Item* pResult = NULL;
	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_RAIN && strEnd != "")
	{
		CString sSQL = "";

		//sSQL.Format("SELECT DISTINCT DATA_TYPE,CURRENT_VAL,HALFH_VAL,TOTAL_VAL,COLLECT_TIME FROM RAIN_DATA WHERE RTU='%s' AND SENSORS_NO='00' AND COLLECT_TIME <= '%s' ORDER BY COLLECT_TIME DESC LIMIT 1",
		//	pAddrItem->Get_RTU_ID(),strEnd);
		sSQL.Format("SELECT DISTINCT DATA_TYPE,CURRENT_VAL,HALFH_VAL,TOTAL_VAL,COLLECT_TIME FROM RAIN_DATA WHERE RTU='%s' AND SENSORS_NO='00' AND COLLECT_TIME < '%s' ORDER BY COLLECT_TIME DESC LIMIT 1",
			pAddrItem->Get_RTU_ID(),strEnd);

		CSqlStatement* rs =ExcuteSQLiteQuery(sSQL);
		CString sTemp = "";
		BYTE DataType =0;
		double CurrentValue =0.0,HalfHourValue = 0.0,TotalValue =0.0;
		CString CollTimeString = "";
		SYSTEMTIME CollectTime;

#ifdef DEBUG
		SYSTEMTIME _debugtime;
		GetLocalTime(&_debugtime);
		TRACE("\t\tread data begin:%s \r\n",
			CSystemTimeOp::ConvertSystemTimeToString(&_debugtime,true));
#endif

		if (rs != NULL)
		{
			if (rs->NextRow())
			{
				sTemp = rs->ValueString(0);
				DataType = (BYTE)atoi(sTemp);

				sTemp = rs->ValueString(1);
				CurrentValue = atof(sTemp);

				sTemp = rs->ValueString(2);
				HalfHourValue = atof(sTemp);

				sTemp = rs->ValueString(3);
				TotalValue =atof(sTemp);

				sTemp = rs->ValueString(4);
				CollTimeString = sTemp;
				CSystemTimeOp::ConvertStringToSysemTime(sTemp,&CollectTime);

				pResult = new CRainData_Item(DataType,CurrentValue,HalfHourValue,TotalValue,CollTimeString);

#ifdef DEBUG
				//SYSTEMTIME _debugtime;
				GetLocalTime(&_debugtime);
				TRACE("%s,获取时间点( <(时间点) %s)的数据:%s\r\n",CSystemTimeOp::ConvertSystemTimeToString(&_debugtime,true),
					strEnd,
					pResult->ToString());
#endif
			}

			_DELETE(rs);
		}

	}
	return pResult;
}

CRainData_Item* CDataTranSQLServer_SC::GetStartTimeRainData( CAddrMap_Item* pAddrItem,CString strStart )
{
	CRainData_Item* pResult = NULL;
	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_RAIN && strStart != "")
	{
		CString sSQL = "";

		sSQL.Format("SELECT DISTINCT DATA_TYPE,CURRENT_VAL,HALFH_VAL,TOTAL_VAL,COLLECT_TIME FROM RAIN_DATA WHERE RTU='%s' AND SENSORS_NO='00' AND COLLECT_TIME > '%s' ORDER BY COLLECT_TIME LIMIT 1",
			pAddrItem->Get_RTU_ID(),strStart);

		CSqlStatement* rs =ExcuteSQLiteQuery(sSQL);
		CString sTemp = "";
		BYTE DataType =0;
		double CurrentValue =0.0,HalfHourValue = 0.0,TotalValue =0.0;
		CString CollTimeString = "";
		SYSTEMTIME CollectTime;

		if (rs != NULL)
		{
			if (rs->NextRow())
			{
				sTemp = rs->ValueString(0);
				DataType = (BYTE)atoi(sTemp);

				sTemp = rs->ValueString(1);
				CurrentValue = atof(sTemp);

				sTemp = rs->ValueString(2);
				HalfHourValue = atof(sTemp);

				sTemp = rs->ValueString(3);
				TotalValue =atof(sTemp);

				sTemp = rs->ValueString(4);
				CollTimeString = sTemp;
				CSystemTimeOp::ConvertStringToSysemTime(sTemp,&CollectTime);

				pResult = new CRainData_Item(DataType,CurrentValue,HalfHourValue,TotalValue,CollTimeString);

#ifdef DEBUG
				SYSTEMTIME _debugtime;
				GetLocalTime(&_debugtime);
				TRACE("%s,获取时间点(START:%s)的数据:%s\r\n",
					CSystemTimeOp::ConvertSystemTimeToString(&_debugtime,true),
					strStart,
					pResult->ToString());
#endif
			}

			_DELETE(rs);
		}
		
	}
	return pResult;
}

//更新接口数据库,降雨表的记录ST_PPTN_R
//Finded = true,	UPDATE
//Finded = false,	INSERT
bool CDataTranSQLServer_SC::UpdateRainfallTableRecord(Table_ST_PPTN_R_Record& sRecord,RIANSTATISTICS_TYPE_Enum xType)
{
	bool ret = true;
	CString sSQL = "";

#ifdef DEBUG
	SYSTEMTIME _dbtime;
	GetLocalTime(&_dbtime);
	TRACE("%s F:UpdateRainfallTableRecord,数据信息:%s\r\n",CSystemTimeOp::ConvertSystemTimeToString(&_dbtime,true),sRecord.ToString());
#endif

	if (SQLServer_Database == m_nDatabaseType)
	{
		sSQL.Format("SELECT STCD, TM, DRP, INTV, PDR, DYP, WTH FROM ST_PPTN_R WHERE STCD='%s' AND TM='%s'",
			sRecord.STCD,sRecord.TM);
	}
	else if (Oracle_Database == m_nDatabaseType)
	{
		sSQL.Format("SELECT STCD, to_char(TM,'YYYY-MM-DD HH24:MI:SS.ff3') AS TM, DRP, INTV, PDR, DYP, WTH FROM ST_PPTN_R WHERE STCD='%s' AND TM=to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3')",
			sRecord.STCD,sRecord.TM);
	}

	Table_ST_PPTN_R_Record findRecord;
	bool blFinded = false;
	blFinded = GetRainfallTableRecord(sSQL,findRecord);

	//update or insert 
	sSQL = "";
	if (xType == Rain_statistics_5minutes)
	{	
		if (sRecord.DRP < 0.1f)
			return false;

		if (blFinded == false)
		{
			if (SQLServer_Database == m_nDatabaseType)
			{
				sSQL.Format("INSERT INTO ST_PPTN_R (STCD, TM, DRP, INTV, WTH) VALUES ('%s','%s',%.1f,0.05,'7')",
					sRecord.STCD,sRecord.TM,sRecord.DRP);
			}
			else if (Oracle_Database == m_nDatabaseType)
			{
				sSQL.Format("INSERT INTO ST_PPTN_R (STCD, TM, DRP, INTV, WTH) VALUES ('%s',to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3'),%.1f,0.05,'7')",
					sRecord.STCD,sRecord.TM,sRecord.DRP);
			}

		}
		else if (true == blFinded)
		{
			if (fabs(sRecord.DRP - findRecord.DRP) >= 0.1f)
			{
				if (SQLServer_Database == m_nDatabaseType)
				{
					sSQL.Format("UPDATE ST_PPTN_R SET DRP = %.1f WHERE STCD='%s' AND TM='%s'",
						sRecord.DRP,sRecord.STCD,sRecord.TM);
				}
				else if (Oracle_Database == m_nDatabaseType)
				{
					sSQL.Format("UPDATE ST_PPTN_R SET DRP = %.1f WHERE STCD='%s' AND TM=to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3')",
						sRecord.DRP,sRecord.STCD,sRecord.TM);
				}
			}
		}
		ret = ExcuteNoQuery(sSQL);
	}
	if (xType == Rain_statistics_1hour)
	{
		sSQL ="";

		if (sRecord.DRP < 0.1f)
			return false;

		//if (blFinded == false)
		//{
		//	if (sRecord.PRD > 0.00 && sRecord.PRD < 0.60 )
		//	{
		//		sSQL.Format("INSERT INTO ST_PPTN_R (STCD, TM, DRP, INTV, PDR, WTH) VALUES ('%s','%s',%.1f,1.00,%.2f,'7')",
		//			sRecord.STCD,sRecord.TM,sRecord.DRP,sRecord.PRD);
		//	}
		//	else if (sRecord.PRD <= 0.00 || sRecord.PRD >= 0.60 )
		//	{
		//		sSQL.Format("INSERT INTO ST_PPTN_R (STCD, TM, DRP, INTV,WTH) VALUES ('%s','%s',%.1f,1.00,'7')",
		//			sRecord.STCD,sRecord.TM,sRecord.DRP);
		//	}
		//}
		//else if (true == blFinded)
		//{
		//	if (sRecord.PRD > 0.00 && sRecord.PRD < 0.60 )
		//	{
		//		sSQL.Format("UPDATE ST_PPTN_R SET DRP=%.1f,PDR=%.2f WHERE STCD = '%s' AND TM='%s'",
		//			sRecord.DRP,sRecord.PRD,sRecord.STCD,sRecord.TM);
		//	}
		//	else if (sRecord.PRD <= 0.00 || sRecord.PRD >= 0.60)
		//	{
		//		sSQL.Format("UPDATE ST_PPTN_R SET DRP=%.1f WHERE STCD = '%s' AND TM='%s'",
		//			sRecord.DRP,sRecord.STCD,sRecord.TM);
		//	}
		//}

		//Modified 2012-08-12
		if (true == blFinded)
		{
			if (SQLServer_Database == m_nDatabaseType)
			{
				sSQL.Format("DELETE FROM ST_PPTN_R WHERE STCD='%s' AND TM='%s' AND INTV=1.00",
					sRecord.STCD,sRecord.TM);
			}
			else if (Oracle_Database == m_nDatabaseType)
			{
				sSQL.Format("DELETE FROM ST_PPTN_R WHERE STCD='%s' AND TM=to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3') AND INTV=1.00",
					sRecord.STCD,sRecord.TM);
			}

			ExcuteNoQuery(sSQL);
		}

		//降雨历时 不用算，直接存为null 20140807
		if (m_RainPRDFlag != 0)
		{
			if (sRecord.PRD > 0.00 && sRecord.PRD < 0.60 )
			{
				if (SQLServer_Database == m_nDatabaseType)
				{
					sSQL.Format("INSERT INTO ST_PPTN_R (STCD, TM, DRP, INTV, PDR, WTH) VALUES ('%s','%s',%.1f,1.00,%.2f,'7')",
						sRecord.STCD,sRecord.TM,sRecord.DRP,sRecord.PRD);
				}
				else if (Oracle_Database == m_nDatabaseType)
				{
					sSQL.Format("INSERT INTO ST_PPTN_R (STCD, TM, DRP, INTV, PDR, WTH) VALUES ('%s',to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3'),%.1f,1.00,%.2f,'7')",
						sRecord.STCD,sRecord.TM,sRecord.DRP,sRecord.PRD);
				}

			}
			else if (sRecord.PRD <= 0.00 || sRecord.PRD >= 0.60 )
			{
				if (SQLServer_Database == m_nDatabaseType)
				{
					sSQL.Format("INSERT INTO ST_PPTN_R (STCD, TM, DRP, INTV,WTH) VALUES ('%s','%s',%.1f,1.00,'7')",
						sRecord.STCD,sRecord.TM,sRecord.DRP);
				}
				else if (Oracle_Database == m_nDatabaseType)
				{
					sSQL.Format("INSERT INTO ST_PPTN_R (STCD, TM, DRP, INTV,WTH) VALUES ('%s',to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3'),%.1f,1.00,'7')",
						sRecord.STCD,sRecord.TM,sRecord.DRP);
				}
			}
		}
		else
		{
			if (SQLServer_Database == m_nDatabaseType)
			{
				sSQL.Format("INSERT INTO ST_PPTN_R (STCD, TM, DRP, INTV,WTH) VALUES ('%s','%s',%.1f,1.00,'7')",
					sRecord.STCD,sRecord.TM,sRecord.DRP);
			}
			else if (Oracle_Database == m_nDatabaseType)
			{
				sSQL.Format("INSERT INTO ST_PPTN_R (STCD, TM, DRP, INTV,WTH) VALUES ('%s',to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3'),%.1f,1.00,'7')",
					sRecord.STCD,sRecord.TM,sRecord.DRP);
			}
		}

		ret = ExcuteNoQuery(sSQL);
	}
	if (xType == Rain_statistics_AM8Point)
	{
		//if (false == blFinded)
		//{
		//	if (sRecord.WTH == "7")
		//	{
		//		if (sRecord.PRD > 0.00 && sRecord.PRD < 0.60 )
		//		{
		//			sSQL.Format("INSERT INTO ST_PPTN_R (STCD, TM, DRP, INTV, PDR, DYP,WTH) VALUES ('%s','%s',%.1f,1.00,%.2f, %.1f,'7')",
		//				sRecord.STCD,sRecord.TM,sRecord.DRP,sRecord.PRD,sRecord.DYP);
		//		}
		//		else if (sRecord.PRD <= 0.00 || sRecord.PRD >= 0.60 )
		//		{
		//			sSQL.Format("INSERT INTO ST_PPTN_R (STCD, TM, DRP, INTV,DYP,WTH) VALUES ('%s','%s',%.1f,1.00,%.1f,'7')",
		//				sRecord.STCD,sRecord.TM,sRecord.DRP,sRecord.DYP);
		//		}
		//	}
		//	else if (sRecord.WTH == "8")
		//	{
		//		sSQL.Format("INSERT INTO ST_PPTN_R(STCD,TM,DYP,WTH) VALUES ('%s','%s',%.1f,'8')",
		//			sRecord.STCD,sRecord.TM,sRecord.DYP);
		//	}
		//}
		//else if (true == blFinded)
		//{
		//	if (sRecord.WTH == "7")
		//	{
		//		if (sRecord.PRD > 0.00 && sRecord.PRD < 0.60 )
		//		{
		//			/*sSQL.Format("INSERT INTO ST_PPTN_R (STCD, TM, DRP, INTV, PDR, DYP,WTH) VALUES ('%s','%s',%.1f,1.00,%.2f, %.1f,'7')",
		//				sRecord.STCD,sRecord.TM,sRecord.DRP,sRecord.PRD,sRecord.DYP);*/
		//			sSQL.Format("UPDATE ST_PPTN_R SET DRP = %.1f,INTV=1.00, PDR=%.2f,DYP=%.1f,WTH='7' WHERE STCD='%s' AND TM='%s'",
		//				sRecord.DRP,sRecord.PRD,sRecord.DYP,sRecord.STCD,sRecord.TM);
		//		}
		//		else if (sRecord.PRD <= 0.00 || sRecord.PRD >= 0.60 )
		//		{
		//			/*sSQL.Format("INSERT INTO ST_PPTN_R (STCD, TM, DRP, INTV,DYP,WTH) VALUES ('%s','%s',%.1f,1.00,%.1f,'7')",
		//				sRecord.STCD,sRecord.TM,sRecord.DRP,sRecord.DYP);*/
		//			sSQL.Format("UPDATE ST_PPTN_R SET DRP=%.1f,INTV=1.00,DYP=%.1f,WTH='7' WHERE STCD='%s' AND TM='%s'",
		//				sRecord.DRP,sRecord.DYP,sRecord.STCD,sRecord.TM);
		//		}
		//	}
		//	else if (sRecord.WTH == "8")
		//	{
		//		/*sSQL.Format("INSERT INTO ST_PPTN_R(STCD,TM,DYP,WTH) VALUES ('%s','%s',%.1f,'8')",
		//			sRecord.STCD,sRecord.TM,sRecord.DYP);*/

		//		sSQL.Format("UPDATE ST_PPTN_R SET DYP=%.1f,WTH='8' WHERE STCD='%s' AND TM='%s'",
		//			sRecord.DYP,sRecord.STCD,sRecord.TM);
		//	}
		//}

		if (true == blFinded)
		{
			if (SQLServer_Database == m_nDatabaseType)
			{
				sSQL.Format("DELETE FROM ST_PPTN_R WHERE STCD='%s' AND TM='%s'",sRecord.STCD,sRecord.TM);
			}
			else if (Oracle_Database == m_nDatabaseType)
			{
				sSQL.Format("DELETE FROM ST_PPTN_R WHERE STCD='%s' AND TM=to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3')",sRecord.STCD,sRecord.TM);
			}
			ExcuteNoQuery(sSQL);
		}

		sSQL ="";
		
		if (sRecord.WTH == "7")
		{
			//降雨历时 不用算，直接存为null 20140807
			if (m_RainPRDFlag != 0)
			{
				if (sRecord.PRD > 0.00 && sRecord.PRD < 0.60 )
				{
					if (SQLServer_Database == m_nDatabaseType)
					{
						sSQL.Format("INSERT INTO ST_PPTN_R (STCD, TM, DRP, INTV, PDR, DYP,WTH) VALUES ('%s','%s',%.1f,1.00,%.2f,%.1f,'7')",
							sRecord.STCD,sRecord.TM,sRecord.DRP,sRecord.PRD,sRecord.DYP);
					}
					else if (Oracle_Database == m_nDatabaseType)
					{
						sSQL.Format("INSERT INTO ST_PPTN_R (STCD, TM, DRP, INTV, PDR, DYP,WTH) VALUES ('%s',to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3'),%.1f,1.00,%.2f,%.1f,'7')",
							sRecord.STCD,sRecord.TM,sRecord.DRP,sRecord.PRD,sRecord.DYP);
					}

				}
				else //if (sRecord.PRD <= 0.00 || sRecord.PRD >= 0.60 )
				{
					if (SQLServer_Database == m_nDatabaseType)
					{
						sSQL.Format("INSERT INTO ST_PPTN_R (STCD, TM, DRP, INTV,DYP,WTH) VALUES ('%s','%s',%.1f,1.00,%.1f,'7')",
							sRecord.STCD,sRecord.TM,sRecord.DRP,sRecord.DYP);
					}
					else if (Oracle_Database == m_nDatabaseType)
					{
						sSQL.Format("INSERT INTO ST_PPTN_R (STCD, TM, DRP, INTV,DYP,WTH) VALUES ('%s',to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3'),%.1f,1.00,%.1f,'7')",
							sRecord.STCD,sRecord.TM,sRecord.DRP,sRecord.DYP);
					}
				}
			}
			else
			{
				if (SQLServer_Database == m_nDatabaseType)
				{
					sSQL.Format("INSERT INTO ST_PPTN_R (STCD, TM, DRP, INTV,DYP,WTH) VALUES ('%s','%s',%.1f,1.00,%.1f,'7')",
						sRecord.STCD,sRecord.TM,sRecord.DRP,sRecord.DYP);
				}
				else if (Oracle_Database == m_nDatabaseType)
				{
					sSQL.Format("INSERT INTO ST_PPTN_R (STCD, TM, DRP, INTV,DYP,WTH) VALUES ('%s',to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3'),%.1f,1.00,%.1f,'7')",
						sRecord.STCD,sRecord.TM,sRecord.DRP,sRecord.DYP);
				}
			}	
		}
		else if (sRecord.WTH == "8")
		{
			if (SQLServer_Database == m_nDatabaseType)
			{
				sSQL.Format("INSERT INTO ST_PPTN_R(STCD,TM,DYP,WTH) VALUES ('%s','%s',%.1f,'8')",
					sRecord.STCD,sRecord.TM,sRecord.DYP);
			}
			else if (Oracle_Database == m_nDatabaseType)
			{
				sSQL.Format("INSERT INTO ST_PPTN_R(STCD,TM,DYP,WTH) VALUES ('%s',to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3'),%.1f,'8')",
					sRecord.STCD,sRecord.TM,sRecord.DYP);
			}

		}
		ret = ExcuteNoQuery(sSQL);
	}


	//sRecord.TM="20130-05-13 08:00:00.000000";
	return ret;
}

//从接口数据库,降雨量表ST_PPTN_R查找记录
//返回值 true	 查找到记录
//		 false	 未查找到记录
bool CDataTranSQLServer_SC::GetRainfallTableRecord( CString sSQL,Table_ST_PPTN_R_Record& findRecord )
{
	bool ret = false;
	CString sLog = "";
	if (sSQL != "")
	{
		//sSQL.Format("SELECT STCD, TM, DRP, INTV, PDR, DYP, WTH FROM ST_PPTN_R WHERE STCD='%s' AND TM='%s'",
		//	sRecord.STCD,sRecord.TM);
		try
		{
#ifdef  DEBUG
			SYSTEMTIME _dbtime;
			GetLocalTime(&_dbtime);
			TRACE("%s F:GetRainfallTableRecord,SQL:%s\r\n",CSystemTimeOp::ConvertSystemTimeToString(&_dbtime,true),sSQL);
#endif

			CRecordset rs(&m_Database);
			
			rs.Open(AFX_DB_USE_DEFAULT_TYPE,sSQL);
			CString sTemp = "";
			if (!rs.IsEOF())
			{
				ret  = true;
				
				rs.GetFieldValue("STCD",sTemp);
				findRecord.STCD = sTemp.Trim();

				rs.GetFieldValue("TM",sTemp);
				findRecord.TM = sTemp.Trim();

				rs.GetFieldValue("DRP",sTemp);
				findRecord.DRP = atof(sTemp);

				rs.GetFieldValue("INTV",sTemp);
				findRecord.INTV = atof(sTemp);

				rs.GetFieldValue("PDR",sTemp);
				findRecord.PRD = atof(sTemp);

				rs.GetFieldValue("DYP",sTemp);
				findRecord.DYP = atof(sTemp);

				rs.GetFieldValue("WTH",sTemp);
				findRecord.WTH = sTemp.Trim();

#ifdef DEBUG
				GetLocalTime(&_dbtime);
				TRACE("%s Find ST_PPTN_R's record[%s]\r\n",CSystemTimeOp::ConvertSystemTimeToString(&_dbtime,true),findRecord.ToString());
#endif

			}
			rs.Close();

		}
		catch (CDBException* e)
		{
			ret = false;

			sLog.Format("执行SQL:%s,Error:%s",sSQL,e->m_strError);
			e->Delete();
			AddThreadLog(sLog,Error_Log_message);
		}
	}
	return ret;
}


//更新暴雨表ST_STORM_R的信息
//Finded = true   UPDATE 
//Finded = false   INSERT
bool CDataTranSQLServer_SC::UpdateRainstormTableRecord( Table_ST_STORM_R_Record& sRecord )
{
	bool ret = true;

#ifdef DEBUG
	SYSTEMTIME _dbtime;
	GetLocalTime(&_dbtime);
	TRACE("%s F:UpdateRainstormTableRecord,数据信息:%s\r\n",CSystemTimeOp::ConvertSystemTimeToString(&_dbtime,true),sRecord.ToString());
#endif

	if (sRecord.STCD != "" && sRecord.STRMP >= 5.0 && sRecord.TM != "")
	{
		CString sSQL = "";
		if (SQLServer_Database == m_nDatabaseType)
		{
			sSQL.Format("SELECT STCD, TM, STRMDR, STRMP, WTH FROM ST_STORM_R WHERE STCD='%s' AND TM='%s'",
				sRecord.STCD,sRecord.TM);
		}
		else if (Oracle_Database == m_nDatabaseType)
		{
			sSQL.Format("SELECT STCD, TM, STRMDR, STRMP, WTH FROM ST_STORM_R WHERE STCD='%s' AND TM=to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3')",
				sRecord.STCD,sRecord.TM);
		}


		Table_ST_STORM_R_Record findRecord;
		bool blFinded = false;
		blFinded = GetRainstormRecord(sSQL,findRecord);

		//更新暴雨表信息
		sSQL = "";
		if (true == blFinded)
		{
			if (fabs(sRecord.STRMP - findRecord.STRMP) >= 0.1f)
			{
				if (SQLServer_Database == m_nDatabaseType)
				{
					sSQL.Format("UPDATE ST_STORM_R SET STRMP=%.1f WHERE STCD='%s' AND TM='%s'",
						sRecord.STRMP,sRecord.STCD,sRecord.TM);
				}
				else if (Oracle_Database == m_nDatabaseType)
				{
					sSQL.Format("UPDATE ST_STORM_R SET STRMP=%.1f WHERE STCD='%s' AND TM=to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3')",
						sRecord.STRMP,sRecord.STCD,sRecord.TM);
				}

			}
		}
		else if (false == blFinded)
		{
			if (SQLServer_Database == m_nDatabaseType)
			{
				sSQL.Format("INSERT INTO ST_STORM_R (STCD, TM, STRMDR, STRMP, WTH ) VALUES ('%s','%s',0.15,%.1f,'7')",
					sRecord.STCD,sRecord.TM,sRecord.STRMP);
			}
			else if (Oracle_Database == m_nDatabaseType)
			{
				sSQL.Format("INSERT INTO ST_STORM_R (STCD, TM, STRMDR, STRMP, WTH ) VALUES ('%s',to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3'),0.15,%.1f,'7')",
					sRecord.STCD,sRecord.TM,sRecord.STRMP);
			}

		}

		ret = ExcuteNoQuery(sSQL);
	}

	return ret;
}

bool CDataTranSQLServer_SC::GetRainstormRecord( CString sSQL,Table_ST_STORM_R_Record& findRecord )
{
	bool ret = false;
	CString sLog = "";
	try
	{
		//SELECT STCD, TM, STRMDR, STRMP, WTH FROM ST_STORM_R

#ifdef  DEBUG
		SYSTEMTIME _dbtime;
		GetLocalTime(&_dbtime);
		TRACE("%s F:GetRainstormRecord,SQL:%s\r\n",CSystemTimeOp::ConvertSystemTimeToString(&_dbtime,true),sSQL);
#endif
		CRecordset rs(&m_Database);

		rs.Open(AFX_DB_USE_DEFAULT_TYPE,sSQL);
		CString sTemp = "";
		if (!rs.IsEOF())
		{
			ret  = true;

			rs.GetFieldValue("STCD",sTemp);
			findRecord.STCD = sTemp.Trim();

			rs.GetFieldValue("TM",sTemp);
			findRecord.TM = sTemp.Trim();

			rs.GetFieldValue("STRMDR",sTemp);
			findRecord.STRMDR = atof(sTemp);

			rs.GetFieldValue("STRMP",sTemp);
			findRecord.STRMP = atof(sTemp);

			rs.GetFieldValue("WTH",sTemp);

#ifdef DEBUG
			GetLocalTime(&_dbtime);
			TRACE("%s Find WT_STORM_R's record[%s]\r\n",CSystemTimeOp::ConvertSystemTimeToString(&_dbtime,true),findRecord.ToString());
#endif

		}
		rs.Close();
	}
	catch (CDBException* e)
	{
		ret = false;

		sLog.Format("执行SQL:%s,Error:%s",sSQL,e->m_strError);
		e->Delete();
		AddThreadLog(sLog,Error_Log_message);
	}

	return ret;
}

//查找strBegin, strEnd的下雨历时
//返回值 分钟;
int CDataTranSQLServer_SC::GetRainMinutes( CAddrMap_Item* pAddrItem, CString strBegin, CString strEnd )
{
	int rainMinutes = 0;

#ifdef DEBUG
	SYSTEMTIME _dbtime;
	GetLocalTime(&_dbtime);
	TRACE("%s 下雨历时, Begin:%s, End:%s\r\n",CSystemTimeOp::ConvertSystemTimeToString(&_dbtime,true),strBegin,strEnd);
#endif

	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_RAIN && strBegin != "" && strEnd != "")
	{
		CString sSQL ="";
		//sSQL.Format("SELECT DISTINCT DATA_TYPE,CURRENT_VAL,HALFH_VAL,TOTAL_VAL,COLLECT_TIME FROM RAIN_DATA WHERE RTU='%s' AND SENSORS_NO='00' AND DATA_TYPE IN ('01','02','04') AND COLLECT_TIME BETWEEN '%s' AND '%s' ORDER BY COLLECT_TIME"
		//	,pAddrItem->Get_RTU_ID(),strBegin,strEnd);
		sSQL.Format("SELECT DISTINCT DATA_TYPE,CURRENT_VAL,HALFH_VAL,TOTAL_VAL,COLLECT_TIME FROM RAIN_DATA WHERE RTU='%s' AND SENSORS_NO='00' AND DATA_TYPE IN ('01','02','04') AND COLLECT_TIME >= '%s' AND COLLECT_TIME < '%s' ORDER BY COLLECT_TIME"
			,pAddrItem->Get_RTU_ID(),strBegin,strEnd);

		CSqlStatement* rs = ExcuteSQLiteQuery(sSQL);

		if (rs != NULL)
		{
			int RowsCount = 0;
			std::vector<CRainData_Item*> RainDataVector;

			CString sTemp = "";
			while (rs->NextRow())
			{
				int dataType = 0;
				double currentVal =0.0,totalVal = 0.0,HalfhVal = 0.0;
				CString sCollectTime = "";

				sTemp = rs->ValueString(0);
				dataType = atoi(sTemp);

				sTemp = rs->ValueString(1);
				currentVal = atof(sTemp);

				sTemp = rs->ValueString(2);
				HalfhVal = atof(sTemp);

				sTemp = rs->ValueString(3);
				totalVal = atof(sTemp);

				sTemp = rs->ValueString(4);
				sCollectTime = sTemp.Trim();

				if (dataType != 0)
				{
					CRainData_Item* pRainObj = new CRainData_Item(dataType,currentVal,HalfhVal,totalVal,sCollectTime);
					if (pRainObj)
					{
						CSystemTimeOp::ConvertStringToSysemTime(sCollectTime,&(pRainObj->m_tCollectTime));
						RainDataVector.push_back(pRainObj);
					}
					RowsCount ++;
				}
			}   //while (rs->NextRow())

			_DELETE(rs);

			SYSTEMTIME begintime,endtime;
			CSystemTimeOp::ConvertStringToSysemTime(strBegin,&begintime);
			CSystemTimeOp::ConvertStringToSysemTime(strEnd,&endtime);

			CTime ctimebegin(begintime);
			CTime ctimeend(endtime);

			//计算下雨历时
			CRainData_Item* beforItem = NULL;
			for (size_t idx = 0; idx < RainDataVector.size(); idx ++)
			{
				CRainData_Item* CurObj = RainDataVector[idx];

				//if (RainDataVector[idx] != NULL)
				if (CurObj != NULL)
				{
					if (beforItem == NULL)
					{
						if (CurObj->DataType() ==  1 )
							rainMinutes = 0;
						else if (CurObj->DataType() == 2 ||
							     CurObj->DataType() == 4)
						{
							CTime temptime(CurObj->m_tCollectTime);
							CTimeSpan t= temptime - ctimebegin;
							rainMinutes +=  t.GetTotalMinutes();
						}
						
					}
					else if (beforItem != NULL)
					{
						if (CurObj->DataType()== 1)
						{
							//rainMinutes += 0;
						}
						else if (CurObj->DataType() == 2)
						{
							if ((beforItem->DataType()== 1 ||
								beforItem->DataType() == 2))
							{
								if (CurObj->CurrentValue() >= beforItem->CurrentValue() )
								{
									CTime tempbg(beforItem->m_tCollectTime);
									CTime tempend(CurObj->m_tCollectTime);
									CTimeSpan t = tempend - tempbg ;
									rainMinutes += t.GetTotalMinutes();
								}
								else  //
								{
									//rainMinutes += 0;
								}

							}
							else if (beforItem->DataType() == 4)
							{
								//rainMinutes += 0;
							}
						}
						else if (CurObj->DataType() == 4)
						{
							if (beforItem->DataType() == 1 ||
								beforItem->DataType() == 2)
							{
								if (CurObj->CurrentValue() >= beforItem->CurrentValue())
								{
									CTime tempbg(beforItem->m_tCollectTime);
									CTime tempend(CurObj->m_tCollectTime);
									CTimeSpan t = tempend - tempbg ;
									rainMinutes += t.GetTotalMinutes();
								}
								else  //
								{
									CTime tempbg(beforItem->m_tCollectTime);
									CTime tempend(CurObj->m_tCollectTime);
									CTimeSpan t = tempend - tempbg ;
									if (t.GetTotalMinutes() >= 15)
										rainMinutes += 15;  //15分钟后判断停止
								}
							}
						}
					}

					beforItem = CurObj;
				}
			}

			if (beforItem != NULL)  //end item
			{
				if (beforItem->DataType() == 1 || 
					beforItem->DataType() == 2)
				{
					CTime tempbg(beforItem->m_tCollectTime);
					CTimeSpan t = ctimeend - tempbg;
					rainMinutes += t.GetTotalMinutes();
				}
				else if (beforItem->DataType() == 4)
				{
					//rainMinutes += 0;
				}
			}

			//clear vector;
			for (size_t idx = 0;idx < RainDataVector.size(); idx ++)
			{
				_DELETE(RainDataVector[idx]);
			}
			RainDataVector.clear();
		}
	}

#ifdef DEBUG
	GetLocalTime(&_dbtime);
	TRACE("%s 下雨历时返回值:%d\r\n",CSystemTimeOp::ConvertSystemTimeToString(&_dbtime,true),rainMinutes);
#endif
	return rainMinutes;
}


//特殊处理
//前后两个半小时的值相加得到1hour的统计值
double CDataTranSQLServer_SC::StatisticsRain_1Hour_special( CAddrMap_Item* pAddrItem,SYSTEMTIME sttime,CRainData_Item* pRecvData /*= NULL*/ )
{
	//sttime 2012-02-06 12:00:00
	//strBegin 2012-02-06 12:00:01.000
	//strEnd   2012-02-06 13:00:01.000
	double HourRain = 0.0f;
	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_RAIN)
	{

#ifdef DEBUG
		SYSTEMTIME _debugtime;
		GetLocalTime(&_debugtime);
		TRACE("%s StatisticsRain_1Hour_special,ITEM:%s,TIME:%s\r\n",
			CSystemTimeOp::ConvertSystemTimeToString(&_debugtime,true),
			pAddrItem->Get_RTU_ID(),
			CSystemTimeOp::ConvertSystemTimeToString(&sttime,true));
#endif

		CString strBegin ="",strEnd ="";
		CTime  curTime(sttime);
		CTimeSpan tspan(0,1,0,0);
		CTime  endTime = curTime + tspan;

		//half hour point time
		SYSTEMTIME halfTime = sttime;
		halfTime.wMinute = 30;
		halfTime.wSecond =1;
		halfTime.wMilliseconds = 0;

		SYSTEMTIME starthourtime = sttime;
	    SYSTEMTIME endhourtime = sttime;
		starthourtime.wMinute = 0;
		starthourtime.wSecond = 1;
		starthourtime.wMilliseconds = 0;

		endhourtime.wYear = endTime.GetYear();
		endhourtime.wMonth = endTime.GetMonth();
		endhourtime.wDay = endTime.GetDay();
		endhourtime.wHour = endTime.GetHour();
		endhourtime.wMinute = 0;
		endhourtime.wSecond = 1;
		endhourtime.wMilliseconds = 0;

		strBegin.Format("%04d-%02d-%02d %02d:00:01.000",
			sttime.wYear,sttime.wMonth,sttime.wDay,sttime.wHour);
		strEnd.Format("%04d-%02d-%02d %02d:00:01.000",
			endTime.GetYear(),endTime.GetMonth(),endTime.GetDay(),endTime.GetHour());

#ifdef DEBUG
		TRACE("%s BEGIN:%s,END:%s,HALF:%s\r\n",
			CSystemTimeOp::ConvertSystemTimeToString(&_debugtime,true),
			CSystemTimeOp::ConvertSystemTimeToString(&starthourtime,true),
			CSystemTimeOp::ConvertSystemTimeToString(&endhourtime,true),
			CSystemTimeOp::ConvertSystemTimeToString(&halfTime,true));
		TRACE("%s BEGIN(STR):%s,END(STR):%s\r\n",
			CSystemTimeOp::ConvertSystemTimeToString(&_debugtime,true),
			strBegin,
			strEnd);
#endif

		CString sSQL = "";
		sSQL.Format("SELECT DISTINCT DATA_TYPE,CURRENT_VAL,HALFH_VAL,TOTAL_VAL,COLLECT_TIME FROM RAIN_DATA WHERE RTU='%s' AND SENSORS_NO='00' AND COLLECT_TIME >= '%s' AND COLLECT_TIME < '%s' ORDER BY COLLECT_TIME"
			,pAddrItem->Get_RTU_ID(),strBegin,strEnd);

		CSqlStatement* sqliters = ExcuteSQLiteQuery(sSQL);
		if (sqliters != NULL)
		{
			CString sTemp = "";
			BYTE DataType =0;
			double CurrentValue =0.0,HalfHourValue = 0.0,TotalValue =0.0;
			CString CollTimeString = "";
			SYSTEMTIME sCollectTime;

			double preHalfHourRain =0.0f, afterHalfHourRain = 0.0f;

			CRainData_Item *pAfterData = NULL;
			CRainData_Item *pPreData = NULL;

			while (sqliters->NextRow())
			{
				sTemp = sqliters->ValueString(0);
				DataType = (BYTE)atoi(sTemp);

				sTemp = sqliters->ValueString(1);
				CurrentValue = atof(sTemp);

				sTemp = sqliters->ValueString(2);
				HalfHourValue = atof(sTemp);

				sTemp = sqliters->ValueString(3);
				TotalValue =atof(sTemp);

				sTemp = sqliters->ValueString(4);
				CollTimeString = sTemp;
				CSystemTimeOp::ConvertStringToSysemTime(CollTimeString,&sCollectTime);

				int comparetime = CSystemTimeOp::CompareSystemTime(&sCollectTime,&halfTime);
				if (comparetime == -1)
				{
					_DELETE(pPreData);
					pPreData = new  CRainData_Item(DataType,CurrentValue,HalfHourValue,TotalValue,CollTimeString);
				}
				else  //=0 or 1
				{
					_DELETE(pAfterData);
					pAfterData = new  CRainData_Item(DataType,CurrentValue,HalfHourValue,TotalValue,CollTimeString);
				}
			}

			if (pRecvData != NULL)
			{
				//确定pRecvData时当前一个小时时间段的数据
				//与starttime和endtime进行比较
				int combegin = CSystemTimeOp::CompareSystemTime(&(pRecvData->m_tCollectTime),&starthourtime);
				int comEnd   = CSystemTimeOp::CompareSystemTime(&(pRecvData->m_tCollectTime),&endhourtime);

				bool isInHour = false;  //确定数据在当前一个小时时间范围内

				if ((combegin == 0 || 
					combegin == 1) && comEnd == -1)
					isInHour = true;

				if (isInHour == true)
				{
					int com = CSystemTimeOp::CompareSystemTime(&(pRecvData->m_tCollectTime),&halfTime);
					if (com == -1)
					{
						if (pPreData != NULL)
						{
							int comparedata = CSystemTimeOp::CompareSystemTime(&(pRecvData->m_tCollectTime),&(pPreData->m_tCollectTime));
							if (comparedata == 1)  //
							{
								_DELETE(pPreData);
								pPreData = new CRainData_Item(pRecvData->DataType(),
															  pRecvData->CurrentValue(),
															  pRecvData->SumHalfHour(),
															  pRecvData->TotalValue(),
															  pRecvData->CollectTimeString());
							}
						}
						else if (pPreData == NULL)
						{
							pPreData = new CRainData_Item(pRecvData->DataType(),
								pRecvData->CurrentValue(),
								pRecvData->SumHalfHour(),
								pRecvData->TotalValue(),
								pRecvData->CollectTimeString());
						}
					}
					else  //com =1 or 0
					{
						if (pAfterData != NULL)
						{
							int comparetime = CSystemTimeOp::CompareSystemTime(&(pRecvData->m_tCollectTime),&(pAfterData->m_tCollectTime));
							if (comparetime == 1)   //
							{
								_DELETE(pAfterData);
								pAfterData = new CRainData_Item(pRecvData->DataType(),
																pRecvData->CurrentValue(),
																pRecvData->SumHalfHour(),
																pRecvData->TotalValue(),
																pRecvData->CollectTimeString());
							}
						}
						else if (pAfterData == NULL)
						{
							pAfterData = new CRainData_Item(pRecvData->DataType(),
								pRecvData->CurrentValue(),
								pRecvData->SumHalfHour(),
								pRecvData->TotalValue(),
								pRecvData->CollectTimeString());
						}
					}
				}
			}

			if (pPreData != NULL)
				preHalfHourRain = pPreData->SumHalfHour();
			if (pAfterData != NULL)
				afterHalfHourRain = pAfterData->SumHalfHour();

			HourRain = preHalfHourRain + afterHalfHourRain;

			_DELETE(pPreData);
			_DELETE(pAfterData);
			_DELETE(sqliters);
		}
	}
	return HourRain;
}

double CDataTranSQLServer_SC::StatisticsRain_1Day_special( CAddrMap_Item* pAddrItem,SYSTEMTIME sttime,CRainData_Item* pRecvData /*= NULL*/ )
{
#ifdef DEBUG
	SYSTEMTIME _debugtime;
	GetLocalTime(&_debugtime);
	TRACE("%s StatisticsRain_1Day_special,Item:%s,TIME:%s\r\n",
		CSystemTimeOp::ConvertSystemTimeToString(&_debugtime,true),pAddrItem->Get_RTU_ID(),
		CSystemTimeOp::ConvertSystemTimeToString(&sttime,true));
#endif

	double DayRain = 0.0f;
	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_RAIN )
	{
		CTime  t(sttime);
		CTimeSpan tspan(1,0,0,0);
		CTime  t2 = t - tspan;
		CTime  t3(t2.GetYear(),t2.GetMonth(),t2.GetDay(),8,0,0);
		for (int Hours = 0; Hours < 24; Hours ++)
		{
			CTimeSpan  addspan(0,Hours,0,0);
			CTime t4 = t3 + addspan;

			SYSTEMTIME sttimehour ;
			sttimehour.wYear = t4.GetYear();
			sttimehour.wMonth = t4.GetMonth();
			sttimehour.wDay = t4.GetDay();
			sttimehour.wHour = t4.GetHour();
			sttimehour.wMinute = 0;
			sttimehour.wSecond = 0;
			sttimehour.wMilliseconds = 0;

			double hourRain =StatisticsRain_1Hour_special(pAddrItem,sttimehour,pRecvData);

			if (hourRain > 0.0)
				DayRain += hourRain;
		}
	}

#ifdef DEBUG
	GetLocalTime(&_debugtime);
	TRACE("%s StatisticsRain_1Day_special,Item:%s,TIME:%s,DAYRAIN:%f\r\n",
		CSystemTimeOp::ConvertSystemTimeToString(&_debugtime,true),pAddrItem->Get_RTU_ID(),
		CSystemTimeOp::ConvertSystemTimeToString(&sttime,true),
		DayRain);
#endif
	return DayRain;
}

void CDataTranSQLServer_SC::SetSqlite( CDbSQLite* sqlite )
{
	if (sqlite)
		m_local_sqlite = sqlite;
}

bool CDataTranSQLServer_SC::OpenScCacheSQLite()
{
	m_blOpenScCacheSucc =false;
	CHAR OLD_PATH[MAX_PATH];
	memset(OLD_PATH,0,sizeof(OLD_PATH));
	::GetCurrentDirectory(MAX_PATH,OLD_PATH);
	::SetCurrentDirectory(CUdfGlobalFUNC::GetRuningPath());

	bool ret = m_scChace_sqlite.Open((LPCTSTR)m_strScCacheDBName);	
	::SetCurrentDirectory(OLD_PATH);

	m_blOpenScCacheSucc = ret;

	return ret;
}

void CDataTranSQLServer_SC::CheckScCacheSQLite()
{
	CheckScCacheTableExist_CurState();
	CheckScCacheTableExist_CacheHis();
	CheckScCacheTableExist_StPptnR();
	CheckScCacheTableExist_StStormR();
	CheckScCacheTableExist_StWlR();
	CheckScCacheTableExist_StDISPR();
	CheckScCacheTableExist_StTEMPR();
	CheckScCacheTableExist_STCObjects();
}

void CDataTranSQLServer_SC::CheckScCacheTableExist_CurState()
{
	CString strSQL = "";
	if (IsExistScCacheTable("CUR_STATE") == false)
	{
		strSQL.Format("CREATE TABLE CUR_STATE (PID integer PRIMARY KEY autoincrement,WAITCOUNT TEXT,CURDEALCOLL TEXT,CURDELARECV TEXT, SAVETM DATETIME)");
		ExcuteScCacheNoQuery(strSQL);
	}
}

void CDataTranSQLServer_SC::CheckScCacheTableExist_CacheHis()
{
	CString strSQL = "";
	if (IsExistScCacheTable("CACHE_HIS") == false)
	{
		strSQL.Format("CREATE TABLE CACHE_HIS (PID integer PRIMARY KEY autoincrement,TABLENAME TEXT,SAVETM DATETIME)");
		ExcuteScCacheNoQuery(strSQL);
	}
}

void CDataTranSQLServer_SC::CheckScCacheTableExist_StPptnR()
{
	CString strSQL = "";
	if (IsExistScCacheTable("ST_PPTN_R") == false)
	{
		strSQL.Format("CREATE TABLE ST_PPTN_R (PID integer PRIMARY KEY autoincrement,STCD TEXT,TYPE TEXT,TM TEXT,FTM TEXT,DRP TEXT,INTV TEXT,PRD TEXT,DYP TEXT,WTH TEXT,CAL_TM DATETIME)");
		ExcuteScCacheNoQuery(strSQL);
	}
}

void CDataTranSQLServer_SC::CheckScCacheTableExist_StStormR()
{
	CString strSQL = "";
	if (IsExistScCacheTable("ST_STORM_R") == false)
	{
		strSQL.Format("CREATE TABLE ST_STORM_R (PID integer PRIMARY KEY autoincrement,STCD TEXT,TM TEXT,FTM TEXT,STRMDR TEXT,STRMP TEXT,WTH TEXT,CAL_TM DATETIME)");
		ExcuteScCacheNoQuery(strSQL);
	}
}

void CDataTranSQLServer_SC::CheckScCacheTableExist_StWlR()
{
	CString strSQL = "";
	if (IsExistScCacheTable("ST_WL_R") == false)
	{
		strSQL.Format("CREATE TABLE ST_WL_R (PID integer PRIMARY KEY autoincrement,STCD TEXT,TYPE TEXT,TM TEXT,FTM TEXT,PV TEXT,CAL_TM DATETIME)");
		ExcuteScCacheNoQuery(strSQL);
	}
}

void CDataTranSQLServer_SC::CheckScCacheTableExist_StDISPR()
{
	CString strSQL = "";
	if (IsExistScCacheTable("ST_DISP_R") == false)
	{
		strSQL.Format("CREATE TABLE ST_DISP_R (PID integer PRIMARY KEY autoincrement,STCD TEXT,TYPE TEXT,TM TEXT,FTM TEXT,X TEXT,Y TEXT,CAL_TM DATETIME)");
		ExcuteScCacheNoQuery(strSQL);
	}
}

void CDataTranSQLServer_SC::CheckScCacheTableExist_StTEMPR()
{
	CString strSQL = "";
	if (IsExistScCacheTable("ST_TEMP_R") == false)
	{
		strSQL.Format("CREATE TABLE ST_TEMP_R (PID integer PRIMARY KEY autoincrement,STCD TEXT,TYPE TEXT,TM TEXT,FTM TEXT,T TEXT,CAL_TM DATETIME)");
		ExcuteScCacheNoQuery(strSQL);
	}
}

void CDataTranSQLServer_SC::CheckScCacheTableExist_STRIVER()
{
	/*CString strSQL = "";
	if (IsExistScCacheTable("ST_RIVER_R") == false)
	{
		strSQL.Format("CREATE TABLE ST_RIVER_R (PID integer PRIMARY KEY autoincrement,STCD TEXT,TYPE TEXT,TM TEXT,FTM TEXT,T TEXT,CAL_TM DATETIME)");
		ExcuteScCacheNoQuery(strSQL);
	}*/
}

void CDataTranSQLServer_SC::CheckScCacheTableExist_STRSVR()
{
	/*CString strSQL = "";
	if (IsExistScCacheTable("ST_RSVR_R") == false)
	{
		strSQL.Format("CREATE TABLE ST_TEMP_R (PID integer PRIMARY KEY autoincrement,STCD TEXT,TYPE TEXT,TM TEXT,FTM TEXT,T TEXT,CAL_TM DATETIME)");
		ExcuteScCacheNoQuery(strSQL);
	}*/
}

void CDataTranSQLServer_SC::CheckScCacheTableExist_STCObjects()
{
	CString strSQL = "";
	if (IsExistScCacheTable("STC_OBJECTS") == false)
	{
		strSQL.Format("CREATE TABLE STC_OBJECTS (PID integer PRIMARY KEY autoincrement,STCD TEXT,TYPE TEXT,WLTYPE TEXT,TM TEXT,CAL_TYPE TEXT,FTM TEXT,BEGIN_ST TEXT,END_ST TEXT,CAL_TM DATETIME,PV TEXT,X TEXT,Y TEXT,T TEXT,COLLECT TEXT)");
		ExcuteScCacheNoQuery(strSQL);
	}
}


bool CDataTranSQLServer_SC::IsExistScCacheTable( CString strTable )
{
	int nCount = 0;
	try
	{
		CString strSQL = "";
		strSQL.Format("SELECT COUNT(*) as nCount FROM sqlite_master WHERE type='table' AND NAME='%s'",strTable);

		CSqlStatement* stmt = m_scChace_sqlite.Statement(strSQL);

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

bool CDataTranSQLServer_SC::ExcuteScCacheNoQuery( CString strSQL )
{
	bool ret = true;

//#ifdef DEBUG
//	SYSTEMTIME dbtime;
//	TRACE("SQLite数据库操作:%s\r\n",strSQL);
//	GetLocalTime(&dbtime);
//	TRACE("\t\tExecute Begin: %s\r\n",
//		CSystemTimeOp::ConvertSystemTimeToString(&dbtime,true));
//#endif
	try
	{
		strSQL.Trim();
		if (!strSQL.IsEmpty())
		{
			sqlite_ex_result r = m_scChace_sqlite.DirectStatement(strSQL);

			ret = r.blResult;
			if (r.blResult == false)
			{
				CString sLog = "";
				_Log_level_type xLevel = Warning_log_Message;
				sLog.Format("sc.cache.db SQLite error:%s,SQL:%s",r.sMessage,strSQL);
				AddThreadLog(sLog,Warning_log_Message,false);
			}
		}
	}
	catch (CException* e)
	{
		CString sLog = "";
		_Log_level_type xLevel = Warning_log_Message;
		sLog.Format("sc.cache.db SQLite error,SQL:%s",strSQL);
		AddThreadLog(sLog,Warning_log_Message,false);

		e->Delete();
		ret = false;
	}

//#ifdef DEBUG
//	GetLocalTime(&dbtime);
//	TRACE("\t\tExecute end: %s\r\n",
//		CSystemTimeOp::ConvertSystemTimeToString(&dbtime,true));
//#endif

	return ret;
}

CSqlStatement* CDataTranSQLServer_SC::ExcuteScCacheQuery( CString strSQL )
{
	CSqlStatement* stmt =NULL;

//#ifdef DEBUG
//	SYSTEMTIME dbtime;
//	TRACE("SQLite数据库操作:%s\r\n",strSQL);
//	GetLocalTime(&dbtime);
//	TRACE("\t\tExecute Begin: %s\r\n",
//		CSystemTimeOp::ConvertSystemTimeToString(&dbtime,true));
//#endif

	try
	{
		if (m_blOpenScCacheSucc == true)
			stmt = m_scChace_sqlite.Statement(strSQL);
	}
	catch (CException* e)
	{
		e->Delete();
		stmt = NULL;
	}

//#ifdef DEBUG
//	GetLocalTime(&dbtime);
//	TRACE("\t\tExecute end: %s\r\n",
//		CSystemTimeOp::ConvertSystemTimeToString(&dbtime,true));
//#endif

	return stmt;
}

void CDataTranSQLServer_SC::ScCacheInSQLServer_OnTimer()
{
#ifdef DEBUG
	SYSTEMTIME debugst;
	GetLocalTime(&debugst);
#endif

	 if (m_dwPreInSQLServerTC <= 0)
	 {
		 m_dwPreInSQLServerTC = GetTickCount();
#ifdef DEBUG

		 TRACE("%s 首次启动ScCacheInSQLServer_OnTimer.\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
#endif
		 return;
	 }


	 if (GetTickCount() - m_dwPreInSQLServerTC >= m_dwInSQLServer_Interval)
	 {
#ifdef DEBUG
		 //TRACE("%s 启动入省标库功能.\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
#endif

		 //invoke 缓存入省标库功能
		 ScCacheInSQLServer();

		 //reset m_dwPreInSQLServerTC's Value
		 m_dwPreInSQLServerTC = GetTickCount();
	 }
}

void CDataTranSQLServer_SC::ScCacheSaveToSQLite_OnTimer()
{
#ifdef DEBUG
	SYSTEMTIME debugst;
	GetLocalTime(&debugst);
#endif

	if (m_dwPreSaveToSQLiteTC <=0)
	{
		m_dwPreSaveToSQLiteTC = GetTickCount();
#ifdef DEBUG
		TRACE("%s 首次启动ScCacheSaveToSQLite_OnTimer\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
#endif
		return;
	}

	if (GetTickCount()- m_dwPreSaveToSQLiteTC >= m_dwSaveToSQLite_Interval)
	{
#ifdef DEBUG
		TRACE("%s 启动缓存存SQLite功能.\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
#endif

		//invoke 缓存存SQLite功能
		ScCacheSaveToSQLite();

		//reset m_dwPreSaveToSQLiteTC's value
		m_dwPreSaveToSQLiteTC = GetTickCount();
	}
}

void CDataTranSQLServer_SC::ScStatistics_OnTimer()
{
#ifdef DEBUG
	SYSTEMTIME debugst;
	GetLocalTime(&debugst);
#endif

	if (m_ScStatisticsTimer.m_nPreTickCount <=0)
	{
		m_ScStatisticsTimer.m_nPreTickCount = GetTickCount();
#ifdef DEBUG
		TRACE("%s 首次启动ScStatistics_OnTimer\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
#endif
		return;
	}

	if (GetTickCount()- m_ScStatisticsTimer.m_nPreTickCount >= m_ScStatisticsTimer.m_nInterval)
	{
#ifdef DEBUG
		//TRACE("%s 启动ScStatistics_OnTimer功能.\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
#endif
		//invoke 
		ScStatisticsCheck();

		//reset 
		m_ScStatisticsTimer.m_nPreTickCount = GetTickCount();
	}
}

void CDataTranSQLServer_SC::ScUpdataCacheCheck_OnTimer()
{
#ifdef DEBUG
	SYSTEMTIME debugst;
	GetLocalTime(&debugst);
#endif

	if (m_ScUpdataCacheTimer.m_nPreTickCount <=0)
	{
		m_ScUpdataCacheTimer.m_nPreTickCount = GetTickCount();
#ifdef DEBUG
		TRACE("%s 首次启动ScUpdataCacheCheck_OnTimer\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
#endif
		return;
	}

	if (GetTickCount()- m_ScUpdataCacheTimer.m_nPreTickCount >= m_ScUpdataCacheTimer.m_nInterval)
	{
#ifdef DEBUG
		TRACE("%s 启动ScUpdataCacheCheck_OnTimer功能.\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
#endif

		//invoke 
		ScUpdataCahceCheck();

		//reset 
		m_ScUpdataCacheTimer.m_nPreTickCount = GetTickCount();
	}
}


void CDataTranSQLServer_SC::ScStatisticsCheck()
{
	std::list<CAddrMap_Item*>::iterator addrItr;
	for (addrItr = m_mapAddressMap.begin();addrItr != m_mapAddressMap.end();addrItr ++)
	{
		CAddrMap_Item* pAddrItem = (*addrItr);
		if (pAddrItem != NULL)
		{
			if (pAddrItem->GetType() == CZ_Type_RAIN)
			{
				ScStatisticsCheck_Rain(pAddrItem);  //启动雨量统计功能
			}
			else if (pAddrItem->GetType() == CZ_Type_WL)
			{
				ScStatisticsCheck_WaterLevel(pAddrItem); //启动水位统计功能
				
			}
			else if (pAddrItem->GetType() == CZ_Type_DISP)
			{
				ScStatisticsCheck_Displace(pAddrItem); //启动深度位移统计功能
			}
			else if (pAddrItem->GetType() == CZ_Type_TEMP)
			{
				ScStatisticsCheck_Tempera(pAddrItem); //启动温度统计功能
			}
			else if(pAddrItem->GetType() == CZ_Type_FLOW)
			{
				ScStatisticsCheck_Flow(pAddrItem);     //启动流量统计功能
			}
		}
	}
}

//2023-5-23
void CDataTranSQLServer_SC::ScStatisticsCheck_Flow(CAddrMap_Item* pAddrItem)
{
	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_FLOW)
	{
		std::list<CString> FlowList;
		std::map<CString, StatisticsOnTimer_Item*>::iterator chkItr;
		for (chkItr = pAddrItem->m_mapStatistics_Flow.begin();chkItr != pAddrItem->m_mapStatistics_Flow.end();chkItr++)
		{
			StatisticsOnTimer_Item* pObj = chkItr->second;
			if (pObj != NULL)
			{
				//int comp = CSystemTimeOp::CompareSystemTime(&(pObj->stBegin),&(pAddrItem->m_UpDataCache.m_stTemperaBeginTime));
				int comp2 = CSystemTimeOp::CompareSystemTime(&(pObj->stBegin), &(pAddrItem->m_UpDataCache.m_stCacheAreaBegin));  //?????????

				std::multimap<CString, CFlow_DataItem*> DataMap;
				bool blStatistics = false;
				//if (comp >=0 || comp2 > 0 )
				if (comp2 > 0)
				{
					DataMap = StatisticsFlow_Optimize2_GetFlowDataFromUpDataCache(pAddrItem, pObj->strBegin, pObj->strEnd);
					blStatistics = true;
				}
				else //if (comp < 0 && m_blLocalSQliteTransStartFlag == false) //在没有启动数据库事务时才能读本地的数据
				{
					if (m_blLocalSQliteTransStartFlag == false)
					{
						DataMap = StatisticsFlow_Optimize2_GetFlowDataFromUpDataCache(pAddrItem, pObj->strBegin, pObj->strEnd);
						blStatistics = true;
					}
				}

				if (true == blStatistics)
				{
					CFlow_DataItem* pFlow = new CFlow_DataItem(pObj->Sunflowdata,pObj->Leiflowdata, pObj->stFTM, pObj->strFTM);
					StatisticsFlow_Optimize2_RealTime(pAddrItem, pFlow, DataMap);
					_DELETE(pFlow);

					ClearMultimap(DataMap);

					FlowList.push_back(chkItr->first);
				}
			}
		}

		if (FlowList.size() > 0)
		{
			std::list<CString>::iterator delItr;
			for (delItr = FlowList.begin();delItr != FlowList.end();delItr++)
			{
				pAddrItem->RemoveStatisticsOnTimerItem(CZ_Type_FLOW, (*delItr));
			}
			FlowList.clear();
		}
	}
}

void CDataTranSQLServer_SC::ScUpdataCahceCheck()
{
	SYSTEMTIME areaBegin,areaEnd;
	CalUpDataCacheTimeArea(areaBegin,areaEnd);
	std::list<CAddrMap_Item*>::iterator addrItr;
	for (addrItr = m_mapAddressMap.begin();addrItr != m_mapAddressMap.end();addrItr ++)
	{
		CAddrMap_Item* pAddr = (*addrItr);
		if (pAddr != NULL)
		{
			pAddr->SetUpDataCacheTimeArea(areaBegin,areaEnd);
			pAddr->m_UpDataCache.RemoveOutDataOnTimer();
		}
	}
}


void CDataTranSQLServer_SC::ScCacheInSQLServer()
{
	//检查数据并上传
	SYSTEMTIME curST;
	std::list<CAddrMap_Item*>::iterator addrItr;
	for (addrItr = m_mapAddressMap.begin();addrItr != m_mapAddressMap.end();addrItr ++)
	{
		CAddrMap_Item* pAddr = (*addrItr);
		if (pAddr != NULL)
		{
			if (pAddr->GetType() == CZ_Type_RAIN) //雨量
			{
				
				//#1  Table_ST_PPTN_R_Record
				std::map<CString,Table_ST_PPTN_R_Record*> tempMap;  //存储未入库的数据信息
				std::map<CString,Table_ST_PPTN_R_Record*>::iterator ttItr ;
				for (ttItr = pAddr->m_mapST_PPTN_R.begin();ttItr != pAddr->m_mapST_PPTN_R.end(); ttItr ++)
				{
					Table_ST_PPTN_R_Record* pObj = ttItr->second;
					if (pObj != NULL)
					{
						//check
						GetLocalTime(&curST);
						int comp = CSystemTimeOp::CompareSystemTime(&(pObj->FTM),&curST);
						if (comp == -1 || comp == 0)
						{
							this->UpdateRainfallTableRecord((*pObj),pObj->TYPE);
							_DELETE(pObj);
						}
						else if (comp == 1)
						{
							INT64 mus = CSystemTimeOp::MinusSystemTime(pObj->FTM,curST);
							if (mus <= m_dwChcekInSecond * 1000 )
							{
								this->UpdateRainfallTableRecord((*pObj),pObj->TYPE);
								_DELETE(pObj);
							}
							else
							{
								tempMap.insert(std::pair<CString, Table_ST_PPTN_R_Record*>(pObj->TM,pObj));
							}
						}
					}
				}

				//将tempMap 拷贝到m_mapST_PPTN_R
				pAddr->m_mapST_PPTN_R.clear();
				for (ttItr = tempMap.begin();ttItr != tempMap.end();ttItr++)
				{
					Table_ST_PPTN_R_Record* pObj = ttItr->second;
					if (pObj != NULL)
						pAddr->m_mapST_PPTN_R.insert(std::pair<CString, Table_ST_PPTN_R_Record*>(pObj->TM,pObj));
				}

				//#2 Table_ST_STORM_R_Record
				std::map<CString,Table_ST_STORM_R_Record*> tempMap2;
				std::map<CString,Table_ST_STORM_R_Record*>::iterator stromItr;
				for (stromItr = pAddr->m_mapST_STROM_R.begin(); stromItr != pAddr->m_mapST_STROM_R.end(); stromItr ++)
				{
					Table_ST_STORM_R_Record* pObj = stromItr->second;
					if (pObj != NULL)
					{
						//check
						GetLocalTime(&curST);
						int comp = CSystemTimeOp::CompareSystemTime(&(pObj->FTM),&curST);
						if (comp == -1 || comp == 0 )
						{
							this->UpdateRainstormTableRecord((*pObj));
							_DELETE(pObj);
						}
						else if (comp == 1)
						{
							INT64 mus = CSystemTimeOp::MinusSystemTime(pObj->FTM,curST);
							if (mus <= m_dwChcekInSecond * 1000)
							{
								this->UpdateRainstormTableRecord((*pObj));
								_DELETE(pObj);
							}
							else
							{
								tempMap2.insert(std::pair<CString, Table_ST_STORM_R_Record*>(pObj->TM,pObj));
							}
						}
					}
				}

				//将tempMap2 拷贝至m_mapST_STROM_R
				pAddr->m_mapST_STROM_R.clear();
				for (stromItr = tempMap2.begin();stromItr != tempMap2.end();stromItr ++)
				{
					Table_ST_STORM_R_Record* pObj = stromItr->second;
					if (pObj != NULL)
						pAddr->m_mapST_STROM_R.insert(std::pair<CString, Table_ST_STORM_R_Record*>(pObj->TM,pObj));
				}
			}
			else if (pAddr->GetType() == CZ_Type_WL) //水位
			{
				//#3 Table_ST_WL_R_Record
				std::map<CString,Table_ST_WL_R_Record*> tempMap;
				std::map<CString,Table_ST_WL_R_Record*>::iterator wlItr;
				
				for (wlItr = pAddr->m_mapST_WL_R.begin();wlItr != pAddr->m_mapST_WL_R.end();wlItr ++)
				{
					Table_ST_WL_R_Record* pObj = wlItr->second;
					if (pObj != NULL)
					{
						//check
						GetLocalTime(&curST);
						int comp = CSystemTimeOp::CompareSystemTime(&(pObj->FTM),&curST);
						if (comp == -1 || comp == 0)
						{
							this->UpdateWaterLevelData(pAddr,pObj->TM,pObj->PV);
							_DELETE(pObj);
						}
						else if (comp == 1)
						{
							INT64 mus = CSystemTimeOp::MinusSystemTime(pObj->FTM,curST);
							if (mus <= m_dwChcekInSecond * 1000)
							{
								this->UpdateWaterLevelData(pAddr,pObj->TM,pObj->PV);
								_DELETE(pObj);
							}
							else
							{
								tempMap.insert(std::pair<CString, Table_ST_WL_R_Record*>(pObj->TM,pObj));
							}
						}
						
					}
				}
				//将tempMap的数据拷贝至m_mapST_WL_R;
				pAddr->m_mapST_WL_R.clear();
				for (wlItr = tempMap.begin();wlItr != tempMap.end();wlItr ++)
				{
					Table_ST_WL_R_Record* pObj = wlItr->second;
					if (pObj != NULL)
					{
						pAddr->m_mapST_WL_R.insert(std::pair<CString, Table_ST_WL_R_Record*>(pObj->TM,pObj));
					}
				}
			}
			else if (pAddr->GetType() == CZ_Type_DISP) //深度位移传感器
			{
				//#3 Table_ST_DISP_R_Record
				std::map<CString,Table_ST_DISP_R_Record*> tempMap;
				std::map<CString,Table_ST_DISP_R_Record*>::iterator DISPItr;
				for (DISPItr = pAddr->m_mapST_DISP_R.begin();DISPItr != pAddr->m_mapST_DISP_R.end();DISPItr ++)
				{
					Table_ST_DISP_R_Record* pObj = DISPItr->second;
					if (pObj != NULL)
					{
						//check
						GetLocalTime(&curST);
						int comp = CSystemTimeOp::CompareSystemTime(&(pObj->FTM),&curST);
						if (comp == -1 || comp == 0)
						{
							this->UpdateDisplaceData(pAddr,pObj->TM,pObj->X,pObj->Y);
							_DELETE(pObj);
						}
						else if (comp == 1)
						{
							INT64 mus = CSystemTimeOp::MinusSystemTime(pObj->FTM,curST);
							if (mus <= m_dwChcekInSecond * 1000)
							{
								this->UpdateDisplaceData(pAddr,pObj->TM,pObj->X,pObj->Y);
								_DELETE(pObj);
							}
							else
							{
								tempMap.insert(std::pair<CString, Table_ST_DISP_R_Record*>(pObj->TM,pObj));
							}
						}
					}
				}

				//将tempMap的数据拷贝至m_mapST_DISP_R;
				pAddr->m_mapST_DISP_R.clear();
				for (DISPItr = tempMap.begin();DISPItr != tempMap.end();DISPItr ++)
				{
					Table_ST_DISP_R_Record* pObj = DISPItr->second;
					if (pObj != NULL)
					{
						pAddr->m_mapST_DISP_R.insert(std::pair<CString, Table_ST_DISP_R_Record*>(pObj->TM,pObj));
					}
				}
			}
			else if (pAddr->GetType() == CZ_Type_TEMP) //温度传感器
			{
				//#3 Table_ST_TEMP_R_Record
				std::map<CString,Table_ST_TEMP_R_Record*> tempMap;
				std::map<CString,Table_ST_TEMP_R_Record*>::iterator TEMPItr;
				for (TEMPItr = pAddr->m_mapST_TEMP_R.begin();TEMPItr != pAddr->m_mapST_TEMP_R.end();TEMPItr ++)
				{
					Table_ST_TEMP_R_Record* pObj = TEMPItr->second;
					if (pObj != NULL)
					{
						//check
						GetLocalTime(&curST);
						int comp = CSystemTimeOp::CompareSystemTime(&(pObj->FTM),&curST);
						if (comp == -1 || comp == 0)
						{
							this->UpdateTemperaData(pAddr,pObj->TM,pObj->T);
							_DELETE(pObj);
						}
						else if (comp == 1)
						{
							INT64 mus = CSystemTimeOp::MinusSystemTime(pObj->FTM,curST);
							if (mus <= m_dwChcekInSecond * 1000)
							{
								this->UpdateTemperaData(pAddr,pObj->TM,pObj->T);
								_DELETE(pObj);
							}
							else
							{
								tempMap.insert(std::pair<CString, Table_ST_TEMP_R_Record*>(pObj->TM,pObj));
							}
						}
					}
				}

				//将tempMap的数据拷贝至m_mapST_TEMP_R;
				pAddr->m_mapST_TEMP_R.clear();
				for (TEMPItr = tempMap.begin(); TEMPItr != tempMap.end(); TEMPItr ++)
				{
					Table_ST_TEMP_R_Record* pObj = TEMPItr->second;
					if (pObj != NULL)
					{
						pAddr->m_mapST_TEMP_R.insert(std::pair<CString, Table_ST_TEMP_R_Record*>(pObj->TM,pObj));
					}
				}
			}
			else if (pAddr->GetType() == CZ_Type_FLOW)    //流量传感器
           {
				 
				std::map<CString, Table_ST_RIVER_R_Record*> flowMapRiver_Value;
				std::map<CString, Table_ST_RIVER_R_Record*>::iterator wlItrRiver_Value;

				std::map<CString, Table_ST_RSVR_R_Record*> flowMapRsvr_Value;
				std::map<CString, Table_ST_RSVR_R_Record*>::iterator wlItrRsvr_Value;

				CString str = NULL;
		   //瞬时流量的记录
				for (wlItrRiver_Value = pAddr->m_mapST_RIVER_R.begin(); wlItrRiver_Value != pAddr->m_mapST_RIVER_R.end(); wlItrRiver_Value++)
				{
					Table_ST_RIVER_R_Record* pobjRiver_Value = wlItrRiver_Value->second;
					if (pobjRiver_Value != NULL)
					{
						FlowDatalist.push_back(pAddr->Get_STCD());
						FlowDatalist.push_back(pobjRiver_Value->TM);
						str.Format(_T("%3f"), pobjRiver_Value->Q);
						FlowDatalist.push_back(str);
						this->UpdateSunFlowData(pAddr,pobjRiver_Value->TM,pobjRiver_Value->Q/1000.0);   //瞬时流量
					}
					_DELETE(pobjRiver_Value);
				}

		   //累计流量的记录
				 for (wlItrRsvr_Value = pAddr->m_mapST_RSVR_R.begin(); wlItrRsvr_Value != pAddr->m_mapST_RSVR_R.end(); wlItrRsvr_Value++)
				 {
					 Table_ST_RSVR_R_Record* pobjRsvr_Value = wlItrRsvr_Value->second;
					 if (pobjRsvr_Value != NULL)
					 {
						 str.Format(_T("%3f"), pobjRsvr_Value->W);                          //流量数据格式化
						 FlowDatalist.push_back(str);   
						 this->UpdateLeiFlowData(pAddr, pobjRsvr_Value->TM,pobjRsvr_Value->W);    //累积流量
					 }
					 _DELETE(pobjRsvr_Value);
				 }
				//FlowDataPrint::FlowDatalist.push_back(flowData);      //将记录存进流量数据打印列表
		   //将tempMapRiver_Value的数据拷贝到m_mapST_RIVER_R
				pAddr->m_mapST_RIVER_R.clear();
				for (wlItrRiver_Value = flowMapRiver_Value.begin(); wlItrRiver_Value != flowMapRiver_Value.end(); wlItrRiver_Value++)
			    {
					Table_ST_RIVER_R_Record* pobjRiver_Value = wlItrRiver_Value->second;
					if (pobjRiver_Value != NULL)
					{
						pAddr->m_mapST_RIVER_R.insert(std::pair<CString, Table_ST_RIVER_R_Record*>(pobjRiver_Value->TM, pobjRiver_Value));
					}
				}
		   //将tempMapRsvr_Value的数据拷贝到m_mapST_RSVR_R
				pAddr->m_mapST_RSVR_R.clear();
				for (wlItrRsvr_Value = flowMapRsvr_Value.begin(); wlItrRsvr_Value != flowMapRsvr_Value.end(); wlItrRsvr_Value++)
				{
					 Table_ST_RSVR_R_Record* pobjRsvr_Value = wlItrRsvr_Value->second;
					if (pobjRsvr_Value != NULL)
					{
						 pAddr->m_mapST_RSVR_R.insert(std::pair<CString, Table_ST_RSVR_R_Record*>(pobjRsvr_Value->TM, pobjRsvr_Value));
					}
				}
			}
		}
	}
}

void CDataTranSQLServer_SC::ScCacheSaveToSQLite()
{
	if (m_blOpenScCacheSucc == true)
	{
		bool ret = true;
		SYSTEMTIME st;
		CString sSQL = "";
		//启动事务
		sSQL.Format("BEGIN");
		ExcuteScCacheNoQuery(sSQL);

		//修改原表名称
		sSQL.Format("ALTER TABLE CUR_STATE RENAME TO CUR_STATE_BAK");
		ExcuteScCacheNoQuery(sSQL);

		sSQL.Format("ALTER TABLE CACHE_HIS RENAME TO CACHE_HIS_BAK");
		ExcuteScCacheNoQuery(sSQL);

		sSQL.Format("ALTER TABLE ST_PPTN_R RENAME TO ST_PPTN_R_BAK");
		ExcuteScCacheNoQuery(sSQL);

		sSQL.Format("ALTER TABLE ST_STORM_R RENAME TO ST_STORM_R_BAK");
		ExcuteScCacheNoQuery(sSQL);

		sSQL.Format("ALTER TABLE ST_WL_R RENAME TO ST_WL_R_BAK");
		ExcuteScCacheNoQuery(sSQL);

		sSQL.Format("ALTER TABLE ST_DISP_R RENAME TO ST_DISP_R_BAK");
		ExcuteScCacheNoQuery(sSQL);

		sSQL.Format("ALTER TABLE ST_TEMP_R RENAME TO ST_TEMP_R_BAK");
		ExcuteScCacheNoQuery(sSQL);

		sSQL.Format("ALTER TABLE STC_OBJECTS RENAME TO STC_OBJECTS_BAK");
		ExcuteScCacheNoQuery(sSQL);

		//创建新表
		CheckScCacheSQLite();

		//存储数据
		//(1)存ST_PPTN_R
		//(2)存ST_STORM_R
		//(3)存ST_WL_R
		//(4)存CACHE_HIS
		std::list<CAddrMap_Item*>::iterator addrItr = m_mapAddressMap.end();
		for (addrItr = m_mapAddressMap.begin();
			 addrItr != m_mapAddressMap.end();
			 addrItr ++)
		{
			CAddrMap_Item* pAddr = (*addrItr);
			if (pAddr != NULL)
			{
				if (pAddr->GetType() == CZ_Type_RAIN)
				{
					std::map<CString,Table_ST_PPTN_R_Record*>::iterator stppItr ;//= pAddr->m_mapST_PPTN_R.end();
					for (stppItr = pAddr->m_mapST_PPTN_R.begin();
						 stppItr != pAddr->m_mapST_PPTN_R.end();
						 stppItr ++)
					{
						Table_ST_PPTN_R_Record* pObj = stppItr->second;
						if (pObj != NULL)
						{
							sSQL.Format("INSERT INTO ST_PPTN_R (STCD,TYPE,TM,FTM,DRP,INTV,PRD,DYP,WTH,CAL_TM) VALUES  ('%s','%d','%s','%s','%.1f','%.2f','%.2f','%.1f','%s','%s') ",
								pAddr->Get_STCD(),(int )pObj->TYPE,pObj->TM,
								CSystemTimeOp::ConvertSystemTimeToString(&(pObj->FTM),true),pObj->DRP,
								pObj->INTV,pObj->PRD,pObj->DYP,
								pObj->WTH,CSystemTimeOp::ConvertSystemTimeToString(&(pObj->CAL_TM),true));
							ExcuteScCacheNoQuery(sSQL);
						}
					}

					std::map<CString,Table_ST_STORM_R_Record*>::iterator ststormItr;
					for (ststormItr = pAddr->m_mapST_STROM_R.begin();
						 ststormItr != pAddr->m_mapST_STROM_R.end();
						 ststormItr ++)
					{
						Table_ST_STORM_R_Record* pObj = ststormItr->second;
						if (pObj != NULL)
						{
							sSQL.Format("INSERT INTO ST_STORM_R (STCD,TM,FTM,STRMDR,STRMP,WTH,CAL_TM) VALUES ('%s','%s','%s','0.15','%.1f','7','%s')",
								pAddr->Get_STCD(),pObj->TM,CSystemTimeOp::ConvertSystemTimeToString(&(pObj->FTM),true),
								pObj->STRMP,CSystemTimeOp::ConvertSystemTimeToString(&(pObj->CAL_TM),true));
							ExcuteScCacheNoQuery(sSQL);
						}
					}

					std::map<CString,StatisticsOnTimer_Item*>::iterator stItr;
					for (stItr = pAddr->m_mapStatistics_Rain.begin();
						 stItr != pAddr->m_mapStatistics_Rain.end();
						 stItr ++)
					{
						StatisticsOnTimer_Item* pObj = stItr->second;
						if (pObj != NULL)
						{
							sSQL.Format("INSERT INTO STC_OBJECTS (STCD,TYPE,WLTYPE,TM,CAL_TYPE,FTM,BEGIN_ST,END_ST,CAL_TM,PV,X,Y,T,COLLECT) VALUES ('%s','%d',0,'%s','%d','%s','%s','%s','%s','%f','%f','%f','%f','%s')",
								pAddr->Get_STCD(),(int)pObj->Type,pObj->strTM,(int)pObj->Cal_Type,
								CSystemTimeOp::ConvertSystemTimeToString(&(pObj->stFTM),true),
								CSystemTimeOp::ConvertSystemTimeToString(&(pObj->stBegin),true),
								CSystemTimeOp::ConvertSystemTimeToString(&(pObj->stEnd),true),
								CSystemTimeOp::ConvertSystemTimeToString(&(pObj->CAL_TM),true),pObj->PV,0,0,0,
								CSystemTimeOp::ConvertSystemTimeToString(&(pObj->Collect),true));
							ExcuteScCacheNoQuery(sSQL);
						}
					}
				}
				else if (pAddr->GetType() == CZ_Type_WL)
				{
					std::map<CString,Table_ST_WL_R_Record*>::iterator stwlItr;
					for (stwlItr = pAddr->m_mapST_WL_R.begin();
						 stwlItr != pAddr->m_mapST_WL_R.end();
						 stwlItr ++)
					{
						Table_ST_WL_R_Record* pObj = stwlItr->second;
						if (pObj != NULL)
						{
							sSQL.Format("INSERT INTO ST_WL_R (STCD,TYPE,TM,FTM,PV,CAL_TM) VALUES ('%s','%d','%s','%s','%.2f','%s')",
								pAddr->Get_STCD(),(int)pAddr->GetWlType(),
								pObj->TM,CSystemTimeOp::ConvertSystemTimeToString(&(pObj->FTM),true),
								pObj->PV,CSystemTimeOp::ConvertSystemTimeToString(&(pObj->CAL_TM),true));
							ExcuteScCacheNoQuery(sSQL);
						}
					}

					std::map<CString,StatisticsOnTimer_Item*>::iterator stItr;
					for (stItr = pAddr->m_mapStatistics_Rain.begin();
						stItr != pAddr->m_mapStatistics_Rain.end();
						stItr ++)
					{
						StatisticsOnTimer_Item* pObj = stItr->second;
						if (pObj != NULL)
						{
							sSQL.Format("INSERT INTO STC_OBJECTS (STCD,TYPE,WLTYPE,TM,CAL_TYPE,FTM,BEGIN_ST,END_ST,CAL_TM,PV,X,Y,T,COLLECT) VALUES ('%s','%d','%d','%s','%d','%s','%s','%s','%s','%f','%f','%f','%f','%s')",
								pAddr->Get_STCD(),(int)pObj->Type,(int)pObj->WLType,pObj->strTM,(int)pObj->Cal_Type,
								CSystemTimeOp::ConvertSystemTimeToString(&(pObj->stFTM),true),
								CSystemTimeOp::ConvertSystemTimeToString(&(pObj->stBegin),true),
								CSystemTimeOp::ConvertSystemTimeToString(&(pObj->stEnd),true),
								CSystemTimeOp::ConvertSystemTimeToString(&(pObj->CAL_TM),true),pObj->PV,0,0,0,
								CSystemTimeOp::ConvertSystemTimeToString(&(pObj->Collect),true));
							ExcuteScCacheNoQuery(sSQL);
						}
					}
				}
				else if (pAddr->GetType() == CZ_Type_DISP)
				{
					std::map<CString,Table_ST_DISP_R_Record*>::iterator stwlItr;
					for (stwlItr = pAddr->m_mapST_DISP_R.begin();
						 stwlItr != pAddr->m_mapST_DISP_R.end();
						 stwlItr ++)
					{
						Table_ST_DISP_R_Record* pObj = stwlItr->second;
						if (pObj != NULL)
						{
							sSQL.Format("INSERT INTO ST_DISP_R (STCD,TM,FTM,X,Y,CAL_TM) VALUES ('%s','%s','%s','%.2f','%.2f','%s')",
								pAddr->Get_STCD(),
								pObj->TM,CSystemTimeOp::ConvertSystemTimeToString(&(pObj->FTM),true),
								pObj->X,pObj->Y,CSystemTimeOp::ConvertSystemTimeToString(&(pObj->CAL_TM),true));
							ExcuteScCacheNoQuery(sSQL);
						}
					}

					std::map<CString,StatisticsOnTimer_Item*>::iterator stItr;
					for (stItr = pAddr->m_mapStatistics_Rain.begin();
						stItr != pAddr->m_mapStatistics_Rain.end();
						stItr ++)
					{
						StatisticsOnTimer_Item* pObj = stItr->second;
						if (pObj != NULL)
						{
							sSQL.Format("INSERT INTO STC_OBJECTS (STCD,TYPE,WLTYPE,TM,CAL_TYPE,FTM,BEGIN_ST,END_ST,CAL_TM,PV,X,Y,T,COLLECT) VALUES ('%s','%d','%d','%s','%d','%s','%s','%s','%s','%f','%f','%f','%f','%s')",
								pAddr->Get_STCD(),(int)pObj->Type,(int)pObj->WLType,pObj->strTM,(int)pObj->Cal_Type,
								CSystemTimeOp::ConvertSystemTimeToString(&(pObj->stFTM),true),
								CSystemTimeOp::ConvertSystemTimeToString(&(pObj->stBegin),true),
								CSystemTimeOp::ConvertSystemTimeToString(&(pObj->stEnd),true),
								CSystemTimeOp::ConvertSystemTimeToString(&(pObj->CAL_TM),true),0,pObj->X,pObj->Y,0,
								CSystemTimeOp::ConvertSystemTimeToString(&(pObj->Collect),true));
							ExcuteScCacheNoQuery(sSQL);
						}
					}
				}
				else if (pAddr->GetType() == CZ_Type_TEMP)
				{
					std::map<CString,Table_ST_TEMP_R_Record*>::iterator stwlItr;
					for (stwlItr = pAddr->m_mapST_TEMP_R.begin();
						 stwlItr != pAddr->m_mapST_TEMP_R.end();
						 stwlItr ++)
					{
						Table_ST_TEMP_R_Record* pObj = stwlItr->second;
						if (pObj != NULL)
						{
							sSQL.Format("INSERT INTO ST_TEMP_R (STCD,TM,FTM,X,Y,CAL_TM) VALUES ('%s','%s','%s','%.2f','%s')",
								pAddr->Get_STCD(),
								pObj->TM,CSystemTimeOp::ConvertSystemTimeToString(&(pObj->FTM),true),
								pObj->T,CSystemTimeOp::ConvertSystemTimeToString(&(pObj->CAL_TM),true));
							ExcuteScCacheNoQuery(sSQL);
						}
					}

					std::map<CString,StatisticsOnTimer_Item*>::iterator stItr;
					for (stItr = pAddr->m_mapStatistics_Rain.begin();
						stItr != pAddr->m_mapStatistics_Rain.end();
						stItr ++)
					{
						StatisticsOnTimer_Item* pObj = stItr->second;
						if (pObj != NULL)
						{
							sSQL.Format("INSERT INTO STC_OBJECTS (STCD,TYPE,WLTYPE,TM,CAL_TYPE,FTM,BEGIN_ST,END_ST,CAL_TM,PV,X,Y,T,COLLECT) VALUES ('%s','%d','%d','%s','%d','%s','%s','%s','%s','%f','%f','%f','%f','%s')",
								pAddr->Get_STCD(),(int)pObj->Type,(int)pObj->WLType,pObj->strTM,(int)pObj->Cal_Type,
								CSystemTimeOp::ConvertSystemTimeToString(&(pObj->stFTM),true),
								CSystemTimeOp::ConvertSystemTimeToString(&(pObj->stBegin),true),
								CSystemTimeOp::ConvertSystemTimeToString(&(pObj->stEnd),true),
								CSystemTimeOp::ConvertSystemTimeToString(&(pObj->CAL_TM),true),0,0,0,pObj->T,
								CSystemTimeOp::ConvertSystemTimeToString(&(pObj->Collect),true));
							ExcuteScCacheNoQuery(sSQL);
						}
					}
				}
			}
		}

		GetLocalTime(&st);
		CString strTM = CSystemTimeOp::ConvertSystemTimeToString(&st,true);
		sSQL.Format("INSERT INTO CACHE_HIS (TABLENAME,SAVETM) VALUES ('%s','%s')",
			"ST_PPTN_R",strTM);
		ExcuteScCacheNoQuery(sSQL);
		sSQL.Format("INSERT INTO CACHE_HIS (TABLENAME,SAVETM) VALUES ('%s','%s')",
			"ST_STORM_R",strTM);
		ExcuteScCacheNoQuery(sSQL);
		sSQL.Format("INSERT INTO CACHE_HIS (TABLENAME,SAVETM) VALUES ('%s','%s')",
			"ST_WL_R",strTM);
		ExcuteScCacheNoQuery(sSQL);
		sSQL.Format("INSERT INTO CACHE_HIS (TABLENAME,SAVETM) VALUES ('%s','%s')",
			"ST_DISP_R",strTM);
		ExcuteScCacheNoQuery(sSQL);
		sSQL.Format("INSERT INTO CACHE_HIS (TABLENAME,SAVETM) VALUES ('%s','%s')",
			"ST_TEMP_R",strTM);
		ExcuteScCacheNoQuery(sSQL);
		sSQL.Format("INSERT INTO CACHE_HIS (TABLENAME,SAVETM) VALUES ('%s','%s')",
			"STC_OBJECTS",strTM);
		ExcuteScCacheNoQuery(sSQL);

		sSQL.Format("INSERT INTO CUR_STATE (WAITCOUNT,CURDEALCOLL,CURDELARECV,SAVETM) VALUES ('%d','%s','%s','%s')",
			m_nWaittingBufferCount,
			CSystemTimeOp::ConvertSystemTimeToString(&curDealCollectTime,true),
			CSystemTimeOp::ConvertSystemTimeToString(&curDealReciveTime,true),
			strTM);
		ExcuteScCacheNoQuery(sSQL);

		//删除原表
		sSQL.Format("DROP TABLE CUR_STATE_BAK");
		ExcuteScCacheNoQuery(sSQL);

		sSQL.Format("DROP TABLE CACHE_HIS_BAK");
		ExcuteScCacheNoQuery(sSQL);
		
		sSQL.Format("DROP TABLE ST_PPTN_R_BAK");
		ExcuteScCacheNoQuery(sSQL);

		sSQL.Format("DROP TABLE ST_STORM_R_BAK");
		ExcuteScCacheNoQuery(sSQL);

		sSQL.Format("DROP TABLE ST_WL_R_BAK");
		ExcuteScCacheNoQuery(sSQL);

		sSQL.Format("DROP TABLE ST_DISP_R_BAK");
		ExcuteScCacheNoQuery(sSQL);

		sSQL.Format("DROP TABLE ST_TEMP_R_BAK");
		ExcuteScCacheNoQuery(sSQL);

		sSQL.Format("DROP TABLE STC_OBJECTS_BAK");
		ExcuteScCacheNoQuery(sSQL);

		//提交事务
		sSQL.Format("COMMIT");
		ExcuteScCacheNoQuery(sSQL);
	}
}

void CDataTranSQLServer_SC::ReadScCacheSQLiteByStart()
{
   //本模块初始化时，读取sc.cache.db数据（缓存SQLite数据文件）
	if (m_blOpenScCacheSucc == true)
	{
		ReadScCacheSQLite_ST_PPTN_R();
		ReadScCacheSQLite_ST_STORM_R();
		ReadScCacheSQLite_ST_WL_R();
		ReadScCacheSQLite_ST_DISP_R();
		ReadScCacheSQLite_ST_TEMP_R();
		ReadScCacheSQlite_STCObjects();
		ReadScCacheSQlite_FLOWV_DATA();
	}
}

void CDataTranSQLServer_SC::ReadScCacheSQLite_ST_PPTN_R()
{
	CString sSQL = "";
	CString sTemp = "";
	try
	{
		std::list<CAddrMap_Item*>::iterator addrItr;

		sSQL.Format("SELECT STCD,TYPE,TM,FTM,DRP,INTV,PRD,DYP,WTH,CAL_TM FROM ST_PPTN_R");
		CSqlStatement* rs = ExcuteScCacheQuery(sSQL);
		if (rs != NULL)
		{
			Table_ST_PPTN_R_Record trow ;
			while(rs->NextRow())
			{
				sTemp = rs->ValueString(0);
				trow.STCD = sTemp.Trim();

				sTemp = rs->ValueString(1);
				sTemp.Trim();
				trow.TYPE = (RIANSTATISTICS_TYPE_Enum) atoi(sTemp);

				sTemp = rs->ValueString(2);
				trow.TM = sTemp.Trim();

				sTemp = rs->ValueString(3);
				sTemp.Trim();
				CSystemTimeOp::ConvertStringToSysemTime(sTemp,&(trow.FTM));

				sTemp = rs->ValueString(4);
				sTemp.Trim();
				trow.DRP = atof(sTemp);

				sTemp = rs->ValueString(5);
				sTemp.Trim();
				trow.INTV = atof(sTemp);

				sTemp = rs->ValueString(6);
				sTemp.Trim();
				trow.PRD = atof(sTemp);

				sTemp = rs->ValueString(7);
				sTemp.Trim();
				trow.DYP = atof(sTemp);

				sTemp = rs->ValueString(8);
				sTemp.Trim();
				trow.WTH = sTemp;

				sTemp = rs->ValueString(9);
				sTemp.Trim();
				CSystemTimeOp::ConvertStringToSysemTime(sTemp,&(trow.CAL_TM));

				for (addrItr = m_mapAddressMap.begin(); 
					addrItr != m_mapAddressMap.end();
					addrItr ++)
				{
					CAddrMap_Item* pAddr = (*addrItr);
					if (pAddr != NULL && 
						pAddr->GetType() ==CZ_Type_RAIN && 
						pAddr->Get_STCD() == trow.STCD)
					{
						pAddr->UpdateST_PPTN_R_Record(&trow);
						break;
					}
				}
			}

			_DELETE(rs);
		}
	}
	catch (CException* e)
	{
		TRACE("ReadScCacheSQLite_ST_PPTN_R() error\r\n");
		e->Delete();
	}
}
void CDataTranSQLServer_SC::ReadScCacheSQlite_FLOWV_DATA() {
	CString sSQL = "";
	CString sTemp = "";
	try
	{
		std::list<CAddrMap_Item*>::iterator addrItr;
		sSQL.Format("SELECT TOTAL_WATER,ANYTIME_FLOW FROM FLOWV_DATA");
		CSqlStatement* rs = ExcuteScCacheQuery(sSQL);
		if (rs != NULL)
		{
			Table_ST_RSVR_R_Record trow1;    //累计流量
			Table_ST_RIVER_R_Record trow2;   //瞬时流量
			while (rs->NextRow())
			{
				sTemp = rs->ValueString(2);
				trow1.W = atof(sTemp.Trim())/10.0; //累计流量
				
				sTemp = rs->ValueString(3);
				trow2.Q = atof(sTemp.Trim())/1000.0; //瞬时流量

				sTemp = rs->ValueString(5);  //数据保存时间
				trow1.TM = sTemp.Trim();
				trow2.TM = sTemp.Trim();

				for (addrItr = m_mapAddressMap.begin();addrItr != m_mapAddressMap.end();addrItr++)
				{
					CAddrMap_Item* pAddr = (*addrItr);
					if (pAddr != NULL &&pAddr->GetType() == CZ_Type_FLOW)
					{
						pAddr->UpdateST_RSVR_R_Record(&trow1);       //将相关数据更新到RSVR和RIVER两张表中
						pAddr->UpdateST_RIVER_R_Record(&trow2);
						break;
					}
				}
			}
			_DELETE(rs);
		}
	}
	catch (CException* e)
	{
		TRACE("ReadScCacheSQLite_ST_RSVR_R() error\r\n");
		TRACE("ReadScCacheSQLite_ST_RIVER_R() error\r\n");
		e->Delete();
	}
}

void CDataTranSQLServer_SC::ReadScCacheSQLite_ST_STORM_R()
{
	CString sSQL = "";
	CString sTemp = "";
	try
	{
		std::list<CAddrMap_Item*>::iterator addrItr;

		sSQL.Format("SELECT STCD,TM,FTM,STRMDR,STRMP,WTH,CAL_TM FROM ST_STORM_R");
		CSqlStatement* rs = ExcuteScCacheQuery(sSQL);
		if (rs != NULL)
		{
			Table_ST_STORM_R_Record trow ;
			while(rs->NextRow())
			{
				sTemp = rs->ValueString(0);
				trow.STCD = sTemp.Trim();

				sTemp = rs->ValueString(1);
				trow.TM = sTemp.Trim();

				sTemp = rs->ValueString(2);
				CSystemTimeOp::ConvertStringToSysemTime(sTemp.Trim(),&(trow.FTM));

				sTemp = rs->ValueString(3);
				trow.STRMDR = atof(sTemp.Trim());

				sTemp = rs->ValueString(4);
				trow.STRMP = atof(sTemp.Trim());

				sTemp = rs->ValueString(5);
				trow.WTH = sTemp.Trim();

				sTemp = rs->ValueString(6);
				CSystemTimeOp::ConvertStringToSysemTime(sTemp.Trim(),&(trow.CAL_TM));

				for (addrItr = m_mapAddressMap.begin(); 
					addrItr != m_mapAddressMap.end();
					addrItr ++)
				{
					CAddrMap_Item* pAddr = (*addrItr);
					if (pAddr != NULL && 
						pAddr->GetType( ) ==CZ_Type_RAIN && 
						pAddr->Get_STCD() == trow.STCD)
					{
						pAddr->UpdateST_STROM_R_Record(&trow);
						break;
					}
				}
			}

			_DELETE(rs);
		}
	}
	catch (CException* e)
	{
		TRACE("ReadScCacheSQLite_ST_STORM_R() error\r\n");
		e->Delete();
	}
}

void CDataTranSQLServer_SC::ReadScCacheSQLite_ST_WL_R()
{
	CString sSQL = "";
	CString sTemp = "";
	try
	{
		std::list<CAddrMap_Item*>::iterator addrItr;

		sSQL.Format("SELECT STCD,TYPE,TM,FTM,PV,CAL_TM FROM ST_WL_R");
		CSqlStatement* rs = ExcuteScCacheQuery(sSQL);
		if (rs != NULL)
		{
			Table_ST_WL_R_Record trow ;
			while(rs->NextRow())
			{
				sTemp = rs->ValueString(0);
				trow.STCD = sTemp.Trim();

				sTemp = rs->ValueString(1);
				trow.TYPE = atoi(sTemp);

				sTemp = rs->ValueString(2);
				trow.TM = sTemp.Trim();

				sTemp = rs->ValueString(3);
				CSystemTimeOp::ConvertStringToSysemTime(sTemp.Trim(),&(trow.FTM));

				sTemp = rs->ValueString(4);
				trow.PV =atof(sTemp.Trim());

				sTemp = rs->ValueString(5);
				CSystemTimeOp::ConvertStringToSysemTime(sTemp.Trim(),&(trow.CAL_TM));

				for (addrItr = m_mapAddressMap.begin(); 
					addrItr != m_mapAddressMap.end();
					addrItr ++)
				{
					CAddrMap_Item* pAddr = (*addrItr);
					if (pAddr != NULL && 
						pAddr->GetType() ==CZ_Type_WL && 
						pAddr->Get_STCD() == trow.STCD && 
						pAddr->GetWlType() == (WL_Type_Enum)trow.TYPE)
					{
						pAddr->UpdateST_WL_R_Record(&trow);
						break;
					}
				}
			}

			_DELETE(rs);
		}
	}
	catch (CException* e)
	{
		TRACE("ReadScCacheSQLite_ST_WL_R() error\r\n");
		e->Delete();
	}
}

void CDataTranSQLServer_SC::ReadScCacheSQLite_ST_DISP_R()
{
	CString sSQL = "";
	CString sTemp = "";
	try
	{
		std::list<CAddrMap_Item*>::iterator addrItr;

		sSQL.Format("SELECT STCD,TM,FTM,X,Y,CAL_TM FROM ST_DISP_R");
		CSqlStatement* rs = ExcuteScCacheQuery(sSQL);
		if (rs != NULL)
		{
			Table_ST_DISP_R_Record trow ;
			while(rs->NextRow())
			{
				sTemp = rs->ValueString(0);
				trow.STCD = sTemp.Trim();

				sTemp = rs->ValueString(1);
				trow.TM = sTemp.Trim();

				sTemp = rs->ValueString(2);
				CSystemTimeOp::ConvertStringToSysemTime(sTemp.Trim(),&(trow.FTM));

				sTemp = rs->ValueString(3);
				trow.X =atof(sTemp.Trim());

				sTemp = rs->ValueString(4);
				trow.Y =atof(sTemp.Trim());

				sTemp = rs->ValueString(5);
				CSystemTimeOp::ConvertStringToSysemTime(sTemp.Trim(),&(trow.CAL_TM));

				for (addrItr = m_mapAddressMap.begin(); 
					addrItr != m_mapAddressMap.end();
					addrItr ++)
				{
					CAddrMap_Item* pAddr = (*addrItr);
					if (pAddr != NULL && 
						pAddr->GetType() ==CZ_Type_DISP && 
						pAddr->Get_STCD() == trow.STCD )
					{
						pAddr->UpdateST_DISP_R_Record(&trow);
						break;
					}
				}
			}

			_DELETE(rs);
		}
	}
	catch (CException* e)
	{
		TRACE("ReadScCacheSQLite_ST_DISP_R() error\r\n");
		e->Delete();
	}
}

void CDataTranSQLServer_SC::ReadScCacheSQLite_ST_TEMP_R()
{
	CString sSQL = "";
	CString sTemp = "";
	try
	{
		std::list<CAddrMap_Item*>::iterator addrItr;

		sSQL.Format("SELECT STCD,TYPE,TM,FTM,T,CAL_TM FROM ST_TEMP_R");
		CSqlStatement* rs = ExcuteScCacheQuery(sSQL);
		if (rs != NULL)
		{
			Table_ST_TEMP_R_Record trow ;
			while(rs->NextRow())
			{
				sTemp = rs->ValueString(0);
				trow.STCD = sTemp.Trim();

				sTemp = rs->ValueString(1);
				trow.TM = sTemp.Trim();

				sTemp = rs->ValueString(2);
				CSystemTimeOp::ConvertStringToSysemTime(sTemp.Trim(),&(trow.FTM));

				sTemp = rs->ValueString(3);
				trow.T =atof(sTemp.Trim());

				sTemp = rs->ValueString(4);
				CSystemTimeOp::ConvertStringToSysemTime(sTemp.Trim(),&(trow.CAL_TM));

				for (addrItr = m_mapAddressMap.begin(); 
					addrItr != m_mapAddressMap.end();
					addrItr ++)
				{
					CAddrMap_Item* pAddr = (*addrItr);
					if (pAddr != NULL && 
						pAddr->GetType() ==CZ_Type_TEMP && 
						pAddr->Get_STCD() == trow.STCD )
					{
						pAddr->UpdateST_TEMP_R_Record(&trow);
						break;
					}
				}
			}

			_DELETE(rs);
		}
	}
	catch (CException* e)
	{
		TRACE("ReadScCacheSQLite_ST_TEMP_R() error\r\n");
		e->Delete();
	}
}

void CDataTranSQLServer_SC::ReadScCacheSQlite_STCObjects()
{
	CString sSQL = "";
	CString sTemp = "";
	try
	{
		std::list<CAddrMap_Item*>::iterator addrItr;

		sSQL.Format("SELECT STCD,TYPE,WLTYPE,TM,CAL_TYPE,FTM,BEGIN_ST,END_ST,CAL_TM,PV,X,Y,T,COLLECT FROM STC_OBJECTS");
		CSqlStatement* rs = ExcuteScCacheQuery(sSQL);
		if (rs != NULL)
		{
			StatisticsOnTimer_Item trow ;
			while(rs->NextRow())
			{
				sTemp = rs->ValueString(0);
				trow.STCD = sTemp.Trim();

				sTemp = rs->ValueString(1);
				trow.Type = (CZ_Type_Enum)atoi(sTemp);

				sTemp = rs->ValueString(2);
				trow.WLType =(WL_Type_Enum) atoi(sTemp);

				sTemp = rs->ValueString(3);
				trow.strTM = sTemp.Trim();

				sTemp = rs->ValueString(4);
				trow.Cal_Type =(RIANSTATISTICS_TYPE_Enum)atoi(sTemp.Trim());

				sTemp = rs->ValueString(5);
				trow.strFTM = sTemp.Trim();
				CSystemTimeOp::ConvertStringToSysemTime(sTemp,&(trow.stFTM));

				sTemp = rs->ValueString(6);
				trow.strBegin = sTemp.Trim();
				CSystemTimeOp::ConvertStringToSysemTime(sTemp,&(trow.stBegin));

				sTemp = rs->ValueString(7);
				trow.strEnd = sTemp.Trim();
				CSystemTimeOp::ConvertStringToSysemTime(sTemp,&(trow.stEnd));

				sTemp = rs->ValueString(8);
				CSystemTimeOp::ConvertStringToSysemTime(sTemp.Trim(),&(trow.CAL_TM));

				sTemp = rs->ValueString(9);
				trow.PV = atof(sTemp.Trim());

				sTemp = rs->ValueString(10);
				trow.X = atof(sTemp.Trim());

				sTemp = rs->ValueString(11);
				trow.Y = atof(sTemp.Trim());

				sTemp = rs->ValueString(12);
				trow.T = atof(sTemp.Trim());

				sTemp = rs->ValueString(13);
				CSystemTimeOp::ConvertStringToSysemTime(sTemp.Trim(),&(trow.Collect));

				for (addrItr = m_mapAddressMap.begin(); 
					addrItr != m_mapAddressMap.end();
					addrItr ++)
				{
					CAddrMap_Item* pAddr = (*addrItr);
					if (pAddr != NULL && 
						pAddr->Get_STCD() == trow.STCD )
					{
						pAddr->UpdateStatisticsOnTimerItem(&trow);
						break;
					}
				}
			}

			_DELETE(rs);
		}
	}
	catch (CException* e)
	{
		TRACE("ReadScCacheSQlite_STCObjects() error\r\n");
		e->Delete();
	}
}


STRUCT_BEGINEND_AREA CDataTranSQLServer_SC::Get5MinuteArea( SYSTEMTIME* pST )
{
	STRUCT_BEGINEND_AREA ret;
	ret.blSucc = false;

	SYSTEMTIME colltime;
	if (pST != NULL)
		colltime = (*pST);
	else
		GetLocalTime(&colltime);
	

	SYSTEMTIME _tempsystemtime = colltime;
	_tempsystemtime.wMinute = 0;
	_tempsystemtime.wSecond = 1;
	_tempsystemtime.wMilliseconds = 0;

	WORD mut = colltime.wMinute / 5;
	WORD mod = colltime.wMinute % 5;

	CTime _beginHour_ctime(_tempsystemtime);
	CTimeSpan _t_add_span(0,0,5 * mut,0);
	CTimeSpan _t_add5_span(0,0,5,0);

	SYSTEMTIME begintime,endtime;
	if (mod == 0 && colltime.wSecond == 0)
	{
		CTime _tempctime_end = _beginHour_ctime + _t_add_span;
		CTime _tempctime_begin  = _tempctime_end - _t_add5_span;
		CSystemTimeOp::ConvertCTimeToSystemtime(&_tempctime_end,&endtime);
		CSystemTimeOp::ConvertCTimeToSystemtime(&_tempctime_begin,&begintime);
	}
	else
	{
		CTime _tempctime_begin = _beginHour_ctime + _t_add_span;
		CTime _tempctime_end   = _tempctime_begin + _t_add5_span;
		CSystemTimeOp::ConvertCTimeToSystemtime(&_tempctime_begin,&begintime);
		CSystemTimeOp::ConvertCTimeToSystemtime(&_tempctime_end,&endtime);
	}

	ret.stBegin = begintime;
	ret.stEnd = endtime;
	ret.blSucc = true;
	
#ifdef DEBUG
	SYSTEMTIME debugst;
	GetLocalTime(&debugst);
	TRACE("%s 5Minutes Area,point(%s),begin(%s),end(%s)\r\n",
		CSystemTimeOp::ConvertSystemTimeToString(&debugst,true),
		CSystemTimeOp::ConvertSystemTimeToString(&colltime,true),
		CSystemTimeOp::ConvertSystemTimeToString(&(ret.stBegin),true),
		CSystemTimeOp::ConvertSystemTimeToString(&(ret.stEnd),true));
#endif

	return ret;
}

STRUCT_BEGINEND_AREA CDataTranSQLServer_SC::Get15MinuteArea( SYSTEMTIME* pST )
{
	STRUCT_BEGINEND_AREA ret;
	ret.blSucc = false;

	//CString strBegin ="",strEnd = "";
	//CString strPoint = "";      //接口数据库TM 字段的值

	SYSTEMTIME collectTime ;
	if (pST != NULL)
		collectTime = (*pST);
	else
		GetLocalTime(&collectTime);

	SYSTEMTIME _tempsystemtime = collectTime;
	_tempsystemtime.wMinute = 0;
	_tempsystemtime.wSecond = 1;
	_tempsystemtime.wMilliseconds = 0;

	WORD mut = collectTime.wMinute / 15;
	WORD mod = collectTime.wMinute % 15;

	CTime _beginhour(_tempsystemtime);
	CTimeSpan _tspan_add(0,0,15 * mut,0);
	CTimeSpan _tspan_15(0,0,15,0);

	SYSTEMTIME begintime,endtime;
	if (mod == 0 && collectTime.wSecond == 0)
	{
		CTime _end_ctime = _beginhour + _tspan_add;
		CTime _begin_ctime = _end_ctime - _tspan_15;
		CSystemTimeOp::ConvertCTimeToSystemtime(&_begin_ctime,&begintime);
		CSystemTimeOp::ConvertCTimeToSystemtime(&_end_ctime,&endtime);
	}
	else
	{
		CTime _begin_ctime = _beginhour + _tspan_add;
		CTime _end_ctime = _begin_ctime + _tspan_15;
		CSystemTimeOp::ConvertCTimeToSystemtime(&_begin_ctime,&begintime);
		CSystemTimeOp::ConvertCTimeToSystemtime(&_end_ctime,&endtime);
	}

	//strBegin = CSystemTimeOp::ConvertSystemTimeToString(&begintime,true);
	//strEnd = CSystemTimeOp::ConvertSystemTimeToString(&endtime,true);
	//strPoint.Format("%04d-%02d-%02d %02d:%02d",
	//	endtime.wYear,endtime.wMonth,endtime.wDay,endtime.wHour,endtime.wMinute);

	ret.stBegin = begintime;
	ret.stEnd = endtime;
	ret.blSucc = true;


#ifdef DEBUG
	SYSTEMTIME debugst;
	GetLocalTime(&debugst);
	TRACE("%s 15Minutes Area,point(%s),begin(%s),end(%s)\r\n",
		CSystemTimeOp::ConvertSystemTimeToString(&debugst,true),
		CSystemTimeOp::ConvertSystemTimeToString(&collectTime,true),
		CSystemTimeOp::ConvertSystemTimeToString(&(ret.stBegin),true),
		CSystemTimeOp::ConvertSystemTimeToString(&(ret.stEnd),true));
#endif

	return ret;
}

STRUCT_BEGINEND_AREA CDataTranSQLServer_SC::Get1HourArea( SYSTEMTIME* pST )
{
	STRUCT_BEGINEND_AREA ret;
	ret.blSucc = false;

	//CString strBegin ="",strEnd = "";
	//CString strPoint = "";      //接口数据库TM 字段的值

	SYSTEMTIME collectTime;
	if (pST != NULL)
		collectTime = (*pST);
	else
		GetLocalTime(&collectTime);

	SYSTEMTIME _tempsystemtime = collectTime;
	_tempsystemtime.wMinute = 0;
	_tempsystemtime.wSecond = 1;
	_tempsystemtime.wMilliseconds = 0;

	CTime _beginhour(_tempsystemtime);
	CTimeSpan _tspan_1hour(0,1,0,0);

	SYSTEMTIME begintime,endtime;

	if (collectTime.wMinute == 0 &&	collectTime.wSecond == 0)      //整点数据
	{
		endtime = _tempsystemtime;
		CTime _temp_begin_ctime = _beginhour - _tspan_1hour;
		CSystemTimeOp::ConvertCTimeToSystemtime(&_temp_begin_ctime,&begintime);
	}
	else									
	{
		begintime = _tempsystemtime;
		CTime _temp_end_ctime = _beginhour + _tspan_1hour;
		CSystemTimeOp::ConvertCTimeToSystemtime(&_temp_end_ctime,&endtime);
	}

	//strBegin = CSystemTimeOp::ConvertSystemTimeToString(&begintime,true);
	//strEnd = CSystemTimeOp::ConvertSystemTimeToString(&endtime,true);
	//strPoint.Format("%04d-%02d-%02d %02d:00",
	//	endtime.wYear,endtime.wMonth,endtime.wDay,endtime.wHour);

	ret.stBegin = begintime;
	ret.stEnd = endtime;
	ret.blSucc = true;


#ifdef DEBUG
	SYSTEMTIME debugst;
	GetLocalTime(&debugst);
	TRACE("%s 1Hour Area,point(%s),begin(%s),end(%s)\r\n",
		CSystemTimeOp::ConvertSystemTimeToString(&debugst,true),
		CSystemTimeOp::ConvertSystemTimeToString(&collectTime,true),
		CSystemTimeOp::ConvertSystemTimeToString(&(ret.stBegin),true),
		CSystemTimeOp::ConvertSystemTimeToString(&(ret.stEnd),true));
#endif

	return ret;
}

STRUCT_BEGINEND_AREA CDataTranSQLServer_SC::Get1DayArea( SYSTEMTIME* pST )
{
	STRUCT_BEGINEND_AREA ret;
	ret.blSucc = false;


	CString strBegin="",strEnd="";

	SYSTEMTIME collecttime ;
	if (pST != NULL)
		collecttime = (*pST);
	else
		GetLocalTime(&collecttime);

	CTime ctimeCur(collecttime);
	CTimeSpan tspan(1,0,0,0);
	

	if ( (collecttime.wHour == 8 && collecttime.wMinute == 0 && collecttime.wSecond >= 1) ||
		(collecttime.wHour == 8 && collecttime.wMinute != 0) ||
		(collecttime.wHour >8 && collecttime.wHour <= 23))
	{
		strBegin.Format("%04d-%02d-%02d 08:00:01.000",collecttime.wYear,collecttime.wMonth,collecttime.wDay);
		CTime t = ctimeCur + tspan;
		strEnd.Format("%04d-%02d-%02d 08:00:01.000",t.GetYear(),t.GetMonth(),t.GetDay());
	}
	else if ((collecttime.wHour == 8 && collecttime.wMinute == 0 && collecttime.wSecond == 0) ||
		(collecttime.wHour >= 0 && collecttime.wHour < 8 ))
	{
		CTime t = ctimeCur - tspan;
		strBegin.Format("%04d-%02d-%02d 08:00:01.000",t.GetYear(),t.GetMonth(),t.GetDay());
		strEnd.Format("%04d-%02d-%02d 08:00:01.000",collecttime.wYear,collecttime.wMonth,collecttime.wDay);

	}

	SYSTEMTIME begintime,endtime;
	CSystemTimeOp::ConvertStringToSysemTime(strBegin,&begintime);
	CSystemTimeOp::ConvertStringToSysemTime(strEnd,&endtime);

	ret.stBegin = begintime;
	ret.stEnd = endtime;
	ret.blSucc = true;

#ifdef DEBUG
	SYSTEMTIME debugst;
	GetLocalTime(&debugst);
	TRACE("%s 1Day Area,point(%s),begin(%s),end(%s)\r\n",
		CSystemTimeOp::ConvertSystemTimeToString(&debugst,true),
		CSystemTimeOp::ConvertSystemTimeToString(&collecttime,true),
		CSystemTimeOp::ConvertSystemTimeToString(&(ret.stBegin),true),
		CSystemTimeOp::ConvertSystemTimeToString(&(ret.stEnd),true));
#endif

	return ret;
}

void CDataTranSQLServer_SC::CurStatisticsState( int nCount,SYSTEMTIME& collTime,SYSTEMTIME& recvTime )
{
	m_nWaittingBufferCount = nCount;
	curDealCollectTime = collTime;
	curDealReciveTime = recvTime;
}

void CDataTranSQLServer_SC::StatisticsWaterLevel_Hour_AppendReport( CAddrMap_Item* pAddrItem, SYSTEMTIME begintime, SYSTEMTIME endtime )
{
	
}

//雨量统计优化1
void CDataTranSQLServer_SC::StatisticsRain_Optimize1_RealTime( CAddrMap_Item* pAddrItem,CRainData_Item* pRecvData )
{

	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_RAIN && pRecvData != NULL)
	{

#ifdef DEBUG
		SYSTEMTIME debugst;
		CString ID = CUdfGlobalFUNC::newGUID();
		GetLocalTime(&debugst);
		TRACE("%s Rain:%s begin\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true),ID);
#endif

		//步骤1 获取整个统计的时间范围
		STRUCT_BEGINEND_AREA totalTimeArea = StatisticsRain_Optimize1_GetTotalTimeArea(pRecvData);

		//步骤2 读取整个统计时间范围的雨量
		std::multimap<CString,CRainData_Item*> RainDataMap = StatisticsRain_Optimize1_GetTotalRainData(pAddrItem,totalTimeArea);

		//步骤3 统计5分钟内降雨量
		StatisticsRain_Optimize1_5Minutes(pAddrItem,pRecvData,RainDataMap);

		//步骤4 统计15分钟内的降雨量,判断是否加报(暴雨加报)
		StatisticsRain_Optimize1_15Minutes(pAddrItem,pRecvData,RainDataMap);

		//步骤5 统计1个小时内的降雨量
		StatisticsRain_Optimize1_1Hour(pAddrItem,pRecvData,RainDataMap);

		//步骤6 统计1day内的降雨量
		StatisticsRain_Optimize1_1Day(pAddrItem,pRecvData,RainDataMap);

		//步骤6 释放multimap的雨量数据
		StatisticsRain_Optimize1_End(RainDataMap);

#ifdef DEBUG
		GetLocalTime(&debugst);
		TRACE("%s Rain:%s end\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true),ID);
#endif
	}
}

//获取雨量统计整个的时间范围
STRUCT_BEGINEND_AREA CDataTranSQLServer_SC::StatisticsRain_Optimize1_GetTotalTimeArea( CRainData_Item* pRecvData )
{
	STRUCT_BEGINEND_AREA TotalArea; 

#ifdef DEBUG
	SYSTEMTIME debugst;
	CString ID = CUdfGlobalFUNC::newGUID();
	GetLocalTime(&debugst);
	TRACE("%s StatisticsRain_Optimize1_GetTotalTimeArea(%s) begin\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true),ID);
#endif

	if (pRecvData != NULL)
	{
		CString strBegin="",strEnd="";
		CTimeSpan tspan(1,0,0,0);
		SYSTEMTIME collecttime = pRecvData->m_tCollectTime;
		CTime ctimeCur(collecttime /*pRecvData->m_tCollectTime*/);

		if ( (collecttime.wHour == 8 && collecttime.wMinute == 0 && collecttime.wSecond >= 1) ||
			(collecttime.wHour == 8 && collecttime.wMinute != 0) ||
			(collecttime.wHour >8 && collecttime.wHour <= 23))
		{
			//strBegin.Format("%04d-%02d-%02d 08:00:01.000",pRecvData->m_tCollectTime.wYear,pRecvData->m_tCollectTime.wMonth,pRecvData->m_tCollectTime.wDay);
			strBegin.Format("%04d-%02d-%02d 08:00:01.000",collecttime.wYear,collecttime.wMonth,collecttime.wDay);
			CTime t = ctimeCur + tspan;
			strEnd.Format("%04d-%02d-%02d 08:00:01.000",t.GetYear(),t.GetMonth(),t.GetDay());
		}
		else if ((collecttime.wHour == 8 && collecttime.wMinute == 0 && collecttime.wSecond == 0) ||
			(collecttime.wHour >= 0 && collecttime.wHour < 8 ))
		{
			CTime t = ctimeCur - tspan;
			strBegin.Format("%04d-%02d-%02d 08:00:01.000",t.GetYear(),t.GetMonth(),t.GetDay());
			//strEnd.Format("%04d-%02d-%02d 08:00:01.000",pRecvData->m_tCollectTime.wYear,pRecvData->m_tCollectTime.wMonth,pRecvData->m_tCollectTime.wDay);
			strEnd.Format("%04d-%02d-%02d 08:00:01.000",collecttime.wYear,collecttime.wMonth,collecttime.wDay);
		}

		SYSTEMTIME begintime,endtime;
		CSystemTimeOp::ConvertStringToSysemTime(strBegin,&begintime);
		CSystemTimeOp::ConvertStringToSysemTime(strEnd,&endtime);

		TotalArea.blSucc = true;
		TotalArea.stBegin = begintime;

		CTime basic_ctime(begintime);
		UINT  m_check_time = 0;

		while (m_check_time <= m_nChcek1Day_MNT + tspan.GetTotalMinutes())
		{
			strBegin = CSystemTimeOp::ConvertSystemTimeToString(&begintime,true);
			strEnd   = CSystemTimeOp::ConvertSystemTimeToString(&endtime,true);

			begintime = endtime;
			CTime temp_ctime(begintime);
			CTime tempend_ctime = temp_ctime + tspan;
			CSystemTimeOp::ConvertCTimeToSystemtime(&tempend_ctime,&endtime);
			CTimeSpan mus_tspan = temp_ctime - basic_ctime;
			m_check_time = mus_tspan.GetTotalMinutes();
		}
		
		CSystemTimeOp::ConvertStringToSysemTime(strEnd,&endtime);
		TotalArea.stEnd = endtime;
	}

//#ifdef DEBUG
//	SYSTEMTIME debugst;
//	GetLocalTime(&debugst);
//	TRACE("%s %s\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true),TotalArea.ToString());
//#endif

#ifdef DEBUG
	GetLocalTime(&debugst);
	TRACE("%s StatisticsRain_Optimize1_GetTotalTimeArea(%s) end(%s) \r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true),ID,TotalArea.ToString());
#endif

	return TotalArea;
}

//获取整个雨量统计所需的雨量原始数据
std::multimap<CString,CRainData_Item*> CDataTranSQLServer_SC::StatisticsRain_Optimize1_GetTotalRainData( CAddrMap_Item* pAddrItem,STRUCT_BEGINEND_AREA TimeArea )
{
#ifdef DEBUG
	SYSTEMTIME _debug_systemtime;
#endif

	std::multimap<CString,CRainData_Item*> resultRainMap;
	if (pAddrItem != NULL)
	{
		SYSTEMTIME beginTime = TimeArea.stBegin;
		SYSTEMTIME endtime = TimeArea.stEnd;

		CTime bgCTime(beginTime);
		CTimeSpan tspan(1,15,0,0);
		CTime beginCTime = bgCTime - tspan;
		CSystemTimeOp::ConvertCTimeToSystemtime(&beginCTime,&beginTime);

		CString strBegin="",strEnd ="";
		strBegin = CSystemTimeOp::ConvertSystemTimeToString(&beginTime,true);
		strEnd = CSystemTimeOp::ConvertSystemTimeToString(&endtime,true);

		CString sSQL = "";
		sSQL.Format("SELECT DISTINCT DATA_TYPE,CURRENT_VAL,HALFH_VAL,TOTAL_VAL,COLLECT_TIME FROM RAIN_DATA WHERE RTU='%s' AND SENSORS_NO='00' AND COLLECT_TIME >= '%s' AND COLLECT_TIME < '%s' ORDER BY COLLECT_TIME",
			pAddrItem->Get_RTU_ID(),strBegin,strEnd);

		CSqlStatement* sqlite_rs = ExcuteSQLiteQuery(sSQL);
#ifdef DEBUG
		GetLocalTime(&_debug_systemtime);
		TRACE("%s Read Begin(StatisticsRain_Optimize1_GetTotalRainData)\r\n", CSystemTimeOp::ConvertSystemTimeToString(&_debug_systemtime,true));
#endif
		if (sqlite_rs != NULL)
		{
			CRainData_Item* pBefore = NULL;
			bool blFinded = false;
			while(sqlite_rs->NextRow())
			{
				CString sTemp = "";
				BYTE DataType =0;
				double CurrentValue =0.0,HalfHourValue = 0.0,TotalValue =0.0;
				CString CollTimeString = "";

				sTemp = sqlite_rs->ValueString(0);
				DataType = (BYTE)atoi(sTemp);

				sTemp = sqlite_rs->ValueString(1);
				CurrentValue = atof(sTemp);

				sTemp = sqlite_rs->ValueString(2);
				HalfHourValue = atof(sTemp);

				sTemp = sqlite_rs->ValueString(3);
				TotalValue =atof(sTemp);

				sTemp = sqlite_rs->ValueString(4);
				CollTimeString = sTemp;

				CRainData_Item* pRain =new CRainData_Item(DataType,CurrentValue,HalfHourValue,TotalValue,CollTimeString);

				//new 目的:取最接近开始时间之前的数据，不用读取过多之前的数据，避免后续统计遍历次数
				if (blFinded == false && CSystemTimeOp::CompareSystemTime(&(pRain->m_tCollectTime), &(TimeArea.stBegin)) >= 0 )
				{
					blFinded = true;
					if (pBefore != NULL)
						resultRainMap.insert(std::pair<CString,CRainData_Item*>(pBefore->CollectTimeString(),pBefore));

					resultRainMap.insert(std::pair<CString,CRainData_Item*>(pRain->CollectTimeString(),pRain));
				}
				else if (blFinded == false && CSystemTimeOp::CompareSystemTime(&(pRain->m_tCollectTime), &(TimeArea.stBegin)) <0 )
				{
					_DELETE(pBefore);
					pBefore = pRain;
				}
				else if (blFinded == true)
				{
					resultRainMap.insert(std::pair<CString,CRainData_Item*>(pRain->CollectTimeString(),pRain));
				}

				//OLD
				//resultRainMap.insert(std::pair<CString,CRainData_Item*>(pRain->CollectTimeString(),pRain));
			}
		}
		_DELETE(sqlite_rs);

#ifdef DEBUG
		GetLocalTime(&_debug_systemtime);
		TRACE("%s end Begin(StatisticsRain_Optimize1_GetTotalRainData)\r\n", CSystemTimeOp::ConvertSystemTimeToString(&_debug_systemtime,true));
#endif
	}
	return resultRainMap;
}

void CDataTranSQLServer_SC::StatisticsRain_Optimize1_5Minutes( CAddrMap_Item* pAddrItem, CRainData_Item* pRecvData, std::multimap<CString,CRainData_Item*>& RainDataMap )
{
#ifdef DEBUG
	SYSTEMTIME debugst;
	CString ID = CUdfGlobalFUNC::newGUID();
	GetLocalTime(&debugst);
	TRACE("%s StatisticsRain_Optimize1_5Minutes(%s) begin\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true),ID);
#endif

	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_RAIN && pRecvData != NULL && RainDataMap.size() > 0)
	{
		CString strBegin = "",strEnd = "";
		CString strPoint = "";      //接口数据库TM 字段的值

		SYSTEMTIME colltime= pRecvData->m_tCollectTime;
		SYSTEMTIME _tempsystemtime = colltime;
		_tempsystemtime.wMinute = 0;
		_tempsystemtime.wSecond = 1;
		_tempsystemtime.wMilliseconds = 0;

		WORD mut = colltime.wMinute / 5;
		WORD mod = colltime.wMinute % 5;

		CTime _beginHour_ctime(_tempsystemtime);
		CTimeSpan _t_add_span(0,0,5 * mut,0);
		CTimeSpan _t_add5_span(0,0,5,0);

		SYSTEMTIME begintime,endtime;
		if (mod == 0 && colltime.wSecond == 0)
		{
			CTime _tempctime_end = _beginHour_ctime + _t_add_span;
			CTime _tempctime_begin  = _tempctime_end - _t_add5_span;
			CSystemTimeOp::ConvertCTimeToSystemtime(&_tempctime_end,&endtime);
			CSystemTimeOp::ConvertCTimeToSystemtime(&_tempctime_begin,&begintime);
		}
		else
		{
			CTime _tempctime_begin = _beginHour_ctime + _t_add_span;
			CTime _tempctime_end   = _tempctime_begin + _t_add5_span;
			CSystemTimeOp::ConvertCTimeToSystemtime(&_tempctime_begin,&begintime);
			CSystemTimeOp::ConvertCTimeToSystemtime(&_tempctime_end,&endtime);
		}

		strBegin = CSystemTimeOp::ConvertSystemTimeToString(&begintime,true);
		strEnd   = CSystemTimeOp::ConvertSystemTimeToString(&endtime,true);
		strPoint.Format("%04d-%02d-%02d %02d:%02d:00.005",
			endtime.wYear,endtime.wMonth,endtime.wDay,endtime.wHour,endtime.wMinute);

//#ifdef DEBUG
//		SYSTEMTIME _dbtime;
//		GetLocalTime(&_dbtime);
//		TRACE("%s 5 minutes,rain data[%s]\r\n",CSystemTimeOp::ConvertSystemTimeToString(&_dbtime,true),pRecvData->ToString());
//		TRACE("%s 5 minutes,begin:%s,end:%s\r\n",CSystemTimeOp::ConvertSystemTimeToString(&_dbtime,true),strBegin,strEnd);
//		TRACE("%s 5 minutes,point:%s\r\n",CSystemTimeOp::ConvertSystemTimeToString(&_dbtime,true),strPoint);
//#endif

		double Minus5Minutes = 0.0;  //5分钟降雨量
		bool   isNull = false;
		Minus5Minutes = CalRainData_Optimize1_OnTimeRange(pAddrItem,strBegin,strEnd,Rain_statistics_5minutes,isNull,RainDataMap);

		if (Minus5Minutes > m_dCHECK_RAIN_PER ||
			Minus5Minutes <= 0.0)
			Minus5Minutes = 0.0;

		if (false == isNull  && Minus5Minutes >= (double)0.1)
		{
			Table_ST_PPTN_R_Record data_record;
			data_record.STCD = pAddrItem->Get_STCD();
			data_record.TM   = strPoint;
			data_record.DRP  = Minus5Minutes;

			//其他字段的信息不需要
			data_record.INTV = 0.05;
			data_record.PRD  = 0.05;
			data_record.DYP  = 0.00;
			data_record.WTH  = "7";

			//new 20140816
			data_record.TYPE = Rain_statistics_5minutes;
			CSystemTimeOp::ConvertStringToSysemTime(strPoint,&(data_record.FTM));
			int comp = CSystemTimeOp::CompareSystemTime(&(data_record.FTM));
			if (comp <= 0)
				UpdateRainfallTableRecord(data_record,Rain_statistics_5minutes);
			else
				pAddrItem->UpdateST_PPTN_R_Record(&data_record);
		}
		else 
		{
			CString strSQL = "";
			if (SQLServer_Database == m_nDatabaseType)
			{
				strSQL.Format("DELETE FROM ST_PPTN_R WHERE STCD='%s' AND TM='%s' AND INTV=0.05",pAddrItem->Get_STCD(),strPoint);
			}
			else if (Oracle_Database == m_nDatabaseType)
			{
				strSQL.Format("DELETE FROM ST_PPTN_R WHERE STCD='%s' AND TM=to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3') AND INTV=0.05",pAddrItem->Get_STCD(),strPoint);
			}
			ExcuteNoQuery(strSQL);

			//new 20140816
			pAddrItem->RemoveST_PPTN_R_Record(strPoint);
		}

		//update by repair data
		SYSTEMTIME repair_begintime, repair_endtime;
		CTime basic_ctime(endtime);

		repair_begintime = endtime;
		CTime _temp_ctime = basic_ctime + _t_add5_span;
		CSystemTimeOp::ConvertCTimeToSystemtime(&_temp_ctime,&repair_endtime);

		UINT _t_repair_check = 0;

		//修改修复数据算法 2013-02-05
		//while (_t_repair_check <= m_nCheck5Minutes_MNT)
		while(true)
		{
			CString str_repair_point= "";
			CString str_repair_begin = "", str_repair_end = "";
			double  repair_5minute_rain = 0.0;
			str_repair_begin = CSystemTimeOp::ConvertSystemTimeToString(&repair_begintime,true);
			str_repair_end = CSystemTimeOp::ConvertSystemTimeToString(&repair_endtime,true);
			str_repair_point.Format("%04d-%02d-%02d %02d:%02d:00.005",
				repair_endtime.wYear,
				repair_endtime.wMonth,
				repair_endtime.wDay,
				repair_endtime.wHour,
				repair_endtime.wMinute);

			//new
			if (StatisticsRain_Optimize1_CompareEndTime(repair_begintime,RainDataMap) > 0)
				break;

			bool repair_isNull = false;
			repair_5minute_rain = CalRainData_Optimize1_OnTimeRange(pAddrItem,str_repair_begin,str_repair_end,Rain_statistics_5minutes,repair_isNull,RainDataMap);

			if (repair_5minute_rain > m_dCHECK_RAIN_PER ||
				repair_5minute_rain < 0.0)
				repair_5minute_rain = 0.0;

			if (false == repair_isNull && repair_5minute_rain >= (double)0.1 )
			{
				Table_ST_PPTN_R_Record data_record;
				data_record.STCD = pAddrItem->Get_STCD();
				data_record.TM   = str_repair_point;
				data_record.DRP  = repair_5minute_rain;

				//其他字段的信息不需要
				data_record.INTV = 0.05;
				data_record.PRD  = 0.05;
				data_record.DYP  = 0.00;
				data_record.WTH  = "7";

				//new 20140816
				data_record.TYPE = Rain_statistics_5minutes;
				CSystemTimeOp::ConvertStringToSysemTime(str_repair_point,&data_record.FTM);
				int comp = CSystemTimeOp::CompareSystemTime(&(data_record.FTM));
				if (comp <= 0)
					UpdateRainfallTableRecord(data_record,Rain_statistics_5minutes);
				else
					pAddrItem->UpdateST_PPTN_R_Record(&data_record);
			}
			else  //delete record
			{
				CString strSQL = "";
				if (SQLServer_Database == m_nDatabaseType)
				{
					strSQL.Format("DELETE FROM ST_PPTN_R WHERE STCD='%s' AND TM='%s' AND INTV=0.05",pAddrItem->Get_STCD(),str_repair_point);
				}
				else if (Oracle_Database == m_nDatabaseType)
				{
					strSQL.Format("DELETE FROM ST_PPTN_R WHERE STCD='%s' AND TM=to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3') AND INTV=0.05",pAddrItem->Get_STCD(),str_repair_point);
				}
				ExcuteNoQuery(strSQL);

				//new 20140816
				pAddrItem->RemoveST_PPTN_R_Record(str_repair_point);
			}

			//Next calc
			repair_begintime = repair_endtime;
			CTime repair_begin_ctime(repair_begintime);
			CTime repair_end_ctime = repair_begin_ctime + _t_add5_span;
			CTimeSpan _repair_tspan = repair_begin_ctime - basic_ctime;
			_t_repair_check = _repair_tspan.GetTotalMinutes();
			CSystemTimeOp::ConvertCTimeToSystemtime(&repair_end_ctime,&repair_endtime);

			//2013-02-05
			if (_t_repair_check > m_nCheck5Minutes_MNT && 
				(repair_begin_ctime.GetYear() != basic_ctime.GetYear() || 
				repair_begin_ctime.GetMonth() != basic_ctime.GetMonth() ||
				repair_begin_ctime.GetDay() != basic_ctime.GetDay() ||
				repair_begin_ctime.GetHour() !=  basic_ctime.GetHour()))
				break;
		}
	}

#ifdef DEBUG
	GetLocalTime(&debugst);
	TRACE("%s StatisticsRain_Optimize1_5Minutes(%s) end\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true),ID);
#endif
}

void CDataTranSQLServer_SC::StatisticsRain_Optimize1_15Minutes( CAddrMap_Item* pAddrItem, CRainData_Item* pRecvData, std::multimap<CString,CRainData_Item*>& RainDataMap )
{
#ifdef DEBUG
	SYSTEMTIME debugst;
	CString ID = CUdfGlobalFUNC::newGUID();
	GetLocalTime(&debugst);
	TRACE("%s StatisticsRain_Optimize1_15Minutes(%s) begin\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true),ID);
#endif

	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_RAIN && pRecvData != NULL)
	{
		CString strBegin ="",strEnd = "";
		CString strPoint = "";      //接口数据库TM 字段的值

		SYSTEMTIME collectTime = pRecvData->m_tCollectTime;
		SYSTEMTIME _tempsystemtime = collectTime;
		_tempsystemtime.wMinute = 0;
		_tempsystemtime.wSecond = 1;
		_tempsystemtime.wMilliseconds = 0;

		WORD mut = collectTime.wMinute / 15;
		WORD mod = collectTime.wMinute % 15;

		CTime _beginhour(_tempsystemtime);
		CTimeSpan _tspan_add(0,0,15 * mut,0);
		CTimeSpan _tspan_15(0,0,15,0);

		SYSTEMTIME begintime,endtime;
		if (mod == 0 && collectTime.wSecond == 0)
		{
			CTime _end_ctime = _beginhour + _tspan_add;
			CTime _begin_ctime = _end_ctime - _tspan_15;
			CSystemTimeOp::ConvertCTimeToSystemtime(&_begin_ctime,&begintime);
			CSystemTimeOp::ConvertCTimeToSystemtime(&_end_ctime,&endtime);
		}
		else
		{
			CTime _begin_ctime = _beginhour + _tspan_add;
			CTime _end_ctime = _begin_ctime + _tspan_15;
			CSystemTimeOp::ConvertCTimeToSystemtime(&_begin_ctime,&begintime);
			CSystemTimeOp::ConvertCTimeToSystemtime(&_end_ctime,&endtime);
		}

		strBegin = CSystemTimeOp::ConvertSystemTimeToString(&begintime,true);
		strEnd = CSystemTimeOp::ConvertSystemTimeToString(&endtime,true);
		strPoint.Format("%04d-%02d-%02d %02d:%02d",
			endtime.wYear,endtime.wMonth,endtime.wDay,endtime.wHour,endtime.wMinute);

//#ifdef DEBUG
//		SYSTEMTIME _dbtime;
//		GetLocalTime(&_dbtime);
//		TRACE("%s 15 minutes,rain data[%s]\r\n",CSystemTimeOp::ConvertSystemTimeToString(&_dbtime,true),pRecvData->ToString());
//		TRACE("%s 15 minutes,begin:%s,end:%s\r\n",CSystemTimeOp::ConvertSystemTimeToString(&_dbtime,true),strBegin,strEnd);
//		TRACE("%s 15 minutes,Point:%s\r\n",CSystemTimeOp::ConvertSystemTimeToString(&_dbtime,true),strPoint);
//#endif

		double Minus15Minutes = 0.0;  //15分钟降雨量
		bool   isNull = false;

		//Minus15Minutes = CalRainData_OnTimeRange(pAddrItem,strBegin,strEnd,Rain_statistics_15minutes,isNull);
		Minus15Minutes = CalRainData_Optimize1_OnTimeRange(pAddrItem,strBegin,strEnd,Rain_statistics_15minutes,isNull,RainDataMap);


		if (Minus15Minutes > m_dCHECK_RAIN_PER ||
			Minus15Minutes < 5.0)
			Minus15Minutes = 0.0;

		if (false == isNull && Minus15Minutes >= (double)5.0)
		{
			Table_ST_STORM_R_Record sRecord;

			sRecord.STCD	= pAddrItem->Get_STCD();
			sRecord.TM		= strPoint;
			sRecord.STRMP	= Minus15Minutes;

			sRecord.STRMDR	= 0.15;
			sRecord.WTH		= "7";

			//new 20140816
			CString strFTM = strPoint + ":00.000";
			CSystemTimeOp::ConvertStringToSysemTime(strFTM,&(sRecord.FTM));
			int comp = CSystemTimeOp::CompareSystemTime(&(sRecord.FTM));
			if (comp <= 0)
				UpdateRainstormTableRecord(sRecord);
			else
				pAddrItem->UpdateST_STROM_R_Record(&sRecord);
		}
		else // delete
		{
			CString strSQL ="";
			if (SQLServer_Database == m_nDatabaseType)
			{
				strSQL.Format("DELETE FROM ST_STORM_R WHERE STCD='%s' AND TM='%s' AND STRMDR = 0.15",
					pAddrItem->Get_STCD(),strPoint);
			}
			else if (Oracle_Database == m_nDatabaseType)
			{
				strSQL.Format("DELETE FROM ST_STORM_R WHERE STCD='%s' AND TM=to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3') AND STRMDR = 0.15",
					pAddrItem->Get_STCD(),strPoint);
			}
			ExcuteNoQuery(strSQL);

			//new 20140816
			pAddrItem->RemoveST_STROM_R_Record(strPoint);
		}


		//update by repair data
		SYSTEMTIME repair_begintime, repair_endtime;
		CTime basic_ctime(endtime);

		repair_begintime = endtime;
		CTime _temp_ctime = basic_ctime + _tspan_15;
		CSystemTimeOp::ConvertCTimeToSystemtime(&_temp_ctime,&repair_endtime);

		UINT _t_repair_check = 0;

		//Modified 2013-02-05
		//while (_t_repair_check <= m_nCheck15Minutes_MNT)
		while(true)
		{
			CString str_repair_point= "";
			CString str_repair_begin = "", str_repair_end = "";
			double  repair_15minute_rain = 0.0;
			str_repair_begin = CSystemTimeOp::ConvertSystemTimeToString(&repair_begintime,true);
			str_repair_end = CSystemTimeOp::ConvertSystemTimeToString(&repair_endtime,true);
			str_repair_point.Format("%04d-%02d-%02d %02d:%02d",
				repair_endtime.wYear,
				repair_endtime.wMonth,
				repair_endtime.wDay,
				repair_endtime.wHour,
				repair_endtime.wMinute);

			//new
			if (StatisticsRain_Optimize1_CompareEndTime(repair_begintime,RainDataMap) > 0)
				break;

			bool repair_isNull = false;
			//repair_15minute_rain = CalRainData_OnTimeRange(pAddrItem,str_repair_begin,str_repair_end,Rain_statistics_15minutes,repair_isNull);
			repair_15minute_rain = CalRainData_Optimize1_OnTimeRange(pAddrItem,str_repair_begin,str_repair_end,Rain_statistics_15minutes,repair_isNull,RainDataMap);


			if (repair_15minute_rain > m_dCHECK_RAIN_PER ||
				repair_15minute_rain < 5.0)
				repair_15minute_rain = 0.0;

			if (false == repair_isNull && repair_15minute_rain >= (double)5.0)
			{
				Table_ST_STORM_R_Record sRecord;

				sRecord.STCD	= pAddrItem->Get_STCD();
				sRecord.TM		= str_repair_point;
				sRecord.STRMP	= repair_15minute_rain;

				sRecord.STRMDR	= 0.15;
				sRecord.WTH		= "7";

				//new 20140816
				CString strFTM = str_repair_point + ":00.000";
				CSystemTimeOp::ConvertStringToSysemTime(strFTM,&(sRecord.FTM));
				int comp = CSystemTimeOp::CompareSystemTime(&(sRecord.FTM));
				if (comp <= 0)
					UpdateRainstormTableRecord(sRecord);
				else
					pAddrItem->UpdateST_STROM_R_Record(&sRecord);
			}
			else // delete record
			{
				CString strSQL ="";
				if (SQLServer_Database == m_nDatabaseType)
				{
					strSQL.Format("DELETE FROM ST_STORM_R WHERE STCD='%s' AND TM='%s' AND STRMDR = 0.15",
						pAddrItem->Get_STCD(),str_repair_point);
				}
				else if (Oracle_Database == m_nDatabaseType)
				{
					strSQL.Format("DELETE FROM ST_STORM_R WHERE STCD='%s' AND TM=to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3') AND STRMDR = 0.15",
						pAddrItem->Get_STCD(),str_repair_point);
				}


				ExcuteNoQuery(strSQL);

				//new 20140816
				pAddrItem->RemoveST_STROM_R_Record(str_repair_point);
			}

			//Next Calc
			repair_begintime		= repair_endtime;
			CTime repair_begin_ctime(repair_begintime);
			CTime repair_end_ctime	= repair_begin_ctime + _tspan_15;
			CTimeSpan _repair_tspan = repair_begin_ctime - basic_ctime;
			_t_repair_check			= _repair_tspan.GetTotalMinutes();
			CSystemTimeOp::ConvertCTimeToSystemtime(&repair_end_ctime,&repair_endtime);

			if (_t_repair_check > m_nCheck15Minutes_MNT &&
				(basic_ctime.GetYear() != repair_begin_ctime.GetYear() ||
				basic_ctime.GetMonth() != repair_begin_ctime.GetMonth() || 
				basic_ctime.GetDay() != repair_begin_ctime.GetDay() ||
				basic_ctime.GetHour() != repair_begin_ctime.GetHour()) )
				break;
		}
	}

#ifdef DEBUG
	GetLocalTime(&debugst);
	TRACE("%s StatisticsRain_Optimize1_15Minutes(%s) end\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true),ID);
#endif
}

void CDataTranSQLServer_SC::StatisticsRain_Optimize1_1Hour( CAddrMap_Item* pAddrItem, CRainData_Item* pRecvData, std::multimap<CString,CRainData_Item*>& RainDataMap )
{

#ifdef DEBUG
	SYSTEMTIME debugst;
	CString ID = CUdfGlobalFUNC::newGUID();
	GetLocalTime(&debugst);
	TRACE("%s StatisticsRain_Optimize1_1Hour(%s) begin\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true),ID);
#endif

	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_RAIN && pRecvData != NULL)
	{
		//new 20140816
		STRUCT_BEGINEND_AREA now1HArea = Get1HourArea(); 

		CString strBegin ="",strEnd = "";
		CString strPoint = "";      //接口数据库TM 字段的值

		SYSTEMTIME collectTime = pRecvData->m_tCollectTime;

		SYSTEMTIME _tempsystemtime = collectTime;
		_tempsystemtime.wMinute = 0;
		_tempsystemtime.wSecond = 1;
		_tempsystemtime.wMilliseconds = 0;

		CTime _beginhour(_tempsystemtime);
		CTimeSpan _tspan_1hour(0,1,0,0);

		SYSTEMTIME begintime,endtime;

		if (collectTime.wMinute == 0 &&
			collectTime.wSecond == 0)      //整点数据
		{
			endtime = _tempsystemtime;
			CTime _temp_begin_ctime = _beginhour - _tspan_1hour;
			CSystemTimeOp::ConvertCTimeToSystemtime(&_temp_begin_ctime,&begintime);
		}
		else									
		{
			begintime = _tempsystemtime;
			CTime _temp_end_ctime = _beginhour + _tspan_1hour;
			CSystemTimeOp::ConvertCTimeToSystemtime(&_temp_end_ctime,&endtime);
		}

		strBegin = CSystemTimeOp::ConvertSystemTimeToString(&begintime,true);
		strEnd = CSystemTimeOp::ConvertSystemTimeToString(&endtime,true);
		strPoint.Format("%04d-%02d-%02d %02d:00",
			endtime.wYear,endtime.wMonth,endtime.wDay,endtime.wHour);

//#ifdef DEBUG
//		SYSTEMTIME _dbtime;
//		GetLocalTime(&_dbtime);
//		TRACE("%s 1 hour,rain data[%s]\r\n",CSystemTimeOp::ConvertSystemTimeToString(&_dbtime,true),pRecvData->ToString());
//		TRACE("%s 1 hour,begin:%s,end:%s\r\n",CSystemTimeOp::ConvertSystemTimeToString(&_dbtime,true),strBegin,strEnd);
//		TRACE("%s 1 hour,Point:%s\r\n",CSystemTimeOp::ConvertSystemTimeToString(&_dbtime,true),strPoint);
//#endif

		double Minus1Hour = 0.0; //小时降雨量
		bool   isHourNull = false;
		//Minus1Hour = CalRainData_OnTimeRange(pAddrItem,strBegin,strEnd,Rain_statistics_1hour,isHourNull);
		Minus1Hour = CalRainData_Optimize1_OnTimeRange(pAddrItem,strBegin,strEnd,Rain_statistics_1hour,isHourNull,RainDataMap);

		//calc rain minutes 
		if (false == isHourNull)
		{
			if (Minus1Hour >  m_dCHECK_RAIN_PER ||
				Minus1Hour <= 0.0)
				Minus1Hour = 0.0f;

			int RainMinutes = 0;
			if (Minus1Hour >= (double)0.1)
			{
				if (m_RainPRDFlag != 0)
				{
					//RainMinutes = GetRainMinutes(pAddrItem,strBegin,strEnd);
					RainMinutes = GetRainMinutes_Optimize1(pAddrItem,strBegin,strEnd,RainDataMap);

					if (RainMinutes <= 0 || RainMinutes > 60)
						RainMinutes = 60;	
				}
			}

			//判断是否8点,如果是8点时,需要添加8点数据(日统计、平安报数据)
			if (endtime.wHour != 8)
			{
				if (Minus1Hour >= (double)0.1)
				{
					Table_ST_PPTN_R_Record sRecord;

					sRecord.STCD = pAddrItem->Get_STCD();
					sRecord.TM = strPoint;
					sRecord.DRP = Minus1Hour;
					sRecord.INTV = 1.00;
					sRecord.WTH = "7";

					if (m_RainPRDFlag != 0)
					{
						if (RainMinutes >= 60 || RainMinutes <= 0 )
							sRecord.PRD = 0.00;
						else
							sRecord.PRD = (double)RainMinutes /100;
					}
					else
					{
						sRecord.PRD = 0.00;
					}

					//new 20140816
					sRecord.TYPE = Rain_statistics_1hour;
					CString strFTM = strPoint + ":00.000";
					CSystemTimeOp::ConvertStringToSysemTime(strFTM,&(sRecord.FTM));
					int comp = CSystemTimeOp::CompareSystemTime(&(sRecord.FTM));
					if (comp <= 0)
						UpdateRainfallTableRecord(sRecord,Rain_statistics_1hour);
					else
						pAddrItem->UpdateST_PPTN_R_Record(&sRecord);
				}
				else
				{
					CString sSQL = "";
					if (SQLServer_Database == m_nDatabaseType)
					{
						sSQL.Format("DELETE FROM ST_PPTN_R WHERE STCD='%s' AND TM='%s' AND INTV=1.00",
							pAddrItem->Get_STCD(),
							strPoint);
					}
					else if (Oracle_Database == m_nDatabaseType)
					{
						sSQL.Format("DELETE FROM ST_PPTN_R WHERE STCD='%s' AND TM=to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3') AND INTV=1.00",
							pAddrItem->Get_STCD(),
							strPoint);
					}

					ExcuteNoQuery(sSQL);

					//new 20140816
					pAddrItem->RemoveST_PPTN_R_Record(strPoint);
				}
			}
			else if (endtime.wHour == 8)
			{
				//calc AM8点数据
				CTime ctimeEnd(endtime);
				CTimeSpan _tspan (1,0,0,0);
				CTime ctime1dayBegin= ctimeEnd - _tspan;

				CString str1DayBegin ="";
				str1DayBegin.Format("%04d-%02d-%02d 08:00:01.000",ctime1dayBegin.GetYear(),ctime1dayBegin.GetMonth(),ctime1dayBegin.GetDay());

				double Rain1Day = 0.0f;
				bool   isNullDay = false;
				//Rain1Day = CalRainData_OnTimeRange(pAddrItem,str1DayBegin,strEnd,Rain_statistics_AM8Point,isNullDay);
				Rain1Day = CalRainData_Optimize1_OnTimeRange(pAddrItem,str1DayBegin,strEnd,Rain_statistics_AM8Point,isNullDay,RainDataMap);


				//遇到大数据时
				if (Rain1Day > m_dCHECK_RAIN_DAY)
					Rain1Day =0.0f;

				Table_ST_PPTN_R_Record srecord;
				srecord.STCD = pAddrItem->Get_STCD();
				srecord.TM	 = strPoint;
				srecord.DYP  = Rain1Day;
				if (Minus1Hour >= 0.1)
				{
					srecord.DRP = Minus1Hour;
					srecord.INTV = 1.00;
					srecord.WTH = "7";
					if (m_RainPRDFlag != 0)
					{
						if (RainMinutes >= 60 || RainMinutes <= 0 )
							srecord.PRD = 0.00;
						else
							srecord.PRD = (double)RainMinutes /100;
					}
					else 
					{
						srecord.PRD = 0.00;
					}

				}
				else if (Minus1Hour < (double)0.1)
					srecord.WTH = "8";


				//new 20140816
				srecord.TYPE = Rain_statistics_AM8Point;
				CString strFTM = strPoint + ":00.000";
				CSystemTimeOp::ConvertStringToSysemTime(strFTM,&(srecord.FTM));
				int comp = CSystemTimeOp::CompareSystemTime(&(srecord.FTM));
				if (comp <= 0 )
					UpdateRainfallTableRecord(srecord,Rain_statistics_AM8Point);
				else
					pAddrItem->UpdateST_PPTN_R_Record(&srecord);
			}
		}

		//update by repair data(for 1 hour)
		SYSTEMTIME repair_begin_time,repair_end_time;
		CTime      basic_ctime(endtime);
		repair_begin_time = endtime;

		CTime      _temp_repair_end_ctime = basic_ctime + _tspan_1hour;
		CSystemTimeOp::ConvertCTimeToSystemtime(&_temp_repair_end_ctime,&repair_end_time);

		UINT repair_time_check = 0;
		while (repair_time_check <= m_nCheck1Hour_MNT)
		{
			CString str_repair_begin="",str_repair_end = "",str_repair_point = "";
			str_repair_begin = CSystemTimeOp::ConvertSystemTimeToString(&repair_begin_time,true);
			str_repair_end = CSystemTimeOp::ConvertSystemTimeToString(&repair_end_time,true);
			str_repair_point.Format("%04d-%02d-%02d %02d:00",
				repair_end_time.wYear,repair_end_time.wMonth,repair_end_time.wDay,repair_end_time.wHour);


			//new
			if (StatisticsRain_Optimize1_CompareEndTime(repair_begin_time,RainDataMap) > 0)
				break;


			double repair_rain_data =0.0;
			bool repair_isNull = false;

			//repair_rain_data = CalRainData_OnTimeRange(pAddrItem,str_repair_begin,str_repair_end,Rain_statistics_1hour,repair_isNull);
			repair_rain_data = CalRainData_Optimize1_OnTimeRange(pAddrItem,str_repair_begin,str_repair_end,Rain_statistics_1hour,repair_isNull,RainDataMap);

			if (false == repair_isNull)
			{
				if (repair_rain_data >  m_dCHECK_RAIN_PER ||
					repair_rain_data <= 0.0)
					repair_rain_data = 0.0f;

				int repair_RainMinutes = 0;
				if (repair_rain_data >= (double)0.1)
				{
					if (m_RainPRDFlag != 0)
					{
						repair_RainMinutes = GetRainMinutes_Optimize1(pAddrItem,str_repair_begin,str_repair_end,RainDataMap);
						if (repair_RainMinutes <= 0 || repair_RainMinutes > 60)
							repair_RainMinutes = 60;	
					}
				}

				//判断是否8点,如果是8点时,需要添加8点数据(日统计、平安报数据)
				if (repair_end_time.wHour != 8)
				{
					if (repair_rain_data >= (double)0.1)
					{
						Table_ST_PPTN_R_Record sRecord;

						sRecord.STCD = pAddrItem->Get_STCD();
						sRecord.TM  = str_repair_point;
						sRecord.DRP = repair_rain_data;
						sRecord.INTV = 1.00;
						sRecord.WTH = "7";

						if (m_RainPRDFlag != 0)
						{
							if (repair_RainMinutes >= 60 || repair_RainMinutes <= 0 )
								sRecord.PRD = 0.00;
							else
								sRecord.PRD = (double)repair_RainMinutes /100;
						}
						else
						{
							sRecord.PRD = 0.00;
						}


						//new 20140816
						sRecord.TYPE = Rain_statistics_1hour;
						CString strFTM = str_repair_point + ":00.000";
						CSystemTimeOp::ConvertStringToSysemTime(strFTM,&(sRecord.FTM));
						int comp = CSystemTimeOp::CompareSystemTime(&(sRecord.FTM));
						if (comp <= 0)
							UpdateRainfallTableRecord(sRecord,Rain_statistics_1hour);
						else
							pAddrItem->UpdateST_PPTN_R_Record(&sRecord);
					}
					else
					{
						CString sSQL = "";
						if (SQLServer_Database == m_nDatabaseType)
						{
							sSQL.Format("DELETE FROM ST_PPTN_R WHERE STCD='%s' AND TM='%s' AND INTV=1.00",pAddrItem->Get_STCD(),str_repair_point);
						}
						else if (Oracle_Database == m_nDatabaseType)
						{
							sSQL.Format("DELETE FROM ST_PPTN_R WHERE STCD='%s' AND TM=to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3') AND INTV=1.00",pAddrItem->Get_STCD(),str_repair_point);
						}

						ExcuteNoQuery(sSQL);

						//new 20140816
						pAddrItem->RemoveST_PPTN_R_Record(str_repair_point);
					}
				}
				else if (repair_end_time.wHour == 8)
				{
					//calc AM8点数据
					CTime ctimeEnd(repair_end_time);
					CTimeSpan _tspan (1,0,0,0);
					CTime ctime1dayBegin= ctimeEnd - _tspan;

					CString str1DayBegin ="";
					str1DayBegin.Format("%04d-%02d-%02d 08:00:01.000",ctime1dayBegin.GetYear(),ctime1dayBegin.GetMonth(),ctime1dayBegin.GetDay());

					double Rain1Day = 0.0f;
					bool   isNullDay = false;
					//Rain1Day = CalRainData_OnTimeRange(pAddrItem,str1DayBegin,str_repair_end,Rain_statistics_AM8Point,isNullDay);
					Rain1Day = CalRainData_Optimize1_OnTimeRange(pAddrItem,str1DayBegin,str_repair_end,Rain_statistics_AM8Point,isNullDay,RainDataMap);

					//遇到大数据时
					if (Rain1Day > m_dCHECK_RAIN_DAY ||
						Rain1Day <= 0.0)
						Rain1Day =0.0f;

					Table_ST_PPTN_R_Record srecord;
					srecord.STCD = pAddrItem->Get_STCD();
					srecord.TM	 = str_repair_point;
					srecord.DYP  = Rain1Day;
					if (repair_rain_data >= 0.1)
					{
						srecord.DRP = repair_rain_data;
						srecord.INTV = 1.00;
						srecord.WTH = "7";
						if (m_RainPRDFlag != 0)
						{
							if (repair_RainMinutes >= 60 || repair_RainMinutes <= 0 )
								srecord.PRD = 0.00;
							else
								srecord.PRD = (double)repair_RainMinutes /100;
						}
						else
						{
							srecord.PRD = 0.00;
						}
					}
					else if (repair_rain_data < (double)0.1)
						srecord.WTH = "8";

					//new 20140816
					srecord.TYPE = Rain_statistics_AM8Point;
					CString strFTM = str_repair_point + ":00.000";
					CSystemTimeOp::ConvertStringToSysemTime(strFTM,&(srecord.FTM));
					int comp = CSystemTimeOp::CompareSystemTime(&(srecord.FTM));
					if (comp <= 0)
						UpdateRainfallTableRecord(srecord,Rain_statistics_AM8Point);
					else
						pAddrItem->UpdateST_PPTN_R_Record(&srecord);
				}
			}

			//next
			repair_begin_time = repair_end_time;
			CTime repair_begin_ctime(repair_begin_time);
			CTime repare_end_ctime = repair_begin_ctime + _tspan_1hour;
			CSystemTimeOp::ConvertCTimeToSystemtime(&repare_end_ctime,&repair_end_time);
			CTimeSpan repair_tspan = repair_begin_ctime - basic_ctime;
			repair_time_check = repair_tspan.GetTotalMinutes();
		}
	}
#ifdef DEBUG
	GetLocalTime(&debugst);
	TRACE("%s StatisticsRain_Optimize1_1Hour(%s) end\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true),ID);
#endif

}

void CDataTranSQLServer_SC::StatisticsRain_Optimize1_1Day( CAddrMap_Item* pAddrItem, CRainData_Item* pRecvData, std::multimap<CString,CRainData_Item*>& RainDataMap )
{
#ifdef DEBUG
	SYSTEMTIME debugst;
	CString ID = CUdfGlobalFUNC::newGUID();
	GetLocalTime(&debugst);
	TRACE("%s StatisticsRain_Optimize1_1Day(%s) begin\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true),ID);
#endif
	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_RAIN && pRecvData != NULL)
	{
		CString strBegin="",strEnd="",strPoint = "";
		CTime ctimeCur(pRecvData->m_tCollectTime);
		CTimeSpan tspan(1,0,0,0);
		SYSTEMTIME collecttime = pRecvData->m_tCollectTime;

		if ( (collecttime.wHour == 8 && collecttime.wMinute == 0 && collecttime.wSecond >= 1) ||
			(collecttime.wHour == 8 && collecttime.wMinute != 0) ||
			(collecttime.wHour >8 && collecttime.wHour <= 23))
		{
			strBegin.Format("%04d-%02d-%02d 08:00:01.000",pRecvData->m_tCollectTime.wYear,pRecvData->m_tCollectTime.wMonth,pRecvData->m_tCollectTime.wDay);
			CTime t = ctimeCur + tspan;
			strEnd.Format("%04d-%02d-%02d 08:00:01.000",t.GetYear(),t.GetMonth(),t.GetDay());
			strPoint.Format("%04d-%02d-%02d 08:00",t.GetYear(),t.GetMonth(),t.GetDay());
		}
		else if ((collecttime.wHour == 8 && collecttime.wMinute == 0 && collecttime.wSecond == 0) ||
			(collecttime.wHour >= 0 && collecttime.wHour < 8 ))
		{
			CTime t = ctimeCur - tspan;
			strBegin.Format("%04d-%02d-%02d 08:00:01.000",t.GetYear(),t.GetMonth(),t.GetDay());
			strEnd.Format("%04d-%02d-%02d 08:00:01.000",pRecvData->m_tCollectTime.wYear,pRecvData->m_tCollectTime.wMonth,pRecvData->m_tCollectTime.wDay);
			strPoint.Format("%04d-%02d-%02d 08:00",pRecvData->m_tCollectTime.wYear,pRecvData->m_tCollectTime.wMonth,pRecvData->m_tCollectTime.wDay);
		}

		SYSTEMTIME begintime,endtime;
		CSystemTimeOp::ConvertStringToSysemTime(strBegin,&begintime);
		CSystemTimeOp::ConvertStringToSysemTime(strEnd,&endtime);

		CTime basic_ctime(begintime);
		UINT  m_check_time = 0;

		while (m_check_time <= m_nChcek1Day_MNT + tspan.GetTotalMinutes())
		{
			strBegin = CSystemTimeOp::ConvertSystemTimeToString(&begintime,true);
			strEnd   = CSystemTimeOp::ConvertSystemTimeToString(&endtime,true);
			strPoint.Format("%04d-%02d-%02d 08:00",endtime.wYear,endtime.wMonth,endtime.wDay);



			//new
			if (StatisticsRain_Optimize1_CompareEndTime(begintime,RainDataMap) > 0)
				break;


			double day_rain = 0;
			bool  isNullDay = false;
			//day_rain = CalRainData_OnTimeRange(pAddrItem,strBegin,strEnd,Rain_statistics_AM8Point,isNullDay);
			day_rain = CalRainData_Optimize1_OnTimeRange(pAddrItem,strBegin,strEnd,Rain_statistics_AM8Point,isNullDay,RainDataMap);


			if (false == isNullDay)
			{
				if (day_rain <=0.0 || day_rain > m_dCHECK_RAIN_DAY)
					day_rain = 0.0;

				//1 hour begin time and end time
				CString strHourBegin="",strHourEnd = "";
				strHourEnd = strEnd;
				strHourBegin.Format("%04d-%02d-%02d 07:00:01.000",endtime.wYear,endtime.wMonth,endtime.wDay);

				double hour_rain = 0.0;
				bool isNullHour = false;
				//hour_rain = CalRainData_OnTimeRange(pAddrItem,strHourBegin,strHourEnd,Rain_statistics_1hour,isNullHour);
				hour_rain = CalRainData_Optimize1_OnTimeRange(pAddrItem,strHourBegin,strHourEnd,Rain_statistics_1hour,isNullHour,RainDataMap);


				if (false == isNullHour)
				{
					if (hour_rain <= 0.0 || hour_rain > m_dCHECK_RAIN_PER)
						hour_rain = 0.0f;

					int HourRainMinutes = 0;
					if (hour_rain >= (double)0.1)
					{
						if (m_RainPRDFlag != 0)
						{
							HourRainMinutes = GetRainMinutes_Optimize1(pAddrItem,strHourBegin,strHourEnd,RainDataMap);
							if (HourRainMinutes <= 0 || HourRainMinutes > 60)
								HourRainMinutes = 60;	
						}

					}

					Table_ST_PPTN_R_Record srecord;
					srecord.STCD = pAddrItem->Get_STCD();
					srecord.TM	 = strPoint;
					srecord.DYP  = day_rain;
					if (hour_rain >= 0.1)
					{
						srecord.DRP = hour_rain;
						srecord.INTV = 1.00;
						srecord.WTH = "7";

						if (m_RainPRDFlag != 0)
						{
							if (HourRainMinutes >= 60 || HourRainMinutes <= 0 )
								srecord.PRD = 0.00;
							else
								srecord.PRD = (double)HourRainMinutes /100;
						}
						else
						{
							srecord.PRD = 0.00;
						}

					}
					else if (hour_rain < (double)0.1)
						srecord.WTH = "8";

					//new 20140816
					srecord.TYPE = Rain_statistics_AM8Point;
					CString strFTM = strPoint + ":00.000";
					CSystemTimeOp::ConvertStringToSysemTime(strFTM,&(srecord.FTM));
					int comp = CSystemTimeOp::CompareSystemTime(&(srecord.FTM));
					if (comp <= 0)
						UpdateRainfallTableRecord(srecord,Rain_statistics_AM8Point);
					else
						pAddrItem->UpdateST_PPTN_R_Record(&srecord);

				}
				else if (true == isNullHour)
				{
					Table_ST_PPTN_R_Record srecord;
					srecord.STCD = pAddrItem->Get_STCD();
					srecord.TM	 = strPoint;
					srecord.DYP  = day_rain;
					srecord.WTH = "8";

					//new 20140816
					srecord.TYPE = Rain_statistics_AM8Point;
					CString strFTM = strPoint + ":00.000";
					CSystemTimeOp::ConvertStringToSysemTime(strFTM,&(srecord.FTM));
					int comp = CSystemTimeOp::CompareSystemTime(&(srecord.FTM));
					if (comp <= 0)
						UpdateRainfallTableRecord(srecord,Rain_statistics_AM8Point);
					else
						pAddrItem->UpdateST_PPTN_R_Record(&srecord);

				}
			}

			//next row
			begintime = endtime;
			CTime temp_ctime(begintime);
			CTime tempend_ctime = temp_ctime + tspan;
			CSystemTimeOp::ConvertCTimeToSystemtime(&tempend_ctime,&endtime);
			CTimeSpan mus_tspan = temp_ctime - basic_ctime;
			m_check_time = mus_tspan.GetTotalMinutes();
		}
	}

#ifdef DEBUG
	GetLocalTime(&debugst);
	TRACE("%s StatisticsRain_Optimize1_1Day(%s) end\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true),ID);
#endif
}

double CDataTranSQLServer_SC::CalRainData_Optimize1_OnTimeRange( CAddrMap_Item* pAddrItem, CString strBegin, CString strEnd, 
																RIANSTATISTICS_TYPE_Enum xType, bool& isNull, 
																std::multimap<CString,CRainData_Item*>& RainDataMap )
{
#ifdef DEBUG
	SYSTEMTIME _debug_systemtime;
	CString ID = CUdfGlobalFUNC::newGUID();
	GetLocalTime(&_debug_systemtime);
	TRACE("%s Function:CalRainData_Optimize1_OnTimeRange(%s)(begin)\r\n", CSystemTimeOp::ConvertSystemTimeToString(&_debug_systemtime,true),ID);
#endif

	double _RainData = 0.0;
	if (pAddrItem && pAddrItem->GetType() == CZ_Type_RAIN && strBegin != "" && strEnd != " ")
	{

#ifdef DEBUG
		CString calcTypeName[] ={"5分钟统计",
			"15分钟统计",
			"小时统计",
			"日统计"};
		//TRACE("%s Function:CalRainData_Optimize1_OnTimeRange(begin)\r\n",CSystemTimeOp::ConvertSystemTimeToString(&_debug_systemtime,true));		
		TRACE("\t\tType :%d,Type's Name:%s\r\n",(int)xType,calcTypeName[int(xType)]);
		TRACE("\t\tBegin:%s\r\n",strBegin);
		TRACE("\t\tEnd  :%s\r\n",strEnd);
		TRACE("\t\tSTCD:%s,RTU:%s\r\n",pAddrItem->Get_STCD(),pAddrItem->Get_RTU_ID());
#endif

		SYSTEMTIME begintime,endtime;
		CSystemTimeOp::ConvertStringToSysemTime(strBegin,&begintime);
		CSystemTimeOp::ConvertStringToSysemTime(strEnd,&endtime);

		UINT check_time_value = 0;
		double check_data_value = 0.0;

		switch(xType)
		{
		case Rain_statistics_5minutes:
			check_time_value = m_nCheck5Minutes_MNT;
			check_data_value = m_dCHECK_RAIN_PER;
			break;
		case Rain_statistics_15minutes:
			check_time_value = m_nCheck15Minutes_MNT;
			check_data_value = m_dCHECK_RAIN_PER;
			break;
		case Rain_statistics_1hour:
			check_time_value = m_nCheck1Hour_MNT;
			check_data_value = m_dCHECK_RAIN_PER;
			break;
		case  Rain_statistics_AM8Point:
			check_time_value = m_nChcek1Day_MNT;
			check_data_value = m_dCHECK_RAIN_DAY;
			break;
		}

		//CString sSQL = "";
		//sSQL.Format("SELECT DISTINCT DATA_TYPE,CURRENT_VAL,HALFH_VAL,TOTAL_VAL,COLLECT_TIME FROM RAIN_DATA WHERE RTU='%s' AND SENSORS_NO='00' AND COLLECT_TIME >= '%s' AND COLLECT_TIME < '%s' ORDER BY COLLECT_TIME",
		//	pAddrItem->Get_RTU_ID(),strBegin,strEnd);

		CRainData_Item* pFirstRow = NULL;
		CRainData_Item* pLastRow = NULL;
		CRainData_Item* pBeforeRow = NULL;

		CRainData_Item* pCopyBeforeRow = NULL;  //Copy data
		int nRowsCount = 0;

#ifdef DEBUG
		GetLocalTime(&_debug_systemtime);
		TRACE("\t\tread data begin:%s Function:CalRainData_Optimize1_OnTimeRange\r\n",
			CSystemTimeOp::ConvertSystemTimeToString(&_debug_systemtime,true));
#endif

		std::multimap<CString,CRainData_Item*>::iterator rainItr;
		for (rainItr = RainDataMap.begin();
			 rainItr != RainDataMap.end();
			 rainItr ++)
		{
			CRainData_Item* pTemp  = rainItr->second;
			if (pTemp != NULL)
			{
				if (CSystemTimeOp::CompareSystemTime(&(pTemp->m_tCollectTime) ,&begintime) >=0 &&
					CSystemTimeOp::CompareSystemTime(&(pTemp->m_tCollectTime), &endtime) < 0)
				{
					if (nRowsCount == 0)
						pFirstRow =new  CRainData_Item(pTemp);

					_DELETE(pLastRow);
					pLastRow = new  CRainData_Item(pTemp);

					nRowsCount ++;
				}
				else
				{
					if (CSystemTimeOp::CompareSystemTime(&(pTemp->m_tCollectTime), &endtime) >= 0)
						break;
				}
			}
		}

#ifdef DEBUG
		//SYSTEMTIME _debug_systemtime;
		GetLocalTime(&_debug_systemtime);
		TRACE("\t\tread data end:%s Function:CalRainData_Optimize1_OnTimeRange\r\n", CSystemTimeOp::ConvertSystemTimeToString(&_debug_systemtime,true));
#endif

		if (nRowsCount == 0)
		{
			isNull = true;
			_RainData = 0.0;
		}
		else
		{
			isNull = false;
			pBeforeRow = GetEndTimeRainData_Optimize1(pAddrItem,strBegin,RainDataMap);

			if (pBeforeRow != NULL)
			{
				//Copy data
				pCopyBeforeRow = new CRainData_Item(pBeforeRow);

				CTime begin_ctime(begintime);
				CTime before_ctime(pBeforeRow->m_tCollectTime);

				CTimeSpan muts = begin_ctime - before_ctime;

				//OLD 针对0.5的误差进行修改(无漏报包时可以修复0.5的误差) 2012-12-11
				//if (muts.GetTotalMinutes() > check_time_value)
				//{
				//	_DELETE(pBeforeRow);
				//	pBeforeRow = NULL;
				//}

				if (muts.GetTotalMinutes() > check_time_value)
				{
					if (xType == Rain_statistics_5minutes ||xType == Rain_statistics_15minutes)
					{
						CTime end_ctime(pLastRow->m_tCollectTime);
						CTimeSpan muts_end = end_ctime - before_ctime;

						if (pBeforeRow->m_tCollectTime.wYear	== pFirstRow->m_tCollectTime.wYear && 
							pBeforeRow->m_tCollectTime.wMonth	== pFirstRow->m_tCollectTime.wMonth && 
							pBeforeRow->m_tCollectTime.wDay		== pFirstRow->m_tCollectTime.wDay && 
							pBeforeRow->m_tCollectTime.wHour	== pFirstRow->m_tCollectTime.wHour)
						{
							double _mut = pFirstRow->TotalValue() - pBeforeRow->TotalValue();

							if ( muts_end.GetTotalMinutes() > 15 &&(_mut < 0.0 ||  _mut > 5.0))
							{
								_DELETE(pBeforeRow);
								pBeforeRow = NULL;
							}
						}
						else 
						{
							if (muts_end.GetTotalMinutes() > 15)
							{
								_DELETE(pBeforeRow);
								pBeforeRow = NULL;
							}
						}
					}
					else 
					{
						_DELETE(pBeforeRow);
						pBeforeRow = NULL;
					}
				}
			}

			//检查first 和 last 记录的时间戳是否相同(2012-09-20)
			if (nRowsCount > 1 )
			{
				if (pFirstRow->m_tCollectTime.wYear			==	pLastRow->m_tCollectTime.wYear && 
					pFirstRow->m_tCollectTime.wMonth		==	pLastRow->m_tCollectTime.wMonth &&
					pFirstRow->m_tCollectTime.wDay			==	pLastRow->m_tCollectTime.wDay &&
					pFirstRow->m_tCollectTime.wHour			==	pLastRow->m_tCollectTime.wHour && 
					pFirstRow->m_tCollectTime.wMinute		==	pLastRow->m_tCollectTime.wMinute && 
					pFirstRow->m_tCollectTime.wSecond		==  pLastRow->m_tCollectTime.wSecond)
				{
					_DELETE(pFirstRow);
					nRowsCount = 1;
				}
			}

			if (nRowsCount == 1)
			{
				if (pBeforeRow != NULL)
				{
					if (pLastRow->TotalValue() >= pBeforeRow->TotalValue())
						_RainData = pLastRow->TotalValue() - pBeforeRow->TotalValue();
					else 
						_RainData = RainFullScale_CONST - pBeforeRow->TotalValue() + pLastRow->TotalValue();
				}
				else  //pBeforeRow == null
				{
					//Old 
					//_RainData = pLastRow->SumHalfHour();

					//采用SumHalf Rain进行修复(2012-09-20)
					if (xType == Rain_statistics_15minutes ||
						xType == Rain_statistics_1hour     ||
						xType == Rain_statistics_AM8Point)
						_RainData = pLastRow->SumHalfHour();
					else if (xType == Rain_statistics_5minutes)
					{
						if (pCopyBeforeRow == NULL)
							_RainData = pLastRow->SumHalfHour();

						else  //pCopyBeforeRow != null
						{
							_RainData = 0;
							if ((endtime.wMinute>0 && endtime.wMinute <= 15)||
								(endtime.wMinute>30 && endtime.wMinute <= 45))
								_RainData = pLastRow->SumHalfHour();
						}
					}
				}
			}
			else //nRowsCount > 1
			{
				if (pBeforeRow != NULL)
				{
					if (pLastRow->TotalValue() >= pBeforeRow->TotalValue())
						_RainData = pLastRow->TotalValue() - pBeforeRow->TotalValue();
					else
						_RainData = RainFullScale_CONST - pBeforeRow->TotalValue() + pLastRow->TotalValue();
				}
				else
				{
					//Old
					//if (pLastRow->TotalValue() >= pFirstRow->TotalValue())
					//	_RainData = pLastRow->TotalValue() - pFirstRow->TotalValue();
					//else
					//	_RainData = RainFullScale_CONST - pFirstRow->TotalValue() + pLastRow->TotalValue();

					//采用SumHalf Rain进行修复(2012-09-20)
					if (xType == Rain_statistics_15minutes ||
						xType == Rain_statistics_1hour     ||
						xType == Rain_statistics_AM8Point)
					{
						if (pLastRow->TotalValue() >= pFirstRow->TotalValue())
							_RainData = pLastRow->TotalValue() - pFirstRow->TotalValue() + pFirstRow->SumHalfHour();
						else
							_RainData = RainFullScale_CONST - pFirstRow->TotalValue() + pLastRow->TotalValue() + pFirstRow->SumHalfHour();
					}
					else if (xType == Rain_statistics_5minutes)
					{
						if (pLastRow->TotalValue() >= pFirstRow->TotalValue())
							_RainData = pLastRow->TotalValue() - pFirstRow->TotalValue() ;
						else
							_RainData = RainFullScale_CONST - pFirstRow->TotalValue() + pLastRow->TotalValue();
					}
				}
			}	

			if (_RainData > check_data_value )
			{
				if (xType == Rain_statistics_5minutes ||
					xType == Rain_statistics_15minutes)
					_RainData = pLastRow->SumHalfHour();
				else if (xType == Rain_statistics_1hour)
				{
					SYSTEMTIME begintime;
					CSystemTimeOp::ConvertStringToSysemTime(strBegin,&begintime);
					_RainData = StatisticsRain_Optimize1_1Hour_special(pAddrItem,begintime,NULL,RainDataMap);
				}
				else if (xType == Rain_statistics_AM8Point)
				{
					SYSTEMTIME daytime;
					CSystemTimeOp::ConvertStringToSysemTime(strEnd,&daytime);
					_RainData = StatisticsRain_Optimize1_1Day_special(pAddrItem,daytime,NULL,RainDataMap);
				}
			}
		}

		_DELETE(pFirstRow);
		_DELETE(pLastRow);
		_DELETE(pBeforeRow);
		_DELETE(pCopyBeforeRow);

#ifdef DEBUG
		GetLocalTime(&_debug_systemtime);
		TRACE("\t\tReturn:%f,isNull:%s,time:%s\r\n",_RainData,
			isNull?"true":"false",
			CSystemTimeOp::ConvertSystemTimeToString(&_debug_systemtime,true));
		TRACE("%s Function:CalRainData_Optimize1_OnTimeRange(%s)(end)\r\n",CSystemTimeOp::ConvertSystemTimeToString(&_debug_systemtime,true),ID);	
#endif
	}

	return _RainData;
}

CRainData_Item* CDataTranSQLServer_SC::GetEndTimeRainData_Optimize1( CAddrMap_Item* pAddrItem, CString strEnd, std::multimap<CString,CRainData_Item*>& RainDataMap )
{
	CRainData_Item* pResult = NULL;
	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_RAIN && strEnd != "")
	{
		SYSTEMTIME endtime;
		CSystemTimeOp::ConvertStringToSysemTime(strEnd,&endtime);

	/*	CString sSQL = "";
		sSQL.Format("SELECT DISTINCT DATA_TYPE,CURRENT_VAL,HALFH_VAL,TOTAL_VAL,COLLECT_TIME FROM RAIN_DATA WHERE RTU='%s' AND SENSORS_NO='00' AND COLLECT_TIME < '%s' ORDER BY COLLECT_TIME DESC LIMIT 1",
			pAddrItem->Get_RTU_ID(),strEnd);*/
		//CSqlStatement* rs =ExcuteSQLiteQuery(sSQL);
		//CString sTemp = "";
		//BYTE DataType =0;
		//double CurrentValue =0.0,HalfHourValue = 0.0,TotalValue =0.0;
		//CString CollTimeString = "";
		//SYSTEMTIME CollectTime;

#ifdef DEBUG
		SYSTEMTIME _debugtime;
		GetLocalTime(&_debugtime);
		TRACE("%s read data begin(GetEndTimeRainData_Optimize1)\r\n",
			CSystemTimeOp::ConvertSystemTimeToString(&_debugtime,true));
#endif

//		if (rs != NULL)
//		{
//			if (rs->NextRow())
//			{
//				sTemp = rs->ValueString(0);
//				DataType = (BYTE)atoi(sTemp);
//
//				sTemp = rs->ValueString(1);
//				CurrentValue = atof(sTemp);
//
//				sTemp = rs->ValueString(2);
//				HalfHourValue = atof(sTemp);
//
//				sTemp = rs->ValueString(3);
//				TotalValue =atof(sTemp);
//
//				sTemp = rs->ValueString(4);
//				CollTimeString = sTemp;
//				CSystemTimeOp::ConvertStringToSysemTime(sTemp,&CollectTime);
//
//				pResult = new CRainData_Item(DataType,CurrentValue,HalfHourValue,TotalValue,CollTimeString);
//
//#ifdef DEBUG
//				//SYSTEMTIME _debugtime;
//				GetLocalTime(&_debugtime);
//				TRACE("%s,获取时间点( <(时间点) %s)的数据:%s\r\n",CSystemTimeOp::ConvertSystemTimeToString(&_debugtime,true),
//					strEnd,
//					pResult->ToString());
//#endif
//			}
//
//			_DELETE(rs);
//		}

		CRainData_Item* pBefore = NULL;
		std::multimap<CString,CRainData_Item*>::iterator rainItr;
		for (rainItr = RainDataMap.begin();
			 rainItr != RainDataMap.end();
			 rainItr ++)
		{
			CRainData_Item* pTemp = rainItr->second;
			if (pTemp != NULL )
			{

				if (CSystemTimeOp::CompareSystemTime(&(pTemp->m_tCollectTime),&endtime) >= 0)
				{
					if (pBefore == NULL)
						return NULL;
					else
					{
						pResult = new CRainData_Item(pBefore);

#ifdef DEBUG
						GetLocalTime(&_debugtime);
						TRACE("%s Read data end(GetEndTimeRainData_Optimize1)\r\n",
							CSystemTimeOp::ConvertSystemTimeToString(&_debugtime,true));
						TRACE("%s 获取时间点(<(时间点) %s)的数据:%s\r\n",CSystemTimeOp::ConvertSystemTimeToString(&_debugtime,true),
							strEnd,
							pResult->ToString());
#endif
						break;
					}

				}
				else
				{
					pBefore = pTemp;
				}
			}
		}

	}
	return pResult;
}

double CDataTranSQLServer_SC::StatisticsRain_Optimize1_1Hour_special( CAddrMap_Item* pAddrItem,SYSTEMTIME sttime,CRainData_Item* pRecvData , std::multimap<CString,CRainData_Item*>& RainDataMap )
{
	//sttime 2012-02-06 12:00:00
	//strBegin 2012-02-06 12:00:01.000
	//strEnd   2012-02-06 13:00:01.000
	double HourRain = 0.0f;
	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_RAIN)
	{

#ifdef DEBUG
		SYSTEMTIME _debugtime;
		GetLocalTime(&_debugtime);
		TRACE("%s StatisticsRain_Optimize1_1Hour_special,ITEM:%s,TIME:%s\r\n",
			CSystemTimeOp::ConvertSystemTimeToString(&_debugtime,true),
			pAddrItem->Get_RTU_ID(),
			CSystemTimeOp::ConvertSystemTimeToString(&sttime,true));
#endif

		CString strBegin ="",strEnd ="";
		CTime  curTime(sttime);
		CTimeSpan tspan(0,1,0,0);
		CTime  endTime = curTime + tspan;

		//half hour point time
		SYSTEMTIME halfTime = sttime;
		halfTime.wMinute = 30;
		halfTime.wSecond =1;
		halfTime.wMilliseconds = 0;

		SYSTEMTIME starthourtime = sttime;
		SYSTEMTIME endhourtime = sttime;
		starthourtime.wMinute = 0;
		starthourtime.wSecond = 1;
		starthourtime.wMilliseconds = 0;

		endhourtime.wYear = endTime.GetYear();
		endhourtime.wMonth = endTime.GetMonth();
		endhourtime.wDay = endTime.GetDay();
		endhourtime.wHour = endTime.GetHour();
		endhourtime.wMinute = 0;
		endhourtime.wSecond = 1;
		endhourtime.wMilliseconds = 0;

		strBegin.Format("%04d-%02d-%02d %02d:00:01.000",
			sttime.wYear,sttime.wMonth,sttime.wDay,sttime.wHour);
		strEnd.Format("%04d-%02d-%02d %02d:00:01.000",
			endTime.GetYear(),endTime.GetMonth(),endTime.GetDay(),endTime.GetHour());

#ifdef DEBUG
		TRACE("%s BEGIN:%s,END:%s,HALF:%s\r\n",
			CSystemTimeOp::ConvertSystemTimeToString(&_debugtime,true),
			CSystemTimeOp::ConvertSystemTimeToString(&starthourtime,true),
			CSystemTimeOp::ConvertSystemTimeToString(&endhourtime,true),
			CSystemTimeOp::ConvertSystemTimeToString(&halfTime,true));
		TRACE("%s BEGIN(STR):%s,END(STR):%s\r\n",
			CSystemTimeOp::ConvertSystemTimeToString(&_debugtime,true),
			strBegin,
			strEnd);
#endif

		//CString sSQL = "";
		//sSQL.Format("SELECT DISTINCT DATA_TYPE,CURRENT_VAL,HALFH_VAL,TOTAL_VAL,COLLECT_TIME FROM RAIN_DATA WHERE RTU='%s' AND SENSORS_NO='00' AND COLLECT_TIME >= '%s' AND COLLECT_TIME < '%s' ORDER BY COLLECT_TIME"
		//	,pAddrItem->Get_RTU_ID(),strBegin,strEnd);
		//CSqlStatement* sqliters = ExcuteSQLiteQuery(sSQL);
		//if (sqliters != NULL)

		SYSTEMTIME begintime,endtime;
		CSystemTimeOp::ConvertStringToSysemTime(strBegin,&begintime);
		CSystemTimeOp::ConvertStringToSysemTime(strEnd,&endtime);

		std::multimap<CString,CRainData_Item*>::iterator rainItr;

		CString sTemp = "";
		BYTE DataType =0;
		double CurrentValue =0.0,HalfHourValue = 0.0,TotalValue =0.0;
		CString CollTimeString = "";
		SYSTEMTIME sCollectTime;

		double preHalfHourRain =0.0f, afterHalfHourRain = 0.0f;

		CRainData_Item *pAfterData = NULL;
		CRainData_Item *pPreData = NULL;

		for(rainItr = RainDataMap.begin();
			rainItr != RainDataMap.end();
			rainItr ++)
		{
			CRainData_Item* pTemp = rainItr->second;
			if (pTemp != NULL)
			{
				if (CSystemTimeOp::CompareSystemTime(&(pTemp->m_tCollectTime) ,&begintime) >=0 &&
					CSystemTimeOp::CompareSystemTime(&(pTemp->m_tCollectTime), &endtime) < 0)
				{

					DataType = pTemp->DataType();
					CurrentValue = pTemp->CurrentValue();
					HalfHourValue = pTemp->SumHalfHour();
					TotalValue =pTemp->TotalValue();
					CollTimeString = pTemp->CollectTimeString();

					CSystemTimeOp::ConvertStringToSysemTime(CollTimeString,&sCollectTime);
					int comparetime = CSystemTimeOp::CompareSystemTime(&sCollectTime,&halfTime);
					if (comparetime == -1)
					{
						_DELETE(pPreData);
						pPreData = new  CRainData_Item(DataType,CurrentValue,HalfHourValue,TotalValue,CollTimeString);
					}
					else  //=0 or 1
					{
						_DELETE(pAfterData);
						pAfterData = new  CRainData_Item(DataType,CurrentValue,HalfHourValue,TotalValue,CollTimeString);
					}
				}
				else
				{
					if (CSystemTimeOp::CompareSystemTime(&(pTemp->m_tCollectTime), &endtime) >= 0)
						break;
				}
			}
		}

		if (pRecvData != NULL)
		{
			//确定pRecvData时当前一个小时时间段的数据
			//与starttime和endtime进行比较
			int combegin = CSystemTimeOp::CompareSystemTime(&(pRecvData->m_tCollectTime),&starthourtime);
			int comEnd   = CSystemTimeOp::CompareSystemTime(&(pRecvData->m_tCollectTime),&endhourtime);

			bool isInHour = false;  //确定数据在当前一个小时时间范围内

			if ((combegin == 0 || 
				combegin == 1) && comEnd == -1)
				isInHour = true;

			if (isInHour == true)
			{
				int com = CSystemTimeOp::CompareSystemTime(&(pRecvData->m_tCollectTime),&halfTime);
				if (com == -1)
				{
					if (pPreData != NULL)
					{
						int comparedata = CSystemTimeOp::CompareSystemTime(&(pRecvData->m_tCollectTime),&(pPreData->m_tCollectTime));
						if (comparedata == 1)  //
						{
							_DELETE(pPreData);
							pPreData = new CRainData_Item(pRecvData->DataType(),
								pRecvData->CurrentValue(),
								pRecvData->SumHalfHour(),
								pRecvData->TotalValue(),
								pRecvData->CollectTimeString());
						}
					}
					else if (pPreData == NULL)
					{
						pPreData = new CRainData_Item(pRecvData->DataType(),
							pRecvData->CurrentValue(),
							pRecvData->SumHalfHour(),
							pRecvData->TotalValue(),
							pRecvData->CollectTimeString());
					}
				}
				else  //com =1 or 0
				{
					if (pAfterData != NULL)
					{
						int comparetime = CSystemTimeOp::CompareSystemTime(&(pRecvData->m_tCollectTime),&(pAfterData->m_tCollectTime));
						if (comparetime == 1)   //
						{
							_DELETE(pAfterData);
							pAfterData = new CRainData_Item(pRecvData->DataType(),
								pRecvData->CurrentValue(),
								pRecvData->SumHalfHour(),
								pRecvData->TotalValue(),
								pRecvData->CollectTimeString());
						}
					}
					else if (pAfterData == NULL)
					{
						pAfterData = new CRainData_Item(pRecvData->DataType(),
							pRecvData->CurrentValue(),
							pRecvData->SumHalfHour(),
							pRecvData->TotalValue(),
							pRecvData->CollectTimeString());
					}
				}
			}
		}

		if (pPreData != NULL)
			preHalfHourRain = pPreData->SumHalfHour();
		if (pAfterData != NULL)
			afterHalfHourRain = pAfterData->SumHalfHour();

		HourRain = preHalfHourRain + afterHalfHourRain;

		_DELETE(pPreData);
		_DELETE(pAfterData);
		

		//CSqlStatement* sqliters = ExcuteSQLiteQuery(sSQL);
		//if (sqliters != NULL)
		//{
		//	CString sTemp = "";
		//	BYTE DataType =0;
		//	double CurrentValue =0.0,HalfHourValue = 0.0,TotalValue =0.0;
		//	CString CollTimeString = "";
		//	SYSTEMTIME sCollectTime;

		//	double preHalfHourRain =0.0f, afterHalfHourRain = 0.0f;

		//	CRainData_Item *pAfterData = NULL;
		//	CRainData_Item *pPreData = NULL;

		//	while (sqliters->NextRow())
		//	{
		//		sTemp = sqliters->ValueString(0);
		//		DataType = (BYTE)atoi(sTemp);

		//		sTemp = sqliters->ValueString(1);
		//		CurrentValue = atof(sTemp);

		//		sTemp = sqliters->ValueString(2);
		//		HalfHourValue = atof(sTemp);

		//		sTemp = sqliters->ValueString(3);
		//		TotalValue =atof(sTemp);

		//		sTemp = sqliters->ValueString(4);
		//		CollTimeString = sTemp;
		//		CSystemTimeOp::ConvertStringToSysemTime(CollTimeString,&sCollectTime);

		//		int comparetime = CSystemTimeOp::CompareSystemTime(&sCollectTime,&halfTime);
		//		if (comparetime == -1)
		//		{
		//			_DELETE(pPreData);
		//			pPreData = new  CRainData_Item(DataType,CurrentValue,HalfHourValue,TotalValue,CollTimeString);
		//		}
		//		else  //=0 or 1
		//		{
		//			_DELETE(pAfterData);
		//			pAfterData = new  CRainData_Item(DataType,CurrentValue,HalfHourValue,TotalValue,CollTimeString);
		//		}
		//	}

		//	if (pRecvData != NULL)
		//	{
		//		//确定pRecvData时当前一个小时时间段的数据
		//		//与starttime和endtime进行比较
		//		int combegin = CSystemTimeOp::CompareSystemTime(&(pRecvData->m_tCollectTime),&starthourtime);
		//		int comEnd   = CSystemTimeOp::CompareSystemTime(&(pRecvData->m_tCollectTime),&endhourtime);

		//		bool isInHour = false;  //确定数据在当前一个小时时间范围内

		//		if ((combegin == 0 || 
		//			combegin == 1) && comEnd == -1)
		//			isInHour = true;

		//		if (isInHour == true)
		//		{
		//			int com = CSystemTimeOp::CompareSystemTime(&(pRecvData->m_tCollectTime),&halfTime);
		//			if (com == -1)
		//			{
		//				if (pPreData != NULL)
		//				{
		//					int comparedata = CSystemTimeOp::CompareSystemTime(&(pRecvData->m_tCollectTime),&(pPreData->m_tCollectTime));
		//					if (comparedata == 1)  //
		//					{
		//						_DELETE(pPreData);
		//						pPreData = new CRainData_Item(pRecvData->DataType(),
		//							pRecvData->CurrentValue(),
		//							pRecvData->SumHalfHour(),
		//							pRecvData->TotalValue(),
		//							pRecvData->CollectTimeString());
		//					}
		//				}
		//				else if (pPreData == NULL)
		//				{
		//					pPreData = new CRainData_Item(pRecvData->DataType(),
		//						pRecvData->CurrentValue(),
		//						pRecvData->SumHalfHour(),
		//						pRecvData->TotalValue(),
		//						pRecvData->CollectTimeString());
		//				}
		//			}
		//			else  //com =1 or 0
		//			{
		//				if (pAfterData != NULL)
		//				{
		//					int comparetime = CSystemTimeOp::CompareSystemTime(&(pRecvData->m_tCollectTime),&(pAfterData->m_tCollectTime));
		//					if (comparetime == 1)   //
		//					{
		//						_DELETE(pAfterData);
		//						pAfterData = new CRainData_Item(pRecvData->DataType(),
		//							pRecvData->CurrentValue(),
		//							pRecvData->SumHalfHour(),
		//							pRecvData->TotalValue(),
		//							pRecvData->CollectTimeString());
		//					}
		//				}
		//				else if (pAfterData == NULL)
		//				{
		//					pAfterData = new CRainData_Item(pRecvData->DataType(),
		//						pRecvData->CurrentValue(),
		//						pRecvData->SumHalfHour(),
		//						pRecvData->TotalValue(),
		//						pRecvData->CollectTimeString());
		//				}
		//			}
		//		}
		//	}

		//	if (pPreData != NULL)
		//		preHalfHourRain = pPreData->SumHalfHour();
		//	if (pAfterData != NULL)
		//		afterHalfHourRain = pAfterData->SumHalfHour();

		//	HourRain = preHalfHourRain + afterHalfHourRain;

		//	_DELETE(pPreData);
		//	_DELETE(pAfterData);
		//	_DELETE(sqliters);
		//}
	}
	return HourRain;
}

double CDataTranSQLServer_SC::StatisticsRain_Optimize1_1Day_special( CAddrMap_Item* pAddrItem,SYSTEMTIME sttime,CRainData_Item* pRecvData , std::multimap<CString,CRainData_Item*>& RainDataMap )
{
#ifdef DEBUG
	SYSTEMTIME _debugtime;
	GetLocalTime(&_debugtime);
	TRACE("%s StatisticsRain_Optimize1_1Day_special,Item:%s,TIME:%s\r\n",
		CSystemTimeOp::ConvertSystemTimeToString(&_debugtime,true),pAddrItem->Get_RTU_ID(),
		CSystemTimeOp::ConvertSystemTimeToString(&sttime,true));
#endif

	double DayRain = 0.0f;
	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_RAIN )
	{
		CTime  t(sttime);
		CTimeSpan tspan(1,0,0,0);
		CTime  t2 = t - tspan;
		CTime  t3(t2.GetYear(),t2.GetMonth(),t2.GetDay(),8,0,0);
		for (int Hours = 0; Hours < 24; Hours ++)
		{
			CTimeSpan  addspan(0,Hours,0,0);
			CTime t4 = t3 + addspan;

			SYSTEMTIME sttimehour ;
			sttimehour.wYear = t4.GetYear();
			sttimehour.wMonth = t4.GetMonth();
			sttimehour.wDay = t4.GetDay();
			sttimehour.wHour = t4.GetHour();
			sttimehour.wMinute = 0;
			sttimehour.wSecond = 0;
			sttimehour.wMilliseconds = 0;

			//double hourRain =StatisticsRain_1Hour_special(pAddrItem,sttimehour,pRecvData);
			double hourRain =StatisticsRain_Optimize1_1Hour_special(pAddrItem,sttimehour,pRecvData,RainDataMap);

			if (hourRain > 0.0)
				DayRain += hourRain;
		}
	}

#ifdef DEBUG
	GetLocalTime(&_debugtime);
	TRACE("%s StatisticsRain_Optimize1_1Day_special,Item:%s,TIME:%s,DAYRAIN:%f\r\n",
		CSystemTimeOp::ConvertSystemTimeToString(&_debugtime,true),pAddrItem->Get_RTU_ID(),
		CSystemTimeOp::ConvertSystemTimeToString(&sttime,true),
		DayRain);
#endif
	return DayRain;
}

int CDataTranSQLServer_SC::GetRainMinutes_Optimize1( CAddrMap_Item* pAddrItem, CString strBegin, CString strEnd, std::multimap<CString,CRainData_Item*>& RainDataMap )
{
	int rainMinutes = 0;

#ifdef DEBUG
	SYSTEMTIME _dbtime;
	GetLocalTime(&_dbtime);
	TRACE("%s 下雨历时(GetRainMinutes_Optimize1), Begin:%s, End:%s\r\n",CSystemTimeOp::ConvertSystemTimeToString(&_dbtime,true),strBegin,strEnd);
#endif

	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_RAIN && strBegin != "" && strEnd != "")
	{
		//CString sSQL ="";
		//sSQL.Format("SELECT DISTINCT DATA_TYPE,CURRENT_VAL,HALFH_VAL,TOTAL_VAL,COLLECT_TIME FROM RAIN_DATA WHERE RTU='%s' AND SENSORS_NO='00' AND DATA_TYPE IN ('01','02','04') AND COLLECT_TIME >= '%s' AND COLLECT_TIME < '%s' ORDER BY COLLECT_TIME"
		//	,pAddrItem->Get_RTU_ID(),strBegin,strEnd);

		//CSqlStatement* rs = ExcuteSQLiteQuery(sSQL);

		//if (rs != NULL)
		//{
		//	int RowsCount = 0;
		//	std::vector<CRainData_Item*> RainDataVector;

		//	CString sTemp = "";
		//	while (rs->NextRow())
		//	{
		//		int dataType = 0;
		//		double currentVal =0.0,totalVal = 0.0,HalfhVal = 0.0;
		//		CString sCollectTime = "";

		//		sTemp = rs->ValueString(0);
		//		dataType = atoi(sTemp);

		//		sTemp = rs->ValueString(1);
		//		currentVal = atof(sTemp);

		//		sTemp = rs->ValueString(2);
		//		HalfhVal = atof(sTemp);

		//		sTemp = rs->ValueString(3);
		//		totalVal = atof(sTemp);

		//		sTemp = rs->ValueString(4);
		//		sCollectTime = sTemp.Trim();

		//		if (dataType != 0)
		//		{
		//			CRainData_Item* pRainObj = new CRainData_Item(dataType,currentVal,HalfhVal,totalVal,sCollectTime);
		//			if (pRainObj)
		//			{
		//				CSystemTimeOp::ConvertStringToSysemTime(sCollectTime,&(pRainObj->m_tCollectTime));
		//				RainDataVector.push_back(pRainObj);
		//			}
		//			RowsCount ++;
		//		}
		//	}   //while (rs->NextRow())

		//	_DELETE(rs);

		//CSqlStatement* rs = ExcuteSQLiteQuery(sSQL);
		SYSTEMTIME begintime,endtime;
		CSystemTimeOp::ConvertStringToSysemTime(strBegin,&begintime);
		CSystemTimeOp::ConvertStringToSysemTime(strEnd,&endtime);

		int RowsCount = 0;
		std::vector<CRainData_Item*> RainDataVector;
		std::multimap<CString,CRainData_Item*>::iterator rainItr ;

		for(rainItr = RainDataMap.begin();
			rainItr != RainDataMap.end();
			rainItr ++)
		{

			CRainData_Item* pTemp = rainItr->second;
			if (pTemp != NULL)
			{
				if (CSystemTimeOp::CompareSystemTime(&(pTemp->m_tCollectTime) ,&begintime) >=0 &&
					CSystemTimeOp::CompareSystemTime(&(pTemp->m_tCollectTime), &endtime) < 0 &&
					(pTemp->DataType() ==1 || pTemp->DataType() == 2 || pTemp->DataType() ==4) )
				{

					CRainData_Item* pRainObj = new CRainData_Item(pTemp);
					if (pRainObj)
					{
						RainDataVector.push_back(pRainObj);
					}
					RowsCount ++;
				}
				else
				{
					if (CSystemTimeOp::CompareSystemTime(&(pTemp->m_tCollectTime), &endtime) >= 0)
						break;
				}
			}
		}   //while (rs->NextRow())

		//_DELETE(rs);

		//SYSTEMTIME begintime,endtime;
		//CSystemTimeOp::ConvertStringToSysemTime(strBegin,&begintime);
		//CSystemTimeOp::ConvertStringToSysemTime(strEnd,&endtime);
		CTime ctimebegin(begintime);
		CTime ctimeend(endtime);

		//计算下雨历时
		CRainData_Item* beforItem = NULL;
		for (size_t idx = 0; idx < RainDataVector.size(); idx ++)
		{
			CRainData_Item* CurObj = RainDataVector[idx];

			if (CurObj != NULL)
			{
				if (beforItem == NULL)
				{
					if (CurObj->DataType() ==  1 )
						rainMinutes = 0;
					else if (CurObj->DataType() == 2 ||
						CurObj->DataType() == 4)
					{
						CTime temptime(CurObj->m_tCollectTime);
						CTimeSpan t= temptime - ctimebegin;
						rainMinutes +=  t.GetTotalMinutes();
					}

				}
				else if (beforItem != NULL)
				{
					if (CurObj->DataType()== 1)
					{
						//rainMinutes += 0;
					}
					else if (CurObj->DataType() == 2)
					{
						if ((beforItem->DataType()== 1 ||
							beforItem->DataType() == 2))
						{
							if (CurObj->CurrentValue() >= beforItem->CurrentValue() )
							{
								CTime tempbg(beforItem->m_tCollectTime);
								CTime tempend(CurObj->m_tCollectTime);
								CTimeSpan t = tempend - tempbg ;
								rainMinutes += t.GetTotalMinutes();
							}
							else  //
							{
								//rainMinutes += 0;
							}

						}
						else if (beforItem->DataType() == 4)
						{
							//rainMinutes += 0;
						}
					}
					else if (CurObj->DataType() == 4)
					{
						if (beforItem->DataType() == 1 ||
							beforItem->DataType() == 2)
						{
							if (CurObj->CurrentValue() >= beforItem->CurrentValue())
							{
								CTime tempbg(beforItem->m_tCollectTime);
								CTime tempend(CurObj->m_tCollectTime);
								CTimeSpan t = tempend - tempbg ;
								rainMinutes += t.GetTotalMinutes();
							}
							else  //
							{
								CTime tempbg(beforItem->m_tCollectTime);
								CTime tempend(CurObj->m_tCollectTime);
								CTimeSpan t = tempend - tempbg ;
								if (t.GetTotalMinutes() >= 15)
									rainMinutes += 15;  //15分钟后判断停止
							}
						}
					}
				}

				beforItem = CurObj;
			}
		}

		if (beforItem != NULL)  //end item
		{
			if (beforItem->DataType() == 1 || 
				beforItem->DataType() == 2)
			{
				CTime tempbg(beforItem->m_tCollectTime);
				CTimeSpan t = ctimeend - tempbg;
				rainMinutes += t.GetTotalMinutes();
			}
			else if (beforItem->DataType() == 4)
			{
				//rainMinutes += 0;
			}
		}

		//clear vector;
		for (size_t idx = 0;idx < RainDataVector.size(); idx ++)
		{
			_DELETE(RainDataVector[idx]);
		}
		RainDataVector.clear();
		
	}

#ifdef DEBUG
	GetLocalTime(&_dbtime);
	TRACE("%s 下雨历时(GetRainMinutes_Optimize1)返回值:%d\r\n",CSystemTimeOp::ConvertSystemTimeToString(&_dbtime,true),rainMinutes);
#endif
	return rainMinutes;
}

void CDataTranSQLServer_SC::StatisticsRain_Optimize1_End( std::multimap<CString,CRainData_Item*>& RainDataMap )
{
	std::multimap<CString,CRainData_Item*>::iterator rainItr;
	for (rainItr = RainDataMap.begin();
		 rainItr != RainDataMap.end();
		 rainItr ++)
	{
		_DELETE(rainItr->second);
	}

	RainDataMap.clear();
}

//与雨量数据集最后时间点进行比较
// 1 :比数据集最后的数据时间戳新，或数据集为空
// 0 : =
// -1: <
int CDataTranSQLServer_SC::StatisticsRain_Optimize1_CompareEndTime( SYSTEMTIME& srctime,std::multimap<CString,CRainData_Item*>& RainDataMap )
{
#ifdef DEBUG
	SYSTEMTIME debugst;
	GetLocalTime(&debugst);
	TRACE("%s StatisticsRain_Optimize1_CompareEndTime(begin)\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
#endif
	int ret = 1;
	std::multimap<CString,CRainData_Item*>::iterator Itr = RainDataMap.end();

	if (RainDataMap.size() >0)
	{
		Itr --;

		if (Itr != RainDataMap.end())
		{
			CRainData_Item* pRain = Itr->second;
			if (pRain != NULL)
			{
				ret = CSystemTimeOp::CompareSystemTime(&srctime,&(pRain->m_tCollectTime));
#ifdef DEBUG
				TRACE("\t src:%s lastecord:%s\r\n",CSystemTimeOp::ConvertSystemTimeToString(&srctime,true),CSystemTimeOp::ConvertSystemTimeToString(&(pRain->m_tCollectTime),true));
#endif
			}
		}
	}


#ifdef DEBUG
	GetLocalTime(&debugst);
	TRACE("%s StatisticsRain_Optimize1_CompareEndTime(end), return:%d\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true),ret);
#endif
	return ret;
}

void CDataTranSQLServer_SC::StatisticsWaterLevel_Optimize1_RealTime_SBK( CAddrMap_Item* pAddrItem,CWaterL_DataItem* pRecvData )
{
#ifdef DEBUG
	SYSTEMTIME debugst;
	GetLocalTime(&debugst);
	TRACE("%s StatisticsWaterLevel_Optimize1_RealTime_SBK(begin)\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
#endif

	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_WL  && pRecvData != NULL)
	{
		//步骤1:获取2个小时的数位数据
		std::multimap<CString,CWaterL_DataItem*> WlDataMap = StatisticsWaterLevel_Optimize1_GetWaterLData(pAddrItem,pRecvData);

		//检测是否是整点数据
		if (pRecvData->m_RecordTime.wMinute == 0 && 
			pRecvData->m_RecordTime.wSecond == 0)
		{
			//更新整点数据
			CString strTM="";
			strTM.Format("%04d-%02d-%02d %02d:00",pRecvData->m_RecordTime.wYear,pRecvData->m_RecordTime.wMonth,pRecvData->m_RecordTime.wDay,pRecvData->m_RecordTime.wHour);

			//new 20140816
			int ret= 0;
			CString strFTM = strTM + ":00.000";
			SYSTEMTIME FTM;
			CSystemTimeOp::ConvertStringToSysemTime(strFTM,&FTM);
			int comp = CSystemTimeOp::CompareSystemTime(&FTM);
			if (comp <= 0)
			{
				ret = UpdateWaterLevelData(pAddrItem,strTM,pRecvData->m_dValue);
			}
			else
			{
				Table_ST_WL_R_Record record;
				record.STCD = pAddrItem->Get_STCD();
				record.TYPE = (int)pAddrItem->GetWlType();
				record.TM = strTM;
				record.FTM = FTM;
				record.PV = pRecvData->m_dValue;
				GetLocalTime(&(record.CAL_TM));

				ret = pAddrItem->UpdateST_WL_R_Record(&record);
			}

			//后1个小时的加报数据统计
			if (ret == 1 || 2 == ret)
			{
				SYSTEMTIME nexthour_begin ,nexthour_end;
				CString temp = strTM + ":01.000";
				CSystemTimeOp::ConvertStringToSysemTime(temp,&nexthour_begin);

				temp.Format("%04d-%02d-%02d %02d:59:59.999",nexthour_begin.wYear,nexthour_begin.wMonth,nexthour_begin.wDay,nexthour_begin.wHour);
				CSystemTimeOp::ConvertStringToSysemTime(temp,&nexthour_end);

				//SYSTEMTIME curST;
				//GetLocalTime(&curST);
				//int comp = CSystemTimeOp::MinusSystemTime(nexthour_begin,curST);

#ifdef DEBUG
				GetLocalTime(&debugst);
				TRACE("%s Next Hour: %s - %s \r\n",
					CSystemTimeOp::ConvertSystemTimeToString(&debugst,true),
					CSystemTimeOp::ConvertSystemTimeToString(&nexthour_begin,true),
					CSystemTimeOp::ConvertSystemTimeToString(&nexthour_end,true));
#endif
				//if (comp <= 0)
				//{
				//	StatisticsWaterLevel_Optimize1_Hour_AppendReport(pAddrItem,nexthour_begin,nexthour_end,WlDataMap);
				//}

				StatisticsWaterLevel_Optimize1_Hour_AppendReport(pAddrItem,nexthour_begin,nexthour_end,WlDataMap);
				
			}
		}
		else
		{
			//更新当前点
			CString strTM="";
			strTM.Format("%04d-%02d-%02d %02d:%02d",
				pRecvData->m_RecordTime.wYear,pRecvData->m_RecordTime.wMonth,
				pRecvData->m_RecordTime.wDay,pRecvData->m_RecordTime.wHour,pRecvData->m_RecordTime.wMinute);

			//从接口数据库里获取基准点的数据
			//未找到基准点将不存储数据
			CWaterL_DataItem* pWData = GetWaterLevelBasePoint(pAddrItem,pRecvData);
			if ( pWData != NULL &&  CUdfGlobalFUNC::CompareDoubleChange(pWData->m_dValue,pRecvData->m_dValue,(double)0.05,7,2) >= 0)
			{
				//new 20140816
				CString strFTM = strTM + ":00.000";
				SYSTEMTIME FTM;
				CSystemTimeOp::ConvertStringToSysemTime(strFTM,&FTM);
				int comp = CSystemTimeOp::CompareSystemTime(&FTM);
				if (comp <= 0)
					UpdateWaterLevelData(pAddrItem,strTM,pRecvData->m_dValue);
				else
				{
					Table_ST_WL_R_Record record;
					record.STCD = pAddrItem->Get_STCD();
					record.TYPE = (int)pAddrItem->GetWlType();
					record.TM = strTM;
					record.FTM = FTM;
					record.PV = pRecvData->m_dValue;
					GetLocalTime(&(record.CAL_TM));

					pAddrItem->UpdateST_WL_R_Record(&record);
				}
			}
			_DELETE(pWData);


			//检查是否为补报数据并进行修复
			//对一个小时内的数据重新进行修改入库
			//------------------------------------------------------
			//CString sSQL = "",_strBegin="",_strEnd="";
			//_strBegin = CSystemTimeOp::ConvertSystemTimeToString(&(pRecvData->m_RecordTime),true);
			//_strEnd.Format("%04d-%02d-%02d %02d:59:59.999",pRecvData->m_RecordTime.wYear,pRecvData->m_RecordTime.wMonth,pRecvData->m_RecordTime.wDay,pRecvData->m_RecordTime.wHour);
			//sSQL.Format("SELECT DISTINCT CURRENT_VAL,COLLECT_TIME FROM WATERL_DATA WHERE RTU='%s' AND SENSORS_NO='00' AND COLLECT_TIME  > '%s' AND COLLECT_TIME < '%s' ORDER BY COLLECT_TIME",
			//	pAddrItem->Get_RTU_ID(),_strBegin,_strEnd);
			//CSqlStatement* stmt = ExcuteSQLiteQuery(sSQL);
#ifdef DEBUG
			GetLocalTime(&debugst);
			TRACE("Begin Read: %s \r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
#endif
			CList<CWaterL_DataItem*> tempList ;
			CString  sSQL = "",_strBegin="",_strEnd="";
			_strBegin = CSystemTimeOp::ConvertSystemTimeToString(&(pRecvData->m_RecordTime),true);
			_strEnd.Format("%04d-%02d-%02d %02d:59:59.999",pRecvData->m_RecordTime.wYear,pRecvData->m_RecordTime.wMonth,pRecvData->m_RecordTime.wDay,pRecvData->m_RecordTime.wHour);
			SYSTEMTIME _begin_time,_end_time;
			CSystemTimeOp::ConvertStringToSysemTime(_strBegin,&_begin_time);
			CSystemTimeOp::ConvertStringToSysemTime(_strEnd,&_end_time);
			std::multimap<CString,CWaterL_DataItem*>::iterator wlItr;
			for (wlItr = WlDataMap.begin();
				 wlItr != WlDataMap.end();
				 wlItr ++)
			{
				CWaterL_DataItem* pTemp = wlItr->second;
				if (pTemp != NULL)
				{
					if (CSystemTimeOp::CompareSystemTime(&(pTemp->m_RecordTime),&_begin_time) > 0 &&
						CSystemTimeOp::CompareSystemTime(&(pTemp->m_RecordTime),&_end_time) < 0)
					{
							CWaterL_DataItem* pTempW = new CWaterL_DataItem(pTemp);
							if (pTempW != NULL)
								tempList.AddTail(pTempW);
					}
				}
			}
#ifdef DEBUG
			GetLocalTime(&debugst);
			TRACE("End Read: %s \r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
#endif

			if (!tempList.IsEmpty())
			{
				if (pAddrItem->GetWlType() == WL_Type_HLSW)
				{
					if (m_nDatabaseType == SQLServer_Database)
						sSQL.Format("DELETE FROM ST_RIVER_R WHERE TM > '%s' AND TM < '%s' AND STCD='%s'",_strBegin,_strEnd,pAddrItem->Get_STCD());
					else if (m_nDatabaseType == Oracle_Database)
						sSQL.Format("DELETE FROM ST_RIVER_R WHERE TM > to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3') AND TM < to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3') AND STCD='%s'",_strBegin,_strEnd,pAddrItem->Get_STCD());
				}
				else
				{
					if (m_nDatabaseType == SQLServer_Database)
						sSQL.Format("DELETE FROM ST_RSVR_R WHERE TM > '%s' AND TM < '%s' AND STCD='%s'",_strBegin,_strEnd,pAddrItem->Get_STCD());
					else if (m_nDatabaseType == Oracle_Database)
						sSQL.Format("DELETE FROM ST_RSVR_R WHERE TM > to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3') AND TM < to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3') AND STCD='%s'",_strBegin,_strEnd,pAddrItem->Get_STCD());
				}
				ExcuteNoQuery(sSQL);

				//new 20140816
				pAddrItem->RemoveST_WL_R_Record(_strBegin,_strEnd);

				//更新补报点（加报水位数据）
				while(tempList.IsEmpty() != TRUE)
				{
					CWaterL_DataItem* pTemp = tempList.RemoveHead();
					if (pTemp != NULL)
					{
						CString _tempStr = "";
						_tempStr.Format("%04d-%02d-%02d %02d:%02d",
							pTemp->m_RecordTime.wYear,pTemp->m_RecordTime.wMonth,
							pTemp->m_RecordTime.wDay,pTemp->m_RecordTime.wHour,pTemp->m_RecordTime.wMinute);

						CWaterL_DataItem* pBasic= GetWaterLevelBasePoint(pAddrItem,pTemp);
						if (pBasic != NULL && CUdfGlobalFUNC::CompareDoubleChange(pTemp->m_dValue,pBasic->m_dValue,(double)0.05,7,2) >= 0)
						{
							CString strFTM = _tempStr + ":00.000";
							SYSTEMTIME FTM ;
							CSystemTimeOp::ConvertStringToSysemTime(strFTM,&FTM);
							int comp = CSystemTimeOp::CompareSystemTime(&FTM);
							if (comp <= 0)
								UpdateWaterLevelData(pAddrItem,_tempStr,pTemp->m_dValue);
							else
							{
								Table_ST_WL_R_Record record;
								record.STCD = pAddrItem->Get_STCD();
								record.TYPE = (int)pAddrItem->GetWlType();
								record.TM = _tempStr;
								record.FTM = FTM;
								record.PV = pTemp->m_dValue;
								GetLocalTime(&(record.CAL_TM));

								pAddrItem->UpdateST_WL_R_Record(&record);
							}
						}

						_DELETE(pBasic);
						_DELETE(pTemp);
					}
				}
			}
			//------------------------------------------------------

			//更新整点数据
			CTime nowTime(pRecvData->m_RecordTime);
			CTimeSpan ttspan(0,1,0,0);
			CTime HourTime = nowTime + ttspan;
			double HourValue = 0.0;

			strTM.Format("%04d-%02d-%02d %02d:00",
				HourTime.GetYear(),HourTime.GetMonth(),HourTime.GetDay(),
				HourTime.GetHour());

			CString strFTM = strTM + ":00.000";
			SYSTEMTIME FTM;
			CSystemTimeOp::ConvertStringToSysemTime(strFTM,&FTM);
			int comp = CSystemTimeOp::CompareSystemTime(&FTM);

			CWaterL_DataItem* HourWL = GetLatestWaterLevelData_Optimize1(pAddrItem,CSystemTimeOp::ConvertSystemTimeToString(&(pRecvData->m_RecordTime),true),WlDataMap);
			if (HourWL != NULL)
			{
				INT64  muls = CSystemTimeOp::MinusSystemTime(HourWL->m_RecordTime,pRecvData->m_RecordTime);

				if (muls > 0 )		//查到的值比当前值新 (不需要更新库，之前已经存库了)
					HourValue = HourWL->m_dValue;
				else				// <= 0 的情况 即查找到的数据都没有当前的新
				{
					HourValue = pRecvData->m_dValue;

					//new 20140816
					int ret = 0;
					if (comp <= 0)
					{
						ret = UpdateWaterLevelData(pAddrItem,strTM,HourValue);
					}
					else
					{
						Table_ST_WL_R_Record record;
						record.STCD = pAddrItem->Get_STCD();
						record.TYPE = (int)pAddrItem->GetWlType();
						record.TM = strTM;
						record.FTM = FTM;
						record.PV = HourValue;
						GetLocalTime(&(record.CAL_TM));

						ret = pAddrItem->UpdateST_WL_R_Record(&record);
					}

					//后1个小时的加报数据统计
					if (ret == 1 || ret == 2)
					{
						SYSTEMTIME nexthour_begin ,nexthour_end;
						CString temp = strTM + ":01.000";
						CSystemTimeOp::ConvertStringToSysemTime(temp,&nexthour_begin);

						temp.Format("%04d-%02d-%02d %02d:59:59.999",nexthour_begin.wYear,nexthour_begin.wMonth,nexthour_begin.wDay,nexthour_begin.wHour);
						CSystemTimeOp::ConvertStringToSysemTime(temp,&nexthour_end);

						//SYSTEMTIME curST;
						//GetLocalTime(&curST);
						//int comp = CSystemTimeOp::MinusSystemTime(nexthour_begin,curST);
#ifdef DEBUG
						GetLocalTime(&debugst);
						TRACE("%s Next Hour: %s - %s \r\n",
							CSystemTimeOp::ConvertSystemTimeToString(&debugst,true),
							CSystemTimeOp::ConvertSystemTimeToString(&nexthour_begin,true),
							CSystemTimeOp::ConvertSystemTimeToString(&nexthour_end,true));
#endif
						//if (comp <= 0)
						//{
						//	StatisticsWaterLevel_Optimize1_Hour_AppendReport(pAddrItem,nexthour_begin,nexthour_end,WlDataMap);
						//}
						StatisticsWaterLevel_Optimize1_Hour_AppendReport(pAddrItem,nexthour_begin,nexthour_end,WlDataMap);

					}
				}

				_DELETE(HourWL);
			}
			
			//else if (HourWL == NULL)
			//{
			//	HourValue = pRecvData->m_dValue;

			//	//new 20140816
			//	if (comp <= 0)
			//		UpdateWaterLevelData(pAddrItem,strTM,HourValue);
			//	else
			//	{
			//		Table_ST_WL_R_Record record;
			//		record.STCD = pAddrItem->Get_STCD();
			//		record.TYPE = (int)pAddrItem->GetWlType();
			//		record.TM = strTM;
			//		record.FTM = FTM;
			//		record.PV = HourValue;
			//		GetLocalTime(&(record.CAL_TM));

			//		pAddrItem->UpdateST_WL_R_Record(&record);
			//	}
			//}
		}
		
		std::multimap<CString,CWaterL_DataItem*>::iterator delItr;
		for (delItr = WlDataMap.begin();
			 delItr != WlDataMap.end();
			 delItr ++)
		{
			_DELETE(delItr->second);
		}
		WlDataMap.clear();
	}
#ifdef DEBUG
	GetLocalTime(&debugst);
	TRACE("%s StatisticsWaterLevel_Optimize1_RealTime_SBK(end)\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
#endif
}

void CDataTranSQLServer_SC::StatisticsWaterLevel_Optimize1_Hour_AppendReport( CAddrMap_Item* pAddrItem, SYSTEMTIME begintime, SYSTEMTIME endtime,std::multimap<CString,CWaterL_DataItem*>& WLDataMap )
{
#ifdef DEBUG
	SYSTEMTIME debugst;
	GetLocalTime(&debugst);
	TRACE("%s StatisticsWaterLevel_Optimize1_Hour_AppendReport(Begin) begin:%s end: %s \r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true),
		CSystemTimeOp::ConvertSystemTimeToString(&begintime,true),CSystemTimeOp::ConvertSystemTimeToString(&endtime,true));
#endif

	//对比开始时间与数据集最后一个数据的时间
	int comp = 1;
	std::multimap<CString,CWaterL_DataItem*>::iterator lastItr = WLDataMap.end();
	CString lastTime = "null";
	if (WLDataMap.size() >0)
	{
		lastItr --;
		if (lastItr != WLDataMap.end())
		{
			CWaterL_DataItem* pWL = lastItr->second;
			if (pWL != NULL)
			{
				comp = CSystemTimeOp::CompareSystemTime(&begintime,&(pWL->m_RecordTime));
				lastTime = pWL->m_sRecordTime;
			}
		}
	}

#ifdef DEBUG
	GetLocalTime(&debugst);
	TRACE("\t comp= %d, last:%s \r\n",comp,lastTime);
#endif

	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_WL && comp <= 0)
	{
		//对一个小时内的数据重新进行修改入库(水位加报)
		//------------------------------------------------------
		CList<CWaterL_DataItem*> tempList ;
		//CString sSQL = "",_strBegin="",_strEnd="";
		//_strBegin = CSystemTimeOp::ConvertSystemTimeToString(&(pRecvData->m_RecordTime),true);
		//_strEnd.Format("%04d-%02d-%02d %02d:59:59.999",pRecvData->m_RecordTime.wYear,pRecvData->m_RecordTime.wMonth,pRecvData->m_RecordTime.wDay,pRecvData->m_RecordTime.wHour);
		//sSQL.Format("SELECT DISTINCT CURRENT_VAL,COLLECT_TIME FROM WATERL_DATA WHERE RTU='%s' AND SENSORS_NO='00' AND COLLECT_TIME  > '%s' AND COLLECT_TIME < '%s' ORDER BY COLLECT_TIME",
		//	pAddrItem->Get_RTU_ID(),_strBegin,_strEnd);
		//CSqlStatement* stmt = ExcuteSQLiteQuery(sSQL);
	#ifdef DEBUG

		GetLocalTime(&debugst);
		TRACE("%s Begin Read\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
	#endif
		CString sSQL="",_strBegin="",_strEnd="";
		_strBegin = CSystemTimeOp::ConvertSystemTimeToString(&begintime,true);
		_strEnd = CSystemTimeOp::ConvertSystemTimeToString(&endtime,true);
		std::multimap<CString,CWaterL_DataItem*>::iterator wlItr;
		for (wlItr = WLDataMap.begin();
			wlItr != WLDataMap.end();
			wlItr ++)
		{
			CWaterL_DataItem* pTemp = wlItr->second;
			if (pTemp != NULL)
			{
				if (CSystemTimeOp::CompareSystemTime(&(pTemp->m_RecordTime),&begintime) > 0 &&
					CSystemTimeOp::CompareSystemTime(&(pTemp->m_RecordTime),&endtime) < 0)
				{
					CWaterL_DataItem* pTempW = new CWaterL_DataItem(pTemp);
					if (pTempW != NULL)
						tempList.AddTail(pTempW);
				}
			}
		}

	#ifdef DEBUG
		GetLocalTime(&debugst);
		TRACE("%s End Read \r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
	#endif

		if (!tempList.IsEmpty())
		{
			if (pAddrItem->GetWlType() == WL_Type_HLSW)
			{
				if (m_nDatabaseType == SQLServer_Database)
					sSQL.Format("DELETE FROM ST_RIVER_R WHERE TM > '%s' AND TM < '%s' AND STCD='%s'",_strBegin,_strEnd,pAddrItem->Get_STCD());
				else if (m_nDatabaseType == Oracle_Database)
					sSQL.Format("DELETE FROM ST_RIVER_R WHERE TM > to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3') AND TM < to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3') AND STCD='%s'",_strBegin,_strEnd,pAddrItem->Get_STCD());
			}
			else
			{
				if (m_nDatabaseType == SQLServer_Database)
					sSQL.Format("DELETE FROM ST_RSVR_R WHERE TM > '%s' AND TM < '%s' AND STCD='%s'",_strBegin,_strEnd,pAddrItem->Get_STCD());
				else if (m_nDatabaseType == Oracle_Database)
					sSQL.Format("DELETE FROM ST_RSVR_R WHERE TM > to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3') AND TM < to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3') AND STCD='%s'",_strBegin,_strEnd,pAddrItem->Get_STCD());
			}
			ExcuteNoQuery(sSQL);

			//new 20140816
			pAddrItem->RemoveST_WL_R_Record(_strBegin,_strEnd);

			//更新补报点
			while(tempList.IsEmpty() != TRUE)
			{
				CWaterL_DataItem* pTemp = tempList.RemoveHead();
				if (pTemp != NULL)
				{
					CString _tempStr = "";
					_tempStr.Format("%04d-%02d-%02d %02d:%02d",
						pTemp->m_RecordTime.wYear,pTemp->m_RecordTime.wMonth,
						pTemp->m_RecordTime.wDay,pTemp->m_RecordTime.wHour,pTemp->m_RecordTime.wMinute);

					CWaterL_DataItem* pBasic= GetWaterLevelBasePoint(pAddrItem,pTemp);
					if (pBasic != NULL && CUdfGlobalFUNC::CompareDoubleChange(pTemp->m_dValue,pBasic->m_dValue,(double)0.05,7,2) >= 0)
					{
						CString strFTM = _tempStr + ":00.000";
						SYSTEMTIME FTM ;
						CSystemTimeOp::ConvertStringToSysemTime(strFTM,&FTM);
						int comp = CSystemTimeOp::CompareSystemTime(&FTM);
						if (comp <= 0)
							UpdateWaterLevelData(pAddrItem,_tempStr,pTemp->m_dValue);
						else
						{
							Table_ST_WL_R_Record record;
							record.STCD = pAddrItem->Get_STCD();
							record.TYPE = (int)pAddrItem->GetWlType();
							record.TM = _tempStr;
							record.FTM = FTM;
							record.PV = pTemp->m_dValue;
							GetLocalTime(&(record.CAL_TM));

							pAddrItem->UpdateST_WL_R_Record(&record);
						}
					}

					_DELETE(pBasic);
					_DELETE(pTemp);
				}
			}
		}
	}

#ifdef DEBUG
	GetLocalTime(&debugst);
	TRACE("%s StatisticsWaterLevel_Optimize1_Hour_AppendReport(end) begin:%s end: %s \r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true),
		CSystemTimeOp::ConvertSystemTimeToString(&begintime,true),CSystemTimeOp::ConvertSystemTimeToString(&endtime,true));
#endif
}

std::multimap<CString,CWaterL_DataItem*> CDataTranSQLServer_SC::StatisticsWaterLevel_Optimize1_GetWaterLData( CAddrMap_Item* pAddrItem, CWaterL_DataItem* pRecvData )
{
	std::multimap<CString,CWaterL_DataItem*> resultWlDataMap;
	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_WL  && pRecvData != NULL)
	{
		CString strBeginTime="", strEndTime = "",strTemp = "";;
		if (pRecvData->m_RecordTime.wMinute == 0 && 
			pRecvData->m_RecordTime.wSecond == 0)
		{
			strBeginTime.Format("%04d-%02d-%02d %02d:00:01.000",
				pRecvData->m_RecordTime.wYear,pRecvData->m_RecordTime.wMonth,
				pRecvData->m_RecordTime.wDay,pRecvData->m_RecordTime.wHour);
			strEndTime.Format("%04d-%02d-%02d %02d:59:59.999",
				pRecvData->m_RecordTime.wYear,pRecvData->m_RecordTime.wMonth,
				pRecvData->m_RecordTime.wDay,pRecvData->m_RecordTime.wHour);
		}
		else
		{
			strBeginTime.Format("%04d-%02d-%02d %02d:00:01.000",
				pRecvData->m_RecordTime.wYear,pRecvData->m_RecordTime.wMonth,
				pRecvData->m_RecordTime.wDay,pRecvData->m_RecordTime.wHour);

			CTime nowTime(pRecvData->m_RecordTime);
			CTimeSpan ttspan(0,1,0,0);
			CTime HourTime = nowTime + ttspan;
			double HourValue = 0.0;

			strEndTime.Format("%04d-%02d-%02d %02d:59:59.999",
				HourTime.GetYear(),HourTime.GetMonth(),HourTime.GetDay(),
				HourTime.GetHour());
		}

		CString sSQL = "";
		sSQL.Format("SELECT DISTINCT CURRENT_VAL,COLLECT_TIME FROM WATERL_DATA WHERE RTU='%s' AND SENSORS_NO='00' AND COLLECT_TIME  >= '%s' AND COLLECT_TIME < '%s' ORDER BY COLLECT_TIME",
			pAddrItem->Get_RTU_ID(),strBeginTime,strEndTime);

		CSqlStatement* stmt = ExcuteSQLiteQuery(sSQL);
#ifdef DEBUG
		SYSTEMTIME debugst;
		GetLocalTime(&debugst);
		TRACE("%s StatisticsWaterLevel_Optimize1_GetWaterLData(Begin Read) \r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
#endif
		if (stmt != NULL)
		{
			double dValue = 0.0;
			CString sCollectTime = "",sTemp="";
			while (stmt->NextRow())
			{
				sTemp= stmt->ValueString(0);
				dValue = atof(sTemp);

				sTemp = stmt->ValueString(1);
				sCollectTime = sTemp.Trim();

				SYSTEMTIME collectTime;
				CSystemTimeOp::ConvertStringToSysemTime(sCollectTime,&collectTime);

				CWaterL_DataItem* pTempW = new CWaterL_DataItem(dValue,collectTime);
				if (pTempW != NULL)
					resultWlDataMap.insert(std::pair<CString,CWaterL_DataItem*>(pTempW->m_sRecordTime,pTempW));
			}

			delete stmt;
			stmt = NULL;
		}
#ifdef DEBUG
		GetLocalTime(&debugst);
		TRACE("%s StatisticsWaterLevel_Optimize1_GetWaterLData(End Read)\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
#endif
	}
	return resultWlDataMap;
}

CWaterL_DataItem* CDataTranSQLServer_SC::GetLatestWaterLevelData_Optimize1( CAddrMap_Item* pAddrItem,CString strPoint,std::multimap<CString,CWaterL_DataItem*>& WLDataMap )
{
	CWaterL_DataItem* pWaterL = NULL;
	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_WL && strPoint != "")
	{
		SYSTEMTIME curTime ;
		CSystemTimeOp::ConvertStringToSysemTime(strPoint,&curTime);

		CTime nowTime(curTime);
		CTimeSpan addhour(0,1,0,0);
		CTime endtime = nowTime + addhour;

		CString strBegin="", strEnd = "";
		strBegin.Format("%04d-%02d-%02d %02d:00:01.000",
			nowTime.GetYear(),nowTime.GetMonth(),nowTime.GetDay(),nowTime.GetHour());
		strEnd.Format("%04d-%02d-%02d %02d:00:00.999",
			endtime.GetYear(),endtime.GetMonth(),endtime.GetDay(),endtime.GetHour());
       
#ifdef DEBUG
		SYSTEMTIME debugtime;
		GetLocalTime(&debugtime);
		TRACE("%s 获取小时水位数据, POINT: %s ,[%s ~ %s]\r\n",
			CSystemTimeOp::ConvertSystemTimeToString(&debugtime,true),strPoint,strBegin,strEnd);
#endif
		//CString sSQL = "";
		//sSQL.Format("SELECT DISTINCT CURRENT_VAL,COLLECT_TIME FROM WATERL_DATA WHERE RTU='%s' AND SENSORS_NO='00' AND COLLECT_TIME  BETWEEN '%s' AND '%s' ORDER BY COLLECT_TIME DESC LIMIT 1",
		//	pAddrItem->Get_RTU_ID(),strBegin,strEnd);
		//CSqlStatement* stmt = ExcuteSQLiteQuery(sSQL);
#ifdef DEBUG
		GetLocalTime(&debugtime);
		TRACE("Begin read: %s\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugtime,true));
#endif
		SYSTEMTIME _begin_time,_end_time;
		CSystemTimeOp::ConvertStringToSysemTime(strBegin,&_begin_time);
		CSystemTimeOp::ConvertStringToSysemTime(strEnd,&_end_time);

		std::multimap<CString,CWaterL_DataItem*>::iterator wlItr ;//=  WLDataMap.end();
		CWaterL_DataItem* pBefore = NULL;
		for (wlItr = WLDataMap.begin();
			 wlItr != WLDataMap.end();
			 wlItr ++)
		{
			CWaterL_DataItem* pTemp = wlItr->second;
			if (pTemp != NULL)
			{
				if (CSystemTimeOp::CompareSystemTime(&(pTemp->m_RecordTime),&_end_time) >0) 
				{
					break;
				}
				else
				{
					pBefore  = pTemp;
				}
			}
		}

		if (pBefore != NULL)
			pWaterL = new CWaterL_DataItem(pBefore);
		
#ifdef DEBUG
		GetLocalTime(&debugtime);
		TRACE("%s 获取小时水位数据,%s=%f\r\n",
			CSystemTimeOp::ConvertSystemTimeToString(&debugtime,true),pWaterL->m_sRecordTime,pWaterL->m_dValue);
#endif


	}

	return pWaterL;
}


void CDataTranSQLServer_SC::ReadLocalSQLiteTOUpDataCache()
{
#ifdef DEBUG
	SYSTEMTIME debugst;
#endif
	//时间范围
	SYSTEMTIME areaBegin,areaEnd;
	CalUpDataCacheTimeArea(areaBegin,areaEnd);
	CString strBegin = "",strEnd="";
	strBegin = CSystemTimeOp::ConvertSystemTimeToString(&areaBegin,true);
	strEnd = CSystemTimeOp::ConvertSystemTimeToString(&areaEnd,true);

	//查询范围
	if (m_mapAddressMap.size() <= 0)
		return;

	CString strRainYjdArea = "",strWaterLYdjArea="";
	std::list<CAddrMap_Item*>::iterator AddrItr;
	int rainCount = 0,waterlCount = 0;
	for (AddrItr = m_mapAddressMap.begin();
		 AddrItr != m_mapAddressMap.end();
		 AddrItr ++)
	{
		CAddrMap_Item* pAddrItem = (CAddrMap_Item*)(*AddrItr);
		if (pAddrItem != NULL)
		{
			pAddrItem->SetUpDataCacheTimeArea(areaBegin,areaEnd);

			if (pAddrItem->GetType() == CZ_Type_RAIN)
			{
				if (rainCount == 0)
					strRainYjdArea += "'" + pAddrItem->Get_RTU_ID() + "'" ;
				else 
					strRainYjdArea += ",'" + pAddrItem->Get_RTU_ID() + "'";
				rainCount ++;
			}
			else if (pAddrItem->GetType() == CZ_Type_WL)
			{
				if (waterlCount == 0)
					strWaterLYdjArea +=  "'" +pAddrItem->Get_RTU_ID() +"'";
				else 
					strWaterLYdjArea += ",'" + pAddrItem->Get_RTU_ID() + "'";
				waterlCount ++;
			}
		}
	}

	//读取Rain_data数据
	CString sSQL = "",strTemp = "";
	sSQL += "SELECT DISTINCT RTU,DATA_TYPE,CURRENT_VAL,HALFH_VAL,TOTAL_VAL,COLLECT_TIME FROM RAIN_DATA ";
	sSQL += "WHERE RTU IN (" + strRainYjdArea + ") AND COLLECT_TIME BETWEEN '" + strBegin + "' AND '" +strEnd + "' AND SENSORS_NO='00' ";
	sSQL += "ORDER BY RTU,COLLECT_TIME";

	CSqlStatement* rain_rs = ExcuteSQLiteQuery(sSQL);

#ifdef DEBUG
	GetLocalTime(&debugst);
	TRACE("ReadLocalSQLiteTOUpDataCache() Read rain_data begin: %s\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
#endif

	if (rain_rs != NULL)
	{
		while(rain_rs->NextRow())
		{
			CString sTemp = "";
			CString sRTU="";
			BYTE DataType =0;
			double CurrentValue =0.0,HalfHourValue = 0.0,TotalValue =0.0;
			CString CollTimeString = "";

			sTemp = rain_rs->ValueString(0);
			sRTU = sTemp.Trim();

			sTemp = rain_rs->ValueString(1);
			DataType = (BYTE)atoi(sTemp);

			sTemp = rain_rs->ValueString(2);
			CurrentValue = atof(sTemp);

			sTemp = rain_rs->ValueString(3);
			HalfHourValue = atof(sTemp);

			sTemp = rain_rs->ValueString(4);
			TotalValue =atof(sTemp);

			sTemp = rain_rs->ValueString(5);
			CollTimeString = sTemp;

			CRainData_Item* pRain =new CRainData_Item(DataType,CurrentValue,HalfHourValue,TotalValue,CollTimeString);

			for (AddrItr = m_mapAddressMap.begin();
				 AddrItr != m_mapAddressMap.end();
				 AddrItr ++)
			{
				CAddrMap_Item* pAddrItem = (CAddrMap_Item*)(*AddrItr);
				if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_RAIN && pAddrItem->Get_RTU_ID() == sRTU)
					pAddrItem->InsertData(pRain);
			}

		}
		_DELETE(rain_rs);
	}

#ifdef DEBUG
	GetLocalTime(&debugst);
	TRACE("ReadLocalSQLiteTOUpDataCache() Read rain_data end: %s\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
#endif

	//读完WaterL_data数据
	sSQL = "SELECT DISTINCT RTU,CURRENT_VAL,COLLECT_TIME FROM WATERL_DATA ";
	sSQL += "WHERE RTU IN (" + strWaterLYdjArea + ") AND COLLECT_TIME BETWEEN '" + strBegin + "' AND '" +strEnd + "' AND SENSORS_NO='00' ";
	sSQL += "ORDER BY RTU,COLLECT_TIME";

	CSqlStatement* walterl_rs = ExcuteSQLiteQuery(sSQL);

#ifdef DEBUG
	GetLocalTime(&debugst);
	TRACE("ReadLocalSQLiteTOUpDataCache() Read waterl_data begin: %s\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
#endif

	if (walterl_rs != NULL)
	{
		double dValue = 0.0;
		CString sRTU = "";
		CString sCollectTime = "",sTemp="";
		while (walterl_rs->NextRow())
		{
			sTemp = walterl_rs->ValueString(0);
			sRTU = sTemp.Trim();

			sTemp= walterl_rs->ValueString(1);
			dValue = atof(sTemp);

			sTemp = walterl_rs->ValueString(2);
			sCollectTime = sTemp.Trim();

			SYSTEMTIME collectTime;
			CSystemTimeOp::ConvertStringToSysemTime(sCollectTime,&collectTime);

			CWaterL_DataItem* pTempW = new CWaterL_DataItem(dValue,collectTime);

			for (AddrItr = m_mapAddressMap.begin();
				AddrItr != m_mapAddressMap.end();
				AddrItr ++)
			{
				CAddrMap_Item* pAddrItem = (CAddrMap_Item*)(*AddrItr);
				if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_WL && pAddrItem->Get_RTU_ID() == sRTU)
					pAddrItem->InsertData(pTempW);
			}
		}

		_DELETE(walterl_rs);
	}

#ifdef DEBUG
	GetLocalTime(&debugst);
	TRACE("ReadLocalSQLiteTOUpDataCache() Read waterl_data end: %s\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
#endif

}

void CDataTranSQLServer_SC::CalUpDataCacheTimeArea( SYSTEMTIME& AreaBegin, SYSTEMTIME& AreaEnd )
{
	SYSTEMTIME stCur;
	GetLocalTime(&stCur);

	CTime curTime(stCur);
	CTimeSpan tspan3(3,0,0,0);
	CTimeSpan tspan(1,0,0,0);

	CTime beginTime = curTime - tspan3;
	CTime endTime = curTime + tspan3;

	SYSTEMTIME st_begin;
	CString strBegin = "";

	CSystemTimeOp::ConvertCTimeToSystemtime(&beginTime,&st_begin);

	if ( (st_begin.wHour == 8 && st_begin.wMinute == 0 && st_begin.wSecond >= 1) ||
		(st_begin.wHour == 8 && st_begin.wMinute != 0) ||
		(st_begin.wHour >8 && st_begin.wHour <= 23))
	{
		strBegin.Format("%04d-%02d-%02d 08:00:01.000",st_begin.wYear,st_begin.wMonth,st_begin.wDay);
	}
	else if ((st_begin.wHour == 8 && st_begin.wMinute == 0 && st_begin.wSecond == 0) ||
		(st_begin.wHour >= 0 && st_begin.wHour < 8 ))
	{
		CTime t = beginTime - tspan;
		strBegin.Format("%04d-%02d-%02d 08:00:01.000",t.GetYear(),t.GetMonth(),t.GetDay());
	}

	CSystemTimeOp::ConvertStringToSysemTime(strBegin,&AreaBegin);
	CSystemTimeOp::ConvertCTimeToSystemtime(&endTime,&AreaEnd);

#ifdef DEBUG
	TRACE("CalUpDataCacheTimeArea() Current:%s,Area(%s-%s)\r\n",
		CSystemTimeOp::ConvertSystemTimeToString(&stCur,true),
		CSystemTimeOp::ConvertSystemTimeToString(&AreaBegin,true),
		CSystemTimeOp::ConvertSystemTimeToString(&AreaEnd,true));
#endif
}

void CDataTranSQLServer_SC::SetLocalSQLiteTransStartFlag( bool blStartFlag )
{
	m_blLocalSQliteTransStartFlag = blStartFlag;
}

void CDataTranSQLServer_SC::StatisticsRain_Optimize2_1Day_OnTimer()
{
	SYSTEMTIME nowTime;
	GetLocalTime(&nowTime);

	//if (nowTime.wHour == 8 && nowTime.wMinute == 2 && m_blStartCalc == false)
	if (nowTime.wHour == 8 && nowTime.wMinute >= 2 && nowTime.wMinute <= 15 && m_blStartCalc == false)
	{
		m_blStartCalc = true;
		m_blCalc = false;

#ifdef DEBUG
		TRACE("%s START 8点定时统计.\r\n",CSystemTimeOp::ConvertSystemTimeToString(&nowTime,true));
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
					if (m_nDatabaseType == SQLServer_Database)
						sSQL.Format("SELECT STCD, TM, DRP, INTV, PDR, DYP, WTH FROM ST_PPTN_R WHERE STCD='%s' AND TM='%s'",
						/*Itr->second->Get_STCD()*/second->Get_STCD(),strPoint);
					else if (m_nDatabaseType == Oracle_Database)
						sSQL.Format("SELECT STCD, TM, DRP, INTV, PDR, DYP, WTH FROM ST_PPTN_R WHERE STCD='%s' AND TM=to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3')",
						/*Itr->second->Get_STCD()*/second->Get_STCD(),strPoint);

					bool blfinded  = GetRainfallTableRecord(sSQL,findrecord);
					if (blfinded == false)
					{
						double Rain1Day = 0.0;
						bool   isDayNull = false;
						//Rain1Day = CalRainData_OnTimeRange(second,strBegin,strEnd,Rain_statistics_AM8Point,isDayNull);
						Rain1Day = CalRainData_Optimize1_OnTimeRange(second,strBegin,strEnd,Rain_statistics_AM8Point,isDayNull,second->m_UpDataCache.m_mapRainData);
						if (Rain1Day <=0.0 || Rain1Day > m_dCHECK_RAIN_DAY)
							Rain1Day = 0.0;

						Table_ST_PPTN_R_Record AM8Record ;
						AM8Record.STCD = second->Get_STCD();//Itr->second->Get_STCD();
						AM8Record.TM   = strPoint;
						AM8Record.DYP  = Rain1Day;
						AM8Record.WTH  = "8";

						//CALC 07:00-08:00's Rain
						if (false == isDayNull)
						{
							CString str7HourBegin = "",str7HourEnd = "";
							str7HourEnd = strEnd;
							str7HourBegin.Format("%04d-%02d-%02d 07:00:01.000",nowTime.wYear,nowTime.wMonth,nowTime.wDay);

							double hour7Rain = 0.0f;
							bool   is7HourNull = false;
							//hour7Rain = CalRainData_OnTimeRange(second,str7HourBegin,str7HourEnd,Rain_statistics_1hour,is7HourNull);
							hour7Rain = CalRainData_Optimize1_OnTimeRange(second,str7HourBegin,str7HourEnd,Rain_statistics_1hour,is7HourNull,second->m_UpDataCache.m_mapRainData);
							if (is7HourNull == false)
							{
								if (hour7Rain<=0.0 || hour7Rain > m_dCHECK_RAIN_PER)
									hour7Rain = 0.0;

								int HourRainMinutes = 0;
								if (hour7Rain >= 0.1)
								{

									if(m_RainPRDFlag != 0)
									{
										HourRainMinutes = GetRainMinutes(second,str7HourBegin,str7HourEnd);
										if (HourRainMinutes <= 0 || HourRainMinutes > 60)
											HourRainMinutes = 60;

										AM8Record.DRP = hour7Rain;
										AM8Record.INTV = 1.00;
										AM8Record.WTH = "7";
										if (HourRainMinutes >= 60 || HourRainMinutes <= 0 )
											AM8Record.PRD = 0.00;
										else
											AM8Record.PRD = (double)HourRainMinutes /100;
									}
									else 
									{

										AM8Record.DRP = hour7Rain;
										AM8Record.INTV = 1.00;
										AM8Record.WTH = "7";
										AM8Record.PRD = 0.00;
									}

								}

							}//end if (is7HourNull == false)	
						}//end if (false == isNull)

						UpdateRainfallTableRecord(AM8Record,Rain_statistics_AM8Point);
					}//end if (blfinded == false)
				}
			}

			m_blCalc = true;
		}

		//Stop calc timer
		if ( (nowTime.wHour == 8 && nowTime.wMinute >= 2 && nowTime.wMinute <= 15) != true && m_blCalc == true)
		{
			m_blStartCalc = false;
#ifdef DEBUG
			TRACE("%s STOP 8点定时统计.\r\n",
				CSystemTimeOp::ConvertSystemTimeToString(&nowTime,true));
#endif
		}
	}
}

void CDataTranSQLServer_SC::StatisticsWaterLevel_Optimize2_RealTime_SBK( CAddrMap_Item* pAddrItem,CWaterL_DataItem* pRecvData,std::multimap<CString,CWaterL_DataItem*>& WlDataMap )
{
#ifdef DEBUG
	SYSTEMTIME debugst;
	GetLocalTime(&debugst);
	TRACE("%s StatisticsWaterLevel_Optimize1_RealTime_SBK(begin)\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
#endif

	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_WL  && pRecvData != NULL)
	{
		//检测是否是整点数据
		if (pRecvData->m_RecordTime.wMinute == 0 && 
			pRecvData->m_RecordTime.wSecond == 0)
		{
			//更新整点数据
			CString strTM="";
			strTM.Format("%04d-%02d-%02d %02d:00",
				pRecvData->m_RecordTime.wYear,pRecvData->m_RecordTime.wMonth,
				pRecvData->m_RecordTime.wDay,pRecvData->m_RecordTime.wHour);

			//new 20140816
			int ret= 0;
			CString strFTM = strTM + ":00.000";
			SYSTEMTIME FTM;
			CSystemTimeOp::ConvertStringToSysemTime(strFTM,&FTM);
			int comp = CSystemTimeOp::CompareSystemTime(&FTM);
			if (comp <= 0)
			{
				ret = UpdateWaterLevelData(pAddrItem,strTM,pRecvData->m_dValue);
			}
			else
			{
				Table_ST_WL_R_Record record;
				record.STCD = pAddrItem->Get_STCD();
				record.TYPE = (int)pAddrItem->GetWlType();
				record.TM = strTM;
				record.FTM = FTM;
				record.PV = pRecvData->m_dValue;
				GetLocalTime(&(record.CAL_TM));

				ret = pAddrItem->UpdateST_WL_R_Record(&record);
			}

			//后1个小时的加报数据统计
			if (ret == 1 || 2 == ret)
			{
				SYSTEMTIME nexthour_begin ,nexthour_end;
				CString temp = strTM + ":01.000";
				CSystemTimeOp::ConvertStringToSysemTime(temp,&nexthour_begin);

				temp.Format("%04d-%02d-%02d %02d:59:59.999",nexthour_begin.wYear,nexthour_begin.wMonth,nexthour_begin.wDay,nexthour_begin.wHour);
				CSystemTimeOp::ConvertStringToSysemTime(temp,&nexthour_end);

#ifdef DEBUG
				GetLocalTime(&debugst);
				TRACE("%s Next Hour: %s - %s \r\n",
					CSystemTimeOp::ConvertSystemTimeToString(&debugst,true),
					CSystemTimeOp::ConvertSystemTimeToString(&nexthour_begin,true),
					CSystemTimeOp::ConvertSystemTimeToString(&nexthour_end,true));
#endif
				StatisticsWaterLevel_Optimize1_Hour_AppendReport(pAddrItem,nexthour_begin,nexthour_end,WlDataMap);
			}
		}
		else
		{
			//更新当前点
			CString strTM="";
			strTM.Format("%04d-%02d-%02d %02d:%02d",
				pRecvData->m_RecordTime.wYear,pRecvData->m_RecordTime.wMonth,
				pRecvData->m_RecordTime.wDay,pRecvData->m_RecordTime.wHour,pRecvData->m_RecordTime.wMinute);

			//从接口数据库里获取基准点的数据
			//未找到基准点将不存储数据
			CWaterL_DataItem* pWData = GetWaterLevelBasePoint(pAddrItem,pRecvData);
			if ( pWData != NULL &&  CUdfGlobalFUNC::CompareDoubleChange(pWData->m_dValue,pRecvData->m_dValue,(double)0.05,7,2) >= 0)
			{
				//new 20140816
				CString strFTM = strTM + ":00.000";
				SYSTEMTIME FTM;
				CSystemTimeOp::ConvertStringToSysemTime(strFTM,&FTM);
				int comp = CSystemTimeOp::CompareSystemTime(&FTM);
				if (comp <= 0)
					UpdateWaterLevelData(pAddrItem,strTM,pRecvData->m_dValue);
				else
				{
					Table_ST_WL_R_Record record;
					record.STCD = pAddrItem->Get_STCD();
					record.TYPE = (int)pAddrItem->GetWlType();
					record.TM = strTM;
					record.FTM = FTM;
					record.PV = pRecvData->m_dValue;
					GetLocalTime(&(record.CAL_TM));

					pAddrItem->UpdateST_WL_R_Record(&record);
				}
			}
			_DELETE(pWData);


			//检查是否为补报数据并进行修复
			//对一个小时内的水位数据进行重新统计其加报数据进行修改入库
			//------------------------------------------------------
#ifdef DEBUG
			GetLocalTime(&debugst);
			TRACE("Begin Read: %s \r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
#endif
			CList<CWaterL_DataItem*> tempList ;
			CString  sSQL = "",_strBegin="",_strEnd="";
			_strBegin = CSystemTimeOp::ConvertSystemTimeToString(&(pRecvData->m_RecordTime),true);
			_strEnd.Format("%04d-%02d-%02d %02d:59:59.999",pRecvData->m_RecordTime.wYear,pRecvData->m_RecordTime.wMonth,pRecvData->m_RecordTime.wDay,pRecvData->m_RecordTime.wHour);
			SYSTEMTIME _begin_time,_end_time;
			CSystemTimeOp::ConvertStringToSysemTime(_strBegin,&_begin_time);
			CSystemTimeOp::ConvertStringToSysemTime(_strEnd,&_end_time);
			std::multimap<CString,CWaterL_DataItem*>::iterator wlItr;
			for (wlItr = WlDataMap.begin();
				wlItr != WlDataMap.end();
				wlItr ++)
			{
				CWaterL_DataItem* pTemp = wlItr->second;
				if (pTemp != NULL)
				{
					if (CSystemTimeOp::CompareSystemTime(&(pTemp->m_RecordTime),&_begin_time) > 0 &&
						CSystemTimeOp::CompareSystemTime(&(pTemp->m_RecordTime),&_end_time) < 0)
					{
						CWaterL_DataItem* pTempW = new CWaterL_DataItem(pTemp);
						if (pTempW != NULL)
							tempList.AddTail(pTempW);
					}
				}
			}
#ifdef DEBUG
			GetLocalTime(&debugst);
			TRACE("End Read: %s \r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
#endif

			if (!tempList.IsEmpty())
			{
				if (pAddrItem->GetWlType() == WL_Type_HLSW)
				{
					if (m_nDatabaseType == SQLServer_Database)
						sSQL.Format("DELETE FROM ST_RIVER_R WHERE TM > '%s' AND TM < '%s' AND STCD='%s'",_strBegin,_strEnd,pAddrItem->Get_STCD());
					else if (m_nDatabaseType == Oracle_Database)
						sSQL.Format("DELETE FROM ST_RIVER_R WHERE TM > to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3') AND TM < to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3') AND STCD='%s'",_strBegin,_strEnd,pAddrItem->Get_STCD());
				}
				else
				{
					if (m_nDatabaseType == SQLServer_Database)
						sSQL.Format("DELETE FROM ST_RSVR_R WHERE TM > '%s' AND TM < '%s' AND STCD='%s'",_strBegin,_strEnd,pAddrItem->Get_STCD());
					else if (m_nDatabaseType == Oracle_Database)
						sSQL.Format("DELETE FROM ST_RSVR_R WHERE TM > to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3') AND TM < to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3') AND STCD='%s'",_strBegin,_strEnd,pAddrItem->Get_STCD());
				}
				ExcuteNoQuery(sSQL);

				//new 20140816
				pAddrItem->RemoveST_WL_R_Record(_strBegin,_strEnd);

				//更新补报点（加报水位数据）
				while(tempList.IsEmpty() != TRUE)
				{
					CWaterL_DataItem* pTemp = tempList.RemoveHead();
					if (pTemp != NULL)
					{
						CString _tempStr = "";
						_tempStr.Format("%04d-%02d-%02d %02d:%02d",
							pTemp->m_RecordTime.wYear,pTemp->m_RecordTime.wMonth,
							pTemp->m_RecordTime.wDay,pTemp->m_RecordTime.wHour,pTemp->m_RecordTime.wMinute);

						CWaterL_DataItem* pBasic= GetWaterLevelBasePoint(pAddrItem,pTemp);
						if (pBasic != NULL && CUdfGlobalFUNC::CompareDoubleChange(pTemp->m_dValue,pBasic->m_dValue,(double)0.05,7,2) >= 0)
						{
							CString strFTM = _tempStr + ":00.000";
							SYSTEMTIME FTM ;
							CSystemTimeOp::ConvertStringToSysemTime(strFTM,&FTM);
							int comp = CSystemTimeOp::CompareSystemTime(&FTM);
							if (comp <= 0)
								UpdateWaterLevelData(pAddrItem,_tempStr,pTemp->m_dValue);
							else
							{
								Table_ST_WL_R_Record record;
								record.STCD = pAddrItem->Get_STCD();
								record.TYPE = (int)pAddrItem->GetWlType();
								record.TM = _tempStr;
								record.FTM = FTM;
								record.PV = pTemp->m_dValue;
								GetLocalTime(&(record.CAL_TM));

								pAddrItem->UpdateST_WL_R_Record(&record);
							}
						}

						_DELETE(pBasic);
						_DELETE(pTemp);
					}
				}
			}
			//------------------------------------------------------

			//更新整点数据
			CTime nowTime(pRecvData->m_RecordTime);
			CTimeSpan ttspan(0,1,0,0);
			CTime HourTime = nowTime + ttspan;
			double HourValue = 0.0;

			strTM.Format("%04d-%02d-%02d %02d:00",
				HourTime.GetYear(),HourTime.GetMonth(),HourTime.GetDay(),
				HourTime.GetHour());

			CString strFTM = strTM + ":00.000";
			SYSTEMTIME FTM;
			CSystemTimeOp::ConvertStringToSysemTime(strFTM,&FTM);
			int comp = CSystemTimeOp::CompareSystemTime(&FTM);

			CWaterL_DataItem* HourWL = GetLatestWaterLevelData_Optimize1(pAddrItem,CSystemTimeOp::ConvertSystemTimeToString(&(pRecvData->m_RecordTime),true),WlDataMap);
			if (HourWL != NULL)
			{
				INT64  muls = CSystemTimeOp::MinusSystemTime(HourWL->m_RecordTime,pRecvData->m_RecordTime);

				if (muls > 0 )		//查到的值比当前值新 (不需要更新库，之前已经存库了)
					HourValue = HourWL->m_dValue;
				else				// <= 0 的情况 即查找到的数据都没有当前的新
					HourValue = pRecvData->m_dValue;
	
				//new 20140816
				int ret = 0;
				if (comp <= 0)
				{
					ret = UpdateWaterLevelData(pAddrItem,strTM,HourValue);
				}
				else
				{
					Table_ST_WL_R_Record record;
					record.STCD = pAddrItem->Get_STCD();
					record.TYPE = (int)pAddrItem->GetWlType();
					record.TM = strTM;
					record.FTM = FTM;
					record.PV = HourValue;
					GetLocalTime(&(record.CAL_TM));

					ret = pAddrItem->UpdateST_WL_R_Record(&record);
				}

				//后1个小时的加报数据统计
				if (ret == 1 || ret == 2)
				{
					SYSTEMTIME nexthour_begin ,nexthour_end;
					CString temp = strTM + ":01.000";
					CSystemTimeOp::ConvertStringToSysemTime(temp,&nexthour_begin);

					temp.Format("%04d-%02d-%02d %02d:59:59.999",nexthour_begin.wYear,nexthour_begin.wMonth,nexthour_begin.wDay,nexthour_begin.wHour);
					CSystemTimeOp::ConvertStringToSysemTime(temp,&nexthour_end);
#ifdef DEBUG
					GetLocalTime(&debugst);
					TRACE("%s Next Hour: %s - %s \r\n",
						CSystemTimeOp::ConvertSystemTimeToString(&debugst,true),
						CSystemTimeOp::ConvertSystemTimeToString(&nexthour_begin,true),
						CSystemTimeOp::ConvertSystemTimeToString(&nexthour_end,true));
#endif
					StatisticsWaterLevel_Optimize1_Hour_AppendReport(pAddrItem,nexthour_begin,nexthour_end,WlDataMap);

				}
				
				_DELETE(HourWL);
			}
		}
	}
#ifdef DEBUG
	GetLocalTime(&debugst);
	TRACE("%s StatisticsWaterLevel_Optimize2_RealTime_SBK(end)\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
#endif
}

void CDataTranSQLServer_SC::StatisticsWaterLevel_Optimize2_Enter( CAddrMap_Item* pAddrItem,CWaterL_DataItem* pRecvData )
{
	if (pAddrItem != NULL &&pAddrItem->GetType()== CZ_Type_WL && pRecvData != NULL)
	{
		CWaterL_DataItem* pCopy = new CWaterL_DataItem(pRecvData);
		pAddrItem->InsertData(pCopy);

		//计算时间范围
		CString strBeginTime="", strEndTime = "";
		if (pRecvData->m_RecordTime.wMinute == 0 && 
			pRecvData->m_RecordTime.wSecond == 0)
		{
			strBeginTime.Format("%04d-%02d-%02d %02d:00:01.000",
				pRecvData->m_RecordTime.wYear,pRecvData->m_RecordTime.wMonth,
				pRecvData->m_RecordTime.wDay,pRecvData->m_RecordTime.wHour);
			strEndTime.Format("%04d-%02d-%02d %02d:59:59.999",
				pRecvData->m_RecordTime.wYear,pRecvData->m_RecordTime.wMonth,
				pRecvData->m_RecordTime.wDay,pRecvData->m_RecordTime.wHour);
		}
		else
		{
			strBeginTime.Format("%04d-%02d-%02d %02d:00:01.000",
				pRecvData->m_RecordTime.wYear,pRecvData->m_RecordTime.wMonth,
				pRecvData->m_RecordTime.wDay,pRecvData->m_RecordTime.wHour);

			CTime nowTime(pRecvData->m_RecordTime);
			CTimeSpan ttspan(0,1,0,0);
			CTime HourTime = nowTime + ttspan;
			double HourValue = 0.0;

			strEndTime.Format("%04d-%02d-%02d %02d:59:59.999",
				HourTime.GetYear(),HourTime.GetMonth(),HourTime.GetDay(),
				HourTime.GetHour());
		}

		//插入到统计缓存
		StatisticsOnTimer_Item item;
		item.STCD = pAddrItem->Get_STCD();
		item.Type = CZ_Type_WL;
		item.WLType = pAddrItem->GetWlType();
		item.strTM = pRecvData->m_sRecordTime;
		item.strFTM = pRecvData->m_sRecordTime;
		item.stFTM = pRecvData->m_RecordTime;
		item.Cal_Type = Rain_statistics_UNKNOWN;
		item.strBegin = strBeginTime;
		item.strEnd = strEndTime;
		CSystemTimeOp::ConvertStringToSysemTime(strBeginTime,&(item.stBegin));
		CSystemTimeOp::ConvertStringToSysemTime(strEndTime,&(item.stEnd));
		GetLocalTime(&(item.CAL_TM));
		item.PV = pRecvData->m_dValue;
		item.Collect = pRecvData->m_RecordTime;

		pAddrItem->UpdateStatisticsOnTimerItem(&item);
	}
}

void CDataTranSQLServer_SC::StatisticsDisplace_Optimize2_RealTime( CAddrMap_Item* pAddrItem,CDisplace_DataItem* pRecvData,std::multimap<CString,CDisplace_DataItem*>& WlDataMap )
{
#ifdef DEBUG
	SYSTEMTIME debugst;
	GetLocalTime(&debugst);
	TRACE("%s StatisticsDisplace_Optimize1_RealTime(begin)\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
#endif

	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_DISP  && pRecvData != NULL)
	{
		//检测是否是整点数据
		if (pRecvData->m_RecordTime.wMinute == 0 && 
			pRecvData->m_RecordTime.wSecond == 0)
		{
			//更新整点数据
			CString strTM="";
			strTM.Format("%04d-%02d-%02d %02d:00",
				pRecvData->m_RecordTime.wYear,pRecvData->m_RecordTime.wMonth,
				pRecvData->m_RecordTime.wDay,pRecvData->m_RecordTime.wHour);

			//new 20140816
			int ret= 0;
			CString strFTM = strTM + ":00.000";
			SYSTEMTIME FTM;
			CSystemTimeOp::ConvertStringToSysemTime(strFTM,&FTM);
			int comp = CSystemTimeOp::CompareSystemTime(&FTM);
			if (comp <= 0)
			{
				ret = UpdateDisplaceData(pAddrItem,strTM,pRecvData->m_XValue,pRecvData->m_YValue);
			}
			else
			{
				Table_ST_DISP_R_Record record;
				record.STCD = pAddrItem->Get_STCD();
				//record.TYPE = (int)pAddrItem->GetWlType();
				record.TM = strTM;
				record.FTM = FTM;
				record.X = pRecvData->m_XValue;
				record.Y = pRecvData->m_YValue;
				GetLocalTime(&(record.CAL_TM));

				ret = pAddrItem->UpdateST_DISP_R_Record(&record);
			}

			//后1个小时的加报数据统计
			if (ret == 1 || 2 == ret)
			{
				SYSTEMTIME nexthour_begin ,nexthour_end;
				CString temp = strTM + ":01.000";
				CSystemTimeOp::ConvertStringToSysemTime(temp,&nexthour_begin);

				temp.Format("%04d-%02d-%02d %02d:59:59.999",nexthour_begin.wYear,nexthour_begin.wMonth,nexthour_begin.wDay,nexthour_begin.wHour);
				CSystemTimeOp::ConvertStringToSysemTime(temp,&nexthour_end);

#ifdef DEBUG
				GetLocalTime(&debugst);
				TRACE("%s Next Hour: %s - %s \r\n",
					CSystemTimeOp::ConvertSystemTimeToString(&debugst,true),
					CSystemTimeOp::ConvertSystemTimeToString(&nexthour_begin,true),
					CSystemTimeOp::ConvertSystemTimeToString(&nexthour_end,true));
#endif
				//StatisticsDisplace_Optimize1_Hour_AppendReport(pAddrItem,nexthour_begin,nexthour_end,WlDataMap);
			}
		}
		else
		{
			//更新当前点
			CString strTM="";
			strTM.Format("%04d-%02d-%02d %02d:%02d",
				pRecvData->m_RecordTime.wYear,pRecvData->m_RecordTime.wMonth,
				pRecvData->m_RecordTime.wDay,pRecvData->m_RecordTime.wHour,pRecvData->m_RecordTime.wMinute);

			//从接口数据库里获取基准点的数据
			//未找到基准点将不存储数据
			CDisplace_DataItem* pWData = GetDisplaceBasePoint(pAddrItem,pRecvData);
			if ( pWData != NULL &&  CUdfGlobalFUNC::CompareDoubleChange(pWData->m_XValue,pRecvData->m_XValue,(double)0.05,7,2) >= 0)
			{
				//new 20140816
				CString strFTM = strTM + ":00.000";
				SYSTEMTIME FTM;
				CSystemTimeOp::ConvertStringToSysemTime(strFTM,&FTM);
				int comp = CSystemTimeOp::CompareSystemTime(&FTM);
				if (comp <= 0)
					UpdateDisplaceData(pAddrItem,strTM,pRecvData->m_XValue,pRecvData->m_YValue);
				else
				{
					Table_ST_DISP_R_Record record;
					record.STCD = pAddrItem->Get_STCD();
					//record.TYPE = (int)pAddrItem->GetWlType();
					record.TM = strTM;
					record.FTM = FTM;
					record.X = pRecvData->m_XValue;
					record.Y = pRecvData->m_YValue;
					GetLocalTime(&(record.CAL_TM));
					pAddrItem->UpdateST_DISP_R_Record(&record);
				}
			}
			_DELETE(pWData);


			//检查是否为补报数据并进行修复
			//对一个小时内的水位数据进行重新统计其加报数据进行修改入库
			//------------------------------------------------------
#ifdef DEBUG
			GetLocalTime(&debugst);
			TRACE("Begin Read: %s \r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
#endif
			CList<CDisplace_DataItem*> tempList ;
			CString  sSQL = "",_strBegin="",_strEnd="";
			_strBegin = CSystemTimeOp::ConvertSystemTimeToString(&(pRecvData->m_RecordTime),true);
			_strEnd.Format("%04d-%02d-%02d %02d:59:59.999",pRecvData->m_RecordTime.wYear,pRecvData->m_RecordTime.wMonth,pRecvData->m_RecordTime.wDay,pRecvData->m_RecordTime.wHour);
			SYSTEMTIME _begin_time,_end_time;
			CSystemTimeOp::ConvertStringToSysemTime(_strBegin,&_begin_time);
			CSystemTimeOp::ConvertStringToSysemTime(_strEnd,&_end_time);
			std::multimap<CString,CDisplace_DataItem*>::iterator wlItr;
			for (wlItr = WlDataMap.begin();
				wlItr != WlDataMap.end();
				wlItr ++)
			{
				CDisplace_DataItem* pTemp = wlItr->second;
				if (pTemp != NULL)
				{
					if (CSystemTimeOp::CompareSystemTime(&(pTemp->m_RecordTime),&_begin_time) > 0 &&
						CSystemTimeOp::CompareSystemTime(&(pTemp->m_RecordTime),&_end_time) < 0)
					{
						CDisplace_DataItem* pTempW = new CDisplace_DataItem(pTemp);
						if (pTempW != NULL)
							tempList.AddTail(pTempW);
					}
				}
			}
#ifdef DEBUG
			GetLocalTime(&debugst);
			TRACE("End Read: %s \r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
#endif

			if (!tempList.IsEmpty())
			{
				if (m_nDatabaseType == SQLServer_Database)
						sSQL.Format("DELETE FROM ST_DISP_R WHERE TM > '%s' AND TM < '%s' AND STCD='%s'",_strBegin,_strEnd,pAddrItem->Get_STCD());
					else if (m_nDatabaseType == Oracle_Database)
						sSQL.Format("DELETE FROM ST_DISP_R WHERE TM > to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3') AND TM < to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3') AND STCD='%s'",_strBegin,_strEnd,pAddrItem->Get_STCD());
				ExcuteNoQuery(sSQL);


				//更新补报点（加报水位数据）
				while(tempList.IsEmpty() != TRUE)
				{
					CDisplace_DataItem* pTemp = tempList.RemoveHead();
					if (pTemp != NULL)
					{
						CString _tempStr = "";
						_tempStr.Format("%04d-%02d-%02d %02d:%02d",
							pTemp->m_RecordTime.wYear,pTemp->m_RecordTime.wMonth,
							pTemp->m_RecordTime.wDay,pTemp->m_RecordTime.wHour,pTemp->m_RecordTime.wMinute);

						CDisplace_DataItem* pBasic= GetDisplaceBasePoint(pAddrItem,pTemp);
						if (pBasic != NULL && ((CUdfGlobalFUNC::CompareDoubleChange(pTemp->m_XValue,pBasic->m_XValue,(double)0.05,7,2) >= 0)||(CUdfGlobalFUNC::CompareDoubleChange(pTemp->m_YValue,pBasic->m_YValue,(double)0.05,7,2) >= 0)))
						{
							CString strFTM = _tempStr + ":00.000";
							SYSTEMTIME FTM ;
							CSystemTimeOp::ConvertStringToSysemTime(strFTM,&FTM);
							int comp = CSystemTimeOp::CompareSystemTime(&FTM);
							if (comp <= 0)
								UpdateDisplaceData(pAddrItem,_tempStr,pTemp->m_XValue,pTemp->m_YValue);
							else
							{
								Table_ST_DISP_R_Record record;
								record.STCD = pAddrItem->Get_STCD();
								//record.TYPE = (int)pAddrItem->GetWlType();
								record.TM = _tempStr;
								record.FTM = FTM;
								record.X = pTemp->m_XValue;
								record.Y = pTemp->m_YValue;
								GetLocalTime(&(record.CAL_TM));

								pAddrItem->UpdateST_DISP_R_Record(&record);
							}
						}

						_DELETE(pBasic);
						_DELETE(pTemp);
					}
				}
			}
			//------------------------------------------------------

			//更新整点数据
			CTime nowTime(pRecvData->m_RecordTime);
			CTimeSpan ttspan(0,1,0,0);
			CTime HourTime = nowTime + ttspan;
			double HourValue = 0.0;

			strTM.Format("%04d-%02d-%02d %02d:00",
				HourTime.GetYear(),HourTime.GetMonth(),HourTime.GetDay(),
				HourTime.GetHour());

			CString strFTM = strTM + ":00.000";
			SYSTEMTIME FTM;
			CSystemTimeOp::ConvertStringToSysemTime(strFTM,&FTM);
			int comp = CSystemTimeOp::CompareSystemTime(&FTM);

			
		}
	}
#ifdef DEBUG
	GetLocalTime(&debugst);
	TRACE("%s StatisticsDisplace_Optimize2_RealTime(end)\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
#endif
}


void CDataTranSQLServer_SC::StatisticsDisplace_Optimize2_Enter( CAddrMap_Item* pAddrItem,CDisplace_DataItem* pRecvData )
{
	if (pAddrItem != NULL &&pAddrItem->GetType()== CZ_Type_DISP && pRecvData != NULL)
	{
		CDisplace_DataItem* pCopy = new CDisplace_DataItem(pRecvData);
		pAddrItem->InsertData(pCopy);

		//计算时间范围
		CString strBeginTime="", strEndTime = "";
		if (pRecvData->m_RecordTime.wMinute == 0 && 
			pRecvData->m_RecordTime.wSecond == 0)
		{
			strBeginTime.Format("%04d-%02d-%02d %02d:00:01.000",
				pRecvData->m_RecordTime.wYear,pRecvData->m_RecordTime.wMonth,
				pRecvData->m_RecordTime.wDay,pRecvData->m_RecordTime.wHour);
			strEndTime.Format("%04d-%02d-%02d %02d:59:59.999",
				pRecvData->m_RecordTime.wYear,pRecvData->m_RecordTime.wMonth,
				pRecvData->m_RecordTime.wDay,pRecvData->m_RecordTime.wHour);
		}
		else
		{
			strBeginTime.Format("%04d-%02d-%02d %02d:00:01.000",
				pRecvData->m_RecordTime.wYear,pRecvData->m_RecordTime.wMonth,
				pRecvData->m_RecordTime.wDay,pRecvData->m_RecordTime.wHour);

			CTime nowTime(pRecvData->m_RecordTime);
			CTimeSpan ttspan(0,1,0,0);
			CTime HourTime = nowTime + ttspan;
			double HourValue = 0.0;

			strEndTime.Format("%04d-%02d-%02d %02d:59:59.999",
				HourTime.GetYear(),HourTime.GetMonth(),HourTime.GetDay(),
				HourTime.GetHour());
		}

		//插入到统计缓存
		StatisticsOnTimer_Item item;
		item.STCD = pAddrItem->Get_STCD();
		item.Type = CZ_Type_DISP;
		item.WLType = WL_Type_UNKNOWN;
		item.RTUNo = pAddrItem->Get_RtuNo();
		item.strTM = pRecvData->m_sRecordTime;
		item.strFTM = pRecvData->m_sRecordTime;
		item.stFTM = pRecvData->m_RecordTime;
		item.Cal_Type = Rain_statistics_UNKNOWN;
		item.strBegin = strBeginTime;
		item.strEnd = strEndTime;
		CSystemTimeOp::ConvertStringToSysemTime(strBeginTime,&(item.stBegin));
		CSystemTimeOp::ConvertStringToSysemTime(strEndTime,&(item.stEnd));
		GetLocalTime(&(item.CAL_TM));
		item.X = pRecvData->m_XValue;
		item.Y = pRecvData->m_YValue;
		item.Collect = pRecvData->m_RecordTime;

		pAddrItem->UpdateStatisticsOnTimerItem(&item);
	}
}

std::multimap<CString,CDisplace_DataItem*> CDataTranSQLServer_SC::StatisticsDisplace_Optimize2_GetDisplaceDataFromSQLite( CAddrMap_Item* pAddrItem,CString strBegin,CString strEnd )
{
	std::multimap<CString,CDisplace_DataItem*> resultDataMap;
	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_DISP  )
	{
		CString strTemp = "";

		CString sSQL = "";
		//sSQL.Format("SELECT DISTINCT CURRENT_XVAL,CURRENT_YVAL,COLLECT_TIME FROM DISPLACE_DATA WHERE RTU='%s' AND SENSORS_NO='00' AND COLLECT_TIME  >= '%s' AND COLLECT_TIME <
		sSQL.Format("SELECT DISTINCT CURRENT_XVAL,CURRENT_YVAL,COLLECT_TIME FROM DISPLACE_DATA WHERE RTU='%s' AND SENSORS_NO='%d' AND COLLECT_TIME  >= '%s' AND COLLECT_TIME < '%s' ORDER BY COLLECT_TIME",
			pAddrItem->Get_RTU_ID(),pAddrItem->Get_RtuNo(),strBegin,strEnd);

		bool blLocked = false;
		if (m_localTrans != NULL)
		{
			m_localTrans->SQLiteCS_Lock();
			blLocked = true;
		}
		CSqlStatement* stmt = ExcuteSQLiteQuery(sSQL);

#ifdef DEBUG
		SYSTEMTIME debugst;
		GetLocalTime(&debugst);
		TRACE("%s StatisticsDisplace_Optimize2_GetDisplaceDataFromSQLite(Begin Read) \r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
#endif
		if (stmt != NULL)
		{
			double xValue = 0.0;
			double yValue = 0.0;
			CString sCollectTime = "",sTemp="";
			while (stmt->NextRow())
			{
				sTemp= stmt->ValueString(0);
				xValue = atof(sTemp);

				sTemp= stmt->ValueString(1);
				yValue = atof(sTemp);

				sTemp = stmt->ValueString(2);
				sCollectTime = sTemp.Trim();

				SYSTEMTIME collectTime;
				CSystemTimeOp::ConvertStringToSysemTime(sCollectTime,&collectTime);

				CDisplace_DataItem* pTempW = new CDisplace_DataItem(xValue,yValue,collectTime);
				if (pTempW != NULL)
					resultDataMap.insert(std::pair<CString,CDisplace_DataItem*>(pTempW->m_sRecordTime,pTempW));
			}

			delete stmt;
			stmt = NULL;
		}
		if (m_localTrans != NULL)
		{
			m_localTrans->SQLiteCS_UnLock();
			blLocked = false;
		}

#ifdef DEBUG
		GetLocalTime(&debugst);
		TRACE("%s StatisticsDisplace_Optimize2_GetDisplaceDataFromSQLite(End Read)\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
#endif
	}
	return resultDataMap;
}

std::multimap<CString,CDisplace_DataItem*> CDataTranSQLServer_SC::StatisticsDisplace_Optimize2_GetDisplaceDataFromUpDataCache( CAddrMap_Item* pAddrItem,CString strBegin,CString strEnd )
{
	std::multimap<CString,CDisplace_DataItem*> resultDataMap;
	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_DISP )
	{
#ifdef DEBUG
		SYSTEMTIME debugst;
		GetLocalTime(&debugst);
		TRACE("%s StatisticsDisplace_Optimize2_GetDisplaceDataFromUpDataCache(Begin Read) \r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
#endif
		SYSTEMTIME begintime,endtime;
		CSystemTimeOp::ConvertStringToSysemTime(strBegin,&begintime);
		CSystemTimeOp::ConvertStringToSysemTime(strEnd,&endtime);
		if (pAddrItem->m_UpDataCache.m_mapDisplaceData.size()>0)
		{
			std::multimap<CString,CDisplace_DataItem*>::reverse_iterator Itr ;
			for(Itr = pAddrItem->m_UpDataCache.m_mapDisplaceData.rbegin();
				Itr != pAddrItem->m_UpDataCache.m_mapDisplaceData.rend();
				Itr ++)
			{
				CDisplace_DataItem* pObj = Itr->second;
				if (pObj != NULL)
				{
					if (CSystemTimeOp::CompareSystemTime(&(pObj->m_RecordTime),&begintime) >=0 &&
						CSystemTimeOp::CompareSystemTime(&(pObj->m_RecordTime),&endtime) <=0)
					{
						CDisplace_DataItem* pCopy = new CDisplace_DataItem(pObj);
						resultDataMap.insert(std::pair<CString,CDisplace_DataItem*>(pCopy->m_sRecordTime,pCopy));
					}

					if (CSystemTimeOp::CompareSystemTime(&(pObj->m_RecordTime),&begintime) < 0)
						break;
				}
			}
		}

#ifdef DEBUG
		GetLocalTime(&debugst);
		TRACE("%s StatisticsDisplace_Optimize2_GetDisplaceDataFromUpDataCache(End Read)\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
#endif
	}
	return resultDataMap;
}


CDisplace_DataItem* CDataTranSQLServer_SC::GetDisplaceBasePoint( CAddrMap_Item* pAddrItem, CDisplace_DataItem* pRecvData )
{
	CDisplace_DataItem* pResult = NULL;
	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_DISP && pRecvData != NULL)
	{
		CString strBegin= "",strEnd = "";
		strBegin.Format("%04d-%02d-%02d %02d:00:00.000",
			pRecvData->m_RecordTime.wYear,
			pRecvData->m_RecordTime.wMonth,
			pRecvData->m_RecordTime.wDay,
			pRecvData->m_RecordTime.wHour);

		strEnd = CSystemTimeOp::ConvertSystemTimeToString(&(pRecvData->m_RecordTime),true);

		CString sSQL = "";
		if (m_nDatabaseType == SQLServer_Database)
			{
				sSQL.Format("SELECT TOP 1 X,Y,TM AS T FROM ST_DISP_R WHERE STCD='%s' AND TM BETWEEN '%s' and '%s' ORDER BY TM DESC",pAddrItem->Get_STCD(),strBegin,strEnd);
			}
		else if (m_nDatabaseType == Oracle_Database)
			{
				sSQL.Format("SELECT X,Y,TO_CHAR(TM,'YYYY-MM-DD HH24:MI:SS.ff3') AS T FROM ST_DISP_R WHERE STCD='%s' AND TM BETWEEN to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3') and to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3') ORDER BY TM DESC",pAddrItem->Get_STCD(),strBegin,strEnd);
		}

		if (sSQL != "")
		{
			try
			{

#ifdef DEBUG
				SYSTEMTIME debugTime;
				GetLocalTime(&debugTime);
				TRACE("%s 查找位移数据,SQL:%s\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugTime,true),sSQL);
#endif
				CRecordset rs(&m_Database);

				rs.Open(AFX_DB_USE_DEFAULT_TYPE,sSQL);

				CString sTemp = "";
				if(! rs.IsEOF())
				{
					double xData= 0.0;
					double yData= 0.0;
					SYSTEMTIME st;

					rs.GetFieldValue("X",sTemp);
					xData = atof(sTemp);

					rs.GetFieldValue("Y",sTemp);
					yData = atof(sTemp);

					rs.GetFieldValue("T",sTemp);
					CSystemTimeOp::ConvertStringToSysemTime(sTemp,&st);

					pResult = new CDisplace_DataItem(xData,yData,st);
#ifdef DEBUG
					GetLocalTime(&debugTime);
					TRACE("%s 查找到位移数据:%s\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugTime,true),pResult->ToString());
#endif
					
				}
				rs.Close();
			}
			catch (CDBException* e)
			{
				CString sLog = "";
				sLog.Format("执行SQL:%s,信息:%s",sSQL,e->m_strError);
				e->Delete();
				AddThreadLog(sLog,Warning_log_Message);
				_DELETE(pResult);

			}

		}
	}
	return pResult;
}
/*
* 更新 strTM点的数据
* 查找到数据 更新 Update
* 未查找到数据 添加 Insert
* 返回： 1: Insert  2: update  3: exist (not Update)
*/
int  CDataTranSQLServer_SC::UpdateDisplaceData( CAddrMap_Item* pAddrItem, CString strTM, double xValue , double yValue)
{
	//bool ret = true;
	int ret = 0;

	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_DISP && strTM != "" )
	{
		CString sSQL = "";
		bool isExist = false;       //Record whether exist
		double xValueInDb = 0.0;     //Data In Database
		double yValueInDb = 0.0;     //Data In Database


		//查询
			if (m_nDatabaseType == SQLServer_Database)
			{
				sSQL.Format("SELECT X,Y FROM ST_DISP_R WHERE STCD='%s' AND TM = '%s'",
					pAddrItem->Get_STCD(),strTM);
			}
			else if (m_nDatabaseType == Oracle_Database)
			{
				sSQL.Format("SELECT X,Y FROM ST_DISP_R WHERE STCD='%s' AND TM = to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3')",
					pAddrItem->Get_STCD(),strTM);
			}


			isExist = GetDisplaceData(sSQL,xValueInDb,yValueInDb);

			if (false == isExist)     //INSERT
			{
				ret = 1;
				if (m_nDatabaseType == SQLServer_Database)
				{
					sSQL.Format("INSERT INTO ST_DISP_R (STCD ,TM, X, Y) VALUES ('%s','%s',%.2f,%.2f)",
						pAddrItem->Get_STCD(),strTM,xValue,yValue);
				}
				else if (m_nDatabaseType == Oracle_Database)
				{
					sSQL.Format("INSERT INTO ST_DISP_R (STCD ,TM, X, Y) VALUES ('%s',to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3'),%.2f,%.2f)",
						pAddrItem->Get_STCD(),strTM,xValue,yValue);
				}

				ret = ExcuteNoQuery(sSQL);
			}
			else if (true == isExist) //UPDATE
			{
				ret = 3;
				//if (ValueInDb != pointValue)
				if ((fabs(xValueInDb - xValue) >= 0.01)||(fabs(yValueInDb - yValue) >= 0.01 ))
				{
					ret = 2;
#ifdef DEBUG
					TRACE("DB:%f\r\nX:%f\r\n",xValueInDb,xValue);
#endif

					if (m_nDatabaseType == SQLServer_Database)
					{
						sSQL.Format("UPDATE ST_DISP_R SET X = %.2f Y = %.2f WHERE STCD='%s' AND TM = '%s'",
							xValue,yValue,pAddrItem->Get_STCD(),strTM);
					}
					else if (m_nDatabaseType == Oracle_Database)
					{
						sSQL.Format("UPDATE ST_DISP_R SET Z = %.2f Y = %.2f WHERE STCD='%s' AND TM = to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3')",
							xValue,yValue,pAddrItem->Get_STCD(),strTM);
					}


					ret = ExcuteNoQuery(sSQL);
				}
			}
	}
	
	return ret;
}

bool CDataTranSQLServer_SC::GetDisplaceData( CString sSQL,double& xData,double& yData )
{
	bool ret = false;
	xData = 0.0;
	yData = 0.0;
	CString sLog = "";
	try
	{

#ifdef DEBUG
		SYSTEMTIME debugTime;
		GetLocalTime(&debugTime);
		TRACE("%s 查找位移数据,SQL:%s\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugTime,true),sSQL);
#endif
		CRecordset rs(&m_Database);

		rs.Open(AFX_DB_USE_DEFAULT_TYPE,sSQL);

		CString sTemp = "";
		if(! rs.IsEOF())
		{
			rs.GetFieldValue("X",sTemp);
			xData = atof(sTemp);
			rs.GetFieldValue("Y",sTemp);
			yData = atof(sTemp);

#ifdef DEBUG
			GetLocalTime(&debugTime);
			TRACE("%s 查找到位移数据:x=%f  y=%f\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugTime,true),xData,yData);
#endif
			ret = true;
		}
		rs.Close();
	}
	catch (CDBException* e)
	{
		sLog.Format("执行SQL:%s,信息:%s",sSQL,e->m_strError);
		e->Delete();
		//AddThreadLog(sLog,Error_Log_message);
		AddThreadLog(sLog,Warning_log_Message);
		ret = false;
	}

	return ret;
}

void CDataTranSQLServer_SC::StatisticsTempera_Optimize2_Enter( CAddrMap_Item* pAddrItem,CTempera_DataItem* pRecvData )
{
	if (pAddrItem != NULL &&pAddrItem->GetType()== CZ_Type_TEMP && pRecvData != NULL)
	{
		CTempera_DataItem* pCopy = new CTempera_DataItem(pRecvData);
		pAddrItem->InsertData(pCopy);

		//计算时间范围
		CString strBeginTime="", strEndTime = "";
		if (pRecvData->m_RecordTime.wMinute == 0 && 
			pRecvData->m_RecordTime.wSecond == 0)
		{
			strBeginTime.Format("%04d-%02d-%02d %02d:00:01.000",
				pRecvData->m_RecordTime.wYear,pRecvData->m_RecordTime.wMonth,
				pRecvData->m_RecordTime.wDay,pRecvData->m_RecordTime.wHour);
			strEndTime.Format("%04d-%02d-%02d %02d:59:59.999",
				pRecvData->m_RecordTime.wYear,pRecvData->m_RecordTime.wMonth,
				pRecvData->m_RecordTime.wDay,pRecvData->m_RecordTime.wHour);
		}
		else
		{
			strBeginTime.Format("%04d-%02d-%02d %02d:00:01.000",
				pRecvData->m_RecordTime.wYear,pRecvData->m_RecordTime.wMonth,
				pRecvData->m_RecordTime.wDay,pRecvData->m_RecordTime.wHour);

			CTime nowTime(pRecvData->m_RecordTime);
			CTimeSpan ttspan(0,1,0,0);
			CTime HourTime = nowTime + ttspan;
			double HourValue = 0.0;

			strEndTime.Format("%04d-%02d-%02d %02d:59:59.999",
				HourTime.GetYear(),HourTime.GetMonth(),HourTime.GetDay(),
				HourTime.GetHour());
		}

		//插入到统计缓存
		StatisticsOnTimer_Item item;
		item.STCD = pAddrItem->Get_STCD();
		item.Type = CZ_Type_TEMP;
		item.WLType = WL_Type_UNKNOWN;
		item.strTM = pRecvData->m_sRecordTime;
		item.strFTM = pRecvData->m_sRecordTime;
		item.stFTM = pRecvData->m_RecordTime;
		item.Cal_Type = Rain_statistics_UNKNOWN;
		item.strBegin = strBeginTime;
		item.strEnd = strEndTime;
		CSystemTimeOp::ConvertStringToSysemTime(strBeginTime,&(item.stBegin));
		CSystemTimeOp::ConvertStringToSysemTime(strEndTime,&(item.stEnd));
		GetLocalTime(&(item.CAL_TM));
		item.T = pRecvData->m_tValue;
		item.Collect = pRecvData->m_RecordTime;

		pAddrItem->UpdateStatisticsOnTimerItem(&item);
	}
}

std::multimap<CString,CTempera_DataItem*> CDataTranSQLServer_SC::StatisticsTempera_Optimize2_GetTemperaDataFromSQLite( CAddrMap_Item* pAddrItem,CString strBegin,CString strEnd )
{
	std::multimap<CString,CTempera_DataItem*> resultDataMap;
	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_TEMP  )
	{
		CString strTemp = "";

		CString sSQL = "";
		sSQL.Format("SELECT DISTINCT CURRENT_TVAL,COLLECT_TIME FROM TEMPERA_DATA WHERE RTU='%s' AND SENSORS_NO='00' AND COLLECT_TIME  >= '%s' AND COLLECT_TIME < '%s' ORDER BY COLLECT_TIME",
			pAddrItem->Get_RTU_ID(),strBegin,strEnd);

		bool blLocked = false;
		if (m_localTrans != NULL)
		{
			m_localTrans->SQLiteCS_Lock();
			blLocked = true;
		}
		CSqlStatement* stmt = ExcuteSQLiteQuery(sSQL);

#ifdef DEBUG
		SYSTEMTIME debugst;
		GetLocalTime(&debugst);
		TRACE("%s StatisticsTempera_Optimize2_GetTemperaDataFromSQLite(Begin Read) \r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
#endif
		if (stmt != NULL)
		{
			double tValue = 0.0;
			CString sCollectTime = "",sTemp="";
			while (stmt->NextRow())
			{
				sTemp= stmt->ValueString(0);
				tValue = atof(sTemp);

				sTemp = stmt->ValueString(1);
				sCollectTime = sTemp.Trim();

				SYSTEMTIME collectTime;
				CSystemTimeOp::ConvertStringToSysemTime(sCollectTime,&collectTime);

				CTempera_DataItem* pTempW = new CTempera_DataItem(tValue,collectTime);
				if (pTempW != NULL)
					resultDataMap.insert(std::pair<CString,CTempera_DataItem*>(pTempW->m_sRecordTime,pTempW));
			}

			delete stmt;
			stmt = NULL;
		}
		if (m_localTrans != NULL)
		{
			m_localTrans->SQLiteCS_UnLock();
			blLocked = false;
		}

#ifdef DEBUG
		GetLocalTime(&debugst);
		TRACE("%s StatisticsTempera_Optimize2_GetTemperaDataFromSQLite(End Read)\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
#endif
	}
	return resultDataMap;
}

std::multimap<CString,CTempera_DataItem*> CDataTranSQLServer_SC::StatisticsTempera_Optimize2_GetTemperaDataFromUpDataCache( CAddrMap_Item* pAddrItem,CString strBegin,CString strEnd )
{
	std::multimap<CString,CTempera_DataItem*> resultDataMap;
	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_TEMP )
	{
#ifdef DEBUG
		SYSTEMTIME debugst;
		GetLocalTime(&debugst);
		TRACE("%s StatisticsTempera_Optimize2_GetTemperaDataFromUpDataCache(Begin Read) \r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
#endif
		SYSTEMTIME begintime,endtime;
		CSystemTimeOp::ConvertStringToSysemTime(strBegin,&begintime);
		CSystemTimeOp::ConvertStringToSysemTime(strEnd,&endtime);
		if (pAddrItem->m_UpDataCache.m_mapTemperaData.size()>0)
		{
			std::multimap<CString,CTempera_DataItem*>::reverse_iterator Itr ;
			for(Itr = pAddrItem->m_UpDataCache.m_mapTemperaData.rbegin();
				Itr != pAddrItem->m_UpDataCache.m_mapTemperaData.rend();
				Itr ++)
			{
				CTempera_DataItem* pObj = Itr->second;
				if (pObj != NULL)
				{
					if (CSystemTimeOp::CompareSystemTime(&(pObj->m_RecordTime),&begintime) >=0 &&
						CSystemTimeOp::CompareSystemTime(&(pObj->m_RecordTime),&endtime) <=0)
					{
						CTempera_DataItem* pCopy = new CTempera_DataItem(pObj);
						resultDataMap.insert(std::pair<CString,CTempera_DataItem*>(pCopy->m_sRecordTime,pCopy));
					}

					if (CSystemTimeOp::CompareSystemTime(&(pObj->m_RecordTime),&begintime) < 0)
						break;
				}
			}
		}

#ifdef DEBUG
		GetLocalTime(&debugst);
		TRACE("%s StatisticsTempera_Optimize2_GetTemperaDataFromUpDataCache(End Read)\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
#endif
	}
	return resultDataMap;
}

void CDataTranSQLServer_SC::StatisticsTempera_Optimize2_RealTime( CAddrMap_Item* pAddrItem,CTempera_DataItem* pRecvData,std::multimap<CString,CTempera_DataItem*>& WlDataMap )
{
#ifdef DEBUG
	SYSTEMTIME debugst;
	GetLocalTime(&debugst);
	TRACE("%s StatisticsTempera_Optimize1_RealTime(begin)\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
#endif

	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_TEMP  && pRecvData != NULL)
	{
		//检测是否是整点数据
		if (pRecvData->m_RecordTime.wMinute == 0 && 
			pRecvData->m_RecordTime.wSecond == 0)
		{
			//更新整点数据
			CString strTM="";
			strTM.Format("%04d-%02d-%02d %02d:00",
				pRecvData->m_RecordTime.wYear,pRecvData->m_RecordTime.wMonth,
				pRecvData->m_RecordTime.wDay,pRecvData->m_RecordTime.wHour);

			//new 20140816
			int ret= 0;
			CString strFTM = strTM + ":00.000";
			SYSTEMTIME FTM;
			CSystemTimeOp::ConvertStringToSysemTime(strFTM,&FTM);
			int comp = CSystemTimeOp::CompareSystemTime(&FTM);
			if (comp <= 0)
			{
				ret = UpdateTemperaData(pAddrItem,strTM,pRecvData->m_tValue);
			}
			else
			{
				Table_ST_TEMP_R_Record record;
				record.STCD = pAddrItem->Get_STCD();
				//record.TYPE = (int)pAddrItem->GetWlType();
				record.TM = strTM;
				record.FTM = FTM;
				record.T = pRecvData->m_tValue;
				GetLocalTime(&(record.CAL_TM));

				ret = pAddrItem->UpdateST_TEMP_R_Record(&record);
			}

			//后1个小时的加报数据统计
			if (ret == 1 || 2 == ret)
			{
				SYSTEMTIME nexthour_begin ,nexthour_end;
				CString temp = strTM + ":01.000";
				CSystemTimeOp::ConvertStringToSysemTime(temp,&nexthour_begin);

				temp.Format("%04d-%02d-%02d %02d:59:59.999",nexthour_begin.wYear,nexthour_begin.wMonth,nexthour_begin.wDay,nexthour_begin.wHour);
				CSystemTimeOp::ConvertStringToSysemTime(temp,&nexthour_end);

#ifdef DEBUG
				GetLocalTime(&debugst);
				TRACE("%s Next Hour: %s - %s \r\n",
					CSystemTimeOp::ConvertSystemTimeToString(&debugst,true),
					CSystemTimeOp::ConvertSystemTimeToString(&nexthour_begin,true),
					CSystemTimeOp::ConvertSystemTimeToString(&nexthour_end,true));
#endif
				//StatisticsTempera_Optimize1_Hour_AppendReport(pAddrItem,nexthour_begin,nexthour_end,WlDataMap);
			}
		}
		else
		{
			//更新当前点
			CString strTM="";
			strTM.Format("%04d-%02d-%02d %02d:%02d",
				pRecvData->m_RecordTime.wYear,pRecvData->m_RecordTime.wMonth,
				pRecvData->m_RecordTime.wDay,pRecvData->m_RecordTime.wHour,pRecvData->m_RecordTime.wMinute);

			//从接口数据库里获取基准点的数据
			//未找到基准点将不存储数据
			CTempera_DataItem* pWData = GetTemperaBasePoint(pAddrItem,pRecvData);
			if ( pWData != NULL &&  CUdfGlobalFUNC::CompareDoubleChange(pWData->m_tValue,pRecvData->m_tValue,(double)0.05,7,2) >= 0)
			{
				//new 20140816
				CString strFTM = strTM + ":00.000";
				SYSTEMTIME FTM;
				CSystemTimeOp::ConvertStringToSysemTime(strFTM,&FTM);
				int comp = CSystemTimeOp::CompareSystemTime(&FTM);
				if (comp <= 0)
					UpdateTemperaData(pAddrItem,strTM,pRecvData->m_tValue);
				else
				{
					Table_ST_TEMP_R_Record record;
					record.STCD = pAddrItem->Get_STCD();
					//record.TYPE = (int)pAddrItem->GetWlType();
					record.TM = strTM;
					record.FTM = FTM;
					record.T = pRecvData->m_tValue;
					GetLocalTime(&(record.CAL_TM));

					pAddrItem->UpdateST_TEMP_R_Record(&record);
				}
			}
			_DELETE(pWData);


			//检查是否为补报数据并进行修复
			//对一个小时内的水位数据进行重新统计其加报数据进行修改入库
			//------------------------------------------------------
#ifdef DEBUG
			GetLocalTime(&debugst);
			TRACE("Begin Read: %s \r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
#endif
			CList<CTempera_DataItem*> tempList ;
			CString  sSQL = "",_strBegin="",_strEnd="";
			_strBegin = CSystemTimeOp::ConvertSystemTimeToString(&(pRecvData->m_RecordTime),true);
			_strEnd.Format("%04d-%02d-%02d %02d:59:59.999",pRecvData->m_RecordTime.wYear,pRecvData->m_RecordTime.wMonth,pRecvData->m_RecordTime.wDay,pRecvData->m_RecordTime.wHour);
			SYSTEMTIME _begin_time,_end_time;
			CSystemTimeOp::ConvertStringToSysemTime(_strBegin,&_begin_time);
			CSystemTimeOp::ConvertStringToSysemTime(_strEnd,&_end_time);
			std::multimap<CString,CTempera_DataItem*>::iterator wlItr;
			for (wlItr = WlDataMap.begin();
				wlItr != WlDataMap.end();
				wlItr ++)
			{
				CTempera_DataItem* pTemp = wlItr->second;
				if (pTemp != NULL)
				{
					if (CSystemTimeOp::CompareSystemTime(&(pTemp->m_RecordTime),&_begin_time) > 0 &&
						CSystemTimeOp::CompareSystemTime(&(pTemp->m_RecordTime),&_end_time) < 0)
					{
						CTempera_DataItem* pTempW = new CTempera_DataItem(pTemp);
						if (pTempW != NULL)
							tempList.AddTail(pTempW);
					}
				}
			}
#ifdef DEBUG
			GetLocalTime(&debugst);
			TRACE("End Read: %s \r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
#endif

			if (!tempList.IsEmpty())
			{
				if (m_nDatabaseType == SQLServer_Database)
						sSQL.Format("DELETE FROM ST_TEMP_R WHERE TM > '%s' AND TM < '%s' AND STCD='%s'",_strBegin,_strEnd,pAddrItem->Get_STCD());
					else if (m_nDatabaseType == Oracle_Database)
						sSQL.Format("DELETE FROM ST_TEMP_R WHERE TM > to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3') AND TM < to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3') AND STCD='%s'",_strBegin,_strEnd,pAddrItem->Get_STCD());
				ExcuteNoQuery(sSQL);


				//更新补报点（加报水位数据）
				while(tempList.IsEmpty() != TRUE)
				{
					CTempera_DataItem* pTemp = tempList.RemoveHead();
					if (pTemp != NULL)
					{
						CString _tempStr = "";
						_tempStr.Format("%04d-%02d-%02d %02d:%02d",
							pTemp->m_RecordTime.wYear,pTemp->m_RecordTime.wMonth,
							pTemp->m_RecordTime.wDay,pTemp->m_RecordTime.wHour,pTemp->m_RecordTime.wMinute);

						CTempera_DataItem* pBasic= GetTemperaBasePoint(pAddrItem,pTemp);
						if (pBasic != NULL && (CUdfGlobalFUNC::CompareDoubleChange(pTemp->m_tValue,pBasic->m_tValue,(double)0.05,7,2) >= 0))
						{
							CString strFTM = _tempStr + ":00.000";
							SYSTEMTIME FTM ;
							CSystemTimeOp::ConvertStringToSysemTime(strFTM,&FTM);
							int comp = CSystemTimeOp::CompareSystemTime(&FTM);
							if (comp <= 0)
								UpdateTemperaData(pAddrItem,_tempStr,pTemp->m_tValue);
							else
							{
								Table_ST_TEMP_R_Record record;
								record.STCD = pAddrItem->Get_STCD();
								//record.TYPE = (int)pAddrItem->GetWlType();
								record.TM = _tempStr;
								record.FTM = FTM;
								record.T = pTemp->m_tValue;
								GetLocalTime(&(record.CAL_TM));

								pAddrItem->UpdateST_TEMP_R_Record(&record);
							}
						}

						_DELETE(pBasic);
						_DELETE(pTemp);
					}
				}
			}
			//------------------------------------------------------

			//更新整点数据
			CTime nowTime(pRecvData->m_RecordTime);
			CTimeSpan ttspan(0,1,0,0);
			CTime HourTime = nowTime + ttspan;
			double HourValue = 0.0;

			strTM.Format("%04d-%02d-%02d %02d:00",
				HourTime.GetYear(),HourTime.GetMonth(),HourTime.GetDay(),
				HourTime.GetHour());

			CString strFTM = strTM + ":00.000";
			SYSTEMTIME FTM;
			CSystemTimeOp::ConvertStringToSysemTime(strFTM,&FTM);
			int comp = CSystemTimeOp::CompareSystemTime(&FTM);

			
		}
	}
#ifdef DEBUG
	GetLocalTime(&debugst);
	TRACE("%s StatisticsTempera_Optimize2_RealTime(end)\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
#endif
}


/*
* 更新 strTM点的数据
* 查找到数据 更新 Update
* 未查找到数据 添加 Insert
* 返回： 1: Insert  2: update  3: exist (not Update)
*/
int  CDataTranSQLServer_SC::UpdateTemperaData( CAddrMap_Item* pAddrItem, CString strTM, double tValue )
{
	//bool ret = true;
	int ret = 0;

	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_TEMP && strTM != "" )
	{
		CString sSQL = "";
		bool isExist = false;       //Record whether exist
		double tValueInDb = 0.0;     //Data In Database

		//查询
			if (m_nDatabaseType == SQLServer_Database)
			{
				sSQL.Format("SELECT T FROM ST_TEMP_R WHERE STCD='%s' AND TM = '%s'",
					pAddrItem->Get_STCD(),strTM);
			}
			else if (m_nDatabaseType == Oracle_Database)
			{
				sSQL.Format("SELECT T FROM ST_TEMP_R WHERE STCD='%s' AND TM = to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3')",
					pAddrItem->Get_STCD(),strTM);
			}


			isExist = GetTemperaData(sSQL,tValueInDb);

			if (false == isExist)     //INSERT
			{
				ret = 1;
				if (m_nDatabaseType == SQLServer_Database)
				{
					sSQL.Format("INSERT INTO ST_TEMP_R (STCD ,TM, T) VALUES ('%s','%s',%.2f)",
						pAddrItem->Get_STCD(),strTM,tValue);
				}
				else if (m_nDatabaseType == Oracle_Database)
				{
					sSQL.Format("INSERT INTO ST_TEMP_R (STCD ,TM, T) VALUES ('%s',to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3'),%.2f)",
						pAddrItem->Get_STCD(),strTM,tValue);
				}

				ret = ExcuteNoQuery(sSQL);
			}
			else if (true == isExist) //UPDATE
			{
				ret = 3;
				//if (ValueInDb != pointValue)
				if (fabs(tValueInDb - tValue) >= 0.01)
				{
					ret = 2;
#ifdef DEBUG
					TRACE("DB:%f\r\nX:%f\r\n",tValueInDb,tValue);
#endif

					if (m_nDatabaseType == SQLServer_Database)
					{
						sSQL.Format("UPDATE ST_TEMP_R SET T = %.2f WHERE STCD='%s' AND TM = '%s'",
							tValue,pAddrItem->Get_STCD(),strTM);
					}
					else if (m_nDatabaseType == Oracle_Database)
					{
						sSQL.Format("UPDATE ST_TEMP_R SET Z = %.2f WHERE STCD='%s' AND TM = to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3')",
							tValue,pAddrItem->Get_STCD(),strTM);
					}
					ret = ExcuteNoQuery(sSQL);
				}
			}
	}
	
	return ret;
}

CTempera_DataItem* CDataTranSQLServer_SC::GetTemperaBasePoint( CAddrMap_Item* pAddrItem, CTempera_DataItem* pRecvData )
{
	CTempera_DataItem* pResult = NULL;
	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_TEMP && pRecvData != NULL)
	{
		CString strBegin= "",strEnd = "";
		strBegin.Format("%04d-%02d-%02d %02d:00:00.000",
			pRecvData->m_RecordTime.wYear,
			pRecvData->m_RecordTime.wMonth,
			pRecvData->m_RecordTime.wDay,
			pRecvData->m_RecordTime.wHour);

		strEnd = CSystemTimeOp::ConvertSystemTimeToString(&(pRecvData->m_RecordTime),true);

		CString sSQL = "";
		if (m_nDatabaseType == SQLServer_Database)
			{
				sSQL.Format("SELECT TOP 1 T,TM FROM ST_TEMP_R WHERE STCD='%s' AND TM BETWEEN '%s' and '%s' ORDER BY TM DESC",pAddrItem->Get_STCD(),strBegin,strEnd);
			}
		else if (m_nDatabaseType == Oracle_Database)
			{
				sSQL.Format("SELECT T,TO_CHAR(TM,'YYYY-MM-DD HH24:MI:SS.ff3') FROM ST_TEMP_R WHERE STCD='%s' AND TM BETWEEN to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3') and to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3') ORDER BY TM DESC",pAddrItem->Get_STCD(),strBegin,strEnd);
		}

		if (sSQL != "")
		{
			try
			{

#ifdef DEBUG
				SYSTEMTIME debugTime;
				GetLocalTime(&debugTime);
				TRACE("%s 查找温度数据,SQL:%s\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugTime,true),sSQL);
#endif
				CRecordset rs(&m_Database);

				rs.Open(AFX_DB_USE_DEFAULT_TYPE,sSQL);

				CString sTemp = "";
				if(! rs.IsEOF())
				{
					double tData= 0.0;
					SYSTEMTIME st;

					rs.GetFieldValue("T",sTemp);
					tData = atof(sTemp);

					rs.GetFieldValue("TM",sTemp);
					CSystemTimeOp::ConvertStringToSysemTime(sTemp,&st);

					pResult = new CTempera_DataItem(tData,st);
#ifdef DEBUG
					GetLocalTime(&debugTime);
					TRACE("%s 查找到温度数据:%s\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugTime,true),pResult->ToString());
#endif
					
				}
				rs.Close();
			}
			catch (CDBException* e)
			{
				CString sLog = "";
				sLog.Format("执行SQL:%s,信息:%s",sSQL,e->m_strError);
				e->Delete();
				AddThreadLog(sLog,Warning_log_Message);
				_DELETE(pResult);

			}

		}
	}
	return pResult;
}


//2023-5-23添加
void CDataTranSQLServer_SC::CFlow_RealTime(CString Rtu_ID, CFlow_DataItem* pRecvData)
{
	if (Rtu_ID != "" && pRecvData != NULL)
	{
		std::list<CAddrMap_Item*>::iterator Itr = m_mapAddressMap.end();
		for (Itr = m_mapAddressMap.begin();Itr != m_mapAddressMap.end();Itr++)
		{
			CAddrMap_Item* pAddrItem = (*Itr);
			if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_FLOW &&pAddrItem->Get_RTU_ID() == Rtu_ID)
			{
				StatisticsFlow_Optimize2_Enter(pAddrItem, pRecvData);
			}
		}
	}
}

void CDataTranSQLServer_SC::StatisticsFlow_Optimize2_Enter(CAddrMap_Item* pAddrItem, CFlow_DataItem* pRecvData)
{
	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_FLOW && pRecvData != NULL)
	{
		CFlow_DataItem* pCopy = new CFlow_DataItem(pRecvData);
		pAddrItem->InsertData(pCopy);

		//计算时间范围
		CString strBeginTime = "", strEndTime = "";
		if (pRecvData->m_RecordTime.wMinute == 0 && pRecvData->m_RecordTime.wSecond == 0)
		{
			strBeginTime.Format("%04d-%02d-%02d %02d:00:01.000",pRecvData->m_RecordTime.wYear, pRecvData->m_RecordTime.wMonth,pRecvData->m_RecordTime.wDay, pRecvData->m_RecordTime.wHour);
			strEndTime.Format("%04d-%02d-%02d %02d:59:59.999",pRecvData->m_RecordTime.wYear, pRecvData->m_RecordTime.wMonth,pRecvData->m_RecordTime.wDay, pRecvData->m_RecordTime.wHour);
		}
		else
		{
			strBeginTime.Format("%04d-%02d-%02d %02d:00:01.000",pRecvData->m_RecordTime.wYear, pRecvData->m_RecordTime.wMonth,pRecvData->m_RecordTime.wDay, pRecvData->m_RecordTime.wHour);

			CTime nowTime(pRecvData->m_RecordTime);
			CTimeSpan ttspan(0, 1, 0, 0);
			CTime HourTime = nowTime + ttspan;
			double HourValue = 0.0;

			strEndTime.Format("%04d-%02d-%02d %02d:59:59.999",HourTime.GetYear(), HourTime.GetMonth(), HourTime.GetDay(),HourTime.GetHour());
		}

		//插入到统计缓存
		StatisticsOnTimer_Item item;
		item.STCD = pAddrItem->Get_STCD();
		item.Type = CZ_Type_FLOW;
		item.WLType = WL_Type_UNKNOWN;
		item.strTM = pRecvData->m_sRecordTime;
		item.strFTM = pRecvData->m_sRecordTime;
		item.stFTM = pRecvData->m_RecordTime;
		item.Cal_Type = Rain_statistics_UNKNOWN;
		item.strBegin = strBeginTime;
		item.strEnd = strEndTime;
		CSystemTimeOp::ConvertStringToSysemTime(strBeginTime, &(item.stBegin));
		CSystemTimeOp::ConvertStringToSysemTime(strEndTime, &(item.stEnd));
		GetLocalTime(&(item.CAL_TM));
		item.Sunflowdata= pRecvData->Sunshiflowdata;
		item.Leiflowdata = pRecvData->Leijiflowdata;
		item.Collect = pRecvData->m_RecordTime;

		pAddrItem->UpdateStatisticsOnTimerItem(&item);
	}
}

std::multimap<CString, CFlow_DataItem*> CDataTranSQLServer_SC::StatisticsFlow_Optimize2_GetFlowDataFromSQLite(CAddrMap_Item* pAddrItem, CString strBegin, CString strEnd)
{
	std::multimap<CString, CFlow_DataItem*> resultDataMap;
	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_FLOW)
	{
		CString strTemp = "";

		CString sSQL = "";
		sSQL.Format("SELECT DISTINCT TOTAL_WATER,ANYTIME_FLOW,COLLECT_TIME FROM FLOWV_DATA WHERE RTU='%s' AND SENSORS_NO='00' AND COLLECT_TIME  >= '%s' AND COLLECT_TIME < '%s' ORDER BY COLLECT_TIME",
			pAddrItem->Get_RTU_ID(), strBegin, strEnd);

		bool blLocked = false;
		if (m_localTrans != NULL)
		{
			m_localTrans->SQLiteCS_Lock();
			blLocked = true;
		}
		CSqlStatement* stmt = ExcuteSQLiteQuery(sSQL);

#ifdef DEBUG
		SYSTEMTIME debugst;
		GetLocalTime(&debugst);
		TRACE("%s StatisticsFlow_Optimize2_GetFlowDataFromSQLite(Begin Read) \r\n", CSystemTimeOp::ConvertSystemTimeToString(&debugst, true));
#endif
		if (stmt != NULL)
		{
			double sunflowdata = 0.0;
			double leijiflowdata = 0.0;
			CString sCollectTime = "", sTemp = "";
			while (stmt->NextRow())
			{
				sTemp = stmt->ValueString(0);
				leijiflowdata = atof(sTemp)/10.0;

				sTemp = stmt->ValueString(1);
				sunflowdata = atof(sTemp)/1000.0;

				sTemp = stmt->ValueString(2);
				sCollectTime = sTemp.Trim();

				SYSTEMTIME collectTime;
				CSystemTimeOp::ConvertStringToSysemTime(sCollectTime, &collectTime);

				CFlow_DataItem* pTempW = new CFlow_DataItem(sunflowdata, leijiflowdata, collectTime);
				if (pTempW != NULL)
					resultDataMap.insert(std::pair<CString, CFlow_DataItem*>(pTempW->m_sRecordTime, pTempW));
			}

			delete stmt;
			stmt = NULL;
		}
		if (m_localTrans != NULL)
		{
			m_localTrans->SQLiteCS_UnLock();
			blLocked = false;
		}

#ifdef DEBUG
		GetLocalTime(&debugst);
		TRACE("%s StatisticsFlow_Optimize2_GetFlowDataFromSQLite(End Read)\r\n", CSystemTimeOp::ConvertSystemTimeToString(&debugst, true));
#endif
	}
	return resultDataMap;
}

std::multimap<CString, CFlow_DataItem*> CDataTranSQLServer_SC::StatisticsFlow_Optimize2_GetFlowDataFromUpDataCache(CAddrMap_Item* pAddrItem, CString strBegin, CString strEnd)
{
	std::multimap<CString, CFlow_DataItem*> resultDataMap;
	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_FLOW)
	{
#ifdef DEBUG
		SYSTEMTIME debugst;
		GetLocalTime(&debugst);
		TRACE("%s StatisticsFlow_Optimize2_GetFlowDataFromUpDataCache(Begin Read) \r\n", CSystemTimeOp::ConvertSystemTimeToString(&debugst, true));
#endif
		SYSTEMTIME begintime, endtime;
		CSystemTimeOp::ConvertStringToSysemTime(strBegin, &begintime);
		CSystemTimeOp::ConvertStringToSysemTime(strEnd, &endtime);
		if (pAddrItem->m_UpDataCache.m_mapFlowData.size() > 0)
		{
			std::multimap<CString, CFlow_DataItem*>::reverse_iterator Itr;
			for (Itr = pAddrItem->m_UpDataCache.m_mapFlowData.rbegin();Itr != pAddrItem->m_UpDataCache.m_mapFlowData.rend();Itr++)
			{
				CFlow_DataItem* pObj = Itr->second;
				if (pObj != NULL)
				{
					if (CSystemTimeOp::CompareSystemTime(&(pObj->m_RecordTime), &begintime) >= 0 && CSystemTimeOp::CompareSystemTime(&(pObj->m_RecordTime), &endtime) <= 0)
					{
						CFlow_DataItem* pCopy = new CFlow_DataItem(pObj);
						resultDataMap.insert(std::pair<CString, CFlow_DataItem*>(pCopy->m_sRecordTime, pCopy));
					}

					if (CSystemTimeOp::CompareSystemTime(&(pObj->m_RecordTime), &begintime) < 0)
						break;
				}
			}
		}

#ifdef DEBUG
		GetLocalTime(&debugst);
		TRACE("%s StatisticsFlow_Optimize2_GetFlowDataFromUpDataCache(End Read)\r\n", CSystemTimeOp::ConvertSystemTimeToString(&debugst, true));
#endif
	}
	return resultDataMap;
}

void CDataTranSQLServer_SC::StatisticsFlow_Optimize2_RealTime(CAddrMap_Item* pAddrItem, CFlow_DataItem* pRecvData, std::multimap<CString, CFlow_DataItem*>& FlowDataMap)
{
#ifdef DEBUG
	SYSTEMTIME debugst;
	GetLocalTime(&debugst);
	TRACE("%s StatisticsFlow_Optimize1_RealTime(begin)\r\n", CSystemTimeOp::ConvertSystemTimeToString(&debugst, true));
#endif

	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_FLOW && pRecvData != NULL)
	{
		//检测是否是整点数据
		if (pRecvData->m_RecordTime.wMinute == 0 &&pRecvData->m_RecordTime.wSecond == 0)
		{
			//更新整点数据
			CString strTM = "";
			strTM.Format("%04d-%02d-%02d %02d:00",pRecvData->m_RecordTime.wYear, pRecvData->m_RecordTime.wMonth,pRecvData->m_RecordTime.wDay, pRecvData->m_RecordTime.wHour);

			//new 20140816
			int ret = 0;
			CString strFTM = strTM + ":00.000";
			SYSTEMTIME FTM;
			CSystemTimeOp::ConvertStringToSysemTime(strFTM, &FTM);
			int comp = CSystemTimeOp::CompareSystemTime(&FTM);
			if (comp <= 0)
			{
				ret = UpdateSunFlowData(pAddrItem, strTM, pRecvData->Sunshiflowdata);
				ret = UpdateLeiFlowData(pAddrItem, strTM, pRecvData->Leijiflowdata);
			}
			else
			{
				Table_ST_RSVR_R_Record LeijiDatarecord;
				Table_ST_RIVER_R_Record SunshiDatarecord;
				SunshiDatarecord.STCD = pAddrItem->Get_STCD();
				LeijiDatarecord.STCD = pAddrItem->Get_STCD();
				//record.TYPE = (int)pAddrItem->GetWlType();
				SunshiDatarecord.TM = strTM;
				LeijiDatarecord.TM = strTM;

				SunshiDatarecord.Q = pRecvData->Sunshiflowdata;
				LeijiDatarecord.W = pRecvData->Leijiflowdata;
				GetLocalTime(&(SunshiDatarecord.CAL_TM));
				GetLocalTime(&(LeijiDatarecord.CAL_TM));

				ret = pAddrItem->UpdateST_RIVER_R_Record(&SunshiDatarecord);
				ret = pAddrItem->UpdateST_RSVR_R_Record(&LeijiDatarecord);
			}

			//后1个小时的加报数据统计
			if (ret == 1 || 2 == ret)
			{
				SYSTEMTIME nexthour_begin, nexthour_end;
				CString temp = strTM + ":01.000";
				CSystemTimeOp::ConvertStringToSysemTime(temp, &nexthour_begin);

				temp.Format("%04d-%02d-%02d %02d:59:59.999", nexthour_begin.wYear, nexthour_begin.wMonth, nexthour_begin.wDay, nexthour_begin.wHour);
				CSystemTimeOp::ConvertStringToSysemTime(temp, &nexthour_end);

#ifdef DEBUG
				GetLocalTime(&debugst);
				TRACE("%s Next Hour: %s - %s \r\n",
					CSystemTimeOp::ConvertSystemTimeToString(&debugst, true),
					CSystemTimeOp::ConvertSystemTimeToString(&nexthour_begin, true),
					CSystemTimeOp::ConvertSystemTimeToString(&nexthour_end, true));
#endif
				//StatisticsTempera_Optimize1_Hour_AppendReport(pAddrItem,nexthour_begin,nexthour_end,WlDataMap);
			}
		}
		else
		{
			//更新当前点
			CString strTM = "";
			strTM.Format("%04d-%02d-%02d %02d:%02d",
				pRecvData->m_RecordTime.wYear, pRecvData->m_RecordTime.wMonth,
				pRecvData->m_RecordTime.wDay, pRecvData->m_RecordTime.wHour, pRecvData->m_RecordTime.wMinute);

			//从接口数据库里获取基准点的数据
			//未找到基准点将不存储数据
			CFlow_DataItem* pWDataSunFlow = GetSunFlowBasePoint(pAddrItem, pRecvData);
			if (pWDataSunFlow != NULL && CUdfGlobalFUNC::CompareDoubleChange(pWDataSunFlow->Sunshiflowdata, pRecvData->Sunshiflowdata, (double)0.05, 9, 3) >= 0)
			{
				//new 20140816
				CString strFTM = strTM + ":00.000";
				SYSTEMTIME FTM;
				CSystemTimeOp::ConvertStringToSysemTime(strFTM, &FTM);
				int comp = CSystemTimeOp::CompareSystemTime(&FTM);
				if (comp <= 0)
					UpdateSunFlowData(pAddrItem, strTM, pRecvData->Sunshiflowdata);
				else
				{
					Table_ST_RIVER_R_Record sunFlowrecord;
					sunFlowrecord.STCD = pAddrItem->Get_STCD();
					//record.TYPE = (int)pAddrItem->GetWlType();
					sunFlowrecord.TM = strTM;
					sunFlowrecord.Q = pRecvData->Sunshiflowdata;
					GetLocalTime(&(sunFlowrecord.CAL_TM));

					pAddrItem->UpdateST_RIVER_R_Record(&sunFlowrecord);
				}
			}

			//从接口数据库里获取基准点的数据
			//未找到基准点将不存储数据
			CFlow_DataItem* pWDataLeijiFlow = GetLeijiFlowBasePoint(pAddrItem, pRecvData);
			if (pWDataLeijiFlow != NULL && CUdfGlobalFUNC::CompareDoubleChange(pWDataLeijiFlow->Leijiflowdata, pRecvData->Leijiflowdata, (double)0.05, 9, 3) >= 0)
			{
				//new 20140816
				CString strFTM = strTM + ":00.000";
				SYSTEMTIME FTM;
				CSystemTimeOp::ConvertStringToSysemTime(strFTM, &FTM);
				int comp = CSystemTimeOp::CompareSystemTime(&FTM);
				if (comp <= 0)
					UpdateLeiFlowData(pAddrItem, strTM, pRecvData->Leijiflowdata);
				else
				{
					Table_ST_RSVR_R_Record LeiFlowrecord;
					LeiFlowrecord.STCD = pAddrItem->Get_STCD();
					//record.TYPE = (int)pAddrItem->GetWlType();
					LeiFlowrecord.TM = strTM;
					LeiFlowrecord.W = pRecvData->Leijiflowdata;
					GetLocalTime(&(LeiFlowrecord.CAL_TM));

					pAddrItem->UpdateST_RSVR_R_Record(&LeiFlowrecord);
				}
			}

			_DELETE(pWDataSunFlow);
			_DELETE(pWDataLeijiFlow);


			//检查是否为补报数据并进行修复
			//对一个小时内的水位数据进行重新统计其加报数据进行修改入库
			//------------------------------------------------------
#ifdef DEBUG
			GetLocalTime(&debugst);
			TRACE("Begin Read: %s \r\n", CSystemTimeOp::ConvertSystemTimeToString(&debugst, true));
#endif
			CList<CFlow_DataItem*> tempList;
			CString  sSQL = "", _strBegin = "", _strEnd = "";
			_strBegin = CSystemTimeOp::ConvertSystemTimeToString(&(pRecvData->m_RecordTime), true);
			_strEnd.Format("%04d-%02d-%02d %02d:59:59.999", pRecvData->m_RecordTime.wYear, pRecvData->m_RecordTime.wMonth, pRecvData->m_RecordTime.wDay, pRecvData->m_RecordTime.wHour);
			SYSTEMTIME _begin_time, _end_time;
			CSystemTimeOp::ConvertStringToSysemTime(_strBegin, &_begin_time);
			CSystemTimeOp::ConvertStringToSysemTime(_strEnd, &_end_time);
			std::multimap<CString, CFlow_DataItem*>::iterator wlItr;
			for (wlItr = FlowDataMap.begin();wlItr != FlowDataMap.end();wlItr++)
			{
				CFlow_DataItem* pTemp = wlItr->second;
				if (pTemp != NULL)
				{
					if (CSystemTimeOp::CompareSystemTime(&(pTemp->m_RecordTime), &_begin_time) > 0 &&
						CSystemTimeOp::CompareSystemTime(&(pTemp->m_RecordTime), &_end_time) < 0)
					{
						CFlow_DataItem* pTempW = new CFlow_DataItem(pTemp);
						if (pTempW != NULL)
							tempList.AddTail(pTempW);
					}
				}
			}
#ifdef DEBUG
			GetLocalTime(&debugst);
			TRACE("End Read: %s \r\n", CSystemTimeOp::ConvertSystemTimeToString(&debugst, true));
#endif

			if (!tempList.IsEmpty())
			{
				if (m_nDatabaseType == SQLServer_Database)
					sSQL.Format("DELETE FROM ST_RIVER_R WHERE TM > '%s' AND TM < '%s' AND STCD='%s'", _strBegin, _strEnd, pAddrItem->Get_STCD());
				ExcuteNoQuery(sSQL);
				if (m_nDatabaseType == SQLServer_Database)
					sSQL.Format("DELETE FROM ST_RSVR_R WHERE TM > '%s' AND TM < '%s' AND STCD='%s'", _strBegin, _strEnd, pAddrItem->Get_STCD());
				ExcuteNoQuery(sSQL);

				//更新补报点（加报水位数据）
				while (tempList.IsEmpty() != TRUE)
				{
					CFlow_DataItem* pTemp = tempList.RemoveHead();
					if (pTemp != NULL)
					{
						CString _tempStr = "";
						_tempStr.Format("%04d-%02d-%02d %02d:%02d",
							pTemp->m_RecordTime.wYear, pTemp->m_RecordTime.wMonth,
							pTemp->m_RecordTime.wDay, pTemp->m_RecordTime.wHour, pTemp->m_RecordTime.wMinute);

						CFlow_DataItem* pBasicSunFlow = GetSunFlowBasePoint(pAddrItem, pTemp);
						if (pBasicSunFlow != NULL && (CUdfGlobalFUNC::CompareDoubleChange(pTemp->Sunshiflowdata, pBasicSunFlow->Sunshiflowdata, (double)0.05, 9, 3) >= 0))
						{
							CString strFTM = _tempStr + ":00.000";
							SYSTEMTIME FTM;
							CSystemTimeOp::ConvertStringToSysemTime(strFTM, &FTM);
							int comp = CSystemTimeOp::CompareSystemTime(&FTM);
							if (comp <= 0)
								UpdateSunFlowData(pAddrItem, _tempStr, pTemp->Sunshiflowdata);
							else
							{
								Table_ST_RIVER_R_Record SunFlowrecord;
								SunFlowrecord.STCD = pAddrItem->Get_STCD();
								//record.TYPE = (int)pAddrItem->GetWlType();
								SunFlowrecord.TM = _tempStr;
								SunFlowrecord.Q = pTemp->Sunshiflowdata;
								GetLocalTime(&(SunFlowrecord.CAL_TM));

								pAddrItem->UpdateST_RIVER_R_Record(&SunFlowrecord);
							}
						}

						CFlow_DataItem* pBasicLeijiFlow = GetSunFlowBasePoint(pAddrItem, pTemp);
						if (pBasicLeijiFlow != NULL && (CUdfGlobalFUNC::CompareDoubleChange(pTemp->Leijiflowdata, pBasicLeijiFlow->Leijiflowdata, (double)0.05, 9, 3) >= 0))
						{
							CString strFTM = _tempStr + ":00.000";
							SYSTEMTIME FTM;
							CSystemTimeOp::ConvertStringToSysemTime(strFTM, &FTM);
							int comp = CSystemTimeOp::CompareSystemTime(&FTM);
							if (comp <= 0)
								UpdateLeiFlowData(pAddrItem, _tempStr, pTemp->Leijiflowdata);
							else
							{
								Table_ST_RSVR_R_Record LeiFlowrecord;
								LeiFlowrecord.STCD = pAddrItem->Get_STCD();
								//record.TYPE = (int)pAddrItem->GetWlType();
								LeiFlowrecord.TM = _tempStr;
								LeiFlowrecord.W = pTemp->Leijiflowdata;
								GetLocalTime(&(LeiFlowrecord.CAL_TM));

								pAddrItem->UpdateST_RSVR_R_Record(&LeiFlowrecord);
							}
						}

						_DELETE(pBasicSunFlow);
						_DELETE(pBasicLeijiFlow);
						_DELETE(pTemp);
					}
				}
			}
			//------------------------------------------------------

			//更新整点数据
			CTime nowTime(pRecvData->m_RecordTime);
			CTimeSpan ttspan(0, 1, 0, 0);
			CTime HourTime = nowTime + ttspan;
			double HourValue = 0.0;

			strTM.Format("%04d-%02d-%02d %02d:00",
				HourTime.GetYear(), HourTime.GetMonth(), HourTime.GetDay(),
				HourTime.GetHour());

			CString strFTM = strTM + ":00.000";
			SYSTEMTIME FTM;
			CSystemTimeOp::ConvertStringToSysemTime(strFTM, &FTM);
			int comp = CSystemTimeOp::CompareSystemTime(&FTM);


		}
	}
#ifdef DEBUG
	GetLocalTime(&debugst);
	TRACE("%s StatisticsTempera_Optimize2_RealTime(end)\r\n", CSystemTimeOp::ConvertSystemTimeToString(&debugst, true));
#endif
}

int CDataTranSQLServer_SC::UpdateSunFlowData(CAddrMap_Item* pAddrItem, CString strTM, double SunflowValue)
{
	//bool ret = true;
	int ret = 0;

	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_FLOW && strTM != "")
	{
		CString sSQL = "";
		bool isExist = false;       //Record whether exist
		double tValueInDb = 0.0;     //Data In Database

		//查询
		if (m_nDatabaseType == SQLServer_Database)
		{
			sSQL.Format("SELECT Q FROM ST_RIVER_R WHERE STCD='%s' AND TM = '%s'",
				pAddrItem->Get_STCD(), strTM);

		}
		isExist = GetFlowData1(sSQL, tValueInDb);

		if (false == isExist)     //INSERT
		{
			ret = 1;
			if (m_nDatabaseType == SQLServer_Database)
			{
				sSQL.Format("INSERT INTO ST_RIVER_R (STCD ,TM, Q) VALUES ('%s','%s',%.3f)",
					pAddrItem->Get_STCD(), strTM, SunflowValue);
			}

			ret = ExcuteNoQuery(sSQL);
		}
		else if (true == isExist) //UPDATE
		{
			ret = 3;
			if (fabs(tValueInDb - SunflowValue) >= 0.01)
			{
				ret = 2;
#ifdef DEBUG
				TRACE("DB:%f\r\nX:%f\r\n", tValueInDb, SunflowValue);
#endif

				if (m_nDatabaseType == SQLServer_Database)
				{
					sSQL.Format("UPDATE ST_RIVER_R SET Q = %.3f WHERE STCD='%s' AND TM = '%s'",
						SunflowValue, pAddrItem->Get_STCD(), strTM);
				}
				ret = ExcuteNoQuery(sSQL);
			}
		}
	}
	return ret;
}

int CDataTranSQLServer_SC::UpdateLeiFlowData(CAddrMap_Item* pAddrItem, CString strTM, double LeiflowValue)
{
	//bool ret = true;
	int ret = 0;

	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_FLOW && strTM != "")
	{
		CString sSQL = "";
		bool isExist = false;       //Record whether exist
		double tValueInDb = 0.0;     //Data In Database

		//查询
		if (m_nDatabaseType == SQLServer_Database)
		{
			sSQL.Format("SELECT W FROM ST_RSVR_R WHERE STCD='%s' AND TM = '%s'",
				pAddrItem->Get_STCD(), strTM);

		}
		isExist = GetFlowData2(sSQL, tValueInDb);

		if (false == isExist)     //INSERT
		{
			ret = 1;
			if (m_nDatabaseType == SQLServer_Database)
			{
				sSQL.Format("INSERT INTO ST_RSVR_R (STCD ,TM, W) VALUES ('%s','%s',%.3f)",
					pAddrItem->Get_STCD(), strTM, LeiflowValue);
			}

			ret = ExcuteNoQuery(sSQL);
		}
		else if (true == isExist) //UPDATE
		{
			ret = 3;
			if (fabs(tValueInDb - LeiflowValue) >= 0.01)
			{
				ret = 2;
#ifdef DEBUG
				TRACE("DB:%f\r\nX:%f\r\n", tValueInDb, LeiflowValue);
#endif

				if (m_nDatabaseType == SQLServer_Database)
				{
					sSQL.Format("UPDATE ST_RSVR_R SET W = %.3f WHERE STCD='%s' AND TM = '%s'",
						LeiflowValue, pAddrItem->Get_STCD(), strTM);
				}
				ret = ExcuteNoQuery(sSQL);
			}
		}
	}
	return ret;
}

bool CDataTranSQLServer_SC::GetFlowData1(CString sSQL, double& SunFlowData)
{
	bool ret = false;
	SunFlowData = 0.0;
	CString sLog = "";
	try
	{

#ifdef DEBUG
		SYSTEMTIME debugTime;
		GetLocalTime(&debugTime);
		TRACE("%s 1 查找瞬时流量数据,SQL:%s\r\n", CSystemTimeOp::ConvertSystemTimeToString(&debugTime, true), sSQL);
#endif
		CRecordset rs(&m_Database);

		rs.Open(AFX_DB_USE_DEFAULT_TYPE, sSQL);

		CString sTemp = "";
		if (!rs.IsEOF())
		{
			rs.GetFieldValue("Q", sTemp);
			SunFlowData = atof(sTemp)/1000.0;

#ifdef DEBUG
			GetLocalTime(&debugTime);
			TRACE("%s 2 查找到瞬时流量数据:x=%f\r\n", CSystemTimeOp::ConvertSystemTimeToString(&debugTime, true), SunFlowData);
#endif
			ret = true;
		}
		rs.Close();
	}
	catch (CDBException* e)
	{
		sLog.Format("执行SQL:%s,信息:%s", sSQL, e->m_strError);
		e->Delete();
		//AddThreadLog(sLog,Error_Log_message);
		AddThreadLog(sLog, Warning_log_Message);
		ret = false;
	}
	return ret;
}

bool CDataTranSQLServer_SC::GetFlowData2(CString sSQL, double& LeiFlowData)
{
	bool ret = false;
	LeiFlowData = 0.0;
	CString sLog = "";
	try
	{

#ifdef DEBUG
		SYSTEMTIME debugTime;
		GetLocalTime(&debugTime);
		TRACE("%s 一 查找累计流量数据,SQL:%s\r\n", CSystemTimeOp::ConvertSystemTimeToString(&debugTime, true), LeiFlowData);
#endif
		CRecordset rs(&m_Database);

		rs.Open(AFX_DB_USE_DEFAULT_TYPE, sSQL);

		CString sTemp = "";
		if (!rs.IsEOF())
		{
			rs.GetFieldValue("W", sTemp);
			LeiFlowData = atof(sTemp)/10.0;

#ifdef DEBUG
			GetLocalTime(&debugTime);
			TRACE("%s 二 查找到累计流量数据:x=%f\r\n", CSystemTimeOp::ConvertSystemTimeToString(&debugTime, true), LeiFlowData);
#endif
			ret = true;
		}
		rs.Close();
	}
	catch (CDBException* e)
	{
		sLog.Format("执行SQL:%s,信息:%s", sSQL, e->m_strError);
		e->Delete();
		//AddThreadLog(sLog,Error_Log_message);
		AddThreadLog(sLog, Warning_log_Message);
		ret = false;
	}
	return ret;
}

CFlow_DataItem* CDataTranSQLServer_SC::GetSunFlowBasePoint(CAddrMap_Item* pAddrItem, CFlow_DataItem* pRecvData)
{
	CFlow_DataItem* pResult = NULL;
	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_FLOW && pRecvData != NULL)
	{
		CString strBegin = "", strEnd = "";
		strBegin.Format("%04d-%02d-%02d %02d:00:00.000",pRecvData->m_RecordTime.wYear,pRecvData->m_RecordTime.wMonth,pRecvData->m_RecordTime.wDay,pRecvData->m_RecordTime.wHour);

		strEnd = CSystemTimeOp::ConvertSystemTimeToString(&(pRecvData->m_RecordTime), true);

		CString sSQL = "";
		if (m_nDatabaseType == SQLServer_Database)
		{
			sSQL.Format("SELECT TOP 1 Q,TM FROM ST_RIVER_R WHERE STCD='%s' AND TM BETWEEN '%s' and '%s' ORDER BY TM DESC", pAddrItem->Get_STCD(), strBegin, strEnd);
		}
		if (sSQL != "")
		{
			try
			{

#ifdef DEBUG
				SYSTEMTIME debugTime;
				GetLocalTime(&debugTime);
				TRACE("%s 一 查找瞬时流量数据,SQL:%s\r\n", CSystemTimeOp::ConvertSystemTimeToString(&debugTime, true), sSQL);
#endif
				CRecordset rs(&m_Database);

				rs.Open(AFX_DB_USE_DEFAULT_TYPE, sSQL);

				CString sTemp = "";
				if (!rs.IsEOF())
				{
					double SunFlowData = 0.0;
					SYSTEMTIME st;

					rs.GetFieldValue("Q", sTemp);
					SunFlowData = atof(sTemp);

					rs.GetFieldValue("TM", sTemp);
					CSystemTimeOp::ConvertStringToSysemTime(sTemp, &st);

					pResult = new CFlow_DataItem(0,SunFlowData, st);
#ifdef DEBUG
					GetLocalTime(&debugTime);
					TRACE("%s 二 查找到瞬时流量数据:%s\r\n", CSystemTimeOp::ConvertSystemTimeToString(&debugTime, true), pResult->ToString());
#endif

				}
				rs.Close();
			}
			catch (CDBException* e)
			{
				CString sLog = "";
				sLog.Format("执行SQL:%s,信息:%s", sSQL, e->m_strError);
				e->Delete();
				AddThreadLog(sLog, Warning_log_Message);
				_DELETE(pResult);

			}

		}
	}
	return pResult;
}

CFlow_DataItem* CDataTranSQLServer_SC::GetLeijiFlowBasePoint(CAddrMap_Item* pAddrItem, CFlow_DataItem* pRecvData)
{
	CFlow_DataItem* pResult = NULL;
	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_FLOW && pRecvData != NULL)
	{
		CString strBegin = "", strEnd = "";
		strBegin.Format("%04d-%02d-%02d %02d:00:00.000", pRecvData->m_RecordTime.wYear, pRecvData->m_RecordTime.wMonth, pRecvData->m_RecordTime.wDay, pRecvData->m_RecordTime.wHour);

		strEnd = CSystemTimeOp::ConvertSystemTimeToString(&(pRecvData->m_RecordTime), true);

		CString sSQL = "";
		if (m_nDatabaseType == SQLServer_Database)
		{
			sSQL.Format("SELECT TOP 1 W,TM FROM ST_RSVR_R WHERE STCD='%s' AND TM BETWEEN '%s' and '%s' ORDER BY TM DESC", pAddrItem->Get_STCD(), strBegin, strEnd);
		}
		if (sSQL != "")
		{
			try
			{

#ifdef DEBUG
				SYSTEMTIME debugTime;
				GetLocalTime(&debugTime);
				TRACE("%s 3 查找累计流量数据,SQL:%s\r\n", CSystemTimeOp::ConvertSystemTimeToString(&debugTime, true), sSQL);
#endif
				CRecordset rs(&m_Database);

				rs.Open(AFX_DB_USE_DEFAULT_TYPE, sSQL);

				CString sTemp = "";
				if (!rs.IsEOF())
				{
					double LeijiFlowData = 0.0;
					SYSTEMTIME st;

					rs.GetFieldValue("W", sTemp);
					LeijiFlowData = atof(sTemp);

					rs.GetFieldValue("TM", sTemp);
					CSystemTimeOp::ConvertStringToSysemTime(sTemp, &st);

					pResult = new CFlow_DataItem(1, LeijiFlowData, st);
#ifdef DEBUG
					GetLocalTime(&debugTime);
					TRACE("%s 4 查找到累积流量数据:%s\r\n", CSystemTimeOp::ConvertSystemTimeToString(&debugTime, true), pResult->ToString());
#endif

				}
				rs.Close();
			}
			catch (CDBException* e)
			{
				CString sLog = "";
				sLog.Format("执行SQL:%s,信息:%s", sSQL, e->m_strError);
				e->Delete();
				AddThreadLog(sLog, Warning_log_Message);
				_DELETE(pResult);

			}

		}
	}
	return pResult;
}

bool CDataTranSQLServer_SC::GetTemperaData( CString sSQL,double& tData)
{
	bool ret = false;
	tData = 0.0;
	CString sLog = "";
	try
	{

#ifdef DEBUG
		SYSTEMTIME debugTime;
		GetLocalTime(&debugTime);
		TRACE("%s 查找温度数据,SQL:%s\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugTime,true),sSQL);
#endif
		CRecordset rs(&m_Database);

		rs.Open(AFX_DB_USE_DEFAULT_TYPE,sSQL);

		CString sTemp = "";
		if(! rs.IsEOF())
		{
			rs.GetFieldValue("T",sTemp);
			tData = atof(sTemp);

#ifdef DEBUG
			GetLocalTime(&debugTime);
			TRACE("%s 查找到温度数据:x=%f\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugTime,true),tData);
#endif
			ret = true;
		}
		rs.Close();
	}
	catch (CDBException* e)
	{
		sLog.Format("执行SQL:%s,信息:%s",sSQL,e->m_strError);
		e->Delete();
		//AddThreadLog(sLog,Error_Log_message);
		AddThreadLog(sLog,Warning_log_Message);
		ret = false;
	}

	return ret;
}

void CDataTranSQLServer_SC::ScStatisticsCheck_Rain( CAddrMap_Item* pAddrItem )
{
	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_RAIN)
	{
		std::list<CString> tempList;

		std::multimap<CString,CRainData_Item*> RainDataMap;  //临时雨量数据缓存
		SYSTEMTIME tempBegin,tempEnd;						 //临时雨量数据的范围
		bool blReaded = false;								 //是否已经读取了临时数据

		CTime AreaBegin(pAddrItem->m_UpDataCache.m_stCacheAreaBegin);
		if (pAddrItem->m_mapStatistics_Rain.size() >0)
		{
			//std::map<CString,StatisticsOnTimer_Item*>::iterator chkItr = pAddrItem->m_mapStatistics_Rain.end();
			//for (chkItr -- ;
			//	 chkItr !=  pAddrItem->m_mapStatistics_Rain.end();
			//	 chkItr --)
			std::map<CString,StatisticsOnTimer_Item*>::reverse_iterator chkItr = pAddrItem->m_mapStatistics_Rain.rbegin();
			for(chkItr = pAddrItem->m_mapStatistics_Rain.rbegin();chkItr != pAddrItem->m_mapStatistics_Rain.rend();chkItr ++)
			{
				StatisticsOnTimer_Item* pObj = chkItr->second;
				if (pObj != NULL)
				{
					CTime now = CTime::GetCurrentTime();
					CTime objTime = CTime(pObj->stFTM);
					//CTimeSpan tSpan = now - objTime; 
					
					//提前统计
					CTimeSpan tSpan = objTime - now; 

					int comp = -1;
					if (pAddrItem->m_UpDataCache.m_mapRainData.size() > 0)
						comp = CSystemTimeOp::CompareSystemTime(&(pAddrItem->m_UpDataCache.m_stRainBeginTime), &pObj->stBegin);
					
					//if (pObj->Cal_Type == Rain_statistics_AM8Point && tSpan.GetTotalSeconds() >= m_RainStcEarly.m_nScStatisticsCheck1dEarly)  //1Day
					if (pObj->Cal_Type == Rain_statistics_AM8Point && tSpan.GetTotalSeconds() <= m_RainStcEarly.m_nScStatisticsCheck1dEarly)  //1Day
					{
						bool blStatistics = false;   //是否可以统计
						std::multimap<CString,CRainData_Item*> CurRainDataMap  = RainDataMap;  

						//Step 1 获取原始雨量数据
						if (blReaded == false || 
							(blReaded == true && 
							 (CSystemTimeOp::CompareSystemTime(&(pObj->stBegin),&tempBegin) < 0 || 
							  CSystemTimeOp::CompareSystemTime(&(pObj->stEnd),&tempEnd)>0)))
						{
							CTime beginTime(pObj->stBegin);
							CTimeSpan tSpan = beginTime-AreaBegin;

							if (tSpan.GetTotalMinutes() > m_nCheck5Minutes_MNT || comp < 0) //在UpdataCache中读取原始数据
							{
								CurRainDataMap = StatisticsRain_Optimize2_GetRainDataFromUpDataCache(pAddrItem,pObj->strBegin,pObj->strEnd);
								blStatistics = true;
							}
							else    
							{
								if (m_blLocalSQliteTransStartFlag == false)   //从本地SQLite读取原始数据
								{
									CurRainDataMap = StatisticsRain_Optimize2_GetRainDataFromSQLite(pAddrItem,pObj->strBegin,pObj->strEnd);
									blStatistics = true;
								}
							}

							if (blStatistics == true) //更新临时缓存
							{
								ClearMultimap(RainDataMap);
								blReaded = true;
								RainDataMap = CurRainDataMap;
								tempBegin  = pObj->stBegin;
								tempEnd = pObj->stEnd;
							}
						}
						else
						{
							if (blReaded == true && CSystemTimeOp::CompareSystemTime(&(pObj->stBegin),&tempBegin) >= 0 &&
								CSystemTimeOp::CompareSystemTime(&(pObj->stEnd),&tempEnd) <= 0)
							{
								blStatistics = true;
							}
						}

						//Step2 进行降雨量统计
						if(true == blStatistics)     
						{
							//Invoke 1天的降雨量统计
							StatisticsRain_Optimize2_1Day(pAddrItem,pObj->stBegin,pObj->stEnd,CurRainDataMap);

							//将已统计分析的等待对象保存至临时链表中
							tempList.push_back(pObj->strTM);
						}
					}
					else if (pObj->Cal_Type == Rain_statistics_1hour && tSpan.GetTotalSeconds() <= m_RainStcEarly.m_nScStatisticsCheck1hEarly) //1Hour
					{
						bool blStatistics = false;   //是否可以统计
						std::multimap<CString,CRainData_Item*> CurRainDataMap  = RainDataMap; 


						//重新计算小时统计的起止时间，考虑早上8点的小时降雨量统计特殊情况
						//在加入等待的链表时已经计算好了

						//Step 1 获取原始雨量数据
						if (blReaded == false || 
							(blReaded == true && 
							(CSystemTimeOp::CompareSystemTime(&(pObj->stBegin),&tempBegin) < 0 || 
							CSystemTimeOp::CompareSystemTime(&(pObj->stEnd),&tempEnd)>0)))
						{
							CTime beginTime(pObj->stBegin);
							CTimeSpan tSpan = beginTime-AreaBegin;
							if (tSpan.GetTotalMinutes() > m_nCheck1Hour_MNT || comp < 0) //在UpdataCache中读取原始数据
							{
								CurRainDataMap = StatisticsRain_Optimize2_GetRainDataFromUpDataCache(pAddrItem,pObj->strBegin,pObj->strEnd);
								blStatistics = true;
							}
							else    
							{
								if (m_blLocalSQliteTransStartFlag == false)   //从本地SQLite读取原始数据
								{
									CurRainDataMap = StatisticsRain_Optimize2_GetRainDataFromSQLite(pAddrItem,pObj->strBegin,pObj->strEnd);
									blStatistics = true;
								}
							}

							if (blStatistics == true)   //更新临时缓存
							{
								ClearMultimap(RainDataMap);
								blReaded = true;
								RainDataMap = CurRainDataMap;
								tempBegin  = pObj->stBegin;
								tempEnd = pObj->stEnd;
							}
						}
						else
						{
							if (blReaded == true && CSystemTimeOp::CompareSystemTime(&(pObj->stBegin),&tempBegin) >= 0 &&
								CSystemTimeOp::CompareSystemTime(&(pObj->stEnd),&tempEnd) <= 0)
							{
								blStatistics = true;
							}
						}

						//Step2 进行降雨量统计
						if(true == blStatistics)     
						{
							//Invoke 1小时的降雨量统计
							StatisticsRain_Optimize2_1Hour(pAddrItem,pObj->stBegin,pObj->stEnd,pObj->stFTM,CurRainDataMap);

							//将已统计分析的等待对象保存至临时链表中
							tempList.push_back(pObj->strTM);
						}
					}
					else if (pObj->Cal_Type == Rain_statistics_15minutes && tSpan.GetTotalSeconds() <= m_RainStcEarly.m_nScStatisticsCheck15mEarly) //15Minutes
					{
						bool blStatistics = false;   //是否可以统计
						std::multimap<CString,CRainData_Item*> CurRainDataMap  = RainDataMap;  

						//Step 1 获取原始雨量数据
						if (blReaded == false || 
							(blReaded == true && 
							(CSystemTimeOp::CompareSystemTime(&(pObj->stBegin),&tempBegin) < 0 || 
							CSystemTimeOp::CompareSystemTime(&(pObj->stEnd),&tempEnd)>0)))
						{
							CTime beginTime(pObj->stBegin);
							CTimeSpan tSpan = beginTime-AreaBegin;
							if (tSpan.GetTotalMinutes() > m_nCheck15Minutes_MNT  || comp < 0) //在UpdataCache中读取原始数据
							{
								CurRainDataMap = StatisticsRain_Optimize2_GetRainDataFromUpDataCache(pAddrItem,pObj->strBegin,pObj->strEnd);
								blStatistics = true;
							}
							else    
							{
								if (m_blLocalSQliteTransStartFlag == false)   //从本地SQLite读取原始数据
								{
									CurRainDataMap = StatisticsRain_Optimize2_GetRainDataFromSQLite(pAddrItem,pObj->strBegin,pObj->strEnd);
									blStatistics = true;
								}
							}

							if (blStatistics == true) //更新临时缓存
							{
								ClearMultimap(RainDataMap);
								blReaded = true;
								RainDataMap = CurRainDataMap;
								tempBegin  = pObj->stBegin;
								tempEnd = pObj->stEnd;
							}
						}
						else
						{
							if (blReaded == true && CSystemTimeOp::CompareSystemTime(&(pObj->stBegin),&tempBegin) >= 0 &&
								CSystemTimeOp::CompareSystemTime(&(pObj->stEnd),&tempEnd) <= 0)
							{
								blStatistics = true;
							}
						}

						//Step2 进行降雨量统计
						if(true == blStatistics)     
						{
							//Invoke 15分钟的降雨量统计
							StatisticsRain_Optimize2_15Minutes(pAddrItem,pObj->stBegin,pObj->stEnd,CurRainDataMap);

							//将已统计分析的等待对象保存至临时链表中
							tempList.push_back(pObj->strTM);
						}
					}
					else if (pObj->Cal_Type == Rain_statistics_5minutes && tSpan.GetTotalSeconds() <= m_RainStcEarly.m_nScStatisticsCheck5mEarly) //5Minutes
					{
						bool blStatistics = false;   //是否可以统计
						std::multimap<CString,CRainData_Item*> CurRainDataMap  = RainDataMap;  

						//Step 1 获取原始雨量数据
						if (blReaded == false || 
							(blReaded == true && 
							(CSystemTimeOp::CompareSystemTime(&(pObj->stBegin),&tempBegin) < 0 || 
							CSystemTimeOp::CompareSystemTime(&(pObj->stEnd),&tempEnd)>0)))
						{
							CTime beginTime(pObj->stBegin);
							CTimeSpan tSpan = beginTime-AreaBegin;
							if (tSpan.GetTotalMinutes() > m_nCheck5Minutes_MNT  || comp < 0) //在UpdataCache中读取原始数据
							{
								CurRainDataMap = StatisticsRain_Optimize2_GetRainDataFromUpDataCache(pAddrItem,pObj->strBegin,pObj->strEnd);
								blStatistics = true;
							}
							else    
							{
								if (m_blLocalSQliteTransStartFlag == false)   //从本地SQLite读取原始数据
								{
									CurRainDataMap = StatisticsRain_Optimize2_GetRainDataFromSQLite(pAddrItem,pObj->strBegin,pObj->strEnd);
									blStatistics = true;
								}
							}

							if (blStatistics == true) //更新临时缓存
							{
								ClearMultimap(RainDataMap);
								blReaded = true;
								RainDataMap = CurRainDataMap;
								tempBegin  = pObj->stBegin;
								tempEnd = pObj->stEnd;
							}
						}
						else
						{
							if (blReaded == true && CSystemTimeOp::CompareSystemTime(&(pObj->stBegin),&tempBegin) >= 0 &&
								CSystemTimeOp::CompareSystemTime(&(pObj->stEnd),&tempEnd) <= 0)
							{
								blStatistics = true;
							}
						}

						//Step2 进行降雨量统计
						if(true == blStatistics)     
						{
							//Invoke 5分钟的降雨量统计
							StatisticsRain_Optimize2_5Minute(pAddrItem,pObj->stBegin,pObj->stEnd,CurRainDataMap);

							//将已统计分析的等待对象保存至临时链表中
							tempList.push_back(pObj->strTM);
						}
					}
				}
			}
		}

		ClearMultimap(RainDataMap);

		//清除已经统计过的雨量等待统计项
		if (tempList.size() >0)
		{
			std::list<CString>::iterator delItr;
			for (delItr = tempList.begin();
				delItr != tempList.end();
				delItr ++)
			{
				pAddrItem->RemoveStatisticsOnTimerItem(CZ_Type_RAIN,(*delItr));
			}
			tempList.clear();
		}
	}
}

void CDataTranSQLServer_SC::ScStatisticsCheck_WaterLevel( CAddrMap_Item* pAddrItem )
{
	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_WL)
	{
		std::list<CString> tempList;
		std::map<CString,StatisticsOnTimer_Item*>::iterator chkItr;
		for (chkItr = pAddrItem->m_mapStatistics_WaterL.begin();
			 chkItr != pAddrItem->m_mapStatistics_WaterL.end();
			 chkItr ++)
		{
			 StatisticsOnTimer_Item* pObj = chkItr->second;
			 if (pObj != NULL)
			 {
				 //int comp = CSystemTimeOp::CompareSystemTime(&(pObj->stBegin),&(pAddrItem->m_UpDataCache.m_stWaterLBeginTime));
				 int comp2 = CSystemTimeOp::CompareSystemTime(&(pObj->stBegin),&(pAddrItem->m_UpDataCache.m_stCacheAreaBegin));  //?????????

				  std::multimap<CString,CWaterL_DataItem*> WLDataMap ;
				  bool blStatistics = false;
				 //if (comp >=0 || comp2 > 0 )
				  if ( comp2 > 0 )
				 {
					 WLDataMap = StatisticsWaterLevel_Optimize2_GetWaterLDataFromUpDataCache(pAddrItem,pObj->strBegin,pObj->strEnd);
					 blStatistics = true;
				 }
				 else //if (comp < 0 && m_blLocalSQliteTransStartFlag == false) //在没有启动数据库事务时才能读本地的数据
				 {
					 if (m_blLocalSQliteTransStartFlag == false)
					 {
						 WLDataMap = StatisticsWaterLevel_Optimize2_GetWaterLDataFromSQLite(pAddrItem,pObj->strBegin,pObj->strEnd);
						 blStatistics = true;
					 }
				 }

				 if (true == blStatistics)
				 {
					 CWaterL_DataItem* pWaterL = new CWaterL_DataItem(pObj->PV,pObj->stFTM,pObj->strFTM);
					 StatisticsWaterLevel_Optimize2_RealTime_SBK(pAddrItem,pWaterL,WLDataMap);
					 _DELETE(pWaterL);

					 //std::multimap<CString,CWaterL_DataItem*>::iterator delItr;
					 //for (delItr = WLDataMap.begin();
						// delItr != WLDataMap.end();
						// delItr ++)
					 //{
						// _DELETE(delItr->second);
					 //}
					 //WLDataMap.clear();

					 ClearMultimap(WLDataMap);

					 tempList.push_back(chkItr->first);
				 }
			 }
		}

		if (tempList.size() >0)
		{
			std::list<CString>::iterator delItr;
			for (delItr = tempList.begin();
				 delItr != tempList.end();
				 delItr ++)
			{
				pAddrItem->RemoveStatisticsOnTimerItem(CZ_Type_WL,(*delItr));
			}
			tempList.clear();
		}
	}
}

void CDataTranSQLServer_SC::ScStatisticsCheck_Displace( CAddrMap_Item* pAddrItem )
{
	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_DISP)
	{
		std::list<CString> tempList;
		std::map<CString,StatisticsOnTimer_Item*>::iterator chkItr;
		for (chkItr = pAddrItem->m_mapStatistics_Displace.begin();
			 chkItr != pAddrItem->m_mapStatistics_Displace.end();
			 chkItr ++)
		{
			 StatisticsOnTimer_Item* pObj = chkItr->second;
			 if (pObj != NULL)
			 {
				 //int comp = CSystemTimeOp::CompareSystemTime(&(pObj->stBegin),&(pAddrItem->m_UpDataCache.m_stDisplaceBeginTime));
				 int comp2 = CSystemTimeOp::CompareSystemTime(&(pObj->stBegin),&(pAddrItem->m_UpDataCache.m_stCacheAreaBegin));  //?????????

				  std::multimap<CString,CDisplace_DataItem*> DataMap ;
				  bool blStatistics = false;
				 //if (comp >=0 || comp2 > 0 )
				  if ( comp2 > 0 )
				 {
					 DataMap = StatisticsDisplace_Optimize2_GetDisplaceDataFromUpDataCache(pAddrItem,pObj->strBegin,pObj->strEnd);
					 blStatistics = true;
				 }
				 else //if (comp < 0 && m_blLocalSQliteTransStartFlag == false) //在没有启动数据库事务时才能读本地的数据
				 {
					 if (m_blLocalSQliteTransStartFlag == false)
					 {
						 DataMap = StatisticsDisplace_Optimize2_GetDisplaceDataFromSQLite(pAddrItem,pObj->strBegin,pObj->strEnd);
						 blStatistics = true;
					 }
				 }

				 if (true == blStatistics)
				 {
					 CDisplace_DataItem* pDisplace = new CDisplace_DataItem(pObj->X,pObj->Y,pObj->stFTM,pObj->strFTM);
					 StatisticsDisplace_Optimize2_RealTime(pAddrItem,pDisplace,DataMap);
					 _DELETE(pDisplace);

					 ClearMultimap(DataMap);

					 tempList.push_back(chkItr->first);
				 }
			 }
		}

		if (tempList.size() >0)
		{
			std::list<CString>::iterator delItr;
			for (delItr = tempList.begin();
				 delItr != tempList.end();
				 delItr ++)
			{
				pAddrItem->RemoveStatisticsOnTimerItem(CZ_Type_DISP,(*delItr));
			}
			tempList.clear();
		}
	}
}

void CDataTranSQLServer_SC::ScStatisticsCheck_Tempera( CAddrMap_Item* pAddrItem )
{
	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_TEMP)
	{
		std::list<CString> tempList;
		std::map<CString,StatisticsOnTimer_Item*>::iterator chkItr;
		for (chkItr = pAddrItem->m_mapStatistics_Tempera.begin();
			 chkItr != pAddrItem->m_mapStatistics_Tempera.end();
			 chkItr ++)
		{
			 StatisticsOnTimer_Item* pObj = chkItr->second;
			 if (pObj != NULL)
			 {
				 //int comp = CSystemTimeOp::CompareSystemTime(&(pObj->stBegin),&(pAddrItem->m_UpDataCache.m_stTemperaBeginTime));
				 int comp2 = CSystemTimeOp::CompareSystemTime(&(pObj->stBegin),&(pAddrItem->m_UpDataCache.m_stCacheAreaBegin));  //?????????

				  std::multimap<CString,CTempera_DataItem*> DataMap ;
				  bool blStatistics = false;
				 //if (comp >=0 || comp2 > 0 )
				  if ( comp2 > 0 )
				 {
					 DataMap = StatisticsTempera_Optimize2_GetTemperaDataFromUpDataCache(pAddrItem,pObj->strBegin,pObj->strEnd);
					 blStatistics = true;
				 }
				 else //if (comp < 0 && m_blLocalSQliteTransStartFlag == false) //在没有启动数据库事务时才能读本地的数据
				 {
					 if (m_blLocalSQliteTransStartFlag == false)
					 {
						 DataMap = StatisticsTempera_Optimize2_GetTemperaDataFromSQLite(pAddrItem,pObj->strBegin,pObj->strEnd);
						 blStatistics = true;
					 }
				 }

				 if (true == blStatistics)
				 {
					 CTempera_DataItem* pTempera = new CTempera_DataItem(pObj->T,pObj->stFTM,pObj->strFTM);
					 StatisticsTempera_Optimize2_RealTime(pAddrItem,pTempera,DataMap);
					 _DELETE(pTempera);

					 ClearMultimap(DataMap);

					 tempList.push_back(chkItr->first);
				 }
			 }
		}

		if (tempList.size() >0)
		{
			std::list<CString>::iterator delItr;
			for (delItr = tempList.begin();
				 delItr != tempList.end();
				 delItr ++)
			{
				pAddrItem->RemoveStatisticsOnTimerItem(CZ_Type_TEMP,(*delItr));
			}
			tempList.clear();
		}
	}
}


std::multimap<CString,CWaterL_DataItem*> CDataTranSQLServer_SC::StatisticsWaterLevel_Optimize2_GetWaterLDataFromSQLite( CAddrMap_Item* pAddrItem,CString strBegin,CString strEnd )
{
	std::multimap<CString,CWaterL_DataItem*> resultWlDataMap;
	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_WL  )
	{
		CString strTemp = "";

		CString sSQL = "";
		sSQL.Format("SELECT DISTINCT CURRENT_VAL,COLLECT_TIME FROM WATERL_DATA WHERE RTU='%s' AND SENSORS_NO='00' AND COLLECT_TIME  >= '%s' AND COLLECT_TIME < '%s' ORDER BY COLLECT_TIME",
			pAddrItem->Get_RTU_ID(),strBegin,strEnd);

		bool blLocked = false;
		if (m_localTrans != NULL)
		{
			m_localTrans->SQLiteCS_Lock();
			blLocked = true;
		}
		CSqlStatement* stmt = ExcuteSQLiteQuery(sSQL);

#ifdef DEBUG
		SYSTEMTIME debugst;
		GetLocalTime(&debugst);
		TRACE("%s StatisticsWaterLevel_Optimize2_GetWaterLDataFromSQLite(Begin Read) \r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
#endif
		if (stmt != NULL)
		{
			double dValue = 0.0;
			CString sCollectTime = "",sTemp="";
			while (stmt->NextRow())
			{
				sTemp= stmt->ValueString(0);
				dValue = atof(sTemp);

				sTemp = stmt->ValueString(1);
				sCollectTime = sTemp.Trim();

				SYSTEMTIME collectTime;
				CSystemTimeOp::ConvertStringToSysemTime(sCollectTime,&collectTime);

				CWaterL_DataItem* pTempW = new CWaterL_DataItem(dValue,collectTime);
				if (pTempW != NULL)
					resultWlDataMap.insert(std::pair<CString,CWaterL_DataItem*>(pTempW->m_sRecordTime,pTempW));
			}

			delete stmt;
			stmt = NULL;
		}
		if (m_localTrans != NULL)
		{
			m_localTrans->SQLiteCS_UnLock();
			blLocked = false;
		}

#ifdef DEBUG
		GetLocalTime(&debugst);
		TRACE("%s StatisticsWaterLevel_Optimize2_GetWaterLDataFromSQLite(End Read)\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
#endif
	}
	return resultWlDataMap;
}

std::multimap<CString,CWaterL_DataItem*> CDataTranSQLServer_SC::StatisticsWaterLevel_Optimize2_GetWaterLDataFromUpDataCache( CAddrMap_Item* pAddrItem,CString strBegin,CString strEnd )
{
	std::multimap<CString,CWaterL_DataItem*> resultWlDataMap;
	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_WL  )
	{
#ifdef DEBUG
		SYSTEMTIME debugst;
		GetLocalTime(&debugst);
		TRACE("%s StatisticsWaterLevel_Optimize2_GetWaterLDataFromUpDataCache(Begin Read) \r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
#endif
		SYSTEMTIME begintime,endtime;
		CSystemTimeOp::ConvertStringToSysemTime(strBegin,&begintime);
		CSystemTimeOp::ConvertStringToSysemTime(strEnd,&endtime);
		if (pAddrItem->m_UpDataCache.m_mapWaterLData.size()>0)
		{
			//std::multimap<CString,CWaterL_DataItem*>::iterator Itr = pAddrItem->m_UpDataCache.m_mapWaterLData.end();
	/*		Itr-- ;
			for (;
				Itr != pAddrItem->m_UpDataCache.m_mapWaterLData.end();
				Itr --)*/
			std::multimap<CString,CWaterL_DataItem*>::reverse_iterator Itr ;
			for(Itr = pAddrItem->m_UpDataCache.m_mapWaterLData.rbegin();
				Itr != pAddrItem->m_UpDataCache.m_mapWaterLData.rend();
				Itr ++)
			{
				CWaterL_DataItem* pObj = Itr->second;
				if (pObj != NULL)
				{
					if (CSystemTimeOp::CompareSystemTime(&(pObj->m_RecordTime),&begintime) >=0 &&
						CSystemTimeOp::CompareSystemTime(&(pObj->m_RecordTime),&endtime) <=0)
					{
						CWaterL_DataItem* pCopy = new CWaterL_DataItem(pObj);
						resultWlDataMap.insert(std::pair<CString,CWaterL_DataItem*>(pCopy->m_sRecordTime,pCopy));
					}

					if (CSystemTimeOp::CompareSystemTime(&(pObj->m_RecordTime),&begintime) < 0)
						break;
				}
			}
		}

#ifdef DEBUG
		GetLocalTime(&debugst);
		TRACE("%s StatisticsWaterLevel_Optimize2_GetWaterLDataFromUpDataCache(End Read)\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true));
#endif
	}
	return resultWlDataMap;
}

void CDataTranSQLServer_SC::StatisticsRain_Optimize2_Enter( CAddrMap_Item* pAddrItem,CRainData_Item* pRecvData )
{
	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_RAIN && pRecvData != NULL)
	{
		//将雨量数据加入到内存缓存中
		CRainData_Item* pCopy = new CRainData_Item(pRecvData);
		pAddrItem->InsertData(pCopy/*pRecvData*/);

		//插入到等待统计对象
		StatisticsOnTimer_Item item;
		item.STCD = pAddrItem->Get_STCD();
		item.Type = CZ_Type_RAIN;
		item.WLType = WL_Type_UNKNOWN;
		GetLocalTime(&(item.CAL_TM));
		item.PV = 0;
		CSystemTimeOp::ResetSystemTime(item.Collect);

		SYSTEMTIME areaBegin,areaEnd;
		CString strTM = "";
		//获取1天降雨量的统计时间范围
		StatisticsRain_Optimize2_1DayTimeArea(pRecvData,areaBegin,areaEnd,strTM);
		item.stBegin = areaBegin;
		item.stEnd = areaEnd;
		item.strTM = strTM;
		item.strFTM = strTM;
		CSystemTimeOp::ConvertStringToSysemTime(strTM,&(item.stFTM)) ;
		item.Cal_Type = Rain_statistics_AM8Point;
		item.strBegin = CSystemTimeOp::ConvertSystemTimeToString(&areaBegin,true);
		item.strEnd = CSystemTimeOp::ConvertSystemTimeToString(&areaEnd,true);
		pAddrItem->UpdateStatisticsOnTimerItem(&item);

		//获取1H降雨量的统计时间范围
		StatisticsRain_Optimize2_1HourTimeArea(pRecvData,areaBegin,areaEnd,strTM);
		item.stBegin = areaBegin;
		item.stEnd = areaEnd;
		item.strTM = strTM;
		item.strFTM = strTM;
		CSystemTimeOp::ConvertStringToSysemTime(strTM,&(item.stFTM)) ;
		item.Cal_Type = Rain_statistics_1hour;
		item.strBegin = CSystemTimeOp::ConvertSystemTimeToString(&areaBegin,true);
		item.strEnd = CSystemTimeOp::ConvertSystemTimeToString(&areaEnd,true);
		pAddrItem->UpdateStatisticsOnTimerItem(&item);

		//获取15分钟降雨量的统计时间范围
		StatisticsRain_Optimize2_15MinutesTimeArea(pRecvData,areaBegin,areaEnd,strTM);
		item.stBegin = areaBegin;
		item.stEnd = areaEnd;
		item.strTM = strTM;
		item.strFTM = strTM;
		CSystemTimeOp::ConvertStringToSysemTime(strTM,&(item.stFTM)) ;
		item.Cal_Type = Rain_statistics_15minutes;
		item.strBegin = CSystemTimeOp::ConvertSystemTimeToString(&areaBegin,true);
		item.strEnd = CSystemTimeOp::ConvertSystemTimeToString(&areaEnd,true);
		pAddrItem->UpdateStatisticsOnTimerItem(&item);

		//获取5分钟降雨量的统计时间范围
		StatisticsRain_Optimize2_5MinutesTimeArea(pRecvData,areaBegin,areaEnd,strTM);
		item.stBegin = areaBegin;
		item.stEnd = areaEnd;
		item.strTM = strTM;
		item.strFTM = strTM;
		CSystemTimeOp::ConvertStringToSysemTime(strTM,&(item.stFTM)) ;
		item.Cal_Type = Rain_statistics_5minutes;
		item.strBegin = CSystemTimeOp::ConvertSystemTimeToString(&areaBegin,true);
		item.strEnd = CSystemTimeOp::ConvertSystemTimeToString(&areaEnd,true);
		pAddrItem->UpdateStatisticsOnTimerItem(&item);

	}
}

void CDataTranSQLServer_SC::StatisticsRain_Optimize2_5MinutesTimeArea( CRainData_Item* pRecvData,SYSTEMTIME& areaBegin,SYSTEMTIME& areaEnd, CString& strTM )
{
#ifdef DEBUG
	SYSTEMTIME debugst;
	CString ID = CUdfGlobalFUNC::newGUID();
	GetLocalTime(&debugst);
	TRACE("%s StatisticsRain_Optimize2_5MinutesTimeArea(%s) begin\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true),ID);
#endif

	if (pRecvData != NULL)
	{
		CString strPoint = "";      //接口数据库TM 字段的值

		SYSTEMTIME colltime= pRecvData->m_tCollectTime;
		SYSTEMTIME _tempsystemtime = colltime;
		_tempsystemtime.wMinute = 0;
		_tempsystemtime.wSecond = 1;
		_tempsystemtime.wMilliseconds = 0;

		WORD mut = colltime.wMinute / 5;
		WORD mod = colltime.wMinute % 5;

		CTime _beginHour_ctime(_tempsystemtime);
		CTimeSpan _t_add_span(0,0,5 * mut,0);
		CTimeSpan _t_add5_span(0,0,5,0);

		SYSTEMTIME begintime,endtime;
		if (mod == 0 && colltime.wSecond == 0)
		{
			CTime _tempctime_end = _beginHour_ctime + _t_add_span;
			CTime _tempctime_begin  = _tempctime_end - _t_add5_span;
			CSystemTimeOp::ConvertCTimeToSystemtime(&_tempctime_end,&endtime);
			CSystemTimeOp::ConvertCTimeToSystemtime(&_tempctime_begin,&begintime);
		}
		else
		{
			CTime _tempctime_begin = _beginHour_ctime + _t_add_span;
			CTime _tempctime_end   = _tempctime_begin + _t_add5_span;
			CSystemTimeOp::ConvertCTimeToSystemtime(&_tempctime_begin,&begintime);
			CSystemTimeOp::ConvertCTimeToSystemtime(&_tempctime_end,&endtime);
		}

		strPoint.Format("%04d-%02d-%02d %02d:%02d:00.005",
			endtime.wYear,endtime.wMonth,endtime.wDay,endtime.wHour,endtime.wMinute);
		areaBegin = begintime;
		strTM = strPoint;


		SYSTEMTIME repair_begintime, repair_endtime;
		CTime basic_ctime(endtime);
		repair_begintime = endtime;
		CTime _temp_ctime = basic_ctime + _t_add5_span;
		CSystemTimeOp::ConvertCTimeToSystemtime(&_temp_ctime,&repair_endtime);

		UINT _t_repair_check = 0;
		CString  str_repair_end = "";
		str_repair_end = CSystemTimeOp::ConvertSystemTimeToString(&repair_endtime,true);

		//修改修复数据算法 2013-02-05
		while(true)
		{
			double  repair_5minute_rain = 0.0;
			str_repair_end = CSystemTimeOp::ConvertSystemTimeToString(&repair_endtime,true);

			//Next calc
			repair_begintime = repair_endtime;
			CTime repair_begin_ctime(repair_begintime);
			CTime repair_end_ctime = repair_begin_ctime + _t_add5_span;
			CTimeSpan _repair_tspan = repair_begin_ctime - basic_ctime;
			_t_repair_check = _repair_tspan.GetTotalMinutes();
			CSystemTimeOp::ConvertCTimeToSystemtime(&repair_end_ctime,&repair_endtime);

			//2013-02-05
			if (_t_repair_check > m_nCheck5Minutes_MNT && 
				(repair_begin_ctime.GetYear() != basic_ctime.GetYear() || 
				repair_begin_ctime.GetMonth() != basic_ctime.GetMonth() ||
				repair_begin_ctime.GetDay() != basic_ctime.GetDay() ||
				repair_begin_ctime.GetHour() !=  basic_ctime.GetHour()))
				break;
		}

		CSystemTimeOp::ConvertStringToSysemTime(str_repair_end,&areaEnd);
	}

#ifdef DEBUG
	GetLocalTime(&debugst);
	STRUCT_BEGINEND_AREA TotalArea;
	TotalArea.stBegin = areaBegin;
	TotalArea.stEnd = areaEnd;
	TRACE("%s StatisticsRain_Optimize2_5MinutesTimeArea(%s) end(%s) %s\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true),ID,TotalArea.ToString(),strTM);
#endif

}

void CDataTranSQLServer_SC::StatisticsRain_Optimize2_15MinutesTimeArea( CRainData_Item* pRecvData,SYSTEMTIME& areaBegin,SYSTEMTIME& areaEnd,CString& strTM )
{
#ifdef DEBUG
	SYSTEMTIME debugst;
	CString ID = CUdfGlobalFUNC::newGUID();
	GetLocalTime(&debugst);
	TRACE("%s StatisticsRain_Optimize2_15MinutesTimeArea(%s) begin\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true),ID);
#endif

	if (pRecvData != NULL)
	{
		CString strPoint = "";     

		SYSTEMTIME collectTime = pRecvData->m_tCollectTime;
		SYSTEMTIME _tempsystemtime = collectTime;
		_tempsystemtime.wMinute = 0;
		_tempsystemtime.wSecond = 1;
		_tempsystemtime.wMilliseconds = 0;

		WORD mut = collectTime.wMinute / 15;
		WORD mod = collectTime.wMinute % 15;

		CTime _beginhour(_tempsystemtime);
		CTimeSpan _tspan_add(0,0,15 * mut,0);
		CTimeSpan _tspan_15(0,0,15,0);

		SYSTEMTIME begintime,endtime;
		if (mod == 0 && collectTime.wSecond == 0)
		{
			CTime _end_ctime = _beginhour + _tspan_add;
			CTime _begin_ctime = _end_ctime - _tspan_15;
			CSystemTimeOp::ConvertCTimeToSystemtime(&_begin_ctime,&begintime);
			CSystemTimeOp::ConvertCTimeToSystemtime(&_end_ctime,&endtime);
		}
		else
		{
			CTime _begin_ctime = _beginhour + _tspan_add;
			CTime _end_ctime = _begin_ctime + _tspan_15;
			CSystemTimeOp::ConvertCTimeToSystemtime(&_begin_ctime,&begintime);
			CSystemTimeOp::ConvertCTimeToSystemtime(&_end_ctime,&endtime);
		}

		strPoint.Format("%04d-%02d-%02d %02d:%02d:00.015",
			endtime.wYear,endtime.wMonth,endtime.wDay,endtime.wHour,endtime.wMinute); //特殊用途
		areaBegin  = begintime;
		strTM = strPoint;

		SYSTEMTIME repair_begintime, repair_endtime;
		CTime basic_ctime(endtime);
		repair_begintime = endtime;
		CTime _temp_ctime = basic_ctime + _tspan_15;
		CSystemTimeOp::ConvertCTimeToSystemtime(&_temp_ctime,&repair_endtime);

		CString str_repair_end = "";
		str_repair_end = CSystemTimeOp::ConvertSystemTimeToString(&repair_endtime,true);

		UINT _t_repair_check = 0;
		while(true)
		{
			double  repair_15minute_rain = 0.0;
			str_repair_end = CSystemTimeOp::ConvertSystemTimeToString(&repair_endtime,true);

			//Next Calc
			repair_begintime		= repair_endtime;
			CTime repair_begin_ctime(repair_begintime);
			CTime repair_end_ctime	= repair_begin_ctime + _tspan_15;
			CTimeSpan _repair_tspan = repair_begin_ctime - basic_ctime;
			_t_repair_check			= _repair_tspan.GetTotalMinutes();
			CSystemTimeOp::ConvertCTimeToSystemtime(&repair_end_ctime,&repair_endtime);

			if (_t_repair_check > m_nCheck15Minutes_MNT &&
				(basic_ctime.GetYear() != repair_begin_ctime.GetYear() ||
				basic_ctime.GetMonth() != repair_begin_ctime.GetMonth() || 
				basic_ctime.GetDay() != repair_begin_ctime.GetDay() ||
				basic_ctime.GetHour() != repair_begin_ctime.GetHour()) )
				break;
		}

		CSystemTimeOp::ConvertStringToSysemTime(str_repair_end,&areaEnd);
	}

#ifdef DEBUG
	GetLocalTime(&debugst);
	STRUCT_BEGINEND_AREA TotalArea;
	TotalArea.stBegin = areaBegin;
	TotalArea.stEnd = areaEnd;
	TRACE("%s StatisticsRain_Optimize2_15MinutesTimeArea(%s) end(%s) %s\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true),ID,TotalArea.ToString(),strTM);
#endif

}

void CDataTranSQLServer_SC::StatisticsRain_Optimize2_1HourTimeArea( CRainData_Item* pRecvData,SYSTEMTIME& areaBegin,SYSTEMTIME& areaEnd,CString& strTM )
{

#ifdef DEBUG
	SYSTEMTIME debugst;
	CString ID = CUdfGlobalFUNC::newGUID();
	GetLocalTime(&debugst);
	TRACE("%s StatisticsRain_Optimize2_1HourTimeArea(%s) begin\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true),ID);
#endif

	if (pRecvData != NULL)
	{
		CString strPoint = "";      //接口数据库TM 字段的值

		SYSTEMTIME collectTime = pRecvData->m_tCollectTime;
		SYSTEMTIME _tempsystemtime = collectTime;
		_tempsystemtime.wMinute = 0;
		_tempsystemtime.wSecond = 1;
		_tempsystemtime.wMilliseconds = 0;

		CTime _beginhour(_tempsystemtime);
		CTimeSpan _tspan_1hour(0,1,0,0);

		SYSTEMTIME begintime,endtime;

		if (collectTime.wMinute == 0 &&
			collectTime.wSecond == 0)      //整点数据
		{
			endtime = _tempsystemtime;
			CTime _temp_begin_ctime = _beginhour - _tspan_1hour;
			CSystemTimeOp::ConvertCTimeToSystemtime(&_temp_begin_ctime,&begintime);
		}
		else									
		{
			begintime = _tempsystemtime;
			CTime _temp_end_ctime = _beginhour + _tspan_1hour;
			CSystemTimeOp::ConvertCTimeToSystemtime(&_temp_end_ctime,&endtime);
		}

		strPoint.Format("%04d-%02d-%02d %02d:00:00.060",
			endtime.wYear,endtime.wMonth,endtime.wDay,endtime.wHour);  ////特殊用途

		//判断是否为早上8点
		if (endtime.wHour == 8)
		{
			CTime curendCTime(endtime);
			CTimeSpan tempspan(1,0,0,0);
			CTime curbeginCTime = curendCTime - tempspan;
			
			SYSTEMTIME temp_time;
			CSystemTimeOp::ConvertCTimeToSystemtime(&curbeginCTime,&temp_time);
			if (CSystemTimeOp::CompareSystemTime(&temp_time,&begintime) < 0)
				begintime = temp_time;
		}

		areaBegin = begintime;
		strTM = strPoint;

		SYSTEMTIME repair_begin_time,repair_end_time;
		CTime      basic_ctime(endtime);
		repair_begin_time = endtime;

		CTime      _temp_repair_end_ctime = basic_ctime + _tspan_1hour;
		CSystemTimeOp::ConvertCTimeToSystemtime(&_temp_repair_end_ctime,&repair_end_time);

		CString str_repair_end = "";
		str_repair_end = CSystemTimeOp::ConvertSystemTimeToString(&repair_end_time,true);

		UINT repair_time_check = 0;
		while (repair_time_check <= m_nCheck1Hour_MNT)
		{
			str_repair_end = CSystemTimeOp::ConvertSystemTimeToString(&repair_end_time,true);


			//判断是否为早上8点
			if (repair_end_time.wHour == 8)
			{
				CTime curendCTime(repair_end_time);
				CTimeSpan tempspan(1,0,0,0);
				CTime curbeginCTime = curendCTime - tempspan;

				SYSTEMTIME temp_time;
				CSystemTimeOp::ConvertCTimeToSystemtime(&curbeginCTime,&temp_time);
				if (CSystemTimeOp::CompareSystemTime(&temp_time,&begintime) < 0)
					begintime = temp_time;
				areaBegin = begintime;
			}


			//next calc
			repair_begin_time = repair_end_time;
			CTime repair_begin_ctime(repair_begin_time);
			CTime repare_end_ctime = repair_begin_ctime + _tspan_1hour;
			CSystemTimeOp::ConvertCTimeToSystemtime(&repare_end_ctime,&repair_end_time);
			CTimeSpan repair_tspan = repair_begin_ctime - basic_ctime;
			repair_time_check = repair_tspan.GetTotalMinutes();
		}

		CSystemTimeOp::ConvertStringToSysemTime(str_repair_end,&areaEnd);
	}
	
#ifdef DEBUG
	GetLocalTime(&debugst);
	STRUCT_BEGINEND_AREA TotalArea;
	TotalArea.stBegin = areaBegin;
	TotalArea.stEnd = areaEnd;
	TRACE("%s StatisticsRain_Optimize2_1HourTimeArea(%s) end(%s) %s\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true),ID,TotalArea.ToString(),strTM);
#endif

}

void CDataTranSQLServer_SC::StatisticsRain_Optimize2_1DayTimeArea( CRainData_Item* pRecvData,SYSTEMTIME& areaBegin,SYSTEMTIME& areaEnd,CString& strTM )
{
	STRUCT_BEGINEND_AREA TotalArea; 
#ifdef DEBUG
	SYSTEMTIME debugst;
	CString ID = CUdfGlobalFUNC::newGUID();
	GetLocalTime(&debugst);
	TRACE("%s StatisticsRain_Optimize2_1DayTimeArea(%s) begin\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true),ID);
#endif

	if (pRecvData != NULL)
	{
		CString strBegin="",strEnd="",strPoint="";
		CTimeSpan tspan(1,0,0,0);
		SYSTEMTIME collecttime = pRecvData->m_tCollectTime;
		CTime ctimeCur(collecttime);

		if ( (collecttime.wHour == 8 && collecttime.wMinute == 0 && collecttime.wSecond >= 1) ||
			(collecttime.wHour == 8 && collecttime.wMinute != 0) ||
			(collecttime.wHour >8 && collecttime.wHour <= 23))
		{
			strBegin.Format("%04d-%02d-%02d 08:00:01.000",collecttime.wYear,collecttime.wMonth,collecttime.wDay);
			CTime t = ctimeCur + tspan;
			strEnd.Format("%04d-%02d-%02d 08:00:01.000",t.GetYear(),t.GetMonth(),t.GetDay());
			strPoint.Format("%04d-%02d-%02d 08:00:00.100",t.GetYear(),t.GetMonth(),t.GetDay()); //特殊用途 
		}
		else if ((collecttime.wHour == 8 && collecttime.wMinute == 0 && collecttime.wSecond == 0) ||
			(collecttime.wHour >= 0 && collecttime.wHour < 8 ))
		{
			CTime t = ctimeCur - tspan;
			strBegin.Format("%04d-%02d-%02d 08:00:01.000",t.GetYear(),t.GetMonth(),t.GetDay());
			strEnd.Format("%04d-%02d-%02d 08:00:01.000",collecttime.wYear,collecttime.wMonth,collecttime.wDay);
			strPoint.Format("%04d-%02d-%02d 08:00:00.100",collecttime.wYear,collecttime.wMonth,collecttime.wDay);//特殊用途
		}

		SYSTEMTIME begintime,endtime;
		CSystemTimeOp::ConvertStringToSysemTime(strBegin,&begintime);
		CSystemTimeOp::ConvertStringToSysemTime(strEnd,&endtime);

		TotalArea.blSucc = true;
		TotalArea.stBegin = begintime;

		CTime basic_ctime(begintime);
		UINT  m_check_time = 0;
		while (m_check_time <= m_nChcek1Day_MNT + tspan.GetTotalMinutes())
		{
			strEnd   = CSystemTimeOp::ConvertSystemTimeToString(&endtime,true);

			begintime = endtime;
			CTime temp_ctime(begintime);
			CTime tempend_ctime = temp_ctime + tspan;
			CSystemTimeOp::ConvertCTimeToSystemtime(&tempend_ctime,&endtime);
			CTimeSpan mus_tspan = temp_ctime - basic_ctime;
			m_check_time = mus_tspan.GetTotalMinutes();
		}

		CSystemTimeOp::ConvertStringToSysemTime(strEnd,&endtime);
		TotalArea.stEnd = endtime;
		strTM = strPoint;
	}

	areaBegin = TotalArea.stBegin;
	areaEnd = TotalArea.stEnd;

#ifdef DEBUG
	GetLocalTime(&debugst);
	TRACE("%s StatisticsRain_Optimize2_1DayTimeArea(%s) end(%s) %s\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true),ID,TotalArea.ToString(),strTM);
#endif

	//return TotalArea;
}

std::multimap<CString,CRainData_Item*> CDataTranSQLServer_SC::StatisticsRain_Optimize2_GetRainDataFromSQLite( CAddrMap_Item* pAddrItem,CString strBegin,CString StrEnd )
{
#ifdef DEBUG
	SYSTEMTIME _debug_systemtime;
#endif

	std::multimap<CString,CRainData_Item*> resultRainMap;
	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_RAIN)
	{
		SYSTEMTIME beginTime ;
		CSystemTimeOp::ConvertStringToSysemTime(strBegin,&beginTime);

		CTime bgCTime(beginTime);
		CTimeSpan tspan(1,15,0,0);
		CTime beginCTime = bgCTime - tspan;
		CSystemTimeOp::ConvertCTimeToSystemtime(&beginCTime,&beginTime);

		CString strBegin="";
		strBegin = CSystemTimeOp::ConvertSystemTimeToString(&beginTime,true);

		CString sSQL = "";
		sSQL.Format("SELECT DISTINCT DATA_TYPE,CURRENT_VAL,HALFH_VAL,TOTAL_VAL,COLLECT_TIME FROM RAIN_DATA WHERE RTU='%s' AND SENSORS_NO='00' AND COLLECT_TIME >= '%s' AND COLLECT_TIME < '%s' ORDER BY COLLECT_TIME",
			pAddrItem->Get_RTU_ID(),strBegin,StrEnd);

		bool blLocked = false;
		if (m_localTrans!= NULL)
		{
			m_localTrans->SQLiteCS_Lock();
			blLocked = true;
		}
		CSqlStatement* sqlite_rs = ExcuteSQLiteQuery(sSQL);

#ifdef DEBUG
		GetLocalTime(&_debug_systemtime);
		TRACE("%s Read Begin(StatisticsRain_Optimize2_GetRainDataFromSQLite)\r\n", CSystemTimeOp::ConvertSystemTimeToString(&_debug_systemtime,true));
#endif
		if (sqlite_rs != NULL)
		{
			CRainData_Item* pBefore = NULL;
			bool blFinded = false;
			while(sqlite_rs->NextRow())
			{
				CString sTemp = "";
				BYTE DataType =0;
				double CurrentValue =0.0,HalfHourValue = 0.0,TotalValue =0.0;
				CString CollTimeString = "";

				sTemp = sqlite_rs->ValueString(0);
				DataType = (BYTE)atoi(sTemp);

				sTemp = sqlite_rs->ValueString(1);
				CurrentValue = atof(sTemp);

				sTemp = sqlite_rs->ValueString(2);
				HalfHourValue = atof(sTemp);

				sTemp = sqlite_rs->ValueString(3);
				TotalValue =atof(sTemp);

				sTemp = sqlite_rs->ValueString(4);
				CollTimeString = sTemp;

				CRainData_Item* pRain =new CRainData_Item(DataType,CurrentValue,HalfHourValue,TotalValue,CollTimeString);

				//目的:取最接近开始时间之前的数据，不用读取过多之前的数据，避免后续统计遍历次数
				if (blFinded == false && CSystemTimeOp::CompareSystemTime(&(pRain->m_tCollectTime), &beginTime) >= 0 )
				{
					blFinded = true;
					if (pBefore != NULL)
						resultRainMap.insert(std::pair<CString,CRainData_Item*>(pBefore->CollectTimeString(),pBefore));

					resultRainMap.insert(std::pair<CString,CRainData_Item*>(pRain->CollectTimeString(),pRain));
				}
				else if (blFinded == false && CSystemTimeOp::CompareSystemTime(&(pRain->m_tCollectTime), &beginTime) <0 )
				{
					_DELETE(pBefore);
					pBefore = pRain;
				}
				else if (blFinded == true)
				{
					resultRainMap.insert(std::pair<CString,CRainData_Item*>(pRain->CollectTimeString(),pRain));
				}
			}
		}
		_DELETE(sqlite_rs);
		if (m_localTrans!= NULL)
		{
			m_localTrans->SQLiteCS_UnLock();
			blLocked = false;
		}

#ifdef DEBUG
		GetLocalTime(&_debug_systemtime);
		TRACE("%s end Begin(StatisticsRain_Optimize2_GetRainDataFromSQLite)\r\n", CSystemTimeOp::ConvertSystemTimeToString(&_debug_systemtime,true));
#endif

	}
	return resultRainMap;
}

std::multimap<CString,CRainData_Item*> CDataTranSQLServer_SC::StatisticsRain_Optimize2_GetRainDataFromUpDataCache( CAddrMap_Item* pAddrItem,CString strBegin,CString strEnd )
{
	std::multimap<CString,CRainData_Item*> RainDataMap;
	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_RAIN)
	{
		
		if (pAddrItem->m_UpDataCache.m_mapRainData.size() >0)
		{

            SYSTEMTIME begintime,endtime;
			CSystemTimeOp::ConvertStringToSysemTime(strBegin,&begintime);
			CSystemTimeOp::ConvertStringToSysemTime(strEnd,&endtime);
		
			std::multimap<CString,CRainData_Item*>::reverse_iterator rItr;// = pAddrItem->m_UpDataCache.m_mapRainData.rbegin();
			for (rItr = pAddrItem->m_UpDataCache.m_mapRainData.rbegin();
				 rItr != pAddrItem->m_UpDataCache.m_mapRainData.rend();
				 rItr ++)
			{
				CRainData_Item* pTemp = rItr->second;
				if (pTemp != NULL)
				{
					if (CSystemTimeOp::CompareSystemTime(&(pTemp->m_tCollectTime), &begintime) >=0 &&
						CSystemTimeOp::CompareSystemTime(&(pTemp->m_tCollectTime),&endtime) < 0)
					{
						CRainData_Item* pCopy = new CRainData_Item(pTemp);
						RainDataMap.insert(std::pair<CString,CRainData_Item*>(pCopy->CollectTimeString(),pCopy));
					}

					if (CSystemTimeOp::CompareSystemTime(&(pTemp->m_tCollectTime), &begintime) < 0)
					{
						CRainData_Item* pCopy = new CRainData_Item(pTemp);
						RainDataMap.insert(std::pair<CString,CRainData_Item*>(pCopy->CollectTimeString(),pCopy));
						break;
					}
				}
			}
		}
	}
	return RainDataMap;
}

void CDataTranSQLServer_SC::ClearMultimap( std::multimap<CString,CRainData_Item*>& dataMap )
{
	if (dataMap.size() > 0)
	{
		std::multimap<CString,CRainData_Item*>::iterator Itr;
		for (Itr = dataMap.begin();
			 Itr != dataMap.end();
			 Itr ++)
		{
			_DELETE(Itr->second);
		}
		dataMap.clear();
	}
}

void CDataTranSQLServer_SC::ClearMultimap( std::multimap<CString,CWaterL_DataItem*>& dataMap )
{
	if (dataMap.size() >0)
	{
		std::multimap<CString,CWaterL_DataItem*>::iterator Itr;
		for (Itr = dataMap.begin();
			 Itr != dataMap.end();
			 Itr ++)
		{
			_DELETE(Itr->second);
		}
		dataMap.clear();
	}
}

void CDataTranSQLServer_SC::ClearMultimap( std::multimap<CString,CDisplace_DataItem*>& dataMap )
{
	if (dataMap.size() >0)
	{
		std::multimap<CString,CDisplace_DataItem*>::iterator Itr;
		for (Itr = dataMap.begin();
			 Itr != dataMap.end();
			 Itr ++)
		{
			_DELETE(Itr->second);
		}
		dataMap.clear();
	}
}

void CDataTranSQLServer_SC::ClearMultimap( std::multimap<CString,CTempera_DataItem*>& dataMap )
{
	if (dataMap.size() >0)
	{
		std::multimap<CString,CTempera_DataItem*>::iterator Itr;
		for (Itr = dataMap.begin();
			 Itr != dataMap.end();
			 Itr ++)
		{
			_DELETE(Itr->second);
		}
		dataMap.clear();
	}
}

//2023-5-23
void CDataTranSQLServer_SC::ClearMultimap(std::multimap<CString, CFlow_DataItem*>& dataMap)
{
	if (dataMap.size() > 0)
	{
		std::multimap<CString, CFlow_DataItem*>::iterator Itr;
		for (Itr = dataMap.begin();Itr != dataMap.end();Itr++)
		{
			_DELETE(Itr->second);
		}
		dataMap.clear();
	}
}


void CDataTranSQLServer_SC::StatisticsRain_Optimize2_1Day( CAddrMap_Item* pAddrItem,SYSTEMTIME stbegin,SYSTEMTIME stend,std::multimap<CString,CRainData_Item*>& RainDataMap )
{

#ifdef DEBUG
	SYSTEMTIME debugst;
	CString ID = CUdfGlobalFUNC::newGUID();
	GetLocalTime(&debugst);
	TRACE("%s StatisticsRain_Optimize2_1Day(%s) begin\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true),ID);
#endif

	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_RAIN )
	{
		CString strBegin="",strEnd="",strPoint = "";
		CTime ctimeBegin(stbegin);
		CTimeSpan tspan(1,0,0,0);
		CTime ctimeEnd = ctimeBegin + tspan;

		strBegin=CSystemTimeOp::ConvertSystemTimeToString(&stbegin,true);
		strEnd.Format("%04d-%02d-%02d 08:00:01.000",ctimeEnd.GetYear(),ctimeEnd.GetMonth(),ctimeEnd.GetDay()); 
		strPoint.Format("%04d-%02d-%02d 08:00",ctimeEnd.GetYear(),ctimeEnd.GetMonth(),ctimeEnd.GetDay());


		SYSTEMTIME begintime,endtime;
		CSystemTimeOp::ConvertStringToSysemTime(strBegin,&begintime);
		CSystemTimeOp::ConvertStringToSysemTime(strEnd,&endtime);

		CTime basic_ctime(begintime);
		UINT  m_check_time = 0;

		while (m_check_time <= m_nChcek1Day_MNT + tspan.GetTotalMinutes())
		{
			strBegin = CSystemTimeOp::ConvertSystemTimeToString(&begintime,true);
			strEnd   = CSystemTimeOp::ConvertSystemTimeToString(&endtime,true);
			strPoint.Format("%04d-%02d-%02d 08:00",endtime.wYear,endtime.wMonth,endtime.wDay);

			//new
			if (StatisticsRain_Optimize1_CompareEndTime(begintime,RainDataMap) > 0)
				break;

			double day_rain = 0;
			bool  isNullDay = false;
			day_rain = CalRainData_Optimize1_OnTimeRange(pAddrItem,strBegin,strEnd,Rain_statistics_AM8Point,isNullDay,RainDataMap);

			if (false == isNullDay)
			{
				if (day_rain <=0.0 || day_rain > m_dCHECK_RAIN_DAY)
					day_rain = 0.0;

				CString strHourBegin="",strHourEnd = "";
				strHourEnd = strEnd;
				strHourBegin.Format("%04d-%02d-%02d 07:00:01.000",endtime.wYear,endtime.wMonth,endtime.wDay);

				double hour_rain = 0.0;
				bool isNullHour = false;
				hour_rain = CalRainData_Optimize1_OnTimeRange(pAddrItem,strHourBegin,strHourEnd,Rain_statistics_1hour,isNullHour,RainDataMap);

				if (false == isNullHour)
				{
					if (hour_rain <= 0.0 || hour_rain > m_dCHECK_RAIN_PER)
						hour_rain = 0.0f;

					int HourRainMinutes = 0;
					if (hour_rain >= (double)0.1)
					{
						if (m_RainPRDFlag != 0)
						{
							HourRainMinutes = GetRainMinutes_Optimize1(pAddrItem,strHourBegin,strHourEnd,RainDataMap);
							if (HourRainMinutes <= 0 || HourRainMinutes > 60)
								HourRainMinutes = 60;	
						}
					}

					Table_ST_PPTN_R_Record srecord;
					srecord.STCD = pAddrItem->Get_STCD();
					srecord.TM	 = strPoint;
					srecord.DYP  = day_rain;
					if (hour_rain >= 0.1)
					{
						srecord.DRP = hour_rain;
						srecord.INTV = 1.00;
						srecord.WTH = "7";

						if (m_RainPRDFlag != 0)
						{
							if (HourRainMinutes >= 60 || HourRainMinutes <= 0 )
								srecord.PRD = 0.00;
							else
								srecord.PRD = (double)HourRainMinutes /100;
						}
						else
						{
							srecord.PRD = 0.00;
						}

					}
					else if (hour_rain < (double)0.1)
						srecord.WTH = "8";

					//new 20140816
					srecord.TYPE = Rain_statistics_AM8Point;
					CString strFTM = strPoint + ":00.000";
					CSystemTimeOp::ConvertStringToSysemTime(strFTM,&(srecord.FTM));
					int comp = CSystemTimeOp::CompareSystemTime(&(srecord.FTM));
					if (comp <= 0)
						UpdateRainfallTableRecord(srecord,Rain_statistics_AM8Point);
					else
						pAddrItem->UpdateST_PPTN_R_Record(&srecord);

				}
				else if (true == isNullHour)
				{
					Table_ST_PPTN_R_Record srecord;
					srecord.STCD = pAddrItem->Get_STCD();
					srecord.TM	 = strPoint;
					srecord.DYP  = day_rain;
					srecord.WTH = "8";

					//new 20140816
					srecord.TYPE = Rain_statistics_AM8Point;
					CString strFTM = strPoint + ":00.000";
					CSystemTimeOp::ConvertStringToSysemTime(strFTM,&(srecord.FTM));
					int comp = CSystemTimeOp::CompareSystemTime(&(srecord.FTM));
					if (comp <= 0)
						UpdateRainfallTableRecord(srecord,Rain_statistics_AM8Point);
					else
						pAddrItem->UpdateST_PPTN_R_Record(&srecord);

				}
			}

			//next row
			begintime = endtime;
			CTime temp_ctime(begintime);
			CTime tempend_ctime = temp_ctime + tspan;
			CSystemTimeOp::ConvertCTimeToSystemtime(&tempend_ctime,&endtime);
			CTimeSpan mus_tspan = temp_ctime - basic_ctime;
			m_check_time = mus_tspan.GetTotalMinutes();
		}
	}

#ifdef DEBUG
	GetLocalTime(&debugst);
	TRACE("%s StatisticsRain_Optimize1_1Day(%s) end\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true),ID);
#endif

}

void CDataTranSQLServer_SC::StatisticsRain_Optimize2_1Hour( CAddrMap_Item* pAddrItem,SYSTEMTIME stbegin,SYSTEMTIME stend,SYSTEMTIME stpoint, std::multimap<CString,CRainData_Item*>& RainDataMap )
{
#ifdef DEBUG
	SYSTEMTIME debugst;
	CString ID = CUdfGlobalFUNC::newGUID();
	GetLocalTime(&debugst);
	TRACE("%s StatisticsRain_Optimize2_1Hour(%s) begin\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true),ID);
#endif

	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_RAIN)
	{
		CString strBegin = "",strEnd="",strPoint = "";


		SYSTEMTIME tempst= stpoint;
		tempst.wSecond = 1;
		tempst.wMilliseconds = 0;
		CTime ctimeEnd(tempst);
		CTimeSpan tspan(0,1,0,0);
		CTime ctimeBegin = ctimeEnd - tspan;
		SYSTEMTIME begintime,endtime;
		endtime = tempst;
		CSystemTimeOp::ConvertCTimeToSystemtime(&ctimeBegin,&begintime);

	
		//CTime ctimeBegin(stbegin);
		//CTimeSpan tspan(0,1,0,0);

		//CTime ctimeEnd = ctimeBegin + tspan;
		//strEnd.Format("%04d-%02d-%02d %02d:00:01.000",ctimeEnd.GetYear(),ctimeEnd.GetMonth(),ctimeEnd.GetDay(),ctimeEnd.GetHour());

		//SYSTEMTIME begintime,endtime;
		//begintime = stbegin;
		//CSystemTimeOp::ConvertStringToSysemTime(strEnd,&endtime);

		CTime basic_ctime(begintime);
		UINT m_check_time = 0;
		
		while(m_check_time <= m_nCheck1Hour_MNT + tspan.GetTotalMinutes())
		{
			strBegin = CSystemTimeOp::ConvertSystemTimeToString(&begintime,true);
			strEnd = CSystemTimeOp::ConvertSystemTimeToString(&endtime,true);
			strPoint.Format("%04d-%02d-%02d %02d:00",endtime.wYear,endtime.wMonth,endtime.wDay,endtime.wHour);

			if (StatisticsRain_Optimize1_CompareEndTime(begintime,RainDataMap) > 0)
				break;

			//统计1小时的降雨量
			double Minus1Hour = 0.0; //小时降雨量
			bool   isHourNull = false;
			Minus1Hour = CalRainData_Optimize1_OnTimeRange(pAddrItem,strBegin,strEnd,Rain_statistics_1hour,isHourNull,RainDataMap);

			//calc rain minutes 
			if (false == isHourNull)
			{
				if (Minus1Hour >  m_dCHECK_RAIN_PER ||
					Minus1Hour <= 0.0)
					Minus1Hour = 0.0f;

				int RainMinutes = 0;
				if (Minus1Hour >= (double)0.1)
				{
					if (m_RainPRDFlag != 0)
					{
						RainMinutes = GetRainMinutes_Optimize1(pAddrItem,strBegin,strEnd,RainDataMap);

						if (RainMinutes <= 0 || RainMinutes > 60)
							RainMinutes = 60;	
					}
				}

				//判断是否8点,如果是8点时,需要添加8点数据(日统计、平安报数据)
				if (endtime.wHour != 8)
				{
					if (Minus1Hour >= (double)0.1)
					{
						Table_ST_PPTN_R_Record sRecord;

						sRecord.STCD = pAddrItem->Get_STCD();
						sRecord.TM = strPoint;
						sRecord.DRP = Minus1Hour;
						sRecord.INTV = 1.00;
						sRecord.WTH = "7";

						if (m_RainPRDFlag != 0)
						{
							if (RainMinutes >= 60 || RainMinutes <= 0 )
								sRecord.PRD = 0.00;
							else
								sRecord.PRD = (double)RainMinutes /100;
						}
						else
						{
							sRecord.PRD = 0.00;
						}

						//new 20140816
						sRecord.TYPE = Rain_statistics_1hour;
						CString strFTM = strPoint + ":00.000";
						CSystemTimeOp::ConvertStringToSysemTime(strFTM,&(sRecord.FTM));
						int comp = CSystemTimeOp::CompareSystemTime(&(sRecord.FTM));
						if (comp <= 0)
							UpdateRainfallTableRecord(sRecord,Rain_statistics_1hour);
						else
							pAddrItem->UpdateST_PPTN_R_Record(&sRecord);
					}
					else
					{
						CString sSQL = "";
						if (SQLServer_Database == m_nDatabaseType)
						{
							sSQL.Format("DELETE FROM ST_PPTN_R WHERE STCD='%s' AND TM='%s' AND INTV=1.00",
								pAddrItem->Get_STCD(),
								strPoint);
						}
						else if (Oracle_Database == m_nDatabaseType)
						{
							sSQL.Format("DELETE FROM ST_PPTN_R WHERE STCD='%s' AND TM=to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3') AND INTV=1.00",
								pAddrItem->Get_STCD(),
								strPoint);
						}

						ExcuteNoQuery(sSQL);

						//new 20140816
						pAddrItem->RemoveST_PPTN_R_Record(strPoint);
					}
				}
				else if (endtime.wHour == 8)
				{
					//calc AM8点数据
					CTime ctimeEnd(endtime);
					CTimeSpan _tspan (1,0,0,0);
					CTime ctime1dayBegin= ctimeEnd - _tspan;

					CString str1DayBegin ="";
					str1DayBegin.Format("%04d-%02d-%02d 08:00:01.000",ctime1dayBegin.GetYear(),ctime1dayBegin.GetMonth(),ctime1dayBegin.GetDay());

					double Rain1Day = 0.0f;
					bool   isNullDay = false;
					Rain1Day = CalRainData_Optimize1_OnTimeRange(pAddrItem,str1DayBegin,strEnd,Rain_statistics_AM8Point,isNullDay,RainDataMap);

					//遇到大数据时
					if (Rain1Day > m_dCHECK_RAIN_DAY)
						Rain1Day =0.0f;

					Table_ST_PPTN_R_Record srecord;
					srecord.STCD = pAddrItem->Get_STCD();
					srecord.TM	 = strPoint;
					srecord.DYP  = Rain1Day;
					if (Minus1Hour >= 0.1)
					{
						srecord.DRP = Minus1Hour;
						srecord.INTV = 1.00;
						srecord.WTH = "7";
						if (m_RainPRDFlag != 0)
						{
							if (RainMinutes >= 60 || RainMinutes <= 0 )
								srecord.PRD = 0.00;
							else
								srecord.PRD = (double)RainMinutes /100;
						}
						else 
						{
							srecord.PRD = 0.00;
						}

					}
					else if (Minus1Hour < (double)0.1)
						srecord.WTH = "8";

					//new 20140816
					srecord.TYPE = Rain_statistics_AM8Point;
					CString strFTM = strPoint + ":00.000";
					CSystemTimeOp::ConvertStringToSysemTime(strFTM,&(srecord.FTM));
					int comp = CSystemTimeOp::CompareSystemTime(&(srecord.FTM));
					if (comp <= 0 )
						UpdateRainfallTableRecord(srecord,Rain_statistics_AM8Point);
					else
						pAddrItem->UpdateST_PPTN_R_Record(&srecord);
				}
			}


			//next row
			begintime = endtime;
			CTime temp_ctime(begintime);
			CTime tempend_ctime = temp_ctime + tspan;
			CSystemTimeOp::ConvertCTimeToSystemtime(&tempend_ctime,&endtime);
			CTimeSpan mus_tspan = temp_ctime - basic_ctime;
			m_check_time = mus_tspan.GetTotalMinutes();
		}
	}

#ifdef DEBUG
	GetLocalTime(&debugst);
	TRACE("%s StatisticsRain_Optimize2_1Hour(%s) end\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true),ID);
#endif

}

void CDataTranSQLServer_SC::StatisticsRain_Optimize2_15Minutes( CAddrMap_Item* pAddrItem,SYSTEMTIME stbegin,SYSTEMTIME stend,std::multimap<CString,CRainData_Item*>& RainDataMap )
{
#ifdef DEBUG
	SYSTEMTIME debugst;
	CString ID = CUdfGlobalFUNC::newGUID();
	GetLocalTime(&debugst);
	TRACE("%s StatisticsRain_Optimize2_15Minutes(%s) begin\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true),ID);
#endif

	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_RAIN)
	{
		CString strBegin = "",strEnd="",strPoint = "";
		CTime ctimeBegin(stbegin);
		CTimeSpan tspan(0,0,15,0);

		//strBegin = CSystemTimeOp::ConvertSystemTimeToString(&stbegin,true);
		CTime ctimeEnd = ctimeBegin + tspan;
		strEnd.Format("%04d-%02d-%02d %02d:%02d:01.000",ctimeEnd.GetYear(),ctimeEnd.GetMonth(),ctimeEnd.GetDay(),ctimeEnd.GetHour(),ctimeEnd.GetMinute());

		SYSTEMTIME begintime,endtime;
		begintime = stbegin;
		CSystemTimeOp::ConvertStringToSysemTime(strEnd,&endtime);

		CTime basic_ctime(begintime);
		UINT m_check_time = 0;

		while(m_check_time <= m_nCheck15Minutes_MNT + tspan.GetTotalMinutes())
		{
			strBegin = CSystemTimeOp::ConvertSystemTimeToString(&begintime,true);
			strEnd = CSystemTimeOp::ConvertSystemTimeToString(&endtime,true);
			strPoint.Format("%04d-%02d-%02d %02d:%02d",endtime.wYear,endtime.wMonth,endtime.wDay,endtime.wHour,endtime.wMinute);

			if (StatisticsRain_Optimize1_CompareEndTime(begintime,RainDataMap) > 0)
				break;

			//统计15分钟的降雨量
			double Minus15Minutes = 0.0;  //15分钟降雨量
			bool   isNull = false;
			Minus15Minutes = CalRainData_Optimize1_OnTimeRange(pAddrItem,strBegin,strEnd,Rain_statistics_15minutes,isNull,RainDataMap);


			if (Minus15Minutes > m_dCHECK_RAIN_PER ||
				Minus15Minutes < 5.0)
				Minus15Minutes = 0.0;

			if (false == isNull && Minus15Minutes >= (double)5.0)
			{
				Table_ST_STORM_R_Record sRecord;

				sRecord.STCD	= pAddrItem->Get_STCD();
				sRecord.TM		= strPoint;
				sRecord.STRMP	= Minus15Minutes;

				sRecord.STRMDR	= 0.15;
				sRecord.WTH		= "7";

				//new 20140816
				CString strFTM = strPoint + ":00.000";
				CSystemTimeOp::ConvertStringToSysemTime(strFTM,&(sRecord.FTM));
				int comp = CSystemTimeOp::CompareSystemTime(&(sRecord.FTM));
				if (comp <= 0)
					UpdateRainstormTableRecord(sRecord);
				else
					pAddrItem->UpdateST_STROM_R_Record(&sRecord);
			}
			else // delete
			{
				CString strSQL ="";
				if (SQLServer_Database == m_nDatabaseType)
				{
					strSQL.Format("DELETE FROM ST_STORM_R WHERE STCD='%s' AND TM='%s' AND STRMDR = 0.15",
						pAddrItem->Get_STCD(),strPoint);
				}
				else if (Oracle_Database == m_nDatabaseType)
				{
					strSQL.Format("DELETE FROM ST_STORM_R WHERE STCD='%s' AND TM=to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3') AND STRMDR = 0.15",
						pAddrItem->Get_STCD(),strPoint);
				}
				ExcuteNoQuery(strSQL);

				//new 20140816
				pAddrItem->RemoveST_STROM_R_Record(strPoint);
			}


			//next row
			begintime = endtime;
			CTime temp_ctime(begintime);
			CTime tempend_ctime = temp_ctime + tspan;
			CSystemTimeOp::ConvertCTimeToSystemtime(&tempend_ctime,&endtime);
			CTimeSpan mus_tspan = temp_ctime - basic_ctime;
			m_check_time = mus_tspan.GetTotalMinutes();
		}
	}

#ifdef DEBUG
	GetLocalTime(&debugst);
	TRACE("%s StatisticsRain_Optimize2_15Minutes(%s) end\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true),ID);
#endif

}

void CDataTranSQLServer_SC::StatisticsRain_Optimize2_5Minute( CAddrMap_Item* pAddrItem,SYSTEMTIME stbegin,SYSTEMTIME stend,std::multimap<CString,CRainData_Item*>& RainDataMap )
{
#ifdef DEBUG
	SYSTEMTIME debugst;
	CString ID = CUdfGlobalFUNC::newGUID();
	GetLocalTime(&debugst);
	TRACE("%s StatisticsRain_Optimize2_5Minute(%s) begin\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true),ID);
#endif
	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_RAIN)
	{
		CString strBegin = "",strEnd="",strPoint = "";
		CTime ctimeBegin(stbegin);
		CTimeSpan tspan(0,0,5,0);

		//strBegin = CSystemTimeOp::ConvertSystemTimeToString(&stbegin,true);
		CTime ctimeEnd = ctimeBegin + tspan;
		strEnd.Format("%04d-%02d-%02d %02d:%02d:01.000",ctimeEnd.GetYear(),ctimeEnd.GetMonth(),ctimeEnd.GetDay(),ctimeEnd.GetHour(),ctimeEnd.GetMinute());

		SYSTEMTIME begintime,endtime;
		begintime = stbegin;
		CSystemTimeOp::ConvertStringToSysemTime(strEnd,&endtime);

		CTime basic_ctime(begintime);
		UINT m_check_time = 0;

		while(m_check_time <= m_nCheck5Minutes_MNT + tspan.GetTotalMinutes())
		{
			strBegin = CSystemTimeOp::ConvertSystemTimeToString(&begintime,true);
			strEnd = CSystemTimeOp::ConvertSystemTimeToString(&endtime,true);
			strPoint.Format("%04d-%02d-%02d %02d:%02d:00.005",endtime.wYear,endtime.wMonth,endtime.wDay,endtime.wHour,endtime.wMinute);

			if (StatisticsRain_Optimize1_CompareEndTime(begintime,RainDataMap) > 0)
				break;

			//统计5分钟的降雨量

			double Minus5Minutes = 0.0;  //5分钟降雨量
			bool   isNull = false;
			Minus5Minutes = CalRainData_Optimize1_OnTimeRange(pAddrItem,strBegin,strEnd,Rain_statistics_5minutes,isNull,RainDataMap);

			if (Minus5Minutes > m_dCHECK_RAIN_PER ||
				Minus5Minutes <= 0.0)
				Minus5Minutes = 0.0;

			if (false == isNull  && Minus5Minutes >= (double)0.1)
			{
				Table_ST_PPTN_R_Record data_record;
				data_record.STCD = pAddrItem->Get_STCD();
				data_record.TM   = strPoint;
				data_record.DRP  = Minus5Minutes;

				//其他字段的信息不需要
				data_record.INTV = 0.05;
				data_record.PRD  = 0.05;
				data_record.DYP  = 0.00;
				data_record.WTH  = "7";

				//new 20140816
				data_record.TYPE = Rain_statistics_5minutes;
				CSystemTimeOp::ConvertStringToSysemTime(strPoint,&(data_record.FTM));
				int comp = CSystemTimeOp::CompareSystemTime(&(data_record.FTM));
				if (comp <= 0)
					UpdateRainfallTableRecord(data_record,Rain_statistics_5minutes);
				else
					pAddrItem->UpdateST_PPTN_R_Record(&data_record);
			}
			else 
			{
				CString strSQL = "";
				if (SQLServer_Database == m_nDatabaseType)
				{
					strSQL.Format("DELETE FROM ST_PPTN_R WHERE STCD='%s' AND TM='%s' AND INTV=0.05",pAddrItem->Get_STCD(),strPoint);
				}
				else if (Oracle_Database == m_nDatabaseType)
				{
					strSQL.Format("DELETE FROM ST_PPTN_R WHERE STCD='%s' AND TM=to_timestamp('%s','yyyy-MM-dd HH24:MI:ss.ff3') AND INTV=0.05",pAddrItem->Get_STCD(),strPoint);
				}
				ExcuteNoQuery(strSQL);

				//new 20140816
				pAddrItem->RemoveST_PPTN_R_Record(strPoint);
			}

			//next row
			begintime = endtime;
			CTime temp_ctime(begintime);
			CTime tempend_ctime = temp_ctime + tspan;
			CSystemTimeOp::ConvertCTimeToSystemtime(&tempend_ctime,&endtime);
			CTimeSpan mus_tspan = temp_ctime - basic_ctime;
			m_check_time = mus_tspan.GetTotalMinutes();
		}
	}

#ifdef DEBUG
	GetLocalTime(&debugst);
	TRACE("%s StatisticsRain_Optimize2_5Minute(%s) end\r\n",CSystemTimeOp::ConvertSystemTimeToString(&debugst,true),ID);
#endif
}

void CDataTranSQLServer_SC::SetLoclTransSQLite( CDataTranSQLite_Local* pLocal )
{
	this->m_localTrans = pLocal;
}


CAddrMap_Item::CAddrMap_Item()
{
	m_RTU_ID = "";
	m_STCD = "";
	m_Type = CZ_Type_RAIN;
	m_wlType = WL_Type_HLSW;

	m_addVCode = "";
	m_RtuAddr = 0;
	m_RtuNo = 0;

	m_blCalc = false;
	m_blCalcHalf = false;

	m_nCalcIndex = 0;
}

CAddrMap_Item::CAddrMap_Item( CString stCd, int Type, int wlType, CString AddVCode, int RTUAddr, int RTUNo)
{
	m_STCD = stCd;

	m_addVCode = AddVCode;
	m_RtuAddr = RTUAddr;
	m_RtuNo = RTUNo;

	m_RTU_ID.Format("%s_%08d",AddVCode,RTUAddr);
	m_Type = (CZ_Type_Enum)Type;
	m_wlType = (WL_Type_Enum)wlType;
	
	m_blCalc = false;
	m_blCalcHalf = false;

	m_nCalcIndex = 0;
}

//析构函数调用
void CAddrMap_Item::Clear()
{
	if (m_mapST_PPTN_R.size() >0)
	{
		std::map<CString,Table_ST_PPTN_R_Record*>::iterator itr = m_mapST_PPTN_R.begin();
		for (;itr != m_mapST_PPTN_R.end(); itr ++)
		{
			_DELETE(itr->second);
		}
		m_mapST_PPTN_R.clear();
	}

	if(m_mapST_STROM_R.size() > 0)
	{
		std::map<CString,Table_ST_STORM_R_Record*>::iterator itr = m_mapST_STROM_R.begin();
		for (;itr != m_mapST_STROM_R.end(); itr ++)
		{
			_DELETE(itr->second);
		}
		m_mapST_STROM_R.clear();
	}

	if (m_mapST_WL_R.size() > 0)
	{
		std::map<CString,Table_ST_WL_R_Record*>::iterator itr = m_mapST_WL_R.begin();
		for (;itr != m_mapST_WL_R.end(); itr ++)
		{
			_DELETE(itr->second);
		}
		m_mapST_WL_R.clear();
	}

	if (m_mapStatistics_Rain.size() >0)
	{
		std::map<CString,StatisticsOnTimer_Item*>::iterator itr = m_mapStatistics_Rain.begin();
		for(;itr != m_mapStatistics_Rain.end(); itr ++)
			_DELETE(itr->second);
		m_mapStatistics_Rain.clear();
	}

	if (m_mapStatistics_WaterL.size() >0)
	{
		std::map<CString,StatisticsOnTimer_Item*>::iterator itr = m_mapStatistics_WaterL.begin();
		for(; itr != m_mapStatistics_WaterL.end(); itr ++)
			_DELETE(itr->second);
		m_mapStatistics_WaterL.clear();
	}
}

bool CAddrMap_Item::UpdateST_PPTN_R_Record(Table_ST_PPTN_R_Record* pUpdateR )
{
	bool ret = true;
	if (pUpdateR != NULL &&
		pUpdateR->STCD == this->m_STCD &&
		this->GetType() == CZ_Type_RAIN)
	{
		//find
		std::map<CString,Table_ST_PPTN_R_Record*>::iterator findItr =  m_mapST_PPTN_R.find(pUpdateR->TM);

		if (findItr != m_mapST_PPTN_R.end()) //found
		{
			//update
			GetLocalTime(&(findItr->second->CAL_TM));
			findItr->second->DRP = pUpdateR->DRP;
			findItr->second->INTV = pUpdateR->INTV;
			findItr->second->PRD = pUpdateR->PRD;
			findItr->second->DYP = pUpdateR->DYP;
			findItr->second->WTH = pUpdateR->WTH;
		}
		else //not found
		{
			//insert
			Table_ST_PPTN_R_Record* pCopy = new Table_ST_PPTN_R_Record(pUpdateR);
			GetLocalTime(&(pCopy->CAL_TM));
			m_mapST_PPTN_R.insert(std::pair<CString, Table_ST_PPTN_R_Record*>(pCopy->TM,pCopy));
		}
	}

	return ret;
}

bool CAddrMap_Item::RemoveST_PPTN_R_Record( CString strTM )
{
	bool ret = true;
	std::map<CString,Table_ST_PPTN_R_Record*>::iterator itr = m_mapST_PPTN_R.find(strTM);
	if (itr != m_mapST_PPTN_R.end())
	{
		_DELETE(itr->second);
		m_mapST_PPTN_R.erase(strTM);
	}

	return ret;
}

//2023/5/19以后
//更新ST_RIVER_R表的数据
bool CAddrMap_Item::UpdateST_RIVER_R_Record(Table_ST_RIVER_R_Record* pUpdateR) {
	bool ret = true;
	if (pUpdateR != NULL && pUpdateR->STCD == this->m_STCD && this->GetType() == CZ_Type_FLOW)
	{
		//find
		std::map<CString, Table_ST_RIVER_R_Record*>::iterator findItr = m_mapST_RIVER_R.find(pUpdateR->TM);

		if (findItr != m_mapST_RIVER_R.end()) //found
		{
			//update
			GetLocalTime(&(findItr->second->CAL_TM));
			findItr->second->STCD = this->m_STCD;
			findItr->second->Q = pUpdateR->Q;
			findItr->second->TM = pUpdateR->TM;

		}
		else //not found
		{
			//insert
			Table_ST_RIVER_R_Record* pCopy = new Table_ST_RIVER_R_Record(pUpdateR);
			GetLocalTime(&(pCopy->CAL_TM));
			m_mapST_RIVER_R.insert(std::pair<CString, Table_ST_RIVER_R_Record*>(pCopy->TM, pCopy));
		}
	}
	return ret;
}

//移除ST_RIVER_R表的数据
bool CAddrMap_Item::RemoveST_RIVER_R_Record(CString strTM)
{
	bool ret = true;
	std::map<CString, Table_ST_RIVER_R_Record*>::iterator itr = m_mapST_RIVER_R.find(strTM);
	if (itr != m_mapST_RIVER_R.end())
	{
		_DELETE(itr->second);
		m_mapST_RIVER_R.erase(strTM);
	}
	return ret;
}
//更新ST_RSVR_R表的数据
bool CAddrMap_Item::UpdateST_RSVR_R_Record(Table_ST_RSVR_R_Record* pUpdateR) {
	bool ret = true;
	if (pUpdateR != NULL && pUpdateR->STCD == this->m_STCD && this->GetType() == CZ_Type_FLOW)
	{
		//find
		std::map<CString, Table_ST_RSVR_R_Record*>::iterator findItr = m_mapST_RSVR_R.find(pUpdateR->TM);

		if (findItr != m_mapST_RSVR_R.end()) //found
		{
			//update
			GetLocalTime(&(findItr->second->CAL_TM));
			findItr->second->STCD = this->m_STCD;
			findItr->second->W = pUpdateR->W;
			findItr->second->TM = pUpdateR->TM;
		}
		else //not found
		{
			//insert
			Table_ST_RSVR_R_Record* pCopy = new Table_ST_RSVR_R_Record(pUpdateR);
			GetLocalTime(&(pCopy->CAL_TM));
			m_mapST_RSVR_R.insert(std::pair<CString, Table_ST_RSVR_R_Record*>(pCopy->TM, pCopy));
		}
	}
	return ret;
}

//移除ST_RSVR_R的数据
bool CAddrMap_Item::RemoveST_RSVR_R_Record(CString strTM)
{
	bool ret = true;
	std::map<CString, Table_ST_RSVR_R_Record*>::iterator itr = m_mapST_RSVR_R.find(strTM);
	if (itr != m_mapST_RSVR_R.end())
	{
		_DELETE(itr->second);
		m_mapST_RSVR_R.erase(strTM);
	}
	return ret;
}


bool CAddrMap_Item::UpdateST_STROM_R_Record( Table_ST_STORM_R_Record* pUpdateR )
{
	bool ret = true;
	if (pUpdateR != NULL &&
		pUpdateR->STCD == this->m_STCD &&
		this->GetType() == CZ_Type_RAIN)
	{
		//find
		std::map<CString,Table_ST_STORM_R_Record*>::iterator findItr = m_mapST_STROM_R.find(pUpdateR->TM);
		
		if (findItr != m_mapST_STROM_R.end()) //found
		{
			//update
			GetLocalTime(&(findItr->second->CAL_TM));
			findItr->second->STRMDR = pUpdateR->STRMDR;
			findItr->second->STRMP = pUpdateR->STRMP;
			findItr->second->WTH = pUpdateR->WTH;
		}
		else   //not found
		{
			//insert
			Table_ST_STORM_R_Record* pCopy = new Table_ST_STORM_R_Record(pUpdateR);
			GetLocalTime(&(pCopy->CAL_TM));
			m_mapST_STROM_R.insert(std::pair<CString, Table_ST_STORM_R_Record*>(pCopy->TM,pCopy));
		}
	}
	return ret;
}


bool CAddrMap_Item::RemoveST_STROM_R_Record( CString strTM )
{
	bool ret = true;
	std::map<CString,Table_ST_STORM_R_Record*>::iterator itr = m_mapST_STROM_R.find(strTM);
	if (itr != m_mapST_STROM_R.end())
	{
		_DELETE(itr->second);
		m_mapST_STROM_R.erase(strTM);
	}
	return ret;
}

/*
* 函数功能：更新统计缓存中的水位数据
* 返回值：
* 1 insert
* 2 Update
* 3 exist(no update，值相同)
*/
//bool CAddrMap_Item::UpdateST_WL_R_Record( Table_ST_WL_R_Record* pUpdateR )
int CAddrMap_Item::UpdateST_WL_R_Record( Table_ST_WL_R_Record* pUpdateR )
{
	//bool ret = true;
	int  ret = 0;

	if (pUpdateR != NULL &&
		pUpdateR->STCD == m_STCD &&
		this->GetType() == CZ_Type_WL && 
		this->GetWlType() == (WL_Type_Enum)(pUpdateR->TYPE))
	{
		//find
		std::map<CString,Table_ST_WL_R_Record*>::iterator findItr = m_mapST_WL_R.find(pUpdateR->TM);

		if (findItr != m_mapST_WL_R.end()) //found
		{
			ret = 3;
			//update
			GetLocalTime(&(findItr->second->CAL_TM));
			if(fabs(findItr->second->PV- pUpdateR->PV) >= 0.01)
			{
				findItr->second->PV = pUpdateR->PV;
				ret = 2;
			}
		}
		else  //nat found
		{
			Table_ST_WL_R_Record* pCopy = new Table_ST_WL_R_Record(pUpdateR);
			GetLocalTime(&(pCopy->CAL_TM));
			m_mapST_WL_R.insert(std::pair<CString, Table_ST_WL_R_Record*>(pCopy->TM,pCopy));
			ret = 1;
		}
	}

	return ret;
}

bool CAddrMap_Item::RemoveST_WL_R_Record( CString strTM )
{
	bool ret = true;
	std::map<CString,Table_ST_WL_R_Record*>::iterator itr = m_mapST_WL_R.find(strTM);
	if (itr != m_mapST_WL_R.end())
	{
		_DELETE(itr->second);
		m_mapST_WL_R.erase(strTM);
	}
	return ret;
}

bool CAddrMap_Item::RemoveST_WL_R_Record( CString strBegin,CString strEnd )
{
	bool ret = true;

	//Query  item between begin and end  in templist;
	SYSTEMTIME beginst,endst;
	CSystemTimeOp::ConvertStringToSysemTime(strBegin,&beginst);
	CSystemTimeOp::ConvertStringToSysemTime(strEnd,&endst);

	std::list<CString> tempList;
	std::map<CString,Table_ST_WL_R_Record*>::iterator wlItr;
	for (wlItr = m_mapST_WL_R.begin();
		 wlItr != m_mapST_WL_R.end();
		 wlItr ++)
	{
		Table_ST_WL_R_Record* pObj = wlItr->second;
		if (pObj != NULL)
		{
			int compbg = CSystemTimeOp::CompareSystemTime(&(pObj->FTM),&beginst);
			int compend = CSystemTimeOp::CompareSystemTime(&(pObj->FTM),&endst);
			if (compbg > 0 && compend < 0)
			{
				tempList.push_back(pObj->TM);
			}
		}
	}

	std::list<CString>::iterator strItr;
	for (strItr = tempList.begin();
		 strItr != tempList.end();
		 strItr ++)
	{
		CString sTemp = (*strItr);
		RemoveST_WL_R_Record(sTemp);
	}

	return ret;
}

bool CAddrMap_Item::UpdateST_DISP_R_Record( Table_ST_DISP_R_Record* pUpdateR )
{
	bool ret = true;

	if (pUpdateR != NULL &&
		pUpdateR->STCD == m_STCD &&
		this->GetType() == CZ_Type_DISP)
	{
		//find
		std::map<CString,Table_ST_DISP_R_Record*>::iterator findItr = m_mapST_DISP_R.find(pUpdateR->TM);

		if (findItr != m_mapST_DISP_R.end()) //found
		{
			//ret = 3;
			//update
			GetLocalTime(&(findItr->second->CAL_TM));
			findItr->second->X = pUpdateR->X;
			findItr->second->Y = pUpdateR->Y;
			/*
			if((fabs(findItr->second->X - pUpdateR->X) >= 0.01)||(fabs(findItr->second->Y - pUpdateR->Y) >= 0.01))
			{
				
				ret = 2;
			}
			*/
		}
		else  //nat found
		{
			Table_ST_DISP_R_Record* pCopy = new Table_ST_DISP_R_Record(pUpdateR);
			GetLocalTime(&(pCopy->CAL_TM));
			m_mapST_DISP_R.insert(std::pair<CString, Table_ST_DISP_R_Record*>(pCopy->TM,pCopy));
			//ret = 1;
		}
	}

	return ret;
}

bool CAddrMap_Item::RemoveST_DISP_R_Record( CString strTM )
{
	bool ret = true;
	std::map<CString,Table_ST_DISP_R_Record*>::iterator itr = m_mapST_DISP_R.find(strTM);
	if (itr != m_mapST_DISP_R.end())
	{
		_DELETE(itr->second);
		m_mapST_DISP_R.erase(strTM);
	}
	return ret;
}

bool CAddrMap_Item::UpdateST_TEMP_R_Record( Table_ST_TEMP_R_Record* pUpdateR )
{
	bool ret = true;

	if (pUpdateR != NULL &&
		pUpdateR->STCD == m_STCD &&
		this->GetType() == CZ_Type_TEMP)
	{
		//find
		std::map<CString,Table_ST_TEMP_R_Record*>::iterator findItr = m_mapST_TEMP_R.find(pUpdateR->TM);

		if (findItr != m_mapST_TEMP_R.end()) //found
		{
			//update
			GetLocalTime(&(findItr->second->CAL_TM));
			findItr->second->T = pUpdateR->T;
		}
		else  //nat found
		{
			Table_ST_TEMP_R_Record* pCopy = new Table_ST_TEMP_R_Record(pUpdateR);
			GetLocalTime(&(pCopy->CAL_TM));
			m_mapST_TEMP_R.insert(std::pair<CString, Table_ST_TEMP_R_Record*>(pCopy->TM,pCopy));
			//ret = 1;
		}
	}
	return ret;
}

bool CAddrMap_Item::RemoveST_TEMP_R_Record( CString strTM )
{
	bool ret = true;
	std::map<CString,Table_ST_TEMP_R_Record*>::iterator itr = m_mapST_TEMP_R.find(strTM);
	if (itr != m_mapST_TEMP_R.end())
	{
		_DELETE(itr->second);
		m_mapST_TEMP_R.erase(strTM);
	}
	return ret;
}

bool CAddrMap_Item::UpdateStatisticsOnTimerItem( StatisticsOnTimer_Item* pUpdateR )
{
	bool ret = true;
	if (pUpdateR != NULL && pUpdateR->STCD == m_STCD)
	{
		if (pUpdateR->Type == CZ_Type_RAIN && GetType() == CZ_Type_RAIN)
		{
			//find 
			std::map<CString,StatisticsOnTimer_Item*>::iterator findItr = m_mapStatistics_Rain.find(pUpdateR->strTM);

			//update
			if (findItr != m_mapStatistics_Rain.end()) //found
			{
				if (findItr->second != NULL)
					GetLocalTime(&(findItr->second->CAL_TM));
			}
			else  //not found
			{
				//insert
				StatisticsOnTimer_Item* pCopy = new StatisticsOnTimer_Item(pUpdateR);
				GetLocalTime(&(pCopy->CAL_TM));
				m_mapStatistics_Rain.insert(std::pair<CString, StatisticsOnTimer_Item*>(pCopy->strTM,pCopy));
			}
		}
		else if (pUpdateR->Type == CZ_Type_WL && GetType()== CZ_Type_WL && pUpdateR->WLType == GetWlType())
		{
			//find
			std::map<CString,StatisticsOnTimer_Item*>::iterator findItr = m_mapStatistics_WaterL.find(pUpdateR->strTM);

			//update
			if (findItr != m_mapStatistics_WaterL.end())
			{
				if (findItr->second != NULL)
					GetLocalTime(&(findItr->second->CAL_TM));
			}
			else //not found 
			{ 
				//insert
				StatisticsOnTimer_Item* pCopy = new StatisticsOnTimer_Item(pUpdateR);
				GetLocalTime(&(pCopy->CAL_TM));
				m_mapStatistics_WaterL.insert(std::pair<CString, StatisticsOnTimer_Item*>(pCopy->strTM,pCopy));
			}
		}
		else if (pUpdateR->Type == CZ_Type_DISP && GetType() == CZ_Type_DISP)
		{
			//find 
			std::map<CString,StatisticsOnTimer_Item*>::iterator findItr = m_mapStatistics_Displace.find(pUpdateR->strTM);

			//update
			if (findItr != m_mapStatistics_Displace.end()) //found
			{
				if (findItr->second != NULL)
					GetLocalTime(&(findItr->second->CAL_TM));
			}
			else  //not found
			{
				//insert
				StatisticsOnTimer_Item* pCopy = new StatisticsOnTimer_Item(pUpdateR);
				GetLocalTime(&(pCopy->CAL_TM));
				m_mapStatistics_Displace.insert(std::pair<CString, StatisticsOnTimer_Item*>(pCopy->strTM,pCopy));
			}
		}
		else if (pUpdateR->Type == CZ_Type_TEMP && GetType() == CZ_Type_TEMP)
		{
			//find 
			std::map<CString,StatisticsOnTimer_Item*>::iterator findItr = m_mapStatistics_Tempera.find(pUpdateR->strTM);

			//update
			if (findItr != m_mapStatistics_Tempera.end()) //found
			{
				if (findItr->second != NULL)
					GetLocalTime(&(findItr->second->CAL_TM));
			}
			else  //not found
			{
				//insert
				StatisticsOnTimer_Item* pCopy = new StatisticsOnTimer_Item(pUpdateR);
				GetLocalTime(&(pCopy->CAL_TM));
				m_mapStatistics_Tempera.insert(std::pair<CString, StatisticsOnTimer_Item*>(pCopy->strTM,pCopy));
			}
		}
		//2023-5-23的修改记录
		else if (pUpdateR->Type == CZ_Type_FLOW && GetType() == CZ_Type_FLOW)
		{
			std::map<CString, StatisticsOnTimer_Item*>::iterator findItr = m_mapStatistics_Flow.find(pUpdateR->strTM);
			//update
			if (findItr != m_mapStatistics_Flow.end()) //found
			{
				if (findItr->second != NULL)
					GetLocalTime(&(findItr->second->CAL_TM));
			}
			else  //not found
			{
				//insert
				StatisticsOnTimer_Item* pCopy = new StatisticsOnTimer_Item(pUpdateR);
				GetLocalTime(&(pCopy->CAL_TM));
				m_mapStatistics_Flow.insert(std::pair<CString, StatisticsOnTimer_Item*>(pCopy->strTM, pCopy));
			}
		}

	}
	return ret;
}

bool CAddrMap_Item::RemoveStatisticsOnTimerItem( CZ_Type_Enum Type,CString strTM )
{
	bool ret = true;
	if (Type == CZ_Type_RAIN)
	{
		std::map<CString,StatisticsOnTimer_Item*>::iterator delItr = m_mapStatistics_Rain.find(strTM);
		if (delItr != m_mapStatistics_Rain.end()) //
		{
			_DELETE(delItr->second);
			m_mapStatistics_Rain.erase(strTM);
		}
	}
	else if(Type == CZ_Type_WL)
	{
		std::map<CString,StatisticsOnTimer_Item*>::iterator delItr = m_mapStatistics_WaterL.find(strTM);
		if (delItr != m_mapStatistics_WaterL.end())
		{
			_DELETE(delItr->second);
			m_mapStatistics_WaterL.erase(strTM);
		}

	}
	else if(Type == CZ_Type_DISP)
	{
		std::map<CString,StatisticsOnTimer_Item*>::iterator delItr = m_mapStatistics_Displace.find(strTM);
		if (delItr != m_mapStatistics_Displace.end())
		{
			_DELETE(delItr->second);
			m_mapStatistics_Displace.erase(strTM);
		}
	}
	else if(Type == CZ_Type_TEMP)
	{
		std::map<CString,StatisticsOnTimer_Item*>::iterator delItr = m_mapStatistics_Tempera.find(strTM);
		if (delItr != m_mapStatistics_Tempera.end())
		{
			_DELETE(delItr->second);
			m_mapStatistics_Tempera.erase(strTM);
		}
	}
	else if(Type == CZ_Type_FLOW)
	{
		//2023-5-23

		std::map<CString, StatisticsOnTimer_Item*>::iterator delItr1 = m_mapStatistics_Flow.find(strTM);
		if (delItr1 != m_mapStatistics_Flow.end())
		{
			_DELETE(delItr1->second);
			m_mapStatistics_Flow.erase(strTM);
		}
	}

	return ret;
}

bool CAddrMap_Item::InsertData( CRainData_Item* pRain )
{
	return m_UpDataCache.InsertData(pRain);
}

bool CAddrMap_Item::InsertData( CWaterL_DataItem* pWaterL )
{
	return m_UpDataCache.InsertData(pWaterL);
}

bool CAddrMap_Item::InsertData( CDisplace_DataItem* pDisplace )
{
	return m_UpDataCache.InsertData(pDisplace);
}

bool CAddrMap_Item::InsertData( CTempera_DataItem* pTempera )
{
	return m_UpDataCache.InsertData(pTempera);
}

bool CAddrMap_Item::InsertData(CFlow_DataItem* pFlowdata)
{
	return m_UpDataCache.InsertData(pFlowdata);
}

void CAddrMap_Item::SetUpDataCacheTimeArea( SYSTEMTIME AreaBegin,SYSTEMTIME AreaEnd )
{
	m_UpDataCache.m_stCacheAreaBegin = AreaBegin;
	m_UpDataCache.m_stCacheAreaEnd = AreaEnd;
}

CRainData_Item::CRainData_Item(int dataType /* = 0 */, double CurVal /* = 0.0 */, double SumHalfH /* = 0.0 */, double TotalVal /* = 0.0 */, CString sCollTime /* ="" */)
{
	m_nDataType = dataType;
	m_dCurrentValue = CurVal;
	m_dSumHalfHour = SumHalfH;
	m_dTotalValue = TotalVal;
	m_sCollectTime = sCollTime;

	if (m_sCollectTime == "")
		GetLocalTime(&m_tCollectTime);
	else
		CSystemTimeOp::ConvertStringToSysemTime(m_sCollectTime,&m_tCollectTime);	
}

CRainData_Item::CRainData_Item(CRainData_Item* pSource)
{
	if (pSource)
	{
		m_nDataType = pSource->DataType();
		m_dCurrentValue = pSource->CurrentValue();
		m_dSumHalfHour = pSource->SumHalfHour();
		m_dTotalValue = pSource->TotalValue();
		m_sCollectTime = pSource->CollectTimeString();

		if (m_sCollectTime == "")
			GetLocalTime(&m_tCollectTime);
		else
			CSystemTimeOp::ConvertStringToSysemTime(m_sCollectTime,&m_tCollectTime);
	}
}

const CString CRainData_Item::ToString()
{
	CString sResult = "";
	sResult.Format("DataType=%d,CurrentValue=%.2f,SumHalfHour= %.2f,TotalValue=%.2f,RecordTime=%s",
		m_nDataType,m_dCurrentValue,m_dSumHalfHour,m_dTotalValue,m_sCollectTime);
	return sResult;
}

bool CRainData_Item::Equals( CRainData_Item* pSrc )
{
	bool ret = false;
	if (pSrc != NULL)
	{
		if (m_nDataType == pSrc->DataType() &&
			CUdfGlobalFUNC::CompareDouble(m_dCurrentValue,pSrc->CurrentValue(),7,1) == 0 &&
			CUdfGlobalFUNC::CompareDouble(m_dSumHalfHour,pSrc->SumHalfHour(),7,1) == 0 &&
			CUdfGlobalFUNC::CompareDouble(m_dTotalValue,pSrc->TotalValue(),7,1) == 0 && 
			m_sCollectTime == pSrc->CollectTimeString() &&
			CSystemTimeOp::CompareSystemTime(&m_tCollectTime,&(pSrc->m_tCollectTime)) == 0)
			ret = true;
	}
	return ret;
}

CWaterL_DataItem::CWaterL_DataItem()
{
	m_dValue = 0.0;
	GetLocalTime(&m_RecordTime);
}

CWaterL_DataItem::CWaterL_DataItem( double dValue, SYSTEMTIME& st )
{
	m_dValue = dValue;
	m_RecordTime = st;
	m_sRecordTime  = CSystemTimeOp::ConvertSystemTimeToString(&st,true);
}

CWaterL_DataItem::CWaterL_DataItem( double dValue, SYSTEMTIME& st, CString strSt )
{
	m_dValue = dValue;
	m_RecordTime = st;
	m_sRecordTime  = strSt;
}

CWaterL_DataItem::CWaterL_DataItem( CWaterL_DataItem* pSource )
{
	if (pSource !=NULL)
	{
		this->m_dValue = pSource->m_dValue;
		this->m_sRecordTime = pSource->m_sRecordTime;
		this->m_RecordTime = pSource->m_RecordTime;
	}
}

CString CWaterL_DataItem::ToString()
{
	CString strResult = "";
	strResult.Format("%s %f",CSystemTimeOp::ConvertSystemTimeToString(&m_RecordTime,true),m_dValue);
	return strResult;
}

bool CWaterL_DataItem::Equals( CWaterL_DataItem* pSrc )
{
	bool ret = false;
	if (pSrc != NULL)
	{
		if (CSystemTimeOp::CompareSystemTime(&m_RecordTime,&(pSrc->m_RecordTime)) == 0 &&
			m_sRecordTime == pSrc->m_sRecordTime &&
			CUdfGlobalFUNC::CompareDouble(m_dValue,pSrc->m_dValue,7,2) == 0)
			ret = true;
	}
	return ret;
}

CDisplace_DataItem::CDisplace_DataItem()
{
	m_XValue = 0.0;
	m_YValue = 0.0;
	GetLocalTime(&m_RecordTime);
}

//CDisplace_DataItem::CDisplace_DataItem( double xValue,double yValue, int iDisplaceNo,SYSTEMTIME& st )
CDisplace_DataItem::CDisplace_DataItem( double xValue,double yValue,SYSTEMTIME& st )
{
	m_XValue = xValue;
	m_YValue = yValue;
	//m_DisplaceNo = iDisplaceNo;
	m_RecordTime = st;
	m_sRecordTime  = CSystemTimeOp::ConvertSystemTimeToString(&st,true);
}

//CDisplace_DataItem::CDisplace_DataItem( double xValue, double yValue,int iDisplaceNo, SYSTEMTIME& st, CString strSt )
CDisplace_DataItem::CDisplace_DataItem( double xValue, double yValue, SYSTEMTIME& st, CString strSt )
{
	m_XValue = xValue;
	m_YValue = yValue;
	//m_DisplaceNo = iDisplaceNo;
	m_RecordTime = st;
	m_sRecordTime  = strSt;
}

CDisplace_DataItem::CDisplace_DataItem( CDisplace_DataItem* pSource )
{
	if (pSource !=NULL)
	{
		this->m_XValue = pSource->m_XValue;
		this->m_YValue = pSource->m_YValue;
		//this->m_DisplaceNo = pSource->m_DisplaceNo;
		this->m_sRecordTime = pSource->m_sRecordTime;
		this->m_RecordTime = pSource->m_RecordTime;
	}
}

CString CDisplace_DataItem::ToString()
{
	CString strResult = "";
	strResult.Format("%s %f %f",CSystemTimeOp::ConvertSystemTimeToString(&m_RecordTime,true),m_XValue,m_YValue);
	return strResult;
}

bool CDisplace_DataItem::Equals( CDisplace_DataItem* pSrc )
{
	bool ret = false;
	if (pSrc != NULL)
	{
		if (CSystemTimeOp::CompareSystemTime(&m_RecordTime,&(pSrc->m_RecordTime)) == 0 &&
			m_sRecordTime == pSrc->m_sRecordTime &&
			CUdfGlobalFUNC::CompareDouble(m_XValue,pSrc->m_XValue,7,2) == 0 &&
			CUdfGlobalFUNC::CompareDouble(m_YValue,pSrc->m_YValue,7,2) == 0)
			ret = true;
	}
	return ret;
}

CTempera_DataItem::CTempera_DataItem()
{
	m_tValue = 0.0;
	GetLocalTime(&m_RecordTime);
}

CTempera_DataItem::CTempera_DataItem( double tValue, SYSTEMTIME& st )
{
	m_tValue = tValue;
	m_RecordTime = st;
	m_sRecordTime  = CSystemTimeOp::ConvertSystemTimeToString(&st,true);
}

CTempera_DataItem::CTempera_DataItem( double tValue, SYSTEMTIME& st, CString strSt )
{
	m_tValue = tValue;
	m_RecordTime = st;
	m_sRecordTime  = strSt;
}

CTempera_DataItem::CTempera_DataItem( CTempera_DataItem* pSource )
{
	if (pSource !=NULL)
	{
		this->m_tValue = pSource->m_tValue;
		this->m_sRecordTime = pSource->m_sRecordTime;
		this->m_RecordTime = pSource->m_RecordTime;
	}
}

CString CTempera_DataItem::ToString()
{
	CString strResult = "";
	strResult.Format("%s %f",CSystemTimeOp::ConvertSystemTimeToString(&m_RecordTime,true),m_tValue);
	return strResult;
}

bool CTempera_DataItem::Equals( CTempera_DataItem* pSrc )
{
	bool ret = false;
	if (pSrc != NULL)
	{
		if (CSystemTimeOp::CompareSystemTime(&m_RecordTime,&(pSrc->m_RecordTime)) == 0 &&
			m_sRecordTime == pSrc->m_sRecordTime &&
			CUdfGlobalFUNC::CompareDouble(m_tValue,pSrc->m_tValue,7,2) == 0)
			ret = true;
	}
	return ret;
}
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


const CString STRUCT_BEGINEND_AREA::ToString()
{
	CString sResult = "";
	sResult.Format("Time Area(Begin=%s,EndTime=%s)",
		CSystemTimeOp::ConvertSystemTimeToString(&stBegin,true),
		CSystemTimeOp::ConvertSystemTimeToString(&stEnd,true));
	return sResult;

}

StatisticsOnTimer_Item::StatisticsOnTimer_Item()
{
	STCD = "";
	Type = CZ_Type_RAIN;
	WLType = WL_Type_HLSW;
	strTM = "";
	Cal_Type = Rain_statistics_UNKNOWN;
	strFTM = "";
	strBegin = "";
	strEnd = "";
	CSystemTimeOp::ResetSystemTime(stFTM);
	CSystemTimeOp::ResetSystemTime(stBegin);
	CSystemTimeOp::ResetSystemTime(stEnd);
	CSystemTimeOp::ResetSystemTime(CAL_TM);
	PV = 0;
	Sunflowdata = 0.0;
	Leiflowdata = 0.0;
	CSystemTimeOp::ResetSystemTime(Collect);
}

StatisticsOnTimer_Item::StatisticsOnTimer_Item( StatisticsOnTimer_Item* pSource )
{
	if (pSource != NULL)
	{
		STCD = pSource->STCD;
		Type = pSource->Type;
		WLType = pSource->WLType;
		RTUNo = pSource->RTUNo;
		strTM = pSource->strTM;
		Cal_Type = pSource->Cal_Type;
		strFTM = pSource->strFTM;
		strBegin = pSource->strBegin;
		strEnd = pSource->strEnd;
		stFTM = pSource->stFTM;
		stBegin = pSource->stBegin;
		stEnd = pSource->stEnd;
		CAL_TM = pSource->CAL_TM;
		PV = pSource->PV;
		X=pSource->X;
		Y=pSource->Y;
		T=pSource->T;
		Sunflowdata = pSource->Sunflowdata;
		Leiflowdata = pSource->Leiflowdata;
		Collect = pSource->Collect;
	}
	else
	{
		STCD = "";
		Type = CZ_Type_RAIN;
		WLType = WL_Type_HLSW;
		RTUNo = 0;
		strTM = "";
		Cal_Type = Rain_statistics_UNKNOWN;
		strFTM = "";
		strBegin = "";
		strEnd = "";
		CSystemTimeOp::ResetSystemTime(stFTM);
		CSystemTimeOp::ResetSystemTime(stBegin);
		CSystemTimeOp::ResetSystemTime(stEnd);
		CSystemTimeOp::ResetSystemTime(CAL_TM);
		PV = 0;
		X=0;
		Y=0;
		T=0;
		Sunflowdata = 0.0;
	    Leiflowdata = 0.0;
		CSystemTimeOp::ResetSystemTime(Collect);
	}
}

const CString StatisticsOnTimer_Item::ToString()
{
	CString sResult = "";
	sResult.Format("STCD=%s,Type=%d,WLType=%d,RTUNo=%d,strTM=%s,Cal_Type=%d,strBegin=%s,strEnd=%s,stTM=%s,stBegin=%s,stEnd=%s,CAL_TM=%s",
		STCD,(int)Type,(int)WLType,RTUNo,strTM,(int)Cal_Type,strBegin,strEnd,
		CSystemTimeOp::ConvertSystemTimeToString(&stFTM,true),
		CSystemTimeOp::ConvertSystemTimeToString(&stBegin,true),
		CSystemTimeOp::ConvertSystemTimeToString(&stEnd,true),
		CSystemTimeOp::ConvertSystemTimeToString(&CAL_TM,true));
	return sResult;
}


bool YDJUpData_Cache::InsertData( CRainData_Item* pRain )
{
	bool ret = true;
	if (pRain != NULL)
	{
		//判断插入内存的数据是否该在当前内存缓存区内
		int comp_area = CSystemTimeOp::CompareSystemTime(&(pRain->m_tCollectTime),&m_stCacheAreaBegin);
		if (comp_area < 0)
			return ret;
		comp_area = CSystemTimeOp::CompareSystemTime(&(pRain->m_tCollectTime),&m_stCacheAreaEnd);
		if (comp_area >0 )
			return ret;


		bool blExist = false;
		int CountKey = m_mapRainData.count(pRain->CollectTimeString());
		if (CountKey > 0)
		{
			std::multimap<CString,CRainData_Item*>::iterator tmpItr;
			std::pair<std::multimap<CString,CRainData_Item*>::iterator,std::multimap<CString,CRainData_Item*>::iterator> rangeItr;
			rangeItr = m_mapRainData.equal_range(pRain->CollectTimeString());
			for (tmpItr = rangeItr.first;
				tmpItr != rangeItr.second; 
				++ tmpItr)
			{
				CRainData_Item* pOld = tmpItr->second;
				if (pOld!= NULL && pOld->Equals(pRain) == true)
				{
#ifdef DEBUG
					TRACE("InsertData() Exist:%s = %s\r\n",pRain->ToString(),pOld->ToString());
#endif
					blExist = true;
					break;;
				}
			}
		}

		if (false == blExist)
		{
			std::multimap<CString,CRainData_Item*>::iterator InItr = m_mapRainData.end();
			InItr = m_mapRainData.insert(std::pair<CString,CRainData_Item*>(pRain->CollectTimeString(),pRain));
			//if (InItr != m_mapRainData.end())
			{
				int nCount = m_mapRainData.size();
				if (1 == nCount)
				{
					m_stRainBeginTime = pRain->m_tCollectTime;
					m_stRainEndTime = pRain->m_tCollectTime;
				}
				else
				{
					int comp = 0;
					comp = CSystemTimeOp::CompareSystemTime(&m_stRainBeginTime,&(pRain->m_tCollectTime));
					if (comp > 0 )
						m_stRainBeginTime = pRain->m_tCollectTime;

					comp = CSystemTimeOp::CompareSystemTime(&m_stRainEndTime,&(pRain->m_tCollectTime));
					if (comp < 0)
						m_stRainEndTime = pRain->m_tCollectTime;
				}
			}
		}
	}
	return ret;
}

bool YDJUpData_Cache::InsertData( CWaterL_DataItem* pWaterL )
{
	bool ret = true;
	if (pWaterL != NULL)
	{

		//判断插入内存的数据是否该在当前内存缓存区内
		int comp_area = CSystemTimeOp::CompareSystemTime(&(pWaterL->m_RecordTime),&m_stCacheAreaBegin);
		if (comp_area < 0)
			return ret;
		comp_area = CSystemTimeOp::CompareSystemTime(&(pWaterL->m_RecordTime),&m_stCacheAreaEnd);
		if (comp_area > 0 )
			return ret;

		bool blExist = false;
		if (m_mapWaterLData.count(pWaterL->m_sRecordTime) > 0)
		{
			std::multimap<CString,CWaterL_DataItem*>::iterator tmpItr;
			std::pair<std::multimap<CString,CWaterL_DataItem*>::iterator,std::multimap<CString,CWaterL_DataItem*>::iterator> rangeRet;
			rangeRet = m_mapWaterLData.equal_range(pWaterL->m_sRecordTime);
			for (tmpItr = rangeRet.first;
				 tmpItr != rangeRet.second;
				 ++ tmpItr)
			{
				CWaterL_DataItem* pOld = tmpItr->second;
				if (pOld != NULL && pOld->Equals(pWaterL) == true)
				{
#ifdef DEBUG
					TRACE("InsertData() Exist:%s=%s\r\n",pOld->ToString(),pWaterL->ToString());
#endif
					blExist = true;
					break;
				}
			}
		}

		if (false == blExist)
		{
			std::multimap<CString,CWaterL_DataItem*>::iterator InItr = m_mapWaterLData.end();
			InItr = m_mapWaterLData.insert(std::pair<CString,CWaterL_DataItem*>(pWaterL->m_sRecordTime,pWaterL));
			//if (InItr != m_mapWaterLData.end())
			{
				int nCount = m_mapWaterLData.size();
				if (1 == nCount)
				{
					m_stWaterLBeginTime = pWaterL->m_RecordTime;
					m_stWaterLEndTime = pWaterL->m_RecordTime;
				}
				else 
				{
					int comp = 0;
					comp = CSystemTimeOp::CompareSystemTime(&m_stWaterLBeginTime,&(pWaterL->m_RecordTime));
					if (comp > 0 )
						m_stWaterLBeginTime = pWaterL->m_RecordTime;

					comp = CSystemTimeOp::CompareSystemTime(&m_stWaterLEndTime,&(pWaterL->m_RecordTime));
					if (comp < 0)
						m_stWaterLEndTime = pWaterL->m_RecordTime;
				}
			}
		}
	}

	return ret;
}

bool YDJUpData_Cache::InsertData( CDisplace_DataItem* pDisplace )
{
	bool ret = true;
	if (pDisplace != NULL)
	{

		//判断插入内存的数据是否该在当前内存缓存区内
		int comp_area = CSystemTimeOp::CompareSystemTime(&(pDisplace->m_RecordTime),&m_stCacheAreaBegin);
		if (comp_area < 0)
			return ret;
		comp_area = CSystemTimeOp::CompareSystemTime(&(pDisplace->m_RecordTime),&m_stCacheAreaEnd);
		if (comp_area > 0 )
			return ret;

		bool blExist = false;
		if (m_mapDisplaceData.count(pDisplace->m_sRecordTime) > 0)
		{
			std::multimap<CString,CDisplace_DataItem*>::iterator tmpItr;
			std::pair<std::multimap<CString,CDisplace_DataItem*>::iterator,std::multimap<CString,CDisplace_DataItem*>::iterator> rangeRet;
			rangeRet = m_mapDisplaceData.equal_range(pDisplace->m_sRecordTime);
			for (tmpItr = rangeRet.first;
				 tmpItr != rangeRet.second;
				 ++ tmpItr)
			{
				CDisplace_DataItem* pOld = tmpItr->second;
				if (pOld != NULL && pOld->Equals(pDisplace) == true)
				{
#ifdef DEBUG
					TRACE("InsertData() Exist:%s=%s\r\n",pOld->ToString(),pDisplace->ToString());
#endif
					blExist = true;
					break;
				}
			}
		}

		if (false == blExist)
		{
			std::multimap<CString,CDisplace_DataItem*>::iterator InItr = m_mapDisplaceData.end();
			InItr = m_mapDisplaceData.insert(std::pair<CString,CDisplace_DataItem*>(pDisplace->m_sRecordTime,pDisplace));
			//if (InItr != m_mapDisplaceData.end())
			{
				int nCount = m_mapDisplaceData.size();
				if (1 == nCount)
				{
					m_stDisplaceBeginTime = pDisplace->m_RecordTime;
					m_stDisplaceEndTime = pDisplace->m_RecordTime;
				}
				else 
				{
					int comp = 0;
					comp = CSystemTimeOp::CompareSystemTime(&m_stDisplaceBeginTime,&(pDisplace->m_RecordTime));
					if (comp > 0 )
						m_stDisplaceBeginTime = pDisplace->m_RecordTime;

					comp = CSystemTimeOp::CompareSystemTime(&m_stDisplaceEndTime,&(pDisplace->m_RecordTime));
					if (comp < 0)
						m_stDisplaceEndTime = pDisplace->m_RecordTime;
				}
			}
		}
	}

	return ret;
}

bool YDJUpData_Cache::InsertData( CTempera_DataItem* pTempera )
{
	bool ret = true;
	if (pTempera != NULL)
	{

		//判断插入内存的数据是否该在当前内存缓存区内
		int comp_area = CSystemTimeOp::CompareSystemTime(&(pTempera->m_RecordTime),&m_stCacheAreaBegin);
		if (comp_area < 0)
			return ret;
		comp_area = CSystemTimeOp::CompareSystemTime(&(pTempera->m_RecordTime),&m_stCacheAreaEnd);
		if (comp_area > 0 )
			return ret;

		bool blExist = false;
		if (m_mapTemperaData.count(pTempera->m_sRecordTime) > 0)
		{
			std::multimap<CString,CTempera_DataItem*>::iterator tmpItr;
			std::pair<std::multimap<CString,CTempera_DataItem*>::iterator,std::multimap<CString,CTempera_DataItem*>::iterator> rangeRet;
			rangeRet = m_mapTemperaData.equal_range(pTempera->m_sRecordTime);
			for (tmpItr = rangeRet.first;
				 tmpItr != rangeRet.second;
				 ++ tmpItr)
			{
				CTempera_DataItem* pOld = tmpItr->second;
				if (pOld != NULL && pOld->Equals(pTempera) == true)
				{
#ifdef DEBUG
					TRACE("InsertData() Exist:%s=%s\r\n",pOld->ToString(),pTempera->ToString());
#endif
					blExist = true;
					break;
				}
			}
		}

		if (false == blExist)
		{
			std::multimap<CString,CTempera_DataItem*>::iterator InItr = m_mapTemperaData.end();
			InItr = m_mapTemperaData.insert(std::pair<CString,CTempera_DataItem*>(pTempera->m_sRecordTime,pTempera));
			//if (InItr != m_mapTemperaData.end())
			{
				int nCount = m_mapTemperaData.size();
				if (1 == nCount)
				{
					m_stTemperaBeginTime = pTempera->m_RecordTime;
					m_stTemperaEndTime = pTempera->m_RecordTime;
				}
				else 
				{
					int comp = 0;
					comp = CSystemTimeOp::CompareSystemTime(&m_stTemperaBeginTime,&(pTempera->m_RecordTime));
					if (comp > 0 )
						m_stTemperaBeginTime = pTempera->m_RecordTime;

					comp = CSystemTimeOp::CompareSystemTime(&m_stTemperaEndTime,&(pTempera->m_RecordTime));
					if (comp < 0)
						m_stTemperaEndTime = pTempera->m_RecordTime;
				}
			}
		}
	}

	return ret;
}

void YDJUpData_Cache::Clear()
{
	if (m_mapRainData.size() > 0)
	{
		std::multimap<CString,CRainData_Item*>::iterator delItr;
		for (delItr = m_mapRainData.begin();
			 delItr != m_mapRainData.end();
			 delItr ++)
		{
			 _DELETE(delItr->second);
		}
		m_mapRainData.clear();
	}

	if (m_mapWaterLData.size() > 0)
	{
		std::multimap<CString,CWaterL_DataItem*>::iterator delItr;
		for (delItr = m_mapWaterLData.begin();
			 delItr != m_mapWaterLData.end();
			 delItr ++)
		{
			_DELETE(delItr->second);
		}
		m_mapWaterLData.clear();
	}

	if (m_mapDisplaceData.size() > 0)
	{
		std::multimap<CString,CDisplace_DataItem*>::iterator delItr;
		for (delItr = m_mapDisplaceData.begin();
			 delItr != m_mapDisplaceData.end();
			 delItr ++)
		{
			_DELETE(delItr->second);
		}
		m_mapDisplaceData.clear();
	}

	if (m_mapTemperaData.size() > 0)
	{
		std::multimap<CString,CTempera_DataItem*>::iterator delItr;
		for (delItr = m_mapTemperaData.begin();
			 delItr != m_mapTemperaData.end();
			 delItr ++)
		{
			_DELETE(delItr->second);
		}
		m_mapTemperaData.clear();
	}

	if (m_mapFlowData.size() > 0)
	{
		std::multimap<CString, CFlow_DataItem*>::iterator delItr;
		for (delItr = m_mapFlowData.begin();delItr != m_mapFlowData.end();delItr++)
		{
			_DELETE(delItr->second);
		}
		m_mapFlowData.clear();
	}
}

void YDJUpData_Cache::SetUpDataCacheTimeArea( SYSTEMTIME AreaBegin,SYSTEMTIME AreaEnd )
{
	this->m_stCacheAreaBegin = AreaBegin;
	this->m_stCacheAreaEnd  = AreaEnd;
}

void YDJUpData_Cache::RemoveOutDataOnTimer()
{
	std::list<CString> tempList;

	std::multimap<CString,CRainData_Item*>::iterator delRainItr;
	for (delRainItr = m_mapRainData.begin();
		 delRainItr != m_mapRainData.end();
		 delRainItr ++)
	{
		CRainData_Item* pObj = delRainItr->second;
		if (pObj != NULL)
		{
			int comp = CSystemTimeOp::CompareSystemTime(&(pObj->m_tCollectTime),&m_stCacheAreaBegin);
			if (comp < 0)
				tempList.push_back(pObj->CollectTimeString());
			if (comp >=0)
				break;
		}
	}

	//清除
	if (tempList.size() >0)
	{
		std::list<CString>::iterator itr;
		for (itr = tempList.begin();
			 itr != tempList.end();
			 itr ++)
		{
			CString sTemp = (*itr);
			RemoveRainData(sTemp);
		}
	}

	//更新雨量数据的起止时间范围
	if (m_mapRainData.size() >0 )
	{
		delRainItr = m_mapRainData.begin();
		CRainData_Item* pObj = delRainItr->second;
		if (pObj != NULL)
			 m_stRainBeginTime = pObj->m_tCollectTime;
		delRainItr = m_mapRainData.end();
		delRainItr --;
		if (delRainItr != m_mapRainData.end())
		{
			pObj = delRainItr->second;
			if (pObj != NULL)
				m_stRainEndTime = pObj->m_tCollectTime;
		}
	}

	tempList.clear();
	std::multimap<CString,CWaterL_DataItem*>::iterator delWaterLItr;
	for (delWaterLItr = m_mapWaterLData.begin();
		 delWaterLItr != m_mapWaterLData.end();
		 delWaterLItr ++)
	{
		CWaterL_DataItem* pObj = delWaterLItr->second;
		if (pObj != NULL)
		{
			int comp = CSystemTimeOp::CompareSystemTime(&(pObj->m_RecordTime),&m_stCacheAreaBegin);
			if (comp <0)
				tempList.push_back(pObj->m_sRecordTime);
			if (comp >=0)
				break;
		}
	}

	//清除
	if (tempList.size() > 0)
	{
		std::list<CString>::iterator itr;
		for (itr = tempList.begin();
			 itr != tempList.end();
			 itr ++)
		{
			CString sTemp = (*itr);
			RemoveWaterLData(sTemp);
		}
	}

	//更新水位数据的起止时间范围
	if (m_mapWaterLData.size() > 0)
	{
		delWaterLItr = m_mapWaterLData.begin();
		CWaterL_DataItem* pObj = delWaterLItr->second;
		if (pObj != NULL)
			m_stWaterLBeginTime  = pObj->m_RecordTime;
		delWaterLItr = m_mapWaterLData.end();
		delWaterLItr --;
		if (delWaterLItr != m_mapWaterLData.end())
		{
			pObj = delWaterLItr->second;
			if (pObj != NULL)
				m_stWaterLEndTime = pObj->m_RecordTime;
		}
	}

	tempList.clear();
	std::multimap<CString,CDisplace_DataItem*>::iterator delDisplaceItr;
	for (delDisplaceItr = m_mapDisplaceData.begin();
		 delDisplaceItr != m_mapDisplaceData.end();
		 delDisplaceItr ++)
	{
		CDisplace_DataItem* pObj = delDisplaceItr->second;
		if (pObj != NULL)
		{
			int comp = CSystemTimeOp::CompareSystemTime(&(pObj->m_RecordTime),&m_stCacheAreaBegin);
			if (comp <0)
				tempList.push_back(pObj->m_sRecordTime);
			if (comp >=0)
				break;
		}
	}

	//清除
	if (tempList.size() > 0)
	{
		std::list<CString>::iterator itr;
		for (itr = tempList.begin(); itr != tempList.end(); itr ++)
		{
			CString sTemp = (*itr);
			RemoveDisplaceData(sTemp);
		}
	}

	//更新位移数据的起止时间范围
	if (m_mapDisplaceData.size() > 0)
	{
		delDisplaceItr = m_mapDisplaceData.begin();
		CDisplace_DataItem* pObj = delDisplaceItr->second;
		if (pObj != NULL)
			m_stDisplaceBeginTime  = pObj->m_RecordTime;
		delDisplaceItr = m_mapDisplaceData.end();
		delDisplaceItr --;
		if (delDisplaceItr != m_mapDisplaceData.end())
		{
			pObj = delDisplaceItr->second;
			if (pObj != NULL)
				m_stDisplaceEndTime = pObj->m_RecordTime;
		}
	}

	tempList.clear();
	std::multimap<CString,CTempera_DataItem*>::iterator delTemperaItr;
	for (delTemperaItr = m_mapTemperaData.begin();delTemperaItr != m_mapTemperaData.end(); delTemperaItr ++)
	{
		CTempera_DataItem* pObj = delTemperaItr->second;
		if (pObj != NULL)
		{
			int comp = CSystemTimeOp::CompareSystemTime(&(pObj->m_RecordTime),&m_stCacheAreaBegin);
			if (comp <0)
				tempList.push_back(pObj->m_sRecordTime);
			if (comp >=0)
				break;
		}
	}

	//清除
	if (tempList.size() > 0)
	{
		std::list<CString>::iterator itr;
		for (itr = tempList.begin();
			 itr != tempList.end();
			 itr ++)
		{
			CString sTemp = (*itr);
			RemoveTemperaData(sTemp);
		}
	}

	//更新温度数据的起止时间范围
	if (m_mapTemperaData.size() > 0)
	{
		delTemperaItr = m_mapTemperaData.begin();
		CTempera_DataItem* pObj = delTemperaItr->second;
		if (pObj != NULL)
			m_stTemperaBeginTime  = pObj->m_RecordTime;
		delTemperaItr = m_mapTemperaData.end();
		delTemperaItr --;
		if (delTemperaItr != m_mapTemperaData.end())
		{
			pObj = delTemperaItr->second;
			if (pObj != NULL)
				m_stTemperaEndTime = pObj->m_RecordTime;
		}
	}
	tempList.clear();
	std::multimap<CString,CFlow_DataItem*>::iterator delFlowItr;
	for (delFlowItr = m_mapFlowData.begin(); delFlowItr != m_mapFlowData.end(); delFlowItr++)
	{
		CFlow_DataItem* pObj = delFlowItr->second;
		if (pObj != NULL)
		{
			int comp = CSystemTimeOp::CompareSystemTime(&(pObj->m_RecordTime), &m_stCacheAreaBegin);
			if (comp < 0)
				tempList.push_back(pObj->m_sRecordTime);
			if (comp >= 0)
				break;
		}
	}

	//清除
	if (tempList.size() > 0)
	{
		std::list<CString>::iterator itr;
		for (itr = tempList.begin();
			itr != tempList.end();
			itr++)
		{
			CString sTemp = (*itr);
			RemoveFlowData(sTemp);
		}
	}

	//更新温度数据的起止时间范围
	if (m_mapFlowData.size() > 0)
	{
		delFlowItr = m_mapFlowData.begin();
		CFlow_DataItem* pObj = delFlowItr->second;
		if (pObj != NULL)
			m_stFlowBeginTime = pObj->m_RecordTime;
		delFlowItr = m_mapFlowData.end();
		delFlowItr--;
		if (delFlowItr != m_mapFlowData.end())
		{
			pObj = delFlowItr->second;
			if (pObj != NULL)
				m_stFlowEndTime = pObj->m_RecordTime;
		}
	}
}

void YDJUpData_Cache::RemoveRainData( CString strTime )
{
	std::multimap<CString,CRainData_Item*>::iterator delItr =  m_mapRainData.find(strTime);
	while(delItr != m_mapRainData.end())
	{
		_DELETE(delItr->second);
		m_mapRainData.erase(delItr);
		delItr  = m_mapRainData.find(strTime);
	}
}

void YDJUpData_Cache::RemoveWaterLData( CString strTime )
{
	std::multimap<CString,CWaterL_DataItem*>::iterator delItr = m_mapWaterLData.find(strTime);
	while(delItr != m_mapWaterLData.end())
	{
		_DELETE(delItr->second);
		m_mapWaterLData.erase(delItr);
		delItr = m_mapWaterLData.find(strTime);
	}
}

void YDJUpData_Cache::RemoveDisplaceData( CString strTime )
{
	std::multimap<CString,CDisplace_DataItem*>::iterator delItr = m_mapDisplaceData.find(strTime);
	while(delItr != m_mapDisplaceData.end())
	{
		_DELETE(delItr->second);
		m_mapDisplaceData.erase(delItr);
		delItr = m_mapDisplaceData.find(strTime);
	}
}

void YDJUpData_Cache::RemoveTemperaData( CString strTime )
{
	std::multimap<CString,CTempera_DataItem*>::iterator delItr = m_mapTemperaData.find(strTime);
	while(delItr != m_mapTemperaData.end())
	{
		_DELETE(delItr->second);
		m_mapTemperaData.erase(delItr);
		delItr = m_mapTemperaData.find(strTime);
	}
}


//2023-5-23
void YDJUpData_Cache::RemoveFlowData(CString strTime)
{
	std::multimap<CString, CFlow_DataItem*>::iterator delItr = m_mapFlowData.find(strTime);
	while (delItr != m_mapFlowData.end())
	{
		_DELETE(delItr->second);
		m_mapFlowData.erase(delItr);
		delItr = m_mapFlowData.find(strTime);
	}
}

YDJUpData_Cache::YDJUpData_Cache()
{
	CSystemTimeOp::ResetSystemTime(m_stCacheAreaBegin);
	CSystemTimeOp::ResetSystemTime(m_stCacheAreaEnd);
	CSystemTimeOp::ResetSystemTime(m_stRainBeginTime);
	CSystemTimeOp::ResetSystemTime(m_stRainEndTime);
	CSystemTimeOp::ResetSystemTime(m_stWaterLBeginTime);
	CSystemTimeOp::ResetSystemTime(m_stWaterLEndTime);
	CSystemTimeOp::ResetSystemTime(m_stDisplaceBeginTime);
	CSystemTimeOp::ResetSystemTime(m_stDisplaceEndTime);
	CSystemTimeOp::ResetSystemTime(m_stTemperaBeginTime);
	CSystemTimeOp::ResetSystemTime(m_stTemperaEndTime);
	CSystemTimeOp::ResetSystemTime(m_stFlowBeginTime);
	CSystemTimeOp::ResetSystemTime(m_stFlowEndTime);
}

//2023/5/23
CFlow_DataItem::CFlow_DataItem()
{
	Sunshiflowdata = 0.0;
	Leijiflowdata = 0.0;
	GetLocalTime(&m_RecordTime);
}

CFlow_DataItem::CFlow_DataItem(int data, double Data, SYSTEMTIME& st)
{
	if (data == 0) {
		Sunshiflowdata = Data;
	}
	else {
		Leijiflowdata = Data;
	}
	m_RecordTime = st;
	m_sRecordTime = CSystemTimeOp::ConvertSystemTimeToString(&st, true);
}

CFlow_DataItem::CFlow_DataItem(double SunData, double LeiData, SYSTEMTIME& st)
{
	Sunshiflowdata = SunData;
	Leijiflowdata = LeiData;
	//m_DisplaceNo = iDisplaceNo;
	m_RecordTime = st;
	m_sRecordTime = CSystemTimeOp::ConvertSystemTimeToString(&st, true);
}

CFlow_DataItem::CFlow_DataItem(double SumData, double LeiData, SYSTEMTIME& st, CString strSt)
{
	Sunshiflowdata = SumData;
	Leijiflowdata = LeiData;
	//m_DisplaceNo = iDisplaceNo;
	m_RecordTime = st;
	m_sRecordTime = strSt;
}

CFlow_DataItem::CFlow_DataItem(CFlow_DataItem* pSource)
{
	if (pSource != NULL)
	{
		this->Sunshiflowdata = pSource->Sunshiflowdata;
		this->Leijiflowdata = pSource->Leijiflowdata;
		this->m_sRecordTime = pSource->m_sRecordTime;
		this->m_RecordTime = pSource->m_RecordTime;
	}
}

CString CFlow_DataItem::ToString()
{
	CString strResult = "";
	strResult.Format("%s %f %f", CSystemTimeOp::ConvertSystemTimeToString(&m_RecordTime, true), Sunshiflowdata, Leijiflowdata);
	return strResult;
}

bool CFlow_DataItem::Equals(CFlow_DataItem* pSrc)
{
	bool ret = false;
	if (pSrc != NULL)
	{
		if (CSystemTimeOp::CompareSystemTime(&m_RecordTime, &(pSrc->m_RecordTime)) == 0 &&m_sRecordTime == pSrc->m_sRecordTime &&
			CUdfGlobalFUNC::CompareDouble(Sunshiflowdata, pSrc->Sunshiflowdata, 9, 3) == 0 &&
			CUdfGlobalFUNC::CompareDouble(Leijiflowdata, pSrc->Leijiflowdata, 9, 3) == 0)
			ret = true;
	}
	return ret;
}

bool YDJUpData_Cache::InsertData(CFlow_DataItem* pFlowdata) {
	bool ret = true;
	if (pFlowdata != NULL)
	{

		//判断插入内存的数据是否该在当前内存缓存区内
		int comp_area = CSystemTimeOp::CompareSystemTime(&(pFlowdata->m_RecordTime), &m_stCacheAreaBegin);
		if (comp_area < 0)
			return ret;
		comp_area = CSystemTimeOp::CompareSystemTime(&(pFlowdata->m_RecordTime), &m_stCacheAreaEnd);
		if (comp_area > 0)
			return ret;

		bool blExist = false;
		if (m_mapFlowData.count(pFlowdata->m_sRecordTime) > 0)
		{
			std::multimap<CString, CFlow_DataItem*>::iterator tmpItr;
			std::pair<std::multimap<CString, CFlow_DataItem*>::iterator, std::multimap<CString, CFlow_DataItem*>::iterator> rangeRet;
			rangeRet = m_mapFlowData.equal_range(pFlowdata->m_sRecordTime);
			for (tmpItr = rangeRet.first;tmpItr != rangeRet.second;++tmpItr)
			{
				CFlow_DataItem* pOld = tmpItr->second;
				if (pOld != NULL && pOld->Equals(pFlowdata) == true)
				{
#ifdef DEBUG
					TRACE("InsertData() Exist:%s=%s\r\n", pOld->ToString(), pFlowdata->ToString());
#endif
					blExist = true;
					break;
				}
			}
		}

		if (false == blExist)
		{
			std::multimap<CString, CFlow_DataItem*>::iterator InItr = m_mapFlowData.end();
			InItr = m_mapFlowData.insert(std::pair<CString, CFlow_DataItem*>(pFlowdata->m_sRecordTime, pFlowdata));
			//if (InItr != m_mapTemperaData.end())
			{
				int nCount = m_mapFlowData.size();
				if (1 == nCount)
				{
					m_stFlowBeginTime = pFlowdata->m_RecordTime;
					m_stFlowEndTime = pFlowdata->m_RecordTime;
				}
				else
				{
					int comp = 0;
					comp = CSystemTimeOp::CompareSystemTime(&m_stFlowBeginTime, &(pFlowdata->m_RecordTime));
					if (comp > 0)
						m_stFlowBeginTime = pFlowdata->m_RecordTime;

					comp = CSystemTimeOp::CompareSystemTime(&m_stFlowEndTime, &(pFlowdata->m_RecordTime));
					if (comp < 0)
						m_stFlowEndTime = pFlowdata->m_RecordTime;
				}
			}
		}
	}
	return ret;
}

const CString Table_ST_RSVR_R_Record::ToString()
{
	return CString();
}
