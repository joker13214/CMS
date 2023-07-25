#include "StdAfx.h"
#include "UserObject.h"


CUserObject::CUserObject(void)
{
	m_strUserName = "";
	m_strUserID = "";
	m_strPwd = "";
	m_bType = 2;
	m_onDuty = 1;
}

CUserObject::CUserObject(PUSER_INFO pUserInfo)
{
	if (pUserInfo)
	{
		m_strUserName	= pUserInfo->sName;
		m_strUserID		= pUserInfo->sID;
		m_strPwd		= pUserInfo->sPwd;
		m_bType			= pUserInfo->bType;
		m_onDuty		= pUserInfo->bOnDuty;
	}
}

CUserObject::~CUserObject(void)
{
}

void CUserObject::SetName(CString _Name)
{
	m_strUserName = _Name;
}

void CUserObject::SetID(CString _ID)
{
	m_strUserID = _ID;
}

void CUserObject::SetType(BYTE _Type)
{
	m_bType = _Type;
}

void CUserObject::SetOnDuty(BYTE _OnDuty)
{
	m_onDuty = _OnDuty;
}

void CUserObject::SetPwd(CString _Pwd)
{
	m_strPwd = _Pwd;
}


bool CUserObject::CheckInfo(PUSER_INFO pUserInfo)
{
	if (!pUserInfo) return false;

	if (pUserInfo->sName != m_strUserName) return true;
	if (pUserInfo->sID != m_strUserID) return true;
	if (pUserInfo->sPwd != m_strPwd) return true;
	if (pUserInfo->bType != m_bType) return true;
	if (pUserInfo->bOnDuty != m_onDuty) return true;
	
	return false;
}

bool CUserObject::UpdateInfo(PUSER_INFO pUserInfo)
{
	if (!pUserInfo) return false;

	m_strUserName	= pUserInfo->sName;
	m_strUserID		= pUserInfo->sID;
	m_strPwd		= pUserInfo->sPwd;
	m_bType			= pUserInfo->bType;
	m_onDuty		= pUserInfo->bOnDuty;

	return true;
}


bool CUserObject::CheckPwd(CString strPwd)
{
	if (strPwd == m_strPwd)
		return true;
	else
		return false;

	return false;
}
