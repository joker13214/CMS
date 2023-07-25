#include "StdAfx.h"
#include "UdfAdDivCode.h"
#include "CMS.h"
#include "UdfGlobalFUNC.h"


CUdfAdDivCode::CUdfAdDivCode(void)
{
}

CUdfAdDivCode::~CUdfAdDivCode(void)
{
	Clear();
}

//读取行政区划码数据文件，初始化行政区划码
/*
  分三层分割行政区划码:省（自治区、直辖市）、地（市）、县
  默认的行政区划码数据文件为：A1.ini，可以通过修改配置文件CMS-config.ini来修改文件名

*/
bool CUdfAdDivCode::BuildAdDivCodeRealDB()
{
   try
   {
	   CString sFilename = "";
	   CCMSApp* pApp = (CCMSApp*)AfxGetApp();
	   if (pApp)
		   sFilename = pApp->m_GlobalOptions.m_strAdDivCodeFilename;

	   if (!CUdfGlobalFUNC::IsFileExist(sFilename))
	   {
		   CString LogMessae ="";
		   LogMessae.Format("行政区划码数据文件'%s'不存在.",sFilename);
		   pApp->m_pLogSystem->InsertLogMessage(SYS_RUNNING_S_LOG,LogMessae,Warning_log_Message);

		   return false;
	   }


	   CStdioFile file(sFilename,CFile::modeRead|CFile::typeText|CFile::shareDenyWrite);
	   file.SeekToBegin();

	   CString strLine;
	   while (file.ReadString(strLine))
	   {
		   BYTE pCode = 0,  //省(直辖市、自治区)码
			    rCode = 0,  //地(市)码
				cCode = 0;  //县 码

		   DWORD totalCode = 0;
		   CString sName = "",sTotalCode="";

		   strLine.TrimLeft();
		   strLine.TrimRight();

		   int rPos = strLine.Find(",");
		   if (rPos != -1)
		   {
			   sName = strLine.Right(strLine.GetLength() - rPos - 1);
			   sName.TrimLeft();
			   sName.TrimRight();
			   if (sName.IsEmpty() || sName == "名称")
				   continue;
			  

			   sTotalCode = strLine.Left(rPos);

			   totalCode = (DWORD)atol(sTotalCode);

			   if (totalCode == 0)
				   continue;



			   DWORD temp1 = totalCode /100;

			   pCode = (BYTE)(temp1 / 100);
			   rCode = (BYTE)(temp1 % 100);
			   cCode = (BYTE)(totalCode % 100);
			   TRACE("%d %d %d %s\r\n",pCode,rCode,cCode,sName);
			   if (rCode == 0 && cCode==0)  //省(直辖市、自治区)码  行政区划码
			   {
				   CUdfProvinceCode* pSProvin = SearchProvinceCodeItem(pCode);
				   if (pSProvin == NULL)
				   {
					   CUdfProvinceCode* pProvin = new CUdfProvinceCode();
					   pProvin->SetName(sName);
					   pProvin->SetZoningCode(pCode);

					   AddProvinceCodeItem(pProvin);
				   }
				   else //pS
				   {
					   pSProvin->SetName(sName);
				   }

			   }
			   else if (rCode !=0 && cCode == 0) //地（市）码 行政区划码
			   {
				   CUdfProvinceCode* pSProvin = SearchProvinceCodeItem(pCode);
				   if (pSProvin == NULL)
				   {
					   CUdfProvinceCode* pProvin = new CUdfProvinceCode();
					   pProvin->SetName("");
					   pProvin->SetZoningCode(pCode);
					   AddProvinceCodeItem(pProvin);


					   CUdfRegionCode* pReg = new CUdfRegionCode();
					   pReg->SetName(sName);
					   pReg->SetZoningCode(rCode);
					   pReg->SetParent(pProvin);

					   pProvin->AddRegionCodeItem(pReg);

				   }
				   else
				   {

					   CUdfRegionCode* pSReg = pSProvin->SearchRegionCodeItem(rCode);
					   if (pSReg == NULL)
					   {
						   CUdfRegionCode* pReg = new CUdfRegionCode();
						   pReg->SetName(sName);
						   pReg->SetZoningCode(rCode);
						   pReg->SetParent(pSProvin);

						   pSProvin->AddRegionCodeItem(pReg);

					   }
					   else
					   {
						   pSReg->SetName(sName);
					   }

				   }
			   }
			   else     //县码 行政区划码
			   {
				   CUdfProvinceCode* pSProvin = SearchProvinceCodeItem(pCode);
				   if (pSProvin == NULL)
				   {
					   CUdfProvinceCode* pProvin = new CUdfProvinceCode();
					   pProvin->SetName("");
					   pProvin->SetZoningCode(pCode);
					   AddProvinceCodeItem(pSProvin);


					   CUdfRegionCode* pReg = new CUdfRegionCode();
					   pReg->SetName("");
					   pReg->SetZoningCode(rCode);
					   pReg->SetParent(pProvin);

					   pProvin->AddRegionCodeItem(pReg);


					   CUdfCountyCode* pCounty = new CUdfCountyCode();
					   pCounty->SetName(sName);
					   pCounty->SetZoningCode(cCode);
					   pCounty->SetParent(pReg);

					   pReg->AddCountyCodeItem(pCounty);


				   }
				   else
				   {
					   CUdfRegionCode* pSReg = pSProvin->SearchRegionCodeItem(rCode);
					   if (pSReg == NULL)
					   {
						   CUdfRegionCode* pReg = new CUdfRegionCode();
						   pReg->SetName("");
						   pReg->SetZoningCode(rCode);
						   pReg->SetParent(pSProvin);

						   pSProvin->AddRegionCodeItem(pReg);



						   CUdfCountyCode* pCounty = new CUdfCountyCode();
						   pCounty->SetName(sName);
						   pCounty->SetZoningCode(cCode);
						   pCounty->SetParent(pReg);

						   pReg->AddCountyCodeItem(pCounty);
					   }
					   else
					   {
						   CUdfCountyCode* pSCounty = pSReg->SearchCountyCodeItem(cCode);
						   if (pSCounty == NULL)
						   {
							   CUdfCountyCode* pCounty = new CUdfCountyCode();
							   pCounty->SetName(sName);
							   pCounty->SetZoningCode(cCode);
							   pCounty->SetParent(pSReg);

							   pSReg->AddCountyCodeItem(pCounty);
						   }
						   else
						   {
							   pSCounty->SetName(sName);
						   }
					   }
				   }
			   }
		   }
	   }
	   file.Close();
   }
   catch (CMemoryException* e)
   {
	   e->Delete();
	   return false;
   }
   catch (CFileException* e)
   {
	   TRACE("BuildAdDivCodeRealDB Error:%s\r\n",e->m_lOsError);
	   e->Delete();
	   return false;
   }
   catch (CException* e)
   {
	   e->Delete();
	   return false;
   }

	return true;
}


CUdfProvinceCode* CUdfAdDivCode::SearchProvinceCodeItem(BYTE provinceCode)
{

	std::map<BYTE,CUdfProvinceCode*>::iterator pIt = m_mapProvinceCode.find(provinceCode);
	if (pIt != m_mapProvinceCode.end())
	{
		if (pIt->second)
			return pIt->second;
	}

	return NULL;
}


bool  CUdfAdDivCode::AddProvinceCodeItem(CUdfProvinceCode* pObj)
{
	m_mapProvinceCode.insert(std::pair<BYTE,CUdfProvinceCode*>(pObj->GetZoningCode(),pObj));
	return true;
}

bool  CUdfAdDivCode::AddProvinceCodeItem(CString sName,BYTE bZoningCode)
{
	CUdfProvinceCode* pObj = new CUdfProvinceCode();
	pObj->SetName(sName);
	pObj->SetZoningCode(bZoningCode);

	return AddProvinceCodeItem(pObj);
}


void CUdfAdDivCode::Clear()
{

	if (m_mapProvinceCode.size())
	{
		std::map<BYTE,CUdfProvinceCode*>::iterator It ;
		for (It = m_mapProvinceCode.begin(); It != m_mapProvinceCode.end(); It ++)
		{
			if (It->second)
				delete (It->second);
		}

		m_mapProvinceCode.clear();

	}
}

CUdfRegionCode* CUdfProvinceCode::SearchRegionCodeItem(BYTE regionCode)
{
	std::map<BYTE,CUdfRegionCode*>::iterator pRgIt  = m_mapRegionCode.find(regionCode);
	if (pRgIt != m_mapRegionCode.end())
	{
		if (pRgIt->second)
			return pRgIt->second;
	}
	return NULL;
}

bool CUdfProvinceCode::AddRegionCodeItem(CUdfRegionCode* pItem)
{
	m_mapRegionCode.insert(std::pair<BYTE,CUdfRegionCode*>(pItem->GetZoningCode(),pItem));
	return true;
}

bool CUdfProvinceCode::AddRegionCodeItem(CString sName,BYTE bZoningCode)
{
	CUdfRegionCode* pObj = new CUdfRegionCode();
	pObj->SetName(sName);
	pObj->SetZoningCode(bZoningCode);
	pObj->SetParent(this);

	return AddRegionCodeItem(pObj);
}


void CUdfProvinceCode::Clear()
{

	if (m_mapRegionCode.size())
	{
		std::map<BYTE,CUdfRegionCode*>::iterator It ;
		for (It = m_mapRegionCode.begin(); It != m_mapRegionCode.end() ; It ++)
		{
			if (It->second)
				delete (It->second);
		}
		m_mapRegionCode.clear();
	}
}



CUdfCountyCode* CUdfRegionCode::SearchCountyCodeItem(BYTE countyCode)
{

	std::map<BYTE,CUdfCountyCode*>::iterator pCIt = m_mapCountyCode.find(countyCode);
	if (pCIt != m_mapCountyCode.end())
	{
		if (pCIt->second)
			return pCIt->second;
	}
	return NULL;
}

bool CUdfRegionCode::AddCountyCodeItem(CUdfCountyCode* pItem)
{
	m_mapCountyCode.insert(std::pair<BYTE,CUdfCountyCode*>(pItem->GetZoningCode(),pItem));
	return true;
}

bool CUdfRegionCode::AddCountyCodeItem(CString sName,BYTE bCountyCode)
{
	CUdfCountyCode* pItem = new CUdfCountyCode();
	pItem->SetName(sName);
	pItem->SetZoningCode(bCountyCode);
	pItem->SetParent( this);

	return AddCountyCodeItem(pItem);
}

void CUdfRegionCode::Clear()
{

	if (m_mapCountyCode.size())
	{
		std::map<BYTE,CUdfCountyCode*>::iterator IT ;
		for (IT = m_mapCountyCode.begin(); IT != m_mapCountyCode.end(); IT++)
		{
			if (IT->second)
				delete (IT->second);
		}
		m_mapCountyCode.clear();
	}
}


void CUdfRegionCode::SetParent(CUdfProvinceCode* pParent)
{
	m_pParent = pParent;
}

CUdfProvinceCode* CUdfRegionCode::GetParent()
{
	return m_pParent;
}



void CUdfCountyCode::SetParent(CUdfRegionCode* pParent)
{
	m_pParent = pParent;
}

CUdfRegionCode* CUdfCountyCode::GetParent()
{
	return m_pParent;
}