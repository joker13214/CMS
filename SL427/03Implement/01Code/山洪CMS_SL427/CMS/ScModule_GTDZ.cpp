#include "StdAfx.h"
#include "ScModule_GTDZ.h"
#include "RtuStation.h"
#include <string.h>
#include <map>

 #pragma   warning(once:4996)

CScModule_GTDZ::CScModule_GTDZ(void)
{
	m_nSendInterval = 60 *1000;      //���Ƿ��ͱ���ʱ����:1����
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
			   //��ȡ��Ҫ���͵ı���
			   PsendObject pSndObj = GetOneSendObject();
			   if (pSndObj  != NULL)
			   {
				   memmove(buf,pSndObj->sendbuf,pSndObj->sendLen);
				   len = pSndObj->sendLen;

				   CString strLog="";
				   strLog.Format("��,����ͨѶ:���ڵ���(V4.0Э��),����:%d,����:%s",pSndObj->sendLen,
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
					//��ȡָ��/��Ϣ
					CString strCMD = "";
					strCMD.Format("$%c%c%c%c",
						(char)m_RECV_BUF[start_pos + 1],
						(char)m_RECV_BUF[start_pos + 2],
						(char)m_RECV_BUF[start_pos + 3],
						(char)m_RECV_BUF[start_pos + 4]);

					//���ָ���Ƿ���ڣ������ڼ�������ָ��

					bool blFinded = false;
				    _SC_CMD_ITEM curCmd ;     //����ͨ�����б���������Ϣ��ָ�����ͣ�
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
							//��ȡ��ȷ����ǰδ������Ҫ�����ı��ģ�������־��
							BYTE tempbuf[1024 * 10];
							if (start_pos != 0)
							{
								memmove(tempbuf,m_RECV_BUF,start_pos);
								CString strLog = "";
								strLog.Format("��,����ͨ��:���ڵ���(V4.0Э��),����:%d,����:%s" ,start_pos,
									CUdfGlobalFUNC::ConvertBytesToString(tempbuf,start_pos));
								AddLog(strLog);
							}

							BYTE* tempbyte = new BYTE[cmdlen];
							memmove(tempbyte,&m_RECV_BUF[start_pos],cmdlen);
                            //��������
							AnalyzeUpMessage(tempbyte,cmdlen,curCmd);
							delete[] tempbyte;

							//��ȡ���ģ������´���							
							memmove(tempbuf,&m_RECV_BUF[start_pos + cmdlen],m_RECV_Len - start_pos -cmdlen);
							m_RECV_Len = m_RECV_Len- start_pos-cmdlen;
							memset(m_RECV_BUF,0,sizeof(m_RECV_BUF));
							memmove(m_RECV_BUF,tempbuf,m_RECV_Len);
							DealScCommMessage();
						}
						else return;  //���ĳ��Ȳ���
					}
				}
				else return;    //���ĳ��Ȳ���
			}
		}
	}
}

void CScModule_GTDZ::AnalyzeUpMessage( BYTE* tempbyte, int cmdlen, _SC_CMD_ITEM curCmd )
{
	if (tempbyte != NULL && cmdlen >0)
	{
		TRACE("%s,%s\r\n",curCmd.Cmd_Name,curCmd.Cmd_Info);
		//���У���
		bool blCkeckSum = true;
		if (curCmd.Cmd_Info != "$GLJC")
		{
			BYTE sum = CUdfGlobalFUNC::XORSumCheck(tempbyte,cmdlen-1);
			if (sum != tempbyte[cmdlen-1])
				blCkeckSum = false;
		}
		
		CString strLog ="";
		if (blCkeckSum == true)
			strLog.Format("��,����ͨѶ:���ڵ���(V4.0Э��),����:%s,ָ��:%s,����:%d,����:%s" ,
							curCmd.Cmd_Name,curCmd.Cmd_Info,cmdlen, 
							CUdfGlobalFUNC::ConvertBytesToString(tempbyte,cmdlen));
		else if (blCkeckSum == false)
			strLog.Format("��,����ͨѶ:���ڵ���(V4.0Э��),����:%s,ָ��:%s,,��У��:����,����:%d,����:%s" ,
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

		UINT localAddr = (tempbyte[7]<<16) | (tempbyte[8] <<8) | (tempbyte[9]);		//���ص�ַ
		BYTE scpd = tempbyte[10];													//���Ƶ��

	
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

		UINT localAddr = (tempbyte[7]<<16) | (tempbyte[8] <<8) | (tempbyte[9]);		//���ص�ַ
		


	}
}

void CScModule_GTDZ::Deal_Up_TXXXMessage( BYTE* tempbyte, int cmdlen )
{
	if (tempbyte != NULL && cmdlen >= 19)
	{					//��ȡָ��/��Ϣ
		CString strCMD = "";
		strCMD.Format("$%c%c%c%c",(char)tempbyte[1],(char)tempbyte[2],(char)tempbyte[3],(char)tempbyte[4]);
		if (strCMD != "$TXXX") return;

		int packetLen = (tempbyte[5] <<8) | (tempbyte[6]);
		if (packetLen != cmdlen)  return;

		UINT localAddr = (tempbyte[7]<<16) | (tempbyte[8] <<8) | (tempbyte[9]);		//���ص�ַ
		BYTE messageType = tempbyte[10];											//��Ϣ����
		UINT remoteAddr = (tempbyte[11]<<16)| (tempbyte[12]<<8) |(tempbyte[13]);	//Դ��ַ
		BYTE Hour = tempbyte[14];
		BYTE Minute = tempbyte[15];
		UINT16 messageLen = (tempbyte[16]<<8)|(tempbyte[17]);							//$TXXX�����е��ĵ� bits����;
		BYTE crcCheck = tempbyte[18 + messageLen];									//crcУ���ֽ�

		if (messageLen > 0)
		{			
			int appLen = messageLen /8;												//$TXXX�����е��ĵ� bytes����;

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
					pScMessageModule->RecvMessage(message,appLen);

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

		UINT localAddr = (tempbyte[7]<<16) | (tempbyte[8] <<8) | (tempbyte[9]);		//���ص�ַ
		BYTE zh	= tempbyte[10];														//֡��
		UINT32 tbID	= tempbyte[11] | tempbyte[12]<<8|tempbyte[13]<<16;				//ͨ��ID
		BYTE   userFlag	= tempbyte[14];												//�û�����
		UINT16 fwpd		= tempbyte[15] |tempbyte[16] <<8;							//����Ƶ��
		UINT8  txdj		= tempbyte[17];												//ͨ�ŵȼ�
		UINT8  jmflag	= tempbyte[18];												//���ܱ�־
		UINT16 xsyhzs	= tempbyte[19] | tempbyte[20] <<8;							//�����û�����

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

		UINT localAddr = (tempbyte[7]<<16) | (tempbyte[8] <<8) | (tempbyte[9]);		//���ص�ַ
		BYTE ickstate = tempbyte[10];												//IC��״̬
		BYTE yjstate  = tempbyte[11];												//Ӳ��״̬
		BYTE dcdl	  = tempbyte[12];												//��ص���
		BYTE rzstate  = tempbyte[13];												//��վ״̬
		BYTE bsgl1	  = tempbyte[14];												//����1����
		BYTE bsgl2	  = tempbyte[15];												//����2����
		BYTE bsgl3	  = tempbyte[16];												//����3����
		BYTE bsgl4	  = tempbyte[17];												//����4����
		BYTE bsgl5	  = tempbyte[18];												//����5����
		BYTE bsgl6	  = tempbyte[19];												//����6����

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

		UINT localAddr = (tempbyte[7]<<16) | (tempbyte[8] <<8) | (tempbyte[9]);		//���ص�ַ
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

		UINT localAddr = (tempbyte[7]<<16) | (tempbyte[8] <<8) | (tempbyte[9]);		//���ص�ַ

		int bbxxLen = packetLen - 11;
		CString strbbxx = "";  //�汾��Ϣ
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

		UINT localAddr = (tempbyte[7]<<16) | (tempbyte[8] <<8) | (tempbyte[9]);		//���ص�ַ

		BYTE fkflag = tempbyte[10];													//������־ 0x00:�ɹ�,0x01:ʧ��, 0x06:CRC����


		//32bite�ĸ�����Ϣ
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




CScModule_GTDZ_2_5::CScModule_GTDZ_2_5(void)
{
}

CScModule_GTDZ_2_5::~CScModule_GTDZ_2_5(void)
{
}

void CScModule_GTDZ_2_5::RecvMessage( BYTE* buf,int Len )
{
	AppendRecvData(buf,Len);
	DealScCommMessage();

}

bool CScModule_GTDZ_2_5::SendMessage( BYTE* buf,int &len )
{
	bool ret = false;
	if (buf !=NULL && len >0)
	{


		ret = true;
	}

	return ret;
}

void CScModule_GTDZ_2_5::DealScCommMessage()
{
	if (m_RECV_Len >= 6)
	{
		int start_pos = 0;
		for (start_pos =0; start_pos < m_RECV_Len; start_pos ++)
		{
			if ((char)m_RECV_BUF[start_pos] =='$') //�ҵ�start
			{
				if (start_pos + 4 < m_RECV_Len)
				{
					//�ҵ�end 
					int end_pos = start_pos +1;
					for (;end_pos < m_RECV_Len-1; end_pos ++)
					{
						if (m_RECV_BUF[end_pos] == 0x0d && 
							m_RECV_BUF[end_pos + 1] == 0x0a)  //�ҵ����Ľ�β
						{

							//�������ĳ���
							int cmdlen = end_pos - start_pos + 2;

							//�ɼ�������ж��Ƿ���������ȷ�ı���
							if (m_RECV_BUF[end_pos-1] == CUdfGlobalFUNC::XORSumCheck(&m_RECV_BUF[start_pos],cmdlen-3))
							{
								//��ȡ��ȷ����ǰδ������Ҫ�����ı��ģ�������־��
								BYTE tempbuf[1024 * 10];
								if (start_pos != 0)
								{
									memmove(tempbuf,m_RECV_BUF,start_pos);
									CString strLog = "";
									strLog.Format("��,����ͨ��:���ڵ���(V2.5Э��),����:%d,����:%s" ,start_pos,
										CUdfGlobalFUNC::ConvertBytesToString(tempbuf,start_pos));
									AddLog(strLog);
								}


								//��ȡָ��/��Ϣ
								CString strCMD = "";
								strCMD.Format("$%c%c%c%c",
									(char)m_RECV_BUF[start_pos + 1],
									(char)m_RECV_BUF[start_pos + 2],
									(char)m_RECV_BUF[start_pos + 3],
									(char)m_RECV_BUF[start_pos + 4]);

								bool blFinded = false;
								_SC_CMD_ITEM curCmd ;     //����ͨ�����б���������Ϣ��ָ�����ͣ�
								for (int ni =0; ni < 14; ni ++)
								{
									if (strCMD == GTDZ2_5_UP_CMDS[ni].Cmd_Info)
									{
										blFinded = true;
										curCmd.Cmd_Info = GTDZ2_5_UP_CMDS[ni].Cmd_Info;
										curCmd.Cmd_Name = GTDZ2_5_UP_CMDS[ni].Cmd_Name;
										break;
									}
								}

								//��������
								BYTE* tempbyte = new BYTE[cmdlen];
								memmove(tempbyte,&m_RECV_BUF[start_pos],cmdlen);

								//��������
								CString strLog = "";
								if (blFinded == true)
								{
									strLog.Format("��,����ͨѶ:���ڵ���(V2.5Э��),����:%s,ָ��:%s,����:%d,����:%s" ,
										curCmd.Cmd_Name,curCmd.Cmd_Info,cmdlen, 
										CUdfGlobalFUNC::ConvertBytesToString(tempbyte,cmdlen));
									AddLog(strLog);
									AnalyzeUpMessage(tempbyte,cmdlen,curCmd);
								}
								else
								{
									strLog.Format("��,����ͨѶ:���ڵ���(V2.5Э��),����:%s,ָ��:%s,����:%d,����:%s" ,
										"δ֪",strCMD,cmdlen, 
										CUdfGlobalFUNC::ConvertBytesToString(tempbyte,cmdlen));
									AddLog(strLog);
								}
								delete[] tempbyte;

								//��ȡ���ģ������´���							
								memmove(tempbuf,&m_RECV_BUF[start_pos + cmdlen],m_RECV_Len - start_pos -cmdlen);
								m_RECV_Len = m_RECV_Len- start_pos-cmdlen;
								memset(m_RECV_BUF,0,sizeof(m_RECV_BUF));
								memmove(m_RECV_BUF,tempbuf,m_RECV_Len);
								DealScCommMessage();
							}
						}
					}
				}
			}
		}
	}
}

void CScModule_GTDZ_2_5::AnalyzeUpMessage( BYTE* tempbyte, int cmdlen, _SC_CMD_ITEM curCmd )
{
	if (tempbyte != NULL && cmdlen >0)
	{
		if (curCmd.Cmd_Info == "$COUT")  //{"ͨ�����","$COUT"},
			Deal_Up_COUTMessage(tempbyte,cmdlen);
	}
}

void CScModule_GTDZ_2_5::Deal_Up_COUTMessage( BYTE* tempbyte,int cmdlen )
{
	if (tempbyte != NULL && cmdlen >0 )
	{
		try
		{
			CString strCMD = "";
			strCMD.Format("$%c%c%c%c",(char)tempbyte[1],(char)tempbyte[2],(char)tempbyte[3],(char)tempbyte[4]);
			if (strCMD != "$COUT") 
				return;

			char* s = new char[cmdlen +1];
			memset(s,0,cmdlen +1);
			memmove(s,tempbyte,cmdlen);
			const char* d = ",";  //�ָ���
			char* p = NULL;

			//����ַ���

			int pos = 0;  //�ַ����������

			std::map<int,CString> tempMap;  
			p = strtok(s,d);
			while(p)
			{
				CString temp ="";
				temp.Format("%s",p);
				tempMap.insert(std::pair<int,CString>(pos,temp));
				TRACE("%d,%s\r\n",pos,temp);
				p = strtok(NULL,d);

				pos ++;
			}

			delete[] s;

			std::map<int,CString>::iterator itr = tempMap.end();
		
			//Զ�����ǵ�ַ
			itr = tempMap.find(4);
			if (itr != tempMap.end())
			{
				//Զ�����ǵ�ַ
				UINT remoteAddr = (UINT)atol(itr->second);

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

				//Ӧ�ñ���
				itr = tempMap.find(7);
				if (itr != tempMap.end())
				{
					//Ӧ�ñ��ĵĳ���
					int appLen = atoi(itr->second);

					BYTE* message = new BYTE[appLen];
					//memmove(message,&tempbyte[18],appLen);

					//����Ӧ�ñ��ĵ���ʼδ֪
					int start_pos = 0;
					for (int i=0 ; i < 8;i++)
					{
						itr = tempMap.find(i);
						start_pos += strlen(itr->second) + 1;
					}

					//Ӧ�ñ�������
					memmove(message,&tempbyte[start_pos],appLen);

					if (pScMessageModule)
						pScMessageModule->RecvMessage(message,appLen);

					delete[] message;
				}
			}
		}
		catch (CException* e)
		{
			e->Delete();
		}
	}
}


TextMsg_GHT::TextMsg_GHT()
{
	memset(m_FrameBuf, 0, 1024);
	m_FrameBufLen = 0;
	m_currCmd = "";

}

TextMsg_GHT::~TextMsg_GHT()
{

}

void TextMsg_GHT::RecvMessage(BYTE* buf,int Len)
{
	AppendRecvData(buf,Len);
	DealTmCommMessage_GHT();

}

bool TextMsg_GHT::SendMessage(BYTE* buf,int &len)
{
	if (buf != NULL)
	{   
		if (m_bSendBefore == false || 
			(m_bSendBefore == true && 
			m_CheckSendTimer.WaitTimerState() == true))
		{
			//��ȡ��Ҫ���͵ı���
			PsendObject pSndObj = GetOneSendObject();
			if (pSndObj  != NULL)
			{
				memmove(buf,pSndObj->sendbuf,pSndObj->sendLen);
				len = pSndObj->sendLen;

				CString strLog="";
				strLog.Format("��,����ͨѶ:���ڵ���,����:%d,����:%s",pSndObj->sendLen,
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

void TextMsg_GHT::SendDown_Message(TextMsg *msg, int type)
{
	if (msg->m_buf != NULL && msg->m_len > 0)
	{
		switch(type) 
		{
		case MSG_INIT:
			{
				m_cs.Lock();
				PsendObject pSndObj = new sendObject();
				pSndObj->sendLen = msg->m_len;
				memcpy(pSndObj->sendbuf, msg->m_buf, msg->m_len);
				AddOneSendObject(pSndObj);
				m_cs.Unlock();
			}
			break;
		case MSG_SEND:
			{
				m_cs.Lock();
				PsendObject pSndObj = new sendObject();
				pSndObj->sendLen = msg->m_len;
				memcpy(pSndObj->sendbuf, msg->m_buf, msg->m_len);
				AddOneSendObject(pSndObj);
				m_cs.Lock();
			}
			break;
		}
		
	}

}

void TextMsg_GHT::SendDown_Message(BYTE *buf, int len, int type)
{
	if (buf != NULL && len > 0)
	{
		switch(type) 
		{
		case MSG_INIT:
			{
				m_cs.Lock();
				PsendObject pSndObj = new sendObject();
				pSndObj->sendLen = len;
				memcpy(pSndObj->sendbuf, buf, len);
				AddOneSendObject(pSndObj);
				m_cs.Unlock();
			}
			break;
		case MSG_SEND:
			{
				PsendObject pSndObj = new sendObject();
				pSndObj->sendLen = len;
				memcpy(pSndObj->sendbuf, buf, len);
				AddOneSendObject(pSndObj);

			}
			break;
		}

	}

}

void TextMsg_GHT::SetCommInfo(COM_INFO comm_info)
{
	m_commInfo = comm_info;
}

void TextMsg_GHT::SetStCode(int stCode)
{
	m_nStCode = stCode;
}

COM_INFO TextMsg_GHT::GetCommInfo()
{
	return m_commInfo;
}

bool TextMsg_GHT::DealTmCommMessage_GHT()
{
	if (m_bIsMsgInit)
	{
		DealTmCommMessage_GHT_Init();
		
	}
	else
	{
		DealTmCommMessage_GHT_Send();
	}

	return true;

}


bool TextMsg_GHT::DealTmCommMessage_GHT_Send()
{
	int start_pos = -1;
	int end_pos = -1;
	bool bFind_head = false;
	bool bFind_tail = false;
	bool bRecv_OK = false;
	bool bFind_Msg = false;
	int cmd = -1;

	BYTE temp[1024];
	memset(temp, 0, 1024);

	if (m_RECV_Len >= 0)
	{
		for (int i=0; i<m_RECV_Len; i++)
		{
			if (bFind_head == false)
			{
				if ((char)m_RECV_BUF[i] == '+')
				{
					CString strCMD = "";
					strCMD.Format("$%c%c%c%c",
						(char)m_RECV_BUF[i+1],
						(char)m_RECV_BUF[i+2],
						(char)m_RECV_BUF[i+3],
						(char)m_RECV_BUF[i+4]);

					if ("CMGL"==strCMD || "CPMS"==strCMD || "CMGS"==strCMD ||
						"CMTI"==strCMD || "CMGR"==strCMD || "CMTI"==strCMD)
 					{//��ȡ����
 						m_currCmd = strCMD;
						if (strCMD == "CMGL")
						{
							cmd = CMGL_waiting_OK;
						}
						else if (strCMD == "CMGR")
						{
							cmd = CMGR_waiting_OK;
						}
						else if (strCMD == "CPMS")
						{
							switch (GetMsgTextDlg()->GetCmd())
							{
							case CPMS_sending:
								cmd = CPMS_waiting_OK;
								break;
							case CPMSa_sending:
								cmd = CPMSa_waiting_OK;
								break;

							}
							
						}
						else if (strCMD == "CMGS")
						{
							//̫�����ˣ�����д
						}
						else if (strCMD == "CMTI")
						{
							cmd = Recv_CMTI;
							
						}

							
					

 						start_pos = i;
 						bFind_head = true;
						continue;
 					}

				}
			}

			if (bFind_head && !bFind_tail)
			{//����ҵ���ͷ������û���ҵ�β��
				bFind_Msg = false;

				if (i+3 < m_RECV_Len)
				{
					
					if ((char)m_RECV_BUF[i] == '\n')
					{
						if ((char)m_RECV_BUF[i+1]=='O' 
							&& (char)m_RECV_BUF[i+2]=='K'
							&& (char)m_RECV_BUF[i+3]=='\n')
						{//�ҵ���β��
							int nMsgLen = (i-1)-start_pos+1;//������\nOK\n�ĳ���
							ZeroMemory(m_FrameBuf, sizeof(m_FrameBuf));
							memcpy(m_FrameBuf, m_RECV_BUF+start_pos, nMsgLen);
						    m_FrameBufLen = nMsgLen;

							int len = m_RECV_Len - (i+4); 
							ZeroMemory(temp, sizeof(temp));
							memmove(temp, m_RECV_BUF+start_pos, len);
							ZeroMemory(m_RECV_BUF, sizeof(m_RECV_BUF));
							memmove(m_RECV_BUF, temp, len);
							m_RECV_Len = len;
							bFind_tail = true;
							bFind_Msg = true;
							break;//�ҵ����������ģ�����ѭ��
						}
					}
				}
				else
				{
					int len = m_RECV_Len - start_pos; 
					ZeroMemory(temp, sizeof(temp));
					memmove(temp, &m_RECV_BUF[start_pos], len);
					ZeroMemory(m_RECV_BUF, sizeof(m_RECV_BUF));
					memmove(m_RECV_BUF, temp, len);
					m_RECV_Len = len;
					bFind_Msg = false;
					break;
				}
	
			}
			else if (!bFind_head && !bFind_tail)
			{//���û�ҵ�ͷ��Ҳû���ҵ�β�ͣ���׼����β��
				bFind_Msg = false;
				if (i+2 < m_RECV_Len)
				{
					if ((char)m_RECV_BUF[i]=='O'
						&& (char)m_RECV_BUF[i+1]=='K'
						&& (char)m_RECV_BUF[i+2] == '\n')
					{
						//�ҵ���OK��־
						bFind_Msg = false;

						if (i!=0)
						{
							//��ok�Լ�ok��ǰ������ȫ����������
							int len = m_RECV_Len - (i+2) - 1;
							ZeroMemory(temp, sizeof(temp));
							memmove(temp, m_RECV_BUF+i+3, len);
							ZeroMemory(m_RECV_BUF, sizeof(m_RECV_BUF));
							memmove(m_RECV_BUF, temp, len);
							m_RECV_Len = len;
						}
						else
						{
							int len = m_RECV_Len - 3;
							if (len >= 0)
							{
								//�Ѷ�����ֽڿ�������
								ZeroMemory(temp, sizeof(temp));
								memmove(temp, m_RECV_BUF+3, len);
								ZeroMemory(m_RECV_BUF, sizeof(m_RECV_BUF));
								memmove(m_RECV_BUF, temp, len);
								m_RECV_Len = len;
								

								switch (GetMsgTextDlg()->GetCmd())
								{
								case CMGF_sending:
									cmd = CMGF_OK_finish;
									GetMsgTextDlg()->UpdateDlg(cmd, TRUE, NULL);
									break;
								case CSMP_sending:
									cmd = CSMP_OK_finish;
									GetMsgTextDlg()->UpdateDlg(cmd, TRUE, NULL);
									break;
								case CMGD_sending:
									cmd = CMGD_OK_finish;
									GetMsgTextDlg()->UpdateDlg(cmd, TRUE, NULL);
									break;
								}
							}
						}		
					}
				}
				else
				{
					int len = m_RECV_Len; 
					ZeroMemory(temp, sizeof(temp));
					memmove(temp, &m_RECV_BUF[start_pos], len);
					ZeroMemory(m_RECV_BUF, sizeof(m_RECV_BUF));
					memmove(m_RECV_BUF, temp, len);
					m_RECV_Len = len;
					bFind_Msg = false;
					break;

				}
				
			}
		}

		if (bFind_Msg)
		{
			switch (cmd)
			{
			case CPMS_waiting_OK:
				Deal_CPMS();
				break;
			case CPMSa_waiting_OK:
				Deal_CPMSa();
				break;
			case CMGL_waiting_OK:
			case CMGR_waiting_OK:
				Deal_CMGL();
				break;
			case Recv_CMTI:
				Deal_CTMI();
				break;
			}
			return true;
		}


	}
	return false;

}


CString TextMsg_GHT::BufferToString(BYTE *buf, int len )
{
	if (!buf  || len <= 0)
		return "";

	CString strResult = "";
	for (int idx = 0 ; idx < len; idx ++)
	{
		CString strTemp = "";
		strTemp.Format("%02X ",buf[idx]);
		strResult += strTemp;
	}
	return strResult;

}

void TextMsg_GHT::Deal_CPMS()
{
	std::list<CString> list;
	CString str = (LPCTSTR)m_FrameBuf;
	CString resToken;
	int curPos = 0;

	resToken = str.Tokenize(_T(","), curPos);
	while (resToken != _T(""))
	{
		list.push_back(resToken);
		resToken = str.Tokenize(_T(","),curPos);
	}

	int mem_used_1 = 0;
	int mem_full_1 = 0;
	int mem_used_2 = 0;
	int mem_full_2 = 0;
	int mem_used_3 = 0;
	int mem_full_3 = 0;

	CString text = "";
	std::list<CString>::iterator it;
	CString content = "";
	int idx = 0;
	bool bOK = true;
	for (it=list.begin(); it!=list.end(); it++)
	{
		content = (CString)(*it);
		switch(idx)
		{
		case 0:
			if (content.Left(6) == "+CPMS:")
			{
				mem_used_1 = atoi(content.Right(7));
				idx++;
			}
			else
			{
				bOK = false;
			}
			break;
		case 1:
			mem_full_1 = atoi(content);
			idx++;
			break;
		case 2:
			mem_used_2 = atoi(content);
			idx++;
			break;
		case 3:
			mem_full_2 = atoi(content);
			idx++;
			break;
		case 4:
			mem_used_3 = atoi(content);
			idx++;
			break;
		case 5:
			mem_full_3 = atoi(content);
			idx++;
			break;
		default:
			idx++;
			break;
		}

		if (!bOK)
		{
			break;
		}
		
	}

	text.Format("�洢��1��%d/%d ,�洢��2��%d/%d ���洢��3��%d/%d",
		mem_used_1, mem_full_1,
		mem_used_2, mem_full_2,
		mem_used_3, mem_full_3);


	TM_Unit *pTmUnit = new TM_Unit();
	SYSTEMTIME t;
	::GetLocalTime(&t);

	CString time = "";
	time.Format("%d:%d:%d %d-%d-%d",t.wHour,t.wMinute,t.wSecond,
		t.wYear,t.wMonth,t.wDay);

	pTmUnit->SetSendMsgType(false);
	pTmUnit->SetContent(text);
	pTmUnit->SetRecvTime(time);
	pTmUnit->SetRead(false);
	pTmUnit->SetPhoneNum(_T("δ֪����"));

	
	GetMsgTextDlg()->UpdateDlg(CPMS_waiting_OK, TRUE, pTmUnit);

}


void TextMsg_GHT::Deal_CPMSa()
{

	std::list<CString> list;
	CString str = (LPCTSTR)m_FrameBuf;
	CString resToken;
	int curPos = 0;

	resToken = str.Tokenize(_T(","), curPos);
	while (resToken != _T(""))
	{
		list.push_back(resToken);
		resToken = str.Tokenize(_T(","),curPos);
	}

	CString mem1 = "";
	CString mem2 = "";
	CString mem3 = "";
	int mem_used_1 = 0;
	int mem_full_1 = 0;
	int mem_used_2 = 0;
	int mem_full_2 = 0;
	int mem_used_3 = 0;
	int mem_full_3 = 0;

	CString text = "";
	std::list<CString>::iterator it;
	CString content = "";
	int idx = 0;
	bool bOK = true;
	for (it=list.begin(); it!=list.end(); it++)
	{
		content = (CString)(*it);
		switch(idx)
		{
		case 0:
			if (content.Left(6) == "+CPMS:")
			{
				mem1 = content.Right(7);
				idx++;
			}
			else
			{
				bOK = false;
			}
			break;
		case 1:
			mem_used_1 = atoi(content);
			idx++;
			break;
		case 2:
			mem_full_1 = atoi(content);
			idx++;
			break;
		case 3:
			mem2 = content;
			break;
		case 4:
			mem_used_2 = atoi(content);
			idx++;
			break;
		case 5:
			mem_full_2 = atoi(content);
			idx++;
			break;
		case 6:
			mem3 = content;
			break;
		case 7:
			mem_used_3 = atoi(content);
			idx++;
			break;
		case 8:
			mem_full_3 = atoi(content);
			idx++;
			break;
		default:
			idx++;
			break;
		}

		if (!bOK)
		{
			break;
		}

	}


	text.Format("%s�洢��1��%d/%d ,%s�洢��2��%d/%d ��%s�洢��3��%d/%d",
		mem1,
		mem_used_1, mem_full_1,
		mem2,
		mem_used_2, mem_full_2,
		mem3,
		mem_used_3, mem_full_3);

	TM_Unit *pTmUnit = new TM_Unit();
	SYSTEMTIME t;
	::GetLocalTime(&t);

	CString time = "";
	time.Format("%d:%d:%d %d-%d-%d",t.wHour,t.wMinute,t.wSecond,
		t.wYear,t.wMonth,t.wDay);

	pTmUnit->SetSendMsgType(false);
	pTmUnit->SetContent(text);
	pTmUnit->SetRecvTime(time);
	pTmUnit->SetRead(false);
	pTmUnit->SetPhoneNum(_T("δ֪����"));

	GetMsgTextDlg()->UpdateDlg(CPMSa_waiting_OK, TRUE, pTmUnit);

}

void TextMsg_GHT::Deal_CSMP()
{

}

void TextMsg_GHT::Deal_CMGD()
{

}

void TextMsg_GHT::Deal_CMGS()
{

}

void TextMsg_GHT::Deal_CMGF()
{

}

void TextMsg_GHT::Deal_CTMI()
{
	std::list<CString> list;
	CString str = (LPCTSTR)m_FrameBuf;
	CString resToken;
	int curPos = 0;

	resToken = str.Tokenize(_T(","), curPos);
	while (resToken != _T(""))
	{
		list.push_back(resToken);
		resToken = str.Tokenize(_T(","),curPos);
	}

	int No = atoi(str.Right(1));
	GetMsgTextDlg()->RecvSmsHint(Recv_CMTI, No);


}

void TextMsg_GHT::Deal_CMGL()
{
	int len = 0;
	BYTE *message = new BYTE[1024];
	for (int i=0; i<m_FrameBufLen; i++)
	{
		if (m_FrameBuf[i]==0x68 &&
			m_FrameBuf[i+2]==0x68 &&
			m_FrameBuf[i+1]!=0)
		{
			len = m_FrameBufLen-i;
			
			memset(message, 0, sizeof(message));
			memmove(message, &m_FrameBuf[i], len);
			break;
		}
	}

	int headLen = m_FrameBufLen - len;
	if (headLen > 6)
	{
		int start = -1;
		int end = -1;
		bool first = false;//�ҵ���һ������
		bool second = false;//�ҵ��ڶ�������
		bool finish = false;

		int No = m_FrameBuf[6];
		CString recvFlag = "";
		CString phoneNum = "";
		CString recvTime = "";

		for (int j=0; j<headLen; j++)
		{
			if (!first)
			{
				if (m_FrameBuf[j] == '"')
				{
					start = j;
					first = true;
				}
			}

			if (!second)
			{
				if (m_FrameBuf[j] == '"')
				{
					end = j;
					second = true;
				}
			}			

			if (first && second)
			{
				BYTE temp[100];
				memset(temp, 0, 100);
				bool flag = false;
				if (start!=-1 && end!=-1)
				{
					memcpy(temp, m_FrameBuf, end-start+1);
					if (recvFlag.IsEmpty())
					{
						recvFlag.Format("%s", temp);
						flag = true;
					}
					else if (phoneNum.IsEmpty())
					{
						recvFlag.Format("%s", temp);
						flag = true;
					}
					else if (recvTime.IsEmpty())
					{
						recvFlag.Format("%s", temp);
						flag = true;
					}

					if (flag)
					{
						first = false;
						second = false;
						start = -1;
						end = -1;
						continue;
					}

					break;
					
				}
				
			}
			
		}



		TM_Unit *pTmUnit = new TM_Unit();
		SYSTEMTIME t;
		::GetLocalTime(&t);

		CString time = "";
		time.Format("%d:%d:%d %d-%d-%d",t.wHour,t.wMinute,t.wSecond,
			t.wYear,t.wMonth,t.wDay);

		CString content = BufferToString(message, len);

		pTmUnit->SetIndex(No);
		pTmUnit->SetSendMsgType(true);
		pTmUnit->SetContent(content);
		pTmUnit->SetRecvTime(time);
		pTmUnit->SetRead(false);
		pTmUnit->SetPhoneNum(_T("δ֪����"));

		GetMsgTextDlg()->UpdateDlg(CMGL_waiting_OK, TRUE, pTmUnit);

		if (!phoneNum.IsEmpty())
		{
			CTmMessageDealModule *pTmMessageModule = NULL;
			std::map<CString, CTmMessageDealModule*>::iterator it = m_mapTmMessageDeal.find(phoneNum);

			if (it != m_mapTmMessageDeal.end()
				&& it->second != NULL)
			{
				pTmMessageModule = it->second;
			}
			else
			{
				pTmMessageModule = new CTmMessageDealModule(phoneNum, this);
				if (pTmMessageModule)
				{
					m_mapTmMessageDeal.insert(std::pair<CString, CTmMessageDealModule*>(pTmMessageModule->GetRemoteAddress(),
						pTmMessageModule));
				}
			}

			if (pTmMessageModule)
				pTmMessageModule->RecvMessage(message,len);

			delete[] message;
			message = NULL;
		}
		
	}
}

void TextMsg_GHT::Deal_CMGR()
{
	int len = 0;
	BYTE *message = new BYTE[1024];
	for (int i=0; i<m_FrameBufLen; i++)
	{
		if (m_FrameBuf[i]==0x68 &&
			m_FrameBuf[i+2]==0x68 &&
			m_FrameBuf[i+1]!=0)
		{
			len = m_FrameBufLen-i;

			memset(message, 0, sizeof(message));
			memmove(message, &m_FrameBuf[i], len);
			break;
		}
	}

	int headLen = m_FrameBufLen - len;
	if (headLen > 6)
	{
		int start = -1;
		int end = -1;
		bool first = false;//�ҵ���һ������
		bool second = false;//�ҵ��ڶ�������
		bool finish = false;

		int No = m_FrameBuf[6];
		CString recvFlag = "";
		CString phoneNum = "";
		CString recvTime = "";

		for (int j=0; j<headLen; j++)
		{
			if (!first)
			{
				if (m_FrameBuf[j] == '"')
				{
					start = j;
					first = true;
				}
			}

			if (!second)
			{
				if (m_FrameBuf[j] == '"')
				{
					end = j;
					second = true;
				}
			}			

			if (first && second)
			{
				BYTE temp[100];
				memset(temp, 0, 100);
				bool flag = false;
				if (start!=-1 && end!=-1)
				{
					memcpy(temp, m_FrameBuf, end-start+1);
					if (recvFlag.IsEmpty())
					{
						recvFlag.Format("%s", temp);
						flag = true;
					}
					else if (phoneNum.IsEmpty())
					{
						recvFlag.Format("%s", temp);
						flag = true;
					}
					else if (recvTime.IsEmpty())
					{
						recvFlag.Format("%s", temp);
						flag = true;
					}

					if (flag)
					{
						first = false;
						second = false;
						start = -1;
						end = -1;
						continue;
					}

					break;

				}

			}

		}



		TM_Unit *pTmUnit = new TM_Unit();
		SYSTEMTIME t;
		::GetLocalTime(&t);

		CString time = "";
		time.Format("%d:%d:%d %d-%d-%d",t.wHour,t.wMinute,t.wSecond,
			t.wYear,t.wMonth,t.wDay);

		CString content = BufferToString(message, len);

		pTmUnit->SetIndex(No);
		pTmUnit->SetSendMsgType(true);
		pTmUnit->SetContent(content);
		pTmUnit->SetRecvTime(time);
		pTmUnit->SetRead(false);
		pTmUnit->SetPhoneNum(_T("δ֪����"));

		GetMsgTextDlg()->UpdateDlg(CMGR_waiting_OK, TRUE, pTmUnit);

		if (!phoneNum.IsEmpty())
		{
			CTmMessageDealModule *pTmMessageModule = NULL;
			std::map<CString, CTmMessageDealModule*>::iterator it = m_mapTmMessageDeal.find(phoneNum);

			if (it != m_mapTmMessageDeal.end()
				&& it->second != NULL)
			{
				pTmMessageModule = it->second;
			}
			else
			{
				pTmMessageModule = new CTmMessageDealModule(phoneNum, this);
				if (pTmMessageModule)
				{
					m_mapTmMessageDeal.insert(std::pair<CString, CTmMessageDealModule*>(pTmMessageModule->GetRemoteAddress(),
						pTmMessageModule));
				}
			}

			if (pTmMessageModule)
				pTmMessageModule->RecvMessage(message,len);

			delete[] message;
			message = NULL;
		}

	}

}

void TextMsg_GHT::DealTmCommMessage_GHT_Init()
{
	//���ڶ˿ڳ�ʼ������
	int currentSendType = 0;
	CString recvInfo = "";
	BYTE tempBuf[1024];
	memset(tempBuf, 0, 1024);

	if (m_pMsgInitDlg)
	{
		int currentCmd = 0;
		if (m_pMsgInitDlg->m_CurrentSendMsg)
		{
			currentSendType = m_pMsgInitDlg->m_CurrentSendMsg->m_Type;
			if (m_RECV_Len >= 0)
			{
				int start_pos = 0;
				for (start_pos=0; start_pos<m_RECV_Len; start_pos++)
				{
					if ((char)m_RECV_BUF[start_pos] == 'O'
						&&(char)m_RECV_BUF[start_pos+1] == 'K')
					{
						int cmd = Invaild_cmd;

						switch(GetMsgInitDlg()->GetCmd())
						{
						case AT_waiting_OK:
							cmd = AT_OK_finish;
							break;
						case ATE0_sending:
							cmd = ATE0_OK_finish;
							break;
						case CGSN_waiting_OK:
							cmd = CGSN_OK_finish;
							break;
						case CGMR_waiting_OK:
							cmd = CGMR_OK_finish;
							break;
						case CIMI_waiting_OK:
							cmd = CIMI_OK_finish;
							break;
						case CMEE_sending:
							cmd = CMEE_OK_finish;
							break;
						case CPIN_waiting_OK:
							cmd = CPIN_OK_finish;
							break;
						case CREG_sending:
							cmd = CGREGa_OK_finish;
							break;
						case CREG_waiting_OK:
							cmd = CGREG_OK_finish;
							break;
						case CSQ_waiting_OK:
							cmd = CSQ_OK_finish;
							break;
						case CGREGa_sending:
							cmd = CGREGa_OK_finish;
							break;
						case CGREG_waiting_OK:
							cmd = CGREGa_OK_finish;
							break;
						case CGATT_waiting_OK:
							cmd = CGATT_OK_finish;
							break;
						case CLIP_sending:
							cmd = CLIP_OK_finish;
							break;
						case CNMI_sending:
							cmd = CNMI_OK_finish;
							break;
						case CMGF_sending:
							cmd = CMGF_OK_finish;
							break;
						case CSCA_waiting_OK:
							cmd = CSCA_OK_finish;
							break;
						}

						GetMsgInitDlg()->UpdateGrid(cmd, MsgInit_Recv, "OK", 2);

					}
					else if ((char)m_RECV_BUF[start_pos] == 'A'
						&&(char)m_RECV_BUF[start_pos+1] == 'T')
					{
						int len = m_RECV_Len - start_pos;
						if (len <= 0)
						{
							continue;
						}
						memmove(tempBuf, m_RECV_BUF, len);

						int a = GetMsgInitDlg()->GetCmd();
						if (GetMsgInitDlg()->GetCmd() == AT_sending)
						{
							recvInfo.Format("%s" ,CUdfGlobalFUNC::ConvertBytesToString(tempBuf,len));
							GetMsgInitDlg()->UpdateGrid(AT_waiting_OK, MsgInit_Recv, recvInfo, len);
						}

					}
					else if ((char)m_RECV_BUF[start_pos] == 'I'
						&&(char)m_RECV_BUF[start_pos+1] == 'M'
						&&(char)m_RECV_BUF[start_pos+2] == 'E'
						&&(char)m_RECV_BUF[start_pos+3] == 'I')
					{
						if (start_pos!=0)
						{
							int len = m_RECV_Len - start_pos + 1;
							if (len <= 0)
							{
								continue;
							}
							memmove(tempBuf, m_RECV_BUF, len);

							if (GetMsgInitDlg()->GetCmd() == CGSN_sending)
							{
								recvInfo.Format("%s" ,CUdfGlobalFUNC::ConvertBytesToString(tempBuf,len));
								GetMsgInitDlg()->UpdateGrid(CGSN_waiting_OK, MsgInit_Recv, recvInfo, len);
							}				
						}
					}
					else if ((char)m_RECV_BUF[start_pos] == '+')
					{
						CString strCMD = "";
						strCMD.Format("$%c%c%c%c",
							(char)m_RECV_BUF[start_pos + 1],
							(char)m_RECV_BUF[start_pos + 2],
							(char)m_RECV_BUF[start_pos + 3],
							(char)m_RECV_BUF[start_pos + 4]);

						if ("CGMR" == strCMD)
						{
							int len = m_RECV_Len - start_pos + 1;
							if (len <= 0)
							{
								continue;
							}
							memmove(tempBuf, m_RECV_BUF, len);

							if (GetMsgInitDlg()->GetCmd() == CGMR_sending)
							{
								recvInfo.Format("%s" ,CUdfGlobalFUNC::ConvertBytesToString(tempBuf,len));
								GetMsgInitDlg()->UpdateGrid(CGMR_waiting_OK, MsgInit_Recv, recvInfo, len);

							}

						}
						else if ("CIMI" == strCMD)
						{
							int len = m_RECV_Len - start_pos + 1;
							if (len <= 0)
							{
								continue;
							}
							memmove(tempBuf, m_RECV_BUF, len);

							if (GetMsgInitDlg()->GetCmd() == CIMI_sending)
							{
								recvInfo.Format("%s" ,CUdfGlobalFUNC::ConvertBytesToString(tempBuf,len));
								GetMsgInitDlg()->UpdateGrid(CIMI_waiting_OK, MsgInit_Recv, recvInfo, len);

							}

						}
						else if ("CPIN" == strCMD)
						{
							int len = m_RECV_Len - start_pos + 1;
							if (len <= 0)
							{
								continue;
							}
							memmove(tempBuf, m_RECV_BUF, len);
							if (GetMsgInitDlg()->GetCmd() == CPIN_sending)
							{
								recvInfo.Format("%s" ,CUdfGlobalFUNC::ConvertBytesToString(tempBuf,len));
								GetMsgInitDlg()->UpdateGrid(CPIN_waiting_OK, MsgInit_Recv, recvInfo, len);
							}

						}
						else if ("CREG" == strCMD)
						{
							if ((char)m_RECV_BUF[start_pos + 5] == ':')
							{
								int len = m_RECV_Len - start_pos + 1;
								if (len <= 0)
								{
									continue;
								}
								memmove(tempBuf, m_RECV_BUF, len);

								if (GetMsgInitDlg()->GetCmd() == CREG_sending)
								{
									recvInfo.Format("%s" ,CUdfGlobalFUNC::ConvertBytesToString(tempBuf,len));
									GetMsgInitDlg()->UpdateGrid(CREG_waiting_OK, MsgInit_Recv, recvInfo, len);

								}

							}
							else if ((char)m_RECV_BUF[start_pos + 5] == 'G')
							{
								int len = m_RECV_Len - start_pos + 1;
								if (len <= 0)
								{
									continue;
								}
								memmove(tempBuf, m_RECV_BUF, len);
								if (GetMsgInitDlg()->GetCmd() == CGREG_sending)
								{
									recvInfo.Format("%s" ,CUdfGlobalFUNC::ConvertBytesToString(tempBuf,len));
									GetMsgInitDlg()->UpdateGrid(CREG_waiting_OK, MsgInit_Recv, recvInfo, len);

								}

							}
						}
						else if ("CSQ:" == strCMD)
						{
							int len = m_RECV_Len - start_pos + 1;
							if (len <= 0)
							{
								continue;
							}
							memmove(tempBuf, m_RECV_BUF, len);

							if (GetMsgInitDlg()->GetCmd() == CSQ_sending)
							{
								recvInfo.Format("%s" ,CUdfGlobalFUNC::ConvertBytesToString(tempBuf,len));
								GetMsgInitDlg()->UpdateGrid(CSQ_waiting_OK, MsgInit_Recv, recvInfo, len);

							}

						}
						else if ("CGAT" == strCMD)
						{
							int len = m_RECV_Len - start_pos + 1;
							if (len <= 0)
							{
								continue;
							}
							memmove(tempBuf, m_RECV_BUF, len);
							if (GetMsgInitDlg()->GetCmd() == CGATT_sending)
							{
								recvInfo.Format("%s" ,CUdfGlobalFUNC::ConvertBytesToString(tempBuf,len));
								GetMsgInitDlg()->UpdateGrid(CGATT_OK_finish, MsgInit_Recv, recvInfo, len);

							}

						}
						else if ("CSCA" == strCMD)
						{
							int len = m_RECV_Len - start_pos + 1;
							if (len <= 0)
							{
								continue;
							}
							memmove(tempBuf, m_RECV_BUF, len);
							if (GetMsgInitDlg()->GetCmd() == CSCA_sending)
							{
								recvInfo.Format("%s" ,CUdfGlobalFUNC::ConvertBytesToString(tempBuf,len));
								GetMsgInitDlg()->UpdateGrid(CSCA_OK_finish, MsgInit_Recv, recvInfo, len);
							}	
						}
					}
				}
			}
		}

	}

}


//----------����--------------
TextMsg_YT::TextMsg_YT()
{

}

TextMsg_YT::~TextMsg_YT()
{

}

void TextMsg_YT::RecvMessage(BYTE* buf,int Len)
{

}

bool TextMsg_YT::SendMessage(BYTE* buf,int &len)
{
	if (buf != NULL)
	{   
		if (m_bSendBefore == false || 
			(m_bSendBefore == true && 
			m_CheckSendTimer.WaitTimerState() == true))
		{
			//��ȡ��Ҫ���͵ı���
			PsendObject pSndObj = GetOneSendObject();
			if (pSndObj  != NULL)
			{
				memmove(buf,pSndObj->sendbuf,pSndObj->sendLen);
				len = pSndObj->sendLen;

				CString strLog="";
				strLog.Format("��,����ͨѶ:���ڵ���,����:%d,����:%s",pSndObj->sendLen,
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

void TextMsg_YT::DealTmCommMessage_YT()
{

}


void TextMsg_YT::SetCommInfo(COM_INFO comm_info)
{
	m_commInfo = comm_info;
}

void TextMsg_YT::SetStCode(int StCode)
{
	m_nStCode = StCode;
	
}


COM_INFO TextMsg_YT::GetCommInfo()
{
	return m_commInfo;
}

int TextMsg_YT::GetStCode()
{
	return m_nStCode;
}