#pragma once
#include <afxmt.h>
#include "UdfGlobalFUNC.h"
#include "WaitTimer.h"
#include "UdfLogSystem.h"

class CRtuStation;
class CCommThreadInfo;
class COneData_INFO;
class CRtuPropertyDlg;

//重发对象
class RESEND_Object_SYQ
{
public:
	RESEND_Object_SYQ()
	{
		m_resendInterval = 0;
		m_CommInterface  = Serial_CommInterface;
	}
	CSYQFrameFormat m_packetFrame;    //帧格式
	BYTE			   m_CommInterface;  //接口（网络或串口）
	NET_INFO           m_ToNetInfo;		 //目的IP地址和端口号
	CWaitTimer         m_resend_timer;   //检查重发的Timer
	int                m_resendInterval; //重发的时间间隔,单位:秒(second)
};

class CSysDataTrans_Syq
{
public:
	CSysDataTrans_Syq(void);
	~CSysDataTrans_Syq(void);
public:

	//将报文转换为字符串信息
	CString BufferToString(BYTE* buffer,int Len);


	void Dispatcher(CSYQFrameFormat* psyqFrame);


	void SetRtuStation(CRtuStation* pSub)            {m_pSubStation = pSub;}					//设置对应的遥测终端对象

public:
	void AddRecvLog(BYTE* buffer,int len, CSYQFrameFormat* syqFrame=NULL);      //添加接收水文资源日志信息，主要是向站属性窗口添加日志信息，主要是向CRtuStation对象添加日志信息
	void AddSendLog(BYTE* buffer,int len, DWORD stcode);										//添加下发报文日志信息，主要是向站的属性窗口添加日志信息,主要是向CRtuStation对象添加日志信息
	void UpdateRtuCommState(CRtuStation* pSub);
	void UpdateRtuCommInfo(CRtuStation* pSub);

	void    BuildSendBuffer(CSYQFrameFormat* packetFrame,BYTE* sBuf,BYTE& len,int mlen);	//生成下发报文格式，字节序列数据

	int  SendRtuLinkFrame(BYTE* sBuf,int len,BYTE CmInterface = Serial_CommInterface,PNET_INFO pToNetInfo = NULL);//向遥测终端发送报文
	CCriticalSection m_cs_SendData;

	 void CheckResendObject();   //重发对象检查

protected:
	CList<RESEND_Object_SYQ*> m_lstReSendObjects;										//存储需要重发的数据信息
	CCriticalSection      m_Mutex_ReSend;											//互斥对象;
	CRtuStation*		  m_pSubStation;											//所属的遥测终端对象


	 void AddReSendObject (BYTE* sBuf,int len,CSYQFrameFormat* appFrame);    //

	 RESEND_Object_SYQ* GetReSendObject(BYTE commandtype);			//从m_lstReSendObjects中获取重发的对象并从重发列表中删除该对象
	 void DeleteOneReSendObject(BYTE commandtype);				//收到报文后清除重发链表里的重发对象

	//更新RTU的属性界面
private:
	void AddLogMessage(CRtuStation* pRTU,CString strLogMessage, _Log_level_type leveType =Normal_Log_Message, PSYSTEMTIME pTime = NULL);   //添加日志信息到窗口中显示
	void  AddDataMessage(CRtuStation* pRTU,CString strDataMessage,PSYSTEMTIME pTime=NULL);	
	void UpdateAttribFrame(CRtuStation* pRTU, int attrType);
	void RenewCallOperation(CRtuStation* pRTU, int attrType);
	CRtuPropertyDlg* GetRtuPropertyDlg();

	std::list<CString> m_YaosuList; //要素标示
	CCriticalSection m_cs_yaosu;

public:  

	void GetPropertyString(unsigned char *buf, int buflen, CString& str,int& len, int& datalen);//获取属性字符串
	void GetPropertyStringList(unsigned char *buf, int buflen,int&len, int stcode);//获取字符串链表 

	void InsertOneSaveData(COneData_INFO* pData);	
//	void GetStringType(CString str; int& type, )

	void Process_SYQ_30H(CSYQFrameFormat* psyqFrame);//测试报
	void Process_SYQ_31H(CSYQFrameFormat* psyqFrame);//均匀时段水文信息
	void Process_SYQ_32H(CSYQFrameFormat* psyqFrame);//遥测站定时报
	void Process_SYQ_33H(CSYQFrameFormat* psyqFrame);//遥测站加报报
	void Process_SYQ_34H(CSYQFrameFormat* psyqFrame);//遥测站小时报
	void Process_SYQ_35H(CSYQFrameFormat* psyqFrame);//遥测站人工置数报
	void Process_SYQ_36H(CSYQFrameFormat* psyqFrame);//中心站查询遥测站实时数据
	void Process_SYQ_37H(CSYQFrameFormat* psyqFrame);//中心站查询遥测站指定要素实时数据
	void Process_SYQ_38H(CSYQFrameFormat* psyqFrame);//中心站查询遥测站时段数据
	void Process_SYQ_39H(CSYQFrameFormat* psyqFrame);//中心站查询遥测站人工置数
	

	void Process_SYQ_40H(CSYQFrameFormat* psyqFrame);//修改配置参数1
	void Process_SYQ_41H(CSYQFrameFormat* psyqFrame);//读取配置参数1
	void Process_SYQ_42H(CSYQFrameFormat* psyqFrame);//修改配置参数2
	void Process_SYQ_43H(CSYQFrameFormat* psyqFrame);//读取配置参数2

	void Process_SYQ_44H(CSYQFrameFormat* psyqFrame);//修改配置参数3
	void Process_SYQ_45H(CSYQFrameFormat* psyqFrame);//读取配置参数3


	void Process_SYQ_46H(CSYQFrameFormat* psyqFrame);//修改密码
	void Process_SYQ_47H(CSYQFrameFormat* psyqFrame);//初始化固态存储数据
	void Process_SYQ_48H(CSYQFrameFormat* psyqFrame);//遥测站状态和报警
	void Process_SYQ_49H(CSYQFrameFormat* psyqFrame);//对钟

	//下行
	void SYQ_EN_COMMON(CSYQFrameFormat* appFrame,int type,BOOL bRepeat);//通用应答报问
	void SYQ_EN_37H(CSYQFrameFormat* appFrame,CString yaosustr);//查询指定要素数据
	void SYQ_EN_38H(CSYQFrameFormat* appFrame,CString yaosustr,int timetype,int ntime ,CTime starttime, CTime endtime);//查询时段数据
	void SYQ_EN_40H(CSYQFrameFormat* appFrame,CString stradd,short comtype,short no);//修改配置表1
	void SYQ_EN_41H(CSYQFrameFormat* appFrame,CString stradd,short comtype,short no);//查询配置表1
	void SYQ_EN_42H(CSYQFrameFormat* appFrame,_SYQ_CONFIG_PACKET2 packet);//修改配置表2
	void SYQ_EN_43H(CSYQFrameFormat* appFrame,_SYQ_CONFIG_PACKET2 packet);//查询配置表2

	void SYQ_EN_44H(CSYQFrameFormat* appFrame,_SYQ_CONFIG_PACKET3 packet);//修改配置表2
	void SYQ_EN_45H(CSYQFrameFormat* appFrame,_SYQ_CONFIG_PACKET3 packet);//查询配置表2


	void SYQ_EN_46H(CSYQFrameFormat* appFrame,int password);//修改密码
	void SYQ_EN_47H(CSYQFrameFormat* appFrame);//清楚固态存储数据
	void SYQ_EN_49H(CSYQFrameFormat* appFrame);//对钟
};
