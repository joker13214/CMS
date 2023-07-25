#include "StdAfx.h"
#include "CMS.h"
#include "ScModuleBase.h"
#include "CommServer.h"

CScModuleBase::CScModuleBase(void)
{
	m_nAddressLocal = 0;
	m_nFactoryType = 0;
	memset(m_RECV_BUF,0,sizeof(m_RECV_BUF));
	m_RECV_Len = 0;
	m_bSendBefore = false;
	m_pCommThreadInfo = NULL;
	m_nMaxSendCount = 100;
}

CScModuleBase::~CScModuleBase(void)
{
	std::map<UINT,CSCMessageDealModule*>::iterator Itr;
	for (Itr = m_mapSCMessageDeal.begin();
		Itr != m_mapSCMessageDeal.end();
		Itr ++)
	{
		if (Itr->second)
			delete Itr->second;
	}
	m_mapSCMessageDeal.clear();


	std::list<PsendObject>::iterator sndItr;
	for (sndItr = m_lstSendObjects.begin();
		 sndItr != m_lstSendObjects.end();
		 sndItr ++)
	{
		PsendObject pSndObj = (PsendObject)(*sndItr);
		if (pSndObj)
		{
			delete pSndObj;
			pSndObj = NULL;
		}
	}
	m_lstSendObjects.clear();
}

//void CScModuleBase::RecvMessage( BYTE* buf,int Len )
//{
//}
//
//bool CScModuleBase::SendMessage( BYTE* buf,int &len )
//{
//	return true;
//}

void CScModuleBase::SetScCommInfo( PSC_COMM_INFO pInfo )
{
	if (pInfo)
	{
		m_nAddressLocal = pInfo->nScAddress;
		m_nFactoryType  = pInfo->nFactoryType;
	}
}

void CScModuleBase::AppendRecvData( BYTE* buf,int Len )
{
	if (buf && Len > 0)
	{
		int a = sizeof(m_RECV_BUF)/sizeof(BYTE);
		if (Len + m_RECV_Len > sizeof(m_RECV_BUF)/sizeof(BYTE))
		{
			BYTE tempbuf[1024 * 10];
			memset(tempbuf,0, sizeof(tempbuf));

			int l = Len + m_RECV_Len - 1024 * 10;
			memmove(tempbuf,&m_RECV_BUF[l],m_RECV_Len -l);
			
			m_RECV_Len = m_RECV_Len - l;
			memset(m_RECV_BUF,0,sizeof(m_RECV_BUF));
			memmove(m_RECV_BUF,tempbuf,m_RECV_Len);            
		}

		memmove(&m_RECV_BUF[m_RECV_Len],buf,Len);
		m_RECV_Len += Len;
	}
}

void CScModuleBase::AddLog( CString strLog,PSYSTEMTIME pTime/*= NULL*/ )
{
	strLog.Trim();
	if (strLog != "")
	{
		SYSTEMTIME st;
		if (pTime == NULL)
		{
			GetLocalTime(&st);
			pTime = &st;
		}

		GETAPP()->SystemLog(RTU_CM_BUFFER_LOG,strLog,Normal_Log_Message,pTime);
	}
}

void CScModuleBase::SetCommThreadInfo( CCommThreadInfo* pCommThreadInfo )
{
	m_pCommThreadInfo = pCommThreadInfo;
}

CCommThreadInfo* CScModuleBase::GetCommThreadInfo()
{
	 return m_pCommThreadInfo;
}

PsendObject CScModuleBase::GetOneSendObject()
{
	PsendObject pSndObj = NULL;
	m_cs_SendObjects.Lock();
	if (m_lstSendObjects.size() >0)
	{
		std::list<PsendObject>::iterator FrontItr = m_lstSendObjects.begin();
		if (FrontItr != m_lstSendObjects.end())
		{
			pSndObj = (PsendObject)(*FrontItr);
		}
		m_lstSendObjects.pop_front();
	}
	m_cs_SendObjects.Unlock();
	return pSndObj;
}

void CScModuleBase::AddOneSendObject( PsendObject pSndObj )
{
	if(pSndObj)
	{
		m_cs_SendObjects.Lock();
		if (m_lstSendObjects.size() > 0 && 
			m_lstSendObjects.size() >= m_nMaxSendCount)
		{
			std::list<PsendObject>::iterator FrontItr = m_lstSendObjects.begin();
			if(FrontItr != m_lstSendObjects.end())
			{
				PsendObject pTemp = (PsendObject)(*FrontItr);
				if(pTemp)
				{
					delete pTemp;
					pTemp = NULL;
				}
			}

			m_lstSendObjects.pop_front();
		}

		m_lstSendObjects.push_back(pSndObj);
		m_cs_SendObjects.Unlock();
		
	}
}

const UINT32 CScModuleBase::GetAddressLocal()
{
	return m_nAddressLocal;
}

const int CScModuleBase::GetFactoryType()
{
	return m_nFactoryType;
}

void CScModuleBase::RemoveScMessgaeModule( CSCMessageDealModule* pOldScMessageModule )
{
	if (pOldScMessageModule != NULL)
	{
		std::map<UINT,CSCMessageDealModule*>::iterator findItr = m_mapSCMessageDeal.find(pOldScMessageModule->GetRemoteAddress());
		if (findItr != m_mapSCMessageDeal.end())
			m_mapSCMessageDeal.erase(findItr);

		delete pOldScMessageModule;
		pOldScMessageModule = NULL;

	}
}


