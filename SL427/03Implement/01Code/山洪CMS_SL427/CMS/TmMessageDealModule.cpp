#include "StdAfx.h"
#include "CMS.h"
#include "TmMessageDealModule.h"
#include "ScModule_GTDZ.h"

CTmMessageDealModule::CTmMessageDealModule(void)
{
	m_pTmModule = NULL;
	m_phoneNum = "";
	Init();
}

CTmMessageDealModule::CTmMessageDealModule( CString rAddr,CScModuleBase* pTmModule )
{
	m_phoneNum = rAddr;
	m_pTmModule = pTmModule;
	Init();
}
void CTmMessageDealModule::Init()
{
	memset(m_RECV_BUF,0,sizeof(m_RECV_BUF));
	m_RECV_Len  = 0;
	m_MaxLen = 4096;
	m_pSubStation = NULL;
}


CTmMessageDealModule::~CTmMessageDealModule(void)
{
	if (m_pSubStation != NULL)
		m_pSubStation->SetTmMessageDealModule(NULL);
}

void CTmMessageDealModule::SetTmModule( CScModuleBase* pTmModule )
{
	m_pTmModule = pTmModule;
}

CScModuleBase* CTmMessageDealModule::GetTmModule()
{
	return m_pTmModule;
}

const CString CTmMessageDealModule::GetRemoteAddress()
{
	return m_phoneNum;
}

void CTmMessageDealModule::SetRemoteAddress( CString rAddr )
{
	m_phoneNum = rAddr;
}

void CTmMessageDealModule::RecvMessage( BYTE* buf,int Len )
{
	if (buf && Len > 0)
	{
		if (Len + m_RECV_Len > m_MaxLen)
		{

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

void CTmMessageDealModule::DealMessage()
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

void CTmMessageDealModule::AnalyzePacket( unsigned char* FRAME_BUF, unsigned int FRAME_Len )
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

			if (GetTmModule() != NULL && GetTmModule()->GetCommThreadInfo() != NULL)
			{
				CCommThreadInfo* pCurThread = GetTmModule()->GetCommThreadInfo();
				waterFrame.m_CommInterface	= pCurThread->GetCommInterfaceType();	
				if (pCurThread->GetComInfo() != NULL)
				{
					waterFrame.m_ComInfo = (*(pCurThread->GetComInfo()));
				}

				if (pCurThread->GetNetInfo() != NULL)
				{
					waterFrame.m_NetInfo = (*(pCurThread->GetNetInfo()));
				}

				if (pCurThread->GetScCommInfo() != NULL)
				{
					waterFrame.m_ScCommInfo = (*(pCurThread->GetScCommInfo()));
				}

				if (pCurThread->GetTmCommInfo() != NULL)
				{
					waterFrame.m_TmCommInfo = (*(pCurThread->GetTmCommInfo()));
				}
			}

			//检测OldScMessage处理对象
			CTmMessageDealModule* pOldTmMessageModule = pSub->GetTmMessageDealModule();
			if (this  != pOldTmMessageModule)
			{
				pSub->SetTmMessageDealModule(this);
				if (pOldTmMessageModule != NULL)
				{
					if (GetTmModule() != NULL)
						GetTmModule()->RemoveTmMessageModule(pOldTmMessageModule);
				}
			}

			pSub->SetCmType(TEXTMSG_CommChannel);
			pSub->SetInterface(waterFrame.m_CommInterface);
			pSub->SetNetInfo(&(waterFrame.m_NetInfo));
			pSub->SetComInfo(&(waterFrame.m_ComInfo));
			//pSub->SetScCommInfo(&(waterFrame.m_ScCommInfo));
			pSub->SetTmCommInfo(&(waterFrame.m_TmCommInfo));
			pSub->GetSysDataTrans()->Dispatcher(&waterFrame);
		}
	}
}

bool CTmMessageDealModule::SendMessage( BYTE* buf,int Len )
{
	if (buf != NULL && Len > 0)
	{
		if (GetTmModule() != NULL )
		{
			BYTE scType = GetTmModule()->GetTmFactoryType();
			if (scType == SCTextMsgType_GHT)
			{
				TextMsg_GHT* pTmModuleGHT = NULL;
				pTmModuleGHT = (TextMsg_GHT*)GetTmModule();
				if (pTmModuleGHT)
				{
					pTmModuleGHT->SendDown_Message(buf, Len, MSG_SEND);
					return true;
				}
			}
			else if (scType == SCTextMsgType_YT)
			{
				TextMsg_YT* pScModuleYT = (TextMsg_YT*)GetTmModule();
				if (pScModuleYT)
				{
					//pScModuleGTDZ->SndDown_TXSQMessage(70,m_nRemoteAddr,0x00,buf,Len);
					return true;
				}
			}
		}
	}
	return false;
}

void CTmMessageDealModule::SetSubStation( CRtuStation* pSub )
{
	m_pSubStation = pSub;
}

CRtuStation* CTmMessageDealModule::GetSubStatin()
{
	return m_pSubStation;
}

