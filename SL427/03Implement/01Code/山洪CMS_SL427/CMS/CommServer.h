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

//�·����ļ�֡���ݽṹ
//�����ͨ����̫����ʽ���䣬��Ҫָ��Ŀ��IP��ַ�Ͷ˿ں�
class en_Frame_Class
{
public:
	en_Frame_Class();
	en_Frame_Class(BYTE *sndBuf,int len);
	en_Frame_Class(BYTE *Sndbuf,int len,PNET_INFO pToNetInfo);
	~en_Frame_Class(){}
public:
	void  SetSendBuf(BYTE* sndBuf,int len);													//�����·��ı���֡���� �� ֡����
	BYTE* GetSendBuf()						{return m_Send_Buf;}							//��ȡ�·��ı���֡����
	const int GetLength()					{return m_Send_Len;}							//��ȡ�·����ĵ�֡����
	void  SetInterface(BYTE bInterface)		{m_Cm_Interface = bInterface;}					//����ͨ�Žӿ�  ��̫�� ���� ����(RS232)
	const BYTE GetInterface()				{return m_Cm_Interface;}						//��ȡͨ�Žӿ�  ��̫�� ���� ����(RS232)
	void  SetToNetInfo(PNET_INFO pNet_inf);													//����Ŀ��IP�Ͷ˿� ���ӿ�Ϊ��̫����ʹ��
	PNET_INFO GetToNetInfo()				{return &m_To_Net_Info;}						//��ȡĿ��IP�Ͷ˿� ���ӿ�Ϊ��̫��ʱʹ��
protected:
   BYTE m_Send_Buf[RECV_BUFFER_LEN];   //����
   int  m_Send_Len;					   //���ĳ���

   //�ӿ� 1����̫�� 2������,Ĭ��Ϊ���� 
   //��������ӿ�ʱ����Ҫָ���Է���IP�Ͷ˿�
   BYTE m_Cm_Interface;				   
   NET_INFO  m_To_Net_Info;    //Ŀ�� IP��port
};


/*
 *  ������ͨ���߳���Ϣ��
 *  ���ܣ��洢ͨ�ŵ�������Ϣ���̵߳����������Ϣ
*/
class CCommThreadInfo
{
public:
	CCommThreadInfo();
	~CCommThreadInfo();
public:
	void SetThreadHandle(HANDLE newThreadHandle){m_handle_Thread = newThreadHandle;}		//����Commserver�߳̾��(HANDLE)
	void SetThreadID(DWORD newThreadID)			{m_dwThreadID = newThreadID;}				//����Commserver�̵߳�ID
	void SetThreadState(BYTE threadState)		{m_bThreadState = threadState;}				//����Commserver�̵߳�״̬
	void SetThreadControl(bool nValue)			{m_bThreadControl = nValue;}				//����Commserver����ֹͣ���Ʊ�־��true:���У�falseֹͣ
	void SetCommType(BYTE ComType)				{m_CmType = ComType;}						//����ͨ�ŷ�ʽ
	void SetCommInferfaceType(BYTE CmInterType) {m_CmInterface = CmInterType;}				//����ͨ�Žӿ� ��̫���򴮿ڷ�ʽ
	void SetNetInfo(PNET_INFO pInfo);														//���ü�����IP��ַ�Ͷ˿�
	void SetComInfo(PCOM_INFO pInfo);														//���÷���/���յĴ���������Ϣ
	void SetScCommInfo(PSC_COMM_INFO pInfo);
	void SetTmCommInfo(PSC_TEXTMSG_INFO pInfo);
	void SetHandle_Com(CHandle_COM* pComHandle)	{m_pHandle_Com = pComHandle;}				//���÷���/���յĴ��ڽӿڶ���
	void SetHandle_Net(CHandle_NET* pNetHandle)	{m_pHandle_Net = pNetHandle;}				//���÷���/���յ�����ӿڶ���
	void SetScModule(CScModuleBase* pScModule)	{m_pScModule = pScModule;}
	void SetCommServer(CCommServer* pComServer)	{m_pCommServer = pComServer;}				//����Commserver����,Commserver��Ҫ������ƹ���Commserver�߳�
	void SetThreadName(CString newName)			{m_strThreadName = newName;}				//����Thread's name ��ʽ:THREAD001
	void SetThreadName(int threadIndex)			{m_strThreadName.Format("THREAD%03d",threadIndex);}
	void ThreadReset();																		//�߳� ���� ���߳�ֹͣʱ��Ϣ����

	void SetProtocolType(BYTE protocolType)		{m_ProtocolType = protocolType;}			//����ͨ�ŷ����̵߳�ͨ��Э������(UDP or TCP)
	void SetHandle_TcpServer(CHandle_TcpServer* pTcpServer){m_pHandle_TcpServer = pTcpServer;}

	HANDLE GetThreadHandle()					{return m_handle_Thread;}					//��ȡCommserver�߳̾��(HANDLE)  
	const DWORD  GetThreadID()					{return m_dwThreadID;}						//��ȡCommserver�̵߳�ID
	const BYTE GetThreadState()					{return m_bThreadState;}					//��ȡCommserver�̵߳�״̬
	const bool GetThreadControl()				{return m_bThreadControl;}					//��ȡCommserver����ֹͣ���Ʊ�־��true:���У�falseֹͣ
	const BYTE GetCommType()					{return m_CmType;}							//��ȡͨ�ŷ�ʽ
	const BYTE GetCommInterfaceType()			{return m_CmInterface;}						//��ȡ�ӿڷ�ʽ 1 ��̫���� 2 ����
	PCOM_INFO GetComInfo()						{return &m_stComInfo;}						//��ȡ����/���յĴ���������Ϣ
	PNET_INFO GetNetInfo()						{return &m_stNetInfo;}						//��ȡ������IP��ַ�Ͷ˿�
	PSC_COMM_INFO GetScCommInfo()				{return &m_stSCCommInfo;}					//��ȡ����ͨ����ص���Ϣ
	PSC_TEXTMSG_INFO GetTmCommInfo()            {return &m_stTmCommInfo;}
	CHandle_COM* GetHandle_Com()				{return m_pHandle_Com;}						//��ȡ����/���յĴ��ڽӿڶ���
	CHandle_NET* GetHandle_Net()				{return m_pHandle_Net;}						//��ȡ����/���յ�����ӿڶ���
	CCommServer* GetCommServer()				{return m_pCommServer;}						//��ȡCommserver����,Commserver��Ҫ������ƹ���Commserver�߳�
	CScModuleBase* GetScModule()				{return m_pScModule;}
	const CString GetThreadName()				{return m_strThreadName;}					//��ȡThread's Name ��ʽ��THREAD001
	const CString GetThreadAllInfo();												        //��ȡ�̵߳�������Ϣ

	const BYTE GetProtocolType()				{return m_ProtocolType;}					//��ȡ�����̵߳�ͨ��Э�����ͣ�UDP or TCP)
	CHandle_TcpServer* GetHandle_TcpServer()	{return m_pHandle_TcpServer;}
	const CString GetProtocolTypeString();

	void			CheckReSendEvent();                                                     //�����ص��ط���Ϣ��FCB!= 0Ҫ���ط�
public:
	void            FrameBufDeal(BYTE* buf,int nLen,BYTE CmInterface=Serial_CommInterface,
		                         CString FromIP="",int FromPort=0);							//���ݱ���֡���� 
    const CString   FrameBufferLog(BYTE* buf,int nLen,BYTE ArrowFlag,
		                           BYTE CommInterface = Serial_CommInterface,
								   PNET_INFO pNetInfo = NULL);								//������Ϣ��־��Ϣ

	//װ���·����ģ��ȴ��̻߳�ȡ���·�
	void			SendData(BYTE* sndbuf,int len,
		                     BYTE Interface=2,
							 PNET_INFO pNet_Info = NULL);

	const INT_PTR   GetEnframeCount();														//����·����ĵ�����
	en_Frame_Class* GetOneEnFrameClass();													//��ȡ����һ���·�����Ϣ
	void			InsertThreadLog(CString strThreadLog,
									_LOG_MESSAGE_TYPE messageType,
		                             _Log_level_type lLevelType = Normal_Log_Message, 
									 PSYSTEMTIME pST=NULL);									//������־��Ϣ
protected:
	HANDLE	m_handle_Thread;																//�߳̾��
	DWORD	m_dwThreadID;																	//�߳�ID
	BYTE	m_bThreadState;																	//�߳�״̬
	bool	m_bThreadControl;																//�߳̿���; true ����  false ֹͣ�߳�
	CString m_strThreadName;																//�ַ���Name  ��ʽ THREAD001

	/*
	    ͨ�ŷ�ʽ
	1	GPRSͨ��ͨ��
	2	GSM�ֻ�����ͨ��ͨ��
	3	����ͨ��ͨ��
	4	���̲�ͨ��ͨ��
	5	΢��ͨ��ͨ��
	6	����
	*/
	BYTE   m_CmType;

    //�ӿڷ�ʽ 
	//1: ��̫�� 
	//2: ����
	BYTE   m_CmInterface;	

	/*
	  ����ӿڷ�ʽΪ��̫��
	  Э�鷽ʽ��:UDP �� TCP���ַ�ʽ��ѡ
	*/
	BYTE   m_ProtocolType;														

	NET_INFO m_stNetInfo;														//����ӿ���Ϣ
	COM_INFO m_stComInfo;														//������Ϣ
	//UINT     m_nScAddress_Local;												//����ͨ�ű��ص�ַ;
	SC_COMM_INFO m_stSCCommInfo;												//����ͨ�Ų�����Ϣ
	SC_TEXTMSG_INFO m_stTmCommInfo;                                             //����ͨѶ����

	CHandle_COM* m_pHandle_Com;													//���ڶ���
	CHandle_NET* m_pHandle_Net;													//��̫������ UDP	   ͨ�Žӿ�
	CHandle_TcpServer* m_pHandle_TcpServer;										//��̫������ TCPserver �ӿ�
	CScModuleBase*     m_pScModule;												//����ͨ��ģ��

	CList<en_Frame_Class*>        m_lstEnfrmaes;								//�洢�·����ĵ�����
	//CMutex					  m_Mutex_lstEnFrames;							//�����·����ĵĻ�����
	CCriticalSection			  m_Mutex_lstEnFrames;							//�����·����ĵĻ�����
	CCommServer*				  m_pCommServer;								//��Ӧ��CommServer����
public:
	CBtnDataBase m_btnStart;													//������ť
	CBtnDataBase m_btnDelete;													//ɾ����ť
	CBtnDataBase m_btnStop;														//ֹͣ��ť
	CBtnDataBase m_btnPro;														//���԰�ť
	CBtnDataBase m_btnInit;                                                     //��ʼ����ť
};


/* 
*  ������ͨ�ŷ�����
*  ���ܣ����������RTU��·ͨ�ŵ��߳�
*        ��������/ֹͣ/�����߳�
*        ͨ�ŷ������������
*/
class CCommServer
{
public:
	CCommServer(void);
	~CCommServer(void);
public:
	void Clear();							//����ڴ�ȣ���������������
	bool Initialization();                  //��ʼ������������ͨ�ŷ���

public:
	CList<CCommThreadInfo*> m_lstThreads;   //�洢�߳���Ϣ���������
public:
	static unsigned long __stdcall         CommThreadFunction(void* pPara);					 //Commserver�̺߳���
	static unsigned long __stdcall         CheckResendFunction(void* pPara);				 //�����ط����̺߳���
	bool                                   CreateCommThread(CCommThreadInfo* pThreadInfo);   //�����������߳�

	//�߳̿��ƺ���
	bool                                   AddCommThread(CCommThreadInfo* pThreadInfo);      //������߳�(����������)�������߳���Ϣ����������
	bool                                   SuspendCommThread(CCommThreadInfo* pThreadInfo);	 //�����߳�
	bool								   ResumeCommThread(CCommThreadInfo* pThreadInfo);   //�ָ��߳�
	bool								   StopCommThread(CCommThreadInfo* pThreadInfo);     //ֹͣ�߳�
	bool								   StartCommThread(CCommThreadInfo* pThreadInfo);    //�����߳�
	bool								   DeleteCommThread(CCommThreadInfo* pThreadInfo);   //ɾ���̣߳��������������

protected:
	CMutex								   m_Mutex_Log_Operate;                              //��־��Ϣ����
	bool								   LoadThreadsCfg();								 //��ȡ�����ļ�������Ϣ
public:
	bool								   BuildRelaCommToRtu();							 //��ʼʱͨ�ŷ�����ң���ն˽�����������Ҫ��Դ���ͨ��
	void								   BuildRelaCommToRtu(CRtuStation* pSub);			 
public:
	bool								   SaveThreadsCfg();								 //������ص�Commserver�����̵߳�������Ϣ�������ļ���
public:
	void								   InsertThreadLog(CString strThreadLog,
														   _LOG_MESSAGE_TYPE messageType,
		                                                   _Log_level_type lLevelType = Normal_Log_Message, 
														   PSYSTEMTIME pST=NULL);			//������־��Ϣ

protected:
	bool								   m_blCR_thread_Control;			//�ط��߳̿��Ʊ�ʶ
	bool								   m_blCR_thread_State;				//�ط��߳�״̬��ʶ
public:
	void								  SetCR_ThreadControl(bool _val)	{m_blCR_thread_Control = _val;}		//�����ط��̵߳Ŀ��Ʊ�ʶ
	void								  SetCR_ThreadState(bool _val)		{m_blCR_thread_State = _val;}		//�����ط��̵߳�״̬��ʶ
	const bool							  GetCR_ThreadControl()				{return m_blCR_thread_Control;}		//��ȡ�ط��̵߳Ŀ��Ʊ�ʶ
	const bool							  GetCR_ThreadState()				{return m_blCR_thread_State;}		//��ȡ�ط��̵߳�״̬��ʶ
protected:
	void								  StartResendThread();				//���������ط����̺߳���

};


