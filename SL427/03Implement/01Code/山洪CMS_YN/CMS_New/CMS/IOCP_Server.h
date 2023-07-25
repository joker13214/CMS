#pragma once

#include <afxmt.h>
#include "Handle_TcpClient.h"

class CIOCP_Server;
typedef struct _IOCP_Thread_Info_ 
{
	CIOCP_Server* m_pIOCPServer;		//IOCPServer����	
	DWORD		  m_ThreadIndex;		//�߳����
	bool          m_bThreadControl;		//�߳̿��Ʊ�־ true: run,false: stop
	bool		  m_bThreadState;	    //�߳�״̬,    true: running, false: stoped
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

/*����:IOCP ���I/O�˿ڷ�����
 *����:(���TCPͨ��)
 *	1.������ɶ˿�
 *  2.���������̳߳�
 *	3.ά��������·ͨ��
*/
class CIOCP_Server
{
public:
	CIOCP_Server(void);
public:
	~CIOCP_Server(void);
	static CIOCP_Server* Instance();								//IOCP����ʵ��

	//IOCP �����̺߳���,������TCPģʽ�ı����գ����ӶϿ����¼�
	static unsigned long __stdcall IOCPServerThreadFunction(void* pPARAM);
public:
	HANDLE GetIOCompletionPort_Handle()	{return m_hCompletionPort;}
	bool   StartIOCPServer();										//������ɶ˿ڷ��� ������ͨ�ŷ����̳߳�
	bool   StopIOCPServer();										//ֹͣ��ɶ˿ڷ���

	CList<CHandle_TcpClient*>* GetClientObjects_List();				//��ȡ����ͨ�ŵģ�TCP���ͻ����б�
	void  AddClientObject(CHandle_TcpClient* pClientObj);			//���һ��(TCP)�ͻ��˶���
	void  DelClientObject(CHandle_TcpClient* pClientObj);			//�Ƴ�һ��(TCP)�ͻ��˶��󣬲�ɾ���ö����ָ��
protected:
	HANDLE  m_hCompletionPort;										//��ɶ˿�
	DWORD   m_dwThreadsCnt;											//�����̳߳��̺߳����ĸ��� 

	CCriticalSection		  m_csClientObjects_lst;				//�������
	CList<CHandle_TcpClient*> m_lstClientObjects;					//����ͻ���ͨ�Ŷ����б�
	CList<IOCP_Thread_Info*>  m_lstIocpThreads;						//�洢IOCP server�߳���Ϣ
};
