// RtuAttrSyqCalldata.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuAttrSyqCalldata.h"


// CRtuAttrSyqCalldata �Ի���
#define W_TIMER_CALLDATA_BYTIME 0x01

IMPLEMENT_DYNAMIC(CRtuAttrSyqCalldata, CDialog)

CRtuAttrSyqCalldata::CRtuAttrSyqCalldata(CWnd* pParent /*=NULL*/)
	: CDialog(CRtuAttrSyqCalldata::IDD, pParent)
	, m_nNumber(1)
{

}

CRtuAttrSyqCalldata::~CRtuAttrSyqCalldata()
{
}

void CRtuAttrSyqCalldata::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_QUERY, m_Btn_Query);
	DDX_Control(pDX, IDC_COMBO_YAOSU_LIST, m_Yaosu);
	DDX_Control(pDX, IDC_DATETIME_BEGIN, m_DtBegin);
	DDX_Control(pDX, IDC_DATETIME_END, m_DtEnd);
	DDX_Control(pDX, IDC_COMBO_TIME_STEP, m_time_step);
	DDX_Control(pDX, IDC_STATIC_TXT, m_text);
	DDX_Text(pDX, IDC_EDIT_NUM, m_nNumber);
	DDX_Control(pDX, IDC_LIST_DATA, m_listdata);
}


BEGIN_MESSAGE_MAP(CRtuAttrSyqCalldata, CDialog)
	ON_BN_CLICKED(IDC_QUERY, &CRtuAttrSyqCalldata::OnBnClickedQuery)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_TIME_STEP, &CRtuAttrSyqCalldata::OnCbnSelchangeComboTimeStep)
END_MESSAGE_MAP()


 BOOL CRtuAttrSyqCalldata::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_DtBegin.SetFormat("yyyy��MM��dd�� HHʱ");
	m_DtEnd.SetFormat("yyyy��MM��dd�� HHʱ");
	
	m_bResult = FALSE;

	if (m_Yaosu.GetSafeHwnd())
	{
		m_Yaosu.ResetContent();
		int appType_idx = 0;
		appType_idx = m_Yaosu.AddString("1minʱ�ν�ˮ��");//PN01
		m_Yaosu.SetItemData(appType_idx, (BYTE)1);
		appType_idx = m_Yaosu.AddString("5minʱ�ν�ˮ��");//PN05
		m_Yaosu.SetItemData(appType_idx, (BYTE)2);
		appType_idx = m_Yaosu.AddString("10minʱ�ν�ˮ��");//PN10
		m_Yaosu.SetItemData(appType_idx, (BYTE)3);
		appType_idx = m_Yaosu.AddString("30minʱ�ν�ˮ��");//PN30
		m_Yaosu.SetItemData(appType_idx, (BYTE)4);

		appType_idx = m_Yaosu.AddString("1hʱ�ν�ˮ��");//P1
		m_Yaosu.SetItemData(appType_idx, (BYTE)5);
		appType_idx = m_Yaosu.AddString("2hʱ�ν�ˮ��");//P2
		m_Yaosu.SetItemData(appType_idx, (BYTE)6);
		appType_idx = m_Yaosu.AddString("3hʱ�ν�ˮ��");//P3
		m_Yaosu.SetItemData(appType_idx, (BYTE)7);
		appType_idx = m_Yaosu.AddString("6hʱ�ν�ˮ��");//P6
		m_Yaosu.SetItemData(appType_idx, (BYTE)8);

		appType_idx = m_Yaosu.AddString("12hʱ�ν�ˮ��");//P12
		m_Yaosu.SetItemData(appType_idx, (BYTE)9);
		appType_idx = m_Yaosu.AddString("�ս�ˮ��");//PD
		m_Yaosu.SetItemData(appType_idx, (BYTE)10);
		appType_idx = m_Yaosu.AddString("�½�ˮ��");//PM
		m_Yaosu.SetItemData(appType_idx, (BYTE)11);
		appType_idx = m_Yaosu.AddString("��������");//PJ
		m_Yaosu.SetItemData(appType_idx, (BYTE)12);
		appType_idx = m_Yaosu.AddString("������");//PR
		m_Yaosu.SetItemData(appType_idx, (BYTE)13);

		appType_idx = m_Yaosu.AddString("��ˮ���ۼ�ֵ");//PT
		m_Yaosu.SetItemData(appType_idx, (BYTE)14);
		appType_idx = m_Yaosu.AddString("˲ʱ�ӵ�ˮλ����λ");//Z
		m_Yaosu.SetItemData(appType_idx, (BYTE)15);
		appType_idx = m_Yaosu.AddString("��������");//ED
		m_Yaosu.SetItemData(appType_idx, (BYTE)16);
		appType_idx = m_Yaosu.AddString("˲ʱ��������ˮ����");//Q
		m_Yaosu.SetItemData(appType_idx, (BYTE)17);

	}
	m_Yaosu.SetCurSel(0);

	if (m_time_step.GetSafeHwnd())
	{
		m_time_step.ResetContent();
		int appType_idx = 0;
		appType_idx = m_time_step.AddString("��");
		m_time_step.SetItemData(appType_idx, (BYTE)1);
		appType_idx = m_time_step.AddString("Сʱ ");
		m_time_step.SetItemData(appType_idx, (BYTE)2);
		appType_idx = m_time_step.AddString("����");
		m_time_step.SetItemData(appType_idx, (BYTE)3);
	}
	
	m_time_step.SetCurSel(0);
	m_text.SetWindowText("(1-31)");



	DWORD dwStyle = m_listdata.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
	dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��
	//dwStyle |= LVS_EX_CHECKBOXES;//itemǰ����checkbox�ؼ�
	m_listdata.SetExtendedStyle(dwStyle); //������չ���

	m_listdata.InsertColumn(0,_T("Ҫ������"),LVCFMT_LEFT,150);
	m_listdata.InsertColumn(1,_T("����"),LVCFMT_LEFT,120);

	return TRUE;
}

void CRtuAttrSyqCalldata::SetParentDlg(CRtuPropertyDlg* pDlg)
{
	m_pParentDlg = pDlg;	
}


void CRtuAttrSyqCalldata::Update()
{
	if(m_pParentDlg)
		m_pSubStation = m_pParentDlg->GetRtuStation();	
}



// CRtuAttrSyqCalldata ��Ϣ�������

void CRtuAttrSyqCalldata::OnBnClickedQuery()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

		m_listdata.DeleteAllItems();

	SYSTEMTIME m_timeBegin;
	SYSTEMTIME m_timeEnd;

//	SetEnable(FALSE);
		
		CTime timeBegin, timeEnd;
		m_DtBegin.GetTime(timeBegin);
		m_DtEnd.GetTime(timeEnd);
		if (timeBegin > timeEnd)
		{
		//	timeEnd.GetAsSystemTime(m_timeBegin);
		//	timeBegin.GetAsSystemTime(m_timeEnd);
			AfxMessageBox("ʱ��������������ÿ�ʼʱ��");
			SetEnable(TRUE);
			return;
		}
		else
		{
			timeBegin.GetAsSystemTime(m_timeBegin);
			timeEnd.GetAsSystemTime(m_timeEnd);
		}


		int  n = m_time_step.GetCurSel();
		
	   UpdateData(TRUE);
		if(n == 0)
		{
			if(m_nNumber<1 || m_nNumber >31)
			{
				AfxMessageBox("��Χ�������������÷�Χ");
				SetEnable(TRUE);
				return;
			}
		}
		else if(n == 1)
		{
			if(m_nNumber<1 || m_nNumber >23)
			{
				AfxMessageBox("��Χ�������������÷�Χ");
				SetEnable(TRUE);
				return;
			}
		}
		else if(n == 2)
		{
			if(m_nNumber<1 || m_nNumber >59)
			{
				AfxMessageBox("��Χ�������������÷�Χ");
				SetEnable(TRUE);
				return;
			}
		}
		

	
		CString str;
		m_Yaosu.GetWindowText(str);

		std::map<CString, SYQ_YS_STRUCT>::iterator it_map;
		//it_map = SYQ_TypeMap::GetInstance()->m_SYQTypeMap.find(tempstr);
		//if (it_map != SYQ_TypeMap::GetInstance()->m_SYQTypeMap.end())
		for(it_map = SYQ_TypeMap::GetInstance()->m_SYQTypeMap.begin();it_map != SYQ_TypeMap::GetInstance()->m_SYQTypeMap.end();it_map++)
		{
			if(it_map->second.name == str)
				break;
		}

		str = it_map->second.ename;

		if (m_pSubStation->blEnbaleTrans())
		{
			CSYQFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //���ɳ�ʼ���·�֡����֡������Ϣ
			//������ݴ��
			m_pSubStation->GetSysDataTrans_Syq()->SYQ_EN_38H(&appFrame,str,n,m_nNumber,timeBegin,timeEnd);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_CALLDATA_BYTIME, nElapse, NULL);
			SetEnable(FALSE);
		}

}


void CRtuAttrSyqCalldata::SetEnable(bool b)
{
	m_Btn_Query.EnableWindow(b);
	m_Yaosu.EnableWindow(b);
	m_DtBegin.EnableWindow(b);
	m_DtEnd.EnableWindow(b);
}

void CRtuAttrSyqCalldata::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CString strLog= "";
	_Log_level_type LogLevel = Normal_Log_Message;

	switch (nIDEvent)
	{
	case W_TIMER_CALLDATA_BYTIME:
		{
			if (m_bResult == TRUE)
			{
				FillDataList();
				strLog = "��ѯң��վʱ������ �ɹ�!";
						}
			else if ( m_bResult == FALSE)
			{
				strLog = "��ѯң��վʱ������ ��ʱ!";
				LogLevel = Warning_log_Message;
			}
			else if (m_bResult == 0xff)
			{
				strLog = "��ѯң��վʱ������ ����!";
				LogLevel = Warning_log_Message;
			}
		}
		break;
	default:
		break;;
	}

	KillTimer(nIDEvent);
	SetEnable(TRUE);
	if (m_pParentDlg && m_pParentDlg->GetSafeHwnd())
		m_pParentDlg->AddLog(strLog,(PSYSTEMTIME)NULL,LogLevel);

	if (m_pSubStation)
		m_pSubStation->InsertLogMessage(strLog,LogLevel,NULL);

	CDialog::OnTimer(nIDEvent);
}

void CRtuAttrSyqCalldata::OnCbnSelchangeComboTimeStep()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	int  n = m_time_step.GetCurSel();

	if(n == 0)
		m_text.SetWindowText("��1-31��");
	else if(n == 1)
		m_text.SetWindowText("��1-23��");
	else if(n == 2)
		m_text.SetWindowText("��1-59��");

}

void CRtuAttrSyqCalldata::UpdateDlgMessage(int attr, bool result)
{
	m_bResult = result;

	if(attr == syq_call_data_bytime)
	{ 
		SendMessage(WM_TIMER,W_TIMER_CALLDATA_BYTIME, NULL);
	}

}

void CRtuAttrSyqCalldata::FillDataList()
{
	m_listdata.DeleteAllItems();

	if(m_pSubStation->m_syqdatadiscribelist.size()>0)
	{
		std::list<_SYQ_DATA_DESCRIBE*>::iterator itr;
		int index = 0;
		LVITEM Item;
		for(itr =m_pSubStation->m_syqdatadiscribelist.begin(); itr != m_pSubStation->m_syqdatadiscribelist.end();itr++ )
		{		
			Item.mask = LVIF_TEXT;
			Item.iItem = index;
			Item.iSubItem =0; //���÷���Ŀ,0����������Ŀ   
			Item.pszText=(LPSTR)(LPCTSTR)((*itr)->strname); 
			m_listdata.InsertItem(&Item);


			Item.mask = LVIF_TEXT;
			Item.iItem = index;
			Item.iSubItem =1; //���÷���Ŀ,0����������Ŀ   
			Item.pszText=(LPSTR)(LPCTSTR)((*itr)->strdata); 
			m_listdata.SetItem(&Item);

			index ++;

		}
	}
}


