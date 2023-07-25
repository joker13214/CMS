#pragma once
#include "UdfBase.h"
#include "UdfGlobalFUNC.h"

class CRtuStation;
class CRtuYxObject:public CUdfBase
{
public:
	CRtuYxObject(void);
	CRtuYxObject(PRTU_YXOBJECT pInfo);
public:
	virtual ~CRtuYxObject(void);

	void SetParent(CRtuStation* _pParent);
	CRtuStation* GetParent();

	bool CheckModify(PRTU_YXOBJECT pNewInfo);		//对比新对象值与本对象属性，得到是否修改  返回值 true:有修改 false:没有修改
	bool UpdateConfigInfo(PRTU_YXOBJECT pInfo);
protected:
	CRtuStation* m_pParent;
};
