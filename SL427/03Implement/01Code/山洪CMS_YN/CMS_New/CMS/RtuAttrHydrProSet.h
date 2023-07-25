#pragma once
#include "afxcmn.h"
#include "XTabCtrl.h"
#include "RtuAttrHydrAutoRpSet.h"
#include "RtuAttrHydrWaterLevel.h"
#include "RtuAttrHydrRain.h"
#include "RtuAttrHydrEvaporate.h"
#include "RtuAttrHydrInDepthGW.h"
#include "RtuAttrHydrSoilMoisture.h"
#include "RtuAttrHydrInTGW.h"
#include "RtuAttrHydrTransCtrl.h"
#include "RtuAttrHydrWorkCondition.h"
#include "RtuAttrHydrInterfaceInfo.h"
// CRtuAttrHydrProSet 对话框

class CRtuStation;
class CRtuPropertyDlg;

class CRtuAttrHydrProSet : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrHydrProSet)

public:
	CRtuAttrHydrProSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRtuAttrHydrProSet();

// 对话框数据
	enum { IDD = IDD_RTU_PROSET_HYDR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()

public:
	CRtuStation*		m_pSubStation;
	CRtuPropertyDlg*	m_pParentDlg;
	CXTabCtrl           m_wndProTabCtrl;       //Tab页控件

	CRtuAttrHydrAutoRpSet* m_pAutoRpSet;       //TAB_0自报类型选项
	RtuAttrHydrTransCtrl* m_pHydrTransCtrl;       //传输控制
	CRtuAttrHydrWorkCondition *m_pHydrWorkCondition; //工况信息
	CRtuAttrHydrInterfaceInfo *m_pHydrInterfaceInfo; //接口信息
	CRtuAttrHydrWaterLevel *m_pHydrTabWaterLevel;     //水位
	CRtuAttrHydrRain       *m_pHydrTabRainValume;     //雨量
	CRtuAttrHydrSoilMoisture *m_pHydrSoilMoisture;    //土地墒情
	CRtuAttrHydrInDepthGW    *m_pHydrInDepthGW;       //地下水瞬时埋深
	CRtuAttrHydrInTGW        *m_pHydrInTGW;           //地下水瞬时温度
	CRtuAttrHydrEvaporate    *m_pHydrEvaporate;       //蒸发

	void SetRtuStation(CRtuStation* pSub);
	void SetParentDlg(CRtuPropertyDlg* pParentDlg){m_pParentDlg = pParentDlg;}

	virtual BOOL OnInitDialog();

	void Update();

public:

	CRtuAttrHydrInterfaceInfo *GetHydrInterfaceInfo(){return m_pHydrInterfaceInfo;}
	CRtuAttrHydrWorkCondition *GetHydrWorkCondition(){return m_pHydrWorkCondition;}
	RtuAttrHydrTransCtrl * GetHydrTransCtrl(){return m_pHydrTransCtrl;}
	CRtuAttrHydrWaterLevel* GetHydrTab_WaterLevel(){return m_pHydrTabWaterLevel;}
	CRtuAttrHydrRain*       GetHydrTab_RainValume(){return m_pHydrTabRainValume;}
	CRtuAttrHydrSoilMoisture* GetHydrTab_SoilMoisture(){return m_pHydrSoilMoisture;}
	CRtuAttrHydrInDepthGW* GetHydrTab_InDepthGW(){return m_pHydrInDepthGW;}
	CRtuAttrHydrInTGW* GetHydrTab_InTGW(){return m_pHydrInTGW;}
	CRtuAttrHydrEvaporate* GetHydrTab_Evaporate(){return m_pHydrEvaporate;}
	

public://消息循环
	afx_msg void OnDestroy();

};
