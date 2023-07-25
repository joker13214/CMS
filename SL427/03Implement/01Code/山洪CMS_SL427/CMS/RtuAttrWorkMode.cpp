// RtuAttrWorkMode.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuAttrWorkMode.h"
#include "RtuPropertyDlg.h"




#define  W_TIMER_YCQUERY_CALL  0x01		//ң���ѯѡ���ѯTimer
#define  W_TIMER_YCQUERY_SET   0x02		//ң���ѯѡ������Timer


// CRtuAttrWorkMode �Ի���
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


// CRtuAttrWorkMode ��Ϣ�������

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


		CString strColHeaders[] ={"������","����ֵ"};

		m_wndGridCtrl.InsertColumn(strColHeaders[0],DT_LEFT|DT_VCENTER|DT_SINGLELINE,0);
		m_wndGridCtrl.SetColumnWidth(0,110);
		m_wndGridCtrl.InsertColumn(strColHeaders[1],DT_LEFT|DT_VCENTER|DT_SINGLELINE,1);
		m_wndGridCtrl.SetColumnWidth(1,80);

		CString strOptName[]={"ˮ������",
								"ˮλ����",
								"ˮѹ����",
								"ˮ������",
								"ˮ����������",
								"��������",
								"ģ��������",
								"���������",
								"λ��������",
								"�¶�������"
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
		options.Add(_T("��ѯ"));
		options.Add(_T("����ѯ"));

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
				m_wndGridCtrl.SetItemText(nIdx,1,"��ѯ");
			else if (bitValue == false)
				m_wndGridCtrl.SetItemText(nIdx,1,"����ѯ");
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
		if (strCellText == "��ѯ")
			m_pSubStation->GetYcQueryType()->SetQueryBitValue(pItem->iRow - 1, true);
		else if (strCellText == "����ѯ")
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
			m_pSubStation->BuildBasicPacket(appFrame);  //���ɳ�ʼ���·�֡����֡������Ϣ

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
				strLog = "��ѯң��վ��Ҫ��ѯ��ʵʱ�������� �ɹ�!";
			else if ( m_blResult == FALSE)
			{
				strLog = "��ѯң��վ��Ҫ��ѯ��ʵʱ�������� ��ʱ!";
				LogLevel = Warning_log_Message;
			}

		}
		break;
	case W_TIMER_YCQUERY_SET:
		{
			if (m_blResult == TRUE)
				strLog = "����ң��վ��Ҫ��ѯ��ʵʱ�������� �ɹ�!";
			else if (m_blResult == FALSE)
			{
				strLog = "����ң��վ��Ҫ��ѯ��ʵʱ�������� ��ʱ!";
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