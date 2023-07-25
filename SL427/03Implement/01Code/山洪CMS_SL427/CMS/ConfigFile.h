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
		CString _LineString;				//�ı�����(1 line)
		BOOL    _blSaveOption;				//��ʾ�Ƿ�洢�������� 
		StructConfigItem *m_pConfigOption;  //����ѡ����Ϣ,Option name/Option value
		_LineString_Item()
		{
			_LineString = "";
			_blSaveOption = FALSE;
			m_pConfigOption = NULL;
		}
	};


	//�����ļ�������
	class CConfigFile  
	{
	public:
		//��ȡ�����ļ�
		BOOL LoadFile(CString _strFileName);

		//��ȡ�����ļ�
		BOOL LoadFile();

		//���������ļ�
		BOOL SaveFile(CString _strFilename);
		BOOL SaveFile();
		CConfigFile();
		CConfigFile(CString _PathFilename);
		virtual ~CConfigFile();

		//��ȡѡ���ֵ (blExist:��ȡѡ���Ƿ����)
		CString GetItemValue(CString _ItemName,BOOL &blExist);

		//����ѡ���ֵ
		void    SetItemValue(CString _ItemName, CString _newValue);
		void    SetItemValue(CString _ItemName, int _newValue);

		//��ȡѡ�����
		int     GetItemCount();

		//����Configuration���ļ���
		int     SetConfigFileName(CString _PathFilename);

		//���һline
		BOOL    AddStringLine(_LineString_Item *pObj);

		//����:���һ������������Ϣ
		//��������:
        //     _opName:ѡ������
        //     _opValue:ѡ��ֵ 
        //     _opContents:ע��
        //     blAddBlankLine:�Ƿ����һ����
		BOOL    AddOneOption(CString _opName,CString _opValue,CString _opContent="",BOOL blAddBlankLine = FALSE);

		//������������Ϣ
		void	Clear();

		//��ȡLinesList
		CPtrList* GetLinesList()
		{
			return &m_lstLines;
		}
		
	protected:
		CTypedPtrMap<CMapStringToPtr,CString,StructConfigItem*> *MapForItemName;

		//Items ����
		int     m_nItemCount;

		//Configuration �ļ���
		CString m_strFileName;
		
		//File's Lines List;
		CPtrList m_lstLines;

	private:
		void AnalyseConfigStr(CString str,CString &_Name,CString &_Value);

	};
}


#endif // !defined(AFX_CONFIGFILE_H__C11531DE_5B46_45EC_8535_0419F21080C9__INCLUDED_)
