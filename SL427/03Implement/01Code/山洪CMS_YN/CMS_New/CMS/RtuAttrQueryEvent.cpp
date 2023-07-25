// RtuAttrQueryEvent.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuAttrQueryEvent.h"

#define  W_TIMER_QUERYEVENT 0x01
// CRtuAttrQueryEvent �Ի���

IMPLEMENT_DYNAMIC(CRtuAttrQueryEvent, CDialog)

CRtuAttrQueryEvent::CRtuAttrQueryEvent(CWnd* pParent /*=NULL*/)
	: CDialog(CRtuAttrQueryEvent::IDD, pParent)
{
	m_blResult = FALSE;
	m_pSubStation = NULL;
	m_pParentDlg = NULL;
}

CRtuAttrQueryEvent::~CRtuAttrQueryEvent()
{
}

void CRtuAttrQueryEvent::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_BTN_QUERYEVENT,m_btnQueryEvent);
}


BEGIN_MESSAGE_MAP(CRtuAttrQueryEvent, CDialog)
	ON_BN_CLICKED(IDC_BTN_QUERYEVENT,&CRtuAttrQueryEvent::OnBnClickBtnQueryEvent)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CRtuAttrQueryEvent ��Ϣ�������

void CRtuAttrQueryEvent::SetParentDlg(CRtuPropertyDlg* pDlg)
{
	m_pParentDlg = pDlg;

}

void CRtuAttrQueryEvent::Update()										//���½������ʾ��Ϣ
{
	if (m_pParentDlg)
		m_pSubStation = m_pParentDlg->GetRtuStation();

	InitGridCtrl();
}

void CRtuAttrQueryEvent::UpdateDlg(int attrType,BOOL blResult)			//���յ����ĺ󡢸��½���
{
	m_blResult = blResult;
	if (attrType == rtu_attr_eventrecord_call)
		SendMessage(WM_TIMER,W_TIMER_QUERYEVENT,NULL);
	
	InitGridCtrl();
}

void CRtuAttrQueryEvent::KillAllTimers()
{
	KillTimer(W_TIMER_QUERYEVENT);
	SetWndsEnable(TRUE);
}

void CRtuAttrQueryEvent::SetWndsEnable(BOOL blEnable)					//���ð�ť�Ƿ����
{
	m_btnQueryEvent.EnableWindow(blEnable);
}

BOOL CRtuAttrQueryEvent::OnInitDialog()
{
	CDialog::OnInitDialog();

	CWnd* pWnd = GetDlgItem(IDC_STATIC_QUERYEVENTGRID);
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
		this, IDC_QUERYEVENT_GRIDCTRL,WS_CHILD|WS_VISIBLE))
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


void CRtuAttrQueryEvent::OnBnClickBtnQueryEvent()
{
	if( m_pSubStation)
	{

		POSITION rPos = m_pSubStation->GetEventRecordsList()->GetHeadPosition();
		while (rPos != NULL)
		{
			CEventRecord_PER* pEvent = m_pSubStation->GetEventRecordsList()->GetNext(rPos);
			if (pEvent)
				pEvent->wEventCount = 0;
		}

		InitGridCtrl();

		if (m_pSubStation->blEnbaleTrans() == true)
		{

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CPacketFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //���ɳ�ʼ���·�֡����֡������Ϣ

			m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_5DH(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_QUERYEVENT,nElapse,NULL);
		}
	}
}

void CRtuAttrQueryEvent::OnTimer(UINT_PTR nIDEvent)
{

	CString strLogMessge = "";
	_Log_level_type _logLevle = Normal_Log_Message;

	switch (nIDEvent)
	{
	case W_TIMER_QUERYEVENT:
		{
			if (m_blResult == TRUE)
				strLogMessge = "��ѯң���ն˵��¼���¼ �ɹ�!";
			else if( m_blResult == FALSE)
			{
				strLogMessge = "��ѯң���ն˵��¼���¼ ��ʱ!";
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

void CRtuAttrQueryEvent::InitGridCtrl()
{
	if (m_wndGridCtrl.GetSafeHwnd())
	{
		m_wndGridCtrl.DeleteAllItems();

		CString strHeaders[] = {"����","�¼���¼��"};

		if (m_pSubStation)
		{
			int rowCount = (int)m_pSubStation->GetEventRecordsList()->GetCount() +1;
			m_wndGridCtrl.SetRowCount(rowCount);
			m_wndGridCtrl.SetColumnCount(2);
			m_wndGridCtrl.SetFixedRowCount(1);

			m_wndGridCtrl.SetColumnWidth(0,170);
			m_wndGridCtrl.SetColumnWidth(1,120);

			m_wndGridCtrl.SetItemText(0,0,strHeaders[0]);
			m_wndGridCtrl.SetItemText(0,1,strHeaders[1]);

			for (int i= 0 ; i < rowCount -1; i ++)
			{
				CString strTemp = "";
				CEventRecord_PER* pEvent = m_pSubStation->GetEventRecordObject(i);
				if (pEvent)
				{
					m_wndGridCtrl.SetItemText(i + 1,0,pEvent->strName);
					strTemp.Format("%d",pEvent->wEventCount);
					m_wndGridCtrl.SetItemText(i + 1,1,strTemp);
				}

				CUdfGlobalFUNC::SetGridCellReadOnly(&m_wndGridCtrl,i +1,0);
				CUdfGlobalFUNC::SetGridCellReadOnly(&m_wndGridCtrl,i +1,1);
				m_wndGridCtrl.SetRowHeight(i +1,MFC_GRIDCTRL_HEIGHT);
			}
		}
		m_wndGridCtrl.Refresh();
	}
}