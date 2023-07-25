#pragma once
#include "UdfBase.h"
#include "UdfGlobalFUNC.h"


class CRtuStation;
class CRtuYcObject:public CUdfBase
{
public:
	CRtuYcObject(void);
	CRtuYcObject(PRTU_YCOBJECT pInfo);
public:
	virtual ~CRtuYcObject(void);

	void		 SetParent(CRtuStation* _pParent);		//����������RTU ����
	CRtuStation* GetParent();							//��ȡ������RTU ����

	bool		 CheckModify(PRTU_YCOBJECT pNewInfo);	//�Ա��¶���ֵ�뱾�������ԣ��õ��Ƿ��޸�  ����ֵ true:���޸� false:û���޸�
protected:
	CRtuStation* m_pParent;

	BYTE	m_xType;									//����
	BYTE	m_DisplayBits;								//��ʾλ��
	BYTE	m_PointBits;								//С����λ��
	CString	m_Unit;										//��λ
	FLOAT	m_MaxValue;									//���ֵ
	FLOAT	m_MinValue;									//��Сֵ

public:
	const BYTE GetXType(){return m_xType;}
	const BYTE GetDisplayBits(){return m_DisplayBits;}
	const BYTE GetPointBits(){return m_PointBits;}
	const CString GetUnit() {return m_Unit;}
	const FLOAT GetMaxValue(){return m_MaxValue;}
	const FLOAT GetMinValue(){return m_MinValue;}
	static CString GetXTypeString(BYTE xType);
	bool  UpdateConfigInfo(PRTU_YCOBJECT pInfo);
};
