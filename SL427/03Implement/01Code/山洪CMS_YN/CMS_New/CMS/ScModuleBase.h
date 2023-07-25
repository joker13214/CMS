#pragma once
#include <map>
#include <list>
#include <afxmt.h>
#include "WaitTimer.h"
#include "SCMessageDealModule.h"

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
	BYTE m_bAppProtoType;           //Ӧ�ò�Э�����͡�
	int  m_nFactoryType;			//��������
	UINT32 m_nAddressLocal;			//����ͨ�ŵ�ַ

	BYTE m_RECV_BUF[1024 * 10];		//����������Ϣ����
	int  m_RECV_Len;				//�������ݳ���
public:
	virtual void  RecvMessage(BYTE* buf,int Len) = 0 ;           //������ܱ���
	virtual bool  SendMessage(BYTE* buf,int &len) = 0 ;          //��ȡ������Ϣ
protected:
	CWaitTimer m_CheckSendTimer;                            //��ⱨ�ķ���,
	__int64    m_nSendInterval;                             //���ķ��ͼ��,
	bool       m_bSendBefore;								//��֮ǰ�Ƿ��͹�������Ϣ(������Ǽ��ͨѶʱʹ��)
	std::map<UINT,CSCMessageDealModule*> m_mapSCMessageDeal;//����ͨ�ŵ��Ĵ���ģ���б�(��Դͨ�ŵ�ַ����Mapӳ��)
	CCommThreadInfo* m_pCommThreadInfo;						//��������ͨ�ŵķ����̶߳���

	size_t m_nMaxSendCount;									//���ͻ�����Ҫ���͵ı��������,Ĭ��:100 ;
	
	//����
	std::list<PsendObject>	m_lstSendObjects;				//��������
	CCriticalSection		m_cs_SendObjects;			    //

	
	void AppendRecvData(BYTE* buf,int Len);					//�������½��յ����ݵ�����������
	void AddLog(CString strLog,PSYSTEMTIME pTime= NULL);	//�����־�洢
	PsendObject GetOneSendObject();							//��ȡһ����Ҫ���͵Ķ���
	void		AddOneSendObject(PsendObject pSndObj);		//���һ����Ҫ���͵Ķ���
public:
	void SetAppProtoType(BYTE b){m_bAppProtoType = b;}
	BYTE GetAppProtoType(){return m_bAppProtoType;}
	void SetScCommInfo(PSC_COMM_INFO pInfo);
	void SetCommThreadInfo(CCommThreadInfo* pCommThreadInfo);
	CCommThreadInfo* GetCommThreadInfo();

public:
	const int		GetFactoryType();
	const UINT32	GetAddressLocal();
	void RemoveScMessgaeModule( CSCMessageDealModule* pOldScMessageModule );  //ɾ������Ӧ�ñ��ģ����ģ�����ģ��
};
