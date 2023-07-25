#include "StdAfx.h"
#include "CMS.h"
#include "SCMessageDealModule.h"
#include "ScModule_GTDZ.h"

CSCMessageDealModule::CSCMessageDealModule(void)
{
	m_pScModule = NULL;
	m_nRemoteAddr = 0;
	Init();
}

CSCMessageDealModule::CSCMessageDealModule( UINT rAddr,CScModuleBase* pScModule )
{
	m_nRemoteAddr = rAddr;
	m_pScModule = pScModule;
	Init();
}
void CSCMessageDealModule::Init()
{
	memset(m_RECV_BUF,0,sizeof(m_RECV_BUF));
	m_RECV_Len  = 0;
	m_MaxLen = 4096;
	m_pSubStation = NULL;
}


CSCMessageDealModule::~CSCMessageDealModule(void)
{
	if (m_pSubStation != NULL)
		m_pSubStation->SetScMessageDealModule(NULL);
}

void CSCMessageDealModule::SetScModule( CScModuleBase* pScModule )
{
	m_pScModule = pScModule;
}

CScModuleBase* CSCMessageDealModule::GetScModule()
{
	return m_pScModule;
}

const UINT CSCMessageDealModule::GetRemoteAddress()
{
	return m_nRemoteAddr;
}

void CSCMessageDealModule::SetRemoteAddress( UINT rAddr )
{
	m_nRemoteAddr = rAddr;
}

void CSCMessageDealModule::RecvMessage( BYTE* buf,int Len )
{
	if (buf && Len > 0)
	{
		if (Len + m_RECV_Len > m_MaxLen)
		{
/*			BYTE tempbuf[1024 * 10];
			memset(tempbuf,0, sizeof(tempbuf));

			int l = Len + m_RECV_Len - 1024 * 10;
			memmove(tempbuf,&m_RECV_BUF[l],m_RECV_Len -l);

			m_RECV_Len = m_RECV_Len - l;
			memset(m_RECV_BUF,0,sizeof(m_RECV_BUF));
			memmove(m_RECV_BUF,tempbuf,m_RECV_Len); */ 


			BYTE tempbuf[4096];
			memset(tempbuf,0, 4096);

			int l = Len + m_RECV_Len - m_MaxLen;
			memmove(tempbuf,&m_RECV_BUF[l], m_RECV_Len - l);

			m_RECV_Len = m_RECV_Len - l;
			memset(m_RECV_BUF,0,sizeof(m_RECV_BUF));
			memmove(m_RECV_BUF,tempbuf,m_RECV_Len);

			TRACE("m_RECV_Len= %d, Len=%d\r\n",m_RECV_Len,Len);

		}

		memmove(&m_RECV_BUF[m_RECV_Len],buf,Len);
		m_RECV_Len += Len;
	}

	DealMessage();
}

void CSCMessageDealModule::DealMessage()
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

void CSCMessageDealModule::AnalyzePacket( unsigned char* FRAME_BUF, unsigned int FRAME_Len )
{
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

			if (GetScModule() != NULL && GetScModule()->GetCommThreadInfo() != NULL)
			{
				CCommThreadInfo* pCurThread = GetScModule()->GetCommThreadInfo();
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
			if (this  != pOldScMessageModule)
			{
				pSub->SetScMessageDealModule(this);
				if (pOldScMessageModule != NULL)
				{
					if (GetScModule() != NULL)
						GetScModule()->RemoveScMessgaeModule(pOldScMessageModule);
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

bool CSCMessageDealModule::SendMessage( BYTE* buf,int Len )
{
	if (buf != NULL && Len > 0)
	{
		if (GetScModule() != NULL )
		{
			BYTE scType = GetScModule()->GetFactoryType();
			if (scType == SCFactoryType_GTDZ_4_0)
			{
				CScModule_GTDZ* pScModuleGTDZ = (CScModule_GTDZ*)GetScModule();
				if (pScModuleGTDZ)
				{
					pScModuleGTDZ->SndDown_TXSQMessage(70,m_nRemoteAddr,0x00,buf,Len);
					return true;
				}
			}
		}
	}
	return false;
}

void CSCMessageDealModule::SetSubStation( CRtuStation* pSub )
{
	m_pSubStation = pSub;
}

CRtuStation* CSCMessageDealModule::GetSubStatin()
{
	return m_pSubStation;
}

