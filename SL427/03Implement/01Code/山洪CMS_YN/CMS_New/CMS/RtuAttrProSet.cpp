// RtuAttrProSet.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuAttrProSet.h"
#include "RtuStation.h"
#include "RtuPropertyDlg.h"


// CRtuAttrProSet �Ի���

IMPLEMENT_DYNAMIC(CRtuAttrProSet, CDialog)

CRtuAttrProSet::CRtuAttrProSet(CWnd* pParent /*=NULL*/)
	: CDialog(CRtuAttrProSet::IDD, pParent)
{
	m_pSubStation = NULL;
	m_pTabAttrAutoRpSet = NULL;
	m_pTabAttrWorkMode = NULL;
	m_pTabAttrAlertAnalog = NULL;
	m_pTabAttrWaterQuality = NULL;
	m_pTabAttrAlarmState = NULL;

	m_pTabAttrWaterValume = NULL;									//��ֵ����ˮ��
	m_pTabAttrWaterLevel = NULL;									//ˮλ
	m_pTabAttrWaterPress = NULL;									//ˮѹ
	m_pTabAttrFlow = NULL;											//����
	m_pTabAttrQueryEvent = NULL;
	m_pTabAttrRainValume= NULL;
	m_pTabAttrPulse	= NULL;

	m_pTabAttrDayHisQuery  = NULL;

	m_pParentDlg   = NULL;
}

CRtuAttrProSet::~CRtuAttrProSet()
{
}

void CRtuAttrProSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_RTUPROSET, m_wndProTabCtrl);
}


BEGIN_MESSAGE_MAP(CRtuAttrProSet, CDialog)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CRtuAttrProSet ��Ϣ�������


void CRtuAttrProSet::SetRtuStation(CRtuStation* pSub)
{
	m_pSubStation = pSub;
}

BOOL CRtuAttrProSet::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pTabAttrAutoRpSet = new CRtuAttrAutoRpSet(&m_wndProTabCtrl);
	m_pTabAttrAutoRpSet->SetParentDlg(m_pParentDlg);
	m_pTabAttrAutoRpSet->Create(CRtuAttrAutoRpSet::IDD,&m_wndProTabCtrl);
	m_wndProTabCtrl.AddTab(m_pTabAttrAutoRpSet, "�Ա�ѡ��");


	m_pTabAttrWorkMode = new CRtuAttrWorkMode(&m_wndProTabCtrl);
	m_pTabAttrWorkMode->SetParentDlg(m_pParentDlg);
	m_pTabAttrWorkMode->Create(CRtuAttrWorkMode::IDD, &m_wndProTabCtrl);
	m_wndProTabCtrl.AddTab(m_pTabAttrWorkMode, "��ѯѡ��");

	m_pTabAttrWaterValume = new CRtuAttrWaterValume(&m_wndProTabCtrl);
	m_pTabAttrWaterValume->SetParentDlg(m_pParentDlg);
	m_pTabAttrWaterValume->Create(CRtuAttrWaterValume::IDD,&m_wndProTabCtrl);
	m_wndProTabCtrl.AddTab(m_pTabAttrWaterValume,"ˮ��");

	m_pTabAttrWaterLevel = new CRtuAttrWaterLevel(&m_wndProTabCtrl);
	m_pTabAttrWaterLevel->SetParentDlg(m_pParentDlg);
	m_pTabAttrWaterLevel->Create(CRtuAttrWaterLevel::IDD,&m_wndProTabCtrl);
	m_wndProTabCtrl.AddTab(m_pTabAttrWaterLevel,"ˮλ");


	m_pTabAttrWaterPress = new CRtuAttrWaterPress(&m_wndProTabCtrl);
	m_pTabAttrWaterPress->SetParentDlg(m_pParentDlg);
	m_pTabAttrWaterPress->Create(CRtuAttrWaterPress::IDD,&m_wndProTabCtrl);
	m_wndProTabCtrl.AddTab(m_pTabAttrWaterPress, "ˮѹ");


	m_pTabAttrWaterQuality = new CRtuAttrWaterQuality(&m_wndProTabCtrl);
	m_pTabAttrWaterQuality->SetParentDlg(m_pParentDlg);
	m_pTabAttrWaterQuality->Create(CRtuAttrWaterQuality::IDD,&m_wndProTabCtrl);
	m_wndProTabCtrl.AddTab(m_pTabAttrWaterQuality,"ˮ��");


	m_pTabAttrFlow = new CRtuAttrFlow(&m_wndProTabCtrl);
	m_pTabAttrFlow->SetParentDlg(m_pParentDlg);
	m_pTabAttrFlow->Create(CRtuAttrFlow::IDD,&m_wndProTabCtrl);
	m_wndProTabCtrl.AddTab(m_pTabAttrFlow,"����");


	m_pTabAttrRainValume = new CRtuAttrRainValume(&m_wndProTabCtrl);
	m_pTabAttrRainValume->SetParentDlg(m_pParentDlg);
	m_pTabAttrRainValume->Create(CRtuAttrRainValume::IDD,&m_wndProTabCtrl);
	m_wndProTabCtrl.AddTab(m_pTabAttrRainValume,"����");


	m_pTabAttrAlertAnalog = new CRtuAttrAlertAnalog(&m_wndProTabCtrl);
	m_pTabAttrAlertAnalog->SetParentDlg(m_pParentDlg);
	m_pTabAttrAlertAnalog->Create(CRtuAttrAlertAnalog::IDD,&m_wndProTabCtrl);
	m_wndProTabCtrl.AddTab(m_pTabAttrAlertAnalog,"ģ����");

	m_pTabAttrPulse = new CRtuAttrPulse(&m_wndProTabCtrl);
	m_pTabAttrPulse->SetParentDlg(m_pParentDlg);
	m_pTabAttrPulse->Create(CRtuAttrPulse::IDD,&m_wndProTabCtrl);
	m_wndProTabCtrl.AddTab(m_pTabAttrPulse,"���");

	m_pTabAttrDayHisQuery  = new CRtuAttrDayHisQuery(&m_wndProTabCtrl);
	m_pTabAttrDayHisQuery->SetParentDlg(m_pParentDlg);
	m_pTabAttrDayHisQuery->Create(CRtuAttrDayHisQuery::IDD,&m_wndProTabCtrl);
	m_wndProTabCtrl.AddTab(m_pTabAttrDayHisQuery,"��ʷ�ռ�¼");

	m_pTabAttrQueryEvent = new CRtuAttrQueryEvent(&m_wndProTabCtrl);
	m_pTabAttrQueryEvent->SetParentDlg(m_pParentDlg);
	m_pTabAttrQueryEvent->Create(CRtuAttrQueryEvent::IDD,&m_wndProTabCtrl);
	m_wndProTabCtrl.AddTab(m_pTabAttrQueryEvent,"�¼���¼");

	m_pTabAttrAlarmState = new CRtuAttrAlarmState(&m_wndProTabCtrl);
	m_pTabAttrAlarmState->SetParentDlg(m_pParentDlg);
	m_pTabAttrAlarmState->Create(CRtuAttrAlarmState::IDD,&m_wndProTabCtrl);
	m_wndProTabCtrl.AddTab(m_pTabAttrAlarmState,"�ն�״̬");

	return TRUE;  
	// return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CRtuAttrProSet::Update()
{
	if (m_pTabAttrWorkMode && m_pTabAttrWorkMode->GetSafeHwnd())
		m_pTabAttrWorkMode->Update();

	if (m_pTabAttrAutoRpSet && m_pTabAttrAutoRpSet->GetSafeHwnd())
		m_pTabAttrAutoRpSet->Update();

	if (m_pTabAttrAlertAnalog && m_pTabAttrAlertAnalog->GetSafeHwnd())
		m_pTabAttrAlertAnalog->Update();

	if (m_pTabAttrWaterQuality && m_pTabAttrWaterQuality->GetSafeHwnd())
		m_pTabAttrWaterQuality->Update();

	if (m_pTabAttrAlarmState && m_pTabAttrAlarmState->GetSafeHwnd())
		m_pTabAttrAlarmState->Update();

	if(m_pTabAttrWaterValume  && m_pTabAttrWaterValume->GetSafeHwnd())
		m_pTabAttrWaterValume->Update();

	if (m_pTabAttrWaterLevel && m_pTabAttrWaterLevel->GetSafeHwnd())
		m_pTabAttrWaterLevel->Update();

     if (m_pTabAttrWaterPress && m_pTabAttrWaterPress->GetSafeHwnd())
		 m_pTabAttrWaterPress->Update();

	if (m_pTabAttrFlow && m_pTabAttrFlow->GetSafeHwnd())
		m_pTabAttrFlow->Update();

	if (m_pTabAttrRainValume && m_pTabAttrRainValume->GetSafeHwnd())
		m_pTabAttrRainValume->Update();

	if (m_pTabAttrPulse && m_pTabAttrPulse->GetSafeHwnd())
		m_pTabAttrPulse->Update();

	if (m_pTabAttrQueryEvent && m_pTabAttrQueryEvent->GetSafeHwnd())
		m_pTabAttrQueryEvent->Update();

	if (m_pTabAttrDayHisQuery && m_pTabAttrDayHisQuery->GetSafeHwnd())
		m_pTabAttrDayHisQuery->Update();
}


void CRtuAttrProSet::OnDestroy()
{
	CDialog::OnDestroy();

	if (m_pTabAttrAutoRpSet != NULL)
		delete m_pTabAttrAutoRpSet;

	if (m_pTabAttrWorkMode != NULL)
		delete m_pTabAttrWorkMode;

	if (m_pTabAttrAlertAnalog != NULL)
		delete m_pTabAttrAlertAnalog;

	if (m_pTabAttrWaterQuality != NULL)
		delete m_pTabAttrWaterQuality;

	if (m_pTabAttrAlarmState != NULL)
		delete m_pTabAttrAlarmState;

	 if (m_pTabAttrWaterValume != NULL)
		 delete m_pTabAttrWaterValume;

	if (m_pTabAttrWaterLevel  != NULL)
		delete m_pTabAttrWaterLevel;

	if (m_pTabAttrWaterPress != NULL)
		delete m_pTabAttrWaterPress;

 	if (m_pTabAttrFlow != NULL)
		delete m_pTabAttrFlow;

	if (m_pTabAttrRainValume)
		delete m_pTabAttrRainValume;

	if (m_pTabAttrQueryEvent)
		delete m_pTabAttrQueryEvent;

	if (m_pTabAttrDayHisQuery)
		delete m_pTabAttrDayHisQuery;

	if (m_pTabAttrPulse)
		delete m_pTabAttrPulse;
}
