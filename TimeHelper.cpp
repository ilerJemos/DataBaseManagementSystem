#include "StdAfx.h"
#include "TimeHelper.h"
#include <comutil.h>


CString CTimeHelper::ToLDatetimeString(SYSTEMTIME t)
{
	CString strTime;
	strTime.Format(_T("%04u-%02u-%02u %02u:%02u:%02u %03u"), t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
	return strTime;
}


CString CTimeHelper::ToDatetimeString(SYSTEMTIME t)
{
	CString strTime;
	strTime.Format(_T("%04u-%02u-%02u %02u:%02u:%02u"), t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
	return strTime;
}


CString CTimeHelper::ToDateString(SYSTEMTIME t)
{
	CString strTime;
	strTime.Format(_T("%04u-%02u-%02u"), t.wYear, t.wMonth, t.wDay);
	return strTime;
}


CString CTimeHelper::ToTimeString(SYSTEMTIME t)
{
	CString strTime;
	strTime.Format(_T("%02u:%02u:%02u"), t.wHour, t.wMinute, t.wSecond);
	return strTime;
}


CString CTimeHelper::ToLTimeString(SYSTEMTIME t)
{
	CString strTime;
	strTime.Format(_T("%02u:%02u:%02u %03u"), t.wHour, t.wMinute, t.wSecond,  t.wMilliseconds);
	return strTime;
}


SYSTEMTIME CTimeHelper::ToSystemTime(CString strTime)
{
	SYSTEMTIME st;
	memset(&st, 0, sizeof(SYSTEMTIME));
	if (strTime.GetLength() > 0)
	{
		int nYear = _wtoi(strTime.Left(4));
		int nMonth  = _wtoi(strTime.Mid(5, 2));
		int nDay  = _wtoi(strTime.Mid(8, 2));
		int nHour  = _wtoi(strTime.Mid(11, 2));
		int nMinute  = _wtoi(strTime.Mid(14, 2));
		int nSecord  = _wtoi(strTime.Mid(17, 2));
		int nMillisecond  = _wtoi(strTime.Mid(20, 3));
		
		st.wYear = nYear;
		st.wMonth = nMonth;
		st.wDay = nDay;
		st.wHour = nHour;
		st.wMinute = nMinute;
		st.wSecond = nSecord;
		st.wMilliseconds = nMillisecond;
	}
	return st;
}


SYSTEMTIME CTimeHelper::GetCurrentTime()
{
	SYSTEMTIME t;
	::GetLocalTime(&t);
	return t;
}


SYSTEMTIME CTimeHelper::ToSystemTime(CTime t)
{
	SYSTEMTIME stCur;
	t.GetAsSystemTime(stCur);
	return stCur;
}


CTime CTimeHelper::ToCTime(SYSTEMTIME st)
{
	return CTime(st);
}