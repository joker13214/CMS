#pragma once
#include <afxmt.h>
#include "UdfGlobalFUNC.h"
#include "WaitTimer.h"
#include "UdfLogSystem.h"

class CRtuStation;
class CCommThreadInfo;
class COneData_INFO;
class CRtuPropertyDlg;

class CSysDataTrans_WriteUpdate
{
public:
	CSysDataTrans_WriteUpdate();
	virtual ~CSysDataTrans_WriteUpdate();

public:
	void Dispatcher(BYTE* buffer,int bufferLen,BYTE CmInterface=Serial_CommInterface, CString FromIP="",int FromPort=0);                      //报文处理，默认串口
	void Dispatcher(CWriteFrame* pWriteFrame);
public:
	void SetRtuStation(CRtuStation* pSub)            {m_pSubStation = pSub;}					//设置对应的遥测终端对象

public:
	void AddRecvLog(BYTE* buffer,int len, CWriteFrame* hydrFrame=NULL);      //添加接收水文资源日志信息，主要是向站属性窗口添加日志信息，主要是向CRtuStation对象添加日志信息
	void AddSendLog(BYTE* buffer,int len, CString strStCode);										//添加下发报文日志信息，主要是向站的属性窗口添加日志信息,主要是向CRtuStation对象添加日志信息
	void UpdateRtuCommState(CRtuStation* pSub);
	void UpdateRtuCommInfo(CRtuStation* pSub);

public:
	int  SendRtuLinkFrame(BYTE* sBuf,int len,BYTE CmInterface = Serial_CommInterface,PNET_INFO pToNetInfo = NULL);//向遥测终端发送报文
	CCriticalSection m_cs_SendData;

private:
	void SCSW_WriteUpdate_A0(CWriteFrame* pHydr, CString &print, int nPack);
	void SCSW_WriteUpdate_A1(CWriteFrame* pHydr, CString &print, int nPack);
	void SCSW_WriteUpdate_A2(CWriteFrame* pHydr, CString &print, int nPack);
	void SCSW_WriteUpdate_A3(CWriteFrame* pHydr, CString &print, int nPack);
	void SCSW_WriteUpdate_B0(CWriteFrame* pHydr, CString &print, int nPack);
	void SCSW_WriteUpdate_B1(CWriteFrame* pHydr, CString &print, int nPack);
	void SCSW_WriteUpdate_B2(CWriteFrame* pHydr, CString &print, int nPack);
	void SCSW_WriteUpdate_B3(CWriteFrame* pHydr, CString &print, int nPack);

	//------------------------查询帧-------------------------------
public:
	void SCSW_WriteUpdate_EN_A0(CWriteFrame *pSendFrame);//水位数据查询帧

private:
	CString BufferToString(BYTE* buffer,int Len);
	bool IsVaildRepeat(CWriteFrame*pSendFrame, CWriteFrame* pRecvFrame);

	//更新RTU的属性界面
private:
	void AddLogMessage(CRtuStation* pRTU,CString strLogMessage, _Log_level_type leveType =Normal_Log_Message, PSYSTEMTIME pTime = NULL);   //添加日志信息到窗口中显示
	void  AddDataMessage(CRtuStation* pRTU,CString strDataMessage,PSYSTEMTIME pTime=NULL);	
	void UpdateAttribFrame(CRtuStation* pRTU, int attrType, int btnCmd);
	void RenewCallOperation(CRtuStation* pRTU, int attrType);
	CRtuPropertyDlg* GetRtuPropertyDlg();

private:
	CRtuStation *m_pSubStation;
	int m_pCurrCallOP; //当前查询操作
};