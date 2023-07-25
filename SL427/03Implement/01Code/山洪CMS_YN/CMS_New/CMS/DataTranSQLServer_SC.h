#pragma once
#include "datatran_base.h"
#include "WaitTimer.h"
#include <map>
#include <list>


const double RainFullScale_CONST = 9999999.9;  //雨量累计值满刻度

//雨量统计类型
enum RIANSTATISTICS_TYPE_Enum
{
	Rain_statistics_5minutes	= 0,  //5分钟统计
	Rain_statistics_15minutes	= 1,  //15分钟统计
	Rain_statistics_1hour		= 2,  //1小时统计
	Rain_statistics_AM8Point	= 3,  //早上8日统计
};

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

//雨量数据
class CRainData_Item
{
private:
	int			m_nDataType;		//数据类型 0:定时数据 1:开始下雨 2:5分钟超阀值 3:有雨整点、半点 4:雨停
	double		m_dCurrentValue;	//实时值(单场雨累积值)(0-999.9)
	double      m_dSumHalfHour;		//半小时累计值(0-999.9)
	double		m_dTotalValue;		//总累计值 (范围:0～9999999.9)
	CString		m_sCollectTime;		//采集时间string
public:
	SYSTEMTIME	m_tCollectTime;		//采集时间
public:
	CRainData_Item(int dataType = 0,
		           double CurVal = 0.0,
				   double SumHalfH = 0.0,
				   double TotalVal = 0.0,
				   CString sCollTime ="");

	const int DataType() {return m_nDataType;}
	const double CurrentValue(){return m_dCurrentValue;}
	const double TotalValue(){return  m_dTotalValue;}
	const double SumHalfHour(){return m_dSumHalfHour;}
	const CString CollectTimeString() {return m_sCollectTime;}
	const CString ToString();
};

//水位数据
class CWaterL_DataItem
{
public:
	double		m_dValue;				//水位实时值
	SYSTEMTIME  m_RecordTime;			//采集时间
public:
	CWaterL_DataItem();
	CWaterL_DataItem(double dValue, SYSTEMTIME& st);
};

//测站类型
enum CZ_Type_Enum
{
	CZ_Type_RAIN = 1,   //雨量站
	CZ_Type_WL	 = 2,   //水位站
};

//水位测站类型
enum WL_Type_Enum
{
	WL_Type_HLSW = 1,		//河流水位站
	WL_Type_SKSSW = 2,		//水库上水位站
	WL_Type_SKXSW = 3,		//水库下水位站
};

const ULONG BEGIN_CALC_INDEX = 0;
const ULONG LAST_CALC_INDEX  = 5;

//地址映射相关
class CAddrMap_Item
{
private:
	CString m_RTU_ID;					//510101_00000000
	CZ_Type_Enum     m_Type;			//类型 1：雨量站  2:水位站
	WL_Type_Enum     m_wlType;			//水位类型 1：河道水位 2：水库上水位 3：水库下水位
	CString m_STCD;						//测站编码

	CString m_addVCode;					//行政区划码 6位  如：510101
	int     m_RtuAddr;					//遥测终端地址 0

	bool    m_blCalc;					//是否按时统计，默认: false
	bool    m_blCalcHalf;				//半小时统计    //雨量统计时使用

	ULONG   m_nCalcIndex;				//统计计数 0-5
public:
	CAddrMap_Item();
	CAddrMap_Item(CString stCd,
		int     Type,
		int     wlType,
		CString AddVCode,
		int     RTUAddr);
public:
	CString Get_RTU_ID()		{return m_RTU_ID;}
	CString Get_STCD()			{return m_STCD;}
	CString Get_ADDVCode()		{return m_addVCode;}
	int     Get_RtuAddr()		{return m_RtuAddr;}
	CZ_Type_Enum GetType()		{return m_Type;}
	WL_Type_Enum GetWlType()	{return m_wlType;}

	//设置是或获取按时统计的标志
	void    SetCalcState(bool _value) { m_blCalc  = _value;}
	const bool GetCalcState() {return m_blCalc;}

	void   SetCalcHalfState(bool _value) { m_blCalcHalf = _value;}
	const bool GetCalcHalfState()	{return m_blCalcHalf;}

	void  SetCalcIndex(ULONG nIdx)	{m_nCalcIndex = nIdx;}
	const ULONG GetCalcIndex()		{return m_nCalcIndex;}
};

//四川省山洪灾害数据接口
class CDataTranSQLServer_SC :
	public CDataTran_Base,public CDataTranSQLServer_Base
{
public:
	CDataTranSQLServer_SC();
	~CDataTranSQLServer_SC();
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
	bool       m_blStartCalc;				  //是否执行统计
	bool	   m_blCalc;
	CDbSQLite*  m_sqlite;	

	//雨量最大值正确性检查
	double		m_dCHECK_RAIN_PER; //检查5、15、1hour的值
	double		m_dCHECK_RAIN_DAY; //检查1day的值 

	//雨量统计时间检查
	UINT	    m_nCheck5Minutes_MNT;
	UINT		m_nCheck15Minutes_MNT;
	UINT		m_nCheck1Hour_MNT;
	UINT		m_nChcek1Day_MNT;
private:	//数据库操作,统计存库
	void CheckDatabase();					 //检查数据存库对应的表是否存在

    //时间< strend的最后一个雨量值,从本地SQLite
	CRainData_Item*  GetEndTimeRainData(CAddrMap_Item* pAddrItem,CString strEnd);

	//时间>= strStart 的一个值 ，从本地SQLite
	CRainData_Item* GetStartTimeRainData(CAddrMap_Item* pAddrItem,CString strStart);

	//从本地SQLite数据库 获取1个小时最后的水位数据(水位数据  1 hours  0 < strpoint < end内)
	CWaterL_DataItem* GetLatestWaterLevelData(CAddrMap_Item* pAddrItem,CString strPoint);
private:	//sqlite数据库操作
	bool OpenSqlite();
	CSqlStatement* ExcuteSQLiteQuery(CString strSQL);							//查询

private:	//地址映射(遥测终端机与主站测站编码的地址映射)
	bool InitAddressMap();														//初始化地址映射
	void CheckAddressMap_ForRtu();												//检查地址映射表的正确性 与遥测终端机的映射关系
	void CheckAddressMap_ForMaster();											//检查地址映射表的正确性 与主站ST_STBPRP_B表的配置数据进行检查
	//std::map<CString,CAddrMap_Item*>  m_mapAddressMap;						//地址映射
	std::list<CAddrMap_Item*> m_mapAddressMap;									//地址映射 针对相同的攀枝花有STCD 修改 20120803
	void ClearAddrMap();

private:	//水位
	void CalcWaterLevel_RealTime(CString Rtu_ID,CWaterL_DataItem* pRecvData);                 //来点统计(水位)
	void StatisticsWaterLevel_RealTime(CAddrMap_Item* pAddrItem,CWaterL_DataItem* pRecvData);
	bool UpdateWaterLevelData( CAddrMap_Item* pAddrItem, CString strTM, double pointValue);
	bool GetWaterLevelData(CString sSQL,double& WaterLevelData);
private:	//雨量
	void CalcRainData_RealTime(CString Rtu_ID,CRainData_Item* pRecvData);					   //来点统计(雨量)
	void StatisticsRain_RealTime(CAddrMap_Item* pAddrItem,CRainData_Item* pRecvData);	
	void StatisticsRain_5Minutes(CAddrMap_Item* pAddrItem,CRainData_Item* pRecvData);		   //5分钟雨量累计值统计
	void StatisticsRain_15Minutes(CAddrMap_Item* pAddrItem,CRainData_Item* pRecvData);		   //15分钟雨量累计值统计
	void StatisticsRain_1Hour(CAddrMap_Item* pAddrItem,CRainData_Item* pRecvData);			   //1个小时雨量累计值统计
	void StatisticsRain_1Day(CAddrMap_Item* pAddrItem,CRainData_Item* pRecvData);

	double StatisticsRain_1Day(CAddrMap_Item* pAddrItem,CString strBegin,CString strEnd,CRainData_Item* pRecvData=NULL);      //1日雨量统计

	//如果用累计值计算出现大数据>=500时,才调用特殊处理(采用半小时累计值进行计算)
	double StatisticsRain_1Hour_special(CAddrMap_Item* pAddrItem,SYSTEMTIME sttime,CRainData_Item* pRecvData = NULL);
	//如果用累计值计算出现大数据>=500时,才调用特殊处理(采用半小时累计值进行计算)
	double StatisticsRain_1Day_special(CAddrMap_Item* pAddrItem,SYSTEMTIME sttime,CRainData_Item* pRecvData = NULL);
    
	//更新降雨量表ST_PPTN_R的记录
	bool UpdateRainfallTableRecord(Table_ST_PPTN_R_Record& sRecord,RIANSTATISTICS_TYPE_Enum xType);
	
	//从降雨量表ST_PPTN_R的查找记录(UPDATE or  INSERT)
	bool GetRainfallTableRecord(CString sSQL,Table_ST_PPTN_R_Record& findRecord);	

	//更新暴雨加报表ST_STORM_R的记录(UPDATE or  INSERT)
	bool UpdateRainstormTableRecord(Table_ST_STORM_R_Record& sRecord);

	//获取暴雨加报表ST_STORM_R的记录
	bool GetRainstormRecord(CString sSQL,Table_ST_STORM_R_Record& findRecord);

	//计算降雨历时
	int  GetRainMinutes( CAddrMap_Item* pAddrItem, CString strBegin, CString strEnd );

	/*
	 函数功能:计算一段时间的累计降雨量
	 添加时间:2012-08-12
	 参数说明:
	 pAddrItem  STCD
	 strBegin   开始时间 >=
	 strEnd     结束时间 <
	 xType      统计类型(5分钟,15分钟,1hour,1day)
	 isNull     返回值(true:该时间段内没记录,false:表示该时间段类有记录)
	*/
	double CalRainData_OnTimeRange(CAddrMap_Item* pAddrItem,
		CString strBegin,
		CString strEnd,
		RIANSTATISTICS_TYPE_Enum xType,
		bool& isNull);
}; 



