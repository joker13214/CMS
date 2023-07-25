// MainFrm.h : CMainFrame ��Ľӿ�
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
	
public: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա	

	CStatusBarXP m_wndStatusBar;
	CToolBarXP   m_wndToolBar;
	//CComboBoxXP  m_wndComboBox;
public:
	CUdfLogFrame		m_wndLogFrame;			//��־ϵͳ���� 
	CUdfRtuCfgFrame		m_wndRtuCfgFrame;		//Rtu�������ô���
	CUdfShowDetailLog*	m_pShowDetailLogDlg;	//��ʾ��ϸ��־��Ϣ����
	CUdfCfgODBCDlg      m_wndCfgOdbcDlg;		//�������ݿ���ز�������
	CUdfCfgLogOptDlg    m_wndCfgLogOptDlg;      //��־ϵͳ�������ô���
	CUdfDataInDbLogDlg  m_wndDataInDbLogDlg;    //Rtu����ʵʱ�����Ϣ��ʾ����
	CCommServerMnDlg	m_wndCommServerMnDlg;	//CommServer���ù�����
	CUserSystemDlg      m_wndUserSystemDlg;		//�û�ϵͳ������
	CUserLoginDlg		m_wndUserLogin;			//�û���¼����
	CRtuPropertyDlg     m_wndRtuPropertyDlg;    //ң���ն�������ʾ/��������
	CShowLogDlg         m_wndShowLogDlg;


	//�����ڷָ���ʾ���
	CCMSLeftView*       m_pLeftView;			//Left View
	CCMSRightView*      m_pRightView;           //Right view
	CUdfSplitterWnd     m_wndSplitterWnd;		//�ָ�ڿؼ�

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnShowSystemTask(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	DECLARE_MENUXP()
protected:
	bool InitRtuPropertyDlg();			//��ʼ��ң���ն����Բ�������
public:	
	void IconToTray(void);				// ��ϵͳ���������ͼ��
	void DeleteIconTray();				// ɾ��������ͼ��
	BOOL CreateOtherFrame();			// ����������Ӧ����
	BOOL CreateToolbar();				// ����������
	void ShowOtherFrame(int nCmdShow);	// ��ʾ�������������еĴ���
	BOOL InitCheckRtuStateTimer();      // �������ң���ն�״̬ʱ��
	void UpdateStatusBarText();         // ����״̬����Ϣ
	void UpdateFrameOnLogin();			// ������صĴ��ڣ����أ������û�ע��ʱ

	bool GetLoginFlag(BYTE& uType);		//��ȡ�û���¼��Ϣ, uTypeΪ�û�����
	void UpdateClientViews();
	CRtuPropertyDlg* GetRtuPropertyDlg();

	void ShowLog(CString strDate,CString strLog);
public:
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLogSystem();		  //�������־ϵͳģ��
	afx_msg void OnUsersSystem();	  //�û�����ϵͳ
	afx_msg void OnUserLogin();		  //�û���¼/ע��
	afx_msg void OnUserEditPwd();	  //�޸�����
	afx_msg void OnDestroy();		  //���ٴ���
	afx_msg void OnRtuOptionsCfg();   //RTU��������ģ��
	afx_msg void OnSysOption();       //ϵͳѡ��
	afx_msg void OnLogOptCfg();		  //��־ϵͳ��������	
	afx_msg void OnDbOptCfg();        //���ݿ��������
	afx_msg void OnPopShowFrame();    //��ʾMainFrame
	afx_msg void OnPopHideFrame();    //����MainFrame
	afx_msg void OnRtuDataInDb();     //Rtu��Ϣʵʱ�����Ϣ
	afx_msg void OnCommServerCfg();   //ͨ�ŷ����������ù���

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


