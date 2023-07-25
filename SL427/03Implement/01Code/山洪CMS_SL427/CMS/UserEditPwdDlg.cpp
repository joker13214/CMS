// UserEditPwdDlg.cpp : 实现文件
//
#include "stdafx.h"
#include "CMS.h"
#include "UserEditPwdDlg.h"


// CUserEditPwdDlg 对话框

IMPLEMENT_DYNAMIC(CUserEditPwdDlg, CDialog)

CUserEditPwdDlg::CUserEditPwdDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserEditPwdDlg::IDD, pParent)
	, m_strName(_T(""))
	, m_strOldPwd(_T(""))
	, m_strNewPwd(_T(""))
	, m_strReNewPwd(_T(""))
{
}

CUserEditPwdDlg::~CUserEditPwdDlg()
{
}

void CUserEditPwdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_BtnCancel);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_OLDPWD, m_strOldPwd);
	DDV_MaxChars(pDX, m_strOldPwd, 16);
	DDX_Text(pDX, IDC_EDIT_NEWPWD, m_strNewPwd);
	DDV_MaxChars(pDX, m_strNewPwd, 16);
	DDX_Text(pDX, IDC_EDIT_RENEWPWD, m_strReNewPwd);
	DDV_MaxChars(pDX, m_strReNewPwd, 16);
}


BEGIN_MESSAGE_MAP(CUserEditPwdDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CUserEditPwdDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CUserEditPwdDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CUserEditPwdDlg 消息处理程序

BOOL CUserEditPwdDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	if (pApp->GetLoginFlag() == true &&
		pApp->GetLoginUser() != NULL)
		m_strName = pApp->GetLoginUser()->GetName();

	UpdateData(FALSE);


	CenterWindow(GetParent());
	SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
	return TRUE;  
}

void CUserEditPwdDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();

	bool blChecked = false;
	if (pApp->GetLoginFlag() == true &&
		pApp->GetLoginUser() != NULL)
	{
		blChecked = pApp->GetLoginUser()->CheckPwd(m_strOldPwd);
		if (blChecked == false)
		{
			MessageBox("旧密码不正确,请重新输入!","提示",MB_OK|MB_ICONWARNING);
			return;
		}

		if (m_strNewPwd != m_strReNewPwd)
		{
			MessageBox("新密码不一致,请重新输入!","提示",MB_OK|MB_ICONWARNING);
			return;
		}

		USER_INFO user_info;
		user_info.sName = pApp->GetLoginUser()->GetName();
		user_info.sID	= pApp->GetLoginUser()->GetID();
		user_info.sPwd	= m_strNewPwd;//pApp->GetLoginUser()->GetPwd();
		user_info.bType	= pApp->GetLoginUser()->GetType();
		user_info.bOnDuty	= pApp->GetLoginUser()->GetOnDuty();
		if (pApp->GetLoginUser()->CheckInfo(&user_info) == true)
		{
			if (pApp->m_pReadDB->ModifyUserInfo(&user_info,pApp->GetLoginUser()) == true)
				pApp->GetLoginUser()->SetPwd(m_strNewPwd);
			else
				return;
		}
		MessageBox("密码修改成功!","提示",MB_OK|MB_ICONINFORMATION);

	}
}

void CUserEditPwdDlg::OnBnClickedCancel()
{
	
	OnCancel();
}
