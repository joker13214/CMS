#pragma once

#include <vector>
#include <iostream>
#include <list>
#include <bitset>
#include <map>
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

enum TEXTMSG_INIT_TYPE
{
	AT = 0,
	ATE0 = 1,        //命令回显模式关闭
	AT_ADD_CGMR = 2, //AT+CGMR 查询模块版本信息
	AT_ADD_CGSN = 3, //AT+CGSN 查询产品序列号
	AT_ADD_CIMI = 4, //IMSI,国际移动台设备标识请求
	AT_ADD_CMEE = 5, //AT+CMEE=2 报告设备出错时的状态，即详细标识Error的内容
	AT_ADD_CPIN_ASK = 6, //AT+CPIN? 检查SIM卡工作是否正常
	AT_ADD_CREG = 7, //AT+CREG=1, 设置模块CSM网络注册提示，当GPRS网络发生掉线后，会自动上报+CGREG:2
	AT_ADD_CREG_ASK = 8,//AT+CREG?检查GPRS网络注册情况
    AT_ADD_CSQ = 9, //
	AT_ADD_CGREG = 10,//AT+CGREG=1, 设置模块CSM网络注册提示，当GPRS网络发生掉线后，会自动上报+CGREG:2
	AT_ADD_CGREG_ASK = 11,//AT+CGREG?检查GPRS网络注册情况，建议该命令空闲时循环发送，了解模块注册网络状态
	AT_ADD_CGATT_ASK = 12, //检查GPRS附着和分离
	AT_ADD_CLIP = 13,  //设置来电号码显示功能
	AT_ADD_CNMI = 14,  //设置短信接收上报的机制，如果不需要短信功能可以不设置
	AT_ADD_CMGF = 15,  //设置SMS格式，0为PDU模式、1为文本模式
	AT_ADD_CSCA_ASK = 16 //短信中心号码，一般卡内都有，不需要另外设置。

};

enum TEXTMSG_STATE
{
	SENDING = 0,
	SENT =1,
	FAIL =2,
	SUCCESS = 3
};

#ifndef _W_L_ITYPE_CONST_
#define _W_L_ITYPE_CONST_
	const BYTE W_L_IType_Default = 0;
	const BYTE W_L_IType_SBK	 = 1;
#endif

#ifndef _DATABASE_TYPE_CONST_
#define _DATABASE_TYPE_CONST_
	const byte  SQLServer_Database = 1;
	const byte  Oracle_Database = 2;
#endif

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

	//20140812
	CString m_strScCacheDbName;			//临时存储SC山洪统计数据的表，存储未到时时间不能入省标库的数据，默认:sc.cache.db

	BYTE    m_bAdminMode;				//Administrator Management Mode
	BYTE    m_TpDelay;					//Tp的延时检查数，单位min(分钟)
	BYTE    m_ResendInterval;			//重放报文时间间隔,单位Second（秒） 默认10s
	BYTE    m_DataInterface;			//数据接口，1：GM自定义数据接口，2：四川省山洪灾害数据接口
	BYTE    m_DataIType;				//接口方式，当m_DataInterface= 1时有用， 1：直接访问数据库，2：通过webservice存储数据
	BYTE    m_WatarLevelIType;			//水位接口方式， 0：遥测终端水位上送的方式，1:按省标库的水位上送方式
	BYTE    m_DataBaseType;				//数据库类型 1：SQL server（默认） 2:Oracle  //20130509 Added 

	BYTE    m_RainPRDFlag;				//是否统计降雨历时 1：统计，0：不统计（默认） Added:20140811

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


	//20140812
	DWORD   m_nPreErrCheck;              //异常数据判断，在当前时间之前的数据  2 * 365 * 24 * 60分钟 （2年） 180天 
	DWORD   m_nLastErrCheck;			 //异常数据判断，在当前时间之后的数据  20 * 24 * 60 分钟（20天）

	
	//检查到点入库的数据
	DWORD   m_nScCacheCheckInSecond;	 //到点入库的数据(提前入库的时间)，默认：10seconde
	DWORD   m_nScCacheCheckOnTimer;		 //检查定时器 5s 间隔,单位:ms
	DWORD	m_nScCacheSaveOnTimer;		 //定时存缓存的数据，2分钟/每，单位：ms


	DWORD   m_nLocalSQLiteSaveInterval;	//本地写SQLite间隔,默认1000ms (1s写一次)
 
	int     m_nGSMFunctionEnable ;		//是否启用短信GSM功能，默认：0（不启用） 1：启用

	DWORD	m_nScStatisticsCheckInterval;	//Sc周期性统计检查时间间隔,默认2s:2000ms
	DWORD   m_nScUpdataCacheCheckInterval;	//sc周期性检查缓存时间范围，默认1小时，60 * 60 *1000

	DWORD  m_nScStatisticsCheck5mEarly;	    //5分钟提前统计时间,默认提前1分钟   1*60 单位：秒 20140905
	DWORD  m_nScStatisticsCheck15mEarly;    //15分钟提前统计时间，默认提前2分钟 2*60 单位：秒 20140905
	DWORD  m_nScStatisticsCheck1hEarly;		//1h提前提前统计时间，默认提前3分钟 3*60 单位：秒 20140905
	DWORD  m_nScStatisticsCheck1dEarly;		//1d提前提前统计时间，默认提前3分钟 3*60 单位：秒 20140905

	//前置机处理的终端类型 =1，包含该类型终端，=0,不包含该类型终端，
	//目前的终端类型包括：水位，水压，水量，水质，流量，雨量，模拟量，位移量，温度，电度 add by yhp 20160531
	int     m_nWaterLevelEnable ;		//水位操作界面是否显示，默认：0（不启用） 1：启用
	int     m_nWaterPressurelEnable ;	//水压操作界面是否显示，默认：0（不启用） 1：启用
	int     m_nWaterYieldEnable ;		//水量操作界面是否显示，默认：0（不启用） 1：启用
	int     m_nWaterQualityEnable ;		//水质操作界面是否显示，默认：0（不启用） 1：启用
	int     m_nFlowEnable ;				//流量操作界面是否显示，默认：0（不启用） 1：启用
	int     m_nRainGaugeEnable ;		//雨量操作界面是否显示，默认：0（不启用） 1：启用
	int     m_nAnalogEnable ;			//模拟量操作界面是否显示，默认：0（不启用） 1：启用
	int     m_nDispalcementEnable ;		//位移量操作界面是否显示，默认：0（不启用） 1：启用
	int     m_nTemperatureEnable ;		//温度操作界面是否显示，默认：0（不启用） 1：启用
	int     m_nElectricityEnable ;		//电度操作界面是否显示，默认：0（不启用） 1：启用

	_CMS_GLOBAL_OPTIONS_()
	{
		m_strCfgFileName.Empty();
		m_strLogFileName.Empty();
		m_dwLogSaveMonths = 2;
		m_strCurRunMoudlePath.Empty();
		m_strCSThreadsCfgFile.Empty();
		m_strLocalDbName.Empty();
		m_strScCacheDbName.Empty();
		m_strAppTitie = "CMS - 通信前置机";
		m_TpDelay = 0;
		m_ResendInterval =   0;
		m_nCheckRtuInterval = 0; 
		m_nKeepaliveInterval = 0;
		m_nCheckRepeatInterval = 90;
		m_DataInterface = 1;
		m_DataIType = 1;
		m_WatarLevelIType = 1;
		m_dRainCheckDay = 0.0;
		m_dRainCheckPer = 0.0;
		m_DataBaseType = 1;

		m_RainPRDFlag = 0;	//Added:20140811

		//
		m_nCheck5Minutes_Count = 10;
		m_nCheck15Minutes_Count = 30;
		m_nCheck1Hour_Count = 61;
		m_nCheck1Day_Count = 1440;

		//
		m_nPreErrCheck = 210 * 24 * 60; //2 * 365 * 24 * 60;
		m_nLastErrCheck = 5 * 24 * 60 ; //20 * 24 * 60 ;

		//
		m_nScCacheCheckInSecond = 10;
		m_nScCacheCheckOnTimer = 5000;
		m_nScCacheSaveOnTimer = 2* 60 * 1000;

		m_nLocalSQLiteSaveInterval = 5000;
		m_nGSMFunctionEnable = 0;

		m_nScStatisticsCheckInterval = 2000;
		m_nScUpdataCacheCheckInterval = 60 * 60 * 1000;

		m_nScStatisticsCheck5mEarly = 60;
		m_nScStatisticsCheck15mEarly = 60;
		m_nScStatisticsCheck1hEarly = 60;
		m_nScStatisticsCheck1dEarly = 60;

		m_nWaterLevelEnable = 0;		//水位操作界面是否显示，默认：0（不启用） 1：启用
		m_nWaterPressurelEnable = 0;	//水压操作界面是否显示，默认：0（不启用） 1：启用
		m_nWaterYieldEnable = 0;		//水量操作界面是否显示，默认：0（不启用） 1：启用
		m_nWaterQualityEnable = 0;		//水质操作界面是否显示，默认：0（不启用） 1：启用
		m_nFlowEnable = 0;				//流量操作界面是否显示，默认：0（不启用） 1：启用
		m_nRainGaugeEnable = 0;			//雨量操作界面是否显示，默认：0（不启用） 1：启用
		m_nAnalogEnable = 0;			//模拟量操作界面是否显示，默认：0（不启用） 1：启用
		m_nDispalcementEnable = 0;		//位移量操作界面是否显示，默认：0（不启用） 1：启用
		m_nTemperatureEnable = 0;		//温度操作界面是否显示，默认：0（不启用） 1：启用
		m_nElectricityEnable = 0;		//电度操作界面是否显示，默认：0（不启用） 1：启用
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
	const BYTE C_CMD_14_DISPLACEMENT = 14;			//电度参数 //add by yhp20160614
	const BYTE C_CMD_15_TEMPERATURE = 15;			//电度参数//add by yhp20160614
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
	const BYTE  TEXTMSG_CommChannel = 6;//6 短信
	const BYTE  Other_CommChannel =7;   //6	其他
#endif

#ifndef _SCFactoryType_Const_
#define _SCFactoryType_Const_
	const int  SCFactoryType_GTDZ_4_0 = 1;  //国腾电子北斗卫星 V4.0协议
	const int  SCFactoryType_GTDZ_2_5 = 2;  //国腾电子北斗卫星 V2.5协议
#endif


#ifndef _TextMsg_Const_
#define _TextMsg_Const_
	const int  SCTextMsgType_GHT = 1;		//广和通短信接口
	const int  SCTextMsgType_YT  = 2;		//驿唐短信接口
#endif

//应用协议版本号
#ifndef _APPLICATION_PROTOCOL_VERSION
#define _APPLICATION_PROTOCOL_VERSION
	const BYTE APP_PROTOCOL_V1_0  = 1;		// V1.0 应用协议
	const BYTE APP_PROTOCOL_V2_0  = 2;		// V2.0 应用协议
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
#endif

#ifndef _TextMsgInitInfo_
#define _TextMsgInitInfo_
	const int MsgInit_Recv = 0;
#endif

#ifndef _TextMsgRecvInitType_
#define _TextMsgRecvInitType_
	const int Invaild_cmd = 0;
	const int AT_sending  = 1;
	const int AT_waiting_OK = 2;
	const int AT_OK_finish  = 3;

	const int ATE0_sending = 4;
	const int ATE0_OK_finish = 5;

	const int CGMR_sending = 6;
	const int CGMR_waiting_OK = 7;
	const int CGMR_OK_finish = 8;

	const int CGSN_sending = 9;
	const int CGSN_waiting_OK = 10;
	const int CGSN_OK_finish = 11;

	const int CIMI_sending = 12;
	const int CIMI_waiting_OK = 13;
	const int CIMI_OK_finish = 14;

	const int CMEE_sending = 15;
	const int CMEE_OK_finish = 16;

	const int CPIN_sending = 17;
	const int CPIN_waiting_OK = 18;
	const int CPIN_OK_finish = 19;

	//------------CGEG?----------
    const int CREG_sending = 20;
	const int CREG_waiting_OK = 21;
	const int CREG_OK_finish = 22;

	//-----------CGEG=1-----------
	const int CGEGa_Sending = 23;
	const int CGEGa_OK_finish = 24;

	const int CSQ_sending = 25;
	const int CSQ_waiting_OK = 26;
	const int CSQ_OK_finish = 27;

	//----------CGREG?----------
	const int CGREG_sending = 28;
	const int CGREG_OK_finish = 29;
	const int CGREG_waiting_OK = 30;

	//---------CGREG=1----------
	const int CGREGa_sending = 31;
	const int CGREGa_OK_finish = 32;

	const int CGATT_sending = 33;
	const int CGATT_waiting_OK = 34;
	const int CGATT_OK_finish = 35;
	
	const int CLIP_sending = 36;
	const int CLIP_OK_finish = 37;

	const int CNMI_sending = 38;
	const int CNMI_OK_finish = 39;

	const int CMGF_sending = 40;
	const int CMGF_OK_finish = 41;

	const int CSCA_sending = 42;
	const int CSCA_waiting_OK = 43;
	const int CSCA_OK_finish = 44;
#endif

#ifndef _TextMsgRecvType_
#define _TextMsgRecvType_
	const int Invaild_SendCmd = 0;

	const int CPMS_sending  = 1;
	const int CPMS_waiting_OK = 2;
	const int CPMS_OK_finish  = 3;

	//---------------CPMS?-----------
	const int CPMSa_sending = 4;
	const int CPMSa_waiting_OK = 5;
	const int CPMSa_OK_finish = 6;

	//const int CMGF_sending = 7;
	//const int CMGF_OK_finish = 8;

	const int CSMP_sending = 9;
	const int CSMP_OK_finish = 10;

	const int CMGL_sending = 11;
	const int CMGL_waiting_OK = 12;
	const int CMGL_OK_finish = 13;

	const int CMGD_sending = 14;
	const int CMGD_OK_finish = 15;

	const int CMGS_sending = 17;
	const int CMGS_waiting_Big = 18;
	const int CMGS_sending_afterBig = 19;
	const int CMGS_waiting_CMGS = 20;
	const int CMGS_waiting_OK = 21;
	const int CMGS_OK_finish = 22;

	const int Recv_CMTI = 23;

	const int CMGR_Sending = 24;
	const int CMGR_waiting_OK = 25;
	const int CMGR_OK_finish = 26;

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
};

enum GRIDCTRL_STYLE
{
	RTUST_STINFO_TYPE = 0,  //显示RTU站信息
	RTUST_YCINFO_TYPE = 1,  //显示RTU站遥测信息
	RTUST_YXINFO_TYPE = 2,  //显示RTU站遥信信息
	RTUST_YKINFO_TYPE = 3,  //显示RTU站遥控信息
	RTUST_ZDINFO_TYPE = 4,  //显示RTU站定值信息
};

#define  MFC_GRIDCTRL_HEIGHT			21//18     //GridCtrl高度
#define  MFC_GRIDCTRL_CHECKBOX_WIDTH	25	   //CheckBox 宽度


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

}COM_INFO,*PCOM_INFO;

//网络接口 SOCKET 信息
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

//短信通信信息;
typedef struct _SC_TEXTMSG_INFO_
{
	int  nFactoryType;  //短信通信厂家
	CString nTmAddress;	//卫星通信本地地址,本地、远方 卫星通信地址
	_SC_TEXTMSG_INFO_& operator = (_SC_TEXTMSG_INFO_& temp);
	_SC_TEXTMSG_INFO_()
	{
		nFactoryType = 1; //
		nTmAddress = "";
	}

}SC_TEXTMSG_INFO,*PSC_TEXTMSG_INFO;

typedef struct _Hydr_CASH_Configuration_
{
	__int64 m_dSendTimeout;			//默认1d  24  * 3600 * 1000
	__int64 m_dResponseTimeout;		//默认 20m  20 * 1000
	int     m_blASCOrder;			//1: 正序 0:逆许,默认(0)
} Hydr_CASH_Configuration,*PHydr_CASH_Configuration;

typedef struct _RTU_STATION_
{
	CString  _strName;					 //Station RTU name
	CString  _strSId;                    //Station's ID
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
	}

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

class CRtuStation;
class CPacketFrameFormat						//数据包帧格式
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
    _SC_TEXTMSG_INFO_   m_TmCommInfo;           //短信通信接口相关信息。
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


class CSmsFrameFormat				//短信包帧格式
{
public:
	CSmsFrameFormat()
	{	
		m_CommInterface = Serial_CommInterface;
		m_ProtocolType = UDP_Protocol;
		m_pStation = NULL;
		m_setVar = "";
	}
	~CSmsFrameFormat(){}

	const CSmsFrameFormat &operator = (const CSmsFrameFormat &f){
		m_CommInterface = f.m_CommInterface;
		m_ProtocolType  = f.m_ProtocolType;

		m_NetInfo.mIpAddress       = f.m_NetInfo.mIpAddress;
		m_NetInfo.mPort = f.m_NetInfo.mPort;
		m_ComInfo.mBaund = f.m_ComInfo.mBaund;
		m_ComInfo.mComName = f.m_ComInfo.mComName;
		m_ComInfo.mDataBits= f.m_ComInfo.mDataBits;
		m_ComInfo.mParity= f.m_ComInfo.mParity;
		m_ComInfo.mStopBits= f.m_ComInfo.mStopBits;

		m_pStation = f.m_pStation;
		m_TmCommInfo.nFactoryType    = f.m_TmCommInfo.nFactoryType;
		m_TmCommInfo.nTmAddress      = f.m_TmCommInfo.nTmAddress;
		memcpy(m_buf, f.m_buf, f.m_bufLen);
		return *this;
	}
	//其他
public:
	BYTE				m_CommInterface;        //接口模式 串口、以太网
	BYTE				m_ProtocolType;			//网络通信协议类型 UDP or TCP 当通信接口为以太网模式时有用
	NET_INFO			m_NetInfo;              //以太网信息
	COM_INFO			m_ComInfo;				//串口信息
	SC_COMM_INFO        m_ScCommInfo;			//卫星通信接口相关信息
	_SC_TEXTMSG_INFO_   m_TmCommInfo;           //短信通信接口相关信息。
	CRtuStation*		m_pStation;             //Current Rtu station
	CString m_setVar;                               

	SYSTEMTIME			m_stRecvTime;			//接收时间
	BYTE m_buf[300]; //帧缓存区
	int m_bufLen;    //发送长度
public:
	void   UpdateRecvTime(PSYSTEMTIME pTime= NULL);
	PSYSTEMTIME GetRecvTime();
};

class TextMsg
{
public:
	TextMsg(){
		memset(m_buf, 0, 300);
		m_len = 0;
	}
	~TextMsg(){}

public:
	BYTE m_buf[300];
	int  m_len;
	int  m_Type;
	int  m_state;//发送的状态

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


class TM_TypeMap
{
public:	
	~TM_TypeMap();
	static TM_TypeMap *GetInstance();

private:
	TM_TypeMap();
	static TM_TypeMap *_instance;
	void InitMap();

public:
	std::map<int, TextMsg*> m_TM_TypeMap;

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

	 //GUID
     CString  newGUID();
	
	 bool IpAddressCheck(CString strIP);									//Ip 地址判断是否有效
	 CString GetCmTypeString(int xType);									//通信方式
	 CString GetInterfaceString(int xType);									//通信接口
	 CString GetParityString(int xType);									//校验方式 
	 CString GetStopBitsString(int xType);									//停止位
	 CString GetSCFactoryString(int xType);									//获取卫星提供厂家
	 CString GetTmFactoryString(int xType);
	 void GetTPInfo(_TP* pTP,BYTE* tp);										//获取TP信息
	 CString GetWeekString(WORD wDayOfWeek,BYTE LangType =ZH_CN_LANGUAGE);	//获取星期字符串

	 CString ConvertBytesToString(BYTE* buf,int len);

	 void SetGridCellNormal(CGridCtrl* pGrid, int row,int col);
	 void SetGridCellReadOnly(CGridCtrl* pGrid,int row,int col);
	 const CString GetGridCellText(CGridCtrl* pGrid,int Row,int Col);

	 //报文帧格式分析
	 bool IEC_waterdata_to_APPFRAME(BYTE* Buffer,int len,CPacketFrameFormat* waterFrame);

	 //src dst两个数据与cpValue的变化比较
	 //nLi :Total length
	 //nPoint: point length
	 //> 1 ,= 0 ,< -1,
	 //2012-12-05
	 int CompareDoubleChange(double src,double dst,double cpValue,int nLi,int nPoint);
	 int CompareFloatChange(float src,float dst,float cpValue,int nLi,int nPoint);


	 //src dst两个数的比较
	 //nLi :Total length
	 //nPoint: point length
	 //2012-12-05
	 //> 1 ,= 0 ,< -1,
	 int CompareDouble(double src,double dst,int nLi,int nPoint);
	 int CompareFloat(float src, float dst,int nLi,int nPoint);

	 //是否是异常数据（对比时间）
	 //added 20140812
	 //返回值： true  异常数据
	 //			false 不是异常数据
	 bool CheckErrorData(SYSTEMTIME* pTime);

}