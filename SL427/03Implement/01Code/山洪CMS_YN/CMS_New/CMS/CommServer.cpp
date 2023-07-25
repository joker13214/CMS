#include "StdAfx.h"
#include "CommServer.h"
#include "WaitTimer.h"
#include "CommServerMnDlg.h"
#include "CMS.h"
#include "MainFrm.h"

#define frame_scsw     1
#define frame_write    2
#define frame_invalid  3

#define Frame_HeartBeat   1
#define Frame_Data        2
#define work_mode         1 //工作模式
#define debug_mode        2 //调试模式

en_Frame_Class::en_Frame_Class()
{
	memset (m_Send_Buf,0,sizeof(m_Send_Buf));
	m_Send_Len = 0;
	m_Cm_Interface = Serial_CommInterface;
}

/*
*函数介绍：en_Frame_Class 构造函数
*输入参数：sndbuf 下发的报文，len 报文长度
*输出参数：无
*返回值：
*/
en_Frame_Class::en_Frame_Class(BYTE *sndBuf,int len)
{
	memset (m_Send_Buf,0,sizeof(m_Send_Buf));
	m_Send_Len = 0;
	m_Cm_Interface = Serial_CommInterface;

	SetSendBuf(sndBuf,len);
}


/*
*函数介绍：en_Frame_Class 构造函数
*输入参数：sndbuf 下发的报文，len 报文长度，pToNetInfo 目的IP地址和端口信息
*输出参数：无
*返回值：
*/
en_Frame_Class::en_Frame_Class(BYTE *Sndbuf,int len,PNET_INFO pToNetInfo)
{
	memset(m_Send_Buf,0,sizeof(m_Send_Buf));
	m_Send_Len = 0;
	m_Cm_Interface = Ethernet_CommInerface;
	
	SetSendBuf(Sndbuf,len);
	SetToNetInfo(pToNetInfo);
}

void en_Frame_Class::SetSendBuf(BYTE* sndBuf,int len)
{
	if (sndBuf)
	{
		memmove(m_Send_Buf,sndBuf,len);
		m_Send_Len = len;
	}
}

void en_Frame_Class::SetToNetInfo(PNET_INFO pNet_inf)
{
	if (pNet_inf)
	{
		m_To_Net_Info.mIpAddress = pNet_inf->mIpAddress;
		m_To_Net_Info.mPort		 = pNet_inf->mPort;
	}
}

CCommThreadInfo::CCommThreadInfo()
{
	m_bThreadControl = false;
	m_handle_Thread  = NULL;
	m_dwThreadID	 = 0;
	m_pHandle_Net    = NULL;
	m_pHandle_Com    = NULL;
	m_pScModule		 = NULL;
	m_bThreadState   = THREAD_STATE_STOP;

	m_CmInterface	 = Serial_CommInterface;
	m_CmType		 = 1;
	m_pCommServer	 = NULL;

	m_ProtocolType = UDP_Protocol;

	m_strThreadName	= "";

	//m_nScAddress_Local = 0;
}

CCommThreadInfo::~CCommThreadInfo()
{
	while (!m_lstEnfrmaes.IsEmpty())
	{
		en_Frame_Class* pEn= m_lstEnfrmaes.RemoveHead();
		if (pEn)
		{
			delete pEn;
			pEn = NULL;
		}
	}
}

void CCommThreadInfo::SetComInfo(PCOM_INFO pInfo)
{
	if (pInfo)
	{
		m_stComInfo.mBaund		= pInfo->mBaund;
		m_stComInfo.mComName	= pInfo->mComName;
		m_stComInfo.mDataBits	= pInfo->mDataBits;
		m_stComInfo.mParity		= pInfo->mParity;
		m_stComInfo.mStopBits	= pInfo->mStopBits;
	}
}

void CCommThreadInfo::SetScCommInfo(PSC_COMM_INFO pInfo)
{
	if (pInfo)
	{
		m_stSCCommInfo.nScAddress = pInfo->nScAddress;
		m_stSCCommInfo.nFactoryType  = pInfo->nFactoryType;
	}
}

void CCommThreadInfo::SetNetInfo(PNET_INFO pInfo)
{
	if (pInfo)
	{
		m_stNetInfo.mIpAddress	= pInfo->mIpAddress;
		m_stNetInfo.mPort		= pInfo->mPort;
	}
}

void CCommThreadInfo::ThreadReset()
{
	SetHandle_Com(NULL);
	SetHandle_Net(NULL);
	SetHandle_TcpServer(NULL);
	SetScModule(NULL);
	SetThreadHandle(NULL);
	SetThreadID(0);
	SetThreadState(THREAD_STATE_STOP);
}


void CCommThreadInfo::FrameBufDeal(BYTE* buf,int nLen,BYTE CmInterface/* =Serial_CommInterface */, CString FromIP/* ="" */,int FromPort/* =0 */)
{
	//报文处理
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	CRealTimeDB* pRealDb = pApp->m_pReadDB;
	if (!pRealDb)  return ;

	CPacketFrameFormat waterFrame;
	if (!CUdfGlobalFUNC::IEC_waterdata_to_APPFRAME(buf,nLen,&waterFrame))
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

		pSub->GetSysDataTrans()->AddRecvLog(buf,nLen,&waterFrame);	
		pSub->SetLastRecvTime();
		waterFrame.UpdateRecvTime();

		//状态有改变，更新CMSRightView的显示
		if (pSub->SetCommState(Connected_State) == true) 
			pSub->GetSysDataTrans()->UpdateRtuCommState(pSub);

		waterFrame.m_CommInterface	= m_CmInterface;
		waterFrame.m_ProtocolType	= m_ProtocolType;

		pSub->SetCmType(m_CmType);
		pSub->SetInterface(CmInterface);
		pSub->SetProtocolType(m_ProtocolType);
		if (CmInterface == Serial_CommInterface)
		{
			pSub->SetHandle_Com(m_pHandle_Com);

			if (m_pHandle_Com)
				m_pHandle_Com->AddRelateStation(pSub);
				//m_pHandle_Com->SetRelateStation(pSub);

			waterFrame.m_ComInfo.mComName	= m_stComInfo.mComName;
			waterFrame.m_ComInfo.mBaund		= m_stComInfo.mBaund;
			waterFrame.m_ComInfo.mDataBits	= m_stComInfo.mDataBits;
			waterFrame.m_ComInfo.mParity	= m_stComInfo.mParity;
			waterFrame.m_ComInfo.mStopBits	= m_stComInfo.mStopBits;

			pSub->SetComPortA(m_stComInfo.mComName);
			pSub->SetBaundA(m_stComInfo.mBaund);
			pSub->SetDataBitsA(m_stComInfo.mDataBits);
			pSub->SetStopBitsA(m_stComInfo.mStopBits);
			pSub->SetParityA(m_stComInfo.mParity);
		}
		else if (CmInterface == Ethernet_CommInerface)
		{
			pSub->SetHandle_Net(m_pHandle_Net);
			if (m_pHandle_Net)
				m_pHandle_Net->AddRelateStation(pSub);

			if(pSub->GetIpAddrA() != FromIP)
				pSub->SetIpAddrA(FromIP);

			if (pSub->GetSockPortA() != FromPort)
				pSub->SetSockPortA(FromPort);

			waterFrame.m_NetInfo.mIpAddress = FromIP;
			waterFrame.m_NetInfo.mPort      = FromPort;
		}

		pSub->GetSysDataTrans()->UpdateRtuCommInfo(pSub);
		pSub->GetSysDataTrans()->Dispatcher(&waterFrame);
	}
}

void CCommThreadInfo::FrameBufDeal_SCSW008(BYTE* buf,int nLen, int FrameType, BYTE CmInterface,CString FromIP,int FromPort)
{
	if (!buf || nLen <= 0 )
		return;

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	CRealTimeDB* pRealDb = pApp->m_pReadDB;
	if (!pRealDb)  return ;

	if (FrameType == frame_scsw)
	{
		CHydrFrameFormat HydrFrame;

		if (!CUdfGlobalFUNC::SCSW008_hydrologic_to_APPFRAME(buf, nLen, &HydrFrame))
		{
			TRACE("报文帧格式分析不正确！\r\n");
			return;
		}

		CRtuStation* pSub = pRealDb->SearchRtuStation(HydrFrame.m_strStCode, SCSW_008_Protocol);

		if (pSub != NULL)
		{
			HydrFrame.m_pStation = pSub;
			HydrFrame.UpdateRecvTime();
			pSub->GetSysDataTrans_Hydr()->AddRecvLog(buf, nLen, &HydrFrame);
			pSub->SetLastRecvTime();

			

			//状态有改变，更新CMSRightView的显示
			if (pSub->SetCommState(Connected_State) == true)  
				pSub->GetSysDataTrans_Hydr()->UpdateRtuCommState(pSub);

			HydrFrame.m_CommInterface	= m_CmInterface;
			HydrFrame.m_ProtocolType	= m_ProtocolType;

			pSub->SetCmType(m_CmType);
			pSub->SetInterface(CmInterface);
			pSub->SetProtocolType(m_ProtocolType);

			if (CmInterface == Serial_CommInterface)
			{
				pSub->SetHandle_Com(m_pHandle_Com);

				if (m_pHandle_Com)
					m_pHandle_Com->AddRelateStation(pSub);

				HydrFrame.m_ComInfo.mComName	= m_stComInfo.mComName;
				HydrFrame.m_ComInfo.mBaund		= m_stComInfo.mBaund;
				HydrFrame.m_ComInfo.mDataBits	= m_stComInfo.mDataBits;
				HydrFrame.m_ComInfo.mParity	= m_stComInfo.mParity;
				HydrFrame.m_ComInfo.mStopBits	= m_stComInfo.mStopBits;

				pSub->SetComPortA(m_stComInfo.mComName);
				pSub->SetBaundA(m_stComInfo.mBaund);
				pSub->SetDataBitsA(m_stComInfo.mDataBits);
				pSub->SetStopBitsA(m_stComInfo.mStopBits);
				pSub->SetParityA(m_stComInfo.mParity);
			}
			else if (CmInterface == Ethernet_CommInerface)
			{
				pSub->SetHandle_Net(m_pHandle_Net);
				if (m_pHandle_Net)
					m_pHandle_Net->AddRelateStation(pSub);

				if(pSub->GetIpAddrA() != FromIP)
					pSub->SetIpAddrA(FromIP);

				if (pSub->GetSockPortA() != FromPort)
					pSub->SetSockPortA(FromPort);

				HydrFrame.m_NetInfo.mIpAddress = FromIP;
				HydrFrame.m_NetInfo.mPort      = FromPort;
			}


			if (GetHandle_Com()->m_recvFrameType == Frame_Data)
			{
				if (GetHandle_Com()->m_msgMode == work_mode)
				{
					HydrFrame.m_workmode = 1;
				}
				else if (GetHandle_Com()->m_msgMode == debug_mode)
				{ 
					HydrFrame.m_workmode = 2;
				}

				pSub->GetSysDataTrans_Hydr()->UpdateRtuCommInfo(pSub);
				pSub->GetSysDataTrans_Hydr()->Dispatcher(&HydrFrame);
			}
			else if (GetHandle_Com()->m_recvFrameType == Frame_HeartBeat)
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
	else if (FrameType == frame_write)
	{
		CWriteFrame writeFrame;

		if (!CUdfGlobalFUNC::WRITE_frame_to_APPFRAME(buf, nLen, &writeFrame))
		{
			TRACE("报文帧格式分析不正确！\r\n");
			return;
		}

		CRtuStation* pSub = pRealDb->SearchRtuStation(writeFrame.m_nDevAdddr, SCSW_008_Protocol);

		if (pSub != NULL)
		{
			writeFrame.m_pStation = pSub;
			writeFrame.UpdateRecvTime();
			pSub->GetSysDataTrans_WriteUpdate()->AddRecvLog(buf, nLen, &writeFrame);
			pSub->SetLastRecvTime();


			if (pSub->SetCommState(Connected_State) == true)  //状态有改变，更新CMSRightView的显示
				pSub->GetSysDataTrans()->UpdateRtuCommState(pSub);

			writeFrame.m_CommInterface	= m_CmInterface;
			writeFrame.m_ProtocolType	= m_ProtocolType;

			pSub->SetCmType(m_CmType);
			pSub->SetInterface(CmInterface);
			pSub->SetProtocolType(m_ProtocolType);

			if (CmInterface == Serial_CommInterface)
			{
				pSub->SetHandle_Com(m_pHandle_Com);

				if (m_pHandle_Com)
					m_pHandle_Com->AddRelateStation(pSub);

				writeFrame.m_ComInfo.mComName	= m_stComInfo.mComName;
				writeFrame.m_ComInfo.mBaund		= m_stComInfo.mBaund;
				writeFrame.m_ComInfo.mDataBits	= m_stComInfo.mDataBits;
				writeFrame.m_ComInfo.mParity	= m_stComInfo.mParity;
				writeFrame.m_ComInfo.mStopBits	= m_stComInfo.mStopBits;

				pSub->SetComPortA(m_stComInfo.mComName);
				pSub->SetBaundA(m_stComInfo.mBaund);
				pSub->SetDataBitsA(m_stComInfo.mDataBits);
				pSub->SetStopBitsA(m_stComInfo.mStopBits);
				pSub->SetParityA(m_stComInfo.mParity);
			}
			else if (CmInterface == Ethernet_CommInerface)
			{
				pSub->SetHandle_Net(m_pHandle_Net);
				if (m_pHandle_Net)
					m_pHandle_Net->AddRelateStation(pSub);

				if(pSub->GetIpAddrA() != FromIP)
					pSub->SetIpAddrA(FromIP);

				if (pSub->GetSockPortA() != FromPort)
					pSub->SetSockPortA(FromPort);

				writeFrame.m_NetInfo.mIpAddress = FromIP;
				writeFrame.m_NetInfo.mPort      = FromPort;
			}


			pSub->GetSysDataTrans_WriteUpdate()->SetRtuStation(pSub);
			pSub->GetSysDataTrans_WriteUpdate()->UpdateRtuCommInfo(pSub);
			pSub->GetSysDataTrans_WriteUpdate()->Dispatcher(&writeFrame);

		}
	}
	else if (FrameType == frame_invalid)
	{
	}

}

void CCommThreadInfo::FrameBufDeal_SYQ(BYTE* buf,int nLen, int FrameType, BYTE CmInterface,CString FromIP,int FromPort)
{
	if (!buf || nLen <= 0 )
		return;

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	CRealTimeDB* pRealDb = pApp->m_pReadDB;
	if (!pRealDb)  return ;

	if (FrameType == frame_scsw)
	{
		CSYQFrameFormat syqFrame;

		if (!CUdfGlobalFUNC::SYQ_to_APPFRAME(buf, nLen, &syqFrame))
		{
			TRACE("报文帧格式分析不正确！\r\n");
			return;
		}

		CRtuStation* pSub = pRealDb->SearchRtuStation(syqFrame.m_stCode, SYQYC_Comm_Protocol);

		if (pSub != NULL)
		{
			syqFrame.m_pStation = pSub;
			syqFrame.UpdateRecvTime();
			pSub->GetSysDataTrans_Syq()->AddRecvLog(buf, nLen, &syqFrame);
			pSub->SetLastRecvTime();

			

			//状态有改变，更新CMSRightView的显示
			if (pSub->SetCommState(Connected_State) == true)  
				pSub->GetSysDataTrans_Syq()->UpdateRtuCommState(pSub);

			syqFrame.m_CommInterface	= m_CmInterface;
			syqFrame.m_ProtocolType	= m_ProtocolType;

			pSub->SetCmType(m_CmType);
			pSub->SetInterface(CmInterface);
			pSub->SetProtocolType(m_ProtocolType);

			if (CmInterface == Serial_CommInterface)
			{
				pSub->SetHandle_Com(m_pHandle_Com);

				if (m_pHandle_Com)
					m_pHandle_Com->AddRelateStation(pSub);

				syqFrame.m_ComInfo.mComName	= m_stComInfo.mComName;
				syqFrame.m_ComInfo.mBaund		= m_stComInfo.mBaund;
				syqFrame.m_ComInfo.mDataBits	= m_stComInfo.mDataBits;
				syqFrame.m_ComInfo.mParity	= m_stComInfo.mParity;
				syqFrame.m_ComInfo.mStopBits	= m_stComInfo.mStopBits;

				pSub->SetComPortA(m_stComInfo.mComName);
				pSub->SetBaundA(m_stComInfo.mBaund);
				pSub->SetDataBitsA(m_stComInfo.mDataBits);
				pSub->SetStopBitsA(m_stComInfo.mStopBits);
				pSub->SetParityA(m_stComInfo.mParity);
			}
			else if (CmInterface == Ethernet_CommInerface)
			{
				pSub->SetHandle_Net(m_pHandle_Net);
				if (m_pHandle_Net)
					m_pHandle_Net->AddRelateStation(pSub);

				if(pSub->GetIpAddrA() != FromIP)
					pSub->SetIpAddrA(FromIP);

				if (pSub->GetSockPortA() != FromPort)
					pSub->SetSockPortA(FromPort);

				syqFrame.m_NetInfo.mIpAddress = FromIP;
				syqFrame.m_NetInfo.mPort      = FromPort;
			}

				pSub->GetSysDataTrans_Syq()->UpdateRtuCommInfo(pSub);
				pSub->GetSysDataTrans_Syq()->Dispatcher(&syqFrame);
		}

	}
	else if (FrameType == frame_write)
	{
		CWriteFrame writeFrame;

		if (!CUdfGlobalFUNC::WRITE_frame_to_APPFRAME(buf, nLen, &writeFrame))
		{
			TRACE("报文帧格式分析不正确！\r\n");
			return;
		}

		CRtuStation* pSub = pRealDb->SearchRtuStation(writeFrame.m_nDevAdddr, SCSW_008_Protocol);

		if (pSub != NULL)
		{
			writeFrame.m_pStation = pSub;
			writeFrame.UpdateRecvTime();
			pSub->GetSysDataTrans_WriteUpdate()->AddRecvLog(buf, nLen, &writeFrame);
			pSub->SetLastRecvTime();


			if (pSub->SetCommState(Connected_State) == true)  //状态有改变，更新CMSRightView的显示
				pSub->GetSysDataTrans()->UpdateRtuCommState(pSub);

			writeFrame.m_CommInterface	= m_CmInterface;
			writeFrame.m_ProtocolType	= m_ProtocolType;

			pSub->SetCmType(m_CmType);
			pSub->SetInterface(CmInterface);
			pSub->SetProtocolType(m_ProtocolType);

			if (CmInterface == Serial_CommInterface)
			{
				pSub->SetHandle_Com(m_pHandle_Com);

				if (m_pHandle_Com)
					m_pHandle_Com->AddRelateStation(pSub);

				writeFrame.m_ComInfo.mComName	= m_stComInfo.mComName;
				writeFrame.m_ComInfo.mBaund		= m_stComInfo.mBaund;
				writeFrame.m_ComInfo.mDataBits	= m_stComInfo.mDataBits;
				writeFrame.m_ComInfo.mParity	= m_stComInfo.mParity;
				writeFrame.m_ComInfo.mStopBits	= m_stComInfo.mStopBits;

				pSub->SetComPortA(m_stComInfo.mComName);
				pSub->SetBaundA(m_stComInfo.mBaund);
				pSub->SetDataBitsA(m_stComInfo.mDataBits);
				pSub->SetStopBitsA(m_stComInfo.mStopBits);
				pSub->SetParityA(m_stComInfo.mParity);
			}
			else if (CmInterface == Ethernet_CommInerface)
			{
				pSub->SetHandle_Net(m_pHandle_Net);
				if (m_pHandle_Net)
					m_pHandle_Net->AddRelateStation(pSub);

				if(pSub->GetIpAddrA() != FromIP)
					pSub->SetIpAddrA(FromIP);

				if (pSub->GetSockPortA() != FromPort)
					pSub->SetSockPortA(FromPort);

				writeFrame.m_NetInfo.mIpAddress = FromIP;
				writeFrame.m_NetInfo.mPort      = FromPort;
			}


			pSub->GetSysDataTrans_WriteUpdate()->SetRtuStation(pSub);
			pSub->GetSysDataTrans_WriteUpdate()->UpdateRtuCommInfo(pSub);
			pSub->GetSysDataTrans_WriteUpdate()->Dispatcher(&writeFrame);

		}
	}

}


const INT_PTR CCommThreadInfo::GetEnframeCount()
{
	INT_PTR nCount = 0;

	m_Mutex_lstEnFrames.Lock();
	nCount = m_lstEnfrmaes.GetCount();
	m_Mutex_lstEnFrames.Unlock();

	return nCount;
}

en_Frame_Class* CCommThreadInfo::GetOneEnFrameClass()
{
	en_Frame_Class* pEn_Frame_class = NULL;
	
	m_Mutex_lstEnFrames.Lock();
	if (!m_lstEnfrmaes.IsEmpty())
	{
		pEn_Frame_class = m_lstEnfrmaes.RemoveHead();
	}
	m_Mutex_lstEnFrames.Unlock();

	return pEn_Frame_class;
}

void CCommThreadInfo::SendData(BYTE* sndbuf,int len, BYTE Interface/* =2 */, PNET_INFO pNet_Info /* = NULL */)
{
	en_Frame_Class* pNew =new en_Frame_Class(sndbuf,len);
	if (pNew)
	{
		pNew->SetInterface(Interface);
		pNew->SetToNetInfo(pNet_Info);

		m_Mutex_lstEnFrames.Lock();
		m_lstEnfrmaes.AddTail(pNew);
		m_Mutex_lstEnFrames.Unlock();
	}
}

const CString CCommThreadInfo::GetThreadAllInfo()
{
	CString strResult = "";

	CString strInterface = "";
	if (GetCommInterfaceType() == Ethernet_CommInerface)
		strInterface.Format("SOCKET:%s,%d\r\n",GetNetInfo()->mIpAddress,GetNetInfo()->mPort);
	else if (GetCommInterfaceType() == Serial_CommInterface)
		strInterface.Format("RS232:%s,%d,%d,%d,%s\r\n",			
							GetComInfo()->mComName,
							GetComInfo()->mBaund,
							GetComInfo()->mDataBits,
							GetComInfo()->mStopBits,
							CUdfGlobalFUNC::GetParityString(GetComInfo()->mParity));

	strResult.Format("线程名称:%s\r\n线程ID:%d\r\n接口:\r\n    %s",GetThreadName(),GetThreadID(),strInterface);
	return strResult;
}


const CString CCommThreadInfo::GetProtocolTypeString()
{
	CString strTemp ="";
	if (m_ProtocolType == UDP_Protocol)
		strTemp = "UDP";
	else if (m_ProtocolType == TCP_Protocol)
		strTemp = "TCP";

	return strTemp;
}


const CString CCommThreadInfo::FrameBufferLog(BYTE* buf,int nLen,BYTE ArrowFlag, BYTE CommInterface /* = Serial_CommInterface */, PNET_INFO pNetInfo /* = NULL */)
{
	CString strTime = "";
	SYSTEMTIME st;
	GetLocalTime(&st);
	strTime.Format("%04d-%02d-%02d %02d:%02d:%02d.%03d",
		st.wYear,
		st.wMonth,
		st.wDay,
		st.wHour,
		st.wMinute,
		st.wSecond,
		st.wMilliseconds);
	//TRACE("时间=%s,",strTime);

	CString strNetInfo="";
	if (CommInterface == Ethernet_CommInerface &&
		pNetInfo != NULL)
	{
		if (ArrowFlag == DOWN_ARROW)
			strNetInfo.Format("UDP,DST_IP=(%s,%d),",pNetInfo->mIpAddress,pNetInfo->mPort);
		else if (ArrowFlag == UP_ARROW)
			strNetInfo.Format("UDP,SRC_IP=(%s,%d),",pNetInfo->mIpAddress,pNetInfo->mPort);


		//TRACE("%s",strNetInfo);
	}
	else if (CommInterface == Serial_CommInterface)
	{
		strNetInfo.Format("RS232=(%s,%d,%d,%d,%s),",
			GetComInfo()->mComName,
			GetComInfo()->mBaund,
			GetComInfo()->mDataBits,
			GetComInfo()->mStopBits,
			CUdfGlobalFUNC::GetParityString(GetComInfo()->mParity));
		//TRACE("%s",strNetInfo);
	}

	CString strArrow = "";
	if (ArrowFlag == DOWN_ARROW)
		strArrow = "发 ,";
	else if (ArrowFlag == UP_ARROW)
		strArrow = "收 ,";
	//TRACE("%s",strArrow);


	CString strBuf="",strTemp = "";
	strTemp.Format("长度=%-3d,报文=",nLen);
	strBuf += strTemp;
	//TRACE("%s",strTemp);

	for (int idx=0;idx <nLen;idx ++)
	{
		strTemp.Format("%02X ",buf[idx]);
		strBuf += strTemp;
		//TRACE("%s",strTemp);
	}
	//TRACE("\r\n");

	CString strResult = "";
	strResult.Format("%s%s%s",strArrow,strNetInfo,strBuf);

	return strResult;
}

void CCommThreadInfo::InsertThreadLog(CString strThreadLog,_LOG_MESSAGE_TYPE messageType, _Log_level_type lLevelType /* = Normal_Log_Message */, PSYSTEMTIME pST/* =NULL */)
{
	if (m_pCommServer)
	{
		m_pCommServer->InsertThreadLog(strThreadLog,messageType,lLevelType,pST);
	}
}


void CCommThreadInfo::CheckReSendEvent()
{
	//if (m_pSysDataTrans == NULL) return;

	//检查每一个需要重发的对象信息
	//m_pSysDataTrans->CheckResendObject();

}


//CCommServer 类的函数实现
CCommServer::CCommServer(void)
{
	m_blCR_thread_Control = false;
	m_blCR_thread_State = false;
}

CCommServer::~CCommServer(void)
{
	//SaveThreadsCfg();
	Clear();

	SetCR_ThreadControl(false);
	while(GetCR_ThreadState() == true)
		Sleep(0);
}

void CCommServer::Clear()
{
	while (!m_lstThreads.IsEmpty())
	{
		CCommThreadInfo* pObj = m_lstThreads.RemoveHead();
		if (pObj)
		{
			
			StopCommThread(pObj);																	 //停止线程控制				

			//等待线程停止
			while (pObj->GetThreadState() != THREAD_STATE_STOP && 
				   pObj->GetThreadHandle()!= NULL)	 
				Sleep(0);

			delete pObj;
			pObj = NULL;
		}
	}
}

unsigned long CCommServer::CommThreadFunction(void* pPara)
{
	CCommThreadInfo* pModule = (CCommThreadInfo*)pPara;
	if (!pModule)   return -1;

	CString strThreadLog = "";					//日志信息
	CHandle_COM* pHandleCOM = NULL;				//串口接口对象    负责报文接收/发送
	CHandle_NET* pHandleNET = NULL;				//以太网接口对象  负责报文接收/发送  处理UDP协议
	CHandle_TcpServer* pHandleTcpServer = NULL; //以太网接口对象  负责接受客户端的连接  处理TCP协议
	CScModuleBase*     pScModule = NULL;		//卫星通讯接口

	BYTE bCommType         = pModule->GetCommType();
	BYTE bComInterfaceType = pModule->GetCommInterfaceType();
	BYTE bProtocolType	   = pModule->GetProtocolType();
	BYTE bAppProtoType     = pModule->GetAppProtoType();
	if (bComInterfaceType == Ethernet_CommInerface )		//NET
	{
		if (pModule->GetProtocolType() == UDP_Protocol)     //UDP 通信方式
		{
			pHandleNET = new CHandle_NET();
			PNET_INFO pNetInfo = pModule->GetNetInfo();
			if (!pNetInfo) 
			{
				pModule->ThreadReset();
				strThreadLog = "创建线程失败,原因:获取网络的基本信息失败!";
				pModule->InsertThreadLog(strThreadLog,SYS_RUNNING_S_LOG,Error_Log_message);
				TRACE("%s\r\n",strThreadLog);
				if (pHandleNET) delete pHandleNET;
				return -1;
			}

			if (!pHandleNET->initNET(pNetInfo->mIpAddress,
				pNetInfo->mPort))
			{
				pModule->ThreadReset();
				strThreadLog.Format("创建线程失败,原因:初始化Socket失败，IP/Port(%s:%d)",pNetInfo->mIpAddress,pNetInfo->mPort);
				pModule->InsertThreadLog(strThreadLog,SYS_RUNNING_S_LOG,Error_Log_message);
				TRACE("%s\r\n",strThreadLog);
				if (pHandleNET) delete pHandleNET;
				return -1;
			}
		}
		else if (pModule->GetProtocolType() == TCP_Protocol)  //TCP通信方式
		{
			pHandleTcpServer = new CHandle_TcpServer();
			PNET_INFO pNetInfo = pModule->GetNetInfo();
			if (!pNetInfo)
			{
				pModule->ThreadReset();
				strThreadLog="创建线程失败,原因:获取网络的基本信息失败!";
				pModule->InsertThreadLog(strThreadLog,SYS_RUNNING_S_LOG,Error_Log_message);
				if (pHandleTcpServer) 
				{
					delete pHandleTcpServer;
					pHandleTcpServer = NULL;
				}
				return -1;
			}

			pHandleTcpServer->SetTcpServerNetInfo(pNetInfo);
			pHandleTcpServer->SetCmType(pModule->GetCommType());
			if (pHandleTcpServer->InitTcpServer() == false)
			{
				pModule->ThreadReset();
				strThreadLog.Format("创建线程失败,原因:初始化listen失败，IP/port(%s:%d)",pNetInfo->mIpAddress,pNetInfo->mPort);
				pModule->InsertThreadLog(strThreadLog,SYS_RUNNING_S_LOG,Error_Log_message);
				if (pHandleTcpServer)
				{
					delete pHandleTcpServer;
					pHandleTcpServer = NULL;
				}

				return -1;
			}
		}
	}
	else if (bComInterfaceType == Serial_CommInterface)	//COM
	{
		pHandleCOM = new CHandle_COM();
		PCOM_INFO pComInfo = pModule->GetComInfo();
        if (!pComInfo) 
		{
			pModule->ThreadReset();
			strThreadLog = "创建线程失败,原因:获取串口(RS232)的基本信息失败!";
			pModule->InsertThreadLog(strThreadLog,SYS_RUNNING_S_LOG,Error_Log_message);
			TRACE("%s\r\n",strThreadLog);
			if (pHandleCOM) delete pHandleCOM;
			return -1;
		}

		if (!pHandleCOM->initCOM(pComInfo->mComName,
			                     pComInfo->mBaund,
								 pComInfo->mDataBits,
								 pComInfo->mStopBits,
								 pComInfo->mParity))
		{

			pModule->ThreadReset();
			strThreadLog.Format("创建线程失败,原因:初始化串口失败,串口信息(%s,%d,%d,%d,%s)",
				pComInfo->mComName,
				pComInfo->mBaund,
				pComInfo->mDataBits,
				pComInfo->mStopBits,
				CUdfGlobalFUNC::GetParityString(pComInfo->mParity));
			pModule->InsertThreadLog(strThreadLog,SYS_RUNNING_S_LOG,Error_Log_message);
			TRACE("%s\r\n",strThreadLog);
			if (pHandleCOM) delete pHandleCOM;
			return -1;
		}

		if (bCommType == SC_CommChannel)
		{
			if (pModule->GetScCommInfo()->nFactoryType == SCFactoryType_GTDZ)
			{
				pScModule = new CScModule_GTDZ();
				pScModule->SetCommThreadInfo(pModule);
				pScModule->SetScCommInfo(pModule->GetScCommInfo());
			}
			else if (pModule->GetScCommInfo()->nFactoryType == SCFactoryType_SZTH)
			{
				pScModule = new CScModule_SZTH();
				pScModule->SetCommThreadInfo(pModule);
				pScModule->SetScCommInfo(pModule->GetScCommInfo());
			}

		}
	}

	pModule->SetHandle_Com(pHandleCOM);
	pModule->SetHandle_Net(pHandleNET);
	pModule->SetHandle_TcpServer(pHandleTcpServer);
	pModule->SetScModule(pScModule);
	pModule->SetThreadState(THREAD_STATE_RUNNING);
	while (pModule->GetThreadControl() != false)
	{
		//接收报文，并处理报文的信息
		if (bComInterfaceType == Ethernet_CommInerface &&
			bCommType != SC_CommChannel)										//以太网 
		{
			if (UDP_Protocol == bProtocolType)									//UDP 通信方式
			{
				long len = pHandleNET->lookNETBuf();
				if (len > 0)
				{
					unsigned char buffer[READPORT_LEN_PER] ={0};
					memset(buffer,0,sizeof(buffer));
					CString strIpFrom	= "";									//源IP
					USHORT	usPortFrom	= 0;									//源端口
					NET_INFO from_netInfo ;

					len = pHandleNET->recvNET(buffer,len,strIpFrom,usPortFrom);

					from_netInfo.mIpAddress = strIpFrom;
					from_netInfo.mPort		= usPortFrom;
					if (len > 0)
					{
						if(pHandleNET->CheckFrameBuffer(buffer,len) == TRUE)
							pModule->FrameBufDeal(buffer,len,Ethernet_CommInerface,strIpFrom,usPortFrom);

						strThreadLog=pModule->FrameBufferLog(buffer,len,UP_ARROW,Ethernet_CommInerface,&from_netInfo);
						pModule->InsertThreadLog(strThreadLog,RTU_CM_BUFFER_LOG);
					}
				}
			}
			else if ( TCP_Protocol == bProtocolType)							//TCP通信方式
			{
				pHandleTcpServer->SetAppProtoType(bAppProtoType);
				pHandleTcpServer->DealConnectionEvent();
			}
		}
		else if (bComInterfaceType == Serial_CommInterface   
			     && bCommType != SC_CommChannel)						        //串口非卫星通信
		{
			long len = pModule->GetHandle_Com()->lookCOMBuf();
			len = len<READPORT_LEN_PER ?len : READPORT_LEN_PER;
			if(len>0)
			{
				unsigned char buffer[READPORT_LEN_PER] = {0};
				memset(buffer,0,READPORT_LEN_PER);
				len = pModule->GetHandle_Com()->recvCOM(buffer,len);
				if(len>0)
				{
					pModule->GetHandle_Com()->recvDATA(buffer,len);
					
					if (len>0 && pModule->GetHandle_Com()->enframe_link(WaterResources_Protocol) == TRUE)   //得到报文帧数据
					{
						BYTE _FRAMEBUF[RECV_BUFFER_LEN];
						memset(_FRAMEBUF,0,sizeof(_FRAMEBUF));
						int _FLen = pModule->GetHandle_Com()->getFrameBuffer(_FRAMEBUF);
						if (_FLen >=5)
						{
							pModule->FrameBufDeal(_FRAMEBUF,_FLen);
							strThreadLog = pModule->FrameBufferLog(_FRAMEBUF,_FLen,UP_ARROW);
							pModule->InsertThreadLog(strThreadLog,RTU_CM_BUFFER_LOG);
						}
					}
					else if (len>0 && pModule->GetHandle_Com()->enframe_link(SCSW_008_Protocol) == TRUE)
					{
						BYTE _FRAMEBUF[RECV_BUFFER_LEN];
						memset(_FRAMEBUF,0,sizeof(_FRAMEBUF));
						int _FLen = pModule->GetHandle_Com()->getFrameBuffer(_FRAMEBUF);
						if (_FLen >=5)
						{
							pModule->FrameBufDeal_SCSW008(_FRAMEBUF,_FLen, pModule->GetHandle_Com()->m_DataOrWriteFlag);
							strThreadLog = pModule->FrameBufferLog(_FRAMEBUF,_FLen,UP_ARROW);
							pModule->InsertThreadLog(strThreadLog,RTU_CM_BUFFER_LOG);
						}
					}//
					else if (len>0 && pModule->GetHandle_Com()->enframe_link(SYQYC_Comm_Protocol) == TRUE)
					{
						BYTE _FRAMEBUF[RECV_BUFFER_LEN];
						memset(_FRAMEBUF,0,sizeof(_FRAMEBUF));
						int _FLen = pModule->GetHandle_Com()->getFrameBuffer(_FRAMEBUF);
						if (_FLen >=5)
						{
							pModule->FrameBufDeal_SYQ(_FRAMEBUF,_FLen, pModule->GetHandle_Com()->m_DataOrWriteFlag);
							strThreadLog = pModule->FrameBufferLog(_FRAMEBUF,_FLen,UP_ARROW);
							pModule->InsertThreadLog(strThreadLog,RTU_CM_BUFFER_LOG);
						}
					}
				}
			}

			if (pModule->GetHandle_Com()->enframe_link(WaterResources_Protocol) == TRUE)   //得到报文帧数据
			{
				BYTE _FRAMEBUF[RECV_BUFFER_LEN];
				memset(_FRAMEBUF,0,sizeof(_FRAMEBUF));
				int _FLen = pModule->GetHandle_Com()->getFrameBuffer(_FRAMEBUF);
				if (_FLen >=5)
				{
					pModule->FrameBufDeal(_FRAMEBUF,_FLen);
					strThreadLog = pModule->FrameBufferLog(_FRAMEBUF,_FLen,UP_ARROW);
					pModule->InsertThreadLog(strThreadLog,RTU_CM_BUFFER_LOG);
				}
			}
		}
		else if (bComInterfaceType == Serial_CommInterface &&
			     bCommType == SC_CommChannel)                         //串口卫星通信
		{
			long len = pModule->GetHandle_Com()->lookCOMBuf();
			len = len<READPORT_LEN_PER ?len : READPORT_LEN_PER;
			if(len>0)
			{
				unsigned char buffer[READPORT_LEN_PER] = {0};
				memset(buffer,0,READPORT_LEN_PER);
				len = pModule->GetHandle_Com()->recvCOM(buffer,len);
				if(len>0)
				{
					//将数据报文传输给卫星处理模块
					if (pScModule != NULL)
					{
						pScModule->SetAppProtoType(bAppProtoType);
						pScModule->RecvMessage(buffer,len);
					}
				}
			}

			if (pScModule != NULL )  //Send Message
			{
				BYTE Sndbuf[1024];
				int  SndLen = 0;
				memset(Sndbuf,0,sizeof(Sndbuf));

				if (pScModule->SendMessage(Sndbuf,SndLen) == true)
				{
					if (SndLen > 0 )
						pModule->GetHandle_Com()->sendCOM(Sndbuf,SndLen);
				}
			}
				
		}

		Sleep(1);
	}

	pModule->ThreadReset();
	if (pHandleCOM)
	{
		delete pHandleCOM;
		pHandleCOM = NULL;
	}

	if (pHandleNET)
	{
		delete pHandleNET;
		pHandleNET = NULL;
	}

	if (pHandleTcpServer )
	{
		delete pHandleTcpServer;
		pHandleTcpServer = NULL;
	}
	if (pScModule)
	{
		delete pScModule;
		pScModule = NULL;
	}

	return 0;
}

unsigned long CCommServer::CheckResendFunction(void* pPara)
{
	CCommServer* pServer = (CCommServer*)pPara;
	if (!pServer) return -1;

	pServer->SetCR_ThreadState(true);
	while (pServer->GetCR_ThreadControl() == true)
	{
		CRealTimeDB* pRealDB = GETAPP()->m_pReadDB;
		if (pRealDB)
		{
			int nCount = pRealDB->GetRtuStationCount();
			for (int idx = 0; idx < nCount; idx ++)
			{
				CRtuStation* pSub = pRealDB->GetRtuStation(idx);
				if (pSub && (pSub->GetCommState() == Connected_State|| pSub->GetCommState() == CommUnknown_State))
				{
					if(pSub->GetAppProtocolType()== WaterResources_Protocol) //zwb ???????????
						pSub->GetSysDataTrans()->CheckResendObject();
					else if(pSub->GetAppProtocolType()== SYQYC_Comm_Protocol)
						pSub->GetSysDataTrans_Syq()->CheckResendObject();
				}

				//自动对遥测终端机进行时间设置,每天自动设置时间一次
				if (pSub != NULL)
				{
					SYSTEMTIME curtime;
					GetLocalTime(&curtime);
					if (curtime.wHour == 0 && curtime.wMinute == 0)
					{
						if (pSub->IsSetTimePerDay() == true)
							pSub->SetTimePerDay(false);
						if (pSub->IsSetTimeEnable() == true)
							pSub->SetTimeEnable(false);
					}
					else
					{ 
						if ( (pSub->GetCommState() == Connected_State || pSub->GetCommState() == CommUnknown_State) && 
							pSub->GetCmType() != SC_CommChannel  && 
							pSub->IsSetTimePerDay() == false &&
							pSub->IsSetTimeEnable() == true)
						{
							CPacketFrameFormat pack;
							pSub->BuildBasicPacket(pack);
							Sleep(100);
							pSub->GetSysDataTrans()->IEC_waterdata_EN_11H(&pack);
							pSub->SetTimePerDay(true);
							Sleep(100);
							pSub->SetTimeEnable(false);
						}
					}
				}
				
			}
		}

		Sleep(1);
	}
	pServer->SetCR_ThreadState(false);

	return 0;
}


//************************************
// Method:    CreateCommThread
// FullName:  CCommServer::CreateCommThread
// Access:    public 
// Returns:   bool
// Qualifier: 创建启动线程
// Parameter: CCommThreadInfo * pThreadInfo
//************************************
bool CCommServer::CreateCommThread(CCommThreadInfo* pThreadInfo)
{
	if (!pThreadInfo)  return false;

	//线程本身是运行状态的
	if (pThreadInfo->GetThreadHandle() != NULL && 
		pThreadInfo->GetThreadID()	   != 0 && 
		pThreadInfo->GetThreadState() == THREAD_STATE_RUNNING)
		return true;


	//线程本身是挂起状态
	if (pThreadInfo->GetThreadHandle() != NULL &&
		pThreadInfo->GetThreadID()	  != 0 && 
		pThreadInfo->GetThreadState() == THREAD_STATE_PAUSE)
		return ResumeCommThread(pThreadInfo);

	//创建启动线程
	pThreadInfo->SetThreadControl(true);
	DWORD thdID = 0;
	HANDLE thdHandle = ::CreateThread(NULL,0,CommThreadFunction,pThreadInfo,0,&thdID);
	if (thdHandle == NULL)
		return false;

	pThreadInfo->SetThreadHandle(thdHandle);
	//DWORD thdID = ::GetThreadId(thdHandle);      // < Windows 2003 不能使用
	pThreadInfo->SetThreadID(thdID);

   return true;
}


bool CCommServer::Initialization()
{
	StartResendThread();
	LoadThreadsCfg();
	BuildRelaCommToRtu();
	return  true;
}

void CCommServer::StartResendThread()
{
	SetCR_ThreadControl(true);
	SetCR_ThreadState(false);
	DWORD thdID = 0;
	HANDLE thdHandle = ::CreateThread(NULL,0,CheckResendFunction,this,0,&thdID);
	if (thdHandle == NULL)
	{
		TRACE("创建重发处理线程失败!\n");
	}
}

bool CCommServer::AddCommThread(CCommThreadInfo* pThreadInfo)
{
	if (!pThreadInfo) return false;

	
	pThreadInfo->SetCommServer(this);

	if (!CreateCommThread(pThreadInfo))
		return false;

	CWaitTimer _waittimer;
	_waittimer.SetWaitTimer(3000);   //等待3s

	while(pThreadInfo->GetThreadState() != THREAD_STATE_RUNNING)
	{
		if (_waittimer.WaitTimerState(100))
		{
			break;
		}
		Sleep(0);
	}

	POSITION rPos = m_lstThreads.AddTail(pThreadInfo);
	
	if (rPos != NULL)
		return true;

	return false;
}


bool CCommServer::SuspendCommThread(CCommThreadInfo* pThreadInfo)
{
	if (!pThreadInfo) return false;

	if (pThreadInfo->GetThreadHandle() == NULL ||
		pThreadInfo->GetThreadID()     == 0    ||
		pThreadInfo->GetThreadState()  != THREAD_STATE_RUNNING) 
		return true;

	DWORD ret = ::SuspendThread(pThreadInfo->GetThreadHandle());
	if (ret == 0xFFFFFFFF)
		return false;
	else 
	{
		pThreadInfo->SetThreadState(THREAD_STATE_PAUSE);
		return true;
	}

	return false;
}

bool CCommServer::ResumeCommThread(CCommThreadInfo* pThreadInfo)
{
	if (!pThreadInfo) return false;
	if (pThreadInfo->GetThreadHandle() != NULL && 
		pThreadInfo->GetThreadID() != 0 && 
		pThreadInfo->GetThreadState() == THREAD_STATE_RUNNING)
		return true;

	if (pThreadInfo->GetThreadHandle() != NULL &&
		pThreadInfo->GetThreadID() != 0 && 
		pThreadInfo->GetThreadState() == THREAD_STATE_PAUSE)
	{
		DWORD ret = ::ResumeThread(pThreadInfo->GetThreadHandle());
		if (ret == 0xFFFFFFFF)
			return false;
		else 
		{
			pThreadInfo->SetThreadState(THREAD_STATE_RUNNING);
			return true;
		}
	}

	return false;
}

bool CCommServer::StopCommThread(CCommThreadInfo* pThreadInfo)
{
	if (!pThreadInfo) return false;

	if (pThreadInfo->GetThreadHandle() == NULL ||
		pThreadInfo->GetThreadID()     == 0    ||
		pThreadInfo->GetThreadState()  != THREAD_STATE_RUNNING) 
		return true;

	CWaitTimer _waittimer;
	_waittimer.SetWaitTimer(3000);					//等待3s
	pThreadInfo->SetThreadControl(false);			//设置“停止”线程标志

	while(pThreadInfo->GetThreadState()== THREAD_STATE_RUNNING)
	{
		if (_waittimer.WaitTimerState(200))
		{
			break;
		}
		Sleep(0);
	}

	if (pThreadInfo->GetThreadState() == THREAD_STATE_STOP)
	{
		pThreadInfo->SetThreadHandle(NULL);
		pThreadInfo->SetThreadID(0);
		return true;
	}

	return false;
}


bool CCommServer::StartCommThread(CCommThreadInfo* pThreadInfo)
{
	if (!pThreadInfo)				return false;
	if (pThreadInfo->GetThreadHandle() != NULL && 
		pThreadInfo->GetThreadID() != 0 && 
		pThreadInfo->GetThreadState() == THREAD_STATE_RUNNING)
		return true;

	if (pThreadInfo->GetThreadHandle() != NULL &&
		pThreadInfo->GetThreadID() != 0 && 
		pThreadInfo->GetThreadState() == THREAD_STATE_PAUSE)
		return ResumeCommThread(pThreadInfo);

	bool ret = CreateCommThread(pThreadInfo);
	if (ret == true)
	{
		CWaitTimer _waittimer;
		_waittimer.SetWaitTimer(3000);   //等待3s

		while(pThreadInfo->GetThreadState() != THREAD_STATE_RUNNING)
		{
			if (_waittimer.WaitTimerState(200))
			{
				break;
			}
			Sleep(0);
		}

		if (pThreadInfo->GetThreadState() == THREAD_STATE_RUNNING)
		{
			return true;
		}
	}

	return false;
}


bool CCommServer::DeleteCommThread(CCommThreadInfo* pThreadInfo)
{
	if (!pThreadInfo)		return false;
	if (!StopCommThread(pThreadInfo)) return false;         //停止线程

	//从链表中删除此线程对应的对象信息
	POSITION rPos = m_lstThreads.GetHeadPosition(),FindPos = NULL;
	bool blFinded = false;
	while (rPos != NULL)
	{
		CCommThreadInfo* pObj = m_lstThreads.GetAt(rPos);
		if (pObj  && pObj == pThreadInfo)
		{
			blFinded = true;
			FindPos = rPos;
			break;
		}
		m_lstThreads.GetNext(rPos);
	}
	if (blFinded == true && rPos != NULL)
	{
		m_lstThreads.RemoveAt(FindPos);

		delete pThreadInfo;
		pThreadInfo = NULL;
		return true;
	}

	return false;
}


void CCommServer::InsertThreadLog(CString strThreadLog,_LOG_MESSAGE_TYPE messageType,_Log_level_type lLevelType /* = Normal_Log_Message */, PSYSTEMTIME pST/* =NULL */)
{
	CCMSApp*	pApp = (CCMSApp*)AfxGetApp();
	pApp->SystemLog(messageType,strThreadLog,lLevelType);
}


bool CCommServer::BuildRelaCommToRtu()
{
	//清除CRtuStation 的CCommThreadInfo对象
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	if (pApp->m_pReadDB)
	{
		POSITION rPos = pApp->m_pReadDB->GetRtuStList()->GetHeadPosition();
		while (rPos != NULL)
		{
			CRtuStation* pStation = (CRtuStation*)pApp->m_pReadDB->GetRtuStList()->GetNext(rPos);
	
			if (pStation  && 
				pStation->GetInterface() == Serial_CommInterface && 
				pStation->GetCmType() != SC_CommChannel)
				BuildRelaCommToRtu(pStation);
		}
	}
	
	return true;
}

void CCommServer::BuildRelaCommToRtu(CRtuStation* pSub)
{
	if (pSub)
	{
		POSITION rPos  = m_lstThreads.GetHeadPosition();
		while (rPos != NULL)
		{
			CCommThreadInfo* pThread = m_lstThreads.GetNext(rPos);
			if (pThread && pThread->GetCommInterfaceType() == Serial_CommInterface &&
				pThread->GetHandle_Com() != NULL)
			{
				if (pSub  && pSub->GetInterface() == Serial_CommInterface)
				{
					PCOM_INFO pComInfo = pThread->GetComInfo();
					if (pComInfo != NULL)
					{
						if (pSub->GetComPortA()		== pComInfo->mComName && 
							pSub->GetBaundA()		== pComInfo->mBaund &&
							pSub->GetDataBitsA()	== pComInfo->mDataBits &&
							pSub->GetStopBitsA()	== pComInfo->mStopBits &&
							pSub->GetParityA()		== pComInfo->mParity)
						{
							pSub->SetHandle_Com(pThread->GetHandle_Com());
							if(pSub->SetCommState(CommUnknown_State) == true)
								pSub->GetSysDataTrans()->UpdateRtuCommState(pSub);
							pThread->GetHandle_Com()->AddRelateStation(pSub);
						}
					}
				}
			}
		}
	}
}

bool CCommServer::LoadThreadsCfg()
{
    CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	CString FileName = pApp->m_GlobalOptions.m_strCSThreadsCfgFile;
	FileName.Trim();

	if (FileName.IsEmpty())						return false;
	if (!CUdfGlobalFUNC::IsFileExist(FileName)) return false;

	try
	{
		CStdioFile file;
		if (!file.Open(FileName,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite|CFile::typeText|CFile::shareDenyWrite))
		{
			AfxMessageBox("File not be Open!",MB_OK);
			file.Close();
			return false;
		}

		file.SeekToBegin();

		CString strLine="";
		while (file.ReadString(strLine))
		{
			strLine.Trim();
			if (strLine.IsEmpty()) continue;

			if (strLine.Left(1) == "#" ||
				strLine.Left(1) == "'") 
				continue;

			CString  sthreadName = "", sIP = "",sComName="";
			int      ComType = 1,ComInterface =1,Procotol=UDP_Protocol, 
				     Port = 0,DataBits=8,Baund = 0,stopBits = 0 ,Parity = 0,
					 ScFactory = 1, ScAddressLocal = 0, nAppProtoType = 1;

			CString subString = "",TempStr="",Str="";
			int nFindPos = -1;
			Str = strLine;

			nFindPos = Str.Find("=");   // Get threadname
			if (nFindPos != -1)
			{
				subString = Str.Left(nFindPos);
				sthreadName = subString;
				TempStr = Str.Right(Str.GetLength() - (nFindPos+1));
				Str = TempStr;
			}

			nFindPos = Str.Find(","); // Get comtype
			if (nFindPos != -1)
			{
				subString = Str.Left(nFindPos);
				ComType = atoi( subString);
				TempStr = Str.Right(Str.GetLength() - (nFindPos+1));
				Str = TempStr;
			}

			nFindPos = Str.Find(","); // Get Cominterface
			if (nFindPos != -1)
			{
				subString = Str.Left(nFindPos);
				ComInterface = atoi( subString);

				TempStr = Str.Right(Str.GetLength() - (nFindPos+1));
				Str = TempStr;
			}

			nFindPos = Str.Find(","); // Get Protocol
			if (nFindPos != -1)
			{
				subString = Str.Left(nFindPos);
				Procotol = atoi( subString);

				TempStr = Str.Right(Str.GetLength() - (nFindPos+1));
				Str = TempStr;
			}

			nFindPos = Str.Find(","); // Get ip
			if (nFindPos != -1)
			{
				subString = Str.Left(nFindPos);
				sIP = subString;
				TempStr = Str.Right(Str.GetLength() - (nFindPos+1));
				Str = TempStr;
			}


			nFindPos = Str.Find(","); // Get port
			if (nFindPos != -1)
			{
				subString = Str.Left(nFindPos);
				Port = atoi(subString);
				TempStr = Str.Right(Str.GetLength() - (nFindPos+1));
				Str = TempStr;
			}
	

			nFindPos = Str.Find(","); // Get ComName
			if (nFindPos != -1)
			{
				subString = Str.Left(nFindPos);
				sComName = subString;
				TempStr = Str.Right(Str.GetLength() - (nFindPos+1));
				Str = TempStr;
			}


			nFindPos = Str.Find(","); // Get baund
			if (nFindPos != -1)
			{
				subString = Str.Left(nFindPos);
				Baund = atoi(subString);
				TempStr = Str.Right(Str.GetLength() - (nFindPos+1));
				Str = TempStr;
			}

			nFindPos = Str.Find(","); // Get databits
			if (nFindPos != -1)
			{
				subString = Str.Left(nFindPos);
				DataBits = atoi(subString);
				TempStr = Str.Right(Str.GetLength() - (nFindPos+1));
				Str = TempStr;
			}


			nFindPos = Str.Find(","); // Get stopbits
			if (nFindPos != -1)
			{
				subString = Str.Left(nFindPos);
				stopBits = atoi(subString);
				TempStr = Str.Right(Str.GetLength() - (nFindPos+1));
				Str = TempStr;
			}

			nFindPos = Str.Find(","); // Get parity
			if (nFindPos != -1)
			{
				subString = Str.Left(nFindPos);
				Parity = atoi(subString);
				TempStr = Str.Right(Str.GetLength() - (nFindPos+1));
				Str = TempStr;

                nFindPos = Str.Find(",");
				if (nFindPos != -1)
				{
					subString = Str.Left(nFindPos);
					ScFactory = atoi(subString);
					TempStr = Str.Right(Str.GetLength() - (nFindPos+1));
					Str = TempStr;
                     
					nFindPos = Str.Find(",");
					if (nFindPos != -1)
					{
						subString = Str.Left(nFindPos);
						ScAddressLocal = atoi(subString);
						TempStr = Str.Right(Str.GetLength() - (nFindPos+1));
						Str = TempStr;
					}
					else
						ScAddressLocal =atoi(Str);
				}
				else
					ScFactory = atoi(Str);
			}
			else
				Parity = atoi(Str); 


			nAppProtoType = atoi( Str);
			
			//对得到的数据进行正确性检查
			sthreadName.Trim();
			if (nAppProtoType <=0 || nAppProtoType >10)
			{
				nAppProtoType = 1;
			}

			if (ComType <=0 || ComType > 6) 
				ComType = 1;

			if (ComInterface != Ethernet_CommInerface &&  
				ComInterface != Serial_CommInterface)
				ComInterface = Ethernet_CommInerface;

	
			sIP.Trim();
			if (sIP.IsEmpty()) 
				sIP = "0.0.0.0";

			if (Port < 0)      
				Port = 0;

			sComName.Trim();
			if (Baund < 0) 
				Baund = 0;

			if (Procotol != UDP_Protocol &&
				Procotol != TCP_Protocol)
				Procotol = UDP_Protocol;

			if (DataBits != 8 && DataBits != 7)        
				DataBits = 8 ;

			if (stopBits != ONESTOPBIT && 
				stopBits != ONE5STOPBITS && 
				stopBits != TWOSTOPBITS)			   
				stopBits = ONE5STOPBITS;

			if (Parity != NOPARITY		&&
				Parity != EVENPARITY	&&
				Parity != ODDPARITY)
				Parity = NOPARITY;

			CCommThreadInfo* pNewThread = new CCommThreadInfo();
			if (pNewThread)
			{
				pNewThread->SetThreadName(sthreadName);
				pNewThread->SetCommType((BYTE)ComType);
				pNewThread->SetCommInferfaceType((BYTE)ComInterface);
				pNewThread->SetProtocolType(Procotol);
				pNewThread->SetAppProtoType((BYTE)nAppProtoType);
				
				NET_INFO net_info;
				net_info.mIpAddress = sIP;
				net_info.mPort = Port;
				pNewThread->SetNetInfo(&net_info);

				COM_INFO com_info;
				com_info.mComName	= sComName;
				com_info.mBaund		= Baund;
				com_info.mDataBits	= DataBits;
				com_info.mStopBits  = stopBits;
				com_info.mParity	= Parity;
				pNewThread->SetComInfo(&com_info);

				SC_COMM_INFO sc_comm_info;
				sc_comm_info.nScAddress = ScAddressLocal;
				sc_comm_info.nFactoryType = ScFactory;
				pNewThread->SetScCommInfo(&sc_comm_info);

				AddCommThread(pNewThread);
			}
		}
		file.Close();
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
		return false;
	}
	catch (CFileException* e)
	{
		e->ReportError();
		e->Delete();
		return false;
	}

	return true;
}


bool CCommServer::SaveThreadsCfg()
{
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	CString FileName = pApp->m_GlobalOptions.m_strCSThreadsCfgFile;
	FileName.Trim();

	if (FileName.IsEmpty()) return false;

	try
	{
		CStdioFile _fp;
		_fp.Open(FileName,CFile::modeCreate|CFile::modeReadWrite|CFile::typeText|CFile::shareDenyWrite);
		_fp.SeekToBegin();

		CString strLine = "#THRADID=CMTYPE,INTERFACETYPE,PROTOCOL,IP,PORT,COMNAME,BAUND,DATABITS,STOPBITS,PARITY,SCFACTORY,SCADDR,APPPROTYPE\n";
		_fp.WriteString(strLine);

		POSITION rPos = m_lstThreads.GetHeadPosition();
		while (rPos != NULL)
		{
			CCommThreadInfo* pThread = m_lstThreads.GetNext(rPos);
			if (pThread)
			{
				strLine.Format("%s=%d,%d,%d,%s,%d,%s,%d,%d,%d,%d,%d,%d,%d\n",
					pThread->GetThreadName(),
					pThread->GetCommType(),
					pThread->GetCommInterfaceType(),
					pThread->GetProtocolType(),
					pThread->GetNetInfo()->mIpAddress,
					pThread->GetNetInfo()->mPort,
					pThread->GetComInfo()->mComName,
					pThread->GetComInfo()->mBaund,
					pThread->GetComInfo()->mDataBits,
					pThread->GetComInfo()->mStopBits,
					pThread->GetComInfo()->mParity,
					pThread->GetScCommInfo()->nFactoryType,
					pThread->GetScCommInfo()->nScAddress,
					pThread->GetAppProtoType());
				_fp.WriteString(strLine);
			}
		}
		_fp.Flush();
		_fp.Close();
	}
	catch (CFileException* e)
	{
		e->ReportError();
		e->Delete();
		return false;
	}

	return true;
}