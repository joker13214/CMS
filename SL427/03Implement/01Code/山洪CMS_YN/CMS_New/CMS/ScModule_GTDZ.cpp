#include "StdAfx.h"
#include "ScModule_GTDZ.h"

CScModule_GTDZ::CScModule_GTDZ(void)
{
	m_nSendInterval = 60 *1000;      //卫星发送报文时间间隔:1分钟
	m_CheckSendTimer.InitWaitTimer();//SetWaitTimer(m_nSendInterval);
	m_nOneSendMax = 77;
}

CScModule_GTDZ::~CScModule_GTDZ(void)
{
}

void CScModule_GTDZ::RecvMessage( BYTE* buf,int Len )
{
	AppendRecvData(buf,Len);
	DealScCommMessage();
}

bool CScModule_GTDZ::SendMessage( BYTE* buf,int &len )
{

	if (buf != NULL)
	{   
		if (m_bSendBefore == false || 
		   (m_bSendBefore == true && 
		    m_CheckSendTimer.WaitTimerState() == true))
	   {
			   //获取需要发送的报文
			   PsendObject pSndObj = GetOneSendObject();
			   if (pSndObj  != NULL)
			   {
				   memmove(buf,pSndObj->sendbuf,pSndObj->sendLen);
				   len = pSndObj->sendLen;

				   CString strLog="";
				   strLog.Format("发,卫星通讯:国腾电子,长度:%d,报文:%s",pSndObj->sendLen,
					   CUdfGlobalFUNC::ConvertBytesToString(pSndObj->sendbuf,pSndObj->sendLen));
				   AddLog(strLog);

				   m_bSendBefore = true;
				   m_CheckSendTimer.SetWaitTimer(m_nSendInterval);

				   return true;
			   } 
			   m_bSendBefore = false;
		 }
   }
   return false;
}

void CScModule_GTDZ::DealScCommMessage()
{
	if (m_RECV_Len >= 10)
	{
		int start_pos = 0;
		for (start_pos = 0 ; start_pos < m_RECV_Len; start_pos ++)
		{
			if ((char)m_RECV_BUF[start_pos] == '$')
			{
				if (start_pos + 6 <= m_RECV_Len)
				{
					//获取指令/信息
					CString strCMD = "";
					strCMD.Format("$%c%c%c%c",
						(char)m_RECV_BUF[start_pos + 1],
						(char)m_RECV_BUF[start_pos + 2],
						(char)m_RECV_BUF[start_pos + 3],
						(char)m_RECV_BUF[start_pos + 4]);

					//检查指令是否存在，不存在继续查找指令

					bool blFinded = false;
				    _SC_CMD_ITEM curCmd ;     //卫星通信上行报文类型信息（指令类型）
					for (int ni =0; ni < 8; ni ++)
					{
						if (strCMD == GTDZ_UP_CMDS[ni].Cmd_Info)
						{
							blFinded = true;
							curCmd.Cmd_Info = GTDZ_UP_CMDS[ni].Cmd_Info;
							curCmd.Cmd_Name = GTDZ_UP_CMDS[ni].Cmd_Name;
							break;
						}
					}

					if (blFinded == true)
					{
						int cmdlen = ((m_RECV_BUF[start_pos + 5] <<8)| m_RECV_BUF[start_pos + 6] );

						if (start_pos + cmdlen <= m_RECV_Len)
						{
							//获取正确报文前未解析将要抛弃的报文，存入日志中
							BYTE tempbuf[1024 * 10];
							if (start_pos != 0)
							{
								memmove(tempbuf,m_RECV_BUF,start_pos);
								CString strLog = "";
								strLog.Format("收,卫星通信:国腾电子,长度:%d,报文:%s" ,start_pos,
									CUdfGlobalFUNC::ConvertBytesToString(tempbuf,start_pos));
								AddLog(strLog);
							}

							BYTE* tempbyte = new BYTE[cmdlen];
							memmove(tempbyte,&m_RECV_BUF[start_pos],cmdlen);
                            //解析报文
							AnalyzeUpMessage(tempbyte,cmdlen,curCmd);
							delete[] tempbyte;

							//截取报文，再重新处理							
							memmove(tempbuf,&m_RECV_BUF[start_pos + cmdlen],m_RECV_Len - start_pos -cmdlen);
							m_RECV_Len = m_RECV_Len- start_pos-cmdlen;
							memset(m_RECV_BUF,0,sizeof(m_RECV_BUF));
							memmove(m_RECV_BUF,tempbuf,m_RECV_Len);
							DealScCommMessage();
						}
						else return;  //报文长度不够
					}
				}
				else return;    //报文长度不够
			}
		}
	}
}

void CScModule_GTDZ::AnalyzeUpMessage( BYTE* tempbyte, int cmdlen, _SC_CMD_ITEM curCmd )
{
	if (tempbyte != NULL && cmdlen >0)
	{
		TRACE("%s,%s\r\n",curCmd.Cmd_Name,curCmd.Cmd_Info);
		//检查校验和
		bool blCkeckSum = true;
		if (curCmd.Cmd_Info != "$GLJC")
		{
			BYTE sum = CUdfGlobalFUNC::XORSumCheck(tempbyte,cmdlen-1);
			if (sum != tempbyte[cmdlen-1])
				blCkeckSum = false;
		}
		
		CString strLog ="";
		if (blCkeckSum == true)
			strLog.Format("收,卫星通讯:国腾电子,类型:%s,指令:%s,长度:%d,报文:%s" ,
							curCmd.Cmd_Name,curCmd.Cmd_Info,cmdlen, 
							CUdfGlobalFUNC::ConvertBytesToString(tempbyte,cmdlen));
		else if (blCkeckSum == false)
			strLog.Format("收,卫星通讯:国腾电子,类型:%s,指令:%s,,和校验:错误,长度:%d,报文:%s" ,
			curCmd.Cmd_Name,curCmd.Cmd_Info,cmdlen, 
			CUdfGlobalFUNC::ConvertBytesToString(tempbyte,cmdlen));

		AddLog(strLog);
		if (blCkeckSum == true)
		{
			if (curCmd.Cmd_Info == "$GLJC")
				Deal_Up_GLJCMessage(tempbyte,cmdlen);
			else if (curCmd.Cmd_Info == "$DWXX")
				Deal_Up_DWXXMessage(tempbyte,cmdlen);
			else if (curCmd.Cmd_Info == "$TXXX")
				Deal_Up_TXXXMessage(tempbyte,cmdlen);
			else if (curCmd.Cmd_Info == "$ICXX")
				Deal_Up_ICXXMessage(tempbyte,cmdlen);
			else if (curCmd.Cmd_Info == "$ZJXX")
				Deal_Up_ZJXXMessage(tempbyte,cmdlen);
			else if (curCmd.Cmd_Info == "$SJXX")
				Deal_Up_SJXXMessage(tempbyte,cmdlen);
			else if (curCmd.Cmd_Info == "$BBXX")
				Deal_Up_BBXXMessage(tempbyte,cmdlen);
			else if (curCmd.Cmd_Info == "$FKXX")
				Deal_Up_FKXXMessage(tempbyte,cmdlen);
		}
	}
}

void CScModule_GTDZ::Deal_Up_GLJCMessage( BYTE* tempbyte, int cmdlen )
{
	if (tempbyte != NULL && cmdlen >= 11)
	{
		CString strCMD="";
		strCMD.Format("$%c%c%c%c",(char)tempbyte[1],(char)tempbyte[2],(char)tempbyte[3],(char)tempbyte[4]);
		if (strCMD != "$GLZJ") return;

		int packetLen = (tempbyte[5] <<8) | (tempbyte[6]);
		if (packetLen != cmdlen)  return;

		UINT localAddr = (tempbyte[7]<<16) | (tempbyte[8] <<8) | (tempbyte[9]);		//本地地址
		BYTE scpd = tempbyte[10];													//输出频道

	
	}
}

void CScModule_GTDZ::Deal_Up_DWXXMessage( BYTE* tempbyte, int cmdlen )
{
	if (tempbyte != NULL && cmdlen >= 30)
	{
		CString strCMD="";
		strCMD.Format("$%c%c%c%c",(char)tempbyte[1],(char)tempbyte[2],(char)tempbyte[3],(char)tempbyte[4]);
		if (strCMD != "$DWXX") return;

		int packetLen = (tempbyte[5] <<8) | (tempbyte[6]);
		if (packetLen != cmdlen)  return;

		UINT localAddr = (tempbyte[7]<<16) | (tempbyte[8] <<8) | (tempbyte[9]);		//本地地址
		


	}
}

void CScModule_GTDZ::Deal_Up_TXXXMessage( BYTE* tempbyte, int cmdlen )
{
	if (tempbyte != NULL && cmdlen >= 19)
	{					//获取指令/信息
		CString strCMD = "";
		strCMD.Format("$%c%c%c%c",(char)tempbyte[1],(char)tempbyte[2],(char)tempbyte[3],(char)tempbyte[4]);
		if (strCMD != "$TXXX") return;

		int packetLen = (tempbyte[5] <<8) | (tempbyte[6]);
		if (packetLen != cmdlen)  return;

		UINT localAddr = (tempbyte[7]<<16) | (tempbyte[8] <<8) | (tempbyte[9]);		//本地地址
		BYTE messageType = tempbyte[10];											//信息类型
		UINT remoteAddr = (tempbyte[11]<<16)| (tempbyte[12]<<8) |(tempbyte[13]);	//源地址
		BYTE Hour = tempbyte[14];
		BYTE Minute = tempbyte[15];
		UINT16 messageLen = (tempbyte[16]<<8)|(tempbyte[17]);							//$TXXX报文中电文的 bits长度;
		BYTE crcCheck = tempbyte[18 + messageLen];									//crc校验字节

		if (messageLen > 0)
		{			
			int appLen = messageLen /8;												//$TXXX报文中电文的 bytes长度;

			if (19 + appLen <= cmdlen)
			{
				BYTE* message = new BYTE[appLen];
				memmove(message,&tempbyte[18],appLen);
			
				CSCMessageDealModule* pScMessageModule = NULL;
				std::map<UINT,CSCMessageDealModule*>::iterator itrFind = m_mapSCMessageDeal.find(remoteAddr);
				if (itrFind != m_mapSCMessageDeal.end() && 
					itrFind->second != NULL)
				{
					pScMessageModule = itrFind->second;
				}
				else
				{
					pScMessageModule = new CSCMessageDealModule(remoteAddr,this);
					if (pScMessageModule)
						m_mapSCMessageDeal.insert(std::pair<UINT,CSCMessageDealModule*>(pScMessageModule->GetRemoteAddress(),pScMessageModule));			
				}

				if (pScMessageModule)
				{
					pScMessageModule->SwitchHandlerModel(m_bAppProtoType);
					pScMessageModule->RecvMessage(message,appLen);
				}
					

				delete[] message;
				message = NULL;
			}
		}
	}
}

void CScModule_GTDZ::Deal_Up_ICXXMessage( BYTE* tempbyte, int cmdlen )
{
	if (tempbyte != NULL && cmdlen >= 22 )
	{
		CString strCMD="";
		strCMD.Format("$%c%c%c%c",(char)tempbyte[1],(char)tempbyte[2],(char)tempbyte[3],(char)tempbyte[4]);
		if (strCMD != "$ICXX") return;

		int packetLen = (tempbyte[5] <<8) | (tempbyte[6]);
		if (packetLen != cmdlen)  return;

		UINT localAddr = (tempbyte[7]<<16) | (tempbyte[8] <<8) | (tempbyte[9]);		//本地地址
		BYTE zh	= tempbyte[10];														//帧号
		UINT32 tbID	= tempbyte[11] | tempbyte[12]<<8|tempbyte[13]<<16;				//通播ID
		BYTE   userFlag	= tempbyte[14];												//用户特征
		UINT16 fwpd		= tempbyte[15] |tempbyte[16] <<8;							//服务频道
		UINT8  txdj		= tempbyte[17];												//通信等级
		UINT8  jmflag	= tempbyte[18];												//加密标志
		UINT16 xsyhzs	= tempbyte[19] | tempbyte[20] <<8;							//下属用户总数

	}
}

void CScModule_GTDZ::Deal_Up_ZJXXMessage( BYTE* tempbyte, int cmdlen )
{
	if (tempbyte != NULL && cmdlen >= 21 )
	{
		CString strCMD="";
		strCMD.Format("$%c%c%c%c",(char)tempbyte[1],(char)tempbyte[2],(char)tempbyte[3],(char)tempbyte[4]);
		if (strCMD != "$ZJXX") return;

		int packetLen = (tempbyte[5] <<8) | (tempbyte[6]);
		if (packetLen != cmdlen)  return;

		UINT localAddr = (tempbyte[7]<<16) | (tempbyte[8] <<8) | (tempbyte[9]);		//本地地址
		BYTE ickstate = tempbyte[10];												//IC卡状态
		BYTE yjstate  = tempbyte[11];												//硬件状态
		BYTE dcdl	  = tempbyte[12];												//电池电量
		BYTE rzstate  = tempbyte[13];												//入站状态
		BYTE bsgl1	  = tempbyte[14];												//波束1功率
		BYTE bsgl2	  = tempbyte[15];												//波束2功率
		BYTE bsgl3	  = tempbyte[16];												//波束3功率
		BYTE bsgl4	  = tempbyte[17];												//波束4功率
		BYTE bsgl5	  = tempbyte[18];												//波束5功率
		BYTE bsgl6	  = tempbyte[19];												//波束6功率

	}
}

void CScModule_GTDZ::Deal_Up_SJXXMessage( BYTE* tempbyte, int cmdlen )
{
	if (tempbyte != NULL && cmdlen >= 21 )
	{
		CString strCMD="";
		strCMD.Format("$%c%c%c%c",(char)tempbyte[1],(char)tempbyte[2],(char)tempbyte[3],(char)tempbyte[4]);
		if (strCMD != "$SJXX") return;

		int packetLen = (tempbyte[5] <<8) | (tempbyte[6]);
		if (packetLen != cmdlen)  return;

		UINT localAddr = (tempbyte[7]<<16) | (tempbyte[8] <<8) | (tempbyte[9]);		//本地地址
		int  year = tempbyte[10]<<8|tempbyte[11];
		int  month =  tempbyte[12];
		int  day	= tempbyte[13];
		int  hour = tempbyte[14];
		int  minute = tempbyte[15];
		int  second = tempbyte[16];

	}
}

void CScModule_GTDZ::Deal_Up_BBXXMessage( BYTE* tempbyte, int cmdlen )
{
	if (tempbyte != NULL && cmdlen >= 11 )
	{
		CString strCMD="";
		strCMD.Format("$%c%c%c%c",(char)tempbyte[1],(char)tempbyte[2],(char)tempbyte[3],(char)tempbyte[4]);
		if (strCMD != "$BBXX") return;

		int packetLen = (tempbyte[5] <<8) | (tempbyte[6]);
		if (packetLen != cmdlen)  return;

		UINT localAddr = (tempbyte[7]<<16) | (tempbyte[8] <<8) | (tempbyte[9]);		//本地地址

		int bbxxLen = packetLen - 11;
		CString strbbxx = "";  //版本信息
		if (bbxxLen > 0)
		{
			char* bbxx = new char[bbxxLen + 1];
			memset(bbxx,0,bbxxLen + 1);
			memmove(bbxx,&tempbyte[10],bbxxLen);
			strbbxx.Format("%s",bbxx);
			delete[] bbxx;		
		}
	}
}

void CScModule_GTDZ::Deal_Up_FKXXMessage( BYTE* tempbyte, int cmdlen )
{
	if (tempbyte != NULL && cmdlen >= 16 )
	{
		CString strCMD="";
		strCMD.Format("$%c%c%c%c",(char)tempbyte[1],(char)tempbyte[2],(char)tempbyte[3],(char)tempbyte[4]);
		if (strCMD != "$FKXX") return;

		int packetLen = (tempbyte[5] <<8) | (tempbyte[6]);
		if (packetLen != cmdlen)  return;

		UINT localAddr = (tempbyte[7]<<16) | (tempbyte[8] <<8) | (tempbyte[9]);		//本地地址

		BYTE fkflag = tempbyte[10];													//反馈标志 0x00:成功,0x01:失败, 0x06:CRC错误


		//32bite的附加信息
		CString strFj = "";
		if (fkflag == 0x00 || 
			fkflag == 0x01 ||
			fkflag == 0x06)
		{
			strFj.Format("$%c%c%c%c",tempbyte[11],tempbyte[12],tempbyte[13],tempbyte[14]);
		}
	}
}

//
void CScModule_GTDZ::SndDown_DWSQMessage( BYTE msgType,UINT32 gcsjAndtxg,UINT32 qysj,UINT16 rzpd )
{

}

void CScModule_GTDZ::SndDown_TXSQMessage( BYTE msgType,UINT32 remoteAddr, BYTE ackType,BYTE* message,int msgLen )
{
	if (message != NULL && msgLen >0)
	{
		int nSendCount = msgLen / m_nOneSendMax + 1;
		int nLastmsgLen = msgLen % m_nOneSendMax;

		for (int sendIdx = 0 ; sendIdx < nSendCount; sendIdx ++)
		{   
			int curMsgLen = m_nOneSendMax;
			if (sendIdx == nSendCount -1)
				curMsgLen = nLastmsgLen;

			UINT16 packetlen = 5 + 2 + 3 + 1 + 3 + 2 + 1 + curMsgLen + 1;
			PsendObject pSndObj = new sendObject();

			pSndObj->sendLen = packetlen;

			char cmd[6] = "$TXSQ";
			int idx = 0;
			memmove(pSndObj->sendbuf,cmd,5);
			idx += 5;

			//len
			pSndObj->sendbuf[idx]		= packetlen / 256;
			pSndObj->sendbuf[idx +1]	= packetlen % 256;
			idx += 2;

			BYTE temp[4] ;
			memmove(temp,&m_nAddressLocal,4);

			pSndObj->sendbuf[idx]		= temp[2];
			pSndObj->sendbuf[idx + 1]	= temp[1];
			pSndObj->sendbuf[idx + 2]	= temp[0];
			idx += 3;

			pSndObj->sendbuf[idx]		= msgType ;
			idx += 1;

			memmove(temp,&remoteAddr,4);
			pSndObj->sendbuf[idx]		= temp[2];
			pSndObj->sendbuf[idx + 1]	= temp[1];
			pSndObj->sendbuf[idx + 2]	= temp[0];
			idx += 3;

			pSndObj->sendbuf[idx]		= (curMsgLen * 8) / 256;
			pSndObj->sendbuf[idx+1]		= (curMsgLen * 8) % 256;
			idx += 2;


			pSndObj->sendbuf[idx]		= ackType;
			idx += 1;

			memmove(&(pSndObj->sendbuf[idx]),&message[m_nOneSendMax * sendIdx],curMsgLen);
			idx += msgLen;

			pSndObj->sendbuf[idx]		= CUdfGlobalFUNC::XORSumCheck(pSndObj->sendbuf,idx);

			AddOneSendObject(pSndObj);
		}
	}
}

void CScModule_GTDZ::SndDown_CKSCMessage( BYTE cspl )
{

}

void CScModule_GTDZ::SndDown_ICJCMessage( BYTE zh )
{

}

void CScModule_GTDZ::SndDown_XTZJMessage( UINT16 zjpd )
{

}

void CScModule_GTDZ::SndDown_SJSCMessage( UINT16 scpd )
{

}


void CScModule_GTDZ::SndDown_BBDQMessage()
{

}


//---------------------------北京神州天鸿 北斗卫星通讯-----------------------------

CScModule_SZTH::CScModule_SZTH(void)
{
	m_nSendInterval = 60 *1000;      //卫星发送报文时间间隔:1分钟
	m_CheckSendTimer.InitWaitTimer();//SetWaitTimer(m_nSendInterval);
	m_nOneSendMax = 77;
}

CScModule_SZTH::~CScModule_SZTH(void)
{
}

void CScModule_SZTH::RecvMessage( BYTE* buf,int Len )
{
	AppendRecvData(buf,Len);
	DealScCommMessage();
}

bool CScModule_SZTH::SendMessage( BYTE* buf,int &len )
{

	if (buf != NULL)
	{   
		if (m_bSendBefore == false || 
			(m_bSendBefore == true && 
			m_CheckSendTimer.WaitTimerState() == true))
		{
			//获取需要发送的报文
			PsendObject pSndObj = GetOneSendObject();
			if (pSndObj  != NULL)
			{
				memmove(buf,pSndObj->sendbuf,pSndObj->sendLen);
				len = pSndObj->sendLen;

				CString strLog="";
				strLog.Format("发,卫星通讯:国腾电子,长度:%d,报文:%s",pSndObj->sendLen,
					CUdfGlobalFUNC::ConvertBytesToString(pSndObj->sendbuf,pSndObj->sendLen));
				AddLog(strLog);

				m_bSendBefore = true;
				m_CheckSendTimer.SetWaitTimer(m_nSendInterval);

				return true;
			} 
			m_bSendBefore = false;
		}
	}
	return false;
}

void CScModule_SZTH::DealScCommMessage()
{
	if (m_RECV_Len >= 10)
	{
		int start_pos = -1;
		int end_pos = -1;

		bool bFindHead = false;
		bool bFindTail = false;
		_SC_CMD_ITEM curCmd ;     //卫星通信上行报文类型信息（指令类型）

		for (int idx=0; idx<m_RECV_Len; idx++)
		{
			if ((char)m_RECV_BUF[idx] == '$')
			{
				if (idx+6 <= m_RECV_Len)
				{
					CString strCMD = "";
					strCMD.Format("$%c%c%c%c",
						(char)m_RECV_BUF[idx + 1],
						(char)m_RECV_BUF[idx + 2],
						(char)m_RECV_BUF[idx + 3],
						(char)m_RECV_BUF[idx + 4]);

					//检查指令是否存在，不存在继续查找指令
					
					for (int ni =0; ni < 6; ni ++)
					{
						if (strCMD == SZTH_UP_CMDS[ni].Cmd_Info)
						{
							bFindHead = true;
							curCmd.Cmd_Info = SZTH_UP_CMDS[ni].Cmd_Info;
							curCmd.Cmd_Name = SZTH_UP_CMDS[ni].Cmd_Name;
							break;
						}
					}
				}

				if (!bFindTail)
				{
					start_pos = idx;
					bFindHead = true;
				}
			}
			else if (m_RECV_BUF[idx]==0x0D && m_RECV_BUF[idx+1]==0x0A)
			{
				if (bFindHead)
				{
					end_pos = idx+1;
					bFindTail = true;
					break;
				}
			}

			
		}

		if (start_pos!=-1 && end_pos!=-1)
		{
			//获取正确报文前未解析将要抛弃的报文，存入日志中
			BYTE tempbuf[1024 * 10];
			int nMsgLen = end_pos - start_pos + 1;

			if (start_pos + nMsgLen <= m_RECV_Len)
			{
				if (start_pos != 0)
				{
					memmove(tempbuf,m_RECV_BUF,start_pos);
					CString strLog = "";
					strLog.Format("收,卫星通信:神州天鸿,长度:%d,报文:%s" ,start_pos,
						CUdfGlobalFUNC::ConvertBytesToString(tempbuf,start_pos));
					AddLog(strLog);
				}

				BYTE* tempbyte = new BYTE[nMsgLen+1];
				memset(tempbyte,0, nMsgLen+1);
				memmove(tempbyte,&m_RECV_BUF[start_pos],nMsgLen);
				//解析报文
				AnalyzeUpMessage(tempbyte,nMsgLen,curCmd);
				delete[] tempbyte;

				//截取报文，再重新处理							
				memmove(tempbuf,&m_RECV_BUF[start_pos + nMsgLen],m_RECV_Len - start_pos -nMsgLen);
				m_RECV_Len = m_RECV_Len- start_pos-nMsgLen;
				memset(m_RECV_BUF,0,sizeof(m_RECV_BUF));
				memmove(m_RECV_BUF,tempbuf,m_RECV_Len);
				DealScCommMessage();
			}
			else
				return;
			
		}
		else
		{
			return;
		}
	}
}

void CScModule_SZTH::AnalyzeUpMessage( BYTE* tempbyte, int cmdlen, _SC_CMD_ITEM curCmd )
{
	if (tempbyte != NULL && cmdlen >0)
	{
		TRACE("%s,%s\r\n",curCmd.Cmd_Name,curCmd.Cmd_Info);
		//检查校验和
		bool blCkeckSum = true;
		BYTE sum = CUdfGlobalFUNC::XORSumCheck(tempbyte,cmdlen-3);
		if (sum != tempbyte[cmdlen-3])
			blCkeckSum = false;


		CString strLog ="";
		if (blCkeckSum == true)
			strLog.Format("收,卫星通讯:神州天鸿,类型:%s,指令:%s,长度:%d,报文:%s" ,
			curCmd.Cmd_Name,curCmd.Cmd_Info,cmdlen, 
			CUdfGlobalFUNC::ConvertBytesToString(tempbyte,cmdlen));
		else if (blCkeckSum == false)
			strLog.Format("收,卫星通讯:神州天鸿,类型:%s,指令:%s,,和校验:错误,长度:%d,报文:%s" ,
			curCmd.Cmd_Name,curCmd.Cmd_Info,cmdlen, 
			CUdfGlobalFUNC::ConvertBytesToString(tempbyte,cmdlen));

		AddLog(strLog);
		if (blCkeckSum == true)
		{
			if (curCmd.Cmd_Info == "$TSTA")
				;//Deal_Up_TSTAMessage(tempbyte,cmdlen);
			else if (curCmd.Cmd_Info == "$DWXX")
				;//Deal_Up_PINFMessage(tempbyte,cmdlen);
			else if (curCmd.Cmd_Info == "$CASS")
				;//Deal_Up_CASSMessage(tempbyte,cmdlen);
			else if (curCmd.Cmd_Info == "$COUT")
				Deal_Up_COUTMessage(tempbyte,cmdlen);
			else if (curCmd.Cmd_Info == "$CAOP")
				;//Deal_Up_CAOPMessage(tempbyte,cmdlen);
			else if (curCmd.Cmd_Info == "$TINF")
				;//Deal_Up_TINFMessage(tempbyte,cmdlen);
		}
	}
}

void CScModule_SZTH::Deal_Up_TSTAMessage( BYTE* tempbyte, int cmdlen )
{
	if (tempbyte != NULL && cmdlen >= 11)
	{
		CString strCMD="";
		strCMD.Format("$%c%c%c%c",(char)tempbyte[1],(char)tempbyte[2],(char)tempbyte[3],(char)tempbyte[4]);
		if (strCMD != "$GLZJ") return;

		int packetLen = (tempbyte[5] <<8) | (tempbyte[6]);
		if (packetLen != cmdlen)  return;

		UINT localAddr = (tempbyte[7]<<16) | (tempbyte[8] <<8) | (tempbyte[9]);		//本地地址
		BYTE scpd = tempbyte[10];													//输出频道


	}
}

void CScModule_SZTH::Deal_Up_PINFMessage( BYTE* tempbyte, int cmdlen )
{
	if (tempbyte != NULL)
	{
		
	}
}

void CScModule_SZTH::Deal_Up_CASSMessage( BYTE* tempbyte, int cmdlen )
{
	if (tempbyte != NULL)
	{
		CString strCMD = "";
		strCMD.Format("$%c%c%c%c",(char)tempbyte[1],(char)tempbyte[2],(char)tempbyte[3],(char)tempbyte[4]);
		if (strCMD != "$CASS") return;

		CString strSusStatus = "";//成功状态

		int startIdx = -1;
		int endIdx = -1;
		int dataPos = 0;
		int dataCnt = 0;
		BYTE *tempBuf = NULL;


		for (int i=0; i<cmdlen; i++)
		{
			if (0x2C == tempbyte[i])
			{
				if (-1==startIdx && -1==endIdx)
				{
					startIdx = i;
					continue;
				}
				else if (startIdx!=-1 && -1==endIdx)
				{
					endIdx = i;

					dataCnt = endIdx - startIdx - 1;
					if (endIdx - startIdx > 0)
					{
						switch(dataPos)
						{
						case 0:
							tempBuf = new BYTE[dataCnt+1];
							memcpy(tempBuf, tempbyte+startIdx+1, dataCnt);
							strSusStatus.Format("%s", (char)tempBuf);
							delete [] tempBuf;

							if (atoi(strSusStatus)==0)
							{
								strSusStatus = _T("终端收到外设的通信申请，但校验不成功");
							}
							else if (atoi(strSusStatus)==1)
							{
								strSusStatus = _T("终端收到外设通信申请，校验成功");
							}
							else
							{
								strSusStatus = _T("无效成功状态");
							}
							break;
						}

						dataPos++;
						startIdx = endIdx;
						endIdx = -1;

					}
				}
			}

		}

		CString strLog = "";
		strLog.Format("$CASS命令解析：方向：终端到外设，成功状态：%s", 
			strSusStatus);

		AddLog(strLog);

	}
}

void CScModule_SZTH::Deal_Up_COUTMessage( BYTE* tempbyte, int cmdlen )
{
	if (tempbyte != NULL)
	{
		CString strCMD = "";
		strCMD.Format("$%c%c%c%c",(char)tempbyte[1],(char)tempbyte[2],(char)tempbyte[3],(char)tempbyte[4]);
		if (strCMD != "$COUT") return;

		CString strCrcFlag = "";//CRC校验标志
		CString strComType = "";//通信类别
		CString SenderType = "";//发信方类型
		CString SenderAddr = "";//发信方地址
		CString RecvFlags = "";//回执标志
		CString MsgNo = "";//报文顺序号
		CString MsgLen = "";
		BYTE Msg[300];
		memset(Msg, 0, 300);
		CString strCRC = "";//CRC校验

		int startIdx = -1;
		int endIdx = -1;
		int dataPos = 0;
		int dataCnt = 0;
		BYTE *tempBuf = NULL;

		for (int i=0; i<cmdlen; i++)
		{
			if (0x2C == tempbyte[i])
			{
				if (-1==startIdx && -1==endIdx)
				{
					startIdx = i;
					continue;
				}
				else if (startIdx!=-1 && -1==endIdx)
				{
					endIdx = i;
					
					dataCnt = endIdx - startIdx - 1;
					if (endIdx - startIdx > 0)
					{
						switch(dataPos)
						{
						case 0:
							tempBuf = new BYTE[dataCnt+1];
							memset(tempBuf, 0, dataCnt+1);
							memcpy(tempBuf, tempbyte+startIdx+1, dataCnt);
							strCRC.Format("%s", (char*)tempBuf);
							delete [] tempBuf;

							if (atoi(strCRC)==0)
							{
								strCRC = _T("校验正确");
							}
							else if (atoi(strCRC)==1)
							{
								strCRC = _T("校验出错");
							}
							else
							{
								strCRC = _T("无效校验");
							}
							break;
						case 1:
							tempBuf = new BYTE[dataCnt+1];
							memcpy(tempBuf, tempbyte+startIdx+1, dataCnt);
							strComType.Format("%s", (char*)tempBuf);
							delete [] tempBuf;

							if (atoi(strComType)==1)
							{
								strComType = _T("终端到终端的普通通讯");
							}
							else if (atoi(strComType)==2)
							{
								strComType = _T("固定用户到终端的普通通讯");
							}
							else
							{
								strComType = _T("无效通讯类别");
							}
							break;
						case 2:
							tempBuf = new BYTE[dataCnt+1];
							memcpy(tempBuf, tempbyte+startIdx+1, dataCnt);
							SenderType.Format("%s", (char*)tempBuf);
							delete [] tempBuf;
							break;
						case 3:
							tempBuf = new BYTE[dataCnt+1];
							memcpy(tempBuf, tempbyte+startIdx+1, dataCnt);
							SenderAddr.Format("%s", (char*)tempBuf);
							delete [] tempBuf;
							break;
						case 4:
							tempBuf = new BYTE[dataCnt+1];
							memcpy(tempBuf, tempbyte+startIdx+1, dataCnt);
							RecvFlags.Format("%s", (char*)tempBuf);
							delete [] tempBuf;

							if (atoi(RecvFlags)==0)
							{
								RecvFlags = _T("不需要回执");
							}
							else if (atoi(RecvFlags)==1)
							{
								RecvFlags = _T("需要回执");
							}
							else
							{
								RecvFlags = _T("错误回执标志");
							}

							break;
						case 5:
							tempBuf = new BYTE[dataCnt+1];
							memcpy(tempBuf, tempbyte+startIdx+1, dataCnt);
							MsgNo.Format("%s", (char*)tempBuf);
							delete [] tempBuf;
							break;
						case 6:
							tempBuf = new BYTE[dataCnt+1];
							memcpy(tempBuf, tempbyte+startIdx+1, dataCnt);
							MsgLen.Format("%s", (char*)tempBuf);
							delete [] tempBuf;
							break;
						case 7:
							memcpy(Msg, tempbyte+startIdx+1, dataCnt);
							break;
						}

						dataPos++;
						startIdx = endIdx;
						endIdx = -1;

					}
				}
			}
			else if (0x0D == tempbyte[cmdlen-2] && 0x0A == tempbyte[cmdlen-1])
			{
				;
			}
		}


		if (atoi(MsgLen) > 0)
		{	

			CString strLog = "";
			strLog.Format("$COUT命令解析：校验标志：%s, 通讯类别：%s, 发信方地址：%s, 回执标志：%s, 报文顺序号：%s", 
				strCRC, strComType, SenderAddr, RecvFlags, MsgNo);

			AddLog(strLog);

			CSCMessageDealModule* pScMessageModule = NULL;
			std::map<UINT,CSCMessageDealModule*>::iterator itrFind = m_mapSCMessageDeal.find(atoi(SenderAddr));
			if (itrFind != m_mapSCMessageDeal.end() && 
				itrFind->second != NULL)
			{
				pScMessageModule = itrFind->second;
			}
			else
			{
				pScMessageModule = new CSCMessageDealModule(atoi(SenderAddr),this);
				if (pScMessageModule)
					m_mapSCMessageDeal.insert(std::pair<UINT,CSCMessageDealModule*>(pScMessageModule->GetRemoteAddress(),pScMessageModule));			
			}

			if (pScMessageModule)
			{
				pScMessageModule->SwitchHandlerModel(m_bAppProtoType);
				pScMessageModule->RecvMessage(Msg,atoi(MsgLen));
			}

		}
	}
}

void CScModule_SZTH::Deal_Up_CAOPMessage( BYTE* tempbyte, int cmdlen )
{
	if (tempbyte != NULL && cmdlen >= 21 )
	{
		CString strCMD="";
		strCMD.Format("$%c%c%c%c",(char)tempbyte[1],(char)tempbyte[2],(char)tempbyte[3],(char)tempbyte[4]);
		if (strCMD != "$ZJXX") return;

		int packetLen = (tempbyte[5] <<8) | (tempbyte[6]);
		if (packetLen != cmdlen)  return;

		UINT localAddr = (tempbyte[7]<<16) | (tempbyte[8] <<8) | (tempbyte[9]);		//本地地址
		BYTE ickstate = tempbyte[10];												//IC卡状态
		BYTE yjstate  = tempbyte[11];												//硬件状态
		BYTE dcdl	  = tempbyte[12];												//电池电量
		BYTE rzstate  = tempbyte[13];												//入站状态
		BYTE bsgl1	  = tempbyte[14];												//波束1功率
		BYTE bsgl2	  = tempbyte[15];												//波束2功率
		BYTE bsgl3	  = tempbyte[16];												//波束3功率
		BYTE bsgl4	  = tempbyte[17];												//波束4功率
		BYTE bsgl5	  = tempbyte[18];												//波束5功率
		BYTE bsgl6	  = tempbyte[19];												//波束6功率

	}
}

void CScModule_SZTH::Deal_Up_TINFMessage( BYTE* tempbyte, int cmdlen )
{
	if (tempbyte != NULL && cmdlen >= 21 )
	{
		CString strCMD="";
		strCMD.Format("$%c%c%c%c",(char)tempbyte[1],(char)tempbyte[2],(char)tempbyte[3],(char)tempbyte[4]);
		if (strCMD != "$SJXX") return;

		int packetLen = (tempbyte[5] <<8) | (tempbyte[6]);
		if (packetLen != cmdlen)  return;

		UINT localAddr = (tempbyte[7]<<16) | (tempbyte[8] <<8) | (tempbyte[9]);		//本地地址
		int  year = tempbyte[10]<<8|tempbyte[11];
		int  month =  tempbyte[12];
		int  day	= tempbyte[13];
		int  hour = tempbyte[14];
		int  minute = tempbyte[15];
		int  second = tempbyte[16];

	}
}



//
void CScModule_SZTH::SndDown_DWSQMessage( BYTE msgType,UINT32 gcsjAndtxg,UINT32 qysj,UINT16 rzpd )
{

}

void CScModule_SZTH::SndDown_TXSQMessage( BYTE msgType,UINT32 remoteAddr, BYTE ackType,BYTE* message,int msgLen )
{
	if (message != NULL && msgLen >0)
	{
		int nSendCount = msgLen / m_nOneSendMax + 1;
		int nLastmsgLen = msgLen % m_nOneSendMax;

		for (int sendIdx = 0 ; sendIdx < nSendCount; sendIdx ++)
		{   
			int curMsgLen = m_nOneSendMax;
			if (sendIdx == nSendCount -1)
				curMsgLen = nLastmsgLen;

			UINT16 packetlen = 5 + 2 + 3 + 1 + 3 + 2 + 1 + curMsgLen + 1;
			PsendObject pSndObj = new sendObject();

			pSndObj->sendLen = packetlen;

			char cmd[6] = "$TXSQ";
			int idx = 0;
			memmove(pSndObj->sendbuf,cmd,5);
			idx += 5;

			//len
			pSndObj->sendbuf[idx]		= packetlen / 256;
			pSndObj->sendbuf[idx +1]	= packetlen % 256;
			idx += 2;

			BYTE temp[4] ;
			memmove(temp,&m_nAddressLocal,4);

			pSndObj->sendbuf[idx]		= temp[2];
			pSndObj->sendbuf[idx + 1]	= temp[1];
			pSndObj->sendbuf[idx + 2]	= temp[0];
			idx += 3;

			pSndObj->sendbuf[idx]		= msgType ;
			idx += 1;

			memmove(temp,&remoteAddr,4);
			pSndObj->sendbuf[idx]		= temp[2];
			pSndObj->sendbuf[idx + 1]	= temp[1];
			pSndObj->sendbuf[idx + 2]	= temp[0];
			idx += 3;

			pSndObj->sendbuf[idx]		= (curMsgLen * 8) / 256;
			pSndObj->sendbuf[idx+1]		= (curMsgLen * 8) % 256;
			idx += 2;


			pSndObj->sendbuf[idx]		= ackType;
			idx += 1;

			memmove(&(pSndObj->sendbuf[idx]),&message[m_nOneSendMax * sendIdx],curMsgLen);
			idx += msgLen;

			pSndObj->sendbuf[idx]		= CUdfGlobalFUNC::XORSumCheck(pSndObj->sendbuf,idx);

			AddOneSendObject(pSndObj);
		}
	}
}

void CScModule_SZTH::SndDown_CKSCMessage( BYTE cspl )
{

}

void CScModule_SZTH::SndDown_ICJCMessage( BYTE zh )
{

}

void CScModule_SZTH::SndDown_XTZJMessage( UINT16 zjpd )
{

}

void CScModule_SZTH::SndDown_SJSCMessage( UINT16 scpd )
{

}


void CScModule_SZTH::SndDown_BBDQMessage()
{

}
