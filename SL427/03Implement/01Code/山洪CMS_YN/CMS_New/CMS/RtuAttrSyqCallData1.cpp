// RtuAttrSyqCallData1.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuAttrSyqCallData1.h"

#define W_TIMER_CALLDATA1   0x01
#define W_TIMER_CALLDATA2  0x02
// CRtuAttrSyqCallData1 �Ի���

IMPLEMENT_DYNAMIC(CRtuAttrSyqCallData1, CDialog)

CRtuAttrSyqCallData1::CRtuAttrSyqCallData1(CWnd* pParent /*=NULL*/)
	: CDialog(CRtuAttrSyqCallData1::IDD, pParent)
{

}

CRtuAttrSyqCallData1::~CRtuAttrSyqCallData1()
{
}

void CRtuAttrSyqCallData1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DATA2, m_listdata1);
	DDX_Control(pDX, IDC_LIST_DATA1, m_listdata2);
	DDX_Control(pDX, IDC_CALLDATA_BTN1, m_btn_calldata1);
	DDX_Control(pDX, IDC_CALLDATA_BTN2, m_btn_calldata2);
}


BEGIN_MESSAGE_MAP(CRtuAttrSyqCallData1, CDialog)
	ON_BN_CLICKED(IDC_CALLDATA_BTN1, &CRtuAttrSyqCallData1::OnBnClickedCalldataBtn1)
	ON_BN_CLICKED(IDC_CALLDATA_BTN2, &CRtuAttrSyqCallData1::OnBnClickedCalldataBtn2)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CRtuAttrSyqCallData1 ��Ϣ�������
BOOL CRtuAttrSyqCallData1::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_bResult = FALSE;

	DWORD dwStyle = m_listdata1.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
	dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��
	//dwStyle |= LVS_EX_CHECKBOXES;//itemǰ����checkbox�ؼ�
	m_listdata1.SetExtendedStyle(dwStyle); //������չ���

	 dwStyle = m_listdata2.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
	dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��
	//dwStyle |= LVS_EX_CHECKBOXES;//itemǰ����checkbox�ؼ�
	m_listdata2.SetExtendedStyle(dwStyle); //������չ���

	m_listdata1.InsertColumn(0,_T("Ҫ������"),LVCFMT_LEFT,150);
	m_listdata1.InsertColumn(1,_T("����"),LVCFMT_LEFT,120);

	m_listdata2.InsertColumn(0,_T("Ҫ������"),LVCFMT_LEFT,150);
	m_listdata2.InsertColumn(1,_T("����"),LVCFMT_LEFT,120);

	return TRUE;
}

void CRtuAttrSyqCallData1::SetParentDlg(CRtuPropertyDlg* pDlg)
{
	m_pParentDlg = pDlg;	
}

void CRtuAttrSyqCallData1::Update()
{
	if(m_pParentDlg)
		m_pSubStation = m_pParentDlg->GetRtuStation();	
}


void CRtuAttrSyqCallData1::OnBnClickedCalldataBtn1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_pSubStation->blEnbaleTrans())
	{
		SetEnable(FALSE);

		CSYQFrameFormat appFrame;
		m_pSubStation->BuildBasicPacket(appFrame);  //���ɳ�ʼ���·�֡����֡������Ϣ
		//������ݴ��
		m_pSubStation->GetSysDataTrans_Syq()->SYQ_EN_COMMON(&appFrame,0x36,TRUE);

		UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
		SetTimer(W_TIMER_CALLDATA1, nElapse, NULL);
	}
}

void CRtuAttrSyqCallData1::OnBnClickedCalldataBtn2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_pSubStation->blEnbaleTrans())
	{
		SetEnable(FALSE);

		CSYQFrameFormat appFrame;
		m_pSubStation->BuildBasicPacket(appFrame);  //���ɳ�ʼ���·�֡����֡������Ϣ
		//������ݴ��
		m_pSubStation->GetSysDataTrans_Syq()->SYQ_EN_COMMON(&appFrame,0x39,TRUE);

		UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
		SetTimer(W_TIMER_CALLDATA2, nElapse, NULL);
	}
}
void CRtuAttrSyqCallData1::UpdateDlgMessage(int attr, bool result)
{
	m_bResult = result;

	if(attr == syq_call_data1)
	{ 
		SendMessage(WM_TIMER,W_TIMER_CALLDATA1, NULL);
	}
	else if(attr == syq_call_data2)
	{ 
		SendMessage(WM_TIMER,W_TIMER_CALLDATA2, NULL);
	}
}

void CRtuAttrSyqCallData1::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CString strLog= "";
	_Log_level_type LogLevel = Normal_Log_Message;

	switch (nIDEvent)
	{
	case W_TIMER_CALLDATA1:
		{
			if (m_bResult == TRUE)
			{
				FillDataList1();
				strLog = "��ѯң��վʵʱҪ������ �ɹ�!";
			}
			else if ( m_bResult == FALSE)
			{
				strLog = "��ѯң��վʵʱҪ������ ��ʱ!";
				LogLevel = Warning_log_Message;
			}
			else if (m_bResult == 0xff)
			{
				strLog = "��ѯң��վʵʱҪ������ ����!";
				LogLevel = Warning_log_Message;
			}
		}
		break;
	case W_TIMER_CALLDATA2:
		{
			if (m_bResult == TRUE)
			{
				FillDataList2();
				strLog = "��ѯң��վ�˹����� �ɹ�!";
			}
			else if ( m_bResult == FALSE)
			{
				strLog = "��ѯң��վ�˹����� ��ʱ!";
				LogLevel = Warning_log_Message;
			}
			else if (m_bResult == 0xff)
			{
				strLog = "��ѯң��վ�˹����� ����!";
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


void CRtuAttrSyqCallData1::SetEnable(bool b)
{
	m_btn_calldata1.EnableWindow(b);
	m_btn_calldata2.EnableWindow(b);
}


void CRtuAttrSyqCallData1::FillDataList1()
{
	m_listdata1.DeleteAllItems();

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
			m_listdata1.InsertItem(&Item);


			Item.mask = LVIF_TEXT;
			Item.iItem = index;
			Item.iSubItem =1; //���÷���Ŀ,0����������Ŀ   
			Item.pszText=(LPSTR)(LPCTSTR)((*itr)->strdata); 
			m_listdata1.SetItem(&Item);

			index ++;

		}
	}
}



void CRtuAttrSyqCallData1::FillDataList2()
{
	m_listdata2.DeleteAllItems();

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
			m_listdata2.InsertItem(&Item);


			Item.mask = LVIF_TEXT;
			Item.iItem = index;
			Item.iSubItem =1; //���÷���Ŀ,0����������Ŀ   
			Item.pszText=(LPSTR)(LPCTSTR)((*itr)->strdata); 
			m_listdata2.SetItem(&Item);

			index ++;

		}
	}
}
