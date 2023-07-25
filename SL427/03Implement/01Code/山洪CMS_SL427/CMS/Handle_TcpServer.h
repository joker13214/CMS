#pragma once

#include "UdfGlobalFUNC.h"

/* Name: Tcp����Listen�˿�
   Functaion: ���Server�˵ȴ�Client����
*/
class CHandle_TcpServer
{
public:
	CHandle_TcpServer(void);
public:
	~CHandle_TcpServer(void);
public:
	bool InitTcpServer();				//����Tcp Listen���񣬳�ʼSocket,������bind����
	void DealConnectionEvent();			//����ͻ��������¼�����ӦAccept�¼�������IO��ɶ˿ڽ�����ϵ
	void SetCmType(BYTE newCmType)	{if (m_CmType != newCmType) m_CmType = newCmType;}
	const BYTE GetCmType()			{return m_CmType;}

	void SetTcpServerNetInfo(PNET_INFO pNetInfo);
	PNET_INFO GetTcpServerNetInfo()	{return &m_NetInfo;}
protected:
	SOCKET		m_Socket;				//Socket Handle
	NET_INFO	m_NetInfo;				//Listen����Ϣ��IP��ַ�Ͷ˿ں�
	BYTE		m_CmType;				//ͨ�ŷ�ʽ
};
