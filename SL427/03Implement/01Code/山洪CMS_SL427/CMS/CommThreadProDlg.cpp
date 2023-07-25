// CommThreadProDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CMS.h"
#include "CommThreadProDlg.h"


// CCommThreadProDlg �Ի���

IMPLEMENT_DYNAMIC(CCommThreadProDlg, CDialog)

CCommThreadProDlg::CCommThreadProDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCommThreadProDlg::IDD, pParent)
	, m_strThreadInfos(_T(""))
{
	m_pCurCommThreadInfo = NULL;
	m_brMine = NULL;
	m_bProtocolType = UDP_Protocol;
	//m_nSCAddress = 0;
}

CCommThreadProDlg::~CCommThreadProDlg()
{

}

void CCommThreadProDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_THREADINFO, m_strThreadInfos);
	DDX_Control(pDX, IDC_CMB_CMTYPE, m_wndCmbCmType);
	DDX_Control(pDX, IDC_CMB_INTERFACE, m_wndCmbCmInterface);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_STATIC_CMPRO, m_wndGridParent);
	DDX_Control(pDX, IDC_CMB_SCCJ, m_wndCmbSccj);
	DDX_Control(pDX, IDC_STATIC_SCCJ, m_wndWxcjStatic);
	DDX_Control(pDX, IDC_CMB_TEXTMSG, m_wndCmbTextMsg);
	DDX_Control(pDX, IDC_STATIC_TEXTMSG, m_wndTextMsgStatic);
}


BEGIN_MESSAGE_MAP(CCommThreadProDlg, CDialog)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_CMB_INTERFACE, &CCommThreadProDlg::OnCbnSelchangeCmbInterface)
	ON_NOTIFY(GVN_ENDLABELEDIT,IDC_THREAD_GRID,&CCommThreadProDlg::OnGridEndEdit)
	ON_CBN_SELCHANGE(IDC_CMB_SCCJ, &CCommThreadProDlg::OnCbnSelchangeCmbSccj)
	ON_CBN_SELCHANGE(IDC_CMB_CMTYPE, &CCommThreadProDlg::OnCbnSelchangeCmbCmtype)
END_MESSAGE_MAP()


// CCommThreadProDlg ��Ϣ�������

HBRUSH CCommThreadProDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if ((pWnd->GetDlgCtrlID() == IDC_EDIT_THREADINFO) /*&& (nCtlColor == CTLCOLOR_EDIT)*/)
	{
		COLORREF clr = RGB(255,0,0);
		pDC->SetTextColor(clr);						 //�����ı�����ʵ��ɫ
		clr = RGB(0,0,0);
		pDC->SetBkColor(clr);						 //���ñ���ɫ
		m_brMine = ::CreateSolidBrush(clr);
		return m_brMine;							 //��ΪԼ�������ر���ɫ��Ӧ��ˢ�Ӿ��
	}
	else
	{
		HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		return hbr;
	}
	return hbr;
}

BOOL CCommThreadProDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CWnd* pWnd = GetDlgItem(IDC_STATIC_CMPRO);
	CRect rect1, rect2;
	int captionHeight = ::GetSystemMetrics(SM_CYCAPTION);
	int cxframe = GetSystemMetrics(SM_CXFRAME);
	int cyframe = GetSystemMetrics(SM_CYFRAME);
	this->GetWindowRect(&rect2);
	if (pWnd)
		pWnd->GetWindowRect(&rect1);
	if (!m_wndCmOptGridCtrl.Create(CRect(rect1.left - rect2.left - cxframe, 
		rect1.top-rect2.top - cyframe-captionHeight, 
		rect1.left + rect1.Width() - rect2.left, 
		rect1.top + rect1.Height() - rect2.top - captionHeight), 
		this, IDC_THREAD_GRID,WS_CHILD|WS_VISIBLE))
	{
		TRACE("Failed to create threads  grid!\n");
		return FALSE;
	}
	

	m_wndCmOptGridCtrl.SetBkColor(RGB(255,255,255));
	m_wndCmOptGridCtrl.SetListMode(TRUE);
	m_wndCmOptGridCtrl.SetSingleColSelection(TRUE);
	m_wndCmOptGridCtrl.SetSingleRowSelection(TRUE);

	if ( m_wndCmbCmType.GetSafeHwnd())
	{
		m_wndCmbCmType.ResetContent();
		int idx =  0;

		/*
		idx = m_wndCmbCmType.AddString("GPRSͨ��ͨ��");
		m_wndCmbCmType.SetItemData(idx,1);

		idx = m_wndCmbCmType.AddString("GSM�ֻ�����ͨ��ͨ��");
		m_wndCmbCmType.SetItemData(idx,2);

		idx = m_wndCmbCmType.AddString("����ͨ��ͨ��");
		m_wndCmbCmType.SetItemData(idx,3);

		idx = m_wndCmbCmType.AddString("���̲�ͨ��ͨ��");
		m_wndCmbCmType.SetItemData(idx,4);

		idx = m_wndCmbCmType.AddString("΢��ͨ��ͨ��");
		m_wndCmbCmType.SetItemData(idx,5);

		idx = m_wndCmbCmType.AddString("����");
		m_wndCmbCmType.SetItemData(idx,6);
		*/

		idx = m_wndCmbCmType.AddString("GPRSͨ��");
		m_wndCmbCmType.SetItemData(idx,GRPS_CommChannel);

		if (GETAPP()->m_GlobalOptions.m_nGSMFunctionEnable == 1)
		{
			idx = m_wndCmbCmType.AddString("����ͨ��");
			m_wndCmbCmType.SetItemData(idx,TEXTMSG_CommChannel);
		}


		idx = m_wndCmbCmType.AddString("����ͨ��");
		m_wndCmbCmType.SetItemData(idx,SC_CommChannel);

        /*
		idx = m_wndCmbCmType.AddString("���̲�ͨ��");
		m_wndCmbCmType.SetItemData(idx,USW_CommChannel);


		idx = m_wndCmbCmType.AddString("΢��ͨ��");
		m_wndCmbCmType.SetItemData(idx,MW_CommChannel);
		*/
		

		idx = m_wndCmbCmType.AddString("����");
		m_wndCmbCmType.SetItemData(idx,Other_CommChannel);


		if (m_wndCmbCmType.GetCount())
		{
			if (m_pCurCommThreadInfo)
			{
				for (int i=0; i< m_wndCmbCmType.GetCount(); i++)
				{
					if (m_wndCmbCmType.GetItemData(i) == m_pCurCommThreadInfo->GetCommType())
					{
						m_wndCmbCmType.SetCurSel(i);
						break;
					}
				}
			}
			else 
				m_wndCmbCmType.SetCurSel(0);
		}
	}

	if (m_wndCmbSccj.GetSafeHwnd())
	{
		m_wndCmbSccj.ResetContent();
		int Idx = 0;
		Idx = m_wndCmbSccj.AddString("���ڵ���(V4.0Э��)");
		m_wndCmbSccj.SetItemData(Idx,SCFactoryType_GTDZ_4_0);
		Idx = m_wndCmbSccj.AddString("���ڵ���(V2.5Э��)");
		m_wndCmbSccj.SetItemData(Idx,SCFactoryType_GTDZ_2_5);
		if (m_wndCmbSccj.GetCount()>0)
		{
			if (m_pCurCommThreadInfo  != NULL && m_pCurCommThreadInfo->GetScCommInfo() != NULL)
			{
				for (int i= 0 ; i < m_wndCmbSccj.GetCount(); i ++)
				{
					if (m_wndCmbSccj.GetItemData(i) == m_pCurCommThreadInfo->GetScCommInfo()->nFactoryType)
					{
						m_wndCmbSccj.SetCurSel(i);
						break;
					}
				}
			}
			else 
				m_wndCmbSccj.SetCurSel(0);
		}
	}

	if (m_wndCmbTextMsg.GetSafeHwnd())
	{
		m_wndCmbTextMsg.ResetContent();
		int Idx = 0;
		Idx = m_wndCmbTextMsg.AddString("���ͨ����");
		m_wndCmbTextMsg.SetItemData(Idx,SCTextMsgType_GHT);

		Idx = m_wndCmbTextMsg.AddString("���ƶ���");
		m_wndCmbTextMsg.SetItemData(Idx, SCTextMsgType_YT);
		if (m_wndCmbTextMsg.GetCount()>0)
			m_wndCmbTextMsg.SetCurSel(0);
	}

	if (m_wndCmbCmInterface.GetSafeHwnd())
	{
		m_wndCmbCmInterface.ResetContent();

		int idx = 0;

		idx = m_wndCmbCmInterface.AddString("��̫��ͨ�Žӿ�");
		m_wndCmbCmInterface.SetItemData(idx,Ethernet_CommInerface);

		idx = m_wndCmbCmInterface.AddString("����ͨ�Žӿ�");
		m_wndCmbCmInterface.SetItemData(idx,Serial_CommInterface);

		if (m_wndCmbCmInterface.GetCount())
		{
			if (m_pCurCommThreadInfo)
			{
				for ( int i = 0; i < m_wndCmbCmInterface.GetCount(); i++)
				{
					if (m_wndCmbCmInterface.GetItemData(i) == m_pCurCommThreadInfo->GetCommInterfaceType())
					{
						m_wndCmbCmInterface.SetCurSel(i);
						break;
					}
				}
			}
			else
				m_wndCmbCmInterface.SetCurSel(0);
		}		
	}

    UpdateThreadInfo();
    OnCbnSelchangeCmbCmtype();
	OnCbnSelchangeCmbInterface();

	

	return TRUE; 
}

void CCommThreadProDlg::UpdateThreadInfo()
{
	m_strThreadInfos = "";
	if (m_pCurCommThreadInfo)
	{
		//��������
		m_bCom_Type				= m_pCurCommThreadInfo->GetCommType();
		m_bCom_Interface		= m_pCurCommThreadInfo->GetCommInterfaceType();
		m_bProtocolType			= m_pCurCommThreadInfo->GetProtocolType();
		m_Com_Info.mComName		= m_pCurCommThreadInfo->GetComInfo()->mComName;
		m_Com_Info.mBaund		= m_pCurCommThreadInfo->GetComInfo()->mBaund;
		m_Com_Info.mDataBits	= m_pCurCommThreadInfo->GetComInfo()->mDataBits;
		m_Com_Info.mParity		= m_pCurCommThreadInfo->GetComInfo()->mParity;
		m_Com_Info.mStopBits	= m_pCurCommThreadInfo->GetComInfo()->mStopBits;
		m_Net_Info.mIpAddress	= m_pCurCommThreadInfo->GetNetInfo()->mIpAddress;
		m_Net_Info.mPort		= m_pCurCommThreadInfo->GetNetInfo()->mPort;
		m_Sc_Comm_Info.nScAddress = m_pCurCommThreadInfo->GetScCommInfo()->nScAddress;
		m_Sc_Comm_Info.nFactoryType  = m_pCurCommThreadInfo->GetScCommInfo()->nFactoryType;


		SetWindowText("Commserver �����߳�[������Ϣ]");
		m_strThreadInfos = "ͨ�ŷ����̵߳���ϸ��Ϣ:\r\n\r\n";
		if (m_pCurCommThreadInfo->GetThreadState() == THREAD_STATE_RUNNING)
			m_strThreadInfos += "��ǰ״̬:������\r\n";
		else if (m_pCurCommThreadInfo->GetThreadState() == THREAD_STATE_PAUSE)
			m_strThreadInfos += "��ǰ״̬:����\r\n";
		else if (m_pCurCommThreadInfo->GetThreadState() == THREAD_STATE_STOP)
			m_strThreadInfos += "��ǰ״̬:δ����/ֹͣ\r\n";

		if(m_pCurCommThreadInfo->GetThreadID() != 0 &&
		   m_pCurCommThreadInfo->GetThreadHandle() != NULL)
		{
			CString _sTemp = "";
			_sTemp.Format("�߳�  ID:%d\r\n",m_pCurCommThreadInfo->GetThreadID());
			m_strThreadInfos += _sTemp;

			FILETIME Ftime1,Ftime2,Ftime3,Ftime4;
			::GetThreadTimes(m_pCurCommThreadInfo->GetThreadHandle(),&Ftime1,&Ftime2,&Ftime3,&Ftime4);
			SYSTEMTIME st;


			FILETIME FLocaltime;
			::FileTimeToLocalFileTime(&Ftime1,&FLocaltime);
			::FileTimeToSystemTime(&FLocaltime,&st);
			_sTemp.Format("����ʱ��:%04d-%02d-%02d %02d:%02d:%02d.%03d\r\n",
				st.wYear,
				st.wMonth,
				st.wDay,
				st.wHour,
				st.wMinute,
				st.wSecond,
				st.wMilliseconds
				);
			m_strThreadInfos += _sTemp;
			
			::FileTimeToSystemTime(&Ftime3,&st);
			_sTemp.Format("�ں�ʱ��:%02d:%02d:%02d.%03d\r\n",
				st.wHour,
				st.wMinute,
				st.wSecond,
				st.wMilliseconds
				);
			m_strThreadInfos += _sTemp;

			::FileTimeToSystemTime(&Ftime4,&st);
			_sTemp.Format("�û�ʱ��:%02d:%02d:%02d.%03d\r\n",
				st.wHour,
				st.wMinute,
				st.wSecond,
				st.wMilliseconds
				);
			m_strThreadInfos += _sTemp;
		}
	}
	else
	{
		SetWindowText("Commserver �����߳�[����]");
	}
	UpdateData(FALSE);
}

void CCommThreadProDlg::SetCommThreadInfo(CCommThreadInfo* pThreadInfo)
{
	m_pCurCommThreadInfo = pThreadInfo;

	if (m_pCurCommThreadInfo)
	{
		m_bCom_Type				= m_pCurCommThreadInfo->GetCommType();
		m_bCom_Interface		= m_pCurCommThreadInfo->GetCommInterfaceType();
		m_bProtocolType			= m_pCurCommThreadInfo->GetProtocolType();
		m_Com_Info.mBaund		= m_pCurCommThreadInfo->GetComInfo()->mBaund;
		m_Com_Info.mComName		= m_pCurCommThreadInfo->GetComInfo()->mComName;
		m_Com_Info.mDataBits	= m_pCurCommThreadInfo->GetComInfo()->mDataBits;
		m_Com_Info.mParity		= m_pCurCommThreadInfo->GetComInfo()->mParity;
		m_Com_Info.mStopBits	= m_pCurCommThreadInfo->GetComInfo()->mStopBits;
		m_Net_Info.mIpAddress	= m_pCurCommThreadInfo->GetNetInfo()->mIpAddress;
		m_Net_Info.mPort		= m_pCurCommThreadInfo->GetNetInfo()->mPort;
		m_Sc_Comm_Info.nScAddress = m_pCurCommThreadInfo->GetScCommInfo()->nScAddress;
		m_Sc_Comm_Info.nFactoryType  = m_pCurCommThreadInfo->GetScCommInfo()->nFactoryType;
	}
}


void CCommThreadProDlg::OnCbnSelchangeCmbInterface()
{
	BYTE CtypeItemData  = 0;
	if (m_wndCmbCmType.GetSafeHwnd())
	{
		int nSel = m_wndCmbCmType.GetCurSel();
		if (nSel == CB_ERR)
			return;

		CtypeItemData = (BYTE)m_wndCmbCmType.GetItemData(nSel);
	}

	if (m_wndCmbCmInterface.GetSafeHwnd())
	{
		int nSel = m_wndCmbCmInterface.GetCurSel();
		if (nSel != CB_ERR)
		{
		  int data=(int)m_wndCmbCmInterface.GetItemData(nSel);

		  if (data == Ethernet_CommInerface)
			  InitGridCtrl_NetInfo();
		  else if (data == Serial_CommInterface)
			  InitGridCtrl_ComInfo(CtypeItemData);
		  else
		  {
			  m_wndCmOptGridCtrl.DeleteAllItems();
			  m_wndCmOptGridCtrl.Invalidate();
		  }
		}
	}
}

void CCommThreadProDlg::InitGridCtrl_ComInfo(BYTE CmType /* = 0 */)
{
	if (m_wndCmOptGridCtrl.GetSafeHwnd())
	{
		m_wndCmOptGridCtrl.DeleteAllItems();
		if(CmType == SC_CommChannel)
			m_wndCmOptGridCtrl.SetRowCount(7);
		else
			m_wndCmOptGridCtrl.SetRowCount(6);

		m_wndCmOptGridCtrl.SetFixedRowCount(1);

		CString strHearders[] = {"���ں�",
								 "������",
								 "����λ",
								 "ֹͣλ",
								 "У��",
		                         "ͨ�ŵ�ַ(����)"};
		 
		CString strColHeaders[] ={"������","ֵ"};

		m_wndCmOptGridCtrl.InsertColumn(strColHeaders[0],DT_LEFT|DT_VCENTER|DT_SINGLELINE,0);
		m_wndCmOptGridCtrl.SetColumnWidth(0,110);
		m_wndCmOptGridCtrl.InsertColumn(strColHeaders[1],DT_LEFT|DT_VCENTER|DT_SINGLELINE,1);
		m_wndCmOptGridCtrl.SetColumnWidth(1,190);
		

		for (int Idx = 1; Idx < m_wndCmOptGridCtrl.GetRowCount(); Idx ++)
		{
			m_wndCmOptGridCtrl.SetItemText(Idx,0,strHearders[Idx-1]);
			OnCellReadOnly(Idx,0);
			OnCellNormal(Idx,1);
			m_wndCmOptGridCtrl.SetRowHeight(Idx,MFC_GRIDCTRL_HEIGHT);
		}

		InitComGridItem11();
		InitComGridItem21();
		InitComGridItem31();
		InitComGridItem41();
		InitComGridItem51();

		if (CmType == SC_CommChannel)
			InitComGridItem61();
	}
}

void  CCommThreadProDlg::InitComGridItem11()
{
	OnCellNormal(1,1);
	if (!m_wndCmOptGridCtrl.SetCellType(1,1, RUNTIME_CLASS(CGridCellCombo)))
		return;

	CStringArray options;
	options.Add(_T("COM1"));
	options.Add(_T("COM2"));
	options.Add(_T("COM3"));

	CGridCellCombo *pCell = (CGridCellCombo*) m_wndCmOptGridCtrl.GetCell(1,1);
	pCell->SetOptions(options);
	pCell->SetStyle(CBS_DROPDOWN); //CBS_DROPDOWN, CBS_DROPDOWNLIST, CBS_SIMPLE

	CString strCom = "COM1";
	strCom = m_Com_Info.mComName;

	m_wndCmOptGridCtrl.SetItemText(1,1, strCom);
}

void  CCommThreadProDlg::InitComGridItem21()
{
	OnCellNormal(2,1);
	if (!m_wndCmOptGridCtrl.SetCellType(2,1, RUNTIME_CLASS(CGridCellCombo)))
		return;
	CStringArray options;

	options.Add("110");
	options.Add("300");
	options.Add("600");
	options.Add("1200");
	options.Add("2400");
	options.Add("4800");
	options.Add("9600");
	options.Add("14400");
	options.Add("19200");
	options.Add("38400");
	options.Add("56000");
	options.Add("57600");
	options.Add("115200");
	options.Add("128000");
	options.Add("256000");	

	CGridCellCombo *pCell = (CGridCellCombo*) m_wndCmOptGridCtrl.GetCell(2,1);
	pCell->SetOptions(options);
	pCell->SetStyle(CBS_DROPDOWN); //CBS_DROPDOWN, CBS_DROPDOWNLIST, CBS_SIMPLE

	CString _strBound = "9600";
	_strBound.Format("%d",m_Com_Info.mBaund);

	m_wndCmOptGridCtrl.SetItemText(2,1, _strBound);
}

void  CCommThreadProDlg::InitComGridItem31()
{
	OnCellNormal(3,1);
	if (!m_wndCmOptGridCtrl.SetCellType(3,1, RUNTIME_CLASS(CGridCellCombo)))
		return;
	
	CStringArray options;
	options.Add(_T("8λ"));
	options.Add(_T("7λ"));

	CGridCellCombo *pCell = (CGridCellCombo*) m_wndCmOptGridCtrl.GetCell(3,1);
	pCell->SetOptions(options);
	pCell->SetStyle(CBS_DROPDOWNLIST); //CBS_DROPDOWN, CBS_DROPDOWNLIST, CBS_SIMPLE

	CString _strDataBits= "8λ";
	BYTE databits = m_Com_Info.mDataBits;
	if (databits == 8)
		_strDataBits= "8λ";
	else if (databits == 7)
		_strDataBits= "7λ";
	m_wndCmOptGridCtrl.SetItemText(3,1, _strDataBits);
}

void  CCommThreadProDlg::InitComGridItem41()
{
	OnCellNormal(4,1);
	if (!m_wndCmOptGridCtrl.SetCellType(4,1, RUNTIME_CLASS(CGridCellCombo)))
		return;
	
	CStringArray options;
	options.Add(_T("1λֹͣλ"));
	options.Add(_T("1.5λֹͣλ"));
	options.Add(_T("2λֹͣλ"));

	CGridCellCombo *pCell = (CGridCellCombo*) m_wndCmOptGridCtrl.GetCell(4,1);
	pCell->SetOptions(options);
	pCell->SetStyle(CBS_DROPDOWNLIST); //CBS_DROPDOWN, CBS_DROPDOWNLIST, CBS_SIMPLE


	BYTE nStopBits = m_Com_Info.mStopBits;
	CString _sParity = "1λֹͣλ";

	if (nStopBits == ONESTOPBIT)
		_sParity = "1λֹͣλ";
	else if (ONE5STOPBITS == nStopBits)
		_sParity = "1.5λֹͣλ";
	else if (TWOSTOPBITS == nStopBits)
		_sParity = "2λֹͣλ";

	m_wndCmOptGridCtrl.SetItemText(4,1,_sParity);

}

void  CCommThreadProDlg::InitComGridItem51()
{
	OnCellNormal(5,1);
	if (!m_wndCmOptGridCtrl.SetCellType(5,1, RUNTIME_CLASS(CGridCellCombo)))
		return;
	
	CStringArray options;
	options.Add(_T("��У��"));
	options.Add(_T("��У��"));
	options.Add(_T("żУ��"));

	CGridCellCombo *pCell = (CGridCellCombo*) m_wndCmOptGridCtrl.GetCell(5,1);
	pCell->SetOptions(options);
	pCell->SetStyle(CBS_DROPDOWNLIST); //CBS_DROPDOWN, CBS_DROPDOWNLIST, CBS_SIMPLE

	BYTE nParity = m_Com_Info.mParity;
	CString _sParity = "��У��";

	if (nParity == NOPARITY)
		_sParity = "��У��";
	else if (ODDPARITY == nParity)
		_sParity = "��У��";
	else if (EVENPARITY == nParity)
		_sParity = "żУ��";

	m_wndCmOptGridCtrl.SetItemText(5,1,_sParity);
}

void CCommThreadProDlg::InitComGridItem61()
{
	OnCellNormal(6,1);
	if (!m_wndCmOptGridCtrl.SetCellType(6,1, RUNTIME_CLASS(CGridCellNumeric)))
		return;

	CString _sTemp = "0";
	_sTemp.Format("%d",m_Sc_Comm_Info.nScAddress);
	m_wndCmOptGridCtrl.SetItemText(6,1, _sTemp);
}

void CCommThreadProDlg::InitGridCtrl_NetInfo()
{
	if (m_wndCmOptGridCtrl.GetSafeHwnd())
	{
		m_wndCmOptGridCtrl.DeleteAllItems();
		m_wndCmOptGridCtrl.SetRowCount(4);
		m_wndCmOptGridCtrl.SetFixedRowCount(1);

		CString strHearders[]  = {"Э������","IP��ַ","�˿ں�"};
		CString strColHeaders[] ={"������", "ֵ"};

		m_wndCmOptGridCtrl.InsertColumn(strColHeaders[0],DT_LEFT|DT_VCENTER|DT_SINGLELINE,0);
		m_wndCmOptGridCtrl.SetColumnWidth(0,90);
		m_wndCmOptGridCtrl.InsertColumn(strColHeaders[1],DT_LEFT|DT_VCENTER|DT_SINGLELINE,1);
		m_wndCmOptGridCtrl.SetColumnWidth(1,210);

		for (int Idx = 1; Idx < 4; Idx ++)
		{
			m_wndCmOptGridCtrl.SetItemText(Idx,0,strHearders[Idx-1]);
			OnCellReadOnly(Idx,0);
			OnCellNormal(Idx,1);
			m_wndCmOptGridCtrl.SetRowHeight(Idx,MFC_GRIDCTRL_HEIGHT);
		}


		OnCellNormal(1,1);
		if (!m_wndCmOptGridCtrl.SetCellType(1,1,RUNTIME_CLASS(CGridCellCombo)))
			return;

		CStringArray protocolArrays;
		protocolArrays.Add(_T("UDP"));
		protocolArrays.Add(_T("TCP"));

		
		CGridCellCombo *pProtocolCell = (CGridCellCombo*) m_wndCmOptGridCtrl.GetCell(1,1);
		pProtocolCell->SetOptions(protocolArrays);
		pProtocolCell->SetStyle(CBS_DROPDOWNLIST); //CBS_DROPDOWN, CBS_DROPDOWNLIST, CBS_SIMPLE


		if (m_bProtocolType == UDP_Protocol)
			m_wndCmOptGridCtrl.SetItemText(1,1,"UDP");
		else if (m_bProtocolType == TCP_Protocol)
			m_wndCmOptGridCtrl.SetItemText(1,1,"TCP");
		else
			m_wndCmOptGridCtrl.SetItemText(1,1,"UDP");



		OnCellNormal(2,1);
		if (!m_wndCmOptGridCtrl.SetCellType(2,1, RUNTIME_CLASS(CGridCellCombo)))
			return;
		CStringArray options;
		options.Add(_T("0.0.0.0"));


		//��ȡ����IP��ַ
		char localname[100]={'\0'};
		if(gethostname(localname,sizeof(localname)) != SOCKET_ERROR)
		{
			HOSTENT* pHostent = NULL;
			pHostent=gethostbyname((char*)&localname);
			LPSTR *pptr = pHostent->h_addr_list;
			while(*pptr!=NULL)
			{
				unsigned long ip_addr;
				memcpy(&ip_addr,*pptr,pHostent->h_length);
				CString strTemp = "";

				in_addr addr;
				addr.s_addr = ip_addr;
				strTemp.Format("%s",inet_ntoa(addr));
				options.Add(strTemp);

				pptr++;
			}
		}

		CGridCellCombo *pCell = (CGridCellCombo*) m_wndCmOptGridCtrl.GetCell(2,1);
		pCell->SetOptions(options);
		pCell->SetStyle(CBS_DROPDOWN); //CBS_DROPDOWN, CBS_DROPDOWNLIST, CBS_SIMPLE
		
		m_wndCmOptGridCtrl.SetItemText(2,1, m_Net_Info.mIpAddress);



		OnCellNormal(3,1);
		if (!m_wndCmOptGridCtrl.SetCellType(3,1, RUNTIME_CLASS(CGridCellNumeric)))
			return;
        
		CString _sTemp = "0";
		_sTemp.Format("%d",m_Net_Info.mPort);
		m_wndCmOptGridCtrl.SetItemText(3,1, _sTemp);
	}
}



void CCommThreadProDlg::OnCellNormal(int Row,int Col)
{
	if (m_wndCmOptGridCtrl.GetSafeHwnd())
	{
		m_wndCmOptGridCtrl.SetCellType(Row,Col, RUNTIME_CLASS(CGridCell));
		m_wndCmOptGridCtrl.SetItemText(Row,Col, _T(""));
		m_wndCmOptGridCtrl.SetItemState(Row,Col, m_wndCmOptGridCtrl.GetItemState(Row,Col) & ~GVIS_READONLY);
		m_wndCmOptGridCtrl.Invalidate();
	}
}

void CCommThreadProDlg::OnCellReadOnly(int Row,int Col)
{
	if (m_wndCmOptGridCtrl.GetSafeHwnd())
	{
		m_wndCmOptGridCtrl.SetItemState(Row,Col, m_wndCmOptGridCtrl.GetItemState(Row,Col) | GVIS_READONLY);
	}
}

void CCommThreadProDlg::OnOK()
{
	UpdateData(TRUE);

	int nSel_ComType = m_wndCmbCmType.GetCurSel();
	if (nSel_ComType == CB_ERR)
	{
		MessageBox("��ѡ��ͨ�ŷ�ʽ!","����",MB_OK|MB_ICONERROR);
		return ;
	}

	int nSel_Interface = m_wndCmbCmInterface.GetCurSel();
	if (nSel_Interface == CB_ERR)
	{
		MessageBox("��ѡ��ͨ�Žӿڷ�ʽ!","����",MB_OK|MB_ICONERROR);
		return;
	}

	int nSel_ScFactoryType = m_wndCmbSccj.GetCurSel();
	if (nSel_ScFactoryType == CB_ERR)
	{
		MessageBox("��ѡ�����ǳ���!","����",MB_OK|MB_ICONERROR);
		return;
	}

	int nSel_TmFactoryType = m_wndCmbTextMsg.GetCurSel();
	if (nSel_TmFactoryType == CB_ERR)
	{
		MessageBox("��ѡ����ų���!","����", MB_OK|MB_ICONERROR);
		return;
	}


	int nGirdRowCount = 0;
	INT_PTR nItemData = m_wndCmbCmInterface.GetItemData(nSel_Interface);


	m_bCom_Interface = (BYTE)m_wndCmbCmInterface.GetItemData(nSel_Interface);
	m_bCom_Type		 = (BYTE)m_wndCmbCmType.GetItemData(nSel_ComType);
	m_Sc_Comm_Info.nFactoryType = (int)m_wndCmbSccj.GetItemData(nSel_ScFactoryType);
	m_Tm_Comm_Info.nFactoryType = (int)m_wndCmbTextMsg.GetItemData(nSel_TmFactoryType);

	if (nItemData == 1)          //Net
	{
		nGirdRowCount  = 3;
	}
	else if (nItemData == 2)     //Com
	{
		if (m_bCom_Type == SC_CommChannel
			|| m_bCom_Type == TEXTMSG_CommChannel)
			nGirdRowCount = 7;
		else
			nGirdRowCount = 6;
	}

	int nRowIndex = 0;
	for (nRowIndex =1; nRowIndex <nGirdRowCount; nRowIndex ++)
	{
		CGridCellBase* pCell = m_wndCmOptGridCtrl.GetCell(nRowIndex,1);
		if (pCell)
		{
			pCell->EndEdit();
			TRACE("Cell(%d,1):%s\r\n",nRowIndex,pCell->GetText());
		}
	}

	//��ȡ��ص���������
   //UpdateAllDate(nSel_ComType,nSel_Interface);


   //�������������Ϣ���м��
   if (nItemData == 1)        //Net
   {
	   if (!CUdfGlobalFUNC::IpAddressCheck(m_Net_Info.mIpAddress))
	   {
		   MessageBox("IP��ַ����ȷ��Ϊ��,����������","����",MB_OK|MB_ICONERROR);
		   return;
	   }

	   if (m_Net_Info.mPort <= 0 ||m_Net_Info.mPort >65535)
	   {
		   MessageBox("�˿ںŲ���ȷ,��ȷֵΪ(1~65535),����������","����",MB_OK|MB_ICONERROR);
		   return;
	   }
   }
   else if (nItemData == 2)  //Com
   {
	   if (m_Com_Info.mComName.IsEmpty())
	   {
		   MessageBox("���ںŲ���Ϊ��,���������룡","����",MB_OK|MB_ICONERROR);
		   return;
	   }
	   if (m_Com_Info.mBaund <= 0 )
	   {
		   MessageBox("�������������,����������","����",MB_OK|MB_ICONERROR);
		   return;
	   }
   }   

	CDialog::OnOK();
}


//void CCommThreadProDlg::UpdateAllDate(int bCmType,int bCmInterface)
//{
//	m_bCom_Interface = (BYTE)m_wndCmbCmInterface.GetItemData(bCmInterface);
//	m_bCom_Type		 = (BYTE)m_wndCmbCmType.GetItemData(bCmType);
//
//	if (m_bCom_Interface == 1)				//net
//	{
//		m_Net_Info.mIpAddress = GetCellText(1,1);
//		m_Net_Info.mPort	  = (WORD)atoi(GetCellText(2,1));	
//	}
//	else if (m_bCom_Interface == 2)			//Com
//	{
//		CString _sTemp = "";
//		m_Com_Info.mComName = GetCellText(1,1);
//		m_Com_Info.mBaund   = atoi(GetCellText(2,1));
//
//		_sTemp = GetCellText(3,1);
//		if (_sTemp.Find("8λ") != -1)
//			m_Com_Info.mDataBits  = 8;
//		else if (_sTemp.Find("7λ") != -1)
//			m_Com_Info.mDataBits = 7;
//		else 
//			m_Com_Info.mDataBits = 8;
//
//		_sTemp = GetCellText(4,1);
//		if (_sTemp.Find("1λֹͣλ") != -1)
//			m_Com_Info.mStopBits = ONESTOPBIT;
//		else if (_sTemp.Find("1.5λֹͣλ")!= -1)
//			m_Com_Info.mStopBits = ONE5STOPBITS;
//		else if (_sTemp.Find("2λֹͣλ")!= -1)
//			m_Com_Info.mStopBits = TWOSTOPBITS;
//		else
//			m_Com_Info.mStopBits = ONESTOPBIT;
//
//		_sTemp = GetCellText(5,1);
//		if (_sTemp.Find("��У��") != -1)
//			m_Com_Info.mParity = NOPARITY;
//		else if (_sTemp.Find("żУ��") != -1)
//			m_Com_Info.mParity = EVENPARITY;
//		else if (_sTemp.Find("��У��") != -1)
//			m_Com_Info.mParity = ODDPARITY;
//		else
//			m_Com_Info.mParity = NOPARITY;
//	}
//}

const CString CCommThreadProDlg::GetCellText(int Row,int Col)
{
	CString _sTemp = "";
	if (m_wndCmOptGridCtrl.GetSafeHwnd())
	{
		CGridCellBase* pCell = m_wndCmOptGridCtrl.GetCell(Row,Col);
		if (pCell)
		{
			_sTemp = pCell->GetText();
			_sTemp.Trim();
			return _sTemp;
		}
	}
	return _sTemp;
}


void CCommThreadProDlg::OnGridEndEdit(NMHDR *pNotifyStruct, 
									 LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	// AcceptChange is a fictional routine that should return TRUE
	// if you want to accept the new value for the cell.

	if (pItem == NULL)				return;
	if (pItem->iColumn != 1)		return;
	if (pItem->iRow <=0  )			return;

	int nSel_CmType = m_wndCmbCmType.GetCurSel();
	if (nSel_CmType == CB_ERR)     return;
	BYTE nCmTypeData = (BYTE)m_wndCmbCmType.GetItemData(nSel_CmType);

	int nSel_Interface = m_wndCmbCmInterface.GetCurSel();
	if (nSel_Interface == CB_ERR)		return;


	//int nGirdRowCount = 0;
	INT_PTR nInterfaceData = m_wndCmbCmInterface.GetItemData(nSel_Interface);
	if (nInterfaceData == 1)          //Net
	{
		//nGirdRowCount  = 4;

		if (pItem->iRow == 1)
		{
			CString Tmp = GetCellText(1,1);
			if(Tmp == "UDP")
				m_bProtocolType = UDP_Protocol;
			else if (Tmp == "TCP")
				m_bProtocolType = TCP_Protocol;
		}
		if (pItem->iRow == 2)	
			m_Net_Info.mIpAddress = GetCellText(2,1);

		if (pItem->iRow == 3)   
		{
			int nPort = atoi(GetCellText(3,1));
			if (nPort > 65535 ||
				nPort < 0)
			{
				MessageBox("����Ķ˿ںų�����Χ(1~65535),����������!","��ʾ",MB_OK|MB_ICONERROR);
				m_wndCmOptGridCtrl.SetItemText(3,1,"0");
				m_Net_Info.mPort = 0;
			}
			else
				m_Net_Info.mPort	  = (WORD)nPort;	
		}

	}
	else if (nInterfaceData == 2)     //Com
	{
		//nGirdRowCount = 6;

		CString _sTemp = "";
		if (pItem->iRow == 1)	
			m_Com_Info.mComName = GetCellText(1,1);

		if (pItem->iRow == 2)	
		{
			m_Com_Info.mBaund   = atoi(GetCellText(2,1));
			_sTemp.Format("%d",m_Com_Info.mBaund);
			m_wndCmOptGridCtrl.SetItemText(2,1,_sTemp);
		}

		if (pItem->iRow == 3)
		{
			_sTemp = GetCellText(3,1);
			if (_sTemp.Find("8λ") != -1)
				m_Com_Info.mDataBits  = 8;
			else if (_sTemp.Find("7λ") != -1)
				m_Com_Info.mDataBits = 7;
			else 
				m_Com_Info.mDataBits = 8;
		}

		if (pItem->iRow == 4)
		{
			_sTemp = GetCellText(4,1);
			if (_sTemp.Find("1λֹͣλ") != -1)
				m_Com_Info.mStopBits = ONESTOPBIT;
			else if (_sTemp.Find("1.5λֹͣλ")!= -1)
				m_Com_Info.mStopBits = ONE5STOPBITS;
			else if (_sTemp.Find("2λֹͣλ")!= -1)
				m_Com_Info.mStopBits = TWOSTOPBITS;
			else
				m_Com_Info.mStopBits = ONESTOPBIT;
		}

		if (pItem->iRow == 5)
		{
			_sTemp = GetCellText(5,1);
			if (_sTemp.Find("��У��") != -1)
				m_Com_Info.mParity = NOPARITY;
			else if (_sTemp.Find("żУ��") != -1)
				m_Com_Info.mParity = EVENPARITY;
			else if (_sTemp.Find("��У��") != -1)
				m_Com_Info.mParity = ODDPARITY;
			else
				m_Com_Info.mParity = NOPARITY;
		}

		if (pItem->iRow == 6)
		{
			_sTemp = GetCellText(6,1);
			m_Sc_Comm_Info.nScAddress = (UINT)atol(_sTemp);

			_sTemp.Format("%d",m_Sc_Comm_Info.nScAddress);
			m_wndCmOptGridCtrl.SetItemText(6,1,_sTemp);
		}

	}
}

void CCommThreadProDlg::OnCbnSelchangeCmbSccj()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CCommThreadProDlg::OnCbnSelchangeCmbCmtype()
{
	if (m_wndCmbCmType.GetSafeHwnd())
	{
		int nSel = m_wndCmbCmType.GetCurSel();
		if (nSel != CB_ERR)
		{
			BYTE itemData = (BYTE)m_wndCmbCmType.GetItemData(nSel);

			int CmInterface_SC = 0,CmInterface_GPRS=0;
			if (m_wndCmbCmInterface.GetSafeHwnd())
			{
				
				for(int i =0; i < m_wndCmbCmInterface.GetCount(); i ++)
				{
					if ((BYTE)m_wndCmbCmInterface.GetItemData(i) == Serial_CommInterface )
						CmInterface_SC = i;
					if ((BYTE)m_wndCmbCmInterface.GetItemData(i) == Ethernet_CommInerface )
						CmInterface_GPRS = i;					
				}
			}

			if (itemData == SC_CommChannel)
			{
				m_wndWxcjStatic.ShowWindow(SW_SHOW);
				m_wndCmbSccj.ShowWindow(SW_SHOW);

				m_wndTextMsgStatic.ShowWindow(SW_HIDE);
				m_wndCmbTextMsg.ShowWindow(SW_HIDE);

				m_wndCmbCmInterface.SetCurSel(CmInterface_SC);
				m_wndCmbCmInterface.EnableWindow(FALSE);
			}
			else if (itemData == TEXTMSG_CommChannel)
			{
				m_wndTextMsgStatic.ShowWindow(SW_SHOW);
				m_wndCmbTextMsg.ShowWindow(SW_SHOW);

				m_wndWxcjStatic.ShowWindow(SW_HIDE);
				m_wndCmbSccj.ShowWindow(SW_HIDE);

				m_wndCmbCmInterface.SetCurSel(CmInterface_SC);
				m_wndCmbCmInterface.EnableWindow(FALSE);
				
			}
			else
			{
				m_wndWxcjStatic.ShowWindow(SW_HIDE);
				m_wndCmbSccj.ShowWindow(SW_HIDE);

				m_wndTextMsgStatic.ShowWindow(SW_HIDE);
				m_wndCmbTextMsg.ShowWindow(SW_HIDE);
				

				if (itemData == GRPS_CommChannel)
				{
					m_wndCmbCmInterface.SetCurSel(CmInterface_GPRS);
					m_wndCmbCmInterface.EnableWindow(FALSE);					
				}
				else
					m_wndCmbCmInterface.EnableWindow(TRUE);
			}
		}

	   OnCbnSelchangeCmbInterface();
	}
}

