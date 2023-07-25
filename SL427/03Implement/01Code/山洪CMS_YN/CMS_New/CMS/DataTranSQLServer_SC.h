#pragma once
#include "datatran_base.h"
#include "WaitTimer.h"
#include <map>
#include <list>


const double RainFullScale_CONST = 9999999.9;  //�����ۼ�ֵ���̶�

//����ͳ������
enum RIANSTATISTICS_TYPE_Enum
{
	Rain_statistics_5minutes	= 0,  //5����ͳ��
	Rain_statistics_15minutes	= 1,  //15����ͳ��
	Rain_statistics_1hour		= 2,  //1Сʱͳ��
	Rain_statistics_AM8Point	= 3,  //����8��ͳ��
};

struct Table_ST_PPTN_R_Record
{
public:
	CString STCD;  //��վ����
	CString TM;	   //ʱ��
	double DRP;	   //ʱ�ν�ˮ��
	double INTV;   //ʱ�γ�    1.00 /0.05
	double PRD;	   //��ˮ��ʱ
	double DYP;	   //�ս�ˮ��
	CString WTH;   //����״�� 7:���� 8:�޽���
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
	CString STCD ;	//��վ����  
	CString	TM; 	//ʱ��  	 
	double	STRMDR; //������ʱ  0.15
	double	STRMP;	//������  	
	CString	WTH;	//����״��  7:���� 8:�޽��� 
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

//��������
class CRainData_Item
{
private:
	int			m_nDataType;		//�������� 0:��ʱ���� 1:��ʼ���� 2:5���ӳ���ֵ 3:�������㡢��� 4:��ͣ
	double		m_dCurrentValue;	//ʵʱֵ(�������ۻ�ֵ)(0-999.9)
	double      m_dSumHalfHour;		//��Сʱ�ۼ�ֵ(0-999.9)
	double		m_dTotalValue;		//���ۼ�ֵ (��Χ:0��9999999.9)
	CString		m_sCollectTime;		//�ɼ�ʱ��string
public:
	SYSTEMTIME	m_tCollectTime;		//�ɼ�ʱ��
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

//ˮλ����
class CWaterL_DataItem
{
public:
	double		m_dValue;				//ˮλʵʱֵ
	SYSTEMTIME  m_RecordTime;			//�ɼ�ʱ��
public:
	CWaterL_DataItem();
	CWaterL_DataItem(double dValue, SYSTEMTIME& st);
};

//��վ����
enum CZ_Type_Enum
{
	CZ_Type_RAIN = 1,   //����վ
	CZ_Type_WL	 = 2,   //ˮλվ
};

//ˮλ��վ����
enum WL_Type_Enum
{
	WL_Type_HLSW = 1,		//����ˮλվ
	WL_Type_SKSSW = 2,		//ˮ����ˮλվ
	WL_Type_SKXSW = 3,		//ˮ����ˮλվ
};

const ULONG BEGIN_CALC_INDEX = 0;
const ULONG LAST_CALC_INDEX  = 5;

//��ַӳ�����
class CAddrMap_Item
{
private:
	CString m_RTU_ID;					//510101_00000000
	CZ_Type_Enum     m_Type;			//���� 1������վ  2:ˮλվ
	WL_Type_Enum     m_wlType;			//ˮλ���� 1���ӵ�ˮλ 2��ˮ����ˮλ 3��ˮ����ˮλ
	CString m_STCD;						//��վ����

	CString m_addVCode;					//���������� 6λ  �磺510101
	int     m_RtuAddr;					//ң���ն˵�ַ 0

	bool    m_blCalc;					//�Ƿ�ʱͳ�ƣ�Ĭ��: false
	bool    m_blCalcHalf;				//��Сʱͳ��    //����ͳ��ʱʹ��

	ULONG   m_nCalcIndex;				//ͳ�Ƽ��� 0-5
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

	//�����ǻ��ȡ��ʱͳ�Ƶı�־
	void    SetCalcState(bool _value) { m_blCalc  = _value;}
	const bool GetCalcState() {return m_blCalc;}

	void   SetCalcHalfState(bool _value) { m_blCalcHalf = _value;}
	const bool GetCalcHalfState()	{return m_blCalcHalf;}

	void  SetCalcIndex(ULONG nIdx)	{m_nCalcIndex = nIdx;}
	const ULONG GetCalcIndex()		{return m_nCalcIndex;}
};

//�Ĵ�ʡɽ���ֺ����ݽӿ�
class CDataTranSQLServer_SC :
	public CDataTran_Base,public CDataTranSQLServer_Base
{
public:
	CDataTranSQLServer_SC();
	~CDataTranSQLServer_SC();
public:
	virtual long ThreadFunction();

	void Statistics_Data_OnRealTime(COneData_INFO* pData);  //��������ʵʱͳ��
	void StatisticsRain_1Day_OnTimer();                     //��ʱͳ��1�����������

	void Initialization();
	virtual void SetDataSource(CString strSource);
	void SetSqlite(CDbSQLite* sqlite);
protected:	//SQL Server ����	
	bool OpenDatabase();
	bool OpenDatabase(CString dataSource);	
	bool GetConnectionState();
	bool CheckTableExist(CString sTableName); //������ݿ��б��Ƿ����, true:�����,false:������
	bool ExcuteNoQuery(CString sSQL);
private:
	bool       m_blStartCalc;				  //�Ƿ�ִ��ͳ��
	bool	   m_blCalc;
	CDbSQLite*  m_sqlite;	

	//�������ֵ��ȷ�Լ��
	double		m_dCHECK_RAIN_PER; //���5��15��1hour��ֵ
	double		m_dCHECK_RAIN_DAY; //���1day��ֵ 

	//����ͳ��ʱ����
	UINT	    m_nCheck5Minutes_MNT;
	UINT		m_nCheck15Minutes_MNT;
	UINT		m_nCheck1Hour_MNT;
	UINT		m_nChcek1Day_MNT;
private:	//���ݿ����,ͳ�ƴ��
	void CheckDatabase();					 //������ݴ���Ӧ�ı��Ƿ����

    //ʱ��< strend�����һ������ֵ,�ӱ���SQLite
	CRainData_Item*  GetEndTimeRainData(CAddrMap_Item* pAddrItem,CString strEnd);

	//ʱ��>= strStart ��һ��ֵ ���ӱ���SQLite
	CRainData_Item* GetStartTimeRainData(CAddrMap_Item* pAddrItem,CString strStart);

	//�ӱ���SQLite���ݿ� ��ȡ1��Сʱ����ˮλ����(ˮλ����  1 hours  0 < strpoint < end��)
	CWaterL_DataItem* GetLatestWaterLevelData(CAddrMap_Item* pAddrItem,CString strPoint);
private:	//sqlite���ݿ����
	bool OpenSqlite();
	CSqlStatement* ExcuteSQLiteQuery(CString strSQL);							//��ѯ

private:	//��ַӳ��(ң���ն˻�����վ��վ����ĵ�ַӳ��)
	bool InitAddressMap();														//��ʼ����ַӳ��
	void CheckAddressMap_ForRtu();												//����ַӳ������ȷ�� ��ң���ն˻���ӳ���ϵ
	void CheckAddressMap_ForMaster();											//����ַӳ������ȷ�� ����վST_STBPRP_B����������ݽ��м��
	//std::map<CString,CAddrMap_Item*>  m_mapAddressMap;						//��ַӳ��
	std::list<CAddrMap_Item*> m_mapAddressMap;									//��ַӳ�� �����ͬ����֦����STCD �޸� 20120803
	void ClearAddrMap();

private:	//ˮλ
	void CalcWaterLevel_RealTime(CString Rtu_ID,CWaterL_DataItem* pRecvData);                 //����ͳ��(ˮλ)
	void StatisticsWaterLevel_RealTime(CAddrMap_Item* pAddrItem,CWaterL_DataItem* pRecvData);
	bool UpdateWaterLevelData( CAddrMap_Item* pAddrItem, CString strTM, double pointValue);
	bool GetWaterLevelData(CString sSQL,double& WaterLevelData);
private:	//����
	void CalcRainData_RealTime(CString Rtu_ID,CRainData_Item* pRecvData);					   //����ͳ��(����)
	void StatisticsRain_RealTime(CAddrMap_Item* pAddrItem,CRainData_Item* pRecvData);	
	void StatisticsRain_5Minutes(CAddrMap_Item* pAddrItem,CRainData_Item* pRecvData);		   //5���������ۼ�ֵͳ��
	void StatisticsRain_15Minutes(CAddrMap_Item* pAddrItem,CRainData_Item* pRecvData);		   //15���������ۼ�ֵͳ��
	void StatisticsRain_1Hour(CAddrMap_Item* pAddrItem,CRainData_Item* pRecvData);			   //1��Сʱ�����ۼ�ֵͳ��
	void StatisticsRain_1Day(CAddrMap_Item* pAddrItem,CRainData_Item* pRecvData);

	double StatisticsRain_1Day(CAddrMap_Item* pAddrItem,CString strBegin,CString strEnd,CRainData_Item* pRecvData=NULL);      //1������ͳ��

	//������ۼ�ֵ������ִ�����>=500ʱ,�ŵ������⴦��(���ð�Сʱ�ۼ�ֵ���м���)
	double StatisticsRain_1Hour_special(CAddrMap_Item* pAddrItem,SYSTEMTIME sttime,CRainData_Item* pRecvData = NULL);
	//������ۼ�ֵ������ִ�����>=500ʱ,�ŵ������⴦��(���ð�Сʱ�ۼ�ֵ���м���)
	double StatisticsRain_1Day_special(CAddrMap_Item* pAddrItem,SYSTEMTIME sttime,CRainData_Item* pRecvData = NULL);
    
	//���½�������ST_PPTN_R�ļ�¼
	bool UpdateRainfallTableRecord(Table_ST_PPTN_R_Record& sRecord,RIANSTATISTICS_TYPE_Enum xType);
	
	//�ӽ�������ST_PPTN_R�Ĳ��Ҽ�¼(UPDATE or  INSERT)
	bool GetRainfallTableRecord(CString sSQL,Table_ST_PPTN_R_Record& findRecord);	

	//���±���ӱ���ST_STORM_R�ļ�¼(UPDATE or  INSERT)
	bool UpdateRainstormTableRecord(Table_ST_STORM_R_Record& sRecord);

	//��ȡ����ӱ���ST_STORM_R�ļ�¼
	bool GetRainstormRecord(CString sSQL,Table_ST_STORM_R_Record& findRecord);

	//���㽵����ʱ
	int  GetRainMinutes( CAddrMap_Item* pAddrItem, CString strBegin, CString strEnd );

	/*
	 ��������:����һ��ʱ����ۼƽ�����
	 ���ʱ��:2012-08-12
	 ����˵��:
	 pAddrItem  STCD
	 strBegin   ��ʼʱ�� >=
	 strEnd     ����ʱ�� <
	 xType      ͳ������(5����,15����,1hour,1day)
	 isNull     ����ֵ(true:��ʱ�����û��¼,false:��ʾ��ʱ������м�¼)
	*/
	double CalRainData_OnTimeRange(CAddrMap_Item* pAddrItem,
		CString strBegin,
		CString strEnd,
		RIANSTATISTICS_TYPE_Enum xType,
		bool& isNull);
}; 



