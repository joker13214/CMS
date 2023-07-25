#include "stdafx.h"
#include "CMS.h"
#include "Protocol_ComScHandler.h"

#define frame_scsw     1
#define frame_write    2
#define frame_invalid  3

CProtocol_ComScHandler::CProtocol_ComScHandler()
{
	m_pParent = NULL;
	m_pSubStation = NULL;
	memset(m_RECV_BUF, 0, 4096);
	m_RECV_Len = 0;

}

CProtocol_ComScHandler::~CProtocol_ComScHandler()
{

}

void CProtocol_ComScHandler::CopyBuf(BYTE* buf,int Len)
{
	m_RECV_Len = Len;
	memcpy(m_RECV_BUF, buf, Len);
}

void CProtocol_ComScHandler::DealMessage()
{
	

}

void CProtocol_ComScHandler::AnalyzePacket(unsigned char* FRAME_BUF, unsigned int FRAME_Len)
{

}


CSl427_ComScHandler::CSl427_ComScHandler()
{
	CProtocol_ComScHandler::CProtocol_ComScHandler();

}

CSl427_ComScHandler::~CSl427_ComScHandler()
{

}

void CSl427_ComScHandler::DealMessage()
{
	if (m_RECV_Len > 5)
	{
		int pos_BEGIN,pos_END;

		pos_BEGIN = -1;
		pos_END = -1;

		for (int idx = 0; idx < m_RECV_Len; idx ++)
		{
			if (m_RECV_BUF[idx] == 0x68 &&
				m_RECV_BUF[idx + 2] == 0x68 &&
				m_RECV_BUF[idx + 1] != 0)
			{
				int appframeLen = m_RECV_BUF[idx+1];     //长度
				if (m_RECV_Len > idx + appframeLen + 4)
				{
					if (CUdfGlobalFUNC::do_crc76520(&m_RECV_BUF[idx + 3],appframeLen)  == m_RECV_BUF[idx + appframeLen + 3]  &&
						m_RECV_BUF[idx + appframeLen + 4] == 0x16)
					{

						//正确帧报文数据
						unsigned char FRAME_BUF[SEND_BUFFER_LEN];			//帧报文
						unsigned int  FRAME_Len;							//帧长度

						//获取帧报文信息
						pos_BEGIN = idx;
						pos_END   = idx + appframeLen + 4;
						ZeroMemory(FRAME_BUF,sizeof(FRAME_BUF));
						memmove(FRAME_BUF,&m_RECV_BUF[idx],appframeLen + 5);
						FRAME_Len = appframeLen + 5;

						//处理报文
						AnalyzePacket(FRAME_BUF,FRAME_Len);

						//更新报文帧信息
						BYTE temp[4096];
						ZeroMemory(temp,sizeof(temp));
						memmove(temp,&m_RECV_BUF[pos_END +1],m_RECV_Len -(pos_END +1));
						ZeroMemory(m_RECV_BUF,sizeof(m_RECV_BUF));
						memmove(m_RECV_BUF,temp,m_RECV_Len -(pos_END +1));

						m_RECV_Len = m_RECV_Len -(pos_END +1);

						//递归处理
						DealMessage();
					}
				}
			}
		}
	}

}

void CSl427_ComScHandler::AnalyzePacket(unsigned char* FRAME_BUF, unsigned int FRAME_Len)
{
	if (m_pParent == NULL)
	{
		return; 
	}

	if (FRAME_BUF != NULL && FRAME_Len > 0)
	{
		CPacketFrameFormat waterFrame;
		if (!CUdfGlobalFUNC::IEC_waterdata_to_APPFRAME(FRAME_BUF,FRAME_Len,&waterFrame))
		{
			TRACE("报文帧格式分析不正确！\r\n");
			return;
		}

		CRtuStation* pSub =  GETAPP()->m_pReadDB->SearchRtuStation(waterFrame._addr_info.A1.ProvinceCode,
			waterFrame._addr_info.A1.RegionCode,
			waterFrame._addr_info.A2);

		if (pSub != NULL)
		{
			m_pSubStation = pSub;

			waterFrame.m_pStation = pSub;
			pSub->GetSysDataTrans()->AddRecvLog(FRAME_BUF,FRAME_Len,&waterFrame);
			pSub->SetLastRecvTime();
			waterFrame.UpdateRecvTime();

			//状态有改变，更新CMSRightView的显示
			if (pSub->SetCommState(Connected_State) == true)  
				pSub->GetSysDataTrans()->UpdateRtuCommState(pSub);


			if (m_pParent->GetScModule() != NULL && m_pParent->GetScModule()->GetCommThreadInfo() != NULL)
			{
				CCommThreadInfo* pCurThread = m_pParent->GetScModule()->GetCommThreadInfo();
				waterFrame.m_CommInterface	= pCurThread->GetCommInterfaceType();	
				if (pCurThread->GetComInfo() != NULL)
				{
					/*
					waterFrame.m_ComInfo.mBaund = pCurThread->GetComInfo()->mBaund;
					waterFrame.m_ComInfo.mComName = pCurThread->GetComInfo()->mComName;
					waterFrame.m_ComInfo.mDataBits = pCurThread->GetComInfo()->mDataBits;
					waterFrame.m_ComInfo.mParity = pCurThread->GetComInfo()->mParity;
					waterFrame.m_ComInfo.mStopBits = pCurThread->GetComInfo()->mStopBits;
					*/
					waterFrame.m_ComInfo = (*(pCurThread->GetComInfo()));
				}

				if (pCurThread->GetNetInfo() != NULL)
				{
					/*
					waterFrame.m_NetInfo.mIpAddress = pCurThread->GetNetInfo()->mIpAddress;
					waterFrame.m_NetInfo.mPort = pCurThread->GetNetInfo()->mPort;
					*/
					waterFrame.m_NetInfo = (*(pCurThread->GetNetInfo()));

				}

				if (pCurThread->GetScCommInfo() != NULL)
				{
					/*
					waterFrame.m_ScCommInfo.nScAddress = pCurThread->GetScCommInfo()->nScAddress;
					waterFrame.m_ScCommInfo.nFactoryType = pCurThread->GetScCommInfo()->nFactoryType;
					*/
					waterFrame.m_ScCommInfo = (*(pCurThread->GetScCommInfo()));
				}
			}

			//检测OldScMessage处理对象
			CSCMessageDealModule* pOldScMessageModule = pSub->GetScMessageDealModule();
			if (m_pParent  != pOldScMessageModule)
			{
				pSub->SetScMessageDealModule(m_pParent);
				if (pOldScMessageModule != NULL)
				{
					if (m_pParent->GetScModule() != NULL)
						m_pParent->GetScModule()->RemoveScMessgaeModule(pOldScMessageModule);
				}
			}

			pSub->SetCmType(SC_CommChannel);
			pSub->SetInterface(waterFrame.m_CommInterface);
			pSub->SetNetInfo(&(waterFrame.m_NetInfo));
			pSub->SetComInfo(&(waterFrame.m_ComInfo));
			pSub->SetScCommInfo(&(waterFrame.m_ScCommInfo));
			pSub->GetSysDataTrans()->Dispatcher(&waterFrame);
		}
	}

}

#define Frame_HeartBeat   1
#define Frame_Data        2
#define work_mode         1 //工作模式
#define debug_mode        2 //调试模式

CSCSW008_ComScHandler::CSCSW008_ComScHandler()
{
	CProtocol_ComScHandler::CProtocol_ComScHandler();
	m_nDivCnt = 0;
	m_recvFrameType = Frame_Data;
	m_msgMode = work_mode;
}

CSCSW008_ComScHandler::~CSCSW008_ComScHandler()
{

}

void CSCSW008_ComScHandler::DealMessage()
{
	int returnVar = frame_invalid;

	if (ParseLinkFrameMsg_Com())
	{
		returnVar = frame_scsw;
	}
	else
	{
		if (ParseLinkFrameWrite_Com())
		{
			returnVar = frame_write;
		}
		else
		{
			ZeroMemory(m_RECV_BUF, sizeof(m_RECV_BUF));
			m_RECV_Len = 0;
			returnVar = frame_invalid;
		}
	}

	if (frame_scsw == returnVar)
	{
		AnalyzePacket(m_FRAME_BUF,m_FRAME_Len);
		DealMessage();//递归调用
	}
	else if (frame_write == returnVar)
	{
		AnalyzePacket_write(m_FRAME_BUF,m_FRAME_Len);
		DealMessage();//递归调用
	}

	

// 	memset(m_FRAME_BUF,0,sizeof(m_FRAME_BUF));
// 	m_FRAME_Len = 0;
// 
// 	if (m_RECV_Len > 8)
// 	{
// 		int pos_BEGIN = -1;
// 		int pos_END = -1;
// 
// 		bool bFindHead = false;
// 		bool bFindTail = false;
// 		int  nMsgLen = 0;
// 		BOOL returnVar = FALSE;
// 
// 		for (int idx=0; idx<m_RECV_Len-1; idx++)
// 		{//使用m_Recv_Len是防止m_Recv_Buf越界
// 			if ('S'==m_RECV_BUF[idx] && 'T'==m_RECV_BUF[idx+1])
// 			{
// 				if (m_RECV_Len > idx+17)
// 				{
// 					if ('a'==m_RECV_BUF[idx+14] && 'a'==m_RECV_BUF[idx+15])
// 					{
// 						if ('N'!= m_RECV_BUF[idx+16] && 'N'!= m_RECV_BUF[idx+17])
// 						{
// 							pos_BEGIN = idx;
// 							pos_END = idx+15;
// 							m_recvFrameType = Frame_HeartBeat;
// 							break;
// 
// 						}
// 					}
// 				}
// 				else if (m_RECV_Len > idx+15)
// 				{
// 					if ('a'==m_RECV_BUF[idx+14] && 'a'==m_RECV_BUF[idx+15])
// 					{
// 						pos_BEGIN = idx;
// 						pos_END = idx+15;
// 						m_recvFrameType = Frame_HeartBeat;
// 						break;
// 
// 					}
// 				}
// 
// 				if (!bFindTail)
// 				{
// 					pos_BEGIN = idx;
// 					bFindHead = true;
// 				}
// 			}
// 			else if ('N'==m_RECV_BUF[idx] && 'N'==m_RECV_BUF[idx+1])
// 			{
// 				if (bFindHead)
// 				{
// 					if (':'==m_RECV_BUF[idx-1] && ':'==m_RECV_BUF[idx-2])
// 					{
// 						m_msgMode = debug_mode;
// 					}
// 					else
// 					{
// 						m_msgMode = work_mode;
// 
// 					}
// 
// 					pos_END = idx+1;
// 					bFindTail = true;
// 					m_recvFrameType = Frame_Data;
// 					break;
// 				}	
// 			}
// 		}
// 
// 		if (pos_BEGIN!=-1 && pos_END!=-1)
// 		{
// 			nMsgLen = pos_END - pos_BEGIN + 1;
// 			ZeroMemory(m_FRAME_BUF, sizeof(m_FRAME_BUF));
// 			memcpy(m_FRAME_BUF, &m_RECV_BUF[pos_BEGIN], nMsgLen);
// 			m_FRAME_Len = nMsgLen;
// 
// 			switch (m_recvFrameType)
// 			{
// 			case Frame_Data:
// 				{
// 					if (('a'==m_RECV_BUF[m_FRAME_Len-4] && 'a'==m_RECV_BUF[m_FRAME_Len-3])
// 						|| (':'==m_RECV_BUF[m_FRAME_Len-4] && ':'==m_RECV_BUF[m_FRAME_Len-3]))
// 					{//如果校验码是aa，表明当前指令为【查询指令】
// 						BYTE temp[RECV_BUFFER_LEN];
// 						ZeroMemory(temp, sizeof(temp));
// 						memmove(temp, &m_RECV_BUF[pos_END +1],m_RECV_Len -(pos_END +1));
// 						ZeroMemory(m_RECV_BUF,sizeof(m_RECV_BUF));
// 						memmove(m_RECV_BUF,temp,m_RECV_Len -(pos_END +1));
// 
// 						m_RECV_Len = m_RECV_Len -(pos_END +1);
// 						returnVar = TRUE;
// 
// 					}
// 					else
// 					{
// 						if (CUdfGlobalFUNC::CheckCRC_SCSW(m_FRAME_BUF, m_FRAME_Len))
// 						{
// 							BYTE temp[RECV_BUFFER_LEN];
// 							ZeroMemory(temp, sizeof(temp));
// 							memmove(temp, &m_RECV_BUF[pos_END +1],m_RECV_Len -(pos_END +1));
// 							ZeroMemory(m_RECV_BUF,sizeof(m_RECV_BUF));
// 							memmove(m_RECV_BUF,temp,m_RECV_Len -(pos_END +1));
// 
// 							m_RECV_Len = m_RECV_Len -(pos_END +1);
// 							returnVar = TRUE;
// 						}
// 						else
// 						{
// 							ZeroMemory(m_FRAME_BUF, sizeof(m_FRAME_BUF));
// 							m_FRAME_Len = 0;
// 
// 							BYTE temp[RECV_BUFFER_LEN];
// 							ZeroMemory(temp, sizeof(temp));
// 							memmove(temp, &m_RECV_BUF[pos_END +1],m_RECV_Len -(pos_END +1));
// 							ZeroMemory(m_RECV_BUF,sizeof(m_RECV_BUF));
// 							memmove(m_RECV_BUF,temp,m_RECV_Len -(pos_END +1));
// 
// 							m_RECV_Len = m_RECV_Len -(pos_END +1);
// 						}
// 					}
// 
// 				}
// 				break;
// 			case Frame_HeartBeat:
// 				{
// 					BYTE temp[RECV_BUFFER_LEN];
// 					ZeroMemory(temp, sizeof(temp));
// 					memmove(temp, &m_RECV_BUF[pos_END +1],m_RECV_Len -(pos_END +1));
// 					ZeroMemory(m_RECV_BUF,sizeof(m_RECV_BUF));
// 					memmove(m_RECV_BUF,temp,m_RECV_Len -(pos_END +1));
// 
// 					m_RECV_Len = m_RECV_Len -(pos_END +1);
// 					returnVar = TRUE;
// 
// 				}
// 				break;
// 			}
// 
// 
// 		}
// 		else if (-1==pos_BEGIN && -1==pos_END)
// 		{
// 			ZeroMemory(m_RECV_BUF, sizeof(m_RECV_BUF));
// 			m_RECV_Len = 0;
// 
// 		}
// 
// 		
// 	}
}

void CSCSW008_ComScHandler::AnalyzePacket(unsigned char* buffer, unsigned int len)
{
	if (m_pParent == NULL)
	{
		return; 
	}

	if (buffer != NULL && len > 0)
	{
		CCMSApp* pApp = (CCMSApp*)AfxGetApp();
		CRealTimeDB* pRealDb = pApp->m_pReadDB;
		if (!pRealDb)  return ;

		CHydrFrameFormat HydrFrame;

		if (!CUdfGlobalFUNC::SCSW008_hydrologic_to_APPFRAME(buffer, len, &HydrFrame))
		{
			TRACE("报文帧格式分析不正确！\r\n");
			return;
		}

		CRtuStation* pSub = pRealDb->SearchRtuStation(HydrFrame.m_strStCode, SCSW_008_Protocol);

		if (pSub != NULL)
		{
			m_pSubStation = pSub;

			HydrFrame.m_pStation = pSub;
			HydrFrame.UpdateRecvTime();
			pSub->GetSysDataTrans_Hydr()->AddRecvLog(buffer, len, &HydrFrame);
			pSub->SetLastRecvTime();

			//状态有改变，更新CMSRightView的显示
			if (pSub->SetCommState(Connected_State) == true)  
				pSub->GetSysDataTrans_Hydr()->UpdateRtuCommState(pSub);


			if (m_pParent->GetScModule() != NULL && m_pParent->GetScModule()->GetCommThreadInfo() != NULL)
			{
				CCommThreadInfo* pCurThread = m_pParent->GetScModule()->GetCommThreadInfo();
				HydrFrame.m_CommInterface	= pCurThread->GetCommInterfaceType();	
				if (pCurThread->GetComInfo() != NULL)
				{
					HydrFrame.m_ComInfo = (*(pCurThread->GetComInfo()));
				}

				if (pCurThread->GetNetInfo() != NULL)
				{
					HydrFrame.m_NetInfo = (*(pCurThread->GetNetInfo()));
				}

				if (pCurThread->GetScCommInfo() != NULL)
				{
					HydrFrame.m_ScCommInfo = (*(pCurThread->GetScCommInfo()));
				}
			}

			//检测OldScMessage处理对象
			CSCMessageDealModule* pOldScMessageModule = pSub->GetScMessageDealModule();
			if (m_pParent  != pOldScMessageModule)
			{
				pSub->SetScMessageDealModule(m_pParent);
				if (pOldScMessageModule != NULL)
				{
					if (m_pParent->GetScModule() != NULL)
						m_pParent->GetScModule()->RemoveScMessgaeModule(pOldScMessageModule);
				}
			}

			pSub->SetCmType(SC_CommChannel);
			pSub->SetInterface(HydrFrame.m_CommInterface);
			pSub->SetNetInfo(&(HydrFrame.m_NetInfo));
			pSub->SetComInfo(&(HydrFrame.m_ComInfo));
			pSub->SetScCommInfo(&(HydrFrame.m_ScCommInfo));
			
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
	}
}

void CSCSW008_ComScHandler::AnalyzePacket_write(unsigned char* FRAME_BUF, unsigned int FRAME_Len)
{
	if (NULL == m_pParent)
		return;

	CWriteFrame writeFrame;
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	CRealTimeDB* pRealDb = pApp->m_pReadDB;
	if (!pRealDb)  return ;

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

		if (m_pParent->GetScModule()!=NULL && m_pParent->GetScModule()->GetCommThreadInfo()!=NULL)
		{
			CCommThreadInfo *pCurThread = m_pParent->GetScModule()->GetCommThreadInfo();
			writeFrame.m_CommInterface = pCurThread->GetCommInterfaceType();

			if (pCurThread->GetComInfo() != NULL)
			{
				writeFrame.m_ComInfo = (*(pCurThread->GetComInfo()));
			}

			if (pCurThread->GetNetInfo() != NULL)
			{
				writeFrame.m_NetInfo = (*(pCurThread->GetNetInfo()));
			}

			if (pCurThread->GetScCommInfo() != NULL)
			{
				writeFrame.m_ScCommInfo = (*(pCurThread->GetScCommInfo()));
			}
		}

		//检测OldScMessage处理对象
		CSCMessageDealModule* pOldScMessageModule = pSub->GetScMessageDealModule();
		if (m_pParent  != pOldScMessageModule)
		{
			pSub->SetScMessageDealModule(m_pParent);
			if (pOldScMessageModule != NULL)
			{
				if (m_pParent->GetScModule() != NULL)
					m_pParent->GetScModule()->RemoveScMessgaeModule(pOldScMessageModule);
			}
		}

		pSub->SetCmType(SC_CommChannel);
		pSub->SetInterface(writeFrame.m_CommInterface);
		pSub->SetNetInfo(&(writeFrame.m_NetInfo));
		pSub->SetComInfo(&(writeFrame.m_ComInfo));
		pSub->SetScCommInfo(&(writeFrame.m_ScCommInfo));

		pSub->GetSysDataTrans_WriteUpdate()->SetRtuStation(pSub);
		pSub->GetSysDataTrans_WriteUpdate()->UpdateRtuCommInfo(pSub);
		pSub->GetSysDataTrans_WriteUpdate()->Dispatcher(&writeFrame);

	}
}

BOOL CSCSW008_ComScHandler::ParseLinkFrameMsg_Com()
{
	memset(m_FRAME_BUF,0,sizeof(m_FRAME_BUF));
	m_FRAME_Len = 0;

	BOOL returnVar = FALSE;

	if (m_RECV_Len > 8)
	{
		int pos_BEGIN = -1;
		int pos_END = -1;

		bool bFindHead = false;
		bool bFindTail = false;
		int  nMsgLen = 0;
		

		for (int idx=0; idx<m_RECV_Len-1; idx++)
		{
			if ('S'==m_RECV_BUF[idx] && 'T'==m_RECV_BUF[idx+1])
			{
				if (m_RECV_Len > idx+17)
				{
					if ('a'==m_RECV_BUF[idx+14] && 'a'==m_RECV_BUF[idx+15])
					{
						if ('N'!= m_RECV_BUF[idx+16] && 'N'!= m_RECV_BUF[idx+17])
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
					if ('a'==m_RECV_BUF[idx+14] && 'a'==m_RECV_BUF[idx+15])
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
			else if ('N'==m_RECV_BUF[idx] && 'N'==m_RECV_BUF[idx+1])
			{
				if (bFindHead)
				{
					if (':'==m_RECV_BUF[idx-1] && ':'==m_RECV_BUF[idx-2])
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
			memcpy(m_FRAME_BUF, &m_RECV_BUF[pos_BEGIN], nMsgLen);
			m_FRAME_Len = nMsgLen;

			switch (m_recvFrameType)
			{
			case Frame_Data:
				{
					if (('a'==m_RECV_BUF[m_FRAME_Len-4] && 'a'==m_RECV_BUF[m_FRAME_Len-3])
						|| (':'==m_RECV_BUF[m_FRAME_Len-4] && ':'==m_RECV_BUF[m_FRAME_Len-3]))
					{//如果校验码是aa，表明当前指令为【查询指令】
						BYTE temp[RECV_BUFFER_LEN];
						ZeroMemory(temp, sizeof(temp));
						memmove(temp, &m_RECV_BUF[pos_END +1],m_RECV_Len -(pos_END +1));
						ZeroMemory(m_RECV_BUF,sizeof(m_RECV_BUF));
						memmove(m_RECV_BUF,temp,m_RECV_Len -(pos_END +1));

						m_RECV_Len = m_RECV_Len -(pos_END +1);
						returnVar = TRUE;

					}
					else
					{
						if (CUdfGlobalFUNC::CheckCRC_SCSW(m_FRAME_BUF, m_FRAME_Len))
						{
							BYTE temp[RECV_BUFFER_LEN];
							ZeroMemory(temp, sizeof(temp));
							memmove(temp, &m_RECV_BUF[pos_END +1],m_RECV_Len -(pos_END +1));
							ZeroMemory(m_RECV_BUF,sizeof(m_RECV_BUF));
							memmove(m_RECV_BUF,temp,m_RECV_Len -(pos_END +1));

							m_RECV_Len = m_RECV_Len -(pos_END +1);
							returnVar = TRUE;
						}
						else
						{
							ZeroMemory(m_FRAME_BUF, sizeof(m_FRAME_BUF));
							m_FRAME_Len = 0;

							BYTE temp[RECV_BUFFER_LEN];
							ZeroMemory(temp, sizeof(temp));
							memmove(temp, &m_RECV_BUF[pos_END +1],m_RECV_Len -(pos_END +1));
							ZeroMemory(m_RECV_BUF,sizeof(m_RECV_BUF));
							memmove(m_RECV_BUF,temp,m_RECV_Len -(pos_END +1));

							m_RECV_Len = m_RECV_Len -(pos_END +1);
						}
					}

				}
				break;
			case Frame_HeartBeat:
				{
					BYTE temp[RECV_BUFFER_LEN];
					ZeroMemory(temp, sizeof(temp));
					memmove(temp, &m_RECV_BUF[pos_END +1],m_RECV_Len -(pos_END +1));
					ZeroMemory(m_RECV_BUF,sizeof(m_RECV_BUF));
					memmove(m_RECV_BUF,temp,m_RECV_Len -(pos_END +1));

					m_RECV_Len = m_RECV_Len -(pos_END +1);
					returnVar = TRUE;

				}
				break;
			}
		}
		else if (-1==pos_BEGIN && -1==pos_END)
		{
			ZeroMemory(m_RECV_BUF, sizeof(m_RECV_BUF));
			m_RECV_Len = 0;

		}
	}

	return returnVar;
}

BOOL CSCSW008_ComScHandler::ParseLinkFrameWrite_Com()
{

	int pos_BEGIN = -1;
	int pos_END = -1;

	bool bFindHead = false;
	bool bFindTail = false;
	int  nMsgLen = 0;
	BOOL returnVar = FALSE;

	for (int idx=0; idx<m_RECV_Len-1; idx++)
	{//使用m_Recv_Len是防止m_Recv_Buf越界
		if ('$'==m_RECV_BUF[idx] && '$'==m_RECV_BUF[idx+1])
		{
			int writeFrameLen = m_RECV_BUF[idx+2];
			if (m_RECV_Len >= writeFrameLen)
			{
				if ('#'==m_RECV_BUF[idx+writeFrameLen-1] && '#'==m_RECV_BUF[idx+writeFrameLen-2])
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
		else if ('#'==m_RECV_BUF[idx] && '#'==m_RECV_BUF[idx+1])
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
		memcpy(m_FRAME_BUF, &m_RECV_BUF[pos_BEGIN], nMsgLen);
		m_FRAME_Len = nMsgLen;

		if (CUdfGlobalFUNC::CheckCRC_Write(m_FRAME_BUF, m_FRAME_Len))
		{
			BYTE temp[RECV_BUFFER_LEN];
			ZeroMemory(temp, sizeof(temp));
			memmove(temp, &m_RECV_BUF[pos_END +1],m_RECV_Len -(pos_END +1));
			ZeroMemory(m_RECV_BUF,sizeof(m_RECV_BUF));
			memmove(m_RECV_BUF,temp,m_RECV_Len -(pos_END +1));

			m_RECV_Len = m_RECV_Len -(pos_END +1);
			returnVar = TRUE;
		}
		else
		{
			ZeroMemory(m_FRAME_BUF, sizeof(m_FRAME_BUF));
			m_FRAME_Len = 0;

			BYTE temp[RECV_BUFFER_LEN];
			ZeroMemory(temp, sizeof(temp));
			memmove(temp, &m_RECV_BUF[pos_END +1],m_RECV_Len -(pos_END +1));
			ZeroMemory(m_RECV_BUF,sizeof(m_RECV_BUF));
			memmove(m_RECV_BUF,temp,m_RECV_Len -(pos_END +1));

			m_RECV_Len = m_RECV_Len -(pos_END +1);
		}

	}
	else if (-1==pos_BEGIN && -1==pos_END)
	{
		ZeroMemory(m_RECV_BUF, sizeof(m_RECV_BUF));
		m_RECV_Len = 0;
	}

	return returnVar;

}



