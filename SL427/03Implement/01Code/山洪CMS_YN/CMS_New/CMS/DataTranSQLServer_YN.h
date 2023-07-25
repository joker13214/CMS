#pragma once
#include "datatran_base.h"
#include "WaitTimer.h"
#include <map>
#include <list>


//����
struct Table_ST_PPTN_R_Record
{
public:
	CString STCD;  //��վ����
	CString TM;	   //ʱ��
	double DRP;	   //ʱ�ν�ˮ��
	double INTV;   //ʱ�γ�    1.00 /0.05
	double PRD;	   //��ˮ��ʱ
	double DYP;	   //�ս�ˮ��
	CString WTH;   //����״�� 7:���� 8:�޽���
public:
	Table_ST_PPTN_R_Record()
	{
		STCD = "";
		TM = "";
		DRP = 0.0;
		INTV = 0.00;
		PRD = 0.00;
		DYP = 0.0;
		WTH = "";
	}
	const CString ToString();
};
//����ӱ�
struct Table_ST_STORM_R_Record
{
public:
	CString STCD ;	//��վ����  
	CString	TM; 	//ʱ��  	 
	double	STRMDR; //������ʱ  0.15
	double	STRMP;	//������  	
	CString	WTH;	//����״��  7:���� 8:�޽��� 
public:
	Table_ST_STORM_R_Record()
	{
		STCD = "";
		TM = "";
		STRMDR = 0.10;
		STRMP = 0.0;
		WTH = "7";
	}
	const CString ToString();
};

//����ˮ�������ݽӿ�
class CDataTranSQLServer_YN:
	public CDataTran_Base,public CDataTranSQLServer_Base
{
public:
	CDataTranSQLServer_YN(void);
	~CDataTranSQLServer_YN(void);

public:
	virtual long ThreadFunction();


	void Statistics_Data_OnRealTime(COneData_INFO* pData);  //��������ʵʱͳ��
	void StatisticsRain_1Day_OnTimer();                     //��ʱͳ��1�����������

	void Initialization();
	virtual void SetDataSource(CString strSource);
	void SetSqlite(CDbSQLite* sqlite);
protected:	//SQL Server ����	
	bool OpenDatabase();
	bool OpenDatabase(CString dataSource);	
	bool GetConnectionState();
	bool CheckTableExist(CString sTableName); //������ݿ��б��Ƿ����, true:�����,false:������
	bool ExcuteNoQuery(CString sSQL);



private:
	CDbSQLite*  m_sqlite;	

};
