#pragma once
#include <map>
#include <list>
#include <afxmt.h>
#include "WaitTimer.h"
#include "SCMessageDealModule.h"
#include "TmMessageDealModule.h"
#include "RtuAttrMsgInit.h"
#include "RtuAttrTextMsg.h"

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
	int  m_nFactoryType;			//厂家类型
	UINT32 m_nAddressLocal;			//卫星通信地址

	BYTE m_RECV_BUF[1024 * 10];		//卫星数据信息缓冲
	int  m_RECV_Len;				//卫星数据长度
protected:
	int m_nTmFactoryType;			//短信厂家类型
	CString m_nTmAddrLocal;			//短信通讯地址

public:
	virtual void  RecvMessage(BYTE* buf,int Len) = 0 ;           //处理接受报文
	virtual bool  SendMessage(BYTE* buf,int &len) = 0 ;          //获取发送信息
protected:
	CWaitTimer m_CheckSendTimer;								//检测报文发送,
	__int64    m_nSendInterval;									//报文发送间隔,
	bool       m_bSendBefore;									//在之前是否发送过数据信息(检查卫星间隔通讯时使用)
	std::map<UINT,CSCMessageDealModule*> m_mapSCMessageDeal;	//卫星通信电文处理模块列表(按源通信地址建立Map映射)
	std::map<CString,CTmMessageDealModule*> m_mapTmMessageDeal; //短信通信电文处理模块列表(按源通讯地址建立Map映射）
	CCommThreadInfo* m_pCommThreadInfo;							//管理卫星通信的服务线程对象

	size_t m_nMaxSendCount;										//发送缓冲需要发送的报文数最大,默认:100 ;
	
	//发送
	std::list<PsendObject>	m_lstSendObjects;					//发送链表
	CCriticalSection		m_cs_SendObjects;					//

	void AppendRecvData(BYTE* buf,int Len);						//拷贝最新接收的数据到解析缓冲区
	void AddLog(CString strLog,PSYSTEMTIME pTime= NULL);		//添加日志存储
	PsendObject GetOneSendObject();								//获取一个需要传送的对象
	void		AddOneSendObject(PsendObject pSndObj);			//添加一个需要发送的对象
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
	void RemoveScMessgaeModule(CSCMessageDealModule* pOldScMessageModule );		//删除卫星应用报文（电文）处理模块
    void RemoveTmMessageModule(CTmMessageDealModule *pOldTmMessageModule);		//删除短信应用报文

protected:
	CRtuAttrMsgInit *m_pMsgInitDlg;
	CCriticalSection m_cs_MsgInit;
	CCriticalSection m_cs_MsgDlg_Init;
	bool m_bIsMsgInit;

	CRtuAttrTextMsg *m_pMsgSendDlg;
	CCriticalSection m_cs_MsgDlg_Send;
};
