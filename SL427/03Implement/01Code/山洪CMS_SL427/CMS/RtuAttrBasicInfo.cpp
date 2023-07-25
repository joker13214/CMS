// RtuAttrBasicInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuAttrBasicInfo.h"
#include "GridCtrl/GridCtrl.h"
#include "RtuPropertyDlg.h"

#define  W_TIMER_CONNCHECK		0x01   //链路检查Timer
#define  W_TIMER_DATETIMECALL	0x02   //时钟查询Timer
#define  W_TIMER_WORKMODE_CALL  0x03   //工作模式查询Timer
#define  W_TIMER_WORKMODE_SET   0x04   //工作模式设置Timer

// CRtuAttrBasicInfo 对话框
IMPLEMENT_DYNAMIC(CRtuAttrBasicInfo, CDialog)

CRtuAttrBasicInfo::CRtuAttrBasicInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CRtuAttrBasicInfo::IDD, pParent)
	, m_strName(_T(""))
	, m_strPronvince(_T(""))
	, m_strRegion(_T(""))
	, m_strCounty(_T(""))
	, m_strA2(_T(""))
	//, m_strCommType(_T(""))
	//, m_strCommInterface(_T(""))
	,m_strBaCommThreadInfo(_T(""))
	, m_strDt(_T(""))
	, m_nDelayDZ(0)
{
	m_pParentDlg = NULL;
	m_pSubStation = NULL;
}

CRtuAttrBasicInfo::~CRtuAttrBasicInfo()
{
}

void CRtuAttrBasicInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BA_IN_INFO, m_wndGridParent);
	DDX_Text(pDX, IDC_BA_NAME, m_strName);
	DDX_Text(pDX, IDC_BA_PRVIN, m_strPronvince);
	DDX_Text(pDX, IDC_BA_REGION, m_strRegion);
	DDX_Text(pDX, IDC_BA_COUNTY, m_strCounty);
	DDX_Text(pDX, IDC_BA_A2, m_strA2);
	//DDX_Text(pDX, IDC_BA_CMTYPE, m_strCommType);
	//DDX_Text(pDX, IDC_BA_INTERFACE, m_strCommInterface);
	DDX_Control(pDX, IDC_COMBO1, m_wndCmbConnType);
	DDX_Control(pDX, IDC_CONN_CHECK, m_btnConnCheck);
	DDX_Control(pDX, IDC_COMBO2, m_wndCmbThreads);
	DDX_Control(pDX, IDC_BTN_SELTHREAD, m_btnSelThread);
	DDX_Text(pDX, IDC_BA_THDINFTEXT, m_strBaCommThreadInfo);
	DDX_Control(pDX, IDC_BASIC_DTCALL, m_btnDtCall);
	DDX_Control(pDX, IDC_BASIC_DTSET, m_btnDtSet);
	DDX_Control(pDX,IDC_BUTTON_SETDELAY,m_btnSetDelay);
	DDX_Text(pDX, IDC_BASIC_DT, m_strDt);

	DDX_Control(pDX, IDC_CMB_WORKMODE, m_wndCmbWorkMode);
	DDX_Control(pDX, IDC_WORKMODE_CALL,m_btnWorkModeCall);
	DDX_Control(pDX, IDC_WORKMODE_SET, m_btnWorkModeSet);
	DDX_Control(pDX, IDC_BA_RECVTIME, m_EditRecvTime);
	DDX_Control(pDX, IDC_BA_CMTYPE, m_wndCmTypeEdit);
	DDX_Control(pDX, IDC_BA_INTERFACE, m_wndInterfaceEdit);
	DDX_Control(pDX, IDC_EDIT_DZDELAY, m_wndDzDelay);
	DDX_Text(pDX, IDC_EDIT_DZDELAY, m_nDelayDZ);
	DDV_MinMaxInt(pDX, m_nDelayDZ, -3600, 3600);
	DDX_Control(pDX, IDC_SPIN1, m_wndSpin1);
}


BEGIN_MESSAGE_MAP(CRtuAttrBasicInfo, CDialog)
	ON_BN_CLICKED(IDC_CONN_CHECK, &CRtuAttrBasicInfo::OnBnClickedConnCheck)
	ON_BN_CLICKED(IDC_BTN_SELTHREAD, &CRtuAttrBasicInfo::OnBnClickedBtnSelthread)
	ON_BN_CLICKED(IDC_BASIC_DTCALL,&CRtuAttrBasicInfo::OnBnClickedDtCall)
	ON_BN_CLICKED(IDC_BASIC_DTSET,&CRtuAttrBasicInfo::OnBnClickedDtSet)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CRtuAttrBasicInfo::OnCbnSelchangeCombo2)
	ON_CBN_SELCHANGE(IDC_CMB_WORKMODE, &CRtuAttrBasicInfo::OnCbnSelchangeCmbWorkmode)
	ON_BN_CLICKED(IDC_WORKMODE_CALL, &CRtuAttrBasicInfo::OnBnClickedWorkmodeCall)
	ON_BN_CLICKED(IDC_WORKMODE_SET, &CRtuAttrBasicInfo::OnBnClickedWorkmodeSet)
	ON_BN_CLICKED(IDC_BUTTON_SETDELAY,&CRtuAttrBasicInfo::OnBnClickedSetDelay)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CRtuAttrBasicInfo 消息处理程序


void CRtuAttrBasicInfo::Update()
{
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	CRealTimeDB* pRealDB = pApp->m_pReadDB;

	/*
	if (m_wndCmbThreads.GetSafeHwnd())
	{
		m_wndCmbThreads.ResetContent();
		CCMSApp* pApp = (CCMSApp*) AfxGetApp();
		CCommServer* pCommServer = pApp->m_pCommServer;
		if (pCommServer)
		{
			POSITION rPos = pCommServer->m_lstThreads.GetHeadPosition();
			while(rPos)
			{
				CCommThreadInfo *pThread = pCommServer->m_lstThreads.GetNext(rPos);
				if (pThread && pThread->GetThreadState() == THREAD_STATE_RUNNING)
				{
					int idx = m_wndCmbThreads.AddString(pThread->GetThreadName());
					m_wndCmbThreads.SetItemDataPtr(idx,pThread);
				}
			}
		}
	}
	*/

	if (m_pParentDlg)
	{
		CRtuStation* pSub = m_pParentDlg->GetRtuStation();
		if (pSub)
		{
			m_pSubStation = pSub;


			CString strTemp = "0";
			strTemp.Format("%d",m_pSubStation->GetDzDelay());
			m_nDelayDZ = m_pSubStation->GetDzDelay();
			m_wndDzDelay.SetWindowText(strTemp);

			if (m_wndCmbWorkMode.GetSafeHwnd())
			{
				BYTE nAppProV = APP_PROTOCOL_V1_0;
				if (m_pSubStation != NULL)
					nAppProV = m_pSubStation->GetAppProVer();
				
				m_wndCmbWorkMode.ResetContent();
				int ItemIdx = 0;
				ItemIdx = m_wndCmbWorkMode.AddString("兼容工作模式");
				m_wndCmbWorkMode.SetItemData(ItemIdx,0);

				ItemIdx = m_wndCmbWorkMode.AddString("自报工作模式");
				m_wndCmbWorkMode.SetItemData(ItemIdx,1);

				/*
				ItemIdx = m_wndCmbWorkMode.AddString("查询/应答工作状态");
				m_wndCmbWorkMode.SetItemData(ItemIdx,2);

				ItemIdx = m_wndCmbWorkMode.AddString("调试/维修状态");
				m_wndCmbWorkMode.SetItemData(ItemIdx,3);
				*/

				if (nAppProV == APP_PROTOCOL_V2_0)
				{
					ItemIdx = m_wndCmbWorkMode.AddString("间歇工作模式");
						m_wndCmbWorkMode.SetItemData(ItemIdx,2);
				}

			}


			if (m_wndCmbWorkMode.GetSafeHwnd() && m_wndCmbWorkMode.GetCount()>0)
			{
				for (int nIdx = 0; nIdx< m_wndCmbWorkMode.GetCount(); nIdx++)
				{
					BYTE itemData = (BYTE)m_wndCmbWorkMode.GetItemData(nIdx);
					if (itemData == m_pSubStation->GetWorkMode())
					{
						m_wndCmbWorkMode.SetCurSel(nIdx);
						break;
					}
				}
			}


			//更新TextBox 信息
			m_strName = m_pSubStation->GetName();
			if (pRealDB)
			{
				CUdfProvinceCode* pProvin = pRealDB->m_AdDivCodeDb.SearchProvinceCodeItem(m_pSubStation->GetProvinceCode());
				if (pProvin)
				{
					m_strPronvince = pProvin->GetName();

					CUdfRegionCode* pRegion = pProvin->SearchRegionCodeItem(m_pSubStation->GetRegionCode());
					if (pRegion)
					{
						m_strRegion = pRegion->GetName();

						CUdfCountyCode* pCounty = pRegion->SearchCountyCodeItem(m_pSubStation->GetCountyCode());
						if (pCounty)
							m_strCounty = pCounty->GetName();
					}
				}
			}

			m_strA2.Format("%d",m_pSubStation->GetAddrA2());

			m_strDt = "";
			UpdateData(FALSE);

			UpdateCommInfo();

			//m_strBaCommThreadInfo = "";
	
			

			/*
			CCommThreadInfo* pSub_Thread = m_pSubStation->GetCommThreadPointer();
			if ( pSub_Thread != NULL)
			{
				bool blFinded = false;
				for (int idx = 0 ; idx < m_wndCmbThreads.GetCount(); idx ++)
				{
					CCommThreadInfo* pTemp_Thread = (CCommThreadInfo*)m_wndCmbThreads.GetItemDataPtr(idx);
					if (pTemp_Thread == pSub_Thread)
					{
						blFinded = true;
						m_wndCmbThreads.SetCurSel(idx);
						OnBnClickedBtnSelthread();
						break;;
					}
				}

				if (blFinded == false)
					m_pSubStation->SetCommThreadPointer(NULL);
			}
			*/

			BYTE linkCheckState = m_pSubStation->GetLinkCheckState();
			for (int idx = 0 ;idx < m_wndCmbConnType.GetCount(); idx ++)
			{
				BYTE itemDate = (BYTE)m_wndCmbConnType.GetItemData(idx);
				if (itemDate == linkCheckState)
				{
					m_wndCmbConnType.SetCurSel(idx);
					break;
				}
			}


			if (m_pSubStation->GetRecvFLAG() == true)
				m_EditRecvTime.SetWindowText(m_pSubStation->GetLastRecvTimeString());
			else if (m_pSubStation->GetRecvFLAG() == false)
				m_EditRecvTime.SetWindowText("");
		}
		else
			m_pSubStation = NULL;
	}
}

void CRtuAttrBasicInfo::UpdateCommInfo()
{
	if (m_pSubStation != NULL)
	{
		m_wndCmTypeEdit.SetWindowText(CUdfGlobalFUNC::GetCmTypeString(m_pSubStation->GetCmType()));
		m_wndInterfaceEdit.SetWindowText(CUdfGlobalFUNC::GetInterfaceString(m_pSubStation->GetInterface()));

		if (m_pSubStation->GetInterface() == Serial_CommInterface)
			InitGridCtrl_ComInfo();
		else if (m_pSubStation->GetInterface() == Ethernet_CommInerface)
			InitGridCtrl_NetInfo();
		else
		{
			m_wndCmOptGridCtrl.DeleteAllItems();
			m_wndCmOptGridCtrl.Invalidate();
		}
		m_wndCmOptGridCtrl.Refresh();
	}
}

BOOL CRtuAttrBasicInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect rect;
	m_wndGridParent.GetClientRect(&rect);

	if (!m_wndCmOptGridCtrl.Create(rect, 
		&m_wndGridParent, IDC_ADDSTRTU_GRID,WS_CHILD|WS_VISIBLE))
	{
		TRACE("Failed to create threads  grid!\n");
		return FALSE;
	}

	m_wndCmOptGridCtrl.SetBkColor(RGB(255,255,255));
	m_wndCmOptGridCtrl.SetListMode(TRUE);
	m_wndCmOptGridCtrl.SetSingleColSelection(TRUE);
	m_wndCmOptGridCtrl.SetSingleRowSelection(TRUE);

	if (m_wndCmbConnType.GetSafeHwnd())
	{
		m_wndCmbConnType.ResetContent();

		int ItemIdx = m_wndCmbConnType.AddString("登录");
		m_wndCmbConnType.SetItemData(ItemIdx,0xF0);

		ItemIdx = m_wndCmbConnType.AddString("退出登录");
		m_wndCmbConnType.SetItemData(ItemIdx,0xF1);

		ItemIdx = m_wndCmbConnType.AddString("在线保持");
		m_wndCmbConnType.SetItemData(ItemIdx,0xF2);
	}

	m_wndSpin1.SetRange(-3600,3600);                   //Adjust   These   Values   
	m_wndSpin1.SetBuddy(this->GetDlgItem(IDC_EDIT_DZDELAY));  

	return TRUE;  
	// return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CRtuAttrBasicInfo::OnCbnSelchangeCmbWorkmode()
{
	if (!m_pParentDlg) return;
	m_pSubStation = m_pParentDlg->GetRtuStation();
	if (!m_pSubStation) return;

	int nSel = m_wndCmbWorkMode.GetCurSel();
	if (nSel == CB_ERR) return;

	m_pSubStation->SetWorkMode((BYTE)m_wndCmbWorkMode.GetItemData(nSel));
}


void CRtuAttrBasicInfo::OnBnClickedWorkmodeCall()
{
	if (m_pSubStation != NULL)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{
			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CPacketFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息

			m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_52H(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)  *1000;
			SetTimer(W_TIMER_WORKMODE_CALL,nElapse,NULL);
		}
	}
}

void CRtuAttrBasicInfo::OnBnClickedWorkmodeSet()
{
	//UpdateData(TRUE);
	if (m_pSubStation != NULL)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{
			CPacketFrameFormat appFrame ;
			m_pSubStation->BuildBasicPacket(appFrame);

			m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_12H(&appFrame);
		}
	}
}

void CRtuAttrBasicInfo::OnBnClickedSetDelay()
{
	if (m_pSubStation != NULL)
	{
		UpdateData(TRUE);

		//CString strTemp = "0";
		//m_wndDzDelay.GetWindowText(strTemp);
		//int nDelay = atoi(strTemp);
		TRACE("%d\r\n",m_nDelayDZ);
		m_pSubStation->SetDzDelay(/*nDelay*/m_nDelayDZ);
	}
}

void CRtuAttrBasicInfo::InitGridCtrl_NetInfo()
{
	if (m_wndCmOptGridCtrl.GetSafeHwnd())
	{
		int nRowsCount = 4;
		if (m_pSubStation != NULL && m_pSubStation->GetCmType() == SC_CommChannel)
			nRowsCount = 5;

		m_wndCmOptGridCtrl.DeleteAllItems();
		m_wndCmOptGridCtrl.SetRowCount(nRowsCount);
		m_wndCmOptGridCtrl.SetFixedRowCount(1);

		CString strHearders[]  = {"IP地址","端口号","通信协议版本","卫星通信地址"};
		CString strColHeaders[] ={"属性项","值"};

		m_wndCmOptGridCtrl.InsertColumn(strColHeaders[0],DT_LEFT|DT_VCENTER|DT_SINGLELINE,0);
		m_wndCmOptGridCtrl.SetColumnWidth(0,90);
		m_wndCmOptGridCtrl.InsertColumn(strColHeaders[1],DT_LEFT|DT_VCENTER|DT_SINGLELINE,1);
		m_wndCmOptGridCtrl.SetColumnWidth(1,200/*210*/);

		for (int Idx = 1; Idx < nRowsCount; Idx ++)
		{
			m_wndCmOptGridCtrl.SetItemText(Idx,0,strHearders[Idx-1]);
			OnCellReadOnly(Idx,0);
			OnCellReadOnly(Idx,1);
			m_wndCmOptGridCtrl.SetRowHeight(Idx,MFC_GRIDCTRL_HEIGHT);
		}

		m_wndCmOptGridCtrl.SetItemText(1,1, m_pSubStation->GetIpAddrA());
	
		if (!m_wndCmOptGridCtrl.SetCellType(2,1, RUNTIME_CLASS(CGridCellNumeric)))
			return;

		CString _sTemp = "0";
		_sTemp.Format("%d",m_pSubStation->GetSockPortA());
		m_wndCmOptGridCtrl.SetItemText(2,1, _sTemp);

		_sTemp.Format("V%d.0 版",m_pSubStation->GetAppProVer());
		m_wndCmOptGridCtrl.SetItemText(3,1, _sTemp);

		if (m_pSubStation->GetCmType() == SC_CommChannel)
		{
			_sTemp.Format("%d",m_pSubStation->GetScAddress());
			m_wndCmOptGridCtrl.SetItemText(4,1,_sTemp);
		}
	}
}


void CRtuAttrBasicInfo::InitGridCtrl_ComInfo()
{
	if (m_wndCmOptGridCtrl.GetSafeHwnd())
	{
		int nRowsCount = 7;
		if (m_pSubStation != NULL  && m_pSubStation->GetCmType() == SC_CommChannel)
			nRowsCount = 8;


		m_wndCmOptGridCtrl.DeleteAllItems();
		m_wndCmOptGridCtrl.SetRowCount(nRowsCount);
		m_wndCmOptGridCtrl.SetFixedRowCount(1);

		CString strHearders[] = {"串口号",
			"波特率",
			"数据位",
			"停止位",
			"校验",
			"通信协议版本",
			"卫星通信地址"
		};

		CString strColHeaders[] ={"属性项","值"};

		m_wndCmOptGridCtrl.InsertColumn(strColHeaders[0],DT_LEFT|DT_VCENTER|DT_SINGLELINE,0);
		m_wndCmOptGridCtrl.SetColumnWidth(0,90);
		m_wndCmOptGridCtrl.InsertColumn(strColHeaders[1],DT_LEFT|DT_VCENTER|DT_SINGLELINE,1);
		m_wndCmOptGridCtrl.SetColumnWidth(1,200/*210*/);

		for (int Idx = 1; Idx < nRowsCount; Idx ++)
		{
			m_wndCmOptGridCtrl.SetItemText(Idx,0,strHearders[Idx-1]);
			OnCellReadOnly(Idx,0);
			OnCellReadOnly(Idx,1);
			m_wndCmOptGridCtrl.SetRowHeight(Idx,MFC_GRIDCTRL_HEIGHT);
		}

		if (m_pSubStation)
		{
			CString strTemp = "";
			m_wndCmOptGridCtrl.SetItemText(1,1,m_pSubStation->GetComPortA());

			strTemp.Format("%d",m_pSubStation->GetBaundA());
			m_wndCmOptGridCtrl.SetItemText(2,1,strTemp);

			strTemp.Format("%d位",m_pSubStation->GetDataBitsA());
			m_wndCmOptGridCtrl.SetItemText(3,1,strTemp);
			
			strTemp = CUdfGlobalFUNC::GetStopBitsString(m_pSubStation->GetStopBitsA());
			m_wndCmOptGridCtrl.SetItemText(4,1,strTemp);

			strTemp = CUdfGlobalFUNC::GetParityString(m_pSubStation->GetParityA());
			m_wndCmOptGridCtrl.SetItemText(5,1,strTemp);	

			strTemp.Format("V%d.0 版",m_pSubStation->GetAppProVer());
			m_wndCmOptGridCtrl.SetItemText(6,1,strTemp);	

			if(m_pSubStation->GetCmType() == SC_CommChannel)
			{
				strTemp.Format("%d",m_pSubStation->GetScAddress());
				m_wndCmOptGridCtrl.SetItemText(7,1,strTemp);
			}
		}

	}
}

void CRtuAttrBasicInfo::OnCellNormal(int Row,int Col)
{
	if (m_wndCmOptGridCtrl.GetSafeHwnd())
	{
		m_wndCmOptGridCtrl.SetCellType(Row,Col, RUNTIME_CLASS(CGridCell));
		m_wndCmOptGridCtrl.SetItemText(Row,Col, _T(""));
		m_wndCmOptGridCtrl.SetItemState(Row,Col, m_wndCmOptGridCtrl.GetItemState(Row,Col) & ~GVIS_READONLY);
		m_wndCmOptGridCtrl.Invalidate();
	}
}

void CRtuAttrBasicInfo::OnCellReadOnly(int Row,int Col)
{
	if (m_wndCmOptGridCtrl.GetSafeHwnd())
	{
		m_wndCmOptGridCtrl.SetItemState(Row,Col, m_wndCmOptGridCtrl.GetItemState(Row,Col) | GVIS_READONLY);
	}
}


void CRtuAttrBasicInfo::OnBnClickedConnCheck()
{

	int nSel = m_wndCmbConnType.GetCurSel();
	if (nSel == CB_ERR)  return;

	BYTE Data  = 0;
	Data = (BYTE)m_wndCmbConnType.GetItemData(nSel);

	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{
			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CPacketFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息

			appFrame._asdu.data.push_back(Data);
			m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_02H(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1) * 1000;
			SetTimer(W_TIMER_CONNCHECK,nElapse,NULL);
		}
	}
} 

void CRtuAttrBasicInfo::OnBnClickedDtCall()
{
	m_strDt = "";
	UpdateData(FALSE);
	if (m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{
			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CPacketFrameFormat appFrame ;
			m_pSubStation->BuildBasicPacket(appFrame);

			m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_51H(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1) * 1000;
			SetTimer(W_TIMER_DATETIMECALL,nElapse,NULL);
		}
	}
}

void CRtuAttrBasicInfo::OnBnClickedDtSet()
{
	if (m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans() == true)
		{
			CPacketFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);
			m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_11H(&appFrame);
		}
	}
}

void CRtuAttrBasicInfo::OnBnClickedBtnSelthread()
{
	
	//int nSel = m_wndCmbThreads.GetCurSel();
	//if (nSel == CB_ERR)	return;

	//CCommThreadInfo* pThread = (CCommThreadInfo*)m_wndCmbThreads.GetItemDataPtr(nSel);
	//if (pThread && m_pSubStation)
	//{
	//	//m_pSubStation->SetCommThreadPointer(pThread);


	//	//BYTE CmInterface= pThread->GetCommInterfaceType();
	//	//if(m_pSubStation->GetInterface() != CmInterface)
	//	//	m_pSubStation->SetInterface(CmInterface);		 //更新通信接口

	//	////waterFrame.m_CommInterface = CmInterface;
	//	//if (CmInterface == Ethernet_CommInerface)
	//	//{

	//	//}
	//	//else if (CmInterface == Serial_CommInterface)
	//	//{
	//	//	PCOM_INFO pComInfo = pThread->GetComInfo();
	//	//	m_pSubStation->SetComPortA(pComInfo->mComName);
	//	//	m_pSubStation->SetBaundA(pComInfo->mBaund);
	//	//	m_pSubStation->SetDataBitsA(pComInfo->mDataBits);
	//	//	m_pSubStation->SetStopBitsA(pComInfo->mStopBits);
	//	//	m_pSubStation->SetParityA(pComInfo->mParity);
	//	//}

	//	m_strBaCommThreadInfo = pThread->GetThreadAllInfo();
	//	UpdateData(FALSE);
	//	
	//	if (m_pSubStation->GetInterface() == Serial_CommInterface)
	//		InitGridCtrl_ComInfo();
	//	else if (m_pSubStation->GetInterface() == Ethernet_CommInerface)
	//		InitGridCtrl_NetInfo();
	//	else
	//	{
	//		m_wndCmOptGridCtrl.DeleteAllItems();
	//		m_wndCmOptGridCtrl.Invalidate();
	//	}
	//	m_wndCmOptGridCtrl.Refresh();
	//}
}

void CRtuAttrBasicInfo::OnCbnSelchangeCombo2()
{
	OnBnClickedBtnSelthread();
}


void CRtuAttrBasicInfo::OnTimer(UINT_PTR nIDEvent)
{
	CString strLog = "";
	_Log_level_type logLevel = Normal_Log_Message;

	switch (nIDEvent)
	{
	case  W_TIMER_CONNCHECK:
		{
			if (m_blResult == TRUE)
			{
				//strLog = "链路检查 成功!";
			}
			else if (m_blResult == FALSE)
			{
				strLog = "链路检查 超时!";
				logLevel = Warning_log_Message;
			}
		}
		break;
	case W_TIMER_DATETIMECALL:
		{
			if (m_blResult == TRUE)
				strLog = "时钟查询 成功!";
			else if (m_blResult == FALSE)
			{
				strLog = "时钟查询 超时!";
				logLevel = Warning_log_Message;
			}
		}
		break;
	case W_TIMER_WORKMODE_CALL:
		{
			if (m_blResult == TRUE)
				strLog = "查询遥测终端的工作模式 成功!";
			else if (m_blResult == FALSE)
			{
				strLog = "查询遥测终端的工作模式 超时!";
				logLevel = Warning_log_Message;
			}
		}
		break;
	case W_TIMER_WORKMODE_SET:
		{
			if (m_blResult == TRUE)
				strLog = "设置遥测终端的工作模式 成功!";
			else if (m_blResult == FALSE)
			{
				strLog = "设置遥测终端的工作模式 超时!";
				logLevel = Warning_log_Message;
			}
		}
		break;

	}
	KillTimer(nIDEvent);
	SetWndsEnable(TRUE);

	if (m_pParentDlg && m_pParentDlg->GetSafeHwnd())
		m_pParentDlg->AddLog(strLog,(PSYSTEMTIME)NULL,logLevel);

	if (m_pSubStation)
		m_pSubStation->InsertLogMessage(strLog,logLevel,NULL);

	CDialog::OnTimer(nIDEvent);
}


void CRtuAttrBasicInfo::SetWndsEnable(BOOL blEnable /* = TRUE */)
{
	if (m_btnConnCheck.GetSafeHwnd())
		m_btnConnCheck.EnableWindow(blEnable);

	if (m_wndCmbConnType.GetSafeHwnd())
		m_wndCmbConnType.EnableWindow(blEnable);

	if (m_wndCmbThreads.GetSafeHwnd())
		m_wndCmbThreads.EnableWindow(blEnable);

	if (m_btnDtCall.GetSafeHwnd())
		m_btnDtCall.EnableWindow(blEnable);

	if (m_btnDtSet.GetSafeHwnd())
		m_btnDtSet.EnableWindow(blEnable);

	if (m_btnWorkModeCall.GetSafeHwnd())
		m_btnWorkModeCall.EnableWindow(blEnable);

	if (m_btnWorkModeSet.GetSafeHwnd())
		m_btnWorkModeSet.EnableWindow(blEnable);

	if (m_wndCmbWorkMode.GetSafeHwnd())
		m_wndCmbWorkMode.EnableWindow(blEnable);
}

void CRtuAttrBasicInfo::UpdateDlg(int attrType,BOOL blResult)
{
	m_blResult = blResult;
	if (attrType == rtu_attr_Connect_check)
	{
		SendMessage(WM_TIMER,W_TIMER_CONNCHECK,NULL);
	}
	else if (attrType == rtu_attr_datetime_call)
	{
		SendMessage(WM_TIMER,W_TIMER_DATETIMECALL,NULL);
	}
	else if (attrType == rtu_attr_workmode_call)
	{
		SendMessage(WM_TIMER,W_TIMER_WORKMODE_CALL,NULL);
	}
	else if (attrType == rtu_attr_workmode_set)
	{
		SendMessage(WM_TIMER,W_TIMER_WORKMODE_SET,NULL);
	}
}

void CRtuAttrBasicInfo::SetDateTime(CString strDateTime)
{
	m_strDt = strDateTime;
	UpdateData(FALSE);
}

void CRtuAttrBasicInfo::KillAllTimers()
{
	KillTimer(W_TIMER_CONNCHECK);
	KillTimer(W_TIMER_DATETIMECALL);
	KillTimer(W_TIMER_WORKMODE_CALL);
	KillTimer(W_TIMER_WORKMODE_SET);
	SetWndsEnable(TRUE);
}