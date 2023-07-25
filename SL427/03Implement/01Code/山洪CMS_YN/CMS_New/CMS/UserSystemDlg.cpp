// UserSystemDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "UserSystemDlg.h"
#include "UserProDlg.h"


// CUserSystemDlg 对话框

IMPLEMENT_DYNAMIC(CUserSystemDlg, CDialog)

CUserSystemDlg::CUserSystemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserSystemDlg::IDD, pParent)
{

}

CUserSystemDlg::~CUserSystemDlg()
{
}

void CUserSystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_ADDUSER, m_btnAddUser);
	DDX_Control(pDX, IDC_BTN_BACKUP, m_btnBackUp);
}


BEGIN_MESSAGE_MAP(CUserSystemDlg, CDialog)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_ADDUSER, &CUserSystemDlg::OnBnClickedBtnAdduser)
	ON_BN_CLICKED(IDC_BTN_BACKUP, &CUserSystemDlg::OnBnClickedBtnBackup)
	ON_NOTIFY(NM_CLICK,IDC_USERS_GRIDCRTL,&CUserSystemDlg::OnLClickGridCtrl)
END_MESSAGE_MAP()


// CUserSystemDlg 消息处理程序

BOOL CUserSystemDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CWnd* pWnd = GetDlgItem(IDC_STATIC_USERSGRID);
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
		this, IDC_USERS_GRIDCRTL,WS_CHILD|WS_VISIBLE))
	{
		TRACE("Failed to create threads  grid!\n");
		return FALSE;
	}

	m_wndGridCtrl.SetBkColor(RGB(255,255,255));
	m_wndGridCtrl.SetListMode(TRUE);
	m_wndGridCtrl.SetEditable(FALSE);
	m_wndGridCtrl.SetSingleColSelection(TRUE);
	m_wndGridCtrl.SetSingleRowSelection(TRUE);

	SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_USERS),FALSE);
	SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);

	InitUserGridCtrl();
	ShowAllUsers();
	return TRUE;  
	// return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CUserSystemDlg::OnClose()
{
	ShowWindow(SW_HIDE);
}

void CUserSystemDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (nType == SIZE_MINIMIZED)
		ShowWindow(SW_HIDE);
}

void CUserSystemDlg::OnBnClickedBtnAdduser()
{
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();

	CUserProDlg userPro;
	if (userPro.DoModal() == IDOK)
	{
		USER_INFO userinfo;
		userinfo.sName = userPro.m_sName;
		userinfo.sID = userPro.m_sID;
		userinfo.sPwd = userPro.m_sPwd;
		userinfo.bType = userPro.m_bType;
		userinfo.bOnDuty = userPro.m_bOnDuty;

		if (pApp && pApp->m_pReadDB)
		{
			if (pApp->m_pReadDB->AddUserInfo(&userinfo) == true)
			{
				CUserObject* pUsers = new CUserObject(&userinfo);

				pApp->m_pReadDB->GetUsersList()->AddTail(pUsers);

				InitUserGridCtrl();
				ShowAllUsers();
			}
		}
	}
	
}

void CUserSystemDlg::OnBnClickedBtnBackup()
{
	ShowWindow(SW_HIDE);
}


void CUserSystemDlg::InitUserGridCtrl()
{
	if (m_wndGridCtrl.GetSafeHwnd())
	{
		m_wndGridCtrl.DeleteAllItems();

		CString strHeaders[] ={
			"序号",
			"用户名",
			"用户编号",
			"用户类型",
			"在岗/离职",
			"修改",
			"删除"
		};

	 int colsWidths[] ={
			40,
			120,
			100,
			110,
			100,
			50,
			50};

		for (int ColIdx = 0; ColIdx<7;ColIdx ++)
		{   
			m_wndGridCtrl.InsertColumn(strHeaders[ColIdx],DT_VCENTER|DT_LEFT|DT_SINGLELINE,ColIdx);
			m_wndGridCtrl.SetColumnWidth(ColIdx,colsWidths[ColIdx]);
		}

		m_wndGridCtrl.SetFixedRowCount(1);
	}
}


void CUserSystemDlg::ShowAllUsers()
{
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	if (pApp && pApp->m_pReadDB)
	{
		POSITION rPos = pApp->m_pReadDB->GetUsersList()->GetHeadPosition();
		int nItemCount = (int)pApp->m_pReadDB->GetUsersList()->GetCount();
		if (m_wndGridCtrl.GetSafeHwnd())
			m_wndGridCtrl.SetRowCount(nItemCount + 1);

		int Index = 1;
		while (rPos != NULL)
		{
			CUserObject* pUser = pApp->m_pReadDB->GetUsersList()->GetNext(rPos);

			if (pUser)
			{
				ShowOneUserInfo(pUser,Index);

				Index ++;
			}
		}
	}

}

void CUserSystemDlg::ShowOneUserInfo(CUserObject* pUser,int ItemRow)
{
	if (!pUser)		return;
	if (!m_wndGridCtrl.GetSafeHwnd())	return;

	int ItemIndex = ItemRow;

	m_wndGridCtrl.SetRowHeight(ItemRow,MFC_GRIDCTRL_HEIGHT);
	CString strTemp ="";
	strTemp.Format("%d",ItemIndex);

	GV_ITEM Item; 
	Item.row = ItemIndex;
	Item.col = 0;
	Item.nFormat = DT_LEFT|DT_VCENTER;
	Item.mask   =  GVIF_TEXT|GVIF_PARAM;
	Item.strText  = strTemp;
	Item.lParam = (LPARAM)pUser;
	m_wndGridCtrl.SetItem(&Item);

	Item.col = 1;
	Item.nFormat = DT_LEFT|DT_VCENTER;
	Item.mask	=  GVIF_TEXT/*|GVIF_IMAGE*/;   //重点bug问题
	Item.strText = pUser->GetName();
	m_wndGridCtrl.SetItem(&Item);

	Item.mask = GVIF_TEXT;
	Item.col = 2;
	Item.strText = pUser->GetID();
	m_wndGridCtrl.SetItem(&Item);

	Item.col = 3;
	if (pUser->GetType() == 1)
		strTemp = "系统管理员";
	else if (pUser->GetType() == 2)
		strTemp = "操作员";
	Item.strText = strTemp;
	m_wndGridCtrl.SetItem(&Item);


	Item.col = 4;
	if (pUser->GetOnDuty() == 0 )
		strTemp = "离职";
	else
		strTemp = "在岗";
	Item.strText = strTemp;
	m_wndGridCtrl.SetItem(&Item);


	CGridBtnCell *pbtnCell=NULL;
	Item.col = 5;
	m_wndGridCtrl.SetCellType(Item.row,Item.col,RUNTIME_CLASS(CGridBtnCell));
	pbtnCell = (CGridBtnCell *)m_wndGridCtrl.GetCell(Item.row,Item.col);
	if (pbtnCell)
	{
		pUser->m_btnModify.SetGrid(&m_wndGridCtrl);
		pbtnCell->SetBtnDataBase(&pUser->m_btnModify);
		pbtnCell->SetupBtns(0,DFC_BUTTON,DFCS_BUTTONPUSH ,CGridBtnCellBase::CTL_ALIGN_CENTER,
			0,FALSE,"修改"); 
	}

	Item.col = 6;
	m_wndGridCtrl.SetCellType(Item.row,Item.col,RUNTIME_CLASS(CGridBtnCell));
	pbtnCell = (CGridBtnCell *)m_wndGridCtrl.GetCell(Item.row,Item.col);
	if (pbtnCell)
	{
		pUser->m_btnDelete.SetGrid(&m_wndGridCtrl);
		pbtnCell->SetBtnDataBase(&pUser->m_btnDelete);
		pbtnCell->SetupBtns(0,DFC_BUTTON,DFCS_BUTTONPUSH,CGridBtnCellBase::CTL_ALIGN_CENTER,
			0,FALSE,"删除");
	}

	m_wndGridCtrl.SetRowHeight(Item.row,MFC_GRIDCTRL_HEIGHT);
}


void CUserSystemDlg::OnLClickGridCtrl(NMHDR *pNotifyStruct,LRESULT *pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;

	if (!pItem)					return;
	if (pItem->iRow <=0 )		return;
	
	CCMSApp* pApp = (CCMSApp*)AfxGetApp();

	CUserObject* pUser  = (CUserObject*)m_wndGridCtrl.GetItemData(pItem->iRow,0);
	if (!pUser)
		return;

	if(pUser == pApp->GetLoginUser() && 
		(pItem->iColumn == 5 || pItem->iColumn == 6))
	{
		MessageBox("不能操作当前登录用户!","提示",MB_OK|MB_ICONWARNING);
		return;
	}

	if (pItem->iColumn  == 5)       //修改
	{
		CUserProDlg userPro;
		userPro.SetUserObject(pUser);
		if (userPro.DoModal() == IDOK)
		{
			USER_INFO userinfo;
			userinfo.sName = userPro.m_sName;
			userinfo.sID   = userPro.m_sID;
			userinfo.sPwd  = userPro.m_sPwd;
			userinfo.bType = userPro.m_bType;
			userinfo.bOnDuty = userPro.m_bOnDuty;

			if (pUser->CheckInfo(&userinfo) == false) 
				return ;


			if (pApp && pApp->m_pReadDB)
			{
				if (pApp->m_pReadDB->ModifyUserInfo(&userinfo,pUser) == true)
				{
					pUser->UpdateInfo(&userinfo);

					InitUserGridCtrl();
					ShowAllUsers();
				}
			}
		}
	}
	else if (pItem->iColumn == 6)   //删除
	{
		//不能删除当前登录用户

		CString strMessage = "";
		strMessage.Format("确定删除用户'%s'?",pUser->GetName());
		int ret = MessageBox(strMessage,"",MB_YESNO|MB_ICONQUESTION);
		if (ret == IDYES)
		{
			if (pApp->m_pReadDB && pApp->m_pReadDB->deleteUserInfo(pUser) == true)
			{
				InitUserGridCtrl();
				ShowAllUsers();
			}
		}
	}
}