// MainFrm.h : CMainFrame 类的接口
//


#pragma once
#include "UdfLogFrame.h"
#include "UdfShowDetailLog.h"
#include "UdfRtuCfgFrame.h"
#include "UdfCfgODBCDlg.h"
#include "UdfCfgLogOptDlg.h"
#include "CMSLeftView.h"
#include "CMSRightView.h"
#include "UdfSplitterWnd.h"
#include "UdfDataInDbLogDlg.h"
#include "CommServerMnDlg.h"
#include "UserSystemDlg.h"
#include "UserLoginDlg.h"
#include "RtuPropertyDlg.h"
#include "ShowLogDlg.h"

#include "XpStyle/MenuXP.h"
#include "XpStyle/StatusBarXP.h"
#include "XpStyle/ToolbarXP.h"

class CMainFrame : public CFrameWnd
{
	
public: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 属性
public:

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员	

	CStatusBarXP m_wndStatusBar;
	CToolBarXP   m_wndToolBar;
	//CComboBoxXP  m_wndComboBox;
public:
	CUdfLogFrame		m_wndLogFrame;			//日志系统窗口 
	CUdfRtuCfgFrame		m_wndRtuCfgFrame;		//Rtu参数配置窗口
	CUdfShowDetailLog*	m_pShowDetailLogDlg;	//显示详细日志信息窗口
	CUdfCfgODBCDlg      m_wndCfgOdbcDlg;		//配置数据库相关参数窗口
	CUdfCfgLogOptDlg    m_wndCfgLogOptDlg;      //日志系统参数配置窗口
	CUdfDataInDbLogDlg  m_wndDataInDbLogDlg;    //Rtu数据实时入库信息显示窗口
	CCommServerMnDlg	m_wndCommServerMnDlg;	//CommServer配置管理窗口
	CUserSystemDlg      m_wndUserSystemDlg;		//用户系统管理窗口
	CUserLoginDlg		m_wndUserLogin;			//用户登录窗口
	CRtuPropertyDlg     m_wndRtuPropertyDlg;    //遥测终端属性显示/操作窗口
	CShowLogDlg         m_wndShowLogDlg;


	//主窗口分隔显示相关
	CCMSLeftView*       m_pLeftView;			//Left View
	CCMSRightView*      m_pRightView;           //Right view
	CUdfSplitterWnd     m_wndSplitterWnd;		//分割窗口控件

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnShowSystemTask(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	DECLARE_MENUXP()
protected:
	bool InitRtuPropertyDlg();			//初始化遥测终端属性操作窗口
public:	
	void IconToTray(void);				// 在系统托盘区添加图标
	void DeleteIconTray();				// 删除托盘区图标
	BOOL CreateOtherFrame();			// 创建其他相应窗口
	BOOL CreateToolbar();				// 创建工具栏
	void ShowOtherFrame(int nCmdShow);	// 显示或隐藏其他所有的窗口
	BOOL InitCheckRtuStateTimer();      // 启动检查遥测终端状态时钟
	void UpdateStatusBarText();         // 更新状态栏信息
	void UpdateFrameOnLogin();			// 更新相关的窗口（隐藏），当用户注销时

	bool GetLoginFlag(BYTE& uType);		//获取用户登录信息, uType为用户类型
	void UpdateClientViews();
	CRtuPropertyDlg* GetRtuPropertyDlg();

	void ShowLog(CString strDate,CString strLog);
public:
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLogSystem();		  //打开软件日志系统模块
	afx_msg void OnUsersSystem();	  //用户管理系统
	afx_msg void OnUserLogin();		  //用户登录/注销
	afx_msg void OnUserEditPwd();	  //修改密码
	afx_msg void OnDestroy();		  //销毁窗口
	afx_msg void OnRtuOptionsCfg();   //RTU参数配置模块
	afx_msg void OnSysOption();       //系统选项
	afx_msg void OnLogOptCfg();		  //日志系统参数配置	
	afx_msg void OnDbOptCfg();        //数据库参数配置
	afx_msg void OnPopShowFrame();    //显示MainFrame
	afx_msg void OnPopHideFrame();    //隐藏MainFrame
	afx_msg void OnRtuDataInDb();     //Rtu信息实时入库信息
	afx_msg void OnCommServerCfg();   //通信服务启动配置管理

	afx_msg void OnGridLineNone();
	afx_msg void OnGridLineVert();
	afx_msg void OnGridLineHorz();
	afx_msg void OnGridLineBoth();

	afx_msg void OnUpdateUIEditPwd(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUIGridLineNone(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUIGridLineVert(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUIGridLineHorz(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUIGridLineBoth(CCmdUI* pCmdUI);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

	
};


