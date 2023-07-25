// RtuAttrAutoRpSet.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuAttrAutoRpSet.h"
#include "RtuPropertyDlg.h"
#include "RtuStation.h"


#define  W_TIMER_AUTOPARA_CALL 0x01     //�������ϱ�����Timer
#define  W_TIMER_AUTOPARA_SET  0x02     //���������ϱ�����Timer

// CRtuAttrAutoRpSet �Ի���

IMPLEMENT_DYNAMIC(CRtuAttrAutoRpSet, CDialog)

CRtuAttrAutoRpSet::CRtuAttrAutoRpSet(CWnd* pParent /*=NULL*/)
	: CDialog(CRtuAttrAutoRpSet::IDD, pParent)
{
	m_pParentDlg = NULL;
	m_pSubStation = NULL;
}

CRtuAttrAutoRpSet::~CRtuAttrAutoRpSet()
{
}

void CRtuAttrAutoRpSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_AUTOSET, m_btnAutoSet);
	DDX_Control(pDX, IDC_BTN_AUTOQUERY, m_btnAutoQuery);
}


BEGIN_MESSAGE_MAP(CRtuAttrAutoRpSet, CDialog)
	ON_BN_CLICKED(IDC_BTN_AUTOSET,				&CRtuAttrAutoRpSet::OnBnClickedBtnAutoset)
	ON_BN_CLICKED(IDC_BTN_AUTOQUERY,			&CRtuAttrAutoRpSet::OnBnClickedBtnAutoquery)
	ON_NOTIFY(GVN_ENDLABELEDIT,IDC_RTU_AUTOGRID,&CRtuAttrAutoRpSet::OnGridEndEdit)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CRtuAttrAutoRpSet ��Ϣ�������
BOOL CRtuAttrAutoRpSet::OnInitDialog()
{
	CDialog::OnInitDialog();

	CWnd* pWnd = GetDlgItem(IDC_STATIC_AUTOGRID);
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
		this, IDC_RTU_AUTOGRID,WS_CHILD|WS_VISIBLE))
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


void CRtuAttrAutoRpSet::InitGridCtrl()
{
	if (m_wndGridCtrl.GetSafeHwnd())
	{
		m_wndGridCtrl.DeleteAllItems();
		m_wndGridCtrl.SetRowCount(16);
		m_wndGridCtrl.SetFixedRowCount(1);


		CString strColHeaders[] ={"������","����ֵ"};
		m_wndGridCtrl.InsertColumn(strColHeaders[0],DT_LEFT|DT_VCENTER|DT_SINGLELINE,0);
		m_wndGridCtrl.SetColumnWidth(0,180);
		m_wndGridCtrl.InsertColumn(strColHeaders[1],DT_LEFT|DT_VCENTER|DT_SINGLELINE,1);
		m_wndGridCtrl.SetColumnWidth(1,90);


		CString strOptName[]={"ˮ������",
		                      "ˮλ����",
							  "ˮѹ����",
							  "ˮ������",
							  "�ۺ�����",
							  "ˮ����������",
							  "�¼�������������",
							  "�±���״̬���¼�",
							  "��/��ˮ��ʱ����״̬���¼�",
							  "��������",
							  "ģ��������",
							  "���������",
							  "λ��������",
							  "�¶�������",
							  "�Ա�ʱ����(min)"
							  };

		int idx = 0;
		for ( idx = 0; idx<15; idx ++)
		{
			m_wndGridCtrl.SetItemText(idx+1,0,strOptName[idx]);
			OnCellReadOnly(idx+1,0);
			OnCellNormal(idx+1,  1);
			if (idx < 14)
				InitSetCellAuto(idx+1,1);
		}
		

		OnCellNormal(15,1);
		if (!m_wndGridCtrl.SetCellType(15,1, RUNTIME_CLASS(CGridCellNumeric)))
			return;

		CString _sTemp = "0";
		m_wndGridCtrl.SetItemText(15,1, _sTemp);

		for (idx = 0; idx < m_wndGridCtrl.GetRowCount(); idx ++)
		{
			m_wndGridCtrl.SetRowHeight(idx,MFC_GRIDCTRL_HEIGHT);
		}
	}
}


void CRtuAttrAutoRpSet::OnCellNormal(int Row,int Col)
{
	if (m_wndGridCtrl.GetSafeHwnd())
	{
		m_wndGridCtrl.SetCellType(Row,Col, RUNTIME_CLASS(CGridCell));
		m_wndGridCtrl.SetItemText(Row,Col, _T(""));
		m_wndGridCtrl.SetItemState(Row,Col, m_wndGridCtrl.GetItemState(Row,Col) & ~GVIS_READONLY);
		m_wndGridCtrl.Invalidate();
	}
}

void CRtuAttrAutoRpSet::OnCellReadOnly(int Row,int Col)
{
	if (m_wndGridCtrl.GetSafeHwnd())
	{
		m_wndGridCtrl.SetItemState(Row,Col, m_wndGridCtrl.GetItemState(Row,Col) | GVIS_READONLY);
	}
}


void CRtuAttrAutoRpSet::InitSetCellAuto(int Row,int Col)
{
	if (m_wndGridCtrl.GetSafeHwnd())
	{
		if (!m_wndGridCtrl.SetCellType(Row,Col, RUNTIME_CLASS(CGridCellCombo)))
			return;

		CStringArray options;
		options.Add(_T("�����ϱ�"));
		options.Add(_T("�������ϱ�"));

		CGridCellCombo *pCell = (CGridCellCombo*) m_wndGridCtrl.GetCell(Row,Col);
		pCell->SetOptions(options);
		pCell->SetStyle(CBS_DROPDOWNLIST);
	}
}


void CRtuAttrAutoRpSet::Update()
{
	if (!m_pParentDlg) return;

	m_pSubStation = m_pParentDlg->GetRtuStation();
	if (m_pSubStation == NULL)
		return;


	if (m_wndGridCtrl.GetSafeHwnd() && 
		m_wndGridCtrl.GetRowCount() > 3 &&
		m_pSubStation->GetAutoReportPara() != NULL)
	{
		for (int nIdx = 1; nIdx < m_wndGridCtrl.GetRowCount()-1; nIdx ++)
		{
			bool autoBitValue = m_pSubStation->GetAutoReportPara()->GetAutoBitValue(nIdx - 1);
			if (autoBitValue == true)
				m_wndGridCtrl.SetItemText(nIdx,1,"�����ϱ�");
			else if (autoBitValue == false)
				m_wndGridCtrl.SetItemText(nIdx,1,"�������ϱ�");
		}

		WORD autoInterval = m_pSubStation->GetAutoReportPara()->GetAutoInterval();
		CString strInterval = "";
		strInterval.Format("%d",autoInterval);
		m_wndGridCtrl.SetItemText(m_wndGridCtrl.GetRowCount()-1,1,strInterval);

		m_wndGridCtrl.Refresh();
	}
}

void CRtuAttrAutoRpSet::OnBnClickedBtnAutoset()
{
	UpdateData(TRUE);
	if (m_pSubStation )
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{
			CPacketFrameFormat appFrame ;
			m_pSubStation->BuildBasicPacket(appFrame);

			//�����Ա���������ʱ�������ģ����·�����
			m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_13H(&appFrame);
		}
	}
}

void CRtuAttrAutoRpSet::OnBnClickedBtnAutoquery()
{

	UpdateData(TRUE);
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CPacketFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //���ɳ�ʼ���·�֡����֡������Ϣ

			m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_53H(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_AUTOPARA_CALL,nElapse,NULL);
		}
	}
}


void CRtuAttrAutoRpSet::OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	if (!pItem  || pItem->iRow <= 0  || pItem->iColumn != 1 ) return;
	if (!m_pSubStation) return;
	if (!m_pSubStation->GetAutoReportPara()) return;


	int rowCount = m_wndGridCtrl.GetRowCount();
	CString strCellText = "";
	strCellText = GetCellText(pItem->iRow,pItem->iColumn);
	if (strCellText.IsEmpty()) return;

	if (pItem->iRow >= 1 && 
		pItem->iRow < rowCount - 1 )
	{
		if (strCellText == "�����ϱ�")
			m_pSubStation->GetAutoReportPara()->SetAutoBitValue(pItem->iRow - 1, true);
		else if (strCellText ="�������ϱ�")
			m_pSubStation->GetAutoReportPara()->SetAutoBitValue(pItem->iRow - 1, false);	
	}
	else if (pItem->iRow == rowCount - 1)
	{
		//
		int nInterval = atoi(strCellText);
		if (nInterval > 9999 ||
			nInterval < 0)
		{
			MessageBox("�Ա�ʱ����(min)������Χ(0~9999),����������.","��ʾ",MB_OK|MB_ICONERROR);
			m_wndGridCtrl.SetItemText(pItem->iRow,pItem->iColumn,"0");
			nInterval = 0;
		}
		else
		{
			//nInterval	  = (WORD)nPort;
			CString sTemp ="";
			sTemp.Format("%d",nInterval);
			m_wndGridCtrl.SetItemText(pItem->iRow,pItem->iColumn,sTemp);
		}

		//m_pSubStation->GetAutoReportPara()->SetAutoInterval((WORD)atoi(strCellText));
		m_pSubStation->GetAutoReportPara()->SetAutoInterval((WORD)nInterval);
	}

}


CString CRtuAttrAutoRpSet::GetCellText(int Row,int Col)
{
	CString strCellText = "";
	if (m_wndGridCtrl.GetSafeHwnd())
	{
		CGridCellBase* pCell = m_wndGridCtrl.GetCell(Row,Col);
		if (pCell != NULL)
		{
			strCellText=pCell->GetText();
			strCellText.Trim();
		}
	}

	return strCellText;
}



void CRtuAttrAutoRpSet::OnTimer(UINT_PTR nIDEvent)
{

	CString strLog = "";
	_Log_level_type  logLevel = Normal_Log_Message;

	switch (nIDEvent)
	{
	case W_TIMER_AUTOPARA_CALL:
		{
			KillTimer(nIDEvent);
			SetWndsEnable(TRUE);
			
			
			if (m_blResult == TRUE)
				strLog = "��ѯң���ն˵������Ա����༰ʱ�������� �ɹ�!";
			else if (m_blResult == FALSE)
			{
				strLog = "��ѯң���ն˵������Ա����༰ʱ�������� ��ʱ!";
				logLevel = Warning_log_Message;
			}
		}
		break;
	case W_TIMER_AUTOPARA_SET:
		{
			KillTimer(nIDEvent);
			SetWndsEnable(TRUE);

			if (m_blResult == TRUE)
				strLog = "����ң���ն˵������Ա����༰ʱ�������� �ɹ�!";
			else if (m_blResult == FALSE)
			{
				strLog = "����ң���ն˵������Ա����༰ʱ�������� ��ʱ!";
				logLevel = Warning_log_Message;
			}
		}
		break;
	}
	if (m_pParentDlg && m_pParentDlg->GetSafeHwnd())
		m_pParentDlg->AddLog(strLog,NULL,logLevel);

	if (m_pSubStation)
		m_pSubStation->InsertLogMessage(strLog,logLevel,NULL);

	CDialog::OnTimer(nIDEvent);
}

void CRtuAttrAutoRpSet::SetWndsEnable(BOOL blEnable /* = TRUE */)
{
	if (m_btnAutoQuery.GetSafeHwnd())
		m_btnAutoQuery.EnableWindow(blEnable);

	if (m_btnAutoSet.GetSafeHwnd())
		m_btnAutoSet.EnableWindow(blEnable);

	if (m_wndGridCtrl.GetSafeHwnd())
		m_wndGridCtrl.EnableWindow(blEnable);
}


void CRtuAttrAutoRpSet::UpdateDlg(int attrType,BOOL blResult)
{
	m_blResult = blResult;
	if (attrType == rtu_attr_autoRpPara_call)
		SendMessage(WM_TIMER,W_TIMER_AUTOPARA_CALL,NULL);
	else if (attrType == rtu_attr_atuoRpPara_set)
		SendMessage(WM_TIMER,W_TIMER_AUTOPARA_SET, NULL);
}

void CRtuAttrAutoRpSet::KillAllTimers()
{
	KillTimer(W_TIMER_AUTOPARA_SET);
	KillTimer(W_TIMER_AUTOPARA_CALL);
	SetWndsEnable(TRUE);
}