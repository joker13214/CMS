// RtuAttrSyqcfg1.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuAttrSyqcfg1.h"


#define W_TIMER_R_ADD1   0x01
#define W_TIMER_R_ADD2   0x02
#define W_TIMER_R_ADD3   0x03
#define W_TIMER_R_ADD4   0x04

#define W_TIMER_W_ADD1   0x05
#define W_TIMER_W_ADD2   0x06
#define W_TIMER_W_ADD3   0x07
#define W_TIMER_W_ADD4   0x08

#define W_TIMER_R_PASSWORD   0x09
#define W_TIMER_W_PASSWORD   0x0A
// CRtuAttrSyqcfg1 对话框

IMPLEMENT_DYNAMIC(CRtuAttrSyqcfg1, CDialog)

CRtuAttrSyqcfg1::CRtuAttrSyqcfg1(CWnd* pParent /*=NULL*/)
	: CDialog(CRtuAttrSyqcfg1::IDD, pParent)
//	, m_strAddress1(_T(""))
// 	, m_strAddress2(_T(""))
//	, m_strAddress3(_T(""))
// 	, m_strAddress4(_T(""))
{
//		m_iPassword = 0;
}

CRtuAttrSyqcfg1::~CRtuAttrSyqcfg1()
{
}

void CRtuAttrSyqcfg1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_COMMTYPE1, m_commtype1);
	DDX_Control(pDX, IDC_COMBO_COMMTYPE2, m_commtype2);
	DDX_Control(pDX, IDC_COMBO_COMMTYPE3, m_commtype3);
	DDX_Control(pDX, IDC_COMBO_COMMTYPE4, m_commtype4);
	DDX_Control(pDX, IDC_BTN_READ1, m_BTN_Read1);
	DDX_Control(pDX, IDC_BTN_READ2, m_BTN_Read2);
	DDX_Control(pDX, IDC_BTN_READ3, m_BTN_Read3);
	DDX_Control(pDX, IDC_BTN_READ4, m_BTN_Read4);
	DDX_Control(pDX, IDC_BTN_CFG1, m_BTN_Cfg1);
	DDX_Control(pDX, IDC_BTN_CFG2, m_BTN_Cfg2);
	DDX_Control(pDX, IDC_BTN_CFG3, m_BTN_Cfg3);
	DDX_Control(pDX, IDC_BTN_CFG4, m_BTN_Cfg4);
	DDX_Control(pDX, IDC_BTN_WRITE_PASSWORD, m_BTN_WritePassword);

//	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_iPassword);
//	DDV_MinMaxUInt(pDX, m_iPassword, 0,65535);
	//DDX_Text(pDX, IDC_EDIT_CENTER_ADDR, m_centercode);
	//DDV_MinMaxShort(pDX, m_centercode, 0, 99);
	//DDX_Text(pDX, IDC_EDIT_RTU_ADDR, m_syqStationCode);
//	DDV_MinMaxUInt(pDX, m_syqStationCode, 0, 999999999);

	//	DDX_Text(pDX, IDC_EDIT_ADDRESS1, m_strAddress1);
	//	DDX_Text(pDX, IDC_EDIT_ADDRESS2, m_strAddress2);
	//	DDX_Text(pDX, IDC_EDIT_ADDRESS3, m_strAddress3);
	//	DDX_Text(pDX, IDC_EDIT_ADDRESS4, m_strAddress4);
	DDX_Control(pDX, IDC_EDIT_ADDRESS1, m_Address1);
	DDX_Control(pDX, IDC_EDIT_ADDRESS2, m_Address2);
	DDX_Control(pDX, IDC_EDIT_ADDRESS3, m_Address3);
	DDX_Control(pDX, IDC_EDIT_ADDRESS4, m_Address4);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_Password);
	DDX_Control(pDX, IDC_EDIT_CENTER_ADDR, m_ECenterCode);
	DDX_Control(pDX, IDC_EDIT_RTU_ADDR, m_EStCode);
}


BEGIN_MESSAGE_MAP(CRtuAttrSyqcfg1, CDialog)
	ON_BN_CLICKED(IDC_BTN_CFG1, &CRtuAttrSyqcfg1::OnBnClickedBtnCfg1)
	ON_BN_CLICKED(IDC_BTN_READ1, &CRtuAttrSyqcfg1::OnBnClickedBtnRead1)
	ON_BN_CLICKED(IDC_BTN_CFG2, &CRtuAttrSyqcfg1::OnBnClickedBtnCfg2)
	ON_BN_CLICKED(IDC_BTN_READ2, &CRtuAttrSyqcfg1::OnBnClickedBtnRead2)
	ON_BN_CLICKED(IDC_BTN_CFG3, &CRtuAttrSyqcfg1::OnBnClickedBtnCfg3)
	ON_BN_CLICKED(IDC_BTN_READ3, &CRtuAttrSyqcfg1::OnBnClickedBtnRead3)
	ON_BN_CLICKED(IDC_BTN_CFG4, &CRtuAttrSyqcfg1::OnBnClickedBtnCfg4)
	ON_BN_CLICKED(IDC_BTN_READ4, &CRtuAttrSyqcfg1::OnBnClickedBtnRead4)
	ON_EN_CHANGE(IDC_EDIT_ADDRESS1, &CRtuAttrSyqcfg1::OnEnChangeEditAddress1)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_WRITE_PASSWORD, &CRtuAttrSyqcfg1::OnBnClickedBtnWritePassword)
END_MESSAGE_MAP()

BOOL CRtuAttrSyqcfg1::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	//m_strAddress1 = "255.255.255.255";


	if (m_commtype1.GetSafeHwnd())
	{
		m_commtype1.ResetContent();
		int appType_idx = 0;
		appType_idx = m_commtype1.AddString("1-短信");
		m_commtype1.SetItemData(appType_idx, (BYTE)1);

		appType_idx = m_commtype1.AddString("2-IP ");
		m_commtype1.SetItemData(appType_idx, (BYTE)2);
	
		appType_idx = m_commtype1.AddString("3-北斗");
		m_commtype1.SetItemData(appType_idx, (BYTE)3);

		appType_idx = m_commtype1.AddString("4-海事卫星");
		m_commtype1.SetItemData(appType_idx, (BYTE)4);

	}
	if (m_commtype2.GetSafeHwnd())
	{
		m_commtype2.ResetContent();
		int appType_idx = 0;
		appType_idx = m_commtype2.AddString("1-短信");
		m_commtype2.SetItemData(appType_idx, (BYTE)1);

		appType_idx = m_commtype2.AddString("2-IP ");
		m_commtype2.SetItemData(appType_idx, (BYTE)2);

		appType_idx = m_commtype2.AddString("3-北斗");
		m_commtype2.SetItemData(appType_idx, (BYTE)3);

		appType_idx = m_commtype2.AddString("4-海事卫星");
		m_commtype2.SetItemData(appType_idx, (BYTE)4);

	}
	if (m_commtype3.GetSafeHwnd())
	{
		m_commtype3.ResetContent();
		int appType_idx = 0;
		appType_idx = m_commtype3.AddString("1-短信");
		m_commtype3.SetItemData(appType_idx, (BYTE)1);

		appType_idx = m_commtype3.AddString("2-IP ");
		m_commtype3.SetItemData(appType_idx, (BYTE)2);

		appType_idx = m_commtype3.AddString("3-北斗");
		m_commtype3.SetItemData(appType_idx, (BYTE)3);

		appType_idx = m_commtype3.AddString("4-海事卫星");
		m_commtype3.SetItemData(appType_idx, (BYTE)4);

	}
	if (m_commtype4.GetSafeHwnd())
	{
		m_commtype4.ResetContent();
		int appType_idx = 0;
		appType_idx = m_commtype4.AddString("1-短信");
		m_commtype4.SetItemData(appType_idx, (BYTE)1);

		appType_idx = m_commtype4.AddString("2-IP ");
		m_commtype4.SetItemData(appType_idx, (BYTE)2);

		appType_idx = m_commtype4.AddString("3-北斗");
		m_commtype4.SetItemData(appType_idx, (BYTE)3);

		appType_idx = m_commtype4.AddString("4-海事卫星");
		m_commtype4.SetItemData(appType_idx, (BYTE)4);

	}
	//m_CenterCode.Get
	
	//m_BTN_Cfg1.EnableWindow(FALSE);
	m_commtype1.SetCurSel(0);
	m_commtype2.SetCurSel(0);
	m_commtype3.SetCurSel(0);
	m_commtype4.SetCurSel(0);
    
	return TRUE;
}

void CRtuAttrSyqcfg1::SetParentDlg(CRtuPropertyDlg* pDlg)
{
	m_pParentDlg = pDlg;	
}

void CRtuAttrSyqcfg1::SetEnable(bool b)
{

	GetDlgItem(IDC_EDIT_ADDRESS1)->EnableWindow(b);
	GetDlgItem(IDC_EDIT_ADDRESS2)->EnableWindow(b);
	GetDlgItem(IDC_EDIT_ADDRESS3)->EnableWindow(b);
	GetDlgItem(IDC_EDIT_ADDRESS4)->EnableWindow(b);
	
	m_commtype1.EnableWindow(b);
	m_commtype2.EnableWindow(b);
	m_commtype3.EnableWindow(b);
	m_commtype4.EnableWindow(b);

	m_BTN_WritePassword.EnableWindow(b);
	m_BTN_Read1.EnableWindow(b);
	m_BTN_Read2.EnableWindow(b);
	m_BTN_Read3.EnableWindow(b);
	m_BTN_Read4.EnableWindow(b);
	m_BTN_Cfg1.EnableWindow(b);
	m_BTN_Cfg2.EnableWindow(b);
	m_BTN_Cfg3.EnableWindow(b);
	m_BTN_Cfg4.EnableWindow(b);
}
void CRtuAttrSyqcfg1::Update()
{
	CString temp ="";
	if(m_pParentDlg)
		m_pSubStation = m_pParentDlg->GetRtuStation();	
	if(m_pSubStation)
	{
//		m_centercode = m_pSubStation->GetSYQCenterCode();
//		m_syqStationCode = m_pSubStation->GetSYQStCode();
//		m_iPassword  =  m_pSubStation->GetSYQPassword();
		temp.Format("%d",m_pSubStation->GetSYQCenterCode());
		m_ECenterCode.SetWindowText(temp);
		temp.Format("%u",m_pSubStation->GetSYQStCode());
		m_EStCode.SetWindowText(temp);
		temp.Format("%d",m_pSubStation->GetSYQPassword());
		m_Password.SetWindowText(temp);


//		UpdateData(FALSE);
	    m_commtype1.SetCurSel(m_pSubStation->m_syqipconfigdata.m_commtype1-1);
		m_commtype2.SetCurSel(m_pSubStation->m_syqipconfigdata.m_commtype2-1);
 		m_commtype3.SetCurSel(m_pSubStation->m_syqipconfigdata.m_commtype3-1);
		m_commtype4.SetCurSel(m_pSubStation->m_syqipconfigdata.m_commtype4-1);

		m_Address1.SetWindowText(m_pSubStation->m_syqipconfigdata.m_strip1);
		m_Address2.SetWindowText(m_pSubStation->m_syqipconfigdata.m_strip2);
		m_Address3.SetWindowText(m_pSubStation->m_syqipconfigdata.m_strip3);
		m_Address4.SetWindowText(m_pSubStation->m_syqipconfigdata.m_strip4);
	}

	

}

void CRtuAttrSyqcfg1::Updatedata(int sel)
{
//	DWORD dwip;
	CString temp ="";
	if (m_pParentDlg)
		m_pSubStation = m_pParentDlg->GetRtuStation();	

	switch(sel)
	{
	case syq_read_address1:
	case syq_config_address1:
		m_Address1.SetWindowText(m_pSubStation->m_syqipconfigdata.m_strip1);  
		m_commtype1.SetCurSel(m_pSubStation->m_syqipconfigdata.m_commtype1-1);
		break;
	case syq_read_address2:
	case syq_config_address2:
		m_Address1.SetWindowText(m_pSubStation->m_syqipconfigdata.m_strip2);  
		m_commtype1.SetCurSel(m_pSubStation->m_syqipconfigdata.m_commtype2-1);
		break;
	case syq_read_address3:
	case syq_config_address3:
		m_Address1.SetWindowText(m_pSubStation->m_syqipconfigdata.m_strip3);  
		m_commtype1.SetCurSel(m_pSubStation->m_syqipconfigdata.m_commtype3-1);
		break;
	case syq_read_address4:
	case syq_config_address4:
		m_Address1.SetWindowText(m_pSubStation->m_syqipconfigdata.m_strip4);  
		m_commtype1.SetCurSel(m_pSubStation->m_syqipconfigdata.m_commtype4-1);
		break;
	case syq_config_password:
		//m_Password.SetWindowText(m_pSubStation->m_iPassword);
		//m_iPassword = m_pSubStation->m_iPassword;
		temp.Format("%d",m_pSubStation->GetSYQPassword());
		m_Password.SetWindowText(temp);
		break;
	}

	UpdateData(FALSE);
}
void CRtuAttrSyqcfg1::UpdateDlgMessage(int attr, bool result)
{
	m_bResult = result;

	if(attr == syq_read_address1)
	{
		SendMessage(WM_TIMER,W_TIMER_R_ADD1, NULL);
	}
	else if(attr == syq_config_address1)
	{
		SendMessage(WM_TIMER,W_TIMER_W_ADD1, NULL);
	}
	else if(attr == syq_read_address2)
	{
		SendMessage(WM_TIMER,W_TIMER_R_ADD2, NULL);
	}
	else if(attr == syq_config_address2)
	{
		SendMessage(WM_TIMER,W_TIMER_W_ADD2, NULL);
	}
	else if(attr == syq_read_address3)
	{
		SendMessage(WM_TIMER,W_TIMER_R_ADD3, NULL);
	}
	else if(attr == syq_config_address3)
	{
		SendMessage(WM_TIMER,W_TIMER_W_ADD3, NULL);
	}
	else if(attr == syq_read_address4)
	{
		SendMessage(WM_TIMER,W_TIMER_R_ADD4, NULL);
	}
	else if(attr == syq_config_address4)
	{
		SendMessage(WM_TIMER,W_TIMER_W_ADD4, NULL);
	}
	else if(attr == syq_config_password)
	{
		SendMessage(WM_TIMER,W_TIMER_W_PASSWORD, NULL);
	}	
}

// CRtuAttrSyqcfg1 消息处理程序

void CRtuAttrSyqcfg1::OnBnClickedBtnRead1()
{
	// TODO: 在此添加控件通知处理程序代码
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans())
		{
			SetEnable(FALSE);
			m_bResult = FALSE;
			CString straddr = "";
			m_Address1.GetWindowText(straddr);
			short comtype;
			comtype = m_commtype1.GetCurSel()+1;

			CSYQFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息
			//添加数据打包
			m_pSubStation->GetSysDataTrans_Syq()->SYQ_EN_41H(&appFrame,straddr,comtype,1);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_R_ADD1, nElapse, NULL);
		}
	}
}

void CRtuAttrSyqcfg1::OnBnClickedBtnCfg1()//修改目的地址1
{
/*	CString str ="";
	int len = 0, datalen = 0;
	unsigned char *buf = new unsigned char[5];
	buf[0] = 'P';
	buf[1] = '0';
	buf[2] = '1';
	buf[3] = 3;
	buf[4] = 5;

	m_pSubStation->GetSysDataTrans_Syq()->GetPropertyString(buf, 5,str,len,datalen);
*/
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans())
		{
			SetEnable(FALSE);
			m_bResult = FALSE;
			CString straddr = "";
			m_Address1.GetWindowText(straddr);
			short comtype;
			comtype = m_commtype1.GetCurSel()+1;

			CSYQFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息
			//添加数据打包
			m_pSubStation->GetSysDataTrans_Syq()->SYQ_EN_40H(&appFrame,straddr,comtype,1);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_W_ADD1, nElapse, NULL);
		}
	}


}

void CRtuAttrSyqcfg1::OnBnClickedBtnRead2()//查询目的地址1
{
	// TODO: 在此添加控件通知处理程序代码
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans())
		{
			SetEnable(FALSE);
			m_bResult = FALSE;
			CString straddr = "";
			m_Address1.GetWindowText(straddr);
			short comtype;
			comtype = m_commtype1.GetCurSel()+1;

			CSYQFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息
			//添加数据打包
			m_pSubStation->GetSysDataTrans_Syq()->SYQ_EN_41H(&appFrame,straddr,comtype,2);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_R_ADD2, nElapse, NULL);
		}
	}
}

void CRtuAttrSyqcfg1::OnBnClickedBtnCfg2()//修改目的地址2
{

	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans())
		{
			SetEnable(FALSE);
			m_bResult = FALSE;
			CString straddr = "";
			m_Address1.GetWindowText(straddr);
			short comtype;
			comtype = m_commtype1.GetCurSel()+1;

			CSYQFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息
			//添加数据打包
			m_pSubStation->GetSysDataTrans_Syq()->SYQ_EN_40H(&appFrame,straddr,comtype,2);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_W_ADD2, nElapse, NULL);
		}
	}


}

void CRtuAttrSyqcfg1::OnBnClickedBtnRead3()//查询目的地址3
{
	// TODO: 在此添加控件通知处理程序代码
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans())
		{
			SetEnable(FALSE);
			m_bResult = FALSE;
			CString straddr = "";
			m_Address1.GetWindowText(straddr);
			short comtype;
			comtype = m_commtype1.GetCurSel()+1;

			CSYQFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息
			//添加数据打包
			m_pSubStation->GetSysDataTrans_Syq()->SYQ_EN_41H(&appFrame,straddr,comtype,3);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_R_ADD3, nElapse, NULL);
		}
	}
}

void CRtuAttrSyqcfg1::OnBnClickedBtnCfg3()//修改目的地址3
{

	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans())
		{
			SetEnable(FALSE);
			m_bResult = FALSE;
			CString straddr = "";
			m_Address1.GetWindowText(straddr);
			short comtype;
			comtype = m_commtype1.GetCurSel()+1;

			CSYQFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息
			//添加数据打包
			m_pSubStation->GetSysDataTrans_Syq()->SYQ_EN_40H(&appFrame,straddr,comtype,3);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_W_ADD3, nElapse, NULL);
		}
	}


}

void CRtuAttrSyqcfg1::OnBnClickedBtnRead4()//查询目的地址4
{
	// TODO: 在此添加控件通知处理程序代码
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans())
		{
			SetEnable(FALSE);
			m_bResult = FALSE;
			CString straddr = "";
			m_Address1.GetWindowText(straddr);
			short comtype;
			comtype = m_commtype1.GetCurSel()+1;

			CSYQFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息
			//添加数据打包
			m_pSubStation->GetSysDataTrans_Syq()->SYQ_EN_41H(&appFrame,straddr,comtype,4);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_R_ADD4, nElapse, NULL);
		}
	}
}

void CRtuAttrSyqcfg1::OnBnClickedBtnCfg4()//修改目的地址2
{

	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans())
		{
			SetEnable(FALSE);
			m_bResult = FALSE;
			CString straddr = "";
			m_Address1.GetWindowText(straddr);
			short comtype;
			comtype = m_commtype1.GetCurSel()+1;

			CSYQFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息
			//添加数据打包
			m_pSubStation->GetSysDataTrans_Syq()->SYQ_EN_40H(&appFrame,straddr,comtype,4);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_W_ADD4, nElapse, NULL);
		}
	}


}




void CRtuAttrSyqcfg1::OnEnChangeEditAddress1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运   算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	m_BTN_Cfg1.EnableWindow(TRUE);
	UpdateData(TRUE);
//	CString temp   = m_strAddress1;
}

void CRtuAttrSyqcfg1::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString strLog= "";
	_Log_level_type LogLevel = Normal_Log_Message;

	switch (nIDEvent)
	{
	case W_TIMER_R_ADD1:
		{
			if (m_bResult == TRUE)
				strLog = "目标地址1查询 成功!";
			else if ( m_bResult == FALSE)
			{
				strLog = "目标地址1查询 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_bResult == 0xff)
			{
				strLog = "目标地址1查询 错误!";
				LogLevel = Warning_log_Message;
			}
		}
		break;
	case W_TIMER_W_ADD1:
		{
			if (m_bResult == TRUE)
				strLog = "目标地址1修改 成功!";
			else if ( m_bResult == FALSE)
			{
				strLog = "目标地址1修改 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_bResult == 0xff)
			{
				strLog = "目标地址1修改 错误!";
				LogLevel = Warning_log_Message;
			}
		}
		break;
	case W_TIMER_R_ADD2:
		{
			if (m_bResult == TRUE)
				strLog = "目标地址2查询 成功!";
			else if ( m_bResult == FALSE)
			{
				strLog = "目标地址2查询 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_bResult == 0xff)
			{
				strLog = "目标地址2查询 错误!";
				LogLevel = Warning_log_Message;
			}
		}
		break;
	case W_TIMER_W_ADD2:
		{
			if (m_bResult == TRUE)
				strLog = "目标地址2修改 成功!";
			else if ( m_bResult == FALSE)
			{
				strLog = "目标地址2修改 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_bResult == 0xff)
			{
				strLog = "目标地址2修改 错误!";
				LogLevel = Warning_log_Message;
			}
		}
		break;
	case W_TIMER_R_ADD3:
		{
			if (m_bResult == TRUE)
				strLog = "目标地址3查询 成功!";
			else if ( m_bResult == FALSE)
			{
				strLog = "目标地址3查询 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_bResult == 0xff)
			{
				strLog = "目标地址3查询 错误!";
				LogLevel = Warning_log_Message;
			}
		}
		break;
	case W_TIMER_W_ADD3:
		{
			if (m_bResult == TRUE)
				strLog = "目标地址3修改 成功!";
			else if ( m_bResult == FALSE)
			{
				strLog = "目标地址3修改 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_bResult == 0xff)
			{
				strLog = "目标地址3修改 错误!";
				LogLevel = Warning_log_Message;
			}
		}
		break;
	case W_TIMER_R_ADD4:
		{
			if (m_bResult == TRUE)
				strLog = "目标地址4查询 成功!";
			else if ( m_bResult == FALSE)
			{
				strLog = "目标地址4查询 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_bResult == 0xff)
			{
				strLog = "目标地址4查询 错误!";
				LogLevel = Warning_log_Message;
			}
		}
		break;
	case W_TIMER_W_ADD4:
		{
			if (m_bResult == TRUE)
				strLog = "目标地址4修改 成功!";
			else if ( m_bResult == FALSE)
			{
				strLog = "目标地址4修改 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_bResult == 0xff)
			{
				strLog = "目标地址1修改 错误!";
				LogLevel = Warning_log_Message;
			}
		}
		break;
	case W_TIMER_W_PASSWORD:
		{
			if (m_bResult == TRUE)
				strLog = "密码修改 成功!";
			else if ( m_bResult == FALSE)
			{
				strLog = "密码修改 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_bResult == 0xff)
			{
				strLog = "密码修改 错误!";
				LogLevel = Warning_log_Message;
			}
		}
		break;
	}

	KillTimer(nIDEvent);
	SetEnable(TRUE);
	if (m_pParentDlg && m_pParentDlg->GetSafeHwnd())
		m_pParentDlg->AddLog(strLog,(PSYSTEMTIME)NULL,LogLevel);

	if (m_pSubStation)
		m_pSubStation->InsertLogMessage(strLog,LogLevel,NULL);
}




void CRtuAttrSyqcfg1::OnBnClickedBtnWritePassword()
{
	// TODO: 在此添加控件通知处理程序代码
	if( m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans())
		{
			SetEnable(FALSE);
			m_bResult = FALSE;
			CString strpass = "";
			int  ps = 0;
			m_Password.GetWindowText(strpass);
			ps = atoi(strpass);
			CSYQFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息
			//添加数据打包
			m_pSubStation->GetSysDataTrans_Syq()->SYQ_EN_46H(&appFrame,ps);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_W_PASSWORD, nElapse, NULL);
		}
	}

}
