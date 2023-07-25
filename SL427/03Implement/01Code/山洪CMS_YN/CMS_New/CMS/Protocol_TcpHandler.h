#pragma once

/* Name: ����TCP���յ�����У�飬���
   Functaion: ��ͷ�������ͨ�ŵ�Soket����
*/
#include "UdfGlobalFUNC.h"
#include "Handle_TcpServer.h"
class CHandle_TcpClient;

class CProtocol_TcpHandle
{
public:
	CProtocol_TcpHandle(CHandle_TcpClient *p)
	{
		m_pTcpClient = p;
		memset(m_RECV_Buf,0,sizeof(m_RECV_Buf));
		m_RECV_Len = 0;

	}
	virtual ~CProtocol_TcpHandle();

public:
	virtual void Deal_RecvData(BYTE* buffer,WORD len)=0;        	//��IOCP�߳��л�ȡ������Ϣͨ���˺������� 
	virtual BOOL enable_linkframe()=0;               				//�����յı����Ƿ�����ȷ֡��ʽ�ı���
	virtual void AddSendMessage(BYTE* buffer,WORD len,PSYSTEMTIME pSt = NULL)=0;    //����·���־��Ϣ����־ϵͳ
	virtual void AddRecvMessage(BYTE* buffer,WORD Len,PSYSTEMTIME pSt = NULL)=0;	  //���������־��Ϣ����־ϵͳ

public:
	CHandle_TcpClient* m_pTcpClient;
	
	BYTE     m_RECV_Buf[4096];							//���ձ��Ļ�����
	WORD     m_RECV_Len;									//���ĳ���

	//��ȷ֡��������
	unsigned char m_FRAME_BUF[SEND_BUFFER_LEN];			//֡����
	unsigned int  m_FRAME_Len;							//һ֡����

};

/* Name: ˮ��Դ��ع���ϵͳ���ݴ����Լ: SL427-2008
   Functaion: 
*/
class CSL427_TcpHandle :public CProtocol_TcpHandle
{
public:
	CSL427_TcpHandle(CHandle_TcpClient *p);
	virtual ~CSL427_TcpHandle();

public:
	CSL427_TcpHandle();
	virtual void Deal_RecvData(BYTE* buffer,WORD len);		//��IOCP�߳��л�ȡ������Ϣͨ���˺������� 
	virtual BOOL enable_linkframe();							//�����յı����Ƿ�����ȷ֡��ʽ�ı���
	virtual void AddSendMessage(BYTE* buffer,WORD len,PSYSTEMTIME pSt = NULL);    //����·���־��Ϣ����־ϵͳ
	virtual void AddRecvMessage(BYTE* buffer,WORD Len,PSYSTEMTIME pSt = NULL);	  //���������־��Ϣ����־ϵͳ

};

/* Name: ˮ�Ĳⱨϵͳ������Լ��Э�飺SCSW008-2001
   Functaion: 
*/
class CSCSW_TcpHandle :public CProtocol_TcpHandle
{
public:
	CSCSW_TcpHandle(CHandle_TcpClient *p);
	virtual ~CSCSW_TcpHandle();
public:
	virtual void Deal_RecvData(BYTE* buffer,WORD len);		//��IOCP�߳��л�ȡ������Ϣͨ���˺������� 
	virtual BOOL enable_linkframe();							//�����յı����Ƿ�����ȷ֡��ʽ�ı���
	virtual void AddSendMessage(BYTE* buffer,WORD len,PSYSTEMTIME pSt = NULL);    //����·���־��Ϣ����־ϵͳ
	virtual void AddRecvMessage(BYTE* buffer,WORD Len,PSYSTEMTIME pSt = NULL);	  //���������־��Ϣ����־ϵͳ 

	BOOL ParseLinkFrame_Msg();
	BOOL ParseLinkFrame_Write();
private:
	int m_nDivCnt;
	int m_recvFrameType;
	int m_msgMode;
	bool m_isFirstMsg;//ÿ��
};


/* Name: ˮ����ң��ϵͳ���ݹ�Լ��Э��SYQ
   Functaion: 
*/
class CSYQ_TcpHandle :public CProtocol_TcpHandle
{
public:
	CSYQ_TcpHandle(CHandle_TcpClient *p);
	virtual ~CSYQ_TcpHandle();
public:
	virtual void Deal_RecvData(BYTE* buffer,WORD len);		//��IOCP�߳��л�ȡ������Ϣͨ���˺������� 
	virtual BOOL enable_linkframe();							//�����յı����Ƿ�����ȷ֡��ʽ�ı���
	virtual void AddSendMessage(BYTE* buffer,WORD len,PSYSTEMTIME pSt = NULL);    //����·���־��Ϣ����־ϵͳ
	virtual void AddRecvMessage(BYTE* buffer,WORD Len,PSYSTEMTIME pSt = NULL);	  //���������־��Ϣ����־ϵͳ 

	BOOL ParseLinkFrame_Msg();
	BOOL ParseLinkFrame_Write();
};