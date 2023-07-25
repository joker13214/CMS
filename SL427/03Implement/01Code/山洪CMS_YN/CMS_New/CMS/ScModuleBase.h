#pragma once
#include <map>
#include <list>
#include <afxmt.h>
#include "WaitTimer.h"
#include "SCMessageDealModule.h"

//Send Object (卫星通信报文快)
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
	BYTE m_bAppProtoType;           //应用层协议类型。
	int  m_nFactoryType;			//厂家类型
	UINT32 m_nAddressLocal;			//卫星通信地址

	BYTE m_RECV_BUF[1024 * 10];		//卫星数据信息缓冲
	int  m_RECV_Len;				//卫星数据长度
public:
	virtual void  RecvMessage(BYTE* buf,int Len) = 0 ;           //处理接受报文
	virtual bool  SendMessage(BYTE* buf,int &len) = 0 ;          //获取发送信息
protected:
	CWaitTimer m_CheckSendTimer;                            //检测报文发送,
	__int64    m_nSendInterval;                             //报文发送间隔,
	bool       m_bSendBefore;								//在之前是否发送过数据信息(检查卫星间隔通讯时使用)
	std::map<UINT,CSCMessageDealModule*> m_mapSCMessageDeal;//卫星通信电文处理模块列表(按源通信地址建立Map映射)
	CCommThreadInfo* m_pCommThreadInfo;						//管理卫星通信的服务线程对象

	size_t m_nMaxSendCount;									//发送缓冲需要发送的报文数最大,默认:100 ;
	
	//发送
	std::list<PsendObject>	m_lstSendObjects;				//发送链表
	CCriticalSection		m_cs_SendObjects;			    //

	
	void AppendRecvData(BYTE* buf,int Len);					//拷贝最新接收的数据到解析缓冲区
	void AddLog(CString strLog,PSYSTEMTIME pTime= NULL);	//添加日志存储
	PsendObject GetOneSendObject();							//获取一个需要传送的对象
	void		AddOneSendObject(PsendObject pSndObj);		//添加一个需要发送的对象
public:
	void SetAppProtoType(BYTE b){m_bAppProtoType = b;}
	BYTE GetAppProtoType(){return m_bAppProtoType;}
	void SetScCommInfo(PSC_COMM_INFO pInfo);
	void SetCommThreadInfo(CCommThreadInfo* pCommThreadInfo);
	CCommThreadInfo* GetCommThreadInfo();

public:
	const int		GetFactoryType();
	const UINT32	GetAddressLocal();
	void RemoveScMessgaeModule( CSCMessageDealModule* pOldScMessageModule );  //删除卫星应用报文（电文）处理模块
};
