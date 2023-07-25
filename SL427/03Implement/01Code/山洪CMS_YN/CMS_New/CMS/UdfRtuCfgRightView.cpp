// UdfRtuCfgRightView.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "MainFrm.h"
#include "GridCtrl/GridCellCheck.h"
#include "UdfRtuCfgRightView.h"
#include "UdfAddRtuStDlg.h"
#include "UdfRtuCfgFrame.h"
#include "UdfAddYcObjectDlg.h"
#include "UdfAddYkObjectDlg.h"
#include "UdfAddYxObjectDlg.h"
#include "UdfAddZdObjectDlg.h"


// CUdfRtuCfgRightView

IMPLEMENT_DYNCREATE(CUdfRtuCfgRightView, CView)
IMPLEMENT_MENUXP(CUdfRtuCfgRightView, CView)

CUdfRtuCfgRightView::CUdfRtuCfgRightView()
{
	m_pRtuCfgFrame = NULL;
	m_pCurStation = NULL;
	m_xGridCtrlStyle = RTUST_STINFO_TYPE;

	m_pSel_RtuStation = NULL;
	m_pSel_YcObject   = NULL;
	m_pSel_YxObject   = NULL;
	m_pSel_YkObject   = NULL;
	m_pSel_ZdObject   = NULL;
}

CUdfRtuCfgRightView::~CUdfRtuCfgRightView()
{
}

BEGIN_MESSAGE_MAP(CUdfRtuCfgRightView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_MENUXP_MESSAGES()
	ON_NOTIFY(NM_CLICK,IDC_RTUCFG_GRIDCTRL,&CUdfRtuCfgRightView::OnLClickGridCtrl)
	ON_NOTIFY(NM_DBLCLK,IDC_RTUCFG_GRIDCTRL,&CUdfRtuCfgRightView::OnLDbClickGridCtrl)
	ON_NOTIFY(NM_RCLICK,IDC_RTUCFG_GRIDCTRL,&CUdfRtuCfgRightView::OnRClickGridCtrl)
	ON_COMMAND(IDM_RTU_SELALL,&CUdfRtuCfgRightView::OnRtuSelAllItem)
	ON_COMMAND(IDM_RTU_NOSELALL,&CUdfRtuCfgRightView::OnRtuNoSelAllItem)
	ON_COMMAND(IDM_RTUCFG_RTUSTADD,&CUdfRtuCfgRightView::OnRtuAddOneItem)
	ON_COMMAND(IDM_RTU_DEL,&CUdfRtuCfgRightView::OnRtuDelItem)
	ON_COMMAND(IDM_RTU_DELSEL,&CUdfRtuCfgRightView::OnRtuDelSelItem)
	ON_UPDATE_COMMAND_UI(IDM_RTU_DELSEL,&CUdfRtuCfgRightView::OnUpdateUIRtuDelSelItem)
	ON_COMMAND(IDM_RTU_PRO,&CUdfRtuCfgRightView::OnRtuProItem)

	ON_COMMAND(IDM_YC_SELALL,&CUdfRtuCfgRightView::OnYcSelAllItem)
	ON_COMMAND(IDM_YC_NOSELALL,&CUdfRtuCfgRightView::OnYcNoSelAllItem)
	ON_COMMAND(IDM_RTUCFG_YCADD,&CUdfRtuCfgRightView::OnYcAddOneItem)
	ON_COMMAND(IDM_YC_DELSEL,&CUdfRtuCfgRightView::OnYcDelSelItem)
	ON_COMMAND(IDM_YC_DEL,&CUdfRtuCfgRightView::OnYcDelItem)
	ON_COMMAND(IDM_YC_PRO,&CUdfRtuCfgRightView::OnYcProItem)

	ON_COMMAND(IDM_YX_SELALL,&CUdfRtuCfgRightView::OnYxSelAllItem)
	ON_COMMAND(IDM_YX_NOSELALL,&CUdfRtuCfgRightView::OnYxNoSelAllItem)
	ON_COMMAND(IDM_RTUCFG_YXADD,&CUdfRtuCfgRightView::OnYxAddOneItem)
	ON_COMMAND(IDM_YX_DELSEL,&CUdfRtuCfgRightView::OnYxDelSelItem)
	ON_COMMAND(IDM_YX_DEL,&CUdfRtuCfgRightView::OnYxDelItem)
	ON_COMMAND(IDM_YX_PRO,&CUdfRtuCfgRightView::OnYxProItem)

	ON_COMMAND(IDM_YK_SELALL,&CUdfRtuCfgRightView::OnYkSelAllItem)
	ON_COMMAND(IDM_YK_NOSELALL,&CUdfRtuCfgRightView::OnYkNoSelAllItem)
	ON_COMMAND(IDM_RTUCFG_YKADD,&CUdfRtuCfgRightView::OnYkAddOneItem)
	ON_COMMAND(IDM_YK_DELSEL,&CUdfRtuCfgRightView::OnYkDelSelItem)
	ON_COMMAND(IDM_YK_DEL,&CUdfRtuCfgRightView::OnYkDelItem)
	ON_COMMAND(IDM_YK_PRO,&CUdfRtuCfgRightView::OnYkProItem)


	ON_COMMAND(IDM_ZD_SELALL,&CUdfRtuCfgRightView::OnZdSelAllItem)
	ON_COMMAND(IDM_ZD_NOSELALL,&CUdfRtuCfgRightView::OnZdNoSelAllItem)
	ON_COMMAND(IDM_RTUCFG_ZDADD,&CUdfRtuCfgRightView::OnZdAddOneItem)
	ON_COMMAND(IDM_ZD_DELSEL,&CUdfRtuCfgRightView::OnZdDelSelItem)
	ON_COMMAND(IDM_ZD_DEL,&CUdfRtuCfgRightView::OnZdDelItem)
	ON_COMMAND(IDM_ZD_PRO,&CUdfRtuCfgRightView::OnZdProItem)

END_MESSAGE_MAP()


// CUdfRtuCfgRightView 绘图

void CUdfRtuCfgRightView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
}


// CUdfRtuCfgRightView 诊断

#ifdef _DEBUG
void CUdfRtuCfgRightView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CUdfRtuCfgRightView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG



void CUdfRtuCfgRightView::SetRtuCfgFrame(CWnd* pFrame)
{
	m_pRtuCfgFrame = pFrame;
}

CWnd* CUdfRtuCfgRightView::GetRtuCfgFrame()
{
	return m_pRtuCfgFrame;
}


int CUdfRtuCfgRightView::OnCreate(LPCREATESTRUCT lpCreateStruct)
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
	if (!m_wndGridCtrl.Create(CRect(0,0,0,0),this,IDC_RTUCFG_GRIDCTRL,WS_CHILD|WS_VISIBLE))
	{
		TRACE("Faild to create rtu cfg right gridctrl\r\n");
		return -1;
	}
    
    CFont* pFont = m_wndGridCtrl.GetFont();
    m_wndCaption.SetFont(pFont);
	m_wndGridCtrl.SetWindowText("Caption");
	m_wndGridCtrl.SetListMode(TRUE);
	m_wndGridCtrl.SetBkColor(RGB(255,255,255));
	m_wndGridCtrl.SetSingleColSelection(TRUE);
	m_wndGridCtrl.SetGridLines(GVL_NONE);

	ShowRtuInfor(RTUST_STINFO_TYPE,NULL);
	return 0;
}

//响应消息WM_SIZE
//处理各个控件在窗口大小发生改变时，控件的显示大小
void CUdfRtuCfgRightView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (m_wndBackGroud.GetSafeHwnd())
		m_wndBackGroud.MoveWindow(0,0,cx,cy);
	if (m_wndCaption.GetSafeHwnd())
		m_wndCaption.MoveWindow(4,4,cx-4,16);
	if (m_wndGridCtrl.GetSafeHwnd())
		m_wndGridCtrl.MoveWindow(0,20,cx,cy-20);
}


void CUdfRtuCfgRightView::SetGridCtrlStyle(GRIDCTRL_STYLE xType)
{
	m_xGridCtrlStyle = xType;
}

GRIDCTRL_STYLE CUdfRtuCfgRightView::GetGridCtrlStyle()
{
	return m_xGridCtrlStyle;
}


void CUdfRtuCfgRightView::SetCaption(CString strCaption)
{
	if (m_wndCaption.GetSafeHwnd())
		m_wndCaption.SetWindowText(strCaption);
}


void CUdfRtuCfgRightView::InitGridCtrlHeaders(GRIDCTRL_STYLE xType)
{
	if (m_wndGridCtrl.GetSafeHwnd())
	{
		m_wndGridCtrl.DeleteAllItems();
		m_wndGridCtrl.EnableTitleTips(FALSE);
		m_wndGridCtrl.SetSingleColSelection(TRUE);
		m_wndGridCtrl.SetSingleRowSelection(TRUE);
		m_wndGridCtrl.SetEditable(FALSE);
	}


	SetGridCtrlStyle(xType);
	switch (xType)
	{
	case  RTUST_STINFO_TYPE:
		InitShowStInfoHeaders();
		break;
	case  RTUST_YCINFO_TYPE:
		InitShowYcInfoHeaders();
		break;
	case RTUST_YXINFO_TYPE:
		InitShowYxInfoHeaders();
		break;
	case RTUST_YKINFO_TYPE:
		InitShowYkInfoHeaders();
		break;
	case  RTUST_ZDINFO_TYPE:
		InitShowZdInfoHeaders();
		break;
	}
}


void CUdfRtuCfgRightView::InitShowStInfoHeaders()
{
  try
  {
	  CString Headers[13];
	  Headers[0] = "";
	  Headers[1] = "名称";
	  Headers[2] = "省(直辖市、自治区)";
	  Headers[3] = "地区(市)";
	  Headers[4] = "县";
	  Headers[5] = "地址类型";
	  //Headers[6] = "地址码";
	  //Headers[7] = "通信方式";

	  Headers[6] = "行政区划码";

	  Headers[7] = "终端地址";
	  Headers[8] = "应用协议类型";
	  Headers[9] = "中心站地址";

	  Headers[10] = "修改";
	  Headers[11] = "删除";


	  //Headers[8] = "通信接口";
	  //Headers[9] = "通信参数";
	  //Headers[10] = "修改";
	  //Headers[11] = "删除";

	  
	  int fixCols = 1;
	  int fixRows = 1;
	  //int nColsCount = 12;
	  int nColsCount = 12;

      m_wndGridCtrl.SetRowCount(1);
	  m_wndGridCtrl.SetColumnCount(nColsCount);
	  //m_wndGridCtrl.SetFixedColumnCount(fixCols);
	  m_wndGridCtrl.SetFixedRowCount(fixRows);

      int idxCol = 0;

	  CGridCellBase* pCell = NULL;
	  for (idxCol = 0; idxCol < nColsCount; idxCol ++)
	  {
		  pCell  = m_wndGridCtrl.GetCell(0,idxCol);
		  if (pCell)
			  pCell->SetText(Headers[idxCol]);
	  }

	  m_wndGridCtrl.SetCellType(0, 0, RUNTIME_CLASS(CGridCellCheck));
	  CGridCellCheck* pGridCellCheck = (CGridCellCheck*)m_wndGridCtrl.GetCell(0, 0);
	  if (pGridCellCheck)
		  pGridCellCheck->SetCheck(FALSE);

	  m_wndGridCtrl.SetColumnWidth(0,MFC_GRIDCTRL_CHECKBOX_WIDTH);
	  m_wndGridCtrl.SetColumnWidth(1,70/*80*/);
	  m_wndGridCtrl.SetColumnWidth(2,80/*100*/);
	  m_wndGridCtrl.SetColumnWidth(3,90/*100*/);
	  m_wndGridCtrl.SetColumnWidth(4,100/*70*/);
	  m_wndGridCtrl.SetColumnWidth(5,70);
	  m_wndGridCtrl.SetColumnWidth(6,85);
	  m_wndGridCtrl.SetColumnWidth(7,100/*85*/);
	  m_wndGridCtrl.SetColumnWidth(8,100/*85*/);
	  m_wndGridCtrl.SetColumnWidth(9,80/*85*/);

	  m_wndGridCtrl.SetColumnWidth(10,45);
	  m_wndGridCtrl.SetColumnWidth(11,45);

/*
	  m_wndGridCtrl.SetColumnWidth(8,75);
	  m_wndGridCtrl.SetColumnWidth(9,140);
	  m_wndGridCtrl.SetColumnWidth(10,45);
	  m_wndGridCtrl.SetColumnWidth(11,45);*/


  }
  catch (CMemoryException* e)
  {
  	e->ReportError();
	e->Delete();
	return;
  }
  catch (CFileException* e)
  {
	  e->ReportError();
	  e->Delete();
	  return;
  }
  catch (CException* e)
  {
	  e->ReportError();
	  e->Delete();
	  return;
  }
}

void CUdfRtuCfgRightView::InitShowYcInfoHeaders()
{
	try
	{
		CString Headers[11];
		Headers[0] = "";
		Headers[1] = "名称";
		Headers[2] = "遥测码";
		Headers[3] = "类型";
		Headers[4] = "显示位数";
		Headers[5] = "显示小数位数";
		Headers[6] = "单位";
		Headers[7] = "最大值";
		Headers[8] = "最小值";
		Headers[9] = "修改";
		Headers[10] = "删除";

		int fixCols = 1;
		int fixRows = 1;
		int nColsCount = 11;

		m_wndGridCtrl.SetRowCount(1);
		m_wndGridCtrl.SetColumnCount(nColsCount);
		//m_wndGridCtrl.SetFixedColumnCount(fixCols);
		m_wndGridCtrl.SetFixedRowCount(fixRows);

		int idxCol = 0;

		CGridCellBase* pCell = NULL;
		for (idxCol = 0; idxCol < nColsCount; idxCol ++)
		{
			pCell  = m_wndGridCtrl.GetCell(0,idxCol);
			if (pCell)
				pCell->SetText(Headers[idxCol]);
		}

		m_wndGridCtrl.SetCellType(0, 0, RUNTIME_CLASS(CGridCellCheck));
		CGridCellCheck* pGridCellCheck = (CGridCellCheck*)m_wndGridCtrl.GetCell(0, 0);
		if (pGridCellCheck)
			pGridCellCheck->SetCheck(FALSE);

		m_wndGridCtrl.SetColumnWidth(0,MFC_GRIDCTRL_CHECKBOX_WIDTH);
		m_wndGridCtrl.SetColumnWidth(1,100);
		m_wndGridCtrl.SetColumnWidth(2,60);
		m_wndGridCtrl.SetColumnWidth(3,60);
		m_wndGridCtrl.SetColumnWidth(4,100);
		m_wndGridCtrl.SetColumnWidth(5,100);
		m_wndGridCtrl.SetColumnWidth(6,50);
		m_wndGridCtrl.SetColumnWidth(7,80);
		m_wndGridCtrl.SetColumnWidth(8,80);
		m_wndGridCtrl.SetColumnWidth(9,45);
		m_wndGridCtrl.SetColumnWidth(10,45);

	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
		return;
	}
	catch (CFileException* e)
	{
		e->ReportError();
		e->Delete();
		return;
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
		return;
	}
}

void CUdfRtuCfgRightView::InitShowYxInfoHeaders()
{
	try
	{
		CString Headers[10];
		Headers[0] = "";
		Headers[1] = "名称";
		Headers[2] = "遥信码";
		Headers[3] = "修改";
		Headers[4] = "删除";
		/*
		Headers[3] = "地区(市)";
		Headers[4] = "县";
		Headers[5] = "地址类型";
		Headers[6] = "地址码";
		Headers[7] = "通信方式";
		Headers[8] = "通信接口";
		Headers[9] = "通信参数";
		*/

		int fixCols = 1;
		int fixRows = 1;
		int nColsCount = 5;

		m_wndGridCtrl.SetRowCount(1);
		m_wndGridCtrl.SetColumnCount(nColsCount);
		//m_wndGridCtrl.SetFixedColumnCount(fixCols);
		m_wndGridCtrl.SetFixedRowCount(fixRows);

		int idxCol = 0;

		CGridCellBase* pCell = NULL;
		for (idxCol = 0; idxCol < nColsCount; idxCol ++)
		{
			pCell  = m_wndGridCtrl.GetCell(0,idxCol);
			if (pCell)
				pCell->SetText(Headers[idxCol]);
		}

		m_wndGridCtrl.SetCellType(0, 0, RUNTIME_CLASS(CGridCellCheck));
		CGridCellCheck* pGridCellCheck = (CGridCellCheck*)m_wndGridCtrl.GetCell(0, 0);
		if (pGridCellCheck)
			pGridCellCheck->SetCheck(FALSE);

		m_wndGridCtrl.SetColumnWidth(0,MFC_GRIDCTRL_CHECKBOX_WIDTH);
		m_wndGridCtrl.SetColumnWidth(1,100);
		m_wndGridCtrl.SetColumnWidth(2,100);
		m_wndGridCtrl.SetColumnWidth(3,45);
		m_wndGridCtrl.SetColumnWidth(4,45);

	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
		return;
	}
	catch (CFileException* e)
	{
		e->ReportError();
		e->Delete();
		return;
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
		return;
	}
}

void CUdfRtuCfgRightView::InitShowYkInfoHeaders()
{
	try
	{
		CString Headers[10];
		Headers[0] = "";
		Headers[1] = "名称";
		Headers[2] = "遥控码";
		Headers[3] = "修改";
		Headers[4] = "删除";

		int fixCols = 1;
		int fixRows = 1;
		int nColsCount = 5;

		m_wndGridCtrl.SetRowCount(1);
		m_wndGridCtrl.SetColumnCount(nColsCount);
		m_wndGridCtrl.SetFixedRowCount(fixRows);

		int idxCol = 0;

		CGridCellBase* pCell = NULL;
		for (idxCol = 0; idxCol < nColsCount; idxCol ++)
		{
			pCell  = m_wndGridCtrl.GetCell(0,idxCol);
			if (pCell)
				pCell->SetText(Headers[idxCol]);
		}

		m_wndGridCtrl.SetCellType(0, 0, RUNTIME_CLASS(CGridCellCheck));
		CGridCellCheck* pGridCellCheck = (CGridCellCheck*)m_wndGridCtrl.GetCell(0, 0);
		if (pGridCellCheck)
			pGridCellCheck->SetCheck(FALSE);

		m_wndGridCtrl.SetColumnWidth(0,MFC_GRIDCTRL_CHECKBOX_WIDTH);
		m_wndGridCtrl.SetColumnWidth(1,100);
		m_wndGridCtrl.SetColumnWidth(2,100);
		m_wndGridCtrl.SetColumnWidth(3,45);
		m_wndGridCtrl.SetColumnWidth(4,45);
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
		return;
	}
	catch (CFileException* e)
	{
		e->ReportError();
		e->Delete();
		return;
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
		return;
	}
}

void CUdfRtuCfgRightView::InitShowZdInfoHeaders()
{
	try
	{
		CString Headers[10];
		Headers[0] = "";
		Headers[1] = "名称";
		Headers[2] = "整定码";
		Headers[3] = "宽度";
		Headers[4] = "项位序号";
		Headers[5] = "单位";
		Headers[6] = "最大值";
		Headers[7] = "最小值";
		Headers[8] = "修改";
		Headers[9] = "删除";

		int fixCols = 1;
		int fixRows = 1;
		int nColsCount = 10;

		m_wndGridCtrl.SetRowCount(1);
		m_wndGridCtrl.SetColumnCount(nColsCount);
		m_wndGridCtrl.SetFixedRowCount(fixRows);

		int idxCol = 0;

		CGridCellBase* pCell = NULL;
		for (idxCol = 0; idxCol < nColsCount; idxCol ++)
		{
			pCell  = m_wndGridCtrl.GetCell(0,idxCol);
			if (pCell)
				pCell->SetText(Headers[idxCol]);
		}

		m_wndGridCtrl.SetCellType(0, 0, RUNTIME_CLASS(CGridCellCheck));
		CGridCellCheck* pGridCellCheck = (CGridCellCheck*)m_wndGridCtrl.GetCell(0, 0);
		if (pGridCellCheck)
			pGridCellCheck->SetCheck(FALSE);

		m_wndGridCtrl.SetColumnWidth(0,MFC_GRIDCTRL_CHECKBOX_WIDTH);
		m_wndGridCtrl.SetColumnWidth(1,100);
		m_wndGridCtrl.SetColumnWidth(2,70);
		m_wndGridCtrl.SetColumnWidth(3,60);
		m_wndGridCtrl.SetColumnWidth(4,70);
		m_wndGridCtrl.SetColumnWidth(5,60);
		m_wndGridCtrl.SetColumnWidth(6,100);
		m_wndGridCtrl.SetColumnWidth(7,100);
		m_wndGridCtrl.SetColumnWidth(8,45);
		m_wndGridCtrl.SetColumnWidth(9,45);

	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
		return;
	}
	catch (CFileException* e)
	{
		e->ReportError();
		e->Delete();
		return;
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
		return;
	}
}


void CUdfRtuCfgRightView::ShowRtuInfor(GRIDCTRL_STYLE xType, CRtuStation* pStation /* = NULL */)
{
	m_pCurStation = pStation;
	InitGridCtrlHeaders(xType);

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();


    CString  strCaption ="";

	if (pStation == NULL)
	{

		if (xType == RTUST_STINFO_TYPE)
		{
			CPtrList* pLst = pApp->m_pReadDB->GetRtuStList();

			strCaption.Format("遥测终端记录,  %4d条数据",pLst->GetCount());
			if (pLst && !pLst->IsEmpty())
			{
				int nCount = 1;
				POSITION rPos = pLst->GetHeadPosition();
				while ( rPos != NULL)
				{
					CRtuStation* pSub = (CRtuStation*)pLst->GetNext(rPos);
					if (pSub)
					{
						CString  strText = "";
						strText.Format("%d",nCount);
						int idxRow = m_wndGridCtrl.InsertRow(strText);

						ShowRtuInfor(pSub,idxRow);

						nCount ++;
					}
				}
			}
		}
	}
	else if (pStation != NULL)
	{
		if (xType == RTUST_STINFO_TYPE)
		{
			strCaption.Format("%s 的基本信息",pStation->GetName());
			CString  strText = "";
			strText.Format("%d",1);
			int idxRow = m_wndGridCtrl.InsertRow(strText);

			ShowRtuInfor(pStation,idxRow);
		}
		else    //显示遥信、遥测、遥控、定值信息
		{
			CPtrList* pLst = NULL;  //相应记录数据列表
			int nCount = 1;

			if (xType == RTUST_YCINFO_TYPE)   //遥测信息
			{
				pLst = pStation->GetYcObjectsList();
				strCaption.Format("%s  的遥测信息,  %4d条数据",pStation->GetName(),pLst->GetCount());

				if (pLst  && !pLst->IsEmpty())
				{
					POSITION rPos = pLst->GetHeadPosition();
					while (rPos != NULL)
					{
						CRtuYcObject* pYc = (CRtuYcObject*)pLst->GetNext(rPos);
						if (pYc)
						{
							CString strText ="";
							strText.Format("%d",nCount);
							int nRow = m_wndGridCtrl.InsertRow(strText);

							ShowRtuInfor(pYc,nRow);
							nCount ++;
						}
					}
				}

			}
			else if (xType == RTUST_YXINFO_TYPE) //遥信信息
			{
				pLst = pStation->GetYxObjectsList();

				strCaption.Format("%s  的遥信信息,  %4d条数据",pStation->GetName(),pLst->GetCount());

				if (pLst && !pLst->IsEmpty())
				{
					POSITION rPos = pLst->GetHeadPosition();
					while (rPos != NULL)
					{
						CRtuYxObject* pYx = (CRtuYxObject*)pLst->GetNext(rPos);
						if (pYx)
						{
							CString strText ="";
							strText.Format("%d",nCount);
							int nRow = m_wndGridCtrl.InsertRow(strText);

							ShowRtuInfor(pYx,nRow);
							nCount ++;
						}
					}
				}

			}
			else if (xType == RTUST_YKINFO_TYPE) //遥控信息
			{
				pLst = pStation->GetYkObjectsList();
				strCaption.Format("%s  的遥控信息,  %4d条数据",pStation->GetName(),pLst->GetCount());
				if (pLst && !pLst->IsEmpty())
				{
					POSITION rPos = pLst->GetHeadPosition();
					while (rPos != NULL)
					{
						CRtuYkObject* pYk = (CRtuYkObject*)pLst->GetNext(rPos);
						if (pYk)
						{
							CString strText ="";
							strText.Format("%d",nCount);
							int nRow = m_wndGridCtrl.InsertRow(strText);

							ShowRtuInfor(pYk,nRow);
							nCount ++;
						}
					}
				}

			}
			else if (xType == RTUST_ZDINFO_TYPE) //定值信息
			{
				pLst = pStation->GetZdObjectsList();
				strCaption.Format("%s  的定值信息,  %4d条数据",pStation->GetName(),pLst->GetCount());
				
				if (pLst && !pLst->IsEmpty())
				{
					POSITION rPos = pLst->GetHeadPosition();
					while (rPos != NULL)
					{
						CRtuZdObject* pZd = (CRtuZdObject*)pLst->GetNext(rPos);
						if (pZd)
						{
							CString strText ="";
							strText.Format("%d",nCount);
							int nRow = m_wndGridCtrl.InsertRow(strText);

							ShowRtuInfor(pZd,nRow);
							nCount ++;
						}
					}
				}

			}
		}
	}
	SetCaption(strCaption);

}


void CUdfRtuCfgRightView::ShowRtuInfor(CRtuStation*  pSub,int nRow)
{
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();

	if (!pSub)
		return;


	CString strText="";

	GV_ITEM item;
	item.row = nRow;
	item.nFormat =  DT_VCENTER|DT_LEFT;

	item.col = 0;
	item.mask = GVIF_TEXT|GVIF_PARAM;
	m_wndGridCtrl.SetItemData(nRow,0,(LPARAM)pSub);

	m_wndGridCtrl.SetCellType(item.row, item.col, RUNTIME_CLASS(CGridCellCheck));
	CGridCellCheck* pGridCellCheck = (CGridCellCheck*)m_wndGridCtrl.GetCell(item.row, item.col);
	if (pGridCellCheck)
		pGridCellCheck->SetCheck(FALSE);


	item.col = 1;
	item.mask = GVIF_TEXT;
	item.nFormat = DT_LEFT|DT_VCENTER;
	item.strText = pSub->GetName();
	m_wndGridCtrl.SetItem(&item);


	CUdfProvinceCode* pProvinCode = pApp->m_pReadDB->m_AdDivCodeDb.SearchProvinceCodeItem(pSub->GetProvinceCode());
	CString strProv="",strRegion="",strCounty="";
	if (pProvinCode)
	{
		strProv = pProvinCode->GetName();

		CUdfRegionCode* pReginCode = pProvinCode->SearchRegionCodeItem(pSub->GetRegionCode());
		if (pReginCode)
		{
			strRegion = pReginCode->GetName();
			CUdfCountyCode* pCountyCode = pReginCode->SearchCountyCodeItem(pSub->GetCountyCode());
			if (pCountyCode)
			{
				strCounty = pCountyCode->GetName();
			}
		}
	}

	item.col = 2;
	item.strText = strProv;
	m_wndGridCtrl.SetItem(&item);


	item.col = 3;
	item.strText = strRegion;
	m_wndGridCtrl.SetItem(&item);


	item.col = 4;
	item.strText = strCounty;
	m_wndGridCtrl.SetItem(&item);


	if (pSub->GetAddrA3() == 2)
		strText ="终端地址";
	else if (pSub->GetAddrA3() == 5)
		strText ="中继地址";

	item.col = 5;
	item.strText = strText;
	m_wndGridCtrl.SetItem(&item);

	//item.col = 6;
	//item.strText.Format("%d",pSub->GetAddrA2());
	//m_wndGridCtrl.SetItem(&item);


	//item.col = 7;
	//item.strText = CUdfGlobalFUNC::GetCmTypeString(pSub->GetCmType());
	//m_wndGridCtrl.SetItem(&item);

	item.col = 6;
	item.strText.Format("%02d%02d%02d",pSub->GetProvinceCode(),pSub->GetRegionCode(),pSub->GetCountyCode());
	m_wndGridCtrl.SetItem(&item);


	item.col = 7;
	if (pSub->GetAppProtocolType() == (BYTE)WaterResources_Protocol)
	{
		item.strText.Format("%d",pSub->GetAddrA2());
		m_wndGridCtrl.SetItem(&item);
	}
	else if (pSub->GetAppProtocolType() == (BYTE)SCSW_008_Protocol)
	{
		item.strText = pSub->GetStringStCode();
		m_wndGridCtrl.SetItem(&item);
	}
	else if (pSub->GetAppProtocolType() == (BYTE)SYQYC_Comm_Protocol)
	{
		item.strText.Format("%d",pSub->GetSYQStCode());
		m_wndGridCtrl.SetItem(&item);
	}


	item.col = 8;
	if (pSub->GetAppProtocolType() == (BYTE)WaterResources_Protocol)
	{
		item.strText = _T("SL427");
		m_wndGridCtrl.SetItem(&item);
	}
	else if (pSub->GetAppProtocolType() == (BYTE)SCSW_008_Protocol)
	{
		item.strText = _T("SCSW008-2011");
		m_wndGridCtrl.SetItem(&item);
	}
	else if (pSub->GetAppProtocolType() == (BYTE)SYQYC_Comm_Protocol)
	{
		item.strText = _T("水雨情遥测系统数据通信规约");
		m_wndGridCtrl.SetItem(&item);
	}

	item.col = 9;
	if (pSub->GetAppProtocolType() == (BYTE)WaterResources_Protocol)
	{
		item.strText = _T("0");
		m_wndGridCtrl.SetItem(&item);
	}
	else if (pSub->GetAppProtocolType() == (BYTE)SCSW_008_Protocol)
	{
		item.strText = _T("0");
		m_wndGridCtrl.SetItem(&item);
	}
	else if (pSub->GetAppProtocolType() == (BYTE)SYQYC_Comm_Protocol)
	{
		item.strText.Format("%d",pSub->GetSYQCenterCode());
		m_wndGridCtrl.SetItem(&item);
	}


	item.col = 10;
	CGridBtnCell *pbtnCell=NULL;
	m_wndGridCtrl.SetCellType(item.row,item.col,RUNTIME_CLASS(CGridBtnCell));
	pbtnCell = (CGridBtnCell *)m_wndGridCtrl.GetCell(item.row,item.col);
	pSub->m_btnDbEdit.SetGrid(&m_wndGridCtrl);
	pbtnCell->SetBtnDataBase(&pSub->m_btnDbEdit);
	pbtnCell->SetupBtns(0,DFC_BUTTON,DFCS_BUTTONPUSH,CGridBtnCellBase::CTL_ALIGN_CENTER,
		0,FALSE,"修改"); 


	item.col = 11;
	m_wndGridCtrl.SetCellType(item.row,item.col,RUNTIME_CLASS(CGridBtnCell));
	pbtnCell = (CGridBtnCell *)m_wndGridCtrl.GetCell(item.row,item.col);
	pSub->m_btnDbDel.SetGrid(&m_wndGridCtrl);
	pbtnCell->SetBtnDataBase(&pSub->m_btnDbDel);
	pbtnCell->SetupBtns(0,DFC_BUTTON,DFCS_BUTTONPUSH,CGridBtnCellBase::CTL_ALIGN_CENTER,
		0,FALSE,"删除"); 

/*
	item.col = 8;
	item.strText = CUdfGlobalFUNC::GetInterfaceString(pSub->GetInterface());
	m_wndGridCtrl.SetItem(&item);


	item.col = 9;

	if (pSub->GetInterface() == 1)
		strText.Format("%s:%d",pSub->GetIpAddrA(),pSub->GetSockPortA());
	else
		strText.Format("%s,%d,%d,%d,%s",
		pSub->GetComPortA(),
		pSub->GetBaundA(),
		pSub->GetDataBitsA(),
		pSub->GetStopBitsA(),
		CUdfGlobalFUNC::GetParityString(pSub->GetParityA()));
	item.strText =strText;
	m_wndGridCtrl.SetItem(&item);

	item.col = 10;
	CGridBtnCell *pbtnCell=NULL;
	m_wndGridCtrl.SetCellType(item.row,item.col,RUNTIME_CLASS(CGridBtnCell));
	pbtnCell = (CGridBtnCell *)m_wndGridCtrl.GetCell(item.row,item.col);
	pSub->m_btnDbEdit.SetGrid(&m_wndGridCtrl);
	pbtnCell->SetBtnDataBase(&pSub->m_btnDbEdit);
	pbtnCell->SetupBtns(0,DFC_BUTTON,DFCS_BUTTONPUSH,CGridBtnCellBase::CTL_ALIGN_CENTER,
		0,FALSE,"修改"); 


	item.col = 11;
	m_wndGridCtrl.SetCellType(item.row,item.col,RUNTIME_CLASS(CGridBtnCell));
	pbtnCell = (CGridBtnCell *)m_wndGridCtrl.GetCell(item.row,item.col);
	pSub->m_btnDbDel.SetGrid(&m_wndGridCtrl);
	pbtnCell->SetBtnDataBase(&pSub->m_btnDbDel);
	pbtnCell->SetupBtns(0,DFC_BUTTON,DFCS_BUTTONPUSH,CGridBtnCellBase::CTL_ALIGN_CENTER,
		0,FALSE,"删除"); */




	m_wndGridCtrl.SetRowHeight(nRow,MFC_GRIDCTRL_HEIGHT);
}


void CUdfRtuCfgRightView::ShowRtuInfor(CRtuYxObject* pYx,int nRow)
{
	if(!pYx)
		return;

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();


	GV_ITEM item;
	item.row = nRow;
	item.nFormat = DT_LEFT|DT_VCENTER;

	item.col = 0;
	item.mask = GVIF_TEXT|GVIF_PARAM;
	m_wndGridCtrl.SetItemData(nRow,0,(LPARAM)pYx);

	m_wndGridCtrl.SetCellType(item.row, item.col, RUNTIME_CLASS(CGridCellCheck));
	CGridCellCheck* pGridCellCheck = (CGridCellCheck*)m_wndGridCtrl.GetCell(item.row, item.col);
	if (pGridCellCheck)
		pGridCellCheck->SetCheck(FALSE);

	item.col = 1;
	item.mask = GVIF_TEXT;
	item.nFormat = DT_LEFT|DT_VCENTER;
	item.strText = pYx->GetName();
	m_wndGridCtrl.SetItem(&item);

	item.col = 2;
	item.strText.Format("%d",pYx->GetCode());
	m_wndGridCtrl.SetItem(&item);

	item.col = 3;
	CGridBtnCell *pbtnCell=NULL;
	m_wndGridCtrl.SetCellType(item.row,item.col,RUNTIME_CLASS(CGridBtnCell));
	pbtnCell = (CGridBtnCell *)m_wndGridCtrl.GetCell(item.row,item.col);
	pYx->m_btnDbEdit.SetGrid(&m_wndGridCtrl);
	pbtnCell->SetBtnDataBase(&pYx->m_btnDbEdit);
	pbtnCell->SetupBtns(0,DFC_BUTTON,DFCS_BUTTONPUSH,CGridBtnCellBase::CTL_ALIGN_CENTER,
		0,FALSE,"修改"); 


	item.col = 4;
	m_wndGridCtrl.SetCellType(item.row,item.col,RUNTIME_CLASS(CGridBtnCell));
	pbtnCell = (CGridBtnCell *)m_wndGridCtrl.GetCell(item.row,item.col);
	pYx->m_btnDbDel.SetGrid(&m_wndGridCtrl);
	pbtnCell->SetBtnDataBase(&pYx->m_btnDbDel);
	pbtnCell->SetupBtns(0,DFC_BUTTON,DFCS_BUTTONPUSH,CGridBtnCellBase::CTL_ALIGN_CENTER,
		0,FALSE,"删除"); 

	m_wndGridCtrl.SetRowHeight(nRow,MFC_GRIDCTRL_HEIGHT);

}

void CUdfRtuCfgRightView::ShowRtuInfor(CRtuYcObject* pYc,int nRow)
{
	if(!pYc)
		return;

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();


	GV_ITEM item;
	item.row = nRow;
	item.nFormat = DT_LEFT|DT_VCENTER;

	item.col = 0;
	item.mask = GVIF_TEXT|GVIF_PARAM;
	m_wndGridCtrl.SetItemData(nRow,0,(LPARAM)pYc);

	m_wndGridCtrl.SetCellType(item.row, item.col, RUNTIME_CLASS(CGridCellCheck));
	CGridCellCheck* pGridCellCheck = (CGridCellCheck*)m_wndGridCtrl.GetCell(item.row, item.col);
	if (pGridCellCheck)
		pGridCellCheck->SetCheck(FALSE);

	item.col = 1;
	item.mask = GVIF_TEXT;
	item.nFormat = DT_LEFT|DT_VCENTER;
	item.strText = pYc->GetName();
	m_wndGridCtrl.SetItem(&item);

	item.col = 2;
	item.strText.Format("%d",pYc->GetCode());
	m_wndGridCtrl.SetItem(&item);

	item.col = 3;
	item.strText = CRtuYcObject::GetXTypeString(pYc->GetXType());
	m_wndGridCtrl.SetItem(&item);

	item.col = 4;
	item.strText.Format("%d",pYc->GetDisplayBits());
	m_wndGridCtrl.SetItem(&item);


	item.col = 5;
	item.strText.Format("%d",pYc->GetPointBits());
	m_wndGridCtrl.SetItem(&item);


	item.col = 6;
	item.strText = pYc->GetUnit();
	m_wndGridCtrl.SetItem(&item);


	item.col = 7;
	item.strText.Format("%.03f",pYc->GetMaxValue());
	m_wndGridCtrl.SetItem(&item);

	item.col = 8;
	item.strText.Format("%.03f",pYc->GetMinValue());
	m_wndGridCtrl.SetItem(&item);

	item.col = 9;
	CGridBtnCell *pbtnCell=NULL;
	m_wndGridCtrl.SetCellType(item.row,item.col,RUNTIME_CLASS(CGridBtnCell));
	pbtnCell = (CGridBtnCell *)m_wndGridCtrl.GetCell(item.row,item.col);
	pYc->m_btnDbEdit.SetGrid(&m_wndGridCtrl);
	pbtnCell->SetBtnDataBase(&pYc->m_btnDbEdit);
	pbtnCell->SetupBtns(0,DFC_BUTTON,DFCS_BUTTONPUSH,CGridBtnCellBase::CTL_ALIGN_CENTER,
		0,FALSE,"修改"); 


	item.col = 10;
	m_wndGridCtrl.SetCellType(item.row,item.col,RUNTIME_CLASS(CGridBtnCell));
	pbtnCell = (CGridBtnCell *)m_wndGridCtrl.GetCell(item.row,item.col);
	pYc->m_btnDbDel.SetGrid(&m_wndGridCtrl);
	pbtnCell->SetBtnDataBase(&pYc->m_btnDbDel);
	pbtnCell->SetupBtns(0,DFC_BUTTON,DFCS_BUTTONPUSH,CGridBtnCellBase::CTL_ALIGN_CENTER,
		0,FALSE,"删除"); 


	m_wndGridCtrl.SetRowHeight(nRow,MFC_GRIDCTRL_HEIGHT);

}

void CUdfRtuCfgRightView::ShowRtuInfor(CRtuYkObject* pYk,int nRow)
{
	if(!pYk)
		return;

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();


	GV_ITEM item;
	item.row = nRow;
	item.nFormat = DT_LEFT|DT_VCENTER;

	item.col = 0;
	item.mask = GVIF_TEXT|GVIF_PARAM;
	m_wndGridCtrl.SetItemData(nRow,0,(LPARAM)pYk);

	m_wndGridCtrl.SetCellType(item.row, item.col, RUNTIME_CLASS(CGridCellCheck));
	CGridCellCheck* pGridCellCheck = (CGridCellCheck*)m_wndGridCtrl.GetCell(item.row, item.col);
	if (pGridCellCheck)
		pGridCellCheck->SetCheck(FALSE);


	item.col = 1;
	item.mask = GVIF_TEXT;
	item.nFormat = DT_LEFT|DT_VCENTER;
	item.strText = pYk->GetName();
	m_wndGridCtrl.SetItem(&item);

	item.col = 2;
	item.strText.Format("%d",pYk->GetCode());
	m_wndGridCtrl.SetItem(&item);

	item.col = 3;
	CGridBtnCell *pbtnCell=NULL;
	m_wndGridCtrl.SetCellType(item.row,item.col,RUNTIME_CLASS(CGridBtnCell));
	pbtnCell = (CGridBtnCell *)m_wndGridCtrl.GetCell(item.row,item.col);
	pYk->m_btnDbEdit.SetGrid(&m_wndGridCtrl);
	pbtnCell->SetBtnDataBase(&pYk->m_btnDbEdit);
	pbtnCell->SetupBtns(0,DFC_BUTTON,DFCS_BUTTONPUSH,CGridBtnCellBase::CTL_ALIGN_CENTER,
		0,FALSE,"修改"); 


	item.col = 4;
	m_wndGridCtrl.SetCellType(item.row,item.col,RUNTIME_CLASS(CGridBtnCell));
	pbtnCell = (CGridBtnCell *)m_wndGridCtrl.GetCell(item.row,item.col);
	pYk->m_btnDbDel.SetGrid(&m_wndGridCtrl);
	pbtnCell->SetBtnDataBase(&pYk->m_btnDbDel);
	pbtnCell->SetupBtns(0,DFC_BUTTON,DFCS_BUTTONPUSH,CGridBtnCellBase::CTL_ALIGN_CENTER,
		0,FALSE,"删除"); 

	m_wndGridCtrl.SetRowHeight(nRow,MFC_GRIDCTRL_HEIGHT);

}

void CUdfRtuCfgRightView::ShowRtuInfor(CRtuZdObject* pZd,int nRow)
{
	if(!pZd)
		return;

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();


	GV_ITEM item;
	item.row = nRow;
	item.nFormat = DT_LEFT|DT_VCENTER;

	item.col = 0;
	item.mask = GVIF_TEXT|GVIF_PARAM;
	m_wndGridCtrl.SetItemData(nRow,0,(LPARAM)pZd);

	m_wndGridCtrl.SetCellType(item.row, item.col, RUNTIME_CLASS(CGridCellCheck));
	CGridCellCheck* pGridCellCheck = (CGridCellCheck*)m_wndGridCtrl.GetCell(item.row, item.col);
	if (pGridCellCheck)
		pGridCellCheck->SetCheck(FALSE);

	item.col = 1;
	item.nFormat = DT_LEFT|DT_VCENTER;
	item.mask = GVIF_TEXT;	
	item.strText = pZd->GetName();
	m_wndGridCtrl.SetItem(&item);

	item.col = 2;
	item.strText.Format("%d",pZd->GetCode());
	m_wndGridCtrl.SetItem(&item);

	item.col = 3;
	item.strText.Format("%d",pZd->GetSizeof());
	m_wndGridCtrl.SetItem(&item);


	item.col = 4;
	item.strText.Format("%d",pZd->GetOrderInItem());
	m_wndGridCtrl.SetItem(&item);


	item.col = 5;
	item.strText = pZd->GetUnit();
	m_wndGridCtrl.SetItem(&item);


	item.col = 6;
	item.strText.Format("%.03f",pZd->GetScaleMax());
	m_wndGridCtrl.SetItem(&item);


	item.col = 7;
	item.strText.Format("%.03f",pZd->GetScaleMin());
	m_wndGridCtrl.SetItem(&item);

	item.col = 8;
	CGridBtnCell *pbtnCell=NULL;
	m_wndGridCtrl.SetCellType(item.row,item.col,RUNTIME_CLASS(CGridBtnCell));
	pbtnCell = (CGridBtnCell *)m_wndGridCtrl.GetCell(item.row,item.col);
	pZd->m_btnDbEdit.SetGrid(&m_wndGridCtrl);
	pbtnCell->SetBtnDataBase(&pZd->m_btnDbEdit);
	pbtnCell->SetupBtns(0,DFC_BUTTON,DFCS_BUTTONPUSH,CGridBtnCellBase::CTL_ALIGN_CENTER,
		0,FALSE,"修改"); 


	item.col = 9;
	m_wndGridCtrl.SetCellType(item.row,item.col,RUNTIME_CLASS(CGridBtnCell));
	pbtnCell = (CGridBtnCell *)m_wndGridCtrl.GetCell(item.row,item.col);
	pZd->m_btnDbDel.SetGrid(&m_wndGridCtrl);
	pbtnCell->SetBtnDataBase(&pZd->m_btnDbDel);
	pbtnCell->SetupBtns(0,DFC_BUTTON,DFCS_BUTTONPUSH,CGridBtnCellBase::CTL_ALIGN_CENTER,
		0,FALSE,"删除"); 


	m_wndGridCtrl.SetRowHeight(nRow,MFC_GRIDCTRL_HEIGHT);
}



 void CUdfRtuCfgRightView::OnLClickGridCtrl(NMHDR *pNotifyStruct,LRESULT *pResult)
 {
	 NM_GRIDVIEW* pItem =(NM_GRIDVIEW*)pNotifyStruct;

	 if (pItem && pItem->iRow ==0 && pItem->iColumn == 0)
	 {
		 SelectAllItem();
	 }
	 else if (pItem && pItem->iRow >0 && pItem->iColumn == 0)
	 {
		 SelectOneItem(pItem->iRow);
	 }

	 if (pItem && pItem->iRow > 0)
	 {
		 m_wndGridCtrl.SetSelectedRange(-1,-1,-1,-1);  //清空选择焦点
	 }

	 GRIDCTRL_STYLE style  = GetGridCtrlStyle();
	 switch (style)
	 {
	 case RTUST_STINFO_TYPE:
		 OnRtuObjectLClick(pItem);
		 break;
	 case  RTUST_YCINFO_TYPE:
		 OnYcObjectLClick(pItem);
		 break;
	 case  RTUST_YXINFO_TYPE:
		 OnYxObjectLClick(pItem);
		 break;
	 case  RTUST_YKINFO_TYPE:
		 OnYkObjectLClick(pItem);
		 break;
	 case  RTUST_ZDINFO_TYPE:
		 OnZdObjectLClick(pItem);

	 }
 }

 void CUdfRtuCfgRightView::OnLDbClickGridCtrl(NMHDR *pNotifyStruct,LRESULT *pResult)
 {
	 NM_GRIDVIEW* pItem =(NM_GRIDVIEW*)pNotifyStruct;



	 GRIDCTRL_STYLE style  = GetGridCtrlStyle();
	 switch (style)
	 {
	 case RTUST_STINFO_TYPE:
		 OnRtuObjectDbClick(pItem);
		 break;
	 case  RTUST_YCINFO_TYPE:
		 OnYcObjectDbClick(pItem);
		 break;
	 case  RTUST_YXINFO_TYPE:
		 OnYxObjectDbClick(pItem);
		 break;
	 case  RTUST_YKINFO_TYPE:
		 OnYkObjectDbClick(pItem);
		 break;
	 case  RTUST_ZDINFO_TYPE:
		 OnZdObjectDbClick(pItem);

	 }
 }

 void CUdfRtuCfgRightView::OnRClickGridCtrl(NMHDR *pNotifyStruct,LRESULT *pResult)
 {
	 NM_GRIDVIEW* pItem =(NM_GRIDVIEW*)pNotifyStruct;
	

	 
	 ClearSelectObject();
	 if (pItem && pItem->iRow > 0)
	 {
		m_wndGridCtrl.SetSelectedRange(-1,-1,-1,-1);  //清空选择焦点
	 }
	

	 GRIDCTRL_STYLE style  = GetGridCtrlStyle();
	 switch (style)
	 {
	 case RTUST_STINFO_TYPE:
		 OnRtuObjectRClick(pItem);
		 break;
	 case  RTUST_YCINFO_TYPE:
		 OnYcObjectRClick(pItem);
		 break;
	 case  RTUST_YXINFO_TYPE:
		 OnYxObjectRClick(pItem);
		 break;
	 case  RTUST_YKINFO_TYPE:
		 OnYkObjectRClick(pItem);
		 break;
	 case  RTUST_ZDINFO_TYPE:
		 OnZdObjectRClick(pItem);

	 }


 }



 //Left Mouse button Click
 void CUdfRtuCfgRightView::OnRtuObjectLClick(NM_GRIDVIEW* pItem)
 {
	 TRACE("Rtu Object left button click\r\n");
	 if(!pItem)
		 return;

	 if (pItem->iRow <=0 )
		 return;

	 m_wndGridCtrl.SetSelectedRange(pItem->iRow,0,pItem->iRow,(int)RTU_HEADER_COUNT-1);


	 CRtuStation* pSub= (CRtuStation*)m_wndGridCtrl.GetItemData(pItem->iRow,0);
	 if (!pSub)
		 return;

	 if (pItem->iColumn == 10/*10*/) //修改
	 {
		 UpdateRtuObject(pSub);
	 }
	 else if (pItem->iColumn ==11 /*11*/) //删除
	 {
		 DeleteRtuObject(pSub);
	 }

 }

 void CUdfRtuCfgRightView::OnYcObjectLClick(NM_GRIDVIEW* pItem)
 {
	 TRACE("Yc Object left button click\r\n");
	 if(!pItem)
		 return;

	 if (pItem->iRow <=0 )
		 return;

	 m_wndGridCtrl.SetSelectedRange(pItem->iRow,0,pItem->iRow,(int)YC_HEADER_COUNT-1);

	 CRtuYcObject* pYc = (CRtuYcObject*)m_wndGridCtrl.GetItemData(pItem->iRow,0);
	 if (!pYc)
		 return;

	 if (pItem->iColumn == 9) //修改
	 {
		 UpdateYcObject(pYc);
	 }
	 else if (pItem->iColumn == 10) //删除
	 {
		 DeleteYcObject(pYc);
	 }

 }

 void CUdfRtuCfgRightView::OnYxObjectLClick(NM_GRIDVIEW* pItem)
 {
	 TRACE("Yx object left button click\r\n");
	 if(!pItem)
		 return;

	 if (pItem->iRow <=0 )
		 return;

	 m_wndGridCtrl.SetSelectedRange(pItem->iRow,0,pItem->iRow,(int)YX_HEADER_COUNT-1);

	 CRtuYxObject* pYx = (CRtuYxObject*)m_wndGridCtrl.GetItemData(pItem->iRow,0);
	 if (!pYx)
		 return;

	 if (pItem->iColumn == 3)  //修改
	 {
		 UpdateYxObject(pYx);
	 }
	 else if (pItem->iColumn == 4) //删除
	 {
		 DeleteYxObject(pYx);
	 }

 }

 void CUdfRtuCfgRightView::OnYkObjectLClick(NM_GRIDVIEW* pItem)
 {
	 TRACE("Yk object left button click\r\n");
	 if(!pItem)
		 return;

	 if (pItem->iRow <=0 )
		 return;

	 m_wndGridCtrl.SetSelectedRange(pItem->iRow,0,pItem->iRow,(int)YK_HEADER_COUNT-1);

	 CRtuYkObject* pYk = (CRtuYkObject*)m_wndGridCtrl.GetItemData(pItem->iRow,0);
	 if (!pYk)
		 return;

	 if (pItem->iColumn == 3)  //修改
	 {
		 UpdateYkObject(pYk);
	 }
	 else if (pItem->iColumn == 4) //删除
	 {
		 DeleteYkObject(pYk);
	 }

 }

 void CUdfRtuCfgRightView::OnZdObjectLClick(NM_GRIDVIEW* pItem)
 {
	 TRACE("Zd object left button click\r\n");
	 if(!pItem)
		 return;

	 if (pItem->iRow <=0 )
		 return;

	 m_wndGridCtrl.SetSelectedRange(pItem->iRow,0,pItem->iRow,(int)ZD_HEADER_COUNT-1);

	 CRtuZdObject* pZd = (CRtuZdObject*)m_wndGridCtrl.GetItemData(pItem->iRow,0);
	 if (!pZd)
		 return;

	 if (pItem->iColumn == 8 ) //修改
	 {
		 UpdateZdObject(pZd);
	 }
	 else if (pItem->iColumn == 9 )
	 {
		 DeleteZdObject(pZd);
	 }
 }


 //Left mouse button double click
 void CUdfRtuCfgRightView::OnRtuObjectDbClick(NM_GRIDVIEW* pItem)
 {
	 TRACE("Rtu object left button double click\r\n");
     if (pItem == NULL)
		 return;

	 if (pItem->iRow <= 0)
		 return;


	 //if(pItem->iColumn ==0 ||  pItem->iColumn == 10 || pItem->iColumn == 11)
	  if(pItem->iColumn ==0 ||  pItem->iColumn == 8 || pItem->iColumn == 9)
		 return;

	 CCMSApp* pApp = (CCMSApp*)AfxGetApp();

	 CRtuStation* pStation = (CRtuStation*)m_wndGridCtrl.GetItemData(pItem->iRow,0);
	 if (pStation)
	 {
		 UpdateRtuObject(pStation);
	 }
 }

 void CUdfRtuCfgRightView::OnYcObjectDbClick(NM_GRIDVIEW* pItem)
 {
	 TRACE("Yc object left button double click\r\n");
	 if (pItem == NULL)
		 return;

	 if (pItem->iRow <= 0)
		 return;


	 if (pItem->iColumn ==0 ||  pItem->iColumn ==9 || pItem->iColumn == 10)
		 return;

	 CCMSApp* pApp = (CCMSApp*)AfxGetApp();

	 CRtuYcObject* pYc = (CRtuYcObject*)m_wndGridCtrl.GetItemData(pItem->iRow,0);
	 
	 if (pYc)
	 {
		 UpdateYcObject(pYc);

	 }
 }

 void CUdfRtuCfgRightView::OnYxObjectDbClick(NM_GRIDVIEW* pItem)
 {
	 TRACE("Yx object left button double click\r\n");
	 if (pItem == NULL)
		 return;

	 if (pItem->iRow <= 0)
		 return;


	 if (pItem->iColumn ==0 ||  pItem->iColumn == 3|| pItem->iColumn == 4)
		 return;

	 CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	 CRtuYxObject* pYx = (CRtuYxObject*)m_wndGridCtrl.GetItemData(pItem->iRow,0);
	 if (pYx)
	 {
		 UpdateYxObject(pYx);
	 }

 }

 void CUdfRtuCfgRightView::OnYkObjectDbClick(NM_GRIDVIEW* pItem)
 {
	 TRACE("Yk object left button double click\r\n");
	 if (pItem == NULL)
		 return;

	 if (pItem->iRow <= 0)
		 return;

	 if (pItem->iColumn ==0 ||  pItem->iColumn == 3 || pItem->iColumn == 4)
		 return;

	 CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	 CRtuYkObject* pYk = (CRtuYkObject*)m_wndGridCtrl.GetItemData(pItem->iRow,0);
	 if (pYk)
	 {
		 UpdateYkObject(pYk);
	 }

 }

 void CUdfRtuCfgRightView::OnZdObjectDbClick(NM_GRIDVIEW* pItem)
 {
	 TRACE("Zd object left button double click\r\n");
	 if (pItem == NULL)
		 return;

	 if (pItem->iRow <= 0)
		 return;

	 if (pItem->iColumn ==0 ||  pItem->iColumn == 8 || pItem->iColumn == 9 )
		 return ;


	 CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	 CRtuZdObject* pZd = (CRtuZdObject*)m_wndGridCtrl.GetItemData(pItem->iRow,0);
	 if (pZd)
	 {
		 UpdateZdObject(pZd);
	 }
 }


 //Right mouse button click
 void CUdfRtuCfgRightView::OnRtuObjectRClick(NM_GRIDVIEW* pItem)
 {
	 TRACE("Rtu object right button click\r\n");
	 
	 CMenu menu;
	 menu.LoadMenu(IDR_MENU_POP);
     
	 CMenu *popMenu=NULL;
	 CPoint cursorPos;
	 GetCursorPos(&cursorPos);
	 popMenu = menu.GetSubMenu(8);


	 if (pItem == NULL ||(pItem && pItem->iRow <=0))
	 {
	
		 if (popMenu && popMenu->GetSafeHmenu())
		 {
			 popMenu->DeleteMenu(IDM_RTU_DEL,MF_BYCOMMAND);
			 popMenu->DeleteMenu(IDM_RTU_PRO,MF_BYCOMMAND);
		 }
	 }
	 else if (pItem && pItem->iRow >0)
	 {
		 CRtuStation* pSub = (CRtuStation*)m_wndGridCtrl.GetItemData(pItem->iRow,0);
		 m_pSel_RtuStation  = pSub;
         
		 m_wndGridCtrl.SetSelectedRange(pItem->iRow,0,pItem->iRow,(int)RTU_HEADER_COUNT-1);

		 if (popMenu && popMenu->GetSafeHmenu() && pSub == NULL)
		 {
			 popMenu->DeleteMenu(IDM_RTU_DEL,MF_BYCOMMAND);
			 popMenu->DeleteMenu(IDM_RTU_PRO,MF_BYCOMMAND);
		 }
		 else if (popMenu && popMenu->GetSafeHmenu() && pSub != NULL)
		 {
			 popMenu->InsertMenu(4,MF_SEPARATOR|MF_BYPOSITION);
		 }
	 }

	 if (popMenu && popMenu->GetSafeHmenu())
		 popMenu->TrackPopupMenu(TPM_LEFTALIGN,cursorPos.x,cursorPos.y,this);
 }

 void CUdfRtuCfgRightView::OnYcObjectRClick(NM_GRIDVIEW* pItem)
 {
	 TRACE("Yc object right button click\r\n");
	 CMenu menu;
	 CMenu* popMenu = NULL;
	 CPoint cursorPos;

	 GetCursorPos(&cursorPos);
	 menu.LoadMenu(IDR_MENU_POP);
	 popMenu  = menu.GetSubMenu(4);
	 if (pItem == NULL || (pItem && pItem->iRow <= 0))
	 {
		 if (popMenu && popMenu->GetSafeHmenu())
		 {
			 popMenu->DeleteMenu(IDM_YC_DEL,MF_BYCOMMAND);
			 popMenu->DeleteMenu(IDM_YC_PRO,MF_BYCOMMAND);
		 }
	 }
	 else if (pItem  && pItem->iRow > 0)
	 {
		 CRtuYcObject* pYc = (CRtuYcObject*)m_wndGridCtrl.GetItemData(pItem->iRow,0);
		 m_pSel_YcObject = pYc;

		 m_wndGridCtrl.SetSelectedRange(pItem->iRow,0,pItem->iRow,(int)YC_HEADER_COUNT-1);

		 if (popMenu && popMenu->GetSafeHmenu() && pYc == NULL)
		 {
			 popMenu->DeleteMenu(IDM_YC_DEL,MF_BYCOMMAND);
			 popMenu->DeleteMenu(IDM_YC_PRO,MF_BYCOMMAND);
		 }
		 else if (popMenu && popMenu->GetSafeHmenu() && pYc != NULL)
		 {
			 popMenu->InsertMenu(4,MF_SEPARATOR|MF_BYPOSITION);
		 }
	 }

	 if (popMenu && popMenu->GetSafeHmenu())
		 popMenu->TrackPopupMenu(TPM_LEFTALIGN,cursorPos.x,cursorPos.y,this);

 }

 void CUdfRtuCfgRightView::OnYxObjectRClick(NM_GRIDVIEW* pItem)
 {
	 TRACE("Yx object right button click\r\n");
	 CMenu menu,*popMenu=NULL;
	 CPoint cursorPos;

	 menu.LoadMenu(IDR_MENU_POP);
	 popMenu = menu.GetSubMenu(5);
	 GetCursorPos(&cursorPos);

	 if (pItem == NULL || (pItem && pItem->iRow <= 0))
	 {
		 if (popMenu && popMenu->GetSafeHmenu())
		 {
			 popMenu->DeleteMenu(IDM_YX_DEL,MF_BYCOMMAND);
			 popMenu->DeleteMenu(IDM_YX_PRO,MF_BYCOMMAND);
		 }
	 }
	 else if (pItem && pItem->iRow > 0)
	 {
		 CRtuYxObject* pYx = (CRtuYxObject*)m_wndGridCtrl.GetItemData(pItem->iRow,0);
		 m_pSel_YxObject = pYx;

		 m_wndGridCtrl.SetSelectedRange(pItem->iRow,0,pItem->iRow,(int)YX_HEADER_COUNT-1);

		 if (popMenu && popMenu->GetSafeHmenu() && pYx == NULL)
		 {
			 popMenu->DeleteMenu(IDM_YX_DEL,MF_BYCOMMAND);
			 popMenu->DeleteMenu(IDM_YX_PRO,MF_BYCOMMAND);
		 }
		 else if (popMenu && popMenu->GetSafeHmenu() && pYx != NULL)
		 {
			 popMenu->InsertMenu(4,MF_SEPARATOR|MF_BYPOSITION);
		 }
	 }

	 if (popMenu && popMenu->GetSafeHmenu())
		 popMenu->TrackPopupMenu(TPM_LEFTALIGN,cursorPos.x,cursorPos.y,this);

 }

 void CUdfRtuCfgRightView::OnYkObjectRClick(NM_GRIDVIEW* pItem)
 {
	 TRACE("Yk object right button click\r\n");
	 CMenu menu,*popMenu=NULL;
	 CPoint cursorPos ;

	 GetCursorPos(&cursorPos);
	 menu.LoadMenu(IDR_MENU_POP);
	 popMenu = menu.GetSubMenu(6);

	 if (pItem == NULL || (pItem && pItem->iRow <= 0))
	 {
		 if (popMenu && popMenu->GetSafeHmenu())
		 {
			 popMenu->DeleteMenu(IDM_YK_DEL,MF_BYCOMMAND);
			 popMenu->DeleteMenu(IDM_YK_PRO,MF_BYCOMMAND);
		 }
	 }
	 else if (pItem && pItem->iRow > 0 )
	 {
		 CRtuYkObject* pYk = (CRtuYkObject*)m_wndGridCtrl.GetItemData(pItem->iRow,0);
		 m_pSel_YkObject = pYk;

		 m_wndGridCtrl.SetSelectedRange(pItem->iRow,0,pItem->iRow,(int)YK_HEADER_COUNT-1);

		 if (popMenu && popMenu->GetSafeHmenu() && pYk == NULL)
		 {
			 popMenu->DeleteMenu(IDM_YK_DEL,MF_BYCOMMAND);
			 popMenu->DeleteMenu(IDM_YK_PRO,MF_BYCOMMAND);
		 }
		 else if (popMenu && popMenu->GetSafeHmenu() && pYk != NULL)
		 {
			 popMenu->InsertMenu(4,MF_SEPARATOR|MF_BYPOSITION);
		 }

	 }

	 if (popMenu && popMenu->GetSafeHmenu())
		 popMenu->TrackPopupMenu(TPM_LEFTALIGN,cursorPos.x,cursorPos.y,this);
 }

 void CUdfRtuCfgRightView::OnZdObjectRClick(NM_GRIDVIEW* pItem)
 {
	 TRACE("Zd object right button click\r\n");
	 CMenu menu,*popMenu=NULL;
	 CPoint cursorPos ;
	 GetCursorPos(&cursorPos);

	 menu.LoadMenu(IDR_MENU_POP);
	 popMenu = menu.GetSubMenu(7);

	 if (pItem == NULL ||(pItem && pItem->iRow <= 0))
	 {
		 if (popMenu && popMenu->GetSafeHmenu())
		 {
			 popMenu->DeleteMenu(IDM_ZD_DEL,MF_BYCOMMAND);
			 popMenu->DeleteMenu(IDM_ZD_PRO,MF_BYCOMMAND);
		 }
	 }
	 else if (pItem && pItem->iRow > 0)
	 {
		 CRtuZdObject* pZd = (CRtuZdObject*)m_wndGridCtrl.GetItemData(pItem->iRow,0);
		 m_pSel_ZdObject = pZd;

		 m_wndGridCtrl.SetSelectedRange(pItem->iRow,0,pItem->iRow,(int)ZD_HEADER_COUNT-1);

		 if (popMenu && popMenu->GetSafeHmenu() && pZd == NULL)
		 {
			 popMenu->DeleteMenu(IDM_ZD_DEL,MF_BYCOMMAND);
			 popMenu->DeleteMenu(IDM_ZD_PRO,MF_BYCOMMAND);
		 }
		 else if (popMenu && popMenu->GetSafeHmenu() && pZd != NULL)
		 {
			 popMenu->InsertMenu(4,MF_SEPARATOR|MF_BYPOSITION);
		 }
	 }
	 
	 if (popMenu && popMenu->GetSafeHmenu())
		 popMenu->TrackPopupMenu(TPM_LEFTALIGN,cursorPos.x,cursorPos.y,this);

 }



 bool CUdfRtuCfgRightView::UpdateRtuObject(CRtuStation* pSub)  //修改RTU站
 {
	 CCMSApp* pApp= (CCMSApp*)AfxGetApp();

	 CUdfAddRtuStDlg dlg(this);

	 dlg.Update(pSub);

	 if (dlg.DoModal() == IDOK)   //更新
	 {
		 RTU_STATION info;
		 info._strSId = pSub->GetID();
		 info._bCmType = dlg.m_CmType;
		 info._bCountyCode = dlg.m_CountyCode;
		 info._bInterface = dlg.m_Interface;
		 info._bProvinceCode = dlg.m_ProvinceCode;
		 info._bRegionCode = dlg.m_RegionCode;
		 info._CmComInfo.mBaund = dlg.m_ComInfo.mBaund;
		 info._CmComInfo.mComName = dlg.m_ComInfo.mComName;
		 info._CmComInfo.mDataBits = dlg.m_ComInfo.mDataBits;
		 info._CmComInfo.mParity = dlg.m_ComInfo.mParity;
		 info._CmComInfo.mStopBits = dlg.m_ComInfo.mStopBits;
		 info._CmNetInfo.mIpAddress =dlg.m_NetInfo.mIpAddress;
		 info._CmNetInfo.mPort = dlg.m_NetInfo.mPort;
		 info._nAddrA1  = (info._bProvinceCode * 100 + info._bRegionCode);
		 info._nAddrA2  = dlg.m_dwA2;
		 info._nAddrA3  = dlg.m_A3Code;
		 info._strName = dlg.m_sName;

		 info._CmScCommInfo.nFactoryType = dlg.m_nScFactoryType;
		 info._CmScCommInfo.nScAddress = dlg.m_nScAddress;
		 info._nAppProVer = dlg.m_nAppProVer;
		 info._nWorkMode = pSub->GetWorkMode();
		 info._bAppProType = dlg.m_nAppProtoType;
		 info._strStCode = dlg.m_strStCode;
		 info._centercode = dlg.m_syqcenter;
		 info._strSYQADDR = dlg.m_strSYQADDR;
		 info._passWord  = pSub->GetSYQPassword();
		 if (pSub->CheckModify(&info) == true )
		 {
			 if (pApp->m_pReadDB->ModifyStInfo(&info,pSub))
			 {
				 pSub->UpdateConfigInfo(&info);
				 ((CUdfRtuCfgFrame*)GetRtuCfgFrame())->m_pRtuCfgLeftView->InitTreeCtrlItem();
				 ShowRtuInfor(RTUST_STINFO_TYPE,NULL);

				 CMainFrame* pMainFrame = (CMainFrame*)pApp->m_pMainWnd;
				 if (pMainFrame  && pMainFrame->m_pRightView && pMainFrame->m_pRightView->GetSafeHwnd())
					 pMainFrame->m_pRightView->InitGridCtrl();


				 if (pSub &&
					 pSub->GetCmType() != SC_CommChannel &&
					 pSub->GetInterface() == Serial_CommInterface && 
					 pApp->m_pCommServer != NULL)
				 {
					 pApp->m_pCommServer->BuildRelaCommToRtu(pSub);
				 }

				 return true;
			 }
		 }
	 }

	 return false;

 }


 bool CUdfRtuCfgRightView::UpdateYcObject(CRtuYcObject* pYc)   //修改Yc对象
 {

	 CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	 CRtuStation * pSub = pYc->GetParent();
	 CUdfAddYcObjectDlg dlg(this);
	 dlg.Update(pYc);
	 if (dlg.DoModal() == IDOK)
	 {
		 RTU_YCOBJECT info;
		 info._strName = dlg.m_sYCName;
		 info._strSId = pYc->GetParent()->GetID();
		 info._strYcObjID = pYc->GetID();
		 info._Type = dlg.m_YcType;
		 info._DisplayBits = dlg.m_DisplayBits;
		 info._MaxValue  = dlg.m_fScaleMax;
		 info._MinValue = dlg.m_fScaleMin;
		 info._nYcCode = dlg.m_dwYcCode;
		 info._PointBits = dlg.m_PointBits;
		 info._Unit = dlg.m_sUnit;

		 if (pYc->CheckModify(&info) == true)
		 {
			 if (pApp->m_pReadDB->ModifyYcInfo(&info,pYc))
			 {
				 //pSub->UpdateRealDBYc(pYc->GetID(),RECORD_EDIT);
				 pYc->UpdateConfigInfo(&info);
				 ShowRtuInfor(GetGridCtrlStyle(),m_pCurStation);
				 return true;
			 }
		 }
	 }

	 return false;
 }


 bool CUdfRtuCfgRightView::UpdateYxObject(CRtuYxObject* pYx)   //修改Yx对象
 {
	 CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	 CRtuStation* pSub =pYx->GetParent();
	 CUdfAddYxObjectDlg dlg(this);
	 dlg.Update(pYx);
	 if (dlg.DoModal() == IDOK)
	 {
		 RTU_YXOBJECT info;
		 info._nYxCode = dlg.m_dwYxCode;
		 info._strName = dlg.m_strYxName;
		 info._strSId = pSub->GetID();
		 info._strYxObjID = pYx->GetID();

		 if (pYx->CheckModify(&info) == true)
		 {
			 if(pApp->m_pReadDB->ModifyYxInfo(&info,pYx))
			 {
				 //pSub->UpdateRealDBYx(pYx->GetID(),RECORD_EDIT);
				 pYx->UpdateConfigInfo(&info);
				 ShowRtuInfor(GetGridCtrlStyle(),m_pCurStation);
				 return true;
			 }
		 }
	 }

	 return false;
 }

 bool CUdfRtuCfgRightView::UpdateYkObject(CRtuYkObject* pYk)   //修改Yk对象
 {
	 CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	 CRtuStation* pSub = pYk->GetParent();
	 CUdfAddYkObjectDlg dlg(this);
	 dlg.Update(pYk);
	 if (dlg.DoModal() == IDOK)
	 {
		 RTU_YKOBJECT info;
		 info._strName = dlg.m_strYkName;
		 info._nYkCode = dlg.m_wYkCode;
		 info._strSId = pSub->GetID();
		 info._strYkObjID = pYk->GetID();

		 if (pYk->CheckModify(&info) == true)
		 {
			 if (pApp->m_pReadDB->ModifyYkInfo(&info,pYk))
			 {

				 //pSub->UpdateRealDBYk(pYk->GetID(),RECORD_EDIT);
				 pYk->UpdateConfigInfo(&info);
				 ShowRtuInfor(GetGridCtrlStyle(),m_pCurStation);
				 return true;
			 }
		 }
	 }

	 return false;

 }
 bool CUdfRtuCfgRightView::UpdateZdObject(CRtuZdObject* pZd)   //修改Zd对象
 {

	 CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	 CRtuStation* pSub = pZd->GetParent();
	 CUdfAddZdObjectDlg dlg(this);
	 dlg.Update(pZd);
	 if (dlg.DoModal() == IDOK)
	 {
		 RTU_ZDOBJECT info;
		 info._strName = dlg.m_strZdName;
		 info._strSId = pSub->GetID();
		 info._strZdObjID = pZd->GetID();
		 info._nZDCode = dlg.m_dwZdCode;
		 info._Sizeof = dlg.m_bSizeof;
		 info._OrderInItem = dlg.m_OrderInItem;
		 info._Unit = dlg.m_strUnit;
		 info._ScaleMax = dlg.m_fScaleMax;
		 info._ScaleMin = dlg.m_fScaleMin;

		 if (pZd->CheckModify(&info) == true )
		 {
			 if (pApp->m_pReadDB->ModifyZdInfo(&info,pZd))
			 {
				 //pSub->UpdateRealDBZd(pZd->GetID(),RECORD_EDIT);
				 pZd->UpdateConfigInfo(&info);
				 ShowRtuInfor(GetGridCtrlStyle(),m_pCurStation);
				 return true;
			 }
		 }
	 }

	 return false;
 }




 bool CUdfRtuCfgRightView::DeleteRtuObject(CRtuStation* pSub) //删除RTU站对象
 {
	 CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	 CMainFrame* pMainFrame = (CMainFrame*)pApp->m_pMainWnd;

	 if (pSub)
	 {
		 CString strMessage="";
		 strMessage.Format("确定删除站'%s'及其所有数据信息?",pSub->GetName());
		 int ret = MessageBox(strMessage,"",MB_OKCANCEL|MB_ICONQUESTION);
		 if (ret == IDOK)
			{
				if (pApp->m_pReadDB->DeleteStInfo(pSub))
				{

					if (pApp->Get_Cur_Op_Station() == pSub)
						pApp->Set_Cur_Op_Station(NULL);

					if (pMainFrame->GetRtuPropertyDlg()->GetRtuStation() == pSub)
						pMainFrame->GetRtuPropertyDlg()->SetRtuStation(NULL);

					pApp->m_pReadDB->UpdateRtusRealDB(pSub->GetID(),RECORD_DEL);
					((CUdfRtuCfgFrame*)GetRtuCfgFrame())->m_pRtuCfgLeftView->InitTreeCtrlItem();
					ShowRtuInfor(RTUST_STINFO_TYPE,NULL);

				
					if (pMainFrame  && pMainFrame->m_pRightView && pMainFrame->m_pRightView->GetSafeHwnd())
						pMainFrame->m_pRightView->InitGridCtrl();

					return true;

				}
			}
	 }

	 return false;
 }

 bool CUdfRtuCfgRightView::DeleteYcObject(CRtuYcObject* pYc)  //删除Yc对象
 {
	 CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	 if (pYc)
	 {
		 CString strMessage ="";
		 strMessage.Format("确定删除站'%s'下名为'%s'的遥测对象信息?",pYc->GetParent()->GetName(),pYc->GetName());
		 int ret = MessageBox(strMessage,"",MB_OKCANCEL|MB_ICONQUESTION);

		 CRtuStation* pParent = pYc->GetParent();
		 if (ret == IDOK)
		 {
			 if (pApp->m_pReadDB->DeleteYcInfo(pYc))
			 {
				 pParent->UpdateRealDBYc(pYc->GetID(),RECORD_DEL);
				 ShowRtuInfor(GetGridCtrlStyle(),m_pCurStation);
				 return true;
			 }
		 }
	 }

	 return false;
 }

 bool CUdfRtuCfgRightView::DeleteYxObject(CRtuYxObject* pYx)  //删除Yx对象
 {
	 CCMSApp* pApp = (CCMSApp*)AfxGetApp();

	 if (pYx)
	 {
		 CString strMessage = "";
		 strMessage.Format("确定删除站'%s'下名为'%s'的遥信对象信息?",pYx->GetParent()->GetName(),pYx->GetName());
		 int ret = MessageBox(strMessage,"",MB_OKCANCEL|MB_ICONQUESTION);

		 CRtuStation* pParent = pYx->GetParent();

		 if (ret == IDOK)
		 {
			 if (pApp->m_pReadDB->DeleteYxInfo(pYx))
			 {
				 pParent->UpdateRealDBYx(pYx->GetID(),RECORD_DEL);
				 ShowRtuInfor(GetGridCtrlStyle(),m_pCurStation);
				 return true;
			 }
		 }
	 }

	 return false;
 }

 bool CUdfRtuCfgRightView::DeleteYkObject(CRtuYkObject* pYk)  //删除Yk对象
 {
	 CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	 if (pYk)
	 {
		 CString strMessage = "";
		 strMessage.Format("确定删除站'%s'下名为'%s'的遥控对象信息?",pYk->GetParent()->GetName(),pYk->GetName());
		 int ret = MessageBox(strMessage,"",MB_OKCANCEL|MB_ICONQUESTION);

		 CRtuStation* pParent = pYk->GetParent();
		 if (ret == IDOK)
		 {
			 if (pApp->m_pReadDB->DeleteYkInfo(pYk))
			 {
				 pParent->UpdateRealDBYk(pYk->GetID(),RECORD_DEL);
				 ShowRtuInfor(GetGridCtrlStyle(),m_pCurStation);
				 return true;
			 }
		 }
	 }

	 return false;
 }

 bool CUdfRtuCfgRightView::DeleteZdObject(CRtuZdObject* pZd)  //删除Zd对象
 {
	 CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	 if (pZd)
	 {
		 CString strMessage="";
		 strMessage.Format("确定删除站'%s'下名为'%s'的定值对象信息?",pZd->GetParent()->GetName(),pZd->GetName());
		 int ret = MessageBox(strMessage,"",MB_OKCANCEL|MB_ICONQUESTION);

		 CRtuStation* pParent = pZd->GetParent();
		 if (ret == IDOK)
		 {
			 if (pApp->m_pReadDB->DeleteZdInfo(pZd))
			 {
				 pParent->UpdateRealDBZd(pZd->GetID(),RECORD_DEL);
				 ShowRtuInfor(GetGridCtrlStyle(),m_pCurStation);
				 return true;
			 }
		 }
	 }

	 return false;
 }


 bool CUdfRtuCfgRightView::AddRtuObject()                      //添加RTU站对象
 {
	 CCMSApp* pApp =(CCMSApp*)AfxGetApp();

	 CUdfAddRtuStDlg dlg(this);
	 if (dlg.DoModal() == IDOK)
	 {
		 RTU_STATION info;
		 info._bCmType = dlg.m_CmType;
		 info._bCountyCode = dlg.m_CountyCode;
		 info._bInterface = dlg.m_Interface;
		 info._bProvinceCode = dlg.m_ProvinceCode;
		 info._bRegionCode = dlg.m_RegionCode;
		 info._CmComInfo.mBaund = dlg.m_ComInfo.mBaund;
		 info._CmComInfo.mComName = dlg.m_ComInfo.mComName;
		 info._CmComInfo.mDataBits = dlg.m_ComInfo.mDataBits;
		 info._CmComInfo.mParity = dlg.m_ComInfo.mParity;
		 info._CmComInfo.mStopBits = dlg.m_ComInfo.mStopBits;
		 info._CmNetInfo.mIpAddress =dlg.m_NetInfo.mIpAddress;
		 info._CmNetInfo.mPort = dlg.m_NetInfo.mPort;
		 info._nAddrA1  = (info._bProvinceCode * 100 + info._bRegionCode);
		 info._nAddrA2  = dlg.m_dwA2;
		 info._nAddrA3  = dlg.m_A3Code;
		 info._strName = dlg.m_sName;

		 info._CmScCommInfo.nFactoryType = dlg.m_nScFactoryType;
		 info._CmScCommInfo.nScAddress = dlg.m_nScAddress;
		 info._nAppProVer = dlg.m_nAppProVer;
		 info._bAppProType = dlg.m_nAppProtoType;
		 info._strStCode = dlg.m_strStCode;
		 info._centercode = dlg.m_syqcenter;
		 info._strSYQADDR = dlg.m_strSYQADDR;

		 CString new_RTUID = "";
		 if (pApp->m_pReadDB->AddRtuStInfo(&info,new_RTUID))
		 {
			 pApp->m_pReadDB->UpdateRtusRealDB(new_RTUID,RECORD_NEW);
			 CUdfRtuCfgFrame* pFrame = (CUdfRtuCfgFrame*)GetRtuCfgFrame();
			 pFrame->m_pRtuCfgLeftView->InitTreeCtrlItem();
			 ShowRtuInfor(RTUST_STINFO_TYPE,NULL);

			 CMainFrame* pMainFrame = (CMainFrame*)pApp->m_pMainWnd;
			 if (pMainFrame  && pMainFrame->m_pRightView && pMainFrame->m_pRightView->GetSafeHwnd())
				 pMainFrame->m_pRightView->InitGridCtrl();


			 CRtuStation* pSub = pApp->m_pReadDB->SearchRtuStation(new_RTUID);
			 if (pSub && pSub->GetInterface() == Serial_CommInterface && 
				 pApp->m_pCommServer != NULL)
			 {
				 pApp->m_pCommServer->BuildRelaCommToRtu(pSub);
			 }
			 

			 return true;
		 }
	 }
	
	 return false;
 }

 bool CUdfRtuCfgRightView::AddYcObject(CRtuStation* pSub)      //向站Rtu添加yc对象
 {
	 if (!pSub)  return false;

	 CCMSApp* pApp =(CCMSApp*)AfxGetApp();

	 CUdfAddYcObjectDlg dlg(this);
	 dlg.SetRtuStation(pSub);
	 if (dlg.DoModal() == IDOK)
	 {
		 RTU_YCOBJECT info;
		 info._strName = dlg.m_sYCName;
		 info._DisplayBits = dlg.m_DisplayBits;
		 info._MaxValue  = dlg.m_fScaleMax;
		 info._MinValue = dlg.m_fScaleMin;
		 info._nYcCode = dlg.m_dwYcCode;
		 info._PointBits = dlg.m_PointBits;
		 info._strSId = pSub->GetID();
		 info._Type = dlg.m_YcType;
		 info._Unit = dlg.m_sUnit;

		 CString new_ycID = "";   
		 if (pApp->m_pReadDB->AddRtuYcInfo(&info,new_ycID))
		 {
			 pSub->UpdateRealDBYc(new_ycID,RECORD_NEW);
			 ShowRtuInfor(GetGridCtrlStyle(),m_pCurStation);
			 return true;
		 }
	 }

	 return false;
 }

 bool CUdfRtuCfgRightView::AddYxObject(CRtuStation* pSub)      //向站Rtu添加yx对象
 {

	 if (!pSub)		return false;

	 CCMSApp* pApp =(CCMSApp*)AfxGetApp();

	 CUdfAddYxObjectDlg dlg(this);
	 dlg.SetRtuStation(pSub);
	 if (dlg.DoModal() == IDOK)
	 {
		 RTU_YXOBJECT info;
		 info._nYxCode = dlg.m_dwYxCode;
		 info._strName = dlg.m_strYxName;
		 info._strSId = pSub->GetID();
		 
		 CString new_yxID = "";
		 if (pApp->m_pReadDB->AddRtuYxInfo(&info,new_yxID))
		 {
			 pSub->UpdateRealDBYx(new_yxID,RECORD_NEW);
			 ShowRtuInfor(GetGridCtrlStyle(),m_pCurStation);
			 return true;
		 }
	 }

	 return false;
 }

 bool CUdfRtuCfgRightView::AddYkObject(CRtuStation* pSub)      //向站Rtu添加yk对象
 {
	 if (!pSub)
		 return false;

	 CCMSApp* pApp =(CCMSApp*)AfxGetApp();

	 CUdfAddYkObjectDlg dlg(this);
	 dlg.SetRtuStation(pSub);
	 if (dlg.DoModal() == IDOK)
	 {
		 RTU_YKOBJECT info;
		 info._nYkCode = dlg.m_wYkCode;
		 info._strName = dlg.m_strYkName;
		 info._strSId = pSub->GetID();

		 CString new_ykID = "";
		 if (pApp->m_pReadDB->AddRtuYkInfo(&info,new_ykID))
		 {
			 pSub->UpdateRealDBYk(new_ykID,RECORD_NEW);
			 ShowRtuInfor(GetGridCtrlStyle(),m_pCurStation);
			 return true;
		 }
	 }

	 return false;
 }

 bool CUdfRtuCfgRightView::AddZdObject(CRtuStation* pSub)      //向站Rtu添加Zd对象
 {
	 if (!pSub)
		 return false;

	 CCMSApp* pApp =(CCMSApp*)AfxGetApp();

	 CUdfAddZdObjectDlg dlg(this);
	 dlg.SetRtuStation(pSub);
	 if (dlg.DoModal() == IDOK)
	 {
		 RTU_ZDOBJECT info;
		 info._nZDCode = dlg.m_dwZdCode;
		 info._OrderInItem = dlg.m_OrderInItem;
		 info._ScaleMax  = dlg.m_fScaleMax;
		 info._ScaleMin = dlg.m_fScaleMin;
		 info._Sizeof = dlg.m_bSizeof;
		 info._strName = dlg.m_strZdName;
		 info._Unit = dlg.m_strUnit;
		 info._strSId = pSub->GetID();


		 CString new_zdID = "";
		 if (pApp->m_pReadDB->AddRtuZdInfo(&info,new_zdID))
		 {
			 pSub->UpdateRealDBZd(new_zdID,RECORD_NEW);
			 ShowRtuInfor(GetGridCtrlStyle(),m_pCurStation);
			 return true;
		 }
	 }

	 return false;
 }

 void CUdfRtuCfgRightView::SelectAllItem()
 {
	 if (m_wndGridCtrl.GetSafeHwnd())
	 {
		 int nCountItems = m_wndGridCtrl.GetRowCount();

		 CGridCellCheck* pCell = (CGridCellCheck*)m_wndGridCtrl.GetCell(0,0);
		 BOOL blCheck = pCell->GetCheck();
		 BOOL blItem = FALSE;
		 if (blCheck)
		 {
			 pCell->SetCheck(FALSE);
			 blItem = FALSE;
		 }
		 else
		 {
			 pCell->SetCheck(TRUE);
			 blItem = TRUE;
		 }

		 if (nCountItems > 1)
		 {
			 for (int nIdx = 1; nIdx<nCountItems; nIdx ++)
			 {
				 pCell = (CGridCellCheck*)m_wndGridCtrl.GetCell(nIdx,0);
				 if (pCell)
					 pCell->SetCheck(blItem);
			 }
		 }

	 }
 }

void CUdfRtuCfgRightView::SelectAllItem(BOOL blSeled)
{
	if (m_wndGridCtrl.GetSafeHwnd())
	{
		int nCountItems = m_wndGridCtrl.GetRowCount();

		CGridCellCheck* pCell = (CGridCellCheck*)m_wndGridCtrl.GetCell(0,0);
		if (pCell)
			pCell->SetCheck(blSeled);

		if (nCountItems > 1)
		{
			for (int nIdx = 1; nIdx<nCountItems; nIdx ++)
			{
				pCell = (CGridCellCheck*)m_wndGridCtrl.GetCell(nIdx,0);
				if (pCell)
					pCell->SetCheck(blSeled);
			}
		}

	}
}


 void CUdfRtuCfgRightView::SelectOneItem(int nRow)
 {
	 CGridCellCheck* pCell = (CGridCellCheck*)m_wndGridCtrl.GetCell(nRow,0);
	 if (!pCell)
		 return;

	 BOOL blCheck = pCell->GetCheck();
	 BOOL blItem = FALSE;
	 if (blCheck)
	 {
		 pCell->SetCheck(FALSE);
		 blItem = FALSE;
	 }
	 else
	 {
		 pCell->SetCheck(TRUE);
		 blItem = TRUE;
	 }
 }


 void CUdfRtuCfgRightView::ClearSelectObject()
 {
	 m_pSel_RtuStation = NULL;
	 m_pSel_YcObject   = NULL;
	 m_pSel_YxObject   = NULL;
	 m_pSel_YkObject   = NULL;
	 m_pSel_ZdObject   = NULL;
 }

 int CUdfRtuCfgRightView::GetSelectedItemsCount()
 {
	 int result = 0;
	 if (m_wndGridCtrl.GetSafeHwnd())
	 {
		 int nRowsCount = m_wndGridCtrl.GetRowCount();
		 if (nRowsCount <= 1)
			 return 0;

		 for (int idx =1; idx < nRowsCount; idx++)
		 {
			 CGridCellCheck* pCell = (CGridCellCheck*)m_wndGridCtrl.GetCell(idx,0);
			 if (pCell && pCell->GetCheck() != FALSE)
				 result ++;
		 
		 }

	 }
	 return result;
 }

void CUdfRtuCfgRightView::OnRtuSelAllItem()
{
	SelectAllItem(TRUE);
}
void CUdfRtuCfgRightView::OnRtuNoSelAllItem()
{
	SelectAllItem(FALSE);
}

void CUdfRtuCfgRightView::OnRtuAddOneItem()
{
	AddRtuObject();
}

void CUdfRtuCfgRightView::OnRtuDelItem()
{
	if (m_pSel_RtuStation)
	{
		DeleteRtuObject(m_pSel_RtuStation);
		m_pSel_RtuStation = NULL;
	}
}

void CUdfRtuCfgRightView::OnRtuDelSelItem()
{
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	CMainFrame* pMainFrame = (CMainFrame*)pApp->m_pMainWnd;

	int nSelCount = GetSelectedItemsCount();
	TRACE("已选择项数：%d\r\n",nSelCount);

	if (nSelCount <= 0)
		return;
	else
	{
		CString message="";
		message.Format("确定删除已选择的'%d'项对象的所有数据信息?",nSelCount);
		int ret = MessageBox(message,"",MB_OKCANCEL|MB_ICONQUESTION);
		if (ret == IDOK)
		{

			CPtrList lst;   //获取被选择项列表
			int nRowsCount = m_wndGridCtrl.GetRowCount();
			for (int idx =1; idx < nRowsCount; idx++)
			{
				CGridCellCheck* pCell = (CGridCellCheck*)m_wndGridCtrl.GetCell(idx,0);
				if (pCell && pCell->GetCheck() != FALSE)
				{
					CRtuStation* pSub = (CRtuStation*)m_wndGridCtrl.GetItemData(idx,0);
					if (pSub)
					{
						lst.AddTail(pSub);
					}
				}
			 }

			while (!lst.IsEmpty())
			{
				CRtuStation* pSub = (CRtuStation*)lst.RemoveHead();
				if (pSub)
				{
					if (pApp->Get_Cur_Op_Station() == pSub)
						pApp->Set_Cur_Op_Station(NULL);

					if (pMainFrame->GetRtuPropertyDlg()->GetRtuStation() == pSub)
						pMainFrame->GetRtuPropertyDlg()->SetRtuStation(NULL);

					pApp->m_pReadDB->DeleteStInfo(pSub);
					pApp->m_pReadDB->UpdateRtusRealDB(pSub->GetID(),RECORD_DEL);
				}
			}

			CUdfRtuCfgFrame* pFrame = (CUdfRtuCfgFrame*)GetRtuCfgFrame();
			pFrame->m_pRtuCfgLeftView->InitTreeCtrlItem();
			ShowRtuInfor(RTUST_STINFO_TYPE,NULL);
	
			if (pMainFrame  && pMainFrame->m_pRightView && pMainFrame->m_pRightView->GetSafeHwnd())
				pMainFrame->m_pRightView->InitGridCtrl();

		}
	}

}

void CUdfRtuCfgRightView::OnUpdateUIRtuDelSelItem(CCmdUI* pCmdUI)
{
	int nSelCount = GetSelectedItemsCount();
	if (nSelCount > 0)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable( FALSE);
}

void CUdfRtuCfgRightView::OnRtuProItem()
{
	if (m_pSel_RtuStation)
	{
		UpdateRtuObject(m_pSel_RtuStation);
		m_pSel_RtuStation = NULL;
	}
}

void CUdfRtuCfgRightView::OnYcSelAllItem()
{
	SelectAllItem(TRUE);
}

void CUdfRtuCfgRightView::OnYcNoSelAllItem()
{
	SelectAllItem(FALSE);
}

void CUdfRtuCfgRightView::OnYcAddOneItem()
{
	if (m_pCurStation)
		AddYcObject(m_pCurStation);
}

void CUdfRtuCfgRightView::OnYcDelSelItem()
{
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	int nSelCount = GetSelectedItemsCount();
	if (nSelCount)
	{
		CString message ="";
		message.Format("确定删除已选择的'%d'项遥测对象的所有数据信息?",nSelCount);
		int ret = MessageBox(message,"",MB_OKCANCEL|MB_ICONQUESTION);
		if (ret == IDOK)
		{
			CPtrList lst;   //获取被选择项列表
			int nRowsCount = m_wndGridCtrl.GetRowCount();
			for (int idx =1; idx < nRowsCount; idx++)
			{
				CGridCellCheck* pCell = (CGridCellCheck*)m_wndGridCtrl.GetCell(idx,0);
				if (pCell && pCell->GetCheck() != FALSE)
				{
					///////////////////////////////////
					CRtuYcObject* pYc =(CRtuYcObject*)m_wndGridCtrl.GetItemData(idx,0);
					if (pYc)
						lst.AddTail(pYc);

				}
			}

			while (!lst.IsEmpty())
			{
                /////////////////////////////////////
				CRtuYcObject* pYc = (CRtuYcObject*)lst.RemoveHead();
				if (pYc)
				{
					if(pApp->m_pReadDB->DeleteYcInfo(pYc) && m_pCurStation)
					{
						m_pCurStation->UpdateRealDBYc(pYc->GetID(),RECORD_DEL);
					}
				}
			}

			if (m_pCurStation)
			{
				ShowRtuInfor(GetGridCtrlStyle(),m_pCurStation);
			}
		}
	}
}

void CUdfRtuCfgRightView::OnYcDelItem()
{
	if (m_pSel_YcObject)
	{
		DeleteYcObject(m_pSel_YcObject);
		m_pSel_YcObject  = NULL;
	}
}

void CUdfRtuCfgRightView::OnYcProItem()
{
	if (m_pSel_YcObject)
	{
		UpdateYcObject(m_pSel_YcObject);
		m_pSel_YcObject = NULL;
	}

}

void CUdfRtuCfgRightView::OnYxSelAllItem()
{
	SelectAllItem(TRUE);
}

void CUdfRtuCfgRightView::OnYxNoSelAllItem()
{
	SelectAllItem(FALSE);
}

void CUdfRtuCfgRightView::OnYxAddOneItem()
{
	if (m_pCurStation)
		AddYxObject(m_pCurStation);
}

void CUdfRtuCfgRightView::OnYxDelSelItem()
{
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	int nSelCount = GetSelectedItemsCount();
	if (nSelCount)
	{
		CString message ="";
		message.Format("确定删除已选择的'%d'项遥信对象的所有数据信息?",nSelCount);
		int ret = MessageBox(message,"",MB_OKCANCEL|MB_ICONQUESTION);
		if (ret == IDOK)
		{
			CPtrList lst;   //获取被选择项列表
			int nRowsCount = m_wndGridCtrl.GetRowCount();
			for (int idx =1; idx < nRowsCount; idx++)
			{
				CGridCellCheck* pCell = (CGridCellCheck*)m_wndGridCtrl.GetCell(idx,0);
				if (pCell && pCell->GetCheck() != FALSE)
				{
					///////////////////////////////////
					CRtuYxObject* pYx = (CRtuYxObject*)m_wndGridCtrl.GetItemData(idx,0);
					if (pYx)
						lst.AddTail(pYx);
				}
			}

			while (!lst.IsEmpty())
			{
				/////////////////////////////////////
				CRtuYxObject* pYx = (CRtuYxObject*)lst.RemoveHead();
				if (pYx)
				{
					if (pApp->m_pReadDB->DeleteYxInfo(pYx)  && m_pCurStation)
					{
						m_pCurStation->UpdateRealDBYx(pYx->GetID(),RECORD_DEL);
					}
				}
			}

			if (m_pCurStation)
			{
				ShowRtuInfor(GetGridCtrlStyle(),m_pCurStation);
			}
		}
	}
}

void CUdfRtuCfgRightView::OnYxDelItem()
{
	if (m_pSel_YxObject)
	{
		DeleteYxObject(m_pSel_YxObject);
		m_pSel_YxObject = NULL;
	}
}

void CUdfRtuCfgRightView::OnYxProItem()
{
	if (m_pSel_YxObject)
	{
		UpdateYxObject(m_pSel_YxObject);
		m_pSel_YxObject= NULL;
	}
}

void CUdfRtuCfgRightView::OnYkSelAllItem()
{
	SelectAllItem(TRUE);
}

void CUdfRtuCfgRightView::OnYkNoSelAllItem()
{
	SelectAllItem(FALSE);
}

void CUdfRtuCfgRightView::OnYkAddOneItem()
{
	if (m_pCurStation)
		AddYkObject(m_pCurStation);
}

void CUdfRtuCfgRightView::OnYkDelSelItem()
{
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	int nSelCount = GetSelectedItemsCount();
	if (nSelCount)
	{
		CString message ="";
		message.Format("确定删除已选择的'%d'项遥控对象的所有数据信息?",nSelCount);
		int ret = MessageBox(message,"",MB_OKCANCEL|MB_ICONQUESTION);
		if (ret == IDOK)
		{
			CPtrList lst;   //获取被选择项列表
			int nRowsCount = m_wndGridCtrl.GetRowCount();
			for (int idx =1; idx < nRowsCount; idx++)
			{
				CGridCellCheck* pCell = (CGridCellCheck*)m_wndGridCtrl.GetCell(idx,0);
				if (pCell && pCell->GetCheck() != FALSE)
				{
					///////////////////////////////////
					CRtuYkObject* pYk = (CRtuYkObject*)m_wndGridCtrl.GetItemData(idx,0);
					if (pYk)
						lst.AddTail(pYk);
				}
			}

			while (!lst.IsEmpty())
			{
				/////////////////////////////////////
				CRtuYkObject* pYk = (CRtuYkObject*)lst.RemoveHead();
				if (pYk)
				{
					if (pApp->m_pReadDB->DeleteYkInfo(pYk))
					{
						if (m_pCurStation)
							m_pCurStation->UpdateRealDBYk(pYk->GetID(),RECORD_DEL);
					}
				}
			}

			if (m_pCurStation)
			{
				ShowRtuInfor(GetGridCtrlStyle(),m_pCurStation);
			}
		}
	}
}

void CUdfRtuCfgRightView::OnYkDelItem()
{
	if (m_pSel_YkObject)
	{
		DeleteYkObject(m_pSel_YkObject);
		m_pSel_YkObject = NULL;
	}
}

void CUdfRtuCfgRightView::OnYkProItem()
{
	if (m_pSel_YkObject)
	{
		UpdateYkObject(m_pSel_YkObject);
		m_pSel_YkObject = NULL;
	}
}


void CUdfRtuCfgRightView::OnZdSelAllItem()
{
	SelectAllItem(TRUE);
}
void CUdfRtuCfgRightView::OnZdNoSelAllItem()
{
	SelectAllItem(FALSE);
}

void CUdfRtuCfgRightView::OnZdAddOneItem()
{
	if(m_pCurStation)
		AddZdObject(m_pCurStation);
}

void CUdfRtuCfgRightView::OnZdDelSelItem()
{
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	int nSelCount = GetSelectedItemsCount();
	if (nSelCount)
	{
		CString message ="";
		message.Format("确定删除已选择的'%d'项定值对象的所有数据信息?",nSelCount);
		int ret = MessageBox(message,"",MB_OKCANCEL|MB_ICONQUESTION);
		if (ret == IDOK)
		{
			CPtrList lst;   //获取被选择项列表
			int nRowsCount = m_wndGridCtrl.GetRowCount();
			for (int idx =1; idx < nRowsCount; idx++)
			{
				CGridCellCheck* pCell = (CGridCellCheck*)m_wndGridCtrl.GetCell(idx,0);
				if (pCell && pCell->GetCheck() != FALSE)
				{
					///////////////////////////////////
					CRtuZdObject* pZd = (CRtuZdObject*)m_wndGridCtrl.GetItemData(idx,0);
					if (pZd)
						lst.AddTail(pZd);

				}
			}

			while (!lst.IsEmpty())
			{
				/////////////////////////////////////
				CRtuZdObject* pZd = (CRtuZdObject*)lst.RemoveHead();
				if (pZd)
				{
					if (pApp->m_pReadDB->DeleteZdInfo(pZd) && m_pCurStation)
					{
						m_pCurStation->UpdateRealDBZd(pZd->GetID(),RECORD_DEL);
					}
				}
			}

			if (m_pCurStation)
			{
				ShowRtuInfor(GetGridCtrlStyle(),m_pCurStation);
			}
		}

	}
}

void CUdfRtuCfgRightView::OnZdDelItem()
{
	if (m_pSel_ZdObject)
	{
		DeleteZdObject(m_pSel_ZdObject);
		m_pSel_ZdObject = NULL;
	}
}

void CUdfRtuCfgRightView::OnZdProItem()
{
	if (m_pSel_ZdObject)
	{
		UpdateZdObject(m_pSel_ZdObject);
		m_pSel_ZdObject = NULL;
	}
}