#pragma once
#include "ScModuleBase.h"
#include "UdfGlobalFUNC.h"
#include <afxmt.h>


#define MSG_INIT 0
#define MSG_SEND 1

struct _SC_CMD_ITEM
{
	CString Cmd_Name;  //指令名称
	CString Cmd_Info;  //指令信息
};

//国腾电子V4.0 
//从终端到外设
static _SC_CMD_ITEM GTDZ_UP_CMDS[8] ={
	{"功率检测","$GLJC"},
	{"定位信息","$DWXX"},
	{"通信信息","$TXXX"},
	{"IC信息",	"$ICXX"},
	{"自检信息","$ZJXX"},
	{"时间信息","$SJXX"},
	{"版本信息","$BBXX"},
	{"反馈信息","$FKXX"}
};

//国腾电子V4.0 
//从外设到终端
static _SC_CMD_ITEM GTDZ_DOWN_CMDS[] ={
	{"定位申请","$DWSQ"},
	{"通信申请","$TXSQ"},
	{"串口输出","$CKSC"},
	{"IC检测",	"$ICJC"},
	{"系统自检","$XTZJ"},
	{"时间输出","$SJSC"},
	{"版本读取","$BBDQ"}
};

//国腾电子V2.5
//从终端到外设
static _SC_CMD_ITEM GTDZ2_5_UP_CMDS[14]={
	{"终端状态信息","$TSTA"},
	{"通信输出","$COUT"},
	{"输出报文顺序号","$OPMS"},
	{"通信申请成功状态","$CASS"},
	{"超长报文通信输出","$LTCO"},
	{"通信回执输出","$CAOP"},
	{"系统回执输出","$SAOP"},
	{"最终固定用户接收回执确认输出","$NAOP"},
	{"定位查询结果","$PINQ"},
	{"通信查询结果","$CINQ"},
	{"回执查询结果","$QOUT"},
	{"授时信息","$TINF"},
	{"GPS定位信息","$GPSL"},
	{"位置信息","$PINF"}
};

//国腾电子V2.5
//从外设到终端
static _SC_CMD_ITEM GTDZ2_5_DOWN_CMDS[16] ={
	{"查询终端状态","$QSTA"},
	{"设置终端状态","$STST"},
	{"查询报文顺序号","$IQMS"},
	{"定位申请","$PAPP"},
	{"结束连续定位","$PEND"},
	{"遇险通信申请","$DCAP"},
	{"终端到终端通信申请","$TTCA"},
	{"终端到固定通信申请","$TNCA"},
	{"终端查询网管最新报文","$TINC"},
	{"通信回执申请","$CACA"},
	{"超长报文通信申请","$LTCA"},
	{"通信输出成功状态","$COSS"},
	{"查询申请","$QAPP"},
	{"授时申请","$TAPP"},
	{"GPS定位申请","$GPSA"},
	{"结束GPS定位","$GPSE"}
};

//成都 国腾电子V4.0 
//北斗卫星通信处理模块
class CScModule_GTDZ :
	public CScModuleBase
{
public:
	CScModule_GTDZ(void);
	~CScModule_GTDZ(void);
	 
public:
	virtual void RecvMessage(BYTE* buf,int Len);		  //接收数据信息
	virtual bool SendMessage(BYTE* buf,int &len);         //检查是否有信息需要发送发送
private:
	//处理卫星通信报文
	void DealScCommMessage();

	//分析卫星通信报文
	void AnalyzeUpMessage( BYTE* tempbyte, int cmdlen, _SC_CMD_ITEM curCmd );

	void Deal_Up_GLJCMessage( BYTE* tempbyte, int cmdlen ); //{"功率检测","$GLJC"},
	void Deal_Up_DWXXMessage( BYTE* tempbyte, int cmdlen ); //{"定位信息","$DWXX"},
	void Deal_Up_TXXXMessage( BYTE* tempbyte, int cmdlen ); //{"通信信息","$TXXX"},
	void Deal_Up_ICXXMessage( BYTE* tempbyte, int cmdlen ); //{"IC信息",  "$ICXX"},
	void Deal_Up_ZJXXMessage( BYTE* tempbyte, int cmdlen ); //{"自检信息","$ZJXX"},
	void Deal_Up_SJXXMessage( BYTE* tempbyte, int cmdlen ); //{"时间信息","$SJXX"},
	void Deal_Up_BBXXMessage( BYTE* tempbyte, int cmdlen ); //{"版本信息","$BBXX"},
	void Deal_Up_FKXXMessage( BYTE* tempbyte, int cmdlen ); //{"反馈信息","$FKXX"}

public:
	void SndDown_TXSQMessage(BYTE msgType,UINT32 remoteAddr,
		BYTE ackType,BYTE* message,int msgLen);				//通信申请,$TXSQ

private:
	int  m_nOneSendMax;										//一次传输最大电文字节数，国腾电子：默认,77 字节

public:
	void SndDown_DWSQMessage(BYTE msgType,UINT32 gcsjAndtxg,
		UINT32 qysj,UINT16 rzpd);							//定位申请,$DWSQ
	void SndDown_CKSCMessage(BYTE cspl);					//串口输出,$CKSC
	void SndDown_ICJCMessage(BYTE zh);						//IC检查,  $ICJC
	void SndDown_XTZJMessage(UINT16 zjpd);					//系统自检,$XTZJ
	void SndDown_SJSCMessage(UINT16 scpd);					//时间输出,$SJSC
	void SndDown_BBDQMessage();								//版本读取,$BBDQ
private:
	
};

//国腾电子V2.5 
//北斗卫星通信处理模块
class CScModule_GTDZ_2_5 :
	public CScModuleBase
{
public:
	CScModule_GTDZ_2_5(void);
	~CScModule_GTDZ_2_5(void);
public:
	virtual void RecvMessage(BYTE* buf,int Len);
	virtual bool SendMessage(BYTE* buf,int &len);
private:
	//处理卫星通信报文
	//处理卫星通信报文
	void DealScCommMessage();

	//分析卫星通信报文
	void AnalyzeUpMessage( BYTE* tempbyte, int cmdlen, _SC_CMD_ITEM curCmd );

	//接收的报文处理
private:
	void Deal_Up_COUTMessage(BYTE* tempbyte,int cmdlen); //{"通信输出","$COUT"},

	//下发的报文处理函数
public:


};

//短信的处理
class TextMsg_GHT:
	public CScModuleBase
{
public:
	TextMsg_GHT();
	~TextMsg_GHT();

public:
	virtual void RecvMessage(BYTE* buf,int Len);		  //接收数据信息
	virtual bool SendMessage(BYTE* buf,int &len);         //检查是否有信息需要发送发送

	void SendDown_Message(TextMsg *msg, int type);
	void SendDown_Message(BYTE *buf, int len, int type);
	CString BufferToString(BYTE *buf, int len );

	//处理短信通信报文
	bool DealTmCommMessage_GHT();
	bool DealTmCommMessage_GHT_Send();
	void DealTmCommMessage_GHT_Init();
	void Deal_CPMS();//优先短信存储器为SIM卡
	void Deal_CPMSa();
	void Deal_CSMP();
	void Deal_CMGL();
	void Deal_CMGR();
	void Deal_CMGD();
	void Deal_CMGS();
	void Deal_CMGF();
	void Deal_CTMI();
	void SetStCode(int stCode);
	void SetCommInfo(COM_INFO comm_info);
	COM_INFO GetCommInfo();
	int GetStCode();

private:
	int m_nStCode;
	COM_INFO m_commInfo;
	CCriticalSection m_cs;
	BYTE m_FrameBuf[1024];
	BYTE m_FrameBufLen;
	CString m_currCmd;
	
};


class TextMsg_YT:
	public CScModuleBase
{
public:
	TextMsg_YT();
	~TextMsg_YT();

public:
	virtual void RecvMessage(BYTE* buf,int Len);		  //接收数据信息
	virtual bool SendMessage(BYTE* buf,int &len);         //检查是否有信息需要发送发送

	void DealTmCommMessage_YT();

	void SetStCode(int StCode);
	void SetCommInfo(COM_INFO comm_info);
	COM_INFO GetCommInfo();
	int GetStCode();

private:
	COM_INFO m_commInfo;
	int m_nStCode;
};


