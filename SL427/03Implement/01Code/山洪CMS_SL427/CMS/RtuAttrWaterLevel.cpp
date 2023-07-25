// RtuAttrWaterLevel.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuAttrWaterLevel.h"

#define W_TIMER_WATERLEVEL_PARA_QUERY 0x01 //水位参数查询
#define W_TIMER_WATERLEVEL_REAL_QUERY 0x02  //水位实时数据查询


// CRtuAttrWaterLevel 对话框
IMPLEMENT_DYNAMIC(CRtuAttrWaterLevel, CDialog)

CRtuAttrWaterLevel::CRtuAttrWaterLevel(CWnd* pParent /*=NULL*/)
	: CDialog(CRtuAttrWaterLevel::IDD, pParent)
{
	m_blResult = FALSE;
	m_pSubStation = NULL;
	m_pParentDlg = NULL;
}

CRtuAttrWaterLevel::~CRtuAttrWaterLevel()
{
}

void CRtuAttrWaterLevel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_EDIT_WATERLEVELCOUNT,m_Edit_WaterLevelCount);
	DDX_Control(pDX,IDC_CMB_WATERLEVEL,m_wndWaterLevelCmb);
	DDX_Control(pDX,IDC_BTN_QUERYWATERLEVEL,m_btnQueryWaterLevel);
	DDX_Control(pDX,IDC_BTN_QUERYWATERLEVELDATA,m_btnQueryWaterLevelData);
	DDX_Control(pDX,IDC_BTN_SETWATERLEVEL,m_btnSetWaterLevel);
	DDX_Control(pDX,IDC_BTN_WATERLEVELOK,m_btnWaterLevelOK);
}


BEGIN_MESSAGE_MAP(CRtuAttrWaterLevel, CDialog)
	ON_BN_CLICKED(IDC_BTN_WATERLEVELOK,					&CRtuAttrWaterLevel::OnBnClickWaterLevelOK)
	ON_BN_CLICKED(IDC_BTN_SETWATERLEVEL,				&CRtuAttrWaterLevel::OnBnClickSetWaterLevel)
	ON_BN_CLICKED(IDC_BTN_QUERYWATERLEVEL,              &CRtuAttrWaterLevel::OnBnClickQueryWaterLevel)
	ON_BN_CLICKED(IDC_BTN_QUERYWATERLEVELDATA,          &CRtuAttrWaterLevel::OnBnClickQueryWaterLevelData)
	ON_CBN_SELCHANGE(IDC_CMB_WATERLEVEL,                &CRtuAttrWaterLevel::OnCbnSelchangeCmbWaterLevel)
	ON_NOTIFY(GVN_ENDLABELEDIT,IDC_WATERLEVEL_GRIDCTRL, &CRtuAttrWaterLevel::OnGridEndEdit)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CRtuAttrWaterLevel 消息处理程序
void CRtuAttrWaterLevel::SetParentDlg(CRtuPropertyDlg* pDlg)
{
	m_pParentDlg = pDlg;

}

void CRtuAttrWaterLevel::Update()										//更新界面的显示信息
{
	if (m_pParentDlg)
		m_pSubStation = m_pParentDlg->GetRtuStation();	
	
	
	if (m_wndGridCtrl.GetSafeHwnd())
	{
		m_wndGridCtrl.DeleteAllItems();
		m_wndGridCtrl.Refresh();
	}

	if (m_pSubStation != NULL)
	{	
		int nWLevelCount = (int)m_pSubStation->GetWaterLevelParasList()->GetCount();

		CString strText ="";
		strText.Format("%d",nWLevelCount);
		m_Edit_WaterLevelCount.SetWindowText(strText);

		int idx = 0;
		if (m_wndWaterLevelCmb.GetSafeHwnd())
		{
			m_wndWaterLevelCmb.ResetContent();

			for (idx = 0; idx < nWLevelCount; idx ++)
			{
				strText.Format("第 %-2d 个水位点",idx + 1);
				int Index = m_wndWaterLevelCmb.AddString(strText);
				m_wndWaterLevelCmb.SetItemData(Index,idx);
			}

			if (m_wndWaterLevelCmb.GetCount())
			{
				m_wndWaterLevelCmb.SetCurSel(0);
			}
			else 
			{
				m_wndWaterLevelCmb.SetCurSel(-1);
			}
			OnCbnSelchangeCmbWaterLevel();
		}
	}
}

void CRtuAttrWaterLevel::UpdateDlg(int attrType,BOOL blResult)			//接收到报文后、更新界面
{
	m_blResult = blResult;
	if (attrType == rtu_attr_waterlevel_data_call)
		SendMessage(WM_TIMER,W_TIMER_WATERLEVEL_REAL_QUERY,NULL);
	else if (attrType == rtu_attr_waterlevel_call)
		SendMessage(WM_TIMER,W_TIMER_WATERLEVEL_PARA_QUERY,NULL);

}

void CRtuAttrWaterLevel::KillAllTimers()
{
	KillTimer(W_TIMER_WATERLEVEL_PARA_QUERY);
	KillTimer(W_TIMER_WATERLEVEL_REAL_QUERY);
	SetWndsEnable(TRUE);
}

void CRtuAttrWaterLevel::SetWndsEnable(BOOL blEnable)
{
	m_Edit_WaterLevelCount.EnableWindow(blEnable);
	m_wndWaterLevelCmb.EnableWindow(blEnable);
	m_wndGridCtrl.EnableWindow(blEnable);
	m_btnWaterLevelOK.EnableWindow(blEnable);
	m_btnSetWaterLevel.EnableWindow(blEnable);
	m_btnQueryWaterLevel.EnableWindow(blEnable);
	m_btnQueryWaterLevelData.EnableWindow(blEnable);
}

BOOL CRtuAttrWaterLevel::OnInitDialog()
{
	CDialog::OnInitDialog();
	CWnd* pWnd = GetDlgItem(IDC_STATIC_WATERLEVELGRID);
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
		this, IDC_WATERLEVEL_GRIDCTRL,WS_CHILD|WS_VISIBLE))
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

void CRtuAttrWaterLevel::OnBnClickQueryWaterLevel()
{
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CPacketFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息

			m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_57H(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_WATERLEVEL_PARA_QUERY,nElapse,NULL);
		}
	}
}

void CRtuAttrWaterLevel::OnBnClickQueryWaterLevelData()
{
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CPacketFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息

			appFrame.ControlC._Command = C_CMD_06_WLPARA;

			m_pSubStation->SetQueryRealDataCmd(appFrame.ControlC._Command);


			m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_5BH(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_WATERLEVEL_REAL_QUERY,nElapse,NULL);
		}
	}
}

void CRtuAttrWaterLevel::OnBnClickSetWaterLevel()
{
	if (m_pSubStation )
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{
			CPacketFrameFormat appFrame ;
			m_pSubStation->BuildBasicPacket(appFrame);


			m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_17H(&appFrame);
		}
	}
}

void CRtuAttrWaterLevel::OnBnClickWaterLevelOK()
{
	UpdateData( TRUE);
	CString strLevelCount = "";
	m_Edit_WaterLevelCount.GetWindowText(strLevelCount);

	int m_nLevelCount = 0;
	m_nLevelCount = atoi(strLevelCount);

	if (m_nLevelCount >0)
	{
		CString strText = "";

		if (m_wndWaterLevelCmb.GetSafeHwnd())
		{
			m_wndWaterLevelCmb.ResetContent();

			for (int i = 0 ; i < m_nLevelCount ; i ++)
			{
				strText.Format("第 %-2d 个水位点",i + 1);
				int idx = m_wndWaterLevelCmb.AddString(strText);
				m_wndWaterLevelCmb.SetItemData(idx,i);
			}
		}

		if (m_pSubStation)
		{
			if (m_nLevelCount !=  m_pSubStation->GetWaterLevelParasList()->GetCount())
			{
				m_pSubStation->ClearWaterLevelParaList();

				for ( int i = 0; i < m_nLevelCount; i ++)
				{
					CWaterLevel_PAR_PER* pLevel_PARA = new CWaterLevel_PAR_PER();
					m_pSubStation->GetWaterLevelParasList()->AddTail(pLevel_PARA);
				}
			}
		}

		if (m_wndWaterLevelCmb.GetCount())
		{
			m_wndWaterLevelCmb.SetCurSel(0);
			OnCbnSelchangeCmbWaterLevel();
		}

		m_wndWaterLevelCmb.Invalidate();
	}
}

void CRtuAttrWaterLevel::OnCbnSelchangeCmbWaterLevel()
{
	InitGridItems();
}

void CRtuAttrWaterLevel::OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	if (!pItem  || pItem->iRow <= 0  || pItem->iColumn != 1 ) return;
	if (!m_pSubStation) return;


	int nSel = m_wndWaterLevelCmb.GetCurSel();
	if (nSel == CB_ERR) return;
	int nItemData = (int) m_wndWaterLevelCmb.GetItemData(nSel);

	CWaterLevel_PAR_PER* pLevel_PARA = m_pSubStation->GetWaterLevelPara(nItemData);
	
	if (pLevel_PARA != NULL)
	{
		CString strText= "";
		strText = CUdfGlobalFUNC::GetGridCellText(&m_wndGridCtrl,pItem->iRow,pItem->iColumn);

		strText.Trim();
		if (strText.IsEmpty())
		{
			OnCbnSelchangeCmbWaterLevel();
			return;
		}

		CString strMessage = "";
		double fInputV = 0.0;
		fInputV = /*(float)*/atof(strText);
		switch (pItem->iRow)
		{
		case 2:
			{
				if (fInputV >9999.99 || fInputV < 0)
				{
					strMessage.Format("输入基值超出范围(0~9999.99),请重新输入!");
					MessageBox(strMessage,"提示",MB_OK|MB_ICONWARNING);

					strText.Format("%.2f",pLevel_PARA->GetBaseValue());
					CUdfGlobalFUNC::SetGridCellNormal(&m_wndGridCtrl,2,1);
					m_wndGridCtrl.SetItemText(2,1,strText);

					return;
				}

				pLevel_PARA->SetBaseValue(fInputV);
				strText.Format("%.2f",pLevel_PARA->GetBaseValue());
				CUdfGlobalFUNC::SetGridCellNormal(&m_wndGridCtrl,2,1);
				m_wndGridCtrl.SetItemText(2,1,strText);
			}
			break;
		case 3:
			{
				if (fInputV > 99.99 || fInputV<0)
				{
					strMessage.Format("输入的上限值超出范围(0~99.99),请重新输入!");
					MessageBox(strMessage,"提示",MB_OK|MB_ICONWARNING);

					strText.Format("%.2f",pLevel_PARA->GetUpValue());
					CUdfGlobalFUNC::SetGridCellNormal(&m_wndGridCtrl,3,1);
					m_wndGridCtrl.SetItemText(3,1,strText);

					return;
				}

				pLevel_PARA->SetUpValue(fInputV);
				strText.Format("%.2f",pLevel_PARA->GetUpValue());
				CUdfGlobalFUNC::SetGridCellNormal(&m_wndGridCtrl,3,1);
				m_wndGridCtrl.SetItemText(3,1,strText);
			}
			break;
		case 4:
			{
				if (fInputV > 99.99 || fInputV < 0)
				{
					strMessage.Format("输入的下限值超出范围(0~99.99),请重新输入!");
					MessageBox(strMessage,"提示",MB_OK|MB_ICONWARNING);

					strText.Format("%.2f",pLevel_PARA->GetDownValue());
					CUdfGlobalFUNC::SetGridCellNormal(&m_wndGridCtrl,4,1);
					m_wndGridCtrl.SetItemText(4,1,strText);
					
					return;
				}

				pLevel_PARA->SetDownValue(fInputV);
				strText.Format("%.2f",pLevel_PARA->GetDownValue());
				CUdfGlobalFUNC::SetGridCellNormal(&m_wndGridCtrl,4,1);
				m_wndGridCtrl.SetItemText(4,1,strText);
			}
			break;
		}
	}
}

void CRtuAttrWaterLevel::InitGridItems()
{
	if (m_wndGridCtrl.GetSafeHwnd())
	{
		m_wndGridCtrl.DeleteAllItems();
		int nSel = m_wndWaterLevelCmb.GetCurSel();
		if (nSel == CB_ERR) return;
		if (!m_pSubStation) return;

		int nItemData = (int)m_wndWaterLevelCmb.GetItemData(nSel);

		CWaterLevel_PAR_PER* pLevel_Para = m_pSubStation->GetWaterLevelPara(nItemData);
		if (pLevel_Para)
		{
			CString strHeaders[] ={
				"名称",
				"单位",
				"水位基值(0～9999.99)",
				"上限值(0~99.99)",
				"下限值(0~99.99)",
				"实时值"
			};

			m_wndGridCtrl.SetRowCount(6);
			m_wndGridCtrl.SetColumnCount(2);

			m_wndGridCtrl.SetColumnWidth(0,170);
			m_wndGridCtrl.SetColumnWidth(1,120);

			for (int i = 0; i < 6; i ++)
			{
				m_wndGridCtrl.SetItemText(i,0,strHeaders[i]);
				CUdfGlobalFUNC::SetGridCellReadOnly(&m_wndGridCtrl,i,0);
				CUdfGlobalFUNC::SetGridCellReadOnly(&m_wndGridCtrl,i,1);
				m_wndGridCtrl.SetItemBkColour(i,0,RGB(128,128,128));
				m_wndGridCtrl.SetRowHeight(i,MFC_GRIDCTRL_HEIGHT);
			}

			CString strText = "";
			strText.Format("第 %-2d 个水位点",nItemData + 1);
			m_wndGridCtrl.SetItemText(0,1,strText);

			m_wndGridCtrl.SetItemText(1,1,"m");

			strText.Format("%.2f",pLevel_Para->GetBaseValue());
			CUdfGlobalFUNC::SetGridCellNormal(&m_wndGridCtrl,2,1);
			m_wndGridCtrl.SetItemText(2,1,strText);
			
			strText.Format("%.2f",pLevel_Para->GetUpValue());
			CUdfGlobalFUNC::SetGridCellNormal(&m_wndGridCtrl,3,1);
			m_wndGridCtrl.SetItemText(3,1,strText);
			
			strText.Format("%.2f",pLevel_Para->GetDownValue());
			CUdfGlobalFUNC::SetGridCellNormal(&m_wndGridCtrl,4,1);
			m_wndGridCtrl.SetItemText(4,1,strText);
			
			strText.Format("%.2f",pLevel_Para->GetRealValue());
			m_wndGridCtrl.SetItemText(5,1,strText);
		}

		m_wndGridCtrl.Refresh();

	}
}

void CRtuAttrWaterLevel::OnTimer(UINT_PTR nIDEvent)
{
	CString strLogMessge = "";
	_Log_level_type _logLevle = Normal_Log_Message;

	switch (nIDEvent)
	{
	case W_TIMER_WATERLEVEL_PARA_QUERY:
		{
			if (m_blResult == TRUE)
				strLogMessge = "查询遥测终端水位基值、上下限值 成功!";
			else if( m_blResult == FALSE)
			{
				strLogMessge = "查询遥测终端水位基值、上下限值 超时!";
				_logLevle = Warning_log_Message;
			}
		}
		break;
	case W_TIMER_WATERLEVEL_REAL_QUERY:
		{
			if (m_blResult == TRUE)
				strLogMessge = "查询遥测终端的水位实时值 成功!";
			else if( m_blResult == FALSE)
			{
				strLogMessge = "查询遥测终端的水位实时值 超时!";
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