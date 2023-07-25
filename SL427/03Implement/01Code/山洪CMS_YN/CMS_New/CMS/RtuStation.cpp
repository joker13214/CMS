#include "RtuStation.h"
#include "StdAfx.h"
#include "RealTimeDB.h"
#include "CMS.h"
#include "MainFrm.h"
#include "SysDataTrans.h"
#include "SystemTimeOp.h"
#include "SCMessageDealModule.h"
#include "Protocol_TcpHandler.h"

bool CRTU_AUTOREPORT_PRA::GetAutoBitValue(size_t _pos)
{
	std::bitset<16> bits = m_wAutoTotal;
	return bits.at(_pos);
}

const WORD CRTU_AUTOREPORT_PRA::SetAutoBitValue(size_t _pos,bool _val /* = true */)
{
	std::bitset<16> bits = m_wAutoTotal;
	bits.set(_pos,_val);

	m_wAutoTotal = (WORD)bits.to_ulong();
	return m_wAutoTotal;
}

bool CRTU_YCQUERYTYPE_PARA::GetQueryBitValue(size_t _pos)
{
	std::bitset<16> bits = m_wYcQueryType;
	return bits.at(_pos);
}

const WORD  CRTU_YCQUERYTYPE_PARA::SetQueryBitValue(size_t _pos,bool _val /* = true */)
{
	std::bitset<16> bits = m_wYcQueryType;
	bits.set(_pos,_val);

	m_wYcQueryType = (WORD)bits.to_ulong();
	return m_wYcQueryType;
}

bool CClearHistoryData_PARA::GetBitValue(size_t _pos)
{
	std::bitset<8> bits = m_bClearHis_BYTE;
	return bits.at(_pos);
}

void CClearHistoryData_PARA::SetBitValue(size_t _pos,bool _val /* = true */)
{
	std::bitset<8> bits = m_bClearHis_BYTE;
	bits.set(_pos,_val);

	m_bClearHis_BYTE = (BYTE)bits.to_ulong();
}

bool CRTUTerminal_State::GetBitValue(size_t _pos)
{
	std::bitset<8> bits = m_TerminalStateByte;

	return bits.at(_pos);
}

void CRTUTerminal_State::SetBitValue(size_t _pos,bool _val /* = true */)
{
	std::bitset<8> bits = m_TerminalStateByte;
	bits.set(_pos,_val);

	m_TerminalStateByte = (BYTE)bits.to_ulong();
}

const CString CRTUTerminal_State::GetDescription()
{
	CString sResult = "",sTemp ="",sValue ="";
	int workMode =(GetBitValue(0) == true ? 1:0 ) +
				  (GetBitValue(1) == true ? 2:0 ) +
				  (GetBitValue(2) == true ? 4:0);
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

	//bitValue = GetBitValue(3);
	//sValue ="";
	//if (bitValue == true)
	//	sValue ="投入";
	//else
	//	sValue = "退出";

	//if (sValue != "")
	//{
	//	sTemp.Format("定值控制是否投入:%s\r\n",sValue);
	//	sResult += sTemp;
	//}
	//
	//bitValue = GetBitValue(4);
	//sValue = "";
	//if (bitValue == true)
	//	sValue = "退出";
	//else
	//	sValue = "启动";
	//if (sValue != "")
	//{
	//	sTemp.Format("水泵工作状态:%s\r\n",sValue);
	//	sResult += sTemp;
	//}

	//bitValue = GetBitValue(5);
	//sValue = "";
	//if (bitValue == true)
	//	sValue = "关闭";
	//else
	//	sValue = "开启";
	//if (sValue != "")
	//{
	//	sTemp.Format("终端箱门状态:%s\r\n",sValue);
	//	sResult += sTemp;
	//}

	bitValue = GetBitValue(6);
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

	return sResult;
}

bool CRTUAlarm_State::GetBitValue(size_t _pos)
{
	std::bitset<8> bits = m_AlarmStateByte;

	return bits.at(_pos);
}

void CRTUAlarm_State::SetBitValue(size_t _pos,bool _val /* = true */)
{
	std::bitset<8> bits = m_AlarmStateByte;
	bits.set(_pos,_val);

	m_AlarmStateByte = (BYTE)bits.to_ulong();
}

const CString CRTUAlarm_State::GetDescription()
{
	CString sResult = "",sTemp ="",sValue ="";
	bool BitValue = false;

	//BitValue = GetBitValue(0);
	//sValue = "";
	//if (BitValue == true)
	//	sValue = "停电";
	//else
	//	sValue = "正常";
	//if (sValue != "")
	//{
	//	sTemp.Format("交流电状态:%s\r\n",sValue);
	//	sResult += sTemp;
	//}

	BitValue = GetBitValue(1);
	sValue ="";
	if (BitValue == true)
		sValue = "电压低";
	else
		sValue = "正常";
	if (sValue != "")
	{
		sTemp.Format("蓄电池电压状态:%s\r\n",sValue);
		sResult += sTemp;
	}

	BitValue = GetBitValue(2);
	sValue = "";
	if (BitValue == true)
		sValue = "报警";
	else
		sValue = "正常";
	if (sValue != "")
	{
		sTemp.Format("水位超限报警状态:%s,",sValue);
		sResult += sTemp;
	}

	BitValue = GetBitValue(3);
	if (BitValue == true)
		sValue = "报警";
	else
		sValue = "正常";
	if (sValue != "")
	{
		sTemp.Format("水压超限报警状态:%s,",sValue);
		sResult += sTemp;
	}

	BitValue = GetBitValue(4);
	sValue = "";
	if (BitValue == true)
		sValue = "报警";
	else
		sValue = "正常";
	if (sValue != "")
	{
		sTemp.Format("水质超限报警状态:%s\r\n",sValue);
		sResult += sTemp;
	}

	//BitValue = GetBitValue(5);
	//sValue = "";
	//if (BitValue == true)
	//	sValue = "故障";
	//else
	//	sValue = "正常";
	//if (sValue != "")
	//{
	//	sTemp.Format("水表传感器状态:%s\r\n",sValue);
	//	sResult += sTemp;
	//}

	//BitValue = GetBitValue(6);
	//sValue = "";
	//if (BitValue == true)
	//	sValue = "故障";
	//else
	//	sValue = "正常";
	//if (sValue != "")
	//{
	//	sTemp.Format("水泵状态:%s\r\n",sValue);
	//	sResult += sTemp;
	//}

	//BitValue = GetBitValue(7);
	//sValue = "";
	//if (BitValue == true)
	//	sValue = "报警";
	//else
	//	sValue = "正常";
	//if (sValue != "")
	//{
	//	sTemp.Format("剩余水量的下限报警状态:%s\r\n",sValue);
	//	sResult += sTemp;
	//}

	return sResult;
}


bool CRTU_EXAlarm_State::GetBitValue(size_t _pos)
{
	std::bitset<8> bits = m_ExAlarmStateByte;

	return bits.at(_pos);
}


void CRTU_EXAlarm_State::SetBitValue(size_t _pos,bool _val /* = true */)
{
	std::bitset<8> bits = m_ExAlarmStateByte;

	bits.set(_pos,_val);
	m_ExAlarmStateByte = (BYTE)bits.to_ulong();
}

const CString CRTU_EXAlarm_State::GetDescription()
{
	CString sResult = "",sTemp ="",sValue = "";
    bool bitValue = false;

	bitValue = GetBitValue(0);
	if (bitValue == true)
		sValue ="报警";
	else
		sValue = "正常";
	if (sValue != "")
	{
		sTemp.Format("雨量报警状态:%s,",sValue);
		sResult += sTemp;
	}

	bitValue = GetBitValue(0);
	sValue = "";
	if (bitValue == true)
		sValue ="报警";
	else
		sValue = "正常";
	if (sValue != "")
	{
		sTemp.Format("模拟量报警状态:%s\r\n",sValue);
		sResult += sTemp;
	}

	return sResult;
}


CPumpGate_YK_PARA::CPumpGate_YK_PARA()
{
	m_YK_Byte = 0;
	m_PumpGate_Code = 0;
	m_PumpGete_Type = 0;
}

void CPumpGate_YK_PARA::SetYKByte(BYTE _ykByte)
{
	m_YK_Byte = _ykByte;
	m_PumpGate_Code = _ykByte & 0x0f;
	m_PumpGete_Type = ((_ykByte & 0xf0)>>4);
}

void CPumpGate_YK_PARA::SetPumpGate_Code(BYTE _Code)
{
	m_PumpGate_Code = _Code;

	m_YK_Byte = ((m_PumpGete_Type<<4) & 0xf0 )| (m_PumpGate_Code & 0x0f);

}

void CPumpGate_YK_PARA::SetPumpGate_Type(BYTE _Type)
{
	m_PumpGete_Type = _Type;

	m_YK_Byte = ((m_PumpGate_Code & 0x0f) | ((m_PumpGete_Type << 4) & 0xf0));
}


//CWaterQuality_PARA类
CWaterQuality_PARA::~CWaterQuality_PARA()
{
	Clear();
}

void CWaterQuality_PARA::Clear()
{
	if (m_mapWaterQulity.size() >0 )
	{
		std::map<BYTE,CWaterQuality_PARA_PER*>::iterator It;
		for (It = m_mapWaterQulity.begin(); It !=m_mapWaterQulity.end(); It ++)
		{
			if (It->second)
				delete It->second;
		}
		m_mapWaterQulity.clear();
	}
}

void CWaterQuality_PARA::BuildQualityMap()
{
	Clear();

	for (BYTE i = 0; i < 35; i ++)
	{
		CWaterQuality_PARA_PER* pObj = new CWaterQuality_PARA_PER();
		if (pObj)
		{
			//pObj->m_bCheck_FLAG = GetBitValue(i);
			pObj->m_BitPos = i;
			pObj->m_DataBits = water_q_ID[i].bCount;
			pObj->m_DownValue = 0.0;
			pObj->m_PointBits = water_q_ID[i].bPointCount;
			pObj->m_ScaleMax = water_q_ID[i].ScaleMax;
			pObj->m_ScaleMin = water_q_ID[i].ScaleMin;
			pObj->m_strName.Format("%s",water_q_ID[i].name);
			pObj->m_strUnit.Format("%s",water_q_ID[i].Units);
			pObj->m_UpValume = 0.0;
			if(i != 24)
				pObj->m_BUF_Cnt = 4;
			else
				pObj->m_BUF_Cnt = 5;

			m_mapWaterQulity.insert(std::pair<BYTE,CWaterQuality_PARA_PER*>(i,pObj));
		}
	}
}

int CWaterQuality_PARA::Get_Buffer_Count()
{
	int n_Buffer_Count = 5;
	for (BYTE i = 0; i< 40 ; i ++)
	{
		if (GetBitValue(i) == true)
		{
			CWaterQuality_PARA_PER* pObj = GetOneQualityObject(i);
			if (pObj != NULL)
			{
				n_Buffer_Count += pObj->m_BUF_Cnt;
			}
		}
	}

	return n_Buffer_Count;
}

void CWaterQuality_PARA::SetBitValue(size_t _pos, bool _val /* = true */)
{
	UINT64 tmpV = 0,SumV = 0;
	for (BYTE i = 0; i < 40; i++)
	{
		if (i == _pos)
			continue;

		if (GetBitValue(i) == true)
		{
			tmpV  = (UINT64)1<<i;
			SumV = SumV | tmpV;

		}
	}
     if (_val == true)
	 {
		 tmpV = ((UINT64) 1 << _pos);
		 SumV = (SumV |tmpV) ;
	 }

	m_wQualityValue = SumV;
}

bool CWaterQuality_PARA::GetBitValue(size_t _pos)
{
	UINT64 tmp = ((UINT64) 1 << _pos);

	return ( (m_wQualityValue & tmp) != 0 ?true:false);
}

CWaterQuality_PARA_PER* CWaterQuality_PARA::GetOneQualityObject(BYTE _bitPos)
{
	std::map<BYTE,CWaterQuality_PARA_PER*>::iterator it;
	it = m_mapWaterQulity.find(_bitPos);
	if (it != m_mapWaterQulity.end())
		return it->second;

	return NULL;
}

//CRtuStation类
CRtuStation::CRtuStation(void)
{
	m_pParent = NULL;
	m_sID = "";
	m_sName = "";
	m_nCode = 0;

	m_AddrA1		= 0;
	m_ProvinceCode	= 0;
	m_RegionCode	= 0;
	m_CountyCode    = 0;
	m_AddrA2		= 0;
	m_AddrA3		= 0;
	m_CmType		= 0;
	m_Interface		= 0;
	m_ComPortA		= "";
	m_BaundA		= 0;
	m_DataBitsA		= 7;
	m_StopBitsA		= 1;
	m_ParityA		= 0;
	m_IpAddrA		= "";
	m_SockPortA		= 0;
	m_strStCode     = "";
	m_strSYQADDR    = 0;
	m_centercode   =  0;
	m_nWriteUpdatePackCnt = 0;

	m_bSetTimePerDay = false;
	m_bSetTimeEnable = false;

	m_nAppProVer = 1;   //默认 1.0
	m_nAppProtoType = (BYTE)WaterResources_Protocol;
	m_nScFactoyType = SCFactoryType_GTDZ;
	m_nScAddress = 0;
	m_CurrUpdataPackCnt = 0;
	curr_it = m_pWriteUpdateList.begin();
	m_CurrentWriteUpdateOp = WriteUpdate_NoOp;

	m_RtuWorkMode	= 1;
	bIteratorVaild = false;
	bIsWriteUpdateStart = false;

	Init();
}

CRtuStation::CRtuStation(PRTU_STATION pInfo)
{
	m_pParent = NULL;

	m_sID			= pInfo->_strSId;
	m_sName			= pInfo->_strName;
	m_nCode			= pInfo->_nAddrA1;

	m_AddrA1		= pInfo->_nAddrA1;
	m_ProvinceCode  = pInfo->_bProvinceCode;
	m_RegionCode    = pInfo->_bRegionCode;
	m_CountyCode    = pInfo->_bCountyCode;
	m_AddrA2		= pInfo->_nAddrA2;
	m_AddrA3		= pInfo->_nAddrA3;
	m_CmType		= pInfo->_bCmType;
	m_Interface		= pInfo->_bInterface;
	m_ComPortA		= pInfo->_CmComInfo.mComName;
	m_BaundA		= pInfo->_CmComInfo.mBaund;
	m_DataBitsA		= pInfo->_CmComInfo.mDataBits;
	m_StopBitsA		= pInfo->_CmComInfo.mStopBits;
	m_ParityA		= pInfo->_CmComInfo.mParity;
	m_IpAddrA		= pInfo->_CmNetInfo.mIpAddress;
	m_SockPortA		= pInfo->_CmNetInfo.mPort;

	m_nAppProVer = pInfo->_nAppProVer;   //默认 1.0
	m_nAppProtoType = pInfo->_bAppProType;
	m_nScFactoyType = pInfo->_CmScCommInfo.nFactoryType;
	m_nScAddress = pInfo->_CmScCommInfo.nScAddress;
	m_RtuWorkMode = pInfo->_nWorkMode;
	m_strStCode = pInfo->_strStCode;
	m_strSYQADDR = pInfo->_strSYQADDR;
	m_centercode = pInfo->_centercode;
	m_iPassword  = pInfo->_passWord;
	m_CurrentWriteUpdateOp = WriteUpdate_NoOp;

	m_bSetTimePerDay = false;
	m_bSetTimeEnable = false;
	m_CurrUpdataPackCnt = 0;
	curr_it = m_pWriteUpdateList.begin();
	bIteratorVaild = false;
	bIsWriteUpdateStart = false;

	Init();
}

void CRtuStation::Init()
{
	if (m_Interface == Serial_CommInterface)
		m_bCommState = CommUnknown_State;
	else
		m_bCommState = Disconnect_State;

	
	m_RtuLinkCheckState = 0xF0;

	m_bRESET_Cmd = 0;
	m_QueryRealData_Cmd = 0;
	m_YK_OP_Result =  NULL;

	m_blRecvFLAG = false;

	m_pDatabase = NULL;

	m_pHandle_Com = NULL;
	m_pHandle_Net = NULL;
	m_pHandle_TcpClient = NULL;
	m_pScMessageDealModule = NULL;
	m_ProtocolType = UDP_Protocol;
	m_SysDataTrans.SetRtuStation(this);
	m_SysDataTrans_Hydr.SetRtuStation(this);
	m_SysDataTrans_Syq.SetRtuStation(this);

	InitReSendInterval();
	m_WaterQualityPara.BuildQualityMap();
	BuildEventRecordsList();
	GetLocalTime(&m_LastRecvTime);


}

CRtuStation::~CRtuStation(void)
{
	Clear();
	ClearCommHandle();//清除通信接口相关的信息

}

//设置重复报文时间间隔
bool CRtuStation::InitReSendInterval()
{
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	if (pApp)
	{
		m_nReSendInterval = pApp->m_GlobalOptions.m_ResendInterval;
	}
	else
		m_nReSendInterval = 10;
	return true;
}

void CRtuStation::SetParent(CRealTimeDB* _pParent)
{
	m_pParent	= _pParent;
	m_pDatabase = _pParent->GetDatabase();
}


CRealTimeDB* CRtuStation::GetParent()
{
	return m_pParent;
}

CPtrList* CRtuStation::GetYcObjectsList()
{
	return &m_lstYcObjects;
}

CPtrList* CRtuStation::GetYxObjectsList()
{
	return &m_lstYxObjects;
}

CPtrList* CRtuStation::GetYkObjectsList()
{
	return &m_lstYkObjects;
}

CPtrList* CRtuStation::GetZdObjectsList()
{
	return &m_lstZdObjects;
}


bool CRtuStation::BuildAllData(CDatabase* pDb)
{
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();

	bool blBuildYc = false,
		 blBuildYx = false,
		 blBuildYk = false,
		 blBuildZd = false;

	BuildExtInformation(pDb);

	if (pApp->m_GlobalOptions.m_RtuOpts.blYcList_FLAG != FALSE)
		blBuildYc = BuildYcObjects(pDb);

	if (pApp->m_GlobalOptions.m_RtuOpts.blYxList_FLAG != FALSE)
		blBuildYx = BuildYxObjects(pDb);

	if (pApp->m_GlobalOptions.m_RtuOpts.blYkList_FLAG != FALSE)
		blBuildYk =	BuildYkObjects(pDb);

	if (pApp->m_GlobalOptions.m_RtuOpts.blZdList_FLAG != FALSE)
		blBuildZd = BuildZdObjects(pDb);

	return (blBuildYc && blBuildYx && blBuildYk && blBuildZd);
}


//************************************
// Method:    ClearYcList
// FullName:  CRtuStation::ClearYcList
// Access:    public 
// Returns:   void
// Qualifier: 清空遥测列表，并清除内存中的数据信息
//************************************
void CRtuStation::ClearYcList()
{
	while(!m_lstYcObjects.IsEmpty())
	{
		CRtuYcObject* pObj = (CRtuYcObject*)m_lstYcObjects.RemoveHead();
		if (pObj)
		{
			delete pObj;
			pObj  = NULL;
		}
	}

}

//************************************
// Method:    ClearYxList
// FullName:  CRtuStation::ClearYxList
// Access:    public 
// Returns:   void
// Qualifier: 清空遥信列表，并清除内存中的数据信息
//************************************
void CRtuStation::ClearYxList()
{

	while (!m_lstYxObjects.IsEmpty())
	{
		CRtuYxObject* pObj = (CRtuYxObject*)m_lstYxObjects.RemoveHead();
		if (pObj)
		{
			delete pObj;
			pObj = NULL;
		}
	}

}


//************************************
// Method:    ClearYkList
// FullName:  CRtuStation::ClearYkList
// Access:    public 
// Returns:   void
// Qualifier: 清空遥控列表，并清除内存中存储的遥控数据信息
//************************************
void CRtuStation::ClearYkList()
{

	while (!m_lstYkObjects.IsEmpty())
	{
		CRtuYkObject* pObj = (CRtuYkObject*)m_lstYkObjects.RemoveHead();
		if (pObj)
		{
			delete pObj;
			pObj = NULL;
		}
	}

}


//************************************
// Method:    ClearZdList
// FullName:  CRtuStation::ClearZdList
// Access:    public 
// Returns:   void
// Qualifier: 清空定值列表，并清除内存中存储的定值的数据信息
//************************************
void CRtuStation::ClearZdList()
{
	while (!m_lstZdObjects.IsEmpty())
	{
		CRtuZdObject* pObj = (CRtuZdObject*)m_lstZdObjects.RemoveHead();
		{
			if (pObj)
			{
				delete pObj;
				pObj = NULL;
			}
		}
	}
}



void CRtuStation::ClearRainAlartParaList()
{
	while (!m_lstRainAlertParas.IsEmpty())
	{
		CRainAlert_PARA_PER* pObj = m_lstRainAlertParas.RemoveHead();
		if (pObj)
		{
			delete pObj;
			pObj = NULL;
		}
	}
}

void CRtuStation::ClearAnalogLimitParaList()
{
	while (!m_lstAnalogLimitParas.IsEmpty())
	{
		CAnalogLimit_PARA_PER* pAnalog_PARA = m_lstAnalogLimitParas.RemoveHead();
		if (pAnalog_PARA)
		{
			delete pAnalog_PARA;
			pAnalog_PARA = NULL;
		}
	}
}

void CRtuStation::ClearWaterLevelParaList()
{
	while (!m_lstWaterLevelParas.IsEmpty())
	{
		CWaterLevel_PAR_PER* pWaterLevel_PARA = m_lstWaterLevelParas.RemoveHead();
		if(pWaterLevel_PARA)
		{
			delete pWaterLevel_PARA;
			pWaterLevel_PARA = NULL;
		}
	}
}



void CRtuStation::ClearWaterPressureParaList()
{
	while (!m_lstWaterPressureParas.IsEmpty())
	{
		CWaterPressure_PARA_PER* pWaterPressure_PARA = m_lstWaterPressureParas.RemoveHead();
		if (pWaterPressure_PARA)
		{
			delete pWaterPressure_PARA;
			pWaterPressure_PARA = NULL;
		}
	}
}

void CRtuStation::ClearFlowInitValumeParaList()
{
	while (!m_lstFlowInitValumeParas.IsEmpty())
	{
		CFlowInitValume_PARA_PER* pFlowInit_PARA = m_lstFlowInitValumeParas.RemoveHead();
		if (pFlowInit_PARA)
		{
			delete pFlowInit_PARA;
			pFlowInit_PARA = NULL;
		}
	}
}

void CRtuStation::ClearWriteUpdateList()
{
	m_cs_writeUpdate.Lock();
	while (m_pWriteUpdateList.begin() != m_pWriteUpdateList.end())
	{
		delete (*m_pWriteUpdateList.begin());
		m_pWriteUpdateList.erase(m_pWriteUpdateList.begin());
	}
	m_cs_writeUpdate.Unlock();
}


void CRtuStation::Clear()
{
	ClearYcList();
	ClearYxList();
	ClearYkList();
	ClearZdList();
	ClearRainAlartParaList();
	ClearAnalogLimitParaList();
	ClearWaterLevelParaList();
	ClearWaterPressureParaList();
	ClearFlowInitValumeParaList();
	ClearEventRecordsList();

	ClearDataList();
	ClearLogList();
	ClearCommList();

	ClearWriteUpdateList();

	m_syqdatadiscribelist.clear();
}


CRtuYcObject* CRtuStation::SearchYcObject(WORD _Code)
{
	CRtuYcObject* pObj = NULL;
	POSITION rPos = m_lstYcObjects.GetHeadPosition();
	while (rPos != NULL)
	{
		CRtuYcObject* _tmppObj = (CRtuYcObject*)m_lstYcObjects.GetNext(rPos);
		if (_tmppObj && _tmppObj->GetCode() == _Code)
		{
			pObj = _tmppObj;
			break;
		}
	}
	return pObj;
}

CRtuYcObject* CRtuStation::SearchYcObject(CString _ycID)
{
	CRtuYcObject* pObj = NULL;
	POSITION rPos = m_lstYcObjects.GetHeadPosition();
	while (rPos != NULL)
	{
	  CRtuYcObject* _tmppObj = (CRtuYcObject*)m_lstYcObjects.GetNext(rPos);
	  if (_tmppObj && _tmppObj->GetID() == _ycID)
	  {
		  pObj = _tmppObj;
		  break;
	  }
	}

	return pObj;
}

CRtuYxObject* CRtuStation::SearchYxObject(WORD _Code)
{
	CRtuYxObject* pObj = NULL;
	POSITION rPos = m_lstYxObjects.GetHeadPosition();
	while (rPos != NULL)
	{
		CRtuYxObject* _tmppObj = (CRtuYxObject*)m_lstYxObjects.GetNext(rPos);
		if (_tmppObj && _tmppObj->GetCode() == _Code)
		{
			pObj = _tmppObj;
			break;
		}
	}
	return pObj;
}

CRtuYxObject* CRtuStation::SearchYxObject(CString _yxID)
{
	CRtuYxObject* pObj = NULL;

	POSITION rPos = m_lstYxObjects.GetHeadPosition();
	while (rPos != NULL)
	{
		CRtuYxObject* _tmppObj = (CRtuYxObject*)m_lstYxObjects.GetNext(rPos);
		if (_tmppObj && _tmppObj->GetID() == _yxID)
		{
			pObj = _tmppObj;
			break;
		}
	}
	return pObj;
}

CRtuYkObject* CRtuStation::SearchYkObject(WORD _Code)
{
	CRtuYkObject* pObj= NULL;
	POSITION rPos = m_lstYkObjects.GetHeadPosition();
	while (rPos != NULL)
	{
		CRtuYkObject* _tmppObj = (CRtuYkObject*)m_lstYkObjects.GetNext(rPos);
		if (_tmppObj && _tmppObj->GetCode() == _Code)
		{
			pObj = _tmppObj;
			break;
		}
	}

	return pObj;
}

CRtuYkObject* CRtuStation::SearchYkObject(CString _ykID)
{
	CRtuYkObject* pObj= NULL;

	POSITION rPos = m_lstYkObjects.GetHeadPosition();
	while (rPos != NULL)
	{
		CRtuYkObject* _tmppObj = (CRtuYkObject*)m_lstYkObjects.GetNext(rPos);
		if (_tmppObj && _tmppObj->GetID() == _ykID)
		{
			pObj = _tmppObj;
			break;
		}
	}

	return pObj;
}

CRtuZdObject* CRtuStation::SearchZdObject(WORD _Code)
{
	CRtuZdObject* pObj = NULL;
	POSITION rPos = m_lstZdObjects.GetHeadPosition();
	while (rPos != NULL)
	{
		CRtuZdObject* _tmppObj = (CRtuZdObject*)m_lstZdObjects.GetNext(rPos);
		if (_tmppObj && _tmppObj->GetCode() == _Code)
		{
			pObj = _tmppObj;
			break;
		}
	}
	
	return pObj;
}

CRtuZdObject* CRtuStation::SearchZdObject(CString _zdID)
{
	CRtuZdObject* pObj = NULL;

	POSITION rPos = m_lstZdObjects.GetHeadPosition();
	while (rPos != NULL)
	{
		CRtuZdObject* _tmppObj = (CRtuZdObject*)m_lstZdObjects.GetNext(rPos);
		if (_tmppObj && _tmppObj->GetID() == _zdID)
		{
			pObj = _tmppObj;
			break;
		}
	}

	return pObj;
}


bool CRtuStation::BuildYxObjects(CDatabase* pDb)
{
	bool _blResult = true;

	CString sLogMessage ="";
	CString sSQL ="";
	try
	{
		ClearYxList();

		sSQL.Format("SELECT DISTINCT sName,sID,yxObjID,yxCode FROM rtuYxObject WHERE sID = '%s' ORDER BY sName ASC",m_sID);

		if (GetParent())
		{
			sLogMessage.Format("配置数据库操作,SQL:%s",sSQL);
			GetParent()->InsertLog(SYS_OTHER_RUN_LOG,sLogMessage,Normal_Log_Message);
		}

		CRecordset rs(pDb);
		rs.Open(CRecordset::forwardOnly,sSQL);

		CString sTemp = "";

		while (!rs.IsEOF())
		{
			sTemp.Empty();
			
			RTU_YXOBJECT info;

			rs.GetFieldValue("sName",sTemp);
			info._strName = sTemp;

			rs.GetFieldValue("sID",sTemp);
			info._strSId = m_sID;

			rs.GetFieldValue("yxObjID",sTemp);
			info._strYxObjID = sTemp;

			rs.GetFieldValue("yxCode",sTemp);
			info._nYxCode = atoi(sTemp);

			CRtuYxObject* pObj = new CRtuYxObject(&info);
			if (pObj)
			{
				pObj->SetParent(this);
				m_lstYxObjects.AddTail((void*)pObj);
			}

			rs.MoveNext();
		}

		rs.Close();
	}
	catch (CDBException* pE)
	{
		TRACE("BuildYxObjects Error:%s\r\n",pE->m_strError);
		if (GetParent())
		{
			sLogMessage.Format("配置数据库操作,SQL:%s,Error:%s",sSQL,pE->m_strError);
			GetParent()->InsertLog(SYS_RUNNING_S_LOG,sLogMessage,Error_Log_message);
		}

		pE->Delete();
		return false;
	}

	return _blResult;
}

bool CRtuStation::BuildYcObjects(CDatabase* pDb)
{
	bool _blResult = true;

	CString sLogMessage ="";
	CString sSQL ="";
	try
	{

		ClearYcList();
		sSQL.Format("SELECT DISTINCT sName,sID,ycObjID,ycCode,xType,DisplayBits,PointBits,Unit,MaxValue,MinValue FROM rtuYcObject WHERE sID = '%s' ORDER BY sName ASC",m_sID);

		if (GetParent())
		{
			sLogMessage.Format("配置数据库操作,SQL:%s",sSQL);
			GetParent()->InsertLog(SYS_OTHER_RUN_LOG,sLogMessage,Normal_Log_Message);
		}

		CRecordset rs(pDb);

		rs.Open(CRecordset::forwardOnly,sSQL);

		CString sTemp="";
		while (!rs.IsEOF())
		{
			sTemp.Empty();
			RTU_YCOBJECT info;

			rs.GetFieldValue("sName",sTemp);
			info._strName = sTemp;

			rs.GetFieldValue("sID",sTemp);
			info._strSId = m_sID;

			rs.GetFieldValue("ycObjID",sTemp);
			info._strYcObjID = sTemp;

			rs.GetFieldValue("ycCode",sTemp);
			info._nYcCode = atoi(sTemp);

			rs.GetFieldValue("xType",sTemp);
			info._Type = (BYTE)atoi(sTemp);

			rs.GetFieldValue("DisplayBits",sTemp);
			info._DisplayBits = (BYTE)atoi(sTemp);

			rs.GetFieldValue("PointBits",sTemp);
			info._PointBits = (BYTE)atoi(sTemp);

			rs.GetFieldValue("Unit",sTemp);
			info._Unit = sTemp;

			rs.GetFieldValue("MaxValue",sTemp);
			info._MaxValue = (float)atof(sTemp);

			rs.GetFieldValue("MinValue",sTemp);
			info._MinValue = (float)atof(sTemp);

			CRtuYcObject* pObj =  new CRtuYcObject(&info);
			if (pObj)
			{
				pObj->SetParent(this);
				m_lstYcObjects.AddHead((void*)pObj);
			}

			rs.MoveNext();
		}
		rs.Close();

	}
	catch (CDBException* pE)
	{
		TRACE("BuildYcObjects Error:%s\r\n",pE->m_strError);
		if (GetParent())
		{
			sLogMessage.Format("配置数据库操作,SQL:%s,Error:%s",sSQL,pE->m_strError);
			GetParent()->InsertLog(SYS_RUNNING_S_LOG,sLogMessage,Error_Log_message);
		}
		pE->Delete();
		return false;
	}

	return _blResult;
}

bool CRtuStation::BuildYkObjects(CDatabase* pDb)
{
	bool _blResult = true;

	CString sLogMessage = "";
	CString sSQL = "";
	try
	{
		ClearYkList();
		
		sSQL.Format("SELECT DISTINCT sName,sID,ykObjID,ykCode FROM rtuYkObject WHERE sID = '%s' ORDER BY sName ASC",m_sID);

		if (GetParent())
		{
			sLogMessage.Format("配置数据库操作,SQL:%s",sSQL);
			GetParent()->InsertLog(SYS_OTHER_RUN_LOG,sLogMessage,Normal_Log_Message);
		}

		CRecordset rs (pDb);

		rs.Open(CRecordset::forwardOnly,sSQL);
		CString sTemp;

		while (!rs.IsEOF())
		{
			RTU_YKOBJECT info;
			
			sTemp.Empty();
			rs.GetFieldValue("sName",sTemp);
			info._strName = sTemp;

			rs.GetFieldValue("sID",sTemp);
			info._strSId = m_sID;

			rs.GetFieldValue("ykObjID",sTemp);
			info._strYkObjID = sTemp;

			rs.GetFieldValue("ykCode",sTemp);
			info._nYkCode = atoi(sTemp);

			CRtuYkObject* pObj = new CRtuYkObject(&info);
			if (pObj)
			{
				pObj->SetParent(this);
				m_lstYkObjects.AddTail((void*)pObj);

			}

			rs.MoveNext();
		}

		rs.Close();

	}
	catch (CDBException* pE)
	{
		TRACE("BuildYkObjects Error:%s\r\n",pE->m_strError);
		if (GetParent())
		{
			sLogMessage.Format("配置数据库操作,SQL:%s,Error:%s",sSQL,pE->m_strError);
			GetParent()->InsertLog(SYS_RUNNING_S_LOG,sLogMessage,Error_Log_message);
		}

		pE->Delete();
		return false;
	}

	return _blResult;
}

bool CRtuStation::BuildZdObjects(CDatabase* pDb)
{

	bool _blResult = true;

	CString sLogMessage ="";
	CString sSQL = "";
	try
	{

		ClearZdList();
		
		sSQL.Format("SELECT DISTINCT sName,sID,ZdObjID,ZdCode,sizeof,OrderInItem,Unit,ScaleMax,ScaleMin FROM rtuZdObject WHERE sID = '%s' ORDER BY sName ASC",m_sID);

		if (GetParent())
		{
			sLogMessage.Format("配置数据库操作,SQL:%s",sSQL);
			GetParent()->InsertLog(SYS_OTHER_RUN_LOG,sLogMessage,Normal_Log_Message);
		}

		CRecordset rs(pDb);
		rs.Open(CRecordset::forwardOnly,sSQL);
		CString sTemp = "";
		while (!rs.IsEOF())
		{
			sTemp.Empty();
			RTU_ZDOBJECT info;

			rs.GetFieldValue("sName",sTemp);
			info._strName = sTemp;

			rs.GetFieldValue("sID",sTemp);
			info._strSId = m_sID;

			rs.GetFieldValue("ZdObjID",sTemp);
			info._strZdObjID = sTemp;

			rs.GetFieldValue("ZdCode",sTemp);
			info._nZDCode = atoi(sTemp);

			rs.GetFieldValue("sizeof",sTemp);
			info._Sizeof = (BYTE)atoi(sTemp);

			rs.GetFieldValue("OrderInItem",sTemp);
			info._OrderInItem = (BYTE)atoi(sTemp);

			rs.GetFieldValue("Unit",sTemp);
			info._Unit = sTemp;

			rs.GetFieldValue("ScaleMax",sTemp);
			info._ScaleMax = (float)atof(sTemp);

			rs.GetFieldValue("ScaleMin",sTemp);
			info._ScaleMin = (float)atof(sTemp);

			CRtuZdObject* pObj = new CRtuZdObject(&info);
			if (pObj)
			{
				pObj->SetParent(this);
				m_lstZdObjects.AddTail((void*)pObj);
			}


			rs.MoveNext();
		}
		rs.Close();

	}
	catch (CDBException* pE)
	{
		TRACE("BuildYkObjects Error:%s\r\n",pE->m_strError);

		if (GetParent())
		{
			sLogMessage.Format("配置数据库操作,SQL:%s,Error:%s",sSQL,pE->m_strError);
			GetParent()->InsertLog(SYS_RUNNING_S_LOG,sLogMessage,Error_Log_message);
		}

		pE->Delete();
		return false;
	}

	return _blResult;
}


bool CRtuStation::BuildExtInformation(CDatabase* pDb)
{
	bool _blResult = true;

	CString sLogMessage = "";
	CString sSQL = "";
	try
	{
		sSQL.Format("SELECT pVer, pType, scAddr,scFactroyType,WorkMode FROM rtuStation_ext WHERE sID='%s'",m_sID);
		CRecordset rs(pDb);

		rs.Open(CRecordset::forwardOnly,sSQL);
		CString strTemp = "";
		bool blEmpty = true;
		if (!rs.IsEOF())
		{
			rs.GetFieldValue("pVer",strTemp);
			m_nAppProVer = (BYTE)atoi(strTemp);

			rs.GetFieldValue("scAddr",strTemp);
			m_nScAddress = (UINT32)atol(strTemp);

			rs.GetFieldValue("scFactroyType",strTemp);
			m_nScFactoyType = atoi(strTemp);

			rs.GetFieldValue("WorkMode",strTemp);
			m_RtuWorkMode = (BYTE)atoi(strTemp);

			//dql add
			rs.GetFieldValue("AppProtocolType", strTemp);
			m_nAppProtoType = (BYTE)atoi(strTemp);

			blEmpty = false;
		}
		rs.Close();

		if (blEmpty)
		{
			sSQL.Format("INSERT INTO rtuStation_ext(sID,pVer,pType, scAddr,scFactroyType,WorkMode) VALUES ('%s',%d, %d, %d,%d,%d)",
				m_sID,m_nAppProVer, m_nAppProtoType, m_nScAddress,m_nScFactoyType,m_RtuWorkMode);
			
			pDb->ExecuteSQL(sSQL);
		}

	}
	catch (CDBException* pEx)
	{
		TRACE("%s\r\n",pEx->m_strError);
		pEx->Delete();
		GETAPP()->m_pReadDB->CreatertuStation_extTable();

		return false;
	}
	return _blResult;
}


bool CRtuStation::CheckYcObject(WORD ycCode,CRtuYcObject* pCurYc /* = NULL */)
{
	if (m_lstYcObjects.IsEmpty())
		return false;

	if (!pCurYc)
	{
		CRtuYcObject* pTempYc = SearchYcObject(ycCode) ;
		if (pTempYc == NULL)
			return false;
		else
			return true;
	}
	else if (pCurYc != NULL)
	{
		POSITION rPos = m_lstYcObjects.GetHeadPosition();
		while ( rPos != NULL)
		{
			CRtuYcObject* pTempYc = (CRtuYcObject*)m_lstYcObjects.GetNext(rPos);
			if (pTempYc && pTempYc != pCurYc && pCurYc->GetCode() == ycCode)
			{
				return true;
			}
		}
	}

	return false;
}

bool CRtuStation::CheckYxObject(WORD yxCode,CRtuYxObject* pCurYx /* = NULL */)
{
	if (m_lstYxObjects.IsEmpty())
		return false;
	if (!pCurYx)
	{
		CRtuYxObject* pTempYx = SearchYxObject(yxCode);
		if (pTempYx == NULL)
			return false;
		else
			return true;
	}
	else if ( pCurYx != NULL)
	{
		POSITION rPos = m_lstYxObjects.GetHeadPosition();
		while (rPos != NULL)
		{
			CRtuYxObject* pTempYx = (CRtuYxObject*)m_lstYxObjects.GetNext(rPos);
			if (pTempYx && pTempYx != pCurYx && pTempYx->GetCode() == yxCode)
			{
				return true;
			}
		}
	}
	return false;
}

bool CRtuStation::CheckYkObject(WORD ykCode,CRtuYkObject* pCurYk /* = NULL */)
{
	if (m_lstYkObjects.IsEmpty())
		return false;

	if (!pCurYk)
	{
		CRtuYkObject* pTempYk = SearchYkObject(ykCode);
		if (pTempYk == NULL)
			return false;
		else
			return true;
	}
	else if (pCurYk != NULL)
	{
		POSITION rPos = m_lstYkObjects.GetHeadPosition();

		while (rPos != NULL)
		{
			CRtuYkObject* pTempYk = (CRtuYkObject*)m_lstYkObjects.GetNext(rPos);
			if (pTempYk && pTempYk != pCurYk && pTempYk->GetCode() == ykCode)
			{
				return true;
			}
		}
	}
	return false;
}


bool CRtuStation::CheckZdObject(WORD zdCode,CRtuZdObject* pCurZd /* = NULL */)
{
	if (m_lstZdObjects.IsEmpty())
		return false;

	if (!pCurZd)
	{
		CRtuZdObject* pTempZd = SearchZdObject(zdCode);
		if (pTempZd == NULL)
			return false;
		else
			return true;
	}
	else if (pCurZd != NULL)
	{
		POSITION rPos = m_lstZdObjects.GetHeadPosition();

		while ( rPos != NULL )
		{
			CRtuZdObject* pTempZd = (CRtuZdObject*)m_lstZdObjects.GetNext(rPos);
			if (pTempZd && pTempZd != pCurZd && pTempZd->GetCode() == zdCode)
				return true;
		}
	}

	return false;
}


bool CRtuStation::UpdateRealDBYc(CString ycID,DB_RECORD_OPERATION_TYPE opType)
{
	if (ycID.IsEmpty())
		return false;

	switch (opType)
	{
	case  RECORD_NEW:
		{
			CRtuYcObject* pYcNew = QueryYcInDB(ycID);
			return AddRealDBYc(pYcNew);
		}
		break;
	case  RECORD_EDIT:
		{
			bool ret = DeleteRealDBYc(ycID);
			if (ret)
			{
				CRtuYcObject* pYcNew = QueryYcInDB(ycID);
				return AddRealDBYc(pYcNew);

			}
			else
				return false;
		}
		break;
	case  RECORD_DEL:
		{
			return DeleteRealDBYc(ycID);
		}
		break;
	}

	return true;
}

bool CRtuStation::UpdateRealDBYx(CString yxID,DB_RECORD_OPERATION_TYPE opType)
{
	if(yxID.IsEmpty())
		return false;

	switch (opType)
	{
	case  RECORD_NEW:
		{
			CRtuYxObject* pYxNew = QueryYxInDB(yxID);
			return AddRealDBYx(pYxNew);
		}
		break;
	case  RECORD_EDIT:
		{
			bool ret = DeleteRealDBYx(yxID);
			if (ret)
			{
				CRtuYxObject* pYxNew = QueryYxInDB(yxID);
				return AddRealDBYx(pYxNew);
			}
			else 
				return false;
		}
		break;
	case  RECORD_DEL:
		{
			return DeleteRealDBYx(yxID);
		}
		break;
	}

	return true;
}

bool CRtuStation::UpdateRealDBYk(CString ykID,DB_RECORD_OPERATION_TYPE opType)
{
	if(ykID.IsEmpty())
		return false;

	switch (opType)
	{
	case  RECORD_NEW:
		{
			CRtuYkObject* pYkNew = QueryYKInDB(ykID);
			return AddRealDBYk(pYkNew);
		}
		break;
	case  RECORD_EDIT:
		{
			bool ret = DeleteRealDBYk(ykID);
			if(ret)
			{
				CRtuYkObject* pYkNew = QueryYKInDB(ykID);
				return AddRealDBYk(pYkNew);
			}
			else
				return false;
		}
		break;
	case  RECORD_DEL:
		{
			return DeleteRealDBYk(ykID);
		}
		break;
	}

	return true;
}

bool CRtuStation::UpdateRealDBZd(CString zdID,DB_RECORD_OPERATION_TYPE opType)
{
	if(zdID.IsEmpty())
		return false;

	switch (opType)
	{
	case  RECORD_NEW:
		{
			CRtuZdObject* pZdNew = QueryZdInDB(zdID);
			return AddRealDBZd(pZdNew);
		}
		break;
	case  RECORD_EDIT:
		{
			bool ret = DeleteRealDBZd(zdID);
			if (ret)
			{
				CRtuZdObject* pZdNew = QueryZdInDB(zdID);
				return AddRealDBZd(pZdNew);
			}
			else
				return false;
		}
		break;
	case  RECORD_DEL:
		{
			return DeleteRealDBZd(zdID);
		}
		break;
	}

	return true;
}


bool CRtuStation::DeleteRealDBYc(CString ycID)
{
	POSITION rPos = m_lstYcObjects.GetHeadPosition();
	bool blFinded = false;

	CRtuYcObject* pYcDel = NULL;  //需要删除的遥测对象
	while (rPos != NULL)
	{
		CRtuYcObject* pYc = (CRtuYcObject*)m_lstYcObjects.GetAt(rPos);
		if (pYc && pYc->GetID() == ycID)
		{
			blFinded = true;
			pYcDel = pYc;
			break;
		}

		m_lstYcObjects.GetNext(rPos);
	}

	if (blFinded && rPos != NULL)
	{
		m_lstYcObjects.RemoveAt(rPos);
		if (pYcDel)
		{
			delete pYcDel;
			pYcDel = NULL;
		}

	}

	return true;
}

bool CRtuStation::DeleteRealDBYx(CString yxID)
{
	POSITION rPos = m_lstYxObjects.GetHeadPosition();
	bool blFinded = false;

	CRtuYxObject* pYxDel = NULL;
	while (rPos != NULL)
	{
		CRtuYxObject* pYx = (CRtuYxObject*)m_lstYxObjects.GetAt(rPos);
		if (pYx && pYx->GetID() == yxID)
		{
			blFinded = true;
			pYxDel = pYx;
			break;
		}
		m_lstYxObjects.GetNext(rPos);
	}

	if (blFinded && rPos != NULL)
	{
		m_lstYxObjects.RemoveAt(rPos);
		if (pYxDel)
		{
			delete pYxDel;
			pYxDel = NULL;
		}
	}

	return true;

}

bool CRtuStation::DeleteRealDBYk(CString ykID)
{
	POSITION rPos = m_lstYkObjects.GetHeadPosition();
	bool blFinded = false;

	CRtuYkObject* pYkDel = NULL;
	while (rPos != NULL)
	{
		CRtuYkObject* pYk = (CRtuYkObject*)m_lstYkObjects.GetAt(rPos);
		if (pYk && pYk->GetID() == ykID)
		{
			blFinded = true;
			pYkDel = pYk;
			break;
		}
		m_lstYkObjects.GetNext(rPos);
	}


	if (blFinded && rPos != NULL)
	{
		m_lstYkObjects.RemoveAt(rPos);
		if (pYkDel)
		{
			delete pYkDel;
			pYkDel = NULL;
		}
	}

	return true;
}

bool CRtuStation::DeleteRealDBZd(CString zdID)
{
	POSITION rPos = m_lstZdObjects.GetHeadPosition();
	bool blFinded = false;
	
	CRtuZdObject* pZdDel = NULL;
	while (NULL != rPos)
	{
		CRtuZdObject* pZd = (CRtuZdObject*)m_lstZdObjects.GetAt(rPos);
		if (pZd && pZd->GetID() == zdID)
		{
			blFinded = true;
			pZdDel = pZd;
			break;
		}
		m_lstZdObjects.GetNext(rPos);
	}

	if (blFinded  && rPos != NULL)
	{
		m_lstZdObjects.RemoveAt(rPos);
		if (pZdDel)
		{
			delete pZdDel;
			pZdDel = NULL;
		}
	}
	return true;
}


CRtuYcObject* CRtuStation::QueryYcInDB(CString ycID)
{
	CRtuYcObject *pYcNew = NULL;

	CString sLogMessage ="";
	CString sSQL ="";
	try
	{
		sSQL.Format("SELECT DISTINCT sName,sID,ycObjID,ycCode,xType,DisplayBits,PointBits,Unit,MaxValue,MinValue FROM rtuYcObject WHERE sID = '%s' AND ycObjID='%s'",m_sID,ycID);

		if (GetParent())
		{
			sLogMessage.Format("配置数据库操作,SQL:%s",sSQL);
			GetParent()->InsertLog(SYS_OTHER_RUN_LOG,sLogMessage,Normal_Log_Message);
		}

		CRecordset rs(m_pDatabase);

		rs.Open(CRecordset::forwardOnly,sSQL);

		CString sTemp="";
		if (!rs.IsEOF())
		{
			sTemp.Empty();
			RTU_YCOBJECT info;

			rs.GetFieldValue("sName",sTemp);
			info._strName = sTemp;

			rs.GetFieldValue("sID",sTemp);
			info._strSId = m_sID;

			rs.GetFieldValue("ycObjID",sTemp);
			info._strYcObjID = sTemp;

			rs.GetFieldValue("ycCode",sTemp);
			info._nYcCode = atoi(sTemp);

			rs.GetFieldValue("xType",sTemp);
			info._Type = (BYTE)atoi(sTemp);

			rs.GetFieldValue("DisplayBits",sTemp);
			info._DisplayBits = (BYTE)atoi(sTemp);


			rs.GetFieldValue("PointBits",sTemp);
			info._PointBits = (BYTE)atoi(sTemp);


			rs.GetFieldValue("Unit",sTemp);
			info._Unit = sTemp;

			rs.GetFieldValue("MaxValue",sTemp);
			info._MaxValue = (float)atof(sTemp);


			rs.GetFieldValue("MinValue",sTemp);
			info._MinValue = (float)atof(sTemp);


			pYcNew =  new CRtuYcObject(&info);
			if (pYcNew)
				pYcNew->SetParent(this);
	
		}
		rs.Close();

		return pYcNew;
	}
	catch (CDBException* pE)
	{
		TRACE("BuildYcObjects Error:%s\r\n",pE->m_strError);
		if (GetParent())
		{
			sLogMessage.Format("配置数据库操作,SQL:%s,Error:%s",sSQL,pE->m_strError);
			GetParent()->InsertLog(SYS_RUNNING_S_LOG,sLogMessage,Error_Log_message);
		}
		pE->Delete();
		return NULL;
	}

	return pYcNew;
}

CRtuYxObject* CRtuStation::QueryYxInDB(CString yxID)
{
	CRtuYxObject* pYxNew = NULL;

	CString sLogMessage ="";
	CString sSQL ="";
	try
	{
		sSQL.Format("SELECT DISTINCT sName,sID,yxObjID,yxCode FROM rtuYxObject WHERE sID = '%s' AND yxObjID='%s'",m_sID,yxID);

		if (GetParent())
		{
			sLogMessage.Format("配置数据库操作,SQL:%s",sSQL);
			GetParent()->InsertLog(SYS_OTHER_RUN_LOG,sLogMessage,Normal_Log_Message);
		}

		CRecordset rs(m_pDatabase);
		rs.Open(CRecordset::forwardOnly,sSQL);

		CString sTemp = "";

		if (!rs.IsEOF())
		{
			sTemp.Empty();

			RTU_YXOBJECT info;

			rs.GetFieldValue("sName",sTemp);
			info._strName = sTemp;

			rs.GetFieldValue("sID",sTemp);
			info._strSId = m_sID;

			rs.GetFieldValue("yxObjID",sTemp);
			info._strYxObjID = sTemp;

			rs.GetFieldValue("yxCode",sTemp);
			info._nYxCode = atoi(sTemp);

			pYxNew = new CRtuYxObject(&info);
			if (pYxNew)
				pYxNew->SetParent(this);
		}
		rs.Close();

		return pYxNew;
	}
	catch (CDBException* pE)
	{
		TRACE("BuildYxObjects Error:%s\r\n",pE->m_strError);
		if (GetParent())
		{
			sLogMessage.Format("配置数据库操作,SQL:%s,Error:%s",sSQL,pE->m_strError);
			GetParent()->InsertLog(SYS_RUNNING_S_LOG,sLogMessage,Error_Log_message);
		}

		pE->Delete();
		return NULL;
	}

	return pYxNew;
}

CRtuYkObject* CRtuStation::QueryYKInDB(CString ykID)
{
	CRtuYkObject* pYkNew = NULL;


	CString sLogMessage = "";
	CString sSQL = "";
	try
	{
		sSQL.Format("SELECT DISTINCT sName,sID,ykObjID,ykCode FROM rtuYkObject WHERE sID = '%s' AND ykObjID='%s'",m_sID,ykID);

		if (GetParent())
		{
			sLogMessage.Format("配置数据库操作,SQL:%s",sSQL);
			GetParent()->InsertLog(SYS_OTHER_RUN_LOG,sLogMessage,Normal_Log_Message);
		}

		CRecordset rs (m_pDatabase);

		rs.Open(CRecordset::forwardOnly,sSQL);
		CString sTemp;

		if (!rs.IsEOF())
		{
			RTU_YKOBJECT info;

			sTemp.Empty();
			rs.GetFieldValue("sName",sTemp);
			info._strName = sTemp;

			rs.GetFieldValue("sID",sTemp);
			info._strSId = m_sID;

			rs.GetFieldValue("ykObjID",sTemp);
			info._strYkObjID = sTemp;

			rs.GetFieldValue("ykCode",sTemp);
			info._nYkCode = atoi(sTemp);

			pYkNew = new CRtuYkObject(&info);
			if (pYkNew)
				pYkNew->SetParent(this);

		}

		rs.Close();

		return pYkNew;

	}
	catch (CDBException* pE)
	{
		TRACE("BuildYkObjects Error:%s\r\n",pE->m_strError);
		if (GetParent())
		{
			sLogMessage.Format("配置数据库操作,SQL:%s,Error:%s",sSQL,pE->m_strError);
			GetParent()->InsertLog(SYS_RUNNING_S_LOG,sLogMessage,Error_Log_message);
		}

		pE->Delete();
		return NULL;
	}


	return pYkNew;
}

CRtuZdObject* CRtuStation::QueryZdInDB(CString zdID)
{
	CRtuZdObject* pZdNew = NULL;

	CString sLogMessage ="";
	CString sSQL = "";
	try
	{
		sSQL.Format("SELECT DISTINCT sName,sID,ZdObjID,ZdCode,sizeof,OrderInItem,Unit,ScaleMax,ScaleMin FROM rtuZdObject WHERE sID = '%s' AND ZdObjID='%s'",m_sID,zdID);

		if (GetParent())
		{
			sLogMessage.Format("配置数据库操作,SQL:%s",sSQL);
			GetParent()->InsertLog(SYS_OTHER_RUN_LOG,sLogMessage,Normal_Log_Message);
		}

		CRecordset rs(m_pDatabase);
		rs.Open(CRecordset::forwardOnly,sSQL);
		CString sTemp = "";
		if (!rs.IsEOF())
		{
			sTemp.Empty();
			RTU_ZDOBJECT info;

			rs.GetFieldValue("sName",sTemp);
			info._strName = sTemp;

			rs.GetFieldValue("sID",sTemp);
			info._strSId = m_sID;

			rs.GetFieldValue("ZdObjID",sTemp);
			info._strZdObjID = sTemp;

			rs.GetFieldValue("ZdCode",sTemp);
			info._nZDCode = atoi(sTemp);

			rs.GetFieldValue("sizeof",sTemp);
			info._Sizeof = (BYTE)atoi(sTemp);

			rs.GetFieldValue("OrderInItem",sTemp);
			info._OrderInItem = (BYTE)atoi(sTemp);

			rs.GetFieldValue("Unit",sTemp);
			info._Unit = sTemp;

			rs.GetFieldValue("ScaleMax",sTemp);
			info._ScaleMax = (float)atof(sTemp);

			rs.GetFieldValue("ScaleMin",sTemp);
			info._ScaleMin = (float)atof(sTemp);

			pZdNew = new CRtuZdObject(&info);
			if (pZdNew)
				pZdNew->SetParent(this);

		}
		rs.Close();

		return pZdNew;

	}
	catch (CDBException* pE)
	{
		TRACE("BuildYkObjects Error:%s\r\n",pE->m_strError);

		if (GetParent())
		{
			sLogMessage.Format("配置数据库操作,SQL:%s,Error:%s",sSQL,pE->m_strError);
			GetParent()->InsertLog(SYS_RUNNING_S_LOG,sLogMessage,Error_Log_message);
		}

		pE->Delete();
		return NULL;
	}

	return pZdNew;
}


bool CRtuStation::AddRealDBYc(CRtuYcObject* pYc)
{
	if (pYc)
	{
		m_lstYcObjects.AddTail(pYc);
	}
	else
		return false;

	return true;
}

bool CRtuStation::AddRealDBYx(CRtuYxObject* pYx)
{
	if (pYx)
	{
		m_lstYxObjects.AddTail(pYx);
	}
	else
		return false;

	return true;
}

bool CRtuStation::AddRealDBYk(CRtuYkObject* pYk)
{
	if (pYk)
	{
		m_lstYkObjects.AddTail(pYk);
	}
	else
		return false;

	return true;
}

bool CRtuStation::AddRealDBZd(CRtuZdObject* pZd)
{
	if (pZd)
	{
		m_lstZdObjects.AddTail(pZd);
	}
	else
		return false;

	return true;
}


bool CRtuStation::CheckModify(PRTU_STATION pNewRtuInfo)
{
	if (!pNewRtuInfo) return false;

	if (pNewRtuInfo->_bCmType != GetCmType()) return true;
	if (pNewRtuInfo->_bCountyCode != GetCountyCode()) return true;
	if (pNewRtuInfo->_bInterface != GetInterface())	 return true;
	if (pNewRtuInfo->_bProvinceCode != GetProvinceCode()) return true;
	if (pNewRtuInfo->_bRegionCode != GetRegionCode())	return true;
	if (pNewRtuInfo->_CmComInfo.mBaund != GetBaundA())  return true;
	if (pNewRtuInfo->_CmComInfo.mComName != GetComPortA()) return true;
	if (pNewRtuInfo->_CmComInfo.mDataBits != GetDataBitsA()) return true;
	if (pNewRtuInfo->_CmComInfo.mParity != GetParityA()) return true;
	if (pNewRtuInfo->_CmComInfo.mStopBits != GetStopBitsA()) return true;
	if (pNewRtuInfo->_CmNetInfo.mIpAddress != GetIpAddrA()) return true;
	if (pNewRtuInfo->_CmNetInfo.mPort != GetSockPortA()) return true;
	if (pNewRtuInfo->_nAddrA1 != GetAddrA1()) return true;
	if (pNewRtuInfo->_nAddrA2 != GetAddrA2()) return true;
	if (pNewRtuInfo->_nAddrA3 != GetAddrA3()) return true;
	if (pNewRtuInfo->_strName != GetName()) return true;
	if (pNewRtuInfo->_bAppProType != GetAppProtocolType()) return true;
	if (pNewRtuInfo->_strStCode != GetStringStCode()) return true;
	if (pNewRtuInfo->_centercode != GetSYQCenterCode()) return true;
	if (pNewRtuInfo->_strSYQADDR != GetSYQStCode()) return true;
	if(pNewRtuInfo->_passWord != GetSYQPassword()) return true;


	if (pNewRtuInfo->_CmScCommInfo.nFactoryType != GetScFactoryType()) return true;
	if (pNewRtuInfo->_CmScCommInfo.nScAddress != GetScAddress()) return true;
	if (pNewRtuInfo->_nAppProVer != GetAppProVer()) return true;

	return false;
}


//设置此站接收报文的最后时间
//如果时间pSt为NULL,则设置函数调用的时间为最后时间
void CRtuStation::SetLastRecvTime(PSYSTEMTIME pSt/* = NULL */)
{
	if (m_blRecvFLAG == false )
		m_blRecvFLAG = true;
	SYSTEMTIME st;
	if (!pSt)
	{
		GetLocalTime(&st);
		pSt = &st;
	}

	m_LastRecvTime.wDay			 = pSt->wDay;
	m_LastRecvTime.wDayOfWeek    = pSt->wDayOfWeek;
	m_LastRecvTime.wHour		 = pSt->wHour;
	m_LastRecvTime.wMilliseconds = pSt->wMilliseconds;
	m_LastRecvTime.wMinute		 = pSt->wMinute;
	m_LastRecvTime.wMonth		 = pSt->wMonth;
	m_LastRecvTime.wSecond		 = pSt->wSecond;
	m_LastRecvTime.wYear		 = pSt->wYear;


	//更新属性窗口的时间信息
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	if (pMainFrame && 
		pMainFrame->GetSafeHwnd()&& 
		pMainFrame->GetRtuPropertyDlg() &&
		pMainFrame->GetRtuPropertyDlg() && 
		pMainFrame->GetRtuPropertyDlg()->GetSafeHwnd() &&
		pMainFrame->GetRtuPropertyDlg()->GetRtuStation() == this)
		pMainFrame->GetRtuPropertyDlg()->GetTab_AttrBasicInfo()->m_EditRecvTime.SetWindowText(GetLastRecvTimeString());
}


PSYSTEMTIME CRtuStation::GetLastRecvTime()
{
	return &m_LastRecvTime;
}

const CString CRtuStation::GetLastRecvTimeString()
{
	CString strTime = "";

	strTime.Format("%04d-%02d-%02d %02d:%02d:%02d",
		m_LastRecvTime.wYear,
		m_LastRecvTime.wMonth,
		m_LastRecvTime.wDay,
		m_LastRecvTime.wHour,
		m_LastRecvTime.wMinute,
		m_LastRecvTime.wSecond);

	return strTime;
}


//更新站的通信状态
//返回值 true：表示状态有改变，false 表示状态未改变
bool CRtuStation::SetCommState(BYTE newState)
{
	if (newState == m_bCommState)
		return false;

	m_Modify_Mutex.Lock();
	m_bCommState = newState;
	m_Modify_Mutex.Unlock();

	AddCmStateChangeLog();

	if (m_bCommState == Disconnect_State)
	{
		m_SysDataTrans.ClearReSendObjectList();
	}

	//更新属性窗口的时间信息
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	if (pMainFrame && 
		pMainFrame->GetSafeHwnd()&& 
		pMainFrame->GetRtuPropertyDlg() &&
		pMainFrame->GetRtuPropertyDlg() && 
		pMainFrame->GetRtuPropertyDlg()->GetSafeHwnd() &&
		pMainFrame->GetRtuPropertyDlg()->GetRtuStation() == this)
		pMainFrame->GetRtuPropertyDlg()->UpdateCaption();


	return true;
}


bool CRtuStation::UpdateConfigInfo(PRTU_STATION pInfo)
{
	if (!pInfo) return false;
	m_sName			= pInfo->_strName;
	m_nCode			= pInfo->_nAddrA1;

	m_AddrA1		= pInfo->_nAddrA1;
	m_ProvinceCode  = pInfo->_bProvinceCode;
	m_RegionCode    = pInfo->_bRegionCode;
	m_CountyCode    = pInfo->_bCountyCode;
	m_AddrA2		= pInfo->_nAddrA2;
	m_AddrA3		= pInfo->_nAddrA3;
	m_CmType		= pInfo->_bCmType;
	m_Interface		= pInfo->_bInterface;
	m_ComPortA		= pInfo->_CmComInfo.mComName;
	m_BaundA		= pInfo->_CmComInfo.mBaund;
	m_DataBitsA		= pInfo->_CmComInfo.mDataBits;
	m_StopBitsA		= pInfo->_CmComInfo.mStopBits;
	m_ParityA		= pInfo->_CmComInfo.mParity;
	m_IpAddrA		= pInfo->_CmNetInfo.mIpAddress;
	m_SockPortA		= pInfo->_CmNetInfo.mPort;

	m_nAppProVer = pInfo->_nAppProVer;
	m_nAppProtoType = pInfo->_bAppProType;
	m_nScAddress = pInfo->_CmScCommInfo.nScAddress;
	m_nScFactoyType = pInfo->_CmScCommInfo.nFactoryType;
	m_strStCode = pInfo->_strStCode;
	m_strSYQADDR = pInfo->_strSYQADDR;
	m_centercode = pInfo->_centercode;


	if (m_Interface == Serial_CommInterface)
		m_bCommState =CommUnknown_State;
	else
		m_bCommState = Disconnect_State;
	
	return true;
}

bool CRtuStation::BuildBasicPacket(CPacketFrameFormat& appFrame)
{
	appFrame.m_CommInterface		= GetInterface();
	appFrame.m_NetInfo.mIpAddress	= GetIpAddrA();
	appFrame.m_NetInfo.mPort		= GetSockPortA();

	appFrame._addr_info.A1.ProvinceCode = GetProvinceCode();
	appFrame._addr_info.A1.RegionCode	= GetRegionCode();
	appFrame._addr_info.A2 = GetAddrA2();
	appFrame._addr_info.A3 = GetAddrA3();
	appFrame.ControlC._DIR = DOWN_ARROW;
	appFrame.ControlC._DIV = 0;
	appFrame.ControlC._FCB = 3;
	appFrame._asdu.data.clear();
	appFrame.m_pStation = this;

	return true;
}

bool CRtuStation::BuildBasicPacket(CHydrSendFrame& appFrame)
{
	appFrame.m_CommInterface		= GetInterface();
	appFrame.m_NetInfo.mIpAddress	= GetIpAddrA();
	appFrame.m_NetInfo.mPort		= GetSockPortA();
	appFrame.m_strStCode              = GetStringStCode();
	return true;
}

bool CRtuStation::BuildBasicPacket(CWriteFrame& appFrame)
{
	appFrame.m_CommInterface		= GetInterface();
	appFrame.m_NetInfo.mIpAddress	= GetIpAddrA();
	appFrame.m_NetInfo.mPort		= GetSockPortA();
	appFrame.m_nDevAdddr            = GetStringStCode();
	return true;
}

bool CRtuStation::BuildBasicPacket(CSYQFrameFormat& appFrame)  //水雨情
{
	appFrame.m_CommInterface		= GetInterface();
	appFrame.m_NetInfo.mIpAddress	= GetIpAddrA();
	appFrame.m_NetInfo.mPort		= GetSockPortA();

	appFrame.m_stCode = GetSYQStCode();
	appFrame.m_CenterCode  = GetSYQCenterCode();
	appFrame.m_repeat = 3;
	appFrame._asdu.data.clear();
	appFrame.m_pStation = this;
	appFrame.m_iPassword = GetSYQPassword();

	return true;
}

CRainAlert_PARA_PER* CRtuStation::GetRainAlartPara(int Index)
{
	POSITION rPos = m_lstRainAlertParas.FindIndex(Index);
	if (rPos == NULL) 
		return NULL;

	return m_lstRainAlertParas.GetAt(rPos);
}


CAnalogLimit_PARA_PER* CRtuStation::GetAnalogLimitPara(int Index)
{
	POSITION rPos = m_lstAnalogLimitParas.FindIndex(Index);
	if (rPos == NULL)
		return NULL;

	return m_lstAnalogLimitParas.GetAt(rPos);
}

CWaterLevel_PAR_PER* CRtuStation::GetWaterLevelPara(int Index)
{
	POSITION rPos = m_lstWaterLevelParas.FindIndex(Index);
	if (rPos == NULL)
		return NULL;

	return m_lstWaterLevelParas.GetAt(rPos);
}


CWaterPressure_PARA_PER* CRtuStation::GetWaterPressurePara(int Index)
{
	POSITION rPos = m_lstWaterPressureParas.FindIndex(Index);
	if (rPos == NULL)
		return NULL;

	return m_lstWaterPressureParas.GetAt(rPos);
}

CFlowInitValume_PARA_PER* CRtuStation::GetFlowInitValumePara(int Index)
{
	POSITION rPos = m_lstFlowInitValumeParas.FindIndex(Index);
	if (rPos == NULL)
		return NULL;

	return m_lstFlowInitValumeParas.GetAt(rPos);
}

CWaterQuality_PARA_PER* CRtuStation::GetOne_WaterQualityPara(BYTE _bitPos)
{
	return m_WaterQualityPara.GetOneQualityObject(_bitPos);
}


CEventRecord_PER* CRtuStation::GetEventRecordObject(int Index)											//Index 0-18
{
	POSITION rPos = m_lstEventRecords.FindIndex(Index);
	if (rPos != NULL)
		return m_lstEventRecords.GetAt(rPos);

	return NULL;
}

CEventRecord_PER* CRtuStation::GetEventRecordByPos(int _pos)
{
	POSITION rPos = m_lstEventRecords.GetHeadPosition();
	while (rPos != NULL)
	{
		CEventRecord_PER* pEvent = m_lstEventRecords.GetNext(rPos);
		if (pEvent->bPos == _pos)
			return pEvent;
	}

	return NULL;
}

void CRtuStation::BuildEventRecordsList()
{
	ClearEventRecordsList();
	for (int i = 0; i < 19; i ++)
	{
		CEventRecord_PER* pEvent = new CEventRecord_PER();
		pEvent->bPos = i + 1;
		pEvent->wEventCount = 0;
		pEvent->strName = _EventRecordName[i];
		m_lstEventRecords.AddTail(pEvent);
	}
}

void CRtuStation::ClearEventRecordsList()
{
	while (!m_lstEventRecords.IsEmpty())
	{
		CEventRecord_PER* pEvent = m_lstEventRecords.RemoveHead();
		if (pEvent)
		{
			delete pEvent;
			pEvent = NULL;
		}
	}
}

//向日志系统插入一条日志信息
BOOL CRtuStation::AddLogMessage(_LOG_MESSAGE_TYPE _logType, 
								CString _LogMessage, 
								_Log_level_type _levelType /* = Normal_Log_Message */, 
								PSYSTEMTIME pLogTime /* = NULL */)
{
	BOOL blReturn = FALSE;

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	blReturn = pApp->SystemLog(_logType,_LogMessage,_levelType,pLogTime);

	return blReturn;
}

BOOL CRtuStation::AddCmStateChangeLog()
{
	CString strMessage="",sTemp="";
	_Log_level_type LogLevel = Normal_Log_Message;
	if (m_bCommState == Connected_State)
	{
		LogLevel = Normal_Log_Message;
		//strMessage.Format("%s 通信恢复正常!",Get_Log_RtuInfo());
		sTemp.Format("通信恢复正常.");
	}
	else if (m_bCommState == CommUnknown_State)
	{
		LogLevel = Warning_log_Message;
		//strMessage.Format("%s 长时间未收到报文,通信异常!",Get_Log_RtuInfo());
		sTemp.Format("长时间未收到报文,通信异常.");
	}
	else if (m_bCommState == Disconnect_State)
	{
		LogLevel = Warning_log_Message/*Error_Log_message*/;
		//strMessage.Format("%s 通信断开!",Get_Log_RtuInfo());
		sTemp.Format("通信断开.");
	}

	if (sTemp =="")
		return FALSE;
	
	InsertCommMessage(sTemp,LogLevel);

	//更新属性窗口的时间信息
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	if (pMainFrame && 
		pMainFrame->GetSafeHwnd()&& 
		pMainFrame->GetRtuPropertyDlg() &&
		pMainFrame->GetRtuPropertyDlg() && 
		pMainFrame->GetRtuPropertyDlg()->GetSafeHwnd() &&
		pMainFrame->GetRtuPropertyDlg()->GetRtuStation() == this)
	{
		if (pMainFrame->GetRtuPropertyDlg()->GetTab_AttrComms() != NULL && 
			pMainFrame->GetRtuPropertyDlg()->GetTab_AttrComms()->GetSafeHwnd())
		{
			SYSTEMTIME mt;
			GetLocalTime(&mt);
			pMainFrame->GetRtuPropertyDlg()->GetTab_AttrComms()->AddMessageToWindow(sTemp,LogLevel,mt);
		}
	}



	strMessage.Format("%s %s",Get_Log_RtuInfo(),sTemp);

	  
	return AddLogMessage(RTU_CM_STATUS_LOG,
		                 strMessage,LogLevel);

}

BOOL CRtuStation::AddYkOperateLog(CString _LogMessage, 
								  _Log_level_type _levelType /* = Normal_Log_Message */, 
								  PSYSTEMTIME pLogTime /* = NULL */)
{
	return AddLogMessage(RTU_YK_OPERAT_LOG,_LogMessage,_levelType,pLogTime);
}

const CString CRtuStation::Get_Log_RtuInfo()
{
	CString str_Rtu_info= "";
	//str_Rtu_info.Format("遥测终端=%s,行政区划码=%02d%02d%02d,ADDR=%-6d",
	//	m_sName,
	//	m_ProvinceCode,
	//	m_RegionCode,
	//	m_CountyCode,
	//	m_AddrA2);

	str_Rtu_info.Format("遥测终端[行政区划码:%02d%02d%02d,ADDR:%-6d]",
		m_ProvinceCode,
		m_RegionCode,
		m_CountyCode,
		m_AddrA2);
	return str_Rtu_info;
}


void CRtuStation::SetHandle_Com(CHandle_COM* pHandleCom)
{
	m_cs_HandleCom.Lock();
	if (m_pHandle_Com != pHandleCom)
		m_pHandle_Com = pHandleCom;
	m_cs_HandleCom.Unlock();
}

void CRtuStation::SetHandle_Net(CHandle_NET* pHandleNet)
{
	m_cs_HandelNet.Lock();
	if (m_pHandle_Net != pHandleNet)		
		m_pHandle_Net = pHandleNet;
	m_cs_HandelNet.Unlock();
}

void CRtuStation::SetScMessageDealModule( CSCMessageDealModule* pScMessageModule )
{
	m_cs_ScMessageDealModule.Lock();
	if (m_pScMessageDealModule != pScMessageModule)
		m_pScMessageDealModule = pScMessageModule;
	m_cs_ScMessageDealModule.Unlock();
}

CSCMessageDealModule* CRtuStation::GetScMessageDealModule()
{
	return m_pScMessageDealModule;
}

void CRtuStation::SetHandle_TcpClient(CHandle_TcpClient* pHandleTcpClient)
{
	CHandle_TcpClient* pTempClient = NULL;

	m_cs_HandleTcpClient.Lock();
	if (m_pHandle_TcpClient != pHandleTcpClient)
	{
		if (m_pHandle_TcpClient != NULL && pHandleTcpClient != NULL)			
			pTempClient = m_pHandle_TcpClient;

		m_pHandle_TcpClient = pHandleTcpClient;
	}
	m_cs_HandleTcpClient.Unlock();


	if (pTempClient != NULL)
	{
		pTempClient->SetRelateStation(NULL);   //关键
		pTempClient->CloseSocket();
	}
}

CSysDataTrans* CRtuStation::GetSysDataTrans()
{
	return &m_SysDataTrans;
}

CSysDataTrans_Hydr* CRtuStation::GetSysDataTrans_Hydr()
{
	return &m_SysDataTrans_Hydr;
}
CSysDataTrans_Syq* CRtuStation::GetSysDataTrans_Syq()
{
	return &m_SysDataTrans_Syq;
}


CSysDataTrans_WriteUpdate* CRtuStation::GetSysDataTrans_WriteUpdate()
{
	return &m_SysDataTrans_WriteUpdate;
}

bool CRtuStation::SendData(BYTE* buffer,WORD len)
{
	if(!buffer  ||
		len <=0 )
		return false;

	if (m_CmType != SC_CommChannel)
	{
		if (m_Interface == Ethernet_CommInerface)   //网络
		{
			if (UDP_Protocol == m_ProtocolType)     //UDP方式
			{
				m_cs_HandelNet.Lock();
				if (m_pHandle_Net != NULL)
				{
					m_pHandle_Net->sendNET(buffer,len,m_IpAddrA,m_SockPortA);
					m_pHandle_Net->AddSendMessage(buffer,len,m_IpAddrA,m_SockPortA);
				}
				m_cs_HandelNet.Unlock();

			}
			else if (TCP_Protocol == m_ProtocolType) //TCP方式
			{
				m_cs_HandleTcpClient.Lock();
				if (m_pHandle_TcpClient != NULL)
				{
					m_pHandle_TcpClient->SendData(buffer,len);
					m_pHandle_TcpClient->GetProtocolTcpHandle()->AddSendMessage(buffer,len);
				}
				m_cs_HandleTcpClient.Unlock();
			}
		}
		else if (m_Interface == Serial_CommInterface) //串口
		{
			m_cs_HandleCom.Lock();
			if (m_pHandle_Com != NULL)
			{
				m_pHandle_Com->sendCOM(buffer,len);

				COM_INFO comInfo;
				comInfo.mComName	= m_ComPortA;
				comInfo.mBaund		= m_BaundA;
				comInfo.mDataBits	= m_DataBitsA;
				comInfo.mParity		= m_ParityA;
				comInfo.mStopBits	= m_StopBitsA;
				m_pHandle_Com->AddSendMessage(buffer,len, &comInfo);
			}
			m_cs_HandleCom.Unlock();
		}
	}
	else if (m_CmType  == SC_CommChannel)  //卫星通信
	{
		m_cs_ScMessageDealModule.Lock();
		if (m_pScMessageDealModule != NULL)
		{
			m_pScMessageDealModule->SendMessage(buffer,len);
		}
		m_cs_ScMessageDealModule.Unlock();
	}

	return true;
}

bool CRtuStation::blEnbaleTrans()
{
	if ((GetSysDataTrans() != NULL)||(GetSysDataTrans_Syq() != NULL))
	{
		if (m_bCommState == Disconnect_State)
			return false;

		if (m_CmType != SC_CommChannel)
		{
			if (m_Interface == Serial_CommInterface  && 
				m_pHandle_Com  != NULL)
				return true;

			if (m_Interface == Ethernet_CommInerface && (
				(m_ProtocolType == UDP_Protocol && m_pHandle_Net != NULL)||
				m_ProtocolType == TCP_Protocol && m_pHandle_TcpClient != NULL))
				return true;
		}
		else if (m_CmType == SC_CommChannel)
		{
			if (m_pScMessageDealModule != NULL)
				return true;
		}
	}

	return false;
}

void CRtuStation::ClearCommHandle()
{
	if (m_pHandle_Com)
		m_pHandle_Com->RemoveRelateStation(this);

	if (m_pHandle_Net)
		m_pHandle_Net->RemoveRelateStation(this);

	if (m_pHandle_TcpClient)
		m_pHandle_TcpClient->SetRelateStation(NULL);

	if (m_pScMessageDealModule)
		m_pScMessageDealModule->SetSubStation(NULL);
}


void CRtuStation::InsertDataMessage(CString DataMsg,PSYSTEMTIME pTime /* = NULL */)
{
	Log_Item* pItem= new Log_Item();

	pItem->m_Message = DataMsg;
	if (pTime)
	{
		pItem->m_Time.wDay = pTime->wDay;
		pItem->m_Time.wDayOfWeek = pTime->wDayOfWeek;
		pItem->m_Time.wHour = pTime->wHour;
		pItem->m_Time.wMilliseconds = pTime->wMilliseconds;
		pItem->m_Time.wMinute = pTime->wMinute;
		pItem->m_Time.wMonth = pTime->wMonth;
		pItem->m_Time.wSecond = pTime->wSecond;
		pItem->m_Time.wYear = pTime->wYear;
	}

	InsertDataMessage(pItem);

}

void CRtuStation::InsertDataMessage(Log_Item* pData)
{
	if (pData)
	{
		if (m_lstData.GetCount() >= MAX_LogCount)
		{
			Log_Item* pItem = m_lstData.RemoveHead();
			if (pItem)
			{
				delete pItem;
				pItem = NULL;
			}
		}

		m_lstData.AddTail(pData);
	}
}

void CRtuStation::ClearDataList()
{
	while (!m_lstData.IsEmpty())
	{
		Log_Item* pObj = m_lstData.RemoveHead();
		if (pObj)
		{
			delete pObj;
			pObj = NULL;
		}
	}
}

CList<Log_Item*>* CRtuStation::GetDataList()
{
	return &m_lstData;
}


void CRtuStation::InsertLogMessage(CString logMessage, _Log_level_type xLevel /* = Normal_Log_Message */, PSYSTEMTIME pTime /* = NULL */)
{
	Log_Item* pItem= new Log_Item();

	pItem->m_Message = logMessage;
	pItem->m_Level = xLevel;
	if (pTime)
	{
		pItem->m_Time.wDay = pTime->wDay;
		pItem->m_Time.wDayOfWeek = pTime->wDayOfWeek;
		pItem->m_Time.wHour = pTime->wHour;
		pItem->m_Time.wMilliseconds = pTime->wMilliseconds;
		pItem->m_Time.wMinute = pTime->wMinute;
		pItem->m_Time.wMonth = pTime->wMonth;
		pItem->m_Time.wSecond = pTime->wSecond;
		pItem->m_Time.wYear = pTime->wYear;
	}

	InsertLogMessage(pItem);
}

void CRtuStation::InsertLogMessage(Log_Item* pLog)
{
	if (pLog)
	{
		if (m_lstLogs.GetCount() > MAX_LogCount)
		{
			Log_Item* pItem = m_lstLogs.RemoveHead();
			if (pItem)
			{
				delete pItem;
				pItem = NULL;
			}
		}

		m_lstLogs.AddTail(pLog);
	}
}

CList<Log_Item*>* CRtuStation::GetLogList()
{
	return &m_lstLogs;
}

void CRtuStation::ClearLogList()
{
	while(!m_lstLogs.IsEmpty())
	{
		Log_Item* pObj = m_lstLogs.RemoveHead();
		if (pObj)
		{
			delete pObj;
			pObj = NULL;
		}
	}
}

void CRtuStation::InsertCommMessage(CString CommLog, _Log_level_type xLevel /* = Normal_Log_Message */, PSYSTEMTIME pTime /* = NULL */)
{
	Log_Item* pItem= new Log_Item();

	pItem->m_Message = CommLog;
	pItem->m_Level = xLevel;
	if (pTime)
		pItem->m_Time = (*pTime);

	InsertCommMessage(pItem);
}

void CRtuStation::InsertCommMessage(Log_Item* LogItem)
{
	if (LogItem)
	{
		if (m_lstComms.GetCount() >= MAX_LogCount)
		{
			Log_Item* pDel = m_lstComms.RemoveHead();
			_DELETE(pDel);
		}
		m_lstComms.AddTail(LogItem);
	}
}

CList<Log_Item*>* CRtuStation::GetCommList()
{
	return &m_lstComms;
}

void CRtuStation::ClearCommList()
{
	while(!m_lstComms.IsEmpty())
	{
		Log_Item* pObj = m_lstComms.RemoveHead();
		if (pObj)
		{
			_DELETE(pObj);
		}
	}
}


CPulsePara_PER* CRtuStation::GetPulseParaObject( UINT64 code )
{
	CPulsePara_PER* pObj = NULL;

	std::map<UINT64,CPulsePara_PER*>::iterator ItrFind;
	ItrFind = m_mapPulseParas.find(code);
	if (ItrFind != m_mapPulseParas.end())
		pObj = ItrFind->second;

	return pObj;
}

void CRtuStation::InsertPulseParaObject( CPulsePara_PER* pPulseObject )
{
	if (pPulseObject)
	{
		m_mapPulseParas.insert(std::pair<UINT64,CPulsePara_PER*>(pPulseObject->GetCode(),pPulseObject));
	}
}

void CRtuStation::UpdateLastPacket( CPacketFrameFormat* pPacket )
{
	if (pPacket)
	{
		try
		{
			m_lastPacket._addr_info		=	pPacket->_addr_info;
			m_lastPacket._asdu._AFN		=	pPacket->_asdu._AFN;
			m_lastPacket._asdu.data		=   pPacket->_asdu.data;
			m_lastPacket._bFLen			=	pPacket->_bFLen;

			m_lastPacket.ControlC._Command	= pPacket->ControlC._Command;
			m_lastPacket.ControlC._DIR		= pPacket->ControlC._DIR;
			m_lastPacket.ControlC._DIV		= pPacket->ControlC._DIV;
			m_lastPacket.ControlC._DIVS		= pPacket->ControlC._DIVS;
			m_lastPacket.ControlC._FCB		= pPacket->ControlC._FCB;

			m_lastPacket.UpdateRecvTime(pPacket->GetRecvTime());
		}
		catch (CMemoryException* e)
		{
			e->Delete();
		}
		catch (CException* e)
		{
			e->Delete();
		}
		catch(...)
		{

		}
	}
}

void CRtuStation::CheckRepeatType( CPacketFrameFormat* pPacket )
{
	if (pPacket)
	{
		//检查,并更新Packet的 repeattype的值
		if (pPacket->ControlC._FCB != 3)
		{
			if (pPacket->ControlC._Command == m_lastPacket.ControlC._Command &&
				pPacket->_asdu._AFN		   == m_lastPacket._asdu._AFN  &&
				pPacket->_asdu.data.size() == m_lastPacket._asdu.data.size())
			{
				bool blAllEqual = true;
				for (size_t i = 0; i < pPacket->_asdu.data.size(); i ++)
				{
					if (pPacket->_asdu.data[i] != m_lastPacket._asdu.data[i])
					{
						blAllEqual = false;
						break;
					}
				}
				if (blAllEqual == true)
				{
					INT64 _minus = CSystemTimeOp::MinusSystemTime((*(pPacket->GetRecvTime())),(*(m_lastPacket.GetRecvTime()))) ;
					INT64 _checkNum = (INT64)(GETAPP()->m_GlobalOptions.m_nCheckRepeatInterval) *(INT64)1000;
					if ( _minus <=  _checkNum)
						pPacket->SetRepeatType(true);
				}
			}
		}

		UpdateLastPacket(pPacket);
	}
}

void CRtuStation::SetNetInfo( PNET_INFO pNetInfo )
{
	if (pNetInfo != NULL)
	{
		SetIpAddrA(pNetInfo->mIpAddress);
		SetSockPortA(pNetInfo->mPort);
	}
}

void CRtuStation::SetComInfo( PCOM_INFO pComInfo )
{
	if (pComInfo != NULL)
	{
		SetComPortA(pComInfo->mComName);
		SetBaundA(pComInfo->mBaund);
		SetDataBitsA(pComInfo->mDataBits);
		SetStopBitsA(pComInfo->mStopBits);
		SetParityA(pComInfo->mParity);
	}
}

void CRtuStation::SetScCommInfo(PSC_COMM_INFO pInfo)
{
	if (pInfo != NULL)
	{
		SetScAddress(pInfo->nScAddress);
		SetScFactoryType(pInfo->nFactoryType);
	}
}


void CPulsePara_PER::SetCodeString( CString strCode )
{
	m_strCodeString = strCode;
}

void CPulsePara_PER::SetValue4( double value4 )
{
	m_dbValue4 = value4;
}

void CPulsePara_PER::SetValue3( double value3 )
{
	m_dbValue3 = value3;
}

void CPulsePara_PER::SetValue2( double value2 )
{
	m_dbValue2 = value2;
}

void CPulsePara_PER::SetValue1( double value1 )
{
	m_dbValue1 = value1;
}

void CPulsePara_PER::SetCode( UINT64 code )
{
	m_unCode = code;
}

const CString CPulsePara_PER::GetCodeString()
{
	return m_strCodeString;
}

const double CPulsePara_PER::GetValue3()
{
	return m_dbValue3;
}

const double CPulsePara_PER::GetValue4()
{
	return m_dbValue4;
}

const double CPulsePara_PER::GetValue2()
{
	return m_dbValue2;
}

const double CPulsePara_PER::GetValue1()
{
	return m_dbValue1;
}

const UINT64 CPulsePara_PER::GetCode()
{
	return m_unCode;
}

CPulsePara_PER::CPulsePara_PER( UINT64 Code /*= 0*/, double value1 /*= 0*/, double value2 /*= 0*/, double value3 /*= 0*/, double value4/*= 0*/, CString strCode /*= ""*/ )
{
	m_unCode = Code;
	m_dbValue1 = value1;
	m_dbValue2 = value2;
	m_dbValue3 = value3;
	m_dbValue4 = value4;
	m_strCodeString = strCode;
}

CPulsePara_PER::CPulsePara_PER( CPulsePara_PER* pSource )
{
	if (pSource != NULL)
	{
		this->m_unCode  = pSource->GetCode();
		this->m_dbValue1 = pSource->GetValue1();
		this->m_dbValue2 = pSource->GetValue2();
		this->m_dbValue3 = pSource->GetValue3();
		this->m_dbValue4 = pSource->GetValue4();
		this->m_strCodeString = pSource->GetCodeString();
	}
	else
	{
		CPulsePara_PER();
	}
}

CWaterLevel_PAR_PER::CWaterLevel_PAR_PER( CWaterLevel_PAR_PER* pSoruce )
{
   if (pSoruce != NULL)
   {
	   this->m_fBaseValue = pSoruce->GetBaseValue();
	   this->m_fDownValue = pSoruce->GetDownValue();
	   this->m_fRealValue = pSoruce->GetRealValue();
	   this->m_fUpValue = pSoruce->GetUpValue();
   }
   else
   {
	   CWaterLevel_PAR_PER();
   }
}

CWaterPressure_PARA_PER::CWaterPressure_PARA_PER( CWaterPressure_PARA_PER* pSource )
{
	if (pSource != NULL)
	{
		this->m_dDownValue = pSource->GetDownValue();
		this->m_dUpValue = pSource->GetUpValue();
		this->m_dRealValue = pSource->GetRealValue();
	}
	else
	{
		CWaterPressure_PARA_PER();
	}
}

CFlowInitValume_PARA_PER::CFlowInitValume_PARA_PER( CFlowInitValume_PARA_PER* pSource )
{
	if (pSource != NULL)
	{
		this->m_dwCalTakeValume = pSource->GetCalTakeValume();
		this->m_dwFlowInitValume = pSource->GetFlowInitValume();
		this->m_dwInsFlowValume = pSource->GetInsFlowValume();
	}
	else
	{
		CFlowInitValume_PARA_PER();
	}
}

CWaterQuality_PARA_PER::CWaterQuality_PARA_PER( CWaterQuality_PARA_PER* pSource )
{
	if (pSource != NULL)
	{
		this->m_ScaleMax = pSource->m_ScaleMax;
		this->m_ScaleMin = pSource->m_ScaleMin;
		this->m_strName  = pSource->m_strName;
		this->m_strUnit  = pSource->m_strUnit;
		this->m_BitPos = pSource->m_BitPos;
		this->m_DataBits = pSource->m_DataBits;
		this->m_UpValume = pSource->m_UpValume;
		this->m_DownValue = pSource->m_DownValue;
		this->m_PointBits = pSource->m_PointBits;
		this->m_BUF_Cnt = pSource->m_BUF_Cnt;
		this->m_CurValue = pSource->m_CurValue;
	}
	else
	{
		CWaterQuality_PARA_PER();
	}
}

CRainAlert_PARA_PER::CRainAlert_PARA_PER( CRainAlert_PARA_PER* pSource )
{
	if (pSource != NULL)
	{
		this->m_DataType = pSource->GetDataType();
		this->m_fRainAlertValue = pSource->GetRainAlertValue();
		this->m_RealTimeValue = pSource->GetRealTimeValue();
		this->m_TotalValue = pSource->GetTotalValue();
		this->m_wPerTime = pSource->GetPerTime();
		this->m_HalfhValue = pSource->GetHalfHourValue();
	}
	else
	{
		CRainAlert_PARA_PER();
	}
}

CAnalogLimit_PARA_PER::CAnalogLimit_PARA_PER( CAnalogLimit_PARA_PER* pSource )
{
	if (pSource != NULL)
	{
		this->m_Analog_DownLimit = pSource->GetAnalogDownLimit();
		this->m_Analog_UpLimit = pSource->GetAnalogUpLimit();
		this->m_RealTimeValue = pSource->GetRealTimeValue();
	}
	else
	{
		CAnalogLimit_PARA_PER();
	}
}
