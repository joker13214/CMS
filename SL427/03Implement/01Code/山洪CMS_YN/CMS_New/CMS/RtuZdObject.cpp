#include "StdAfx.h"
#include "RtuZdObject.h"
#include "RtuStation.h"

CRtuZdObject::CRtuZdObject(void)
{
	m_pParent = NULL;

	m_sName			= "";
	m_sID			= "";
	m_nCode			= 0;

	m_Sizeof		= 0 ;
	m_OrderInItem	= 0;
	m_Unit			= "";
	m_ScaleMax		= 0.0f;
	m_ScaleMin		= 0.0f;
}


CRtuZdObject::CRtuZdObject(PRTU_ZDOBJECT pInfo)
{
	m_pParent = NULL;

	m_sName			= pInfo->_strName;
	m_sID			= pInfo->_strZdObjID;
	m_nCode			= pInfo->_nZDCode;

	m_Sizeof		= pInfo->_Sizeof;
	m_OrderInItem	= pInfo->_OrderInItem;
	m_Unit			= pInfo->_Unit;
	m_ScaleMax		= pInfo->_ScaleMax;
	m_ScaleMin		= pInfo->_ScaleMin;
}

CRtuZdObject::~CRtuZdObject(void)
{

}

void CRtuZdObject::SetParent(CRtuStation* _pParent)
{
	m_pParent = _pParent;
}

CRtuStation* CRtuZdObject::GetParent()
{

	return m_pParent;
}


const BYTE CRtuZdObject::GetSizeof()
{
	return m_Sizeof;
}

 const BYTE CRtuZdObject::GetOrderInItem()
{
	return m_OrderInItem;
}

const CString CRtuZdObject::GetUnit()
{
	return m_Unit;
}

const FLOAT CRtuZdObject::GetScaleMax()
{
	return m_ScaleMax;
}

const FLOAT CRtuZdObject::GetScaleMin()
{
	return m_ScaleMin;
}


bool CRtuZdObject::CheckModify(PRTU_ZDOBJECT pNewInfo)
{
	if (!pNewInfo)		return false;
	if (pNewInfo->_nZDCode != GetCode())	return true;
	if (pNewInfo->_OrderInItem != GetOrderInItem()) return true;
	if (pNewInfo->_ScaleMax != GetScaleMax())	return true;
	if (pNewInfo->_ScaleMin != GetScaleMin())	return true;
	if (pNewInfo->_Sizeof != GetSizeof())	return true;
	if (pNewInfo->_strName != GetName())	return true;
	if (pNewInfo->_Unit != GetUnit())	return true;

	return false;
}


bool CRtuZdObject::UpdateConfigInfo(PRTU_ZDOBJECT pInfo)
{
	if (!pInfo)	return false;

	m_sName			= pInfo->_strName;
	m_nCode			= pInfo->_nZDCode;

	m_Sizeof		= pInfo->_Sizeof;
	m_OrderInItem	= pInfo->_OrderInItem;
	m_Unit			= pInfo->_Unit;
	m_ScaleMax		= pInfo->_ScaleMax;
	m_ScaleMin		= pInfo->_ScaleMin;
	return true;
}