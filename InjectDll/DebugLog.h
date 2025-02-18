#pragma once

#include <windows.h>
#include <crtdbg.h>
#include <stdio.h>

#include <time.h>

#pragma comment(lib, "user32")

// warning C4002: too many actual parameters for macro 'LogMsg'
#pragma warning (disable:4002)

enum
{
	LOG_TYPE_OUTPUT,
	LOG_TYPE_CONTROL,
	LOG_TYPE_MSG,
};

//#define DEBUG_LOG_
#define LOG_FILE "C:\\test.txt"

#ifdef DEBUG_LOG
#define _LogOutPut0(x)					_RPT0(_CRT_WARN, x)
#define _LogOutPut1(x, y)				_RPT1(_CRT_WARN, x, y)
#define _LogOutPut2(x, y, z)			_RPT2(_CRT_WARN, x, y, z)
#define _LogOutPut3(x, y, z, w)			_RPT3(_CRT_WARN, x, y, z, w)
#define _LogOutPut4(x, y, z, w, b)		_RPT4(_CRT_WARN, x, y, z, w, b)
#else
#define _LogOutPut0(x)
#define _LogOutPut1(x, y)
#define _LogOutPut2(x, y, z)
#define _LogOutPut3(x, y, z, w)
#define _LogOutPut4(x, y, z, w, b)
#endif



#ifdef DEBUG_LOG

extern clock_t start_time;


// ��ʼ��ʱ��
#define LogTimeInit() \
{ \
	 start_time = clock(); \
}

// ��¼��ʼʱ�䵽��ʱ֮�����õ�ʱ��
#define LogTimeRecord() \
{ \
	clock_t end_time = clock(); \
	double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC; \
	int minutes = (int)(elapsed_time / 60); \
	int seconds = (int)(elapsed_time) % 60; \
	int milliseconds = (int)((elapsed_time - seconds - minutes * 60) * 1000); \
	LogOutput("����ʱ��: %d �� %d �� %d ����, �ļ���:[%s] ������:[%s] ��:[%d]", minutes, seconds, milliseconds, __FILE__, __FUNCTION__, __LINE__); \
}

// �������һ
void Log(int nLogType, PCHAR lpParameter, ...);
// ���dbgview
void LogOutput(PCHAR lpParameter, ...);
// �������̨
void LogConsole(PCHAR lpParameter, ...);
// �����Ϣ��
void LogMsg(PCHAR lpParameter, ...);
// ����ļ�
void LogFile(PCHAR lpParameter, ...);
// �����ϸ�Ĵ���λ��
#define LogErrorLine() LogOutput("ErrorCode: [%08X] FunName: [%s] Line: [%d] File: [%s] Data: [%s] Data: [%s]", GetLastError(), __FUNCTION__, __LINE__, __FILE__, __DATE__, __TIME__);
#define LogOutputTip(x) LogOutput("%s --> FunName: [%s] Line: [%d] File: [%s]", x, __FUNCTION__, __LINE__, __FILE__);

#else
// ��ʼ��ʱ��
#define LogTimeInit()
// ��¼��ʼʱ�䵽��ʱ֮�����õ�ʱ��
#define LogTimeRecord()

#define Log(x)
#define LogOutput(x)
#define LogConsole(x)
#define LogMsg(x)
#define LogFile(x)
#define LogErrorLine()
#define LogOutputTip(x)
#endif