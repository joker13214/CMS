// CommServerMnDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CMS.h"
#include "CommServerMnDlg.h"
#include "WaitTimer.h"
#include "CommThreadProDlg.h"
#include "RtuAttrMsgInit.h"


// CCommServerMnDlg �Ի���
IMPLEMENT_DYNAMIC(CCommServerMnDlg, CDialog)

CCommServerMnDlg::CCommServerMnDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCommServerMnDlg::IDD, pParent)
{
	m_pTextMsgDlg = NULL;

}

CCommServerMnDlg::~CCommServerMnDlg()
{
}

void CCommServerMnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_THDLOG, m_wndThreadLog);
	DDX_Control(pDX, IDC_BUTTON1, m_btnAdd);
	DDX_Control(pDX, IDC_BTN_REFRESH, m_btnRefresh);
	DDX_Control(pDX, IDC_BTN_OK, m_btnOK);
}


BEGIN_MESSAGE_MAP(CCommServerMnDlg, CDialog)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_REFRESH, &CCommServerMnDlg::OnBnClickedBtnRefresh)
	ON_BN_CLICKED(IDC_BTN_OK, &CCommServerMnDlg::OnBnClickedBtnOk)
	ON_NOTIFY(NM_CLICK, IDC_THREAD_GRID,&CCommServerMnDlg::OnLClickThreadGrid)
	ON_BN_CLICKED(IDC_BUTTON1, &CCommServerMnDlg::OnBnClickedButton1)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CCommServerMnDlg ��Ϣ�������

void CCommServerMnDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//CDialog::OnClose();
	ShowWindow(SW_HIDE);
}

BOOL CCommServerMnDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CWnd* pWnd = GetDlgItem(IDC_THREAD_GRIDPIC);
	CRect rect1, rect2;
	int captionHeight = ::GetSystemMetrics(SM_CYCAPTION);
	int cxframe = GetSystemMetrics(SM_CXFRAME);
	int cyframe = GetSystemMetrics(SM_CYFRAME);
	this->GetWindowRect(&rect2);
	if (pWnd)
		pWnd->GetWindowRect(&rect1);
	if (!m_wndThreadsGrid.Create(CRect(rect1.left - rect2.left - cxframe, 
		                           rect1.top-rect2.top - cyframe-captionHeight, 
								   rect1.left + rect1.Width() - rect2.left, 
								   rect1.top + rect1.Height() - rect2.top - captionHeight), 
								   this, IDC_THREAD_GRID,WS_CHILD|WS_VISIBLE))
	{
		TRACE("Failed to create threads  grid!\n");
		return FALSE;
	}


	if (!m_wndImagelist.Create(16,16,ILC_MASK|ILC_COLOR32,3,3))
	{ 
		TRACE("Faild to create threads imagelist!\n");
		return FALSE;
	}

	HICON hicon[3];
	hicon[0] = AfxGetApp()->LoadIcon(IDI_THRD_RUN);
	hicon[1] = AfxGetApp()->LoadIcon(IDI_THRD_PAUSE);
	hicon[2] = AfxGetApp()->LoadIcon(IDI_THRD_STOP);
	for ( int idx =0; idx <3;idx ++)
	{
		m_wndImagelist.Add(hicon[idx]);
	}

	m_wndImagelist.SetBkColor(RGB(255,255,255));
	m_wndThreadsGrid.SetImageList(&m_wndImagelist);

	m_wndThreadsGrid.SetBkColor(RGB(255,255,255));
	m_wndThreadsGrid.SetListMode(TRUE);
	m_wndThreadsGrid.SetEditable(FALSE);
	m_wndThreadsGrid.SetSingleColSelection(TRUE);
	m_wndThreadsGrid.SetSingleRowSelection(TRUE);

	//m_wndThreadsGrid.SetGridLines(GVL_NONE);

	InitListCtrlHeader();
	InitGridHeader();
	ShowAllThreadsInfo();

	SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_THREAD),FALSE);
	SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
	return TRUE;  
}


bool CCommServerMnDlg::InitGridHeader()
{
	if (m_wndThreadsGrid.GetSafeHwnd())
	{
		m_wndThreadsGrid.DeleteAllItems();

		CString strHeaders[]={
			"No#",
			"״̬",
			"������Ϣ",
			"����",
			"ֹͣ",
			"ɾ��",
			"����",
			"��ʼ��"
		};

		int colsWidths[] ={
			40,
			80,
			355,
			45,
			45,
			45,
			45,
			50
		};

		for (int ColIdx = 0; ColIdx<8;ColIdx ++)
		{   
			if (ColIdx <= 2)
				m_wndThreadsGrid.InsertColumn(strHeaders[ColIdx],DT_VCENTER|DT_LEFT|DT_SINGLELINE,ColIdx);
			else
				m_wndThreadsGrid.InsertColumn(strHeaders[ColIdx],DT_VCENTER|DT_CENTER|DT_SINGLELINE,ColIdx);

			m_wndThreadsGrid.SetColumnWidth(ColIdx,colsWidths[ColIdx]);
		}

		m_wndThreadsGrid.SetFixedRowCount(1);
	}
	return true;
}

void CCommServerMnDlg::OnBnClickedBtnRefresh()
{
	InitGridHeader();
	ShowAllThreadsInfo();
}

bool CCommServerMnDlg::ShowAllThreadsInfo()
{
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	if (pApp && pApp->m_pCommServer)
	{
		POSITION rPos = pApp->m_pCommServer->m_lstThreads.GetHeadPosition();
		int nItemCount = (int)pApp->m_pCommServer->m_lstThreads.GetCount();
		if (m_wndThreadsGrid.GetSafeHwnd())
			m_wndThreadsGrid.SetRowCount(nItemCount + 1);

		int Index = 1;
		while (rPos != NULL)
		{
			CCommThreadInfo* pThreadInfo = pApp->m_pCommServer->m_lstThreads.GetNext(rPos);
			if (pThreadInfo)
			{
				ShowOneThreadInfo(pThreadInfo,Index);
				pThreadInfo->SetThreadName(Index);
				Index ++;
			}
		}
	}
	return true;
}

void CCommServerMnDlg::ShowOneThreadInfo(CCommThreadInfo* pThread,int ItemRow)
{
	if (!pThread)		return;
	if (!m_wndThreadsGrid.GetSafeHwnd())	return;

	int ItemIndex = ItemRow;

	m_wndThreadsGrid.SetRowHeight(ItemRow,MFC_GRIDCTRL_HEIGHT);
	CString strTemp ="";
	strTemp.Format("%d",ItemIndex);

	GV_ITEM Item; 
	Item.row = ItemIndex;
	Item.col = 0;
	Item.nFormat = DT_LEFT|DT_VCENTER;
	Item.mask = GVIF_TEXT|GVIF_PARAM;
	Item.strText  = strTemp;
	Item.lParam = (LPARAM)pThread;

	m_wndThreadsGrid.SetItem(&Item);


	Item.col = 1;
	Item.nFormat = DT_LEFT|DT_VCENTER;
	Item.mask	=  GVIF_TEXT|GVIF_IMAGE;
	
	BYTE nThreadState = pThread->GetThreadState();
	if (nThreadState == THREAD_STATE_RUNNING)
	{
		Item.iImage = 0;
		Item.strText = "������";
	}
	else if (nThreadState == THREAD_STATE_PAUSE)
	{
		Item.iImage = 1;
		Item.strText = "����";
	}
	else if (nThreadState == THREAD_STATE_STOP)
	{
		Item.iImage = 2;
		Item.strText = "ֹͣ";
	}
	m_wndThreadsGrid.SetItem(&Item);

	Item.mask = GVIF_TEXT;
	Item.col = 2;
	CString strCmType = CUdfGlobalFUNC::GetCmTypeString(pThread->GetCommType());
	if (pThread->GetCommType() == SC_CommChannel)
		strCmType += ":" + CUdfGlobalFUNC::GetSCFactoryString(pThread->GetScCommInfo()->nFactoryType);
	else if (pThread->GetCommType() == TEXTMSG_CommChannel)
		strCmType += ":" + CUdfGlobalFUNC::GetTmFactoryString(pThread->GetTmCommInfo()->nFactoryType);

	if (pThread->GetCommInterfaceType() == Ethernet_CommInerface)
	{
		strTemp.Format("Э������:%s,����IP�Ͷ˿�(%s,%d)",
			pThread->GetProtocolTypeString(),
			pThread->GetNetInfo()->mIpAddress,
			pThread->GetNetInfo()->mPort);
	}
	else if (pThread->GetCommInterfaceType() == Serial_CommInterface)
	{
		strTemp.Format("RS232,������Ϣ(%s,%d,%s)",
			pThread->GetComInfo()->mComName,
			pThread->GetComInfo()->mBaund,
			CUdfGlobalFUNC::GetParityString(pThread->GetComInfo()->mParity)
			);
	}
	CString strItem = "";
	strItem.Format("%s,%s",strCmType,strTemp);
	Item.strText = strItem;
	m_wndThreadsGrid.SetItem(&Item);

	Item.col = 3;
	CGridBtnCell *pbtnCell=NULL;
	m_wndThreadsGrid.SetCellType(Item.row,Item.col,RUNTIME_CLASS(CGridBtnCell));
	pbtnCell = (CGridBtnCell *)m_wndThreadsGrid.GetCell(Item.row,Item.col);
	pThread->m_btnStart.SetGrid(&m_wndThreadsGrid);
	pbtnCell->SetBtnDataBase(&pThread->m_btnStart);
	pbtnCell->SetupBtns(0,DFC_BUTTON,DFCS_BUTTONPUSH,CGridBtnCellBase::CTL_ALIGN_CENTER,
		0,FALSE,"����"); 


	Item.col = 4;
	m_wndThreadsGrid.SetCellType(Item.row,Item.col,RUNTIME_CLASS(CGridBtnCell));
	pbtnCell = (CGridBtnCell *)m_wndThreadsGrid.GetCell(Item.row,Item.col);
	pThread->m_btnStop.SetGrid(&m_wndThreadsGrid);
	pbtnCell->SetBtnDataBase(&pThread->m_btnStop);
	pbtnCell->SetupBtns(0,DFC_BUTTON,DFCS_BUTTONPUSH,CGridBtnCellBase::CTL_ALIGN_CENTER,
		0,FALSE,"ֹͣ"); 


	Item.col = 5;
	m_wndThreadsGrid.SetCellType(Item.row,Item.col,RUNTIME_CLASS(CGridBtnCell));
	pbtnCell = (CGridBtnCell *)m_wndThreadsGrid.GetCell(Item.row,Item.col);
	pThread->m_btnDelete.SetGrid(&m_wndThreadsGrid);
	pbtnCell->SetBtnDataBase(&pThread->m_btnDelete);
	pbtnCell->SetupBtns(0,DFC_BUTTON,DFCS_BUTTONPUSH ,CGridBtnCellBase::CTL_ALIGN_CENTER,
		0,FALSE,"ɾ��"); 

	Item.col = 6;
	m_wndThreadsGrid.SetCellType(Item.row,Item.col,RUNTIME_CLASS(CGridBtnCell));
	pbtnCell = (CGridBtnCell *)m_wndThreadsGrid.GetCell(Item.row,Item.col);
	pThread->m_btnPro.SetGrid(&m_wndThreadsGrid);
	pbtnCell->SetBtnDataBase(&pThread->m_btnPro);
	pbtnCell->SetupBtns(0,DFC_BUTTON,DFCS_BUTTONPUSH,CGridBtnCellBase::CTL_ALIGN_CENTER,
		0,FALSE,"����"); 

	Item.col = 7;
	m_wndThreadsGrid.SetCellType(Item.row,Item.col,RUNTIME_CLASS(CGridBtnCell));
	pbtnCell = (CGridBtnCell *)m_wndThreadsGrid.GetCell(Item.row,Item.col);
	pThread->m_btnInit.SetGrid(&m_wndThreadsGrid);
	pbtnCell->SetBtnDataBase(&pThread->m_btnInit);
	pbtnCell->SetupBtns(0,DFC_BUTTON,DFCS_BUTTONPUSH,CGridBtnCellBase::CTL_ALIGN_CENTER,
		0,FALSE,"��ʼ��"); 
	


}
void CCommServerMnDlg::OnBnClickedBtnOk()
{
	ShowWindow(SW_HIDE);
}


void CCommServerMnDlg::OnLClickThreadGrid(NMHDR *pNotifyStruct,LRESULT *pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;

	if (!pItem)					return;
	if (pItem->iRow <=0 )		return;

	CCommThreadInfo* pThreadInfo = NULL;
	pThreadInfo = (CCommThreadInfo*)m_wndThreadsGrid.GetItemData(pItem->iRow,0);

	if (!pThreadInfo)			return;

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	CCommServer* pCommServer = pApp->m_pCommServer;
	if (!pCommServer)			return;

	if (pItem->iColumn == 3)				//���� Start
	{
		if (pThreadInfo->GetThreadState() != THREAD_STATE_RUNNING)
		{
			pCommServer->StartCommThread(pThreadInfo);
			UpdateThreadState(pThreadInfo,pItem->iRow);

			if (pThreadInfo->GetThreadState() == THREAD_STATE_RUNNING && 
				pThreadInfo->GetCommInterfaceType() == Serial_CommInterface)
				pCommServer->BuildRelaCommToRtu();
		}
	}
	else if (pItem->iColumn == 4)			//ֹͣ Suspend
	{
		if (pThreadInfo->GetThreadState() == THREAD_STATE_RUNNING)
		{
			int ret  = MessageBox("ȷ��ֹͣ�������е�Commserver�������߳�?","��ʾ",MB_YESNO|MB_ICONQUESTION);
			if (ret != IDYES)
				return;
		}

		pCommServer->StopCommThread(pThreadInfo);
		UpdateThreadState(pThreadInfo,pItem->iRow);
	}
	else if (pItem->iColumn == 5)			//ɾ�� Stop
	{
		if (DeleteCommThreadPro(pThreadInfo))
		{
			InitGridHeader();
			ShowAllThreadsInfo();
			pCommServer->SaveThreadsCfg();
		}
	}
	else if (pItem->iColumn == 6)			//���� Pro
	{
		if (ModifyCommThreadPro(pThreadInfo))
		{
			InitGridHeader();
			ShowAllThreadsInfo();
			pCommServer->SaveThreadsCfg();

			if (pThreadInfo->GetThreadState() == THREAD_STATE_RUNNING && 
				pThreadInfo->GetCommInterfaceType() == Serial_CommInterface)
				pCommServer->BuildRelaCommToRtu();
		}
	}
	else if (pItem->iColumn == 7)
	{
		if (pThreadInfo->GetCommType() == TEXTMSG_CommChannel)
		{
			if (InitMsgCommThreadPro(pThreadInfo))
			{
			}
			
		}
		else
		{
			MessageBox("ͨѶ�ӿ����Ͳ��Ƕ��Žӿڣ�","����",MB_OK|MB_ICONERROR);
			return;
		}
	}
	else 
		return;
}

void CCommServerMnDlg::UpdateThreadState(CCommThreadInfo* pThread,int ItemRow)
{
	if (!pThread)				return;
	if (ItemRow <= 0)			return;
	if (ItemRow>= m_wndThreadsGrid.GetRowCount())	return;

	GV_ITEM Item; 
	Item.row = ItemRow;
	Item.col = 1;
	Item.nFormat = DT_LEFT|DT_VCENTER;
	Item.mask    = GVIF_TEXT|GVIF_IMAGE;

	BYTE nThreadState = pThread->GetThreadState();
	if (nThreadState == THREAD_STATE_RUNNING)
	{
		Item.iImage = 0;
		Item.strText = "������";
	}
	else if (nThreadState == THREAD_STATE_PAUSE)
	{
		Item.iImage = 1;
		Item.strText = "����";
	}
	else if (nThreadState == THREAD_STATE_STOP)
	{
		Item.iImage = 2;
		Item.strText = "ֹͣ";
	}
	m_wndThreadsGrid.SetItem(&Item);
	m_wndThreadsGrid.Invalidate();
}


void CCommServerMnDlg::InitListCtrlHeader()
{
	if (!m_wndThreadLog.GetSafeHwnd())	return;

	DWORD dw = m_wndThreadLog.GetExtendedStyle();
	dw |= LVS_EX_FULLROWSELECT;
	m_wndThreadLog.SetExtendedStyle(dw);

	m_wndThreadLog.DeleteAllItems();
	m_wndThreadLog.InsertColumn(0, _T("ʱ��"));
	m_wndThreadLog.InsertColumn(1, _T("��Ϣ"));
	m_wndThreadLog.SetColumnWidth(0, 160);
	m_wndThreadLog.SetColumnWidth(1, 1000);
}

void CCommServerMnDlg::SetTextMsgDlg(CRtuAttrTextMsg *pDlg)
{
	m_pTextMsgDlg = pDlg;
}

void CCommServerMnDlg::InsertLog(CString strLog,_Log_level_type xLevle,PSYSTEMTIME pST/* =NULL */)
{
	if (!m_wndThreadLog.GetSafeHwnd()) return;

	CString strTime="";
	if (!pST)
	{
		SYSTEMTIME st;
		GetLocalTime(&st);
		strTime.Format("%04d-%02d-%02d %02d:%02d:%02d.%03d",
			st.wYear,
			st.wMonth,
			st.wDay,
			st.wHour,
			st.wMinute,
			st.wSecond,
			st.wMilliseconds);
	}
	else
	{
		strTime.Format("%04d-%02d-%02d %02d:%02d:%02d.%03d",
			pST->wYear,
			pST->wMonth,
			pST->wDay,
			pST->wHour,
			pST->wMinute,
			pST->wSecond,
			pST->wMilliseconds);
	}

	int Idx = m_wndThreadLog.InsertItem(0,strTime);
	m_wndThreadLog.SetItemText(Idx,1,strLog);
	m_wndThreadLog.SetItemData(Idx,(DWORD_PTR)xLevle);

	int nCount = m_wndThreadLog.GetItemCount() ;
	if (nCount >= 1000)
	{
		m_wndThreadLog.DeleteItem(nCount-1);
	}
}
void CCommServerMnDlg::OnBnClickedButton1()
{

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	CCommServer* pCommServer = pApp->m_pCommServer;
	if (!pCommServer)			return;

	if (pCommServer->m_lstThreads.GetCount() >= 10 )
	{
		MessageBox("Commserver�����߳������ﵽ10��������,����\r\n������µ�Commserver�����߳�!","��ʾ",MB_OK|MB_ICONWARNING);
		return;
	}

	CCommThreadProDlg dlg(this);
	if (dlg.DoModal()== IDOK)
	{
		CCommThreadInfo* pThreadNew = new CCommThreadInfo();

		if (pThreadNew)
		{
			pThreadNew->SetComInfo(&(dlg.m_Com_Info));
			pThreadNew->SetNetInfo(&(dlg.m_Net_Info));
			pThreadNew->SetCommType(dlg.m_bCom_Type);
			pThreadNew->SetCommInferfaceType(dlg.m_bCom_Interface);
			pThreadNew->SetProtocolType(dlg.m_bProtocolType);
			pThreadNew->SetScCommInfo(&(dlg.m_Sc_Comm_Info));
			pThreadNew->SetTmCommInfo(&(dlg.m_Tm_Comm_Info));
			if (pCommServer->AddCommThread(pThreadNew))
			{
				InitGridHeader();
				ShowAllThreadsInfo();

				pCommServer->SaveThreadsCfg();
				
				if (pThreadNew->GetThreadState() == THREAD_STATE_RUNNING && 
					pThreadNew->GetCommInterfaceType() == Serial_CommInterface)
					pCommServer->BuildRelaCommToRtu();
			}
		}
	}
}

bool CCommServerMnDlg::ModifyCommThreadPro(CCommThreadInfo* pThread)
{
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	CCommServer* pCommServer = pApp->m_pCommServer;
	if (!pCommServer)			return false;

	CCommThreadProDlg dlg(this);
	dlg.SetCommThreadInfo(pThread);
	INT_PTR ret = dlg.DoModal();
	if (ret == IDOK)
	{
		bool blModify = false;
		if (pThread->GetCommType() != dlg.m_bCom_Type)
		{
			if (pThread->GetCommType() == SC_CommChannel ||
			    (pThread->GetCommType() != SC_CommChannel &&
				dlg.m_bCom_Type == SC_CommChannel))
			{
				if (pCommServer->StopCommThread(pThread) == true)
				{
					pThread->SetCommType(dlg.m_bCom_Type);
					pThread->SetComInfo(&(dlg.m_Com_Info));
					pThread->SetNetInfo(&(dlg.m_Net_Info));
					pThread->SetCommInferfaceType(dlg.m_bCom_Interface);
					pThread->SetProtocolType(dlg.m_bProtocolType);
					pThread->SetScCommInfo(&(dlg.m_Sc_Comm_Info));
					pThread->SetTmCommInfo(&(dlg.m_Tm_Comm_Info));
					pCommServer->StartCommThread(pThread);
					return true;
				}
				else
					return false;
			}

			pThread->SetCommType(dlg.m_bCom_Type);
			blModify = true;
		}

		//�ж��Ƿ���Ҫ�����߳�,��������Ӧ�Ĳ���ѡ����Ϣ
		if ((pThread->GetCommInterfaceType() != dlg.m_bCom_Interface))
		{
			if (pCommServer->StopCommThread(pThread) == true)
			{
				pThread->SetComInfo(&(dlg.m_Com_Info));
				pThread->SetNetInfo(&(dlg.m_Net_Info));
				pThread->SetCommInferfaceType(dlg.m_bCom_Interface);
				pThread->SetProtocolType(dlg.m_bProtocolType);
				pThread->SetScCommInfo(&(dlg.m_Sc_Comm_Info));
				pThread->SetTmCommInfo(&(dlg.m_Tm_Comm_Info));
				pCommServer->StartCommThread(pThread);
				return true;
			}
			else
				return false;

		}
		else if (pThread->GetCommInterfaceType() == dlg.m_bCom_Interface)
		{
			BYTE bInterface = pThread->GetCommInterfaceType();
			if ( bInterface == Ethernet_CommInerface)  //net
			{
				if (pThread->GetNetInfo()->mIpAddress != dlg.m_Net_Info.mIpAddress ||
					pThread->GetNetInfo()->mPort	  != dlg.m_Net_Info.mPort	   ||
					pThread->GetProtocolType()		  != dlg.m_bProtocolType)
				{
					if (pCommServer->StopCommThread(pThread) == true)
					{
						pThread->SetComInfo(&(dlg.m_Com_Info));
						pThread->SetNetInfo(&(dlg.m_Net_Info));
						pThread->SetProtocolType(dlg.m_bProtocolType);
						pThread->SetScCommInfo(&(dlg.m_Sc_Comm_Info));
						pThread->SetTmCommInfo(&(dlg.m_Tm_Comm_Info));
						pCommServer->StartCommThread(pThread);
						return true;
					}
					else
						return false;
				}
				else
				{
					pThread->SetComInfo(&(dlg.m_Com_Info));
					pThread->SetScCommInfo(&(dlg.m_Sc_Comm_Info));
					pThread->SetTmCommInfo(&(dlg.m_Tm_Comm_Info));
				}
			}
			else if (bInterface == Serial_CommInterface)
			{
				if (pThread->GetComInfo()->mComName != dlg.m_Com_Info.mComName ||
					pThread->GetComInfo()->mBaund	!= dlg.m_Com_Info.mBaund   ||
					pThread->GetComInfo()->mDataBits!= dlg.m_Com_Info.mDataBits||
					pThread->GetComInfo()->mParity	!= dlg.m_Com_Info.mParity  ||
					pThread->GetComInfo()->mStopBits!= dlg.m_Com_Info.mStopBits ||
					(pThread->GetCommType() == SC_CommChannel && 
					 (pThread->GetScCommInfo()->nScAddress != dlg.m_Sc_Comm_Info.nScAddress ||
					 pThread->GetScCommInfo()->nFactoryType != dlg.m_Sc_Comm_Info.nFactoryType)))
				{
					
					if (pCommServer->StopCommThread(pThread) == true)
					{
						pThread->SetComInfo(&(dlg.m_Com_Info));
						pThread->SetNetInfo(&(dlg.m_Net_Info));
						pThread->SetScCommInfo(&(dlg.m_Sc_Comm_Info));
						pThread->SetTmCommInfo(&(dlg.m_Tm_Comm_Info));
						pCommServer->StartCommThread(pThread);
						return true;
					}
					else
						return false;
				}
				else
				{
					pThread->SetNetInfo(&(dlg.m_Net_Info));
					pThread->SetTmCommInfo(&(dlg.m_Tm_Comm_Info));
					pThread->SetScCommInfo(&(dlg.m_Sc_Comm_Info));
				}
			}
		}
		return blModify;	
	}
	
	return false;
}


bool CCommServerMnDlg::DeleteCommThreadPro(CCommThreadInfo* pTHread)
{
	int ret = MessageBox("ȷ��ɾ����Commserver�����̣߳�","��ʾ",MB_YESNO|MB_ICONQUESTION);
	if (ret != IDYES)
		return false;

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	CCommServer* pCommServer = pApp->m_pCommServer;
	if (!pCommServer)			return false;


	return pCommServer->DeleteCommThread(pTHread);
}

bool CCommServerMnDlg::InitMsgCommThreadPro(CCommThreadInfo* pThread)
{

	if (pThread == NULL)
	{
		return false;
	}

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	CCommServer* pCommServer = pApp->m_pCommServer;
	if (!pCommServer)			return false;

	if (m_pTextMsgDlg != NULL)
	{
		m_pTextMsgDlg->SetCommThreadInfo(pThread);
	}

	CRtuAttrMsgInit dlg(this);
	dlg.SetCommThreadInfo(pThread);
	INT_PTR ret = dlg.DoModal();

	if (dlg.DoModal()== IDOK)
	{
		return true;
	}


	return false;
}

void CCommServerMnDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (nType == SIZE_MINIMIZED)
		ShowWindow(SW_HIDE);
}
