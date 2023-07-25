#pragma once


// CRtuAttrDataShow 对话框
class CRtuPropertyDlg;

class CRtuAttrDataShow : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrDataShow)

public:
	CRtuAttrDataShow(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRtuAttrDataShow();
	void  Update();							  //显示信息更新		
	void  SetParentDlg(CRtuPropertyDlg* pParent) {m_pParentDlg = pParent;}
	void  AddMessageToWindow(CString strTime,CString strMessage);   //添加信息到GridCtrl显示
	void  AddMessageToWindow(CString strMessage,PSYSTEMTIME pTime = NULL);

// 对话框数据
	enum { IDD = IDD_RTU_DATASHOW };
protected:
	CRtuPropertyDlg* m_pParentDlg;                      //TabCtrl的父窗口
	CRtuStation*     m_pSubStation;						//此窗口当前的RtuStation对象

	int				 m_nRowsMax;						//显示数据信息的最大行数，默认为100
public:
	CGridCtrl        m_wndGridCtrl;                     //显示数据信息的GridCtrl
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	BOOL    OnInitGridCtrl();
public:
	void    InitGridHeader();							//初始化GridCtrl的headers

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

public:
	afx_msg void OnRClickGridCtrl(NMHDR* pNotifyStruct,LRESULT *pResutl);//GridCtrl的右键事件
	afx_msg void OnClearMessage();						//清空所有信息
	afx_msg void OnSaveAsMessage();						//保存数据信息

};
