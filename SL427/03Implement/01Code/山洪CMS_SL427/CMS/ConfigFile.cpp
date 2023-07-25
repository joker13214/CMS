// ConfigFile.cpp: implementation of the CConfigFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ConfigFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace JDGM_CONFIG_FILE
{
	CConfigFile::CConfigFile()
	{
		MapForItemName = new CTypedPtrMap<CMapStringToPtr,CString,StructConfigItem*>();
		m_nItemCount = 0;
		m_strFileName = "";
	}

	CConfigFile::CConfigFile(CString _PathFilename)
	{
		m_strFileName = _PathFilename ;
		m_nItemCount = 0;
		MapForItemName = new CTypedPtrMap<CMapStringToPtr,CString,StructConfigItem*>();
	}

	CConfigFile::~CConfigFile()
	{
		
		if (MapForItemName)
		{
			MapForItemName->RemoveAll();
			delete MapForItemName;
			MapForItemName = NULL;
		}
		
		Clear();
	}

	void CConfigFile::Clear()
	{

		if (MapForItemName)
		{
			MapForItemName->RemoveAll();
		}
		while (!m_lstLines.IsEmpty())
		{
			_LineString_Item *pObj = (_LineString_Item*)m_lstLines.RemoveHead();
			if (pObj)
			{
				if (pObj->m_pConfigOption)
				{
					delete pObj->m_pConfigOption;
					pObj->m_pConfigOption = NULL;
				}

				delete pObj;
				pObj = NULL;

			}
		}
	}

	CString CConfigFile::GetItemValue(CString _ItemName,BOOL &blExist)
	{
		StructConfigItem *pItem = NULL;
		CString strResult = "";
		if (MapForItemName)
		{
			CString  strKey = _ItemName;
			strKey.MakeUpper();
			MapForItemName->Lookup(strKey,pItem);
			if (pItem)
			{
				strResult = pItem->GetValue();
				blExist = TRUE;
			}
			else
			{
				blExist = FALSE;
			}
		}

		return strResult;

	}

	int CConfigFile::GetItemCount()
	{
		m_nItemCount = 0;
		if (MapForItemName)
		{
			m_nItemCount = (int)MapForItemName->GetCount();
		}

		return m_nItemCount;
	}

	int CConfigFile::SetConfigFileName(CString _PathFilename)
	{
		m_strFileName = _PathFilename;

		return 1;
	}

	BOOL CConfigFile::LoadFile()
	{
	   BOOL blResult = FALSE;

	   Clear();

	   try
	   {

		   if (m_strFileName.IsEmpty())
			   return FALSE;

		   CStdioFile file;
		   if (!file.Open(m_strFileName,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite|CFile::typeText|CFile::shareDenyNone))
		   {
			   AfxMessageBox("File not be Open!",MB_OK);
			   file.Close();
			   return FALSE;
		   }


		   file.SeekToBegin();

		   CString Tmp="";
		   while (file.ReadString(Tmp))
		   {
			   CString _Name="",_Value = "";
			   Tmp.TrimLeft();
			   Tmp.TrimRight();
			   AnalyseConfigStr(Tmp,_Name,_Value);
			   
			   _LineString_Item *_LineOpj =new _LineString_Item();
			   _LineOpj->_LineString = Tmp;
			  
			   if (!_Name.IsEmpty())
			   {
				   TRACE("ID:%20s\tValue:%20s\r\n",_Name,_Value);

				   StructConfigItem *pObj= new StructConfigItem(_Name,_Value);
				   if (pObj)
				   {
					   CString strKey = pObj->GetName();
					   strKey.MakeUpper();

					   MapForItemName->SetAt(strKey,pObj);

					   _LineOpj->_blSaveOption = TRUE;
					   _LineOpj->m_pConfigOption = pObj;
				   }

			   }
			   else
			   {
				   _LineOpj->_blSaveOption = FALSE;
				   _LineOpj->m_pConfigOption = NULL;
			   }

			   m_lstLines.AddTail((void*)_LineOpj);
		   }

		   file.Close();
		   
		   m_nItemCount = (int)MapForItemName->GetCount();

		   blResult = TRUE;
	   }
	   catch (CFileException* fe)
	   {
   		   fe->ReportError(MB_OK|MB_ICONERROR);
		   fe->Delete();
		   return FALSE;	   
	   }
	   return blResult;
	}

	void CConfigFile::AnalyseConfigStr(CString str, CString &_Name, CString &_Value)
	{
		str.TrimLeft();
		str.TrimRight();
		
		// ×¢ÊÍ »òÕß ¿ÕÐÐ
		if (str.Left(1) == "#" ||
			str.Left(1) == ";" ||
			str.Left(1) == "'" ||
			str.IsEmpty()) 
		{
			_Name = "";
			_Value ="";
			return;
		}

		int eqPos = str.Find("=",0);
		if (eqPos != -1)
		{
			_Name = str.Left(eqPos);
			_Name.TrimLeft();
			_Name.TrimRight();

			_Value = str.Right(str.GetLength()- (eqPos+1));
			_Value.TrimLeft();
			_Value.TrimRight();

		}
		else
		{
			_Name = "";
			_Value = "";
		}
	}

	BOOL CConfigFile::LoadFile(CString _strFileName)
	{
		SetConfigFileName(_strFileName);
		return LoadFile();

	}

	BOOL CConfigFile::SaveFile(CString _strFilename)
	{
		SetConfigFileName(_strFilename);
		return SaveFile();
	}

	BOOL CConfigFile::SaveFile()
	{
		BOOL _blResult = TRUE;

		CStdioFile _fp;
		try
		{
			_fp.Open(m_strFileName,CFile::modeCreate/*|CFile::modeNoTruncate*/|CFile::modeReadWrite|CFile::typeText|CFile::shareDenyNone);
			POSITION rPos = m_lstLines.GetHeadPosition();

			
			_fp.SeekToBegin();
			while (rPos != NULL)
			{
				_LineString_Item *pObj = (_LineString_Item*)m_lstLines.GetNext(rPos);
				if (pObj)
				{
					CString _lineStr="";
					if (pObj->_blSaveOption == FALSE || (pObj->m_pConfigOption == NULL))
					{
						_lineStr.Format("%s\n",pObj->_LineString);
					}
					else if (pObj->_blSaveOption == TRUE && pObj->m_pConfigOption != NULL )
					{
						_lineStr.Format("%s = %s\n",pObj->m_pConfigOption->GetName(),pObj->m_pConfigOption->GetValue());
					}
					_fp.WriteString(_lineStr);
				}
			}
			_fp.Close();
		}
		catch (CFileException* e)
		{
			TRACE("File Error:File save error!\r\n");
			e->ReportError(MB_OK|MB_ICONERROR);
			e->Delete();
			_blResult = FALSE;
			
		}

		return _blResult;
	}

	void CConfigFile::SetItemValue(CString _ItemName,CString _newValue)
	{
		StructConfigItem *pItem = NULL;
		//CString strResult = "";
		if (MapForItemName)
		{
			CString  strKey = _ItemName;
			strKey.MakeUpper();
			MapForItemName->Lookup(strKey,pItem);
			if (pItem)
			{
				//strResult = pItem->GetValue();
				pItem->SetValue(_newValue);
			}
		}
	}

	void CConfigFile::SetItemValue(CString _ItemName, int _newValue)
	{
		CString strValue ="";
		strValue.Format("%d",_newValue);

		SetItemValue(_ItemName,strValue);

	}

	BOOL CConfigFile::AddStringLine(_LineString_Item *_pObj)
	{
		BOOL _blResult = TRUE;

		if (!_pObj)
			return _blResult;

		m_lstLines.AddTail((void*)_pObj);

		if (_pObj->_blSaveOption && _pObj->m_pConfigOption != NULL)
		{
		    CString strKey = _pObj->m_pConfigOption->GetName();
			strKey.MakeUpper();
			MapForItemName->SetAt(strKey,_pObj->m_pConfigOption);
		}

		return _blResult ;	
	}

	BOOL CConfigFile::AddOneOption(CString _opName,CString _opValue,CString _opContent/* = */,BOOL blAddBlankLine /* = FALSE */)
	{

		BOOL _blResult = TRUE;

		_LineString_Item *pNew = NULL;
		StructConfigItem *pOption = NULL;

		_opName.TrimLeft();
		_opName.TrimRight();

		if (_opName.IsEmpty())
			return FALSE;

        //Add Content
		_opContent.Trim();
		if (!_opContent.IsEmpty())
		{
			CString _StrContent="";
			_StrContent.Format("#%s",_opContent);
			pNew = new _LineString_Item();
			pNew->_LineString=_StrContent;
			pNew->_blSaveOption = FALSE;
			pNew->m_pConfigOption = NULL;
			AddStringLine(pNew);
		}

		//Add Option's information
		pNew = new _LineString_Item();
		pNew->_blSaveOption = TRUE;
		pOption = new StructConfigItem(_opName,_opValue);
		pNew->m_pConfigOption = pOption;
		AddStringLine(pNew);

		//Add blank line
		if (blAddBlankLine == TRUE)
		{
			pNew = new _LineString_Item();
			pNew->_LineString="";
			pNew->_blSaveOption = FALSE;
			pNew->m_pConfigOption = NULL;
			AddStringLine(pNew);
		}

		return _blResult; 
	}

}  //end namespace JDGM_CONFIG_FILE