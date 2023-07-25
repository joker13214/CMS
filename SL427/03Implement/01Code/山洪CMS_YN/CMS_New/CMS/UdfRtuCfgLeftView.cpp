// UdfRtuCfgLeftView.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "UdfRtuCfgLeftView.h"
#include "resource.h"
#include "UdfRtuCfgFrame.h"
#include "UdfAddRtuStDlg.h"
#include "UdfRtuCfgRightView.h"


// CUdfRtuCfgLeftView

IMPLEMENT_DYNCREATE(CUdfRtuCfgLeftView, CView)
IMPLEMENT_MENUXP(CUdfRtuCfgLeftView, CView)

CUdfRtuCfgLeftView::CUdfRtuCfgLeftView()
{
	m_pTreeItemRoot = NULL;
	m_pRtuCfgFrame = NULL;
	m_pSelectItem = NULL;
	m_pCurStation = NULL;
}

CUdfRtuCfgLeftView::~CUdfRtuCfgLeftView()
{
}

BEGIN_MESSAGE_MAP(CUdfRtuCfgLeftView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_MENUXP_MESSAGES()
	ON_NOTIFY(NM_CLICK,IDC_RTUCFG_TREECTRL,&CUdfRtuCfgLeftView::OnRtuTreeClick)
	ON_NOTIFY(NM_RCLICK,IDC_RTUCFG_TREECTRL,&CUdfRtuCfgLeftView::OnRtuTreeRClick)
	ON_COMMAND(IDM_RTUCFG_RTUSTADD,&CUdfRtuCfgLeftView::OnRtuCfgAddRTUSt)
	ON_COMMAND(IDM_RTUCFG_YCADD,&CUdfRtuCfgLeftView::OnRtuCfgYcAdd)
	ON_COMMAND(IDM_RTUCFG_YXADD,&CUdfRtuCfgLeftView::OnRtuCfgYxAdd)
	ON_COMMAND(IDM_RTUCFG_YKADD,&CUdfRtuCfgLeftView::OnRtuCfgYkAdd)
	ON_COMMAND(IDM_RTUCFG_ZDADD,&CUdfRtuCfgLeftView::OnRtuCfgZdAdd)
	ON_COMMAND(IDM_RTUCFG_DELRTUST,&CUdfRtuCfgLeftView::OnRtuCfgDelRtuSt)
	ON_COMMAND(IDM_RTUCFG_RTUSTPRO,&CUdfRtuCfgLeftView::OnRtuCfgRtuStPro)
END_MESSAGE_MAP()


// CUdfRtuCfgLeftView 绘图

void CUdfRtuCfgLeftView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
}


// CUdfRtuCfgLeftView 诊断

#ifdef _DEBUG
void CUdfRtuCfgLeftView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CUdfRtuCfgLeftView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CUdfRtuCfgLeftView 消息处理程序

int CUdfRtuCfgLeftView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CMenuXP::SetXPLookNFeel(this);

	// TODO:  在此添加您专用的创建代码
	if (!m_wndTreeCtrl.Create(TVS_HASBUTTONS|/*TVS_HASLINES|*/TVS_LINESATROOT|WS_CHILD|WS_VISIBLE|TVS_SHOWSELALWAYS,CRect(0,0,10,10),this,IDC_RTUCFG_TREECTRL))
	{
		TRACE("Create RtuCfg Tree Ctrl Error!\r\n");
		return -1;
	}

	if (!m_wndImageList.Create(16,16,ILC_MASK|ILC_COLOR32,3,3))
	{
		TRACE("Create RtuCfg imagelist ctrl error!\r\n");
		return -1;
	}

	HICON icon[3];
	icon[0] = AfxGetApp()->LoadIcon(IDI_ICON_RTUROOT);
	icon[1] = AfxGetApp()->LoadIcon(IDI_ICON_RTUST);
	icon[2] = AfxGetApp()->LoadIcon(IDI_ICON_RTUPRO);

	for (int i= 0; i < 3; i++)
	{
		m_wndImageList.Add(icon[i]);
	}

	m_wndImageList.SetBkColor(RGB(255,255,255));

	m_wndTreeCtrl.SetImageList(&m_wndImageList,TVSIL_NORMAL);

	DWORD dwStyles=GetWindowLong(m_wndTreeCtrl.m_hWnd,GWL_STYLE);	//获取树控制原风格

	dwStyles|= TVS_HASBUTTONS/*|TVS_HASLINES|TVS_LINESATROOT*/;

	SetWindowLong(m_wndTreeCtrl.m_hWnd,GWL_STYLE,dwStyles);			//设置风格

	InitTreeCtrlItem();

	return 0;
}


void CUdfRtuCfgLeftView::SetRtuCfgFrame(CWnd* pFrame)
{
	m_pRtuCfgFrame = pFrame;
}

CWnd* CUdfRtuCfgLeftView::GetRtuCfgFrame()
{
	return m_pRtuCfgFrame;
}

void CUdfRtuCfgLeftView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_wndTreeCtrl.GetSafeHwnd())
		m_wndTreeCtrl.MoveWindow(0,0,cx,cy);

}


bool CUdfRtuCfgLeftView::InitTreeCtrlItem()
{
	m_wndTreeCtrl.DeleteAllItems();

	TVINSERTSTRUCT TCItem;//定义插入项数据结构

	TCItem.hParent=NULL;												//设置父项句柄
	TCItem.hInsertAfter=TVI_LAST;										//在最后增加
	TCItem.item.mask=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;//设屏蔽
	TCItem.item.pszText="遥测终端信息";
	TCItem.item.iImage=0;
	TCItem.item.iSelectedImage=0;
	TCItem.item.lParam = NULL;


	HTREEITEM pRoot = m_wndTreeCtrl.InsertItem(&TCItem);

	m_pTreeItemRoot = pRoot;


   CCMSApp* pApp = (CCMSApp*)AfxGetApp();
   CPtrList* pList = pApp->m_pReadDB->GetRtuStList();
   if (pList  && !pList->IsEmpty())
   {
	   POSITION rPos = pList->GetHeadPosition();
	   while (rPos != NULL)
	   {
		   CRtuStation* pStation  = (CRtuStation*)pList->GetNext(rPos);
		   if (pStation)
			   AddStTreeItem(m_pTreeItemRoot,pStation);
	   }
   }

    m_wndTreeCtrl.Expand(m_pTreeItemRoot,TVE_EXPAND);

	return true;
}



void CUdfRtuCfgLeftView::OnRtuTreeClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	if (!pNMTreeView)
		return;

	UINT   uFlags;  
	CPoint ptAction;  
	::GetCursorPos(   &ptAction   );  
	m_wndTreeCtrl.ScreenToClient(   &ptAction   );  

	HTREEITEM   item = m_wndTreeCtrl.HitTest(   ptAction,   &uFlags   );  

	if (item != NULL && (TVHT_ONITEM & uFlags))
	{
		m_wndTreeCtrl.SelectItem(item);

		m_pSelectItem = item;

		CUdfRtuCfgFrame* pRtuCfgFrame = (CUdfRtuCfgFrame*)GetRtuCfgFrame();
		CString strCaption = "";

		if (item != NULL && item == m_pTreeItemRoot)
		{
			pRtuCfgFrame->m_pRtuCfgRightView->ShowRtuInfor(RTUST_STINFO_TYPE,NULL);
			m_pCurStation = NULL;
		}
		else if (item != NULL && item != m_pTreeItemRoot  && m_wndTreeCtrl.GetParentItem(item) == m_pTreeItemRoot) //站
		{
			/*
			CRtuStation* pStation = (CRtuStation*)m_wndTreeCtrl.GetItemData(item);

			if (pStation)
			{
				pRtuCfgFrame->m_pRtuCfgRightView->ShowRtuInfor(RTUST_STINFO_TYPE,pStation);
				m_pCurStation = pStation;

			}
			*/
		}
		else if (item != NULL )  //遥测、遥信、遥控、定值信息
		{

			CString strTitle= "";
			strTitle =m_wndTreeCtrl.GetItemText(item) ;
			strTitle.Trim();

			HTREEITEM pParentItem = m_wndTreeCtrl.GetParentItem(item);
			if (!pParentItem)
				return;


			CRtuStation* pStation = NULL;
			pStation = (CRtuStation*)m_wndTreeCtrl.GetItemData(pParentItem);
			if (!pStation)
				return ;

			m_pCurStation = pStation;

			
			if (strTitle == "遥测")
			{
				pRtuCfgFrame->m_pRtuCfgRightView->ShowRtuInfor(RTUST_YCINFO_TYPE,pStation);
			}
			else if (strTitle == "遥信")
			{
				pRtuCfgFrame->m_pRtuCfgRightView->ShowRtuInfor(RTUST_YXINFO_TYPE,pStation);
			}
			else if (strTitle == "遥控")
			{
				pRtuCfgFrame->m_pRtuCfgRightView->ShowRtuInfor(RTUST_YKINFO_TYPE,pStation);
			}
			else if (strTitle == "定值")
			{
				pRtuCfgFrame->m_pRtuCfgRightView->ShowRtuInfor(RTUST_ZDINFO_TYPE,pStation);
			}
		}
	}
	else
	{
		m_pCurStation = NULL;
	}
}


void CUdfRtuCfgLeftView::OnRtuTreeRClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	if (!pNMTreeView)
		return;
	
	UINT   uFlags = TVHT_ONITEM;  
	CPoint ptAction;  
	::GetCursorPos(   &ptAction   );  
	m_wndTreeCtrl.ScreenToClient(   &ptAction   );  

	HTREEITEM   item = m_wndTreeCtrl.HitTest(ptAction,   &uFlags   );  

	//  [12/10/2010 ]
	//  重点，设置选择项
	if (item != NULL)
		m_wndTreeCtrl.SelectItem(item);            
    
	m_pSelectItem = item;

     ClientToScreen(&ptAction);
	if (item != NULL && item == m_pTreeItemRoot)     //root
	{
		CMenu menu;
		menu.LoadMenu(IDR_MENU_POP);

		CMenu* popMenu = menu.GetSubMenu(2);
		popMenu->TrackPopupMenu(TPM_LEFTALIGN,ptAction.x,ptAction.y,this);
	}
	/*
	else if (item != NULL && item != m_pTreeItemRoot  && m_wndTreeCtrl.GetParentItem(item) == m_pTreeItemRoot)  //站
	{
		CRtuStation* pStation = (CRtuStation*)m_wndTreeCtrl.GetItemData(item);

		if (pStation)
			m_pCurStation = pStation;
		CMenu menu;
		menu.LoadMenu(IDR_MENU_POP);

		CMenu* popMeun = menu.GetSubMenu(3);
		popMeun->TrackPopupMenu(TPM_LEFTALIGN,ptAction.x,ptAction.y,this);
	}
	*/

}


void CUdfRtuCfgLeftView::OnRtuCfgAddRTUSt()
{
	CUdfRtuCfgFrame* pFrame = (CUdfRtuCfgFrame*)GetRtuCfgFrame();
	pFrame->m_pRtuCfgRightView->AddRtuObject();
}

bool CUdfRtuCfgLeftView::AddStTreeItem(HTREEITEM pParent,CRtuStation* pStation)
{

	if (!pStation)  return false;
	
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();



	TVINSERTSTRUCT TCItem;//定义插入项数据结构

	TCItem.hParent=pParent;												//设置父项句柄
	TCItem.hInsertAfter=TVI_LAST;										//在最后增加
	TCItem.item.mask=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;//设屏蔽
	//strcpy(TCItem.item.pszText,pStation->GetName());
	TCItem.item.pszText = (LPSTR)(LPCTSTR)pStation->GetName();

	TCItem.item.iImage=1;
	TCItem.item.iSelectedImage=1;
	TCItem.item.lParam = NULL;


	HTREEITEM pRoot = m_wndTreeCtrl.InsertItem(&TCItem);
	m_wndTreeCtrl.SetItemData(pRoot,(DWORD_PTR)pStation);


	if (pApp->m_GlobalOptions.m_RtuOpts.blYcList_FLAG != FALSE)
	{
		TCItem.hParent = pRoot;
		TCItem.item.iImage = 2;
		TCItem.item.iSelectedImage = 2;
		TCItem.item.pszText = "遥测";
		TCItem.item.lParam = NULL;
		m_wndTreeCtrl.InsertItem(&TCItem);
	}

	if (pApp->m_GlobalOptions.m_RtuOpts.blYxList_FLAG != FALSE)
	{
		TCItem.hParent = pRoot;
		TCItem.item.iImage = 2;
		TCItem.item.iSelectedImage = 2;
		TCItem.item.pszText = "遥信";
		TCItem.item.lParam = NULL;
		m_wndTreeCtrl.InsertItem(&TCItem);
	}


	if (pApp->m_GlobalOptions.m_RtuOpts.blYkList_FLAG != FALSE)
	{
		TCItem.hParent = pRoot;
		TCItem.item.iImage = 2;
		TCItem.item.iSelectedImage = 2;
		TCItem.item.pszText = "遥控";
		TCItem.item.lParam = NULL;
		m_wndTreeCtrl.InsertItem(&TCItem);
	}


	if (pApp->m_GlobalOptions.m_RtuOpts.blZdList_FLAG != FALSE)
	{
		TCItem.hParent = pRoot;
		TCItem.item.iImage = 2;
		TCItem.item.iSelectedImage = 2;
		TCItem.item.pszText = "定值";
		TCItem.item.lParam = NULL;
		m_wndTreeCtrl.InsertItem(&TCItem);
	}


	return true;
}


void CUdfRtuCfgLeftView::OnRtuCfgYcAdd()
{
	CUdfRtuCfgFrame* pFrame =(CUdfRtuCfgFrame*)GetRtuCfgFrame();
	pFrame->m_pRtuCfgRightView->AddYcObject(m_pCurStation);
}

void CUdfRtuCfgLeftView::OnRtuCfgYxAdd()
{
	CUdfRtuCfgFrame* pFrame =(CUdfRtuCfgFrame*)GetRtuCfgFrame();
	pFrame->m_pRtuCfgRightView->AddYxObject(m_pCurStation);
}

void CUdfRtuCfgLeftView::OnRtuCfgYkAdd()
{
	CUdfRtuCfgFrame* pFrame =(CUdfRtuCfgFrame*)GetRtuCfgFrame();
	pFrame->m_pRtuCfgRightView->AddYkObject(m_pCurStation);
}

void CUdfRtuCfgLeftView::OnRtuCfgZdAdd()
{
	CUdfRtuCfgFrame* pFrame =(CUdfRtuCfgFrame*)GetRtuCfgFrame();
	pFrame->m_pRtuCfgRightView->AddZdObject(m_pCurStation);
}

void CUdfRtuCfgLeftView::OnRtuCfgDelRtuSt()
{
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	if (m_pSelectItem != NULL && m_pSelectItem != m_pTreeItemRoot && m_wndTreeCtrl.GetParentItem(m_pSelectItem) == m_pTreeItemRoot)
	{
		CRtuStation* pStation = (CRtuStation*)m_wndTreeCtrl.GetItemData(m_pSelectItem);
		if (pStation)
		{
			CUdfRtuCfgFrame* pFrame= (CUdfRtuCfgFrame*)GetRtuCfgFrame();
			pFrame->m_pRtuCfgRightView->DeleteRtuObject(pStation);
		}
	}
}

void CUdfRtuCfgLeftView::OnRtuCfgRtuStPro()
{

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();

	if (m_pSelectItem != NULL  && m_pSelectItem != m_pTreeItemRoot && m_wndTreeCtrl.GetParentItem(m_pSelectItem) == m_pTreeItemRoot)
	{
		CRtuStation* pStation = (CRtuStation*)m_wndTreeCtrl.GetItemData(m_pSelectItem);
		if (pStation)
		{
			CUdfRtuCfgFrame* pFrame= (CUdfRtuCfgFrame*)GetRtuCfgFrame();
			pFrame->m_pRtuCfgRightView->UpdateRtuObject(pStation);   
		}
	}
}


bool CUdfRtuCfgLeftView::blRtuStTreeItem(HTREEITEM pItem)
{
	if (!pItem)
		return false;

	if (pItem == m_pTreeItemRoot)
		return false;

	if (m_wndTreeCtrl.GetParentItem(pItem) == m_pTreeItemRoot)
		return true;


	return false;
}

bool CUdfRtuCfgLeftView::blSelectItemRtuSt()
{
	if (!m_pSelectItem)
		return false;

	if (m_pSelectItem == m_pTreeItemRoot)
		return false;

	if (m_wndTreeCtrl.GetParentItem(m_pSelectItem) == m_pTreeItemRoot)
		return true;

	return false;
}