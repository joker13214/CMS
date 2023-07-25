// RtuAttrSyqCallData2.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuAttrSyqCallData2.h"


#define W_TIMER_CALLDATA   0x01

// CRtuAttrSyqCallData2 �Ի���

IMPLEMENT_DYNAMIC(CRtuAttrSyqCallData2, CDialog)

CRtuAttrSyqCallData2::CRtuAttrSyqCallData2(CWnd* pParent /*=NULL*/)
	: CDialog(CRtuAttrSyqCallData2::IDD, pParent)
{

}

CRtuAttrSyqCallData2::~CRtuAttrSyqCallData2()
{
}

void CRtuAttrSyqCallData2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_YAOSU_DATA, m_listdata);
	DDX_Control(pDX, IDC_LIST_YAOSU, m_yaosulist1);
	DDX_Control(pDX, IDC_LIST_YAOSU2, m_yaosulist2);
	DDX_Control(pDX, IDC_BTN_ADD, m_btn_add);
	DDX_Control(pDX, IDC_BTN_DEL, m_btn_del);
	DDX_Control(pDX, IDC_BTN_QUERY, m_btn_query);

}


BEGIN_MESSAGE_MAP(CRtuAttrSyqCallData2, CDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_ADD, &CRtuAttrSyqCallData2::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL, &CRtuAttrSyqCallData2::OnBnClickedBtnDel)
	ON_BN_CLICKED(IDC_BTN_QUERY, &CRtuAttrSyqCallData2::OnBnClickedBtnQuery)
	ON_LBN_DBLCLK(IDC_LIST_YAOSU, &CRtuAttrSyqCallData2::OnLbnDblclkListYaosu)
	ON_LBN_DBLCLK(IDC_LIST_YAOSU2, &CRtuAttrSyqCallData2::OnLbnDblclkListYaosu2)
END_MESSAGE_MAP()


// CRtuAttrSyqCallData2 ��Ϣ�������

BOOL CRtuAttrSyqCallData2::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_bResult = FALSE;

	if (m_yaosulist1.GetSafeHwnd())
	{
		m_yaosulist1.InsertString(0,"1minʱ�ν�ˮ��");//PN01

		m_yaosulist1.InsertString(1,"5minʱ�ν�ˮ��");//PN05

		m_yaosulist1.InsertString(2,"10minʱ�ν�ˮ��");//PN10

		m_yaosulist1.InsertString(3,"30minʱ�ν�ˮ��");//PN30

		m_yaosulist1.InsertString(4,"1hʱ�ν�ˮ��");//P1

		m_yaosulist1.InsertString(5,"2hʱ�ν�ˮ��");//P2

		m_yaosulist1.InsertString(6,"3hʱ�ν�ˮ��");//P3

		m_yaosulist1.InsertString(7,"6hʱ�ν�ˮ��");//P6

		m_yaosulist1.InsertString(8,"12hʱ�ν�ˮ��");//P12

		m_yaosulist1.InsertString(9,"�ս�ˮ��");//PD

		m_yaosulist1.InsertString(10,"�½�ˮ��");//PM

		m_yaosulist1.InsertString(11,"��������");//PJ

		m_yaosulist1.InsertString(12,"������");//PR

		m_yaosulist1.InsertString(13,"��ˮ���ۼ�ֵ");//PT

		m_yaosulist1.InsertString(14,"˲ʱ�ӵ�ˮλ����λ");//Z

		m_yaosulist1.InsertString(15,"��������");//ED

		m_yaosulist1.InsertString(16,"˲ʱ��������ˮ����");//Q
	}
	

     m_yaosulist1.SetCurSel(0);

	 DWORD dwStyle = m_listdata.GetExtendedStyle();
	 dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
	 dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��
	 //dwStyle |= LVS_EX_CHECKBOXES;//itemǰ����checkbox�ؼ�
	 m_listdata.SetExtendedStyle(dwStyle); //������չ���

	m_listdata.InsertColumn(0,_T("Ҫ������"),LVCFMT_LEFT,150);
	m_listdata.InsertColumn(1,_T("����"),LVCFMT_LEFT,120);


	return TRUE;
}

void CRtuAttrSyqCallData2::SetParentDlg(CRtuPropertyDlg* pDlg)
{
	m_pParentDlg = pDlg;	
}

void CRtuAttrSyqCallData2::Update()
{
	if(m_pParentDlg)
		m_pSubStation = m_pParentDlg->GetRtuStation();	
}


void CRtuAttrSyqCallData2::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CString strLog= "";
	_Log_level_type LogLevel = Normal_Log_Message;

	switch (nIDEvent)
	{
	case W_TIMER_CALLDATA:
		{
			if (m_bResult == TRUE)
			{
				FillDataList();
				strLog = "��ѯң��վָ��Ҫ������ �ɹ�!";
			}
			else if ( m_bResult == FALSE)
			{
				strLog = "��ѯң��վָ��Ҫ������ ��ʱ!";
				LogLevel = Warning_log_Message;
			}
			else if (m_bResult == 0xff)
			{
				strLog = "��ѯң��վָ��Ҫ������ ����!";
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


void CRtuAttrSyqCallData2::OnBnClickedBtnAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int sel = m_yaosulist1.GetCurSel();
	CString str = "";
	m_yaosulist1.GetText(sel,str);

	if(str !="")
	{
		if(m_yaosulist2.GetCurSel() == -1)
		{
			m_yaosulist2.AddString(str);
			m_yaosulist2.SetCurSel(0);
		}
		else
		{
			int n = m_yaosulist2.FindString(0,str);
			if(n >=0)
			{
				AfxMessageBox("Ҫ���Ѵ��ڣ�");
			}
			else
			{
				m_yaosulist2.AddString(str);
				m_yaosulist2.SetCurSel(m_yaosulist2.GetCurSel()+1);
			}

		}

		m_yaosulist1.SetCurSel(m_yaosulist1.GetCurSel()+1);

		
	}

}

void CRtuAttrSyqCallData2::OnBnClickedBtnDel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int sel = m_yaosulist2.GetCurSel();
	m_yaosulist2.DeleteString(sel);
	if(sel>=1)
		m_yaosulist2.SetCurSel(sel-1);
	else
	{
		if(m_yaosulist2.GetCount()>0)
		{
			sel = m_yaosulist2.GetCount();
			m_yaosulist2.SetCurSel(sel -1);
		}
	}


}



void CRtuAttrSyqCallData2::OnLbnDblclkListYaosu()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	int sel = m_yaosulist1.GetCurSel();
	CString str = "";
	m_yaosulist1.GetText(sel,str);

	if(str !="")
	{
		
		int n = m_yaosulist2.FindString(0,str);
			if(n >=0)
			{
				AfxMessageBox("Ҫ���Ѵ��ڣ�");
			}
			else
			{
				m_yaosulist2.AddString(str);
				m_yaosulist2.SetCurSel(m_yaosulist2.GetCurSel()+1);
			}
	}
}

void CRtuAttrSyqCallData2::OnLbnDblclkListYaosu2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int sel = m_yaosulist2.GetCurSel();
	m_yaosulist2.DeleteString(sel);
	if(sel>=1)
		m_yaosulist2.SetCurSel(sel-1);
	else
	{
		if(m_yaosulist2.GetCount()>0)
			m_yaosulist2.SetCurSel(m_yaosulist2.GetCount()-1);
	}

}

void CRtuAttrSyqCallData2::OnBnClickedBtnQuery()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int num =m_yaosulist2.GetCount();
	CString str1 = "",str="";
	if(num == 0)
	{
			AfxMessageBox("��ָ��Ҫ�أ���ѡ��");
			return;
	}
	else
	{
		for (int i =0; i<num; i++)
		{
			m_yaosulist2.GetText(i,str);
			std::map<CString, SYQ_YS_STRUCT>::iterator it_map;
			//it_map = SYQ_TypeMap::GetInstance()->m_SYQTypeMap.find(tempstr);
			//if (it_map != SYQ_TypeMap::GetInstance()->m_SYQTypeMap.end())
			for(it_map = SYQ_TypeMap::GetInstance()->m_SYQTypeMap.begin();it_map != SYQ_TypeMap::GetInstance()->m_SYQTypeMap.end();it_map++)
			{
				if(it_map->second.name == str)
					break;
			}

			str = str +it_map->second.ename;


		}

		if (m_pSubStation->blEnbaleTrans())
		{
			CSYQFrameFormat appFrame;
			m_pSubStation->BuildBasicPacket(appFrame);  //���ɳ�ʼ���·�֡����֡������Ϣ
			//������ݴ��
			m_pSubStation->GetSysDataTrans_Syq()->SYQ_EN_37H(&appFrame,str);

			UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			SetTimer(W_TIMER_CALLDATA, nElapse, NULL);
			SetEnable(FALSE);
		}


	}

}

void CRtuAttrSyqCallData2::SetEnable(bool b)
{
	m_yaosulist1.EnableWindow(b);
	m_yaosulist2.EnableWindow(b);
	m_btn_add.EnableWindow(b);
	m_btn_del.EnableWindow(b);
	m_btn_query.EnableWindow(b);
}


void CRtuAttrSyqCallData2::UpdateDlgMessage(int attr, bool result)
{
	m_bResult = result;

	if(attr == syq_call_data)
	{ 
		SendMessage(WM_TIMER,W_TIMER_CALLDATA, NULL);
	}

}

void CRtuAttrSyqCallData2::FillDataList()
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
