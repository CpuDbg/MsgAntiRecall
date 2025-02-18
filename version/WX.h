// WX.h: interface for the CWX class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WX_H__FD62F341_4F6E_4D2E_AA5D_0DE4E6B241AE__INCLUDED_)
#define AFX_WX_H__FD62F341_4F6E_4D2E_AA5D_0DE4E6B241AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include <stdio.h>
#include <direct.h>
#include <MMSYSTEM.H>
#include "public.h"

#include "WXToolsDlg.h"
#include "WXSettingsDlg.h"
#include "WXAboutMeDlg.h"
#include "WXPublicData.h"


#pragma comment(lib, "winmm")


// ������Ϣ
// ��������ʾ������Ϣ
#define HOTKEY_WXDIALOG_DISPLAY 0x1111



// ����������Ϣ������ʾ
/*
6D5774C0    8D8D 38FFFFFF   lea ecx,dword ptr ss:[ebp-0xC8]
6D5774C6    C645 FC 01      mov byte ptr ss:[ebp-0x4],0x1
6D5774CA    8BF0            mov esi,eax
6D5774CC    E8 1FE20000     call WeChatWi.6D5856F0
6D5774D1    8B83 3C050000   mov eax,dword ptr ds:[ebx+0x53C]
6D5774D7    8985 60FFFFFF   mov dword ptr ss:[ebp-0xA0],eax
6D5774DD    8B48 3C         mov ecx,dword ptr ds:[eax+0x3C]          ; SogouPY.10329577
6D5774E0    83C1 3C         add ecx,0x3C
6D5774E3    3BCE            cmp ecx,esi                              ; ����������Ϣ������ʾ
6D5774E5    0F87 A1000000   ja WeChatWi.6D57758C
6D5774EB    E8 209AFBFF     call WeChatWi.6D530F10
6D5774F0    FFB5 60FFFFFF   push dword ptr ss:[ebp-0xA0]
6D5774F6    E8 55C21800     call WeChatWi.6D703750
6D5774FB    8D8D 38FFFFFF   lea ecx,dword ptr ss:[ebp-0xC8]
6D577501    E8 AA893700     call WeChatWi.6D8EFEB0
6D577506    68 BA000000     push 0xBA
6D57750B    8D8D 38FFFFFF   lea ecx,dword ptr ss:[ebp-0xC8]
6D577511    C645 FC 11      mov byte ptr ss:[ebp-0x4],0x11
6D577515    E8 36973700     call WeChatWi.6D8F0C50
6D57751A    6A 22           push 0x22
8B833C050000898560FFFFFF8B483C83C13C3BCE
*/


#define MsgBox(hHwnd, content, title, MsgType) {HWND hwnd = GetFocus();MessageBox(hHwnd, content, title, MsgType | MB_SYSTEMMODAL);SetFocus(hwnd);} 


#define WX_CLASSNAME		"WeChatMainWndForPC"
// �ʹ��ڶ����ľ���
#define WX_POSITION_TOP		420
// ÿһ��ͼ��ļ��
#define WX_POSITION_SPACE	40
// �ʹ�����ߵľ���
#define WX_POSITION_LEFT	12



#define QQ_DLL_NAME				"im.dll"
#define QQ_EXE_NAME				"qq.exe"
// [3.5.0.46]
#define WX_EXE_NAME				"WeChat.exe"
#define WX_DLL_NAME				"WeChatWin.dll"

#define WX_PATH_SETTINGS		"Wx_Settings.ini"
#define WX_PATH_VOICE			"Voice"

/*
// 2.6.2.27
#define WX_WIN_DLL_TIMEDATASTAMP 0x5a9520ea
#define WX_WIN_DLL_SIZE			 18778272

// 6D5774E5-6D4E0000=974E5
#define WX_RECOVER_OFFSET			0x974E5
#define WX_RECOVER_OFFSET_SIZE		0x2
*/


/*
v2.6.3.78
68C8B452-684A0000=7EB452
*/
#define WX_RECOVER_OFFSET		0x7EB452
#define WX_RECOVER_OFFSET_SIZE  0x1


#define QQ_RECOVER_OFFSET_SIZE  0x2

//#define WX_WIN_DLL_TIMEDATASTAMP 0x025E48D2
//#define WX_WIN_DLL_SIZE			 19271328     // 2.6.3.78

// v3.5.0.46
// #define WX_WIN_DLL_TIMEDATASTAMP 0x61EE2646
// #define WX_WIN_DLL_SIZE			 39707592

// v3.6.0.18
// #define WX_WIN_DLL_TIMEDATASTAMP 0x6231AF99
// #define WX_WIN_DLL_SIZE			 40088008

// v3.7.6.44
#define WX_WIN_DLL_TIMEDATASTAMP 0x63104E25
#define WX_WIN_DLL_SIZE			 43260808

//#define WX_MAIL_TIP "΢��С��� v1.0.1  ֧��΢�Ű汾v2.6.3.78  code by ���˃�"
//#define WX_MAIL_TIP "΢��С��� v2.0.3  ֧��΢�Ű汾v3.5.0.46  code by ���˃�"
//#define WX_MAIL_TIP "΢��С��� v2.0.4  ֧��΢�Ű汾v3.6.0.18  code by ���˃�"
//#define WX_MAIL_TIP "΢��С��� v2.0.6  ֧��΢�Ű汾v3.7.6.44  code by ���˃�"
//#define WX_MAIL_TIP "΢��С��� v2.0.8  ֧��΢�Ű汾v3.9.0.28  code by ���˃�"
//#define WX_MAIL_TIP "΢��С��� v2.0.9  ֧��΢�Ű汾v3.9.2.23  code by ���˃�"
//#define WX_MAIL_TIP "΢��С��� v2.1.0  ֧��΢�Ű汾v3.9.5.81  code by ���˃�"
//#define WX_MAIL_TIP "΢��С��� v2.1.1  ֧��΢�Ű汾v3.9.7.25  code by ���˃�"
//#define WX_MAIL_TIP "΢��С��� v2.1.2  ֧��΢�Ű汾v3.9.9.43  code by ���˃�"
//#define WX_MAIL_TIP "΢��С��� v2.1.3  ֧��΢�Ű汾v3.9.10.19  code by ���˃�"
//#define WX_MAIL_TIP "΢��С��� v2.1.4  ֧��΢�Ű汾v3.9.10.27  code by ���˃�"
#define WX_MAIL_TIP "΢��С��� v2.1.5  ֧��΢�Ű汾v3.9.12.17  code by ���˃�"

#define HOOK_JUMP(dst,src) (((DWORD)dst)-((DWORD)src)-5)


#define WX_PATH_PLUGIN "version.dll"


typedef struct
{
	// ҪHOOK�ĵ�ַ
	PVOID m_lpHookAddress;
	// HOOK���ݵĴ�С
	DWORD m_dwHookSize;
	// ҪHOOK������
	BYTE m_szHookData[20];
	// �ָ�������
	BYTE m_szHookRecoverData[20];
}TAG_MEMWRITE, *LPTAG_MEMWRITE;

typedef enum
{
	// ΢�ŷ�����
	ENUM_REVOKE_WX_ADDRESS = 0,
	// QQ������
	ENUM_REVOKE_QQ_ADDRESS,
	// ��������ʧЧ
	ENUM_PC_ON_ADDRESS,
	// �࿪
	ENUM_CLINET_OPEN_ADDRESS,
	// ������ʾ����
	ENUM_REVOKE_DATED_ADDRESS,
	// �����Ǳ��˵���Ϣ ������  ���˾Ͳ��ܱ༭
	ENUM_REVOKE_OTHER_MSG_ADDRESS,
	// �����ж����ı���Ϣ�����б༭  1���ı���Ϣ  ������
	ENUM_REVOKE_MSG_TYPE_ADDRESS,
	// ���CALL�����ַ��� �㳷����һ����Ϣ  [ebp+0x10]ָ���������û���
	ENUM_REVOKE_TRUE_NAME_ADDRESS,
	// �����޸� ���±༭ [esp+4]ָ�����±༭
	ENUM_REVOKE_REEDIT_ADDRESS,
}ENUM_HOOK;


/*
ver 2.6.0.40
0x0001   �ı���Ϣ
0x0003   ͼƬ
0x0031   �����ļ�
0x0022   ����
0x0047   �������
0x0049   ���ںŷ�����������/����
0x2710   ���
0x2712   ������Ϣ
*/
typedef enum
{
	ENUM_WX_TYPE_TEXT = 0x1,
	ENUM_WX_TYPE_IMG = 0x3,
	ENUM_WX_TYPE_VOICE = 0x22,
	ENUM_WX_TYPE_VIDEO = 0x2b,
	ENUM_WX_TYPE_EMOJI = 0x2f,
	ENUM_WX_TYPE_FILE = 0x31,
	ENUM_WX_TYPE_NEW = 0x49,
	ENUM_WX_TYPE_MONEY = 0x2710,
	ENUM_WX_TYPE_REVOKE = 0x2712
}ENUM_WX_TYPE;


class CWX  
{
public:
	// �µ�΢��Ƥ��
	CWXToolsDlg m_Tools;
	CWXSettingsDlg m_Settings;
	CWXAboutDlg m_About;
	CWXPublicData m_PublicData;


//	CHAR m_szCurrentPath[MAX_PATH];
//	CHAR m_szPluginPath[MAX_PATH];
	CHAR m_szSqlPath[MAX_PATH];
	CHAR m_szSqlTempPath[MAX_PATH];
	CHAR m_szWxSettingsPath[MAX_PATH];
	CHAR m_szWxVoicePath[MAX_PATH];

	
	// ΢�Ų��Ŀ¼
 	CHAR m_szWxPluginDirectory[MAX_PATH];
 	// ΢��������Ŀ¼
	CHAR m_szWxDirectory[MAX_PATH];
 	// ΢�Ų��INI�ļ�·��
 	CHAR m_szWxPluginINIFilePath[MAX_PATH];
 	// ΢��WINDLL�ļ�·��
 	CHAR m_szWXWinDllFilePath[MAXBYTE];
 	// ΢���������ļ�·��
 	CHAR m_szWXExeFilePath[MAXBYTE];

	// ������ʾ
	CHAR m_szWXVoiceRedPacketsPath[MAX_PATH];
	CHAR m_szWXVoiceWelcome1[MAX_PATH];
	CHAR m_szWXVoiceWelcome2[MAX_PATH];
	CHAR m_szWXVoiceRevoke[MAX_PATH];

	BOOL PlayMP3(PCHAR lpPath);
	BOOL ReadSettings();
	BOOL SaveSettings();
	void InitData(HWND hWnd);
	HWND m_hWnd;
	// ��¼��һЩ��Ϣ
	BOOL m_bLogSqlSave;
	// ��ʾ��¼
	BOOL m_bLogDisplay;
	// ���dbgview
	BOOL m_bLogDBG;
	// ��¼�ı�
	BOOL m_bLogText;
	// ��¼ͼƬ
	BOOL m_bLogImg;
	// ��¼С��Ƶ
	BOOL m_bLogVideo;
	// ��¼�ļ�
	BOOL m_bLogFile;
	// ��¼����
	BOOL m_bLogVoice;
	// ��¼�������
	BOOL m_bLogEmoji;
	// ��¼���ں�������/����
	BOOL m_bLogNew;
	// ��¼���
	BOOL m_bLogMoney;
	// ��¼������Ϣ
	BOOL m_bLogRevoke;

	// �������
	BOOL m_bNormalMoneyTip;
	// ��������
	BOOL m_bNormalVoiceTip;
	// ��Ϣ����
	BOOL m_bNormalRevoke;
	// ΢�Ŷ࿪
	BOOL m_bNormalClientOpen;

	// �ȼ���
	DWORD m_dwHotKey1;
	// �ȼ���
	DWORD m_dwHotKey2;


	HINSTANCE m_hInstance;

	TAG_MEMWRITE m_tagWrite[100];

	BOOL MemWrite(PVOID lpAddress, DWORD dwHookSize, PVOID lpHookValue, PVOID lpBackupHookValue);
	BOOL MemRecover(PVOID lpAddress, DWORD dwHookSize, PVOID lpBackupHookValue);

	
	BOOL MemHook(PVOID lpAddress, DWORD dwHookSize, PVOID lpHookValue, PVOID lpBackupHookValue);

	BOOL GetHookData(DWORD dwModule);
	void CenterWindowOnScreen(HWND hWnd, int nWindows = 0);


	BOOL CWX::GetModuleName(PCHAR lpCmp, PCHAR lpOut);
	char* CWX::stristr(const char * str1, const char * str2);

	CWX();
	virtual ~CWX();

};

#endif // !defined(AFX_WX_H__FD62F341_4F6E_4D2E_AA5D_0DE4E6B241AE__INCLUDED_)
