#pragma once

/* Name: 处理TCP接收的数据校验，拆分
   Functaion: 与客服端连接通信的Soket连接
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
	virtual void Deal_RecvData(BYTE* buffer,WORD len)=0;        	//从IOCP线程中获取报文信息通过此函数处理 
	virtual BOOL enable_linkframe()=0;               				//检查接收的报文是否有正确帧格式的报文
	virtual void AddSendMessage(BYTE* buffer,WORD len,PSYSTEMTIME pSt = NULL)=0;    //添加下发日志信息到日志系统
	virtual void AddRecvMessage(BYTE* buffer,WORD Len,PSYSTEMTIME pSt = NULL)=0;	  //添加上送日志信息到日志系统

public:
	CHandle_TcpClient* m_pTcpClient;
	
	BYTE     m_RECV_Buf[4096];							//接收报文缓冲区
	WORD     m_RECV_Len;									//报文长度

	//正确帧报文数据
	unsigned char m_FRAME_BUF[SEND_BUFFER_LEN];			//帧报文
	unsigned int  m_FRAME_Len;							//一帧长度

};

/* Name: 水资源监控管理系统数据传输规约: SL427-2008
   Functaion: 
*/
class CSL427_TcpHandle :public CProtocol_TcpHandle
{
public:
	CSL427_TcpHandle(CHandle_TcpClient *p);
	virtual ~CSL427_TcpHandle();

public:
	CSL427_TcpHandle();
	virtual void Deal_RecvData(BYTE* buffer,WORD len);		//从IOCP线程中获取报文信息通过此函数处理 
	virtual BOOL enable_linkframe();							//检查接收的报文是否有正确帧格式的报文
	virtual void AddSendMessage(BYTE* buffer,WORD len,PSYSTEMTIME pSt = NULL);    //添加下发日志信息到日志系统
	virtual void AddRecvMessage(BYTE* buffer,WORD Len,PSYSTEMTIME pSt = NULL);	  //添加上送日志信息到日志系统

};

/* Name: 水文测报系统技术规约和协议：SCSW008-2001
   Functaion: 
*/
class CSCSW_TcpHandle :public CProtocol_TcpHandle
{
public:
	CSCSW_TcpHandle(CHandle_TcpClient *p);
	virtual ~CSCSW_TcpHandle();
public:
	virtual void Deal_RecvData(BYTE* buffer,WORD len);		//从IOCP线程中获取报文信息通过此函数处理 
	virtual BOOL enable_linkframe();							//检查接收的报文是否有正确帧格式的报文
	virtual void AddSendMessage(BYTE* buffer,WORD len,PSYSTEMTIME pSt = NULL);    //添加下发日志信息到日志系统
	virtual void AddRecvMessage(BYTE* buffer,WORD Len,PSYSTEMTIME pSt = NULL);	  //添加上送日志信息到日志系统 

	BOOL ParseLinkFrame_Msg();
	BOOL ParseLinkFrame_Write();
private:
	int m_nDivCnt;
	int m_recvFrameType;
	int m_msgMode;
	bool m_isFirstMsg;//每天
};


/* Name: 水雨情遥测系统数据规约和协议SYQ
   Functaion: 
*/
class CSYQ_TcpHandle :public CProtocol_TcpHandle
{
public:
	CSYQ_TcpHandle(CHandle_TcpClient *p);
	virtual ~CSYQ_TcpHandle();
public:
	virtual void Deal_RecvData(BYTE* buffer,WORD len);		//从IOCP线程中获取报文信息通过此函数处理 
	virtual BOOL enable_linkframe();							//检查接收的报文是否有正确帧格式的报文
	virtual void AddSendMessage(BYTE* buffer,WORD len,PSYSTEMTIME pSt = NULL);    //添加下发日志信息到日志系统
	virtual void AddRecvMessage(BYTE* buffer,WORD Len,PSYSTEMTIME pSt = NULL);	  //添加上送日志信息到日志系统 

	BOOL ParseLinkFrame_Msg();
	BOOL ParseLinkFrame_Write();
};