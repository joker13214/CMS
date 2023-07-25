#pragma once

class CSCMessageDealModule;
class CRtuStation;

class CProtocol_ComScHandler
{
public:
	CProtocol_ComScHandler();
	virtual ~CProtocol_ComScHandler();

	void SetParent(CSCMessageDealModule *p){m_pParent = p;}
	void SetRtuStation(CRtuStation *p){m_pSubStation = p;}
	CSCMessageDealModule* GetParent(){return m_pParent;}
	void CopyBuf(BYTE* buf,int Len);
	virtual void DealMessage();
	virtual void AnalyzePacket(unsigned char* FRAME_BUF, unsigned int FRAME_Len);

protected:
	CSCMessageDealModule *m_pParent;
	CRtuStation *m_pSubStation;

public:
	BYTE m_RECV_BUF[4096];					//卫星电文报文数据缓冲区
	int  m_RECV_Len;						//卫星电文数据长度

	//正确帧报文数据
	unsigned char m_FRAME_BUF[SEND_BUFFER_LEN];			//帧报文
	unsigned int  m_FRAME_Len;							//一帧长度

};


class CSl427_ComScHandler :public CProtocol_ComScHandler
{
public:
	CSl427_ComScHandler();
	virtual ~CSl427_ComScHandler();

	virtual void DealMessage();
	virtual void AnalyzePacket(unsigned char* FRAME_BUF, unsigned int FRAME_Len);

};


class CSCSW008_ComScHandler :public CProtocol_ComScHandler
{
public:
	CSCSW008_ComScHandler();
	virtual ~CSCSW008_ComScHandler();

	virtual void DealMessage();
	virtual void AnalyzePacket(unsigned char* FRAME_BUF, unsigned int FRAME_Len);
	void AnalyzePacket_write(unsigned char* FRAME_BUF, unsigned int FRAME_Len);
	BOOL ParseLinkFrameMsg_Com();
	BOOL ParseLinkFrameWrite_Com();

public:
	int m_nDivCnt;
	int m_recvFrameType;
	int m_msgMode;

};