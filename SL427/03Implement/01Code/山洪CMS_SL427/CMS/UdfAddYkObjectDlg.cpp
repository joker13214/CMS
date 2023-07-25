// UdfAddYkObjectDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "UdfAddYkObjectDlg.h"


// CUdfAddYkObjectDlg 对话框

IMPLEMENT_DYNAMIC(CUdfAddYkObjectDlg, CDialog)

CUdfAddYkObjectDlg::CUdfAddYkObjectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUdfAddYkObjectDlg::IDD, pParent)
	, m_stName(_T(""))
	, m_strYkName(_T(""))
{
	m_strCaption = "";
	m_OperateFlag = RECORD_NEW;

	m_pCurYkObject= NULL;
	m_pCurStation = NULL;
	m_wYkCode = 0;
}

CUdfAddYkObjectDlg::~CUdfAddYkObjectDlg()
{
}

void CUdfAddYkObjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_STNAME, m_stName);
	DDV_MaxChars(pDX, m_stName, 50);
	DDX_Text(pDX, IDC_EDIT_YKNAME, m_strYkName);
	DDV_MaxChars(pDX, m_strYkName, 50);
	DDX_Text(pDX, IDC_EDIT_YKCODE, m_wYkCode);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CUdfAddYkObjectDlg, CDialog)
END_MESSAGE_MAP()


// CUdfAddYkObjectDlg 消息处理程序


void CUdfAddYkObjectDlg::Update(CRtuYkObject* pYk)
{
	if (pYk)
	{
		m_stName = pYk->GetParent()->GetName();
		m_strYkName = pYk->GetName();
		m_wYkCode = pYk->GetCode();

		m_pCurYkObject = pYk;
		m_pCurStation = pYk->GetParent();

		m_OperateFlag = RECORD_EDIT;
		m_strCaption.Format("%s [基本信息]",pYk->GetName());

	}
}

BOOL CUdfAddYkObjectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	

	if (!m_strCaption.IsEmpty())
	{
		SetWindowText(m_strCaption);

	}

    UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CUdfAddYkObjectDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData(TRUE);

	m_strYkName.Trim();
	if (m_strYkName.IsEmpty())
	{
		MessageBox("遥控名称不能为空,请输入!","",MB_OK|MB_ICONERROR);
		return;
	}
	if (m_wYkCode <= 0)
	{
		MessageBox("遥控码不能小于或等于零,请重新输入!","",MB_OK|MB_ICONERROR);
		return;
	}

	CDialog::OnOK();
}


void CUdfAddYkObjectDlg::SetRtuStation(CRtuStation* pStation)
{
	m_pCurStation = pStation;
	if (pStation)
		m_stName = pStation->GetName();
}