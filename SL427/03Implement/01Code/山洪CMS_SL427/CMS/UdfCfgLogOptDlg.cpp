// UdfCfgLogOptDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "UdfCfgLogOptDlg.h"
#include "MainFrm.h"


// CUdfCfgLogOptDlg 对话框

IMPLEMENT_DYNAMIC(CUdfCfgLogOptDlg, CDialog)

CUdfCfgLogOptDlg::CUdfCfgLogOptDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUdfCfgLogOptDlg::IDD, pParent)
	, m_blCmStatus(FALSE)
	, m_blCmBuffer(FALSE)
	, m_blYkOperate(FALSE)
	, m_blYcChange(FALSE)
	, m_blYxChange(FALSE)
	, m_blZdOperate(FALSE)
	, m_blSysRunning(FALSE)
	, m_blOthersRun(FALSE)
	, m_blDataInErr(FALSE)
	, m_nSaveMonths(0)
{

}

CUdfCfgLogOptDlg::~CUdfCfgLogOptDlg()
{


	
}

void CUdfCfgLogOptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CUdfCfgLogOptDlg, CDialog)
	ON_WM_CLOSE()
	ON_NOTIFY(GVN_ENDLABELEDIT,IDC_LOGCFG_GRIDCTRL,&CUdfCfgLogOptDlg::OnGridEndEdit)
END_MESSAGE_MAP()


// CUdfCfgLogOptDlg 消息处理程序

void CUdfCfgLogOptDlg::OnOK()
{
	UpdateData(TRUE);


	BOOL blModified = SaveLogOpts();

	if (blModified == TRUE)
	{
		CCMSApp* pApp =(CCMSApp*)AfxGetApp();
		CMainFrame* pMainFrame = (CMainFrame*)pApp->m_pMainWnd;
		if (pMainFrame->m_wndLogFrame.GetSafeHwnd() && 
			pMainFrame->m_wndLogFrame.m_pLeftView &&
			pMainFrame->m_wndLogFrame.m_pLeftView->GetSafeHwnd())
			pMainFrame->m_wndLogFrame.m_pLeftView->InitTreeView();
	}

	ShowWindow(SW_HIDE);
}

BOOL CUdfCfgLogOptDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CWnd* pWnd = GetDlgItem(IDC_STATIC_LOGCFGGRID);
	CRect rect1, rect2;
	int captionHeight = ::GetSystemMetrics(SM_CYCAPTION);
	int cxframe = GetSystemMetrics(SM_CXFRAME);
	int cyframe = GetSystemMetrics(SM_CYFRAME);
	this->GetWindowRect(&rect2);
	if (pWnd)
		pWnd->GetWindowRect(&rect1);
	if (!m_wndGridCtrl.Create(CRect(rect1.left - rect2.left - cxframe, 
		rect1.top-rect2.top - cyframe-captionHeight, 
		rect1.left + rect1.Width() - rect2.left, 
		rect1.top + rect1.Height() - rect2.top - captionHeight), 
		this, IDC_LOGCFG_GRIDCTRL,WS_CHILD|WS_VISIBLE))
	{
		TRACE("Failed to create threads  grid!\n");
		return FALSE;
	}
	
	m_wndGridCtrl.SetBkColor(RGB(255,255,255));
	m_wndGridCtrl.SetListMode(TRUE);
	m_wndGridCtrl.SetSingleColSelection(TRUE);
	m_wndGridCtrl.SetSingleRowSelection(TRUE);
	//m_wndGridCtrl.SetEditable(FALSE);
	InitGridCtrl();

	UpdateData(FALSE);
	SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
	return TRUE;  
	// return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CUdfCfgLogOptDlg::InitGridCtrl()
{
	if (m_wndGridCtrl.GetSafeHwnd())
	{
		m_wndGridCtrl.SetColumnCount(2);
		m_wndGridCtrl.SetRowCount(7);
		m_wndGridCtrl.SetFixedRowCount(1);

		CString strColHeaders[] ={"属性项","属性值"};
		m_wndGridCtrl.SetItemText(0,0,strColHeaders[0]);
		m_wndGridCtrl.SetColumnWidth(0,160);
		m_wndGridCtrl.SetItemText(0,1,strColHeaders[1]);
		m_wndGridCtrl.SetColumnWidth(1,90);

		m_wndGridCtrl.SetItemText(1,0,CUdfLogSystem::GetLogTypeName(RTU_CM_STATUS_LOG));
		m_wndGridCtrl.SetItemText(2,0,CUdfLogSystem::GetLogTypeName(RTU_CM_BUFFER_LOG));
		m_wndGridCtrl.SetItemText(3,0,CUdfLogSystem::GetLogTypeName(RTU_YK_OPERAT_LOG));
		m_wndGridCtrl.SetItemText(4,0,CUdfLogSystem::GetLogTypeName(DATAIN_ERROR_LOG));
		m_wndGridCtrl.SetItemText(5,0,CUdfLogSystem::GetLogTypeName(SYS_RUNNING_S_LOG));
		m_wndGridCtrl.SetItemText(6,0,"日志容量(1~3月)");

		for (int i = 1;i < m_wndGridCtrl.GetRowCount(); i ++)
		{
			CUdfGlobalFUNC::SetGridCellReadOnly(&m_wndGridCtrl,i,0);
			CUdfGlobalFUNC::SetGridCellNormal(&m_wndGridCtrl,i,1);
			if (i < m_wndGridCtrl.GetRowCount() -1)
			{
				SetGridCellComboBox(i,1);
			}
		}

		if (!m_wndGridCtrl.SetCellType(6,1, RUNTIME_CLASS(CGridCellNumeric)))
			return;
	}

}

void CUdfCfgLogOptDlg::SetGridCellComboBox(int Row,int Col)
{
	if (m_wndGridCtrl.GetSafeHwnd())
	{
		if (!m_wndGridCtrl.SetCellType(Row,Col, RUNTIME_CLASS(CGridCellCombo)))
			return;

		CStringArray options;
		options.Add(_T("启动"));
		options.Add(_T("关闭"));

		CGridCellCombo *pCell = (CGridCellCombo*) m_wndGridCtrl.GetCell(Row,Col);
		pCell->SetOptions(options);
		pCell->SetStyle(CBS_DROPDOWNLIST);
	}
}


void CUdfCfgLogOptDlg::Show(int nCmdShow)
{
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();

	m_blCmStatus	= pApp->m_GlobalOptions.m_LogOpts.CmStatusSave_Flag;
	m_blCmBuffer	= pApp->m_GlobalOptions.m_LogOpts.CmBufferSave_Flag;
	m_blYkOperate	= pApp->m_GlobalOptions.m_LogOpts.YkOperatSave_Flag;
	m_blYcChange	= pApp->m_GlobalOptions.m_LogOpts.YcChangeSave_Flag;
	m_blYxChange	= pApp->m_GlobalOptions.m_LogOpts.YxChangeSave_Flag;
	m_blZdOperate	= pApp->m_GlobalOptions.m_LogOpts.ZdOperatSave_Flag;
	m_blSysRunning	= pApp->m_GlobalOptions.m_LogOpts.RunningSSave_Flag;
	m_blOthersRun	= pApp->m_GlobalOptions.m_LogOpts.OhterRunSave_FLag;
	m_blDataInErr   = pApp->m_GlobalOptions.m_LogOpts.DataInErrSave_Flag;
	m_nSaveMonths	= pApp->m_GlobalOptions.m_dwLogSaveMonths;	


	if (m_wndGridCtrl.GetSafeHwnd())
	{
		if (m_blCmStatus )
			m_wndGridCtrl.SetItemText(1,1,"启动");
		else
			m_wndGridCtrl.SetItemText(1,1,"关闭");

		if (m_blCmBuffer)
			m_wndGridCtrl.SetItemText(2,1,"启动");
		else
			m_wndGridCtrl.SetItemText(2,1,"关闭");


		if (m_blYkOperate)
			m_wndGridCtrl.SetItemText(3,1,"启动");
		else
			m_wndGridCtrl.SetItemText(3,1,"关闭");

		if (m_blDataInErr)
			m_wndGridCtrl.SetItemText(4,1,"启动");
		else
			m_wndGridCtrl.SetItemText(4,1,"关闭");

		if (m_blSysRunning)
			m_wndGridCtrl.SetItemText(5,1,"启动");
		else
			m_wndGridCtrl.SetItemText(5,1,"关闭");

		CString strTemp = "";
		strTemp.Format("%d",m_nSaveMonths);
		
		m_wndGridCtrl.SetItemText(6,1,strTemp);
		m_wndGridCtrl.Refresh();
	}

	ShowWindow(nCmdShow);
}

void CUdfCfgLogOptDlg::OnCancel()
{
	ShowWindow(SW_HIDE);
}


BOOL CUdfCfgLogOptDlg::SaveLogOpts()
{ 
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	if (m_blCmStatus	!= pApp->m_GlobalOptions.m_LogOpts.CmStatusSave_Flag ||
		m_blCmBuffer	!= pApp->m_GlobalOptions.m_LogOpts.CmBufferSave_Flag ||
		m_blYkOperate	!= pApp->m_GlobalOptions.m_LogOpts.YkOperatSave_Flag ||
		m_blYcChange	!= pApp->m_GlobalOptions.m_LogOpts.YcChangeSave_Flag ||
		m_blYxChange	!= pApp->m_GlobalOptions.m_LogOpts.YxChangeSave_Flag ||
		m_blZdOperate	!= pApp->m_GlobalOptions.m_LogOpts.ZdOperatSave_Flag ||
		m_blSysRunning	!= pApp->m_GlobalOptions.m_LogOpts.RunningSSave_Flag ||
		m_blOthersRun	!= pApp->m_GlobalOptions.m_LogOpts.OhterRunSave_FLag ||
		m_blDataInErr   != pApp->m_GlobalOptions.m_LogOpts.DataInErrSave_Flag||
		m_nSaveMonths	!= pApp->m_GlobalOptions.m_dwLogSaveMonths)
	{
		//修改运行参数的值
		pApp->m_GlobalOptions.m_LogOpts.CmStatusSave_Flag	=  (BYTE)m_blCmStatus;
		pApp->m_GlobalOptions.m_LogOpts.CmBufferSave_Flag	=  (BYTE)m_blCmBuffer;
		pApp->m_GlobalOptions.m_LogOpts.YkOperatSave_Flag	=  (BYTE)m_blYkOperate;
		pApp->m_GlobalOptions.m_LogOpts.YcChangeSave_Flag	=  (BYTE)m_blYcChange;
		pApp->m_GlobalOptions.m_LogOpts.YxChangeSave_Flag	=  (BYTE)m_blYxChange;
		pApp->m_GlobalOptions.m_LogOpts.ZdOperatSave_Flag 	=  (BYTE)m_blZdOperate;
		pApp->m_GlobalOptions.m_LogOpts.RunningSSave_Flag	=  (BYTE)m_blSysRunning;
		pApp->m_GlobalOptions.m_LogOpts.OhterRunSave_FLag	=  (BYTE)m_blOthersRun;
		pApp->m_GlobalOptions.m_LogOpts.DataInErrSave_Flag  =  (BYTE)m_blDataInErr;
		pApp->m_GlobalOptions.m_dwLogSaveMonths				=  (BYTE)m_nSaveMonths;	

		//修改运行文件参数的值
		pApp->m_sysConfigObj.SetItemValue("CMSTATUS_SV",(int)m_blCmStatus);
		pApp->m_sysConfigObj.SetItemValue("CMBUFFER_SV",(int)m_blCmBuffer);
		pApp->m_sysConfigObj.SetItemValue("YKOPERAT_SV",(int)m_blYkOperate);
		pApp->m_sysConfigObj.SetItemValue("YCCHANGE_SV",(int)m_blYcChange);
		pApp->m_sysConfigObj.SetItemValue("YXCHANGE_SV",(int)m_blYxChange);
		pApp->m_sysConfigObj.SetItemValue("ZDOPERAT_SV",(int)m_blZdOperate);
		pApp->m_sysConfigObj.SetItemValue("RUNNINGS_SV",(int)m_blSysRunning);
		pApp->m_sysConfigObj.SetItemValue("OTHERRUN_SV",(int)m_blOthersRun);
		pApp->m_sysConfigObj.SetItemValue("SAVELOGLEN", (int)m_nSaveMonths);
		pApp->m_sysConfigObj.SetItemValue("DATAINER_SV",(int)m_blDataInErr);
		pApp->m_sysConfigObj.SaveFile();


		return TRUE;
	}

	return FALSE;
}

void CUdfCfgLogOptDlg::OnClose()
{
	ShowWindow(SW_HIDE);
}


void CUdfCfgLogOptDlg::OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	if (!pItem  || pItem->iRow <= 0  || pItem->iColumn != 1 ) return;

	int rowCount = m_wndGridCtrl.GetRowCount();
	CString strCellText = "";
	strCellText = CUdfGlobalFUNC::GetGridCellText(&m_wndGridCtrl,pItem->iRow,pItem->iColumn);
	if (strCellText.IsEmpty()) return;

	if (pItem->iRow >= 1 && 
		pItem->iRow < rowCount - 1 )
	{
		BYTE blData = 0;
		if (strCellText == "启动")			
		{
			blData = 1;
		}
		else if (strCellText ="关闭")
		{
			blData = 0;
		}

		
		if (pItem->iRow == 1)
			m_blCmStatus = blData;
		else if (pItem->iRow == 2)
			m_blCmBuffer = blData;
		else if (pItem->iRow == 3)
			m_blYkOperate = blData;
		else if (pItem->iRow == 4)
			m_blDataInErr = blData;
		else if (pItem->iRow == 5)
			m_blSysRunning = blData;

	}
	else if (pItem->iRow == rowCount - 1)
	{
		int SaveMonths = atoi (strCellText);

		if (SaveMonths <1 ||
			SaveMonths >3)
		{
			MessageBox("日志数据存储容量为(1~3)月,请重新输入","错误",MB_OK|MB_ICONERROR);
			
			CString strTemp = "";
			strTemp.Format("%d",m_nSaveMonths);
			m_wndGridCtrl.SetItemText(6,1,strTemp);
			m_wndGridCtrl.Refresh();

			return ;
		}

		m_nSaveMonths = SaveMonths;

	}

}