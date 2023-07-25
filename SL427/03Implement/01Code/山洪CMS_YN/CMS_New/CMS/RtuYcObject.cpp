#include "StdAfx.h"
#include "RtuYcObject.h"
#include "RtuStation.h"

CRtuYcObject::CRtuYcObject(void)
{
	m_pParent = NULL;


	m_sID = "";
	m_sName = "";
	m_nCode = 0;

	m_xType = 0;
	m_DisplayBits = 7;
	m_PointBits = 6;
	m_Unit = "";
	m_MaxValue = 0.0f;
	m_MinValue = 0.0f;
}

CRtuYcObject::CRtuYcObject(PRTU_YCOBJECT pInfo)
{
	m_pParent = NULL;

	m_sID = pInfo->_strYcObjID;
	m_sName = pInfo->_strName;
	m_nCode = pInfo->_nYcCode;
	
	m_xType = pInfo->_Type;
	m_DisplayBits = pInfo->_DisplayBits;
	m_PointBits = pInfo->_PointBits;
	m_Unit = pInfo->_Unit;
	m_MaxValue = pInfo->_MaxValue;
	m_MinValue = pInfo->_MinValue;

}

CRtuYcObject::~CRtuYcObject(void)
{
}


void CRtuYcObject::SetParent(CRtuStation* _pParent)
{

	m_pParent = _pParent;
}

CRtuStation* CRtuYcObject::GetParent()
{
	return m_pParent;
}


CString CRtuYcObject::GetXTypeString(BYTE xType)
{
	CString strType ="";
    switch (xType)
    {
	case  1:
		strType ="水位";
		break;
	case  2:
		strType = "流量";
		break;
	case  3:
		strType = "水压";
		break;
	case  4:
		strType = "水质";
		break;
	case  5:
		strType = "综合";
		break;
	case 6:
		strType = "其他";
		break;
	}
	return strType;
}

bool CRtuYcObject::CheckModify(PRTU_YCOBJECT pNewInfo)
{
	if (!pNewInfo)		return false;

	if (pNewInfo->_DisplayBits != GetDisplayBits()) return true;
	if (pNewInfo->_MaxValue != GetMaxValue())	return true;
	if (pNewInfo->_MinValue != GetMinValue())   return true;
	if (pNewInfo->_nYcCode != GetCode())	return true;
	if (pNewInfo->_PointBits != GetPointBits()) return true;
	if (pNewInfo->_strName != GetName()) return true;
	if (pNewInfo->_Type !=  GetXType()) return true;
	if (pNewInfo->_Unit != GetUnit()) return true;

	return false;
}

bool CRtuYcObject::UpdateConfigInfo(PRTU_YCOBJECT pInfo)
{
	if (!pInfo)	return false;

	m_sName = pInfo->_strName;
	m_nCode = pInfo->_nYcCode;

	m_xType = pInfo->_Type;
	m_DisplayBits = pInfo->_DisplayBits;
	m_PointBits = pInfo->_PointBits;
	m_Unit = pInfo->_Unit;
	m_MaxValue = pInfo->_MaxValue;
	m_MinValue = pInfo->_MinValue;

	return true;
}