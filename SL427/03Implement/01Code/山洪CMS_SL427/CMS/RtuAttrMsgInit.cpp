// RtuAttrMsgInit.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuAttrMsgInit.h"
#include "ScModuleBase.h"


// CRtuAttrMsgInit 对话框
#define Msg_Send_Timeout   1

IMPLEMENT_DYNAMIC(CRtuAttrMsgInit, CDialog)

CRtuAttrMsgInit::CRtuAttrMsgInit(CWnd* pParent /*=NULL*/)
	: CDialog(CRtuAttrMsgInit::IDD, pParent)
{
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	m_it_current = pApp->m_TextMsgInitHash.begin();
	m_nCurrentRowIdxGrid = 1;
	m_CurrentSendMsg = NULL;
	m_nCurrentCmd = Invaild_cmd;
	m_bReturn = false;
	m_strRecvInfo = "";
	m_CurrentRow = 1;


}

CRtuAttrMsgInit::~CRtuAttrMsgInit()
{
}

void CRtuAttrMsgInit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_BTN_INITSTART,m_StartInitBtn);
	DDX_Control(pDX,IDC_BTN_INITSTOP, m_StopInitBtn);
	DDX_Control(pDX,IDC_LET_UPDATEINFO,m_MsgInitInfoLet);
}

BOOL CRtuAttrMsgInit::OnInitDialog()
{
	CDialog::OnInitDialog();
	//SetWndsEnable(FALSE, 1);

	CWnd* pWnd = GetDlgItem(IDC_STATIC_MSGLIST);
	CRect rect1, rect2;
	int captionHeight = 0; 
	int cxframe = GetSystemMetrics(SM_CXFRAME);
	int cyframe = GetSystemMetrics(SM_CYFRAME);
	this->GetWindowRect(&rect2);
	if (pWnd)
		pWnd->GetWindowRect(&rect1);
	if (!m_GridCtrl.Create(CRect(rect1.left - rect2.left - cxframe, 
		rect1.top-rect2.top - cyframe-captionHeight, 
		rect1.left + rect1.Width() - rect2.left, 
		rect1.top + rect1.Height() - rect2.top - captionHeight), 
		this, IDC_WATERLEVEL_GRIDCTRL,WS_CHILD|WS_VISIBLE))
	{
		TRACE("Failed to create threads  grid!\n");
		return FALSE;
	}

	m_GridCtrl.SetBkColor(RGB(255,255,255));
	m_GridCtrl.SetListMode(TRUE);
	m_GridCtrl.SetSingleColSelection(TRUE);
	m_GridCtrl.SetSingleRowSelection(TRUE);

	InitGridItems();

	return TRUE;
}

void CRtuAttrMsgInit::InitGridItems()
{
	int nSel = 0;
	if (m_GridCtrl.GetSafeHwnd())
	{
		m_GridCtrl.DeleteAllItems();
		CString strHeader[] = 
		{
			"行号",
			"状态",
			"时间",
			"命令类型"
		};

		CCMSApp* pApp = (CCMSApp*)AfxGetApp();
		int nRow = pApp->m_TextMsgInitHash.size();

		m_CurrentRow = 1;
		m_GridCtrl.SetRowCount(nRow+1);
		m_GridCtrl.SetColumnCount(4);
		m_GridCtrl.SetColumnWidth(0,50);
		m_GridCtrl.SetColumnWidth(1,80);
		m_GridCtrl.SetColumnWidth(2,160);
		m_GridCtrl.SetColumnWidth(3,280);


		for (int i=0; i<4; i++)
		{
			m_GridCtrl.SetItemText(0, i, strHeader[i]);
			CUdfGlobalFUNC::SetGridCellReadOnly(&m_GridCtrl,0,i);
			m_GridCtrl.SetItemBkColour(0,i,RGB(128,128,128));
		}

		int nRowIdx = 1;
		CString strRowIdx;

// 		for (it=it_b; it!=it_e; it++)
// 		{
// 			msg = (WaitingMsg*)(*it);
// 			if (msg == NULL)
// 			{
// 				nRowIdx++;
// 				continue;
// 			}
// 
// 			strRowIdx.Format("%d", nRowIdx);
// 			m_GridCtrl.SetItemText(nRowIdx, 0, strRowIdx);
// 			switch(msg->GetMsgStatus())
// 			{
// 			case MsgStatus::Sending:
// 				m_GridCtrl.SetItemText(nRowIdx, 1, _T("正在发送"));
// 				break;
// 
// 			}
// 
// 			if (m_it_current != pApp->m_TextMsgInitHash.end())
// 			{
// 				m_GridCtrl.SetItemText(nRowIdx, 3, m_it_current->second);
// 			}
// 
// 
// 			switch(msg->GetMsgType())
// 			{
// 			case AT:
// 				m_GridCtrl.SetItemText(nRowIdx, 3, _T("错误的召唤类型"));
// 				break;
// 			case MsgStatus::scsw_call_p:
// 				m_GridCtrl.SetItemText(nRowIdx, 3, _T("召唤雨量"));
// 				break;
// 			}
// 
// 			nRowIdx++;
// 
// 		}


		m_GridCtrl.Refresh();

	}

}


void CRtuAttrMsgInit::SetCmd(int cmd)
{
	m_cs_cmd.Lock();
	m_nCurrentCmd = cmd;
	m_cs_cmd.Unlock();
}

int CRtuAttrMsgInit::GetCmd()
{
	return m_nCurrentCmd;
}

void CRtuAttrMsgInit::StartTimer(int itv)
{
	SetTimer(Msg_Send_Timeout, itv,  NULL);
}

void CRtuAttrMsgInit::SendData(TextMsg *m_CurrentSendMsg)
{
	CScModuleBase *pBase = m_pCurCommThreadInfo->GetScModule();
	int TmType = pBase->GetTmFactoryType();
	TextMsg_GHT *pGht = NULL;
	switch(TmType)
	{
	case SCTextMsgType_GHT:	
		pGht = static_cast<TextMsg_GHT*>(pBase);
		if (pGht != NULL)
		{
			pGht->SetMsgInitDlg(this);
			pGht->SendDown_Message(m_CurrentSendMsg, MSG_INIT);
		}
		break;
	case SCTextMsgType_YT:
		break;
	default:
		break;
	}

}


void CRtuAttrMsgInit::UpdateGrid(int cmd, int attrType, CString str, int len)
{
	m_nCurrentCmd = cmd;
	m_strRecvInfo = str;
	switch(attrType)
	{
	case MsgInit_Recv:
		m_bReturn = true;
		SendMessage(WM_TIMER, Msg_Send_Timeout, NULL);
		break;
	}

}

void CRtuAttrMsgInit::AppendRowToGrid(TextMsg *m_CurrentSendMsg)
{
	if (m_GridCtrl.GetSafeHwnd())
	{
		int nGridRowCnt = m_GridCtrl.GetRowCount();

		if (m_CurrentRow >= nGridRowCnt)
		{
			return;
		}

		CString strRowCnt;
		strRowCnt.Format("%d", m_CurrentRow);

		//nGridRowCnt = m_GridCtrl.InsertRow(strRowCnt);
		m_GridCtrl.SetItemText(m_CurrentRow, 0,strRowCnt);
		if (m_CurrentSendMsg != NULL)
		{
			switch(m_CurrentSendMsg->m_state)
			{
			case SENDING:
				m_GridCtrl.SetItemText(m_CurrentRow, 1, _T("正在发送"));
				break;
			case SENT:
				m_GridCtrl.SetItemText(m_CurrentRow, 1, _T("已发送"));
				break;
			case FAIL:
				m_GridCtrl.SetItemText(m_CurrentRow, 1, _T("发送失败"));
				break;
			case SUCCESS:
				m_GridCtrl.SetItemText(m_CurrentRow, 1, _T("发送成功"));
				break;
			default:
				m_GridCtrl.SetItemText(m_CurrentRow, 1, _T("未知发送类型"));
				break;	
			}

			SYSTEMTIME t;
			::GetLocalTime(&t);
			CString strTime = "";
			strTime.Format("%d年%d月%d日 %d时%d分%d秒",t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
			m_GridCtrl.SetItemText(m_CurrentRow, 2, strTime);

			switch(m_CurrentSendMsg->m_Type)
			{
			case AT:
				m_GridCtrl.SetItemText(m_CurrentRow, 3, _T("发送命令--->AT"));
				break;
			case ATE0:
				m_GridCtrl.SetItemText(m_CurrentRow, 3, _T("发送命令--->ATE0"));
				break;
			case AT_ADD_CGMR:
				m_GridCtrl.SetItemText(m_CurrentRow, 3, _T("发送命令--->AT+CGMR"));
				break;
			case AT_ADD_CGSN:
				m_GridCtrl.SetItemText(m_CurrentRow, 3, _T("发送命令--->AT+CSGN"));
				break;
			case AT_ADD_CIMI:
				m_GridCtrl.SetItemText(m_CurrentRow, 3, _T("发送命令--->AT+CIMI"));
				break;
			case AT_ADD_CMEE:
				m_GridCtrl.SetItemText(m_CurrentRow, 3, _T("发送命令--->AT+CMEE=2"));
				break;
			case AT_ADD_CPIN_ASK:
				m_GridCtrl.SetItemText(m_CurrentRow, 3, _T("发送命令--->AT+CPIN?"));
				break;
			case AT_ADD_CREG:
				m_GridCtrl.SetItemText(m_CurrentRow, 3, _T("发送命令--->AT+CREG=1"));
				break;
			case AT_ADD_CREG_ASK:
				m_GridCtrl.SetItemText(m_CurrentRow, 3, _T("发送命令--->AT+CREG?"));
				break;
			case AT_ADD_CSQ:
				m_GridCtrl.SetItemText(m_CurrentRow, 3, _T("发送命令--->AT+CSQ"));
				break;
			case AT_ADD_CGREG:
				m_GridCtrl.SetItemText(m_CurrentRow, 3, _T("发送命令--->AT+CGREG=1"));
				break;
			case  AT_ADD_CGREG_ASK:
				m_GridCtrl.SetItemText(m_CurrentRow, 3, _T("发送命令--->AT+CGREG? "));
				break;
			case AT_ADD_CGATT_ASK:
				m_GridCtrl.SetItemText(m_CurrentRow, 3, _T("发送命令--->AT+CGATT?"));
				break;
			case AT_ADD_CLIP:
				m_GridCtrl.SetItemText(m_CurrentRow, 3, _T("发送命令--->AT+CLIP=1"));
				break;
			case AT_ADD_CNMI:
				m_GridCtrl.SetItemText(m_CurrentRow, 3, _T("发送命令--->AT+CNMI=1,1,0,0,0"));
				break;
			case AT_ADD_CMGF:
				m_GridCtrl.SetItemText(m_CurrentRow, 3, _T("发送命令--->AT+CMGF=1"));
				break;
			case AT_ADD_CSCA_ASK:
				m_GridCtrl.SetItemText(m_CurrentRow, 3, _T("发送命令--->AT+CSCA?"));
				break;
			default:
				m_GridCtrl.SetItemText(m_CurrentRow, 3, _T("发送未知命令"));
				break;

			}

			

		}

		m_CurrentRow++;
		m_GridCtrl.Refresh();

	}



}

void CRtuAttrMsgInit::SetCommThreadInfo(CCommThreadInfo* pThreadInfo)
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


BEGIN_MESSAGE_MAP(CRtuAttrMsgInit, CDialog)
	ON_BN_CLICKED(IDC_BTN_INITSTART, &CRtuAttrMsgInit::OnBnClickedBtnInitstart)
	ON_BN_CLICKED(IDOK, &CRtuAttrMsgInit::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CRtuAttrMsgInit::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_INITSTOP, &CRtuAttrMsgInit::OnBnClickedBtnInitstop)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CRtuAttrMsgInit 消息处理程序
void CRtuAttrMsgInit::OnTimer(UINT_PTR nIDEvent)
{
	CString strLogMessge = "";
	_Log_level_type _logLevle = Normal_Log_Message;
	CString str = "";

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();

	switch (nIDEvent)
	{
	case  Msg_Send_Timeout:
		{
			if (m_GridCtrl.GetSafeHwnd())
			{
				if (!m_bReturn)
				{
					m_MsgInitInfoLet.SetWindowText("命令接收超时，初始化失败");
					m_it_current = pApp->m_TextMsgInitHash.begin();
					m_StartInitBtn.EnableWindow(TRUE);
					m_StopInitBtn.EnableWindow(FALSE);
					InitGridItems();
					
					m_CurrentRow = 1;
				}
				else 
				{
					m_bReturn = false;
					switch(m_nCurrentCmd)
					{
					case Invaild_cmd:
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 3, _T("无效命令"));
						m_MsgInitInfoLet.SetWindowText("无效命令，初始化失败");
						m_it_current = pApp->m_TextMsgInitHash.begin();
						m_StartInitBtn.EnableWindow(TRUE);
						m_StopInitBtn.EnableWindow(FALSE);
						KillTimer(nIDEvent);
						break;
					case AT_waiting_OK:
						str = _T("发送命令--->AT\n接收命令--->AT");
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 3, str);
						m_MsgInitInfoLet.SetWindowText("AT命令，初始化未完成");
						m_StartInitBtn.EnableWindow(FALSE);
						m_StopInitBtn.EnableWindow(TRUE);
						StartTimer();
						break;
					case AT_OK_finish:
						str = str + _T("\n接收命令--->OK");
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 3, str);
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 2, _T("已完成"));
						m_MsgInitInfoLet.SetWindowText("AT命令，初始化未完成");
						m_StartInitBtn.EnableWindow(FALSE);
						m_StopInitBtn.EnableWindow(TRUE);
						m_it_current++;
						DoTextMsgInit();
						StartTimer();
						str = "";
						break;
					case ATE0_OK_finish:
						str = _T("发送命令--->ATE0\n接收命令--->OK");
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 3, str);
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 2, _T("已完成"));
                        m_MsgInitInfoLet.SetWindowText("ATE0命令，初始化未完成");
						m_StartInitBtn.EnableWindow(FALSE);
						m_StopInitBtn.EnableWindow(TRUE);
						m_it_current++;
						DoTextMsgInit();
						StartTimer();
						str = "";
						break;
					case CGMR_waiting_OK:
						str.Format("发送命令--->AT+CGMR\n接收命令--->%s",m_strRecvInfo);
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 3, str);
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 2, _T("未完成"));
						m_MsgInitInfoLet.SetWindowText("AT+CGMR命令，初始化未完成");
						m_StartInitBtn.EnableWindow(FALSE);
						m_StopInitBtn.EnableWindow(TRUE);
						StartTimer();
						break;
					case CGMR_OK_finish:
						str = str + _T("\n接收命令--->OK");
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 3, str);
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 2, _T("已完成"));
						m_MsgInitInfoLet.SetWindowText("AT+CGMR命令，初始化未完成");
						m_StartInitBtn.EnableWindow(FALSE);
						m_StopInitBtn.EnableWindow(TRUE);
						m_it_current++;
						DoTextMsgInit();
						StartTimer();
						str = "";
						break;
					case CGSN_waiting_OK:
						str.Format("发送命令--->AT+CGSN\n接收命令--->%s",m_strRecvInfo);
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 3, str);
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 2, _T("未完成"));
						m_MsgInitInfoLet.SetWindowText("AT+CGSN命令，初始化未完成");
						m_StartInitBtn.EnableWindow(FALSE);
						m_StopInitBtn.EnableWindow(TRUE);
						StartTimer();
						break;
					case CGSN_OK_finish:
						str = str + _T("\n接收命令--->OK");
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 3, str);
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 2, _T("已完成"));
						m_MsgInitInfoLet.SetWindowText("AT+CGSN命令，初始化未完成");
						m_StartInitBtn.EnableWindow(FALSE);
						m_StopInitBtn.EnableWindow(TRUE);
						m_it_current++;
						DoTextMsgInit();
						StartTimer();
						str = "";
						break;
					case CIMI_waiting_OK:
						str.Format("发送命令--->AT+CIMI\n接收命令--->%s",m_strRecvInfo);;
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 3, str);
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 2, _T("未完成"));
						m_MsgInitInfoLet.SetWindowText("AT+CIMI命令，初始化未完成");
						m_StartInitBtn.EnableWindow(FALSE);
						m_StopInitBtn.EnableWindow(TRUE);
						StartTimer();
						break;
					case CIMI_OK_finish:
						str = str + _T("\n接收命令--->OK");
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 3, str);
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 2, _T("已完成"));
						m_MsgInitInfoLet.SetWindowText("AT+CIMI命令，初始化未完成");
						m_StartInitBtn.EnableWindow(FALSE);
						m_StopInitBtn.EnableWindow(TRUE);
						m_it_current++;
						DoTextMsgInit();
						StartTimer();
						str = "";
						break;
					case CMEE_OK_finish:
						str = _T("发送命令--->AT+CMEE=2\n接收命令--->OK");
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 3, str);
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 2, _T("已完成"));
						m_MsgInitInfoLet.SetWindowText("AT+CMEE命令，初始化未完成");
						m_StartInitBtn.EnableWindow(FALSE);
						m_StopInitBtn.EnableWindow(TRUE);
						m_it_current++;
						DoTextMsgInit();
						StartTimer();
						str = "";
						break;
					case CPIN_waiting_OK:
						str.Format("发送命令--->AT+AT+CPIN?\n接收命令--->%s",m_strRecvInfo);
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 3, str);
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 2, _T("未完成"));
						m_MsgInitInfoLet.SetWindowText("AT+CPIN?命令，初始化未完成");
						m_StartInitBtn.EnableWindow(FALSE);
						m_StopInitBtn.EnableWindow(TRUE);
						StartTimer();
						break;
					case CPIN_OK_finish:
						str = str + _T("\n接收命令--->OK");
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 3, str);
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 2, _T("已完成"));
						m_MsgInitInfoLet.SetWindowText("ATE0命令，初始化未完成");
						m_StartInitBtn.EnableWindow(FALSE);
						m_StopInitBtn.EnableWindow(TRUE);
						m_it_current++;
						DoTextMsgInit();
						StartTimer();
						str = "";
						break;
					case CREG_waiting_OK:
						str.Format("发送命令--->AT+CREG?\n接收命令--->%s",m_strRecvInfo);
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 3, str);
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 2, _T("未完成"));
						m_MsgInitInfoLet.SetWindowText("AT+CREG?，初始化未完成");
						m_StartInitBtn.EnableWindow(FALSE);
						m_StopInitBtn.EnableWindow(TRUE);
						StartTimer();
						break;
					case CREG_OK_finish:
						str = str + _T("\n接收命令--->OK");
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 3, str);
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 2, _T("已完成"));
						m_MsgInitInfoLet.SetWindowText("AT+CREG?，初始化未完成");
						m_StartInitBtn.EnableWindow(FALSE);
						m_StopInitBtn.EnableWindow(TRUE);
						m_it_current++;
						DoTextMsgInit();
						StartTimer();
						str = "";
						break;
					case CGEGa_OK_finish:
						str = _T("发送命令--->AT+CREG=1\n接收命令--->OK");
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 3, str);
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 2, _T("已完成"));
						m_MsgInitInfoLet.SetWindowText("AT+CREG=1，初始化未完成");
						m_StartInitBtn.EnableWindow(FALSE);
						m_StopInitBtn.EnableWindow(TRUE);
						m_it_current++;
						DoTextMsgInit();
						StartTimer();
						break;
					case CSQ_waiting_OK:
						str.Format("发送命令--->AT+CSQ\n接收命令--->%s",m_strRecvInfo);
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 3, str);
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 2, _T("未完成"));
						m_MsgInitInfoLet.SetWindowText("AT+CSQ命令，初始化未完成");
						m_StartInitBtn.EnableWindow(FALSE);
						m_StopInitBtn.EnableWindow(TRUE);
						StartTimer();
						break;
					case CSQ_OK_finish:
						str = str + _T("\n接收命令--->OK");
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 3, str);
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 2, _T("已完成"));
						m_MsgInitInfoLet.SetWindowText("AT+CSQ命令，初始化未完成");
						m_StartInitBtn.EnableWindow(FALSE);
						m_StopInitBtn.EnableWindow(TRUE);
						m_it_current++;
						DoTextMsgInit();
						StartTimer();
						str = "";
						break;
					case CGREG_OK_finish:
						str = _T("发送命令--->AT+CRREG=1\n接收命令--->OK");
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 3, str);
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 2, _T("已完成"));
						m_MsgInitInfoLet.SetWindowText("AT+CRREG=1，初始化未完成");
						m_StartInitBtn.EnableWindow(FALSE);
						m_StopInitBtn.EnableWindow(TRUE);
						m_it_current++;
						DoTextMsgInit();
						StartTimer();
						str = "";
						break;
					case CGREG_waiting_OK:
						str.Format("发送命令--->AT+CGREG?\n接收命令--->%s",m_strRecvInfo);
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 3, str);
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 2, _T("未完成"));
						m_MsgInitInfoLet.SetWindowText("AT+CGREG?命令，初始化未完成");
						m_StartInitBtn.EnableWindow(FALSE);
						m_StopInitBtn.EnableWindow(TRUE);
						StartTimer();
						break;
					case CGREGa_OK_finish:
						str = str + _T("\n接收命令--->OK");
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 3, str);
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 2, _T("已完成"));
						m_MsgInitInfoLet.SetWindowText("AT+CGREG?命令，初始化未完成");
						m_StartInitBtn.EnableWindow(FALSE);
						m_StopInitBtn.EnableWindow(TRUE);
						m_it_current++;
						DoTextMsgInit();
						StartTimer();
						str = "";
						break;
					case CGATT_waiting_OK:
						str.Format("发送命令--->AT+CGATT?\n接收命令--->%s",m_strRecvInfo);
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 3, str);
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 2, _T("未完成"));
						m_MsgInitInfoLet.SetWindowText("AT+CGATT?，初始化未完成");
						m_StartInitBtn.EnableWindow(FALSE);
						m_StopInitBtn.EnableWindow(TRUE);
						StartTimer();
						break;
					case CGATT_OK_finish:
						str = str + _T("\n接收命令--->OK");
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 3, str);
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 2, _T("已完成"));
						m_MsgInitInfoLet.SetWindowText("AT+CGATT?命令，初始化未完成");
						m_StartInitBtn.EnableWindow(FALSE);
						m_StopInitBtn.EnableWindow(TRUE);
						m_it_current++;
						DoTextMsgInit();
						StartTimer();
						str = "";
						break;
					case CLIP_OK_finish:
						str = _T("发送命令--->AT+CLIP=1\n接收命令--->OK");
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 3, str);
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 2, _T("已完成"));
						m_MsgInitInfoLet.SetWindowText("AT+CLIP=1命令，初始化未完成");
						m_StartInitBtn.EnableWindow(FALSE);
						m_StopInitBtn.EnableWindow(TRUE);
						m_it_current++;
						DoTextMsgInit();
						StartTimer();
						break;
					case CNMI_OK_finish:
						str = _T("发送命令--->AT+CNMI=1,1,0,0,0\n接收命令--->OK");
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 3, str);
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 2, _T("已完成"));
						m_MsgInitInfoLet.SetWindowText("AT+CNMI=1,1,0,0,0命令，初始化未完成");
						m_StartInitBtn.EnableWindow(FALSE);
						m_StopInitBtn.EnableWindow(TRUE);
						m_it_current++;
						DoTextMsgInit();
						StartTimer();
						break;
					case CMGF_OK_finish:
						str = _T("发送命令--->AT+CMGF\n接收命令--->OK");
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 3, str);
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 2, _T("已完成"));
						m_MsgInitInfoLet.SetWindowText("AT+CMGF命令，初始化未完成");
						m_StartInitBtn.EnableWindow(FALSE);
						m_StopInitBtn.EnableWindow(TRUE);
						m_it_current++;
						DoTextMsgInit();
						StartTimer();
						break;
					case CSCA_waiting_OK:
						str.Format("发送命令--->AT+CSCA?\n接收命令--->%s",m_strRecvInfo);
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 3, str);
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 2, _T("未完成"));
						m_MsgInitInfoLet.SetWindowText("AT+CSCA?命令，初始化未完成");
						m_StartInitBtn.EnableWindow(FALSE);
						m_StopInitBtn.EnableWindow(TRUE);
						StartTimer();
						break;
					case CSCA_OK_finish:
						str = str + _T("\n接收命令--->OK");
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 3, str);
						m_GridCtrl.SetItemText(m_nCurrentRowIdxGrid-1, 2, _T("已完成"));
						m_MsgInitInfoLet.SetWindowText("初始化完成");

						m_it_current = pApp->m_TextMsgInitHash.begin();
						m_StartInitBtn.EnableWindow(TRUE);
						m_StopInitBtn.EnableWindow(FALSE);
						KillTimer(nIDEvent);
						m_pCurCommThreadInfo->GetScModule()->SetMsgInitFlag(false);
						break;
					}

				}
				
			}
		}
		break;
	default:
		break;
	}


	
	CDialog::OnTimer(nIDEvent);
}

void CRtuAttrMsgInit::OnBnClickedBtnInitstart()
{
	// TODO: 在此添加控件通知处理程序代码
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	CCommServer* pCommServer = NULL;
	pCommServer = pApp->m_pCommServer;
	if (pCommServer != NULL)
	{
		CScModuleBase *p = m_pCurCommThreadInfo->GetScModule();
		if (NULL == p)
		{
			m_MsgInitInfoLet.SetWindowText("故障：线程未启动或其他！");
			return;
		}
		p->SetMsgInitFlag(true);
		m_StartInitBtn.EnableWindow(false);

		m_MsgInitInfoLet.SetWindowText("升级准备中");
		DoTextMsgInit();
	}				
}



void CRtuAttrMsgInit::DoTextMsgInit()
{
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();

	if (m_it_current != pApp->m_TextMsgInitHash.end())
	{
		switch(m_it_current->first)
		{
		case AT:
			Do_AT_();
			break;
		case ATE0:
			Do_ATE0_();
			break;
		case AT_ADD_CGMR:
			Do_AT_ADD_CGMR_();
			break;
		case AT_ADD_CGSN:
			Do_AT_ADD_CGSN_();
			break;
		case AT_ADD_CIMI:
			Do_AT_ADD_CIMI_();
			break;
		case AT_ADD_CMEE:
			Do_AT_ADD_CMEE_();
			break;
		case AT_ADD_CPIN_ASK:
			Do_AT_ADD_CPIN_ASK_();
			break;
		case AT_ADD_CREG:
			Do_AT_ADD_CREG_();
			break;
		case AT_ADD_CREG_ASK:
			Do_AT_ADD_CREG_ASK_();
			break;
		case AT_ADD_CSQ:
			Do_AT_ADD_CSQ_();
			break;
		case AT_ADD_CGREG:
			Do_AT_ADD_CGREG_();
			break;
		case  AT_ADD_CGREG_ASK:
			Do_AT_ADD_CGREG_ASK_();
			break;
		case AT_ADD_CGATT_ASK:
			Do_AT_ADD_CGATT_ASK_();
			break;
		case AT_ADD_CLIP:
			Do_AT_ADD_CLIP_();
			break;
		case AT_ADD_CNMI:
			Do_AT_ADD_CNMI_();
			break;
		case AT_ADD_CMGF:
			Do_AT_ADD_CMGF_();
			break;
		case AT_ADD_CSCA_ASK:
			Do_AT_ADD_CSCA_ASK_();
			break;
		}

		
	}


}

void CRtuAttrMsgInit::Do_AT_()
{
	m_CurrentSendMsg = new TextMsg();
	m_CurrentSendMsg->m_buf[0] = 'A';
	m_CurrentSendMsg->m_buf[1] = 'T';
	m_CurrentSendMsg->m_len = 2;
	m_CurrentSendMsg->m_Type = AT;
	m_CurrentSendMsg->m_state = SENDING;
	SetCmd(AT_sending);
	AppendRowToGrid(m_CurrentSendMsg);
	StartTimer();
	SendData(m_CurrentSendMsg);

}

void CRtuAttrMsgInit::Do_ATE0_()
{
	m_CurrentSendMsg = new TextMsg();
	m_CurrentSendMsg->m_buf[0] = 'A';
	m_CurrentSendMsg->m_buf[1] = 'T';
	m_CurrentSendMsg->m_buf[2] = 'E';
	m_CurrentSendMsg->m_buf[3] = '0';
	m_CurrentSendMsg->m_len = 4;
	m_CurrentSendMsg->m_Type = ATE0;
	m_CurrentSendMsg->m_state = SENDING;
	SetCmd(ATE0_sending);
	AppendRowToGrid(m_CurrentSendMsg);
	StartTimer();
	SendData(m_CurrentSendMsg);
}

void CRtuAttrMsgInit::Do_AT_ADD_CGMR_()
{
	m_CurrentSendMsg = new TextMsg();
	m_CurrentSendMsg->m_buf[0] = 'A';
	m_CurrentSendMsg->m_buf[1] = 'T';
	m_CurrentSendMsg->m_buf[2] = '+';
	m_CurrentSendMsg->m_buf[3] = 'C';
	m_CurrentSendMsg->m_buf[4] = 'G';
	m_CurrentSendMsg->m_buf[5] = 'M';
	m_CurrentSendMsg->m_buf[6] = 'R';
	m_CurrentSendMsg->m_Type = AT_ADD_CGMR;
	m_CurrentSendMsg->m_len = 7;
	m_CurrentSendMsg->m_state = SENDING;
	SetCmd(CGMR_waiting_OK);
	AppendRowToGrid(m_CurrentSendMsg);
	StartTimer();
	SendData(m_CurrentSendMsg);
}

void CRtuAttrMsgInit::Do_AT_ADD_CGSN_()
{
	m_CurrentSendMsg = new TextMsg();
	m_CurrentSendMsg->m_buf[0] = 'A';
	m_CurrentSendMsg->m_buf[1] = 'T';
	m_CurrentSendMsg->m_buf[2] = '+';
	m_CurrentSendMsg->m_buf[3] = 'C';
	m_CurrentSendMsg->m_buf[4] = 'G';
	m_CurrentSendMsg->m_buf[5] = 'S';
	m_CurrentSendMsg->m_buf[6] = 'N';
	m_CurrentSendMsg->m_len = 7;
	m_CurrentSendMsg->m_Type = AT_ADD_CGSN;
	m_CurrentSendMsg->m_state = SENDING;
	SetCmd(CGSN_waiting_OK);
	AppendRowToGrid(m_CurrentSendMsg);
	StartTimer();
	SendData(m_CurrentSendMsg);
}

void CRtuAttrMsgInit::Do_AT_ADD_CIMI_()
{
	m_CurrentSendMsg = new TextMsg();
	m_CurrentSendMsg->m_buf[0] = 'A';
	m_CurrentSendMsg->m_buf[1] = 'T';
	m_CurrentSendMsg->m_buf[2] = '+';
	m_CurrentSendMsg->m_buf[3] = 'C';
	m_CurrentSendMsg->m_buf[4] = 'I';
	m_CurrentSendMsg->m_buf[5] = 'M';
	m_CurrentSendMsg->m_buf[6] = 'I';
	m_CurrentSendMsg->m_len = 7;
	m_CurrentSendMsg->m_Type = AT_ADD_CIMI;
	m_CurrentSendMsg->m_state = SENDING;
	SetCmd(CIMI_waiting_OK);
	AppendRowToGrid(m_CurrentSendMsg);
	StartTimer();
	SendData(m_CurrentSendMsg);
}

void CRtuAttrMsgInit::Do_AT_ADD_CMEE_()
{
	m_CurrentSendMsg = new TextMsg();
	m_CurrentSendMsg->m_buf[0] = 'A';
	m_CurrentSendMsg->m_buf[1] = 'T';
	m_CurrentSendMsg->m_buf[2] = '+';
	m_CurrentSendMsg->m_buf[3] = 'C';
	m_CurrentSendMsg->m_buf[4] = 'M';
	m_CurrentSendMsg->m_buf[5] = 'E';
	m_CurrentSendMsg->m_buf[6] = 'E';
	m_CurrentSendMsg->m_buf[7] = '=';
	m_CurrentSendMsg->m_buf[8] = '2';
	m_CurrentSendMsg->m_len = 9;
	m_CurrentSendMsg->m_Type = AT_ADD_CMEE;
	m_CurrentSendMsg->m_state = SENDING;
	SetCmd(CMEE_sending);
	AppendRowToGrid(m_CurrentSendMsg);
	StartTimer();
	SendData(m_CurrentSendMsg);
}

void CRtuAttrMsgInit::Do_AT_ADD_CPIN_ASK_()
{
	m_CurrentSendMsg = new TextMsg();
	m_CurrentSendMsg->m_buf[0] = 'A';
	m_CurrentSendMsg->m_buf[1] = 'T';
	m_CurrentSendMsg->m_buf[2] = '+';
	m_CurrentSendMsg->m_buf[3] = 'C';
	m_CurrentSendMsg->m_buf[4] = 'P';
	m_CurrentSendMsg->m_buf[5] = 'I';
	m_CurrentSendMsg->m_buf[6] = 'N';
	m_CurrentSendMsg->m_buf[7] = '?';
	m_CurrentSendMsg->m_len = 8;
	m_CurrentSendMsg->m_Type = AT_ADD_CPIN_ASK;
	m_CurrentSendMsg->m_state = SENDING;
	SetCmd(CPIN_waiting_OK);
	AppendRowToGrid(m_CurrentSendMsg);
	StartTimer();
	SendData(m_CurrentSendMsg);
}

void CRtuAttrMsgInit::Do_AT_ADD_CREG_()
{
	m_CurrentSendMsg = new TextMsg();
	m_CurrentSendMsg->m_buf[0] = 'A';
	m_CurrentSendMsg->m_buf[1] = 'T';
	m_CurrentSendMsg->m_buf[2] = '+';
	m_CurrentSendMsg->m_buf[3] = 'C';
	m_CurrentSendMsg->m_buf[4] = 'R';
	m_CurrentSendMsg->m_buf[5] = 'E';
	m_CurrentSendMsg->m_buf[6] = 'G';
	m_CurrentSendMsg->m_buf[7] = '=';
	m_CurrentSendMsg->m_buf[8] = '1';
	m_CurrentSendMsg->m_len = 9;
	m_CurrentSendMsg->m_Type = AT_ADD_CREG;
	m_CurrentSendMsg->m_state = SENDING;
	SetCmd(CREG_waiting_OK);
	AppendRowToGrid(m_CurrentSendMsg);
	StartTimer();
	SendData(m_CurrentSendMsg);
}

void CRtuAttrMsgInit::Do_AT_ADD_CREG_ASK_()
{
	TextMsg *m_CurrentSendMsg = NULL;

	m_CurrentSendMsg = new TextMsg();
	m_CurrentSendMsg->m_buf[0] = 'A';
	m_CurrentSendMsg->m_buf[1] = 'T';
	m_CurrentSendMsg->m_buf[2] = '+';
	m_CurrentSendMsg->m_buf[3] = 'C';
	m_CurrentSendMsg->m_buf[4] = 'R';
	m_CurrentSendMsg->m_buf[5] = 'E';
	m_CurrentSendMsg->m_buf[6] = 'G';
	m_CurrentSendMsg->m_buf[7] = '?';
	m_CurrentSendMsg->m_len = 8;
	m_CurrentSendMsg->m_Type = AT_ADD_CREG_ASK;
	m_CurrentSendMsg->m_state = SENDING;
	SetCmd(CGEGa_Sending);
	AppendRowToGrid(m_CurrentSendMsg);
	StartTimer();
	SendData(m_CurrentSendMsg);
}


void CRtuAttrMsgInit::Do_AT_ADD_CSQ_()
{
	TextMsg *m_CurrentSendMsg = NULL;

	m_CurrentSendMsg = new TextMsg();
	m_CurrentSendMsg->m_buf[0] = 'A';
	m_CurrentSendMsg->m_buf[1] = 'T';
	m_CurrentSendMsg->m_buf[2] = '+';
	m_CurrentSendMsg->m_buf[3] = 'C';
	m_CurrentSendMsg->m_buf[4] = 'S';
	m_CurrentSendMsg->m_buf[5] = 'Q';
	m_CurrentSendMsg->m_len = 6;
	m_CurrentSendMsg->m_Type = AT_ADD_CSQ;
	m_CurrentSendMsg->m_state = SENDING;
	SetCmd(CSQ_waiting_OK);
	AppendRowToGrid(m_CurrentSendMsg);
	StartTimer();
	SendData(m_CurrentSendMsg);
}

void CRtuAttrMsgInit::Do_AT_ADD_CGREG_()
{
	TextMsg *m_CurrentSendMsg = NULL;

	m_CurrentSendMsg = new TextMsg();
	m_CurrentSendMsg->m_buf[0] = 'A';
	m_CurrentSendMsg->m_buf[1] = 'T';
	m_CurrentSendMsg->m_buf[2] = '+';
	m_CurrentSendMsg->m_buf[3] = 'C';
	m_CurrentSendMsg->m_buf[4] = 'G';
	m_CurrentSendMsg->m_buf[5] = 'R';
	m_CurrentSendMsg->m_buf[6] = 'E';
	m_CurrentSendMsg->m_buf[7] = 'G';
	m_CurrentSendMsg->m_buf[8] = '=';
	m_CurrentSendMsg->m_buf[9] = '1';
	m_CurrentSendMsg->m_len = 10;
	m_CurrentSendMsg->m_Type = AT_ADD_CGREG;
	m_CurrentSendMsg->m_state = SENDING;
	SetCmd(CGREG_OK_finish);
	AppendRowToGrid(m_CurrentSendMsg);
	StartTimer();
	SendData(m_CurrentSendMsg);
}

void CRtuAttrMsgInit::Do_AT_ADD_CGREG_ASK_()
{
	TextMsg *m_CurrentSendMsg = NULL;

	m_CurrentSendMsg = new TextMsg();
	m_CurrentSendMsg->m_buf[0] = 'A';
	m_CurrentSendMsg->m_buf[1] = 'T';
	m_CurrentSendMsg->m_buf[2] = '+';
	m_CurrentSendMsg->m_buf[3] = 'C';
	m_CurrentSendMsg->m_buf[4] = 'G';
	m_CurrentSendMsg->m_buf[5] = 'R';
	m_CurrentSendMsg->m_buf[6] = 'E';
	m_CurrentSendMsg->m_buf[7] = 'G';
	m_CurrentSendMsg->m_buf[8] = '?';
	m_CurrentSendMsg->m_len = 9;
	m_CurrentSendMsg->m_Type = AT_ADD_CGREG_ASK;
	m_CurrentSendMsg->m_state = SENDING;
	SetCmd(CGREGa_sending);
	AppendRowToGrid(m_CurrentSendMsg);
	StartTimer();
	SendData(m_CurrentSendMsg);

// 	if (m_pSubStation != NULL)
// 	{
// 		m_pSubStation->GetSysDataTrans()->SendRtuLinkFrame(m_CurrentSendMsg->m_buf, m_CurrentSendMsg->m_len, Serial_CommInterface);
// 	}
}

void CRtuAttrMsgInit::Do_AT_ADD_CGATT_ASK_()
{
	TextMsg *m_CurrentSendMsg = NULL;

	m_CurrentSendMsg = new TextMsg();
	m_CurrentSendMsg->m_buf[0] = 'A';
	m_CurrentSendMsg->m_buf[1] = 'T';
	m_CurrentSendMsg->m_buf[2] = '+';
	m_CurrentSendMsg->m_buf[3] = 'C';
	m_CurrentSendMsg->m_buf[4] = 'G';
	m_CurrentSendMsg->m_buf[5] = 'A';
	m_CurrentSendMsg->m_buf[6] = 'T';
	m_CurrentSendMsg->m_buf[7] = 'T';
	m_CurrentSendMsg->m_buf[8] = '?';
	m_CurrentSendMsg->m_len = 9;
	m_CurrentSendMsg->m_Type = AT_ADD_CGATT_ASK;
	m_CurrentSendMsg->m_state = SENDING;
	SetCmd(CGATT_waiting_OK);
	AppendRowToGrid(m_CurrentSendMsg);
	StartTimer();
	SendData(m_CurrentSendMsg);
}

void CRtuAttrMsgInit::Do_AT_ADD_CLIP_()
{
	TextMsg *m_CurrentSendMsg = NULL;

	m_CurrentSendMsg = new TextMsg();
	m_CurrentSendMsg->m_buf[0] = 'A';
	m_CurrentSendMsg->m_buf[1] = 'T';
	m_CurrentSendMsg->m_buf[2] = '+';
	m_CurrentSendMsg->m_buf[3] = 'C';
	m_CurrentSendMsg->m_buf[4] = 'L';
	m_CurrentSendMsg->m_buf[5] = 'I';
	m_CurrentSendMsg->m_buf[6] = 'P';
	m_CurrentSendMsg->m_buf[7] = '=';
	m_CurrentSendMsg->m_buf[8] = '1';
	m_CurrentSendMsg->m_len = 9;
	m_CurrentSendMsg->m_Type = AT_ADD_CLIP;
	m_CurrentSendMsg->m_state = SENDING;
	SetCmd(CLIP_sending);
	AppendRowToGrid(m_CurrentSendMsg);
	StartTimer();
	SendData(m_CurrentSendMsg);
}

void CRtuAttrMsgInit::Do_AT_ADD_CNMI_()
{
	TextMsg *m_CurrentSendMsg = NULL;

	m_CurrentSendMsg = new TextMsg();
	m_CurrentSendMsg->m_buf[0] = 'A';
	m_CurrentSendMsg->m_buf[1] = 'T';
	m_CurrentSendMsg->m_buf[2] = '+';
	m_CurrentSendMsg->m_buf[3] = 'C';
	m_CurrentSendMsg->m_buf[4] = 'N';
	m_CurrentSendMsg->m_buf[5] = 'M';
	m_CurrentSendMsg->m_buf[6] = 'I';
	m_CurrentSendMsg->m_buf[7] = '=';
	m_CurrentSendMsg->m_buf[8] = '1';
	m_CurrentSendMsg->m_buf[9] = ',';
	m_CurrentSendMsg->m_buf[10] = '1';
	m_CurrentSendMsg->m_buf[11] = ',';
	m_CurrentSendMsg->m_buf[12] = '0';
	m_CurrentSendMsg->m_buf[13] = ',';
	m_CurrentSendMsg->m_buf[14] = '0';
	m_CurrentSendMsg->m_buf[15] = ',';
	m_CurrentSendMsg->m_buf[16] = '0';
	m_CurrentSendMsg->m_len = 17;
	m_CurrentSendMsg->m_Type = AT_ADD_CNMI;
	m_CurrentSendMsg->m_state = SENDING;
	SetCmd(CNMI_sending);
	AppendRowToGrid(m_CurrentSendMsg);
	StartTimer();
	SendData(m_CurrentSendMsg);
}

void CRtuAttrMsgInit::Do_AT_ADD_CMGF_()
{
	TextMsg *m_CurrentSendMsg = NULL;

	m_CurrentSendMsg = new TextMsg();
	m_CurrentSendMsg->m_buf[0] = 'A';
	m_CurrentSendMsg->m_buf[1] = 'T';
	m_CurrentSendMsg->m_buf[2] = '+';
	m_CurrentSendMsg->m_buf[3] = 'C';
	m_CurrentSendMsg->m_buf[4] = 'M';
	m_CurrentSendMsg->m_buf[5] = 'G';
	m_CurrentSendMsg->m_buf[6] = 'F';
	m_CurrentSendMsg->m_buf[7] = '=';
	m_CurrentSendMsg->m_buf[8] = '1';
	m_CurrentSendMsg->m_len = 9;
	m_CurrentSendMsg->m_Type = AT_ADD_CMGF;
	m_CurrentSendMsg->m_state = SENDING;
	SetCmd(CMGF_sending);
	AppendRowToGrid(m_CurrentSendMsg);
	StartTimer();
	SendData(m_CurrentSendMsg);
}

void CRtuAttrMsgInit::Do_AT_ADD_CSCA_ASK_()
{
	TextMsg *m_CurrentSendMsg = NULL;

	m_CurrentSendMsg = new TextMsg();
	m_CurrentSendMsg->m_buf[0] = 'A';
	m_CurrentSendMsg->m_buf[1] = 'T';
	m_CurrentSendMsg->m_buf[2] = '+';
	m_CurrentSendMsg->m_buf[3] = 'C';
	m_CurrentSendMsg->m_buf[4] = 'S';
	m_CurrentSendMsg->m_buf[5] = 'C';
	m_CurrentSendMsg->m_buf[6] = 'A';
	m_CurrentSendMsg->m_buf[7] = '?';
	m_CurrentSendMsg->m_len = 8;
	m_CurrentSendMsg->m_Type = AT_ADD_CSCA_ASK;
	m_CurrentSendMsg->m_state = SENDING;
	SetCmd(CSCA_waiting_OK);
	AppendRowToGrid(m_CurrentSendMsg);
	StartTimer();
	SendData(m_CurrentSendMsg);
}


void CRtuAttrMsgInit::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void CRtuAttrMsgInit::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void CRtuAttrMsgInit::OnBnClickedBtnInitstop()
{
	// TODO: 在此添加控件通知处理程序代码
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	CCommServer* pCommServer = NULL;
	pCommServer = pApp->m_pCommServer;
	if (pCommServer != NULL)
	{
		m_pCurCommThreadInfo->GetScModule()->SetMsgInitFlag(false);
		m_StartInitBtn.EnableWindow(TRUE);


	}			

}
