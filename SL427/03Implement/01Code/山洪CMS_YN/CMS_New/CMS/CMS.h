// CMS.h : CMS Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������

#include "UdfGlobalFUNC.h"
#include "UdfLogSystem.h"
#include "RealTimeDB.h"
#include "UdfRtuDataSave.h"
#include "ConfigFile.h"
#include "UdfStartUpDlg.h"
#include "CommServer.h"
#include "RtuStation.h"

// CCMSApp:
// �йش����ʵ�֣������ CMS.cpp
//
class CCMSApp : public CWinApp
{
public:
	CCMSApp();

	BOOL LoadConfigFile();																							//��ȡ/���������ļ�
	BOOL SystemLog(_LOG_MESSAGE_TYPE _logType,CString _LogMessage,_Log_level_type _levelType = Normal_Log_Message,PSYSTEMTIME pLogTime = NULL); //�洢��־��Ϣ  Log information
protected:
	BOOL InitStartUpWnd();
	BOOL InitLogSystem();								//������־ϵͳ(Log System)
	BOOL InitRealTimeDB();								//�����������ݿ�
	bool InitRtuDataSaveSystem();						//����ʵʱ���ݿ�ת��ӿڷ���
	bool InitProcessKey();								//����Process key
	BOOL SaveStartUpLog();                              //�洢���������־��Ϣ
	bool InitCommServer();                              //��ʼ��������ͨ�ŷ�����

	bool CheckProcessRunning();							//�ж��Ƿ�����Ѿ����У���֤ϵͳֻ����һ�����̣�
public:
	CMS_GLOBAL_OPTIONS				m_GlobalOptions;				//����ʱ��ص����в�����Ϣ
	CUdfLogSystem*					m_pLogSystem;					//�����־ϵͳ
	CRealTimeDB*					m_pReadDB;						//RTU��������ʵʱ���ݿ�
	CUdfRtuDataSave_Server*			m_pRtuDatasave;					//�ɼ�����ʵʱ���ϵͳ
	JDGM_CONFIG_FILE::CConfigFile	m_sysConfigObj;                 //����ϵͳ�����ļ�����
	//CUdfStartUpDlg                  m_wndStartUpDlg;				//��������
	CCommServer*                    m_pCommServer;                  //ͨ�ŷ�����

	//����
	CString             m_sProcessKey;                  //���н���ΨһKey,��ѯ����Ƿ������������˳�
	HANDLE				m_hMutex;						//Ϊ��ֻ����һ������ ��ȫ������

protected:
	CUserObject* m_pLoginUser;							//��ǰ��¼���û�
	bool         m_bLogined;                            //�Ƿ��¼�ı�־ 
	CRtuStation* m_pCurOpStation;						//��ǰ������ң���ն˶���,Ĭ��NULL
public:
	void         SetLoginUser(CUserObject* pLoginUser)	{m_pLoginUser = pLoginUser;}
	void		 SetLoginFlag(bool blLogined)			{m_bLogined = blLogined;}
	CUserObject* GetLoginUser()							{return m_pLoginUser;}
	const bool   GetLoginFlag()							{return m_bLogined;}

	void         Set_Cur_Op_Station(CRtuStation* pCur_Station){m_pCurOpStation = pCur_Station;}
	CRtuStation* Get_Cur_Op_Station(){return m_pCurOpStation;}
// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

};

extern CCMSApp theApp;