#pragma once
#include "datatran_base.h"

const BYTE Database_Interface = 1;						//ֱ�ӷ������ݿ�ķ�ʽ
const BYTE WebService_Interface = 2;					//ͨ������WebService�ӿڵķ�ʽ


class CDataTranSQLServer_GM :
	public CDataTran_Base,public CDataTranSQLServer_Base
{
public:
	CDataTranSQLServer_GM();
	~CDataTranSQLServer_GM();
public:
	virtual long ThreadFunction();
	virtual void SetDataSource(CString strSource);
protected:
	//bool     SaveWaterLevelData(COneData_INFO* pData,CDatabase* pDb);                                          //�洢ˮλ�ɼ�����
	//bool	 SaveWaterPressData(COneData_INFO* pData,CDatabase* pDb);										   //�洢ˮѹ�ɼ�����
	//bool     SaveWaterQualityData(COneData_INFO* pData,CDatabase* pDb);										   //�洢ˮ�ʲɼ�����
	//bool     SaveWaterFlowData(COneData_INFO* pData,CDatabase* pDb);										   //�洢�����ɼ�����
	//bool     SaveRainData(COneData_INFO* pData, CDatabase* pDb);											   //�洢�����ɼ�����
	//bool     SaveOperateRecord(COneData_INFO* pData,CDatabase* pDb);										   //�洢������¼����
	//bool     SaveAnalogData(COneData_INFO* pData,CDatabase* pDb);											   //�洢ģ�����ɼ�����
	//bool     SavePulseData(COneData_INFO* pData,CDatabase* pDb);											   //�洢������ɼ�����
	//bool     SaveAlarmRecord(COneData_INFO* pData,CDatabase* pDb);											   //�洢������¼��Ϣ

	////�洢������ص�������Ϣ
	//bool     Save_Alarm_WaterLevel(COneData_INFO* pData,CDatabase* pDb);										//�洢����ˮλ��Ϣ
	//bool     Save_Alarm_WaterPress(COneData_INFO* pData,CDatabase* pDb);										//�洢����ˮѹ��Ϣ
	//bool     Save_Alarm_WaterQuality(COneData_INFO* pData,CDatabase* pDb);										//�洢����ˮ����Ϣ
	//bool	 Save_Alarm_Flow(COneData_INFO* pData,CDatabase* pDb);												//�洢����������Ϣ
	//bool	 Save_Alarm_Rain(COneData_INFO* pData,CDatabase* pDb);												//�洢����������Ϣ
	//bool	 Save_Alarm_Analog(COneData_INFO* pData,CDatabase* pDb);											//�洢����ģ������Ϣ


	bool     SaveWaterLevelData(COneData_INFO* pData);                                         //�洢ˮλ�ɼ�����
	bool	 SaveWaterPressData(COneData_INFO* pData);										   //�洢ˮѹ�ɼ�����
	bool     SaveWaterQualityData(COneData_INFO* pData);									   //�洢ˮ�ʲɼ�����
	bool     SaveWaterFlowData(COneData_INFO* pData);										   //�洢�����ɼ�����
	bool     SaveRainData(COneData_INFO* pData);											   //�洢�����ɼ�����
	bool     SaveOperateRecord(COneData_INFO* pData);										   //�洢������¼����
	bool     SaveAnalogData(COneData_INFO* pData);											   //�洢ģ�����ɼ�����
	bool     SavePulseData(COneData_INFO* pData);											   //�洢������ɼ�����
	bool     SaveAlarmRecord(COneData_INFO* pData);											   //�洢������¼��Ϣ

	//�洢������ص�������Ϣ
	bool     Save_Alarm_WaterLevel(COneData_INFO* pData);										//�洢����ˮλ��Ϣ
	bool     Save_Alarm_WaterPress(COneData_INFO* pData);										//�洢����ˮѹ��Ϣ
	bool     Save_Alarm_WaterQuality(COneData_INFO* pData);										//�洢����ˮ����Ϣ
	bool	 Save_Alarm_Flow(COneData_INFO* pData);												//�洢����������Ϣ
	bool	 Save_Alarm_Rain(COneData_INFO* pData);												//�洢����������Ϣ
	bool	 Save_Alarm_Analog(COneData_INFO* pData);											//�洢����ģ������Ϣ
protected:
	bool OpenDatabase();
	bool OpenDatabase(CString dataSource);	
	bool GetConnectionState();
	bool CheckTableExist(CString sTableName); //������ݿ��б��Ƿ����, true:�����,false:������
	bool ExcuteNoQuery(CString sSQL);         //�����ݿ���ִ��sql
	bool TransByWebService(CString sSQL);	  //ͨ��webserviceִ��sql;

	void Initialization();					  //��ʼ��thead
	bool TransmissionSQL(CString strSQL);	  //ִ��strSQL ,��m_nInterfaceType=1,���͸����ݿ�ִ��,m_nInterfaceType = 2,����webserviceִ��
private:
	void CheckDatabase();					  //������ݿ�

	BYTE m_nInterfaceType;					  //�ӿڷ�ʽ 1:ֱ�Ӵ����ݿ� 2:ͨ��WebService�ӿ�
	CString m_strWebServiceURL;				  //���ӿ���webserviceʱ,��ҪURL
	CString m_strWebServiceUsername;		  //���ӿ���webserviceʱ,��ҪUsername
	CString m_strWebServicePassword;		  //���ӿ���webserviceʱ,��ҪPassword
};
