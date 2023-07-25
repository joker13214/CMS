// RtuAttrTemperature.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuAttrTemperature.h"
#include "RtuStation.h"
#include "RtuPropertyDlg.h"

// CRtuAttrTemperature 对话框
#define  W_TIMER_TEMPERLIMIT_CALL		0x01      //温度上下限值查询Timer
#define  W_TIMER_QUERY_TEMPERDATA       0x02      //温度实时值查询




IMPLEMENT_DYNAMIC(CRtuAttrTemperature, CDialog)

CRtuAttrTemperature::CRtuAttrTemperature(CWnd* pParent /*=NULL*/)
	: CDialog(CRtuAttrTemperature::IDD, pParent)
	, m_nTemperaCount(0)
{
	m_pSubStation = NULL;
	m_pParentDlg =  NULL;
	m_blResult = FALSE;
}

CRtuAttrTemperature::~CRtuAttrTemperature()
{
}

void CRtuAttrTemperature::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_TEMPERCOUNT, m_wndTemperaPCount);
	DDX_Control(pDX, IDC_COMBO_TEMPERPARA, m_wndTemperaCmb);
	DDX_Control(pDX, IDC_BTN_TEMPERCALL, m_btnTemperaLCall);
	DDX_Control(pDX, IDC_BTN_TEMPERSET, m_btnTemperaLSet);
	DDX_Control(pDX, IDC_STATIC_TEMPERCAPTION, m_wndTemperaCaption);
	DDX_Control(pDX, IDC_BTN_TEMPEROK, m_btnTemperaOK);
	DDX_Text(pDX, IDC_EDIT_TEMPERCOUNT, m_nTemperaCount);
	DDX_Control(pDX, IDC_BTN_QUERYTEMPERREAL, m_btnQueryAReal);
}


BEGIN_MESSAGE_MAP(CRtuAttrTemperature, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_TEMPERPARA, &CRtuAttrTemperature::OnCbnSelchangeComboTemperapara)
	ON_BN_CLICKED(IDC_BTN_TEMPERCALL, &CRtuAttrTemperature::OnBnClickedBtnTemperalcall)
	ON_BN_CLICKED(IDC_BTN_TEMPERSET, &CRtuAttrTemperature::OnBnClickedBtnTemperalset)
	ON_BN_CLICKED(IDC_BTN_TEMPEROK, &CRtuAttrTemperature::OnBnClickedBtnTemperaok)
	ON_BN_CLICKED(IDC_BTN_QUERYTEMPERREAL,&CRtuAttrTemperature::OnBnClickBtnQueryAReal)
	ON_NOTIFY(GVN_ENDLABELEDIT,IDC_TEMPERLIMIT_GRID,&CRtuAttrTemperature::OnEndEditTemperaGrid)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CRtuAttrTemperature 消息处理程序
BOOL CRtuAttrTemperature::OnInitDialog()
{
	CDialog::OnInitDialog();

	CWnd* pWnd = NULL;
	CRect rect1, rect2;
	int captionHeight = 0; // ::GetSystemMetrics(SM_CYCAPTION);
	int cxframe = GetSystemMetrics(SM_CXFRAME);
	int cyframe = GetSystemMetrics(SM_CYFRAME);
	this->GetWindowRect(&rect2);

	pWnd = GetDlgItem(IDC_STATIC_TEMPERPRAR);
	if (pWnd)
		pWnd->GetWindowRect(&rect1);
	if (!m_wndTemperaGrid.Create(CRect(rect1.left - rect2.left - cxframe, 
		rect1.top-rect2.top - cyframe-captionHeight, 
		rect1.left + rect1.Width() - rect2.left, 
		rect1.top + rect1.Height() - rect2.top - captionHeight), 
		this, IDC_TEMPERLIMIT_GRID,WS_CHILD|WS_VISIBLE))
	{
		TRACE("Failed to create threads  grid!\n");
		return FALSE;
	}

	m_wndTemperaGrid.SetBkColor(RGB(255,255,255));
	m_wndTemperaGrid.SetListMode(TRUE);
	m_wndTemperaGrid.SetSingleColSelection(TRUE);
	m_wndTemperaGrid.SetSingleRowSelection(TRUE);


	return TRUE;  
}


void CRtuAttrTemperature::FillTemperaGridCtrl()
{
	if (m_wndTemperaGrid.GetSafeHwnd())
	{
		m_wndTemperaGrid.DeleteAllItems();
		m_wndTemperaGrid.SetRowCount(4);
		m_wndTemperaGrid.SetColumnCount(2);
		m_wndTemperaGrid.SetFixedRowCount(1);

		CString strHeaders[]={_T("选项"),_T("值")};
		int colWidth[] = {110,80};

		m_wndTemperaGrid.SetItemText(0,0,strHeaders[0]);
		m_wndTemperaGrid.SetItemText(0,1,strHeaders[1]);

		m_wndTemperaGrid.SetColumnWidth(0,colWidth[0]);
		m_wndTemperaGrid.SetColumnWidth(1,colWidth[1]);
		m_wndTemperaGrid.SetRowHeight(0,MFC_GRIDCTRL_HEIGHT);

		m_wndTemperaGrid.SetItemText(1,0,"上限值(0~999.9)");
		CUdfGlobalFUNC::SetGridCellReadOnly(&m_wndTemperaGrid,1,0);
		CUdfGlobalFUNC::SetGridCellNormal(&m_wndTemperaGrid,1,1);
		m_wndTemperaGrid.SetRowHeight(0,MFC_GRIDCTRL_HEIGHT);

		m_wndTemperaGrid.SetItemText(2,0,"下限值(0~999.9)");
		CUdfGlobalFUNC::SetGridCellReadOnly(&m_wndTemperaGrid,2,0);
		CUdfGlobalFUNC::SetGridCellNormal(&m_wndTemperaGrid,2,1);
		m_wndTemperaGrid.SetRowHeight(2,MFC_GRIDCTRL_HEIGHT);

		m_wndTemperaGrid.SetItemText(3,0,"实时值");
		CUdfGlobalFUNC::SetGridCellReadOnly(&m_wndTemperaGrid,3,0);
		CUdfGlobalFUNC::SetGridCellReadOnly(&m_wndTemperaGrid,3,1);
		m_wndTemperaGrid.SetRowHeight(3,MFC_GRIDCTRL_HEIGHT);

	}
}


void CRtuAttrTemperature::OnCbnSelchangeComboTemperapara()
{
	m_wndTemperaGrid.DeleteAllItems();
	m_wndTemperaGrid.Refresh();
	int nSel = m_wndTemperaCmb.GetCurSel();
	if (nSel != CB_ERR)
	{
		int nItemData = (int)m_wndTemperaCmb.GetItemData(nSel);
		CString strCaption = "";
		strCaption.Format("第 %-2d 个温度点的温度的详细信息",nItemData + 1);
		m_wndTemperaCaption.SetWindowText(strCaption);

		if (m_pSubStation)
		{
			CTempera_PARA_PER* pTempera_PARA = m_pSubStation->GetTemperaLimitPara(nItemData);
			if (pTempera_PARA)
			{
				FillTemperaGridCtrl();

				CString strText ="";
				strText.Format("%0.1f",pTempera_PARA->GetTemperaUpLimit());

				m_wndTemperaGrid.SetItemText(1,1,strText);

				strText.Format("%0.1f",pTempera_PARA->GetTemperaDownLimit());
				m_wndTemperaGrid.SetItemText(2,1,strText);

				strText.Format("%0.1f",pTempera_PARA->GetRealTimeValue());
				m_wndTemperaGrid.SetItemText(3,1,strText);
			}
		}

		m_wndTemperaGrid.Refresh();
	}
}



void CRtuAttrTemperature::OnBnClickedBtnTemperalcall()
{
	if (m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{
			SetWndCtrlEnable(FALSE);

			m_blResult = FALSE;

			CPacketFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);

			m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_6BH(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)  *1000;
			SetTimer(W_TIMER_TEMPERLIMIT_CALL,nElapse,NULL);
		}
	}
}

void CRtuAttrTemperature::OnBnClickedBtnTemperalset()
{
	if (m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{
			CPacketFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);

			m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_22H(&appFrame);
		}
	}
}


void CRtuAttrTemperature::OnBnClickedBtnTemperaok()
{
	UpdateData( TRUE);
	if (m_nTemperaCount >0)
	{
		CString strText = "";

		if (m_wndTemperaCmb.GetSafeHwnd())
		{
			m_wndTemperaCmb.ResetContent();

			for (int i = 0 ; i < m_nTemperaCount ; i ++)
			{
				strText.Format("第 %-2d 个温度点",i + 1);
				int idx = m_wndTemperaCmb.AddString(strText);
				m_wndTemperaCmb.SetItemData(idx,i);
			}
		}

		if (m_pSubStation != NULL)
		{
			if (m_nTemperaCount != m_pSubStation->GetTemperaLimitParasList()->GetCount())
			{
				m_pSubStation->ClearTemperaLimitParaList();

				for (int i= 0; i <m_nTemperaCount; i ++)
				{
					CTempera_PARA_PER* pTempera_PARA = new CTempera_PARA_PER();
					m_pSubStation->GetTemperaLimitParasList()->AddTail(pTempera_PARA);
				}
			}
		}

		if (m_wndTemperaCmb.GetCount())
		{
			m_wndTemperaCmb.SetCurSel(0);
			OnCbnSelchangeComboTemperapara();
		}

		m_wndTemperaCmb.Invalidate();
	}
}

void CRtuAttrTemperature::OnBnClickBtnQueryAReal()
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

			appFrame.ControlC._Command = C_CMD_15_TEMPERATURE;
			m_pSubStation->SetQueryRealDataCmd(appFrame.ControlC._Command);

			if (m_pSubStation->GetAppProVer() == APP_PROTOCOL_V1_0)
				m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_67H(&appFrame);
			else if (m_pSubStation->GetAppProVer() == APP_PROTOCOL_V2_0)
				m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_68H(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_QUERY_TEMPERDATA,nElapse,NULL);
		}
	}
}

void CRtuAttrTemperature::OnEndEditTemperaGrid(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct; 
	if (!pItem  || pItem->iRow <=0 || pItem->iColumn <= 0) return;
	if (!m_pSubStation) return;

	int nTemperaSel = m_wndTemperaCmb.GetCurSel();
	if (nTemperaSel != CB_ERR)
	{
		int nItemData = (int)m_wndTemperaCmb.GetItemData(nTemperaSel);

		CString strMessage = "";
		CString strText ="";

		CTempera_PARA_PER* pTempera_PARA = m_pSubStation->GetTemperaLimitPara(nItemData);
		if (pTempera_PARA)
		{
			if (pItem->iRow == 1 && pItem->iColumn == 1)
			{
				strText = CUdfGlobalFUNC::GetGridCellText(&m_wndTemperaGrid,1,1);
				int UpLimit = atoi (strText);
				if (UpLimit < 0 || UpLimit > 99999)
				{
					strMessage.Format("输入的上限值超出范围(0~999.9),请重新输入!");
					MessageBox(strMessage,"提示",MB_ICONWARNING|MB_OK);
				}
				else
					pTempera_PARA->SetTemperaUpLimit(UpLimit);


				strText.Format("%d",pTempera_PARA->GetTemperaUpLimit());
				m_wndTemperaGrid.SetItemText(1,1,strText);
			}
			else if (pItem->iRow ==2 && pItem->iColumn == 1)
			{
				CString strText = CUdfGlobalFUNC::GetGridCellText(&m_wndTemperaGrid,2,1);
				int DownLimit = atoi(strText);
				if (DownLimit < 0 || DownLimit > 99999)
				{
					strMessage.Format("输入的下限值超出范围(0~999.9),请重新输入!");
					MessageBox(strMessage,"提示",MB_ICONWARNING|MB_OK);
				}
				else
					pTempera_PARA->SetTemperaDownLimit(atol(strText));

				strText.Format("%d",pTempera_PARA->GetTemperaDownLimit());
				m_wndTemperaGrid.SetItemText(2,1,strText);
			}
		}
	}
}

void CRtuAttrTemperature::Update()
{
	if (!m_pParentDlg) return;
	if (!m_pParentDlg->GetRtuStation()) return;

	m_pSubStation = m_pParentDlg->GetRtuStation();

	if (m_wndTemperaGrid.GetSafeHwnd())
	{
		m_wndTemperaGrid.DeleteAllItems();
		m_wndTemperaGrid.Refresh();
	}

	if (m_pSubStation)
	{
		CString strText ="";
		int idx = 0;

		int nTemperaCount = (int)m_pSubStation->GetTemperaLimitParasList()->GetCount();
		strText.Format("%d",nTemperaCount);
		m_wndTemperaPCount.SetWindowText(strText);

		if (m_wndTemperaCmb.GetSafeHwnd())
		{
			m_wndTemperaCmb.ResetContent();

			for (idx =0; idx < nTemperaCount; idx ++)
			{
				strText.Format("第 %-2d 个温度点",idx + 1);

				int Index = m_wndTemperaCmb.AddString(strText);
				m_wndTemperaCmb.SetItemData(Index,idx);
			}

			if (m_wndTemperaCmb.GetCount())
			{
				m_wndTemperaCmb.SetCurSel(0);
			}
			else
			{
				m_wndTemperaCmb.SetCurSel(-1);
			}
			OnCbnSelchangeComboTemperapara();
		}
	}
}


void CRtuAttrTemperature::OnTimer(UINT_PTR nIDEvent)
{
	CString strLog= "";
	_Log_level_type LogLevel = Normal_Log_Message;

	switch (nIDEvent)
	{

	case W_TIMER_TEMPERLIMIT_CALL:
		{
			if (m_blResult == TRUE)
				strLog = "查询遥测站温度数据上下限 成功!";
			else if (m_blResult == FALSE)
			{
				strLog = "查询遥测站温度数据上下限 超时!";
				LogLevel = Warning_log_Message;
			}
		}
		break;
	case W_TIMER_QUERY_TEMPERDATA:
		{
			if (m_blResult == TRUE)
				strLog = "查询遥测站温度实时数据 成功!";
			else if (m_blResult == FALSE)
			{
				strLog = "查询遥测站温度实时数据 超时!";
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

void CRtuAttrTemperature::SetWndCtrlEnable(BOOL blEnable)
{
	m_wndTemperaPCount.EnableWindow(blEnable);
	m_wndTemperaCmb.EnableWindow(blEnable);
	m_wndTemperaGrid.EnableWindow(blEnable);

	m_btnTemperaLCall.EnableWindow(blEnable);
	m_btnTemperaLSet.EnableWindow(blEnable);
	m_btnTemperaOK.EnableWindow(blEnable);
	m_btnQueryAReal.EnableWindow(blEnable);
}

void CRtuAttrTemperature::UpdateDlg(int attrType,BOOL blResult)
{
	m_blResult = blResult;
	switch (attrType)
	{
	case  rtu_attr_temperaminmax_call:
		SendMessage(WM_TIMER,W_TIMER_TEMPERLIMIT_CALL,NULL);
		break;
	case rtu_attr_tempera_data_call:
		SendMessage(WM_TIMER,W_TIMER_QUERY_TEMPERDATA,NULL);
		break;
	}
}


void CRtuAttrTemperature::KillAllTimers()
{
	KillTimer(W_TIMER_QUERY_TEMPERDATA);
	KillTimer(W_TIMER_TEMPERLIMIT_CALL);
	SetWndCtrlEnable(TRUE);
}