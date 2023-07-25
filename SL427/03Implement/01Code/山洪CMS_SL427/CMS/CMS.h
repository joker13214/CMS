// CMS.h : CMS 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号

#include "UdfGlobalFUNC.h"
#include "UdfLogSystem.h"
#include "RealTimeDB.h"
#include "UdfRtuDataSave.h"
#include "ConfigFile.h"
#include "UdfStartUpDlg.h"
#include "CommServer.h"
#include "RtuStation.h"
#include <map>

// CCMSApp:
// 有关此类的实现，请参阅 CMS.cpp
//
class FlowDataPrint {
public:
	CString m_stcd;      //编码
	CString m_time;      //时间
	double  sunflowdata; //瞬时流量
	double leiflowdata;  //累积流量
public:
	FlowDataPrint()
	{
		m_stcd = "";
		m_time = "";
		sunflowdata = 0.0;
		leiflowdata = 0.0;
	}
	FlowDataPrint(CString stcd, CString time, double flowdata1, double flowdata2)
	{
		m_stcd = stcd;
		m_time = time;
		sunflowdata = flowdata1;
		leiflowdata = flowdata2;
	}
   //static std::list<FlowDataPrint*> FlowDatalist;
};

//extern CList<FlowDataPrint*> FlowDatalist;
//extern std::list<FlowDataPrint*> FlowDatalist;
static std::list<CString> FlowDatalist;

class CCMSApp : public CWinApp
{
public:
	CCMSApp();

	BOOL LoadConfigFile();																							//读取/生成配置文件
	BOOL SystemLog(_LOG_MESSAGE_TYPE _logType,CString _LogMessage,_Log_level_type _levelType = Normal_Log_Message,PSYSTEMTIME pLogTime = NULL); //存储日志信息  Log information
protected:
	BOOL InitStartUpWnd();
	BOOL InitLogSystem();								//启动日志系统(Log System)
	BOOL InitRealTimeDB();								//启动配置数据库
	bool InitRtuDataSaveSystem();						//启动实时数据库转存接口服务
	bool InitProcessKey();								//生成Process key
	BOOL SaveStartUpLog();                              //存储软件启动日志信息
	bool InitCommServer();                              //初始化并启动通信服务器

	bool CheckProcessRunning();							//判断是否进程已经运行（保证系统只运行一个进程）
public:
	CMS_GLOBAL_OPTIONS				m_GlobalOptions;				//运行时相关的所有参数信息
	CUdfLogSystem*					m_pLogSystem;					//软件日志系统
	CRealTimeDB*					m_pReadDB;						//RTU等配置项实时数据库
	CUdfRtuDataSave_Server*			m_pRtuDatasave;					//采集数据实时存库系统
	JDGM_CONFIG_FILE::CConfigFile	m_sysConfigObj;                 //操作系统配置文件对象
	//CUdfStartUpDlg                  m_wndStartUpDlg;				//启动窗口
	CCommServer*                    m_pCommServer;                  //通信服务器

	//变量
	CString             m_sProcessKey;                  //运行进程唯一Key,查询软件是否正常启动、退出
	HANDLE				m_hMutex;						//为了只运行一个进程 （全局锁）

protected:
	CUserObject* m_pLoginUser;							//当前登录的用户
	bool         m_bLogined;                            //是否登录的标志 
	CRtuStation* m_pCurOpStation;						//当前操作的遥测终端对象,默认NULL
public:
	void         SetLoginUser(CUserObject* pLoginUser)	{m_pLoginUser = pLoginUser;}
	void		 SetLoginFlag(bool blLogined)			{m_bLogined = blLogined;}
	CUserObject* GetLoginUser()							{return m_pLoginUser;}
	const bool   GetLoginFlag()							{return m_bLogined;}

	void         InitTmMsgList(); 
	void         Set_Cur_Op_Station(CRtuStation* pCur_Station){m_pCurOpStation = pCur_Station;}
	CRtuStation* Get_Cur_Op_Station(){return m_pCurOpStation;}
// 重写
public:
	std::map<int, CString> m_TextMsgInitHash;

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

};

extern CCMSApp theApp;