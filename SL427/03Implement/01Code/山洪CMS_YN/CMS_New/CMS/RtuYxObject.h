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

	bool CheckModify(PRTU_YXOBJECT pNewInfo);		//�Ա��¶���ֵ�뱾�������ԣ��õ��Ƿ��޸�  ����ֵ true:���޸� false:û���޸�
	bool UpdateConfigInfo(PRTU_YXOBJECT pInfo);
protected:
	CRtuStation* m_pParent;
};
