// CMS.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "CMS.h"
#include "MainFrm.h"

#include "CMSDoc.h"
#include "CMSView.h"

#include "UdfLogSystem.h"
#include "XpStyle/MenuXP.h"
#include "XpStyle/ButtonXP.h"
#include "afxwin.h"

#include "IOCP_Server.h"


#include <tlhelp32.h> //声明快照函数文件
#include <shlwapi.h>

#pragma comment(lib,"shlwapi.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCMSApp

BEGIN_MESSAGE_MAP(CCMSApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CCMSApp::OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CCMSApp 构造

CCMSApp::CCMSApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
	m_pLogSystem = NULL;
	m_pReadDB = NULL;
	m_pRtuDatasave = NULL;
	m_hMutex = NULL;
	m_sProcessKey = "";
	m_pCommServer = NULL;

	m_bLogined = false;
	m_pLoginUser = NULL;
	m_pCurOpStation = NULL;
}


// 唯一的一个 CCMSApp 对象

CCMSApp theApp;


//true 表示有进程在运行
bool CCMSApp::CheckProcessRunning()
{

	//HANDLE	hMutex = ::OpenMutex(MUTEX_ALL_ACCESS, FALSE, "CDGM_CMS_DWRP");
	//if (hMutex)
	//{
	//	::CloseHandle(hMutex);
	//	return true;
	//}
	//else
	//{
	//	m_hMutex=::CreateMutex(NULL, FALSE, "CDGM_CMS_DWRP");
	//	if(m_hMutex == NULL)
	//	{
	//		return true;
	//	}
	//}


	PROCESSENTRY32 pe32;
	//在使用这个结构之前，先设置它的大小
	pe32.dwSize = sizeof(pe32);
	//给系统内的所有进程拍一个快照
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	//Includes the process list in the snapshot
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		TRACE("CreateToolhelp32Snapshot 调用失败! \n");
		return false;
	}

	DWORD currid = ::GetCurrentProcessId();
	TRACE("CUR:%5u\r\n",currid);
	CHAR currmodule[MAX_PATH];
	::GetModuleFileName(NULL,currmodule,MAX_PATH);
	CString currprocessname = PathFindFileName(currmodule);
	TRACE("CUR:%s\r\nName:%s\r\n",currmodule,currprocessname);

	//遍历进程快照，轮流显示每个进程信息
	BOOL bMore = ::Process32First(hProcessSnap,&pe32);
	while (bMore)
	{
		TRACE("%5u  %s\r\n",pe32.th32ProcessID,pe32.szExeFile);

		CString sTemp = pe32.szExeFile;
		if (pe32.th32ProcessID !=  currid)
		{
			if (sTemp == currprocessname)
			{
				CloseHandle(hProcessSnap);
				return true;
			}
		}

		//遍历下一个
		bMore = ::Process32Next(hProcessSnap,&pe32);
	}
	//清除snapshot对象
	::CloseHandle(hProcessSnap);

	return false;
}


// CCMSApp 初始化
BOOL CCMSApp::InitInstance()
{
	//保证只运行一个进程
//     if(CheckProcessRunning() == true)
// 	{
// 		::MessageBox(NULL,"'CMS - 通信前置机'已经在当前系统中运行","提示",MB_OK|MB_ICONINFORMATION);
// 		return FALSE;
// 	}

	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();

	AfxInitRichEdit();


	CMenuXP::InitializeHook();
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)
	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CCMSDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CCMSView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	//生成CRC校验table
	//CUdfGlobalFUNC::create_crc_table();

	//获取运行文件所在的目录路径（绝对路径）
	m_GlobalOptions.m_strCurRunMoudlePath = CUdfGlobalFUNC::GetRuningPath();

	//读取运行配置文件
    LoadConfigFile();

	SCSW_TypeMap *p = SCSW_TypeMap::GetInstance();
	//初始相关
	InitProcessKey();
	InitLogSystem(); 

	//启动IOCP服务
	CIOCP_Server::Instance()->StartIOCPServer();

	//初始化RealDB
	InitRealTimeDB();

	//启动通信服务
	InitCommServer();

	// 分析标准外壳命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
    
	//初始相关
	InitRtuDataSaveSystem();		//等主窗口初始化完后在初始化RTU数据实时入库线程


	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	//m_pMainWnd->SetWindowText("CMS - 通信前置机 (Communications Management Server) ");
	//m_pMainWnd->SetWindowText("CMS - 通信前置机");
	m_pMainWnd->ShowWindow(SW_HIDE);
	m_pMainWnd->SetWindowText(m_GlobalOptions.m_strAppTitie);
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();

	//仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand  之后发生

	SaveStartUpLog();

	return TRUE;
}


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	CButtonXP m_btnOK;
public:
	virtual BOOL OnInitDialog();
public:
	CStatic m_wndCaptionStatic;
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_STATIC_CAPTION, m_wndCaptionStatic);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CCMSApp::OnAppAbout()
{
	CMainFrame* pMainFrame = (CMainFrame*)m_pMainWnd;

	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
	
	pMainFrame->UpdateClientViews();
}

BOOL CCMSApp::SaveStartUpLog()
{
	if (m_pLogSystem)
	{
		CString logMessage ="";
		logMessage.Format("系统正常启动完成,PROCESS KEY:%s",m_sProcessKey);
		m_pLogSystem->InsertLogMessage(SYS_RUNNING_S_LOG,logMessage);
	}
	return TRUE;
}

BOOL CCMSApp::LoadConfigFile()
{

	CString sFile="";
	CString _sTemp ="";

	CString strRunPath= CUdfGlobalFUNC::GetRuningPath();

	sFile.Format("%s\\CMS-CONFIG.INI",strRunPath);

	m_sysConfigObj.SetConfigFileName(sFile);
	m_sysConfigObj.LoadFile();
	BOOL blOptExist = FALSE;


	m_GlobalOptions.m_strAppTitie = m_sysConfigObj.GetItemValue("APP_TITLE",blOptExist);
	if (!blOptExist)
	{
		m_sysConfigObj.AddOneOption("APP_TITLE","CMS - 通信前置机","",TRUE);
		m_GlobalOptions.m_strAppTitie = "CMS - 通信前置机";
	}
	
	if (m_GlobalOptions.m_strAppTitie == "")
		m_GlobalOptions.m_strAppTitie = "CMS - 通信前置机";

	m_GlobalOptions.m_strCfgFileName = m_sysConfigObj.GetItemValue("CFGFILE",blOptExist);
	if (!blOptExist)
	{
		m_sysConfigObj.AddOneOption("CFGFILE","CMConfig.mdb","保存配置项的数据库文件",TRUE);
		m_GlobalOptions.m_strCfgFileName  = "CMConfig.mdb";
	}
	_sTemp = m_GlobalOptions.m_strCfgFileName;
	m_GlobalOptions.m_strCfgFileName.Format("%s\\%s",CUdfGlobalFUNC::GetRuningPath(),_sTemp);      //绝对文件路径.重点

	m_GlobalOptions.m_strAdDivCodeFilename = m_sysConfigObj.GetItemValue("ADDIVFILE",blOptExist);
	if (!blOptExist)
	{
		m_sysConfigObj.AddOneOption("ADDIVFILE","A1.ini","行政区划码数据文件",TRUE);
		m_GlobalOptions.m_strAdDivCodeFilename = "A1.ini";
	}
	_sTemp = m_GlobalOptions.m_strAdDivCodeFilename;
	m_GlobalOptions.m_strAdDivCodeFilename.Format("%s\\%s",CUdfGlobalFUNC::GetRuningPath(),_sTemp);   //绝对文件路径.重点


	m_GlobalOptions.m_strCSThreadsCfgFile = m_sysConfigObj.GetItemValue("CM_THREADS_CFG",blOptExist);
	if (!blOptExist)
	{
		m_sysConfigObj.AddOneOption("CM_THREADS_CFG","THRD-CONFIG.INI","Commserver服务线程配置文件",TRUE);
		m_GlobalOptions.m_strCSThreadsCfgFile = "THRD-CONFIG.INI";
	}
	_sTemp = m_GlobalOptions.m_strCSThreadsCfgFile;
	m_GlobalOptions.m_strCSThreadsCfgFile.Format("%s\\%s",CUdfGlobalFUNC::GetRuningPath(),_sTemp);


	m_GlobalOptions.m_strLocalDbName = m_sysConfigObj.GetItemValue("LOCAL_DBFILE",blOptExist);
	if (!blOptExist)
	{
		m_sysConfigObj.AddOneOption("LOCAL_DBFILE","cms.db","Local db file",TRUE);
		m_GlobalOptions.m_strLocalDbName = "cms.db";
	}
	//_sTemp = m_GlobalOptions.m_strLocalDbName;
	//m_GlobalOptions.m_strLocalDbName.Format("%s\\%s",CUdfGlobalFUNC::GetRuningPath(),_sTemp);


	m_GlobalOptions.m_DataInterface = (BYTE)atoi(m_sysConfigObj.GetItemValue("DATA_INTERFACE",blOptExist));
	if (!blOptExist)
	{
		m_sysConfigObj.AddOneOption("DATA_INTERFACE","1","Data Interface,1:GM,2:SC",TRUE);
		m_GlobalOptions.m_DataInterface = 1;
	}
	else
	{
		if (m_GlobalOptions.m_DataInterface < 1 ||
			m_GlobalOptions.m_DataInterface > 3)
			m_GlobalOptions.m_DataInterface = 1 ;
	}

	m_GlobalOptions.m_DataIType = (BYTE)atoi(m_sysConfigObj.GetItemValue("DATA_ITYPE",blOptExist));
	if (!blOptExist)
	{
		m_sysConfigObj.AddOneOption("DATA_ITYPE","1","接口类型(1:直接访问数据库,2:WebService接口)",TRUE);
		m_GlobalOptions.m_DataIType = 1;
	}
	else
	{
		if (m_GlobalOptions.m_DataIType != 1 &&
			m_GlobalOptions.m_DataIType != 2)
		{
			m_GlobalOptions.m_DataIType = 1;
		}
	}

	m_GlobalOptions.m_DbOpts.m_strMISDSN = m_sysConfigObj.GetItemValue("SYSDBDSN",blOptExist);
	if (!blOptExist)
	{
		m_sysConfigObj.AddOneOption("SYSDBDSN","MISDSN","数据接口参数",FALSE);
		m_GlobalOptions.m_DbOpts.m_strMISDSN = "MISDSN";
	}

	m_GlobalOptions.m_DbOpts.m_strMISUID = m_sysConfigObj.GetItemValue("SYSDBUID",blOptExist);
	if (!blOptExist)
	{
		m_sysConfigObj.AddOneOption("SYSDBUID","sa","",FALSE);
		m_GlobalOptions.m_DbOpts.m_strMISUID = "sa";
	}

	m_GlobalOptions.m_DbOpts.m_strMISPWD = m_sysConfigObj.GetItemValue("SYSDBPWD",blOptExist);
	if (!blOptExist)
	{
		m_sysConfigObj.AddOneOption("SYSDBPWD","sqlsql","",TRUE);
		m_GlobalOptions.m_DbOpts.m_strMISPWD  = "sqlsql";
	}

   m_GlobalOptions.m_bAdminMode = (BYTE)atoi(m_sysConfigObj.GetItemValue("ADMINMODE",blOptExist));
   if (!blOptExist)
   {
	   m_sysConfigObj.AddOneOption("ADMINMODE","0","Admin mode 1/0",TRUE);
	   m_GlobalOptions.m_bAdminMode = 0;
   }

   m_GlobalOptions.m_TpDelay = (BYTE)atoi(m_sysConfigObj.GetItemValue("TPDELAY",blOptExist));
   if (!blOptExist)
   {
	   m_sysConfigObj.AddOneOption("TPDELAY","0","TP Delay",TRUE);
	   m_GlobalOptions.m_TpDelay = 0;
   }

   m_GlobalOptions.m_ResendInterval = (BYTE)atoi(m_sysConfigObj.GetItemValue("RESENDINTERVAL",blOptExist));
   if (!blOptExist)
   {
	   m_sysConfigObj.AddOneOption("RESENDINTERVAL","10","Resend Interval(单位:second) 1-255",TRUE);
	   m_GlobalOptions.m_ResendInterval = 10;
   }

   m_GlobalOptions.m_LogOpts.CmBufferSave_Flag = (BYTE)atoi(m_sysConfigObj.GetItemValue("CMBUFFER_SV",blOptExist));
   if (!blOptExist)
   {
	   m_sysConfigObj.AddOneOption("CMBUFFER_SV","0","日志系统相关参数");
	   m_GlobalOptions.m_LogOpts.CmBufferSave_Flag = 0;
   }

   m_GlobalOptions.m_LogOpts.CmStatusSave_Flag = (BYTE)atoi(m_sysConfigObj.GetItemValue("CMSTATUS_SV",blOptExist));
   if (!blOptExist)
   {
	   m_sysConfigObj.AddOneOption("CMSTATUS_SV","0","");
	   m_GlobalOptions.m_LogOpts.CmStatusSave_Flag = 0;
   }

   m_GlobalOptions.m_LogOpts.OhterRunSave_FLag = (BYTE)atoi(m_sysConfigObj.GetItemValue("OTHERRUN_SV",blOptExist));
   if (!blOptExist)
   {
	   m_sysConfigObj.AddOneOption("OTHERRUN_SV","0");
	   m_GlobalOptions.m_LogOpts.OhterRunSave_FLag = 0;
   }

   m_GlobalOptions.m_LogOpts.RunningSSave_Flag = (BYTE)atoi(m_sysConfigObj.GetItemValue("RUNNINGS_SV",blOptExist));
   if (!blOptExist)
   {
	   m_sysConfigObj.AddOneOption("RUNNINGS_SV","0");
	   m_GlobalOptions.m_LogOpts.RunningSSave_Flag =0;
   }

   m_GlobalOptions.m_LogOpts.YcChangeSave_Flag = (BYTE)atoi(m_sysConfigObj.GetItemValue("YCCHANGE_SV",blOptExist));
   if (!blOptExist)
   {
	   m_sysConfigObj.AddOneOption("YCCHANGE_SV","0");
	   m_GlobalOptions.m_LogOpts.YcChangeSave_Flag = 0;
   }
	

   m_GlobalOptions.m_LogOpts.YkOperatSave_Flag = (BYTE)atoi(m_sysConfigObj.GetItemValue("YKOPERAT_SV",blOptExist));
   if (!blOptExist)
   {
	   m_sysConfigObj.AddOneOption("YKOPERAT_SV","0");
	   m_GlobalOptions.m_LogOpts.YkOperatSave_Flag = 0;
   }

   m_GlobalOptions.m_LogOpts.YxChangeSave_Flag = (BYTE)atoi(m_sysConfigObj.GetItemValue("YXCHANGE_SV",blOptExist));
   if (!blOptExist)
   {
	   m_sysConfigObj.AddOneOption("YXCHANGE_SV","0");
	   m_GlobalOptions.m_LogOpts.YxChangeSave_Flag = 0;
   }
   m_GlobalOptions.m_LogOpts.ZdOperatSave_Flag = (BYTE)atoi(m_sysConfigObj.GetItemValue("ZDOPERAT_SV",blOptExist));
   if (!blOptExist)
   {
	   m_sysConfigObj.AddOneOption("ZDOPERAT_SV","0");
	   m_GlobalOptions.m_LogOpts.ZdOperatSave_Flag = 0;
   }

   m_GlobalOptions.m_LogOpts.DataInErrSave_Flag = (BYTE)atoi(m_sysConfigObj.GetItemValue("DATAINER_SV",blOptExist));
   if (!blOptExist)
   {
	   m_sysConfigObj.AddOneOption("DATAINER_SV","0","",TRUE);
	   m_GlobalOptions.m_LogOpts.DataInErrSave_Flag = 0;
   }

   m_GlobalOptions.m_strLogFileName = m_sysConfigObj.GetItemValue("LOGFILE",blOptExist);
   if (!blOptExist)
   {
	   m_sysConfigObj.AddOneOption("LOGFILE","RUNNING_LOG.mdb","日志数据库文件",TRUE);
	   m_GlobalOptions.m_strLogFileName = "RUNNING_LOG.mdb";
   }
   _sTemp = m_GlobalOptions.m_strLogFileName;
   m_GlobalOptions.m_strLogFileName.Format("%s\\%s",CUdfGlobalFUNC::GetRuningPath(),_sTemp);   //绝对文件路径.重点

   m_GlobalOptions.m_dwLogSaveMonths = (BYTE)atoi(m_sysConfigObj.GetItemValue("SAVELOGLEN",blOptExist));
   if (!blOptExist)
   {
	   m_sysConfigObj.AddOneOption("SAVELOGLEN","2","保存日志数据容量(单位：月,默认：2)",TRUE);
	   m_GlobalOptions.m_dwLogSaveMonths = 2;
   }

   if (m_GlobalOptions.m_dwLogSaveMonths > 3)
   {
	   m_GlobalOptions.m_dwLogSaveMonths = 3;
	   m_sysConfigObj.SetItemValue("SAVELOGLEN","3");
   }


   m_GlobalOptions.m_RtuOpts.blYcList_FLAG = (BYTE)atoi(m_sysConfigObj.GetItemValue("YCLIST_FLAG",blOptExist));
   if (!blOptExist)
   {
	   m_sysConfigObj.AddOneOption("YCLIST_FLAG","0","RTU信息参数");
	   m_GlobalOptions.m_RtuOpts.blYcList_FLAG = 0;
   }

   m_GlobalOptions.m_RtuOpts.blYxList_FLAG = (BYTE)atoi(m_sysConfigObj.GetItemValue("YXLIST_FLAG",blOptExist));
   if (!blOptExist)
   {
	   m_sysConfigObj.AddOneOption("YXLIST_FLAG","0");
	   m_GlobalOptions.m_RtuOpts.blYxList_FLAG = 0;
   }

   m_GlobalOptions.m_RtuOpts.blYkList_FLAG = (BYTE)atoi(m_sysConfigObj.GetItemValue("YKLIST_FLAG",blOptExist));
   if (!blOptExist)
   {
	   m_sysConfigObj.AddOneOption("YKLIST_FLAG","0");
	   m_GlobalOptions.m_RtuOpts.blYkList_FLAG = 0;
   }

   m_GlobalOptions.m_RtuOpts.blZdList_FLAG = (BYTE)atoi(m_sysConfigObj.GetItemValue("ZDLIST_FLAG",blOptExist));
   if (!blOptExist)
   {
	   m_sysConfigObj.AddOneOption("ZDLIST_FLAG","0","",TRUE);
	   m_GlobalOptions.m_RtuOpts.blZdList_FLAG = 0;
   }

   m_GlobalOptions.m_PW.PW_CALC = (WORD)atoi(m_sysConfigObj.GetItemValue("PW_CALC",blOptExist));
   if (!blOptExist)
   {
	   m_sysConfigObj.AddOneOption("PW_CALC","0","PW选项,范围:PW_CALC(0-9),PW_CODE(0-999)",FALSE);
	   m_GlobalOptions.m_PW.PW_CALC = 0;
   }

   m_GlobalOptions.m_PW.PW_CODE = (WORD)atoi(m_sysConfigObj.GetItemValue("PW_CODE",blOptExist));
   if (!blOptExist)
   {
	   m_sysConfigObj.AddOneOption("PW_CODE","0","",TRUE);
	   m_GlobalOptions.m_PW.PW_CODE = 0;
   }
   m_GlobalOptions.m_PW.BuildPWBuf();

   m_GlobalOptions.m_nCheckRtuInterval = (DWORD)atol(m_sysConfigObj.GetItemValue("CHECKSTATE_INTERVEL",blOptExist));
   if (!blOptExist)
   {
	   m_sysConfigObj.AddOneOption("CHECKSTATE_INTERVEL","5","检查遥测终端通信状态时间间隔(单位为：分钟)",TRUE);
	   m_GlobalOptions.m_nCheckRtuInterval = 5;
   }

   m_GlobalOptions.m_nKeepaliveInterval = (u_long)atol(m_sysConfigObj.GetItemValue("KEEPALIVEINTERVAL",blOptExist));
   if (!blOptExist)
   {
	   m_sysConfigObj.AddOneOption("KEEPALIVEINTERVAL","5","Keep alive interval (second) (5 ~ 60)",TRUE);
	   m_GlobalOptions.m_nKeepaliveInterval = 5;
   }
   else 
   {
	   if (m_GlobalOptions.m_nKeepaliveInterval < 5)
		   m_GlobalOptions.m_nKeepaliveInterval = 5;
	   else if (m_GlobalOptions.m_nKeepaliveInterval > 60)
		   m_GlobalOptions.m_nKeepaliveInterval = 60;
   }

   m_GlobalOptions.m_nCheckRepeatInterval = (WORD)atoi(m_sysConfigObj.GetItemValue("CheckRepeatInterval",blOptExist));
   if (!blOptExist)
   {
	   m_sysConfigObj.AddOneOption("CheckRepeatInterval","90","检查重复数据时间间隔 单位:second,默认:90s ,(0～300)",TRUE);
	   m_GlobalOptions.m_nCheckRepeatInterval = 90;
   }
   else 
   {
	   if (m_GlobalOptions.m_nCheckRepeatInterval < 0 )
		   m_GlobalOptions.m_nCheckRepeatInterval = 0;
	   else if (m_GlobalOptions.m_nCheckRepeatInterval > 300)
		   m_GlobalOptions.m_nCheckRepeatInterval = 300;
   }


   m_GlobalOptions.m_dRainCheckPer = atof(m_sysConfigObj.GetItemValue("RAIN_CHECK_PER",blOptExist));
   if (!blOptExist)
   {
	   m_sysConfigObj.AddOneOption("RAIN_CHECK_PER","500","雨量最大值检查");
	   m_GlobalOptions.m_dRainCheckPer = 500.0;
   }
   else
   {
	   if (m_GlobalOptions.m_dRainCheckPer <= 0.0 ||
		   m_GlobalOptions.m_dRainCheckPer >= 500.0)
	   {
		   m_GlobalOptions.m_dRainCheckPer = 500.0;
	   }
   }

   m_GlobalOptions.m_dRainCheckDay = atof(m_sysConfigObj.GetItemValue("RAIN_CHECK_DAY",blOptExist));
   if (!blOptExist)
   {
	   m_sysConfigObj.AddOneOption("RAIN_CHECK_DAY","500","",TRUE);
	   m_GlobalOptions.m_dRainCheckDay = 500.0;
   }
   else
   {
	   if (m_GlobalOptions.m_dRainCheckDay <= 50.0)
	   {
		   m_GlobalOptions.m_dRainCheckDay = 500.0;
	   }
   }
   

   m_GlobalOptions.m_nCheck5Minutes_Count = (UINT)atoi(m_sysConfigObj.GetItemValue("CheckRain_5Minutes",blOptExist));
   if (!blOptExist)
   {
	   m_sysConfigObj.AddOneOption("CheckRain_5Minutes","10","雨量统计时间检查(分钟)");
	   m_GlobalOptions.m_nCheck5Minutes_Count = 10;
   }
   else
   {
	   if (m_GlobalOptions.m_nCheck5Minutes_Count <=0 )
		   m_GlobalOptions.m_nCheck5Minutes_Count = 10;
   }

   m_GlobalOptions.m_nCheck15Minutes_Count = (UINT)atoi(m_sysConfigObj.GetItemValue("CheckRain_15Minutes",blOptExist));
   if (!blOptExist)
   {
	   m_sysConfigObj.AddOneOption("CheckRain_15Minutes","30","");
	   m_GlobalOptions.m_nCheck15Minutes_Count = 30;
   }
   else
   {
	   if (m_GlobalOptions.m_nCheck15Minutes_Count <= 0)
		   m_GlobalOptions.m_nCheck15Minutes_Count = 30;
   }


   m_GlobalOptions.m_nCheck1Hour_Count = (UINT)atoi(m_sysConfigObj.GetItemValue("CheckRain_1Hour",blOptExist));
   if (!blOptExist)
   {
	   m_sysConfigObj.AddOneOption("CheckRain_1Hour","61");
	   m_GlobalOptions.m_nCheck1Hour_Count = 61;
   }
   else
   {
	   if (m_GlobalOptions.m_nCheck1Hour_Count <=0)
		   m_GlobalOptions.m_nCheck1Hour_Count = 61;
   }


   m_GlobalOptions.m_nCheck1Day_Count = (UINT)atoi(m_sysConfigObj.GetItemValue("CheckRain_1Day",blOptExist));
   if (!blOptExist)
   {
	   m_sysConfigObj.AddOneOption("CheckRain_1Day","1440","",TRUE);
	   m_GlobalOptions.m_nCheck1Day_Count = 1440;
   }
   else
   {
	   if (m_GlobalOptions.m_nCheck1Day_Count <=0 )
		   m_GlobalOptions.m_nCheck1Day_Count= 1440;
   }

	m_sysConfigObj.SaveFile();

	return TRUE;
}


BOOL CCMSApp::InitLogSystem()
{
	CMainFrame* pMainFrame = (CMainFrame*)m_pMainWnd;

	if (m_pLogSystem != NULL)
	{
		delete m_pLogSystem;
		m_pLogSystem = NULL;
	}
   
	m_pLogSystem = new CUdfLogSystem(m_GlobalOptions.m_strLogFileName);

	if (m_pLogSystem)
	{
		m_pLogSystem->InitLogSystem();
		if (pMainFrame && pMainFrame->GetSafeHwnd() &&
			pMainFrame->m_wndLogFrame.GetSafeHwnd())
			m_pLogSystem->SetLogFrame(&(pMainFrame->m_wndLogFrame));
	}

	return TRUE;
}
BOOL CCMSApp::InitStartUpWnd()
{
    /*
	if (!m_wndStartUpDlg.Create(IDD_DLG_START))
	{
		return FALSE;
	}
	m_wndStartUpDlg.SendMessage(WM_PAINT,NULL,NULL);
	m_wndStartUpDlg.ShowWindow(SW_HIDE);
	*/

	return TRUE;
}


BOOL CCMSApp::InitRealTimeDB()
{
	if (m_pReadDB != NULL)
	{
		delete m_pReadDB;
		m_pReadDB = NULL;
	}

	m_pReadDB = new CRealTimeDB(m_GlobalOptions.m_strCfgFileName);

	if (m_pReadDB)
	{
		
		m_pReadDB->SetLogSystem(m_pLogSystem);
		m_pReadDB->InitRealTimeDB();
		m_pReadDB->m_AdDivCodeDb.BuildAdDivCodeRealDB();
	}
	else
	{
		return FALSE;
	}
    
	return TRUE;
}


int CCMSApp::ExitInstance()
{
	if(m_pCommServer)
	{
		delete m_pCommServer;
		m_pCommServer = NULL;
	}

	CIOCP_Server::Instance()->StopIOCPServer();

	if (m_pRtuDatasave)
	{
		delete m_pRtuDatasave;
		m_pRtuDatasave = NULL;
	}

	if (m_pReadDB)
	{
		delete m_pReadDB;
		m_pReadDB = NULL;
	}

	if (m_pLogSystem)
	{
		CString logMessage ="";
		logMessage.Format("系统正常退出成功,PROCESS KEY:%s",m_sProcessKey);
		m_pLogSystem->InsertLogMessage(SYS_RUNNING_S_LOG,logMessage);

		delete m_pLogSystem;
		m_pLogSystem = NULL;
	}

	::CloseHandle(m_hMutex);
	CMenuXP::UninitializeHook();
	return CWinApp::ExitInstance();
}


BOOL CCMSApp::SystemLog(_LOG_MESSAGE_TYPE _logType,CString _LogMessage,_Log_level_type _levelType,PSYSTEMTIME pLogTime)
{
	BOOL _Result = FALSE;
	if (m_pLogSystem)
		_Result = m_pLogSystem->InsertLogMessage(_logType,_LogMessage,_levelType,pLogTime);

	return _Result;
}


bool CCMSApp::InitRtuDataSaveSystem()
{
	CMainFrame* pMainFrame = (CMainFrame*)m_pMainWnd;
	CString strMsg = "";
	if (m_pRtuDatasave != NULL)
	{
		delete m_pRtuDatasave;
		m_pRtuDatasave = NULL;
	}
	//if (pMainFrame && pMainFrame->m_wndDataInDbLogDlg.GetSafeHwnd())
	//{
	//	strMsg.Format("启动'数据实时入库'线程...");
	//	pMainFrame->m_wndDataInDbLogDlg.AddOneMessage(strMsg);
	//}

	m_pRtuDatasave = new CUdfRtuDataSave_Server();
	if (m_pRtuDatasave)
	{
		m_pRtuDatasave->SetDataInDbLogWnd(&(pMainFrame->m_wndDataInDbLogDlg));

		CString dataSource = "";
		dataSource.Format("DSN=%s;UID=%s;PWD=%s",
			m_GlobalOptions.m_DbOpts.m_strMISDSN,
			m_GlobalOptions.m_DbOpts.m_strMISUID,
			m_GlobalOptions.m_DbOpts.m_strMISPWD);
		m_pRtuDatasave->SetDataSource(dataSource);

		bool ret = m_pRtuDatasave->Initialization();

		return ret;
	}

	return true;
}

bool CCMSApp::InitProcessKey()
{
	m_sProcessKey = "";

	SYSTEMTIME st;
	GetLocalTime(&st);

	m_sProcessKey.Format("%04X-%02X-%02X-%02X-%02X-%02X-%03X",
		st.wYear,
		st.wMonth,
		st.wDay,
		st.wHour,
		st.wMinute,
		st.wSecond,
		st.wMilliseconds);
	TRACE("PROCESS KEY:%s\r\n",m_sProcessKey);
	return true;
}


bool CCMSApp::InitCommServer()
{
	m_pCommServer = new CCommServer();

	if (!m_pCommServer)
		return false;

	if (m_pCommServer)
	{
		bool ret = m_pCommServer->Initialization();
		return ret;
	}


	return true;
}



BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CenterWindow();
	SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW);

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	if (m_wndCaptionStatic.GetSafeHwnd())
		m_wndCaptionStatic.SetWindowText(pApp->m_GlobalOptions.m_strAppTitie);

	return TRUE;  
	// return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
