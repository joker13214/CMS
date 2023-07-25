// UdfShowDetailLog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CMS.h"
#include "UdfShowDetailLog.h"


// CUdfShowDetailLog �Ի���

IMPLEMENT_DYNAMIC(CUdfShowDetailLog, CDialog)

CUdfShowDetailLog::CUdfShowDetailLog(CWnd* pParent /*=NULL*/)
	: CDialog(CUdfShowDetailLog::IDD, pParent)
	, m_strDatetime(_T(""))
	, m_strLogLevel(_T(""))
	, m_strLogType(_T(""))
	, m_strLogMessage(_T(""))
{

}

CUdfShowDetailLog::~CUdfShowDetailLog()
{
}

void CUdfShowDetailLog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_DATETIME, m_strDatetime);
	DDX_Text(pDX, IDC_STATIC_LOGLEVEL, m_strLogLevel);
	DDX_Text(pDX, IDC_STATIC_LOGTYPE, m_strLogType);
	DDX_Text(pDX, IDC_EDIT_LOGMESSAGE, m_strLogMessage);
}


BEGIN_MESSAGE_MAP(CUdfShowDetailLog, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// CUdfShowDetailLog ��Ϣ�������

HBRUSH CUdfShowDetailLog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

	if ((pWnd->GetDlgCtrlID() == IDC_EDIT_LOGMESSAGE) /*&& (nCtlColor == CTLCOLOR_EDIT)*/)
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

void CUdfShowDetailLog::OnClose()
{
	ShowWindow(SW_HIDE);
}

void CUdfShowDetailLog::Show(bool blShow)
{
	UpdateData(FALSE);
	if (blShow)
	{
		//CenterWindow(GetParent());
		ShowWindow(SW_SHOW);
		SetActiveWindow();
	}
	else
	{
		ShowWindow(SW_HIDE);
	}
}
BOOL CUdfShowDetailLog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	ModifyStyleEx(0,WS_EX_STATICEDGE); 

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CUdfShowDetailLog::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	// TODO: �ڴ˴������Ϣ����������
	if (nState == WA_INACTIVE)
		ShowWindow(SW_HIDE);
}
