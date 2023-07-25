#include "stdafx.h"
#include "CMS.h"
#include "Protocol_TcpHandler.h"
#include "Handle_TcpClient.h"

#define frame_scsw     1
#define frame_write    2
#define frame_invalid  3

CProtocol_TcpHandle::~CProtocol_TcpHandle()
{

}

//---------------------CSL427_TcpHandle-------------------------
CSL427_TcpHandle::CSL427_TcpHandle(CHandle_TcpClient *p)
:CProtocol_TcpHandle(p)
{

}

CSL427_TcpHandle::~CSL427_TcpHandle()
{

}

void CSL427_TcpHandle::Deal_RecvData(BYTE* buffer,WORD len)
{
	if (!buffer || len <= 0 )
		return;

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	CRealTimeDB* pRealDb = pApp->m_pReadDB;
	if (!pRealDb)  return ;

	GetLocalTime(&m_pTcpClient->m_lastRecvTime);
	AddRecvMessage(buffer,len,&m_pTcpClient->m_lastRecvTime);

	int overed = 0;

	//实际存入缓冲区的字节数
	//溢出原则:后收到的数据丢失
	overed = len > (4096 - m_RECV_Len) ? (4096 - m_RECV_Len) : len;//处理缓冲区溢出
	if(overed>0)
	{
		memmove(&m_RECV_Buf[m_RECV_Len],buffer,overed);
		m_RECV_Len += overed;
	}

	//报文处理
	memset(m_FRAME_BUF,0,sizeof(m_FRAME_BUF));
	m_FRAME_Len = 0;
	while(enable_linkframe() != FALSE)
	{
		CPacketFrameFormat waterFrame;
		if (!CUdfGlobalFUNC::IEC_waterdata_to_APPFRAME(m_FRAME_BUF,m_FRAME_Len,&waterFrame))
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

			pSub->GetSysDataTrans()->AddRecvLog(m_FRAME_BUF,m_FRAME_Len,&waterFrame);
			pSub->SetLastRecvTime();
			waterFrame.UpdateRecvTime();

			if (pSub->SetCommState(Connected_State) == true)  //状态有改变，更新CMSRightView的显示
				pSub->GetSysDataTrans()->UpdateRtuCommState(pSub);

			waterFrame.m_CommInterface		= Ethernet_CommInerface;
			waterFrame.m_ProtocolType		= TCP_Protocol;
			waterFrame.m_NetInfo.mIpAddress = m_pTcpClient->m_Client_NetInfo.mIpAddress;
			waterFrame.m_NetInfo.mPort		= m_pTcpClient->m_Client_NetInfo.mPort;

			pSub->SetInterface(Ethernet_CommInerface);
			pSub->SetProtocolType(TCP_Protocol);
			pSub->SetHandle_TcpClient(m_pTcpClient);
			pSub->SetIpAddrA(m_pTcpClient->m_Client_NetInfo.mIpAddress);
			pSub->SetSockPortA(m_pTcpClient->m_Client_NetInfo.mPort);
			pSub->SetCmType(m_pTcpClient->m_CmType);

			m_pTcpClient->SetRelateStation(pSub);

			pSub->GetSysDataTrans()->UpdateRtuCommInfo(pSub);
			pSub->GetSysDataTrans()->Dispatcher(&waterFrame);
		}

		memset(m_FRAME_BUF,0,sizeof(m_FRAME_BUF));
		m_FRAME_Len = 0;
	}
}


BOOL CSL427_TcpHandle::enable_linkframe()
{
	if (m_RECV_Len > 5)
	{
		int pos_BEGIN,pos_END;

		pos_BEGIN = -1;
		pos_END = -1;

		for (int idx = 0; idx < m_RECV_Len; idx ++)
		{
			if (m_RECV_Buf[idx] == 0x68 &&
				m_RECV_Buf[idx + 2] == 0x68 &&
				m_RECV_Buf[idx + 1] != 0)
			{
				int appframeLen = m_RECV_Buf[idx+1];     //长度
				if (m_RECV_Len > idx + appframeLen + 4)
				{
					if (CUdfGlobalFUNC::do_crc76520(&m_RECV_Buf[idx + 3],appframeLen)  == m_RECV_Buf[idx + appframeLen + 3]  &&
						m_RECV_Buf[idx + appframeLen + 4] == 0x16)
					{
						pos_BEGIN = idx;
						pos_END   = idx + appframeLen + 4;
						ZeroMemory(m_FRAME_BUF,sizeof(m_FRAME_BUF));
						memmove(m_FRAME_BUF,&m_RECV_Buf[idx],appframeLen + 5);
						m_FRAME_Len = appframeLen + 5;

						BYTE temp[RECV_BUFFER_LEN];
						ZeroMemory(temp,sizeof(temp));
						memmove(temp,&m_RECV_Buf[pos_END +1],m_RECV_Len -(pos_END +1));
						ZeroMemory(m_RECV_Buf,sizeof(m_RECV_Buf));
						memmove(m_RECV_Buf,temp,m_RECV_Len -(pos_END +1));

						m_RECV_Len = m_RECV_Len -(pos_END +1);

						return TRUE;
					}
				}
			}
		}
	}
	return FALSE;

}

void CSL427_TcpHandle::AddSendMessage(BYTE* buffer,WORD len,PSYSTEMTIME pSt)
{
	if (!buffer ||
		len <=0 ) return;

	CString strNetInfo="",strArrow  = "",strLength="";
	strNetInfo.Format("TCP,DST_IP=(%s,%d),",m_pTcpClient->m_Client_NetInfo.mIpAddress,m_pTcpClient->m_Client_NetInfo.mPort);
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

void CSL427_TcpHandle::AddRecvMessage(BYTE* buffer,WORD Len,PSYSTEMTIME pSt)
{
	if (!buffer ||
		Len <=0 ) return;

	CString strNetInfo="",strArrow  = "",strLength="";
	strNetInfo.Format("TCP,SRC_IP=(%s,%d),",m_pTcpClient->m_Client_NetInfo.mIpAddress,m_pTcpClient->m_Client_NetInfo.mPort);
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

#define Frame_HeartBeat   1
#define Frame_Data        2
#define work_mode         1 //工作模式
#define debug_mode        2 //调试模式

//-----------------SCSWOO8-2011水文测报系统技术规约和协议-----------------
CSCSW_TcpHandle::CSCSW_TcpHandle(CHandle_TcpClient *p)
:CProtocol_TcpHandle(p)
{
	m_nDivCnt = 0;
	m_recvFrameType = Frame_Data;
	m_msgMode = work_mode;
	m_isFirstMsg = false;

}

CSCSW_TcpHandle::~CSCSW_TcpHandle()
{

}

void CSCSW_TcpHandle::Deal_RecvData(BYTE* buffer,WORD len)
{
	if (!buffer || len <= 0 )
		return;

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	CRealTimeDB* pRealDb = pApp->m_pReadDB;
	if (!pRealDb)  return ;

	GetLocalTime(&m_pTcpClient->m_lastRecvTime);
	AddRecvMessage(buffer,len,&m_pTcpClient->m_lastRecvTime);

	int overed = 0;

	//实际存入缓冲区的字节数
	//溢出原则:后收到的数据丢失
	overed = len > (4096 - m_RECV_Len) ? (4096 - m_RECV_Len) : len;//处理缓冲区溢出
	if(overed>0)
	{
		memmove(&m_RECV_Buf[m_RECV_Len],buffer,overed);
		m_RECV_Len += overed;
	}

	//报文处理
	memset(m_FRAME_BUF,0,sizeof(m_FRAME_BUF));
	m_FRAME_Len = 0;

	int nCircle = enable_linkframe();
	while(nCircle != frame_invalid)
	{
		if ( nCircle == frame_scsw )
		{
			CHydrFrameFormat HydrFrame;

			if (!CUdfGlobalFUNC::SCSW008_hydrologic_to_APPFRAME(m_FRAME_BUF, m_FRAME_Len, &HydrFrame))
			{
				TRACE("报文帧格式分析不正确！\r\n");
				return;
			}

			CRtuStation* pSub = pRealDb->SearchRtuStation(HydrFrame.m_strStCode, SCSW_008_Protocol);

			if (pSub != NULL)
			{
// 			
// 				SYSTEMTIME s;
// 				::GetLocalTime(&s);
// 
// 				if (s.wHour == 8)
// 				{
// 					m_isFirstMsg = true;
// 
// 				}

				HydrFrame.m_pStation = pSub;
				HydrFrame.UpdateRecvTime();
				pSub->GetSysDataTrans_Hydr()->AddRecvLog(m_FRAME_BUF, m_FRAME_Len, &HydrFrame);
				pSub->SetLastRecvTime();


				if (pSub->SetCommState(Connected_State) == true)  //状态有改变，更新CMSRightView的显示
					pSub->GetSysDataTrans()->UpdateRtuCommState(pSub);

				HydrFrame.m_CommInterface		= Ethernet_CommInerface;
				HydrFrame.m_ProtocolType		= TCP_Protocol;
				HydrFrame.m_NetInfo.mIpAddress = m_pTcpClient->m_Client_NetInfo.mIpAddress;
				HydrFrame.m_NetInfo.mPort		= m_pTcpClient->m_Client_NetInfo.mPort;

				pSub->SetInterface(Ethernet_CommInerface);
				pSub->SetProtocolType(TCP_Protocol);
				pSub->SetHandle_TcpClient(m_pTcpClient);
				pSub->SetIpAddrA(m_pTcpClient->m_Client_NetInfo.mIpAddress);
				pSub->SetSockPortA(m_pTcpClient->m_Client_NetInfo.mPort);
				pSub->SetCmType(m_pTcpClient->m_CmType);

				m_pTcpClient->SetRelateStation(pSub);

				pSub->GetSysDataTrans_Hydr()->UpdateRtuCommInfo(pSub);
				if (m_recvFrameType == Frame_Data)
				{
					if (m_msgMode == work_mode)
					{
						HydrFrame.m_workmode = 1;
					}
					else if (m_msgMode == debug_mode)
					{ 
						HydrFrame.m_workmode = 2;
					}

					pSub->GetSysDataTrans_Hydr()->Dispatcher(&HydrFrame);
				}
				else if (m_recvFrameType == Frame_HeartBeat)
				{

					HydrFrame.m_cs_info.Lock();
					while (HydrFrame.m_MsgInfoList.begin() != HydrFrame.m_MsgInfoList.end())
					{
						delete (*HydrFrame.m_MsgInfoList.begin());
						HydrFrame.m_MsgInfoList.erase(HydrFrame.m_MsgInfoList.begin());
					}
					HydrFrame.m_cs_info.Unlock();
				}

			}
			else
			{
				HydrFrame.m_cs_info.Lock();
				while (HydrFrame.m_MsgInfoList.begin() != HydrFrame.m_MsgInfoList.end())
				{
					delete (*HydrFrame.m_MsgInfoList.begin());
					HydrFrame.m_MsgInfoList.erase(HydrFrame.m_MsgInfoList.begin());
				}
				HydrFrame.m_cs_info.Unlock();
			}

			memset(m_FRAME_BUF,0,sizeof(m_FRAME_BUF));
			m_FRAME_Len = 0;
		}
		else if (nCircle == frame_write)
		{
			CWriteFrame writeFrame;

			if (!CUdfGlobalFUNC::WRITE_frame_to_APPFRAME(m_FRAME_BUF, m_FRAME_Len, &writeFrame))
			{
				TRACE("报文帧格式分析不正确！\r\n");
				return;
			}

			CRtuStation* pSub = pRealDb->SearchRtuStation(writeFrame.m_nDevAdddr, SCSW_008_Protocol);

			if (pSub != NULL)
			{
				writeFrame.m_pStation = pSub;
				writeFrame.UpdateRecvTime();
				pSub->GetSysDataTrans_WriteUpdate()->AddRecvLog(m_FRAME_BUF, m_FRAME_Len, &writeFrame);
				pSub->SetLastRecvTime();


				if (pSub->SetCommState(Connected_State) == true)  //状态有改变，更新CMSRightView的显示
					pSub->GetSysDataTrans()->UpdateRtuCommState(pSub);

				writeFrame.m_CommInterface		= Ethernet_CommInerface;
				writeFrame.m_ProtocolType		= TCP_Protocol;
				writeFrame.m_NetInfo.mIpAddress = m_pTcpClient->m_Client_NetInfo.mIpAddress;
				writeFrame.m_NetInfo.mPort		= m_pTcpClient->m_Client_NetInfo.mPort;

				pSub->SetInterface(Ethernet_CommInerface);
				pSub->SetProtocolType(TCP_Protocol);
				pSub->SetHandle_TcpClient(m_pTcpClient);
				pSub->SetIpAddrA(m_pTcpClient->m_Client_NetInfo.mIpAddress);
				pSub->SetSockPortA(m_pTcpClient->m_Client_NetInfo.mPort);
				pSub->SetCmType(m_pTcpClient->m_CmType);

				m_pTcpClient->SetRelateStation(pSub);

				pSub->GetSysDataTrans_WriteUpdate()->SetRtuStation(pSub);
				pSub->GetSysDataTrans_WriteUpdate()->UpdateRtuCommInfo(pSub);
				pSub->GetSysDataTrans_WriteUpdate()->Dispatcher(&writeFrame);

			}

		}

		nCircle = enable_linkframe();

	}
}

BOOL CSCSW_TcpHandle::enable_linkframe()
{
	if (ParseLinkFrame_Msg())
	{
		return frame_scsw;
	}
	else
	{
		if (ParseLinkFrame_Write())
		{
			return frame_write;
		}
		else
		{
			ZeroMemory(m_RECV_Buf, sizeof(m_RECV_Buf));
			m_RECV_Len = 0;
			return frame_invalid;
		}
	}
}

BOOL CSCSW_TcpHandle::ParseLinkFrame_Msg()
{
// 	if (m_RECV_Len < 8)
// 	{
// 		return FALSE;
// 	}

	int pos_BEGIN = -1;
	int pos_END = -1;

	bool bFindHead = false;
	bool bFindTail = false;
	int  nMsgLen = 0;
	BOOL returnVar = FALSE;

	for (int idx=0; idx<m_RECV_Len-1; idx++)
	{//使用m_Recv_Len是防止m_Recv_Buf越界
		if ('S'==m_RECV_Buf[idx] && 'T'==m_RECV_Buf[idx+1])
		{
			if (m_RECV_Len > idx+17)
			{
				if ('a'==m_RECV_Buf[idx+14] && 'a'==m_RECV_Buf[idx+15])
				{
					if ('N'!= m_RECV_Buf[idx+16] && 'N'!= m_RECV_Buf[idx+17])
					{
						pos_BEGIN = idx;
						pos_END = idx+15;
						m_recvFrameType = Frame_HeartBeat;
						break;

					}
				}
			}
			else if (m_RECV_Len > idx+15)
			{
				if ('a'==m_RECV_Buf[idx+14] && 'a'==m_RECV_Buf[idx+15])
				{
					pos_BEGIN = idx;
					pos_END = idx+15;
					m_recvFrameType = Frame_HeartBeat;
					break;

				}
			}

			if (!bFindTail)
			{
				pos_BEGIN = idx;
				bFindHead = true;
			}
		}
		else if ('N'==m_RECV_Buf[idx] && 'N'==m_RECV_Buf[idx+1])
		{
			if (bFindHead)
			{
				if (':'==m_RECV_Buf[idx-1] && ':'==m_RECV_Buf[idx-2])
				{
					m_msgMode = debug_mode;
				}
				else
				{
					m_msgMode = work_mode;

				}

				pos_END = idx+1;
				bFindTail = true;
				m_recvFrameType = Frame_Data;
				break;
			}	
		}
	}

	if (pos_BEGIN!=-1 && pos_END!=-1)
	{
		nMsgLen = pos_END - pos_BEGIN + 1;
		ZeroMemory(m_FRAME_BUF, sizeof(m_FRAME_BUF));
		memcpy(m_FRAME_BUF, &m_RECV_Buf[pos_BEGIN], nMsgLen);
		m_FRAME_Len = nMsgLen;

		switch (m_recvFrameType)
		{
		case Frame_Data:
			{
				if (('a'==m_RECV_Buf[m_FRAME_Len-4] && 'a'==m_RECV_Buf[m_FRAME_Len-3])
					|| (':'==m_RECV_Buf[m_FRAME_Len-4] && ':'==m_RECV_Buf[m_FRAME_Len-3]))
				{//如果校验码是aa，表明当前指令为【查询指令】
					BYTE temp[RECV_BUFFER_LEN];
					ZeroMemory(temp, sizeof(temp));
					memmove(temp, &m_RECV_Buf[pos_END +1],m_RECV_Len -(pos_END +1));
					ZeroMemory(m_RECV_Buf,sizeof(m_RECV_Buf));
					memmove(m_RECV_Buf,temp,m_RECV_Len -(pos_END +1));

					m_RECV_Len = m_RECV_Len -(pos_END +1);
					returnVar = TRUE;

				}
				else
				{
					if (CUdfGlobalFUNC::CheckCRC_SCSW(m_FRAME_BUF, m_FRAME_Len))
					{
						BYTE temp[RECV_BUFFER_LEN];
						ZeroMemory(temp, sizeof(temp));
						memmove(temp, &m_RECV_Buf[pos_END +1],m_RECV_Len -(pos_END +1));
						ZeroMemory(m_RECV_Buf,sizeof(m_RECV_Buf));
						memmove(m_RECV_Buf,temp,m_RECV_Len -(pos_END +1));

						m_RECV_Len = m_RECV_Len -(pos_END +1);
						returnVar = TRUE;
					}
					else
					{
						ZeroMemory(m_FRAME_BUF, sizeof(m_FRAME_BUF));
						m_FRAME_Len = 0;

						BYTE temp[RECV_BUFFER_LEN];
						ZeroMemory(temp, sizeof(temp));
						memmove(temp, &m_RECV_Buf[pos_END +1],m_RECV_Len -(pos_END +1));
						ZeroMemory(m_RECV_Buf,sizeof(m_RECV_Buf));
						memmove(m_RECV_Buf,temp,m_RECV_Len -(pos_END +1));

						m_RECV_Len = m_RECV_Len -(pos_END +1);
					}
				}

			}
			break;
		case Frame_HeartBeat:
			{
				BYTE temp[RECV_BUFFER_LEN];
				ZeroMemory(temp, sizeof(temp));
				memmove(temp, &m_RECV_Buf[pos_END +1],m_RECV_Len -(pos_END +1));
				ZeroMemory(m_RECV_Buf,sizeof(m_RECV_Buf));
				memmove(m_RECV_Buf,temp,m_RECV_Len -(pos_END +1));

				m_RECV_Len = m_RECV_Len -(pos_END +1);
				returnVar = TRUE;

			}
			break;
		}


	}
	else if (-1==pos_BEGIN && -1==pos_END)
	{
// 		ZeroMemory(m_RECV_Buf, sizeof(m_RECV_Buf));
// 		m_RECV_Len = 0;
	}

	return returnVar;

}

BOOL CSCSW_TcpHandle::ParseLinkFrame_Write()
{
	int pos_BEGIN = -1;
	int pos_END = -1;

	bool bFindHead = false;
	bool bFindTail = false;
	int  nMsgLen = 0;
	BOOL returnVar = FALSE;

	for (int idx=0; idx<m_RECV_Len-1; idx++)
	{//使用m_Recv_Len是防止m_Recv_Buf越界
		if ('$'==m_RECV_Buf[idx] && '$'==m_RECV_Buf[idx+1])
		{
			int writeFrameLen = m_RECV_Buf[idx+2];
			if (m_RECV_Len >= writeFrameLen)
			{
				if ('#'==m_RECV_Buf[idx+writeFrameLen-1] && '#'==m_RECV_Buf[idx+writeFrameLen-2])
				{
					pos_BEGIN = idx;
					pos_END = idx + writeFrameLen - 1;
					break;

				}
			}

			if (!bFindTail)
			{
				pos_BEGIN = idx;
				bFindHead = true;
			}
		}
		else if ('#'==m_RECV_Buf[idx] && '#'==m_RECV_Buf[idx+1])
		{
			if (bFindHead)
			{
				pos_END = idx+1;
				bFindTail = true;
				break;
			}	
		}
	}

	if (pos_BEGIN!=-1 && pos_END!=-1)
	{
		nMsgLen = pos_END - pos_BEGIN + 1;
		ZeroMemory(m_FRAME_BUF, sizeof(m_FRAME_BUF));
		memcpy(m_FRAME_BUF, &m_RECV_Buf[pos_BEGIN], nMsgLen);
		m_FRAME_Len = nMsgLen;

		if (CUdfGlobalFUNC::CheckCRC_Write(m_FRAME_BUF, m_FRAME_Len))
		{
			BYTE temp[RECV_BUFFER_LEN];
			ZeroMemory(temp, sizeof(temp));
			memmove(temp, &m_RECV_Buf[pos_END +1],m_RECV_Len -(pos_END +1));
			ZeroMemory(m_RECV_Buf,sizeof(m_RECV_Buf));
			memmove(m_RECV_Buf,temp,m_RECV_Len -(pos_END +1));

			m_RECV_Len = m_RECV_Len -(pos_END +1);
			returnVar = TRUE;
		}
		else
		{
			ZeroMemory(m_FRAME_BUF, sizeof(m_FRAME_BUF));
			m_FRAME_Len = 0;

			BYTE temp[RECV_BUFFER_LEN];
			ZeroMemory(temp, sizeof(temp));
			memmove(temp, &m_RECV_Buf[pos_END +1],m_RECV_Len -(pos_END +1));
			ZeroMemory(m_RECV_Buf,sizeof(m_RECV_Buf));
			memmove(m_RECV_Buf,temp,m_RECV_Len -(pos_END +1));

			m_RECV_Len = m_RECV_Len -(pos_END +1);
		}

	}
	else if (-1==pos_BEGIN && -1==pos_END)
	{
		ZeroMemory(m_RECV_Buf, sizeof(m_RECV_Buf));
		m_RECV_Len = 0;
	}

	return returnVar;

}

void CSCSW_TcpHandle::AddSendMessage(BYTE* buffer,WORD len, PSYSTEMTIME pSt)
{
	if (!buffer ||
		len <=0 ) return;

	CString strNetInfo="",strArrow  = "",strLength="";
	strNetInfo.Format("TCP,DST_IP=(%s,%d),",m_pTcpClient->m_Client_NetInfo.mIpAddress,m_pTcpClient->m_Client_NetInfo.mPort);
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

void CSCSW_TcpHandle::AddRecvMessage(BYTE* buffer,WORD Len,PSYSTEMTIME pSt)
{
	if (!buffer ||
		Len <=0 ) return;

	CString strNetInfo="",strArrow  = "",strLength="";
	strNetInfo.Format("TCP,SRC_IP=(%s,%d),",m_pTcpClient->m_Client_NetInfo.mIpAddress,m_pTcpClient->m_Client_NetInfo.mPort);
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



//-----------------水雨情遥测通信规约-----------------
CSYQ_TcpHandle::CSYQ_TcpHandle(CHandle_TcpClient *p)
:CProtocol_TcpHandle(p)
{

}

CSYQ_TcpHandle::~CSYQ_TcpHandle()
{

}

BOOL CSYQ_TcpHandle::enable_linkframe()
{
	if (ParseLinkFrame_Msg())
	{
		return frame_scsw;
	}
	else
	{
		if (ParseLinkFrame_Write())
		{
			return frame_write;
		}
		else
		{
			ZeroMemory(m_RECV_Buf, sizeof(m_RECV_Buf));
			m_RECV_Len = 0;
			return frame_invalid;
		}
	}
}

BOOL CSYQ_TcpHandle::ParseLinkFrame_Msg()
{
	int pos_BEGIN = -1;
	int pos_END = -1;

	bool bFindHead = false;
	bool bFindTail = false;
	int  nMsgLen = 0;//报文正文长度
	BOOL returnVar = FALSE;

	for (int idx=0; idx<m_RECV_Len-1; idx++)
	{//使用m_Recv_Len是防止m_Recv_Buf越界
		if (0x01==m_RECV_Buf[idx])//SOH
		{			
			if (m_RECV_Len > idx+25)
			{
				nMsgLen = m_RECV_Buf[idx+20]*256+m_RECV_Buf[idx+19];
				if (0x02==m_RECV_Buf[idx+21] && ((0x03==m_RECV_Buf[idx+nMsgLen+22])||(0x17==m_RECV_Buf[idx+nMsgLen+22])))// STX  ETX ETB
				{
					pos_BEGIN = idx;

					memcpy(m_FRAME_BUF, &m_RECV_Buf[pos_BEGIN], nMsgLen+25);
					m_FRAME_Len = nMsgLen +25;
					if (CUdfGlobalFUNC::CheckCRC_SYQ(m_FRAME_BUF, m_FRAME_Len))
					{

						pos_END = idx+nMsgLen+24;
						m_FRAME_Len = nMsgLen + 25;

						BYTE temp[RECV_BUFFER_LEN];
						ZeroMemory(temp,sizeof(temp));
						memmove(temp,&m_RECV_Buf[pos_END +1],m_RECV_Len -(pos_END +1));
						ZeroMemory(m_RECV_Buf,sizeof(m_RECV_Buf));
						memmove(m_RECV_Buf,temp,m_RECV_Len -(pos_END +1));

						m_RECV_Len = m_RECV_Len -(pos_END +1);

						return TRUE;

					}
				}
			}


		}

	}

	return returnVar;

	

}

BOOL CSYQ_TcpHandle::ParseLinkFrame_Write()//??????????????????
{
	int pos_BEGIN = -1;
	int pos_END = -1;

	bool bFindHead = false;
	bool bFindTail = false;
	int  nMsgLen = 0;
	BOOL returnVar = FALSE;

	for (int idx=0; idx<m_RECV_Len-1; idx++)
	{//使用m_Recv_Len是防止m_Recv_Buf越界
		if ('$'==m_RECV_Buf[idx] && '$'==m_RECV_Buf[idx+1])
		{
			int writeFrameLen = m_RECV_Buf[idx+2];
			if (m_RECV_Len >= writeFrameLen)
			{
				if ('#'==m_RECV_Buf[idx+writeFrameLen-1] && '#'==m_RECV_Buf[idx+writeFrameLen-2])
				{
					pos_BEGIN = idx;
					pos_END = idx + writeFrameLen - 1;
					break;

				}
			}

			if (!bFindTail)
			{
				pos_BEGIN = idx;
				bFindHead = true;
			}
		}
		else if ('#'==m_RECV_Buf[idx] && '#'==m_RECV_Buf[idx+1])
		{
			if (bFindHead)
			{
				pos_END = idx+1;
				bFindTail = true;
				break;
			}	
		}
	}

	if (pos_BEGIN!=-1 && pos_END!=-1)
	{
		nMsgLen = pos_END - pos_BEGIN + 1;
		ZeroMemory(m_FRAME_BUF, sizeof(m_FRAME_BUF));
		memcpy(m_FRAME_BUF, &m_RECV_Buf[pos_BEGIN], nMsgLen);
		m_FRAME_Len = nMsgLen;

		if (CUdfGlobalFUNC::CheckCRC_Write(m_FRAME_BUF, m_FRAME_Len))
		{
			BYTE temp[RECV_BUFFER_LEN];
			ZeroMemory(temp, sizeof(temp));
			memmove(temp, &m_RECV_Buf[pos_END +1],m_RECV_Len -(pos_END +1));
			ZeroMemory(m_RECV_Buf,sizeof(m_RECV_Buf));
			memmove(m_RECV_Buf,temp,m_RECV_Len -(pos_END +1));

			m_RECV_Len = m_RECV_Len -(pos_END +1);
			returnVar = TRUE;
		}
		else
		{
			ZeroMemory(m_FRAME_BUF, sizeof(m_FRAME_BUF));
			m_FRAME_Len = 0;

			BYTE temp[RECV_BUFFER_LEN];
			ZeroMemory(temp, sizeof(temp));
			memmove(temp, &m_RECV_Buf[pos_END +1],m_RECV_Len -(pos_END +1));
			ZeroMemory(m_RECV_Buf,sizeof(m_RECV_Buf));
			memmove(m_RECV_Buf,temp,m_RECV_Len -(pos_END +1));

			m_RECV_Len = m_RECV_Len -(pos_END +1);
		}

	}
	else if (-1==pos_BEGIN && -1==pos_END)
	{
		ZeroMemory(m_RECV_Buf, sizeof(m_RECV_Buf));
		m_RECV_Len = 0;
	}

	return returnVar;

}

void CSYQ_TcpHandle::Deal_RecvData(BYTE* buffer,WORD len)
{
	if (!buffer || len <= 0 )
		return;

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	CRealTimeDB* pRealDb = pApp->m_pReadDB;
	if (!pRealDb)  return ;

	GetLocalTime(&m_pTcpClient->m_lastRecvTime);
	AddRecvMessage(buffer,len,&m_pTcpClient->m_lastRecvTime);

	int overed = 0;

	//实际存入缓冲区的字节数
	//溢出原则:后收到的数据丢失
	overed = len > (4096 - m_RECV_Len) ? (4096 - m_RECV_Len) : len;//处理缓冲区溢出
	if(overed>0)
	{
		memmove(&m_RECV_Buf[m_RECV_Len],buffer,overed);
		m_RECV_Len += overed;
	}

	//报文处理
	memset(m_FRAME_BUF,0,sizeof(m_FRAME_BUF));
	m_FRAME_Len = 0;

	int nCircle = enable_linkframe();
	while(nCircle != frame_invalid)
	{
		if ( nCircle == frame_scsw )
		{
			CSYQFrameFormat syqFrame;

			if (!CUdfGlobalFUNC::SYQ_to_APPFRAME(m_FRAME_BUF, m_FRAME_Len, &syqFrame))
			{
				TRACE("报文帧格式分析不正确！\r\n");
				return;
			}

			CRtuStation* pSub = pRealDb->SearchRtuStation(syqFrame.m_stCode, SYQYC_Comm_Protocol);

			if (pSub != NULL)
			{

				syqFrame.m_pStation = pSub;
				syqFrame.UpdateRecvTime();
				pSub->GetSysDataTrans_Syq()->AddRecvLog(m_FRAME_BUF, m_FRAME_Len, &syqFrame);
				pSub->SetLastRecvTime();


				//状态有改变，更新CMSRightView的显示
				if (pSub->SetCommState(Connected_State) == true)  
					pSub->GetSysDataTrans_Syq()->UpdateRtuCommState(pSub);

				syqFrame.m_CommInterface		= Ethernet_CommInerface;
				syqFrame.m_ProtocolType		= TCP_Protocol;
				syqFrame.m_NetInfo.mIpAddress = m_pTcpClient->m_Client_NetInfo.mIpAddress;
				syqFrame.m_NetInfo.mPort		= m_pTcpClient->m_Client_NetInfo.mPort;

				pSub->SetInterface(Ethernet_CommInerface);
				pSub->SetProtocolType(TCP_Protocol);
				pSub->SetHandle_TcpClient(m_pTcpClient);
				pSub->SetIpAddrA(m_pTcpClient->m_Client_NetInfo.mIpAddress);
				pSub->SetSockPortA(m_pTcpClient->m_Client_NetInfo.mPort);
				pSub->SetCmType(m_pTcpClient->m_CmType);

				m_pTcpClient->SetRelateStation(pSub);

				pSub->GetSysDataTrans_Syq()->UpdateRtuCommInfo(pSub);
				pSub->GetSysDataTrans_Syq()->Dispatcher(&syqFrame);
			
				
			}
			

			memset(m_FRAME_BUF,0,sizeof(m_FRAME_BUF));
			m_FRAME_Len = 0;
		}
		else if (nCircle == frame_write)
		{
			CWriteFrame writeFrame;

			if (!CUdfGlobalFUNC::WRITE_frame_to_APPFRAME(m_FRAME_BUF, m_FRAME_Len, &writeFrame))
			{
				TRACE("报文帧格式分析不正确！\r\n");
				return;
			}

			CRtuStation* pSub = pRealDb->SearchRtuStation(writeFrame.m_nDevAdddr, SCSW_008_Protocol);

			if (pSub != NULL)
			{
				writeFrame.m_pStation = pSub;
				writeFrame.UpdateRecvTime();
				pSub->GetSysDataTrans_WriteUpdate()->AddRecvLog(m_FRAME_BUF, m_FRAME_Len, &writeFrame);
				pSub->SetLastRecvTime();


				if (pSub->SetCommState(Connected_State) == true)  //状态有改变，更新CMSRightView的显示
					pSub->GetSysDataTrans()->UpdateRtuCommState(pSub);

				writeFrame.m_CommInterface		= Ethernet_CommInerface;
				writeFrame.m_ProtocolType		= TCP_Protocol;
				writeFrame.m_NetInfo.mIpAddress = m_pTcpClient->m_Client_NetInfo.mIpAddress;
				writeFrame.m_NetInfo.mPort		= m_pTcpClient->m_Client_NetInfo.mPort;

				pSub->SetInterface(Ethernet_CommInerface);
				pSub->SetProtocolType(TCP_Protocol);
				pSub->SetHandle_TcpClient(m_pTcpClient);
				pSub->SetIpAddrA(m_pTcpClient->m_Client_NetInfo.mIpAddress);
				pSub->SetSockPortA(m_pTcpClient->m_Client_NetInfo.mPort);
				pSub->SetCmType(m_pTcpClient->m_CmType);

				m_pTcpClient->SetRelateStation(pSub);

				pSub->GetSysDataTrans_WriteUpdate()->SetRtuStation(pSub);
				pSub->GetSysDataTrans_WriteUpdate()->UpdateRtuCommInfo(pSub);
				pSub->GetSysDataTrans_WriteUpdate()->Dispatcher(&writeFrame);

			}

		}

		nCircle = enable_linkframe();

	}
}


void CSYQ_TcpHandle::AddSendMessage(BYTE* buffer,WORD len, PSYSTEMTIME pSt)
{
	if (!buffer ||
		len <=0 ) return;

	CString strNetInfo="",strArrow  = "",strLength="";
	strNetInfo.Format("TCP,DST_IP=(%s,%d),",m_pTcpClient->m_Client_NetInfo.mIpAddress,m_pTcpClient->m_Client_NetInfo.mPort);
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

void CSYQ_TcpHandle::AddRecvMessage(BYTE* buffer,WORD Len,PSYSTEMTIME pSt)
{
	if (!buffer ||
		Len <=0 ) return;

	CString strNetInfo="",strArrow  = "",strLength="";
	strNetInfo.Format("TCP,SRC_IP=(%s,%d),",m_pTcpClient->m_Client_NetInfo.mIpAddress,m_pTcpClient->m_Client_NetInfo.mPort);
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
