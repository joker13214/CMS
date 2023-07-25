// UdfLogRightView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CMS.h"
#include "UdfLogRightView.h"
#include "CMS.h"
#include "MainFrm.h"
#include "UdfLogQueryDlg.h"
#include "SystemTimeOp.h"
#include "UdfLogFrame.h"
#include "UdfLogSystem.h"


// CUdfLogRightView

IMPLEMENT_DYNCREATE(CUdfLogRightView, CView)
IMPLEMENT_MENUXP(CUdfLogRightView, CView)

CUdfLogRightView::CUdfLogRightView()
{
	m_queryLogType = CUdfLogQueryDlg::QUERY_LOGTYPE_ALL;
	memset(&m_queryBeginTime,0,sizeof(SYSTEMTIME));
	memset(&m_queryEndTime,0,sizeof(SYSTEMTIME));
	m_pFrameWnd = NULL;


	EnableAutomation();
}

CUdfLogRightView::~CUdfLogRightView()
{

}

void CUdfLogRightView::OnFinalRelease()
{
	// �ͷ��˶��Զ�����������һ�����ú󣬽�����
	// OnFinalRelease�����ཫ�Զ�
	// ɾ���ö����ڵ��øû���֮ǰ�����������
	// ��������ĸ���������롣

	CView::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CUdfLogRightView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_MENUXP_MESSAGES()
	ON_NOTIFY(NM_DBLCLK,IDC_LOGMSG_LISTCTRL,&CUdfLogRightView::OnDBClickLogMessage)
	ON_NOTIFY(NM_RCLICK,IDC_LOGMSG_LISTCTRL,&CUdfLogRightView::OnRButtonClickLogMessage)
	ON_NOTIFY(NM_CLICK, IDC_LOGMSG_LISTCTRL,&CUdfLogRightView::OnLClickLogMessage)
	ON_COMMAND(IDM_LOG_QUERY,&CUdfLogRightView::OnCmdLogQuery)
	ON_COMMAND(IDM_LOG_DELETE,&CUdfLogRightView::OnCmdLogDelete)
	ON_COMMAND(IDM_LOG_EXPORT,&CUdfLogRightView::OnCmdLogExport)
	ON_COMMAND(IDM_LOG_DELALL,&CUdfLogRightView::OnCmdLogDeleteAll)
	ON_COMMAND(IDM_LOG_DEL1MONTH,&CUdfLogRightView::OnCmdLogDelete1MonthPre)
	ON_COMMAND(IDM_LOG_DEL2MONTH,&CUdfLogRightView::OnCmdLogDelete2MonthPre)
	ON_COMMAND(IDM_LOG_DELCUSTOM,&CUdfLogRightView::OnCmdLogDeleteCustom)
	ON_COMMAND(IDM_LOG_REFRESH,&CUdfLogRightView::OnCmdLogRefresh)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CUdfLogRightView, CView)
END_DISPATCH_MAP()

// ע��: ������� IID_IUdfLogRightView ֧��
//  ��֧������ VBA �����Ͱ�ȫ�󶨡��� IID ����ͬ���ӵ� .IDL �ļ��е�
//  ���Ƚӿڵ� GUID ƥ�䡣

// {C2C2C2E3-6081-4565-A0A6-AC032BB2E009}
static const IID IID_IUdfLogRightView =
{ 0xC2C2C2E3, 0x6081, 0x4565, { 0xA0, 0xA6, 0xAC, 0x3, 0x2B, 0xB2, 0xE0, 0x9 } };

BEGIN_INTERFACE_MAP(CUdfLogRightView, CView)
	INTERFACE_PART(CUdfLogRightView, IID_IUdfLogRightView, Dispatch)
END_INTERFACE_MAP()


// CUdfLogRightView ��ͼ

void CUdfLogRightView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: �ڴ���ӻ��ƴ���
}


// CUdfLogRightView ���

#ifdef _DEBUG
void CUdfLogRightView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CUdfLogRightView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CUdfLogRightView ��Ϣ�������

int CUdfLogRightView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CMenuXP::SetXPLookNFeel(this);
	// TODO:  �ڴ������ר�õĴ�������
	CRect cr;
	GetClientRect(&cr);


	if (!m_wndBackGroud.Create("",WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this))
	{
		return -1;
	}

	if (!m_wndCaption.Create("��־ϵͳ",WS_VISIBLE| WS_CHILD,CRect(0,0,0,0),this))
	{
		return -1;
	}

	//m_pListCtrl = new CUdfListCtrl();
	if (!m_pListCtrl.Create(WS_VISIBLE|WS_CHILD|LVS_REPORT|LVS_SHOWSELALWAYS|LVS_SINGLESEL|LVS_ALIGNLEFT,cr,this,IDC_LOGMSG_LISTCTRL))
	{
		return -1;
	}

	m_pListCtrl.SetExtendedStyle(m_pListCtrl.GetExtendedStyle() 
		| LVS_EX_FULLROWSELECT
		/*| LVS_EX_GRIDLINES*/
		| LVS_EX_ONECLICKACTIVATE
		/*| LVS_EX_CHECKBOXES*/);

	CFont* pFont = m_pListCtrl.GetFont();
	m_wndCaption.SetFont(pFont);

	if (m_pListCtrl.GetSafeHwnd())
	{
		//m_pListCtrl.InsertColumn(0,_T("No#"),LVCFMT_LEFT,30 );
		m_pListCtrl.InsertColumn(0,_T(""),LVCFMT_LEFT,0 );
		m_pListCtrl.InsertColumn(1,_T("����"),LVCFMT_LEFT,40 );
		m_pListCtrl.InsertColumn(2,_T("����ʱ��"),LVCFMT_LEFT,130);
		m_pListCtrl.InsertColumn(3,_T("��Դ"),LVCFMT_LEFT,110);
		m_pListCtrl.InsertColumn(4,_T("��Ϣ"),LVCFMT_LEFT,650/*3000*/);
	}

	return 0;
}

void CUdfLogRightView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	/*
	if (m_pListCtrl.GetSafeHwnd())
	{
		m_pListCtrl.MoveWindow(0,0,cx,cy);
	}
	*/
	if (m_wndBackGroud.GetSafeHwnd())
		m_wndBackGroud.MoveWindow(0,0,cx,17);
	if (m_wndCaption.GetSafeHwnd())
		m_wndCaption.MoveWindow(2,4,cx-2,16);
	if (m_pListCtrl.GetSafeHwnd())
		m_pListCtrl.MoveWindow(0,20,cx,cy-20);

}

void CUdfLogRightView::OnDestroy()
{
	CView::OnDestroy();
}


BOOL CUdfLogRightView::ShowLogInformation(_LOG_MESSAGE_TYPE _nType,CUdfLogQueryDlg::QUERY_LOG_TYPE queryLogType,PSYSTEMTIME _beginTime/* =NULL */, PSYSTEMTIME _endTime /* = NULL */)
{
	CWaitCursor wCursor;
	if (m_pListCtrl.GetSafeHwnd())
	{
		m_pListCtrl.DeleteAllItems();

		CPtrList* pSet = NULL;

       m_queryLogType = queryLogType;
        
		CCMSApp* pApp = (CCMSApp*)AfxGetApp();
		if (pApp)
		{

			CUdfLogFrame*  pLogFrame = (CUdfLogFrame*)GetFrameWnd();


			CString strFormCaption ="";
			if (!CUdfLogSystem::GetLogTypeName(_nType).IsEmpty())
			{
				strFormCaption = CUdfLogSystem::GetLogTypeName(_nType);
			}
			else
			{
				strFormCaption = "��־ϵͳ";
			}			
			SetCaption(strFormCaption);

			CString strStatusTxt = "";
			if (CUdfLogSystem::GetLogTypeName(_nType) == "")
				return FALSE;


			strStatusTxt.Format("���ڶ�ȡ[%s]����־��Ϣ, ��ȴ�... ",CUdfLogSystem::GetLogTypeName(_nType));

			if (pLogFrame)
				pLogFrame->SetStatusBarText(0,strStatusTxt);


			int nLogCount = pApp->m_pLogSystem->QueryLogInformation(_nType,queryLogType,_beginTime,_endTime);

			if (nLogCount < 5000)
				strStatusTxt.Format("%s , %d ����־��Ϣ",CUdfLogSystem::GetLogTypeName(_nType),nLogCount);
			else
				strStatusTxt.Format("%s , ���� %d ����־��Ϣ",CUdfLogSystem::GetLogTypeName(_nType),nLogCount);

			if (pLogFrame)
				pLogFrame->SetStatusBarText(0,strStatusTxt);
		}
	}

	return TRUE;
}

void CUdfLogRightView::OnLClickLogMessage(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pItem = ( LPNMITEMACTIVATE )pNMHDR;
	//InvalidateListCtrl();
}




void CUdfLogRightView::OnDBClickLogMessage(NMHDR* pNMHDR, LRESULT* pResult)
{
	//InvalidateListCtrl();

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	CMainFrame*  pMainFrame = (CMainFrame*)pApp->m_pMainWnd;
	LPNMITEMACTIVATE pItem = ( LPNMITEMACTIVATE )pNMHDR;
	if (pItem->iItem != -1)
	{
		CString _strLevel = "",
			    _strDate = "",
				_strFrom = "",
				_strLogMessage = "";
		if (m_pListCtrl.GetSafeHwnd())
		{
			_strLevel	= m_pListCtrl.GetItemText(pItem->iItem,1);
			_strDate	= m_pListCtrl.GetItemText(pItem->iItem,2);
			_strFrom	= m_pListCtrl.GetItemText(pItem->iItem,3);
			_strLogMessage = m_pListCtrl.GetItemText(pItem->iItem,4);

			if (pMainFrame->m_pShowDetailLogDlg && pMainFrame->m_pShowDetailLogDlg->GetSafeHwnd())
			{
				pMainFrame->m_pShowDetailLogDlg->m_strDatetime = _strDate;
				pMainFrame->m_pShowDetailLogDlg->m_strLogLevel = _strLevel;
				pMainFrame->m_pShowDetailLogDlg->m_strLogMessage = _strLogMessage;
				pMainFrame->m_pShowDetailLogDlg->m_strLogType = _strFrom;
				pMainFrame->m_pShowDetailLogDlg->Show();
			}

		}
	}
}



void CUdfLogRightView::OnRButtonClickLogMessage(NMHDR* pNMHDR, LRESULT* pResult)
{
	//InvalidateListCtrl();
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	CMainFrame* pMainFrame = (CMainFrame*)pApp->m_pMainWnd;
	LPNMITEMACTIVATE pItem = ( LPNMITEMACTIVATE )pNMHDR;

	POINT point;
	GetCursorPos(&point);
	CMenu menu;
	menu.LoadMenu(IDR_MENU_POP);

	CMenu* popMenu= menu.GetSubMenu(1);

	if (popMenu && popMenu->GetSafeHmenu())
		popMenu->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);


}



void CUdfLogRightView::OnCmdLogQuery()
{
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	CMainFrame* pMainFrame  = (CMainFrame*)pApp->m_pMainWnd;


   CUdfLogQueryDlg dlg(this);
   if (dlg.DoModal() == IDOK)
   {
	   m_queryLogType = dlg.m_queryLogType;
	   memcpy(&m_queryBeginTime,&dlg.m_timeBegin,sizeof(SYSTEMTIME));
	   memcpy(&m_queryEndTime,&dlg.m_timeEnd,sizeof(SYSTEMTIME));

	   TRACE("��ѯʱ��:%s --> %s\n",CSystemTimeOp::ConvertSystemTimeToString(&m_queryBeginTime), CSystemTimeOp::ConvertSystemTimeToString(&m_queryEndTime));

	   ShowLogInformation(pMainFrame->m_wndLogFrame.m_pLeftView->n_CurentType,
		                  m_queryLogType,
						  &m_queryBeginTime,
						  &m_queryEndTime);

   }
}

void CUdfLogRightView::OnCmdLogDelete()
{
   //

}

//����������Ϣ
void CUdfLogRightView::OnCmdLogExport()
{
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	CMainFrame* pMainFrame= (CMainFrame*)pApp->m_pMainWnd;

   if (m_pListCtrl.GetSafeHwnd()  && m_pListCtrl.GetItemCount() > 0)
   {
	   CFileDialog fileDlg(FALSE, _T("txt"), _T("��־��Ϣ"),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Text Files (*.txt)|*.txt|Excel CSV��ʽ�ļ� (*.csv)|*.csv|All Files (*.*)|*.*||"),this);
	   if (fileDlg.DoModal() == IDOK)
	   {
		   CString sFilename = fileDlg.GetPathName();
		   
		   try
		   {
			   CStdioFile file(sFilename,CFile::modeCreate|CFile::modeWrite);

			   int nItemCount = m_pListCtrl.GetItemCount();
			   for (int idx = 0; idx < nItemCount; idx ++)
			   {
				   CString strNo="",strType="",strDatetime="", strFrom = "", strMessages = "";
				   //strNo		= m_pListCtrl.GetItemText(idx,0);
				   strType		= m_pListCtrl.GetItemText(idx,1);
				   strDatetime	= m_pListCtrl.GetItemText(idx,2);
				   strFrom		= m_pListCtrl.GetItemText(idx,3);
				   strMessages	= m_pListCtrl.GetItemText(idx,4);


				   strMessages.Replace(",",";");
				   strMessages.Replace("\r\n"," ");
				   strMessages.Replace("\n","");
				   strMessages.Replace("\"",",");


				   CString strWrite ="";
				   //strWrite.Format("%s,%s,%s,%s,%s\n",
					  // strNo,
					  // strType,
					  // strDatetime,
					  // strFrom,
					  // strMessages);

				   strWrite.Format("%d,%s,%s,%s,%s\n",
					   idx + 1,
					   strType,
					   strDatetime,
					   strFrom,
					   strMessages);

				   file.WriteString(strWrite);

			   }
			   file.Flush();

			   m_pListCtrl.Invalidate();
		   }
		   catch (CFileException* e)
		   {
			   e->ReportError(MB_OK);
			   e->Delete();
			   return;
		   }
	   }
   }
   Invalidate(TRUE);
}


void CUdfLogRightView::OnCmdLogDeleteAll()
{
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	CMainFrame* pMainFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;

	if (pApp->m_pLogSystem  && 
		pMainFrame->m_wndLogFrame.GetSafeHwnd() && 
		pMainFrame->m_wndLogFrame.m_pLeftView->GetSafeHwnd())
	{
		int ret = MessageBox("ȷ�����������־��Ϣ?","��ʾ",MB_OKCANCEL|MB_ICONQUESTION);
		if (ret == IDCANCEL)
			return;
	
		pApp->m_pLogSystem->DeleteLogByHand(pMainFrame->m_wndLogFrame.m_pLeftView->n_CurentType);


		//ShowLogInformation(pMainFrame->m_wndLogFrame.m_pLeftView->n_CurentType,
		//	CUdfLogQueryDlg::QUERY_LOGTYPE_ALL,NULL,NULL);

	   m_pListCtrl.DeleteAllItems();
	   m_pListCtrl.Invalidate(TRUE);
	}

	//Invalidate(TRUE);
}

void CUdfLogRightView::OnCmdLogDelete1MonthPre()
{
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	CMainFrame* pMainFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;

	if (pApp->m_pLogSystem  && 
		pMainFrame->m_wndLogFrame.GetSafeHwnd() && 
		pMainFrame->m_wndLogFrame.m_pLeftView->GetSafeHwnd())
	{
		int ret = MessageBox("ȷ�����һ����ǰ��������־��Ϣ?","��ʾ",MB_OKCANCEL|MB_ICONQUESTION);
		if (ret == IDCANCEL)
			return;

		SYSTEMTIME st;
		GetLocalTime(&st);

		long TotalMonths = st.wYear * 12 + st.wMonth;

		long _logEndMonth = TotalMonths ;

		CString strDelEndTime = "";
		if (_logEndMonth % 12 == 0 )
			strDelEndTime.Format("%04d-12-01 00:00:00",long(_logEndMonth /12 )-1);
		else
			strDelEndTime.Format("%04d-%02d-01 00:00:00",long(_logEndMonth /12 ),long(_logEndMonth % 12));

		pApp->m_pLogSystem->DeleteLogByHand(pMainFrame->m_wndLogFrame.m_pLeftView->n_CurentType,"",strDelEndTime);

		ShowLogInformation(pMainFrame->m_wndLogFrame.m_pLeftView->n_CurentType,
			               CUdfLogQueryDlg::QUERY_LOGTYPE_ALL,NULL,NULL);

	}

	Invalidate(TRUE);
}

void CUdfLogRightView::OnCmdLogDelete2MonthPre()
{
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	CMainFrame* pMainFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;

	if (pApp->m_pLogSystem  && 
		pMainFrame->m_wndLogFrame.GetSafeHwnd() && 
		pMainFrame->m_wndLogFrame.m_pLeftView->GetSafeHwnd())
	{
		int ret = MessageBox("ȷ�����������ǰ��������־��Ϣ?","��ʾ",MB_OKCANCEL|MB_ICONQUESTION);
		if(ret == IDCANCEL)
			return;

		SYSTEMTIME st;
		GetLocalTime(&st);

		long TotalMonths = st.wYear * 12 + st.wMonth;

		long _logEndMonth = TotalMonths - 1 ;

		CString strDelEndTime = "";
		if (_logEndMonth % 12 == 0)
			strDelEndTime.Format("%04d-12-01 00:00:00",long(_logEndMonth /12 )-1);
		else
			strDelEndTime.Format("%04d-%02d-01 00:00:00",long(_logEndMonth /12 ),long(_logEndMonth % 12));


		pApp->m_pLogSystem->DeleteLogByHand(pMainFrame->m_wndLogFrame.m_pLeftView->n_CurentType,"",strDelEndTime);

		ShowLogInformation(pMainFrame->m_wndLogFrame.m_pLeftView->n_CurentType,
			CUdfLogQueryDlg::QUERY_LOGTYPE_ALL,NULL,NULL);
	}
	Invalidate(TRUE);
}

void CUdfLogRightView::OnCmdLogDeleteCustom()
{
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	CMainFrame* pMainFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;

	if (pApp->m_pLogSystem  && 
		pMainFrame->m_wndLogFrame.GetSafeHwnd() && 
		pMainFrame->m_wndLogFrame.m_pLeftView->GetSafeHwnd())
	{

		ShowLogInformation(pMainFrame->m_wndLogFrame.m_pLeftView->n_CurentType,
			CUdfLogQueryDlg::QUERY_LOGTYPE_ALL,NULL,NULL);
	}
}

void CUdfLogRightView::OnCmdLogRefresh()
{
	CUdfLogFrame* pLogFrame = (CUdfLogFrame*)GetFrameWnd();

	if (pLogFrame->GetSafeHwnd())
	{
		ShowLogInformation(pLogFrame->m_pLeftView->n_CurentType,
			CUdfLogQueryDlg::QUERY_LOGTYPE_ALL,NULL,NULL);
	}
}

void  CUdfLogRightView::SetFrameWnd(CWnd* pFrameWnd)
{
	m_pFrameWnd = pFrameWnd;
}

CWnd* CUdfLogRightView::GetFrameWnd()
{
	return m_pFrameWnd;
}


void CUdfLogRightView::SetCaption(CString strCaption)
{
	if (m_wndCaption.GetSafeHwnd())
		m_wndCaption.SetWindowText(strCaption);
}


BOOL CUdfLogRightView::InvalidateListCtrl()
{
	//Invalidate(TRUE);
	if (m_pListCtrl.GetSafeHwnd())
		m_pListCtrl.Invalidate(TRUE);

	return TRUE;
}