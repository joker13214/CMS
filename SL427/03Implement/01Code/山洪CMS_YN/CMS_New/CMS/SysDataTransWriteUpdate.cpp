#include "stdafx.h"
#include "SysDataTransHydr.h"
#include "CMS.h"
#include "RealTimeDB.h"
#include "MainFrm.h"
#include "RtuStation.h"
#include "CommServer.h"
#include "UdfGlobalFUNC.h"

CSysDataTrans_WriteUpdate::CSysDataTrans_WriteUpdate()
{
	m_pSubStation = NULL;
	m_pCurrCallOP = scsw_no_call_operation;
}

CSysDataTrans_WriteUpdate::~CSysDataTrans_WriteUpdate()
{

}
void CSysDataTrans_WriteUpdate::Dispatcher(BYTE* buffer,int bufferLen, BYTE CmInterface, CString FromIP,int FromPort)                      //报文处理，默认串口
{

}

void CSysDataTrans_WriteUpdate::Dispatcher(CWriteFrame* pWriteFrame)
{
	if (NULL == pWriteFrame)  
		return;

	CString print = "";
	print.Format("%s号遥测站 %d月%d日%d时%d分 收到烧写反馈信息如下：\r\n\n",
		pWriteFrame->m_nDevAdddr,
		pWriteFrame->m_DataTimeTT.month,
		pWriteFrame->m_DataTimeTT.day,
		pWriteFrame->m_DataTimeTT.hour,
		pWriteFrame->m_DataTimeTT.min);

	bool bFlag = false;
	int nPackNo = 0;
	if (pWriteFrame->bHasContentFrame)
	{
		nPackNo = pWriteFrame->m_nPackCnt;
	}
	
	switch(pWriteFrame->m_nCmdCode)
	{
	case 0xA0:
		SCSW_WriteUpdate_A0(pWriteFrame, print, nPackNo);
		bFlag = true;
		break;           
	case 0xA1:
		SCSW_WriteUpdate_A1(pWriteFrame, print, nPackNo);
		bFlag = true;
		break;            
	case 0xA2:
		SCSW_WriteUpdate_A2(pWriteFrame, print, nPackNo);
		bFlag = true;
		break;
	case 0xA3:
		SCSW_WriteUpdate_A3(pWriteFrame, print, nPackNo);
		bFlag = true;
		break;
	case 0xB0:
		SCSW_WriteUpdate_B0(pWriteFrame, print, nPackNo);
		bFlag = true;
		break;
	case 0xB1:
		SCSW_WriteUpdate_B1(pWriteFrame, print, nPackNo);
		bFlag = true;
		break;
	case 0xB2:
		SCSW_WriteUpdate_B2(pWriteFrame, print, nPackNo);
		bFlag = true;
		break;
	case 0xB3:
		SCSW_WriteUpdate_B3(pWriteFrame, print, nPackNo);
		bFlag = true;
		break;

	}



	if (NULL != pWriteFrame->m_pStation)
	{
		AddLogMessage(pWriteFrame->m_pStation, print, Normal_Log_Message, (PSYSTEMTIME)NULL);
		return;
	}

}

//将报文转换为字符串信息
CString CSysDataTrans_WriteUpdate::BufferToString(BYTE* buffer,int Len)
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

bool CSysDataTrans_WriteUpdate::IsVaildRepeat(CWriteFrame* pSendFrame, CWriteFrame* pRecvFrame)
{
	if (NULL==pSendFrame || NULL==pRecvFrame)
	{
		return false;
	}

	if (pSendFrame->m_nPackCnt == pRecvFrame->m_nPackCnt)
	{
		return true;
	}


	return false;
}


void CSysDataTrans_WriteUpdate::AddRecvLog(BYTE* buffer,int len, CWriteFrame* pWriteFrame)
{
	if (!buffer  || len <=0) return;

	CString strBuffer = BufferToString(buffer,len);
	CString strLogBuffer ="";

	CWriteFrame Pff;
	if (pWriteFrame == NULL)
	{
		if (!CUdfGlobalFUNC::WRITE_frame_to_APPFRAME(buffer, len, &Pff))  
			return;

		pWriteFrame = &Pff;
	}

	if (pWriteFrame != NULL)
	{
		if (pWriteFrame->m_pStation == NULL)
		{
			CCMSApp* pApp = (CCMSApp*)AfxGetApp();
			CRealTimeDB* pRealDb = pApp->m_pReadDB;
			if (!pRealDb)	return ;

			CRtuStation* pSub =  pRealDb->SearchRtuStation(pWriteFrame->m_nDevAdddr, SCSW_008_Protocol);
			pWriteFrame->m_pStation = pSub;
		}

		CString strBuffer = CUdfGlobalFUNC::ConvertBytesToString(buffer,len);
		CString strLogBuffer ="";

		strLogBuffer.Format("收,%-2d月%-2d日%-2d点%-2d分,DIR=%s,Len=%-3d,报文=%s",
			pWriteFrame->m_DataTimeTT.month,
			pWriteFrame->m_DataTimeTT.day,
			pWriteFrame->m_DataTimeTT.hour,
			pWriteFrame->m_DataTimeTT.min,
			_T("上行"),
			len,
			strBuffer);

		AddLogMessage(pWriteFrame->m_pStation ,strLogBuffer, Normal_Log_Message);
	}
}

int CSysDataTrans_WriteUpdate::SendRtuLinkFrame(BYTE* sBuf,int len,BYTE CmInterface, PNET_INFO pToNetInfo)
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
void CSysDataTrans_WriteUpdate::AddSendLog(BYTE* buffer,int len,  CString strStCode)
{  
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	CRealTimeDB* pRealDb = pApp->m_pReadDB;
	if (!pRealDb)			 return ;
	if (!buffer  || len <=0) return;

	CRtuStation* pSub =  pRealDb->SearchRtuStation(strStCode, SCSW_008_Protocol);

	CString strBuffer = CUdfGlobalFUNC::ConvertBytesToString(buffer,len);

	if (pSub != NULL)
	{
		CString strLogBuffer= "";
		strLogBuffer.Format("[烧写升级程序]发,DIR=%s,Len=%-3d,报文=%s",_T("下行"), len, strBuffer);
		AddLogMessage(pSub,strLogBuffer,Normal_Log_Message);
	}
}

void CSysDataTrans_WriteUpdate::AddLogMessage(CRtuStation* pRTU,CString strLogMessage, _Log_level_type leveType, PSYSTEMTIME pTime)
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
void CSysDataTrans_WriteUpdate::AddDataMessage(CRtuStation* pRTU,CString strDataMessage,PSYSTEMTIME pTime/* =NULL */)
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

void CSysDataTrans_WriteUpdate::UpdateAttribFrame(CRtuStation* pRTU, int attrType, int btnCmd)
{
	CRtuPropertyDlg* pWnd = GetRtuPropertyDlg();
	if (pWnd == NULL  ||
		pWnd->GetRtuStation() == NULL ||
		pWnd->GetRtuStation() != pRTU)
		return;


	CRtuAttrWriteUpdate *pTab = pWnd->GetTab_AttrWriteUpdate();
	if ( pTab != NULL || pTab->GetSafeHwnd())
	{
		pTab->UpdateDlg(attrType, btnCmd, TRUE, pRTU->GetInterface());
		pTab->Update();
	}
}

void CSysDataTrans_WriteUpdate::RenewCallOperation(CRtuStation* pRTU, int attrType)
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
CRtuPropertyDlg* CSysDataTrans_WriteUpdate::GetRtuPropertyDlg()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	if (pMainFrame)
		return pMainFrame->GetRtuPropertyDlg();
	return NULL;
}

//更新遥测终端的通信状态
//重点:更新列表信息和添加通信状态改变日志信息
void CSysDataTrans_WriteUpdate::UpdateRtuCommState(CRtuStation* pSub)
{
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	CMainFrame* pMainFrame = (CMainFrame*)pApp->m_pMainWnd;

	if (pMainFrame && pMainFrame->m_pRightView && pMainFrame->m_pRightView->GetSafeHwnd())
		pMainFrame->m_pRightView->UpdateCommState(pSub);
}

void CSysDataTrans_WriteUpdate::UpdateRtuCommInfo(CRtuStation* pSub)
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
void CSysDataTrans_WriteUpdate::SCSW_WriteUpdate_A0(CWriteFrame* pWriteFrame,  CString &print, int nPack)
{
	if (pWriteFrame == NULL)
	{
		return;
	}

	CString result = "";
	result.Format(_T("接收命令码为：A0，包计数为%d的代码烧写"), pWriteFrame->m_nPackCnt);

	
	if (pWriteFrame->m_pStation && m_pSubStation)
	{
		if (m_pSubStation->bIsWriteUpdateStart)
		{
			pWriteFrame->m_pStation->m_it_map = pWriteFrame->m_pStation->m_WriteUpdateMap.find(nPack);
			if (pWriteFrame->m_pStation->m_it_map != pWriteFrame->m_pStation->m_WriteUpdateMap.end())
			{
				pWriteFrame->m_pStation->bIteratorVaild = true;
				print = print + result + _T("成功") + "\r\n";
				UpdateAttribFrame(pWriteFrame->m_pStation, writeUpdate_A0, m_pSubStation->m_CurrentWriteUpdateOp);
			}
			else
			{
				pWriteFrame->m_pStation->bIteratorVaild = false;
				print = print + result + _T("未找到匹配计数包的升级报文") + "\r\n";
				UpdateAttribFrame(pWriteFrame->m_pStation, writeUpdate_A0, m_pSubStation->m_CurrentWriteUpdateOp);
			}
		}
		else
		{
			print = _T("警告：请先启动升级");
		}
		
	}

}

void CSysDataTrans_WriteUpdate::SCSW_WriteUpdate_A1(CWriteFrame* pWriteFrame,  CString &print, int nPack)
{
	if (pWriteFrame == NULL)
	{
		return;
	}

	CString result = "";
	

	if (pWriteFrame->m_pStation && m_pSubStation)
	{
		pWriteFrame->m_pStation->m_it_map = pWriteFrame->m_pStation->m_WriteUpdateMap.find(nPack);
		if (pWriteFrame->m_pStation->m_it_map != pWriteFrame->m_pStation->m_WriteUpdateMap.end())
		{
			pWriteFrame->m_pStation->bIteratorVaild = true;
			result.Format(_T("接收命令码为：A1，跳包发送：发送包计数为%d，接收包计数为%d"), pWriteFrame->m_pStation->m_pSendWriteFrame.m_nPackCnt, pWriteFrame->m_nPackCnt);
			print = print + result + "\r\n";
			UpdateAttribFrame(pWriteFrame->m_pStation, writeUpdate_A1, m_pSubStation->m_CurrentWriteUpdateOp);
		}
		else
		{
			pWriteFrame->m_pStation->bIteratorVaild = false;
			print = print + result + _T("未找到匹配计数包的升级报文") + "\r\n";
			UpdateAttribFrame(pWriteFrame->m_pStation, writeUpdate_A1, m_pSubStation->m_CurrentWriteUpdateOp);
		}
	}
}

void CSysDataTrans_WriteUpdate::SCSW_WriteUpdate_A2(CWriteFrame* pWriteFrame,  CString &print, int nPack)
{
	if (pWriteFrame == NULL)
	{
		return;
	}

	CString result = "";


	if (pWriteFrame->m_pStation && m_pSubStation)
	{
		pWriteFrame->m_pStation->m_it_map = pWriteFrame->m_pStation->m_WriteUpdateMap.find(nPack);
		if (pWriteFrame->m_pStation->m_it_map != pWriteFrame->m_pStation->m_WriteUpdateMap.end())
		{
			pWriteFrame->m_pStation->bIteratorVaild = true;
			result.Format(_T("接收命令码为：A2，主站发送两次序号%d相同，但内容不同的错误包"), pWriteFrame->m_nPackCnt);
			print = print + result + "\r\n";
			UpdateAttribFrame(pWriteFrame->m_pStation, writeUpdate_A2, m_pSubStation->m_CurrentWriteUpdateOp);
		}
		else
		{
			pWriteFrame->m_pStation->bIteratorVaild = false;
			print = print + result + _T("未找到匹配计数包的升级报文") + "\r\n";
			UpdateAttribFrame(pWriteFrame->m_pStation, writeUpdate_A2, m_pSubStation->m_CurrentWriteUpdateOp);
		}
	}
}

void CSysDataTrans_WriteUpdate::SCSW_WriteUpdate_A3(CWriteFrame* pWriteFrame,  CString &print, int nPack)
{
	if (pWriteFrame == NULL)
	{
		return;
	}

	CString result = "";


	if (pWriteFrame->m_pStation && m_pSubStation)
	{
		pWriteFrame->m_pStation->m_it_map = pWriteFrame->m_pStation->m_WriteUpdateMap.find(nPack);
		if (pWriteFrame->m_pStation->m_it_map != pWriteFrame->m_pStation->m_WriteUpdateMap.end())
		{
			pWriteFrame->m_pStation->bIteratorVaild = true;
			result.Format(_T("接收命令码为：A3，当条报文已写入错误，主站准备重头开始发送升级报文"));
			print = print + result + "\r\n";

			UpdateAttribFrame(pWriteFrame->m_pStation, writeUpdate_A3, m_pSubStation->m_CurrentWriteUpdateOp);
		}
		else
		{
			pWriteFrame->m_pStation->bIteratorVaild = false;
			print = print + result + _T("未找到匹配计数包的升级报文") + "\r\n";
			UpdateAttribFrame(pWriteFrame->m_pStation, writeUpdate_A3, m_pSubStation->m_CurrentWriteUpdateOp);
		}
	}

}

void CSysDataTrans_WriteUpdate::SCSW_WriteUpdate_B0(CWriteFrame* pWriteFrame,  CString &print, int nPack)
{
	if (pWriteFrame == NULL)
	{
		return;
	}

	CString result = "";


	if (pWriteFrame->m_pStation && m_pSubStation)
	{

		result.Format(_T("接收命令码为：B0，开机启动后程序存储区为空"));
		print = print + result + "\r\n";

		UpdateAttribFrame(pWriteFrame->m_pStation, writeUpdate_B0, m_pSubStation->m_CurrentWriteUpdateOp);

	}
}

void CSysDataTrans_WriteUpdate::SCSW_WriteUpdate_B1(CWriteFrame* pWriteFrame,  CString &print, int nPack)
{
	if (pWriteFrame == NULL)
	{
		return;
	}

	CString result = "";


	if (pWriteFrame->m_pStation && m_pSubStation)
	{

		result.Format(_T("接收命令码为：B1，程序接收并验证正确，开始升级"));
		print = print + result + "\r\n";


		UpdateAttribFrame(pWriteFrame->m_pStation, writeUpdate_B1, m_pSubStation->m_CurrentWriteUpdateOp);

	}
}

void CSysDataTrans_WriteUpdate::SCSW_WriteUpdate_B2(CWriteFrame* pWriteFrame,  CString &print, int nPack)
{
	if (pWriteFrame == NULL)
	{
		return;
	}

	CString result = "";


	if (pWriteFrame->m_pStation && m_pSubStation)
	{

		pWriteFrame->m_pStation->bIteratorVaild = true;
		result.Format(_T("接收命令码为：B2，程序升级成功"));
		print = print + result + "\r\n";
		UpdateAttribFrame(pWriteFrame->m_pStation, writeUpdate_B2, m_pSubStation->m_CurrentWriteUpdateOp);

	}

}

void CSysDataTrans_WriteUpdate::SCSW_WriteUpdate_B3(CWriteFrame* pWriteFrame,  CString &print, int nPack)
{
	if (pWriteFrame == NULL)
	{
		return;
	}

	CString result = "";


	if (pWriteFrame->m_pStation && m_pSubStation)
	{

		pWriteFrame->m_pStation->bIteratorVaild = true;
		result.Format(_T("接收命令码为：B3，连续3次升级失败，需要去现场检修"));
		print = print + result + "\r\n";
		UpdateAttribFrame(pWriteFrame->m_pStation, writeUpdate_B3, m_pSubStation->m_CurrentWriteUpdateOp);

	}
}
