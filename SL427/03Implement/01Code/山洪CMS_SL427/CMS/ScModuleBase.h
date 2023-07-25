#pragma once
#include <map>
#include <list>
#include <afxmt.h>
#include "WaitTimer.h"
#include "SCMessageDealModule.h"
#include "TmMessageDealModule.h"
#include "RtuAttrMsgInit.h"
#include "RtuAttrTextMsg.h"

//Send Object (����ͨ�ű��Ŀ�)
typedef struct _sendObject
{
	BYTE sendbuf[512];				  //Send buffer
	int  sendLen;					  //Send buffer's length
	_sendObject()
	{
		memset(sendbuf,0,sizeof(sendbuf));
		sendLen = 0;
	}
}sendObject,*PsendObject;

class CCommThreadInfo;
class CScModuleBase
{
public:
	CScModuleBase(void);
	virtual ~CScModuleBase(void);
protected:
	int  m_nFactoryType;			//��������
	UINT32 m_nAddressLocal;			//����ͨ�ŵ�ַ

	BYTE m_RECV_BUF[1024 * 10];		//����������Ϣ����
	int  m_RECV_Len;				//�������ݳ���
protected:
	int m_nTmFactoryType;			//���ų�������
	CString m_nTmAddrLocal;			//����ͨѶ��ַ

public:
	virtual void  RecvMessage(BYTE* buf,int Len) = 0 ;           //������ܱ���
	virtual bool  SendMessage(BYTE* buf,int &len) = 0 ;          //��ȡ������Ϣ
protected:
	CWaitTimer m_CheckSendTimer;								//��ⱨ�ķ���,
	__int64    m_nSendInterval;									//���ķ��ͼ��,
	bool       m_bSendBefore;									//��֮ǰ�Ƿ��͹�������Ϣ(������Ǽ��ͨѶʱʹ��)
	std::map<UINT,CSCMessageDealModule*> m_mapSCMessageDeal;	//����ͨ�ŵ��Ĵ���ģ���б�(��Դͨ�ŵ�ַ����Mapӳ��)
	std::map<CString,CTmMessageDealModule*> m_mapTmMessageDeal; //����ͨ�ŵ��Ĵ���ģ���б�(��ԴͨѶ��ַ����Mapӳ�䣩
	CCommThreadInfo* m_pCommThreadInfo;							//��������ͨ�ŵķ����̶߳���

	size_t m_nMaxSendCount;										//���ͻ�����Ҫ���͵ı��������,Ĭ��:100 ;
	
	//����
	std::list<PsendObject>	m_lstSendObjects;					//��������
	CCriticalSection		m_cs_SendObjects;					//

	void AppendRecvData(BYTE* buf,int Len);						//�������½��յ����ݵ�����������
	void AddLog(CString strLog,PSYSTEMTIME pTime= NULL);		//�����־�洢
	PsendObject GetOneSendObject();								//��ȡһ����Ҫ���͵Ķ���
	void		AddOneSendObject(PsendObject pSndObj);			//���һ����Ҫ���͵Ķ���
public:
	void SetScCommInfo(PSC_COMM_INFO pInfo);
	void SetTmCommInfo(PSC_TEXTMSG_INFO pInfo);
	void SetCommThreadInfo(CCommThreadInfo* pCommThreadInfo);
	void SetMsgInitFlag(bool b);
	CCommThreadInfo* GetCommThreadInfo();
	void SetMsgInitDlg(CRtuAttrMsgInit *pDlg);
	void SetMsgSendDlg(CRtuAttrTextMsg *pDlg);
public:
	CRtuAttrTextMsg* GetMsgTextDlg();
	CRtuAttrMsgInit* GetMsgInitDlg();
	const int       GetTmFactoryType();
	const int		GetFactoryType();
	const UINT32	GetAddressLocal();
	const CString    GetTmAddressLocal();
	bool GetMsgInitFlag();
	void RemoveScMessgaeModule(CSCMessageDealModule* pOldScMessageModule );		//ɾ������Ӧ�ñ��ģ����ģ�����ģ��
    void RemoveTmMessageModule(CTmMessageDealModule *pOldTmMessageModule);		//ɾ������Ӧ�ñ���

protected:
	CRtuAttrMsgInit *m_pMsgInitDlg;
	CCriticalSection m_cs_MsgInit;
	CCriticalSection m_cs_MsgDlg_Init;
	bool m_bIsMsgInit;

	CRtuAttrTextMsg *m_pMsgSendDlg;
	CCriticalSection m_cs_MsgDlg_Send;
};
