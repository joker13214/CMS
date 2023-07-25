#pragma once

#include "UdfGlobalFUNC.h"

/* Name: Tcp服务Listen端口
   Functaion: 提高Server端等待Client连接
*/
class CHandle_TcpServer
{
public:
	CHandle_TcpServer(void);
public:
	~CHandle_TcpServer(void);
public:
	bool InitTcpServer();				//启动Tcp Listen服务，初始Socket,并建立bind监听
	void DealConnectionEvent();			//处理客户端连接事件，响应Accept事件，并与IO完成端口建立联系
	void SetCmType(BYTE newCmType)	{if (m_CmType != newCmType) m_CmType = newCmType;}
	const BYTE GetCmType()			{return m_CmType;}

	void SetTcpServerNetInfo(PNET_INFO pNetInfo);
	PNET_INFO GetTcpServerNetInfo()	{return &m_NetInfo;}
protected:
	SOCKET		m_Socket;				//Socket Handle
	NET_INFO	m_NetInfo;				//Listen端信息，IP地址和端口号
	BYTE		m_CmType;				//通信方式
};
