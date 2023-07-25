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

	void		 SetParent(CRtuStation* _pParent);		//设置所属的RTU 对象
	CRtuStation* GetParent();							//获取所属的RTU 对象

	bool		 CheckModify(PRTU_YCOBJECT pNewInfo);	//对比新对象值与本对象属性，得到是否修改  返回值 true:有修改 false:没有修改
protected:
	CRtuStation* m_pParent;

	BYTE	m_xType;									//类型
	BYTE	m_DisplayBits;								//显示位数
	BYTE	m_PointBits;								//小数点位数
	CString	m_Unit;										//单位
	FLOAT	m_MaxValue;									//最大值
	FLOAT	m_MinValue;									//最小值

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
