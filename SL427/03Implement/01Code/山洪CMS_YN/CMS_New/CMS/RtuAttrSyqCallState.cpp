// RtuAttrSyqCallState.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuAttrSyqCallState.h"


#define W_TIMER_CALLSTATE   0x01
// CRtuAttrSyqCallState �Ի���

IMPLEMENT_DYNAMIC(CRtuAttrSyqCallState, CDialog)

CRtuAttrSyqCallState::CRtuAttrSyqCallState(CWnd* pParent /*=NULL*/)
	: CDialog(CRtuAttrSyqCallState::IDD, pParent)
{

}

CRtuAttrSyqCallState::~CRtuAttrSyqCallState()
{
}

void CRtuAttrSyqCallState::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_QUERY, m_btn_query);
	DDX_Control(pDX, IDC_LIST, m_list);
}


BEGIN_MESSAGE_MAP(CRtuAttrSyqCallState, CDialog)
	ON_BN_CLICKED(IDC_BTN_QUERY, &CRtuAttrSyqCallState::OnBnClickedBtnQuery)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CRtuAttrSyqCallState ��Ϣ�������

// CRtuAttrSyqCallData1 ��Ϣ�������
BOOL CRtuAttrSyqCallState::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_bResult = FALSE;


	DWORD dwStyle = m_list.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
	dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��
	//dwStyle |= LVS_EX_CHECKBOXES;//itemǰ����checkbox�ؼ�
	m_list.SetExtendedStyle(dwStyle); //������չ���

	m_list.InsertColumn(0,_T("����"),LVCFMT_LEFT,150);
	m_list.InsertColumn(1,_T("״̬"),LVCFMT_LEFT,120);


	FillList(0,"��������״̬","");
	FillList(1,"���ص�ѹ״̬","");
	FillList(2,"ˮλ���ޱ���״̬","");
	FillList(3,"�������ޱ���״̬","");
	FillList(4,"ˮ�ʳ��ޱ���״̬","");
	FillList(5,"�����Ǳ�״̬","");
	FillList(6,"ˮλ�Ǳ�״̬","");
	FillList(7,"�ն�����״̬","");
	FillList(8,"�洢��״̬","");
	FillList(9,"IC��������Ч��","");
	FillList(10,"ˮ�ù���״̬","");



	return TRUE;
}

void CRtuAttrSyqCallState::FillList(int index,CString strname,CString strstate)
{
	LVITEM Item;
	Item.mask = LVIF_TEXT;
	Item.iItem = index;
	Item.iSubItem =0; //���÷���Ŀ,0����������Ŀ   
	Item.pszText=(LPSTR)(LPCTSTR)(strname); 
	m_list.InsertItem(&Item);


	Item.mask = LVIF_TEXT;
	Item.iItem = index;
	Item.iSubItem =1; //���÷���Ŀ,0����������Ŀ   
	Item.pszText=(LPSTR)(LPCTSTR)(strstate); 
	m_list.SetItem(&Item);
}

void CRtuAttrSyqCallState::SetParentDlg(CRtuPropertyDlg* pDlg)
{
	m_pParentDlg = pDlg;	
}

void CRtuAttrSyqCallState::Update()
{
	if(m_pParentDlg)
		m_pSubStation = m_pParentDlg->GetRtuStation();	
}


void CRtuAttrSyqCallState::OnBnClickedBtnQuery()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_pSubStation->blEnbaleTrans())
	{
		SetEnable(FALSE);

		m_pSubStation->m_syqState = -1;

		CSYQFrameFormat appFrame;
		m_pSubStation->BuildBasicPacket(appFrame);  //���ɳ�ʼ���·�֡����֡������Ϣ
		//������ݴ��
		m_pSubStation->GetSysDataTrans_Syq()->SYQ_EN_COMMON(&appFrame,0x48,TRUE);

		UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
		SetTimer(W_TIMER_CALLSTATE, nElapse, NULL);
	}
//	m_pSubStation->m_syqState = 30;
	//FillState();

}

void CRtuAttrSyqCallState::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CString strLog= "";
	_Log_level_type LogLevel = Normal_Log_Message;

	switch (nIDEvent)
	{
	case W_TIMER_CALLSTATE:
		{
			if (m_bResult == TRUE)
			{
			//	FillDataList1();
				strLog = "��ѯң��վ״̬�ͱ�����Ϣ �ɹ�!";
			}
			else if ( m_bResult == FALSE)
			{
				strLog = "��ѯң��վ״̬�ͱ�����Ϣ ��ʱ!";
				LogLevel = Warning_log_Message;
			}
			else if (m_bResult == 0xff)
			{
				strLog = "��ѯң��վ״̬�ͱ�����Ϣ ����!";
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
void CRtuAttrSyqCallState::SetEnable(bool b)
{
	m_btn_query.EnableWindow(b);
}

void CRtuAttrSyqCallState::UpdateDlgMessage(int attr, bool result)
{
	m_bResult = result;

	if(attr == syq_call_state)
		SendMessage(WM_TIMER,W_TIMER_CALLSTATE, NULL);
	
}

void CRtuAttrSyqCallState::FillState()
{
	m_list.DeleteAllItems();

	CString strname = "", strstate = "";
	int temp;
	if(m_pSubStation->m_syqState != -1)
	{
		for (int i = 0; i<10; i++)
		{
			switch(i)
			{
			case 0:
				{
					strname = "��������״̬";
					temp = m_pSubStation->m_syqState&0x01;
					if(temp == 0)
						strstate = "����";
					else if(temp == 1)
						strstate = "ͣ��";
				}
				break;
			case 1:
			{
				strname = "���ص�ѹ״̬";
				temp = (m_pSubStation->m_syqState&0x02)>>1;
				if(temp == 0)
					strstate = "����";
				else if(temp == 1)
					strstate = "��ѹ��";
			}
			break;
			case 2:
				{
					strname = "ˮλ���ޱ���״̬";
					temp = (m_pSubStation->m_syqState&0x04)>>2;
					if(temp == 0)
						strstate = "����";
					else if(temp == 1)
						strstate = "����";
				}
				break;
			case 3:
				{
					strname = "�������ޱ���״̬";
					temp = (m_pSubStation->m_syqState&0x08)>>3;
					if(temp == 0)
						strstate = "����";
					else if(temp == 1)
						strstate = "����";
				}
				break;
			case 4:
				{
					strname = "ˮ�ʳ��ޱ���״̬";
					temp = (m_pSubStation->m_syqState&0x10)>>4;
					if(temp == 0)
						strstate = "����";
					else if(temp == 1)
						strstate = "����";
				}
				break;
			case 5:
				{
					strname = "�����Ǳ�״̬";
					temp = (m_pSubStation->m_syqState&0x20)>>5;
					if(temp == 0)
						strstate = "����";
					else if(temp == 1)
						strstate = "����";
				}
				break;
			case 6:
				{
					strname = "ˮλ�Ǳ�״̬";
					temp = (m_pSubStation->m_syqState&0x40)>>6;
					if(temp == 0)
						strstate = "����";
					else if(temp == 1)
						strstate = "����";
				}
				break;
			case 7:
				{
					strname = "�ն�����״̬";
					temp = (m_pSubStation->m_syqState&0x80)>>7;
					if(temp == 0)
						strstate = "����";
					else if(temp == 1)
						strstate = "�ر�";
				}
				break;
			case 8:
				{
					strname = "�洢��״̬";
					temp = (m_pSubStation->m_syqState&0x100)>>8;
					if(temp == 0)
						strstate = "����";
					else if(temp == 1)
						strstate = "�쳣";
				}
				break;
			case 9:
				{
					strname = "��������Ч��";
					temp = (m_pSubStation->m_syqState&0x200)>>9;
					if(temp == 0)
						strstate = "IC����Ч";
					else if(temp == 1)
						strstate = "�ر�";
				}
				break;
			case 10:
				{
					strname = "��������Ч��";
					temp = (m_pSubStation->m_syqState&0x400)>>10;
					if(temp == 0)
						strstate = "ICˮ�ù���";
					else if(temp == 1)
						strstate = "ˮ��ͣ��";
				}
				break;

			}

			FillList(i,strname,strstate);
		}
	}
}