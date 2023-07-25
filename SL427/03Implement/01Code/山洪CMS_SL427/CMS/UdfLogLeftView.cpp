// UdfLogLeftView.cpp : ʵ���ļ�
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
	// �ͷ��˶��Զ�����������һ�����ú󣬽�����
	// OnFinalRelease�����ཫ�Զ�
	// ɾ���ö����ڵ��øû���֮ǰ�����������
	// ��������ĸ���������롣

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

// ע��: ������� IID_IUdfLogLeftView ֧��
//  ��֧������ VBA �����Ͱ�ȫ�󶨡��� IID ����ͬ���ӵ� .IDL �ļ��е�
//  ���Ƚӿڵ� GUID ƥ�䡣

// {B661C2A0-1C70-4579-A39A-F39D294F4D41}
static const IID IID_IUdfLogLeftView =
{ 0xB661C2A0, 0x1C70, 0x4579, { 0xA3, 0x9A, 0xF3, 0x9D, 0x29, 0x4F, 0x4D, 0x41 } };

BEGIN_INTERFACE_MAP(CUdfLogLeftView, CView)
	INTERFACE_PART(CUdfLogLeftView, IID_IUdfLogLeftView, Dispatch)
END_INTERFACE_MAP()


// CUdfLogLeftView ��ͼ

void CUdfLogLeftView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: �ڴ���ӻ��ƴ���
}


// CUdfLogLeftView ���

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


// CUdfLogLeftView ��Ϣ�������

int CUdfLogLeftView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
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

	DWORD dwStyles=GetWindowLong(m_pTreeCtrl.m_hWnd,GWL_STYLE);	//��ȡ������ԭ���

	dwStyles|= TVS_HASBUTTONS/*|TVS_HASLINES|TVS_LINESATROOT*/;

	SetWindowLong(m_pTreeCtrl.m_hWnd,GWL_STYLE,dwStyles);			//���÷��


	return 0;
}

void CUdfLogLeftView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if (m_pTreeCtrl.GetSafeHwnd())
	{
		m_pTreeCtrl.MoveWindow(0,0,cx,cy);
	}
}

void CUdfLogLeftView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
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


	TVINSERTSTRUCT TCItem;//������������ݽṹ

	TCItem.hParent=NULL;												//���ø�����
	TCItem.hInsertAfter=TVI_LAST;										//���������
	TCItem.item.mask=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;//������
	TCItem.item.pszText="��־ϵͳ";
	TCItem.item.iImage=0;
	TCItem.item.iSelectedImage=0;
	TCItem.item.lParam = NULL;


	HTREEITEM tempItem = NULL;   
	HTREEITEM HRoot = pTree->InsertItem(&TCItem);

	TCItem.item.iImage=1;
	TCItem.item.iSelectedImage=1;

	if(pApp->m_GlobalOptions.m_LogOpts.CmStatusSave_Flag != 0)
	{

		TCItem.item.pszText = "ͨ��״̬��־";
		TCItem.hParent = HRoot ;
		tempItem =pTree->InsertItem(&TCItem);
		pTree->SetItemData(tempItem,(DWORD_PTR)RTU_CM_STATUS_LOG);
	}

	if (pApp->m_GlobalOptions.m_LogOpts.YxChangeSave_Flag != 0)
	{
		TCItem.hParent = HRoot;
		TCItem.item.pszText = "ң�ű�λ��־";
		tempItem=pTree->InsertItem(&TCItem);
		pTree->SetItemData(tempItem,(DWORD_PTR)RTU_YX_CHANGE_LOG);
	}

	if (pApp->m_GlobalOptions.m_LogOpts.YcChangeSave_Flag != 0)
	{
		TCItem.hParent = HRoot;
		TCItem.item.pszText = "ң���λ��־";
		tempItem=pTree->InsertItem(&TCItem);
		pTree->SetItemData(tempItem,(DWORD_PTR)RTU_YC_CHANGE_LOG);
	}

    if (pApp->m_GlobalOptions.m_LogOpts.CmBufferSave_Flag != 0)
	{
		TCItem.hParent = HRoot;
		TCItem.item.pszText = "ͨ�ű�����־";
		tempItem=pTree->InsertItem(&TCItem);
		pTree->SetItemData(tempItem,(DWORD_PTR)RTU_CM_BUFFER_LOG);
	}

	if (pApp->m_GlobalOptions.m_LogOpts.YkOperatSave_Flag != 0)
	{
		TCItem.hParent = HRoot;
		TCItem.item.pszText = "ң�ز�����־";
		tempItem=pTree->InsertItem(&TCItem);
		pTree->SetItemData(tempItem,(DWORD_PTR)RTU_YK_OPERAT_LOG);
	}

	if (pApp->m_GlobalOptions.m_LogOpts.ZdOperatSave_Flag != 0)
	{
		TCItem.hParent = HRoot;
		TCItem.item.pszText = "��ֵ������־";
		tempItem=pTree->InsertItem(&TCItem);
		pTree->SetItemData(tempItem,(DWORD_PTR)RTU_ZD_OPERAT_LOG);
	}


	if (pApp->m_GlobalOptions.m_LogOpts.DataInErrSave_Flag != 0)
	{
		TCItem.hParent = HRoot;
		TCItem.item.pszText = "���ݴ�������־";
		tempItem = pTree->InsertItem(&TCItem);
		pTree->SetItemData(tempItem,(DWORD_PTR)DATAIN_ERROR_LOG);
	}

	if (pApp->m_GlobalOptions.m_LogOpts.RunningSSave_Flag != 0)
	{
		TCItem.hParent = HRoot;
		TCItem.item.pszText = "ϵͳ����ʱ��־";
		tempItem=pTree->InsertItem(&TCItem);
		pTree->SetItemData(tempItem,(DWORD_PTR)SYS_RUNNING_S_LOG);
	}


	/*
	if (pApp->m_GlobalOptions.m_bAdminMode != 0)
	{
		TCItem.hParent = HRoot;
		TCItem.item.pszText = "������־��Ϣ";
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