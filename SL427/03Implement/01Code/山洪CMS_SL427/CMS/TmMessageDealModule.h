#pragma once


class CScModuleBase;
class CRtuStation;

//����ͨ�Ž��յ��Ĵ�����
//����:Ӧ�ò㱨��
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

	void RecvMessage(BYTE* buf,int Len);		//�Խ��յĵ�����Ϣ���д���
	bool SendMessage(BYTE* buf,int Len);     //ͨ���������⴫������ 
protected:
	CScModuleBase* m_pTmModule;				//����ģ��
	CString m_phoneNum;						//����ͨ��:Զ�̵�ַ;

	BYTE m_RECV_BUF[4096];					//���ǵ��ı������ݻ�����
	int  m_RECV_Len;						//���ǵ������ݳ���
	int  m_MaxLen;							//�������ܱ������ݵ������ĳ���

	void Init();							//Init
	void DealMessage();						//����������ı�����Ϣ
	void AnalyzePacket( unsigned char* FRAME_BUF, unsigned int FRAME_Len );   //Ӧ�ò㱨�Ľ���
public:

	void SetSubStation(CRtuStation* pSub);
	CRtuStation* GetSubStatin();
private:
	CRtuStation* m_pSubStation;				//�뱾ģ������վ;
};
