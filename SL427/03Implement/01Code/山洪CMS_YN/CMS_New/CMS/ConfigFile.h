// ConfigFile.h: interface for the CConfigFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONFIGFILE_H__C11531DE_5B46_45EC_8535_0419F21080C9__INCLUDED_)
#define AFX_CONFIGFILE_H__C11531DE_5B46_45EC_8535_0419F21080C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
namespace JDGM_CONFIG_FILE
{

	class StructConfigItem
	{
	protected:
		CString ItemName;   //Option Name
		CString ItemValue;  //Option Value
	public:
		StructConfigItem()
		{
			ItemName.Empty();
			ItemValue.Empty();
		}
		StructConfigItem(CString _Name, CString _Value)
		{
			ItemName = _Name;
			ItemValue = _Value;
		}
		const CString GetName() {return ItemName;}
		const CString GetValue() {return ItemValue;}
		void    SetValue(CString _newValue)
		{
			ItemValue = _newValue;
		}
		/*
		CString SetName(CString _newName)
		{
			ItemName = _newName;
		}
		*/
	};


	class _LineString_Item
	{
		public:
		CString _LineString;				//文本文字(1 line)
		BOOL    _blSaveOption;				//表示是否存储配置数据 
		StructConfigItem *m_pConfigOption;  //配置选项信息,Option name/Option value
		_LineString_Item()
		{
			_LineString = "";
			_blSaveOption = FALSE;
			m_pConfigOption = NULL;
		}
	};


	//配置文件操作类
	class CConfigFile  
	{
	public:
		//读取配置文件
		BOOL LoadFile(CString _strFileName);

		//读取配置文件
		BOOL LoadFile();

		//保存配置文件
		BOOL SaveFile(CString _strFilename);
		BOOL SaveFile();
		CConfigFile();
		CConfigFile(CString _PathFilename);
		virtual ~CConfigFile();

		//获取选项的值 (blExist:获取选项是否存在)
		CString GetItemValue(CString _ItemName,BOOL &blExist);

		//设置选项的值
		void    SetItemValue(CString _ItemName, CString _newValue);
		void    SetItemValue(CString _ItemName, int _newValue);

		//获取选项计数
		int     GetItemCount();

		//设置Configuration的文件名
		int     SetConfigFileName(CString _PathFilename);

		//添加一line
		BOOL    AddStringLine(_LineString_Item *pObj);

		//功能:添加一项配置数据信息
		//参数解释:
        //     _opName:选项名称
        //     _opValue:选择值 
        //     _opContents:注释
        //     blAddBlankLine:是否添加一空行
		BOOL    AddOneOption(CString _opName,CString _opValue,CString _opContent="",BOOL blAddBlankLine = FALSE);

		//清除链表相关信息
		void	Clear();

		//获取LinesList
		CPtrList* GetLinesList()
		{
			return &m_lstLines;
		}
		
	protected:
		CTypedPtrMap<CMapStringToPtr,CString,StructConfigItem*> *MapForItemName;

		//Items 计数
		int     m_nItemCount;

		//Configuration 文件名
		CString m_strFileName;
		
		//File's Lines List;
		CPtrList m_lstLines;

	private:
		void AnalyseConfigStr(CString str,CString &_Name,CString &_Value);

	};
}


#endif // !defined(AFX_CONFIGFILE_H__C11531DE_5B46_45EC_8535_0419F21080C9__INCLUDED_)
