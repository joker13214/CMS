#pragma once

#include <afxmt.h>
#include "Handle_TcpClient.h"

class CIOCP_Server;
typedef struct _IOCP_Thread_Info_ 
{
	CIOCP_Server* m_pIOCPServer;		//IOCPServer对象	
	DWORD		  m_ThreadIndex;		//线程序号
	bool          m_bThreadControl;		//线程控制标志 true: run,false: stop
	bool		  m_bThreadState;	    //线程状态,    true: running, false: stoped
	_IOCP_Thread_Info_()
	{
		m_pIOCPServer = NULL;
		m_ThreadIndex = 0;
		m_bThreadControl = false;
		m_bThreadState = false;
	}
	
}IOCP_Thread_Info,*PIOCP_Thread_Info;

const BYTE Auto_Disconnect_type = 1;     //Tcp link disconnect auto
const BYTE Hand_Disconnect_type = 2;	 //Tcp link disconnect by hand

/*名称:IOCP 完成I/O端口服务类
 *功能:(针对TCP通信)
 *	1.创建完成端口
 *  2.启动服务线程池
 *	3.维持网络链路通信
*/
class CIOCP_Server
{
public:
	CIOCP_Server(void);
public:
	~CIOCP_Server(void);
	static CIOCP_Server* Instance();								//IOCP服务实例

	//IOCP 服务线程函数,负责处理TCP模式的报文收，连接断开等事件
	static unsigned long __stdcall IOCPServerThreadFunction(void* pPARAM);
public:
	HANDLE GetIOCompletionPort_Handle()	{return m_hCompletionPort;}
	bool   StartIOCPServer();										//启动完成端口服务 并创建通信服务线程池
	bool   StopIOCPServer();										//停止完成端口服务

	CList<CHandle_TcpClient*>* GetClientObjects_List();				//获取负责通信的（TCP）客户端列表
	void  AddClientObject(CHandle_TcpClient* pClientObj);			//添加一个(TCP)客户端对象
	void  DelClientObject(CHandle_TcpClient* pClientObj);			//移除一个(TCP)客户端对象，并删除该对象的指针
protected:
	HANDLE  m_hCompletionPort;										//完成端口
	DWORD   m_dwThreadsCnt;											//服务线程池线程函数的个数 

	CCriticalSection		  m_csClientObjects_lst;				//互斥对象
	CList<CHandle_TcpClient*> m_lstClientObjects;					//负责客户端通信对象列表
	CList<IOCP_Thread_Info*>  m_lstIocpThreads;						//存储IOCP server线程信息
};
