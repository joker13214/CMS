#pragma once
#include "XpStyle/ButtonXP.h"
#include "XpStyle/ComboBoxXP.h"
#include "GridCtrl/GridCtrl.h"
#include "UdfGlobalFUNC.h"
#include <list>

// CRtuAttrTextMsg 对话框
class TM_Unit;
class CRtuAttrTextMsg : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrTextMsg)

public:
	CRtuAttrTextMsg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRtuAttrTextMsg();

// 对话框数据
	enum { IDD = IDD_RTU_TEXTMSG };

public:
	void SetCommThreadInfo(CCommThreadInfo* pThreadInfo);
	void SetParentDlg(CRtuPropertyDlg* pDlg);
	void Update();										//更新界面的显示信息
	void UpdateDlg(int attrType,BOOL blResult,TM_Unit *pUnit);			//接收到报文后、更新界面
	void KillAllTimers();
	void SetWndsEnable(BOOL blEnable);					//设置按钮是否可用
	void InitTmList();								//GridCtrl显示
	void OnCellReadOnly(int Row,int Col);
	void RecvSmsHint(int attrType, int No);

	void SetCmd(int cmd);
	void SendData(TextMsg *m_CurrentSendMsg);
	void AppendRowToGrid();
	void ShowDetailGridItem(int nRowIdx, TM_Unit *pUnit);
	void UpdateGrid();

	void ReadMsg(int No);

	virtual BOOL OnInitDialog();

	int GetCmd();

protected:
	CRtuPropertyDlg* m_pParentDlg;

public:
	COM_INFO m_Com_Info;
	NET_INFO m_Net_Info;
	SC_COMM_INFO m_Sc_Comm_Info;
	_SC_TEXTMSG_INFO_ m_Tm_Comm_Info;
	BYTE     m_bCom_Type;
	BYTE     m_bCom_Interface;
	BYTE     m_bProtocolType;

	CGridCtrl   m_wndTmListGrid;
	CRtuStation* m_pSubStation;						//此窗口当前的RtuStation对象
	CEdit     m_InitTmDetail;

	CComboBoxXP m_cpmsCbx;
	CComboBoxXP m_csmpCbx;
	CEdit       m_cpmsaCbx;
	CComboBoxXP m_cmgfCbx;

	BOOL			 m_blResult;
	TextMsg *m_pReadMsg;

private:
	int m_nCurrentCmd;
	TM_Unit* m_pTmUnit;
	int m_nCurrentSetIdx;
	int m_nCurrentSendIdx;
protected:
	CCommThreadInfo* m_pCurCommThreadInfo;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnCpms();
	afx_msg void OnBnClickedBtnCpmsa();
	afx_msg void OnBnClickedBtnCmgf();
	afx_msg void OnBnClickedBtnCsmp();
	afx_msg void OnRClickGridCtrl(NMHDR *pNotifyStruct,LRESULT *pResult);
	afx_msg void OnDLClickGridCtrl(NMHDR *pNotifyStruct,LRESULT *pResult);
	afx_msg void OnDeleteSMS();

private:
	TM_Unit *m_selTmUnit;
};
