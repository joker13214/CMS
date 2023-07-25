#pragma once

class CSystemTimeOp
{
public:
	static void CopySystemTime(PSYSTEMTIME srcSystemTime, PSYSTEMTIME desSystemTime);
	static void AddSystemTime(SYSTEMTIME& desSystemTime, double millseconds);
	static INT64 MinusSystemTime(const SYSTEMTIME& srcSystemTime, const SYSTEMTIME& desSystemTime);
	static BOOL ConvertStringToSysemTime(CString sTime, PSYSTEMTIME pSystemTime);
	static int  CompareSystemTime(SYSTEMTIME* srcSystemTime, SYSTEMTIME* desSystemTime); //-1= <,0 = =, 1 = > 
	static int  GetDaysOfMonth(int year, int month);
	static CString ConvertSystemTimeToString(PSYSTEMTIME pSystemTime,bool blIncludeMilliseconds=false);
	static INT64 MinusCTime(CTime* srcTime,CTime* desTime);
	static void ConvertCTimeToSystemtime(CTime* psrcCTime,SYSTEMTIME* pdstSystemtime);

protected:
	virtual ~CSystemTimeOp();
private:
	CSystemTimeOp();
};
