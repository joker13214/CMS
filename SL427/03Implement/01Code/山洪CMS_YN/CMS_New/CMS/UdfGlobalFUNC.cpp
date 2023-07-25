#include "StdAfx.h"
#include <afxdb.h>
#include "UdfGlobalFUNC.h"
#include <bitset>
#include "RtuStation.h"

#pragma warning(disable:4996)

SCSW_TypeMap * SCSW_TypeMap::_instance = 0;
SCSW_TypeMap::SCSW_TypeMap()
{
	InitMap();
}

SCSW_TypeMap::~SCSW_TypeMap()
{

}


void SCSW_TypeMap::InitMap()
{
	using namespace std;
	typedef pair<CString, int> PAIR;

	m_SCSWTypeMap.insert(PAIR(CString("BD"),    SCSW_BD));
	m_SCSWTypeMap.insert(PAIR(CString("BV"),    SCSW_BV));
	m_SCSWTypeMap.insert(PAIR(CString("C"),     SCSW_C));
	m_SCSWTypeMap.insert(PAIR(CString("CN"),    SCSW_CN));
	m_SCSWTypeMap.insert(PAIR(CString("CP"),    SCSW_CP));
	m_SCSWTypeMap.insert(PAIR(CString("DZ"),    SCSW_DZ));
	m_SCSWTypeMap.insert(PAIR(CString("DV"),    SCSW_DV));
	m_SCSWTypeMap.insert(PAIR(CString("DI"),    SCSW_DI));
	m_SCSWTypeMap.insert(PAIR(CString("DC"),    SCSW_DC));
	m_SCSWTypeMap.insert(PAIR(CString("ED"),    SCSW_ED));
	m_SCSWTypeMap.insert(PAIR(CString("ET"),    SCSW_ET));
	m_SCSWTypeMap.insert(PAIR(CString("ER"),    SCSW_ER));
	m_SCSWTypeMap.insert(PAIR(CString("GSM"),   SCSW_GSM));
	m_SCSWTypeMap.insert(PAIR(CString("H"),     SCSW_H));
	m_SCSWTypeMap.insert(PAIR(CString("IT"),    SCSW_IT));
	m_SCSWTypeMap.insert(PAIR(CString("ML"),    SCSW_ML));
	m_SCSWTypeMap.insert(PAIR(CString("MF"),    SCSW_MF));
	m_SCSWTypeMap.insert(PAIR(CString("M10"),   SCSW_M10));
	m_SCSWTypeMap.insert(PAIR(CString("NN"),    SCSW_NN));
	m_SCSWTypeMap.insert(PAIR(CString("OK"),    SCSW_OK));
	m_SCSWTypeMap.insert(PAIR(CString("PN"),    SCSW_PN));
	m_SCSWTypeMap.insert(PAIR(CString("P"),     SCSW_P));
	m_SCSWTypeMap.insert(PAIR(CString("PI"),    SCSW_PI));
	m_SCSWTypeMap.insert(PAIR(CString("PSTN"),  SCSW_PSTN));
	m_SCSWTypeMap.insert(PAIR(CString("PT"),    SCSW_PT));
	m_SCSWTypeMap.insert(PAIR(CString("PW"),    SCSW_PW));
	m_SCSWTypeMap.insert(PAIR(CString("RC"),    SCSW_RC));
	m_SCSWTypeMap.insert(PAIR(CString("RD"),    SCSW_RD));
	m_SCSWTypeMap.insert(PAIR(CString("RR"),    SCSW_RR));
	m_SCSWTypeMap.insert(PAIR(CString("RF"),    SCSW_RF));
	m_SCSWTypeMap.insert(PAIR(CString("SS"),    SCSW_SS));
	m_SCSWTypeMap.insert(PAIR(CString("SV"),    SCSW_SV));
	m_SCSWTypeMap.insert(PAIR(CString("SI"),    SCSW_SI));
	m_SCSWTypeMap.insert(PAIR(CString("ST"),    SCSW_ST));
	m_SCSWTypeMap.insert(PAIR(CString("TT"),    SCSW_TT));
	m_SCSWTypeMap.insert(PAIR(CString("TM"),    SCSW_TM));
	m_SCSWTypeMap.insert(PAIR(CString("TN"),    SCSW_TN));
	m_SCSWTypeMap.insert(PAIR(CString("TB"),    SCSW_TB));
	m_SCSWTypeMap.insert(PAIR(CString("TE"),    SCSW_TE));
	m_SCSWTypeMap.insert(PAIR(CString("TC"),    SCSW_TC));
	m_SCSWTypeMap.insert(PAIR(CString("Z"),     SCSW_Z));
	m_SCSWTypeMap.insert(PAIR(CString("ZR"),    SCSW_ZR));
	m_SCSWTypeMap.insert(PAIR(CString("ZJ"),    SCSW_ZJ));
	m_SCSWTypeMap.insert(PAIR(CString("ZT"),    SCSW_ZT));
	m_SCSWTypeMap.insert(PAIR(CString("ZF"),    SCSW_ZF));
	m_SCSWTypeMap.insert(PAIR(CString("ZA"),    SCSW_ZA));
	m_SCSWTypeMap.insert(PAIR(CString("ZB"),    SCSW_ZB));
	m_SCSWTypeMap.insert(PAIR(CString("ZC"),    SCSW_ZC));
	m_SCSWTypeMap.insert(PAIR(CString("ZD"),    SCSW_ZD));
	m_SCSWTypeMap.insert(PAIR(CString("ZE"),    SCSW_ZE));
	m_SCSWTypeMap.insert(PAIR(CString("DZA"),    SCSW_DZA));
	m_SCSWTypeMap.insert(PAIR(CString("DZB"),    SCSW_DZB));
	m_SCSWTypeMap.insert(PAIR(CString("DZC"),    SCSW_DZC));
	m_SCSWTypeMap.insert(PAIR(CString("DZD"),    SCSW_DZD));
	m_SCSWTypeMap.insert(PAIR(CString("DZE"),    SCSW_DZE));
	m_SCSWTypeMap.insert(PAIR(CString("HOST"),   SCSW_HOST));

	m_SCSWTypeMap.insert(PAIR(CString("A0"),   SCSW_A0));
	m_SCSWTypeMap.insert(PAIR(CString("A1"),   SCSW_A1));
	m_SCSWTypeMap.insert(PAIR(CString("A2"),   SCSW_A2));
	m_SCSWTypeMap.insert(PAIR(CString("A3"),   SCSW_A3));
	m_SCSWTypeMap.insert(PAIR(CString("B0"),   SCSW_B0));
	m_SCSWTypeMap.insert(PAIR(CString("B1"),   SCSW_B1));
	m_SCSWTypeMap.insert(PAIR(CString("B2"),   SCSW_B2));
	m_SCSWTypeMap.insert(PAIR(CString("B3"),   SCSW_B3));

}

SCSW_TypeMap* SCSW_TypeMap::GetInstance()
{
	if (_instance == NULL)
	{
		_instance = new SCSW_TypeMap();
	}

	return _instance;
}


//水雨情



SYQ_YS_STRUCT m_yaosu[] ={
	//8
	{"DRD", "日时间步长",  1, "日",  5, 0},
	{"DRH",  "小时时间步长", 1, "小时", 5, 0},
	{"DRN",  "分钟时间步长", 1, "小时", 5, 0},
	{"P1",   "1h时段降水量", 2, "毫米", 5, 1},
	{"P2",   "2h时段降水量", 2, "毫米", 5, 1},
	{"P3",   "3h时段降水量", 2, "毫米", 5, 1},
	{"P6",   "6h时段降水量", 2, "毫米", 5, 1},
	{"P12",   "12h时段降水量", 2, "毫米", 5, 1},
//7
	{"PD",  "日降水量", 2, "毫米", 5, 1},
	{"PM",  "月降水量",2,"毫米",5,1},
	{"PN01", "1min时段降水量",2,"毫米",5,1},
	{"PN05", "5min时段降水量",2,"毫米",5,1},
	{"PN10", "10min时段降水量",2,"毫米",5,1},
	{"PN30", "30min时段降水量",2,"毫米",5,1},
	{"PR",   "暴雨量",2,"毫米",5,1},//14
//33
	{"PX",   "旬降水量  ",2,"毫米",5,1},
	{"Q",     "瞬时流量、抽水流量",2,"立方米/秒",7,3},
	{"QA",    "总出库流量、过闸总流量",2,"立方米/秒",7,3},
	{"QAD",   "日平均出库流量",2,"立方米/秒",7,3},
	{"QAM",   "月平均出库流量",2,"立方米/秒",7,3},
	{"QAMM",  "月最大出库流量",2,"立方米/秒",7,3},
	{"QAMN",  "月最小出库流量",2,"立方米/秒",7,3},
	{"QAX",   "旬平均出库流量",2,"立方米/秒",7,3},
	{"QAXM",  "旬最大总出库量",2,"立方米/秒",7,3},
	{"QAXN",  "旬最小总出库量",2,"立方米/秒",7,3},
	{"QD",    "河道、闸坝日平均流量,泵站日平均抽水流量",2,"立方米/秒",7,3},
	{"QI",    "入库流量",2,"立方米/秒",7,3},
	{"QID",  "日平均入库流量",2,"立方米/秒",7,3},
	{"QIM",  "月平均入库流量",2,"立方米/秒",7,3},
	{"QIMM",  "月最大入库流量",2,"立方米/秒",7,3},
	{"QIMN",  "月最小入库流量",2,"立方米/秒",7,3},
	{"QIX",   "旬平均入库流量",2,"立方米/秒",7,3},
	{"QIXM",  "旬最大入库流量",2,"立方米/秒",7,3},
	{"QIXN",  "旬最小入库流量",2,"立方米/秒",7,3},
	{"PJ",    "当日雨量",2,"毫米",4,1},
	{"PT",    "降水量累计值",2,"毫米",5,2},
	{"DRP",   "1小时内每5分钟时段雨量",2,"毫米",12,0},
	{"DRZ",   "1小时内5分钟间隔相对水位",2,"米",24,0},
	{"TT",   "电源电压", 2, "伏特"  ,4, 2},
	{"Z",   "瞬时河道水位、潮位",2,"米",5,3},
	{"ZB",   "库(闸、站)下水位",2,"米",5,3},
	{"ZBD",   "库(闸、站)下日平均水位",2,"米",5,3},
	{"ZBM",   "库(闸、站)下月平均水位",2,"米",5,3},
	{"ZBMM",   "库(闸、站)下月最高水位",2,"米",5,3},
	{"ZBMN",   "库(闸、站)下月最低水位",2,"米",5,3},
	{"ZBX",   "库(闸、站)下旬平均水位",2,"米",5,3},
	{"ZBXM",   "库(闸、站)下旬最高水位",2,"米",5,3},
	{"ZBXN",   "库(闸、站)下旬最低水位",2,"米",5,3},
//13
	{"ZD",   "日平均水位",2,"米",5,3},
	{"ZH",   "高潮位",2,"米",5,3},
	{"ZHD",   "日平均高潮位",2,"米",5,3},
	{"ZHH",   "高高潮位",2,"米",5,3},
	{"ZHL",   "低高潮位",2,"米",5,3},
	{"ZHM",   "月平均高潮位",2,"米",5,3},
	{"ZHX",   "旬平均高潮位",2,"米",5,3},
	{"ZL",   "低潮位",2,"米",5,3},
	{"ZLD",   "日平均低潮位",2,"米",5,3},
	{"ZLH",   "高低潮位",2,"米",5,3},
	{"ZLL",   "低低潮位",2,"米",5,3},
	{"ZLM",   "月平均低潮位",2,"米",5,3},
	{"ZLX",   "旬平均低潮位",2,"米",5,3},
//15
	{"ZM",   "月平均水位",2,"米",5,3},
	{"ZMM",   "月最高水位、潮位",2,"米",7,3},
	{"ZMN",   "月最低水位、潮位",2,"米",7,3},
	{"ZS",   "水势状态",2,"米",5,3},
	{"ZU",   "库(闸、站)上水位",2,"米",5,3},
	{"ZUD",   "库(闸、站)上日平均水位",2,"米",5,3},
	{"ZUM",   "库(闸、站)上月平均水位",2,"米",5,3},
	{"ZUMM",   "库(闸、站)上月最高水位",2,"米",5,3},
	{"ZUMN",   "库(闸、站)上月最低水位",2,"米",5,3},
	{"ZUX",   "库(闸、站)上旬平均水位",2,"米",5,3},
	{"ZUXM",   "库(闸、站)上旬最高水位",2,"米",5,3},
	{"ZUXN",   "库(闸、站)上旬最低水位",2,"米",5,3},
	{"ZX",     "旬平均水位",2,"米",5,3},
	{"ZXM",   "旬最高水位、潮位",2,"米",5,3},
	{"ZXN",   "旬最低水位、潮位",2,"米",5,3},
	{"VT",    "电源电压",2,"伏特",3,2},
	{"DT",     "时段长、降水、引排水、抽水历时",1,"",4,0},
	{"ED",   "日蒸发量",2,"豪米",6,1},
	{"EM",    "月蒸发量",2,"豪米",6,1},
	{"EX",    "旬蒸发量",2,"豪米",6,1}
};

SYQ_TypeMap * SYQ_TypeMap::_syqinstance = 0;
SYQ_TypeMap::SYQ_TypeMap()
{
	InitMap();
}

SYQ_TypeMap::~SYQ_TypeMap()
{

}


void SYQ_TypeMap::InitMap()
{
	using namespace std;
	typedef pair<CString, SYQ_YS_STRUCT> PAIR;
	 		
	m_SYQTypeMap.insert(PAIR(CString("DRD"),   m_yaosu[0]));
	m_SYQTypeMap.insert(PAIR(CString("DRH"),   m_yaosu[1]));
	m_SYQTypeMap.insert(PAIR(CString("DRN"),   m_yaosu[2]));
	m_SYQTypeMap.insert(PAIR(CString("P1"),    m_yaosu[3]));
	m_SYQTypeMap.insert(PAIR(CString("P2"),    m_yaosu[4]));
	m_SYQTypeMap.insert(PAIR(CString("P3"),    m_yaosu[5]));
	m_SYQTypeMap.insert(PAIR(CString("P6"),    m_yaosu[6]));
	m_SYQTypeMap.insert(PAIR(CString("P12"),   m_yaosu[7]));
	m_SYQTypeMap.insert(PAIR(CString("PD"),    m_yaosu[8]));
	m_SYQTypeMap.insert(PAIR(CString("PM"),    m_yaosu[9]));
	m_SYQTypeMap.insert(PAIR(CString("PN01"),   m_yaosu[10]));
	m_SYQTypeMap.insert(PAIR(CString("PN05"),   m_yaosu[11]));
	m_SYQTypeMap.insert(PAIR(CString("PN10"),   m_yaosu[12]));
	m_SYQTypeMap.insert(PAIR(CString("PN30"),   m_yaosu[13]));
	m_SYQTypeMap.insert(PAIR(CString("PR"),    m_yaosu[14]));


	m_SYQTypeMap.insert(PAIR(CString("PX"),    m_yaosu[15]));
	m_SYQTypeMap.insert(PAIR(CString("Q"),    m_yaosu[16]));
	m_SYQTypeMap.insert(PAIR(CString("QA"),   m_yaosu[17]));
	m_SYQTypeMap.insert(PAIR(CString("QAD"),    m_yaosu[18]));
	m_SYQTypeMap.insert(PAIR(CString("QAM"),    m_yaosu[19]));
	m_SYQTypeMap.insert(PAIR(CString("QAMM"),    m_yaosu[20]));
	m_SYQTypeMap.insert(PAIR(CString("QAMN"),    m_yaosu[21]));
	m_SYQTypeMap.insert(PAIR(CString("QAX"),    m_yaosu[22]));
	m_SYQTypeMap.insert(PAIR(CString("QAXM"),  m_yaosu[23]));
	m_SYQTypeMap.insert(PAIR(CString("QAXN"),  m_yaosu[24]));
	m_SYQTypeMap.insert(PAIR(CString("QD"),    m_yaosu[25]));

	m_SYQTypeMap.insert(PAIR(CString("QI"),    m_yaosu[26]));
	m_SYQTypeMap.insert(PAIR(CString("QID"),   m_yaosu[27]));
	m_SYQTypeMap.insert(PAIR(CString("QIM"),   m_yaosu[28]));
	m_SYQTypeMap.insert(PAIR(CString("QIMM"),  m_yaosu[29]));
	m_SYQTypeMap.insert(PAIR(CString("QIMN"),  m_yaosu[30]));
	m_SYQTypeMap.insert(PAIR(CString("QIX"),   m_yaosu[31]));
	m_SYQTypeMap.insert(PAIR(CString("QIXM"),  m_yaosu[32]));
	m_SYQTypeMap.insert(PAIR(CString("QIXN"),  m_yaosu[33]));
	m_SYQTypeMap.insert(PAIR(CString("PJ"),    m_yaosu[34]));
	m_SYQTypeMap.insert(PAIR(CString("PT"),    m_yaosu[35]));
	m_SYQTypeMap.insert(PAIR(CString("DRP"),    m_yaosu[36]));
	m_SYQTypeMap.insert(PAIR(CString("DRZ"),    m_yaosu[37]));
	m_SYQTypeMap.insert(PAIR(CString("TT"),    m_yaosu[38]));
	m_SYQTypeMap.insert(PAIR(CString("Z"),    m_yaosu[39]));
	m_SYQTypeMap.insert(PAIR(CString("ZB"),    m_yaosu[40]));

	m_SYQTypeMap.insert(PAIR(CString("ZBD"),    m_yaosu[41]));
	m_SYQTypeMap.insert(PAIR(CString("ZBM"),    m_yaosu[42]));
	m_SYQTypeMap.insert(PAIR(CString("ZBMM"),    m_yaosu[43]));
	m_SYQTypeMap.insert(PAIR(CString("ZBMN"),    m_yaosu[44]));
	m_SYQTypeMap.insert(PAIR(CString("ZBX"),    m_yaosu[45]));
	m_SYQTypeMap.insert(PAIR(CString("ZBXM"),    m_yaosu[46]));
	m_SYQTypeMap.insert(PAIR(CString("ZBXN"),    m_yaosu[47]));
		//13
	m_SYQTypeMap.insert(PAIR(CString("ZD"),    m_yaosu[48]));
	m_SYQTypeMap.insert(PAIR(CString("ZH"),    m_yaosu[49]));
	m_SYQTypeMap.insert(PAIR(CString("ZHD"),    m_yaosu[50]));
	m_SYQTypeMap.insert(PAIR(CString("ZHH"),    m_yaosu[51]));
	m_SYQTypeMap.insert(PAIR(CString("ZHL"),    m_yaosu[52]));
	m_SYQTypeMap.insert(PAIR(CString("ZHM"),    m_yaosu[53]));
	m_SYQTypeMap.insert(PAIR(CString("ZHX"),    m_yaosu[54]));
	m_SYQTypeMap.insert(PAIR(CString("ZL"),    m_yaosu[55]));
	m_SYQTypeMap.insert(PAIR(CString("ZLD"),    m_yaosu[56]));
	m_SYQTypeMap.insert(PAIR(CString("ZLH"),    m_yaosu[57]));
	m_SYQTypeMap.insert(PAIR(CString("ZLL"),    m_yaosu[58]));
	m_SYQTypeMap.insert(PAIR(CString("ZLM"),    m_yaosu[59]));
	m_SYQTypeMap.insert(PAIR(CString("ZLX"),    m_yaosu[60]));
//15
	m_SYQTypeMap.insert(PAIR(CString("ZM"),    m_yaosu[61]));
	m_SYQTypeMap.insert(PAIR(CString("ZMM"),    m_yaosu[62]));
	m_SYQTypeMap.insert(PAIR(CString("ZMN"),    m_yaosu[63]));
	m_SYQTypeMap.insert(PAIR(CString("ZS"),    m_yaosu[64]));
	m_SYQTypeMap.insert(PAIR(CString("ZU"),    m_yaosu[65]));
	m_SYQTypeMap.insert(PAIR(CString("ZUD"),    m_yaosu[66]));
	m_SYQTypeMap.insert(PAIR(CString("ZUM"),    m_yaosu[67]));
	m_SYQTypeMap.insert(PAIR(CString("ZUMM"),    m_yaosu[68]));
	m_SYQTypeMap.insert(PAIR(CString("ZUMN"),    m_yaosu[69]));
	m_SYQTypeMap.insert(PAIR(CString("ZUX"),    m_yaosu[70]));

	m_SYQTypeMap.insert(PAIR(CString("ZUXM"),    m_yaosu[71]));
	m_SYQTypeMap.insert(PAIR(CString("ZUXN"),    m_yaosu[72]));
	m_SYQTypeMap.insert(PAIR(CString("ZX"),    m_yaosu[73]));
	m_SYQTypeMap.insert(PAIR(CString("ZXM"),    m_yaosu[74]));
	m_SYQTypeMap.insert(PAIR(CString("ZXN"),    m_yaosu[75]));
	m_SYQTypeMap.insert(PAIR(CString("VT"),    m_yaosu[76]));
	m_SYQTypeMap.insert(PAIR(CString("DT"),    m_yaosu[77]));
	m_SYQTypeMap.insert(PAIR(CString("ED"),    m_yaosu[78]));
	m_SYQTypeMap.insert(PAIR(CString("EM"),    m_yaosu[79]));
	m_SYQTypeMap.insert(PAIR(CString("EX"),    m_yaosu[80]));

}

SYQ_TypeMap* SYQ_TypeMap::GetInstance()
{
	if (_syqinstance == NULL)
	{
		_syqinstance = new SYQ_TypeMap();
	}

	return _syqinstance;
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
		case SCFactoryType_GTDZ:
			strResult = "国腾电子";
			break;
		case SCFactoryType_SZTH:
			strResult = "神州天鸿";
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

	CString CalcTime(struct tm *t, int span, int ret)
	{
		switch(ret)
		{
		case 1://span的单位为分钟
			{
				if (t->tm_min - span < 0)
				{
					t->tm_min = t->tm_min + 60 - span;
					if (t->tm_hour - 1 < 0)
					{
						t->tm_hour = t->tm_hour + 24 - 1;
					}
					else
					{
						t->tm_hour = t->tm_hour - 1;
					}
				}
				else
				{
					t->tm_min = t->tm_min - span;

				}
			}
			break;
		case 2://span单位为小时
			{
				if (t->tm_hour - span < 0)
				{
					t->tm_hour = t->tm_hour + 24 - span;
				}
				else
				{
					t->tm_hour = t->tm_hour - span;
				}
			}
			
			break;

		}
		
		CString str = "";
		str.Format("%.2d:%.2d", t->tm_hour, t->tm_min);
		return str;
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

	void InitHydrPara(SCSW_MsgInfo *p, CHydrFrameFormat *pHydr)
	{
		switch (p->m_nProtoType)
		{
		case SCSW_TT:
			{
				pHydr->m_cs_info.Lock();
				if (p->m_OpDataList.begin() != p->m_OpDataList.end())
				{
					//添加站码
					pHydr->m_TimeType = SCSW_TT;
					CString data = p->m_OpDataList.front();
					pHydr->m_DataTimeTT.month = atoi(data.Mid(0, 2));
					pHydr->m_DataTimeTT.day   = atoi(data.Mid(2, 2));
					pHydr->m_DataTimeTT.hour  = atoi(data.Mid(4, 2));
					pHydr->m_DataTimeTT.min   = atoi(data.Mid(6, 2));
				}
				pHydr->m_cs_info.Unlock();

			}
			break;
		case SCSW_DC://查询流水号
			pHydr->m_nCot = SCSW_DC;
			break;
		case SCSW_CN:
			pHydr->m_nCot = SCSW_CN;
			break;
		}

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

	bool SCSW008_hydrologic_to_APPFRAME(BYTE *buf, int len, CHydrFrameFormat* pHydrFrame)
	{
		if (!buf || !pHydrFrame)   return false;
		if (len <7 )			   return false;

		std::list<CString> list;
		CString asdu = (LPCTSTR)buf;
		CString resToken;
		int curPos = 0;

		//将接收缓冲区的数据分割成字符串
		resToken = asdu.Tokenize(_T(" "), curPos);
		while (resToken != _T(""))
		{
			list.push_back(resToken);
			resToken = asdu.Tokenize(_T(" "), curPos);
		}

		std::list<CString>::iterator it_list;
		std::map<CString, int>::iterator it;
		CString key = "";
		
		SCSW_MsgInfo *pInfo = NULL;
		const char *ptrChar = NULL;
		CString text = "";
		int textLen = 0;
		std::map<CString, int>::iterator it_map;
		for (it_list=list.begin(); it_list!=list.end(); it_list++)
		{
			text = (CString)(*it_list);
			textLen = text.GetLength();
			ptrChar = (LPCTSTR)text;
			int h = (int)(*ptrChar);
			int t = (int)(*(ptrChar+textLen-1));

			if (10 == textLen )
			{
				if (pInfo!=NULL)
				{
					if (pInfo->m_nProtoType == SCSW_ST)
					{
						pInfo->m_cs.Lock();
						pInfo->m_OpDataList.push_back(text);
						pInfo->m_cs.Unlock();

						pHydrFrame->m_strStCode = text;

						pHydrFrame->m_cs_info.Lock();
						if (pInfo->m_OpDataList.begin() != pInfo->m_OpDataList.end())
						{
							//添加站码
							pHydrFrame->m_strStCode = pInfo->m_OpDataList.front();
						}
						pHydrFrame->m_cs_info.Unlock();

					}
				}
			}
			else
			{
				if ((h>=65 && h<=90) && (t>=65 && t<=90))
				{//首尾都是大写字母
					if (pInfo != NULL)
					{
						if (pInfo->m_nProtoType != -1)
						{
							InitHydrPara(pInfo, pHydrFrame);
							pHydrFrame->m_cs_info.Lock();
							pHydrFrame->m_MsgInfoList.push_back(pInfo);
							pHydrFrame->m_cs_info.Unlock();
						}

					}

					it_map = SCSW_TypeMap::GetInstance()->m_SCSWTypeMap.find(text);
					if (it_map != SCSW_TypeMap::GetInstance()->m_SCSWTypeMap.end())
					{
						pInfo = new SCSW_MsgInfo();
						pInfo->m_nProtoType = (int)it_map->second;
						pInfo->m_nExtraInfo = -1;
					}
					else
					{
						pInfo = NULL;
					}

				}
				else if ((h>=65 && h<=90) && (t>=48 && t<=57))
				{//首是字母，尾是数字  如Z01
					int d;
					int idx = 0;
					for (int i=0; i<textLen; i++)
					{
						d = (int)(*(ptrChar+i));
						if (!(d>=65 && d<=90))
						{
							break;
						}
						idx++;
					}

					if (pInfo != NULL)
					{
						if (pInfo->m_nProtoType != -1)
						{
							InitHydrPara(pInfo, pHydrFrame);

							pHydrFrame->m_cs_info.Lock();
							pHydrFrame->m_MsgInfoList.push_back(pInfo);
							pHydrFrame->m_cs_info.Unlock();
						}

					}

					it_map = it_map = SCSW_TypeMap::GetInstance()->m_SCSWTypeMap.find(text.Mid(0,idx));
					if (it_map != SCSW_TypeMap::GetInstance()->m_SCSWTypeMap.end())
					{
						pInfo = new SCSW_MsgInfo();
						pInfo->m_nProtoType = (int)it_map->second;
						pInfo->m_nExtraInfo = atoi(text.Mid(idx, textLen-idx));
					}
					else
					{
						pInfo = NULL;
					}

				}
				else if ((h>=48 && h<=57) && (t>=48 && t<=57))
				{//首尾都是数字
					if (pInfo != NULL)
					{
						pInfo->m_cs.Lock();
						pInfo->m_OpDataList.push_back(text);
						pInfo->m_cs.Unlock();
					}
				}
				else if (h>=97 && h<=122 || 58 == h) //58为冒号
				{//首尾小写字母，证明已经到了结尾的校验部分，把最后一个字符串添加到链表里
					if (pInfo != NULL)
					{
						if (pInfo->m_nProtoType != -1)
						{
							InitHydrPara(pInfo, pHydrFrame);

							pHydrFrame->m_cs_info.Lock();
							pHydrFrame->m_MsgInfoList.push_back(pInfo);
							pHydrFrame->m_cs_info.Unlock();
						}
					}		
				}

			}
			

		}

		return true;
	}

	bool SYQ_Down_APPFRAME(BYTE *buffer, int len, CSYQFrameFormat* syqFrame)
	{
		if (!buffer || !syqFrame)   return false;
		if (len <25 )				  return false;

		syqFrame->nMsgLen = buffer[20]*256+buffer[19];
		syqFrame->m_CenterCode = buffer[12]*256+buffer[11];
		syqFrame->m_stCode = ByteFromBCD(buffer[5])+ByteFromBCD(buffer[4])*100+ByteFromBCD(buffer[3])*10000+ByteFromBCD(buffer[2])*1000000;
		syqFrame->m_iPassword = buffer[14]*256+buffer[13];
		syqFrame->m_FunType = buffer[17];

		syqFrame->_asdu.data.clear();
		syqFrame->_asdu.data =std::vector<unsigned char>(syqFrame->nMsgLen);
		memmove(&*(syqFrame->_asdu.data.begin()),&buffer[22],syqFrame->nMsgLen);

		return true;
	}

	bool SYQ_to_APPFRAME(BYTE *buffer, int len, CSYQFrameFormat* syqFrame)
	{
		if (!buffer || !syqFrame)   return false;
		if (len <25 )				  return false;

		syqFrame->nMsgLen = buffer[20]*256+buffer[19];
		syqFrame->m_CenterCode = buffer[2]*256+buffer[1];
		syqFrame->m_stCode = ByteFromBCD(buffer[7])+ByteFromBCD(buffer[6])*100+ByteFromBCD(buffer[5])*10000+ByteFromBCD(buffer[4])*1000000;
		syqFrame->m_iPassword = buffer[14]*256+buffer[13];
		syqFrame->m_FunType = buffer[17];
		if(buffer[22]!='T')
			syqFrame->messageno =  ByteFromBCD(buffer[22]);

		syqFrame->_asdu.data.clear();
		syqFrame->_asdu.data =std::vector<unsigned char>(syqFrame->nMsgLen);
		memmove(&*(syqFrame->_asdu.data.begin()),&buffer[22],syqFrame->nMsgLen);

		return true;
	}

	bool WRITE_frame_to_APPFRAME(BYTE *buf, int len, CWriteFrame* pWaterFrame)
	{
		if (!buf || !pWaterFrame)   return false;
		if (len <7 )			   return false;

		pWaterFrame->m_nFrameLen = buf[2];

		CString strStCode = "";
		for (int i=0; i<10; i++)
		{
			strStCode += buf[3+i];
		}

		if (len == 0x12)
		{
			pWaterFrame->m_nDevAdddr = strStCode;
			pWaterFrame->m_nCmdCode = buf[13];
			pWaterFrame->m_nCRC = (buf[14] << 8 ) | buf[15];
			pWaterFrame->bHasContentFrame = false;
		}
		else
		{
			pWaterFrame->bHasContentFrame = true;
			pWaterFrame->m_nDevAdddr = strStCode;
			pWaterFrame->m_nCmdCode = buf[13];
			pWaterFrame->m_nPackCnt = (buf[14]<<8) | buf[15];
			pWaterFrame->m_nCodeAddr = (buf[18]<<24) | (buf[19]<<16) | (buf[20]<<8) | buf[21];
			pWaterFrame->m_nCodeLen = buf[22];
			memcpy(pWaterFrame->m_ContentBuf, 23+buf, pWaterFrame->m_nCodeLen);
			pWaterFrame->m_nCRC = (buf[23+pWaterFrame->m_nCodeLen] << 8 ) | buf[24+pWaterFrame->m_nCodeLen];
		}

		return true;
	}



	bool CheckCRC_SCSW(BYTE *buffer, int len)
	{
		if (0 == len)
		{
			return false;
		}

		BYTE sum = 0;
		for (int i=0; i<len-4; i++)
		{
			sum += buffer[i];
		}

		sum = ~sum + 1;
		BYTE lo = (sum & 0x0F) + 0x61;
		BYTE hi = ((sum & 0xF0) >> 4) + 0x61;

		if ((lo==buffer[len-3])&&(hi==buffer[len-4]))
		{
			return true;
		}

		return false;
	}
	WORD CreateCRC_SCSW(BYTE *buffer, int len)
	{
		if (0 == len)
		{
			return 0;
		}

		BYTE sum = 0;
		for (int i=0; i<len; i++)
		{
			sum += buffer[i];
		}

		sum = ~sum + 1;
		BYTE lo = (sum & 0x0F) + 0x61;
		BYTE hi = ((sum & 0xF0) >> 4) + 0x61;

		WORD var = (hi << 8) | lo;
		return var;

	}

	const WORD auchCRCHi[256]= {
		0x0000,0x00C1,0x0081,0x0040,0x0001,0x00C0,0x0080,0x0041,0x0001,0x00C0,
		0x0080,0x0041,0x0000,0x00C1,0x0081,0x0040,0x0001,0x00C0,0x0080,0x0041,
		0x0000,0x00C1,0x0081,0x0040,0x0000,0x00C1,0x0081,0x0040,0x0001,0x00C0,
		0x0080,0x0041,0x0001,0x00C0,0x0080,0x0041,0x0000,0x00C1,0x0081,0x0040,
		0x0000,0x00C1,0x0081,0x0040,0x0001,0x00C0,0x0080,0x0041,0x0000,0x00C1,
		0x0081,0x0040,0x0001,0x00C0,0x0080,0x0041,0x0001,0x00C0,0x0080,0x0041,
		0x0000,0x00C1,0x0081,0x0040,0x0001,0x00C0,0x0080,0x0041,0x0000,0x00C1,
		0x0081,0x0040,0x0000,0x00C1,0x0081,0x0040,0x0001,0x00C0,0x0080,0x0041,
		0x0000,0x00C1,0x0081,0x0040,0x0001,0x00C0,0x0080,0x0041,0x0001,0x00C0,
		0x0080,0x0041,0x0000,0x00C1,0x0081,0x0040,0x0000,0x00C1,0x0081,0x0040,
		0x0001,0x00C0,0x0080,0x0041,0x0001,0x00C0,0x0080,0x0041,0x0000,0x00C1,
		0x0081,0x0040,0x0001,0x00C0,0x0080,0x0041,0x0000,0x00C1,0x0081,0x0040,
		0x0000,0x00C1,0x0081,0x0040,0x0001,0x00C0,0x0080,0x0041,0x0001,0x00C0,
		0x0080,0x0041,0x0000,0x00C1,0x0081,0x0040,0x0000,0x00C1,0x0081,0x0040,
		0x0001,0x00C0,0x0080,0x0041,0x0000,0x00C1,0x0081,0x0040,0x0001,0x00C0,
		0x0080,0x0041,0x0001,0x00C0,0x0080,0x0041,0x0000,0x00C1,0x0081,0x0040,
		0x0000,0x00C1,0x0081,0x0040,0x0001,0x00C0,0x0080,0x0041,0x0001,0x00C0,
		0x0080,0x0041,0x0000,0x00C1,0x0081,0x0040,0x0001,0x00C0,0x0080,0x0041,
		0x0000,0x00C1,0x0081,0x0040,0x0000,0x00C1,0x0081,0x0040,0x0001,0x00C0,
		0x0080,0x0041,0x0000,0x00C1,0x0081,0x0040,0x0001,0x00C0,0x0080,0x0041,
		0x0001,0x00C0,0x0080,0x0041,0x0000,0x00C1,0x0081,0x0040,0x0001,0x00C0,
		0x0080,0x0041,0x0000,0x00C1,0x0081,0x0040,0x0000,0x00C1,0x0081,0x0040,
		0x0001,0x00C0,0x0080,0x0041,0x0001,0x00C0,0x0080,0x0041,0x0000,0x00C1,
		0x0081,0x0040,0x0000,0x00C1,0x0081,0x0040,0x0001,0x00C0,0x0080,0x0041,
		0x0000,0x00C1,0x0081,0x0040,0x0001,0x00C0,0x0080,0x0041,0x0001,0x00C0,
		0x0080,0x0041,0x0000,0x00C1,0x0081,0x0040
	};

	const	WORD auchCRCLo[256]= {
		0x0000,0x00C0,0x00C1,0x0001,0x00C3,0x0003,0x0002,0x00C2,0x00C6,0x0006,
		0x0007,0x00C7,0x0005,0x00C5,0x00C4,0x0004,0x00CC,0x000C,0x000D,0x00CD,
		0x000F,0x00CF,0x00CE,0x000E,0x000A,0x00CA,0x00CB,0x000B,0x00C9,0x0009,
		0x0008,0x00C8,0x00D8,0x0018,0x0019,0x00D9,0x001B,0x00DB,0x00DA,0x001A,
		0x001E,0x00DE,0x00DF,0x001F,0x00DD,0x001D,0x001C,0x00DC,0x0014,0x00D4,
		0x00D5,0x0015,0x00D7,0x0017,0x0016,0x00D6,0x00D2,0x0012,0x0013,0x00D3,
		0x0011,0x00D1,0x00D0,0x0010,0x00F0,0x0030,0x0031,0x00F1,0x0033,0x00F3,
		0x00F2,0x0032,0x0036,0x00F6,0x00F7,0x0037,0x00F5,0x0035,0x0034,0x00F4,
		0x003C,0x00FC,0x00FD,0x003D,0x00FF,0x003F,0x003E,0x00FE,0x00FA,0x003A,
		0x003B,0x00FB,0x0039,0x00F9,0x00F8,0x0038,0x0028,0x00E8,0x00E9,0x0029,
		0x00EB,0x002B,0x002A,0x00EA,0x00EE,0x002E,0x002F,0x00EF,0x002D,0x00ED,
		0x00EC,0x002C,0x00E4,0x0024,0x0025,0x00E5,0x0027,0x00E7,0x00E6,0x0026,
		0x0022,0x00E2,0x00E3,0x0023,0x00E1,0x0021,0x0020,0x00E0,0x00A0,0x0060,
		0x0061,0x00A1,0x0063,0x00A3,0x00A2,0x0062,0x0066,0x00A6,0x00A7,0x0067,
		0x00A5,0x0065,0x0064,0x00A4,0x006C,0x00AC,0x00AD,0x006D,0x00AF,0x006F,
		0x006E,0x00AE,0x00AA,0x006A,0x006B,0x00AB,0x0069,0x00A9,0x00A8,0x0068,
		0x0078,0x00B8,0x00B9,0x0079,0x00BB,0x007B,0x007A,0x00BA,0x00BE,0x007E,
		0x007F,0x00BF,0x007D,0x00BD,0x00BC,0x007C,0x00B4,0x0074,0x0075,0x00B5,
		0x0077,0x00B7,0x00B6,0x0076,0x0072,0x00B2,0x00B3,0x0073,0x00B1,0x0071,
		0x0070,0x00B0,0x0050,0x0090,0x0091,0x0051,0x0093,0x0053,0x0052,0x0092,
		0x0096,0x0056,0x0057,0x0097,0x0055,0x0095,0x0094,0x0054,0x009C,0x005C,
		0x005D,0x009D,0x005F,0x009F,0x009E,0x005E,0x005A,0x009A,0x009B,0x005B,
		0x0099,0x0059,0x0058,0x0098,0x0088,0x0048,0x0049,0x0089,0x004B,0x008B,
		0x008A,0x004A,0x004E,0x008E,0x008F,0x004F,0x008D,0x004D,0x004C,0x008C,
		0x0044,0x0084,0x0085,0x0045,0x0087,0x0047,0x0046,0x0086,0x0082,0x0042,
		0x0043,0x0083,0x0041,0x0081,0x0080,0x0040
	};

	WORD CreateCRC_Write(BYTE *buffer, int len)
	{
		BYTE uchCRCHi = 0xFF ;
		BYTE uchCRCLo = 0xFF ;
		BYTE uIndex ,i=0;
		BYTE usDataLen;
		usDataLen=len;
		WORD temp;
		while (usDataLen--)
		{
			uIndex = uchCRCHi ^ buffer[i++];
			uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
			uchCRCLo = auchCRCLo[uIndex];
		}
		temp=uchCRCHi;
		temp<<=8;
		temp+=uchCRCLo;


		return temp;

	}

	bool CheckCRC_Write(BYTE *buffer, int len)
	{
		BYTE uchCRCHi = 0xFF ;
		BYTE uchCRCLo = 0xFF ;
		BYTE uIndex,i=0 ;
		BYTE usDataLen;
		BYTE tempbuf[256];
		WORD temp;

		for(i=0; i<len; i++)
			tempbuf[i]=buffer[i];
		i=0;
		usDataLen=len-4;
		while (usDataLen--)
		{
			uIndex = uchCRCHi ^ tempbuf[i++];
			uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
			uchCRCLo = auchCRCLo[uIndex];
		}
		temp=uchCRCHi;
		temp<<=8;
		temp+=uchCRCLo;

		if(uchCRCHi==tempbuf[len-4] && uchCRCLo==tempbuf[len-3])        
			return true;
		else
			return false;
	}



	WORD CreateCRC_SYQ(BYTE *buffer, int len)
	{
		BYTE uchCRCHi = 0xFF ;
		BYTE uchCRCLo = 0xFF ;
		BYTE uIndex ,i=0;
		BYTE usDataLen;
		usDataLen=len;
		WORD temp;
		while (usDataLen--)
		{
			uIndex = uchCRCHi ^ buffer[i++];
			uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
			uchCRCLo = auchCRCLo[uIndex];
		}
		temp=uchCRCHi;
		temp<<=8;
		temp+=uchCRCLo;


		return temp;

	}
	bool CheckCRC_SYQ(BYTE *buffer, int len)
	{
		BYTE uchCRCHi = 0xFF ;
		BYTE uchCRCLo = 0xFF ;
		BYTE uIndex,i=0 ;
		BYTE usDataLen;
		BYTE tempbuf[256];
		WORD temp;

		for(i=0; i<len; i++)
			tempbuf[i]=buffer[i];
		i=0;
		usDataLen=len-2;
		while (usDataLen--)
		{
			uIndex = uchCRCHi ^ tempbuf[i++];
			uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
			uchCRCLo = auchCRCLo[uIndex];
		}
		temp=uchCRCHi;
		temp<<=8;
		temp+=uchCRCLo;

		if(uchCRCHi==tempbuf[len-2] && uchCRCLo==tempbuf[len-1])        
			return true;
		else
			return false;

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

SCSW_ASDU::SCSW_ASDU()
{
	memset(data, 0, 300);
}

SCSW_ASDU::~SCSW_ASDU()
{
	memset(data, 0, 300);

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

void CHydrFrameFormat::UpdateRecvTime(PSYSTEMTIME pTime)
{
	if (!pTime)
	{
		GetLocalTime(&m_stRecvTime);
		m_DataTimeTT.hour = m_stRecvTime.wHour;
		m_DataTimeTT.day  = m_stRecvTime.wDay;
		m_DataTimeTT.month = m_stRecvTime.wMonth;
		m_DataTimeTT.min   = m_stRecvTime.wMinute;

	}
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
		m_DataTimeTT.hour = pTime->wHour;
		m_DataTimeTT.day  = pTime->wDay;
		m_DataTimeTT.month = pTime->wMonth;
		m_DataTimeTT.min = pTime->wMinute;
	}
}

void CWriteFrame::UpdateRecvTime(PSYSTEMTIME pTime)
{
	if (!pTime)
	{
		GetLocalTime(&m_stRecvTime);
		m_DataTimeTT.hour = m_stRecvTime.wHour;
		m_DataTimeTT.day  = m_stRecvTime.wDay;
		m_DataTimeTT.month = m_stRecvTime.wMonth;
		m_DataTimeTT.min   = m_stRecvTime.wMinute;

	}
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
		m_DataTimeTT.hour = pTime->wHour;
		m_DataTimeTT.day  = pTime->wDay;
		m_DataTimeTT.month = pTime->wMonth;
		m_DataTimeTT.min = pTime->wMinute;
	}
}

void CSYQFrameFormat::UpdateRecvTime( PSYSTEMTIME pTime/*= NULL*/ )
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

_TP& _TP::operator=( _TP &_tp )
{
	this->m_adelay = _tp.m_adelay;
	this->m_day = _tp.m_day;
	this->m_hour = _tp.m_hour;
	this->m_minute = _tp.m_minute;
	this->m_second = _tp.m_second;
	return (*this);
}
