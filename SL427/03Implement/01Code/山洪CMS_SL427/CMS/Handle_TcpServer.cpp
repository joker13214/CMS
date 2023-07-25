#include "StdAfx.h"
#include "Handle_TcpServer.h"
#include "Handle_TcpClient.h"
#include "IOCP_Server.h"
#include "CMS.h"

CHandle_TcpServer::CHandle_TcpServer(void)
{
	m_Socket = INVALID_SOCKET;
	m_CmType = 0;
}

CHandle_TcpServer::~CHandle_TcpServer(void)
{
	if (m_Socket != INVALID_SOCKET)
	{
		shutdown(m_Socket,SD_BOTH);
		closesocket(m_Socket);
		m_Socket = INVALID_SOCKET;
	}
}

void CHandle_TcpServer::SetTcpServerNetInfo(PNET_INFO pNetInfo)
{
	if (pNetInfo != NULL)
	{
		m_NetInfo.mIpAddress = pNetInfo->mIpAddress;
		m_NetInfo.mPort	= pNetInfo->mPort;
	}
}


bool CHandle_TcpServer::InitTcpServer()
{
	m_Socket = WSASocket(AF_INET, SOCK_STREAM, 0, 0, 0, WSA_FLAG_OVERLAPPED);
	if(INVALID_SOCKET == m_Socket)
	{
		TRACE("WSASocket errors!\n");
		return false;
	}

	SOCKADDR_IN addrSvr;
	ZeroMemory(&addrSvr, sizeof(SOCKADDR_IN));
	addrSvr.sin_family = AF_INET;
	addrSvr.sin_port = htons(m_NetInfo.mPort);
	addrSvr.sin_addr.s_addr = inet_addr(m_NetInfo.mIpAddress);
	int nRet = bind(m_Socket, (SOCKADDR*)&addrSvr, sizeof(SOCKADDR));
	if(SOCKET_ERROR == nRet)
	{
		TRACE("bind errors!\n");
		return false;
	}

	nRet = listen(m_Socket, 500);
	if(SOCKET_ERROR == nRet)
	{
		TRACE("listen errors!\n");
		return 0;
	}
	return true;
}


void CHandle_TcpServer::DealConnectionEvent()
{
	if (m_Socket != INVALID_SOCKET)
	{
		
		try
		{	
			fd_set fd;
			timeval timeout={0,0/*1*1000*/};//2ms
			FD_ZERO(&fd);
			FD_SET(m_Socket,&fd);

			int ret = select(1,&fd,0,0,&timeout);
			if(FD_ISSET(m_Socket,&fd) != 0)
			{
				SOCKADDR_IN addrAccept;
				ZeroMemory(&addrAccept, sizeof(SOCKADDR_IN));
				int nSockLen = sizeof(SOCKADDR);
				SOCKET sockAccept = WSAAccept(m_Socket, (SOCKADDR*)&addrAccept, &nSockLen, 0, 0);
				if(INVALID_SOCKET == sockAccept)
				{
					TRACE("WSAAccept errors!\n");
					return;
				}

				CHandle_TcpClient* pNewClient = new CHandle_TcpClient();
				if (pNewClient != NULL)
				{
					NET_INFO ClientNetInfo;
					ClientNetInfo.mIpAddress.Format("%s",inet_ntoa(addrAccept.sin_addr));
					ClientNetInfo.mPort = ntohs(addrAccept.sin_port);

					pNewClient->SetSocket(sockAccept);
					pNewClient->SetClientNetInfo(&ClientNetInfo);
					pNewClient->SetCmType(m_CmType);


					CCMSApp* pApp = (CCMSApp*)AfxGetApp();

					//Set Keep alive Property
					pNewClient->SetKeepaliveInterval(pApp->m_GlobalOptions.m_nKeepaliveInterval);
					pNewClient->SetKeepAliveProperty();


					if(NULL==CreateIoCompletionPort((HANDLE)sockAccept, 
						CIOCP_Server::Instance()->GetIOCompletionPort_Handle(), (ULONG_PTR)pNewClient, 0))
					{
						TRACE("CreateIoCompletionPort errors!\n");
						delete pNewClient;
						pNewClient = NULL;
						return;
					}

					CIOCP_Server::Instance()->AddClientObject(pNewClient);

					OVERLAPPEDPLUS* pOlp = new OVERLAPPEDPLUS;
					ZeroMemory(pOlp, sizeof(OVERLAPPEDPLUS));
					ZeroMemory(&(pOlp->ol), sizeof(OVERLAPPED));
					pOlp->socket = sockAccept;
					pOlp->dwFlags = 0;
					pOlp->wsaBuf.buf = pOlp->pBuf;
					pOlp->wsaBuf.len = 1024;
					pOlp->nOpCode = FD_READ;

					pNewClient->SetOverlappedPlus(pOlp);

					int nRet = WSARecv(pOlp->socket, &(pOlp->wsaBuf), 1, &(pOlp->dwBytes), &(pOlp->dwFlags), &(pOlp->ol), NULL);
					if(SOCKET_ERROR == nRet)
					{
						if(WSAGetLastError() != ERROR_IO_PENDING)
						{

						}
					}
				}
			}
		}
		catch(...)
		{
		}
	}
}