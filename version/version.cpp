////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////					    Dll模块劫持源码生成器 -- CpuDbg x96   												////
////																											////
////						     www.OllyDbg.com																////
////																											////
////						  如若失效尝试如下注册表修改														////
////																											////
////		 1. xp下 HKEY_LOCAL_MACHINE\System\CurrentControlSet\Control\SessionManager\knowndlls     			////
////	 2. win7下 HKEY_LOCAL_MACHINE\System\CurrentControlSet\Control\SessionManager\ExcludeFromKnownDlls      ////
////																											////
////						   2022.02.22 22:22:22																////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 公共头文件
#include "stdafx.h"

// Win头文件
#include <Windows.h>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "WX.h"
#include "res/resource.h"
#include <stdio.h>
#include <Shellapi.h>

#pragma comment(lib, "shell32")

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 加载相应的库文件
#pragma comment(lib, "user32")
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




CWX wx;

WNDPROC wpOrigWXProc; 

// Subclass procedure 
LRESULT APIENTRY WXSubclassProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_MOVE:
		{
			_Log0("窗口移动");
			break;
		}

	case WM_SHOWWINDOW:
		{
			if(wParam == TRUE)
			{
				_Log0("显示窗口");
			}
			else
			{
				_Log0("隐藏窗口");
			}

			break;
		}

	case WM_KILLFOCUS:
		{
			_Log0("失去焦点");
			break;
		}

	case WM_SETFOCUS:
		{
			_Log0("获取焦点");
			break;
		}
	}

	return CallWindowProc(wpOrigWXProc, hwnd, uMsg, 
		wParam, lParam); 
} 

// 功能
void WINAPI ThreadProc1(PVOID lpParameter)
{
	DialogBox((HINSTANCE)lpParameter, MAKEINTRESOURCE(DIALOG_TOOLS), NULL, (DLGPROC)ToolsProc);
}

// 设置
void WINAPI ThreadProc2(PVOID lpParameter)
{
	DialogBox((HINSTANCE)lpParameter, MAKEINTRESOURCE(DIALOG_SETTINGS), NULL, (DLGPROC)SettingsProc);
}

// 关于窗口
void WINAPI ThreadProc3(PVOID lpParameter)
{
	DialogBox((HINSTANCE)lpParameter, MAKEINTRESOURCE(DIALOG_ABOUT), NULL, (DLGPROC)AboutProc);
}



void WINAPI CheckProcessProc(PVOID lpParameter)
{
	DWORD dwWho = 0;
	HMODULE hModule = NULL;


	// 判断是不是qq
	if(GetModuleHandle(QQ_EXE_NAME))
	{
		int a = 0;
	}
	else if(GetModuleHandle(WX_EXE_NAME))
	{
		int a = 0;
	}
	else
	{
		_Log0("不是微信 也不是QQ 啥也不是  直接退出.");
		return;
	}


	while(TRUE)
	{
		if(hModule = GetModuleHandle(QQ_DLL_NAME))
		{
			_Log0("发现 qq.exe  im.dll.");
			dwWho = 1;
		}
		// 判断是不是微信
		else if(hModule = GetModuleHandle(WX_DLL_NAME))
		{
			_Log0("发现 微信  WeChatWin.dll 开始吸附窗口");
			dwWho = 2;
		}
		else
		{
			_Log0("未发现 微信 和 qq 延迟3秒!");
			Sleep(3000);
			continue;
		}

		// 初始化HOOK数据
		while(!wx.GetHookData((DWORD)hModule))
		{
			_Log0("未找到, 继续查找!");

			static int nFound = 0;
			nFound++;

			if(nFound >= 3)
			{
				_Log0("未找到微信! 退出程序!");
				PostMessage(wx.m_hWnd, WM_CLOSE, 0, 0);
			}

			Sleep(1000);
			continue;
		}

		if(dwWho == 1)
		{
			_Log0("是QQ就直接退出了,不需要吸附窗口");
			break;
		}


		DWORD dwPID = 0;
		_Log2("子类化 准备查找微信窗口  目标PID: %d 本程序主窗口句柄: %08X", dwPID, wx.m_PublicData.m_WxMailHwnd);
		while((wx.m_PublicData.m_WxMailHwnd == NULL))
		{
			wx.m_PublicData.m_WxMailHwnd = ::FindWindow(WX_CLASSNAME, NULL);

			GetWindowThreadProcessId(wx.m_PublicData.m_WxMailHwnd, &dwPID);

			_Log2("子类化 未找到微信窗口  目标PID: %d 微信窗口句柄: %08X", dwPID, wx.m_PublicData.m_WxMailHwnd);

			Sleep(5000);
		}

		_Log2("已经找到微信窗口  微信窗口句柄: %08X 本程序PID: %d", wx.m_PublicData.m_WxMailHwnd, GetCurrentProcessId());


		wx.m_Tools.GetSafeData(&wx.m_PublicData);
		wx.m_Settings.GetSafeData(&wx.m_PublicData);
		wx.m_About.GetSafeData(&wx.m_PublicData);

		// 吸附微信上的三个小窗口
		HANDLE h1 = NULL, h2 = NULL, h3 = NULL;
		_Log1("准备吸附微信 1 实例句柄: %016I64X", wx.m_hInstance);
		h1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc1, wx.m_hInstance, 0, NULL);
		h2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc2, wx.m_hInstance, 0, NULL);
		h3 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc3, wx.m_hInstance, 0, NULL);

		// 子类化一下
		wpOrigWXProc = (WNDPROC)GetWindowLongPtr(wx.m_PublicData.m_WxMailHwnd, GWLP_WNDPROC);
		SetWindowLong(wx.m_PublicData.m_WxMailHwnd, GWLP_WNDPROC, (LONG)WXSubclassProc);

		WaitForSingleObject(h1, INFINITE);
		WaitForSingleObject(h2, INFINITE);
		WaitForSingleObject(h3, INFINITE);

		dwPID = 0;
		h1 = NULL, h2 = NULL, h3 = NULL;

		_Log0("三个线程都已经安全结束!!!  休息5秒");
		Sleep(5000);
	}
}


void WINAPI CheckProc(PVOID lpParameter)
{
	DWORD dwPID = 0;
	DWORD dwCurrentPID = 0;
	HANDLE h1 = NULL, h2 = NULL, h3 = NULL;

	while(TRUE)
	{
		dwCurrentPID = GetCurrentProcessId();

		static int xxx = 1;

		if(xxx)
		{
			xxx = 0;

			// 初始化HOOK数据
			//		while(!wx.GetHookData())
			{
				// 				_Log0("退出了!");
				// 				PostMessage(wx.m_hWnd, WM_CLOSE, 0, 0);

				_Log0("未找到, 继续查找!");

				static int nFound = 0;
				nFound++;

				if(nFound >= 3)
				{
					_Log0("未找到微信! 退出程序!");
					PostMessage(wx.m_hWnd, WM_CLOSE, 0, 0);
				}

				Sleep(1000);
				continue;
			}
		}

		while(/*!wx.m_PublicData.m_WxMailHwnd && */!(dwPID == dwCurrentPID))
		{
			wx.m_PublicData.m_WxMailHwnd = ::FindWindow(WX_CLASSNAME, NULL);


			GetWindowThreadProcessId(wx.m_PublicData.m_WxMailHwnd, &dwPID);

			_Log2("子类化 未找到微信窗口  目标PID: %d 本程序PID: %d", dwPID, dwCurrentPID);

			Sleep(5000);
		}

		_Log2("已经找到微信窗口  目标PID: %d 本程序PID: %d", dwPID, GetCurrentProcessId());


		wx.m_Tools.GetSafeData(&wx.m_PublicData);
		wx.m_Settings.GetSafeData(&wx.m_PublicData);
		wx.m_About.GetSafeData(&wx.m_PublicData);

		// 吸附微信上的三个小窗口
		h1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc1, wx.m_hInstance, 0, NULL);
		h2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc2, wx.m_hInstance, 0, NULL);
		h3 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc3, wx.m_hInstance, 0, NULL);

		// 子类化一下
		wpOrigWXProc = (WNDPROC)GetWindowLongPtr(wx.m_PublicData.m_WxMailHwnd, GWLP_WNDPROC);
		SetWindowLong(wx.m_PublicData.m_WxMailHwnd, GWLP_WNDPROC, (LONG)WXSubclassProc);

		WaitForSingleObject(h1, INFINITE);
		WaitForSingleObject(h2, INFINITE);
		WaitForSingleObject(h3, INFINITE);

		dwPID = 0;
		dwCurrentPID = 0;
		h1 = NULL, h2 = NULL, h3 = NULL;

		_Log0("三个线程都已经安全结束!!!  休息5秒");
		Sleep(5000);
	}
}






////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
#pragma comment(linker, "/EXPORT:GetFileVersionInfoA=HiJackDll_GetFileVersionInfoA,@1")
#pragma comment(linker, "/EXPORT:GetFileVersionInfoByHandle=HiJackDll_GetFileVersionInfoByHandle,@2")
#pragma comment(linker, "/EXPORT:GetFileVersionInfoExA=HiJackDll_GetFileVersionInfoExA,@3")
#pragma comment(linker, "/EXPORT:GetFileVersionInfoExW=HiJackDll_GetFileVersionInfoExW,@4")
#pragma comment(linker, "/EXPORT:GetFileVersionInfoSizeA=HiJackDll_GetFileVersionInfoSizeA,@5")
#pragma comment(linker, "/EXPORT:GetFileVersionInfoSizeExA=HiJackDll_GetFileVersionInfoSizeExA,@6")
#pragma comment(linker, "/EXPORT:GetFileVersionInfoSizeExW=HiJackDll_GetFileVersionInfoSizeExW,@7")
#pragma comment(linker, "/EXPORT:GetFileVersionInfoSizeW=HiJackDll_GetFileVersionInfoSizeW,@8")
#pragma comment(linker, "/EXPORT:GetFileVersionInfoW=HiJackDll_GetFileVersionInfoW,@9")
#pragma comment(linker, "/EXPORT:VerFindFileA=HiJackDll_VerFindFileA,@10")
#pragma comment(linker, "/EXPORT:VerFindFileW=HiJackDll_VerFindFileW,@11")
#pragma comment(linker, "/EXPORT:VerInstallFileA=HiJackDll_VerInstallFileA,@12")
#pragma comment(linker, "/EXPORT:VerInstallFileW=HiJackDll_VerInstallFileW,@13")
#pragma comment(linker, "/EXPORT:VerLanguageNameA=HiJackDll_VerLanguageNameA,@14")
#pragma comment(linker, "/EXPORT:VerLanguageNameW=HiJackDll_VerLanguageNameW,@15")
#pragma comment(linker, "/EXPORT:VerQueryValueA=HiJackDll_VerQueryValueA,@16")
#pragma comment(linker, "/EXPORT:VerQueryValueW=HiJackDll_VerQueryValueW,@17")


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern "C"
{
	PVOID pfnHiJackDll_GetFileVersionInfoA;
	PVOID pfnHiJackDll_GetFileVersionInfoByHandle;
	PVOID pfnHiJackDll_GetFileVersionInfoExA;
	PVOID pfnHiJackDll_GetFileVersionInfoExW;
	PVOID pfnHiJackDll_GetFileVersionInfoSizeA;
	PVOID pfnHiJackDll_GetFileVersionInfoSizeExA;
	PVOID pfnHiJackDll_GetFileVersionInfoSizeExW;
	PVOID pfnHiJackDll_GetFileVersionInfoSizeW;
	PVOID pfnHiJackDll_GetFileVersionInfoW;
	PVOID pfnHiJackDll_VerFindFileA;
	PVOID pfnHiJackDll_VerFindFileW;
	PVOID pfnHiJackDll_VerInstallFileA;
	PVOID pfnHiJackDll_VerInstallFileW;
	PVOID pfnHiJackDll_VerLanguageNameA;
	PVOID pfnHiJackDll_VerLanguageNameW;
	PVOID pfnHiJackDll_VerQueryValueA;
	PVOID pfnHiJackDll_VerQueryValueW;
}////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// HiJackDll 命名空间
namespace HiJackDll
{
	HMODULE m_hModule = NULL;	// 原始模块句柄
	DWORD m_dwReturn[17] = {0};	// 原始函数返回地址


	// 加载原始模块
	inline BOOL WINAPI Load()
	{
		TCHAR tzPath[MAX_PATH];
		TCHAR tzTemp[MAX_PATH * 2];

		GetSystemDirectory(tzPath, MAX_PATH);
		lstrcat(tzPath, TEXT("\\version.dll"));
		m_hModule = LoadLibrary(tzPath);
		if (m_hModule == NULL)
		{
			wsprintf(tzTemp, TEXT("无法加载 %s，程序无法正常运行。"), tzPath);
			MessageBox(NULL, tzTemp, TEXT("HiJackDll"), MB_ICONSTOP);
		}

		return (m_hModule != NULL);	
	}
		
	// 释放原始模块
	inline VOID WINAPI Free()
	{
		if (m_hModule)
		{
			FreeLibrary(m_hModule);
		}
	}

	// 获取原始函数地址1
	FARPROC WINAPI GetAddress(PCSTR pszProcName)
	{
		FARPROC fpAddress;
		CHAR szProcName[16];
		TCHAR tzTemp[MAX_PATH];

		fpAddress = GetProcAddress(m_hModule, pszProcName);
		if (fpAddress == NULL)
		{
			if (HIWORD(pszProcName) == 0)
			{
				wsprintf(szProcName, "%d", pszProcName);
				pszProcName = szProcName;
			}

			wsprintf(tzTemp, TEXT("无法找到函数 %hs，程序无法正常运行。"), pszProcName);
			MessageBox(NULL, tzTemp, TEXT("HiJackDll"), MB_ICONSTOP);
			ExitProcess(-2);
		}

		return fpAddress;
	}
}
using namespace HiJackDll;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WINAPI InitFunPointer()
{
	pfnHiJackDll_GetFileVersionInfoA = GetAddress("GetFileVersionInfoA");
	pfnHiJackDll_GetFileVersionInfoByHandle = GetAddress("GetFileVersionInfoByHandle");
	pfnHiJackDll_GetFileVersionInfoExA = GetAddress("GetFileVersionInfoExA");
	pfnHiJackDll_GetFileVersionInfoExW = GetAddress("GetFileVersionInfoExW");
	pfnHiJackDll_GetFileVersionInfoSizeA = GetAddress("GetFileVersionInfoSizeA");
	pfnHiJackDll_GetFileVersionInfoSizeExA = GetAddress("GetFileVersionInfoSizeExA");
	pfnHiJackDll_GetFileVersionInfoSizeExW = GetAddress("GetFileVersionInfoSizeExW");
	pfnHiJackDll_GetFileVersionInfoSizeW = GetAddress("GetFileVersionInfoSizeW");
	pfnHiJackDll_GetFileVersionInfoW = GetAddress("GetFileVersionInfoW");
	pfnHiJackDll_VerFindFileA = GetAddress("VerFindFileA");
	pfnHiJackDll_VerFindFileW = GetAddress("VerFindFileW");
	pfnHiJackDll_VerInstallFileA = GetAddress("VerInstallFileA");
	pfnHiJackDll_VerInstallFileW = GetAddress("VerInstallFileW");
	pfnHiJackDll_VerLanguageNameA = GetAddress("VerLanguageNameA");
	pfnHiJackDll_VerLanguageNameW = GetAddress("VerLanguageNameW");
	pfnHiJackDll_VerQueryValueA = GetAddress("VerQueryValueA");
	pfnHiJackDll_VerQueryValueW = GetAddress("VerQueryValueW");
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 入口函数
BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, PVOID pvReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hModule);


		OutputDebugString("dll inject success by 半斤八两");

		// 工作线程
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CheckProcessProc, wx.m_hInstance, 0, NULL);

		wx.m_hInstance = (HINSTANCE)hModule;

		Load();
		InitFunPointer();
		
return true;
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		Free();
	}

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

