// UserProDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CMS.h"
#include "UserProDlg.h"



// CUserProDlg �Ի���

IMPLEMENT_DYNAMIC(CUserProDlg, CDialog)

CUserProDlg::CUserProDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserProDlg::IDD, pParent)
	, m_sName(_T(""))
	, m_sID(_T(""))
	, m_sPwd(_T(""))
{
	m_bType = 2;
	m_bOnDuty = 1;
	m_pUserObj = NULL;
}

CUserProDlg::~CUserProDlg()
{
}

void CUserProDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_CMB_USERTYPE, m_wndUserTypeCmb);
	DDX_Control(pDX, IDC_CMB_USERONDUTY, m_wndUserOnDutyCmb);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_sName);
	DDX_Text(pDX, IDC_EDIT_USERID, m_sID);
	DDX_Text(pDX, IDC_EDIT_PWD, m_sPwd);
	DDV_MaxChars(pDX, m_sPwd, 16);
	DDV_MaxChars(pDX, m_sName, 20);
	DDV_MaxChars(pDX, m_sID, 10);
}


BEGIN_MESSAGE_MAP(CUserProDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CUserProDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CUserProDlg::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_CMB_USERTYPE, &CUserProDlg::OnCbnSelchangeCmbUsertype)
	ON_CBN_SELCHANGE(IDC_CMB_USERONDUTY, &CUserProDlg::OnCbnSelchangeCmbUseronduty)
	ON_EN_CHANGE(IDC_EDIT_USERNAME, &CUserProDlg::OnEnChangeEditUsername)
	ON_EN_CHANGE(IDC_EDIT_USERID, &CUserProDlg::OnEnChangeEditUserid)
END_MESSAGE_MAP()


// CUserProDlg ��Ϣ�������

BOOL CUserProDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	UpdateData(FALSE);

	if (m_wndUserTypeCmb.GetSafeHwnd())
	{
		m_wndUserTypeCmb.ResetContent();

		int idx = m_wndUserTypeCmb.AddString("����Ա");
		m_wndUserTypeCmb.SetItemData(idx,2);

		idx = m_wndUserTypeCmb.AddString("ϵͳ����Ա");
		m_wndUserTypeCmb.SetItemData(idx,1);

		for (int i =0; i< m_wndUserTypeCmb.GetCount() ; i ++)
		{
			BYTE iData = (BYTE)m_wndUserTypeCmb.GetItemData(i);
			if (iData == m_bType)
			{
				m_wndUserTypeCmb.SetCurSel(i);
				break;
			}
		}
	}

	if (m_wndUserOnDutyCmb.GetSafeHwnd())
	{
		m_wndUserOnDutyCmb.ResetContent();

		int idx = m_wndUserOnDutyCmb.AddString("�ڸ�");
		m_wndUserOnDutyCmb.SetItemData(idx,1);

		idx = m_wndUserOnDutyCmb.AddString("��ְ");
		m_wndUserOnDutyCmb.SetItemData(idx,0);

		for (int i = 0; i < m_wndUserOnDutyCmb.GetCount(); i ++)
		{
			BYTE iData = (BYTE)m_wndUserOnDutyCmb.GetItemData(i);
			if (iData == m_bOnDuty)
			{
				m_wndUserOnDutyCmb.SetCurSel(i);
				break;
			}
		}
	}

	
	CString strCaption = "";
	if (m_pUserObj != NULL)
		strCaption.Format("[%s] ����",m_pUserObj->GetName());
	else
		strCaption = "������û�";

	SetWindowText(strCaption);
	return TRUE;  
	// return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CUserProDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	m_sName.TrimLeft();
	m_sName.TrimRight();
	m_sID.TrimLeft();
	m_sID.TrimRight();
	UpdateData(FALSE);
	if (m_sName.IsEmpty() || m_sID.IsEmpty() || m_sPwd.IsEmpty())
	{
		MessageBox("�û�������š����벻��Ϊ�գ�������!","",MB_OK|MB_ICONWARNING);
		return;
	}

	if (CheckUserName(m_sName)== false)
	{
		MessageBox("�û��������������ַ�(�ո������ַ���),����������!","",MB_OK|MB_ICONWARNING);
		return;
	}


	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	if (pApp->m_pReadDB)
	{
		if (pApp->m_pReadDB->CheckUserExist(m_sID,m_pUserObj) == true)
		{
			MessageBox("�û�����Ѿ�����,����������","",MB_OK|MB_ICONWARNING);
			return;
		}
	}
	
	OnOK();
}

void CUserProDlg::OnBnClickedCancel()
{
	
	OnCancel();
}

void CUserProDlg::OnCbnSelchangeCmbUsertype()
{
	int nSel = m_wndUserTypeCmb.GetCurSel();
	if (nSel != CB_ERR)
	{
		BYTE iData = (BYTE)m_wndUserTypeCmb.GetItemData(nSel);
		m_bType = iData;
	}
}

void CUserProDlg::OnCbnSelchangeCmbUseronduty()
{
	int nSel = m_wndUserOnDutyCmb.GetCurSel();
	if (nSel != CB_ERR)
	{
		BYTE iData = (BYTE)m_wndUserOnDutyCmb.GetItemData(nSel);
		m_bOnDuty = iData;
	}
}

void CUserProDlg::SetUserObject(CUserObject* pUser)
{
	m_pUserObj = pUser;
	if (m_pUserObj)
	{
		m_sName = m_pUserObj->GetName();
		m_sID  = m_pUserObj->GetID();
		m_sPwd = m_pUserObj->GetPwd();
		m_bType = m_pUserObj->GetType();
		m_bOnDuty = m_pUserObj->GetOnDuty();
	}
}
void CUserProDlg::OnEnChangeEditUsername()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

void CUserProDlg::OnEnChangeEditUserid()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

}


bool CUserProDlg::CheckUserName(CString _Name)
{
	int length = _Name.GetLength();
	char* chr = new char[length + 1];
	memset(chr,0,length + 1);

	memcpy(chr,_Name,length);
	bool bl = true;
	for (int i = 0; i < length; i ++)
	{
		if ((chr[i] >= '0' && chr[i] <='9')  ||
			(chr[i] >= 'a' && chr[i] <='z') ||
			(chr[i] >= 'A' && chr[i] <='Z') ||
			 chr[i] <= 0 )
		{

		}
		else
		{
			bl = false ;
			break;
		}
	}

	delete[] chr;

	return bl;
}