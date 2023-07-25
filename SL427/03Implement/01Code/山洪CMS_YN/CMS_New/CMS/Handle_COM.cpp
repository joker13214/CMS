#include "StdAfx.h"
#include "CMS.h"
#include "UdfGlobalFUNC.h"
#include "Handle_COM.h"
#include "RtuStation.h"

#define Frame_HeartBeat   1
#define Frame_Data        2
#define work_mode         1 //工作模式
#define debug_mode        2 //调试模式

#define frame_scsw     1
#define frame_write    2
#define frame_invalid  3

#pragma warning(disable:4018)
CHandle_COM::CHandle_COM(void)
{
	m_COM_HANDLE = INVALID_HANDLE_VALUE;
	memset(RECV_BUF,0,RECV_BUFFER_LEN);
	RECV_len = 0;
	m_bAppProtoType = (BYTE)WaterResources_Protocol;
	m_recvFrameType = Frame_Data;
	m_msgMode = work_mode;
	m_DataOrWriteFlag = frame_invalid;

	//m_pRelate_Station = NULL;
}

CHandle_COM::~CHandle_COM(void)
{

	while (!m_lstRelateStations.IsEmpty())
	{
		CRtuStation* pSub = m_lstRelateStations.RemoveHead();
		if (pSub)
		{
			if (pSub->SetCommState(Disconnect_State) == true)
				pSub->GetSysDataTrans()->UpdateRtuCommState(pSub);
			pSub->SetHandle_Com(NULL);
		}
	}

	if (INVALID_HANDLE_VALUE != m_COM_HANDLE)
		CloseHandle(m_COM_HANDLE);
}


unsigned char CHandle_COM::Add_Sum_Check(unsigned char *data, unsigned char data_len)
{
	unsigned char addcrc;
	for(addcrc=0;data_len>0;data_len--)
		addcrc += *data++;
	return addcrc;
}

void CHandle_COM::clearCOM()
{
	if(INVALID_HANDLE_VALUE!=m_COM_HANDLE)
	{
		PurgeComm(m_COM_HANDLE,PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXCLEAR|PURGE_TXABORT);
	}
}

void CHandle_COM::clearBuf()
{
	memset(RECV_BUF,0,RECV_BUFFER_LEN);
	RECV_len = 0;
}

void CHandle_COM::SetAppProtoType(BYTE type)
{
	m_bAppProtoType = type;
}

BYTE CHandle_COM::GetAppProtoType()
{
	return m_bAppProtoType;
}


void CHandle_COM::recvDATA(unsigned char *buffer, unsigned int len)
{
	int overed = 0;
	//实际存入缓冲区的字节数

	//溢出原则:后收到的数据丢失
//	overed = len>(RECV_BUFFER_LEN-RECV_len) ? (RECV_BUFFER_LEN-RECV_len) : len;//处理缓冲区溢出
//	if(overed>0)
//	{
	//	memmove(&RECV_BUF[RECV_len],buffer,overed);
	//	RECV_len += overed;
	//}

	if (len + RECV_len > RECV_BUFFER_LEN)
	{

		BYTE tempbuf[RECV_BUFFER_LEN];
		memset(tempbuf,0, RECV_BUFFER_LEN);

		int overed = len + RECV_len - RECV_BUFFER_LEN;
		memmove(tempbuf,&RECV_BUF[overed], RECV_len - overed);

		RECV_len = RECV_len - overed;
		memset(RECV_BUF,0,sizeof(RECV_BUF));
		memmove(RECV_BUF,tempbuf,RECV_len);

		TRACE("m_RECV_Len= %d, Len=%d\r\n",RECV_len,len);

	}

	memmove(&RECV_BUF[RECV_len],buffer,len);
	RECV_len += len;



}


unsigned long CHandle_COM::recvCOM(unsigned char *buffer, unsigned long len)
{
	unsigned long actRecvL=0,actLen=0;
	actLen = lookCOMBuf();
	if(actLen>0)
	{
		len = len < actLen ? len : actLen;

		if(len>0)
		{
			actLen=0;
			if(!ReadFile(m_COM_HANDLE,buffer,len,&actRecvL,0))
				actRecvL = 0;
		}
	}
	return actRecvL;
}

void CHandle_COM::sendCOM(unsigned char *buffer, unsigned long len)
{
	if(INVALID_HANDLE_VALUE!=m_COM_HANDLE)
	{
		unsigned long actSendL = 0;
		WriteFile(m_COM_HANDLE,buffer,len,&actSendL,0);
	}
}

unsigned long CHandle_COM::lookCOMBuf()
{
	if(INVALID_HANDLE_VALUE==m_COM_HANDLE)
		return 0;

	COMSTAT comStat;
	unsigned long errCode = 0;
	if(!ClearCommError(m_COM_HANDLE,&errCode,&comStat))
		return 0;
	return comStat.cbInQue;
}


BOOL CHandle_COM::initCOM(CString name,int Baund,int DataBits,int StopBits,int Parity)
{
	BOOL  retVal = FALSE;
	if(m_COM_HANDLE!=INVALID_HANDLE_VALUE)
		CloseHandle(m_COM_HANDLE);
	m_COM_HANDLE = CreateFile(name, GENERIC_READ|GENERIC_WRITE,\
		0, NULL, OPEN_EXISTING, 0, NULL);
	if(INVALID_HANDLE_VALUE!=m_COM_HANDLE)
	{
		PurgeComm(m_COM_HANDLE,PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXCLEAR|PURGE_TXABORT);

		DCB dcb;
		GetCommState(m_COM_HANDLE,&dcb);
		dcb.BaudRate = Baund;
		dcb.ByteSize = DataBits;
		dcb.StopBits = StopBits;
		dcb.Parity	 = Parity;
		SetCommState(m_COM_HANDLE,&dcb);
		COMMTIMEOUTS timeout;
		GetCommTimeouts(m_COM_HANDLE,&timeout);
		timeout.ReadIntervalTimeout = MAXDWORD;
		timeout.ReadTotalTimeoutMultiplier = 0;
		timeout.ReadTotalTimeoutConstant   = 0;
		timeout.WriteTotalTimeoutMultiplier= 0;
		timeout.WriteTotalTimeoutConstant  = 0;
		SetCommTimeouts(m_COM_HANDLE,&timeout);
		return TRUE;
	}
	return retVal;
}


BOOL CHandle_COM::enframe_link(BYTE protocolType)
{
	if ( protocolType == (BYTE)WaterResources_Protocol)
	{
		m_DataOrWriteFlag = frame_invalid;
		return enframe_link_WaterResources();
	}
	else if (protocolType == (BYTE)SCSW_008_Protocol)
	{
		return enframe_link_Hydrology();
	}
	else if (protocolType == (BYTE)SYQYC_Comm_Protocol)//BY ZWB
	{
		return enframe_link_SYQ();// zwb
	}
	else
		return FALSE;

	return FALSE;
}


BOOL CHandle_COM::enframe_link_Hydrology()
{
	if (ParseLinkFrameCom_Msg())
	{
		m_DataOrWriteFlag = frame_scsw;
		return TRUE;
	}
	else
	{
		if (ParseLinkFrameCOM_Write())
		{
			m_DataOrWriteFlag = frame_write;
			return TRUE;
		}
		else
		{
			m_DataOrWriteFlag = frame_invalid;
			return FALSE;
		}
	}

	return FALSE;
}


BOOL CHandle_COM::ParseLinkFrameCom_Msg()
{
	int pos_BEGIN = -1;
	int pos_END = -1;

	bool bFindHead = false;
	bool bFindTail = false;
	int  nMsgLen = 0;
	BOOL returnVar = FALSE;

	for (int idx=0; idx<RECV_len-1; idx++)
	{//使用m_Recv_Len是防止m_Recv_Buf越界
		if ('S'==RECV_BUF[idx] && 'T'==RECV_BUF[idx+1])
		{
			if (RECV_len > idx+17)
			{
				if ('a'==RECV_BUF[idx+14] && 'a'==RECV_BUF[idx+15])
				{
					if ('N'!= RECV_BUF[idx+16] && 'N'!= RECV_BUF[idx+17])
					{
						pos_BEGIN = idx;
						pos_END = idx+15;
						m_recvFrameType = Frame_HeartBeat;
						break;

					}
				}
			}
			else if (RECV_len > idx+15)
			{
				if ('a'==RECV_BUF[idx+14] && 'a'==RECV_BUF[idx+15])
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
		else if ('N'==RECV_BUF[idx] && 'N'==RECV_BUF[idx+1])
		{
			if (bFindHead)
			{
				if (':'==RECV_BUF[idx-1] && ':'==RECV_BUF[idx-2])
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
		ZeroMemory(FRAME_BUF, sizeof(FRAME_BUF));
		memcpy(FRAME_BUF, &RECV_BUF[pos_BEGIN], nMsgLen);
		FRAME_Len = nMsgLen;

		switch (m_recvFrameType)
		{
		case Frame_Data:
			{
				if (('a'==RECV_BUF[FRAME_Len-4] && 'a'==RECV_BUF[FRAME_Len-3])
					|| (':'==RECV_BUF[FRAME_Len-4] && ':'==RECV_BUF[FRAME_Len-3]))
				{//如果校验码是aa，表明当前指令为【查询指令】
					BYTE temp[RECV_BUFFER_LEN];
					ZeroMemory(temp, sizeof(temp));
					memmove(temp, &RECV_BUF[pos_END +1],RECV_len -(pos_END +1));
					ZeroMemory(RECV_BUF,sizeof(RECV_BUF));
					memmove(RECV_BUF,temp,RECV_len -(pos_END +1));

					RECV_len = RECV_len -(pos_END +1);
					returnVar = TRUE;

				}
				else
				{
					if (CUdfGlobalFUNC::CheckCRC_SCSW(FRAME_BUF, FRAME_Len))
					{
						BYTE temp[RECV_BUFFER_LEN];
						ZeroMemory(temp, sizeof(temp));
						memmove(temp, &RECV_BUF[pos_END +1],RECV_len -(pos_END +1));
						ZeroMemory(RECV_BUF,sizeof(RECV_BUF));
						memmove(RECV_BUF,temp,RECV_len -(pos_END +1));

						RECV_len = RECV_len -(pos_END +1);
						returnVar = TRUE;
					}
					else
					{
						ZeroMemory(FRAME_BUF, sizeof(FRAME_BUF));
						FRAME_Len = 0;

						BYTE temp[RECV_BUFFER_LEN];
						ZeroMemory(temp, sizeof(temp));
						memmove(temp, &RECV_BUF[pos_END +1],RECV_len -(pos_END +1));
						ZeroMemory(RECV_BUF,sizeof(RECV_BUF));
						memmove(RECV_BUF,temp,RECV_len -(pos_END +1));

						RECV_len = RECV_len -(pos_END +1);
					}
				}

			}
			break;
		case Frame_HeartBeat:
			{
				BYTE temp[RECV_BUFFER_LEN];
				ZeroMemory(temp, sizeof(temp));
				memmove(temp, &RECV_BUF[pos_END +1],RECV_len -(pos_END +1));
				ZeroMemory(RECV_BUF,sizeof(RECV_BUF));
				memmove(RECV_BUF,temp,RECV_len -(pos_END +1));

				RECV_len = RECV_len -(pos_END +1);
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

BOOL CHandle_COM::ParseLinkFrameCom_SYQ()
{
	int pos_BEGIN = -1;
	int pos_END = -1;

	bool bFindHead = false;
	bool bFindTail = false;
	int  nMsgLen = 0;//报文正文长度
	BOOL returnVar = FALSE;

	for (int idx=0; idx<RECV_len-1; idx++)
	{//使用m_Recv_Len是防止m_Recv_Buf越界
		if (0x01==RECV_BUF[idx])//SOH
		{			
			if (RECV_len > idx+25)
			{
				nMsgLen = RECV_BUF[idx+20]*256+RECV_BUF[idx+19];
				if (0x02==RECV_BUF[idx+21] && ((0x03==RECV_BUF[idx+nMsgLen+22])||(0x17==RECV_BUF[idx+nMsgLen+22])))// STX  ETX ETB
				{
					pos_BEGIN = idx;
					
					memcpy(FRAME_BUF, &RECV_BUF[pos_BEGIN], nMsgLen+25);
					FRAME_Len = nMsgLen+25;
					if (CUdfGlobalFUNC::CheckCRC_SYQ(FRAME_BUF, FRAME_Len))
					{

						pos_END = idx+nMsgLen+24;
						FRAME_Len = nMsgLen + 25;

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

	return returnVar;

}

BOOL CHandle_COM::ParseLinkFrameCOM_Write()
{
	int pos_BEGIN = -1;
	int pos_END = -1;

	bool bFindHead = false;
	bool bFindTail = false;
	int  nMsgLen = 0;
	BOOL returnVar = FALSE;

	for (int idx=0; idx<RECV_len-1; idx++)
	{//使用m_Recv_Len是防止m_Recv_Buf越界
		if ('$'==RECV_BUF[idx] && '$'==RECV_BUF[idx+1])
		{
			int writeFrameLen = RECV_BUF[idx+2];
			if (RECV_len >= writeFrameLen)
			{
				if ('#'==RECV_BUF[idx+writeFrameLen-1] && '#'==RECV_BUF[idx+writeFrameLen-2])
				{
					pos_BEGIN = idx;
					pos_END = idx + writeFrameLen - 1;
					bFindTail = true;
					break;

				}
			}

			if (!bFindTail)
			{
				pos_BEGIN = idx;
				bFindHead = true;
			}
		}
		else if ('#'==RECV_BUF[idx] && '#'==RECV_BUF[idx+1])
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
		ZeroMemory(FRAME_BUF, sizeof(FRAME_BUF));
		memcpy(FRAME_BUF, &RECV_BUF[pos_BEGIN], nMsgLen);
		FRAME_Len = nMsgLen;

		if (CUdfGlobalFUNC::CheckCRC_Write(FRAME_BUF, FRAME_Len))
		{
			BYTE temp[RECV_BUFFER_LEN];
			ZeroMemory(temp, sizeof(temp));
			memmove(temp, &RECV_BUF[pos_END +1], RECV_len -(pos_END +1));
			ZeroMemory(RECV_BUF,sizeof(RECV_BUF));
			memmove(RECV_BUF,temp,RECV_len -(pos_END +1));

			RECV_len = RECV_len -(pos_END +1);
			returnVar = TRUE;
		}
		else
		{
			ZeroMemory(FRAME_BUF, sizeof(FRAME_BUF));
			FRAME_Len = 0;

			BYTE temp[RECV_BUFFER_LEN];
			ZeroMemory(temp, sizeof(temp));
			memmove(temp, &RECV_BUF[pos_END +1],RECV_len -(pos_END +1));
			ZeroMemory(RECV_BUF,sizeof(RECV_BUF));
			memmove(RECV_BUF,temp,RECV_len -(pos_END +1));

			RECV_len = RECV_len -(pos_END +1);
		}

	}
	else if (-1==pos_BEGIN && -1==pos_END)
	{
		//ZeroMemory(RECV_BUF, sizeof(RECV_BUF));
		//RECV_len = 0;
	}

	return returnVar;

}



BOOL CHandle_COM::enframe_link_WaterResources()
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
				if (RECV_len > idx + appframeLen + 4)
				{
					if (CUdfGlobalFUNC::do_crc76520(&RECV_BUF[idx + 3],appframeLen)  == RECV_BUF[idx + appframeLen + 3]  &&
						RECV_BUF[idx + appframeLen + 4] == 0x16)
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


int CHandle_COM::getFrameBuffer(BYTE* frBuf)
{
	if (!frBuf)
		return 0;
    int fLen = FRAME_Len;
	memmove(frBuf,FRAME_BUF,fLen);

    FRAME_Len = 0;
	memset(FRAME_BUF,0,sizeof(FRAME_BUF));

	return fLen;
}



//void CHandle_COM::SetRelateStation(CRtuStation* pSub)
//{
//	m_pRelate_Station = pSub;
//}

void CHandle_COM::AddRelateStation(CRtuStation* pSub)
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


void CHandle_COM::RemoveRelateStation(CRtuStation* pSub)
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
//CRtuStation* CHandle_COM::GetRelateStation()
//{
//	return  m_pRelate_Station;
//}

void CHandle_COM::AddSendMessage(BYTE* buffer,unsigned long len,PCOM_INFO pComInfo,PSYSTEMTIME pSt/* =NULL */)
{

	if (!buffer  || len <= 0 || pComInfo == NULL) return;

	CString strNetInfo="";
	strNetInfo.Format("RS232=(%s,%d,%d,%d,%s),",
		pComInfo->mComName,
		pComInfo->mBaund,
		pComInfo->mDataBits,
		pComInfo->mStopBits,
		CUdfGlobalFUNC::GetParityString(pComInfo->mParity));

	CString strArrow = "";
	strArrow = "发 ,";

	CString strBuf="",strTemp = "";
	strTemp.Format("长度=%-3d,报文=",len);
	strBuf += strTemp;

	for (int idx=0;idx <len;idx ++)
	{
		strTemp.Format("%02X ",buffer[idx]);
		strBuf += strTemp;
	}

	CString strResult = "";
	strResult.Format("%s%s%s",strArrow,strNetInfo,strBuf);

	GETAPP()->SystemLog(RTU_CM_BUFFER_LOG,strResult,Normal_Log_Message,pSt);
}

BOOL CHandle_COM::enframe_link_SYQ()
{

	if (ParseLinkFrameCom_SYQ())
	{
		m_DataOrWriteFlag = frame_scsw;
		return TRUE;
	}
	else
	{
		if (ParseLinkFrameCOM_Write())
		{
			m_DataOrWriteFlag = frame_write;
			return TRUE;
		}
		else
		{
			m_DataOrWriteFlag = frame_invalid;
			return FALSE;
		}
	}

	return FALSE;;
}
