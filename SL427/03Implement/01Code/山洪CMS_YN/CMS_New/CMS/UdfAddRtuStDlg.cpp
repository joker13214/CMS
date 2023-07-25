// UdfAddRtuStDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "UdfAddRtuStDlg.h"
#include "RealTimeDB.h"
#include "UdfComInfoDlg.h"
#include "UdfNetInfoDlg.h"


// CUdfAddRtuStDlg 对话框

IMPLEMENT_DYNAMIC(CUdfAddRtuStDlg, CDialog)

CUdfAddRtuStDlg::CUdfAddRtuStDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUdfAddRtuStDlg::IDD, pParent)
	, m_dwA2(0)
	,m_syqcenter(1)
	, m_sName(_T(""))
	, m_strCaption(_T(""))
	, m_bFirstCfg(true)
{

	m_strStCode = "";
	m_ProvinceCode = 0; 
	m_RegionCode = 0 ;
	m_CountyCode = 0;
	m_A3Code = 0;
	m_CmType = 0;
	m_Interface = 0;
	m_OperateFlag = RECORD_NEW;

	m_pCurRtuStation = NULL;
	m_nAppProVer = 1;
	m_nAppProtoType = (BYTE)WaterResources_Protocol;
	m_nScFactoryType = SCFactoryType_GTDZ;
	m_nScAddress = 0;
}

CUdfAddRtuStDlg::~CUdfAddRtuStDlg()
{
}

void CUdfAddRtuStDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_RROVINCE, m_wndCmbProvince);
	DDX_Control(pDX, IDC_CMB_REGION, m_wndCmbRegion);
	DDX_Control(pDX, IDC_CMB_COUNTY, m_wndCmbCounty);
	DDX_Control(pDX, IDC_CMB_A3, m_wndCmbA3);
	DDX_Control(pDX, IDC_CMB_CMTYPE, m_wndCmbCmType);
	DDX_Control(pDX, IDC_CMB_APPPROTOTYPE, m_wndCmbAppProType);
	DDX_Control(pDX, IDC_CMB_INTERFACE, m_wndCmbInterface);
	DDX_Text(pDX, IDC_EDI_A2, m_dwA2);
	DDX_Control(pDX, IDC_EDI_A2, m_wndLetA2);
	DDV_MinMaxUInt(pDX, m_dwA2, 0, 2097150);
	DDX_Text(pDX, IDC_EDI_NAME, m_sName);
	DDX_Text(pDX, IDC_EDI_SYQCENTER, m_syqcenter);
	DDV_MinMaxUInt(pDX, m_syqcenter, 0, 65535);
	DDV_MaxChars(pDX, m_sName, 50);
	DDX_Control(pDX, IDC_BTN_INTERFACECFG, m_wndBtnInterfaceCfg);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_PIC_CMPRO, m_wndGridParent);
	DDX_Control(pDX, IDC_CMB_APPPROVER, m_wndCmbAppProVer);
	DDX_Control(pDX, IDC_STATIC_SCFC, m_wndStaticScFactory);	
	DDX_Control(pDX, IDC_EDIT_ADDVCODE, m_wndEditAddvCode);
	DDX_Control(pDX, IDC_CMB_SCFACTORY, m_wndCmbScFactory);
	DDX_Control(pDX, IDC_STATIC_SCSW008, m_wndStaticSCSW008);
	DDX_Control(pDX, IDC_LET_SCSW008, m_wndLetSCSW008);
	DDX_Control(pDX, IDC_STATIC_SYQCENTER, m_wndStaticSYQCENTER);
	DDX_Control(pDX, IDC_STATIC_SYQ_YCADD, m_wndStaticSYQADDR);
	DDX_Control(pDX, IDC_EDI_SYQCENTER, m_wndLetSYQCenter);
	DDX_Control(pDX, IDC_EDI_SYQADDR, m_wndLetSYQAddr);
	DDX_Text(pDX, IDC_LET_SCSW008, m_strSCSW008Code);
	DDX_Text(pDX,IDC_EDI_SYQADDR,m_strSYQADDR);
	//DDV_MinMaxUInt(pDX, m_strSYQADDR, 0, 99999999);

	//DDV_MaxChars(pDX, m_strSYQADDR, 4);
	DDV_MaxChars(pDX, m_strSCSW008Code, 10);
	
	
}


BEGIN_MESSAGE_MAP(CUdfAddRtuStDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_CMB_RROVINCE, &CUdfAddRtuStDlg::OnCbnSelchangeCmbRrovince)
	ON_CBN_SELCHANGE(IDC_CMB_REGION, &CUdfAddRtuStDlg::OnCbnSelchangeCmbRegion)
	ON_BN_CLICKED(IDC_BTN_INTERFACECFG, &CUdfAddRtuStDlg::OnBnClickedBtnInterfacecfg)
	ON_CBN_SELCHANGE(IDC_CMB_INTERFACE, &CUdfAddRtuStDlg::OnCbnSelchangeCmbInterface)
	ON_NOTIFY(GVN_ENDLABELEDIT,IDC_ADDSTRTU_GRID, &CUdfAddRtuStDlg::OnGridEndEdit)
	ON_CBN_SELCHANGE(IDC_CMB_CMTYPE, &CUdfAddRtuStDlg::OnCbnSelchangeCmbCmtype)
	ON_CBN_SELCHANGE(IDC_CMB_APPPROTOTYPE, &CUdfAddRtuStDlg::OnCbnSelchangeAppProType)
	ON_EN_CHANGE(IDC_EDIT_ADDVCODE, &CUdfAddRtuStDlg::OnEnChangeEditAddvcode)
	ON_CBN_SELCHANGE(IDC_CMB_COUNTY, &CUdfAddRtuStDlg::OnCbnSelchangeCmbCounty)
END_MESSAGE_MAP()


// CUdfAddRtuStDlg 消息处理程序
void CUdfAddRtuStDlg::OnCbnSelchangeCmbRrovince()
{
	if (m_wndCmbProvince.GetSafeHwnd() && m_wndCmbProvince.GetCurSel() != CB_ERR )
	{
		int nSel = m_wndCmbProvince.GetCurSel();
		m_wndCmbRegion.ResetContent();

		int nRegionSel = 0;
		bool bFinded = false;

		CUdfProvinceCode* pProvince = (CUdfProvinceCode*)m_wndCmbProvince.GetItemDataPtr(nSel);
		if (pProvince)
		{

			std::map<BYTE,CUdfRegionCode*>::iterator It;
			for (It = pProvince->m_mapRegionCode.begin();
				 It != pProvince->m_mapRegionCode.end();
				 It ++)
			{
				if (It->second)
				{
					int idx = m_wndCmbRegion.AddString(It->second->GetName());
					m_wndCmbRegion.SetItemDataPtr(idx,It->second);
					if (It->second->GetZoningCode() == m_RegionCode)
					{
						nRegionSel = idx;
						bFinded = true;
					}
				}
			}
		}

		if (m_wndCmbRegion.GetCount())
		{
			if (bFinded)
				m_wndCmbRegion.SetCurSel(nRegionSel);
			else
				m_wndCmbRegion.SetCurSel(0);
			OnCbnSelchangeCmbRegion();
		}
	}
}

void CUdfAddRtuStDlg::OnCbnSelchangeCmbRegion()
{
	if (m_wndCmbRegion.GetSafeHwnd() && m_wndCmbRegion.GetCurSel() != CB_ERR)
	{
		int nSel = m_wndCmbRegion.GetCurSel();
		m_wndCmbCounty.ResetContent();

		int nCountySel = 0;
		bool bFinded = false;

		CUdfRegionCode* pReg = (CUdfRegionCode*)m_wndCmbRegion.GetItemDataPtr(nSel);
		if (pReg)
		{
			std::map<BYTE,CUdfCountyCode*>::iterator It ;
			for (It = pReg->m_mapCountyCode.begin();
				 It != pReg->m_mapCountyCode.end();
				 It ++)
			{
				if (It->second)
				{
					int idx = m_wndCmbCounty.AddString(It->second->GetName());
					m_wndCmbCounty.SetItemDataPtr(idx,It->second);
					if (It->second->GetZoningCode() == m_CountyCode)
					{
						nCountySel = idx;
						bFinded = true;
					}
				}
			}
			if (m_wndCmbCounty.GetCount())
			{
				if (bFinded)
					m_wndCmbCounty.SetCurSel(nCountySel);
				else
					m_wndCmbCounty.SetCurSel(0);
			}

			OnCbnSelchangeCmbCounty();
		}
	}
}

BOOL CUdfAddRtuStDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CWnd* pWnd = GetDlgItem(IDC_PIC_CMPRO);
	CRect rect1, rect2;
	int captionHeight = ::GetSystemMetrics(SM_CYCAPTION);
	int cxframe = GetSystemMetrics(SM_CXFRAME);
	int cyframe = GetSystemMetrics(SM_CYFRAME);
	this->GetWindowRect(&rect2);
	if (pWnd)
		pWnd->GetWindowRect(&rect1);
	if (!m_wndCmOptGridCtrl.Create(CRect(rect1.left - rect2.left - cxframe, 
		rect1.top-rect2.top - cyframe-captionHeight, 
		rect1.left + rect1.Width() - rect2.left, 
		rect1.top + rect1.Height() - rect2.top - captionHeight), 
		this, IDC_ADDSTRTU_GRID,WS_CHILD|WS_VISIBLE))
	{
		TRACE("Failed to create threads  grid!\n");
		return FALSE;
	}

	m_wndCmOptGridCtrl.SetBkColor(RGB(255,255,255));
	m_wndCmOptGridCtrl.SetListMode(TRUE);
	m_wndCmOptGridCtrl.SetSingleColSelection(TRUE);
	m_wndCmOptGridCtrl.SetSingleRowSelection(TRUE);

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	UpdateData(FALSE);

	int nSel = 0;
	bool bFinded = false;
	if (m_wndCmbProvince.GetSafeHwnd())
	{
		m_wndCmbProvince.ResetContent();

		std::map<BYTE,CUdfProvinceCode*>::iterator It ;
		for (It = pApp->m_pReadDB->m_AdDivCodeDb.m_mapProvinceCode.begin();
			 It  != pApp->m_pReadDB->m_AdDivCodeDb.m_mapProvinceCode.end();
			 It ++)
		{
			if (It->second)
			{
				int idx = m_wndCmbProvince.AddString(It->second->GetName());
				m_wndCmbProvince.SetItemDataPtr(idx,It->second);
				if (It->second->GetZoningCode() == m_ProvinceCode)
				{
					nSel = idx;
					bFinded = true;
				}
			}
		}

		if (m_wndCmbProvince.GetCount())
		{
			if (bFinded)
				m_wndCmbProvince.SetCurSel(nSel);
			else
				m_wndCmbProvince.SetCurSel(0);
			OnCbnSelchangeCmbRrovince();
		}
	}

	if (m_wndCmbA3.GetSafeHwnd())
	{
		m_wndCmbA3.ResetContent();
		int idx = 0;
		idx = m_wndCmbA3.AddString("终端地址");
		m_wndCmbA3.SetItemData(idx,2);

		if (m_A3Code == 2)
			nSel  = idx;

		idx = m_wndCmbA3.AddString("中继地址");
		m_wndCmbA3.SetItemData(idx,5);
		if (m_A3Code == 5)
			nSel = idx;

		if (m_wndCmbA3.GetCount())
		{
			m_wndCmbA3.SetCurSel(nSel);
		}
	}

	if (m_wndCmbAppProVer.GetSafeHwnd())
	{
		m_wndCmbAppProVer.ResetContent();
		int Idx = 0,nSel = -1;
		Idx = m_wndCmbAppProVer.AddString("V1.0 版");
		m_wndCmbAppProVer.SetItemData(Idx,1);
		if (1 == m_nAppProVer)
			nSel = Idx;

		Idx = m_wndCmbAppProVer.AddString("V2.0 版");
		m_wndCmbAppProVer.SetItemData(Idx,2);
		if (2 == m_nAppProVer)
			nSel = Idx;

		m_wndCmbAppProVer.SetCurSel(nSel);
	}

	if (m_wndCmbAppProType.GetSafeHwnd())
	{
		m_wndCmbAppProType.ResetContent();
		int Idx = 0, nSel = -1;
		Idx = m_wndCmbAppProType.AddString("水资源SL427-2008规约");
		m_wndCmbAppProType.SetItemData(Idx,(BYTE)WaterResources_Protocol);
		if ((BYTE)WaterResources_Protocol == m_nAppProtoType)
			nSel = Idx;

		Idx = m_wndCmbAppProType.AddString("水文SCSW008规约");
		m_wndCmbAppProType.SetItemData(Idx,(BYTE)SCSW_008_Protocol);
		if ((BYTE)SCSW_008_Protocol == m_nAppProtoType)
			nSel = Idx;


		Idx = m_wndCmbAppProType.AddString("水雨情遥测系统数据通信规约");
		m_wndCmbAppProType.SetItemData(Idx,(BYTE)SYQYC_Comm_Protocol);
		if ((BYTE)SYQYC_Comm_Protocol == m_nAppProtoType)
			nSel = Idx;

		m_wndCmbAppProType.SetCurSel(nSel);
	}


	if (m_wndCmbScFactory.GetSafeHwnd())
	{
		m_wndCmbScFactory.ResetContent();
		int Idx = 0, nSel = -1;
		Idx = m_wndCmbScFactory.AddString(CUdfGlobalFUNC::GetSCFactoryString(SCFactoryType_GTDZ));
		m_wndCmbScFactory.SetItemData(Idx,SCFactoryType_GTDZ);
		if (m_nScFactoryType == SCFactoryType_GTDZ)
			nSel = Idx;
		
		Idx = m_wndCmbScFactory.AddString(CUdfGlobalFUNC::GetSCFactoryString(SCFactoryType_SZTH));
		m_wndCmbScFactory.SetItemData(Idx,SCFactoryType_SZTH);
		if (m_nScFactoryType == SCFactoryType_SZTH)
			nSel = Idx;

		m_wndCmbScFactory.SetCurSel(nSel);
	}

	if ( m_wndCmbCmType.GetSafeHwnd())
	{
		m_wndCmbCmType.ResetContent();
		int idx =  0;
		idx = m_wndCmbCmType.AddString("请选择");
		m_wndCmbCmType.SetItemData(idx,0);


		idx = m_wndCmbCmType.AddString("GPRS通道");
		m_wndCmbCmType.SetItemData(idx,1);

		//idx = m_wndCmbCmType.AddString("GSM手机短信通道");
		//m_wndCmbCmType.SetItemData(idx,2);

		idx = m_wndCmbCmType.AddString("卫星通道");
		m_wndCmbCmType.SetItemData(idx,3);

		//idx = m_wndCmbCmType.AddString("超短波通道");
		//m_wndCmbCmType.SetItemData(idx,4);

		//idx = m_wndCmbCmType.AddString("微波通道");
		//m_wndCmbCmType.SetItemData(idx,5);

		idx = m_wndCmbCmType.AddString("其他");
		m_wndCmbCmType.SetItemData(idx,6);


		if (m_wndCmbCmType.GetCount())
		{
			for (int i=0; i< m_wndCmbCmType.GetCount(); i++)
			{
				if (m_wndCmbCmType.GetItemData(i) == m_CmType)
				{
					m_wndCmbCmType.SetCurSel(i);
					break;
				}
			}
		}
	}

	 if (m_wndCmbInterface.GetSafeHwnd())
	 {
		 m_wndCmbInterface.ResetContent();

		 int idx = 0;
		 idx = m_wndCmbInterface.AddString("请选择");
		 m_wndCmbInterface.SetItemData(idx,0);

		 idx = m_wndCmbInterface.AddString("以太网通信接口");
		 m_wndCmbInterface.SetItemData(idx,1);

		 idx = m_wndCmbInterface.AddString("串行通信接口");
		 m_wndCmbInterface.SetItemData(idx,2);

		 if (m_wndCmbInterface.GetCount())
		 {
			 for ( int i = 0; i < m_wndCmbInterface.GetCount(); i++)
			 {
				 if (m_wndCmbInterface.GetItemData(i) == m_Interface)
				 {
					 m_wndCmbInterface.SetCurSel(i);
					 break;
				 }
			 }
		 }
	 }

	 OnCbnSelchangeCmbCmtype();
	 OnCbnSelchangeCmbInterface();
	 OnCbnSelchangeAppProType();

	 if (!m_strCaption.IsEmpty())
		 SetWindowText(m_strCaption);

	 if (m_OperateFlag != RECORD_NEW)
		  m_wndBtnInterfaceCfg.EnableWindow(TRUE);

	 if(m_wndLetSCSW008.GetSafeHwnd())
	 {
		 if (!m_strStCode.IsEmpty())
		 {
			 m_wndLetSCSW008.SetWindowText(m_strStCode);
		 }
	 }
	
	 if(m_wndLetSYQAddr.GetSafeHwnd())
	 {
	//	 if (!m_strSYQADDR.IsEmpty())
		 {
			// m_wndLetSYQAddr.SetWindowText(m_strSYQADDR);
		 }
	 }

	 if(m_wndLetSYQCenter.GetSafeHwnd())
	 {
		// if (!m_syqcenter.IsEmpty())
		 {
//			 m_wndLetSYQCenter.SetWindowText(m_syqcenter);
		 }
	 }
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CUdfAddRtuStDlg::OnOK()
{
//	UpdateData(TRUE);
	CWnd* pWnd = GetDlgItem(IDC_EDI_NAME);
	if (pWnd)
		pWnd->GetWindowText(m_sName);

	pWnd = GetDlgItem(IDC_EDI_A2);
	CString strtemp ="";
	if(pWnd != NULL)
		pWnd->GetWindowText(strtemp);
	m_dwA2 = (DWORD)atol(strtemp);

	if (m_wndCmbAppProType.GetCurSel() == CB_ERR)
	{
		MessageBox("应用层协议类型无效","错误",MB_OK|MB_ICONERROR);
		return;
	}

	m_sName.Trim();
	if (m_sName.IsEmpty())
	{
		MessageBox("名称不能为空,请重新输入","错误",MB_OK|MB_ICONERROR);
		return;
	}

	if (m_wndCmbProvince.GetCurSel() != CB_ERR)
	{
		CUdfProvinceCode* pProvincode =(CUdfProvinceCode*)m_wndCmbProvince.GetItemDataPtr(m_wndCmbProvince.GetCurSel());
		if (pProvincode)
		{
			m_ProvinceCode = pProvincode->GetZoningCode();
		}
	}

	if (m_wndCmbRegion.GetCurSel() != CB_ERR)
	{
		CUdfRegionCode* pRegioncode = (CUdfRegionCode*)m_wndCmbRegion.GetItemDataPtr(m_wndCmbRegion.GetCurSel());
		if (pRegioncode)
		{
			m_RegionCode = pRegioncode->GetZoningCode();
		}
	}

	if (m_wndCmbCounty.GetCurSel() != CB_ERR)
	{
		CUdfCountyCode* pCountyCode = (CUdfCountyCode*)m_wndCmbCounty.GetItemDataPtr(m_wndCmbCounty.GetCurSel());
		if (pCountyCode)
		{
			m_CountyCode = pCountyCode->GetZoningCode();
		}
	}

	if (m_wndCmbA3.GetCurSel() != CB_ERR)
	{
		m_A3Code = (BYTE)m_wndCmbA3.GetItemData(m_wndCmbA3.GetCurSel());
	}

	if (m_wndCmbCmType.GetCurSel() != CB_ERR)
	{
		m_CmType = (BYTE)m_wndCmbCmType.GetItemData(m_wndCmbCmType.GetCurSel());
	}

	if (m_wndCmbInterface.GetCurSel() != CB_ERR)
	{
		m_Interface = (BYTE)m_wndCmbInterface.GetItemData(m_wndCmbInterface.GetCurSel());
	}

	if (m_wndCmbAppProVer.GetCurSel() != CB_ERR)
	{
		m_nAppProVer = (BYTE)m_wndCmbAppProVer.GetItemData(m_wndCmbAppProVer.GetCurSel());
	}

	m_nAppProtoType = (BYTE)m_wndCmbAppProType.GetItemData(m_wndCmbAppProType.GetCurSel());
	if (m_nAppProtoType == (BYTE)WaterResources_Protocol)
	{
		if(m_dwA2 <= 0 || m_dwA2 > 2097150)

		{
			MessageBox("终端、终端定值范围(1～2097150(1FFFFEH)),\r\n请重新输入","错误",MB_OK|MB_ICONERROR);
			return;
		}

		CCMSApp* pApp = (CCMSApp*)AfxGetApp();
		if (m_OperateFlag == RECORD_NEW  && pApp->m_pReadDB->CheckRtuStExist(m_ProvinceCode,m_RegionCode,m_dwA2))
		{
			MessageBox("与在同一地域内的终端地址、中继地址有重复,请重新输入!","",MB_OK|MB_ICONERROR);
			return;
		}
		else if (m_OperateFlag != RECORD_NEW )
		{
			if (pApp->m_pReadDB->CheckRtuStExist(m_ProvinceCode,m_RegionCode,m_dwA2,m_pCurRtuStation))
			{
				MessageBox("与在同一地域内的终端地址、中继地址有重复,请重新输入!","",MB_OK|MB_ICONERROR);
				return;
			}
		}
	}
	else if (m_nAppProtoType == (BYTE)SCSW_008_Protocol)
	{
		CString strStCode = "";
		m_wndLetSCSW008.GetWindowText(strStCode);
		int nLen = strStCode.GetLength();

		if (nLen != 10)
		{
			MessageBox("《SCSW008-2001》协议规定站地址必须为10位字符","",MB_OK|MB_ICONERROR);
			return;
		}

		m_strStCode = strStCode;
		CCMSApp* pApp = (CCMSApp*)AfxGetApp();
		if (m_OperateFlag == RECORD_NEW && pApp->m_pReadDB->CheckRtuStExist(strStCode))
		{
			MessageBox("与在同一地域内的SCSW008地址码有重复, 请重新输入!","",MB_OK|MB_ICONERROR);
			return;
		}
		else if (m_OperateFlag != RECORD_NEW)
		{
			if (pApp->m_pReadDB->CheckRtuStExist(strStCode, m_pCurRtuStation))
			{
				MessageBox("与在同一地域内的SCSW008地址码有重复, 请重新输入!","",MB_OK|MB_ICONERROR);
				return;
			}
		}

	}
	else if(m_nAppProtoType == (BYTE)SYQYC_Comm_Protocol)//SYQYC_Comm_Protocol
	{
		if (m_syqcenter <= 0)
		{
			MessageBox("《水雨情遥测系统数据通信规约》协议规定中心站地址范围(1～65535(FFFFH)),\r\n请重新输入","错误",MB_OK|MB_ICONERROR);
			return;
		}
		
		CCMSApp* pApp = (CCMSApp*)AfxGetApp();
		if (/*m_OperateFlag == RECORD_NEW  &&*/ pApp->m_pReadDB->CheckCenterStNo(m_syqcenter, m_pCurRtuStation))
		{
			MessageBox("与在同一地域内的中心站地址不同,请重新输入!","",MB_OK|MB_ICONERROR);
			return;
		}
	/*	else if (m_OperateFlag != RECORD_NEW )
		{
			if (pApp->m_pReadDB->CheckRtuStExist(m_ProvinceCode,m_RegionCode,m_dwA2,m_pCurRtuStation))
			{
				MessageBox("与在同一地域内的遥测站地址有重复,请重新输入!","",MB_OK|MB_ICONERROR);
				return;
			}
		}*/


		if (m_strSYQADDR < 0)
		{
			MessageBox("《水雨情遥测系统数据通信规约》协议规定站地址必地址范围(0～9999999999)","",MB_OK|MB_ICONERROR);
			return;
		}
		
		if (m_OperateFlag == RECORD_NEW && pApp->m_pReadDB->CheckSYQADDExist(m_strSYQADDR))
		{
			MessageBox("与在同一地域内的SCSW008地址码有重复, 请重新输入!","",MB_OK|MB_ICONERROR);
			return;
		}
		else if (m_OperateFlag != RECORD_NEW)
		{
			if (pApp->m_pReadDB->CheckSYQADDExist(m_strSYQADDR, m_pCurRtuStation))
			{
				MessageBox("与在同一地域内的SCSW008地址码有重复, 请重新输入!","",MB_OK|MB_ICONERROR);
				return;
			}
		}

		

	}
	 
	//对输入的数据信息进行检查
	if (m_Interface == 1)        //Net
	{
		//修改于 2012-04-24 
		/*
		if (!CUdfGlobalFUNC::IpAddressCheck(m_NetInfo.mIpAddress))
		{
			MessageBox("IP地址不正确或为空,请重新输入","错误",MB_OK|MB_ICONERROR);
			return;
		}
		
		if (m_NetInfo.mPort <= 0 || m_NetInfo.mPort > 65535)
		{
			MessageBox("端口不正确(1~65535),请重新输入","错误",MB_OK|MB_ICONERROR);
			return;
		}
		*/
	}
	else if (m_Interface == 2)  //Com
	{
		//修改于 2012-04-24 
		/*
		if (m_ComInfo.mComName.IsEmpty())
		{
			MessageBox("串口号不能为空,请重新输入","错误",MB_OK|MB_ICONERROR);
			return;
		}
		if (m_ComInfo.mBaund <= 0 )
		{
			MessageBox("波特率输入错误,请重新输入","错误",MB_OK|MB_ICONERROR);
			return;
		}
		*/
	}   	

	CDialog::OnOK();
}

void CUdfAddRtuStDlg::OnBnClickedBtnInterfacecfg()
{
	/*
	if (m_wndCmbInterface.GetSafeHwnd() &&
		m_wndCmbInterface.GetCurSel() != CB_ERR)
	{
		int nSel = m_wndCmbInterface.GetCurSel();
		DWORD_PTR nItemDate = m_wndCmbInterface.GetItemData(nSel);

		if (nItemDate == 2)
		{
			m_wndBtnInterfaceCfg.EnableWindow(TRUE);
			CUdfComInfoDlg dlg(this);
			dlg.m_strComNo = m_ComInfo.mComName;
			dlg.m_nBaund   = m_ComInfo.mBaund;
			dlg.m_nDataBits = m_ComInfo.mDataBits;
			dlg.m_nStopBits = m_ComInfo.mStopBits;
			dlg.m_nParity   = m_ComInfo.mParity;

			if (dlg.DoModal() == IDOK)
			{
				m_ComInfo.mComName = dlg.m_strComNo ;
				m_ComInfo.mBaund = dlg.m_nBaund;
				m_ComInfo.mDataBits = dlg.m_nDataBits;
				m_ComInfo.mStopBits = dlg.m_nStopBits;
				m_ComInfo.mParity = dlg.m_nParity;
			}
		}
		else if (nItemDate == 1)
		{
			m_wndBtnInterfaceCfg.EnableWindow(TRUE);
			CUdfNetInfoDlg dlg(this);
			dlg.m_strIPAddr = m_NetInfo.mIpAddress;
			dlg.m_nPort     = m_NetInfo.mPort;

			if (dlg.DoModal() == IDOK)
			{
				m_NetInfo.mPort = dlg.m_nPort;
				m_NetInfo.mIpAddress = dlg.m_strIPAddr;
			}
		}
		else if (nItemDate == 0)
		{
			m_wndBtnInterfaceCfg.EnableWindow(FALSE);
		}
	}
	*/
}

void CUdfAddRtuStDlg::OnCbnSelchangeCmbInterface()
{
	InitGridCtrl();
}

void CUdfAddRtuStDlg::InitGridCtrl()
{
	if (m_wndCmbInterface.GetSafeHwnd() &&
		m_wndCmbInterface.GetCurSel() != CB_ERR)
	{
		int nSel = m_wndCmbInterface.GetCurSel();
		DWORD_PTR nItemDate = m_wndCmbInterface.GetItemData(nSel);

		if (nItemDate == Serial_CommInterface)
		{
			InitGridCtrl_ComInfo();
		}
		else if (nItemDate == Ethernet_CommInerface)
		{
			InitGridCtrl_NetInfo();
		}
		else
		{
			if (m_wndCmOptGridCtrl.GetSafeHwnd())
			{
				m_wndCmOptGridCtrl.DeleteAllItems();
				m_wndCmOptGridCtrl.Invalidate();
			}
		}
	}
}

void CUdfAddRtuStDlg::Update(CRtuStation* pStaion)
{
	if (pStaion)
	{
		 m_dwA2 = pStaion->GetAddrA2();
		 m_sName = pStaion->GetName();
		 m_ProvinceCode = pStaion->GetProvinceCode(); 
		 m_RegionCode = pStaion->GetRegionCode();
		 m_CountyCode = pStaion->GetCountyCode();
		 m_A3Code = pStaion->GetAddrA3();
		 m_CmType = pStaion->GetCmType();
		 m_Interface = pStaion->GetInterface();
		 m_ComInfo.mBaund  = pStaion->GetBaundA();    //串口信息
		 m_ComInfo.mComName =  pStaion->GetComPortA();
		 m_ComInfo.mDataBits = pStaion->GetDataBitsA();
		 m_ComInfo.mParity  = pStaion->GetParityA();
		 m_ComInfo.mStopBits  = pStaion->GetStopBitsA();
		 m_NetInfo.mPort =  pStaion->GetSockPortA();    //网络信息
		 m_NetInfo.mIpAddress = pStaion->GetIpAddrA();

		 //ADD 2012-04-24
		 m_nScAddress = pStaion->GetScAddress();
		 m_nScFactoryType = pStaion->GetScFactoryType();
		 m_nAppProVer = pStaion->GetAppProVer();
		 m_nAppProtoType = pStaion->GetAppProtocolType();
		 m_strStCode = pStaion->GetStringStCode();
		 m_syqcenter = pStaion->GetSYQCenterCode();
		 m_strSYQADDR = pStaion->GetSYQStCode();


		 m_OperateFlag = RECORD_EDIT; 
		 m_pCurRtuStation = pStaion;
		 m_strCaption.Format("%s [基本信息]",pStaion->GetName()); 
	}
}

void CUdfAddRtuStDlg::InitGridCtrl_NetInfo()
{
	if (m_wndCmOptGridCtrl.GetSafeHwnd())
	{
		int nCmTypeSel = m_wndCmbCmType.GetCurSel();
		BYTE nCmType = (BYTE)m_wndCmbCmType.GetItemData(nCmTypeSel);

		int nCount = 3;
		if (nCmType == SC_CommChannel)
			nCount = 4;

		m_wndCmOptGridCtrl.DeleteAllItems();
		m_wndCmOptGridCtrl.SetRowCount(nCount);
		m_wndCmOptGridCtrl.SetFixedRowCount(1);

		CString strHearders[]  = {"IP地址","端口号","卫星通信地址"};
		CString strColHeaders[] ={"属性项","值"};

		m_wndCmOptGridCtrl.InsertColumn(strColHeaders[0],DT_LEFT|DT_VCENTER|DT_SINGLELINE,0);
		m_wndCmOptGridCtrl.SetColumnWidth(0,90);
		m_wndCmOptGridCtrl.InsertColumn(strColHeaders[1],DT_LEFT|DT_VCENTER|DT_SINGLELINE,1);
		m_wndCmOptGridCtrl.SetColumnWidth(1,210);

		for (int Idx = 1; Idx < nCount; Idx ++)
		{
			m_wndCmOptGridCtrl.SetItemText(Idx,0,strHearders[Idx-1]);
			OnCellReadOnly(Idx,0);
			OnCellNormal(Idx,1);
			m_wndCmOptGridCtrl.SetRowHeight(Idx,MFC_GRIDCTRL_HEIGHT);
		}

		OnCellNormal(1,1);
		m_wndCmOptGridCtrl.SetItemText(1,1, m_NetInfo.mIpAddress);

		OnCellNormal(2,1);
		if (!m_wndCmOptGridCtrl.SetCellType(2,1, RUNTIME_CLASS(CGridCellNumeric)))
			return;

		CString _sTemp = "0";
		_sTemp.Format("%d",m_NetInfo.mPort);
		m_wndCmOptGridCtrl.SetItemText(2,1, _sTemp);

		if (nCmType == SC_CommChannel)
		{
			OnCellNormal(3,1);
			if (!m_wndCmOptGridCtrl.SetCellType(3,1, RUNTIME_CLASS(CGridCellNumeric)))
				return;

			CString _sTemp = "0";
			_sTemp.Format("%d",m_nScAddress);
			m_wndCmOptGridCtrl.SetItemText(3,1, _sTemp);
		}
	}
}



void CUdfAddRtuStDlg::InitGridCtrl_ComInfo()
{
	if (m_wndCmOptGridCtrl.GetSafeHwnd())
	{

		int nCmTypeSel = m_wndCmbCmType.GetCurSel();
		BYTE nCmType = (BYTE)m_wndCmbCmType.GetItemData(nCmTypeSel);
		int nCount = 6;
		if (nCmType == SC_CommChannel)
			nCount = 7;

		m_wndCmOptGridCtrl.DeleteAllItems();
		m_wndCmOptGridCtrl.SetRowCount(nCount);
		m_wndCmOptGridCtrl.SetFixedRowCount(1);

		CString strHearders[] = {"串口号",
			"波特率",
			"数据位",
			"停止位",
			"校验",
			"卫星通信地址"
		};

		CString strColHeaders[] ={"属性项","值"};

		m_wndCmOptGridCtrl.InsertColumn(strColHeaders[0],DT_LEFT|DT_VCENTER|DT_SINGLELINE,0);
		m_wndCmOptGridCtrl.SetColumnWidth(0,90);
		m_wndCmOptGridCtrl.InsertColumn(strColHeaders[1],DT_LEFT|DT_VCENTER|DT_SINGLELINE,1);
		m_wndCmOptGridCtrl.SetColumnWidth(1,210);

		for (int Idx = 1; Idx < nCount; Idx ++)
		{
			m_wndCmOptGridCtrl.SetItemText(Idx,0,strHearders[Idx-1]);
			OnCellReadOnly(Idx,0);
			OnCellNormal(Idx,1);
			m_wndCmOptGridCtrl.SetRowHeight(Idx,MFC_GRIDCTRL_HEIGHT);
		}

		InitComGridItem11();
		InitComGridItem21();
		InitComGridItem31();
		InitComGridItem41();
		InitComGridItem51();

		if(nCmType == SC_CommChannel)
			InitComGridItem61();
	}
}



void  CUdfAddRtuStDlg::InitComGridItem11()
{
	OnCellNormal(1,1);
	if (!m_wndCmOptGridCtrl.SetCellType(1,1, RUNTIME_CLASS(CGridCellCombo)))
		return;

	CStringArray options;
	options.Add(_T("COM1"));
	options.Add(_T("COM2"));
	options.Add(_T("COM3"));

	CGridCellCombo *pCell = (CGridCellCombo*) m_wndCmOptGridCtrl.GetCell(1,1);
	pCell->SetOptions(options);
	pCell->SetStyle(CBS_DROPDOWN); //CBS_DROPDOWN, CBS_DROPDOWNLIST, CBS_SIMPLE

	CString strCom = "COM1";
	strCom = m_ComInfo.mComName;

	m_wndCmOptGridCtrl.SetItemText(1,1, strCom);
}

void  CUdfAddRtuStDlg::InitComGridItem21()
{
	OnCellNormal(2,1);
	if (!m_wndCmOptGridCtrl.SetCellType(2,1, RUNTIME_CLASS(CGridCellCombo)))
		return;
	CStringArray options;

	options.Add("110");
	options.Add("300");
	options.Add("600");
	options.Add("1200");
	options.Add("2400");
	options.Add("4800");
	options.Add("9600");
	options.Add("14400");
	options.Add("19200");
	options.Add("38400");
	options.Add("56000");
	options.Add("57600");
	options.Add("115200");
	options.Add("128000");
	options.Add("256000");	

	CGridCellCombo *pCell = (CGridCellCombo*) m_wndCmOptGridCtrl.GetCell(2,1);
	pCell->SetOptions(options);
	pCell->SetStyle(CBS_DROPDOWN); //CBS_DROPDOWN, CBS_DROPDOWNLIST, CBS_SIMPLE

	CString _strBound = "9600";
	_strBound.Format("%d",m_ComInfo.mBaund);

	m_wndCmOptGridCtrl.SetItemText(2,1, _strBound);
}

void  CUdfAddRtuStDlg::InitComGridItem31()
{
	OnCellNormal(3,1);
	if (!m_wndCmOptGridCtrl.SetCellType(3,1, RUNTIME_CLASS(CGridCellCombo)))
		return;

	CStringArray options;
	options.Add(_T("8位"));
	options.Add(_T("7位"));

	CGridCellCombo *pCell = (CGridCellCombo*) m_wndCmOptGridCtrl.GetCell(3,1);
	pCell->SetOptions(options);
	pCell->SetStyle(CBS_DROPDOWNLIST); //CBS_DROPDOWN, CBS_DROPDOWNLIST, CBS_SIMPLE

	CString _strDataBits= "8位";
	BYTE databits = m_ComInfo.mDataBits;
	if (databits == 8)
		_strDataBits= "8位";
	else if (databits == 7)
		_strDataBits= "7位";
	m_wndCmOptGridCtrl.SetItemText(3,1, _strDataBits);
}

void  CUdfAddRtuStDlg::InitComGridItem41()
{
	OnCellNormal(4,1);
	if (!m_wndCmOptGridCtrl.SetCellType(4,1, RUNTIME_CLASS(CGridCellCombo)))
		return;

	CStringArray options;
	options.Add(_T("1位停止位"));
	options.Add(_T("1.5位停止位"));
	options.Add(_T("2位停止位"));

	CGridCellCombo *pCell = (CGridCellCombo*) m_wndCmOptGridCtrl.GetCell(4,1);
	pCell->SetOptions(options);
	pCell->SetStyle(CBS_DROPDOWNLIST); //CBS_DROPDOWN, CBS_DROPDOWNLIST, CBS_SIMPLE



	BYTE nStopBits = m_ComInfo.mStopBits;
	CString _sParity = "1位停止位";

	if (nStopBits == ONESTOPBIT)
		_sParity = "1位停止位";
	else if (ONE5STOPBITS == nStopBits)
		_sParity = "1.5位停止位";
	else if (TWOSTOPBITS == nStopBits)
		_sParity = "2位停止位";

	m_wndCmOptGridCtrl.SetItemText(4,1,_sParity);
}

void  CUdfAddRtuStDlg::InitComGridItem51()
{
	OnCellNormal(5,1);
	if (!m_wndCmOptGridCtrl.SetCellType(5,1, RUNTIME_CLASS(CGridCellCombo)))
		return;

	CStringArray options;
	options.Add(_T("无校验"));
	options.Add(_T("奇校验"));
	options.Add(_T("偶校验"));

	CGridCellCombo *pCell = (CGridCellCombo*) m_wndCmOptGridCtrl.GetCell(5,1);
	pCell->SetOptions(options);
	pCell->SetStyle(CBS_DROPDOWNLIST); //CBS_DROPDOWN, CBS_DROPDOWNLIST, CBS_SIMPLE


	BYTE nParity = m_ComInfo.mParity;
	CString _sParity = "无校验";

	if (nParity == NOPARITY)
		_sParity = "无校验";
	else if (ODDPARITY == nParity)
		_sParity = "奇校验";
	else if (EVENPARITY == nParity)
		_sParity = "偶校验";

	m_wndCmOptGridCtrl.SetItemText(5,1,_sParity);

}

void CUdfAddRtuStDlg::InitComGridItem61()
{
	OnCellNormal(6,1);
	if (!m_wndCmOptGridCtrl.SetCellType(6,1, RUNTIME_CLASS(CGridCellNumeric)))
		return;

	CString _sTemp = "0";
	_sTemp.Format("%d",m_nScAddress);
	m_wndCmOptGridCtrl.SetItemText(6,1, _sTemp);
}


void CUdfAddRtuStDlg::OnCellNormal(int Row,int Col)
{
	if (m_wndCmOptGridCtrl.GetSafeHwnd())
	{
		m_wndCmOptGridCtrl.SetCellType(Row,Col, RUNTIME_CLASS(CGridCell));
		m_wndCmOptGridCtrl.SetItemText(Row,Col, _T(""));
		m_wndCmOptGridCtrl.SetItemState(Row,Col, m_wndCmOptGridCtrl.GetItemState(Row,Col) & ~GVIS_READONLY);
		m_wndCmOptGridCtrl.Invalidate();
	}
}

void CUdfAddRtuStDlg::OnCellReadOnly(int Row,int Col)
{
	if (m_wndCmOptGridCtrl.GetSafeHwnd())
	{
		m_wndCmOptGridCtrl.SetItemState(Row,Col, m_wndCmOptGridCtrl.GetItemState(Row,Col) | GVIS_READONLY);
	}
}


void CUdfAddRtuStDlg::OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	// AcceptChange is a fictional routine that should return TRUE
	// if you want to accept the new value for the cell.

	if (pItem == NULL) return;
	if (pItem->iColumn != 1) return;
	if (pItem->iRow <=0  )   return;

	int nSel_Interface = m_wndCmbInterface.GetCurSel();
	if (nSel_Interface == CB_ERR)		return;


	int nSel_CmType = m_wndCmbCmType.GetCurSel();
	BYTE nCmType = 0;
	if (nSel_CmType != CB_ERR)
		nCmType = (BYTE)m_wndCmbCmType.GetItemData(nSel_CmType);
	
	//int nGirdRowCount = 0;
	INT_PTR nItemData = m_wndCmbInterface.GetItemData(nSel_Interface);
	if (nItemData == Ethernet_CommInerface)          //Net
	{
		//nGirdRowCount  = 3;
		if (pItem->iRow == 1)	
			m_NetInfo.mIpAddress= GetCellText(1,1);
		if (pItem->iRow == 2)   
		{
			if (atoi(GetCellText(2,1)) >  65535 ||
				atoi(GetCellText(2,1)) < 0 )
			{
				MessageBox("端口号超出范围(1~65535),请重新输入!","提 <0示",MB_OK|MB_ICONERROR);
				m_wndCmOptGridCtrl.SetItemText(2,1,"0");
				m_NetInfo.mPort = 0;
				return ;
			}
			else
				m_NetInfo.mPort	  = (WORD)atoi(GetCellText(2,1));
		}

		if (nCmType == SC_CommChannel)
		{
			m_nScAddress = (UINT32)atol(GetCellText(3,1)); 
		}

	}
	else if (nItemData == Serial_CommInterface)     //Com
	{
		//nGirdRowCount = 6;

		CString _sTemp = "";
		if (pItem->iRow == 1)	m_ComInfo.mComName = GetCellText(1,1);
		if (pItem->iRow == 2)	m_ComInfo.mBaund   = atoi(GetCellText(2,1));
        if (pItem->iRow == 3)
		{
			_sTemp = GetCellText(3,1);
			if (_sTemp.Find("8位") != -1)
				m_ComInfo.mDataBits  = 8;
			else if (_sTemp.Find("7位") != -1)
				m_ComInfo.mDataBits = 7;
			else 
				m_ComInfo.mDataBits = 8;
		}

        if (pItem->iRow == 4)
		{
			_sTemp = GetCellText(4,1);
			if (_sTemp.Find("1位停止位") != -1)
				m_ComInfo.mStopBits = ONESTOPBIT;
			else if (_sTemp.Find("1.5位停止位")!= -1)
				m_ComInfo.mStopBits = ONE5STOPBITS;
			else if (_sTemp.Find("2位停止位")!= -1)
				m_ComInfo.mStopBits = TWOSTOPBITS;
			else
				m_ComInfo.mStopBits = ONESTOPBIT;
		}

		if (pItem->iRow == 5)
		{
			_sTemp = GetCellText(5,1);
			if (_sTemp.Find("无校验") != -1)
				m_ComInfo.mParity = NOPARITY;
			else if (_sTemp.Find("偶校验") != -1)
				m_ComInfo.mParity = EVENPARITY;
			else if (_sTemp.Find("奇校验") != -1)
				m_ComInfo.mParity = ODDPARITY;
			else
				m_ComInfo.mParity = NOPARITY;
		}

		if (nCmType == SC_CommChannel)
		{
			m_nScAddress = (UINT32)atol(GetCellText(6,1));
		}
	}
}


const CString CUdfAddRtuStDlg::GetCellText(int Row,int Col)
{
	CString _sTemp = "";
	if (m_wndCmOptGridCtrl.GetSafeHwnd())
	{
		CGridCellBase* pCell = m_wndCmOptGridCtrl.GetCell(Row,Col);
		if (pCell)
		{
			_sTemp = pCell->GetText();
			_sTemp.Trim();
			return _sTemp;
		}
	}
	return _sTemp;
}

void CUdfAddRtuStDlg::OnCbnSelchangeCmbCmtype()
{
	int nSel = m_wndCmbCmType.GetCurSel();
	if (nSel != CB_ERR)
	{
		BYTE cmtype = (BYTE)m_wndCmbCmType.GetItemData(nSel);

		if (cmtype == SC_CommChannel)
		{
			m_wndStaticScFactory.ShowWindow(SW_SHOW);
			m_wndCmbScFactory.ShowWindow(SW_SHOW);
		}
		else 
		{
			m_wndStaticScFactory.ShowWindow(SW_HIDE);
			m_wndCmbScFactory.ShowWindow(SW_HIDE);
		}
	}

	InitGridCtrl();
}

void CUdfAddRtuStDlg::OnCbnSelchangeAppProType()
{
	int nSel = m_wndCmbAppProType.GetCurSel();
	if (nSel != CB_ERR)
	{
		BYTE cmtype = (BYTE)m_wndCmbAppProType.GetItemData(nSel);

		if (cmtype == (BYTE)WaterResources_Protocol)
		{
			m_wndStaticSCSW008.ShowWindow(SW_HIDE);
			m_wndLetSCSW008.ShowWindow(SW_HIDE);

			m_wndStaticSYQADDR.ShowWindow(SW_HIDE);
			m_wndLetSYQAddr.ShowWindow(SW_HIDE);
			m_wndStaticSYQCENTER.ShowWindow(SW_HIDE);
			m_wndLetSYQCenter.ShowWindow(SW_HIDE);

			m_wndLetA2.EnableWindow(true);
			
		}
		else if (cmtype == (BYTE)SCSW_008_Protocol)
		{
			m_wndStaticSCSW008.ShowWindow(SW_SHOW);
			m_wndLetSCSW008.ShowWindow(SW_SHOW);
			
			m_wndStaticSYQADDR.ShowWindow(SW_HIDE);
			m_wndLetSYQAddr.ShowWindow(SW_HIDE);
			m_wndStaticSYQCENTER.ShowWindow(SW_HIDE);
			m_wndLetSYQCenter.ShowWindow(SW_HIDE);
			m_wndLetA2.EnableWindow(false);
			
		}
		if (cmtype == (BYTE)SYQYC_Comm_Protocol)
		{
			m_wndStaticSCSW008.ShowWindow(SW_HIDE);
			m_wndLetSCSW008.ShowWindow(SW_HIDE);

			m_wndStaticSYQADDR.ShowWindow(SW_SHOW);
			m_wndLetSYQAddr.ShowWindow(SW_SHOW);
			m_wndStaticSYQCENTER.ShowWindow(SW_SHOW);
			m_wndLetSYQCenter.ShowWindow(SW_SHOW);
			m_wndLetA2.EnableWindow(false);

		}
	}
}


void CUdfAddRtuStDlg::OnEnChangeEditAddvcode()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void CUdfAddRtuStDlg::ShowAddvCode()
{
	if (m_wndCmbProvince.GetCurSel() != CB_ERR)
	{
		CUdfProvinceCode* pProvincode =(CUdfProvinceCode*)m_wndCmbProvince.GetItemDataPtr(m_wndCmbProvince.GetCurSel());
		if (pProvincode)
		{
			m_ProvinceCode = pProvincode->GetZoningCode();
		}
	}

	if (m_wndCmbRegion.GetCurSel() != CB_ERR)
	{
		CUdfRegionCode* pRegioncode = (CUdfRegionCode*)m_wndCmbRegion.GetItemDataPtr(m_wndCmbRegion.GetCurSel());
		if (pRegioncode)
		{
			m_RegionCode = pRegioncode->GetZoningCode();
		}
	}

	if (m_wndCmbCounty.GetCurSel() != CB_ERR)
	{
		CUdfCountyCode* pCountyCode = (CUdfCountyCode*)m_wndCmbCounty.GetItemDataPtr(m_wndCmbCounty.GetCurSel());
		if (pCountyCode)
		{
			m_CountyCode = pCountyCode->GetZoningCode();
		}
	}

	CString m_strAddvCode = "";
	m_strAddvCode.Format("%02d%02d%02d",m_ProvinceCode,m_RegionCode,m_CountyCode);
	if (m_wndEditAddvCode.GetSafeHwnd())
		m_wndEditAddvCode.SetWindowText(m_strAddvCode);
}

void CUdfAddRtuStDlg::OnCbnSelchangeCmbCounty()
{
	ShowAddvCode();
}


