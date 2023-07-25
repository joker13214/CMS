#pragma once
#include "datatran_base.h"
#include "WaitTimer.h"
#include <map>
#include <list>
#include "SystemTimeOp.h"
#include "DataTranSQLite_Local.h"
#include <iterator>
  

const double RainFullScale_CONST = 9999999.9;  //�����ۼ�ֵ���̶�

//����ͳ������
enum RIANSTATISTICS_TYPE_Enum
{
	Rain_statistics_5minutes	= 0,  //5����ͳ��
	Rain_statistics_15minutes	= 1,  //15����ͳ��
	Rain_statistics_1hour		= 2,  //1Сʱͳ��
	Rain_statistics_AM8Point	= 3,  //����8��ͳ��
	Rain_statistics_UNKNOWN     = 255,//δ֪����
};

//ʱ���
//��ʼ����ʱ��
struct STRUCT_BEGINEND_AREA
{
	SYSTEMTIME stBegin;
	SYSTEMTIME stEnd;
	bool blSucc;						//�Ƿ����ɹ���false:ʧ�ܣ�true���ɹ�
public:
	STRUCT_BEGINEND_AREA()
	{
		CSystemTimeOp::ResetSystemTime(stBegin);
		CSystemTimeOp::ResetSystemTime(stEnd);
		blSucc = false;
	}
	const CString ToString();
};


//ˮ�������ݼ�¼
struct Table_ST_PPTN_R_Record
{
public:
	CString STCD;						//��վ����
	RIANSTATISTICS_TYPE_Enum TYPE;		//���� 0��5���ӣ�1:15����ͳ�ƣ�2:Сʱͳ�� 3����ͳ��
	CString TM;							//ʱ��
	SYSTEMTIME FTM;						//������ʱ�� XXXX-XX-XX XX:XX:XX.XXX��ʽ���ؼ����Ա�ʱ��
	CString WTH;						//����״�� 7:���� 8:�޽���
	double DRP;							//ʱ�ν�ˮ��
	double INTV;						//ʱ�γ�    1.00 /0.05
	double PRD;							//��ˮ��ʱ
	double DYP;							//�ս�ˮ��

	//added 20140813
	SYSTEMTIME CAL_TM; //ͳ��ʱ��
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

	//�������캯��
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


//����ST_RIVER_R�����ݼ�¼
struct Table_ST_RIVER_R_Record {
public:
	CString STCD;  //��վ����
	CString TM;    //ʱ��
	double Z;
	double Q;    //˲ʱ����
	double XSA;  
	double XSAVV;
	double XSMXV;
	CString FLWCHRCD;
	CString WPTN;
	CString MSQMT;
	CString MSAMT;
	CString MSVMT;

	//2023/5/18
	SYSTEMTIME CAL_TM;		//ͳ��ʱ��
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
//����ST_RSVR_Record�����ݼ�¼
struct Table_ST_RSVR_R_Record {
public:
	CString STCD; //��վ����
	CString TM;   //ʱ��
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
	SYSTEMTIME CAL_TM;		//ͳ��ʱ��
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
//����ӱ���¼
struct Table_ST_STORM_R_Record
{
public:
	CString STCD ;			//��վ����  
	CString	TM; 			//ʱ��  	 
	SYSTEMTIME FTM;			//������ʱ�� XXXX-XX-XX XX:XX:XX.XXX��ʽ,�ؼ����Ա�ʱ��
	double	STRMDR;			//������ʱ  0.15
	double	STRMP;			//������  	
	CString	WTH;			//����״��  7:���� 8:�޽��� 

	//added 20140813
	SYSTEMTIME CAL_TM;		//ͳ��ʱ��
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
	
	//�������캯��
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

//ˮλ����
struct Table_ST_WL_R_Record
{
public:
	CString STCD;			//��վ����
	CString TM;				//ʱ���
	SYSTEMTIME FTM;			//������ʱ�� XXXX-XX-XX XX:XX:XX.XXX��ʽ���ؼ����Ա�ʱ��
	int     TYPE;			//���ͣ�1 ����ˮλ��2 ˮ����ˮλ��3 ˮ����ˮλ
	double  PV;				//ˮλֵ
	SYSTEMTIME CAL_TM;		//ͳ��ʱ��
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

//���λ������
struct Table_ST_DISP_R_Record
{
public:
	CString STCD;			//��վ����
	CString TM;				//ʱ���
	SYSTEMTIME FTM;			//������ʱ�� XXXX-XX-XX XX:XX:XX.XXX��ʽ���ؼ����Ա�ʱ��
	double  X;				//ˮλֵ
	double  Y;				//ˮλֵ
	SYSTEMTIME CAL_TM;		//ͳ��ʱ��
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

//�¶�����
struct Table_ST_TEMP_R_Record
{
public:
	CString STCD;			//��վ����
	CString TM;				//ʱ���
	SYSTEMTIME FTM;			//������ʱ�� XXXX-XX-XX XX:XX:XX.XXX��ʽ���ؼ����Ա�ʱ��
	double  T;				//�¶�ֵ
	SYSTEMTIME CAL_TM;		//ͳ��ʱ��
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

//�������� �洢�ڱ���SQLite���ݿ�����ݣ�YDJ�ϴ������ݣ�
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
	CRainData_Item(CRainData_Item* pSource);

	const int DataType() {return m_nDataType;}
	const double CurrentValue(){return m_dCurrentValue;}
	const double TotalValue(){return  m_dTotalValue;}
	const double SumHalfHour(){return m_dSumHalfHour;}
	const CString CollectTimeString() {return m_sCollectTime;}
	const CString ToString();
public:
	bool Equals(CRainData_Item* pSrc); //�Ƚ��Ƿ���� 20140904
};

//ˮλ���� �洢�ڱ���SQLite���ݿ�����ݣ�YDJ�ϴ������ݣ�
class CWaterL_DataItem
{
public:
	double		m_dValue;				//ˮλʵʱֵ
	SYSTEMTIME  m_RecordTime;			//�ɼ�ʱ��
	CString     m_sRecordTime;
public:
	CWaterL_DataItem();
	CWaterL_DataItem(double dValue, SYSTEMTIME& st);
	CWaterL_DataItem(double dValue, SYSTEMTIME& st, CString strSt);
	CWaterL_DataItem(CWaterL_DataItem* pSource);
	CString ToString();
public:
	bool Equals(CWaterL_DataItem* pSrc);  //�Ƚ��Ƿ���� 20140904

};

//���λ������ �洢�ڱ���SQLite���ݿ�����ݣ�YDJ�ϴ������ݣ�
class CDisplace_DataItem
{
public:
	double		m_XValue;				//X��ʵʱֵ
	double		m_YValue;				//Y��ʵʱֵ
	//int			m_DisplaceNo;	   //���������
	SYSTEMTIME  m_RecordTime;			//�ɼ�ʱ��
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
	bool Equals(CDisplace_DataItem* pSrc);  //�Ƚ��Ƿ���� 20140904
};

//2023/5/23 �������� �洢�ڱ���SQLite�����ݿ������
class CFlow_DataItem
{
public:
	double Sunshiflowdata;     //˲ʱ��������
	double Leijiflowdata;      //�ۻ���������
	SYSTEMTIME m_RecordTime;   //�ɼ�ʱ��
	CString m_sRecordTime;
public:
	CFlow_DataItem();
	CFlow_DataItem(int data,double Data, SYSTEMTIME& st);
	CFlow_DataItem(double SunData, double LeiData, SYSTEMTIME& st);
	CFlow_DataItem(double SumData, double LeiData, SYSTEMTIME& st, CString strSt);
	CFlow_DataItem(CFlow_DataItem* pSource);
	CString ToString();
public:
	bool Equals(CFlow_DataItem* pSrc);  //�Ƚ��Ƿ���� 20140904
};

//�¶����� �洢�ڱ���SQLite���ݿ�����ݣ�YDJ�ϴ������ݣ�
class CTempera_DataItem
{
public:

	double		m_tValue;				//�¶�ʵʱֵ
	SYSTEMTIME  m_RecordTime;			//�ɼ�ʱ��
	CString     m_sRecordTime;
public:
	CTempera_DataItem();
	CTempera_DataItem(double tValue, SYSTEMTIME& st);
	CTempera_DataItem(double tValue, SYSTEMTIME& st, CString strSt);
	CTempera_DataItem(CTempera_DataItem* pSource);
	CString ToString();
public:
	bool Equals(CTempera_DataItem* pSrc);  //�Ƚ��Ƿ���� 20140904
};
//��վ����
enum CZ_Type_Enum
{
	CZ_Type_UNKNOWN = 0, //δ֪����
	CZ_Type_RAIN = 1,    //����վ
	CZ_Type_WL	 = 2,    //ˮλվ
	CZ_Type_DISP = 3,    //���λ�ƴ�����
	CZ_Type_TEMP = 4,    //�¶ȴ�����
	CZ_Type_FLOW = 5,    //����������
};

//ˮλ��վ����
enum WL_Type_Enum
{
	WL_Type_UNKNOWN = 0,    //δ֪����
	WL_Type_HLSW = 1,		//����ˮλվ
	WL_Type_SKSSW = 2,		//ˮ����ˮλվ
	WL_Type_SKXSW = 3,		//ˮ����ˮλվ
};

/*
* ��ʱͳ�ƶ���(�ȴ�ͳ�ƶ���)
* ����: 5���ӽ�����
*	    15���ӽ�����������ӱ���
*		1Сʱ������
*		1�ս�����
*		����ˮλͳ��[��]
*/
struct StatisticsOnTimer_Item
{
public: 
	CString STCD;						//��վ����
	CZ_Type_Enum Type;					//���� 1 ����վ��2 ˮλվ
	WL_Type_Enum WLType;				//���� 1 ����ˮλ��2 ˮλ��ˮλ��3 ˮ����ˮλ
	int RTUNo;							//���������
	CString strTM;						//���ʱ��㣨ˮλ��CollectTime��
	RIANSTATISTICS_TYPE_Enum Cal_Type;	//ͳ������
	CString strFTM;						//���ʱ��� ��ʽ��yyyy-mm-dd HH:MM:ss.s3
	CString strBegin;				    //ͳ��ʱ�䷶Χ��ʼʱ��
	CString strEnd;						 //ͳ��ʱ�䷶Χ����ʱ��
	SYSTEMTIME stFTM;					//���ʱ��� ��ʽ:yyyy-mm-dd HH:MM:ss.s3
	SYSTEMTIME stBegin;					//��ʼʱ��
	SYSTEMTIME stEnd;					//����ʱ��
	SYSTEMTIME CAL_TM;					//��¼ʱ��

	double     PV;						//ˮλ��ֵ  (ˮλֵ��������ͳ��)
	double     X;						//λ��X��ֵ  (ˮλֵ��������ͳ��)
	double     Y;						//λ��Y��ֵ  (ˮλֵ��������ͳ��)
	double     T;						//�¶ȵ�ֵ  (ˮλֵ��������ͳ��)

	double    Sunflowdata;              //˲ʱ������ֵ
	double    Leiflowdata;              //�ۻ�������ֵ

	SYSTEMTIME Collect;					//ˮλ�ɼ�ʱ��
public:
	StatisticsOnTimer_Item();
	StatisticsOnTimer_Item(StatisticsOnTimer_Item* pSource);  //�������캯��
	~StatisticsOnTimer_Item(){}								
	const CString ToString();
};

const ULONG BEGIN_CALC_INDEX = 0;
const ULONG LAST_CALC_INDEX  = 5;

//YDJ�ϴ������ڴ滺�棨ԭʼ�ϴ����ݣ�
//�������3����������ڴ滺��
//��ǰ���������λΪ���������ݡ�ˮλ����
//Added 20140901,for:�����Ż�
class YDJUpData_Cache
{
public:
	YDJUpData_Cache();
	~YDJUpData_Cache(){Clear();}
public:
	SYSTEMTIME m_stCacheAreaBegin;								//����Ŀ�ʼʱ��
	SYSTEMTIME m_stCacheAreaEnd;								//����Ľ���ʱ��
public:
	std::multimap<CString,CRainData_Item*> m_mapRainData;		//�����������ݻ���
	std::multimap<CString,CWaterL_DataItem*> m_mapWaterLData;	//ˮλ�������ݻ���
	std::multimap<CString,CDisplace_DataItem*> m_mapDisplaceData;	//λ���������ݻ���
	std::multimap<CString,CTempera_DataItem*> m_mapTemperaData;	//λ���������ݻ���
	
	std::multimap<CString, CFlow_DataItem*> m_mapFlowData;	//�����������ݻ���

public:
	SYSTEMTIME m_stRainBeginTime;								//�������ݿ�ʼʱ��
	SYSTEMTIME m_stRainEndTime;									//�������ݽ���ʱ��
	SYSTEMTIME m_stWaterLBeginTime;								//ˮλ���ݿ�ʼʱ��
	SYSTEMTIME m_stWaterLEndTime;								//ˮλ���ݽ���ʱ��
	SYSTEMTIME m_stDisplaceBeginTime;							//���λ�����ݿ�ʼʱ��
	SYSTEMTIME m_stDisplaceEndTime;								//���λ�����ݽ���ʱ��
	SYSTEMTIME m_stTemperaBeginTime;							//�¶����ݿ�ʼʱ��
	SYSTEMTIME m_stTemperaEndTime;								//�¶����ݽ���ʱ��

	//2023-5-23
	SYSTEMTIME m_stFlowBeginTime;							    //�������ݿ�ʼʱ��
	SYSTEMTIME m_stFlowEndTime;								    //�������ݽ���ʱ��
	
public:
	bool InsertData(CRainData_Item* pRain);						//���������ϴ�����
	bool InsertData(CWaterL_DataItem* pWaterL);					//����ˮλ�ϴ�����
	bool InsertData(CDisplace_DataItem* pDisplace);			    //����λ���ϴ�����
	bool InsertData(CTempera_DataItem* pTempera);				//�����¶��ϴ�����

	//2023-5-23
	bool InsertData(CFlow_DataItem* pFlowdata);                 //���������ϴ�����

	void SetUpDataCacheTimeArea(SYSTEMTIME AreaBegin,SYSTEMTIME AreaEnd);	 //���û���ʱ�䷶Χ
	void RemoveOutDataOnTimer();								//��ʱ�������ʱ�䷶Χ�ڵ�����
private:
	void RemoveRainData(CString strTime);
	void RemoveWaterLData(CString strTime);
	void RemoveDisplaceData(CString strTime);
	void RemoveTemperaData(CString strTime);

	//2023-5-23
	void RemoveFlowData(CString strTime);

private:
	void Clear();												//����ڴ�
};

//��ַӳ�����
class CAddrMap_Item
{
private:
	CString m_RTU_ID;					//510101_00000000
	CZ_Type_Enum     m_Type;			//���� 1������վ  2:ˮλվ
	WL_Type_Enum     m_wlType;			//ˮλ���� 1���ӵ�ˮλ 2��ˮ����ˮλ 3��ˮ����ˮλ
	int m_RTU_No;						//���������
	CString m_STCD;						//��վ����

	CString m_addVCode;					//���������� 6λ  �磺510101
	int     m_RtuAddr;					//ң���ն˵�ַ 0
	int     m_RtuNo;					//ң���ն˴�������� 1

	bool    m_blCalc;					//�Ƿ�ʱͳ�ƣ�Ĭ��: false
	bool    m_blCalcHalf;				//��Сʱͳ��    //����ͳ��ʱʹ��

	ULONG   m_nCalcIndex;				//ͳ�Ƽ��� 0-5
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
	void Clear();  //���map���ϵ�����
public:
	CString Get_RTU_ID()		{return m_RTU_ID;}
	CString Get_STCD()			{return m_STCD;}
	CString Get_ADDVCode()		{return m_addVCode;}
	int     Get_RtuAddr()		{return m_RtuAddr;}
	int     Get_RtuNo()			{return m_RtuNo;}
	CZ_Type_Enum GetType()		{return m_Type;}
	WL_Type_Enum GetWlType()	{return m_wlType;}

	//�����ǻ��ȡ��ʱͳ�Ƶı�־
	void    SetCalcState(bool _value) { m_blCalc  = _value;}
	const bool GetCalcState() {return m_blCalc;}

	void   SetCalcHalfState(bool _value) { m_blCalcHalf = _value;}
	const bool GetCalcHalfState()	{return m_blCalcHalf;}

	void  SetCalcIndex(ULONG nIdx)	{m_nCalcIndex = nIdx;}
	const ULONG GetCalcIndex()		{return m_nCalcIndex;}

	//added 20140813 sc.cache ʵʱͳ���ڴ滺��
public:
	std::map<CString,Table_ST_PPTN_R_Record*> m_mapST_PPTN_R;		//��ˮ��ͳ�ƻ����¼
	std::map<CString,Table_ST_STORM_R_Record*> m_mapST_STROM_R;		//����ӱ�ͳ�ƻ����¼
	std::map<CString,Table_ST_WL_R_Record*> m_mapST_WL_R;			//ˮλͳ�ƻ����¼
	std::map<CString,Table_ST_DISP_R_Record*> m_mapST_DISP_R;			//λ��ͳ�ƻ����¼
	std::map<CString,Table_ST_TEMP_R_Record*> m_mapST_TEMP_R;			//�¶�ͳ�ƻ����¼

	//2023-5-23
	std::map <CString, Table_ST_RIVER_R_Record*> m_mapST_RIVER_R;   //˲ʱ���������¼
	std::map <CString, Table_ST_RSVR_R_Record*> m_mapST_RSVR_R;      //�ۼ����������¼


	bool UpdateST_PPTN_R_Record(Table_ST_PPTN_R_Record* pUpdateR);	//���»����е���ˮ���¼��INSERT��UPDATE��
	bool RemoveST_PPTN_R_Record(CString strTM);						//�ӻ������Ƴ���ˮ���¼
	bool UpdateST_STROM_R_Record(Table_ST_STORM_R_Record* pUpdateR);//���½���ӱ��Ļ����¼��INSERT��UPDATE��
	bool RemoveST_STROM_R_Record(CString strTM);					//�ӻ������Ƴ�����ӱ���¼

	//2023-5-23
	bool UpdateST_RIVER_R_Record(Table_ST_RIVER_R_Record* pUpdateR);                //����RIVER������ļ�¼
	bool RemoveST_RIVER_R_Record(CString strTM);									//�ӻ������Ƴ�˲ʱ������¼
	bool UpdateST_RSVR_R_Record(Table_ST_RSVR_R_Record* pUpdateR);                  //����RSVR�����������
	bool RemoveST_RSVR_R_Record(CString strTM);                                     //�ӻ������Ƴ��ۻ�������¼

	//bool UpdateST_WL_R_Record(Table_ST_WL_R_Record* pUpdateR);    //����ˮλ����ļ�¼(INSERT,UPDATE)
	int UpdateST_WL_R_Record(Table_ST_WL_R_Record* pUpdateR);       //����ˮλ����ļ�¼(INSERT,UPDATE)
	bool RemoveST_WL_R_Record(CString strTM);						//�ӻ������Ƴ�ˮλ��¼	
	bool RemoveST_WL_R_Record(CString strBegin,CString strEnd);     //ɾ��һ��ʱ��ε�ˮλͳ������
	bool UpdateST_DISP_R_Record(Table_ST_DISP_R_Record* pUpdateR);	//���»����е�λ�Ƽ�¼��INSERT��UPDATE��
	bool RemoveST_DISP_R_Record(CString strTM);						//�ӻ������Ƴ�λ�Ƽ�¼
	bool UpdateST_TEMP_R_Record(Table_ST_TEMP_R_Record* pUpdateR);	//���»����е��¶ȼ�¼��INSERT��UPDATE��
	bool RemoveST_TEMP_R_Record(CString strTM);						//�ӻ������Ƴ��¶ȼ�¼

public: //���ܸĽ� 20140901
	std::map<CString,StatisticsOnTimer_Item*> m_mapStatistics_Rain;			//������ʱͳ�ƶ���
	std::map<CString,StatisticsOnTimer_Item*> m_mapStatistics_WaterL;		//ˮλ��ʱͳ�ƶ���
	std::map<CString,StatisticsOnTimer_Item*> m_mapStatistics_Displace;		//λ�ƶ�ʱͳ�ƶ���
	std::map<CString,StatisticsOnTimer_Item*> m_mapStatistics_Tempera;		//�¶ȶ�ʱͳ�ƶ���

	//2023-5-23
	std::map<CString, StatisticsOnTimer_Item*> m_mapStatistics_Flow;        //������ʱͳ�ƶ���

	bool UpdateStatisticsOnTimerItem(StatisticsOnTimer_Item* pUpdateR);		//���¶���(INSERT,UPDATE)
	bool RemoveStatisticsOnTimerItem(CZ_Type_Enum Type,CString strTM);		//��ͳ����ɺ��Ƴ��ö���
public:
	YDJUpData_Cache m_UpDataCache;											//YDJ�ϴ������ڴ滺��
	bool InsertData(CRainData_Item* pRain);
	bool InsertData(CWaterL_DataItem* pWaterL);	
	bool InsertData(CDisplace_DataItem* pDislace);
	bool InsertData(CTempera_DataItem* pTempera);
	bool InsertData(CFlow_DataItem* pFlowdata);

	void SetUpDataCacheTimeArea(SYSTEMTIME AreaBegin,SYSTEMTIME AreaEnd);	 //���û���ʱ�䷶Χ
};

//��ʱ������
//add 20140903
struct CheckOnTimer_Item
{
public: 
	DWORD m_nInterval;			//���ʱ����
	DWORD m_nPreTickCount;		//��һ�δ�����TickCount;
public:
	CheckOnTimer_Item()
	{
		m_nInterval = 0;
		m_nPreTickCount = 0;
	}
};

struct RainStatistics_Early{
public:
	DWORD  m_nScStatisticsCheck5mEarly;	    //5������ǰͳ��ʱ��,Ĭ����ǰ1����   1*60 ��λ���� 20140905
	DWORD  m_nScStatisticsCheck15mEarly;    //15������ǰͳ��ʱ�䣬Ĭ����ǰ2���� 1*60 ��λ���� 20140905
	DWORD  m_nScStatisticsCheck1hEarly;		//1h��ǰ��ǰͳ��ʱ�䣬Ĭ����ǰ3���� 1*60 ��λ���� 20140905
	DWORD  m_nScStatisticsCheck1dEarly;		//1d��ǰ��ǰͳ��ʱ�䣬Ĭ����ǰ3���� 1*60 ��λ���� 20140905
public:
	RainStatistics_Early()
	{
		m_nScStatisticsCheck15mEarly = 0;
		m_nScStatisticsCheck1dEarly = 0;
		m_nScStatisticsCheck1hEarly = 0;
		m_nScStatisticsCheck5mEarly = 0;
	}
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
	void SetLoclTransSQLite(CDataTranSQLite_Local* pLocal);
protected:
	//added 20140812
	CString m_strScCacheDBName;
	bool m_blOpenScCacheSucc;									//�Ƿ��Ѿ���SQLite�ļ�
	bool OpenScCacheSQLite();									//��ʵʱͳ�ƻ���SQLite���ݿ�
	void CheckScCacheSQLite();									//���ʵʱͳ�ƻ���SQLite���ݿ�ṹ
	void CheckScCacheTableExist_CurState();						//��ǰʵʱͳ�Ƶ�״̬
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
	bool IsExistScCacheTable( CString strTable );				//���SQLite���еı��Ƿ����
	bool ExcuteScCacheNoQuery(CString sSQL);					//��sc.cache.db��ִ�в���ѯ��
	CSqlStatement* ExcuteScCacheQuery(CString strSQL);			//��sc.cache.db��SQLite�в�ѯ����
	
	//ͳ�ƻ��涨ʱ���� 20140815
public:
	void SetLocalSQLiteTransStartFlag(bool blStartFlag);		//���ñ���SQLite����ı�־			
	void ScCacheInSQLServer_OnTimer();							//ͳ�ƻ��涨ʱ��ʡ���(��ʱ����)
	void ScCacheSaveToSQLite_OnTimer();							//ͳ�ƻ��涨ʱ��SQLite�ļ�(��ʱ����)
	void ScStatistics_OnTimer();								//��ʱͳ�ƣ���ʵʱͳ���޸�Ϊ��ʱͳ�ƣ�20140903
	void ScUpdataCacheCheck_OnTimer();							//��ʱ����ڴ滺�棬�����Ƿ�Χ�ڵ������Ƴ��ڴ滺�棬20140903
	void CurStatisticsState(int nCount,SYSTEMTIME& collTime,SYSTEMTIME& recvTime);//��⵱ǰ�Ƿ��м�ѹ����
private:
	int m_nWaittingBufferCount;									//��ǰδ����ı��Ļ�ѹ��
	SYSTEMTIME curDealCollectTime;							    //��ǰ����ı���ʱ��
	SYSTEMTIME curDealReciveTime;								//��ǰ�����ĵĽ���ʱ��
private:
	bool m_blLocalSQliteTransStartFlag;							//����SQLite�������Ƿ������ı�־,true�������������ܴӱ���SQLite��ȡ���ݣ� false:δ�������񣬿��Դӱ���SQLite��ȡ����
	DWORD m_dwInSQLServer_Interval;								//����ʡ����ʱ��������λ������
	DWORD m_dwSaveToSQLite_Interval;							//����SQLite�޵�ʱ��������λ������
	DWORD m_dwPreInSQLServerTC;									//�ϴδ�SQLServer��TickCount,���ڶ�ʱ�����
	DWORD m_dwPreSaveToSQLiteTC;								//�ϴδ�SQLite��TickCount,���ڶ�ʱ���

	CheckOnTimer_Item m_ScStatisticsTimer;						//��ʱͳ��
	CheckOnTimer_Item m_ScUpdataCacheTimer;						//��ʱ����ڴ滺��

	void ScCacheInSQLServer();									//ͳ�ƻ������ʡ��⹦�ܺ���
	DWORD m_dwChcekInSecond;									//ͳ�ƻ������ʡ���ʱ����
	void ScCacheSaveToSQLite();									//ͳ�ƻ������SQLite

	void ScStatisticsCheck();									//��ʱͳ�ƹ���
	void ScStatisticsCheck_Rain(CAddrMap_Item* pAddrItem);		//�ӻ����л�ȡ�ȴ�ͳ�Ƶ��������������ͳ�ƹ��ܣ�ͳ����ɺ��Ƴ��ȴ���
	void ScStatisticsCheck_WaterLevel(CAddrMap_Item* pAddrItem);//�ӻ����л�ȡ�ȴ�ͳ�Ƶ�ˮλ�����ˮλͳ�ƹ��ܣ�ͳ����ɺ��Ƴ��ȴ���
	void ScStatisticsCheck_Displace(CAddrMap_Item* pAddrItem);//�ӻ����л�ȡ�ȴ�ͳ�Ƶ�λ�������λ��ͳ�ƹ��ܣ�ͳ����ɺ��Ƴ��ȴ���
	void ScStatisticsCheck_Tempera(CAddrMap_Item* pAddrItem);//�ӻ����л�ȡ�ȴ�ͳ�Ƶ��¶�������¶�ͳ�ƹ��ܣ�ͳ����ɺ��Ƴ��ȴ���

	//2023-5-23
	void ScStatisticsCheck_Flow(CAddrMap_Item* pAddrItem);  //�ӻ����л�ȡ�ȴ�ͳ�Ƶ��¶�������¶�ͳ�ƹ��ܣ�ͳ����ɺ��Ƴ��ȴ���

	void ScUpdataCahceCheck();									//��ʱ����ڴ湦��

	void ReadScCacheSQLiteByStart();							//����ʱ��ȡsc.cache.db����
	void ReadScCacheSQLite_ST_PPTN_R();							//��ȡ��ST_PPTN_R

	//2023-5-23
	void ReadScCacheSQlite_FLOWV_DATA();                        //��ȡ��FLOWV_DATA

	void ReadScCacheSQLite_ST_STORM_R();						//��ȡ��ST_STORM_R
	void ReadScCacheSQLite_ST_WL_R();							//��ȡ��ST_WL_R
	void ReadScCacheSQLite_ST_DISP_R();							//��ȡ��ST_DISP_R
	void ReadScCacheSQLite_ST_TEMP_R();							//��ȡ��ST_TEMP_R
	void ReadScCacheSQlite_STCObjects();						//��ȡ��ST_OBJECTS

	void ReadLocalSQLiteTOUpDataCache();									//������SQLite�����ݶ�ȡ���ϴ�������
	void CalUpDataCacheTimeArea(SYSTEMTIME& AreaBegin, SYSTEMTIME& AreaEnd); //���㱾���ڴ滺���ʱ�䷶Χ 3��ʱ��

	//add 20140816 ʱ��μ���
private:
	STRUCT_BEGINEND_AREA Get5MinuteArea(SYSTEMTIME* pST = NULL);       //��ȡpST���ڵ�5���ӿ�ʼ����ʱ������,��pST = NULL,���㵱ǰʱ��
	STRUCT_BEGINEND_AREA Get15MinuteArea(SYSTEMTIME* pST= NULL);	   //��ȡpST���ڵ�15���ӿ�ʼ����ʱ������,��pST = NULL,���㵱ǰʱ��
	STRUCT_BEGINEND_AREA Get1HourArea(SYSTEMTIME* pST= NULL);		   //��ȡpST���ڵ�Сʱ�Ŀ�ʼ����ʱ������,��pST = NULL,���㵱ǰʱ��
	STRUCT_BEGINEND_AREA Get1DayArea(SYSTEMTIME* pST = NULL);		   //��ȡpST���ڵ�1��Ŀ�ʼ����ʱ������,��pST = NULL,���㵱ǰʱ��
protected:	//SQL Server ����	
	bool OpenDatabase();
	bool OpenDatabase(CString dataSource);	
	bool GetConnectionState();
	bool CheckTableExist(CString sTableName); //������ݿ��б��Ƿ����, true:�����,false:������
	bool ExcuteNoQuery(CString sSQL);
private:
	bool       m_blStartCalc;				  //�Ƿ�ִ��ͳ��
	bool	   m_blCalc;
	CDbSQLite*  m_local_sqlite;				  //���ش洢ң���ն˻�ԭʼ����SQLite�����ļ�,��DataTranSQLite_Localģ���д�

	CDataTranSQLite_Local* m_localTrans;

	//���ش洢ʵʱͳ�ƵĻ���SQLite�����ļ�
	CDbSQLite m_scChace_sqlite;				

	//�������ֵ��ȷ�Լ��
	double		m_dCHECK_RAIN_PER; //���5��15��1hour��ֵ
	double		m_dCHECK_RAIN_DAY; //���1day��ֵ 

	//����ͳ��ʱ����
	UINT	    m_nCheck5Minutes_MNT;
	UINT		m_nCheck15Minutes_MNT;
	UINT		m_nCheck1Hour_MNT;
	UINT		m_nChcek1Day_MNT;

	//ˮλ�����ϴ���ʽ
	BYTE        m_nWaterLevaeIType;

	//�Ƿ���Ҫͳ�ƽ�����ʱ��0����ͳ�ơ�1��ͳ�ƣ�
	BYTE        m_RainPRDFlag; 

	BYTE m_nDatabaseType;					  //���ݿ����� 1:SQL Server 2: Oracle (Added 20130509)
private:	//���ݿ����,ͳ�ƴ��
	void CheckDatabase();					 //������ݴ���Ӧ�ı��Ƿ����

	//ʱ��>= strStart ��һ��ֵ ���ӱ���SQLite
	CRainData_Item* GetStartTimeRainData(CAddrMap_Item* pAddrItem,CString strStart);

private:	//sqlite���ݿ����
	//bool OpenSqlite();
	CSqlStatement* ExcuteSQLiteQuery(CString strSQL);							//��ѯ

private:	//��ַӳ��(ң���ն˻�����վ��վ����ĵ�ַӳ��)
	bool InitAddressMap();														//��ʼ����ַӳ��
	void CheckAddressMap_ForRtu();												//����ַӳ������ȷ�� ��ң���ն˻���ӳ���ϵ
	void CheckAddressMap_ForMaster();											//����ַӳ������ȷ�� ����վST_STBPRP_B����������ݽ��м��
	//std::map<CString,CAddrMap_Item*>  m_mapAddressMap;						//��ַӳ��
	std::list<CAddrMap_Item*> m_mapAddressMap;									//��ַӳ�� �����ͬ����֦����STCD �޸� 20120803
	void ClearAddrMap();
private:	//ˮλ
	void CalcWaterLevel_RealTime(CString Rtu_ID,CWaterL_DataItem* pRecvData);								//����ͳ��(ˮλ)
private:   //ˮλͳ�� old
	void StatisticsWaterLevel_RealTime_Default(CAddrMap_Item* pAddrItem,CWaterL_DataItem* pRecvData);		//�Բ���ң���ն˻�ʵʱ���͵�ˮλ�������
	void StatisticsWaterLevel_RealTime_SBK(CAddrMap_Item* pAddrItem,CWaterL_DataItem* pRecvData);			//����ͳ�������ˮλ�ͼӱ������
	void StatisticsWaterLevel_Hour_AppendReport( CAddrMap_Item* pAddrItem, SYSTEMTIME begintime, SYSTEMTIME endtime ); //1Сʱˮλ�ӱ�ͳ��
	//�ӱ���SQLite���ݿ� ��ȡ1��Сʱ����ˮλ����(ˮλ����  1 hours  0 < strpoint < end��)
	CWaterL_DataItem* GetLatestWaterLevelData(CAddrMap_Item* pAddrItem,CString strPoint);

private: //����ˮλʡ���
	int UpdateWaterLevelData( CAddrMap_Item* pAddrItem, CString strTM, double pointValue);
	int UpdateFlowData1(CAddrMap_Item* pAddrItem,double value, CString strTM);                             //˲ʱ����
	int UpdateFlowData2(CAddrMap_Item* pAddrItem, double value, CString strTM);                            //�ۼ�����
	bool GetWaterLevelData(CString sSQL,double& WaterLevelData);
	CWaterL_DataItem* GetWaterLevelBasePoint( CAddrMap_Item* pAddrItem, CWaterL_DataItem* pRecvData );

private:	//����
	void CalcRainData_RealTime(CString Rtu_ID,CRainData_Item* pRecvData);					   //����ͳ��(����)
private:    //����ͳ�� OLD
	void StatisticsRain_RealTime(CAddrMap_Item* pAddrItem,CRainData_Item* pRecvData);	
	void StatisticsRain_5Minutes(CAddrMap_Item* pAddrItem,CRainData_Item* pRecvData);		   //5���������ۼ�ֵͳ��
	void StatisticsRain_15Minutes(CAddrMap_Item* pAddrItem,CRainData_Item* pRecvData);		   //15���������ۼ�ֵͳ��
	void StatisticsRain_1Hour(CAddrMap_Item* pAddrItem,CRainData_Item* pRecvData);			   //1��Сʱ�����ۼ�ֵͳ��
	void StatisticsRain_1Day(CAddrMap_Item* pAddrItem,CRainData_Item* pRecvData);
	//double StatisticsRain_1Day(CAddrMap_Item* pAddrItem,CString strBegin,CString strEnd,CRainData_Item* pRecvData=NULL);      //1������ͳ��
	//������ۼ�ֵ������ִ�����>=500ʱ,�ŵ������⴦��(���ð�Сʱ�ۼ�ֵ���м���)
	double StatisticsRain_1Hour_special(CAddrMap_Item* pAddrItem,SYSTEMTIME sttime,CRainData_Item* pRecvData = NULL);
	//������ۼ�ֵ������ִ�����>=500ʱ,�ŵ������⴦��(���ð�Сʱ�ۼ�ֵ���м���)
	double StatisticsRain_1Day_special(CAddrMap_Item* pAddrItem,SYSTEMTIME sttime,CRainData_Item* pRecvData = NULL);
	//���㽵����ʱ
	int  GetRainMinutes( CAddrMap_Item* pAddrItem, CString strBegin, CString strEnd );
	//ʱ��< strend�����һ������ֵ,�ӱ���SQLite
	CRainData_Item*  GetEndTimeRainData(CAddrMap_Item* pAddrItem,CString strEnd);
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

private: //����ʡ�����������
	//���½�������ST_PPTN_R�ļ�¼
	bool UpdateRainfallTableRecord(Table_ST_PPTN_R_Record& sRecord,RIANSTATISTICS_TYPE_Enum xType);

	//�ӽ�������ST_PPTN_R�Ĳ��Ҽ�¼(UPDATE or  INSERT)
	bool GetRainfallTableRecord(CString sSQL,Table_ST_PPTN_R_Record& findRecord);	

	//���±���ӱ���ST_STORM_R�ļ�¼(UPDATE or  INSERT)
	bool UpdateRainstormTableRecord(Table_ST_STORM_R_Record& sRecord);

	//��ȡ����ӱ���ST_STORM_R�ļ�¼
	bool GetRainstormRecord(CString sSQL,Table_ST_STORM_R_Record& findRecord);

private: 
		//�Ż�1(����ͳ��)
		//�Ż�����:5���ӡ�15���ӡ�1Сʱ��1�콵������ͳ��ԭʼ�����Ż�Ϊֻ�ӱ���SQLite��ȡһ��
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
	    //�Ż�1��ˮλͳ�ƣ�
	    //�Ż����ݣ�Ԥ�Ƚ�ͳ�������ˮλԭʼ���ݴӱ���SQLite����1���Զ�ȡ
		void StatisticsWaterLevel_Optimize1_RealTime_SBK(CAddrMap_Item* pAddrItem,CWaterL_DataItem* pRecvData);			//����ͳ�������ˮλ�ͼӱ������
		void StatisticsWaterLevel_Optimize1_Hour_AppendReport( CAddrMap_Item* pAddrItem, SYSTEMTIME begintime, SYSTEMTIME endtime,std::multimap<CString,CWaterL_DataItem*>& WLDataMap);
		std::multimap<CString,CWaterL_DataItem*> StatisticsWaterLevel_Optimize1_GetWaterLData( CAddrMap_Item* pAddrItem, CWaterL_DataItem* pRecvData );
		CWaterL_DataItem* GetLatestWaterLevelData_Optimize1(CAddrMap_Item* pAddrItem,CString strPoint,std::multimap<CString,CWaterL_DataItem*>& WLDataMap);

private: 
	//�Ż�2������ͳ�ƣ�
	RainStatistics_Early m_RainStcEarly;
	void StatisticsRain_Optimize2_Enter(CAddrMap_Item* pAddrItem,CRainData_Item* pRecvData); //YDJ���͵������������
	void StatisticsRain_Optimize2_5MinutesTimeArea(CRainData_Item* pRecvData,SYSTEMTIME& areaBegin,SYSTEMTIME& areaEnd, CString& strTM);		//5���ӽ�����ʱ�䷶Χ
	void StatisticsRain_Optimize2_15MinutesTimeArea(CRainData_Item* pRecvData,SYSTEMTIME& areaBegin,SYSTEMTIME& areaEnd,CString& strTM);		//15���ӽ�����ʱ�䷶Χ
	void StatisticsRain_Optimize2_1HourTimeArea(CRainData_Item* pRecvData,SYSTEMTIME& areaBegin,SYSTEMTIME& areaEnd,CString& strTM);			//1Сʱ������ʱ�䷶Χ
	void StatisticsRain_Optimize2_1DayTimeArea(CRainData_Item* pRecvData,SYSTEMTIME& areaBegin,SYSTEMTIME& areaEnd,CString& strTM);				 //1�콵����ʱ�䷶Χ
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
	void StatisticsRain_Optimize2_1Day_OnTimer();			//ÿ������8������ս�����ͳ��

private://�Ż�2 ��ˮλͳ�ƣ� 20140903
	void StatisticsWaterLevel_Optimize2_Enter(CAddrMap_Item* pAddrItem,CWaterL_DataItem* pRecvData);																	//ˮλ�ϴ�������ں�������Ҫ�Ǽ������Ҫ��ȡˮλ��ֵ
    std::multimap<CString,CWaterL_DataItem*> StatisticsWaterLevel_Optimize2_GetWaterLDataFromSQLite( CAddrMap_Item* pAddrItem,CString strBegin,CString strEnd);			//�ӱ���SQLite��ȡˮλ��������
	std::multimap<CString,CWaterL_DataItem*> StatisticsWaterLevel_Optimize2_GetWaterLDataFromUpDataCache( CAddrMap_Item* pAddrItem,CString strBegin,CString strEnd);	//���ڴ滺���ж�ȡˮλ��������
	void StatisticsWaterLevel_Optimize2_RealTime_SBK(CAddrMap_Item* pAddrItem,CWaterL_DataItem* pRecvData,std::multimap<CString,CWaterL_DataItem*>& WlDataMap);			//����ͳ�������ˮλ�ͼӱ������

private://���λ����ʡ���
	void CalcDisplace_RealTime(CString Rtu_ID,int Rtu_No,CDisplace_DataItem* pRecvData);					   //����ͳ��(λ��)
	void StatisticsDisplace_Optimize2_Enter(CAddrMap_Item* pAddrItem,CDisplace_DataItem* pRecvData);														//�ϴ����λ��������ں�������Ҫ�Ǽ������Ҫ��ȡ��ֵ
    std::multimap<CString,CDisplace_DataItem*> StatisticsDisplace_Optimize2_GetDisplaceDataFromSQLite( CAddrMap_Item* pAddrItem,CString strBegin,CString strEnd);		//�ӱ���SQLite��ȡ���λ�ƻ�������
	std::multimap<CString,CDisplace_DataItem*> StatisticsDisplace_Optimize2_GetDisplaceDataFromUpDataCache( CAddrMap_Item* pAddrItem,CString strBegin,CString strEnd);	//���ڴ滺���ж�ȡ���λ�ƻ�������
	void StatisticsDisplace_Optimize2_RealTime(CAddrMap_Item* pAddrItem,CDisplace_DataItem* pRecvData,std::multimap<CString,CDisplace_DataItem*>& DispDataMap);	//����ͳ����������λ�ƺͼӱ������
	int UpdateDisplaceData( CAddrMap_Item* pAddrItem, CString strTM, double xValue, double yValue);
	bool GetDisplaceData(CString sSQL,double& xData,double& yData);
	CDisplace_DataItem* GetDisplaceBasePoint( CAddrMap_Item* pAddrItem, CDisplace_DataItem* pRecvData );

private://�¶���ʡ���
	void CalcTempera_RealTime(CString Rtu_ID,CTempera_DataItem* pRecvData);					   //����ͳ��(λ��)
	void StatisticsTempera_Optimize2_Enter(CAddrMap_Item* pAddrItem,CTempera_DataItem* pRecvData);														//�ϴ��¶�������ں�������Ҫ�Ǽ������Ҫ��ȡ��ֵ
    std::multimap<CString,CTempera_DataItem*> StatisticsTempera_Optimize2_GetTemperaDataFromSQLite( CAddrMap_Item* pAddrItem,CString strBegin,CString strEnd);		//�ӱ���SQLite��ȡ�¶Ȼ�������
	std::multimap<CString,CTempera_DataItem*> StatisticsTempera_Optimize2_GetTemperaDataFromUpDataCache( CAddrMap_Item* pAddrItem,CString strBegin,CString strEnd);	//���ڴ滺���ж�ȡ�¶Ȼ�������
	void StatisticsTempera_Optimize2_RealTime(CAddrMap_Item* pAddrItem,CTempera_DataItem* pRecvData,std::multimap<CString,CTempera_DataItem*>& TempDataMap);	//����ͳ��������¶Ⱥͼӱ������
	int UpdateTemperaData( CAddrMap_Item* pAddrItem, CString strTM, double tValue);
	bool GetTemperaData(CString sSQL,double& tData);
	CTempera_DataItem* GetTemperaBasePoint( CAddrMap_Item* pAddrItem, CTempera_DataItem* pRecvData );

//2023-5-23
private://������ʡ���
	void CFlow_RealTime(CString Rtu_ID, CFlow_DataItem* pRecvData);					   //����ͳ��(����)
	void StatisticsFlow_Optimize2_Enter(CAddrMap_Item* pAddrItem, CFlow_DataItem* pRecvData);
	std::multimap<CString, CFlow_DataItem*> StatisticsFlow_Optimize2_GetFlowDataFromSQLite(CAddrMap_Item* pAddrItem, CString strBegin, CString strEnd);		//�ӱ���SQLite��ȡ������������
	std::multimap<CString, CFlow_DataItem*> StatisticsFlow_Optimize2_GetFlowDataFromUpDataCache(CAddrMap_Item* pAddrItem, CString strBegin, CString strEnd);	//���ڴ滺���ж�ȡ��������
	void StatisticsFlow_Optimize2_RealTime(CAddrMap_Item* pAddrItem, CFlow_DataItem* pRecvData, std::multimap<CString, CFlow_DataItem*>& FlowDataMap);	//����ͳ������������ͼӱ������
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



