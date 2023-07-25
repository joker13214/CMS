#pragma once
#include "scmodulebase.h"


struct _SC_CMD_ITEM
{
	CString Cmd_Name;  //ָ������
	CString Cmd_Info;  //ָ����Ϣ
};

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

static _SC_CMD_ITEM GTDZ_DOWN_CMDS[] ={
	{"��λ����","$DWSQ"},
	{"ͨ������","$TXSQ"},
	{"�������","$CKSC"},
	{"IC���",	"$ICJC"},
	{"ϵͳ�Լ�","$XTZJ"},
	{"ʱ�����","$SJSC"},
	{"�汾��ȡ","$BBDQ"}
};

static _SC_CMD_ITEM SZTH_UP_CMDS[6] ={
	{"�ն�״̬��Ϣ","$TSTA"},
	{"λ����Ϣ",    "$PINF"},
	{"ͨѶ����ɹ�״̬","$CASS"},
	{"ͨѶ���",	"$COUT"},
	{"ͨѶ��ִ���","$CAOP"},
	{"��ʱ��Ϣ","$TINF"}
};

static _SC_CMD_ITEM SZTH_DOWN_CMDS[] ={
	{"��ѯ�ն�״̬","$QSTA"},
	{"��λ����",    "$PAPP"},
	{"����������λ","$PEND"},
	{"��������",	"$TTCA"},
	{"ͨѶ����ɹ�״̬","$COSS"},
	{"��ʱ����","$TAPP"}
};

//�ɶ� ���ڵ��� ��������ͨ�Ŵ���ģ��
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


//���� ������� ��������ͨ�Ŵ���ģ��
class CScModule_SZTH :
	public CScModuleBase
{
public:
	CScModule_SZTH(void);
	~CScModule_SZTH(void);

public:
	virtual void RecvMessage(BYTE* buf,int Len);		  //����������Ϣ
	virtual bool SendMessage(BYTE* buf,int &len);         //����Ƿ�����Ϣ��Ҫ���ͷ���
private:
	//��������ͨ�ű���
	void DealScCommMessage();

	//��������ͨ�ű���
	void AnalyzeUpMessage( BYTE* tempbyte, int cmdlen, _SC_CMD_ITEM curCmd );

	void Deal_Up_TSTAMessage( BYTE* tempbyte, int cmdlen ); //{"�ն�״̬��Ϣ","$GLJC"},
	void Deal_Up_PINFMessage( BYTE* tempbyte, int cmdlen ); //{"��λ��Ϣ","$DWXX"},
	void Deal_Up_CASSMessage( BYTE* tempbyte, int cmdlen ); //{"ͨ����Ϣ","$TXXX"},
	void Deal_Up_COUTMessage( BYTE* tempbyte, int cmdlen ); //{"IC��Ϣ",  "$ICXX"},
	void Deal_Up_CAOPMessage( BYTE* tempbyte, int cmdlen ); //{"�Լ���Ϣ","$ZJXX"},
	void Deal_Up_TINFMessage( BYTE* tempbyte, int cmdlen ); //{"ʱ����Ϣ","$SJXX"},

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



