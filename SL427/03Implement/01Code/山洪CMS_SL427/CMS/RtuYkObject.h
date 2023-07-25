#pragma once
#include "UdfBase.h"
#include "UdfGlobalFUNC.h"

class CRtuStation;
class CRtuYkObject:public CUdfBase
{
public:
	CRtuYkObject(void);
	CRtuYkObject(PRTU_YKOBJECT pInfo);
public:
	virtual ~CRtuYkObject(void);

	void SetParent(CRtuStation* _pParent);
	CRtuStation* GetParent();

	bool CheckModify(PRTU_YKOBJECT pNewInfo);			//对比新对象值与本对象属性，得到是否修改  返回值 true:有修改 false:没有修改
	bool UpdateConfigInfo(PRTU_YKOBJECT pInfo);
protected:
	CRtuStation* m_pParent;
};
