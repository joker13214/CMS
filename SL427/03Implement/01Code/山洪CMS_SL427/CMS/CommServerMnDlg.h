#pragma once
#include "afxcmn.h"
#include "UdfListCtrl.h"
#include "UdfLogSystem.h"
#include "XpStyle/ButtonXP.h"

// CCommServerMnDlg 对话框

class CCommServerMnDlg : public CDialog
{
	DECLARE_DYNAMIC(CCommServerMnDlg)

public:
	CCommServerMnDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCommServerMnDlg();

	CImageList	m_wndImagelist;
	CGridCtrl	m_wndThreadsGrid;
	CUdfListCtrl m_wndThreadLog;

// 对话框数据
	enum { IDD = IDD_DLG_COMMSERVER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnRefresh();
	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnLClickThreadGrid(NMHDR *pNotifyStruct,LRESULT *pResult);
	afx_msg void OnBnClickedButton1();							//添加新线程
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	virtual BOOL OnInitDialog();

public:
	bool InitGridHeader();
	void InitListCtrlHeader();
	bool ShowAllThreadsInfo();     //显示所有的线程信息
	void ShowOneThreadInfo(CCommThreadInfo* pThread,int ItemRow);
	void UpdateThreadState(CCommThreadInfo* pThread,int ItemRow); //更新GridCtrl上线程的运行状态信息
	void InsertLog(CString strLog,_Log_level_type xLevle=Normal_Log_Message,PSYSTEMTIME pST=NULL);		  //插入日志信息
    void SetTextMsgDlg(CRtuAttrTextMsg *pDlg);

protected:
	// 线程操作相关日志信息
	CRtuAttrTextMsg *m_pTextMsgDlg;
	
public:
private:
	bool ModifyCommThreadPro(CCommThreadInfo* pThread);        //修改线程  返回值 false 未修改、修改失败, true 有修改、并修改成功
	bool DeleteCommThreadPro(CCommThreadInfo* pTHread);        //删除线程
	bool InitMsgCommThreadPro(CCommThreadInfo* pThread);       //初始化短信流程

	CButtonXP m_btnAdd;
	CButtonXP m_btnRefresh;
	CButtonXP m_btnOK;
};

