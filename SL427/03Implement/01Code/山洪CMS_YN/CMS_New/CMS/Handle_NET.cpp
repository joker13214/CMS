#include "StdAfx.h"
#include "CMS.h"
#include "UdfGlobalFUNC.h"
#include "Handle_NET.h"
#include "RtuStation.h"

#pragma warning(disable:4018)

CHandle_NET::CHandle_NET(void)
{
	m_NET_PORT= INVALID_SOCKET;
	memset(RECV_BUF,0,sizeof(RECV_BUF));
	RECV_len = 0;

	memset(FRAME_BUF,0,sizeof(FRAME_BUF));
}

CHandle_NET::~CHandle_NET(void)
{
	while (!m_lstRelateStations.IsEmpty())
	{
		CRtuStation* pSub = m_lstRelateStations.RemoveHead();
		if (pSub)
		{
			if (pSub->SetCommState(Disconnect_State) == true)
				pSub->GetSysDataTrans()->UpdateRtuCommState(pSub);
			pSub->SetHandle_Net(NULL);
		}
	}

	if (m_NET_PORT != INVALID_SOCKET)
		closesocket(m_NET_PORT);
}


BOOL CHandle_NET::initNET(unsigned long address,unsigned short port)
{
	if(m_NET_PORT!=INVALID_SOCKET)
	{
		closesocket(m_NET_PORT);
	}
	m_NET_PORT = socket(AF_INET,SOCK_DGRAM,0);
	BOOL Error = FALSE;
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = address;
	if(bind(m_NET_PORT,(sockaddr*)&addr,sizeof(addr))==SOCKET_ERROR)
		Error = TRUE;
	int timeout = 50;
	setsockopt(m_NET_PORT,SOL_SOCKET,SO_RCVTIMEO,(const char*)&timeout,sizeof(timeout));
	setsockopt(m_NET_PORT,SOL_SOCKET,SO_SNDTIMEO,(const char*)&timeout,sizeof(timeout));
	if(Error!=FALSE)
	{
		closesocket(m_NET_PORT);
		m_NET_PORT = INVALID_SOCKET;
		return FALSE;
	}
	return TRUE;
}

BOOL CHandle_NET::initNET(CString name,unsigned short port)
{
	return initNET(inet_addr(name),port);
}


unsigned long CHandle_NET::lookNETBuf()
{
	if(m_NET_PORT!=INVALID_SOCKET)
	{
		fd_set fd;
		timeval timeout={0,0/*1*1000*/};//2ms
		FD_ZERO(&fd);
		FD_SET(m_NET_PORT,&fd);

		int ret = select(1,&fd,0,0,&timeout);
		if(FD_ISSET(m_NET_PORT,&fd)!=0)
			return 1;
	}
	return 0;
}

void CHandle_NET::sendNET(unsigned char *buffer,long len,CString addr,unsigned short port)
{
	if(INVALID_SOCKET!=m_NET_PORT)
	{
		sockaddr_in addr_net;
		addr_net.sin_family = AF_INET;
		addr_net.sin_port = htons(port);
		addr_net.sin_addr.s_addr = inet_addr(addr);
		sendto(m_NET_PORT,(char *)buffer,len,0,(sockaddr *)&addr_net,sizeof(addr_net));
	}
}

unsigned long CHandle_NET::recvNET(unsigned char *buffer,long len)
{
	unsigned long active_len = 0;
	if(INVALID_SOCKET!=m_NET_PORT)
	{
		active_len=recvfrom(m_NET_PORT,(char*)buffer,len,0,0,0);
		int errCode = WSAGetLastError();
	}
	return active_len;
}

unsigned long CHandle_NET::recvNET(unsigned char *buffer,long len,CString& FromIp,unsigned short& FromPort)
{
	unsigned long active_len = 0;
	FromIp ="";
	FromPort = 0;
	
	if(INVALID_SOCKET!=m_NET_PORT)
	{
		sockaddr_in SenderAddr;
		int SenderAddrSize = sizeof(SenderAddr);

		active_len=recvfrom(m_NET_PORT,(char*)buffer,READPORT_LEN_PER,0,(SOCKADDR *)&SenderAddr, 
			&SenderAddrSize);

		
		FromIp.Format("%s",inet_ntoa(SenderAddr.sin_addr));
		FromPort = ntohs(SenderAddr.sin_port);

		int errCode = WSAGetLastError();
	}
	return active_len;
}

BOOL CHandle_NET::enframe_link(BYTE protocolType)
{
	if ( protocolType == (BYTE)WaterResources_Protocol)
	{
		return enframe_link_WaterResources();
	}
	else if (protocolType == (BYTE)Hydrology_Protocol)
	{
		return enframe_link_Hydrology();
	}
	else
		return FALSE;

	return TRUE;
}


BOOL CHandle_NET::enframe_link_Hydrology()
{
	if (RECV_len > 5)
	{
		int pos_BEGIN,pos_END;

		pos_BEGIN = -1;
		pos_END = -1;

		for (int idx = 0; idx < RECV_len; idx ++)
		{
			if (RECV_BUF[idx] == 0x68 &&
				RECV_BUF[idx + 2] == 0x68 &&
				RECV_BUF[idx + 1] != 0)
			{
				int appframeLen = RECV_BUF[idx+1];     //长度
				if (RECV_len > idx + appframeLen +4)
				{
					if (CUdfGlobalFUNC::do_crc76520(&RECV_BUF[idx + 3],appframeLen)  == RECV_BUF[idx + appframeLen + 3]  &&
						RECV_BUF[idx + appframeLen + 4] == 0x68)
					{
						pos_BEGIN = idx;
						pos_END   = idx + appframeLen + 4;
						ZeroMemory(FRAME_BUF,sizeof(FRAME_BUF));
						memmove(FRAME_BUF,&RECV_BUF[idx],appframeLen + 5);
						FRAME_Len = appframeLen + 5;

						BYTE temp[RECV_BUFFER_LEN];
						ZeroMemory(temp,sizeof(temp));
						memmove(temp,&RECV_BUF[pos_END +1],RECV_len -(pos_END +1));
						ZeroMemory(RECV_BUF,sizeof(RECV_BUF));
						memmove(RECV_BUF,temp,RECV_len -(pos_END +1));

						RECV_len = RECV_len -(pos_END +1);

						return TRUE;
					}
				}
			}
		}
	}

	return FALSE;
}


BOOL CHandle_NET::enframe_link_WaterResources()
{
	if (RECV_len > 5)
	{
		int pos_BEGIN,pos_END;

		pos_BEGIN = -1;
		pos_END = -1;

		for (int idx = 0; idx < RECV_len; idx ++)
		{
			if (RECV_BUF[idx] == 0x68 &&
				RECV_BUF[idx + 2] == 0x68 &&
				RECV_BUF[idx + 1] != 0)
			{
				int appframeLen = RECV_BUF[idx+1];     //长度
				if (RECV_len > idx + appframeLen +4)
				{
					if (CUdfGlobalFUNC::do_crc76520(&RECV_BUF[idx + 3],appframeLen)  == RECV_BUF[idx + appframeLen + 3]  &&
						RECV_BUF[idx + appframeLen + 4] == 0x68)
					{
						pos_BEGIN = idx;
						pos_END   = idx + appframeLen + 4;
						ZeroMemory(FRAME_BUF,sizeof(FRAME_BUF));
						memmove(FRAME_BUF,&RECV_BUF[idx],appframeLen + 5);
						FRAME_Len = appframeLen + 5;

						BYTE temp[RECV_BUFFER_LEN];
						ZeroMemory(temp,sizeof(temp));
						memmove(temp,&RECV_BUF[pos_END +1],RECV_len -(pos_END +1));
						ZeroMemory(RECV_BUF,sizeof(RECV_BUF));
						memmove(RECV_BUF,temp,RECV_len -(pos_END +1));

						RECV_len = RECV_len -(pos_END +1);

						return TRUE;
					}
				}
				else
				{
					return FALSE;
				}
			}
		}
	}
	return FALSE;
}


int CHandle_NET::getFrameBuffer(BYTE* frBuf)
{
	if (!frBuf)
		return 0;

	memmove(frBuf,FRAME_BUF,FRAME_Len);

	return FRAME_Len;
}


BOOL CHandle_NET::CheckFrameBuffer(BYTE* buffer,int nLen)
{
	if (!buffer)
		return  FALSE;

	if (nLen > 5)
	{
		if (buffer[0] == 0x68 &&
			buffer[2] == 0x68 &&
			buffer[1] != 0)
		{
			int appframeLen = buffer[1];     //长度
			if (nLen = appframeLen + 5)
			{
				if (CUdfGlobalFUNC::do_crc76520(&buffer[3],appframeLen)  == buffer[nLen - 2]  &&
					buffer[nLen - 1] == 0x16)
				{
					return TRUE;
				}
			}
		}
	}


	return FALSE;
}


void CHandle_NET::AddRelateStation(CRtuStation* pSub)
{
	if (pSub)
	{
		POSITION rPos = m_lstRelateStations.GetHeadPosition();

		bool blFinded = false;
		while (rPos != NULL)
		{
			CRtuStation* pTemp = m_lstRelateStations.GetNext(rPos);
			if (pTemp && pTemp == pSub)
			{
				blFinded = true;
				break;
			}
		}

		if (blFinded == false)
		{
			m_lstRelateStations.AddTail(pSub);
		}
	}
}

void CHandle_NET::RemoveRelateStation(CRtuStation* pSub)
{
	if (pSub)
	{
		POSITION rPos = m_lstRelateStations.GetHeadPosition();

		bool blFinded = false;
		POSITION rFindPos = NULL;

		while (rPos != NULL)
		{
			CRtuStation* pTemp = m_lstRelateStations.GetAt(rPos);
			if (pTemp && pTemp == pSub)
			{
				blFinded = true;
				rFindPos  =  rPos;
				break;
			}

			m_lstRelateStations.GetNext(rPos);

		}


		if (blFinded == true && 
			rFindPos  != NULL)
		{
			m_lstRelateStations.RemoveAt(rFindPos);
		}
	}
}



void CHandle_NET::AddSendMessage(BYTE* buffer,WORD len,CString toIp,unsigned short toPort, PSYSTEMTIME pSt /* = NULL */)
{
	if (!buffer ||
		len <=0 ) return;

	CString strNetInfo="",strArrow  = "",strLength="";
	strNetInfo.Format("UDP,DST_IP=(%s,%d),",toIp,toPort);
	strArrow = "发 ,";
	strLength.Format("长度=%-3d,报文=",len);

	CString strBuf="",strTemp="";
	for (WORD idx=0;idx <len;idx ++)
	{
		strTemp.Format("%02X ",buffer[idx]);
		strBuf += strTemp;
	}

	CString strResult = "";

	strResult.Format("%s%s%s%s",strArrow,strNetInfo,strLength,strBuf);

	GETAPP()->SystemLog(RTU_CM_BUFFER_LOG,strResult,Normal_Log_Message,pSt);
}