#pragma once
#include <afxmt.h>
#include "UdfGlobalFUNC.h"
#include "WaitTimer.h"
#include "UdfLogSystem.h"


const BYTE AUTO_DATA = 0x01;   //�Ա�ʵʱ����
const BYTE MANU_DATA = 0x02;   //�˹�����

//�ط�����
class RESEND_Object
{
public:
	RESEND_Object()
	{
		m_resendInterval = 0;
		m_CommInterface  = Serial_CommInterface;
	}
	CPacketFrameFormat m_packetFrame;    //֡��ʽ
	BYTE			   m_CommInterface;  //�ӿڣ�����򴮿ڣ�
	NET_INFO           m_ToNetInfo;		 //Ŀ��IP��ַ�Ͷ˿ں�
	CWaitTimer         m_resend_timer;   //����ط���Timer
	int                m_resendInterval; //�ط���ʱ����,��λ:��(second)
};

class CRtuStation;
class CCommThreadInfo;
class COneData_INFO;
class CRtuPropertyDlg;


//ˮ��Դ��ع���ϵͳ���ݴ���
//���ݰ��Ľ���
//US  ���б���/��Ӧ֡
//EN  ���б��� 
class CSysDataTrans
{
public:
	CSysDataTrans(void);
public:
	virtual ~CSysDataTrans(void);
public: 
	void Dispatcher(BYTE* buffer,int bufferLen,BYTE CmInterface=Serial_CommInterface,
		CString FromIP="",int FromPort=0);                      //���Ĵ���Ĭ�ϴ���
	void Dispatcher(CPacketFrameFormat* pWaterFrame);
protected:
	//V2.0 Э������
	void IEC_waterdata_US_03H(CPacketFrameFormat* waterFrame);	 //��·����(RTU �� ����������·)

	void IEC_waterdata_US_02H(CPacketFrameFormat* waterFrame);   //��·���(ȷ��֡)
	void IEC_waterdata_US_50H(CPacketFrameFormat* waterFrame);   //��ѯң���ն˻��м�վ��ַ����Ӧ֡��
	void IEC_waterdata_US_51H(CPacketFrameFormat* waterFrame);   //��ѯң���ն˻��ռ�վʱ�ӣ���Ӧ֡��
	void IEC_waterdata_US_52H(CPacketFrameFormat* waterFrame);   //��ѯң���ն˵Ĺ���ģʽ����Ӧ֡��
	void IEC_waterdata_US_53H(CPacketFrameFormat* waterFrame);   //��ѯң���ն˵������Ա����༰ʱ����(��Ӧ֡��
    void IEC_waterdata_US_54H(CPacketFrameFormat* waterFrame);   //��ѯң��վ���ѯ��ʵʱ�������ࣨ��Ӧ֡��
	void IEC_waterdata_US_55H(CPacketFrameFormat* waterFrame);   //��ѯң���ն�����ɹ���ֵ�������е�ʣ��ˮ������Ӧ֡��
	void IEC_waterdata_US_56H(CPacketFrameFormat* waterFrame);   //��ѯң���ն˵�ʣ��ˮ���ͱ���ֵ����Ӧ֡��
	void IEC_waterdata_US_57H(CPacketFrameFormat* waterFrame);   //��ѯң���ն˵�ˮλ��ֵ��ˮλ�����ޣ���Ӧ֡��
	void IEC_waterdata_US_58H(CPacketFrameFormat* waterFrame);   //��ѯң���ն�ˮѹ�ϡ�����ֵ����Ӧ֡��
	void IEC_waterdata_US_59H(CPacketFrameFormat* waterFrame);   //��ѯң���ն�ˮ�ʲ������ࡢ����ֵ����Ӧ֡��
	void IEC_waterdata_US_5AH(CPacketFrameFormat* waterFrame);   //��ѯң���ն�����ֵˮ�ʲ������ࡢ����ֵ����Ӧ֡��
	void IEC_waterdata_US_1BH(CPacketFrameFormat* waterFrame);   //�����ն�վ�����ı�ף���ʼ��ֵ����Ӧ֡��
	void IEC_waterdata_US_60H(CPacketFrameFormat* waterFrame);   //��ѯ�ն�վת���м������볤ֵ����Ӧ֡��
	void IEC_waterdata_US_61H(CPacketFrameFormat* waterFrame);   //��ѯң���ն�ͼ���¼����Ӧ֡��
	void IEC_waterdata_US_62H(CPacketFrameFormat* waterFrame);   //��ѯ�м�վת���ն˵�ַ����Ӧ֡)
	void IEC_waterdata_US_63H(CPacketFrameFormat* waterFrame);   //��ѯ�м�վ״̬���л���¼����Ӧ֡��
	void IEC_waterdata_US_64H(CPacketFrameFormat* waterFrame);	 //��ѯң��վ�������ݱ���ֵ��ANF��64H��
	void IEC_waterdata_US_65H(CPacketFrameFormat* waterFrame);   //��ѯң��վģ�������������ޣ�ANF��65H��
	void IEC_waterdata_US_66H(CPacketFrameFormat* waterFrame);   //��ѯң���ն���չ����״̬���ն�״̬��ANF��66H��
	void IEC_waterdata_US_67H(CPacketFrameFormat* waterFrame);   //��ѯң���ն���չʵʱֵ��ANF��67H��
	void IEC_waterdata_US_5BH(CPacketFrameFormat* waterFrame);   //��ѯң���ն�ʵʱֵ����Ӧ֡��
	void IEC_waterdata_US_5CH(CPacketFrameFormat* waterFrame);   //��ѯ�ն˵���ʷ�ռ�¼����Ӧ֡��
	void IEC_waterdata_US_5DH(CPacketFrameFormat* waterFrame);   //��ѯң���ն˵��¼���¼����Ӧ֡��
	void IEC_waterdata_US_5EH(CPacketFrameFormat* waterFrame);   //��ѯң���ն�״̬�ͱ���״̬����Ӧ֡��
	void IEC_waterdata_US_5FH(CPacketFrameFormat* waterFrame);   //��ѯˮ�õ��ʵʱ�������ݣ���Ӧ֡��
	void IEC_waterdata_US_92H(CPacketFrameFormat* waterFrame);   //ң������ˮ�û���/բ�ţ���Ӧ֡��
	void IEC_waterdata_US_93H(CPacketFrameFormat* waterFrame);   //ң�عر�ˮ�û���/բ�ţ���Ӧ֡��
	void IEC_waterdata_US_94H(CPacketFrameFormat* waterFrame);   //ң���ն˻��м�վͨ�Ż��л�����Ӧ֡��
	void IEC_waterdata_US_95H(CPacketFrameFormat* waterFrame);   //ң���м�վ�������л�����Ӧ֡��
	void IEC_waterdata_US_30H(CPacketFrameFormat* waterFrame);   //��ң���ն�IC��������Ч����Ӧ֡��
	void IEC_waterdata_US_31H(CPacketFrameFormat* waterFrame);   //ȡ��ң���ն�IC�����ܣ���Ӧ֡��
	void IEC_waterdata_US_32H(CPacketFrameFormat* waterFrame);   //��ֵ���ƹ���Ͷ�루��Ӧ֡��
	void IEC_waterdata_US_33H(CPacketFrameFormat* waterFrame);   //��ֵ���ƹ����˳�����Ӧ֡��
	void IEC_waterdata_US_34H(CPacketFrameFormat* waterFrame);   //��ֵ���趨����Ӧ֡��
	void IEC_waterdata_US_90H(CPacketFrameFormat* waterFrame);   //��λң���ն˲�����״̬�����Ӧ֡��
	void IEC_waterdata_US_91H(CPacketFrameFormat* waterFrame);   //���ң���ն˵���ʷ���ݵ�Ԫ����Ӧ֡��

	//�Ա�����
	void IEC_waterdata_US_80H(CPacketFrameFormat* waterFrame);   //��ʱ�Ա�ʵʱ����
    void IEC_waterdata_US_81H(CPacketFrameFormat* waterFrame);   //����Ա���������
	void IEC_waterdata_US_82H(CPacketFrameFormat* waterFrame);   //�˹�ֵ��
	void IEC_waterdata_US_83H(CPacketFrameFormat* waterFrame);   //��ֵ������չʵʱ����
	void IEC_waterdata_US_84H(CPacketFrameFormat* waterFrame);   //����Ա���չ��������


	//V2.0 �汾�������֦����Ŀ�޸���չ��
	void IEC_waterdata_US_68H(CPacketFrameFormat* waterFrame);	//��ѯң���ն˻���չʵʱֵ����Ӧ֡
	void IEC_waterdata_US_69H(CPacketFrameFormat* waterFrame);  //��ѯң���ն���չ����״̬���ն�״̬_ALL��Ӧ֡
	void IEC_waterdata_US_85H(CPacketFrameFormat* waterFrame);  //��ʱ�Ա�ʵʱ����_ALL
	void IEC_waterdata_US_A0H(CPacketFrameFormat* waterFrame);	//�ն�ÿ��ƽ����

public:
   //V2.0 �汾����
   void IEC_waterdata_EN_03H(CPacketFrameFormat* appFrame);		//��·ȷ��

   //���б��� 
   void IEC_waterdata_EN_02H(CPacketFrameFormat* appFrame);		//������
   void IEC_waterdata_EN_10H(CPacketFrameFormat* appFrame);		//����ң���ն˻��м�վ��ַ
   void IEC_waterdata_EN_11H(CPacketFrameFormat* appFrame);		//����ң���նˡ��м�վʱ��
   void IEC_waterdata_EN_12H(CPacketFrameFormat* appFrame);		//����ң���ն˹���ģʽ
   void IEC_waterdata_EN_13H(CPacketFrameFormat* appFrame);		//���ң���ն˵������Ա����༰ʱ����
   void IEC_waterdata_EN_14H(CPacketFrameFormat* appFrame);		//����ң���ն����ѯ��ʵʱ��������
   void IEC_waterdata_EN_15H(CPacketFrameFormat* appFrame);		//����ң���ն˱��γ�ֵ��
   void IEC_waterdata_EN_16H(CPacketFrameFormat* appFrame);		//����ң���ն�ʣ��ˮ������ֵ
   void IEC_waterdata_EN_17H(CPacketFrameFormat* appFrame);		//����ң���ն˵�ˮλ��ֵ��ˮλ������
   void IEC_waterdata_EN_18H(CPacketFrameFormat* appFrame);		//����ң���ն�ˮѹ�ϡ�����
   void IEC_waterdata_EN_19H(CPacketFrameFormat* appFrame);		//����ң���ն�ˮ�ʲ������ࡢ����ֵ
   void IEC_waterdata_EN_1AH(CPacketFrameFormat* appFrame);		//����ң���ն�ˮ�ʲ������ࡢ����ֵ
   void IEC_waterdata_EN_1BH(CPacketFrameFormat* appFrame);		//�����ն�վ�����ı�ף���ʼ��ֵ
   void IEC_waterdata_EN_1CH(CPacketFrameFormat* appFrame);		//����ң���ն�ת���м������볤
   void IEC_waterdata_EN_1DH(CPacketFrameFormat* appFrame);		//�����м�վת���ն˵�ַ
   void IEC_waterdata_EN_1EH(CPacketFrameFormat* appFrame);		//�����м�վ�������Զ��л����Ա�״̬
   void IEC_waterdata_EN_1FH(CPacketFrameFormat* appFrame);		//����ң���ն���������ֵ
   void IEC_waterdata_EN_20H(CPacketFrameFormat* appFrame);		//����ң���ն�ģ����������ֵ
   //21H-4FH  ����

   void IEC_waterdata_EN_30H(CPacketFrameFormat* appFrame);		//��ң���ն�IC��������Ч
   void IEC_waterdata_EN_31H(CPacketFrameFormat* appFrame);		//ȡ��ң���ն�IC������
   void IEC_waterdata_EN_32H(CPacketFrameFormat* appFrame);		//��ֵ����Ͷ��
   void IEC_waterdata_EN_33H(CPacketFrameFormat* appFrame);		//��ֵ�����˳�
   void IEC_waterdata_EN_34H(CPacketFrameFormat* appFrame);		//��ֵ���趨
   void IEC_waterdata_EN_50H(CPacketFrameFormat* appFrame);		//��ѯң���նˡ��м�վ��ַ
   void IEC_waterdata_EN_51H(CPacketFrameFormat* appFrame);		//��ѯң���նˡ��м�վʱ��
   void IEC_waterdata_EN_52H(CPacketFrameFormat* appFrame);		//��ѯң���ն˹���ģʽ
   void IEC_waterdata_EN_53H(CPacketFrameFormat* appFrame);		//��ѯң���ն˵������Ա����༰ʱ����
   void IEC_waterdata_EN_54H(CPacketFrameFormat* appFrame);		//��ѯң���ն����ѯ��ʵʱ��������
   void IEC_waterdata_EN_55H(CPacketFrameFormat* appFrame);		//��ѯң���ն�����ɹ���ֵ��������ʣ��ˮ��
   void IEC_waterdata_EN_56H(CPacketFrameFormat* appFrame);		//��ѯң���ն�ʣ��ˮ���ͱ���ֵ
   void IEC_waterdata_EN_57H(CPacketFrameFormat* appFrame);		//��ѯң���ն�ˮλ��ֵ��ˮλ������ֵ
   void IEC_waterdata_EN_58H(CPacketFrameFormat* appFrame);		//��ѯң���ն�ˮѹ�ϡ�����
   void IEC_waterdata_EN_59H(CPacketFrameFormat* appFrame);		//��ѯң���ն�ˮ�ʲ������ࡢ����ֵ
   void IEC_waterdata_EN_5AH(CPacketFrameFormat* appFrame);		//��ѯң���ն�ˮ�ʲ������ࡢ����ֵ
   void IEC_waterdata_EN_5BH(CPacketFrameFormat* appFrame);		//��ѯң���ն˵�ʵʱֵ
   void IEC_waterdata_EN_5CH(CPacketFrameFormat* appFrame);		//��ѯң���ն���ʷ�ռ�¼
   void IEC_waterdata_EN_5DH(CPacketFrameFormat* appFrame);		//��ѯң���ն˵��¼���¼
   void IEC_waterdata_EN_5EH(CPacketFrameFormat* appFrame);		//��ѯң���ն�״̬�ͱ���״̬
   void IEC_waterdata_EN_5FH(CPacketFrameFormat* appFrame);		//��ѯˮ�õ��ʵʱ��������
   void IEC_waterdata_EN_60H(CPacketFrameFormat* appFrame);		//��ѯң���ն�״̬�ͱ���״̬
   void IEC_waterdata_EN_61H(CPacketFrameFormat* appFrame);		//��ѯң���ն�ͼ���¼
   void IEC_waterdata_EN_62H(CPacketFrameFormat* appFrame);		//��ѯ�м�վת���ն˵�ַ
   void IEC_waterdata_EN_63H(CPacketFrameFormat* appFrame);		//��ѯ�м�վ������״̬���л���¼
   void IEC_waterdata_EN_64H(CPacketFrameFormat* appFrame);		//��ѯң���ն���������ֵ
   void IEC_waterdata_EN_65H(CPacketFrameFormat* appFrame);		//��ѯң���ն�ģ����������ֵ
   void IEC_waterdata_EN_66H(CPacketFrameFormat* appFrame);		//��ѯң���ն���չ����״̬���ն�״̬
   void IEC_waterdata_EN_67H(CPacketFrameFormat* appFrame);		//��ѯң���ն���չʵʱֵ
   //68H-7FH ����

   void IEC_waterdata_EN_80H(CPacketFrameFormat* appFrame);		//��ʱ�Ա�ʵʱ���ݣ�ȷ��֡��
   void IEC_waterdata_EN_81H(CPacketFrameFormat* appFrame);		//����Ա��������ݣ�ȷ��֡��
   void IEC_waterdata_EN_82H(CPacketFrameFormat* appFrame);		//�˹�������ȷ��֡��
   void IEC_waterdata_EN_83H(CPacketFrameFormat* appFrame);		//��ʱ�Ա���չʵʱ���ݣ���Ӧ֡��
   void IEC_waterdata_EN_84H(CPacketFrameFormat* appFrame);		//����Ա���չ�������ݣ���Ӧ֡��
   void IEC_waterdata_EN_90H(CPacketFrameFormat* appFrame);		//��λң���ն˲�����״̬
   void IEC_waterdata_EN_91H(CPacketFrameFormat* appFrame);		//���ң���ն���ʷ���ݵ�Ԫ
   void IEC_waterdata_EN_92H(CPacketFrameFormat* appFrame);		//ң������ˮ�û���/բ��
   void IEC_waterdata_EN_93H(CPacketFrameFormat* appFrame);		//ң�عر�ˮ�û���/բ��
   void IEC_waterdata_EN_94H(CPacketFrameFormat* appFrame);		//ң���ն˻��м�վͨ�Ż��л�
   void IEC_waterdata_EN_95H(CPacketFrameFormat* appFrame);		//ң���м�վ�������л�


	//V2.0 �汾�������֦����Ŀ�޸���չ��
   void IEC_waterdata_EN_68H(CPacketFrameFormat* appFrame);		//��ѯң���ն���չʵʱֵ
   void IEC_waterdata_EN_69H(CPacketFrameFormat* appFrame);		//��ѯң���ն���չ����״̬���ն�״̬_ALL
   void IEC_waterdata_EN_85H(CPacketFrameFormat* appFrame);		//�Զ��ϱ�ʵʱ����_ALL(ȷ��֡)
   void IEC_waterdata_EN_A0H(CPacketFrameFormat* appFrame);		//�ն�ÿ��ƽ������ȷ��֡
protected:
   int  SendRtuLinkFrame(BYTE* sBuf,int len,CPacketFrameFormat* appFrame);										 //��ң���ն˷��ͱ���
   int  SendRtuLinkFrame(BYTE* sBuf,int len,BYTE CmInterface = Serial_CommInterface,PNET_INFO pToNetInfo = NULL);//��ң���ն˷��ͱ���
   CCriticalSection m_cs_SendData;																				 //�������ݻ������
  
   //����ط�������Ϣ
   void AddReSendObject (BYTE* sBuf,int len,CPacketFrameFormat* appFrame);
   void AddReSendObject (BYTE* sBuf,int len,BYTE CmInterface = Serial_CommInterface,PNET_INFO pToNetInfo = NULL);//���һ����Ҫ�ط��Ķ�����Ϣ(����Ҫ����ط��ĵط����ʴ˺���)
   
   RESEND_Object* GetReSendObject(BYTE Command,BYTE AFN,_ADDR_Info _Addr);			//��m_lstReSendObjects�л�ȡ�ط��Ķ��󲢴��ط��б���ɾ���ö���
   void DeleteOneReSendObject(BYTE Command,BYTE AFN,_ADDR_Info _Addr);				//�յ����ĺ�����ط���������ط�����

   CString BufferToString(BYTE* buffer,int Len);									//������ת��Ϊ�ַ�����Ϣ
   void    BuildSendBuffer(CPacketFrameFormat* packetFrame,BYTE* sBuf,BYTE& len);	//�����·����ĸ�ʽ���ֽ���������
protected:
	CList<RESEND_Object*> m_lstReSendObjects;										//�洢��Ҫ�ط���������Ϣ
	CCriticalSection      m_Mutex_ReSend;											//�������;
	CRtuStation*		  m_pSubStation;											//������ң���ն˶���
public:
	static CString GetFunName(BYTE _DIR,BYTE _CMD);									//��ȡ���������ƣ� _DIR ���䷽��_CMD  ������Ĺ�����
	void    SetRtuStation(CRtuStation* pSub){m_pSubStation = pSub;}					//���ö�Ӧ��ң���ն˶���
	void    AddSendLog(BYTE* buffer,int len);										//����·�������־��Ϣ����Ҫ����վ�����Դ��������־��Ϣ,��Ҫ����CRtuStation���������־��Ϣ
	void    AddRecvLog(BYTE* buffer,int len,CPacketFrameFormat* waterFrame=NULL);   //��ӽ��ձ�����־��Ϣ����Ҫ����վ�����Դ��������־��Ϣ,��Ҫ����CRtuStation���������־��Ϣ
    void    UpdateRtuCommState(CRtuStation* pSub);									//ң���ն�ͨ��״̬��Ϣ����
	void	UpdateRtuCommInfo(CRtuStation* pSub);									//ң���ն˵�ͨ����ص���Ϣ����
	void	ClearReSendObjectList();												//��ձ����ط���������Ҫ��ϧ���������ã���ͨ���ж�ʱ����
protected:
	void BuildBasicDataInfo(COneData_INFO* pData,CPacketFrameFormat* packetFrame);  //���ɱ������ݵĻ�����Ϣ
	void InsertOneSaveData(COneData_INFO* pData);										
public:
	//�ط�������
	void  CheckResendObject();														//�����Ҫ�ط��Ķ����ж��Ƿ���Ҫ����������Ϣ,CommServer�����̵߳���

	//����RTU�����Խ���
protected:
	void  AddDataMessage(CRtuStation* pRtu,COneData_INFO* pData,BYTE nType=AUTO_DATA);
	void  AddDataMessage(CRtuStation* pRTU,CString strDataMessage,PSYSTEMTIME pTime=NULL);													//���������Ϣ��datashow����
	void  AddLogMessage(CRtuStation* pRTU,CString strLogMessage, _Log_level_type leveType =Normal_Log_Message, PSYSTEMTIME pTime = NULL);   //�����־��Ϣ����������ʾ
	void  UpdateAttribFrame(CRtuStation* pRTU,int attrType);																				//��������ҳ������Ϣ��ʱ����/���óɹ�
	void  UpdateRtuClockString(CRtuStation* pRTU,CString strClock);																			//�������Խ����ʱ����Ϣ
	void  UpdateRtuDayHistoryDlg(CRtuStation* pRTU,BOOL blSucc,CString sDayHisInfo);														//�������Դ��ڵ���ʷ�ռ�¼��Ϣ
	CRtuPropertyDlg* GetRtuPropertyDlg();																									//��ȡң���ն����Դ���
};
