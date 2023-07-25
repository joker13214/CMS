#include "StdAfx.h"
#include "SystemTimeOp.h"

CSystemTimeOp::CSystemTimeOp()
{

}

CSystemTimeOp::~CSystemTimeOp()
{

}

//把时间从srcSystemTime复制到desSystemTime
void CSystemTimeOp::CopySystemTime(PSYSTEMTIME srcSystemTime, PSYSTEMTIME desSystemTime)
{
	desSystemTime->wYear         = srcSystemTime->wYear;
	desSystemTime->wMonth        = srcSystemTime->wMonth;
	desSystemTime->wDay          = srcSystemTime->wDay;
	desSystemTime->wHour         = srcSystemTime->wHour;
	desSystemTime->wMinute       = srcSystemTime->wMinute;
	desSystemTime->wSecond       = srcSystemTime->wSecond;
	desSystemTime->wMilliseconds = srcSystemTime->wMilliseconds;
}

//把给定的毫秒加到指定的时间上
void CSystemTimeOp::AddSystemTime(SYSTEMTIME& desSystemTime, double ms)
{
	CTime tmpTime(desSystemTime);
	int tmpms = int(ms / 1000); //秒
	int mss   = (int)((int)ms % 1000) + desSystemTime.wMilliseconds; //毫秒
	if (mss >= 1000) //如果毫秒大于1秒
	{
		tmpms = tmpms + 1; //秒加1
		mss   = mss - 1000; //得到新的毫秒
	}
	else if (mss < 0)
	{
		tmpms = tmpms - 1;
		mss   = mss + 1000;  
	}
	CTimeSpan timeSpan(abs(tmpms));
	CTime tm;
	if (ms >=0)
		tm = tmpTime + timeSpan;
	else
		tm = tmpTime - timeSpan;

	tm.GetAsSystemTime(desSystemTime);
	desSystemTime.wMilliseconds = mss;
}

//返回毫秒(srcSystemTime - desSystemTime)
INT64 CSystemTimeOp::MinusSystemTime(const SYSTEMTIME& srcSystemTime, const SYSTEMTIME& desSystemTime)
{
	CTime srcTime(srcSystemTime);
	CTime desTime(desSystemTime);
	CTimeSpan ts = srcTime - desTime;
	INT64 ms = (INT64)ts.GetTotalSeconds() * (INT64)1000 + srcSystemTime.wMilliseconds - desSystemTime.wMilliseconds;
	return ms;
}

//时间String转换 格式:2011-01-01 01:00:00.000
BOOL CSystemTimeOp::ConvertStringToSysemTime(CString sTime, PSYSTEMTIME pSystemTime)
{
	try
	{
		int endIndex=0, startIndex=0;
		CString sSubStr, sstr;
		endIndex = sTime.Find("-", 0);

		int year=0,month=0,day=0,hour=0,minute=0,second=0,milsec=0;

		if (endIndex != -1) //year
		{
			sSubStr = sTime.Left(endIndex);
			year    = atoi(sSubStr);
		}

		startIndex = endIndex + 1;

		endIndex = sTime.Find("-", startIndex);
		if (endIndex != -1) //month
		{
			sSubStr = sTime.Left(endIndex);
			sstr = sSubStr;
			sSubStr = sstr.Right(endIndex - startIndex);
			month = atoi(sSubStr);
		}

		startIndex = endIndex + 1;

		endIndex = sTime.Find(" ", startIndex);
		if (endIndex != -1) //day
		{
			sSubStr = sTime.Left(endIndex);
			sstr = sSubStr;
			sSubStr = sstr.Right(endIndex - startIndex);
			day = atoi(sSubStr);
		}

		startIndex = endIndex + 1;

		endIndex = sTime.Find(":", startIndex);
		if (endIndex != -1) //hour
		{
			sSubStr = sTime.Left(endIndex);
			sstr = sSubStr;
			sSubStr = sstr.Right(endIndex - startIndex);
			hour = atoi(sSubStr);
		}
		startIndex = endIndex + 1;
		endIndex = sTime.Find(":", startIndex);
		if (endIndex != -1) //minute
		{
			sSubStr = sTime.Left(endIndex);
			sstr = sSubStr;
			sSubStr = sstr.Right(endIndex - startIndex);
			minute = atoi(sSubStr);
		}
		startIndex = endIndex + 1;
		endIndex = sTime.Find(".", startIndex);
		if (endIndex != -1) //second
		{
			sSubStr = sTime.Left(endIndex);
			sstr = sSubStr;
			sSubStr = sstr.Right(endIndex - startIndex);
			second = atoi(sSubStr);
		}
		startIndex = endIndex + 1;
		sSubStr = sTime.Right(sTime.GetLength() - startIndex);
		milsec = atoi(sSubStr);

		pSystemTime->wYear  = year;
		pSystemTime->wMonth = month;
		pSystemTime->wDay   = day;
		pSystemTime->wHour  = hour;
		pSystemTime->wMinute = minute;
		pSystemTime->wSecond = second;
		pSystemTime->wMilliseconds = milsec;
	}
	catch(...)
	{
		return FALSE;
	}

	return TRUE;
}

int CSystemTimeOp::CompareSystemTime(SYSTEMTIME* srcSystemTime, SYSTEMTIME* desSystemTime)
{

	//double ms = (double)CSystemTimeOp::MinusSystemTime(*srcSystemTime, *desSystemTime);
	//if (ms > 0.0) 
	//	return 1;
	//else if (ms < 0.0) 
	//	return -1;
	//else	
	//	return 0;

	INT64 ms = CSystemTimeOp::MinusSystemTime(*srcSystemTime, *desSystemTime);
	if (ms > 0) 
		return 1;
	else if (ms < 0) 
		return -1;
	else	
		return 0;
}

//与当前时间相比
// -1  <  0 =  1 >
int CSystemTimeOp::CompareSystemTime( SYSTEMTIME* srcSystemTime )
{
	SYSTEMTIME curST;
	GetLocalTime(&curST);
	return CompareSystemTime(srcSystemTime,&curST);
}

int CSystemTimeOp::GetDaysOfMonth(int year, int month)
{
	int days;
	switch (month)
	{
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		days = 31;
		break;
	case 2:
		if ((((year % 4) == 0) && ((year % 100) != 0)) || ((year % 400) == 0))
			days = 29;
		else
			days = 28;
		break;
	default:
		days = 30;
		break;
	}
	return days;

}


CString CSystemTimeOp::ConvertSystemTimeToString(PSYSTEMTIME pSystemTime,bool blIncludeMilliseconds)
{
	CString strResult="";
	if (pSystemTime != NULL)
	{
		if (blIncludeMilliseconds == true)
		{
			strResult.Format("%04d-%02d-%02d %02d:%02d:%02d.%03d",
				pSystemTime->wYear,
				pSystemTime->wMonth,
				pSystemTime->wDay,
				pSystemTime->wHour,
				pSystemTime->wMinute,
				pSystemTime->wSecond,
				pSystemTime->wMilliseconds);
		}
		else
		{
			strResult.Format("%04d-%02d-%02d %02d:%02d:%02d",
				pSystemTime->wYear,
				pSystemTime->wMonth,
				pSystemTime->wDay,
				pSystemTime->wHour,
				pSystemTime->wMinute,
				pSystemTime->wSecond);
		}
	}

	return strResult;
}

INT64 CSystemTimeOp::MinusCTime( CTime* srcTime,CTime* desTime )
{
	INT64 minus = 0;
	if (srcTime != NULL && desTime != NULL)
	{
		CTimeSpan _minusSpan = (*srcTime)-(*desTime);
		minus = _minusSpan.GetTotalSeconds();
	}
	return minus;
}

void CSystemTimeOp::ConvertCTimeToSystemtime( CTime* psrcCTime,SYSTEMTIME* pdstSystemtime )
{
	if (psrcCTime  && pdstSystemtime)
	{
		pdstSystemtime->wYear	= psrcCTime->GetYear();
		pdstSystemtime->wMonth	= psrcCTime->GetMonth();
		pdstSystemtime->wDay	= psrcCTime->GetDay();
		pdstSystemtime->wHour	= psrcCTime->GetHour();
		pdstSystemtime->wMinute = psrcCTime->GetMinute();
		pdstSystemtime->wSecond = psrcCTime->GetSecond();
		pdstSystemtime->wMilliseconds = 0;
	}
}

void CSystemTimeOp::ResetSystemTime( SYSTEMTIME& st )
{
	memset(&st,0,sizeof(st));
}
