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

//�û���Ϣ
//���:2011-03-28
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
	bool  CheckPwd(CString strPwd);                                          //��֤���룬true ������ȷ�� false �������
protected:
	CString m_strUserName;    //�û�����
	CString m_strUserID;      //�û�����
	BYTE    m_bType;          //���1 ϵͳ����Ա�� 2 ����Ա
	BYTE    m_onDuty;         //��ְ/���  0 ��ڣ�1 ��ְ
	CString m_strPwd;         //�û�����
public:
	CBtnDataBase m_btnModify;													//�޸İ�ť
	CBtnDataBase m_btnDelete;													//ɾ����ť
};
