#include "StdAfx.h"
#include "RtuYxObject.h"
#include "RtuStation.h"

CRtuYxObject::CRtuYxObject(void)
{
	m_pParent = NULL;

	m_sID		= "";
	m_sName		= "";
	m_nCode		= 0;
}

CRtuYxObject::CRtuYxObject(PRTU_YXOBJECT pInfo)
{
	m_pParent = NULL;

	m_sID		= pInfo->_strYxObjID;
	m_sName		= pInfo->_strName;
	m_nCode		= pInfo->_nYxCode;

}

CRtuYxObject::~CRtuYxObject(void)
{
}



void CRtuYxObject::SetParent(CRtuStation* _pParent)
{
	m_pParent = _pParent;
}


CRtuStation* CRtuYxObject::GetParent()
{

	return m_pParent;
}

bool CRtuYxObject::CheckModify(PRTU_YXOBJECT pNewInfo)
{
	if (!pNewInfo)	return false;

	if (pNewInfo->_nYxCode != GetCode())	return true;
	if (pNewInfo->_strName != GetName())	return true;

	return false;
}

bool CRtuYxObject::UpdateConfigInfo(PRTU_YXOBJECT pInfo)
{
	if (!pInfo)	return false;

	m_sName		= pInfo->_strName;
	m_nCode		= pInfo->_nYxCode;
	return true;
}