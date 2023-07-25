#pragma once

#include "afxmt.h"
#include <vector>
#include <iostream>
#include <list>
#include <map>
#include <bitset>
#include "GridCtrl/GridCtrl.h"

#ifndef _GET_APPLICATION_
#define _GET_APPLICATION_
#define GETAPP() ((CCMSApp*)AfxGetApp())
#define GETMAINFRAME() ((CMainFrame*)(AfxGetApp()->m_pMainWnd))
#define SHOW_LOG_MESSAGE(strDate,strLog) \
					{if (GETMAINFRAME()->GetSafeHwnd()) \
							GETMAINFRAME()->ShowLog(strDate,strLog); }
#endif

#define _DELETE(p) {if (p!= NULL){delete p; p = NULL;}}

#define ENCPY 0xE5 /* 11100101 */
/* 定义反转生成多项式*/
#define ENCPY_REV 0xA7 /* 10100111 */

//随机自报报警数据所需
static CString _Alert_Options_Name[18]=
{
	"工作交流点停电报警",
	"蓄电池电压报警",
	"水位超限报警",
	"水压超限报警",
	"水质超限报警",
	"流量仪表故障报警",
	"水泵故障报警或开停自报",
	"剩余水量的下限报警",
	"",
	"",
	"终端IC卡功能报警",
	"定值控制报警",
	"水泵工作状态自报",
	"终端箱门状态报警",
	"",
	"",
	"雨量超限报警",
	"模拟量超限报警"
};

enum _Log_level_type
{
	Normal_Log_Message =  1,  //普通
	Warning_log_Message = 2,  //警告
	Error_Log_message  =  3,  //错误
};

//流水日志信息Item
typedef struct _LOG_ITEM_
{
	SYSTEMTIME m_Time;			//时间信息
	CString    m_Message;		//数据信息
	_Log_level_type m_Level;
	_LOG_ITEM_()
	{
		GetLocalTime(&m_Time);
		m_Message = "";
		m_Level = Normal_Log_Message;
	}
}Log_Item,*PLog_Item;

const int MAX_LogCount = 100/*150*/;

typedef struct WATER_QU_STRUCT
{
	char	name[100];
	BYTE	bitPos;
	DOUBLE	ScaleMax;
	DOUBLE  ScaleMin;
	BYTE    bCount;      //数据总位数
	BYTE    bPointCount; //小数点位数
	char    Units[20];   //单位
}WATER_QU_STRUCT;

typedef struct _CMS_GLOBAL_LOG_OPTIONS_
{
	
	BYTE CmStatusSave_Flag;		//RTU 通信 状态日志 存储标志  0 不存 ,1 存储
	BYTE CmBufferSave_Flag;		//RTU 通信 报文日志 存储标志  0 不存 ,1 存储
	BYTE YcChangeSave_Flag;		//RTU 遥测 变位日志 存储标志  0 不存 ,1 存储
	BYTE YxChangeSave_Flag;		//RTU 遥信 变位日志 存储标志  0 不存 ,1 存储
	BYTE YkOperatSave_Flag;		//RTU 遥控 操作日志 存储标志  0 不存 ,1 存储
	BYTE ZdOperatSave_Flag;		//RTU 定值 操作日志 存储标志  0 不存 ,1 存储
	BYTE RunningSSave_Flag;		//系统 运行时 日志  存储标志  0 不存 ,1 存储
	BYTE OhterRunSave_FLag;		//其他日志信息 
	BYTE DataInErrSave_Flag;	//数据实时入库错误信息 的存储标志
	_CMS_GLOBAL_LOG_OPTIONS_()
	{
		CmBufferSave_Flag = 0;
		CmStatusSave_Flag = 0;
		YcChangeSave_Flag = 0;
		YxChangeSave_Flag = 0;
		YkOperatSave_Flag = 0;
		ZdOperatSave_Flag = 0;
		RunningSSave_Flag = 0;
		OhterRunSave_FLag = 0;
		DataInErrSave_Flag = 0;
	}
}CMS_GLOBAL_LOG_OPTIONS,*PCMS_GLOBAL_LOG_OPTIONS;

typedef struct  _CMS_GLOBAL_RTU_OPTIONS_
{
	BOOL  blYxList_FLAG;
	BOOL  blYcList_FLAG;
	BOOL  blYkList_FLAG;
	BOOL  blZdList_FLAG;
	_CMS_GLOBAL_RTU_OPTIONS_()
	{
		blYcList_FLAG = FALSE;
		blYkList_FLAG = FALSE;
		blYxList_FLAG = FALSE;
		blZdList_FLAG = FALSE;
	}
}CMS_GLOBAL_RTU_OPTIONS,PCMS_GLOBAL_RTU_OPTIONS;

typedef struct _CMS_GLOBAL_DB_OPTIONS_  //数据库相关参数
{
	//MIS system SQL server数据源   ODBC 数据源
	CString m_strMISDSN;				//DSN
	CString m_strMISUID;				//User ID   
	CString m_strMISPWD;				//Password
	_CMS_GLOBAL_DB_OPTIONS_()
	{
		m_strMISDSN.Empty();
		m_strMISUID.Empty();
		m_strMISPWD.Empty();
	}

}CMS_GLOBAL_DB_OPTIONS,*PCMS_GLOBAL_DB_OPTIONS;

//密码PW
typedef struct _CMS_GLOBAL_PW_
{
	WORD PW_CALC;                   //密钥算法 0-9
	WORD PW_CODE;					//密钥Code 0-999
	BYTE PW_BUF[2];                 //密钥报文
	_CMS_GLOBAL_PW_()
	{
		PW_BUF[0] = 0;
		PW_BUF[1] = 0;
		PW_CALC   = 0;
		PW_CODE   = 0;
	}
	void BuildPWBuf();
}CMS_GLOBAL_PW,PCMS_GLOBAL_PW;



//运行参数信息
typedef struct _CMS_GLOBAL_OPTIONS_ 
{
	CString m_strLogFileName;			//日志数据库文件名(完整路径)
	BYTE    m_dwLogSaveMonths;			//日志数据保存上限,默认为2个月
	CString m_strCfgFileName;			//配置项存储数据库文件名(完整路径)
	CString m_strAdDivCodeFilename;		//行政区划码表文件
	CString m_strCurRunMoudlePath;		//运行文件所在目录
	CString m_strCSThreadsCfgFile;		//Commserver 线程配置文件名
	CString m_strLocalDbName;			//本地SQLite数据文件名称
	CString m_strAppTitie;				//Application Mainframe Title

	BYTE    m_bAdminMode;				//Administrator Management Mode
	BYTE    m_TpDelay;					//Tp的延时检查数，单位min(分钟)
	BYTE    m_ResendInterval;			//重放报文时间间隔,单位Second（秒） 默认10s
	BYTE    m_DataInterface;			//数据接口，1：GM自定义数据接口，2：四川省山洪灾害数据接口n 3: 云南水雨情
	BYTE    m_DataIType;				//接口方式，当m_DataInterface= 1时有用， 1：直接访问数据库，2：通过webservice存储数据

	CMS_GLOBAL_LOG_OPTIONS m_LogOpts;	//Log System Option;
	CMS_GLOBAL_DB_OPTIONS  m_DbOpts;	//数据库相关参数
	CMS_GLOBAL_RTU_OPTIONS m_RtuOpts;	//Rtu信息含有
	CMS_GLOBAL_PW          m_PW;		//密钥信息
	DWORD m_nCheckRtuInterval;			//检查遥测终端通信状态时间间隔，单位：分钟，默认：5分钟
	u_long  m_nKeepaliveInterval;       //keepaliveIntervel
	WORD	m_nCheckRepeatInterval;		//检查重复数据时间间隔 单位:second,默认:90s ,(0～300)

	double m_dRainCheckPer;				//检查5、15、1hour降雨量计算值
	double m_dRainCheckDay;				//检查1day降雨量的计算值

	//针对冕宁现场雨量数据统计的问题
	UINT   m_nCheck5Minutes_Count;		//5分钟雨量统计时间check 默认10分钟
	UINT   m_nCheck15Minutes_Count;		//15分钟雨量统计时间check 默认30分钟
	UINT   m_nCheck1Hour_Count;			//1小时雨量统计时间check 默认60分钟
	UINT   m_nCheck1Day_Count;			//1天雨量统计时间check 默认60 * 24分钟
 
	_CMS_GLOBAL_OPTIONS_()
	{
		m_strCfgFileName.Empty();
		m_strLogFileName.Empty();
		m_dwLogSaveMonths = 2;
		m_strCurRunMoudlePath.Empty();
		m_strCSThreadsCfgFile.Empty();
		m_strLocalDbName.Empty();
		m_strAppTitie = "CMS - 通信前置机";
		m_TpDelay = 0;
		m_ResendInterval =   0;
		m_nCheckRtuInterval = 0; 
		m_nKeepaliveInterval = 0;
		m_nCheckRepeatInterval = 90;
		m_DataInterface = 1;
		m_DataIType = 1;
		m_dRainCheckDay = 0.0;
		m_dRainCheckPer = 0.0;

		//
		m_nCheck5Minutes_Count = 10;
		m_nCheck15Minutes_Count = 30;
		m_nCheck1Hour_Count = 61;
		m_nCheck1Day_Count = 1440;
	}

}CMS_GLOBAL_OPTIONS,*PCMS_GLOBAL_OPTIONS;

//const long	RECV_BUFFER_LEN		= 512;		//接收缓冲区最大长度
const long	RECV_BUFFER_LEN		= 1024;			//接收缓冲区最大长度
const long  SEND_BUFFER_LEN     = 255;			//发送报文最大长度
const long  READPORT_LEN_PER    = 300;			//读取单条串行报文、网络报文的最大长度
const int   RESEND_Count        = 2;            //重发次数,FCB=3 

const BYTE  ZH_CN_LANGUAGE      = 0x01;         //中文
const BYTE  ENGLISH_LANGUAGE	= 0x02;         //英文


#ifndef _PROTOCOL_TRANS_ARROW_
#define _PROTOCOL_TRANS_ARROW_
	const BYTE   UP_ARROW         = 1;			//由终端发出的上行报文
	const BYTE   DOWN_ARROW       = 0;			//有中心站发出的下行报文	
#endif

#ifndef _THREAD_STATUS_INFO_					//线程运行状态定义
#define _THREAD_STATUS_INFO_
	const BYTE  THREAD_STATE_RUNNING = 1;		//正在运行
	const BYTE  THREAD_STATE_PAUSE   = 2;		//暂停、挂起
	const BYTE  THREAD_STATE_STOP    = 0;		//停止
#endif

#ifndef _PROTOCOL_C_COMMAND_					//控制域功能码定义 下行
#define _PROTOCOL_C_COMMAND_
	const BYTE C_CMD_01_CMD = 1;				//命令
	const BYTE C_CMD_02_USERDATA = 2;			//用户数据
	const BYTE C_CMD_03_LINKTEST = 3;			//链路测试
	const BYTE C_CMD_04_MSUREPARA = 4;			//被测参数
	const BYTE C_CMD_05_MSUERSTATE = 5;			//被测状态
	const BYTE C_CMD_06_WLPARA = 6;				//水位参数
	const BYTE C_CMD_07_FLOWPARA = 7;			//流量参数
	const BYTE C_CMD_08_WPPARA = 8;				//水压参数
	const BYTE C_CMD_09_WQPARA = 9;				//水质参数
	const BYTE C_CMD_10_GENERALPARA =10;		//综合参数
	const BYTE C_CMD_11_RAINPARA = 11;			//雨量参数
	const BYTE C_CMD_12_MONIPARA = 12;			//模拟量参数
	const BYTE C_CMD_13_PULSEPARA = 13;			//电度参数
#endif

//通信通道类型
#ifndef _COMM_TYPE_
#define _COMM_TYPE_
	/*
	    通信方式
	1	GPRS通道通信
	2	GSM手机短信通道通信
	3	卫星通道通信
	4	超短波通道通信
	5	微波通道通信
	6	其他
	*/
	const BYTE	GRPS_CommChannel = 1;	//1	GPRS通道通信
	const BYTE	GSM_CommChannel  = 2;	//2	GSM手机短信通道通信
	const BYTE	SC_CommChannel   = 3;   //3	卫星通道通信
	const BYTE	USW_CommChannel  = 4;   //4	超短波通道通信
	const BYTE  MW_CommChannel   = 5;   //5	微波通道通信
	const BYTE  Other_CommChannel =6;   //6	其他
#endif

#ifndef _SCFactoryType_Const_
#define _SCFactoryType_Const_
	const int  SCFactoryType_GTDZ = 1;  //国腾电子北斗卫星
	const int  SCFactoryType_SZTH = 2;  //神州天鸿北斗卫星
#endif

//应用协议版本号
#ifndef _APPLICATION_PROTOCOL_VERSION
#define _APPLICATION_PROTOCOL_VERSION
	const BYTE APP_PROTOCOL_V1_0  = 1; // V1.0 应用协议
	const BYTE APP_PROTOCOL_V2_0  = 2; // V2.0 应用协议
#endif

#ifndef _COMM_INTERFACE_TYPE_                   //通信接口定义
#define _COMM_INTERFACE_TYPE_
	const BYTE   Ethernet_CommInerface = 1;		//以太网接口
	const BYTE   Serial_CommInterface  = 2;		//串行接口
#endif


#ifndef _DATAINTERFACE_TYPE_
#define _DATAINTERFACE_TYPE_
	const BYTE DATAINTERFACE_GM = 1;			//交大光芒自定义数据接口
	const BYTE DATAINTERFACE_SC = 2;			//四川省山洪灾害数据库接口
	const BYTE DATAINTERFACE_YN = 3;          //云南水雨情数据库接口
#endif

typedef struct _OVERLAPPEDPLUS
{
	OVERLAPPED	ol;
	SOCKET		socket;
	int			nOpCode;
	WSABUF		wsaBuf;
	DWORD		dwFlags;
	DWORD		dwBytes;
	char		pBuf[1024];
	_OVERLAPPEDPLUS()
	{
		socket = INVALID_SOCKET;
		nOpCode = 0;
		dwBytes = 0;
		memset(pBuf,0,sizeof(pBuf));
	}
}OVERLAPPEDPLUS, *POVERLAPPEDPLUS;


#ifndef _COMM_PROTOCOL_TYPE_					//以太网通信协议类型
#define _COMM_PROTOCOL_TYPE_			
	const BYTE UDP_Protocol   = 0x01;			//UDP 方式
	const BYTE TCP_Protocol	  = 0x02;			//TCP 方式
#endif


#ifndef _COMM_LINK_STATE_
#define _COMM_LINK_STATE_
	const BYTE Disconnect_State		= 0;			//通信断开
	const BYTE Connected_State		= 1;			//通信连接
	const BYTE CommUnknown_State	= 2;			//通信未知
#endif

enum DB_RECORD_OPERATION_TYPE
{
	RECORD_NEW			= 0,        //New added
	RECORD_EDIT			= 1,	    //Edit/modified record
	RECORD_DEL			= 2,		//Delete record
	RECORD_NOOPERATE	= 3,        //No Operate record
};

enum WATER_PROTOCOL_ENUM
{
	WaterResources_Protocol = 1,   //水资源
	Hydrology_Protocol      = 2,   //水文
	SCSW_008_Protocol       = 3,
	SYQYC_Comm_Protocol     = 4, //水雨情遥测系统数据通信规约
};

enum GRIDCTRL_STYLE
{
	RTUST_STINFO_TYPE = 0,  //显示RTU站信息
	RTUST_YCINFO_TYPE = 1,  //显示RTU站遥测信息
	RTUST_YXINFO_TYPE = 2,  //显示RTU站遥信信息
	RTUST_YKINFO_TYPE = 3,  //显示RTU站遥控信息
	RTUST_ZDINFO_TYPE = 4,  //显示RTU站定值信息
};

#define  MFC_GRIDCTRL_HEIGHT			18     //GridCtrl高度
#define  MFC_GRIDCTRL_CHECKBOX_WIDTH	25	   //CheckBox 宽度

enum SCSW_TIMETYPE
{
	TIMETYPE_TT = 0,//时间
	TIMETYPE_TB = 1,//历史起始时间
	TIMETYPE_TE = 2,//历史结束时间
	TIMETYPE_TM = 3,//最大极值发生时间
	TIMETYPE_TN = 4, //最小极值发生时间
	TIMETYPE_CURR = 5
};
enum SCSW_TYPE
{
	SCSW_Z   = 1000,             //水位
	SCSW_PN  = 1001,  			  //雨量一
	SCSW_P   = 1002,
	SCSW_M10 = 1003,
	SCSW_H   = 1004,
	SCSW_C   = 1005,
	SCSW_ED  = 1006,
	SCSW_ZR  = 1007, 
	SCSW_RC  = 1008,
	SCSW_SS  = 1009,
	SCSW_ML  = 1010,
	SCSW_MF  = 1011,
	SCSW_BV  = 1012,
	SCSW_SV  = 1013,
	SCSW_ET  = 1014,
	SCSW_SI  = 1015,
	SCSW_PI  = 1016, 
	SCSW_RD  = 1017,
	SCSW_ZJ  = 1018,
	SCSW_ZT  = 1019,
	SCSW_ZF  = 1020,
	SCSW_DZ  = 1022,
	SCSW_ST  = 1023,
	SCSW_NN  = 1024,
	SCSW_TT  = 1025,
	SCSW_TM  = 1026,
	SCSW_TN  = 1027,
	SCSW_TB  = 1028,
	SCSW_TE  = 1029,
	SCSW_RR  = 1030,
	SCSW_DV  = 1031,
	SCSW_DI  = 1032,
	SCSW_DC  = 1033,
	SCSW_CN  = 1034,
	SCSW_CP  = 1035,
	SCSW_GSM = 1036,
	SCSW_BD  = 1037,
	SCSW_PSTN= 1038,
	SCSW_IT  = 1039,
	SCSW_PT  = 1040,
	SCSW_TC  = 1041,
	SCSW_PW  = 1042,
	SCSW_OK  = 1043,
	SCSW_ER  = 1044,
	SCSW_RF  = 1045,
	SCSW_ZA  = 1046,
	SCSW_ZB  = 1047,
	SCSW_ZC  = 1049,
	SCSW_ZD  = 1050,
	SCSW_ZE  = 1051,
	SCSW_DZA  = 1052,
	SCSW_DZB  = 1053,
	SCSW_DZC  = 1054,
	SCSW_DZD  = 1055,
	SCSW_DZE  = 1056,
	SCSW_HOST = 1057,
	SCSW_A0   = 1058,
	SCSW_A1   = 1059,
	SCSW_A2   = 1060,
	SCSW_A3   = 1061,
	SCSW_B0   = 1062,
	SCSW_B1   = 1063,
	SCSW_B2   = 1064,
	SCSW_B3   = 1065
};

enum SCSW_START
{
	START_B = 0,
	START_C = 1,
	START_D = 2,
	START_E = 3,
	START_G = 4,
	START_H = 5,
	START_I = 6,
	START_M = 7,
	START_N = 8,
	START_O = 9,
	START_P = 10,
	START_R = 11,
	START_S = 12,
	START_T = 13,
	START_Z = 14
};




enum SYQ_TYPE
{
	SYQ_DRD  = 1997,//时间步长 单位日
	SYQ_DRH  = 1998,//时间步长 单位小时
	SQY_DRN  = 1999,//时间步长 单位分钟
	SYQ_ED   = 2000,//日蒸发量
	SYQ_EM   =  2001,//月蒸发量
	SYQ_EX   = 2002,//
	SYQ_FDZ  = 2003,//
	SYQ_P1   = 2004,//1h时段降水量 毫米 
	SYQ_P2   = 2005,//2h时段降水量 毫米 
	SYQ_P3   = 2006,//3h时段降水量 毫米 
	SYQ_P6   = 2007,//6h时段降水量 毫米 
	SYQ_P12  = 2008,//12h时段降水量 毫米 
	SYQ_PD   = 2009,//日降水量
	SYQ_PM   = 2010,//月降水量
	SYQ_PN01 = 2011,//1min时段降水量 毫米 
	SYQ_PN05 = 2012,//5min时段降水量 毫米 
	SYQ_PN10 = 2013,//10min时段降水量 毫米 
	SYQ_PN30 = 2014,//30min时段降水量 毫米 
	SYQ_PR   = 2015,//暴雨量 毫米
	SYQ_PX   = 2016,//旬降水量
	SYQ_Q    = 2017,//瞬时流量、抽水流量
	SYQ_QA   = 2018,//总出库流量、过闸总流量
	SYQ_QAD  = 2019,//日平均出库流量
	SYQ_QAM  = 2020,//月平均出库流量
	SYQ_QAMM = 2020,//月最大总出库量
	SYQ_QAMN = 2021,//月最小总出库量
	SYQ_QAX  = 2022,//旬平均出库流量
	SYQ_QAXM = 2023,//旬最大总出库量
	SYQ_QAXN = 2024,//旬最小总出库量
	SYQ_QD   = 2025,//河道、闸坝日平均流量，泵站日平均抽水流量
	SYQ_QI   = 2025,//入库流量
	SYQ_QID   = 2026,//日平均入库流量
	SYQ_QIM   = 2027,//月平均入库流量
	SYQ_QIMM  = 2028,//月最大入库流量
	SYQ_QIMN  = 2029,//月最小入库流量
	SYQ_QIX   = 2030,//旬平均入库流量
	SYQ_QIXM   = 2031,//旬最大入库流量
	SYQ_QIXN   = 2032,//旬最小入库流量

    SYQ_PT  =   2050,//降水量累计值  
};
//串口信息
typedef struct _COM_STRUCT_INFO_
{
	CString mComName;		//串口名称
	int     mBaund;			//波特率
	BYTE    mDataBits;		//数据位数
	BYTE    mStopBits;		//停止位数
	BYTE     mParity;       //校验 （NOPARITY    0 无检验）、（ODDPARITY   1 奇校验）、（EVENPARITY  2 偶校验）、（MARKPARITY  3）、 （SPACEPARITY 4）
	_COM_STRUCT_INFO_& operator = (_COM_STRUCT_INFO_ & temp);
    _COM_STRUCT_INFO_()
	{
		mComName ="";
		mBaund = 9600;
		mDataBits = 8;
		mStopBits = ONESTOPBIT;
		mParity = NOPARITY;
	}

}COM_INFO, *PCOM_INFO;
//网络接口 SOCKET 信息w
typedef struct _NET_STRUCT_INFO_
{
	CString mIpAddress;          //IP地址
	unsigned short mPort;        //端口
	_NET_STRUCT_INFO_& operator = (_NET_STRUCT_INFO_& temp);
	_NET_STRUCT_INFO_()
	{
		mIpAddress = "0.0.0.0";
		mPort  = 0;
	}
}NET_INFO,*PNET_INFO;

//卫星通信信息;
typedef struct _SC_COMM_INFO_
{
	int  nFactoryType;  //卫星通信厂家
	UINT32 nScAddress;	//卫星通信本地地址,本地、远方 卫星通信地址
	_SC_COMM_INFO_& operator = (_SC_COMM_INFO_& temp);
	_SC_COMM_INFO_()
	{
		nFactoryType = 1;
		nScAddress = 0;
	}

}SC_COMM_INFO,*PSC_COMM_INFO;

typedef struct _RTU_STATION_
{
	CString  _strName;					 //Station RTU name
	CString  _strSId;                    //Station's ID
	CString  _strStCode;                 //用于SCSW008的地址码
	int  _strSYQADDR;				     //水雨情地址码
	int      _centercode;				 //水雨情中心站码
	int    _passWord;                    //水雨情密码
	WORD     _nAddrA1;					 //地址域：行政区划码A1（0-9999）
	BYTE     _bProvinceCode;             //省(直辖市，自治区）码
	BYTE     _bRegionCode;               //地（市）码
	BYTE     _bCountyCode;               //县码
	DWORD    _nAddrA2;                   //地址域：终端或中继地址A2、A3
	BYTE     _nAddrA3;                   //地址域：控制编码 2： A2为终端地址  5：A2为中继站地址（2，5）

	BYTE    _bCmType;					 //通信方式（GSM,GPRS）
	BYTE    _bInterface;				 //接口方式（串口、网络）
	COM_INFO  _CmComInfo;				 //通信 串口信息
	NET_INFO  _CmNetInfo;				 //通信 网络接口信息（IP,端口号）

	SC_COMM_INFO _CmScCommInfo;			 //卫星通信接口（卫星通信地址、厂家）
	BYTE _nAppProVer;					 //应用协议版本： v1.0 v2.0
	BYTE _nWorkMode;					 //工作模式：需要存库 ，00B 兼容工作模式,01B 自报工作模式,
	_RTU_STATION_()
	{
		_strName = "";
		_strSId = "";
		_nWorkMode = 1;					//默认自报
		_bAppProType = (BYTE)WaterResources_Protocol;
		_nAppProVer = 1;

	}

	BYTE _bAppProType;                  //应用层协议类型

}RTU_STATION,*PRTU_STATION;

typedef struct _RTU_YC_OBJECT_
{
	CString _strName;					//YC Name
	CString _strSId;					//Station's ID
	CString _strYcObjID;				//Yc Object ID
	unsigned int _nYcCode;				//Yc's Code
	BYTE	_Type;						//Yc Type;
	BYTE    _DisplayBits;				//显示位数
	BYTE	_PointBits;					//显示小数点位数
	CString _Unit;						//Yc's Unit
	float	_MaxValue;					//最大值
	float   _MinValue;					//最小值
}RTU_YCOBJECT,*PRTU_YCOBJECT;

typedef struct _RTU_YX_OBJECT_
{
	CString _strName;					//Yx Name;
	CString _strSId;					//Station's ID;
	CString _strYxObjID;				//Yx object ID
	unsigned int _nYxCode;				//Yx Code;

}RTU_YXOBJECT,*PRTU_YXOBJECT;

typedef struct _RTU_YK_OBJECT_
{
	CString _strName;					//Yk Name;
	CString _strSId;					//Station's ID;
	CString _strYkObjID;				//Yk Object ID
	unsigned int _nYkCode;				//Yk code

}RTU_YKOBJECT,*PRTU_YKOBJECT;

typedef struct _RTU_ZD_OBJECT_
{
	CString _strName;					//ZD Name;
	CString _strSId;					//Station's ID;
	CString _strZdObjID;				//ZD Object(Item) ID
	unsigned int _nZDCode;				//ZD Object(Item) code;
	BYTE    _Sizeof;					//整定项的宽度，Bit
	BYTE	_OrderInItem;				//位于整定项的第几位，0表示整个整定项
	CString _Unit;						//Unit
	float   _ScaleMax;					//ZD Object(item)'s ScaleMax value
	float	_ScaleMin;					//ZD Object(item)'s ScaleMin value;

}RTU_ZDOBJECT,*PRTU_ZDOBJECT;

struct _ControlC //控制域
{
	unsigned char _DIR:1;				//DIR  传输方向 D7 
	unsigned char _DIV:1;				//DIV  拆分标志位 D6
	unsigned char _FCB:2;				//FCB  帧计数位 D4~D5
	unsigned char _Command:4;			//控制域功能码   D3~D0
	unsigned char _DIVS;				//拆分计数； 当_DIV=1是 才有_DIVS字节
	_ControlC()
	{
		_DIR = 0;
		_DIV = 0;
		_FCB = 0;
		_Command = 0;
		_DIVS = 0;
	}

	BYTE GetControlCByte();              //将控制域相关信息转换为报文字节数据(1 BYTE)
};


class _ASDU   //应用层 帧格式
{
public:
	_ASDU();
	~_ASDU();
public:
	unsigned char  _AFN;					//应用层功能码
	std::vector<unsigned char> data;		//包含：数据域和附加信息域AUX(PW和TP)
	//std::vector<unsigned char> _AUX;		//附加信息域AUX
};

class SCSW_ASDU
{
public:
	SCSW_ASDU();
	~SCSW_ASDU();

public:
	unsigned char data[300];

};

//行政区划码A1
struct _A1
{
	BYTE  ProvinceCode;						//省码(0-99)
	BYTE  RegionCode;						//地区（市）码(0-99)
	_A1()
	{
		ProvinceCode = 0;
		RegionCode = 0;
	}
};

//存放SCSW报文中的具体信息
class SCSW_MsgInfo
{
public:
	SCSW_MsgInfo()
	{
		m_nProtoType = -1;
		m_nExtraInfo = -1;
	}
	~SCSW_MsgInfo(){}

public:
	int m_nProtoType;            //类型标示
	int m_nExtraInfo;            //额外信息存储报文中某些类型之后跟随的数字，如Z01
	std::list<CString> m_OpDataList; //操作数
	CCriticalSection m_cs;
};


class _ADDR_Info
{
public:
	struct _A1 A1;                          //两个字节的BCD码， 行政区划码 A1 4位十进制数表示，前两位为省码，后两位为地码
	DWORD	   A2;                          //d0-d20 终端地址/中继地址 (3 Byte)
    BYTE       A3;                          //控制编码 A3  为010B(2)时 A2为终端地址，  101B(5)时，A2为中继地址(2-5)
	_ADDR_Info()
	{
		A1.ProvinceCode = 0;
		A1.RegionCode   = 0;
		A2 = 0;
		A3 = 0;
	}
};

struct _SYQ_CONFIG_PACKET2//遥测站参数2配置包 
{
	_SYQ_CONFIG_PACKET2()
	{
		CFG_20 = 1;CFG_21 = 0;CFG_22 = 1;CFG_23 = 1;CFG_24 = 0;CFG_25 = 0;CFG_26 = 1;CFG_27 = 1;CFG_28 = 0;CFG_29 = 0;CFG_2A = 1;CFG_2B = 0;
		CFG_2C = 0;CFG_2D = 1;CFG_2E = 0;CFG_2F = 0;CFG_30 = 0;CFG_31 = 0;CFG_32 = 0;CFG_33 = 0;CFG_34 = 0;
	}

	BYTE  CFG_20;
	BYTE  CFG_21;
	BYTE  CFG_22;
	BYTE  CFG_23;
	BYTE  CFG_24;// 2 HEX
	int   CFG_25;//2 BCD
	int   CFG_26;//2 BCD
    int   CFG_27;//2 BCD
	BYTE  CFG_28;
	INT   CFG_29;//2 BCD
	BYTE  CFG_2A;
	int   CFG_2B;//4 BCD
	int   CFG_2C;//3 BCD
	BYTE  CFG_2D;
	DWORD CFG_2E;//5 byet BCD
	int   CFG_2F;//3 BCD
	int   CFG_30;//2 BCD
	int   CFG_31;//2 BCD
	int   CFG_32;//2 BCD
	int   CFG_33;//2 BCD
	int   CFG_34;//2 BCD
};


struct _SYQ_CONFIG_PACKET3//遥测站参数3配置包 
{

	_SYQ_CONFIG_PACKET3()
	{
		memset(CFG_40,0,10);
		memset(CFG_41,0,10);
		memset(CFG_42,0,6);
		memset(CFG_43,0,10);
		memset(CFG_69,0,14);
	}
	char  CFG_40[10];//(7,3)
	char  CFG_41[10];//(7,3)
	char  CFG_42[6];//(4,2)
	char  CFG_43[6];//(7,3)
	char CFG_69[14];//(11,3)

};

struct _SYQ_DATA_DESCRIBE
{
	_SYQ_DATA_DESCRIBE()
	{
		strname ="";
		strdata = "";
	}
	CString strname;
	CString strdata;;
};

class CRtuStation;
class CPacketFrameFormat				//数据包帧格式
{
public:
	CPacketFrameFormat();
	~CPacketFrameFormat();
public:
	unsigned char		_bFLen;					//用户数据长度L   注意:整条报文长度为 L + 5


	_ADDR_Info			_addr_info;				//地址域A  分解信息
	struct _ControlC	ControlC;				//控制域
	_ASDU				_asdu;					//应用层数据域格式

	//std::vector<unsigned char> _ADDR;			//地址域A  数据
	//unsigned char _bStart2;					//起始字符 (0x68H)
	//unsigned char _bStart1;					//起始字符 (0x68H)
	//unsigned char _CS;						//CSR校验
	//unsigned char _bEnd;						//结束字符 (0x16h)

	//其他
public:
	BYTE				m_CommInterface;        //接口模式 串口、以太网
	BYTE				m_ProtocolType;			//网络通信协议类型 UDP or TCP 当通信接口为以太网模式时有用
	NET_INFO			m_NetInfo;              //以太网信息
	COM_INFO			m_ComInfo;				//串口信息
	SC_COMM_INFO        m_ScCommInfo;			//卫星通信接口相关信息
	CRtuStation*		m_pStation;             //Current Rtu station

	//重发数据不存库/处理的问题
protected:
	bool				m_bRepeatType;			//是否是重复数据的报文,默认为false,不是重复的数据
	SYSTEMTIME			m_stRecvTime;			//接收时间
public:
	bool   GetRepeatType() const;				//获取 Repeat标志
	void   SetRepeatType(bool val);				//设置 Repeat标志
	void   UpdateRecvTime(PSYSTEMTIME pTime= NULL);
	PSYSTEMTIME GetRecvTime();
};

class DateTime
{
public:
	void SetZero(){
		month = 0;
		day = 0;
		hour = 0;
		min = 0;
	}

	DateTime(){
		SetZero();
	}

public:
	int month;
	int day;
	int hour;
	int min;
};

class DateTime7
{
public:
	void SetZero(){
		month = 0;
		day = 0;
		hour = 0;
		min = 0;
		second = 0;
		year = 2000;
	}

	DateTime7(){
		SetZero();
	}

	void SetTime(DateTime7 dt)
	{
		month = dt.month;
		day  = dt.day;
		hour = dt.hour;
		min = dt.min;
		second = dt.second;
		year = dt.year;
	}

public:
	int year;
	int month;
	int day;
	int hour;
	int min;
	int second;
};
//水资源数据帧格式报文
class CHydrFrameFormat
{
public:
	CHydrFrameFormat()
	{
		m_nDivCnt = 0;
		m_nStCode = -1;
		m_nDIR = UP_ARROW;
		m_strStCode = "";
		m_TimeType = SCSW_TT;
		m_RR = -1;
		m_workmode = 1;//工作模式

	}

	~CHydrFrameFormat()
	{
		m_cs_info.Lock();
		while (m_MsgInfoList.begin() != m_MsgInfoList.end())
		{
			delete (*m_MsgInfoList.begin());
			m_MsgInfoList.erase(m_MsgInfoList.begin());
		}
		m_cs_info.Unlock();
	}

public:
	void   UpdateRecvTime(PSYSTEMTIME pTime= NULL);

public:
	int m_nFrameLen;			//接收数据帧长度
	int m_nDataCnt;             //数据个数

public:
	BYTE				m_CommInterface;        //接口模式 串口、以太网
	BYTE				m_ProtocolType;			//网络通信协议类型 UDP or TCP 当通信接口为以太网模式时有用
	NET_INFO			m_NetInfo;              //以太网信息
	COM_INFO			m_ComInfo;				//串口信息
	SC_COMM_INFO        m_ScCommInfo;			//卫星通信接口相关信息
	CRtuStation*		m_pStation;             //Current Rtu station

	CCriticalSection          m_cs_info;
	std::list<SCSW_MsgInfo *> m_MsgInfoList;
private:
	SYSTEMTIME			m_stRecvTime;			//接收时间
public:
	int  m_RR;
	int  m_workmode;
	int  m_nDivCnt;
	int  m_nStCode;
	CString m_strStCode;
	BYTE m_nDIR;   //方向
	int  m_nCot;   //传送原因
	int  m_TimeType;//TT, TB, TE, TM, TN
	DateTime m_DataTimeTT;
	DateTime m_DataTimeTB;
	DateTime m_DataTimeTE;
	DateTime m_DataTimeTM;
	DateTime m_DataTimeTN;
};

class CSYQFrameFormat//水雨情报文正文
{
public:
	CSYQFrameFormat(){m_repeat=3;}
	~CSYQFrameFormat(){}
public:
	BYTE				m_CommInterface;        //接口模式 串口、以太网
	BYTE				m_ProtocolType;			//网络通信协议类型 UDP or TCP 当通信接口为以太网模式时有用
	NET_INFO			m_NetInfo;              //以太网信息
	COM_INFO			m_ComInfo;				//串口信息
	SC_COMM_INFO        m_ScCommInfo;			//卫星通信接口相关信息
	CRtuStation*		m_pStation;             //Current Rtu station

	int m_FunType;//功能码，见附录B
	int     m_iPassword;//密码
	DWORD   m_stCode;//'遥测站码
	int     m_CenterCode;//中心站
	int     m_ControlCode;//报文结束符
	int   nMsgLen; //报文正文长度
	int messageno;//流水号
	DateTime7 m_DataTimeSend;//发报时间
	DateTime m_DataTimeGet;//采集时间
	SYSTEMTIME m_stRecvTime;//
	char    m_cType;//编码分类 见附录A
	int    m_repeat;//重发次数
	_ASDU				_asdu;					//应用层数据域格式
	int     len;//报文正文长度

public:
	void   UpdateRecvTime(PSYSTEMTIME pTime= NULL);

};

class CWriteFrame
{
public:
	CWriteFrame()
	{
		m_nFrameLen = 0;
		m_nDevAdddr = "";
		m_nCmdCode = 0;
		m_nPackCnt = 0;
		m_nCodeAddr = 0;
		m_nCodeLen = 0;
		memset(m_ContentBuf, 0, 200);
		memset(Sendbuf, 0, 500);
		m_nCRC = 0;
		bIteratorValid = false;
		bHasContentFrame = false;
	}

	~CWriteFrame()
	{
		m_nFrameLen = 0;
		m_nDevAdddr = "";
		m_nCmdCode = 0;
		m_nPackCnt = 0;
		m_nCodeAddr = 0;
		m_nCodeLen = 0;
		memset(m_ContentBuf, 0, 200);
		memset(Sendbuf, 0, 500);
		m_nCRC = 0;
		bIteratorValid = false;
		bHasContentFrame = false;
	}

	void   UpdateRecvTime(PSYSTEMTIME pTime= NULL);

public:
	BYTE				m_CommInterface;        //接口模式 串口、以太网
	BYTE				m_ProtocolType;			//网络通信协议类型 UDP or TCP 当通信接口为以太网模式时有用
	NET_INFO			m_NetInfo;              //以太网信息
	COM_INFO			m_ComInfo;				//串口信息
	SC_COMM_INFO        m_ScCommInfo;			//卫星通信接口相关信息
	CRtuStation*		m_pStation;             //Current Rtu station

public:
	int m_nFrameLen;
	CString m_nDevAdddr;
	int m_nCmdCode;
	int m_nPackCnt;
	int m_nCodeAddr;
	int m_nCodeLen;
	BYTE m_ContentBuf[200];
	int m_nCRC;
	DateTime m_DataTimeTT;
	SYSTEMTIME	m_stRecvTime;			//接收时间
	bool bIteratorValid;
	bool bHasContentFrame;

public:
	BYTE Sendbuf[500];

};

class CHydrSendFrame
{
public:
	CHydrSendFrame(){
		m_FrameType = 0;
		m_LPARME = 0;
		memset(m_buf, 0, 300);
		m_bufLen = 0;

		m_nTimeType = -1;
		m_CommInterface = -1;
		m_ProtocolType = -1;
		m_setVar = 0.0f;
		m_strStCode = "";
		m_strPsw = "";
	}
	~CHydrSendFrame(){}

public:
	BYTE				m_CommInterface;        //接口模式 串口、以太网
	BYTE				m_ProtocolType;			//网络通信协议类型 UDP or TCP 当通信接口为以太网模式时有用
	NET_INFO			m_NetInfo;              //以太网信息
	COM_INFO			m_ComInfo;				//串口信息
	SC_COMM_INFO        m_ScCommInfo;			//卫星通信接口相关信息
	int m_FrameType; //标示帧的类型，即水位帧，雨量帧
	int m_LPARME;    //与帧相关的参数，水位中标示传感器，土地墒情中标示深度
	int m_RPARME;    //一些附加参数
	float m_setVar;  //设置值
	CString m_strSetVar;
	BYTE m_buf[300]; //帧缓存区
	int m_bufLen;    //发送长度
	CString m_strStCode;   //站码
	int m_nTimeType; //时间类型
	//struct tm t_a;   //两个时间的时候，t_a作为起始时间
	//struct tm t_b;   //结束时间
	SYSTEMTIME t_a;
	SYSTEMTIME t_b;
	CString m_strPsw;
};

class WriteDataFromFile
{
public:
	WriteDataFromFile(){
		m_strCodeAddr = "";
		memset(m_CodeContext, 0, 300);
		m_nBufLen = 0;
		m_nPackNo = 0;
	}

	~WriteDataFromFile(){
		//m_strCodeAddr = "";
		//memset(m_CodeContext, 0, 300);
		//m_nBufLen = 0;
		//m_nPackNo = 0;
	}

public:
	CString m_strCodeAddr;
	BYTE m_CodeContext[300];
	int m_nBufLen;
	int m_nPackNo;



};

typedef struct _LOG_BUFFER_STRUCT_      //报文日志信息
{
	BYTE m_bArrow ;						//上下行信息
	BYTE m_bBuffer[RECV_BUFFER_LEN];	//报文数据
	WORD m_bLen;                        //报文长度

	CString m_strBuf;                   //格式化后的字符串报文信息
	_LOG_BUFFER_STRUCT_()
	{
		m_bArrow = UP_ARROW;
		memset(m_bBuffer,0,sizeof(m_bBuffer));
		m_bLen = 0;

		m_strBuf = "";
	}

}LOG_BUFFER_STRUCT,*PLOG_BUFFER_STRUCT;

//时间标签TP结构
struct _TP
{
	BYTE m_second;				//秒
	BYTE m_minute;				//分
	BYTE m_hour;				//时
	BYTE m_day;					//日
	BYTE m_adelay;				//允许发送传输延时时间 单位min
	_TP& operator = (_TP &_tp);

	_TP()
	{
		m_adelay = 0;
		m_second = 0;
		m_minute = 0;
		m_hour = 0;
		m_day = 0;
	}
};


typedef struct SYQ_YS_STRUCT
{
	CString	ename;//要素标示符
	CString name;//编码要素
	BYTE	type;//数据类型 1: C，2：FLOAT
	CString  Unit;//单位描述
	BYTE    datalen1;      //小数点前几个位数
	BYTE    datalen2; //小数点后数据 位数
}SYQ_YS_STRUCT;

class SCSW_TypeMap
{
public:	
	~SCSW_TypeMap();
    static SCSW_TypeMap *GetInstance();

private:
	SCSW_TypeMap();
	static SCSW_TypeMap *_instance;
	void InitMap();

public:
	std::map<CString, int> m_SCSWTypeMap;

};


// 水雨情要素标示映射
class SYQ_TypeMap
{
public:	
	~SYQ_TypeMap();
	static SYQ_TypeMap *GetInstance();

private:
	SYQ_TypeMap();
	static SYQ_TypeMap *_syqinstance;
	void InitMap();

public:
	std::map<CString, SYQ_YS_STRUCT> m_SYQTypeMap;

};


namespace CUdfGlobalFUNC
{
	//判断文件是否存在
	 bool IsFileExist(const CString _strFname);
	 void ConvertTpToSystemtime(_TP* _tp, SYSTEMTIME* pSt);

	//获取运行文件所在的目录
	 CString GetRuningPath();

	//检查某一目录是否存在
	 bool CheckDirExist(const CString strPath);

	//检查数据源是否正确，判断能否与数据库建立连接
	 bool IsCanConnectDatabase(const CString _strDataSource);
	 bool IsCanConnectDatabase(const CString _DSN,const CString _UID, const CString _PWD);

	//生成crc校验编码表
	 void create_crc_table();              

	//查询相应值的CRC校验码
	 void crc_code(int count,unsigned char *p,unsigned char *crc16);

	//BCD码转换
	 BYTE ByteFromBCD(BYTE BCD);  //BCD码转换为十进制数
	 BYTE ByteToBCD(BYTE DEC);    //10进制转换为BCD码 
	 void ToBCD(UINT64 DEC,BYTE* bytes,int bytesCount);

	/*
	* 水资源监控管理系统数据传输规约中指定数据帧的校验采用CRC校验，生成多项式为：
	* X^7+X^6+X^5+X^2+1
	* 基本传输顺序为低位在前，高位在后，所以需要采用反转CRC算法
	* crc76520的算法如下do_crc76520(unsigned char *message, unsigned int len) 
	* message为需要进行校验的发送序列
	* len为需要校验的发送序列的长度（字节）
	*/
	/* 定义生成CRC多项式*/
	 unsigned char do_crc76520(unsigned char *message, unsigned int len);


	 //卫星通讯校验和（异或和校验）
	 unsigned char XORSumCheck(BYTE *message, int len);

	
	 bool IpAddressCheck(CString strIP);									//Ip 地址判断是否有效
	 CString GetCmTypeString(int xType);									//通信方式
	 CString GetInterfaceString(int xType);									//通信接口
	 CString GetParityString(int xType);									//校验方式 
	 CString GetStopBitsString(int xType);									//停止位
	 CString GetSCFactoryString(int xType);									//获取卫星提供厂家
	 void GetTPInfo(_TP* pTP,BYTE* tp);										//获取TP信息
	 CString GetWeekString(WORD wDayOfWeek,BYTE LangType =ZH_CN_LANGUAGE);	//获取星期字符串

	 CString ConvertBytesToString(BYTE* buf,int len);
	 CString CalcTime(struct tm *t, int span, int ret=1);

	 void SetGridCellNormal(CGridCtrl* pGrid, int row,int col);
	 void SetGridCellReadOnly(CGridCtrl* pGrid,int row,int col);
	 const CString GetGridCellText(CGridCtrl* pGrid,int Row,int Col);

	 //报文帧格式分析
	 bool IEC_waterdata_to_APPFRAME(BYTE* Buffer,int len,CPacketFrameFormat* waterFrame);
	 bool SCSW008_hydrologic_to_APPFRAME(BYTE *buffer, int len, CHydrFrameFormat* hydrFrame);
	 bool WRITE_frame_to_APPFRAME(BYTE *buffer, int len, CWriteFrame* hydrFrame);
	 bool SYQ_to_APPFRAME(BYTE *buffer, int len, CSYQFrameFormat* syqFrame);
	 bool SYQ_Down_APPFRAME(BYTE *buffer, int len, CSYQFrameFormat* syqFrame);

	 //------------------------------SCSW008-2011水文测报系统技术规约和协议-----------------------
	 bool CheckCRC_SCSW(BYTE *buffer, int len);
	 WORD CreateCRC_SCSW(BYTE *buffer, int len);
	 bool CheckCRC_Write(BYTE *buffer, int len);
	 WORD CreateCRC_Write(BYTE *buffer, int len);

	 bool CheckCRC_SYQ(BYTE *buffer, int len);
	 WORD CreateCRC_SYQ(BYTE *buffer, int len);


}