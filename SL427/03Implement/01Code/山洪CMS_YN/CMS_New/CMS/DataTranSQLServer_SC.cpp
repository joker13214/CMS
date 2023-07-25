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
	//修改为Local调用

	m_dCHECK_RAIN_PER		= GETAPP()->m_GlobalOptions.m_dRainCheckPer;
	m_dCHECK_RAIN_DAY		= GETAPP()->m_GlobalOptions.m_dRainCheckDay;

	m_nCheck5Minutes_MNT	= GETAPP()->m_GlobalOptions.m_nCheck5Minutes_Count;
	m_nCheck15Minutes_MNT	= GETAPP()->m_GlobalOptions.m_nCheck15Minutes_Count;
	m_nCheck1Hour_MNT		= GETAPP()->m_GlobalOptions.m_nCheck1Hour_Count;
	m_nChcek1Day_MNT		= GETAPP()->m_GlobalOptions.m_nCheck1Day_Count;

	m_blCalc = false;
	m_sqlite = NULL;
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
	}
	
	ClearAddrMap();
}


void CDataTranSQLServer_SC::Initialization()
{
	AddThreadLog("正在启动数据接口模块:四川省山洪灾害数据接口");

	//if (!OpenSqlite())
	//	return ;//0;

	if (!InitAddressMap())
		return ;

	CheckAddressMap_ForRtu();

	if (OpenDatabase() == true)
	{
		AddThreadLog("连接主站数据库成功.");
		CheckDatabase();
	}
}

void CDataTranSQLServer_SC::StatisticsRain_1Day_OnTimer()
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

void CDataTranSQLServer_SC::Statistics_Data_OnRealTime(COneData_INFO* pData)
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
			CString sRTUID = pData->Get_RTU_ID();
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


long CDataTranSQLServer_SC::ThreadFunction()
{
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
		ret = false;
	}

	return ret;
}

void CDataTranSQLServer_SC::CheckDatabase()
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

bool CDataTranSQLServer_SC::OpenSqlite()
{
	bool ret = false;
	/*
	try
	{
		CCMSApp* pApp = (CCMSApp*)AfxGetApp();
		CString SqliteFile = pApp->m_GlobalOptions.m_strLocalDbName;

		CHAR OLD_PATH[MAX_PATH];
		memset(OLD_PATH,0,sizeof(OLD_PATH));
		::GetCurrentDirectory(MAX_PATH,OLD_PATH);
		::SetCurrentDirectory(CUdfGlobalFUNC::GetRuningPath());

		ret = m_sqlite.Open((LPCTSTR)SqliteFile);	
		::SetCurrentDirectory(OLD_PATH);

		if (ret == false)
			AddThreadLog("连接本地数据库失败.",Error_Log_message);
	}
	catch (...)
	{
		ret = false;
	}*/


	return ret;
}

CSqlStatement* CDataTranSQLServer_SC::ExcuteSQLiteQuery( CString strSQL )
{
	CSqlStatement* stmt =NULL;
	try
	{
#ifdef DEBUG
		TRACE("%s\r\n",strSQL);
#endif
		if (m_sqlite != NULL)
			stmt = m_sqlite->Statement(strSQL);
		//stmt = m_sqlite.Statement(strSQL);
	}
	catch (CException* e)
	{
		e->Delete();
		stmt = NULL;
	}

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
			sSql.Format(_T("SELECT 测站编码,类型,水位站类型,行政区划码,终端地址 FROM [%s$]"),sheetName);
			rs.Open(AFX_DB_USE_DEFAULT_TYPE,sSql);
		
			CString sTemp = "";
			CString sSTCD="",sADDVCode="";
			int     nType = 0, nwlType = 0,nRtuAddr = 0;

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


				CAddrMap_Item* pAddr = new CAddrMap_Item(sSTCD,nType,nwlType,sADDVCode,nRtuAddr);
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
		for (Itr = m_mapAddressMap.begin();
			 Itr != m_mapAddressMap.end();
			 Itr ++)
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
						sRTUID.Format("%02d%02d%02d_%08d",
							pStation->GetProvinceCode(),
							pStation->GetRegionCode(),
							pStation->GetCountyCode(),
							pStation->GetAddrA2());

						//if (sRTUID == Itr->second->Get_RTU_ID())
						if (sRTUID == pAddrItem->Get_RTU_ID())
						{
							blFinded = true;
							break;
						}
					}
				}
				if (false == blFinded)
				{
					//strLog.Format("地址映射检查,警告:测站编码'%s'没有与之对应的遥测终端机(行政区划码:%s,终端地址:%d)",
					//	Itr->second->Get_STCD(),
					//	Itr->second->Get_ADDVCode(),
					//	Itr->second->Get_RtuAddr());

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
		strBegin.Format("%04d-%02d-%02d %02d:00:00.000",
			nowTime.GetYear(),nowTime.GetMonth(),nowTime.GetDay(),nowTime.GetHour());

		strEnd.Format("%04d-%02d-%02d %02d:00:00.000",
			endtime.GetYear(),endtime.GetMonth(),endtime.GetDay(),endtime.GetHour());

#ifdef DEBUG
		SYSTEMTIME dbtime;
		GetLocalTime(&dbtime);
		TRACE("%s 获取小时水位数据, POINT: %s ,[%s ~ %s]\r\n",
			CSystemTimeOp::ConvertSystemTimeToString(&dbtime,true),strPoint,strBegin,strEnd);
#endif

		CString sSQL = "";
		sSQL.Format("SELECT DISTINCT CURRENT_VAL,COLLECT_TIME FROM WATERL_DATA WHERE RTU='%s' AND SENSORS_NO='00' AND COLLECT_TIME  BETWEEN '%s' AND '%s' ORDER BY COLLECT_TIME DESC LIMIT 1",
			pAddrItem->Get_RTU_ID(),strBegin,strEnd);

		CSqlStatement* stmt = ExcuteSQLiteQuery(sSQL);
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
				GetLocalTime(&dbtime);
				TRACE("%s 获取小时水位数据,%s=%f\r\n",
					CSystemTimeOp::ConvertSystemTimeToString(&dbtime,true),sCollectTime,dValue);
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
				StatisticsWaterLevel_RealTime(pAddrItem,pRecvData);
		}
	}
}

//某水位监测点数据实时统计
void CDataTranSQLServer_SC::StatisticsWaterLevel_RealTime( CAddrMap_Item* pAddrItem,CWaterL_DataItem* pRecvData )
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

			UpdateWaterLevelData(pAddrItem,strTM,pRecvData->m_dValue);
		}
		else
		{
			//更新当前点
			CString strTM="";
			strTM.Format("%04d-%02d-%02d %02d:%02d",
				pRecvData->m_RecordTime.wYear,pRecvData->m_RecordTime.wMonth,
				pRecvData->m_RecordTime.wDay,pRecvData->m_RecordTime.wHour,pRecvData->m_RecordTime.wMinute);
			UpdateWaterLevelData(pAddrItem,strTM,pRecvData->m_dValue);


			//更新整点数据
			CTime nowTime(pRecvData->m_RecordTime);
			CTimeSpan ttspan(0,1,0,0);
			CTime HourTime = nowTime + ttspan;
			double HourValue = 0.0;

			strTM.Format("%04d-%02d-%02d %02d:00",
				HourTime.GetYear(),HourTime.GetMonth(),HourTime.GetDay(),
				HourTime.GetHour());

			CWaterL_DataItem* HourWL = GetLatestWaterLevelData(pAddrItem,CSystemTimeOp::ConvertSystemTimeToString(&(pRecvData->m_RecordTime),true));
			if (HourWL != NULL)
			{
				INT64  muls = CSystemTimeOp::MinusSystemTime(HourWL->m_RecordTime,pRecvData->m_RecordTime);
			
				if (muls > 0 )   //查到的值比当前值新
					HourValue = HourWL->m_dValue;
				else
					HourValue = pRecvData->m_dValue;

				UpdateWaterLevelData(pAddrItem,strTM,HourValue);

				_DELETE(HourWL);
			}
			else if (HourWL == NULL)
			{
				HourValue = pRecvData->m_dValue;
				UpdateWaterLevelData(pAddrItem,strTM,HourValue);
			}
		}
	}
}

/*
* 更新 strTM点的数据
* 查找到数据 更新 Update
* 未查找到数据 添加 Insert
*/
bool CDataTranSQLServer_SC::UpdateWaterLevelData( CAddrMap_Item* pAddrItem, CString strTM, double pointValue )
{
	bool ret = true;

	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_WL && strTM != "" && pointValue > 0)
	{
		CString sSQL = "";
		bool isExist = false;       //Record whether exist
		double ValueInDb = 0.0;     //Data In Database
		if (pAddrItem->GetWlType() == WL_Type_HLSW)
		{
			//查询
			sSQL.Format("SELECT Z AS L FROM ST_RIVER_R WHERE STCD='%s' AND TM = '%s'",
				pAddrItem->Get_STCD(),strTM);

			isExist = GetWaterLevelData(sSQL,ValueInDb);

			if (false == isExist)     //INSERT
			{
				sSQL.Format("INSERT INTO ST_RIVER_R (STCD ,TM, Z, WPTN) VALUES ('%s','%s',%.2f,6)",
					pAddrItem->Get_STCD(),strTM,pointValue);
				
				ret = ExcuteNoQuery(sSQL);
			}
			else if (true == isExist) //UPDATE
			{
				//if (ValueInDb != pointValue)
				if (fabs(ValueInDb - pointValue) >= 0.01 )
				{
#ifdef DEBUG
					TRACE("DB:%f\r\nPV:%f\r\n",ValueInDb,pointValue);
#endif

					sSQL.Format("UPDATE ST_RIVER_R SET Z = %.2f WHERE STCD='%s' AND TM = '%s'",
						pointValue,pAddrItem->Get_STCD(),strTM);

					ret = ExcuteNoQuery(sSQL);
				}
			}
		}
		else if (pAddrItem->GetWlType() == WL_Type_SKSSW)
		{
			//查询
			sSQL.Format("SELECT RZ AS L FROM ST_RSVR_R WHERE STCD = '%s' AND TM = '%s'",
				pAddrItem->Get_STCD(),strTM);

			isExist = GetWaterLevelData(sSQL,ValueInDb);

			if (false == isExist)  //INSERT
			{
				sSQL.Format("INSERT INTO ST_RSVR_R (STCD,TM, RZ) VALUES ('%s','%s',%.2f)",
					pAddrItem->Get_STCD(),strTM,pointValue);

				ret = ExcuteNoQuery(sSQL);
			}
			else if (true == isExist) //UPDATE
			{
				//if (ValueInDb != pointValue)
				if (fabs(ValueInDb - pointValue) >= 0.01 )
				{
#ifdef DEBUG
					TRACE("DB:%f\r\nPV:%f\r\n",ValueInDb,pointValue);
#endif
					sSQL.Format("UPDATE ST_RSVR_R SET RZ = %.2f WHERE STCD='%s' AND TM = '%s'",
						pointValue,pAddrItem->Get_STCD(),strTM);

					ret = ExcuteNoQuery(sSQL);
				}
			}
		}
		else if (pAddrItem->GetWlType() == WL_Type_SKXSW)
		{
			//查询
			sSQL.Format("SELECT BLRZ AS L FROM ST_RSVR_R WHERE STCD = '%s' AND TM = '%s'",
				pAddrItem->Get_STCD(),strTM);

			isExist = GetWaterLevelData(sSQL,ValueInDb);

			if (false == isExist)  //INSERT
			{
				sSQL.Format("INSERT INTO ST_RSVR_R (STCD,TM, BLRZ) VALUES ('%s','%s',%.2f)",
					pAddrItem->Get_STCD(),strTM,pointValue);

				ret = ExcuteNoQuery(sSQL);
			}
			else if (true == isExist) //UPDATE
			{
				//if (ValueInDb != pointValue)
				if (fabs(ValueInDb - pointValue) >= 0.01 )
				{
#ifdef DEBUG
					TRACE("DB:%f\r\nPV:%f\r\n",ValueInDb,pointValue);
#endif

					sSQL.Format("UPDATE ST_RSVR_R SET BLRZ = %.2f WHERE STCD='%s' AND TM = '%s'",
						pointValue,pAddrItem->Get_STCD(),strTM);

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
				StatisticsRain_RealTime(/*Itr->second*/pAddrItem,pRecvData);
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

		if (false == isNull)
		{
			if (Minus5Minutes > m_dCHECK_RAIN_PER ||
				Minus5Minutes <= 0.0)
				Minus5Minutes = 0.0;

			if ( Minus5Minutes >= (double)0.1  )
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

				UpdateRainfallTableRecord(data_record,Rain_statistics_5minutes);
			}
			else  //delete record
			{
				CString strSQL = "";
				strSQL.Format("DELETE FROM ST_PPTN_R WHERE STCD='%s' AND TM='%s' AND INTV=0.05",pAddrItem->Get_STCD(),strPoint);
				ExcuteNoQuery(strSQL);
			}
		}

		//update by repair data
		SYSTEMTIME repair_begintime, repair_endtime;
		CTime basic_ctime(endtime);

		repair_begintime = endtime;
		CTime _temp_ctime = basic_ctime + _t_add5_span;
		CSystemTimeOp::ConvertCTimeToSystemtime(&_temp_ctime,&repair_endtime);

        UINT _t_repair_check = 0;
		while (_t_repair_check <= m_nCheck5Minutes_MNT)
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

            bool repair_isNull = false;
			repair_5minute_rain = CalRainData_OnTimeRange(pAddrItem,str_repair_begin,str_repair_end,Rain_statistics_5minutes,repair_isNull);

			if (false == repair_isNull)
			{
				if (repair_5minute_rain > m_dCHECK_RAIN_PER ||
					repair_5minute_rain < 0.0)
					repair_5minute_rain = 0.0;

				if ( repair_5minute_rain >= (double)0.1  )
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

					UpdateRainfallTableRecord(data_record,Rain_statistics_5minutes);
				}
				else  //delete record
				{
					CString strSQL = "";
					strSQL.Format("DELETE FROM ST_PPTN_R WHERE STCD='%s' AND TM='%s' AND INTV=0.05",pAddrItem->Get_STCD(),str_repair_point);
					ExcuteNoQuery(strSQL);
				}
			}

			//Next calc
			repair_begintime = repair_endtime;
			CTime repair_begin_ctime(repair_begintime);
			CTime repair_end_ctime = repair_begin_ctime + _t_add5_span;
			CTimeSpan _repair_tspan = repair_begin_ctime - basic_ctime;
			_t_repair_check = _repair_tspan.GetTotalMinutes();
			CSystemTimeOp::ConvertCTimeToSystemtime(&repair_end_ctime,&repair_endtime);
		}
	}
}

double CDataTranSQLServer_SC::CalRainData_OnTimeRange(CAddrMap_Item* pAddrItem, 
													  CString strBegin, 
													  CString strEnd, 
													  RIANSTATISTICS_TYPE_Enum xType, 
													  bool& isNull)
{
	double _RainData = 0.0;
	if (pAddrItem && pAddrItem->GetType() == CZ_Type_RAIN && strBegin != "" && strEnd != "")
	{

#ifdef DEBUG
		CString calcTypeName[] ={"5分钟统计",
							 "15分钟统计",
							 "小时统计",
							 "日统计"};
		SYSTEMTIME _debug_systemtime;
		GetLocalTime(&_debug_systemtime);
		TRACE("%s Function:CalRainData_OnTimeRange\r\n",CSystemTimeOp::ConvertSystemTimeToString(&_debug_systemtime,true));		
		TRACE("\t\tType :%d,Type's Name:%s\r\n",(int)xType,calcTypeName[int(xType)]);
		TRACE("\t\tBegin:%s\r\n",strBegin);
		TRACE("\t\tEnd  :%s\r\n",strEnd);
		TRACE("\t\tSTCD:%s,RTU:%s\r\n",pAddrItem->Get_STCD(),pAddrItem->Get_RTU_ID());
#endif

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
		int nRowsCount = 0;

		CSqlStatement* sqlite_rs = ExcuteSQLiteQuery(sSQL);
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
				SYSTEMTIME begintime;
				CSystemTimeOp::ConvertStringToSysemTime(strBegin,&begintime);

				CTime begin_ctime(begintime);
				CTime before_ctime(pBeforeRow->m_tCollectTime);

				CTimeSpan muts = begin_ctime - before_ctime;
				if (muts.GetTotalMinutes() > check_time_value)
				{
					_DELETE(pBeforeRow);
					pBeforeRow = NULL;
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
					_RainData = pLastRow->SumHalfHour();
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
					if (pLastRow->TotalValue() >= pFirstRow->TotalValue())
						_RainData = pLastRow->TotalValue() - pFirstRow->TotalValue();
					else
						_RainData = RainFullScale_CONST - pFirstRow->TotalValue() + pLastRow->TotalValue();
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

		if (false == isNull)
		{
			if (Minus15Minutes > m_dCHECK_RAIN_PER ||
				Minus15Minutes < 5.0)
				Minus15Minutes = 0.0;

			if (Minus15Minutes >= (double)5.0)
			{
				Table_ST_STORM_R_Record sRecord;

				sRecord.STCD	= pAddrItem->Get_STCD();
				sRecord.TM		= strPoint;
				sRecord.STRMP	= Minus15Minutes;

				sRecord.STRMDR	= 0.15;
				sRecord.WTH		= "7";

				UpdateRainstormTableRecord(sRecord);
			}
			else // delete
			{
				CString strSQL ="";
				strSQL.Format("DELETE FROM ST_STORM_R WHERE STCD='%s' AND TM='%s' AND STRMDR = 0.15",
					pAddrItem->Get_STCD(),strPoint);

				ExcuteNoQuery(strSQL);
			}
		}

		//update by repair data
		SYSTEMTIME repair_begintime, repair_endtime;
		CTime basic_ctime(endtime);

		repair_begintime = endtime;
		CTime _temp_ctime = basic_ctime + _tspan_15;
		CSystemTimeOp::ConvertCTimeToSystemtime(&_temp_ctime,&repair_endtime);

		UINT _t_repair_check = 0;
		while (_t_repair_check <= m_nCheck15Minutes_MNT)
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

			bool repair_isNull = false;
			repair_15minute_rain = CalRainData_OnTimeRange(pAddrItem,str_repair_begin,str_repair_end,Rain_statistics_15minutes,repair_isNull);

			if (false == repair_isNull)
			{
				if (repair_15minute_rain > m_dCHECK_RAIN_PER ||
					repair_15minute_rain < 5.0)
					repair_15minute_rain = 0.0;

				if (repair_15minute_rain >= (double)5.0)
				{
					Table_ST_STORM_R_Record sRecord;

					sRecord.STCD	= pAddrItem->Get_STCD();
					sRecord.TM		= str_repair_point;
					sRecord.STRMP	= repair_15minute_rain;

					sRecord.STRMDR	= 0.15;
					sRecord.WTH		= "7";

					UpdateRainstormTableRecord(sRecord);
				}
				else // delete record
				{
					CString strSQL ="";
					strSQL.Format("DELETE FROM ST_STORM_R WHERE STCD='%s' AND TM='%s' AND STRMDR = 0.15",
						pAddrItem->Get_STCD(),str_repair_point);

					ExcuteNoQuery(strSQL);
				}
			}

			//Next
			repair_begintime		= repair_endtime;
			CTime repair_begin_ctime(repair_begintime);
			CTime repair_end_ctime	= repair_begin_ctime + _tspan_15;
			CTimeSpan _repair_tspan = repair_begin_ctime - basic_ctime;
			_t_repair_check			= _repair_tspan.GetTotalMinutes();
			CSystemTimeOp::ConvertCTimeToSystemtime(&repair_end_ctime,&repair_endtime);
		}
	}
}

void CDataTranSQLServer_SC::StatisticsRain_1Hour( CAddrMap_Item* pAddrItem,CRainData_Item* pRecvData )
{
	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_RAIN && pRecvData != NULL)
	{
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
		bool   isNull = false;
		Minus1Hour = CalRainData_OnTimeRange(pAddrItem,strBegin,strEnd,Rain_statistics_1hour,isNull);

		//calc rain minutes 
		if (false == isNull)
		{
			if (Minus1Hour >  m_dCHECK_RAIN_PER ||
				Minus1Hour <= 0.0)
				Minus1Hour = 0.0f;

			int RainMinutes = 0;
			if (Minus1Hour >= (double)0.1)
			{
				RainMinutes = GetRainMinutes(pAddrItem,strBegin,strEnd);
				if (RainMinutes <= 0 || RainMinutes > 60)
					RainMinutes = 60;	
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
					if (RainMinutes >= 60 || RainMinutes <= 0 )
						sRecord.PRD = 0.00;
					else
						sRecord.PRD = (double)RainMinutes /100;

					UpdateRainfallTableRecord(sRecord,Rain_statistics_1hour);
				}
				else
				{
					CString sSQL = "";
					sSQL.Format("DELETE FROM ST_PPTN_R WHERE STCD='%s' AND TM='%s' AND INTV=1.00",
						pAddrItem->Get_STCD(),
						strPoint);
					ExcuteNoQuery(sSQL);
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
					if (RainMinutes >= 60 || RainMinutes <= 0 )
						srecord.PRD = 0.00;
					else
						srecord.PRD = (double)RainMinutes /100;
				}
				else if (Minus1Hour < (double)0.1)
					srecord.WTH = "8";
				
				UpdateRainfallTableRecord(srecord,Rain_statistics_AM8Point);
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
					repair_RainMinutes = GetRainMinutes(pAddrItem,str_repair_begin,str_repair_end);
					if (repair_RainMinutes <= 0 || repair_RainMinutes > 60)
						repair_RainMinutes = 60;	
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
						if (repair_RainMinutes >= 60 || repair_RainMinutes <= 0 )
							sRecord.PRD = 0.00;
						else
							sRecord.PRD = (double)repair_RainMinutes /100;

						UpdateRainfallTableRecord(sRecord,Rain_statistics_1hour);
					}
					else
					{
						CString sSQL = "";
						sSQL.Format("DELETE FROM ST_PPTN_R WHERE STCD='%s' AND TM='%s' AND INTV=1.00",pAddrItem->Get_STCD(),str_repair_point);
						ExcuteNoQuery(sSQL);
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
						if (repair_RainMinutes >= 60 || repair_RainMinutes <= 0 )
							srecord.PRD = 0.00;
						else
							srecord.PRD = (double)repair_RainMinutes /100;
					}
					else if (repair_rain_data < (double)0.1)
						srecord.WTH = "8";

					UpdateRainfallTableRecord(srecord,Rain_statistics_AM8Point);
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

		//1 hour begin time and end time
		//SYSTEMTIME hourbegintime,hourendtime;
		//SYSTEMTIME temphourtime = collecttime;
		//temphourtime.wMinute = 0;
		//temphourtime.wSecond = 1;
		//temphourtime.wMilliseconds = 0;
		//CTime time_hour(temphourtime);
		//CTimeSpan _tspan_1hour(0,1,0,0);
		//if (collecttime.wMinute == 0 && 
		//	collecttime.wSecond == 0 )
		//{
		//	hourendtime = temphourtime;
		//	CTime hour_beginctime = time_hour - _tspan_1hour;
		//	CSystemTimeOp::ConvertCTimeToSystemtime(&hour_beginctime,&hourbegintime);
		//}
		//else
		//{
		//	hourendtime = temphourtime;
		//	CTime hour_endctime = time_hour + _tspan_1hour;
		//	CSystemTimeOp::ConvertCTimeToSystemtime(&hour_endctime,&hourendtime);
		//}


		//-----------------------------------------------------------------------------------------
		CTime basic_ctime(begintime);
		UINT  m_check_time = 0;

		while (m_check_time <= m_nChcek1Day_MNT + tspan.GetTotalMinutes())
		{
			strBegin = CSystemTimeOp::ConvertSystemTimeToString(&begintime,true);
			strEnd   = CSystemTimeOp::ConvertSystemTimeToString(&endtime,true);
			strPoint.Format("%04d-%02d-%02d 08:00",endtime.wYear,endtime.wMonth,endtime.wDay);

			double day_rain = 0;
			bool  isNullDay = false;
			day_rain = CalRainData_OnTimeRange(pAddrItem,strBegin,strEnd,Rain_statistics_AM8Point,isNullDay);

			if (false == isNullDay)
			{
				if (day_rain <=0.0 || day_rain > m_dCHECK_RAIN_DAY)
					day_rain = 0.0;


				CString strHourBegin="",strHourEnd = "";
				strHourEnd = strEnd;
				strBegin.Format("%04d-%02d-%02d 07:00:01.000",endtime.wYear,endtime.wMonth,endtime.wDay);

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
						HourRainMinutes = GetRainMinutes(pAddrItem,strHourBegin,strHourEnd);
						if (HourRainMinutes <= 0 || HourRainMinutes > 60)
							HourRainMinutes = 60;	
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
						if (HourRainMinutes >= 60 || HourRainMinutes <= 0 )
							srecord.PRD = 0.00;
						else
							srecord.PRD = (double)HourRainMinutes /100;
					}
					else if (hour_rain < (double)0.1)
						srecord.WTH = "8";

					UpdateRainfallTableRecord(srecord,Rain_statistics_AM8Point);

				}
				else if (true == isNullHour)
				{
					Table_ST_PPTN_R_Record srecord;
					srecord.STCD = pAddrItem->Get_STCD();
					srecord.TM	 = strPoint;
					srecord.DYP  = day_rain;
					srecord.WTH = "8";

					UpdateRainfallTableRecord(srecord,Rain_statistics_AM8Point);
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

double CDataTranSQLServer_SC::StatisticsRain_1Day(CAddrMap_Item* pAddrItem,CString strBegin,CString strEnd,CRainData_Item* pRecvData/* =NULL */)
{
	double Rain1Days = 0.0;
	if (pAddrItem != NULL && pAddrItem->GetType() == CZ_Type_RAIN && strBegin != "" && strEnd != "")
	{
		SYSTEMTIME begintime,endtime;
		CSystemTimeOp::ConvertStringToSysemTime(strBegin,&begintime);
		CSystemTimeOp::ConvertStringToSysemTime(strEnd,&endtime);

		CString sSQL = "";
		//sSQL.Format("SELECT DISTINCT DATA_TYPE,CURRENT_VAL,HALFH_VAL,TOTAL_VAL,COLLECT_TIME FROM RAIN_DATA WHERE RTU='%s' AND SENSORS_NO='00' AND COLLECT_TIME BETWEEN '%s' AND '%s' ORDER BY COLLECT_TIME"
		//	,pAddrItem->Get_RTU_ID(),strBegin,strEnd);
		sSQL.Format("SELECT DISTINCT DATA_TYPE,CURRENT_VAL,HALFH_VAL,TOTAL_VAL,COLLECT_TIME FROM RAIN_DATA WHERE RTU='%s' AND SENSORS_NO='00' AND COLLECT_TIME >= '%s' AND COLLECT_TIME < '%s' ORDER BY COLLECT_TIME"
			,pAddrItem->Get_RTU_ID(),strBegin,strEnd);

		CSqlStatement* rs = ExcuteSQLiteQuery(sSQL);
		if (rs != NULL)
		{
			CString sTemp = "";
			BYTE DataType =0;
			double CurrentValue =0.0,HalfHourValue = 0.0,TotalValue =0.0;
			CString CollTimeString = "";

			CRainData_Item* pBeginData = NULL;
			CRainData_Item* pEndData = NULL;

			int nRowsCount = 0;
			while (rs->NextRow())
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

				if (nRowsCount == 0)
					pBeginData =new  CRainData_Item(DataType,CurrentValue,HalfHourValue,TotalValue,CollTimeString);

				_DELETE(pEndData);
				pEndData = new  CRainData_Item(DataType,CurrentValue,HalfHourValue,TotalValue,CollTimeString);

				nRowsCount ++;
			}

			//if (pRecvData == NULL)
			{
				if (nRowsCount == 0)
				{
					Rain1Days = 0.0;
				}
				else if (nRowsCount >= 1)
				{
					int compareTime = 0;
					if (pBeginData->m_tCollectTime.wYear	== begintime.wYear && 
						pBeginData->m_tCollectTime.wMonth	== begintime.wMonth && 
						pBeginData->m_tCollectTime.wDay		== begintime.wDay &&
						pBeginData->m_tCollectTime.wHour	== begintime.wHour && 
						pBeginData->m_tCollectTime.wMinute	== begintime.wMinute &&
						pBeginData->m_tCollectTime.wSecond	== begintime.wSecond)
						compareTime = 0;
					else
						compareTime = 1;

					if (nRowsCount == 1)
					{
						if (compareTime == 0)   // == begin time
							Rain1Days = 0.0;
						else //  = 1            > begin time
						{
							_DELETE(pBeginData);
							pBeginData = GetEndTimeRainData(pAddrItem,strBegin);
							if (pBeginData != NULL)
							{

								CTime tdata(pBeginData->m_tCollectTime);
								CTime tbegin(begintime);
								CTimeSpan _tspan = tbegin - tdata;
								if (_tspan.GetTotalMinutes() <= m_nChcek1Day_MNT)
								{
									if (pEndData->TotalValue() >= pBeginData->TotalValue())
										Rain1Days = pEndData->TotalValue() - pBeginData->TotalValue();
									else
									{
										if (fabs(RainFullScale_CONST - pBeginData->TotalValue()) < m_dCHECK_RAIN_DAY /*500*/)
										{
											Rain1Days = RainFullScale_CONST - pBeginData->TotalValue()  + pEndData->TotalValue();
										}
										else
										{
											Rain1Days = pEndData->SumHalfHour();
										}
									}
								}
								else
								{
									Rain1Days = pEndData->SumHalfHour();
								}
							}
							else if (pBeginData == NULL)
							{
								Rain1Days = pEndData->SumHalfHour();
							}
						}
					}
					else  //nrowsCount > 1
					{
						 if (compareTime == 0)   //= begin time
						 {
							 if (pEndData->TotalValue() >= pBeginData->TotalValue())
								 Rain1Days = pEndData->TotalValue() - pBeginData->TotalValue();
							 else
							 {
									if (fabs(RainFullScale_CONST - pBeginData->TotalValue()) < m_dCHECK_RAIN_DAY/*500*/)
									{
										Rain1Days = RainFullScale_CONST - pBeginData->TotalValue()  + pEndData->TotalValue();
									}
									else
									{
										Rain1Days = pEndData->SumHalfHour();
									}
							 }
						 }
						 else   //compare == 1 > begin time
						 {
							 CRainData_Item* ptempBeginData = GetEndTimeRainData(pAddrItem,strBegin);
							 if (ptempBeginData != NULL)
							 {
								 CTime tdata(ptempBeginData->m_tCollectTime);
								 CTime tbegin(begintime);
								 CTimeSpan _tspan = tbegin - tdata;
								 if (_tspan.GetTotalMinutes() <= m_nChcek1Day_MNT)
								 {
									 _DELETE(pBeginData);
									 pBeginData = ptempBeginData;
								 }
								 else
								 {
									 _DELETE(ptempBeginData);
								 }
							 }

							 if (pEndData->TotalValue() >= pBeginData->TotalValue())
								 Rain1Days = pEndData->TotalValue() - pBeginData->TotalValue();
							 else
							 {
								 if (fabs(RainFullScale_CONST - pBeginData->TotalValue()) < m_dCHECK_RAIN_DAY/*500*/)
									{
										Rain1Days = RainFullScale_CONST - pBeginData->TotalValue()  + pEndData->TotalValue();
									}
									else
									{
										Rain1Days = pEndData->SumHalfHour();
									}
							 }
						 }
					}
				}
			}

			//else if (pRecvData != NULL)
			//{
			//	if (nRowsCount == 0)
			//	{
			//		pEndData = new CRainData_Item(pRecvData->DataType(),pRecvData->CurrentValue(),pRecvData->SumHalfHour(),pRecvData->TotalValue(),pRecvData->CollectTimeString());
			//		pBeginData = GetEndTimeRainData(pAddrItem,strBegin);

			//	}
			//	else if (nRowsCount > 0)
			//	{
			//		int compareBeginTime = CSystemTimeOp::CompareSystemTime(&(pBeginData->m_tCollectTime),&begintime);
			//		int compareEndTime   = CSystemTimeOp::CompareSystemTime(&(pEndData->m_tCollectTime),&(pRecvData->m_tCollectTime));

			//		if (nRowsCount == 1)
			//		{
			//			if (compareBeginTime == 0)
			//			{
			//				if (compareEndTime  == -1 )
			//				{
			//					_DELETE(pEndData);
			//					pEndData = new CRainData_Item(pRecvData->DataType(),pRecvData->CurrentValue(),pRecvData->SumHalfHour(),pRecvData->TotalValue(),pRecvData->CollectTimeString());
			//				}
			//			}
			//			else if (compareBeginTime == 1)
			//			{
			//				if (compareEndTime == -1)
			//				{
			//					_DELETE(pEndData);
			//					pEndData = new CRainData_Item(pRecvData->DataType(),pRecvData->CurrentValue(),pRecvData->SumHalfHour(),pRecvData->TotalValue(),pRecvData->CollectTimeString());

			//				}
			//				else if (compareEndTime == 1)
			//				{
			//					_DELETE(pBeginData);
			//					pBeginData = new CRainData_Item(pRecvData->DataType(),pRecvData->CurrentValue(),pRecvData->SumHalfHour(),pRecvData->TotalValue(),pRecvData->CollectTimeString());
			//				}
			//				else if (compareEndTime == 0)
			//				{
			//					CRainData_Item* ptempBeginData = GetEndTimeRainData(pAddrItem,strBegin);
			//					if (ptempBeginData != NULL)
			//					 {
			//						 _DELETE(pBeginData);
			//						 pBeginData = ptempBeginData;
			//					 }
			//				}
			//			}
			//		}
			//		else if (nRowsCount > 1)
			//		{
			//			if (compareBeginTime == 0)  // == begin time
			//			{
			//				if (compareEndTime == -1)  //ent < recvtime
			//				{
			//					_DELETE(pEndData);
			//					pEndData =  new CRainData_Item(pRecvData->DataType(),pRecvData->CurrentValue(),pRecvData->SumHalfHour(),pRecvData->TotalValue(),pRecvData->CollectTimeString());
			//				}
			//			}
			//			else if (compareBeginTime == 1)  // > begin time
			//			{
			//				int comp = CSystemTimeOp::CompareSystemTime(&(pBeginData->m_tCollectTime),&(pRecvData->m_tCollectTime));
			//				if (comp ==0 || comp == -1)
			//				{
			//					CRainData_Item* ptempBeginData = GetEndTimeRainData(pAddrItem,strBegin);
			//					if (ptempBeginData != NULL)
			//					{
			//						_DELETE(pBeginData);
			//						pBeginData = ptempBeginData;
			//					}
			//				}
			//				else if (comp == 1)
			//				{
			//					CRainData_Item* ptempBeginData = GetEndTimeRainData(pAddrItem,strBegin);
			//					if (ptempBeginData != NULL)
			//					{
			//						_DELETE(pBeginData);
			//						pBeginData = ptempBeginData;
			//					}
			//					else if (ptempBeginData == NULL)
			//					{
			//						_DELETE(pBeginData);
			//						pBeginData = new CRainData_Item(pRecvData->DataType(),pRecvData->CurrentValue(),pRecvData->SumHalfHour(),pRecvData->TotalValue(),pRecvData->CollectTimeString());
			//					}
			//				}

			//				if (compareEndTime == -1)
			//				{
			//					_DELETE(pEndData);
			//					pEndData = new CRainData_Item(pRecvData->DataType(),pRecvData->CurrentValue(),pRecvData->SumHalfHour(),pRecvData->TotalValue(),pRecvData->CollectTimeString());
			//				}
			//			}
			//		}
			//	}

			//	//Calc 1 day rain fall
			//	if (pBeginData && pBeginData)
			//	{
			//		if (pEndData->TotalValue() >= pBeginData->TotalValue())
			//			Rain1Days = pEndData->TotalValue() - pBeginData->TotalValue();
			//		else
			//		{
			//			if (fabs(RainFullScale - pBeginData->TotalValue()) < m_dCHECK_RAIN_DAY/*500*/)
			//			{
			//				Rain1Days = RainFullScale - pBeginData->TotalValue()  + pEndData->TotalValue();
			//			}
			//			else
			//			{
			//				Rain1Days = pEndData->SumHalfHour();
			//			}
			//		}
			//	}
			//	else
			//	{
			//		if (pBeginData != NULL)
			//			Rain1Days = pBeginData->SumHalfHour();

			//		if (pEndData != NULL)
			//			Rain1Days = pEndData->SumHalfHour();					
			//	}
			//	//End Calc
			//}  //else if (pRecvData != NULL)

			_DELETE(pBeginData);
			_DELETE(pEndData);
			_DELETE(rs);

			if (Rain1Days < 0.0)
				Rain1Days = 0.0;

		} //if (rs != NULL)
	}
	return Rain1Days;
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


	sSQL.Format("SELECT STCD, TM, DRP, INTV, PDR, DYP, WTH FROM ST_PPTN_R WHERE STCD='%s' AND TM='%s'",
		sRecord.STCD,sRecord.TM);

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
			sSQL.Format("INSERT INTO ST_PPTN_R (STCD, TM, DRP, INTV, WTH) VALUES ('%s','%s',%.1f,0.05,'7')",
				sRecord.STCD,sRecord.TM,sRecord.DRP);
		}
		else if (true == blFinded)
		{
			if (fabs(sRecord.DRP - findRecord.DRP) >= 0.1f)
			{
				sSQL.Format("UPDATE ST_PPTN_R SET DRP = %.1f WHERE STCD='%s' AND TM='%s'",
					sRecord.DRP,sRecord.STCD,sRecord.TM);
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
			sSQL.Format("DELETE FROM ST_PPTN_R WHERE STCD='%s' AND TM='%s' AND INTV=1.00",
				sRecord.STCD,sRecord.TM);
			ExcuteNoQuery(sSQL);
		}

		if (sRecord.PRD > 0.00 && sRecord.PRD < 0.60 )
		{
			sSQL.Format("INSERT INTO ST_PPTN_R (STCD, TM, DRP, INTV, PDR, WTH) VALUES ('%s','%s',%.1f,1.00,%.2f,'7')",
				sRecord.STCD,sRecord.TM,sRecord.DRP,sRecord.PRD);
		}
		else if (sRecord.PRD <= 0.00 || sRecord.PRD >= 0.60 )
		{
			sSQL.Format("INSERT INTO ST_PPTN_R (STCD, TM, DRP, INTV,WTH) VALUES ('%s','%s',%.1f,1.00,'7')",
				sRecord.STCD,sRecord.TM,sRecord.DRP);
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
			sSQL.Format("DELETE FROM ST_PPTN_R WHERE STCD='%s' AND TM='%s'",sRecord.STCD,sRecord.TM);
			ExcuteNoQuery(sSQL);
		}

		sSQL ="";
		
		if (sRecord.WTH == "7")
		{
			if (sRecord.PRD > 0.00 && sRecord.PRD < 0.60 )
			{
				sSQL.Format("INSERT INTO ST_PPTN_R (STCD, TM, DRP, INTV, PDR, DYP,WTH) VALUES ('%s','%s',%.1f,1.00,%.2f,%.1f,'7')",
					sRecord.STCD,sRecord.TM,sRecord.DRP,sRecord.PRD,sRecord.DYP);
			}
			else if (sRecord.PRD <= 0.00 || sRecord.PRD >= 0.60 )
			{
				sSQL.Format("INSERT INTO ST_PPTN_R (STCD, TM, DRP, INTV,DYP,WTH) VALUES ('%s','%s',%.1f,1.00,%.1f,'7')",
					sRecord.STCD,sRecord.TM,sRecord.DRP,sRecord.DYP);
			}
		}
		else if (sRecord.WTH == "8")
		{
			sSQL.Format("INSERT INTO ST_PPTN_R(STCD,TM,DYP,WTH) VALUES ('%s','%s',%.1f,'8')",
				sRecord.STCD,sRecord.TM,sRecord.DYP);
		}
		ret = ExcuteNoQuery(sSQL);
	}

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
		sSQL.Format("SELECT STCD, TM, STRMDR, STRMP, WTH FROM ST_STORM_R WHERE STCD='%s' AND TM='%s'",
			sRecord.STCD,sRecord.TM);

		Table_ST_STORM_R_Record findRecord;
		bool blFinded = false;
		blFinded = GetRainstormRecord(sSQL,findRecord);

		//更新暴雨表信息
		sSQL = "";
		if (true == blFinded)
		{
			if (fabs(sRecord.STRMP - findRecord.STRMP) >= 0.1f)
			{
				sSQL.Format("UPDATE ST_STORM_R SET STRMP=%.1f WHERE STCD='%s' AND TM='%s'",
					sRecord.STRMP,sRecord.STCD,sRecord.TM);
			}
		}
		else if (false == blFinded)
		{
			sSQL.Format("INSERT INTO ST_STORM_R (STCD, TM, STRMDR, STRMP, WTH ) VALUES ('%s','%s',0.15,%.1f,'7')",
				sRecord.STCD,sRecord.TM,sRecord.STRMP);
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
	if (sqlite )
		m_sqlite = sqlite;
}

CAddrMap_Item::CAddrMap_Item()
{
	m_RTU_ID = "";
	m_STCD = "";
	m_Type = CZ_Type_RAIN;
	m_wlType = WL_Type_HLSW;

	m_addVCode = "";
	m_RtuAddr = 0;

	m_blCalc = false;
	m_blCalcHalf = false;

	m_nCalcIndex = 0;
}

CAddrMap_Item::CAddrMap_Item( CString stCd, int Type, int wlType, CString AddVCode, int RTUAddr )
{
	m_STCD = stCd;

	m_addVCode = AddVCode;
	m_RtuAddr = RTUAddr;

	m_RTU_ID.Format("%s_%08d",AddVCode,RTUAddr);
	m_Type = (CZ_Type_Enum)Type;
	m_wlType = (WL_Type_Enum)wlType;
	
	m_blCalc = false;
	m_blCalcHalf = false;

	m_nCalcIndex = 0;
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

const CString CRainData_Item::ToString()
{
	CString sResult = "";
	sResult.Format("DataType=%d,CurrentValue=%.2f,SumHalfHour= %.2f,TotalValue=%.2f,RecordTime=%s",
		m_nDataType,m_dCurrentValue,m_dSumHalfHour,m_dTotalValue,m_sCollectTime);
	return sResult;
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
