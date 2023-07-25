// RtuAttrDataShow.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuAttrDataShow.h"
#include "RtuPropertyDlg.h"
#include "DataTranSQLServer_SC.h"
#include <fstream>
#include <stdlib.h>
// CRtuAttrDataShow 对话框
IMPLEMENT_DYNAMIC(CRtuAttrDataShow, CDialog)

CRtuAttrDataShow::CRtuAttrDataShow(CWnd* pParent /*=NULL*/)
	: CDialog(CRtuAttrDataShow::IDD, pParent)
{
	m_pParentDlg	= NULL;
	m_pSubStation	= NULL;
	m_nRowsMax		= 100;
}

CRtuAttrDataShow::~CRtuAttrDataShow()
{
}

void CRtuAttrDataShow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRtuAttrDataShow, CDialog)
	ON_NOTIFY(NM_RCLICK,IDC_DATASHOW_GRIDCTRL,&CRtuAttrDataShow::OnRClickGridCtrl)
	ON_COMMAND(IDM_OP_CLEAR,&CRtuAttrDataShow::OnClearMessage)
	ON_COMMAND(IDM_OP_SAVEAS,&CRtuAttrDataShow::OnSaveAsMessage)
	ON_COMMAND(IDM_OP_Print,&CRtuAttrDataShow::OnPrintAsMessage)
END_MESSAGE_MAP()


// CRtuAttrDataShow 消息处理程序
BOOL CRtuAttrDataShow::OnInitDialog()
{
	CDialog::OnInitDialog();

   if (!OnInitGridCtrl())
	   return FALSE;

	return TRUE;  
}


void CRtuAttrDataShow::Update()
{
	if (!m_pParentDlg) return;
	if (!m_pParentDlg->GetRtuStation()) return;

	m_pSubStation = m_pParentDlg->GetRtuStation();

}

BOOL CRtuAttrDataShow::OnInitGridCtrl()
{
	CWnd* pWnd = GetDlgItem(IDC_DATASHOW_GRIDSTATIC);
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
		this, IDC_DATASHOW_GRIDCTRL,WS_CHILD|WS_VISIBLE))
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
	return TRUE;
}


void CRtuAttrDataShow::InitGridHeader()
{
	m_wndGridCtrl.DeleteAllItems();
	m_wndGridCtrl.SetColumnCount(2);
	m_wndGridCtrl.SetFixedRowCount(1);
	m_wndGridCtrl.SetColumnWidth(0,140);
	m_wndGridCtrl.SetColumnWidth(1,3000);
	GV_ITEM item;
	item.mask = GVIF_TEXT | GVIF_FORMAT;
	item.row  = 0;
	item.col = 0;
	item.nFormat = DT_LEFT | DT_WORDBREAK|DT_VCENTER;//|DT_CALCRECT;
	item.strText.Format(_T("%s"), "时间");
	m_wndGridCtrl.SetItem(&item);
	item.col  = 1;
	item.strText.Format(_T("%s"), "数据信息");
	m_wndGridCtrl.SetItem(&item);
}

void CRtuAttrDataShow::AddMessageToWindow(CString strMessage,PSYSTEMTIME pTime /* = NULL */)
{
	SYSTEMTIME sTime;

	if (pTime == NULL)
		GetLocalTime(&sTime);
	else
	{
		sTime.wDay			= pTime->wDay;
		sTime.wDayOfWeek	= pTime->wDayOfWeek;
		sTime.wHour			= pTime->wHour;
		sTime.wMilliseconds	= pTime->wMilliseconds;
		sTime.wMinute		= pTime->wMinute;
		sTime.wMonth		= pTime->wMonth;
		sTime.wSecond		= pTime->wSecond;
		sTime.wYear			= pTime->wYear;
	}

	CString strTime = "";
	//strTime.Format("%04d-%02d-%02d %02d:%02d:%02d",
	//	sTime.wYear,
	//	sTime.wMonth,
	//	sTime.wDay,
	//	sTime.wHour,
	//	sTime.wMinute,
	//	sTime.wSecond);
	strTime.Format("%04d-%02d-%02d %02d:%02d:%02d.%03d",
		sTime.wYear,
		sTime.wMonth,
		sTime.wDay,
		sTime.wHour,
		sTime.wMinute,
		sTime.wSecond,
		sTime.wMilliseconds);
	AddMessageToWindow(strTime,strMessage);
}

void CRtuAttrDataShow::AddMessageToWindow(CString strTime,CString strMessage)
{
	if (m_wndGridCtrl.GetSafeHwnd())
	{
		int nRowCount =m_wndGridCtrl.GetRowCount();
		int nItemRow = -1;

		if (nRowCount == 1)
			nItemRow = m_wndGridCtrl.InsertRow("");
		else
			nItemRow = m_wndGridCtrl.InsertRow("",1);

		if (nItemRow != -1)  //插入成功
		{
			GV_ITEM rowItem;
			rowItem.mask = GVIF_TEXT | GVIF_FORMAT;
			rowItem.row  = nItemRow;
			rowItem.col  = 0;
			rowItem.nFormat = DT_VCENTER|DT_LEFT|DT_WORDBREAK;;

			rowItem.strText= strTime;
			m_wndGridCtrl.SetItem(&rowItem);


			rowItem.col  = 1;
			rowItem.strText = strMessage;

			int nLine = strMessage.Replace("\r\n","\r\n");
			m_wndGridCtrl.SetItem(&rowItem);
			m_wndGridCtrl.SetRowHeight(nItemRow,(nLine /*+ 1*/) * MFC_GRIDCTRL_HEIGHT/*13*/);
			m_wndGridCtrl.Invalidate();
		}

		if(m_wndGridCtrl.GetRowCount() > m_nRowsMax)
		{
			nRowCount = m_wndGridCtrl.GetRowCount();
			m_wndGridCtrl.DeleteRow(nRowCount - 1);
		}
	}
}

void CRtuAttrDataShow::OnClearMessage()
{
    int ret = MessageBox("确定清空所有信息?","提示",MB_OKCANCEL|MB_ICONQUESTION);
	if (ret == IDCANCEL)
		return;

	if (m_wndGridCtrl.GetSafeHwnd())
	{
		InitGridHeader();
	}
	if (m_pSubStation)
	{
		m_pSubStation->ClearDataList();
	}
}

//存储数据信息到其他文件
void CRtuAttrDataShow::OnSaveAsMessage()
{
	if (m_wndGridCtrl.GetSafeHwnd() && m_wndGridCtrl.GetRowCount() > 1)
	{
		CFileDialog fileDlg(FALSE, _T("txt"), _T("自报数据信息"),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||"),this);
		if (fileDlg.DoModal() == IDOK)
		{
			CString sFilename = fileDlg.GetPathName();

			try
			{
				CStdioFile file(sFilename,CFile::modeCreate|CFile::modeWrite);

				int nItemCount = m_wndGridCtrl.GetRowCount();

				for (int idx = 1; idx < nItemCount; idx ++)
				{
					CString strDatetime="", strMessages = "";
	
					strDatetime = CUdfGlobalFUNC::GetGridCellText(&m_wndGridCtrl,idx,0);
					strMessages = CUdfGlobalFUNC::GetGridCellText(&m_wndGridCtrl,idx,1);


					CString strWrite ="";
					strWrite.Format("%s,%s",
							strDatetime,
							strMessages);

					strWrite.Replace("\r\n","\n");
					file.WriteString(strWrite);
				}
				file.Flush();

				m_wndGridCtrl.Invalidate();
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
void CRtuAttrDataShow::OnPrintAsMessage()
{
	CFileDialog fileDlg(FALSE, _T("csv"), _T("流量数据信息"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Csv Files (*.csv)|*.csv|All Files (*.*)|*.*||"), this);
	if (fileDlg.DoModal() == IDOK)
	{
		CString sFilename = fileDlg.GetPathName();
		try
		{
			std::ofstream datafile;
			datafile.open(sFilename, std::ios::out | std::ios::trunc);

			std::list<CString>::iterator it;
			int i = 0;
			for(it = FlowDatalist.begin();it!=FlowDatalist.end();it++)
			{
				datafile << *it << ",";
				i++;
				if (i>0 && i % 4 == 0) {
					datafile << std::endl;
				}
			}
			datafile.close();
		}
		catch (CFileException* e)
		{
			e->ReportError(MB_OK);
			e->Delete();
			return;
		}
	}
	FlowDatalist.clear();    //打印一次就清除里面的所有元素

}


void CRtuAttrDataShow::OnRClickGridCtrl(NMHDR* pNotifyStruct,LRESULT *pResutl)
{
	NM_GRIDVIEW* pItem =(NM_GRIDVIEW*)pNotifyStruct;
	CPoint CursorPos;
	GetCursorPos(&CursorPos);

	CMenu menu;
	menu.LoadMenu(IDR_MENU_POP);
	if (menu.GetSafeHmenu())
	{
		CMenu* popupMenu = menu.GetSubMenu(10);
		if (popupMenu && popupMenu->GetSafeHmenu())
			popupMenu->TrackPopupMenu(TPM_LEFTALIGN,CursorPos.x,CursorPos.y,this);
	}
}