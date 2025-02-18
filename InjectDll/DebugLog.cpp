#include "StdAfx.h"
#include "DebugLog.h"




#ifdef DEBUG_LOG


clock_t start_time;

// 获取时间
void GetLocalTime(PCHAR lpCurTime, int &nTimeLen)
{
	SYSTEMTIME tagCurTime = {0};
	GetLocalTime(&tagCurTime);
	sprintf(lpCurTime, "%04d-%02d-%02d %02d:%02d:%02d:%04d  ", 
		tagCurTime.wYear,
		tagCurTime.wMonth,
		tagCurTime.wDay,
		tagCurTime.wHour,
		tagCurTime.wMinute,
		tagCurTime.wSecond,
		tagCurTime.wMilliseconds);

	nTimeLen = strlen(lpCurTime);
}

void Log(int nLogType, PCHAR lpParameter, ...)
{
	char szOut[1000] = {0};
	va_list ap = NULL;
	va_start(ap, lpParameter);

	int nTimeLen = 0;
	GetLocalTime(szOut, nTimeLen);
	vsprintf(szOut+nTimeLen, lpParameter, ap);
	strcat(szOut, "\r\n");
	
	if(nLogType == LOG_TYPE_OUTPUT)
	{
		OutputDebugString(szOut);
	}
	else if(nLogType == LOG_TYPE_CONTROL)
	{
		printf("%s", szOut);
	}
	else if(nLogType == LOG_TYPE_MSG)
	{
		MessageBox(NULL, szOut, NULL, MB_ICONINFORMATION);
	}
}

void LogOutput(PCHAR lpParameter, ...)
{
	char szOut[5000] = {0};
	va_list ap = NULL;
	va_start(ap, lpParameter);

	int nTimeLen = 0;
	GetLocalTime(szOut, nTimeLen);
	vsprintf(szOut+nTimeLen, lpParameter, ap);

	strcat(szOut, "\r\n");
	
	OutputDebugString(szOut);
}

void LogConsole(PCHAR lpParameter, ...)
{
	char szOut[1000] = {0};
	va_list ap = NULL;
	va_start(ap, lpParameter);
	
	int nTimeLen = 0;
	GetLocalTime(szOut, nTimeLen);
	vsprintf(szOut+nTimeLen, lpParameter, ap);

	strcat(szOut, "\r\n");
	
	printf("%s", szOut);
}

void LogMsg(PCHAR lpParameter, ...)
{
	char szOut[1000] = {0};
	va_list ap = NULL;
	va_start(ap, lpParameter);
	
	int nTimeLen = 0;
	GetLocalTime(szOut, nTimeLen);
	vsprintf(szOut+nTimeLen, lpParameter, ap);

	strcat(szOut, "\r\n");
	
	MessageBox(NULL, szOut, NULL, MB_ICONINFORMATION);
}
void LogFile(PCHAR lpParameter, ...)
{
	char szOut[1000] = {0};
	va_list ap = NULL;
	va_start(ap, lpParameter);
	
	int nTimeLen = 0;
	GetLocalTime(szOut, nTimeLen);
	vsprintf(szOut+nTimeLen, lpParameter, ap);

	strcat(szOut, "\r\n");
	
	FILE* file = NULL;
	file = fopen(LOG_FILE, "ab+");
	fprintf(file, "%s", szOut);
	fclose(file);
}



#endif
