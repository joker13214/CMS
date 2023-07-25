// RtuAttrDisplacement.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuAttrDisplacement.h"
#include "RtuStation.h"
#include "RtuPropertyDlg.h"

// CRtuAttrDisplacement 对话框
#define  W_TIMER_DISPLACELIMIT_CALL		0x01      //位移上下限值查询Timer
#define  W_TIMER_QUERY_DISPLACEDATA       0x02      //位移实时值查询




IMPLEMENT_DYNAMIC(CRtuAttrDisplacement, CDialog)

CRtuAttrDisplacement::CRtuAttrDisplacement(CWnd* pParent /*=NULL*/)
	: CDialog(CRtuAttrDisplacement::IDD, pParent)
	, m_nDisplaceCount(0)
{
	m_pSubStation = NULL;
	m_pParentDlg =  NULL;
	m_blResult = FALSE;
}

CRtuAttrDisplacement::~CRtuAttrDisplacement()
{
}

void CRtuAttrDisplacement::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_DISPLACECOUNT, m_wndDisplacePCount);
	DDX_Control(pDX, IDC_COMBO_DISPLACEPARA, m_wndDisplaceCmb);
	DDX_Control(pDX, IDC_BTN_DISPLACECALL, m_btnDisplaceLCall);
	DDX_Control(pDX, IDC_BTN_DISPLACESET, m_btnDisplaceLSet);
	DDX_Control(pDX, IDC_STATIC_DISPLACECAPTION, m_wndDisplaceCaption);
	DDX_Control(pDX, IDC_BTN_DISPLACEOK, m_btnDisplaceOK);
	DDX_Text(pDX, IDC_EDIT_DISPLACECOUNT, m_nDisplaceCount);
	DDX_Control(pDX, IDC_BTN_QUERYDISPLACEREAL, m_btnQueryAReal);
}


BEGIN_MESSAGE_MAP(CRtuAttrDisplacement, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_DISPLACEPARA, &CRtuAttrDisplacement::OnCbnSelchangeComboDisplacepara)
	ON_BN_CLICKED(IDC_BTN_DISPLACECALL, &CRtuAttrDisplacement::OnBnClickedBtnDisplacelcall)
	ON_BN_CLICKED(IDC_BTN_DISPLACESET, &CRtuAttrDisplacement::OnBnClickedBtnDisplacelset)
	ON_BN_CLICKED(IDC_BTN_DISPLACEOK, &CRtuAttrDisplacement::OnBnClickedBtnDisplaceok)
	ON_BN_CLICKED(IDC_BTN_QUERYDISPLACEREAL,&CRtuAttrDisplacement::OnBnClickBtnQueryAReal)
	ON_NOTIFY(GVN_ENDLABELEDIT,IDC_DISPLACELIMIT_GRID,&CRtuAttrDisplacement::OnEndEditDisplaceGrid)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CRtuAttrDisplacement 消息处理程序
BOOL CRtuAttrDisplacement::OnInitDialog()
{
	CDialog::OnInitDialog();

	CWnd* pWnd = NULL;
	CRect rect1, rect2;
	int captionHeight = 0; // ::GetSystemMetrics(SM_CYCAPTION);
	int cxframe = GetSystemMetrics(SM_CXFRAME);
	int cyframe = GetSystemMetrics(SM_CYFRAME);
	this->GetWindowRect(&rect2);

	pWnd = GetDlgItem(IDC_STATIC_DISPLACEPRAR);
	if (pWnd)
		pWnd->GetWindowRect(&rect1);
	if (!m_wndDisplaceGrid.Create(CRect(rect1.left - rect2.left - cxframe, 
		rect1.top-rect2.top - cyframe-captionHeight, 
		rect1.left + rect1.Width() - rect2.left, 
		rect1.top + rect1.Height() - rect2.top - captionHeight), 
		this, IDC_DISPLACELIMIT_GRID,WS_CHILD|WS_VISIBLE))
	{
		TRACE("Failed to create threads  grid!\n");
		return FALSE;
	}

	m_wndDisplaceGrid.SetBkColor(RGB(255,255,255));
	m_wndDisplaceGrid.SetListMode(TRUE);
	m_wndDisplaceGrid.SetSingleColSelection(TRUE);
	m_wndDisplaceGrid.SetSingleRowSelection(TRUE);


	return TRUE;  
}


void CRtuAttrDisplacement::FillDisplaceGridCtrl()
{
	if (m_wndDisplaceGrid.GetSafeHwnd())
	{
		m_wndDisplaceGrid.DeleteAllItems();
		m_wndDisplaceGrid.SetRowCount(7);
		m_wndDisplaceGrid.SetColumnCount(2);
		m_wndDisplaceGrid.SetFixedRowCount(1);

		CString strHeaders[]={_T("选项"),_T("值")};
		int colWidth[] = {150,80};

		m_wndDisplaceGrid.SetItemText(0,0,strHeaders[0]);
		m_wndDisplaceGrid.SetItemText(0,1,strHeaders[1]);

		m_wndDisplaceGrid.SetColumnWidth(0,colWidth[0]);
		m_wndDisplaceGrid.SetColumnWidth(1,colWidth[1]);
		m_wndDisplaceGrid.SetRowHeight(0,MFC_GRIDCTRL_HEIGHT);

		m_wndDisplaceGrid.SetItemText(1,0,"A轴上限值(0~99999)");
		CUdfGlobalFUNC::SetGridCellReadOnly(&m_wndDisplaceGrid,1,0);
		CUdfGlobalFUNC::SetGridCellNormal(&m_wndDisplaceGrid,1,1);
		m_wndDisplaceGrid.SetRowHeight(0,MFC_GRIDCTRL_HEIGHT);

		m_wndDisplaceGrid.SetItemText(2,0,"A轴下限值(0~99999)");
		CUdfGlobalFUNC::SetGridCellReadOnly(&m_wndDisplaceGrid,2,0);
		CUdfGlobalFUNC::SetGridCellNormal(&m_wndDisplaceGrid,2,1);
		m_wndDisplaceGrid.SetRowHeight(2,MFC_GRIDCTRL_HEIGHT);

		m_wndDisplaceGrid.SetItemText(3,0,"A轴实时值");
		CUdfGlobalFUNC::SetGridCellReadOnly(&m_wndDisplaceGrid,3,0);
		CUdfGlobalFUNC::SetGridCellReadOnly(&m_wndDisplaceGrid,3,1);
		m_wndDisplaceGrid.SetRowHeight(3,MFC_GRIDCTRL_HEIGHT);

		m_wndDisplaceGrid.SetItemText(4,0,"B轴上限值(0~99999)");
		CUdfGlobalFUNC::SetGridCellReadOnly(&m_wndDisplaceGrid,4,0);
		CUdfGlobalFUNC::SetGridCellNormal(&m_wndDisplaceGrid,4,1);
		m_wndDisplaceGrid.SetRowHeight(4,MFC_GRIDCTRL_HEIGHT);

		m_wndDisplaceGrid.SetItemText(5,0,"B轴下限值(0~99999)");
		CUdfGlobalFUNC::SetGridCellReadOnly(&m_wndDisplaceGrid,5,0);
		CUdfGlobalFUNC::SetGridCellNormal(&m_wndDisplaceGrid,5,1);
		m_wndDisplaceGrid.SetRowHeight(5,MFC_GRIDCTRL_HEIGHT);

		m_wndDisplaceGrid.SetItemText(6,0,"B轴实时值");
		CUdfGlobalFUNC::SetGridCellReadOnly(&m_wndDisplaceGrid,6,0);
		CUdfGlobalFUNC::SetGridCellReadOnly(&m_wndDisplaceGrid,6,1);
		m_wndDisplaceGrid.SetRowHeight(6,MFC_GRIDCTRL_HEIGHT);


	}
}


void CRtuAttrDisplacement::OnCbnSelchangeComboDisplacepara()
{
	m_wndDisplaceGrid.DeleteAllItems();
	m_wndDisplaceGrid.Refresh();
	int nSel = m_wndDisplaceCmb.GetCurSel();
	if (nSel != CB_ERR)
	{
		int nItemData = (int)m_wndDisplaceCmb.GetItemData(nSel);
		CString strCaption = "";
		strCaption.Format("第 %-2d 个位移点的位移的详细信息",nItemData + 1);
		m_wndDisplaceCaption.SetWindowText(strCaption);

		if (m_pSubStation)
		{
			CDisplace_PARA_PER* pDisplace_PARA = m_pSubStation->GetDisplaceLimitPara(nItemData);
			if (pDisplace_PARA)
			{
				FillDisplaceGridCtrl();

				CString strText ="";
				strText.Format("%d",pDisplace_PARA->GetDisplaceXUpLimit());
				m_wndDisplaceGrid.SetItemText(1,1,strText);

				strText.Format("%d",pDisplace_PARA->GetDisplaceXDownLimit());
				m_wndDisplaceGrid.SetItemText(2,1,strText);

				strText.Format("%d",pDisplace_PARA->GetRealTimeXValue());
				m_wndDisplaceGrid.SetItemText(3,1,strText);

				strText.Format("%d",pDisplace_PARA->GetDisplaceYUpLimit());
				m_wndDisplaceGrid.SetItemText(4,1,strText);

				strText.Format("%d",pDisplace_PARA->GetDisplaceYDownLimit());
				m_wndDisplaceGrid.SetItemText(5,1,strText);

				strText.Format("%d",pDisplace_PARA->GetRealTimeYValue());
				m_wndDisplaceGrid.SetItemText(6,1,strText);
			}
		}

		m_wndDisplaceGrid.Refresh();
	}
}



void CRtuAttrDisplacement::OnBnClickedBtnDisplacelcall()
{
	if (m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{
			SetWndCtrlEnable(FALSE);

			m_blResult = FALSE;

			CPacketFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);

			m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_6AH(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)  *1000;
			SetTimer(W_TIMER_DISPLACELIMIT_CALL,nElapse,NULL);
		}
	}
}

void CRtuAttrDisplacement::OnBnClickedBtnDisplacelset()
{
	if (m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{
			CPacketFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);

			m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_21H(&appFrame);
		}
	}
}


void CRtuAttrDisplacement::OnBnClickedBtnDisplaceok()
{
	UpdateData( TRUE);
	if (m_nDisplaceCount >0)
	{
		CString strText = "";

		if (m_wndDisplaceCmb.GetSafeHwnd())
		{
			m_wndDisplaceCmb.ResetContent();

			for (int i = 0 ; i < m_nDisplaceCount ; i ++)
			{
				strText.Format("第 %-2d 个位移量点",i + 1);
				int idx = m_wndDisplaceCmb.AddString(strText);
				m_wndDisplaceCmb.SetItemData(idx,i);
			}
		}

		if (m_pSubStation != NULL)
		{
			if (m_nDisplaceCount != m_pSubStation->GetDisplaceLimitParasList()->GetCount())
			{
				m_pSubStation->ClearDisplaceLimitParaList();

				for (int i= 0; i <m_nDisplaceCount; i ++)
				{
					CDisplace_PARA_PER* pDisplace_PARA = new CDisplace_PARA_PER();
					m_pSubStation->GetDisplaceLimitParasList()->AddTail(pDisplace_PARA);
				}
			}
		}

		if (m_wndDisplaceCmb.GetCount())
		{
			m_wndDisplaceCmb.SetCurSel(0);
			OnCbnSelchangeComboDisplacepara();
		}

		m_wndDisplaceCmb.Invalidate();
	}
}

void CRtuAttrDisplacement::OnBnClickBtnQueryAReal()
{
	Update();
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{

			SetWndCtrlEnable(FALSE);
			m_blResult = FALSE;

			CPacketFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息

			appFrame.ControlC._Command = C_CMD_14_DISPLACEMENT;
			m_pSubStation->SetQueryRealDataCmd(appFrame.ControlC._Command);

			if (m_pSubStation->GetAppProVer() == APP_PROTOCOL_V1_0)
				m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_67H(&appFrame);
			else if (m_pSubStation->GetAppProVer() == APP_PROTOCOL_V2_0)
				m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_68H(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_QUERY_DISPLACEDATA,nElapse,NULL);
		}
	}
}

void CRtuAttrDisplacement::OnEndEditDisplaceGrid(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct; 
	if (!pItem  || pItem->iRow <=0 || pItem->iColumn <= 0) return;
	if (!m_pSubStation) return;

	int nDisplaceSel = m_wndDisplaceCmb.GetCurSel();
	if (nDisplaceSel != CB_ERR)
	{
		int nItemData = (int)m_wndDisplaceCmb.GetItemData(nDisplaceSel);

		CString strMessage = "";
		CString strText ="";

		CDisplace_PARA_PER* pDisplace_PARA = m_pSubStation->GetDisplaceLimitPara(nItemData);
		if (pDisplace_PARA)
		{
			if (pItem->iRow == 1 && pItem->iColumn == 1)
			{
				strText = CUdfGlobalFUNC::GetGridCellText(&m_wndDisplaceGrid,1,1);
				int UpLimit = atoi (strText);
				if (UpLimit < 0 || UpLimit > 999999)
				{
					strMessage.Format("输入的A轴上限值超出范围(0~999999),请重新输入!");
					MessageBox(strMessage,"提示",MB_ICONWARNING|MB_OK);
				}
				else
					pDisplace_PARA->SetDisplaceXUpLimit(UpLimit);


				strText.Format("%d",pDisplace_PARA->GetDisplaceXUpLimit());
				m_wndDisplaceGrid.SetItemText(1,1,strText);
			}
			else if (pItem->iRow ==2 && pItem->iColumn == 1)
			{
				CString strText = CUdfGlobalFUNC::GetGridCellText(&m_wndDisplaceGrid,2,1);
				int DownLimit = atoi(strText);
				if (DownLimit < 0 || DownLimit > 999999)
				{
					strMessage.Format("输入的A轴下限值超出范围(0~999999),请重新输入!");
					MessageBox(strMessage,"提示",MB_ICONWARNING|MB_OK);
				}
				else
					pDisplace_PARA->SetDisplaceXDownLimit(atol(strText));

				strText.Format("%d",pDisplace_PARA->GetDisplaceXDownLimit());
				m_wndDisplaceGrid.SetItemText(2,1,strText);
			}
			else if (pItem->iRow == 4 && pItem->iColumn == 1)
			{
				strText = CUdfGlobalFUNC::GetGridCellText(&m_wndDisplaceGrid,4,1);
				int UpLimit = atoi (strText);
				if (UpLimit < 0 || UpLimit > 999999)
				{
					strMessage.Format("输入的B轴上限值超出范围(0~999999),请重新输入!");
					MessageBox(strMessage,"提示",MB_ICONWARNING|MB_OK);
				}
				else
					pDisplace_PARA->SetDisplaceYUpLimit(UpLimit);


				strText.Format("%d",pDisplace_PARA->GetDisplaceYUpLimit());
				m_wndDisplaceGrid.SetItemText(4,1,strText);
			}
			else if (pItem->iRow ==5 && pItem->iColumn == 1)
			{
				CString strText = CUdfGlobalFUNC::GetGridCellText(&m_wndDisplaceGrid,5,1);
				int DownLimit = atoi(strText);
				if (DownLimit < 0 || DownLimit > 999999)
				{
					strMessage.Format("输入的B轴下限值超出范围(0~999999),请重新输入!");
					MessageBox(strMessage,"提示",MB_ICONWARNING|MB_OK);
				}
				else
					pDisplace_PARA->SetDisplaceYDownLimit(atol(strText));

				strText.Format("%d",pDisplace_PARA->GetDisplaceYDownLimit());
				m_wndDisplaceGrid.SetItemText(5,1,strText);
			}
		}
	}
}

void CRtuAttrDisplacement::Update()
{
	if (!m_pParentDlg) return;
	if (!m_pParentDlg->GetRtuStation()) return;

	m_pSubStation = m_pParentDlg->GetRtuStation();

	if (m_wndDisplaceGrid.GetSafeHwnd())
	{
		m_wndDisplaceGrid.DeleteAllItems();
		m_wndDisplaceGrid.Refresh();
	}

	if (m_pSubStation)
	{
		CString strText ="";
		int idx = 0;

		int nDisplaceCount = (int)m_pSubStation->GetDisplaceLimitParasList()->GetCount();
		strText.Format("%d",nDisplaceCount);
		m_wndDisplacePCount.SetWindowText(strText);

		if (m_wndDisplaceCmb.GetSafeHwnd())
		{
			m_wndDisplaceCmb.ResetContent();

			for (idx =0; idx < nDisplaceCount; idx ++)
			{
				strText.Format("第 %-2d 个位移量点",idx + 1);

				int Index = m_wndDisplaceCmb.AddString(strText);
				m_wndDisplaceCmb.SetItemData(Index,idx);
			}

			if (m_wndDisplaceCmb.GetCount())
			{
				m_wndDisplaceCmb.SetCurSel(0);
			}
			else
			{
				m_wndDisplaceCmb.SetCurSel(-1);
			}
			OnCbnSelchangeComboDisplacepara();
		}
	}
}


void CRtuAttrDisplacement::OnTimer(UINT_PTR nIDEvent)
{
	CString strLog= "";
	_Log_level_type LogLevel = Normal_Log_Message;

	switch (nIDEvent)
	{

	case W_TIMER_DISPLACELIMIT_CALL:
		{
			if (m_blResult == TRUE)
				strLog = "查询遥测站位移量数据上下限 成功!";
			else if (m_blResult == FALSE)
			{
				strLog = "查询遥测站位移数据上下限 超时!";
				LogLevel = Warning_log_Message;
			}
		}
		break;
	case W_TIMER_QUERY_DISPLACEDATA:
		{
			if (m_blResult == TRUE)
				strLog = "查询遥测站位移量实时数据 成功!";
			else if (m_blResult == FALSE)
			{
				strLog = "查询遥测站位移量实时数据 超时!";
				LogLevel = Warning_log_Message;
			}
		}
	}
	KillTimer(nIDEvent);
	SetWndCtrlEnable(TRUE);
	if (m_pParentDlg && m_pParentDlg->GetSafeHwnd())
		m_pParentDlg->AddLog(strLog,(PSYSTEMTIME)NULL,LogLevel);

	if (m_pSubStation)
		m_pSubStation->InsertLogMessage(strLog,LogLevel,NULL);
	CDialog::OnTimer(nIDEvent);
}

void CRtuAttrDisplacement::SetWndCtrlEnable(BOOL blEnable)
{
	m_wndDisplacePCount.EnableWindow(blEnable);
	m_wndDisplaceCmb.EnableWindow(blEnable);
	m_wndDisplaceGrid.EnableWindow(blEnable);

	m_btnDisplaceLCall.EnableWindow(blEnable);
	m_btnDisplaceLSet.EnableWindow(blEnable);
	m_btnDisplaceOK.EnableWindow(blEnable);
	m_btnQueryAReal.EnableWindow(blEnable);
}

void CRtuAttrDisplacement::UpdateDlg(int attrType,BOOL blResult)
{
	m_blResult = blResult;
	switch (attrType)
	{
	case  rtu_attr_displaceminmax_call:
		SendMessage(WM_TIMER,W_TIMER_DISPLACELIMIT_CALL,NULL);
		break;
	case rtu_attr_displace_data_call:
		SendMessage(WM_TIMER,W_TIMER_QUERY_DISPLACEDATA,NULL);
		break;
	}
}


void CRtuAttrDisplacement::KillAllTimers()
{
	KillTimer(W_TIMER_QUERY_DISPLACEDATA);
	KillTimer(W_TIMER_DISPLACELIMIT_CALL);
	SetWndCtrlEnable(TRUE);
}