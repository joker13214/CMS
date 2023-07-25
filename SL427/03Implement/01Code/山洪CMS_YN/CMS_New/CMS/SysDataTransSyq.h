#pragma once
#include <afxmt.h>
#include "UdfGlobalFUNC.h"
#include "WaitTimer.h"
#include "UdfLogSystem.h"

class CRtuStation;
class CCommThreadInfo;
class COneData_INFO;
class CRtuPropertyDlg;

//�ط�����
class RESEND_Object_SYQ
{
public:
	RESEND_Object_SYQ()
	{
		m_resendInterval = 0;
		m_CommInterface  = Serial_CommInterface;
	}
	CSYQFrameFormat m_packetFrame;    //֡��ʽ
	BYTE			   m_CommInterface;  //�ӿڣ�����򴮿ڣ�
	NET_INFO           m_ToNetInfo;		 //Ŀ��IP��ַ�Ͷ˿ں�
	CWaitTimer         m_resend_timer;   //����ط���Timer
	int                m_resendInterval; //�ط���ʱ����,��λ:��(second)
};

class CSysDataTrans_Syq
{
public:
	CSysDataTrans_Syq(void);
	~CSysDataTrans_Syq(void);
public:

	//������ת��Ϊ�ַ�����Ϣ
	CString BufferToString(BYTE* buffer,int Len);


	void Dispatcher(CSYQFrameFormat* psyqFrame);


	void SetRtuStation(CRtuStation* pSub)            {m_pSubStation = pSub;}					//���ö�Ӧ��ң���ն˶���

public:
	void AddRecvLog(BYTE* buffer,int len, CSYQFrameFormat* syqFrame=NULL);      //��ӽ���ˮ����Դ��־��Ϣ����Ҫ����վ���Դ��������־��Ϣ����Ҫ����CRtuStation���������־��Ϣ
	void AddSendLog(BYTE* buffer,int len, DWORD stcode);										//����·�������־��Ϣ����Ҫ����վ�����Դ��������־��Ϣ,��Ҫ����CRtuStation���������־��Ϣ
	void UpdateRtuCommState(CRtuStation* pSub);
	void UpdateRtuCommInfo(CRtuStation* pSub);

	void    BuildSendBuffer(CSYQFrameFormat* packetFrame,BYTE* sBuf,BYTE& len,int mlen);	//�����·����ĸ�ʽ���ֽ���������

	int  SendRtuLinkFrame(BYTE* sBuf,int len,BYTE CmInterface = Serial_CommInterface,PNET_INFO pToNetInfo = NULL);//��ң���ն˷��ͱ���
	CCriticalSection m_cs_SendData;

	 void CheckResendObject();   //�ط�������

protected:
	CList<RESEND_Object_SYQ*> m_lstReSendObjects;										//�洢��Ҫ�ط���������Ϣ
	CCriticalSection      m_Mutex_ReSend;											//�������;
	CRtuStation*		  m_pSubStation;											//������ң���ն˶���


	 void AddReSendObject (BYTE* sBuf,int len,CSYQFrameFormat* appFrame);    //

	 RESEND_Object_SYQ* GetReSendObject(BYTE commandtype);			//��m_lstReSendObjects�л�ȡ�ط��Ķ��󲢴��ط��б���ɾ���ö���
	 void DeleteOneReSendObject(BYTE commandtype);				//�յ����ĺ�����ط���������ط�����

	//����RTU�����Խ���
private:
	void AddLogMessage(CRtuStation* pRTU,CString strLogMessage, _Log_level_type leveType =Normal_Log_Message, PSYSTEMTIME pTime = NULL);   //�����־��Ϣ����������ʾ
	void  AddDataMessage(CRtuStation* pRTU,CString strDataMessage,PSYSTEMTIME pTime=NULL);	
	void UpdateAttribFrame(CRtuStation* pRTU, int attrType);
	void RenewCallOperation(CRtuStation* pRTU, int attrType);
	CRtuPropertyDlg* GetRtuPropertyDlg();

	std::list<CString> m_YaosuList; //Ҫ�ر�ʾ
	CCriticalSection m_cs_yaosu;

public:  

	void GetPropertyString(unsigned char *buf, int buflen, CString& str,int& len, int& datalen);//��ȡ�����ַ���
	void GetPropertyStringList(unsigned char *buf, int buflen,int&len, int stcode);//��ȡ�ַ������� 

	void InsertOneSaveData(COneData_INFO* pData);	
//	void GetStringType(CString str; int& type, )

	void Process_SYQ_30H(CSYQFrameFormat* psyqFrame);//���Ա�
	void Process_SYQ_31H(CSYQFrameFormat* psyqFrame);//����ʱ��ˮ����Ϣ
	void Process_SYQ_32H(CSYQFrameFormat* psyqFrame);//ң��վ��ʱ��
	void Process_SYQ_33H(CSYQFrameFormat* psyqFrame);//ң��վ�ӱ���
	void Process_SYQ_34H(CSYQFrameFormat* psyqFrame);//ң��վСʱ��
	void Process_SYQ_35H(CSYQFrameFormat* psyqFrame);//ң��վ�˹�������
	void Process_SYQ_36H(CSYQFrameFormat* psyqFrame);//����վ��ѯң��վʵʱ����
	void Process_SYQ_37H(CSYQFrameFormat* psyqFrame);//����վ��ѯң��վָ��Ҫ��ʵʱ����
	void Process_SYQ_38H(CSYQFrameFormat* psyqFrame);//����վ��ѯң��վʱ������
	void Process_SYQ_39H(CSYQFrameFormat* psyqFrame);//����վ��ѯң��վ�˹�����
	

	void Process_SYQ_40H(CSYQFrameFormat* psyqFrame);//�޸����ò���1
	void Process_SYQ_41H(CSYQFrameFormat* psyqFrame);//��ȡ���ò���1
	void Process_SYQ_42H(CSYQFrameFormat* psyqFrame);//�޸����ò���2
	void Process_SYQ_43H(CSYQFrameFormat* psyqFrame);//��ȡ���ò���2

	void Process_SYQ_44H(CSYQFrameFormat* psyqFrame);//�޸����ò���3
	void Process_SYQ_45H(CSYQFrameFormat* psyqFrame);//��ȡ���ò���3


	void Process_SYQ_46H(CSYQFrameFormat* psyqFrame);//�޸�����
	void Process_SYQ_47H(CSYQFrameFormat* psyqFrame);//��ʼ����̬�洢����
	void Process_SYQ_48H(CSYQFrameFormat* psyqFrame);//ң��վ״̬�ͱ���
	void Process_SYQ_49H(CSYQFrameFormat* psyqFrame);//����

	//����
	void SYQ_EN_COMMON(CSYQFrameFormat* appFrame,int type,BOOL bRepeat);//ͨ��Ӧ����
	void SYQ_EN_37H(CSYQFrameFormat* appFrame,CString yaosustr);//��ѯָ��Ҫ������
	void SYQ_EN_38H(CSYQFrameFormat* appFrame,CString yaosustr,int timetype,int ntime ,CTime starttime, CTime endtime);//��ѯʱ������
	void SYQ_EN_40H(CSYQFrameFormat* appFrame,CString stradd,short comtype,short no);//�޸����ñ�1
	void SYQ_EN_41H(CSYQFrameFormat* appFrame,CString stradd,short comtype,short no);//��ѯ���ñ�1
	void SYQ_EN_42H(CSYQFrameFormat* appFrame,_SYQ_CONFIG_PACKET2 packet);//�޸����ñ�2
	void SYQ_EN_43H(CSYQFrameFormat* appFrame,_SYQ_CONFIG_PACKET2 packet);//��ѯ���ñ�2

	void SYQ_EN_44H(CSYQFrameFormat* appFrame,_SYQ_CONFIG_PACKET3 packet);//�޸����ñ�2
	void SYQ_EN_45H(CSYQFrameFormat* appFrame,_SYQ_CONFIG_PACKET3 packet);//��ѯ���ñ�2


	void SYQ_EN_46H(CSYQFrameFormat* appFrame,int password);//�޸�����
	void SYQ_EN_47H(CSYQFrameFormat* appFrame);//�����̬�洢����
	void SYQ_EN_49H(CSYQFrameFormat* appFrame);//����
};
