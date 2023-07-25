// RtuAttrTextMsg.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuAttrTextMsg.h"
#include "RtuStation.h"
#include "ScModuleBase.h"


// CRtuAttrTextMsg 对话框
#define WM_TIMER_TM_INIT_START  0x01//短信初始化

#define WM_TIMER_CPMS  0x05
#define WM_TIMER_CPMSa 0x06
#define WM_TIMER_CMGF  0x07
#define WM_TIMER_CSMP  0x08
#define WM_TIMER_CMGL  0x09
#define WM_TIMER_CMGR  0x10
#define WM_TIMER_CMGD  0x11

const int msg_invalid = -1;
const int msg_seccess = 0;
const int msg_fail = 1;

IMPLEMENT_DYNAMIC(CRtuAttrTextMsg, CDialog)

CRtuAttrTextMsg::CRtuAttrTextMsg(CWnd* pParent /*=NULL*/)
	: CDialog(CRtuAttrTextMsg::IDD, pParent)
{
	m_pSubStation = NULL;
	m_nCurrentCmd = Invaild_SendCmd;
	m_blResult = FALSE;
	m_pTmUnit = NULL;
	m_nCurrentSetIdx = -1;//设置的idx是小于0的
	m_nCurrentSendIdx = -1;
	m_pReadMsg = NULL;

}

CRtuAttrTextMsg::~CRtuAttrTextMsg()
{
}

void CRtuAttrTextMsg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LET_TMDETAIL, m_InitTmDetail);
	DDX_Control(pDX, IDC_CBX_CPMS, m_cpmsCbx);
	DDX_Control(pDX, IDC_CBX_CPMSA, m_cpmsaCbx);
	DDX_Control(pDX, IDC_CBX_CMGF, m_cmgfCbx);
	DDX_Control(pDX, IDC_CBX_CSMP, m_csmpCbx);

}


BEGIN_MESSAGE_MAP(CRtuAttrTextMsg, CDialog)
	ON_WM_TIMER()
	ON_NOTIFY(NM_RCLICK, IDC_SMS_GRID, &CRtuAttrTextMsg::OnRClickGridCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_SMS_GRID, &CRtuAttrTextMsg::OnDLClickGridCtrl)
	ON_COMMAND(IDM_OP_CLEAR_SMS, &CRtuAttrTextMsg::OnDeleteSMS)

	ON_BN_CLICKED(IDC_BTN_CPMS, &CRtuAttrTextMsg::OnBnClickedBtnCpms)
	ON_BN_CLICKED(IDC_BTN_CPMSa, &CRtuAttrTextMsg::OnBnClickedBtnCpmsa)
	ON_BN_CLICKED(IDC_BTN_CMGF, &CRtuAttrTextMsg::OnBnClickedBtnCmgf)
	ON_BN_CLICKED(IDC_BTN_CSMP, &CRtuAttrTextMsg::OnBnClickedBtnCsmp)
END_MESSAGE_MAP()

BOOL CRtuAttrTextMsg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CWnd* pWnd = GetDlgItem(IDC_STATIC_TMLIST);

	CRect rect1, rect2;
	int captionHeight = 0; 
	int cxframe = GetSystemMetrics(SM_CXFRAME);
	int cyframe = GetSystemMetrics(SM_CYFRAME);
	this->GetWindowRect(&rect2);

	if (pWnd)
		pWnd->GetWindowRect(&rect1);
	if (!m_wndTmListGrid.Create(CRect(rect1.left - rect2.left - cxframe, 
		rect1.top-rect2.top - cyframe-captionHeight, 
		rect1.left + rect1.Width() - rect2.left, 
		rect1.top + rect1.Height() - rect2.top - captionHeight), 
		this, IDC_SMS_GRID,WS_CHILD|WS_VISIBLE))
	{
		TRACE("Failed to create threads  grid!\n");
		return FALSE;
	}

	m_wndTmListGrid.SetBkColor(RGB(255,255,255));
	m_wndTmListGrid.SetListMode(TRUE);
	m_wndTmListGrid.EnableTitleTips(FALSE);
	m_wndTmListGrid.SetSingleColSelection(TRUE);
	m_wndTmListGrid.SetSingleRowSelection(TRUE);
	m_wndTmListGrid.SetEditable(FALSE);

	InitTmList();

	return TRUE;
	
}

void CRtuAttrTextMsg::SetParentDlg(CRtuPropertyDlg* pDlg)
{
	m_pParentDlg = pDlg;
}

void CRtuAttrTextMsg::Update()										//更新界面的显示信息
{
	if (m_pParentDlg)
		m_pSubStation = m_pParentDlg->GetRtuStation();

	if (m_wndTmListGrid.GetSafeHwnd())
	{
		m_wndTmListGrid.DeleteAllItems();
		m_wndTmListGrid.Refresh();
	}

	if (m_pSubStation != NULL)
	{
		InitTmList();

	}

}

void CRtuAttrTextMsg::UpdateDlg(int attrType,BOOL blResult,TM_Unit *pUnit)			//接收到报文后、更新界面
{
	m_blResult = blResult;
	m_pTmUnit = pUnit;

	switch (attrType)
	{
	case CPMS_waiting_OK:
		SendMessage(WM_TIMER, WM_TIMER_CPMS, NULL);
		break;
	case CPMSa_waiting_OK:
		SendMessage(WM_TIMER, WM_TIMER_CPMSa, NULL);
		break;
	case CMGL_waiting_OK:
		SendMessage(WM_TIMER, WM_TIMER_CMGL, NULL);
		break;
	case CMGR_waiting_OK:
		SendMessage(WM_TIMER, WM_TIMER_CMGR, NULL);
		break;
	case CMGF_OK_finish:
		SendMessage(WM_TIMER, WM_TIMER_CMGF, NULL);
		break;
	case CSMP_OK_finish:
		SendMessage(WM_TIMER, WM_TIMER_CSMP, NULL);
		break;
	case CMGD_OK_finish:
		SendMessage(WM_TIMER, WM_TIMER_CMGD, NULL);
		break;

	}

}



void CRtuAttrTextMsg::RecvSmsHint(int attrType, int No)
{
	if (attrType == Recv_CMTI)
	{
		ReadMsg(No);
	}

}

void CRtuAttrTextMsg::KillAllTimers()
{

}

void CRtuAttrTextMsg::SetWndsEnable(BOOL blEnable)				//设置按钮是否可用
{

}

void CRtuAttrTextMsg::InitTmList()
{//GridCtrl显示
	UpdateGrid();
}

void CRtuAttrTextMsg::AppendRowToGrid()
{
	if (m_wndTmListGrid.GetSafeHwnd())
	{
		if (m_pSubStation)
		{
			m_pSubStation->m_cs_TmUnit.Lock();
			int nCnt = m_pSubStation->GetTmUnitList()->size();
			m_pSubStation->m_cs_TmUnit.Unlock();

			TM_Unit *unit = NULL;
			unit = m_pSubStation->GetTmUnitList()->back();
			int nGridRowCnt = m_wndTmListGrid.GetRowCount();

			if (unit != NULL)
			{
				if (nGridRowCnt <= 0)
				{
					return ;
				}

				CString str_Idx = "";
				str_Idx.Format("%d", unit->GetIndex());
				nGridRowCnt = m_wndTmListGrid.InsertRow(str_Idx);

				unit->SetRow(nGridRowCnt);
				ShowDetailGridItem(nGridRowCnt, unit);
				m_wndTmListGrid.Refresh();

			}
		}
	}
}

void CRtuAttrTextMsg::ShowDetailGridItem(int nRowIdx, TM_Unit *pUnit)
{
	if (nRowIdx <=0) return;
	if (pUnit == NULL) return;

	CString strRowIdx = "";
	CString strTemp = "";
	strRowIdx.Format(_T("%d"), pUnit->GetIndex());

	GV_ITEM rowItem;
	rowItem.row = nRowIdx;
	rowItem.nFormat =  DT_TOP|DT_LEFT|DT_WORDBREAK;
	rowItem.mask = GVIF_TEXT|GVIF_PARAM|GVIF_BKCLR|GVIF_FGCLR;
	rowItem.col = 0;
	rowItem.lParam = (LPARAM)pUnit;
	rowItem.strText = strRowIdx;
	m_wndTmListGrid.SetItem(&rowItem);

	rowItem.col = 1;
	rowItem.mask = GVIF_TEXT|GVIF_BKCLR|GVIF_FGCLR;
	strTemp = _T("");

	if (pUnit->isRead())
	{
		strTemp = _T("已读");
	}
	else
	{
		strTemp = _T("未读");
	}
	rowItem.strText = strTemp;
	m_wndTmListGrid.SetItem(&rowItem);

	rowItem.col = 2;
	rowItem.mask = GVIF_TEXT|GVIF_BKCLR|GVIF_FGCLR;
	rowItem.strText = pUnit->GetRecvTime();
	m_wndTmListGrid.SetItem(&rowItem);

	rowItem.col = 3;
	rowItem.mask = GVIF_TEXT|GVIF_BKCLR|GVIF_FGCLR;

	strTemp = _T("");
	strTemp.Format("%s %s", pUnit->GetPhoneNum(), pUnit->GetContent());
	rowItem.strText = strTemp;
	m_wndTmListGrid.SetItem(&rowItem);

}

void CRtuAttrTextMsg::UpdateGrid()
{
	if (m_wndTmListGrid.GetSafeHwnd())
	{
		CString strHearders[] = {
			"序号",
			"读取标记",
			"时标",
			"缩略"
		};

		if (m_pSubStation!=NULL)
		{
			m_pSubStation->m_cs_TmUnit.Lock();
			int nCnt = m_pSubStation->GetTmUnitList()->size();
			m_pSubStation->m_cs_TmUnit.Unlock();

			m_wndTmListGrid.DeleteAllItems();
			m_wndTmListGrid.SetRowCount(nCnt+1);
			m_wndTmListGrid.SetFixedRowCount(1);


			m_wndTmListGrid.InsertColumn(strHearders[0],DT_LEFT|DT_VCENTER|DT_SINGLELINE,0);
			m_wndTmListGrid.InsertColumn(strHearders[1],DT_LEFT|DT_VCENTER|DT_SINGLELINE,1);
			m_wndTmListGrid.InsertColumn(strHearders[2],DT_LEFT|DT_VCENTER|DT_SINGLELINE,2);
			m_wndTmListGrid.InsertColumn(strHearders[3],DT_LEFT|DT_VCENTER|DT_SINGLELINE,3);

			m_wndTmListGrid.SetColumnWidth(0,40);
			m_wndTmListGrid.SetColumnWidth(1,60);
			m_wndTmListGrid.SetColumnWidth(2,100);
			m_wndTmListGrid.SetColumnWidth(3,200);
		
			std::list<TM_Unit*>::iterator it;
			std::list<TM_Unit*>::iterator it_b;
			std::list<TM_Unit*>::iterator it_e;

			m_pSubStation->m_cs_TmUnit.Lock();
			it_b = m_pSubStation->GetTmUnitList()->begin();
			it_e = m_pSubStation->GetTmUnitList()->end();

			TM_Unit *pUnit = NULL;
			int nRowIdx = 1;

			for (it=it_b; it!=it_e; it++)
			{
				pUnit = (TM_Unit *)(*it);
				if (pUnit == NULL)
				{
					continue;
				}

				nRowIdx = m_wndTmListGrid.InsertRow("");
				pUnit->SetRow(nRowIdx);
				ShowDetailGridItem(nRowIdx, pUnit);

			}
			m_pSubStation->m_cs_TmUnit.Unlock();
			m_wndTmListGrid.Refresh();

			

		}
		else
		{
			m_wndTmListGrid.SetRowCount(1);
			m_wndTmListGrid.SetFixedRowCount(1);

			m_wndTmListGrid.InsertColumn(strHearders[0],DT_LEFT|DT_VCENTER|DT_SINGLELINE,0);
			m_wndTmListGrid.InsertColumn(strHearders[1],DT_LEFT|DT_VCENTER|DT_SINGLELINE,1);
			m_wndTmListGrid.InsertColumn(strHearders[2],DT_LEFT|DT_VCENTER|DT_SINGLELINE,2);
			m_wndTmListGrid.InsertColumn(strHearders[3],DT_LEFT|DT_VCENTER|DT_SINGLELINE,3);

			m_wndTmListGrid.SetColumnWidth(0,40);
			m_wndTmListGrid.SetColumnWidth(1,60);
			m_wndTmListGrid.SetColumnWidth(2,100);
			m_wndTmListGrid.SetColumnWidth(3,200);

			m_wndTmListGrid.Refresh();
		}
	}
}


void CRtuAttrTextMsg::OnCellReadOnly(int Row,int Col)
{
	if (m_wndTmListGrid.GetSafeHwnd())
	{
		m_wndTmListGrid.SetItemState(Row,Col, m_wndTmListGrid.GetItemState(Row,Col) | GVIS_READONLY);
	}
}


void CRtuAttrTextMsg::SetCommThreadInfo(CCommThreadInfo* pThreadInfo)
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

void CRtuAttrTextMsg::ReadMsg(int No)
{
	if (m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{
			CSmsFrameFormat frame;
			frame.m_setVar.Format("%d", No);

			m_pSubStation->BuildBasicPacket(frame);
			m_pSubStation->GetSysDataTrans()->IEC_textMsg_cmgr(&frame);
			SetTimer(WM_TIMER_CMGR, 30000,  NULL);
		}
	}	
}

void CRtuAttrTextMsg::SetCmd(int cmd)
{
	m_nCurrentCmd = cmd;
}

int CRtuAttrTextMsg::GetCmd()
{
	return m_nCurrentCmd;
}

void CRtuAttrTextMsg::SendData(TextMsg *m_CurrentSendMsg)
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
			pGht->SetMsgSendDlg(this);
		}
		break;
	case SCTextMsgType_YT:
		break;
	default:
		break;
	}
}






void CRtuAttrTextMsg::OnTimer(UINT_PTR nIDEvent)
{
	CString strLogMessge = "";
	_Log_level_type _logLevle = Normal_Log_Message;

 	switch (nIDEvent)
 	{
	case  WM_TIMER_CPMS:
		{
			if (m_pSubStation == NULL)	return;
			if (m_pTmUnit == NULL) break;

			if (m_blResult==TRUE)
			{
				m_pTmUnit->SetIndex(m_nCurrentSetIdx);
				m_pTmUnit->SetState(msg_seccess);
				m_pSubStation->GetTmUnitList()->push_back(m_pTmUnit);
				m_nCurrentSetIdx--;
				AppendRowToGrid();
			}
			else
			{
			}
 		}
 		break;
	case WM_TIMER_CPMSa:
		{
			if (m_pSubStation==NULL)
			{
				return;
			}

			if (m_pTmUnit == NULL)
			{
				break;
			}

			m_pTmUnit->SetIndex(m_nCurrentSetIdx);
			m_pSubStation->GetTmUnitList()->push_back(m_pTmUnit);
			m_nCurrentSetIdx--;
			AppendRowToGrid();

		}
		break;
	case WM_TIMER_CMGL:
	case WM_TIMER_CMGR:
		{
			if (m_pSubStation == NULL)
			{
				return;
			}

			if (m_pTmUnit == NULL)
			{
				break;
			}

			m_pTmUnit->SetIndex(m_nCurrentSendIdx);
			m_pSubStation->GetTmUnitList()->push_back(m_pTmUnit);
			m_nCurrentSendIdx++;
			AppendRowToGrid();
		}
		break;
	case WM_TIMER_CMGF:
		{
			strLogMessge = _T("设置文本模式成功！");
		}
		break;
	case WM_TIMER_CMGD:
		{
			strLogMessge = _T("短信删除成功！");
		}
		break;
	case WM_TIMER_CSMP:
		{
			strLogMessge = _T("数据的编码方式设置成功！");
		}
		break;
 	default:
 		break;
 	}


	KillTimer(nIDEvent);
	SetWndsEnable(TRUE);

	if (m_pParentDlg && m_pParentDlg->GetSafeHwnd())
		m_pParentDlg->AddLog(strLogMessge, NULL, _logLevle);

	if (m_pSubStation)
		m_pSubStation->InsertLogMessage(strLogMessge, _logLevle, NULL);

	CDialog::OnTimer(nIDEvent);
}



void CRtuAttrTextMsg::OnBnClickedBtnCpms()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{
			CSmsFrameFormat frame;
			int nsel = m_cpmsCbx.GetCurSel();
			if (CB_ERR == nsel)
			{
				MessageBox("请选择合适的存储器","提示",MB_OK|MB_ICONERROR);
				return;
			}
			
			CString curText = "";
			m_cpmsCbx.GetWindowText(curText);
			frame.m_setVar = curText;

			SetWndsEnable(FALSE);
			m_blResult = FALSE;
			m_nCurrentCmd = CPMS_sending;

			m_pSubStation->BuildBasicPacket(frame);
			m_pSubStation->GetSysDataTrans()->IEC_textMsg_cpms(&frame);
			SetTimer(WM_TIMER_CPMS, 30000,  NULL);
		}
	}
}

void CRtuAttrTextMsg::OnBnClickedBtnCpmsa()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{
			CSmsFrameFormat frame;

			SetWndsEnable(FALSE);
			m_blResult = FALSE;
			m_nCurrentCmd = CPMSa_sending;

			m_pSubStation->BuildBasicPacket(frame);
			m_pSubStation->GetSysDataTrans()->IEC_textMsg_cpmsa(&frame);
			SetTimer(WM_TIMER_CPMSa, 30000,  NULL);
		}
	}
}

void CRtuAttrTextMsg::OnBnClickedBtnCmgf()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pSubStation->blEnbaleTrans() == true)
	{
		CSmsFrameFormat frame;
		int nsel = m_cmgfCbx.GetCurSel();
		if (CB_ERR == nsel)
		{
			MessageBox("请选择合适的存储器","提示",MB_OK|MB_ICONERROR);
			return;
		}

		CString curText = "";
		m_cmgfCbx.GetWindowText(curText);
		frame.m_setVar = curText;

		SetWndsEnable(FALSE);
		m_blResult = FALSE;
		m_nCurrentCmd = CMGF_sending;
		m_pSubStation->BuildBasicPacket(frame);
		m_pSubStation->GetSysDataTrans()->IEC_textMsg_cmgf(&frame);
		SetTimer(WM_TIMER_CMGF, 30000,  NULL);
	}
}

void CRtuAttrTextMsg::OnBnClickedBtnCsmp()
{
	// TODO: 在此添加控件通知处理程序代码

	CSmsFrameFormat frame;

	SetWndsEnable(FALSE);
	m_blResult = FALSE;
	m_nCurrentCmd = CSMP_sending;

	m_pSubStation->BuildBasicPacket(frame);
	m_pSubStation->GetSysDataTrans()->IEC_textMsg_csmp(&frame);
	SetTimer(WM_TIMER_CSMP, 30000,  NULL);
}


void CRtuAttrTextMsg::OnRClickGridCtrl(NMHDR *pNotifyStruct,LRESULT *pResult)
{
	NM_GRIDVIEW* pItem =(NM_GRIDVIEW*)pNotifyStruct;

	if (pItem && pItem->iRow <=0)
		return;

	m_selTmUnit = 0;
	if (pItem && pItem->iRow > 0)
	{
		m_wndTmListGrid.SetSelectedRange(-1,-1,-1,-1);  //清空选择焦点
	}

	m_wndTmListGrid.SetSelectedRange(pItem->iRow,0,pItem->iRow,(int)m_wndTmListGrid.GetColumnCount()-1);

	TM_Unit* ItemData = (TM_Unit*)m_wndTmListGrid.GetItemData(pItem->iRow,0);
	if (ItemData == NULL)
		return ;

	m_selTmUnit = ItemData;

	CMenu menu;
	CMenu* popMenu = NULL;
	CPoint cursorPos;

	GetCursorPos(&cursorPos);
	menu.LoadMenu(IDR_MENU_POP);
	popMenu  = menu.GetSubMenu(11);

	if (popMenu && popMenu->GetSafeHmenu())
		popMenu->TrackPopupMenu(TPM_LEFTALIGN,cursorPos.x,cursorPos.y,this);
}


void CRtuAttrTextMsg::OnDLClickGridCtrl(NMHDR *pNotifyStruct,LRESULT *pResult)
{
	NM_GRIDVIEW* pItem =(NM_GRIDVIEW*)pNotifyStruct;

	if (pItem && pItem->iRow <=0)
		return;

	m_selTmUnit = 0;
	if (pItem && pItem->iRow > 0)
	{
		m_wndTmListGrid.SetSelectedRange(-1,-1,-1,-1);  //清空选择焦点
	}

	m_wndTmListGrid.SetSelectedRange(pItem->iRow,0,pItem->iRow,(int)m_wndTmListGrid.GetColumnCount()-1);

	TM_Unit* ItemData = (TM_Unit*)m_wndTmListGrid.GetItemData(pItem->iRow,0);
	if (ItemData == NULL)
		return ;

	m_selTmUnit = ItemData;
	m_InitTmDetail.SetWindowText(ItemData->GetContent());

}
void CRtuAttrTextMsg::OnDeleteSMS()
{
	if (m_selTmUnit != NULL && m_pSubStation != NULL)
	{
		if (m_pSubStation->blEnbaleTrans() == false)
		{
			return;
		}

		if (m_wndTmListGrid.GetSafeHwnd())
		{
			int idx = m_selTmUnit->GetIndex();

			std::list<TM_Unit*>::iterator it;
			it = m_pSubStation->GetTmUnitList()->begin();
			TM_Unit *temp = NULL;
			while (it != m_pSubStation->GetTmUnitList()->end())
			{
				temp = (TM_Unit*)(*it);
				if (temp == NULL)
				{
					continue;
				}

				if (temp->GetIndex() == m_selTmUnit->GetIndex())
				{
					m_wndTmListGrid.DeleteRow(m_selTmUnit->GetRow());
					m_pSubStation->GetTmUnitList()->erase(it);
					delete temp;
					temp = 0;
					m_selTmUnit = 0;

					{
						CSmsFrameFormat frame;
						m_blResult = FALSE;
						m_nCurrentCmd = CMGD_sending;

						m_pSubStation->BuildBasicPacket(frame);
						m_pSubStation->GetSysDataTrans()->IEC_textMsg_cmgd(&frame);
						SetTimer(WM_TIMER_CMGD, 30000,  NULL);

					}

					UpdateGrid();
				}
			}
			
		}
	}

}
