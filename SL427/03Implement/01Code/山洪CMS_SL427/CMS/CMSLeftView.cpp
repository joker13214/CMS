// CMSLeftView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CMS.h"
#include "CMSLeftView.h"
#include "MainFrm.h"


// CCMSLeftView

IMPLEMENT_DYNCREATE(CCMSLeftView, CView)

CCMSLeftView::CCMSLeftView()
{

}

CCMSLeftView::~CCMSLeftView()
{
}

BEGIN_MESSAGE_MAP(CCMSLeftView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY(NM_CLICK,IDC_MAIN_TREECTRL,&CCMSLeftView::OnLeftClickTree)
	ON_NOTIFY(NM_RCLICK,IDC_MAIN_TREECTRL,&CCMSLeftView::OnRightClickTree)
END_MESSAGE_MAP()


// CCMSLeftView ��ͼ

void CCMSLeftView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: �ڴ���ӻ��ƴ���
}


// CCMSLeftView ���

#ifdef _DEBUG
void CCMSLeftView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CCMSLeftView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCMSLeftView ��Ϣ�������

int CCMSLeftView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndBackGroud.Create("",WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this))
	{
		return -1;
	}

	if (!m_wndCaption.Create("����",WS_VISIBLE| WS_CHILD,CRect(0,0,0,0),this))
	{
		return -1;
	}
	if (!m_wndTreeCtrl.Create(TVS_HASBUTTONS|/*TVS_HASLINES|*/TVS_LINESATROOT|WS_CHILD|WS_VISIBLE|TVS_SHOWSELALWAYS|WS_CLIPCHILDREN ,
		CRect(0,0,10,10),this,IDC_MAIN_TREECTRL))
	{
		return -1;
	}

	CFont* pFont =  m_wndTreeCtrl.GetFont();
	m_wndCaption.SetFont(pFont);


	m_wndImageList.Create(16,16,ILC_MASK | ILC_COLOR32,2,2);

	HICON hicon[2];


	hicon[0] = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_RTUROOT));
	hicon[1] = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_RTUST));
	for (int idx = 0;idx < 2; idx++)
	{
		m_wndImageList.Add(hicon[idx]);
	}

	m_wndImageList.SetBkColor(RGB(255,255,255));
	m_wndTreeCtrl.SetImageList(&m_wndImageList,TVSIL_NORMAL);



	return 0;
}

void CCMSLeftView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (m_wndBackGroud.GetSafeHwnd())
		m_wndBackGroud.MoveWindow(0,0,cx,cy);
	if (m_wndCaption.GetSafeHwnd())
		m_wndCaption.MoveWindow(4,4,cx-4,16);
	if (m_wndTreeCtrl.GetSafeHwnd())
		m_wndTreeCtrl.MoveWindow(0,20,cx,cy-20);
}


BOOL CCMSLeftView::InitTreeItem()
{

	if (!m_wndTreeCtrl.GetSafeHwnd())
		return FALSE;

	m_wndTreeCtrl.DeleteAllItems();

	HTREEITEM pTItem = NULL;

	TVINSERTSTRUCT TCItem;//������������ݽṹ

	TCItem.hParent=NULL;												//���ø�����
	TCItem.hInsertAfter=TVI_LAST;										//���������
	TCItem.item.mask=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;//������
	TCItem.item.pszText="CMS";
	TCItem.item.iImage=0;
	TCItem.item.iSelectedImage=0;
	TCItem.item.lParam = NULL;

	HTREEITEM tempItem = NULL;  
	HTREEITEM toolRoot = NULL,OptRoot = NULL,OtherRoot= NULL;
	HTREEITEM HRoot = m_wndTreeCtrl.InsertItem(&TCItem);


	TCItem.item.iImage=1;
	TCItem.item.iSelectedImage=1;
	TCItem.item.pszText = "����";
	TCItem.hParent = HRoot ;
	toolRoot =m_wndTreeCtrl.InsertItem(&TCItem);


	TCItem.item.iImage=1;
	TCItem.item.iSelectedImage=1;
	TCItem.item.pszText = "ң���ն���Ϣ����";
	TCItem.hParent = toolRoot ;
	tempItem =m_wndTreeCtrl.InsertItem(&TCItem);

	TCItem.item.iImage=1;
	TCItem.item.iSelectedImage=1;
	TCItem.item.pszText = "��־ϵͳ";
	TCItem.hParent = toolRoot ;
	tempItem =m_wndTreeCtrl.InsertItem(&TCItem);

	TCItem.item.iImage = 1;
	TCItem.item.iSelectedImage = 1;
	TCItem.item.pszText = "Commserver���ù���";
	TCItem.hParent = toolRoot;
	tempItem = m_wndTreeCtrl.InsertItem(&TCItem);

	TCItem.item.iImage = 1;
	TCItem.item.iSelectedImage = 1;
	TCItem.item.pszText = "�û�ϵͳ";
	TCItem.hParent = toolRoot;
	tempItem = m_wndTreeCtrl.InsertItem(&TCItem);
	m_wndTreeCtrl.Expand(toolRoot,TVE_EXPAND);

	TCItem.item.iImage=1;
	TCItem.item.iSelectedImage=1;
	TCItem.item.pszText = "ѡ��";
	TCItem.hParent = HRoot ;
	OptRoot =m_wndTreeCtrl.InsertItem(&TCItem);

	TCItem.item.iImage=1;
	TCItem.item.iSelectedImage=1;
	TCItem.item.pszText = "���ݿ����";
	TCItem.hParent = OptRoot ;
	tempItem =m_wndTreeCtrl.InsertItem(&TCItem);


	TCItem.item.iImage=1;
	TCItem.item.iSelectedImage=1;
	TCItem.item.pszText = "��־ϵͳ����";
	TCItem.hParent = OptRoot ;
	tempItem =m_wndTreeCtrl.InsertItem(&TCItem);
	//m_wndTreeCtrl.Expand(OptRoot,TVE_EXPAND);

	TCItem.item.iImage=1;
	TCItem.item.iSelectedImage=1;
	TCItem.item.pszText = "����";
	TCItem.hParent = HRoot ;
	OtherRoot =m_wndTreeCtrl.InsertItem(&TCItem);

	TCItem.item.iImage = 1;
	TCItem.item.iSelectedImage = 1;
	TCItem.item.pszText = "����ʵʱ�����Ϣ";
	TCItem.hParent = OtherRoot;
	tempItem = m_wndTreeCtrl.InsertItem(&TCItem);
	
	TCItem.item.iImage=1;
	TCItem.item.iSelectedImage=1;
	TCItem.item.pszText = "�˳�";
	TCItem.hParent = HRoot ;
	tempItem =m_wndTreeCtrl.InsertItem(&TCItem);


	m_wndTreeCtrl.Expand(HRoot,TVE_EXPAND);

	return TRUE;
}


void CCMSLeftView::OnLeftClickTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	RedrawTreeCtrl();

	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	if (!pNMTreeView)
		return;

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	CMainFrame* pMainFrame = (CMainFrame*)pApp->m_pMainWnd;
	if(!pMainFrame  || !pMainFrame->GetSafeHwnd())	return;


	UINT   uFlags = TVHT_ONITEM;  
	CPoint ptAction;  
	::GetCursorPos(   &ptAction   );  
	m_wndTreeCtrl.ScreenToClient(   &ptAction   );  

	HTREEITEM   item = m_wndTreeCtrl.HitTest(   ptAction,   &uFlags   );  

	m_wndTreeCtrl.SelectItem(item);
	if (item != NULL && (TVHT_ONITEM & uFlags))
	{
		CString strText = "";
		strText = m_wndTreeCtrl.GetItemText(item);

		if (strText == "�˳�")
		{
			//pMainFrame->ShowOtherFrame(SW_HIDE);
			pMainFrame->OnClose();
		}
		else if (strText == "ң���ն���Ϣ����")
			pMainFrame->SendMessage(WM_COMMAND,IDM_RTUOPTIONS_CFG,NULL);
		else if (strText == "��־ϵͳ")
			pMainFrame->SendMessage(WM_COMMAND,IDM_LOG_SYSTEM,NULL);
		else if (strText == "Commserver���ù���")
			pMainFrame->SendMessage(WM_COMMAND,IDM_COMMSERVERCFG,NULL);
		else if (strText == "����ʵʱ�����Ϣ")
			pMainFrame->SendMessage(WM_COMMAND,IDM_DATAINDB,NULL);
		else if (strText == "���ݿ����")
			pMainFrame->SendMessage(WM_COMMAND,IDM_DBOPT_CFG,NULL);
		else if (strText == "��־ϵͳ����")
			pMainFrame->SendMessage(WM_COMMAND,IDM_LOGOPT_CFG,NULL);
		else if (strText == "�û�ϵͳ")
			pMainFrame->SendMessage(WM_COMMAND,IDM_USERSSYSTEM,NULL);
	}
}

void CCMSLeftView::OnRightClickTree(NMHDR* pNMHDR,LRESULT* pResult)
{
	RedrawTreeCtrl();
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	if (!pNMTreeView)
		return;

	UINT   uFlags= TVHT_ONITEM;  
	CPoint ptAction;  
	::GetCursorPos(   &ptAction   );  
	m_wndTreeCtrl.ScreenToClient(   &ptAction   );  

	HTREEITEM   item = m_wndTreeCtrl.HitTest(   ptAction,   &uFlags   );  

	m_wndTreeCtrl.SelectItem(item);
}


BOOL CCMSLeftView::RedrawTreeCtrl()
{
	CRect rc;
	m_wndTreeCtrl.GetClientRect(&rc);
	m_wndTreeCtrl.InvalidateRect(&rc);

	return TRUE;
}