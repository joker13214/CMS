#pragma once

#include "UdfGlobalFUNC.h"
class CRtuStation;

//网络接口
class CHandle_NET
{
public:
	SOCKET m_NET_PORT;              //SOCKET 
	struct sockaddr_in server_RTU;  //1要连接的RTU,采用TCP处理流程中使用
	CHandle_NET(void);
public:
	virtual ~CHandle_NET(void);
public:
	unsigned char RECV_BUF[RECV_BUFFER_LEN];
	unsigned int  RECV_len;
protected:
	unsigned char FRAME_BUF[SEND_BUFFER_LEN];   //帧报文
	unsigned int  FRAME_Len;                    //一帧长度

public:

	//UDP
	BOOL initNET(CString name,unsigned short port);       //初始化UDP连接
	BOOL initNET(unsigned long address,unsigned short port);
	unsigned long recvNET(unsigned char *buffer,long len);	//接收报文，不需要From IP和Port
	unsigned long recvNET(unsigned char *buffer,long len,CString& FromIp,unsigned short& FromPort);//接收报文，需要获取From IP和Port
	void sendNET(unsigned char *buffer,long len,CString addr,unsigned short port);
	unsigned long lookNETBuf();                          //检查SOCKET 是否有数据接收

	//TCP



    //---------------------------------------------------------------------------------------
public:
	BOOL enframe_link(BYTE protocolType);			//protocolType 1 水文， 2 水资源
	BOOL enframe_link_WaterResources();				//水资源
	BOOL enframe_link_Hydrology();					//水文 
	BOOL enframe_link_SYQ();   ///水雨情
	int  getFrameBuffer(BYTE* frBuf);				//获取帧报文
	BOOL CheckFrameBuffer(BYTE* buffer,int nLen);	//检查报文是否准确

	void AddSendMessage(BYTE* buffer,WORD len,CString toIp,unsigned short toPort, PSYSTEMTIME pSt = NULL);  //添加下发日志到日志系统
protected:
	CList<CRtuStation*> m_lstRelateStations;		//此通信接口关联的遥测终端机列表

public:
	void AddRelateStation(CRtuStation* pSub);		//添加一个与此通信接口关联的遥测终端对象
	void RemoveRelateStation(CRtuStation* pSub);	//删除一个与通信接口关联的遥测终端对象
};
