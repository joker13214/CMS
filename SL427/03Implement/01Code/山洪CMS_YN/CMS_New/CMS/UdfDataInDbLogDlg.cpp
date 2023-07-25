// UdfDataInDbLogDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "UdfDataInDbLogDlg.h"
#include "MainFrm.h"

// CUdfDataInDbLogDlg 对话框

IMPLEMENT_DYNAMIC(CUdfDataInDbLogDlg, CDialog)

CUdfDataInDbLogDlg::CUdfDataInDbLogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUdfDataInDbLogDlg::IDD, pParent)
{

}

CUdfDataInDbLogDlg::~CUdfDataInDbLogDlg()
{
}

void CUdfDataInDbLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DATAINDBLOG, m_wndListCtrl);
}


BEGIN_MESSAGE_MAP(CUdfDataInDbLogDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_NOTIFY(NM_RCLICK, IDC_LIST_DATAINDBLOG, &CUdfDataInDbLogDlg::OnNMRclickListDataindblog)
	ON_COMMAND(IDM_OP_CLEAR,&CUdfDataInDbLogDlg::OnClearLogList)
	ON_COMMAND(IDM_OP_SAVEAS,&CUdfDataInDbLogDlg::OnSaveAsLogList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DATAINDBLOG, &CUdfDataInDbLogDlg::OnNMDblclkListDataindblog)
END_MESSAGE_MAP()


// CUdfDataInDbLogDlg 消息处理程序

BOOL CUdfDataInDbLogDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (m_wndListCtrl.GetSafeHwnd())
	{
		DWORD dw = m_wndListCtrl.GetExtendedStyle();
		dw |= LVS_EX_FULLROWSELECT;
		m_wndListCtrl.SetExtendedStyle(dw);

		m_wndListCtrl.InsertColumn(0, _T("时间"));
		m_wndListCtrl.InsertColumn(1, _T("信息"));
		m_wndListCtrl.SetColumnWidth(0, 140);
		m_wndListCtrl.SetColumnWidth(1, 650);

		RECT rect;
		GetClientRect(&rect);
		m_wndListCtrl.MoveWindow(&rect);
	}

	CenterWindow();
	SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_RTUROOT),FALSE);
	SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	return TRUE;  
	// return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CUdfDataInDbLogDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (nType == SIZE_MINIMIZED)
		ShowWindow(SW_HIDE);

	if (m_wndListCtrl.GetSafeHwnd())
		m_wndListCtrl.MoveWindow(0,0,cx,cy);
}

void CUdfDataInDbLogDlg::AddOneMessage(CString strMessage,CString strTime,_Log_level_type _xLevel/* = Normal_Log_Message*/)
{
	if (strTime.IsEmpty() || strMessage.IsEmpty())
		return;

	if (m_wndListCtrl.GetSafeHwnd())
	{
		int itemCount = m_wndListCtrl.GetItemCount();
		if (itemCount >= 1000)						//最多只显示1000条数据信息
			m_wndListCtrl.DeleteItem(itemCount -1 );
		int itemIdx = m_wndListCtrl.InsertItem(0,strTime);
		m_wndListCtrl.SetItemText(itemIdx,1,strMessage);
		m_wndListCtrl.SetItemData(itemIdx,(DWORD_PTR)_xLevel);
	}
}

void CUdfDataInDbLogDlg::AddOneMessage(CString strMessage,PSYSTEMTIME pTime /* = NULL */,_Log_level_type _xLevel /*= Normal_Log_Message*/)
{
	CString strTime ="";
	if (pTime)
	{
		strTime.Format("%04d-%02d-%02d %02d:%02d:%02d.%03d",
			pTime->wYear,
			pTime->wMonth,
			pTime->wDay,
			pTime->wHour,
			pTime->wMinute,
			pTime->wSecond,
			pTime->wMilliseconds);
	}
	else if (pTime == NULL)
	{
		SYSTEMTIME st;
		GetLocalTime(&st);
		strTime.Format("%04d-%02d-%02d %02d:%02d:%02d.%03d",
			st.wYear,
			st.wMonth,
			st.wDay,
			st.wHour,
			st.wMinute,
			st.wSecond,
			st.wMilliseconds);
	}

	AddOneMessage(strMessage,strTime,_xLevel);
}

void CUdfDataInDbLogDlg::AddOneMessage(CString strMessage,CTime wTime,_Log_level_type _xLevel /*= Normal_Log_Message*/)
{
	CString strTime = "";
	strTime.Format("%04d-%02d-%02d %02d:%02d:%02d",
		wTime.GetYear(),
		wTime.GetMonth(),
		wTime.GetDay(),
		wTime.GetHour(),
		wTime.GetMinute(),
		wTime.GetSecond());

	AddOneMessage(strMessage,strTime,_xLevel);
}

//关闭窗口时，隐藏窗口
void CUdfDataInDbLogDlg::OnClose()
{
	ShowWindow(SW_HIDE);
}

void CUdfDataInDbLogDlg::OnNMRclickListDataindblog(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	CMenu menu;
	menu.LoadMenu(IDR_MENU_POP);
	CPoint cursorPos;
	GetCursorPos(&cursorPos);

	if (menu.GetSafeHmenu())
	{
		CMenu* popupMenu = menu.GetSubMenu(10);
		if (popupMenu && popupMenu->GetSafeHmenu())
			popupMenu->TrackPopupMenu(TPM_LEFTALIGN,cursorPos.x,cursorPos.y,this);

	}
	*pResult = 0;
}


void CUdfDataInDbLogDlg::OnClearLogList()
{
	if (m_wndListCtrl.GetSafeHwnd())
	{
		int ret =MessageBox("确定清空日志信息?","提示",MB_OKCANCEL|MB_ICONQUESTION);
		if (IDOK == ret)
			m_wndListCtrl.DeleteAllItems();
	}
}

//导出日志信息存储到其他文件
void CUdfDataInDbLogDlg::OnSaveAsLogList()
{
	if (m_wndListCtrl.GetSafeHwnd()  && m_wndListCtrl.GetItemCount() > 0)
	{
		CFileDialog fileDlg(FALSE, _T("txt"), _T("日志信息"),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||"),this);
		if (fileDlg.DoModal() == IDOK)
		{
			CString sFilename = fileDlg.GetPathName();

			try
			{
				CStdioFile file(sFilename,CFile::modeCreate|CFile::modeWrite);

				int nItemCount = m_wndListCtrl.GetItemCount();
				for (int idx = 0; idx < nItemCount; idx ++)
				{
					CString strDatetime="", strMessages = "";
					strDatetime	= m_wndListCtrl.GetItemText(idx,0);
					strMessages	= m_wndListCtrl.GetItemText(idx,1);


					CString strWrite ="";
					strWrite.Format("%s,%s\n",
						strDatetime,
						strMessages);

					file.WriteString(strWrite);
				}
				file.Flush();

				m_wndListCtrl.Invalidate();
			}
			catch (CFileException* e)
			{
				e->ReportError(MB_OK);
				e->Delete();
				return;
			}
		}
	}
}
void CUdfDataInDbLogDlg::OnNMDblclkListDataindblog(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	LPNMITEMACTIVATE pItem = ( LPNMITEMACTIVATE )pNMHDR;
	if (pItem->iItem != -1)
	{
		CString strDate = m_wndListCtrl.GetItemText(pItem->iItem,0);
		CString strLog = m_wndListCtrl.GetItemText(pItem->iItem,1);

		SHOW_LOG_MESSAGE(strDate,strLog);
	}

	*pResult = 0;
}
