#pragma once
#include <map>

/*
  行政区划码 实时数据库和省、地、县对象
  文件名:UdfAdDivCode.h
  行政区划码文件格式
  行政区划码,名称
*/


/*
  行政区划码 基类
*/
class CUdfBaseZoningCode
{
protected:
	BYTE m_bZoningCode;  //区划码
	CString m_sName;     //名称
public:
	CUdfBaseZoningCode()
	{
		m_bZoningCode = 0;
		m_sName = "";
	}
	~CUdfBaseZoningCode(){}
	const BYTE GetZoningCode(){return m_bZoningCode;} //获取行政区划码
	const CString GetName(){return m_sName;}          //获取名称
    
	void SetZoningCode(BYTE bZoningCode){m_bZoningCode = bZoningCode;}  //设置行政区划码
	void SetName(CString sName){m_sName = sName;}						//设置对应的名称
};


class CUdfAdDivCode;
class CUdfRegionCode;
class CUdfProvinceCode;


/*
 行政区划码 县级码
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
  行政区划码 地区（市）码
*/
class CUdfRegionCode: public CUdfBaseZoningCode
{
public:
	//存储县行政区划码对象的map
	std::map<BYTE,CUdfCountyCode*> m_mapCountyCode;

	//查询所属下的县行政区划码对象
	CUdfCountyCode* SearchCountyCodeItem(BYTE countyCode);

	//添加县行政区划码对象
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
 行政区划码 省(自治区、直辖市)码
*/
class CUdfProvinceCode: public CUdfBaseZoningCode
{
public:
	//存储地区（市）行政区划码对象的map
	std::map<BYTE,CUdfRegionCode*> m_mapRegionCode;

	//查询所属下的地区（市）行政区划码对象
	CUdfRegionCode* SearchRegionCodeItem(BYTE regionCode);

	//添加地区（市）行政区划码对象
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
   行政区划码 Database类
*/
class CUdfAdDivCode
{
public:
	CUdfAdDivCode(void);
public:
	virtual ~CUdfAdDivCode(void);
	void Clear();
public:
	//存储省（直辖市、自治区）行政区划码对象的map
	std::map<BYTE,CUdfProvinceCode*> m_mapProvinceCode;			 

	 //在map中查询省（直辖市、自治区）行政区划码对象
	CUdfProvinceCode* SearchProvinceCodeItem(BYTE provinceCode);

	//添加省(直辖市、自治区）行政区划码对象
	bool  AddProvinceCodeItem(CUdfProvinceCode* pObj);
	bool  AddProvinceCodeItem(CString sName,BYTE bZoningCode);
public:
	bool  BuildAdDivCodeRealDB();  //读取行政区划码数据文件，初始化行政区划码
};
