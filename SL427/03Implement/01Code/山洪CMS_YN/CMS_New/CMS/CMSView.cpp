// CMSView.cpp : CCMSView ���ʵ��
//

#include "stdafx.h"
#include "CMS.h"

#include "CMSDoc.h"
#include "CMSView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define  IDC_UDF_RIGHTVIEW_GRIDCTRL 9998
// CCMSView

IMPLEMENT_DYNCREATE(CCMSView, CView)

BEGIN_MESSAGE_MAP(CCMSView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CCMSView ����/����

CCMSView::CCMSView()
{
	// TODO: �ڴ˴���ӹ������

}

CCMSView::~CCMSView()
{
}

BOOL CCMSView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CCMSView ����

void CCMSView::OnDraw(CDC* /*pDC*/)
{
	CCMSDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CCMSView ��ӡ

BOOL CCMSView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CCMSView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CCMSView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CCMSView ���

#ifdef _DEBUG
void CCMSView::AssertValid() const
{
	CView::AssertValid();
}

void CCMSView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCMSDoc* CCMSView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCMSDoc)));
	return (CCMSDoc*)m_pDocument;
}
#endif //_DEBUG


// CCMSView ��Ϣ�������

int CCMSView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	/*
	if (!m_wndGridCtrl.Create(CRect(0,0,0,0),
		this, IDC_UDF_RIGHTVIEW_GRIDCTRL,WS_CHILD| WS_VISIBLE))
	{
		TRACE("Failed to create zd group grid!\n");
		return -1;
	}

	m_wndGridCtrl.SetBkColor(RGB(255,255,255));
	m_wndGridCtrl.SetListMode(TRUE);
	m_wndGridCtrl.SetSingleColSelection(TRUE);
	m_wndGridCtrl.SetSingleColSelection(TRUE);

	m_wndGridCtrl.InsertColumn("1",DT_LEFT);
	m_wndGridCtrl.InsertColumn("2",DT_LEFT);	
	m_wndGridCtrl.InsertColumn("3",DT_LEFT);
	m_wndGridCtrl.InsertColumn("4",DT_LEFT);
	m_wndGridCtrl.SetRowCount(10);
	m_wndGridCtrl.SetFixedRowCount(1);

	for (int i = 1;i<10;i++)
	{
		m_wndGridCtrl.SetCellType(i,2,RUNTIME_CLASS(CGridCellCheck));
		m_wndGridCtrl.SetCellType(i,3,RUNTIME_CLASS(CGridBtnCell));
		CGridBtnCell* pBtnCell = (CGridBtnCell*)m_wndGridCtrl.GetCell(i,3);
		m_DBClockCheck[i].SetGrid(&m_wndGridCtrl);
		pBtnCell->SetBtnDataBase(&m_DBClockCheck[i]);
		pBtnCell->SetupBtns(0,DFC_BUTTON,DFCS_BUTTONPUSH,CGridBtnCellBase::CTL_ALIGN_CENTER,\
			0,FALSE,">>>");

	}
	*/


	return 0;
}

void CCMSView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	/*
	if (m_wndGridCtrl.GetSafeHwnd())
	{
		m_wndGridCtrl.MoveWindow(0,0,cx,cy);
	}
	*/
}
