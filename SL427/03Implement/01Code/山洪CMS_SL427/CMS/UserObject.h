#pragma once


typedef struct _USER_INFORMATION_
{
	CString sName;
	CString sID;
	CString sPwd;
	BYTE    bType;
	BYTE    bOnDuty;
	_USER_INFORMATION_()
	{
		sName = "";
		sID = "";
		sPwd = "";
		bType = 2;
		bOnDuty = 1;
	}
}USER_INFO,*PUSER_INFO;

//用户信息
//添加:2011-03-28
class CUserObject
{
public:
	CUserObject(void); 
	CUserObject(PUSER_INFO pUserInfo);
	~CUserObject(void);
public:
	void SetName(CString _Name);
	void SetID(CString _ID);
	void SetType(BYTE _Type);
	void SetOnDuty(BYTE _OnDuty);
	void SetPwd(CString _Pwd);

	const CString GetName() {return m_strUserName;}
	const CString GetID()	{return m_strUserID;}
	const CString GetPwd()	{return m_strPwd;}
	const BYTE  GetType()	{return m_bType;}
	const BYTE  GetOnDuty()	{return m_onDuty;}

	bool  CheckInfo(PUSER_INFO pUserInfo);
	bool  UpdateInfo(PUSER_INFO pUserInfo);
	bool  CheckPwd(CString strPwd);                                          //验证密码，true 密码正确， false 密码错误
protected:
	CString m_strUserName;    //用户姓名
	CString m_strUserID;      //用户编码
	BYTE    m_bType;          //类别，1 系统管理员， 2 操作员
	BYTE    m_onDuty;         //在职/离岗  0 离岗，1 在职
	CString m_strPwd;         //用户密码
public:
	CBtnDataBase m_btnModify;													//修改按钮
	CBtnDataBase m_btnDelete;													//删除按钮
};
