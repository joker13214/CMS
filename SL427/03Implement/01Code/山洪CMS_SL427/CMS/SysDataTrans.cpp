#include "StdAfx.h"
#include "SysDataTrans.h"
#include "UdfGlobalFUNC.h"
#include "CommServer.h"
#include "CMS.h"
#include "MainFrm.h"
#include "RtuStation.h"

#include "SystemTimeOp.h"

#pragma warning(disable:4244)
#pragma warning(disable:4267)
#pragma warning(disable:4018)
#pragma warning(disable:4819)
double flow1=0;
double flow2=0;
CSysDataTrans::CSysDataTrans(void)
{
	m_pSubStation =NULL;
}

CSysDataTrans::~CSysDataTrans(void)
{
	ClearReSendObjectList();
}

void CSysDataTrans::Dispatcher(CPacketFrameFormat* pWaterFrame)
{
	if (pWaterFrame != NULL)
	{
		if (pWaterFrame->m_pStation)
			m_pSubStation->CheckRepeatType(pWaterFrame);

		switch (pWaterFrame->_asdu._AFN)
		{
		case 0x02:
			IEC_waterdata_US_02H(pWaterFrame);
			break;
		case 0x03:
			IEC_waterdata_US_03H(pWaterFrame);
			break;
		case 0x1B:
			IEC_waterdata_US_1BH(pWaterFrame);
			break;
		case 0x30:
			IEC_waterdata_US_30H(pWaterFrame);
			break;
		case 0x31:
			IEC_waterdata_US_31H(pWaterFrame);
			break;
		case 0x32:
			IEC_waterdata_US_32H(pWaterFrame);
			break;
		case 0x33:
			IEC_waterdata_US_33H(pWaterFrame);
			break;
		case 0x34:
			IEC_waterdata_US_34H(pWaterFrame);
			break;
		case 0x50:
			IEC_waterdata_US_50H(pWaterFrame);
			break;
		case 0x51:
			IEC_waterdata_US_51H(pWaterFrame);
			break;
		case 0x52:
			IEC_waterdata_US_52H(pWaterFrame);
			break;
		case 0x53:
			IEC_waterdata_US_53H(pWaterFrame);
			break;
		case 0x54:
			IEC_waterdata_US_54H(pWaterFrame);
			break;
		case 0x55:
			IEC_waterdata_US_55H(pWaterFrame);
			break;
		case 0x56:
			IEC_waterdata_US_56H(pWaterFrame);
			break;
		case 0x57:
			IEC_waterdata_US_57H(pWaterFrame);
			break;
		case 0x58:
			IEC_waterdata_US_58H(pWaterFrame);
			break;
		case 0x59:
			IEC_waterdata_US_59H(pWaterFrame);
			break;
		case 0x5A:
			IEC_waterdata_US_5AH(pWaterFrame);
			break;
		case 0x5B:
			IEC_waterdata_US_5BH(pWaterFrame);
			break;
		case 0x5C:
			IEC_waterdata_US_5CH(pWaterFrame);
			break;
		case 0x5D:
			IEC_waterdata_US_5DH(pWaterFrame);
			break;
		case 0x5E:
			IEC_waterdata_US_5EH(pWaterFrame);
			break;
		case 0x5F:
			IEC_waterdata_US_5FH(pWaterFrame);
			break;
		case 0x60:
			IEC_waterdata_US_60H(pWaterFrame);
			break;
		case 0x61:
			IEC_waterdata_US_61H(pWaterFrame);
			break;
		case 0x62:
			IEC_waterdata_US_62H(pWaterFrame);
			break;
		case 0x63:
			IEC_waterdata_US_63H(pWaterFrame);
			break;
		case 0x64:
			IEC_waterdata_US_64H(pWaterFrame);
			break;
		case 0x65:
			IEC_waterdata_US_65H(pWaterFrame);
			break;
		case 0x66:
			IEC_waterdata_US_66H(pWaterFrame);
			break;
		case 0x67:
			IEC_waterdata_US_67H(pWaterFrame);
			break;
		case 0x68:				//V2.0 �汾�������֦����Ŀ�޸���չ��
			IEC_waterdata_US_68H(pWaterFrame);
			break;
		case 0x69:				//V2.0 �汾�������֦����Ŀ�޸���չ��
			IEC_waterdata_US_69H(pWaterFrame);
			break;
		case 0x6A:				//V2.0 �汾�����ľ������Ŀ�޸���չ�����λ�ƴ�����
			IEC_waterdata_US_6AH(pWaterFrame);
			break;
		case 0x6B:				//V2.0 �汾�����ľ������Ŀ�޸���չ���¶ȴ�����
			IEC_waterdata_US_6BH(pWaterFrame);
			break;
		case 0x80:
			IEC_waterdata_US_80H(pWaterFrame);
			break;
		case 0x81:
			IEC_waterdata_US_81H(pWaterFrame);
			break;
		case 0x82:
			IEC_waterdata_US_82H(pWaterFrame);
			break;
		case 0x83:
			IEC_waterdata_US_83H(pWaterFrame);
			break;
		case 0x84:
			IEC_waterdata_US_84H(pWaterFrame);
			break;
		case 0x85:			//V2.0 �汾�������֦����Ŀ�޸���չ��
			IEC_waterdata_US_85H(pWaterFrame);
			break;
		case 0x90:
			IEC_waterdata_US_90H(pWaterFrame);
			break;
		case 0x91:
			IEC_waterdata_US_91H(pWaterFrame);
			break;
		case 0x92:
			IEC_waterdata_US_92H(pWaterFrame);
			break;
		case 0x93:
			IEC_waterdata_US_93H(pWaterFrame);
			break;
		case 0x94:
			IEC_waterdata_US_94H(pWaterFrame);
			break;
		case 0x95:
			IEC_waterdata_US_95H(pWaterFrame);
			break;

		case 0xA0:      //V2.0 �汾����,ƽ����
			IEC_waterdata_US_A0H(pWaterFrame);
			break;
		}
	}
}

//���ͱ��Ĵ�����
//Ҫ����ͨ������ȷ���ĵĸ�ʽ���
void CSysDataTrans::Dispatcher(BYTE* buffer,int bufferLen,BYTE CmInterface/* =Serial_CommInterface */, CString FromIP/* ="" */,int FromPort/* =0 */)
{

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	CRealTimeDB* pRealDb = pApp->m_pReadDB;
	if (!pRealDb)  
		return ;

	CPacketFrameFormat waterFrame;
	if (!CUdfGlobalFUNC::IEC_waterdata_to_APPFRAME(buffer,bufferLen,&waterFrame))
	{
		TRACE("����֡��ʽ��������ȷ��\r\n");
		return;
	}

	CRtuStation* pSub =  pRealDb->SearchRtuStation(waterFrame._addr_info.A1.ProvinceCode,
		waterFrame._addr_info.A1.RegionCode,
		waterFrame._addr_info.A2);
	if (!pSub)     
	{
		TRACE("δ�ҵ�վ:ʡ��:%d,����:%d,�ն�/�м̵�ַ:%d\r\n",waterFrame._addr_info.A1.ProvinceCode,
			waterFrame._addr_info.A1.RegionCode,
			waterFrame._addr_info.A2);
		waterFrame.m_pStation = NULL;
		return ;
	}
	else
	{
		waterFrame.m_pStation = pSub;

		//������ձ�����־
		AddRecvLog(buffer,bufferLen,&waterFrame);

		pSub->SetLastRecvTime();
		if (pSub->SetCommState(Connected_State) == true)  //״̬�иı䣬����CMSRightView����ʾ
			UpdateRtuCommState(pSub);

		if(pSub->GetInterface() != CmInterface)
			pSub->SetInterface(CmInterface);		 //����ͨ�Žӿ�

		waterFrame.m_CommInterface = CmInterface;
		if (CmInterface == Ethernet_CommInerface)
		{
			if(pSub->GetIpAddrA() != FromIP)
				pSub->SetIpAddrA(FromIP);

			if (pSub->GetSockPortA() != FromPort)
				pSub->SetSockPortA(FromPort);

			waterFrame.m_NetInfo.mIpAddress = FromIP;
			waterFrame.m_NetInfo.mPort      = FromPort;
		}
		else if (CmInterface == Serial_CommInterface)
		{

			//PCOM_INFO pComInfo = GetParentCommThread()->GetComInfo();
			//pSub->SetComPortA(pComInfo->mComName);
			//pSub->SetBaundA(pComInfo->mBaund);
			//pSub->SetDataBitsA(pComInfo->mDataBits);
			//pSub->SetStopBitsA(pComInfo->mStopBits);
			//pSub->SetParityA(pComInfo->mParity);

		}
	}

	switch (waterFrame._asdu._AFN)
	{
	case 0x02:
		IEC_waterdata_US_02H(&waterFrame);
		break;
	case 0x1B:
		IEC_waterdata_US_1BH(&waterFrame);
		break;
	case 0x30:
		IEC_waterdata_US_30H(&waterFrame);
		break;
	case 0x31:
		IEC_waterdata_US_31H(&waterFrame);
		break;
	case 0x32:
		IEC_waterdata_US_32H(&waterFrame);
		break;
	case  0x33:
		IEC_waterdata_US_33H(&waterFrame);
		break;
	case  0x34:
		IEC_waterdata_US_34H(&waterFrame);
		break;
	case  0x50:
		IEC_waterdata_US_50H(&waterFrame);
		break;
	case  0x51:
		IEC_waterdata_US_51H(&waterFrame);
		break;
	case 0x52:
		IEC_waterdata_US_52H(&waterFrame);
		break;
	case 0x53:
		IEC_waterdata_US_53H(&waterFrame);
		break;
	case 0x54:
		IEC_waterdata_US_54H(&waterFrame);
		break;
	case 0x55:
		IEC_waterdata_US_55H(&waterFrame);
		break;
	case 0x56:
		IEC_waterdata_US_56H(&waterFrame);
		break;
	case 0x57:
		IEC_waterdata_US_57H(&waterFrame);
		break;
	case 0x58:
		IEC_waterdata_US_58H(&waterFrame);
		break;
	case 0x59:
		IEC_waterdata_US_59H(&waterFrame);
		break;
	case 0x5A:
		IEC_waterdata_US_5AH(&waterFrame);
		break;
	case 0x5B:
		IEC_waterdata_US_5BH(&waterFrame);
		break;
	case 0x5C:
		IEC_waterdata_US_5CH(&waterFrame);
		break;
	case 0x5D:
		IEC_waterdata_US_5DH(&waterFrame);
		break;
	case 0x5E:
		IEC_waterdata_US_5EH(&waterFrame);
		break;
	case 0x5F:
		IEC_waterdata_US_5FH(&waterFrame);
		break;
	case 0x60:
		IEC_waterdata_US_60H(&waterFrame);
		break;
	case 0x61:
		IEC_waterdata_US_61H(&waterFrame);
		break;
	case 0x62:
		IEC_waterdata_US_62H(&waterFrame);
		break;
	case 0x63:
		IEC_waterdata_US_63H(&waterFrame);
		break;
	case 0x64:
		IEC_waterdata_US_64H(&waterFrame);
		break;
	case 0x65:
		IEC_waterdata_US_65H(&waterFrame);
		break;
	case 0x66:
		IEC_waterdata_US_66H(&waterFrame);
		break;
	case 0x67:
		IEC_waterdata_US_67H(&waterFrame);
		break;
	case 0x6A://add by yhp 20160615
		IEC_waterdata_US_6AH(&waterFrame);
		break;
	case 0x6B://add by yhp 20160615
		IEC_waterdata_US_6BH(&waterFrame);
		break;
	case  0x80:
		IEC_waterdata_US_80H(&waterFrame);
		break;
	case  0x81:
		IEC_waterdata_US_81H(&waterFrame);
		break;
	case 0x82:
		IEC_waterdata_US_82H(&waterFrame);
		break;
	case 0x83:
		IEC_waterdata_US_83H(&waterFrame);
		break;
	case 0x84:
		IEC_waterdata_US_84H(&waterFrame);
		break;
	case  0x90:
		IEC_waterdata_US_90H(&waterFrame);
		break;
	case  0x91:
		IEC_waterdata_US_91H(&waterFrame);
		break;
	case  0x92:
		IEC_waterdata_US_92H(&waterFrame);
		break;
	case  0x93:
		IEC_waterdata_US_93H(&waterFrame);
		break;
	case  0x94:
		IEC_waterdata_US_94H(&waterFrame);
		break;
	case  0x95:
		IEC_waterdata_US_95H(&waterFrame);
		break;
	}
}

//����֡��ʽ����
//bool CSysDataTrans::IEC_waterdata_to_APPFRAME(BYTE* Buffer,int len,CPacketFrameFormat* waterFrame)
//{
//	if (!Buffer || !waterFrame)   return false;
//	if (len <5 )				  return false;
//
//	waterFrame->_bFLen	 = Buffer[1];
//	std::bitset<8> ContorlC = Buffer[3];
//	waterFrame->ControlC._DIR = ContorlC.at(7);
//    waterFrame->ControlC._DIV = ContorlC.at(6);
//	waterFrame->ControlC._Command = (BYTE)(Buffer[3] & 0x0f );
//	waterFrame->ControlC._FCB     = (BYTE)((Buffer[3] & 0x30)>>4);
//	if (waterFrame->ControlC._DIV == 1)				//��ֱ���
//	{
//		waterFrame->ControlC._DIVS = Buffer[4];
//
//		//������ַ��
//		DWORD _A2A3 = Buffer[7]|Buffer[8]<<8| Buffer[9]<<16;
//		waterFrame->_addr_info.A1.ProvinceCode = CUdfGlobalFUNC::ByteFromBCD(Buffer[5]);
//		waterFrame->_addr_info.A1.RegionCode   = CUdfGlobalFUNC::ByteFromBCD(Buffer[6]);
//		waterFrame->_addr_info.A2 = (_A2A3 & 0x001fffff);
//		waterFrame->_addr_info.A3 = (BYTE)((_A2A3 & 0x00E00000)>>21);
//
//		waterFrame->_asdu._AFN = Buffer[10];
//
//		if (waterFrame->_bFLen - 8 > 0 )  //
//		{
//			waterFrame->_asdu.data.clear();
//			waterFrame->_asdu.data =std::vector<unsigned char>(waterFrame->_bFLen - 8);
//			memmove(&*(waterFrame->_asdu.data.begin()),&Buffer[11],waterFrame->_bFLen - 8);
//		}
//	}
//	else if (waterFrame->ControlC._DIV == 0)			//δ��ֱ���
//	{
//
//		//������ַ����Ϣ
//		DWORD _A2A3  = Buffer[6]|Buffer[7]<<8|Buffer[8]<<16;
//		waterFrame->_addr_info.A1.ProvinceCode = CUdfGlobalFUNC::ByteFromBCD(Buffer[4]);
//		waterFrame->_addr_info.A1.RegionCode   = CUdfGlobalFUNC::ByteFromBCD(Buffer[5]);
//		waterFrame->_addr_info.A2 =  (_A2A3 & 0x001fffff);
//		waterFrame->_addr_info.A3 = (BYTE)((_A2A3 & 0x00E00000)>>21);
//
//		waterFrame->_asdu._AFN = Buffer[9];
//		if (waterFrame->_bFLen -7 > 0)
//		{
//			waterFrame->_asdu.data.clear();
//			waterFrame->_asdu.data =std::vector<unsigned char>(waterFrame->_bFLen - 7);
//			memmove(&*(waterFrame->_asdu.data.begin()),&Buffer[10],waterFrame->_bFLen - 7);
//		}
//	}
//	return true;
//}


void CSysDataTrans::IEC_textMsg_cpms(CSmsFrameFormat* sendFrame)
{
	if (NULL == sendFrame)
	{
		return;
	}

	BYTE buf[3];
	memset(buf, 0, sizeof(buf));

	if (sendFrame->m_setVar == "SM")
	{
		buf[0] = 'S';
		buf[1] = 'M';
	}
	else if (sendFrame->m_setVar == "BM")
	{
		buf[0] = 'B';
		buf[1] = 'M';
	}

	sendFrame->m_buf[0] = 'A';
	sendFrame->m_buf[1] = 'T';
	sendFrame->m_buf[2] = '+';
	sendFrame->m_buf[3] = 'C';
	sendFrame->m_buf[4] = 'P';
	sendFrame->m_buf[5] = 'M';
	sendFrame->m_buf[6] = 'S';
	sendFrame->m_buf[7] = '=';
	sendFrame->m_buf[8] = '"';
	sendFrame->m_buf[9] = buf[0];
	sendFrame->m_buf[10] = buf[1];
	sendFrame->m_buf[11] = '"';

	sendFrame->m_bufLen = 12;
	SendRtuLinkFrame(sendFrame->m_buf, sendFrame->m_bufLen, sendFrame->m_CommInterface);


}

void CSysDataTrans::IEC_textMsg_cpmsa(CSmsFrameFormat* sendFrame)
{
	if (NULL == sendFrame)
	{
		return;
	}

	sendFrame->m_buf[0] = 'A';
	sendFrame->m_buf[1] = 'T';
	sendFrame->m_buf[2] = '+';
	sendFrame->m_buf[3] = 'C';
	sendFrame->m_buf[4] = 'P';
	sendFrame->m_buf[5] = 'M';
	sendFrame->m_buf[6] = 'S';
	sendFrame->m_buf[7] = '?';

	sendFrame->m_bufLen = 8;
	SendRtuLinkFrame(sendFrame->m_buf, sendFrame->m_bufLen, sendFrame->m_CommInterface);


}

void CSysDataTrans::IEC_textMsg_cmgf(CSmsFrameFormat* sendFrame)
{
	if (NULL == sendFrame)
	{
		return;
	}

	BYTE msgFormat = '0';
	if (sendFrame->m_setVar == "PDU Mode")
	{
		msgFormat = '0';
	}
	else if (sendFrame->m_setVar == "Text Mode")
	{
		msgFormat = '1';
	}
	sendFrame->m_buf[0] = 'A';
	sendFrame->m_buf[1] = 'T';
	sendFrame->m_buf[2] = '+';
	sendFrame->m_buf[3] = 'C';
	sendFrame->m_buf[4] = 'M';
	sendFrame->m_buf[5] = 'G';
	sendFrame->m_buf[6] = 'F';
	sendFrame->m_buf[7] = '=';
	sendFrame->m_buf[8] = msgFormat;


	sendFrame->m_bufLen = 9;
	SendRtuLinkFrame(sendFrame->m_buf, sendFrame->m_bufLen, sendFrame->m_CommInterface);


}

void CSysDataTrans::IEC_textMsg_csmp(CSmsFrameFormat* sendFrame)
{
	if (NULL == sendFrame)
	{
		return;
	}

	sendFrame->m_buf[0] = 'A';
	sendFrame->m_buf[1] = 'T';
	sendFrame->m_buf[2] = '+';
	sendFrame->m_buf[3] = 'C';
	sendFrame->m_buf[4] = 'S';
	sendFrame->m_buf[5] = 'M';
	sendFrame->m_buf[6] = 'P';
	sendFrame->m_buf[7] = '=';
	sendFrame->m_buf[8] = '1';
	sendFrame->m_buf[9] = '7';
	sendFrame->m_buf[10] = ',';
	sendFrame->m_buf[11] = '1';

	sendFrame->m_buf[12] = '6';
	sendFrame->m_buf[13] = '7';
	sendFrame->m_buf[14] = ',';
	sendFrame->m_buf[15] = '0';

	sendFrame->m_buf[16] = ',';
	sendFrame->m_buf[17] = '0';


	sendFrame->m_bufLen = 18;
	SendRtuLinkFrame(sendFrame->m_buf, sendFrame->m_bufLen, sendFrame->m_CommInterface);


}

void CSysDataTrans::IEC_textMsg_cmgl(CSmsFrameFormat* tmFrame)
{
	if (NULL == tmFrame)
	{
		return;
	}

	tmFrame->m_buf[0] = 'A';
	tmFrame->m_buf[1] = 'T';
	tmFrame->m_buf[2] = '+';
	tmFrame->m_buf[3] = 'C';
	tmFrame->m_buf[4] = 'M';
	tmFrame->m_buf[5] = 'G';
	tmFrame->m_buf[6] = 'L';
	tmFrame->m_buf[7] = '=';

	int No = atoi(tmFrame->m_setVar) + 48;
	tmFrame->m_buf[8] = No;
	tmFrame->m_bufLen = 9;


	SendRtuLinkFrame(tmFrame->m_buf, tmFrame->m_bufLen, tmFrame->m_CommInterface);

}

void CSysDataTrans::IEC_textMsg_cmgr(CSmsFrameFormat* tmFrame)
{
	if (NULL == tmFrame)
	{
		return;
	}

	tmFrame->m_buf[0] = 'A';
	tmFrame->m_buf[1] = 'T';
	tmFrame->m_buf[2] = '+';
	tmFrame->m_buf[3] = 'C';
	tmFrame->m_buf[4] = 'M';
	tmFrame->m_buf[5] = 'G';
	tmFrame->m_buf[6] = 'R';
	tmFrame->m_buf[7] = '=';

	int No = atoi(tmFrame->m_setVar);
	tmFrame->m_buf[8] = No;
	tmFrame->m_bufLen = 9;


	SendRtuLinkFrame(tmFrame->m_buf, tmFrame->m_bufLen, tmFrame->m_CommInterface);


}

void CSysDataTrans::IEC_textMsg_cmgd(CSmsFrameFormat* tmFrame)
{
	if (NULL == tmFrame)
	{
		return;
	}

	tmFrame->m_buf[0] = 'A';
	tmFrame->m_buf[1] = 'T';
	tmFrame->m_buf[2] = '+';
	tmFrame->m_buf[3] = 'C';
	tmFrame->m_buf[4] = 'M';
	tmFrame->m_buf[5] = 'G';
	tmFrame->m_buf[6] = 'D';
	tmFrame->m_buf[7] = '=';

	int No = atoi(tmFrame->m_setVar);
	tmFrame->m_buf[8] = No;
	tmFrame->m_buf[9] = ',';
	tmFrame->m_buf[10] = 1;
	tmFrame->m_bufLen = 11;


	SendRtuLinkFrame(tmFrame->m_buf, tmFrame->m_bufLen, tmFrame->m_CommInterface);

}


//��·���(ȷ��֡)
void CSysDataTrans::IEC_waterdata_US_02H(CPacketFrameFormat* waterFrame)   
{
	if (!waterFrame)  return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;
	if (waterFrame->ControlC._Command != C_CMD_03_LINKTEST) return;
	if (waterFrame->_asdu._AFN != 0x02) return;
	if (waterFrame->_asdu.data.size() <= 0) return;

	DeleteOneReSendObject(waterFrame->ControlC._Command,waterFrame->_asdu._AFN,waterFrame->_addr_info);

	CString			strLog = "";
	_Log_level_type logLevel = Normal_Log_Message;

	BYTE LinkCheckState = waterFrame->_asdu.data[0];
	if (LinkCheckState      == 0xF0)		//��¼
	{
		strLog.Format("��·���:��¼ �ɹ�!");
	}
	else if (LinkCheckState == 0xF1)     //�˳���¼
	{
		strLog.Format("��·���:�˳���¼ �ɹ�!");
	}
	else if (LinkCheckState == 0xF2)     //��������
	{
		strLog.Format("��·���:���߱��� �ɹ�!");
	}
	else
	{
		strLog.Format("��·���:δ֪!");
		logLevel = Warning_log_Message;
	}

	TRACE("��·���(ȷ��֡):%s\r\n",strLog);

	if (waterFrame->m_pStation != NULL)
	{
		if (LinkCheckState == 0xF0 ||
			LinkCheckState == 0xF1 ||
			LinkCheckState == 0xF2)
			waterFrame->m_pStation->SetLinkCheckState(LinkCheckState);

		UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_Connect_check);
		AddLogMessage(waterFrame->m_pStation,strLog,logLevel,(PSYSTEMTIME)NULL);
	}
}

void CSysDataTrans::IEC_waterdata_US_03H( CPacketFrameFormat* waterFrame )
{
	if (!waterFrame) return;
	if (waterFrame->_asdu._AFN != 0x03) return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;

	CString sLog = "ң���ն˻�:��·���";
	_Log_level_type xLevel = Normal_Log_Message;
	AddLogMessage(waterFrame->m_pStation,sLog,xLevel,(PSYSTEMTIME)NULL);

	IEC_waterdata_EN_03H(waterFrame);
}



//�����ն�վ�����ı�ף���ʼ��ֵ����Ӧ֡��
void CSysDataTrans::IEC_waterdata_US_1BH(CPacketFrameFormat* waterFrame)   
{
	if (!waterFrame) return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;
	if (waterFrame->_asdu._AFN != 0x1B) return;

	if (waterFrame->m_pStation)
	{
		int nCount = (int)waterFrame->_asdu.data.size() / 4;

		int nAdded = nCount - (int)waterFrame->m_pStation->GetFlowInitValumeParasList()->GetCount();
		if (nAdded > 0)
		{
			for (int i = 0; i < nAdded; i ++)
			{
				CFlowInitValume_PARA_PER* pFlowInit = new CFlowInitValume_PARA_PER();
				waterFrame->m_pStation->GetFlowInitValumeParasList()->AddTail(pFlowInit);
			}
		}
		for ( int n = 0; n < nCount; n++)
		{
			CFlowInitValume_PARA_PER* pFlowInit = waterFrame->m_pStation->GetFlowInitValumePara(n);
			if (pFlowInit)
			{
				DWORD flowInitData = (DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[n * 4 + 0]) +
					(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[n * 4 + 1]) * 100 +
					(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[n * 4 + 2]) * 10000 +
					(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[n * 4 + 3]) * 1000000;
				pFlowInit->SetFlowInitValume(flowInitData);
			}
		}

		UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_flowpara_set);
	}
}

//��ѯң���ն˻��м�վ��ַ����Ӧ֡��
void CSysDataTrans::IEC_waterdata_US_50H(CPacketFrameFormat* waterFrame)  
{
	if (!waterFrame) return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;
	if (waterFrame->ControlC._Command != C_CMD_04_MSUREPARA)  return;
	if (waterFrame->_asdu._AFN != 0x50) return;

}


//��ѯң���ն˻��ռ�վʱ�ӣ���Ӧ֡��
void CSysDataTrans::IEC_waterdata_US_51H(CPacketFrameFormat* waterFrame)   
{
	if (!waterFrame) return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;
	if (waterFrame->ControlC._Command != C_CMD_04_MSUREPARA) return;
	if (waterFrame->_asdu._AFN != 0x51) return;
	if (waterFrame->_asdu.data.size() < 6) return;

	DeleteOneReSendObject(C_CMD_04_MSUREPARA,0x51,waterFrame->_addr_info);

	SYSTEMTIME st;
	ZeroMemory(&st,sizeof(SYSTEMTIME));

	st.wYear  = 2000 + CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[5]);
	st.wMonth = CUdfGlobalFUNC::ByteFromBCD( waterFrame->_asdu.data[4] & 0x1f);
	BYTE wdayweek = ((waterFrame->_asdu.data[4] & 0xE0) >>5);
	if (wdayweek != 0)
		st.wDayOfWeek = wdayweek;
	st.wDay		= CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[3]);
	st.wHour	= CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[2]);
	st.wMinute	= CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[1]);
	st.wSecond	= CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[0]);


	CString strClock = "";
	strClock.Format("%04d-%02d-%02d %02d:%02d:%02d",
		st.wYear,
		st.wMonth,
		st.wDay,
		st.wHour,
		st.wMinute,
		st.wSecond);

	TRACE("��ѯʱ����Ӧ:%s\r\n",strClock);

	if (waterFrame->m_pStation != NULL)
	{
		UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_datetime_call);
		UpdateRtuClockString(waterFrame->m_pStation,strClock);
	}
}

//��ѯң���ն˵Ĺ���ģʽ����Ӧ֡��
void CSysDataTrans::IEC_waterdata_US_52H(CPacketFrameFormat* waterFrame)   
{
	if (!waterFrame) return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;
	if (waterFrame->ControlC._Command != C_CMD_04_MSUREPARA) return;

	if (waterFrame->_asdu._AFN != 0x52||
		waterFrame->_asdu.data.size() != 1) return;

	DeleteOneReSendObject(C_CMD_04_MSUREPARA,0x52,waterFrame->_addr_info);

	BYTE bWorkMode = waterFrame->_asdu.data[0];
	if (waterFrame->m_pStation)
	{
		waterFrame->m_pStation->SetWorkMode(bWorkMode);
		UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_workmode_call);
	}
}

//��ѯң���ն˵������Ա����༰ʱ����(��Ӧ֡��
void CSysDataTrans::IEC_waterdata_US_53H(CPacketFrameFormat* waterFrame)   
{
	if	(!waterFrame) return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;
	if (waterFrame->_asdu._AFN != 0x53 || waterFrame->_asdu.data.size() != 4)  return;

	DeleteOneReSendObject(C_CMD_04_MSUREPARA,0x53,waterFrame->_addr_info);

	WORD wAutoReport   = (waterFrame->_asdu.data[0] | (waterFrame->_asdu.data[1]<<8));
	WORD wAutoInterval = (waterFrame->_asdu.data[2] | (waterFrame->_asdu.data[3]<<8));

	if (waterFrame->m_pStation) 
	{
		if (waterFrame->m_pStation->GetAutoReportPara() == NULL) 
			return;

		waterFrame->m_pStation->GetAutoReportPara()->SetAutoValue(wAutoReport);
		waterFrame->m_pStation->GetAutoReportPara()->SetAutoInterval(wAutoInterval);

		UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_autoRpPara_call);
	}
}

//��ѯң��վ���ѯ��ʵʱ�������ࣨ��Ӧ֡��
void CSysDataTrans::IEC_waterdata_US_54H(CPacketFrameFormat* waterFrame)   
{
	if (!waterFrame)	return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;
	if (waterFrame->_asdu._AFN  != 0x54  || waterFrame->_asdu.data.size() != 2 ) return;

	DeleteOneReSendObject(C_CMD_04_MSUREPARA,0x54,waterFrame->_addr_info);

	WORD wYcQueryPara = (waterFrame->_asdu.data[0]|(waterFrame->_asdu.data[1]<<8));

	if (waterFrame->m_pStation)
	{
		if (waterFrame->m_pStation->GetYcQueryType() != NULL)
			waterFrame->m_pStation->GetYcQueryType()->SetQueryPara(wYcQueryPara);

		UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_ycquerypara_call);
	}
}

//��ѯң���ն�����ɹ���ֵ�������е�ʣ��ˮ������Ӧ֡��
void CSysDataTrans::IEC_waterdata_US_55H(CPacketFrameFormat* waterFrame)  
{
	if (!waterFrame)     return;
	if (waterFrame->ControlC._DIR != UP_ARROW)  return;
	if (waterFrame->_asdu._AFN != 0x54 || waterFrame->_asdu.data.size() !=9) return;

	DeleteOneReSendObject(C_CMD_04_MSUREPARA,0x55,waterFrame->_addr_info);

	if (waterFrame->m_pStation  && waterFrame->m_pStation->GetWaterValumePara())
	{
		DWORD b1=0,b2=0,b3=0,b4=0,Sum = 0;
		b1 = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[0]);
		b2 = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[1]);
		b3 = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[2]);
		b4 = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[3]);

		Sum  = b1 + b2 * 100 + b3 * 10000 + b4 * 1000000;

		UINT64 r1=0,r2=0,r3=0,r4=0,r5=0,rSum = 0;
		BYTE   bSignBit = 0;
		r1 = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[4]);
		r2 = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[5]);
		r3 = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[6]);
		r4 = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[7]);

		bSignBit = ((waterFrame->_asdu.data[8] & 0x80) != 0? 1:0);
		r5 = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[8]& 0x7f);

		rSum = r1 + r2 * 100 + r3 * 10000 + r4 * 1000000 + r5 * 100000000;

		waterFrame->m_pStation->GetWaterValumePara()->SetRechargeValume(Sum);
		waterFrame->m_pStation->GetWaterValumePara()->SetRemainValume(rSum);
		waterFrame->m_pStation->GetWaterValumePara()->SetSignBit(bSignBit);

		UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_water_recharge_call);
	}
}

//��ѯң���ն˵�ʣ��ˮ���ͱ���ֵ����Ӧ֡��
void CSysDataTrans::IEC_waterdata_US_56H(CPacketFrameFormat* waterFrame)   
{
	if (!waterFrame)  return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;
	if (waterFrame->_asdu._AFN != 0x56 || waterFrame->_asdu.data.size() != 8) return;

	DeleteOneReSendObject(C_CMD_04_MSUREPARA,0x56,waterFrame->_addr_info);

	if (waterFrame->m_pStation && waterFrame->m_pStation->GetWaterValumePara())
	{
		DWORD a1=0,a2=0,a3= 0,alarmSum = 0;
		a1 = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[0]);
		a2 = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[1]);
		a3 = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[2]);
		alarmSum = a1 + a2 * 100 + a3 * 10000;

		UINT64 r1=0,r2=0,r3=0,r4=0,r5=0,rSum = 0;
		BYTE   bSignBit = 0;
		r1 = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[3]);
		r2 = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[4]);
		r3 = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[5]);
		r4 = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[6]);

		bSignBit = ((waterFrame->_asdu.data[8] & 0x80) != 0? 1:0);
		r5 = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[7]& 0x7f);

		rSum = r1 + r2 * 100 + r3 * 10000 + r4 * 1000000 + r5 * 100000000;

		waterFrame->m_pStation->GetWaterValumePara()->SetRemainAlertValue(alarmSum);
		waterFrame->m_pStation->GetWaterValumePara()->SetRemainValume(rSum);
		waterFrame->m_pStation->GetWaterValumePara()->SetSignBit(bSignBit);

		UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_water_alert_call);
	}
}

//��ѯң���ն˵�ˮλ��ֵ��ˮλ�����ޣ���Ӧ֡��
void CSysDataTrans::IEC_waterdata_US_57H(CPacketFrameFormat* waterFrame)   
{
	if (!waterFrame)   return;
	if (waterFrame->ControlC._DIR != UP_ARROW)  return;
	if (waterFrame->_asdu._AFN != 0x57 || waterFrame->_asdu.data.size()< 8) return;

	DeleteOneReSendObject(C_CMD_04_MSUREPARA,0x57,waterFrame->_addr_info);

	int nCount = (int)(waterFrame->_asdu.data.size() - 1) /7 ;

	if (waterFrame->m_pStation && waterFrame->m_pStation->GetWaterLevelParasList())
	{
		int nAdded = nCount - (int)waterFrame->m_pStation->GetWaterLevelParasList()->GetCount();
		if (nAdded > 0)
		{
			for (int i = 0; i < nAdded; i ++)
			{
				CWaterLevel_PAR_PER* pLevel_PARA = new CWaterLevel_PAR_PER();
				waterFrame->m_pStation->GetWaterLevelParasList()->AddTail(pLevel_PARA);
			}
		}

		for(int i = 0; i < nCount; i ++)
		{
			CWaterLevel_PAR_PER* pLevel_PARA = waterFrame->m_pStation->GetWaterLevelPara(i);
			if (pLevel_PARA)
			{
				float baseV = ((float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 7 + 0]) * 0.01 +
					(float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 7 + 1]) +
					(float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 7 + 2]) * 100);
				pLevel_PARA->SetBaseValue(baseV);

				float downV = (float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 7 + 3]) * 0.01 +
					(float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 7 + 4]);

				pLevel_PARA->SetDownValue(downV);

				float upV  = (float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 7 + 5]) * 0.01 +
					(float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 7 + 6]);
				pLevel_PARA->SetUpValue(upV);
			}
		}
	}

	if (waterFrame->m_pStation && waterFrame->m_pStation->GetRtuAlarm_State())
	{
		if ( nCount * 7 == waterFrame->_asdu.data.size() - 1 )
		{
			waterFrame->m_pStation->GetRtuAlarm_State()->SetAlarmStateByte(waterFrame->_asdu.data[nCount * 7]);
		}
	}

	if (waterFrame->m_pStation)
		UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_waterlevel_call);
}

//��ѯң���ն�ˮѹ�ϡ�����ֵ����Ӧ֡��
void CSysDataTrans::IEC_waterdata_US_58H(CPacketFrameFormat* waterFrame)   
{
	if (!waterFrame)  return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;
	if (waterFrame->_asdu._AFN != 0x58 || 
		waterFrame->_asdu.data.size() < 9) return;

	DeleteOneReSendObject(C_CMD_04_MSUREPARA,0x58,waterFrame->_addr_info);

	if (waterFrame->m_pStation)
	{
		int nCount = ((int)waterFrame->_asdu.data.size() - 1) / 8 ;
		if (waterFrame->m_pStation->GetWaterPressureParasList())
		{
			int nAdded = nCount - (int)waterFrame->m_pStation->GetWaterPressureParasList()->GetCount();
			if (nAdded > 0)
			{
				for ( int i = 0 ;i < nAdded; i ++)
				{
					CWaterPressure_PARA_PER* pPressure_PARA = new CWaterPressure_PARA_PER();
					waterFrame->m_pStation->GetWaterPressureParasList()->AddTail(pPressure_PARA);
				}
			}
			for (int i = 0; i < nCount; i ++)
			{

				CWaterPressure_PARA_PER* pPressure_PARA  = waterFrame->m_pStation->GetWaterPressurePara(i);
				if (pPressure_PARA)
				{
					double upV = (double) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 8]) * 0.01 +
						(double) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 8 + 1]) +
						(double) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 8 + 2]) * 100 +
						(double) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 8 + 3]) * 10000;
					pPressure_PARA->SetUpValue(upV);

					double downV =(double) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 8 + 4]) * 0.01 +
						(double) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 8 + 5]) +
						(double) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 8 + 6]) * 100 +
						(double) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 8 + 7]) * 10000;

					pPressure_PARA->SetDownValue(downV);
				}

			}
		}
		if (waterFrame->m_pStation->GetRtuAlarm_State())
		{
			if (nCount * 8  == waterFrame->_asdu.data.size() -1)
				waterFrame->m_pStation->GetRtuAlarm_State()->SetAlarmStateByte(waterFrame->_asdu.data[nCount * 8]);
		}

		UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_waterpress_call);
	}
}

//��ѯң���ն�ˮ�ʲ������ࡢ����ֵ����Ӧ֡��
void CSysDataTrans::IEC_waterdata_US_59H(CPacketFrameFormat* waterFrame)   
{
	if (!waterFrame)  return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;
	if (waterFrame->_asdu._AFN != 0x59 ||
		waterFrame->ControlC._Command != C_CMD_04_MSUREPARA) return;

	DeleteOneReSendObject(C_CMD_04_MSUREPARA,0x59,waterFrame->_addr_info);
	if (waterFrame->m_pStation && waterFrame->m_pStation->GetWaterQualityPara())
	{
		CWaterQuality_PARA* pWaterQuality_PAPR = waterFrame->m_pStation->GetWaterQualityPara();

		if (waterFrame->_asdu.data.size() < 5) return;

		UINT64 U64_BYTE[5];
		memset(U64_BYTE,0,sizeof(U64_BYTE));

		for (int n= 0; n < 5; n ++)
			U64_BYTE[n] = waterFrame->_asdu.data[n];

		UINT64  WaterQualityCheckValue =U64_BYTE[0] +
			(U64_BYTE[1]<< 8) +
			(U64_BYTE[2]<<16) +
			(U64_BYTE[3]<<24) +
			(U64_BYTE[4]<<32);

		pWaterQuality_PAPR->SetQualityValue(WaterQualityCheckValue);

		if (pWaterQuality_PAPR->Get_Buffer_Count()  != waterFrame->_asdu.data.size())
			return;

		int nStep = 5;
		for (BYTE n = 0 ; n < 40; n ++)
		{
			if (pWaterQuality_PAPR->GetBitValue(n) == true)
			{
				CWaterQuality_PARA_PER* pQuality_ONE = pWaterQuality_PAPR->GetOneQualityObject(n);

				UINT64 nBufValue = 0;
				DOUBLE dParaValue = 0.0f;

				if (n != 24)
				{
					nBufValue = (UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 0]) +
						(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 1]) * 100 +
						(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2]) * 10000 +
						(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 3]) * 1000000;
					nStep += 4;
				}
				else
				{

					nBufValue =  (UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 0]) +
						(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 1]) * 100 +
						(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2]) * 10000 +
						(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 3]) * 1000000 +
						(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 4]) * 100000000;
					nStep += 5;
				}

				if (pQuality_ONE)
				{
					dParaValue = (DOUBLE)nBufValue;
					if (pQuality_ONE->m_PointBits > 0)
					{
						for (int m = 0 ; m < pQuality_ONE->m_PointBits; m ++)
						{
							dParaValue /= 10;

						}
					}
					pQuality_ONE->m_UpValume = dParaValue;
				}

			}
		}

		UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_waterquality_up_call);
	}
}

//��ѯң���ն�ˮ�ʲ������ࡢ����ֵ����Ӧ֡��
void CSysDataTrans::IEC_waterdata_US_5AH(CPacketFrameFormat* waterFrame)   
{
	if (!waterFrame)  return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;
	if (waterFrame->_asdu._AFN != 0x5A ||
		waterFrame->ControlC._Command != C_CMD_04_MSUREPARA) return;

	DeleteOneReSendObject(C_CMD_04_MSUREPARA,0x5A,waterFrame->_addr_info);
	if (waterFrame->m_pStation && waterFrame->m_pStation->GetWaterQualityPara())
	{
		CWaterQuality_PARA* pWaterQuality_PAPR = waterFrame->m_pStation->GetWaterQualityPara();

		if (waterFrame->_asdu.data.size() < 5) return;

		UINT64  WaterQualityCheckValue =(UINT64) (waterFrame->_asdu.data[0]) +
			((UINT64)waterFrame->_asdu.data[1] << 8) +
			((UINT64)waterFrame->_asdu.data[2] <<16) +
			((UINT64)waterFrame->_asdu.data[3] <<24) +
			((UINT64)waterFrame->_asdu.data[4] <<32);

		pWaterQuality_PAPR->SetQualityValue(WaterQualityCheckValue);

		if (pWaterQuality_PAPR->Get_Buffer_Count()  != waterFrame->_asdu.data.size())
			return;

		int nStep = 5;
		for (BYTE n = 0 ; n < 40; n ++)
		{
			if (pWaterQuality_PAPR->GetBitValue(n) == true)
			{
				CWaterQuality_PARA_PER* pQuality_ONE = pWaterQuality_PAPR->GetOneQualityObject(n);

				UINT64 nBufValue = 0;
				DOUBLE dParaValue = 0.0f;

				if (n != 24)
				{
					nBufValue = (UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 0]) +
						(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 1]) * 100 +
						(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2]) * 10000 +
						(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 3]) * 1000000;
					nStep += 4;
				}
				else
				{

					nBufValue =  (UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 0]) +
						(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 1]) * 100 +
						(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2]) * 10000 +
						(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 3]) * 1000000 +
						(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 4]) * 100000000;
					nStep += 5;
				}

				if (pQuality_ONE)
				{
					dParaValue = (DOUBLE)nBufValue;
					if (pQuality_ONE->m_PointBits > 0)
					{
						for (int m = 0 ; m < pQuality_ONE->m_PointBits; m ++)
						{
							dParaValue /= 10;

						}
					}
					pQuality_ONE->m_DownValue = dParaValue;
				}

			}
		}
		UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_waterquality_down_call);
	}
}

//��ѯң���ն�ʵʱֵ����Ӧ֡��
void CSysDataTrans::IEC_waterdata_US_5BH(CPacketFrameFormat* waterFrame)   
{
	if (!waterFrame) return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;
	if (waterFrame->_asdu._AFN != 0x5B) return;
	if (!waterFrame->m_pStation) return;
	if (waterFrame->_asdu.data.size() < 2) return;


	BYTE QueryCmd = waterFrame->ControlC._Command;

	if (QueryCmd == waterFrame->m_pStation->GetQueryRealDataCmd())
		DeleteOneReSendObject(waterFrame->m_pStation->GetQueryRealDataCmd(),waterFrame->_asdu._AFN,waterFrame->_addr_info);

	if (QueryCmd == 2)  //����֡
	{

	}
	else if (QueryCmd == C_CMD_06_WLPARA)									//ˮλ
	{
		if (waterFrame->m_pStation != NULL)
		{
			int nLevelCount = ((int)waterFrame->_asdu.data.size()-2) / 3 ; //ˮλ�����
			int nAdded = nLevelCount - (int)waterFrame->m_pStation->GetWaterLevelParasList()->GetCount();
			if (nAdded > 0)
			{
				for (int i = 0 ; i < nAdded; i ++)
				{
					CWaterLevel_PAR_PER* pLevel_PARA = new CWaterLevel_PAR_PER();
					waterFrame->m_pStation->GetWaterLevelParasList()->AddTail(pLevel_PARA);
				}
			}

			for ( int i = 0 ; i < nLevelCount; i ++)
			{
				float RealV = ((float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 3 + 0]) * 0.01 +
					(float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 3 + 1]) +
					(float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 3 + 2]) * 100);
				CWaterLevel_PAR_PER* pLevel_PARA = waterFrame->m_pStation->GetWaterLevelPara(i);
				if (pLevel_PARA != NULL)
					pLevel_PARA->SetRealValue(RealV);
			}

			waterFrame->m_pStation->GetRtuAlarm_State()->SetAlarmStateByte(waterFrame->_asdu.data[nLevelCount * 3]);
			waterFrame->m_pStation->GetRtuTerminal_State()->SetTerminalStateByte(waterFrame->_asdu.data[nLevelCount*3 + 1]);

			UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_waterlevel_data_call);
		}
	}
	else if (QueryCmd == C_CMD_07_FLOWPARA)			//����
	{
		if (waterFrame->m_pStation != NULL)
		{
			int nFlowCount = ((int)waterFrame->_asdu.data.size() - 2)/ 6;   //������������
			int nAdded = nFlowCount -(int)waterFrame->m_pStation->GetFlowInitValumeParasList()->GetCount();
			if (nAdded > 0)
			{
				for (int i = 0; i < nAdded; i ++)
				{
					CFlowInitValume_PARA_PER* pFlow_PARA = new CFlowInitValume_PARA_PER();
					waterFrame->m_pStation->GetFlowInitValumeParasList()->AddTail(pFlow_PARA);
				}
			}

			for (int i = 0 ; i < nFlowCount ; i ++)
			{
				//�ۼ�ȡˮ��
				//DWORD CalTakeWater = (DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 0]) +
				//	(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 1]) * 100 +
				//	(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 2]) * 10000 +
				//	(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 3]) * 1000000;

				////˲ʱ����
				//WORD InsFlow   = (WORD)(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 4])) +
				//	(WORD)(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 5])) * 100;

				//�ۼ�ȡˮ��
				DWORD s1 = (DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 0]);
				DWORD s2 = (DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 1]) * 100;
				DWORD s3 = (DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 2]) * 10000;
				DWORD s4 = (DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 3]) * 1000000;
				DWORD s5 = (DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 4]) * 100000000;
				DWORD CalTakeWater = s1 + s2 + s3 + s4+s5;
				//˲ʱ����
				DWORD v1 = (WORD)(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 5]));
				DWORD v2 = (WORD)(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 6])) * 100;
				DWORD v3 = (WORD)(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 7])) * 10000;
				WORD InsFlow = v1 + v2 + v3;
				CFlowInitValume_PARA_PER* pFlow_PARA = waterFrame->m_pStation->GetFlowInitValumePara(i);
				if (pFlow_PARA)
				{
					pFlow_PARA->SetCalTakeValume(CalTakeWater);
					pFlow_PARA->SetInsFlowValume(InsFlow);
				}
			}

			waterFrame->m_pStation->GetRtuAlarm_State()->SetAlarmStateByte(waterFrame->_asdu.data[nFlowCount * 6]);
			waterFrame->m_pStation->GetRtuTerminal_State()->SetTerminalStateByte(waterFrame->_asdu.data[nFlowCount * 6 + 1]);

			UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_flowdata_call);
		}
	}
	else if (QueryCmd == C_CMD_08_WPPARA)			//ˮѹ
	{
		if (waterFrame->m_pStation != NULL)
		{
			int nPressCount = ((int)waterFrame->_asdu.data.size() - 2) / 4;   //ˮѹ�Ǳ���Ŀ
			int nAdded = nPressCount - (int)waterFrame->m_pStation->GetWaterPressureParasList()->GetCount();
			if (nAdded > 0)
			{
				for (int i = 0 ; i < nAdded; i ++)
				{
					CWaterPressure_PARA_PER* pPress_PARA = new CWaterPressure_PARA_PER();
					waterFrame->m_pStation->GetWaterPressureParasList()->AddTail(pPress_PARA);
				}
			}

			for (int i = 0 ; i < nPressCount; i ++)
			{
				//ˮѹʵʱֵ
				double realV = (double) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 4]) * 0.01 +
					(double) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 4 + 1]) +
					(double) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 4 + 2]) * 100 +
					(double) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 4 + 3]) * 10000;
				CWaterPressure_PARA_PER* pPress_PARA = waterFrame->m_pStation->GetWaterPressurePara(i);
				if (pPress_PARA)
					pPress_PARA->SetRealValue(realV);
			}

			waterFrame->m_pStation->GetRtuAlarm_State()->SetAlarmStateByte(waterFrame->_asdu.data[nPressCount * 4]);
			waterFrame->m_pStation->GetRtuTerminal_State()->SetTerminalStateByte(waterFrame->_asdu.data[nPressCount * 4 + 1]);

			UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_waterpress_data_call);
		}
	}
	else if (QueryCmd == C_CMD_09_WQPARA)			//ˮ��
	{
		if (waterFrame->m_pStation && waterFrame->m_pStation->GetWaterQualityPara())
		{
			CWaterQuality_PARA* pWaterQuality_PAPR = waterFrame->m_pStation->GetWaterQualityPara();

			if (waterFrame->_asdu.data.size() < 5) return;

			UINT64  WaterQualityCheckValue =(UINT64) (waterFrame->_asdu.data[0]) +
				((UINT64) waterFrame->_asdu.data[1] << 8) +
				((UINT64) waterFrame->_asdu.data[2] <<16) +
				((UINT64)waterFrame->_asdu.data[3] <<24) +
				((UINT64)waterFrame->_asdu.data[4] <<32);

			pWaterQuality_PAPR->SetQualityValue(WaterQualityCheckValue);

			if (pWaterQuality_PAPR->Get_Buffer_Count()  + 2 != waterFrame->_asdu.data.size())
				return;

			int nStep = 5;
			for (BYTE n = 0 ; n < 40; n ++)
			{
				if (pWaterQuality_PAPR->GetBitValue(n) == true)
				{
					CWaterQuality_PARA_PER* pQuality_ONE = pWaterQuality_PAPR->GetOneQualityObject(n);

					UINT64 nBufValue = 0;
					DOUBLE dParaValue = 0.0f;

					if (n != 24)
					{
						nBufValue = (UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 0]) +
							(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 1]) * 100 +
							(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2]) * 10000 +
							(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 3]) * 1000000;
						nStep += 4;
					}
					else
					{

						nBufValue =  (UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 0]) +
							(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 1]) * 100 +
							(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2]) * 10000 +
							(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 3]) * 1000000 +
							(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 4]) * 100000000;
						nStep += 5;
					}

					if (pQuality_ONE)
					{
						dParaValue = (DOUBLE)nBufValue;
						if (pQuality_ONE->m_PointBits > 0)
						{
							for (int m = 0 ; m < pQuality_ONE->m_PointBits; m ++)
							{
								dParaValue /= 10;

							}
						}
						pQuality_ONE->m_CurValue = dParaValue;
					}
				}
			}//end for (BYTE n = 0 ; n < 40; n ++)

			waterFrame->m_pStation->GetRtuAlarm_State()->SetAlarmStateByte(waterFrame->_asdu.data[nStep]);
			waterFrame->m_pStation->GetRtuTerminal_State()->SetTerminalStateByte(waterFrame->_asdu.data[nStep + 1]);


			UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_waterquality_data_call);
		}
	}
	else if (QueryCmd == C_CMD_10_GENERALPARA)		//�ۺϲ���
	{

	}
}


//��ѯ�ն˵���ʷ�ռ�¼����Ӧ֡��
void CSysDataTrans::IEC_waterdata_US_5CH(CPacketFrameFormat* waterFrame)   
{
	if (!waterFrame) return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;
	if (waterFrame->_asdu._AFN != 0x5C ||
		waterFrame->_asdu.data.size() < 3) return;

	DeleteOneReSendObject(waterFrame->ControlC._Command,0x5C,waterFrame->_addr_info);

	BOOL blCallSucc = FALSE;

	if (waterFrame->m_pStation != NULL)
	{
		if (waterFrame->m_pStation->GetDayHistoryObject()->GetDataType() ==
			waterFrame->ControlC._Command)
			blCallSucc = TRUE;

		CCMSApp* pApp = (CCMSApp*)AfxGetApp();
		if (pApp->Get_Cur_Op_Station() != waterFrame->m_pStation)
			return;

		CString sDayHisData = "";				//����ʷ������Ϣ
		CString strDate = "",strType = "";		//���ڡ�������Ϣ
		CString strTemp = "",strDataInfo="";	//������Ϣ

		SYSTEMTIME MaxTime,MinTime;
		GetLocalTime(&MaxTime);
		GetLocalTime(&MinTime);

		BYTE zbType = ((waterFrame->_asdu.data[0] >> 6) & 0x03);
		BYTE zdQuery = ((waterFrame->_asdu.data[1] >> 5) & 0x07);
		WORD wDay = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[0] & 0x3f);
		WORD wMonth = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[1] & 0x1f);
		WORD wYear	= 2000 + CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[2]);
		if (zbType == 2)		//��������
			strDate.Format("%04d��%02d��%02d�� ������ʷ�ռ�¼\r\n��ѯ�����ı��:%d\r\n",wYear,wMonth,wDay,zdQuery);
		else if (zbType == 1)	//�������
			strDate.Format("%04d��%02d��%02d�� ������ʷ�ռ�¼\r\n��ѯ�����ı��:%d\r\n",wYear,wMonth,wDay,zdQuery);

		if (waterFrame->ControlC._Command == C_CMD_06_WLPARA) //ˮλ
		{
			strType.Format("��������:ˮλ����\r\n");

			if (waterFrame->_asdu.data.size() < 3 + 3 * 26 + 5 * 2 )
				return;

			strDataInfo = "";

			int nStep = 3;
			for (int i=0; i < 24; i ++)
			{
				float fValue = (float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep]) * 0.01  +
					(float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 1]) +
					(float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2]) * 100;

				if (zbType == 2)
					strTemp.Format("%02d:00 %6.2f m\r\n",i,fValue);
				else if (zbType == 1)
					strTemp.Format("%02d:30 %6.2f m\r\n",i,fValue);

				strDataInfo += strTemp;

				nStep += 3;
			}

			MaxTime.wMinute = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 0]);
			MaxTime.wHour   = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 1]);
			MaxTime.wDay    = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2]);
			MaxTime.wMonth	= CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 3] & 0x1f);
			MaxTime.wYear   = 2000 + CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 4]);
			nStep += 5;

			float fMaxValue =	(float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep]) * 0.01  +
				(float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 1]) +
				(float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2]) * 100;

			nStep += 3;
			strTemp.Format("���ֵ:%6.2f m,%04d��%02d��%02d�� %02d:%02d\r\n",
				fMaxValue,
				MaxTime.wYear,
				MaxTime.wMonth,
				MaxTime.wDay,
				MaxTime.wHour,
				MaxTime.wMinute);
			strDataInfo += strTemp;

			MinTime.wMinute = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 0]);
			MinTime.wHour   = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 1]);
			MinTime.wDay    = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2]);
			MinTime.wMonth	= CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 3] & 0x1f);
			MinTime.wYear   = 2000 + CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 4]);
			nStep += 5;

			float fMinValue =	(float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep]) * 0.01  +
				(float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 1]) +
				(float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2]) * 100;

			strTemp.Format("��Сֵ:%6.2f m,%04d��%02d��%02d�� %02d:%02d\r\n",
				fMinValue,
				MinTime.wYear,
				MinTime.wMonth,
				MinTime.wDay,
				MinTime.wHour,
				MinTime.wMinute);
			strDataInfo += strTemp;

		}
		else if (waterFrame->ControlC._Command  == C_CMD_07_FLOWPARA) //����
		{
			strType.Format("��������:��������\r\n");

			if (waterFrame->_asdu.data.size() < 3 + 6 * 26 + 5 * 2)
				return;

			strDataInfo ="";

			int nStep = 3;
			for (int i = 0; i < 24; i ++)
			{
				//�ۼ�ȡˮ��
				DWORD CalTakeWater = (DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep+ 0]) +
					(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep+ 1]) * 100 +
					(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2]) * 10000 +
					(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 3]) * 1000000;
				//˲ʱ����
				WORD InsFlow   = (WORD)(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 4])) +
					(WORD)(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 5])) * 100;

				if (zbType == 2)
					strTemp.Format("%02d:00 �ۼ�ȡˮ��:%8d ������ ˲ʱ����:%4d ������/h\r\n",i,CalTakeWater,InsFlow);
				else if (zbType == 1)
					strTemp.Format("%02d:30 �ۼ�ȡˮ��:%8d ������ ˲ʱ����:%4d ������/h\r\n",i,CalTakeWater,InsFlow);
				strDataInfo += strTemp;

				nStep += 6;
			}

			MaxTime.wMinute = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 0]);
			MaxTime.wHour   = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 1]);
			MaxTime.wDay    = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2]);
			MaxTime.wMonth	= CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 3] & 0x1f);
			MaxTime.wYear   = 2000 + CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 4]);
			nStep += 5;


			//�ۼ�ȡˮ��
			DWORD CalTakeWater = (DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep+ 0]) +
				(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep+ 1]) * 100 +
				(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2]) * 10000 +
				(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 3]) * 1000000;
			//˲ʱ����
			WORD InsFlow   = (WORD)(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 4])) +
				(WORD)(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 5])) * 100;

			nStep += 4;

			strTemp.Format("���ֵ, �ۼ�ȡˮ��:%8d ������ ˲ʱ����:%4d ������/h,,%04d��%02d��%02d�� %02d:%02d\r\n",
				CalTakeWater,
				InsFlow,
				MaxTime.wYear,
				MaxTime.wMonth,
				MaxTime.wDay,
				MaxTime.wHour,
				MaxTime.wMinute);
			strDataInfo += strTemp;

			MinTime.wMinute = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 0]);
			MinTime.wHour   = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 1]);
			MinTime.wDay    = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2]);
			MinTime.wMonth	= CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 3] & 0x1f);
			MinTime.wYear   = 2000 + CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 4]);
			nStep += 5;

			//�ۼ�ȡˮ��
			CalTakeWater = (DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep+ 0]) +
				(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep+ 1]) * 100 +
				(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2]) * 10000 +
				(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 3]) * 1000000;
			//˲ʱ����
			InsFlow   = (WORD)(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 4])) +
				(WORD)(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 5])) * 100;

			strTemp.Format("��Сֵ, �ۼ�ȡˮ��:%8d ������ ˲ʱ����:%4d ������/h,,%04d��%02d��%02d�� %02d:%02d\r\n",
				CalTakeWater,
				InsFlow,
				MinTime.wYear,
				MinTime.wMonth,
				MinTime.wDay,
				MinTime.wHour,
				MinTime.wMinute);

			strDataInfo += strTemp;

		}
		else if (waterFrame->ControlC._Command == C_CMD_08_WPPARA)  //ˮѹ
		{
			strType.Format("��������:ˮѹ����\r\n");

			if (waterFrame->_asdu.data.size() < 3 + 4 * 26 + 5 * 2)
				return;

			strDataInfo = "";
			int nStep = 3;
			for (int i = 0; i < 24; i ++)
			{
				float fValue =  (float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep]) * 0.01  +
					(float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 1]) +
					(float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2]) * 100 +
					(float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 3]) * 10000;

				if (zbType == 2)
					strTemp.Format("%02d:00 %8.2f kPa\r\n",i,fValue);
				else if (zbType == 1)
					strTemp.Format("%02d:30 %8.2f kPa\r\n",i,fValue);

				strDataInfo += strTemp;

				nStep += 4;
			}


			MaxTime.wMinute = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 0]);
			MaxTime.wHour   = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 1]);
			MaxTime.wDay    = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2]);
			MaxTime.wMonth	= CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 3] & 0x1f);
			MaxTime.wYear   = 2000 + CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 4]);
			nStep += 5;

			float fMaxValue =	(float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep]) * 0.01  +
				(float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 1]) +
				(float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2]) * 100 +
				(float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 3]) * 10000;

			nStep += 4;
			strTemp.Format("���ֵ:%8.2f kPa,%04d��%02d��%02d�� %02d:%02d\r\n",
				fMaxValue,
				MaxTime.wYear,
				MaxTime.wMonth,
				MaxTime.wDay,
				MaxTime.wHour,
				MaxTime.wMinute);
			strDataInfo += strTemp;

			MinTime.wMinute = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 0]);
			MinTime.wHour   = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 1]);
			MinTime.wDay    = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2]);
			MinTime.wMonth	= CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 3] & 0x1f);
			MinTime.wYear   = 2000 + CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 4]);
			nStep += 5;

			float fMinValue =	(float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep]) * 0.01  +
				(float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 1]) +
				(float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2]) * 100 +
				(float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 3]) * 10000;

			strTemp.Format("��Сֵ:%8.2f kPa,%04d��%02d��%02d�� %02d:%02d\r\n",
				fMinValue,
				MinTime.wYear,
				MinTime.wMonth,
				MinTime.wDay,
				MinTime.wHour,
				MinTime.wMinute);
			strDataInfo += strTemp;
		}
		else if (waterFrame->ControlC._Command == C_CMD_09_WQPARA)  //ˮ��
		{
			strType.Format("��������:ˮ������\r\n");
			if (waterFrame->_asdu.data.size() < 3 + 5 + 5 * 2)
				return;

			INT64 wQType =  (UINT64) (waterFrame->_asdu.data[3]) +
				((UINT64) waterFrame->_asdu.data[4] << 8) +
				((UINT64) waterFrame->_asdu.data[5] <<16) +
				((UINT64) waterFrame->_asdu.data[6] <<24) +
				((UINT64) waterFrame->_asdu.data[7] <<32);

			bool blFinded = false;
			int  rPos = 0;
			for (int _pos = 0; _pos < 40; _pos ++ )
			{			
				UINT64 tmp = ((UINT64) 1 << _pos);
				if ((wQType & tmp) != 0) 
				{
					blFinded = true;
					rPos = _pos;
					break;
				}
			}

			if (blFinded == true)
			{
				CWaterQuality_PARA_PER* pWQuality = waterFrame->m_pStation->GetOne_WaterQualityPara((BYTE)rPos);
				if (pWQuality)
				{
					if (waterFrame->_asdu.data.size() < 3 + 5 + (int)pWQuality->m_BUF_Cnt * 26 + 5 * 2)
						return;

					strDataInfo = "";
					strTemp.Format("ˮ������:%s\r\n",pWQuality->m_strName);
					strDataInfo += strTemp;

					int nStep = 3 + 5;
					for ( int i = 0; i < 24; i ++)
					{
						UINT64 nBufValue = 0;
						DOUBLE dParaValue = 0.0f;

						if (rPos != 24)
						{
							nBufValue = (UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 0]) +
								(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 1]) * 100 +
								(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2]) * 10000 +
								(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 3]) * 1000000;
							nStep += 4;
						}
						else
						{

							nBufValue =		(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 0]) +
								(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 1]) * 100 +
								(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2]) * 10000 +
								(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 3]) * 1000000 +
								(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 4]) * 100000000;
							nStep += 5;
						}

						dParaValue = (DOUBLE)nBufValue;
						if (pWQuality->m_PointBits > 0)
						{
							for (int m = 0 ; m < pWQuality->m_PointBits; m ++)
							{
								dParaValue /= 10;

							}
						}

						if (zbType == 2)
							strTemp.Format("%02d:00 %10.*f %s\r\n",i,pWQuality->m_PointBits ,dParaValue,pWQuality->m_strUnit);
						else if (zbType == 1)
							strTemp.Format("%02d:30 %10.*f %s\r\n",i,pWQuality->m_PointBits ,dParaValue,pWQuality->m_strUnit);

						strDataInfo += strTemp;
					}


					MaxTime.wMinute = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 0]);
					MaxTime.wHour   = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 1]);
					MaxTime.wDay    = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2]);
					MaxTime.wMonth	= CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 3] & 0x1f);
					MaxTime.wYear   = 2000 + CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 4]);
					nStep += 5;

					UINT64 nBufValue = 0;
					DOUBLE dParaValue = 0.0f;

					if (rPos != 24)
					{
						nBufValue = (UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 0]) +
							(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 1]) * 100 +
							(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2]) * 10000 +
							(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 3]) * 1000000;
						nStep += 4;
					}
					else
					{

						nBufValue =		(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 0]) +
							(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 1]) * 100 +
							(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2]) * 10000 +
							(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 3]) * 1000000 +
							(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 4]) * 100000000;
						nStep += 5;
					}

					dParaValue = (DOUBLE)nBufValue;
					if (pWQuality->m_PointBits > 0)
					{
						for (int m = 0 ; m < pWQuality->m_PointBits; m ++)
						{
							dParaValue /= 10;
						}
					}

					strTemp.Format("���ֵ:%10.*f %s,%04d��%02d��%02d�� %02d:%02d\r\n",
						pWQuality->m_PointBits ,
						dParaValue,
						pWQuality->m_strUnit,
						MaxTime.wYear,
						MaxTime.wMonth,
						MaxTime.wDay,
						MaxTime.wHour,
						MaxTime.wMinute);

					strDataInfo += strTemp;


					MinTime.wMinute = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 0]);
					MinTime.wHour   = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 1]);
					MinTime.wDay    = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2]);
					MinTime.wMonth	= CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 3] & 0x1f);
					MinTime.wYear   = 2000 + CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 4]);
					nStep += 5;

					if (rPos != 24)
					{
						nBufValue = (UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 0]) +
							(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 1]) * 100 +
							(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2]) * 10000 +
							(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 3]) * 1000000;
						nStep += 4;
					}
					else
					{

						nBufValue =		(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 0]) +
							(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 1]) * 100 +
							(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2]) * 10000 +
							(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 3]) * 1000000 +
							(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 4]) * 100000000;
						nStep += 5;
					}

					dParaValue = (DOUBLE)nBufValue;
					if (pWQuality->m_PointBits > 0)
					{
						for (int m = 0 ; m < pWQuality->m_PointBits; m ++)
						{
							dParaValue /= 10;

						}
					}

					strTemp.Format("��Сֵ:%10.*f %s,%04d��%02d��%02d�� %02d:%02d\r\n",
						pWQuality->m_PointBits ,
						dParaValue,
						pWQuality->m_strUnit,
						MinTime.wYear,
						MinTime.wMonth,
						MinTime.wDay,
						MinTime.wHour,
						MaxTime.wMinute);

					strDataInfo += strTemp;
				}
			}
		}
		else if (waterFrame->ControlC._Command == C_CMD_11_RAINPARA) //����
		{
			strType.Format("��������:��������\r\n");

			if (waterFrame->_asdu.data.size() < 3 + 2 * 26 + 5 * 2)
				return;

			strDataInfo = "";
			int nStep = 3;
			for (int i = 0 ; i < 24; i ++)
			{
				float fValue = ((float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + i * 2 ]))* 0.1 +
					((float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + i * 2 + 1])) * 10;

				if (zbType == 2)
					strTemp.Format("%02d:00 %.1f mm\r\n",i, fValue);
				else if (zbType == 1)
					strTemp.Format("%02d:30 %.1f mm\r\n",i, fValue);

				strDataInfo += strTemp;
			}

			nStep = 3 + 24 * 2;
			MaxTime.wMinute = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 0]);
			MaxTime.wHour   = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 1]);
			MaxTime.wDay    = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2]);
			MaxTime.wMonth	= CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 3] & 0x1f);
			MaxTime.wYear   = 2000 + CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 4]);

			nStep += 5;
			float MaxData = ((float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 0])) * 0.1 +
				((float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 1])) * 10 ;

			strTemp.Format("���ֵ:%3.1f,%04d��%02d��%02d�� %02d:%02d\r\n",MaxData,
				MaxTime.wYear,
				MaxTime.wMonth,
				MaxTime.wDay,
				MaxTime.wHour,
				MaxTime.wMinute);
			strDataInfo += strTemp;

			nStep += 2;

			MinTime.wMinute = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 0]);
			MinTime.wHour   = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 1]);
			MinTime.wDay    = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2]);
			MinTime.wMonth	= CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 3] & 0x1f);
			MinTime.wYear   = 2000 + CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 4]);

			nStep += 5;

			float MinData = ((float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 0]))* 0.1 +
				((float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 1])) * 10;


			strTemp.Format("��Сֵ:%3.1f,%04d��%02d��%02d�� %02d:%02d\r\n",MinData,
				MinTime.wYear,
				MinTime.wMonth,
				MinTime.wDay,
				MinTime.wHour,
				MinTime.wMinute);

			strDataInfo += strTemp;

		}
		else if (waterFrame->ControlC._Command == C_CMD_12_MONIPARA) //ģ����
		{
			strType.Format("��������:ģ��������\r\n");


			if (waterFrame->_asdu.data.size() < 3 + 3 * 26 + 5 * 2)
				return;

			strDataInfo = "";
			for (int i = 0 ; i < 24; i ++)
			{
				DWORD Data = (DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[3 + i * 3]) +
					((DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[4 + i * 3])) * 100 +
					((DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[5 + i * 3] & 0x0f)) * 10000;
				if (zbType == 2)
					strTemp.Format("%02d:00 %d\r\n",i, Data);
				else if (zbType == 1)
					strTemp.Format("%02d:30 %d\r\n",i, Data);

				strDataInfo += strTemp;
			}


			int nStep = 3 + 24 * 3;

			MaxTime.wMinute = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 0]);
			MaxTime.wHour   = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 1]);
			MaxTime.wDay    = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2]);
			MaxTime.wMonth	= CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 3] & 0x1f);
			MaxTime.wYear   = 2000 + CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 4]);

			nStep += 5;

			DWORD MaxData = (DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 0]) +
				((DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 1])) * 100 +
				((DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2] & 0x0f)) * 10000;

			strTemp.Format("���ֵ:%5d,%04d��%02d��%02d�� %02d:%02d\r\n",MaxData,
				MaxTime.wYear,
				MaxTime.wMonth,
				MaxTime.wDay,
				MaxTime.wHour,
				MaxTime.wMinute);
			strDataInfo += strTemp;

			nStep += 3;

			MinTime.wMinute = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 0]);
			MinTime.wHour   = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 1]);
			MinTime.wDay    = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2]);
			MinTime.wMonth	= CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 3] & 0x1f);
			MinTime.wYear   = 2000 + CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 4]);

			nStep += 5;

			DWORD MinData = (DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 0]) +
				((DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 1])) * 100 +
				((DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2] & 0x0f)) * 10000;

			strTemp.Format("��Сֵ:%5d,%04d��%02d��%02d�� %02d:%02d\r\n",MinData,
				MinTime.wYear,
				MinTime.wMonth,
				MinTime.wDay,
				MinTime.wHour,
				MinTime.wMinute);
			strDataInfo += strTemp;

		}

		sDayHisData += strType;
		sDayHisData += strDate;
		sDayHisData += strDataInfo;

		UpdateRtuDayHistoryDlg(waterFrame->m_pStation,blCallSucc,sDayHisData);
	}
}

//��ѯң���ն˵��¼���¼����Ӧ֡��
void CSysDataTrans::IEC_waterdata_US_5DH(CPacketFrameFormat* waterFrame)   
{
	if (!waterFrame) return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;
	if (waterFrame->_asdu._AFN != 0x5D) return;
	if (waterFrame->ControlC._Command != 0x04) return;
	if (waterFrame->_asdu.data.size() !=64) return;

	if (waterFrame->m_pStation != NULL)
	{

		for (int i = 0 ;i < 32; i ++)
		{
			WORD eventRecord = (waterFrame->_asdu.data[i*2] | waterFrame->_asdu.data[i*2 +1]<<8);
			CEventRecord_PER* pEvent = waterFrame->m_pStation->GetEventRecordObject(i);
			if (pEvent != NULL)
			{
				pEvent->wEventCount = eventRecord;
			}
		}

		UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_eventrecord_call);
	}

	DeleteOneReSendObject(C_CMD_05_MSUERSTATE,0x5D,waterFrame->_addr_info);
}

//��ѯң���ն�״̬�ͱ���״̬����Ӧ֡��
void CSysDataTrans::IEC_waterdata_US_5EH(CPacketFrameFormat* waterFrame)   
{
	if (!waterFrame) return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;
	if (waterFrame->ControlC._Command != C_CMD_04_MSUREPARA) return;
	if (waterFrame->_asdu._AFN != 0x5E || waterFrame->_asdu.data.size() != 2) return;


	DeleteOneReSendObject(C_CMD_05_MSUERSTATE,0x5E,waterFrame->_addr_info);
	if (waterFrame->m_pStation)
	{
		waterFrame->m_pStation->GetRtuAlarm_State()->SetAlarmStateByte(waterFrame->_asdu.data[0]);
		waterFrame->m_pStation->GetRtuTerminal_State()->SetTerminalStateByte(waterFrame->_asdu.data[1]);

		UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_alarmstate_call);
	}

}

//��ѯˮ�õ��ʵʱ�������ݣ���Ӧ֡��
void CSysDataTrans::IEC_waterdata_US_5FH(CPacketFrameFormat* waterFrame)   
{
	if (!waterFrame)	return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;
	if (waterFrame->ControlC._Command != C_CMD_04_MSUREPARA) return;
	if (waterFrame->_asdu._AFN != 0x5F ||
		waterFrame->_asdu.data.size() != 12)
		return ;

	DeleteOneReSendObject(C_CMD_05_MSUERSTATE,0x5F,waterFrame->_addr_info);
	if (waterFrame->m_pStation)
	{
		WORD _value = 0;
		_value = (WORD)(waterFrame->_asdu.data[0] | waterFrame->_asdu.data[1] <<8);
		waterFrame->m_pStation->GetWaterPump_WorkData()->SetVoltageA(_value);

		_value = (WORD)(waterFrame->_asdu.data[2] | waterFrame->_asdu.data[3] <<8);
		waterFrame->m_pStation->GetWaterPump_WorkData()->SetVoltageB(_value);

		_value = (WORD)(waterFrame->_asdu.data[4] | waterFrame->_asdu.data[5] <<8);
		waterFrame->m_pStation->GetWaterPump_WorkData()->SetVoltageC(_value);

		_value = (WORD)(waterFrame->_asdu.data[6] | waterFrame->_asdu.data[7] <<8);
		waterFrame->m_pStation->GetWaterPump_WorkData()->SetVoltageA(_value);

		_value = (WORD)(waterFrame->_asdu.data[8] | waterFrame->_asdu.data[9] <<8);
		waterFrame->m_pStation->GetWaterPump_WorkData()->SetVoltageB(_value);

		_value = (WORD)(waterFrame->_asdu.data[10] | waterFrame->_asdu.data[11] <<8);
		waterFrame->m_pStation->GetWaterPump_WorkData()->SetVoltageC(_value);

	}
}

//��ѯ�ն�վת���м������볤ֵ����Ӧ֡��
void CSysDataTrans::IEC_waterdata_US_60H(CPacketFrameFormat* waterFrame)   
{
	if (!waterFrame) return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;
}

//��ѯң���ն�ͼ���¼����Ӧ֡��
void CSysDataTrans::IEC_waterdata_US_61H(CPacketFrameFormat* waterFrame)   
{
	if (!waterFrame) return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;

}

//��ѯ�м�վת���ն˵�ַ����Ӧ֡)
void CSysDataTrans::IEC_waterdata_US_62H(CPacketFrameFormat* waterFrame)   
{
	if (!waterFrame) return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;
}

//��ѯ�м�վ״̬���л���¼����Ӧ֡��
void CSysDataTrans::IEC_waterdata_US_63H(CPacketFrameFormat* waterFrame)   
{
	if (!waterFrame)  return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;
}

//��ѯң��վ�������ݱ���ֵ��ANF��64H��
void CSysDataTrans::IEC_waterdata_US_64H(CPacketFrameFormat* waterFrame)  
{
	if (!waterFrame)  return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;
	if (waterFrame->_asdu._AFN != 0x64  ||
		waterFrame->_asdu.data.size() < 4) return;

	DeleteOneReSendObject(C_CMD_04_MSUREPARA,0x64,waterFrame->_addr_info);

	if (waterFrame->m_pStation != NULL)
	{
		int nRainAlertCount = waterFrame->_asdu.data.size()/ 4;

		int nAdded = nRainAlertCount- waterFrame->m_pStation->GetRainAlertParasList()->GetCount();

		if (nAdded > 0 )
		{
			for (int i = 0; i< nAdded; i ++)
			{
				CRainAlert_PARA_PER* pRain_PARA = new CRainAlert_PARA_PER();
				waterFrame->m_pStation->GetRainAlertParasList()->AddTail(pRain_PARA);
			}
		}

		for (int i = 0 ;i < nRainAlertCount; i ++)
		{

			float RainAlert = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i*4]) * 0.1 +
				CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[1+i*4]) *10;

			WORD  perTime =  CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[2+i*4]) +
				CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[3+i*4]) * 100;


			CRainAlert_PARA_PER* pRain_PARA = waterFrame->m_pStation->GetRainAlartPara(i);
			if (pRain_PARA)
			{
				pRain_PARA->SetPerTime(perTime);
				pRain_PARA->SetRainAlertValue(RainAlert);
			}
		}

		UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_rainalert_call);
	}
}

//��ѯң��վģ�������������ޣ�ANF��65H��
void CSysDataTrans::IEC_waterdata_US_65H(CPacketFrameFormat* waterFrame)   
{
	if (!waterFrame)  return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;
	if (waterFrame->_asdu._AFN != 0x65 ||
		waterFrame->_asdu.data.size() < 6) return;
	DeleteOneReSendObject(C_CMD_04_MSUREPARA,0x65,waterFrame->_addr_info);

	if (waterFrame->m_pStation != NULL)
	{
		int nAnalogCount = waterFrame->_asdu.data.size() / 6;

		int nAdded = nAnalogCount - waterFrame->m_pStation->GetAnalogLimitParasList()->GetCount();
		if (nAdded > 0)
		{
			for (int i = 0; i <nAdded; i ++)
			{
				CAnalogLimit_PARA_PER* pAnalog_PARA = new CAnalogLimit_PARA_PER();
				waterFrame->m_pStation->GetAnalogLimitParasList()->AddTail(pAnalog_PARA);
			}
		}

		for (int i = 0; i < nAnalogCount; i++)
		{
			DWORD UpLimit = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i*6]) +
				CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[1+i*6]) * 100 +
				CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[2+i*6] & 0x0F) * 10000;

			DWORD DownLimit = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[3 + i * 6]) +
				CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[4 + i * 6]) * 100 +
				CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[5 + i * 6] & 0x0F) * 10000;

			CAnalogLimit_PARA_PER* pAnalog_PARA  = waterFrame->m_pStation->GetAnalogLimitPara(i);
			if (pAnalog_PARA)
			{
				pAnalog_PARA->SetAnalogDownLimit(DownLimit);
				pAnalog_PARA->SetAnalogUpLimit(UpLimit);
			}
		}

		UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_analogminmax_call);
	}
}



//��ѯң���ն���չ����״̬���ն�״̬��ANF��66H��
void CSysDataTrans::IEC_waterdata_US_66H(CPacketFrameFormat* waterFrame)   
{
	if (!waterFrame)  return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;
	if (waterFrame->_asdu._AFN != 0x66 ) return;

	DeleteOneReSendObject(C_CMD_05_MSUERSTATE,0x66,waterFrame->_addr_info);

	if (waterFrame->m_pStation)
	{
		if (waterFrame->_asdu.data.size() < 2) return;

		waterFrame->m_pStation->GetRtuExAlarm_State()->SetExAlarmStateByte(waterFrame->_asdu.data[0]);
		waterFrame->m_pStation->GetRtuTerminal_State()->SetTerminalStateByte(waterFrame->_asdu.data[1]);

		UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_exalarmstate_call);
	}
}

//��ѯң���ն���չʵʱֵ��ANF��67H��
void CSysDataTrans::IEC_waterdata_US_67H(CPacketFrameFormat* waterFrame)  
{
	if (!waterFrame) return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;
	if (waterFrame->_asdu.data.size() < 2) return;


	DeleteOneReSendObject(waterFrame->m_pStation->GetQueryRealDataCmd(),waterFrame->_asdu._AFN,waterFrame->_addr_info);

	BYTE QueryCmd = waterFrame->m_pStation->GetQueryRealDataCmd();
	if (QueryCmd == C_CMD_11_RAINPARA)  //����
	{
		if (waterFrame->m_pStation)
		{
			int nRainCount = (waterFrame->_asdu.data.size() -2) / 2;
			int nAdded = nRainCount - waterFrame->m_pStation->GetRainAlertParasList()->GetCount();

			if (nAdded > 0 )
			{
				for (int i = 0; i <nAdded; i ++)
				{
					CRainAlert_PARA_PER* pRain_PARA = new CRainAlert_PARA_PER();
					waterFrame->m_pStation->GetRainAlertParasList()->AddTail(pRain_PARA);
				}
			}

			for (int i = 0; i < nRainCount; i ++)
			{
				float RainValue = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i*2]) * 0.1 +
					CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[1+i*2]) *10;

				CRainAlert_PARA_PER* pRain_PARA = waterFrame->m_pStation->GetRainAlartPara(i);
				if (pRain_PARA)
					pRain_PARA->SetRealTimeValue(RainValue);
			}

			waterFrame->m_pStation->GetRtuExAlarm_State()->SetExAlarmStateByte(waterFrame->_asdu.data[nRainCount * 2 ]);
			waterFrame->m_pStation->GetRtuTerminal_State()->SetTerminalStateByte(waterFrame->_asdu.data[nRainCount * 2 + 1]);

			UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_rain_data_call);
		}
	}
	else if (QueryCmd == C_CMD_12_MONIPARA) //ģ����
	{
		if (waterFrame->m_pStation)
		{
			int AnalogCount = (waterFrame->_asdu.data.size() - 2) / 3;
			int nAdded = AnalogCount - waterFrame->m_pStation->GetAnalogLimitParasList()->GetCount();
			if (nAdded > 0)
			{
				for ( int i = 0; i < nAdded; i ++)
				{
					CAnalogLimit_PARA_PER* pAnalog_PARA = new CAnalogLimit_PARA_PER();
					waterFrame->m_pStation->GetAnalogLimitParasList()->AddTail(pAnalog_PARA);
				}
			}

			for (int i = 0 ;i < AnalogCount ; i ++)
			{
				DWORD RealValue = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i*3]) +
					CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[1+i*3]) * 100 +
					CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[2+i*3] & 0x0F) * 10000;

				CAnalogLimit_PARA_PER* pAnalog_PARA = waterFrame->m_pStation->GetAnalogLimitPara(i);
				if (pAnalog_PARA)
					pAnalog_PARA->SetRealTimeValue(RealValue);
			}

			waterFrame->m_pStation->GetRtuExAlarm_State()->SetExAlarmStateByte(waterFrame->_asdu.data[AnalogCount * 3 ]);
			waterFrame->m_pStation->GetRtuTerminal_State()->SetTerminalStateByte(waterFrame->_asdu.data[AnalogCount * 3 + 1]);

			UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_analog_data_call);
		}
	}
	else if (QueryCmd == C_CMD_13_PULSEPARA)   //���
	{
		if (waterFrame->m_pStation)
		{
			int pulseCunt = (waterFrame->_asdu.data.size() - 2) / 22;

			for (int i = 0; i < pulseCunt; i ++)
			{
				UINT64 code = 0;
				code = (UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 0] ) +
					(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 1] ) * (UINT64) 100 +
					(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 2] ) * (UINT64) 10000 +
					(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 3] ) * (UINT64) 1000000 +
					(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 4] ) * (UINT64) 100000000 +
					(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 5] ) * (UINT64) 10000000000 ;

				CPulsePara_PER *pPulse = waterFrame->m_pStation->GetPulseParaObject(code);

				if (pPulse == NULL)
				{
					pPulse = new CPulsePara_PER(code);
					waterFrame->m_pStation->InsertPulseParaObject(pPulse);					
				}

				double dbvalue = 0.0;
				dbvalue = (double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 6] ) * (double) 0.01 +
					(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 7] ) * (double) 1 +
					(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 8] ) * (double) 100 +
					(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 9] ) * (double) 10000;
				pPulse->SetValue1(dbvalue);


				dbvalue = (double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 10] ) * (double) 0.01 +
					(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 11] ) * (double) 1 +
					(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 12] ) * (double) 100 +
					(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 13] ) * (double) 10000;
				pPulse->SetValue2(dbvalue);


				dbvalue = (double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 14] ) * (double) 0.01 +
					(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 15] ) * (double) 1 +
					(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 16] ) * (double) 100 +
					(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 17] ) * (double) 10000;
				pPulse->SetValue3(dbvalue);


				dbvalue = (double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 18] ) * (double) 0.01 +
					(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 19] ) * (double) 1 +
					(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 20] ) * (double) 100 +
					(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 21] ) * (double) 10000;
				pPulse->SetValue4(dbvalue);

			}

			waterFrame->m_pStation->GetRtuExAlarm_State()->SetExAlarmStateByte(waterFrame->_asdu.data[pulseCunt * 22 ]);
			waterFrame->m_pStation->GetRtuTerminal_State()->SetTerminalStateByte(waterFrame->_asdu.data[pulseCunt * 22 + 1]);

			UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_pulsepara_call);
		}
	}
}





//��ѯң��վ���λ�������������ޣ�ANF��6AH��//add by yhp20160614
void CSysDataTrans::IEC_waterdata_US_6AH(CPacketFrameFormat* waterFrame)   
{
	if (!waterFrame)  return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;
	if (waterFrame->_asdu._AFN != 0x65 ||
		waterFrame->_asdu.data.size() < 6) return;
	DeleteOneReSendObject(C_CMD_04_MSUREPARA,0x65,waterFrame->_addr_info);

	if (waterFrame->m_pStation != NULL)
	{
		int nDisplaceCount = waterFrame->_asdu.data.size() / 12;

		int nAdded = nDisplaceCount - waterFrame->m_pStation->GetDisplaceLimitParasList()->GetCount();
		if (nAdded > 0)
		{
			for (int i = 0; i <nAdded; i ++)
			{
				CDisplace_PARA_PER* pDisplace_PARA = new CDisplace_PARA_PER();
				waterFrame->m_pStation->GetDisplaceLimitParasList()->AddTail(pDisplace_PARA);
			}
		}

		for (int i = 0; i < nDisplaceCount; i++)
		{
			DWORD XUpLimit = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i*12]) +
				CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[1+i*12]) * 100 +
				CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[2+i*12] & 0x0F) * 10000;

			DWORD XDownLimit = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[3 + i * 12]) +
				CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[4 + i * 12]) * 100 +
				CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[5 + i * 12] & 0x0F) * 10000;

			DWORD YUpLimit = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[6+i*12]) +
				CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[7+i*12]) * 100 +
				CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[8+i*12] & 0x0F) * 10000;

			DWORD YDownLimit = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[9 + i * 12]) +
				CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[10 + i * 12]) * 100 +
				CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[11 + i * 12] & 0x0F) * 10000;

			CDisplace_PARA_PER* pDisplace_PARA  = waterFrame->m_pStation->GetDisplaceLimitPara(i);
			if (pDisplace_PARA)
			{
				pDisplace_PARA->SetDisplaceXDownLimit(XDownLimit);
				pDisplace_PARA->SetDisplaceXUpLimit(XUpLimit);
				pDisplace_PARA->SetDisplaceYDownLimit(YDownLimit);
				pDisplace_PARA->SetDisplaceYUpLimit(YUpLimit);
				pDisplace_PARA->SetDisplaceNo(i+1);
			}
		}

		UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_displaceminmax_call);
	}
}



//��ѯң��վ�¶������������ޣ�ANF��6BH��//add by yhp20160614
void CSysDataTrans::IEC_waterdata_US_6BH(CPacketFrameFormat* waterFrame)   
{
	if (!waterFrame)  return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;
	if (waterFrame->_asdu._AFN != 0x6B ||
		waterFrame->_asdu.data.size() < 6) return;
	DeleteOneReSendObject(C_CMD_04_MSUREPARA,0x65,waterFrame->_addr_info);

	if (waterFrame->m_pStation != NULL)
	{
		int nTemperaCount = waterFrame->_asdu.data.size() / 4;

		int nAdded = nTemperaCount - waterFrame->m_pStation->GetTemperaLimitParasList()->GetCount();
		if (nAdded > 0)
		{
			for (int i = 0; i <nAdded; i ++)
			{
				CTempera_PARA_PER* pTempera_PARA = new CTempera_PARA_PER();
				waterFrame->m_pStation->GetTemperaLimitParasList()->AddTail(pTempera_PARA);
			}
		}

		for (int i = 0; i < nTemperaCount; i++)
		{
			DWORD UpLimit = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i*6]) +
				CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[1+i*6]) * 100;

			DWORD DownLimit = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[3 + i * 6]) +
				CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[4 + i * 6]) * 100;

			CTempera_PARA_PER* pTempera_PARA  = waterFrame->m_pStation->GetTemperaLimitPara(i);
			if (pTempera_PARA)
			{
				pTempera_PARA->SetTemperaDownLimit(DownLimit/10.0);
				pTempera_PARA->SetTemperaUpLimit(UpLimit/10.0);
			}
		}

		UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_temperaminmax_call);
	}
}



//ң������ˮ�û���/բ�ţ���Ӧ֡��
void CSysDataTrans::IEC_waterdata_US_92H(CPacketFrameFormat* waterFrame)   
{
	if (!waterFrame) return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;
	if (waterFrame->_asdu._AFN != 0x92) return;

	DeleteOneReSendObject(C_CMD_01_CMD,0x92,waterFrame->_addr_info);

	if (waterFrame->m_pStation)
	{
		if (waterFrame->_asdu.data.size() != 1) return;

		BYTE Code = ((waterFrame->_asdu.data[0] & 0x0f));
		waterFrame->m_pStation->GetCurPumpGateObjecte()->SetPumpGate_Code(Code);

		BYTE ResultFlag = ((waterFrame->_asdu.data[0] & 0xf0) >> 4);

		bool blYK_Operate = false;
		if (ResultFlag == 0x0A )  //ң������ˮ�û���/բ�ųɹ�
		{
			blYK_Operate = true;
		}

		//���½���
		if (blYK_Operate == true)
			waterFrame->m_pStation->Set_YK_Op_Result(YKStart_PumpGate_SUC);
		else if (blYK_Operate == false)
			waterFrame->m_pStation->Set_YK_Op_Result(YKStart_PumpGate_ERR);

		UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_yk_operate);
	}

}

//ң�عر�ˮ�û���/բ�ţ���Ӧ֡��
void CSysDataTrans::IEC_waterdata_US_93H(CPacketFrameFormat* waterFrame)   
{
	if (!waterFrame) return;
	if (!waterFrame) return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;
	if (waterFrame->_asdu._AFN != 0x93) return;

	DeleteOneReSendObject(C_CMD_01_CMD,0x93,waterFrame->_addr_info);

	if (waterFrame->m_pStation)
	{
		if (waterFrame->_asdu.data.size() != 1) return;

		BYTE Code = ((waterFrame->_asdu.data[0] & 0x0f));
		waterFrame->m_pStation->GetCurPumpGateObjecte()->SetPumpGate_Code(Code);

		BYTE ResultFlag = ((waterFrame->_asdu.data[0] & 0xf0) >> 4);

		bool blYK_Operate = false;
		if (ResultFlag == 0x0A )  //ң�عر�ˮ�û���/բ�ųɹ�
		{
			blYK_Operate = true;
		}

		//���½���
		if (blYK_Operate == true)
			waterFrame->m_pStation->Set_YK_Op_Result(YKClose_PumpGate_SUC);
		else if (blYK_Operate == false)
			waterFrame->m_pStation->Set_YK_Op_Result(YKClose_PumpGate_ERR);

		UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_yk_operate);
	}
}

//ң���ն˻��м�վͨ�Ż��л�����Ӧ֡��
void CSysDataTrans::IEC_waterdata_US_94H(CPacketFrameFormat* waterFrame)   
{
	if (!waterFrame) return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;

}


//ң���м�վ�������л�����Ӧ֡��
void CSysDataTrans::IEC_waterdata_US_95H(CPacketFrameFormat* waterFrame)   
{
	if (!waterFrame) return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;

}

//��ң���ն�IC��������Ч����Ӧ֡��
void CSysDataTrans::IEC_waterdata_US_30H(CPacketFrameFormat* waterFrame)   
{
	if (!waterFrame) return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;
	if (waterFrame->_asdu._AFN != 0x30) return;

	DeleteOneReSendObject(C_CMD_01_CMD,0x30,waterFrame->_addr_info);

	if (waterFrame->m_pStation)
	{
		if (waterFrame->_asdu.data.size() <1) return;

		bool blResult = false;

		if (waterFrame->_asdu.data[0] == 0x5A)  //��IC��������Чִ�����
		{
			blResult = true;
		}

		//���½���
		if (blResult == true)
			waterFrame->m_pStation->Set_YK_Op_Result(YKICCard_Enable_SUC);
		else if (blResult == false)
			waterFrame->m_pStation->Set_YK_Op_Result(YKICCard_Enable_ERR);

		UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_yk_operate);
	}
}

//ȡ��ң���ն�IC�����ܣ���Ӧ֡��
void CSysDataTrans::IEC_waterdata_US_31H(CPacketFrameFormat* waterFrame)   
{
	if (!waterFrame) return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;
	if (waterFrame->_asdu._AFN != 0x31) return;
	DeleteOneReSendObject(C_CMD_01_CMD,0x31,waterFrame->_addr_info);


	if (waterFrame->m_pStation)
	{
		if (waterFrame->_asdu.data.size() <1) return;

		bool blResult = false;

		if (waterFrame->_asdu.data[0] == 0x5A)  //��IC��������Чִ�����
		{
			blResult = true;
		}

		//���½���
		if (blResult == true)
			waterFrame->m_pStation->Set_YK_Op_Result(YKICCard_DisEnable_SUC);
		else if (blResult == false)
			waterFrame->m_pStation->Set_YK_Op_Result(YKICCard_DisEnable_ERR);

		UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_yk_operate);
	}	
}

//��ֵ���ƹ���Ͷ�루��Ӧ֡��
void CSysDataTrans::IEC_waterdata_US_32H(CPacketFrameFormat* waterFrame)   
{
	if (!waterFrame) return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;
	if (waterFrame->_asdu._AFN != 0x32) return;

	DeleteOneReSendObject(C_CMD_01_CMD,0x32,waterFrame->_addr_info);

	if (waterFrame->m_pStation)
	{
		if (waterFrame->_asdu.data.size() <1) return;

		bool blResult = false;

		if (waterFrame->_asdu.data[0] == 0x5A)  //��ֵ���ƹ���Ͷ��ִ�����
		{
			blResult = true;
		}

		//���½���
		if (blResult == true)
			waterFrame->m_pStation->Set_YK_Op_Result(YKZD_Input_SUC);
		else if (blResult == false)
			waterFrame->m_pStation->Set_YK_Op_Result(YKZD_Input_ERR);

		UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_yk_operate);
	}	
}

//��ֵ���ƹ����˳�����Ӧ֡��
void CSysDataTrans::IEC_waterdata_US_33H(CPacketFrameFormat* waterFrame)   
{
	if (!waterFrame) return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;
	if (waterFrame->_asdu._AFN != 0x33) return;

	DeleteOneReSendObject(C_CMD_01_CMD,0x33,waterFrame->_addr_info);

	if (waterFrame->m_pStation)
	{
		if (waterFrame->_asdu.data.size() <1) return;

		bool blResult = false;

		if (waterFrame->_asdu.data[0] == 0x5A)  //��ֵ���ƹ����˳�ִ�����
		{
			blResult = true;
		}

		//���½���
		if (blResult  == true)
			waterFrame->m_pStation->Set_YK_Op_Result(YKZD_Exit_SUC);
		else if(blResult == false)
			waterFrame->m_pStation->Set_YK_Op_Result(YKZD_Exit_ERR);

		UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_yk_operate);
	}	
}

//��ֵ���趨����Ӧ֡��
void CSysDataTrans::IEC_waterdata_US_34H(CPacketFrameFormat* waterFrame)   
{
	if (!waterFrame) return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;

}

//��λң���ն˲�����״̬�����Ӧ֡��
void CSysDataTrans::IEC_waterdata_US_90H(CPacketFrameFormat* waterFrame)   
{
	if (!waterFrame) return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;
	if (waterFrame->_asdu._AFN != 0x90) return;

	DeleteOneReSendObject(C_CMD_01_CMD,0x90,waterFrame->_addr_info);

	if (waterFrame->m_pStation)
	{
		if (waterFrame->_asdu.data.size() <1) return;

		bool blResult = false;

		if (waterFrame->_asdu.data[0] == 0x5A)  //��λң���ն�ִ�����
		{
			blResult = true;
		}

		//���½���
		if (blResult  == true)
			waterFrame->m_pStation->Set_YK_Op_Result(YKReset_Rtu_SUC);
		else if (blResult  == false)
			waterFrame->m_pStation->Set_YK_Op_Result(YKReset_Rtu_ERR);

		UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_yk_operate);
	}	
}

//���ң���ն˵���ʷ���ݵ�Ԫ����Ӧ֡��
void CSysDataTrans::IEC_waterdata_US_91H(CPacketFrameFormat* waterFrame)   
{
	if (!waterFrame) return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;
	if (waterFrame->_asdu._AFN != 0x91) return;

	DeleteOneReSendObject(C_CMD_01_CMD,0x91,waterFrame->_addr_info);

	if (waterFrame->m_pStation)
	{
		if (waterFrame->_asdu.data.size() < 1) return;
		waterFrame->m_pStation->GetClearHistoryDataPara()->SetClearHistoryPARA(waterFrame->_asdu.data[0]);
		waterFrame->m_pStation->Set_YK_Op_Result(YKClear_His_SUC);

		UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_yk_operate);
	}
}


//��ʱ�Ա�ʵʱ����
void CSysDataTrans::IEC_waterdata_US_80H(CPacketFrameFormat* waterFrame)  
{
	if(!waterFrame)  return;
	if (waterFrame->ControlC._DIR != UP_ARROW)  return;
	if (waterFrame->_asdu._AFN != 0x80)	 return;
	if (waterFrame->_asdu.data.size() < 2) return;

	if (waterFrame->GetRepeatType() == false)
	{
		COneData_INFO* pSave_data = new COneData_INFO();
		BuildBasicDataInfo(pSave_data,waterFrame);

		if(waterFrame->ControlC._Command == 5) //ˮλ
		{
			pSave_data->mDataType = COneData_INFO::WATER_L_DATA;

			int nLevelCount = (waterFrame->_asdu.data.size()-2) / 3 ; //ˮλ�����
			for ( int i = 0 ; i < nLevelCount; i ++)
			{
				float RealV = ((float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 3 + 0]) * 0.01 +
					(float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 3 + 1]) +
					(float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 3 + 2]) * 100);

				//������ݴ洢
				CWaterLevel_PAR_PER* pLevel = new CWaterLevel_PAR_PER();
				pLevel->SetRealValue(RealV);
				pSave_data->m_lstWaterLevel.AddTail(pLevel);

			}
			if (waterFrame->m_pStation != NULL)
			{
				waterFrame->m_pStation->GetRtuAlarm_State()->SetAlarmStateByte(waterFrame->_asdu.data[nLevelCount * 3]);
				waterFrame->m_pStation->GetRtuTerminal_State()->SetTerminalStateByte(waterFrame->_asdu.data[nLevelCount* 3 + 1]);
			}
		}
		else if (waterFrame->ControlC._Command == 6) //����
		{

			pSave_data->mDataType = COneData_INFO::FLOW_V_DATA;

			int nFlowCount = (waterFrame->_asdu.data.size() - 2)/ 6;   //������������

			for (int i = 0 ; i < nFlowCount ; i ++)
			{
				//�ۼ�ȡˮ��
				DWORD CalTakeWater = (DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 0]) +
					(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 1]) * 100 +
					(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 2]) * 10000 +
					(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 3]) * 1000000;

				//˲ʱ����
				WORD InsFlow   = (WORD)(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 4])) +
					(WORD)(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 5])) * 100;


				//������ݴ洢
				CFlowInitValume_PARA_PER* pFlow = new CFlowInitValume_PARA_PER();
				pFlow->SetCalTakeValume( CalTakeWater);
				pFlow->SetInsFlowValume(InsFlow);
				pSave_data->m_lstFlow.AddTail(pFlow);

			}

			if (waterFrame->m_pStation)
			{
				waterFrame->m_pStation->GetRtuAlarm_State()->SetAlarmStateByte(waterFrame->_asdu.data[nFlowCount * 6]);
				waterFrame->m_pStation->GetRtuTerminal_State()->SetTerminalStateByte(waterFrame->_asdu.data[nFlowCount * 6 + 1]);
			}
		}
		else if (waterFrame->ControlC._Command == 7)  //ˮѹ
		{
			pSave_data->mDataType = COneData_INFO::WATER_P_DATA;

			int nPressCount = (waterFrame->_asdu.data.size() - 2) / 4;   //ˮѹ�Ǳ���Ŀ

			for (int i = 0 ; i < nPressCount; i ++)
			{
				//ˮѹʵʱֵ
				double realV = (double) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 4]) * 0.01 +
					(double) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 4 + 1]) +
					(double) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 4 + 2]) * 100 +
					(double) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 4 + 3]) * 10000;


				//������ݴ洢
				CWaterPressure_PARA_PER* pPress = new CWaterPressure_PARA_PER();
				pPress->SetRealValue(realV);
				pSave_data->m_lstWaterPress.AddTail(pPress);

			}
			if( waterFrame->m_pStation != NULL)
			{
				waterFrame->m_pStation->GetRtuAlarm_State()->SetAlarmStateByte(waterFrame->_asdu.data[nPressCount * 4]);
				waterFrame->m_pStation->GetRtuTerminal_State()->SetTerminalStateByte(waterFrame->_asdu.data[nPressCount * 4 + 1]);
			}


		}
		else if (waterFrame->ControlC._Command == 8) //ˮ��
		{
			pSave_data->mDataType = COneData_INFO::WATER_Q_DATA;

			//���ĳ��Ȳ���ʱ
			if (waterFrame->_asdu.data.size() < 2 + 5)
			{
				delete pSave_data;
				pSave_data = NULL;
				return;
			}

			if (waterFrame->m_pStation && waterFrame->m_pStation->GetWaterQualityPara())
			{
				CWaterQuality_PARA* pWaterQuality_PAPR = waterFrame->m_pStation->GetWaterQualityPara();

				if (waterFrame->_asdu.data.size() < 5) return;

				UINT64  WaterQualityCheckValue =(UINT64) (waterFrame->_asdu.data[0]) +
					((UINT64)waterFrame->_asdu.data[1] << 8) +
					((UINT64)waterFrame->_asdu.data[2] <<16) +
					((UINT64)waterFrame->_asdu.data[3] <<24) +
					((UINT64)waterFrame->_asdu.data[4] <<32);

				pWaterQuality_PAPR->SetQualityValue(WaterQualityCheckValue);

				//���ĳ��Ȳ���ʱ
				if (waterFrame->_asdu.data.size() < pWaterQuality_PAPR->Get_Buffer_Count() + 2   )
				{
					delete pSave_data;
					pSave_data = NULL;
					return ;
				}

				int nStep = 5;
				for (BYTE n = 0 ; n < 40; n ++)
				{
					if (pWaterQuality_PAPR->GetBitValue(n) == true)
					{
						CWaterQuality_PARA_PER* pQuality_ONE = pWaterQuality_PAPR->GetOneQualityObject(n);

						UINT64 nBufValue = 0;
						DOUBLE dParaValue = 0.0f;

						if (n != 24)
						{
							nBufValue = (UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 0]) +
								(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 1]) * 100 +
								(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2]) * 10000 +
								(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 3]) * 1000000;
							nStep += 4;
						}
						else
						{

							nBufValue =  (UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 0]) +
								(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 1]) * 100 +
								(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2]) * 10000 +
								(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 3]) * 1000000 +
								(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 4]) * 100000000;
							nStep += 5;
						}

						if (pQuality_ONE)
						{
							dParaValue = (DOUBLE)nBufValue;
							if (pQuality_ONE->m_PointBits > 0)
							{
								for (int m = 0 ; m < pQuality_ONE->m_PointBits; m ++)
								{
									dParaValue /= 10;

								}
							}
							pQuality_ONE->m_CurValue = dParaValue;

							//������ݴ洢
							CWaterQuality_PARA_PER* pQuality_Save = new CWaterQuality_PARA_PER();
							pQuality_Save->m_strName = pQuality_ONE->m_strName;
							pQuality_Save->m_strUnit = pQuality_ONE->m_strUnit;
							pQuality_Save->m_BitPos = pQuality_ONE->m_BitPos;
							pQuality_Save->m_DataBits = pQuality_ONE->m_DataBits;
							pQuality_Save->m_PointBits = pQuality_ONE->m_PointBits;
							pQuality_Save->m_CurValue = pQuality_ONE->m_CurValue;

							pSave_data->m_lstWaterQuality.AddTail(pQuality_Save);
						}

					}
				}//end for (BYTE n = 0 ; n < 40; n ++)

				waterFrame->m_pStation->GetRtuAlarm_State()->SetAlarmStateByte(waterFrame->_asdu.data[nStep]);
				waterFrame->m_pStation->GetRtuTerminal_State()->SetTerminalStateByte(waterFrame->_asdu.data[nStep + 1]);
			}
		}

		AddDataMessage(waterFrame->m_pStation,pSave_data);
		InsertOneSaveData(pSave_data);	
	}  //end if (waterFrame->GetResendType() == false)

	IEC_waterdata_EN_80H(waterFrame);
}

//����Ա���������
void CSysDataTrans::IEC_waterdata_US_81H(CPacketFrameFormat* waterFrame)   
{
	if (!waterFrame) return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;
	if (waterFrame->_asdu.data.size() < 9) return;

	if (waterFrame->GetRepeatType() == false)
	{
		WORD AlertValue  = (waterFrame->_asdu.data[0] | (waterFrame->_asdu.data[1] << 8));
		std::bitset<16> alertbits = AlertValue;
		int nCount = waterFrame->_asdu.data.size();   //����֡���� �����������Tp

		//����TP
		BYTE tp[5];
		memset(tp,0,sizeof(tp));
		int _tpPos = (int)waterFrame->_asdu.data.size() - 5;
		for (int idx=0; idx <5; idx++)
			tp[idx] = waterFrame->_asdu.data[_tpPos +idx];

		_TP _autoTp ;
		CUdfGlobalFUNC::GetTPInfo(&_autoTp,tp);

		COneData_INFO* pSave_data = new COneData_INFO();
		BuildBasicDataInfo(pSave_data,waterFrame);
		pSave_data->mDataType = COneData_INFO::ALARM_RECORD;
		/*	pSave_data->mCollectTime.wDay = _autoTp.m_day;
		pSave_data->mCollectTime.wHour = _autoTp.m_hour;
		pSave_data->mCollectTime.wMinute = _autoTp.m_minute;
		pSave_data->mCollectTime.wSecond = _autoTp.m_second;*/
		CUdfGlobalFUNC::ConvertTpToSystemtime(&_autoTp,&(pSave_data->mCollectTime));

		pSave_data->SetErrorFlag(CUdfGlobalFUNC::CheckErrorData(&(pSave_data->mCollectTime)));

		//TP��ֵ
		pSave_data->mTP.m_adelay = _autoTp.m_adelay;
		pSave_data->mTP.m_day	 = _autoTp.m_day;
		pSave_data->mTP.m_hour	 = _autoTp.m_hour;
		pSave_data->mTP.m_minute = _autoTp.m_minute;
		pSave_data->mTP.m_second = _autoTp.m_second;


		CString strMessage = "",strTemp = "";
		for (size_t _pos = 0; _pos < 16; _pos ++)
		{
			if (alertbits.at(_pos) == true)
			{
				pSave_data->mAlarmType = (int)_pos;


				if (_pos == 2)  //ˮλ����
				{
					int nLevelCount = (nCount - 2 - 2 -5 )/ 3;
					for (int i = 0; i < nLevelCount; i ++)
					{
						float RealV = ((float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[2 + i * 3 + 0]) * 0.01 +
							(float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[2 + i * 3 + 1]) +
							(float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[2 + i * 3 + 2]) * 100);

						CWaterLevel_PAR_PER* pLevel = new CWaterLevel_PAR_PER();
						pLevel->SetRealValue(RealV);
						pSave_data->m_lstWaterLevel.AddTail(pLevel);
					}
				}
				else if(_pos == 3) //ˮѹ����
				{
					int nPressCount = (waterFrame->_asdu.data.size() - 2 -2- 5) / 4;   //ˮѹ�Ǳ���Ŀ

					for (int i = 0 ; i < nPressCount; i ++)
					{
						//ˮѹʵʱֵ
						double realV = (double) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[2 + i * 4]) * 0.01 +
							(double) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[2 + i * 4 + 1]) +
							(double) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[2 + i * 4 + 2]) * 100 +
							(double) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[2 + i * 4 + 3]) * 10000;

						CWaterPressure_PARA_PER* pPress = new CWaterPressure_PARA_PER();
						pPress->SetRealValue(realV);
						pSave_data->m_lstWaterPress.AddTail(pPress);
					}
				}
				else if (_pos == 4) //ˮ��Խ��
				{
					//���ĳ��Ȳ���ʱ
					if (waterFrame->_asdu.data.size() < 2 + 5 + 2 + 5 )
					{
						delete pSave_data;
						pSave_data = NULL;
						return;
					}

					if (waterFrame->m_pStation && waterFrame->m_pStation->GetWaterQualityPara())
					{
						CWaterQuality_PARA* pWaterQuality_PAPR = waterFrame->m_pStation->GetWaterQualityPara();

						if (waterFrame->_asdu.data.size() < 5) return;

						UINT64  WaterQualityCheckValue =(UINT64) (waterFrame->_asdu.data[2 + 0]) +
							((UINT64) waterFrame->_asdu.data[2 + 1] << 8) +
							((UINT64) waterFrame->_asdu.data[2 + 2] <<16) +
							((UINT64) waterFrame->_asdu.data[2 + 3] <<24) +
							((UINT64) waterFrame->_asdu.data[2 + 4] <<32);

						pWaterQuality_PAPR->SetQualityValue(WaterQualityCheckValue);

						//���ĳ��Ȳ���ʱ
						if (waterFrame->_asdu.data.size() < 2 + 5 + 2  + pWaterQuality_PAPR->Get_Buffer_Count())
						{
							delete pSave_data;
							pSave_data = NULL;
							return;
						}

						int nStep = 7;
						for (BYTE n = 0 ; n < 40; n ++)
						{
							if (pWaterQuality_PAPR->GetBitValue(n) == true)
							{
								CWaterQuality_PARA_PER* pQuality_ONE = pWaterQuality_PAPR->GetOneQualityObject(n);

								UINT64 nBufValue = 0;
								DOUBLE dParaValue = 0.0f;

								if (n != 24)
								{
									nBufValue = (UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 0]) +
										(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 1]) * 100 +
										(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2]) * 10000 +
										(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 3]) * 1000000;
									nStep += 4;
								}
								else
								{

									nBufValue =  (UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 0]) +
										(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 1]) * 100 +
										(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2]) * 10000 +
										(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 3]) * 1000000 +
										(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 4]) * 100000000;
									nStep += 5;
								}

								if (pQuality_ONE)
								{
									dParaValue = (DOUBLE)nBufValue;
									if (pQuality_ONE->m_PointBits > 0)
									{
										for (int m = 0 ; m < pQuality_ONE->m_PointBits; m ++)
										{
											dParaValue /= 10;

										}
									}
									pQuality_ONE->m_CurValue = dParaValue;


									//������ݴ洢
									CWaterQuality_PARA_PER* pQuality_Save = new CWaterQuality_PARA_PER();
									pQuality_Save->m_strName = pQuality_ONE->m_strName;
									pQuality_Save->m_strUnit = pQuality_ONE->m_strUnit;
									pQuality_Save->m_BitPos = pQuality_ONE->m_BitPos;
									pQuality_Save->m_DataBits = pQuality_ONE->m_DataBits;
									pQuality_Save->m_PointBits = pQuality_ONE->m_PointBits;
									pQuality_Save->m_CurValue = pQuality_ONE->m_CurValue;
									pSave_data->m_lstWaterQuality.AddTail(pQuality_Save);
								}

							}
							if( nStep >= nCount)
								break;
						}//end for (BYTE n = 0 ; n < 40; n ++)
					}
				}
				else if (_pos == 7) //ʣ��ˮ������
				{
					int nFlowCount = (waterFrame->_asdu.data.size() - 2 -2 -5 )/ 6;   //������������

					for (int i = 0 ; i < nFlowCount ; i ++)
					{
						//�ۼ�ȡˮ��
						DWORD CalTakeWater = (DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 0]) +
							(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 1]) * 100 +
							(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 2]) * 10000 +
							(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 3]) * 1000000;

						//˲ʱ����
						WORD InsFlow   = (WORD)(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 4])) +
							(WORD)(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 5])) * 100;

						//������ݴ洢
						CFlowInitValume_PARA_PER* pFlow = new CFlowInitValume_PARA_PER();
						pFlow->SetCalTakeValume( CalTakeWater);
						pFlow->SetInsFlowValume(InsFlow);
						pSave_data->m_lstFlow.AddTail(pFlow);

					}
				}   //end 


				break;
			}  //end if (alertbits.at(_pos) == true)
		}

		if (waterFrame->m_pStation != NULL)
		{
			waterFrame->m_pStation->GetRtuAlarm_State()->SetAlarmStateByte(waterFrame->_asdu.data[nCount - 7]);
			waterFrame->m_pStation->GetRtuTerminal_State()->SetTerminalStateByte(waterFrame->_asdu.data[nCount - 6]);
		}

		AddDataMessage(waterFrame->m_pStation,pSave_data);
		InsertOneSaveData(pSave_data);

	}  //end if (waterFrame->GetResendType() == false)

	//��Ӧ����Ա���������
	IEC_waterdata_EN_81H(waterFrame);
}


//�˹�ֵ��
void CSysDataTrans::IEC_waterdata_US_82H(CPacketFrameFormat* waterFrame)  
{
	if (!waterFrame) return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;
	if (waterFrame->_asdu._AFN != 0x82)	return;
	if (waterFrame->_asdu.data.size() < 2) return;

	if (waterFrame->GetRepeatType() == false)
	{
		COneData_INFO* pSave_data = new COneData_INFO();
		BuildBasicDataInfo(pSave_data,waterFrame);
		if (waterFrame->ControlC._Command == 5) //ˮλ�˹�����
		{
			pSave_data->mDataType = COneData_INFO::WATER_L_DATA;

			int nLevelCount = (waterFrame->_asdu.data.size()-2) / 3 ; //ˮλ�����

			for ( int i = 0 ; i < nLevelCount; i ++)
			{
				float RealV = ((float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 3 + 0]) * 0.01 +
					(float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 3 + 1]) +
					(float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 3 + 2]) * 100);

				//������ݴ洢
				CWaterLevel_PAR_PER* pLevel = new CWaterLevel_PAR_PER();
				pLevel->SetRealValue(RealV);
				pSave_data->m_lstWaterLevel.AddTail(pLevel);
			}

			if (waterFrame->m_pStation != NULL)
			{
				waterFrame->m_pStation->GetRtuAlarm_State()->SetAlarmStateByte(waterFrame->_asdu.data[nLevelCount * 3]);
				waterFrame->m_pStation->GetRtuTerminal_State()->SetTerminalStateByte(waterFrame->_asdu.data[nLevelCount* 3 + 1]);
			}
		}
		else if (waterFrame->ControlC._Command == 6) //���������˹�����
		{
			pSave_data->mDataType = COneData_INFO::FLOW_V_DATA;

			int nFlowCount = (waterFrame->_asdu.data.size() - 2)/ 6;   //������������

			for (int i = 0 ; i < nFlowCount ; i ++)
			{
				//�ۼ�ȡˮ��
				DWORD CalTakeWater = (DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 0]) +
					(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 1]) * 100 +
					(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 2]) * 10000 +
					(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 3]) * 1000000;

				//˲ʱ����
				WORD InsFlow   =   (WORD)(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 4])) +
					(WORD)(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 5])) * 100;


				//������ݴ洢
				CFlowInitValume_PARA_PER* pFlow = new CFlowInitValume_PARA_PER();
				pFlow->SetCalTakeValume( CalTakeWater);
				pFlow->SetInsFlowValume(InsFlow);
				pSave_data->m_lstFlow.AddTail(pFlow);
			}

			if (waterFrame->m_pStation)
			{
				waterFrame->m_pStation->GetRtuAlarm_State()->SetAlarmStateByte(waterFrame->_asdu.data[nFlowCount * 6]);
				waterFrame->m_pStation->GetRtuTerminal_State()->SetTerminalStateByte(waterFrame->_asdu.data[nFlowCount * 6 + 1]);
			}

		}
		else if (waterFrame->ControlC._Command == 7) //ˮѹ�����˹�����
		{
			pSave_data->mDataType = COneData_INFO::WATER_P_DATA;

			int nPressCount = (waterFrame->_asdu.data.size() - 2) / 4;   //ˮѹ�Ǳ���Ŀ

			for (int i = 0 ; i < nPressCount; i ++)
			{
				//ˮѹʵʱֵ
				double realV = (double) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 4]) * 0.01 +
					(double) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 4 + 1]) +
					(double) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 4 + 2]) * 100 +
					(double) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 4 + 3]) * 10000;
				//������ݴ洢
				CWaterPressure_PARA_PER* pPress = new CWaterPressure_PARA_PER();
				pPress->SetRealValue(realV);
				pSave_data->m_lstWaterPress.AddTail(pPress);
			}

			if( waterFrame->m_pStation != NULL)
			{
				waterFrame->m_pStation->GetRtuAlarm_State()->SetAlarmStateByte(waterFrame->_asdu.data[nPressCount * 4]);
				waterFrame->m_pStation->GetRtuTerminal_State()->SetTerminalStateByte(waterFrame->_asdu.data[nPressCount * 4 + 1]);
			}
		}
		else if (waterFrame->ControlC._Command == 8) //ˮ�ʲ����˹�����
		{
			pSave_data->mDataType = COneData_INFO::WATER_Q_DATA;

			if (waterFrame->m_pStation && waterFrame->m_pStation->GetWaterQualityPara())
			{
				CWaterQuality_PARA* pWaterQuality_PAPR = waterFrame->m_pStation->GetWaterQualityPara();

				//���ĳ��Ȳ���ʱ
				if (waterFrame->_asdu.data.size() < 5 +2) 
				{
					delete pSave_data;
					pSave_data = NULL;
					return;
				}

				UINT64  WaterQualityCheckValue =(UINT64) (waterFrame->_asdu.data[0]) +
					((UINT64)waterFrame->_asdu.data[1] << 8) +
					((UINT64)waterFrame->_asdu.data[2] <<16) +
					((UINT64)waterFrame->_asdu.data[3] <<24) +
					((UINT64)waterFrame->_asdu.data[4] <<32);

				pWaterQuality_PAPR->SetQualityValue(WaterQualityCheckValue);

				//���ĳ��Ȳ���ʱ
				if (waterFrame->_asdu.data.size() < 2 + pWaterQuality_PAPR->Get_Buffer_Count())
				{
					delete pSave_data;
					pSave_data = NULL;
					return;
				}

				int nStep = 5;
				for (BYTE n = 0 ; n < 40; n ++)
				{
					if (pWaterQuality_PAPR->GetBitValue(n) == true)
					{
						CWaterQuality_PARA_PER* pQuality_ONE = pWaterQuality_PAPR->GetOneQualityObject(n);

						UINT64 nBufValue = 0;
						DOUBLE dParaValue = 0.0f;

						if (n != 24)
						{
							nBufValue = (UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 0]) +
								(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 1]) * 100 +
								(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2]) * 10000 +
								(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 3]) * 1000000;
							nStep += 4;
						}
						else
						{
							nBufValue =  (UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 0]) +
								(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 1]) * 100 +
								(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2]) * 10000 +
								(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 3]) * 1000000 +
								(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 4]) * 100000000;
							nStep += 5;
						}

						if (pQuality_ONE)
						{
							dParaValue = (DOUBLE)nBufValue;
							if (pQuality_ONE->m_PointBits > 0)
							{
								for (int m = 0 ; m < pQuality_ONE->m_PointBits; m ++)
									dParaValue /= 10;
							}
							//pQuality_ONE->m_CurValue = dParaValue;

							//������ݴ洢
							CWaterQuality_PARA_PER* pQuality_Save = new CWaterQuality_PARA_PER();
							pQuality_Save->m_strName = pQuality_ONE->m_strName;
							pQuality_Save->m_strUnit = pQuality_ONE->m_strUnit;
							pQuality_Save->m_BitPos = pQuality_ONE->m_BitPos;
							pQuality_Save->m_DataBits = pQuality_ONE->m_DataBits;
							pQuality_Save->m_PointBits = pQuality_ONE->m_PointBits;
							pQuality_Save->m_CurValue = dParaValue; //pQuality_ONE->m_CurValue;

							pSave_data->m_lstWaterQuality.AddTail(pQuality_Save);
						}  //end if (pQuality_ONE)

					}//end if (pWaterQuality_PAPR->GetBitValue(n) == true)
				}//end for (BYTE n = 0 ; n < 40; n ++)

				waterFrame->m_pStation->GetRtuAlarm_State()->SetAlarmStateByte(waterFrame->_asdu.data[nStep]);
				waterFrame->m_pStation->GetRtuTerminal_State()->SetTerminalStateByte(waterFrame->_asdu.data[nStep + 1]);
			}  //end if (waterFrame->m_pStation && waterFrame->m_pStation->GetWaterQualityPara())
		}
		else if (waterFrame->ControlC._Command == 11) //���������˹�����
		{
			pSave_data->mDataType = COneData_INFO::RAIN_DATA;

			int nCount = (waterFrame->_asdu.data.size() - 2) / 3;   //�����Ǳ���Ŀ

			for (int i= 0; i< nCount; i ++)
			{
				BYTE type   = waterFrame->_asdu.data[i*3 + 0];
				double fvalue = ((double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i*3 + 1])) * 0.1 +
					((double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i*3 + 2])) * 10;
				//������ݴ洢��Ϣ
				CRainAlert_PARA_PER* pRain = new CRainAlert_PARA_PER();
				pRain->SetDataType( type);
				pRain->SetRealTimeValue(fvalue);
				pSave_data->m_lstRain.AddTail(pRain);
			}

			if (waterFrame->m_pStation != NULL)
			{
				waterFrame->m_pStation->GetRtuExAlarm_State()->SetExAlarmStateByte(waterFrame->_asdu.data[nCount * 3 ]);
				waterFrame->m_pStation->GetRtuTerminal_State()->SetTerminalStateByte(waterFrame->_asdu.data[nCount*3 +1]);
			}

		}
		else if (waterFrame->ControlC._Command == 12) //ģ���������˹�����
		{
			pSave_data->mDataType = COneData_INFO::ANALOG_DATA;

			int nCount = (waterFrame->_asdu.data.size()-2) /3;  //ģ��������
			for (int i = 0 ;i < nCount; i ++)
			{
				DWORD AnalogV =  (DWORD) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 3]) +
					(DWORD) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 3 + 1]) * 100 +
					(DWORD) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 3 + 2] & 0x0f) * 10000;

				//������ݴ洢��Ϣ
				CAnalogLimit_PARA_PER* pAnalog = new CAnalogLimit_PARA_PER();
				pAnalog->SetRealTimeValue(AnalogV);
				pSave_data->m_lstAnalog.AddTail(pAnalog);
			}

			if (waterFrame->m_pStation != NULL)
			{
				waterFrame->m_pStation->GetRtuExAlarm_State()->SetExAlarmStateByte(waterFrame->_asdu.data[nCount * 3]);
				waterFrame->m_pStation->GetRtuTerminal_State()->SetTerminalStateByte(waterFrame->_asdu.data[nCount * 3 + 1]);
			}
		}
		AddDataMessage(waterFrame->m_pStation,pSave_data,MANU_DATA);
		//InsertOneSaveData(pSave_data);

	}  //end  if (waterFrame->GetResendType() == false)

	IEC_waterdata_EN_82H(waterFrame);
}

//��ֵ�Ա���չʵʱ����
void CSysDataTrans::IEC_waterdata_US_83H(CPacketFrameFormat* waterFrame)
{
	if (!waterFrame)	return;
	if (waterFrame->ControlC._DIR != UP_ARROW)	return;
	if (waterFrame->_asdu._AFN != 0x83)	 return;
	if (waterFrame->_asdu.data.size()<7 ) return;

	if (waterFrame->GetRepeatType() == false)
	{
		COneData_INFO* pSave_data = new COneData_INFO();
		BuildBasicDataInfo(pSave_data,waterFrame);

		if (waterFrame->ControlC._Command == C_CMD_11_RAINPARA)  //�����Ա�����
		{
			pSave_data->mDataType = COneData_INFO::RAIN_DATA;

			BYTE tp[5];
			memset(tp,0,sizeof(tp));
			int _tpPos = (int)waterFrame->_asdu.data.size() - 5;
			for (int idx=0; idx <5; idx++)
			{
				tp[idx] = waterFrame->_asdu.data[_tpPos +idx];
			}

			_TP _autoTp ;
			CUdfGlobalFUNC::GetTPInfo(&_autoTp,tp);

			/*		pSave_data->mCollectTime.wDay = _autoTp.m_day;
			pSave_data->mCollectTime.wHour = _autoTp.m_hour;
			pSave_data->mCollectTime.wMinute = _autoTp.m_minute;
			pSave_data->mCollectTime.wSecond = _autoTp.m_second;*/
			CUdfGlobalFUNC::ConvertTpToSystemtime(&_autoTp,&(pSave_data->mCollectTime));


			pSave_data->SetErrorFlag(CUdfGlobalFUNC::CheckErrorData(&(pSave_data->mCollectTime)));


			//TP��ֵ
			pSave_data->mTP.m_adelay = _autoTp.m_adelay;
			pSave_data->mTP.m_day	 = _autoTp.m_day;
			pSave_data->mTP.m_hour	 = _autoTp.m_hour;
			pSave_data->mTP.m_minute = _autoTp.m_minute;
			pSave_data->mTP.m_second = _autoTp.m_second;

			int nCount = (_tpPos -2) /3;
			for (int i= 0; i< nCount; i ++)
			{
				BYTE type   = waterFrame->_asdu.data[i*3 + 0];
				double fvalue =  ((double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i*3 + 1])) * 0.1 +
					((double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i*3 + 2])) * 10;


				//������ݴ洢��Ϣ
				CRainAlert_PARA_PER* pRain = new CRainAlert_PARA_PER();
				pRain->SetDataType( type);
				pRain->SetRealTimeValue(fvalue);
				pSave_data->m_lstRain.AddTail(pRain);
			}
			if (nCount * 3 + 2 + 5 == waterFrame->_asdu.data.size())
			{ 
				if (waterFrame->m_pStation != NULL)
				{
					waterFrame->m_pStation->GetRtuExAlarm_State()->SetExAlarmStateByte(waterFrame->_asdu.data[nCount * 3 ]);
					waterFrame->m_pStation->GetRtuTerminal_State()->SetTerminalStateByte(waterFrame->_asdu.data[nCount*3 +1]);
				}
			}

		}
		else if (waterFrame->ControlC._Command == C_CMD_12_MONIPARA)  //ģ���������Ա�
		{
			pSave_data->mDataType = COneData_INFO::ANALOG_DATA;


			BYTE tp[5];
			memset(tp,0,sizeof(tp));
			int _tpPos = (int)waterFrame->_asdu.data.size() - 5;
			for (int idx=0; idx <5; idx++)
			{
				tp[idx] = waterFrame->_asdu.data[_tpPos +idx];
			}

			_TP _autoTp ;
			CUdfGlobalFUNC::GetTPInfo(&_autoTp,tp);

			//pSave_data->mCollectTime.wDay = _autoTp.m_day;
			//pSave_data->mCollectTime.wHour = _autoTp.m_hour;
			//pSave_data->mCollectTime.wMinute = _autoTp.m_minute;
			//pSave_data->mCollectTime.wSecond = _autoTp.m_second;
			CUdfGlobalFUNC::ConvertTpToSystemtime(&_autoTp,&(pSave_data->mCollectTime));

			//TP��ֵ
			pSave_data->mTP.m_adelay = _autoTp.m_adelay;
			pSave_data->mTP.m_day	 = _autoTp.m_day;
			pSave_data->mTP.m_hour	 = _autoTp.m_hour;
			pSave_data->mTP.m_minute = _autoTp.m_minute;
			pSave_data->mTP.m_second = _autoTp.m_second;


			int nAnalogCount = (waterFrame->_asdu.data.size()- 7)/ 3;
			for (int i = 0 ;i < nAnalogCount; i ++)
			{
				DWORD AnalogV =  (DWORD) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 3]) +
					(DWORD) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 3 + 1]) * 100 +
					(DWORD) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 3 + 2] & 0x0f) * 10000;

				//������ݴ洢��Ϣ
				CAnalogLimit_PARA_PER* pAnalog = new CAnalogLimit_PARA_PER();
				pAnalog->SetRealTimeValue(AnalogV);
				pSave_data->m_lstAnalog.AddTail(pAnalog);
			}

			if (waterFrame->m_pStation != NULL)
			{
				waterFrame->m_pStation->GetRtuExAlarm_State()->SetExAlarmStateByte(waterFrame->_asdu.data[nAnalogCount * 3]);
				waterFrame->m_pStation->GetRtuTerminal_State()->SetTerminalStateByte(waterFrame->_asdu.data[nAnalogCount * 3 + 1]);
			}
		}
		else if (waterFrame->ControlC._Command == C_CMD_13_PULSEPARA) //����������Ա�
		{
			pSave_data->mDataType = COneData_INFO::PULSE_DATA;

			BYTE tp[5];
			memset(tp,0,sizeof(tp));
			int _tpPos = (int)waterFrame->_asdu.data.size() - 5;
			for (int idx=0; idx <5; idx++)
			{
				tp[idx] = waterFrame->_asdu.data[_tpPos +idx];
			}

			_TP _autoTp ;
			CUdfGlobalFUNC::GetTPInfo(&_autoTp,tp);

			//pSave_data->mCollectTime.wDay = _autoTp.m_day;
			//pSave_data->mCollectTime.wHour = _autoTp.m_hour;
			//pSave_data->mCollectTime.wMinute = _autoTp.m_minute;
			//pSave_data->mCollectTime.wSecond = _autoTp.m_second;
			CUdfGlobalFUNC::ConvertTpToSystemtime(&_autoTp,&(pSave_data->mCollectTime));

			//TP��ֵ
			pSave_data->mTP.m_adelay = _autoTp.m_adelay;
			pSave_data->mTP.m_day	 = _autoTp.m_day;
			pSave_data->mTP.m_hour	 = _autoTp.m_hour;
			pSave_data->mTP.m_minute = _autoTp.m_minute;
			pSave_data->mTP.m_second = _autoTp.m_second;

			int nCount = (_tpPos - 2) / 22;
			for (int i = 0; i < nCount; i ++)
			{
				UINT64 code = 0;
				code =  (UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 0] ) +
					(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 1] ) * (UINT64) 100 +
					(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 2] ) * (UINT64) 10000 +
					(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 3] ) * (UINT64) 1000000 +
					(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 4] ) * (UINT64) 100000000 +
					(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 5] ) * (UINT64) 10000000000 ;

				CString strCode = "",strTemp = "";
				for (int cIdx = 5; cIdx >= 0; cIdx --)
				{
					if (waterFrame->_asdu.data[i * 22 + cIdx] != 0)
					{
						strTemp.Format("%X",waterFrame->_asdu.data[i * 22 + cIdx]);
						strCode += strTemp;
						for(int _Idx= cIdx-1;_Idx >= 0; _Idx --)
						{
							strTemp.Format("%02X",waterFrame->_asdu.data[i * 22 + _Idx]);
							strCode += strTemp;
						}

						break;
					}				
				}
				if (strCode.IsEmpty())
					strCode = "0";

				CPulsePara_PER *pPulse = waterFrame->m_pStation->GetPulseParaObject(code);

				if (pPulse == NULL)
				{
					pPulse = new CPulsePara_PER(code,0.0,0.0,0.0,0.0,strCode);
					waterFrame->m_pStation->InsertPulseParaObject(pPulse);					
				}


				double dbvalue = 0.0;
				dbvalue =	(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 6] ) * (double) 0.01 +
					(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 7] ) * (double) 1 +
					(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 8] ) * (double) 100 +
					(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 9] ) * (double) 10000;
				pPulse->SetValue1(dbvalue);


				dbvalue =	(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 10] ) * (double) 0.01 +
					(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 11] ) * (double) 1 +
					(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 12] ) * (double) 100 +
					(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 13] ) * (double) 10000;
				pPulse->SetValue2(dbvalue);


				dbvalue =	(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 14] ) * (double) 0.01 +
					(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 15] ) * (double) 1 +
					(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 16] ) * (double) 100 +
					(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 17] ) * (double) 10000;
				pPulse->SetValue3(dbvalue);


				dbvalue =	(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 18] ) * (double) 0.01 +
					(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 19] ) * (double) 1 +
					(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 20] ) * (double) 100 +
					(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 21] ) * (double) 10000;
				pPulse->SetValue4(dbvalue);

				CPulsePara_PER* pSavePulse = new CPulsePara_PER(code,pPulse->GetValue1(),pPulse->GetValue2(),pPulse->GetValue3(),pPulse->GetValue4(),pPulse->GetCodeString());
				pSave_data->m_lstPulses.AddTail(pSavePulse);
			}

			waterFrame->m_pStation->GetRtuExAlarm_State()->SetExAlarmStateByte(waterFrame->_asdu.data[nCount * 22 ]);
			waterFrame->m_pStation->GetRtuTerminal_State()->SetTerminalStateByte(waterFrame->_asdu.data[nCount * 22 + 1]);
		}
		else if (waterFrame->ControlC._Command == C_CMD_14_DISPLACEMENT)  //ģ���������Ա�
		{
			pSave_data->mDataType = COneData_INFO::DISPLACE_DATA;


			BYTE tp[5];
			memset(tp,0,sizeof(tp));
			int _tpPos = (int)waterFrame->_asdu.data.size() - 5;
			for (int idx=0; idx <5; idx++)
			{
				tp[idx] = waterFrame->_asdu.data[_tpPos +idx];
			}

			_TP _autoTp ;
			CUdfGlobalFUNC::GetTPInfo(&_autoTp,tp);
			CUdfGlobalFUNC::ConvertTpToSystemtime(&_autoTp,&(pSave_data->mCollectTime));

			//TP��ֵ
			pSave_data->mTP.m_adelay = _autoTp.m_adelay;
			pSave_data->mTP.m_day	 = _autoTp.m_day;
			pSave_data->mTP.m_hour	 = _autoTp.m_hour;
			pSave_data->mTP.m_minute = _autoTp.m_minute;
			pSave_data->mTP.m_second = _autoTp.m_second;


			int nDisplaceCount = (waterFrame->_asdu.data.size()- 7)/ 6;
			for (int i = 0 ;i < nDisplaceCount; i ++)
			{
				DWORD DisplaceXV =  (DWORD) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6]) +
					(DWORD) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 1]) * 100 +
					(DWORD) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 2]) * 10000;
					
				DWORD DisplaceYV =  (DWORD) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6+ 3]) +
					(DWORD) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 4]) * 100 +
					(DWORD) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 25]) * 10000;

				//������ݴ洢��Ϣ
				CDisplace_PARA_PER* pDisplace = new CDisplace_PARA_PER();
				pDisplace->SetRealTimeXValue(DisplaceXV);
				pDisplace->SetRealTimeYValue(DisplaceYV);
				pDisplace->SetDisplaceNo(i+1);
				pSave_data->m_lstDisplace.AddTail(pDisplace);
			}

			if (waterFrame->m_pStation != NULL)
			{
				waterFrame->m_pStation->GetRtuExAlarm_State()->SetExAlarmStateByte(waterFrame->_asdu.data[nDisplaceCount * 6]);
				waterFrame->m_pStation->GetRtuTerminal_State()->SetTerminalStateByte(waterFrame->_asdu.data[nDisplaceCount * 6 + 1]);
			}
		}
		
		else if (waterFrame->ControlC._Command == C_CMD_15_TEMPERATURE)  //�¶��������Ա�
		{
			pSave_data->mDataType = COneData_INFO::TEMPERA_DATA;
			BYTE tp[5];
			memset(tp,0,sizeof(tp));
			int _tpPos = (int)waterFrame->_asdu.data.size() - 5;
			for (int idx=0; idx <5; idx++)
			{
				tp[idx] = waterFrame->_asdu.data[_tpPos +idx];
			}

			_TP _autoTp ;
			CUdfGlobalFUNC::GetTPInfo(&_autoTp,tp);
			CUdfGlobalFUNC::ConvertTpToSystemtime(&_autoTp,&(pSave_data->mCollectTime));

			//TP��ֵ
			pSave_data->mTP.m_adelay = _autoTp.m_adelay;
			pSave_data->mTP.m_day	 = _autoTp.m_day;
			pSave_data->mTP.m_hour	 = _autoTp.m_hour;
			pSave_data->mTP.m_minute = _autoTp.m_minute;
			pSave_data->mTP.m_second = _autoTp.m_second;


			int nTemperaCount = (waterFrame->_asdu.data.size()- 7)/ 2;
			for (int i = 0 ;i < nTemperaCount; i ++)
			{
				DWORD RealValue =  (DWORD) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 2]) +
					(DWORD) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 2 + 1]) * 100;

				//������ݴ洢��Ϣ
				CTempera_PARA_PER* pTempera = new CTempera_PARA_PER();
				pTempera->SetRealTimeValue(RealValue/10.0);
				pSave_data->m_lstTempera.AddTail(pTempera);
			}

			if (waterFrame->m_pStation != NULL)
			{
				waterFrame->m_pStation->GetRtuExAlarm_State()->SetExAlarmStateByte(waterFrame->_asdu.data[nTemperaCount * 6]);
				waterFrame->m_pStation->GetRtuTerminal_State()->SetTerminalStateByte(waterFrame->_asdu.data[nTemperaCount * 6 + 1]);
			}
		}

		AddDataMessage(waterFrame->m_pStation,pSave_data);
		InsertOneSaveData(pSave_data);

	} //end if (waterFrame->GetResendType() == false)

	IEC_waterdata_EN_83H(waterFrame);
}

//����Ա���չ��������
void CSysDataTrans::IEC_waterdata_US_84H(CPacketFrameFormat* waterFrame)
{
	if (!waterFrame)	return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;
	if (waterFrame->_asdu._AFN != 0x84)	return;
	if (waterFrame->_asdu.data.size() < 9) return;

	if (waterFrame->GetRepeatType() == false)
	{
		WORD warningType = waterFrame->_asdu.data[0]|(waterFrame->_asdu.data[1]<<8);
		std::bitset<16> bitWarn = warningType;

		COneData_INFO* pSave_data = new COneData_INFO();
		BuildBasicDataInfo(pSave_data,waterFrame);
		pSave_data->mDataType = COneData_INFO::ALARM_RECORD;

		BYTE tp[5];
		memset(tp,0,sizeof(tp));
		int _tpPos = (int)waterFrame->_asdu.data.size() - 5;
		for (int idx=0; idx <5; idx++)
		{
			tp[idx] = waterFrame->_asdu.data[_tpPos +idx];
		}

		_TP _autoTp ;
		CUdfGlobalFUNC::GetTPInfo(&_autoTp,tp);

		//pSave_data->mCollectTime.wDay = _autoTp.m_day;
		//pSave_data->mCollectTime.wHour = _autoTp.m_hour;
		//pSave_data->mCollectTime.wMinute = _autoTp.m_minute;
		//pSave_data->mCollectTime.wSecond = _autoTp.m_second;
		CUdfGlobalFUNC::ConvertTpToSystemtime(&_autoTp,&(pSave_data->mCollectTime));

		pSave_data->SetErrorFlag(CUdfGlobalFUNC::CheckErrorData(&(pSave_data->mCollectTime)));


		//TP��ֵ
		pSave_data->mTP.m_adelay = _autoTp.m_adelay;
		pSave_data->mTP.m_day	 = _autoTp.m_day;
		pSave_data->mTP.m_hour	 = _autoTp.m_hour;
		pSave_data->mTP.m_minute = _autoTp.m_minute;
		pSave_data->mTP.m_second = _autoTp.m_second;

		if (bitWarn.at(0) == true)     //������Խ�ޱ���
		{
			pSave_data->mAlarmType = 16;

			int nRainCount = ((int)waterFrame->_asdu.data.size() - 4 - 5) / 4;
			for (int i = 0 ; i < nRainCount; i ++)
			{
				float RainV = (float) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[2 + i * 4]) *0.1 + 
					(float) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[2 + i * 4 + 1]) * 10;

				WORD PerTime=(WORD) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[2 + i * 4 + 2]) + 
					(WORD) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[2 + i * 4 + 3]) * 100;

				CRainAlert_PARA_PER* pRain = new CRainAlert_PARA_PER();
				pRain->SetRealTimeValue(RainV);
				pRain->SetPerTime(PerTime);
				pSave_data->m_lstRain.AddTail(pRain);
			}

			if (waterFrame->m_pStation != NULL)
			{
				waterFrame->m_pStation->GetRtuExAlarm_State()->SetExAlarmStateByte(waterFrame->_asdu.data[2 + nRainCount * 4]);
				waterFrame->m_pStation->GetRtuTerminal_State()->SetTerminalStateByte(waterFrame->_asdu.data[2 + nRainCount * 4 + 1]);
			}
		}
		else if (bitWarn.at(1) == true)		//ģ������Խ�ޱ���
		{

			pSave_data->mAlarmType = 17;
			int nAnalogCount = (waterFrame->_asdu.data.size() - 4 - 5) / 3;

			for (int i = 0 ; i < nAnalogCount; i ++)
			{
				DWORD AnalogV = (DWORD) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[2 + i * 3]) +
					(DWORD) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[2 + i * 3 + 1]) * 100 +
					(DWORD) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[2 + i * 3 + 2] & 0x0f) * 10000;

				CAnalogLimit_PARA_PER* pAnalog = new CAnalogLimit_PARA_PER();
				pAnalog->SetRealTimeValue(AnalogV);
				pSave_data->m_lstAnalog.AddTail(pAnalog);

			}

			if (waterFrame->m_pStation)
			{
				waterFrame->m_pStation->GetRtuExAlarm_State()->SetExAlarmStateByte(waterFrame->_asdu.data[2 + nAnalogCount * 3]);
				waterFrame->m_pStation->GetRtuTerminal_State()->SetTerminalStateByte(waterFrame->_asdu.data[2 + nAnalogCount * 3 +1]);
			}
		}

		AddDataMessage(waterFrame->m_pStation,pSave_data);
		InsertOneSaveData(pSave_data);

	} //end if (waterFrame->GetResendType() == false)

	IEC_waterdata_EN_84H(waterFrame);
}

//��RTU�·�����
//��·���
void CSysDataTrans::IEC_waterdata_EN_02H(CPacketFrameFormat* appFrame)  
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));
	if (!appFrame)   return;


	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._DIR = DOWN_ARROW;
	appFrame->ControlC._Command = C_CMD_03_LINKTEST;

	appFrame->_asdu._AFN = 0x02;

	BYTE Len = 0;
	BuildSendBuffer(appFrame,sBuf,Len);

	SendRtuLinkFrame(sBuf,Len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	AddReSendObject (sBuf,Len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}


void CSysDataTrans::IEC_waterdata_EN_03H(CPacketFrameFormat* appFrame)
{
	if (!appFrame)   return;
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	appFrame->ControlC._DIR = DOWN_ARROW;
	appFrame->_asdu._AFN = 0x03;

	BYTE Len = 0;
	BuildSendBuffer(appFrame,sBuf,Len);
	SendRtuLinkFrame(sBuf,Len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));

}


//����ң���ն˻��м�վ��ַ
void CSysDataTrans::IEC_waterdata_EN_10H(CPacketFrameFormat* appFrame)   
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	if (!appFrame) return;
}

//����ң���նˡ��м�վʱ��
void CSysDataTrans::IEC_waterdata_EN_11H(CPacketFrameFormat* appFrame)
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	if (!appFrame) return;

	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._DIR = DOWN_ARROW;
	appFrame->ControlC._Command = C_CMD_02_USERDATA;

	appFrame->_asdu._AFN = 0x11;

	appFrame->_asdu.data.clear();
	appFrame->_asdu.data = std::vector<BYTE>(13);

	SYSTEMTIME st;
	GetLocalTime(&st);

	//Added 2012-11-28 ��Ӷ���ʱ��
	if (appFrame->m_pStation != NULL && appFrame->m_pStation->GetDzDelay() != 0)
	{
		CTime _cur(st); 
		int delay = appFrame->m_pStation->GetDzDelay();
		CTimeSpan _tspan(delay);

		CTime _exTime = _cur + _tspan;

		CSystemTimeOp::ConvertCTimeToSystemtime(&_exTime,&st);

	}

	appFrame->_asdu.data[0]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wSecond);
	appFrame->_asdu.data[1]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wMinute);
	appFrame->_asdu.data[2]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wHour);
	appFrame->_asdu.data[3]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wDay);
	appFrame->_asdu.data[4]	 = ((st.wDayOfWeek << 5)|CUdfGlobalFUNC::ByteToBCD((BYTE)st.wMonth));
	appFrame->_asdu.data[5]	 = CUdfGlobalFUNC::ByteToBCD(st.wYear - 2000);

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	appFrame->_asdu.data[6]	 = pApp->m_GlobalOptions.m_PW.PW_BUF[0];
	appFrame->_asdu.data[7]	 = pApp->m_GlobalOptions.m_PW.PW_BUF[1];

	appFrame->_asdu.data[8]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wSecond);
	appFrame->_asdu.data[9]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wMinute);
	appFrame->_asdu.data[10]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wHour);
	appFrame->_asdu.data[11]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wDay);
	appFrame->_asdu.data[12]	 = pApp->m_GlobalOptions.m_TpDelay;


	BYTE len = 0;
	BuildSendBuffer(appFrame,sBuf,len);

	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//����ң���ն˹���ģʽ
void CSysDataTrans::IEC_waterdata_EN_12H(CPacketFrameFormat* appFrame)
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();

	if (!appFrame)  return;
	if (!appFrame->m_pStation) return;

	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._DIR = DOWN_ARROW;
	appFrame->ControlC._Command = C_CMD_02_USERDATA;

	appFrame->_asdu._AFN = 0x12;

	appFrame->_asdu.data.clear();
	appFrame->_asdu.data = std::vector<BYTE>(8);

	appFrame->_asdu.data[0] = appFrame->m_pStation->GetWorkMode();

	appFrame->_asdu.data[1] = pApp->m_GlobalOptions.m_PW.PW_BUF[0];
	appFrame->_asdu.data[2] = pApp->m_GlobalOptions.m_PW.PW_BUF[1];

	SYSTEMTIME st;
	GetLocalTime(&st);
	appFrame->_asdu.data[3]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wSecond);
	appFrame->_asdu.data[4]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wMinute);
	appFrame->_asdu.data[5]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wHour);
	appFrame->_asdu.data[6]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wDay);
	appFrame->_asdu.data[7] = pApp->m_GlobalOptions.m_TpDelay;

	BYTE len  = 0;
	BuildSendBuffer(appFrame,sBuf,len);
	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));

}

//���ң���ն˵������Ա����༰ʱ����
void CSysDataTrans::IEC_waterdata_EN_13H(CPacketFrameFormat* appFrame)
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));
	BYTE len = 0;
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();

	if (!appFrame)  return;
	if (!appFrame->m_pStation) return;

	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._DIR = DOWN_ARROW;
	appFrame->ControlC._Command = C_CMD_02_USERDATA;
	appFrame->_asdu._AFN = 0x13;

	//��RTU���ȡ�Ա�����������Ϣ
	appFrame->_asdu.data.clear();
	appFrame->_asdu.data = std::vector<BYTE>(11);

	WORD wAutoPara = appFrame->m_pStation->GetAutoReportPara()->GetAutoValue();
	WORD wAutoInterval = appFrame->m_pStation->GetAutoReportPara()->GetAutoInterval();

	appFrame->_asdu.data[0] = LOBYTE(wAutoPara);
	appFrame->_asdu.data[1] = HIBYTE(wAutoPara);
	appFrame->_asdu.data[2] = CUdfGlobalFUNC::ByteToBCD(wAutoInterval % 100);
	appFrame->_asdu.data[3] = CUdfGlobalFUNC::ByteToBCD(wAutoInterval / 100);

	appFrame->_asdu.data[4] = pApp->m_GlobalOptions.m_PW.PW_BUF[0];
	appFrame->_asdu.data[5] = pApp->m_GlobalOptions.m_PW.PW_BUF[1];

	SYSTEMTIME st;
	GetLocalTime(&st);
	appFrame->_asdu.data[6]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wSecond);
	appFrame->_asdu.data[7]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wMinute);
	appFrame->_asdu.data[8]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wHour);
	appFrame->_asdu.data[9]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wDay);
	appFrame->_asdu.data[10] = pApp->m_GlobalOptions.m_TpDelay;


	BuildSendBuffer(appFrame,sBuf,len);
	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//����ң���ն����ѯ��ʵʱ��������
void CSysDataTrans::IEC_waterdata_EN_14H(CPacketFrameFormat* appFrame)
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	if (!appFrame)   return;
	if (!appFrame->m_pStation) return;

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._DIR = DOWN_ARROW;
	appFrame->ControlC._Command = C_CMD_02_USERDATA;

	appFrame->_asdu._AFN = 0x14;

	appFrame->_asdu.data.clear();
	appFrame->_asdu.data = std::vector<BYTE>(9);

	WORD wQueryPara = appFrame->m_pStation->GetYcQueryType()->GetQueryPara();
	appFrame->_asdu.data[0] = LOBYTE(wQueryPara);
	appFrame->_asdu.data[1] = HIBYTE(wQueryPara);

	appFrame->_asdu.data[2] = pApp->m_GlobalOptions.m_PW.PW_BUF[0];
	appFrame->_asdu.data[3] = pApp->m_GlobalOptions.m_PW.PW_BUF[1];

	SYSTEMTIME st;
	GetLocalTime(&st);
	appFrame->_asdu.data[4]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wSecond);
	appFrame->_asdu.data[5]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wMinute);
	appFrame->_asdu.data[6]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wHour);
	appFrame->_asdu.data[7]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wDay);
	appFrame->_asdu.data[8]  = pApp->m_GlobalOptions.m_TpDelay;

	BYTE len = 0;
	BuildSendBuffer(appFrame,sBuf,len);
	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//����ң���ն˱��γ�ֵ��
void CSysDataTrans::IEC_waterdata_EN_15H(CPacketFrameFormat* appFrame)
{
	if (!appFrame)  return;
	if (!appFrame->m_pStation) return;
	if (appFrame->m_pStation->GetWaterValumePara() == NULL)  return;

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._DIR = DOWN_ARROW;
	appFrame->ControlC._DIV = 0;
	appFrame->ControlC._Command = C_CMD_02_USERDATA;
	appFrame->_asdu._AFN = 0x15;

	appFrame->_asdu.data.clear();
	appFrame->_asdu.data = std::vector<BYTE>(4 + 2 + 5);

	BYTE temp[4];
	memset(temp,0,sizeof(temp));
	DWORD dwDec  = appFrame->m_pStation->GetWaterValumePara()->GetRechargeValume();

	CUdfGlobalFUNC::ToBCD(dwDec,temp,4);
	appFrame->_asdu.data[0] = temp[0];
	appFrame->_asdu.data[1] = temp[1];
	appFrame->_asdu.data[2] = temp[2];
	appFrame->_asdu.data[3] = temp[3];


	appFrame->_asdu.data[4] = pApp->m_GlobalOptions.m_PW.PW_BUF[0];
	appFrame->_asdu.data[5] = pApp->m_GlobalOptions.m_PW.PW_BUF[1];

	SYSTEMTIME st;
	GetLocalTime(&st);
	appFrame->_asdu.data[6]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wSecond);
	appFrame->_asdu.data[7]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wMinute);
	appFrame->_asdu.data[8]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wHour);
	appFrame->_asdu.data[9]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wDay);
	appFrame->_asdu.data[10]  = pApp->m_GlobalOptions.m_TpDelay;


	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));
	BYTE len = 0;
	BuildSendBuffer(appFrame,sBuf,len);
	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//����ң���ն�ʣ��ˮ������ֵ
void CSysDataTrans::IEC_waterdata_EN_16H(CPacketFrameFormat* appFrame)
{
	if (!appFrame)  return;
	if (!appFrame->m_pStation) return;
	if (appFrame->m_pStation->GetWaterValumePara() == NULL) return;

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();

	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._DIR = DOWN_ARROW;
	appFrame->ControlC._DIV = 0;
	appFrame->ControlC._Command = C_CMD_02_USERDATA;

	appFrame->_asdu._AFN = 0x16;
	appFrame->_asdu.data.clear();
	appFrame->_asdu.data = std::vector<BYTE>(3 + 2 + 5);

	BYTE tmpBYTE[3];
	memset(tmpBYTE,0,sizeof(tmpBYTE));
	DWORD decAlert = appFrame->m_pStation->GetWaterValumePara()->GetRemainAlertValume();
	CUdfGlobalFUNC::ToBCD(decAlert,tmpBYTE,3);

	appFrame->_asdu.data[0] = tmpBYTE[0];
	appFrame->_asdu.data[1] = tmpBYTE[1];
	appFrame->_asdu.data[2] = tmpBYTE[2];

	appFrame->_asdu.data[3] = pApp->m_GlobalOptions.m_PW.PW_BUF[0];
	appFrame->_asdu.data[4] = pApp->m_GlobalOptions.m_PW.PW_BUF[1];

	SYSTEMTIME st;
	GetLocalTime(&st);
	appFrame->_asdu.data[5]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wSecond);
	appFrame->_asdu.data[6]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wMinute);
	appFrame->_asdu.data[7]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wHour);
	appFrame->_asdu.data[8]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wDay);
	appFrame->_asdu.data[9]  = pApp->m_GlobalOptions.m_TpDelay;


	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));
	BYTE len;

	BuildSendBuffer(appFrame,sBuf,len);
	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//����ң���ն˵�ˮλ��ֵ��ˮλ������
void CSysDataTrans::IEC_waterdata_EN_17H(CPacketFrameFormat* appFrame)
{
	if (!appFrame) return;
	if (!appFrame->m_pStation) return;
	if (appFrame->m_pStation->GetWaterLevelParasList() == NULL ||
		appFrame->m_pStation->GetWaterLevelParasList()->IsEmpty()) return;

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	appFrame->ControlC._DIR = DOWN_ARROW;
	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._DIV = 0;
	appFrame->ControlC._Command = C_CMD_02_USERDATA;

	appFrame->_asdu._AFN = 0x17;
	int waterLevel_pCount = appFrame->m_pStation->GetWaterLevelParasList()->GetCount();
	appFrame->_asdu.data.clear();
	appFrame->_asdu.data = std::vector<BYTE>(waterLevel_pCount * 7 + 2 + 5);

	int i = 0;
	for (i = 0; i < waterLevel_pCount; i ++)
	{
		BYTE baseByte[3],downByte[2],upByte[2];
		DWORD decBase = 0,decDown = 0, decUp = 0;
		memset(baseByte,0,sizeof(baseByte));
		memset(downByte,0,sizeof(downByte));
		memset(upByte,0,sizeof(upByte));

		CWaterLevel_PAR_PER* pWaterLevel_PARA = appFrame->m_pStation->GetWaterLevelPara(i);
		if (pWaterLevel_PARA)
		{
			decBase = (DWORD)(pWaterLevel_PARA->GetBaseValue() * 100);
			decDown = (DWORD)(pWaterLevel_PARA->GetDownValue() * 100);
			decUp   = (DWORD)(pWaterLevel_PARA->GetUpValue() * 100);

			CUdfGlobalFUNC::ToBCD(decBase,baseByte,3);
			CUdfGlobalFUNC::ToBCD(decDown,downByte,2);
			CUdfGlobalFUNC::ToBCD(decUp,upByte,2);
		}

		appFrame->_asdu.data[i * 7 ] = baseByte[0];
		appFrame->_asdu.data[i * 7 + 1] = baseByte[1];
		appFrame->_asdu.data[i * 7 + 2] = baseByte[2];

		appFrame->_asdu.data[i * 7 + 3] = downByte[0];
		appFrame->_asdu.data[i * 7 + 4] = downByte[1];

		appFrame->_asdu.data[i * 7 + 5] = upByte[0];
		appFrame->_asdu.data[i * 7 + 6] = upByte[1];
	}

	appFrame->_asdu.data[waterLevel_pCount * 7] = pApp->m_GlobalOptions.m_PW.PW_BUF[0];
	appFrame->_asdu.data[waterLevel_pCount * 7 + 1] = pApp->m_GlobalOptions.m_PW.PW_BUF[1];

	SYSTEMTIME st;
	GetLocalTime(&st);
	appFrame->_asdu.data[waterLevel_pCount * 7 + 2]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wSecond);
	appFrame->_asdu.data[waterLevel_pCount * 7 + 3]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wMinute);
	appFrame->_asdu.data[waterLevel_pCount * 7 + 4]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wHour);
	appFrame->_asdu.data[waterLevel_pCount * 7 + 5]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wDay);
	appFrame->_asdu.data[waterLevel_pCount * 7 + 6]  = pApp->m_GlobalOptions.m_TpDelay;

	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));
	BYTE len;

	BuildSendBuffer(appFrame,sBuf,len);
	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//����ң���ն�ˮѹ�ϡ�����
void CSysDataTrans::IEC_waterdata_EN_18H(CPacketFrameFormat* appFrame)
{
	if (!appFrame) return;
	if (!appFrame->m_pStation) return;
	if (appFrame->m_pStation->GetWaterPressureParasList() == NULL ||
		appFrame->m_pStation->GetWaterPressureParasList()->IsEmpty()) return;

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._DIR = DOWN_ARROW;
	appFrame->ControlC._DIV = 0;
	appFrame->ControlC._Command = C_CMD_02_USERDATA;

	appFrame->_asdu._AFN = 0x18;

	int nPointCount = appFrame->m_pStation->GetWaterPressureParasList()->GetCount();
	appFrame->_asdu.data.clear();
	appFrame->_asdu.data = std::vector<BYTE>(nPointCount * 8 + 2 + 5);

	for (int i = 0; i < nPointCount; i ++)
	{
		BYTE upByte[4],downByte[4];
		DWORD upValue= 0,downValue = 0;
		memset(upByte,0,sizeof(upByte));
		memset(downByte,0,sizeof(downByte));

		CWaterPressure_PARA_PER* pPressure_PARA = appFrame->m_pStation->GetWaterPressurePara(i);
		if (pPressure_PARA)
		{
			upValue = (DWORD)(pPressure_PARA->GetUpValue() * 100);
			downValue = (DWORD)(pPressure_PARA->GetDownValue() * 100);
			CUdfGlobalFUNC::ToBCD(upValue,upByte,4);
			CUdfGlobalFUNC::ToBCD(downValue,downByte,4);
		}

		appFrame->_asdu.data[i * 8] = upByte[0];
		appFrame->_asdu.data[i * 8 + 1] = upByte[1];
		appFrame->_asdu.data[i * 8 + 2] = upByte[2];
		appFrame->_asdu.data[i * 8 + 3] = upByte[3];

		appFrame->_asdu.data[i * 8 + 4] = downByte[0];
		appFrame->_asdu.data[i * 8 + 5] = downByte[1];
		appFrame->_asdu.data[i * 8 + 6] = downByte[2];
		appFrame->_asdu.data[i * 8 + 7] = downByte[3];
	}

	appFrame->_asdu.data[nPointCount * 8] = pApp->m_GlobalOptions.m_PW.PW_BUF[0];
	appFrame->_asdu.data[nPointCount * 8 + 1] = pApp->m_GlobalOptions.m_PW.PW_BUF[1];

	SYSTEMTIME st;
	GetLocalTime(&st);
	appFrame->_asdu.data[nPointCount * 8 + 2]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wSecond);
	appFrame->_asdu.data[nPointCount * 8 + 3]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wMinute);
	appFrame->_asdu.data[nPointCount * 8 + 4]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wHour);
	appFrame->_asdu.data[nPointCount * 8 + 5]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wDay);
	appFrame->_asdu.data[nPointCount * 8 + 6]  = pApp->m_GlobalOptions.m_TpDelay;

	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));
	BYTE len;

	BuildSendBuffer(appFrame,sBuf,len);
	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//����ң���ն�ˮ�ʲ������ࡢ����ֵ
void CSysDataTrans::IEC_waterdata_EN_19H(CPacketFrameFormat* appFrame)
{
	if (!appFrame) return;
	if (!appFrame->m_pStation) return;

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._DIR = DOWN_ARROW;
	appFrame->ControlC._DIV = 0;
	appFrame->ControlC._Command = C_CMD_02_USERDATA;

	appFrame->_asdu._AFN = 0x19;

	int nQuality_buf_count = appFrame->m_pStation->GetWaterQualityPara()->Get_Buffer_Count();
	CWaterQuality_PARA* pWaterQuality = appFrame->m_pStation->GetWaterQualityPara();

	appFrame->_asdu.data.clear();
	appFrame->_asdu.data = std::vector<BYTE>(nQuality_buf_count + 2 + 5);


	BYTE b[5];
	CUdfGlobalFUNC::ToBCD(appFrame->m_pStation->GetWaterQualityPara()->GetQualityValue(),b,5);

	for ( int idx = 0; idx < 5; idx ++)
	{
		appFrame->_asdu.data[idx] = b[idx];
	}

	int nStep = 5;
	for (BYTE i = 0; i < 40; i ++)
	{
		CWaterQuality_PARA_PER* pONE_Quality = pWaterQuality->GetOneQualityObject(i);
		if (pWaterQuality->GetBitValue(i) == true && 
			pONE_Quality != NULL)
		{

			int n = 0;
			UINT64 qualityValue = 0;
			if (pONE_Quality->m_PointBits == 0 )
				qualityValue = (UINT64) pONE_Quality->m_UpValume;
			else
			{
				UINT64 Multiple = 1;

				for (n = 0 ; n < pONE_Quality->m_PointBits; n ++)
					Multiple = Multiple * 10;

				qualityValue = (UINT64)(pONE_Quality->m_UpValume * Multiple);
			}

			BYTE tmpBuf[5];
			memset(tmpBuf,0,sizeof(tmpBuf));
			CUdfGlobalFUNC::ToBCD(qualityValue,tmpBuf,pONE_Quality->m_BUF_Cnt);

			for (n= 0; n <pONE_Quality->m_BUF_Cnt; n ++)
			{
				appFrame->_asdu.data[nStep + n] = tmpBuf[n];
			}

			nStep += pONE_Quality->m_BUF_Cnt;
		}
	}

	appFrame->_asdu.data[nStep] = pApp->m_GlobalOptions.m_PW.PW_BUF[0];
	appFrame->_asdu.data[nStep + 1] = pApp->m_GlobalOptions.m_PW.PW_BUF[1];

	SYSTEMTIME st;
	GetLocalTime(&st);
	appFrame->_asdu.data[nStep + 2]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wSecond);
	appFrame->_asdu.data[nStep + 3]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wMinute);
	appFrame->_asdu.data[nStep + 4]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wHour);
	appFrame->_asdu.data[nStep + 5]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wDay);
	appFrame->_asdu.data[nStep + 6]  = pApp->m_GlobalOptions.m_TpDelay;

	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));
	BYTE len = 0;

	BuildSendBuffer(appFrame,sBuf,len);
	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//����ң���ն�ˮ�ʲ������ࡢ����ֵ
void CSysDataTrans::IEC_waterdata_EN_1AH(CPacketFrameFormat* appFrame)
{
	if (!appFrame) return;
	if (!appFrame->m_pStation) return;

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._DIR = DOWN_ARROW;
	appFrame->ControlC._DIV = 0;
	appFrame->ControlC._Command = C_CMD_02_USERDATA;

	appFrame->_asdu._AFN = 0x1A;

	int nQuality_buf_count = appFrame->m_pStation->GetWaterQualityPara()->Get_Buffer_Count();
	CWaterQuality_PARA* pWaterQuality = appFrame->m_pStation->GetWaterQualityPara();

	appFrame->_asdu.data.clear();
	appFrame->_asdu.data = std::vector<BYTE>(nQuality_buf_count + 2 + 5);

	BYTE b[5];
	CUdfGlobalFUNC::ToBCD(appFrame->m_pStation->GetWaterQualityPara()->GetQualityValue(),b,5);

	for ( int idx = 0; idx < 5; idx ++)
	{
		appFrame->_asdu.data[idx] = b[idx];
	}

	int nStep = 5;
	for (BYTE i = 0; i < 40; i ++)
	{
		CWaterQuality_PARA_PER* pONE_Quality = pWaterQuality->GetOneQualityObject(i);
		if (pWaterQuality->GetBitValue(i) == true && 
			pONE_Quality != NULL)
		{
			int n = 0;
			UINT64 qualityValue = 0;
			if (pONE_Quality->m_PointBits == 0 )
				qualityValue = (UINT64) pONE_Quality->m_DownValue;
			else
			{
				UINT64 Multiple = 1;
				for (n = 0 ; n < pONE_Quality->m_PointBits; n ++)
					Multiple = Multiple * 10;
				qualityValue = (UINT64)(pONE_Quality->m_DownValue * Multiple);
			}

			BYTE tmpBuf[5];
			memset(tmpBuf,0,sizeof(tmpBuf));
			CUdfGlobalFUNC::ToBCD(qualityValue,tmpBuf,pONE_Quality->m_BUF_Cnt);

			for (n= 0; n <pONE_Quality->m_BUF_Cnt; n ++)
			{
				appFrame->_asdu.data[nStep + n] = tmpBuf[n];
			}

			nStep += pONE_Quality->m_BUF_Cnt;
		}
	}

	appFrame->_asdu.data[nStep] = pApp->m_GlobalOptions.m_PW.PW_BUF[0];
	appFrame->_asdu.data[nStep + 1] = pApp->m_GlobalOptions.m_PW.PW_BUF[1];

	SYSTEMTIME st;
	GetLocalTime(&st);
	appFrame->_asdu.data[nStep + 2]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wSecond);
	appFrame->_asdu.data[nStep + 3]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wMinute);
	appFrame->_asdu.data[nStep + 4]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wHour);
	appFrame->_asdu.data[nStep + 5]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wDay);
	appFrame->_asdu.data[nStep + 6]  = pApp->m_GlobalOptions.m_TpDelay;

	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));
	BYTE len = 0;

	BuildSendBuffer(appFrame,sBuf,len);
	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//�����ն�վ�����ı�ף���ʼ��ֵ
void CSysDataTrans::IEC_waterdata_EN_1BH(CPacketFrameFormat* appFrame)
{
	if (!appFrame) return;
	if (!appFrame->m_pStation) return;
	if (appFrame->m_pStation->GetFlowInitValumeParasList() == NULL ||
		appFrame->m_pStation->GetFlowInitValumeParasList()->IsEmpty()) return;

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	appFrame->ControlC._DIR = DOWN_ARROW;
	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._DIV = 0;
	appFrame->ControlC._Command = C_CMD_02_USERDATA;

	appFrame->_asdu._AFN = 0x1B;
	appFrame->_asdu.data.clear();

	int npCount = appFrame->m_pStation->GetFlowInitValumeParasList()->GetCount();
	appFrame->_asdu.data = std::vector<BYTE>(npCount * 4 + 2 + 5);

	for (int i = 0 ;i < npCount; i ++)
	{
		BYTE dwByte[4];
		DWORD dwValue = 0;
		memset(dwByte,0,sizeof(dwByte));

		CFlowInitValume_PARA_PER* pFlowInit_PARA = appFrame->m_pStation->GetFlowInitValumePara(i);
		if (pFlowInit_PARA)
		{
			dwValue = pFlowInit_PARA->GetFlowInitValume();

			CUdfGlobalFUNC::ToBCD(dwValue,dwByte,4);

		}

		appFrame->_asdu.data[i * 4] = dwByte[0];
		appFrame->_asdu.data[i * 4 + 1] = dwByte[1];
		appFrame->_asdu.data[i * 4 + 2] = dwByte[2];
		appFrame->_asdu.data[i * 4 + 3] = dwByte[3];
	}

	appFrame->_asdu.data[npCount * 4] = pApp->m_GlobalOptions.m_PW.PW_BUF[0];
	appFrame->_asdu.data[npCount * 4 + 1] = pApp->m_GlobalOptions.m_PW.PW_BUF[1];

	SYSTEMTIME st;
	GetLocalTime(&st);
	appFrame->_asdu.data[npCount * 4 + 2]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wSecond);
	appFrame->_asdu.data[npCount * 4 + 3]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wMinute);
	appFrame->_asdu.data[npCount * 4 + 4]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wHour);
	appFrame->_asdu.data[npCount * 4 + 5]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wDay);
	appFrame->_asdu.data[npCount * 4 + 6]    = pApp->m_GlobalOptions.m_TpDelay;

	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));
	BYTE len = 0;

	BuildSendBuffer(appFrame,sBuf,len);
	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	AddReSendObject (sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//����ң���ն�ת���м������볤
void CSysDataTrans::IEC_waterdata_EN_1CH(CPacketFrameFormat* appFrame)
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));
}

//�����м�վת���ն˵�ַ
void CSysDataTrans::IEC_waterdata_EN_1DH(CPacketFrameFormat* appFrame)
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	if (!appFrame) return;
}

//�����м�վ�������Զ��л����Ա�״̬
void CSysDataTrans::IEC_waterdata_EN_1EH(CPacketFrameFormat* appFrame)
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	if (!appFrame) return;
}

//����ң���ն���������ֵ
void CSysDataTrans::IEC_waterdata_EN_1FH(CPacketFrameFormat* appFrame)
{
	if (!appFrame)		return;
	if (!appFrame->m_pStation) return;
	if (appFrame->m_pStation->GetRainAlertParasList()->IsEmpty()) return;

	appFrame->ControlC._DIR = DOWN_ARROW;
	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._DIV = 0;
	appFrame->ControlC._Command = C_CMD_02_USERDATA;

	appFrame->_asdu._AFN = 0x1F;

	appFrame->_asdu.data.clear();
	int nRainCount = appFrame->m_pStation->GetRainAlertParasList()->GetCount();
	appFrame->_asdu.data = std::vector<BYTE>(nRainCount * 4 + 2 + 5);

	int nC = 0;
	POSITION rPos = appFrame->m_pStation->GetRainAlertParasList()->GetHeadPosition();
	while (rPos != NULL)
	{
		CRainAlert_PARA_PER* pRain_PARA = appFrame->m_pStation->GetRainAlertParasList()->GetNext(rPos);
		if (pRain_PARA)
		{
			float fRainAlert = pRain_PARA->GetRainAlertValue();
			int   nRainAlert  = (int)(fRainAlert * 10);

			WORD nPerTime = pRain_PARA->GetPerTime();

			appFrame->_asdu.data[0 + nC * 4] = CUdfGlobalFUNC::ByteToBCD(nRainAlert % 100);
			appFrame->_asdu.data[1 + nC * 4] = CUdfGlobalFUNC::ByteToBCD(nRainAlert / 100);

			appFrame->_asdu.data[2 + nC * 4] = CUdfGlobalFUNC::ByteToBCD(nPerTime % 100);
			appFrame->_asdu.data[3 + nC * 4] = CUdfGlobalFUNC::ByteToBCD(nPerTime / 100);

			nC ++ ;
		}
	}

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	appFrame->_asdu.data[nC * 4] = pApp->m_GlobalOptions.m_PW.PW_BUF[0];
	appFrame->_asdu.data[nC * 4 + 1] = pApp->m_GlobalOptions.m_PW.PW_BUF[1];

	SYSTEMTIME st;
	GetLocalTime(&st);
	appFrame->_asdu.data[nC * 4 + 2]		= CUdfGlobalFUNC::ByteToBCD((BYTE)st.wSecond);
	appFrame->_asdu.data[nC * 4 + 3]		= CUdfGlobalFUNC::ByteToBCD((BYTE)st.wMinute);
	appFrame->_asdu.data[nC * 4 + 4]		= CUdfGlobalFUNC::ByteToBCD((BYTE)st.wHour);
	appFrame->_asdu.data[nC * 4 + 5]		= CUdfGlobalFUNC::ByteToBCD((BYTE)st.wDay);
	appFrame->_asdu.data[nC * 4 + 6]		= pApp->m_GlobalOptions.m_TpDelay;

	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));
	BYTE len  = 0;

	BuildSendBuffer(appFrame,sBuf,len);
	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//����ң���ն�ģ����������ֵ
void CSysDataTrans::IEC_waterdata_EN_20H(CPacketFrameFormat* appFrame)
{
	if (!appFrame)		return;
	if (!appFrame->m_pStation) return;
	if (appFrame->m_pStation->GetAnalogLimitParasList()->IsEmpty()) return;

	appFrame->ControlC._DIR = DOWN_ARROW;
	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._Command =C_CMD_02_USERDATA;

	appFrame->_asdu._AFN = 0x20;

	int nAnalogCount = appFrame->m_pStation->GetAnalogLimitParasList()->GetCount();
	appFrame->_asdu.data.clear();
	appFrame->_asdu.data = std::vector<BYTE>(nAnalogCount * 6 + 2 + 5);

	int nC = 0 ;
	POSITION rPos = appFrame->m_pStation->GetAnalogLimitParasList()->GetHeadPosition();
	while (rPos != NULL)
	{
		CAnalogLimit_PARA_PER* pAnalog_PARA = appFrame->m_pStation->GetAnalogLimitParasList()->GetNext(rPos);
		if (pAnalog_PARA )
		{
			appFrame->_asdu.data[ nC * 6]     = CUdfGlobalFUNC::ByteToBCD(pAnalog_PARA->GetAnalogUpLimit() % 100);
			appFrame->_asdu.data[ 1 + nC * 6] = CUdfGlobalFUNC::ByteToBCD((pAnalog_PARA->GetAnalogUpLimit() %10000) / 100);
			appFrame->_asdu.data[ 2 + nC * 6] = CUdfGlobalFUNC::ByteToBCD(pAnalog_PARA->GetAnalogUpLimit() /10000);

			appFrame->_asdu.data[ 3 + nC * 6] = CUdfGlobalFUNC::ByteToBCD(pAnalog_PARA->GetAnalogDownLimit() % 100);
			appFrame->_asdu.data[ 4 + nC * 6] = CUdfGlobalFUNC::ByteToBCD((pAnalog_PARA->GetAnalogDownLimit() % 10000)/100);
			appFrame->_asdu.data[ 5 + nC * 6] = CUdfGlobalFUNC::ByteToBCD(pAnalog_PARA->GetAnalogDownLimit() / 10000);
			nC ++;
		}
	}

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	appFrame->_asdu.data[nC * 6] = pApp->m_GlobalOptions.m_PW.PW_BUF[0];
	appFrame->_asdu.data[nC * 6 + 1] = pApp->m_GlobalOptions.m_PW.PW_BUF[1];

	SYSTEMTIME st;
	GetLocalTime(&st);
	appFrame->_asdu.data[nC * 6 + 2]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wSecond);
	appFrame->_asdu.data[nC * 6 + 3]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wMinute);
	appFrame->_asdu.data[nC * 6 + 4]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wHour);
	appFrame->_asdu.data[nC * 6 + 5]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wDay);
	appFrame->_asdu.data[nC * 6 + 6]	 = pApp->m_GlobalOptions.m_TpDelay;


	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));
	BYTE len = 0;    //���ĳ���

	BuildSendBuffer(appFrame,sBuf,len);
	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}



//����ң���ն����λ����������ֵ//add by yhp20160614
void CSysDataTrans::IEC_waterdata_EN_21H(CPacketFrameFormat* appFrame)
{
	if (!appFrame)		return;
	if (!appFrame->m_pStation) return;
	if (appFrame->m_pStation->GetDisplaceLimitParasList()->IsEmpty()) return;

	appFrame->ControlC._DIR = DOWN_ARROW;
	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._Command =C_CMD_14_DISPLACEMENT;

	appFrame->_asdu._AFN = 0x21;

	int nDisplaceCount = appFrame->m_pStation->GetDisplaceLimitParasList()->GetCount();
	appFrame->_asdu.data.clear();
	appFrame->_asdu.data = std::vector<BYTE>(nDisplaceCount * 12 + 2 + 5);

	int nC = 0 ;
	POSITION rPos = appFrame->m_pStation->GetDisplaceLimitParasList()->GetHeadPosition();
	while (rPos != NULL)
	{
		CDisplace_PARA_PER* pDisplace_PARA = appFrame->m_pStation->GetDisplaceLimitParasList()->GetNext(rPos);
		if (pDisplace_PARA )
		{
			double fDisplaceXUpLimitt = pDisplace_PARA->GetDisplaceXUpLimit();
			int   nDisplaceXUpLimit  = (int)(fDisplaceXUpLimitt);
			if(nDisplaceXUpLimit<0) nDisplaceXUpLimit = 0x800000-(int)(fDisplaceXUpLimitt);
			double fDisplaceXDownLimit = pDisplace_PARA->GetDisplaceXDownLimit();
			int   nDisplaceXDownLimit  = (int)(fDisplaceXDownLimit);
			if(nDisplaceXDownLimit<0) nDisplaceXDownLimit = 0x800000-(int)(fDisplaceXDownLimit);
			double fDisplaceYUpLimit = pDisplace_PARA->GetDisplaceYUpLimit();
			int   nDisplaceYUpLimit  = (int)(fDisplaceYUpLimit);
			if(nDisplaceYUpLimit<0) nDisplaceYUpLimit = 0x800000-(int)(fDisplaceYUpLimit);
			double fDisplaceYDownLimit = pDisplace_PARA->GetDisplaceYDownLimit();
			int   nDisplaceYDownLimit  = (int)(fDisplaceYDownLimit);
			if(nDisplaceYDownLimit<0) nDisplaceYDownLimit = 0x800000-(int)(fDisplaceYDownLimit);


			appFrame->_asdu.data[ 0 + nC * 12] = CUdfGlobalFUNC::ByteToBCD(nDisplaceXUpLimit % 100);
			appFrame->_asdu.data[ 1 + nC * 12] = CUdfGlobalFUNC::ByteToBCD((nDisplaceXUpLimit %10000) / 100);
			appFrame->_asdu.data[ 2 + nC * 12] = CUdfGlobalFUNC::ByteToBCD(nDisplaceXUpLimit /10000);

			appFrame->_asdu.data[ 3 + nC * 12] = CUdfGlobalFUNC::ByteToBCD(nDisplaceXDownLimit % 100);
			appFrame->_asdu.data[ 4 + nC * 12] = CUdfGlobalFUNC::ByteToBCD((nDisplaceXDownLimit % 10000)/100);
			appFrame->_asdu.data[ 5 + nC * 12] = CUdfGlobalFUNC::ByteToBCD(nDisplaceXDownLimit / 10000);

			appFrame->_asdu.data[ 6 + nC * 12] = CUdfGlobalFUNC::ByteToBCD(nDisplaceYUpLimit % 100);
			appFrame->_asdu.data[ 7 + nC * 12] = CUdfGlobalFUNC::ByteToBCD((nDisplaceYUpLimit %10000) / 100);
			appFrame->_asdu.data[ 8 + nC * 12] = CUdfGlobalFUNC::ByteToBCD(nDisplaceYUpLimit /10000);

			appFrame->_asdu.data[ 9 + nC * 12] = CUdfGlobalFUNC::ByteToBCD(nDisplaceYDownLimit % 100);
			appFrame->_asdu.data[ 10 + nC * 12] = CUdfGlobalFUNC::ByteToBCD((nDisplaceYDownLimit % 10000)/100);
			appFrame->_asdu.data[ 11 + nC * 12] = CUdfGlobalFUNC::ByteToBCD(nDisplaceYDownLimit / 10000);
			nC ++;
		}
	}

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	appFrame->_asdu.data[nC * 12] = pApp->m_GlobalOptions.m_PW.PW_BUF[0];
	appFrame->_asdu.data[nC * 12 + 1] = pApp->m_GlobalOptions.m_PW.PW_BUF[1];

	SYSTEMTIME st;
	GetLocalTime(&st);
	appFrame->_asdu.data[nC * 12 + 2]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wSecond);
	appFrame->_asdu.data[nC * 12 + 3]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wMinute);
	appFrame->_asdu.data[nC * 12 + 4]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wHour);
	appFrame->_asdu.data[nC * 12 + 5]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wDay);
	appFrame->_asdu.data[nC * 12 + 6]	 = pApp->m_GlobalOptions.m_TpDelay;


	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));
	BYTE len = 0;    //���ĳ���

	BuildSendBuffer(appFrame,sBuf,len);
	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}



//����ң���ն��¶���������ֵ//add by yhp20160614
void CSysDataTrans::IEC_waterdata_EN_22H(CPacketFrameFormat* appFrame)
{
	if (!appFrame)		return;
	if (!appFrame->m_pStation) return;
	if (appFrame->m_pStation->GetTemperaLimitParasList()->IsEmpty()) return;

	appFrame->ControlC._DIR = DOWN_ARROW;
	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._Command =C_CMD_02_USERDATA;

	appFrame->_asdu._AFN = 0x20;

	int nTemperaCount = appFrame->m_pStation->GetTemperaLimitParasList()->GetCount();
	appFrame->_asdu.data.clear();
	appFrame->_asdu.data = std::vector<BYTE>(nTemperaCount * 4 + 2 + 5);

	int nC = 0 ;
	POSITION rPos = appFrame->m_pStation->GetTemperaLimitParasList()->GetHeadPosition();
	while (rPos != NULL)
	{
		CTempera_PARA_PER* pTempera_PARA = appFrame->m_pStation->GetTemperaLimitParasList()->GetNext(rPos);
		if (pTempera_PARA )
		{
			float fTemperaUpLimit = pTempera_PARA->GetTemperaUpLimit();
			int   nTemperaUpLimit  = (int)(fTemperaUpLimit * 10);
			float fTemperaDownLimit = pTempera_PARA->GetTemperaDownLimit();
			int   nTemperaDownLimit  = (int)(fTemperaDownLimit * 10);

			appFrame->_asdu.data[ nC * 4]     = CUdfGlobalFUNC::ByteToBCD(nTemperaUpLimit %100);
			appFrame->_asdu.data[ 1 + nC * 4] = CUdfGlobalFUNC::ByteToBCD(nTemperaUpLimit /100);

			appFrame->_asdu.data[ 3 + nC * 4] = CUdfGlobalFUNC::ByteToBCD(nTemperaDownLimit %100);
			appFrame->_asdu.data[ 4 + nC * 4] = CUdfGlobalFUNC::ByteToBCD(nTemperaDownLimit /100);
			nC ++;
		}
	}

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	appFrame->_asdu.data[nC * 4] = pApp->m_GlobalOptions.m_PW.PW_BUF[0];
	appFrame->_asdu.data[nC * 4 + 1] = pApp->m_GlobalOptions.m_PW.PW_BUF[1];

	SYSTEMTIME st;
	GetLocalTime(&st);
	appFrame->_asdu.data[nC * 4 + 2]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wSecond);
	appFrame->_asdu.data[nC * 4 + 3]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wMinute);
	appFrame->_asdu.data[nC * 4 + 4]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wHour);
	appFrame->_asdu.data[nC * 4 + 5]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wDay);
	appFrame->_asdu.data[nC * 4 + 6]	 = pApp->m_GlobalOptions.m_TpDelay;


	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));
	BYTE len = 0;    //���ĳ���

	BuildSendBuffer(appFrame,sBuf,len);
	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}


//��ң���ն�IC��������Ч
void CSysDataTrans::IEC_waterdata_EN_30H(CPacketFrameFormat* appFrame)
{
	if (!appFrame)		return;

	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._DIR = DOWN_ARROW;
	appFrame->ControlC._DIV = 0;
	appFrame->ControlC._Command = C_CMD_01_CMD;

	appFrame->_asdu._AFN = 0x30;
	appFrame->_asdu.data.clear();
	appFrame->_asdu.data = std::vector<BYTE>(2 + 5);

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	appFrame->_asdu.data[0] = pApp->m_GlobalOptions.m_PW.PW_BUF[0];
	appFrame->_asdu.data[1] = pApp->m_GlobalOptions.m_PW.PW_BUF[1];

	SYSTEMTIME st;
	GetLocalTime(&st);
	appFrame->_asdu.data[2]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wSecond);
	appFrame->_asdu.data[3]  = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wMinute);
	appFrame->_asdu.data[4]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wHour);
	appFrame->_asdu.data[5]  = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wDay);
	appFrame->_asdu.data[6]  = pApp->m_GlobalOptions.m_TpDelay;

	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));
	BYTE len = 0;

	BuildSendBuffer(appFrame,sBuf,len);
	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	AddReSendObject (sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//ȡ��ң���ն�IC������
void CSysDataTrans::IEC_waterdata_EN_31H(CPacketFrameFormat* appFrame)
{
	if (!appFrame)		return;

	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._DIR = DOWN_ARROW;
	appFrame->ControlC._DIV = 0;
	appFrame->ControlC._Command = C_CMD_01_CMD;

	appFrame->_asdu._AFN = 0x31;
	appFrame->_asdu.data.clear();
	appFrame->_asdu.data = std::vector<BYTE>(2 + 5);

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	appFrame->_asdu.data[0] = pApp->m_GlobalOptions.m_PW.PW_BUF[0];
	appFrame->_asdu.data[1] = pApp->m_GlobalOptions.m_PW.PW_BUF[1];

	SYSTEMTIME st;
	GetLocalTime(&st);
	appFrame->_asdu.data[2]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wSecond);
	appFrame->_asdu.data[3]  = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wMinute);
	appFrame->_asdu.data[4]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wHour);
	appFrame->_asdu.data[5]  = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wDay);
	appFrame->_asdu.data[6]  = pApp->m_GlobalOptions.m_TpDelay;

	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));
	BYTE len = 0;

	BuildSendBuffer(appFrame,sBuf,len);
	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	AddReSendObject (sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//��ֵ����Ͷ��
void CSysDataTrans::IEC_waterdata_EN_32H(CPacketFrameFormat* appFrame)
{
	if (!appFrame)		return;

	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._DIR = DOWN_ARROW;
	appFrame->ControlC._DIV = 0;
	appFrame->ControlC._Command = C_CMD_01_CMD;

	appFrame->_asdu._AFN = 0x32;
	appFrame->_asdu.data.clear();
	appFrame->_asdu.data = std::vector<BYTE>(2 + 5);

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	appFrame->_asdu.data[0] = pApp->m_GlobalOptions.m_PW.PW_BUF[0];
	appFrame->_asdu.data[1] = pApp->m_GlobalOptions.m_PW.PW_BUF[1];

	SYSTEMTIME st;
	GetLocalTime(&st);
	appFrame->_asdu.data[2]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wSecond);
	appFrame->_asdu.data[3]  = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wMinute);
	appFrame->_asdu.data[4]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wHour);
	appFrame->_asdu.data[5]  = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wDay);
	appFrame->_asdu.data[6]  = pApp->m_GlobalOptions.m_TpDelay;

	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));
	BYTE len = 0;

	BuildSendBuffer(appFrame,sBuf,len);
	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	AddReSendObject (sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//��ֵ�����˳�
void CSysDataTrans::IEC_waterdata_EN_33H(CPacketFrameFormat* appFrame)
{
	if (!appFrame)		return;

	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._DIR = DOWN_ARROW;
	appFrame->ControlC._DIV = 0;
	appFrame->ControlC._Command = C_CMD_01_CMD;

	appFrame->_asdu._AFN = 0x33;
	appFrame->_asdu.data.clear();
	appFrame->_asdu.data = std::vector<BYTE>(2 + 5);

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	appFrame->_asdu.data[0] = pApp->m_GlobalOptions.m_PW.PW_BUF[0];
	appFrame->_asdu.data[1] = pApp->m_GlobalOptions.m_PW.PW_BUF[1];

	SYSTEMTIME st;
	GetLocalTime(&st);
	appFrame->_asdu.data[2]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wSecond);
	appFrame->_asdu.data[3]  = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wMinute);
	appFrame->_asdu.data[4]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wHour);
	appFrame->_asdu.data[5]  = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wDay);
	appFrame->_asdu.data[6]  = pApp->m_GlobalOptions.m_TpDelay;

	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));
	BYTE len = 0;

	BuildSendBuffer(appFrame,sBuf,len);
	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	AddReSendObject (sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//��ֵ���趨
void CSysDataTrans::IEC_waterdata_EN_34H(CPacketFrameFormat* appFrame)
{
	if (!appFrame)		return;
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));
}

//��ѯң���նˡ��м�վ��ַ
void CSysDataTrans::IEC_waterdata_EN_50H(CPacketFrameFormat* appFrame)
{
	if (!appFrame)		return;
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));
}

//��ѯң���նˡ��м�վʱ��
void CSysDataTrans::IEC_waterdata_EN_51H(CPacketFrameFormat* appFrame)
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));
	if (!appFrame)  return;

	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._DIR = DOWN_ARROW;
	appFrame->ControlC._Command = C_CMD_04_MSUREPARA;

	appFrame->_asdu._AFN = 0x51;

	BYTE len  = 0;
	BuildSendBuffer(appFrame,sBuf,len);
	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	AddReSendObject (sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//��ѯң���ն˹���ģʽ
void CSysDataTrans::IEC_waterdata_EN_52H(CPacketFrameFormat* appFrame)
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	if (!appFrame)  return;

	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._DIR = DOWN_ARROW;
	appFrame->ControlC._Command = C_CMD_04_MSUREPARA;

	appFrame->_asdu._AFN = 0x52;

	BYTE len = 0;
	BuildSendBuffer(appFrame,sBuf,len);

	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	AddReSendObject (sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//��ѯң���ն˵������Ա����༰ʱ����
void CSysDataTrans::IEC_waterdata_EN_53H(CPacketFrameFormat* appFrame)
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	if (!appFrame)  return;

	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._DIR = DOWN_ARROW;
	appFrame->ControlC._Command = C_CMD_04_MSUREPARA;

	appFrame->_asdu._AFN = 0x53;

	BYTE len = 0;
	BuildSendBuffer(appFrame,sBuf,len);

	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	AddReSendObject (sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//��ѯң���ն����ѯ��ʵʱ��������
void CSysDataTrans::IEC_waterdata_EN_54H(CPacketFrameFormat* appFrame)
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	if (!appFrame)   return;

	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._DIR = DOWN_ARROW;
	appFrame->ControlC._Command = C_CMD_04_MSUREPARA;

	appFrame->_asdu._AFN = 0x54;


	BYTE len = 0;
	BuildSendBuffer(appFrame,sBuf,len);

	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	AddReSendObject (sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//��ѯң���ն�����ɹ���ֵ��������ʣ��ˮ��
void CSysDataTrans::IEC_waterdata_EN_55H(CPacketFrameFormat* appFrame)
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	if (!appFrame)   return;

	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._DIR = DOWN_ARROW;
	appFrame->ControlC._Command = C_CMD_04_MSUREPARA;

	appFrame->_asdu._AFN = 0x55;

	BYTE len = 0;
	BuildSendBuffer(appFrame,sBuf,len);

	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	AddReSendObject (sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//��ѯң���ն�ʣ��ˮ���ͱ���ֵ
void CSysDataTrans::IEC_waterdata_EN_56H(CPacketFrameFormat* appFrame)
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	if (!appFrame)   return;

	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._DIR = DOWN_ARROW;
	appFrame->ControlC._Command = C_CMD_04_MSUREPARA;

	appFrame->_asdu._AFN = 0x56;

	BYTE len = 0;
	BuildSendBuffer(appFrame,sBuf,len);

	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	AddReSendObject (sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//��ѯң���ն�ˮλ��ֵ��ˮλ������ֵ
void CSysDataTrans::IEC_waterdata_EN_57H(CPacketFrameFormat* appFrame)
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	if (!appFrame)   return;

	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._DIR = DOWN_ARROW;
	appFrame->ControlC._Command = C_CMD_04_MSUREPARA;

	appFrame->_asdu._AFN = 0x57;

	BYTE len = 0;
	BuildSendBuffer(appFrame,sBuf,len);

	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	AddReSendObject (sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//��ѯң���ն�ˮѹ�ϡ�����
void CSysDataTrans::IEC_waterdata_EN_58H(CPacketFrameFormat* appFrame)
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	if (!appFrame)   return;

	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._DIR = DOWN_ARROW;
	appFrame->ControlC._Command = C_CMD_04_MSUREPARA;

	appFrame->_asdu._AFN = 0x58;
	BYTE len = 0;
	BuildSendBuffer(appFrame,sBuf,len);

	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	AddReSendObject (sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//��ѯң���ն�ˮ�ʲ������ࡢ����ֵ
void CSysDataTrans::IEC_waterdata_EN_59H(CPacketFrameFormat* appFrame)
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	if (!appFrame)   return;

	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._DIR = DOWN_ARROW;
	appFrame->ControlC._Command = C_CMD_04_MSUREPARA;

	appFrame->_asdu._AFN = 0x59;

	BYTE len = 0;
	BuildSendBuffer(appFrame,sBuf,len);

	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	AddReSendObject (sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//��ѯң���ն�ˮ�ʲ������ࡢ����ֵ
void CSysDataTrans::IEC_waterdata_EN_5AH(CPacketFrameFormat* appFrame)
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	if (!appFrame)   return;

	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._DIR = DOWN_ARROW;
	appFrame->ControlC._Command = C_CMD_04_MSUREPARA;

	appFrame->_asdu._AFN = 0x5A;

	BYTE len = 0;
	BuildSendBuffer(appFrame,sBuf,len);

	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	AddReSendObject (sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//��ѯң���ն˵�ʵʱֵ
void CSysDataTrans::IEC_waterdata_EN_5BH(CPacketFrameFormat* appFrame)
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	if (!appFrame)   return;

	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._DIR = DOWN_ARROW;

	appFrame->_asdu._AFN = 0x5B;

	BYTE len = 0;
	BuildSendBuffer(appFrame,sBuf,len);

	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	AddReSendObject (sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//��ѯң���ն���ʷ�ռ�¼
void CSysDataTrans::IEC_waterdata_EN_5CH(CPacketFrameFormat* appFrame)
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	if (!appFrame)   return;
	if (!appFrame->m_pStation) return;

	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._Command = appFrame->m_pStation->GetDayHistoryObject()->GetDataType();
	appFrame->ControlC._DIR = DOWN_ARROW;

	appFrame->_asdu._AFN = 0x5C;
	appFrame->_asdu.data.clear();
	appFrame->_asdu.data = std::vector<BYTE>(3);

	BYTE ZBtype		= appFrame->m_pStation->GetDayHistoryObject()->GetZBType();
	BYTE QueryNo	= appFrame->m_pStation->GetDayHistoryObject()->GetQueryNo();
	BYTE bDay =   (BYTE)appFrame->m_pStation->GetDayHistoryObject()->GetDay();
	BYTE bMonth = (BYTE)appFrame->m_pStation->GetDayHistoryObject()->GetMonth();
	BYTE bYear  = (BYTE)(appFrame->m_pStation->GetDayHistoryObject()->GetYear() - 2000);

	appFrame->_asdu.data[0] = (ZBtype << 6) | CUdfGlobalFUNC::ByteToBCD(bDay);
	appFrame->_asdu.data[1] = (QueryNo << 5) | CUdfGlobalFUNC::ByteToBCD(bMonth);
	appFrame->_asdu.data[2] = CUdfGlobalFUNC::ByteToBCD(bYear);

	BYTE len = 0;
	BuildSendBuffer(appFrame,sBuf,len);

	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	AddReSendObject (sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//��ѯң���ն˵��¼���¼
void CSysDataTrans::IEC_waterdata_EN_5DH(CPacketFrameFormat* appFrame)
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	if (!appFrame)   return;

	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._DIR = DOWN_ARROW;
	appFrame->ControlC._Command = C_CMD_05_MSUERSTATE;

	appFrame->_asdu._AFN = 0x5D;

	BYTE len = 0;
	BuildSendBuffer(appFrame,sBuf,len);

	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	AddReSendObject (sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//��ѯң���ն�״̬�ͱ���״̬
void CSysDataTrans::IEC_waterdata_EN_5EH(CPacketFrameFormat* appFrame)
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	if (!appFrame)   return;

	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._DIR = DOWN_ARROW;
	appFrame->ControlC._Command = C_CMD_05_MSUERSTATE;

	appFrame->_asdu._AFN = 0x5E;

	BYTE len = 0;
	BuildSendBuffer(appFrame,sBuf,len);

	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	AddReSendObject (sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//��ѯˮ�õ��ʵʱ��������
void CSysDataTrans::IEC_waterdata_EN_5FH(CPacketFrameFormat* appFrame)
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	if (!appFrame)   return;

	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._DIR = DOWN_ARROW;
	appFrame->ControlC._Command = C_CMD_05_MSUERSTATE;

	appFrame->_asdu._AFN = 0x5F;

	BYTE len = 0;
	BuildSendBuffer(appFrame,sBuf,len);

	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	AddReSendObject (sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//��ѯң���ն�״̬�ͱ���״̬
void CSysDataTrans::IEC_waterdata_EN_60H(CPacketFrameFormat* appFrame)
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	if (!appFrame)   return;

	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._DIR = DOWN_ARROW;
	appFrame->ControlC._Command = C_CMD_04_MSUREPARA;

	appFrame->_asdu._AFN = 0x60;

	BYTE len = 0;
	BuildSendBuffer(appFrame,sBuf,len);

	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	AddReSendObject (sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//��ѯң���ն�ͼ���¼
void CSysDataTrans::IEC_waterdata_EN_61H(CPacketFrameFormat* appFrame)
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	if (!appFrame)   return;

	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._DIR = DOWN_ARROW;
	appFrame->ControlC._Command = C_CMD_04_MSUREPARA;

	appFrame->_asdu._AFN = 0x61;

	BYTE len = 0;
	BuildSendBuffer(appFrame,sBuf,len);

	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	AddReSendObject (sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//��ѯ�м�վת���ն˵�ַ
void CSysDataTrans::IEC_waterdata_EN_62H(CPacketFrameFormat* appFrame)
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	if (!appFrame)   return;

	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._DIR = DOWN_ARROW;
	appFrame->ControlC._Command = C_CMD_04_MSUREPARA;

	appFrame->_asdu._AFN = 0x62;

	BYTE len = 0;
	BuildSendBuffer(appFrame,sBuf,len);

	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	AddReSendObject (sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//��ѯ�м�վ������״̬���л���¼
void CSysDataTrans::IEC_waterdata_EN_63H(CPacketFrameFormat* appFrame)  
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	if (!appFrame)   return;

	appFrame->_asdu._AFN = 0x63;

	BYTE len = 0;
	BuildSendBuffer(appFrame,sBuf,len);

	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	AddReSendObject (sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//��ѯң���ն���������ֵ
void CSysDataTrans::IEC_waterdata_EN_64H(CPacketFrameFormat* appFrame)  
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	if (!appFrame)  return;

	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._DIR = DOWN_ARROW;
	appFrame->ControlC._Command = C_CMD_04_MSUREPARA;

	appFrame->_asdu._AFN = 0x64;

	BYTE len = 0;
	BuildSendBuffer(appFrame,sBuf,len);

	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	AddReSendObject (sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//��ѯң���ն�ģ����������ֵ
void CSysDataTrans::IEC_waterdata_EN_65H(CPacketFrameFormat* appFrame)  
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	if (!appFrame)  return;

	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._DIR = DOWN_ARROW;
	appFrame->ControlC._Command = C_CMD_04_MSUREPARA;

	appFrame->_asdu._AFN = 0x65;

	BYTE len = 0;
	BuildSendBuffer(appFrame,sBuf,len);

	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	AddReSendObject (sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}


//��ѯң���ն����λ����������ֵ//add by yhp20160614
void CSysDataTrans::IEC_waterdata_EN_6AH(CPacketFrameFormat* appFrame)  
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	if (!appFrame)  return;

	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._DIR = DOWN_ARROW;
	appFrame->ControlC._Command = C_CMD_04_MSUREPARA;

	appFrame->_asdu._AFN = 0x6A;

	BYTE len = 0;
	BuildSendBuffer(appFrame,sBuf,len);

	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	AddReSendObject (sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//��ѯң���ն��¶���������ֵ//add by yhp20160614
void CSysDataTrans::IEC_waterdata_EN_6BH(CPacketFrameFormat* appFrame)  
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	if (!appFrame)  return;

	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._DIR = DOWN_ARROW;
	appFrame->ControlC._Command = C_CMD_04_MSUREPARA;

	appFrame->_asdu._AFN = 0x6B;

	BYTE len = 0;
	BuildSendBuffer(appFrame,sBuf,len);

	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	AddReSendObject (sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//��ѯң���ն���չ����״̬���ն�״̬
void CSysDataTrans::IEC_waterdata_EN_66H(CPacketFrameFormat* appFrame)  
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	if (!appFrame)  return;

	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._DIR = DOWN_ARROW;
	appFrame->ControlC._Command = C_CMD_05_MSUERSTATE;

	appFrame->_asdu._AFN = 0x66;
	BYTE len = 0;
	BuildSendBuffer(appFrame,sBuf,len);

	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	AddReSendObject (sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//��ѯң���ն���չʵʱֵ
void CSysDataTrans::IEC_waterdata_EN_67H(CPacketFrameFormat* appFrame) 
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	if (!appFrame)  return;
	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._DIR = DOWN_ARROW;


	appFrame->_asdu._AFN = 0x67;

	BYTE len = 0;
	BuildSendBuffer(appFrame,sBuf,len);

	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	AddReSendObject (sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//��ʱ�Ա�ʵʱ���ݣ�ȷ��֡��
void CSysDataTrans::IEC_waterdata_EN_80H(CPacketFrameFormat* appFrame)
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	if (!appFrame)         return;

	appFrame->ControlC._DIR = DOWN_ARROW;

	appFrame->_asdu._AFN = 0x80;
	appFrame->_asdu.data.clear();
	appFrame->_asdu.data = std::vector<BYTE>(1);

	if (appFrame->m_pStation != NULL)
		appFrame->_asdu.data[0] = appFrame->m_pStation->GetWorkMode();
	else
		appFrame->_asdu.data[0] = 1; //�Ա�ģʽ

	BYTE len = 0;
	BuildSendBuffer(appFrame,sBuf,len);

	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//����Ա��������ݣ�ȷ��֡��
void CSysDataTrans::IEC_waterdata_EN_81H(CPacketFrameFormat* appFrame) 
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	if (!appFrame)         return;
	appFrame->ControlC._DIR = DOWN_ARROW;


	appFrame->_asdu._AFN = 0x81;
	appFrame->_asdu.data.clear();
	appFrame->_asdu.data = std::vector<BYTE>(1);

	if (appFrame->m_pStation != NULL)
		appFrame->_asdu.data[0] = appFrame->m_pStation->GetWorkMode();
	else
		appFrame->_asdu.data[0] = 1;

	BYTE len = 0;

	BuildSendBuffer(appFrame,sBuf,len);
	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//�˹�������ȷ��֡��
void CSysDataTrans::IEC_waterdata_EN_82H(CPacketFrameFormat* appFrame)
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	if (!appFrame)         return;
	appFrame->ControlC._DIR = DOWN_ARROW;

	appFrame->_asdu._AFN = 0x82;
	appFrame->_asdu.data.clear();
	appFrame->_asdu.data = std::vector<BYTE>(1);

	if (appFrame->m_pStation != NULL)
		appFrame->_asdu.data[0] = appFrame->m_pStation->GetWorkMode();
	else
		appFrame->_asdu.data[0] = 1;

	BYTE len = 0;
	BuildSendBuffer(appFrame,sBuf,len);
	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//��ʱ�Ա���չʵʱ���ݣ���Ӧ֡��
void CSysDataTrans::IEC_waterdata_EN_83H(CPacketFrameFormat* appFrame) 
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	if (!appFrame)  return;
	appFrame->ControlC._DIR = DOWN_ARROW;

	appFrame->_asdu._AFN = 0x83;
	appFrame->_asdu.data.clear();
	appFrame->_asdu.data = std::vector<BYTE>(1);

	if (appFrame->m_pStation != NULL)
		appFrame->_asdu.data[0] = appFrame->m_pStation->GetWorkMode();
	else
		appFrame->_asdu.data[0] = 1;

	BYTE len = 0;

	BuildSendBuffer(appFrame,sBuf,len);
	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//����Ա���չ�������ݣ���Ӧ֡��
void CSysDataTrans::IEC_waterdata_EN_84H(CPacketFrameFormat* appFrame)  
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	if (!appFrame)  return;

	appFrame->ControlC._DIR = DOWN_ARROW;


	appFrame->_asdu._AFN = 0x84;
	appFrame->_asdu.data.clear();
	appFrame->_asdu.data = std::vector<BYTE>(1);

	if (appFrame->m_pStation != NULL)
		appFrame->_asdu.data[0] = appFrame->m_pStation->GetWorkMode();
	else
		appFrame->_asdu.data[0] = 1;

	BYTE len = 0;

	BuildSendBuffer(appFrame,sBuf,len);
	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//��λң���ն˲�����״̬
void CSysDataTrans::IEC_waterdata_EN_90H(CPacketFrameFormat* appFrame)
{
	if (!appFrame) return ;
	if (!appFrame->m_pStation) return;
	if (appFrame->m_pStation->Get_RESET_Cmd() != 1 &&
		appFrame->m_pStation->Get_RESET_Cmd() != 2)
		return;

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();

	appFrame->ControlC._DIR = DOWN_ARROW;
	appFrame->ControlC._DIV = 0;
	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._Command = C_CMD_01_CMD;

	appFrame->_asdu._AFN = 0x90;
	appFrame->_asdu.data.clear();
	appFrame->_asdu.data = std::vector<BYTE>(1 + 2 + 5);

	appFrame->_asdu.data[0] = appFrame->m_pStation->Get_RESET_Cmd();
	appFrame->_asdu.data[1] = pApp->m_GlobalOptions.m_PW.PW_BUF[0];
	appFrame->_asdu.data[2] = pApp->m_GlobalOptions.m_PW.PW_BUF[1];

	SYSTEMTIME st;
	GetLocalTime(&st);
	appFrame->_asdu.data[3]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wSecond);
	appFrame->_asdu.data[4]  = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wMinute);
	appFrame->_asdu.data[5]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wHour);
	appFrame->_asdu.data[6]  = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wDay);
	appFrame->_asdu.data[7]  = pApp->m_GlobalOptions.m_TpDelay;

	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));
	BYTE len = 0;

	BuildSendBuffer(appFrame,sBuf,len);
	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface, &(appFrame->m_NetInfo));
	AddReSendObject (sBuf,len,appFrame->m_CommInterface, &(appFrame->m_NetInfo));
}

//���ң���ն���ʷ���ݵ�Ԫ
void CSysDataTrans::IEC_waterdata_EN_91H(CPacketFrameFormat* appFrame)
{
	if (!appFrame) return;
	if (!appFrame->m_pStation) return;

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._DIR = 0;
	appFrame->ControlC._DIV = 0;
	appFrame->ControlC._Command = C_CMD_01_CMD;

	appFrame->_asdu._AFN = 0x91;
	appFrame->_asdu.data.clear();
	appFrame->_asdu.data = std::vector<BYTE>(1 + 2 + 5);

	appFrame->_asdu.data[0] = appFrame->m_pStation->GetClearHistoryDataPara()->GetClearHistoryPARA();

	appFrame->_asdu.data[1] = pApp->m_GlobalOptions.m_PW.PW_BUF[0];
	appFrame->_asdu.data[2] = pApp->m_GlobalOptions.m_PW.PW_BUF[1];

	SYSTEMTIME st;
	GetLocalTime(&st);
	appFrame->_asdu.data[3]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wSecond);
	appFrame->_asdu.data[4]  = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wMinute);
	appFrame->_asdu.data[5]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wHour);
	appFrame->_asdu.data[6]  = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wDay);
	appFrame->_asdu.data[7]  = pApp->m_GlobalOptions.m_TpDelay;

	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));
	BYTE len = 0;

	BuildSendBuffer(appFrame,sBuf,len);
	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface, &(appFrame->m_NetInfo));
	AddReSendObject (sBuf,len,appFrame->m_CommInterface, &(appFrame->m_NetInfo));
}

//ң������ˮ�û���/բ��
void CSysDataTrans::IEC_waterdata_EN_92H(CPacketFrameFormat* appFrame)
{
	if (!appFrame) return;
	if (!appFrame->m_pStation) return;

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	appFrame->ControlC._DIR = DOWN_ARROW;
	appFrame->ControlC._DIV = 0;
	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._Command = C_CMD_01_CMD;

	appFrame->_asdu._AFN = 0x92;
	appFrame->_asdu.data.clear();

	appFrame->_asdu.data = std::vector<BYTE>(1 + 2 + 5);
	appFrame->_asdu.data[0] = appFrame->m_pStation->GetCurPumpGateObjecte()->GetYKByte();

	appFrame->_asdu.data[1] = pApp->m_GlobalOptions.m_PW.PW_BUF[0];
	appFrame->_asdu.data[2] = pApp->m_GlobalOptions.m_PW.PW_BUF[1];

	SYSTEMTIME st;
	GetLocalTime(&st);
	appFrame->_asdu.data[3]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wSecond);
	appFrame->_asdu.data[4]  = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wMinute);
	appFrame->_asdu.data[5]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wHour);
	appFrame->_asdu.data[6]  = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wDay);
	appFrame->_asdu.data[7]  = pApp->m_GlobalOptions.m_TpDelay;	

	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));
	BYTE len = 0;

	BuildSendBuffer(appFrame,sBuf,len);
	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	AddReSendObject (sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//ң�عر�ˮ�û���/բ��
void CSysDataTrans::IEC_waterdata_EN_93H(CPacketFrameFormat* appFrame)
{
	if (!appFrame) return;
	if (!appFrame->m_pStation) return;

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	appFrame->ControlC._DIR = DOWN_ARROW;
	appFrame->ControlC._DIV = 0;
	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._Command = C_CMD_01_CMD;

	appFrame->_asdu._AFN = 0x93;
	appFrame->_asdu.data.clear();

	appFrame->_asdu.data = std::vector<BYTE>(1 + 2 + 5);
	appFrame->_asdu.data[0] = appFrame->m_pStation->GetCurPumpGateObjecte()->GetYKByte();

	appFrame->_asdu.data[1] = pApp->m_GlobalOptions.m_PW.PW_BUF[0];
	appFrame->_asdu.data[2] = pApp->m_GlobalOptions.m_PW.PW_BUF[1];

	SYSTEMTIME st;
	GetLocalTime(&st);
	appFrame->_asdu.data[3]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wSecond);
	appFrame->_asdu.data[4]  = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wMinute);
	appFrame->_asdu.data[5]	 = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wHour);
	appFrame->_asdu.data[6]  = CUdfGlobalFUNC::ByteToBCD((BYTE)st.wDay);
	appFrame->_asdu.data[7]  = pApp->m_GlobalOptions.m_TpDelay;	

	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));
	BYTE len = 0;

	BuildSendBuffer(appFrame,sBuf,len);
	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	AddReSendObject (sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//ң���ն˻��м�վͨ�Ż��л�
void CSysDataTrans::IEC_waterdata_EN_94H(CPacketFrameFormat* appFrame)
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	if (!appFrame)   return;
}

//ң���м�վ�������л�
void CSysDataTrans::IEC_waterdata_EN_95H(CPacketFrameFormat* appFrame)
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	if (!appFrame)   return;
}

//����·��������
int CSysDataTrans::SendRtuLinkFrame(BYTE* sBuf,int len,CPacketFrameFormat* appFrame)
{
	if (!appFrame) return 0;

	return SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//��ͨ����·���ͱ�������
int CSysDataTrans::SendRtuLinkFrame(BYTE* sBuf,int len,BYTE CmInterface/* =Serial_CommInterface */,PNET_INFO pToNetInfo /* = NULL */)
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
				AddSendLog(sBuf,len);
		m_cs_SendData.Unlock();
	}

	return sendLen;
}

int  CSysDataTrans::SendRtuLinkFrame_TmInit(BYTE* sBuf, int Len)
{
	if (!sBuf  || Len <=0 ) return 0;
	int sendLen = Len;

	//��ң���ն��·�����
	if (m_pSubStation != NULL)
	{
		m_cs_SendData.Lock();

		//����ͨ��ֻ�ղ��� 2012-07-13 �޸�
		if (m_pSubStation->GetCmType() != SC_CommChannel)
			if (m_pSubStation->SendData(sBuf,Len))
				AddSendLog(sBuf,Len);
		m_cs_SendData.Unlock();
	}

	return sendLen;

}

//��ȡ����C������
CString CSysDataTrans::GetFunName(BYTE _DIR,BYTE _CMD)
{
	CString  strResult = "";

	if (_DIR  == 0 )  // ���б���
	{
		switch (_CMD)
		{
		case 0:
			strResult ="����";
			break;
		case 1:
			strResult ="����";
			break;
		case 2:
			strResult ="�û�����";
			break;
		case 3:
			strResult ="��·����";
			break;
		case 4:
			strResult ="�������";
			break;
		case 5:
			strResult ="����״̬";
			break;
		case 6:
			strResult ="ˮλ����";
			break;
		case 7:
			strResult ="��������";
			break;
		case 8:
			strResult ="ˮѹ����";
			break;
		case 9:
			strResult ="ˮ�ʲ���";
			break;
		case 10:
			strResult ="�ۺϲ���";
			break;
		case 11:
			strResult = "��������";
			break;
		case 12:
			strResult = "ģ��������";
			break;
		case 13:
		case 14:
		case 15:
			strResult ="����";
			break;
		}
	}
	else if (_DIR == 1) //���б���
	{
		switch (_CMD)
		{
		case 0:
			strResult = "����";
			break;
		case 1:
			strResult = "�Ͽ�";
			break;
		case 2:
			strResult = "�û�����";
			break;
		case 3:
			strResult = "��·״̬";
			break;
		case 4:
			strResult = "��Ӧ����";
			break;
		case 5:
			strResult = "ˮλ����";
			break;
		case 6:
			strResult = "��������";
			break;
		case 7:
			strResult = "ˮѹ����";
			break;
		case 8:
			strResult = "ˮ�ʲ���";
			break;
		case 9:
			strResult = "�ۺϲ���";
			break;
		case 10:
			strResult = "������״̬����";
			break;
		case 11:
			strResult = "��������";
			break;
		case 12:
			strResult = "ģ��������";
			break;
		case 13:
		case 14:
		case 15:
			strResult ="����";
			break;
		}
	}
	return strResult;
}


//������ת��Ϊ�ַ�����Ϣ
CString CSysDataTrans::BufferToString(BYTE* buffer,int Len)
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

//����·�������־
//����־��Ϣ�����ң���ն˵����Դ���
void CSysDataTrans::AddSendLog(BYTE* buffer,int len)
{  
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	CRealTimeDB* pRealDb = pApp->m_pReadDB;
	if (!pRealDb)			 return ;
	if (!buffer  || len <=0) return;

	CPacketFrameFormat Pff;
	if (!CUdfGlobalFUNC::IEC_waterdata_to_APPFRAME(buffer,len,&Pff))
		return;

	CRtuStation* pSub =  pRealDb->SearchRtuStation(Pff._addr_info.A1.ProvinceCode,
		Pff._addr_info.A1.RegionCode,
		Pff._addr_info.A2);

	CString strBuffer = BufferToString(buffer,len);

	if (pSub != NULL)
	{
		CString strLogBuffer= "";
		strLogBuffer.Format("��,DIR=%d,DIV=%d,FCB=%d,������=%-2d,AFN=%02XH,Len=%-3d,����=%s",
			Pff.ControlC._DIR,
			Pff.ControlC._DIV,
			Pff.ControlC._FCB,
			Pff.ControlC._Command,
			Pff._asdu._AFN,len, strBuffer);

		AddLogMessage(pSub,strLogBuffer,Normal_Log_Message);
	}
}

//��ӽ��ձ�����־��Ϣ
//����־��Ϣ�����ң���ն˵����Դ���
void CSysDataTrans::AddRecvLog(BYTE* buffer,int len,CPacketFrameFormat* waterFrame/* =NULL */)
{
	if (!buffer  || len <=0) return;

	CString strBuffer = BufferToString(buffer,len);
	CString strLogBuffer ="";

	CPacketFrameFormat Pff;
	if (waterFrame == NULL)
	{
		if (!CUdfGlobalFUNC::IEC_waterdata_to_APPFRAME(buffer,len,&Pff))  
			return;

		waterFrame = &Pff;
	}

	if (waterFrame != NULL)
	{
		if (waterFrame->m_pStation == NULL)
		{
			CCMSApp* pApp = (CCMSApp*)AfxGetApp();
			CRealTimeDB* pRealDb = pApp->m_pReadDB;
			if (!pRealDb)	return ;

			CRtuStation* pSub =  pRealDb->SearchRtuStation(waterFrame->_addr_info.A1.ProvinceCode,
				waterFrame->_addr_info.A1.RegionCode,
				waterFrame->_addr_info.A2);
			waterFrame->m_pStation = pSub;
		}

		CString strBuffer = BufferToString(buffer,len);
		CString strLogBuffer ="";

		strLogBuffer.Format("��,DIR=%d,DIV=%d,FCB=%d,������=%-2d,AFN=%02XH,Len=%-3d,����=%s",
			waterFrame->ControlC._DIR,
			waterFrame->ControlC._DIV,
			waterFrame->ControlC._FCB,
			waterFrame->ControlC._Command,
			waterFrame->_asdu._AFN,len,strBuffer);

		AddLogMessage(waterFrame->m_pStation ,strLogBuffer,Normal_Log_Message);
	}
}

//����ң���ն˵�ͨ��״̬
//�ص�:�����б���Ϣ�����ͨ��״̬�ı���־��Ϣ
void CSysDataTrans::UpdateRtuCommState(CRtuStation* pSub)
{
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	CMainFrame* pMainFrame = (CMainFrame*)pApp->m_pMainWnd;

	if (pMainFrame && pMainFrame->m_pRightView && pMainFrame->m_pRightView->GetSafeHwnd())
		pMainFrame->m_pRightView->UpdateCommState(pSub);
}

void CSysDataTrans::UpdateRtuCommInfo(CRtuStation* pSub)
{
	if (GetRtuPropertyDlg() == NULL ||
		GetRtuPropertyDlg()->GetRtuStation() == NULL ||
		GetRtuPropertyDlg()->GetRtuStation() != pSub)
		return;

	if (GetRtuPropertyDlg()->GetTab_AttrBasicInfo()->GetSafeHwnd())
		GetRtuPropertyDlg()->GetTab_AttrBasicInfo()->UpdateCommInfo();
}

//����ط�����
void CSysDataTrans::CheckResendObject()
{
	//��鱣����Ҫ�ط������List
	m_Mutex_ReSend.Lock();

	//�洢����ط��������ʱ����
	CList<RESEND_Object*> tempList;
	while (!m_lstReSendObjects.IsEmpty())
	{
		RESEND_Object* pObj = m_lstReSendObjects.RemoveHead();

		if (pObj != NULL)
		{
			BOOL blDeleted = FALSE ;								//�ط������Ƿ���ɾ���ı�־

			if (pObj->m_resend_timer.WaitTimerState() == true)		//����Ƿ����ط���ʱ��
			{
				pObj->m_packetFrame.ControlC._FCB -- ;				//FCB �Զ���һ

				if (pObj->m_packetFrame.ControlC._FCB == 0)
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
					BuildSendBuffer(&(pObj->m_packetFrame),sBuf,nLen);
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
		RESEND_Object* pObj = tempList.RemoveHead();
		if (pObj)
			m_lstReSendObjects.AddTail(pObj);
	}

	m_Mutex_ReSend.Unlock();
}

//���һ���ط�����
void CSysDataTrans::AddReSendObject(BYTE* sBuf,int len,CPacketFrameFormat* appFrame)
{
	if (!appFrame) return;

	AddReSendObject(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//���һ���ط�����
void CSysDataTrans::AddReSendObject(BYTE* sBuf,int len,BYTE CmInterface /* = Serial_CommInterface */,PNET_INFO pToNetInfo /* = NULL */)
{
	if (!sBuf) return;
	if (len <= 0) return;

	//����ͨ��ʱ�������ط�����
	if (m_pSubStation != NULL && 
		m_pSubStation->GetCmType() == SC_CommChannel)
		return;

	if (CmInterface == Ethernet_CommInerface && pToNetInfo == NULL) 
		return;

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	RESEND_Object* pObj = new RESEND_Object();

	if (pObj)
	{
		if (!CUdfGlobalFUNC::IEC_waterdata_to_APPFRAME(sBuf,len,&(pObj->m_packetFrame)))
		{
			delete pObj;
			pObj = NULL;
			return;
		}

		pObj->m_resendInterval = pApp->m_GlobalOptions.m_ResendInterval;
		pObj->m_resend_timer.SetWaitTimer(pObj->m_resendInterval * 1000);  //�����ط�ʱ����
		pObj->m_CommInterface = CmInterface;
		if (pToNetInfo != NULL)
		{
			pObj->m_ToNetInfo.mIpAddress = pToNetInfo->mIpAddress;
			pObj->m_ToNetInfo.mPort      = pToNetInfo->mPort;
		}

		m_Mutex_ReSend.Lock();
		m_lstReSendObjects.AddTail(pObj);
		m_Mutex_ReSend.Unlock();
	}
}

//���ط��б���������ȡһ���ط�����
/*
* Command Ϊ������������
* AFN     ΪӦ��������
* _Addr   Ϊ��Ӧ��ң���ն˵�ַ��Ϣ
*/
RESEND_Object* CSysDataTrans::GetReSendObject(BYTE Command,BYTE AFN,_ADDR_Info _Addr)
{
	RESEND_Object* pReSendObj = NULL;
	m_Mutex_ReSend.Lock();
	POSITION rPos = m_lstReSendObjects.GetHeadPosition();
	while (rPos != NULL)
	{
		RESEND_Object* pObj = m_lstReSendObjects.GetAt(rPos);
		if (pObj && pObj->m_packetFrame.ControlC._Command == Command &&
			pObj->m_packetFrame._asdu._AFN == AFN &&
			pObj->m_packetFrame._addr_info.A1.ProvinceCode == _Addr.A1.ProvinceCode &&
			pObj->m_packetFrame._addr_info.A1.RegionCode == _Addr.A1.RegionCode &&
			pObj->m_packetFrame._addr_info.A2 == _Addr.A2 &&
			pObj->m_packetFrame._addr_info.A3 == _Addr.A3)
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

//���ط��б���ɾ��ĳһ�ط�����
/*
* Command Ϊ������������
* AFN     ΪӦ��������
* _Addr   Ϊ��Ӧ��ң���ն˵�ַ��Ϣ
*/
void CSysDataTrans::DeleteOneReSendObject(BYTE Command,BYTE AFN,_ADDR_Info _Addr)
{
	RESEND_Object* pDel_RESEND_Obj = GetReSendObject(Command,AFN,_Addr);
	if (pDel_RESEND_Obj)
	{
		delete pDel_RESEND_Obj;
		pDel_RESEND_Obj = NULL;
	}
}

//�����·�����
/*
* ͨ��CPacketFrameFormat���������·��ı�����Ϣ
* sbuf:�·��ı���֡����   (In)
* len :���ĵĳ��ȣ�<=255��(Out)
*/
void CSysDataTrans::BuildSendBuffer(CPacketFrameFormat* packetFrame,BYTE* sBuf,BYTE& len)
{
	if (!packetFrame) return;
	if (!sBuf)        return;

	BYTE dataSize = (BYTE)packetFrame->_asdu.data.size();

	sBuf[0]		= 0x68;
	sBuf[1]		= 7 + dataSize;
	sBuf[2]		= 0x68;

	//������
	sBuf[3]     =   packetFrame->ControlC.GetControlCByte();
	//��ַ��
	sBuf[4]		=   CUdfGlobalFUNC::ByteToBCD(packetFrame->_addr_info.A1.ProvinceCode);
	sBuf[5]		=   CUdfGlobalFUNC::ByteToBCD(packetFrame->_addr_info.A1.RegionCode);
	sBuf[6]		=  (BYTE)(packetFrame->_addr_info.A2 & 0x000000ff);
	sBuf[7]		=  (BYTE)((packetFrame->_addr_info.A2 & 0x0000ff00)>>8);
	sBuf[8]		=  (BYTE)(((packetFrame->_addr_info.A2 & 0x00ff0000)>>16) |  packetFrame->_addr_info.A3<<5);
	//AFN
	sBuf[9]		=   packetFrame->_asdu._AFN;

	for (int i= 0; i< dataSize; i++)
	{
		sBuf[10 + i] = packetFrame->_asdu.data[i];
	}
	sBuf[10 + dataSize] = CUdfGlobalFUNC::do_crc76520(&sBuf[3],sBuf[1]);
	sBuf[11 + dataSize] = 0x16;

	len = sBuf[1] + 5;
}

//����ط������б�
//������������
void CSysDataTrans::ClearReSendObjectList()
{
	m_Mutex_ReSend.Lock();
	while (!m_lstReSendObjects.IsEmpty())
	{
		RESEND_Object* pRESEND_Obj = m_lstReSendObjects.RemoveHead();
		if (pRESEND_Obj)
		{
			delete pRESEND_Obj;
			pRESEND_Obj = NULL;
		}
	}
	m_Mutex_ReSend.Unlock();
}

//���ɱ������ݵĻ�����Ϣ
void CSysDataTrans::BuildBasicDataInfo(COneData_INFO* pData,CPacketFrameFormat* packetFrame)      
{
	if(pData != NULL && 
		packetFrame != NULL)
	{
		pData->mProvinceCode	= packetFrame->_addr_info.A1.ProvinceCode;
		pData->mRegionCode		= packetFrame->_addr_info.A1.RegionCode;
		if (packetFrame->m_pStation != NULL)
			pData->mCountyCode  = packetFrame->m_pStation->GetCountyCode();

		pData->mA2 = packetFrame->_addr_info.A2;
	}
}

//���һ����Ҫ�����������Ϣ
void CSysDataTrans::InsertOneSaveData(COneData_INFO* pData)	
{
	if (!pData) return;

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	if (pApp && pApp->m_pRtuDatasave != NULL)
		pApp->m_pRtuDatasave->InsertOneData(pData);
}

//��ȡң���ն����Բ�������
//�����Բ��������Ͽ��Խ�����Ӧ�ĵ������Թ���
CRtuPropertyDlg* CSysDataTrans::GetRtuPropertyDlg()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	if (pMainFrame)
		return pMainFrame->GetRtuPropertyDlg();
	return NULL;
}

//���������Ϣ��������ʾ����
void CSysDataTrans::AddDataMessage(CRtuStation* pRtu,COneData_INFO* pData,BYTE nType/* =/* =AUTO_DATA */)
{
	if (!pData || !pRtu) return;

	CString strMessage = "",strTemp="";
	COneData_INFO::_Data_Type dataType = pData->mDataType;
	if (dataType == COneData_INFO::WATER_L_DATA)			//ˮλ��Ϣ
	{
		if (nType== AUTO_DATA)
			strTemp.Format("����:ˮλ�Ա�ʵʱ����\r\n");
		else if (nType == MANU_DATA)
			strTemp.Format("����:ˮλ�����˹�����\r\n");

		strMessage += strTemp;


		strTemp.Format("����ʱ��:%s\r\n",pData->Get_CollectTime_String());
		strMessage += strTemp;


		int Index = 1;
		POSITION rPos = pData->m_lstWaterLevel.GetHeadPosition();
		while (rPos != NULL)
		{
			CWaterLevel_PAR_PER* pLevel = pData->m_lstWaterLevel.GetNext(rPos);
			if (pLevel)
			{
				//strTemp.Format("ˮλ�Ǳ�:%d,��ǰֵ:%.2fm\r\n",Index,pLevel->GetRealValue());
				strTemp.Format("�� %2d ��ˮλ�Ǳ�,��ǰֵ:%.2fm\r\n",Index,pLevel->GetRealValue());
				strMessage += strTemp;
				Index ++;
			}
		}
	}
	else if (dataType == COneData_INFO::WATER_P_DATA)		//ˮѹ��Ϣ
	{
		if (nType == AUTO_DATA)
			strTemp.Format("����:ˮѹ�Ա�ʵʱ����\r\n");
		else if (nType == MANU_DATA)
			strTemp.Format("����:ˮѹ�����˹�����\r\n");

		strMessage += strTemp;


		strTemp.Format("����ʱ��:%s\r\n",pData->Get_CollectTime_String());
		strMessage += strTemp;

		int Index = 1;
		POSITION rPos = pData->m_lstWaterPress.GetHeadPosition();
		while (rPos != NULL)
		{
			CWaterPressure_PARA_PER* pPress = pData->m_lstWaterPress.GetNext(rPos);
			if (pPress)
			{
				//strTemp.Format("ˮѹ�Ǳ�:%d,��ǰֵ:%.2fkPA\r\n",Index,pPress->GetRealValue());
				strTemp.Format("�� %2d ��ˮѹ�Ǳ�,��ǰֵ:%.2fkPA\r\n",Index,pPress->GetRealValue());
				strMessage += strTemp;
				Index ++;
			}
		}
	}
	else if (dataType == COneData_INFO::WATER_Q_DATA)	 //ˮ����Ϣ
	{
		if (nType == AUTO_DATA)
			strTemp.Format("����:ˮ���Ա�ʵʱ����\r\n");
		else if (nType == MANU_DATA)
			strTemp.Format("����:ˮ�ʲ����˹�����\r\n");

		strMessage += strTemp;


		strTemp.Format("����ʱ��:%s\r\n",pData->Get_CollectTime_String());
		strMessage += strTemp;

		POSITION rPos = pData->m_lstWaterQuality.GetHeadPosition();
		while (rPos !=NULL)
		{
			CWaterQuality_PARA_PER* pQuality = pData->m_lstWaterQuality.GetNext(rPos);

			strTemp.Format("%s,��ǰֵ:%.*f %s\r\n",
				pQuality->m_strName,
				pQuality->m_PointBits,
				pQuality->m_CurValue,
				pQuality->m_strUnit);

			strMessage += strTemp;
		}
	}
	else if (dataType == COneData_INFO::FLOW_V_DATA)	//������Ϣ
	{
		if (nType == AUTO_DATA)
			strTemp.Format("����:�����Ա�ʵʱ����\r\n");
		else if (nType == MANU_DATA)
			strTemp.Format("����:���������˹�����\r\n");

		strMessage += strTemp;


		strTemp.Format("����ʱ��:%s\r\n",pData->Get_CollectTime_String());
		strMessage += strTemp;

		int Index = 1;
		POSITION rPos = pData->m_lstFlow.GetHeadPosition();
		while (rPos != NULL)
		{
			CFlowInitValume_PARA_PER* pFlow = pData->m_lstFlow.GetNext(rPos);
			if (pFlow)
			{
				//2023-5-26�޸�
				double leijivalue = (double)pFlow->GetCalTakeValume() / 10.0;
				double sunshivalue =(double)pFlow->GetInsFlowValume()/1000000.0;
				//strTemp.Format("��������:%d,�ۼ�ȡˮ��:%d������,˲ʱ����:%d ������/h\r\n",Index,pFlow->GetCalTakeValume(),pFlow->GetInsFlowValume());
				strTemp.Format("�� %2d ��������,�ۼ�ȡˮ��:%.3f������,˲ʱ����:%.3f ������/��\r\n",Index,leijivalue,sunshivalue);
				strMessage += strTemp;

				Index ++;
			}
		}
	}
	else if (dataType == COneData_INFO::RAIN_DATA)	//������Ϣ
	{
		if (nType == AUTO_DATA)
			strTemp.Format("����:���������Ա�����\r\n");
		else if (nType == MANU_DATA)
			strTemp.Format("����:���������˹�����\r\n");
		strMessage += strTemp;

		/*
		if(pRtu->GetAppProVer() == APP_PROTOCOL_V1_0)
		strTemp.Format("����ʱ��: %d�� %02d:%02d:%02d(delay:%d)\r\n",
		pData->mTP.m_day,
		pData->mTP.m_hour,
		pData->mTP.m_minute,
		pData->mTP.m_second,
		pData->mTP.m_adelay);
		else 
		*/
		strTemp.Format("����ʱ��:%s\r\n",pData->Get_CollectTime_String());
		strMessage += strTemp;


		int Index = 1;
		POSITION rPos = pData->m_lstRain.GetHeadPosition();
		while (rPos)
		{
			CRainAlert_PARA_PER* pRain = pData->m_lstRain.GetNext(rPos);
			if (pRain)
			{
				CString strValue = "";
				CString strType = "";

				if (nType  == AUTO_DATA)
				{
					int type = pRain->GetDataType();
					double fvalue = pRain->GetRealTimeValue();
					double halfHValue = pRain->GetHalfHourValue();

					if (type == 0)
						strType = "��ʱ����";
					else if (type == 0x01)
						strType = "��ʼ����";
					else if (type == 0x02)
						strType = "����ֵ����";
					else if (type == 0x03)
						strType = "�ӱ�";//"��/�������";
					else if (type == 0x04)
						strType = "��ͣ,�������ۼ�";

					if (pRtu->GetAppProVer() == APP_PROTOCOL_V1_0)
					{
						//strValue.Format("�� %2d ��������,��������:%s,ֵ:%.1f mm\r\n",Index,strType,fvalue);
						strValue.Format("�� %2d ��������,%s,ֵ:%.1f mm\r\n",Index,strType,fvalue);
					}
					else if (pRtu->GetAppProVer() == APP_PROTOCOL_V2_0)
					{
						//strValue.Format("�� %2d ��������,��������:%s,ֵ:%.1f mm,�ն��ۼ�ֵ: %.1f mm\r\n",Index,strType,fvalue,pRain->GetTotalValue());
						strValue.Format("�� %2d ��������,%s,�����ۼ�ֵ:%.1f mm,��Сʱ�ۼ�ֵ:%.1f mm,���ۼ�ֵ: %.1f mm\r\n",
							Index,strType,
							fvalue,halfHValue,pRain->GetTotalValue());
					}
				}
				else if (nType == MANU_DATA)
				{
					strValue.Format("�� %2d ��������,�˹�����:%.1f mm\r\n",
						Index,pRain->GetRealTimeValue());
				}


				strMessage += strValue;

				Index ++;
			}
		}
	}
	else if (dataType == COneData_INFO::ANALOG_DATA)	//ģ������Ϣ
	{
		if (nType == AUTO_DATA)
			strTemp.Format("����:ģ���������Ա�����\r\n");
		else if (nType == MANU_DATA)
			strTemp.Format("����:ģ���������˹�����\r\n");
		strMessage += strTemp;

		/*
		if(pRtu->GetAppProVer() == APP_PROTOCOL_V1_0)
		strTemp.Format("����ʱ��: %d�� %02d:%02d:%02d(delay:%d)\r\n",
		pData->mTP.m_day,
		pData->mTP.m_hour,
		pData->mTP.m_minute,
		pData->mTP.m_second,
		pData->mTP.m_adelay);
		else 
		*/
		strTemp.Format("����ʱ��:%s\r\n",pData->Get_CollectTime_String());
		strMessage += strTemp;


		int Index = 1;
		POSITION rPos = pData->m_lstAnalog.GetHeadPosition();
		while (rPos)
		{
			CAnalogLimit_PARA_PER* pAnalog = pData->m_lstAnalog.GetNext(rPos);
			if (pAnalog)
			{				
				strTemp.Format("�� %2d ��ģ����,��ǰģ����ֵ:%5d\r\n",Index,pAnalog->GetRealTimeValue());
				strMessage += strTemp;
				Index ++;
			}
		}
	}
	else if (dataType == COneData_INFO::PULSE_DATA)
	{
		if (nType == AUTO_DATA)
			strTemp.Format("����:����������Ա�����\r\n");
		else if (nType == MANU_DATA)
			strTemp.Format("����:����������˹�����\r\n");
		strMessage += strTemp;

		/*
		if(pRtu->GetAppProVer() == APP_PROTOCOL_V1_0)
		strTemp.Format("����ʱ��: %d�� %02d:%02d:%02d(delay:%d)\r\n",
		pData->mTP.m_day,
		pData->mTP.m_hour,
		pData->mTP.m_minute,
		pData->mTP.m_second,
		pData->mTP.m_adelay);
		else */
		strTemp.Format("����ʱ��:%s\r\n",pData->Get_CollectTime_String());
		strMessage += strTemp;


		POSITION rPos = pData->m_lstPulses.GetHeadPosition();
		while (rPos)
		{			
			CPulsePara_PER* pPulse = pData->m_lstPulses.GetNext(rPos);

			if (pPulse)
			{		
				/*
				strTemp.Format("��ȼǵ�ַ:%d,�����й�:%.2f,�����޹�:%.2f,�����й�:%.2f,�����޹�:%.2f\r\n",
				pPulse->GetCode(),
				pPulse->GetValue1(),
				pPulse->GetValue2(),
				pPulse->GetValue3(),
				pPulse->GetValue4());				
				*/

				strTemp.Format("��ȼǵ�ַ:%s,",pPulse->GetCodeString());
				strMessage += strTemp;

				strTemp.Format("�����й�:%.2f kWh,",pPulse->GetValue1());
				strMessage += strTemp;

				strTemp.Format("�����޹�:%.2f kWh,",pPulse->GetValue2());
				strMessage += strTemp;

				strTemp.Format("�����й�:%.2f kWh,",pPulse->GetValue3());
				strMessage += strTemp;

				strTemp.Format("�����޹�:%.2f kWh\r\n",pPulse->GetValue4());
				strMessage += strTemp;

			}
		}
	}
	else if (dataType == COneData_INFO::ALARM_RECORD)	//�����������
	{
		int nAlarmType = pData->mAlarmType;

		strTemp.Format("����:����Ա���������\r\n");
		strMessage += strTemp;

		/*
		if(pRtu->GetAppProVer() == APP_PROTOCOL_V1_0)
		strTemp.Format("����ʱ��: %d�� %02d:%02d:%02d(delay:%d)\r\n",
		pData->mTP.m_day,
		pData->mTP.m_hour,
		pData->mTP.m_minute,
		pData->mTP.m_second,
		pData->mTP.m_adelay);
		else 
		*/
		strTemp.Format("����ʱ��:%s\r\n",pData->Get_CollectTime_String());
		strMessage += strTemp;


		if (nAlarmType >= 0 && nAlarmType < 18)
		{
			strTemp.Format("��������:%s\r\n",_Alert_Options_Name[nAlarmType]);
			strMessage += strTemp;
		}

		if (nAlarmType == (int)COneData_INFO::ALARM_WATERLEVEL)		//����������ˮλ��Ϣ
		{
			int Index = 1;
			POSITION rPos = pData->m_lstWaterLevel.GetHeadPosition();
			while (rPos != NULL)
			{
				CWaterLevel_PAR_PER* pLevel = pData->m_lstWaterLevel.GetNext(rPos);
				if (pLevel)
				{
					//strTemp.Format("ˮλ�Ǳ�:%d,��ǰֵ:%.2fm\r\n",Index,pLevel->GetRealValue());
					strTemp.Format("�� %2d ��ˮλ��,��ǰֵ:%.2fm\r\n",Index,pLevel->GetRealValue());
					strMessage += strTemp;
					Index ++;
				}
			}
		}
		else if (nAlarmType == (int)COneData_INFO::ALARM_WATERPRESS)	//����������ˮѹ��Ϣ
		{
			int Index = 1;
			POSITION rPos = pData->m_lstWaterPress.GetHeadPosition();
			while (rPos != NULL)
			{
				CWaterPressure_PARA_PER* pPress = pData->m_lstWaterPress.GetNext(rPos);
				if (pPress)
				{
					//strTemp.Format("ˮѹ�Ǳ�:%d,��ǰֵ:%.2fkPA\r\n",Index,pPress->GetRealValue());
					strTemp.Format("�� %2d ��ˮѹ��,��ǰֵ:%.2fkPA\r\n",Index,pPress->GetRealValue());
					strMessage += strTemp;
					Index ++;
				}
			}
		}
		else if (nAlarmType == (int)COneData_INFO::ALARM_WATERQUALITY)	//����������ˮ����Ϣ
		{
			POSITION rPos = pData->m_lstWaterQuality.GetHeadPosition();
			while (rPos !=NULL)
			{
				CWaterQuality_PARA_PER* pQuality = pData->m_lstWaterQuality.GetNext(rPos);

				strTemp.Format("%s,��ǰֵ:%.*f %s\r\n",
					pQuality->m_strName,
					pQuality->m_PointBits,
					pQuality->m_CurValue,
					pQuality->m_strUnit);

				strMessage += strTemp;
			}
		}
		else if (nAlarmType == (int)COneData_INFO::ALARM_FLOW)		//����������������Ϣ
		{
			int Index = 1;
			POSITION rPos = pData->m_lstFlow.GetHeadPosition();
			while (rPos != NULL)
			{
				CFlowInitValume_PARA_PER* pFlow = pData->m_lstFlow.GetNext(rPos);
				if (pFlow)
				{
					//strTemp.Format("��������:%d,�ۼ�ȡˮ��:%d������,˲ʱ����:%d ������/h\r\n",Index,pFlow->GetCalTakeValume(),pFlow->GetInsFlowValume());
					strTemp.Format("�� %2d ��������,�ۼ�ȡˮ��:%d ������,˲ʱ����:%d ������/h\r\n",Index,pFlow->GetCalTakeValume(),pFlow->GetInsFlowValume());
					strMessage += strTemp;

					Index ++;
				}
			}
		}
		else if (nAlarmType ==(int)COneData_INFO::ALARM_RAIN)	//����������������Ϣ
		{
			int Index = 1;
			POSITION rPos = pData->m_lstRain.GetHeadPosition();
			while (rPos)
			{
				CRainAlert_PARA_PER* pRain = pData->m_lstRain.GetNext(rPos);
				if (pRain)
				{

					/*
					strTemp.Format("������ %d,��������ֵ:%.1f mm,��λʱ��:%4d min\r\n",
					Index,
					pRain->GetRealTimeValue(),
					pRain->GetPerTime());
					*/
					strTemp.Format("�� %2d ��������,��������ֵ:%.1f mm,��λʱ��:%4d min\r\n",
						Index,
						pRain->GetRealTimeValue(),
						pRain->GetPerTime());
					strMessage += strTemp;

					Index ++;
				}
			}
		}
		else if (nAlarmType == (int)COneData_INFO::ALARM_ANALOG) //����������ģ������Ϣ
		{
			int Index = 1;
			POSITION rPos = pData->m_lstAnalog.GetHeadPosition();
			while (rPos)
			{
				CAnalogLimit_PARA_PER* pAnalog = pData->m_lstAnalog.GetNext(rPos);
				if (pAnalog)
				{

					//strTemp.Format("ģ���� %d,��ǰģ����ֵ:%5d\r\n",Index,pAnalog->GetRealTimeValue());
					strTemp.Format("�� %2d ��ģ����,��ǰģ����ֵ:%5d\r\n",Index,pAnalog->GetRealTimeValue());
					strMessage += strTemp;

					Index ++;
				}
			}
		}

	}
	else if (dataType == COneData_INFO::PEACE_REPORT)
	{
		strMessage = pData->sPeaceReport;
	}

	else if (dataType == COneData_INFO::DISPLACE_DATA)	//���λ��
	{
		if (nType == AUTO_DATA)
			strTemp.Format("����:���λ���������Ա�����\r\n");
		else if (nType == MANU_DATA)
			strTemp.Format("����:���λ���������˹�����\r\n");
		strMessage += strTemp;

		strTemp.Format("����ʱ��:%s\r\n",pData->Get_CollectTime_String());
		strMessage += strTemp;

		int Index = 1;
		POSITION rPos = pData->m_lstDisplace.GetHeadPosition();
		while (rPos)
		{
			CDisplace_PARA_PER* pDisplace = pData->m_lstDisplace.GetNext(rPos);
			if (pDisplace)
			{				
				strTemp.Format("�� %2d �����λ����,��ǰ���λ����ֵA��:%5f mm��B��:%5f mm\r\n",Index,pDisplace->GetRealTimeXValue(),pDisplace->GetRealTimeYValue());
				strMessage += strTemp;
				Index ++;
			}
		}
	}
	else if (dataType == COneData_INFO::TEMPERA_DATA)	//�¶�
	{
		if (nType == AUTO_DATA)
			strTemp.Format("����:�¶Ȳ����Ա�����\r\n");
		else if (nType == MANU_DATA)
			strTemp.Format("����:�¶Ȳ����˹�����\r\n");
		strMessage += strTemp;
		strTemp.Format("����ʱ��:%s\r\n",pData->Get_CollectTime_String());
		strMessage += strTemp;


		int Index = 1;
		POSITION rPos = pData->m_lstTempera.GetHeadPosition();
		while (rPos)
		{
			CTempera_PARA_PER* pTempera = pData->m_lstTempera.GetNext(rPos);
			if (pTempera)
			{				
				strTemp.Format("�� %2d ���¶�,��ǰ�¶�ֵ:%.1f ��\r\n",Index,pTempera->GetRealTimeValue());
				strMessage += strTemp;
				Index ++;
			}
		}
	}
	if (!strMessage.IsEmpty())
	{
		pRtu->InsertDataMessage(strMessage);
		AddDataMessage(pRtu,strMessage,(PSYSTEMTIME)NULL);
	}
}


//���������Ϣ��ң���ն˵��Ա�������ʾ����
void CSysDataTrans::AddDataMessage(CRtuStation* pRTU,CString strDataMessage,PSYSTEMTIME pTime/* =NULL */)
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

//�����־��Ϣ��ң���ն˵����Բ�������
void CSysDataTrans::AddLogMessage(CRtuStation* pRTU,CString strLogMessage, _Log_level_type leveType /* =Normal_Log_Message */, PSYSTEMTIME pTime /* = NULL */)
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

//����ң���ն����Դ��ڵ���Ϣ
void CSysDataTrans::UpdateAttribFrame(CRtuStation* pRTU,int attrType)
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
	case rtu_attr_workmode_call:
	case rtu_attr_workmode_set:
	case rtu_attr_Connect_check:
	case rtu_attr_datetime_set:
	case rtu_attr_datetime_call:
		{
			if (pWnd->GetTab_AttrBasicInfo() != NULL && 
				pWnd->GetTab_AttrBasicInfo()->GetSafeHwnd())
			{
				pWnd->GetTab_AttrBasicInfo()->UpdateDlg(attrType,TRUE);
				//pWnd->GetTab_AttrBasicInfo()->Update();//kill by yhp temp 
			}
		}
		break;
	case rtu_attr_ycquerypara_call:
	case rtu_attr_ycquerypara_set:
		{
			if (pWnd->GetTab_AttrWorkMode() != NULL && 
				pWnd->GetTab_AttrWorkMode()->GetSafeHwnd())
			{
				pWnd->GetTab_AttrWorkMode()->UpdateDlg(attrType,TRUE);
				pWnd->GetTab_AttrWorkMode()->Update();
			}
		}
		break;
	case  rtu_attr_autoRpPara_call:
	case  rtu_attr_atuoRpPara_set:
		{
			if (pWnd->GetTab_AttrAutoRpSet() != NULL && 
				pWnd->GetTab_AttrAutoRpSet()->GetSafeHwnd())
			{
				pWnd->GetTab_AttrAutoRpSet()->UpdateDlg(attrType,TRUE);
				pWnd->GetTab_AttrAutoRpSet()->Update();
			}
		}
		break;
	case rtu_attr_rainalert_set:
	case rtu_attr_rainalert_call:
	case rtu_attr_rain_data_call:
		{
			if (pWnd->GetTab_AttrRainValume() != NULL && 
				pWnd->GetTab_AttrRainValume()->GetSafeHwnd())
			{
				pWnd->GetTab_AttrRainValume()->UpdateDlg(attrType,TRUE);
				pWnd->GetTab_AttrRainValume()->Update();
			}
		}
		break;
	case rtu_attr_analogminmax_call:
	case rtu_attr_analogminmax_set:
	case rtu_attr_analog_data_call:
		{
			if (pWnd->GetTab_AttrAlertAnalog() != NULL && 
				pWnd->GetTab_AttrAlertAnalog()->GetSafeHwnd())
			{
				pWnd->GetTab_AttrAlertAnalog()->UpdateDlg(attrType,TRUE);
				pWnd->GetTab_AttrAlertAnalog()->Update();
			}
		}
		break;
	case rtu_attr_displaceminmax_call:
	case rtu_attr_displaceminmax_set:
	case rtu_attr_displace_data_call:
		{
			if (pWnd->GetTab_AttrDisplacement() != NULL && 
				pWnd->GetTab_AttrDisplacement()->GetSafeHwnd())
			{
				pWnd->GetTab_AttrDisplacement()->UpdateDlg(attrType,TRUE);
				pWnd->GetTab_AttrDisplacement()->Update();
			}
		}
		break;
	case rtu_attr_temperaminmax_call:
	case rtu_attr_temperaminmax_set:
	case rtu_attr_tempera_data_call:
		{
			if (pWnd->GetTab_AttrTemperature() != NULL && 
				pWnd->GetTab_AttrTemperature()->GetSafeHwnd())
			{
				pWnd->GetTab_AttrTemperature()->UpdateDlg(attrType,TRUE);
				pWnd->GetTab_AttrTemperature()->Update();
			}
		}
		break;
	case rtu_attr_water_recharge_call:
	case rtu_attr_water_alert_call:
		{
			if (pWnd->GetTab_AttrWaterValume() != NULL && 
				pWnd->GetTab_AttrWaterValume()->GetSafeHwnd())
			{
				pWnd->GetTab_AttrWaterValume()->UpdateDlg(attrType,TRUE);
				pWnd->GetTab_AttrWaterValume()->Update();
			}
		}
		break;
	case rtu_attr_waterlevel_call:
	case rtu_attr_waterlevel_data_call:
		{
			if (pWnd->GetTab_AttrWaterLevel() != NULL && 
				pWnd->GetTab_AttrWaterLevel()->GetSafeHwnd())
			{
				pWnd->GetTab_AttrWaterLevel()->UpdateDlg(attrType,TRUE);
				pWnd->GetTab_AttrWaterLevel()->Update();
			}
		}
		break;
	case rtu_attr_waterpress_call:
	case rtu_attr_waterpress_data_call:
		{
			if (pWnd->GetTab_AttrWaterPress() != NULL && 
				pWnd->GetTab_AttrWaterPress()->GetSafeHwnd())
			{
				pWnd->GetTab_AttrWaterPress()->UpdateDlg(attrType,TRUE);
				pWnd->GetTab_AttrWaterPress()->Update();
			}
		}
		break;
	case  rtu_attr_flowpara_set:
	case rtu_attr_flowdata_call:
		{
			if (pWnd->GetTab_AttrFlow() != NULL && 
				pWnd->GetTab_AttrFlow()->GetSafeHwnd())
			{
				pWnd->GetTab_AttrFlow()->UpdateDlg(attrType,TRUE);
				pWnd->GetTab_AttrFlow()->Update();
			}
		}
		break;
	case rtu_attr_waterquality_data_call:
	case rtu_attr_waterquality_down_call:
	case rtu_attr_waterquality_up_call:
		{
			if (pWnd->GetTab_AttrWaterQuality() != NULL && 
				pWnd->GetTab_AttrWaterQuality()->GetSafeHwnd())
			{
				pWnd->GetTab_AttrWaterQuality()->UpdateDlg(attrType,TRUE);
			}
		}
		break;
	case rtu_attr_alarmstate_call:
	case rtu_attr_exalarmstate_call:
		{
			if(pWnd->GetTab_AttrAlarmState() != NULL && 
				pWnd->GetTab_AttrAlarmState()->GetSafeHwnd())
				pWnd->GetTab_AttrAlarmState()->UpdateDlg(attrType,TRUE);
		}
		break;
	case rtu_attr_yk_operate:
		{
			if (pWnd->GetTab_AttrControlCmd() != NULL && 
				pWnd->GetTab_AttrControlCmd()->GetSafeHwnd())
				pWnd->GetTab_AttrControlCmd()->UpdateDlg();
		}
		break;
	case rtu_attr_eventrecord_call:
		{
			if (pWnd->GetTab_AttrQueryEvent() != NULL && 
				pWnd->GetTab_AttrQueryEvent()->GetSafeHwnd())
				pWnd->GetTab_AttrQueryEvent()->UpdateDlg(attrType,TRUE);
		}
		break;
	case  rtu_attr_pulsepara_call:
		{
			if (pWnd->GetTab_AttrPulse() != NULL && 
				pWnd->GetTab_AttrPulse()->GetSafeHwnd())
				pWnd->GetTab_AttrPulse()->UpdateDlg(attrType,TRUE);
		}
		break;
	}
}

//����ң���ն˵�ʱ����Ϣ
void CSysDataTrans::UpdateRtuClockString(CRtuStation* pRTU,CString strClock)
{
	CRtuPropertyDlg* pWnd = GetRtuPropertyDlg();
	if (pWnd == NULL  ||
		pWnd->GetRtuStation() == NULL ||
		pWnd->GetRtuStation() != pRTU)
		return;

	if (strClock.IsEmpty()) return;
	if (pWnd->GetTab_AttrBasicInfo() != NULL &&
		pWnd->GetTab_AttrBasicInfo()->GetSafeHwnd())
	{
		pWnd->GetTab_AttrBasicInfo()->SetDateTime(strClock);
	}
}

//����ң���ն˵���ʷ�ռ�¼��Ϣ
void CSysDataTrans::UpdateRtuDayHistoryDlg(CRtuStation* pRTU,BOOL blSucc,CString sDayHisInfo)
{
	if (pRTU == NULL) return;

	CRtuPropertyDlg* pWnd = GetRtuPropertyDlg();
	if (pWnd->GetRtuStation() == NULL ||
		pWnd->GetRtuStation() != pRTU)
		return;

	if (pWnd->GetTab_AttrDayHisQuery() != NULL && 
		pWnd->GetTab_AttrDayHisQuery()->GetSafeHwnd())
	{
		pWnd->GetTab_AttrDayHisQuery()->UpdateDlg(blSucc,sDayHisInfo);
	}
}


//V2.0 �汾�������֦����Ŀ�޸���չ��
void CSysDataTrans::IEC_waterdata_US_68H( CPacketFrameFormat* waterFrame )
{
	if (!waterFrame) return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;
	if (waterFrame->_asdu._AFN != 0x68) return;
	if (waterFrame->_asdu.data.size() < 3) return;


	DeleteOneReSendObject(waterFrame->m_pStation->GetQueryRealDataCmd(),waterFrame->_asdu._AFN,waterFrame->_addr_info);

	BYTE QueryCmd = waterFrame->m_pStation->GetQueryRealDataCmd();
	if (QueryCmd == C_CMD_11_RAINPARA)  //����
	{
		if (waterFrame->m_pStation)
		{
			int nRainCount = (waterFrame->_asdu.data.size() - 3) / 6;
			if ((waterFrame->_asdu.data.size() - 3) % 6 != 0)
				return;

			int nAdded = nRainCount - waterFrame->m_pStation->GetRainAlertParasList()->GetCount();

			if (nAdded > 0 )
			{
				for (int i = 0; i <nAdded; i ++)
				{
					CRainAlert_PARA_PER* pRain_PARA = new CRainAlert_PARA_PER();
					waterFrame->m_pStation->GetRainAlertParasList()->AddTail(pRain_PARA);
				}
			}

			for (int i = 0; i < nRainCount; i ++)
			{
				float RainValue =	CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i*6]) * 0.1 +
					CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[1+i*6]) *10;
				float totalValue =  CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[2+i*6]) * 0.1 +
					CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[3+i*6]) * 10 +
					CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[4+i*6]) * 1000 +
					CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[5+i*6]) * 100000 ;

				CRainAlert_PARA_PER* pRain_PARA = waterFrame->m_pStation->GetRainAlartPara(i);
				if (pRain_PARA)
				{
					pRain_PARA->SetRealTimeValue(RainValue);
					pRain_PARA->SetTotalValue(totalValue);
				}
			}

			waterFrame->m_pStation->GetRtuAlarm_State()->SetAlarmStateByte(waterFrame->_asdu.data[nRainCount * 6]);
			waterFrame->m_pStation->GetRtuExAlarm_State()->SetExAlarmStateByte(waterFrame->_asdu.data[nRainCount * 6 +1]);
			waterFrame->m_pStation->GetRtuTerminal_State()->SetTerminalStateByte(waterFrame->_asdu.data[nRainCount * 6 + 2]);

			UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_rain_data_call);
		}
	}
	else if (QueryCmd == C_CMD_12_MONIPARA) //ģ����
	{
		if (waterFrame->m_pStation)
		{
			int AnalogCount = (waterFrame->_asdu.data.size() - 3) / 3;
			if ((waterFrame->_asdu.data.size() - 3) % 3 != 0 )
				return;

			int nAdded = AnalogCount - waterFrame->m_pStation->GetAnalogLimitParasList()->GetCount();
			if (nAdded > 0)
			{
				for ( int i = 0; i < nAdded; i ++)
				{
					CAnalogLimit_PARA_PER* pAnalog_PARA = new CAnalogLimit_PARA_PER();
					waterFrame->m_pStation->GetAnalogLimitParasList()->AddTail(pAnalog_PARA);
				}
			}

			for (int i = 0 ;i < AnalogCount ; i ++)
			{
				DWORD RealValue =	CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i*3]) +
					CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[1+i*3]) * 100 +
					CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[2+i*3] & 0x0F) * 10000;

				CAnalogLimit_PARA_PER* pAnalog_PARA = waterFrame->m_pStation->GetAnalogLimitPara(i);
				if (pAnalog_PARA)
					pAnalog_PARA->SetRealTimeValue(RealValue);
			}

			waterFrame->m_pStation->GetRtuAlarm_State()->SetAlarmStateByte(waterFrame->_asdu.data[AnalogCount * 3]);
			waterFrame->m_pStation->GetRtuExAlarm_State()->SetExAlarmStateByte(waterFrame->_asdu.data[AnalogCount * 3 + 1]);
			waterFrame->m_pStation->GetRtuTerminal_State()->SetTerminalStateByte(waterFrame->_asdu.data[AnalogCount * 3 + 2]);

			UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_analog_data_call);
		}
	}
	else if (QueryCmd == C_CMD_13_PULSEPARA)   //���
	{
		if (waterFrame->m_pStation)
		{
			int pulseCunt = (waterFrame->_asdu.data.size() - 3) / 22;
			if ((waterFrame->_asdu.data.size() - 3) % 22 != 0)
				return;

			for (int i = 0; i < pulseCunt; i ++)
			{
				UINT64 code = 0;
				code = (UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 0] ) +
					(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 1] ) * (UINT64) 100 +
					(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 2] ) * (UINT64) 10000 +
					(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 3] ) * (UINT64) 1000000 +
					(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 4] ) * (UINT64) 100000000 +
					(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 5] ) * (UINT64) 10000000000 ;

				CPulsePara_PER *pPulse = waterFrame->m_pStation->GetPulseParaObject(code);

				if (pPulse == NULL)
				{
					pPulse = new CPulsePara_PER(code);
					waterFrame->m_pStation->InsertPulseParaObject(pPulse);					
				}

				double dbvalue = 0.0;
				dbvalue =	(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 6] ) * (double) 0.01 +
					(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 7] ) * (double) 1 +
					(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 8] ) * (double) 100 +
					(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 9] ) * (double) 10000;
				pPulse->SetValue1(dbvalue);

				dbvalue =	(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 10] ) * (double) 0.01 +
					(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 11] ) * (double) 1 +
					(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 12] ) * (double) 100 +
					(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 13] ) * (double) 10000;
				pPulse->SetValue2(dbvalue);

				dbvalue =	(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 14] ) * (double) 0.01 +
					(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 15] ) * (double) 1 +
					(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 16] ) * (double) 100 +
					(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 17] ) * (double) 10000;
				pPulse->SetValue3(dbvalue);

				dbvalue =	(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 18] ) * (double) 0.01 +
					(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 19] ) * (double) 1 +
					(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 20] ) * (double) 100 +
					(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 21] ) * (double) 10000;
				pPulse->SetValue4(dbvalue);

			}

			waterFrame->m_pStation->GetRtuAlarm_State()->SetAlarmStateByte(waterFrame->_asdu.data[pulseCunt * 22]);
			waterFrame->m_pStation->GetRtuExAlarm_State()->SetExAlarmStateByte(waterFrame->_asdu.data[pulseCunt * 22 + 1]);
			waterFrame->m_pStation->GetRtuTerminal_State()->SetTerminalStateByte(waterFrame->_asdu.data[pulseCunt * 22 + 2]);

			UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_pulsepara_call);
		}
	}
	else if (QueryCmd == C_CMD_14_DISPLACEMENT) //���λ����
	{
		if (waterFrame->m_pStation)
		{
			int DisplaceCount = (waterFrame->_asdu.data.size() - 2) / 6;
			int nAdded = DisplaceCount - waterFrame->m_pStation->GetDisplaceLimitParasList()->GetCount();
			if (nAdded > 0)
			{
				for ( int i = 0; i < nAdded; i ++)
				{
					CDisplace_PARA_PER* pDisplace_PARA = new CDisplace_PARA_PER();
					waterFrame->m_pStation->GetDisplaceLimitParasList()->AddTail(pDisplace_PARA);
				}
			}

			for (int i = 0 ;i < DisplaceCount ; i ++)
			{
				DWORD XRealValue = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i*6]) +
					CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[1+i*6]) * 100 +
					CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[2+i*6]) * 10000;

				DWORD YRealValue = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[3+i*6]) +
					CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[4+i*6]) * 100 +
					CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[5+i*6]) * 10000;

				CDisplace_PARA_PER* pDisplace_PARA = waterFrame->m_pStation->GetDisplaceLimitPara(i);
				if (pDisplace_PARA)
				{
					pDisplace_PARA->SetRealTimeXValue(XRealValue);
					pDisplace_PARA->SetRealTimeYValue(YRealValue);
				}
			}

			waterFrame->m_pStation->GetRtuExAlarm_State()->SetExAlarmStateByte(waterFrame->_asdu.data[DisplaceCount * 6 ]);
			waterFrame->m_pStation->GetRtuTerminal_State()->SetTerminalStateByte(waterFrame->_asdu.data[DisplaceCount * 6 + 1]);

			UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_displace_data_call);
		}
	}
	else if (QueryCmd == C_CMD_15_TEMPERATURE) //�¶���
	{
		if (waterFrame->m_pStation)
		{
			int TemperaCount = (waterFrame->_asdu.data.size() - 2) / 2;
			int nAdded = TemperaCount - waterFrame->m_pStation->GetTemperaLimitParasList()->GetCount();
			if (nAdded > 0)
			{
				for ( int i = 0; i < nAdded; i ++)
				{
					CTempera_PARA_PER* pTempera_PARA = new CTempera_PARA_PER();
					waterFrame->m_pStation->GetTemperaLimitParasList()->AddTail(pTempera_PARA);
				}
			}

			for (int i = 0 ;i < TemperaCount ; i ++)
			{
				DWORD RealValue = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i*3]) +
					CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[1+i*3]) * 100;


				CTempera_PARA_PER* pTempera_PARA = waterFrame->m_pStation->GetTemperaLimitPara(i);
				if (pTempera_PARA)
					pTempera_PARA->SetRealTimeValue(RealValue/10.0);
			}

			waterFrame->m_pStation->GetRtuExAlarm_State()->SetExAlarmStateByte(waterFrame->_asdu.data[TemperaCount * 2 ]);
			waterFrame->m_pStation->GetRtuTerminal_State()->SetTerminalStateByte(waterFrame->_asdu.data[TemperaCount * 2 + 1]);

			UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_tempera_data_call);
		}
	}
}

void CSysDataTrans::IEC_waterdata_US_69H( CPacketFrameFormat* waterFrame )
{
	if (!waterFrame)  return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;
	if (waterFrame->_asdu._AFN != 0x69 ) return;

	DeleteOneReSendObject(C_CMD_05_MSUERSTATE,0x69,waterFrame->_addr_info);

	if (waterFrame->m_pStation)
	{
		if (waterFrame->_asdu.data.size() < 3) return;

		waterFrame->m_pStation->GetRtuAlarm_State()->SetAlarmStateByte(waterFrame->_asdu.data[0]);
		waterFrame->m_pStation->GetRtuExAlarm_State()->SetExAlarmStateByte(waterFrame->_asdu.data[1]);
		waterFrame->m_pStation->GetRtuTerminal_State()->SetTerminalStateByte(waterFrame->_asdu.data[2]);

		UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_exalarmstate_call);
	}
}

void CSysDataTrans::IEC_waterdata_US_85H( CPacketFrameFormat* waterFrame )
{
	if (!waterFrame)	return;
	if (waterFrame->ControlC._DIR != UP_ARROW)	return;
	if (waterFrame->_asdu._AFN != 0x85)	 return;
	if (waterFrame->_asdu.data.size() < 10 ) return;

	if (waterFrame->m_pStation != NULL)
		waterFrame->m_pStation->SetTimeEnable(false);

	bool blSetTimeEnable = false;
	if (waterFrame->GetRepeatType() == false)
	{
		BYTE cp56[7];
		memset(cp56,0,sizeof(cp56));
		int _tpPos = (int)waterFrame->_asdu.data.size() - 7;
		for (int i = 0 ;i < 7; i ++)
		{
			cp56[i] = waterFrame->_asdu.data[_tpPos + i];
		}

		SYSTEMTIME tempcp56;
		GetLocalTime(&tempcp56);
		WORD curYear = tempcp56.wYear;

		tempcp56.wMilliseconds	= (cp56[0] | cp56[1]<<8) % 1000;
		tempcp56.wSecond		= (cp56[0] | cp56[1] <<8) /1000;
		tempcp56.wMinute		= cp56[2];
		tempcp56.wHour			= cp56[3];
		tempcp56.wDay			= cp56[4];
		tempcp56.wMonth			= cp56[5];
		tempcp56.wYear			= 2000 + cp56[6];

		//�޸�����cp56ʱ����������
		bool  blCp56Right = false;
		if (tempcp56.wMonth >0 && tempcp56.wMonth <= 12 &&
			tempcp56.wDay >0 && tempcp56.wDay <= 31 &&
			tempcp56.wHour>=0 && tempcp56.wHour < 24 &&
			tempcp56.wMinute >=0 && tempcp56.wMinute < 60 &&
			tempcp56.wSecond >=0 && tempcp56.wSecond < 60 &&
			tempcp56.wMilliseconds >=0  && tempcp56.wMilliseconds < 1000 /*&&
																		 abs(tempcp56.wYear - curYear) < 2*/)
		{
			blCp56Right = true;
		}

		if (blCp56Right == true)
		{
			COneData_INFO* pSave_data = new COneData_INFO();
			BuildBasicDataInfo(pSave_data,waterFrame);

			//ʱ����Ϣ
			pSave_data->mCollectTime.wMilliseconds	= (cp56[0] | cp56[1]<<8) % 1000;
			pSave_data->mCollectTime.wSecond		= (cp56[0] | cp56[1] <<8) /1000;
			pSave_data->mCollectTime.wMinute		= cp56[2];
			pSave_data->mCollectTime.wHour			= cp56[3];
			pSave_data->mCollectTime.wDay			= cp56[4];
			pSave_data->mCollectTime.wMonth			= cp56[5];
			pSave_data->mCollectTime.wYear			= 2000 + cp56[6];


			pSave_data->SetErrorFlag(CUdfGlobalFUNC::CheckErrorData(&(pSave_data->mCollectTime)));


			if (waterFrame->ControlC._Command == 5/*C_CMD_06_WLPARA*/) //ˮλ
			{
				pSave_data->mDataType = COneData_INFO::WATER_L_DATA;

				int nLevelCount = (waterFrame->_asdu.data.size()- 10) / 3 ; //ˮλ�����

				for ( int i = 0 ; i < nLevelCount; i ++)
				{
					float RealV = ((float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 3 + 0]) * 0.01 +
						(float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 3 + 1]) +
						(float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 3 + 2]) * 100);

					//������ݴ洢
					CWaterLevel_PAR_PER* pLevel = new CWaterLevel_PAR_PER();
					pLevel->SetRealValue(RealV);
					pSave_data->m_lstWaterLevel.AddTail(pLevel);

				}
				if (waterFrame->m_pStation != NULL)
				{
					waterFrame->m_pStation->GetRtuAlarm_State()->SetAlarmStateByte(waterFrame->_asdu.data[nLevelCount * 3]);
					waterFrame->m_pStation->GetRtuExAlarm_State()->SetExAlarmStateByte(waterFrame->_asdu.data[nLevelCount * 3 + 1]);
					waterFrame->m_pStation->GetRtuTerminal_State()->SetTerminalStateByte(waterFrame->_asdu.data[nLevelCount* 3 + 2]);
				}

				blSetTimeEnable = true;
			}
			else if (waterFrame->ControlC._Command == 6/*C_CMD_07_FLOWPARA*/) //����
			{
				pSave_data->mDataType = COneData_INFO::FLOW_V_DATA;

				int nFlowCount = (waterFrame->_asdu.data.size() - 10)/ 6;   //������������

				for (int i = 0 ; i < nFlowCount ; i ++)
				{
					//�ۼ�ȡˮ��
					/*DWORD CalTakeWater = (DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 0]) +
						(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 1]) * 100 +
						(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 2]) * 10000 +
						(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 3]) * 1000000;*/
					DWORD s1 = (DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 0]);
					DWORD s2 = (DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 1]) * 100;
					DWORD s3 = (DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 2]) * 10000;
					DWORD s4 = (DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 3]) * 1000000;
					DWORD s5 = (DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 4]) * 100000000;
					DWORD CalTakeWater = s1 + s2 + s3 + s4 + s5;
					//˲ʱ����
					/*WORD InsFlow   =	 (WORD)(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 4])) +
						(WORD)(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 5])) * 100;*/
					//DWORD v = (WORD)(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 4]));
					DWORD v1 = (WORD)(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 5]));
					DWORD v2 = (WORD)(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 6])) * 100;
					DWORD v3 = (WORD)(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 7])) * 10000;
					WORD InsFlow = v1 + v2 + v3;
					//������ݴ洢
					CFlowInitValume_PARA_PER* pFlow = new CFlowInitValume_PARA_PER();

					flow1 = (double)CalTakeWater / 10.0;
					flow2 = (double)InsFlow / 1000.0;

					pFlow->SetCalTakeValume( CalTakeWater);
					pFlow->SetInsFlowValume(InsFlow);
					pSave_data->m_lstFlow.AddTail(pFlow);
				}

				if (waterFrame->m_pStation)
				{
					waterFrame->m_pStation->GetRtuAlarm_State()->SetAlarmStateByte(waterFrame->_asdu.data[nFlowCount * 6]);
					waterFrame->m_pStation->GetRtuExAlarm_State()->SetExAlarmStateByte(waterFrame->_asdu.data[nFlowCount * 6 + 1]);
					waterFrame->m_pStation->GetRtuTerminal_State()->SetTerminalStateByte(waterFrame->_asdu.data[nFlowCount * 6 + 2]);
				}	
			}
			else if (waterFrame->ControlC._Command == 7/*C_CMD_08_WPPARA*/) //ˮѹ
			{
				pSave_data->mDataType = COneData_INFO::WATER_P_DATA;

				int nPressCount = (waterFrame->_asdu.data.size() - 10 ) / 4;   //ˮѹ�Ǳ���Ŀ

				for (int i = 0 ; i < nPressCount; i ++)
				{
					//ˮѹʵʱֵ
					double realV =  (double) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 4]) * 0.01 +
						(double) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 4 + 1]) +
						(double) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 4 + 2]) * 100 +
						(double) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 4 + 3]) * 10000;

					//������ݴ洢
					CWaterPressure_PARA_PER* pPress = new CWaterPressure_PARA_PER();
					pPress->SetRealValue(realV);
					pSave_data->m_lstWaterPress.AddTail(pPress);

				}
				if( waterFrame->m_pStation != NULL)
				{
					waterFrame->m_pStation->GetRtuAlarm_State()->SetAlarmStateByte(waterFrame->_asdu.data[nPressCount * 4]);
					waterFrame->m_pStation->GetRtuExAlarm_State()->SetExAlarmStateByte(waterFrame->_asdu.data[nPressCount * 4 + 1]);
					waterFrame->m_pStation->GetRtuTerminal_State()->SetTerminalStateByte(waterFrame->_asdu.data[nPressCount * 4 + 2]);
				}
			}
			else if (waterFrame->ControlC._Command == 8/*C_CMD_09_WQPARA*/) //ˮ��
			{
				pSave_data->mDataType = COneData_INFO::WATER_Q_DATA;

				//���ĳ��Ȳ���ʱ
				if (waterFrame->_asdu.data.size() < 3 + 5 + 7)
				{
					delete pSave_data;
					pSave_data = NULL;
					return;
				}

				if (waterFrame->m_pStation && waterFrame->m_pStation->GetWaterQualityPara())
				{
					CWaterQuality_PARA* pWaterQuality_PAPR = waterFrame->m_pStation->GetWaterQualityPara();

					UINT64  WaterQualityCheckValue =(UINT64) (waterFrame->_asdu.data[0]) +
						((UINT64)waterFrame->_asdu.data[1] << 8) +
						((UINT64)waterFrame->_asdu.data[2] <<16) +
						((UINT64)waterFrame->_asdu.data[3] <<24) +
						((UINT64)waterFrame->_asdu.data[4] <<32);

					pWaterQuality_PAPR->SetQualityValue(WaterQualityCheckValue);

					//���ĳ��Ȳ���ʱ
					if (waterFrame->_asdu.data.size() < pWaterQuality_PAPR->Get_Buffer_Count() + 3 + 7   )
					{
						delete pSave_data;
						pSave_data = NULL;
						return ;
					}

					int nStep = 5;
					for (BYTE n = 0 ; n < 40; n ++)
					{
						if (pWaterQuality_PAPR->GetBitValue(n) == true)
						{
							CWaterQuality_PARA_PER* pQuality_ONE = pWaterQuality_PAPR->GetOneQualityObject(n);

							UINT64 nBufValue = 0;
							DOUBLE dParaValue = 0.0f;

							if (n != 24)
							{
								nBufValue = (UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 0]) +
									(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 1]) * 100 +
									(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2]) * 10000 +
									(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 3]) * 1000000;
								nStep += 4;
							}
							else
							{
								nBufValue =  (UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 0]) +
									(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 1]) * 100 +
									(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2]) * 10000 +
									(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 3]) * 1000000 +
									(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 4]) * 100000000;
								nStep += 5;
							}

							if (pQuality_ONE)
							{
								dParaValue = (DOUBLE)nBufValue;
								if (pQuality_ONE->m_PointBits > 0)
								{
									for (int m = 0 ; m < pQuality_ONE->m_PointBits; m ++)
									{
										dParaValue /= 10;
									}
								}
								pQuality_ONE->m_CurValue = dParaValue;

								//������ݴ洢
								CWaterQuality_PARA_PER* pQuality_Save = new CWaterQuality_PARA_PER();
								pQuality_Save->m_strName = pQuality_ONE->m_strName;
								pQuality_Save->m_strUnit = pQuality_ONE->m_strUnit;
								pQuality_Save->m_BitPos = pQuality_ONE->m_BitPos;
								pQuality_Save->m_DataBits = pQuality_ONE->m_DataBits;
								pQuality_Save->m_PointBits = pQuality_ONE->m_PointBits;
								pQuality_Save->m_CurValue = pQuality_ONE->m_CurValue;

								pSave_data->m_lstWaterQuality.AddTail(pQuality_Save);
							}
						}
					}//end for (BYTE n = 0 ; n < 40; n ++)

					waterFrame->m_pStation->GetRtuAlarm_State()->SetAlarmStateByte(waterFrame->_asdu.data[nStep]);
					waterFrame->m_pStation->GetRtuExAlarm_State()->SetExAlarmStateByte(waterFrame->_asdu.data[nStep + 1]);
					waterFrame->m_pStation->GetRtuTerminal_State()->SetTerminalStateByte(waterFrame->_asdu.data[nStep + 2]);
				}
			}//
			else if (waterFrame->ControlC._Command == C_CMD_11_RAINPARA)//�����Ա�����
			{
				pSave_data->mDataType = COneData_INFO::RAIN_DATA;

				int nRainCount = (waterFrame->_asdu.data.size() - 10 ) / 9;
				for (int i= 0; i< nRainCount; i ++)
				{
					BYTE type   = waterFrame->_asdu.data[i * 9 + 0];

					double fvalue =			((double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 9 + 1])) * 0.1 +
						((double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 9 + 2])) * 10;

					double halfhValue =		((double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 9 + 3])) * 0.1 +
						((double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 9 + 4])) * 10;

					//double ftotalValue =	((double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 9 + 5])) * 0.1 +
					//						((double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 9 + 6])) * 10 +
					//						((double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 9 + 7])) * 1000 +
					//						((double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 9 + 8])) * 100000;
					INT64  ntotalValue =	(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 9 + 5]))  +
						(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 9 + 6])) * 100 +
						(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 9 + 7])) * 10000 +
						(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 9 + 8])) * 1000000;

					double ftotalValue = ntotalValue * 0.1;

					if (type == 0 &&
						fvalue == 0.0 && 
						halfhValue == 0.0)
						blSetTimeEnable = true;
					else
						blSetTimeEnable = false;

					//������ݴ洢��Ϣ
					CRainAlert_PARA_PER* pRain = new CRainAlert_PARA_PER();
					pRain->SetDataType( type);
					pRain->SetRealTimeValue(fvalue);
					pRain->SetHalfHourValue(halfhValue);
					pRain->SetTotalValue(ftotalValue);
					pSave_data->m_lstRain.AddTail(pRain);
				}
				if (nRainCount * 9 + 3 + 7 == waterFrame->_asdu.data.size())
				{ 
					if (waterFrame->m_pStation != NULL)
					{
						waterFrame->m_pStation->GetRtuAlarm_State()->SetAlarmStateByte(waterFrame->_asdu.data[nRainCount * 9]);
						waterFrame->m_pStation->GetRtuExAlarm_State()->SetExAlarmStateByte(waterFrame->_asdu.data[nRainCount * 9 +1]);
						waterFrame->m_pStation->GetRtuTerminal_State()->SetTerminalStateByte(waterFrame->_asdu.data[nRainCount * 9 +2]);
					}
				}
			}
			else if (waterFrame->ControlC._Command == C_CMD_12_MONIPARA) //ģ����
			{
				pSave_data->mDataType = COneData_INFO::ANALOG_DATA;

				int nAnalogCount = (waterFrame->_asdu.data.size()- 10 )/ 3;

				for (int i = 0 ;i < nAnalogCount; i ++)
				{
					DWORD AnalogV =  (DWORD) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 3]) +
						(DWORD) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 3 + 1]) * 100 +
						(DWORD) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 3 + 2] & 0x0f) * 10000;

					//������ݴ洢��Ϣ
					CAnalogLimit_PARA_PER* pAnalog = new CAnalogLimit_PARA_PER();
					pAnalog->SetRealTimeValue(AnalogV);
					pSave_data->m_lstAnalog.AddTail(pAnalog);
				}

				if (waterFrame->m_pStation != NULL)
				{
					waterFrame->m_pStation->GetRtuAlarm_State()->SetAlarmStateByte(waterFrame->_asdu.data[nAnalogCount * 3]);
					waterFrame->m_pStation->GetRtuExAlarm_State()->SetExAlarmStateByte(waterFrame->_asdu.data[nAnalogCount * 3+1]);
					waterFrame->m_pStation->GetRtuTerminal_State()->SetTerminalStateByte(waterFrame->_asdu.data[nAnalogCount * 3 + 2]);
				}
			}
			else if (waterFrame->ControlC._Command == C_CMD_13_PULSEPARA) //���
			{
				pSave_data->mDataType = COneData_INFO::PULSE_DATA;

				int nPulseCount = (waterFrame->_asdu.data.size() - 10)/ 22;
				for (int i = 0; i < nPulseCount; i ++)
				{
					UINT64 code = 0;
					code =  (UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 0] ) +
						(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 1] ) * (UINT64) 100 +
						(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 2] ) * (UINT64) 10000 +
						(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 3] ) * (UINT64) 1000000 +
						(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 4] ) * (UINT64) 100000000 +
						(UINT64)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 5] ) * (UINT64) 10000000000 ;

					CString strCode = "",strTemp = "";
					for (int cIdx = 5; cIdx >= 0; cIdx --)
					{
						if (waterFrame->_asdu.data[i * 22 + cIdx] != 0)
						{
							strTemp.Format("%X",waterFrame->_asdu.data[i * 22 + cIdx]);
							strCode += strTemp;
							for(int _Idx= cIdx-1;_Idx >= 0; _Idx --)
							{
								strTemp.Format("%02X",waterFrame->_asdu.data[i * 22 + _Idx]);
								strCode += strTemp;
							}
							break;
						}				
					}
					if (strCode.IsEmpty())
						strCode = "0";

					CPulsePara_PER *pPulse = waterFrame->m_pStation->GetPulseParaObject(code);

					if (pPulse == NULL)
					{
						pPulse = new CPulsePara_PER(code,0.0,0.0,0.0,0.0,strCode);
						waterFrame->m_pStation->InsertPulseParaObject(pPulse);					
					}

					double dbvalue = 0.0;
					dbvalue =	(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 6] ) * (double) 0.01 +
						(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 7] ) * (double) 1 +
						(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 8] ) * (double) 100 +
						(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 9] ) * (double) 10000;
					pPulse->SetValue1(dbvalue);


					dbvalue =	(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 10] ) * (double) 0.01 +
						(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 11] ) * (double) 1 +
						(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 12] ) * (double) 100 +
						(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 13] ) * (double) 10000;
					pPulse->SetValue2(dbvalue);


					dbvalue =	(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 14] ) * (double) 0.01 +
						(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 15] ) * (double) 1 +
						(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 16] ) * (double) 100 +
						(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 17] ) * (double) 10000;
					pPulse->SetValue3(dbvalue);


					dbvalue =	(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 18] ) * (double) 0.01 +
						(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 19] ) * (double) 1 +
						(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 20] ) * (double) 100 +
						(double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[ i * 22 + 21] ) * (double) 10000;
					pPulse->SetValue4(dbvalue);

					CPulsePara_PER* pSavePulse = new CPulsePara_PER(code,pPulse->GetValue1(),pPulse->GetValue2(),pPulse->GetValue3(),pPulse->GetValue4(),pPulse->GetCodeString());
					pSave_data->m_lstPulses.AddTail(pSavePulse);
				}

				waterFrame->m_pStation->GetRtuAlarm_State()->SetAlarmStateByte(waterFrame->_asdu.data[nPulseCount* 22]);
				waterFrame->m_pStation->GetRtuExAlarm_State()->SetExAlarmStateByte(waterFrame->_asdu.data[nPulseCount * 22  + 1]);
				waterFrame->m_pStation->GetRtuTerminal_State()->SetTerminalStateByte(waterFrame->_asdu.data[nPulseCount * 22 + 2]);

			}
			else if (waterFrame->ControlC._Command == C_CMD_14_DISPLACEMENT) //���λ����
			{
				pSave_data->mDataType = COneData_INFO::DISPLACE_DATA;
				int DisplaceCount = (waterFrame->_asdu.data.size()- 10) / 6;
				for (int i = 0 ;i < DisplaceCount ; i ++)
				{
					int XRealValue = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i*6]) +
						CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[1+i*6]) * 100 +
						CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[2+i*6]) * 10000;

					int YRealValue = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[3+i*6]) +
						CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[4+i*6]) * 100 +
						CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[5+i*6]) * 10000;

					CDisplace_PARA_PER* pDisplace_PARA = new CDisplace_PARA_PER();
					if(XRealValue>800000)	XRealValue=800000-XRealValue;
					if(YRealValue>800000)	YRealValue=800000-YRealValue;
					pDisplace_PARA->SetRealTimeXValue(XRealValue);
					pDisplace_PARA->SetRealTimeYValue(YRealValue);
					pDisplace_PARA->SetDisplaceNo(i+1);
					pSave_data->m_lstDisplace.AddTail(pDisplace_PARA);
					
				}

				if (waterFrame->m_pStation != NULL)
				{
					waterFrame->m_pStation->GetRtuAlarm_State()->SetAlarmStateByte(waterFrame->_asdu.data[DisplaceCount * 6]);
					waterFrame->m_pStation->GetRtuExAlarm_State()->SetExAlarmStateByte(waterFrame->_asdu.data[DisplaceCount * 6+1]);
					waterFrame->m_pStation->GetRtuTerminal_State()->SetTerminalStateByte(waterFrame->_asdu.data[DisplaceCount * 6 + 2]);
				}
			}
			
			else if (waterFrame->ControlC._Command == C_CMD_15_TEMPERATURE) //�¶���
			{
				pSave_data->mDataType = COneData_INFO::TEMPERA_DATA;
				int TemperaCount = (waterFrame->_asdu.data.size() - 10) / 2;
				for (int i = 0 ;i < TemperaCount ; i ++)
				{
					DWORD RealValue = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i*2]) +
						CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[1+i*2]) * 100;


					CTempera_PARA_PER* pTempera_PARA = new CTempera_PARA_PER();
					pTempera_PARA->SetRealTimeValue(RealValue/10.0);
					pSave_data->m_lstTempera.AddTail(pTempera_PARA);
				}

				if (waterFrame->m_pStation != NULL)
				{
					waterFrame->m_pStation->GetRtuAlarm_State()->SetAlarmStateByte(waterFrame->_asdu.data[TemperaCount * 2]);
					waterFrame->m_pStation->GetRtuTerminal_State()->SetTerminalStateByte(waterFrame->_asdu.data[TemperaCount * 2 + 1]);
				}
			}

			AddDataMessage(waterFrame->m_pStation,pSave_data);
			InsertOneSaveData(pSave_data);

		}
	} //end if (waterFrame->GetResendType() == false)

	IEC_waterdata_EN_85H(waterFrame);

	if (waterFrame->m_pStation)
		waterFrame->m_pStation->SetTimeEnable(blSetTimeEnable);
}

void CSysDataTrans::IEC_waterdata_EN_68H( CPacketFrameFormat* appFrame )
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	if (!appFrame)  return;
	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._DIR = DOWN_ARROW;

	appFrame->_asdu._AFN = 0x68;

	BYTE len = 0;
	BuildSendBuffer(appFrame,sBuf,len);

	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	AddReSendObject (sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

void CSysDataTrans::IEC_waterdata_EN_69H( CPacketFrameFormat* appFrame )
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	if (!appFrame)  return;

	appFrame->ControlC._FCB = 3;
	appFrame->ControlC._DIR = DOWN_ARROW;
	appFrame->ControlC._Command = C_CMD_05_MSUERSTATE;

	appFrame->_asdu._AFN = 0x69;
	BYTE len = 0;
	BuildSendBuffer(appFrame,sBuf,len);

	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
	AddReSendObject (sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

void CSysDataTrans::IEC_waterdata_EN_85H( CPacketFrameFormat* appFrame )
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	if (!appFrame)  return;
	appFrame->ControlC._DIR = DOWN_ARROW;

	appFrame->_asdu._AFN = 0x85;
	appFrame->_asdu.data.clear();
	appFrame->_asdu.data = std::vector<BYTE>(1);

	if (appFrame->m_pStation != NULL)
		appFrame->_asdu.data[0] = appFrame->m_pStation->GetWorkMode();
	else
		appFrame->_asdu.data[0] = 1;

	BYTE len = 0;

	BuildSendBuffer(appFrame,sBuf,len);
	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

void CSysDataTrans::IEC_waterdata_US_A0H( CPacketFrameFormat* waterFrame )
{	
	if (!waterFrame)	return;
	if (waterFrame->ControlC._DIR != UP_ARROW)	return;
	if (waterFrame->_asdu._AFN != 0xA0)	 return;
	if (waterFrame->_asdu.data.size() < 10 ) return;
	if (waterFrame->m_pStation == NULL) return;


	if (waterFrame->_asdu.data.size() >= 14 )
	{
		COneData_INFO* pSave_data = new COneData_INFO();
		BuildBasicDataInfo(pSave_data,waterFrame);

		BYTE cp56[7];
		memset(cp56,0,sizeof(cp56));
		int _tpPos = (int)waterFrame->_asdu.data.size() - 7;
		for (int i = 0 ;i < 7; i ++)
		{
			cp56[i] = waterFrame->_asdu.data[_tpPos + i];
		}

		//ʱ����Ϣ
		pSave_data->mCollectTime.wMilliseconds	= (cp56[0] | cp56[1]<<8) % 1000;
		pSave_data->mCollectTime.wSecond		= (cp56[0] | cp56[1] <<8) /1000;
		pSave_data->mCollectTime.wMinute		= cp56[2];
		pSave_data->mCollectTime.wHour			= cp56[3];
		pSave_data->mCollectTime.wDay			= cp56[4];
		pSave_data->mCollectTime.wMonth			= cp56[5];
		pSave_data->mCollectTime.wYear			= 2000 + cp56[6];

		pSave_data->mDataType = COneData_INFO::PEACE_REPORT;


		waterFrame->m_pStation->GetRtuAlarm_State()->SetAlarmStateByte(waterFrame->_asdu.data[0]);
		waterFrame->m_pStation->GetRtuExAlarm_State()->SetExAlarmStateByte(waterFrame->_asdu.data[1]);
		//waterFrame->m_pStation->GetRtuTerminal_State()->SetTerminalStateByte(waterFrame->_asdu.data[2]);

		CString sReport = "",sTemp = "";


		sTemp.Format("�ն�ÿ��ƽ����\r\n");
		sReport += sTemp;

		sTemp.Format("����ʱ��:%s\r\n",pSave_data->Get_CollectTime_String());
		sReport += sTemp;

		//sTemp.Format("����״̬:0x%02X,��չ����״̬:0x%02X,�ն�״̬:0x%02X\r\n",
		//	waterFrame->_asdu.data[0],
		//	waterFrame->_asdu.data[1],
		//	waterFrame->_asdu.data[2]);
		//sReport += sTemp


		sReport += waterFrame->m_pStation->GetRtuAlarm_State()->GetDescription();
		sReport += waterFrame->m_pStation->GetRtuExAlarm_State()->GetDescription();


		//2013-04-11 ��ƽ���������˵���
		//---------------------------
		std::bitset<8> bits = waterFrame->_asdu.data[2];

		//return bits.at(_pos);

		CString sResult = ""/*,sTemp =""*/,sValue ="";
		int workMode =(bits.at(0) == true ? 1:0 ) +
			(bits.at(1) == true ? 2:0 ) +
			(bits.at(7) == true ? 4:0);
		if (workMode>=0 && workMode <= 4)
		{
			CString strWorkMode[]={"���ݹ���ģʽ",
				"�Ա�����ģʽ",
				"��ѯ/Ӧ����ģʽ",
				"����/ά��ģʽ",
				"��Ъ����ģʽ"};
			sTemp.Format("����ģʽ:%s,",strWorkMode[workMode]);
			sResult += sTemp;
		}

		bool bitValue = false;

		bitValue = bits.at(6);
		sValue = "";
		if (bitValue == true)
			sValue = "���ع���";
		else
			sValue = "�ⲿ����";
		if (sValue != "")
		{
			sTemp.Format("��Դ����״̬:%s\r\n",sValue);
			sResult += sTemp;
		}

		//return sResult;

		//sReport += waterFrame->m_pStation->GetRtuTerminal_State()->GetDescription()
		sReport += sResult;

		UINT16 dcDy = waterFrame->_asdu.data[3] + waterFrame->_asdu.data[4] * 256;
		UINT16 dyDy	= waterFrame->_asdu.data[5] + waterFrame->_asdu.data[6] * 256;

		//sTemp.Format("װ�õ�ص�ѹ:%.1fV,װ�õ�Դ��ѹ:%dV\r\n",(float)(0.1 * dcDy),dyDy);
		sTemp.Format("װ�õ�ص�ѹ:%.1fV,װ�õ�Դ��ѹ:%.1fV\r\n",(float)(0.1 * dcDy),(float)(0.1*dyDy));
		sReport += sTemp;

		pSave_data->sPeaceReport = sReport;

		AddDataMessage(waterFrame->m_pStation,pSave_data);
		InsertOneSaveData(pSave_data);
	}

	IEC_waterdata_EN_A0H(waterFrame);
}

void CSysDataTrans::IEC_waterdata_EN_A0H( CPacketFrameFormat* appFrame )
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	if (!appFrame)  return;
	appFrame->ControlC._DIR = DOWN_ARROW;

	appFrame->_asdu._AFN = 0xA0;
	appFrame->_asdu.data.clear();
	appFrame->_asdu.data = std::vector<BYTE>(1);

	if (appFrame->m_pStation != NULL)
		appFrame->_asdu.data[0] = appFrame->m_pStation->GetWorkMode();
	else
		appFrame->_asdu.data[0] = 1;

	BYTE len = 0;

	BuildSendBuffer(appFrame,sBuf,len);
	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

