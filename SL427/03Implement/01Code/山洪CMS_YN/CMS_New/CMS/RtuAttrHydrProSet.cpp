// RtuAttrHydrProSet.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuAttrHydrProSet.h"


// CRtuAttrHydrProSet 对话框

IMPLEMENT_DYNAMIC(CRtuAttrHydrProSet, CDialog)

CRtuAttrHydrProSet::CRtuAttrHydrProSet(CWnd* pParent /*=NULL*/)
	: CDialog(CRtuAttrHydrProSet::IDD, pParent)
{
	m_pHydrWorkCondition = NULL;
	m_pHydrTransCtrl = NULL;
	m_pHydrTabWaterLevel = NULL;
	m_pHydrTabRainValume = NULL;
	m_pHydrSoilMoisture = NULL;
	m_pHydrInDepthGW = NULL;
	m_pHydrInTGW = NULL;
	m_pHydrEvaporate = NULL;
	m_pHydrInterfaceInfo = NULL;

}

CRtuAttrHydrProSet::~CRtuAttrHydrProSet()
{
}

void CRtuAttrHydrProSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_PROSET_HYDR, m_wndProTabCtrl);
}


BEGIN_MESSAGE_MAP(CRtuAttrHydrProSet, CDialog)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CRtuAttrHydrProSet 消息处理程序

void CRtuAttrHydrProSet::SetRtuStation(CRtuStation* pSub)
{
	m_pSubStation = pSub;
}

BOOL CRtuAttrHydrProSet::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pHydrTransCtrl = new RtuAttrHydrTransCtrl(&m_wndProTabCtrl);
	m_pHydrTransCtrl->SetParentDlg(m_pParentDlg);
	m_pHydrTransCtrl->Create(RtuAttrHydrTransCtrl::IDD,&m_wndProTabCtrl);
	m_wndProTabCtrl.AddTab(m_pHydrTransCtrl,"传输控制");
	
	m_pHydrWorkCondition = new CRtuAttrHydrWorkCondition(&m_wndProTabCtrl);
	m_pHydrWorkCondition->SetParentDlg(m_pParentDlg);
	m_pHydrWorkCondition->Create(CRtuAttrHydrWorkCondition::IDD,&m_wndProTabCtrl);
	m_wndProTabCtrl.AddTab(m_pHydrWorkCondition,"工况信息");

	m_pHydrInterfaceInfo = new CRtuAttrHydrInterfaceInfo(&m_wndProTabCtrl);
	m_pHydrInterfaceInfo->SetParentDlg(m_pParentDlg);
	m_pHydrInterfaceInfo->Create(CRtuAttrHydrInterfaceInfo::IDD, &m_wndProTabCtrl);
	m_wndProTabCtrl.AddTab(m_pHydrInterfaceInfo,"接口信息");

	//------------水位-------------------
 	m_pHydrTabWaterLevel = new CRtuAttrHydrWaterLevel(&m_wndProTabCtrl);
 	m_pHydrTabWaterLevel->SetParentDlg(m_pParentDlg);
 	m_pHydrTabWaterLevel->Create(CRtuAttrHydrWaterLevel::IDD,&m_wndProTabCtrl);
 	m_wndProTabCtrl.AddTab(m_pHydrTabWaterLevel,"水位");
	//------------雨量-------------------

	m_pHydrTabRainValume = new CRtuAttrHydrRain(&m_wndProTabCtrl);
	m_pHydrTabRainValume->SetParentDlg(m_pParentDlg);
	m_pHydrTabRainValume->Create(CRtuAttrHydrRain::IDD, &m_wndProTabCtrl);
	m_wndProTabCtrl.AddTab(m_pHydrTabRainValume, "雨量");

	//------------土地墒情------------------
	m_pHydrSoilMoisture = new CRtuAttrHydrSoilMoisture(&m_wndProTabCtrl);
	m_pHydrSoilMoisture->SetParentDlg(m_pParentDlg);
	m_pHydrSoilMoisture->Create(CRtuAttrHydrSoilMoisture::IDD, &m_wndProTabCtrl);
	m_wndProTabCtrl.AddTab(m_pHydrSoilMoisture, "土地墒情");

	//------------地下水瞬时埋深------------------
	m_pHydrInDepthGW = new CRtuAttrHydrInDepthGW(&m_wndProTabCtrl);
	m_pHydrInDepthGW->SetParentDlg(m_pParentDlg);
	m_pHydrInDepthGW->Create(CRtuAttrHydrInDepthGW::IDD, &m_wndProTabCtrl);
	m_wndProTabCtrl.AddTab(m_pHydrInDepthGW, "地下水瞬时埋深");
	

	//------------地下水瞬时温度------------------
	m_pHydrInTGW = new CRtuAttrHydrInTGW(&m_wndProTabCtrl);
	m_pHydrInTGW->SetParentDlg(m_pParentDlg);
	m_pHydrInTGW->Create(CRtuAttrHydrInTGW::IDD, &m_wndProTabCtrl);
	m_wndProTabCtrl.AddTab(m_pHydrInTGW, "地下水瞬时温度");

	//------------蒸发------------------
	m_pHydrEvaporate = new CRtuAttrHydrEvaporate(&m_wndProTabCtrl);
	m_pHydrEvaporate->SetParentDlg(m_pParentDlg);
	m_pHydrEvaporate->Create(CRtuAttrHydrEvaporate::IDD, &m_wndProTabCtrl);
	m_wndProTabCtrl.AddTab(m_pHydrEvaporate, "蒸发");

	return TRUE;
}

void CRtuAttrHydrProSet::OnDestroy()
{
	CDialog::OnDestroy();

	if (m_pHydrTabWaterLevel)
	{
		delete m_pHydrTabWaterLevel;
		m_pHydrTabWaterLevel = 0;
	}

	if (m_pHydrTabRainValume)
	{
		delete m_pHydrTabRainValume;
		m_pHydrTabRainValume = 0;
	}

	if (m_pHydrSoilMoisture)
	{
		delete m_pHydrSoilMoisture;
		m_pHydrSoilMoisture = 0;
	}


	if (m_pHydrInDepthGW)
	{
		delete m_pHydrInDepthGW;
		m_pHydrInDepthGW = 0;
	}


	if (m_pHydrInTGW)
	{
		delete m_pHydrInTGW;
		m_pHydrInTGW = 0;
	}

	if (m_pHydrEvaporate)
	{
		delete m_pHydrEvaporate;
		m_pHydrEvaporate = 0;
	}

	if (m_pHydrTransCtrl)
	{
		delete m_pHydrTransCtrl;
		m_pHydrTransCtrl = 0;
	}



	if (m_pHydrWorkCondition)
	{
		delete m_pHydrWorkCondition;
		m_pHydrWorkCondition = 0;
	}

	if (m_pHydrInterfaceInfo)
	{
		delete m_pHydrInterfaceInfo;
		m_pHydrInterfaceInfo = 0;
	}


}

void CRtuAttrHydrProSet::Update()
{
	if (m_pHydrTransCtrl && m_pHydrTransCtrl->GetSafeHwnd())
	{
		m_pHydrTransCtrl->Update();
	}

	if (m_pHydrWorkCondition && m_pHydrWorkCondition->GetSafeHwnd())
		m_pHydrWorkCondition->Update();

	if (m_pHydrTabWaterLevel && m_pHydrTabWaterLevel->GetSafeHwnd())
		m_pHydrTabWaterLevel->Update();

	if (m_pHydrTabRainValume && m_pHydrTabRainValume->GetSafeHwnd())
		m_pHydrTabRainValume->Update();

	if (m_pHydrSoilMoisture && m_pHydrSoilMoisture->GetSafeHwnd())
		m_pHydrSoilMoisture->Update();

	if (m_pHydrInDepthGW && m_pHydrInDepthGW->GetSafeHwnd())
		m_pHydrInDepthGW->Update();

	if (m_pHydrInTGW && m_pHydrInTGW->GetSafeHwnd())
		m_pHydrInTGW->Update();

	if (m_pHydrEvaporate && m_pHydrEvaporate->GetSafeHwnd())
		m_pHydrEvaporate->Update();

	if (m_pHydrInterfaceInfo && m_pHydrInterfaceInfo->GetSafeHwnd())
		m_pHydrInterfaceInfo->Update();


	

}
