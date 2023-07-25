// RtuAttrFlow.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuAttrFlow.h"

#define W_TIMER_FLOWPARA_SET	0x01             //流量表底设置
#define W_TIMER_FLOWDATA_QUERY  0x02			 //流量实时数据查询


// CRtuAttrFlow 对话框
IMPLEMENT_DYNAMIC(CRtuAttrFlow, CDialog)

CRtuAttrFlow::CRtuAttrFlow(CWnd* pParent /*=NULL*/)
	: CDialog(CRtuAttrFlow::IDD, pParent)
{
	m_blResult = FALSE;
	m_pSubStation = NULL;
	m_pParentDlg = NULL;
}

CRtuAttrFlow::~CRtuAttrFlow()
{
}

void CRtuAttrFlow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_EDIT_FLOWCOUNT,m_Edit_FlowCount);
	DDX_Control(pDX,IDC_CMB_FLOW,m_wndFlowCmb);
	DDX_Control(pDX,IDC_BTN_FLOWOK,m_btnFlowOK);
	DDX_Control(pDX,IDC_BTN_SETFLOWPARA,m_btnSetFlowPara);
	DDX_Control(pDX,IDC_BTN_QUERYFLOWDATA,m_btnQueryFlowData);
}


BEGIN_MESSAGE_MAP(CRtuAttrFlow, CDialog)
	ON_BN_CLICKED(IDC_BTN_FLOWOK,					&CRtuAttrFlow::OnBnClickFlowOK)
	ON_BN_CLICKED(IDC_BTN_SETFLOWPARA,				&CRtuAttrFlow::OnBnClickSetFlowPara)
	ON_BN_CLICKED(IDC_BTN_QUERYFLOWDATA,			&CRtuAttrFlow::OnBnClickQueryFlowData)
	ON_CBN_SELCHANGE(IDC_CMB_FLOW,					&CRtuAttrFlow::OnCbnSelchangeCmbFlow)
	ON_NOTIFY(GVN_ENDLABELEDIT,IDC_FLOW_GRIDCTRL,	&CRtuAttrFlow::OnGridEndEdit)
	ON_WM_TIMER()	
END_MESSAGE_MAP()


// CRtuAttrFlow 消息处理程序

void CRtuAttrFlow::SetParentDlg(CRtuPropertyDlg* pDlg)
{
	m_pParentDlg = pDlg;

}

void CRtuAttrFlow::Update()										//更新界面的显示信息
{
	if (m_pParentDlg)
		m_pSubStation = m_pParentDlg->GetRtuStation();
	if  (m_pSubStation != NULL)
	{
		int nFlowCount = (int)m_pSubStation->GetFlowInitValumeParasList()->GetCount();

		CString strText ="";
		strText.Format("%d",nFlowCount);
		m_Edit_FlowCount.SetWindowText(strText);

		int idx = 0;
		if (m_wndFlowCmb.GetSafeHwnd())
		{
			m_wndFlowCmb.ResetContent();

			for (idx = 0; idx < nFlowCount; idx ++)
			{
				strText.Format("第 %-2d 个流量表",idx + 1);
				int Index = m_wndFlowCmb.AddString(strText);
				m_wndFlowCmb.SetItemData(Index,idx);
			}

			if (m_wndFlowCmb.GetCount())
			{
				m_wndFlowCmb.SetCurSel(0);
			}
			else
			{
				m_wndFlowCmb.SetCurSel(-1);
			}
			OnCbnSelchangeCmbFlow();
		}
	}


}

void CRtuAttrFlow::UpdateDlg(int attrType,BOOL blResult)	   //接收到报文后、更新界面
{
	m_blResult = blResult;
	if (attrType == rtu_attr_flowdata_call)
		SendMessage(WM_TIMER,W_TIMER_FLOWDATA_QUERY);
	else if (attrType == rtu_attr_flowpara_set)
		SendMessage(WM_TIMER,W_TIMER_FLOWPARA_SET);
}


void CRtuAttrFlow::KillAllTimers()
{
	KillTimer(W_TIMER_FLOWDATA_QUERY);
	KillTimer(W_TIMER_FLOWPARA_SET);
	SetWndsEnable(TRUE);
}

void CRtuAttrFlow::SetWndsEnable(BOOL blEnable)
{
	m_Edit_FlowCount.EnableWindow(blEnable);
	m_wndFlowCmb.EnableWindow(blEnable);
	m_wndGridCtrl.EnableWindow(blEnable);

	m_btnFlowOK.EnableWindow(blEnable);
	m_btnQueryFlowData.EnableWindow(blEnable);
	m_btnSetFlowPara.EnableWindow(blEnable);
}

void CRtuAttrFlow::InitGridItems()
{
	if (m_wndGridCtrl.GetSafeHwnd())
	{
		m_wndGridCtrl.DeleteAllItems();
		int nSel = m_wndFlowCmb.GetCurSel();
		if (nSel == CB_ERR) return;
		if (!m_pSubStation) return;

		int nItemData = (int)m_wndFlowCmb.GetItemData(nSel);

		CFlowInitValume_PARA_PER* pFlow_PARA = m_pSubStation->GetFlowInitValumePara(nItemData);
		if (pFlow_PARA)
		{
			CString strHeaders[] ={
				"名称",
				"单位",
				"表底值(0~99999999)",
				"累计取水量",
				"瞬时流量"
			};

			m_wndGridCtrl.SetRowCount(5);
			m_wndGridCtrl.SetColumnCount(2);

			m_wndGridCtrl.SetColumnWidth(0,170);
			m_wndGridCtrl.SetColumnWidth(1,100);

			for (int i = 0; i < 5; i ++)
			{
				m_wndGridCtrl.SetItemText(i,0,strHeaders[i]);
				CUdfGlobalFUNC::SetGridCellReadOnly(&m_wndGridCtrl,i,0);
				CUdfGlobalFUNC::SetGridCellReadOnly(&m_wndGridCtrl,i,1);
				m_wndGridCtrl.SetItemBkColour(i,0,RGB(128,128,128));
				m_wndGridCtrl.SetRowHeight(i,MFC_GRIDCTRL_HEIGHT);
			}

			CString strText = "";
			strText.Format("第%-2d个流量表",nItemData + 1);
			m_wndGridCtrl.SetItemText(0,1,strText);

			m_wndGridCtrl.SetItemText(1,1,"立方米");

			strText.Format("%d",pFlow_PARA->GetFlowInitValume());
			CUdfGlobalFUNC::SetGridCellNormal(&m_wndGridCtrl,2,1);
			m_wndGridCtrl.SetItemText(2,1,strText);

			strText.Format("%d 立方米",pFlow_PARA->GetCalTakeValume());
			m_wndGridCtrl.SetItemText(3,1,strText);

			strText.Format("%d 立方米/h",pFlow_PARA->GetInsFlowValume());
			m_wndGridCtrl.SetItemText(4,1,strText);

		}
	}
}

BOOL CRtuAttrFlow::OnInitDialog()
{
	CDialog::OnInitDialog();

	CWnd* pWnd = GetDlgItem(IDC_STATIC_FLOWGRID);
	CRect rect1, rect2;
	int captionHeight = 0; 
	int cxframe = GetSystemMetrics(SM_CXFRAME);
	int cyframe = GetSystemMetrics(SM_CYFRAME);
	this->GetWindowRect(&rect2);
	if (pWnd)
		pWnd->GetWindowRect(&rect1);
	if (!m_wndGridCtrl.Create(CRect(rect1.left - rect2.left - cxframe, 
		rect1.top-rect2.top - cyframe-captionHeight, 
		rect1.left + rect1.Width() - rect2.left, 
		rect1.top + rect1.Height() - rect2.top - captionHeight), 
		this, IDC_FLOW_GRIDCTRL,WS_CHILD|WS_VISIBLE))
	{
		TRACE("Failed to create threads  grid!\n");
		return FALSE;
	}

	m_wndGridCtrl.SetBkColor(RGB(255,255,255));
	m_wndGridCtrl.SetListMode(TRUE);
	m_wndGridCtrl.SetSingleColSelection(TRUE);
	m_wndGridCtrl.SetSingleRowSelection(TRUE);

	return TRUE;
}


void CRtuAttrFlow::OnBnClickQueryFlowData()
{
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CPacketFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息

			appFrame.ControlC._Command = C_CMD_07_FLOWPARA;

			m_pSubStation->SetQueryRealDataCmd(appFrame.ControlC._Command);


			m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_5BH(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_FLOWDATA_QUERY,nElapse,NULL);
		}
	}
}

void CRtuAttrFlow::OnBnClickSetFlowPara()
{
	if( m_pSubStation)
	{

		if (m_pSubStation->blEnbaleTrans() == true)
		{

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CPacketFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息


			m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_1BH(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_FLOWPARA_SET,nElapse,NULL);
		}
	}
}

void CRtuAttrFlow::OnBnClickFlowOK()
{
	UpdateData( TRUE);
	CString strFlowCount = "";
	m_Edit_FlowCount.GetWindowText(strFlowCount);

	int m_nFlowCount = 0;
	m_nFlowCount = atoi(strFlowCount);

	if (m_nFlowCount >0)
	{
		CString strText = "";

		if (m_wndFlowCmb.GetSafeHwnd())
		{
			m_wndFlowCmb.ResetContent();

			for (int i = 0 ; i < m_nFlowCount ; i ++)
			{
				strText.Format("第 %-2d 个流量表",i + 1);
				int idx = m_wndFlowCmb.AddString(strText);
				m_wndFlowCmb.SetItemData(idx,i);
			}
		}

		if (m_pSubStation)
		{
			if (m_nFlowCount !=  m_pSubStation->GetFlowInitValumeParasList()->GetCount())
			{
				m_pSubStation->ClearFlowInitValumeParaList();

				for ( int i = 0; i < m_nFlowCount; i ++)
				{
					CFlowInitValume_PARA_PER* pFlow_PARA = new CFlowInitValume_PARA_PER();
					m_pSubStation->GetFlowInitValumeParasList()->AddTail(pFlow_PARA);
				}
			}
		}

		if (m_wndFlowCmb.GetCount())
		{
			m_wndFlowCmb.SetCurSel(0);
			OnCbnSelchangeCmbFlow();
		}

		m_wndFlowCmb.Invalidate();
	}
}

void CRtuAttrFlow::OnCbnSelchangeCmbFlow()
{
	InitGridItems();
}

void CRtuAttrFlow::OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	if (!pItem  || pItem->iRow <= 0  || pItem->iColumn != 1 ) return;
	if (!m_pSubStation) return;


	int nSel = m_wndFlowCmb.GetCurSel();
	if (nSel == CB_ERR) return;
	int nItemData = (int) m_wndFlowCmb.GetItemData(nSel);

	CFlowInitValume_PARA_PER* pFlow_PARA = m_pSubStation->GetFlowInitValumePara(nItemData);
	if (pFlow_PARA)
	{
		CString strText= "";
		strText = CUdfGlobalFUNC::GetGridCellText(&m_wndGridCtrl,pItem->iRow,pItem->iColumn);

		strText.Trim();
		if (strText.IsEmpty())
		{
			OnCbnSelchangeCmbFlow();
			return;
		}

		CString strMessage = "";
		long dwInputV = 0;
		dwInputV =  atol(strText);

		if (pItem->iRow == 2)
		{
			if (dwInputV < 0 || dwInputV > 99999999)
			{
				strMessage.Format("输入的表底值超出范围(0~99999999),请重新输入!");
				MessageBox(strMessage,"提示",MB_OK|MB_ICONWARNING);
			}
			else
				pFlow_PARA->SetFlowInitValume(dwInputV);

			strText.Format("%d",pFlow_PARA->GetFlowInitValume());
			CUdfGlobalFUNC::SetGridCellNormal(&m_wndGridCtrl,2,1);
			m_wndGridCtrl.SetItemText(2,1,strText);
		}
	}
}

void CRtuAttrFlow::OnTimer(UINT_PTR nIDEvent)
{

	CString strLogMessge = "";
	_Log_level_type _logLevle = Normal_Log_Message;

	switch (nIDEvent)
	{
	case W_TIMER_FLOWPARA_SET:
		{
			if (m_blResult == TRUE)
				strLogMessge = "设置遥测终端l流量表底(初始)值 成功!";
			else if( m_blResult == FALSE)
			{
				strLogMessge = "设置遥测终端l流量表底(初始)值 超时!";
				_logLevle = Warning_log_Message;
			}
		}
		break;
	case W_TIMER_FLOWDATA_QUERY:
		{
			if (m_blResult == TRUE)
				strLogMessge = "查询遥测终端的流量实时值 成功!";
			else if( m_blResult == FALSE)
			{
				strLogMessge = "查询遥测终端的流量实时值 超时!";
				_logLevle = Warning_log_Message;
			}
		}
		break;
	}
	KillTimer(nIDEvent);
	SetWndsEnable(TRUE);

	if (m_pParentDlg && m_pParentDlg->GetSafeHwnd())
		m_pParentDlg->AddLog(strLogMessge,NULL,_logLevle);

	if (m_pSubStation)
		m_pSubStation->InsertLogMessage(strLogMessge,_logLevle,NULL);

	CDialog::OnTimer(nIDEvent);
}