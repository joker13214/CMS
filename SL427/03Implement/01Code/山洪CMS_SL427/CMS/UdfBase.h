#pragma once

#include "GridCtrl/BtnDataBase.h"
class CUdfBase
{
public:
	CUdfBase(void);
public:
	virtual ~CUdfBase(void);
protected:
	CString m_sID;
	CString m_sName;
	int     m_nCode;

public:
	const CString GetID()  {return m_sID;}
	const CString GetName(){return m_sName;}
	const int     GetCode(){return m_nCode;}

public:
	CBtnDataBase m_btnDbEdit;   //ÐÞ¸Ä Btn
	CBtnDataBase m_btnDbDel;    //É¾³ý Btn
	CBtnDataBase m_btnPro;      //ÊôÐÔ Btn
};
