#pragma once


#include <afxdb.h>
#include <afxdao.h>
#include "UdfGlobalFUNC.h"

#include "RtuStation.h"
#include "UdfAdDivCode.h"
#include "UdfLogSystem.h"
#include "UserObject.h"

//ʵʱ���ݿ�
class CRealTimeDB
{
public:
	CRealTimeDB(void);
	CRealTimeDB(CString _ConfigFile);

	//��ʼ��ʵʱ���ݿ�
	BOOL InitRealTimeDB();
	BOOL BuildRealTimeDB();  
	BOOL BuildUsersList(CDatabase* pDb);
	BOOL BuildRtuStations(CDatabase* pDb);
	BOOL ReBuildRealTimeDB();

	CPtrList* GetRtuStList(){return &m_lstRtuStations;}
	CList<CUserObject*>* GetUsersList() {return &m_lstUsers;}

	BOOL ConnectDB();								 //�������ݿ�
	BOOL CreateTables();							 //���������ļ����ݿ�ṹ��Create Tables��
	BOOL CreatertuStation_extTable();

	void SetLogSystem(CUdfLogSystem* pLogSystem);	 //����_��־ϵͳ
	BOOL InsertLog(_LOG_MESSAGE_TYPE _logType,CString _LogMessage,_Log_level_type _levelType);   //�洢��־��Ϣ
public:
	virtual ~CRealTimeDB(void);
	void Clear();
	void ClearRtuList();
	void ClearUsersList();


protected:
	CString m_strConfigFile;						//Save Configuration Inforation Database file Name
	CDatabase m_Database;
	CPtrList m_lstRtuStations;
	CUdfLogSystem* m_pLogSystem;					
	CList<CUserObject*> m_lstUsers;					//�û���Ϣ�б�
	CCriticalSection m_RtuList_Mutex;
public:
	CDatabase* GetDatabase();



public:
	CRtuStation*  SearchRtuStation(CString strStCode, int AppProtoType);//Ҫ���ն˻���ַ������վ���б���Ψһ����,ֻ������SCSW008Э��
	CRtuStation*  SearchRtuStation(WORD A2);
	CRtuStation*  SearchRtuStation(CString _sID);
	CRtuStation*  SearchRtuStation(WORD A1, DWORD A2);
	CRtuStation*  SearchRtuStation(BYTE ProvinceCode,BYTE RegionCode,DWORD A2);
	CRtuStation*  SearchRtuStation(DWORD stcode,int AppProtoType);
	CRtuStation*  GetRtuStation(int Idx);
	int			  GetRtuStationCount();
	CUdfAdDivCode m_AdDivCodeDb;    //�����������ڴ����ݿ�

	CUserObject*  SearchUserObject(CString _sID);
	//��ӡ�ɾ�����޸Ĳ���
public:
	//���RTU St ��Ϣ
	bool AddRtuStInfo(PRTU_STATION pInfo, CString& r_rtuID);      //���վ��Ϣ
	bool ModifyStInfo(PRTU_STATION pInfo, CRtuStation* pStation); //�޸�վ����Ϣ
	bool DeleteStInfo(CRtuStation* pStation);                     //ɾ��վ����Ϣ
	bool UpdateRtusRealDB(CString rtuID,DB_RECORD_OPERATION_TYPE opType); //��������RTUվ���ڴ�ʵʱ���ݿ����Ϣ

    //��ӡ�ɾ�����޸�ң�����
	bool AddRtuYcInfo(PRTU_YCOBJECT pInfo,CString& r_ycID);		 //���ң����Ϣ
	bool ModifyYcInfo(PRTU_YCOBJECT pInfo,CRtuYcObject* pYcObj); //�޸�ң����Ϣ
	bool DeleteYcInfo(CRtuYcObject* pYcObj);                     //ɾ��ң����Ϣ

    //��ӡ�ɾ�����޸�ң�Ŷ���
    bool AddRtuYxInfo(PRTU_YXOBJECT pInfo,CString& r_yxID);		//���ң����Ϣ
	bool ModifyYxInfo(PRTU_YXOBJECT pInfo,CRtuYxObject* pYxObj); //�޸�ң����Ϣ
	bool DeleteYxInfo(CRtuYxObject* pYxObj);                     //ɾ��ң����Ϣ

	//��ӡ�ɾ�����޸�ң�Ŷ���
	bool AddRtuYkInfo(PRTU_YKOBJECT pInfo,CString& r_ykID);		 //���ң�ؼ�¼
	bool ModifyYkInfo(PRTU_YKOBJECT pInfo,CRtuYkObject* pYkObj); //�޸�ң�ؼ�¼
	bool DeleteYkInfo(CRtuYkObject* pYkObj);                     //ɾ��ң�ؼ�¼   

	//��ӡ�ɾ�����޸Ķ�ֵ����
	bool AddRtuZdInfo(PRTU_ZDOBJECT pInfo,CString& r_zdID);		 //��Ӷ�ֵ��¼
	bool ModifyZdInfo(PRTU_ZDOBJECT pInfo,CRtuZdObject* pZdObj); //�޸Ķ�ֵ��¼
	bool DeleteZdInfo(CRtuZdObject* pZdObj);                     //ɾ����ֵ��¼


	//��ӡ�ɾ�����޸��û���Ϣ
	bool AddUserInfo(PUSER_INFO pUserInfo);
	bool ModifyUserInfo(PUSER_INFO pUserInfo,CUserObject* pUser);
	bool deleteUserInfo(CUserObject* pUser);
	bool CheckUserExist(CString sID,CUserObject* pCurUser = NULL);  //true ��ʾ���ڣ�false ��ʾ������


	//����µ�RTU ʱ����Ƿ����ظ���RTU��Ϣ
	//����ֵ�� true �Ѿ��洢�� false ������
	bool  CheckRtuStExist(PRTU_STATION pInfo);
	bool  CheckRtuStExist(BYTE pCode,BYTE rCode,DWORD A2);
	bool  CheckRtuStExist(CString strStCode);
	bool  CheckRtuStExist(CString strStCode, CRtuStation *pCurStation);
	bool  CheckCenterStNo(int centercode, CRtuStation *pCurStation);//ˮ����
	bool  CheckSYQADDExist(int strAddr);//ˮ����
	bool  CheckSYQADDExist(int strAddr, CRtuStation *pCurStation);//ˮ����

	//�޸�ʱRTU��Ϣʱ ����Ƿ����ظ���RTU��Ϣ
	//����ֵ�� true �Ѿ��洢�� false ������
	bool  CheckRtuStExist(PRTU_STATION pInfo,CRtuStation* pCurStation);
	bool  CheckRtuStExist(BYTE pCode,BYTE rCode,DWORD A2,CRtuStation* pCurStation);
private:
	//����ʵʱ���ݿ�RealDB(�ڴ��еĶ���)
	bool         DeleteRealDBRtu(CString rtuID);		//ɾ�������ܵ�rtu������Ϣ
	CRtuStation* QueryRtuInDb(CString rtuID);			//�����ݿ����RTUվ��Ϣ
	bool         AddRtuToRealDB(CRtuStation* pObjNew);	//��Rtu����������µ�Rtu����
	void		 SaveRtuInforOnExit();                  //�˳�ʱ�������е�RTU��Ϣ

};
