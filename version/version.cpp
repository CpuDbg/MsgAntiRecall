////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////					    Dllģ��ٳ�Դ�������� -- CpuDbg x96   												////
////																											////
////						     www.OllyDbg.com																////
////																											////
////						  ����ʧЧ��������ע����޸�														////
////																											////
////		 1. xp�� HKEY_LOCAL_MACHINE\System\CurrentControlSet\Control\SessionManager\knowndlls     			////
////	 2. win7�� HKEY_LOCAL_MACHINE\System\CurrentControlSet\Control\SessionManager\ExcludeFromKnownDlls      ////
////																											////
////						   2022.02.22 22:22:22																////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ͷ�ļ�
#include "stdafx.h"

// Winͷ�ļ�
#include <Windows.h>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "WX.h"
#include "res/resource.h"
#include <stdio.h>
#include <Shellapi.h>

#pragma comment(lib, "shell32")

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ������Ӧ�Ŀ��ļ�
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
			_Log0("�����ƶ�");
			break;
		}

	case WM_SHOWWINDOW:
		{
			if(wParam == TRUE)
			{
				_Log0("��ʾ����");
			}
			else
			{
				_Log0("���ش���");
			}

			break;
		}

	case WM_KILLFOCUS:
		{
			_Log0("ʧȥ����");
			break;
		}

	case WM_SETFOCUS:
		{
			_Log0("��ȡ����");
			break;
		}
	}

	return CallWindowProc(wpOrigWXProc, hwnd, uMsg, 
		wParam, lParam); 
} 

// ����
void WINAPI ThreadProc1(PVOID lpParameter)
{
	DialogBox((HINSTANCE)lpParameter, MAKEINTRESOURCE(DIALOG_TOOLS), NULL, (DLGPROC)ToolsProc);
}

// ����
void WINAPI ThreadProc2(PVOID lpParameter)
{
	DialogBox((HINSTANCE)lpParameter, MAKEINTRESOURCE(DIALOG_SETTINGS), NULL, (DLGPROC)SettingsProc);
}

// ���ڴ���
void WINAPI ThreadProc3(PVOID lpParameter)
{
	DialogBox((HINSTANCE)lpParameter, MAKEINTRESOURCE(DIALOG_ABOUT), NULL, (DLGPROC)AboutProc);
}



void WINAPI CheckProcessProc(PVOID lpParameter)
{
	DWORD dwWho = 0;
	HMODULE hModule = NULL;


	// �ж��ǲ���qq
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
		_Log0("����΢�� Ҳ����QQ ɶҲ����  ֱ���˳�.");
		return;
	}


	while(TRUE)
	{
		if(hModule = GetModuleHandle(QQ_DLL_NAME))
		{
			_Log0("���� qq.exe  im.dll.");
			dwWho = 1;
		}
		// �ж��ǲ���΢��
		else if(hModule = GetModuleHandle(WX_DLL_NAME))
		{
			_Log0("���� ΢��  WeChatWin.dll ��ʼ��������");
			dwWho = 2;
		}
		else
		{
			_Log0("δ���� ΢�� �� qq �ӳ�3��!");
			Sleep(3000);
			continue;
		}

		// ��ʼ��HOOK����
		while(!wx.GetHookData((DWORD)hModule))
		{
			_Log0("δ�ҵ�, ��������!");

			static int nFound = 0;
			nFound++;

			if(nFound >= 3)
			{
				_Log0("δ�ҵ�΢��! �˳�����!");
				PostMessage(wx.m_hWnd, WM_CLOSE, 0, 0);
			}

			Sleep(1000);
			continue;
		}

		if(dwWho == 1)
		{
			_Log0("��QQ��ֱ���˳���,����Ҫ��������");
			break;
		}


		DWORD dwPID = 0;
		_Log2("���໯ ׼������΢�Ŵ���  Ŀ��PID: %d �����������ھ��: %08X", dwPID, wx.m_PublicData.m_WxMailHwnd);
		while((wx.m_PublicData.m_WxMailHwnd == NULL))
		{
			wx.m_PublicData.m_WxMailHwnd = ::FindWindow(WX_CLASSNAME, NULL);

			GetWindowThreadProcessId(wx.m_PublicData.m_WxMailHwnd, &dwPID);

			_Log2("���໯ δ�ҵ�΢�Ŵ���  Ŀ��PID: %d ΢�Ŵ��ھ��: %08X", dwPID, wx.m_PublicData.m_WxMailHwnd);

			Sleep(5000);
		}

		_Log2("�Ѿ��ҵ�΢�Ŵ���  ΢�Ŵ��ھ��: %08X ������PID: %d", wx.m_PublicData.m_WxMailHwnd, GetCurrentProcessId());


		wx.m_Tools.GetSafeData(&wx.m_PublicData);
		wx.m_Settings.GetSafeData(&wx.m_PublicData);
		wx.m_About.GetSafeData(&wx.m_PublicData);

		// ����΢���ϵ�����С����
		HANDLE h1 = NULL, h2 = NULL, h3 = NULL;
		_Log1("׼������΢�� 1 ʵ�����: %016I64X", wx.m_hInstance);
		h1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc1, wx.m_hInstance, 0, NULL);
		h2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc2, wx.m_hInstance, 0, NULL);
		h3 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc3, wx.m_hInstance, 0, NULL);

		// ���໯һ��
		wpOrigWXProc = (WNDPROC)GetWindowLongPtr(wx.m_PublicData.m_WxMailHwnd, GWLP_WNDPROC);
		SetWindowLong(wx.m_PublicData.m_WxMailHwnd, GWLP_WNDPROC, (LONG)WXSubclassProc);

		WaitForSingleObject(h1, INFINITE);
		WaitForSingleObject(h2, INFINITE);
		WaitForSingleObject(h3, INFINITE);

		dwPID = 0;
		h1 = NULL, h2 = NULL, h3 = NULL;

		_Log0("�����̶߳��Ѿ���ȫ����!!!  ��Ϣ5��");
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

			// ��ʼ��HOOK����
			//		while(!wx.GetHookData())
			{
				// 				_Log0("�˳���!");
				// 				PostMessage(wx.m_hWnd, WM_CLOSE, 0, 0);

				_Log0("δ�ҵ�, ��������!");

				static int nFound = 0;
				nFound++;

				if(nFound >= 3)
				{
					_Log0("δ�ҵ�΢��! �˳�����!");
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

			_Log2("���໯ δ�ҵ�΢�Ŵ���  Ŀ��PID: %d ������PID: %d", dwPID, dwCurrentPID);

			Sleep(5000);
		}

		_Log2("�Ѿ��ҵ�΢�Ŵ���  Ŀ��PID: %d ������PID: %d", dwPID, GetCurrentProcessId());


		wx.m_Tools.GetSafeData(&wx.m_PublicData);
		wx.m_Settings.GetSafeData(&wx.m_PublicData);
		wx.m_About.GetSafeData(&wx.m_PublicData);

		// ����΢���ϵ�����С����
		h1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc1, wx.m_hInstance, 0, NULL);
		h2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc2, wx.m_hInstance, 0, NULL);
		h3 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc3, wx.m_hInstance, 0, NULL);

		// ���໯һ��
		wpOrigWXProc = (WNDPROC)GetWindowLongPtr(wx.m_PublicData.m_WxMailHwnd, GWLP_WNDPROC);
		SetWindowLong(wx.m_PublicData.m_WxMailHwnd, GWLP_WNDPROC, (LONG)WXSubclassProc);

		WaitForSingleObject(h1, INFINITE);
		WaitForSingleObject(h2, INFINITE);
		WaitForSingleObject(h3, INFINITE);

		dwPID = 0;
		dwCurrentPID = 0;
		h1 = NULL, h2 = NULL, h3 = NULL;

		_Log0("�����̶߳��Ѿ���ȫ����!!!  ��Ϣ5��");
		Sleep(5000);
	}
}






////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������
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
// HiJackDll �����ռ�
namespace HiJackDll
{
	HMODULE m_hModule = NULL;	// ԭʼģ����
	DWORD m_dwReturn[17] = {0};	// ԭʼ�������ص�ַ


	// ����ԭʼģ��
	inline BOOL WINAPI Load()
	{
		TCHAR tzPath[MAX_PATH];
		TCHAR tzTemp[MAX_PATH * 2];

		GetSystemDirectory(tzPath, MAX_PATH);
		lstrcat(tzPath, TEXT("\\version.dll"));
		m_hModule = LoadLibrary(tzPath);
		if (m_hModule == NULL)
		{
			wsprintf(tzTemp, TEXT("�޷����� %s�������޷��������С�"), tzPath);
			MessageBox(NULL, tzTemp, TEXT("HiJackDll"), MB_ICONSTOP);
		}

		return (m_hModule != NULL);	
	}
		
	// �ͷ�ԭʼģ��
	inline VOID WINAPI Free()
	{
		if (m_hModule)
		{
			FreeLibrary(m_hModule);
		}
	}

	// ��ȡԭʼ������ַ1
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

			wsprintf(tzTemp, TEXT("�޷��ҵ����� %hs�������޷��������С�"), pszProcName);
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
// ��ں���
BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, PVOID pvReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hModule);


		OutputDebugString("dll inject success by ������");

		// �����߳�
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

