#pragma once
#include "datatran_base.h"
#include "WaitTimer.h"
#include <map>
#include <list>


//雨量
struct Table_ST_PPTN_R_Record
{
public:
	CString STCD;  //测站编码
	CString TM;	   //时间
	double DRP;	   //时段降水量
	double INTV;   //时段长    1.00 /0.05
	double PRD;	   //降水历时
	double DYP;	   //日降水量
	CString WTH;   //天气状况 7:降雨 8:无降雨
public:
	Table_ST_PPTN_R_Record()
	{
		STCD = "";
		TM = "";
		DRP = 0.0;
		INTV = 0.00;
		PRD = 0.00;
		DYP = 0.0;
		WTH = "";
	}
	const CString ToString();
};
//暴雨加报
struct Table_ST_STORM_R_Record
{
public:
	CString STCD ;	//测站编码  
	CString	TM; 	//时间  	 
	double	STRMDR; //暴雨历时  0.15
	double	STRMP;	//暴雨量  	
	CString	WTH;	//天气状况  7:降雨 8:无降雨 
public:
	Table_ST_STORM_R_Record()
	{
		STCD = "";
		TM = "";
		STRMDR = 0.10;
		STRMP = 0.0;
		WTH = "7";
	}
	const CString ToString();
};

//云南水雨情数据接口
class CDataTranSQLServer_YN:
	public CDataTran_Base,public CDataTranSQLServer_Base
{
public:
	CDataTranSQLServer_YN(void);
	~CDataTranSQLServer_YN(void);

public:
	virtual long ThreadFunction();


	void Statistics_Data_OnRealTime(COneData_INFO* pData);  //来点数据实时统计
	void StatisticsRain_1Day_OnTimer();                     //定时统计1天的雨量数据

	void Initialization();
	virtual void SetDataSource(CString strSource);
	void SetSqlite(CDbSQLite* sqlite);
protected:	//SQL Server 操作	
	bool OpenDatabase();
	bool OpenDatabase(CString dataSource);	
	bool GetConnectionState();
	bool CheckTableExist(CString sTableName); //检查数据库中标是否存在, true:表存在,false:表不存在
	bool ExcuteNoQuery(CString sSQL);



private:
	CDbSQLite*  m_sqlite;	

};
