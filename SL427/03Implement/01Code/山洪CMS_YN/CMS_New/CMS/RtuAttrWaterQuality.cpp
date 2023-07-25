// RtuAttrWaterQuality.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuAttrWaterQuality.h"
#include "RtuPropertyDlg.h"


#define W_TIMER_WQ_UP_CALL		0x01        //水质参数上限值查询
#define W_TIMER_WQ_DOWN_CALL	0x02        //水质参数下限值查询
#define W_TIMER_WQ_DATA_CALL    0x03		//水质参数实时值查询


// CRtuAttrWaterQuality 对话框、
IMPLEMENT_DYNAMIC(CRtuAttrWaterQuality, CDialog)
CRtuAttrWaterQuality::CRtuAttrWaterQuality(CWnd* pParent /*=NULL*/)
	: CDialog(CRtuAttrWaterQuality::IDD, pParent)
{
	m_pParentDlg = NULL;
	m_pSubStation = NULL;
}

CRtuAttrWaterQuality::~CRtuAttrWaterQuality()
{
}

void CRtuAttrWaterQuality::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_WQUALITY, m_wndWQualityTypeCmd);
	DDX_Control(pDX, IDC_BTN_WQUPCALL, m_btnWQUpCall);
	DDX_Control(pDX, IDC_BTN_WQUPSET, m_btnWQUpSet);
	DDX_Control(pDX, IDC_BTN_WQDOWNCALL, m_btnWQDownCall);
	DDX_Control(pDX, IDC_BTN_WQDOWNSET, m_btnWQDownSet);
	DDX_Control(pDX, IDC_BTN_WQDATACALL, m_btnWQDataCall);
}


BEGIN_MESSAGE_MAP(CRtuAttrWaterQuality, CDialog)
	ON_CBN_SELCHANGE(IDC_CMB_WQUALITY, &CRtuAttrWaterQuality::OnCbnSelchangeCmbWquality)
	ON_NOTIFY(GVN_ENDLABELEDIT,IDC_WQUALITY_GRID,&CRtuAttrWaterQuality::OnGridEndEdit)
	ON_BN_CLICKED(IDC_BTN_WQUPCALL, &CRtuAttrWaterQuality::OnBnClickedBtnWqupcall)
	ON_BN_CLICKED(IDC_BTN_WQUPSET, &CRtuAttrWaterQuality::OnBnClickedBtnWqupset)
	ON_BN_CLICKED(IDC_BTN_WQDOWNCALL, &CRtuAttrWaterQuality::OnBnClickedBtnWqdowncall)
	ON_BN_CLICKED(IDC_BTN_WQDOWNSET, &CRtuAttrWaterQuality::OnBnClickedBtnWqdownset)
	ON_BN_CLICKED(IDC_BTN_WQDATACALL, &CRtuAttrWaterQuality::OnBnClickedBtnWqdatacall)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CRtuAttrWaterQuality 消息处理程序

void CRtuAttrWaterQuality::SetParentDlg(CRtuPropertyDlg* pDlg)
{
	m_pParentDlg = pDlg;
}

void CRtuAttrWaterQuality::Update()
{

	if (m_pParentDlg != NULL)
		m_pSubStation = m_pParentDlg->GetRtuStation();

	if (m_wndWQualityTypeCmd.GetSafeHwnd())
	{
		m_wndWQualityTypeCmd.ResetContent();

		if( m_pSubStation )
		{
			CWaterQuality_PARA* pWaterQuality = m_pSubStation->GetWaterQualityPara();

			for (BYTE idx = 0; idx < 40; idx ++)
			{
				CWaterQuality_PARA_PER* pWQ_PER = pWaterQuality->GetOneQualityObject(idx);
				if (pWQ_PER != NULL)
				{
					int ItemIndex = m_wndWQualityTypeCmd.AddString(pWQ_PER->m_strName);
					m_wndWQualityTypeCmd.SetItemDataPtr(ItemIndex,pWQ_PER);
				}
			}
		}

		m_wndWQualityTypeCmd.SetCurSel(-1);
		OnCbnSelchangeCmbWquality();
	}
}


BOOL CRtuAttrWaterQuality::OnInitDialog()
{
	CDialog::OnInitDialog();

	CWnd* pWnd = GetDlgItem(IDC_STATIC_WQUALITY);
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
		this, IDC_WQUALITY_GRID,WS_CHILD|WS_VISIBLE))
	{
		TRACE("Failed to create threads  grid!\n");
		return FALSE;
	}

	m_wndGridCtrl.SetBkColor(RGB(255,255,255));
	m_wndGridCtrl.SetListMode(TRUE);
	m_wndGridCtrl.SetSingleColSelection(TRUE);
	m_wndGridCtrl.SetSingleRowSelection(TRUE);




	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CRtuAttrWaterQuality::InitGridItems()
{
	if (m_wndGridCtrl.GetSafeHwnd())
	{
		m_wndGridCtrl.DeleteAllItems();
		m_wndGridCtrl.Refresh();
		int nSel = m_wndWQualityTypeCmd.GetCurSel();
		if (nSel == CB_ERR) return;

		CWaterQuality_PARA_PER* pWQ_One = (CWaterQuality_PARA_PER*)m_wndWQualityTypeCmd.GetItemDataPtr(nSel);

		if (pWQ_One)
		{
			CString strHeaders[8] = {
				"水质参数种类名称",
				"监测与否",
				"单位",
				"最大值(值范围)",
				"最小值(值范围)",
				"上限值",
				"下限值",
				"实时值"
			};

			m_wndGridCtrl.SetRowCount(8);
			m_wndGridCtrl.SetColumnCount(2);

			m_wndGridCtrl.SetColumnWidth(0,150);
			m_wndGridCtrl.SetColumnWidth(1,150);
			for (int idx = 0; idx < 8;idx ++)
			{
				m_wndGridCtrl.SetItemText(idx,0,strHeaders[idx]);
				m_wndGridCtrl.SetRowHeight(idx,MFC_GRIDCTRL_HEIGHT);
				m_wndGridCtrl.SetItemBkColour(idx,0,RGB(128,128,128));
				CUdfGlobalFUNC::SetGridCellReadOnly(&m_wndGridCtrl,idx,0);
			}

			m_wndGridCtrl.SetItemText(0,1,pWQ_One->m_strName);
			CUdfGlobalFUNC::SetGridCellReadOnly(&m_wndGridCtrl,0,1);

			m_wndGridCtrl.SetCellType(1,1,RUNTIME_CLASS(CGridCellCombo));
			CStringArray strArray;
			strArray.Add("监测");
			strArray.Add("不监测");
			CGridCellCombo *pCell = (CGridCellCombo*) m_wndGridCtrl.GetCell(1,1);
			pCell->SetOptions(strArray);
			pCell->SetStyle(CBS_DROPDOWNLIST);
			bool blCheck = m_pSubStation->GetWaterQualityPara()->GetBitValue(pWQ_One->m_BitPos);
			if (blCheck == true)
				pCell->SetText("监测");
			else if (blCheck == false)
				pCell->SetText("不监测");

			m_wndGridCtrl.SetItemText(2,1,pWQ_One->m_strUnit);
			CUdfGlobalFUNC::SetGridCellReadOnly(&m_wndGridCtrl,2,1);

			CString strTemp = "";
			strTemp.Format("%*.*f",pWQ_One->m_DataBits,pWQ_One->m_PointBits,pWQ_One->m_ScaleMax);
			m_wndGridCtrl.SetItemText(3,1,strTemp);
			CUdfGlobalFUNC::SetGridCellReadOnly(&m_wndGridCtrl,3,1);

			strTemp.Format("%*.*f",pWQ_One->m_DataBits,pWQ_One->m_PointBits,pWQ_One->m_ScaleMin);
			m_wndGridCtrl.SetItemText(4,1,strTemp);
			CUdfGlobalFUNC::SetGridCellReadOnly(&m_wndGridCtrl,4,1);

			//m_wndGridCtrl.SetCellType(5,1,RUNTIME_CLASS(CGridCellNumeric));
			strTemp.Format("%*.*f",pWQ_One->m_DataBits,pWQ_One->m_PointBits,pWQ_One->m_UpValume);
			m_wndGridCtrl.SetItemText(5,1,strTemp);

			//m_wndGridCtrl.SetCellType(6,1,RUNTIME_CLASS(CGridCellNumeric));
			strTemp.Format("%*.*f",pWQ_One->m_DataBits,pWQ_One->m_PointBits,pWQ_One->m_DownValue);
			m_wndGridCtrl.SetItemText(6,1,strTemp);

			strTemp.Format("%*.*f",pWQ_One->m_DataBits,pWQ_One->m_PointBits,pWQ_One->m_CurValue);
			m_wndGridCtrl.SetItemText(7,1,strTemp);
			CUdfGlobalFUNC::SetGridCellReadOnly(&m_wndGridCtrl,7,1);

			m_wndGridCtrl.Refresh();
		}
	}

}
void CRtuAttrWaterQuality::OnCbnSelchangeCmbWquality()
{
	InitGridItems();
}

void CRtuAttrWaterQuality::OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	if (!pItem  || pItem->iRow <= 0  || pItem->iColumn != 1 ) return;
	if (!m_pSubStation) return;


	int nSel = m_wndWQualityTypeCmd.GetCurSel();
	if (nSel == CB_ERR) return;

	CWaterQuality_PARA*     pWQ_PARA = m_pSubStation->GetWaterQualityPara();
	CWaterQuality_PARA_PER* pWQ_One = (CWaterQuality_PARA_PER*)m_wndWQualityTypeCmd.GetItemDataPtr(nSel);
	if (!pWQ_One || !pWQ_PARA) return;


	CString strText = CUdfGlobalFUNC::GetGridCellText(&m_wndGridCtrl,pItem->iRow,pItem->iColumn);

	CString strMessage = "";
	CString strTemp= "";
 	switch (pItem->iRow)
	{
	case 1: //设置监测与否
		{
			if (strText == "监测")
				pWQ_PARA->SetBitValue(pWQ_One->m_BitPos,true);
			else if (strText == "不监测")
				pWQ_PARA->SetBitValue(pWQ_One->m_BitPos,false);
		}
		break;
	case 5: //设置上限值
		{
			DOUBLE dValue = atof(strText);
			if (dValue < pWQ_One->m_ScaleMin || 
				dValue > pWQ_One->m_ScaleMax)
			{
				strMessage.Format("输入的值超出范围(%.*f~%.*f),请重新输入!",pWQ_One->m_PointBits,pWQ_One->m_ScaleMin,
					                                                        pWQ_One->m_PointBits,pWQ_One->m_ScaleMax);
				MessageBox(strMessage,"",MB_ICONWARNING|MB_OK);

				
				strTemp.Format("%*.*f",pWQ_One->m_DataBits,pWQ_One->m_PointBits,pWQ_One->m_UpValume);
				m_wndGridCtrl.SetItemText(5,1,strTemp);

				return;

			}

			pWQ_One->m_UpValume = dValue;
			strTemp.Format("%*.*f",pWQ_One->m_DataBits,pWQ_One->m_PointBits,pWQ_One->m_UpValume);
			m_wndGridCtrl.SetItemText(5,1,strTemp);
		}
		break;
	case 6:  //设置下限值
		{
			DOUBLE dValue = atof(strText);
			if (dValue < pWQ_One->m_ScaleMin || 
				dValue > pWQ_One->m_ScaleMax)
			{
				strMessage.Format("输入的值超出范围(%.*f~%.*f),请重新输入!",pWQ_One->m_PointBits,pWQ_One->m_ScaleMin,
																			pWQ_One->m_PointBits,pWQ_One->m_ScaleMax);
				MessageBox(strMessage,"",MB_ICONWARNING|MB_OK);

				
				strTemp.Format("%*.*f",pWQ_One->m_DataBits,pWQ_One->m_PointBits,pWQ_One->m_DownValue);
				m_wndGridCtrl.SetItemText(6,1,strTemp);

				return;
			}

			pWQ_One->m_DownValue = dValue;		
			strTemp.Format("%*.*f",pWQ_One->m_DataBits,pWQ_One->m_PointBits,pWQ_One->m_DownValue);
			m_wndGridCtrl.SetItemText(6,1,strTemp);
		}
		break;
	}

}



void CRtuAttrWaterQuality::OnBnClickedBtnWqupcall()
{
	Update();
	if( m_pSubStation)
	{

		if (m_pSubStation->blEnbaleTrans() == true)
		{

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CPacketFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息

			m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_59H(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_WQ_UP_CALL,nElapse,NULL);
		}
	}
}

void CRtuAttrWaterQuality::OnBnClickedBtnWqupset()
{
	if (m_pSubStation )
	{

		if (m_pSubStation->blEnbaleTrans() == true)
		{
			CPacketFrameFormat appFrame ;
			m_pSubStation->BuildBasicPacket(appFrame);

			
			m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_19H(&appFrame);
		}
	}
}

void CRtuAttrWaterQuality::OnBnClickedBtnWqdowncall()
{	
	Update();
	if( m_pSubStation)
	{

		if (m_pSubStation->blEnbaleTrans() == true)
		{

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CPacketFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息

			m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_5AH(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_WQ_DOWN_CALL,nElapse,NULL);
		}
	}
}

void CRtuAttrWaterQuality::OnBnClickedBtnWqdownset()
{
	if (m_pSubStation )
	{

		if (m_pSubStation->blEnbaleTrans() == true)
		{
			CPacketFrameFormat appFrame ;
			m_pSubStation->BuildBasicPacket(appFrame);

			
			m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_1AH(&appFrame);
		}
	}
}

void CRtuAttrWaterQuality::OnBnClickedBtnWqdatacall()
{
	Update();
	if( m_pSubStation)
	{

		if (m_pSubStation->blEnbaleTrans() == true)
		{

			SetWndsEnable(FALSE);
			m_blResult = FALSE;

			CPacketFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息

			appFrame.ControlC._Command = C_CMD_09_WQPARA;

			m_pSubStation->SetQueryRealDataCmd(appFrame.ControlC._Command);


			m_pSubStation->GetSysDataTrans()->IEC_waterdata_EN_5BH(&appFrame);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_WQ_DATA_CALL,nElapse,NULL);
		}
	}
}

void CRtuAttrWaterQuality::OnTimer(UINT_PTR nIDEvent)
{
	CString strLogMessge = "";
	_Log_level_type _logLevle = Normal_Log_Message;
	
	switch (nIDEvent)
	{
	case W_TIMER_WQ_UP_CALL:
		{
			if (m_blResult == TRUE)
				strLogMessge = "查询遥测终端水质参数种类、上限值 成功!";
			else if( m_blResult == FALSE)
			{
				strLogMessge = "查询遥测终端水质参数种类、上限值 超时!";
				_logLevle = Warning_log_Message;
			}
		}
		break;
	case W_TIMER_WQ_DOWN_CALL:
		{
			if (m_blResult == TRUE)
				strLogMessge = "查询遥测终端水质参数种类、下限值 成功!";
			else if( m_blResult == FALSE)
			{
				strLogMessge = "查询遥测终端水质参数种类、下限值 超时!";
				_logLevle = Warning_log_Message;
			}
		}
		break;
	case W_TIMER_WQ_DATA_CALL:
		{
			if (m_blResult == TRUE)
				strLogMessge = "查询遥测终端水质参数实时值 成功!";
			else if( m_blResult == FALSE)
			{
				strLogMessge = "查询遥测终端水质参数实时值 超时!";
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


void CRtuAttrWaterQuality::SetWndsEnable(BOOL blEnable /* = TRUE */)
{
	if (m_wndGridCtrl.GetSafeHwnd())
		m_wndGridCtrl.EnableWindow(blEnable);
	if (m_wndWQualityTypeCmd.GetSafeHwnd())
		m_wndWQualityTypeCmd.EnableWindow(blEnable);
	if (m_btnWQDataCall.GetSafeHwnd()) 
		m_btnWQDataCall.EnableWindow(blEnable);
	if (m_btnWQDownCall.GetSafeHwnd())
		m_btnWQDownCall.EnableWindow(blEnable);
	if (m_btnWQDownSet.GetSafeHwnd())
		m_btnWQDownSet.EnableWindow(blEnable);
	if (m_btnWQUpCall.GetSafeHwnd())
		m_btnWQUpCall.EnableWindow(blEnable);
	if (m_btnWQUpSet.GetSafeHwnd())
		m_btnWQUpSet.EnableWindow(blEnable);

}


void CRtuAttrWaterQuality::UpdateDlg(int attrType,BOOL blResult)
{
	m_blResult = blResult;
	
	if (attrType == rtu_attr_waterquality_up_call)
		SendMessage(WM_TIMER,W_TIMER_WQ_UP_CALL,NULL);
	else if (attrType == rtu_attr_waterquality_down_call)
		SendMessage(WM_TIMER,W_TIMER_WQ_DOWN_CALL,NULL);
	else if (attrType == rtu_attr_waterquality_data_call)
		SendMessage(WM_TIMER,W_TIMER_WQ_DATA_CALL,NULL);

	Update();
}

void CRtuAttrWaterQuality::KillAllTimers()
{
	KillTimer(W_TIMER_WQ_DATA_CALL);
	KillTimer(W_TIMER_WQ_DOWN_CALL);
	KillTimer(W_TIMER_WQ_UP_CALL);
	SetWndsEnable(TRUE);
}