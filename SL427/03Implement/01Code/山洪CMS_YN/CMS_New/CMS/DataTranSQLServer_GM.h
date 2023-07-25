#pragma once
#include "datatran_base.h"

const BYTE Database_Interface = 1;						//直接访问数据库的方式
const BYTE WebService_Interface = 2;					//通过访问WebService接口的方式


class CDataTranSQLServer_GM :
	public CDataTran_Base,public CDataTranSQLServer_Base
{
public:
	CDataTranSQLServer_GM();
	~CDataTranSQLServer_GM();
public:
	virtual long ThreadFunction();
	virtual void SetDataSource(CString strSource);
protected:
	//bool     SaveWaterLevelData(COneData_INFO* pData,CDatabase* pDb);                                          //存储水位采集数据
	//bool	 SaveWaterPressData(COneData_INFO* pData,CDatabase* pDb);										   //存储水压采集数据
	//bool     SaveWaterQualityData(COneData_INFO* pData,CDatabase* pDb);										   //存储水质采集数据
	//bool     SaveWaterFlowData(COneData_INFO* pData,CDatabase* pDb);										   //存储流量采集数据
	//bool     SaveRainData(COneData_INFO* pData, CDatabase* pDb);											   //存储雨量采集数据
	//bool     SaveOperateRecord(COneData_INFO* pData,CDatabase* pDb);										   //存储操作记录数据
	//bool     SaveAnalogData(COneData_INFO* pData,CDatabase* pDb);											   //存储模拟量采集数据
	//bool     SavePulseData(COneData_INFO* pData,CDatabase* pDb);											   //存储电度量采集数据
	//bool     SaveAlarmRecord(COneData_INFO* pData,CDatabase* pDb);											   //存储报警记录信息

	////存储报警相关的数据信息
	//bool     Save_Alarm_WaterLevel(COneData_INFO* pData,CDatabase* pDb);										//存储报警水位信息
	//bool     Save_Alarm_WaterPress(COneData_INFO* pData,CDatabase* pDb);										//存储报警水压信息
	//bool     Save_Alarm_WaterQuality(COneData_INFO* pData,CDatabase* pDb);										//存储报警水质信息
	//bool	 Save_Alarm_Flow(COneData_INFO* pData,CDatabase* pDb);												//存储报警流量信息
	//bool	 Save_Alarm_Rain(COneData_INFO* pData,CDatabase* pDb);												//存储报警雨量信息
	//bool	 Save_Alarm_Analog(COneData_INFO* pData,CDatabase* pDb);											//存储报警模拟量信息


	bool     SaveWaterLevelData(COneData_INFO* pData);                                         //存储水位采集数据
	bool	 SaveWaterPressData(COneData_INFO* pData);										   //存储水压采集数据
	bool     SaveWaterQualityData(COneData_INFO* pData);									   //存储水质采集数据
	bool     SaveWaterFlowData(COneData_INFO* pData);										   //存储流量采集数据
	bool     SaveRainData(COneData_INFO* pData);											   //存储雨量采集数据
	bool     SaveOperateRecord(COneData_INFO* pData);										   //存储操作记录数据
	bool     SaveAnalogData(COneData_INFO* pData);											   //存储模拟量采集数据
	bool     SavePulseData(COneData_INFO* pData);											   //存储电度量采集数据
	bool     SaveAlarmRecord(COneData_INFO* pData);											   //存储报警记录信息

	//存储报警相关的数据信息
	bool     Save_Alarm_WaterLevel(COneData_INFO* pData);										//存储报警水位信息
	bool     Save_Alarm_WaterPress(COneData_INFO* pData);										//存储报警水压信息
	bool     Save_Alarm_WaterQuality(COneData_INFO* pData);										//存储报警水质信息
	bool	 Save_Alarm_Flow(COneData_INFO* pData);												//存储报警流量信息
	bool	 Save_Alarm_Rain(COneData_INFO* pData);												//存储报警雨量信息
	bool	 Save_Alarm_Analog(COneData_INFO* pData);											//存储报警模拟量信息
protected:
	bool OpenDatabase();
	bool OpenDatabase(CString dataSource);	
	bool GetConnectionState();
	bool CheckTableExist(CString sTableName); //检查数据库中标是否存在, true:表存在,false:表不存在
	bool ExcuteNoQuery(CString sSQL);         //在数据库中执行sql
	bool TransByWebService(CString sSQL);	  //通过webservice执行sql;

	void Initialization();					  //初始化thead
	bool TransmissionSQL(CString strSQL);	  //执行strSQL ,当m_nInterfaceType=1,传送给数据库执行,m_nInterfaceType = 2,访问webservice执行
private:
	void CheckDatabase();					  //检查数据库

	BYTE m_nInterfaceType;					  //接口方式 1:直接存数据库 2:通过WebService接口
	CString m_strWebServiceURL;				  //当接口是webservice时,需要URL
	CString m_strWebServiceUsername;		  //当接口是webservice时,需要Username
	CString m_strWebServicePassword;		  //当接口是webservice时,需要Password
};
