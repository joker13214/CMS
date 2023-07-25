#include "stdafx.h"
#include "SysDataTransHydr.h"
#include "CMS.h"
#include "RealTimeDB.h"
#include "MainFrm.h"
#include "RtuStation.h"
#include "CommServer.h"

CSysDataTrans_Hydr::CSysDataTrans_Hydr()
{
	m_pSubStation = NULL;
	m_pCurrCallOP = scsw_no_call_operation;
}

CSysDataTrans_Hydr::~CSysDataTrans_Hydr()
{

}
void CSysDataTrans_Hydr::Dispatcher(BYTE* buffer,int bufferLen, BYTE CmInterface, CString FromIP,int FromPort)                      //报文处理，默认串口
{

}

void CSysDataTrans_Hydr::Dispatcher(CHydrFrameFormat* phydrFrame)
{
	if (NULL == phydrFrame)  
		return;

	CString print = "";
	print.Format("%s号遥测站 %d月%d日%d时%d分 水文信息如下：\r\n\n",
		phydrFrame->m_strStCode,
		phydrFrame->m_DataTimeTT.month,
		phydrFrame->m_DataTimeTT.day,
		phydrFrame->m_DataTimeTT.hour,
		phydrFrame->m_DataTimeTT.min);

	SCSW_MsgInfo *pInfo = NULL;
	bool bFlag = false;
	std::list<SCSW_MsgInfo*>::iterator it;
	for (it=phydrFrame->m_MsgInfoList.begin();
		it!=phydrFrame->m_MsgInfoList.end();
		it++)
	{
		pInfo = (SCSW_MsgInfo*)(*it);
		if (NULL == pInfo)
			continue;

		switch(pInfo->m_nProtoType)
		{
		case SCSW_Z:
			SCSW_hydrdata_Z(phydrFrame, pInfo, print);
			bFlag = true;
			break;             //水位
		case SCSW_PN:
			SCSW_hydrdata_PN(phydrFrame, pInfo, print);
			bFlag = true;
			break;             //雨量一
		case SCSW_P:
			SCSW_hydrdata_P(phydrFrame, pInfo, print);
			bFlag = true;
			break;
		case SCSW_M10:
			SCSW_hydrdata_M10(phydrFrame, pInfo, print);
			bFlag = true;
			break;
		case SCSW_H:
			SCSW_hydrdata_H(phydrFrame, pInfo, print);
			bFlag = true;
			break;
		case SCSW_C:
			SCSW_hydrdata_C(phydrFrame, pInfo, print);
			bFlag = true;
			break;
		case SCSW_ED:
			SCSW_hydrdata_ED(phydrFrame, pInfo, print);
			bFlag = true;
			break;
		case SCSW_ZR:
			SCSW_hydrdata_ZR(phydrFrame, pInfo, print);
			bFlag = true;
			break;
		case SCSW_RC:
			SCSW_hydrdata_RC(phydrFrame, pInfo, print);
			bFlag = true;
			break;
		case SCSW_ML:
			SCSW_hydrdata_ML(phydrFrame, pInfo, print);
			bFlag = true;
			break;
		case SCSW_MF:
			SCSW_hydrdata_MF(phydrFrame, pInfo, print);
			bFlag = true;
			break;
		case SCSW_BV:
			SCSW_hydrdata_BV(phydrFrame, pInfo, print);
			bFlag = true;
			break;
		case SCSW_SV:
			SCSW_hydrdata_SV(phydrFrame, pInfo, print);
			bFlag = true;
			break;
		case SCSW_ET:
			SCSW_hydrdata_ET(phydrFrame, pInfo, print);
			bFlag = true;
			break;
		case SCSW_SI:
			SCSW_hydrdata_SI(phydrFrame, pInfo, print);
			bFlag = true;
			break;
		case SCSW_PI:
			SCSW_hydrdata_PI(phydrFrame, pInfo, print);
			bFlag = true;
			break;
		case SCSW_RD:
			SCSW_hydrdata_RD(phydrFrame, pInfo, print);
			bFlag = true;
			break;
		case SCSW_ZJ:
			SCSW_hydrdata_ZJ(phydrFrame, pInfo, print);
			bFlag = true;
			break;
		case SCSW_ZT:
			SCSW_hydrdata_ZT(phydrFrame, pInfo, print);
			bFlag = true;
			break;
		case SCSW_ZF:
			SCSW_hydrdata_ZF(phydrFrame, pInfo, print);
			bFlag = true;
			break;
		case SCSW_ZA:
		case SCSW_ZB:
		case SCSW_ZC:
		case SCSW_ZD:
		case SCSW_ZE:
			SCSW_hydrdata_Zx(phydrFrame, pInfo, print);
			bFlag = true;
			break;
		case SCSW_DZA:
		case SCSW_DZB:
		case SCSW_DZC:
		case SCSW_DZD:
		case SCSW_DZE:
			SCSW_hydrdata_DZ(phydrFrame, pInfo, print);
			bFlag = true;
			break;
		case SCSW_TT:
			SCSW_hydrdata_TT(phydrFrame, pInfo, print);
			bFlag = true;
			break;	
		case SCSW_TM:
			SCSW_hydrdata_TM(phydrFrame, pInfo, print);
			bFlag = true;
			break;
		case SCSW_TN:
			SCSW_hydrdata_TN(phydrFrame, pInfo, print);
			bFlag = true;
			break;
		case SCSW_TB:
			SCSW_hydrdata_TB(phydrFrame, pInfo, print);
			bFlag = true;
			break;
		case SCSW_TE:
			SCSW_hydrdata_TE(phydrFrame, pInfo, print);
			bFlag = true;
			break;
		case SCSW_RR:
			SCSW_hydrdata_RR(phydrFrame, pInfo, print);
			bFlag = true;
			break;
		case SCSW_DV:
			SCSW_hydrdata_DV(phydrFrame, pInfo, print);
			bFlag = true;
			break;
		case SCSW_DI:
			SCSW_hydrdata_DI(phydrFrame, pInfo, print);
			bFlag = true;
			break;
		case SCSW_DC:
			SCSW_hydrdata_DC(phydrFrame, pInfo, print);
			bFlag = true;
			break;
		case SCSW_CN:
			SCSW_hydrdata_CN(phydrFrame, pInfo, print);
			bFlag = true;
			break;
		case SCSW_CP:
			SCSW_hydrdata_CP(phydrFrame, pInfo, print);
			bFlag = true;
			break;
		case SCSW_GSM:
			SCSW_hydrdata_GSM(phydrFrame, pInfo, print);
			bFlag = true;
			break;
		case SCSW_BD:
			SCSW_hydrdata_BD(phydrFrame, pInfo, print);
			bFlag = true;
			break;
		case SCSW_PSTN:
			SCSW_hydrdata_PSTN(phydrFrame, pInfo, print);
			bFlag = true;
			break;
		case SCSW_IT:
			SCSW_hydrdata_IT(phydrFrame, pInfo, print);
			bFlag = true;
			break;
		case SCSW_PT:
			SCSW_hydrdata_PT(phydrFrame, pInfo, print);
			bFlag = true;
			break;
		case SCSW_TC:
			SCSW_hydrdata_TC(phydrFrame, pInfo, print);
			bFlag = true;
			break;
		case SCSW_PW:
			SCSW_hydrdata_PW(phydrFrame, pInfo, print);
			bFlag = true;
			break;
		case SCSW_ER:
			SCSW_hydrdata_ER(phydrFrame, pInfo, print);
			bFlag = true;
			break;
		case SCSW_RF:
			SCSW_hydrdata_RF(phydrFrame, pInfo, print);
			bFlag = true;
			break;
		case SCSW_HOST:
			SCSW_hydrdata_HOST(phydrFrame, pInfo, print);
			bFlag = true;
			break;
		}

		//delete pInfo; 
		//pInfo = 0;

	}

	if (NULL == phydrFrame->m_pStation)
	{
		return;
	}

	if (bFlag)
	{
		switch (phydrFrame->m_nCot)
		{
		case SCSW_DC://自报类型数据
			{
				if (NULL != phydrFrame->m_pStation)
				{
					_Log_level_type xLevel = Normal_Log_Message;
					CString content = "";
					if (phydrFrame->m_workmode == 1)
					{
						content.Format("[自报类型报文] %s", print);
					}
					else if (phydrFrame->m_workmode == 2)
					{
						content.Format("调试模式[自报类型报文] %s", print);
					}
					
					AddLogMessage(phydrFrame->m_pStation, content, xLevel, (PSYSTEMTIME)NULL);
					CHydrSendFrame appFrame;
					m_pSubStation->BuildBasicPacket(appFrame);

					if (!print.IsEmpty())
					{
						if (phydrFrame->m_workmode == 2)
						{
							print = "[调试模式]" + print;
						}

						phydrFrame->m_pStation->InsertDataMessage(print);
						AddDataMessage(phydrFrame->m_pStation, print, (PSYSTEMTIME)NULL);
					}

					SCSW_hydrdata_EN_OK(&appFrame);
				}
				else
				{
					print = "";
				}

			}
			break;
			//case SCSW_CN://响应召唤类型
		default:
			{
				if (NULL != phydrFrame->m_pStation)
				{
					_Log_level_type xLevel = Normal_Log_Message;
					CString content = "";
					if (phydrFrame->m_workmode == 1)
					{
						content.Format("[响应召测报文] %s", print);
					}
					else if (phydrFrame->m_workmode == 2)
					{
						content.Format("调试模式[响应召测报文] %s", print);
					}

					AddLogMessage(phydrFrame->m_pStation, content, xLevel, (PSYSTEMTIME)NULL);
					CHydrSendFrame appFrame;
					m_pSubStation->BuildBasicPacket(appFrame);
					//SCSW_hydrdata_EN_OK(&appFrame);

				}
				else
				{
					print = "";
				}

			}
			break;
		}
		
		
	}
	
}

//将报文转换为字符串信息
CString CSysDataTrans_Hydr::BufferToString(BYTE* buffer,int Len)
{
	if (!buffer  || Len <= 0)
		return "";

	CString strResult = (char*)buffer;
// 	for (int idx = 0 ; idx < Len; idx ++)
// 	{
// 		CString strTemp = "";
// 		strTemp = buffer[idx];
// 		strResult += strTemp;
// 	}
	return strResult;
}

void CSysDataTrans_Hydr::AddRecvLog(BYTE* buffer,int len, CHydrFrameFormat* hydrFrame)
{
	if (!buffer  || len <=0) return;

	CString strBuffer = BufferToString(buffer,len);
	CString strLogBuffer ="";

	CHydrFrameFormat Pff;
	if (hydrFrame == NULL)
	{
		if (!CUdfGlobalFUNC::SCSW008_hydrologic_to_APPFRAME(buffer, len, &Pff))  
			return;

		hydrFrame = &Pff;
	}

	if (hydrFrame != NULL)
	{
		if (hydrFrame->m_pStation == NULL)
		{
			CCMSApp* pApp = (CCMSApp*)AfxGetApp();
			CRealTimeDB* pRealDb = pApp->m_pReadDB;
			if (!pRealDb)	return ;

			CRtuStation* pSub =  pRealDb->SearchRtuStation(hydrFrame->m_strStCode, SCSW_008_Protocol);
			hydrFrame->m_pStation = pSub;
		}

		CString strBuffer = BufferToString(buffer,len);
		CString strLogBuffer ="";

		strLogBuffer.Format("收,%-2d月%-2d日%-2d点%-2d分,DIR=%s,DIV=%-3d,Len=%-3d,报文=%s",
			hydrFrame->m_DataTimeTT.month,
			hydrFrame->m_DataTimeTT.day,
			hydrFrame->m_DataTimeTT.hour,
			hydrFrame->m_DataTimeTT.min,
			_T("上行"),
			hydrFrame->m_nDivCnt,
			len,
			strBuffer);

		AddLogMessage(hydrFrame->m_pStation ,strLogBuffer, Normal_Log_Message);
	}
}

int CSysDataTrans_Hydr::SendRtuLinkFrame(BYTE* sBuf,int len,BYTE CmInterface, PNET_INFO pToNetInfo)
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
				AddSendLog(sBuf,len, m_pSubStation->GetStringStCode());
		m_cs_SendData.Unlock();
	}

	return sendLen;

}

//添加下发报文日志
//将日志信息添加至遥测终端的属性窗口
void CSysDataTrans_Hydr::AddSendLog(BYTE* buffer,int len,  CString strStCode)
{  
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	CRealTimeDB* pRealDb = pApp->m_pReadDB;
	if (!pRealDb)			 return ;
	if (!buffer  || len <=0) return;

	CRtuStation* pSub =  pRealDb->SearchRtuStation(strStCode, SCSW_008_Protocol);

	CString strBuffer = BufferToString(buffer,len);

	if (pSub != NULL)
	{
		CString strLogBuffer= "";
		strLogBuffer.Format("发,DIR=%s,Len=%-3d,报文=%s",_T("下行"), len, strBuffer);
		AddLogMessage(pSub,strLogBuffer,Normal_Log_Message);
	}
}

void CSysDataTrans_Hydr::AddLogMessage(CRtuStation* pRTU,CString strLogMessage, _Log_level_type leveType, PSYSTEMTIME pTime)
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

//添加数据信息到遥测终端的自报数据显示窗口
void CSysDataTrans_Hydr::AddDataMessage(CRtuStation* pRTU,CString strDataMessage,PSYSTEMTIME pTime/* =NULL */)
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

void CSysDataTrans_Hydr::UpdateAttribFrame(CRtuStation* pRTU, int attrType)
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
	case scsw_waterLevel_call:
		{
			CRtuAttrHydrWaterLevel *pTab = pWnd->GetRtuAttrHydrrProSet()->GetHydrTab_WaterLevel();
			if ( pTab != NULL || pTab->GetSafeHwnd())
			{
				pTab->UpdateDlg(attrType, TRUE);
				pTab->Update();
				m_pCurrCallOP = scsw_no_call_operation;
			}
		}
		break;
	case scsw_waterRain_call:
		{
			CRtuAttrHydrRain *pTab = pWnd->GetRtuAttrHydrrProSet()->GetHydrTab_RainValume();
			if (pTab != NULL || pTab->GetSafeHwnd())
			{
				pTab->UpdateDlg(attrType, TRUE);
				pTab->Update();
				m_pCurrCallOP = scsw_no_call_operation;
			}
		}
		break;
	case scsw_waterSoil_call:
		{
			CRtuAttrHydrSoilMoisture *pTab = pWnd->GetRtuAttrHydrrProSet()->GetHydrTab_SoilMoisture();
			if (pTab != NULL || pTab->GetSafeHwnd())
			{
				pTab->UpdateDlg(attrType, TRUE);
				pTab->Update();
				m_pCurrCallOP = scsw_no_call_operation;
			}
		}
		break;
	case scsw_waterGWD_call:
		{
			CRtuAttrHydrInDepthGW *pTab = pWnd->GetRtuAttrHydrrProSet()->GetHydrTab_InDepthGW();
			if (pTab != NULL || pTab->GetSafeHwnd())
			{
				pTab->UpdateDlg(attrType, TRUE);
				pTab->Update();
				m_pCurrCallOP = scsw_no_call_operation;
			}
		}
		break;
	case scsw_waterGWT_call:
		{
			CRtuAttrHydrInTGW *pTab = pWnd->GetRtuAttrHydrrProSet()->GetHydrTab_InTGW();
			if (pTab != NULL || pTab->GetSafeHwnd())
			{
				pTab->UpdateDlg(attrType, TRUE);
				pTab->Update();
				m_pCurrCallOP = scsw_no_call_operation;
			}
		}
		break;
	case scsw_waterEvaporate_call:
		{
			CRtuAttrHydrEvaporate *pTab = pWnd->GetRtuAttrHydrrProSet()->GetHydrTab_Evaporate();
			if (pTab != NULL || pTab->GetSafeHwnd())
			{
				pTab->UpdateDlg(attrType, TRUE);
				pTab->Update();
				m_pCurrCallOP = scsw_no_call_operation;
			}
		}
		break;
	case scsw_waterRainStardard: 
	case scsw_waterRainSensorDpi: 
	case scsw_waterRainSensorBase: 
	case scsw_waterRainSensorPara: 
	case scsw_waterDefine: 
	case scsw_waterrainTransStardard: 
	case scsw_waterrainTimespan:
	case scsw_waterCheckTime:
		{
			RtuAttrHydrTransCtrl *pTab = pWnd->GetRtuAttrHydrrProSet()->GetHydrTransCtrl();
			if (pTab != NULL || pTab->GetSafeHwnd())
			{
				pTab->UpdateDlg(attrType, TRUE);
				pTab->Update(attrType);
				m_pCurrCallOP = scsw_no_call_operation;
			}
		}
		break;
	case scsw_workconditionSensorVar : 
	case scsw_workconditionMemoryMargin : 
	case scsw_workconditionEvoirTemperature : 
	case scsw_workconditionSolarCellVoltage : 
	case scsw_workconditionStorageVoltage : 
	case scsw_workconditionRainCntVar: 
	case scsw_workconditionSensorInstallStatus:
	case scsw_workconditionMemoryPtr:
		{
			 CRtuAttrHydrWorkCondition *pTab = pWnd->GetRtuAttrHydrrProSet()->GetHydrWorkCondition();
			if (pTab != NULL || pTab->GetSafeHwnd())
			{
				pTab->UpdateDlg(attrType, TRUE);
				pTab->Update(attrType);
				m_pCurrCallOP = scsw_no_call_operation;
			}

		}
		break;
	case scsw_interface_GSM:
	case scsw_interface_BD:
	case scsw_interface_IP:
	case scsw_interface_IP2:
	case scsw_interface_PORT:
	case scsw_interface_GSM_Set:
	case scsw_interface_IP_Set:
	case scsw_interface_BD_Set:
	case scsw_interface_PORT_Set:
	case scsw_interface_GSM_Del:
	case scsw_interface_BD_Del:
	case scsw_interface_IP_Del:
		{
			CRtuAttrHydrInterfaceInfo *pTab = pWnd->GetRtuAttrHydrrProSet()->GetHydrInterfaceInfo();
			if (pTab != NULL || pTab->GetSafeHwnd())
			{
				pTab->UpdateDlg(attrType, TRUE);
				pTab->Update(attrType);
				m_pCurrCallOP = scsw_no_call_operation;
			}
		}
		break;
	}
}

void CSysDataTrans_Hydr::RenewCallOperation(CRtuStation* pRTU, int attrType)
{
	CRtuPropertyDlg* pWnd = GetRtuPropertyDlg();
	if (pWnd == NULL  ||
		pWnd->GetRtuStation() == NULL ||
		pWnd->GetRtuStation() != pRTU)
		return;

	switch (attrType)
	{
	case scsw_waterLevel_call:
		{
			CRtuAttrHydrWaterLevel *pTab = pWnd->GetRtuAttrHydrrProSet()->GetHydrTab_WaterLevel();
			if ( pTab != NULL || pTab->GetSafeHwnd())
			{
				pTab->UpdateDlg(attrType, 0xff);
				m_pCurrCallOP = scsw_no_call_operation;
			}
		}
		break;
	case scsw_waterRain_call:
		{
			CRtuAttrHydrRain *pTab = pWnd->GetRtuAttrHydrrProSet()->GetHydrTab_RainValume();
			if (pTab != NULL || pTab->GetSafeHwnd())
			{
				pTab->UpdateDlg(attrType, 0xff);
				m_pCurrCallOP = scsw_no_call_operation;
			}
		}
		break;
	case scsw_waterSoil_call:
		{
			CRtuAttrHydrSoilMoisture *pTab = pWnd->GetRtuAttrHydrrProSet()->GetHydrTab_SoilMoisture();
			if (pTab != NULL || pTab->GetSafeHwnd())
			{
				pTab->UpdateDlg(attrType, 0xff);
				m_pCurrCallOP = scsw_no_call_operation;
			}
		}
		break;
	case scsw_waterGWD_call:
		{
			CRtuAttrHydrInDepthGW *pTab = pWnd->GetRtuAttrHydrrProSet()->GetHydrTab_InDepthGW();
			if (pTab != NULL || pTab->GetSafeHwnd())
			{
				pTab->UpdateDlg(attrType, 0xff);
				m_pCurrCallOP = scsw_no_call_operation;
			}
		}
		break;
	case scsw_waterGWT_call:
		{
			CRtuAttrHydrInTGW *pTab = pWnd->GetRtuAttrHydrrProSet()->GetHydrTab_InTGW();
			if (pTab != NULL || pTab->GetSafeHwnd())
			{
				pTab->UpdateDlg(attrType, 0xff);
				m_pCurrCallOP = scsw_no_call_operation;
			}
		}
		break;
	case scsw_waterEvaporate_call:
		{
			CRtuAttrHydrEvaporate *pTab = pWnd->GetRtuAttrHydrrProSet()->GetHydrTab_Evaporate();
			if (pTab != NULL || pTab->GetSafeHwnd())
			{
				pTab->UpdateDlg(attrType, 0xff);
				m_pCurrCallOP = scsw_no_call_operation;
			}
		}
		break;
	case scsw_waterRainStardard: 
	case scsw_waterRainSensorDpi: 
	case scsw_waterRainSensorBase: 
	case scsw_waterRainSensorPara: 
	case scsw_waterDefine: 
	case scsw_waterrainTransStardard: 
	case scsw_waterrainTimespan:
	case scsw_waterCheckTime:
		{
			RtuAttrHydrTransCtrl *pTab = pWnd->GetRtuAttrHydrrProSet()->GetHydrTransCtrl();
			if (pTab != NULL || pTab->GetSafeHwnd())
			{
				pTab->UpdateDlg(attrType, 0xff);
				m_pCurrCallOP = scsw_no_call_operation;
			}
		}
		break;
	case scsw_workconditionSensorVar : 
	case scsw_workconditionMemoryMargin : 
	case scsw_workconditionEvoirTemperature : 
	case scsw_workconditionSolarCellVoltage : 
	case scsw_workconditionStorageVoltage : 
	case scsw_workconditionRainCntVar: 
	case scsw_workconditionSensorInstallStatus:
	case scsw_workconditionMemoryPtr:
		{
			CRtuAttrHydrWorkCondition *pTab = pWnd->GetRtuAttrHydrrProSet()->GetHydrWorkCondition();
			if (pTab != NULL || pTab->GetSafeHwnd())
			{
				pTab->UpdateDlg(attrType, 0xff);
				m_pCurrCallOP = scsw_no_call_operation;
			}

		}
		break;
	case scsw_interface_GSM:
	case scsw_interface_BD:
	case scsw_interface_IP:
	case scsw_interface_PORT:
	case scsw_interface_GSM_Set:
	case scsw_interface_IP_Set:
	case scsw_interface_BD_Set:
	case scsw_interface_PORT_Set:
	case scsw_interface_GSM_Del:
	case scsw_interface_BD_Del:
	case scsw_interface_IP_Del:
		{
			CRtuAttrHydrInterfaceInfo *pTab = pWnd->GetRtuAttrHydrrProSet()->GetHydrInterfaceInfo();
			if (pTab != NULL || pTab->GetSafeHwnd())
			{
				pTab->UpdateDlg(attrType, 0xff);
				m_pCurrCallOP = scsw_no_call_operation;
			}

		}
		break;
	}
}

//获取遥测终端属性操作窗口
//在属性操作窗口上可以进行相应的单机调试功能
CRtuPropertyDlg* CSysDataTrans_Hydr::GetRtuPropertyDlg()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	if (pMainFrame)
		return pMainFrame->GetRtuPropertyDlg();
	return NULL;
}

//更新遥测终端的通信状态
//重点:更新列表信息和添加通信状态改变日志信息
void CSysDataTrans_Hydr::UpdateRtuCommState(CRtuStation* pSub)
{
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	CMainFrame* pMainFrame = (CMainFrame*)pApp->m_pMainWnd;

	if (pMainFrame && pMainFrame->m_pRightView && pMainFrame->m_pRightView->GetSafeHwnd())
		pMainFrame->m_pRightView->UpdateCommState(pSub);
}

void CSysDataTrans_Hydr::UpdateRtuCommInfo(CRtuStation* pSub)
{
	if (GetRtuPropertyDlg() == NULL ||
		GetRtuPropertyDlg()->GetRtuStation() == NULL ||
		GetRtuPropertyDlg()->GetRtuStation() != pSub)
		return;

	if (GetRtuPropertyDlg()->GetTab_AttrBasicInfo()->GetSafeHwnd())
		GetRtuPropertyDlg()->GetTab_AttrBasicInfo()->UpdateCommInfo();
}

//---------------------协议处理部分---------------------------

//查询遥测终端的水位值（响应帧）
void CSysDataTrans_Hydr::SCSW_hydrdata_Z(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print)
{//水位处理
	if (NULL == pInfo) 
		return;

	CHydrWaterLevel *pData = pHydr->m_pStation->GetHydrWaterLevel();
	if (NULL == pData)
		return;

	int nSersorNo = 0;
	int nTemp     = 0;
	float waterPos = 0.0f;

	nSersorNo = pInfo->m_nExtraInfo;

	pInfo->m_cs.Lock();
	nTemp = atoi(pInfo->m_OpDataList.front());
	pInfo->m_cs.Unlock();

    waterPos = ((float)nTemp);



	CString result = "";

	switch(pHydr->m_TimeType)
	{
	case SCSW_TT:

		if (pHydr->m_RR == 0)
		{
			CString rrStr = "";
			pInfo->m_cs.Lock();
			rrStr.Format("%d号水位传感器人工置数值为：%d\r\n\r\n", nSersorNo, (int)waterPos);
			pInfo->m_cs.Unlock();
			print = print + rrStr;
			pHydr->m_RR = -1;
			return;
		}

		result.Format("%d号传感器的水位为：%.2f\r\n", nSersorNo, waterPos);
		pData->SetZero();
		pData->SetSensorNo(nSersorNo);
		struct tm t;
		t.tm_mon = pHydr->m_DataTimeTT.month;
		t.tm_mday = pHydr->m_DataTimeTT.day;
		t.tm_hour = pHydr->m_DataTimeTT.hour;
		t.tm_min = pHydr->m_DataTimeTT.min;
		pData->SetTime(t);
		break;
	case SCSW_TM:
		{
			result.Format("%d号传感器在最大极值时间TM %d月%d日%d时%d分的水位值为：%.2f  \r\n",
				nSersorNo,
				pHydr->m_DataTimeTM.month,
				pHydr->m_DataTimeTM.day,
				pHydr->m_DataTimeTM.hour,
				pHydr->m_DataTimeTM.min,
				waterPos);

			pData->SetZero();
			pData->SetSensorNo(nSersorNo);
			struct tm t;
			t.tm_mon = pHydr->m_DataTimeTM.month;
			t.tm_mday = pHydr->m_DataTimeTM.day;
			t.tm_hour = pHydr->m_DataTimeTM.hour;
			t.tm_min = pHydr->m_DataTimeTM.min;
			pData->SetTime(t);

		    pHydr->m_DataTimeTM.SetZero();
			pHydr->m_TimeType = SCSW_TT;
		}
		break;
	case SCSW_TN:
			{
				result.Format("%d号传感器在最小极值时间TN %d月%d日%d时%d分的水位值为：%.2f  \r\n",
					nSersorNo,
					pHydr->m_DataTimeTN.month,
					pHydr->m_DataTimeTN.day,
					pHydr->m_DataTimeTN.hour,
					pHydr->m_DataTimeTN.min,
					waterPos);

				pData->SetZero();
				pData->SetSensorNo(nSersorNo);
				struct tm t;
				t.tm_mon = pHydr->m_DataTimeTN.month;
				t.tm_mday = pHydr->m_DataTimeTN.day;
				t.tm_hour = pHydr->m_DataTimeTN.hour;
				t.tm_min = pHydr->m_DataTimeTN.min;
				pData->SetTime(t);

				pHydr->m_DataTimeTN.SetZero();
				pHydr->m_TimeType = SCSW_TT;
			}
		break;
	case SCSW_TE:
		{
			result.Format("%d号传感器在 %d月%d日%d时%d分~%d月%d日%d时%d分间的水位值为：%.2f  \r\n",
				nSersorNo,
				pHydr->m_DataTimeTB.month,
				pHydr->m_DataTimeTB.day,
				pHydr->m_DataTimeTB.hour,
				pHydr->m_DataTimeTB.min,
				pHydr->m_DataTimeTE.month,
				pHydr->m_DataTimeTE.day,
				pHydr->m_DataTimeTE.hour,
				pHydr->m_DataTimeTE.min,
				waterPos);

			pData->SetZero();
			pData->SetSensorNo(nSersorNo);
			struct tm t;
			t.tm_mon = pHydr->m_DataTimeTB.month;
			t.tm_mday = pHydr->m_DataTimeTB.day;
			t.tm_hour = pHydr->m_DataTimeTB.hour;
			t.tm_min = pHydr->m_DataTimeTB.min;
			pData->SetTime(t);

			pHydr->m_DataTimeTB.SetZero();
			pHydr->m_DataTimeTE.SetZero();
			pHydr->m_TimeType = SCSW_TT;
		}
		break;

	}
	
	print = print + result + "\r\n";
	pData->SetRealValue(waterPos);

	if (pHydr->m_pStation)
	{
		UpdateAttribFrame(pHydr->m_pStation, scsw_waterLevel_call);	
	}
		
}

void CSysDataTrans_Hydr::SCSW_hydrdata_PN(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print)
{
	if (NULL == pInfo || NULL == pHydr || NULL == pHydr->m_pStation) 
		return;

	int nTimeSpan = pInfo->m_nExtraInfo;
	CString head = "";
	head.Format(_T("%d分钟时段雨量增量为：\r\n"), nTimeSpan);

	CString rainTime = "";
	CString temp = "";
	CString result = "";
	tm t, t_temp;
	t.tm_hour = pHydr->m_DataTimeTT.hour;
	t.tm_min  = pHydr->m_DataTimeTT.min;
	

	int rainValue = 0;
	std::list<CString>::iterator it;
	pInfo->m_cs.Lock();
	std::list<CString>::iterator it_begin = pInfo->m_OpDataList.begin();
	std::list<CString>::iterator it_end   = pInfo->m_OpDataList.end();
	pInfo->m_cs.Unlock();

	CHydrRainValue *pData = pHydr->m_pStation->GetHydrRainVal();
	if (NULL == pData) return;

	pData->SetDw(0);
	pData->SetSpan(nTimeSpan);
	pData->SetTime(t);
	pData->Clear();

	for (it=it_begin; it!=it_end; it++)
	{
		pInfo->m_cs.Lock();
		rainValue = atoi((CString)(*it));
		pInfo->m_cs.Unlock();

		if (pHydr->m_nCot != SCSW_DC)
		{
			pData->m_mutex.Lock();
			pData->m_lstRainVal.push_back(rainValue);
			pData->m_mutex.Unlock();
		}
		
		t_temp = t;
		rainTime = CUdfGlobalFUNC::CalcTime(&t, nTimeSpan);
		temp.Format("[ %s-%.2d:%.2d ]为：%d 毫米/10； \r\n", rainTime, t_temp.tm_hour, t_temp.tm_min, rainValue);
		result = result + temp;

	}

	print = print + head + result + "\r\n";

	if (pHydr->m_nCot != SCSW_DC)
	{
		UpdateAttribFrame(pHydr->m_pStation, scsw_waterRain_call);
	}
	
}

void CSysDataTrans_Hydr::SCSW_hydrdata_P(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print)
{
	if (NULL == pInfo || NULL == pHydr || NULL == pHydr->m_pStation) 
		return;

	int nTimeSpan = pInfo->m_nExtraInfo;
	CString head = "";
	head.Format(_T("%d分钟时段雨量增量为：\r\n"), nTimeSpan);

	CString rainTime = "";
	CString temp = "";
	CString result = "";
	tm t, t_temp;
	t.tm_hour = pHydr->m_DataTimeTT.hour;
	t.tm_min  = pHydr->m_DataTimeTT.min;
	t.tm_mday = pHydr->m_DataTimeTT.day;
	t.tm_mon = pHydr->m_DataTimeTT.month;

	int rainValue = 0;

	std::list<CString>::iterator it;
	pInfo->m_cs.Lock();
	std::list<CString>::iterator it_begin = pInfo->m_OpDataList.begin();
	std::list<CString>::iterator it_end   = pInfo->m_OpDataList.end();
	pInfo->m_cs.Unlock();

	CHydrRainValue *pData = pHydr->m_pStation->GetHydrRainVal();
	if (NULL == pData) return;

	if (pHydr->m_RR == 0)
	{
		CString rrStr = "";
		pInfo->m_cs.Lock();
		rrStr.Format("雨量人工置数值为：%d\r\n\r\n", atoi((CString)(*it_begin)));
		pInfo->m_cs.Unlock();
		print = print + rrStr;
		pHydr->m_RR = -1;
		return;
	}

	pData->SetDw(1);
	pData->SetSpan(nTimeSpan);
	pData->SetTime(t);
	pData->Clear();
	for (it=it_begin; it!=it_end; it++)
	{
		pInfo->m_cs.Lock();
		rainValue = atoi((CString)(*it));
		pInfo->m_cs.Unlock();

		pData->m_mutex.Lock();
		pData->m_lstRainVal.push_back(rainValue);
		pData->m_mutex.Unlock();

		t_temp = t;
		rainTime = CUdfGlobalFUNC::CalcTime(&t, nTimeSpan, 2);
		temp.Format("[ %s-%.2d:%.2d ]为：%d 毫米/10； \r\n", rainTime, t_temp.tm_hour, t_temp.tm_min, rainValue*10);
		result = result + temp;

	}

	print = print + head + result + "\r\n";

	if (pHydr->m_nCot != SCSW_DC)
	{
		UpdateAttribFrame(pHydr->m_pStation, scsw_waterRain_call);
	}
}

void CSysDataTrans_Hydr::SCSW_hydrdata_M10(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print)
{
	if (NULL==pHydr || NULL==pInfo || NULL == pHydr->m_pStation)
		return;

	int deepth = 0;
	int argValue = 0;
	CString result = "";

	CHydrSoilMoisture *pData = pHydr->m_pStation->GetHydrSoilMoisture();
	if (pData == NULL) return;


	if (pInfo->m_OpDataList.size() == 0)
	{
		print += "土地墒情数据异常";
		return;
	}

	deepth = pInfo->m_nExtraInfo;
	pInfo->m_cs.Lock();
	argValue = atoi(pInfo->m_OpDataList.front());
	pInfo->m_cs.Unlock();

	result.Format("%d厘米的土地墒情%d\r\n", deepth, argValue);
	print = print + result + "\r\n";

	struct tm t;
	t.tm_mon = pHydr->m_DataTimeTT.month;
	t.tm_mday = pHydr->m_DataTimeTT.day;
	t.tm_hour = pHydr->m_DataTimeTT.hour;
	t.tm_min = pHydr->m_DataTimeTT.min;

	pData->SetTime(t);
	pData->SetDepth(deepth);
	pData->SetSoilVar(argValue);

	UpdateAttribFrame(pHydr->m_pStation, scsw_waterSoil_call);

}

void CSysDataTrans_Hydr::SCSW_hydrdata_H(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print)
{
	if (NULL==pHydr || NULL==pInfo)
		return;

	int value = 0;
	CString result = "";

	if (pInfo->m_OpDataList.size() == 0)
	{
		print += "地下水瞬时埋深数据异常";
		return;
	}

	pInfo->m_cs.Lock();
	value = atoi(pInfo->m_OpDataList.front());
	pInfo->m_cs.Unlock();

	result.Format("地下水瞬时埋深%d厘米\r\n",value);

	print = print + result + "\r\n";

	CHydrGWD *pData = pHydr->m_pStation->GetHydrGWD();
	if (pData == NULL) return;

	struct tm t;
	t.tm_mon = pHydr->m_DataTimeTT.month;
	t.tm_mday = pHydr->m_DataTimeTT.day;
	t.tm_hour = pHydr->m_DataTimeTT.hour;
	t.tm_min = pHydr->m_DataTimeTT.min;

	pData->SetTime(t);
	pData->SetGWD(value);
	UpdateAttribFrame(pHydr->m_pStation, scsw_waterGWD_call);
}

void CSysDataTrans_Hydr::SCSW_hydrdata_C(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print)
{
	if (NULL==pHydr || NULL==pInfo)
		return;

	int value = 0;
	CString result = "";

	if (pInfo->m_OpDataList.size() == 0)
	{
		print += "地下水瞬时温度数据异常";
		return;
	}

	pInfo->m_cs.Lock();
	value = atoi(pInfo->m_OpDataList.front());
	pInfo->m_cs.Unlock();

	result.Format("地下水瞬时温度%df度\r\n",value*10);

	print = print + result;

	CHydrGWT *pData = pHydr->m_pStation->GetHydrGWT();
	if (pData == NULL) return;

	struct tm t;
	t.tm_mon = pHydr->m_DataTimeTT.month;
	t.tm_mday = pHydr->m_DataTimeTT.day;
	t.tm_hour = pHydr->m_DataTimeTT.hour;
	t.tm_min = pHydr->m_DataTimeTT.min;

	pData->SetTime(t);
	pData->SetGWT(value);
	UpdateAttribFrame(pHydr->m_pStation, scsw_waterGWT_call);
}

void CSysDataTrans_Hydr::SCSW_hydrdata_ED(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print)
{
	if (NULL==pHydr || NULL==pInfo)
		return;

	int value = 0;
	CString result = "";

	if (pInfo->m_OpDataList.size() == 0)
	{
		print += "蒸发量数据异常";
		return;
	}

	pInfo->m_cs.Lock();
	value = atoi(pInfo->m_OpDataList.front());
	pInfo->m_cs.Unlock();
	result.Format("蒸发量%d毫米\r\n",value*10);

	print = print + result + "\r\n";

	CHydrEvaporate *pData = pHydr->m_pStation->GetHydrEvaporate();
	if (pData == NULL) return;

	struct tm t;
	t.tm_mon = pHydr->m_DataTimeTT.month;
	t.tm_mday = pHydr->m_DataTimeTT.day;
	t.tm_hour = pHydr->m_DataTimeTT.hour;
	t.tm_min = pHydr->m_DataTimeTT.min;

	pData->SetTime(t);
	pData->SetEvaporate(value);
	UpdateAttribFrame(pHydr->m_pStation, scsw_waterGWT_call);

}

void CSysDataTrans_Hydr::SCSW_hydrdata_ZR(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print)
{
	if (NULL==pHydr || NULL==pInfo)
		return;

	int value = 0;
	int sensorNo = 0;
	CString result = "";

	if (pInfo->m_OpDataList.size() == 0)
	{
		print += "水位传感器读数数据异常";
		return;
	}

	pInfo->m_cs.Lock();
	value = atoi(pInfo->m_OpDataList.front());
	pInfo->m_cs.Unlock();

	sensorNo = pInfo->m_nExtraInfo;
	result.Format("%d号传感器的水位传感器读数为：%.2f\r\n", sensorNo, (float)value);

	print = print + result + "\r\n";

	if (pHydr->m_pStation)
	{
		CWorkCondition *pData = pHydr->m_pStation->GetSensorVar();
		if (pData != NULL)
		{
			pData->SetZero();
			pData->SetData(value);
			UpdateAttribFrame(pHydr->m_pStation, scsw_workconditionSensorVar);
		}

	}


}
void CSysDataTrans_Hydr::SCSW_hydrdata_RC(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print)
{
	if (NULL==pHydr || NULL==pInfo)
		return;

	int value = 0;
	CString result = "";

	if (pInfo->m_OpDataList.size() == 0)
	{
		print += "雨量计数器累计值数据异常";
		return;
	}

	pInfo->m_cs.Lock();
	value = atoi(pInfo->m_OpDataList.front());
	pInfo->m_cs.Unlock();

	result.Format("雨量计数器累计值%d  毫米  \r\n",value);

	print = print + result + "\r\n";

	if (pHydr->m_pStation)
	{
		CWorkCondition *pData = pHydr->m_pStation->GetRainCntVar();
		if (pData != NULL)
		{
			pData->SetZero();
			pData->SetData(value);
			UpdateAttribFrame(pHydr->m_pStation, scsw_workconditionRainCntVar);
		}

	}

}

void CSysDataTrans_Hydr::SCSW_hydrdata_ML(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print)
{
	if (NULL==pHydr || NULL==pInfo)
		return;

	int value = 0;
	CString result = "";

	if (pInfo->m_OpDataList.size() == 0)
	{
		print += "数据存储器指针数据异常";
		return;
	}

	pInfo->m_cs.Lock();
	value = atoi(pInfo->m_OpDataList.front());
	pInfo->m_cs.Unlock();


	result.Format("数据存储器指针%d\r\n", value);

	print = print + result + "\r\n";

	if (pHydr->m_pStation)
	{
		CWorkCondition *pData = pHydr->m_pStation->GetDataStoragePtr();
		if (pData != NULL)
		{
			pData->SetZero();
			pData->SetData(value);
			UpdateAttribFrame(pHydr->m_pStation, scsw_workconditionMemoryPtr);
		}

	}
}

void CSysDataTrans_Hydr::SCSW_hydrdata_MF(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print)
{
	if (NULL==pHydr || NULL==pInfo)
		return;

	int value = 0;
	CString result = "";

	if (pInfo->m_OpDataList.size() == 0)
	{
		print += "数据存储器余量数据异常";
		return;
	}

	pInfo->m_cs.Lock();
	value = atoi(pInfo->m_OpDataList.front());
	pInfo->m_cs.Unlock();

	result.Format(_T("数据存储器余量%d字节\r\n"), value);

	print = print + result + "\r\n";

	if (pHydr->m_pStation)
	{
		CWorkCondition *pData = pHydr->m_pStation->GetMemoryMargin();
		if (pData != NULL)
		{
			pData->SetZero();
			pData->SetData(value);
			UpdateAttribFrame(pHydr->m_pStation, scsw_workconditionMemoryMargin);
		}

	}
}

void CSysDataTrans_Hydr::SCSW_hydrdata_BV(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print)
{
	if (NULL==pHydr || NULL==pInfo)
		return;

	int value = 0;
	CString result = "";

	if (pInfo->m_OpDataList.size() == 0)
	{
		print += "蓄电池电压数据异常";
		return;
	}

	pInfo->m_cs.Lock();
	value = atoi(pInfo->m_OpDataList.front());
	pInfo->m_cs.Unlock();


	result.Format("蓄电池电压为：%.2f伏\r\n", (float)value / 10);

	print = print + result + "\r\n";

	if (pHydr->m_pStation)
	{
		CWorkCondition *pData = pHydr->m_pStation->GetStorageVoltage();
		if (pData != NULL)
		{
			pData->SetZero();
			pData->SetData((float)value/10);
			UpdateAttribFrame(pHydr->m_pStation, scsw_workconditionStorageVoltage);
		}

	}
}

void CSysDataTrans_Hydr::SCSW_hydrdata_SV(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print)
{
	if (NULL==pHydr || NULL==pInfo)
		return;

	int value = 0;
	CString result = "";

	if (pInfo->m_OpDataList.size() == 0)
	{
		print += "太阳能电池电压数据异常";
		return;
	}

	pInfo->m_cs.Lock();
	value = atoi(pInfo->m_OpDataList.front());
	pInfo->m_cs.Unlock();

	result.Format("太阳能电池电压为：%.2f伏\r\n", (float)value / 10);

	print = print + result + "\r\n";

	if (pHydr->m_pStation)
	{
		CWorkCondition *pData = pHydr->m_pStation->GetSolarCellVoltage();
		if (pData != NULL)
		{
			pData->SetZero();
			pData->SetData((float)value/10);
			UpdateAttribFrame(pHydr->m_pStation, scsw_workconditionSolarCellVoltage);
		}

	}
}

void CSysDataTrans_Hydr::SCSW_hydrdata_ET(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print)
{
	if (NULL==pHydr || NULL==pInfo)
		return;

	int value = 0;
	CString result = "";

	if (pInfo->m_OpDataList.size() == 0)
	{
		print += "环境温度数据异常";
		return;
	}


	pInfo->m_cs.Lock();
	value = atoi(pInfo->m_OpDataList.front());
	pInfo->m_cs.Unlock();

	result.Format("环境温度为：%d度\r\n", value);
	print = print + result + "\r\n";

	if (pHydr->m_pStation)
	{
		CWorkCondition *pData = pHydr->m_pStation->GetEviorTemperation();
		if (pData != NULL)
		{
			pData->SetZero();
			pData->SetData(value);
			UpdateAttribFrame(pHydr->m_pStation, scsw_workconditionEvoirTemperature);
		}

	}
}

void CSysDataTrans_Hydr::SCSW_hydrdata_SI(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print)
{
	if (NULL==pHydr || NULL==pInfo)
		return;

	int nComType = pInfo->m_nExtraInfo;
	CString head = "";

	switch(nComType)
	{
	case 1:
		head = _T("GSM短信信道指标为：");
		break;
	case 2:
		head = _T("北斗卫星信道指标为：");
		break;
	case 3:
		head = _T("PSTN信道指标为：");
		break;
	case 4:
		head = _T("GPRS信道指标为：");
		break;
	default:
		head.Format(_T("未知信道[%d]指标："), nComType);
		break;

	}

	if (pInfo->m_OpDataList.size() == 0)
	{
		print += "信道数据异常";
		return;
	}

	int data = 0;
	pInfo->m_cs.Lock();
	data = atoi(pInfo->m_OpDataList.front());
	pInfo->m_cs.Unlock();

	CString var = "";
	var.Format("%d", data);

	print = print + head + var + "\r\n";

}

void CSysDataTrans_Hydr::SCSW_hydrdata_PI(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print)
{
	if (NULL==pHydr || NULL==pInfo)
		return;

	float value = 0.0;
	CString result = "";

	if (pInfo->m_OpDataList.size() == 0)
	{
		print += "雨量加报标准数据异常";
		return;
	}

	pInfo->m_cs.Lock();
	value = atoi(pInfo->m_OpDataList.front());
	pInfo->m_cs.Unlock();

	result.Format("雨量加报标准：%.2f毫米\r\n", value);
	print = print + result + "\r\n";

	if (pHydr->m_pStation)
	{
		CHydrTransCtrlData *pData = pHydr->m_pStation->GetRainStardard();
		if (pData != NULL)
		{
			pData->SetZero();
			pData->SetCtrlData(value);
			UpdateAttribFrame(pHydr->m_pStation, scsw_waterRainStardard);
		}

	}

}

void CSysDataTrans_Hydr::SCSW_hydrdata_RD(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print)
{
	if (NULL==pHydr || NULL==pInfo)
		return;

	int value = 0;
	CString result = "";

	if (pInfo->m_OpDataList.size() == 0)
	{
		print += "雨量传感器分辨力数据异常";
		return;
	}

	pInfo->m_cs.Lock();
	value = atoi(pInfo->m_OpDataList.front());
	pInfo->m_cs.Unlock();

	result.Format("雨量传感器分辨力为：%.2d  \r\n", value);
	print = print + result + "\r\n";

	if (pHydr->m_pStation)
	{
		CHydrTransCtrlData *pData = pHydr->m_pStation->GetRainSensorDpi();
		if (pData != NULL)
		{
			pData->SetZero();
			pData->SetCtrlData(value);
			UpdateAttribFrame(pHydr->m_pStation, scsw_waterRainSensorDpi);
		}

	}

}

void CSysDataTrans_Hydr::SCSW_hydrdata_ZJ(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print)
{
	if (NULL==pHydr || NULL==pInfo)
		return;

	int value = 0;
	int sensorNo = 0;
	CString result = "";

	if (pInfo->m_OpDataList.size() == 0)
	{
		print += "水位传感器的基数数据异常";
		return;
	}

	pInfo->m_cs.Lock();
	value = atoi(pInfo->m_OpDataList.front());
	pInfo->m_cs.Unlock();

	sensorNo = pInfo->m_nExtraInfo;

	result.Format("%d号水位传感器的基数为：%.2d厘米\r\n", sensorNo, value);
	print = print + result + "\r\n";

	if (pHydr->m_pStation)
	{
		CHydrTransCtrlData *pData = pHydr->m_pStation->GetRainSensorBase();
		if (pData != NULL)
		{
			pData->SetZero();
			pData->SetCtrlData(value);
			UpdateAttribFrame(pHydr->m_pStation, scsw_waterRainSensorBase);
		}

	}

}

void CSysDataTrans_Hydr::SCSW_hydrdata_ZT(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print)
{
	if (NULL==pHydr || NULL==pInfo)
		return;

	int value = 0;
	int sensorNo = 0;
	CString result = "";


	if (pInfo->m_OpDataList.size() == 0)
	{
		print += "水位传感器转换系数数据异常";
		return;
	}

	pInfo->m_cs.Lock();
	value = atoi(pInfo->m_OpDataList.front());
	pInfo->m_cs.Unlock();

	sensorNo = pInfo->m_nExtraInfo;

	result.Format("%d号水位传感器转换系数为：%.2d厘米\r\n", sensorNo, value);
	print = print + result + "\r\n";

	if (pHydr->m_pStation)
	{
		CHydrTransCtrlData *pData = pHydr->m_pStation->GetRainSensorPara();
		if (pData != NULL)
		{
			pData->SetZero();
			pData->SetCtrlData(value);
			UpdateAttribFrame(pHydr->m_pStation, scsw_waterRainSensorPara);
		}

	}

}

void CSysDataTrans_Hydr::SCSW_hydrdata_ZF(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print)
{
	if (NULL==pHydr || NULL==pInfo)
		return;

	int value = 0;
	int sensorNo = 0;
	CString result = "";

	if (pInfo->m_OpDataList.size() == 0)
	{
		print += "水位传感器水公式定义数据异常";
		return;
	}

	pInfo->m_cs.Lock();
	value = atoi(pInfo->m_OpDataList.front());
	pInfo->m_cs.Unlock();

	sensorNo = pInfo->m_nExtraInfo;

	result.Format("%d号水位传感器水公式定义为：%.2d厘米\r\n", sensorNo, value);
	print = print + result + "\r\n";

	if (pHydr->m_pStation)
	{
		CHydrTransCtrlData *pData = pHydr->m_pStation->GetRainDefine();
		if (pData != NULL)
		{
			pData->SetZero();
			pData->SetCtrlData(value);
			UpdateAttribFrame(pHydr->m_pStation, scsw_waterDefine);
		}

	}


}
void CSysDataTrans_Hydr::SCSW_hydrdata_Zx(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print)
{
	if (NULL==pHydr || NULL==pInfo)
		return;

	int value = 0;
	int sensorNo = 0;
	CString result = "";

	if (pInfo->m_OpDataList.size() == 0)
	{
		print += "水位传感器发送控制标准数据异常";
		return;
	}

	pInfo->m_cs.Lock();
	value = atoi(pInfo->m_OpDataList.front());
	pInfo->m_cs.Unlock();

	sensorNo = pInfo->m_nExtraInfo;

	result.Format("%d号水位传感器发送控制标准为：%.2d\r\n", sensorNo, value);
	print = print + result + "\r\n";

	if (pHydr->m_pStation)
	{
		CHydrTransCtrlData *pData = pHydr->m_pStation->GetRainSendStardard();
		if (pData != NULL)
		{
			pData->SetZero();
			pData->SetCtrlData(value);
			UpdateAttribFrame(pHydr->m_pStation, scsw_waterrainTransStardard);
		}

	}

}

void CSysDataTrans_Hydr::SCSW_hydrdata_DZ(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print)
{
	if (NULL==pHydr || NULL==pInfo)
		return;

	int value = 0;
	int sensorNo = 0;
	CString result = "";

	if (pInfo->m_OpDataList.size() == 0)
	{
		print += "水位发送时间间隔数据异常";
		return;
	}

	pInfo->m_cs.Lock();
	value = atoi(pInfo->m_OpDataList.front());
	pInfo->m_cs.Unlock();

	sensorNo = pInfo->m_nExtraInfo;

	result.Format("%d号水位发送时间间隔为：%.2f\r\n", sensorNo, value);
	print = print + result + "\r\n";

	if (pHydr->m_pStation)
	{
		CHydrTransCtrlData *pData = pHydr->m_pStation->GetRainSendTimeSpan();
		if (pData != NULL)
		{
			pData->SetZero();
			pData->SetCtrlData(value);
			UpdateAttribFrame(pHydr->m_pStation, scsw_waterrainTimespan);
		}

	}

}

void CSysDataTrans_Hydr::SCSW_hydrdata_TT(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print)
{
	if (NULL==pHydr || NULL==pInfo)
		return;

	CString data = "";
	pHydr->m_cs_info.Lock();
	if (pInfo->m_OpDataList.begin() != pInfo->m_OpDataList.end())
	{
		//添加站码
		pHydr->m_TimeType = SCSW_TT;
		data = pInfo->m_OpDataList.front();
	}
	pHydr->m_cs_info.Unlock();

	if (data.GetLength() == 12)
	{
		UpdateAttribFrame(pHydr->m_pStation, scsw_waterCheckTime);
	}
}

void CSysDataTrans_Hydr::SCSW_hydrdata_TM(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print)
{
	if (NULL==pHydr || NULL==pInfo)
		return;

	pHydr->m_cs_info.Lock();
	if (pInfo->m_OpDataList.begin() != pInfo->m_OpDataList.end())
	{
		//添加站码
		pHydr->m_TimeType = SCSW_TM;
		CString data = pInfo->m_OpDataList.front();
		pHydr->m_DataTimeTM.month = atoi(data.Mid(0, 2));
		pHydr->m_DataTimeTM.day   = atoi(data.Mid(2, 2));
		pHydr->m_DataTimeTM.hour  = atoi(data.Mid(4, 2));
		pHydr->m_DataTimeTM.min   = atoi(data.Mid(6, 2));
	}
	pHydr->m_cs_info.Unlock();
}

void CSysDataTrans_Hydr::SCSW_hydrdata_TN(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print)
{
	if (NULL==pHydr || NULL==pInfo)
		return;

	pHydr->m_cs_info.Lock();
	if (pInfo->m_OpDataList.begin() != pInfo->m_OpDataList.end())
	{
		//添加站码
		pHydr->m_TimeType = SCSW_TN;
		CString data = pInfo->m_OpDataList.front();
		pHydr->m_DataTimeTN.month = atoi(data.Mid(0, 2));
		pHydr->m_DataTimeTN.day   = atoi(data.Mid(2, 2));
		pHydr->m_DataTimeTN.hour  = atoi(data.Mid(4, 2));
		pHydr->m_DataTimeTN.min   = atoi(data.Mid(6, 2));
	}
	pHydr->m_cs_info.Unlock();
}
void CSysDataTrans_Hydr::SCSW_hydrdata_TB(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print)
{
	if (NULL==pHydr || NULL==pInfo)
		return;

	pHydr->m_cs_info.Lock();
	if (pInfo->m_OpDataList.begin() != pInfo->m_OpDataList.end())
	{
		//添加站码
		pHydr->m_TimeType = SCSW_TB;
		CString data = pInfo->m_OpDataList.front();
		pHydr->m_DataTimeTB.month = atoi(data.Mid(0, 2));
		pHydr->m_DataTimeTB.day   = atoi(data.Mid(2, 2));
		pHydr->m_DataTimeTB.hour  = atoi(data.Mid(4, 2));
		pHydr->m_DataTimeTB.min   = atoi(data.Mid(6, 2));
	}
	pHydr->m_cs_info.Unlock();
}

void CSysDataTrans_Hydr::SCSW_hydrdata_TE(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print)
{
	if (NULL==pHydr || NULL==pInfo)
		return;

	pHydr->m_cs_info.Lock();
	if (pInfo->m_OpDataList.begin() != pInfo->m_OpDataList.end())
	{
		//添加站码
		pHydr->m_TimeType = SCSW_TE;
		CString data = pInfo->m_OpDataList.front();
		pHydr->m_DataTimeTE.month = atoi(data.Mid(0, 2));
		pHydr->m_DataTimeTE.day   = atoi(data.Mid(2, 2));
		pHydr->m_DataTimeTE.hour  = atoi(data.Mid(4, 2));
		pHydr->m_DataTimeTE.min   = atoi(data.Mid(6, 2));
	}
	pHydr->m_cs_info.Unlock();

}
void CSysDataTrans_Hydr::SCSW_hydrdata_RR(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print)
{
	pHydr->m_RR = 0;
}

void CSysDataTrans_Hydr::SCSW_hydrdata_DV(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print){}
void CSysDataTrans_Hydr::SCSW_hydrdata_DI(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print){}

void CSysDataTrans_Hydr::SCSW_hydrdata_DC(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print)
{
	if (NULL==pHydr || NULL==pInfo)
		return;

	if (pInfo->m_OpDataList.size() == 0)
	{
		print += "自报类型流水号数据异常";
		return;
	}

	pInfo->m_cs.Lock();
	int value = atoi(pInfo->m_OpDataList.front());
	pInfo->m_cs.Unlock();
	
	CString temp = "";
	temp.Format("传送原因：自报类型，流水号=%d\r\n\r\n", value);
	print = print + temp;
}

void CSysDataTrans_Hydr::SCSW_hydrdata_CN(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print)
{
	if (NULL==pHydr || NULL==pInfo)
		return;

	if (pInfo->m_OpDataList.size() == 0)
	{
		print += "响应召测包计数数据异常";
		return;
	}

	pInfo->m_cs.Lock();
	int value = atoi(pInfo->m_OpDataList.front());
	pInfo->m_cs.Unlock();

	CString temp = "";
	temp.Format("传送原因：响应召测，包计数=%d\r\n\r\n", value);
	print = print + temp;


}

void CSysDataTrans_Hydr::SCSW_hydrdata_CP(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print){}

void CSysDataTrans_Hydr::SCSW_hydrdata_GSM(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print)
{
	if (NULL==pHydr || NULL==pInfo)
		return;

	CString value = "";
	int sensorNo = 0;
	CString result = "";

	if (pInfo->m_OpDataList.size() == 0)
	{
		print += "GSM短信传输地址序号数据异常";
		return;
	}

	pInfo->m_cs.Lock();
	value = pInfo->m_OpDataList.front();
	pInfo->m_cs.Unlock();

	sensorNo = pInfo->m_nExtraInfo;

	result.Format("GSM短信传输地址序号为%d的目标地址为：%s\r\n", sensorNo, value);
	print = print + result + "\r\n";

	if (pHydr->m_pStation)
	{
		CInterfaceInfo *pData = pHydr->m_pStation->GetGSMValue();
		if (pData != NULL)
		{
			pData->SetZero();
			pData->SetData(value);
			UpdateAttribFrame(pHydr->m_pStation, scsw_interface_GSM);
		}

	}

}

void CSysDataTrans_Hydr::SCSW_hydrdata_BD(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print)
{
	if (NULL==pHydr || NULL==pInfo)
		return;

	CString value = "";
	int sensorNo = 0;
	CString result = "";

	if (pInfo->m_OpDataList.size() == 0)
	{
		print += "北斗卫星地址序号目标地址数据异常";
		return;
	}


	pInfo->m_cs.Lock();
	value = pInfo->m_OpDataList.front();
	pInfo->m_cs.Unlock();

	sensorNo = pInfo->m_nExtraInfo;

	result.Format("北斗卫星地址序号为%d的目标地址为：%.s\r\n", sensorNo, value);
	print = print + result + "\r\n";

	if (pHydr->m_pStation)
	{
		CInterfaceInfo *pData = pHydr->m_pStation->GetBDValue();
		if (pData != NULL)
		{
			pData->SetZero();
			pData->SetData(value);
			UpdateAttribFrame(pHydr->m_pStation, scsw_interface_BD);
		}

	}


}
void CSysDataTrans_Hydr::SCSW_hydrdata_PSTN(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print){}
void CSysDataTrans_Hydr::SCSW_hydrdata_IT(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print)
{
	if (NULL==pHydr || NULL==pInfo)
		return;

	CString value = "";
	CString result = "";

	std::list<CString>::iterator it;
	std::list<CString>::iterator it_b = pInfo->m_OpDataList.begin();
	std::list<CString>::iterator it_e = pInfo->m_OpDataList.end();

	CString ipAddr = "";
	for (it=it_b; it!=it_e; it++)
	{
		pInfo->m_cs.Lock();
		value = (CString)(*it);
		ipAddr = ipAddr + value + ".";
		pInfo->m_cs.Unlock();
	}
	
	int len = ipAddr.GetLength();
	ipAddr.Delete(len-1, 1);

	result.Format("互联网IP地址为：%s\r\n", ipAddr);
	print = print + result + "\r\n";

	if (pHydr->m_pStation)
	{
		CInterfaceInfo *pData = pHydr->m_pStation->GetIPValue2();
		if (pData != NULL)
		{
			pData->SetZero();
			pData->SetData(ipAddr);
			UpdateAttribFrame(pHydr->m_pStation, scsw_interface_IP2);
		}

	}

}

void CSysDataTrans_Hydr::SCSW_hydrdata_PT(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print)
{
	if (NULL==pHydr || NULL==pInfo)
		return;

	CString value = "";
	CString result = "";

	if (pInfo->m_OpDataList.size() == 0)
	{
		print += "互联网端口号数据异常";
		return;
	}

	pInfo->m_cs.Lock();
	value = pInfo->m_OpDataList.front();
	pInfo->m_cs.Unlock();

	//sensorNo = pInfo->m_nExtraInfo;

	result.Format("互联网端口号为：%s\r\n", value);
	print = print + result + "\r\n";

	if (pHydr->m_pStation)
	{
		CInterfaceInfo *pData = pHydr->m_pStation->GetPortValue();
		if (pData != NULL)
		{
			pData->SetZero();
			pData->SetData(value);
			UpdateAttribFrame(pHydr->m_pStation, scsw_interface_PORT);
		}

	}

}
void CSysDataTrans_Hydr::SCSW_hydrdata_TC(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print){}
void CSysDataTrans_Hydr::SCSW_hydrdata_PW(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print){}

void CSysDataTrans_Hydr::SCSW_hydrdata_ER(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print)
{
	if (NULL==pHydr || NULL==pInfo)
		return;

	CString result = "";
	CString strErrCode = "";

	if (pInfo->m_OpDataList.size() == 0)
	{
		print += "错误代码据异常";
		return;
	}

	pInfo->m_cs.Lock();
	strErrCode = pInfo->m_OpDataList.front();
	pInfo->m_cs.Unlock();

	switch(atoi(strErrCode))
	{
	case 0://保留
		result.Format("错误代码：[0] -保留\r\n");
		break;
	case 1://
		result.Format("错误代码：[1] -语句不完整\r\n");
		break;
	case 2:
		result.Format("错误代码：[2] -语法错误\r\n");
		break;
	case 3:
		result.Format("错误代码：[3] -有不能识别的标示符\r\n");
		break;
	case 4:
		result.Format("错误代码：[4] -参数修改指令中密码错误\r\n");
		break;
	case 5:
		result.Format("错误代码：[5] -非法数据\r\n");
		break;
	case 6:
		result.Format("错误代码：[6] -数据超限\r\n");
		break;
	case 7:
		result.Format("错误代码：[7] -时间数据非法\r\n");
		break;
	case 8:
		result.Format("错误代码：[8] -数据矛盾\r\n");
		break;
	case 9:
		result.Format("错误代码：[9] -指定数据不存在\r\n");
		break;
	case 10:
		result.Format("错误代码：[10] -指定设备不存在\r\n");
		break;
	case 11:
		result.Format("错误代码：[11] -保留\r\n");
		break;
	case 12:
		result.Format("错误代码：[12] -保留\r\n");
		break;
	case 13:
		result.Format("错误代码：[13] -保留\r\n");
		break;
	case 14:
		result.Format("错误代码：[14] -保留\r\n");
		break;
	case 15:
		result.Format("错误代码：[15] -其他错误\r\n");
		break;
	}

	print = print + result + "\r\n";

	if (pHydr->m_pStation)
	{
		RenewCallOperation(pHydr->m_pStation, m_pCurrCallOP);
	}

}

void CSysDataTrans_Hydr::SCSW_hydrdata_RF(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print){}

void CSysDataTrans_Hydr::SCSW_hydrdata_EN_OK(CHydrSendFrame *pSendFrame)
{
	//-----------站码---------------------
	CString strStCode = pSendFrame->m_strStCode;
	char charStCode[11];
	memset(charStCode, 0, 10);
	memcpy(charStCode, strStCode.GetBuffer(10), 10);

	//--------------ST头----------------
	pSendFrame->m_buf[0] = 'S';
	pSendFrame->m_buf[1] = 'T';
	pSendFrame->m_buf[2] = ' ';
	//---------------站码---------------
	pSendFrame->m_buf[3] = charStCode[0];
	pSendFrame->m_buf[4] = charStCode[1];
	pSendFrame->m_buf[5] = charStCode[2];
	pSendFrame->m_buf[6] = charStCode[3];
	pSendFrame->m_buf[7] = charStCode[4];
	pSendFrame->m_buf[8] = charStCode[5];
	pSendFrame->m_buf[9] = charStCode[6];
	pSendFrame->m_buf[10] = charStCode[7];
	pSendFrame->m_buf[11] = charStCode[8];
	pSendFrame->m_buf[12] = charStCode[9];
	pSendFrame->m_buf[13] = ' ';

	pSendFrame->m_buf[14] = 'O';
	pSendFrame->m_buf[15] = 'K';
	pSendFrame->m_buf[16] = ' ';
	pSendFrame->m_buf[17] = 'a';
	pSendFrame->m_buf[18] = 'a';
	pSendFrame->m_buf[19] = 'N';
	pSendFrame->m_buf[20] = 'N';
	pSendFrame->m_bufLen = 21;

	SendRtuLinkFrame(pSendFrame->m_buf, pSendFrame->m_bufLen, pSendFrame->m_CommInterface);
}

void CSysDataTrans_Hydr::SCSW_hydrdata_HOST(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo, CString &print)
{
	if (NULL==pHydr || NULL==pInfo)
		return;

	CString value = "";
	CString result = "";

	if (pInfo->m_OpDataList.size() == 0)
	{
		print += "互联网IP域名异常";
		return;
	}

	pInfo->m_cs.Lock();
	value = pInfo->m_OpDataList.front();
	pInfo->m_cs.Unlock();

	//sensorNo = pInfo->m_nExtraInfo;

	result.Format("互联网IP域名为：%s\r\n", value);
	print = print + result + "\r\n";

	if (pHydr->m_pStation)
	{
		CInterfaceInfo *pData = pHydr->m_pStation->GetIPValue();
		if (pData != NULL)
		{
			pData->SetZero();
			pData->SetData(value);
			UpdateAttribFrame(pHydr->m_pStation, scsw_interface_IP);
		}

	}

}

//----------------------------查询报文---------------------------
void CSysDataTrans_Hydr::SCSW_hydrdata_EN_Z(CHydrSendFrame *pSendFrame)
{//查询水位
	if(NULL == pSendFrame)
		return;

	//-----------传感器号----------------
	CString strSensorNo = "";
	int m_nSensorNo = pSendFrame->m_LPARME;
	strSensorNo.Format("%02d", m_nSensorNo);
	char charSensor[3];
	memset(charSensor, 0, 3);
	memcpy(charSensor, strSensorNo.GetBuffer(2), 2);

	//-----------站码---------------------
	CString strStCode = pSendFrame->m_strStCode;
	char charStCode[11];
	memset(charStCode, 0, 10);
	memcpy(charStCode, strStCode.GetBuffer(10), 10);

	//--------------ST头----------------
	pSendFrame->m_buf[0] = 'S';
	pSendFrame->m_buf[1] = 'T';
	pSendFrame->m_buf[2] = ' ';
	//---------------站码---------------
	pSendFrame->m_buf[3] = charStCode[0];
	pSendFrame->m_buf[4] = charStCode[1];
	pSendFrame->m_buf[5] = charStCode[2];
	pSendFrame->m_buf[6] = charStCode[3];
	pSendFrame->m_buf[7] = charStCode[4];
	pSendFrame->m_buf[8] = charStCode[5];
	pSendFrame->m_buf[9] = charStCode[6];
	pSendFrame->m_buf[10] = charStCode[7];
	pSendFrame->m_buf[11] = charStCode[8];
	pSendFrame->m_buf[12] = charStCode[9];
	pSendFrame->m_buf[13] = ' ';

	switch (pSendFrame->m_nTimeType)
	{
	case TIMETYPE_CURR:
		{
			pSendFrame->m_buf[14] = 'Z';
			pSendFrame->m_buf[15] = charSensor[0];
			pSendFrame->m_buf[16] = charSensor[1];
			pSendFrame->m_buf[17] = ' ';
			pSendFrame->m_buf[18] = '?';
			pSendFrame->m_buf[19] = ' ';
			pSendFrame->m_buf[20] = 'a';
			pSendFrame->m_buf[21] = 'a';
			pSendFrame->m_buf[22] = 'N';
			pSendFrame->m_buf[23] = 'N';
			pSendFrame->m_bufLen = 24;	
		}
		break;
	case TIMETYPE_TT:
		{
			//----------时间---------------------
			CString strTime_a = "";
			strTime_a.Format("%02d%02d%02d%02d", 
				pSendFrame->t_a.wMonth,
				pSendFrame->t_a.wDay,
				pSendFrame->t_a.wHour,
				pSendFrame->t_a.wMinute);
			char charTimeA[9];
			memset(charTimeA, 0, 9);
			memcpy(charTimeA, strTime_a.GetBuffer(8), 8);

			pSendFrame->m_buf[14] = 'T';
			pSendFrame->m_buf[15] = 'T';
			pSendFrame->m_buf[16] = ' ';
			pSendFrame->m_buf[16+1] = charTimeA[0];
			pSendFrame->m_buf[17+1] = charTimeA[1];
			pSendFrame->m_buf[18+1] = charTimeA[2];
			pSendFrame->m_buf[19+1] = charTimeA[3];
			pSendFrame->m_buf[20+1] = charTimeA[4];
			pSendFrame->m_buf[21+1] = charTimeA[5];
			pSendFrame->m_buf[22+1] = charTimeA[6];
			pSendFrame->m_buf[23+1] = charTimeA[7];
			pSendFrame->m_buf[24+1] = ' ';
			pSendFrame->m_buf[25+1] = 'Z';
			pSendFrame->m_buf[26+1] = charSensor[0];
			pSendFrame->m_buf[27+1] = charSensor[1];
			pSendFrame->m_buf[28+1] = ' ';
			pSendFrame->m_buf[29+1] = '?';
			pSendFrame->m_buf[30+1] = ' ';
			pSendFrame->m_buf[31+1] = 'a';
			pSendFrame->m_buf[32+1] = 'a';
			pSendFrame->m_buf[33+1] = 'N';
			pSendFrame->m_buf[34+1] = 'N';
			pSendFrame->m_bufLen = 35+1;	
		}
		break;
	case TIMETYPE_TB:
		{
			//----------时间---------------------
			int year = pSendFrame->t_a.wYear;
			year = year % 100;
			CString strTime_a = "";
			strTime_a.Format("%2d%02d%02d%02d%02d", 
				year,
				pSendFrame->t_a.wMonth,
				pSendFrame->t_a.wDay,
				pSendFrame->t_a.wHour,
				pSendFrame->t_a.wMinute);
			char charTimeA[11];
			memset(charTimeA, 0, 10);
			memcpy(charTimeA, strTime_a.GetBuffer(10), 10);

			year = pSendFrame->t_b.wYear;
			year = year % 100;
			CString strTime_b = "";
			strTime_b.Format("%2d%02d%02d%02d%02d",
				year,
				pSendFrame->t_b.wMonth,
				pSendFrame->t_b.wDay,
				pSendFrame->t_b.wHour,
				pSendFrame->t_b.wMinute);
			char charTimeB[11];
			memset(charTimeB, 0, 10);
			memcpy(charTimeB, strTime_b.GetBuffer(10), 10);

			pSendFrame->m_buf[14] = 'T';
			pSendFrame->m_buf[15] = 'B';
			pSendFrame->m_buf[16] = ' ';
			pSendFrame->m_buf[17] = charTimeA[0];
			pSendFrame->m_buf[18] = charTimeA[1];
			pSendFrame->m_buf[19] = charTimeA[2];
			pSendFrame->m_buf[20] = charTimeA[3];
			pSendFrame->m_buf[21] = charTimeA[4];
			pSendFrame->m_buf[22] = charTimeA[5];
			pSendFrame->m_buf[23] = charTimeA[6];
			pSendFrame->m_buf[24] = charTimeA[7];
			pSendFrame->m_buf[25] = charTimeA[8];
			pSendFrame->m_buf[26] = charTimeA[9];
			pSendFrame->m_buf[27] = ' ';
			pSendFrame->m_buf[28] = 'T';
			pSendFrame->m_buf[29] = 'E';
			pSendFrame->m_buf[30] = ' ';
			pSendFrame->m_buf[31] = charTimeB[0];
			pSendFrame->m_buf[32] = charTimeB[1];
			pSendFrame->m_buf[33] = charTimeB[2];
			pSendFrame->m_buf[34] = charTimeB[3];
			pSendFrame->m_buf[35] = charTimeB[4];
			pSendFrame->m_buf[36] = charTimeB[5];
			pSendFrame->m_buf[37] = charTimeB[6];
			pSendFrame->m_buf[38] = charTimeB[7];
			pSendFrame->m_buf[39] = charTimeB[8];
			pSendFrame->m_buf[40] = charTimeB[9];
			pSendFrame->m_buf[41] = ' ';
			pSendFrame->m_buf[42] = 'Z';
			pSendFrame->m_buf[43] = charSensor[0];
			pSendFrame->m_buf[44] = charSensor[1];
			pSendFrame->m_buf[45] = ' ';
			pSendFrame->m_buf[46] = '?';
			pSendFrame->m_buf[47] = ' ';
			pSendFrame->m_buf[48] = 'a';
			pSendFrame->m_buf[49] = 'a';
			pSendFrame->m_buf[50] = 'N';
			pSendFrame->m_buf[51] = 'N';
			pSendFrame->m_bufLen = 52;	
		}
		break;
	}

	m_pCurrCallOP = scsw_waterLevel_call;
	SendRtuLinkFrame(pSendFrame->m_buf, pSendFrame->m_bufLen, pSendFrame->m_CommInterface);
}

void CSysDataTrans_Hydr::SCSW_hydrdata_EN_P(CHydrSendFrame *pSendFrame)
{
	if(NULL == pSendFrame)
		return;

	//-----------传感器号----------------
	CString strTimeSpan = "";
	int m_nTimeSpan = pSendFrame->m_LPARME;
	strTimeSpan.Format("%02d", m_nTimeSpan);
	char charTimeSpan[3];
	memset(charTimeSpan, 0, 3);
	memcpy(charTimeSpan, strTimeSpan.GetBuffer(2), 2);

	//-----------站码---------------------
	CString strStCode = pSendFrame->m_strStCode;
	char charStCode[11];
	memset(charStCode, 0, 10);
	memcpy(charStCode, strStCode.GetBuffer(10), 10);

	//--------------ST头----------------
	pSendFrame->m_buf[0] = 'S';
	pSendFrame->m_buf[1] = 'T';
	pSendFrame->m_buf[2] = ' ';
	//---------------站码---------------
	pSendFrame->m_buf[3] = charStCode[0];
	pSendFrame->m_buf[4] = charStCode[1];
	pSendFrame->m_buf[5] = charStCode[2];
	pSendFrame->m_buf[6] = charStCode[3];
	pSendFrame->m_buf[7] = charStCode[4];
	pSendFrame->m_buf[8] = charStCode[5];
	pSendFrame->m_buf[9] = charStCode[6];
	pSendFrame->m_buf[10] = charStCode[7];
	pSendFrame->m_buf[11] = charStCode[8];
	pSendFrame->m_buf[12] = charStCode[9];
	pSendFrame->m_buf[13] = ' ';

	switch (pSendFrame->m_nTimeType)
	{
	case TIMETYPE_TT:
		{
			pSendFrame->m_buf[14] = 'P';
			pSendFrame->m_buf[15] = charTimeSpan[0];
			pSendFrame->m_buf[16] = charTimeSpan[1];
			pSendFrame->m_buf[17] = ' ';
			pSendFrame->m_buf[18] = '?';
			pSendFrame->m_buf[19] = ' ';
			pSendFrame->m_buf[20] = 'a';
			pSendFrame->m_buf[21] = 'a';
			pSendFrame->m_buf[22] = 'N';
			pSendFrame->m_buf[23] = 'N';
			pSendFrame->m_bufLen = 24;	
		}
		break;
	case TIMETYPE_TB:
		{
			//----------时间---------------------
			int year = pSendFrame->t_a.wYear;
			year = year % 100;

			CString strTime_a = "";
			strTime_a.Format("%2d%02d%02d%02d%02d", 
				year,
				pSendFrame->t_a.wMonth,
				pSendFrame->t_a.wDay,
				pSendFrame->t_a.wHour,
				pSendFrame->t_a.wMinute);
			char charTimeA[11];
			memset(charTimeA, 0, 10);
			memcpy(charTimeA, strTime_a.GetBuffer(10), 10);

			year = pSendFrame->t_b.wYear;
			year = year % 100;
			CString strTime_b = "";
			strTime_b.Format("%2d%02d%02d%02d%02d",
				year,
				pSendFrame->t_b.wMonth,
				pSendFrame->t_b.wDay,
				pSendFrame->t_b.wHour,
				pSendFrame->t_b.wMinute);
			char charTimeB[11];
			memset(charTimeB, 0, 10);
			memcpy(charTimeB, strTime_b.GetBuffer(10), 10);

			pSendFrame->m_buf[14] = 'T';
			pSendFrame->m_buf[15] = 'B';
			pSendFrame->m_buf[16] = ' ';
			pSendFrame->m_buf[17] = charTimeA[0];
			pSendFrame->m_buf[18] = charTimeA[1];
			pSendFrame->m_buf[19] = charTimeA[2];
			pSendFrame->m_buf[20] = charTimeA[3];
			pSendFrame->m_buf[21] = charTimeA[4];
			pSendFrame->m_buf[22] = charTimeA[5];
			pSendFrame->m_buf[23] = charTimeA[6];
			pSendFrame->m_buf[24] = charTimeA[7];
			pSendFrame->m_buf[25] = charTimeA[8];
			pSendFrame->m_buf[26] = charTimeA[9];
			pSendFrame->m_buf[27] = ' ';
			pSendFrame->m_buf[28] = 'T';
			pSendFrame->m_buf[29] = 'E';
			pSendFrame->m_buf[30] = ' ';
			pSendFrame->m_buf[31] = charTimeB[0];
			pSendFrame->m_buf[32] = charTimeB[1];
			pSendFrame->m_buf[33] = charTimeB[2];
			pSendFrame->m_buf[34] = charTimeB[3];
			pSendFrame->m_buf[35] = charTimeB[4];
			pSendFrame->m_buf[36] = charTimeB[5];
			pSendFrame->m_buf[37] = charTimeB[6];
			pSendFrame->m_buf[38] = charTimeB[7];
			pSendFrame->m_buf[39] = charTimeB[8];
			pSendFrame->m_buf[40] = charTimeB[9];
			pSendFrame->m_buf[41] = ' ';
			pSendFrame->m_buf[42] = 'P';
			pSendFrame->m_buf[43] = charTimeSpan[0];
			pSendFrame->m_buf[44] = charTimeSpan[1];
			pSendFrame->m_buf[45] = ' ';
			pSendFrame->m_buf[46] = '?';
			pSendFrame->m_buf[47] = ' ';
			pSendFrame->m_buf[48] = 'a';
			pSendFrame->m_buf[49] = 'a';
			pSendFrame->m_buf[50] = 'N';
			pSendFrame->m_buf[51] = 'N';
			pSendFrame->m_bufLen = 42;	
		}
		break;
	case TIMETYPE_CURR:
		{
			pSendFrame->m_buf[14] = 'P';
			pSendFrame->m_buf[15] = charTimeSpan[0];
			pSendFrame->m_buf[16] = charTimeSpan[1];
			pSendFrame->m_buf[17] = ' ';
			pSendFrame->m_buf[18] = '?';
			pSendFrame->m_buf[19] = ' ';
			pSendFrame->m_buf[20] = 'a';
			pSendFrame->m_buf[21] = 'a';
			pSendFrame->m_buf[22] = 'N';
			pSendFrame->m_buf[23] = 'N';
			pSendFrame->m_bufLen = 24;	
		}
		break;
	}

	m_pCurrCallOP = scsw_waterRain_call;
	SendRtuLinkFrame(pSendFrame->m_buf, pSendFrame->m_bufLen, pSendFrame->m_CommInterface);

}

void CSysDataTrans_Hydr::SCSW_hydrdata_EN_P_MH(CHydrSendFrame *pSendFrame)
{
	if(NULL == pSendFrame)
		return;

	//-----------站码---------------------
	CString strStCode = pSendFrame->m_strStCode;
	char charStCode[11];
	memset(charStCode, 0, 10);
	memcpy(charStCode, strStCode.GetBuffer(10), 10);

	//--------------ST头----------------
	pSendFrame->m_buf[0] = 'S';
	pSendFrame->m_buf[1] = 'T';
	pSendFrame->m_buf[2] = ' ';
	//---------------站码---------------
	pSendFrame->m_buf[3] = charStCode[0];
	pSendFrame->m_buf[4] = charStCode[1];
	pSendFrame->m_buf[5] = charStCode[2];
	pSendFrame->m_buf[6] = charStCode[3];
	pSendFrame->m_buf[7] = charStCode[4];
	pSendFrame->m_buf[8] = charStCode[5];
	pSendFrame->m_buf[9] = charStCode[6];
	pSendFrame->m_buf[10] = charStCode[7];
	pSendFrame->m_buf[11] = charStCode[8];
	pSendFrame->m_buf[12] = charStCode[9];
	pSendFrame->m_buf[13] = ' ';

	pSendFrame->m_buf[14] = 'P';
	pSendFrame->m_buf[15] = ' ';
	pSendFrame->m_buf[16] = '?';
	pSendFrame->m_buf[17] = ' ';
	pSendFrame->m_buf[18] = 'a';
	pSendFrame->m_buf[19] = 'a';
	pSendFrame->m_buf[20] = 'N';
	pSendFrame->m_buf[21] = 'N';
	pSendFrame->m_bufLen = 22;

	m_pCurrCallOP = scsw_waterRain_call;
	SendRtuLinkFrame(pSendFrame->m_buf, pSendFrame->m_bufLen, pSendFrame->m_CommInterface);
}

void CSysDataTrans_Hydr::SCSW_hydrdata_EN_M(CHydrSendFrame *pSendFrame)
{
// 	if(NULL == pSendFrame)
// 		return;
// 
// 	//-----------土地埋深----------------
// 	CString depth = "";
// 	char charDepth[4];
// 	int nOffset = 0;
// 	memset(charDepth, 0, 4);
// 
// 	int ndepth = pSendFrame->m_LPARME;
// 	if (ndepth != 100)
// 	{
// 		depth.Format("%02d", ndepth);	
// 		memcpy(charDepth, depth.GetBuffer(2), 2);
// 		nOffset = 0;
// 	}
// 	else
// 	{
// 		depth.Format("%3d", ndepth);
// 		memcpy(charDepth, depth.GetBuffer(3), 3);
// 		nOffset = 1;
// 	}
// 
// 	//-----------站码---------------------
// 	CString strStCode = "";
// 	strStCode.Format("%08d", pSendFrame->m_nStCode);
// 	char charStCode[9];
// 	memset(charStCode, 0, 9);
// 	memcpy(charStCode, strStCode.GetBuffer(8), 8);
// 
// 	//--------------ST头----------------
// 	pSendFrame->m_buf[0] = 'S';
// 	pSendFrame->m_buf[1] = 'T';
// 	pSendFrame->m_buf[2] = ' ';
// 	//---------------站码---------------
// 	pSendFrame->m_buf[3] = charStCode[0];
// 	pSendFrame->m_buf[4] = charStCode[1];
// 	pSendFrame->m_buf[5] = charStCode[2];
// 	pSendFrame->m_buf[6] = charStCode[3];
// 	pSendFrame->m_buf[7] = charStCode[4];
// 	pSendFrame->m_buf[8] = charStCode[5];
// 	pSendFrame->m_buf[9] = charStCode[6];
// 	pSendFrame->m_buf[10] = charStCode[7];
// 	pSendFrame->m_buf[11] = ' ';
// 
// 	switch (pSendFrame->m_nTimeType)
// 	{
// 	case TIMETYPE_TT:
// 		{
// 			//----------时间---------------------
// 			CString strTime_a = "";
// 			strTime_a.Format("%02d%02d%02d%02d", 
// 				pSendFrame->t_a.wMonth,
// 				pSendFrame->t_a.wDay,
// 				pSendFrame->t_a.wHour,
// 				pSendFrame->t_a.wMinute);
// 			char charTimeA[9];
// 			memset(charTimeA, 0, 9);
// 			memcpy(charTimeA, strTime_a.GetBuffer(8), 8);
// 
// 			pSendFrame->m_buf[12] = 'T';
// 			pSendFrame->m_buf[13] = 'T';
// 			pSendFrame->m_buf[14] = charTimeA[0];
// 			pSendFrame->m_buf[15] = charTimeA[1];
// 			pSendFrame->m_buf[16] = charTimeA[2];
// 			pSendFrame->m_buf[17] = charTimeA[3];
// 			pSendFrame->m_buf[18] = charTimeA[4];
// 			pSendFrame->m_buf[19] = charTimeA[5];
// 			pSendFrame->m_buf[20] = charTimeA[6];
// 			pSendFrame->m_buf[21] = charTimeA[7];
// 			pSendFrame->m_buf[22] = ' ';
// 			pSendFrame->m_buf[23] = 'M';
// 			if (1== nOffset)
// 			{
// 				pSendFrame->m_buf[24] = charDepth[0];
// 				pSendFrame->m_buf[25] = charDepth[1];
// 				pSendFrame->m_buf[26] = charDepth[2];
// 			}
// 			else
// 			{
// 				pSendFrame->m_buf[24] = charDepth[0];
// 				pSendFrame->m_buf[25] = charDepth[1];
// 			}
// 			
// 			pSendFrame->m_buf[26+nOffset] = ' ';
// 			pSendFrame->m_buf[27+nOffset] = '?';
// 			pSendFrame->m_buf[28+nOffset] = ' ';
// 			pSendFrame->m_buf[29+nOffset] = 'a';
// 			pSendFrame->m_buf[30+nOffset] = 'a';
// 			pSendFrame->m_buf[31+nOffset] = 'N';
// 			pSendFrame->m_buf[32+nOffset] = 'N';
// 			pSendFrame->m_bufLen = 33 + nOffset;	
// 		}
// 		break;
// 	case TIMETYPE_TB:
// 		{
// 			//----------时间---------------------
// 			CString strTime_a = "";
// 			strTime_a.Format("%02d%02d%02d%02d", 
// 				pSendFrame->t_a.wMonth,
// 				pSendFrame->t_a.wDay,
// 				pSendFrame->t_a.wHour,
// 				pSendFrame->t_a.wMinute);
// 			char charTimeA[9];
// 			memset(charTimeA, 0, 9);
// 			memcpy(charTimeA, strTime_a.GetBuffer(8), 8);
// 
// 			CString strTime_b = "";
// 			strTime_a.Format("%02d%02d%02d%02d", 
// 				pSendFrame->t_b.wMonth,
// 				pSendFrame->t_b.wDay,
// 				pSendFrame->t_b.wHour,
// 				pSendFrame->t_b.wMinute);
// 			char charTimeB[9];
// 			memset(charTimeB, 0, 9);
// 			memcpy(charTimeB, strTime_b.GetBuffer(8), 8);
// 
// 			pSendFrame->m_buf[12] = 'T';
// 			pSendFrame->m_buf[13] = 'B';
// 			pSendFrame->m_buf[14] = charTimeA[0];
// 			pSendFrame->m_buf[15] = charTimeA[1];
// 			pSendFrame->m_buf[16] = charTimeA[2];
// 			pSendFrame->m_buf[17] = charTimeA[3];
// 			pSendFrame->m_buf[18] = charTimeA[4];
// 			pSendFrame->m_buf[19] = charTimeA[5];
// 			pSendFrame->m_buf[20] = charTimeA[6];
// 			pSendFrame->m_buf[21] = charTimeA[7];
// 			pSendFrame->m_buf[22] = ' ';
// 			pSendFrame->m_buf[23] = 'T';
// 			pSendFrame->m_buf[24] = 'E';
// 			pSendFrame->m_buf[25] = charTimeB[0];
// 			pSendFrame->m_buf[26] = charTimeB[1];
// 			pSendFrame->m_buf[27] = charTimeB[2];
// 			pSendFrame->m_buf[28] = charTimeB[3];
// 			pSendFrame->m_buf[29] = charTimeB[4];
// 			pSendFrame->m_buf[30] = charTimeB[5];
// 			pSendFrame->m_buf[31] = charTimeB[6];
// 			pSendFrame->m_buf[32] = charTimeB[7];
// 			pSendFrame->m_buf[33] = ' ';
// 			pSendFrame->m_buf[34] = 'M';
// 
// 			if (1 == nOffset)
// 			{
// 				pSendFrame->m_buf[35] = charDepth[0];
// 				pSendFrame->m_buf[36] = charDepth[1];
// 				pSendFrame->m_buf[37] = charDepth[2];
// 			}
// 			else
// 			{
// 				pSendFrame->m_buf[35] = charDepth[0];
// 				pSendFrame->m_buf[36] = charDepth[1];
// 			}
// 			
// 			pSendFrame->m_buf[37+nOffset] = ' ';
// 			pSendFrame->m_buf[38+nOffset] = '?';
// 			pSendFrame->m_buf[39+nOffset] = ' ';
// 			pSendFrame->m_buf[40+nOffset] = 'a';
// 			pSendFrame->m_buf[41+nOffset] = 'a';
// 			pSendFrame->m_buf[42+nOffset] = 'N';
// 			pSendFrame->m_buf[43+nOffset] = 'N';
// 			pSendFrame->m_bufLen = 44 + nOffset;	
// 		}
// 		break;
// 	}
// 
// 	SendRtuLinkFrame(pSendFrame->m_buf, pSendFrame->m_bufLen, pSendFrame->m_CommInterface);

}

void CSysDataTrans_Hydr::SCSW_hydrdata_EN_H(CHydrSendFrame *pSendFrame)
{
// 	if(NULL == pSendFrame)
// 		return;
// 
// 	//-----------站码---------------------
// 	CString strStCode = "";
// 	strStCode.Format("%08d", pSendFrame->m_nStCode);
// 	char charStCode[9];
// 	memset(charStCode, 0, 9);
// 	memcpy(charStCode, strStCode.GetBuffer(8), 8);
// 
// 	//--------------ST头----------------
// 	pSendFrame->m_buf[0] = 'S';
// 	pSendFrame->m_buf[1] = 'T';
// 	pSendFrame->m_buf[2] = ' ';
// 	//---------------站码---------------
// 	pSendFrame->m_buf[3] = charStCode[0];
// 	pSendFrame->m_buf[4] = charStCode[1];
// 	pSendFrame->m_buf[5] = charStCode[2];
// 	pSendFrame->m_buf[6] = charStCode[3];
// 	pSendFrame->m_buf[7] = charStCode[4];
// 	pSendFrame->m_buf[8] = charStCode[5];
// 	pSendFrame->m_buf[9] = charStCode[6];
// 	pSendFrame->m_buf[10] = charStCode[7];
// 	pSendFrame->m_buf[11] = ' ';
// 
// 	switch (pSendFrame->m_nTimeType)
// 	{
// 	case TIMETYPE_TT:
// 		{
// 			//----------时间---------------------
// 			CString strTime_a = "";
// 			strTime_a.Format("%02d%02d%02d%02d", 
// 				pSendFrame->t_a.wMonth,
// 				pSendFrame->t_a.wDay,
// 				pSendFrame->t_a.wHour,
// 				pSendFrame->t_a.wMinute);
// 			char charTimeA[9];
// 			memset(charTimeA, 0, 9);
// 			memcpy(charTimeA, strTime_a.GetBuffer(8), 8);
// 
// 			pSendFrame->m_buf[12] = 'T';
// 			pSendFrame->m_buf[13] = 'T';
// 			pSendFrame->m_buf[14] = charTimeA[0];
// 			pSendFrame->m_buf[15] = charTimeA[1];
// 			pSendFrame->m_buf[16] = charTimeA[2];
// 			pSendFrame->m_buf[17] = charTimeA[3];
// 			pSendFrame->m_buf[18] = charTimeA[4];
// 			pSendFrame->m_buf[19] = charTimeA[5];
// 			pSendFrame->m_buf[20] = charTimeA[6];
// 			pSendFrame->m_buf[21] = charTimeA[7];
// 			pSendFrame->m_buf[22] = ' ';
// 			pSendFrame->m_buf[23] = 'H';
// 			pSendFrame->m_buf[24] = ' ';
// 			pSendFrame->m_buf[25] = '?';
// 			pSendFrame->m_buf[26] = ' ';
// 			pSendFrame->m_buf[27] = 'a';
// 			pSendFrame->m_buf[28] = 'a';
// 			pSendFrame->m_buf[29] = 'N';
// 			pSendFrame->m_buf[30] = 'N';
// 			pSendFrame->m_bufLen = 31;	
// 		}
// 		break;
// 	case TIMETYPE_TB:
// 		{
// 			//----------时间---------------------
// 			CString strTime_a = "";
// 			strTime_a.Format("%02d%02d%02d%02d", 
// 				pSendFrame->t_a.wMonth,
// 				pSendFrame->t_a.wDay,
// 				pSendFrame->t_a.wHour,
// 				pSendFrame->t_a.wMinute);
// 			char charTimeA[9];
// 			memset(charTimeA, 0, 9);
// 			memcpy(charTimeA, strTime_a.GetBuffer(8), 8);
// 
// 			CString strTime_b = "";
// 			strTime_a.Format("%02d%02d%02d%02d", 
// 				pSendFrame->t_b.wMonth,
// 				pSendFrame->t_b.wDay,
// 				pSendFrame->t_b.wHour,
// 				pSendFrame->t_b.wMinute);
// 			char charTimeB[9];
// 			memset(charTimeB, 0, 9);
// 			memcpy(charTimeB, strTime_b.GetBuffer(8), 8);
// 
// 			pSendFrame->m_buf[12] = 'T';
// 			pSendFrame->m_buf[13] = 'B';
// 			pSendFrame->m_buf[14] = charTimeA[0];
// 			pSendFrame->m_buf[15] = charTimeA[1];
// 			pSendFrame->m_buf[16] = charTimeA[2];
// 			pSendFrame->m_buf[17] = charTimeA[3];
// 			pSendFrame->m_buf[18] = charTimeA[4];
// 			pSendFrame->m_buf[19] = charTimeA[5];
// 			pSendFrame->m_buf[20] = charTimeA[6];
// 			pSendFrame->m_buf[21] = charTimeA[7];
// 			pSendFrame->m_buf[22] = ' ';
// 			pSendFrame->m_buf[23] = 'T';
// 			pSendFrame->m_buf[24] = 'E';
// 			pSendFrame->m_buf[25] = charTimeB[0];
// 			pSendFrame->m_buf[26] = charTimeB[1];
// 			pSendFrame->m_buf[27] = charTimeB[2];
// 			pSendFrame->m_buf[28] = charTimeB[3];
// 			pSendFrame->m_buf[29] = charTimeB[4];
// 			pSendFrame->m_buf[30] = charTimeB[5];
// 			pSendFrame->m_buf[31] = charTimeB[6];
// 			pSendFrame->m_buf[32] = charTimeB[7];
// 			pSendFrame->m_buf[33] = ' ';
// 			pSendFrame->m_buf[34] = 'H';
// 			pSendFrame->m_buf[35] = ' ';
// 			pSendFrame->m_buf[36] = '?';
// 			pSendFrame->m_buf[37] = ' ';
// 			pSendFrame->m_buf[38] = 'a';
// 			pSendFrame->m_buf[39] = 'a';
// 			pSendFrame->m_buf[40] = 'N';
// 			pSendFrame->m_buf[41] = 'N';
// 			pSendFrame->m_bufLen = 42;	
// 		}
// 		break;
// 	}
// 
// 	SendRtuLinkFrame(pSendFrame->m_buf, pSendFrame->m_bufLen, pSendFrame->m_CommInterface);
}

void CSysDataTrans_Hydr::SCSW_hydrdata_EN_C(CHydrSendFrame *pSendFrame)
{
// 	if(NULL == pSendFrame)
// 		return;
// 
// 	//-----------站码---------------------
// 	CString strStCode = "";
// 	strStCode.Format("%08d", pSendFrame->m_nStCode);
// 	char charStCode[9];
// 	memset(charStCode, 0, 9);
// 	memcpy(charStCode, strStCode.GetBuffer(8), 8);
// 
// 	//--------------ST头----------------
// 	pSendFrame->m_buf[0] = 'S';
// 	pSendFrame->m_buf[1] = 'T';
// 	pSendFrame->m_buf[2] = ' ';
// 	//---------------站码---------------
// 	pSendFrame->m_buf[3] = charStCode[0];
// 	pSendFrame->m_buf[4] = charStCode[1];
// 	pSendFrame->m_buf[5] = charStCode[2];
// 	pSendFrame->m_buf[6] = charStCode[3];
// 	pSendFrame->m_buf[7] = charStCode[4];
// 	pSendFrame->m_buf[8] = charStCode[5];
// 	pSendFrame->m_buf[9] = charStCode[6];
// 	pSendFrame->m_buf[10] = charStCode[7];
// 	pSendFrame->m_buf[11] = ' ';
// 
// 	switch (pSendFrame->m_nTimeType)
// 	{
// 	case TIMETYPE_TT:
// 		{
// 			//----------时间---------------------
// 			CString strTime_a = "";
// 			strTime_a.Format("%02d%02d%02d%02d", 
// 				pSendFrame->t_a.wMonth,
// 				pSendFrame->t_a.wDay,
// 				pSendFrame->t_a.wHour,
// 				pSendFrame->t_a.wMinute);
// 			char charTimeA[9];
// 			memset(charTimeA, 0, 9);
// 			memcpy(charTimeA, strTime_a.GetBuffer(8), 8);
// 
// 			pSendFrame->m_buf[12] = 'T';
// 			pSendFrame->m_buf[13] = 'T';
// 			pSendFrame->m_buf[14] = charTimeA[0];
// 			pSendFrame->m_buf[15] = charTimeA[1];
// 			pSendFrame->m_buf[16] = charTimeA[2];
// 			pSendFrame->m_buf[17] = charTimeA[3];
// 			pSendFrame->m_buf[18] = charTimeA[4];
// 			pSendFrame->m_buf[19] = charTimeA[5];
// 			pSendFrame->m_buf[20] = charTimeA[6];
// 			pSendFrame->m_buf[21] = charTimeA[7];
// 			pSendFrame->m_buf[22] = ' ';
// 			pSendFrame->m_buf[23] = 'C';
// 			pSendFrame->m_buf[24] = ' ';
// 			pSendFrame->m_buf[25] = '?';
// 			pSendFrame->m_buf[26] = ' ';
// 			pSendFrame->m_buf[27] = 'a';
// 			pSendFrame->m_buf[28] = 'a';
// 			pSendFrame->m_buf[29] = 'N';
// 			pSendFrame->m_buf[30] = 'N';
// 			pSendFrame->m_bufLen = 31;	
// 		}
// 		break;
// 	case TIMETYPE_TB:
// 		{
// 			//----------时间---------------------
// 			CString strTime_a = "";
// 			strTime_a.Format("%02d%02d%02d%02d", 
// 				pSendFrame->t_a.wMonth,
// 				pSendFrame->t_a.wDay,
// 				pSendFrame->t_a.wHour,
// 				pSendFrame->t_a.wMinute);
// 			char charTimeA[9];
// 			memset(charTimeA, 0, 9);
// 			memcpy(charTimeA, strTime_a.GetBuffer(8), 8);
// 
// 			CString strTime_b = "";
// 			strTime_a.Format("%02d%02d%02d%02d", 
// 				pSendFrame->t_b.wMonth,
// 				pSendFrame->t_b.wDay,
// 				pSendFrame->t_b.wHour,
// 				pSendFrame->t_b.wMinute);
// 			char charTimeB[9];
// 			memset(charTimeB, 0, 9);
// 			memcpy(charTimeB, strTime_b.GetBuffer(8), 8);
// 
// 			pSendFrame->m_buf[12] = 'T';
// 			pSendFrame->m_buf[13] = 'B';
// 			pSendFrame->m_buf[14] = charTimeA[0];
// 			pSendFrame->m_buf[15] = charTimeA[1];
// 			pSendFrame->m_buf[16] = charTimeA[2];
// 			pSendFrame->m_buf[17] = charTimeA[3];
// 			pSendFrame->m_buf[18] = charTimeA[4];
// 			pSendFrame->m_buf[19] = charTimeA[5];
// 			pSendFrame->m_buf[20] = charTimeA[6];
// 			pSendFrame->m_buf[21] = charTimeA[7];
// 			pSendFrame->m_buf[22] = ' ';
// 			pSendFrame->m_buf[23] = 'T';
// 			pSendFrame->m_buf[24] = 'E';
// 			pSendFrame->m_buf[25] = charTimeB[0];
// 			pSendFrame->m_buf[26] = charTimeB[1];
// 			pSendFrame->m_buf[27] = charTimeB[2];
// 			pSendFrame->m_buf[28] = charTimeB[3];
// 			pSendFrame->m_buf[29] = charTimeB[4];
// 			pSendFrame->m_buf[30] = charTimeB[5];
// 			pSendFrame->m_buf[31] = charTimeB[6];
// 			pSendFrame->m_buf[32] = charTimeB[7];
// 			pSendFrame->m_buf[33] = ' ';
// 			pSendFrame->m_buf[34] = 'C';
// 			pSendFrame->m_buf[35] = ' ';
// 			pSendFrame->m_buf[36] = '?';
// 			pSendFrame->m_buf[37] = ' ';
// 			pSendFrame->m_buf[38] = 'a';
// 			pSendFrame->m_buf[39] = 'a';
// 			pSendFrame->m_buf[40] = 'N';
// 			pSendFrame->m_buf[41] = 'N';
// 			pSendFrame->m_bufLen = 42;	
// 		}
// 		break;
// 	}
// 
// 	SendRtuLinkFrame(pSendFrame->m_buf, pSendFrame->m_bufLen, pSendFrame->m_CommInterface);

}

void CSysDataTrans_Hydr::SCSW_hydrdata_EN_ED(CHydrSendFrame *pSendFrame)
{
// 	if(NULL == pSendFrame)
// 		return;
// 
// 	//-----------站码---------------------
// 	CString strStCode = "";
// 	strStCode.Format("%08d", pSendFrame->m_nStCode);
// 	char charStCode[9];
// 	memset(charStCode, 0, 9);
// 	memcpy(charStCode, strStCode.GetBuffer(8), 8);
// 
// 	//--------------ST头----------------
// 	pSendFrame->m_buf[0] = 'S';
// 	pSendFrame->m_buf[1] = 'T';
// 	pSendFrame->m_buf[2] = ' ';
// 	//---------------站码---------------
// 	pSendFrame->m_buf[3] = charStCode[0];
// 	pSendFrame->m_buf[4] = charStCode[1];
// 	pSendFrame->m_buf[5] = charStCode[2];
// 	pSendFrame->m_buf[6] = charStCode[3];
// 	pSendFrame->m_buf[7] = charStCode[4];
// 	pSendFrame->m_buf[8] = charStCode[5];
// 	pSendFrame->m_buf[9] = charStCode[6];
// 	pSendFrame->m_buf[10] = charStCode[7];
// 	pSendFrame->m_buf[11] = ' ';
// 
// 	switch (pSendFrame->m_nTimeType)
// 	{
// 	case TIMETYPE_TT:
// 		{
// 			//----------时间---------------------
// 			CString strTime_a = "";
// 			strTime_a.Format("%02d%02d%02d%02d", 
// 				pSendFrame->t_a.wMonth,
// 				pSendFrame->t_a.wDay,
// 				pSendFrame->t_a.wHour,
// 				pSendFrame->t_a.wMinute);
// 			char charTimeA[9];
// 			memset(charTimeA, 0, 9);
// 			memcpy(charTimeA, strTime_a.GetBuffer(8), 8);
// 
// 			pSendFrame->m_buf[12] = 'T';
// 			pSendFrame->m_buf[13] = 'T';
// 			pSendFrame->m_buf[14] = charTimeA[0];
// 			pSendFrame->m_buf[15] = charTimeA[1];
// 			pSendFrame->m_buf[16] = charTimeA[2];
// 			pSendFrame->m_buf[17] = charTimeA[3];
// 			pSendFrame->m_buf[18] = charTimeA[4];
// 			pSendFrame->m_buf[19] = charTimeA[5];
// 			pSendFrame->m_buf[20] = charTimeA[6];
// 			pSendFrame->m_buf[21] = charTimeA[7];
// 			pSendFrame->m_buf[22] = ' ';
// 			pSendFrame->m_buf[23] = 'E';
// 			pSendFrame->m_buf[24] = 'D';
// 			pSendFrame->m_buf[25] = ' ';
// 			pSendFrame->m_buf[26] = '?';
// 			pSendFrame->m_buf[27] = ' ';
// 			pSendFrame->m_buf[28] = 'a';
// 			pSendFrame->m_buf[29] = 'a';
// 			pSendFrame->m_buf[30] = 'N';
// 			pSendFrame->m_buf[31] = 'N';
// 			pSendFrame->m_bufLen = 32;	
// 		}
// 		break;
// 	case TIMETYPE_TB:
// 		{
// 			//----------时间---------------------
// 			CString strTime_a = "";
// 			strTime_a.Format("%02d%02d%02d%02d", 
// 				pSendFrame->t_a.wMonth,
// 				pSendFrame->t_a.wDay,
// 				pSendFrame->t_a.wHour,
// 				pSendFrame->t_a.wMinute);
// 			char charTimeA[9];
// 			memset(charTimeA, 0, 9);
// 			memcpy(charTimeA, strTime_a.GetBuffer(8), 8);
// 
// 			CString strTime_b = "";
// 			strTime_a.Format("%02d%02d%02d%02d", 
// 				pSendFrame->t_b.wMonth,
// 				pSendFrame->t_b.wDay,
// 				pSendFrame->t_b.wHour,
// 				pSendFrame->t_b.wMinute);
// 			char charTimeB[9];
// 			memset(charTimeB, 0, 9);
// 			memcpy(charTimeB, strTime_b.GetBuffer(8), 8);
// 
// 			pSendFrame->m_buf[12] = 'T';
// 			pSendFrame->m_buf[13] = 'B';
// 			pSendFrame->m_buf[14] = charTimeA[0];
// 			pSendFrame->m_buf[15] = charTimeA[1];
// 			pSendFrame->m_buf[16] = charTimeA[2];
// 			pSendFrame->m_buf[17] = charTimeA[3];
// 			pSendFrame->m_buf[18] = charTimeA[4];
// 			pSendFrame->m_buf[19] = charTimeA[5];
// 			pSendFrame->m_buf[20] = charTimeA[6];
// 			pSendFrame->m_buf[21] = charTimeA[7];
// 			pSendFrame->m_buf[22] = ' ';
// 			pSendFrame->m_buf[23] = 'T';
// 			pSendFrame->m_buf[24] = 'E';
// 			pSendFrame->m_buf[25] = charTimeB[0];
// 			pSendFrame->m_buf[26] = charTimeB[1];
// 			pSendFrame->m_buf[27] = charTimeB[2];
// 			pSendFrame->m_buf[28] = charTimeB[3];
// 			pSendFrame->m_buf[29] = charTimeB[4];
// 			pSendFrame->m_buf[30] = charTimeB[5];
// 			pSendFrame->m_buf[31] = charTimeB[6];
// 			pSendFrame->m_buf[32] = charTimeB[7];
// 			pSendFrame->m_buf[33] = ' ';
// 			pSendFrame->m_buf[34] = 'E';
// 			pSendFrame->m_buf[35] = 'D';
// 			pSendFrame->m_buf[36] = ' ';
// 			pSendFrame->m_buf[37] = '?';
// 			pSendFrame->m_buf[38] = ' ';
// 			pSendFrame->m_buf[39] = 'a';
// 			pSendFrame->m_buf[40] = 'a';
// 			pSendFrame->m_buf[41] = 'N';
// 			pSendFrame->m_buf[42] = 'N';
// 			pSendFrame->m_bufLen = 43;	
// 		}
// 		break;
// 	}
// 
// 	SendRtuLinkFrame(pSendFrame->m_buf, pSendFrame->m_bufLen, pSendFrame->m_CommInterface);
}

void CSysDataTrans_Hydr::SCSW_hydrdata_EN_PI(CHydrSendFrame *pSendFrame)
{
	if(NULL == pSendFrame)
		return;

	//-----------站码---------------------
	CString strStCode = pSendFrame->m_strStCode;
	char charStCode[11];
	memset(charStCode, 0, 10);
	memcpy(charStCode, strStCode.GetBuffer(10), 10);

	//--------------ST头----------------
	pSendFrame->m_buf[0] = 'S';
	pSendFrame->m_buf[1] = 'T';
	pSendFrame->m_buf[2] = ' ';
	//---------------站码---------------
	pSendFrame->m_buf[3] = charStCode[0];
	pSendFrame->m_buf[4] = charStCode[1];
	pSendFrame->m_buf[5] = charStCode[2];
	pSendFrame->m_buf[6] = charStCode[3];
	pSendFrame->m_buf[7] = charStCode[4];
	pSendFrame->m_buf[8] = charStCode[5];
	pSendFrame->m_buf[9] = charStCode[6];
	pSendFrame->m_buf[10] = charStCode[7];
	pSendFrame->m_buf[11] = charStCode[8];
	pSendFrame->m_buf[12] = charStCode[9];
	pSendFrame->m_buf[13] = ' ';

	pSendFrame->m_buf[14] = 'P';
	pSendFrame->m_buf[15] = 'I';
	pSendFrame->m_buf[16] = ' ';
	pSendFrame->m_buf[17] = '?';
	pSendFrame->m_buf[18] = ' ';
	pSendFrame->m_buf[19] = 'a';
	pSendFrame->m_buf[20] = 'a';
	pSendFrame->m_buf[21] = 'N';
	pSendFrame->m_buf[22] = 'N';
	pSendFrame->m_bufLen = 23;	

	m_pCurrCallOP = scsw_waterRainStardard;
	SendRtuLinkFrame(pSendFrame->m_buf, pSendFrame->m_bufLen, pSendFrame->m_CommInterface);

}

void CSysDataTrans_Hydr::SCSW_hydrdata_EN_RD(CHydrSendFrame *pSendFrame)
{
	if(NULL == pSendFrame)
		return;

	//-----------站码---------------------
	CString strStCode = pSendFrame->m_strStCode;
	char charStCode[11];
	memset(charStCode, 0, 10);
	memcpy(charStCode, strStCode.GetBuffer(10), 10);

	//--------------ST头----------------
	pSendFrame->m_buf[0] = 'S';
	pSendFrame->m_buf[1] = 'T';
	pSendFrame->m_buf[2] = ' ';
	//---------------站码---------------
	pSendFrame->m_buf[3] = charStCode[0];
	pSendFrame->m_buf[4] = charStCode[1];
	pSendFrame->m_buf[5] = charStCode[2];
	pSendFrame->m_buf[6] = charStCode[3];
	pSendFrame->m_buf[7] = charStCode[4];
	pSendFrame->m_buf[8] = charStCode[5];
	pSendFrame->m_buf[9] = charStCode[6];
	pSendFrame->m_buf[10] = charStCode[7];
	pSendFrame->m_buf[11] = charStCode[8];
	pSendFrame->m_buf[12] = charStCode[9];
	pSendFrame->m_buf[13] = ' ';

	pSendFrame->m_buf[14] = 'R';
	pSendFrame->m_buf[15] = 'D';
	pSendFrame->m_buf[16] = ' ';
	pSendFrame->m_buf[17] = '?';
	pSendFrame->m_buf[18] = ' ';
	pSendFrame->m_buf[19] = 'a';
	pSendFrame->m_buf[20] = 'a';
	pSendFrame->m_buf[21] = 'N';
	pSendFrame->m_buf[22] = 'N';
	pSendFrame->m_bufLen = 23;	

	m_pCurrCallOP = scsw_waterRainSensorDpi;
	SendRtuLinkFrame(pSendFrame->m_buf, pSendFrame->m_bufLen, pSendFrame->m_CommInterface);

}

void CSysDataTrans_Hydr::SCSW_hydrdata_EN_ZJ(CHydrSendFrame *pSendFrame)
{
	if(NULL == pSendFrame)
		return;

	//-----------传感器号----------------
	CString strSensorNo = "";
	int m_nSensorNo = pSendFrame->m_LPARME;
	strSensorNo.Format("%02d", m_nSensorNo);
	char charSensor[3];
	memset(charSensor, 0, 3);
	memcpy(charSensor, strSensorNo.GetBuffer(2), 2);

	//-----------站码---------------------
	CString strStCode = pSendFrame->m_strStCode;
	char charStCode[11];
	memset(charStCode, 0, 10);
	memcpy(charStCode, strStCode.GetBuffer(10), 10);

	//--------------ST头----------------
	pSendFrame->m_buf[0] = 'S';
	pSendFrame->m_buf[1] = 'T';
	pSendFrame->m_buf[2] = ' ';
	//---------------站码---------------
	pSendFrame->m_buf[3] = charStCode[0];
	pSendFrame->m_buf[4] = charStCode[1];
	pSendFrame->m_buf[5] = charStCode[2];
	pSendFrame->m_buf[6] = charStCode[3];
	pSendFrame->m_buf[7] = charStCode[4];
	pSendFrame->m_buf[8] = charStCode[5];
	pSendFrame->m_buf[9] = charStCode[6];
	pSendFrame->m_buf[10] = charStCode[7];
	pSendFrame->m_buf[11] = charStCode[8];
	pSendFrame->m_buf[12] = charStCode[9];
	pSendFrame->m_buf[13] = ' ';

	pSendFrame->m_buf[14] = 'Z';
	pSendFrame->m_buf[15] = 'J';
	pSendFrame->m_buf[16] = charSensor[0];
	pSendFrame->m_buf[17] = charSensor[1];
	pSendFrame->m_buf[18] = ' ';
	pSendFrame->m_buf[19] = '?';
	pSendFrame->m_buf[20] = ' ';
	pSendFrame->m_buf[21] = 'a';
	pSendFrame->m_buf[22] = 'a';
	pSendFrame->m_buf[23] = 'N';
	pSendFrame->m_buf[24] = 'N';
	pSendFrame->m_bufLen = 25;	

	m_pCurrCallOP = scsw_waterRainSensorBase;
	SendRtuLinkFrame(pSendFrame->m_buf, pSendFrame->m_bufLen, pSendFrame->m_CommInterface);
}

void CSysDataTrans_Hydr::SCSW_hydrdata_EN_ZT(CHydrSendFrame *pSendFrame)
{
	if(NULL == pSendFrame)
		return;

	//-----------传感器号----------------
	CString strSensorNo = "";
	int m_nSensorNo = pSendFrame->m_LPARME;
	strSensorNo.Format("%02d", m_nSensorNo);
	char charSensor[3];
	memset(charSensor, 0, 3);
	memcpy(charSensor, strSensorNo.GetBuffer(2), 2);

	//-----------站码---------------------
	CString strStCode = pSendFrame->m_strStCode;
	char charStCode[11];
	memset(charStCode, 0, 10);
	memcpy(charStCode, strStCode.GetBuffer(10), 10);

	//--------------ST头----------------
	pSendFrame->m_buf[0] = 'S';
	pSendFrame->m_buf[1] = 'T';
	pSendFrame->m_buf[2] = ' ';
	//---------------站码---------------
	pSendFrame->m_buf[3] = charStCode[0];
	pSendFrame->m_buf[4] = charStCode[1];
	pSendFrame->m_buf[5] = charStCode[2];
	pSendFrame->m_buf[6] = charStCode[3];
	pSendFrame->m_buf[7] = charStCode[4];
	pSendFrame->m_buf[8] = charStCode[5];
	pSendFrame->m_buf[9] = charStCode[6];
	pSendFrame->m_buf[10] = charStCode[7];
	pSendFrame->m_buf[11] = charStCode[8];
	pSendFrame->m_buf[12] = charStCode[9];
	pSendFrame->m_buf[13] = ' ';

	pSendFrame->m_buf[14] = 'Z';
	pSendFrame->m_buf[15] = 'T';
	pSendFrame->m_buf[16] = charSensor[0];
	pSendFrame->m_buf[17] = charSensor[1];
	pSendFrame->m_buf[18] = ' ';
	pSendFrame->m_buf[19] = '?';
	pSendFrame->m_buf[20] = ' ';
	pSendFrame->m_buf[21] = 'a';
	pSendFrame->m_buf[22] = 'a';
	pSendFrame->m_buf[23] = 'N';
	pSendFrame->m_buf[24] = 'N';
	pSendFrame->m_bufLen = 25;	

	m_pCurrCallOP = scsw_waterRainSensorPara;
	SendRtuLinkFrame(pSendFrame->m_buf, pSendFrame->m_bufLen, pSendFrame->m_CommInterface);
}

void CSysDataTrans_Hydr::SCSW_hydrdata_EN_ZF(CHydrSendFrame *pSendFrame)
{
	if(NULL == pSendFrame)
		return;

	//-----------传感器号----------------
	CString strSensorNo = "";
	int m_nSensorNo = pSendFrame->m_LPARME;
	strSensorNo.Format("%02d", m_nSensorNo);
	char charSensor[3];
	memset(charSensor, 0, 3);
	memcpy(charSensor, strSensorNo.GetBuffer(2), 2);

	//-----------站码---------------------
	CString strStCode = pSendFrame->m_strStCode;
	char charStCode[11];
	memset(charStCode, 0, 10);
	memcpy(charStCode, strStCode.GetBuffer(10), 10);

	//--------------ST头----------------
	pSendFrame->m_buf[0] = 'S';
	pSendFrame->m_buf[1] = 'T';
	pSendFrame->m_buf[2] = ' ';
	//---------------站码---------------
	pSendFrame->m_buf[3] = charStCode[0];
	pSendFrame->m_buf[4] = charStCode[1];
	pSendFrame->m_buf[5] = charStCode[2];
	pSendFrame->m_buf[6] = charStCode[3];
	pSendFrame->m_buf[7] = charStCode[4];
	pSendFrame->m_buf[8] = charStCode[5];
	pSendFrame->m_buf[9] = charStCode[6];
	pSendFrame->m_buf[10] = charStCode[7];
	pSendFrame->m_buf[11] = charStCode[8];
	pSendFrame->m_buf[12] = charStCode[9];
	pSendFrame->m_buf[13] = ' ';

	pSendFrame->m_buf[14] = 'Z';
	pSendFrame->m_buf[15] = 'F';
	pSendFrame->m_buf[16] = charSensor[0];
	pSendFrame->m_buf[17] = charSensor[1];
	pSendFrame->m_buf[18] = ' ';
	pSendFrame->m_buf[19] = '?';
	pSendFrame->m_buf[20] = ' ';
	pSendFrame->m_buf[21] = 'a';
	pSendFrame->m_buf[22] = 'a';
	pSendFrame->m_buf[23] = 'N';
	pSendFrame->m_buf[24] = 'N';
	pSendFrame->m_bufLen = 25;	

	m_pCurrCallOP = scsw_waterDefine;
	SendRtuLinkFrame(pSendFrame->m_buf, pSendFrame->m_bufLen, pSendFrame->m_CommInterface);

}

void CSysDataTrans_Hydr::SCSW_hydrdata_EN_Zx(CHydrSendFrame *pSendFrame)
{
	if(NULL == pSendFrame)
		return;

	//-----------传感器号----------------
	CString strSensorNo = "";
	int m_nSensorNo = pSendFrame->m_LPARME;
	strSensorNo.Format("%02d", m_nSensorNo);
	char charSensor[3];
	memset(charSensor, 0, 3);
	memcpy(charSensor, strSensorNo.GetBuffer(2), 2);

	//-----------标准---------------------
	CString strStardard = "";
	int nStardard = pSendFrame->m_RPARME;
	char chStardard = (char)nStardard;

	//-----------站码---------------------
	CString strStCode = pSendFrame->m_strStCode;
	char charStCode[11];
	memset(charStCode, 0, 10);
	memcpy(charStCode, strStCode.GetBuffer(10), 10);

	//--------------ST头----------------
	pSendFrame->m_buf[0] = 'S';
	pSendFrame->m_buf[1] = 'T';
	pSendFrame->m_buf[2] = ' ';
	//---------------站码---------------
	pSendFrame->m_buf[3] = charStCode[0];
	pSendFrame->m_buf[4] = charStCode[1];
	pSendFrame->m_buf[5] = charStCode[2];
	pSendFrame->m_buf[6] = charStCode[3];
	pSendFrame->m_buf[7] = charStCode[4];
	pSendFrame->m_buf[8] = charStCode[5];
	pSendFrame->m_buf[9] = charStCode[6];
	pSendFrame->m_buf[10] = charStCode[7];
	pSendFrame->m_buf[11] = charStCode[8];
	pSendFrame->m_buf[12] = charStCode[9];
	pSendFrame->m_buf[13] = ' ';

	pSendFrame->m_buf[14] = 'Z';
	pSendFrame->m_buf[15] = chStardard;
	pSendFrame->m_buf[16] = charSensor[0];
	pSendFrame->m_buf[17] = charSensor[1];
	pSendFrame->m_buf[18] = ' ';
	pSendFrame->m_buf[19] = '?';
	pSendFrame->m_buf[20] = ' ';
	pSendFrame->m_buf[21] = 'a';
	pSendFrame->m_buf[22] = 'a';
	pSendFrame->m_buf[23] = 'N';
	pSendFrame->m_buf[24] = 'N';
	pSendFrame->m_bufLen = 25;	

	m_pCurrCallOP = scsw_waterrainTransStardard;
	SendRtuLinkFrame(pSendFrame->m_buf, pSendFrame->m_bufLen, pSendFrame->m_CommInterface);

}

void CSysDataTrans_Hydr::SCSW_hydrdata_EN_DZx(CHydrSendFrame *pSendFrame)
{
	if(NULL == pSendFrame)
		return;

	//-----------传感器号----------------
	CString strSensorNo = "";
	int m_nSensorNo = pSendFrame->m_LPARME;
	strSensorNo.Format("%02d", m_nSensorNo);
	char charSensor[3];
	memset(charSensor, 0, 3);
	memcpy(charSensor, strSensorNo.GetBuffer(2), 2);

	//-----------标准---------------------
	CString strStardard = "";
	int nStardard = pSendFrame->m_RPARME;
	char chStardard = (char)nStardard;

	//-----------站码---------------------
	CString strStCode = pSendFrame->m_strStCode;
	char charStCode[11];
	memset(charStCode, 0, 10);
	memcpy(charStCode, strStCode.GetBuffer(10), 10);

	//--------------ST头----------------
	pSendFrame->m_buf[0] = 'S';
	pSendFrame->m_buf[1] = 'T';
	pSendFrame->m_buf[2] = ' ';
	//---------------站码---------------
	pSendFrame->m_buf[3] = charStCode[0];
	pSendFrame->m_buf[4] = charStCode[1];
	pSendFrame->m_buf[5] = charStCode[2];
	pSendFrame->m_buf[6] = charStCode[3];
	pSendFrame->m_buf[7] = charStCode[4];
	pSendFrame->m_buf[8] = charStCode[5];
	pSendFrame->m_buf[9] = charStCode[6];
	pSendFrame->m_buf[10] = charStCode[7];
	pSendFrame->m_buf[11] = charStCode[8];
	pSendFrame->m_buf[12] = charStCode[9];
	pSendFrame->m_buf[13] = ' ';

	pSendFrame->m_buf[14] = 'D';
	pSendFrame->m_buf[15] = 'Z';
	pSendFrame->m_buf[16] = chStardard;
	pSendFrame->m_buf[17] = charSensor[0];
	pSendFrame->m_buf[18] = charSensor[1];
	pSendFrame->m_buf[19] = ' ';
	pSendFrame->m_buf[20] = '?';
	pSendFrame->m_buf[21] = ' ';
	pSendFrame->m_buf[22] = 'a';
	pSendFrame->m_buf[23] = 'a';
	pSendFrame->m_buf[24] = 'N';
	pSendFrame->m_buf[25] = 'N';
	pSendFrame->m_bufLen = 26;	

	m_pCurrCallOP = scsw_waterrainTimespan;
	SendRtuLinkFrame(pSendFrame->m_buf, pSendFrame->m_bufLen, pSendFrame->m_CommInterface);
}

void CSysDataTrans_Hydr::SCSW_hydrdata_EN_GSM(CHydrSendFrame *pSendFrame)
{
	if(NULL == pSendFrame)
		return;

	//-----------传感器号----------------
	CString strSensorNo = "";
	int m_nSensorNo = pSendFrame->m_LPARME;
	strSensorNo.Format("%02d", m_nSensorNo);
	char charSensor[3];
	memset(charSensor, 0, 3);
	memcpy(charSensor, strSensorNo.GetBuffer(2), 2);

	//-----------站码---------------------
	CString strStCode = pSendFrame->m_strStCode;
	char charStCode[11];
	memset(charStCode, 0, 10);
	memcpy(charStCode, strStCode.GetBuffer(10), 10);

	//--------------ST头----------------
	pSendFrame->m_buf[0] = 'S';
	pSendFrame->m_buf[1] = 'T';
	pSendFrame->m_buf[2] = ' ';
	//---------------站码---------------
	pSendFrame->m_buf[3] = charStCode[0];
	pSendFrame->m_buf[4] = charStCode[1];
	pSendFrame->m_buf[5] = charStCode[2];
	pSendFrame->m_buf[6] = charStCode[3];
	pSendFrame->m_buf[7] = charStCode[4];
	pSendFrame->m_buf[8] = charStCode[5];
	pSendFrame->m_buf[9] = charStCode[6];
	pSendFrame->m_buf[10] = charStCode[7];
	pSendFrame->m_buf[11] = charStCode[8];
	pSendFrame->m_buf[12] = charStCode[9];
	pSendFrame->m_buf[13] = ' ';

	pSendFrame->m_buf[14] = 'G';
	pSendFrame->m_buf[15] = 'S';
	pSendFrame->m_buf[16] = 'M';
	pSendFrame->m_buf[17] = charSensor[0];
	pSendFrame->m_buf[18] = charSensor[1];
	pSendFrame->m_buf[19] = ' ';
	pSendFrame->m_buf[20] = '?';
	pSendFrame->m_buf[21] = ' ';
	pSendFrame->m_buf[22] = 'a';
	pSendFrame->m_buf[23] = 'a';
	pSendFrame->m_buf[24] = 'N';
	pSendFrame->m_buf[25] = 'N';
	pSendFrame->m_bufLen = 26;	

	m_pCurrCallOP = scsw_interface_GSM;
	SendRtuLinkFrame(pSendFrame->m_buf, pSendFrame->m_bufLen, pSendFrame->m_CommInterface);

}

void CSysDataTrans_Hydr::SCSW_hydrdata_EN_BD(CHydrSendFrame *pSendFrame)
{
	if(NULL == pSendFrame)
		return;

	//-----------传感器号----------------
	CString strSensorNo = "";
	int m_nSensorNo = pSendFrame->m_LPARME;
	strSensorNo.Format("%02d", m_nSensorNo);
	char charSensor[3];
	memset(charSensor, 0, 3);
	memcpy(charSensor, strSensorNo.GetBuffer(2), 2);

	//-----------站码---------------------
	CString strStCode = pSendFrame->m_strStCode;
	char charStCode[11];
	memset(charStCode, 0, 10);
	memcpy(charStCode, strStCode.GetBuffer(10), 10);

	//--------------ST头----------------
	pSendFrame->m_buf[0] = 'S';
	pSendFrame->m_buf[1] = 'T';
	pSendFrame->m_buf[2] = ' ';
	//---------------站码---------------
	pSendFrame->m_buf[3] = charStCode[0];
	pSendFrame->m_buf[4] = charStCode[1];
	pSendFrame->m_buf[5] = charStCode[2];
	pSendFrame->m_buf[6] = charStCode[3];
	pSendFrame->m_buf[7] = charStCode[4];
	pSendFrame->m_buf[8] = charStCode[5];
	pSendFrame->m_buf[9] = charStCode[6];
	pSendFrame->m_buf[10] = charStCode[7];
	pSendFrame->m_buf[11] = charStCode[8];
	pSendFrame->m_buf[12] = charStCode[9];
	pSendFrame->m_buf[13] = ' ';

	pSendFrame->m_buf[14] = 'B';
	pSendFrame->m_buf[15] = 'D';
	pSendFrame->m_buf[16] = charSensor[0];
	pSendFrame->m_buf[17] = charSensor[1];
	pSendFrame->m_buf[18] = ' ';
	pSendFrame->m_buf[19] = '?';
	pSendFrame->m_buf[20] = ' ';
	pSendFrame->m_buf[21] = 'a';
	pSendFrame->m_buf[22] = 'a';
	pSendFrame->m_buf[23] = 'N';
	pSendFrame->m_buf[24] = 'N';
	pSendFrame->m_bufLen = 25;	

	m_pCurrCallOP = scsw_interface_BD;
	SendRtuLinkFrame(pSendFrame->m_buf, pSendFrame->m_bufLen, pSendFrame->m_CommInterface);

}

void CSysDataTrans_Hydr::SCSW_hydrdata_EN_IP(CHydrSendFrame *pSendFrame)
{
	if(NULL == pSendFrame)
		return;

	//-----------传感器号----------------
	CString strSensorNo = "";
	int m_nSensorNo = pSendFrame->m_LPARME;
	strSensorNo.Format("%d", m_nSensorNo);
	char charSensor[3];
	memset(charSensor, 0, 3);
	memcpy(charSensor, strSensorNo.GetBuffer(1), 1);

	//-----------标准---------------------
	CString strStardard = "";
	int nStardard = pSendFrame->m_RPARME;
	char chStardard = (char)nStardard;

	//-----------站码---------------------
	CString strStCode = pSendFrame->m_strStCode;
	char charStCode[11];
	memset(charStCode, 0, 10);
	memcpy(charStCode, strStCode.GetBuffer(10), 10);

	//--------------ST头----------------
	pSendFrame->m_buf[0] = 'S';
	pSendFrame->m_buf[1] = 'T';
	pSendFrame->m_buf[2] = ' ';
	//---------------站码---------------
	pSendFrame->m_buf[3] = charStCode[0];
	pSendFrame->m_buf[4] = charStCode[1];
	pSendFrame->m_buf[5] = charStCode[2];
	pSendFrame->m_buf[6] = charStCode[3];
	pSendFrame->m_buf[7] = charStCode[4];
	pSendFrame->m_buf[8] = charStCode[5];
	pSendFrame->m_buf[9] = charStCode[6];
	pSendFrame->m_buf[10] = charStCode[7];
	pSendFrame->m_buf[11] = charStCode[8];
	pSendFrame->m_buf[12] = charStCode[9];
	pSendFrame->m_buf[13] = ' ';

	pSendFrame->m_buf[14] = 'H';
	pSendFrame->m_buf[15] = 'O';
	pSendFrame->m_buf[16] = 'S';
	pSendFrame->m_buf[17] = 'T';
	pSendFrame->m_buf[18] = charSensor[0];
	pSendFrame->m_buf[19] = ' ';
	pSendFrame->m_buf[20] = '?';
	pSendFrame->m_buf[21] = ' ';
	pSendFrame->m_buf[22] = 'a';
	pSendFrame->m_buf[23] = 'a';
	pSendFrame->m_buf[24] = 'N';
	pSendFrame->m_buf[25] = 'N';
	pSendFrame->m_bufLen = 26;	

	m_pCurrCallOP = scsw_interface_IP;
	SendRtuLinkFrame(pSendFrame->m_buf, pSendFrame->m_bufLen, pSendFrame->m_CommInterface);

}

void CSysDataTrans_Hydr::SCSW_hydrdata_EN_IP2(CHydrSendFrame *pSendFrame)
{
	if(NULL == pSendFrame)
		return;

	//-----------传感器号----------------
	CString strSensorNo = "";
	int m_nSensorNo = pSendFrame->m_LPARME;
	strSensorNo.Format("%02d", m_nSensorNo);
	char charSensor[3];
	memset(charSensor, 0, 3);
	memcpy(charSensor, strSensorNo.GetBuffer(2), 2);

	//-----------标准---------------------
	CString strStardard = "";
	int nStardard = pSendFrame->m_RPARME;
	char chStardard = (char)nStardard;

	//-----------站码---------------------
	CString strStCode = pSendFrame->m_strStCode;
	char charStCode[11];
	memset(charStCode, 0, 10);
	memcpy(charStCode, strStCode.GetBuffer(10), 10);

	//--------------ST头----------------
	pSendFrame->m_buf[0] = 'S';
	pSendFrame->m_buf[1] = 'T';
	pSendFrame->m_buf[2] = ' ';
	//---------------站码---------------
	pSendFrame->m_buf[3] = charStCode[0];
	pSendFrame->m_buf[4] = charStCode[1];
	pSendFrame->m_buf[5] = charStCode[2];
	pSendFrame->m_buf[6] = charStCode[3];
	pSendFrame->m_buf[7] = charStCode[4];
	pSendFrame->m_buf[8] = charStCode[5];
	pSendFrame->m_buf[9] = charStCode[6];
	pSendFrame->m_buf[10] = charStCode[7];
	pSendFrame->m_buf[11] = charStCode[8];
	pSendFrame->m_buf[12] = charStCode[9];
	pSendFrame->m_buf[13] = ' ';

	pSendFrame->m_buf[14] = 'I';
	pSendFrame->m_buf[15] = 'T';
	pSendFrame->m_buf[16] = charSensor[0];
	pSendFrame->m_buf[17] = charSensor[1];
	pSendFrame->m_buf[18] = ' ';
	pSendFrame->m_buf[19] = '?';
	pSendFrame->m_buf[20] = ' ';
	pSendFrame->m_buf[21] = 'a';
	pSendFrame->m_buf[22] = 'a';
	pSendFrame->m_buf[23] = 'N';
	pSendFrame->m_buf[24] = 'N';
	pSendFrame->m_bufLen = 25;	

	m_pCurrCallOP = scsw_interface_IP2;
	SendRtuLinkFrame(pSendFrame->m_buf, pSendFrame->m_bufLen, pSendFrame->m_CommInterface);

}

void CSysDataTrans_Hydr::SCSW_hydrdata_EN_PORT(CHydrSendFrame *pSendFrame)
{
	if(NULL == pSendFrame)
		return;

	//-----------传感器号----------------
	CString strSensorNo = "";
	int m_nSensorNo = pSendFrame->m_LPARME;
	strSensorNo.Format("%02d", m_nSensorNo);
	char charSensor[3];
	memset(charSensor, 0, 3);
	memcpy(charSensor, strSensorNo.GetBuffer(2), 2);

	//-----------标准---------------------
	CString strStardard = "";
	int nStardard = pSendFrame->m_RPARME;
	char chStardard = (char)nStardard;

	//-----------站码---------------------
	CString strStCode = pSendFrame->m_strStCode;
	char charStCode[11];
	memset(charStCode, 0, 10);
	memcpy(charStCode, strStCode.GetBuffer(10), 10);

	//--------------ST头----------------
	pSendFrame->m_buf[0] = 'S';
	pSendFrame->m_buf[1] = 'T';
	pSendFrame->m_buf[2] = ' ';
	//---------------站码---------------
	pSendFrame->m_buf[3] = charStCode[0];
	pSendFrame->m_buf[4] = charStCode[1];
	pSendFrame->m_buf[5] = charStCode[2];
	pSendFrame->m_buf[6] = charStCode[3];
	pSendFrame->m_buf[7] = charStCode[4];
	pSendFrame->m_buf[8] = charStCode[5];
	pSendFrame->m_buf[9] = charStCode[6];
	pSendFrame->m_buf[10] = charStCode[7];
	pSendFrame->m_buf[11] = charStCode[8];
	pSendFrame->m_buf[12] = charStCode[9];
	pSendFrame->m_buf[13] = ' ';

	pSendFrame->m_buf[14] = 'P';
	pSendFrame->m_buf[15] = 'T';
	pSendFrame->m_buf[16] = charSensor[0];
	pSendFrame->m_buf[17] = charSensor[1];
	pSendFrame->m_buf[18] = ' ';
	pSendFrame->m_buf[19] = '?';
	pSendFrame->m_buf[20] = ' ';
	pSendFrame->m_buf[21] = 'a';
	pSendFrame->m_buf[22] = 'a';
	pSendFrame->m_buf[23] = 'N';
	pSendFrame->m_buf[24] = 'N';
	pSendFrame->m_bufLen = 25;	

	m_pCurrCallOP = scsw_interface_PORT;
	SendRtuLinkFrame(pSendFrame->m_buf, pSendFrame->m_bufLen, pSendFrame->m_CommInterface);

}


void CSysDataTrans_Hydr::SCSW_hydrdata_EN_SET_GSM(CHydrSendFrame *pSendFrame)
{
	if(NULL == pSendFrame)
		return;

	CString strPsw = pSendFrame->m_strPsw;
	char charPsw[7];
	memset(charPsw, 0, 6);
	memcpy(charPsw, strPsw.GetBuffer(6), 6);

	CString SetVar = pSendFrame->m_strSetVar;
	int Varlen = SetVar.GetLength();
	char charVarBuf[20];
	memset(charVarBuf, 0, 19);
	memcpy(charVarBuf, SetVar.GetBuffer(19), 19);

	//-----------传感器号----------------
	CString strSensorNo = "";
	int m_nSensorNo = pSendFrame->m_LPARME;
	char charSensor[3];
	if (m_nSensorNo<10)
	{
		strSensorNo.Format("%d", m_nSensorNo);
		
		memset(charSensor, 0, 3);
		memcpy(charSensor, strSensorNo.GetBuffer(1), 1);
	}
	else
	{
		strSensorNo.Format("%02d", m_nSensorNo);
		
		memset(charSensor, 0, 3);
		memcpy(charSensor, strSensorNo.GetBuffer(2), 2);
	}
	

	//-----------站码---------------------
	CString strStCode = pSendFrame->m_strStCode;
	char charStCode[11];
	memset(charStCode, 0, 10);
	memcpy(charStCode, strStCode.GetBuffer(10), 10);

	//--------------ST头----------------
	pSendFrame->m_buf[0] = 'S';
	pSendFrame->m_buf[1] = 'T';
	pSendFrame->m_buf[2] = ' ';
	//---------------站码---------------
	pSendFrame->m_buf[3] = charStCode[0];
	pSendFrame->m_buf[4] = charStCode[1];
	pSendFrame->m_buf[5] = charStCode[2];
	pSendFrame->m_buf[6] = charStCode[3];
	pSendFrame->m_buf[7] = charStCode[4];
	pSendFrame->m_buf[8] = charStCode[5];
	pSendFrame->m_buf[9] = charStCode[6];
	pSendFrame->m_buf[10] = charStCode[7];
	pSendFrame->m_buf[11] = charStCode[8];
	pSendFrame->m_buf[12] = charStCode[9];
	pSendFrame->m_buf[13] = ' ';

	pSendFrame->m_buf[14] = 'P';
	pSendFrame->m_buf[15] = 'W';
	pSendFrame->m_buf[16] = ' ';
	pSendFrame->m_buf[17] = charPsw[0];
	pSendFrame->m_buf[18] = charPsw[1];
	pSendFrame->m_buf[19] = charPsw[2];
	pSendFrame->m_buf[20] = charPsw[3];
	pSendFrame->m_buf[21] = charPsw[4];
	pSendFrame->m_buf[22] = charPsw[5];
	pSendFrame->m_buf[23] = ' ';

	pSendFrame->m_buf[24] = 'G';
	pSendFrame->m_buf[25] = 'S';
	pSendFrame->m_buf[26] = 'M';
	int nOffset = 0;
	if (m_nSensorNo < 10)
	{
		pSendFrame->m_buf[27] = charSensor[0];
		nOffset = 1;
	}
	else
	{
		pSendFrame->m_buf[27] = charSensor[0];
		pSendFrame->m_buf[28] = charSensor[1];
		nOffset = 2;

	}

	pSendFrame->m_buf[27+nOffset] = ' ';
	

	for (int i=0; i<Varlen; i++)
	{
		pSendFrame->m_buf[28+i+nOffset] = charVarBuf[i];
	}

	pSendFrame->m_buf[28+Varlen+nOffset] = ' ';
	WORD check = CUdfGlobalFUNC::CreateCRC_SCSW(pSendFrame->m_buf, 29+Varlen);
	pSendFrame->m_buf[29+Varlen+nOffset] = HIBYTE(check);
	pSendFrame->m_buf[30+Varlen+nOffset] = LOBYTE(check);
	pSendFrame->m_buf[31+Varlen+nOffset] = 'N';
	pSendFrame->m_buf[32+Varlen+nOffset] = 'N';
	pSendFrame->m_bufLen = 33+Varlen+nOffset;	

	m_pCurrCallOP = scsw_interface_GSM_Set;
	SendRtuLinkFrame(pSendFrame->m_buf, pSendFrame->m_bufLen, pSendFrame->m_CommInterface);

}

void CSysDataTrans_Hydr::SCSW_hydrdata_EN_SET_BD(CHydrSendFrame *pSendFrame)
{
	if(NULL == pSendFrame)
		return;

	CString strPsw = pSendFrame->m_strPsw;
	char charPsw[7];
	memset(charPsw, 0, 6);
	memcpy(charPsw, strPsw.GetBuffer(6), 6);

	CString SetVar = pSendFrame->m_strSetVar;
	int Varlen = SetVar.GetLength();
	char charVarBuf[20];
	memset(charVarBuf, 0, 19);
	memcpy(charVarBuf, SetVar.GetBuffer(19), 19);

	//-----------传感器号----------------
	CString strSensorNo = "";
	int m_nSensorNo = pSendFrame->m_LPARME;
	strSensorNo.Format("%02d", m_nSensorNo);
	char charSensor[3];
	memset(charSensor, 0, 3);
	memcpy(charSensor, strSensorNo.GetBuffer(2), 2);

	//-----------站码---------------------
	CString strStCode = pSendFrame->m_strStCode;
	char charStCode[11];
	memset(charStCode, 0, 10);
	memcpy(charStCode, strStCode.GetBuffer(10), 10);

	//--------------ST头----------------
	pSendFrame->m_buf[0] = 'S';
	pSendFrame->m_buf[1] = 'T';
	pSendFrame->m_buf[2] = ' ';
	//---------------站码---------------
	pSendFrame->m_buf[3] = charStCode[0];
	pSendFrame->m_buf[4] = charStCode[1];
	pSendFrame->m_buf[5] = charStCode[2];
	pSendFrame->m_buf[6] = charStCode[3];
	pSendFrame->m_buf[7] = charStCode[4];
	pSendFrame->m_buf[8] = charStCode[5];
	pSendFrame->m_buf[9] = charStCode[6];
	pSendFrame->m_buf[10] = charStCode[7];
	pSendFrame->m_buf[11] = charStCode[8];
	pSendFrame->m_buf[12] = charStCode[9];
	pSendFrame->m_buf[13] = ' ';

	pSendFrame->m_buf[14] = 'P';
	pSendFrame->m_buf[15] = 'W';
	pSendFrame->m_buf[16] = ' ';
	pSendFrame->m_buf[17] = charPsw[0];
	pSendFrame->m_buf[18] = charPsw[1];
	pSendFrame->m_buf[19] = charPsw[2];
	pSendFrame->m_buf[20] = charPsw[3];
	pSendFrame->m_buf[21] = charPsw[4];
	pSendFrame->m_buf[22] = charPsw[5];
	pSendFrame->m_buf[23] = ' ';

	pSendFrame->m_buf[24] = 'B';
	pSendFrame->m_buf[25] = 'D';
	pSendFrame->m_buf[26] = charSensor[0];
	pSendFrame->m_buf[27] = charSensor[1];
	pSendFrame->m_buf[28] = ' ';

	for (int i=0; i<Varlen; i++)
	{
		pSendFrame->m_buf[29+i] = charVarBuf[i];
	}

	pSendFrame->m_buf[29+Varlen] = ' ';
	WORD check = CUdfGlobalFUNC::CreateCRC_SCSW(pSendFrame->m_buf, 30+Varlen);
	pSendFrame->m_buf[30+Varlen] = HIBYTE(check);
	pSendFrame->m_buf[31+Varlen] = LOBYTE(check);
	pSendFrame->m_buf[32+Varlen] = 'N';
	pSendFrame->m_buf[33+Varlen] = 'N';
	pSendFrame->m_bufLen = 34+Varlen;	

	m_pCurrCallOP = scsw_interface_BD_Set;
	SendRtuLinkFrame(pSendFrame->m_buf, pSendFrame->m_bufLen, pSendFrame->m_CommInterface);
}

void CSysDataTrans_Hydr::SCSW_hydrdata_EN_SET_IP(CHydrSendFrame *pSendFrame)
{
	if(NULL == pSendFrame)
		return;

	CString strPsw = pSendFrame->m_strPsw;
	char charPsw[7];
	memset(charPsw, 0, 6);
	memcpy(charPsw, strPsw.GetBuffer(6), 6);

	CString SetVar = pSendFrame->m_strSetVar;
	int Varlen = SetVar.GetLength();
	char charVarBuf[20];
	memset(charVarBuf, 0, 19);
	memcpy(charVarBuf, SetVar.GetBuffer(19), 19);
	//-----------传感器号----------------
	CString strSensorNo = "";
	int m_nSensorNo = pSendFrame->m_LPARME;
	strSensorNo.Format("%d", m_nSensorNo);
	char charSensor[3];
	memset(charSensor, 0, 3);
	memcpy(charSensor, strSensorNo.GetBuffer(1), 1);

	//-----------站码---------------------
	CString strStCode = pSendFrame->m_strStCode;
	char charStCode[11];
	memset(charStCode, 0, 10);
	memcpy(charStCode, strStCode.GetBuffer(10), 10);

	//--------------ST头----------------
	pSendFrame->m_buf[0] = 'S';
	pSendFrame->m_buf[1] = 'T';
	pSendFrame->m_buf[2] = ' ';
	//---------------站码---------------
	pSendFrame->m_buf[3] = charStCode[0];
	pSendFrame->m_buf[4] = charStCode[1];
	pSendFrame->m_buf[5] = charStCode[2];
	pSendFrame->m_buf[6] = charStCode[3];
	pSendFrame->m_buf[7] = charStCode[4];
	pSendFrame->m_buf[8] = charStCode[5];
	pSendFrame->m_buf[9] = charStCode[6];
	pSendFrame->m_buf[10] = charStCode[7];
	pSendFrame->m_buf[11] = charStCode[8];
	pSendFrame->m_buf[12] = charStCode[9];
	pSendFrame->m_buf[13] = ' ';

	pSendFrame->m_buf[14] = 'P';
	pSendFrame->m_buf[15] = 'W';
	pSendFrame->m_buf[16] = ' ';
	pSendFrame->m_buf[17] = charPsw[0];
	pSendFrame->m_buf[18] = charPsw[1];
	pSendFrame->m_buf[19] = charPsw[2];
	pSendFrame->m_buf[20] = charPsw[3];
	pSendFrame->m_buf[21] = charPsw[4];
	pSendFrame->m_buf[22] = charPsw[5];
	pSendFrame->m_buf[23] = ' ';

	pSendFrame->m_buf[24] = 'H';
	pSendFrame->m_buf[25] = 'O';
	pSendFrame->m_buf[26] = 'S';
	pSendFrame->m_buf[27] = 'T';
	pSendFrame->m_buf[28] = charVarBuf[0];
	pSendFrame->m_buf[29] = ' ';


	for (int i=0; i<Varlen; i++)
	{
		pSendFrame->m_buf[30+i] = charVarBuf[i];
	}

	pSendFrame->m_buf[30+Varlen] = ' ';
	WORD check = CUdfGlobalFUNC::CreateCRC_SCSW(pSendFrame->m_buf, 31+Varlen);
	pSendFrame->m_buf[31+Varlen] = HIBYTE(check);
	pSendFrame->m_buf[32+Varlen] = LOBYTE(check);
	pSendFrame->m_buf[33+Varlen] = 'N';
	pSendFrame->m_buf[34+Varlen] = 'N';
	pSendFrame->m_bufLen = 35+Varlen;	

	m_pCurrCallOP = scsw_interface_IP_Set;
	SendRtuLinkFrame(pSendFrame->m_buf, pSendFrame->m_bufLen, pSendFrame->m_CommInterface);
}

void CSysDataTrans_Hydr::SCSW_hydrdata_EN_SET_IP2(CHydrSendFrame *pSendFrame)
{
	if(NULL == pSendFrame)
		return;

	CString strPsw = pSendFrame->m_strPsw;
	char charPsw[7];
	memset(charPsw, 0, 6);
	memcpy(charPsw, strPsw.GetBuffer(6), 6);

	CString SetVar = pSendFrame->m_strSetVar;
	SetVar.Trim();
	SetVar.Replace(".", " ");
	int Varlen = SetVar.GetLength();
	char charVarBuf[20];
	memset(charVarBuf, 0, 19);
	memcpy(charVarBuf, SetVar.GetBuffer(19), 19);
	//-----------传感器号----------------
	CString strSensorNo = "";
	int m_nSensorNo = pSendFrame->m_LPARME;
	strSensorNo.Format("%02d", m_nSensorNo);
	char charSensor[3];
	memset(charSensor, 0, 3);
	memcpy(charSensor, strSensorNo.GetBuffer(2), 2);

	//-----------站码---------------------
	CString strStCode = pSendFrame->m_strStCode;
	char charStCode[11];
	memset(charStCode, 0, 10);
	memcpy(charStCode, strStCode.GetBuffer(10), 10);

	//--------------ST头----------------
	pSendFrame->m_buf[0] = 'S';
	pSendFrame->m_buf[1] = 'T';
	pSendFrame->m_buf[2] = ' ';
	//---------------站码---------------
	pSendFrame->m_buf[3] = charStCode[0];
	pSendFrame->m_buf[4] = charStCode[1];
	pSendFrame->m_buf[5] = charStCode[2];
	pSendFrame->m_buf[6] = charStCode[3];
	pSendFrame->m_buf[7] = charStCode[4];
	pSendFrame->m_buf[8] = charStCode[5];
	pSendFrame->m_buf[9] = charStCode[6];
	pSendFrame->m_buf[10] = charStCode[7];
	pSendFrame->m_buf[11] = charStCode[8];
	pSendFrame->m_buf[12] = charStCode[9];
	pSendFrame->m_buf[13] = ' ';

	pSendFrame->m_buf[14] = 'P';
	pSendFrame->m_buf[15] = 'W';
	pSendFrame->m_buf[16] = ' ';
	pSendFrame->m_buf[17] = charPsw[0];
	pSendFrame->m_buf[18] = charPsw[1];
	pSendFrame->m_buf[19] = charPsw[2];
	pSendFrame->m_buf[20] = charPsw[3];
	pSendFrame->m_buf[21] = charPsw[4];
	pSendFrame->m_buf[22] = charPsw[5];
	pSendFrame->m_buf[23] = ' ';

	pSendFrame->m_buf[24] = 'I';
	pSendFrame->m_buf[25] = 'T';
	pSendFrame->m_buf[26] = strSensorNo[0];
	pSendFrame->m_buf[27] = strSensorNo[1];
	pSendFrame->m_buf[28] = ' ';


	for (int i=0; i<Varlen; i++)
	{
		pSendFrame->m_buf[29+i] = charVarBuf[i];
	}

	pSendFrame->m_buf[29+Varlen] = ' ';
	WORD check = CUdfGlobalFUNC::CreateCRC_SCSW(pSendFrame->m_buf, 32+Varlen);
	pSendFrame->m_buf[30+Varlen] = HIBYTE(check);
	pSendFrame->m_buf[31+Varlen] = LOBYTE(check);
	pSendFrame->m_buf[32+Varlen] = 'N';
	pSendFrame->m_buf[33+Varlen] = 'N';
	pSendFrame->m_bufLen = 34+Varlen;	

	m_pCurrCallOP = scsw_interface_IP_Set2;
	SendRtuLinkFrame(pSendFrame->m_buf, pSendFrame->m_bufLen, pSendFrame->m_CommInterface);
}

void CSysDataTrans_Hydr::SCSW_hydrdata_EN_SET_PORT(CHydrSendFrame *pSendFrame)
{
	if(NULL == pSendFrame)
		return;

	CString strPsw = pSendFrame->m_strPsw;
	char charPsw[7];
	memset(charPsw, 0, 6);
	memcpy(charPsw, strPsw.GetBuffer(6), 6);

	CString SetVar = pSendFrame->m_strSetVar;
	int Varlen = SetVar.GetLength();
	char charVarBuf[20];
	memset(charVarBuf, 0, 19);
	memcpy(charVarBuf, SetVar.GetBuffer(19), 19);

	//-----------传感器号----------------
	CString strSensorNo = "";
	int m_nSensorNo = pSendFrame->m_LPARME;
	strSensorNo.Format("%02d", m_nSensorNo);
	char charSensor[3];
	memset(charSensor, 0, 3);
	memcpy(charSensor, strSensorNo.GetBuffer(2), 2);

	//-----------站码---------------------
	CString strStCode = pSendFrame->m_strStCode;
	char charStCode[11];
	memset(charStCode, 0, 10);
	memcpy(charStCode, strStCode.GetBuffer(10), 10);

	//--------------ST头----------------
	pSendFrame->m_buf[0] = 'S';
	pSendFrame->m_buf[1] = 'T';
	pSendFrame->m_buf[2] = ' ';
	//---------------站码---------------
	pSendFrame->m_buf[3] = charStCode[0];
	pSendFrame->m_buf[4] = charStCode[1];
	pSendFrame->m_buf[5] = charStCode[2];
	pSendFrame->m_buf[6] = charStCode[3];
	pSendFrame->m_buf[7] = charStCode[4];
	pSendFrame->m_buf[8] = charStCode[5];
	pSendFrame->m_buf[9] = charStCode[6];
	pSendFrame->m_buf[10] = charStCode[7];
	pSendFrame->m_buf[11] = charStCode[8];
	pSendFrame->m_buf[12] = charStCode[9];
	pSendFrame->m_buf[13] = ' ';

	pSendFrame->m_buf[14] = 'P';
	pSendFrame->m_buf[15] = 'W';
	pSendFrame->m_buf[16] = ' ';
	pSendFrame->m_buf[17] = charPsw[0];
	pSendFrame->m_buf[18] = charPsw[1];
	pSendFrame->m_buf[19] = charPsw[2];
	pSendFrame->m_buf[20] = charPsw[3];
	pSendFrame->m_buf[21] = charPsw[4];
	pSendFrame->m_buf[22] = charPsw[5];
	pSendFrame->m_buf[23] = ' ';

	pSendFrame->m_buf[24] = 'P';
	pSendFrame->m_buf[25] = 'T';
	pSendFrame->m_buf[26] = charVarBuf[0];
	pSendFrame->m_buf[27] = charVarBuf[1];
	pSendFrame->m_buf[28] = ' ';


	for (int i=0; i<Varlen; i++)
	{
		pSendFrame->m_buf[29+i] = charVarBuf[i];
	}

	pSendFrame->m_buf[29+Varlen] = ' ';
	WORD check = CUdfGlobalFUNC::CreateCRC_SCSW(pSendFrame->m_buf, 30+Varlen);
	pSendFrame->m_buf[30+Varlen] = HIBYTE(check);
	pSendFrame->m_buf[31+Varlen] = LOBYTE(check);
	pSendFrame->m_buf[32+Varlen] = 'N';
	pSendFrame->m_buf[33+Varlen] = 'N';
	pSendFrame->m_bufLen = 34+Varlen;	

	m_pCurrCallOP = scsw_interface_PORT_Set;
	SendRtuLinkFrame(pSendFrame->m_buf, pSendFrame->m_bufLen, pSendFrame->m_CommInterface);
}

void CSysDataTrans_Hydr::SCSW_hydrdata_EN_DEL_GSM(CHydrSendFrame *pSendFrame)
{
	if(NULL == pSendFrame)
		return;

	//-----------传感器号----------------
	CString strSensorNo = "";
	int m_nSensorNo = pSendFrame->m_LPARME;
	strSensorNo.Format("%02d", m_nSensorNo);
	char charSensor[3];
	memset(charSensor, 0, 3);
	memcpy(charSensor, strSensorNo.GetBuffer(2), 2);

	//-----------站码---------------------
	CString strStCode = pSendFrame->m_strStCode;
	char charStCode[11];
	memset(charStCode, 0, 10);
	memcpy(charStCode, strStCode.GetBuffer(10), 10);

	//--------------ST头----------------
	pSendFrame->m_buf[0] = 'S';
	pSendFrame->m_buf[1] = 'T';
	pSendFrame->m_buf[2] = ' ';
	//---------------站码---------------
	pSendFrame->m_buf[3] = charStCode[0];
	pSendFrame->m_buf[4] = charStCode[1];
	pSendFrame->m_buf[5] = charStCode[2];
	pSendFrame->m_buf[6] = charStCode[3];
	pSendFrame->m_buf[7] = charStCode[4];
	pSendFrame->m_buf[8] = charStCode[5];
	pSendFrame->m_buf[9] = charStCode[6];
	pSendFrame->m_buf[10] = charStCode[7];
	pSendFrame->m_buf[11] = charStCode[8];
	pSendFrame->m_buf[12] = charStCode[9];
	pSendFrame->m_buf[13] = ' ';

	pSendFrame->m_buf[14] = 'G';
	pSendFrame->m_buf[15] = 'S';
	pSendFrame->m_buf[16] = 'M';
	pSendFrame->m_buf[17] = charSensor[0];
	pSendFrame->m_buf[18] = charSensor[1];
	pSendFrame->m_buf[19] = ' ';
	pSendFrame->m_buf[20] = 'd';
	pSendFrame->m_buf[21] = 'e';
	pSendFrame->m_buf[22] = 'l';
	pSendFrame->m_buf[23] = ' ';
	WORD check = CUdfGlobalFUNC::CreateCRC_SCSW(pSendFrame->m_buf, 24);
	pSendFrame->m_buf[24] = HIBYTE(check);
	pSendFrame->m_buf[25] = LOBYTE(check);
	pSendFrame->m_buf[26] = 'N';
	pSendFrame->m_buf[27] = 'N';
	pSendFrame->m_bufLen = 28;	

	m_pCurrCallOP = scsw_interface_GSM_Del;
	SendRtuLinkFrame(pSendFrame->m_buf, pSendFrame->m_bufLen, pSendFrame->m_CommInterface);

}

void CSysDataTrans_Hydr::SCSW_hydrdata_EN_DEL_BD(CHydrSendFrame *pSendFrame)
{
	if(NULL == pSendFrame)
		return;

	//-----------传感器号----------------
	CString strSensorNo = "";
	int m_nSensorNo = pSendFrame->m_LPARME;
	strSensorNo.Format("%02d", m_nSensorNo);
	char charSensor[3];
	memset(charSensor, 0, 3);
	memcpy(charSensor, strSensorNo.GetBuffer(2), 2);

	//-----------站码---------------------
	CString strStCode = pSendFrame->m_strStCode;
	char charStCode[11];
	memset(charStCode, 0, 10);
	memcpy(charStCode, strStCode.GetBuffer(10), 10);

	//--------------ST头----------------
	pSendFrame->m_buf[0] = 'S';
	pSendFrame->m_buf[1] = 'T';
	pSendFrame->m_buf[2] = ' ';
	//---------------站码---------------
	pSendFrame->m_buf[3] = charStCode[0];
	pSendFrame->m_buf[4] = charStCode[1];
	pSendFrame->m_buf[5] = charStCode[2];
	pSendFrame->m_buf[6] = charStCode[3];
	pSendFrame->m_buf[7] = charStCode[4];
	pSendFrame->m_buf[8] = charStCode[5];
	pSendFrame->m_buf[9] = charStCode[6];
	pSendFrame->m_buf[10] = charStCode[7];
	pSendFrame->m_buf[11] = charStCode[8];
	pSendFrame->m_buf[12] = charStCode[9];
	pSendFrame->m_buf[13] = ' ';

	pSendFrame->m_buf[14] = 'B';
	pSendFrame->m_buf[15] = 'D';
	pSendFrame->m_buf[16] = charSensor[0];
	pSendFrame->m_buf[17] = charSensor[1];
	pSendFrame->m_buf[18] = ' ';
	pSendFrame->m_buf[19] = 'd';
	pSendFrame->m_buf[20] = 'e';
	pSendFrame->m_buf[21] = 'l';
	pSendFrame->m_buf[22] = ' ';
	WORD check = CUdfGlobalFUNC::CreateCRC_SCSW(pSendFrame->m_buf, 23);
	pSendFrame->m_buf[23] = HIBYTE(check);
	pSendFrame->m_buf[24] = LOBYTE(check);
	pSendFrame->m_buf[25] = 'N';
	pSendFrame->m_buf[26] = 'N';
	pSendFrame->m_bufLen = 27;	

	m_pCurrCallOP = scsw_interface_BD;
	SendRtuLinkFrame(pSendFrame->m_buf, pSendFrame->m_bufLen, pSendFrame->m_CommInterface);

}

void CSysDataTrans_Hydr::SCSW_hydrdata_EN_DEL_IP(CHydrSendFrame *pSendFrame)
{
	if(NULL == pSendFrame)
		return;

	//-----------传感器号----------------
	CString strSensorNo = "";
	int m_nSensorNo = pSendFrame->m_LPARME;
	strSensorNo.Format("%02d", m_nSensorNo);
	char charSensor[3];
	memset(charSensor, 0, 3);
	memcpy(charSensor, strSensorNo.GetBuffer(1), 1);

	//-----------标准---------------------
	CString strStardard = "";
	int nStardard = pSendFrame->m_RPARME;
	char chStardard = (char)nStardard;

	//-----------站码---------------------
	CString strStCode = pSendFrame->m_strStCode;
	char charStCode[11];
	memset(charStCode, 0, 10);
	memcpy(charStCode, strStCode.GetBuffer(10), 10);

	//--------------ST头----------------
	pSendFrame->m_buf[0] = 'S';
	pSendFrame->m_buf[1] = 'T';
	pSendFrame->m_buf[2] = ' ';
	//---------------站码---------------
	pSendFrame->m_buf[3] = charStCode[0];
	pSendFrame->m_buf[4] = charStCode[1];
	pSendFrame->m_buf[5] = charStCode[2];
	pSendFrame->m_buf[6] = charStCode[3];
	pSendFrame->m_buf[7] = charStCode[4];
	pSendFrame->m_buf[8] = charStCode[5];
	pSendFrame->m_buf[9] = charStCode[6];
	pSendFrame->m_buf[10] = charStCode[7];
	pSendFrame->m_buf[11] = charStCode[8];
	pSendFrame->m_buf[12] = charStCode[9];
	pSendFrame->m_buf[13] = ' ';

	pSendFrame->m_buf[14] = 'H';
	pSendFrame->m_buf[15] = 'O';
	pSendFrame->m_buf[16] = 'S';
	pSendFrame->m_buf[17] = 'T';
	pSendFrame->m_buf[18] = charSensor[0];
	pSendFrame->m_buf[19] = ' ';
	pSendFrame->m_buf[20] = 'd';
	pSendFrame->m_buf[21] = 'e';
	pSendFrame->m_buf[22] = 'l';
	pSendFrame->m_buf[23] = ' ';
	WORD check = CUdfGlobalFUNC::CreateCRC_SCSW(pSendFrame->m_buf, 24);
	pSendFrame->m_buf[24] = HIBYTE(check);
	pSendFrame->m_buf[25] = LOBYTE(check);
	pSendFrame->m_buf[26] = 'N';
	pSendFrame->m_buf[27] = 'N';
	pSendFrame->m_bufLen = 28;	

	m_pCurrCallOP = scsw_interface_IP_Del;
	SendRtuLinkFrame(pSendFrame->m_buf, pSendFrame->m_bufLen, pSendFrame->m_CommInterface);

}

void CSysDataTrans_Hydr::SCSW_hydrdata_EN_DEL_IP2(CHydrSendFrame *pSendFrame)
{
	if(NULL == pSendFrame)
		return;

	//-----------传感器号----------------
	CString strSensorNo = "";
	int m_nSensorNo = pSendFrame->m_LPARME;
	strSensorNo.Format("%02d", m_nSensorNo);
	char charSensor[3];
	memset(charSensor, 0, 3);
	memcpy(charSensor, strSensorNo.GetBuffer(2), 2);

	//-----------标准---------------------
	CString strStardard = "";
	int nStardard = pSendFrame->m_RPARME;
	char chStardard = (char)nStardard;

	//-----------站码---------------------
	CString strStCode = pSendFrame->m_strStCode;
	char charStCode[11];
	memset(charStCode, 0, 10);
	memcpy(charStCode, strStCode.GetBuffer(10), 10);

	//--------------ST头----------------
	pSendFrame->m_buf[0] = 'S';
	pSendFrame->m_buf[1] = 'T';
	pSendFrame->m_buf[2] = ' ';
	//---------------站码---------------
	pSendFrame->m_buf[3] = charStCode[0];
	pSendFrame->m_buf[4] = charStCode[1];
	pSendFrame->m_buf[5] = charStCode[2];
	pSendFrame->m_buf[6] = charStCode[3];
	pSendFrame->m_buf[7] = charStCode[4];
	pSendFrame->m_buf[8] = charStCode[5];
	pSendFrame->m_buf[9] = charStCode[6];
	pSendFrame->m_buf[10] = charStCode[7];
	pSendFrame->m_buf[11] = charStCode[8];
	pSendFrame->m_buf[12] = charStCode[9];
	pSendFrame->m_buf[13] = ' ';

	pSendFrame->m_buf[14] = 'I';
	pSendFrame->m_buf[15] = 'T';
	pSendFrame->m_buf[16] = charSensor[0];
	pSendFrame->m_buf[17] = charSensor[1];
	pSendFrame->m_buf[18] = ' ';
	pSendFrame->m_buf[19] = 'd';
	pSendFrame->m_buf[20] = 'e';
	pSendFrame->m_buf[21] = 'l';
	pSendFrame->m_buf[22] = ' ';
	WORD check = CUdfGlobalFUNC::CreateCRC_SCSW(pSendFrame->m_buf, 23);
	pSendFrame->m_buf[23] = HIBYTE(check);
	pSendFrame->m_buf[24] = LOBYTE(check);
	pSendFrame->m_buf[25] = 'N';
	pSendFrame->m_buf[26] = 'N';
	pSendFrame->m_bufLen = 27;	

	m_pCurrCallOP = scsw_interface_IP_Del2;
	SendRtuLinkFrame(pSendFrame->m_buf, pSendFrame->m_bufLen, pSendFrame->m_CommInterface);

}


void CSysDataTrans_Hydr::SCSW_hydrdata_EN_DEL_PORT(CHydrSendFrame *pSendFrame)
{
	if(NULL == pSendFrame)
		return;

	//-----------传感器号----------------
	CString strSensorNo = "";
	int m_nSensorNo = pSendFrame->m_LPARME;
	strSensorNo.Format("%02d", m_nSensorNo);
	char charSensor[3];
	memset(charSensor, 0, 3);
	memcpy(charSensor, strSensorNo.GetBuffer(2), 2);

	//-----------标准---------------------
	CString strStardard = "";
	int nStardard = pSendFrame->m_RPARME;
	char chStardard = (char)nStardard;

	//-----------站码---------------------
	CString strStCode = pSendFrame->m_strStCode;
	char charStCode[11];
	memset(charStCode, 0, 10);
	memcpy(charStCode, strStCode.GetBuffer(10), 10);

	//--------------ST头----------------
	pSendFrame->m_buf[0] = 'S';
	pSendFrame->m_buf[1] = 'T';
	pSendFrame->m_buf[2] = ' ';
	//---------------站码---------------
	pSendFrame->m_buf[3] = charStCode[0];
	pSendFrame->m_buf[4] = charStCode[1];
	pSendFrame->m_buf[5] = charStCode[2];
	pSendFrame->m_buf[6] = charStCode[3];
	pSendFrame->m_buf[7] = charStCode[4];
	pSendFrame->m_buf[8] = charStCode[5];
	pSendFrame->m_buf[9] = charStCode[6];
	pSendFrame->m_buf[10] = charStCode[7];
	pSendFrame->m_buf[11] = charStCode[8];
	pSendFrame->m_buf[12] = charStCode[9];
	pSendFrame->m_buf[13] = ' ';

	pSendFrame->m_buf[14] = 'P';
	pSendFrame->m_buf[15] = 'T';
	pSendFrame->m_buf[16] = charSensor[0];
	pSendFrame->m_buf[17] = charSensor[1];
	pSendFrame->m_buf[18] = ' ';
	pSendFrame->m_buf[19] = 'd';
	pSendFrame->m_buf[19] = 'e';
	pSendFrame->m_buf[19] = 'l';
	pSendFrame->m_buf[20] = ' ';
	WORD check = CUdfGlobalFUNC::CreateCRC_SCSW(pSendFrame->m_buf, 21);
	pSendFrame->m_buf[21] = HIBYTE(check);
	pSendFrame->m_buf[22] = LOBYTE(check);
	pSendFrame->m_buf[23] = 'N';
	pSendFrame->m_buf[24] = 'N';
	pSendFrame->m_bufLen = 25;	

	m_pCurrCallOP = scsw_interface_PORT;
	SendRtuLinkFrame(pSendFrame->m_buf, pSendFrame->m_bufLen, pSendFrame->m_CommInterface);

}

void CSysDataTrans_Hydr::SCSW_hydrdata_EN_SET_PI(CHydrSendFrame *pSendFrame)
{
	if(NULL == pSendFrame)
		return;

	CString strPsw = pSendFrame->m_strPsw;
	char charPsw[7];
	memset(charPsw, 0, 6);
	memcpy(charPsw, strPsw.GetBuffer(6), 6);

	CString SetVar = "";
	float fSetVar = pSendFrame->m_setVar;
	int nSetVar = (int)fSetVar;
	SetVar.Format("%d", nSetVar);
	int Varlen = SetVar.GetLength();
	char charVarBuf[20];
	memset(charVarBuf, 0, 19);
	memcpy(charVarBuf, SetVar.GetBuffer(19), 19);

	//-----------站码---------------------
	CString strStCode = pSendFrame->m_strStCode;
	char charStCode[11];
	memset(charStCode, 0, 10);
	memcpy(charStCode, strStCode.GetBuffer(10), 10);

	//--------------ST头----------------
	pSendFrame->m_buf[0] = 'S';
	pSendFrame->m_buf[1] = 'T';
	pSendFrame->m_buf[2] = ' ';
	//---------------站码---------------
	pSendFrame->m_buf[3] = charStCode[0];
	pSendFrame->m_buf[4] = charStCode[1];
	pSendFrame->m_buf[5] = charStCode[2];
	pSendFrame->m_buf[6] = charStCode[3];
	pSendFrame->m_buf[7] = charStCode[4];
	pSendFrame->m_buf[8] = charStCode[5];
	pSendFrame->m_buf[9] = charStCode[6];
	pSendFrame->m_buf[10] = charStCode[7];
	pSendFrame->m_buf[11] = charStCode[8];
	pSendFrame->m_buf[12] = charStCode[9];
	pSendFrame->m_buf[13] = ' ';

	pSendFrame->m_buf[14] = 'P';
	pSendFrame->m_buf[15] = 'W';
	pSendFrame->m_buf[16] = ' ';
	pSendFrame->m_buf[17] = charPsw[0];
	pSendFrame->m_buf[18] = charPsw[1];
	pSendFrame->m_buf[19] = charPsw[2];
	pSendFrame->m_buf[20] = charPsw[3];
	pSendFrame->m_buf[21] = charPsw[4];
	pSendFrame->m_buf[22] = charPsw[5];
	pSendFrame->m_buf[23] = ' ';

	pSendFrame->m_buf[24] = 'P';
	pSendFrame->m_buf[25] = 'I';
	pSendFrame->m_buf[26] = ' ';

	for (int i=0; i<Varlen; i++)
	{
		pSendFrame->m_buf[27+i] = charVarBuf[i];
	}

	pSendFrame->m_buf[27+Varlen] = ' ';
	pSendFrame->m_buf[28+Varlen] = 'a';
	pSendFrame->m_buf[29+Varlen] = 'a';
	pSendFrame->m_buf[30+Varlen] = 'N';
	pSendFrame->m_buf[31+Varlen] = 'N';
	pSendFrame->m_bufLen = 32+Varlen;	

	m_pCurrCallOP = scsw_waterRainStardard;
	SendRtuLinkFrame(pSendFrame->m_buf, pSendFrame->m_bufLen, pSendFrame->m_CommInterface);

}

void CSysDataTrans_Hydr::SCSW_hydrdata_EN_SET_RD(CHydrSendFrame *pSendFrame)
{
	if(NULL == pSendFrame)
		return;

	CString strPsw = pSendFrame->m_strPsw;
	char charPsw[7];
	memset(charPsw, 0, 6);
	memcpy(charPsw, strPsw.GetBuffer(6), 6);

	CString SetVar = "";
	float fSetVar = pSendFrame->m_setVar;
	int nSetVar = (int)fSetVar;
	SetVar.Format("%d", nSetVar);
	int Varlen = SetVar.GetLength();
	char charVarBuf[20];
	memset(charVarBuf, 0, 19);
	memcpy(charVarBuf, SetVar.GetBuffer(19), 19);

	//-----------站码---------------------
	CString strStCode = pSendFrame->m_strStCode;
	char charStCode[11];
	memset(charStCode, 0, 10);
	memcpy(charStCode, strStCode.GetBuffer(10), 10);

	//--------------ST头----------------
	pSendFrame->m_buf[0] = 'S';
	pSendFrame->m_buf[1] = 'T';
	pSendFrame->m_buf[2] = ' ';
	//---------------站码---------------
	pSendFrame->m_buf[3] = charStCode[0];
	pSendFrame->m_buf[4] = charStCode[1];
	pSendFrame->m_buf[5] = charStCode[2];
	pSendFrame->m_buf[6] = charStCode[3];
	pSendFrame->m_buf[7] = charStCode[4];
	pSendFrame->m_buf[8] = charStCode[5];
	pSendFrame->m_buf[9] = charStCode[6];
	pSendFrame->m_buf[10] = charStCode[7];
	pSendFrame->m_buf[11] = charStCode[8];
	pSendFrame->m_buf[12] = charStCode[9];
	pSendFrame->m_buf[13] = ' ';

	pSendFrame->m_buf[14] = 'P';
	pSendFrame->m_buf[15] = 'W';
	pSendFrame->m_buf[16] = ' ';
	pSendFrame->m_buf[17] = charPsw[0];
	pSendFrame->m_buf[18] = charPsw[1];
	pSendFrame->m_buf[19] = charPsw[2];
	pSendFrame->m_buf[20] = charPsw[3];
	pSendFrame->m_buf[21] = charPsw[4];
	pSendFrame->m_buf[22] = charPsw[5];
	pSendFrame->m_buf[23] = ' ';

	pSendFrame->m_buf[24] = 'R';
	pSendFrame->m_buf[25] = 'D';
	pSendFrame->m_buf[26] = ' ';

	for (int i=0; i<Varlen; i++)
	{
		pSendFrame->m_buf[27+i] = charVarBuf[i];
	}

	pSendFrame->m_buf[27+Varlen] = ' ';
	pSendFrame->m_buf[28+Varlen] = 'a';
	pSendFrame->m_buf[29+Varlen] = 'a';
	pSendFrame->m_buf[30+Varlen] = 'N';
	pSendFrame->m_buf[31+Varlen] = 'N';
	pSendFrame->m_bufLen = 32+Varlen;	

	m_pCurrCallOP = scsw_waterRainSensorDpi;
	SendRtuLinkFrame(pSendFrame->m_buf, pSendFrame->m_bufLen, pSendFrame->m_CommInterface);
}

void CSysDataTrans_Hydr::SCSW_hydrdata_EN_SET_ZJ(CHydrSendFrame *pSendFrame)
{
	if(NULL == pSendFrame)
		return;

	CString strPsw = pSendFrame->m_strPsw;
	char charPsw[7];
	memset(charPsw, 0, 6);
	memcpy(charPsw, strPsw.GetBuffer(6), 6);

	//-----------传感器号----------------
	CString strSensorNo = "";
	int m_nSensorNo = pSendFrame->m_LPARME;
	strSensorNo.Format("%02d", m_nSensorNo);
	char charSensor[3];
	memset(charSensor, 0, 3);
	memcpy(charSensor, strSensorNo.GetBuffer(2), 2);

	CString SetVar = "";
	float fSetVar = pSendFrame->m_setVar;
	int nSetVar = (int)fSetVar;
	SetVar.Format("%d", nSetVar);
	int Varlen = SetVar.GetLength();
	char charVarBuf[20];
	memset(charVarBuf, 0, 19);
	memcpy(charVarBuf, SetVar.GetBuffer(19), 19);

	//-----------站码---------------------
	CString strStCode = pSendFrame->m_strStCode;
	char charStCode[11];
	memset(charStCode, 0, 10);
	memcpy(charStCode, strStCode.GetBuffer(10), 10);

	//--------------ST头----------------
	pSendFrame->m_buf[0] = 'S';
	pSendFrame->m_buf[1] = 'T';
	pSendFrame->m_buf[2] = ' ';
	//---------------站码---------------
	pSendFrame->m_buf[3] = charStCode[0];
	pSendFrame->m_buf[4] = charStCode[1];
	pSendFrame->m_buf[5] = charStCode[2];
	pSendFrame->m_buf[6] = charStCode[3];
	pSendFrame->m_buf[7] = charStCode[4];
	pSendFrame->m_buf[8] = charStCode[5];
	pSendFrame->m_buf[9] = charStCode[6];
	pSendFrame->m_buf[10] = charStCode[7];
	pSendFrame->m_buf[11] = charStCode[8];
	pSendFrame->m_buf[12] = charStCode[9];
	pSendFrame->m_buf[13] = ' ';

	pSendFrame->m_buf[14] = 'P';
	pSendFrame->m_buf[15] = 'W';
	pSendFrame->m_buf[16] = ' ';
	pSendFrame->m_buf[17] = charPsw[0];
	pSendFrame->m_buf[18] = charPsw[1];
	pSendFrame->m_buf[19] = charPsw[2];
	pSendFrame->m_buf[20] = charPsw[3];
	pSendFrame->m_buf[21] = charPsw[4];
	pSendFrame->m_buf[22] = charPsw[5];
	pSendFrame->m_buf[23] = ' ';


	pSendFrame->m_buf[24] = 'Z';
	pSendFrame->m_buf[25] = 'J';
	pSendFrame->m_buf[26] = charSensor[0];
	pSendFrame->m_buf[27] = charSensor[1];
	pSendFrame->m_buf[28] = ' ';

	for (int i=0; i<Varlen; i++)
	{
		pSendFrame->m_buf[29+i] = charVarBuf[i];
	}

	pSendFrame->m_buf[29+Varlen] = ' ';
	pSendFrame->m_buf[30+Varlen] = 'a';
	pSendFrame->m_buf[31+Varlen] = 'a';
	pSendFrame->m_buf[32+Varlen] = 'N';
	pSendFrame->m_buf[33+Varlen] = 'N';
	pSendFrame->m_bufLen = 34+Varlen;	

	m_pCurrCallOP = scsw_waterRainSensorBase;
	SendRtuLinkFrame(pSendFrame->m_buf, pSendFrame->m_bufLen, pSendFrame->m_CommInterface);

}

void CSysDataTrans_Hydr::SCSW_hydrdata_EN_SET_ZT(CHydrSendFrame *pSendFrame)
{
	if(NULL == pSendFrame)
		return;

	CString strPsw = pSendFrame->m_strPsw;
	char charPsw[7];
	memset(charPsw, 0, 6);
	memcpy(charPsw, strPsw.GetBuffer(6), 6);

	//-----------传感器号----------------
	CString strSensorNo = "";
	int m_nSensorNo = pSendFrame->m_LPARME;
	strSensorNo.Format("%02d", m_nSensorNo);
	char charSensor[3];
	memset(charSensor, 0, 3);
	memcpy(charSensor, strSensorNo.GetBuffer(2), 2);

	CString SetVar = "";
	float fSetVar = pSendFrame->m_setVar;
	int nSetVar = (int)fSetVar;
	SetVar.Format("%d", nSetVar);
	int Varlen = SetVar.GetLength();
	char charVarBuf[20];
	memset(charVarBuf, 0, 19);
	memcpy(charVarBuf, SetVar.GetBuffer(19), 19);

	//-----------站码---------------------
	CString strStCode = pSendFrame->m_strStCode;
	char charStCode[11];
	memset(charStCode, 0, 10);
	memcpy(charStCode, strStCode.GetBuffer(10), 10);

	//--------------ST头----------------
	pSendFrame->m_buf[0] = 'S';
	pSendFrame->m_buf[1] = 'T';
	pSendFrame->m_buf[2] = ' ';
	//---------------站码---------------
	pSendFrame->m_buf[3] = charStCode[0];
	pSendFrame->m_buf[4] = charStCode[1];
	pSendFrame->m_buf[5] = charStCode[2];
	pSendFrame->m_buf[6] = charStCode[3];
	pSendFrame->m_buf[7] = charStCode[4];
	pSendFrame->m_buf[8] = charStCode[5];
	pSendFrame->m_buf[9] = charStCode[6];
	pSendFrame->m_buf[10] = charStCode[7];
	pSendFrame->m_buf[11] = charStCode[8];
	pSendFrame->m_buf[12] = charStCode[9];
	pSendFrame->m_buf[13] = ' ';

	pSendFrame->m_buf[14] = 'P';
	pSendFrame->m_buf[15] = 'W';
	pSendFrame->m_buf[16] = ' ';
	pSendFrame->m_buf[17] = charPsw[0];
	pSendFrame->m_buf[18] = charPsw[1];
	pSendFrame->m_buf[19] = charPsw[2];
	pSendFrame->m_buf[20] = charPsw[3];
	pSendFrame->m_buf[21] = charPsw[4];
	pSendFrame->m_buf[22] = charPsw[5];
	pSendFrame->m_buf[23] = ' ';

	pSendFrame->m_buf[24] = 'Z';
	pSendFrame->m_buf[25] = 'T';
	pSendFrame->m_buf[26] = charSensor[0];
	pSendFrame->m_buf[27] = charSensor[1];
	pSendFrame->m_buf[28] = ' ';

	for (int i=0; i<Varlen; i++)
	{
		pSendFrame->m_buf[29+i] = charVarBuf[i];
	}

	pSendFrame->m_buf[29+Varlen] = ' ';
	pSendFrame->m_buf[30+Varlen] = 'a';
	pSendFrame->m_buf[31+Varlen] = 'a';
	pSendFrame->m_buf[32+Varlen] = 'N';
	pSendFrame->m_buf[33+Varlen] = 'N';
	pSendFrame->m_bufLen = 34+Varlen;	

	m_pCurrCallOP = scsw_waterRainSensorPara;
	SendRtuLinkFrame(pSendFrame->m_buf, pSendFrame->m_bufLen, pSendFrame->m_CommInterface);

}

void CSysDataTrans_Hydr::SCSW_hydrdata_EN_SET_ZF(CHydrSendFrame *pSendFrame)
{
	if(NULL == pSendFrame)
		return;

	CString strPsw = pSendFrame->m_strPsw;
	char charPsw[7];
	memset(charPsw, 0, 6);
	memcpy(charPsw, strPsw.GetBuffer(6), 6);

	//-----------传感器号----------------
	CString strSensorNo = "";
	int m_nSensorNo = pSendFrame->m_LPARME;
	strSensorNo.Format("%02d", m_nSensorNo);
	char charSensor[3];
	memset(charSensor, 0, 3);
	memcpy(charSensor, strSensorNo.GetBuffer(2), 2);

	CString SetVar = "";
	float fSetVar = pSendFrame->m_setVar;
	int nSetVar = (int)fSetVar;
	SetVar.Format("%d", nSetVar);
	int Varlen = SetVar.GetLength();
	char charVarBuf[20];
	memset(charVarBuf, 0, 19);
	memcpy(charVarBuf, SetVar.GetBuffer(19), 19);

	//-----------站码---------------------
	CString strStCode = pSendFrame->m_strStCode;
	char charStCode[11];
	memset(charStCode, 0, 10);
	memcpy(charStCode, strStCode.GetBuffer(10), 10);

	//--------------ST头----------------
	pSendFrame->m_buf[0] = 'S';
	pSendFrame->m_buf[1] = 'T';
	pSendFrame->m_buf[2] = ' ';
	//---------------站码---------------
	pSendFrame->m_buf[3] = charStCode[0];
	pSendFrame->m_buf[4] = charStCode[1];
	pSendFrame->m_buf[5] = charStCode[2];
	pSendFrame->m_buf[6] = charStCode[3];
	pSendFrame->m_buf[7] = charStCode[4];
	pSendFrame->m_buf[8] = charStCode[5];
	pSendFrame->m_buf[9] = charStCode[6];
	pSendFrame->m_buf[10] = charStCode[7];
	pSendFrame->m_buf[11] = charStCode[8];
	pSendFrame->m_buf[12] = charStCode[9];
	pSendFrame->m_buf[13] = ' ';

	pSendFrame->m_buf[14] = 'P';
	pSendFrame->m_buf[15] = 'W';
	pSendFrame->m_buf[16] = ' ';
	pSendFrame->m_buf[17] = charPsw[0];
	pSendFrame->m_buf[18] = charPsw[1];
	pSendFrame->m_buf[19] = charPsw[2];
	pSendFrame->m_buf[20] = charPsw[3];
	pSendFrame->m_buf[21] = charPsw[4];
	pSendFrame->m_buf[22] = charPsw[5];
	pSendFrame->m_buf[23] = ' ';

	pSendFrame->m_buf[24] = 'Z';
	pSendFrame->m_buf[25] = 'F';
	pSendFrame->m_buf[26] = charSensor[0];
	pSendFrame->m_buf[27] = charSensor[1];
	pSendFrame->m_buf[28] = ' ';
	for (int i=0; i<Varlen; i++)
	{
		pSendFrame->m_buf[29+i] = charVarBuf[i];

	}

	pSendFrame->m_buf[29+Varlen] = ' ';
	pSendFrame->m_buf[30+Varlen] = 'a';
	pSendFrame->m_buf[31+Varlen] = 'a';
	pSendFrame->m_buf[32+Varlen] = 'N';
	pSendFrame->m_buf[33+Varlen] = 'N';
	pSendFrame->m_bufLen = 34+Varlen;	

	m_pCurrCallOP = scsw_waterDefine;
	SendRtuLinkFrame(pSendFrame->m_buf, pSendFrame->m_bufLen, pSendFrame->m_CommInterface);

}

void CSysDataTrans_Hydr::SCSW_hydrdata_EN_SET_Zx(CHydrSendFrame *pSendFrame)
{
	if(NULL == pSendFrame)
		return;

	CString strPsw = pSendFrame->m_strPsw;
	char charPsw[7];
	memset(charPsw, 0, 6);
	memcpy(charPsw, strPsw.GetBuffer(6), 6);

	//-----------传感器号----------------
	CString strSensorNo = "";
	int m_nSensorNo = pSendFrame->m_LPARME;
	strSensorNo.Format("%02d", m_nSensorNo);
	char charSensor[3];
	memset(charSensor, 0, 3);
	memcpy(charSensor, strSensorNo.GetBuffer(2), 2);

	//-----------标准---------------------
	CString strStardard = "";
	int nStardard = pSendFrame->m_RPARME;
	char chStardard = (char)nStardard;

	CString SetVar = "";
	float fSetVar = pSendFrame->m_setVar;
	int nSetVar = (int)fSetVar;
	SetVar.Format("%d", nSetVar);
	int Varlen = SetVar.GetLength();
	char charVarBuf[20];
	memset(charVarBuf, 0, 19);
	memcpy(charVarBuf, SetVar.GetBuffer(19), 19);

	//-----------站码---------------------
	CString strStCode = pSendFrame->m_strStCode;
	char charStCode[11];
	memset(charStCode, 0, 10);
	memcpy(charStCode, strStCode.GetBuffer(10), 10);

	//--------------ST头----------------
	pSendFrame->m_buf[0] = 'S';
	pSendFrame->m_buf[1] = 'T';
	pSendFrame->m_buf[2] = ' ';
	//---------------站码---------------
	pSendFrame->m_buf[3] = charStCode[0];
	pSendFrame->m_buf[4] = charStCode[1];
	pSendFrame->m_buf[5] = charStCode[2];
	pSendFrame->m_buf[6] = charStCode[3];
	pSendFrame->m_buf[7] = charStCode[4];
	pSendFrame->m_buf[8] = charStCode[5];
	pSendFrame->m_buf[9] = charStCode[6];
	pSendFrame->m_buf[10] = charStCode[7];
	pSendFrame->m_buf[11] = charStCode[8];
	pSendFrame->m_buf[12] = charStCode[9];
	pSendFrame->m_buf[13] = ' ';

	pSendFrame->m_buf[14] = 'P';
	pSendFrame->m_buf[15] = 'W';
	pSendFrame->m_buf[16] = ' ';
	pSendFrame->m_buf[17] = charPsw[0];
	pSendFrame->m_buf[18] = charPsw[1];
	pSendFrame->m_buf[19] = charPsw[2];
	pSendFrame->m_buf[20] = charPsw[3];
	pSendFrame->m_buf[21] = charPsw[4];
	pSendFrame->m_buf[22] = charPsw[5];
	pSendFrame->m_buf[23] = ' ';

	pSendFrame->m_buf[24] = 'Z';
	pSendFrame->m_buf[25] = chStardard;
	pSendFrame->m_buf[26] = charSensor[0];
	pSendFrame->m_buf[27] = charSensor[1];
	pSendFrame->m_buf[28] = ' ';

	for (int i=0; i<Varlen; i++)
	{
		pSendFrame->m_buf[29+i] = charVarBuf[i];

	}

	pSendFrame->m_buf[29+Varlen] = ' ';
	pSendFrame->m_buf[30+Varlen] = 'a';
	pSendFrame->m_buf[31+Varlen] = 'a';
	pSendFrame->m_buf[32+Varlen] = 'N';
	pSendFrame->m_buf[33+Varlen] = 'N';
	pSendFrame->m_bufLen = 34+Varlen;	

	m_pCurrCallOP = scsw_waterrainTransStardard;
	SendRtuLinkFrame(pSendFrame->m_buf, pSendFrame->m_bufLen, pSendFrame->m_CommInterface);

}

void CSysDataTrans_Hydr::SCSW_hydrdata_EN_SET_DZx(CHydrSendFrame *pSendFrame)
{
	if(NULL == pSendFrame)
		return;

	CString strPsw = pSendFrame->m_strPsw;
	char charPsw[7];
	memset(charPsw, 0, 6);
	memcpy(charPsw, strPsw.GetBuffer(6), 6);

	//-----------传感器号----------------
	CString strSensorNo = "";
	int m_nSensorNo = pSendFrame->m_LPARME;
	strSensorNo.Format("%02d", m_nSensorNo);
	char charSensor[3];
	memset(charSensor, 0, 3);
	memcpy(charSensor, strSensorNo.GetBuffer(2), 2);

	//-----------标准---------------------
	CString strStardard = "";
	int nStardard = pSendFrame->m_RPARME;
	char chStardard = (char)nStardard;

	CString SetVar = "";
	float fSetVar = pSendFrame->m_setVar;
	int nSetVar = (int)fSetVar;
	SetVar.Format("%d", nSetVar);
	int Varlen = SetVar.GetLength();
	char charVarBuf[20];
	memset(charVarBuf, 0, 19);
	memcpy(charVarBuf, SetVar.GetBuffer(19), 19);

	//-----------站码---------------------
	CString strStCode = pSendFrame->m_strStCode;
	char charStCode[11];
	memset(charStCode, 0, 10);
	memcpy(charStCode, strStCode.GetBuffer(10), 10);

	//--------------ST头----------------
	pSendFrame->m_buf[0] = 'S';
	pSendFrame->m_buf[1] = 'T';
	pSendFrame->m_buf[2] = ' ';
	//---------------站码---------------
	pSendFrame->m_buf[3] = charStCode[0];
	pSendFrame->m_buf[4] = charStCode[1];
	pSendFrame->m_buf[5] = charStCode[2];
	pSendFrame->m_buf[6] = charStCode[3];
	pSendFrame->m_buf[7] = charStCode[4];
	pSendFrame->m_buf[8] = charStCode[5];
	pSendFrame->m_buf[9] = charStCode[6];
	pSendFrame->m_buf[10] = charStCode[7];
	pSendFrame->m_buf[11] = charStCode[8];
	pSendFrame->m_buf[12] = charStCode[9];
	pSendFrame->m_buf[13] = ' ';

	pSendFrame->m_buf[14] = 'P';
	pSendFrame->m_buf[15] = 'W';
	pSendFrame->m_buf[16] = ' ';
	pSendFrame->m_buf[17] = charPsw[0];
	pSendFrame->m_buf[18] = charPsw[1];
	pSendFrame->m_buf[19] = charPsw[2];
	pSendFrame->m_buf[20] = charPsw[3];
	pSendFrame->m_buf[21] = charPsw[4];
	pSendFrame->m_buf[22] = charPsw[5];
	pSendFrame->m_buf[23] = ' ';

	pSendFrame->m_buf[24] = 'D';
	pSendFrame->m_buf[25] = 'Z';
	pSendFrame->m_buf[26] = chStardard;
	pSendFrame->m_buf[27] = charSensor[0];
	pSendFrame->m_buf[28] = charSensor[1];
	pSendFrame->m_buf[29] = ' ';

	for (int i=0; i<Varlen; i++)
	{
		pSendFrame->m_buf[30+i] = charVarBuf[i];

	}

	pSendFrame->m_buf[30+Varlen] = ' ';
	pSendFrame->m_buf[31+Varlen] = 'a';
	pSendFrame->m_buf[32+Varlen] = 'a';
	pSendFrame->m_buf[33+Varlen] = 'N';
	pSendFrame->m_buf[34+Varlen] = 'N';
	pSendFrame->m_bufLen = 35+Varlen;	

	m_pCurrCallOP = scsw_waterrainTimespan;
	SendRtuLinkFrame(pSendFrame->m_buf, pSendFrame->m_bufLen, pSendFrame->m_CommInterface);

}

void CSysDataTrans_Hydr::SCSW_hydrdata_EN_SET_TT(CHydrSendFrame *pSendFrame)
{
	if (NULL == pSendFrame)
		return;

	CString strPsw = pSendFrame->m_strPsw;
	char charPsw[7];
	memset(charPsw, 0, 6);
	memcpy(charPsw, strPsw.GetBuffer(6), 6);

	//-----------站码---------------------
	CString strStCode = pSendFrame->m_strStCode;
	char charStCode[11];
	memset(charStCode, 0, 10);
	memcpy(charStCode, strStCode.GetBuffer(10), 10);

	CString strTime_a = "";
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);

	strTime_a.Format("%2d%02d%02d%02d%02d%02d",
		(sysTime.wYear)%100,
		sysTime.wMonth,
		sysTime.wDay,
		sysTime.wHour,
		sysTime.wMinute,
		sysTime.wSecond);

	char charTimeA[13];
	memset(charTimeA, 0, 12);
	memcpy(charTimeA, strTime_a.GetBuffer(12), 12);


	//--------------ST头----------------
	pSendFrame->m_buf[0] = 'S';
	pSendFrame->m_buf[1] = 'T';
	pSendFrame->m_buf[2] = ' ';
	//---------------站码---------------
	pSendFrame->m_buf[3] = charStCode[0];
	pSendFrame->m_buf[4] = charStCode[1];
	pSendFrame->m_buf[5] = charStCode[2];
	pSendFrame->m_buf[6] = charStCode[3];
	pSendFrame->m_buf[7] = charStCode[4];
	pSendFrame->m_buf[8] = charStCode[5];
	pSendFrame->m_buf[9] = charStCode[6];
	pSendFrame->m_buf[10] = charStCode[7];
	pSendFrame->m_buf[11] = charStCode[8];
	pSendFrame->m_buf[12] = charStCode[9];
	pSendFrame->m_buf[13] = ' ';

	pSendFrame->m_buf[14] = 'P';
	pSendFrame->m_buf[15] = 'W';
	pSendFrame->m_buf[16] = ' ';
	pSendFrame->m_buf[17] = charPsw[0];
	pSendFrame->m_buf[18] = charPsw[1];
	pSendFrame->m_buf[19] = charPsw[2];
	pSendFrame->m_buf[20] = charPsw[3];
	pSendFrame->m_buf[21] = charPsw[4];
	pSendFrame->m_buf[22] = charPsw[5];
	pSendFrame->m_buf[23] = ' ';

	pSendFrame->m_buf[24] = 'T';
	pSendFrame->m_buf[25] = 'T';
	pSendFrame->m_buf[26] = ' ';
	pSendFrame->m_buf[27] = charTimeA[0];
	pSendFrame->m_buf[28] = charTimeA[1];
	pSendFrame->m_buf[29] = charTimeA[2];
	pSendFrame->m_buf[30] = charTimeA[3];
	pSendFrame->m_buf[31] = charTimeA[4];
	pSendFrame->m_buf[32] = charTimeA[5];
	pSendFrame->m_buf[33] = charTimeA[6];
	pSendFrame->m_buf[34] = charTimeA[7];
	pSendFrame->m_buf[35] = charTimeA[8];
	pSendFrame->m_buf[36] = charTimeA[9];
	pSendFrame->m_buf[37] = charTimeA[10];
	pSendFrame->m_buf[38] = charTimeA[11];
	pSendFrame->m_buf[39] = ' ';

	WORD check = CUdfGlobalFUNC::CreateCRC_SCSW(pSendFrame->m_buf, 40);
	pSendFrame->m_buf[40] = HIBYTE(check);
	pSendFrame->m_buf[41] = LOBYTE(check);
	pSendFrame->m_buf[42] = 'N';
	pSendFrame->m_buf[43] = 'N';

	pSendFrame->m_bufLen = 44;
	m_pCurrCallOP = scsw_waterCheckTime;
	SendRtuLinkFrame(pSendFrame->m_buf, pSendFrame->m_bufLen, pSendFrame->m_CommInterface);

}

void CSysDataTrans_Hydr::SCSW_hydrdata_EN_SET_HOST(CHydrSendFrame *pSendFrame)
{

}

void CSysDataTrans_Hydr::SCSW_hydrdata_EN_ZR(CHydrSendFrame *pSendFrame)
{
	if(NULL == pSendFrame)
		return;

	//-----------传感器号----------------
	CString strSensorNo = "";
	int m_nSensorNo = pSendFrame->m_LPARME;
	strSensorNo.Format("%02d", m_nSensorNo);
	char charSensor[3];
	memset(charSensor, 0, 3);
	memcpy(charSensor, strSensorNo.GetBuffer(2), 2);

	//-----------站码---------------------
	CString strStCode = pSendFrame->m_strStCode;
	char charStCode[11];
	memset(charStCode, 0, 10);
	memcpy(charStCode, strStCode.GetBuffer(10), 10);

	//--------------ST头----------------
	pSendFrame->m_buf[0] = 'S';
	pSendFrame->m_buf[1] = 'T';
	pSendFrame->m_buf[2] = ' ';
	//---------------站码---------------
	pSendFrame->m_buf[3] = charStCode[0];
	pSendFrame->m_buf[4] = charStCode[1];
	pSendFrame->m_buf[5] = charStCode[2];
	pSendFrame->m_buf[6] = charStCode[3];
	pSendFrame->m_buf[7] = charStCode[4];
	pSendFrame->m_buf[8] = charStCode[5];
	pSendFrame->m_buf[9] = charStCode[6];
	pSendFrame->m_buf[10] = charStCode[7];
	pSendFrame->m_buf[11] = charStCode[8];
	pSendFrame->m_buf[12] = charStCode[9];
	pSendFrame->m_buf[13] = ' ';

	pSendFrame->m_buf[14] = 'Z';
	pSendFrame->m_buf[15] = 'R';
	pSendFrame->m_buf[16] = charSensor[0];
	pSendFrame->m_buf[17] = charSensor[1];
	pSendFrame->m_buf[18] = ' ';
	pSendFrame->m_buf[19] = '?';
	pSendFrame->m_buf[20] = ' ';
	pSendFrame->m_buf[21] = 'a';
	pSendFrame->m_buf[22] = 'a';
	pSendFrame->m_buf[23] = 'N';
	pSendFrame->m_buf[24] = 'N';
	pSendFrame->m_bufLen = 25;	

	m_pCurrCallOP = scsw_workconditionSensorVar;
	SendRtuLinkFrame(pSendFrame->m_buf, pSendFrame->m_bufLen, pSendFrame->m_CommInterface);

}

void CSysDataTrans_Hydr::SCSW_hydrdata_EN_RC(CHydrSendFrame *pSendFrame)
{
	if(NULL == pSendFrame)
		return;

	//-----------站码---------------------
	CString strStCode = pSendFrame->m_strStCode;
	char charStCode[11];
	memset(charStCode, 0, 10);
	memcpy(charStCode, strStCode.GetBuffer(10), 10);

	//--------------ST头----------------
	pSendFrame->m_buf[0] = 'S';
	pSendFrame->m_buf[1] = 'T';
	pSendFrame->m_buf[2] = ' ';
	//---------------站码---------------
	pSendFrame->m_buf[3] = charStCode[0];
	pSendFrame->m_buf[4] = charStCode[1];
	pSendFrame->m_buf[5] = charStCode[2];
	pSendFrame->m_buf[6] = charStCode[3];
	pSendFrame->m_buf[7] = charStCode[4];
	pSendFrame->m_buf[8] = charStCode[5];
	pSendFrame->m_buf[9] = charStCode[6];
	pSendFrame->m_buf[10] = charStCode[7];
	pSendFrame->m_buf[11] = charStCode[8];
	pSendFrame->m_buf[12] = charStCode[9];
	pSendFrame->m_buf[13] = ' ';

	pSendFrame->m_buf[14] = 'R';
	pSendFrame->m_buf[15] = 'C';
	pSendFrame->m_buf[16] = ' ';
	pSendFrame->m_buf[17] = '?';
	pSendFrame->m_buf[18] = ' ';
	pSendFrame->m_buf[19] = 'a';
	pSendFrame->m_buf[20] = 'a';
	pSendFrame->m_buf[21] = 'N';
	pSendFrame->m_buf[22] = 'N';
	pSendFrame->m_bufLen = 23;	

	m_pCurrCallOP = scsw_workconditionRainCntVar;
	SendRtuLinkFrame(pSendFrame->m_buf, pSendFrame->m_bufLen, pSendFrame->m_CommInterface);

}

void CSysDataTrans_Hydr::SCSW_hydrdata_EN_SS(CHydrSendFrame *pSendFrame)
{
	if(NULL == pSendFrame)
		return;

	//-----------站码---------------------
	CString strStCode = pSendFrame->m_strStCode;
	char charStCode[11];
	memset(charStCode, 0, 10);
	memcpy(charStCode, strStCode.GetBuffer(10), 10);

	//--------------ST头----------------
	pSendFrame->m_buf[0] = 'S';
	pSendFrame->m_buf[1] = 'T';
	pSendFrame->m_buf[2] = ' ';
	//---------------站码---------------
	pSendFrame->m_buf[3] = charStCode[0];
	pSendFrame->m_buf[4] = charStCode[1];
	pSendFrame->m_buf[5] = charStCode[2];
	pSendFrame->m_buf[6] = charStCode[3];
	pSendFrame->m_buf[7] = charStCode[4];
	pSendFrame->m_buf[8] = charStCode[5];
	pSendFrame->m_buf[9] = charStCode[6];
	pSendFrame->m_buf[10] = charStCode[7];
	pSendFrame->m_buf[11] = charStCode[8];
	pSendFrame->m_buf[12] = charStCode[9];
	pSendFrame->m_buf[13] = ' ';

	pSendFrame->m_buf[14] = 'S';
	pSendFrame->m_buf[15] = 'S';
	pSendFrame->m_buf[16] = ' ';
	pSendFrame->m_buf[17] = '?';
	pSendFrame->m_buf[18] = ' ';
	pSendFrame->m_buf[19] = 'a';
	pSendFrame->m_buf[20] = 'a';
	pSendFrame->m_buf[21] = 'N';
	pSendFrame->m_buf[22] = 'N';
	pSendFrame->m_bufLen = 23;	

	m_pCurrCallOP = scsw_workconditionSensorInstallStatus;
	SendRtuLinkFrame(pSendFrame->m_buf, pSendFrame->m_bufLen, pSendFrame->m_CommInterface);

}

void CSysDataTrans_Hydr::SCSW_hydrdata_EN_ML(CHydrSendFrame *pSendFrame)
{
	if(NULL == pSendFrame)
		return;

	//-----------站码---------------------
	CString strStCode = pSendFrame->m_strStCode;
	char charStCode[11];
	memset(charStCode, 0, 10);
	memcpy(charStCode, strStCode.GetBuffer(10), 10);

	//--------------ST头----------------
	pSendFrame->m_buf[0] = 'S';
	pSendFrame->m_buf[1] = 'T';
	pSendFrame->m_buf[2] = ' ';
	//---------------站码---------------
	pSendFrame->m_buf[3] = charStCode[0];
	pSendFrame->m_buf[4] = charStCode[1];
	pSendFrame->m_buf[5] = charStCode[2];
	pSendFrame->m_buf[6] = charStCode[3];
	pSendFrame->m_buf[7] = charStCode[4];
	pSendFrame->m_buf[8] = charStCode[5];
	pSendFrame->m_buf[9] = charStCode[6];
	pSendFrame->m_buf[10] = charStCode[7];
	pSendFrame->m_buf[11] = charStCode[8];
	pSendFrame->m_buf[12] = charStCode[9];
	pSendFrame->m_buf[13] = ' ';

	pSendFrame->m_buf[14] = 'M';
	pSendFrame->m_buf[15] = 'L';
	pSendFrame->m_buf[16] = ' ';
	pSendFrame->m_buf[17] = '?';
	pSendFrame->m_buf[18] = ' ';
	pSendFrame->m_buf[19] = 'a';
	pSendFrame->m_buf[20] = 'a';
	pSendFrame->m_buf[21] = 'N';
	pSendFrame->m_buf[22] = 'N';
	pSendFrame->m_bufLen = 23;	

	m_pCurrCallOP = scsw_workconditionMemoryPtr;
	SendRtuLinkFrame(pSendFrame->m_buf, pSendFrame->m_bufLen, pSendFrame->m_CommInterface);

}

void CSysDataTrans_Hydr::SCSW_hydrdata_EN_MF(CHydrSendFrame *pSendFrame)
{
	if(NULL == pSendFrame)
		return;

	//-----------站码---------------------
	CString strStCode = pSendFrame->m_strStCode;
	char charStCode[11];
	memset(charStCode, 0, 10);
	memcpy(charStCode, strStCode.GetBuffer(10), 10);

	//--------------ST头----------------
	pSendFrame->m_buf[0] = 'S';
	pSendFrame->m_buf[1] = 'T';
	pSendFrame->m_buf[2] = ' ';
	//---------------站码---------------
	pSendFrame->m_buf[3] = charStCode[0];
	pSendFrame->m_buf[4] = charStCode[1];
	pSendFrame->m_buf[5] = charStCode[2];
	pSendFrame->m_buf[6] = charStCode[3];
	pSendFrame->m_buf[7] = charStCode[4];
	pSendFrame->m_buf[8] = charStCode[5];
	pSendFrame->m_buf[9] = charStCode[6];
	pSendFrame->m_buf[10] = charStCode[7];
	pSendFrame->m_buf[11] = charStCode[8];
	pSendFrame->m_buf[12] = charStCode[9];
	pSendFrame->m_buf[13] = ' ';

	pSendFrame->m_buf[14] = 'M';
	pSendFrame->m_buf[15] = 'F';
	pSendFrame->m_buf[16] = ' ';
	pSendFrame->m_buf[17] = '?';
	pSendFrame->m_buf[18] = ' ';
	pSendFrame->m_buf[19] = 'a';
	pSendFrame->m_buf[20] = 'a';
	pSendFrame->m_buf[21] = 'N';
	pSendFrame->m_buf[22] = 'N';
	pSendFrame->m_bufLen = 23;	

	m_pCurrCallOP = scsw_workconditionMemoryMargin;
	SendRtuLinkFrame(pSendFrame->m_buf, pSendFrame->m_bufLen, pSendFrame->m_CommInterface);

}

void CSysDataTrans_Hydr::SCSW_hydrdata_EN_BV(CHydrSendFrame *pSendFrame)
{
	if(NULL == pSendFrame)
		return;

	//-----------站码---------------------
	CString strStCode = pSendFrame->m_strStCode;
	char charStCode[11];
	memset(charStCode, 0, 10);
	memcpy(charStCode, strStCode.GetBuffer(10), 10);

	//--------------ST头----------------
	pSendFrame->m_buf[0] = 'S';
	pSendFrame->m_buf[1] = 'T';
	pSendFrame->m_buf[2] = ' ';
	//---------------站码---------------
	pSendFrame->m_buf[3] = charStCode[0];
	pSendFrame->m_buf[4] = charStCode[1];
	pSendFrame->m_buf[5] = charStCode[2];
	pSendFrame->m_buf[6] = charStCode[3];
	pSendFrame->m_buf[7] = charStCode[4];
	pSendFrame->m_buf[8] = charStCode[5];
	pSendFrame->m_buf[9] = charStCode[6];
	pSendFrame->m_buf[10] = charStCode[7];
	pSendFrame->m_buf[11] = charStCode[8];
	pSendFrame->m_buf[12] = charStCode[9];
	pSendFrame->m_buf[13] = ' ';

	pSendFrame->m_buf[14] = 'B';
	pSendFrame->m_buf[15] = 'V';
	pSendFrame->m_buf[16] = ' ';
	pSendFrame->m_buf[17] = '?';
	pSendFrame->m_buf[18] = ' ';
	pSendFrame->m_buf[19] = 'a';
	pSendFrame->m_buf[20] = 'a';
	pSendFrame->m_buf[21] = 'N';
	pSendFrame->m_buf[22] = 'N';
	pSendFrame->m_bufLen = 23;	

	m_pCurrCallOP = scsw_workconditionStorageVoltage;
	SendRtuLinkFrame(pSendFrame->m_buf, pSendFrame->m_bufLen, pSendFrame->m_CommInterface);

}

void CSysDataTrans_Hydr::SCSW_hydrdata_EN_SV(CHydrSendFrame *pSendFrame)
{
	if(NULL == pSendFrame)
		return;

	//-----------站码---------------------
	CString strStCode = pSendFrame->m_strStCode;
	char charStCode[11];
	memset(charStCode, 0, 10);
	memcpy(charStCode, strStCode.GetBuffer(10), 10);

	//--------------ST头----------------
	pSendFrame->m_buf[0] = 'S';
	pSendFrame->m_buf[1] = 'T';
	pSendFrame->m_buf[2] = ' ';
	//---------------站码---------------
	pSendFrame->m_buf[3] = charStCode[0];
	pSendFrame->m_buf[4] = charStCode[1];
	pSendFrame->m_buf[5] = charStCode[2];
	pSendFrame->m_buf[6] = charStCode[3];
	pSendFrame->m_buf[7] = charStCode[4];
	pSendFrame->m_buf[8] = charStCode[5];
	pSendFrame->m_buf[9] = charStCode[6];
	pSendFrame->m_buf[10] = charStCode[7];
	pSendFrame->m_buf[11] = charStCode[8];
	pSendFrame->m_buf[12] = charStCode[9];
	pSendFrame->m_buf[13] = ' ';

	pSendFrame->m_buf[14] = 'S';
	pSendFrame->m_buf[15] = 'V';
	pSendFrame->m_buf[16] = ' ';
	pSendFrame->m_buf[17] = '?';
	pSendFrame->m_buf[18] = ' ';
	pSendFrame->m_buf[19] = 'a';
	pSendFrame->m_buf[20] = 'a';
	pSendFrame->m_buf[21] = 'N';
	pSendFrame->m_buf[22] = 'N';
	pSendFrame->m_bufLen = 23;	

	m_pCurrCallOP = scsw_workconditionSolarCellVoltage;
	SendRtuLinkFrame(pSendFrame->m_buf, pSendFrame->m_bufLen, pSendFrame->m_CommInterface);
}

void CSysDataTrans_Hydr::SCSW_hydrdata_EN_ET(CHydrSendFrame *pSendFrame)
{
	if(NULL == pSendFrame)
		return;

	//-----------站码---------------------
	CString strStCode = pSendFrame->m_strStCode;
	char charStCode[11];
	memset(charStCode, 0, 10);
	memcpy(charStCode, strStCode.GetBuffer(10), 10);

	//--------------ST头----------------
	pSendFrame->m_buf[0] = 'S';
	pSendFrame->m_buf[1] = 'T';
	pSendFrame->m_buf[2] = ' ';
	//---------------站码---------------
	pSendFrame->m_buf[3] = charStCode[0];
	pSendFrame->m_buf[4] = charStCode[1];
	pSendFrame->m_buf[5] = charStCode[2];
	pSendFrame->m_buf[6] = charStCode[3];
	pSendFrame->m_buf[7] = charStCode[4];
	pSendFrame->m_buf[8] = charStCode[5];
	pSendFrame->m_buf[9] = charStCode[6];
	pSendFrame->m_buf[10] = charStCode[7];
	pSendFrame->m_buf[11] = charStCode[8];
	pSendFrame->m_buf[12] = charStCode[9];
	pSendFrame->m_buf[13] = ' ';

	pSendFrame->m_buf[14] = 'E';
	pSendFrame->m_buf[15] = 'T';
	pSendFrame->m_buf[16] = ' ';
	pSendFrame->m_buf[17] = '?';
	pSendFrame->m_buf[18] = ' ';
	pSendFrame->m_buf[19] = 'a';
	pSendFrame->m_buf[20] = 'a';
	pSendFrame->m_buf[21] = 'N';
	pSendFrame->m_buf[22] = 'N';
	pSendFrame->m_bufLen = 23;	

	m_pCurrCallOP = scsw_workconditionEvoirTemperature;
	SendRtuLinkFrame(pSendFrame->m_buf, pSendFrame->m_bufLen, pSendFrame->m_CommInterface);
}