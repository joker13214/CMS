#pragma once

#include <afxmt.h>
#include "UdfGlobalFUNC.h"
#include "Handle_COM.h"
#include "Handle_NET.h"

#include "IOCP_Server.h"
#include "Handle_TcpServer.h"
#include "Handle_TcpClient.h"

#include "SysDataTrans.h"
#include "UdfLogSystem.h"
#include "ScModule_GTDZ.h"


class CCommServer;

//下发报文及帧数据结构
//如果是通过以太网方式传输，需要指定目的IP地址和端口号
class en_Frame_Class
{
public:
	en_Frame_Class();
	en_Frame_Class(BYTE *sndBuf,int len);
	en_Frame_Class(BYTE *Sndbuf,int len,PNET_INFO pToNetInfo);
	~en_Frame_Class(){}
public:
	void  SetSendBuf(BYTE* sndBuf,int len);													//设置下发的报文帧数据 和 帧长度
	BYTE* GetSendBuf()						{return m_Send_Buf;}							//获取下发的报文帧数据
	const int GetLength()					{return m_Send_Len;}							//获取下发报文的帧长度
	void  SetInterface(BYTE bInterface)		{m_Cm_Interface = bInterface;}					//设置通信接口  以太网 或者 串口(RS232)
	const BYTE GetInterface()				{return m_Cm_Interface;}						//获取通信接口  以太网 或者 串口(RS232)
	void  SetToNetInfo(PNET_INFO pNet_inf);													//设置目的IP和端口 当接口为以太网是使用
	PNET_INFO GetToNetInfo()				{return &m_To_Net_Info;}						//获取目的IP和端口 当接口为以太网时使用
protected:
   BYTE m_Send_Buf[RECV_BUFFER_LEN];   //报文
   int  m_Send_Len;					   //报文长度

   //接口 1：以太网 2：串口,默认为串口 
   //当是网络接口时，需要指明对方的IP和端口
   BYTE m_Cm_Interface;				   
   NET_INFO  m_To_Net_Info;    //目的 IP和port
};


/*
 *  类名：通信线程信息类
 *  功能：存储通信的配置信息和线程的相关数据信息
*/
class CCommThreadInfo
{
public:
	CCommThreadInfo();
	~CCommThreadInfo();
public:
	void SetThreadHandle(HANDLE newThreadHandle){m_handle_Thread = newThreadHandle;}		//设置Commserver线程句柄(HANDLE)
	void SetThreadID(DWORD newThreadID)			{m_dwThreadID = newThreadID;}				//设置Commserver线程的ID
	void SetThreadState(BYTE threadState)		{m_bThreadState = threadState;}				//设置Commserver线程的状态
	void SetThreadControl(bool nValue)			{m_bThreadControl = nValue;}				//设置Commserver运行停止控制标志，true:运行，false停止
	void SetCommType(BYTE ComType)				{m_CmType = ComType;}						//设置通信方式
	void SetCommInferfaceType(BYTE CmInterType) {m_CmInterface = CmInterType;}				//设置通信接口 以太网或串口方式
	void SetNetInfo(PNET_INFO pInfo);														//设置监听的IP地址和端口
	void SetComInfo(PCOM_INFO pInfo);														//设置发送/接收的串口配置信息
	void SetScCommInfo(PSC_COMM_INFO pInfo);
	void SetTmCommInfo(PSC_TEXTMSG_INFO pInfo);
	void SetHandle_Com(CHandle_COM* pComHandle)	{m_pHandle_Com = pComHandle;}				//设置发送/接收的串口接口对象
	void SetHandle_Net(CHandle_NET* pNetHandle)	{m_pHandle_Net = pNetHandle;}				//设置发送/接收的网络接口对象
	void SetScModule(CScModuleBase* pScModule)	{m_pScModule = pScModule;}
	void SetCommServer(CCommServer* pComServer)	{m_pCommServer = pComServer;}				//设置Commserver对象,Commserver主要负责控制管理Commserver线程
	void SetThreadName(CString newName)			{m_strThreadName = newName;}				//设置Thread's name 格式:THREAD001
	void SetThreadName(int threadIndex)			{m_strThreadName.Format("THREAD%03d",threadIndex);}
	void ThreadReset();																		//线程 重置 ，线程停止时信息设置

	void SetProtocolType(BYTE protocolType)		{m_ProtocolType = protocolType;}			//设置通信服务线程的通信协议类型(UDP or TCP)
	void SetHandle_TcpServer(CHandle_TcpServer* pTcpServer){m_pHandle_TcpServer = pTcpServer;}

	HANDLE GetThreadHandle()					{return m_handle_Thread;}					//获取Commserver线程句柄(HANDLE)  
	const DWORD  GetThreadID()					{return m_dwThreadID;}						//获取Commserver线程的ID
	const BYTE GetThreadState()					{return m_bThreadState;}					//获取Commserver线程的状态
	const bool GetThreadControl()				{return m_bThreadControl;}					//获取Commserver运行停止控制标志，true:运行，false停止
	const BYTE GetCommType()					{return m_CmType;}							//获取通信方式
	const BYTE GetCommInterfaceType()			{return m_CmInterface;}						//获取接口方式 1 以太网、 2 串口
	PCOM_INFO GetComInfo()						{return &m_stComInfo;}						//获取发送/接收的串口配置信息
	PNET_INFO GetNetInfo()						{return &m_stNetInfo;}						//获取监听的IP地址和端口
	PSC_COMM_INFO GetScCommInfo()				{return &m_stSCCommInfo;}					//获取卫星通信相关的信息
	PSC_TEXTMSG_INFO GetTmCommInfo()            {return &m_stTmCommInfo;}
	CHandle_COM* GetHandle_Com()				{return m_pHandle_Com;}						//获取发送/接收的串口接口对象
	CHandle_NET* GetHandle_Net()				{return m_pHandle_Net;}						//获取发送/接收的网络接口对象
	CCommServer* GetCommServer()				{return m_pCommServer;}						//获取Commserver对象,Commserver主要负责控制管理Commserver线程
	CScModuleBase* GetScModule()				{return m_pScModule;}
	const CString GetThreadName()				{return m_strThreadName;}					//获取Thread's Name 格式：THREAD001
	const CString GetThreadAllInfo();												        //获取线程的所有信息

	const BYTE GetProtocolType()				{return m_ProtocolType;}					//获取服务线程的通信协议类型（UDP or TCP)
	CHandle_TcpServer* GetHandle_TcpServer()	{return m_pHandle_TcpServer;}
	const CString GetProtocolTypeString();

	void			CheckReSendEvent();                                                     //检查相关的重发信息，FCB!= 0要求重发
public:
	void            FrameBufDeal(BYTE* buf,int nLen,BYTE CmInterface=Serial_CommInterface,
		                         CString FromIP="",int FromPort=0);							//数据报文帧处理 
    const CString   FrameBufferLog(BYTE* buf,int nLen,BYTE ArrowFlag,
		                           BYTE CommInterface = Serial_CommInterface,
								   PNET_INFO pNetInfo = NULL);								//报文信息日志信息

	//装载下发报文，等待线程获取后下发
	void			SendData(BYTE* sndbuf,int len,
		                     BYTE Interface=2,
							 PNET_INFO pNet_Info = NULL);

	const INT_PTR   GetEnframeCount();														//获得下发报文的数量
	en_Frame_Class* GetOneEnFrameClass();													//获取其中一条下发的信息
	void			InsertThreadLog(CString strThreadLog,
									_LOG_MESSAGE_TYPE messageType,
		                             _Log_level_type lLevelType = Normal_Log_Message, 
									 PSYSTEMTIME pST=NULL);									//插入日志信息
protected:
	HANDLE	m_handle_Thread;																//线程句柄
	DWORD	m_dwThreadID;																	//线程ID
	BYTE	m_bThreadState;																	//线程状态
	bool	m_bThreadControl;																//线程控制; true 启动  false 停止线程
	CString m_strThreadName;																//字符串Name  格式 THREAD001

	/*
	    通信方式
	1	GPRS通道通信
	2	GSM手机短信通道通信
	3	卫星通道通信
	4	超短波通道通信
	5	微波通道通信
	6	其他
	*/
	BYTE   m_CmType;

    //接口方式 
	//1: 以太网 
	//2: 串口
	BYTE   m_CmInterface;	

	/*
	  如果接口方式为以太网
	  协议方式有:UDP 和 TCP两种方式可选
	*/
	BYTE   m_ProtocolType;														

	NET_INFO m_stNetInfo;														//网络接口信息
	COM_INFO m_stComInfo;														//串口信息
	//UINT     m_nScAddress_Local;												//卫星通信本地地址;
	SC_COMM_INFO m_stSCCommInfo;												//卫星通信参数信息
	SC_TEXTMSG_INFO m_stTmCommInfo;                                             //短信通讯参数

	CHandle_COM* m_pHandle_Com;													//串口对象
	CHandle_NET* m_pHandle_Net;													//以太网对象 UDP	   通信接口
	CHandle_TcpServer* m_pHandle_TcpServer;										//以太网对象 TCPserver 接口
	CScModuleBase*     m_pScModule;												//卫星通信模块

	CList<en_Frame_Class*>        m_lstEnfrmaes;								//存储下发报文的链表
	//CMutex					  m_Mutex_lstEnFrames;							//访问下发报文的互斥量
	CCriticalSection			  m_Mutex_lstEnFrames;							//访问下发报文的互斥量
	CCommServer*				  m_pCommServer;								//对应的CommServer对象
public:
	CBtnDataBase m_btnStart;													//启动按钮
	CBtnDataBase m_btnDelete;													//删除按钮
	CBtnDataBase m_btnStop;														//停止按钮
	CBtnDataBase m_btnPro;														//属性按钮
	CBtnDataBase m_btnInit;                                                     //初始化按钮
};


/* 
*  类名：通信服务类
*  功能：负责管理与RTU链路通信的线程
*        包括启动/停止/管理线程
*        通信服务参数的配置
*/
class CCommServer
{
public:
	CCommServer(void);
	~CCommServer(void);
public:
	void Clear();							//清空内存等，供析构函数访问
	bool Initialization();                  //初始化并创建启动通信服务

public:
	CList<CCommThreadInfo*> m_lstThreads;   //存储线程信息的链表对象
public:
	static unsigned long __stdcall         CommThreadFunction(void* pPara);					 //Commserver线程函数
	static unsigned long __stdcall         CheckResendFunction(void* pPara);				 //处理重发的线程函数
	bool                                   CreateCommThread(CCommThreadInfo* pThreadInfo);   //创建并启动线程

	//线程控制函数
	bool                                   AddCommThread(CCommThreadInfo* pThreadInfo);      //添加新线程(并创建启动)，并将线程信息存入链表中
	bool                                   SuspendCommThread(CCommThreadInfo* pThreadInfo);	 //挂起线程
	bool								   ResumeCommThread(CCommThreadInfo* pThreadInfo);   //恢复线程
	bool								   StopCommThread(CCommThreadInfo* pThreadInfo);     //停止线程
	bool								   StartCommThread(CCommThreadInfo* pThreadInfo);    //启动线程
	bool								   DeleteCommThread(CCommThreadInfo* pThreadInfo);   //删除线程，并从链表里清除

protected:
	CMutex								   m_Mutex_Log_Operate;                              //日志信息操作
	bool								   LoadThreadsCfg();								 //读取配置文件配置信息
public:
	bool								   BuildRelaCommToRtu();							 //初始时通信服务于遥测终端建立关联，主要针对串口通信
	void								   BuildRelaCommToRtu(CRtuStation* pSub);			 
public:
	bool								   SaveThreadsCfg();								 //保存相关的Commserver服务线程的配置信息到配置文件中
public:
	void								   InsertThreadLog(CString strThreadLog,
														   _LOG_MESSAGE_TYPE messageType,
		                                                   _Log_level_type lLevelType = Normal_Log_Message, 
														   PSYSTEMTIME pST=NULL);			//插入日志信息

protected:
	bool								   m_blCR_thread_Control;			//重发线程控制标识
	bool								   m_blCR_thread_State;				//重发线程状态标识
public:
	void								  SetCR_ThreadControl(bool _val)	{m_blCR_thread_Control = _val;}		//设置重发线程的控制标识
	void								  SetCR_ThreadState(bool _val)		{m_blCR_thread_State = _val;}		//设置重发线程的状态标识
	const bool							  GetCR_ThreadControl()				{return m_blCR_thread_Control;}		//获取重发线程的控制标识
	const bool							  GetCR_ThreadState()				{return m_blCR_thread_State;}		//获取重发线程的状态标识
protected:
	void								  StartResendThread();				//启动处理重发的线程函数

};


