#include "StdAfx.h"
#include "SysDataTransSyq.h"
#include "UdfGlobalFUNC.h"
#include "CommServer.h"
#include "CMS.h"
#include "MainFrm.h"
#include "RtuStation.h"

CSysDataTrans_Syq::CSysDataTrans_Syq(void)
{
}

CSysDataTrans_Syq::~CSysDataTrans_Syq(void)
{
}
//������ת��Ϊ�ַ�����Ϣ
CString CSysDataTrans_Syq::BufferToString(BYTE* buffer,int Len)
{
	if (!buffer  || Len <= 0)
		return "";

	CString strResult = "";
	for (int idx = 0 ; idx < Len; idx ++)
	{
		CString strTemp = "";
		strTemp.Format("%02X ",buffer[idx]);
		strResult += strTemp;
	}
	return strResult;
}


//��ȡң���ն����Բ�������
//�����Բ��������Ͽ��Խ�����Ӧ�ĵ������Թ���
CRtuPropertyDlg* CSysDataTrans_Syq::GetRtuPropertyDlg()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	if (pMainFrame)
		return pMainFrame->GetRtuPropertyDlg();
	return NULL;
}

void CSysDataTrans_Syq::AddRecvLog(BYTE* buffer,int len, CSYQFrameFormat* syqFrame/* =NULL */)
{
	if (!buffer  || len <=0) return;

	CString strBuffer = BufferToString(buffer,len);
	CString strLogBuffer ="";

	CSYQFrameFormat syqff;
	if (syqFrame == NULL)
	{
		if (!CUdfGlobalFUNC::SYQ_to_APPFRAME(buffer,len,&syqff))  
			return;

		syqFrame = &syqff;
	}

	if (syqFrame != NULL)
	{
		if (syqFrame->m_pStation == NULL)
		{
			CCMSApp* pApp = (CCMSApp*)AfxGetApp();
			CRealTimeDB* pRealDb = pApp->m_pReadDB;
			if (!pRealDb)	return ;

			CRtuStation* pSub =  pRealDb->SearchRtuStation(syqFrame->m_stCode, SYQYC_Comm_Protocol);
			syqFrame->m_pStation = pSub;
		}

		CString strBuffer = BufferToString(buffer,len);
		CString strLogBuffer ="";

		strLogBuffer.Format("��,%-2d��%-2d��%-2d��%-2d��,DIR=%s,NO=%3d,FUN= %2x,Len=%-5d,����=%s",
			syqFrame->m_stRecvTime.wMonth,
			syqFrame->m_stRecvTime.wDay,
			syqFrame->m_stRecvTime.wHour,
			syqFrame->m_stRecvTime.wMinute,
			_T("����"),
			syqFrame->messageno,
			syqFrame->m_FunType,
			len,
			strBuffer);

		AddLogMessage(syqFrame->m_pStation ,strLogBuffer, Normal_Log_Message);
	}

}


//�����־��Ϣ��ң���ն˵����Բ�������
void CSysDataTrans_Syq::AddLogMessage(CRtuStation* pRTU,CString strLogMessage, _Log_level_type leveType /* =Normal_Log_Message */, PSYSTEMTIME pTime /* = NULL */)
{

	if (pRTU)
		pRTU->InsertLogMessage(strLogMessage,leveType,pTime);

	CRtuPropertyDlg* pWnd = GetRtuPropertyDlg();
	if (pWnd == NULL  ||
		pWnd->GetRtuStation() == NULL ||
		pWnd->GetRtuStation() != pRTU)
		return;

	pWnd->AddLog(strLogMessage,pTime,leveType);
}
//���������Ϣ��ң���ն˵��Ա�������ʾ����
void CSysDataTrans_Syq::AddDataMessage(CRtuStation* pRTU,CString strDataMessage,PSYSTEMTIME pTime/* =NULL */)
{

	if (GetRtuPropertyDlg() != NULL && 
		GetRtuPropertyDlg()->GetSafeHwnd())
	{
		if (GetRtuPropertyDlg()->GetRtuStation() != pRTU) return ;

		if (GetRtuPropertyDlg()->GetTab_AttrDataShow() != NULL && 
			GetRtuPropertyDlg()->GetTab_AttrDataShow()->GetSafeHwnd())
			GetRtuPropertyDlg()->GetTab_AttrDataShow()->AddMessageToWindow(strDataMessage,pTime);
	}
}

//����ң���ն˵�ͨ��״̬
//�ص�:�����б���Ϣ�����ͨ��״̬�ı���־��Ϣ
void CSysDataTrans_Syq::UpdateRtuCommState(CRtuStation* pSub)
{
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	CMainFrame* pMainFrame = (CMainFrame*)pApp->m_pMainWnd;

	if (pMainFrame && pMainFrame->m_pRightView && pMainFrame->m_pRightView->GetSafeHwnd())
		pMainFrame->m_pRightView->UpdateCommState(pSub);
}

void CSysDataTrans_Syq::UpdateRtuCommInfo(CRtuStation* pSub)
{
	if (GetRtuPropertyDlg() == NULL ||
		GetRtuPropertyDlg()->GetRtuStation() == NULL ||
		GetRtuPropertyDlg()->GetRtuStation() != pSub)
		return;

	if (GetRtuPropertyDlg()->GetTab_AttrBasicInfo()->GetSafeHwnd())
		GetRtuPropertyDlg()->GetTab_AttrBasicInfo()->UpdateCommInfo();
}


void CSysDataTrans_Syq::UpdateAttribFrame(CRtuStation* pRTU, int attrType)
{
	CRtuPropertyDlg* pWnd = GetRtuPropertyDlg();
	if (pWnd == NULL  ||
		pWnd->GetRtuStation() == NULL ||
		pWnd->GetRtuStation() != pRTU)
		return;

	switch (attrType)
	{
	case 0xff:
		{
			pWnd->Update();
		}
		break;
	case syq_read_address1:
	case syq_read_address2:
	case syq_read_address3:
	case syq_read_address4:	
	case syq_config_address1:
	case syq_config_address2:
	case syq_config_address3:
	case syq_config_address4:
		{
			CRtuAttrSyqcfg1 *pTab = pWnd->GetRtuAttrSyqProSet()->GetSyqTab_Config1();
			if ( pTab != NULL || pTab->GetSafeHwnd())
			{
				pTab->Updatedata(attrType);
				//	pTab->Update();
				pTab->UpdateDlgMessage(attrType,TRUE);
				//	m_pCurrCallOP = scsw_no_call_operation;
			}
		}
		break;
	case syq_config2_Read:
	case syq_config2_Write:
		{
			CRtuAttrSyqcfg2 *pTab = pWnd->GetRtuAttrSyqProSet()->GetSyqTab_Config2();
			if ( pTab != NULL || pTab->GetSafeHwnd())
			{
				pTab->Update();
				//	pTab->Update();
				pTab->UpdateDlgMessage(attrType,TRUE);
				//	m_pCurrCallOP = scsw_no_call_operation;
			}
		}
		break;
	case syq_config3_Read:
	case syq_config3_Write:
		{
			CRtuAttrSyqcfg3 *pTab = pWnd->GetRtuAttrSyqProSet()->GetSyqTab_Config3();
			if ( pTab != NULL || pTab->GetSafeHwnd())
			{
				pTab->Update();
				pTab->UpdateDlgMessage(attrType,TRUE);
				//	m_pCurrCallOP = scsw_no_call_operation;
			}
		}
		break;
	case syq_init_memory:
		{
			CRtuAttrSyqcfg3 *pTab = pWnd->GetRtuAttrSyqProSet()->GetSyqTab_Config3();
			if ( pTab != NULL || pTab->GetSafeHwnd())
			{
				//pTab->Update();
				pTab->UpdateDlgMessage(attrType,TRUE);
				//	m_pCurrCallOP = scsw_no_call_operation;
			}
		}
		break;
	case syq_check_time:
		{
			CRtuAttrSyqcfg3 *pTab = pWnd->GetRtuAttrSyqProSet()->GetSyqTab_Config3();
			if ( pTab != NULL || pTab->GetSafeHwnd())
			{
				//pTab->Update();
				pTab->UpdateDlgMessage(attrType,TRUE);
				//	m_pCurrCallOP = scsw_no_call_operation;
			}
		}
		break;
	case syq_call_data://��ѯָ��Ҫ��
		{
			CRtuAttrSyqCallData2 *pTab = pWnd->GetRtuAttrSyqProSet()->GetSyqTab_Calldata2();
			if ( pTab != NULL || pTab->GetSafeHwnd())
			{
				//pTab->Update();
				pTab->UpdateDlgMessage(attrType,TRUE);
				//	m_pCurrCallOP = scsw_no_call_operation;
			}
		}
		break;
	case syq_call_data_bytime://��ѯָ��ʱ���ָ��Ҫ��
		{
			CRtuAttrSyqCalldata *pTab = pWnd->GetRtuAttrSyqProSet()->GetSyqTab_Calldata();
			if ( pTab != NULL || pTab->GetSafeHwnd())
			{
				//pTab->Update();
				pTab->UpdateDlgMessage(attrType,TRUE);
				//	m_pCurrCallOP = scsw_no_call_operation;
			}
		}
		break;
	case syq_call_data1://��ѯʵʱ����
	case syq_call_data2://��ѯң��վ�˹�����
		{
			CRtuAttrSyqCallData1 *pTab = pWnd->GetRtuAttrSyqProSet()->GetSyqTab_Calldata1();
			if ( pTab != NULL || pTab->GetSafeHwnd())
			{
				//pTab->Update();
				pTab->UpdateDlgMessage(attrType,TRUE);
				//	m_pCurrCallOP = scsw_no_call_operation;
			}
		}
		break;
	case syq_call_state://��ѯң��վ�˹�����
		{
			CRtuAttrSyqCallState *pTab = pWnd->GetRtuAttrSyqProSet()->GetSyqTab_CalldState();
			if ( pTab != NULL || pTab->GetSafeHwnd())
			{
				//pTab->Update();
				pTab->UpdateDlgMessage(attrType,TRUE);
				//	m_pCurrCallOP = scsw_no_call_operation;
			}
		}
		break;
	}

}

//���һ����Ҫ�����������Ϣ
void CSysDataTrans_Syq::InsertOneSaveData(COneData_INFO* pData)	
{
	if (!pData) return;

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	if (pApp && pApp->m_pRtuDatasave != NULL)
		pApp->m_pRtuDatasave->InsertOneData(pData);
}

///�������Ľ���
void CSysDataTrans_Syq::Dispatcher(CSYQFrameFormat* psyqFrame)
{
	if(psyqFrame == NULL)
		return;

	switch(psyqFrame->m_FunType)
	{
	case 0x30://���Ա� ����ʵʱ����
		Process_SYQ_30H(psyqFrame);
		break;
	case 0x31://����ʱ��
		Process_SYQ_31H(psyqFrame);
		break;
	case  0x32://��ʱ��
		Process_SYQ_32H(psyqFrame);
		break;
	case 0x33://
		Process_SYQ_33H(psyqFrame);
		break;
	case 0x34://
		Process_SYQ_34H(psyqFrame);
		break;
	case  0x35://
		Process_SYQ_35H(psyqFrame);
		break;

	case 0x36://
		Process_SYQ_36H(psyqFrame);
		break;
	case 0x37://
		Process_SYQ_37H(psyqFrame);
		break;
	case  0x38://
		Process_SYQ_38H(psyqFrame);
		break;
	case 0x39://
		Process_SYQ_39H(psyqFrame);
		break;
	case 0x40://
		Process_SYQ_40H(psyqFrame);
		break;
	case  0x41://��ʱ��
		Process_SYQ_41H(psyqFrame);
		break;
	case 0x42://
		Process_SYQ_42H(psyqFrame);
		break;
	case  0x43://��ʱ��
		Process_SYQ_43H(psyqFrame);
		break;
	case 0x44://
		Process_SYQ_44H(psyqFrame);
		break;
	case 0x45://��ʱ��
		Process_SYQ_45H(psyqFrame);
		break;
	case 0x46://�����޸�
		Process_SYQ_46H(psyqFrame);
		break;



	}

}

void CSysDataTrans_Syq::GetPropertyString(unsigned char *buf, int buflen, CString& str,int& len, int& datalen)
{
	int temp,temp1 ;
	for(int i = 0; i<6; i++)//�ҵ���һ������ĸ��λ��
	{
		temp = (int)buf[i];
		if(temp<65|| temp >90)
		{
			len = i;
			break;
		}
	}

	if((len == 1)&&buf[0] == 'P')   // P01 �� P03
	{
		//memcpy(str,buf,3);
		len = 3;
		for (int i= 0; i<len; i++)
		{
			str = str+(char)buf[i];
		}

		if((buflen-len)>24)//���ݳ����������24���ֽ�
			temp1 = 24;
		else
			temp1 = buflen -len;

		for(int j = 0; j<temp1; j++)
		{
			temp = (int)buf[len+j];
			if(temp>=65 && temp<=90)//�ҵ���ĸλ������
			{
				datalen = j;
				break;
			}

			if(j == (temp1-1))//�ҵ�ĩβ�ˣ�
				datalen =j+1;
		}

	}
	else if((len == 2)&&buf[0] == 'P'&&buf[1]=='N')   // PN01 �� PN03
	{
		//memcpy(str,buf,3);
		len = 4;
		for (int i= 0; i<len; i++)
		{
			str = str+(char)buf[i];
		}

		if((buflen-len)>24)
			temp1 = 24;
		else
			temp1 = buflen -len;

		for(int j = 0; j<temp1; j++)
		{
			temp = (int)buf[len+j];
			if(temp>=65 && temp<=90)
			{
				datalen = j;
				break;
			}

			if(j == (temp1-1))
				datalen =j+1;
		}

	}
	else if((len == 3)&&buf[0] == 'D'&&buf[1]=='R'&&buf[2]!='P'&&buf[2]!='Z')   // DRxnn
	{
		//memcpy(str,buf,3);
		len = 5;
		for (int i= 0; i<len; i++)
		{
			str = str+(char)buf[i];
		}

		datalen = 0;

	}
	else
	{
		for (int i= 0; i<len; i++)
		{
			str = str+(char)buf[i];
		}
		if((buflen-len)>24)
			temp1 = 24;
		else
			temp1 = buflen -len;

		for(int j = 0; j<temp1; j++)
		{
			temp = (int)buf[len+j];
			if(temp>=65 && temp<=90)
			{
				datalen = j;
				break;
			}

			if(j == (temp1-1))
				datalen =j+1;
		}
	}




}

void CSysDataTrans_Syq::GetPropertyStringList(unsigned char *buf, int buflen,int& len, int stcode)//��ȡ�ַ������� 
{
	CString  tempstr ="";
	CString  savestr ="";
	m_cs_yaosu.Lock();
	if(m_YaosuList.size()>0)
		m_YaosuList.clear();
	m_cs_yaosu.Unlock();

	bool bfind = false;
	int findlen = 0;

	for(int i = 0; i<buflen; i++)//�ҵ���һ������ĸ��λ��
	{
		int temp = (int)buf[i];
		int temp1 = 0;
		if(temp>= 65 &&temp <= 90)//Between A-Z
		{
			findlen ++;
			len ++;
			tempstr = tempstr + (char)buf[i];

			temp1 = (int)buf[i+1];
			if(tempstr == "P"&& ((temp1<65|| temp >190)) )//��ĸ��ʱ����
			{				
				tempstr = tempstr +(char)buf[i+1]+(char)buf[i+2];
				i = i + 2;
				len = len +2;
				findlen = findlen +2;
			}
			else if(tempstr == "PN"&& ((temp1<65|| temp >190)) )//��ĸ��ʱ����
			{
				tempstr = tempstr +(char)buf[i+1]+(char)buf[i+2];
				i = i + 2;
				len = len +2;
				findlen = findlen +2;
			}

			std::map<CString, SYQ_YS_STRUCT>::iterator it_map;
			it_map = SYQ_TypeMap::GetInstance()->m_SYQTypeMap.find(tempstr);
			if (it_map != SYQ_TypeMap::GetInstance()->m_SYQTypeMap.end())
			{
				bfind = true;
				savestr = tempstr;
				continue;
			}
			else
			{
				if(bfind)
				{
					m_cs_yaosu.Lock();
					if(savestr != "")
						m_YaosuList.push_back(savestr);
					m_cs_yaosu.Unlock();
					tempstr = "";
					bfind = false;
					findlen = 0;
					i=i-1;
				}
				else
				{
					if(findlen>5)//����5����ĸ�޷��ҵ���ӦҪ�أ�ȡ������
					{
						len = len - findlen;

						CCMSApp* pApp = (CCMSApp*)AfxGetApp();
						CRealTimeDB* pRealDb = pApp->m_pReadDB;
						if (!pRealDb)			 return ;
						CRtuStation* pSub =  pRealDb->SearchRtuStation(stcode, SYQYC_Comm_Protocol);

						if (pSub != NULL)
						{
							CString strLogBuffer= "";
							strLogBuffer.Format("�޷��ҵ���ӦҪ�أ�%s,", tempstr);
							AddLogMessage(pSub,strLogBuffer,Error_Log_message);
						}

						break;
					}
					else
						continue;

				}

			}

		}
		else
		{
			if(bfind)
			{
				m_cs_yaosu.Lock();
				if(savestr != "")
					m_YaosuList.push_back(savestr);
				m_cs_yaosu.Unlock();
				tempstr = "";
				len --;
				break;
		}

		}
	}


}

void CSysDataTrans_Syq::BuildSendBuffer(CSYQFrameFormat* packetFrame,BYTE* sBuf,BYTE& len,int mlen)
{
	if (!packetFrame) return;
	if (!sBuf)        return;

	BYTE temp[4];
	memset(temp,0,sizeof(temp));
	CUdfGlobalFUNC::ToBCD(packetFrame->m_stCode,temp,4);
	sBuf[0]		    = 0x01;
	sBuf[1] = 0;
	sBuf[2] = temp[3];
	sBuf[3] = temp[2];
	sBuf[4] = temp[1];
	sBuf[5] = temp[0];
	for(int i =0; i<5;i++)
		sBuf[6+i] = 0;
	sBuf[11]		=  packetFrame->m_CenterCode%256;
	sBuf[12]		=  packetFrame->m_CenterCode/256;

	sBuf[13]       = packetFrame->m_iPassword%256;//����
	sBuf[14]       = packetFrame->m_iPassword/256;//����
	sBuf[15]       = 0;//����
	sBuf[16]       = 0;//����

	sBuf[17]      = packetFrame->m_FunType;
	sBuf[18]      = 0x00;

	sBuf[19]      =  mlen %256;
	sBuf[20]      =  mlen/256;  //���ĳ���

	sBuf[21]     = 0x02; //STX

	for (int i= 0; i< mlen; i++)
	{
		sBuf[22 + i] = packetFrame->_asdu.data[i];
	}
	sBuf[22 + mlen] = 0x03;//etx

	WORD CRC = CUdfGlobalFUNC::CreateCRC_SYQ(sBuf, mlen +23);
	sBuf[23 + mlen] =  HIBYTE(CRC);
	sBuf[24 + mlen] =  LOBYTE(CRC) ;

	len = 25+mlen;
}

//��ͨ����·���ͱ�������
int CSysDataTrans_Syq::SendRtuLinkFrame(BYTE* sBuf,int len,BYTE CmInterface/* =Serial_CommInterface */,PNET_INFO pToNetInfo /* = NULL */)
{
	if (!sBuf  || len <=0 ) return 0;
	int sendLen = len;

	//��ң���ն��·�����
	if (m_pSubStation != NULL)
	{
		m_cs_SendData.Lock();

		//����ͨ��ֻ�ղ��� 2012-07-13 �޸�
		if (m_pSubStation->GetCmType() != SC_CommChannel)
			if (m_pSubStation->SendData(sBuf,len))
				AddSendLog(sBuf,len,m_pSubStation->GetSYQStCode());
		m_cs_SendData.Unlock();
	}

	return sendLen;
}


void CSysDataTrans_Syq::AddSendLog(BYTE* buffer,int len, DWORD stcode)
{
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	CRealTimeDB* pRealDb = pApp->m_pReadDB;
	if (!pRealDb)			 return ;
	if (!buffer  || len <=0) return;

	CRtuStation* pSub =  pRealDb->SearchRtuStation(stcode, SYQYC_Comm_Protocol);

	CString strBuffer = BufferToString(buffer,len);

	if (pSub != NULL)
	{
		CString strLogBuffer= "";
		strLogBuffer.Format("��,DIR=%s,Len=%-3d,����=%s",_T("����"), len, strBuffer);
		AddLogMessage(pSub,strLogBuffer,Normal_Log_Message);
	}
}

void CSysDataTrans_Syq::Process_SYQ_30H(CSYQFrameFormat* psyqFrame)
{
	psyqFrame->m_DataTimeSend.year = 2000+CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[3]);
	psyqFrame->m_DataTimeSend.month = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[4]);
	psyqFrame->m_DataTimeSend.day = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[5]);
	psyqFrame->m_DataTimeSend.hour = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[6]);
	psyqFrame->m_DataTimeSend.min =CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[7]);
	psyqFrame->m_DataTimeSend.second = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[8]);

	psyqFrame->m_DataTimeGet.month = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[12]);
	psyqFrame->m_DataTimeGet.day = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[13]);
	psyqFrame->m_DataTimeGet.hour = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[14]);
	psyqFrame->m_DataTimeGet.min = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[15]);


	CString strTemp = "";

	strTemp.Format("ң��վ��:%05d ���Ա�����ʱ�䣺%04d-%02d-%02d %02d:%02d:%02d,�ɼ�ʱ�䣺%02d-%02d %02d:%02d\r\n",
		psyqFrame->m_stCode,
		psyqFrame->m_DataTimeSend.year,
		psyqFrame->m_DataTimeSend.month,
		psyqFrame->m_DataTimeSend.day,
		psyqFrame->m_DataTimeSend.hour,
		psyqFrame->m_DataTimeSend.min,
		psyqFrame->m_DataTimeSend.second,
		psyqFrame->m_DataTimeGet.month,
		psyqFrame->m_DataTimeGet.day,
		psyqFrame->m_DataTimeGet.hour,
		psyqFrame->m_DataTimeGet.min);
	//////////////////////////////////////////////////////////////////////////
	//data δ����
	CString str; int len =0,datalen =0; 
	int tempmsglen = psyqFrame->nMsgLen - 16;

	for(int i = 0; i<(psyqFrame->nMsgLen - 16);)
	{
		GetPropertyString(&psyqFrame->_asdu.data[16+i],tempmsglen,str,len,datalen);

		std::map<CString, SYQ_YS_STRUCT>::iterator it_map;
		it_map = SYQ_TypeMap::GetInstance()->m_SYQTypeMap.find(str);
		if (it_map != SYQ_TypeMap::GetInstance()->m_SYQTypeMap.end())
		{
			if(it_map->second.type == 2)//FLOAT
			{
				if(it_map->second.ename != "DRP"&&it_map->second.ename != "DRZ")
				{
					if(it_map->second.ename != "DT")
					{
						strTemp = strTemp + it_map->second.name +":";

						for(int j = 0;j<it_map->second.datalen1;j++)
							strTemp = strTemp +(char)psyqFrame->_asdu.data[16+i+len+j];//����

						//strTemp = strTemp+".";//С����

						for(int j = 0;j<it_map->second.datalen2;j++)
							strTemp = strTemp +(char)psyqFrame->_asdu.data[16+i+len+it_map->second.datalen1+j];//С��

						strTemp = strTemp +it_map->second.Unit + " ";//��λ	
					}
				}
				else
				{
					if(it_map->second.ename == "DRP")
					{
						strTemp = strTemp + it_map->second.name +":";

						for(int j = 0;j<it_map->second.datalen1;j++)
						{
							CString str1 ="";
							str1.Format("%f",((int)psyqFrame->_asdu.data[16+i+len+j])*0.1);
							strTemp = strTemp +str1;
							strTemp = strTemp +it_map->second.Unit+" ";//��λ

							if(j == 6)
								strTemp = strTemp +"\r\n";
						}
					}
					else if(it_map->second.ename == "DRZ")
					{
						strTemp = strTemp + it_map->second.name +":";

						for(int j = 0;j<it_map->second.datalen1;j++)
						{
							CString str1 ="";
							int t1 = psyqFrame->_asdu.data[16+i+len+j]+psyqFrame->_asdu.data[16+i+len+j+1]*256;
							float f1 = t1*0.01;
							str1.Format("%f",f1);
							strTemp = strTemp +str1;
							strTemp = strTemp +it_map->second.Unit+" ";//��λ

							j++;

							if(j == 12)
								strTemp = strTemp +"\r\n";
						}
					}
					strTemp = strTemp +"\r\n";
				}

			}

		}

		str ="";
		tempmsglen = tempmsglen - len - datalen;
		i = i+len+datalen;
		len = 0; datalen = 0;
	}

	////////////////////

	SYSTEMTIME pt ;
	pt.wYear = psyqFrame->m_DataTimeSend.year;
	pt.wMonth = psyqFrame->m_DataTimeSend.month;
	pt.wDay = psyqFrame->m_DataTimeSend.day;
	pt.wHour = psyqFrame->m_DataTimeSend.hour;
	pt.wMinute = psyqFrame->m_DataTimeSend.min;
	pt.wMilliseconds = psyqFrame->m_DataTimeSend.second*1000;


	psyqFrame->m_pStation->InsertDataMessage(strTemp,&pt);//���뻺����У�
	AddDataMessage(psyqFrame->m_pStation, strTemp, &pt);//��ʾ������

}

void CSysDataTrans_Syq::Process_SYQ_31H(CSYQFrameFormat* psyqFrame)
{
	psyqFrame->m_DataTimeSend.year = 2000+CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[3]);
	psyqFrame->m_DataTimeSend.month = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[4]);
	psyqFrame->m_DataTimeSend.day = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[5]);
	psyqFrame->m_DataTimeSend.hour = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[6]);
	psyqFrame->m_DataTimeSend.min =CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[7]);
	psyqFrame->m_DataTimeSend.second = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[8]);

	psyqFrame->m_DataTimeGet.month = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[13]);//������������������
	psyqFrame->m_DataTimeGet.day = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[14]);
	psyqFrame->m_DataTimeGet.hour = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[15]);
	psyqFrame->m_DataTimeGet.min = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[16]);


	CString tempstr = "";//��ѹ����+ʱ�䲽��
	CString str1 = "";
	//	CString pointstr =".";
	//	tempstr = "��ѹ����:" + (char)psyqFrame->_asdu.data[19]+(char)psyqFrame->_asdu.data[20]+(char)psyqFrame->_asdu.data[21]+(char)psyqFrame->_asdu.data[22]+"."+(char)psyqFrame->_asdu.data[23]+(char)psyqFrame->_asdu.data[24];
	tempstr = "��ѹ����:";// + (char)psyqFrame->_asdu.data[19]+(char)psyqFrame->_asdu.data[20]+(char)psyqFrame->_asdu.data[21]+(char)psyqFrame->_asdu.data[22]+(char)psyqFrame->_asdu.data[23];

	for(int i = 0;i<5;i++)
		tempstr = tempstr +(char)psyqFrame->_asdu.data[19+i];
	tempstr = tempstr +"���� ";

	std::map<CString, SYQ_YS_STRUCT>::iterator it_map;
	for(int i = 0;i<3;i++)
		str1 = str1 +(char)psyqFrame->_asdu.data[24+i];
	it_map = SYQ_TypeMap::GetInstance()->m_SYQTypeMap.find(str1);
	if (it_map != SYQ_TypeMap::GetInstance()->m_SYQTypeMap.end())
	{
		str1 = "";
		str1 = it_map->second.name +(char)psyqFrame->_asdu.data[27]+(char)psyqFrame->_asdu.data[28]+it_map->second.Unit;
	}

	tempstr = tempstr + str1;
	CString strTemp = "";

	strTemp.Format("ң��վ��:%05d ����ʱ��ˮ����Ϣ������ʱ�䣺%04d-%02d-%02d %02d:%02d:%02d,�ɼ�ʱ�䣺%02d-%02d %02d:%02d\r\n",
		psyqFrame->m_stCode,
		psyqFrame->m_DataTimeSend.year,
		psyqFrame->m_DataTimeSend.month,
		psyqFrame->m_DataTimeSend.day,
		psyqFrame->m_DataTimeSend.hour,
		psyqFrame->m_DataTimeSend.min,
		psyqFrame->m_DataTimeSend.second,
		psyqFrame->m_DataTimeGet.month,
		psyqFrame->m_DataTimeGet.day,
		psyqFrame->m_DataTimeGet.hour,
		psyqFrame->m_DataTimeGet.min);

	strTemp = strTemp +tempstr +"\r\n";

	//////////////////////////////////////////////////////////////////////////
	CString str; int len =0; 
	int tempmsglen = psyqFrame->nMsgLen - 29;
	GetPropertyStringList(&psyqFrame->_asdu.data[29],tempmsglen,len,psyqFrame->m_stCode);

	std::list<CString>::iterator its;
	//	std::map<CString, SYQ_YS_STRUCT>::iterator it_map;
	int datagrouplen = 0;
	int point = 0;
	if(m_YaosuList.size()>0)
	{
		for(its = m_YaosuList.begin();its != m_YaosuList.end();its++)
		{
			it_map = SYQ_TypeMap::GetInstance()->m_SYQTypeMap.find((*its));
			if (it_map != SYQ_TypeMap::GetInstance()->m_SYQTypeMap.end())
				datagrouplen = datagrouplen + it_map->second.datalen1 + it_map->second.datalen2;

		}

		int groupno = (tempmsglen - len)/datagrouplen;

		for(int i = 0; i<groupno; i++)
		{
			for(its = m_YaosuList.begin();its != m_YaosuList.end();its++)
			{
				it_map = SYQ_TypeMap::GetInstance()->m_SYQTypeMap.find((*its));
				strTemp = strTemp + it_map->second.name;
				CString str1 = "";
				str1.Format("��%d������:",(i+1));
				strTemp = strTemp +str1;

				for(int j = 0;j<it_map->second.datalen1;j++)
					strTemp = strTemp +(char)psyqFrame->_asdu.data[29+len+j+ point];//����

				//strTemp = strTemp+".";//С����

				for(int j = 0;j<it_map->second.datalen2;j++)
					strTemp = strTemp +(char)psyqFrame->_asdu.data[29+len+it_map->second.datalen1+j+point];//С��

				strTemp = strTemp +it_map->second.Unit +" ";//��λ

				point = point + it_map->second.datalen1 + it_map->second.datalen2;
			}
		}

	}

	////////////////////

	SYSTEMTIME pt ;
	pt.wYear = psyqFrame->m_DataTimeSend.year;
	pt.wMonth = psyqFrame->m_DataTimeSend.month;
	pt.wDay = psyqFrame->m_DataTimeSend.day;
	pt.wHour = psyqFrame->m_DataTimeSend.hour;
	pt.wMinute = psyqFrame->m_DataTimeSend.min;
	pt.wMilliseconds = psyqFrame->m_DataTimeSend.second*1000;


	psyqFrame->m_pStation->InsertDataMessage(strTemp,&pt);//���뻺����У�
	AddDataMessage(psyqFrame->m_pStation, strTemp, &pt);//��ʾ������

	CSYQFrameFormat appFrame;
	m_pSubStation->BuildBasicPacket(appFrame);  //���ɳ�ʼ���·�֡����֡������Ϣ
	//������ݴ��
	m_pSubStation->GetSysDataTrans_Syq()->SYQ_EN_COMMON(&appFrame,0x31,FALSE);

}

void CSysDataTrans_Syq::Process_SYQ_32H(CSYQFrameFormat* psyqFrame)
{
	psyqFrame->m_DataTimeSend.year = 2000+CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[3]);
	psyqFrame->m_DataTimeSend.month = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[4]);
	psyqFrame->m_DataTimeSend.day = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[5]);
	psyqFrame->m_DataTimeSend.hour = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[6]);
	psyqFrame->m_DataTimeSend.min =CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[7]);
	psyqFrame->m_DataTimeSend.second = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[8]);

	psyqFrame->m_DataTimeGet.month = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[19]);//������������������
	psyqFrame->m_DataTimeGet.day = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[20]);
	psyqFrame->m_DataTimeGet.hour = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[21]);
	psyqFrame->m_DataTimeGet.min = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[22]);


	CString strTemp = "";

	strTemp.Format(" վ��:%05d ң��վ��ʱ������ʱ�䣺%04d-%02d-%02d %02d:%02d:%02d,�۲�ʱ�䣺%02d-%02d %02d:%02d\r\n",
		psyqFrame->m_stCode,
		psyqFrame->m_DataTimeSend.year,
		psyqFrame->m_DataTimeSend.month,
		psyqFrame->m_DataTimeSend.day,
		psyqFrame->m_DataTimeSend.hour,
		psyqFrame->m_DataTimeSend.min,
		psyqFrame->m_DataTimeSend.second,
		psyqFrame->m_DataTimeGet.month,
		psyqFrame->m_DataTimeGet.day,
		psyqFrame->m_DataTimeGet.hour,
		psyqFrame->m_DataTimeGet.min);
	//////////////////////////////////////////////////////////////////////////
	CString str; int len =0,datalen =0; 
	int tempmsglen = psyqFrame->nMsgLen - 23;
	//	 unsigned char * tempbuf = new unsigned char[tempmsglen];	
	//	 memcpy(tempbuf,&psyqFrame->_asdu.data[23],tempmsglen);

	for(int i = 0; i<(psyqFrame->nMsgLen - 23);)
	{
		GetPropertyString(&psyqFrame->_asdu.data[23+i],tempmsglen,str,len,datalen);

		std::map<CString, SYQ_YS_STRUCT>::iterator it_map;
		it_map = SYQ_TypeMap::GetInstance()->m_SYQTypeMap.find(str);
		if (it_map != SYQ_TypeMap::GetInstance()->m_SYQTypeMap.end())
		{
		    CString strvalue="";
			double fvalue=0.0;
			if(it_map->second.type == 2)//FLOAT
			{
				if(it_map->second.ename != "DRP"&&it_map->second.ename != "DRZ")
				{
					strTemp = strTemp + it_map->second.name +":";

					for(int j = 0;j<it_map->second.datalen1;j++)
					{
						strTemp = strTemp +(char)psyqFrame->_asdu.data[23+i+len+j];//����
						strvalue = strvalue+(char)psyqFrame->_asdu.data[23+i+len+j];//����
					}

					//	strTemp = strTemp+".";//С����

					for(int j = 0;j<it_map->second.datalen2;j++)
					{
						strTemp = strTemp +(char)psyqFrame->_asdu.data[23+i+len+it_map->second.datalen1+j];//С��
						strvalue = strvalue +(char)psyqFrame->_asdu.data[23+i+len+it_map->second.datalen1+j];//С��
					}

					if(str =="P1"||str=="PN05"||str=="PJ"||str=="PD"||str=="Z")//1Сʱ������5�����������������������ս�ˮ��)���ӵ�ˮλ
					{								
						COneData_INFO* pSave_data = new COneData_INFO();
						pSave_data->m_protocoltype = 1;
						pSave_data->m_syqSendTime.SetTime(psyqFrame->m_DataTimeSend);
						pSave_data->m_syqGetTime.month = psyqFrame->m_DataTimeGet.month;
						pSave_data->m_syqGetTime.day = psyqFrame->m_DataTimeGet.day;
						pSave_data->m_syqGetTime.hour = psyqFrame->m_DataTimeGet.hour;
						pSave_data->m_syqGetTime.min = psyqFrame->m_DataTimeGet.min;
						pSave_data->mA2 = psyqFrame->m_stCode;
						fvalue = _tstof(strvalue);
						pSave_data->m_strSyqType = str;
						if(str !="Z")
						{
							CRainAlert_PARA_PER* pRain = new CRainAlert_PARA_PER();
							pRain->SetRealTimeValue(fvalue);
							pSave_data->m_lstRain.AddTail(pRain);
						}
						else//ˮλ
						{
							CWaterLevel_PAR_PER* pLevel = new CWaterLevel_PAR_PER();
							pLevel->SetRealValue(fvalue);
							pSave_data->m_lstWaterLevel.AddTail(pLevel);
						}

						InsertOneSaveData(pSave_data);

					}
					strTemp = strTemp +it_map->second.Unit+" ";//��λ
	
				}
				else
				{
					if(it_map->second.ename == "DRP")
					{
						strTemp = strTemp + it_map->second.name +":";

						COneData_INFO* pSave_data = new COneData_INFO();
						pSave_data->m_protocoltype = 1;
						pSave_data->m_syqSendTime.SetTime(psyqFrame->m_DataTimeSend);
						pSave_data->m_syqGetTime.month = psyqFrame->m_DataTimeGet.month;
						pSave_data->m_syqGetTime.day = psyqFrame->m_DataTimeGet.day;
						pSave_data->m_syqGetTime.hour = psyqFrame->m_DataTimeGet.hour;
							pSave_data->m_syqGetTime.min = psyqFrame->m_DataTimeGet.min;
						pSave_data->mA2 = psyqFrame->m_stCode;
						
						pSave_data->m_strSyqType = str;

						for(int j = 0;j<it_map->second.datalen1;j++)
						{
							CString str1 ="";
							str1.Format("%f",((int)psyqFrame->_asdu.data[23+i+len+j])*0.1);
							strTemp = strTemp +str1;
							strTemp = strTemp +it_map->second.Unit+" ";//��λ

							CRainAlert_PARA_PER* pRain = new CRainAlert_PARA_PER();
							fvalue = (int)psyqFrame->_asdu.data[23+i+len+j]*0.1;
							pRain->SetRealTimeValue(fvalue);
							pSave_data->m_lstRain.AddTail(pRain);

							if(j == 6)
								strTemp = strTemp +"\r\n";
						}

							InsertOneSaveData(pSave_data);
					}
					else if(it_map->second.ename == "DRZ")
					{
						strTemp = strTemp + it_map->second.name +":";


						COneData_INFO* pSave_data = new COneData_INFO();
						pSave_data->m_protocoltype = 1;
						pSave_data->m_syqSendTime.SetTime(psyqFrame->m_DataTimeSend);
						pSave_data->m_syqGetTime.month = psyqFrame->m_DataTimeGet.month;
						pSave_data->m_syqGetTime.day = psyqFrame->m_DataTimeGet.day;
						pSave_data->m_syqGetTime.hour = psyqFrame->m_DataTimeGet.hour;
						pSave_data->m_syqGetTime.min = psyqFrame->m_DataTimeGet.min;
						pSave_data->mA2 = psyqFrame->m_stCode;

						pSave_data->m_strSyqType = str;

						for(int j = 0;j<it_map->second.datalen1;j++)
						{
							CString str1 ="";
							int t1 = psyqFrame->_asdu.data[23+i+len+j]+psyqFrame->_asdu.data[23+i+len+j+1]*256;
							float f1 = t1*0.01;
							str1.Format("%f",f1);
							strTemp = strTemp +str1;
							strTemp = strTemp +it_map->second.Unit+" ";//��λ

							CWaterLevel_PAR_PER* pLevel = new CWaterLevel_PAR_PER();
							fvalue = f1;
							pLevel->SetRealValue(fvalue);
							pSave_data->m_lstWaterLevel.AddTail(pLevel);


							j++;
							if(j == 12)
								strTemp = strTemp +"\r\n";


						}

						InsertOneSaveData(pSave_data);
					}
					strTemp = strTemp +"\r\n";
				}

			}

		}

		str ="";
		tempmsglen = tempmsglen - len - datalen;
		i = i+len+datalen;
		len = 0; datalen = 0;
	}


	////////////////////

	SYSTEMTIME pt ;
	pt.wYear = psyqFrame->m_DataTimeSend.year;
	pt.wMonth = psyqFrame->m_DataTimeSend.month;
	pt.wDay = psyqFrame->m_DataTimeSend.day;
	pt.wHour = psyqFrame->m_DataTimeSend.hour;
	pt.wMinute = psyqFrame->m_DataTimeSend.min;
	pt.wMilliseconds = psyqFrame->m_DataTimeSend.second*1000;
    

	psyqFrame->m_pStation->InsertDataMessage(strTemp,&pt);//���뻺����У�
	AddDataMessage(psyqFrame->m_pStation, strTemp, &pt);//��ʾ������

	CSYQFrameFormat appFrame;
	m_pSubStation->BuildBasicPacket(appFrame);  //���ɳ�ʼ���·�֡����֡������Ϣ
	//������ݴ��
	m_pSubStation->GetSysDataTrans_Syq()->SYQ_EN_COMMON(&appFrame,0x32,FALSE);

}

void CSysDataTrans_Syq::Process_SYQ_33H(CSYQFrameFormat* psyqFrame)
{
	psyqFrame->m_DataTimeSend.year = 2000+CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[3]);
	psyqFrame->m_DataTimeSend.month = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[4]);
	psyqFrame->m_DataTimeSend.day = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[5]);
	psyqFrame->m_DataTimeSend.hour = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[6]);
	psyqFrame->m_DataTimeSend.min =CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[7]);
	psyqFrame->m_DataTimeSend.second = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[8]);

	psyqFrame->m_DataTimeGet.month = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[12]);//������������������
	psyqFrame->m_DataTimeGet.day = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[13]);
	psyqFrame->m_DataTimeGet.hour = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[14]);
	psyqFrame->m_DataTimeGet.min = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[15]);


	CString strTemp = "";

	strTemp.Format("ң��վ��:%05d ң��վ�ӱ�������ʱ�䣺%04d-%02d-%02d %02d:%02d:%02d,�ɼ�ʱ�䣺%02d-%02d %02d:%02d\r\n",
		psyqFrame->m_stCode,
		psyqFrame->m_DataTimeSend.year,
		psyqFrame->m_DataTimeSend.month,
		psyqFrame->m_DataTimeSend.day,
		psyqFrame->m_DataTimeSend.hour,
		psyqFrame->m_DataTimeSend.min,
		psyqFrame->m_DataTimeSend.second,
		psyqFrame->m_DataTimeGet.month,
		psyqFrame->m_DataTimeGet.day,
		psyqFrame->m_DataTimeGet.hour,
		psyqFrame->m_DataTimeGet.min);
	//////////////////////////////////////////////////////////////////////////
	CString str; int len =0,datalen =0; 
	int tempmsglen = psyqFrame->nMsgLen - 16;

	for(int i = 0; i<(psyqFrame->nMsgLen - 16);)
	{
		GetPropertyString(&psyqFrame->_asdu.data[16+i],tempmsglen,str,len,datalen);

		std::map<CString, SYQ_YS_STRUCT>::iterator it_map;
		it_map = SYQ_TypeMap::GetInstance()->m_SYQTypeMap.find(str);
		if (it_map != SYQ_TypeMap::GetInstance()->m_SYQTypeMap.end())
		{
			if(it_map->second.type == 2)//FLOAT
			{
				if(it_map->second.ename != "DRP"&&it_map->second.ename != "DRZ")
				{				
					strTemp = strTemp + it_map->second.name +":";

					for(int j = 0;j<it_map->second.datalen1;j++)
						strTemp = strTemp +(char)psyqFrame->_asdu.data[16+i+len+j];//����

					//	strTemp = strTemp+".";//С����

					for(int j = 0;j<it_map->second.datalen2;j++)
						strTemp = strTemp +(char)psyqFrame->_asdu.data[16+i+len+it_map->second.datalen1+j];//С��

					strTemp = strTemp +it_map->second.Unit+" ";//��λ
				}
				else
				{
					if(it_map->second.ename == "DRP")
					{
						strTemp = strTemp + it_map->second.name +":";

						for(int j = 0;j<it_map->second.datalen1;j++)
						{
							CString str1 ="";
							str1.Format("%f",((int)psyqFrame->_asdu.data[16+i+len+j])*0.1);
							strTemp = strTemp +str1;
							strTemp = strTemp +it_map->second.Unit+" ";//��λ

							if(j == 6)
								strTemp = strTemp +"\r\n";
						}
					}
					else if(it_map->second.ename == "DRZ")
					{
						strTemp = strTemp + it_map->second.name +":";

						for(int j = 0;j<it_map->second.datalen1;j++)
						{
							CString str1 ="";
							int t1 = psyqFrame->_asdu.data[16+i+len+j]+psyqFrame->_asdu.data[16+i+len+j+1]*256;
							float f1 = t1*0.01;
							str1.Format("%f",f1);
							strTemp = strTemp +str1;
							strTemp = strTemp +it_map->second.Unit+" ";//��λ

							j++;

							if(j == 13)
								strTemp = strTemp +"\r\n";
						}
					}
					strTemp = strTemp +"\r\n";
				}

			}

		}

		str ="";
		tempmsglen = tempmsglen - len - datalen;
		i = i+len+datalen;
		len = 0; datalen = 0;
	}

	////////////////////

	SYSTEMTIME pt ;
	pt.wYear = psyqFrame->m_DataTimeSend.year;
	pt.wMonth = psyqFrame->m_DataTimeSend.month;
	pt.wDay = psyqFrame->m_DataTimeSend.day;
	pt.wHour = psyqFrame->m_DataTimeSend.hour;
	pt.wMinute = psyqFrame->m_DataTimeSend.min;
	pt.wMilliseconds = psyqFrame->m_DataTimeSend.second*1000;


	psyqFrame->m_pStation->InsertDataMessage(strTemp,&pt);//���뻺����У�
	AddDataMessage(psyqFrame->m_pStation, strTemp, &pt);//��ʾ������

	CSYQFrameFormat appFrame;
	m_pSubStation->BuildBasicPacket(appFrame);  //���ɳ�ʼ���·�֡����֡������Ϣ
	//������ݴ��
	m_pSubStation->GetSysDataTrans_Syq()->SYQ_EN_COMMON(&appFrame,0x33,FALSE);

}

void CSysDataTrans_Syq::Process_SYQ_34H(CSYQFrameFormat* psyqFrame)
{
	psyqFrame->m_DataTimeSend.year = 2000+CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[3]);
	psyqFrame->m_DataTimeSend.month = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[4]);
	psyqFrame->m_DataTimeSend.day = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[5]);
	psyqFrame->m_DataTimeSend.hour = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[6]);
	psyqFrame->m_DataTimeSend.min =CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[7]);
	psyqFrame->m_DataTimeSend.second = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[8]);

	psyqFrame->m_DataTimeGet.month = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[12]);//������������������
	psyqFrame->m_DataTimeGet.day = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[13]);
	psyqFrame->m_DataTimeGet.hour = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[14]);
	psyqFrame->m_DataTimeGet.min = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[15]);


	CString strTemp = "";

	strTemp.Format("ң��վ��:%05d ң��վСʱ������ʱ�䣺%04d-%02d-%02d %02d:%02d:%02d,�ɼ�ʱ�䣺%02d-%02d %02d:%02d\r\n",
		psyqFrame->m_stCode,
		psyqFrame->m_DataTimeSend.year,
		psyqFrame->m_DataTimeSend.month,
		psyqFrame->m_DataTimeSend.day,
		psyqFrame->m_DataTimeSend.hour,
		psyqFrame->m_DataTimeSend.min,
		psyqFrame->m_DataTimeSend.second,
		psyqFrame->m_DataTimeGet.month,
		psyqFrame->m_DataTimeGet.day,
		psyqFrame->m_DataTimeGet.hour,
		psyqFrame->m_DataTimeGet.min);
	//////////////////////////////////////////////////////////////////////////
	CString str; int len =0,datalen =0; 
	int tempmsglen = psyqFrame->nMsgLen - 16;

	for(int i = 0; i<(psyqFrame->nMsgLen - 16);)
	{
		GetPropertyString(&psyqFrame->_asdu.data[16+i],tempmsglen,str,len,datalen);

		std::map<CString, SYQ_YS_STRUCT>::iterator it_map;
		it_map = SYQ_TypeMap::GetInstance()->m_SYQTypeMap.find(str);
		if (it_map != SYQ_TypeMap::GetInstance()->m_SYQTypeMap.end())
		{
			if(it_map->second.type == 2)//FLOAT
			{
				if(it_map->second.ename != "DRP"&&it_map->second.ename != "DRZ")
				{				
					strTemp = strTemp + it_map->second.name +":";

					for(int j = 0;j<it_map->second.datalen1;j++)
						strTemp = strTemp +(char)psyqFrame->_asdu.data[16+i+len+j];//����

					//	strTemp = strTemp+".";//С����

					for(int j = 0;j<it_map->second.datalen2;j++)
						strTemp = strTemp +(char)psyqFrame->_asdu.data[16+i+len+it_map->second.datalen1+j];//С��

					strTemp = strTemp +it_map->second.Unit+" ";//��λ
				}
				else
				{
					if(it_map->second.ename == "DRP")
					{
						strTemp = strTemp + it_map->second.name +":";

						for(int j = 0;j<it_map->second.datalen1;j++)
						{
							CString str1 ="";
							str1.Format("%f",((int)psyqFrame->_asdu.data[16+i+len+j])*0.1);
							strTemp = strTemp +str1;
							strTemp = strTemp +it_map->second.Unit+" ";//��λ

							if(j == 5)
								strTemp = strTemp +"\r\n";
						}
					}
					else if(it_map->second.ename == "DRZ")
					{
						strTemp = strTemp + it_map->second.name +":";

						for(int j = 0;j<it_map->second.datalen1;j++)
						{
							CString str1 ="";
							int t1 = psyqFrame->_asdu.data[16+i+len+j]+psyqFrame->_asdu.data[16+i+len+j+1]*256;
							float f1 = t1*0.01;
							str1.Format("%f",f1);
							strTemp = strTemp +str1;
							strTemp = strTemp +it_map->second.Unit+" ";//��λ

							j++;

							if(j == 11)
								strTemp = strTemp +"\r\n";
						}
					}
					strTemp = strTemp +"\r\n";
				}

			}

		}

		str ="";
		tempmsglen = tempmsglen - len - datalen;
		i = i+len+datalen;
		len = 0; datalen = 0;
	}

	////////////////////

	SYSTEMTIME pt ;
	pt.wYear = psyqFrame->m_DataTimeSend.year;
	pt.wMonth = psyqFrame->m_DataTimeSend.month;
	pt.wDay = psyqFrame->m_DataTimeSend.day;
	pt.wHour = psyqFrame->m_DataTimeSend.hour;
	pt.wMinute = psyqFrame->m_DataTimeSend.min;
	pt.wMilliseconds = psyqFrame->m_DataTimeSend.second*1000;


	psyqFrame->m_pStation->InsertDataMessage(strTemp,&pt);//���뻺����У�
	AddDataMessage(psyqFrame->m_pStation, strTemp, &pt);//��ʾ������

	CSYQFrameFormat appFrame;
	m_pSubStation->BuildBasicPacket(appFrame);  //���ɳ�ʼ���·�֡����֡������Ϣ
	//������ݴ��
	m_pSubStation->GetSysDataTrans_Syq()->SYQ_EN_COMMON(&appFrame,0x34,FALSE);

}

void CSysDataTrans_Syq::Process_SYQ_35H(CSYQFrameFormat* psyqFrame)
{
	psyqFrame->m_DataTimeSend.year = 2000+CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[3]);
	psyqFrame->m_DataTimeSend.month = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[4]);
	psyqFrame->m_DataTimeSend.day = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[5]);
	psyqFrame->m_DataTimeSend.hour = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[6]);
	psyqFrame->m_DataTimeSend.min =CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[7]);
	psyqFrame->m_DataTimeSend.second = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[8]);

	psyqFrame->m_DataTimeGet.month = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[19]);//������������������
	psyqFrame->m_DataTimeGet.day = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[20]);
	psyqFrame->m_DataTimeGet.hour = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[21]);
	psyqFrame->m_DataTimeGet.min = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[22]);


	CString strTemp = "";

	strTemp.Format(" վ��:%05d ң��վ�˹�����������ʱ�䣺%04d-%02d-%02d %02d:%02d:%02d,�۲�ʱ�䣺%02d-%02d %02d:%02d\r\n",
		psyqFrame->m_stCode,
		psyqFrame->m_DataTimeSend.year,
		psyqFrame->m_DataTimeSend.month,
		psyqFrame->m_DataTimeSend.day,
		psyqFrame->m_DataTimeSend.hour,
		psyqFrame->m_DataTimeSend.min,
		psyqFrame->m_DataTimeSend.second,
		psyqFrame->m_DataTimeGet.month,
		psyqFrame->m_DataTimeGet.day,
		psyqFrame->m_DataTimeGet.hour,
		psyqFrame->m_DataTimeGet.min);
	//////////////////////////////////////////////////////////////////////////
	CString str; int len =0,datalen =0; 
	int tempmsglen = psyqFrame->nMsgLen - 23;
	//	 unsigned char * tempbuf = new unsigned char[tempmsglen];	
	//	 memcpy(tempbuf,&psyqFrame->_asdu.data[23],tempmsglen);

	for(int i = 0; i<(psyqFrame->nMsgLen - 23);)
	{
		GetPropertyString(&psyqFrame->_asdu.data[23+i],tempmsglen,str,len,datalen);

		std::map<CString, SYQ_YS_STRUCT>::iterator it_map;
		it_map = SYQ_TypeMap::GetInstance()->m_SYQTypeMap.find(str);
		if (it_map != SYQ_TypeMap::GetInstance()->m_SYQTypeMap.end())
		{
			if(it_map->second.type == 2)//FLOAT
			{
				if(it_map->second.ename != "DRP"&&it_map->second.ename != "DRZ")
				{
					strTemp = strTemp + it_map->second.name +":";

					for(int j = 0;j<it_map->second.datalen1;j++)
						strTemp = strTemp +(char)psyqFrame->_asdu.data[23+i+len+j];//����

					//	strTemp = strTemp+".";//С����

					for(int j = 0;j<it_map->second.datalen2;j++)
						strTemp = strTemp +(char)psyqFrame->_asdu.data[23+i+len+it_map->second.datalen1+j];//С��

					strTemp = strTemp +it_map->second.Unit+" ";//��λ
				}
				else
				{
					if(it_map->second.ename == "DRP")
					{
						strTemp = strTemp + it_map->second.name +":";

						for(int j = 0;j<it_map->second.datalen1;j++)
						{
							CString str1 ="";
							str1.Format("%f",((int)psyqFrame->_asdu.data[23+i+len+j])*0.1);
							strTemp = strTemp +str1;
							strTemp = strTemp +it_map->second.Unit+" ";//��λ

							if(j == 6)
								strTemp = strTemp +"\r\n";
						}
					}
					else if(it_map->second.ename == "DRZ")
					{
						strTemp = strTemp + it_map->second.name +":";

						for(int j = 0;j<it_map->second.datalen1;j++)
						{
							CString str1 ="";
							int t1 = psyqFrame->_asdu.data[23+i+len+j]+psyqFrame->_asdu.data[23+i+len+j+1]*256;
							float f1 = t1*0.01;
							str1.Format("%f",f1);
							strTemp = strTemp +str1;
							strTemp = strTemp +it_map->second.Unit+" ";//��λ

							j++;

							if(j == 12)
								strTemp = strTemp +"\r\n";
						}
					}
					strTemp = strTemp +"\r\n";
				}

			}

		}

		str ="";
		tempmsglen = tempmsglen - len - datalen;
		i = i+len+datalen;
		len = 0; datalen = 0;
	}


	////////////////////

	SYSTEMTIME pt ;
	pt.wYear = psyqFrame->m_DataTimeSend.year;
	pt.wMonth = psyqFrame->m_DataTimeSend.month;
	pt.wDay = psyqFrame->m_DataTimeSend.day;
	pt.wHour = psyqFrame->m_DataTimeSend.hour;
	pt.wMinute = psyqFrame->m_DataTimeSend.min;
	pt.wMilliseconds = psyqFrame->m_DataTimeSend.second*1000;


	psyqFrame->m_pStation->InsertDataMessage(strTemp,&pt);//���뻺����У�
	AddDataMessage(psyqFrame->m_pStation, strTemp, &pt);//��ʾ������


	CSYQFrameFormat appFrame;
	m_pSubStation->BuildBasicPacket(appFrame);  //���ɳ�ʼ���·�֡����֡������Ϣ
	//������ݴ��
	m_pSubStation->GetSysDataTrans_Syq()->SYQ_EN_COMMON(&appFrame,0x35,FALSE);

}



void CSysDataTrans_Syq::Process_SYQ_36H(CSYQFrameFormat* psyqFrame)
{
	psyqFrame->m_DataTimeSend.year = 2000+CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[3]);
	psyqFrame->m_DataTimeSend.month = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[4]);
	psyqFrame->m_DataTimeSend.day = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[5]);
	psyqFrame->m_DataTimeSend.hour = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[6]);
	psyqFrame->m_DataTimeSend.min =CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[7]);
	psyqFrame->m_DataTimeSend.second = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[8]);

	psyqFrame->m_DataTimeGet.month = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[12]);//������������������
	psyqFrame->m_DataTimeGet.day = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[13]);
	psyqFrame->m_DataTimeGet.hour = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[14]);
	psyqFrame->m_DataTimeGet.min = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[15]);


	CString strTemp = "";

	strTemp.Format("ң��վ��:%05d ����վ��ѯң��վʵʱ���ݱ�������ʱ�䣺%04d-%02d-%02d %02d:%02d:%02d,�ɼ�ʱ�䣺%02d-%02d %02d:%02d\r\n",
		psyqFrame->m_stCode,
		psyqFrame->m_DataTimeSend.year,
		psyqFrame->m_DataTimeSend.month,
		psyqFrame->m_DataTimeSend.day,
		psyqFrame->m_DataTimeSend.hour,
		psyqFrame->m_DataTimeSend.min,
		psyqFrame->m_DataTimeSend.second,
		psyqFrame->m_DataTimeGet.month,
		psyqFrame->m_DataTimeGet.day,
		psyqFrame->m_DataTimeGet.hour,
		psyqFrame->m_DataTimeGet.min);
	//////////////////////////////////////////////////////////////////////////
	//data δ����
	CString str ="",str1 ="";
	int len =0,datalen =0; 
	int tempmsglen = psyqFrame->nMsgLen - 16;

	if(m_pSubStation->m_syqdatadiscribelist.size()>0)
		m_pSubStation->m_syqdatadiscribelist.clear();

	for(int i = 0; i<(psyqFrame->nMsgLen - 16);)
	{
		GetPropertyString(&psyqFrame->_asdu.data[16+i],tempmsglen,str,len,datalen);

		std::map<CString, SYQ_YS_STRUCT>::iterator it_map;
		it_map = SYQ_TypeMap::GetInstance()->m_SYQTypeMap.find(str);
		if (it_map != SYQ_TypeMap::GetInstance()->m_SYQTypeMap.end())
		{
			if(it_map->second.type == 2)//FLOAT
			{
				if(it_map->second.ename != "DRP"&&it_map->second.ename != "DRZ")
				{				
					strTemp = strTemp + it_map->second.name +":";
					_SYQ_DATA_DESCRIBE *sdd = new _SYQ_DATA_DESCRIBE();
					sdd->strname = it_map->second.name;

					for(int j = 0;j<it_map->second.datalen1;j++)
					{
						strTemp = strTemp +(char)psyqFrame->_asdu.data[16+i+len+j];//����
						str1 = str1 +(char)psyqFrame->_asdu.data[16+i+len+j];//����
					}

					//strTemp = strTemp+".";//С����

					for(int j = 0;j<it_map->second.datalen2;j++)
					{
						strTemp = strTemp +(char)psyqFrame->_asdu.data[16+i+len+it_map->second.datalen1+j];//С��
						str1 = str1 +(char)psyqFrame->_asdu.data[16+i+len+it_map->second.datalen1+j];//С��
					}

					strTemp = strTemp +it_map->second.Unit+" ";//��λ
					str1 = str1 +it_map->second.Unit;//��λ

					sdd->strdata = str1;
					m_pSubStation->m_syqdatadiscribelist.push_back(sdd);
					str1 = "";
				}
				else
				{
					if(it_map->second.ename == "DRP")
					{
						strTemp = strTemp + it_map->second.name +":";
						_SYQ_DATA_DESCRIBE *sdd = new _SYQ_DATA_DESCRIBE();
						sdd->strname = it_map->second.name;

						for(int j = 0;j<it_map->second.datalen1;j++)
						{
							str1 ="";
							str1.Format("%f",((int)psyqFrame->_asdu.data[16+i+len+j])*0.1);
							strTemp = strTemp +str1;
							strTemp = strTemp +it_map->second.Unit+" ";//��λ

							sdd->strdata= str1 +it_map->second.Unit;
							m_pSubStation->m_syqdatadiscribelist.push_back(sdd);

							if(j == 6)
								strTemp = strTemp +"\r\n";
						}
					}
					else if(it_map->second.ename == "DRZ")
					{
						strTemp = strTemp + it_map->second.name +":";
						_SYQ_DATA_DESCRIBE *sdd = new _SYQ_DATA_DESCRIBE();
						sdd->strname = it_map->second.name;

						for(int j = 0;j<it_map->second.datalen1;j++)
						{
							str1 ="";
							int t1 = psyqFrame->_asdu.data[16+i+len+j]+psyqFrame->_asdu.data[16+i+len+j+1]*256;
							float f1 = t1*0.01;
							str1.Format("%f",f1);
							strTemp = strTemp +str1;
							strTemp = strTemp +it_map->second.Unit+" ";//��λ

							sdd->strdata= str1 +it_map->second.Unit;
							m_pSubStation->m_syqdatadiscribelist.push_back(sdd);

							j++;

							if(j == 13)
								strTemp = strTemp +"\r\n";
						}
					}
					strTemp = strTemp +"\r\n";
				}

			}

		}

		str ="";
		tempmsglen = tempmsglen - len - datalen;
		i = i+len+datalen;
		len = 0; datalen = 0;
	}

	////////////////////

	SYSTEMTIME pt ;
	pt.wYear = psyqFrame->m_DataTimeSend.year;
	pt.wMonth = psyqFrame->m_DataTimeSend.month;
	pt.wDay = psyqFrame->m_DataTimeSend.day;
	pt.wHour = psyqFrame->m_DataTimeSend.hour;
	pt.wMinute = psyqFrame->m_DataTimeSend.min;
	pt.wMilliseconds = psyqFrame->m_DataTimeSend.second*1000;

	UpdateAttribFrame(m_pSubStation,syq_call_data1);
	psyqFrame->m_pStation->InsertDataMessage(strTemp,&pt);//���뻺����У�
	AddDataMessage(psyqFrame->m_pStation, strTemp, &pt);//��ʾ������

	/*	CSYQFrameFormat appFrame;
	m_pSubStation->BuildBasicPacket(appFrame);  //���ɳ�ʼ���·�֡����֡������Ϣ
	//������ݴ��
	m_pSubStation->GetSysDataTrans_Syq()->SYQ_EN_COMMON(&appFrame,0x34);
	*/
}


void CSysDataTrans_Syq::Process_SYQ_37H(CSYQFrameFormat* psyqFrame)
{
	psyqFrame->m_DataTimeSend.year = 2000+CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[3]);
	psyqFrame->m_DataTimeSend.month = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[4]);
	psyqFrame->m_DataTimeSend.day = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[5]);
	psyqFrame->m_DataTimeSend.hour = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[6]);
	psyqFrame->m_DataTimeSend.min =CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[7]);
	psyqFrame->m_DataTimeSend.second = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[8]);

	psyqFrame->m_DataTimeGet.month = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[12]);//������������������
	psyqFrame->m_DataTimeGet.day = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[13]);
	psyqFrame->m_DataTimeGet.hour = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[14]);
	psyqFrame->m_DataTimeGet.min = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[15]);


	CString strTemp = "";

	strTemp.Format("ң��վ��:%05d ����վ��ѯң��վָ��Ҫ��ʵʱ���ݱ�����ʱ�䣺%04d-%02d-%02d %02d:%02d:%02d,�ɼ�ʱ�䣺%02d-%02d %02d:%02d\r\n",
		psyqFrame->m_stCode,
		psyqFrame->m_DataTimeSend.year,
		psyqFrame->m_DataTimeSend.month,
		psyqFrame->m_DataTimeSend.day,
		psyqFrame->m_DataTimeSend.hour,
		psyqFrame->m_DataTimeSend.min,
		psyqFrame->m_DataTimeSend.second,
		psyqFrame->m_DataTimeGet.month,
		psyqFrame->m_DataTimeGet.day,
		psyqFrame->m_DataTimeGet.hour,
		psyqFrame->m_DataTimeGet.min);
	//////////////////////////////////////////////////////////////////////////
	//data δ����
	if(m_pSubStation->m_syqdatadiscribelist.size()>0)
		m_pSubStation->m_syqdatadiscribelist.clear();

	CString str,str1="";
	int len =0,datalen =0; 
	int tempmsglen = psyqFrame->nMsgLen - 16;

	for(int i = 0; i<(psyqFrame->nMsgLen - 16);)
	{
		GetPropertyString(&psyqFrame->_asdu.data[16+i],tempmsglen,str,len,datalen);

		std::map<CString, SYQ_YS_STRUCT>::iterator it_map;
		it_map = SYQ_TypeMap::GetInstance()->m_SYQTypeMap.find(str);
		if (it_map != SYQ_TypeMap::GetInstance()->m_SYQTypeMap.end())
		{
			if(it_map->second.type == 2)//FLOAT
			{
				if(it_map->second.ename != "DRP"&&it_map->second.ename != "DRZ")
				{
					_SYQ_DATA_DESCRIBE *sdd = new _SYQ_DATA_DESCRIBE();

					strTemp = strTemp + it_map->second.name +":";

					sdd->strname = it_map->second.name;

					for(int j = 0;j<it_map->second.datalen1;j++)
					{
						strTemp = strTemp +(char)psyqFrame->_asdu.data[16+i+len+j];//����
						str1 = str1 + (char)psyqFrame->_asdu.data[16+i+len+j];//����
					}

					//	strTemp = strTemp+".";//С����

					for(int j = 0;j<it_map->second.datalen2;j++)
					{
						strTemp = strTemp +(char)psyqFrame->_asdu.data[16+i+len+it_map->second.datalen1+j];//С��
						str1 = str1 + (char)psyqFrame->_asdu.data[16+i+len+it_map->second.datalen1+j];//С��
					}

					strTemp = strTemp +it_map->second.Unit+" ";//��λ
					str1 = str1 + it_map->second.Unit;//��λ

					sdd->strdata = str1;

					m_pSubStation->m_syqdatadiscribelist.push_back(sdd);

					str1 ="";
				}
				else//δ���
				{
					if(it_map->second.ename == "DRP")
					{
						strTemp = strTemp + it_map->second.name +":";

						_SYQ_DATA_DESCRIBE *sdd = new _SYQ_DATA_DESCRIBE();

						sdd->strname = it_map->second.name;

						for(int j = 0;j<it_map->second.datalen1;j++)
						{
							 str1 ="";
							str1.Format("%f",((int)psyqFrame->_asdu.data[16+i+len+j])*0.1);
							strTemp = strTemp +str1;
							strTemp = strTemp +it_map->second.Unit+" ";//��λ

							sdd->strdata = str1+it_map->second.Unit;

							m_pSubStation->m_syqdatadiscribelist.push_back(sdd);

							if(j == 6)
								strTemp = strTemp +"\r\n";
						}
					}
					else if(it_map->second.ename == "DRZ")
					{
						strTemp = strTemp + it_map->second.name +":";

						_SYQ_DATA_DESCRIBE *sdd = new _SYQ_DATA_DESCRIBE();
						sdd->strname = it_map->second.name;

						for(int j = 0;j<it_map->second.datalen1;j++)
						{
							str1 ="";
							int t1 = psyqFrame->_asdu.data[16+i+len+j]+psyqFrame->_asdu.data[16+i+len+j+1]*256;
							float f1 = t1*0.01;
							str1.Format("%f",f1);
							strTemp = strTemp +str1;
							strTemp = strTemp +it_map->second.Unit+" ";//��λ

							sdd->strdata = str1+it_map->second.Unit;
							m_pSubStation->m_syqdatadiscribelist.push_back(sdd);

							j++;

							if(j == 13)
								strTemp = strTemp +"\r\n";
						}
					}
					strTemp = strTemp +"\r\n";
				}

			}

		}

		str ="";
		tempmsglen = tempmsglen - len - datalen;
		i = i+len+datalen;
		len = 0; datalen = 0;
	}

	////////////////////

	SYSTEMTIME pt ;
	pt.wYear = psyqFrame->m_DataTimeSend.year;
	pt.wMonth = psyqFrame->m_DataTimeSend.month;
	pt.wDay = psyqFrame->m_DataTimeSend.day;
	pt.wHour = psyqFrame->m_DataTimeSend.hour;
	pt.wMinute = psyqFrame->m_DataTimeSend.min;
	pt.wMilliseconds = psyqFrame->m_DataTimeSend.second*1000;

	UpdateAttribFrame(m_pSubStation,syq_call_data);
	psyqFrame->m_pStation->InsertDataMessage(strTemp,&pt);//���뻺����У�
	AddDataMessage(psyqFrame->m_pStation, strTemp, &pt);//��ʾ������

	/*	CSYQFrameFormat appFrame;
	m_pSubStation->BuildBasicPacket(appFrame);  //���ɳ�ʼ���·�֡����֡������Ϣ
	//������ݴ��
	m_pSubStation->GetSysDataTrans_Syq()->SYQ_EN_COMMON(&appFrame,0x34);
	*/
}


void CSysDataTrans_Syq::Process_SYQ_38H(CSYQFrameFormat* psyqFrame)
{
	psyqFrame->m_DataTimeSend.year = 2000+CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[3]);
	psyqFrame->m_DataTimeSend.month = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[4]);
	psyqFrame->m_DataTimeSend.day = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[5]);
	psyqFrame->m_DataTimeSend.hour = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[6]);
	psyqFrame->m_DataTimeSend.min =CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[7]);
	psyqFrame->m_DataTimeSend.second = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[8]);

	psyqFrame->m_DataTimeGet.month = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[13]);//������������������
	psyqFrame->m_DataTimeGet.day = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[14]);
	psyqFrame->m_DataTimeGet.hour = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[15]);
	psyqFrame->m_DataTimeGet.min = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[16]);

	CString tempstr = "";//ʱ�䲽��
	std::map<CString, SYQ_YS_STRUCT>::iterator it_map;
	for(int i = 0;i<3;i++)
		tempstr = tempstr +(char)psyqFrame->_asdu.data[17+i];
	it_map = SYQ_TypeMap::GetInstance()->m_SYQTypeMap.find(tempstr);
	if (it_map != SYQ_TypeMap::GetInstance()->m_SYQTypeMap.end())
	{
		tempstr = "";
		tempstr = it_map->second.name +(char)psyqFrame->_asdu.data[20]+(char)psyqFrame->_asdu.data[21]+it_map->second.Unit;
	}

	CString strTemp = "";

	strTemp.Format("ң��վ��:%05d ����վ��ѯң��վʱ�����ݱ�����ʱ�䣺%04d-%02d-%02d %02d:%02d:%02d,�ɼ�ʱ�䣺%02d-%02d %02d:%02d\r\n",
		psyqFrame->m_stCode,
		psyqFrame->m_DataTimeSend.year,
		psyqFrame->m_DataTimeSend.month,
		psyqFrame->m_DataTimeSend.day,
		psyqFrame->m_DataTimeSend.hour,
		psyqFrame->m_DataTimeSend.min,
		psyqFrame->m_DataTimeSend.second,
		psyqFrame->m_DataTimeGet.month,
		psyqFrame->m_DataTimeGet.day,
		psyqFrame->m_DataTimeGet.hour,
		psyqFrame->m_DataTimeGet.min);

	strTemp = strTemp + tempstr +"\r\n";
	//////////////////////////////////////////////////////////////////////////
	CString str; int len =0,datalen =0; 
	int tempmsglen = psyqFrame->nMsgLen - 22;

	if(m_pSubStation->m_syqdatadiscribelist.size()>0)
		m_pSubStation->m_syqdatadiscribelist.clear();

	GetPropertyString(&psyqFrame->_asdu.data[23],tempmsglen,str,len,datalen);

	it_map = SYQ_TypeMap::GetInstance()->m_SYQTypeMap.find(str);
	if (it_map != SYQ_TypeMap::GetInstance()->m_SYQTypeMap.end())
	{
		if(it_map->second.type == 2)//FLOAT
		{
			if(it_map->second.ename != "DRP"&&it_map->second.ename != "DRZ")
			{	
				int nlen = it_map->second.datalen1 +it_map->second.datalen2;
				nlen = (tempmsglen - len)/nlen;
				CString str1 ="";
				for(int i=0; i<nlen;i++)
				{
					_SYQ_DATA_DESCRIBE *sdd = new _SYQ_DATA_DESCRIBE();
					str1.Format("��%d������:",i+1);

					sdd->strname = it_map->second.name+str1;

					strTemp = strTemp + str1;

					str1 ="";
					for(int j = 0;j<it_map->second.datalen1;j++)
					{
						strTemp = strTemp +(char)psyqFrame->_asdu.data[22+i+len+j];//����
						str1 =str1+(char)psyqFrame->_asdu.data[22+i+len+j];
					}

					//	strTemp = strTemp+".";//С����

					for(int j = 0;j<it_map->second.datalen2;j++)
					{
						strTemp = strTemp +(char)psyqFrame->_asdu.data[22+i+len+it_map->second.datalen1+j];//С��

						str1 = str1 +(char)psyqFrame->_asdu.data[22+i+len+it_map->second.datalen1+j];
					}

					strTemp = strTemp + it_map->second.Unit+" "+"\r\n";
					str1 = str1 +it_map->second.Unit;
					 sdd->strdata = str1;
					 
					 m_pSubStation->m_syqdatadiscribelist.push_back(sdd);
	
					str1 = "";

				}

			}

		}

	}

	////////////////////

	SYSTEMTIME pt ;
	pt.wYear = psyqFrame->m_DataTimeSend.year;
	pt.wMonth = psyqFrame->m_DataTimeSend.month;
	pt.wDay = psyqFrame->m_DataTimeSend.day;
	pt.wHour = psyqFrame->m_DataTimeSend.hour;
	pt.wMinute = psyqFrame->m_DataTimeSend.min;
	pt.wMilliseconds = psyqFrame->m_DataTimeSend.second*1000;

	UpdateAttribFrame(m_pSubStation,syq_call_data_bytime);
	psyqFrame->m_pStation->InsertDataMessage(strTemp,&pt);//���뻺����У�
	AddDataMessage(psyqFrame->m_pStation, strTemp, &pt);//��ʾ������

	/*	CSYQFrameFormat appFrame;
	m_pSubStation->BuildBasicPacket(appFrame);  //���ɳ�ʼ���·�֡����֡������Ϣ
	//������ݴ��
	m_pSubStation->GetSysDataTrans_Syq()->SYQ_EN_COMMON(&appFrame,0x34);
	*/
}


void CSysDataTrans_Syq::Process_SYQ_39H(CSYQFrameFormat* psyqFrame)
{
	psyqFrame->m_DataTimeSend.year = 2000+CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[3]);
	psyqFrame->m_DataTimeSend.month = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[4]);
	psyqFrame->m_DataTimeSend.day = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[5]);
	psyqFrame->m_DataTimeSend.hour = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[6]);
	psyqFrame->m_DataTimeSend.min =CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[7]);
	psyqFrame->m_DataTimeSend.second = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[8]);

	psyqFrame->m_DataTimeGet.month = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[19]);//������������������
	psyqFrame->m_DataTimeGet.day = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[20]);
	psyqFrame->m_DataTimeGet.hour = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[21]);
	psyqFrame->m_DataTimeGet.min = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[22]);


	CString strTemp = "";

	strTemp.Format("վ��:%05d ����վ��ѯң��վ�˹�����������ʱ�䣺%04d-%02d-%02d %02d:%02d:%02d,�۲�ʱ�䣺%02d-%02d %02d:%02d\r\n",
		psyqFrame->m_stCode,
		psyqFrame->m_DataTimeSend.year,
		psyqFrame->m_DataTimeSend.month,
		psyqFrame->m_DataTimeSend.day,
		psyqFrame->m_DataTimeSend.hour,
		psyqFrame->m_DataTimeSend.min,
		psyqFrame->m_DataTimeSend.second,
		psyqFrame->m_DataTimeGet.month,
		psyqFrame->m_DataTimeGet.day,
		psyqFrame->m_DataTimeGet.hour,
		psyqFrame->m_DataTimeGet.min);
	//////////////////////////////////////////////////////////////////////////




	CString str="",str1 =""; 
	int len =0,datalen =0; 
	int tempmsglen = psyqFrame->nMsgLen - 23;
	//	 unsigned char * tempbuf = new unsigned char[tempmsglen];	
	//	 memcpy(tempbuf,&psyqFrame->_asdu.data[23],tempmsglen);

	if(m_pSubStation->m_syqdatadiscribelist.size()>0)
		m_pSubStation->m_syqdatadiscribelist.clear();

	for(int i = 0; i<(psyqFrame->nMsgLen - 23);)
	{
		GetPropertyString(&psyqFrame->_asdu.data[23+i],tempmsglen,str,len,datalen);

		std::map<CString, SYQ_YS_STRUCT>::iterator it_map;
		it_map = SYQ_TypeMap::GetInstance()->m_SYQTypeMap.find(str);
		if (it_map != SYQ_TypeMap::GetInstance()->m_SYQTypeMap.end())
		{
			if(it_map->second.type == 2)//FLOAT
			{
				if(it_map->second.ename != "DRP"||it_map->second.ename != "DRZ")
				{
					strTemp = strTemp + it_map->second.name +":";

					_SYQ_DATA_DESCRIBE *sdd = new _SYQ_DATA_DESCRIBE();
					sdd->strname = it_map->second.name;

					for(int j = 0;j<it_map->second.datalen1;j++)
					{
						strTemp = strTemp +(char)psyqFrame->_asdu.data[23+i+len+j];//����
						str1 = str1 + (char)psyqFrame->_asdu.data[23+i+len+j];//����
					}


					//strTemp = strTemp+".";//С����

					for(int j = 0;j<it_map->second.datalen2;j++)
					{
						strTemp = strTemp +(char)psyqFrame->_asdu.data[23+i+len+it_map->second.datalen1+j];//С��
						str1 = str1 + (char)psyqFrame->_asdu.data[23+i+len+it_map->second.datalen1+j];//С��
					}

					strTemp = strTemp +it_map->second.Unit+" ";//��λ
					str1 = str1 + it_map->second.Unit;

					sdd->strdata = str1;

					m_pSubStation->m_syqdatadiscribelist.push_back(sdd);

					str1 ="";
				}
				else
				{
					if(it_map->second.ename == "DRP")
					{
						strTemp = strTemp + it_map->second.name +":";
						_SYQ_DATA_DESCRIBE *sdd = new _SYQ_DATA_DESCRIBE();
						sdd->strname = it_map->second.name;

						for(int j = 0;j<it_map->second.datalen1;j++)
						{
							str1 ="";
							str1.Format("%f",((int)psyqFrame->_asdu.data[23+i+len+j])*0.1);
							strTemp = strTemp +str1;
							strTemp = strTemp +it_map->second.Unit+" ";//��λ

							sdd->strdata = str1+it_map->second.Unit;

							m_pSubStation->m_syqdatadiscribelist.push_back(sdd);

							if(j == 6)
								strTemp = strTemp +"\r\n";
						}
					}
					else if(it_map->second.ename == "DRZ")
					{
						strTemp = strTemp + it_map->second.name +":";
						_SYQ_DATA_DESCRIBE *sdd = new _SYQ_DATA_DESCRIBE();

						sdd->strname = it_map->second.name;

						for(int j = 0;j<it_map->second.datalen1;j++)
						{
							str1 ="";
							int t1 = psyqFrame->_asdu.data[23+i+len+j]+psyqFrame->_asdu.data[23+i+len+j+1]*256;
							float f1 = t1*0.01;
							str1.Format("%f",f1);
							strTemp = strTemp +str1;
							strTemp = strTemp +it_map->second.Unit+" ";//��λ

							sdd->strdata = str1+it_map->second.Unit;

							m_pSubStation->m_syqdatadiscribelist.push_back(sdd);

							j++;

							if(j == 12)
								strTemp = strTemp +"\r\n";
						}
					}
					strTemp = strTemp +"\r\n";
				}

			}

		}

		str ="";
		tempmsglen = tempmsglen - len - datalen;
		i = i+len+datalen;
		len = 0; datalen = 0;
	}


	////////////////////

	SYSTEMTIME pt ;
	pt.wYear = psyqFrame->m_DataTimeSend.year;
	pt.wMonth = psyqFrame->m_DataTimeSend.month;
	pt.wDay = psyqFrame->m_DataTimeSend.day;
	pt.wHour = psyqFrame->m_DataTimeSend.hour;
	pt.wMinute = psyqFrame->m_DataTimeSend.min;
	pt.wMilliseconds = psyqFrame->m_DataTimeSend.second*1000;

	UpdateAttribFrame(m_pSubStation,syq_call_data2);
	psyqFrame->m_pStation->InsertDataMessage(strTemp,&pt);//���뻺����У�
	AddDataMessage(psyqFrame->m_pStation, strTemp, &pt);//��ʾ������


	CSYQFrameFormat appFrame;
	m_pSubStation->BuildBasicPacket(appFrame);  //���ɳ�ʼ���·�֡����֡������Ϣ
	//������ݴ��
	//	m_pSubStation->GetSysDataTrans_Syq()->SYQ_EN_COMMON(&appFrame,0x39);

}



void CSysDataTrans_Syq::Process_SYQ_40H(CSYQFrameFormat* psyqFrame)//�޸����ò���1
{
	psyqFrame->m_DataTimeSend.year = 2000+CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[3]);
	psyqFrame->m_DataTimeSend.month = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[4]);
	psyqFrame->m_DataTimeSend.day = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[5]);
	psyqFrame->m_DataTimeSend.hour = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[6]);
	psyqFrame->m_DataTimeSend.min =CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[7]);
	psyqFrame->m_DataTimeSend.second = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[8]);

	int no = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[9])- 3;
	no = (no - 4)/2 +1;

	CString strTemp = "";

	strTemp.Format("ң��վ��:%05d����ʱ�䣺%04d-%02d-%02d %02d:%02d:%02d,�޸�Ŀ���ַ%d�ɹ���",
		psyqFrame->m_stCode,
		psyqFrame->m_DataTimeSend.year,
		psyqFrame->m_DataTimeSend.month,
		psyqFrame->m_DataTimeSend.day,
		psyqFrame->m_DataTimeSend.hour,
		psyqFrame->m_DataTimeSend.min,
		psyqFrame->m_DataTimeSend.second,
		no);
	//////////////////////////////////////////////////////////////////////////
	short data1  = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[10]);
	CString data2 = (CString)&(psyqFrame->_asdu.data[12]);
	if(no == 1)
	{
		m_pSubStation->m_syqipconfigdata.SetIp1(data1,data2);
		UpdateAttribFrame(m_pSubStation,syq_config_address1);
	}
	else if(no == 2)
	{
		m_pSubStation->m_syqipconfigdata.SetIp2(data1,data2);
		UpdateAttribFrame(m_pSubStation,syq_config_address2);
	}
	else if(no == 3)
	{
		m_pSubStation->m_syqipconfigdata.SetIp3(data1,data2);
		UpdateAttribFrame(m_pSubStation,syq_config_address3);
	}
	else if(no == 4)
	{
		m_pSubStation->m_syqipconfigdata.SetIp4(data1,data2);
		UpdateAttribFrame(m_pSubStation,syq_config_address4);
	}
	////////////////////

	SYSTEMTIME pt ;
	pt.wYear = psyqFrame->m_DataTimeSend.year;
	pt.wMonth = psyqFrame->m_DataTimeSend.month;
	pt.wDay = psyqFrame->m_DataTimeSend.day;
	pt.wHour = psyqFrame->m_DataTimeSend.hour;
	pt.wMinute = psyqFrame->m_DataTimeSend.min;
	pt.wMilliseconds = psyqFrame->m_DataTimeSend.second*1000;

	psyqFrame->m_pStation->InsertDataMessage(strTemp,&pt);//���뻺����У�
	AddDataMessage(psyqFrame->m_pStation, strTemp, &pt);//��ʾ������

}
void CSysDataTrans_Syq::Process_SYQ_41H(CSYQFrameFormat* psyqFrame)//��ȡ���ò���1
{
	psyqFrame->m_DataTimeSend.year = 2000+CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[3]);
	psyqFrame->m_DataTimeSend.month = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[4]);
	psyqFrame->m_DataTimeSend.day = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[5]);
	psyqFrame->m_DataTimeSend.hour = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[6]);
	psyqFrame->m_DataTimeSend.min =CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[7]);
	psyqFrame->m_DataTimeSend.second = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[8]);

	int no = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[9])- 3;
	no = (no - 4)/2 +1;

	CString strTemp = "";

	strTemp.Format("ң��վ��:%05d����ʱ�䣺%04d-%02d-%02d %02d:%02d:%02d,��ȡĿ���ַ%d�ɹ���",
		psyqFrame->m_stCode,
		psyqFrame->m_DataTimeSend.year,
		psyqFrame->m_DataTimeSend.month,
		psyqFrame->m_DataTimeSend.day,
		psyqFrame->m_DataTimeSend.hour,
		psyqFrame->m_DataTimeSend.min,
		psyqFrame->m_DataTimeSend.second,
		no);

	//////////////////////////////////////////////////////////////////////////
	short data1  = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[10]);
	CString data2 = (CString)&(psyqFrame->_asdu.data[12]);
	if(no == 1)
	{
		m_pSubStation->m_syqipconfigdata.SetIp1(data1,data2);
		UpdateAttribFrame(m_pSubStation,syq_read_address1);
	}
	else if(no == 2)
	{
		m_pSubStation->m_syqipconfigdata.SetIp2(data1,data2);
		UpdateAttribFrame(m_pSubStation,syq_read_address2);
	}
	else if(no == 3)
	{
		m_pSubStation->m_syqipconfigdata.SetIp3(data1,data2);
		UpdateAttribFrame(m_pSubStation,syq_read_address3);
	}
	else if(no == 4)
	{
		m_pSubStation->m_syqipconfigdata.SetIp4(data1,data2);
		UpdateAttribFrame(m_pSubStation,syq_read_address4);
	}
	////////////////////

	SYSTEMTIME pt ;
	pt.wYear = psyqFrame->m_DataTimeSend.year;
	pt.wMonth = psyqFrame->m_DataTimeSend.month;
	pt.wDay = psyqFrame->m_DataTimeSend.day;
	pt.wHour = psyqFrame->m_DataTimeSend.hour;
	pt.wMinute = psyqFrame->m_DataTimeSend.min;
	pt.wMilliseconds = psyqFrame->m_DataTimeSend.second*1000;

	psyqFrame->m_pStation->InsertDataMessage(strTemp,&pt);//���뻺����У�
	AddDataMessage(psyqFrame->m_pStation, strTemp, &pt);//��ʾ������

}

void CSysDataTrans_Syq::Process_SYQ_42H(CSYQFrameFormat* psyqFrame)//�޸����ò���2
{
	psyqFrame->m_DataTimeSend.year = 2000+CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[3]);
	psyqFrame->m_DataTimeSend.month = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[4]);
	psyqFrame->m_DataTimeSend.day = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[5]);
	psyqFrame->m_DataTimeSend.hour = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[6]);
	psyqFrame->m_DataTimeSend.min =CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[7]);
	psyqFrame->m_DataTimeSend.second = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[8]);

	//	int no = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[9])- 3;
	//	no = (no - 4)/2 +1;

	CString strTemp = "";

	strTemp.Format("ң��վ��:%05d����ʱ�䣺%04d-%02d-%02d %02d:%02d:%02d,�޸����ò���2�ɹ���",
		psyqFrame->m_stCode,
		psyqFrame->m_DataTimeSend.year,
		psyqFrame->m_DataTimeSend.month,
		psyqFrame->m_DataTimeSend.day,
		psyqFrame->m_DataTimeSend.hour,
		psyqFrame->m_DataTimeSend.min,
		psyqFrame->m_DataTimeSend.second);

	//////////////////////////////////////////////////////////////////////////
	_SYQ_CONFIG_PACKET2  syqCfgPack;
	int tempmsglen = psyqFrame->nMsgLen - 9;
	for(int i = 0; i<tempmsglen;)
	{
		int sel = psyqFrame->_asdu.data[9+i];
		switch(sel)
		{
		case 0x20:
			syqCfgPack.CFG_20 = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[10+i]);
			i++;
			break;
		case 0x21:
			syqCfgPack.CFG_21 = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[10+i]);
			i++;
			break;
		case 0x22:
			syqCfgPack.CFG_22 = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[10+i]);
			i++;
			break;
		case 0x23:
			syqCfgPack.CFG_23 = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[10+i]);
			i++;
			break;
		case 0x24:
			syqCfgPack.CFG_24 = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[10+i]);
			i++;
			break;
		case 0x25:
			syqCfgPack.CFG_25 = psyqFrame->_asdu.data[11+i]*256+psyqFrame->_asdu.data[10+i];
			i= i+2;
			break;
		case 0x26:
			syqCfgPack.CFG_26 = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[11+i])*100+CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[10+i]);
			i= i+2;
			break;
		case 0x27:
			syqCfgPack.CFG_27 = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[11+i])*100+CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[10+i]);
			i= i+2;
			break;
		case 0x28:
			syqCfgPack.CFG_28 = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[10+i]);
			i++;
			break;
		case 0x29:
			syqCfgPack.CFG_29 = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[11+i])*100+CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[10+i]);
			i= i+2;
			break;
		case 0x2A:
			syqCfgPack.CFG_2A = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[10+i]);
			i++;
			break;
		case 0x2B:
			syqCfgPack.CFG_2B = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[13+i])*1000000+
				CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[12+i])*10000+
				CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[11+i])*100+
				CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[10+i]);
			i= i+4;
			break;
		case 0x2C:
			syqCfgPack.CFG_2C = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[12+i])*10000+
				CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[11+i])*100+
				CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[10+i]);
			i= i+3;
			break;
		case 0x2D:
			syqCfgPack.CFG_2D = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[10+i]);
			i++;
			break;
		case 0x2E:
			syqCfgPack.CFG_2E = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[14+i])*100000000+
				CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[13+i])*1000000+
				CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[12+i])*10000+
				CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[11+i])*100+
				CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[10+i]);
			i= i+5;
			break;
		case 0x2F:
			syqCfgPack.CFG_2F = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[12+i])*10000+
				CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[11+i])*100+
				CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[10+i]);
			i= i+3;
			break;
		case 0x30:
			syqCfgPack.CFG_30 = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[11+i])*100+CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[10+i]);
			i= i+2;
			break;
		case 0x31:
			syqCfgPack.CFG_31 = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[12+i])*10000+
				CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[11+i])*100+
				CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[10+i]);
			i= i+3;
			break;
		case 0x32:
			syqCfgPack.CFG_32 = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[11+i])*100+CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[10+i]);
			i= i+2;
			break;
		case 0x33:
			syqCfgPack.CFG_33 = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[11+i])*100+CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[10+i]);
			i= i+2;
			break;
		case 0x34:
			syqCfgPack.CFG_34 = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[11+i])*100+CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[10+i]);
			i= i+2;
			break;
		default:
			break;
		}
	}

	m_pSubStation->SetSyqConfig2Data(syqCfgPack);
	UpdateAttribFrame(m_pSubStation,syq_config2_Write);

	////////////////////

	SYSTEMTIME pt ;
	pt.wYear = psyqFrame->m_DataTimeSend.year;
	pt.wMonth = psyqFrame->m_DataTimeSend.month;
	pt.wDay = psyqFrame->m_DataTimeSend.day;
	pt.wHour = psyqFrame->m_DataTimeSend.hour;
	pt.wMinute = psyqFrame->m_DataTimeSend.min;
	pt.wMilliseconds = psyqFrame->m_DataTimeSend.second*1000;

	psyqFrame->m_pStation->InsertDataMessage(strTemp,&pt);//���뻺����У�
	AddDataMessage(psyqFrame->m_pStation, strTemp, &pt);//��ʾ������





}
void CSysDataTrans_Syq::Process_SYQ_43H(CSYQFrameFormat* psyqFrame)//��ȡ���ò���2
{
	psyqFrame->m_DataTimeSend.year = 2000+CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[3]);
	psyqFrame->m_DataTimeSend.month = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[4]);
	psyqFrame->m_DataTimeSend.day = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[5]);
	psyqFrame->m_DataTimeSend.hour = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[6]);
	psyqFrame->m_DataTimeSend.min =CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[7]);
	psyqFrame->m_DataTimeSend.second = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[8]);

	//	int no = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[9])- 3;
	//	no = (no - 4)/2 +1;

	CString strTemp = "";

	strTemp.Format("ң��վ��:%05d����ʱ�䣺%04d-%02d-%02d %02d:%02d:%02d,��ȡ���ò���2�ɹ���",
		psyqFrame->m_stCode,
		psyqFrame->m_DataTimeSend.year,
		psyqFrame->m_DataTimeSend.month,
		psyqFrame->m_DataTimeSend.day,
		psyqFrame->m_DataTimeSend.hour,
		psyqFrame->m_DataTimeSend.min,
		psyqFrame->m_DataTimeSend.second);

	//////////////////////////////////////////////////////////////////////////
	_SYQ_CONFIG_PACKET2  syqCfgPack;
	int tempmsglen = psyqFrame->nMsgLen - 9;
	for(int i = 0; i<tempmsglen;)
	{
		int sel = psyqFrame->_asdu.data[9+i];
		switch(sel)
		{
		case 0x20:
			syqCfgPack.CFG_20 = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[10+i]);
			i++;
			break;
		case 0x21:
			syqCfgPack.CFG_21 = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[10+i]);
			i++;
			break;
		case 0x22:
			syqCfgPack.CFG_22 = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[10+i]);
			i++;
			break;
		case 0x23:
			syqCfgPack.CFG_23 = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[10+i]);
			i++;
			break;
		case 0x24:
			syqCfgPack.CFG_24 = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[10+i]);
			i++;
			break;
		case 0x25:
			syqCfgPack.CFG_25 = psyqFrame->_asdu.data[11+i]*256+psyqFrame->_asdu.data[10+i];
			i= i+2;
			break;
		case 0x26:
			syqCfgPack.CFG_26 = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[11+i])*100+CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[10+i]);
			i= i+2;
			break;
		case 0x27:
			syqCfgPack.CFG_27 = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[11+i])*100+CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[10+i]);
			i= i+2;
			break;
		case 0x28:
			syqCfgPack.CFG_28 = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[10+i]);
			i++;
			break;
		case 0x29:
			syqCfgPack.CFG_29 = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[11+i])*100+CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[10+i]);
			i= i+2;
			break;
		case 0x2A:
			syqCfgPack.CFG_2A = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[10+i]);
			i++;
			break;
		case 0x2B:
			syqCfgPack.CFG_2B = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[13+i])*1000000+
				CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[12+i])*10000+
				CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[11+i])*100+
				CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[10+i]);
			i= i+4;
			break;
		case 0x2C:
			syqCfgPack.CFG_2C = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[12+i])*10000+
				CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[11+i])*100+
				CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[10+i]);
			i= i+3;
			break;
		case 0x2D:
			syqCfgPack.CFG_2D = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[10+i]);
			i++;
			break;
		case 0x2E:
			syqCfgPack.CFG_2E = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[14+i])*100000000+
				CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[13+i])*1000000+
				CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[12+i])*10000+
				CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[11+i])*100+
				CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[10+i]);
			i= i+5;
			break;
		case 0x2F:
			syqCfgPack.CFG_2F = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[12+i])*10000+
				CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[11+i])*100+
				CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[10+i]);
			i= i+3;
			break;
		case 0x30:
			syqCfgPack.CFG_30 = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[11+i])*100+CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[10+i]);
			i= i+2;
			break;
		case 0x31:
			syqCfgPack.CFG_31 = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[12+i])*10000+
				CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[11+i])*100+
				CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[10+i]);
			i= i+3;
			break;
		case 0x32:
			syqCfgPack.CFG_32 = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[11+i])*100+CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[10+i]);
			i= i+2;
			break;
		case 0x33:
			syqCfgPack.CFG_33 = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[11+i])*100+CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[10+i]);
			i= i+2;
			break;
		case 0x34:
			syqCfgPack.CFG_34 = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[11+i])*100+CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[10+i]);
			i= i+2;
			break;
		default:
			break;
		}
	}

	m_pSubStation->SetSyqConfig2Data(syqCfgPack);
	UpdateAttribFrame(m_pSubStation,syq_config2_Read);

	////////////////////

	SYSTEMTIME pt ;
	pt.wYear = psyqFrame->m_DataTimeSend.year;
	pt.wMonth = psyqFrame->m_DataTimeSend.month;
	pt.wDay = psyqFrame->m_DataTimeSend.day;
	pt.wHour = psyqFrame->m_DataTimeSend.hour;
	pt.wMinute = psyqFrame->m_DataTimeSend.min;
	pt.wMilliseconds = psyqFrame->m_DataTimeSend.second*1000;

	psyqFrame->m_pStation->InsertDataMessage(strTemp,&pt);//���뻺����У�
	AddDataMessage(psyqFrame->m_pStation, strTemp, &pt);//��ʾ������

}

void CSysDataTrans_Syq::Process_SYQ_44H(CSYQFrameFormat* psyqFrame)//�޸����ò���2
{
	psyqFrame->m_DataTimeSend.year = 2000+CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[3]);
	psyqFrame->m_DataTimeSend.month = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[4]);
	psyqFrame->m_DataTimeSend.day = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[5]);
	psyqFrame->m_DataTimeSend.hour = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[6]);
	psyqFrame->m_DataTimeSend.min =CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[7]);
	psyqFrame->m_DataTimeSend.second = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[8]);


	CString strTemp = "";

	strTemp.Format("ң��վ��:%05d����ʱ�䣺%04d-%02d-%02d %02d:%02d:%02d,�޸����ò���3�ɹ���",
		psyqFrame->m_stCode,
		psyqFrame->m_DataTimeSend.year,
		psyqFrame->m_DataTimeSend.month,
		psyqFrame->m_DataTimeSend.day,
		psyqFrame->m_DataTimeSend.hour,
		psyqFrame->m_DataTimeSend.min,
		psyqFrame->m_DataTimeSend.second);

	//////////////////////////////////////////////////////////////////////////
	_SYQ_CONFIG_PACKET3  syqCfgPack;
	int tempmsglen = psyqFrame->nMsgLen - 9;
	for(int i = 0; i<tempmsglen;)
	{
		int sel = psyqFrame->_asdu.data[9+i];
		switch(sel)
		{
		case 0x40:
			for(int j = 0; i<10;j++)
				syqCfgPack.CFG_40[j] = psyqFrame->_asdu.data[10+i+j];
			i=i+10;
			break;
		case 0x41:
			for(int j = 0; i<10;j++)
				syqCfgPack.CFG_41[j] = psyqFrame->_asdu.data[10+i+j];
			i=i+10;
			break;
		case 0x69:
			for(int j = 0; i<14;j++)
				syqCfgPack.CFG_69[j] = psyqFrame->_asdu.data[10+i+j];
			i=i+14;
			break;		
		default:
			break;
		}
	}

	m_pSubStation->SetSyqConfig3Data(syqCfgPack);
	UpdateAttribFrame(m_pSubStation,syq_config3_Write);

	////////////////////

	SYSTEMTIME pt ;
	pt.wYear = psyqFrame->m_DataTimeSend.year;
	pt.wMonth = psyqFrame->m_DataTimeSend.month;
	pt.wDay = psyqFrame->m_DataTimeSend.day;
	pt.wHour = psyqFrame->m_DataTimeSend.hour;
	pt.wMinute = psyqFrame->m_DataTimeSend.min;
	pt.wMilliseconds = psyqFrame->m_DataTimeSend.second*1000;

	psyqFrame->m_pStation->InsertDataMessage(strTemp,&pt);//���뻺����У�
	AddDataMessage(psyqFrame->m_pStation, strTemp, &pt);//��ʾ������


}


void CSysDataTrans_Syq::Process_SYQ_45H(CSYQFrameFormat* psyqFrame)//�޸����ò���2
{
	psyqFrame->m_DataTimeSend.year = 2000+CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[3]);
	psyqFrame->m_DataTimeSend.month = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[4]);
	psyqFrame->m_DataTimeSend.day = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[5]);
	psyqFrame->m_DataTimeSend.hour = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[6]);
	psyqFrame->m_DataTimeSend.min =CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[7]);
	psyqFrame->m_DataTimeSend.second = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[8]);


	CString strTemp = "";

	strTemp.Format("ң��վ��:%05d����ʱ�䣺%04d-%02d-%02d %02d:%02d:%02d,��ȡ���ò���3�ɹ���",
		psyqFrame->m_stCode,
		psyqFrame->m_DataTimeSend.year,
		psyqFrame->m_DataTimeSend.month,
		psyqFrame->m_DataTimeSend.day,
		psyqFrame->m_DataTimeSend.hour,
		psyqFrame->m_DataTimeSend.min,
		psyqFrame->m_DataTimeSend.second);

	//////////////////////////////////////////////////////////////////////////
	_SYQ_CONFIG_PACKET3  syqCfgPack;
	int tempmsglen = psyqFrame->nMsgLen - 9;
	for(int i = 0; i<tempmsglen;)
	{
		int sel = psyqFrame->_asdu.data[9+i];
		switch(sel)
		{
		case 0x40:
			for(int j = 0; i<10;j++)
				syqCfgPack.CFG_40[j] = psyqFrame->_asdu.data[10+i+j];
			i=i+10;
			break;
		case 0x41:
			for(int j = 0; i<10;j++)
				syqCfgPack.CFG_41[j] = psyqFrame->_asdu.data[10+i+j];
			i=i+10;
			break;
		case 0x69:
			for(int j = 0; i<14;j++)
				syqCfgPack.CFG_69[j] = psyqFrame->_asdu.data[10+i+j];
			i=i+14;
			break;		
		default:
			break;
		}
	}

	m_pSubStation->SetSyqConfig3Data(syqCfgPack);
	UpdateAttribFrame(m_pSubStation,syq_config3_Write);

	////////////////////

	SYSTEMTIME pt ;
	pt.wYear = psyqFrame->m_DataTimeSend.year;
	pt.wMonth = psyqFrame->m_DataTimeSend.month;
	pt.wDay = psyqFrame->m_DataTimeSend.day;
	pt.wHour = psyqFrame->m_DataTimeSend.hour;
	pt.wMinute = psyqFrame->m_DataTimeSend.min;
	pt.wMilliseconds = psyqFrame->m_DataTimeSend.second*1000;

	psyqFrame->m_pStation->InsertDataMessage(strTemp,&pt);//���뻺����У�
	AddDataMessage(psyqFrame->m_pStation, strTemp, &pt);//��ʾ������


}


void CSysDataTrans_Syq::Process_SYQ_46H(CSYQFrameFormat* psyqFrame)
{
	psyqFrame->m_DataTimeSend.year = 2000+CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[2]);
	psyqFrame->m_DataTimeSend.month = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[3]);
	psyqFrame->m_DataTimeSend.day = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[4]);
	psyqFrame->m_DataTimeSend.hour = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[5]);
	psyqFrame->m_DataTimeSend.min =CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[6]);
	psyqFrame->m_DataTimeSend.second = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[7]);


	CString strTemp = "";

	strTemp.Format("ң��վ��:%05d����ʱ�䣺%04d-%02d-%02d %02d:%02d:%02d,�޸�����ɹ���",
		psyqFrame->m_stCode,
		psyqFrame->m_DataTimeSend.year,
		psyqFrame->m_DataTimeSend.month,
		psyqFrame->m_DataTimeSend.day,
		psyqFrame->m_DataTimeSend.hour,
		psyqFrame->m_DataTimeSend.min,
		psyqFrame->m_DataTimeSend.second);
	//////////////////////////////////////////////////////////////////////////
	//data δ����

	if(psyqFrame->_asdu.data[8] == 0x03)//����
	{
		m_pSubStation->m_iPassword = psyqFrame->_asdu.data[10]*256+psyqFrame->_asdu.data[11];

	}
	////////////////////

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	CRealTimeDB* pRealDb = pApp->m_pReadDB;
	if (!pRealDb)	return ;


	RTU_STATION info;
	info._strSId = m_pSubStation->GetID();
	info._bCmType = m_pSubStation->m_CmType;
	info._bCountyCode = m_pSubStation->m_CountyCode;
	info._bInterface = m_pSubStation->m_Interface;
	info._bProvinceCode = m_pSubStation->m_ProvinceCode;
	info._bRegionCode = m_pSubStation->m_RegionCode;
	info._CmComInfo.mBaund = m_pSubStation->GetBaundA();
	info._CmComInfo.mComName = m_pSubStation->GetComPortA();
	info._CmComInfo.mDataBits = m_pSubStation->GetDataBitsA();
	info._CmComInfo.mParity = m_pSubStation->GetParityA();
	info._CmComInfo.mStopBits = m_pSubStation->GetStopBitsA();
	info._CmNetInfo.mIpAddress = m_pSubStation->GetIpAddrA();
	info._CmNetInfo.mPort = m_pSubStation->GetSockPortA();
	info._nAddrA1  = (info._bProvinceCode * 100 + info._bRegionCode);
	info._nAddrA2  = m_pSubStation->GetAddrA2();
	info._nAddrA3  = m_pSubStation->GetAddrA3();
	info._strName = m_pSubStation->GetName();

	info._CmScCommInfo.nFactoryType = m_pSubStation->GetScFactoryType();
	info._CmScCommInfo.nScAddress = m_pSubStation->GetScAddress();
	info._nAppProVer = m_pSubStation->GetAppProVer();
	info._nWorkMode = m_pSubStation->GetWorkMode();
	info._bAppProType = m_pSubStation->GetAppProtocolType();
	info._strStCode = m_pSubStation->GetStringStCode();
	info._centercode = m_pSubStation->GetSYQCenterCode();
	info._strSYQADDR = m_pSubStation->GetSYQStCode();
	info._passWord = m_pSubStation->GetSYQPassword();

	if (m_pSubStation->CheckModify(&info) == true )
	{
		pRealDb->ModifyStInfo(&info,m_pSubStation);
	}


	SYSTEMTIME pt ;
	pt.wYear = psyqFrame->m_DataTimeSend.year;
	pt.wMonth = psyqFrame->m_DataTimeSend.month;
	pt.wDay = psyqFrame->m_DataTimeSend.day;
	pt.wHour = psyqFrame->m_DataTimeSend.hour;
	pt.wMinute = psyqFrame->m_DataTimeSend.min;
	pt.wMilliseconds = psyqFrame->m_DataTimeSend.second*1000;

	UpdateAttribFrame(m_pSubStation,syq_config_password);
	psyqFrame->m_pStation->InsertDataMessage(strTemp,&pt);//���뻺����У�
	AddDataMessage(psyqFrame->m_pStation, strTemp, &pt);//��ʾ������

}
void CSysDataTrans_Syq::Process_SYQ_47H(CSYQFrameFormat* psyqFrame)
{
	psyqFrame->m_DataTimeSend.year = 2000+CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[2]);
	psyqFrame->m_DataTimeSend.month = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[3]);
	psyqFrame->m_DataTimeSend.day = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[4]);
	psyqFrame->m_DataTimeSend.hour = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[5]);
	psyqFrame->m_DataTimeSend.min =CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[6]);
	psyqFrame->m_DataTimeSend.second = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[7]);


	CString strTemp = "";

	strTemp.Format("ң��վ��:%05d����ʱ�䣺%04d-%02d-%02d %02d:%02d:%02d,��ʼ����̬�洢���ݳɹ���",
		psyqFrame->m_stCode,
		psyqFrame->m_DataTimeSend.year,
		psyqFrame->m_DataTimeSend.month,
		psyqFrame->m_DataTimeSend.day,
		psyqFrame->m_DataTimeSend.hour,
		psyqFrame->m_DataTimeSend.min,
		psyqFrame->m_DataTimeSend.second);
	//////////////////////////////////////////////////////////////////////////

	SYSTEMTIME pt ;
	pt.wYear = psyqFrame->m_DataTimeSend.year;
	pt.wMonth = psyqFrame->m_DataTimeSend.month;
	pt.wDay = psyqFrame->m_DataTimeSend.day;
	pt.wHour = psyqFrame->m_DataTimeSend.hour;
	pt.wMinute = psyqFrame->m_DataTimeSend.min;
	pt.wMilliseconds = psyqFrame->m_DataTimeSend.second*1000;

	UpdateAttribFrame(m_pSubStation,syq_init_memory);
	psyqFrame->m_pStation->InsertDataMessage(strTemp,&pt);//���뻺����У�
	AddDataMessage(psyqFrame->m_pStation, strTemp, &pt);//��ʾ������

}
void CSysDataTrans_Syq::Process_SYQ_48H(CSYQFrameFormat* psyqFrame)//ң��վ״̬�ͱ���
{
	psyqFrame->m_DataTimeSend.year = 2000+CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[2]);
	psyqFrame->m_DataTimeSend.month = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[3]);
	psyqFrame->m_DataTimeSend.day = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[4]);
	psyqFrame->m_DataTimeSend.hour = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[5]);
	psyqFrame->m_DataTimeSend.min =CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[6]);
	psyqFrame->m_DataTimeSend.second = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[7]);


	CString strTemp = "";

	strTemp.Format("ң��վ��:%05d����ʱ�䣺%04d-%02d-%02d %02d:%02d:%02d,��ѯң��վ״̬�ͱ�����Ϣ�ɹ���",
		psyqFrame->m_stCode,
		psyqFrame->m_DataTimeSend.year,
		psyqFrame->m_DataTimeSend.month,
		psyqFrame->m_DataTimeSend.day,
		psyqFrame->m_DataTimeSend.hour,
		psyqFrame->m_DataTimeSend.min,
		psyqFrame->m_DataTimeSend.second);
	//////////////////////////////////////////////////////////////////////////

	SYSTEMTIME pt ;
	pt.wYear = psyqFrame->m_DataTimeSend.year;
	pt.wMonth = psyqFrame->m_DataTimeSend.month;
	pt.wDay = psyqFrame->m_DataTimeSend.day;
	pt.wHour = psyqFrame->m_DataTimeSend.hour;
	pt.wMinute = psyqFrame->m_DataTimeSend.min;
	pt.wMilliseconds = psyqFrame->m_DataTimeSend.second*1000;

	m_pSubStation->m_syqState = psyqFrame->_asdu.data[9]*256+psyqFrame->_asdu.data[8];

	UpdateAttribFrame(m_pSubStation,syq_call_state);
	psyqFrame->m_pStation->InsertDataMessage(strTemp,&pt);//���뻺����У�
	AddDataMessage(psyqFrame->m_pStation, strTemp, &pt);//��ʾ������
}
void CSysDataTrans_Syq::Process_SYQ_49H(CSYQFrameFormat* psyqFrame)
{
	psyqFrame->m_DataTimeSend.year = 2000+CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[2]);
	psyqFrame->m_DataTimeSend.month = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[3]);
	psyqFrame->m_DataTimeSend.day = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[4]);
	psyqFrame->m_DataTimeSend.hour = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[5]);
	psyqFrame->m_DataTimeSend.min =CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[6]);
	psyqFrame->m_DataTimeSend.second = CUdfGlobalFUNC::ByteFromBCD(psyqFrame->_asdu.data[7]);

	CString strTemp = "";

	strTemp.Format("ң��վ��:%05d����ʱ�䣺%04d-%02d-%02d %02d:%02d:%02d,���ӳɹ���",
		psyqFrame->m_stCode,
		psyqFrame->m_DataTimeSend.year,
		psyqFrame->m_DataTimeSend.month,
		psyqFrame->m_DataTimeSend.day,
		psyqFrame->m_DataTimeSend.hour,
		psyqFrame->m_DataTimeSend.min,
		psyqFrame->m_DataTimeSend.second);
	//////////////////////////////////////////////////////////////////////////

	SYSTEMTIME pt ;
	pt.wYear = psyqFrame->m_DataTimeSend.year;
	pt.wMonth = psyqFrame->m_DataTimeSend.month;
	pt.wDay = psyqFrame->m_DataTimeSend.day;
	pt.wHour = psyqFrame->m_DataTimeSend.hour;
	pt.wMinute = psyqFrame->m_DataTimeSend.min;
	pt.wMilliseconds = psyqFrame->m_DataTimeSend.second*1000;

	UpdateAttribFrame(m_pSubStation,syq_check_time);
	psyqFrame->m_pStation->InsertDataMessage(strTemp,&pt);//���뻺����У�
	AddDataMessage(psyqFrame->m_pStation, strTemp, &pt);//��ʾ������

}
void CSysDataTrans_Syq::SYQ_EN_COMMON(CSYQFrameFormat* appFrame,int type,BOOL bRepeat)//0x30 ---0x36  0x39
{
	if (!appFrame)   return;
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));


	SYSTEMTIME t;
	GetLocalTime(&t);

	appFrame->m_FunType = type;

	appFrame->_asdu.data.clear();
	appFrame->_asdu.data = std::vector<BYTE>(30);
	appFrame->_asdu.data[0] ='T';
	appFrame->_asdu.data[1] ='X';

	appFrame->_asdu.data[2] =  CUdfGlobalFUNC::ByteToBCD((BYTE)(t.wYear-2000));
	appFrame->_asdu.data[3] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wMonth);
	appFrame->_asdu.data[4] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wDay);
	appFrame->_asdu.data[5] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wHour);
	appFrame->_asdu.data[6] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wMinute);
	appFrame->_asdu.data[7] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wSecond);

	appFrame->nMsgLen = 8;

	BYTE Len = 0;
	BuildSendBuffer(appFrame,sBuf,Len,appFrame->nMsgLen);
	SendRtuLinkFrame(sBuf,Len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	if(bRepeat)
		AddReSendObject(sBuf,Len,appFrame);
}



void CSysDataTrans_Syq::SYQ_EN_37H(CSYQFrameFormat* appFrame,CString yaosustr)//��ѯʱ������
{
	if (!appFrame)   return;
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));


	SYSTEMTIME t;
	GetLocalTime(&t);

	appFrame->m_FunType = 0x38;

	appFrame->_asdu.data.clear();
	appFrame->_asdu.data = std::vector<BYTE>(30);
	appFrame->_asdu.data[0] ='T';
	appFrame->_asdu.data[1] ='X';

	appFrame->_asdu.data[2] =  CUdfGlobalFUNC::ByteToBCD((BYTE)(t.wYear-2000));
	appFrame->_asdu.data[3] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wMonth);
	appFrame->_asdu.data[4] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wDay);
	appFrame->_asdu.data[5] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wHour);
	appFrame->_asdu.data[6] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wMinute);
	appFrame->_asdu.data[7] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wSecond);

	int Varlen = yaosustr.GetLength();
	char Buf[100];
	memset(Buf, 0, 100);
	memcpy(Buf, yaosustr.GetBuffer(100), 100);

	for (int i=0; i<Varlen; i++)
	{
		appFrame->_asdu.data[8+i] = Buf[i];

	}


	appFrame->nMsgLen = 8+Varlen;

	BYTE Len = 0;
	BuildSendBuffer(appFrame,sBuf,Len,appFrame->nMsgLen);
	SendRtuLinkFrame(sBuf,Len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	AddReSendObject(sBuf,Len,appFrame);

}
void CSysDataTrans_Syq::SYQ_EN_38H(CSYQFrameFormat* appFrame,CString yaosustr,int timetype,int ntime ,CTime starttime, CTime endtime)//��ѯʱ������
{
	if (!appFrame)   return;
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));


	SYSTEMTIME t;
	GetLocalTime(&t);

	appFrame->m_FunType = 0x38;

	appFrame->_asdu.data.clear();
	appFrame->_asdu.data = std::vector<BYTE>(30);
	appFrame->_asdu.data[0] ='T';
	appFrame->_asdu.data[1] ='X';

	appFrame->_asdu.data[2] =  CUdfGlobalFUNC::ByteToBCD((BYTE)(t.wYear-2000));
	appFrame->_asdu.data[3] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wMonth);
	appFrame->_asdu.data[4] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wDay);
	appFrame->_asdu.data[5] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wHour);
	appFrame->_asdu.data[6] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wMinute);
	appFrame->_asdu.data[7] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wSecond);

	appFrame->_asdu.data[8] =  'D';
	appFrame->_asdu.data[9] =  'R';

	if(timetype == 0)
		appFrame->_asdu.data[10] =  'D';
	else if(timetype == 1)
		appFrame->_asdu.data[10] =  'H';
	else if(timetype == 0)
		appFrame->_asdu.data[10] =  'N';

	char tempByte[3];
	sprintf_s(tempByte,"%d",ntime);
	if(ntime <10)
	{
		appFrame->_asdu.data[11] = '0';
		appFrame->_asdu.data[12] = tempByte[0];
	}
	else
	{
		appFrame->_asdu.data[11] = tempByte[0];
		appFrame->_asdu.data[12] = tempByte[1];
	}



	int Varlen = yaosustr.GetLength();
	char Buf[5];
	memset(Buf, 0, 5);
	memcpy(Buf, yaosustr.GetBuffer(5), 5);

	for (int i=0; i<Varlen; i++)
	{
		appFrame->_asdu.data[13+i] = Buf[i];

	}

	appFrame->_asdu.data[13+Varlen] = CUdfGlobalFUNC::ByteToBCD((BYTE)(starttime.GetYear() -2000));
	appFrame->_asdu.data[14+Varlen] = CUdfGlobalFUNC::ByteToBCD((BYTE)(starttime.GetMonth()));
	appFrame->_asdu.data[15+Varlen] = CUdfGlobalFUNC::ByteToBCD((BYTE)(starttime.GetDay()));
	appFrame->_asdu.data[16+Varlen] = CUdfGlobalFUNC::ByteToBCD((BYTE)(starttime.GetHour()));


	appFrame->_asdu.data[17+Varlen] = CUdfGlobalFUNC::ByteToBCD((BYTE)(endtime.GetYear() -2000));
	appFrame->_asdu.data[18+Varlen] = CUdfGlobalFUNC::ByteToBCD((BYTE)(endtime.GetMonth()));
	appFrame->_asdu.data[19+Varlen] = CUdfGlobalFUNC::ByteToBCD((BYTE)(endtime.GetDay()));
	appFrame->_asdu.data[20+Varlen] = CUdfGlobalFUNC::ByteToBCD((BYTE)(endtime.GetHour()));


	appFrame->nMsgLen = 21+Varlen;

	BYTE Len = 0;
	BuildSendBuffer(appFrame,sBuf,Len,appFrame->nMsgLen);
	SendRtuLinkFrame(sBuf,Len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	AddReSendObject(sBuf,Len,appFrame);

}
void CSysDataTrans_Syq::SYQ_EN_40H(CSYQFrameFormat* appFrame,CString stradd,short comtype,short no)
{
	if (!appFrame)   return;
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));


	SYSTEMTIME t;
	GetLocalTime(&t);

	int Varlen = stradd.GetLength();
	char Buf[20];
	memset(Buf, 0, 19);
	memcpy(Buf, stradd.GetBuffer(19), 19);


	for (int i=0; i<Varlen; i++)
	{
		appFrame->_asdu.data[11+i] = Buf[i];
	}
	appFrame->nMsgLen = 11 + Varlen;

	appFrame->m_FunType = 0x40;

	appFrame->_asdu.data.clear();
	appFrame->_asdu.data = std::vector<BYTE>(appFrame->nMsgLen);
	appFrame->_asdu.data[0] ='T';
	appFrame->_asdu.data[1] ='X';

	appFrame->_asdu.data[2] =  CUdfGlobalFUNC::ByteToBCD((BYTE)(t.wYear-2000));
	appFrame->_asdu.data[3] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wMonth);
	appFrame->_asdu.data[4] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wDay);
	appFrame->_asdu.data[5] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wHour);
	appFrame->_asdu.data[6] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wMinute);
	appFrame->_asdu.data[7] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wSecond);
	appFrame->_asdu.data[8] = no + 3 +(no -1);
	appFrame->_asdu.data[9] = comtype;
	appFrame->_asdu.data[10] = no + 4 +(no -1);



	BYTE Len = 0;
	BuildSendBuffer(appFrame,sBuf,Len,appFrame->nMsgLen);
	SendRtuLinkFrame(sBuf,Len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	AddReSendObject(sBuf,Len,appFrame);
}




void CSysDataTrans_Syq::SYQ_EN_41H(CSYQFrameFormat* appFrame,CString stradd,short comtype,short no)
{
	if (!appFrame)   return;
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));


	SYSTEMTIME t;
	GetLocalTime(&t);


	int Varlen = stradd.GetLength();
	char Buf[20];
	memset(Buf, 0, 19);
	memcpy(Buf, stradd.GetBuffer(19), 19);


	for (int i=0; i<Varlen; i++)
	{
		appFrame->_asdu.data[11+i] = Buf[i];
	}
	appFrame->nMsgLen = 11 + Varlen;

	appFrame->m_FunType = 0x41;

	appFrame->_asdu.data.clear();
	appFrame->_asdu.data = std::vector<BYTE>(appFrame->nMsgLen);
	appFrame->_asdu.data[0] ='T';
	appFrame->_asdu.data[1] ='X';

	appFrame->_asdu.data[2] =  CUdfGlobalFUNC::ByteToBCD((BYTE)(t.wYear-2000));
	appFrame->_asdu.data[3] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wMonth);
	appFrame->_asdu.data[4] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wDay);
	appFrame->_asdu.data[5] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wHour);
	appFrame->_asdu.data[6] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wMinute);
	appFrame->_asdu.data[7] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wSecond);
	appFrame->_asdu.data[8] = no + 3 +(no -1);
	appFrame->_asdu.data[9] = comtype;
	appFrame->_asdu.data[10] = no + 4 +(no -1);



	BYTE Len = 0;
	BuildSendBuffer(appFrame,sBuf,Len,appFrame->nMsgLen);
	SendRtuLinkFrame(sBuf,Len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	AddReSendObject(sBuf,Len,appFrame);
}




void CSysDataTrans_Syq::SYQ_EN_42H(CSYQFrameFormat* appFrame,_SYQ_CONFIG_PACKET2 packet)
{
	if (!appFrame)   return;
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));


	SYSTEMTIME t;
	GetLocalTime(&t);

	appFrame->m_FunType = 0x42;

	appFrame->_asdu.data.clear();
	appFrame->_asdu.data = std::vector<BYTE>(52);
	appFrame->_asdu.data[0] ='T';
	appFrame->_asdu.data[1] ='X';

	appFrame->_asdu.data[2] =  CUdfGlobalFUNC::ByteToBCD((BYTE)(t.wYear-2000));
	appFrame->_asdu.data[3] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wMonth);
	appFrame->_asdu.data[4] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wDay);
	appFrame->_asdu.data[5] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wHour);
	appFrame->_asdu.data[6] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wMinute);
	appFrame->_asdu.data[7] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wSecond);
	appFrame->_asdu.data[8] =  0x20;
	appFrame->_asdu.data[9] = CUdfGlobalFUNC::ByteToBCD(packet.CFG_20);
	appFrame->_asdu.data[10] =  0x21;
	appFrame->_asdu.data[11] = CUdfGlobalFUNC::ByteToBCD(packet.CFG_21);
	appFrame->_asdu.data[12] =  0x22;
	appFrame->_asdu.data[13] = CUdfGlobalFUNC::ByteToBCD(packet.CFG_22);
	appFrame->_asdu.data[14] =  0x23;
	appFrame->_asdu.data[15] = CUdfGlobalFUNC::ByteToBCD(packet.CFG_23);
	appFrame->_asdu.data[16] =  0x24;
	appFrame->_asdu.data[17] = CUdfGlobalFUNC::ByteToBCD(packet.CFG_24);

	appFrame->_asdu.data[18] =  0x25;
	appFrame->_asdu.data[19] = packet.CFG_25%256;
	appFrame->_asdu.data[20] = packet.CFG_25/256;

	BYTE tempByte5[5],tempByte4[4],tempByte3[3],tempByte2[2];
	DWORD temp = 0; 
	memset(tempByte5,0,sizeof(tempByte5));
	memset(tempByte4,0,sizeof(tempByte4));
	memset(tempByte3,0,sizeof(tempByte3));
	memset(tempByte2,0,sizeof(tempByte2));

	temp = (DWORD)packet.CFG_26;
	CUdfGlobalFUNC::ToBCD(temp,tempByte2,2);
	appFrame->_asdu.data[21] =  0x26;
	appFrame->_asdu.data[22] = tempByte2[0];
	appFrame->_asdu.data[23] = tempByte2[1];

	temp = (DWORD)packet.CFG_27;
	CUdfGlobalFUNC::ToBCD(temp,tempByte2,2);
	appFrame->_asdu.data[24] =  0x27;
	appFrame->_asdu.data[25] = tempByte2[0];
	appFrame->_asdu.data[26] = tempByte2[1];

	appFrame->_asdu.data[27] =  0x28;
	appFrame->_asdu.data[28] = CUdfGlobalFUNC::ByteToBCD(packet.CFG_28);

	temp = (DWORD)packet.CFG_29;
	CUdfGlobalFUNC::ToBCD(temp,tempByte2,2);
	appFrame->_asdu.data[29] =  0x29;
	appFrame->_asdu.data[30] = tempByte2[0];
	appFrame->_asdu.data[31] = tempByte2[1];


	temp = (DWORD)packet.CFG_2B;
	CUdfGlobalFUNC::ToBCD(temp,tempByte4,4);
	appFrame->_asdu.data[32] =  0x2B;
	for (int i = 0; i<4;i++)
		appFrame->_asdu.data[33+i] = tempByte4[i];

	temp = (DWORD)packet.CFG_2C;
	CUdfGlobalFUNC::ToBCD(temp,tempByte3,3);
	appFrame->_asdu.data[37] =  0x2C;
	for (int i = 0; i<3;i++)
		appFrame->_asdu.data[38+i] = tempByte3[i];

	appFrame->_asdu.data[41] =  0x2D;
	appFrame->_asdu.data[42] = CUdfGlobalFUNC::ByteToBCD(packet.CFG_2D);

	temp = (DWORD)packet.CFG_2E;
	CUdfGlobalFUNC::ToBCD(temp,tempByte5,5);
	appFrame->_asdu.data[43] =  0x2E;
	for (int i = 0; i<5;i++)
		appFrame->_asdu.data[44+i] = tempByte5[i];


	temp = (DWORD)packet.CFG_2F;
	CUdfGlobalFUNC::ToBCD(temp,tempByte3,3);
	appFrame->_asdu.data[49] =  0x2F;
	for (int i = 0; i<3;i++)
		appFrame->_asdu.data[49+i] = tempByte3[i];


	/*	int Varlen = stradd.GetLength();
	char Buf[20];
	memset(Buf, 0, 19);
	memcpy(Buf, stradd.GetBuffer(19), 19);
	*/


	appFrame->nMsgLen = 52;

	BYTE Len = 0;
	BuildSendBuffer(appFrame,sBuf,Len,appFrame->nMsgLen);
	SendRtuLinkFrame(sBuf,Len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	AddReSendObject(sBuf,Len,appFrame);
}
void CSysDataTrans_Syq::SYQ_EN_43H(CSYQFrameFormat* appFrame,_SYQ_CONFIG_PACKET2 packet)
{
	if (!appFrame)   return;
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));


	SYSTEMTIME t;
	GetLocalTime(&t);

	appFrame->m_FunType = 0x43;

	appFrame->_asdu.data.clear();
	appFrame->_asdu.data = std::vector<BYTE>(52);
	appFrame->_asdu.data[0] ='T';
	appFrame->_asdu.data[1] ='X';

	appFrame->_asdu.data[2] =  CUdfGlobalFUNC::ByteToBCD((BYTE)(t.wYear-2000));
	appFrame->_asdu.data[3] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wMonth);
	appFrame->_asdu.data[4] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wDay);
	appFrame->_asdu.data[5] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wHour);
	appFrame->_asdu.data[6] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wMinute);
	appFrame->_asdu.data[7] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wSecond);
	appFrame->_asdu.data[8] =  0x20;
	appFrame->_asdu.data[9] = CUdfGlobalFUNC::ByteToBCD(packet.CFG_20);
	appFrame->_asdu.data[10] =  0x21;
	appFrame->_asdu.data[11] = CUdfGlobalFUNC::ByteToBCD(packet.CFG_21);
	appFrame->_asdu.data[12] =  0x22;
	appFrame->_asdu.data[13] = CUdfGlobalFUNC::ByteToBCD(packet.CFG_22);
	appFrame->_asdu.data[14] =  0x23;
	appFrame->_asdu.data[15] = CUdfGlobalFUNC::ByteToBCD(packet.CFG_23);
	appFrame->_asdu.data[16] =  0x24;
	appFrame->_asdu.data[17] = CUdfGlobalFUNC::ByteToBCD(packet.CFG_24);

	appFrame->_asdu.data[18] =  0x25;
	appFrame->_asdu.data[19] = packet.CFG_25%256;
	appFrame->_asdu.data[20] = packet.CFG_25/256;

	BYTE tempByte5[5],tempByte4[4],tempByte3[3],tempByte2[2];
	DWORD temp = 0; 
	memset(tempByte5,0,sizeof(tempByte5));
	memset(tempByte4,0,sizeof(tempByte4));
	memset(tempByte3,0,sizeof(tempByte3));
	memset(tempByte2,0,sizeof(tempByte2));

	temp = (DWORD)packet.CFG_26;
	CUdfGlobalFUNC::ToBCD(temp,tempByte2,2);
	appFrame->_asdu.data[21] =  0x26;
	appFrame->_asdu.data[22] = tempByte2[0];
	appFrame->_asdu.data[23] = tempByte2[1];

	temp = (DWORD)packet.CFG_27;
	CUdfGlobalFUNC::ToBCD(temp,tempByte2,2);
	appFrame->_asdu.data[24] =  0x27;
	appFrame->_asdu.data[25] = tempByte2[0];
	appFrame->_asdu.data[26] = tempByte2[1];

	appFrame->_asdu.data[27] =  0x28;
	appFrame->_asdu.data[28] = CUdfGlobalFUNC::ByteToBCD(packet.CFG_28);

	temp = (DWORD)packet.CFG_29;
	CUdfGlobalFUNC::ToBCD(temp,tempByte2,2);
	appFrame->_asdu.data[29] =  0x29;
	appFrame->_asdu.data[30] = tempByte2[0];
	appFrame->_asdu.data[31] = tempByte2[1];


	temp = (DWORD)packet.CFG_2B;
	CUdfGlobalFUNC::ToBCD(temp,tempByte4,4);
	appFrame->_asdu.data[32] =  0x2B;
	for (int i = 0; i<4;i++)
		appFrame->_asdu.data[33+i] = tempByte4[i];

	temp = (DWORD)packet.CFG_2C;
	CUdfGlobalFUNC::ToBCD(temp,tempByte3,3);
	appFrame->_asdu.data[37] =  0x2C;
	for (int i = 0; i<3;i++)
		appFrame->_asdu.data[38+i] = tempByte3[i];

	appFrame->_asdu.data[41] =  0x2D;
	appFrame->_asdu.data[42] = CUdfGlobalFUNC::ByteToBCD(packet.CFG_2D);

	temp = (DWORD)packet.CFG_2E;
	CUdfGlobalFUNC::ToBCD(temp,tempByte5,5);
	appFrame->_asdu.data[43] =  0x2E;
	for (int i = 0; i<5;i++)
		appFrame->_asdu.data[44+i] = tempByte5[i];


	temp = (DWORD)packet.CFG_2F;
	CUdfGlobalFUNC::ToBCD(temp,tempByte3,3);
	appFrame->_asdu.data[49] =  0x2F;
	for (int i = 0; i<3;i++)
		appFrame->_asdu.data[49+i] = tempByte3[i];


	/*	int Varlen = stradd.GetLength();
	char Buf[20];
	memset(Buf, 0, 19);
	memcpy(Buf, stradd.GetBuffer(19), 19);
	*/


	appFrame->nMsgLen = 52;

	BYTE Len = 0;
	BuildSendBuffer(appFrame,sBuf,Len,appFrame->nMsgLen);
	SendRtuLinkFrame(sBuf,Len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	AddReSendObject(sBuf,Len,appFrame);
}
void CSysDataTrans_Syq::SYQ_EN_44H(CSYQFrameFormat* appFrame,_SYQ_CONFIG_PACKET3 packet)
{
	if (!appFrame)   return;
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));


	SYSTEMTIME t;
	GetLocalTime(&t);

	appFrame->m_FunType = 0x44;

	appFrame->_asdu.data.clear();
	appFrame->_asdu.data = std::vector<BYTE>(52);
	appFrame->_asdu.data[0] ='T';
	appFrame->_asdu.data[1] ='X';

	appFrame->_asdu.data[2] =  CUdfGlobalFUNC::ByteToBCD((BYTE)(t.wYear-2000));
	appFrame->_asdu.data[3] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wMonth);
	appFrame->_asdu.data[4] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wDay);
	appFrame->_asdu.data[5] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wHour);
	appFrame->_asdu.data[6] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wMinute);
	appFrame->_asdu.data[7] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wSecond);
	appFrame->_asdu.data[8] =  0x40;
	for(int i=0;i<10;i++)
		appFrame->_asdu.data[9+i] = packet.CFG_40[i];

	appFrame->_asdu.data[19] =  0x41;
	for(int i=0;i<10;i++)
		appFrame->_asdu.data[20+i] = packet.CFG_41[i];

	appFrame->_asdu.data[30] =  0x69;
	for(int i=0;i<14;i++)
		appFrame->_asdu.data[31+i] = packet.CFG_69[i];	

	appFrame->nMsgLen = 45;

	BYTE Len = 0;
	BuildSendBuffer(appFrame,sBuf,Len,appFrame->nMsgLen);
	SendRtuLinkFrame(sBuf,Len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	AddReSendObject(sBuf,Len,appFrame);
}
void CSysDataTrans_Syq::SYQ_EN_45H(CSYQFrameFormat* appFrame,_SYQ_CONFIG_PACKET3 packet)
{
	if (!appFrame)   return;
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));


	SYSTEMTIME t;
	GetLocalTime(&t);

	appFrame->m_FunType = 0x45;

	appFrame->_asdu.data.clear();
	appFrame->_asdu.data = std::vector<BYTE>(52);
	appFrame->_asdu.data[0] ='T';
	appFrame->_asdu.data[1] ='X';

	appFrame->_asdu.data[2] =  CUdfGlobalFUNC::ByteToBCD((BYTE)(t.wYear-2000));
	appFrame->_asdu.data[3] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wMonth);
	appFrame->_asdu.data[4] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wDay);
	appFrame->_asdu.data[5] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wHour);
	appFrame->_asdu.data[6] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wMinute);
	appFrame->_asdu.data[7] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wSecond);
	appFrame->_asdu.data[8] =  0x40;
	for(int i=0;i<10;i++)
		appFrame->_asdu.data[9+i] = packet.CFG_40[i];

	appFrame->_asdu.data[19] =  0x41;
	for(int i=0;i<10;i++)
		appFrame->_asdu.data[20+i] = packet.CFG_41[i];

	appFrame->_asdu.data[30] =  0x69;
	for(int i=0;i<14;i++)
		appFrame->_asdu.data[31+i] = packet.CFG_69[i];	

	appFrame->nMsgLen = 45;

	BYTE Len = 0;
	BuildSendBuffer(appFrame,sBuf,Len,appFrame->nMsgLen);
	SendRtuLinkFrame(sBuf,Len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	AddReSendObject(sBuf,Len,appFrame);
}
void CSysDataTrans_Syq::SYQ_EN_46H(CSYQFrameFormat* appFrame,int password)//�޸�����
{
	if (!appFrame)   return;
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));


	SYSTEMTIME t;
	GetLocalTime(&t);

	appFrame->m_FunType = 0x46;

	appFrame->_asdu.data.clear();
	appFrame->_asdu.data = std::vector<BYTE>(11);
	appFrame->_asdu.data[0] ='T';
	appFrame->_asdu.data[1] ='X';

	appFrame->_asdu.data[2] =  CUdfGlobalFUNC::ByteToBCD((BYTE)(t.wYear-2000));
	appFrame->_asdu.data[3] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wMonth);
	appFrame->_asdu.data[4] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wDay);
	appFrame->_asdu.data[5] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wHour);
	appFrame->_asdu.data[6] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wMinute);
	appFrame->_asdu.data[7] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wSecond);
	appFrame->_asdu.data[8] =  0x03;
	appFrame->_asdu.data[9] =	password/256;
	appFrame->_asdu.data[10] = password %256;


	appFrame->nMsgLen = 11;

	BYTE Len = 0;
	BuildSendBuffer(appFrame,sBuf,Len,appFrame->nMsgLen);
	SendRtuLinkFrame(sBuf,Len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	AddReSendObject(sBuf,Len,appFrame);
}
void CSysDataTrans_Syq::SYQ_EN_47H(CSYQFrameFormat* appFrame)
{
	if (!appFrame)   return;
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));


	SYSTEMTIME t;
	GetLocalTime(&t);

	appFrame->m_FunType = 0x47;

	appFrame->_asdu.data.clear();
	appFrame->_asdu.data = std::vector<BYTE>(13);
	appFrame->_asdu.data[0] ='T';
	appFrame->_asdu.data[1] ='X';

	appFrame->_asdu.data[2] =  CUdfGlobalFUNC::ByteToBCD((BYTE)(t.wYear-2000));
	appFrame->_asdu.data[3] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wMonth);
	appFrame->_asdu.data[4] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wDay);
	appFrame->_asdu.data[5] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wHour);
	appFrame->_asdu.data[6] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wMinute);
	appFrame->_asdu.data[7] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wSecond);

	appFrame->nMsgLen = 8;

	BYTE Len = 0;
	BuildSendBuffer(appFrame,sBuf,Len,appFrame->nMsgLen);
	SendRtuLinkFrame(sBuf,Len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	AddReSendObject(sBuf,Len,appFrame);
}


void CSysDataTrans_Syq::SYQ_EN_49H(CSYQFrameFormat* appFrame)
{
	if (!appFrame)   return;
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));


	SYSTEMTIME t;
	GetLocalTime(&t);

	appFrame->m_FunType = 0x49;

	appFrame->_asdu.data.clear();
	appFrame->_asdu.data = std::vector<BYTE>(13);
	appFrame->_asdu.data[0] ='T';
	appFrame->_asdu.data[1] ='X';

	appFrame->_asdu.data[2] =  CUdfGlobalFUNC::ByteToBCD((BYTE)(t.wYear-2000));
	appFrame->_asdu.data[3] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wMonth);
	appFrame->_asdu.data[4] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wDay);
	appFrame->_asdu.data[5] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wHour);
	appFrame->_asdu.data[6] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wMinute);
	appFrame->_asdu.data[7] =  CUdfGlobalFUNC::ByteToBCD((BYTE)t.wSecond);

	appFrame->_asdu.data[8] =  'E';
	appFrame->_asdu.data[9] =  'A';
	appFrame->_asdu.data[10] = 'R';
	appFrame->_asdu.data[11] = 'S';
	appFrame->_asdu.data[12] = 'E';

	appFrame->nMsgLen = 13;

	BYTE Len = 0;
	BuildSendBuffer(appFrame,sBuf,Len,appFrame->nMsgLen);
	SendRtuLinkFrame(sBuf,Len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	AddReSendObject(sBuf,Len,appFrame);
}





void CSysDataTrans_Syq::AddReSendObject (BYTE* sBuf,int len,CSYQFrameFormat* appFrame)
{
	if (!sBuf) return;
	if (len <= 0) return;

	//����ͨ��ʱ�������ط�����
	if (m_pSubStation != NULL && 
		m_pSubStation->GetCmType() == SC_CommChannel)
		return;

	//if (appFrame->m_CommInterface == Ethernet_CommInerface && &(appFrame->m_NetInfo) == NULL) 
	//	return;

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	RESEND_Object_SYQ* pObj = new RESEND_Object_SYQ();

	if (pObj)
	{
		if (!CUdfGlobalFUNC::SYQ_Down_APPFRAME(sBuf,len,&(pObj->m_packetFrame)))
		{
			delete pObj;
			pObj = NULL;
			return;
		}

		pObj->m_resendInterval = pApp->m_GlobalOptions.m_ResendInterval;
		pObj->m_resend_timer.SetWaitTimer(pObj->m_resendInterval * 1000);  //�����ط�ʱ����
		pObj->m_CommInterface = appFrame->m_CommInterface;

		//if (pToNetInfo != NULL)
		{
			pObj->m_ToNetInfo.mIpAddress = appFrame->m_NetInfo.mIpAddress;
			pObj->m_ToNetInfo.mPort      = appFrame->m_NetInfo.mPort;
		}

		m_Mutex_ReSend.Lock();
		m_lstReSendObjects.AddTail(pObj);
		m_Mutex_ReSend.Unlock();
	}
}

//���ط��б���ɾ��ĳһ�ط�����
/*
*/
void CSysDataTrans_Syq::DeleteOneReSendObject(BYTE commandtype)
{
	RESEND_Object_SYQ* pDel_RESEND_Obj = GetReSendObject(commandtype);
	if (pDel_RESEND_Obj)
	{
		delete pDel_RESEND_Obj;
		pDel_RESEND_Obj = NULL;
	}
}

//���ط��б���������ȡһ���ط�����
/*
*/
RESEND_Object_SYQ* CSysDataTrans_Syq::GetReSendObject(BYTE commandtype)
{
	RESEND_Object_SYQ* pReSendObj = NULL;
	m_Mutex_ReSend.Lock();
	POSITION rPos = m_lstReSendObjects.GetHeadPosition();
	while (rPos != NULL)
	{
		RESEND_Object_SYQ* pObj = m_lstReSendObjects.GetAt(rPos);
		if (pObj && pObj->m_packetFrame.m_FunType == commandtype)
		{
			m_lstReSendObjects.RemoveAt(rPos);
			pReSendObj = pObj;
			break;
		}

		m_lstReSendObjects.GetNext(rPos);
	}
	m_Mutex_ReSend.Unlock();
	return pReSendObj;
}

//����ط�����
void CSysDataTrans_Syq::CheckResendObject()
{
	//��鱣����Ҫ�ط������List
	m_Mutex_ReSend.Lock();

	//�洢����ط��������ʱ����
	CList<RESEND_Object_SYQ*> tempList;
	while (!m_lstReSendObjects.IsEmpty())
	{
		RESEND_Object_SYQ* pObj = m_lstReSendObjects.RemoveHead();

		if (pObj != NULL)
		{
			BOOL blDeleted = FALSE ;								//�ط������Ƿ���ɾ���ı�־

			if (pObj->m_resend_timer.WaitTimerState() == true)		//����Ƿ����ط���ʱ��
			{
				pObj->m_packetFrame.m_repeat -- ;				//FCB �Զ���һ

				if (pObj->m_packetFrame.m_repeat == 0)
				{
					delete pObj;
					pObj = NULL; 
					blDeleted = TRUE;
				}
				else 
				{
					//���汨�Ĳ��·�
					BYTE sBuf[SEND_BUFFER_LEN];
					BYTE  nLen = 0;
					memset(sBuf,0,sizeof(sBuf));
					BuildSendBuffer(&(pObj->m_packetFrame),sBuf,nLen,pObj->m_packetFrame.nMsgLen);
					SendRtuLinkFrame(sBuf,nLen,pObj->m_CommInterface,&(pObj->m_ToNetInfo));

					pObj->m_resend_timer.SetWaitTimer(pObj->m_resendInterval * 1000);
				}
			}

			if (blDeleted == FALSE)
				tempList.AddTail(pObj);
		}
	}
	while (!tempList.IsEmpty())
	{
		RESEND_Object_SYQ* pObj = tempList.RemoveHead();
		if (pObj)
			m_lstReSendObjects.AddTail(pObj);
	}

	m_Mutex_ReSend.Unlock();
}
