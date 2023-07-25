// RtuAttrWorkMode.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuAttrWorkMode.h"
#include "RtuPropertyDlg.h"




#define  W_TIMER_YCQUERY_CALL  0x01		//遥测查询选项查询Timer
#define  W_TIMER_YCQUERY_SET   0x02		//遥测查询选项设置Timer


// CRtuAttrWorkMode 对话框
IMPLEMENT_DYNAMIC(CRtuAttrWorkMode, CDialog)
CRtuAttrWorkMode::CRtuAttrWorkMode(CWnd* pParent /*=NULL*/)
	: CDialog(CRtuAttrWorkMode::IDD, pParent)
{
	m_pParentDlg = NULL;
	m_pSubStation = NULL;

}

CRtuAttrWorkMode::~CRtuAttrWorkMode()
{
}

void CRtuAttrWorkMode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_YCQUERY_CALL, m_btnYcQueryCall);
	DDX_Control(pDX, IDC_YCQUERY_SET, m_btnYcQuerySet);
}


BEGIN_MESSAGE_MAP(CRtuAttrWorkMode, CDialog)
	ON_BN_CLICKED(IDC_YCQUERY_CALL, &CRtuAttrWorkMode::OnBnClickedYcqueryCall)
	ON_BN_CLICKED(IDC_YCQUERY_SET, &CRtuAttrWorkMode::OnBnClickedYcquerySet)
	ON_NOTIFY(GVN_ENDLABELEDIT,IDC_RTU_YCQUERYGRIDCTRL,&CRtuAttrWorkMode::OnGridEndEdit)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CRtuAttrWorkMode 消息处理程序

BOOL CRtuAttrWorkMode::OnInitDialog()
{
	CDialog::OnInitDialog();

	CWnd* pWnd = GetDlgItem(IDC_GC_YCQUERYPARA);
	CRect rect1, rect2;
	int captionHeight = 0; // ::GetSystemMetrics(SM_CYCAPTION);
	int cxframe = GetSystemMetrics(SM_CXFRAME);
	int cyframe = GetSystemMetrics(SM_CYFRAME);
	this->GetWindowRect(&rect2);
	if (pWnd)
		pWnd->GetWindowRect(&rect1);
	if (!m_wndGridCtrl.Create(CRect(rect1.left - rect2.left - cxframe, 
		rect1.top-rect2.top - cyframe-captionHeight, 
		rect1.left + rect1.Width() - rect2.left, 
		rect1.top + rect1.Height() - rect2.top - captionHeight), 
		this, IDC_RTU_YCQUERYGRIDCTRL,WS_CHILD|WS_VISIBLE))
	{
		TRACE("Failed to create threads  grid!\n");
		return FALSE;
	}

	m_wndGridCtrl.SetBkColor(RGB(255,255,255));
	m_wndGridCtrl.SetListMode(TRUE);
	m_wndGridCtrl.SetSingleColSelection(TRUE);
	m_wndGridCtrl.SetSingleRowSelection(TRUE);
	InitGridCtrl();

	return TRUE;  
}


void CRtuAttrWorkMode::InitGridCtrl()
{
	if (m_wndGridCtrl.GetSafeHwnd())
	{
		m_wndGridCtrl.DeleteAllItems();
		m_wndGridCtrl.SetRowCount(11);
		m_wndGridCtrl.SetFixedRowCount(1);


		CString strColHeaders[] ={"属性项","属性值"};

		m_wndGridCtrl.InsertColumn(strColHeaders[0],DT_LEFT|DT_VCENTER|DT_SINGLELINE,0);
		m_wndGridCtrl.SetColumnWidth(0,110);
		m_wndGridCtrl.InsertColumn(strColHeaders[1],DT_LEFT|DT_VCENTER|DT_SINGLELINE,1);
		m_wndGridCtrl.SetColumnWidth(1,80);

		CString strOptName[]={"水量数据",
								"水位数据",
								"水压数据",
								"水质数据",
								"水泵运行数据",
								"雨量数据",
								"模拟量数据",
								"电度量数据",
								"位移量数据",
								"温度量数据"
								};
		int idx = 0;
		for ( idx = 0; idx<10; idx ++)
		{
			m_wndGridCtrl.SetItemText(idx+1,0,strOptName[idx]);
			OnCellReadOnly(idx+1,0);
			OnCellNormal(idx+1,  1);
			InitSetYcQuery(idx+1,1);
		}

		for (idx = 0; idx < m_wndGridCtrl.GetRowCount(); idx ++)
			m_wndGridCtrl.SetRowHeight(idx,MFC_GRIDCTRL_HEIGHT);
	}
}

void CRtuAttrWorkMode::OnCellNormal(int Row,int Col)
{
	if (m_wndGridCtrl.GetSafeHwnd())
	{
		m_wndGridCtrl.SetCellType(Row,Col, RUNTIME_CLASS(CGridCell));
		m_wndGridCtrl.SetItemText(Row,Col, _T(""));
		m_wndGridCtrl.SetItemState(Row,Col, m_wndGridCtrl.GetItemState(Row,Col) & ~GVIS_READONLY);
		m_wndGridCtrl.Invalidate();
	}
}

void CRtuAttrWorkMode::OnCellReadOnly(int Row,int Col)
{
	if (m_wndGridCtrl.GetSafeHwnd())
		m_wndGridCtrl.SetItemState(Row,Col, m_wndGridCtrl.GetItemState(Row,Col) | GVIS_READONLY);

}

CString CRtuAttrWorkMode::GetCellText(int Row,int Col)
{
	CString strText = "";
	if(m_wndGridCtrl.GetSafeHwnd())
	{
		CGridCellBase* pCell = m_wndGridCtrl.GetCell(Row,Col);
		if (pCell != NULL)
		{
			strText = pCell->GetText();
			strText.Trim();
		}
	}

	return strText;
}

void CRtuAttrWorkMode::InitSetYcQuery(int Row,int Col)
{
	if (m_wndGridCtrl.GetSafeHwnd())
	{
		if (!m_wndGridCtrl.SetCellType(Row,Col, RUNTIME_CLASS(CGridCellCombo)))  return;

		CStringArray options;
		options.Add(_T("查询"));
		options.Add(_T("不查询"));

		CGridCellCombo *pCell = (CGridCellCombo*) m_wndGridCtrl.GetCell(Row,Col);
		pCell->SetOptions(options);
		pCell->SetStyle(CBS_DROPDOWNLIST);
	}
}

void CRtuAttrWorkMode::SetParentDlg(CRtuPropertyDlg* pDlg)
{
		m_pParentDlg = pDlg;	
}


void CRtuAttrWorkMode::Update()
{
	if (!m_pParentDlg)	return;
	m_pSubStation = m_pParentDlg->GetRtuStation();

	if (!m_pSubStation) return;

	if (m_wndGridCtrl.GetSafeHwnd() && m_wndGridCtrl.GetRowCount() > 1 &&
		m_pSubStation->GetYcQueryType() != NULL)
	{
		for (int nIdx = 1; nIdx < m_wndGridCtrl.GetRowCount(); nIdx ++)
		{
			bool bitValue =  m_pSubStation->GetYcQueryType()->GetQueryBitValue(nIdx - 1);
			if (bitValue == true)
				m_wndGridCtrl.SetItemText(nIdx,1,"查询");
			else if (bitValue == false)
				m_wndGridCtrl.SetItemText(nIdx,1,"不查询");
		}

		m_wndGridCtrl.Refresh();
	}
}

void CRtuAttrWorkMode::OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	if (!pItem || pItem->iRow <= 0 || pItem->iColumn != 1) return;

	CString strCellText = "";
	strCellText = GetCellText(pItem->iRow,pItem->iColumn);
	if (strCellText.IsEmpty()) return;

    if (m_pSubStation != NULL && m_pSubStation->GetYcQueryType() != NULL)
	{
		if (strCellText == "查询")
			m_pSubStation->GetYcQueryType()->SetQueryBitValue(pItem->iRow - 1, true);
		else if (strCellText == "不查询")
			m_pSubStation->GetYcQueryType()->SetQueryBitValue(pItem->iRow - 1, false);
	}

}



void CRtuAttrWorkMode::OnBnClickedYcqueryCall()
{
	UpdateData(TRUE);
	if (m_pSubStation != NULL)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{
			SetWndsEnable(FALSE);
			m_blResult = FALSE;


			CPacketFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息

			m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_54H(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)  *1000;
			SetTimer(W_TIMER_YCQUERY_CALL,nElapse,NULL);
		}
	}	
}

void CRtuAttrWorkMode::OnBnClickedYcquerySet()
{
	UpdateData(TRUE);
	if (m_pSubStation != NULL)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{
			CPacketFrameFormat appFrame ;
			m_pSubStation->BuildBasicPacket(appFrame);

			m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_14H(&appFrame);
		}
	}
}


void CRtuAttrWorkMode::OnTimer(UINT_PTR nIDEvent)
{
	CString strLog= "";
	_Log_level_type LogLevel = Normal_Log_Message;


	switch (nIDEvent)
	{
	case W_TIMER_YCQUERY_CALL:
		{
			if (m_blResult == TRUE)
				strLog = "查询遥测站需要查询的实时数据种类 成功!";
			else if ( m_blResult == FALSE)
			{
				strLog = "查询遥测站需要查询的实时数据种类 超时!";
				LogLevel = Warning_log_Message;
			}

		}
		break;
	case W_TIMER_YCQUERY_SET:
		{
			if (m_blResult == TRUE)
				strLog = "设置遥测站需要查询的实时数据种类 成功!";
			else if (m_blResult == FALSE)
			{
				strLog = "设置遥测站需要查询的实时数据种类 超时!";
				LogLevel = Warning_log_Message;
			}
		}
		break;
	}
	KillTimer(nIDEvent);
	SetWndsEnable(TRUE);
	if (m_pParentDlg && m_pParentDlg->GetSafeHwnd())
		m_pParentDlg->AddLog(strLog,(PSYSTEMTIME)NULL,LogLevel);

	if (m_pSubStation)
		m_pSubStation->InsertLogMessage(strLog,LogLevel,NULL);

	CDialog::OnTimer(nIDEvent);
}

void CRtuAttrWorkMode::SetWndsEnable(BOOL blEnable /* = TRUE */)
{


	if (m_btnYcQueryCall.GetSafeHwnd())
		m_btnYcQueryCall.EnableWindow(blEnable);

	if (m_btnYcQuerySet.GetSafeHwnd())
		m_btnYcQuerySet.EnableWindow(blEnable);

	if (m_wndGridCtrl.GetSafeHwnd())
		m_wndGridCtrl.EnableWindow(blEnable);
}


void CRtuAttrWorkMode::UpdateDlg(int attrType,BOOL blResult)
{
	m_blResult = blResult;
	if (attrType == rtu_attr_ycquerypara_call)
	{
		SendMessage(WM_TIMER,W_TIMER_YCQUERY_CALL,NULL);
	}
	else if (attrType == rtu_attr_ycquerypara_set)
	{
		SendMessage(WM_TIMER,W_TIMER_YCQUERY_SET,NULL);
	}
}

void CRtuAttrWorkMode::KillAllTimers()
{
	KillTimer(W_TIMER_YCQUERY_CALL);
	KillTimer(W_TIMER_YCQUERY_SET);
	SetWndsEnable(TRUE);
}