// UdfLogQueryDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "UdfLogQueryDlg.h"
#include "SystemTimeOp.h"


// CUdfLogQueryDlg 对话框

IMPLEMENT_DYNAMIC(CUdfLogQueryDlg, CDialog)

CUdfLogQueryDlg::CUdfLogQueryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUdfLogQueryDlg::IDD, pParent)
{

}

CUdfLogQueryDlg::~CUdfLogQueryDlg()
{
}

void CUdfLogQueryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BEGIN_TEXT, m_wndBeginTextCtrl);
	DDX_Control(pDX, IDC_END_TEXT, m_wndEndTextCtrl);
	DDX_Control(pDX, IDC_END_TIME, m_wndEndTimeCtrl);
	DDX_Control(pDX, IDC_BEGIN_TIME, m_wndBeginTimeCtrl);
	DDX_Control(pDX, IDC_QUERY_TYPE, m_wndQueryTypeCtrl);
	DDX_Control(pDX, IDC_LOG_TYPE, m_wndQueryLogTypeCtrl);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CUdfLogQueryDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_QUERY_TYPE, &CUdfLogQueryDlg::OnCbnSelchangeQueryType)
	ON_CBN_SELCHANGE(IDC_LOG_TYPE, &CUdfLogQueryDlg::OnCbnSelchangeLogType)
	ON_BN_CLICKED(IDOK, &CUdfLogQueryDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CUdfLogQueryDlg 消息处理程序

BOOL CUdfLogQueryDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	int index =0;
	if (m_wndQueryTypeCtrl.GetSafeHwnd())
	{

		index = m_wndQueryTypeCtrl.AddString("按分钟查询");
		m_wndQueryTypeCtrl.SetItemData(index, QUERYTYPE_MINUTE);
		index = m_wndQueryTypeCtrl.AddString("按小时查询");
		m_wndQueryTypeCtrl.SetItemData(index, QUERYTYPE_HOUR);
		index = m_wndQueryTypeCtrl.AddString("按天查询");
		m_wndQueryTypeCtrl.SetItemData(index, QUERYTYPE_DAY);
		//index = m_wndQueryTypeCtrl.AddString("按月查询");
		//m_wndQueryTypeCtrl.SetItemData(index, QUERYTYPE_MONTH);
		index = m_wndQueryTypeCtrl.AddString("任意时间段");
		m_wndQueryTypeCtrl.SetItemData(index, QUERYTYPE_ANYTIME);
		m_wndQueryTypeCtrl.SetCurSel(0);
		OnCbnSelchangeQueryType();
	}

	if (m_wndQueryLogTypeCtrl.GetSafeHwnd())
	{
		index = m_wndQueryLogTypeCtrl.AddString("全部");
		m_wndQueryLogTypeCtrl.SetItemData(index,QUERY_LOGTYPE_ALL);
		index = m_wndQueryLogTypeCtrl.AddString("警告");
		m_wndQueryLogTypeCtrl.SetItemData(index,QUERY_LOGTYPE_WORNING);
		index = m_wndQueryLogTypeCtrl.AddString("错误");
		m_wndQueryLogTypeCtrl.SetItemData(index,QUERY_LOGTYPE_ERROR);
		index = m_wndQueryLogTypeCtrl.AddString("警告和错误");
		m_wndQueryLogTypeCtrl.SetItemData(index,QUERY_LOGTYPE_WORNINGANDERROR);
		m_wndQueryLogTypeCtrl.SetCurSel(0);
		OnCbnSelchangeLogType();

	}

    CenterWindow(GetParent());


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CUdfLogQueryDlg::OnCbnSelchangeQueryType()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = m_wndQueryTypeCtrl.GetCurSel();
	if (index != CB_ERR)
		SetQueryType((QUERYTYPE) m_wndQueryTypeCtrl.GetItemData(index));

}

void CUdfLogQueryDlg::SetQueryType(QUERYTYPE queryType)
{

	m_queryType = queryType;
	SYSTEMTIME localtime;
	::GetLocalTime(&localtime);
	SYSTEMTIME begintime = localtime;
	m_wndEndTimeCtrl.SetTime(&localtime);
	m_wndBeginTimeCtrl.SetTime(&begintime);

	if (queryType == QUERYTYPE_ANYTIME)
	{
		m_wndBeginTextCtrl.ShowWindow(SW_SHOW);
		m_wndBeginTimeCtrl.ShowWindow(SW_SHOW);
		m_wndEndTextCtrl.ShowWindow(SW_SHOW);
		m_wndEndTimeCtrl.ShowWindow(SW_SHOW);
		m_wndBeginTextCtrl.SetWindowText("起始时间");

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
	else if (queryType == QUERYTYPE_MINUTE)
	{   
		m_wndBeginTextCtrl.SetWindowText("分选择");
		m_wndEndTextCtrl.ShowWindow(SW_HIDE);
		m_wndEndTimeCtrl.ShowWindow(SW_HIDE);
		m_wndBeginTimeCtrl.SetFormat("yyyy年MM月dd日 HH时mm分");
	}
	else if (queryType == QUERYTYPE_HOUR)
	{
		m_wndBeginTextCtrl.SetWindowText("时选择");
		m_wndEndTextCtrl.ShowWindow(SW_HIDE);
		m_wndEndTimeCtrl.ShowWindow(SW_HIDE);
		m_wndBeginTimeCtrl.SetFormat("yyyy年MM月dd日 HH时");
	}
	else if (queryType == QUERYTYPE_DAY)
	{
		m_wndBeginTextCtrl.SetWindowText("日选择");
		m_wndEndTextCtrl.ShowWindow(SW_HIDE);
		m_wndEndTimeCtrl.ShowWindow(SW_HIDE);
		m_wndBeginTimeCtrl.SetFormat("yyyy年MM月dd日");
	}
	else if (queryType == QUERYTYPE_MONTH)
	{
		m_wndBeginTextCtrl.SetWindowText("月选择");
		m_wndEndTextCtrl.ShowWindow(SW_HIDE);
		m_wndEndTimeCtrl.ShowWindow(SW_HIDE);
		m_wndBeginTimeCtrl.SetFormat("yyyy年MM月");
	}
}
void CUdfLogQueryDlg::OnCbnSelchangeLogType()
{
	int index = m_wndQueryLogTypeCtrl.GetCurSel();
	if (index != CB_ERR)
		m_queryLogType = (QUERY_LOG_TYPE)m_wndQueryLogTypeCtrl.GetItemData(index);
}

void CUdfLogQueryDlg::OnBnClickedOk()
{
	UpdateData();
	if (m_queryType == QUERYTYPE_ANYTIME)
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
	else if (m_queryType == QUERYTYPE_MINUTE)
	{
		CTime time;
		m_wndBeginTimeCtrl.GetTime(time);
		time.GetAsSystemTime(m_timeBegin);
		CTimeSpan ts(0, 0, 1, 0);
		time = time + ts;
		time.GetAsSystemTime(m_timeEnd);
		m_timeBegin.wSecond = m_timeBegin.wMilliseconds = 0;
		m_timeEnd.wSecond = m_timeEnd.wMilliseconds = 0;
	}
	else if (m_queryType == QUERYTYPE_HOUR)
	{
		CTime time;
		m_wndBeginTimeCtrl.GetTime(time);
		time.GetAsSystemTime(m_timeBegin);
		CTimeSpan ts(0, 1, 0, 0);
		time = time + ts;
		time.GetAsSystemTime(m_timeEnd);
		m_timeBegin.wMinute = m_timeBegin.wSecond = m_timeBegin.wMilliseconds = 0;
		m_timeEnd.wMinute = m_timeEnd.wSecond = m_timeEnd.wMilliseconds = 0;
	}
	else if (m_queryType == QUERYTYPE_DAY)
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
	else if (m_queryType == QUERYTYPE_MONTH)
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

	OnOK();
}
