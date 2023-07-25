#pragma once
#include "UdfGlobalFUNC.h"
class CRtuStation;
//串行接口Class

class CHandle_COM
{
public:
	CHandle_COM(void);
	HANDLE m_COM_HANDLE;     //串口句柄

public:
	virtual ~CHandle_COM(void);
public:
	unsigned char RECV_BUF[RECV_BUFFER_LEN];    //接收数据缓冲区
	unsigned int  RECV_len;                     //接收数据宽度
public:
	unsigned char FRAME_BUF[SEND_BUFFER_LEN];   //帧报文
	unsigned int  FRAME_Len;                    //一帧长度
    BYTE m_bAppProtoType;
	int m_recvFrameType;
	int m_msgMode;
	int m_DataOrWriteFlag;

public:
	void SetAppProtoType(BYTE type);
	BYTE GetAppProtoType();
	static unsigned char Add_Sum_Check(unsigned char *data, unsigned char data_len);
	void clearCOM();

	//清空接收Buffer [RECV_BUF]
	void clearBuf();
public:
	void recvDATA(unsigned char *buffer, unsigned int len);
public:
	//从串口接收数据
	unsigned long recvCOM(unsigned char *buffer, unsigned long  len);

	//通过串口发送数据
	void sendCOM(unsigned char *buffer,unsigned long len);

	//检查串口是否有数据接收
	//返回值为数据字节数
	unsigned long lookCOMBuf();

	//初始化串口
	//name :    串口名称 'COM1'、 '\\.\COM29'
	//Baund:    波特率
	//DataBits: 数据位
	//StopBits: 停止位
	//Partity:  校验标志  （NOPARITY    0 无检验）、（ODDPARITY   1 奇校验）、（EVENPARITY  2 偶校验）、（MARKPARITY  3）、 （SPACEPARITY 4）
	BOOL initCOM(CString name,int Baund=CBR_9600,int DataBits=8,int StopBits=ONESTOPBIT,int Parity=NOPARITY/*EVENPARITY*/);

	void AddSendMessage(BYTE* buffer,unsigned long len,PCOM_INFO pComInfo,PSYSTEMTIME pSt=NULL);    //添加下发日志信息到日志系统
public:
	BOOL enframe_link(BYTE protocolType);  //protocolType 1 水文， 2 水资源
	BOOL enframe_link_WaterResources();    //水资源
	BOOL enframe_link_Hydrology();         //水文 
	BOOL enframe_link_SYQ();// 水雨情
	BOOL ParseLinkFrameCom_Msg();
	BOOL ParseLinkFrameCom_SYQ();//水雨情数据校验
	BOOL ParseLinkFrameCOM_Write();
	int  getFrameBuffer(BYTE* frBuf);       //获取帧报文
protected:
	//CRtuStation* m_pRelate_Station;			//此通信接口关联的遥测终端对象
	CList<CRtuStation*> m_lstRelateStations;		//此通信接口关联的遥测终端机列表
public:
	//void         SetRelateStation(CRtuStation* pSub);
	//CRtuStation* GetRelateStation();
public:
	void AddRelateStation(CRtuStation* pSub);		//添加一个与此通信接口关联的遥测终端对象
	void RemoveRelateStation(CRtuStation* pSub);    //删除一个与通信接口关联的遥测终端对象
};
