#pragma once

#include "UdfGlobalFUNC.h"
#include "Handle_TcpServer.h"
#include "Protocol_TcpHandler.h"
class CRtuStation;
class CProtocol_TcpHandle;

/* Name: Tcp����Client�˿�
   Functaion: ��ͷ�������ͨ�ŵ�Soket����
*/
class CHandle_TcpClient
{
public:
	CHandle_TcpClient(void);
public:
	~CHandle_TcpClient(void);

public:
	void SetSocket(SOCKET s)										{m_Socket = s;}
	void SetTcpServer_Handle(CHandle_TcpServer* pTcpServer)			{m_pListenObj = pTcpServer;}
	void SetOverlappedPlus(POVERLAPPEDPLUS pOl)						{m_pOl = pOl;}
	void SetKeepaliveInterval(u_long _value)						{m_nKeepaliveInterval = _value;}
	void SetClientNetInfo(PNET_INFO pClientNetInfo);

	void SetCmType(BYTE newCmType)									{if (m_CmType != newCmType) m_CmType = newCmType;}
	const BYTE GetCmType()											{return m_CmType;}

	SOCKET GetSocket()												{return m_Socket;}
	CHandle_TcpServer* GetTcpServer_Handle()						{return m_pListenObj;}
	CProtocol_TcpHandle* GetProtocolTcpHandle()                     {return m_pProtocolHandle;}
	POVERLAPPEDPLUS	GetOverleppedPlus()								{return m_pOl;}
	PNET_INFO GetClientNetInfo()									{return &m_Client_NetInfo;}

	int  SendData(BYTE* buffer,int len);				//�·�����
	void InitProtocolMoudle(int nProtocolType);
public:
	SOCKET m_Socket;									//SOCKET Handle
	CHandle_TcpServer* m_pListenObj;					//Lisent Obj
	CProtocol_TcpHandle*   m_pProtocolHandle;             //Э�鴦��ģ��
	CSL427_TcpHandle  *m_pSL427_TcpHandle;
	CSCSW_TcpHandle   *m_pSCSW_TcpHandle;
	CSYQ_TcpHandle   *m_pSYQ_TcpHandle;

	
	POVERLAPPEDPLUS	   m_pOl;							//Ol pointer		
	NET_INFO		   m_Client_NetInfo;				//�ͻ���ip��ַ�Ͷ˿ں�	

	BYTE     RECV_Buf[4096];							//���ձ��Ļ�����
	WORD     RECV_Len;									//���ĳ���

	//��ȷ֡��������
	unsigned char FRAME_BUF[SEND_BUFFER_LEN];			//֡����
	unsigned int  FRAME_Len;							//һ֡����

	BYTE		  m_CmType;								//ͨ�ŷ�ʽ

	SYSTEMTIME m_lastRecvTime;							//Last Recv time;
	u_long     m_nKeepaliveInterval;                    //Keep alive interval (second)
protected:
	CRtuStation*	  m_pRelate_RtuStation;				//ͨ����������ң���ն˻�

public:
	void	SetRelateStation(CRtuStation* pSub);
	CRtuStation* GetRelateStation();
public:
	PSYSTEMTIME GetLastRecvTime();
public:
	void        CloseSocket();		
	bool        SetKeepAliveProperty();                                   //����Keep alive ���Բ���,ʵʱ���TCP�����Ƿ����


};
