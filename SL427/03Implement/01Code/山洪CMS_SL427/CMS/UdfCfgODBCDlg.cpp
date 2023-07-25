// UdfCfgODBCDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "UdfCfgODBCDlg.h"


// CUdfCfgODBCDlg 对话框
IMPLEMENT_DYNAMIC(CUdfCfgODBCDlg, CDialog)

CUdfCfgODBCDlg::CUdfCfgODBCDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUdfCfgODBCDlg::IDD, pParent)
	, m_strDSN(_T(""))
	, m_strUID(_T(""))
	, m_strPWD(_T(""))
{
	m_nDataInterface = 0;
	m_nDataIType = 0;
	m_nDatabase = 1;
}

CUdfCfgODBCDlg::~CUdfCfgODBCDlg()
{
}

void CUdfCfgODBCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DSN, m_strDSN);
	DDX_Text(pDX, IDC_EDIT_UID, m_strUID);
	DDX_Text(pDX, IDC_EDIT_PWD, m_strPWD);
	DDX_Control(pDX, IDOK, m_BtnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_CMB_DATAINTERFACE, m_wndCbmDataInterface);
	DDX_Control(pDX, IDC_CMB_ITYPE, m_wndComboBoxIType);
	DDX_Control(pDX, IDC_STATIC_ITYPE, m_wndStaticIType);
	DDX_Control(pDX, IDC_STATIC_TNAME, m_wndStaticIName);
	DDX_Control(pDX, IDC_STATIC_DATABASE, m_wndStaticDatabase);
	DDX_Control(pDX, IDC_CMB_DATABASE, m_wndCmbDatabase);
}


BEGIN_MESSAGE_MAP(CUdfCfgODBCDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CUdfCfgODBCDlg::OnBnClickedOk)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_CMB_DATAINTERFACE, &CUdfCfgODBCDlg::OnCbnSelchangeCmbDatainterface)
	ON_CBN_SELCHANGE(IDC_CMB_ITYPE, &CUdfCfgODBCDlg::OnCbnSelchangeCmbItype)
END_MESSAGE_MAP()


// CUdfCfgODBCDlg 消息处理程序
void CUdfCfgODBCDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	m_strDSN.Trim();
	if (m_strDSN.IsEmpty())
	{
		MessageBox("DSN数据源名不能为空,请输入","提示",MB_OK);
		return;
	}

	m_strUID.Trim();
	if (m_strUID.IsEmpty())
	{
		MessageBox("用户名不能为空,请输入","提示",MB_OK);
		return;

	}

	int nSel = m_wndCbmDataInterface.GetCurSel();
	if (nSel != CB_ERR)
	{
		m_nDataInterface = (BYTE)m_wndCbmDataInterface.GetItemData(nSel);
	}

	nSel = m_wndComboBoxIType.GetCurSel();
	if (nSel != CB_ERR)
	{
		m_nDataIType = (BYTE)m_wndComboBoxIType.GetItemData(nSel);
	}

	nSel = m_wndCmbDatabase.GetCurSel();
	if (nSel != CB_ERR)
	{
		m_nDatabase = (BYTE)m_wndCmbDatabase.GetItemData(nSel);
	}

	SaveOdbcOptions();
	OnOK();
}

BOOL CUdfCfgODBCDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CenterWindow();
	UpdateData(FALSE);

	if (m_wndCbmDataInterface.GetSafeHwnd())
	{
		m_wndCbmDataInterface.ResetContent();

		int Index  = m_wndCbmDataInterface.AddString("光芒公司数据接口");
		m_wndCbmDataInterface.SetItemData(Index,1);
		
		Index  = m_wndCbmDataInterface.AddString("四川省山洪数据接口");
		m_wndCbmDataInterface.SetItemData(Index,2);
	}

	if (m_wndComboBoxIType.GetSafeHwnd())
	{
		m_wndComboBoxIType.ResetContent();
		
		int Index = m_wndComboBoxIType.AddString("直接访问数据库方式");
		m_wndComboBoxIType.SetItemData(Index,1);

		Index = m_wndComboBoxIType.AddString("通过WebService接口方式");
		m_wndComboBoxIType.SetItemData(Index,2);
	}


	if (m_wndCmbDatabase.GetSafeHwnd())
	{
		m_wndCmbDatabase.ResetContent();

		int Index = m_wndCmbDatabase.AddString("SQL Server 数据库");
		m_wndCmbDatabase.SetItemData(Index,1);

		Index = m_wndCmbDatabase.AddString("Oracle 数据库");
		m_wndCmbDatabase.SetItemData(Index,2);
	}


	SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	return TRUE;  
	// return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CUdfCfgODBCDlg::OnOK()
{
	ShowWindow(SW_HIDE);
}


void CUdfCfgODBCDlg::Show(int nCmdShow)
{
	CCMSApp* pApp = (CCMSApp*) AfxGetApp();
	m_strDSN = pApp->m_GlobalOptions.m_DbOpts.m_strMISDSN;
	m_strUID = pApp->m_GlobalOptions.m_DbOpts.m_strMISUID;
	m_strPWD = pApp->m_GlobalOptions.m_DbOpts.m_strMISPWD;
	UpdateData(FALSE);

	if (m_wndCbmDataInterface.GetSafeHwnd())
	{
		int nSelIndex = -1;
		for (int idx = 0; idx < m_wndCbmDataInterface.GetCount(); idx ++)
		{
			BYTE itemData = (BYTE)m_wndCbmDataInterface.GetItemData(idx);
			if (itemData == pApp->m_GlobalOptions.m_DataInterface)
			{
				nSelIndex = idx;
				break;
			}
		}

		m_wndCbmDataInterface.SetCurSel(nSelIndex);
		OnCbnSelchangeCmbDatainterface();
	}

	if (m_wndComboBoxIType.GetSafeHwnd())
	{
		int nSelIndex = -1;
		for (int idx = 0; idx < m_wndComboBoxIType.GetCount(); idx ++)
		{
			BYTE itemData  = (BYTE)m_wndComboBoxIType.GetItemData(idx);

			if (itemData == pApp->m_GlobalOptions.m_DataIType)
			{
				nSelIndex = idx;
				break;
			}
		}

		m_wndComboBoxIType.SetCurSel(nSelIndex);
		OnCbnSelchangeCmbItype();
	}


	if (m_wndCmbDatabase.GetSafeHwnd())
	{
		int nSelIndex = -1;
		for (int idx = 0;idx < m_wndCmbDatabase.GetCount();idx ++)
		{
			BYTE itemData = (BYTE)m_wndCmbDatabase.GetItemData(idx);
			if(itemData == pApp->m_GlobalOptions.m_DataBaseType)
			{
				nSelIndex  = idx;
				break;
			}
		}

		m_wndCmbDatabase.SetCurSel(nSelIndex);
	}


	ShowWindow(nCmdShow);
}

BOOL CUdfCfgODBCDlg::SaveOdbcOptions()
{
	BOOL result = TRUE;
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();

	if (m_strDSN != pApp->m_GlobalOptions.m_DbOpts.m_strMISDSN ||
		m_strUID != pApp->m_GlobalOptions.m_DbOpts.m_strMISUID ||
		m_strPWD != pApp->m_GlobalOptions.m_DbOpts.m_strMISPWD ||
		m_nDataInterface != pApp->m_GlobalOptions.m_DataInterface ||
		m_nDataIType != pApp->m_GlobalOptions.m_DataIType ||
		m_nDatabase != pApp->m_GlobalOptions.m_DataBaseType)
	{
		pApp->m_GlobalOptions.m_DbOpts.m_strMISDSN = m_strDSN;
		pApp->m_GlobalOptions.m_DbOpts.m_strMISUID = m_strUID;
		pApp->m_GlobalOptions.m_DbOpts.m_strMISPWD = m_strPWD;
		pApp->m_GlobalOptions.m_DataInterface = m_nDataInterface;
		pApp->m_GlobalOptions.m_DataIType = m_nDataIType;
		pApp->m_GlobalOptions.m_DataBaseType = m_nDatabase;

		pApp->m_sysConfigObj.SetItemValue("SYSDBDSN",m_strDSN);
		pApp->m_sysConfigObj.SetItemValue("SYSDBUID",m_strUID);
		pApp->m_sysConfigObj.SetItemValue("SYSDBPWD",m_strPWD);
		pApp->m_sysConfigObj.SetItemValue("DATA_INTERFACE",(INT)m_nDataInterface);
		pApp->m_sysConfigObj.SetItemValue("DATA_ITYPE",(INT)m_nDataIType);
		pApp->m_sysConfigObj.SetItemValue("DATABASE_TYPE",(INT)m_nDatabase);
		pApp->m_sysConfigObj.SaveFile();
	}

	return result;
}
void CUdfCfgODBCDlg::OnClose()
{
	ShowWindow(SW_HIDE);
}

void CUdfCfgODBCDlg::OnCbnSelchangeCmbDatainterface()
{
	int nSel = m_wndCbmDataInterface.GetCurSel();
	if (nSel != CB_ERR)
	{
		m_nDataInterface = (BYTE)m_wndCbmDataInterface.GetItemData(nSel);

		if (m_nDataInterface == 1)
		{
			m_wndComboBoxIType.ShowWindow(SW_SHOW);
			m_wndStaticIType.ShowWindow(SW_SHOW);
		}
		else
		{
			m_wndStaticIType.ShowWindow(SW_HIDE);
			m_wndComboBoxIType.ShowWindow(SW_HIDE);
			m_wndStaticIName.SetWindowText("ODBC数据源:");

			int ISel = -1;
			for (int Idx = 0; Idx <m_wndComboBoxIType.GetCount(); Idx++)
			{
				int Idata = m_wndComboBoxIType.GetItemData(Idx);
				if (Idata == 1)
				{
					nSel = Idx;
					break;
				}
			}
			m_wndComboBoxIType.SetCurSel(nSel);
		}
		OnCbnSelchangeCmbItype();
	}
}

void CUdfCfgODBCDlg::OnCbnSelchangeCmbItype()
{
	int nsel = m_wndComboBoxIType.GetCurSel();
	if (nsel != CB_ERR)
	{
	   m_nDataIType = (BYTE)m_wndComboBoxIType.GetItemData(nsel);

	   if (m_nDataIType == 1)
	   {
		   m_wndStaticIName.SetWindowText("ODBC数据源:");
		   m_wndStaticDatabase.ShowWindow(SW_SHOW);
		   m_wndCmbDatabase.ShowWindow(SW_SHOW);


		   if (1 == m_nDataInterface)
		   {
			   int dbSel = -1;
				for (int idx = 0; idx < m_wndCmbDatabase.GetCount();idx ++)
				{
					if (SQLServer_Database == (BYTE)m_wndCmbDatabase.GetItemData(idx))
					{
						dbSel = idx;
						break;
					}
				}

				m_wndCmbDatabase.SetCurSel(dbSel);
				m_wndCmbDatabase.EnableWindow(FALSE);
		   }
		   else 
			   m_wndCmbDatabase.EnableWindow(TRUE);

	   }
	   else
	   {
		   m_wndStaticIName.SetWindowText("WebService网址:");
		   m_wndStaticDatabase.ShowWindow(SW_HIDE);
		   m_wndCmbDatabase.ShowWindow(SW_HIDE);
		   
	   }
	}
}
