// RtuAttrSyqProSet.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuAttrSyqProSet.h"


// CRtuAttrSyqProSet 对话框

IMPLEMENT_DYNAMIC(CRtuAttrSyqProSet, CDialog)

CRtuAttrSyqProSet::CRtuAttrSyqProSet(CWnd* pParent /*=NULL*/)
	: CDialog(CRtuAttrSyqProSet::IDD, pParent)
{
	m_pSyqcfg1 = NULL;
	m_pSyqcfg2 = NULL;
	m_pSyqcfg3 = NULL;
	m_pSyqCalldata = NULL;
	m_pSyqCalldata1 = NULL;
	m_pSyqCalldata2 = NULL;
	m_pSyqCallState = NULL;
}

CRtuAttrSyqProSet::~CRtuAttrSyqProSet()
{
}

void CRtuAttrSyqProSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_PROSET_SYQ, m_wndProTabCtrl);
}


BEGIN_MESSAGE_MAP(CRtuAttrSyqProSet, CDialog)
END_MESSAGE_MAP()


BOOL CRtuAttrSyqProSet::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pSyqcfg1 = new CRtuAttrSyqcfg1(&m_wndProTabCtrl);
	m_pSyqcfg1->SetParentDlg(m_pParentDlg);
	m_pSyqcfg1->Create(CRtuAttrSyqcfg1::IDD,&m_wndProTabCtrl);
	m_wndProTabCtrl.AddTab(m_pSyqcfg1,"遥测站参数配置1");

	m_pSyqcfg2 = new CRtuAttrSyqcfg2(&m_wndProTabCtrl);
	m_pSyqcfg2->SetParentDlg(m_pParentDlg);
	m_pSyqcfg2->Create(CRtuAttrSyqcfg2::IDD,&m_wndProTabCtrl);
	m_wndProTabCtrl.AddTab(m_pSyqcfg2,"遥测站参数配置2");

	m_pSyqcfg3 = new CRtuAttrSyqcfg3(&m_wndProTabCtrl);
	m_pSyqcfg3->SetParentDlg(m_pParentDlg);
	m_pSyqcfg3->Create(CRtuAttrSyqcfg3::IDD,&m_wndProTabCtrl);
	m_wndProTabCtrl.AddTab(m_pSyqcfg3,"遥测站参数配置3");


	m_pSyqCallState	 = new CRtuAttrSyqCallState(&m_wndProTabCtrl);
	m_pSyqCallState->SetParentDlg(m_pParentDlg);
	m_pSyqCallState->Create(CRtuAttrSyqCallState::IDD,&m_wndProTabCtrl);
	m_wndProTabCtrl.AddTab(m_pSyqCallState,"召唤状态和报警");

	m_pSyqCalldata1 = new CRtuAttrSyqCallData1(&m_wndProTabCtrl);
	m_pSyqCalldata1->SetParentDlg(m_pParentDlg);
	m_pSyqCalldata1->Create(CRtuAttrSyqCallData1::IDD,&m_wndProTabCtrl);
	m_wndProTabCtrl.AddTab(m_pSyqCalldata1,"召唤实时数据");

	m_pSyqCalldata2 = new CRtuAttrSyqCallData2(&m_wndProTabCtrl);
	m_pSyqCalldata2->SetParentDlg(m_pParentDlg);
	m_pSyqCalldata2->Create(CRtuAttrSyqCallData2::IDD,&m_wndProTabCtrl);
	m_wndProTabCtrl.AddTab(m_pSyqCalldata2,"召唤指定要素数据");


	m_pSyqCalldata = new CRtuAttrSyqCalldata(&m_wndProTabCtrl);
	m_pSyqCalldata->SetParentDlg(m_pParentDlg);
	m_pSyqCalldata->Create(CRtuAttrSyqCalldata::IDD,&m_wndProTabCtrl);
	m_wndProTabCtrl.AddTab(m_pSyqCalldata,"召唤指定时段数据");


	return TRUE;
}

void CRtuAttrSyqProSet::Update()
{
	if (m_pSyqcfg1 && m_pSyqcfg1->GetSafeHwnd())
	{
		m_pSyqcfg1->Update();
	}

	if (m_pSyqcfg2 && m_pSyqcfg2->GetSafeHwnd())
	{
		m_pSyqcfg2->Update();
	}
	if (m_pSyqcfg3 && m_pSyqcfg2->GetSafeHwnd())
	{
		m_pSyqcfg3->Update();
	}

	if (m_pSyqCalldata && m_pSyqCalldata->GetSafeHwnd())
	{
		m_pSyqCalldata->Update();
	}
	if(m_pSyqCalldata1 && m_pSyqCalldata1->GetSafeHwnd())
	{
		m_pSyqCalldata1->Update();
	}
	if(m_pSyqCalldata2 && m_pSyqCalldata2->GetSafeHwnd())
	{
		m_pSyqCalldata2->Update();
	}
	if(m_pSyqCallState && m_pSyqCallState->GetSafeHwnd())
	{
		m_pSyqCallState->Update();
	}

}