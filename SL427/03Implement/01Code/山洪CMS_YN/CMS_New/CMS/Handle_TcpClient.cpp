#include "StdAfx.h"
#include "CMS.h"
#include "Handle_TcpClient.h"
#include "Protocol_TcpHandler.h"
#include "RtuStation.h"

struct TCP_KEEPALIVE {
	u_long  onoff;
	u_long  keepalivetime;
	u_long  keepaliveinterval;
} ;

#define SIO_KEEPALIVE_VALS _WSAIOW(IOC_VENDOR,4)


CHandle_TcpClient::CHandle_TcpClient(void)
{
	m_pProtocolHandle = NULL;
	m_pSYQ_TcpHandle = new CSYQ_TcpHandle(this);
	m_pSL427_TcpHandle = new CSL427_TcpHandle(this);
	m_pSCSW_TcpHandle = new CSCSW_TcpHandle(this);
	
	m_pListenObj = NULL;
	m_pOl		 = NULL;
	m_Socket     = INVALID_SOCKET;
	m_CmType	 = 0;

	memset(RECV_Buf,0,sizeof(RECV_Buf));
	RECV_Len = 0;
	m_pRelate_RtuStation = NULL;
	m_nKeepaliveInterval = 5;

	GetLocalTime(&m_lastRecvTime);
}

CHandle_TcpClient::~CHandle_TcpClient(void)
{

	try
	{
		if(m_pRelate_RtuStation != NULL)
		{
			if (m_pRelate_RtuStation->SetCommState(Disconnect_State) == true)  //状态有改变，更新CMSRightView的显示
				m_pRelate_RtuStation->GetSysDataTrans()->UpdateRtuCommState(m_pRelate_RtuStation);

			m_pRelate_RtuStation->SetHandle_TcpClient(NULL);
		}
		

		if (m_Socket != INVALID_SOCKET)
		{
			shutdown(m_Socket,SD_BOTH);
			closesocket(m_Socket);
		}

		m_Socket = INVALID_SOCKET;
	}
	catch (...)
	{

	}
}

void CHandle_TcpClient::CloseSocket()
{
	if (m_Socket != INVALID_SOCKET)
	{
		shutdown(m_Socket,SD_BOTH);
		closesocket(m_Socket);
	}

	m_Socket = INVALID_SOCKET;

}



PSYSTEMTIME CHandle_TcpClient::GetLastRecvTime()
{
	return &m_lastRecvTime;
}


void CHandle_TcpClient::SetClientNetInfo(PNET_INFO pClientNetInfo)
{
	if (pClientNetInfo != NULL)
	{
		m_Client_NetInfo.mIpAddress = pClientNetInfo->mIpAddress;
		m_Client_NetInfo.mPort = pClientNetInfo->mPort;
	}
}



void CHandle_TcpClient::SetRelateStation(CRtuStation* pSub)
{
	m_pRelate_RtuStation = pSub;
}


CRtuStation* CHandle_TcpClient::GetRelateStation()
{
	return m_pRelate_RtuStation;
}


int  CHandle_TcpClient::SendData(BYTE* buffer,int len)
{
	if (!buffer  ||
		len <=0) return 0;

	int nSendCnt = 0;
	if (m_Socket != INVALID_SOCKET)
	{
		nSendCnt = len;
		DWORD dwSendBytes = 0;
		DWORD dwFlags = 0;

		SOCKET s = m_Socket;
		char pBuf[1024];
		WSABUF wsaBuf;
		memcpy(pBuf,(char*)buffer,len);
		wsaBuf.buf = reinterpret_cast<char*>(pBuf);
		wsaBuf.len = len;

		if(WSASend(s, &(wsaBuf), 1, &dwSendBytes, dwFlags, NULL, NULL) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != ERROR_IO_PENDING)
			{
				
			}
		}
	}
	return nSendCnt;
}

bool CHandle_TcpClient::SetKeepAliveProperty()
{
	//KeepAlive实现
	TCP_KEEPALIVE inKeepAlive = {0}; //输入参数
	unsigned long ulInLen = sizeof(TCP_KEEPALIVE);

	TCP_KEEPALIVE outKeepAlive = {0}; //输出参数
	unsigned long ulOutLen = sizeof(TCP_KEEPALIVE);

	unsigned long ulBytesReturn = 0;


	//设置socket的keep alive为5秒，并且发送次数为3次
	inKeepAlive.onoff = 1; 
	inKeepAlive.keepaliveinterval  = m_nKeepaliveInterval * 1000;			//两次KeepAlive探测间的时间间隔
	inKeepAlive.keepalivetime      = m_nKeepaliveInterval * 1000;			//开始首次KeepAlive探测前的TCP空闭时间

	if (WSAIoctl((unsigned int)m_Socket, SIO_KEEPALIVE_VALS,
		(LPVOID)&inKeepAlive, ulInLen,
		(LPVOID)&outKeepAlive, ulOutLen,
		&ulBytesReturn, NULL, NULL) == SOCKET_ERROR) 
	{
		return false;
	}

	return true;
}

void CHandle_TcpClient::InitProtocolMoudle(int nProtocolType)
{
	switch(nProtocolType)
	{
	case WaterResources_Protocol:
		m_pProtocolHandle = m_pSL427_TcpHandle;
		break;
	case SCSW_008_Protocol:
		m_pProtocolHandle = m_pSCSW_TcpHandle;
		break;
	case SYQYC_Comm_Protocol:
		m_pProtocolHandle = m_pSYQ_TcpHandle;
		break;
	default:
		break;

	}

}