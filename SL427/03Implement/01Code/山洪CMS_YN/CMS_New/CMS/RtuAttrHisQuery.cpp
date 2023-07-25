// RtuAttrHisQuery.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuAttrHisQuery.h"
#include "SystemTimeOp.h"


// CRtuAttrHisQuery 对话框

IMPLEMENT_DYNAMIC(CRtuAttrHisQuery, CDialog)

CRtuAttrHisQuery::CRtuAttrHisQuery(CWnd* pParent /*=NULL*/)
	: CDialog(CRtuAttrHisQuery::IDD, pParent)
{

	EnableAutomation();
	m_pParentDlg	= NULL;
	m_pSubStation	= NULL;
	m_sBegin = "";
	m_sEnd = "";

}

CRtuAttrHisQuery::~CRtuAttrHisQuery()
{
}

void CRtuAttrHisQuery::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。基类将自动
	// 删除该对象。在调用该基类之前，请添加您的
	// 对象所需的附加清除代码。

	CDialog::OnFinalRelease();
}

void CRtuAttrHisQuery::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER_BEGIN, m_wndBeginTimeCtrl);
	DDX_Control(pDX, IDC_DATETIMEPICKER_END, m_wndEndTimeCtrl);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_wndQueryTypeCtrl);
	DDX_Control(pDX, IDC_BUTTON1, m_BtnQuery);
}


BEGIN_MESSAGE_MAP(CRtuAttrHisQuery, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CRtuAttrHisQuery::OnBnClickedButton1)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, &CRtuAttrHisQuery::OnCbnSelchangeComboType)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CRtuAttrHisQuery, CDialog)
END_DISPATCH_MAP()

// 注意: 我们添加 IID_IRtuAttrHisQuery 支持
//  以支持来自 VBA 的类型安全绑定。此 IID 必须同附加到 .IDL 文件中的
//  调度接口的 GUID 匹配。

// {72349B29-EC78-4A6A-B036-614B965C66C5}
static const IID IID_IRtuAttrHisQuery =
{ 0x72349B29, 0xEC78, 0x4A6A, { 0xB0, 0x36, 0x61, 0x4B, 0x96, 0x5C, 0x66, 0xC5 } };

BEGIN_INTERFACE_MAP(CRtuAttrHisQuery, CDialog)
	INTERFACE_PART(CRtuAttrHisQuery, IID_IRtuAttrHisQuery, Dispatch)
END_INTERFACE_MAP()

void CRtuAttrHisQuery::Update()
{
	if (!m_pParentDlg) return;
	if (!m_pParentDlg->GetRtuStation()) return;

	m_pSubStation = m_pParentDlg->GetRtuStation();
}

void CRtuAttrHisQuery::SetParentDlg( CRtuPropertyDlg* pParent )
{
	m_pParentDlg = pParent;
}


// CRtuAttrHisQuery 消息处理程序

BOOL CRtuAttrHisQuery::OnInitDialog()
{
	CDialog::OnInitDialog();

	CWnd* pWnd = GetDlgItem(IDC_HISDATASHOW_GRIDSTATIC);
	CRect rect1, rect2;
	int captionHeight = 0;	// ::GetSystemMetrics(SM_CYCAPTION);
	int cxframe = GetSystemMetrics(SM_CXFRAME);
	int cyframe = GetSystemMetrics(SM_CYFRAME);
	this->GetWindowRect(&rect2);
	if (pWnd)
		pWnd->GetWindowRect(&rect1);
	if (!m_wndGridCtrl.Create(CRect(rect1.left - rect2.left - cxframe, 
		rect1.top-rect2.top - cyframe-captionHeight, 
		rect1.left + rect1.Width() - rect2.left, 
		rect1.top + rect1.Height() - rect2.top - captionHeight), 
		this, IDC_HISDATASHOW_GRIDCTRL,WS_CHILD|WS_VISIBLE))
	{
		TRACE("Failed to create threads  grid!\n");
		return FALSE;
	}

	m_wndGridCtrl.SetBkColor(RGB(255,255,255));
	m_wndGridCtrl.SetListMode(TRUE);
	m_wndGridCtrl.SetSingleColSelection(TRUE);
	m_wndGridCtrl.SetSingleRowSelection(TRUE);
	m_wndGridCtrl.SetDefCellHeight(MFC_GRIDCTRL_HEIGHT);
	m_wndGridCtrl.SetEditable(FALSE);
	m_wndGridCtrl.EnableTitleTips(FALSE);
	m_wndGridCtrl.EnableToolTips(FALSE);
	m_wndGridCtrl.SetGridLines(GVL_HORZ);

	InitGridHeader();


	int index =0;
	if (m_wndQueryTypeCtrl.GetSafeHwnd())
	{

		index = m_wndQueryTypeCtrl.AddString("按天查询");
		m_wndQueryTypeCtrl.SetItemData(index, 1);
		index = m_wndQueryTypeCtrl.AddString("按月查询");
		m_wndQueryTypeCtrl.SetItemData(index, 2);
		index = m_wndQueryTypeCtrl.AddString("任意时间段");
		m_wndQueryTypeCtrl.SetItemData(index, 3);
		m_wndQueryTypeCtrl.SetCurSel(0);

		OnCbnSelchangeComboType();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CRtuAttrHisQuery::InitGridHeader()
{
	if (m_wndGridCtrl.GetSafeHwnd())
	{
		m_wndGridCtrl.DeleteAllItems();
		m_wndGridCtrl.SetColumnCount(3);
		m_wndGridCtrl.SetFixedRowCount(1);
		m_wndGridCtrl.SetColumnWidth(0,40);
		m_wndGridCtrl.SetColumnWidth(1,140);
		m_wndGridCtrl.SetColumnWidth(2,440);
		m_wndGridCtrl.SetGridLines();
		GV_ITEM item;
		item.mask = GVIF_TEXT | GVIF_FORMAT;
		item.row  = 0;

		item.col = 0;
		item.nFormat = DT_LEFT | DT_WORDBREAK|DT_VCENTER;//|DT_CALCRECT;
		item.strText.Format("No#");
		m_wndGridCtrl.SetItem(&item);

		item.col = 1;
		item.strText.Format(_T("%s"), "日期时间");
		m_wndGridCtrl.SetItem(&item);

		item.col  = 2;
		item.strText.Format(_T("%s"), "平安报信息");
		m_wndGridCtrl.SetItem(&item);
	}

}

//查询
void CRtuAttrHisQuery::OnBnClickedButton1()
{
	if (!m_pSubStation)
		return;

	m_BtnQuery.EnableWindow(FALSE);
	CWaitCursor waitCursor;
	try
	{
		CCMSApp* pApp = (CCMSApp*)AfxGetApp();
		CString SqliteFile = pApp->m_GlobalOptions.m_strLocalDbName;

		CDbSQLite sqlite;
		CHAR OLD_PATH[MAX_PATH];
		memset(OLD_PATH,0,sizeof(OLD_PATH));
		::GetCurrentDirectory(MAX_PATH,OLD_PATH);
		::SetCurrentDirectory(CUdfGlobalFUNC::GetRuningPath());

		bool ret = sqlite.Open((LPCTSTR)SqliteFile);	
		::SetCurrentDirectory(OLD_PATH);

		if (ret == true)
		{
			UpdateQueryTime();
			if(m_sBegin !="" && m_sEnd != "")
			{
				CString sSQL = "",sRTU_ID="";
				sRTU_ID.Format("%02d%02d%02d_%08d",
					m_pSubStation->GetProvinceCode(),
					m_pSubStation->GetRegionCode(),
					m_pSubStation->GetCountyCode(),
					m_pSubStation->GetAddrA2());
				sSQL.Format("SELECT REPORTTIME,REPORT FROM PEACE_REPORT WHERE RTU='%s' AND REPORTTIME BETWEEN '%s' AND '%s'",
					sRTU_ID,m_sBegin,m_sEnd);

				TRACE("平安报查询:%s\r\n",sSQL);

				CSqlStatement* rs = sqlite.Statement(sSQL);
				if (rs != NULL)
				{
					InitGridHeader();
					while (rs->NextRow())
					{
						CString strTime = "",strReport = "";
						strTime = rs->ValueString(0);
						strReport = rs->ValueString(1);

						AddMessageToGridCtrl(strTime,strReport);

					}
					_DELETE(rs);
					m_wndGridCtrl.Invalidate();
				}
			}

		}
	}
	catch (CException* e)
	{
		e->Delete();
	}
	m_BtnQuery.EnableWindow(TRUE);
}

void CRtuAttrHisQuery::AddMessageToGridCtrl( CString strTime,CString strReport )
{
	if (m_wndGridCtrl.GetSafeHwnd())
	{
		int nRowsCount = m_wndGridCtrl.GetRowCount();
		int Index = m_wndGridCtrl.InsertRow("");

		if (Index != -1)
		{
			GV_ITEM rowItem;
			rowItem.mask = GVIF_TEXT | GVIF_FORMAT;
			rowItem.row  = Index;
			rowItem.col  = 0;
			rowItem.nFormat = DT_VCENTER|DT_LEFT|DT_WORDBREAK;;
			rowItem.strText.Format("%d",nRowsCount);
		    m_wndGridCtrl.SetItem(&rowItem);

			rowItem.col = 1;
			rowItem.strText= strTime;
			m_wndGridCtrl.SetItem(&rowItem);

			rowItem.col  = 2;
			rowItem.strText = strReport;

			int nLine = strReport.Replace("\r\n","\r\n");
			m_wndGridCtrl.SetItem(&rowItem);
			m_wndGridCtrl.SetRowHeight(Index,nLine * 14/*13*/);
			m_wndGridCtrl.Invalidate();
		}
	}
}


void CRtuAttrHisQuery::OnCbnSelchangeComboType()
{
	int index = m_wndQueryTypeCtrl.GetCurSel();
	if (index != CB_ERR)
	{
		int  m_queryType =  m_wndQueryTypeCtrl.GetItemData(index);
		SYSTEMTIME localtime;
		::GetLocalTime(&localtime);
		SYSTEMTIME begintime = localtime;
		m_wndEndTimeCtrl.SetTime(&localtime);
		m_wndBeginTimeCtrl.SetTime(&begintime);

		if (m_queryType == 3)
		{
			m_wndBeginTimeCtrl.ShowWindow(SW_SHOW);
			m_wndEndTimeCtrl.ShowWindow(SW_SHOW);
			begintime.wDay = 1;     //月初
			begintime.wHour = 0;
			begintime.wMinute = 0;
			begintime.wSecond = 0;
			begintime.wMilliseconds = 0;

			m_wndEndTimeCtrl.SetTime(&localtime);
			m_wndBeginTimeCtrl.SetTime(&begintime);

			m_wndBeginTimeCtrl.SetFormat("yyyy年MM月dd日 HH时mm分ss秒");
			m_wndEndTimeCtrl.SetFormat("yyyy年MM月dd日 HH时mm分ss秒");
		}
		else if (m_queryType == 1)
		{

			m_wndEndTimeCtrl.ShowWindow(SW_HIDE);
			m_wndBeginTimeCtrl.SetFormat("yyyy年MM月dd日");
		}
		else if (m_queryType == 2)
		{
			m_wndEndTimeCtrl.ShowWindow(SW_HIDE);
			m_wndBeginTimeCtrl.SetFormat("yyyy年MM月");
		}
	}
}

void CRtuAttrHisQuery::UpdateQueryTime()
{
	UpdateData();
	m_sBegin = "";
	m_sEnd = "";
	int index = m_wndQueryTypeCtrl.GetCurSel();
	if (index == CB_ERR) return;

	int  m_queryType =  m_wndQueryTypeCtrl.GetItemData(index);

	SYSTEMTIME m_timeBegin;
	SYSTEMTIME m_timeEnd;
	if (m_queryType == 3)
	{	
		CTime timeBegin, timeEnd;
		m_wndBeginTimeCtrl.GetTime(timeBegin);
		m_wndEndTimeCtrl.GetTime(timeEnd);
		if (timeBegin > timeEnd)
		{
			timeEnd.GetAsSystemTime(m_timeBegin);
			timeBegin.GetAsSystemTime(m_timeEnd);
		}
		else
		{
			timeBegin.GetAsSystemTime(m_timeBegin);
			timeEnd.GetAsSystemTime(m_timeEnd);
		}
	}
	else if (m_queryType == 1)
	{
		CTime time;
		m_wndBeginTimeCtrl.GetTime(time);
		time.GetAsSystemTime(m_timeBegin);
		CTimeSpan ts(1, 0, 0, 0);
		time = time + ts;
		time.GetAsSystemTime(m_timeEnd);
		m_timeBegin.wHour = m_timeBegin.wMinute = m_timeBegin.wSecond = m_timeBegin.wMilliseconds = 0;
		m_timeEnd.wHour =m_timeEnd.wMinute = m_timeEnd.wSecond = m_timeEnd.wMilliseconds = 0;
	}
	else if (m_queryType == 2)
	{
		CTime time;
		m_wndBeginTimeCtrl.GetTime(time);
		time.GetAsSystemTime(m_timeBegin);

		m_timeBegin.wDay = 1;
		m_timeBegin.wHour = m_timeBegin.wMinute = m_timeBegin.wSecond = m_timeBegin.wMilliseconds = 0;

		int days = CSystemTimeOp::GetDaysOfMonth(m_timeBegin.wYear, m_timeBegin.wMonth);		
		m_timeEnd.wYear = m_timeBegin.wYear;
		m_timeEnd.wMonth = m_timeBegin.wMonth;
		m_timeEnd.wDay = days;
		m_timeEnd.wHour =23;
		m_timeEnd.wMinute = 59;
		m_timeEnd.wSecond = 59;
		m_timeEnd.wMilliseconds = 999;
	}

	m_sBegin.Format("%04d-%02d-%02d %02d:%02d:%02d.%03d",
		m_timeBegin.wYear,
		m_timeBegin.wMonth,
		m_timeBegin.wDay,
		m_timeBegin.wHour,
		m_timeBegin.wMinute,
		m_timeBegin.wSecond,
		m_timeBegin.wMilliseconds);
	m_sEnd.Format("%04d-%02d-%02d %02d:%02d:%02d.%03d",
		m_timeEnd.wYear,
		m_timeEnd.wMonth,
		m_timeEnd.wDay,
		m_timeEnd.wHour,
		m_timeEnd.wMinute,
		m_timeEnd.wSecond,
		m_timeEnd.wMilliseconds);
}

