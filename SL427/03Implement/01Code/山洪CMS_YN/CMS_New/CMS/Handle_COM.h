#pragma once
#include "UdfGlobalFUNC.h"
class CRtuStation;
//���нӿ�Class

class CHandle_COM
{
public:
	CHandle_COM(void);
	HANDLE m_COM_HANDLE;     //���ھ��

public:
	virtual ~CHandle_COM(void);
public:
	unsigned char RECV_BUF[RECV_BUFFER_LEN];    //�������ݻ�����
	unsigned int  RECV_len;                     //�������ݿ��
public:
	unsigned char FRAME_BUF[SEND_BUFFER_LEN];   //֡����
	unsigned int  FRAME_Len;                    //һ֡����
    BYTE m_bAppProtoType;
	int m_recvFrameType;
	int m_msgMode;
	int m_DataOrWriteFlag;

public:
	void SetAppProtoType(BYTE type);
	BYTE GetAppProtoType();
	static unsigned char Add_Sum_Check(unsigned char *data, unsigned char data_len);
	void clearCOM();

	//��ս���Buffer [RECV_BUF]
	void clearBuf();
public:
	void recvDATA(unsigned char *buffer, unsigned int len);
public:
	//�Ӵ��ڽ�������
	unsigned long recvCOM(unsigned char *buffer, unsigned long  len);

	//ͨ�����ڷ�������
	void sendCOM(unsigned char *buffer,unsigned long len);

	//��鴮���Ƿ������ݽ���
	//����ֵΪ�����ֽ���
	unsigned long lookCOMBuf();

	//��ʼ������
	//name :    �������� 'COM1'�� '\\.\COM29'
	//Baund:    ������
	//DataBits: ����λ
	//StopBits: ֹͣλ
	//Partity:  У���־  ��NOPARITY    0 �޼��飩����ODDPARITY   1 ��У�飩����EVENPARITY  2 żУ�飩����MARKPARITY  3���� ��SPACEPARITY 4��
	BOOL initCOM(CString name,int Baund=CBR_9600,int DataBits=8,int StopBits=ONESTOPBIT,int Parity=NOPARITY/*EVENPARITY*/);

	void AddSendMessage(BYTE* buffer,unsigned long len,PCOM_INFO pComInfo,PSYSTEMTIME pSt=NULL);    //����·���־��Ϣ����־ϵͳ
public:
	BOOL enframe_link(BYTE protocolType);  //protocolType 1 ˮ�ģ� 2 ˮ��Դ
	BOOL enframe_link_WaterResources();    //ˮ��Դ
	BOOL enframe_link_Hydrology();         //ˮ�� 
	BOOL enframe_link_SYQ();// ˮ����
	BOOL ParseLinkFrameCom_Msg();
	BOOL ParseLinkFrameCom_SYQ();//ˮ��������У��
	BOOL ParseLinkFrameCOM_Write();
	int  getFrameBuffer(BYTE* frBuf);       //��ȡ֡����
protected:
	//CRtuStation* m_pRelate_Station;			//��ͨ�Žӿڹ�����ң���ն˶���
	CList<CRtuStation*> m_lstRelateStations;		//��ͨ�Žӿڹ�����ң���ն˻��б�
public:
	//void         SetRelateStation(CRtuStation* pSub);
	//CRtuStation* GetRelateStation();
public:
	void AddRelateStation(CRtuStation* pSub);		//���һ�����ͨ�Žӿڹ�����ң���ն˶���
	void RemoveRelateStation(CRtuStation* pSub);    //ɾ��һ����ͨ�Žӿڹ�����ң���ն˶���
};
