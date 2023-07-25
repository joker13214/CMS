#pragma once
#include <afxmt.h>
#include "UdfGlobalFUNC.h"
#include "WaitTimer.h"
#include "UdfLogSystem.h"


const BYTE AUTO_DATA = 0x01;   //自报实时参数
const BYTE MANU_DATA = 0x02;   //人工置数

//重发对象
class RESEND_Object
{
public:
	RESEND_Object()
	{
		m_resendInterval = 0;
		m_CommInterface  = Serial_CommInterface;
	}
	CPacketFrameFormat m_packetFrame;    //帧格式
	BYTE			   m_CommInterface;  //接口（网络或串口）
	NET_INFO           m_ToNetInfo;		 //目的IP地址和端口号
	CWaitTimer         m_resend_timer;   //检查重发的Timer
	int                m_resendInterval; //重发的时间间隔,单位:秒(second)
};

class CRtuStation;
class CCommThreadInfo;
class COneData_INFO;
class CRtuPropertyDlg;


//水资源监控管理系统数据传输
//数据包的解析
//US  上行报文/响应帧
//EN  下行报文 
class CSysDataTrans
{
public:
	CSysDataTrans(void);
public:
	virtual ~CSysDataTrans(void);
public: 
	void Dispatcher(BYTE* buffer,int bufferLen,BYTE CmInterface=Serial_CommInterface,
		CString FromIP="",int FromPort=0);                      //报文处理，默认串口
	void Dispatcher(CPacketFrameFormat* pWaterFrame);
protected:
	//V2.0 协议新增
	void IEC_waterdata_US_03H(CPacketFrameFormat* waterFrame);	 //链路请求(RTU 向 中心请求链路)

	void IEC_waterdata_US_02H(CPacketFrameFormat* waterFrame);   //链路检查(确认帧)
	void IEC_waterdata_US_50H(CPacketFrameFormat* waterFrame);   //查询遥测终端或中继站地址（响应帧）
	void IEC_waterdata_US_51H(CPacketFrameFormat* waterFrame);   //查询遥测终端或终极站时钟（响应帧）
	void IEC_waterdata_US_52H(CPacketFrameFormat* waterFrame);   //查询遥测终端的工作模式（响应帧）
	void IEC_waterdata_US_53H(CPacketFrameFormat* waterFrame);   //查询遥测终端的数据自报种类及时间间隔(响应帧）
    void IEC_waterdata_US_54H(CPacketFrameFormat* waterFrame);   //查询遥测站需查询的实时数据种类（响应帧）
	void IEC_waterdata_US_55H(CPacketFrameFormat* waterFrame);   //查询遥测终端最近成功充值量和现有的剩余水量（响应帧）
	void IEC_waterdata_US_56H(CPacketFrameFormat* waterFrame);   //查询遥测终端的剩余水量和报警值（响应帧）
	void IEC_waterdata_US_57H(CPacketFrameFormat* waterFrame);   //查询遥测终端的水位基值、水位上下限（响应帧）
	void IEC_waterdata_US_58H(CPacketFrameFormat* waterFrame);   //查询遥测终端水压上、下限值（响应帧）
	void IEC_waterdata_US_59H(CPacketFrameFormat* waterFrame);   //查询遥测终端水质参数种类、上限值（响应帧）
	void IEC_waterdata_US_5AH(CPacketFrameFormat* waterFrame);   //查询遥测终端下限值水质参数种类、下限值（响应帧）
	void IEC_waterdata_US_1BH(CPacketFrameFormat* waterFrame);   //设置终端站流量的表底（初始）值（响应帧）
	void IEC_waterdata_US_60H(CPacketFrameFormat* waterFrame);   //查询终端站转发中继引导码长值（响应帧）
	void IEC_waterdata_US_61H(CPacketFrameFormat* waterFrame);   //查询遥测终端图像记录（响应帧）
	void IEC_waterdata_US_62H(CPacketFrameFormat* waterFrame);   //查询中继站转发终端地址（响应帧)
	void IEC_waterdata_US_63H(CPacketFrameFormat* waterFrame);   //查询中继站状态和切换记录（响应帧）
	void IEC_waterdata_US_64H(CPacketFrameFormat* waterFrame);	 //查询遥测站雨量数据报警值（ANF＝64H）
	void IEC_waterdata_US_65H(CPacketFrameFormat* waterFrame);   //查询遥测站模拟量数据上下限（ANF＝65H）
	void IEC_waterdata_US_66H(CPacketFrameFormat* waterFrame);   //查询遥测终端扩展报警状态和终端状态（ANF＝66H）
	void IEC_waterdata_US_67H(CPacketFrameFormat* waterFrame);   //查询遥测终端扩展实时值（ANF＝67H）
	void IEC_waterdata_US_5BH(CPacketFrameFormat* waterFrame);   //查询遥测终端实时值（响应帧）
	void IEC_waterdata_US_5CH(CPacketFrameFormat* waterFrame);   //查询终端的历史日记录（响应帧）
	void IEC_waterdata_US_5DH(CPacketFrameFormat* waterFrame);   //查询遥测终端的事件记录（响应帧）
	void IEC_waterdata_US_5EH(CPacketFrameFormat* waterFrame);   //查询遥测终端状态和报警状态（响应帧）
	void IEC_waterdata_US_5FH(CPacketFrameFormat* waterFrame);   //查询水泵电机实时工作数据（响应帧）
	void IEC_waterdata_US_92H(CPacketFrameFormat* waterFrame);   //遥控启动水泵或阀门/闸门（响应帧）
	void IEC_waterdata_US_93H(CPacketFrameFormat* waterFrame);   //遥控关闭水泵或阀门/闸门（响应帧）
	void IEC_waterdata_US_94H(CPacketFrameFormat* waterFrame);   //遥控终端或中继站通信机切换（响应帧）
	void IEC_waterdata_US_95H(CPacketFrameFormat* waterFrame);   //遥控中继站工作机切换（响应帧）
	void IEC_waterdata_US_30H(CPacketFrameFormat* waterFrame);   //置遥测终端IC卡功能有效（响应帧）
	void IEC_waterdata_US_31H(CPacketFrameFormat* waterFrame);   //取消遥测终端IC卡功能（响应帧）
	void IEC_waterdata_US_32H(CPacketFrameFormat* waterFrame);   //定值控制功能投入（响应帧）
	void IEC_waterdata_US_33H(CPacketFrameFormat* waterFrame);   //定值控制功能退出（响应帧）
	void IEC_waterdata_US_34H(CPacketFrameFormat* waterFrame);   //定值量设定（响应帧）
	void IEC_waterdata_US_90H(CPacketFrameFormat* waterFrame);   //复位遥测终端参数和状态命令（响应帧）
	void IEC_waterdata_US_91H(CPacketFrameFormat* waterFrame);   //清空遥测终端的历史数据单元（响应帧）

	//自报数据
	void IEC_waterdata_US_80H(CPacketFrameFormat* waterFrame);   //定时自报实时数据
    void IEC_waterdata_US_81H(CPacketFrameFormat* waterFrame);   //随机自报报警数据
	void IEC_waterdata_US_82H(CPacketFrameFormat* waterFrame);   //人工值数
	void IEC_waterdata_US_83H(CPacketFrameFormat* waterFrame);   //定值自曝扩展实时数据
	void IEC_waterdata_US_84H(CPacketFrameFormat* waterFrame);   //随机自报扩展报警数据


	//V2.0 版本，针对攀枝花项目修改扩展的
	void IEC_waterdata_US_68H(CPacketFrameFormat* waterFrame);	//查询遥测终端机扩展实时值的响应帧
	void IEC_waterdata_US_69H(CPacketFrameFormat* waterFrame);  //查询遥测终端扩展报警状态和终端状态_ALL响应帧
	void IEC_waterdata_US_85H(CPacketFrameFormat* waterFrame);  //定时自报实时数据_ALL
	void IEC_waterdata_US_A0H(CPacketFrameFormat* waterFrame);	//终端每日平安报

public:
   //V2.0 版本新增
   void IEC_waterdata_EN_03H(CPacketFrameFormat* appFrame);		//链路确认

   //下行报文 
   void IEC_waterdata_EN_02H(CPacketFrameFormat* appFrame);		//联络检查
   void IEC_waterdata_EN_10H(CPacketFrameFormat* appFrame);		//设置遥测终端或中继站地址
   void IEC_waterdata_EN_11H(CPacketFrameFormat* appFrame);		//设置遥测终端、中继站时钟
   void IEC_waterdata_EN_12H(CPacketFrameFormat* appFrame);		//设置遥测终端工作模式
   void IEC_waterdata_EN_13H(CPacketFrameFormat* appFrame);		//设计遥测终端的数据自报种类及时间间隔
   void IEC_waterdata_EN_14H(CPacketFrameFormat* appFrame);		//设置遥测终端需查询的实时数据种类
   void IEC_waterdata_EN_15H(CPacketFrameFormat* appFrame);		//设置遥测终端本次充值量
   void IEC_waterdata_EN_16H(CPacketFrameFormat* appFrame);		//设置遥测终端剩余水量报警值
   void IEC_waterdata_EN_17H(CPacketFrameFormat* appFrame);		//设置遥测终端的水位基值、水位上下限
   void IEC_waterdata_EN_18H(CPacketFrameFormat* appFrame);		//设置遥测终端水压上、下限
   void IEC_waterdata_EN_19H(CPacketFrameFormat* appFrame);		//设置遥测终端水质参数种类、上限值
   void IEC_waterdata_EN_1AH(CPacketFrameFormat* appFrame);		//设置遥测终端水质参数种类、下限值
   void IEC_waterdata_EN_1BH(CPacketFrameFormat* appFrame);		//设置终端站流量的表底（初始）值
   void IEC_waterdata_EN_1CH(CPacketFrameFormat* appFrame);		//设置遥测终端转发中继引导码长
   void IEC_waterdata_EN_1DH(CPacketFrameFormat* appFrame);		//设置中继站转发终端地址
   void IEC_waterdata_EN_1EH(CPacketFrameFormat* appFrame);		//设置中继站工作机自动切换，自报状态
   void IEC_waterdata_EN_1FH(CPacketFrameFormat* appFrame);		//设置遥测终端雨量报警值
   void IEC_waterdata_EN_20H(CPacketFrameFormat* appFrame);		//设置遥测终端模拟量上下限值
   //21H-4FH  备用

   void IEC_waterdata_EN_30H(CPacketFrameFormat* appFrame);		//置遥测终端IC卡功能有效
   void IEC_waterdata_EN_31H(CPacketFrameFormat* appFrame);		//取消遥测终端IC卡功能
   void IEC_waterdata_EN_32H(CPacketFrameFormat* appFrame);		//定值控制投入
   void IEC_waterdata_EN_33H(CPacketFrameFormat* appFrame);		//定值控制退出
   void IEC_waterdata_EN_34H(CPacketFrameFormat* appFrame);		//定值量设定
   void IEC_waterdata_EN_50H(CPacketFrameFormat* appFrame);		//查询遥测终端、中继站地址
   void IEC_waterdata_EN_51H(CPacketFrameFormat* appFrame);		//查询遥测终端、中继站时钟
   void IEC_waterdata_EN_52H(CPacketFrameFormat* appFrame);		//查询遥测终端工作模式
   void IEC_waterdata_EN_53H(CPacketFrameFormat* appFrame);		//查询遥测终端的数据自报种类及时间间隔
   void IEC_waterdata_EN_54H(CPacketFrameFormat* appFrame);		//查询遥测终端需查询的实时数据种类
   void IEC_waterdata_EN_55H(CPacketFrameFormat* appFrame);		//查询遥测终端最近成功充值量和现有剩余水量
   void IEC_waterdata_EN_56H(CPacketFrameFormat* appFrame);		//查询遥测终端剩余水量和报警值
   void IEC_waterdata_EN_57H(CPacketFrameFormat* appFrame);		//查询遥测终端水位基值、水位上下限值
   void IEC_waterdata_EN_58H(CPacketFrameFormat* appFrame);		//查询遥测终端水压上、下限
   void IEC_waterdata_EN_59H(CPacketFrameFormat* appFrame);		//查询遥测终端水质参数种类、上限值
   void IEC_waterdata_EN_5AH(CPacketFrameFormat* appFrame);		//查询遥测终端水质参数种类、下限值
   void IEC_waterdata_EN_5BH(CPacketFrameFormat* appFrame);		//查询遥测终端的实时值
   void IEC_waterdata_EN_5CH(CPacketFrameFormat* appFrame);		//查询遥测终端历史日记录
   void IEC_waterdata_EN_5DH(CPacketFrameFormat* appFrame);		//查询遥测终端的事件记录
   void IEC_waterdata_EN_5EH(CPacketFrameFormat* appFrame);		//查询遥测终端状态和报警状态
   void IEC_waterdata_EN_5FH(CPacketFrameFormat* appFrame);		//查询水泵电机实时工作数据
   void IEC_waterdata_EN_60H(CPacketFrameFormat* appFrame);		//查询遥测终端状态和报警状态
   void IEC_waterdata_EN_61H(CPacketFrameFormat* appFrame);		//查询遥测终端图像记录
   void IEC_waterdata_EN_62H(CPacketFrameFormat* appFrame);		//查询中继站转发终端地址
   void IEC_waterdata_EN_63H(CPacketFrameFormat* appFrame);		//查询中继站工作机状态和切换记录
   void IEC_waterdata_EN_64H(CPacketFrameFormat* appFrame);		//查询遥测终端雨量报警值
   void IEC_waterdata_EN_65H(CPacketFrameFormat* appFrame);		//查询遥测终端模拟量上下限值
   void IEC_waterdata_EN_66H(CPacketFrameFormat* appFrame);		//查询遥测终端扩展报警状态和终端状态
   void IEC_waterdata_EN_67H(CPacketFrameFormat* appFrame);		//查询遥测终端扩展实时值
   //68H-7FH 备用

   void IEC_waterdata_EN_80H(CPacketFrameFormat* appFrame);		//定时自报实时数据（确认帧）
   void IEC_waterdata_EN_81H(CPacketFrameFormat* appFrame);		//随机自报报警数据（确认帧）
   void IEC_waterdata_EN_82H(CPacketFrameFormat* appFrame);		//人工置数（确认帧）
   void IEC_waterdata_EN_83H(CPacketFrameFormat* appFrame);		//定时自报扩展实时数据（响应帧）
   void IEC_waterdata_EN_84H(CPacketFrameFormat* appFrame);		//随机自报扩展报警数据（响应帧）
   void IEC_waterdata_EN_90H(CPacketFrameFormat* appFrame);		//复位遥测终端参数和状态
   void IEC_waterdata_EN_91H(CPacketFrameFormat* appFrame);		//清空遥测终端历史数据单元
   void IEC_waterdata_EN_92H(CPacketFrameFormat* appFrame);		//遥控启动水泵或阀门/闸门
   void IEC_waterdata_EN_93H(CPacketFrameFormat* appFrame);		//遥控关闭水泵或阀门/闸门
   void IEC_waterdata_EN_94H(CPacketFrameFormat* appFrame);		//遥控终端或中继站通信机切换
   void IEC_waterdata_EN_95H(CPacketFrameFormat* appFrame);		//遥控中继站工作机切换


	//V2.0 版本，针对攀枝花项目修改扩展的
   void IEC_waterdata_EN_68H(CPacketFrameFormat* appFrame);		//查询遥测终端扩展实时值
   void IEC_waterdata_EN_69H(CPacketFrameFormat* appFrame);		//查询遥测终端扩展报警状态和终端状态_ALL
   void IEC_waterdata_EN_85H(CPacketFrameFormat* appFrame);		//自动上报实时数据_ALL(确认帧)
   void IEC_waterdata_EN_A0H(CPacketFrameFormat* appFrame);		//终端每日平安报的确认帧
protected:
   int  SendRtuLinkFrame(BYTE* sBuf,int len,CPacketFrameFormat* appFrame);										 //向遥测终端发送报文
   int  SendRtuLinkFrame(BYTE* sBuf,int len,BYTE CmInterface = Serial_CommInterface,PNET_INFO pToNetInfo = NULL);//向遥测终端发送报文
   CCriticalSection m_cs_SendData;																				 //发送数据互斥对象
  
   //添加重发对象信息
   void AddReSendObject (BYTE* sBuf,int len,CPacketFrameFormat* appFrame);
   void AddReSendObject (BYTE* sBuf,int len,BYTE CmInterface = Serial_CommInterface,PNET_INFO pToNetInfo = NULL);//添加一个需要重发的对象信息(在需要检查重发的地方访问此函数)
   
   RESEND_Object* GetReSendObject(BYTE Command,BYTE AFN,_ADDR_Info _Addr);			//从m_lstReSendObjects中获取重发的对象并从重发列表中删除该对象
   void DeleteOneReSendObject(BYTE Command,BYTE AFN,_ADDR_Info _Addr);				//收到报文后清除重发链表里的重发对象

   CString BufferToString(BYTE* buffer,int Len);									//将报文转换为字符串信息
   void    BuildSendBuffer(CPacketFrameFormat* packetFrame,BYTE* sBuf,BYTE& len);	//生成下发报文格式，字节序列数据
protected:
	CList<RESEND_Object*> m_lstReSendObjects;										//存储需要重发的数据信息
	CCriticalSection      m_Mutex_ReSend;											//互斥对象;
	CRtuStation*		  m_pSubStation;											//所属的遥测终端对象
public:
	static CString GetFunName(BYTE _DIR,BYTE _CMD);									//获取服务功能名称， _DIR 传输方向，_CMD  控制域的功能码
	void    SetRtuStation(CRtuStation* pSub){m_pSubStation = pSub;}					//设置对应的遥测终端对象
	void    AddSendLog(BYTE* buffer,int len);										//添加下发报文日志信息，主要是向站的属性窗口添加日志信息,主要是向CRtuStation对象添加日志信息
	void    AddRecvLog(BYTE* buffer,int len,CPacketFrameFormat* waterFrame=NULL);   //添加接收报文日志信息，主要是向站的属性窗口添加日志信息,主要是向CRtuStation对象添加日志信息
    void    UpdateRtuCommState(CRtuStation* pSub);									//遥测终端通信状态信息更新
	void	UpdateRtuCommInfo(CRtuStation* pSub);									//遥测终端的通信相关的信息更新
	void	ClearReSendObjectList();												//清空保存重发的链表，主要由惜购函数调用，或当通信中断时调用
protected:
	void BuildBasicDataInfo(COneData_INFO* pData,CPacketFrameFormat* packetFrame);  //生成保存数据的基础信息
	void InsertOneSaveData(COneData_INFO* pData);										
public:
	//重发对象检查
	void  CheckResendObject();														//检查需要重发的对象，判断是否需要发送数据信息,CommServer服务线程调用

	//更新RTU的属性界面
protected:
	void  AddDataMessage(CRtuStation* pRtu,COneData_INFO* pData,BYTE nType=AUTO_DATA);
	void  AddDataMessage(CRtuStation* pRTU,CString strDataMessage,PSYSTEMTIME pTime=NULL);													//添加数据信息到datashow窗口
	void  AddLogMessage(CRtuStation* pRTU,CString strLogMessage, _Log_level_type leveType =Normal_Log_Message, PSYSTEMTIME pTime = NULL);   //添加日志信息到窗口中显示
	void  UpdateAttribFrame(CRtuStation* pRTU,int attrType);																				//更新属性页窗口信息，时期召/设置成功
	void  UpdateRtuClockString(CRtuStation* pRTU,CString strClock);																			//更新属性界面的时钟信息
	void  UpdateRtuDayHistoryDlg(CRtuStation* pRTU,BOOL blSucc,CString sDayHisInfo);														//更新属性窗口的历史日记录信息
	CRtuPropertyDlg* GetRtuPropertyDlg();																									//获取遥测终端属性窗口
};
