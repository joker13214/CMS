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
		case 0x68:				//V2.0 版本，针对攀枝花项目修改扩展的
			IEC_waterdata_US_68H(pWaterFrame);
			break;
		case 0x69:				//V2.0 版本，针对攀枝花项目修改扩展的
			IEC_waterdata_US_69H(pWaterFrame);
			break;
		case 0x6A:				//V2.0 版本，针对木里县项目修改扩展的深度位移传感器
			IEC_waterdata_US_6AH(pWaterFrame);
			break;
		case 0x6B:				//V2.0 版本，针对木里县项目修改扩展的温度传感器
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
		case 0x85:			//V2.0 版本，针对攀枝花项目修改扩展的
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

		case 0xA0:      //V2.0 版本新增,平安报
			IEC_waterdata_US_A0H(pWaterFrame);
			break;
		}
	}
}

//上送报文处理函数
//要求报文通过了正确报文的格式检查
void CSysDataTrans::Dispatcher(BYTE* buffer,int bufferLen,BYTE CmInterface/* =Serial_CommInterface */, CString FromIP/* ="" */,int FromPort/* =0 */)
{

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	CRealTimeDB* pRealDb = pApp->m_pReadDB;
	if (!pRealDb)  
		return ;

	CPacketFrameFormat waterFrame;
	if (!CUdfGlobalFUNC::IEC_waterdata_to_APPFRAME(buffer,bufferLen,&waterFrame))
	{
		TRACE("报文帧格式分析不正确！\r\n");
		return;
	}

	CRtuStation* pSub =  pRealDb->SearchRtuStation(waterFrame._addr_info.A1.ProvinceCode,
		waterFrame._addr_info.A1.RegionCode,
		waterFrame._addr_info.A2);
	if (!pSub)     
	{
		TRACE("未找到站:省码:%d,地码:%d,终端/中继地址:%d\r\n",waterFrame._addr_info.A1.ProvinceCode,
			waterFrame._addr_info.A1.RegionCode,
			waterFrame._addr_info.A2);
		waterFrame.m_pStation = NULL;
		return ;
	}
	else
	{
		waterFrame.m_pStation = pSub;

		//插入接收报文日志
		AddRecvLog(buffer,bufferLen,&waterFrame);

		pSub->SetLastRecvTime();
		if (pSub->SetCommState(Connected_State) == true)  //状态有改变，更新CMSRightView的显示
			UpdateRtuCommState(pSub);

		if(pSub->GetInterface() != CmInterface)
			pSub->SetInterface(CmInterface);		 //更新通信接口

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

//报文帧格式分析
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
//	if (waterFrame->ControlC._DIV == 1)				//拆分报文
//	{
//		waterFrame->ControlC._DIVS = Buffer[4];
//
//		//分析地址域
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
//	else if (waterFrame->ControlC._DIV == 0)			//未拆分报文
//	{
//
//		//分析地址域信息
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


//链路检查(确认帧)
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
	if (LinkCheckState      == 0xF0)		//登录
	{
		strLog.Format("链路检测:登录 成功!");
	}
	else if (LinkCheckState == 0xF1)     //退出登录
	{
		strLog.Format("链路检测:退出登录 成功!");
	}
	else if (LinkCheckState == 0xF2)     //保持在线
	{
		strLog.Format("链路检测:在线保持 成功!");
	}
	else
	{
		strLog.Format("链路检测:未知!");
		logLevel = Warning_log_Message;
	}

	TRACE("链路检测(确认帧):%s\r\n",strLog);

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

	CString sLog = "遥测终端机:链路检测";
	_Log_level_type xLevel = Normal_Log_Message;
	AddLogMessage(waterFrame->m_pStation,sLog,xLevel,(PSYSTEMTIME)NULL);

	IEC_waterdata_EN_03H(waterFrame);
}



//设置终端站流量的表底（初始）值（响应帧）
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

//查询遥测终端或中继站地址（响应帧）
void CSysDataTrans::IEC_waterdata_US_50H(CPacketFrameFormat* waterFrame)  
{
	if (!waterFrame) return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;
	if (waterFrame->ControlC._Command != C_CMD_04_MSUREPARA)  return;
	if (waterFrame->_asdu._AFN != 0x50) return;

}


//查询遥测终端或终极站时钟（响应帧）
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

	TRACE("查询时钟响应:%s\r\n",strClock);

	if (waterFrame->m_pStation != NULL)
	{
		UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_datetime_call);
		UpdateRtuClockString(waterFrame->m_pStation,strClock);
	}
}

//查询遥测终端的工作模式（响应帧）
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

//查询遥测终端的数据自报种类及时间间隔(响应帧）
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

//查询遥测站需查询的实时数据种类（响应帧）
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

//查询遥测终端最近成功充值量和现有的剩余水量（响应帧）
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

//查询遥测终端的剩余水量和报警值（响应帧）
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

//查询遥测终端的水位基值、水位上下限（响应帧）
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

//查询遥测终端水压上、下限值（响应帧）
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

//查询遥测终端水质参数种类、上限值（响应帧）
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

//查询遥测终端水质参数种类、下限值（响应帧）
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

//查询遥测终端实时值（响应帧）
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

	if (QueryCmd == 2)  //否认帧
	{

	}
	else if (QueryCmd == C_CMD_06_WLPARA)									//水位
	{
		if (waterFrame->m_pStation != NULL)
		{
			int nLevelCount = ((int)waterFrame->_asdu.data.size()-2) / 3 ; //水位点个数
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
	else if (QueryCmd == C_CMD_07_FLOWPARA)			//流量
	{
		if (waterFrame->m_pStation != NULL)
		{
			int nFlowCount = ((int)waterFrame->_asdu.data.size() - 2)/ 6;   //流量仪器个数
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
				//累计取水量
				//DWORD CalTakeWater = (DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 0]) +
				//	(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 1]) * 100 +
				//	(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 2]) * 10000 +
				//	(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 3]) * 1000000;

				////瞬时流量
				//WORD InsFlow   = (WORD)(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 4])) +
				//	(WORD)(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 5])) * 100;

				//累计取水量
				DWORD s1 = (DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 0]);
				DWORD s2 = (DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 1]) * 100;
				DWORD s3 = (DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 2]) * 10000;
				DWORD s4 = (DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 3]) * 1000000;
				DWORD s5 = (DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 4]) * 100000000;
				DWORD CalTakeWater = s1 + s2 + s3 + s4+s5;
				//瞬时流量
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
	else if (QueryCmd == C_CMD_08_WPPARA)			//水压
	{
		if (waterFrame->m_pStation != NULL)
		{
			int nPressCount = ((int)waterFrame->_asdu.data.size() - 2) / 4;   //水压仪表数目
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
				//水压实时值
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
	else if (QueryCmd == C_CMD_09_WQPARA)			//水质
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
	else if (QueryCmd == C_CMD_10_GENERALPARA)		//综合参数
	{

	}
}


//查询终端的历史日记录（响应帧）
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

		CString sDayHisData = "";				//日历史数据信息
		CString strDate = "",strType = "";		//日期、类型信息
		CString strTemp = "",strDataInfo="";	//数据信息

		SYSTEMTIME MaxTime,MinTime;
		GetLocalTime(&MaxTime);
		GetLocalTime(&MinTime);

		BYTE zbType = ((waterFrame->_asdu.data[0] >> 6) & 0x03);
		BYTE zdQuery = ((waterFrame->_asdu.data[1] >> 5) & 0x07);
		WORD wDay = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[0] & 0x3f);
		WORD wMonth = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[1] & 0x1f);
		WORD wYear	= 2000 + CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[2]);
		if (zbType == 2)		//整点数据
			strDate.Format("%04d年%02d月%02d日 整点历史日记录\r\n查询参数的编号:%d\r\n",wYear,wMonth,wDay,zdQuery);
		else if (zbType == 1)	//半点数据
			strDate.Format("%04d年%02d月%02d日 整点历史日记录\r\n查询参数的编号:%d\r\n",wYear,wMonth,wDay,zdQuery);

		if (waterFrame->ControlC._Command == C_CMD_06_WLPARA) //水位
		{
			strType.Format("参数类型:水位数据\r\n");

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
			strTemp.Format("最大值:%6.2f m,%04d年%02d月%02d日 %02d:%02d\r\n",
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

			strTemp.Format("最小值:%6.2f m,%04d年%02d月%02d日 %02d:%02d\r\n",
				fMinValue,
				MinTime.wYear,
				MinTime.wMonth,
				MinTime.wDay,
				MinTime.wHour,
				MinTime.wMinute);
			strDataInfo += strTemp;

		}
		else if (waterFrame->ControlC._Command  == C_CMD_07_FLOWPARA) //流量
		{
			strType.Format("参数类型:流量数据\r\n");

			if (waterFrame->_asdu.data.size() < 3 + 6 * 26 + 5 * 2)
				return;

			strDataInfo ="";

			int nStep = 3;
			for (int i = 0; i < 24; i ++)
			{
				//累计取水量
				DWORD CalTakeWater = (DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep+ 0]) +
					(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep+ 1]) * 100 +
					(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2]) * 10000 +
					(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 3]) * 1000000;
				//瞬时流量
				WORD InsFlow   = (WORD)(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 4])) +
					(WORD)(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 5])) * 100;

				if (zbType == 2)
					strTemp.Format("%02d:00 累计取水量:%8d 立方米 瞬时流量:%4d 立方米/h\r\n",i,CalTakeWater,InsFlow);
				else if (zbType == 1)
					strTemp.Format("%02d:30 累计取水量:%8d 立方米 瞬时流量:%4d 立方米/h\r\n",i,CalTakeWater,InsFlow);
				strDataInfo += strTemp;

				nStep += 6;
			}

			MaxTime.wMinute = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 0]);
			MaxTime.wHour   = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 1]);
			MaxTime.wDay    = CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2]);
			MaxTime.wMonth	= CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 3] & 0x1f);
			MaxTime.wYear   = 2000 + CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 4]);
			nStep += 5;


			//累计取水量
			DWORD CalTakeWater = (DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep+ 0]) +
				(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep+ 1]) * 100 +
				(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2]) * 10000 +
				(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 3]) * 1000000;
			//瞬时流量
			WORD InsFlow   = (WORD)(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 4])) +
				(WORD)(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 5])) * 100;

			nStep += 4;

			strTemp.Format("最大值, 累计取水量:%8d 立方米 瞬时流量:%4d 立方米/h,,%04d年%02d月%02d日 %02d:%02d\r\n",
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

			//累计取水量
			CalTakeWater = (DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep+ 0]) +
				(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep+ 1]) * 100 +
				(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 2]) * 10000 +
				(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 3]) * 1000000;
			//瞬时流量
			InsFlow   = (WORD)(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 4])) +
				(WORD)(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[nStep + 5])) * 100;

			strTemp.Format("最小值, 累计取水量:%8d 立方米 瞬时流量:%4d 立方米/h,,%04d年%02d月%02d日 %02d:%02d\r\n",
				CalTakeWater,
				InsFlow,
				MinTime.wYear,
				MinTime.wMonth,
				MinTime.wDay,
				MinTime.wHour,
				MinTime.wMinute);

			strDataInfo += strTemp;

		}
		else if (waterFrame->ControlC._Command == C_CMD_08_WPPARA)  //水压
		{
			strType.Format("参数类型:水压数据\r\n");

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
			strTemp.Format("最大值:%8.2f kPa,%04d年%02d月%02d日 %02d:%02d\r\n",
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

			strTemp.Format("最小值:%8.2f kPa,%04d年%02d月%02d日 %02d:%02d\r\n",
				fMinValue,
				MinTime.wYear,
				MinTime.wMonth,
				MinTime.wDay,
				MinTime.wHour,
				MinTime.wMinute);
			strDataInfo += strTemp;
		}
		else if (waterFrame->ControlC._Command == C_CMD_09_WQPARA)  //水质
		{
			strType.Format("参数类型:水质数据\r\n");
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
					strTemp.Format("水质类型:%s\r\n",pWQuality->m_strName);
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

					strTemp.Format("最大值:%10.*f %s,%04d年%02d月%02d日 %02d:%02d\r\n",
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

					strTemp.Format("最小值:%10.*f %s,%04d年%02d月%02d日 %02d:%02d\r\n",
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
		else if (waterFrame->ControlC._Command == C_CMD_11_RAINPARA) //雨量
		{
			strType.Format("参数类型:雨量数据\r\n");

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

			strTemp.Format("最大值:%3.1f,%04d年%02d月%02d日 %02d:%02d\r\n",MaxData,
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


			strTemp.Format("最小值:%3.1f,%04d年%02d月%02d日 %02d:%02d\r\n",MinData,
				MinTime.wYear,
				MinTime.wMonth,
				MinTime.wDay,
				MinTime.wHour,
				MinTime.wMinute);

			strDataInfo += strTemp;

		}
		else if (waterFrame->ControlC._Command == C_CMD_12_MONIPARA) //模拟量
		{
			strType.Format("参数类型:模拟量数据\r\n");


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

			strTemp.Format("最大值:%5d,%04d年%02d月%02d日 %02d:%02d\r\n",MaxData,
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

			strTemp.Format("最小值:%5d,%04d年%02d月%02d日 %02d:%02d\r\n",MinData,
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

//查询遥测终端的事件记录（响应帧）
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

//查询遥测终端状态和报警状态（响应帧）
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

//查询水泵电机实时工作数据（响应帧）
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

//查询终端站转发中继引导码长值（响应帧）
void CSysDataTrans::IEC_waterdata_US_60H(CPacketFrameFormat* waterFrame)   
{
	if (!waterFrame) return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;
}

//查询遥测终端图像记录（响应帧）
void CSysDataTrans::IEC_waterdata_US_61H(CPacketFrameFormat* waterFrame)   
{
	if (!waterFrame) return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;

}

//查询中继站转发终端地址（响应帧)
void CSysDataTrans::IEC_waterdata_US_62H(CPacketFrameFormat* waterFrame)   
{
	if (!waterFrame) return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;
}

//查询中继站状态和切换记录（响应帧）
void CSysDataTrans::IEC_waterdata_US_63H(CPacketFrameFormat* waterFrame)   
{
	if (!waterFrame)  return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;
}

//查询遥测站雨量数据报警值（ANF＝64H）
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

//查询遥测站模拟量数据上下限（ANF＝65H）
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



//查询遥测终端扩展报警状态和终端状态（ANF＝66H）
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

//查询遥测终端扩展实时值（ANF＝67H）
void CSysDataTrans::IEC_waterdata_US_67H(CPacketFrameFormat* waterFrame)  
{
	if (!waterFrame) return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;
	if (waterFrame->_asdu.data.size() < 2) return;


	DeleteOneReSendObject(waterFrame->m_pStation->GetQueryRealDataCmd(),waterFrame->_asdu._AFN,waterFrame->_addr_info);

	BYTE QueryCmd = waterFrame->m_pStation->GetQueryRealDataCmd();
	if (QueryCmd == C_CMD_11_RAINPARA)  //雨量
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
	else if (QueryCmd == C_CMD_12_MONIPARA) //模拟量
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
	else if (QueryCmd == C_CMD_13_PULSEPARA)   //电度
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





//查询遥测站深度位移量数据上下限（ANF＝6AH）//add by yhp20160614
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



//查询遥测站温度量数据上下限（ANF＝6BH）//add by yhp20160614
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



//遥控启动水泵或阀门/闸门（响应帧）
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
		if (ResultFlag == 0x0A )  //遥控启动水泵或阀门/闸门成功
		{
			blYK_Operate = true;
		}

		//更新界面
		if (blYK_Operate == true)
			waterFrame->m_pStation->Set_YK_Op_Result(YKStart_PumpGate_SUC);
		else if (blYK_Operate == false)
			waterFrame->m_pStation->Set_YK_Op_Result(YKStart_PumpGate_ERR);

		UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_yk_operate);
	}

}

//遥控关闭水泵或阀门/闸门（响应帧）
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
		if (ResultFlag == 0x0A )  //遥控关闭水泵或阀门/闸门成功
		{
			blYK_Operate = true;
		}

		//更新界面
		if (blYK_Operate == true)
			waterFrame->m_pStation->Set_YK_Op_Result(YKClose_PumpGate_SUC);
		else if (blYK_Operate == false)
			waterFrame->m_pStation->Set_YK_Op_Result(YKClose_PumpGate_ERR);

		UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_yk_operate);
	}
}

//遥控终端或中继站通信机切换（响应帧）
void CSysDataTrans::IEC_waterdata_US_94H(CPacketFrameFormat* waterFrame)   
{
	if (!waterFrame) return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;

}


//遥控中继站工作机切换（响应帧）
void CSysDataTrans::IEC_waterdata_US_95H(CPacketFrameFormat* waterFrame)   
{
	if (!waterFrame) return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;

}

//置遥测终端IC卡功能有效（响应帧）
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

		if (waterFrame->_asdu.data[0] == 0x5A)  //置IC卡功能有效执行完毕
		{
			blResult = true;
		}

		//更新界面
		if (blResult == true)
			waterFrame->m_pStation->Set_YK_Op_Result(YKICCard_Enable_SUC);
		else if (blResult == false)
			waterFrame->m_pStation->Set_YK_Op_Result(YKICCard_Enable_ERR);

		UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_yk_operate);
	}
}

//取消遥测终端IC卡功能（响应帧）
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

		if (waterFrame->_asdu.data[0] == 0x5A)  //置IC卡功能无效执行完毕
		{
			blResult = true;
		}

		//更新界面
		if (blResult == true)
			waterFrame->m_pStation->Set_YK_Op_Result(YKICCard_DisEnable_SUC);
		else if (blResult == false)
			waterFrame->m_pStation->Set_YK_Op_Result(YKICCard_DisEnable_ERR);

		UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_yk_operate);
	}	
}

//定值控制功能投入（响应帧）
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

		if (waterFrame->_asdu.data[0] == 0x5A)  //定值控制功能投入执行完毕
		{
			blResult = true;
		}

		//更新界面
		if (blResult == true)
			waterFrame->m_pStation->Set_YK_Op_Result(YKZD_Input_SUC);
		else if (blResult == false)
			waterFrame->m_pStation->Set_YK_Op_Result(YKZD_Input_ERR);

		UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_yk_operate);
	}	
}

//定值控制功能退出（响应帧）
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

		if (waterFrame->_asdu.data[0] == 0x5A)  //定值控制功能退出执行完毕
		{
			blResult = true;
		}

		//更新界面
		if (blResult  == true)
			waterFrame->m_pStation->Set_YK_Op_Result(YKZD_Exit_SUC);
		else if(blResult == false)
			waterFrame->m_pStation->Set_YK_Op_Result(YKZD_Exit_ERR);

		UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_yk_operate);
	}	
}

//定值量设定（响应帧）
void CSysDataTrans::IEC_waterdata_US_34H(CPacketFrameFormat* waterFrame)   
{
	if (!waterFrame) return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;

}

//复位遥测终端参数和状态命令（响应帧）
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

		if (waterFrame->_asdu.data[0] == 0x5A)  //复位遥测终端执行完毕
		{
			blResult = true;
		}

		//更新界面
		if (blResult  == true)
			waterFrame->m_pStation->Set_YK_Op_Result(YKReset_Rtu_SUC);
		else if (blResult  == false)
			waterFrame->m_pStation->Set_YK_Op_Result(YKReset_Rtu_ERR);

		UpdateAttribFrame(waterFrame->m_pStation,rtu_attr_yk_operate);
	}	
}

//清空遥测终端的历史数据单元（响应帧）
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


//定时自报实时数据
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

		if(waterFrame->ControlC._Command == 5) //水位
		{
			pSave_data->mDataType = COneData_INFO::WATER_L_DATA;

			int nLevelCount = (waterFrame->_asdu.data.size()-2) / 3 ; //水位点个数
			for ( int i = 0 ; i < nLevelCount; i ++)
			{
				float RealV = ((float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 3 + 0]) * 0.01 +
					(float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 3 + 1]) +
					(float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 3 + 2]) * 100);

				//添加数据存储
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
		else if (waterFrame->ControlC._Command == 6) //流量
		{

			pSave_data->mDataType = COneData_INFO::FLOW_V_DATA;

			int nFlowCount = (waterFrame->_asdu.data.size() - 2)/ 6;   //流量仪器个数

			for (int i = 0 ; i < nFlowCount ; i ++)
			{
				//累计取水量
				DWORD CalTakeWater = (DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 0]) +
					(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 1]) * 100 +
					(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 2]) * 10000 +
					(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 3]) * 1000000;

				//瞬时流量
				WORD InsFlow   = (WORD)(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 4])) +
					(WORD)(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 5])) * 100;


				//添加数据存储
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
		else if (waterFrame->ControlC._Command == 7)  //水压
		{
			pSave_data->mDataType = COneData_INFO::WATER_P_DATA;

			int nPressCount = (waterFrame->_asdu.data.size() - 2) / 4;   //水压仪表数目

			for (int i = 0 ; i < nPressCount; i ++)
			{
				//水压实时值
				double realV = (double) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 4]) * 0.01 +
					(double) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 4 + 1]) +
					(double) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 4 + 2]) * 100 +
					(double) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 4 + 3]) * 10000;


				//添加数据存储
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
		else if (waterFrame->ControlC._Command == 8) //水质
		{
			pSave_data->mDataType = COneData_INFO::WATER_Q_DATA;

			//报文长度不对时
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

				//报文长度不对时
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

							//添加数据存储
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

//随机自报报警数据
void CSysDataTrans::IEC_waterdata_US_81H(CPacketFrameFormat* waterFrame)   
{
	if (!waterFrame) return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;
	if (waterFrame->_asdu.data.size() < 9) return;

	if (waterFrame->GetRepeatType() == false)
	{
		WORD AlertValue  = (waterFrame->_asdu.data[0] | (waterFrame->_asdu.data[1] << 8));
		std::bitset<16> alertbits = AlertValue;
		int nCount = waterFrame->_asdu.data.size();   //报文帧长度 包括数据域和Tp

		//处理TP
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

		//TP赋值
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


				if (_pos == 2)  //水位超限
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
				else if(_pos == 3) //水压超限
				{
					int nPressCount = (waterFrame->_asdu.data.size() - 2 -2- 5) / 4;   //水压仪表数目

					for (int i = 0 ; i < nPressCount; i ++)
					{
						//水压实时值
						double realV = (double) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[2 + i * 4]) * 0.01 +
							(double) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[2 + i * 4 + 1]) +
							(double) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[2 + i * 4 + 2]) * 100 +
							(double) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[2 + i * 4 + 3]) * 10000;

						CWaterPressure_PARA_PER* pPress = new CWaterPressure_PARA_PER();
						pPress->SetRealValue(realV);
						pSave_data->m_lstWaterPress.AddTail(pPress);
					}
				}
				else if (_pos == 4) //水质越限
				{
					//报文长度不对时
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

						//报文长度不对时
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


									//添加数据存储
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
				else if (_pos == 7) //剩余水量报警
				{
					int nFlowCount = (waterFrame->_asdu.data.size() - 2 -2 -5 )/ 6;   //流量仪器个数

					for (int i = 0 ; i < nFlowCount ; i ++)
					{
						//累计取水量
						DWORD CalTakeWater = (DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 0]) +
							(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 1]) * 100 +
							(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 2]) * 10000 +
							(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 3]) * 1000000;

						//瞬时流量
						WORD InsFlow   = (WORD)(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 4])) +
							(WORD)(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 5])) * 100;

						//添加数据存储
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

	//响应随机自报报警报文
	IEC_waterdata_EN_81H(waterFrame);
}


//人工值数
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
		if (waterFrame->ControlC._Command == 5) //水位人工置数
		{
			pSave_data->mDataType = COneData_INFO::WATER_L_DATA;

			int nLevelCount = (waterFrame->_asdu.data.size()-2) / 3 ; //水位点个数

			for ( int i = 0 ; i < nLevelCount; i ++)
			{
				float RealV = ((float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 3 + 0]) * 0.01 +
					(float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 3 + 1]) +
					(float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 3 + 2]) * 100);

				//添加数据存储
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
		else if (waterFrame->ControlC._Command == 6) //流量参数人工置数
		{
			pSave_data->mDataType = COneData_INFO::FLOW_V_DATA;

			int nFlowCount = (waterFrame->_asdu.data.size() - 2)/ 6;   //流量仪器个数

			for (int i = 0 ; i < nFlowCount ; i ++)
			{
				//累计取水量
				DWORD CalTakeWater = (DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 0]) +
					(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 1]) * 100 +
					(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 2]) * 10000 +
					(DWORD)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 3]) * 1000000;

				//瞬时流量
				WORD InsFlow   =   (WORD)(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 4])) +
					(WORD)(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 5])) * 100;


				//添加数据存储
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
		else if (waterFrame->ControlC._Command == 7) //水压参数人工置数
		{
			pSave_data->mDataType = COneData_INFO::WATER_P_DATA;

			int nPressCount = (waterFrame->_asdu.data.size() - 2) / 4;   //水压仪表数目

			for (int i = 0 ; i < nPressCount; i ++)
			{
				//水压实时值
				double realV = (double) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 4]) * 0.01 +
					(double) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 4 + 1]) +
					(double) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 4 + 2]) * 100 +
					(double) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 4 + 3]) * 10000;
				//添加数据存储
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
		else if (waterFrame->ControlC._Command == 8) //水质参数人工置数
		{
			pSave_data->mDataType = COneData_INFO::WATER_Q_DATA;

			if (waterFrame->m_pStation && waterFrame->m_pStation->GetWaterQualityPara())
			{
				CWaterQuality_PARA* pWaterQuality_PAPR = waterFrame->m_pStation->GetWaterQualityPara();

				//报文长度不对时
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

				//报文长度不对时
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

							//添加数据存储
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
		else if (waterFrame->ControlC._Command == 11) //雨量参数人工置数
		{
			pSave_data->mDataType = COneData_INFO::RAIN_DATA;

			int nCount = (waterFrame->_asdu.data.size() - 2) / 3;   //雨量仪表数目

			for (int i= 0; i< nCount; i ++)
			{
				BYTE type   = waterFrame->_asdu.data[i*3 + 0];
				double fvalue = ((double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i*3 + 1])) * 0.1 +
					((double)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i*3 + 2])) * 10;
				//添加数据存储信息
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
		else if (waterFrame->ControlC._Command == 12) //模拟量参数人工置数
		{
			pSave_data->mDataType = COneData_INFO::ANALOG_DATA;

			int nCount = (waterFrame->_asdu.data.size()-2) /3;  //模拟量个数
			for (int i = 0 ;i < nCount; i ++)
			{
				DWORD AnalogV =  (DWORD) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 3]) +
					(DWORD) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 3 + 1]) * 100 +
					(DWORD) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 3 + 2] & 0x0f) * 10000;

				//添加数据存储信息
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

//定值自报扩展实时数据
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

		if (waterFrame->ControlC._Command == C_CMD_11_RAINPARA)  //雨量自报数据
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


			//TP赋值
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


				//添加数据存储信息
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
		else if (waterFrame->ControlC._Command == C_CMD_12_MONIPARA)  //模拟量数据自报
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

			//TP赋值
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

				//添加数据存储信息
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
		else if (waterFrame->ControlC._Command == C_CMD_13_PULSEPARA) //电度量数据自报
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

			//TP赋值
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
		else if (waterFrame->ControlC._Command == C_CMD_14_DISPLACEMENT)  //模拟量数据自报
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

			//TP赋值
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

				//添加数据存储信息
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
		
		else if (waterFrame->ControlC._Command == C_CMD_15_TEMPERATURE)  //温度量数据自报
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

			//TP赋值
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

				//添加数据存储信息
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

//随机自报扩展报警数据
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


		//TP赋值
		pSave_data->mTP.m_adelay = _autoTp.m_adelay;
		pSave_data->mTP.m_day	 = _autoTp.m_day;
		pSave_data->mTP.m_hour	 = _autoTp.m_hour;
		pSave_data->mTP.m_minute = _autoTp.m_minute;
		pSave_data->mTP.m_second = _autoTp.m_second;

		if (bitWarn.at(0) == true)     //雨量超越限报警
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
		else if (bitWarn.at(1) == true)		//模拟量超越限报警
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

//向RTU下发报文
//链路检查
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


//设置遥测终端或中继站地址
void CSysDataTrans::IEC_waterdata_EN_10H(CPacketFrameFormat* appFrame)   
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	if (!appFrame) return;
}

//设置遥测终端、中继站时钟
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

	//Added 2012-11-28 添加对钟时延
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

//设置遥测终端工作模式
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

//设计遥测终端的数据自报种类及时间间隔
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

	//从RTU里获取自报数据配置信息
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

//设置遥测终端需查询的实时数据种类
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

//设置遥测终端本次充值量
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

//设置遥测终端剩余水量报警值
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

//设置遥测终端的水位基值、水位上下限
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

//设置遥测终端水压上、下限
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

//设置遥测终端水质参数种类、上限值
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

//设置遥测终端水质参数种类、下限值
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

//设置终端站流量的表底（初始）值
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

//设置遥测终端转发中继引导码长
void CSysDataTrans::IEC_waterdata_EN_1CH(CPacketFrameFormat* appFrame)
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));
}

//设置中继站转发终端地址
void CSysDataTrans::IEC_waterdata_EN_1DH(CPacketFrameFormat* appFrame)
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	if (!appFrame) return;
}

//设置中继站工作机自动切换，自报状态
void CSysDataTrans::IEC_waterdata_EN_1EH(CPacketFrameFormat* appFrame)
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	if (!appFrame) return;
}

//设置遥测终端雨量报警值
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

//设置遥测终端模拟量上下限值
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
	BYTE len = 0;    //报文长度

	BuildSendBuffer(appFrame,sBuf,len);
	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}



//设置遥测终端深度位移量上下限值//add by yhp20160614
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
	BYTE len = 0;    //报文长度

	BuildSendBuffer(appFrame,sBuf,len);
	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}



//设置遥测终端温度量上下限值//add by yhp20160614
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
	BYTE len = 0;    //报文长度

	BuildSendBuffer(appFrame,sBuf,len);
	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}


//置遥测终端IC卡功能有效
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

//取消遥测终端IC卡功能
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

//定值控制投入
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

//定值控制退出
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

//定值量设定
void CSysDataTrans::IEC_waterdata_EN_34H(CPacketFrameFormat* appFrame)
{
	if (!appFrame)		return;
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));
}

//查询遥测终端、中继站地址
void CSysDataTrans::IEC_waterdata_EN_50H(CPacketFrameFormat* appFrame)
{
	if (!appFrame)		return;
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));
}

//查询遥测终端、中继站时钟
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

//查询遥测终端工作模式
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

//查询遥测终端的数据自报种类及时间间隔
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

//查询遥测终端需查询的实时数据种类
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

//查询遥测终端最近成功充值量和现有剩余水量
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

//查询遥测终端剩余水量和报警值
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

//查询遥测终端水位基值、水位上下限值
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

//查询遥测终端水压上、下限
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

//查询遥测终端水质参数种类、上限值
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

//查询遥测终端水质参数种类、下限值
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

//查询遥测终端的实时值
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

//查询遥测终端历史日记录
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

//查询遥测终端的事件记录
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

//查询遥测终端状态和报警状态
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

//查询水泵电机实时工作数据
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

//查询遥测终端状态和报警状态
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

//查询遥测终端图像记录
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

//查询中继站转发终端地址
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

//查询中继站工作机状态和切换记录
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

//查询遥测终端雨量报警值
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

//查询遥测终端模拟量上下限值
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


//查询遥测终端深度位移量上下限值//add by yhp20160614
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

//查询遥测终端温度量上下限值//add by yhp20160614
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

//查询遥测终端扩展报警状态和终端状态
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

//查询遥测终端扩展实时值
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

//定时自报实时数据（确认帧）
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
		appFrame->_asdu.data[0] = 1; //自报模式

	BYTE len = 0;
	BuildSendBuffer(appFrame,sBuf,len);

	SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//随机自报报警数据（确认帧）
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

//人工置数（确认帧）
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

//定时自报扩展实时数据（响应帧）
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

//随机自报扩展报警数据（响应帧）
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

//复位遥测终端参数和状态
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

//清空遥测终端历史数据单元
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

//遥控启动水泵或阀门/闸门
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

//遥控关闭水泵或阀门/闸门
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

//遥控终端或中继站通信机切换
void CSysDataTrans::IEC_waterdata_EN_94H(CPacketFrameFormat* appFrame)
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	if (!appFrame)   return;
}

//遥控中继站工作机切换
void CSysDataTrans::IEC_waterdata_EN_95H(CPacketFrameFormat* appFrame)
{
	BYTE sBuf[SEND_BUFFER_LEN];
	memset(sBuf,0,sizeof(sBuf));

	if (!appFrame)   return;
}

//向链路发送数据
int CSysDataTrans::SendRtuLinkFrame(BYTE* sBuf,int len,CPacketFrameFormat* appFrame)
{
	if (!appFrame) return 0;

	return SendRtuLinkFrame(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//向通信链路发送报文数据
int CSysDataTrans::SendRtuLinkFrame(BYTE* sBuf,int len,BYTE CmInterface/* =Serial_CommInterface */,PNET_INFO pToNetInfo /* = NULL */)
{
	if (!sBuf  || len <=0 ) return 0;
	int sendLen = len;

	//向遥测终端下发报文
	if (m_pSubStation != NULL)
	{
		m_cs_SendData.Lock();

		//卫星通道只收不发 2012-07-13 修改
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

	//向遥测终端下发报文
	if (m_pSubStation != NULL)
	{
		m_cs_SendData.Lock();

		//卫星通道只收不发 2012-07-13 修改
		if (m_pSubStation->GetCmType() != SC_CommChannel)
			if (m_pSubStation->SendData(sBuf,Len))
				AddSendLog(sBuf,Len);
		m_cs_SendData.Unlock();
	}

	return sendLen;

}

//获取功能C的名称
CString CSysDataTrans::GetFunName(BYTE _DIR,BYTE _CMD)
{
	CString  strResult = "";

	if (_DIR  == 0 )  // 下行报文
	{
		switch (_CMD)
		{
		case 0:
			strResult ="备用";
			break;
		case 1:
			strResult ="命令";
			break;
		case 2:
			strResult ="用户数据";
			break;
		case 3:
			strResult ="链路测试";
			break;
		case 4:
			strResult ="被测参数";
			break;
		case 5:
			strResult ="被测状态";
			break;
		case 6:
			strResult ="水位参数";
			break;
		case 7:
			strResult ="流量参数";
			break;
		case 8:
			strResult ="水压参数";
			break;
		case 9:
			strResult ="水质参数";
			break;
		case 10:
			strResult ="综合参数";
			break;
		case 11:
			strResult = "雨量参数";
			break;
		case 12:
			strResult = "模拟量参数";
			break;
		case 13:
		case 14:
		case 15:
			strResult ="备用";
			break;
		}
	}
	else if (_DIR == 1) //上行报文
	{
		switch (_CMD)
		{
		case 0:
			strResult = "备用";
			break;
		case 1:
			strResult = "认可";
			break;
		case 2:
			strResult = "用户数据";
			break;
		case 3:
			strResult = "链路状态";
			break;
		case 4:
			strResult = "相应参数";
			break;
		case 5:
			strResult = "水位参数";
			break;
		case 6:
			strResult = "流量参数";
			break;
		case 7:
			strResult = "水压参数";
			break;
		case 8:
			strResult = "水质参数";
			break;
		case 9:
			strResult = "综合参数";
			break;
		case 10:
			strResult = "报警或状态参数";
			break;
		case 11:
			strResult = "雨量参数";
			break;
		case 12:
			strResult = "模拟量参数";
			break;
		case 13:
		case 14:
		case 15:
			strResult ="备用";
			break;
		}
	}
	return strResult;
}


//将报文转换为字符串信息
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

//添加下发报文日志
//将日志信息添加至遥测终端的属性窗口
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
		strLogBuffer.Format("发,DIR=%d,DIV=%d,FCB=%d,功能码=%-2d,AFN=%02XH,Len=%-3d,报文=%s",
			Pff.ControlC._DIR,
			Pff.ControlC._DIV,
			Pff.ControlC._FCB,
			Pff.ControlC._Command,
			Pff._asdu._AFN,len, strBuffer);

		AddLogMessage(pSub,strLogBuffer,Normal_Log_Message);
	}
}

//添加接收报文日志信息
//将日志信息添加至遥测终端的属性窗口
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

		strLogBuffer.Format("收,DIR=%d,DIV=%d,FCB=%d,功能码=%-2d,AFN=%02XH,Len=%-3d,报文=%s",
			waterFrame->ControlC._DIR,
			waterFrame->ControlC._DIV,
			waterFrame->ControlC._FCB,
			waterFrame->ControlC._Command,
			waterFrame->_asdu._AFN,len,strBuffer);

		AddLogMessage(waterFrame->m_pStation ,strLogBuffer,Normal_Log_Message);
	}
}

//更新遥测终端的通信状态
//重点:更新列表信息和添加通信状态改变日志信息
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

//检查重发对象
void CSysDataTrans::CheckResendObject()
{
	//检查保存需要重发对象的List
	m_Mutex_ReSend.Lock();

	//存储相关重发对象的临时链表
	CList<RESEND_Object*> tempList;
	while (!m_lstReSendObjects.IsEmpty())
	{
		RESEND_Object* pObj = m_lstReSendObjects.RemoveHead();

		if (pObj != NULL)
		{
			BOOL blDeleted = FALSE ;								//重发对象是否已删除的标志

			if (pObj->m_resend_timer.WaitTimerState() == true)		//检查是否到了重发的时间
			{
				pObj->m_packetFrame.ControlC._FCB -- ;				//FCB 自动减一

				if (pObj->m_packetFrame.ControlC._FCB == 0)
				{
					delete pObj;
					pObj = NULL;
					blDeleted = TRUE;
				}
				else 
				{
					//生存报文并下发
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

//添加一个重发对象
void CSysDataTrans::AddReSendObject(BYTE* sBuf,int len,CPacketFrameFormat* appFrame)
{
	if (!appFrame) return;

	AddReSendObject(sBuf,len,appFrame->m_CommInterface,&(appFrame->m_NetInfo));
}

//添加一个重发对象
void CSysDataTrans::AddReSendObject(BYTE* sBuf,int len,BYTE CmInterface /* = Serial_CommInterface */,PNET_INFO pToNetInfo /* = NULL */)
{
	if (!sBuf) return;
	if (len <= 0) return;

	//卫星通信时不启动重发机制
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
		pObj->m_resend_timer.SetWaitTimer(pObj->m_resendInterval * 1000);  //设置重发时间间隔
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

//从重发列表（弹出）获取一个重发对象
/*
* Command 为控制域命令码
* AFN     为应用命令码
* _Addr   为对应的遥测终端地址信息
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

//从重发列表中删除某一重发对象
/*
* Command 为控制域命令码
* AFN     为应用命令码
* _Addr   为对应的遥测终端地址信息
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

//生成下发报文
/*
* 通过CPacketFrameFormat对象生成下发的报文信息
* sbuf:下发的报文帧数据   (In)
* len :报文的长度（<=255）(Out)
*/
void CSysDataTrans::BuildSendBuffer(CPacketFrameFormat* packetFrame,BYTE* sBuf,BYTE& len)
{
	if (!packetFrame) return;
	if (!sBuf)        return;

	BYTE dataSize = (BYTE)packetFrame->_asdu.data.size();

	sBuf[0]		= 0x68;
	sBuf[1]		= 7 + dataSize;
	sBuf[2]		= 0x68;

	//控制域
	sBuf[3]     =   packetFrame->ControlC.GetControlCByte();
	//地址域
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

//清空重发对象列表
//析构函数访问
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

//生成保存数据的基础信息
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

//添加一条需要保存的数据信息
void CSysDataTrans::InsertOneSaveData(COneData_INFO* pData)	
{
	if (!pData) return;

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	if (pApp && pApp->m_pRtuDatasave != NULL)
		pApp->m_pRtuDatasave->InsertOneData(pData);
}

//获取遥测终端属性操作窗口
//在属性操作窗口上可以进行相应的单机调试功能
CRtuPropertyDlg* CSysDataTrans::GetRtuPropertyDlg()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	if (pMainFrame)
		return pMainFrame->GetRtuPropertyDlg();
	return NULL;
}

//添加数据信息到数据显示窗口
void CSysDataTrans::AddDataMessage(CRtuStation* pRtu,COneData_INFO* pData,BYTE nType/* =/* =AUTO_DATA */)
{
	if (!pData || !pRtu) return;

	CString strMessage = "",strTemp="";
	COneData_INFO::_Data_Type dataType = pData->mDataType;
	if (dataType == COneData_INFO::WATER_L_DATA)			//水位信息
	{
		if (nType== AUTO_DATA)
			strTemp.Format("类型:水位自报实时数据\r\n");
		else if (nType == MANU_DATA)
			strTemp.Format("类型:水位参数人工置数\r\n");

		strMessage += strTemp;


		strTemp.Format("日期时间:%s\r\n",pData->Get_CollectTime_String());
		strMessage += strTemp;


		int Index = 1;
		POSITION rPos = pData->m_lstWaterLevel.GetHeadPosition();
		while (rPos != NULL)
		{
			CWaterLevel_PAR_PER* pLevel = pData->m_lstWaterLevel.GetNext(rPos);
			if (pLevel)
			{
				//strTemp.Format("水位仪表:%d,当前值:%.2fm\r\n",Index,pLevel->GetRealValue());
				strTemp.Format("第 %2d 个水位仪表,当前值:%.2fm\r\n",Index,pLevel->GetRealValue());
				strMessage += strTemp;
				Index ++;
			}
		}
	}
	else if (dataType == COneData_INFO::WATER_P_DATA)		//水压信息
	{
		if (nType == AUTO_DATA)
			strTemp.Format("类型:水压自报实时数据\r\n");
		else if (nType == MANU_DATA)
			strTemp.Format("类型:水压参数人工置数\r\n");

		strMessage += strTemp;


		strTemp.Format("日期时间:%s\r\n",pData->Get_CollectTime_String());
		strMessage += strTemp;

		int Index = 1;
		POSITION rPos = pData->m_lstWaterPress.GetHeadPosition();
		while (rPos != NULL)
		{
			CWaterPressure_PARA_PER* pPress = pData->m_lstWaterPress.GetNext(rPos);
			if (pPress)
			{
				//strTemp.Format("水压仪表:%d,当前值:%.2fkPA\r\n",Index,pPress->GetRealValue());
				strTemp.Format("第 %2d 个水压仪表,当前值:%.2fkPA\r\n",Index,pPress->GetRealValue());
				strMessage += strTemp;
				Index ++;
			}
		}
	}
	else if (dataType == COneData_INFO::WATER_Q_DATA)	 //水质信息
	{
		if (nType == AUTO_DATA)
			strTemp.Format("类型:水质自报实时数据\r\n");
		else if (nType == MANU_DATA)
			strTemp.Format("类型:水质参数人工置数\r\n");

		strMessage += strTemp;


		strTemp.Format("日期时间:%s\r\n",pData->Get_CollectTime_String());
		strMessage += strTemp;

		POSITION rPos = pData->m_lstWaterQuality.GetHeadPosition();
		while (rPos !=NULL)
		{
			CWaterQuality_PARA_PER* pQuality = pData->m_lstWaterQuality.GetNext(rPos);

			strTemp.Format("%s,当前值:%.*f %s\r\n",
				pQuality->m_strName,
				pQuality->m_PointBits,
				pQuality->m_CurValue,
				pQuality->m_strUnit);

			strMessage += strTemp;
		}
	}
	else if (dataType == COneData_INFO::FLOW_V_DATA)	//流量信息
	{
		if (nType == AUTO_DATA)
			strTemp.Format("类型:流量自报实时数据\r\n");
		else if (nType == MANU_DATA)
			strTemp.Format("类型:流量参数人工置数\r\n");

		strMessage += strTemp;


		strTemp.Format("日期时间:%s\r\n",pData->Get_CollectTime_String());
		strMessage += strTemp;

		int Index = 1;
		POSITION rPos = pData->m_lstFlow.GetHeadPosition();
		while (rPos != NULL)
		{
			CFlowInitValume_PARA_PER* pFlow = pData->m_lstFlow.GetNext(rPos);
			if (pFlow)
			{
				//2023-5-26修改
				double leijivalue = (double)pFlow->GetCalTakeValume() / 10.0;
				double sunshivalue =(double)pFlow->GetInsFlowValume()/1000000.0;
				//strTemp.Format("流量仪器:%d,累计取水量:%d立方米,瞬时流量:%d 立方米/h\r\n",Index,pFlow->GetCalTakeValume(),pFlow->GetInsFlowValume());
				strTemp.Format("第 %2d 个流量仪,累计取水量:%.3f立方米,瞬时流量:%.3f 立方米/秒\r\n",Index,leijivalue,sunshivalue);
				strMessage += strTemp;

				Index ++;
			}
		}
	}
	else if (dataType == COneData_INFO::RAIN_DATA)	//雨量信息
	{
		if (nType == AUTO_DATA)
			strTemp.Format("类型:雨量参数自报数据\r\n");
		else if (nType == MANU_DATA)
			strTemp.Format("类型:雨量参数人工置数\r\n");
		strMessage += strTemp;

		/*
		if(pRtu->GetAppProVer() == APP_PROTOCOL_V1_0)
		strTemp.Format("发生时间: %d日 %02d:%02d:%02d(delay:%d)\r\n",
		pData->mTP.m_day,
		pData->mTP.m_hour,
		pData->mTP.m_minute,
		pData->mTP.m_second,
		pData->mTP.m_adelay);
		else 
		*/
		strTemp.Format("日期时间:%s\r\n",pData->Get_CollectTime_String());
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
						strType = "定时上送";
					else if (type == 0x01)
						strType = "开始下雨";
					else if (type == 0x02)
						strType = "超阀值上送";
					else if (type == 0x03)
						strType = "加报";//"整/半点上送";
					else if (type == 0x04)
						strType = "雨停,整场雨累计";

					if (pRtu->GetAppProVer() == APP_PROTOCOL_V1_0)
					{
						//strValue.Format("第 %2d 个雨量计,数据类型:%s,值:%.1f mm\r\n",Index,strType,fvalue);
						strValue.Format("第 %2d 个雨量计,%s,值:%.1f mm\r\n",Index,strType,fvalue);
					}
					else if (pRtu->GetAppProVer() == APP_PROTOCOL_V2_0)
					{
						//strValue.Format("第 %2d 个雨量计,数据类型:%s,值:%.1f mm,终端累计值: %.1f mm\r\n",Index,strType,fvalue,pRain->GetTotalValue());
						strValue.Format("第 %2d 个雨量计,%s,场雨累计值:%.1f mm,半小时累计值:%.1f mm,总累计值: %.1f mm\r\n",
							Index,strType,
							fvalue,halfHValue,pRain->GetTotalValue());
					}
				}
				else if (nType == MANU_DATA)
				{
					strValue.Format("第 %2d 个雨量计,人工置数:%.1f mm\r\n",
						Index,pRain->GetRealTimeValue());
				}


				strMessage += strValue;

				Index ++;
			}
		}
	}
	else if (dataType == COneData_INFO::ANALOG_DATA)	//模拟量信息
	{
		if (nType == AUTO_DATA)
			strTemp.Format("类型:模拟量参数自报数据\r\n");
		else if (nType == MANU_DATA)
			strTemp.Format("类型:模拟量参数人工置数\r\n");
		strMessage += strTemp;

		/*
		if(pRtu->GetAppProVer() == APP_PROTOCOL_V1_0)
		strTemp.Format("发生时间: %d日 %02d:%02d:%02d(delay:%d)\r\n",
		pData->mTP.m_day,
		pData->mTP.m_hour,
		pData->mTP.m_minute,
		pData->mTP.m_second,
		pData->mTP.m_adelay);
		else 
		*/
		strTemp.Format("日期时间:%s\r\n",pData->Get_CollectTime_String());
		strMessage += strTemp;


		int Index = 1;
		POSITION rPos = pData->m_lstAnalog.GetHeadPosition();
		while (rPos)
		{
			CAnalogLimit_PARA_PER* pAnalog = pData->m_lstAnalog.GetNext(rPos);
			if (pAnalog)
			{				
				strTemp.Format("第 %2d 个模拟量,当前模拟量值:%5d\r\n",Index,pAnalog->GetRealTimeValue());
				strMessage += strTemp;
				Index ++;
			}
		}
	}
	else if (dataType == COneData_INFO::PULSE_DATA)
	{
		if (nType == AUTO_DATA)
			strTemp.Format("类型:电度量参数自报数据\r\n");
		else if (nType == MANU_DATA)
			strTemp.Format("类型:电度量参数人工置数\r\n");
		strMessage += strTemp;

		/*
		if(pRtu->GetAppProVer() == APP_PROTOCOL_V1_0)
		strTemp.Format("发生时间: %d日 %02d:%02d:%02d(delay:%d)\r\n",
		pData->mTP.m_day,
		pData->mTP.m_hour,
		pData->mTP.m_minute,
		pData->mTP.m_second,
		pData->mTP.m_adelay);
		else */
		strTemp.Format("日期时间:%s\r\n",pData->Get_CollectTime_String());
		strMessage += strTemp;


		POSITION rPos = pData->m_lstPulses.GetHeadPosition();
		while (rPos)
		{			
			CPulsePara_PER* pPulse = pData->m_lstPulses.GetNext(rPos);

			if (pPulse)
			{		
				/*
				strTemp.Format("电度记地址:%d,正向有功:%.2f,正向无功:%.2f,反向有功:%.2f,反向无功:%.2f\r\n",
				pPulse->GetCode(),
				pPulse->GetValue1(),
				pPulse->GetValue2(),
				pPulse->GetValue3(),
				pPulse->GetValue4());				
				*/

				strTemp.Format("电度记地址:%s,",pPulse->GetCodeString());
				strMessage += strTemp;

				strTemp.Format("正向有功:%.2f kWh,",pPulse->GetValue1());
				strMessage += strTemp;

				strTemp.Format("正向无功:%.2f kWh,",pPulse->GetValue2());
				strMessage += strTemp;

				strTemp.Format("反向有功:%.2f kWh,",pPulse->GetValue3());
				strMessage += strTemp;

				strTemp.Format("反向无功:%.2f kWh\r\n",pPulse->GetValue4());
				strMessage += strTemp;

			}
		}
	}
	else if (dataType == COneData_INFO::ALARM_RECORD)	//随机报警数据
	{
		int nAlarmType = pData->mAlarmType;

		strTemp.Format("类型:随机自报报警数据\r\n");
		strMessage += strTemp;

		/*
		if(pRtu->GetAppProVer() == APP_PROTOCOL_V1_0)
		strTemp.Format("发生时间: %d日 %02d:%02d:%02d(delay:%d)\r\n",
		pData->mTP.m_day,
		pData->mTP.m_hour,
		pData->mTP.m_minute,
		pData->mTP.m_second,
		pData->mTP.m_adelay);
		else 
		*/
		strTemp.Format("日期时间:%s\r\n",pData->Get_CollectTime_String());
		strMessage += strTemp;


		if (nAlarmType >= 0 && nAlarmType < 18)
		{
			strTemp.Format("报警类型:%s\r\n",_Alert_Options_Name[nAlarmType]);
			strMessage += strTemp;
		}

		if (nAlarmType == (int)COneData_INFO::ALARM_WATERLEVEL)		//报警关联的水位信息
		{
			int Index = 1;
			POSITION rPos = pData->m_lstWaterLevel.GetHeadPosition();
			while (rPos != NULL)
			{
				CWaterLevel_PAR_PER* pLevel = pData->m_lstWaterLevel.GetNext(rPos);
				if (pLevel)
				{
					//strTemp.Format("水位仪表:%d,当前值:%.2fm\r\n",Index,pLevel->GetRealValue());
					strTemp.Format("第 %2d 个水位仪,当前值:%.2fm\r\n",Index,pLevel->GetRealValue());
					strMessage += strTemp;
					Index ++;
				}
			}
		}
		else if (nAlarmType == (int)COneData_INFO::ALARM_WATERPRESS)	//报警关联的水压信息
		{
			int Index = 1;
			POSITION rPos = pData->m_lstWaterPress.GetHeadPosition();
			while (rPos != NULL)
			{
				CWaterPressure_PARA_PER* pPress = pData->m_lstWaterPress.GetNext(rPos);
				if (pPress)
				{
					//strTemp.Format("水压仪表:%d,当前值:%.2fkPA\r\n",Index,pPress->GetRealValue());
					strTemp.Format("第 %2d 个水压仪,当前值:%.2fkPA\r\n",Index,pPress->GetRealValue());
					strMessage += strTemp;
					Index ++;
				}
			}
		}
		else if (nAlarmType == (int)COneData_INFO::ALARM_WATERQUALITY)	//报警关联的水质信息
		{
			POSITION rPos = pData->m_lstWaterQuality.GetHeadPosition();
			while (rPos !=NULL)
			{
				CWaterQuality_PARA_PER* pQuality = pData->m_lstWaterQuality.GetNext(rPos);

				strTemp.Format("%s,当前值:%.*f %s\r\n",
					pQuality->m_strName,
					pQuality->m_PointBits,
					pQuality->m_CurValue,
					pQuality->m_strUnit);

				strMessage += strTemp;
			}
		}
		else if (nAlarmType == (int)COneData_INFO::ALARM_FLOW)		//报警关联的流量信息
		{
			int Index = 1;
			POSITION rPos = pData->m_lstFlow.GetHeadPosition();
			while (rPos != NULL)
			{
				CFlowInitValume_PARA_PER* pFlow = pData->m_lstFlow.GetNext(rPos);
				if (pFlow)
				{
					//strTemp.Format("流量仪器:%d,累计取水量:%d立方米,瞬时流量:%d 立方米/h\r\n",Index,pFlow->GetCalTakeValume(),pFlow->GetInsFlowValume());
					strTemp.Format("第 %2d 个流量仪,累计取水量:%d 立方米,瞬时流量:%d 立方米/h\r\n",Index,pFlow->GetCalTakeValume(),pFlow->GetInsFlowValume());
					strMessage += strTemp;

					Index ++;
				}
			}
		}
		else if (nAlarmType ==(int)COneData_INFO::ALARM_RAIN)	//报警关联的雨量信息
		{
			int Index = 1;
			POSITION rPos = pData->m_lstRain.GetHeadPosition();
			while (rPos)
			{
				CRainAlert_PARA_PER* pRain = pData->m_lstRain.GetNext(rPos);
				if (pRain)
				{

					/*
					strTemp.Format("雨量记 %d,雨量报警值:%.1f mm,单位时间:%4d min\r\n",
					Index,
					pRain->GetRealTimeValue(),
					pRain->GetPerTime());
					*/
					strTemp.Format("第 %2d 个雨量计,雨量报警值:%.1f mm,单位时间:%4d min\r\n",
						Index,
						pRain->GetRealTimeValue(),
						pRain->GetPerTime());
					strMessage += strTemp;

					Index ++;
				}
			}
		}
		else if (nAlarmType == (int)COneData_INFO::ALARM_ANALOG) //报警关联的模拟量信息
		{
			int Index = 1;
			POSITION rPos = pData->m_lstAnalog.GetHeadPosition();
			while (rPos)
			{
				CAnalogLimit_PARA_PER* pAnalog = pData->m_lstAnalog.GetNext(rPos);
				if (pAnalog)
				{

					//strTemp.Format("模拟量 %d,当前模拟量值:%5d\r\n",Index,pAnalog->GetRealTimeValue());
					strTemp.Format("第 %2d 个模拟量,当前模拟量值:%5d\r\n",Index,pAnalog->GetRealTimeValue());
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

	else if (dataType == COneData_INFO::DISPLACE_DATA)	//深度位移
	{
		if (nType == AUTO_DATA)
			strTemp.Format("类型:深度位移量参数自报数据\r\n");
		else if (nType == MANU_DATA)
			strTemp.Format("类型:深度位移量参数人工置数\r\n");
		strMessage += strTemp;

		strTemp.Format("日期时间:%s\r\n",pData->Get_CollectTime_String());
		strMessage += strTemp;

		int Index = 1;
		POSITION rPos = pData->m_lstDisplace.GetHeadPosition();
		while (rPos)
		{
			CDisplace_PARA_PER* pDisplace = pData->m_lstDisplace.GetNext(rPos);
			if (pDisplace)
			{				
				strTemp.Format("第 %2d 个深度位移量,当前深度位移量值A轴:%5f mm，B轴:%5f mm\r\n",Index,pDisplace->GetRealTimeXValue(),pDisplace->GetRealTimeYValue());
				strMessage += strTemp;
				Index ++;
			}
		}
	}
	else if (dataType == COneData_INFO::TEMPERA_DATA)	//温度
	{
		if (nType == AUTO_DATA)
			strTemp.Format("类型:温度参数自报数据\r\n");
		else if (nType == MANU_DATA)
			strTemp.Format("类型:温度参数人工置数\r\n");
		strMessage += strTemp;
		strTemp.Format("日期时间:%s\r\n",pData->Get_CollectTime_String());
		strMessage += strTemp;


		int Index = 1;
		POSITION rPos = pData->m_lstTempera.GetHeadPosition();
		while (rPos)
		{
			CTempera_PARA_PER* pTempera = pData->m_lstTempera.GetNext(rPos);
			if (pTempera)
			{				
				strTemp.Format("第 %2d 个温度,当前温度值:%.1f ℃\r\n",Index,pTempera->GetRealTimeValue());
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


//添加数据信息到遥测终端的自报数据显示窗口
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

//添加日志信息到遥测终端的属性操作窗口
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

//更新遥测终端属性窗口的信息
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

//更新遥测终端的时钟信息
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

//更新遥测终端的历史日记录信息
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


//V2.0 版本，针对攀枝花项目修改扩展的
void CSysDataTrans::IEC_waterdata_US_68H( CPacketFrameFormat* waterFrame )
{
	if (!waterFrame) return;
	if (waterFrame->ControlC._DIR != UP_ARROW) return;
	if (waterFrame->_asdu._AFN != 0x68) return;
	if (waterFrame->_asdu.data.size() < 3) return;


	DeleteOneReSendObject(waterFrame->m_pStation->GetQueryRealDataCmd(),waterFrame->_asdu._AFN,waterFrame->_addr_info);

	BYTE QueryCmd = waterFrame->m_pStation->GetQueryRealDataCmd();
	if (QueryCmd == C_CMD_11_RAINPARA)  //雨量
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
	else if (QueryCmd == C_CMD_12_MONIPARA) //模拟量
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
	else if (QueryCmd == C_CMD_13_PULSEPARA)   //电度
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
	else if (QueryCmd == C_CMD_14_DISPLACEMENT) //深度位移量
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
	else if (QueryCmd == C_CMD_15_TEMPERATURE) //温度量
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

		//修改由于cp56时间出错的问题
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

			//时间信息
			pSave_data->mCollectTime.wMilliseconds	= (cp56[0] | cp56[1]<<8) % 1000;
			pSave_data->mCollectTime.wSecond		= (cp56[0] | cp56[1] <<8) /1000;
			pSave_data->mCollectTime.wMinute		= cp56[2];
			pSave_data->mCollectTime.wHour			= cp56[3];
			pSave_data->mCollectTime.wDay			= cp56[4];
			pSave_data->mCollectTime.wMonth			= cp56[5];
			pSave_data->mCollectTime.wYear			= 2000 + cp56[6];


			pSave_data->SetErrorFlag(CUdfGlobalFUNC::CheckErrorData(&(pSave_data->mCollectTime)));


			if (waterFrame->ControlC._Command == 5/*C_CMD_06_WLPARA*/) //水位
			{
				pSave_data->mDataType = COneData_INFO::WATER_L_DATA;

				int nLevelCount = (waterFrame->_asdu.data.size()- 10) / 3 ; //水位点个数

				for ( int i = 0 ; i < nLevelCount; i ++)
				{
					float RealV = ((float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 3 + 0]) * 0.01 +
						(float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 3 + 1]) +
						(float)CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 3 + 2]) * 100);

					//添加数据存储
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
			else if (waterFrame->ControlC._Command == 6/*C_CMD_07_FLOWPARA*/) //流量
			{
				pSave_data->mDataType = COneData_INFO::FLOW_V_DATA;

				int nFlowCount = (waterFrame->_asdu.data.size() - 10)/ 6;   //流量仪器个数

				for (int i = 0 ; i < nFlowCount ; i ++)
				{
					//累计取水量
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
					//瞬时流量
					/*WORD InsFlow   =	 (WORD)(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 4])) +
						(WORD)(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 5])) * 100;*/
					//DWORD v = (WORD)(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 4]));
					DWORD v1 = (WORD)(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 5]));
					DWORD v2 = (WORD)(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 6])) * 100;
					DWORD v3 = (WORD)(CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 6 + 7])) * 10000;
					WORD InsFlow = v1 + v2 + v3;
					//添加数据存储
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
			else if (waterFrame->ControlC._Command == 7/*C_CMD_08_WPPARA*/) //水压
			{
				pSave_data->mDataType = COneData_INFO::WATER_P_DATA;

				int nPressCount = (waterFrame->_asdu.data.size() - 10 ) / 4;   //水压仪表数目

				for (int i = 0 ; i < nPressCount; i ++)
				{
					//水压实时值
					double realV =  (double) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 4]) * 0.01 +
						(double) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 4 + 1]) +
						(double) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 4 + 2]) * 100 +
						(double) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 4 + 3]) * 10000;

					//添加数据存储
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
			else if (waterFrame->ControlC._Command == 8/*C_CMD_09_WQPARA*/) //水质
			{
				pSave_data->mDataType = COneData_INFO::WATER_Q_DATA;

				//报文长度不对时
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

					//报文长度不对时
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

								//添加数据存储
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
			else if (waterFrame->ControlC._Command == C_CMD_11_RAINPARA)//雨量自报数据
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

					//添加数据存储信息
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
			else if (waterFrame->ControlC._Command == C_CMD_12_MONIPARA) //模拟量
			{
				pSave_data->mDataType = COneData_INFO::ANALOG_DATA;

				int nAnalogCount = (waterFrame->_asdu.data.size()- 10 )/ 3;

				for (int i = 0 ;i < nAnalogCount; i ++)
				{
					DWORD AnalogV =  (DWORD) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 3]) +
						(DWORD) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 3 + 1]) * 100 +
						(DWORD) CUdfGlobalFUNC::ByteFromBCD(waterFrame->_asdu.data[i * 3 + 2] & 0x0f) * 10000;

					//添加数据存储信息
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
			else if (waterFrame->ControlC._Command == C_CMD_13_PULSEPARA) //电度
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
			else if (waterFrame->ControlC._Command == C_CMD_14_DISPLACEMENT) //深度位移量
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
			
			else if (waterFrame->ControlC._Command == C_CMD_15_TEMPERATURE) //温度量
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

		//时间信息
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


		sTemp.Format("终端每日平安报\r\n");
		sReport += sTemp;

		sTemp.Format("日期时间:%s\r\n",pSave_data->Get_CollectTime_String());
		sReport += sTemp;

		//sTemp.Format("报警状态:0x%02X,扩展报警状态:0x%02X,终端状态:0x%02X\r\n",
		//	waterFrame->_asdu.data[0],
		//	waterFrame->_asdu.data[1],
		//	waterFrame->_asdu.data[2]);
		//sReport += sTemp


		sReport += waterFrame->m_pStation->GetRtuAlarm_State()->GetDescription();
		sReport += waterFrame->m_pStation->GetRtuExAlarm_State()->GetDescription();


		//2013-04-11 对平安保进行了调整
		//---------------------------
		std::bitset<8> bits = waterFrame->_asdu.data[2];

		//return bits.at(_pos);

		CString sResult = ""/*,sTemp =""*/,sValue ="";
		int workMode =(bits.at(0) == true ? 1:0 ) +
			(bits.at(1) == true ? 2:0 ) +
			(bits.at(7) == true ? 4:0);
		if (workMode>=0 && workMode <= 4)
		{
			CString strWorkMode[]={"兼容工作模式",
				"自报工作模式",
				"查询/应答工作模式",
				"调试/维修模式",
				"间歇工作模式"};
			sTemp.Format("工作模式:%s,",strWorkMode[workMode]);
			sResult += sTemp;
		}

		bool bitValue = false;

		bitValue = bits.at(6);
		sValue = "";
		if (bitValue == true)
			sValue = "蓄电池供电";
		else
			sValue = "外部供电";
		if (sValue != "")
		{
			sTemp.Format("电源工作状态:%s\r\n",sValue);
			sResult += sTemp;
		}

		//return sResult;

		//sReport += waterFrame->m_pStation->GetRtuTerminal_State()->GetDescription()
		sReport += sResult;

		UINT16 dcDy = waterFrame->_asdu.data[3] + waterFrame->_asdu.data[4] * 256;
		UINT16 dyDy	= waterFrame->_asdu.data[5] + waterFrame->_asdu.data[6] * 256;

		//sTemp.Format("装置电池电压:%.1fV,装置电源电压:%dV\r\n",(float)(0.1 * dcDy),dyDy);
		sTemp.Format("装置电池电压:%.1fV,装置电源电压:%.1fV\r\n",(float)(0.1 * dcDy),(float)(0.1*dyDy));
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

