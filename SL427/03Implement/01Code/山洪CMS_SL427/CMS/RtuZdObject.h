#pragma once
#include "UdfBase.h"
#include "UdfGlobalFUNC.h"

class CRtuStation;
class CRtuZdObject:public CUdfBase
{
public:
	CRtuZdObject(void);
	CRtuZdObject(PRTU_ZDOBJECT pInfo);
public:
	virtual ~CRtuZdObject(void);

	void SetParent(CRtuStation* _pParent);
	CRtuStation* GetParent();
	bool CheckModify(PRTU_ZDOBJECT pNewInfo); //对比新对象值与本对象属性，得到是否修改  返回值 true:有修改 false:没有修改
	bool UpdateConfigInfo(PRTU_ZDOBJECT pInfo);
protected:
	CRtuStation* m_pParent;

	BYTE m_Sizeof;
	BYTE m_OrderInItem;
	CString m_Unit;
	FLOAT	m_ScaleMax;
	FLOAT   m_ScaleMin;

public:
	const BYTE GetSizeof();
	const BYTE GetOrderInItem();
	const CString GetUnit();
	const FLOAT GetScaleMax();
	const FLOAT GetScaleMin();
};
