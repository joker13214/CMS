#include "StdAfx.h"
#include <afxdb.h>
#include <ObjBase.h>
#include "UdfGlobalFUNC.h"
#include <bitset>
#include <math.h>
#include <map>
#include "SystemTimeOp.h"
#include "CMS.h"
#include "RtuStation.h"

#pragma warning(disable:4996)

TM_TypeMap * TM_TypeMap::_instance = 0;
TM_TypeMap::TM_TypeMap()
{
	InitMap();
}

TM_TypeMap::~TM_TypeMap()
{

}


void TM_TypeMap::InitMap()
{
	using namespace std;
	typedef pair<int, TextMsg*> PAIR;

	TextMsg *pMsg = NULL;

	pMsg = new TextMsg();
	pMsg->m_buf[0] = 'A';
	pMsg->m_buf[1] = 'T';
	pMsg->m_len = 2;
	pMsg->m_Type = AT;
	m_TM_TypeMap.insert(PAIR(AT, pMsg));

	pMsg = new TextMsg();
    pMsg->m_buf[0] = 'A';
	pMsg->m_buf[1] = 'T';
	pMsg->m_buf[2] = 'E';
	pMsg->m_buf[3] = '0';
	pMsg->m_len = 4;
	pMsg->m_Type = ATE0;
	m_TM_TypeMap.insert(PAIR(ATE0, pMsg));

	pMsg = new TextMsg();
	pMsg->m_buf[0] = 'A';
	pMsg->m_buf[1] = 'T';
	pMsg->m_buf[2] = '+';
	pMsg->m_buf[3] = 'C';
	pMsg->m_buf[4] = 'G';
	pMsg->m_buf[5] = 'M';
	pMsg->m_buf[6] = 'R';
	pMsg->m_Type = AT_ADD_CGMR;
	pMsg->m_len = 7;
	m_TM_TypeMap.insert(PAIR(AT_ADD_CGMR, pMsg));

	pMsg = new TextMsg();
	pMsg->m_buf[0] = 'A';
	pMsg->m_buf[1] = 'T';
	pMsg->m_buf[2] = '+';
	pMsg->m_buf[3] = 'C';
	pMsg->m_buf[4] = 'G';
	pMsg->m_buf[5] = 'S';
	pMsg->m_buf[6] = 'N';
	pMsg->m_len = 7;
	pMsg->m_Type = AT_ADD_CGSN;
	m_TM_TypeMap.insert(PAIR(AT_ADD_CGSN, pMsg));

	pMsg = new TextMsg();
	pMsg->m_buf[0] = 'A';
	pMsg->m_buf[1] = 'T';
	pMsg->m_buf[2] = '+';
	pMsg->m_buf[3] = 'C';
	pMsg->m_buf[4] = 'I';
	pMsg->m_buf[5] = 'M';
	pMsg->m_buf[6] = 'I';
	pMsg->m_len = 7;
	pMsg->m_Type = AT_ADD_CIMI;
	m_TM_TypeMap.insert(PAIR(AT_ADD_CIMI, pMsg));

	pMsg = new TextMsg();
	pMsg->m_buf[0] = 'A';
	pMsg->m_buf[1] = 'T';
	pMsg->m_buf[2] = '+';
	pMsg->m_buf[3] = 'C';
	pMsg->m_buf[4] = 'M';
	pMsg->m_buf[5] = 'E';
	pMsg->m_buf[6] = 'E';
	pMsg->m_buf[7] = '=';
	pMsg->m_buf[8] = '2';
	pMsg->m_len = 9;
	pMsg->m_Type = AT_ADD_CMEE;
	m_TM_TypeMap.insert(PAIR(AT_ADD_CMEE, pMsg));

	pMsg = new TextMsg();
	pMsg->m_buf[0] = 'A';
	pMsg->m_buf[1] = 'T';
	pMsg->m_buf[2] = '+';
	pMsg->m_buf[3] = 'C';
	pMsg->m_buf[4] = 'P';
	pMsg->m_buf[5] = 'I';
	pMsg->m_buf[6] = 'N';
	pMsg->m_buf[7] = '?';
	pMsg->m_len = 8;
	pMsg->m_Type = AT_ADD_CPIN_ASK;
	m_TM_TypeMap.insert(PAIR(AT_ADD_CPIN_ASK, pMsg));

	pMsg = new TextMsg();
	pMsg->m_buf[0] = 'A';
	pMsg->m_buf[1] = 'T';
	pMsg->m_buf[2] = '+';
	pMsg->m_buf[3] = 'C';
	pMsg->m_buf[4] = 'R';
	pMsg->m_buf[5] = 'E';
	pMsg->m_buf[6] = 'G';
	pMsg->m_buf[7] = '=';
	pMsg->m_buf[8] = '1';
	pMsg->m_len = 9;
	pMsg->m_Type = AT_ADD_CREG;
	m_TM_TypeMap.insert(PAIR(AT_ADD_CREG, pMsg));

	pMsg = new TextMsg();
	pMsg->m_buf[0] = 'A';
	pMsg->m_buf[1] = 'T';
	pMsg->m_buf[2] = '+';
	pMsg->m_buf[3] = 'C';
	pMsg->m_buf[4] = 'R';
	pMsg->m_buf[5] = 'E';
	pMsg->m_buf[6] = 'G';
	pMsg->m_buf[7] = '?';
	pMsg->m_len = 8;
	pMsg->m_Type = AT_ADD_CREG_ASK;
	m_TM_TypeMap.insert(PAIR(AT_ADD_CREG_ASK, pMsg));

	pMsg = new TextMsg();
	pMsg->m_buf[0] = 'A';
	pMsg->m_buf[1] = 'T';
	pMsg->m_buf[2] = '+';
	pMsg->m_buf[3] = 'C';
	pMsg->m_buf[4] = 'S';
	pMsg->m_buf[5] = 'Q';
	pMsg->m_len = 6;
	pMsg->m_Type = AT_ADD_CSQ;
	m_TM_TypeMap.insert(PAIR(AT_ADD_CSQ, pMsg));

	pMsg = new TextMsg();
	pMsg->m_buf[0] = 'A';
	pMsg->m_buf[1] = 'T';
	pMsg->m_buf[2] = '+';
	pMsg->m_buf[3] = 'C';
	pMsg->m_buf[4] = 'G';
	pMsg->m_buf[5] = 'R';
	pMsg->m_buf[6] = 'E';
	pMsg->m_buf[7] = 'G';
	pMsg->m_buf[8] = '=';
	pMsg->m_buf[9] = '1';
	pMsg->m_len = 10;
	pMsg->m_Type = AT_ADD_CGREG;
	m_TM_TypeMap.insert(PAIR(AT_ADD_CGREG, pMsg));

	pMsg = new TextMsg();
	pMsg->m_buf[0] = 'A';
	pMsg->m_buf[1] = 'T';
	pMsg->m_buf[2] = '+';
	pMsg->m_buf[3] = 'C';
	pMsg->m_buf[4] = 'G';
	pMsg->m_buf[5] = 'R';
	pMsg->m_buf[6] = 'E';
	pMsg->m_buf[7] = 'G';
	pMsg->m_buf[8] = '?';
	pMsg->m_len = 9;
	pMsg->m_Type = AT_ADD_CGREG_ASK;
	m_TM_TypeMap.insert(PAIR(AT_ADD_CGREG_ASK, pMsg));


	pMsg = new TextMsg();
	pMsg->m_buf[0] = 'A';
	pMsg->m_buf[1] = 'T';
	pMsg->m_buf[2] = '+';
	pMsg->m_buf[3] = 'C';
	pMsg->m_buf[4] = 'G';
	pMsg->m_buf[5] = 'A';
	pMsg->m_buf[6] = 'T';
	pMsg->m_buf[7] = 'T';
	pMsg->m_buf[8] = '?';
	pMsg->m_len = 9;
	pMsg->m_Type = AT_ADD_CGATT_ASK;
	m_TM_TypeMap.insert(PAIR(AT_ADD_CGATT_ASK, pMsg));

	pMsg = new TextMsg();
	pMsg->m_buf[0] = 'A';
	pMsg->m_buf[1] = 'T';
	pMsg->m_buf[2] = '+';
	pMsg->m_buf[3] = 'C';
	pMsg->m_buf[4] = 'L';
	pMsg->m_buf[5] = 'I';
	pMsg->m_buf[6] = 'P';
	pMsg->m_buf[7] = '=';
	pMsg->m_buf[8] = '1';
	pMsg->m_len = 9;
	pMsg->m_Type = AT_ADD_CLIP;
	m_TM_TypeMap.insert(PAIR(AT_ADD_CLIP, pMsg));


	pMsg = new TextMsg();
	pMsg->m_buf[0] = 'A';
	pMsg->m_buf[1] = 'T';
	pMsg->m_buf[2] = '+';
	pMsg->m_buf[3] = 'C';
	pMsg->m_buf[4] = 'N';
	pMsg->m_buf[5] = 'M';
	pMsg->m_buf[6] = 'I';
	pMsg->m_buf[7] = '=';
	pMsg->m_buf[8] = '1';
	pMsg->m_buf[9] = ',';
	pMsg->m_buf[10] = '1';
	pMsg->m_buf[11] = ',';
	pMsg->m_buf[12] = '0';
	pMsg->m_buf[13] = ',';
	pMsg->m_buf[14] = '0';
	pMsg->m_buf[15] = ',';
	pMsg->m_buf[16] = '0';
	pMsg->m_len = 17;
	pMsg->m_Type = AT_ADD_CNMI;
	m_TM_TypeMap.insert(PAIR(AT_ADD_CNMI, pMsg));

	pMsg = new TextMsg();
	pMsg->m_buf[0] = 'A';
	pMsg->m_buf[1] = 'T';
	pMsg->m_buf[2] = '+';
	pMsg->m_buf[3] = 'C';
	pMsg->m_buf[4] = 'M';
	pMsg->m_buf[5] = 'C';
	pMsg->m_buf[6] = 'F';
	pMsg->m_buf[7] = '=';
	pMsg->m_buf[8] = '1';
	pMsg->m_len = 9;
	pMsg->m_Type = AT_ADD_CMGF;
	m_TM_TypeMap.insert(PAIR(AT_ADD_CMGF, pMsg));


	pMsg = new TextMsg();
	pMsg->m_buf[0] = 'A';
	pMsg->m_buf[1] = 'T';
	pMsg->m_buf[2] = '+';
	pMsg->m_buf[3] = 'C';
	pMsg->m_buf[4] = 'S';
	pMsg->m_buf[5] = 'C';
	pMsg->m_buf[6] = 'A';
	pMsg->m_buf[7] = '?';
	pMsg->m_len = 8;
	pMsg->m_Type = AT_ADD_CSCA_ASK;
	m_TM_TypeMap.insert(PAIR(AT_ADD_CSCA_ASK, pMsg));



}

TM_TypeMap* TM_TypeMap::GetInstance()
{
	if (_instance == NULL)
	{
		_instance = new TM_TypeMap();
	}

	return _instance;
}

//crc校验编码表
BYTE crc_tablel[256],crc_tableh[256];

namespace CUdfGlobalFUNC
{
	bool IsFileExist(const CString _strFname)
	{

		if (_access(_strFname,0)== -1)
		{
			return false;
		}
		return true;
	}

	CString GetRuningPath()
	{
		CHAR strPATH[MAX_PATH];
		memset(strPATH,0,sizeof(strPATH));

		GetModuleFileName(NULL,strPATH,MAX_PATH);

		CString _strTemp ="";
		_strTemp.Format("%s",strPATH);


		int rsPos = _strTemp.ReverseFind('\\');

		CString strResult="";
		if (rsPos != -1)
		{
			strResult = _strTemp.Left(rsPos);
		}
		else
		{
			strResult=".";
		}

		return strResult;

	}
	bool IpAddressCheck(CString strIP)
	{

		CHAR IP[255];
		memset(IP,0,sizeof(IP));

		strcpy_s(IP,strIP);
		char *temp,*end;
		int numDot=0;
		long length;

		for(temp=strtok(IP, ".");
			temp != NULL;
			temp=strtok(NULL, "."))
		{
			numDot++;
			length=strtol(temp,&end,0);
			if(length<0 || length>255 || strlen(end)!=0)
				return false;
		}

		if(numDot != 4)
			return false;
		return true;
	}



	bool CheckDirExist(const CString strPath)
	{
		WIN32_FIND_DATA  wfd;
		bool rValue = false;
		HANDLE hFind = FindFirstFile(strPath, &wfd);
		if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			rValue = true;   
		}
		FindClose(hFind);
		return rValue;
	}


	bool IsCanConnectDatabase(const CString _DSN,const CString _UID, const CString _PWD)
	{
		CString strDataSource="";
		strDataSource.Format("DSN=%s;UID=%s;PWD=%s",_DSN,_UID,_PWD);

		return IsCanConnectDatabase(strDataSource);
	}

	bool IsCanConnectDatabase(const CString _strDataSource)
	{
		CDatabase db;
		db.SetLoginTimeout(20);
		bool bRet = true;

		try
		{
			if (!db.OpenEx(_strDataSource, CDatabase::noOdbcDialog))
				bRet = false;
			else 
				bRet = true;
			if (bRet)
				db.Close();
		}
		catch(CDBException *pe)
		{
			pe->Delete();
			bRet = false;
		}

		return bRet;
	}


	BYTE ByteFromBCD(BYTE BCD)
	{
		BYTE byResult = 0;

		BYTE gPart=0,sPart = 0;
		BYTE Temp = 0;
		Temp = BCD;
		gPart = (Temp & (0x0f));

		Temp = BCD;
		sPart = ((Temp>>4) &(0x0f));

		byResult = sPart * 10 + gPart;
		TRACE("DEC:%02d BCD:0x%02x\r\n",byResult,BCD);

		return byResult;
	}

	BYTE ByteToBCD(BYTE DEC)
	{
		BYTE byResult = 0;
		BYTE gPart =0, sPart = 0;
		gPart = DEC % 10;
		sPart = DEC / 10;

		byResult = (gPart & 0x0f) | ((sPart<<4) & 0xf0);

		//TRACE("DEC:%02d BCD:0x%02x\r\n",DEC,byResult);


		return byResult;
	}

	void ToBCD(UINT64 DEC,BYTE* bytes,int bytesCount)
	{
		if (!bytes) return;

		if (bytesCount == 1)
		{
			bytes[0] = ByteToBCD((BYTE)(DEC%100));
		}
		else if (bytesCount == 2)
		{
			UINT64 a,b,c ;
			a = DEC % 10000;
			b = a /100;
			c = a % 100;

			bytes[0] = ByteToBCD((BYTE)c);
			bytes[1] = ByteToBCD((BYTE)b);
		}
		else if (bytesCount == 3)
		{
			UINT64 a,b,c,d;
			a = DEC % 1000000;
			b = a / 10000;
			a = a % 10000;

			c = a /100;
			d = a % 100;

			bytes[0] = ByteToBCD((BYTE)d);
			bytes[1] = ByteToBCD((BYTE)c);
			bytes[2] = ByteToBCD((BYTE)b);
			
		}
		else if (bytesCount == 4)
		{
			UINT64 a,b,c,d,e;
			a = DEC % 100000000;
			b = a / 1000000;
			a = a % 1000000;

			c = a /10000;
			a = a % 10000;

			d = a / 100;
			e = a % 100;

			bytes[0] = ByteToBCD((BYTE)e);
			bytes[1] = ByteToBCD((BYTE)d);
			bytes[2] = ByteToBCD((BYTE)c);
			bytes[3] = ByteToBCD((BYTE)b);
			
		}
		else if (bytesCount == 5)
		{
			UINT64 a,b,c,d,e,f;
			a = DEC % 10000000000;
			b = a / 100000000;
			a = a % 100000000;

			c = a /1000000;
			a = a % 1000000;

			d = a / 10000;
			a = a % 10000;

			e = a / 100;
			f = a % 100;

			bytes[0] = ByteToBCD((BYTE)f);
			bytes[1] = ByteToBCD((BYTE)e);
			bytes[2] = ByteToBCD((BYTE)d);
			bytes[3] = ByteToBCD((BYTE)c);
			bytes[4] = ByteToBCD((BYTE)b);
		}
	}


	CString GetCmTypeString(int xType)
	{
		CString strResult = "";
		switch (xType)
		{
		case 1:
			strResult	= "GPRS通道";
			break;
		case 2:
			strResult = "GSM手机短信通道";
			break;
		case 3:
			strResult = "卫星通道";
			break;
		case 4:
			strResult = "超短波通道";
			break;
		case 5:
			strResult = "微波通道";
			break;
		case 6:
			strResult = "短信通道";
			break;
		default:
			strResult = "其他";
			break;
		}

		return strResult;
	}

	CString GetSCFactoryString(int xType)
	{
		CString strResult = "";
		switch (xType)
		{
		case SCFactoryType_GTDZ_4_0:
			strResult = "国腾电子(V4.0协议)";
			break;
		case  SCFactoryType_GTDZ_2_5:
			strResult = "国腾电子(V2.5协议)";
			break;
		default:
			strResult = "其他";
			break;
		}
		return strResult;
	}

	CString GetTmFactoryString(int xType)
	{
		CString strResult = "";
		switch (xType)
		{
		case SCTextMsgType_GHT:
			strResult = "广和通";
			break;
		case SCTextMsgType_YT:
			strResult = "驿唐";
			break;
		default:
			strResult = "其他";
			break;
		}

		return strResult;
	}

	CString GetInterfaceString(int xType)
	{
		CString strResult ="";
		switch (xType)
		{
		case 1:
			strResult ="以太网接口";
			break;
		case 2:
			strResult ="串行接口";
			break;

		}

		return strResult;
	}

	CString GetParityString(int xType)
	{
		CString strResult ="";
		switch (xType)
		{
		case 0:
			strResult ="无校验";
			break;
		case 1:
			strResult ="奇校验";
			break;
		case 2:
			strResult ="偶校验";
			break;
		}

		return strResult;
	}

	CString GetStopBitsString(int xType)
	{
		CString strResult = "";
		switch (xType)
		{
		case ONESTOPBIT:
			strResult ="1位停止位";
			break;
		case  ONE5STOPBITS:
			strResult ="1.5位停止位";
			break;
		case  TWOSTOPBITS:
			strResult = "2位停止位";
			break;
		}

		return strResult;
	}



	//生成crc校验编码表
	void create_crc_table()              
	{
		unsigned char j,m[8],tmp1,tmp0;
		int i;

		for(i=0;i<256;i++)
			crc_tablel[i]=crc_tableh[i]=0;
		for(i=0;i<256;i++)
		{
			tmp0=tmp1=0;
			for(j=0;j<8;j++)
			{
				m[j]=(((unsigned char)i>>j)&0x01);
				tmp0^=m[j];
			}
			tmp1=tmp0^m[0];
			for(j=0;j<6;j++)
				crc_tablel[i]|=(m[j]^m[j+1])<<(j+2);
			crc_tablel[i]|=(tmp1<<1)|tmp0;
			crc_tableh[i]=(tmp0<<7)|(m[7]<<1)|(m[7]^m[6]);
		}
	}

	//查询相应值的CRC校验码
	void crc_code(int count,unsigned char *p,unsigned char *crc16)
	{
		unsigned char temp;

		temp=0;
		crc16[0]=crc16[1]=0;

		while(count-->0)
		{
			temp=crc16[1]^*p++;
			crc16[1]=crc16[0]^crc_tableh[temp];
			crc16[0]=crc_tablel[temp];
		}
	}

	unsigned char do_crc76520(unsigned char *message, unsigned int len)
	{
		unsigned int uiMaxBit;
		unsigned int uiRsb;
		unsigned char crc_reg; 

		unsigned int uiMesIndex;
		unsigned int uiLsb;

		int i;
		unsigned char crcreturn=0;   

		uiMaxBit=len*8-1;
		uiRsb=0;
		crc_reg=message[0]^ENCPY_REV;
		while(uiRsb < uiMaxBit)
		{
			if(crc_reg&0x01)
			{
				crc_reg=crc_reg^ENCPY_REV;
			}
			else
			{
				crc_reg=crc_reg >> 1;
				uiMesIndex=uiRsb / 8 + 1;
				uiLsb=uiRsb % 8;
				if(uiMesIndex<len)
				{
					if(message[uiMesIndex]&(0x01<<uiLsb))
					{
						crc_reg=crc_reg | 0x80; 
					}
				}
				uiRsb++;
			}
		}
		//将crc从发送顺序转变为正常数据表示顺序，即最高位和最低位依次调换
		for(i=0;i<7;i++)
		{
			if(crc_reg&(0x01<<i))
			{
				crcreturn=crcreturn|(0x01<<(7-i)); 
			}
		}
		return crcreturn;
	} 


	unsigned char XORSumCheck( BYTE *message, int len )
	{
		BYTE xorsum = 0;
		if (message != NULL && len)
		{
			for (int i = 0; i < len; i ++)
			{
				xorsum = xorsum ^ message[i];
			}
		}
		return xorsum;
	}



	void GetTPInfo(_TP* pTP,BYTE* tp)
	{
		if (!pTP) return;
		if (!tp) return;

		pTP->m_second = ByteFromBCD(tp[0]);
		pTP->m_minute = ByteFromBCD(tp[1]);
		pTP->m_hour	  = ByteFromBCD(tp[2]);
		pTP->m_day	  = ByteFromBCD(tp[3]);
		pTP->m_adelay = tp[4];
	}


	CString ConvertBytesToString(BYTE* buf,int len)
	{
		CString strBuf = "";
		if (buf != NULL && len > 0)
		{
			for (int idx=0;idx <len;idx ++)
			{
				CString strTemp = "";
				strTemp.Format("%02X ",buf[idx]);
				strBuf += strTemp;
			}
		}
		return strBuf;
	}


	CString GetWeekString(WORD wDayOfWeek,BYTE LangType /* =ZH_CN_LANGUAGE */)
	{
		CString strWeek = "";

		if (LangType == ENGLISH_LANGUAGE)
		{
			if (wDayOfWeek == 1)
				strWeek = "Monday";
			else if (wDayOfWeek == 2)
				strWeek = "Tuesday";
			else if (wDayOfWeek == 3)
				strWeek = "Wednesday";
			else if (wDayOfWeek == 4)
				strWeek = "Thursday";
			else if (wDayOfWeek == 5)
				strWeek = "Friday";
			else if (wDayOfWeek == 6)
				strWeek = "Saturday";
			else
				strWeek = "Sunday";
		}
		else
		{
			if (wDayOfWeek == 1)
				strWeek = "星期一";
			else if (wDayOfWeek == 2)
				strWeek = "星期二";
			else if (wDayOfWeek == 3)
				strWeek = "星期三";
			else if (wDayOfWeek == 4)
				strWeek = "星期四";
			else if (wDayOfWeek == 5)
				strWeek = "星期五";
			else if (wDayOfWeek == 6)
				strWeek = "星期六";
			else
				strWeek = "星期日";
		}

		return strWeek;
	}


	void SetGridCellNormal(CGridCtrl* pGrid, int row,int col)
	{
		if (pGrid && pGrid->GetSafeHwnd())
		{
			pGrid->SetCellType(row,col, RUNTIME_CLASS(CGridCell));
			pGrid->SetItemText(row,col,_T(""));
			pGrid->SetItemState(row,col,pGrid->GetItemState(row,col) & ~GVIS_READONLY);
			pGrid->Invalidate();
		}
	}

	void SetGridCellReadOnly(CGridCtrl* pGrid,int row,int col)
	{
		if (pGrid && pGrid->GetSafeHwnd())
			pGrid->SetItemState(row,col,pGrid->GetItemState(row,col) | GVIS_READONLY);
	}

	const CString GetGridCellText(CGridCtrl* pGrid,int Row,int Col)
	{
		CString strText = "";
		if (pGrid && pGrid->GetSafeHwnd())
		{
			CGridCellBase* pCell= pGrid->GetCell(Row,Col);
			if (pCell)
			{
				strText = pCell->GetText();
			}
		}
		return strText;
	}

	bool IEC_waterdata_to_APPFRAME(BYTE* Buffer,int len,CPacketFrameFormat* waterFrame)
	{
		if (!Buffer || !waterFrame)   return false;
		if (len <5 )				  return false;

		waterFrame->_bFLen	 = Buffer[1];
		std::bitset<8> ContorlC = Buffer[3];
		waterFrame->ControlC._DIR = ContorlC.at(7);
		waterFrame->ControlC._DIV = ContorlC.at(6);
		waterFrame->ControlC._Command = (BYTE)(Buffer[3] & 0x0f );
		waterFrame->ControlC._FCB     = (BYTE)((Buffer[3] & 0x30)>>4);
		if (waterFrame->ControlC._DIV == 1)				//拆分报文
		{
			waterFrame->ControlC._DIVS = Buffer[4];

			//分析地址域
			DWORD _A2A3 = Buffer[7]|Buffer[8]<<8| Buffer[9]<<16;
			waterFrame->_addr_info.A1.ProvinceCode = CUdfGlobalFUNC::ByteFromBCD(Buffer[5]);
			waterFrame->_addr_info.A1.RegionCode   = CUdfGlobalFUNC::ByteFromBCD(Buffer[6]);
			waterFrame->_addr_info.A2 = (_A2A3 & 0x001fffff);
			waterFrame->_addr_info.A3 = (BYTE)((_A2A3 & 0x00E00000)>>21);

			waterFrame->_asdu._AFN = Buffer[10];

			if (waterFrame->_bFLen - 8 > 0 )  //
			{
				waterFrame->_asdu.data.clear();
				waterFrame->_asdu.data =std::vector<unsigned char>(waterFrame->_bFLen - 8);
				memmove(&*(waterFrame->_asdu.data.begin()),&Buffer[11],waterFrame->_bFLen - 8);
			}
		}
		else if (waterFrame->ControlC._DIV == 0)			//未拆分报文
		{

			//分析地址域信息
			DWORD _A2A3  = Buffer[6]|Buffer[7]<<8|Buffer[8]<<16;
			waterFrame->_addr_info.A1.ProvinceCode = CUdfGlobalFUNC::ByteFromBCD(Buffer[4]);
			waterFrame->_addr_info.A1.RegionCode   = CUdfGlobalFUNC::ByteFromBCD(Buffer[5]);
			waterFrame->_addr_info.A2 =  (_A2A3 & 0x001fffff);
			waterFrame->_addr_info.A3 = (BYTE)((_A2A3 & 0x00E00000)>>21);

			waterFrame->_asdu._AFN = Buffer[9];
			if (waterFrame->_bFLen -7 > 0)
			{
				waterFrame->_asdu.data.clear();
				waterFrame->_asdu.data =std::vector<unsigned char>(waterFrame->_bFLen - 7);
				memmove(&*(waterFrame->_asdu.data.begin()),&Buffer[10],waterFrame->_bFLen - 7);
			}
		}
		return true;
	}

	void ConvertTpToSystemtime( _TP* _tp, SYSTEMTIME* pSt )
	{


		if (_tp != NULL &&
			pSt != NULL &&
			_tp->m_day >0 && _tp->m_day <= 31 &&
			_tp->m_hour >=0 && _tp->m_hour < 24 &&
			_tp->m_minute >=0 && _tp->m_minute < 60 &&
			_tp->m_second >=0 && _tp->m_second < 60)
		{

			CTime nowTime = CTime::GetCurrentTime();
			CTime t = nowTime;
			if (nowTime.GetDay() != _tp->m_day)
			{
				CTimeSpan _tspan(1,0,0,0);
				t= nowTime - _tspan;
			}

			pSt->wYear = t.GetYear();
			pSt->wMonth = t.GetMonth();
		
			pSt->wDay = _tp->m_day;
			pSt->wHour = _tp->m_hour;
			pSt->wMinute = _tp->m_minute;
			pSt->wSecond = _tp->m_second;
			pSt->wMilliseconds = 0;
		}
	}

	CString newGUID()
	{
		CString strGUID = "";
		GUID guid;
		if (S_OK == ::CoCreateGuid(&guid))
		{
			strGUID.Format("%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
				guid.Data1,
				guid.Data2,
				guid.Data3,
				guid.Data4[0],guid.Data4[1],guid.Data4[2],guid.Data4[3],guid.Data4[4],guid.Data4[5],guid.Data4[6],guid.Data4[7]);
		}
		return strGUID;
	}

	int CompareDoubleChange( double src,double dst,double cpValue,int nLi,int nPoint )
	{
		int ret = -1;
		try
		{
			double mut = fabs(src-dst);

			CString mutStr ="",cpStr="";
			mutStr.Format("%0*.*f",nLi,nPoint,mut);
			cpStr.Format("%0*.*f",nLi,nPoint,cpValue);

			ret = mutStr.CompareNoCase(cpStr);
		}
		catch (...)
		{
		}
		return ret ;
	}

	int  CompareFloatChange( float src,float dst,float cpValue,int nLi,int nPoint )
	{
		int ret = -1;
		try
		{
			float mut = (float)fabs(src-dst);

			CString mutStr ="",cpStr="";
			mutStr.Format("%0*.*f",nLi,nPoint,mut);
			cpStr.Format("%0*.*f",nLi,nPoint,cpValue);


			ret = mutStr.CompareNoCase(cpStr);
		}
		catch (...)
		{
		}
		return ret ;
	}

	int CompareDouble( double src,double dst,int nLi,int nPoint )
	{ 
		int ret = -1;
		try
		{
			if ( (src >= 0.0 && dst >=0.0)||
				 (src <0.0 && dst <0.0))
			{
				CString srcStr ="",dstStr="";
				srcStr.Format("%0*.*f",nLi,nPoint,src);
				dstStr.Format("%0*.*f",nLi,nPoint,dst);

				ret = srcStr.CompareNoCase(dstStr);
			}
			else
			{
				if (src > dst)
					ret = 1;
				else if (src == dst)
					ret = 0;
				else
					ret = -1;
			}
		}
		catch (...)
		{
		}
		return ret;
	}

	int CompareFloat( float src, float dst,int nLi,int nPoint )
	{
		int ret = -1;
		try
		{
			if ( (src >= 0.0 && dst >=0.0)||
				(src <0.0 && dst <0.0))
			{
				CString srcStr ="",dstStr="";
				srcStr.Format("%0*.*f",nLi,nPoint,src);
				dstStr.Format("%0*.*f",nLi,nPoint,dst);

				ret = srcStr.CompareNoCase(dstStr);
			}
			else
			{
				if (src > dst)
					ret = 1;
				else if (src == dst)
					ret = 0;
				else
					ret = -1;
			}
		}
		catch (...)
		{
		}
		return ret;
	}

	bool CheckErrorData(SYSTEMTIME* pTime )
	{
		bool ret = false;

		if (pTime != NULL)
		{
			DWORD preCheck = GETAPP()->m_GlobalOptions.m_nPreErrCheck;
			DWORD lastCheck = GETAPP()->m_GlobalOptions.m_nLastErrCheck;

#ifdef DEBUG
			TRACE("CheckErrorData:%s \t %d \t%d \r\n",CSystemTimeOp::ConvertSystemTimeToString(pTime,true),preCheck,lastCheck);
#endif

			SYSTEMTIME curSt;
			GetLocalTime(&curSt);
			CTime curDt = CTime::GetCurrentTime();

			int comp = CSystemTimeOp::CompareSystemTime(pTime,&curSt);
			if (comp = -1)
			{
				
				CTime preDT(*pTime);

				CTimeSpan tspan = curDt - preDT;
				if (tspan.GetTotalMinutes() >= preCheck)
					ret = true;

			}
			else if (comp == 1)
			{
				CTime lastDt(*pTime);
				CTimeSpan tspan = lastDt - curDt;
				if (tspan.GetTotalMinutes() >= lastCheck)
					ret = true;
			}
		}

		return ret;
	}


}  //end namespace


BYTE _ControlC::GetControlCByte()
{
	BYTE C = 0;
	C = ((this->_DIR?1:0) << 7)  |
		((this->_DIV?1:0) << 6)  |
		((this->_FCB)<< 4) |
		(this->_Command);

	return C;
}

_ASDU::_ASDU()
{
	data.clear();
	//_AUX.clear();
}

_ASDU::~_ASDU()
{
	data.clear();
	//_AUX.clear();

}



CPacketFrameFormat::CPacketFrameFormat()
{
	//_ADDR.clear();
	//_bStart1	= 0x68;
	//_bStart2	= 0x68;
	//_bEnd		= 0x16;
	_bFLen		= 0x0;

	m_CommInterface = Serial_CommInterface;
	m_ProtocolType = UDP_Protocol;
	m_pStation = NULL;


	//
	m_bRepeatType = false;
}

CPacketFrameFormat::~CPacketFrameFormat()
{
	//_ADDR.clear();

}

void CPacketFrameFormat::UpdateRecvTime( PSYSTEMTIME pTime/*= NULL*/ )
{
	if (!pTime)
		GetLocalTime(&m_stRecvTime);
	else
	{
		m_stRecvTime.wDay			= pTime->wDay;
		m_stRecvTime.wDayOfWeek		= pTime->wDayOfWeek;
		m_stRecvTime.wHour			= pTime->wHour;
		m_stRecvTime.wMilliseconds	= pTime->wMilliseconds;
		m_stRecvTime.wMinute		= pTime->wMinute;
		m_stRecvTime.wMonth			= pTime->wMonth;
		m_stRecvTime.wSecond		= pTime->wSecond;
		m_stRecvTime.wYear			= pTime->wYear;
	}
}

bool CPacketFrameFormat::GetRepeatType() const
{
	return m_bRepeatType;
}

void CPacketFrameFormat::SetRepeatType( bool val )
{
	m_bRepeatType = val;
}

PSYSTEMTIME CPacketFrameFormat::GetRecvTime()
{
	return &m_stRecvTime;
}


void CSmsFrameFormat::UpdateRecvTime( PSYSTEMTIME pTime/*= NULL*/ )
{
	if (!pTime)
		GetLocalTime(&m_stRecvTime);
	else
	{
		m_stRecvTime.wDay			= pTime->wDay;
		m_stRecvTime.wDayOfWeek		= pTime->wDayOfWeek;
		m_stRecvTime.wHour			= pTime->wHour;
		m_stRecvTime.wMilliseconds	= pTime->wMilliseconds;
		m_stRecvTime.wMinute		= pTime->wMinute;
		m_stRecvTime.wMonth			= pTime->wMonth;
		m_stRecvTime.wSecond		= pTime->wSecond;
		m_stRecvTime.wYear			= pTime->wYear;
	}
}

PSYSTEMTIME CSmsFrameFormat::GetRecvTime()
{
	return &m_stRecvTime;
}

void _CMS_GLOBAL_PW_::BuildPWBuf()
{
	BYTE  temp = 0;
	temp = (BYTE)(PW_CALC * 10 + (PW_CODE /100));
	PW_BUF[1] = CUdfGlobalFUNC::ByteToBCD(temp);
	temp = (BYTE)(PW_CODE % 100);
	PW_BUF[0] = CUdfGlobalFUNC::ByteToBCD(temp);
}

_COM_STRUCT_INFO_& _COM_STRUCT_INFO_::operator=( _COM_STRUCT_INFO_ & temp )
{
	this->mComName =temp.mComName;
	this->mBaund = temp.mBaund;
	this->mDataBits = temp.mDataBits;
	this->mStopBits = temp.mStopBits;
	this->mParity = temp.mParity;

	return (*this);
}

_NET_STRUCT_INFO_& _NET_STRUCT_INFO_::operator=( _NET_STRUCT_INFO_& temp )
{
	this->mIpAddress = temp.mIpAddress;
	this->mPort = temp.mPort;

	return (*this);
}

_SC_COMM_INFO_& _SC_COMM_INFO_::operator=( _SC_COMM_INFO_& temp )
{
	this->nFactoryType = temp.nFactoryType;
	this->nScAddress = temp.nScAddress;
	return (*this);
}

_SC_TEXTMSG_INFO_& _SC_TEXTMSG_INFO_::operator=( _SC_TEXTMSG_INFO_& temp )
{
	this->nFactoryType = temp.nFactoryType;
	this->nTmAddress = temp.nTmAddress;
	return (*this);
}

_TP& _TP::operator=( _TP &_tp )
{
	this->m_adelay = _tp.m_adelay;
	this->m_day = _tp.m_day;
	this->m_hour = _tp.m_hour;
	this->m_minute = _tp.m_minute;
	this->m_second = _tp.m_second;
	return (*this);
}
