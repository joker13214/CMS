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
/* ���巴ת���ɶ���ʽ*/
#define ENCPY_REV 0xA7 /* 10100111 */

//����Ա�������������
static CString _Alert_Options_Name[18]=
{
	"����������ͣ�籨��",
	"���ص�ѹ����",
	"ˮλ���ޱ���",
	"ˮѹ���ޱ���",
	"ˮ�ʳ��ޱ���",
	"�����Ǳ���ϱ���",
	"ˮ�ù��ϱ�����ͣ�Ա�",
	"ʣ��ˮ�������ޱ���",
	"",
	"",
	"�ն�IC�����ܱ���",
	"��ֵ���Ʊ���",
	"ˮ�ù���״̬�Ա�",
	"�ն�����״̬����",
	"",
	"",
	"�������ޱ���",
	"ģ�������ޱ���"
};

enum _Log_level_type
{
	Normal_Log_Message =  1,  //��ͨ
	Warning_log_Message = 2,  //����
	Error_Log_message  =  3,  //����
};

enum TEXTMSG_INIT_TYPE
{
	AT = 0,
	ATE0 = 1,        //�������ģʽ�ر�
	AT_ADD_CGMR = 2, //AT+CGMR ��ѯģ��汾��Ϣ
	AT_ADD_CGSN = 3, //AT+CGSN ��ѯ��Ʒ���к�
	AT_ADD_CIMI = 4, //IMSI,�����ƶ�̨�豸��ʶ����
	AT_ADD_CMEE = 5, //AT+CMEE=2 �����豸����ʱ��״̬������ϸ��ʶError������
	AT_ADD_CPIN_ASK = 6, //AT+CPIN? ���SIM�������Ƿ�����
	AT_ADD_CREG = 7, //AT+CREG=1, ����ģ��CSM����ע����ʾ����GPRS���緢�����ߺ󣬻��Զ��ϱ�+CGREG:2
	AT_ADD_CREG_ASK = 8,//AT+CREG?���GPRS����ע�����
    AT_ADD_CSQ = 9, //
	AT_ADD_CGREG = 10,//AT+CGREG=1, ����ģ��CSM����ע����ʾ����GPRS���緢�����ߺ󣬻��Զ��ϱ�+CGREG:2
	AT_ADD_CGREG_ASK = 11,//AT+CGREG?���GPRS����ע�������������������ʱѭ�����ͣ��˽�ģ��ע������״̬
	AT_ADD_CGATT_ASK = 12, //���GPRS���źͷ���
	AT_ADD_CLIP = 13,  //�������������ʾ����
	AT_ADD_CNMI = 14,  //���ö��Ž����ϱ��Ļ��ƣ��������Ҫ���Ź��ܿ��Բ�����
	AT_ADD_CMGF = 15,  //����SMS��ʽ��0ΪPDUģʽ��1Ϊ�ı�ģʽ
	AT_ADD_CSCA_ASK = 16 //�������ĺ��룬һ�㿨�ڶ��У�����Ҫ�������á�

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

//��ˮ��־��ϢItem
typedef struct _LOG_ITEM_
{
	SYSTEMTIME m_Time;			//ʱ����Ϣ
	CString    m_Message;		//������Ϣ
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
	BYTE    bCount;      //������λ��
	BYTE    bPointCount; //С����λ��
	char    Units[20];   //��λ
}WATER_QU_STRUCT;

typedef struct _CMS_GLOBAL_LOG_OPTIONS_
{
	
	BYTE CmStatusSave_Flag;		//RTU ͨ�� ״̬��־ �洢��־  0 ���� ,1 �洢
	BYTE CmBufferSave_Flag;		//RTU ͨ�� ������־ �洢��־  0 ���� ,1 �洢
	BYTE YcChangeSave_Flag;		//RTU ң�� ��λ��־ �洢��־  0 ���� ,1 �洢
	BYTE YxChangeSave_Flag;		//RTU ң�� ��λ��־ �洢��־  0 ���� ,1 �洢
	BYTE YkOperatSave_Flag;		//RTU ң�� ������־ �洢��־  0 ���� ,1 �洢
	BYTE ZdOperatSave_Flag;		//RTU ��ֵ ������־ �洢��־  0 ���� ,1 �洢
	BYTE RunningSSave_Flag;		//ϵͳ ����ʱ ��־  �洢��־  0 ���� ,1 �洢
	BYTE OhterRunSave_FLag;		//������־��Ϣ 
	BYTE DataInErrSave_Flag;	//����ʵʱ��������Ϣ �Ĵ洢��־
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

typedef struct _CMS_GLOBAL_DB_OPTIONS_  //���ݿ���ز���
{
	//MIS system SQL server����Դ   ODBC ����Դ
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

//����PW
typedef struct _CMS_GLOBAL_PW_
{
	WORD PW_CALC;                   //��Կ�㷨 0-9
	WORD PW_CODE;					//��ԿCode 0-999
	BYTE PW_BUF[2];                 //��Կ����
	_CMS_GLOBAL_PW_()
	{
		PW_BUF[0] = 0;
		PW_BUF[1] = 0;
		PW_CALC   = 0;
		PW_CODE   = 0;
	}
	void BuildPWBuf();
}CMS_GLOBAL_PW,PCMS_GLOBAL_PW;



//���в�����Ϣ
typedef struct _CMS_GLOBAL_OPTIONS_ 
{
	CString m_strLogFileName;			//��־���ݿ��ļ���(����·��)
	BYTE    m_dwLogSaveMonths;			//��־���ݱ�������,Ĭ��Ϊ2����
	CString m_strCfgFileName;			//������洢���ݿ��ļ���(����·��)
	CString m_strAdDivCodeFilename;		//������������ļ�
	CString m_strCurRunMoudlePath;		//�����ļ�����Ŀ¼
	CString m_strCSThreadsCfgFile;		//Commserver �߳������ļ���
	CString m_strLocalDbName;			//����SQLite�����ļ�����
	CString m_strAppTitie;				//Application Mainframe Title

	//20140812
	CString m_strScCacheDbName;			//��ʱ�洢SCɽ��ͳ�����ݵı��洢δ��ʱʱ�䲻����ʡ�������ݣ�Ĭ��:sc.cache.db

	BYTE    m_bAdminMode;				//Administrator Management Mode
	BYTE    m_TpDelay;					//Tp����ʱ���������λmin(����)
	BYTE    m_ResendInterval;			//�طű���ʱ����,��λSecond���룩 Ĭ��10s
	BYTE    m_DataInterface;			//���ݽӿڣ�1��GM�Զ������ݽӿڣ�2���Ĵ�ʡɽ���ֺ����ݽӿ�
	BYTE    m_DataIType;				//�ӿڷ�ʽ����m_DataInterface= 1ʱ���ã� 1��ֱ�ӷ������ݿ⣬2��ͨ��webservice�洢����
	BYTE    m_WatarLevelIType;			//ˮλ�ӿڷ�ʽ�� 0��ң���ն�ˮλ���͵ķ�ʽ��1:��ʡ����ˮλ���ͷ�ʽ
	BYTE    m_DataBaseType;				//���ݿ����� 1��SQL server��Ĭ�ϣ� 2:Oracle  //20130509 Added 

	BYTE    m_RainPRDFlag;				//�Ƿ�ͳ�ƽ�����ʱ 1��ͳ�ƣ�0����ͳ�ƣ�Ĭ�ϣ� Added:20140811

	CMS_GLOBAL_LOG_OPTIONS m_LogOpts;	//Log System Option;
	CMS_GLOBAL_DB_OPTIONS  m_DbOpts;	//���ݿ���ز���
	CMS_GLOBAL_RTU_OPTIONS m_RtuOpts;	//Rtu��Ϣ����
	CMS_GLOBAL_PW          m_PW;		//��Կ��Ϣ
	DWORD m_nCheckRtuInterval;			//���ң���ն�ͨ��״̬ʱ��������λ�����ӣ�Ĭ�ϣ�5����
	u_long  m_nKeepaliveInterval;       //keepaliveIntervel
	WORD	m_nCheckRepeatInterval;		//����ظ�����ʱ���� ��λ:second,Ĭ��:90s ,(0��300)

	double m_dRainCheckPer;				//���5��15��1hour����������ֵ
	double m_dRainCheckDay;				//���1day�������ļ���ֵ

	//��������ֳ���������ͳ�Ƶ�����
	UINT   m_nCheck5Minutes_Count;		//5��������ͳ��ʱ��check Ĭ��10����
	UINT   m_nCheck15Minutes_Count;		//15��������ͳ��ʱ��check Ĭ��30����
	UINT   m_nCheck1Hour_Count;			//1Сʱ����ͳ��ʱ��check Ĭ��60����
	UINT   m_nCheck1Day_Count;			//1������ͳ��ʱ��check Ĭ��60 * 24����


	//20140812
	DWORD   m_nPreErrCheck;              //�쳣�����жϣ��ڵ�ǰʱ��֮ǰ������  2 * 365 * 24 * 60���� ��2�꣩ 180�� 
	DWORD   m_nLastErrCheck;			 //�쳣�����жϣ��ڵ�ǰʱ��֮�������  20 * 24 * 60 ���ӣ�20�죩

	
	//��鵽����������
	DWORD   m_nScCacheCheckInSecond;	 //������������(��ǰ����ʱ��)��Ĭ�ϣ�10seconde
	DWORD   m_nScCacheCheckOnTimer;		 //��鶨ʱ�� 5s ���,��λ:ms
	DWORD	m_nScCacheSaveOnTimer;		 //��ʱ�滺������ݣ�2����/ÿ����λ��ms


	DWORD   m_nLocalSQLiteSaveInterval;	//����дSQLite���,Ĭ��1000ms (1sдһ��)
 
	int     m_nGSMFunctionEnable ;		//�Ƿ����ö���GSM���ܣ�Ĭ�ϣ�0�������ã� 1������

	DWORD	m_nScStatisticsCheckInterval;	//Sc������ͳ�Ƽ��ʱ����,Ĭ��2s:2000ms
	DWORD   m_nScUpdataCacheCheckInterval;	//sc�����Լ�黺��ʱ�䷶Χ��Ĭ��1Сʱ��60 * 60 *1000

	DWORD  m_nScStatisticsCheck5mEarly;	    //5������ǰͳ��ʱ��,Ĭ����ǰ1����   1*60 ��λ���� 20140905
	DWORD  m_nScStatisticsCheck15mEarly;    //15������ǰͳ��ʱ�䣬Ĭ����ǰ2���� 2*60 ��λ���� 20140905
	DWORD  m_nScStatisticsCheck1hEarly;		//1h��ǰ��ǰͳ��ʱ�䣬Ĭ����ǰ3���� 3*60 ��λ���� 20140905
	DWORD  m_nScStatisticsCheck1dEarly;		//1d��ǰ��ǰͳ��ʱ�䣬Ĭ����ǰ3���� 3*60 ��λ���� 20140905

	//ǰ�û�������ն����� =1�������������նˣ�=0,�������������նˣ�
	//Ŀǰ���ն����Ͱ�����ˮλ��ˮѹ��ˮ����ˮ�ʣ�������������ģ������λ�������¶ȣ���� add by yhp 20160531
	int     m_nWaterLevelEnable ;		//ˮλ���������Ƿ���ʾ��Ĭ�ϣ�0�������ã� 1������
	int     m_nWaterPressurelEnable ;	//ˮѹ���������Ƿ���ʾ��Ĭ�ϣ�0�������ã� 1������
	int     m_nWaterYieldEnable ;		//ˮ�����������Ƿ���ʾ��Ĭ�ϣ�0�������ã� 1������
	int     m_nWaterQualityEnable ;		//ˮ�ʲ��������Ƿ���ʾ��Ĭ�ϣ�0�������ã� 1������
	int     m_nFlowEnable ;				//�������������Ƿ���ʾ��Ĭ�ϣ�0�������ã� 1������
	int     m_nRainGaugeEnable ;		//�������������Ƿ���ʾ��Ĭ�ϣ�0�������ã� 1������
	int     m_nAnalogEnable ;			//ģ�������������Ƿ���ʾ��Ĭ�ϣ�0�������ã� 1������
	int     m_nDispalcementEnable ;		//λ�������������Ƿ���ʾ��Ĭ�ϣ�0�������ã� 1������
	int     m_nTemperatureEnable ;		//�¶Ȳ��������Ƿ���ʾ��Ĭ�ϣ�0�������ã� 1������
	int     m_nElectricityEnable ;		//��Ȳ��������Ƿ���ʾ��Ĭ�ϣ�0�������ã� 1������

	_CMS_GLOBAL_OPTIONS_()
	{
		m_strCfgFileName.Empty();
		m_strLogFileName.Empty();
		m_dwLogSaveMonths = 2;
		m_strCurRunMoudlePath.Empty();
		m_strCSThreadsCfgFile.Empty();
		m_strLocalDbName.Empty();
		m_strScCacheDbName.Empty();
		m_strAppTitie = "CMS - ͨ��ǰ�û�";
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

		m_nWaterLevelEnable = 0;		//ˮλ���������Ƿ���ʾ��Ĭ�ϣ�0�������ã� 1������
		m_nWaterPressurelEnable = 0;	//ˮѹ���������Ƿ���ʾ��Ĭ�ϣ�0�������ã� 1������
		m_nWaterYieldEnable = 0;		//ˮ�����������Ƿ���ʾ��Ĭ�ϣ�0�������ã� 1������
		m_nWaterQualityEnable = 0;		//ˮ�ʲ��������Ƿ���ʾ��Ĭ�ϣ�0�������ã� 1������
		m_nFlowEnable = 0;				//�������������Ƿ���ʾ��Ĭ�ϣ�0�������ã� 1������
		m_nRainGaugeEnable = 0;			//�������������Ƿ���ʾ��Ĭ�ϣ�0�������ã� 1������
		m_nAnalogEnable = 0;			//ģ�������������Ƿ���ʾ��Ĭ�ϣ�0�������ã� 1������
		m_nDispalcementEnable = 0;		//λ�������������Ƿ���ʾ��Ĭ�ϣ�0�������ã� 1������
		m_nTemperatureEnable = 0;		//�¶Ȳ��������Ƿ���ʾ��Ĭ�ϣ�0�������ã� 1������
		m_nElectricityEnable = 0;		//��Ȳ��������Ƿ���ʾ��Ĭ�ϣ�0�������ã� 1������
	}

}CMS_GLOBAL_OPTIONS,*PCMS_GLOBAL_OPTIONS;

//const long	RECV_BUFFER_LEN		= 512;		//���ջ�������󳤶�
const long	RECV_BUFFER_LEN		= 1024;			//���ջ�������󳤶�
const long  SEND_BUFFER_LEN     = 255;			//���ͱ�����󳤶�
const long  READPORT_LEN_PER    = 300;			//��ȡ�������б��ġ����籨�ĵ���󳤶�
const int   RESEND_Count        = 2;            //�ط�����,FCB=3 

const BYTE  ZH_CN_LANGUAGE      = 0x01;         //����
const BYTE  ENGLISH_LANGUAGE	= 0x02;         //Ӣ��


#ifndef _PROTOCOL_TRANS_ARROW_
#define _PROTOCOL_TRANS_ARROW_
	const BYTE   UP_ARROW         = 1;			//���ն˷��������б���
	const BYTE   DOWN_ARROW       = 0;			//������վ���������б���	
#endif

#ifndef _THREAD_STATUS_INFO_					//�߳�����״̬����
#define _THREAD_STATUS_INFO_
	const BYTE  THREAD_STATE_RUNNING = 1;		//��������
	const BYTE  THREAD_STATE_PAUSE   = 2;		//��ͣ������
	const BYTE  THREAD_STATE_STOP    = 0;		//ֹͣ
#endif

#ifndef _PROTOCOL_C_COMMAND_					//���������붨�� ����
#define _PROTOCOL_C_COMMAND_
	const BYTE C_CMD_01_CMD = 1;				//����
	const BYTE C_CMD_02_USERDATA = 2;			//�û�����
	const BYTE C_CMD_03_LINKTEST = 3;			//��·����
	const BYTE C_CMD_04_MSUREPARA = 4;			//�������
	const BYTE C_CMD_05_MSUERSTATE = 5;			//����״̬
	const BYTE C_CMD_06_WLPARA = 6;				//ˮλ����
	const BYTE C_CMD_07_FLOWPARA = 7;			//��������
	const BYTE C_CMD_08_WPPARA = 8;				//ˮѹ����
	const BYTE C_CMD_09_WQPARA = 9;				//ˮ�ʲ���
	const BYTE C_CMD_10_GENERALPARA =10;		//�ۺϲ���
	const BYTE C_CMD_11_RAINPARA = 11;			//��������
	const BYTE C_CMD_12_MONIPARA = 12;			//ģ��������
	const BYTE C_CMD_13_PULSEPARA = 13;			//��Ȳ���
	const BYTE C_CMD_14_DISPLACEMENT = 14;			//��Ȳ��� //add by yhp20160614
	const BYTE C_CMD_15_TEMPERATURE = 15;			//��Ȳ���//add by yhp20160614
#endif

//ͨ��ͨ������
#ifndef _COMM_TYPE_
#define _COMM_TYPE_
	/*
	    ͨ�ŷ�ʽ
	1	GPRSͨ��ͨ��
	2	GSM�ֻ�����ͨ��ͨ��
	3	����ͨ��ͨ��
	4	���̲�ͨ��ͨ��
	5	΢��ͨ��ͨ��
	6	����
	*/
	const BYTE	GRPS_CommChannel = 1;	//1	GPRSͨ��ͨ��
	const BYTE	GSM_CommChannel  = 2;	//2	GSM�ֻ�����ͨ��ͨ��
	const BYTE	SC_CommChannel   = 3;   //3	����ͨ��ͨ��
	const BYTE	USW_CommChannel  = 4;   //4	���̲�ͨ��ͨ��
	const BYTE  MW_CommChannel   = 5;   //5	΢��ͨ��ͨ��
	const BYTE  TEXTMSG_CommChannel = 6;//6 ����
	const BYTE  Other_CommChannel =7;   //6	����
#endif

#ifndef _SCFactoryType_Const_
#define _SCFactoryType_Const_
	const int  SCFactoryType_GTDZ_4_0 = 1;  //���ڵ��ӱ������� V4.0Э��
	const int  SCFactoryType_GTDZ_2_5 = 2;  //���ڵ��ӱ������� V2.5Э��
#endif


#ifndef _TextMsg_Const_
#define _TextMsg_Const_
	const int  SCTextMsgType_GHT = 1;		//���ͨ���Žӿ�
	const int  SCTextMsgType_YT  = 2;		//���ƶ��Žӿ�
#endif

//Ӧ��Э��汾��
#ifndef _APPLICATION_PROTOCOL_VERSION
#define _APPLICATION_PROTOCOL_VERSION
	const BYTE APP_PROTOCOL_V1_0  = 1;		// V1.0 Ӧ��Э��
	const BYTE APP_PROTOCOL_V2_0  = 2;		// V2.0 Ӧ��Э��
#endif

#ifndef _COMM_INTERFACE_TYPE_                   //ͨ�Žӿڶ���
#define _COMM_INTERFACE_TYPE_
	const BYTE   Ethernet_CommInerface = 1;		//��̫���ӿ�
	const BYTE   Serial_CommInterface  = 2;		//���нӿ�
#endif


#ifndef _DATAINTERFACE_TYPE_
#define _DATAINTERFACE_TYPE_
	const BYTE DATAINTERFACE_GM = 1;			//�����â�Զ������ݽӿ�
	const BYTE DATAINTERFACE_SC = 2;			//�Ĵ�ʡɽ���ֺ����ݿ�ӿ�
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


#ifndef _COMM_PROTOCOL_TYPE_					//��̫��ͨ��Э������
#define _COMM_PROTOCOL_TYPE_			
	const BYTE UDP_Protocol   = 0x01;			//UDP ��ʽ
	const BYTE TCP_Protocol	  = 0x02;			//TCP ��ʽ
#endif


#ifndef _COMM_LINK_STATE_
#define _COMM_LINK_STATE_
	const BYTE Disconnect_State		= 0;			//ͨ�ŶϿ�
	const BYTE Connected_State		= 1;			//ͨ������
	const BYTE CommUnknown_State	= 2;			//ͨ��δ֪
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
	WaterResources_Protocol = 1,   //ˮ��Դ
	Hydrology_Protocol      = 2,   //ˮ��
};

enum GRIDCTRL_STYLE
{
	RTUST_STINFO_TYPE = 0,  //��ʾRTUվ��Ϣ
	RTUST_YCINFO_TYPE = 1,  //��ʾRTUվң����Ϣ
	RTUST_YXINFO_TYPE = 2,  //��ʾRTUվң����Ϣ
	RTUST_YKINFO_TYPE = 3,  //��ʾRTUվң����Ϣ
	RTUST_ZDINFO_TYPE = 4,  //��ʾRTUվ��ֵ��Ϣ
};

#define  MFC_GRIDCTRL_HEIGHT			21//18     //GridCtrl�߶�
#define  MFC_GRIDCTRL_CHECKBOX_WIDTH	25	   //CheckBox ���


//������Ϣ
typedef struct _COM_STRUCT_INFO_
{
	CString mComName;		//��������
	int     mBaund;			//������
	BYTE    mDataBits;		//����λ��
	BYTE    mStopBits;		//ֹͣλ��
	BYTE     mParity;       //У�� ��NOPARITY    0 �޼��飩����ODDPARITY   1 ��У�飩����EVENPARITY  2 żУ�飩����MARKPARITY  3���� ��SPACEPARITY 4��
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

//����ӿ� SOCKET ��Ϣ
typedef struct _NET_STRUCT_INFO_
{
	CString mIpAddress;          //IP��ַ
	unsigned short mPort;        //�˿�
	_NET_STRUCT_INFO_& operator = (_NET_STRUCT_INFO_& temp);
	_NET_STRUCT_INFO_()
	{
		mIpAddress = "0.0.0.0";
		mPort  = 0;
	}
}NET_INFO,*PNET_INFO;

//����ͨ����Ϣ;
typedef struct _SC_COMM_INFO_
{
	int  nFactoryType;  //����ͨ�ų���
	UINT32 nScAddress;	//����ͨ�ű��ص�ַ,���ء�Զ�� ����ͨ�ŵ�ַ
	_SC_COMM_INFO_& operator = (_SC_COMM_INFO_& temp);
	_SC_COMM_INFO_()
	{
		nFactoryType = 1;
		nScAddress = 0;
	}

}SC_COMM_INFO,*PSC_COMM_INFO;

//����ͨ����Ϣ;
typedef struct _SC_TEXTMSG_INFO_
{
	int  nFactoryType;  //����ͨ�ų���
	CString nTmAddress;	//����ͨ�ű��ص�ַ,���ء�Զ�� ����ͨ�ŵ�ַ
	_SC_TEXTMSG_INFO_& operator = (_SC_TEXTMSG_INFO_& temp);
	_SC_TEXTMSG_INFO_()
	{
		nFactoryType = 1; //
		nTmAddress = "";
	}

}SC_TEXTMSG_INFO,*PSC_TEXTMSG_INFO;

typedef struct _Hydr_CASH_Configuration_
{
	__int64 m_dSendTimeout;			//Ĭ��1d  24  * 3600 * 1000
	__int64 m_dResponseTimeout;		//Ĭ�� 20m  20 * 1000
	int     m_blASCOrder;			//1: ���� 0:����,Ĭ��(0)
} Hydr_CASH_Configuration,*PHydr_CASH_Configuration;

typedef struct _RTU_STATION_
{
	CString  _strName;					 //Station RTU name
	CString  _strSId;                    //Station's ID
	WORD     _nAddrA1;					 //��ַ������������A1��0-9999��
	BYTE     _bProvinceCode;             //ʡ(ֱϽ�У�����������
	BYTE     _bRegionCode;               //�أ��У���
	BYTE     _bCountyCode;               //����
	DWORD    _nAddrA2;                   //��ַ���ն˻��м̵�ַA2��A3
	BYTE     _nAddrA3;                   //��ַ�򣺿��Ʊ��� 2�� A2Ϊ�ն˵�ַ  5��A2Ϊ�м�վ��ַ��2��5��

	BYTE    _bCmType;					 //ͨ�ŷ�ʽ��GSM,GPRS��
	BYTE    _bInterface;				 //�ӿڷ�ʽ�����ڡ����磩
	COM_INFO  _CmComInfo;				 //ͨ�� ������Ϣ
	NET_INFO  _CmNetInfo;				 //ͨ�� ����ӿ���Ϣ��IP,�˿ںţ�

	SC_COMM_INFO _CmScCommInfo;			 //����ͨ�Žӿڣ�����ͨ�ŵ�ַ�����ң�
	BYTE _nAppProVer;					 //Ӧ��Э��汾�� v1.0 v2.0
	BYTE _nWorkMode;					 //����ģʽ����Ҫ��� ��00B ���ݹ���ģʽ,01B �Ա�����ģʽ,
	_RTU_STATION_()
	{
		_strName = "";
		_strSId = "";
		_nWorkMode = 1;					//Ĭ���Ա�
	}

}RTU_STATION,*PRTU_STATION;

typedef struct _RTU_YC_OBJECT_
{
	CString _strName;					//YC Name
	CString _strSId;					//Station's ID
	CString _strYcObjID;				//Yc Object ID
	unsigned int _nYcCode;				//Yc's Code
	BYTE	_Type;						//Yc Type;
	BYTE    _DisplayBits;				//��ʾλ��
	BYTE	_PointBits;					//��ʾС����λ��
	CString _Unit;						//Yc's Unit
	float	_MaxValue;					//���ֵ
	float   _MinValue;					//��Сֵ
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
	BYTE    _Sizeof;					//������Ŀ�ȣ�Bit
	BYTE	_OrderInItem;				//λ��������ĵڼ�λ��0��ʾ����������
	CString _Unit;						//Unit
	float   _ScaleMax;					//ZD Object(item)'s ScaleMax value
	float	_ScaleMin;					//ZD Object(item)'s ScaleMin value;

}RTU_ZDOBJECT,*PRTU_ZDOBJECT;

struct _ControlC //������
{
	unsigned char _DIR:1;				//DIR  ���䷽�� D7 
	unsigned char _DIV:1;				//DIV  ��ֱ�־λ D6
	unsigned char _FCB:2;				//FCB  ֡����λ D4~D5
	unsigned char _Command:4;			//����������   D3~D0
	unsigned char _DIVS;				//��ּ����� ��_DIV=1�� ����_DIVS�ֽ�
	_ControlC()
	{
		_DIR = 0;
		_DIV = 0;
		_FCB = 0;
		_Command = 0;
		_DIVS = 0;
	}

	BYTE GetControlCByte();              //�������������Ϣת��Ϊ�����ֽ�����(1 BYTE)
};


class _ASDU   //Ӧ�ò� ֡��ʽ
{
public:
	_ASDU();
	~_ASDU();
public:
	unsigned char  _AFN;					//Ӧ�ò㹦����
	std::vector<unsigned char> data;		//������������͸�����Ϣ��AUX(PW��TP)
	//std::vector<unsigned char> _AUX;		//������Ϣ��AUX
};


//����������A1
struct _A1
{
	BYTE  ProvinceCode;						//ʡ��(0-99)
	BYTE  RegionCode;						//�������У���(0-99)
	_A1()
	{
		ProvinceCode = 0;
		RegionCode = 0;
	}
};

class _ADDR_Info
{
public:
	struct _A1 A1;                          //�����ֽڵ�BCD�룬 ���������� A1 4λʮ��������ʾ��ǰ��λΪʡ�룬����λΪ����
	DWORD	   A2;                          //d0-d20 �ն˵�ַ/�м̵�ַ (3 Byte)
    BYTE       A3;                          //���Ʊ��� A3  Ϊ010B(2)ʱ A2Ϊ�ն˵�ַ��  101B(5)ʱ��A2Ϊ�м̵�ַ(2-5)
	_ADDR_Info()
	{
		A1.ProvinceCode = 0;
		A1.RegionCode   = 0;
		A2 = 0;
		A3 = 0;
	}
};

class CRtuStation;
class CPacketFrameFormat						//���ݰ�֡��ʽ
{
public:
	CPacketFrameFormat();
	~CPacketFrameFormat();
public:
	unsigned char		_bFLen;					//�û����ݳ���L   ע��:�������ĳ���Ϊ L + 5


	_ADDR_Info			_addr_info;				//��ַ��A  �ֽ���Ϣ
	struct _ControlC	ControlC;				//������
	_ASDU				_asdu;					//Ӧ�ò��������ʽ

	//std::vector<unsigned char> _ADDR;			//��ַ��A  ����
	//unsigned char _bStart2;					//��ʼ�ַ� (0x68H)
	//unsigned char _bStart1;					//��ʼ�ַ� (0x68H)
	//unsigned char _CS;						//CSRУ��
	//unsigned char _bEnd;						//�����ַ� (0x16h)

	//����
public:
	BYTE				m_CommInterface;        //�ӿ�ģʽ ���ڡ���̫��
	BYTE				m_ProtocolType;			//����ͨ��Э������ UDP or TCP ��ͨ�Žӿ�Ϊ��̫��ģʽʱ����
	NET_INFO			m_NetInfo;              //��̫����Ϣ
	COM_INFO			m_ComInfo;				//������Ϣ
	SC_COMM_INFO        m_ScCommInfo;			//����ͨ�Žӿ������Ϣ
    _SC_TEXTMSG_INFO_   m_TmCommInfo;           //����ͨ�Žӿ������Ϣ��
	CRtuStation*		m_pStation;             //Current Rtu station

	//�ط����ݲ����/���������
protected:
	bool				m_bRepeatType;			//�Ƿ����ظ����ݵı���,Ĭ��Ϊfalse,�����ظ�������
	SYSTEMTIME			m_stRecvTime;			//����ʱ��
public:
	bool   GetRepeatType() const;				//��ȡ Repeat��־
	void   SetRepeatType(bool val);				//���� Repeat��־
	void   UpdateRecvTime(PSYSTEMTIME pTime= NULL);
	PSYSTEMTIME GetRecvTime();
};


class CSmsFrameFormat				//���Ű�֡��ʽ
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
	//����
public:
	BYTE				m_CommInterface;        //�ӿ�ģʽ ���ڡ���̫��
	BYTE				m_ProtocolType;			//����ͨ��Э������ UDP or TCP ��ͨ�Žӿ�Ϊ��̫��ģʽʱ����
	NET_INFO			m_NetInfo;              //��̫����Ϣ
	COM_INFO			m_ComInfo;				//������Ϣ
	SC_COMM_INFO        m_ScCommInfo;			//����ͨ�Žӿ������Ϣ
	_SC_TEXTMSG_INFO_   m_TmCommInfo;           //����ͨ�Žӿ������Ϣ��
	CRtuStation*		m_pStation;             //Current Rtu station
	CString m_setVar;                               

	SYSTEMTIME			m_stRecvTime;			//����ʱ��
	BYTE m_buf[300]; //֡������
	int m_bufLen;    //���ͳ���
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
	int  m_state;//���͵�״̬

};

typedef struct _LOG_BUFFER_STRUCT_      //������־��Ϣ
{
	BYTE m_bArrow ;						//��������Ϣ
	BYTE m_bBuffer[RECV_BUFFER_LEN];	//��������
	WORD m_bLen;                        //���ĳ���

	CString m_strBuf;                   //��ʽ������ַ���������Ϣ
	_LOG_BUFFER_STRUCT_()
	{
		m_bArrow = UP_ARROW;
		memset(m_bBuffer,0,sizeof(m_bBuffer));
		m_bLen = 0;

		m_strBuf = "";
	}

}LOG_BUFFER_STRUCT,*PLOG_BUFFER_STRUCT;

//ʱ���ǩTP�ṹ
struct _TP
{
	BYTE m_second;				//��
	BYTE m_minute;				//��
	BYTE m_hour;				//ʱ
	BYTE m_day;					//��
	BYTE m_adelay;				//�����ʹ�����ʱʱ�� ��λmin
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
	//�ж��ļ��Ƿ����
	 bool IsFileExist(const CString _strFname);
	 void ConvertTpToSystemtime(_TP* _tp, SYSTEMTIME* pSt);

	//��ȡ�����ļ����ڵ�Ŀ¼
	 CString GetRuningPath();

	//���ĳһĿ¼�Ƿ����
	 bool CheckDirExist(const CString strPath);

	//�������Դ�Ƿ���ȷ���ж��ܷ������ݿ⽨������
	 bool IsCanConnectDatabase(const CString _strDataSource);
	 bool IsCanConnectDatabase(const CString _DSN,const CString _UID, const CString _PWD);

	//����crcУ������
	 void create_crc_table();              

	//��ѯ��Ӧֵ��CRCУ����
	 void crc_code(int count,unsigned char *p,unsigned char *crc16);

	//BCD��ת��
	 BYTE ByteFromBCD(BYTE BCD);  //BCD��ת��Ϊʮ������
	 BYTE ByteToBCD(BYTE DEC);    //10����ת��ΪBCD�� 
	 void ToBCD(UINT64 DEC,BYTE* bytes,int bytesCount);

	/*
	* ˮ��Դ��ع���ϵͳ���ݴ����Լ��ָ������֡��У�����CRCУ�飬���ɶ���ʽΪ��
	* X^7+X^6+X^5+X^2+1
	* ��������˳��Ϊ��λ��ǰ����λ�ں�������Ҫ���÷�תCRC�㷨
	* crc76520���㷨����do_crc76520(unsigned char *message, unsigned int len) 
	* messageΪ��Ҫ����У��ķ�������
	* lenΪ��ҪУ��ķ������еĳ��ȣ��ֽڣ�
	*/
	/* ��������CRC����ʽ*/
	 unsigned char do_crc76520(unsigned char *message, unsigned int len);


	 //����ͨѶУ��ͣ�����У�飩
	 unsigned char XORSumCheck(BYTE *message, int len);

	 //GUID
     CString  newGUID();
	
	 bool IpAddressCheck(CString strIP);									//Ip ��ַ�ж��Ƿ���Ч
	 CString GetCmTypeString(int xType);									//ͨ�ŷ�ʽ
	 CString GetInterfaceString(int xType);									//ͨ�Žӿ�
	 CString GetParityString(int xType);									//У�鷽ʽ 
	 CString GetStopBitsString(int xType);									//ֹͣλ
	 CString GetSCFactoryString(int xType);									//��ȡ�����ṩ����
	 CString GetTmFactoryString(int xType);
	 void GetTPInfo(_TP* pTP,BYTE* tp);										//��ȡTP��Ϣ
	 CString GetWeekString(WORD wDayOfWeek,BYTE LangType =ZH_CN_LANGUAGE);	//��ȡ�����ַ���

	 CString ConvertBytesToString(BYTE* buf,int len);

	 void SetGridCellNormal(CGridCtrl* pGrid, int row,int col);
	 void SetGridCellReadOnly(CGridCtrl* pGrid,int row,int col);
	 const CString GetGridCellText(CGridCtrl* pGrid,int Row,int Col);

	 //����֡��ʽ����
	 bool IEC_waterdata_to_APPFRAME(BYTE* Buffer,int len,CPacketFrameFormat* waterFrame);

	 //src dst����������cpValue�ı仯�Ƚ�
	 //nLi :Total length
	 //nPoint: point length
	 //> 1 ,= 0 ,< -1,
	 //2012-12-05
	 int CompareDoubleChange(double src,double dst,double cpValue,int nLi,int nPoint);
	 int CompareFloatChange(float src,float dst,float cpValue,int nLi,int nPoint);


	 //src dst�������ıȽ�
	 //nLi :Total length
	 //nPoint: point length
	 //2012-12-05
	 //> 1 ,= 0 ,< -1,
	 int CompareDouble(double src,double dst,int nLi,int nPoint);
	 int CompareFloat(float src, float dst,int nLi,int nPoint);

	 //�Ƿ����쳣���ݣ��Ա�ʱ�䣩
	 //added 20140812
	 //����ֵ�� true  �쳣����
	 //			false �����쳣����
	 bool CheckErrorData(SYSTEMTIME* pTime);

}