// RtuAttrHydrRain.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuAttrHydrRain.h"

#define W_TIMER_WATERLEVEL_PARA_QUERY 0x01 //ˮλ������ѯ
#define W_TIMER_WATERLEVEL_REAL_QUERY 0x02  //ˮλʵʱ���ݲ�ѯ

#define W_TIMER_RAIN_DATA_QUERY 0x01 //ˮλ������ѯ
#define W_TIMER_RAIN_DATA_QUERY_ER 0x02  //ˮλʵʱ���ݲ�ѯ

// CRtuAttrHydrRain �Ի���

IMPLEMENT_DYNAMIC(CRtuAttrHydrRain, CDialog)

CRtuAttrHydrRain::CRtuAttrHydrRain(CWnd* pParent /*=NULL*/)
	: CDialog(CRtuAttrHydrRain::IDD, pParent)
{
	m_blResult = FALSE;
	m_pSubStation = NULL;
	m_pParentDlg = NULL;

}

CRtuAttrHydrRain::~CRtuAttrHydrRain()
{
}

void CRtuAttrHydrRain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TIMEIVL, m_wndComboBox);
	DDX_Control(pDX, IDC_CBX_TIMETYPE, m_wndTimeTypeCbx);
	DDX_Control(pDX,IDC_D_START ,m_wndDateStartDtc);
	DDX_Control(pDX,IDC_D_END ,m_wndDateEndDtc);
	DDX_Control(pDX,IDC_STATIC_START ,m_wndTimeStartLabel);
	DDX_Control(pDX,IDC_STATIC_END ,m_wndTimeEndLabel);
	DDX_Control(pDX, IDC_BTN_QUERYRAINVAR, m_btnQueryRain);
}

BEGIN_MESSAGE_MAP(CRtuAttrHydrRain, CDialog)
	ON_CBN_SELCHANGE(IDC_TIMEIVL, &CRtuAttrHydrRain::OnSelComboxChangeTimeInterval)
	ON_BN_CLICKED(IDC_BTN_QUERYRAINVAR,&CRtuAttrHydrRain::OnBnClickedBtnRainacall)
	ON_NOTIFY(GVN_ENDLABELEDIT, IDC_RAINALERT_GRID, &CRtuAttrHydrRain::OnEndEditRainGrid)
	ON_CBN_SELCHANGE(IDC_CBX_TIMETYPE,      &CRtuAttrHydrRain::OnCbnSelChangeTimeType)
	ON_WM_TIMER()
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_D_START, &CRtuAttrHydrRain::OnStartDataTimeChange)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_D_END, &CRtuAttrHydrRain::OnEndDataTimeChange)
END_MESSAGE_MAP()


// CRtuAttrHydrRain ��Ϣ�������
void CRtuAttrHydrRain::SetParentDlg(CRtuPropertyDlg* pDlg)
{
	m_pParentDlg = pDlg;

}

void CRtuAttrHydrRain::Update()										//���½������ʾ��Ϣ
{
	if (m_pParentDlg)
		m_pSubStation = m_pParentDlg->GetRtuStation();	


	if (m_wndRainGrid.GetSafeHwnd())
	{
		m_wndRainGrid.DeleteAllItems();
		m_wndRainGrid.Refresh();
	}

	if (m_pSubStation != NULL)
	{
		CHydrRainValue *pData = m_pSubStation->GetHydrRainVal();
		int nSel = m_wndComboBox.GetCurSel();
		if (CB_ERR == nSel)
			return;

		CString strText = "";
		m_wndComboBox.GetLBText(nSel, strText);
		if (pData->GetTimespan() == atoi(strText)
			|| (_T("ģ����ѯ") == strText))
		{
			OnSelComboxChangeTimeInterval();
		}		
	}
}

void CRtuAttrHydrRain::UpdateDlg(int attrType,BOOL blResult)			//���յ����ĺ󡢸��½���
{
	m_blResult = blResult;
	if (attrType == scsw_waterRain_call)
		SendMessage(WM_TIMER,W_TIMER_RAIN_DATA_QUERY,NULL);

}

void CRtuAttrHydrRain::KillAllTimers()
{
	KillTimer(W_TIMER_RAIN_DATA_QUERY);
	KillTimer(W_TIMER_WATERLEVEL_REAL_QUERY);
	SetWndsEnable(TRUE);
}

void CRtuAttrHydrRain::SetWndsEnable(BOOL blEnable)
{
	m_wndComboBox.EnableWindow(blEnable);
	m_wndTimeTypeCbx.EnableWindow(blEnable);
	m_wndDateStartDtc.EnableWindow(blEnable);
	m_wndDateEndDtc.EnableWindow(blEnable);
	m_wndRainGrid.EnableWindow(blEnable);
	m_btnQueryRain.EnableWindow(blEnable);
}

BOOL CRtuAttrHydrRain::OnInitDialog()
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
	if (!m_wndRainGrid.Create(CRect(rect1.left - rect2.left - cxframe, 
		rect1.top-rect2.top - cyframe-captionHeight, 
		rect1.left + rect1.Width() - rect2.left, 
		rect1.top + rect1.Height() - rect2.top - captionHeight), 
		this, IDC_WATERLEVEL_GRIDCTRL,WS_CHILD|WS_VISIBLE))
	{
		TRACE("Failed to create threads  grid!\n");
		return FALSE;
	}

	m_wndRainGrid.SetBkColor(RGB(255,255,255));
	m_wndRainGrid.SetListMode(TRUE);
	m_wndRainGrid.SetSingleColSelection(TRUE);
	m_wndRainGrid.SetSingleRowSelection(TRUE);

	m_wndComboBox.SetCurSel(0);
	m_wndTimeTypeCbx.SetCurSel(0);
	OnCbnSelChangeTimeType();

	return TRUE;
}


void CRtuAttrHydrRain::InitGridItems()
{
	int nSel = 0;
	if (m_wndRainGrid.GetSafeHwnd())
	{
		m_wndRainGrid.DeleteAllItems();
		if (!m_pSubStation) return;

		CString strHeader[] = {
			"ʱ��",
			"����ֵ",
			"��λ"
		};

		CHydrRainValue *pData = m_pSubStation->GetHydrRainVal();
		if (NULL == pData) return;

		pData->m_mutex.Lock();
		int nListCnt = pData->m_lstRainVal.size();
		pData->m_mutex.Unlock();

		int nRowCnt = 0;
		if (nListCnt <= 5)
		{
			nRowCnt = 7;
		}
		else
		{
			nRowCnt = nListCnt;

		}

		m_wndRainGrid.SetRowCount(nRowCnt);
		m_wndRainGrid.SetColumnCount(3);
		m_wndRainGrid.SetColumnWidth(0,120);
		m_wndRainGrid.SetColumnWidth(1,120);
		m_wndRainGrid.SetColumnWidth(2,120);

		for (int i=0; i<3; i++)
		{
			m_wndRainGrid.SetItemText(1,i,strHeader[i]);
			m_wndRainGrid.SetItemBkColour(1,i,RGB(128,128,128));
			m_wndRainGrid.SetRowHeight(i,MFC_GRIDCTRL_HEIGHT);
		}

		CString strSpan = "";
		if (0 == nListCnt)
		{
			m_wndRainGrid.SetItemText(0,0,_T("ʱ����Ϊ��"));
			m_wndRainGrid.SetItemText(0,1,_T("��"));
			m_wndRainGrid.SetItemText(0,2,_T("ԭ��δ�յ���Ч��������"));
			m_wndRainGrid.SetColumnWidth(0,120);
			m_wndRainGrid.SetColumnWidth(1,50);
			m_wndRainGrid.SetColumnWidth(2,200);
		}	
		else
		{
			if (pData->GetDw() == 0)
			{
				strSpan.Format("%-2d����", pData->GetTimespan());
			}
			else
			{
				strSpan.Format("%-2dСʱ", pData->GetTimespan());
			}
			
			m_wndRainGrid.SetItemText(0,0,_T("ʱ����Ϊ��"));
			m_wndRainGrid.SetItemText(0,1,strSpan);

		}

		m_wndRainGrid.SetItemBkColour(0,0,RGB(207,243,253));
		m_wndRainGrid.SetRowHeight(0,MFC_GRIDCTRL_HEIGHT);
		CUdfGlobalFUNC::SetGridCellReadOnly(&m_wndRainGrid,1,0);
		CUdfGlobalFUNC::SetGridCellReadOnly(&m_wndRainGrid,1,1);

		pData->m_mutex.Lock();
		std::list<float>::iterator begin = pData->m_lstRainVal.begin();
		std::list<float>::iterator end = pData->m_lstRainVal.end();
		pData->m_mutex.Unlock();

		std::list<float>::iterator it;
		int idx = 2;

		struct tm t_temp, t;
		t.tm_hour = pData->GetTime().tm_hour;
		t.tm_min = pData->GetTime().tm_min;

		CString rainTime = "";
		CString tempTime = "";
		CString var = "";

		pData->m_mutex.Lock();
		for (it=begin; it!=end; it++)
		{
			tempTime = "";
			rainTime = "";
			var = "";

			t_temp = t;
			tempTime = CUdfGlobalFUNC::CalcTime(&t, pData->GetTimespan(), 2);
			rainTime.Format("%s��%d:%.2d", tempTime, t_temp.tm_hour, t_temp.tm_min);
			m_wndRainGrid.SetItemText(idx, 0, rainTime);

			var.Format("%-2f", (float)(*it));
			m_wndRainGrid.SetItemText(idx, 1, var);

			m_wndRainGrid.SetItemText(idx, 2, _T("0.1����"));
			idx++;
		}

		pData->m_mutex.Unlock();
		m_wndRainGrid.Refresh();


	}
}
	
	

void CRtuAttrHydrRain::OnTimer(UINT_PTR nIDEvent)
{
	CString strLogMessge = "";
	_Log_level_type _logLevle = Normal_Log_Message;

	switch (nIDEvent)
	{
	case W_TIMER_RAIN_DATA_QUERY:
		{
			if (m_blResult == TRUE)
				strLogMessge = "��ѯң���ն˵�����ʵʱֵ �ɹ�!";
			else if( m_blResult == FALSE)
			{
				strLogMessge = "��ѯң���ն˵�����ʵʱֵ ��ʱ!";
				_logLevle = Warning_log_Message;
			}
			else if (m_blResult == 0xff)
			{
				strLogMessge = "��ѯң���ն˵�����ʵʱֵ ����!";
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

void CRtuAttrHydrRain::OnBnClickedBtnRainacall()
{
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{

			int nsel_itvl = m_wndComboBox.GetCurSel();
			if (CB_ERR == nsel_itvl)
			{
				MessageBox("��ѡ����ʵ�ʱ����","��ʾ",MB_OK|MB_ICONERROR);
				return;
			}

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CHydrSendFrame appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //���ɳ�ʼ���·�֡����֡������Ϣ
	
			int nsel = m_wndTimeTypeCbx.GetCurSel();
			if (CB_ERR == nsel)
			{
				MessageBox("��ѡ����ʵ�ʱ������","��ʾ",MB_OK|MB_ICONERROR);
				return;
			}

			if (0 == nsel)
			{
				appFrame.m_nTimeType = TIMETYPE_CURR;
				appFrame.t_a = m_timeA;

			}
			else if (1==nsel)
			{
				appFrame.m_nTimeType = TIMETYPE_TB;
				appFrame.t_a = m_timeA;
				appFrame.t_b = m_timeB;
			}
			else
			{
				appFrame.m_nTimeType = TIMETYPE_TT;
				appFrame.t_a = m_timeA;
			}

			CString strSensor = "";
			m_wndComboBox.GetWindowText(strSensor);
			if (strSensor == _T("ģ����ѯ"))
			{
				m_pSubStation->GetSysDataTrans_Hydr()->SCSW_hydrdata_EN_P_MH(&appFrame);
				UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
				SetTimer(W_TIMER_WATERLEVEL_PARA_QUERY, nElapse, NULL);
			}
			else
			{
				if (strSensor.GetLength()!=5)
				{
					strSensor = strSensor.Left(2);
				}
				else
				{
					strSensor = strSensor.Left(1);
					appFrame.m_LPARME = atoi(strSensor);
					m_pSubStation->GetSysDataTrans_Hydr()->SCSW_hydrdata_EN_P(&appFrame);

					UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
					SetTimer(W_TIMER_WATERLEVEL_PARA_QUERY, nElapse, NULL);

				}
			}
					
		}
	}
}

void CRtuAttrHydrRain::OnEndEditRainGrid(NMHDR *pNotifyStruct, LRESULT* pResult)
{

}

void CRtuAttrHydrRain::OnSelComboxChangeTimeInterval()
{
	InitGridItems();
}

void CRtuAttrHydrRain::OnCbnSelChangeTimeType()
{
	if (m_wndTimeTypeCbx.GetSafeHwnd())
	{
		SYSTEMTIME sysTime;
		int nSel = m_wndTimeTypeCbx.GetCurSel();
		if (CB_ERR == nSel) return;

		switch(nSel)
		{
		case 0://��ǰʱ��
			m_wndTimeStartLabel.ShowWindow(SW_HIDE);
			m_wndDateStartDtc.ShowWindow(SW_HIDE);
			m_wndTimeEndLabel.ShowWindow(SW_HIDE);
			m_wndDateEndDtc.ShowWindow(SW_HIDE);

			m_wndDateStartDtc.SetFormat("yyyy��MM��dd�� HHʱmm��ss��");
			memset(&sysTime, 0, sizeof(sysTime));
			::GetLocalTime(&sysTime);
			VERIFY(m_wndDateStartDtc.SetTime(&sysTime));
			m_timeA = sysTime;
			break;
		case 1://��ʷ��ѯ
			m_wndTimeStartLabel.SetWindowText("��ʼʱ��");
			m_wndTimeEndLabel.SetWindowText("����ʱ��");
			m_wndTimeEndLabel.ShowWindow(SW_SHOW);
			m_wndDateEndDtc.ShowWindow(SW_SHOW);
			m_wndTimeStartLabel.ShowWindow(SW_SHOW);
			m_wndDateStartDtc.ShowWindow(SW_SHOW);

			m_wndDateStartDtc.SetFormat("yyyy��MM��dd�� HHʱmm��ss��");
			m_wndDateEndDtc.SetFormat("yyyy��MM��dd�� HHʱmm��ss��");
			memset(&sysTime, 0, sizeof(sysTime));
			::GetLocalTime(&sysTime);
			VERIFY(m_wndDateStartDtc.SetTime(&sysTime));
			VERIFY(m_wndDateEndDtc.SetTime(&sysTime));
			m_timeA = sysTime;
			m_timeB = sysTime;
			break;
		}
	}

}



void CRtuAttrHydrRain::OnStartDataTimeChange(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_timeA = pDTChange->st;
	*pResult = 0;
}

void CRtuAttrHydrRain::OnEndDataTimeChange(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_timeB = pDTChange->st;
	*pResult = 0;
}
