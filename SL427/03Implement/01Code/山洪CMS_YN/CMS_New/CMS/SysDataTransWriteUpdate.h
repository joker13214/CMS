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
	void Dispatcher(BYTE* buffer,int bufferLen,BYTE CmInterface=Serial_CommInterface, CString FromIP="",int FromPort=0);                      //���Ĵ���Ĭ�ϴ���
	void Dispatcher(CWriteFrame* pWriteFrame);
public:
	void SetRtuStation(CRtuStation* pSub)            {m_pSubStation = pSub;}					//���ö�Ӧ��ң���ն˶���

public:
	void AddRecvLog(BYTE* buffer,int len, CWriteFrame* hydrFrame=NULL);      //��ӽ���ˮ����Դ��־��Ϣ����Ҫ����վ���Դ��������־��Ϣ����Ҫ����CRtuStation���������־��Ϣ
	void AddSendLog(BYTE* buffer,int len, CString strStCode);										//����·�������־��Ϣ����Ҫ����վ�����Դ��������־��Ϣ,��Ҫ����CRtuStation���������־��Ϣ
	void UpdateRtuCommState(CRtuStation* pSub);
	void UpdateRtuCommInfo(CRtuStation* pSub);

public:
	int  SendRtuLinkFrame(BYTE* sBuf,int len,BYTE CmInterface = Serial_CommInterface,PNET_INFO pToNetInfo = NULL);//��ң���ն˷��ͱ���
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

	//------------------------��ѯ֡-------------------------------
public:
	void SCSW_WriteUpdate_EN_A0(CWriteFrame *pSendFrame);//ˮλ���ݲ�ѯ֡

private:
	CString BufferToString(BYTE* buffer,int Len);
	bool IsVaildRepeat(CWriteFrame*pSendFrame, CWriteFrame* pRecvFrame);

	//����RTU�����Խ���
private:
	void AddLogMessage(CRtuStation* pRTU,CString strLogMessage, _Log_level_type leveType =Normal_Log_Message, PSYSTEMTIME pTime = NULL);   //�����־��Ϣ����������ʾ
	void  AddDataMessage(CRtuStation* pRTU,CString strDataMessage,PSYSTEMTIME pTime=NULL);	
	void UpdateAttribFrame(CRtuStation* pRTU, int attrType, int btnCmd);
	void RenewCallOperation(CRtuStation* pRTU, int attrType);
	CRtuPropertyDlg* GetRtuPropertyDlg();

private:
	CRtuStation *m_pSubStation;
	int m_pCurrCallOP; //��ǰ��ѯ����
};