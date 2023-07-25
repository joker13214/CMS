#pragma once

#include "UdfGlobalFUNC.h"
#include "Handle_TcpServer.h"
class CRtuStation;

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
	POVERLAPPEDPLUS	GetOverleppedPlus()								{return m_pOl;}
	PNET_INFO GetClientNetInfo()									{return &m_Client_NetInfo;}

	void Deal_RecvData(BYTE* buffer,WORD len);			//��IOCP�߳��л�ȡ������Ϣͨ���˺�������
	int  SendData(BYTE* buffer,int len);				//�·�����
protected:
	SOCKET m_Socket;									//SOCKET Handle
	CHandle_TcpServer* m_pListenObj;					//Lisent Obj
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

	BOOL enable_linkframe();							//�����յı����Ƿ�����ȷ֡��ʽ�ı���
public:
	void AddSendMessage(BYTE* buffer,WORD len,PSYSTEMTIME pSt = NULL);    //����·���־��Ϣ����־ϵͳ
	void AddRecvMessage(BYTE* buffer,WORD Len,PSYSTEMTIME pSt = NULL);	  //���������־��Ϣ����־ϵͳ

	void	SetRelateStation(CRtuStation* pSub);
	CRtuStation* GetRelateStation();
public:
	PSYSTEMTIME GetLastRecvTime();
public:
	void        CloseSocket();		
	bool        SetKeepAliveProperty();                                   //����Keep alive ���Բ���,ʵʱ���TCP�����Ƿ����


};
