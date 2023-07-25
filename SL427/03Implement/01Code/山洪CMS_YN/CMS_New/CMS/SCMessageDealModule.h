#pragma once


class CScModuleBase;
class CRtuStation;
class CProtocol_ComScHandler;
class CSCSW008_ComScHandler;
class CSl427_ComScHandler;

//卫星通信接收电文处理类
//电文:应用层报文
class CSCMessageDealModule
{
public:
	CSCMessageDealModule(void);
	CSCMessageDealModule(UINT rAddr,CScModuleBase* pScModule);
	~CSCMessageDealModule(void);

public:
   void SetScModule(CScModuleBase* pScModule);
   CScModuleBase* GetScModule();

   void SetRemoteAddress(UINT rAddr);
   const UINT GetRemoteAddress();

   void RecvMessage(BYTE* buf,int Len);		//对接收的电文信息进行处理
   bool SendMessage(BYTE* buf,int Len);     //通过卫星向外传送数据 
protected:
	CScModuleBase* m_pScModule;				//处理模块
	UINT m_nRemoteAddr;						//卫星通信:远程地址;

	BYTE m_RECV_BUF[4096];					//卫星电文报文数据缓冲区
	int  m_RECV_Len;						//卫星电文数据长度
	int  m_MaxLen;							//缓冲区能保存数据的最大电文长度

	void Init();							//Init
	
	void DealMessage();						//解析处理电文报文信息
	void AnalyzePacket( unsigned char* FRAME_BUF, unsigned int FRAME_Len );   //应用层报文解析
public:
    void SwitchHandlerModel(int AppProtoType);
	void SetSubStation(CRtuStation* pSub);
	CRtuStation* GetSubStatin();
private:
    CRtuStation* m_pSubStation;				//与本模块管理的站;

public:
	CProtocol_ComScHandler *m_pBase;
	CSCSW008_ComScHandler *m_pHandler_SCSW008;
	CSl427_ComScHandler *m_pHandler_SL427;
	
};
