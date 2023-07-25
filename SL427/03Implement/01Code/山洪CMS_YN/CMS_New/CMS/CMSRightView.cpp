// CMSRightView.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "CMSRightView.h"
#include "RtuStation.h"
#include "RtuPropertyDlg.h"
#include "MainFrm.h"


// CCMSRightView

IMPLEMENT_DYNCREATE(CCMSRightView, CView)
IMPLEMENT_MENUXP(CCMSRightView, CView)

CCMSRightView::CCMSRightView()
{

}

CCMSRightView::~CCMSRightView()
{
}

BEGIN_MESSAGE_MAP(CCMSRightView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_MENUXP_MESSAGES()
	ON_NOTIFY(NM_CLICK,		IDC_MAIN_GRIDCTRL,&CCMSRightView::OnLClickGridCtrl)
	ON_NOTIFY(NM_DBLCLK,	IDC_MAIN_GRIDCTRL,&CCMSRightView::OnLDbClickGridCtrl)
	ON_NOTIFY(NM_RCLICK,	IDC_MAIN_GRIDCTRL,&CCMSRightView::OnRClickGridCtrl)
	ON_COMMAND(IDM_MAINGRID_NONE,			  &CCMSRightView::OnGridLineNone)
	ON_COMMAND(IDM_MAINGRID_VERT,			  &CCMSRightView::OnGridLineVert)
	ON_COMMAND(IDM_MAINGRID_HORZ,		      &CCMSRightView::OnGridLineHorz)
	ON_COMMAND(IDM_MAINGRID_BOTH,			  &CCMSRightView::OnGridLineBoth)
	ON_UPDATE_COMMAND_UI(IDM_MAINGRID_BOTH,   &CCMSRightView::OnUpdateUIGridLineBoth)
	ON_UPDATE_COMMAND_UI(IDM_MAINGRID_HORZ,   &CCMSRightView::OnUpdateUIGridLineHorz)
	ON_UPDATE_COMMAND_UI(IDM_MAINGRID_NONE,   &CCMSRightView::OnUpdateUIGridLineNone)
	ON_UPDATE_COMMAND_UI(IDM_MAINGRID_VERT,   &CCMSRightView::OnUpdateUIGridLineVert)
END_MESSAGE_MAP()


// CCMSRightView 绘图

void CCMSRightView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
}


// CCMSRightView 诊断

#ifdef _DEBUG
void CCMSRightView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CCMSRightView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCMSRightView 消息处理程序

int CCMSRightView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CMenuXP::SetXPLookNFeel(this);
	if (!m_wndBackGroud.Create("",WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this))
	{
		return -1;
	}

	if (!m_wndCaption.Create("",WS_VISIBLE| WS_CHILD,CRect(0,0,0,0),this))
	{
		return -1;
	}

	if (!m_wndListCtrl.Create(CRect(0,0,0,0),this,IDC_MAIN_GRIDCTRL,WS_CHILD|WS_VISIBLE))
	{
		TRACE("Faild to create rtu cfg right gridctrl\r\n");
		return -1;
	}


	if (!m_wndImageList.Create(16,16,ILC_MASK|ILC_COLOR32,3,3))
	{
		TRACE("Create RtuCfg imagelist ctrl error!\r\n");
		return -1;
	}

	HICON icon[3];
	icon[0] = AfxGetApp()->LoadIcon(IDI_CMS_UNCONNECT);
	icon[1] = AfxGetApp()->LoadIcon(IDI_CMS_CONNECT);
	icon[2] = AfxGetApp()->LoadIcon(IDI_CMS_UNKNOWN);

	for (int i= 0; i < 3; i++)
	{
		m_wndImageList.Add(icon[i]);
	}

	m_wndImageList.SetBkColor(RGB(255,255,255));
	m_wndListCtrl.SetImageList(&m_wndImageList);

	CFont* pFont = m_wndListCtrl.GetFont();
	m_wndCaption.SetFont(pFont);
	m_wndListCtrl.SetListMode(TRUE);
	m_wndListCtrl.SetBkColor(RGB(255,255,255));
	m_wndListCtrl.SetSingleColSelection(TRUE);
	m_wndListCtrl.SetSingleRowSelection(TRUE);
	m_wndListCtrl.SetEditable(FALSE);
	m_wndListCtrl.SetFixedRowCount(1);

	m_wndListCtrl.SetGridLines(GVL_NONE);

    InitGridCtrl();

	SetCaption("终端信息");
	return 0;
}

void CCMSRightView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (m_wndBackGroud.GetSafeHwnd())
		m_wndBackGroud.MoveWindow(0,0,cx,cy);
	if (m_wndCaption.GetSafeHwnd())
		m_wndCaption.MoveWindow(4,4,cx-4,16);
	if (m_wndListCtrl.GetSafeHwnd())
		m_wndListCtrl.MoveWindow(0,20,cx,cy-20);
}


void CCMSRightView::InitGridCtrl()
{
	if (!m_wndListCtrl.GetSafeHwnd())
		return;

	m_wndListCtrl.DeleteAllItems();

	int nColsCount = 10;
	CString Hearders[] = {"名称",
		                   "通信状态",
	                       "省/自治区/直辖市",
						   "地区/市",
						   "县",
						   "行政区划码",
						   "终端地址",
						   "应用层协议类型",
						   "中心站地址",
						   "属 性"};
	int  colsWidths[] = {100,
		                 70,
		                 90,
					     80,
					     80,
					     85,
						 100,
						 120,
						 80,
					     80};
    m_wndListCtrl.SetFixedRowCount(1);

	int ColIdx = 0,RowIdx =0;
	for (ColIdx = 0;ColIdx <nColsCount; ColIdx++)
	{
		m_wndListCtrl.InsertColumn(Hearders[ColIdx],DT_VCENTER|DT_LEFT,ColIdx);
		m_wndListCtrl.SetColumnWidth(ColIdx,colsWidths[ColIdx]);
	}

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();

	if (pApp->m_pReadDB && pApp->m_pReadDB->GetRtuStList())
	{
		CPtrList* pLst = pApp->m_pReadDB->GetRtuStList();
		
		if (pLst && !pLst->IsEmpty())
		{
			int nCount = (int)pLst->GetCount();
			m_wndListCtrl.SetRowCount(nCount + 1);

			RowIdx = 1;
			POSITION rPos = pLst->GetHeadPosition();
			while (rPos != NULL)
			{
				CRtuStation* pObj = (CRtuStation*)pLst->GetNext(rPos);
				if (pObj)
				{
					for (ColIdx = 0; ColIdx < nColsCount; ColIdx ++)
					{
						GV_ITEM Item;
						Item.row = RowIdx;
						Item.col = ColIdx;
						Item.nFormat = DT_LEFT|DT_VCENTER;

						CUdfProvinceCode* pProvinCode = pApp->m_pReadDB->m_AdDivCodeDb.SearchProvinceCodeItem(pObj->GetProvinceCode());
						CString strProv="",strRegion="",strCounty="";
						int AppProtoType=1;
						CString strStCode = "";//用于SCSW008规约
						if (pProvinCode)
						{
							strProv = pProvinCode->GetName();

							CUdfRegionCode* pReginCode = pProvinCode->SearchRegionCodeItem(pObj->GetRegionCode());
							if (pReginCode)
							{
								strRegion = pReginCode->GetName();
								CUdfCountyCode* pCountyCode = pReginCode->SearchCountyCodeItem(pObj->GetCountyCode());
								if (pCountyCode)
								{
									strCounty = pCountyCode->GetName();
								}
							}
						}

						AppProtoType = pObj->GetAppProtocolType();

						switch (ColIdx)
						{
						case 0:
							Item.mask = GVIF_TEXT|GVIF_PARAM;
							Item.lParam = (LPARAM) pObj;
							Item.strText = pObj->GetName();
							break;
						case 1:
							{
								Item.mask = GVIF_IMAGE|GVIF_TEXT;
								BYTE CommState = pObj->GetCommState();
								if (CommState == Disconnect_State)       //Un Connect
								{
									//Item.strText = "异常";
									Item.iImage = 0;
								}
								else if (CommState == Connected_State)  //Connect
								{
									//Item.strText = "正常";
									Item.iImage = 1;
								}
								else if (CommState == CommUnknown_State)   //Unknown
								{
									//Item.strText = "未知";
									Item.iImage = 2;
								}
							}
							break;
						case 2:
							Item.mask = GVIF_TEXT;
							Item.strText = strProv;
							break;
						case 3:
							Item.mask = GVIF_TEXT;
							Item.strText = strRegion;
							break;
						case 4:
							Item.mask = GVIF_TEXT;
							Item.strText = strCounty;
							break;
						case 5:
							Item.mask = GVIF_TEXT;
							Item.strText.Format(_T("%02d%02d%02d"),pObj->GetProvinceCode(),pObj->GetRegionCode(),pObj->GetCountyCode());
							break;
						case 6:
							{
								Item.mask = GVIF_TEXT;
								if (AppProtoType==WaterResources_Protocol)
								{
									Item.strText.Format(_T("%d"),pObj->GetAddrA2());
								}
								else if (AppProtoType == SCSW_008_Protocol)
								{
									Item.strText = pObj->GetStringStCode();
								}
								else if (AppProtoType == SYQYC_Comm_Protocol)
								{
									//Item.strText.Format(_T("%d"),pObj->GetAddrA2());
									Item.strText.Format(_T("%d"),pObj->GetSYQStCode());
								}
							}
							break;
						case 7:
							{
								Item.mask = GVIF_TEXT;
								if (AppProtoType==WaterResources_Protocol)
								{
									Item.strText = _T("SL427");
								}
								else if (AppProtoType == SCSW_008_Protocol)
								{
									Item.strText = _T("SCSW008-2011");
								}
								else if (AppProtoType == SYQYC_Comm_Protocol)
								{
									Item.strText = _T("水雨情遥测系统数据通信规约");
								}
							}
							break;
						case 8:
							if (AppProtoType == SYQYC_Comm_Protocol)
							{
								//Item.strText.Format(_T("%d"),pObj->GetAddrA2());
								Item.strText.Format(_T("%d"),pObj->GetSYQCenterCode());
							}
							else
								Item.strText = "0";

							break;
						case 9:
							{
								CGridBtnCell *pbtnCell=NULL;
								m_wndListCtrl.SetCellType(Item.row,Item.col,RUNTIME_CLASS(CGridBtnCell));
								pbtnCell = (CGridBtnCell *)m_wndListCtrl.GetCell(Item.row,Item.col);
								pObj->m_btnPro.SetGrid(&m_wndListCtrl);
								pbtnCell->SetBtnDataBase(&pObj->m_btnPro);
								pbtnCell->SetupBtns(0,DFC_BUTTON,DFCS_BUTTONPUSH ,CGridBtnCellBase::CTL_ALIGN_CENTER,
									0,FALSE,"属性"); 
							}
							break;
						}  //end switch

						if (Item.col != 9)
							m_wndListCtrl.SetItem(&Item);

					}	 //end for

					m_wndListCtrl.SetRowHeight(RowIdx,MFC_GRIDCTRL_HEIGHT);
					RowIdx ++;
				}	//end if (pObj) 
				
			}	//end while
		}
	}

	m_wndListCtrl.Refresh();
}

void CCMSRightView::OnLClickGridCtrl(NMHDR *pNotifyStruct,LRESULT *pResult)
{
	 NM_GRIDVIEW* pItem =(NM_GRIDVIEW*)pNotifyStruct;
	 if (!pItem  || pItem->iRow <= 0) return;
	 if (pItem->iColumn != 9 )		  return;

	 CRtuStation* pSub = (CRtuStation*)m_wndListCtrl.GetItemData(pItem->iRow,0);
	 if (!pSub) return;


	 CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	 CMainFrame* pMainFrame = (CMainFrame*)pApp->m_pMainWnd;
	 BYTE utype = 0;
	 if (pMainFrame->GetLoginFlag(utype) == false)
		 return;
	 
	 //显示遥测终端属性操作窗口
	 pApp->Set_Cur_Op_Station(pSub);
	 CRtuPropertyDlg* pDlg = pMainFrame->GetRtuPropertyDlg();
	 pDlg->SetRtuStation(pSub);
	 pDlg->Show(SW_SHOWNORMAL);

}

void CCMSRightView::OnLDbClickGridCtrl(NMHDR *pNotifyStruct,LRESULT *pResult)
{
	 NM_GRIDVIEW* pItem =(NM_GRIDVIEW*)pNotifyStruct;
	 if (!pItem || pItem->iRow <= 0)						return;
	 if (pItem->iColumn == 0 || pItem->iColumn == 5)		return;
}

void CCMSRightView::OnRClickGridCtrl(NMHDR* pNotifyStruct,LRESULT *pResutl)
{
	 NM_GRIDVIEW* pItem =(NM_GRIDVIEW*)pNotifyStruct;
	 CPoint CursorPos;
	 GetCursorPos(&CursorPos);
}


void CCMSRightView::OnGridLineNone()
{
	if (m_wndListCtrl.GetSafeHwnd())
	{
		m_wndListCtrl.SetGridLines(GVL_NONE);
	}
}

void CCMSRightView::OnGridLineVert()
{
	if (m_wndListCtrl.GetSafeHwnd())
	{
		m_wndListCtrl.SetGridLines(GVL_VERT);
	}
}

void CCMSRightView::OnGridLineHorz()
{
	if (m_wndListCtrl.GetSafeHwnd())
	{
		m_wndListCtrl.SetGridLines(GVL_HORZ);
	}
}

void CCMSRightView::OnGridLineBoth()
{
	if (m_wndListCtrl.GetSafeHwnd())
	{
		m_wndListCtrl.SetGridLines(GVL_BOTH);
	}
}


void CCMSRightView::OnUpdateUIGridLineBoth(CCmdUI* pCmdUI)
{
	if (!m_wndListCtrl.GetSafeHwnd()) return;

	int nGirdLinesStyle = m_wndListCtrl.GetGridLines();
	if (nGirdLinesStyle == GVL_BOTH)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

void CCMSRightView::OnUpdateUIGridLineHorz(CCmdUI* pCmdUI)
{
	if (!m_wndListCtrl.GetSafeHwnd()) return;

	int nGirdLinesStyle = m_wndListCtrl.GetGridLines();
	if (nGirdLinesStyle == GVL_HORZ)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}


void CCMSRightView::OnUpdateUIGridLineNone(CCmdUI* pCmdUI)
{
	if (!m_wndListCtrl.GetSafeHwnd()) return;

	int nGirdLinesStyle = m_wndListCtrl.GetGridLines();
	if (nGirdLinesStyle == GVL_NONE)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

void CCMSRightView::OnUpdateUIGridLineVert(CCmdUI* pCmdUI)
{
	if (!m_wndListCtrl.GetSafeHwnd()) return;

	int nGirdLinesStyle = m_wndListCtrl.GetGridLines();
	if (nGirdLinesStyle == GVL_VERT)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}


void CCMSRightView::SetCaption(CString strCaption)
{
	if (m_wndCaption.GetSafeHwnd())
		m_wndCaption.SetWindowText(strCaption);
}

void CCMSRightView::UpdateCommState(CRtuStation* pSub)
{
	if (!pSub) return;

	if (m_wndListCtrl.GetSafeHwnd())
	{
		m_UpdateComm_Mutex.Lock();
		int nCount = m_wndListCtrl.GetRowCount();
		int rPos = 0;
		bool blFinded = false;
		for (int i = 1; i < nCount; i ++)
		{
			CRtuStation* pItemSub = (CRtuStation*)m_wndListCtrl.GetItemData(i,0);
			if (pItemSub  && pItemSub == pSub)
			{
				rPos = i;
				blFinded =true;
			}
		}

		if (blFinded == true)
		{
			GV_ITEM Item;
			Item.col = 1;
			Item.row = rPos;
			Item.mask = GVIF_IMAGE|GVIF_TEXT;
			BYTE CommState = pSub->GetCommState();
			if (CommState == Disconnect_State)       //Un Connect
			{
				//Item.strText = "异常";
				Item.iImage = 0;
			}
			else if (CommState == Connected_State)  //Connect
			{
				//Item.strText = "正常";
				Item.iImage = 1;
			}
			else if (CommState == CommUnknown_State)   //Unknown
			{
				//Item.strText = "未知";
				Item.iImage = 2;
			}
			m_wndListCtrl.SetItem(&Item);
		}
		m_UpdateComm_Mutex.Unlock();
		m_wndListCtrl.Refresh();
	}
		
}