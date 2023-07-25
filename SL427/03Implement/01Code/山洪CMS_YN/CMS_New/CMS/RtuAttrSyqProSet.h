#pragma once
#include "afxcmn.h"
#include "XTabCtrl.h"
#include "RtuAttrSyqcfg1.h"
#include "RtuAttrSyqcfg2.h"
#include "RtuAttrSyqcfg3.h"
#include "RtuAttrSyqCallData.h"
#include "RtuAttrSyqCallData1.h"
#include "RtuAttrSyqCallData2.h"
#include "RtuAttrSyqCallState.h"



class CRtuStation;
class CRtuPropertyDlg;


// CRtuAttrSyqProSet 对话框

class CRtuAttrSyqProSet : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrSyqProSet)

public:
	CRtuAttrSyqProSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRtuAttrSyqProSet();

// 对话框数据
	enum { IDD = IDD_RTU_PROSET_SYQ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CRtuStation*		m_pSubStation;
	CRtuPropertyDlg*	m_pParentDlg;
	CXTabCtrl           m_wndProTabCtrl;       //Tab页控件


	CRtuAttrSyqcfg1 *m_pSyqcfg1;
	CRtuAttrSyqcfg2 *m_pSyqcfg2;
	CRtuAttrSyqcfg3 *m_pSyqcfg3;
	CRtuAttrSyqCalldata *m_pSyqCalldata;
	CRtuAttrSyqCallData1 *m_pSyqCalldata1;
	CRtuAttrSyqCallData2 *m_pSyqCalldata2;
	CRtuAttrSyqCallState *m_pSyqCallState;
public: 
	CRtuAttrSyqcfg1 *GetSyqTab_Config1(){return m_pSyqcfg1;}
	CRtuAttrSyqcfg2 *GetSyqTab_Config2(){return m_pSyqcfg2;}
	CRtuAttrSyqcfg3 *GetSyqTab_Config3(){return m_pSyqcfg3;}
	CRtuAttrSyqCalldata *GetSyqTab_Calldata(){return m_pSyqCalldata;}
	CRtuAttrSyqCallData1 *GetSyqTab_Calldata1(){return m_pSyqCalldata1;}
	CRtuAttrSyqCallData2 *GetSyqTab_Calldata2(){return m_pSyqCalldata2;}
	CRtuAttrSyqCallState *GetSyqTab_CalldState(){return m_pSyqCallState;}
public:

	virtual BOOL OnInitDialog();
	void SetParentDlg(CRtuPropertyDlg* pParentDlg){m_pParentDlg = pParentDlg;}
	void Update();


};
