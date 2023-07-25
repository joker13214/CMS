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

	bool CheckModify(PRTU_YKOBJECT pNewInfo);			//�Ա��¶���ֵ�뱾�������ԣ��õ��Ƿ��޸�  ����ֵ true:���޸� false:û���޸�
	bool UpdateConfigInfo(PRTU_YKOBJECT pInfo);
protected:
	CRtuStation* m_pParent;
};
