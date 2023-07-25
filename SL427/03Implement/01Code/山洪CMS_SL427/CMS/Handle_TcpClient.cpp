#include "StdAfx.h"
#include "CMS.h"
#include "Handle_TcpClient.h"
#include "RtuStation.h"

struct TCP_KEEPALIVE {
	u_long  onoff;
	u_long  keepalivetime;
	u_long  keepaliveinterval;
} ;

#define SIO_KEEPALIVE_VALS _WSAIOW(IOC_VENDOR,4)


CHandle_TcpClient::CHandle_TcpClient(void)
{
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

void CHandle_TcpClient::Deal_RecvData(BYTE* buffer,WORD len)
{
	if (!buffer  ||
		len <= 0 )
		return;

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	CRealTimeDB* pRealDb = pApp->m_pReadDB;
	if (!pRealDb)  return ;

	GetLocalTime(&m_lastRecvTime);
	AddRecvMessage(buffer,len,&m_lastRecvTime);

	int overed = 0;

	//实际存入缓冲区的字节数
	//溢出原则:后收到的数据丢失
	overed = len > (4096 - RECV_Len) ? (4096 - RECV_Len) : len;//处理缓冲区溢出
	if(overed>0)
	{
		memmove(&RECV_Buf[RECV_Len],buffer,overed);
		RECV_Len += overed;
	}


	//报文处理
	memset(FRAME_BUF,0,sizeof(FRAME_BUF));
	FRAME_Len = 0;
	while(enable_linkframe() != FALSE)
	{
		CPacketFrameFormat waterFrame;
		if (!CUdfGlobalFUNC::IEC_waterdata_to_APPFRAME(FRAME_BUF,FRAME_Len,&waterFrame))
		{
			TRACE("报文帧格式分析不正确！\r\n");
			return;
		}

		CRtuStation* pSub =  pRealDb->SearchRtuStation(waterFrame._addr_info.A1.ProvinceCode,
			waterFrame._addr_info.A1.RegionCode,
			waterFrame._addr_info.A2);

		if (pSub != NULL)
		{
			waterFrame.m_pStation = pSub;

			pSub->GetSysDataTrans()->AddRecvLog(FRAME_BUF,FRAME_Len,&waterFrame);
			pSub->SetLastRecvTime();
			waterFrame.UpdateRecvTime();

			if (pSub->SetCommState(Connected_State) == true)  //状态有改变，更新CMSRightView的显示
				pSub->GetSysDataTrans()->UpdateRtuCommState(pSub);

			waterFrame.m_CommInterface		= Ethernet_CommInerface;
			waterFrame.m_ProtocolType		= TCP_Protocol;
			waterFrame.m_NetInfo.mIpAddress = m_Client_NetInfo.mIpAddress;
			waterFrame.m_NetInfo.mPort		= m_Client_NetInfo.mPort;

			pSub->SetInterface(Ethernet_CommInerface);
			pSub->SetProtocolType(TCP_Protocol);
			pSub->SetHandle_TcpClient(this);
			pSub->SetIpAddrA(m_Client_NetInfo.mIpAddress);
			pSub->SetSockPortA(m_Client_NetInfo.mPort);
			pSub->SetCmType(m_CmType);

			SetRelateStation(pSub);

			pSub->GetSysDataTrans()->UpdateRtuCommInfo(pSub);
			pSub->GetSysDataTrans()->Dispatcher(&waterFrame);
		}

		memset(FRAME_BUF,0,sizeof(FRAME_BUF));
		FRAME_Len = 0;
	}
}



BOOL CHandle_TcpClient::enable_linkframe()
{
	if (RECV_Len > 5)
	{
		int pos_BEGIN,pos_END;

		pos_BEGIN = -1;
		pos_END = -1;

		for (int idx = 0; idx < RECV_Len; idx ++)
		{
			if (RECV_Buf[idx] == 0x68 &&
				RECV_Buf[idx + 2] == 0x68 &&
				RECV_Buf[idx + 1] != 0)
			{
				int appframeLen = RECV_Buf[idx+1];     //长度
				if (RECV_Len > idx + appframeLen + 4)
				{
					if (CUdfGlobalFUNC::do_crc76520(&RECV_Buf[idx + 3],appframeLen)  == RECV_Buf[idx + appframeLen + 3]  &&
						RECV_Buf[idx + appframeLen + 4] == 0x16)
					{
						pos_BEGIN = idx;
						pos_END   = idx + appframeLen + 4;
						ZeroMemory(FRAME_BUF,sizeof(FRAME_BUF));
						memmove(FRAME_BUF,&RECV_Buf[idx],appframeLen + 5);
						FRAME_Len = appframeLen + 5;

						BYTE temp[RECV_BUFFER_LEN];
						ZeroMemory(temp,sizeof(temp));
						memmove(temp,&RECV_Buf[pos_END +1],RECV_Len -(pos_END +1));
						ZeroMemory(RECV_Buf,sizeof(RECV_Buf));
						memmove(RECV_Buf,temp,RECV_Len -(pos_END +1));

						RECV_Len = RECV_Len -(pos_END +1);

						return TRUE;
					}
				}
			}
		}
	}
	return FALSE;
}


void CHandle_TcpClient::SetRelateStation(CRtuStation* pSub)
{
	m_pRelate_RtuStation = pSub;
}


CRtuStation* CHandle_TcpClient::GetRelateStation()
{
	return m_pRelate_RtuStation;
}

void CHandle_TcpClient::AddRecvMessage(BYTE* buffer,WORD Len,PSYSTEMTIME pSt /* = NULL */)
{
	if (!buffer ||
		Len <=0 ) return;

	CString strNetInfo="",strArrow  = "",strLength="";
	strNetInfo.Format("TCP,SRC_IP=(%s,%d),",m_Client_NetInfo.mIpAddress,m_Client_NetInfo.mPort);
	strArrow = "收 ,";
	strLength.Format("长度=%-3d,报文=",Len);

	CString strBuf="",strTemp="";
	for (WORD idx=0;idx <Len;idx ++)
	{
		strTemp.Format("%02X ",buffer[idx]);
		strBuf += strTemp;
	}

	CString strResult = "";

	strResult.Format("%s%s%s%s",strArrow,strNetInfo,strLength,strBuf);

	GETAPP()->SystemLog(RTU_CM_BUFFER_LOG,strResult,Normal_Log_Message,pSt);
	
}

void CHandle_TcpClient::AddSendMessage(BYTE* buffer,WORD len,PSYSTEMTIME pSt /* = NULL */)
{
	if (!buffer ||
		len <=0 ) return;

	CString strNetInfo="",strArrow  = "",strLength="";
	strNetInfo.Format("TCP,DST_IP=(%s,%d),",m_Client_NetInfo.mIpAddress,m_Client_NetInfo.mPort);
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