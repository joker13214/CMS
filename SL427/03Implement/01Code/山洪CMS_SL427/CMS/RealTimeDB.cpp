#include "StdAfx.h"
#include "RealTimeDB.h"
#include "CMS.h"
#pragma warning(disable:4995)


CRealTimeDB::CRealTimeDB(void)
{
	m_strConfigFile.Empty();
	m_pLogSystem = NULL;
	try
	{
		if (m_Database.IsOpen())
			m_Database.Close();
	}
	catch (CDBException* e)
	{
		TRACE("CRealTimeDB ERROR:%s\r\n",e->m_strError);
		e->Delete();
	}
}

CRealTimeDB::CRealTimeDB(CString _ConfigFile)
{
	m_strConfigFile = _ConfigFile;
	m_pLogSystem    = NULL;
	try
	{
		if (m_Database.IsOpen())
			m_Database.Close();
	}
	catch (CDBException* e)
	{
		TRACE("CRealTimeDB ERROR:%s\r\n",e->m_strError);
		e->Delete();
	}
}

CRealTimeDB::~CRealTimeDB(void)
{
	SaveRtuInforOnExit();

	if (m_Database.IsOpen())
		m_Database.Close();
	Clear();
}


//************************************
// Method:    InitRealTimeDB
// FullName:  CRealTimeDB::InitRealTimeDB
// Access:    public 
// Returns:   BOOL
// Qualifier:
//************************************
BOOL CRealTimeDB::InitRealTimeDB()
{
	

	//Check file is exist;
	bool blLogDBExist = true;
	if (CUdfGlobalFUNC::IsFileExist(m_strConfigFile) == false)  //配置数据库文件不存在
	{
		blLogDBExist = false;
		int nCount = 0;

		//创建配置数据库文件,创建3次失败报出创建文件失败
		while(nCount<3)
		{
			CDaoDatabase tmpDaoDB;
			tmpDaoDB.Create(m_strConfigFile);

			if (CUdfGlobalFUNC::IsFileExist(m_strConfigFile) == true)
				break;

			nCount ++;
		}
		if(nCount >= 3)
		{
			if (CUdfGlobalFUNC::IsFileExist(m_strConfigFile) == false)
			{
				AfxMessageBox("创建配置数据库文件失败!",MB_OK|MB_ICONERROR);
				return FALSE;
			}
		}
	}

	//与日志数据库建立连接
	if (!ConnectDB())
	{
		return FALSE;
	}

	if (m_Database.IsOpen() && blLogDBExist == false)
	{
		//创建配置数据库表结构
		CreateTables();
	}

	if (!BuildRealTimeDB())
	{
		return FALSE;
	}


	return TRUE;
}


void CRealTimeDB::SetLogSystem(CUdfLogSystem* pLogSystem)
{
	m_pLogSystem = pLogSystem;
}

BOOL CRealTimeDB::InsertLog(_LOG_MESSAGE_TYPE _logType,CString _LogMessage,_Log_level_type _levelType)
{
	BOOL _Result = FALSE;
	if (m_pLogSystem)
		_Result = m_pLogSystem->InsertLogMessage(_logType,_LogMessage,_levelType);

	return _Result;
}


BOOL CRealTimeDB::ConnectDB()
{
	BOOL _Result = TRUE;
	CString _LogMessage = "";
	CString sDsn = "";
	CString sDriver = "MICROSOFT ACCESS DRIVER (*.mdb)";

	try
	{

		sDsn.Format("ODBC;DRIVER={%s};DSN='';DBQ=%s",sDriver,m_strConfigFile);

		if (m_Database.IsOpen())
			m_Database.Close();


		_LogMessage.Format("连接配置数据文件,连接字符串:%s",sDsn);
		InsertLog(SYS_OTHER_RUN_LOG,_LogMessage,Normal_Log_Message);
		

		if (!m_Database.IsOpen())
		{
			//m_DataBase.Open(NULL,FALSE,FALSE,sDsn);
			if (!m_Database.Open(NULL,FALSE,FALSE,sDsn))
			{
				return FALSE;
			}
		}
		else
		{
			return TRUE;
		}
	}
	catch (CDBException* e)
	{
		TRACE("ConnectLogDB error:%s\r\n",e->m_strError);
		_LogMessage.Format("连接配置数据文件失败:%s,错误信息:%s",sDsn,e->m_strError);
		InsertLog(SYS_RUNNING_S_LOG,_LogMessage,Error_Log_message);

		e->Delete();
		return FALSE;
	}

	return _Result;
}


BOOL CRealTimeDB::CreatertuStation_extTable()
{
	CString _strSQL = "";
	try
	{
		//rtuStation_ext table
		_strSQL.Format("CREATE TABLE rtuStation_ext(sID varchar(40),pVer BYTE,scAddr INT,scFactroyType INT,WorkMode BYTE)");
		//_LogMessage.Format("配置数据库操作,SQL:%s",_strSQL);
		//InsertLog(SYS_OTHER_RUN_LOG,_LogMessage,Normal_Log_Message);
		if (m_Database.IsOpen())
			m_Database.ExecuteSQL(_strSQL);

	}
	catch (CDBException* e)
	{
		e->Delete();
		return FALSE;
	}
	return TRUE;
}

BOOL CRealTimeDB::CreatertuStation_extParaTable()
{
	CString _strSQL = "";
	try
	{

		_strSQL.Format("CREATE TABLE rtu_ext_para(sID varchar(40),p_Name varchar(50),p_Value varchar(255))");
		if (m_Database.IsOpen())
			m_Database.ExecuteSQL(_strSQL);
	}
	catch (CDBException* e)
	{
		e->Delete();
		return FALSE;
	}
	return TRUE;
}

BOOL CRealTimeDB::CreateTables()
{
	CString _LogMessage ="";
	CString _strSQL ="";
	try
	{
		
		//rtuStation table
		_strSQL.Format("CREATE TABLE rtuStation (sName varchar(50),sID varchar(40),ProvinceCode BYTE,RegionCode BYTE,CountyCode BYTE,AddrA2 LONG,AddrA3 BYTE,CmType BYTE,Interface BYTE,ComPortA varchar(20),BaundA int,DataBitsA BYTE,StopBitsA BYTE,ParityA BYTE,IpAddrA varchar(20),SockPortA int)");
		_LogMessage.Format("配置数据库操作,SQL:%s",_strSQL);
		InsertLog(SYS_OTHER_RUN_LOG,_LogMessage,Normal_Log_Message);
		if (m_Database.IsOpen())
			m_Database.ExecuteSQL(_strSQL);

		//rtuStation_ext table
		CreatertuStation_extTable();

		CreatertuStation_extParaTable();

		//rtuYcObject table
		_strSQL.Format("CREATE TABLE rtuYcObject (sName varchar(50),sID varchar(40),ycObjID varchar(40),ycCode int, xType BYTE,Displaybits int ,PointBits int,Unit varchar(10), MaxValue FLOAT,MinValue FLOAT)");
		_LogMessage.Format("配置数据库操作,SQL:%s",_strSQL);
		InsertLog(SYS_OTHER_RUN_LOG,_LogMessage,Normal_Log_Message);
		if (m_Database.IsOpen())
			m_Database.ExecuteSQL(_strSQL);

		//rtuYxObject table
		_strSQL.Format("CREATE TABLE rtuYxObject (sName varchar(50),sID varchar(40), yxObjID varchar(40), yxCode int)");
		_LogMessage.Format("配置数据库操作,SQL:%s",_strSQL);
		InsertLog(SYS_OTHER_RUN_LOG,_LogMessage,Normal_Log_Message);
		if (m_Database.IsOpen())
			m_Database.ExecuteSQL(_strSQL);

		//rtuYkObject table
		_strSQL.Format("CREATE TABLE rtuYkObject (sName varchar(50), sID varchar(40),ykObjID varchar(40),ykCode int)");
		_LogMessage.Format("配置数据库操作,SQL:%s",_strSQL);
		InsertLog(SYS_OTHER_RUN_LOG,_LogMessage,Normal_Log_Message);
		if (m_Database.IsOpen())
			m_Database.ExecuteSQL(_strSQL);
       

		//rtuZdObject table
		_strSQL.Format("CREATE TABLE rtuZdObject (sName varchar(50),sID varchar(40),ZdObjID varchar(40),ZdCode int, Sizeof BYTE,OrderInItem BYTE,Unit varchar(10),ScaleMax FLOAT,ScaleMin FLOAT)");
		_LogMessage.Format("配置数据库操作,SQL:%s",_strSQL);
		InsertLog(SYS_OTHER_RUN_LOG,_LogMessage,Normal_Log_Message);
		if (m_Database.IsOpen())
			m_Database.ExecuteSQL(_strSQL);

		//users_tab
		_strSQL.Format("CREATE TABLE users_tab (sName varchar(20),sID varchar(40),sPWD varchar(16),bType BYTE,OnDuty BYTE)");
		_LogMessage.Format("配置数据库操作,SQL:%s",_strSQL);
		InsertLog(SYS_OTHER_RUN_LOG,_LogMessage,Normal_Log_Message);
		if (m_Database.IsOpen())
			m_Database.ExecuteSQL(_strSQL);

		USER_INFO userinfo;
		userinfo.sName = "Admin";
		userinfo.bOnDuty = 1;
		userinfo.bType = 1;
		userinfo.sID = "00001";
		userinfo.sPwd = "admin";
		AddUserInfo(&userinfo);
	}
	catch(CDBException* pe)
	{
		TRACE("CRealTimeDB CreateTables Error:%s\r\n",pe->m_strError);
		_LogMessage.Format("配置数据库操作,SQL:%s, Error:%s",_strSQL,pe->m_strError);
		InsertLog(SYS_RUNNING_S_LOG,_LogMessage,Error_Log_message);
		pe->Delete();
		return FALSE;
	}

	return TRUE;
}


BOOL CRealTimeDB::BuildRealTimeDB()
{
    if (!BuildRtuStations(&m_Database))
	{
		return FALSE;
	}

	if (!BuildUsersList(&m_Database))
	{
		return FALSE;
	}

	return TRUE;
}


BOOL CRealTimeDB::ReBuildRealTimeDB()
{

	Clear();
	BuildRtuStations(&m_Database);

	return TRUE;
}

BOOL CRealTimeDB::BuildUsersList(CDatabase* pDb)
{
	CString sLogMessage = "";
	CString sSQL = "";

	ClearUsersList();
	try
	{
		sSQL.Format("SELECT DISTINCT sName,sID,sPWD,bType,Onduty FROM users_tab ORDER BY sName");
		sLogMessage.Format("配置数据库操作:%s",sSQL);
		InsertLog(SYS_OTHER_RUN_LOG,sLogMessage,Normal_Log_Message);

		CRecordset rs(pDb);
		
		rs.Open(CRecordset::forwardOnly,sSQL);
		CString sTemp = "";

		while(! rs.IsEOF())
		{
			USER_INFO userInfo;

			rs.GetFieldValue("sName",sTemp);
			userInfo.sName = sTemp;

			rs.GetFieldValue("sID",sTemp);
			userInfo.sID = sTemp;

			rs.GetFieldValue("sPWD",sTemp);
			userInfo.sPwd = sTemp;

			rs.GetFieldValue("bType",sTemp);
			userInfo.bType = (BYTE)atoi(sTemp);

			rs.GetFieldValue("Onduty",sTemp);
			userInfo.bOnDuty= (BYTE)atoi(sTemp);

			CUserObject* pObj = new CUserObject(&userInfo);
			if (pObj)
				m_lstUsers.AddTail(pObj);

			rs.MoveNext();
		}
		rs.Close();
	}
	catch (CDBException* pe)
	{		
		TRACE("BuildUsersList Error:%s\r\n",pe->m_strError);
		sLogMessage.Format("配置数据操作,SQL:%s,Error:%s",sSQL,pe->m_strError);
		InsertLog(SYS_RUNNING_S_LOG,sLogMessage,Error_Log_message);
		pe->Delete();
		return FALSE;
	}

	return TRUE;
}


BOOL CRealTimeDB::BuildRtuStations(CDatabase* pDb)
{
	CString sLogMessage ="";
	CString sSQL="";
	try
	{

		//sSQL.Format("SELECT DISTINCT sName,sID, ProvinceCode,RegionCode,CountyCode,AddrA2,AddrA3,CmType,Interface,ComPortA,BaundA,DataBitsA,StopBitsA,ParityA,IpAddrA,SockPortA FROM rtuStation ORDER BY sName ASC");
		sSQL.Format("SELECT DISTINCT sName,sID, ProvinceCode,RegionCode,CountyCode,AddrA2,AddrA3,CmType,Interface,ComPortA,BaundA,DataBitsA,StopBitsA,ParityA,IpAddrA,SockPortA FROM rtuStation ORDER BY ProvinceCode,RegionCode,CountyCode,AddrA2 ASC");

		sLogMessage.Format("配置数据库操作,SQL:%s",sSQL);
		InsertLog(SYS_OTHER_RUN_LOG,sLogMessage,Normal_Log_Message);

		CRecordset rs(pDb);

		rs.Open(CRecordset::forwardOnly,sSQL);

		CString sTemp = "";
		while (!rs.IsEOF())
		{
			RTU_STATION info;
			rs.GetFieldValue("sName",sTemp);
			info._strName = sTemp;

			rs.GetFieldValue("sID",sTemp);
			info._strSId = sTemp;

			//rs.GetFieldValue("AddrA1",sTemp);
			//info._nAddrA1 = atoi(sTemp);
			rs.GetFieldValue("ProvinceCode",sTemp);
			info._bProvinceCode = (BYTE)atoi(sTemp);

			rs.GetFieldValue("RegionCode",sTemp);
			info._bRegionCode = (BYTE)atoi(sTemp);

			rs.GetFieldValue("CountyCode",sTemp);
			info._bCountyCode  = (BYTE)atoi(sTemp);

			info._nAddrA1 = info._bProvinceCode * 100 + info._bRegionCode;

			rs.GetFieldValue("AddrA2",sTemp);
			info._nAddrA2 = (DWORD)atoi(sTemp);

			rs.GetFieldValue("AddrA3",sTemp);
			info._nAddrA3  = (BYTE)atoi(sTemp);

			rs.GetFieldValue("CmType",sTemp);
			info._bCmType = (BYTE)atoi(sTemp);

			rs.GetFieldValue("Interface",sTemp);
			info._bInterface = (BYTE)atoi(sTemp);

			rs.GetFieldValue("ComPortA",sTemp);
			info._CmComInfo.mComName = sTemp;

			rs.GetFieldValue("BaundA",sTemp);
			info._CmComInfo.mBaund = atoi(sTemp);

			rs.GetFieldValue("DataBitsA",sTemp);
			info._CmComInfo.mDataBits = (BYTE)atoi(sTemp);

			rs.GetFieldValue("StopBitsA",sTemp);
			info._CmComInfo.mStopBits = (BYTE)atoi(sTemp);

			rs.GetFieldValue("ParityA",sTemp);
			info._CmComInfo.mParity = (BYTE)atoi(sTemp);

			rs.GetFieldValue("IpAddrA",sTemp);
			info._CmNetInfo.mIpAddress = sTemp;

			rs.GetFieldValue("SockPortA",sTemp);
			info._CmNetInfo.mPort = atoi(sTemp);


			CRtuStation* pObj = new CRtuStation(&info);

			if (pObj)
			{
				pObj->SetParent(this);
				m_lstRtuStations.AddTail((void*)pObj);
			}


			rs.MoveNext();
		}
		rs.Close();


		POSITION rPos = m_lstRtuStations.GetHeadPosition();

		while (rPos != NULL)
		{
			CRtuStation* pObj = (CRtuStation*)m_lstRtuStations.GetNext(rPos);
			
			if (pObj)
			{
				pObj->BuildAllData(pDb);
			}
		}

	}
	catch (CDBException* pe)
	{
		TRACE("BuildRtuStations Error:%s\r\n",pe->m_strError);
		sLogMessage.Format("配置数据操作,SQL:%s,Error:%s",sSQL,pe->m_strError);
		InsertLog(SYS_RUNNING_S_LOG,sLogMessage,Error_Log_message);
		pe->Delete();
		return FALSE;
	}
	return TRUE;
}

CDatabase* CRealTimeDB::GetDatabase()
{
	return &m_Database;
}

void CRealTimeDB::Clear()
{
	ClearUsersList();
	ClearRtuList();
	m_AdDivCodeDb.Clear();
}


void CRealTimeDB::ClearRtuList()
{
	while (!m_lstRtuStations.IsEmpty())
	{
		CRtuStation* pObj = (CRtuStation*)m_lstRtuStations.RemoveHead();
		if (pObj)
		{
			delete pObj;
			pObj  = NULL;
		}
	}
}

void CRealTimeDB::ClearUsersList()
{
	while (!m_lstUsers.IsEmpty())
	{
		CUserObject* pUser= m_lstUsers.RemoveHead();
		if (pUser)
		{
			delete pUser;
			pUser = NULL;
		}
	}
}


//************************************
// Method:    SearchRtuStation
// FullName:  CRealTimeDB::SearchRtuStation
// Access:    public 
// Returns:   CRtuStation*
// Qualifier: 通过ID查找Rtu站对象
// Parameter: CString _sID
//************************************
CRtuStation* CRealTimeDB::SearchRtuStation(CString _sID)
{
	CRtuStation* pObj = NULL;

	m_RtuList_Mutex.Lock();
	POSITION rPos = m_lstRtuStations.GetHeadPosition();
	while (rPos != NULL)
	{
		CRtuStation* _tmpObj = (CRtuStation*)m_lstRtuStations.GetNext(rPos);

		if (_tmpObj && _tmpObj->GetID() == _sID)
		{
			pObj = _tmpObj;
			break;
		}
	}

	m_RtuList_Mutex.Unlock();
	return pObj;
}


CRtuStation*  CRealTimeDB::SearchRtuStation(WORD A1, DWORD A2)
{
	CRtuStation* pObj = NULL;
	m_RtuList_Mutex.Lock();
	POSITION rPos = m_lstRtuStations.GetHeadPosition();
	while (rPos != NULL)
	{
		CRtuStation* _tempObj = (CRtuStation*)m_lstRtuStations.GetNext(rPos);
		if (_tempObj && _tempObj->GetAddrA1() == A1 && 
			A2 == _tempObj->GetAddrA2())
		{
			pObj = _tempObj;
			break;
		}
	}
	m_RtuList_Mutex.Unlock();

	return pObj;
}

CRtuStation*  CRealTimeDB::SearchRtuStation(BYTE ProvinceCode,BYTE RegionCode,DWORD A2)
{
	 CRtuStation* pObj = NULL;

	 m_RtuList_Mutex.Lock();
	 POSITION rPos = m_lstRtuStations.GetHeadPosition();
	 while (rPos != NULL)
	 {
		 CRtuStation* _tempObj =(CRtuStation*)m_lstRtuStations.GetNext(rPos);
		 if (_tempObj && _tempObj->GetProvinceCode() == ProvinceCode  &&
			 _tempObj->GetRegionCode() ==RegionCode && 
			 _tempObj->GetAddrA2() == A2)
		 {
			 pObj = _tempObj;
			 break;;
		 }
	 }
	 m_RtuList_Mutex.Unlock();

	 return pObj;
}

CRtuStation* CRealTimeDB::GetRtuStation(int Idx)
{
	CRtuStation* pObj = NULL;
	
	m_RtuList_Mutex.Lock();
	POSITION rPos = NULL;
	rPos = m_lstRtuStations.FindIndex(Idx);
	if (rPos != NULL)
		pObj = (CRtuStation*)m_lstRtuStations.GetAt(rPos);
	m_RtuList_Mutex.Unlock();

	return pObj;
}

int CRealTimeDB::GetRtuStationCount()
{
	int nCount = 0;
	m_RtuList_Mutex.Lock();
	nCount = (int)m_lstRtuStations.GetCount();
	m_RtuList_Mutex.Unlock();

	return nCount;
}


//************************************
// Method:    AddRtuStInfo
// FullName:  CRealTimeDB::AddRtuStInfo
// Access:    public 
// Returns:   bool
// Qualifier: 添加Rtu站信息
// Parameter: PRTU_STATION pInfo
//************************************
bool CRealTimeDB::AddRtuStInfo(PRTU_STATION pInfo, CString& r_rtuID)
{
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	CString sLogMessage = "";  // Log messages
	CString strSQL="";

   try
   {
	   if (!pInfo)
		   return false;

       //生成ID
	   SYSTEMTIME st;
	   GetLocalTime(&st);

	   CString strID = "";
	   strID.Format("ID%04d%02d%02d%02d%02d%02d%03d",
		   st.wYear,
		   st.wMonth,
		   st.wDay,
		   st.wHour,
		   st.wMinute,
		   st.wSecond,
		   st.wMilliseconds);

	   r_rtuID = strID;

	   strSQL.Format("INSERT INTO rtuStation (sName,sID, ProvinceCode,RegionCode,CountyCode,AddrA2,AddrA3,CmType,Interface,ComPortA,BaundA,DataBitsA,StopBitsA,ParityA,IpAddrA,SockPortA) VALUES ('%s','%s',%d,%d,%d,%d,%d,%d,%d,'%s',%d,%d,%d,%d,'%s',%d)",
					 pInfo->_strName,
					 strID,
					 pInfo->_bProvinceCode,
					 pInfo->_bRegionCode,
					 pInfo->_bCountyCode,
					 pInfo->_nAddrA2,
					 pInfo->_nAddrA3,
					 pInfo->_bCmType,
					 pInfo->_bInterface,
					 pInfo->_CmComInfo.mComName,
					 pInfo->_CmComInfo.mBaund,
					 pInfo->_CmComInfo.mDataBits,
					 pInfo->_CmComInfo.mStopBits,
					 pInfo->_CmComInfo.mParity,
					 pInfo->_CmNetInfo.mIpAddress,
					 pInfo->_CmNetInfo.mPort);

	 
	   sLogMessage.Format("新增RTU对象,SQL:%s",strSQL);
	   InsertLog(SYS_OTHER_RUN_LOG,sLogMessage,Normal_Log_Message);
	   m_Database.ExecuteSQL(strSQL);


	   strSQL.Format("INSERT INTO rtuStation_ext(sID,pVer,scAddr,scFactroyType,WorkMode) VALUES ('%s',%d,%d,%d,%d)",
		   strID,pInfo->_nAppProVer,pInfo->_CmScCommInfo.nScAddress,pInfo->_CmScCommInfo.nFactoryType,pInfo->_nWorkMode);
	   m_Database.ExecuteSQL(strSQL);

   }
   catch (CDBException* e)
   {
	   sLogMessage.Format("新增RTU对象,SQL:%s,Error:%s",strSQL,e->m_strError);
	   InsertLog(SYS_RUNNING_S_LOG,sLogMessage,Error_Log_message);
	   //e->ReportError();
	   e->Delete();
	   return false;
   }
   catch (CMemoryException* e)
   {
	   //e->ReportError();
	   e->Delete();
	   return false;

   }
   catch (CFileException* e)
   {
	   //e->ReportError();
	   e->Delete();
	   return false;
   }
   catch (CException* e)
   {
	   //e->ReportError();
	   e->Delete();
	   return false;
   }
   return true;
}


//************************************
// Method:    CheckRtuStExist
// FullName:  CRealTimeDB::CheckRtuStExist
// Access:    public 
// Returns:   bool
// Qualifier: 检查站是否已经存在
// Parameter: PRTU_STATION pInfo
//************************************
bool CRealTimeDB::CheckRtuStExist(PRTU_STATION pInfo)
{
	return CheckRtuStExist(pInfo->_bProvinceCode,pInfo->_bRegionCode,pInfo->_nAddrA2);
}

//************************************
// Method:    CheckRtuStExist
// FullName:  CRealTimeDB::CheckRtuStExist
// Access:    public 
// Returns:   bool
// Qualifier: 检查站是否已经存在
// Parameter: BYTE pCode
// Parameter: BYTE rCode
// Parameter: DWORD A2
//************************************
bool CRealTimeDB::CheckRtuStExist(BYTE pCode, BYTE rCode, DWORD A2)
{
  
	if (m_lstRtuStations.IsEmpty())
		return false;

	POSITION rPos = m_lstRtuStations.GetHeadPosition();
	while (rPos != NULL)
	{
		CRtuStation* pStation = (CRtuStation*)m_lstRtuStations.GetNext(rPos);
		if (pStation && 
			pStation->GetProvinceCode() == pCode && 
			pStation->GetRegionCode() == rCode  && 
			pStation->GetAddrA2() == A2)
		{
			 
			return true;
		}
	}

	return  false;
}

//************************************
// Method:    CheckRtuStExist
// FullName:  CRealTimeDB::CheckRtuStExist
// Access:    public 
// Returns:   bool
// Qualifier: 检查站是否已经存在
// Parameter: PRTU_STATION pInfo
// Parameter: CRtuStation * pCurStation
//************************************
bool  CRealTimeDB::CheckRtuStExist(PRTU_STATION pInfo,CRtuStation* pCurStation)
{
	return CheckRtuStExist(pInfo->_bProvinceCode,pInfo->_bRegionCode,pInfo->_nAddrA2,pCurStation);
}

//************************************
// Method:    CheckRtuStExist
// FullName:  CRealTimeDB::CheckRtuStExist
// Access:    public 
// Returns:   bool
// Qualifier: 检查站是否已经存在
// Parameter: BYTE pCode
// Parameter: BYTE rCode
// Parameter: DWORD A2
// Parameter: CRtuStation * pCurStation
//************************************
bool  CRealTimeDB::CheckRtuStExist(BYTE pCode,BYTE rCode,DWORD A2,CRtuStation* pCurStation)
{
	if (m_lstRtuStations.IsEmpty())
		return false;

	POSITION rPos = m_lstRtuStations.GetHeadPosition();
	while (rPos != NULL)
	{
		CRtuStation* pStation = (CRtuStation*)m_lstRtuStations.GetNext(rPos);
		if (pStation && 
			pStation != pCurStation && 
			pStation->GetProvinceCode() == pCode && 
			pStation->GetRegionCode() == rCode  && 
			pStation->GetAddrA2() == A2)
		{

			return true;
		}
	}

	return  false;
}


//************************************
// Method:    ModifyStInfo
// FullName:  CRealTimeDB::ModifyStInfo
// Access:    public 
// Returns:   bool
// Qualifier: 修改RTU站信息
// Parameter: PRTU_STATION pInfo
// Parameter: CRtuStation * pStation
//************************************
bool CRealTimeDB::ModifyStInfo(PRTU_STATION pInfo, CRtuStation *pStation)
{

	CString strSQL = "";
	CString sLogMessage = "";

	try
	{
		if (!pInfo || !pStation)
			return false;
		
		strSQL.Format("UPDATE rtuStation SET sName ='%s',ProvinceCode=%d,RegionCode=%d,CountyCode=%d,AddrA2=%d,AddrA3=%d,CmType=%d,Interface=%d,ComPortA='%s',BaundA=%d,DataBitsA=%d,StopBitsA=%d,ParityA=%d,IpAddrA='%s',SockPortA=%d WHERE sID='%s'",
			pInfo->_strName,
			pInfo->_bProvinceCode,
			pInfo->_bRegionCode,
			pInfo->_bCountyCode,
			pInfo->_nAddrA2,
			pInfo->_nAddrA3,
			pInfo->_bCmType,
			pInfo->_bInterface,
			pInfo->_CmComInfo.mComName,
			pInfo->_CmComInfo.mBaund,
			pInfo->_CmComInfo.mDataBits,
			pInfo->_CmComInfo.mStopBits,
			pInfo->_CmComInfo.mParity,
			pInfo->_CmNetInfo.mIpAddress,
			pInfo->_CmNetInfo.mPort,
			pStation->GetID());

			sLogMessage.Format("修改RTU对象信息,SQL:%s",strSQL);
			InsertLog(SYS_OTHER_RUN_LOG,sLogMessage,Normal_Log_Message);
			m_Database.ExecuteSQL(strSQL);

			strSQL.Format("SELECT * FROM rtuStation_ext WHERE sID='%s'",pStation->GetID());
			CString strTemp = "";
			CRecordset rs(&m_Database);
			rs.Open(CRecordset::forwardOnly,strSQL);
		
			if (!rs.IsEOF())
				strTemp.Format("UPDATE rtuStation_ext SET pVer=%d,scAddr=%d,scFactroyType=%d,WorkMode=%d WHERE sID='%s'",
				pInfo->_nAppProVer,pInfo->_CmScCommInfo.nScAddress,pInfo->_CmScCommInfo.nFactoryType,pInfo->_nWorkMode,pStation->GetID());
			else
				strTemp.Format("INSERT INTO rtuStation_ext(sID,pVer,scAddr,scFactroyType,WorkMode) VALUES ('%s',%d,%d,%d,%d)",
				pStation->GetID(),pInfo->_nAppProVer,pInfo->_CmScCommInfo.nScAddress,pInfo->_CmScCommInfo.nFactoryType,pInfo->_nWorkMode);

			rs.Close();
			m_Database.ExecuteSQL(strTemp);


			strSQL.Format("SELECT * FROM rtu_ext_para WHERE sID='%s' AND p_Name='DZ_DELAY'",pStation->GetID());
			CRecordset _rs(&m_Database);
			_rs.Open(CRecordset::forwardOnly,strSQL);

			if (!_rs.IsEOF())
				strTemp.Format("UPDATE rtu_ext_para SET p_Value='%d' WHERE sID='%s' AND p_Name='DZ_DELAY'",pStation->GetDzDelay(),pStation->GetID());
			else
				strTemp.Format("INSERT INTO rtu_ext_para(sID,p_Name,p_Value) VALUES ('%s','DZ_DELAY','%d')",pStation->GetID(),pStation->GetDzDelay());

			_rs.Close();
			m_Database.ExecuteSQL(strTemp);
	}
	catch (CDBException* e)
	{
		sLogMessage.Format("修改RTU对象信息,SQL:%s,Error:%s",strSQL,e->m_strError);
		InsertLog(SYS_RUNNING_S_LOG,sLogMessage,Error_Log_message);
		e->ReportError();
		e->Delete();
		return false;
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
		return false;
	}
	catch (CFileException* e)
	{
		e->ReportError();
		e->Delete();
		return false;
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
		return false;
	}
	return true;
}


//************************************
// Method:    DeleteStInfo
// FullName:  CRealTimeDB::DeleteStInfo
// Access:    public 
// Returns:   bool
// Qualifier: 删除RTU站信息
// Parameter: CRtuStation * pStation
//************************************
bool CRealTimeDB::DeleteStInfo(CRtuStation* pStation)
{
	CString strSQL = "";
	CString sLogMessage = "";

	try
	{
		if (!pStation)
			return false;
		
		strSQL.Format("DELETE FROM rtuStation WHERE sID='%s'",pStation->GetID());
		sLogMessage.Format("删除RTU对象信息,SQL:%s",strSQL);
		InsertLog(SYS_OTHER_RUN_LOG,sLogMessage,Normal_Log_Message);
		m_Database.ExecuteSQL(strSQL);

		strSQL.Format("DELETE FROM rtuYcObject WHERE sID='%s'",pStation->GetID());
		sLogMessage.Format("删除RTU下所有遥测对象信息,SQL:%s",strSQL);
		InsertLog(SYS_OTHER_RUN_LOG,sLogMessage,Normal_Log_Message);
		m_Database.ExecuteSQL(strSQL);

		strSQL.Format("DELETE FROM rtuYxObject WHERE sID='%s'",pStation->GetID());
		sLogMessage.Format("删除RTU下所有遥信对象信息,SQL:%s",strSQL);
		InsertLog(SYS_OTHER_RUN_LOG,sLogMessage,Normal_Log_Message);
		m_Database.ExecuteSQL(strSQL);

		strSQL.Format("DELETE FROM rtuYkObject WHERE sID='%s'",pStation->GetID());
		sLogMessage.Format("删除RTU下所有遥控对象信息,SQL:%s",strSQL);
		InsertLog(SYS_OTHER_RUN_LOG,sLogMessage,Normal_Log_Message);
		m_Database.ExecuteSQL(strSQL);

		strSQL.Format("DELETE FROM rtuZdObject WHERE sID='%s'",pStation->GetID());
		sLogMessage.Format("删除RTU下所有终端对象信息,SQL:%s",strSQL);
		InsertLog(SYS_OTHER_RUN_LOG,sLogMessage,Normal_Log_Message);
		m_Database.ExecuteSQL(strSQL);

		strSQL.Format("DELETE FROM rtuStation_ext WHERE sID='%s'",pStation->GetID());
		m_Database.ExecuteSQL(strSQL);
	}
	catch (CDBException* e)
	{
		sLogMessage.Format("数据库操作错误,SQL:%s,Error:%s",strSQL,e->m_strError);
		InsertLog(SYS_RUNNING_S_LOG,sLogMessage,Error_Log_message);
		e->ReportError();
		e->Delete();
		return false;
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
		return false;
	}
	catch (CFileException* e)
	{
		e->ReportError();
		e->Delete();
		return false;
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
		return false;
	}

	return true;
}



//添加、删除、修改遥测对象
//************************************
// Method:    AddRtuYcInfo
// FullName:  CRealTimeDB::AddRtuYcInfo
// Access:    public 
// Returns:   bool
// Qualifier: 添加RTU遥测对象信息
// Parameter: PRTU_YCOBJECT pInfo
//************************************
bool CRealTimeDB::AddRtuYcInfo(PRTU_YCOBJECT pInfo,CString& r_ycID)
{
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();

	CString strSQL = "";
	CString sLogMessage = "";
	try
	{
		if (!pInfo)
			return false;

		//生成ID
		SYSTEMTIME st;
		GetLocalTime(&st);
		CString ycID ="";
		ycID.Format("YC%04d%02d%02d%02d%02d%02d%03d",
			st.wYear,
			st.wMonth,
			st.wDay,
			st.wHour,
			st.wMinute,
			st.wSecond,st.wMilliseconds);

		r_ycID = ycID;

		strSQL.Format("INSERT INTO rtuYcObject (sName,sID,ycObjID,ycCode,xType,DisplayBits,PointBits,Unit,MaxValue,MinValue) VALUES ('%s','%s','%s',%d,%d,%d,%d,'%s',%f,%f)",
			pInfo->_strName,
			pInfo->_strSId,
			ycID,
			pInfo->_nYcCode,
			pInfo->_Type,
			pInfo->_DisplayBits,
			pInfo->_PointBits,
			pInfo->_Unit,
			pInfo->_MaxValue,
			pInfo->_MinValue);

		sLogMessage.Format("添加遥测对象,SQL:%s",strSQL);
		InsertLog(SYS_OTHER_RUN_LOG,sLogMessage,Normal_Log_Message);

		m_Database.ExecuteSQL(strSQL);

	}
	catch (CMemoryException* e)
	{

		e->ReportError();
		e->Delete();
		return false;
	}
	catch (CFileException* e)
	{
		e->ReportError();
		e->Delete();
		return false;
	}
	catch (CDBException* e)
	{
		sLogMessage.Format("添加遥测对象,SQL:%s,Error:%s",strSQL,e->m_strError);
		InsertLog(SYS_RUNNING_S_LOG,sLogMessage,Error_Log_message);
		e->ReportError();
		e->Delete();
		return false;
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
		return false;
	}
	return true;
}

//************************************
// Method:    ModifyYcInfo
// FullName:  CRealTimeDB::ModifyYcInfo
// Access:    public 
// Returns:   bool
// Qualifier: 修改Rtu遥测对象信息
// Parameter: PRTU_YCOBJECT pInfo
// Parameter: CRtuYcObject * pYcObj
//************************************
bool CRealTimeDB::ModifyYcInfo(PRTU_YCOBJECT pInfo,CRtuYcObject* pYcObj)
{
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();

	CString strSQL = "";
	CString sLogMessage = "";

	try
	{
		if (!pInfo || !pYcObj)
			return false;

		strSQL.Format("UPDATE rtuYcObject SET sName='%s',ycCode=%d,xType=%d,DisplayBits=%d,PointBits=%d,Unit='%s',MaxValue=%f,MinValue=%f WHERE sID='%s' AND ycObjID='%s'",
			pInfo->_strName,
			pInfo->_nYcCode,
			pInfo->_Type,
			pInfo->_DisplayBits,
			pInfo->_PointBits,
			pInfo->_Unit,
			pInfo->_MaxValue,
			pInfo->_MinValue,
			pYcObj->GetParent()->GetID(),
			pYcObj->GetID());
		sLogMessage.Format("修改遥测对象,SQL:%s",strSQL);
		InsertLog(SYS_OTHER_RUN_LOG,sLogMessage,Normal_Log_Message);
		m_Database.ExecuteSQL(strSQL);
	}
	catch (CDBException* e)
	{
		sLogMessage.Format("修改遥测对象,SQL:%s,Error:%s",strSQL,e->m_strError);
		InsertLog(SYS_RUNNING_S_LOG,sLogMessage,Error_Log_message);
		e->ReportError();
		e->Delete();
		return false;
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
		return false;
	}
	catch (CFileException* e)
	{
		e->ReportError();
		e->Delete();
		return false;
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
		return false;
	}

	return true;
}

//************************************
// Method:    DeleteYcInfo
// FullName:  CRealTimeDB::DeleteYcInfo
// Access:    public 
// Returns:   bool
// Qualifier: 删除RTU遥测信息
// Parameter: CRtuYcObject * pYcObj
//************************************
bool CRealTimeDB::DeleteYcInfo(CRtuYcObject* pYcObj)
{
	CString strSQL = "";
	CString sLogMessage = "";

	try
	{
		if (!pYcObj)
			return false;

		strSQL.Format("DELETE FROM rtuYcObject WHERE sID='%s' AND ycObjID='%s'",
			pYcObj->GetParent()->GetID(),
			pYcObj->GetID());
		sLogMessage.Format("删除遥测对象,SQL:%s",strSQL);
		InsertLog(SYS_OTHER_RUN_LOG,sLogMessage,Normal_Log_Message);
		m_Database.ExecuteSQL(strSQL);

	}
	catch (CDBException* e)
	{
		sLogMessage.Format("删除遥测对象,SQL:%s,Error:%s",strSQL,e->m_strError);
		InsertLog(SYS_RUNNING_S_LOG,sLogMessage,Error_Log_message);
		e->ReportError();
		e->Delete();
		return false;
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
		return false;
	}
	catch (CFileException* e)
	{
		e->ReportError();
		e->Delete();
		return false;
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
		return false;
	}

	return true;
}

//************************************
// Method:    AddRtuYxInfo
// FullName:  CRealTimeDB::AddRtuYxInfo
// Access:    public 
// Returns:   bool
// Qualifier: 添加RTU遥信对象信息
// Parameter: PRTU_YXOBJECT pInfo
//************************************
bool CRealTimeDB::AddRtuYxInfo(PRTU_YXOBJECT pInfo,CString& r_yxID)
{
	CString strSQL = "";
	CString sLogMessage = "";

	try
	{
		if (!pInfo)
			return false;

		//生成遥信ID
		CString yxID ="";
		SYSTEMTIME st;
		GetLocalTime(&st);
		yxID.Format("YX%04d%02d%02d%02d%02d%02d%03d",
			st.wYear,
			st.wMonth,
			st.wDay,
			st.wHour,
			st.wMinute,
			st.wSecond,st.wMilliseconds);

		r_yxID = yxID;

		strSQL.Format("INSERT INTO rtuYxObject (sName,sID,yxObjID,yxCode) VALUES ('%s','%s','%s',%d)",
			pInfo->_strName,
			pInfo->_strSId,
			yxID,
			pInfo->_nYxCode);

		sLogMessage.Format("添加遥信对象,SQL:%s",strSQL);
		InsertLog(SYS_OTHER_RUN_LOG,sLogMessage,Normal_Log_Message);

		m_Database.ExecuteSQL(strSQL);


	}
	catch (CDBException* e)
	{
		sLogMessage.Format("添加遥信对象,SQL:%s,Error:%s",strSQL,e->m_strError);
		InsertLog(SYS_RUNNING_S_LOG,sLogMessage,Error_Log_message);
		e->ReportError();
		e->Delete();
		return false;
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
		return false;
	}
	catch (CFileException* e)
	{
		e->ReportError();
		e->Delete();
		return false;
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
		return false;
	}
	return true;
}

//************************************
// Method:    ModifyYxInfo
// FullName:  CRealTimeDB::ModifyYxInfo
// Access:    public 
// Returns:   bool
// Qualifier: 修改Rtu遥信对象的信息
// Parameter: PRTU_YXOBJECT pInfo    新的对象相关信息
// Parameter: CRtuYxObject * pYxObj  需要修改的遥信对象
//************************************
bool CRealTimeDB::ModifyYxInfo(PRTU_YXOBJECT pInfo,CRtuYxObject* pYxObj)
{
	CString strSQL = "";
	CString sLogMessage = "";


	try
	{
		if (!pInfo || !pYxObj)
			return false;

		
		strSQL.Format("UPDATE rtuYxObject SET sName='%s',yxCode=%d WHERE sID='%s' AND yxObjID='%s'",
			pInfo->_strName,
			pInfo->_nYxCode,
			pYxObj->GetParent()->GetID(),
			pYxObj->GetID());

		sLogMessage.Format("修改遥信对象,SQL:%s",strSQL);
		InsertLog(SYS_OTHER_RUN_LOG,sLogMessage,Normal_Log_Message);
		m_Database.ExecuteSQL(strSQL);

	}
	catch (CDBException* e)
	{
		sLogMessage.Format("修改遥信对象,SQL:%s,Error:%s",strSQL,e->m_strError);
		InsertLog(SYS_RUNNING_S_LOG,sLogMessage,Error_Log_message);
		e->ReportError();
		e->Delete();
		return false;
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
		return false;
	}
	catch (CFileException* e)
	{
		e->ReportError();
		e->Delete();
		return false;
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
		return false;

	}
	return true;
}

//************************************
// Method:    DeleteYxInfo
// FullName:  CRealTimeDB::DeleteYxInfo
// Access:    public 
// Returns:   bool
// Qualifier: 删除遥信对象的信息
// Parameter: CRtuYxObject * pYxObj  需要删除的遥信对象指针
//************************************
bool CRealTimeDB::DeleteYxInfo(CRtuYxObject* pYxObj)
{
	CString strSQL = "";
	CString sLogMessage = "";
	try
	{
		if (!pYxObj)
			return false;

		strSQL.Format("DELETE FROM rtuYxObject WHERE sID='%s' AND yxObjID='%s'",
			pYxObj->GetParent()->GetID(),
			pYxObj->GetID());

		sLogMessage.Format("删除遥信对象,SQL:%s",strSQL);
		InsertLog(SYS_OTHER_RUN_LOG,sLogMessage,Normal_Log_Message);
		m_Database.ExecuteSQL(strSQL);

	}
	catch (CDBException* e)
	{
		sLogMessage.Format("删除遥信对象,SQL:%s,Error:%s",strSQL,e->m_strError);
		InsertLog(SYS_RUNNING_S_LOG,sLogMessage,Error_Log_message);
		e->ReportError();
		e->Delete();
		return false;
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
		return false;
	}
	catch (CFileException* e)
	{
		e->ReportError();
		e->Delete();
		return false;
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
		return false;
	}
	return true;
}



//************************************
// Method:    AddRtuYkInfo
// FullName:  CRealTimeDB::AddRtuYkInfo
// Access:    public 
// Returns:   bool
// Qualifier: 添加遥控对象信息
// Parameter: PRTU_YKOBJECT pInfo
//************************************
bool CRealTimeDB::AddRtuYkInfo(PRTU_YKOBJECT pInfo,CString& r_ykID)
{
	CString strSQL = "";
	CString sLogMessage = "";
	try
	{
		if (!pInfo)
			return false;

		//生成YK ID
		CString YkID;
		SYSTEMTIME st;
		GetLocalTime(&st);

		YkID.Format("YK%04d%02d%02d%02d%02d%02d%03d",
			st.wYear,
			st.wMonth,
			st.wDay,
			st.wHour,
			st.wMinute,
			st.wSecond,st.wMilliseconds);

		r_ykID = YkID;

		strSQL.Format("INSERT INTO rtuYkObject (sName,sID,ykObjID,ykCode) VALUES ('%s','%s','%s',%d)",
			pInfo->_strName,
			pInfo->_strSId,
			YkID,
			pInfo->_nYkCode);

		sLogMessage.Format("添加遥控对象,SQL:%s",strSQL);
		InsertLog(SYS_OTHER_RUN_LOG,sLogMessage,Normal_Log_Message);
		m_Database.ExecuteSQL(strSQL);

	}
	catch (CDBException* e)
	{
		sLogMessage.Format("添加遥控对象,SQL:%s,Error:%s",strSQL,e->m_strError);
		InsertLog(SYS_RUNNING_S_LOG,sLogMessage,Error_Log_message);
		e->ReportError();
		e->Delete();
		return false;
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
		return false;
	}
	catch (CFileException* e)
	{
		e->ReportError();
		e->Delete();
		return false;
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
		return false;
	}

	return true;
}

//************************************
// Method:    ModifyYkInfo
// FullName:  CRealTimeDB::ModifyYkInfo
// Access:    public 
// Returns:   bool
// Qualifier: 修改遥控对象的信息
// Parameter: PRTU_YKOBJECT pInfo      新的数据信息
// Parameter: CRtuYkObject * pYkObj    需要修改的遥控对象
//************************************
bool CRealTimeDB::ModifyYkInfo(PRTU_YKOBJECT pInfo,CRtuYkObject* pYkObj)
{
	CString strSQL = "";
	CString sLogMessage = "";
	try
	{
		if (!pInfo || !pYkObj)
			return false;

		strSQL.Format("UPDATE rtuYkObject SET sName='%s',ykCode=%d WHERE sID='%s' AND ykObjID='%s'",
			pInfo->_strName,
			pInfo->_nYkCode,
			pYkObj->GetParent()->GetID(),
			pYkObj->GetID());

		sLogMessage.Format("修改遥控对象,SQL:%s",strSQL);
		InsertLog(SYS_OTHER_RUN_LOG,sLogMessage,Normal_Log_Message);

		m_Database.ExecuteSQL(strSQL);

	}
	catch (CDBException* e)
	{
		sLogMessage.Format("修改遥控对象,SQL:%s,Error:%s",strSQL,e->m_strError);
		InsertLog(SYS_RUNNING_S_LOG,sLogMessage,Error_Log_message);
		e->ReportError();
		e->Delete();
		return false;
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
		return false;
	}
	catch (CFileException* e)
	{
		e->ReportError();
		e->Delete();
		return false;
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
		return false;
	}

	return true;
}

//************************************
// Method:    DeleteYkInfo
// FullName:  CRealTimeDB::DeleteYkInfo
// Access:    public 
// Returns:   bool
// Qualifier: 删除遥控对象
// Parameter: CRtuYkObject * pYkObj  需要删除的遥控对象
//************************************
bool CRealTimeDB::DeleteYkInfo(CRtuYkObject* pYkObj)
{
	CString strSQL = "";
	CString sLogMessage = "";
	try
	{
		if (!pYkObj)
			return false;

		strSQL.Format("DELETE FROM rtuYkObject WHERE sID='%s' AND ykObjID='%s'",
			pYkObj->GetParent()->GetID(),
			pYkObj->GetID());

		sLogMessage.Format("修改遥控对象,SQL:%s",strSQL);
		InsertLog(SYS_OTHER_RUN_LOG,sLogMessage,Normal_Log_Message);

		m_Database.ExecuteSQL(strSQL);

	}
	catch (CDBException* e)
	{
		sLogMessage.Format("修改遥控对象,SQL:%s,Error:%s",strSQL,e->m_strError);
		InsertLog(SYS_RUNNING_S_LOG,sLogMessage,Error_Log_message);

		e->ReportError();
		e->Delete();
		return false;
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
		return false;
	}
	catch (CFileException* e)
	{
		e->ReportError();
		e->Delete();
		return false;
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
		return false;
	}

	return true;
}


//************************************
// Method:    AddRtuZdInfo
// FullName:  CRealTimeDB::AddRtuZdInfo
// Access:    public 
// Returns:   bool
// Qualifier: 添加定值对象
// Parameter: PRTU_ZDOBJECT pInfo  新定值对象的信息
//************************************
bool CRealTimeDB::AddRtuZdInfo(PRTU_ZDOBJECT pInfo,CString& r_zdID)
{
	CString strSQL = "";
	CString sLogMessage = "";
	try
	{
		if (!pInfo)
			return false;

		//生成ZD ID
		CString zdID;
		SYSTEMTIME st;
		GetLocalTime(&st);

		zdID.Format("ZD%04d%02d%02d%02d%02d%02d%03d",
			st.wYear,
			st.wMonth,
			st.wDay,
			st.wHour,
			st.wMinute,
			st.wSecond,st.wMilliseconds);

		r_zdID  = zdID;

		strSQL.Format("INSERT INTO rtuZdObject (sName,sID,ZdObjID,ZdCode,sizeof,OrderInItem,Unit,ScaleMax,ScaleMin) VALUES ('%s','%s','%s',%d,%d,%d,'%s',%f,%f)",
			pInfo->_strName,
			pInfo->_strSId,
			zdID,
			pInfo->_nZDCode,
			pInfo->_Sizeof,
			pInfo->_OrderInItem,
			pInfo->_Unit,
			pInfo->_ScaleMax,
			pInfo->_ScaleMin);

		sLogMessage.Format("添加定值对象,SQL:%s",strSQL);
		InsertLog(SYS_OTHER_RUN_LOG,sLogMessage,Normal_Log_Message);

		m_Database.ExecuteSQL(strSQL);



	}
	catch (CDBException* e)
	{
		sLogMessage.Format("添加定值对象,SQL:%s,Error:%s",strSQL,e->m_strError);
		InsertLog(SYS_RUNNING_S_LOG,sLogMessage,Error_Log_message);

		e->ReportError();
		e->Delete();
		return false;
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
		return false;
	}
	catch (CFileException* e)
	{
		e->ReportError();
		e->Delete();
		return false;
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
		return false;
	}

	return true;

}

//************************************
// Method:    ModifyZdInfo
// FullName:  CRealTimeDB::ModifyZdInfo
// Access:    public 
// Returns:   bool
// Qualifier: 修改定值对象的数据信息
// Parameter: PRTU_ZDOBJECT pInfo    对象新的相关信息
// Parameter: CRtuZdObject * pZdObj  修改修改的定值对象
//************************************
bool CRealTimeDB::ModifyZdInfo(PRTU_ZDOBJECT pInfo,CRtuZdObject* pZdObj)
{
	CString strSQL = "";
	CString sLogMessage = "";

	try
	{
		if (!pInfo || !pZdObj)
			return false;

		strSQL.Format("UPDATE rtuZdObject SET sName='%s',ZdCode=%d,sizeof=%d,OrderInItem=%d,Unit='%s',ScaleMax=%f,ScaleMin=%f WHERE sID='%s' AND ZdObjID='%s'",
			pInfo->_strName,
			pInfo->_nZDCode,
			pInfo->_Sizeof,
			pInfo->_OrderInItem,
			pInfo->_Unit,
			pInfo->_ScaleMax,
			pInfo->_ScaleMin,
			pZdObj->GetParent()->GetID(),
			pZdObj->GetID());

		sLogMessage.Format("修改定值对象,SQL:%s",strSQL);
		InsertLog(SYS_OTHER_RUN_LOG,sLogMessage,Normal_Log_Message);

		m_Database.ExecuteSQL(strSQL);

	}
	catch (CDBException* e)
	{
		sLogMessage.Format("修改定值对象,SQL:%s,Error:%s",strSQL,e->m_strError);
		InsertLog(SYS_RUNNING_S_LOG,sLogMessage,Error_Log_message);
		e->ReportError();
		e->Delete();
		return false;
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
		return false;
	}
	catch (CFileException* e)
	{
		e->ReportError();
		e->Delete();
		return false;
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
		return false;
	}

	return true;
}

//************************************
// Method:    DeleteZdInfo
// FullName:  CRealTimeDB::DeleteZdInfo
// Access:    public 
// Returns:   bool
// Qualifier: 删除定值对象信息
// Parameter: CRtuZdObject * pZdObj  需要删除的定值对象
//************************************
bool CRealTimeDB::DeleteZdInfo(CRtuZdObject* pZdObj)
{
	CString strSQL = "";
	CString sLogMessage = "";
	try
	{
		if (!pZdObj)
			return false;

		strSQL.Format("DELETE FROM rtuZdObject WHERE sID='%s' AND ZdObjID='%s'",
			pZdObj->GetParent()->GetID(),
			pZdObj->GetID());

		sLogMessage.Format("删除定值对象,SQL:%s",strSQL);
		InsertLog(SYS_OTHER_RUN_LOG,sLogMessage,Normal_Log_Message);

		m_Database.ExecuteSQL(strSQL);

	}
	catch (CDBException* e)
	{
		sLogMessage.Format("删除定值对象,SQL:%s,Error:%s",strSQL,e->m_strError);
		InsertLog(SYS_RUNNING_S_LOG,sLogMessage,Error_Log_message);

		e->ReportError();
		e->Delete();
		return false;
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
		return false;
	}
	catch (CFileException* e)
	{
		e->ReportError();
		e->Delete();
		return false;
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
		return false;
	}
	return true;
}


bool CRealTimeDB::UpdateRtusRealDB(CString rtuID,DB_RECORD_OPERATION_TYPE opType)
{
	switch (opType)
	{
	case RECORD_NEW:        //新增
		{
			CRtuStation* pObj = QueryRtuInDb(rtuID);
			return	AddRtuToRealDB(pObj);

		}
		break;
	case RECORD_EDIT:       //修改
		{
			/*
			if (DeleteRealDBRtu(rtuID))
			{
				CRtuStation* pObj = QueryRtuInDb(rtuID);
				return	AddRtuToRealDB(pObj);
			}
			else 
				return false;
			*/
		}
		break;
	case RECORD_DEL:		//删除
		{
			return DeleteRealDBRtu(rtuID);
		}
		break;
	}

	return true;
}


bool  CRealTimeDB::DeleteRealDBRtu(CString rtuID)    //删除链表总的rtu对象信息
{
	if( rtuID.IsEmpty())
		return false;

	bool blFinded = false;
	CRtuStation* pObjDel = NULL;  //需要删除的对象
	
	m_RtuList_Mutex.Lock();
	POSITION rPos = m_lstRtuStations.GetHeadPosition();
	while (rPos != NULL)
	{
		CRtuStation* pObj = (CRtuStation*)m_lstRtuStations.GetAt(rPos);
		if (pObj && pObj->GetID() == rtuID)
		{
			blFinded = true;
			pObjDel = pObj;
			break;
		}
		m_lstRtuStations.GetNext(rPos);
	}

	if (blFinded == true && rPos != NULL)
	{
		m_lstRtuStations.RemoveAt(rPos);

		if (pObjDel)
		{
			delete pObjDel;
			pObjDel = NULL;
		}
	}
	m_RtuList_Mutex.Unlock();
	
	return true;
}

CRtuStation*  CRealTimeDB::QueryRtuInDb(CString rtuID)       //从数据库查找RTU站信息
{
	CString sSQL = "";
	CString sLogMessage = "";

	CRtuStation* pObj = NULL;

	try
	{
		sSQL.Format("SELECT DISTINCT sName,sID, ProvinceCode,RegionCode,CountyCode,AddrA2,AddrA3,CmType,Interface,ComPortA,BaundA,DataBitsA,StopBitsA,ParityA,IpAddrA,SockPortA FROM rtuStation WHERE sID='%s'",rtuID);
		sLogMessage.Format("配置数据库操作,SQL:%s",sSQL);
		InsertLog(SYS_OTHER_RUN_LOG,sLogMessage,Normal_Log_Message);

		CRecordset rs(&m_Database);

		rs.Open(CRecordset::forwardOnly,sSQL);

		CString sTemp = "";
		if (!rs.IsEOF())
		{
			RTU_STATION info;
			rs.GetFieldValue("sName",sTemp);
			info._strName = sTemp;

			rs.GetFieldValue("sID",sTemp);
			info._strSId = sTemp;

			rs.GetFieldValue("ProvinceCode",sTemp);
			info._bProvinceCode = (BYTE)atoi(sTemp);

			rs.GetFieldValue("RegionCode",sTemp);
			info._bRegionCode = (BYTE)atoi(sTemp);

			rs.GetFieldValue("CountyCode",sTemp);
			info._bCountyCode  = (BYTE)atoi(sTemp);

			info._nAddrA1 = info._bProvinceCode * 100 + info._bRegionCode;

			rs.GetFieldValue("AddrA2",sTemp);
			info._nAddrA2 = (DWORD)atoi(sTemp);

			rs.GetFieldValue("AddrA3",sTemp);
			info._nAddrA3  = (BYTE)atoi(sTemp);

			rs.GetFieldValue("CmType",sTemp);
			info._bCmType = (BYTE)atoi(sTemp);

			rs.GetFieldValue("Interface",sTemp);
			info._bInterface = (BYTE)atoi(sTemp);

			rs.GetFieldValue("ComPortA",sTemp);
			info._CmComInfo.mComName = sTemp;

			rs.GetFieldValue("BaundA",sTemp);
			info._CmComInfo.mBaund = atoi(sTemp);

			rs.GetFieldValue("DataBitsA",sTemp);
			info._CmComInfo.mDataBits = (BYTE)atoi(sTemp);

			rs.GetFieldValue("StopBitsA",sTemp);
			info._CmComInfo.mStopBits = (BYTE)atoi(sTemp);

			rs.GetFieldValue("ParityA",sTemp);
			info._CmComInfo.mParity = (BYTE)atoi(sTemp);

			rs.GetFieldValue("IpAddrA",sTemp);
			info._CmNetInfo.mIpAddress = sTemp;

			rs.GetFieldValue("SockPortA",sTemp);
			info._CmNetInfo.mPort = atoi(sTemp);

			pObj = new CRtuStation(&info);

			if (pObj)
			{
				pObj->SetParent(this);
			}
		}
		rs.Close();

		if (pObj)
		{
			pObj->BuildAllData(&m_Database);
		}


		return pObj;
	}
	catch (CDBException* e)
	{
		sLogMessage.Format("删除定值对象,SQL:%s,Error:%s",sSQL,e->m_strError);
		InsertLog(SYS_RUNNING_S_LOG,sLogMessage,Error_Log_message);

		e->ReportError();
		e->Delete();
		return NULL;
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
		return NULL;
	}
	catch (CFileException* e)
	{
		e->ReportError();
		e->Delete();
		return NULL;
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
		return NULL;
	}
	return pObj;
}

bool CRealTimeDB::AddRtuToRealDB(CRtuStation* pObjNew)
{
	if (pObjNew)
	{
		m_RtuList_Mutex.Lock();
		m_lstRtuStations.AddTail(pObjNew);
		m_RtuList_Mutex.Unlock();
	}
	else
		return false;

	return true;
}


void  CRealTimeDB::SaveRtuInforOnExit()
{
	POSITION rPos  = m_lstRtuStations.GetHeadPosition();

	try
	{
		while (rPos != NULL)
		{
			CRtuStation* pObj = (CRtuStation*)m_lstRtuStations.GetNext(rPos);
			if (pObj)
			{
				RTU_STATION info;
				info._strSId = pObj->GetID();
				info._bCmType = pObj->GetCmType();
				info._bCountyCode = pObj->GetCountyCode();
				info._bInterface = pObj->GetInterface();
				info._bProvinceCode = pObj->GetProvinceCode();
				info._bRegionCode = pObj->GetRegionCode();
				info._CmComInfo.mBaund = pObj->GetBaundA();
				info._CmComInfo.mComName = pObj->GetComPortA();
				info._CmComInfo.mDataBits = pObj->GetDataBitsA();
				info._CmComInfo.mParity = pObj->GetParityA();
				info._CmComInfo.mStopBits = pObj->GetStopBitsA();
				info._CmNetInfo.mIpAddress =pObj->GetIpAddrA();
				info._CmNetInfo.mPort = pObj->GetSockPortA();
				info._nAddrA1  = (info._bProvinceCode * 100 + info._bRegionCode);
				info._nAddrA2  = pObj->GetAddrA2();
				info._nAddrA3  = pObj->GetAddrA3();
				info._strName = pObj->GetName();

				info._CmScCommInfo.nScAddress = pObj->GetScAddress();
				info._CmScCommInfo.nFactoryType = pObj->GetScFactoryType();
				info._nAppProVer = pObj->GetAppProVer();
				info._nWorkMode = pObj->GetWorkMode();

				ModifyStInfo(&info,pObj);
			}
		}
	}
	catch (CDBException* e)
	{
		e->ReportError();
		e->Delete();
	
	}
}


bool CRealTimeDB::AddUserInfo(PUSER_INFO pUserInfo)
{
	if (!pUserInfo)
		return false;

	CString sLogMessage = "";
	CString sSQL = "";
	
	try
	{
		sSQL.Format("INSERT INTO users_tab (sName,sID,sPWD,bType,OnDuty) VALUES ('%s','%s','%s',%d,%d)",
			pUserInfo->sName,
			pUserInfo->sID,
			pUserInfo->sPwd,
			pUserInfo->bType,
			pUserInfo->bOnDuty);

		sLogMessage.Format("添加用户信息,SQL:%s",sSQL);
		InsertLog(SYS_OTHER_RUN_LOG,sLogMessage,Normal_Log_Message);
		m_Database.ExecuteSQL(sSQL);
	}
	catch (CDBException* pe)
	{
		sLogMessage.Format("添加用户信息,SQL:%s,Error:%s",sSQL,pe->m_strError);
		pe->Delete();
		InsertLog(SYS_RUNNING_S_LOG,sLogMessage,Error_Log_message);
		return false;
	}
	return true;
}

bool CRealTimeDB::ModifyUserInfo(PUSER_INFO pUserInfo,CUserObject* pUser)
{
	if (!pUserInfo || !pUser)
		return false;

	CString sLogMessage = "";
	CString sSQL = "";

	try
	{
		sSQL.Format("UPDATE users_tab set sName='%s',sID='%s',sPWD='%s',bType=%d,OnDuty=%d WHERE sID='%s'",
			pUserInfo->sName,
			pUserInfo->sID,
			pUserInfo->sPwd,
			pUserInfo->bType,
			pUserInfo->bOnDuty,
			pUser->GetID()
			);

		sLogMessage.Format("修改用户信息,SQL:%s",sSQL);
		InsertLog(SYS_OTHER_RUN_LOG,sLogMessage,Normal_Log_Message);
		m_Database.ExecuteSQL(sSQL);
	}
	catch (CDBException* pe)
	{
		sLogMessage.Format("修改用户信息,SQL:%s,Error:%s",sSQL,pe->m_strError);
		pe->Delete();
		InsertLog(SYS_RUNNING_S_LOG,sLogMessage,Error_Log_message);
		return false;
	}
	return true;
}

bool CRealTimeDB::deleteUserInfo(CUserObject* pUser)
{
	if (!pUser) 
		return false;

	CString sLogMessage = "";
	CString sSQL = "";

	try
	{
		sSQL.Format("DELETE FROM users_tab WHERE sID='%s'",pUser->GetID());
		sLogMessage.Format("删除用户信息,SQL:%s",sSQL);
		InsertLog(SYS_OTHER_RUN_LOG,sLogMessage,Normal_Log_Message);
		m_Database.ExecuteSQL(sSQL);

		POSITION rPos = m_lstUsers.GetHeadPosition();
		BOOL blFinded = FALSE;
		while(rPos != NULL)
		{
			CUserObject* pTempUser = m_lstUsers.GetAt(rPos);
			if (pTempUser && pTempUser == pUser)
			{
			   blFinded = TRUE;
			   break;
			}
			m_lstUsers.GetNext(rPos);
		}

		if (blFinded == TRUE && rPos != NULL)
		{
			delete pUser;
			pUser = NULL;

			m_lstUsers.RemoveAt(rPos);
		}
	}
	catch (CDBException* pe)
	{
		sLogMessage.Format("删除用户信息,SQL:%s,Error:%s",sSQL,pe->m_strError);
		pe->Delete();
		InsertLog(SYS_RUNNING_S_LOG,sLogMessage,Error_Log_message);
		return false;
	}

	return true;
}


bool CRealTimeDB::CheckUserExist(CString sID,CUserObject* pCurUser /* = NULL */)
{
	
	POSITION rPos = m_lstUsers.GetHeadPosition();
	while (rPos != NULL)
	{
		CUserObject* pTempUser = m_lstUsers.GetNext(rPos);
		if (pCurUser ==NULL)
		{
			if (pTempUser && pTempUser->GetID() == sID)
				return true;
		}
		else
		{
			if (pTempUser && pTempUser != pCurUser && pTempUser->GetID() == sID)
			{
				return true;
			}
		}
	}

	return false;
}

CUserObject* CRealTimeDB::SearchUserObject(CString _sID)
{
	CUserObject* pUser = NULL;

	POSITION rPos = m_lstUsers.GetHeadPosition();
	while(rPos != NULL)
	{
		CUserObject* pTempUser = m_lstUsers.GetNext(rPos);
		if (pTempUser && pTempUser->GetID() == _sID)
		{
			pUser = pTempUser;
			break;
		}
	}
	
	return pUser;
}