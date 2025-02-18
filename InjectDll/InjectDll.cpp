// InjectDll.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include "DebugLog.h"



HANDLE NtCreateThreadEx(
						HANDLE hProcess,
						LPVOID lpBaseAddress,
						LPVOID lpSpace
						)
{
	//The prototype NtCreateThreadEx from undocumented.ntinternals.com
    typedef DWORD (WINAPI * functypeNtCreateThreadEx)(
        PHANDLE                 ThreadHandle,
        ACCESS_MASK             DesiredAccess,
        LPVOID                  ObjectAttributes,
        HANDLE                  ProcessHandle,
        LPTHREAD_START_ROUTINE  lpStartAddress,
        LPVOID                  lpParameter,
        BOOL                    CreateSuspended,
        DWORD                   dwStackSize,
        DWORD                   Unknown1,
        DWORD                   Unknown2,
        LPVOID                  Unknown3
		);
	
    HANDLE                      hRemoteThread           = NULL;
    HMODULE                     hNtDllModule            = NULL;
    functypeNtCreateThreadEx    funcNtCreateThreadEx    = NULL;
	
	
	//Get handle for ntdll which contains NtCreateThreadEx
    hNtDllModule = GetModuleHandle( "ntdll.dll" );
    if ( hNtDllModule == NULL )
    {
		return NULL;
    }
	
    funcNtCreateThreadEx = (functypeNtCreateThreadEx)GetProcAddress( hNtDllModule, "NtCreateThreadEx" );
    if ( !funcNtCreateThreadEx )
    {
		return NULL;
    }
	
    funcNtCreateThreadEx( &hRemoteThread,  GENERIC_ALL, NULL, hProcess, (LPTHREAD_START_ROUTINE)lpBaseAddress, lpSpace, FALSE, NULL, NULL, NULL, NULL );
	
    return hRemoteThread;
}

bool IsWindows7(void)   
{  
    OSVERSIONINFO osVersion = {0};  
    osVersion.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);  
	
    ::GetVersionEx(&osVersion);  
	
    //greater than windows 7  
    if( osVersion.dwMajorVersion > 6)  
        return true;  
    if( osVersion.dwMajorVersion == 6 && osVersion.dwMinorVersion >= 1 )  
        return true;  
	
    //equal to windows 7  
    if (osVersion.dwMajorVersion == 6 && osVersion.dwMinorVersion == 1)  
        return false;  
	
    return false;  
}



//设置调试权限
void SetDebugPrivileges()
{
    TOKEN_PRIVILEGES tp;
    HANDLE hToken;
    HANDLE hCurProcess = ::GetCurrentProcess();
    if (::OpenProcessToken(hCurProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
    {
        ::LookupPrivilegeValue(0, "SeDebugPrivilege", &tp.Privileges[0].Luid);
        tp.PrivilegeCount = 1;
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
        ::AdjustTokenPrivileges(hToken, false, &tp, 0, 0, 0);
    }
}


#define WX_WG_TITLE "微信小程序 v0.1.3  code by 半斤八兩"
#define WX_DLL "version.dll"
#define WX_EXE "WeChat.exe"

#include <stdlib.h>

int main(int argc, char* argv[])
{
	system("color 0a & title 2025 新年快乐~``");
	printf("Hello World!\n");
	
	// TODO: Place code here.
	SetDebugPrivileges();
	
	// 获取插件的DLL绝对路径, 并加载
	char szDll[MAXBYTE] = {0};
	GetCurrentDirectory(MAX_PATH, szDll);
	strcat(szDll, "\\");
	strcat(szDll, WX_DLL);
	
	char szEXE[MAXBYTE] = {0};
	GetCurrentDirectory(MAX_PATH, szEXE);
	strcat(szEXE, "\\");
	strcat(szEXE, WX_EXE);


	
	STARTUPINFO si = {0};
	PROCESS_INFORMATION pi = {0};
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;
	GetStartupInfo(&si);

	
	char szPath[MAX_PATH] = {0};
	GetCurrentDirectory(MAX_PATH, szPath);
	if(!CreateProcess(NULL, szEXE, NULL, NULL, FALSE, 0, NULL, szPath, &si, &pi))
	{
		MessageBox(NULL, "未找到微信主程序, 请将解压出来连同目录一起拷贝到微信目录下", WX_WG_TITLE, MB_ICONERROR);
		return -1;
	}
	
	Sleep(5*1000);
//	system("pause");
	
	
	LPVOID Param = VirtualAllocEx(pi.hProcess, NULL, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	printf("地址是: [%08X_%08X]", (DWORD64)Param>>32, Param);
	WriteProcessMemory(pi.hProcess, Param, (LPVOID)szDll, strlen(szDll)+sizeof(TCHAR), NULL);

	Sleep(1*1000);
	
	HANDLE hThread = NULL;
	DWORD hDll = 0;
	if(!IsWindows7())
	{
		LogOutput("Win7");
		//		APCInject(pi.dwProcessId, (DWORD)LoadLibrary, (DWORD)Param);
		hThread = NtCreateThreadEx(pi.hProcess, LoadLibrary, Param);
		
		WaitForSingleObject(hThread, INFINITE);
		GetExitCodeThread(hThread, &hDll);
		
//		SuspendThread(pi.hThread);
		Sleep(500);
		
		if(hDll == NULL)
		{
			MessageBox(NULL, "win7 创建线程发现致命错误!", WX_WG_TITLE, MB_ICONERROR);
		}
		
// 		char szTemp[MAXBYTE] = {0};
// 		sprintf(szTemp, "%08X   %08X   %08X", hDll, dwFunOffset, (DWORD)hDll);
// 		LogOutput(szTemp);
// 		
// 		
// 		// 		ResumeThread(pi.hThread);
// 		// 		Sleep(5*1000);
// 		
// 		HelloKitty = (pfnHelloKitty)((DWORD)hDll + dwFunOffset);
// 		hThread = NtCreateThreadEx(pi.hProcess, HelloKitty, NULL);
// 		//		APCInject(pi.dwProcessId, (DWORD)HelloKitty, (DWORD)Param);
	}
	else
	{
		LogOutput("not Win7");
		hThread = CreateRemoteThread(pi.hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibrary, Param, 0, NULL);
		
		WaitForSingleObject(hThread, INFINITE);
		GetExitCodeThread(hThread, &hDll);
		
		if(hDll == NULL)
		{
			MessageBox(NULL, "other 创建线程发现致命错误!", WX_WG_TITLE, MB_ICONERROR);
		}
		
// 		char szTemp[MAXBYTE] = {0};
// 		sprintf(szTemp, "%08X   %08X   %08X", hDll, dwFunOffset, (DWORD)hDll);
// 		LogOutput(szTemp);
// 		
// 		HelloKitty = (pfnHelloKitty)((DWORD)hDll + dwFunOffset);
// 		hThread = CreateRemoteThread(pi.hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)HelloKitty, NULL, 0, NULL);
	}
	
	ResumeThread(pi.hThread);
	puts("bye~");
	Sleep(3000);


	
	return 0;
}

