#pragma once
#include "DataTran_Base.h"


//采集数据处理
//本地SQLite数据存储
//继承:CDataTranBase
class CDataTranSQLite_Local
	:public CDataTran_Base
{
public:
	CDataTranSQLite_Local();
	~CDataTranSQLite_Local();
public: 
	virtual long ThreadFunction();
	virtual void SetDataSource(CString strSource);
protected:
	bool OpenSqlite();

protected:
	CDbSQLite m_sqlite;

	void  InitSQLiteDataBase();
	bool  SaveDataToSQLite(COneData_INFO* pData);
	bool SaveSyqDataToSQLite(COneData_INFO* pData);

	bool  IsExistTable(CString strTable);													   //检查表是否存在
protected:
	bool     SaveWaterLevelData(COneData_INFO* pData);                                         //存储水位采集数据
	bool	 SaveWaterPressData(COneData_INFO* pData);										   //存储水压采集数据
	bool     SaveWaterQualityData(COneData_INFO* pData);									   //存储水质采集数据
	bool     SaveWaterFlowData(COneData_INFO* pData);										   //存储流量采集数据
	bool     SaveRainData(COneData_INFO* pData);											   //存储雨量采集数据
	bool     SaveOperateRecord(COneData_INFO* pData);										   //存储操作记录数据
	bool     SaveAnalogData(COneData_INFO* pData);											   //存储模拟量采集数据
	bool     SavePulseData(COneData_INFO* pData);											   //存储电度量采集数据
	bool     SaveAlarmRecord(COneData_INFO* pData);											   //存储报警记录信息
	bool	 SavePeaceReport( COneData_INFO* pData );										   //存储每日平安报

	bool     SaveSyqWaterLevelData(COneData_INFO* pData);                                         //存储水雨情水位采集数据
	bool     SaveSyqRainData(COneData_INFO* pData);											   //存储水雨情雨量采集数据

	int    SetSyqWaterType(COneData_INFO* pData);              //获取报文雨量的类型


	//存储报警相关的数据信息
	bool     Save_Alarm_WaterLevel(COneData_INFO* pData);										//存储报警水位信息
	bool     Save_Alarm_WaterPress(COneData_INFO* pData);										//存储报警水压信息
	bool     Save_Alarm_WaterQuality(COneData_INFO* pData);										//存储报警水质信息
	bool	 Save_Alarm_Flow(COneData_INFO* pData);												//存储报警流量信息
	bool	 Save_Alarm_Rain(COneData_INFO* pData);												//存储报警雨量信息
	bool	 Save_Alarm_Analog(COneData_INFO* pData);											//存储报警模拟量信息
private:
	void CheckSQLiteDatabase();
	bool ExcuteNoQuery(CString strSQL);															//执行不查询
    CSqlStatement* ExcuteQuery(CString strSQL);													//查询

	bool DeleteDataOnTimer();																	//定时删除数据
	long m_nSaveMonth;																			//保存最近数据的月份，默认6个月,半年，不包括本月在内的 
	CString m_strDataSource;
};
