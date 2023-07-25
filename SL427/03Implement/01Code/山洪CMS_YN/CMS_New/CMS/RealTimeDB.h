#pragma once


#include <afxdb.h>
#include <afxdao.h>
#include "UdfGlobalFUNC.h"

#include "RtuStation.h"
#include "UdfAdDivCode.h"
#include "UdfLogSystem.h"
#include "UserObject.h"

//实时数据库
class CRealTimeDB
{
public:
	CRealTimeDB(void);
	CRealTimeDB(CString _ConfigFile);

	//初始化实时数据库
	BOOL InitRealTimeDB();
	BOOL BuildRealTimeDB();  
	BOOL BuildUsersList(CDatabase* pDb);
	BOOL BuildRtuStations(CDatabase* pDb);
	BOOL ReBuildRealTimeDB();

	CPtrList* GetRtuStList(){return &m_lstRtuStations;}
	CList<CUserObject*>* GetUsersList() {return &m_lstUsers;}

	BOOL ConnectDB();								 //连接数据库
	BOOL CreateTables();							 //创建配置文件数据库结构（Create Tables）
	BOOL CreatertuStation_extTable();

	void SetLogSystem(CUdfLogSystem* pLogSystem);	 //设置_日志系统
	BOOL InsertLog(_LOG_MESSAGE_TYPE _logType,CString _LogMessage,_Log_level_type _levelType);   //存储日志信息
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
	CList<CUserObject*> m_lstUsers;					//用户信息列表
	CCriticalSection m_RtuList_Mutex;
public:
	CDatabase* GetDatabase();



public:
	CRtuStation*  SearchRtuStation(CString strStCode, int AppProtoType);//要求终端机地址在所有站当中必须唯一编码,只是用于SCSW008协议
	CRtuStation*  SearchRtuStation(WORD A2);
	CRtuStation*  SearchRtuStation(CString _sID);
	CRtuStation*  SearchRtuStation(WORD A1, DWORD A2);
	CRtuStation*  SearchRtuStation(BYTE ProvinceCode,BYTE RegionCode,DWORD A2);
	CRtuStation*  SearchRtuStation(DWORD stcode,int AppProtoType);
	CRtuStation*  GetRtuStation(int Idx);
	int			  GetRtuStationCount();
	CUdfAdDivCode m_AdDivCodeDb;    //行政区划码内存数据库

	CUserObject*  SearchUserObject(CString _sID);
	//添加、删除、修改操作
public:
	//添加RTU St 信息
	bool AddRtuStInfo(PRTU_STATION pInfo, CString& r_rtuID);      //添加站信息
	bool ModifyStInfo(PRTU_STATION pInfo, CRtuStation* pStation); //修改站点信息
	bool DeleteStInfo(CRtuStation* pStation);                     //删除站点信息
	bool UpdateRtusRealDB(CString rtuID,DB_RECORD_OPERATION_TYPE opType); //更新数据RTU站的内存实时数据库的信息

    //添加、删除、修改遥测对象
	bool AddRtuYcInfo(PRTU_YCOBJECT pInfo,CString& r_ycID);		 //添加遥测信息
	bool ModifyYcInfo(PRTU_YCOBJECT pInfo,CRtuYcObject* pYcObj); //修改遥测信息
	bool DeleteYcInfo(CRtuYcObject* pYcObj);                     //删除遥测信息

    //添加、删除、修改遥信对象
    bool AddRtuYxInfo(PRTU_YXOBJECT pInfo,CString& r_yxID);		//添加遥信信息
	bool ModifyYxInfo(PRTU_YXOBJECT pInfo,CRtuYxObject* pYxObj); //修改遥信信息
	bool DeleteYxInfo(CRtuYxObject* pYxObj);                     //删除遥信信息

	//添加、删除、修改遥信对象
	bool AddRtuYkInfo(PRTU_YKOBJECT pInfo,CString& r_ykID);		 //添加遥控记录
	bool ModifyYkInfo(PRTU_YKOBJECT pInfo,CRtuYkObject* pYkObj); //修改遥控记录
	bool DeleteYkInfo(CRtuYkObject* pYkObj);                     //删除遥控记录   

	//添加、删除、修改定值对象
	bool AddRtuZdInfo(PRTU_ZDOBJECT pInfo,CString& r_zdID);		 //添加定值记录
	bool ModifyZdInfo(PRTU_ZDOBJECT pInfo,CRtuZdObject* pZdObj); //修改定值记录
	bool DeleteZdInfo(CRtuZdObject* pZdObj);                     //删除定值记录


	//添加、删除、修改用户信息
	bool AddUserInfo(PUSER_INFO pUserInfo);
	bool ModifyUserInfo(PUSER_INFO pUserInfo,CUserObject* pUser);
	bool deleteUserInfo(CUserObject* pUser);
	bool CheckUserExist(CString sID,CUserObject* pCurUser = NULL);  //true 表示存在，false 表示不存在


	//添加新的RTU 时检查是否有重复的RTU信息
	//返回值： true 已经存储， false 不存在
	bool  CheckRtuStExist(PRTU_STATION pInfo);
	bool  CheckRtuStExist(BYTE pCode,BYTE rCode,DWORD A2);
	bool  CheckRtuStExist(CString strStCode);
	bool  CheckRtuStExist(CString strStCode, CRtuStation *pCurStation);
	bool  CheckCenterStNo(int centercode, CRtuStation *pCurStation);//水雨情
	bool  CheckSYQADDExist(int strAddr);//水雨情
	bool  CheckSYQADDExist(int strAddr, CRtuStation *pCurStation);//水雨情

	//修改时RTU信息时 检查是否有重复的RTU信息
	//返回值： true 已经存储， false 不存在
	bool  CheckRtuStExist(PRTU_STATION pInfo,CRtuStation* pCurStation);
	bool  CheckRtuStExist(BYTE pCode,BYTE rCode,DWORD A2,CRtuStation* pCurStation);
private:
	//操作实时数据库RealDB(内存中的对象)
	bool         DeleteRealDBRtu(CString rtuID);		//删除链表总的rtu对象信息
	CRtuStation* QueryRtuInDb(CString rtuID);			//从数据库查找RTU站信息
	bool         AddRtuToRealDB(CRtuStation* pObjNew);	//项Rtu的链表添加新的Rtu对象
	void		 SaveRtuInforOnExit();                  //退出时保存所有的RTU信息

};
