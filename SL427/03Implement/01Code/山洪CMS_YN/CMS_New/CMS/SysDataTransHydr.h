#pragma once
#include <afxmt.h>
#include "UdfGlobalFUNC.h"
#include "WaitTimer.h"
#include "UdfLogSystem.h"

class CRtuStation;
class CCommThreadInfo;
class COneData_INFO;
class CRtuPropertyDlg;

class CSysDataTrans_Hydr
{
public:
	CSysDataTrans_Hydr();
	virtual ~CSysDataTrans_Hydr();

public:
	void Dispatcher(BYTE* buffer,int bufferLen,BYTE CmInterface=Serial_CommInterface, CString FromIP="",int FromPort=0);                      //���Ĵ���Ĭ�ϴ���
	void Dispatcher(CHydrFrameFormat* phydrFrame);
public:
	void    SetRtuStation(CRtuStation* pSub)            {m_pSubStation = pSub;}					//���ö�Ӧ��ң���ն˶���

public:
	void AddRecvLog(BYTE* buffer,int len, CHydrFrameFormat* hydrFrame=NULL);      //��ӽ���ˮ����Դ��־��Ϣ����Ҫ����վ���Դ��������־��Ϣ����Ҫ����CRtuStation���������־��Ϣ
	void AddSendLog(BYTE* buffer,int len, CString strStCode);										//����·�������־��Ϣ����Ҫ����վ�����Դ��������־��Ϣ,��Ҫ����CRtuStation���������־��Ϣ
	void UpdateRtuCommState(CRtuStation* pSub);
	void UpdateRtuCommInfo(CRtuStation* pSub);

public:
	int  SendRtuLinkFrame(BYTE* sBuf,int len,BYTE CmInterface = Serial_CommInterface,PNET_INFO pToNetInfo = NULL);//��ң���ն˷��ͱ���
	CCriticalSection m_cs_SendData;

private:
	void SCSW_hydrdata_Z(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print);
	void SCSW_hydrdata_PN(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print);
	void SCSW_hydrdata_P(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print);
	void SCSW_hydrdata_M10(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print);
	void SCSW_hydrdata_H(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print);
	void SCSW_hydrdata_C(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print);
	void SCSW_hydrdata_ED(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print);
	void SCSW_hydrdata_ZR(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print);
	void SCSW_hydrdata_RC(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print);
	void SCSW_hydrdata_ML(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print);
	void SCSW_hydrdata_MF(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print);
	void SCSW_hydrdata_BV(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print);
	void SCSW_hydrdata_SV(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print);
	void SCSW_hydrdata_ET(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print);
	void SCSW_hydrdata_SI(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print);
	void SCSW_hydrdata_PI(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print);
	void SCSW_hydrdata_RD(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print);
	void SCSW_hydrdata_ZJ(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print);
	void SCSW_hydrdata_ZT(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print);
	void SCSW_hydrdata_ZF(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print);
	void SCSW_hydrdata_Zx(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print);
	void SCSW_hydrdata_DZ(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print);
	void SCSW_hydrdata_TT(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print);
	void SCSW_hydrdata_TM(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print);
	void SCSW_hydrdata_TN(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print);
	void SCSW_hydrdata_TB(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print);
	void SCSW_hydrdata_TE(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print);
	void SCSW_hydrdata_RR(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print);
	void SCSW_hydrdata_DV(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print);
	void SCSW_hydrdata_DI(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print);
	void SCSW_hydrdata_DC(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print);
	void SCSW_hydrdata_CN(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print);
	void SCSW_hydrdata_CP(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print);
	void SCSW_hydrdata_GSM(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print);
	void SCSW_hydrdata_BD(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print);
	void SCSW_hydrdata_PSTN(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print);
	void SCSW_hydrdata_IT(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print);
	void SCSW_hydrdata_PT(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print);
	void SCSW_hydrdata_TC(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print);
	void SCSW_hydrdata_PW(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print);
	void SCSW_hydrdata_ER(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print);
	void SCSW_hydrdata_RF(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo,  CString &print);
	void SCSW_hydrdata_HOST(CHydrFrameFormat* pHydr, SCSW_MsgInfo *pInfo, CString &print);

	//------------------------��ѯ֡-------------------------------
public:
	void SCSW_hydrdata_EN_Z(CHydrSendFrame *pSendFrame);//ˮλ���ݲ�ѯ֡
	void SCSW_hydrdata_EN_P(CHydrSendFrame *pSendFrame);//�������ݲ�ѯ֡
	void SCSW_hydrdata_EN_P_MH(CHydrSendFrame *pSendFrame);//��������ģ����ѯ
	void SCSW_hydrdata_EN_M(CHydrSendFrame *pSendFrame);//���������ѯ֡
	void SCSW_hydrdata_EN_H(CHydrSendFrame *pSendFrame);//����ˮ˲ʱ�����ѯ֡
	void SCSW_hydrdata_EN_C(CHydrSendFrame *pSendFrame);//����ˮ˲ʱ�¶Ȳ�ѯ֡
	void SCSW_hydrdata_EN_ED(CHydrSendFrame *pSendFrame);//�������ݲ�ѯ֡

	void SCSW_hydrdata_EN_PI(CHydrSendFrame *pSendFrame);
	void SCSW_hydrdata_EN_RD(CHydrSendFrame *pSendFrame);
	void SCSW_hydrdata_EN_ZJ(CHydrSendFrame *pSendFrame);
	void SCSW_hydrdata_EN_ZT(CHydrSendFrame *pSendFrame);
	void SCSW_hydrdata_EN_ZF(CHydrSendFrame *pSendFrame);
	void SCSW_hydrdata_EN_Zx(CHydrSendFrame *pSendFrame);
	void SCSW_hydrdata_EN_DZx(CHydrSendFrame *pSendFrame);

	void SCSW_hydrdata_EN_GSM(CHydrSendFrame *pSendFrame);
	void SCSW_hydrdata_EN_BD(CHydrSendFrame *pSendFrame);
	void SCSW_hydrdata_EN_IP(CHydrSendFrame *pSendFrame);
	void SCSW_hydrdata_EN_IP2(CHydrSendFrame *pSendFrame);
	void SCSW_hydrdata_EN_PORT(CHydrSendFrame *pSendFrame);

	void SCSW_hydrdata_EN_SET_GSM(CHydrSendFrame *pSendFrame);
	void SCSW_hydrdata_EN_SET_BD(CHydrSendFrame *pSendFrame);
	void SCSW_hydrdata_EN_SET_IP(CHydrSendFrame *pSendFrame);
	void SCSW_hydrdata_EN_SET_IP2(CHydrSendFrame *pSendFrame);
	void SCSW_hydrdata_EN_SET_PORT(CHydrSendFrame *pSendFrame);

	void SCSW_hydrdata_EN_DEL_GSM(CHydrSendFrame *pSendFrame);
	void SCSW_hydrdata_EN_DEL_BD(CHydrSendFrame *pSendFrame);
	void SCSW_hydrdata_EN_DEL_IP(CHydrSendFrame *pSendFrame);
	void SCSW_hydrdata_EN_DEL_IP2(CHydrSendFrame *pSendFrame);
	void SCSW_hydrdata_EN_DEL_PORT(CHydrSendFrame *pSendFrame);

	void SCSW_hydrdata_EN_SET_PI(CHydrSendFrame *pSendFrame);
	void SCSW_hydrdata_EN_SET_RD(CHydrSendFrame *pSendFrame);
	void SCSW_hydrdata_EN_SET_ZJ(CHydrSendFrame *pSendFrame);
	void SCSW_hydrdata_EN_SET_ZT(CHydrSendFrame *pSendFrame);
	void SCSW_hydrdata_EN_SET_ZF(CHydrSendFrame *pSendFrame);
	void SCSW_hydrdata_EN_SET_Zx(CHydrSendFrame *pSendFrame);
	void SCSW_hydrdata_EN_SET_DZx(CHydrSendFrame *pSendFrame);
	void SCSW_hydrdata_EN_SET_TT(CHydrSendFrame *pSendFrame);
	void SCSW_hydrdata_EN_SET_HOST(CHydrSendFrame *pSendFrame);

	void SCSW_hydrdata_EN_ZR(CHydrSendFrame *pSendFrame);
	void SCSW_hydrdata_EN_RC(CHydrSendFrame *pSendFrame);
	void SCSW_hydrdata_EN_SS(CHydrSendFrame *pSendFrame);
	void SCSW_hydrdata_EN_ML(CHydrSendFrame *pSendFrame);
	void SCSW_hydrdata_EN_MF(CHydrSendFrame *pSendFrame);
	void SCSW_hydrdata_EN_BV(CHydrSendFrame *pSendFrame);
	void SCSW_hydrdata_EN_SV(CHydrSendFrame *pSendFrame);
	void SCSW_hydrdata_EN_ET(CHydrSendFrame *pSendFrame);

public:
	void SCSW_hydrdata_EN_OK(CHydrSendFrame *pSendFrame); //OKȷ��֡

private:
	CString BufferToString(BYTE* buffer,int Len);

	//����RTU�����Խ���
private:
	void AddLogMessage(CRtuStation* pRTU,CString strLogMessage, _Log_level_type leveType =Normal_Log_Message, PSYSTEMTIME pTime = NULL);   //�����־��Ϣ����������ʾ
	void  AddDataMessage(CRtuStation* pRTU,CString strDataMessage,PSYSTEMTIME pTime=NULL);	
	void UpdateAttribFrame(CRtuStation* pRTU, int attrType);
	void RenewCallOperation(CRtuStation* pRTU, int attrType);
	CRtuPropertyDlg* GetRtuPropertyDlg();

private:
	CRtuStation *m_pSubStation;
	int m_pCurrCallOP; //��ǰ��ѯ����
};