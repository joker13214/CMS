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
#include "RtuPropertyDlg.h"
#include "TmMessageDealModule.h"
#include <vector>
#include <list>

#include "Handle_COM.h"
#include "Handle_NET.h"
#include "Handle_TcpClient.h"

//遥测终端自报参数信息
class CRTU_AUTOREPORT_PRA
{
public:
	CRTU_AUTOREPORT_PRA()
	{
		m_wAutoTotal = 0;
		m_wAutoInterval = 0;
	}
	~CRTU_AUTOREPORT_PRA(){}
	bool		GetAutoBitValue(size_t _pos);										//获取bit位值，_pos(0~n bit),true:表示该bit位为1,false:表示该bit位为0
	const WORD	SetAutoBitValue(size_t _pos,bool _val = true);
	void		SetAutoValue(WORD TotalValue)		{m_wAutoTotal = TotalValue;}    //设置自动上报参数
	const WORD	GetAutoValue()						{return m_wAutoTotal;}			//获取自动上报参数
	void		SetAutoInterval(WORD interval)		{m_wAutoInterval = interval;}	//设置自动上报时间间隔，单位min
	const WORD  GetAutoInterval()					{return m_wAutoInterval;}		//获取自动上报时间间隔，单位min
protected:
	WORD m_wAutoTotal;		//自报参数
	WORD m_wAutoInterval;	//自报时间间隔，单位：min(分钟)
};

//遥测站需查询的实时数据种类参数
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

//清空遥测终端的历史数据单元参数
class CClearHistoryData_PARA
{
public:
	CClearHistoryData_PARA()
	{
		m_bClearHis_BYTE = 0;
	}
	const BYTE GetClearHistoryPARA() {return m_bClearHis_BYTE;}
	void       SetClearHistoryPARA(BYTE _value) {m_bClearHis_BYTE = _value;}
	bool       GetBitValue(size_t _pos);                        //获取某一位的值，从0-7
    void       SetBitValue(size_t _pos,bool _val = true);
protected:
	BYTE  m_bClearHis_BYTE;  //是否清空的标志字节，每一位是否为1来确定是否清空某一项
};

//遥测终端状态  1Bytes
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
	BYTE m_TerminalStateByte;        //遥测终端状态Byte, 1个字节，HEX
};

//遥测终端的报警状态 1Byte
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
	BYTE m_AlarmStateByte;           //遥测终端报警状态Byte, 1个字节，HEX
};

//遥测终端扩展报警状态 1Byte
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
	BYTE m_ExAlarmStateByte;       //遥测终端扩展报警状态Byte值，1个字节，HEX
};

//水泵电机的实时工作数据
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
	WORD m_VoltageA;						//A相电压
	WORD m_VoltageB;						//B相电压
	WORD m_VoltageC;						//C相电压

	WORD m_ECurrentA;						//A相电流
	WORD m_ECurrentB;						//B相电流
	WORD m_ECurrentC;						//C相电流
};

//遥控启动/关闭对应水泵或闸门/阀门的配置信息
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
	BYTE m_PumpGate_Code;                  //水泵/闸门编码号 D0-D3 0-15
	BYTE m_PumpGete_Type;                  //类型 D4-D7   0000（0） 水泵， 1111（15） 阀门/闸门

	BYTE m_YK_Byte;                        //遥控字节 前面两项
};

//遥测站雨量数据报警值参数，
//单个雨量点
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
	double m_fRainAlertValue;     //单位时间内的雨量报警值  2个字节，BCD码 0-999.9
	WORD  m_wPerTime;            //单位时间，0-9999，2个字节，BCD码
	double m_RealTimeValue;       //雨量实时值0-999.9  单位：mm/h  
	BYTE  m_DataType;			 //自报时数据类型,自报时有用
	double m_TotalValue;			 //总雨量值 0-9999999.9 单位:mm
	double m_HalfhValue;			 //半小时雨量累计值 0-999.9 单位:mm
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

//遥测站模拟量数据上下参数，单个模拟量PER
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
	void SetAnalogUpLimit(DWORD _upLimit)	{m_Analog_UpLimit = _upLimit;}			//设置模拟量上限值
	void SetAnalogDownLimit(DWORD _downLimit){m_Analog_DownLimit = _downLimit;}		//设置模拟量下限值
	void SetRealTimeValue(DWORD _realValue)	 {m_RealTimeValue = _realValue;}
	const DWORD GetAnalogUpLimit() {return m_Analog_UpLimit;}						//获取模拟量上限值
	const DWORD GetAnalogDownLimit(){return m_Analog_DownLimit;}					//获取模拟量下限值
	const DWORD GetRealTimeValue()	{return m_RealTimeValue;}
protected:
	DWORD m_Analog_UpLimit;		//模拟量上限 3个字节，BCD,0-99999
	DWORD m_Analog_DownLimit;	//模拟量下限 3个字节，BCD,0-99999
	DWORD m_RealTimeValue;      //模拟量实时查询值  0-99999
};



//遥测站位移数据上下参数，单个深度位移量PER单位：mm(毫米)
class CDisplace_PARA_PER
{
public:
	CDisplace_PARA_PER()
	{
		m_Displace_XUpLimit = 0;
		m_Displace_XDownLimit =  0;
		m_RealTimeXValue = 0;
		m_Displace_YUpLimit = 0;
		m_Displace_YDownLimit =  0;
		m_RealTimeYValue = 0;
		m_Displace_No = 0;
	}
	CDisplace_PARA_PER(CDisplace_PARA_PER* pSource);
public:
	void SetDisplaceXUpLimit(double _xupLimit)	{m_Displace_XUpLimit = _xupLimit;}			//设置深度位移量X轴上限值
	void SetDisplaceYUpLimit(double _yupLimit)	{m_Displace_YUpLimit = _yupLimit;}			//设置深度位移量Y轴上限值
	void SetDisplaceXDownLimit(double _xdownLimit){m_Displace_XDownLimit = _xdownLimit;}		//设置深度位移量下限值
	void SetDisplaceYDownLimit(double _ydownLimit){m_Displace_YDownLimit = _ydownLimit;}		//设置深度位移量下限值
	void SetRealTimeXValue(double _xrealValue)	 {m_RealTimeXValue = _xrealValue;}          //设置深度位移量X轴实时值
	void SetRealTimeYValue(double _yrealValue)	 {m_RealTimeYValue = _yrealValue;}          //设置深度位移量Y轴实时值
	void SetDisplaceNo(int _Displace_No)	 {m_Displace_No = _Displace_No;}          //设置深度位移量Y轴实时值
	int GetDisplaceNo()	{return m_Displace_No;}								//获取深度位移量X轴实时值
	const double GetDisplaceXUpLimit() {return m_Displace_XUpLimit;}							//获取深度位移量X轴上限值
	const double GetDisplaceXDownLimit(){return m_Displace_XDownLimit;}						//获取深度位移量X轴下限值
	const double GetRealTimeXValue()	{return m_RealTimeXValue;}								//获取深度位移量X轴实时值
	const double GetDisplaceYUpLimit() {return m_Displace_YUpLimit;}							//获取深度位移量X轴上限值
	const double GetDisplaceYDownLimit(){return m_Displace_YDownLimit;}						//获取深度位移量X轴下限值
	const double GetRealTimeYValue()	{return m_RealTimeYValue;}								//获取深度位移量X轴实时值
	
protected:
	double m_Displace_XUpLimit;			//深度位移量X轴上限 3个字节，BCD码,0-999999 
	double m_Displace_YUpLimit;			//深度位移量Y轴上限 3个字节，BCD码,0-999999
	double m_Displace_XDownLimit;		//深度位移量X轴下限 3个字节，BCD,0-999999
	double m_Displace_YDownLimit;		//深度位移量Y轴下限 3个字节，BCD,0-999999
	double m_RealTimeXValue;				//深度位移量X轴实时查询值  0-999999
	double m_RealTimeYValue;				//深度位移量传感器序号
	int m_Displace_No;
};

//遥测站温度数据上下参数，单个温度PER
class CTempera_PARA_PER
{
public:
	CTempera_PARA_PER()
	{
		m_Tempera_DownLimit = 0;
		m_Tempera_UpLimit =  0;
		m_RealTimeValue = 0;
	}
	CTempera_PARA_PER(CTempera_PARA_PER* pSource);
public:
	void SetTemperaUpLimit(float _upLimit)	{m_Tempera_UpLimit = _upLimit;}			//设置温度上限值
	void SetTemperaDownLimit(float _downLimit){m_Tempera_DownLimit = _downLimit;}		//设置温度下限值
	void SetRealTimeValue(float _realValue)	 {m_RealTimeValue = _realValue;}
	const float GetTemperaUpLimit() {return m_Tempera_UpLimit;}						//获取温度上限值
	const float GetTemperaDownLimit(){return m_Tempera_DownLimit;}					//获取温度下限值
	const float GetRealTimeValue()	{return m_RealTimeValue;}
protected:
	float m_Tempera_UpLimit;		//温度上限 2个字节，BCD,0-999.9
	float m_Tempera_DownLimit;	//温度下限 2个字节，BCD,0-999.9
	float m_RealTimeValue;      //温度实时查询值  0-999.9
};


//遥测终端水量相关参数信息 
//包含:充值量、剩余水量,剩余水量报警值
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
	DWORD  m_lReChargeValume;		 //充值量，0-99999999 4Bytes BCD      单位：立方米
	UINT64 m_lRemainValume;          //剩余水量,0-7999999999 5bytes，BCD，最高位为符号位，0为正 1为负值 单位:立方米
	DWORD  m_lRemainAlertValume;	 //剩余水量报警值
	BYTE   m_bSignBit;               //符号位 1:为负值，0为正值
};

//水位参数，单个水位点的参数
//包含:水位基值、上下限值
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
	float m_fBaseValue;     //水位基值,  0-9999.99 3Bytes,BCD，单位:m
	float m_fUpValue;       //水位上限值,0-99.99,  2Bytes,BCD，单位:m
	float m_fDownValue;     //水位上限值,0-99.99,  2Bytes,BCD，单位:m
	float m_fRealValue;		//水位实时值,0-9999.99, 3Bytes,BCD，单位:m
};

//水压参数,单个水压点的参数
//包括:水压上限值、水压下限值
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
	double m_dUpValue;			//水压上限值,0-999999.99 4Bytes,BCD,单位;kPa
	double m_dDownValue;		//水压下限值,0-999999.99 4Bytes,BCD,单位;kPa	
	double m_dRealValue;	    //水压实时值,0-999999.99 4Bytes,BCD,单位:kPa,查询实时值用
};


static WATER_QU_STRUCT water_q_ID[]={
	{"水温",			0,99.9,0,		3,1,"℃"},
	{"pH值",			1,99.99,0,		4,2,""},
	{"溶解氧",			2,999.9,0,		4,1,"mg/L"},
	{"高锰酸盐指数",	3,999.9,0,		4,1,"mg/L"},
	{"电导率",			4,99999,0,		5,0,"uS/cm"},
	{"氧化还原电位",	5,9999.9,0,		5,1,"mV"},
	{"浊度",			6,999,0,		3,0,"度"},
	{"化学需氧量",		7,999999.9,0,	7,1,"mg/L"},
	{"五日生化需氧量",	8,9999.9,0,		5,1,"mg/L"},
	{"氨氮",			9,9999.99,0,	6,2,"mg/L"},
	{"总氮",			10,999.99,0,	5,2,"mg/L"},
	{"铜",				11,999.9999,0,	7,4,"mg/L"},
	{"锌",				12,99.9999,0,	6,4,"mg/L"},
	{"氟化物",			13,999.99,0,	5,2,"mg/L"},
	{"硒",				14,99.99999,0,	7,5,"mg/L"},
	{"砷",				15,99.99999,0,	7,5,"mg/L"},
	{"汞",				16,99.99999,0,	7,5,"mg/L"},
	{"镉",				17,99.99999,0,	7,5,"mg/L"},
	{"六价铬",			18,99.999,0,	5,3,"mg/L"},
	{"铅",				19,99.99999,0,	7,5,"mg/L"},
	{"氰化物",			20,99.999,0,	5,3,"mg/L"},
	{"挥发酚",			21,99.999,0,	5,3,"mg/L"},
	{"苯酚",			22,999.99,0,	5,2,"mg/L"},
	{"硫化物",			23,99.999,0,	5,3,"mg/L"},
	{"粪大肠菌群",		24,9999999999,0,10,0,"个/L"},
	{"硫酸盐",			25,9999.99,0,	6,2,"mg/L"},
	{"氯化物",			26,999999.99,0, 8,2,"mg/L"},
	{"硝酸盐氮",		27,999.99,0,	5,2,"mg/L"},
	{"铁",				28,99.99,0,		4,2,"mg/L"},
	{"锰",				29,99.99,0,		4,2,"mg/L"},
	{"石油类",			30,99.99,0,		4,2,"mg/L"},
	{"阴离子表面活性剂",31,99.99,0,		4,2,"mg/L"},
	{"六六六",			32,9.999999,0,	7,6,"mg/L"},
	{"滴滴涕",			33,9.999999,0,	7,6,"mg/L"},
	{"有机氯农药",		34,9.999999,0,	7,6,"mg/L"}
};

//单个水质的参数信息
//包含bit 0-39
//值的范围：最大值、最新值
//值的上下限值： up,max
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
	DOUBLE m_ScaleMax;				//最大值
	DOUBLE m_ScaleMin;				//最小值
	BYTE   m_BitPos;				//bit位位置 0 - 39
	BYTE   m_DataBits;              //数据位总数，不含小数点 0-10
	BYTE   m_PointBits;				//数据小数位数
	BYTE   m_BUF_Cnt;               //本项水质参数的报文字节数 4/5

	DOUBLE m_UpValume;				//上限值
	DOUBLE m_DownValue;				//下限值
	//bool   m_bCheck_FLAG;			//是否监测的标志 0 ：不监测， 1:监测
	DOUBLE m_CurValue;              //水质参数实时值
	CString m_strName;              //水质参数的名称
	CString m_strUnit;              //单位
};

//水质参数
//包括：水质参数种类、及是否监测或不检查
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
	int Get_Buffer_Count();     //获取水质参数报文的长度， 5 + 4*N(+1)
protected:
	UINT64 m_wQualityValue;      //5个字节的水质参数标志位 40bits
	std::map<BYTE,CWaterQuality_PARA_PER*> m_mapWaterQulity;

};

//流量的表底（初始）值参数，当流量表的PER
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
	void SetFlowInitValume(DWORD _FlowInitV) {m_dwFlowInitValume = _FlowInitV;}  //设置流量表底的初始值
	void SetCalTakeValume(DWORD CalValume)   {m_dwCalTakeValume = CalValume;}	 //设置累计取水量
	void SetInsFlowValume(DWORD InsFlowV)	 {m_dwInsFlowValume = InsFlowV;}	 //设置瞬时流量
	const DWORD GetFlowInitValume()	{return m_dwFlowInitValume;}				 //获取流量表底的初始值
	const DWORD GetCalTakeValume()	{return m_dwCalTakeValume;}					 //获取累计取水量
	const DWORD GetInsFlowValume()	{return m_dwInsFlowValume;}					 //获取瞬时流量
protected:
	DWORD m_dwFlowInitValume;     //流量表底的初始值，0-99999999，4 Bytes, BCD，单位：立方米
	DWORD m_dwCalTakeValume;      //累计取水量 单位：立方米，0-9999999，4Bytes,BCD
	DWORD m_dwInsFlowValume;      //瞬时流量 单位:立方米/h 0-9999,2bytes,BCD
};

class TM_Unit
{
public:
	void SetZero(){
		m_phoneNum = "";
		m_recvTime = "";
		m_Content = "";
		m_len = 0;
		m_index = 0;
		m_bRead = false;
		m_isSendMsgType = true;
		m_state = 0;
		m_Row = -1;
		memset(m_buf, 0, 1024);
	}

	inline TM_Unit(){SetZero();}
	inline ~TM_Unit(){}

	
	void SetPhoneNum(CString str){m_phoneNum = str;}
	void SetRecvTime(CString str){m_recvTime = str;}
	void SetContent(CString str){m_Content = str;}
	void SetBufLen(int d){m_len = d;}
	void SetIndex(int d){m_index = d;}
	void SetRead(bool b){m_bRead = b;}
	void SetSendMsgType(bool d){m_isSendMsgType = d;}
	void SetState(int d){m_state = d;}
	void SetRow(int d){m_Row = d;}

	CString& GetPhoneNum(){return m_phoneNum;}
	CString& GetRecvTime(){return m_recvTime;}
	CString& GetContent(){return m_Content;}
	int      GetBufLen(){return m_len;}
	int      GetIndex(){return m_index;}
	bool     isRead(){return m_bRead;}
	bool     isSendMsgType(){return m_isSendMsgType;}
	int     GetState(){return m_state;}
	int      GetRow(){return m_Row;}

public:
	BYTE m_buf[1024];
private:
	CString m_phoneNum;
	CString m_recvTime;
	CString m_Content;
	int  m_len;
	int  m_index;
	bool m_bRead;
	bool m_isSendMsgType;//是发送类的消息？
	int  m_state;
	int  m_Row;
	
};


static CString  _EventRecordName[] ={
	"数据初始化记录",
	"参数变更记录",
	"状态量变位记录",
	"仪表故障记录",
	"密码错误记录",
	"终端故障记录",
	"交流失电记录",
	"蓄电池电压低告警记录",
	"终端箱非法打开记录",
	"水泵工作记录",
	"剩余水量越限告警记录",
	"水位超限告警记录",
	"水压超限告警记录",
	"水质参数超限告警记录",
	"数据出错记录",
	"发报文记录",
	"收报文记录",
	"雨量参数超限告警记录",
	"模拟量参数超限告警记录"
};

//事件记录对象
class CEventRecord_PER
{
public:
	CEventRecord_PER()
	{
		strName = "";
		bPos = 0;
		wEventCount = 0;
	}
	CString strName;	 //名称
	BYTE    bPos ;		 //1-32
	WORD    wEventCount; //事件记录数
};

//电度参数（per）
class CPulsePara_PER
{
private:
	CString    m_strCodeString;
	UINT64  m_unCode;    //电度表地址码 BCD 0-999999999999 6 字节
	double  m_dbValue1;	 //正向有功     BCD 0-999999.99	   4 字节
	double  m_dbValue2;	 //正向无功     BCD 0-999999.99	   4 字节
	double  m_dbValue3;	 //反向有功		BCD 0-999999.99	   4 字节
	double	m_dbValue4;	 //反向无功		BCD 0-999999.99	   4 字节
public:
	CPulsePara_PER(UINT64 Code = 0,
		           double value1 = 0,
				   double value2 = 0,
				   double value3 = 0,
				   double value4= 0,
				   CString strCode = "");
	CPulsePara_PER(CPulsePara_PER* pSource);
public:
	const UINT64 GetCode();   //电度表地址码
	const double GetValue1(); //正向有功
	const double GetValue2(); //正向无功 
	const double GetValue3(); //反向有功
	const double GetValue4(); //反向无功
	const CString GetCodeString();

	void SetCode(UINT64 code);		//电度表地址码 BCD 0-999999999999 6 字节
	void SetValue1(double value1);	//正向有功     BCD 0-999999.99	   4 字节
	void SetValue2(double value2);	//正向无功     BCD 0-999999.99	   4 字节
	void SetValue3(double value3);	//反向有功		BCD 0-999999.99	   4 字节
	void SetValue4(double value4);	//反向无功		BCD 0-999999.99	   4 字节
	void SetCodeString(CString strCode);
};

//遥测终端历史日记录
class CDayHistoryData
{
public:
	CDayHistoryData(){ClearData();}
protected:
	BYTE mZBType;		//整、半点标准 2整点、1半点
	BYTE mQueryNo;		//查询参数的编号 0-7
	BYTE mDataType;		//数据类型：水位、水压、水质...
	WORD mYear;			//年
	WORD mMonth;		//月
	WORD mDay;			//日

	BYTE mPointBits;	//小数位数
	CString sUnit;		//单位

	SYSTEMTIME mMaxTime; //最大值时间
	SYSTEMTIME mMinTime; //最小值时间
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

#ifndef  _rtu_attrib_type_const_
#define  _rtu_attrib_type_const_
	const int rtu_attr_Connect_check	= 0x01;				//链路检查
	const int rtu_attr_workmode_call	= 0x02;				//工作模式召
	const int rtu_attr_workmode_set		= 0x03;				//工作模式设置
	const int rtu_attr_ycquerypara_call = 0x04;				//遥测查询参数查询
	const int rtu_attr_ycquerypara_set  = 0x05;				//遥信查询参数设置
	const int rtu_attr_autoRpPara_call	= 0x06;				//自报参数查询
	const int rtu_attr_atuoRpPara_set	= 0x07;				//自报参数设置
	const int rtu_attr_datetime_call	= 0x08;				//查询遥测终端或中继站时钟
	const int rtu_attr_datetime_set     = 0x09;				//设置遥测终端或中继站时钟
	const int rtu_attr_rainalert_call   = 0x0A;				//雨量数据报警值查询
	const int rtu_attr_rainalert_set    = 0x0B;				//雨量数据报警值设置
	const int rtu_attr_analogminmax_call= 0x0C;				//模拟量上下限值查询
	const int rtu_attr_analogminmax_set = 0x0D;				//模拟量上下限值设置
	const int rtu_attr_water_recharge_call = 0x0E;			//水量参数查询
	const int rtu_attr_water_alert_call	= 0x0F;				//水量参数设置
	//add by yhp 20160614
	const int rtu_attr_displaceminmax_call= 0x10;				//深度位移量上下限值查询
	const int rtu_attr_displaceminmax_set = 0x11;				//深度位移量上下限值设置
	const int rtu_attr_temperaminmax_call= 0x12;				//温度上下限值查询
	const int rtu_attr_temperaminmax_set = 0x13;				//温度上下限值设置
	const int rtu_attr_displace_data_call = 0x30;				//深度位移量实时数据查询
	const int rtu_attr_tempera_data_call = 0x31;				//温度实时数据查询	
	//end add by yhp 20160614
	const int scsw_no_call_operation = 0x100;//当前无查询操作
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
	const int writeUpdate_B4 = 0x147;
	const int writeUpdate_B5 = 0x148;
	//end add by yhp 20160801
	const int rtu_attr_waterlevel_call	= 0x20;				//水位参数设置查询
	const int rtu_attr_waterlevel_data_call   = 0x21;		//水位数据实时查询
	const int rtu_attr_waterpress_call  = 0x22;				//水压参数查询
	const int rtu_attr_waterpress_data_call   = 0x23;		//水压实时数据查询
	const int rtu_attr_waterquality_up_call   = 0x24;       //水质参数上限值查询
	const int rtu_attr_waterquality_down_call = 0x25;       //水质参数下限值查询
	const int rtu_attr_waterquality_data_call = 0x26;       //水质参数实时值查询
	const int rtu_attr_alarmstate_call        = 0x27;       //遥测终端报警状态和遥测终端状态查询
	const int rtu_attr_exalarmstate_call      = 0x28;       //遥测终端扩展报警状态和遥测终端状态查询
	const int rtu_attr_yk_operate             = 0x29;       //遥控操作
	const int rtu_attr_rain_data_call         = 0x2A;       //雨量实时数据查询 
	const int rtu_attr_analog_data_call       = 0x2B;       //模拟量实时数据查询
	const int rtu_attr_flowpara_set           = 0x2C;		//流量表底设置
	const int rtu_attr_flowdata_call		  = 0x2d;       //流量实时数据查询
	const int rtu_attr_eventrecord_call       = 0x2e;       //查询遥测终端的事件记录
	const int rtu_attr_pulsepara_call		  = 0x2f;		//查询电度实时数据
#endif


//遥控操作结果常数
#ifndef _YK_RESULT_TYPE_CONST＿                       
#define _YK_RESULT_TYPE_CONST＿
	const BYTE YKStart_PumpGate_SUC = 0x01;                //遥控启动水泵或闸门/阀门 成功
	const BYTE YKStart_PumpGate_ERR = 0x02;				   //遥控启动水泵或闸门/阀门 失败
	const BYTE YKClose_PumpGate_SUC = 0x03;				   //遥控关闭水泵或闸门/阀门 成功
	const BYTE YKClose_PumpGate_ERR = 0x04;				   //遥控关闭失败或闸门/阀门 失败
	const BYTE YKICCard_Enable_SUC  = 0x05;                //遥控IC卡功能有限 成功
	const BYTE YKICCard_Enable_ERR  = 0x06;				   //遥控IC卡功能有效 失败
	const BYTE YKICCard_DisEnable_SUC = 0x07;              //遥控取消IC卡功能 成功
	const BYTE YKICCard_DisEnable_ERR = 0x08;			   //遥控取消IC卡功能 失败
	const BYTE YKZD_Input_SUC = 0x09;					   //遥控定值功能投入 成功
	const BYTE YKZD_Input_ERR = 0x0A;					   //遥控定值功能投入 失败
	const BYTE YKZD_Exit_SUC  = 0x0B;					   //遥控定值功能退出 成功
	const BYTE YKZD_Exit_ERR  = 0x0C;                      //遥控定值功能退出 失败
	const BYTE YKReset_Rtu_SUC = 0x0D;					   //复位遥测终端操作 成功
	const BYTE YKReset_Rtu_ERR = 0x0E;					   //复位遥测终端操作 失败
	const BYTE YKClear_His_SUC = 0x0F;					   //清除遥测终端历史数据单元 成功
	const BYTE YKClear_his_ERR = 0x10;					   //清除遥测终端历史数据单元 失败
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

	//清空保存相关信息的链表
	void ClearYcList();
	void ClearYxList();
	void ClearYkList();
	void ClearZdList();
	void ClearRainAlartParaList();																		//清空雨量数据报警值的参数集合
	void ClearAnalogLimitParaList();																	//清空模拟量上下限参数集合
	void ClearDisplaceLimitParaList();																	//清空模拟量上下限参数集合
	void ClearTemperaLimitParaList();																	//清空温度上下限参数集合
	void ClearWaterLevelParaList();																		//清空水位点参数集合信息
	void ClearWaterPressureParaList();																	//清空水压点参数集合信息
	void ClearFlowInitValumeParaList();	//清空流量表表底参数集合信息
	void ClearTmUnitList();
	void ClearEventRecordsList();																		//清空事件记录列表集合信息，主要析购函数调用
public:
	void					SetParent(CRealTimeDB* _pParent);
	CRealTimeDB*			GetParent();

    bool BuildAllData(CDatabase* pDb);																	//读取所有的信息 yC Yx yK Zd
	bool BuildBasicPacket(CPacketFrameFormat& appFrame);//生成初始的下发帧报文帧数据信息
	bool BuildBasicPacket(CSmsFrameFormat& appFrame);
	CRTU_AUTOREPORT_PRA* GetAutoReportPara(){return &m_AutoReportPara;}									//获取自报参数信息
	CRTU_YCQUERYTYPE_PARA* GetYcQueryType(){return &m_YcQueryTypePara;}									//获取遥测站需要查询的实时数据种类信息参数对象
	const int GetReSendInterval(){return m_nReSendInterval;}											//获取重发报文的时间间隔，单位:秒(second)

	CList<CRainAlert_PARA_PER*>* GetRainAlertParasList() {return &m_lstRainAlertParas;}					//获取雨量数据报警值参数集合
	CRainAlert_PARA_PER* GetRainAlartPara(int Index);													//获取其中一项雨量数据报警值的参数,Index为索引号

	CList<CAnalogLimit_PARA_PER*>* GetAnalogLimitParasList(){return &m_lstAnalogLimitParas;}			//获取模拟量上下限参数集合
	CAnalogLimit_PARA_PER* GetAnalogLimitPara(int Index);												//获取其中一项模拟量上下限参数,Index为索引号

	CWaterValume_PARA*  GetWaterValumePara(){return &m_sWaterValumePara;}								//获取本RTU的水量参数信息

	//add by yhp 20160614
	CList<CDisplace_PARA_PER*>* GetDisplaceLimitParasList(){return &m_lstDisplaceLimitParas;}			//获取位移量上下限参数集合
	CDisplace_PARA_PER* GetDisplaceLimitPara(int Index);												//获取其中一项位移量上下限参数,Index为索引号

	CList<CTempera_PARA_PER*>* GetTemperaLimitParasList(){return &m_lstTemperaLimitParas;}			//获取温度量上下限参数集合
	CTempera_PARA_PER* GetTemperaLimitPara(int Index);												//获取其中一项温度量上下限参数,Index为索引号
	//end add
	//CWaterValume_PARA*  GetWaterValumePara(){return &m_sWaterValumePara;}								//获取本RTU的位移参数信息

	CList<CWaterLevel_PAR_PER*>* GetWaterLevelParasList(){return &m_lstWaterLevelParas;}				//获取水位点参数的集合
	CWaterLevel_PAR_PER* GetWaterLevelPara(int Index);													//获取其中一项水位点的参数,Index为索引号

	CList<CWaterPressure_PARA_PER*>* GetWaterPressureParasList(){return &m_lstWaterPressureParas;}		//获取水压点参数的集合
	CWaterPressure_PARA_PER* GetWaterPressurePara(int Index);											//获取其中一项水压点的参数对象，Index为索引号

	CList<CFlowInitValume_PARA_PER*>* GetFlowInitValumeParasList(){return &m_lstFlowInitValumeParas;}
	CFlowInitValume_PARA_PER* GetFlowInitValumePara(int Index);	//获取其中一项流量表表底的初始化值,Index为索引号


	std::list<TM_Unit *>* GetTmUnitList(){return &m_lstTmUnit;}
	TM_Unit* GetTmUnitFromMap(int idx);
	//电度相关
	std::map<UINT64,CPulsePara_PER*>* GetPulseParasMap(){return &m_mapPulseParas;}
	CPulsePara_PER*					  GetPulseParaObject(UINT64 code);
	void							  InsertPulseParaObject(CPulsePara_PER* pPulseObject);

	void Set_RESET_Cmd(BYTE _Cmd){m_bRESET_Cmd = _Cmd;}													//设置复位命令
	const BYTE Get_RESET_Cmd() {return m_bRESET_Cmd;}													//获取复位命令	
	CClearHistoryData_PARA* GetClearHistoryDataPara(){return &m_ClearHistoryDataPara;}					//获取清空遥测终端历史数据单元的参数信息
	CPumpGate_YK_PARA* GetCurPumpGateObjecte(){return &m_Cur_PumpGate_Object;}							//获取当前遥控操作的水泵或阀门/闸门对象
	CRTUAlarm_State*   GetRtuAlarm_State() {return &m_RTUAlarm_State;}									//获取遥测终端的报警状态信息对象
	CRTU_EXAlarm_State* GetRtuExAlarm_State(){return &m_RtuExAlarm_State;}								//获取遥测终端扩展报警状态对象
	CRTUTerminal_State* GetRtuTerminal_State(){return &m_RTUTerminal_State;}							//获取遥测终端的状态信息对象
	CWaterPump_WorkData* GetWaterPump_WorkData(){return &m_WaterPump_WorkData;}							//水泵电机实时工作数据信息对象

	CWaterQuality_PARA*  GetWaterQualityPara(){return &m_WaterQualityPara;}								//水质参数
	CWaterQuality_PARA_PER* GetOne_WaterQualityPara(BYTE _bitPos);                                      //获取其中一项水质参数
	const BYTE GetQueryRealDataCmd()	{return m_QueryRealData_Cmd;}									//查询遥测终端实时数据命令码 控制域的Cmd值
	void       SetQueryRealDataCmd(BYTE _qCmd){m_QueryRealData_Cmd = _qCmd;}							//设置遥测终端实时数据命令码 控制域的Cmd值
	void       Set_YK_Op_Result(BYTE _ykresult) {m_YK_OP_Result = _ykresult;}							//设置遥控操作的结果
	const BYTE Get_YK_Op_Result()				{return m_YK_OP_Result;}								//获取遥控操作的结果
	CDayHistoryData*	GetDayHistoryObject()	{return &m_DayHisoryData;}								//获取历史日记录对象

	CList<CEventRecord_PER*>* GetEventRecordsList(){return &m_lstEventRecords;}
	CEventRecord_PER*         GetEventRecordObject(int Index);											//Index 0-18
	CEventRecord_PER*         GetEventRecordByPos(int _pos);											//_pos = 1-19
	void					  BuildEventRecordsList();													//创建事件记录列表
	bool					  GetRecvFLAG(){return m_blRecvFLAG;}										//获取是否接收报的标志


protected:
	CRealTimeDB*		m_pParent;																		//RealTimeDB实例对象
	CRTU_AUTOREPORT_PRA m_AutoReportPara;																//自报参数信息
	CRTU_YCQUERYTYPE_PARA m_YcQueryTypePara;															//遥测站需要查询的实时数据种类信息参数
	int					m_nReSendInterval;																//重复时间间隔,单位:秒（second）  
	BYTE                m_bRESET_Cmd;																	//复位遥测终端的命令， HEX 01,复位，重启 02 复位 恢复出厂默认值，并重启
	CList<CRainAlert_PARA_PER*> m_lstRainAlertParas;													//雨量数据报警值参数集合
	CList<CAnalogLimit_PARA_PER*> m_lstAnalogLimitParas;												//模拟量上下限参数集合
	CList<CDisplace_PARA_PER*> m_lstDisplaceLimitParas;											//位移量上下限参数集合// add by yhp
	CList<CTempera_PARA_PER*> m_lstTemperaLimitParas;												//温度量上下限参数集合// add by yhp
	CWaterValume_PARA	m_sWaterValumePara;																//存储水量参数对象
	CList<CWaterLevel_PAR_PER*> m_lstWaterLevelParas;													//存储水位点参数信息的集合
	CList<CWaterPressure_PARA_PER*> m_lstWaterPressureParas;											//存储水压点参数（上下限）信息的集合
	CList<CFlowInitValume_PARA_PER*> m_lstFlowInitValumeParas;	//存储流量标底的初始化值集合
	std::list<TM_Unit *> m_lstTmUnit;//
	
public:
	CCriticalSection m_cs_TmUnit;
	
	
	CClearHistoryData_PARA m_ClearHistoryDataPara;														//存储清空遥测终端的历史数据单元配置信息
	CPumpGate_YK_PARA      m_Cur_PumpGate_Object;														//当前操作的遥控水泵或阀门/闸门对象
	CRTUAlarm_State		   m_RTUAlarm_State;															//遥测终端的报警状态
	CRTU_EXAlarm_State     m_RtuExAlarm_State;															//遥测知道扩展报警状态
	CRTUTerminal_State     m_RTUTerminal_State;															//遥测终端的状态信息
	CWaterPump_WorkData    m_WaterPump_WorkData;														//水泵电机实时工作数据
	CWaterQuality_PARA     m_WaterQualityPara;															//水质参数信息

	BYTE                   m_QueryRealData_Cmd;															//查询遥测终端实时值的命令
	BYTE                   m_YK_OP_Result;																//遥控操作结果;
	CList<CEventRecord_PER*> m_lstEventRecords;															//事件记录list
	CDayHistoryData		  m_DayHisoryData;																//历史日记录数据
	bool                   m_blRecvFLAG;																//是否已经接收报文标准， 初始状态为 false ,
	std::map<UINT64,CPulsePara_PER*> m_mapPulseParas;													//电度参数map

	CPtrList m_lstYcObjects;																			//遥测对象列表
	CPtrList m_lstYxObjects;																			//遥信对象列表
	CPtrList m_lstYkObjects;																			//遥控对象列表
	CPtrList m_lstZdObjects;																			//定值对象列表
	bool BuildYxObjects(CDatabase* pDb);																//读取所属遥测对象信息
	bool BuildYcObjects(CDatabase* pDb);																//读取所属遥信对象信息
	bool BuildYkObjects(CDatabase* pDb);																//读取所属遥控对象信息
	bool BuildZdObjects(CDatabase* pDb);																//读取所属定值对象信息
	bool BuildExtInformation(CDatabase* pDb);															//扩展信息（卫星通信地址、协议版本号）
	bool BuildExt_Para(CDatabase* pDb);
	bool InitReSendInterval();																			//设置重发时间间隔，重发时间间隔，单位：秒（second）											    
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
	bool CheckYcObject(WORD ycCode,CRtuYcObject* pCurYc = NULL);			//检查遥测对象是否存在， pCurYc 为当前yc对象 true 存在, false 不存在
	bool CheckYxObject(WORD yxCode,CRtuYxObject* pCurYx = NULL);			//检查遥信对象是否存在， pCurYx 为当前yx对象 true 存在, false 不存在
	bool CheckYkObject(WORD ykCode,CRtuYkObject* pCurYk = NULL);			//检查遥控对象是否存在， pCurYk 为当前yk对象 true 存在, false 不存在
	bool CheckZdObject(WORD zdCode,CRtuZdObject* pCurZd = NULL);			//检查定值对象是否存在， pCurZd 为当前zd对象 true 存在, false 不存在
	bool CheckModify(PRTU_STATION pNewRtuInfo);								//检查新的信息，与本是的信息对比，得到是否有修改， true 有修改，false 没修改
	void SetLastRecvTime(PSYSTEMTIME pSt= NULL);							//设置最新接收报文的时间
	PSYSTEMTIME GetLastRecvTime();											//获取最近接收报文的时间
	const CString GetLastRecvTimeString();									//获取最新接收报文时间[转换为字符串]
	//Option's informations
protected:
	WORD  m_AddrA1;				//行政区划码A1 (0-9999)
	BYTE  m_ProvinceCode;		//省码 0 - 99
	BYTE  m_RegionCode;			//区码 0 - 99
	BYTE  m_CountyCode;			//县码 0 - 99

	DWORD m_AddrA2;				//终端或中继地址 A2 
	BYTE  m_AddrA3;				//控制编码 A3(2,5)
	BYTE m_CmType;				//通信方式
	BYTE m_Interface;			//接口方式
	BYTE m_ProtocolType;		//网络协议 UDP 、 TCP
	CString m_ComPortA;			//端口号
	int m_BaundA;				//波特率
	BYTE m_DataBitsA;			//数据位
	BYTE m_StopBitsA;			//停止位
	BYTE m_ParityA;				//奇偶校验方式
	CString m_IpAddrA;			//Ip地址
	unsigned short m_SockPortA;	//Socket端口号

	//Add 2012-4-24 扩展
	BYTE   m_nAppProVer;		//协议版本 1.0：Old  2.0:攀枝花项目修改后协议
	UINT32 m_nScAddress;		//卫星通信地址
	int   m_nScFactoyType;		//卫星通信设备厂家

	BYTE   m_nTmAppProVer;		//协议版本 1.0：Old  2.0:攀枝花项目修改后协议
	CString m_nTmAddress;		//短信通信地址
	int   m_nTmFactoyType;		//短信通信设备厂家

	bool  m_bSetTimePerDay;     //是否对遥测终端机设置了时间，每天对时一次 默认 false
	bool  m_bSetTimeEnable;     //是否能够设置时间，默认:false

	int	  m_nDzDelay;			//对钟时延,默认:0	  
protected:
    BYTE m_bCommState ;			//通信状态信息  0 不通 1 正常 2 未知
	CDatabase* m_pDatabase;		//数据库连接 =CRealTimeDB.DB
	SYSTEMTIME m_LastRecvTime;	//最后接收报文的时间
	BYTE m_RtuWorkMode;         //遥测终端工作模式 00B 兼容工作模式,01B 自报工作模式,02B 查询/应答工作模式,03B 调试维护工作模式,默认：01B 自报模式
	BYTE m_RtuLinkCheckState;   //链路检测状态，F0H：登录,F1H：退出登录,F2H：在线保持,默认：F0
	CMutex m_Modify_Mutex;		//修改属性的互斥锁 主要针对临界属性
public:
	bool SetCommState(BYTE newState);														//设置通信状态
	const BYTE GetCommState()					{ return m_bCommState;}						//获取通信状态 
	void SetWorkMode(BYTE workMode)				{m_RtuWorkMode = workMode;}					//设置遥测终端的工作模式
	const BYTE GetWorkMode()					{return m_RtuWorkMode;}						//获取遥测终端的工作模式
	void SetLinkCheckState(BYTE LinkState)		{m_RtuLinkCheckState = LinkState;}			//设置链路检测状态
	const BYTE GetLinkCheckState()				{return m_RtuLinkCheckState;}				//获取链路检测状态

	//2012-05-08 自动对时
	bool IsSetTimePerDay()						{return m_bSetTimePerDay;}
	void SetTimePerDay(bool _value)				{if (m_bSetTimePerDay != _value) m_bSetTimePerDay = _value;}

	void SetTimeEnable(bool _value)				{if (m_bSetTimeEnable != _value) m_bSetTimeEnable = _value;}
	bool IsSetTimeEnable()						{return m_bSetTimeEnable;}

	void SetDzDelay(int nDelay)					{ if (m_nDzDelay != nDelay) m_nDzDelay = nDelay;}
	const int GetDzDelay()						{return m_nDzDelay ;}
public:
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
	const int  GetScFactoryType()				{return m_nScFactoyType;}

    //更新RTU的IP地址和端口号,通信接口方式
	void   SetNetInfo(PNET_INFO pNetInfo);
	void   SetIpAddrA(CString newIP)			{if (m_IpAddrA != newIP) m_IpAddrA = newIP;}
	void   SetSockPortA(USHORT newSockPort)		{if (m_SockPortA != newSockPort) m_SockPortA = newSockPort;}
	void   SetCmType(BYTE newCmType)			{if (m_CmType != newCmType) m_CmType = newCmType;}
	void   SetInterface(BYTE newInterface)		{if (m_Interface != newInterface) m_Interface = newInterface;}
	void   SetProtocolType(BYTE newProtocol)	{if (m_ProtocolType != newProtocol) m_ProtocolType = newProtocol;}

	//更新串口配置信息
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

    void  SetTmCommInfo(PSC_TEXTMSG_INFO pInfo);
	void  SetTmAddress(CString tmAddr)			{if (m_nTmAddress != tmAddr) m_nTmAddress = tmAddr;}
	void  SetTmAppProVer(BYTE proVer)				{if (m_nTmAppProVer != proVer) m_nTmAppProVer = proVer;}
	void  SetTmFactoryType(int tmfactoryType)	{if (m_nTmFactoyType != tmfactoryType) m_nTmFactoyType = tmfactoryType;}

public:
	//更新RealDB内存中的数据信息
	bool UpdateRealDBYc(CString ycID,DB_RECORD_OPERATION_TYPE opType);
	bool UpdateRealDBYx(CString yxID,DB_RECORD_OPERATION_TYPE opType);
	bool UpdateRealDBYk(CString ykID,DB_RECORD_OPERATION_TYPE opType);
	bool UpdateRealDBZd(CString zdID,DB_RECORD_OPERATION_TYPE opType);
	bool UpdateConfigInfo(PRTU_STATION pInfo);									//更新RTU站的基本配置信息
private:
	//删除RealDB中的内存对象
	bool DeleteRealDBYc(CString ycID);
	bool DeleteRealDBYx(CString yxID);
	bool DeleteRealDBYk(CString ykID);
	bool DeleteRealDBZd(CString zdID);

	//从数据库中获取新增或修改后的对象信息
	CRtuYcObject* QueryYcInDB(CString ycID);
	CRtuYxObject* QueryYxInDB(CString yxID);
	CRtuYkObject* QueryYKInDB(CString ykID);
	CRtuZdObject* QueryZdInDB(CString zdID);

	//向内存链表中添加对象信息
	bool AddRealDBYc(CRtuYcObject* pYc);
	bool AddRealDBYx(CRtuYxObject* pYx);
	bool AddRealDBYk(CRtuYkObject* pYk);
	bool AddRealDBZd(CRtuZdObject* pZd);

public:
	void Init();

private:
	//添加一条日志信息到日志系统（存库）
	BOOL AddLogMessage(_LOG_MESSAGE_TYPE _logType,
		                CString _LogMessage,
						_Log_level_type _levelType = Normal_Log_Message,
						PSYSTEMTIME pLogTime = NULL);

	//添加一条通信状态改变的日志信息到日志系统（存库）
	BOOL AddCmStateChangeLog();
public:
	//添加一条遥控操作的日志信息
	BOOL AddYkOperateLog(CString _LogMessage,
		                 _Log_level_type _levelType = Normal_Log_Message,
						 PSYSTEMTIME pLogTime = NULL);

	const CString Get_Log_RtuInfo();									//插入日志时调用( Rtu完整的信息)
protected:
	CHandle_COM* m_pHandle_Com;											//串口通信接口
	CCriticalSection m_cs_HandleCom;

	CHandle_NET* m_pHandle_Net;											//UDP通信接口
	CCriticalSection m_cs_HandelNet;

	CHandle_TcpClient* m_pHandle_TcpClient;								//TCP通信接口
	CCriticalSection m_cs_HandleTcpClient;

	CSCMessageDealModule* m_pScMessageDealModule;						//卫星通信处理接口
	CCriticalSection m_cs_ScMessageDealModule;

	CCriticalSection m_cs_TmMessageDealModule;                          //短信通信处理接口
	CTmMessageDealModule *m_pTmMessageDealModule;

	CSysDataTrans m_SysDataTrans;										// 报文处理对象
public:
	void SetHandle_Com(CHandle_COM* pHandleCom);
	void SetHandle_Net(CHandle_NET* pHandleNet);
	void SetHandle_TcpClient(CHandle_TcpClient* pHandleTcpClient);

	void SetScMessageDealModule(CSCMessageDealModule* pScMessageModule);
	void SetTmMessageDealModule(CTmMessageDealModule* pTmMessageModule);
	CSCMessageDealModule* GetScMessageDealModule();
	CTmMessageDealModule* GetTmMessageDealModule();

	bool SendData(BYTE* buffer,WORD len);								//向链路发送下发报文
	CSysDataTrans* GetSysDataTrans();									//报文处理对象
	
	bool blEnbaleTrans();												//是否具有下发报文的先决条件
	void ClearCommHandle();												//清除所有的通信接口关联的属性信息
protected:
	CList<Log_Item*> m_lstData;											//存储数据（自报）的链表（属性页数据显示使用）--存储在缓冲区
	CList<Log_Item*> m_lstLogs;											//存储日志信息的链表（属性页的日志信息）--存储在缓冲区
	CList<Log_Item*> m_lstComms;										//通信日志信息（遥测终端通信情况的变化），保存最新Max_log_Count
public:
	void InsertDataMessage(CString DataMsg,PSYSTEMTIME pTime = NULL);   //添加数据信息
	void InsertDataMessage(Log_Item* pData);							//添加数据信息
	CList<Log_Item*>* GetDataList();
	void ClearDataList();

	void InsertLogMessage(CString logMessage,
		 _Log_level_type xLevel = Normal_Log_Message,
		 PSYSTEMTIME pTime = NULL);										//添加属性日志信息
	void InsertLogMessage(Log_Item* pLog);								//添加属性日志信息
	CList<Log_Item*>*  GetLogList();
	void ClearLogList();												//清空Log链表


	void InsertCommMessage(CString CommLog,
		_Log_level_type xLevel = Normal_Log_Message,
		PSYSTEMTIME pTime = NULL);
	void InsertCommMessage(Log_Item* LogItem);
	CList<Log_Item*>* GetCommList();
	void ClearCommList();
public:
	void              CheckRepeatType(CPacketFrameFormat* pPacket);		//检查报文是否为重复报文，如为已经接收的报文，不处理
protected:
	CPacketFrameFormat m_lastPacket;									//最后接收的packet(报文)
	void               UpdateLastPacket(CPacketFrameFormat* pPacket);	//保存最后接收的报文信息
};


