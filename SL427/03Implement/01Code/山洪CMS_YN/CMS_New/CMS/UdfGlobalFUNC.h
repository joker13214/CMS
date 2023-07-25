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

	BYTE    m_bAdminMode;				//Administrator Management Mode
	BYTE    m_TpDelay;					//Tp����ʱ���������λmin(����)
	BYTE    m_ResendInterval;			//�طű���ʱ����,��λSecond���룩 Ĭ��10s
	BYTE    m_DataInterface;			//���ݽӿڣ�1��GM�Զ������ݽӿڣ�2���Ĵ�ʡɽ���ֺ����ݽӿ�n 3: ����ˮ����
	BYTE    m_DataIType;				//�ӿڷ�ʽ����m_DataInterface= 1ʱ���ã� 1��ֱ�ӷ������ݿ⣬2��ͨ��webservice�洢����

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
 
	_CMS_GLOBAL_OPTIONS_()
	{
		m_strCfgFileName.Empty();
		m_strLogFileName.Empty();
		m_dwLogSaveMonths = 2;
		m_strCurRunMoudlePath.Empty();
		m_strCSThreadsCfgFile.Empty();
		m_strLocalDbName.Empty();
		m_strAppTitie = "CMS - ͨ��ǰ�û�";
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
	const BYTE  Other_CommChannel =6;   //6	����
#endif

#ifndef _SCFactoryType_Const_
#define _SCFactoryType_Const_
	const int  SCFactoryType_GTDZ = 1;  //���ڵ��ӱ�������
	const int  SCFactoryType_SZTH = 2;  //������豱������
#endif

//Ӧ��Э��汾��
#ifndef _APPLICATION_PROTOCOL_VERSION
#define _APPLICATION_PROTOCOL_VERSION
	const BYTE APP_PROTOCOL_V1_0  = 1; // V1.0 Ӧ��Э��
	const BYTE APP_PROTOCOL_V2_0  = 2; // V2.0 Ӧ��Э��
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
	const BYTE DATAINTERFACE_YN = 3;          //����ˮ�������ݿ�ӿ�
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
	SCSW_008_Protocol       = 3,
	SYQYC_Comm_Protocol     = 4, //ˮ����ң��ϵͳ����ͨ�Ź�Լ
};

enum GRIDCTRL_STYLE
{
	RTUST_STINFO_TYPE = 0,  //��ʾRTUվ��Ϣ
	RTUST_YCINFO_TYPE = 1,  //��ʾRTUվң����Ϣ
	RTUST_YXINFO_TYPE = 2,  //��ʾRTUվң����Ϣ
	RTUST_YKINFO_TYPE = 3,  //��ʾRTUվң����Ϣ
	RTUST_ZDINFO_TYPE = 4,  //��ʾRTUվ��ֵ��Ϣ
};

#define  MFC_GRIDCTRL_HEIGHT			18     //GridCtrl�߶�
#define  MFC_GRIDCTRL_CHECKBOX_WIDTH	25	   //CheckBox ���

enum SCSW_TIMETYPE
{
	TIMETYPE_TT = 0,//ʱ��
	TIMETYPE_TB = 1,//��ʷ��ʼʱ��
	TIMETYPE_TE = 2,//��ʷ����ʱ��
	TIMETYPE_TM = 3,//���ֵ����ʱ��
	TIMETYPE_TN = 4, //��С��ֵ����ʱ��
	TIMETYPE_CURR = 5
};
enum SCSW_TYPE
{
	SCSW_Z   = 1000,             //ˮλ
	SCSW_PN  = 1001,  			  //����һ
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
	SYQ_DRD  = 1997,//ʱ�䲽�� ��λ��
	SYQ_DRH  = 1998,//ʱ�䲽�� ��λСʱ
	SQY_DRN  = 1999,//ʱ�䲽�� ��λ����
	SYQ_ED   = 2000,//��������
	SYQ_EM   =  2001,//��������
	SYQ_EX   = 2002,//
	SYQ_FDZ  = 2003,//
	SYQ_P1   = 2004,//1hʱ�ν�ˮ�� ���� 
	SYQ_P2   = 2005,//2hʱ�ν�ˮ�� ���� 
	SYQ_P3   = 2006,//3hʱ�ν�ˮ�� ���� 
	SYQ_P6   = 2007,//6hʱ�ν�ˮ�� ���� 
	SYQ_P12  = 2008,//12hʱ�ν�ˮ�� ���� 
	SYQ_PD   = 2009,//�ս�ˮ��
	SYQ_PM   = 2010,//�½�ˮ��
	SYQ_PN01 = 2011,//1minʱ�ν�ˮ�� ���� 
	SYQ_PN05 = 2012,//5minʱ�ν�ˮ�� ���� 
	SYQ_PN10 = 2013,//10minʱ�ν�ˮ�� ���� 
	SYQ_PN30 = 2014,//30minʱ�ν�ˮ�� ���� 
	SYQ_PR   = 2015,//������ ����
	SYQ_PX   = 2016,//Ѯ��ˮ��
	SYQ_Q    = 2017,//˲ʱ��������ˮ����
	SYQ_QA   = 2018,//�ܳ�����������բ������
	SYQ_QAD  = 2019,//��ƽ����������
	SYQ_QAM  = 2020,//��ƽ����������
	SYQ_QAMM = 2020,//������ܳ�����
	SYQ_QAMN = 2021,//����С�ܳ�����
	SYQ_QAX  = 2022,//Ѯƽ����������
	SYQ_QAXM = 2023,//Ѯ����ܳ�����
	SYQ_QAXN = 2024,//Ѯ��С�ܳ�����
	SYQ_QD   = 2025,//�ӵ���բ����ƽ����������վ��ƽ����ˮ����
	SYQ_QI   = 2025,//�������
	SYQ_QID   = 2026,//��ƽ���������
	SYQ_QIM   = 2027,//��ƽ���������
	SYQ_QIMM  = 2028,//������������
	SYQ_QIMN  = 2029,//����С�������
	SYQ_QIX   = 2030,//Ѯƽ���������
	SYQ_QIXM   = 2031,//Ѯ����������
	SYQ_QIXN   = 2032,//Ѯ��С�������

    SYQ_PT  =   2050,//��ˮ���ۼ�ֵ  
};
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

}COM_INFO, *PCOM_INFO;
//����ӿ� SOCKET ��Ϣw
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

typedef struct _RTU_STATION_
{
	CString  _strName;					 //Station RTU name
	CString  _strSId;                    //Station's ID
	CString  _strStCode;                 //����SCSW008�ĵ�ַ��
	int  _strSYQADDR;				     //ˮ�����ַ��
	int      _centercode;				 //ˮ��������վ��
	int    _passWord;                    //ˮ��������
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
		_bAppProType = (BYTE)WaterResources_Protocol;
		_nAppProVer = 1;

	}

	BYTE _bAppProType;                  //Ӧ�ò�Э������

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

class SCSW_ASDU
{
public:
	SCSW_ASDU();
	~SCSW_ASDU();

public:
	unsigned char data[300];

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

//���SCSW�����еľ�����Ϣ
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
	int m_nProtoType;            //���ͱ�ʾ
	int m_nExtraInfo;            //������Ϣ�洢������ĳЩ����֮���������֣���Z01
	std::list<CString> m_OpDataList; //������
	CCriticalSection m_cs;
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

struct _SYQ_CONFIG_PACKET2//ң��վ����2���ð� 
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


struct _SYQ_CONFIG_PACKET3//ң��վ����3���ð� 
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
class CPacketFrameFormat				//���ݰ�֡��ʽ
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
//ˮ��Դ����֡��ʽ����
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
		m_workmode = 1;//����ģʽ

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
	int m_nFrameLen;			//��������֡����
	int m_nDataCnt;             //���ݸ���

public:
	BYTE				m_CommInterface;        //�ӿ�ģʽ ���ڡ���̫��
	BYTE				m_ProtocolType;			//����ͨ��Э������ UDP or TCP ��ͨ�Žӿ�Ϊ��̫��ģʽʱ����
	NET_INFO			m_NetInfo;              //��̫����Ϣ
	COM_INFO			m_ComInfo;				//������Ϣ
	SC_COMM_INFO        m_ScCommInfo;			//����ͨ�Žӿ������Ϣ
	CRtuStation*		m_pStation;             //Current Rtu station

	CCriticalSection          m_cs_info;
	std::list<SCSW_MsgInfo *> m_MsgInfoList;
private:
	SYSTEMTIME			m_stRecvTime;			//����ʱ��
public:
	int  m_RR;
	int  m_workmode;
	int  m_nDivCnt;
	int  m_nStCode;
	CString m_strStCode;
	BYTE m_nDIR;   //����
	int  m_nCot;   //����ԭ��
	int  m_TimeType;//TT, TB, TE, TM, TN
	DateTime m_DataTimeTT;
	DateTime m_DataTimeTB;
	DateTime m_DataTimeTE;
	DateTime m_DataTimeTM;
	DateTime m_DataTimeTN;
};

class CSYQFrameFormat//ˮ���鱨������
{
public:
	CSYQFrameFormat(){m_repeat=3;}
	~CSYQFrameFormat(){}
public:
	BYTE				m_CommInterface;        //�ӿ�ģʽ ���ڡ���̫��
	BYTE				m_ProtocolType;			//����ͨ��Э������ UDP or TCP ��ͨ�Žӿ�Ϊ��̫��ģʽʱ����
	NET_INFO			m_NetInfo;              //��̫����Ϣ
	COM_INFO			m_ComInfo;				//������Ϣ
	SC_COMM_INFO        m_ScCommInfo;			//����ͨ�Žӿ������Ϣ
	CRtuStation*		m_pStation;             //Current Rtu station

	int m_FunType;//�����룬����¼B
	int     m_iPassword;//����
	DWORD   m_stCode;//'ң��վ��
	int     m_CenterCode;//����վ
	int     m_ControlCode;//���Ľ�����
	int   nMsgLen; //�������ĳ���
	int messageno;//��ˮ��
	DateTime7 m_DataTimeSend;//����ʱ��
	DateTime m_DataTimeGet;//�ɼ�ʱ��
	SYSTEMTIME m_stRecvTime;//
	char    m_cType;//������� ����¼A
	int    m_repeat;//�ط�����
	_ASDU				_asdu;					//Ӧ�ò��������ʽ
	int     len;//�������ĳ���

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
	BYTE				m_CommInterface;        //�ӿ�ģʽ ���ڡ���̫��
	BYTE				m_ProtocolType;			//����ͨ��Э������ UDP or TCP ��ͨ�Žӿ�Ϊ��̫��ģʽʱ����
	NET_INFO			m_NetInfo;              //��̫����Ϣ
	COM_INFO			m_ComInfo;				//������Ϣ
	SC_COMM_INFO        m_ScCommInfo;			//����ͨ�Žӿ������Ϣ
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
	SYSTEMTIME	m_stRecvTime;			//����ʱ��
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
	BYTE				m_CommInterface;        //�ӿ�ģʽ ���ڡ���̫��
	BYTE				m_ProtocolType;			//����ͨ��Э������ UDP or TCP ��ͨ�Žӿ�Ϊ��̫��ģʽʱ����
	NET_INFO			m_NetInfo;              //��̫����Ϣ
	COM_INFO			m_ComInfo;				//������Ϣ
	SC_COMM_INFO        m_ScCommInfo;			//����ͨ�Žӿ������Ϣ
	int m_FrameType; //��ʾ֡�����ͣ���ˮλ֡������֡
	int m_LPARME;    //��֡��صĲ�����ˮλ�б�ʾ�����������������б�ʾ���
	int m_RPARME;    //һЩ���Ӳ���
	float m_setVar;  //����ֵ
	CString m_strSetVar;
	BYTE m_buf[300]; //֡������
	int m_bufLen;    //���ͳ���
	CString m_strStCode;   //վ��
	int m_nTimeType; //ʱ������
	//struct tm t_a;   //����ʱ���ʱ��t_a��Ϊ��ʼʱ��
	//struct tm t_b;   //����ʱ��
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


typedef struct SYQ_YS_STRUCT
{
	CString	ename;//Ҫ�ر�ʾ��
	CString name;//����Ҫ��
	BYTE	type;//�������� 1: C��2��FLOAT
	CString  Unit;//��λ����
	BYTE    datalen1;      //С����ǰ����λ��
	BYTE    datalen2; //С��������� λ��
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


// ˮ����Ҫ�ر�ʾӳ��
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

	
	 bool IpAddressCheck(CString strIP);									//Ip ��ַ�ж��Ƿ���Ч
	 CString GetCmTypeString(int xType);									//ͨ�ŷ�ʽ
	 CString GetInterfaceString(int xType);									//ͨ�Žӿ�
	 CString GetParityString(int xType);									//У�鷽ʽ 
	 CString GetStopBitsString(int xType);									//ֹͣλ
	 CString GetSCFactoryString(int xType);									//��ȡ�����ṩ����
	 void GetTPInfo(_TP* pTP,BYTE* tp);										//��ȡTP��Ϣ
	 CString GetWeekString(WORD wDayOfWeek,BYTE LangType =ZH_CN_LANGUAGE);	//��ȡ�����ַ���

	 CString ConvertBytesToString(BYTE* buf,int len);
	 CString CalcTime(struct tm *t, int span, int ret=1);

	 void SetGridCellNormal(CGridCtrl* pGrid, int row,int col);
	 void SetGridCellReadOnly(CGridCtrl* pGrid,int row,int col);
	 const CString GetGridCellText(CGridCtrl* pGrid,int Row,int Col);

	 //����֡��ʽ����
	 bool IEC_waterdata_to_APPFRAME(BYTE* Buffer,int len,CPacketFrameFormat* waterFrame);
	 bool SCSW008_hydrologic_to_APPFRAME(BYTE *buffer, int len, CHydrFrameFormat* hydrFrame);
	 bool WRITE_frame_to_APPFRAME(BYTE *buffer, int len, CWriteFrame* hydrFrame);
	 bool SYQ_to_APPFRAME(BYTE *buffer, int len, CSYQFrameFormat* syqFrame);
	 bool SYQ_Down_APPFRAME(BYTE *buffer, int len, CSYQFrameFormat* syqFrame);

	 //------------------------------SCSW008-2011ˮ�Ĳⱨϵͳ������Լ��Э��-----------------------
	 bool CheckCRC_SCSW(BYTE *buffer, int len);
	 WORD CreateCRC_SCSW(BYTE *buffer, int len);
	 bool CheckCRC_Write(BYTE *buffer, int len);
	 WORD CreateCRC_Write(BYTE *buffer, int len);

	 bool CheckCRC_SYQ(BYTE *buffer, int len);
	 WORD CreateCRC_SYQ(BYTE *buffer, int len);


}