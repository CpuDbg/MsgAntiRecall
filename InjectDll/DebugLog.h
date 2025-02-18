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


// 初始化时间
#define LogTimeInit() \
{ \
	 start_time = clock(); \
}

// 记录初始时间到此时之间所用的时长
#define LogTimeRecord() \
{ \
	clock_t end_time = clock(); \
	double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC; \
	int minutes = (int)(elapsed_time / 60); \
	int seconds = (int)(elapsed_time) % 60; \
	int milliseconds = (int)((elapsed_time - seconds - minutes * 60) * 1000); \
	LogOutput("运行时间: %d 分 %d 秒 %d 毫秒, 文件名:[%s] 函数名:[%s] 行:[%d]", minutes, seconds, milliseconds, __FILE__, __FUNCTION__, __LINE__); \
}

// 输出三合一
void Log(int nLogType, PCHAR lpParameter, ...);
// 输出dbgview
void LogOutput(PCHAR lpParameter, ...);
// 输出控制台
void LogConsole(PCHAR lpParameter, ...);
// 输出消息框
void LogMsg(PCHAR lpParameter, ...);
// 输出文件
void LogFile(PCHAR lpParameter, ...);
// 输出详细的错误位置
#define LogErrorLine() LogOutput("ErrorCode: [%08X] FunName: [%s] Line: [%d] File: [%s] Data: [%s] Data: [%s]", GetLastError(), __FUNCTION__, __LINE__, __FILE__, __DATE__, __TIME__);
#define LogOutputTip(x) LogOutput("%s --> FunName: [%s] Line: [%d] File: [%s]", x, __FUNCTION__, __LINE__, __FILE__);

#else
// 初始化时间
#define LogTimeInit()
// 记录初始时间到此时之间所用的时长
#define LogTimeRecord()

#define Log(x)
#define LogOutput(x)
#define LogConsole(x)
#define LogMsg(x)
#define LogFile(x)
#define LogErrorLine()
#define LogOutputTip(x)
#endif