#pragma once
#include "UdfGlobalFUNC.h"
#include "XpStyle/ButtonXP.h"
#include "XpStyle/ComboBoxXP.h"
#include "CommServer.h"
#include "GridCtrl/GridCtrl.h"
#include <CString>
#include <map>


// CRtuAttrMsgInit �Ի���
//-------------------���ų�ʼ�����̱�ʶ----------------------------

const BYTE TM_INIT_READY  = 0x01;                //����׼��
const BYTE TM_INIT_START  = 0x02;                //���ſ�ʼ��������
const BYTE TM_INIT_SENDING = 0x03;               //���ų�ʼ��������
const BYTE TM_INIT_FINISHED = 0x04;              //���ų�ʼ������
//-----------------------------------------------------------------

class CRtuAttrMsgInit :public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrMsgInit)

public:
	CRtuAttrMsgInit(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRtuAttrMsgInit();

// �Ի�������
	enum { IDD = IDD_DLG_MSGINIT };

	void SetCommThreadInfo(CCommThreadInfo* pThreadInfo);
	void InitGridItems();//GridCtrl��ʾ
	void AppendRowToGrid(TextMsg *pMsg);
	void StartTimer(int itv = 30000);
	void SendData(TextMsg *pMsg);
	void UpdateGrid(int cmd, int attrType, CString str, int len);
	void SetCmd(int cmd);
	int GetCmd();

protected:
	CButtonXP m_StartInitBtn;
	CButtonXP m_StopInitBtn;
	CGridCtrl m_GridCtrl;
	CEdit     m_MsgInitInfoLet;
	int       m_CurrentRow;

	CCommThreadInfo* m_pCurCommThreadInfo;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

public:
	COM_INFO m_Com_Info;
	NET_INFO m_Net_Info;
	SC_COMM_INFO m_Sc_Comm_Info;
	_SC_TEXTMSG_INFO_ m_Tm_Comm_Info;
	BYTE     m_bCom_Type;
	BYTE     m_bCom_Interface;
	BYTE     m_bProtocolType;
	
	int m_CurrentTmInitIdx;             //��ǰ���ų�ʼ����ϣ������
	int m_CurrentTmState;               //���ų�ʼ��״̬
	std::map<int, CString>::iterator m_it_current;
	int m_nCurrentRowIdxGrid;
	bool m_bReturn;
	TextMsg *m_CurrentSendMsg;//��ǰ���͵�TextMsg

private:
	CCriticalSection m_cs_cmd;
	int m_nCurrentCmd;
	CString m_strRecvInfo;
	

public:
	
	void DoTextMsgInit();
	void Do_AT_();
	void Do_ATE0_();
	void Do_AT_ADD_CGMR_();
	void Do_AT_ADD_CGSN_();
	void Do_AT_ADD_CIMI_();
	void Do_AT_ADD_CMEE_();
	void Do_AT_ADD_CPIN_ASK_();
	void Do_AT_ADD_CREG_();
	void Do_AT_ADD_CREG_ASK_();
	void Do_AT_ADD_CSQ_();
	void Do_AT_ADD_CGREG_();
	void Do_AT_ADD_CGREG_ASK_();
	void Do_AT_ADD_CGATT_ASK_();
	void Do_AT_ADD_CLIP_();
	void Do_AT_ADD_CNMI_();
	void Do_AT_ADD_CMGF_();
	void Do_AT_ADD_CSCA_ASK_();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnInitstart();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnInitstop();
};
