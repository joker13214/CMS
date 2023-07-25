// UserLoginDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CMS.h"
#include "UserLoginDlg.h"
#include "MainFrm.h"

// CUserLoginDlg �Ի���

IMPLEMENT_DYNAMIC(CUserLoginDlg, CDialog)

CUserLoginDlg::CUserLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserLoginDlg::IDD, pParent)
	, m_sUserID(_T(""))
	, m_sPwd(_T(""))
{

}

CUserLoginDlg::~CUserLoginDlg()
{
}

void CUserLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_USERS, m_wndUsersCmb);
	DDX_Text(pDX, IDC_EDIT_USERID, m_sUserID);
	DDX_Text(pDX, IDC_EDIT_USERPWD, m_sPwd);
	DDV_MaxChars(pDX, m_sPwd, 16);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_EDIT_USERID, m_EditUserID);
	DDX_Control(pDX, IDC_EDIT_USERPWD, m_EditPwd);
}


BEGIN_MESSAGE_MAP(CUserLoginDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CUserLoginDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CUserLoginDlg::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_CMB_USERS, &CUserLoginDlg::OnCbnSelchangeCmbUsers)
END_MESSAGE_MAP()


// CUserLoginDlg ��Ϣ�������
void CUserLoginDlg::OnBnClickedOk()
{
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	CMainFrame* pMainFrame = (CMainFrame*)pApp->m_pMainWnd;

	if (pApp->GetLoginUser() != NULL &&
		pApp->GetLoginFlag() == true)
	{
		pApp->SetLoginFlag(false);
		pApp->SetLoginUser(NULL);

		//������Ҫ�û���¼��صĴ���
		m_btnOK.SetWindowText("��¼");
		m_EditPwd.EnableWindow(TRUE);
		m_wndUsersCmb.EnableWindow(TRUE);
		m_EditUserID.EnableWindow(TRUE);
		OnCbnSelchangeCmbUsers();
		SetWindowText("�û���¼");
		pMainFrame->UpdateStatusBarText();
		pMainFrame->UpdateFrameOnLogin();
	}
	else
	{
		UpdateData(TRUE);
		int nSel = m_wndUsersCmb.GetCurSel();
		if (nSel != CB_ERR)
		{
			CUserObject* pUser = (CUserObject*)m_wndUsersCmb.GetItemDataPtr(nSel);

			CString strMessage = "";
			if (pUser->GetOnDuty() == 0 )
			{
				strMessage.Format("�û�'%s'�Ѿ���ְ/����,���ܵ�¼!",pUser->GetName());
				MessageBox(strMessage,"��ʾ",MB_OK|MB_ICONWARNING);
				return ;
			}


			bool blCheckPwd = pUser->CheckPwd(m_sPwd);
			if (blCheckPwd == true)  //��¼�ɹ�
			{
				MessageBox("�û���¼�ɹ�!","��ʾ",MB_OK|MB_ICONINFORMATION);

				pApp->SetLoginFlag(true);
				pApp->SetLoginUser(pUser);

				m_btnOK.SetWindowText("ע��");
				m_EditPwd.EnableWindow(FALSE);
				m_wndUsersCmb.EnableWindow(FALSE);
				m_EditUserID.EnableWindow(FALSE);
				SetWindowText("�û�ע��");

				pMainFrame->UpdateStatusBarText();
				
				ShowWindow(SW_HIDE);
			}
			else //��¼ʧ��
			{
				MessageBox("�û����������������������!","��ʾ",MB_OK|MB_ICONWARNING);
				m_EditPwd.SetFocus();
			}
		}
	}
	
}

void CUserLoginDlg::OnBnClickedCancel()
{
	
	ShowWindow(SW_HIDE);
}

void CUserLoginDlg::OnCbnSelchangeCmbUsers()
{
	int nSel = m_wndUsersCmb.GetCurSel();
	if (nSel != CB_ERR)
	{
		CUserObject* pUser = (CUserObject*)m_wndUsersCmb.GetItemDataPtr(nSel);
		if (pUser != NULL)
			m_EditUserID.SetWindowText(pUser->GetID());

		m_EditPwd.SetWindowText("");
	}
}

BOOL CUserLoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	return TRUE;  
}


void CUserLoginDlg::UpdateDailog()
{
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	if(m_wndUsersCmb.GetSafeHwnd())
	{
		m_wndUsersCmb.ResetContent();

		if (pApp->m_pReadDB)
		{
			POSITION rPos = pApp->m_pReadDB->GetUsersList()->GetHeadPosition();
			int nCurSel = 0;
			while (rPos != NULL)
			{
				CUserObject* pUser = pApp->m_pReadDB->GetUsersList()->GetNext(rPos);
				if (pUser)
				{
					int idx = m_wndUsersCmb.AddString(pUser->GetName());
					m_wndUsersCmb.SetItemDataPtr(idx,pUser);
					if (pUser == pApp->GetLoginUser())
					{
						nCurSel = idx;
					}
				}
			}

			if (m_wndUsersCmb.GetCount() > 0)
			{
				m_wndUsersCmb.SetCurSel(nCurSel);
				OnCbnSelchangeCmbUsers();
			}
		}
	}

	if (pApp->GetLoginUser() != NULL && 
		pApp->GetLoginFlag() == true)
	{
		SetWindowText("�û�ע��");
		m_btnOK.SetWindowText("ע��");
		m_wndUsersCmb.EnableWindow(FALSE);
		m_EditPwd.EnableWindow(FALSE);
		m_EditUserID.EnableWindow(FALSE);
		m_wndUsersCmb.SetWindowText(pApp->GetLoginUser()->GetName());
		m_EditUserID.SetWindowText(pApp->GetLoginUser()->GetID());
		m_EditPwd.SetWindowText(pApp->GetLoginUser()->GetPwd());
	}
	else
	{
		SetWindowText("�û���¼");
		pApp->SetLoginFlag(false);
		pApp->SetLoginUser(NULL);
		m_btnOK.SetWindowText("��¼");
		m_wndUsersCmb.EnableWindow(TRUE);
		m_EditPwd.EnableWindow(TRUE);
		m_EditUserID.EnableWindow(TRUE);
		OnCbnSelchangeCmbUsers();
	}
	SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
}