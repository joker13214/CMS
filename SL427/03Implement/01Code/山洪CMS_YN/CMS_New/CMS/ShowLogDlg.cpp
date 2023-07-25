// ShowLogDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CMS.h"
#include "ShowLogDlg.h"


// CShowLogDlg �Ի���

IMPLEMENT_DYNAMIC(CShowLogDlg, CDialog)

CShowLogDlg::CShowLogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShowLogDlg::IDD, pParent)
{

}

CShowLogDlg::~CShowLogDlg()
{
}

void CShowLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_RICHEDIT_LOGMSG, m_wndRichEditCtrl);
	DDX_Control(pDX, IDC_EDIT_DATE, m_wndEditDate);
	DDX_Control(pDX, IDC_EDIT_DMASSAGE, m_wndEditMessage);
}


BEGIN_MESSAGE_MAP(CShowLogDlg, CDialog)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CShowLogDlg ��Ϣ�������

void CShowLogDlg::OnClose()
{

	//CDialog::OnClose();
	ShowWindow(SW_HIDE);
}

BOOL CShowLogDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CShowLogDlg::ShowLog( CString strDate,CString strLog )
{
	if (m_wndEditDate.GetSafeHwnd())
		m_wndEditDate.SetWindowText(strDate);
	//if (m_wndRichEditCtrl.GetSafeHwnd())
	//	m_wndRichEditCtrl.SetWindowText(strLog);
	if (m_wndEditMessage.GetSafeHwnd())
		m_wndEditMessage.SetWindowText(strLog);

	ShowWindow(SW_SHOW);
}

HBRUSH CShowLogDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if ((pWnd->GetDlgCtrlID() == IDC_EDIT_DMASSAGE) /*&& (nCtlColor == CTLCOLOR_EDIT)*/)
	{
		COLORREF clr = RGB(0,0,0);
		pDC->SetTextColor(clr);   //�����ı�����ʵ��ɫ
		clr = RGB(255,255,255);
		pDC->SetBkColor(clr);     //���ñ���ɫ
		m_brMine = ::CreateSolidBrush(clr);
		return m_brMine;   //��ΪԼ�������ر���ɫ��Ӧ��ˢ�Ӿ��
	}
	else
	{
		HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		return hbr;
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}
