#pragma once

#include "RtuYcObject.h"
#include "RtuYkObject.h"
#include "RtuYxObject.h"
#include "RtuZdObject.h"
#include <afxcoll.h>
#include "UdfBase.h"
#include "UdfGlobalFUNC.h"
#include <afxdb.h>
#include "SysDataTrans.h"
#include "SysDataTransHydr.h"
#include "SysDataTransWriteUpdate.h"
#include "SysDataTransSyq.h"
#include "RtuPropertyDlg.h"
#include <vector>
#include <map>

#include "Handle_COM.h"
#include "Handle_NET.h"
#include "Handle_TcpClient.h"

//ң���ն��Ա�������Ϣ
class CRTU_AUTOREPORT_PRA
{
public:
	CRTU_AUTOREPORT_PRA()
	{
		m_wAutoTotal = 0;
		m_wAutoInterval = 0;
	}
	~CRTU_AUTOREPORT_PRA(){}
	bool		GetAutoBitValue(size_t _pos);										//��ȡbitλֵ��_pos(0~n bit),true:��ʾ��bitλΪ1,false:��ʾ��bitλΪ0
	const WORD	SetAutoBitValue(size_t _pos,bool _val = true);
	void		SetAutoValue(WORD TotalValue)		{m_wAutoTotal = TotalValue;}    //�����Զ��ϱ�����
	const WORD	GetAutoValue()						{return m_wAutoTotal;}			//��ȡ�Զ��ϱ�����
	void		SetAutoInterval(WORD interval)		{m_wAutoInterval = interval;}	//�����Զ��ϱ�ʱ��������λmin
	const WORD  GetAutoInterval()					{return m_wAutoInterval;}		//��ȡ�Զ��ϱ�ʱ��������λmin
protected:
	WORD m_wAutoTotal;		//�Ա�����
	WORD m_wAutoInterval;	//�Ա�ʱ��������λ��min(����)
};

//ң��վ���ѯ��ʵʱ�����������
class CRTU_YCQUERYTYPE_PARA
{
public:
	CRTU_YCQUERYTYPE_PARA()
	{
		m_wYcQueryType = 0;
	}
	~CRTU_YCQUERYTYPE_PARA(){}
	bool		GetQueryBitValue(size_t _pos);
	const WORD	SetQueryBitValue(size_t _pos,bool _val = true);
	const WORD  GetQueryPara()				{return m_wYcQueryType;     }
	void		SetQueryPara(WORD queryType){m_wYcQueryType = queryType;}
protected:
	WORD m_wYcQueryType;
};

//���ң���ն˵���ʷ���ݵ�Ԫ����
class CClearHistoryData_PARA
{
public:
	CClearHistoryData_PARA()
	{
		m_bClearHis_BYTE = 0;
	}
	const BYTE GetClearHistoryPARA() {return m_bClearHis_BYTE;}
	void       SetClearHistoryPARA(BYTE _value) {m_bClearHis_BYTE = _value;}
	bool       GetBitValue(size_t _pos);                        //��ȡĳһλ��ֵ����0-7
    void       SetBitValue(size_t _pos,bool _val = true);
protected:
	BYTE  m_bClearHis_BYTE;  //�Ƿ���յı�־�ֽڣ�ÿһλ�Ƿ�Ϊ1��ȷ���Ƿ����ĳһ��
};

//ң���ն�״̬  1Bytes
class CRTUTerminal_State     
{
public:
	CRTUTerminal_State(){m_TerminalStateByte = 0;}
	void SetTerminalStateByte(BYTE _stateByte) {m_TerminalStateByte = _stateByte;}
	const BYTE GetTerminalStateByte() {return m_TerminalStateByte;}
	bool GetBitValue(size_t _pos);
	void SetBitValue(size_t _pos,bool _val = true);
	const CString GetDescription();
protected:
	BYTE m_TerminalStateByte;        //ң���ն�״̬Byte, 1���ֽڣ�HEX
};

//ң���ն˵ı���״̬ 1Byte
class CRTUAlarm_State
{
public:
	CRTUAlarm_State() {m_AlarmStateByte = 0;}
	void SetAlarmStateByte(BYTE _alarmState) {m_AlarmStateByte = _alarmState;}
	const BYTE GetAlarmStateByte(){return m_AlarmStateByte;}
	bool GetBitValue(size_t _pos);
	void SetBitValue(size_t _pos,bool _val = true);
	const CString GetDescription();
protected:
	BYTE m_AlarmStateByte;           //ң���ն˱���״̬Byte, 1���ֽڣ�HEX
};

//ң���ն���չ����״̬ 1Byte
class CRTU_EXAlarm_State
{
public:
	CRTU_EXAlarm_State() {m_ExAlarmStateByte = 0;}
	void SetExAlarmStateByte(BYTE _ExAlarmByte) {m_ExAlarmStateByte = _ExAlarmByte;}
	const BYTE GetExAlarmStateByte() {return m_ExAlarmStateByte;}
	bool GetBitValue(size_t _pos);
	void SetBitValue(size_t _pos,bool _val = true);
	const CString GetDescription();
protected:
	BYTE m_ExAlarmStateByte;       //ң���ն���չ����״̬Byteֵ��1���ֽڣ�HEX
};

//ˮ�õ����ʵʱ��������
class CWaterPump_WorkData
{
public:
	CWaterPump_WorkData()
	{
		m_VoltageA = m_VoltageB = m_VoltageC = 0;
		m_ECurrentA = m_ECurrentB = m_ECurrentC = 0;
	}
	void SetVoltageA(WORD _VoltageA){m_VoltageA = _VoltageA;}
	void SetVoltageB(WORD _VoltageB){m_VoltageB = _VoltageB;}
	void SetVoltageC(WORD _VoltageC){m_VoltageC = _VoltageC;}
	void SetECurrentA(WORD _ECurrentA){m_ECurrentA = _ECurrentA;}
	void SetECurrentB(WORD _ECurrentB){m_ECurrentB = _ECurrentB;}
	void SetECurrentC(WORD _ECurrentC){m_ECurrentC = _ECurrentC;}

	const WORD GetVoltageA() {return m_VoltageA;}
	const WORD GetVoltageB() {return m_VoltageB;}
	const WORD GetVoltageC() {return m_VoltageC;}
	const WORD GetECurrentA() {return m_ECurrentA;}
	const WORD GetECurrentB() {return m_ECurrentB;}
	const WORD GetECurrentC() {return m_ECurrentC;}
protected:
	WORD m_VoltageA;						//A���ѹ
	WORD m_VoltageB;						//B���ѹ
	WORD m_VoltageC;						//C���ѹ

	WORD m_ECurrentA;						//A�����
	WORD m_ECurrentB;						//B�����
	WORD m_ECurrentC;						//C�����
};

//ң������/�رն�Ӧˮ�û�բ��/���ŵ�������Ϣ
class CPumpGate_YK_PARA
{
public:
	CPumpGate_YK_PARA();
	void SetYKByte(BYTE _ykByte);
	void SetPumpGate_Code(BYTE _Code);
	void SetPumpGate_Type(BYTE _Type);
	const BYTE GetYKByte() {return m_YK_Byte;}
	const BYTE GetPumpGate_Code() {return m_PumpGate_Code;}
	const BYTE GetPumpGate_Type() {return m_PumpGete_Type;}
protected:
	BYTE m_PumpGate_Code;                  //ˮ��/բ�ű���� D0-D3 0-15
	BYTE m_PumpGete_Type;                  //���� D4-D7   0000��0�� ˮ�ã� 1111��15�� ����/բ��

	BYTE m_YK_Byte;                        //ң���ֽ� ǰ������
};

//ң��վ�������ݱ���ֵ������
//����������
class CRainAlert_PARA_PER 
{
public:
	CRainAlert_PARA_PER()
	{
		m_fRainAlertValue = 0.0;
		m_wPerTime = 0;
		m_RealTimeValue = 0.0;
		m_DataType = 0;
		m_TotalValue = 0.0;
		m_HalfhValue = 0.0;
	}
	CRainAlert_PARA_PER(CRainAlert_PARA_PER* pSource);
protected:
	double m_fRainAlertValue;     //��λʱ���ڵ���������ֵ  2���ֽڣ�BCD�� 0-999.9
	WORD  m_wPerTime;            //��λʱ�䣬0-9999��2���ֽڣ�BCD��
	double m_RealTimeValue;       //����ʵʱֵ0-999.9  ��λ��mm/h  
	BYTE  m_DataType;			 //�Ա�ʱ��������,�Ա�ʱ���� //ˮ�������� 0��5�������� 1:1Сʱ������ 24:�ս�����  25��������  12��1Сʱÿ5���ӵ�ֵ
	double m_TotalValue;			 //������ֵ 0-9999999.9 ��λ:mm
	double m_HalfhValue;			 //��Сʱ�����ۼ�ֵ 0-999.9 ��λ:mm
public:
	void SetRainAlertValue(double _rainAlertVlue){m_fRainAlertValue = _rainAlertVlue;}
	void SetPerTime(WORD _perTimeValue)			 {m_wPerTime = _perTimeValue;}
	void SetRealTimeValue(double _realValue)	 {m_RealTimeValue = _realValue;}
	void SetDataType(BYTE _Type)				 {m_DataType = _Type;}
	void SetTotalValue(double _tValue)			 {m_TotalValue = _tValue;}
	void SetHalfHourValue(double _hValue)		 {m_HalfhValue = _hValue;}
	const double GetRainAlertValue()				 {return m_fRainAlertValue;}
	const WORD  GetPerTime()					 {return m_wPerTime;}
	const double GetRealTimeValue()				 {return m_RealTimeValue;}
	const BYTE  GetDataType()					 {return m_DataType;}
	const double  GetTotalValue()				 {return m_TotalValue;}
	const double GetHalfHourValue()				 {return m_HalfhValue;}
};

//ң��վģ�����������²���������ģ����PER
class CAnalogLimit_PARA_PER
{
public:
	CAnalogLimit_PARA_PER()
	{
		m_Analog_DownLimit = 0;
		m_Analog_UpLimit =  0;
		m_RealTimeValue = 0;
	}
	CAnalogLimit_PARA_PER(CAnalogLimit_PARA_PER* pSource);
public:
	void SetAnalogUpLimit(DWORD _upLimit)	{m_Analog_UpLimit = _upLimit;}			//����ģ��������ֵ
	void SetAnalogDownLimit(DWORD _downLimit){m_Analog_DownLimit = _downLimit;}		//����ģ��������ֵ
	void SetRealTimeValue(WORD _realValue)	 {m_RealTimeValue = _realValue;}
	const DWORD GetAnalogUpLimit() {return m_Analog_UpLimit;}						//��ȡģ��������ֵ
	const DWORD GetAnalogDownLimit(){return m_Analog_DownLimit;}					//��ȡģ��������ֵ
	const DWORD GetRealTimeValue()	{return m_RealTimeValue;}
protected:
	DWORD m_Analog_UpLimit;		//ģ�������� 3���ֽڣ�BCD,0-99999
	DWORD m_Analog_DownLimit;	//ģ�������� 3���ֽڣ�BCD,0-99999
	DWORD m_RealTimeValue;      //ģ����ʵʱ��ѯֵ  0-99999
};


//ң���ն�ˮ����ز�����Ϣ 
//����:��ֵ����ʣ��ˮ��,ʣ��ˮ������ֵ
class CWaterValume_PARA
{
public:
	CWaterValume_PARA(){
		m_lReChargeValume =0;
		m_lRemainAlertValume =0;
		m_lRemainValume =0;
		m_bSignBit = 0;

	}
public:
	void SetRechargeValume(DWORD _rechargeV)				{m_lReChargeValume = _rechargeV;}
	void SetRemainValume(UINT64 _remainV)					{m_lRemainValume = _remainV;}
	void SetRemainAlertValue(DWORD _remainAlertV)			{m_lRemainAlertValume = _remainAlertV;}
	void SetSignBit(BYTE _signBit)							{m_bSignBit = _signBit;}
	const DWORD GetRechargeValume()							{return m_lReChargeValume;}
	const UINT64 GetRemainValume()							{return m_lRemainValume;}
	const DWORD GetRemainAlertValume()						{return m_lRemainAlertValume;}
	const BYTE  GetSignBit()								{return m_bSignBit;}

protected:
	DWORD  m_lReChargeValume;		 //��ֵ����0-99999999 4Bytes BCD      ��λ��������
	UINT64 m_lRemainValume;          //ʣ��ˮ��,0-7999999999 5bytes��BCD�����λΪ����λ��0Ϊ�� 1Ϊ��ֵ ��λ:������
	DWORD  m_lRemainAlertValume;	 //ʣ��ˮ������ֵ
	BYTE   m_bSignBit;               //����λ 1:Ϊ��ֵ��0Ϊ��ֵ
};

//ˮλ����������ˮλ��Ĳ���
//����:ˮλ��ֵ��������ֵ
class CWaterLevel_PAR_PER
{
public:
	CWaterLevel_PAR_PER()
	{
		m_fBaseValue = 0.0f;
		m_fUpValue = 0.0f;
		m_fDownValue = 0.0f;
		m_fRealValue = 0.0f;
	}
	CWaterLevel_PAR_PER(CWaterLevel_PAR_PER* pSoruce);
public:
	void SetBaseValue(float _baseValue)						{m_fBaseValue = _baseValue;}
	void SetUpValue(float _upValue)							{m_fUpValue = _upValue;}
	void SetDownValue(float _downValue)						{m_fDownValue = _downValue;}
	void SetRealValue(float _realValue)						{m_fRealValue = _realValue;}
	const float GetBaseValue()								{return m_fBaseValue;}
	const float GetUpValue()								{return m_fUpValue;}
	const float GetDownValue()								{return m_fDownValue;}
	const float GetRealValue()								{return m_fRealValue;}
protected:
	float m_fBaseValue;     //ˮλ��ֵ,  0-9999.99 3Bytes,BCD����λ:m
	float m_fUpValue;       //ˮλ����ֵ,0-99.99,  2Bytes,BCD����λ:m
	float m_fDownValue;     //ˮλ����ֵ,0-99.99,  2Bytes,BCD����λ:m
	float m_fRealValue;		//ˮλʵʱֵ,0-9999.99, 3Bytes,BCD����λ:m
};

//ˮѹ����,����ˮѹ��Ĳ���
//����:ˮѹ����ֵ��ˮѹ����ֵ
class CWaterPressure_PARA_PER
{
public:
	CWaterPressure_PARA_PER()
	{
		m_dUpValue = 0.0f;
		m_dDownValue = 0.0f;
		m_dRealValue = 0.0f;
	}
	CWaterPressure_PARA_PER(CWaterPressure_PARA_PER* pSource);
public:
	void SetUpValue(double _upValue)						{m_dUpValue = _upValue;}
	void SetDownValue(double _downValue)					{m_dDownValue = _downValue;}
	void SetRealValue(double _realValue)					{m_dRealValue = _realValue;}
	const double GetUpValue()								{return m_dUpValue;}
	const double GetDownValue()								{return m_dDownValue;}
	const double GetRealValue()								{return m_dRealValue;}
protected:
	double m_dUpValue;			//ˮѹ����ֵ,0-999999.99 4Bytes,BCD,��λ;kPa
	double m_dDownValue;		//ˮѹ����ֵ,0-999999.99 4Bytes,BCD,��λ;kPa	
	double m_dRealValue;	    //ˮѹʵʱֵ,0-999999.99 4Bytes,BCD,��λ:kPa,��ѯʵʱֵ��
};

class CHydrWaterLevel
{
public:
	CHydrWaterLevel(){
		m_nSensorNo = -1;
		m_dRealValue = 0.0f;
	}
	~CHydrWaterLevel(){}
	
	void SetZero(){
		m_nSensorNo = -1;
		m_dRealValue = 0.0f;
		m_time.tm_mon = 0;
		m_time.tm_mday = 0;
		m_time.tm_hour = 0;
		m_time.tm_min = 0;
	}
	void SetRealValue(double d){m_dRealValue = d;}
	void SetSensorNo(int d){m_nSensorNo = d;}
	void SetTime(struct tm t){m_time = t;}
	double GetRealValue(){return m_dRealValue;}
	int    GetSensorNo(){return m_nSensorNo;}
	struct tm GetTime(){return m_time;}

private:
	int m_nSensorNo;
	double m_dRealValue;
	struct tm m_time;
};

class CHydrRainValue
{
public:
	CHydrRainValue(){
		m_nDw = 0;
	}
	~CHydrRainValue(){}
	void SetTime(tm t){m_time = t;}
	void SetSpan(int d){m_nTimespan = d;}
	void SetDw(int d){m_nDw = d;}
	tm GetTime(){return m_time;}
	int GetTimespan(){return m_nTimespan;}
	int GetDw(){return m_nDw;}
	void Clear()
	{
		m_lstRainVal.clear();
	}

public:
	CCriticalSection m_mutex;
	std::list<float> m_lstRainVal;
private:
	struct tm m_time;
	int m_nTimespan;//������ʱ��������P<m>�е�m
	int m_nDw; //ʱ������λ 0=���� 1=Сʱ
};

class CHydrSoilMoisture
{//��������
public:
	CHydrSoilMoisture(){}
	~CHydrSoilMoisture(){}

	void SetDepth(int d){m_nDepth = d;}
	void SetSoilVar(float d){m_Data = d;}
	void SetTime(struct tm t){m_t = t;}
	struct tm GetTime(){return m_t;}
	float GetDepth(){return m_nDepth;}
	float GetSoilVar(){return m_Data;}
private:
	int m_nDepth;//��������
	float m_Data; //��������ֵ
	struct tm m_t;
};

class CHydrGWD
{
public:
	CHydrGWD(){}
	~CHydrGWD(){}

	void SetGWD(float d){m_data = d;}
	void SetTime(struct tm t){m_t = t;}
	float GetGWD(){return m_data;}
	struct tm GetTime(){return m_t;}

private:
	float m_data;
	struct tm m_t;
};

class CHydrGWT
{
public:
	CHydrGWT(){}
	~CHydrGWT(){}

	void SetGWT(float d){m_data = d;}
	void SetTime(struct tm t){m_t = t;}
	struct tm GetTime(){return m_t;}
	float GetGWT(){return m_data;}
private:
	float m_data;
	struct tm m_t;
};

class CHydrEvaporate
{
public:
	CHydrEvaporate(){}
	~CHydrEvaporate(){}

	void SetEvaporate(float d){m_data = d;}
	void SetTime(struct tm t){m_t = t;}
	struct tm GetTime(){return m_t;}
	float GetCHydrEvaporate(){return m_data;}
private:
	float m_data;
	struct tm m_t;
};

class CHydrTransCtrlData
{
public:
	CHydrTransCtrlData(){
		m_data = 0.0f;
	}
	~CHydrTransCtrlData(){}

	void SetZero(){
		m_data = 0.0f;
	}
	void SetCtrlData(float d){m_data = d;}
	float GetCtrlData(){return m_data;}
private:
	float m_data;
};


class CWorkCondition
{
public:
	CWorkCondition(){
		m_data = 0.0f;
	}
	~CWorkCondition(){}

	void SetZero(){
		m_data = 0.0f;
	}
	void SetData(float d){m_data = d;}
	float GetData(){return m_data;}
private:
	float m_data;
};

class CInterfaceInfo
{
public:
	CInterfaceInfo(){
		m_data = "";
	}
	~CInterfaceInfo(){}

	void SetZero(){
		m_data = "";
	}
	void SetData(CString d){m_data = d;}
	CString GetData(){return m_data;}
private:
	CString m_data;
};

static WATER_QU_STRUCT water_q_ID[]={
	{"ˮ��",			0,99.9,0,		3,1,"��"},
	{"pHֵ",			1,99.99,0,		4,2,""},
	{"�ܽ���",			2,999.9,0,		4,1,"mg/L"},
	{"��������ָ��",	3,999.9,0,		4,1,"mg/L"},
	{"�絼��",			4,99999,0,		5,0,"uS/cm"},
	{"������ԭ��λ",	5,9999.9,0,		5,1,"mV"},
	{"�Ƕ�",			6,999,0,		3,0,"��"},
	{"��ѧ������",		7,999999.9,0,	7,1,"mg/L"},
	{"��������������",	8,9999.9,0,		5,1,"mg/L"},
	{"����",			9,9999.99,0,	6,2,"mg/L"},
	{"�ܵ�",			10,999.99,0,	5,2,"mg/L"},
	{"ͭ",				11,999.9999,0,	7,4,"mg/L"},
	{"п",				12,99.9999,0,	6,4,"mg/L"},
	{"������",			13,999.99,0,	5,2,"mg/L"},
	{"��",				14,99.99999,0,	7,5,"mg/L"},
	{"��",				15,99.99999,0,	7,5,"mg/L"},
	{"��",				16,99.99999,0,	7,5,"mg/L"},
	{"��",				17,99.99999,0,	7,5,"mg/L"},
	{"���۸�",			18,99.999,0,	5,3,"mg/L"},
	{"Ǧ",				19,99.99999,0,	7,5,"mg/L"},
	{"�軯��",			20,99.999,0,	5,3,"mg/L"},
	{"�ӷ���",			21,99.999,0,	5,3,"mg/L"},
	{"����",			22,999.99,0,	5,2,"mg/L"},
	{"����",			23,99.999,0,	5,3,"mg/L"},
	{"��󳦾�Ⱥ",		24,9999999999,0,10,0,"��/L"},
	{"������",			25,9999.99,0,	6,2,"mg/L"},
	{"�Ȼ���",			26,999999.99,0, 8,2,"mg/L"},
	{"�����ε�",		27,999.99,0,	5,2,"mg/L"},
	{"��",				28,99.99,0,		4,2,"mg/L"},
	{"��",				29,99.99,0,		4,2,"mg/L"},
	{"ʯ����",			30,99.99,0,		4,2,"mg/L"},
	{"�����ӱ�����Լ�",31,99.99,0,		4,2,"mg/L"},
	{"������",			32,9.999999,0,	7,6,"mg/L"},
	{"�ε���",			33,9.999999,0,	7,6,"mg/L"},
	{"�л���ũҩ",		34,9.999999,0,	7,6,"mg/L"}
};

//����ˮ�ʵĲ�����Ϣ
//����bit 0-39
//ֵ�ķ�Χ�����ֵ������ֵ
//ֵ��������ֵ�� up,max
class CWaterQuality_PARA;
class CWaterQuality_PARA_PER
{
public:
	CWaterQuality_PARA_PER()
	{
		m_ScaleMax = 0.0;
		m_ScaleMin = 0.0;
		m_strName  = "";
		m_strUnit  = "";
		m_BitPos = 0;
		m_DataBits = 0;
		m_UpValume = 0.0;
		m_DownValue = 0.0;
		m_PointBits = 0;
		m_BUF_Cnt = 0;
		m_CurValue = 0.0;
	}
	CWaterQuality_PARA_PER(CWaterQuality_PARA_PER* pSource);
public:
	DOUBLE m_ScaleMax;				//���ֵ
	DOUBLE m_ScaleMin;				//��Сֵ
	BYTE   m_BitPos;				//bitλλ�� 0 - 39
	BYTE   m_DataBits;              //����λ����������С���� 0-10
	BYTE   m_PointBits;				//����С��λ��
	BYTE   m_BUF_Cnt;               //����ˮ�ʲ����ı����ֽ��� 4/5

	DOUBLE m_UpValume;				//����ֵ
	DOUBLE m_DownValue;				//����ֵ
	//bool   m_bCheck_FLAG;			//�Ƿ���ı�־ 0 ������⣬ 1:���
	DOUBLE m_CurValue;              //ˮ�ʲ���ʵʱֵ
	CString m_strName;              //ˮ�ʲ���������
	CString m_strUnit;              //��λ
};

//ˮ�ʲ���
//������ˮ�ʲ������ࡢ���Ƿ���򲻼��
class CWaterQuality_PARA
{
public:
	CWaterQuality_PARA(){m_wQualityValue = 0;}
	~CWaterQuality_PARA();
	void Clear();
	CWaterQuality_PARA_PER* GetOneQualityObject(BYTE _bitPos);
	void SetQualityValue(UINT64 _Value) {m_wQualityValue = _Value;}
	void SetBitValue(size_t _pos, bool _val = true);
	const UINT64 GetQualityValue() {return m_wQualityValue;}
	bool GetBitValue(size_t _pos);
	void BuildQualityMap();
	int Get_Buffer_Count();     //��ȡˮ�ʲ������ĵĳ��ȣ� 5 + 4*N(+1)
protected:
	UINT64 m_wQualityValue;      //5���ֽڵ�ˮ�ʲ�����־λ 40bits
	std::map<BYTE,CWaterQuality_PARA_PER*> m_mapWaterQulity;

};

//�����ı�ף���ʼ��ֵ���������������PER
class CFlowInitValume_PARA_PER
{
public:
	CFlowInitValume_PARA_PER()
	{
		m_dwCalTakeValume = 0;
		m_dwFlowInitValume = 0;
		m_dwInsFlowValume = 0;
	}
	CFlowInitValume_PARA_PER(CFlowInitValume_PARA_PER* pSource);
	void SetFlowInitValume(DWORD _FlowInitV) {m_dwFlowInitValume = _FlowInitV;}  //����������׵ĳ�ʼֵ
	void SetCalTakeValume(DWORD CalValume)   {m_dwCalTakeValume = CalValume;}	 //�����ۼ�ȡˮ��
	void SetInsFlowValume(DWORD InsFlowV)	 {m_dwInsFlowValume = InsFlowV;}	 //����˲ʱ����
	const DWORD GetFlowInitValume()	{return m_dwFlowInitValume;}				 //��ȡ������׵ĳ�ʼֵ
	const DWORD GetCalTakeValume()	{return m_dwCalTakeValume;}					 //��ȡ�ۼ�ȡˮ��
	const DWORD GetInsFlowValume()	{return m_dwInsFlowValume;}					 //��ȡ˲ʱ����
protected:
	DWORD m_dwFlowInitValume;     //������׵ĳ�ʼֵ��0-99999999��4 Bytes, BCD����λ��������
	DWORD m_dwCalTakeValume;      //�ۼ�ȡˮ�� ��λ�������ף�0-9999999��4Bytes,BCD
	DWORD m_dwInsFlowValume;      //˲ʱ���� ��λ:������/h 0-9999,2bytes,BCD
};


static CString  _EventRecordName[] ={
	"���ݳ�ʼ����¼",
	"���������¼",
	"״̬����λ��¼",
	"�Ǳ���ϼ�¼",
	"��������¼",
	"�ն˹��ϼ�¼",
	"����ʧ���¼",
	"���ص�ѹ�͸澯��¼",
	"�ն���Ƿ��򿪼�¼",
	"ˮ�ù�����¼",
	"ʣ��ˮ��Խ�޸澯��¼",
	"ˮλ���޸澯��¼",
	"ˮѹ���޸澯��¼",
	"ˮ�ʲ������޸澯��¼",
	"���ݳ����¼",
	"�����ļ�¼",
	"�ձ��ļ�¼",
	"�����������޸澯��¼",
	"ģ�����������޸澯��¼"
};

//�¼���¼����
class CEventRecord_PER
{
public:
	CEventRecord_PER()
	{
		strName = "";
		bPos = 0;
		wEventCount = 0;
	}
	CString strName;	 //����
	BYTE    bPos ;		 //1-32
	WORD    wEventCount; //�¼���¼��
};

//��Ȳ�����per��
class CPulsePara_PER
{
private:
	CString    m_strCodeString;
	UINT64  m_unCode;    //��ȱ��ַ�� BCD 0-999999999999 6 �ֽ�
	double  m_dbValue1;	 //�����й�     BCD 0-999999.99	   4 �ֽ�
	double  m_dbValue2;	 //�����޹�     BCD 0-999999.99	   4 �ֽ�
	double  m_dbValue3;	 //�����й�		BCD 0-999999.99	   4 �ֽ�
	double	m_dbValue4;	 //�����޹�		BCD 0-999999.99	   4 �ֽ�
public:
	CPulsePara_PER(UINT64 Code = 0,
		           double value1 = 0,
				   double value2 = 0,
				   double value3 = 0,
				   double value4= 0,
				   CString strCode = "");
	CPulsePara_PER(CPulsePara_PER* pSource);
public:
	const UINT64 GetCode();   //��ȱ��ַ��
	const double GetValue1(); //�����й�
	const double GetValue2(); //�����޹� 
	const double GetValue3(); //�����й�
	const double GetValue4(); //�����޹�
	const CString GetCodeString();

	void SetCode(UINT64 code);		//��ȱ��ַ�� BCD 0-999999999999 6 �ֽ�
	void SetValue1(double value1);	//�����й�     BCD 0-999999.99	   4 �ֽ�
	void SetValue2(double value2);	//�����޹�     BCD 0-999999.99	   4 �ֽ�
	void SetValue3(double value3);	//�����й�		BCD 0-999999.99	   4 �ֽ�
	void SetValue4(double value4);	//�����޹�		BCD 0-999999.99	   4 �ֽ�
	void SetCodeString(CString strCode);
};

//ң���ն���ʷ�ռ�¼
class CDayHistoryData
{
public:
	CDayHistoryData(){ClearData();}
protected:
	BYTE mZBType;		//��������׼ 2���㡢1���
	BYTE mQueryNo;		//��ѯ�����ı�� 0-7
	BYTE mDataType;		//�������ͣ�ˮλ��ˮѹ��ˮ��...
	WORD mYear;			//��
	WORD mMonth;		//��
	WORD mDay;			//��

	BYTE mPointBits;	//С��λ��
	CString sUnit;		//��λ

	SYSTEMTIME mMaxTime; //���ֵʱ��
	SYSTEMTIME mMinTime; //��Сֵʱ��
public:
	void SetZBType(BYTE _value)				{mZBType = _value;}
	void SetQueryNo(BYTE _value)			{mQueryNo = _value;}
	void SetDataType(BYTE _value)			{mDataType = _value;}
	void SetYear(WORD _value)				{mYear = _value;}
	void SetMonth(WORD _value)				{mMonth = _value;}
	void SetDay(WORD _value)				{mDay = _value;}
	
	void SetPointBits(BYTE _Value)			{mPointBits = _Value;}
	void SetUnit(CString _value)			{sUnit = _value;}

	BYTE GetZBType() const					{return mZBType;}
	BYTE GetQueryNo() const					{return mQueryNo;}
	BYTE GetDataType() const				{return mDataType;}
	WORD GetYear() const					{return mYear;}
	WORD GetMonth()	const					{return mMonth;}
	WORD GetDay() const						{return mDay;}
	
	BYTE GetPointBits() const				{return mPointBits;}
	CString GetUnit() const					{return sUnit;}

	PSYSTEMTIME GetMaxTime()				{return &mMaxTime;}
	PSYSTEMTIME GetMinTime()				{return &mMinTime;}
	void ClearData(){
		mZBType = 0;
		mQueryNo = 0;
		mDataType = 0;
		mYear = 0;
		mMonth = 0;
		mDay = 0;
		mPointBits = 0;
		sUnit = "";
		GetLocalTime(&mMaxTime);
		GetLocalTime(&mMinTime);
	}	
};

//ˮ����ң��վIP��������Ϣ
class CSYQIPConfigData
{
public:
	CSYQIPConfigData()
	{
		m_strip1 = " ";
		m_commtype1 = 1;
	    m_strip2 =" ";
		m_commtype2 =  1;
	    m_strip3 =" ";
		m_commtype3 = 1;
		m_strip4 =" ";
		m_commtype4 = 1;
	}
public:
	CString  m_strip1;
	short    m_commtype1;
	CString  m_strip2;
	short    m_commtype2;
	CString  m_strip3;
	short    m_commtype3;
	CString  m_strip4;
	short    m_commtype4;

	void SetIp1(short cn,CString strip){m_strip1 = strip, m_commtype1 = cn;}
	void SetIp2(short cn,CString strip){m_strip2 = strip, m_commtype2 = cn;}
	void SetIp3(short cn,CString strip){m_strip3 = strip, m_commtype3 = cn;}
	void SetIp4(short cn,CString strip){m_strip4 = strip, m_commtype4 = cn;}
};

#ifndef  _rtu_attrib_type_const_
#define  _rtu_attrib_type_const_
	const int rtu_attr_Connect_check	= 0x01;				//��·���
	const int rtu_attr_workmode_call	= 0x02;				//����ģʽ��
	const int rtu_attr_workmode_set		= 0x03;				//����ģʽ����
	const int rtu_attr_ycquerypara_call = 0x04;				//ң���ѯ������ѯ
	const int rtu_attr_ycquerypara_set  = 0x05;				//ң�Ų�ѯ��������
	const int rtu_attr_autoRpPara_call	= 0x06;				//�Ա�������ѯ
	const int rtu_attr_atuoRpPara_set	= 0x07;				//�Ա���������
	const int rtu_attr_datetime_call	= 0x08;				//��ѯң���ն˻��м�վʱ��
	const int rtu_attr_datetime_set     = 0x09;				//����ң���ն˻��м�վʱ��
	const int rtu_attr_rainalert_call   = 0x0A;				//�������ݱ���ֵ��ѯ
	const int rtu_attr_rainalert_set    = 0x0B;				//�������ݱ���ֵ����
	const int rtu_attr_analogminmax_call= 0x0C;				//ģ����������ֵ��ѯ
	const int rtu_attr_analogminmax_set = 0x0D;				//ģ����������ֵ����
	const int rtu_attr_water_recharge_call = 0x0E;			//ˮ��������ѯ
	const int rtu_attr_water_alert_call	= 0x0F;				//ˮ����������
	const int rtu_attr_waterlevel_call	= 0x20;				//ˮλ�������ò�ѯ
	const int rtu_attr_waterlevel_data_call   = 0x21;		//ˮλ����ʵʱ��ѯ
	const int rtu_attr_waterpress_call  = 0x22;				//ˮѹ������ѯ
	const int rtu_attr_waterpress_data_call   = 0x23;		//ˮѹʵʱ���ݲ�ѯ
	const int rtu_attr_waterquality_up_call   = 0x24;       //ˮ�ʲ�������ֵ��ѯ
	const int rtu_attr_waterquality_down_call = 0x25;       //ˮ�ʲ�������ֵ��ѯ
	const int rtu_attr_waterquality_data_call = 0x26;       //ˮ�ʲ���ʵʱֵ��ѯ
	const int rtu_attr_alarmstate_call        = 0x27;       //ң���ն˱���״̬��ң���ն�״̬��ѯ
	const int rtu_attr_exalarmstate_call      = 0x28;       //ң���ն���չ����״̬��ң���ն�״̬��ѯ
	const int rtu_attr_yk_operate             = 0x29;       //ң�ز���
	const int rtu_attr_rain_data_call         = 0x2A;       //����ʵʱ���ݲ�ѯ 
	const int rtu_attr_analog_data_call       = 0x2B;       //ģ����ʵʱ���ݲ�ѯ
	const int rtu_attr_flowpara_set           = 0x2C;		//�����������
	const int rtu_attr_flowdata_call		  = 0x2d;       //����ʵʱ���ݲ�ѯ
	const int rtu_attr_eventrecord_call       = 0x2e;       //��ѯң���ն˵��¼���¼
	const int rtu_attr_pulsepara_call		  = 0x2f;		//��ѯ���ʵʱ����
#endif

#ifndef  _scsw_attrib_type_const_
#define  _scsw_attrib_type_const_
	const int scsw_no_call_operation = 0x100;//��ǰ�޲�ѯ����
	const int scsw_waterLevel_call = 0x101;//��ѯˮλ��Ӧ
	const int scsw_waterRain_call  = 0x102;//��ѯ������Ӧ
	const int scsw_waterSoil_call  = 0x103;//��ѯ����������Ӧ
	const int scsw_waterGWD_call   = 0x104;//��ѯ����ˮ˲ʱ�����Ӧ
	const int scsw_waterGWT_call   = 0x105;//��ѯ����ˮ˲ʱ�¶���Ӧ
	const int scsw_waterEvaporate_call   = 0x106;//��ѯ������Ӧ	
	const int scsw_waterRainStardard = 0x107;
	const int scsw_waterRainSensorDpi = 0x108;
	const int scsw_waterRainSensorBase = 0x109;
	const int scsw_waterRainSensorPara = 0x110;
	const int scsw_waterDefine = 0x111;
	const int scsw_waterrainTransStardard = 0x112;
	const int scsw_waterrainTimespan = 0x113;

	const int scsw_waterRainStardard_set = 0x107;
	const int scsw_waterRainSensorDpi_set = 0x108;
	const int scsw_waterRainSensorBase_set = 0x109;
	const int scsw_waterRainSensorPara_set = 0x110;
	const int scsw_waterDefine_set = 0x111;
	const int scsw_waterrainTransStardard_set = 0x112;
	const int scsw_waterrainTimespan_set = 0x113;

	const int scsw_workconditionSensorVar = 0x114;
	const int scsw_workconditionMemoryMargin = 0x115;
	const int scsw_workconditionEvoirTemperature = 0x116;
	const int scsw_workconditionSolarCellVoltage = 0x117;
	const int scsw_workconditionStorageVoltage = 0x118;
	const int scsw_workconditionRainCntVar = 0x119;
	const int scsw_workconditionSensorInstallStatus = 0x120;
	const int scsw_workconditionMemoryPtr = 0x121;

	const int scsw_waterCheckTime = 0x122;
	const int scsw_interface_GSM = 0x123;
	const int scsw_interface_BD = 0x124;
	const int scsw_interface_IP = 0x125;
	const int scsw_interface_IP2 = 0x225;
	const int scsw_interface_PORT = 0x126;

	const int scsw_interface_GSM_Set = 0x128;
	const int scsw_interface_BD_Set = 0x129;
	const int scsw_interface_IP_Set = 0x130;
	const int scsw_interface_IP_Set2 = 0x230;
	const int scsw_interface_PORT_Set = 0x131;

	const int scsw_interface_GSM_Del = 0x133;
	const int scsw_interface_BD_Del = 0x134;
	const int scsw_interface_IP_Del = 0x135;
	const int scsw_interface_IP_Del2 = 0x235;

	const int WriteUpdate_Start = 0x136;
	const int WriteUpdate_Read  = 0x137;
	const int WriteUpdate_Send  = 0x138;
	const int WriteUpdate_NoOp  = 0xff;

	const int writeUpdate_A0 = 0x139;
	const int writeUpdate_A1 = 0x140;
	const int writeUpdate_A2 = 0x141;
	const int writeUpdate_A3 = 0x142;
	const int writeUpdate_B0 = 0x143;
	const int writeUpdate_B1 = 0x144;
	const int writeUpdate_B2 = 0x145;
	const int writeUpdate_B3 = 0x146;
#endif

#ifndef _syq_attrib_type_const_
#define _syq_attrib_type_const_
	//const int rtu_config_ip_tab   = 0x201;//ң���������1
	//const int rtu_config_data1_tab1 = 0x202;//ң���������2-1
	//const int rtu_config_data1_tab2 = 0x203;//ң���������2-2
	//const int rtu_config_data2_tab1 = 0x204;//ң���������3-1
	//const int rtu_config_data2_tab2 = 0x205;//ң���������3-1
	//const int rtu_rain_data_tab = 0x206;// ����
	//const int rtu_waterlevel_tab   = 0x207;//ˮλ
	
	const int syq_read_address1  = 0x201;
	const int syq_config_address1  = 0x202;
	const int syq_read_address2  = 0x203;
	const int syq_config_address2  = 0x204;
	const int syq_read_address3 = 0x205;
	const int syq_config_address3  = 0x206;
	const int syq_read_address4  = 0x207;
	const int syq_config_address4  = 0x208;
	const int syq_read_password = 0x209;
	const int syq_config_password = 0x20A;
	const int syq_config2_Write  = 0x20B;
	const int syq_config2_Read  = 0x20C;
	const int syq_config3_Write  = 0x20D;
	const int syq_config3_Read  = 0x20E;
	const int syq_init_memory   = 0x20F;
	const int syq_check_time    = 0x210;
	const int syq_call_data     = 0x211;//��ѯָ��Ҫ��
	const int syq_call_data_bytime = 0x212;//��ѯָ��ʱ���ָ��Ҫ��
	const int syq_call_data1     = 0x213;//��ѯʵʱ����
	const int syq_call_data2     = 0x214;//��ѯң��վ�˹�����
	const int syq_call_state     = 0x215;//��ѯң��վ״̬�ͱ���



#endif

//ң�ز����������
#ifndef _YK_RESULT_TYPE_CONST��                       
#define _YK_RESULT_TYPE_CONST��
	const BYTE YKStart_PumpGate_SUC = 0x01;                //ң������ˮ�û�բ��/���� �ɹ�
	const BYTE YKStart_PumpGate_ERR = 0x02;				   //ң������ˮ�û�բ��/���� ʧ��
	const BYTE YKClose_PumpGate_SUC = 0x03;				   //ң�عر�ˮ�û�բ��/���� �ɹ�
	const BYTE YKClose_PumpGate_ERR = 0x04;				   //ң�عر�ʧ�ܻ�բ��/���� ʧ��
	const BYTE YKICCard_Enable_SUC  = 0x05;                //ң��IC���������� �ɹ�
	const BYTE YKICCard_Enable_ERR  = 0x06;				   //ң��IC��������Ч ʧ��
	const BYTE YKICCard_DisEnable_SUC = 0x07;              //ң��ȡ��IC������ �ɹ�
	const BYTE YKICCard_DisEnable_ERR = 0x08;			   //ң��ȡ��IC������ ʧ��
	const BYTE YKZD_Input_SUC = 0x09;					   //ң�ض�ֵ����Ͷ�� �ɹ�
	const BYTE YKZD_Input_ERR = 0x0A;					   //ң�ض�ֵ����Ͷ�� ʧ��
	const BYTE YKZD_Exit_SUC  = 0x0B;					   //ң�ض�ֵ�����˳� �ɹ�
	const BYTE YKZD_Exit_ERR  = 0x0C;                      //ң�ض�ֵ�����˳� ʧ��
	const BYTE YKReset_Rtu_SUC = 0x0D;					   //��λң���ն˲��� �ɹ�
	const BYTE YKReset_Rtu_ERR = 0x0E;					   //��λң���ն˲��� ʧ��
	const BYTE YKClear_His_SUC = 0x0F;					   //���ң���ն���ʷ���ݵ�Ԫ �ɹ�
	const BYTE YKClear_his_ERR = 0x10;					   //���ң���ն���ʷ���ݵ�Ԫ ʧ��
#endif

class CSCMessageDealModule;
class CRealTimeDB;
class CSysDataTrans;
class CRtuStation:public CUdfBase
{
public:
	CRtuStation(void);
	CRtuStation(PRTU_STATION pInfo);
public:
	virtual ~CRtuStation(void);
	void Clear();

	//��ձ��������Ϣ������
	void ClearYcList();
	void ClearYxList();
	void ClearYkList();
	void ClearZdList();
	void ClearRainAlartParaList();																		//����������ݱ���ֵ�Ĳ�������
	void ClearAnalogLimitParaList();																	//���ģ���������޲�������
	void ClearWaterLevelParaList();																		//���ˮλ�����������Ϣ
	void ClearWaterPressureParaList();																	//���ˮѹ�����������Ϣ
	void ClearFlowInitValumeParaList();																	//����������ײ���������Ϣ
	void ClearEventRecordsList();	//����¼���¼�б�����Ϣ����Ҫ������������

	void ClearWriteUpdateList();
public:
	void					SetParent(CRealTimeDB* _pParent);
	CRealTimeDB*			GetParent();

    bool BuildAllData(CDatabase* pDb);																	//��ȡ���е���Ϣ yC Yx yK Zd
	bool BuildBasicPacket(CPacketFrameFormat& appFrame);	
    bool BuildBasicPacket(CWriteFrame& appFrame);
	bool BuildBasicPacket(CHydrSendFrame& appFrame);
	bool BuildBasicPacket(CSYQFrameFormat& appFrame);   //ˮ����
	std::list<WriteDataFromFile *> m_pWriteUpdateList;
	std::list<WriteDataFromFile *>::iterator curr_it;
	CCriticalSection          m_cs_writeUpdate;

	std::map<int, WriteDataFromFile*> m_WriteUpdateMap;
	std::map<int, WriteDataFromFile*>::iterator m_it_map;
	bool bIteratorVaild;//��������Ч��
	bool bIsWriteUpdateStart;
	int m_CurrUpdataPackCnt;
	int m_CurrentWriteUpdateOp;

	CRTU_AUTOREPORT_PRA* GetAutoReportPara(){return &m_AutoReportPara;}									//��ȡ�Ա�������Ϣ
	CRTU_YCQUERYTYPE_PARA* GetYcQueryType(){return &m_YcQueryTypePara;}									//��ȡң��վ��Ҫ��ѯ��ʵʱ����������Ϣ��������
	const int GetReSendInterval(){return m_nReSendInterval;}											//��ȡ�ط����ĵ�ʱ��������λ:��(second)

	CList<CRainAlert_PARA_PER*>* GetRainAlertParasList() {return &m_lstRainAlertParas;}					//��ȡ�������ݱ���ֵ��������
	CRainAlert_PARA_PER* GetRainAlartPara(int Index);													//��ȡ����һ���������ݱ���ֵ�Ĳ���,IndexΪ������

	CList<CAnalogLimit_PARA_PER*>* GetAnalogLimitParasList(){return &m_lstAnalogLimitParas;}			//��ȡģ���������޲�������
	CAnalogLimit_PARA_PER* GetAnalogLimitPara(int Index);												//��ȡ����һ��ģ���������޲���,IndexΪ������
	CWaterValume_PARA*  GetWaterValumePara(){return &m_sWaterValumePara;}								//��ȡ��RTU��ˮ��������Ϣ

	CList<CWaterLevel_PAR_PER*>* GetWaterLevelParasList(){return &m_lstWaterLevelParas;}				//��ȡˮλ������ļ���
	CWaterLevel_PAR_PER* GetWaterLevelPara(int Index);													//��ȡ����һ��ˮλ��Ĳ���,IndexΪ������

	CList<CWaterPressure_PARA_PER*>* GetWaterPressureParasList(){return &m_lstWaterPressureParas;}		//��ȡˮѹ������ļ���
	CWaterPressure_PARA_PER* GetWaterPressurePara(int Index);											//��ȡ����һ��ˮѹ��Ĳ�������IndexΪ������

	CList<CFlowInitValume_PARA_PER*>* GetFlowInitValumeParasList(){return &m_lstFlowInitValumeParas;}
	CFlowInitValume_PARA_PER* GetFlowInitValumePara(int Index);											//��ȡ����һ���������׵ĳ�ʼ��ֵ,IndexΪ������

	//������
	std::map<UINT64,CPulsePara_PER*>* GetPulseParasMap(){return &m_mapPulseParas;}
	CPulsePara_PER*					  GetPulseParaObject(UINT64 code);
	void							  InsertPulseParaObject(CPulsePara_PER* pPulseObject);

	void Set_RESET_Cmd(BYTE _Cmd){m_bRESET_Cmd = _Cmd;}													//���ø�λ����
	const BYTE Get_RESET_Cmd() {return m_bRESET_Cmd;}													//��ȡ��λ����	
	CClearHistoryData_PARA* GetClearHistoryDataPara(){return &m_ClearHistoryDataPara;}					//��ȡ���ң���ն���ʷ���ݵ�Ԫ�Ĳ�����Ϣ
	CPumpGate_YK_PARA* GetCurPumpGateObjecte(){return &m_Cur_PumpGate_Object;}							//��ȡ��ǰң�ز�����ˮ�û���/բ�Ŷ���
	CRTUAlarm_State*   GetRtuAlarm_State() {return &m_RTUAlarm_State;}									//��ȡң���ն˵ı���״̬��Ϣ����
	CRTU_EXAlarm_State* GetRtuExAlarm_State(){return &m_RtuExAlarm_State;}								//��ȡң���ն���չ����״̬����
	CRTUTerminal_State* GetRtuTerminal_State(){return &m_RTUTerminal_State;}							//��ȡң���ն˵�״̬��Ϣ����
	CWaterPump_WorkData* GetWaterPump_WorkData(){return &m_WaterPump_WorkData;}							//ˮ�õ��ʵʱ����������Ϣ����

	CWaterQuality_PARA*  GetWaterQualityPara(){return &m_WaterQualityPara;}								//ˮ�ʲ���
	CWaterQuality_PARA_PER* GetOne_WaterQualityPara(BYTE _bitPos);                                      //��ȡ����һ��ˮ�ʲ���
	const BYTE GetQueryRealDataCmd()	{return m_QueryRealData_Cmd;}									//��ѯң���ն�ʵʱ���������� �������Cmdֵ
	void       SetQueryRealDataCmd(BYTE _qCmd){m_QueryRealData_Cmd = _qCmd;}							//����ң���ն�ʵʱ���������� �������Cmdֵ
	void       Set_YK_Op_Result(BYTE _ykresult) {m_YK_OP_Result = _ykresult;}							//����ң�ز����Ľ��
	const BYTE Get_YK_Op_Result()				{return m_YK_OP_Result;}								//��ȡң�ز����Ľ��
	CDayHistoryData*	GetDayHistoryObject()	{return &m_DayHisoryData;}								//��ȡ��ʷ�ռ�¼����

	CList<CEventRecord_PER*>* GetEventRecordsList(){return &m_lstEventRecords;}
	CEventRecord_PER*         GetEventRecordObject(int Index);											//Index 0-18
	CEventRecord_PER*         GetEventRecordByPos(int _pos);											//_pos = 1-19
	void					  BuildEventRecordsList();													//�����¼���¼�б�
	bool					  GetRecvFLAG(){return m_blRecvFLAG;}										//��ȡ�Ƿ���ձ��ı�־
protected:
	CRealTimeDB*		m_pParent;																		//RealTimeDBʵ������
	CRTU_AUTOREPORT_PRA m_AutoReportPara;																//�Ա�������Ϣ
	CRTU_YCQUERYTYPE_PARA m_YcQueryTypePara;															//ң��վ��Ҫ��ѯ��ʵʱ����������Ϣ����
	int					m_nReSendInterval;																//�ظ�ʱ����,��λ:�루second��  
	BYTE                m_bRESET_Cmd;																	//��λң���ն˵���� HEX 01,��λ������ 02 ��λ �ָ�����Ĭ��ֵ��������
	
	//-----------------------------------ˮ��Դʹ�õ����ݻ�������---------------------------------------------
	CList<CRainAlert_PARA_PER*> m_lstRainAlertParas;													//�������ݱ���ֵ��������
	CList<CAnalogLimit_PARA_PER*> m_lstAnalogLimitParas;												//ģ���������޲�������
	CWaterValume_PARA	m_sWaterValumePara;																//�洢ˮ����������
	CList<CWaterLevel_PAR_PER*> m_lstWaterLevelParas;													//�洢ˮλ�������Ϣ�ļ���
	CList<CWaterPressure_PARA_PER*> m_lstWaterPressureParas;											//�洢ˮѹ������������ޣ���Ϣ�ļ���
	CList<CFlowInitValume_PARA_PER*> m_lstFlowInitValumeParas;											//�洢������׵ĳ�ʼ��ֵ����
	CClearHistoryData_PARA m_ClearHistoryDataPara;														//�洢���ң���ն˵���ʷ���ݵ�Ԫ������Ϣ
	CPumpGate_YK_PARA      m_Cur_PumpGate_Object;														//��ǰ������ң��ˮ�û���/բ�Ŷ���
	CRTUAlarm_State		   m_RTUAlarm_State;															//ң���ն˵ı���״̬
	CRTU_EXAlarm_State     m_RtuExAlarm_State;															//ң��֪����չ����״̬
	CRTUTerminal_State     m_RTUTerminal_State;															//ң���ն˵�״̬��Ϣ
	CWaterPump_WorkData    m_WaterPump_WorkData;														//ˮ�õ��ʵʱ��������
	CWaterQuality_PARA     m_WaterQualityPara;															//ˮ�ʲ�����Ϣ

	BYTE                   m_QueryRealData_Cmd;															//��ѯң���ն�ʵʱֵ������
	BYTE                   m_YK_OP_Result;																//ң�ز������;
	CList<CEventRecord_PER*> m_lstEventRecords;															//�¼���¼list
	CDayHistoryData		  m_DayHisoryData;																//��ʷ�ռ�¼����
	bool                   m_blRecvFLAG;																//�Ƿ��Ѿ����ձ��ı�׼�� ��ʼ״̬Ϊ false ,
	std::map<UINT64,CPulsePara_PER*> m_mapPulseParas;													//��Ȳ���map

	//-----------------------SCSW008 ˮ�����ݻ���----------------------
	CHydrWaterLevel m_RealHydrWaterLevel;
	CHydrRainValue  m_RealHydrRainVal;
	CHydrSoilMoisture m_RealSoilMoisture;
	CHydrGWD m_RealGWD;
	CHydrGWT m_RealGWT;
	CHydrEvaporate m_RealEvaporate;

	CHydrTransCtrlData m_rainStardard;//�����ӱ���׼
	CHydrTransCtrlData m_rainSensorDpi;//ˮλ�������ֱ���
	CHydrTransCtrlData m_rainSensorBase;//ˮλ����������
	CHydrTransCtrlData m_rainSensorPara;//ˮλ����������ת��ϵ��
	CHydrTransCtrlData m_rainDefine;//ˮλ���㹫ʽ����
	CHydrTransCtrlData m_rainSendStardard;//ˮλ���ͱ�׼
	CHydrTransCtrlData m_rainSendTimeSpan;//ˮλ���ͼ��
	CWorkCondition m_wcSensorvar;//����������
	CWorkCondition m_wcMemoryMargin;
	CWorkCondition m_wcEviorTemperation;
	CWorkCondition m_wcSolarCellVoltage;
	CWorkCondition m_wcStorageVoltage;
	CWorkCondition m_wcRainCntVar;
	CWorkCondition m_wcSensorInstallStatus;
	CWorkCondition m_wcDataStoragePtr;

	CInterfaceInfo m_iiGSM;
	CInterfaceInfo m_iiBD;
	CInterfaceInfo m_iiIP;
	CInterfaceInfo m_iiIP2;
	CInterfaceInfo m_iiPort;


	//ˮ����ң��վ��������
public:
	CSYQIPConfigData  m_syqipconfigdata;
	_SYQ_CONFIG_PACKET2 m_syqconfig2data;
	_SYQ_CONFIG_PACKET3 m_syqconfig3data;

	std::list<_SYQ_DATA_DESCRIBE*> m_syqdatadiscribelist;
	int  m_syqState;

	void SetSyqConfig2Data(_SYQ_CONFIG_PACKET2  syqcfg2packet){m_syqconfig2data = syqcfg2packet;}
	void SetSyqConfig3Data(_SYQ_CONFIG_PACKET3  syqcfg3packet){m_syqconfig3data = syqcfg3packet;}

public:
	CHydrWaterLevel* GetHydrWaterLevel(){return &m_RealHydrWaterLevel;}
	CHydrRainValue*  GetHydrRainVal(){return &m_RealHydrRainVal;}
	CHydrSoilMoisture* GetHydrSoilMoisture(){return &m_RealSoilMoisture;}
	CHydrGWD *       GetHydrGWD(){return &m_RealGWD;}
	CHydrGWT*        GetHydrGWT(){return &m_RealGWT;}
	CHydrEvaporate*  GetHydrEvaporate(){return &m_RealEvaporate;}
	CHydrTransCtrlData *GetRainStardard(){return &m_rainStardard;}
	CHydrTransCtrlData *GetRainSensorDpi(){return &m_rainSensorDpi;}
	CHydrTransCtrlData *GetRainSensorBase(){return &m_rainSensorBase;}
	CHydrTransCtrlData *GetRainSensorPara(){return &m_rainSensorPara;}
	CHydrTransCtrlData *GetRainDefine(){return &m_rainDefine;}
	CHydrTransCtrlData *GetRainSendStardard(){return &m_rainSendStardard;}
	CHydrTransCtrlData *GetRainSendTimeSpan(){return &m_rainSendTimeSpan;}
	CWorkCondition *GetSensorVar() {return &m_wcSensorvar;}//����������
	CWorkCondition *GetMemoryMargin() {return &m_wcMemoryMargin;}
	CWorkCondition *GetEviorTemperation() {return &m_wcEviorTemperation;}
	CWorkCondition *GetSolarCellVoltage() {return &m_wcSolarCellVoltage;}
	CWorkCondition *GetStorageVoltage() {return &m_wcStorageVoltage;}
	CWorkCondition *GetRainCntVar() {return &m_wcRainCntVar;}
	CWorkCondition *GetSensorInstallStatus() {return &m_wcSensorInstallStatus;}
	CWorkCondition *GetDataStoragePtr() {return &m_wcDataStoragePtr;}
	CInterfaceInfo *GetGSMValue(){return &m_iiGSM;}
	CInterfaceInfo *GetBDValue(){return &m_iiBD;}
	CInterfaceInfo *GetIPValue(){return &m_iiIP;}
	CInterfaceInfo *GetIPValue2(){return &m_iiIP2;}
	CInterfaceInfo *GetPortValue(){return &m_iiPort;}


	//--------------------------------------------------------------------------------

	CPtrList m_lstYcObjects;																			//ң������б�
	CPtrList m_lstYxObjects;																			//ң�Ŷ����б�
	CPtrList m_lstYkObjects;																			//ң�ض����б�
	CPtrList m_lstZdObjects;																			//��ֵ�����б�
	bool BuildYxObjects(CDatabase* pDb);																//��ȡ����ң�������Ϣ
	bool BuildYcObjects(CDatabase* pDb);																//��ȡ����ң�Ŷ�����Ϣ
	bool BuildYkObjects(CDatabase* pDb);																//��ȡ����ң�ض�����Ϣ
	bool BuildZdObjects(CDatabase* pDb);																//��ȡ������ֵ������Ϣ
	bool BuildExtInformation(CDatabase* pDb);															//��չ��Ϣ������ͨ�ŵ�ַ��Э��汾�ţ�
	bool InitReSendInterval();																			//�����ط�ʱ�������ط�ʱ��������λ���루second��											    
public:
	CPtrList* GetYcObjectsList();
	CPtrList* GetYxObjectsList();
	CPtrList* GetYkObjectsList();
	CPtrList* GetZdObjectsList();
public:
	CRtuYcObject* SearchYcObject(WORD _Code);
	CRtuYcObject* SearchYcObject(CString _ycID);
	CRtuYxObject* SearchYxObject(WORD _Code);
	CRtuYxObject* SearchYxObject(CString _yxID);
	CRtuYkObject* SearchYkObject(WORD _Code);
	CRtuYkObject* SearchYkObject(CString _ykID);
	CRtuZdObject* SearchZdObject(WORD _Code);
	CRtuZdObject* SearchZdObject(CString _zdID);
public:
	bool CheckYcObject(WORD ycCode,CRtuYcObject* pCurYc = NULL);			//���ң������Ƿ���ڣ� pCurYc Ϊ��ǰyc���� true ����, false ������
	bool CheckYxObject(WORD yxCode,CRtuYxObject* pCurYx = NULL);			//���ң�Ŷ����Ƿ���ڣ� pCurYx Ϊ��ǰyx���� true ����, false ������
	bool CheckYkObject(WORD ykCode,CRtuYkObject* pCurYk = NULL);			//���ң�ض����Ƿ���ڣ� pCurYk Ϊ��ǰyk���� true ����, false ������
	bool CheckZdObject(WORD zdCode,CRtuZdObject* pCurZd = NULL);			//��鶨ֵ�����Ƿ���ڣ� pCurZd Ϊ��ǰzd���� true ����, false ������
	bool CheckModify(PRTU_STATION pNewRtuInfo);								//����µ���Ϣ���뱾�ǵ���Ϣ�Աȣ��õ��Ƿ����޸ģ� true ���޸ģ�false û�޸�
	void SetLastRecvTime(PSYSTEMTIME pSt= NULL);							//�������½��ձ��ĵ�ʱ��
	PSYSTEMTIME GetLastRecvTime();											//��ȡ������ձ��ĵ�ʱ��
	const CString GetLastRecvTimeString();									//��ȡ���½��ձ���ʱ��[ת��Ϊ�ַ���]
	//Option's informations
public:
	CString m_strStCode;        //����SCSW008��Լ
	int     m_centercode;       //ˮ����ң��ϵͳ��Լ
	int m_strSYQADDR;		//ˮ����ң��ϵͳ��Լ
	int     m_iPassword;    //ˮ����ң��վ����
	WORD  m_AddrA1;				//����������A1 (0-9999)
	BYTE  m_ProvinceCode;		//ʡ�� 0 - 99
	BYTE  m_RegionCode;			//���� 0 - 99
	BYTE  m_CountyCode;			//���� 0 - 99

	DWORD m_AddrA2;				//�ն˻��м̵�ַ A2 
	BYTE  m_AddrA3;				//���Ʊ��� A3(2,5)
	BYTE m_CmType;				//ͨ�ŷ�ʽ
	BYTE m_Interface;			//�ӿڷ�ʽ
	BYTE m_ProtocolType;		//����Э�� UDP �� TCP
	CString m_ComPortA;			//�˿ں�
	int m_BaundA;				//������
	BYTE m_DataBitsA;			//����λ
	BYTE m_StopBitsA;			//ֹͣλ
	BYTE m_ParityA;				//��żУ�鷽ʽ
	CString m_IpAddrA;			//Ip��ַ
	unsigned short m_SockPortA;	//Socket�˿ں�

	//Add 2012-4-24 ��չ
	BYTE   m_nAppProVer;		//Э��汾 1.0��Old  2.0:��֦����Ŀ�޸ĺ�Э��
	UINT32 m_nScAddress;		//����ͨ�ŵ�ַ
	int   m_nScFactoyType;		//����ͨ���豸����

	bool  m_bSetTimePerDay;     //�Ƿ��ң���ն˻�������ʱ�䣬ÿ���ʱһ�� Ĭ�� false
	bool  m_bSetTimeEnable;        //�Ƿ��ܹ�����ʱ�䣬Ĭ��:false

	int   m_nAppProtoType;         //Э������ SCSW008=3
	int   m_nRecvMsgCot;           //���յ����ĵĴ���ԭ��
	int   m_nWriteUpdatePackCnt;   //��д������
	CWriteFrame m_pSendWriteFrame;
protected:
    BYTE m_bCommState ;			//ͨ��״̬��Ϣ  0 ��ͨ 1 ���� 2 δ֪
	CDatabase* m_pDatabase;		//���ݿ����� =CRealTimeDB.DB
	SYSTEMTIME m_LastRecvTime;	//�����ձ��ĵ�ʱ��
	BYTE m_RtuWorkMode;         //ң���ն˹���ģʽ 00B ���ݹ���ģʽ,01B �Ա�����ģʽ,02B ��ѯ/Ӧ����ģʽ,03B ����ά������ģʽ,Ĭ�ϣ�01B �Ա�ģʽ
	BYTE m_RtuLinkCheckState;   //��·���״̬��F0H����¼,F1H���˳���¼,F2H�����߱���,Ĭ�ϣ�F0
	CMutex m_Modify_Mutex;		//�޸����ԵĻ����� ��Ҫ����ٽ�����
public:
	bool SetCommState(BYTE newState);														//����ͨ��״̬
	const BYTE GetCommState()					{ return m_bCommState;}						//��ȡͨ��״̬ 
	void SetWorkMode(BYTE workMode)				{m_RtuWorkMode = workMode;}					//����ң���ն˵Ĺ���ģʽ
	const BYTE GetWorkMode()					{return m_RtuWorkMode;}						//��ȡң���ն˵Ĺ���ģʽ
	void SetLinkCheckState(BYTE LinkState)		{m_RtuLinkCheckState = LinkState;}			//������·���״̬
	const BYTE GetLinkCheckState()				{return m_RtuLinkCheckState;}				//��ȡ��·���״̬
	void SetProtocol(int d)                     {m_nAppProtoType = d;}

	//2012-05-08 �Զ���ʱ
	bool IsSetTimePerDay()						{return m_bSetTimePerDay;}
	void SetTimePerDay(bool _value)				{if (m_bSetTimePerDay != _value) m_bSetTimePerDay = _value;}

	void SetTimeEnable(bool _value)				{if (m_bSetTimeEnable != _value) m_bSetTimeEnable = _value;}
	bool IsSetTimeEnable()						{return m_bSetTimeEnable;}

public:
	const CString GetStringStCode()             {return m_strStCode;}
	const int GetSYQStCode()                    {return m_strSYQADDR;}
	const int GetSYQCenterCode()                {return m_centercode;}
	const int GetSYQPassword()                  {return m_iPassword;}
	const WORD GetAddrA1()						{return m_AddrA1;}
	const DWORD GetAddrA2()						{return m_AddrA2;}
	const BYTE GetAddrA3()						{return m_AddrA3;} 
	const BYTE GetCmType()						{return m_CmType;}
	const BYTE GetInterface()					{return m_Interface;}
	const BYTE GetProtocolType()				{return m_ProtocolType;}
	const CString GetComPortA()					{return m_ComPortA;}
	const int GetBaundA()						{return m_BaundA;}
	const BYTE GetDataBitsA()					{return m_DataBitsA;}
	const BYTE GetStopBitsA()					{return m_StopBitsA;}
	const BYTE GetParityA()						{return m_ParityA;}
	const CString GetIpAddrA()					{return m_IpAddrA;}
	const USHORT  GetSockPortA()				{return m_SockPortA;}
	const BYTE GetProvinceCode()				{return m_ProvinceCode;}
	const BYTE GetRegionCode()					{return m_RegionCode;}
	const BYTE GetCountyCode()					{return m_CountyCode;}

	const UINT32 GetScAddress()					{return m_nScAddress;}
	const BYTE GetAppProVer()					{return m_nAppProVer;}
	const int GetAppProtocolType()             {return m_nAppProtoType;}
	const int  GetScFactoryType()				{return m_nScFactoyType;}


    //����RTU��IP��ַ�Ͷ˿ں�,ͨ�Žӿڷ�ʽ
	void   SetNetInfo(PNET_INFO pNetInfo);
	void   SetIpAddrA(CString newIP)			{if (m_IpAddrA != newIP) m_IpAddrA = newIP;}
	void   SetSockPortA(USHORT newSockPort)		{if (m_SockPortA != newSockPort) m_SockPortA = newSockPort;}
	void   SetCmType(BYTE newCmType)			{if (m_CmType != newCmType) m_CmType = newCmType;}
	void   SetInterface(BYTE newInterface)		{if (m_Interface != newInterface) m_Interface = newInterface;}
	void   SetProtocolType(BYTE newProtocol)	{if (m_ProtocolType != newProtocol) m_ProtocolType = newProtocol;}

	//���´���������Ϣ
	void   SetComInfo(PCOM_INFO pComInfo);
	void   SetComPortA(CString newPortA)		{if (m_ComPortA != newPortA) m_ComPortA = newPortA;}
	void   SetBaundA(int newBaundA)				{if (m_BaundA != newBaundA) m_BaundA = newBaundA;}
	void   SetDataBitsA(BYTE newdataBit)		{if (m_DataBitsA != newdataBit) m_DataBitsA = newdataBit;}
	void   SetStopBitsA(BYTE newStopBit)		{if (m_StopBitsA != newStopBit) m_StopBitsA = newStopBit;}
	void   SetParityA(BYTE newParity)			{if (m_ParityA != newParity) m_ParityA = newParity;}

	void  SetScCommInfo(PSC_COMM_INFO pInfo);
	void  SetScAddress(UINT32 scAddr)			{if (m_nScAddress != scAddr) m_nScAddress = scAddr;}
	void  SetAppProVer(BYTE proVer)				{if (m_nAppProVer != proVer) m_nAppProVer = proVer;}
	void  SetScFactoryType(int scfactoryType)	{if (m_nScFactoyType != scfactoryType) m_nScFactoyType = scfactoryType;}
public:
	//����RealDB�ڴ��е�������Ϣ
	bool UpdateRealDBYc(CString ycID,DB_RECORD_OPERATION_TYPE opType);
	bool UpdateRealDBYx(CString yxID,DB_RECORD_OPERATION_TYPE opType);
	bool UpdateRealDBYk(CString ykID,DB_RECORD_OPERATION_TYPE opType);
	bool UpdateRealDBZd(CString zdID,DB_RECORD_OPERATION_TYPE opType);
	bool UpdateConfigInfo(PRTU_STATION pInfo);									//����RTUվ�Ļ���������Ϣ
private:
	//ɾ��RealDB�е��ڴ����
	bool DeleteRealDBYc(CString ycID);
	bool DeleteRealDBYx(CString yxID);
	bool DeleteRealDBYk(CString ykID);
	bool DeleteRealDBZd(CString zdID);

	//�����ݿ��л�ȡ�������޸ĺ�Ķ�����Ϣ
	CRtuYcObject* QueryYcInDB(CString ycID);
	CRtuYxObject* QueryYxInDB(CString yxID);
	CRtuYkObject* QueryYKInDB(CString ykID);
	CRtuZdObject* QueryZdInDB(CString zdID);

	//���ڴ���������Ӷ�����Ϣ
	bool AddRealDBYc(CRtuYcObject* pYc);
	bool AddRealDBYx(CRtuYxObject* pYx);
	bool AddRealDBYk(CRtuYkObject* pYk);
	bool AddRealDBZd(CRtuZdObject* pZd);

	void Init();
private:
	//���һ����־��Ϣ����־ϵͳ����⣩
	BOOL AddLogMessage(_LOG_MESSAGE_TYPE _logType,
		                CString _LogMessage,
						_Log_level_type _levelType = Normal_Log_Message,
						PSYSTEMTIME pLogTime = NULL);

	//���һ��ͨ��״̬�ı����־��Ϣ����־ϵͳ����⣩
	BOOL AddCmStateChangeLog();
public:
	//���һ��ң�ز�������־��Ϣ
	BOOL AddYkOperateLog(CString _LogMessage,
		                 _Log_level_type _levelType = Normal_Log_Message,
						 PSYSTEMTIME pLogTime = NULL);

	const CString Get_Log_RtuInfo();									//������־ʱ����( Rtu��������Ϣ)
protected:
	CHandle_COM* m_pHandle_Com;											//����ͨ�Žӿ�
	CCriticalSection m_cs_HandleCom;

	CHandle_NET* m_pHandle_Net;											//UDPͨ�Žӿ�
	CCriticalSection m_cs_HandelNet;

	CHandle_TcpClient* m_pHandle_TcpClient;								//TCPͨ�Žӿ�
	CCriticalSection m_cs_HandleTcpClient;

	CSCMessageDealModule* m_pScMessageDealModule;						//����ͨ�Ŵ���ӿ�
	CCriticalSection m_cs_ScMessageDealModule;

	CSysDataTrans m_SysDataTrans;										// ���Ĵ������
	CSysDataTrans_Hydr m_SysDataTrans_Hydr;                             // ˮ�ı��Ĵ������
	CSysDataTrans_WriteUpdate m_SysDataTrans_WriteUpdate;               // ��д��������
	CSysDataTrans_Syq m_SysDataTrans_Syq;                               //ˮ���鱨�Ĵ������


public:
	void SetHandle_Com(CHandle_COM* pHandleCom);
	void SetHandle_Net(CHandle_NET* pHandleNet);
	void SetHandle_TcpClient(CHandle_TcpClient* pHandleTcpClient);
	void SetScMessageDealModule(CSCMessageDealModule* pScMessageModule);
	CSCMessageDealModule* GetScMessageDealModule();
	bool SendData(BYTE* buffer,WORD len);								//����·�����·�����
	CSysDataTrans* GetSysDataTrans();									//���Ĵ������
	CSysDataTrans_Hydr*  GetSysDataTrans_Hydr();                         //ˮ�ı��Ĵ������
	CSysDataTrans_WriteUpdate* GetSysDataTrans_WriteUpdate();
	CSysDataTrans_Syq* GetSysDataTrans_Syq();                           //ˮ���鱨�Ĵ������
	
	bool blEnbaleTrans();												//�Ƿ�����·����ĵ��Ⱦ�����
	void ClearCommHandle();												//������е�ͨ�Žӿڹ�����������Ϣ
protected:
	CList<Log_Item*> m_lstData;											//�洢���ݣ��Ա�������������ҳ������ʾʹ�ã�--�洢�ڻ�����
	CList<Log_Item*> m_lstLogs;											//�洢��־��Ϣ����������ҳ����־��Ϣ��--�洢�ڻ�����
	CList<Log_Item*> m_lstComms;										//ͨ����־��Ϣ��ң���ն�ͨ������ı仯������������Max_log_Count
public:
	void InsertDataMessage(CString DataMsg,PSYSTEMTIME pTime = NULL);   //���������Ϣ
	void InsertDataMessage(Log_Item* pData);							//���������Ϣ
	CList<Log_Item*>* GetDataList();
	void ClearDataList();

	void InsertLogMessage(CString logMessage,
		 _Log_level_type xLevel = Normal_Log_Message,
		 PSYSTEMTIME pTime = NULL);										//���������־��Ϣ
	void InsertLogMessage(Log_Item* pLog);								//���������־��Ϣ
	CList<Log_Item*>*  GetLogList();
	void ClearLogList();												//���Log����


	void InsertCommMessage(CString CommLog,
		_Log_level_type xLevel = Normal_Log_Message,
		PSYSTEMTIME pTime = NULL);
	void InsertCommMessage(Log_Item* LogItem);
	CList<Log_Item*>* GetCommList();
	void ClearCommList();
public:
	void              CheckRepeatType(CPacketFrameFormat* pPacket);		//��鱨���Ƿ�Ϊ�ظ����ģ���Ϊ�Ѿ����յı��ģ�������
protected:
	CPacketFrameFormat m_lastPacket;									//�����յ�packet(����)
	void               UpdateLastPacket(CPacketFrameFormat* pPacket);	//���������յı�����Ϣ
};


