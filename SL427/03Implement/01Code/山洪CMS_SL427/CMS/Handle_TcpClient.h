#pragma once

#include "UdfGlobalFUNC.h"
#include "Handle_TcpServer.h"
class CRtuStation;

/* Name: Tcp服务Client端口
   Functaion: 与客服端连接通信的Soket连接
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

	void Deal_RecvData(BYTE* buffer,WORD len);			//从IOCP线程中获取报文信息通过此函数处理
	int  SendData(BYTE* buffer,int len);				//下发报文
protected:
	SOCKET m_Socket;									//SOCKET Handle
	CHandle_TcpServer* m_pListenObj;					//Lisent Obj
	POVERLAPPEDPLUS	   m_pOl;							//Ol pointer		
	NET_INFO		   m_Client_NetInfo;				//客户端ip地址和端口号	

	BYTE     RECV_Buf[4096];							//接收报文缓冲区
	WORD     RECV_Len;									//报文长度

	//正确帧报文数据
	unsigned char FRAME_BUF[SEND_BUFFER_LEN];			//帧报文
	unsigned int  FRAME_Len;							//一帧长度

	BYTE		  m_CmType;								//通信方式

	SYSTEMTIME m_lastRecvTime;							//Last Recv time;
	u_long     m_nKeepaliveInterval;                    //Keep alive interval (second)
protected:
	CRtuStation*	  m_pRelate_RtuStation;				//通信所关联的遥测终端机

	BOOL enable_linkframe();							//检查接收的报文是否有正确帧格式的报文
public:
	void AddSendMessage(BYTE* buffer,WORD len,PSYSTEMTIME pSt = NULL);    //添加下发日志信息到日志系统
	void AddRecvMessage(BYTE* buffer,WORD Len,PSYSTEMTIME pSt = NULL);	  //添加上送日志信息到日志系统

	void	SetRelateStation(CRtuStation* pSub);
	CRtuStation* GetRelateStation();
public:
	PSYSTEMTIME GetLastRecvTime();
public:
	void        CloseSocket();		
	bool        SetKeepAliveProperty();                                   //设置Keep alive 属性参数,实时监测TCP连接是否可用


};
