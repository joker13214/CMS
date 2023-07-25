// RtuAttrWaterPress.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuAttrWaterPress.h"


#define W_TIMER_WATERPRESS_PARA_QUERY 0x01 //水位参数查询
#define W_TIMER_WATERPRESS_REAL_QUERY 0x02  //水位实时数据查询
// CRtuAttrWaterPress 对话框

IMPLEMENT_DYNAMIC(CRtuAttrWaterPress, CDialog)

CRtuAttrWaterPress::CRtuAttrWaterPress(CWnd* pParent /*=NULL*/)
	: CDialog(CRtuAttrWaterPress::IDD, pParent)
{
	m_blResult = FALSE;
	m_pSubStation = NULL;
	m_pParentDlg = NULL;
}

CRtuAttrWaterPress::~CRtuAttrWaterPress()
{
}

void CRtuAttrWaterPress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_EDIT_WPRESSCOUNT,m_Edit_WPressCount);
	DDX_Control(pDX,IDC_CMB_WPRESS,m_wndWaterPressCmb);
	DDX_Control(pDX,IDC_BTN_WPRESSOK,m_btnWPressOK);
	DDX_Control(pDX,IDC_BTN_QUERYWPRESS,m_btnQueryWPress);
	DDX_Control(pDX,IDC_BTN_QUERYWPRESSDATA,m_btnQueryWPressData);
	DDX_Control(pDX,IDC_BTN_SETWPRESS,m_btnSetWPress);
}


BEGIN_MESSAGE_MAP(CRtuAttrWaterPress, CDialog)
	ON_BN_CLICKED(IDC_BTN_WPRESSOK,					&CRtuAttrWaterPress::OnBnClickWPressOK)
	ON_BN_CLICKED(IDC_BTN_SETWPRESS,				&CRtuAttrWaterPress::OnBnClickSetWPress)
	ON_BN_CLICKED(IDC_BTN_QUERYWPRESS,              &CRtuAttrWaterPress::OnBnClickQueryWPress)
	ON_BN_CLICKED(IDC_BTN_QUERYWPRESSDATA,          &CRtuAttrWaterPress::OnBnClickQueryWPressData)
	ON_CBN_SELCHANGE(IDC_CMB_WPRESS,                &CRtuAttrWaterPress::OnCbnSelchangeCmbWPress)
	ON_NOTIFY(GVN_ENDLABELEDIT,IDC_WPRESS_GRIDCTRL, &CRtuAttrWaterPress::OnGridEndEdit)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CRtuAttrWaterPress 消息处理程序
void CRtuAttrWaterPress::SetParentDlg(CRtuPropertyDlg* pDlg)
{
	m_pParentDlg = pDlg;

}

void CRtuAttrWaterPress::Update()										//更新界面的显示信息
{
	if (m_pParentDlg)
		m_pSubStation = m_pParentDlg->GetRtuStation();

	if (m_pSubStation)
	{
		int nWPressCount = (int)m_pSubStation->GetWaterPressureParasList()->GetCount();

		if (m_wndGridCtrl.GetSafeHwnd())
		{
			m_wndGridCtrl.DeleteAllItems();
			m_wndGridCtrl.Refresh();
		}

		CString strText ="";
		strText.Format("%d",nWPressCount);
		m_Edit_WPressCount.SetWindowText(strText);

		int idx = 0;
		if (m_wndWaterPressCmb.GetSafeHwnd())
		{
			m_wndWaterPressCmb.ResetContent();

			for (idx = 0; idx < nWPressCount; idx ++)
			{
				strText.Format("第 %-2d 个水压点",idx + 1);
				int Index = m_wndWaterPressCmb.AddString(strText);
				m_wndWaterPressCmb.SetItemData(Index,idx);
			}

			if (m_wndWaterPressCmb.GetCount())
			{
				m_wndWaterPressCmb.SetCurSel(0);
			}
			else
			{
				m_wndWaterPressCmb.SetCurSel(-1);
			}
			OnCbnSelchangeCmbWPress();
		}
	}


}
void CRtuAttrWaterPress::UpdateDlg(int attrType,BOOL blResult)									//接收到报文后、更新界面
{
	m_blResult = blResult;
	if (attrType == rtu_attr_waterpress_call)
		SendMessage(WM_TIMER,W_TIMER_WATERPRESS_PARA_QUERY,NULL);
	else if (attrType == rtu_attr_waterpress_data_call)
		SendMessage(WM_TIMER,W_TIMER_WATERPRESS_REAL_QUERY,NULL);
}


void CRtuAttrWaterPress::KillAllTimers()
{
	KillTimer(W_TIMER_WATERPRESS_PARA_QUERY);
	KillTimer(W_TIMER_WATERPRESS_REAL_QUERY);
	SetWndsEnable(TRUE);
}

BOOL CRtuAttrWaterPress::OnInitDialog()
{
	CDialog::OnInitDialog();

	CWnd* pWnd = GetDlgItem(IDC_STATIC_WPRESSGRID);
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
		this, IDC_WPRESS_GRIDCTRL,WS_CHILD|WS_VISIBLE))
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


void CRtuAttrWaterPress::SetWndsEnable(BOOL blEnable)
{
	m_Edit_WPressCount.EnableWindow(blEnable);
	m_wndGridCtrl.EnableWindow(blEnable);
	m_wndWaterPressCmb.EnableWindow(blEnable);
	
	m_btnSetWPress.EnableWindow(blEnable);
	m_btnQueryWPress.EnableWindow(blEnable);
	m_btnQueryWPressData.EnableWindow(blEnable);
	m_btnWPressOK.EnableWindow(blEnable);
}

void CRtuAttrWaterPress::InitGridItems()
{
	if (m_wndGridCtrl.GetSafeHwnd())
	{
		m_wndGridCtrl.DeleteAllItems();
		int nSel = m_wndWaterPressCmb.GetCurSel();
		if (nSel == CB_ERR) return;
		if (!m_pSubStation) return;

		int nItemData = (int)m_wndWaterPressCmb.GetItemData(nSel);

		CWaterPressure_PARA_PER* pPress_PARA = m_pSubStation->GetWaterPressurePara(nItemData);
		if (pPress_PARA)
		{
			CString strHeaders[] ={
				"名称",
				"单位",
				"上限值(0~999999.99)",
				"下限值(0~999999.99)",
				"实时值"
			};

			m_wndGridCtrl.SetRowCount(5);
			m_wndGridCtrl.SetColumnCount(2);

			m_wndGridCtrl.SetColumnWidth(0,150);
			m_wndGridCtrl.SetColumnWidth(1,120);

			for (int i = 0; i < 5; i ++)
			{
				m_wndGridCtrl.SetItemText(i,0,strHeaders[i]);
				CUdfGlobalFUNC::SetGridCellReadOnly(&m_wndGridCtrl,i,0);
				CUdfGlobalFUNC::SetGridCellReadOnly(&m_wndGridCtrl,i,1);
				m_wndGridCtrl.SetItemBkColour(i,0,RGB(128,128,128));
				m_wndGridCtrl.SetRowHeight(i,MFC_GRIDCTRL_HEIGHT);
			}

			CString strText = "";
			strText.Format("第 %-2d 个水压点",nItemData + 1);
			m_wndGridCtrl.SetItemText(0,1,strText);

			m_wndGridCtrl.SetItemText(1,1,"kPa");

			strText.Format("%.2f",pPress_PARA->GetUpValue());
			CUdfGlobalFUNC::SetGridCellNormal(&m_wndGridCtrl,2,1);
			m_wndGridCtrl.SetItemText(2,1,strText);

			strText.Format("%.2f",pPress_PARA->GetDownValue());
			CUdfGlobalFUNC::SetGridCellNormal(&m_wndGridCtrl,3,1);
			m_wndGridCtrl.SetItemText(3,1,strText);

			strText.Format("%.2f",pPress_PARA->GetRealValue());
			m_wndGridCtrl.SetItemText(4,1,strText);
		}

		m_wndGridCtrl.Refresh();
	}
}


void CRtuAttrWaterPress::OnBnClickQueryWPress()
{
	if( m_pSubStation)
	{

		if (m_pSubStation->blEnbaleTrans() == true)
		{

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CPacketFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息

			m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_58H(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_WATERPRESS_PARA_QUERY,nElapse,NULL);
		}
	}
}

void CRtuAttrWaterPress::OnBnClickQueryWPressData()
{
	if( m_pSubStation)
	{

		if (m_pSubStation->blEnbaleTrans() == true)
		{

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CPacketFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息

			appFrame.ControlC._Command = C_CMD_08_WPPARA;

			m_pSubStation->SetQueryRealDataCmd(appFrame.ControlC._Command);


			m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_5BH(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_WATERPRESS_REAL_QUERY,nElapse,NULL);
		}
	}
}

void CRtuAttrWaterPress::OnBnClickSetWPress()
{
	if (m_pSubStation )
	{

		if (m_pSubStation->blEnbaleTrans() == true)
		{
			CPacketFrameFormat appFrame ;
			m_pSubStation->BuildBasicPacket(appFrame);


			m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_18H(&appFrame);
		}
	}
}

void CRtuAttrWaterPress::OnBnClickWPressOK()
{
	UpdateData( TRUE);
	CString strPressCount = "";
	m_Edit_WPressCount.GetWindowText(strPressCount);

	int m_nPressCount = 0;
	m_nPressCount = atoi(strPressCount);

	if (m_nPressCount >0)
	{
		CString strText = "";

		if (m_wndWaterPressCmb.GetSafeHwnd())
		{
			m_wndWaterPressCmb.ResetContent();

			for (int i = 0 ; i < m_nPressCount ; i ++)
			{
				strText.Format("第 %-2d 个水压点",i + 1);
				int idx = m_wndWaterPressCmb.AddString(strText);
				m_wndWaterPressCmb.SetItemData(idx,i);
			}
		}

		if (m_pSubStation)
		{
			if (m_nPressCount !=  m_pSubStation->GetWaterPressureParasList()->GetCount())
			{
				m_pSubStation->ClearWaterPressureParaList();

				for ( int i = 0; i < m_nPressCount; i ++)
				{
					CWaterPressure_PARA_PER* pPress_PARA = new CWaterPressure_PARA_PER();
					m_pSubStation->GetWaterPressureParasList()->AddTail(pPress_PARA);
				}
			}
		}

		if (m_wndWaterPressCmb.GetCount())
		{
			m_wndWaterPressCmb.SetCurSel(0);
			OnCbnSelchangeCmbWPress();
		}

		m_wndWaterPressCmb.Invalidate();
	}
}

void CRtuAttrWaterPress::OnCbnSelchangeCmbWPress()
{
	InitGridItems();
}

void CRtuAttrWaterPress::OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	if (!pItem  || pItem->iRow <= 0  || pItem->iColumn != 1 ) return;
	if (!m_pSubStation) return;


	int nSel = m_wndWaterPressCmb.GetCurSel();
	if (nSel == CB_ERR) return;
	int nItemData = (int) m_wndWaterPressCmb.GetItemData(nSel);

	CWaterPressure_PARA_PER* pPress_PARA = m_pSubStation->GetWaterPressurePara(nItemData);
	
	if (pPress_PARA)
	{
		CString strText= "";
		strText = CUdfGlobalFUNC::GetGridCellText(&m_wndGridCtrl,pItem->iRow,pItem->iColumn);

		strText.Trim();
		if (strText.IsEmpty())
		{
			OnCbnSelchangeCmbWPress();
			return;
		}

		CString strMessage = "";
		double fInputV = 0.0;
		fInputV = /*(float)*/atof(strText);


		switch (pItem->iRow)
		{
		case 2:
			{
				if (fInputV<0 || fInputV > 999999.99)
				{
					strMessage.Format("输入的上限值超出范围(0~999999.99),请重新输入!");
					MessageBox(strMessage,"提示",MB_OK|MB_ICONWARNING);

					strText.Format("%.2f",pPress_PARA->GetUpValue());
					CUdfGlobalFUNC::SetGridCellNormal(&m_wndGridCtrl,2,1);
					m_wndGridCtrl.SetItemText(2,1,strText);

					return;
				}

				pPress_PARA->SetUpValue(fInputV);
				strText.Format("%.2f",pPress_PARA->GetUpValue());
				CUdfGlobalFUNC::SetGridCellNormal(&m_wndGridCtrl,2,1);
				m_wndGridCtrl.SetItemText(2,1,strText);

			}
			break;
		case 3:
			{
				if (fInputV<0 || fInputV > 999999.99)
				{
					strMessage.Format("输入的下限值超出范围(0~999999.99),请重新输入!");
					MessageBox(strMessage,"提示",MB_OK|MB_ICONWARNING);

					strText.Format("%.2f",pPress_PARA->GetDownValue());
					CUdfGlobalFUNC::SetGridCellNormal(&m_wndGridCtrl,3,1);
					m_wndGridCtrl.SetItemText(3,1,strText);

					return;
				}

				pPress_PARA->SetDownValue(fInputV);
				strText.Format("%.2f",pPress_PARA->GetDownValue());
				CUdfGlobalFUNC::SetGridCellNormal(&m_wndGridCtrl,3,1);
				m_wndGridCtrl.SetItemText(3,1,strText);
			}
			break;
		}
	}
}

void CRtuAttrWaterPress::OnTimer(UINT_PTR nIDEvent)
{
	CString strLogMessge = "";
	_Log_level_type _logLevle = Normal_Log_Message;

	switch (nIDEvent)
	{
	case W_TIMER_WATERPRESS_PARA_QUERY:
		{
			if (m_blResult == TRUE)
				strLogMessge = "查询遥测终端水压基上、下限值 成功!";
			else if( m_blResult == FALSE)
			{
				strLogMessge = "查询遥测终端水压基上、下限值 超时!";
				_logLevle = Warning_log_Message;
			}
		}
		break;
	case W_TIMER_WATERPRESS_REAL_QUERY:
		{
			if (m_blResult == TRUE)
				strLogMessge = "查询遥测终端的水压实时值 成功!";
			else if( m_blResult == FALSE)
			{
				strLogMessge = "查询遥测终端的水压实时值 超时!";
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