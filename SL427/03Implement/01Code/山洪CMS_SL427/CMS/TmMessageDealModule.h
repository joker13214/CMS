#pragma once


class CScModuleBase;
class CRtuStation;

//卫星通信接收电文处理类
//电文:应用层报文
class CTmMessageDealModule
{
public:
	CTmMessageDealModule(void);
	CTmMessageDealModule(CString rAddr,CScModuleBase* pScModule);
	~CTmMessageDealModule(void);

public:
	void SetTmModule(CScModuleBase* pScModule);
	CScModuleBase* GetTmModule();

	void SetRemoteAddress(CString rAddr);
	const CString GetRemoteAddress();

	void RecvMessage(BYTE* buf,int Len);		//对接收的电文信息进行处理
	bool SendMessage(BYTE* buf,int Len);     //通过卫星向外传送数据 
protected:
	CScModuleBase* m_pTmModule;				//处理模块
	CString m_phoneNum;						//卫星通信:远程地址;

	BYTE m_RECV_BUF[4096];					//卫星电文报文数据缓冲区
	int  m_RECV_Len;						//卫星电文数据长度
	int  m_MaxLen;							//缓冲区能保存数据的最大电文长度

	void Init();							//Init
	void DealMessage();						//解析处理电文报文信息
	void AnalyzePacket( unsigned char* FRAME_BUF, unsigned int FRAME_Len );   //应用层报文解析
public:

	void SetSubStation(CRtuStation* pSub);
	CRtuStation* GetSubStatin();
private:
	CRtuStation* m_pSubStation;				//与本模块管理的站;
};
