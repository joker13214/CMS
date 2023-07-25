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
// CRtuAttrHydrProSet �Ի���

class CRtuStation;
class CRtuPropertyDlg;

class CRtuAttrHydrProSet : public CDialog
{
	DECLARE_DYNAMIC(CRtuAttrHydrProSet)

public:
	CRtuAttrHydrProSet(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRtuAttrHydrProSet();

// �Ի�������
	enum { IDD = IDD_RTU_PROSET_HYDR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()

public:
	CRtuStation*		m_pSubStation;
	CRtuPropertyDlg*	m_pParentDlg;
	CXTabCtrl           m_wndProTabCtrl;       //Tabҳ�ؼ�

	CRtuAttrHydrAutoRpSet* m_pAutoRpSet;       //TAB_0�Ա�����ѡ��
	RtuAttrHydrTransCtrl* m_pHydrTransCtrl;       //�������
	CRtuAttrHydrWorkCondition *m_pHydrWorkCondition; //������Ϣ
	CRtuAttrHydrInterfaceInfo *m_pHydrInterfaceInfo; //�ӿ���Ϣ
	CRtuAttrHydrWaterLevel *m_pHydrTabWaterLevel;     //ˮλ
	CRtuAttrHydrRain       *m_pHydrTabRainValume;     //����
	CRtuAttrHydrSoilMoisture *m_pHydrSoilMoisture;    //��������
	CRtuAttrHydrInDepthGW    *m_pHydrInDepthGW;       //����ˮ˲ʱ����
	CRtuAttrHydrInTGW        *m_pHydrInTGW;           //����ˮ˲ʱ�¶�
	CRtuAttrHydrEvaporate    *m_pHydrEvaporate;       //����

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
	

public://��Ϣѭ��
	afx_msg void OnDestroy();

};
