#pragma once
#include "DataTran_Base.h"
     

//�ɼ����ݴ���
//����SQLite���ݴ洢
//�̳�:CDataTranBase
class CDataTranSQLite_Local
	:public CDataTran_Base
{
public:
	CDataTranSQLite_Local();
	~CDataTranSQLite_Local();
public: 
	virtual long ThreadFunction();
	virtual void SetDataSource(CString strSource);
	void Initialization();
protected:
	bool OpenSqlite();

public:
	CDbSQLite m_sqlite;
protected:

	void  InitSQLiteDataBase();
	bool  SaveDataToSQLite(COneData_INFO* pData);

	bool  IsExistTable(CString strTable);													   //�����Ƿ����
protected:
	bool     SaveWaterLevelData(COneData_INFO* pData);                                         //�洢ˮλ�ɼ�����
	bool	 SaveWaterPressData(COneData_INFO* pData);										   //�洢ˮѹ�ɼ�����
	bool     SaveWaterQualityData(COneData_INFO* pData);									   //�洢ˮ�ʲɼ�����
	bool     SaveWaterFlowData(COneData_INFO* pData);										   //�洢�����ɼ�����
	bool     SaveRainData(COneData_INFO* pData);											   //�洢�����ɼ�����
	bool     SaveOperateRecord(COneData_INFO* pData);										   //�洢������¼����
	bool     SaveAnalogData(COneData_INFO* pData);											   //�洢ģ�����ɼ�����
	bool     SaveDisplaceData(COneData_INFO* pData);										   //�洢λ�����ɼ�����
	bool     SaveTemperaData(COneData_INFO* pData);											   //�洢�¶����ɼ�����
	bool     SavePulseData(COneData_INFO* pData);											   //�洢������ɼ�����
	bool     SaveAlarmRecord(COneData_INFO* pData);											   //�洢������¼��Ϣ
	bool	 SavePeaceReport( COneData_INFO* pData );										   //�洢ÿ��ƽ����

	//�洢������ص�������Ϣ
	bool     Save_Alarm_WaterLevel(COneData_INFO* pData);										//�洢����ˮλ��Ϣ
	bool     Save_Alarm_WaterPress(COneData_INFO* pData);										//�洢����ˮѹ��Ϣ
	bool     Save_Alarm_WaterQuality(COneData_INFO* pData);										//�洢����ˮ����Ϣ
	bool	 Save_Alarm_Flow(COneData_INFO* pData);												//�洢����������Ϣ
	bool	 Save_Alarm_Rain(COneData_INFO* pData);												//�洢����������Ϣ
	bool	 Save_Alarm_Analog(COneData_INFO* pData);											//�洢����ģ������Ϣ
	bool	 Save_Alarm_Displace(COneData_INFO* pData);											//�洢����λ����Ϣ
	bool	 Save_Alarm_Tempera(COneData_INFO* pData);											//�洢�����¶���Ϣ
private:
	void CheckSQLiteDatabase();
	bool ExcuteNoQuery(CString strSQL);															//ִ�в���ѯ
    //CSqlStatement* ExcuteQuery(CString strSQL);												//��ѯ

	bool DeleteDataOnTimer();																	//��ʱɾ������
	long m_nSaveMonth;																			//����������ݵ��·ݣ�Ĭ��6����,���꣬�������������ڵ� 
	CString m_strDataSource;

	//Add 20140811 By HB
private:
	CCriticalSection m_cs_Sqlite;																//SQLite����������

	SYSTEMTIME    m_stBeginTransaction;															//���������ʱ��

	void SQLiteBeginTransaction();																//��������
	void SQLiteCommitTransaction();																//�ύ����
public:
	bool m_blStartTransaction ;																	//�Ƿ��������ݿ�����,false :δ�������������Ѿ��ύ��true:�Ѿ���������������δ�ύ
	void SQLiteCS_Lock();																		//��������
	void SQLiteCS_UnLock();																		//����
	void PragmaSQLite();																		//SQLiteѡ������
private:
	DWORD m_dwPreBeginTcTransaction;															//��������ʱ��TickCount
	DWORD m_dwLocalSaveInterval;																//����SQLiteд����


};
