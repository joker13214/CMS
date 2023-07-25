#pragma once

#include "UdfGlobalFUNC.h"
class CRtuStation;

//����ӿ�
class CHandle_NET
{
public:
	SOCKET m_NET_PORT;              //SOCKET 
	struct sockaddr_in server_RTU;  //1Ҫ���ӵ�RTU,����TCP����������ʹ��
	CHandle_NET(void);
public:
	virtual ~CHandle_NET(void);
public:
	unsigned char RECV_BUF[RECV_BUFFER_LEN];
	unsigned int  RECV_len;
protected:
	unsigned char FRAME_BUF[SEND_BUFFER_LEN];   //֡����
	unsigned int  FRAME_Len;                    //һ֡����

public:

	//UDP
	BOOL initNET(CString name,unsigned short port);       //��ʼ��UDP����
	BOOL initNET(unsigned long address,unsigned short port);
	unsigned long recvNET(unsigned char *buffer,long len);	//���ձ��ģ�����ҪFrom IP��Port
	unsigned long recvNET(unsigned char *buffer,long len,CString& FromIp,unsigned short& FromPort);//���ձ��ģ���Ҫ��ȡFrom IP��Port
	void sendNET(unsigned char *buffer,long len,CString addr,unsigned short port);
	unsigned long lookNETBuf();                          //���SOCKET �Ƿ������ݽ���

	//TCP



    //---------------------------------------------------------------------------------------
public:
	BOOL enframe_link(BYTE protocolType);			//protocolType 1 ˮ�ģ� 2 ˮ��Դ
	BOOL enframe_link_WaterResources();				//ˮ��Դ
	BOOL enframe_link_Hydrology();					//ˮ�� 
	BOOL enframe_link_SYQ();   ///ˮ����
	int  getFrameBuffer(BYTE* frBuf);				//��ȡ֡����
	BOOL CheckFrameBuffer(BYTE* buffer,int nLen);	//��鱨���Ƿ�׼ȷ

	void AddSendMessage(BYTE* buffer,WORD len,CString toIp,unsigned short toPort, PSYSTEMTIME pSt = NULL);  //����·���־����־ϵͳ
protected:
	CList<CRtuStation*> m_lstRelateStations;		//��ͨ�Žӿڹ�����ң���ն˻��б�

public:
	void AddRelateStation(CRtuStation* pSub);		//���һ�����ͨ�Žӿڹ�����ң���ն˶���
	void RemoveRelateStation(CRtuStation* pSub);	//ɾ��һ����ͨ�Žӿڹ�����ң���ն˶���
};
