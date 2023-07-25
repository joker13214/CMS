#include "StdAfx.h"
#include "IOCP_Server.h"

CIOCP_Server::CIOCP_Server(void)
{
	m_hCompletionPort = NULL;
}

CIOCP_Server::~CIOCP_Server(void)
{
}

bool CIOCP_Server::StartIOCPServer()
{
	m_hCompletionPort = NULL;
	m_hCompletionPort = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,NULL,0);
	if (!m_hCompletionPort )
	{
		TRACE("Create IoCompletionPort Error!\n");
		return false;
	}

	SYSTEM_INFO SystemInfo;
	DWORD i = 0;
	DWORD dwThreadID = 0;


	GetSystemInfo(&SystemInfo);

	m_dwThreadsCnt = SystemInfo.dwNumberOfProcessors * 2;
	for (i = 0; i < m_dwThreadsCnt; i ++)
	{
		IOCP_Thread_Info* pIocpThread = new IOCP_Thread_Info();
		pIocpThread->m_bThreadControl = true;
		pIocpThread->m_pIOCPServer = this;
		pIocpThread->m_ThreadIndex = i;

		HANDLE threadHandle = ::CreateThread(NULL,0,IOCPServerThreadFunction,pIocpThread,0,&dwThreadID);
		if (threadHandle == NULL)
		{
			TRACE("CreateThread() failed with error %d\n",GetLastError());
			delete  pIocpThread;
			pIocpThread = NULL;

			return false;
		}

		m_lstIocpThreads.AddTail(pIocpThread);

		CloseHandle(threadHandle);
	}

	return true;
}


bool CIOCP_Server::StopIOCPServer()
{
	POSITION rPos = m_lstIocpThreads.GetHeadPosition();
	while (rPos != NULL)
	{
		PIOCP_Thread_Info pIocp_threadInfo = m_lstIocpThreads.GetNext(rPos);
		if (pIocp_threadInfo)
			pIocp_threadInfo->m_bThreadControl = false;
	}
	
	::PostQueuedCompletionStatus(m_hCompletionPort,0,NULL,NULL);
 
	WSACleanup();

	if (m_hCompletionPort != NULL)
	{
		CloseHandle(m_hCompletionPort);
		m_hCompletionPort   = NULL;
	}


	m_csClientObjects_lst.Lock();
	while( !m_lstClientObjects.IsEmpty() )
	{
		CHandle_TcpClient* pClientObj = m_lstClientObjects.RemoveHead();
		if  (pClientObj)
		{
			delete pClientObj;
			pClientObj = NULL;
		}
	}
	m_csClientObjects_lst.Unlock();

	

	return true;
}


CIOCP_Server* CIOCP_Server::Instance()
{
	static CIOCP_Server _instance;
	return &_instance;
}

CList<CHandle_TcpClient*>* CIOCP_Server::GetClientObjects_List()
{
	return &m_lstClientObjects;
}

void CIOCP_Server::AddClientObject(CHandle_TcpClient* pClientObj)
{
	if (pClientObj != NULL)
	{
		m_csClientObjects_lst.Lock();
		m_lstClientObjects.AddTail(pClientObj);
		m_csClientObjects_lst.Unlock();
	}
}

void CIOCP_Server::DelClientObject(CHandle_TcpClient* pClientObj)
{
	if (pClientObj != NULL)
	{
		m_csClientObjects_lst.Lock();
		POSITION rFindPos = NULL;
		bool     blFinded = false;
		
		POSITION rPos = m_lstClientObjects.GetHeadPosition();
		while (rPos != NULL)
		{
			CHandle_TcpClient* pTemp = m_lstClientObjects.GetAt(rPos);
			if (pTemp && pTemp == pClientObj)
			{
				rFindPos = rPos;
				blFinded = true;
				break;
			}

			m_lstClientObjects.GetNext(rPos);
		}

		if (blFinded == true && 
			rFindPos != NULL)
		{
			m_lstClientObjects.RemoveAt(rFindPos);

			if(pClientObj->GetOverleppedPlus() != NULL)
			{
				POVERLAPPEDPLUS pOver = pClientObj->GetOverleppedPlus();
				delete pOver;
				pOver = NULL;
				pClientObj->SetOverlappedPlus(NULL);
			}

			delete pClientObj;
			pClientObj = NULL;
		}

		m_csClientObjects_lst.Unlock();
	}
}

unsigned long CIOCP_Server::IOCPServerThreadFunction(void* pPARAM)
{
	IOCP_Thread_Info* pIocp_ThreadInfo = (IOCP_Thread_Info*)pPARAM;
	if (!pIocp_ThreadInfo)
		return 0;

	CIOCP_Server* pIOCP_Server =pIocp_ThreadInfo->m_pIOCPServer ;
	if (!pIOCP_Server) 
		return  0;

	pIocp_ThreadInfo->m_bThreadState = true;
	while(pIocp_ThreadInfo->m_bThreadControl == true)
	{
		OVERLAPPEDPLUS*   pOlp = NULL;
		DWORD dwBytesTransferred = 0;
		CHandle_TcpClient* pClientObj = NULL;

		BOOL bRet = GetQueuedCompletionStatus(pIOCP_Server->GetIOCompletionPort_Handle(), 
			&dwBytesTransferred, (LPDWORD)&pClientObj, (LPOVERLAPPED*)&pOlp, INFINITE);

		if ( !bRet )						//connection  error
		{
			pIOCP_Server->DelClientObject(pClientObj);
		}
		else if (0 == dwBytesTransferred)  //connection closed
		{
			pIOCP_Server->DelClientObject(pClientObj);
		}
		else 
		{
			if (pOlp != NULL  && pClientObj != NULL)
			{
				switch(pOlp->nOpCode)
				{
				case FD_READ:
					{
						pClientObj->Deal_RecvData((BYTE*)pOlp->pBuf,(WORD)dwBytesTransferred);
						
						//接收的数据报文信息
						//printf("%s\n", pOlp->wsaBuf.buf);
						
						WSARecv(pOlp->socket, &(pOlp->wsaBuf), 1, &(pOlp->dwBytes), &(pOlp->dwFlags), &(pOlp->ol), NULL);
					}
					break;
				default:  	// "no data!!";
					;
				}
			}
		}
	}

	pIocp_ThreadInfo->m_bThreadState = false;

	return 0;
}
 