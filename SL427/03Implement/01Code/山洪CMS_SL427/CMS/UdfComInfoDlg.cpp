// UdfComInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "UdfComInfoDlg.h"


// CUdfComInfoDlg 对话框

IMPLEMENT_DYNAMIC(CUdfComInfoDlg, CDialog)

CUdfComInfoDlg::CUdfComInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUdfComInfoDlg::IDD, pParent)
{
	m_strComNo = "";
	m_nBaund   = 0;
	m_nDataBits = 8;
	m_nStopBits = 0;
	m_nParity = 0;
}

CUdfComInfoDlg::~CUdfComInfoDlg()
{
}

void CUdfComInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_COMNO, m_wndCmbComNo);
	DDX_Control(pDX, IDC_CMB_BAUND, m_wndCmbBaund);
	DDX_Control(pDX, IDC_CMB_DATABITS, m_wndCmbDataBits);
	DDX_Control(pDX, IDC_CMB_STOPBITS, m_wndCmbStopBits);
	DDX_Control(pDX, IDC_CMB_PARIRTY, m_wndCmbParirty);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CUdfComInfoDlg, CDialog)
END_MESSAGE_MAP()


// CUdfComInfoDlg 消息处理程序

BOOL CUdfComInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

    bool bFind = false;
	if (m_wndCmbStopBits.GetSafeHwnd())
	{
		m_wndCmbStopBits.ResetContent();
		int nidx = 0;

		nidx = m_wndCmbStopBits.AddString("1位停止位");
		m_wndCmbStopBits.SetItemData(nidx,ONESTOPBIT);

		nidx = m_wndCmbStopBits.AddString("1.5位停止位");
		m_wndCmbStopBits.SetItemData(nidx,ONE5STOPBITS);

		nidx = m_wndCmbStopBits.AddString("2位停止位");
		m_wndCmbStopBits.SetItemData(nidx,TWOSTOPBITS);

		if (m_wndCmbStopBits.GetCount())
		{
			for (int  i = 0; i < m_wndCmbStopBits.GetCount(); i++)
			{
				if (m_wndCmbStopBits.GetItemData(i) == m_nStopBits)
				{
					bFind = true;
					m_wndCmbStopBits.SetCurSel(i);
					break;
				}
			}
			if (!bFind )
				m_wndCmbStopBits.SetCurSel(0);
		}
	}


	if (m_wndCmbDataBits.GetSafeHwnd())
	{
		m_wndCmbDataBits.ResetContent();

		int nidx = 0;
		nidx = m_wndCmbDataBits.AddString("8位");
		m_wndCmbDataBits.SetItemData(nidx,8);

		nidx = m_wndCmbDataBits.AddString("7位");
		m_wndCmbDataBits.SetItemData(nidx,7);

		if (m_wndCmbDataBits.GetCount())
		{
			bFind = false;
			for (int i = 0; i< m_wndCmbDataBits.GetCount(); i++)
			{
				if (m_wndCmbDataBits.GetItemData(i) == m_nDataBits)
				{
					bFind = true;
					m_wndCmbDataBits.SetCurSel(i);
					break;
				}
			}
			
			if (!bFind)
				m_wndCmbDataBits.SetCurSel(0);
		}

	}

	if (m_wndCmbParirty.GetSafeHwnd())
	{
		 m_wndCmbParirty.ResetContent();

		 int nIdx = 0;
		 nIdx = m_wndCmbParirty.AddString("无校验");
		 m_wndCmbParirty.SetItemData(nIdx,NOPARITY);

		 nIdx = m_wndCmbParirty.AddString("奇校验");
		 m_wndCmbParirty.SetItemData(nIdx,ODDPARITY);

		 nIdx = m_wndCmbParirty.AddString("偶校验");
		 m_wndCmbParirty.SetItemData(nIdx,EVENPARITY);

		 if (m_wndCmbParirty.GetCount())
		 {
			 bFind = false;
			 for (int i = 0; i < m_wndCmbParirty.GetCount(); i ++)
			 {
				 if (m_wndCmbParirty.GetItemData(i) == m_nParity)
				 {
					 bFind = true;
					 m_wndCmbParirty.SetCurSel(i);
					 break;
				 }
			 }
			 if (!bFind)
				 m_wndCmbParirty.SetCurSel(0);
		 }
	}

    if (m_wndCmbBaund)
	{
		m_wndCmbBaund.ResetContent();

		int nIdx = 0;

		nIdx = m_wndCmbBaund.AddString("110");
		nIdx = m_wndCmbBaund.AddString("300");
		nIdx = m_wndCmbBaund.AddString("600");
		nIdx = m_wndCmbBaund.AddString("1200");
		nIdx = m_wndCmbBaund.AddString("2400");
		nIdx = m_wndCmbBaund.AddString("4800");
		nIdx = m_wndCmbBaund.AddString("9600");
		nIdx = m_wndCmbBaund.AddString("14400");
		nIdx = m_wndCmbBaund.AddString("19200");
		nIdx = m_wndCmbBaund.AddString("38400");
		nIdx = m_wndCmbBaund.AddString("56000");
		nIdx = m_wndCmbBaund.AddString("57600");
		nIdx = m_wndCmbBaund.AddString("115200");
		nIdx = m_wndCmbBaund.AddString("128000");
		nIdx = m_wndCmbBaund.AddString("256000");	

		CString TextValue = "";
		TextValue.Format("%d",m_nBaund);
		m_wndCmbBaund.SetWindowText(TextValue);

	}

	if (m_wndCmbComNo.GetSafeHwnd())
	{
		m_wndCmbComNo.ResetContent();

		m_wndCmbComNo.AddString("COM1");
		m_wndCmbComNo.AddString("COM2");
		m_wndCmbComNo.AddString("COM3");

		m_wndCmbComNo.SetWindowText(m_strComNo);


	}



	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CUdfComInfoDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData(TRUE);
	m_wndCmbComNo.GetWindowText(m_strComNo);
	m_strComNo.TrimLeft();
	m_strComNo.TrimRight();
	if (m_strComNo.IsEmpty())
	{
		MessageBox("串口号不能为空,请重新输入","错误",MB_OK|MB_ICONERROR);
		return;
	}

	CString strTemp ="";
	m_wndCmbBaund.GetWindowText(strTemp);
	m_nBaund = atoi(strTemp);
	if (m_nBaund == 0)
	{
		MessageBox("波特率不能为空或零,请重新输入","错误",MB_OK|MB_ICONERROR);
		return;
	}


	m_nDataBits = (BYTE) m_wndCmbDataBits.GetItemData(m_wndCmbDataBits.GetCurSel());
	m_nStopBits = (BYTE) m_wndCmbStopBits.GetItemData(m_wndCmbStopBits.GetCurSel());
	m_nParity   = (BYTE) m_wndCmbParirty.GetItemData(m_wndCmbParirty.GetCurSel());


	CDialog::OnOK();
}
