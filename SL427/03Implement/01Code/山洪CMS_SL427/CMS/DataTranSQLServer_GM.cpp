#include "StdAfx.h"
#include "DataTranSQLServer_GM.h"
#include "UdfRtuDataSave.h"
#include "CMS.h"
#include "SystemTimeOp.h"
#include "WebService.h"

CDataTranSQLServer_GM::CDataTranSQLServer_GM()
{
	m_ThreadType = DataTran_SQLServer_GM;
	m_bNeedData = true;
	m_nInterfaceType = 1;
	m_strWebServiceURL ="";
	m_strWebServiceUsername = "";
	m_strWebServicePassword = "";

	m_nDatabaseType = GETAPP()->m_GlobalOptions.m_DataBaseType;
}

CDataTranSQLServer_GM::~CDataTranSQLServer_GM()
{
	//线程已经停止
	//存储没有入库的数据信息
	while (!m_lstData.IsEmpty())
	{
		COneData_INFO* pData = m_lstData.RemoveHead();
		if (pData)
		{
			if ( (m_nInterfaceType== Database_Interface && GetConnectionState() == true)||
				 (m_nInterfaceType == WebService_Interface)) //数据库打开的时 存储到数据库中
			{
				switch (pData->mDataType)
				{
				case COneData_INFO::OPERATE_RECORD:
					SaveOperateRecord(pData);
					break;
				case COneData_INFO::WATER_L_DATA:
					SaveWaterLevelData(pData);
					break;	
				case COneData_INFO::WATER_P_DATA:	
					SaveWaterPressData(pData);
					break;
				case COneData_INFO::FLOW_V_DATA:		
					SaveWaterFlowData(pData);
					break;
				case COneData_INFO::WATER_Q_DATA:
					SaveWaterQualityData(pData);
					break;	
				case COneData_INFO::RAIN_DATA:		
					SaveRainData(pData);
					break;
				case COneData_INFO::ANALOG_DATA:
					SaveAnalogData(pData);
					break;	
				case COneData_INFO::PULSE_DATA:
					SavePulseData(pData);
					break;
				case COneData_INFO::ALARM_RECORD:  
					SaveAlarmRecord(pData);
					break;
				case COneData_INFO::DISPLACE_DATA:
					SaveDisplaceData(pData);
					break;
				case COneData_INFO::TEMPERA_DATA:
					SaveTemperaData(pData);
					break;
				}
			}

			delete pData;
			pData = NULL;
		}
	}
}

void CDataTranSQLServer_GM::Initialization()
{
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	m_nInterfaceType = pApp->m_GlobalOptions.m_DataIType;

	if (Database_Interface == m_nInterfaceType)
	{
		if (OpenDatabase() == true)
		{
			AddThreadLog("连接数据库成功.");
			CheckDatabase();
		}
	}
	else if (WebService_Interface == m_nInterfaceType)
	{
		m_strWebServiceURL = pApp->m_GlobalOptions.m_DbOpts.m_strMISDSN;
		m_strWebServiceUsername = pApp->m_GlobalOptions.m_DbOpts.m_strMISUID;
		m_strWebServicePassword = pApp->m_GlobalOptions.m_DbOpts.m_strMISPWD;
	}


	AddThreadLog("数据上传接口模块启动成功.");

}

long CDataTranSQLServer_GM::ThreadFunction()
{

	AddThreadLog("正在启动数据接口模块:交大光芒定义数据接口");

    Initialization();

	SetThreadControl(true);
	SetThreadState(THREAD_STATE_RUNNING);
	while(GetThreadControl() != false)
	{
		if (GetDataCount() <= 0 )
		{
			Sleep(1);
			continue;
		}

		 //数据库打开的时 存储到数据库中
		if ( (m_nInterfaceType == Database_Interface && GetConnectionState() == true ) ||
			 (m_nInterfaceType == WebService_Interface ))
		{
			COneData_INFO* pData =GetOneData();
			if (pData != NULL)  
			{
				switch (pData->mDataType)
				{
				case COneData_INFO::OPERATE_RECORD:
					SaveOperateRecord(pData);
					break;
				case COneData_INFO::WATER_L_DATA:
					SaveWaterLevelData(pData);
					break;	
				case COneData_INFO::WATER_P_DATA:	
					SaveWaterPressData(pData);
					break;
				case COneData_INFO::FLOW_V_DATA:		
					SaveWaterFlowData(pData);
					break;
				case COneData_INFO::WATER_Q_DATA:
					SaveWaterQualityData(pData);
					break;	
				case COneData_INFO::RAIN_DATA:		
					SaveRainData(pData);
					break;
				case COneData_INFO::ANALOG_DATA:
					SaveAnalogData(pData);
					break;		
				case COneData_INFO::PULSE_DATA:
					SavePulseData(pData);
					break;
				case COneData_INFO::ALARM_RECORD:  
					SaveAlarmRecord(pData);
					break;
				case COneData_INFO::DISPLACE_DATA:
					SaveDisplaceData(pData);
					break;	
				case COneData_INFO::TEMPERA_DATA:
					SaveTemperaData(pData);
					break;	
				}

				//删除pData
				delete pData;
				pData = NULL;
			}
		}
		else    //数据库没有连接或断开时、重新连接
		{
			if (OpenDatabase() == true)
			{
				CString strMsg  = "";
				strMsg.Format("连接数据库成功.");
				AddThreadLog(strMsg);

				CheckDatabase();
			}
		}
		if (GetConnectionState() == true)
			Sleep(0);
		else
			Sleep(1);
	}
	ResetThread();

	return 0;
}


bool CDataTranSQLServer_GM::SaveWaterLevelData( COneData_INFO* pData)
{

	CString strSQL="";
	bool blResult = true;
	if(pData != NULL)
	{
		int nSensorsNo = 0;
		while (!pData->m_lstWaterLevel.IsEmpty())
		{
			CWaterLevel_PAR_PER* pLevel = pData->m_lstWaterLevel.RemoveHead();
			if (pLevel)
			{
				strSQL.Format("INSERT INTO WATERL_DATA (RTU,SENSORS_NO,CURRENT_VAL,COLLECT_TIME,SAVE_TIME) VALUES('%s','%02d','%.02f','%s','%s');",
					pData->Get_RTU_ID(),
					nSensorsNo,
					pLevel->GetRealValue(),
					pData->Get_CollectTime_String(),
					pData->Get_RecvTime_String());

				//if (pDb)
				//	pDb->ExecuteSQL(strSQL);
				blResult = TransmissionSQL(strSQL);
				

				delete pLevel;
				pLevel = NULL;
				nSensorsNo ++;
			}
		}
	}

	return blResult;
}

bool CDataTranSQLServer_GM::SaveWaterPressData( COneData_INFO* pData)
{
	CString strSQL="";
	bool blResult = true;
	if(pData != NULL)
	{
		int nSensorsNo = 0;
		while ( !pData->m_lstWaterPress.IsEmpty())
		{
			CWaterPressure_PARA_PER* pPress = pData->m_lstWaterPress.RemoveHead();
			if (pPress)
			{
				strSQL.Format("INSERT INTO WATERP_DATA (RTU,SENSORS_NO,CURRENT_VAL,COLLECT_TIME,SAVE_TIME) VALUES('%s','%02d','%.02f','%s','%s');",
					pData->Get_RTU_ID(),
					nSensorsNo,
					pPress->GetRealValue(),
					pData->Get_CollectTime_String(),
					pData->Get_RecvTime_String());

				//if (pDb)
				//	pDb->ExecuteSQL(strSQL);
				blResult = TransmissionSQL(strSQL);

				nSensorsNo ++;
				delete pPress;
				pPress = NULL;
			}
		}
	}


	return blResult;
}

bool CDataTranSQLServer_GM::SaveWaterQualityData( COneData_INFO* pData)
{
	CString strSQL="";
	bool blResult = true;
	if(pData != NULL)
	{
		while (! pData->m_lstWaterQuality.IsEmpty())
		{
			CWaterQuality_PARA_PER* pQuality = pData->m_lstWaterQuality.RemoveHead();
			if (pQuality != NULL)
			{
				strSQL.Format("INSERT INTO WATERQ_DATA (RTU,SPECIES,CURRENT_VAL,COLLECT_TIME,SAVE_TIME) VALUES('%s','D%d','%.*f','%s','%s');",
					pData->Get_RTU_ID(),
					pQuality->m_BitPos,
					pQuality->m_PointBits,
					pQuality->m_CurValue,
					pData->Get_CollectTime_String(),
					pData->Get_RecvTime_String());

				//if (pDb)
				//	pDb->ExecuteSQL(strSQL);
				blResult = TransmissionSQL(strSQL);

				delete pQuality;
				pQuality = NULL;
			}
		}
	}


	return blResult;
}

bool CDataTranSQLServer_GM::SaveWaterFlowData( COneData_INFO* pData)
{

	CString strSQL="";
	bool blResult = true;
	if(pData != NULL)
	{
		int nSensorsNo = 0;
		while (! pData->m_lstFlow.IsEmpty())
		{
			CFlowInitValume_PARA_PER* pFLow = pData->m_lstFlow.RemoveHead();
			if (pFLow)
			{
				strSQL.Format("INSERT INTO FLOWV_DATA (RTU,SENSORS_NO,TOTAL_WATER,ANYTIME_FLOW,COLLECT_TIME,SAVE_TIME) VALUES('%s','%02d',%d,%d,'%s','%s');",
					pData->Get_RTU_ID(),
					nSensorsNo,
					pFLow->GetCalTakeValume(),
					pFLow->GetInsFlowValume(),
					pData->Get_CollectTime_String(),
					pData->Get_RecvTime_String());

				//if (pDb)
				//	pDb->ExecuteSQL(strSQL);
				blResult = TransmissionSQL(strSQL);

				nSensorsNo ++ ;
				delete pFLow;
				pFLow = NULL;
			}
		}
	}


	return blResult;
}

bool CDataTranSQLServer_GM::SaveRainData( COneData_INFO* pData)
{

	CString strSQL="";
	bool blResult = true;
	if(pData != NULL)
	{
		int nSensorsNo = 0;
		while (! pData->m_lstRain.IsEmpty())
		{
			CRainAlert_PARA_PER* pRain = pData->m_lstRain.RemoveHead();
			if (pRain)
			{

				strSQL.Format("INSERT INTO RAIN_DATA (RTU,SENSORS_NO,DATA_TYPE,CURRENT_VAL,HALFH_VAL,TOTAL_VAL,COLLECT_TIME,SAVE_TIME) VALUES ('%s','%02d','%02d','%.1f','%.1f','%.1f','%s','%s');",
					pData->Get_RTU_ID(),
					nSensorsNo,
					pRain->GetDataType(),
					pRain->GetRealTimeValue(),
					pRain->GetHalfHourValue(),
					pRain->GetTotalValue(),
					pData->Get_CollectTime_String(),
					pData->Get_RecvTime_String());

				//if (pDb)
				//	pDb->ExecuteSQL(strSQL);
				blResult = TransmissionSQL(strSQL);

				nSensorsNo ++;
				delete pRain;
				pRain = NULL;
			}
		}
	}


	return blResult;
}

bool CDataTranSQLServer_GM::SaveOperateRecord( COneData_INFO* pData)
{

	CString strSQL="";
	bool blResult = true;
	if (pData != NULL)
	{
		strSQL.Format("INSERT INTO OPERATE_RECORD (RTU,OPERATE_ACCOUNT,OPERATE_TIME,OPERATE_DESC) VALUES ('%s','%s','%s','%s');",
			pData->Get_RTU_ID(),
			pData->mOperateAccount,
			pData->Get_OperateTime_String(),
			pData->mOperateDESC);

		//if (pDb)
		//	pDb->ExecuteSQL(strSQL);
		TransmissionSQL(strSQL);
	}
	

	return blResult;
}

bool CDataTranSQLServer_GM::SaveAnalogData( COneData_INFO* pData)
{

	CString strSQL="";
	bool blResult = true;
	if(pData != NULL)
	{
		int nAnalogNo = 0;
		while (! pData->m_lstAnalog.IsEmpty())
		{
			CAnalogLimit_PARA_PER* pAnalog = pData->m_lstAnalog.RemoveHead();
			if (pAnalog)
			{
				strSQL.Format("INSERT INTO ANALOG_DATA (RTU,ANALOG_NO,CURRENT_VAL,COLLECT_TIME,SAVE_TIME) VALUES ('%s','%02d','%d','%s','%s');",
					pData->Get_RTU_ID(),
					nAnalogNo,
					pAnalog->GetRealTimeValue(),
					pData->Get_CollectTime_String(),
					pData->Get_RecvTime_String());

				//if (pDb)
				//	pDb->ExecuteSQL(strSQL);
				blResult = TransmissionSQL(strSQL);

				delete pAnalog;
				pAnalog = NULL;
				nAnalogNo ++;
			}
		}
	}


	return blResult;
}

bool CDataTranSQLServer_GM::SaveDisplaceData( COneData_INFO* pData)
{

	CString strSQL="";
	bool blResult = true;
	if(pData != NULL)
	{
		int nDisplaceNo = 1;
		while (! pData->m_lstDisplace.IsEmpty())
		{
			CDisplace_PARA_PER* pDisplace = pData->m_lstDisplace.RemoveHead();
			if (pDisplace)
			{
				strSQL.Format("INSERT INTO DISPALCE_DATA (RTU,SENSORS_NO,CURRENT_XVAL,CURRENT_YVAL,COLLECT_TIME,SAVE_TIME) VALUES ('%s','%02d','%d','%d','%s','%s');",
					pData->Get_RTU_ID(),
					nDisplaceNo,
					pDisplace->GetRealTimeXValue(),
					pDisplace->GetRealTimeYValue(),
					pData->Get_CollectTime_String(),
					pData->Get_RecvTime_String());

				blResult = TransmissionSQL(strSQL);

				delete pDisplace;
				pDisplace = NULL;
				nDisplaceNo ++;
			}
		}
	}
	return blResult;
}

bool CDataTranSQLServer_GM::SaveTemperaData( COneData_INFO* pData)
{

	CString strSQL="";
	bool blResult = true;
	if(pData != NULL)
	{
		int nTemperaNo = 0;
		while (! pData->m_lstTempera.IsEmpty())
		{
			CTempera_PARA_PER* pTempera = pData->m_lstTempera.RemoveHead();
			if (pTempera)
			{
				strSQL.Format("INSERT INTO TEMPERA_DATA (RTU,SENSORS_NO,CURRENT_VAL,COLLECT_TIME,SAVE_TIME) VALUES ('%s','%02d','%d','%s','%s');",
					pData->Get_RTU_ID(),
					nTemperaNo,
					pTempera->GetRealTimeValue(),
					pData->Get_CollectTime_String(),
					pData->Get_RecvTime_String());

				//if (pDb)
				//	pDb->ExecuteSQL(strSQL);
				blResult = TransmissionSQL(strSQL);

				delete pTempera;
				pTempera = NULL;
				nTemperaNo ++;
			}
		}
	}


	return blResult;
}

bool CDataTranSQLServer_GM::SavePulseData( COneData_INFO* pData )
{

	CString strSQL="";
	
	bool blResult = true;
	if(pData != NULL)
	{
		while (! pData->m_lstPulses.IsEmpty())
		{
			CPulsePara_PER* pPulse = pData->m_lstPulses.RemoveHead();
			if (pPulse)
			{	
  		       /*
			   strSQL.Format("INSERT INTO PULSE_DATA (RTU,CODE,VALUE1,VALUE2,VALUE3,VALUE4,COLLECT_TIME,SAVE_TIME) VALUES ('%s','%d','%.2f','%.2f','%.2f','%.2f','%s','%s')",
					pData->Get_RTU_ID(),
					pPulse->GetCode(),
					pPulse->GetValue1(),
					pPulse->GetValue2(),
					pPulse->GetValue3(),
					pPulse->GetValue4(),
					pData->Get_CollectTime_String(),
					pData->Get_RecvTime_String());
				  */
				
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

				strTemp.Format("'%s');",pData->Get_RecvTime_String());
				strSQL += strTemp;

						
				/*if (pDb)
					pDb->ExecuteSQL(strSQL);*/
				blResult = TransmissionSQL(strSQL);

				delete pPulse;
				pPulse = NULL;
			}
		}
	}


	return blResult;
}

bool CDataTranSQLServer_GM::SaveAlarmRecord( COneData_INFO* pData)
{
	CString strSQL = "";
	bool blResult = true;
	if (pData != NULL)
	{
		//存储报警信息到ALARM_RECORD表

	
		strSQL.Format("INSERT INTO ALARM_RECORD (P_ID,RTU,W_TYPE,WARNING_TIME,SAVE_TIME) VALUES ('%s','%s',%d,'%s','%s');",
			pData->Get_Alarm_PID(),
			pData->Get_RTU_ID(),
			pData->mAlarmType,
			pData->Get_CollectTime_String(),
			pData->Get_RecvTime_String());

		//if (pDb)
		//	pDb->ExecuteSQL(strSQL);

		blResult = TransmissionSQL(strSQL);

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

bool CDataTranSQLServer_GM::Save_Alarm_WaterLevel( COneData_INFO* pData)
{
	bool blResult = true;
	if (pData != NULL)
	{
		CString strSQL = "";
		int nSensorsNo = 0;

		while (!pData->m_lstWaterLevel.IsEmpty())
		{
			CWaterLevel_PAR_PER* pLevel = pData->m_lstWaterLevel.RemoveHead();
			if (pLevel)
			{
				strSQL.Format("INSERT INTO ALARM_WL (WARNING_ID,SENSORS_NO,CURRENT_VAL) VALUES ('%s','%02d','%.02f');",
					pData->Get_Alarm_PID(),
					nSensorsNo,
					pLevel->GetRealValue());

				/*if (pDb)
					pDb->ExecuteSQL(strSQL);*/
				blResult = TransmissionSQL(strSQL);


				strSQL.Format("INSERT INTO WATERL_DATA (RTU,SENSORS_NO,CURRENT_VAL,COLLECT_TIME,SAVE_TIME) VALUES('%s','%02d','%.02f','%s','%s');",
					pData->Get_RTU_ID(),
					nSensorsNo,
					pLevel->GetRealValue(),
					pData->Get_CollectTime_String(),
					pData->Get_RecvTime_String());

				//if (pDb)
				//	pDb->ExecuteSQL(strSQL);
				blResult = TransmissionSQL(strSQL);

				delete pLevel;
				pLevel = NULL;
				nSensorsNo ++;
			}
		}
	}

	return blResult;
}

bool CDataTranSQLServer_GM::Save_Alarm_WaterPress( COneData_INFO* pData)
{
	bool blResult = true;

	if(pData != NULL)
	{
		CString strSQL = "";
		int nSensorsNo = 0;
		while (!pData->m_lstWaterPress.IsEmpty())
		{
			CWaterPressure_PARA_PER* pPress = pData->m_lstWaterPress.RemoveHead();
			if (pPress)
			{
				strSQL.Format("INSERT INTO ALARM_WP (WARNING_ID,SENSORS_NO,CURRENT_VAL) VALUES ('%s','%02d','%.02f');",
					pData->Get_Alarm_PID(),
					nSensorsNo,
					pPress->GetRealValue());

				/*if (pDb)
					pDb->ExecuteSQL(strSQL);*/
				blResult = TransmissionSQL(strSQL);

				delete pPress;
				pPress = NULL;
				nSensorsNo ++ ;
			}
		}
	}

	return blResult;
}

bool CDataTranSQLServer_GM::Save_Alarm_WaterQuality( COneData_INFO* pData)
{
	bool blResult = true;

	if(pData != NULL)
	{
		CString strSQL = "";

		while (!pData->m_lstWaterQuality.IsEmpty())
		{
			CWaterQuality_PARA_PER* pQuality = pData->m_lstWaterQuality.RemoveHead();
			if (pQuality)
			{
				strSQL.Format("INSERT INTO ALARM_WQ (WARNING_ID,SPECIES,CURRENT_VAL) VALUES ('%s','D%d','%.*f');",
					pData->Get_Alarm_PID(),
					pQuality->m_BitPos,
					pQuality->m_PointBits,
					pQuality->m_CurValue);

				/*if (pDb)
					pDb->ExecuteSQL(strSQL);*/
				blResult = TransmissionSQL(strSQL);

				delete pQuality;
				pQuality = NULL;
			}
		}
	}

	return blResult;
}

bool CDataTranSQLServer_GM::Save_Alarm_Flow( COneData_INFO* pData)
{
	bool blResult = true;
	if (pData != NULL)
	{
		CString strSQL = "";
		int nSensorsNo = 0;

		while (!pData->m_lstFlow.IsEmpty())
		{
			CFlowInitValume_PARA_PER* pFlow = pData->m_lstFlow.RemoveHead();
			if (pFlow)
			{
				strSQL.Format("INSERT INTO ALARM_FL (WARNING_ID,SENSORS_NO,TOTAL_WATER,ANYTIME_FLOW) VALUES ('%s','%02d',%d,%d);",
					pData->Get_Alarm_PID(),
					nSensorsNo,
					pFlow->GetCalTakeValume(),
					pFlow->GetInsFlowValume());

				//if (pDb)
				//	pDb->ExecuteSQL(strSQL);
				blResult = TransmissionSQL(strSQL);

				delete pFlow;
				pFlow = NULL;
				nSensorsNo ++;
			}
		}
	}

	return blResult;
}

bool CDataTranSQLServer_GM::Save_Alarm_Rain( COneData_INFO* pData)
{
	bool blResult = true;
	if (pData != NULL)
	{
		CString strSQL = "";
		int nSensorsNo = 0;

		while (! pData->m_lstRain.IsEmpty())
		{
			CRainAlert_PARA_PER* pRain  = pData->m_lstRain.RemoveHead();
			if (pRain)
			{
				strSQL.Format("INSERT INTO ALARM_RAIN (WARNING_ID,SENSORS_NO,CURRENT_VAL,PER_TIME) VALUES ('%s','%02d','%.1f',%d);",
					pData->Get_Alarm_PID(),
					nSensorsNo,
					pRain->GetRealTimeValue(),
					pRain->GetPerTime());

				//if (pDb)
				//	pDb->ExecuteSQL(strSQL);
				blResult = TransmissionSQL(strSQL);

				delete pRain;
				pRain = NULL;
				nSensorsNo ++;
			}
		}
	}

	return blResult;
}

bool CDataTranSQLServer_GM::Save_Alarm_Analog( COneData_INFO* pData )
{
	bool blResult = true;

	if (pData != NULL)
	{
		CString strSQL = "";
		int nAnalog = 0;
		while (! pData->m_lstAnalog.IsEmpty())
		{
			CAnalogLimit_PARA_PER* pAnalog = pData->m_lstAnalog.RemoveHead();
			if (pAnalog)
			{
				strSQL.Format("INSERT INTO ALARM_ANALOG (WARNING_ID,ANALOG_NO,CURRENT_VAL) VALUES ('%s','%02d',%d);",
					pData->Get_Alarm_PID(),
					nAnalog,
					pAnalog->GetRealTimeValue());

				/*if (pDb)
					pDb->ExecuteSQL(strSQL);*/

				blResult = TransmissionSQL(strSQL);

				delete pAnalog;
				pAnalog = NULL;
				nAnalog ++;
			}
		}		
	}

	return blResult;
}

bool CDataTranSQLServer_GM::Save_Alarm_Displace( COneData_INFO* pData )
{
	bool blResult = true;

	if (pData != NULL)
	{
		CString strSQL = "";
		int nDisplace = 1;
		while (! pData->m_lstDisplace.IsEmpty())
		{
			CDisplace_PARA_PER* pDisplace = pData->m_lstDisplace.RemoveHead();
			if (pDisplace)
			{
				strSQL.Format("INSERT INTO ALARM_DISPLACE (WARNING_ID,SENSORS_NO,CURRENT_XVAL,CURRENT_YVAL) VALUES ('%s','%02d',%d,%d);",
					pData->Get_Alarm_PID(),
					nDisplace,
					pDisplace->GetRealTimeXValue(),
					pDisplace->GetRealTimeYValue());


				blResult = TransmissionSQL(strSQL);

				delete pDisplace;
				pDisplace = NULL;
				nDisplace ++;
			}
		}		
	}
	return blResult;
}

bool CDataTranSQLServer_GM::Save_Alarm_Tempera( COneData_INFO* pData )
{
	bool blResult = true;

	if (pData != NULL)
	{
		CString strSQL = "";
		int nTempera = 0;
		while (! pData->m_lstTempera.IsEmpty())
		{
			CTempera_PARA_PER* pTempera = pData->m_lstTempera.RemoveHead();
			if (pTempera)
			{
				strSQL.Format("INSERT INTO ALARM_TEMPERA (WARNING_ID,SENSORS_NO,CURRENT_VAL) VALUES ('%s','%02d',%d);",
					pData->Get_Alarm_PID(),
					nTempera,
					pTempera->GetRealTimeValue());

				blResult = TransmissionSQL(strSQL);

				delete pTempera;
				pTempera = NULL;
				nTempera ++;
			}
		}		
	}
	return blResult;
}

void CDataTranSQLServer_GM::CheckDatabase()
{

}

void CDataTranSQLServer_GM::SetDataSource( CString strSource )
{
	m_strDataSource = strSource;
}

bool CDataTranSQLServer_GM::OpenDatabase()
{
	return OpenDatabase(m_strDataSource);
}

bool CDataTranSQLServer_GM::OpenDatabase( CString dataSource )
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
bool CDataTranSQLServer_GM::GetConnectionState()
{
	if (!m_Database.IsOpen())
		return false;
	try
	{
		if (m_nDatabaseType == SQLServer_Database)
		{
			CString strSQL="--";
			m_Database.ExecuteSQL(strSQL);
		}

	}
	catch (CDBException* e)
	{
		CString strMessage = "";
		strMessage.Format("与数据库连接断开,Error:%s",e->m_strError);
		AddThreadLog(strMessage,Error_Log_message);
		e->Delete();
		m_Database.Close();
		return false;
	}
	return true;
}

bool CDataTranSQLServer_GM::CheckTableExist( CString sTableName )
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

bool CDataTranSQLServer_GM::ExcuteNoQuery( CString sSQL )
{
	bool ret = true;

	CString sLog = "";
	try
	{
		m_Database.ExecuteSQL(sSQL);
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


bool CDataTranSQLServer_GM::TransmissionSQL(CString strSQL)
{
	bool ret = false;
	if (m_nInterfaceType == Database_Interface)
		ret = ExcuteNoQuery(strSQL);
	else if (m_nInterfaceType == WebService_Interface)
		ret = TransByWebService(strSQL);
	
	return ret;
}

bool CDataTranSQLServer_GM::TransByWebService(CString sSQL)
{
	bool ret = true;
	if (sSQL != "")
	{
		CoInitialize(NULL);
        HRESULT  hr = S_OK;
		CComBSTR bstrUsername = m_strWebServiceUsername;
		CComBSTR bstrPassword = m_strWebServicePassword;
		CComBSTR bstrSQLs = "";
		CComBSTR bstrResult = "";  //返回值


		CString  Url = m_strWebServiceURL;   


		CString strTemp = "";
		strTemp.Format("<?xml version=\"1.0\" encoding=\"UTF-8\"?><sqls><statement><![CDATA[%s]]></statement></sqls>",sSQL);
		bstrSQLs = strTemp;

		#ifdef DEBUG
		SYSTEMTIME _debugtime;
		GetLocalTime(&_debugtime);
		TRACE("%s %s\r\n",CSystemTimeOp::ConvertSystemTimeToString(&_debugtime,true),sSQL);
		TRACE("WebService:%s\r\n",strTemp);
		#endif

		//初始化访问WebService接口对象
		StubWebService::CStubWebService* service = new StubWebService::CStubWebService();
		if (service != NULL)
		{
			//设置webservice的URL
			service->SetUrl(Url);

			//调用WebService接口
			hr = service->execDML(bstrUsername,bstrPassword,bstrSQLs,&bstrResult);

			//判断调用WebService接口的结果
			int nCount = 0;

			while(FAILED(hr) && nCount < 2)
			{
				AddThreadLog("调用WebSservice接口失败",Error_Log_message);
				nCount ++;

				hr = service->execDML(bstrUsername,bstrPassword,bstrSQLs,&bstrResult);
			}

			if (FAILED(hr))
			{

			}
			else
			{
				CString strResult(bstrResult);
				if (strResult.Find("failure") != -1)
				{
					CString strMessage ="";
					strMessage.Format("调用WebService接口execDML函数,报警:%s",strResult);

					AddThreadLog(strMessage,Warning_log_Message,false);
				}
			}

		}
		_DELETE(service);

		CoUninitialize();
	}
	
	return ret;
}