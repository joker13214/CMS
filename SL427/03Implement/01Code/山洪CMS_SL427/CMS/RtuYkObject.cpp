#include "StdAfx.h"
#include "RtuYkObject.h"
#include "RtuStation.h"

CRtuYkObject::CRtuYkObject(void)
{
	m_pParent = NULL;
	m_sID = "";
	m_sName = "";
	m_nCode = 0;
}

CRtuYkObject::CRtuYkObject(PRTU_YKOBJECT pInfo)
{
	m_pParent = NULL;
	m_sID   = pInfo->_strYkObjID;
	m_sName = pInfo->_strName;
	m_nCode = pInfo->_nYkCode;

}

CRtuYkObject::~CRtuYkObject(void)
{
}


void CRtuYkObject::SetParent(CRtuStation* _pParent)
{
	m_pParent = _pParent;
}

CRtuStation* CRtuYkObject::GetParent()
{

	return m_pParent;
}


bool CRtuYkObject::CheckModify(PRTU_YKOBJECT pNewInfo)
{
	if (!pNewInfo)   return false;
	if (pNewInfo->_nYkCode != GetCode()) return true;
	if (pNewInfo->_strName != GetName()) return true;

	return false;
}

bool CRtuYkObject::UpdateConfigInfo(PRTU_YKOBJECT pInfo)
{
	if (!pInfo)	return false;

	m_sName = pInfo->_strName;
	m_nCode = pInfo->_nYkCode;
	return true;
}