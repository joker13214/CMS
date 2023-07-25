// RtuAttrSyqcfg3.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuAttrSyqcfg3.h"


// CRtuAttrSyqcfg3 对话框
#define W_TIMER_R   0x01
#define W_TIMER_W   0x02
#define W_TIMER_INIT 0x03
#define W_TIMER_CHECKTIME 0x04
#define W_TIMER_CALLDATA 0x05


IMPLEMENT_DYNAMIC(CRtuAttrSyqcfg3, CDialog)

CRtuAttrSyqcfg3::CRtuAttrSyqcfg3(CWnd* pParent /*=NULL*/)
	: CDialog(CRtuAttrSyqcfg3::IDD, pParent)
{

}

CRtuAttrSyqcfg3::~CRtuAttrSyqcfg3()
{
}

void CRtuAttrSyqcfg3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_WATER_LEVEL_UP, m_cfg_40);
	DDX_Control(pDX, IDC_EDIT_WATER_LEVEL_DOWN, m_cfg_41);
	DDX_Control(pDX, IDC_EDIT_WATER_PRESSURE_UP, m_cfg_42);
	DDX_Control(pDX, IDC_EDIT_WATER_PRESSURE_DOWN, m_cfg_43);
	DDX_Control(pDX, IDC_EDIT_WATER_VALUE, m_cfg_69);
	DDX_Control(pDX, IDC_READBTN, m_ReadBtn);
	DDX_Control(pDX, IDC_WRITEBTN, m_WriteBtn);
	DDX_Control(pDX, IDC_EDIT1, m_Static);
	DDX_Control(pDX, IDC_EDIT2, m_Static2);
	DDX_Control(pDX, IDC_BUTTON1, m_Btn_Cancel);
	DDX_Control(pDX, IDC_CHECKTIME_BTN, m_BtnCheckTime);
}


BEGIN_MESSAGE_MAP(CRtuAttrSyqcfg3, CDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_READBTN, &CRtuAttrSyqcfg3::OnBnClickedReadbtn)
	ON_BN_CLICKED(IDC_WRITEBTN, &CRtuAttrSyqcfg3::OnBnClickedWritebtn)
	ON_BN_CLICKED(IDC_BUTTON1, &CRtuAttrSyqcfg3::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CHECKTIME_BTN, &CRtuAttrSyqcfg3::OnBnClickedChecktimeBtn)
END_MESSAGE_MAP()


// CRtuAttrSyqcfg3 消息处理程序

BOOL CRtuAttrSyqcfg3::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;
}




void CRtuAttrSyqcfg3::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString strLog= "";
	_Log_level_type LogLevel = Normal_Log_Message;

	switch (nIDEvent)
	{
	case W_TIMER_R:
		{
			if (m_bResult == TRUE)
				strLog = "查询遥测站参数配置表3 成功!";
			else if ( m_bResult == FALSE)
			{
				strLog = "查询遥测站参数配置表3 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_bResult == 0xff)
			{
				strLog = "查询遥测站参数配置表3 错误!";
				LogLevel = Warning_log_Message;
			}
		}
		break;
	case W_TIMER_W:
		{
			if (m_bResult == TRUE)
				strLog = "修改遥测站参数配置表3 成功!";
			else if ( m_bResult == FALSE)
			{
				strLog = "修改遥测站参数配置表3 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_bResult == 0xff)
			{
				strLog = "修改查询遥测站参数配置表3 错误!";
				LogLevel = Warning_log_Message;
			}
		}
		break;
	case W_TIMER_INIT:
		{
			if (m_bResult == TRUE)
			{
				m_Static.SetWindowText("成功");
				strLog = "固态存储器初始化 成功!";
			}
			else if ( m_bResult == FALSE)
			{
				m_Static.SetWindowText("超时");
				strLog = "固态存储器初始化 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_bResult == 0xff)
			{
				m_Static.SetWindowText("错误");
				strLog = "固态存储器初始化 错误!";
				LogLevel = Warning_log_Message;
			}
		}
		break;
	case W_TIMER_CHECKTIME:
		{
			if (m_bResult == TRUE)
			{
				m_Static2.SetWindowText("成功");
				strLog = "设置遥测站时钟 成功!";
			}
			else if ( m_bResult == FALSE)
			{
				m_Static2.SetWindowText("超时");
				strLog = "设置遥测站时钟 超时!";
				LogLevel = Warning_log_Message;
			}
			else if (m_bResult == 0xff)
			{
				m_Static2.SetWindowText("错误");
				strLog = "设置遥测站时钟 错误!";
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

	CDialog::OnTimer(nIDEvent);
}


void CRtuAttrSyqcfg3::SetParentDlg(CRtuPropertyDlg* pDlg)
{
	m_pParentDlg = pDlg;	
}

void CRtuAttrSyqcfg3::Update()
{
	CString temp ="";
	if(m_pParentDlg)
		m_pSubStation = m_pParentDlg->GetRtuStation();		
	if(m_pSubStation)
	{
		CString tempstr="";
		BOOL bfind  = false;
		for (int i=0;i<10;i++)
		{
			if((m_pSubStation->m_syqconfig3data.CFG_40[i]!='0')||bfind)
			{
				bfind = true;
				tempstr = tempstr + m_pSubStation->m_syqconfig3data.CFG_40[i];
				if(i == 6)
					tempstr = tempstr +".";
			}
		}

		if(tempstr =="")
			tempstr ="0";
       m_cfg_40.SetWindowText(tempstr);


	   tempstr="";
		bfind  = false;
	   for (int i=0;i<10;i++)
	   {
		   if((m_pSubStation->m_syqconfig3data.CFG_41[i]!='0')||bfind)
		   {
			   bfind = true;
			   tempstr = tempstr + m_pSubStation->m_syqconfig3data.CFG_41[i];
			   if(i == 6)
				   tempstr = tempstr +".";
		   }
	   }
	   m_cfg_41.SetWindowText(tempstr);


	   tempstr="";
	   bfind  = false;
	   for (int i=0;i<6;i++)
	   {
		   if((m_pSubStation->m_syqconfig3data.CFG_42[i]!='0')||bfind)
		   {
			   bfind = true;
			   tempstr = tempstr + m_pSubStation->m_syqconfig3data.CFG_42[i];
			   if(i == 3)
				   tempstr = tempstr +".";
		   }
	   }
	   m_cfg_42.SetWindowText(tempstr);


	   tempstr="";
		bfind  = false;
	   for (int i=0;i<6;i++)
	   {
		   if((m_pSubStation->m_syqconfig3data.CFG_43[i]!='0')||bfind)
		   {
			   bfind = true;
			   tempstr = tempstr + m_pSubStation->m_syqconfig3data.CFG_43[i];
			   if(i == 3)
				   tempstr = tempstr +".";
		   }
	   }
	   m_cfg_43.SetWindowText(tempstr);


	   tempstr="";
	   bfind  = false;
	   for (int i=0;i<14;i++)
	   {
		   if((m_pSubStation->m_syqconfig3data.CFG_69[i]!='0')||bfind)
		   {
			   bfind = true;
			   tempstr = tempstr + m_pSubStation->m_syqconfig3data.CFG_69[i];
			   if(i == 10)
				   tempstr = tempstr +".";
		   }
	   }
	   m_cfg_69.SetWindowText(tempstr);

	}

}

void CRtuAttrSyqcfg3::SetEnable(bool b)
{
	m_cfg_40.EnableWindow(b);
	m_cfg_41.EnableWindow(b);
	m_cfg_42.EnableWindow(b);
	m_cfg_43.EnableWindow(b);
	m_cfg_69.EnableWindow(b);

	m_ReadBtn.EnableWindow(b);
	
}
void CRtuAttrSyqcfg3::UpdateDlgMessage(int attr, bool result)
{
	m_bResult = result;

	if(attr == syq_config3_Write)
	{
		SendMessage(WM_TIMER,W_TIMER_W, NULL);
	}
	else if(attr == syq_config3_Read)
	{
		SendMessage(WM_TIMER,W_TIMER_R, NULL);
	}
	else if(attr == syq_init_memory)
	{ 
		SendMessage(WM_TIMER,W_TIMER_INIT, NULL);
	}
	else if(attr == syq_check_time)
	{ 
		SendMessage(WM_TIMER,W_TIMER_CHECKTIME, NULL);
	}

}

void CRtuAttrSyqcfg3::StringToChar(CString str,char *buf, int len1, int len2)
{
	int Varlen = str.GetLength();
	char Buf[20];
	memset(Buf, 0, 19);
	memcpy(Buf, str.GetBuffer(19), 19);
		
	int d1= 0,d2 = 0;
	for (int i=0; i<Varlen; i++)
	{
		if(Buf[i]!='.')
			d1++;
		else
			break;
	}
	
	if(d1<= len1)
	{
		d2 = len1 - d1;
		
		for(int i = 0; i<d2 ; i++)
			buf[i] = '0';
		for(int j = d2; j<7; j++)
			buf[j] = Buf[j-d2];

		if(Varlen >d1+1)
		{
			for(int i = 0 ;i<(Varlen-1-d1);i++)
				buf[len1+i] = Buf[d1+1+i];

			for(int i = 0 ;i<(len2 -(Varlen-1-d1));i++)
				buf[len1+(Varlen-1-d1)+i] = '0';

		}
		else
		{
			for(int i = 0 ;i< len2 ;i++)
				buf[len1+i] = '0';
		}

	}
}


void CRtuAttrSyqcfg3::OnBnClickedReadbtn()
{
	// TODO: 在此添加控件通知处理程序代码
	CString temp = "";
	_SYQ_CONFIG_PACKET3 pack;

	if(m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans())
		{

		m_cfg_40.GetWindowText(temp);
		StringToChar(temp,pack.CFG_40,7,3);
		m_cfg_41.GetWindowText(temp);
		StringToChar(temp,pack.CFG_41,7,3);

		m_cfg_42.GetWindowText(temp);
		StringToChar(temp,pack.CFG_42,4,2);
		m_cfg_43.GetWindowText(temp);
			StringToChar(temp,pack.CFG_43,4,2);
	
		m_cfg_69.GetWindowText(temp);
		StringToChar(temp,pack.CFG_69,11,3);

	
		CSYQFrameFormat appFrame;
		m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息
	//添加数据打包
		m_pSubStation->GetSysDataTrans_Syq()->SYQ_EN_45H(&appFrame,pack);

		UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
		SetTimer(W_TIMER_W, nElapse, NULL);
		SetEnable(FALSE);
		}
	}
	
}

void CRtuAttrSyqcfg3::OnBnClickedWritebtn()
{
	// TODO: 在此添加控件通知处理程序代码
	CString temp = "";
	_SYQ_CONFIG_PACKET3 pack;

	if(m_pSubStation)
	{
		if (m_pSubStation->blEnbaleTrans())
		{
			m_cfg_40.GetWindowText(temp);
			StringToChar(temp,pack.CFG_40,7,3);
			m_cfg_41.GetWindowText(temp);
			StringToChar(temp,pack.CFG_41,7,3);

			m_cfg_42.GetWindowText(temp);
			StringToChar(temp,pack.CFG_42,4,2);
			m_cfg_43.GetWindowText(temp);
			StringToChar(temp,pack.CFG_43,4,2);

			m_cfg_69.GetWindowText(temp);
			StringToChar(temp,pack.CFG_69,11,3);


			CSYQFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息
			//添加数据打包
			m_pSubStation->GetSysDataTrans_Syq()->SYQ_EN_45H(&appFrame,pack);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_R, nElapse, NULL);
			SetEnable(FALSE);
		}
	}
}

void CRtuAttrSyqcfg3::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pSubStation->blEnbaleTrans())
	{
	m_Static.SetWindowText("初始化");
	SetEnable(FALSE);

	CSYQFrameFormat appFrame;
	m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息
	//添加数据打包
	m_pSubStation->GetSysDataTrans_Syq()->SYQ_EN_47H(&appFrame);

	UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
	SetTimer(W_TIMER_INIT, nElapse, NULL);

	}
}

void CRtuAttrSyqcfg3::OnBnClickedChecktimeBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pSubStation->blEnbaleTrans())
	{
	m_Static2.SetWindowText("下发时钟");
	SetEnable(FALSE);

	CSYQFrameFormat appFrame;
	m_pSubStation->BuildBasicPacket(appFrame);  //生成初始的下发帧报文帧数据信息
	//添加数据打包
	m_pSubStation->GetSysDataTrans_Syq()->SYQ_EN_49H(&appFrame);

	UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
	SetTimer(W_TIMER_CHECKTIME, nElapse, NULL);
	}
	

}
