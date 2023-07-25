// UdfLogLeftView.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "UdfLogLeftView.h"
#include "resource.h"
#include "MainFrm.h"
#include "UdfLogSystem.h"


// CUdfLogLeftView

IMPLEMENT_DYNCREATE(CUdfLogLeftView, CView)
IMPLEMENT_MENUXP(CUdfLogLeftView, CView);

CUdfLogLeftView::CUdfLogLeftView()
{
	//m_pTreeCtrl = NULL;
	m_pImageList = NULL;
	n_CurentType = SYS_RUNNING_S_LOG;
	m_pFrameWnd  =  NULL;

	EnableAutomation();
}

CUdfLogLeftView::~CUdfLogLeftView()
{


}

void CUdfLogLeftView::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。基类将自动
	// 删除该对象。在调用该基类之前，请添加您的
	// 对象所需的附加清除代码。

	CView::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CUdfLogLeftView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_MENUXP_MESSAGES()
	ON_NOTIFY(NM_CLICK,IDC_LOG_TREECTRL,CUdfLogLeftView::OnClkTreeCtrl)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CUdfLogLeftView, CView)
END_DISPATCH_MAP()

// 注意: 我们添加 IID_IUdfLogLeftView 支持
//  以支持来自 VBA 的类型安全绑定。此 IID 必须同附加到 .IDL 文件中的
//  调度接口的 GUID 匹配。

// {B661C2A0-1C70-4579-A39A-F39D294F4D41}
static const IID IID_IUdfLogLeftView =
{ 0xB661C2A0, 0x1C70, 0x4579, { 0xA3, 0x9A, 0xF3, 0x9D, 0x29, 0x4F, 0x4D, 0x41 } };

BEGIN_INTERFACE_MAP(CUdfLogLeftView, CView)
	INTERFACE_PART(CUdfLogLeftView, IID_IUdfLogLeftView, Dispatch)
END_INTERFACE_MAP()


// CUdfLogLeftView 绘图

void CUdfLogLeftView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
}


// CUdfLogLeftView 诊断

#ifdef _DEBUG
void CUdfLogLeftView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CUdfLogLeftView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CUdfLogLeftView 消息处理程序

int CUdfLogLeftView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	if (!m_pTreeCtrl)
	{
		//m_pTreeCtrl = new CTreeCtrl();
		if (!m_pTreeCtrl.Create(TVS_HASBUTTONS|/*TVS_HASLINES|*/TVS_LINESATROOT|WS_CHILD|WS_VISIBLE|TVS_SHOWSELALWAYS,CRect(0,0,10,10),this,IDC_LOG_TREECTRL))
		{
			return -1;
		}
	}

	m_pImageList = new CImageList();
	m_pImageList->Create(16,16,ILC_MASK | ILC_COLOR32,2,2);

	HICON hicon[2];


	hicon[0] = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_RTUROOT));
	hicon[1] = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_RTUPRO));
	for (int idx = 0;idx < 2; idx++)
	{
		m_pImageList->Add(hicon[idx]);
	}



	m_pImageList->SetBkColor(RGB(255,255,255));

	m_pTreeCtrl.SetImageList(m_pImageList,TVSIL_NORMAL);

	DWORD dwStyles=GetWindowLong(m_pTreeCtrl.m_hWnd,GWL_STYLE);	//获取树控制原风格

	dwStyles|= TVS_HASBUTTONS/*|TVS_HASLINES|TVS_LINESATROOT*/;

	SetWindowLong(m_pTreeCtrl.m_hWnd,GWL_STYLE,dwStyles);			//设置风格


	return 0;
}

void CUdfLogLeftView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_pTreeCtrl.GetSafeHwnd())
	{
		m_pTreeCtrl.MoveWindow(0,0,cx,cy);
	}
}

void CUdfLogLeftView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}


BOOL CUdfLogLeftView::InitTreeView()
{

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();

	CTreeCtrl *pTree = &m_pTreeCtrl;
   
    if (!pTree)
		return FALSE;

	//m_pTreeCtrl->SetImageList(m_pImageList,TVSIL_NORMAL);
	m_pTreeCtrl.DeleteAllItems();

    HTREEITEM pTItem = NULL;


	TVINSERTSTRUCT TCItem;//定义插入项数据结构

	TCItem.hParent=NULL;												//设置父项句柄
	TCItem.hInsertAfter=TVI_LAST;										//在最后增加
	TCItem.item.mask=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;//设屏蔽
	TCItem.item.pszText="日志系统";
	TCItem.item.iImage=0;
	TCItem.item.iSelectedImage=0;
	TCItem.item.lParam = NULL;


	HTREEITEM tempItem = NULL;   
	HTREEITEM HRoot = pTree->InsertItem(&TCItem);

	TCItem.item.iImage=1;
	TCItem.item.iSelectedImage=1;

	if(pApp->m_GlobalOptions.m_LogOpts.CmStatusSave_Flag != 0)
	{

		TCItem.item.pszText = "通信状态日志";
		TCItem.hParent = HRoot ;
		tempItem =pTree->InsertItem(&TCItem);
		pTree->SetItemData(tempItem,(DWORD_PTR)RTU_CM_STATUS_LOG);
	}

	if (pApp->m_GlobalOptions.m_LogOpts.YxChangeSave_Flag != 0)
	{
		TCItem.hParent = HRoot;
		TCItem.item.pszText = "遥信变位日志";
		tempItem=pTree->InsertItem(&TCItem);
		pTree->SetItemData(tempItem,(DWORD_PTR)RTU_YX_CHANGE_LOG);
	}

	if (pApp->m_GlobalOptions.m_LogOpts.YcChangeSave_Flag != 0)
	{
		TCItem.hParent = HRoot;
		TCItem.item.pszText = "遥测变位日志";
		tempItem=pTree->InsertItem(&TCItem);
		pTree->SetItemData(tempItem,(DWORD_PTR)RTU_YC_CHANGE_LOG);
	}

    if (pApp->m_GlobalOptions.m_LogOpts.CmBufferSave_Flag != 0)
	{
		TCItem.hParent = HRoot;
		TCItem.item.pszText = "通信报文日志";
		tempItem=pTree->InsertItem(&TCItem);
		pTree->SetItemData(tempItem,(DWORD_PTR)RTU_CM_BUFFER_LOG);
	}

	if (pApp->m_GlobalOptions.m_LogOpts.YkOperatSave_Flag != 0)
	{
		TCItem.hParent = HRoot;
		TCItem.item.pszText = "遥控操作日志";
		tempItem=pTree->InsertItem(&TCItem);
		pTree->SetItemData(tempItem,(DWORD_PTR)RTU_YK_OPERAT_LOG);
	}

	if (pApp->m_GlobalOptions.m_LogOpts.ZdOperatSave_Flag != 0)
	{
		TCItem.hParent = HRoot;
		TCItem.item.pszText = "定值操作日志";
		tempItem=pTree->InsertItem(&TCItem);
		pTree->SetItemData(tempItem,(DWORD_PTR)RTU_ZD_OPERAT_LOG);
	}


	if (pApp->m_GlobalOptions.m_LogOpts.DataInErrSave_Flag != 0)
	{
		TCItem.hParent = HRoot;
		TCItem.item.pszText = "数据存库错误日志";
		tempItem = pTree->InsertItem(&TCItem);
		pTree->SetItemData(tempItem,(DWORD_PTR)DATAIN_ERROR_LOG);
	}

	if (pApp->m_GlobalOptions.m_LogOpts.RunningSSave_Flag != 0)
	{
		TCItem.hParent = HRoot;
		TCItem.item.pszText = "系统运行时日志";
		tempItem=pTree->InsertItem(&TCItem);
		pTree->SetItemData(tempItem,(DWORD_PTR)SYS_RUNNING_S_LOG);
	}


	/*
	if (pApp->m_GlobalOptions.m_bAdminMode != 0)
	{
		TCItem.hParent = HRoot;
		TCItem.item.pszText = "其他日志信息";
		tempItem = pTree->InsertItem(&TCItem);
		pTree->SetItemData(tempItem,(DWORD_PTR)SYS_OTHER_RUN_LOG);
	}
	*/

	pTree->Expand(HRoot,TVE_EXPAND);

	
	return TRUE;
}



void CUdfLogLeftView::OnClkTreeCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{
	//LPNMITEMACTIVATE pItem = ( LPNMITEMACTIVATE )pNMHDR;
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	if (pNMTreeView != NULL)
	{
		UINT   uFlags;  
		CPoint ptAction;  
		::GetCursorPos(   &ptAction   );  
		ScreenToClient(   &ptAction   );  

		HTREEITEM   item = m_pTreeCtrl.HitTest(ptAction,   &uFlags);  



		if (item != NULL  && (TVHT_ONITEM & uFlags) )
		{		
			_LOG_MESSAGE_TYPE n_Type = (_LOG_MESSAGE_TYPE)m_pTreeCtrl.GetItemData(item);
			TRACE("%d\r\n",(int)n_Type);
			n_CurentType = n_Type;

			CCMSApp* pApp = (CCMSApp*)AfxGetApp();
			CMainFrame* pMainFrame = (CMainFrame*)pApp->m_pMainWnd;
			pMainFrame->m_wndLogFrame.m_pRightView->ShowLogInformation(n_Type,CUdfLogQueryDlg::QUERY_LOGTYPE_ALL, NULL,NULL);

		}
	}
	m_pTreeCtrl.Invalidate(TRUE);
}


void  CUdfLogLeftView::SetFrameWnd(CWnd* pFrameWnd)
{
	m_pFrameWnd = pFrameWnd;
}

CWnd* CUdfLogLeftView::GetFrameWnd()
{
	return m_pFrameWnd;
}