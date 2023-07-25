#pragma once
#include <map>

/*
  ���������� ʵʱ���ݿ��ʡ���ء��ض���
  �ļ���:UdfAdDivCode.h
  �����������ļ���ʽ
  ����������,����
*/


/*
  ���������� ����
*/
class CUdfBaseZoningCode
{
protected:
	BYTE m_bZoningCode;  //������
	CString m_sName;     //����
public:
	CUdfBaseZoningCode()
	{
		m_bZoningCode = 0;
		m_sName = "";
	}
	~CUdfBaseZoningCode(){}
	const BYTE GetZoningCode(){return m_bZoningCode;} //��ȡ����������
	const CString GetName(){return m_sName;}          //��ȡ����
    
	void SetZoningCode(BYTE bZoningCode){m_bZoningCode = bZoningCode;}  //��������������
	void SetName(CString sName){m_sName = sName;}						//���ö�Ӧ������
};


class CUdfAdDivCode;
class CUdfRegionCode;
class CUdfProvinceCode;


/*
 ���������� �ؼ���
*/
class CUdfCountyCode: public CUdfBaseZoningCode
{
  public:
	  CUdfCountyCode(){m_pParent = NULL;}
	  ~CUdfCountyCode(){}
      
	  void SetParent(CUdfRegionCode* pParent);
	  CUdfRegionCode* GetParent();
protected:
	CUdfRegionCode* m_pParent;

};

/*
  ���������� �������У���
*/
class CUdfRegionCode: public CUdfBaseZoningCode
{
public:
	//�洢����������������map
	std::map<BYTE,CUdfCountyCode*> m_mapCountyCode;

	//��ѯ�����µ����������������
	CUdfCountyCode* SearchCountyCodeItem(BYTE countyCode);

	//������������������
	bool AddCountyCodeItem(CUdfCountyCode* pItem);
	bool AddCountyCodeItem(CString sName,BYTE bCountyCode);
public:
	void Clear();
	CUdfRegionCode(){m_pParent = NULL;}
	~CUdfRegionCode(){ Clear();	}
protected:
	CUdfProvinceCode* m_pParent;
public:
	void SetParent(CUdfProvinceCode* pParent);
	CUdfProvinceCode* GetParent();
};


/*
 ���������� ʡ(��������ֱϽ��)��
*/
class CUdfProvinceCode: public CUdfBaseZoningCode
{
public:
	//�洢�������У���������������map
	std::map<BYTE,CUdfRegionCode*> m_mapRegionCode;

	//��ѯ�����µĵ������У��������������
	CUdfRegionCode* SearchRegionCodeItem(BYTE regionCode);

	//��ӵ������У��������������
	bool AddRegionCodeItem(CUdfRegionCode* pItem);
	bool AddRegionCodeItem(CString sName,BYTE bZoningCode);

public:
	void Clear();
	~CUdfProvinceCode(){ Clear();}
	CUdfProvinceCode()
	{
		m_sName = "";
		m_bZoningCode = 0;
	}
};


/*
   ���������� Database��
*/
class CUdfAdDivCode
{
public:
	CUdfAdDivCode(void);
public:
	virtual ~CUdfAdDivCode(void);
	void Clear();
public:
	//�洢ʡ��ֱϽ�С�����������������������map
	std::map<BYTE,CUdfProvinceCode*> m_mapProvinceCode;			 

	 //��map�в�ѯʡ��ֱϽ�С����������������������
	CUdfProvinceCode* SearchProvinceCodeItem(BYTE provinceCode);

	//���ʡ(ֱϽ�С����������������������
	bool  AddProvinceCodeItem(CUdfProvinceCode* pObj);
	bool  AddProvinceCodeItem(CString sName,BYTE bZoningCode);
public:
	bool  BuildAdDivCodeRealDB();  //��ȡ���������������ļ�����ʼ������������
};
