#pragma once
#include "scmodulebase.h"


struct _SC_CMD_ITEM
{
	CString Cmd_Name;  //指令名称
	CString Cmd_Info;  //指令信息
};

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

static _SC_CMD_ITEM GTDZ_DOWN_CMDS[] ={
	{"定位申请","$DWSQ"},
	{"通信申请","$TXSQ"},
	{"串口输出","$CKSC"},
	{"IC检测",	"$ICJC"},
	{"系统自检","$XTZJ"},
	{"时间输出","$SJSC"},
	{"版本读取","$BBDQ"}
};

static _SC_CMD_ITEM SZTH_UP_CMDS[6] ={
	{"终端状态信息","$TSTA"},
	{"位置信息",    "$PINF"},
	{"通讯申请成功状态","$CASS"},
	{"通讯输出",	"$COUT"},
	{"通讯回执输出","$CAOP"},
	{"授时信息","$TINF"}
};

static _SC_CMD_ITEM SZTH_DOWN_CMDS[] ={
	{"查询终端状态","$QSTA"},
	{"定位申请",    "$PAPP"},
	{"结束连续定位","$PEND"},
	{"数据内容",	"$TTCA"},
	{"通讯输出成功状态","$COSS"},
	{"授时申请","$TAPP"}
};

//成都 国腾电子 北斗卫星通信处理模块
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


//北京 神州天鸿 北斗卫星通信处理模块
class CScModule_SZTH :
	public CScModuleBase
{
public:
	CScModule_SZTH(void);
	~CScModule_SZTH(void);

public:
	virtual void RecvMessage(BYTE* buf,int Len);		  //接收数据信息
	virtual bool SendMessage(BYTE* buf,int &len);         //检查是否有信息需要发送发送
private:
	//处理卫星通信报文
	void DealScCommMessage();

	//分析卫星通信报文
	void AnalyzeUpMessage( BYTE* tempbyte, int cmdlen, _SC_CMD_ITEM curCmd );

	void Deal_Up_TSTAMessage( BYTE* tempbyte, int cmdlen ); //{"终端状态信息","$GLJC"},
	void Deal_Up_PINFMessage( BYTE* tempbyte, int cmdlen ); //{"定位信息","$DWXX"},
	void Deal_Up_CASSMessage( BYTE* tempbyte, int cmdlen ); //{"通信信息","$TXXX"},
	void Deal_Up_COUTMessage( BYTE* tempbyte, int cmdlen ); //{"IC信息",  "$ICXX"},
	void Deal_Up_CAOPMessage( BYTE* tempbyte, int cmdlen ); //{"自检信息","$ZJXX"},
	void Deal_Up_TINFMessage( BYTE* tempbyte, int cmdlen ); //{"时间信息","$SJXX"},

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



