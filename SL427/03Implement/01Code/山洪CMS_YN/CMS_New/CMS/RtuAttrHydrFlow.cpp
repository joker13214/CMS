// RtuAttrHydrFlow.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuAttrHydrFlow.h"


// CRtuAttrHydrFlow 对话框

IMPLEMENT_DYNAMIC(CRtuAttrHydrFlow, CDialog)

CRtuAttrHydrFlow::CRtuAttrHydrFlow(CWnd* pParent /*=NULL*/)
	: CDialog(CRtuAttrHydrFlow::IDD, pParent)
{
	m_blResult = FALSE;
	m_pSubStation = NULL;
	m_pParentDlg = NULL;

}

CRtuAttrHydrFlow::~CRtuAttrHydrFlow()
{
}

void CRtuAttrHydrFlow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_CBX_SENSORNO ,m_wndSensorNoCbx);
	DDX_Control(pDX,IDC_CBX_TIMETYPE ,m_wndTimeTypeCbx);	
	DDX_Control(pDX,IDC_D_START ,m_wndDateStartDtc);
	DDX_Control(pDX,IDC_D_END ,m_wndDateEndDtc);
	DDX_Control(pDX,IDC_STATIC_START ,m_wndTimeStartLabel);
	DDX_Control(pDX,IDC_STATIC_END ,m_wndTimeEndLabel);
}


BEGIN_MESSAGE_MAP(CRtuAttrHydrFlow, CDialog)
	//ON_BN_CLICKED(IDC_BTN_SETWATERLEVEL,				&CRtuAttrHydrFlow::OnBnClickSetWaterLevel)
	//	ON_BN_CLICKED(IDC_BTN_QUERYWATERLEVELDATA,          &CRtuAttrHydrWaterLevel::OnBnClickQueryWaterLevelData)
	//ON_CBN_SELCHANGE(IDC_CBX_TIMETYPE,                  &CRtuAttrHydrFlow::OnCbnSelChangeTimeType)
	//ON_CBN_SELCHANGE(IDC_CBX_SENSORNO,                  &CRtuAttrHydrFlow::OnCbnSelChangeSensorNo)
//	ON_NOTIFY(GVN_ENDLABELEDIT,IDC_WATERLEVEL_GRIDCTRL, &CRtuAttrHydrWaterLevel::OnGridEndEdit)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CRtuAttrHydrFlow 消息处理程序
BOOL CRtuAttrHydrFlow::OnInitDialog()
{
	return FALSE;

}

void CRtuAttrHydrFlow::SetParentDlg(CRtuPropertyDlg* pDlg)
{

}

void CRtuAttrHydrFlow::Update()
{

}	

void CRtuAttrHydrFlow::UpdateDlg(int attrType,BOOL blResult)
{

}

void CRtuAttrHydrFlow::KillAllTimers()
{

}

void CRtuAttrHydrFlow::SetWndsEnable(BOOL blEnable)
{

}

void CRtuAttrHydrFlow::InitGridItems()
{

}

void CRtuAttrHydrFlow::OnTimer(UINT_PTR nIDEvent)
{
	
}
