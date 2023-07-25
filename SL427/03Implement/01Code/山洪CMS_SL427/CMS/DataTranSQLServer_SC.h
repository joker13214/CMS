#pragma once
#include "datatran_base.h"
#include "WaitTimer.h"
#include <map>
#include <list>
#include "SystemTimeOp.h"
#include "DataTranSQLite_Local.h"
#include <iterator>
  

const double RainFullScale_CONST = 9999999.9;  //雨量累计值满刻度

//雨量统计类型
enum RIANSTATISTICS_TYPE_Enum
{
	Rain_statistics_5minutes	= 0,  //5分钟统计
	Rain_statistics_15minutes	= 1,  //15分钟统计
	Rain_statistics_1hour		= 2,  //1小时统计
	Rain_statistics_AM8Point	= 3,  //早上8日统计
	Rain_statistics_UNKNOWN     = 255,//未知类型
};

//时间段
//开始结束时间
struct STRUCT_BEGINEND_AREA
{
	SYSTEMTIME stBegin;
	SYSTEMTIME stEnd;
	bool blSucc;						//是否计算成功，false:失败，true：成功
public:
	STRUCT_BEGINEND_AREA()
	{
		CSystemTimeOp::ResetSystemTime(stBegin);
		CSystemTimeOp::ResetSystemTime(stEnd);
		blSucc = false;
	}
	const CString ToString();
};


//水雨情数据记录
struct Table_ST_PPTN_R_Record
{
public:
	CString STCD;						//测站编码
	RIANSTATISTICS_TYPE_Enum TYPE;		//类型 0：5分钟，1:15分钟统计，2:小时统计 3：日统计
	CString TM;							//时间
	SYSTEMTIME FTM;						//完整的时间 XXXX-XX-XX XX:XX:XX.XXX格式，关键：对比时间
	CString WTH;						//天气状况 7:降雨 8:无降雨
	double DRP;							//时段降水量
	double INTV;						//时段长    1.00 /0.05
	double PRD;							//降水历时
	double DYP;							//日降水量

	//added 20140813
	SYSTEMTIME CAL_TM; //统计时间
public:
	Table_ST_PPTN_R_Record()
	{
		STCD = "";
		TYPE = Rain_statistics_5minutes;
		TM = "";
		DRP = 0.0;
		INTV = 0.00;
		PRD = 0.00;
		DYP = 0.0;
		WTH = "8";
		GetLocalTime(&CAL_TM);
	}

	//拷贝构造函数
	Table_ST_PPTN_R_Record(Table_ST_PPTN_R_Record* pSource)
	{
		if (pSource != NULL)
		{
			STCD = pSource->STCD;
			TYPE = pSource->TYPE;
			TM = pSource->TM;
			FTM = pSource->FTM;
			DRP = pSource->DRP;
			INTV = pSource->INTV;
			PRD = pSource->PRD;
			DYP = pSource->DYP;
			WTH = pSource->WTH;
			CAL_TM = pSource->CAL_TM;
		}
		else
		{
			STCD = "";
			TM = "";
			DRP = 0.0;
			INTV = 0.00;
			PRD = 0.00;
			DYP = 0.0;
			WTH = "8";
			GetLocalTime(&CAL_TM);
		}
	}

	~Table_ST_PPTN_R_Record()
	{		
		//STCD = "";
		//TM = "";
		//DRP = 0.0;
		//INTV = 0.00;
		//PRD = 0.00;
		//DYP = 0.0;
		//WTH = "";

	}
	const CString ToString();
};


//流量ST_RIVER_R的数据记录
struct Table_ST_RIVER_R_Record {
public:
	CString STCD;  //测站编码
	CString TM;    //时间
	double Z;
	double Q;    //瞬时流量
	double XSA;  
	double XSAVV;
	double XSMXV;
	CString FLWCHRCD;
	CString WPTN;
	CString MSQMT;
	CString MSAMT;
	CString MSVMT;

	//2023/5/18
	SYSTEMTIME CAL_TM;		//统计时间
public:
	Table_ST_RIVER_R_Record() {
		STCD = "";
		TM = "";
		Z = 0.0;
		Q = 0.0;
		XSA = 0.0;
		XSAVV = 0.0;
		XSMXV = 0.0;
		FLWCHRCD = "";
		WPTN = "";
		MSQMT = "";
		MSAMT = "";
		MSVMT = "";

		GetLocalTime(&CAL_TM);
	}
	Table_ST_RIVER_R_Record(Table_ST_RIVER_R_Record* pSource) {
		if(pSource != NULL) {
			STCD = pSource->STCD;
			TM = pSource->TM;
			Z = pSource->Z;
			Q = pSource->Q;
			XSA = pSource->XSA;
			XSAVV = pSource->XSAVV;
			XSMXV = pSource->XSMXV;
			FLWCHRCD = pSource->FLWCHRCD;
			WPTN = pSource->WPTN;
			MSQMT = pSource->MSQMT;
			MSAMT = pSource->MSAMT;
			MSVMT = pSource->MSVMT;
		}
		else {
			STCD = "";
			TM = "";
			Z = 0.0;
			Q = 0.0;
			XSA = 0.0;
			XSAVV = 0.0;
			XSMXV = 0.0;
			FLWCHRCD = "";
			WPTN = "";
			MSQMT = "";
			MSAMT = "";
			MSVMT = "";

			GetLocalTime(&CAL_TM);
		}
	}
	~Table_ST_RIVER_R_Record() {

	}
	const CString ToString();
};
//流量ST_RSVR_Record的数据记录
struct Table_ST_RSVR_R_Record {
public:
	CString STCD; //测站编码
	CString TM;   //时间
	double RZ;
	double INQ;
	double W;
	double BLRZ;
	double OTQ;
	CString RWCHRCD;
	CString RWPTN;
	double INQDR;
	CString MSQMT;
	
	//2023/5/19;
	SYSTEMTIME CAL_TM;		//统计时间
public:
	Table_ST_RSVR_R_Record() {
		STCD = "";
		TM = "";
		RZ = 0.0;
		INQ = 0.0;
		W = 0.0;
		BLRZ = 0.0;
		OTQ = 0.0;
		RWCHRCD = "";
		INQDR = 0.0;
		MSQMT = "";
		RWPTN = "";
		
		GetLocalTime(&CAL_TM);
	}
	Table_ST_RSVR_R_Record(Table_ST_RSVR_R_Record* pSource) {
		if (pSource != NULL) {
			STCD = pSource->STCD;
			TM = pSource->TM;
			RZ = pSource->RZ;
			INQ = pSource->INQ;
			W = pSource->W;
			BLRZ = pSource->BLRZ;
			OTQ = pSource->OTQ;
			RWCHRCD = pSource->RWCHRCD;
			INQDR = pSource->INQDR;
			MSQMT = pSource->MSQMT;
			RWPTN = pSource->RWPTN;
		}
		else {
			STCD = "";
			TM = "";
			RZ = 0.0;
			INQ = 0.0;
			W = 0.0;
			BLRZ = 0.0;
			OTQ = 0.0;
			RWCHRCD = "";
			INQDR = 0.0;
			MSQMT = "";
			RWPTN = "";
			GetLocalTime(&CAL_TM);
		}
	}
	~Table_ST_RSVR_R_Record() {}
	const CString ToString();
};
//暴雨加报记录
struct Table_ST_STORM_R_Record
{
public:
	CString STCD ;			//测站编码  
	CString	TM; 			//时间  	 
	SYSTEMTIME FTM;			//完整的时间 XXXX-XX-XX XX:XX:XX.XXX格式,关键：对比时间
	double	STRMDR;			//暴雨历时  0.15
	double	STRMP;			//暴雨量  	
	CString	WTH;			//天气状况  7:降雨 8:无降雨 

	//added 20140813
	SYSTEMTIME CAL_TM;		//统计时间
public:
	Table_ST_STORM_R_Record()
	{
		STCD = "";
		TM = "";
		STRMDR = 0.10;
		STRMP = 0.0;
		WTH = "7";

		GetLocalTime(&CAL_TM);
	}
	
	//拷贝构造函数
	Table_ST_STORM_R_Record(Table_ST_STORM_R_Record* pSource)
	{
		if (pSource != NULL)
		{
			STCD = pSource->STCD;
			TM = pSource->TM;
			FTM = pSource->FTM;
			STRMDR = pSource->STRMDR;
			STRMP = pSource->STRMP;
			WTH = pSource->WTH;

			CAL_TM = pSource->CAL_TM;
		}
		else
		{
			STCD = "";
			TM = "";
			STRMDR = 0.10;
			STRMP = 0.0;
			WTH = "7";

			GetLocalTime(&CAL_TM);
		}
	}

	~Table_ST_STORM_R_Record(){}


	const CString ToString();
};

//水位数据
struct Table_ST_WL_R_Record
{
public:
	CString STCD;			//测站编码
	CString TM;				//时间点
	SYSTEMTIME FTM;			//完整的时间 XXXX-XX-XX XX:XX:XX.XXX格式，关键：对比时间
	int     TYPE;			//类型，1 河流水位、2 水库上水位、3 水库下水位
	double  PV;				//水位值
	SYSTEMTIME CAL_TM;		//统计时间
public:
	Table_ST_WL_R_Record()
	{
		STCD = "";
		TM = "";
		TYPE = 1;
		PV = 0.0;
		GetLocalTime(&CAL_TM);
	}
	Table_ST_WL_R_Record(Table_ST_WL_R_Record* pSource)
	{
		if (pSource != NULL)
		{
			STCD = pSource->STCD;
			TM = pSource->TM;
			FTM = pSource->FTM;
			TYPE = pSource->TYPE;
			PV = pSource->PV;
			CAL_TM = pSource->CAL_TM;
		}
		else
		{
			STCD = "";
			TM = "";
			TYPE = 1;
			PV = 0.0;
			GetLocalTime(&CAL_TM);
		}
	}
	~Table_ST_WL_R_Record(){}
};

//深度位移数据
struct Table_ST_DISP_R_Record
{
public:
	CString STCD;			//测站编码
	CString TM;				//时间点
	SYSTEMTIME FTM;			//完整的时间 XXXX-XX-XX XX:XX:XX.XXX格式，关键：对比时间
	double  X;				//水位值
	double  Y;				//水位值
	SYSTEMTIME CAL_TM;		//统计时间
public:
	Table_ST_DISP_R_Record()
	{
		STCD = "";
		TM = "";
		X = 0.0;
		Y = 0.0;
		GetLocalTime(&CAL_TM);
	}
	Table_ST_DISP_R_Record(Table_ST_DISP_R_Record* pSource)
	{
		if (pSource != NULL)
		{
			STCD = pSource->STCD;
			TM = pSource->TM;
			FTM = pSource->FTM;
			X = pSource->X;
			Y = pSource->Y;
			
			CAL_TM = pSource->CAL_TM;
		}
		else
		{
			STCD = "";
			TM = "";
			X = 0.0;
			Y = 0.0;
			GetLocalTime(&CAL_TM);
		}
	}
	~Table_ST_DISP_R_Record(){}
};

//温度数据
struct Table_ST_TEMP_R_Record
{
public:
	CString STCD;			//测站编码
	CString TM;				//时间点
	SYSTEMTIME FTM;			//完整的时间 XXXX-XX-XX XX:XX:XX.XXX格式，关键：对比时间
	double  T;				//温度值
	SYSTEMTIME CAL_TM;		//统计时间
public:
	Table_ST_TEMP_R_Record()
	{
		STCD = "";
		TM = "";
		T = 0.0;
		GetLocalTime(&CAL_TM);
	}
	Table_ST_TEMP_R_Record(Table_ST_TEMP_R_Record* pSource)
	{
		if (pSource != NULL)
		{
			STCD = pSource->STCD;
			TM = pSource->TM;
			FTM = pSource->FTM;
			T = pSource->T;
			
			CAL_TM = pSource->CAL_TM;
		}
		else
		{
			STCD = "";
			TM = "";
			T = 0.0;
			GetLocalTime(&CAL_TM);
		}
	}
	~Table_ST_TEMP_R_Record(){}
};

//雨量数据 存储于本地SQLite数据库的数据（YDJ上传的数据）
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
	CRainData_Item(CRainData_Item* pSource);

	const int DataType() {return m_nDataType;}
	const double CurrentValue(){return m_dCurrentValue;}
	const double TotalValue(){return  m_dTotalValue;}
	const double SumHalfHour(){return m_dSumHalfHour;}
	const CString CollectTimeString() {return m_sCollectTime;}
	const CString ToString();
public:
	bool Equals(CRainData_Item* pSrc); //比较是否相等 20140904
};

//水位数据 存储于本地SQLite数据库的数据（YDJ上传的数据）
class CWaterL_DataItem
{
public:
	double		m_dValue;				//水位实时值
	SYSTEMTIME  m_RecordTime;			//采集时间
	CString     m_sRecordTime;
public:
	CWaterL_DataItem();
	CWaterL_DataItem(double dValue, SYSTEMTIME& st);
	CWaterL_DataItem(double dValue, SYSTEMTIME& st, CString strSt);
	CWaterL_DataItem(CWaterL_DataItem* pSource);
	CString ToString();
public:
	bool Equals(CWaterL_DataItem* pSrc);  //比较是否相等 20140904

};

//深度位移数据 存储于本地SQLite数据库的数据（YDJ上传的数据）
class CDisplace_DataItem
{
public:
	double		m_XValue;				//X轴实时值
	double		m_YValue;				//Y轴实时值
	//int			m_DisplaceNo;	   //传感器序号
	SYSTEMTIME  m_RecordTime;			//采集时间
	CString     m_sRecordTime;
public:
	CDisplace_DataItem();
	//CDisplace_DataItem(double xValue,double yValue,int iDisplaceNo,SYSTEMTIME& st);
	//CDisplace_DataItem(double xValue,double yValue,int iDisplaceNo, SYSTEMTIME& st, CString strSt);
	CDisplace_DataItem(double xValue,double yValue,SYSTEMTIME& st);
	CDisplace_DataItem(double xValue,double yValue,SYSTEMTIME& st, CString strSt);
	CDisplace_DataItem(CDisplace_DataItem* pSource);
	CString ToString();
public:
	bool Equals(CDisplace_DataItem* pSrc);  //比较是否相等 20140904
};

//2023/5/23 流量数据 存储在本地SQLite的数据库的数据
class CFlow_DataItem
{
public:
	double Sunshiflowdata;     //瞬时流量数据
	double Leijiflowdata;      //累积流量数据
	SYSTEMTIME m_RecordTime;   //采集时间
	CString m_sRecordTime;
public:
	CFlow_DataItem();
	CFlow_DataItem(int data,double Data, SYSTEMTIME& st);
	CFlow_DataItem(double SunData, double LeiData, SYSTEMTIME& st);
	CFlow_DataItem(double SumData, double LeiData, SYSTEMTIME& st, CString strSt);
	CFlow_DataItem(CFlow_DataItem* pSource);
	CString ToString();
public:
	bool Equals(CFlow_DataItem* pSrc);  //比较是否相等 20140904
};

//温度数据 存储于本地SQLite数据库的数据（YDJ上传的数据）
class CTempera_DataItem
{
public:

	double		m_tValue;				//温度实时值
	SYSTEMTIME  m_RecordTime;			//采集时间
	CString     m_sRecordTime;
public:
	CTempera_DataItem();
	CTempera_DataItem(double tValue, SYSTEMTIME& st);
	CTempera_DataItem(double tValue, SYSTEMTIME& st, CString strSt);
	CTempera_DataItem(CTempera_DataItem* pSource);
	CString ToString();
public:
	bool Equals(CTempera_DataItem* pSrc);  //比较是否相等 20140904
};
//测站类型
enum CZ_Type_Enum
{
	CZ_Type_UNKNOWN = 0, //未知类型
	CZ_Type_RAIN = 1,    //雨量站
	CZ_Type_WL	 = 2,    //水位站
	CZ_Type_DISP = 3,    //深度位移传感器
	CZ_Type_TEMP = 4,    //温度传感器
	CZ_Type_FLOW = 5,    //流量传感器
};

//水位测站类型
enum WL_Type_Enum
{
	WL_Type_UNKNOWN = 0,    //未知类型
	WL_Type_HLSW = 1,		//河流水位站
	WL_Type_SKSSW = 2,		//水库上水位站
	WL_Type_SKXSW = 3,		//水库下水位站
};

/*
* 定时统计对象(等待统计对象)
* 包括: 5分钟降雨量
*	    15分钟降雨量（暴雨加保）
*		1小时降雨量
*		1日降雨量
*		整点水位统计[少]
*/
struct StatisticsOnTimer_Item
{
public: 
	CString STCD;						//主站编码
	CZ_Type_Enum Type;					//类型 1 雨量站、2 水位站
	WL_Type_Enum WLType;				//类型 1 河流水位、2 水位上水位、3 水库下水位
	int RTUNo;							//传感器序号
	CString strTM;						//入库时间点（水位的CollectTime）
	RIANSTATISTICS_TYPE_Enum Cal_Type;	//统计类型
	CString strFTM;						//入库时间点 格式：yyyy-mm-dd HH:MM:ss.s3
	CString strBegin;				    //统计时间范围开始时间
	CString strEnd;						 //统计时间范围结束时间
	SYSTEMTIME stFTM;					//入库时间点 格式:yyyy-mm-dd HH:MM:ss.s3
	SYSTEMTIME stBegin;					//开始时间
	SYSTEMTIME stEnd;					//结束时间
	SYSTEMTIME CAL_TM;					//记录时间

	double     PV;						//水位的值  (水位值采用来点统计)
	double     X;						//位移X的值  (水位值采用来点统计)
	double     Y;						//位移Y的值  (水位值采用来点统计)
	double     T;						//温度的值  (水位值采用来点统计)

	double    Sunflowdata;              //瞬时流量的值
	double    Leiflowdata;              //累积流量的值

	SYSTEMTIME Collect;					//水位采集时间
public:
	StatisticsOnTimer_Item();
	StatisticsOnTimer_Item(StatisticsOnTimer_Item* pSource);  //拷贝构造函数
	~StatisticsOnTimer_Item(){}								
	const CString ToString();
};

const ULONG BEGIN_CALC_INDEX = 0;
const ULONG LAST_CALC_INDEX  = 5;

//YDJ上传数据内存缓存（原始上传数据）
//保存最近3天的数据至内存缓存
//当前保存的数据位为：雨量数据、水位数据
//Added 20140901,for:性能优化
class YDJUpData_Cache
{
public:
	YDJUpData_Cache();
	~YDJUpData_Cache(){Clear();}
public:
	SYSTEMTIME m_stCacheAreaBegin;								//缓存的开始时间
	SYSTEMTIME m_stCacheAreaEnd;								//缓存的结束时间
public:
	std::multimap<CString,CRainData_Item*> m_mapRainData;		//雨量上送数据缓存
	std::multimap<CString,CWaterL_DataItem*> m_mapWaterLData;	//水位上送数据缓存
	std::multimap<CString,CDisplace_DataItem*> m_mapDisplaceData;	//位移上送数据缓存
	std::multimap<CString,CTempera_DataItem*> m_mapTemperaData;	//位移上送数据缓存
	
	std::multimap<CString, CFlow_DataItem*> m_mapFlowData;	//流量上送数据缓存

public:
	SYSTEMTIME m_stRainBeginTime;								//雨量数据开始时间
	SYSTEMTIME m_stRainEndTime;									//雨量数据结束时间
	SYSTEMTIME m_stWaterLBeginTime;								//水位数据开始时间
	SYSTEMTIME m_stWaterLEndTime;								//水位数据结束时间
	SYSTEMTIME m_stDisplaceBeginTime;							//深度位移数据开始时间
	SYSTEMTIME m_stDisplaceEndTime;								//深度位移数据结束时间
	SYSTEMTIME m_stTemperaBeginTime;							//温度数据开始时间
	SYSTEMTIME m_stTemperaEndTime;								//温度数据结束时间

	//2023-5-23
	SYSTEMTIME m_stFlowBeginTime;							    //流量数据开始时间
	SYSTEMTIME m_stFlowEndTime;								    //流量数据结束时间
	
public:
	bool InsertData(CRainData_Item* pRain);						//插入雨量上传数据
	bool InsertData(CWaterL_DataItem* pWaterL);					//插入水位上传数据
	bool InsertData(CDisplace_DataItem* pDisplace);			    //插入位移上传数据
	bool InsertData(CTempera_DataItem* pTempera);				//插入温度上传数据

	//2023-5-23
	bool InsertData(CFlow_DataItem* pFlowdata);                 //插入流量上传数据

	void SetUpDataCacheTimeArea(SYSTEMTIME AreaBegin,SYSTEMTIME AreaEnd);	 //设置缓存时间范围
	void RemoveOutDataOnTimer();								//定时清除不在时间范围内的数据
private:
	void RemoveRainData(CString strTime);
	void RemoveWaterLData(CString strTime);
	void RemoveDisplaceData(CString strTime);
	void RemoveTemperaData(CString strTime);

	//2023-5-23
	void RemoveFlowData(CString strTime);

private:
	void Clear();												//清除内存
};

//地址映射相关
class CAddrMap_Item
{
private:
	CString m_RTU_ID;					//510101_00000000
	CZ_Type_Enum     m_Type;			//类型 1：雨量站  2:水位站
	WL_Type_Enum     m_wlType;			//水位类型 1：河道水位 2：水库上水位 3：水库下水位
	int m_RTU_No;						//传感器序号
	CString m_STCD;						//测站编码

	CString m_addVCode;					//行政区划码 6位  如：510101
	int     m_RtuAddr;					//遥测终端地址 0
	int     m_RtuNo;					//遥测终端传感器序号 1

	bool    m_blCalc;					//是否按时统计，默认: false
	bool    m_blCalcHalf;				//半小时统计    //雨量统计时使用

	ULONG   m_nCalcIndex;				//统计计数 0-5
public:
	CAddrMap_Item();
	CAddrMap_Item(CString stCd,
		int     Type,
		int     wlType,
		CString AddVCode,
		int     RTUAddr,
		int RTUNo);
	~CAddrMap_Item(){ Clear();}
private:
	void Clear();  //清除map集合的数据
public:
	CString Get_RTU_ID()		{return m_RTU_ID;}
	CString Get_STCD()			{return m_STCD;}
	CString Get_ADDVCode()		{return m_addVCode;}
	int     Get_RtuAddr()		{return m_RtuAddr;}
	int     Get_RtuNo()			{return m_RtuNo;}
	CZ_Type_Enum GetType()		{return m_Type;}
	WL_Type_Enum GetWlType()	{return m_wlType;}

	//设置是或获取按时统计的标志
	void    SetCalcState(bool _value) { m_blCalc  = _value;}
	const bool GetCalcState() {return m_blCalc;}

	void   SetCalcHalfState(bool _value) { m_blCalcHalf = _value;}
	const bool GetCalcHalfState()	{return m_blCalcHalf;}

	void  SetCalcIndex(ULONG nIdx)	{m_nCalcIndex = nIdx;}
	const ULONG GetCalcIndex()		{return m_nCalcIndex;}

	//added 20140813 sc.cache 实时统计内存缓存
public:
	std::map<CString,Table_ST_PPTN_R_Record*> m_mapST_PPTN_R;		//雨水情统计缓存记录
	std::map<CString,Table_ST_STORM_R_Record*> m_mapST_STROM_R;		//降雨加报统计缓存记录
	std::map<CString,Table_ST_WL_R_Record*> m_mapST_WL_R;			//水位统计缓存记录
	std::map<CString,Table_ST_DISP_R_Record*> m_mapST_DISP_R;			//位移统计缓存记录
	std::map<CString,Table_ST_TEMP_R_Record*> m_mapST_TEMP_R;			//温度统计缓存记录

	//2023-5-23
	std::map <CString, Table_ST_RIVER_R_Record*> m_mapST_RIVER_R;   //瞬时流量缓存记录
	std::map <CString, Table_ST_RSVR_R_Record*> m_mapST_RSVR_R;      //累计流量缓存记录


	bool UpdateST_PPTN_R_Record(Table_ST_PPTN_R_Record* pUpdateR);	//更新缓存中的雨水情记录（INSERT、UPDATE）
	bool RemoveST_PPTN_R_Record(CString strTM);						//从缓存中移除雨水情记录
	bool UpdateST_STROM_R_Record(Table_ST_STORM_R_Record* pUpdateR);//更新降雨加报的缓存记录（INSERT、UPDATE）
	bool RemoveST_STROM_R_Record(CString strTM);					//从缓存中移除降雨加报记录

	//2023-5-23
	bool UpdateST_RIVER_R_Record(Table_ST_RIVER_R_Record* pUpdateR);                //更新RIVER表里面的记录
	bool RemoveST_RIVER_R_Record(CString strTM);									//从缓存中移除瞬时流量记录
	bool UpdateST_RSVR_R_Record(Table_ST_RSVR_R_Record* pUpdateR);                  //更新RSVR表里面的数据
	bool RemoveST_RSVR_R_Record(CString strTM);                                     //从缓存中移除累积流量记录

	//bool UpdateST_WL_R_Record(Table_ST_WL_R_Record* pUpdateR);    //更新水位缓存的记录(INSERT,UPDATE)
	int UpdateST_WL_R_Record(Table_ST_WL_R_Record* pUpdateR);       //更新水位缓存的记录(INSERT,UPDATE)
	bool RemoveST_WL_R_Record(CString strTM);						//从缓存中移除水位记录	
	bool RemoveST_WL_R_Record(CString strBegin,CString strEnd);     //删除一个时间段的水位统计数据
	bool UpdateST_DISP_R_Record(Table_ST_DISP_R_Record* pUpdateR);	//更新缓存中的位移记录（INSERT、UPDATE）
	bool RemoveST_DISP_R_Record(CString strTM);						//从缓存中移除位移记录
	bool UpdateST_TEMP_R_Record(Table_ST_TEMP_R_Record* pUpdateR);	//更新缓存中的温度记录（INSERT、UPDATE）
	bool RemoveST_TEMP_R_Record(CString strTM);						//从缓存中移除温度记录

public: //性能改进 20140901
	std::map<CString,StatisticsOnTimer_Item*> m_mapStatistics_Rain;			//雨量定时统计对象
	std::map<CString,StatisticsOnTimer_Item*> m_mapStatistics_WaterL;		//水位定时统计对象
	std::map<CString,StatisticsOnTimer_Item*> m_mapStatistics_Displace;		//位移定时统计对象
	std::map<CString,StatisticsOnTimer_Item*> m_mapStatistics_Tempera;		//温度定时统计对象

	//2023-5-23
	std::map<CString, StatisticsOnTimer_Item*> m_mapStatistics_Flow;        //流量定时统计对象

	bool UpdateStatisticsOnTimerItem(StatisticsOnTimer_Item* pUpdateR);		//更新对象(INSERT,UPDATE)
	bool RemoveStatisticsOnTimerItem(CZ_Type_Enum Type,CString strTM);		//当统计完成后移除该对象
public:
	YDJUpData_Cache m_UpDataCache;											//YDJ上传数据内存缓存
	bool InsertData(CRainData_Item* pRain);
	bool InsertData(CWaterL_DataItem* pWaterL);	
	bool InsertData(CDisplace_DataItem* pDislace);
	bool InsertData(CTempera_DataItem* pTempera);
	bool InsertData(CFlow_DataItem* pFlowdata);

	void SetUpDataCacheTimeArea(SYSTEMTIME AreaBegin,SYSTEMTIME AreaEnd);	 //设置缓存时间范围
};

//定时检查对象
//add 20140903
struct CheckOnTimer_Item
{
public: 
	DWORD m_nInterval;			//检查时间间隔
	DWORD m_nPreTickCount;		//上一次触发的TickCount;
public:
	CheckOnTimer_Item()
	{
		m_nInterval = 0;
		m_nPreTickCount = 0;
	}
};

struct RainStatistics_Early{
public:
	DWORD  m_nScStatisticsCheck5mEarly;	    //5分钟提前统计时间,默认提前1分钟   1*60 单位：秒 20140905
	DWORD  m_nScStatisticsCheck15mEarly;    //15分钟提前统计时间，默认提前2分钟 1*60 单位：秒 20140905
	DWORD  m_nScStatisticsCheck1hEarly;		//1h提前提前统计时间，默认提前3分钟 1*60 单位：秒 20140905
	DWORD  m_nScStatisticsCheck1dEarly;		//1d提前提前统计时间，默认提前3分钟 1*60 单位：秒 20140905
public:
	RainStatistics_Early()
	{
		m_nScStatisticsCheck15mEarly = 0;
		m_nScStatisticsCheck1dEarly = 0;
		m_nScStatisticsCheck1hEarly = 0;
		m_nScStatisticsCheck5mEarly = 0;
	}
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
	void SetLoclTransSQLite(CDataTranSQLite_Local* pLocal);
protected:
	//added 20140812
	CString m_strScCacheDBName;
	bool m_blOpenScCacheSucc;									//是否已经打开SQLite文件
	bool OpenScCacheSQLite();									//打开实时统计缓存SQLite数据库
	void CheckScCacheSQLite();									//检查实时统计缓存SQLite数据库结构
	void CheckScCacheTableExist_CurState();						//当前实时统计的状态
	void CheckScCacheTableExist_CacheHis();
	void CheckScCacheTableExist_StPptnR();
	void CheckScCacheTableExist_StStormR();
	void CheckScCacheTableExist_StWlR();

	void CheckScCacheTableExist_StDISPR();
	void CheckScCacheTableExist_StTEMPR();

	//2023-5-23
	void CheckScCacheTableExist_STRIVER();
	void CheckScCacheTableExist_STRSVR();

	void CheckScCacheTableExist_STCObjects();
	bool IsExistScCacheTable( CString strTable );				//检查SQLite库中的表是否存在
	bool ExcuteScCacheNoQuery(CString sSQL);					//在sc.cache.db中执行不查询的
	CSqlStatement* ExcuteScCacheQuery(CString strSQL);			//再sc.cache.db的SQLite中查询数据
	
	//统计缓存定时任务 20140815
public:
	void SetLocalSQLiteTransStartFlag(bool blStartFlag);		//设置本地SQLite事务的标志			
	void ScCacheInSQLServer_OnTimer();							//统计缓存定时存省标库(定时任务)
	void ScCacheSaveToSQLite_OnTimer();							//统计缓存定时存SQLite文件(定时任务)
	void ScStatistics_OnTimer();								//定时统计，将实时统计修改为定时统计，20140903
	void ScUpdataCacheCheck_OnTimer();							//定时检查内存缓存，将不是范围内的数据移除内存缓存，20140903
	void CurStatisticsState(int nCount,SYSTEMTIME& collTime,SYSTEMTIME& recvTime);//监测当前是否有挤压报文
private:
	int m_nWaittingBufferCount;									//当前未处理的报文积压数
	SYSTEMTIME curDealCollectTime;							    //当前处理的报文时间
	SYSTEMTIME curDealReciveTime;								//当前处理报文的接收时间
private:
	bool m_blLocalSQliteTransStartFlag;							//本地SQLite的事务是否启动的标志,true：已启动，不能从本地SQLite读取数据， false:未启动事务，可以从本地SQLite读取数据
	DWORD m_dwInSQLServer_Interval;								//存入省标库的时间间隔，单位：毫秒
	DWORD m_dwSaveToSQLite_Interval;							//存入SQLite哭的时间间隔，单位：毫秒
	DWORD m_dwPreInSQLServerTC;									//上次存SQLServer的TickCount,用于定时器检查
	DWORD m_dwPreSaveToSQLiteTC;								//上次存SQLite的TickCount,用于定时检查

	CheckOnTimer_Item m_ScStatisticsTimer;						//定时统计
	CheckOnTimer_Item m_ScUpdataCacheTimer;						//定时检查内存缓存

	void ScCacheInSQLServer();									//统计缓存存入省标库功能函数
	DWORD m_dwChcekInSecond;									//统计缓存存入省标库时间检查
	void ScCacheSaveToSQLite();									//统计缓存村人SQLite

	void ScStatisticsCheck();									//定时统计功能
	void ScStatisticsCheck_Rain(CAddrMap_Item* pAddrItem);		//从缓存中获取等待统计的雨量项，进行雨量统计功能，统计完成后，移除等待项
	void ScStatisticsCheck_WaterLevel(CAddrMap_Item* pAddrItem);//从缓存中获取等待统计的水位项，进行水位统计功能，统计完成后，移除等待项
	void ScStatisticsCheck_Displace(CAddrMap_Item* pAddrItem);//从缓存中获取等待统计的位移项，进行位移统计功能，统计完成后，移除等待项
	void ScStatisticsCheck_Tempera(CAddrMap_Item* pAddrItem);//从缓存中获取等待统计的温度项，进行温度统计功能，统计完成后，移除等待项

	//2023-5-23
	void ScStatisticsCheck_Flow(CAddrMap_Item* pAddrItem);  //从缓存中获取等待统计的温度项，进行温度统计功能，统计完成后，移除等待项

	void ScUpdataCahceCheck();									//定时检查内存功能

	void ReadScCacheSQLiteByStart();							//启动时读取sc.cache.db数据
	void ReadScCacheSQLite_ST_PPTN_R();							//读取表ST_PPTN_R

	//2023-5-23
	void ReadScCacheSQlite_FLOWV_DATA();                        //读取表FLOWV_DATA

	void ReadScCacheSQLite_ST_STORM_R();						//读取表ST_STORM_R
	void ReadScCacheSQLite_ST_WL_R();							//读取表ST_WL_R
	void ReadScCacheSQLite_ST_DISP_R();							//读取表ST_DISP_R
	void ReadScCacheSQLite_ST_TEMP_R();							//读取表ST_TEMP_R
	void ReadScCacheSQlite_STCObjects();						//读取表ST_OBJECTS

	void ReadLocalSQLiteTOUpDataCache();									//将本地SQLite的数据读取到上传缓存中
	void CalUpDataCacheTimeArea(SYSTEMTIME& AreaBegin, SYSTEMTIME& AreaEnd); //计算本地内存缓存的时间范围 3天时间

	//add 20140816 时间段计算
private:
	STRUCT_BEGINEND_AREA Get5MinuteArea(SYSTEMTIME* pST = NULL);       //获取pST所在的5分钟开始结束时间区域,当pST = NULL,计算当前时间
	STRUCT_BEGINEND_AREA Get15MinuteArea(SYSTEMTIME* pST= NULL);	   //获取pST所在的15分钟开始结束时间区域,当pST = NULL,计算当前时间
	STRUCT_BEGINEND_AREA Get1HourArea(SYSTEMTIME* pST= NULL);		   //获取pST所在的小时的开始结束时间区域,当pST = NULL,计算当前时间
	STRUCT_BEGINEND_AREA Get1DayArea(SYSTEMTIME* pST = NULL);		   //获取pST所在的1天的开始结束时间区域,当pST = NULL,计算当前时间
protected:	//SQL Server 操作	
	bool OpenDatabase();
	bool OpenDatabase(CString dataSource);	
	bool GetConnectionState();
	bool CheckTableExist(CString sTableName); //检查数据库中标是否存在, true:表存在,false:表不存在
	bool ExcuteNoQuery(CString sSQL);
private:
	bool       m_blStartCalc;				  //是否执行统计
	bool	   m_blCalc;
	CDbSQLite*  m_local_sqlite;				  //本地存储遥测终端机原始数据SQLite数据文件,在DataTranSQLite_Local模块中打开

	CDataTranSQLite_Local* m_localTrans;

	//本地存储实时统计的缓存SQLite数据文件
	CDbSQLite m_scChace_sqlite;				

	//雨量最大值正确性检查
	double		m_dCHECK_RAIN_PER; //检查5、15、1hour的值
	double		m_dCHECK_RAIN_DAY; //检查1day的值 

	//雨量统计时间检查
	UINT	    m_nCheck5Minutes_MNT;
	UINT		m_nCheck15Minutes_MNT;
	UINT		m_nCheck1Hour_MNT;
	UINT		m_nChcek1Day_MNT;

	//水位数据上传方式
	BYTE        m_nWaterLevaeIType;

	//是否需要统计降雨历时（0：不统计、1：统计）
	BYTE        m_RainPRDFlag; 

	BYTE m_nDatabaseType;					  //数据库类型 1:SQL Server 2: Oracle (Added 20130509)
private:	//数据库操作,统计存库
	void CheckDatabase();					 //检查数据存库对应的表是否存在

	//时间>= strStart 的一个值 ，从本地SQLite
	CRainData_Item* GetStartTimeRainData(CAddrMap_Item* pAddrItem,CString strStart);

private:	//sqlite数据库操作
	//bool OpenSqlite();
	CSqlStatement* ExcuteSQLiteQuery(CString strSQL);							//查询

private:	//地址映射(遥测终端机与主站测站编码的地址映射)
	bool InitAddressMap();														//初始化地址映射
	void CheckAddressMap_ForRtu();												//检查地址映射表的正确性 与遥测终端机的映射关系
	void CheckAddressMap_ForMaster();											//检查地址映射表的正确性 与主站ST_STBPRP_B表的配置数据进行检查
	//std::map<CString,CAddrMap_Item*>  m_mapAddressMap;						//地址映射
	std::list<CAddrMap_Item*> m_mapAddressMap;									//地址映射 针对相同的攀枝花有STCD 修改 20120803
	void ClearAddrMap();
private:	//水位
	void CalcWaterLevel_RealTime(CString Rtu_ID,CWaterL_DataItem* pRecvData);								//来点统计(水位)
private:   //水位统计 old
	void StatisticsWaterLevel_RealTime_Default(CAddrMap_Item* pAddrItem,CWaterL_DataItem* pRecvData);		//对采用遥测终端机实时上送的水位进行入库
	void StatisticsWaterLevel_RealTime_SBK(CAddrMap_Item* pAddrItem,CWaterL_DataItem* pRecvData);			//来点统计整点的水位和加报再入库
	void StatisticsWaterLevel_Hour_AppendReport( CAddrMap_Item* pAddrItem, SYSTEMTIME begintime, SYSTEMTIME endtime ); //1小时水位加报统计
	//从本地SQLite数据库 获取1个小时最后的水位数据(水位数据  1 hours  0 < strpoint < end内)
	CWaterL_DataItem* GetLatestWaterLevelData(CAddrMap_Item* pAddrItem,CString strPoint);

private: //操作水位省标库
	int UpdateWaterLevelData( CAddrMap_Item* pAddrItem, CString strTM, double pointValue);
	int UpdateFlowData1(CAddrMap_Item* pAddrItem,double value, CString strTM);                             //瞬时流量
	int UpdateFlowData2(CAddrMap_Item* pAddrItem, double value, CString strTM);                            //累计流量
	bool GetWaterLevelData(CString sSQL,double& WaterLevelData);
	CWaterL_DataItem* GetWaterLevelBasePoint( CAddrMap_Item* pAddrItem, CWaterL_DataItem* pRecvData );

private:	//雨量
	void CalcRainData_RealTime(CString Rtu_ID,CRainData_Item* pRecvData);					   //来点统计(雨量)
private:    //雨量统计 OLD
	void StatisticsRain_RealTime(CAddrMap_Item* pAddrItem,CRainData_Item* pRecvData);	
	void StatisticsRain_5Minutes(CAddrMap_Item* pAddrItem,CRainData_Item* pRecvData);		   //5分钟雨量累计值统计
	void StatisticsRain_15Minutes(CAddrMap_Item* pAddrItem,CRainData_Item* pRecvData);		   //15分钟雨量累计值统计
	void StatisticsRain_1Hour(CAddrMap_Item* pAddrItem,CRainData_Item* pRecvData);			   //1个小时雨量累计值统计
	void StatisticsRain_1Day(CAddrMap_Item* pAddrItem,CRainData_Item* pRecvData);
	//double StatisticsRain_1Day(CAddrMap_Item* pAddrItem,CString strBegin,CString strEnd,CRainData_Item* pRecvData=NULL);      //1日雨量统计
	//如果用累计值计算出现大数据>=500时,才调用特殊处理(采用半小时累计值进行计算)
	double StatisticsRain_1Hour_special(CAddrMap_Item* pAddrItem,SYSTEMTIME sttime,CRainData_Item* pRecvData = NULL);
	//如果用累计值计算出现大数据>=500时,才调用特殊处理(采用半小时累计值进行计算)
	double StatisticsRain_1Day_special(CAddrMap_Item* pAddrItem,SYSTEMTIME sttime,CRainData_Item* pRecvData = NULL);
	//计算降雨历时
	int  GetRainMinutes( CAddrMap_Item* pAddrItem, CString strBegin, CString strEnd );
	//时间< strend的最后一个雨量值,从本地SQLite
	CRainData_Item*  GetEndTimeRainData(CAddrMap_Item* pAddrItem,CString strEnd);
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

private: //操作省标库雨量数据
	//更新降雨量表ST_PPTN_R的记录
	bool UpdateRainfallTableRecord(Table_ST_PPTN_R_Record& sRecord,RIANSTATISTICS_TYPE_Enum xType);

	//从降雨量表ST_PPTN_R的查找记录(UPDATE or  INSERT)
	bool GetRainfallTableRecord(CString sSQL,Table_ST_PPTN_R_Record& findRecord);	

	//更新暴雨加报表ST_STORM_R的记录(UPDATE or  INSERT)
	bool UpdateRainstormTableRecord(Table_ST_STORM_R_Record& sRecord);

	//获取暴雨加报表ST_STORM_R的记录
	bool GetRainstormRecord(CString sSQL,Table_ST_STORM_R_Record& findRecord);

private: 
		//优化1(雨量统计)
		//优化内容:5分钟、15分钟、1小时、1天降雨量的统计原始数据优化为只从本地SQLite读取一次
		void StatisticsRain_Optimize1_RealTime(CAddrMap_Item* pAddrItem,CRainData_Item* pRecvData);
		STRUCT_BEGINEND_AREA StatisticsRain_Optimize1_GetTotalTimeArea(CRainData_Item* pRecvData);
		std::multimap<CString,CRainData_Item*> StatisticsRain_Optimize1_GetTotalRainData(CAddrMap_Item* pAddrItem,STRUCT_BEGINEND_AREA TimeArea);
		void StatisticsRain_Optimize1_5Minutes( CAddrMap_Item* pAddrItem, CRainData_Item* pRecvData, std::multimap<CString,CRainData_Item*>& RainDataMap );
		void StatisticsRain_Optimize1_15Minutes( CAddrMap_Item* pAddrItem, CRainData_Item* pRecvData, std::multimap<CString,CRainData_Item*>& RainDataMap );
		void StatisticsRain_Optimize1_1Hour( CAddrMap_Item* pAddrItem, CRainData_Item* pRecvData, std::multimap<CString,CRainData_Item*>& RainDataMap );
		void StatisticsRain_Optimize1_1Day( CAddrMap_Item* pAddrItem, CRainData_Item* pRecvData, std::multimap<CString,CRainData_Item*>& RainDataMap );
		double CalRainData_Optimize1_OnTimeRange( CAddrMap_Item* pAddrItem, CString strBegin, CString strEnd, RIANSTATISTICS_TYPE_Enum xType, 
			bool& isNull, std::multimap<CString,CRainData_Item*>& RainDataMap );
		CRainData_Item* GetEndTimeRainData_Optimize1( CAddrMap_Item* pAddrItem, CString strEnd, std::multimap<CString,CRainData_Item*>& RainDataMap );
		double StatisticsRain_Optimize1_1Hour_special( CAddrMap_Item* pAddrItem,SYSTEMTIME sttime,CRainData_Item* pRecvData , std::multimap<CString,CRainData_Item*>& RainDataMap );
		double StatisticsRain_Optimize1_1Day_special( CAddrMap_Item* pAddrItem,SYSTEMTIME sttime,CRainData_Item* pRecvData , std::multimap<CString,CRainData_Item*>& RainDataMap );
		int  GetRainMinutes_Optimize1( CAddrMap_Item* pAddrItem, CString strBegin, CString strEnd,std::multimap<CString,CRainData_Item*>& RainDataMap );
		int StatisticsRain_Optimize1_CompareEndTime(SYSTEMTIME& srctime,std::multimap<CString,CRainData_Item*>& RainDataMap );
		void StatisticsRain_Optimize1_End(std::multimap<CString,CRainData_Item*>& RainDataMap);


private: 
	    //优化1（水位统计）
	    //优化内容：预先将统计所需的水位原始数据从本地SQLite进行1次性读取
		void StatisticsWaterLevel_Optimize1_RealTime_SBK(CAddrMap_Item* pAddrItem,CWaterL_DataItem* pRecvData);			//来点统计整点的水位和加报再入库
		void StatisticsWaterLevel_Optimize1_Hour_AppendReport( CAddrMap_Item* pAddrItem, SYSTEMTIME begintime, SYSTEMTIME endtime,std::multimap<CString,CWaterL_DataItem*>& WLDataMap);
		std::multimap<CString,CWaterL_DataItem*> StatisticsWaterLevel_Optimize1_GetWaterLData( CAddrMap_Item* pAddrItem, CWaterL_DataItem* pRecvData );
		CWaterL_DataItem* GetLatestWaterLevelData_Optimize1(CAddrMap_Item* pAddrItem,CString strPoint,std::multimap<CString,CWaterL_DataItem*>& WLDataMap);

private: 
	//优化2（雨量统计）
	RainStatistics_Early m_RainStcEarly;
	void StatisticsRain_Optimize2_Enter(CAddrMap_Item* pAddrItem,CRainData_Item* pRecvData); //YDJ上送的雨量数据入口
	void StatisticsRain_Optimize2_5MinutesTimeArea(CRainData_Item* pRecvData,SYSTEMTIME& areaBegin,SYSTEMTIME& areaEnd, CString& strTM);		//5分钟降雨量时间范围
	void StatisticsRain_Optimize2_15MinutesTimeArea(CRainData_Item* pRecvData,SYSTEMTIME& areaBegin,SYSTEMTIME& areaEnd,CString& strTM);		//15分钟降雨量时间范围
	void StatisticsRain_Optimize2_1HourTimeArea(CRainData_Item* pRecvData,SYSTEMTIME& areaBegin,SYSTEMTIME& areaEnd,CString& strTM);			//1小时降雨量时间范围
	void StatisticsRain_Optimize2_1DayTimeArea(CRainData_Item* pRecvData,SYSTEMTIME& areaBegin,SYSTEMTIME& areaEnd,CString& strTM);				 //1天降雨量时间范围
	std::multimap<CString,CRainData_Item*> StatisticsRain_Optimize2_GetRainDataFromSQLite(CAddrMap_Item* pAddrItem,CString strBegin,CString StrEnd);
	std::multimap<CString,CRainData_Item*> StatisticsRain_Optimize2_GetRainDataFromUpDataCache(CAddrMap_Item* pAddrItem,CString strBegin,CString strEnd);
	void StatisticsRain_Optimize2_1Day(CAddrMap_Item* pAddrItem,SYSTEMTIME stbegin,SYSTEMTIME stend,std::multimap<CString,CRainData_Item*>& RainDataMap);
	//void StatisticsRain_Optimize2_1Hour(CAddrMap_Item* pAddrItem,SYSTEMTIME stbegin,SYSTEMTIME stend,std::multimap<CString,CRainData_Item*>& RainDataMap); 
	void StatisticsRain_Optimize2_1Hour(CAddrMap_Item* pAddrItem,SYSTEMTIME stbegin,SYSTEMTIME stend,SYSTEMTIME stpoint, std::multimap<CString,CRainData_Item*>& RainDataMap);
	void StatisticsRain_Optimize2_15Minutes(CAddrMap_Item* pAddrItem,SYSTEMTIME stbegin,SYSTEMTIME stend,std::multimap<CString,CRainData_Item*>& RainDataMap);
	void StatisticsRain_Optimize2_5Minute(CAddrMap_Item* pAddrItem,SYSTEMTIME stbegin,SYSTEMTIME stend,std::multimap<CString,CRainData_Item*>& RainDataMap);
	//double CalRainData_Optimize2_OnTimeRange( CAddrMap_Item* pAddrItem, CString strBegin, CString strEnd, RIANSTATISTICS_TYPE_Enum xType, 
	//	bool& isNull, std::multimap<CString,CRainData_Item*>& RainDataMap );

public:
	void StatisticsRain_Optimize2_1Day_OnTimer();			//每天早上8点进行日降雨量统计

private://优化2 （水位统计） 20140903
	void StatisticsWaterLevel_Optimize2_Enter(CAddrMap_Item* pAddrItem,CWaterL_DataItem* pRecvData);																	//水位上传数据入口函数，主要是计算出需要读取水位的值
    std::multimap<CString,CWaterL_DataItem*> StatisticsWaterLevel_Optimize2_GetWaterLDataFromSQLite( CAddrMap_Item* pAddrItem,CString strBegin,CString strEnd);			//从本地SQLite读取水位基础数据
	std::multimap<CString,CWaterL_DataItem*> StatisticsWaterLevel_Optimize2_GetWaterLDataFromUpDataCache( CAddrMap_Item* pAddrItem,CString strBegin,CString strEnd);	//从内存缓存中读取水位基础数据
	void StatisticsWaterLevel_Optimize2_RealTime_SBK(CAddrMap_Item* pAddrItem,CWaterL_DataItem* pRecvData,std::multimap<CString,CWaterL_DataItem*>& WlDataMap);			//来点统计整点的水位和加报再入库

private://深度位移入省标库
	void CalcDisplace_RealTime(CString Rtu_ID,int Rtu_No,CDisplace_DataItem* pRecvData);					   //来点统计(位移)
	void StatisticsDisplace_Optimize2_Enter(CAddrMap_Item* pAddrItem,CDisplace_DataItem* pRecvData);														//上传深度位移数据入口函数，主要是计算出需要读取的值
    std::multimap<CString,CDisplace_DataItem*> StatisticsDisplace_Optimize2_GetDisplaceDataFromSQLite( CAddrMap_Item* pAddrItem,CString strBegin,CString strEnd);		//从本地SQLite读取深度位移基础数据
	std::multimap<CString,CDisplace_DataItem*> StatisticsDisplace_Optimize2_GetDisplaceDataFromUpDataCache( CAddrMap_Item* pAddrItem,CString strBegin,CString strEnd);	//从内存缓存中读取深度位移基础数据
	void StatisticsDisplace_Optimize2_RealTime(CAddrMap_Item* pAddrItem,CDisplace_DataItem* pRecvData,std::multimap<CString,CDisplace_DataItem*>& DispDataMap);	//来点统计整点的深度位移和加报再入库
	int UpdateDisplaceData( CAddrMap_Item* pAddrItem, CString strTM, double xValue, double yValue);
	bool GetDisplaceData(CString sSQL,double& xData,double& yData);
	CDisplace_DataItem* GetDisplaceBasePoint( CAddrMap_Item* pAddrItem, CDisplace_DataItem* pRecvData );

private://温度入省标库
	void CalcTempera_RealTime(CString Rtu_ID,CTempera_DataItem* pRecvData);					   //来点统计(位移)
	void StatisticsTempera_Optimize2_Enter(CAddrMap_Item* pAddrItem,CTempera_DataItem* pRecvData);														//上传温度数据入口函数，主要是计算出需要读取的值
    std::multimap<CString,CTempera_DataItem*> StatisticsTempera_Optimize2_GetTemperaDataFromSQLite( CAddrMap_Item* pAddrItem,CString strBegin,CString strEnd);		//从本地SQLite读取温度基础数据
	std::multimap<CString,CTempera_DataItem*> StatisticsTempera_Optimize2_GetTemperaDataFromUpDataCache( CAddrMap_Item* pAddrItem,CString strBegin,CString strEnd);	//从内存缓存中读取温度基础数据
	void StatisticsTempera_Optimize2_RealTime(CAddrMap_Item* pAddrItem,CTempera_DataItem* pRecvData,std::multimap<CString,CTempera_DataItem*>& TempDataMap);	//来点统计整点的温度和加报再入库
	int UpdateTemperaData( CAddrMap_Item* pAddrItem, CString strTM, double tValue);
	bool GetTemperaData(CString sSQL,double& tData);
	CTempera_DataItem* GetTemperaBasePoint( CAddrMap_Item* pAddrItem, CTempera_DataItem* pRecvData );

//2023-5-23
private://流量入省标库
	void CFlow_RealTime(CString Rtu_ID, CFlow_DataItem* pRecvData);					   //来点统计(流量)
	void StatisticsFlow_Optimize2_Enter(CAddrMap_Item* pAddrItem, CFlow_DataItem* pRecvData);
	std::multimap<CString, CFlow_DataItem*> StatisticsFlow_Optimize2_GetFlowDataFromSQLite(CAddrMap_Item* pAddrItem, CString strBegin, CString strEnd);		//从本地SQLite读取流量基础数据
	std::multimap<CString, CFlow_DataItem*> StatisticsFlow_Optimize2_GetFlowDataFromUpDataCache(CAddrMap_Item* pAddrItem, CString strBegin, CString strEnd);	//从内存缓存中读取流量数据
	void StatisticsFlow_Optimize2_RealTime(CAddrMap_Item* pAddrItem, CFlow_DataItem* pRecvData, std::multimap<CString, CFlow_DataItem*>& FlowDataMap);	//来点统计整点的流量和加报再入库
	int UpdateSunFlowData(CAddrMap_Item* pAddrItem, CString strTM, double SunflowValue);
	int UpdateLeiFlowData(CAddrMap_Item* pAddrItem, CString strTM, double LeiflowValue);
	bool GetFlowData1(CString sSQL, double& SunFlowData);
	bool GetFlowData2(CString sSQL, double& LeiFlowData);
	CFlow_DataItem* GetSunFlowBasePoint(CAddrMap_Item* pAddrItem, CFlow_DataItem* pRecvData);
	CFlow_DataItem* GetLeijiFlowBasePoint(CAddrMap_Item* pAddrItem, CFlow_DataItem* pRecvData);


private:
	void ClearMultimap(std::multimap<CString,CRainData_Item*>& dataMap);
	void ClearMultimap(std::multimap<CString,CWaterL_DataItem*>& dataMap);
	void ClearMultimap(std::multimap<CString,CDisplace_DataItem*>& dataMap);
	void ClearMultimap(std::multimap<CString,CTempera_DataItem*>& dataMap);

	//2023-5-23
	void ClearMultimap(std::multimap<CString, CFlow_DataItem*>& dataMap);
}; 



