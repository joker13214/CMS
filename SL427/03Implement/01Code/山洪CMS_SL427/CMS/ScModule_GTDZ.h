#pragma once
#include "ScModuleBase.h"
#include "UdfGlobalFUNC.h"
#include <afxmt.h>


#define MSG_INIT 0
#define MSG_SEND 1

struct _SC_CMD_ITEM
{
	CString Cmd_Name;  //ָ������
	CString Cmd_Info;  //ָ����Ϣ
};

//���ڵ���V4.0 
//���ն˵�����
static _SC_CMD_ITEM GTDZ_UP_CMDS[8] ={
	{"���ʼ��","$GLJC"},
	{"��λ��Ϣ","$DWXX"},
	{"ͨ����Ϣ","$TXXX"},
	{"IC��Ϣ",	"$ICXX"},
	{"�Լ���Ϣ","$ZJXX"},
	{"ʱ����Ϣ","$SJXX"},
	{"�汾��Ϣ","$BBXX"},
	{"������Ϣ","$FKXX"}
};

//���ڵ���V4.0 
//�����赽�ն�
static _SC_CMD_ITEM GTDZ_DOWN_CMDS[] ={
	{"��λ����","$DWSQ"},
	{"ͨ������","$TXSQ"},
	{"�������","$CKSC"},
	{"IC���",	"$ICJC"},
	{"ϵͳ�Լ�","$XTZJ"},
	{"ʱ�����","$SJSC"},
	{"�汾��ȡ","$BBDQ"}
};

//���ڵ���V2.5
//���ն˵�����
static _SC_CMD_ITEM GTDZ2_5_UP_CMDS[14]={
	{"�ն�״̬��Ϣ","$TSTA"},
	{"ͨ�����","$COUT"},
	{"�������˳���","$OPMS"},
	{"ͨ������ɹ�״̬","$CASS"},
	{"��������ͨ�����","$LTCO"},
	{"ͨ�Ż�ִ���","$CAOP"},
	{"ϵͳ��ִ���","$SAOP"},
	{"���չ̶��û����ջ�ִȷ�����","$NAOP"},
	{"��λ��ѯ���","$PINQ"},
	{"ͨ�Ų�ѯ���","$CINQ"},
	{"��ִ��ѯ���","$QOUT"},
	{"��ʱ��Ϣ","$TINF"},
	{"GPS��λ��Ϣ","$GPSL"},
	{"λ����Ϣ","$PINF"}
};

//���ڵ���V2.5
//�����赽�ն�
static _SC_CMD_ITEM GTDZ2_5_DOWN_CMDS[16] ={
	{"��ѯ�ն�״̬","$QSTA"},
	{"�����ն�״̬","$STST"},
	{"��ѯ����˳���","$IQMS"},
	{"��λ����","$PAPP"},
	{"����������λ","$PEND"},
	{"����ͨ������","$DCAP"},
	{"�ն˵��ն�ͨ������","$TTCA"},
	{"�ն˵��̶�ͨ������","$TNCA"},
	{"�ն˲�ѯ�������±���","$TINC"},
	{"ͨ�Ż�ִ����","$CACA"},
	{"��������ͨ������","$LTCA"},
	{"ͨ������ɹ�״̬","$COSS"},
	{"��ѯ����","$QAPP"},
	{"��ʱ����","$TAPP"},
	{"GPS��λ����","$GPSA"},
	{"����GPS��λ","$GPSE"}
};

//�ɶ� ���ڵ���V4.0 
//��������ͨ�Ŵ���ģ��
class CScModule_GTDZ :
	public CScModuleBase
{
public:
	CScModule_GTDZ(void);
	~CScModule_GTDZ(void);
	 
public:
	virtual void RecvMessage(BYTE* buf,int Len);		  //����������Ϣ
	virtual bool SendMessage(BYTE* buf,int &len);         //����Ƿ�����Ϣ��Ҫ���ͷ���
private:
	//��������ͨ�ű���
	void DealScCommMessage();

	//��������ͨ�ű���
	void AnalyzeUpMessage( BYTE* tempbyte, int cmdlen, _SC_CMD_ITEM curCmd );

	void Deal_Up_GLJCMessage( BYTE* tempbyte, int cmdlen ); //{"���ʼ��","$GLJC"},
	void Deal_Up_DWXXMessage( BYTE* tempbyte, int cmdlen ); //{"��λ��Ϣ","$DWXX"},
	void Deal_Up_TXXXMessage( BYTE* tempbyte, int cmdlen ); //{"ͨ����Ϣ","$TXXX"},
	void Deal_Up_ICXXMessage( BYTE* tempbyte, int cmdlen ); //{"IC��Ϣ",  "$ICXX"},
	void Deal_Up_ZJXXMessage( BYTE* tempbyte, int cmdlen ); //{"�Լ���Ϣ","$ZJXX"},
	void Deal_Up_SJXXMessage( BYTE* tempbyte, int cmdlen ); //{"ʱ����Ϣ","$SJXX"},
	void Deal_Up_BBXXMessage( BYTE* tempbyte, int cmdlen ); //{"�汾��Ϣ","$BBXX"},
	void Deal_Up_FKXXMessage( BYTE* tempbyte, int cmdlen ); //{"������Ϣ","$FKXX"}

public:
	void SndDown_TXSQMessage(BYTE msgType,UINT32 remoteAddr,
		BYTE ackType,BYTE* message,int msgLen);				//ͨ������,$TXSQ

private:
	int  m_nOneSendMax;										//һ�δ����������ֽ��������ڵ��ӣ�Ĭ��,77 �ֽ�

public:
	void SndDown_DWSQMessage(BYTE msgType,UINT32 gcsjAndtxg,
		UINT32 qysj,UINT16 rzpd);							//��λ����,$DWSQ
	void SndDown_CKSCMessage(BYTE cspl);					//�������,$CKSC
	void SndDown_ICJCMessage(BYTE zh);						//IC���,  $ICJC
	void SndDown_XTZJMessage(UINT16 zjpd);					//ϵͳ�Լ�,$XTZJ
	void SndDown_SJSCMessage(UINT16 scpd);					//ʱ�����,$SJSC
	void SndDown_BBDQMessage();								//�汾��ȡ,$BBDQ
private:
	
};

//���ڵ���V2.5 
//��������ͨ�Ŵ���ģ��
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
	//��������ͨ�ű���
	//��������ͨ�ű���
	void DealScCommMessage();

	//��������ͨ�ű���
	void AnalyzeUpMessage( BYTE* tempbyte, int cmdlen, _SC_CMD_ITEM curCmd );

	//���յı��Ĵ���
private:
	void Deal_Up_COUTMessage(BYTE* tempbyte,int cmdlen); //{"ͨ�����","$COUT"},

	//�·��ı��Ĵ�����
public:


};

//���ŵĴ���
class TextMsg_GHT:
	public CScModuleBase
{
public:
	TextMsg_GHT();
	~TextMsg_GHT();

public:
	virtual void RecvMessage(BYTE* buf,int Len);		  //����������Ϣ
	virtual bool SendMessage(BYTE* buf,int &len);         //����Ƿ�����Ϣ��Ҫ���ͷ���

	void SendDown_Message(TextMsg *msg, int type);
	void SendDown_Message(BYTE *buf, int len, int type);
	CString BufferToString(BYTE *buf, int len );

	//�������ͨ�ű���
	bool DealTmCommMessage_GHT();
	bool DealTmCommMessage_GHT_Send();
	void DealTmCommMessage_GHT_Init();
	void Deal_CPMS();//���ȶ��Ŵ洢��ΪSIM��
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
	virtual void RecvMessage(BYTE* buf,int Len);		  //����������Ϣ
	virtual bool SendMessage(BYTE* buf,int &len);         //����Ƿ�����Ϣ��Ҫ���ͷ���

	void DealTmCommMessage_YT();

	void SetStCode(int StCode);
	void SetCommInfo(COM_INFO comm_info);
	COM_INFO GetCommInfo();
	int GetStCode();

private:
	COM_INFO m_commInfo;
	int m_nStCode;
};


