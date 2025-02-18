// WX.cpp: implementation of the CWX class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WX.h"
#include "res/resource.h"


// error C2065: 'GetMappedFileName' : undeclared identifier
#include <Psapi.h>

#pragma comment(lib, "psapi")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BYTE g_szHookJmp[] = {0xe9, 0x90, 0x90, 0x90, 0x90};
BYTE g_szHookCALL[] = {0xe8, 0x90, 0x90, 0x90, 0x90, 0x90};

CWX *lpWX = NULL;




CWX::CWX()
{
	lpWX = this;
	
	GetModuleFileName(GetModuleHandle(WX_PATH_PLUGIN), m_szWxPluginDirectory, MAX_PATH);
	m_szWxPluginDirectory[strlen(m_szWxPluginDirectory)-strlen(WX_PATH_PLUGIN)] = 0;
	
	strcpy(m_szWxSettingsPath, m_szWxPluginDirectory);
	strcpy(m_szSqlPath, m_szWxPluginDirectory);
	strcpy(m_szSqlTempPath, m_szWxPluginDirectory);
	strcpy(m_szWxVoicePath, m_szWxPluginDirectory);
	
	strcat(m_szWxSettingsPath, WX_PATH_SETTINGS);
	// 	strcat(m_szSqlPath, SQL_FILE_NAME);
	// 	strcat(m_szSqlTempPath, SQL_TEMP_FILE);
	strcat(m_szWxVoicePath, WX_PATH_VOICE);
	//	m_Sql.InitSqlInit(m_szSqlPath, m_szSqlTempPath);
	
	
	
	if(access(m_szWxPluginDirectory, 0))
	{
		mkdir(m_szWxPluginDirectory);
	}
	// 	if(access(m_szWxVoicePath, 0))
	// 	{
	// 		mkdir(m_szWxVoicePath);
	// 	}
	
	strcpy(m_szWXVoiceWelcome1, m_szWxVoicePath);
	strcpy(m_szWXVoiceWelcome2, m_szWxVoicePath);
	strcpy(m_szWXVoiceRedPacketsPath, m_szWxVoicePath);
	strcpy(m_szWXVoiceRevoke, m_szWxVoicePath);
	strcat(m_szWXVoiceRevoke, "\\Revoke.mp3");
	strcat(m_szWXVoiceWelcome1, "\\Welcome1.mp3");
	strcat(m_szWXVoiceWelcome2, "\\Welcome2.mp3");
	strcat(m_szWXVoiceRedPacketsPath, "\\RedPackets.mp3");
	
	_Log0(m_szWXVoiceWelcome1);
	_Log0(m_szWXVoiceWelcome2);
	_Log0(m_szWXVoiceRedPacketsPath);
	
	if(access(m_szWxSettingsPath, 0))
	{
		FILE *lpFile = fopen(m_szWxSettingsPath, "wb+");
		fclose(lpFile);
	}
}

CWX::~CWX()
{
	
}

void CWX::CenterWindowOnScreen(HWND hWnd, int nWindows)
{
    RECT    rect;
    int     xLeft,yTop;
	
	if(nWindows == 0)
	{
		GetWindowRect(hWnd, &rect);
		xLeft = (GetSystemMetrics(SM_CXFULLSCREEN)-(rect.right-rect.left))/2;
		yTop = (GetSystemMetrics(SM_CYFULLSCREEN)-(rect.bottom-rect.top))/2;
		SetWindowPos(hWnd, HWND_TOPMOST, CWXPublicData::DPI(xLeft), CWXPublicData::DPI(yTop),-1,-1, SWP_NOSIZE | SWP_NOZORDER);
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	}	
	if(nWindows == 1)
	{
		GetWindowRect(m_PublicData.m_WxMailHwnd, &rect);
		xLeft = rect.left+60;
		yTop = rect.top+60;
		SetWindowPos(hWnd, HWND_TOPMOST, CWXPublicData::DPI(xLeft), CWXPublicData::DPI(yTop), -1, -1, SWP_NOSIZE | SWP_NOZORDER);
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	}
}

DWORD64 g_dwDealWxClientOpenRetAddress = 0;
DWORD64 g_dwDealWxAllMsgRetAddress = 0;
DWORD64 g_dwDealWxRevokeAddress = 0;
DWORD64 g_dwDealWxRevokeAddress_Up = 0;

DWORD64 g_dwMsgType = ENUM_WX_TYPE_EMOJI;
WCHAR g_szMsg[MAXWORD] = {0};
DWORD64 g_dwChatRoomAddress = 0;
DWORD64 g_dwMsgContentAddress = 0;

//CSql::tag_WxSql g_tagWxSql;




DWORD64 g_dwAddressWX = 0;
// 个人防撤回
DWORD64 g_dwAddressQQ_1 = 0;
// QQ群防撤回
DWORD64 g_dwAddressQQ_2 = 0;

BOOL CompareAPage(DWORD64 dwBaseAddr, PBYTE lpValue, int nLen, PBYTE lpReadValue, DWORD dwSize, PBYTE lpFoundPass, DWORD64 &dwRetAddress)
{
    BOOL bRet = TRUE;
	
    PBYTE lpBase = (PBYTE)dwBaseAddr;
    
    for (DWORD i=0;i<dwSize;i++)  
    {  
        for(int j = 0; j < nLen; j++)
        {
			// 2022.02.21 要找两次
			static int nOne = 1;
            if(lpBase+i != lpValue && (lpBase[i+j] == lpValue[j] || lpValue[j] == '?'))
            {
                if(j >= nLen-1 && nOne == 2)
                {
                    bRet = 0x2b;
					
                    char szTemp[MAXBYTE] = {0};
                    sprintf(szTemp, "找到数据 222 数据地址 %016I64x  -  %016I64x   i= %d  ", lpBase+i, lpValue, i);
                    _Log0(szTemp);
					
					
					dwRetAddress = (DWORD64)(lpBase+i);
					
					return bRet;
                }
				else if(j >= nLen-1)
				{
// 					_Log0("找到一次数据! 准备第二次查找!");
// 					nOne++;


					bRet = 0x2b;

					char szTemp[MAXBYTE] = {0};
					sprintf(szTemp, "找到数据 111 数据地址 %016I64x  -  %016I64x   i= %d  ", lpBase+i, lpValue, i);
					_Log0(szTemp);


					dwRetAddress = (DWORD64)(lpBase+i);

					return bRet;
				}
            }
            else
            {
                break;
            }
        }
    }
	
    return bRet;
}


BOOL FindFirst(PBYTE lpValue, int nLen, PBYTE lpFoundPass, DWORD64 &dwRetAddress)  
{
    DWORD64 dwBig = 0;
    BOOL bRet = FALSE;
    PBYTE ptr = lpFoundPass; 
    MEMORY_BASIC_INFORMATION mbi; 
    DWORD64 dwBytesReturn = sizeof(MEMORY_BASIC_INFORMATION); 
    while( dwBytesReturn == sizeof(MEMORY_BASIC_INFORMATION) ) 
    { 
        dwBytesReturn = VirtualQuery(ptr,&mbi,sizeof(MEMORY_BASIC_INFORMATION) ); 
        if(mbi.Type == MEM_FREE ) 
        {
            mbi.AllocationBase = mbi.BaseAddress; 
        }
        
        switch(mbi.Protect)
        {
        case PAGE_NOACCESS: 
            {
                int i = 0;
                break;
            }
        case PAGE_READONLY: 
            {
                //                CompareAPage((DWORD)ptr,lpValue, nLen, mbi.RegionSize);
                break;
            }
        case PAGE_READWRITE:
        case PAGE_WRITECOPY:
        case PAGE_EXECUTE:
        case PAGE_EXECUTE_READ:
        case PAGE_EXECUTE_READWRITE:
        case PAGE_EXECUTE_WRITECOPY:
            {
                PBYTE lpReadValue = new BYTE[mbi.RegionSize];
                
                if(mbi.RegionSize > dwBig)
                    dwBig = mbi.RegionSize;
                
				_try
				{
					// 0x7FF9CCF9B92C
					if(ptr >= (PBYTE)0x00007FF9CC000000)
					{
						int a = 0;
					}

					// 0x2b 表示成功找到密码
					if(0x2b == CompareAPage((DWORD64)ptr,lpValue, nLen, lpReadValue, mbi.RegionSize, lpFoundPass, dwRetAddress))
					{
						bRet = TRUE;
					}
				}
				_except(1)
				{
#ifdef LOG
					OutputDebugString("xxxx  异常了 ....");
#endif
				}
				
                
                if(lpReadValue)
                {
                    delete[] lpReadValue;
                    lpReadValue = NULL;
                }
                
                if(bRet)
                    return bRet;
                
                break;
            }
        default:
            //din="OTHER";
            int x = 0;
        }
        
        ptr += mbi.RegionSize;
    }
    
    return FALSE;
}


void GetModuleFilePath(HANDLE hModule, PVOID lpBase)
{
	//有DLL加载 提示模块信息表要更新
	DWORD m_GetModule = TRUE;
	
	//传入参数的有效性判断
	if(hModule == NULL || lpBase == NULL)
	{
		return;
	}
	
	TCHAR pszFilename[MAX_PATH*10];
	HANDLE hFileMap;
	
	// Get the file size.
	LARGE_INTEGER unFileSize = {0};
	unFileSize.LowPart = GetFileSize(hModule, (LPDWORD)&unFileSize.HighPart);
	
	if(unFileSize.LowPart == 0 && unFileSize.HighPart == 0)
	{
		printf(TEXT("Cannot map a file with a length of zero.\n"));
		return ;
	}
	
	// Create a file mapping object.
	hFileMap = CreateFileMapping(hModule, 
		NULL, 
		PAGE_READONLY,
		0, 
		0,
		NULL);
	
	if (hFileMap) 
	{
		// Create a file mapping to get the file name.
		void* pMem = MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 0);
		
		if (pMem) 
		{
			if (GetMappedFileName (GetCurrentProcess(), 
				pMem, 
				pszFilename,
				MAX_PATH)) 
			{
				
				// Translate path with device name to drive letters.
				TCHAR szTemp[MAXBYTE] = {0};
				szTemp[0] = '\0';
				
				if (GetLogicalDriveStrings(MAXBYTE-1, szTemp)) 
				{
					TCHAR szName[MAX_PATH];
					TCHAR szDrive[3] = TEXT(" :");
					BOOL bFound = FALSE;
					TCHAR* p = szTemp;
					
					do 
					{
						// Copy the drive letter to the template string
						*szDrive = *p;
						
						// Look up each device name
						if (QueryDosDevice(szDrive, szName, MAX_PATH))
						{
							size_t uNameLen = strlen(szName);
							
							if (uNameLen < MAX_PATH) 
							{
								bFound = strnicmp(pszFilename, szName, uNameLen) == 0;
								
								if (bFound && *(pszFilename + uNameLen) == '\\') 
								{
									// Reconstruct pszFilename using szTempFile
									// Replace device path with DOS path
									TCHAR szTempFile[MAX_PATH*10] = {0};
									sprintf(szTempFile,
										
										TEXT("%s%s"),
										szDrive,
										pszFilename+uNameLen);
									strncpy(pszFilename, szTempFile, sizeof(szTempFile));
									
									_Log0(szTempFile);
									
									break;
								}
							}
						}
						
						// Go to the next NULL character.
						while (*p++);
					} while (!bFound && *p); // end of string
				}
			}
			
			
			UnmapViewOfFile(pMem);
			pMem = NULL;
		} 
		
		CloseHandle(hFileMap);
		hFileMap = NULL;
	}
	
	// 	m_Result.AddString(pszFilename);
	// 	m_Result.SetTopIndex(m_Result.GetCount()-1); 
	_Log1("地址: [%s]", pszFilename);
}



BOOL CWX::GetHookData(DWORD dwModule)
{	
	_Log0("准备开始搜索数据..");
	
	////////////////微信数据//////////////////////////////////////////////////////////
	// v3.5.0.46 只有防撤回,没有撤回提示
	// 	unsigned char szMagic[] = {
	// 		0x74, 0x32, 0xb9, '?', '?', '?', '?', 
	// 		0x8A, 0x10, 0x3A, 0x11, 0x75, 0x1A, 0x84, 0xD2, 0x74, 0x12, 0x8A, 0x50, 0x01, 0x3A, 0x51, 0x01, 
	// 		0x75, 0x0E, 0x83, 0xC0, 0x02, 0x83, 0xC1, 0x02, 0x84, 0xD2, 0x75, 0xE4, 0x33, 0xC0, 0xEB, 0x05, 
	// 		0x1B, 0xC0, 0x83, 0xC8, 0x01};
	
	/*
	// v3.5.0.46 有防撤回并且有撤回提示
	
	  // 特征码
	  66DBFB63   /74 4B           JE SHORT WeChatWi.66DBFBB0
	  66DBFB65   |8D8D BCFCFFFF   LEA ECX,DWORD PTR [EBP-0x344]
	  66DBFB6B   |51              PUSH ECX                                 ; WeChatWi.<ModuleEntryPoint>
	  66DBFB6C   |8BC8            MOV ECX,EAX
	  
		744B8D8D????????518BC8
	*/
	//	unsigned char szMagic[] = {0x74, 0x4B, 0x8D, 0x8D, '?', '?', '?', '?', 0x51, 0x8B, 0xC8};
	/*
	// v3.5.0.46 关机状态,再开启,也防撤回
	
	  // 特征码
	  66DBEFF1    64:A3 00000000              MOV DWORD PTR FS:[0],EAX
	  66DBEFF7    C745 EC 00000000            MOV DWORD PTR [EBP-0x14],0x0
	  66DBEFFE    C745 FC 00000000            MOV DWORD PTR [EBP-0x4],0x0
	  66DBF005    837D 0C 00                  CMP DWORD PTR [EBP+0xC],0x0
	  66DBF009    7E 2A                       JLE SHORT WeChatWi.66DBF035
	  66DBF00B    6A FF                       PUSH -0x1
	  
	  66DBF00D    68 B4339368                 PUSH WeChatWi.689333B4                   ; UNICODE "revokemsg"
	  66DBF012    8D4D C4                     LEA ECX,DWORD PTR [EBP-0x3C]
	  64A300000000C745EC00000000C745FC00000000837D0C007E2A6AFF
	*/
	
	
	/*
	// v3.7.6.29
	
	  // 特征码
	  6024C279  |. /74 65             JE SHORT WeChatWi.6024C2E0
	  6024C27B  |. |E8 5077CAFF       CALL WeChatWi.5FEF39D0
	  6024C280  |. |8D8D B8FCFFFF     LEA ECX,DWORD PTR [EBP-0x348]
	  6024C286  |.  E8 C5A1FFFF       CALL 0FBA6450                            ; [WeChatWin.0FBA6450
	  6024C28B  |.  85C0              TEST EAX, EAX
	  7465E8????????8D8D
	  unsigned char szWXData[] = {0x74, 0x65, 0xE8, '?', '?', '?', '?', 0x8D, 0x8D};
	*/

	
	/*
	// v3.9.0.28
	
	  // 特征码
	  6257C78B   /0F84 EF010000   JE 6257C980                              ; 0F8490909090E8909090908D8D
	  6257C791   |E8 CA5EBEFF     CALL 62162660                            ; 0F84????????E8????????8D8D????????E8????????85
	  6257C796   |8D8D A8FCFFFF   LEA ECX,DWORD PTR SS:[EBP-0x358]
	  6257C79C   |E8 5FA0FFFF     CALL 62576800                            ; WeChatWi.62576800
	  6257C7A1   |85C0            TEST EAX,EAX
	  0F84????????E8????????8D8D????????E8????????85
	
	  unsigned char szWXData[] = {0x0F, 0x84, '?', '?', '?', '?', 0xE8, '?', '?', '?', '?', 0x8D, 0x8D, '?', '?', '?', '?', 0xE8, '?', '?', '?', '?', 0x85};
	*/

	  
	/*
	// v3.9.2.23
	  
	// 特征码
	679265ED                            | 83E0 40                      | and eax,0x40                                        |
	679265F0                            | C747 40 02000000             | mov dword ptr ds:[edi+0x40],0x2                     | edi+40:",0"
	679265F7                            | 807D EA 00                   | cmp byte ptr ss:[ebp-0x16],0x0                      |
	679265FB                            | 8987 A0020000                | mov dword ptr ds:[edi+0x2A0],eax                    |
	67926601                            | 74 53                        | je wechatwin.67926656                               | Magic Jump
	67926603                            | E8 F8C0BDFF                  | call <wechatwin.sub_67502700>                       |
	67926608                            | 8BCF                         | mov ecx,edi                                         |
	6792660A                            | E8 51A5FFFF                  | call <wechatwin.sub_67920B60>                       |
	6792660F                            | 85C0                         | test eax,eax                                        |
	67926611                            | 74 0E                        | je wechatwin.67926621                               |
	67926613                            | 8B08                         | mov ecx,dword ptr ds:[eax]                          |
	67926615                            | 6A 01                        | push 0x1                                            |
	67926617                            | 6A 01                        | push 0x1                                            |
	67926619                            | 57                           | push edi                                            |
	6792661A                            | 8B51 20                      | mov edx,dword ptr ds:[ecx+0x20]                     |
	6792661D                            | 8BC8                         | mov ecx,eax                                         |
	74??E8????????8BCFE8????????85C074??8B086A01
	unsigned char szWXData[] = {0x74, '?', 0xE8, '?', '?', '?', '?', 0x8B, 0xCF, 0xE8, '?', '?', '?', '?', 0x85, 0xC0, 0x74, '?', 0x8B, 0x08, 0x6A, 0x01};
	*/







	/*
	// v3.9.5.81
	  
	// 特征码
	00007FF9CCA4B916 | 8986 10040000            | mov dword ptr ds:[rsi+410],eax            |
	00007FF9CCA4B91C | 44:0FB6BD 482D0000       | movzx r15d,byte ptr ss:[rbp+2D48]         |
	00007FF9CCA4B924 | E8 D74AABFF              | call wechatwin.7FF9CC500400               |
	00007FF9CCA4B929 | 45:84FF                  | test r15b,r15b                            |
	00007FF9CCA4B92C | 74 42                    | je wechatwin.7FF9CCA4B970                 | Magic Jump
	00007FF9CCA4B92E | 48:8BCE                  | mov rcx,rsi                               | 刚开始搜索 "gh_" x32只有1个 x64会有多个
	00007FF9CCA4B931 | E8 DA3EFFFF              | call wechatwin.7FF9CCA3F810               |  每个下断. 撤回消息中断的就是
	00007FF9CCA4B936 | 48:85C0                  | test rax,rax                              |
	00007FF9CCA4B939 | 74 17                    | je wechatwin.7FF9CCA4B952                 |
	00007FF9CCA4B93B | 48:8B08                  | mov rcx,qword ptr ds:[rax]                |
	00007FF9CCA4B93E | 4C:8B51 48               | mov r10,qword ptr ds:[rcx+48]             |
	00007FF9CCA4B942 | 41:B1 01                 | mov r9b,1                                 |
	00007FF9CCA4B945 | 45:0FB6C1                | movzx r8d,r9b                             |
	00007FF9CCA4B949 | 48:8BD6                  | mov rdx,rsi                               |
	00007FF9CCA4B94C | 48:8BC8                  | mov rcx,rax                               |
	00007FF9CCA4B94F | 41:FFD2                  | call r10                                  |
	7442488BCEE8????????4885C07417
	unsigned char szWXData[] = {0x74, 0x42, 0x48, 0x8B, 0xCE, 0xE8, '?', '?', '?', '?', 0x48, 0x85, 0xC0, 0x74, 0x17};
	*/





	

	/*
	// v3.9.7.25
	  
	// 特征码
	00007FFEC7E1EE36 | B9 02000000             | mov ecx,2                               |
	00007FFEC7E1EE3B | E8 507D9A00             | call <wechatwin.sub_7FFEC87C6B90>       |
	00007FFEC7E1EE40 | 8B86 10040000           | mov eax,dword ptr ds:[rsi+410]          |
	00007FFEC7E1EE46 | C746 40 02000000        | mov dword ptr ds:[rsi+40],2             |
	00007FFEC7E1EE4D | 83E0 40                 | and eax,40                              |
	00007FFEC7E1EE50 | 8986 10040000           | mov dword ptr ds:[rsi+410],eax          | 
	00007FFEC7E1EE56 | E8 2510F7FF             | call <wechatwin.sub_7FFEC7D8FE80>       |
	00007FFEC7E1EE5B | 45:84ED                 | test r13b,r13b                          |
	00007FFEC7E1EE5E | 74 3F                   | je wechatwin.7FFEC7E1EE9F               | Magic Jump
	00007FFEC7E1EE60 | 48:8BCE                 | mov rcx,rsi                             | 直接搜索 "Regen rv msg sortseq" 只有两个结果,第一个就是
	00007FFEC7E1EE63 | E8 A8214D00             | call <wechatwin.sub_7FFEC82F1010>       |
	00007FFEC7E1EE68 | 48:85C0                 | test rax,rax                            |
	00007FFEC7E1EE6B | 74 17                   | je wechatwin.7FFEC7E1EE84               |
	00007FFEC7E1EE6D | 48:8B08                 | mov rcx,qword ptr ds:[rax]              |
	00007FFEC7E1EE70 | 4C:8B51 48              | mov r10,qword ptr ds:[rcx+48]           |
	00007FFEC7E1EE74 | 41:B1 01                | mov r9b,1                               |
	00007FFEC7E1EE77 | 45:0FB6C1               | movzx r8d,r9b                           |
	00007FFEC7E1EE7B | 48:8BD6                 | mov rdx,rsi                             |
	00007FFEC7E1EE7E | 48:8BC8                 | mov rcx,rax                             |
	00007FFEC7E1EE81 | 41:FFD2                 | call r10                                |
	00007FFEC7E1EE84 | B9 30000000             | mov ecx,30                              | 30:'0'
	00007FFEC7E1EE89 | E8 FAAB3A02             | call <wechatwin.sub_7FFECA1C9A88>       |
	00007FFEC7E1EE8E | 48:8945 80              | mov qword ptr ss:[rbp-80],rax           |
	00007FFEC7E1EE92 | 48:8BC8                 | mov rcx,rax                             |
	00007FFEC7E1EE95 | E8 16040000             | call <wechatwin.sub_7FFEC7E1F2B0>       |
	00007FFEC7E1EE9A | 48:8BF8                 | mov rdi,rax                             |
	74??488BCEE8????????4885C07417
	unsigned char szWXData[] = {0x74, '?', 0x48, 0x8B, 0xCE, 0xE8, '?', '?', '?', '?', 0x48, 0x85, 0xC0, 0x74, 0x17};
	*/












	
	/*
	// v3.9.9.43
	  
	// 特征码
	00007FFF82314672 | 4C:8D8424 E0000000       | lea r8,qword ptr ss:[rsp+E0]            | [rsp+E0]:GetHandleVerifier+16902
	00007FFF8231467A | 48:8BD3                  | mov rdx,rbx                             |
	00007FFF8231467D | 49:8D4E 38               | lea rcx,qword ptr ds:[r14+38]           |
	00007FFF82314681 | E8 FA5D0000              | call wechatwin.7FFF8231A480             |
	00007FFF82314686 | 32C0                     | xor al,al                               |
	00007FFF82314688 | EB 56                    | jmp wechatwin.7FFF823146E0              |
	00007FFF8231468A | 83FF 04                  | cmp edi,4                               |
	00007FFF8231468D | 75 10                    | jne wechatwin.7FFF8231469F              | 这里是上个版本magic处,回溯两次
	00007FFF8231468F | 4D:8BC6                  | mov r8,r14                              | 75??4D8BC6488BD6488BCDE8????????EB??83
	00007FFF82314692 | 48:8BD6                  | mov rdx,rsi                             |
	00007FFF82314695 | 48:8BCD                  | mov rcx,rbp                             |
	00007FFF82314698 | E8 837BFEFF              | call wechatwin.7FFF822FC220             |
	00007FFF8231469D | EB 41                    | jmp wechatwin.7FFF823146E0              |
	00007FFF8231469F | 83FF 21                  | cmp edi,21                              | 21:'!'
	00007FFF823146A2 | 75 12                    | jne wechatwin.7FFF823146B6              |
	00007FFF823146A4 | 4D:8BC6                  | mov r8,r14                              |
	00007FFF823146A7 | 48:8BD6                  | mov rdx,rsi                             |
	00007FFF823146AA | 48:8BCD                  | mov rcx,rbp                             |
	00007FFF823146AD | E8 4E83FEFF              | call wechatwin.7FFF822FCA00             |
	00007FFF823146B2 | 32C0                     | xor al,al                               |
	00007FFF823146B4 | EB 2A                    | jmp wechatwin.7FFF823146E0              |
	00007FFF823146B6 | 83FF 24                  | cmp edi,24                              | 24:'$'
	00007FFF823146B9 | 75 12                    | jne wechatwin.7FFF823146CD              |
	00007FFF823146BB | 4D:8BC6                  | mov r8,r14                              |
	00007FFF823146BE | 48:8BD6                  | mov rdx,rsi                             |
	00007FFF823146C1 | 48:8BCD                  | mov rcx,rbp                             |
	00007FFF823146C4 | E8 478AFEFF              | call wechatwin.7FFF822FD110             |
	75??4D8BC6488BD6488BCDE8????????EB??83
	unsigned char szWXData[] = {0x75, '?', 0x4D, 0x8B, 0xC6, 0x48, 0x8B, 0xD6, 0x48, 0x8B, 0xCD, 0xE8, '?', '?', '?', '?', 0xEB, '?', 0x83};
	*/








	
	/*
	// v3.9.10.19
	// 特征码
	00007FFBA7461588 | 48:81EC A0060000         | sub rsp,6A0                             |
	00007FFBA746158F | 0F29B424 90060000        | movaps xmmword ptr ss:[rsp+690],xmm6    |
	00007FFBA7461597 | 48:8BF2                  | mov rsi,rdx                             |
	00007FFBA746159A | 4C:8BF9                  | mov r15,rcx                             |
	00007FFBA746159D | 45:33E4                  | xor r12d,r12d                           |
	00007FFBA74615A0 | 44:89A5 F8050000         | mov dword ptr ss:[rbp+5F8],r12d         |
	00007FFBA74615A7 | E8 C4E8F3FF              | call wechatwin.7FFBA739FE70             |
	00007FFBA74615AC | 44:0FB680 F8070000       | movzx r8d,byte ptr ds:[rax+7F8]         |
	00007FFBA74615B4 | 45:84C0                  | test r8b,r8b                            |
	00007FFBA74615B7 | 0F84 F3080000            | je wechatwin.7FFBA7461EB0               | 之前的无效了,现在改成搜 "xx",共2结果,进第一个结果函数,选函数开始就是.
	00007FFBA74615BD | 45:0FB66F 4D             | movzx r13d,byte ptr ds:[r15+4D]         |
	00007FFBA74615C2 | 44:88A5 F0050000         | mov byte ptr ss:[rbp+5F0],r12b          |
	00007FFBA74615C9 | 45:84ED                  | test r13b,r13b                          |
	00007FFBA74615CC | 0F84 B4000000            | je wechatwin.7FFBA7461686               |
	00007FFBA74615D2 | 817E 38 10270000         | cmp dword ptr ds:[rsi+38],2710          |
	00007FFBA74615D9 | 0F85 A7000000            | jne wechatwin.7FFBA7461686              |
	00007FFBA74615DF | 44:886424 60             | mov byte ptr ss:[rsp+60],r12b           |
	00007FFBA74615E4 | 49:8B07                  | mov rax,qword ptr ds:[r15]              |
	00007FFBA74615E7 | 48:894424 68             | mov qword ptr ss:[rsp+68],rax           |
	00007FFBA74615EC | 0F1005 2D092303          | movups xmm0,xmmword ptr ds:[7FFBAA691F2 |
	00007FFBA74615F3 | 0F2945 90                | movaps xmmword ptr ss:[rbp-70],xmm0     |
	00007FFBA74615F7 | 0F2945 80                | movaps xmmword ptr ss:[rbp-80],xmm0     |
	00007FFBA74615FB | 0F2945 F0                | movaps xmmword ptr ss:[rbp-10],xmm0     |
	00007FFBA74615FF | 0F294424 70              | movaps xmmword ptr ss:[rsp+70],xmm0     |
	0F??F3080000450FB66F4D4488A5F00500004584ED
	unsigned char szWXData[] = {0x0F, '?', 0xF3, 0x08, 0x00, 0x00, 0x45, 0x0F, 0xB6, 0x6F, 0x4D, 0x44, 0x88, 0xA5, 0xF0, 0x05, 0x00, 0x00, 0x45, 0x84, 0xED};
	*/



	
	/*
	// v3.9.12.17
	// 参考 https://github.com/huiyadanli/RevokeMsgPatcher/blob/master/RevokeMsgPatcher.Assistant/JsonData.cs 防撤回带提示(新)
	// 特征码
	00007FFAAB13E268        | 48:8D15 41995903                   | lea rdx,qword ptr ds:[7FFAAE6D7BB0]    | 00007FFAAE6D7BB0:L"D:\\Tools\\agent\\workspace\\MicroMsgWindowsV3912\\MM3Party\\pugixml\\pugixml.cpp"
	00007FFAAB13E26F        | 48:8D0D 22995903                   | lea rcx,qword ptr ds:[7FFAAE6D7B98]    | 00007FFAAE6D7B98:L"src && dst"
	00007FFAAB13E276        | E8 D5207902                        | call wechatwin.7FFAAD8D0350            |
	00007FFAAB13E27B        | 48:8BC7                            | mov rax,rdi                            |
	00007FFAAB13E27E        | 48:2BDF                            | sub rbx,rdi                            |
	00007FFAAB13E281        | 0FB710                             | movzx edx,word ptr ds:[rax]            |
	00007FFAAB13E284        | 0FB70C18                           | movzx ecx,word ptr ds:[rax+rbx]        |
	00007FFAAB13E288        | 2BD1                               | sub edx,ecx                            |
	00007FFAAB13E28A        | 75 08                              | jne wechatwin.7FFAAB13E294             |
	00007FFAAB13E28C        | 48:83C0 02                         | add rax,2                              |
	00007FFAAB13E290        | 85C9                               | test ecx,ecx                           |
	00007FFAAB13E292        | 75 ED                              | jne wechatwin.7FFAAB13E281             |
	00007FFAAB13E294        | 85D2                               | test edx,edx                           |
	00007FFAAB13E296        | 74 0F                              | je wechatwin.7FFAAB13E2A7              |
	00007FFAAB13E298        | 48:8B76 30                         | mov rsi,qword ptr ds:[rsi+30]          |
	00007FFAAB13E29C        | 48:85F6                            | test rsi,rsi                           |
	00007FFAAB13E29F        | 0F84 92FDFFFF                      | je wechatwin.7FFAAB13E037              |
	00007FFAAB13E2A5        | EB AD                              | jmp wechatwin.7FFAAB13E254             |
	00007FFAAB13E2A7        | 4C:8B46 20                         | mov r8,qword ptr ds:[rsi+20]           |
	00007FFAAB13E2AB        | 4D:85C0                            | test r8,r8                             |
	00007FFAAB13E2AE        | 0F84 83FDFFFF                      | je wechatwin.7FFAAB13E037              |
	00007FFAAB13E2B4        | 4C:8D0D 6D752503                   | lea r9,qword ptr ds:[7FFAAE395828]     | 00007FFAAE395828:L"newmsgid"
	00007FFAAB13E2BB        | 0F1F4400 00                        | nop dword ptr ds:[rax+rax],eax         |
	00007FFAAB13E2C0        | 49:8B50 08                         | mov rdx,qword ptr ds:[r8+8]            |
	00007FFAAB13E2C4        | 48:85D2                            | test rdx,rdx                           |
	00007FFAAB13E2C7        | 74 29                              | je wechatwin.7FFAAB13E2F2              |
	00007FFAAB13E2C9        | 48:C7C1 FFFFFFFF                   | mov rcx,FFFFFFFFFFFFFFFF               |
	00007FFAAB13E2D0        | 41:0FB74449 02                     | movzx eax,word ptr ds:[r9+rcx*2+2]     |
	00007FFAAB13E2D6        | 66:3B444A 02                       | cmp ax,word ptr ds:[rdx+rcx*2+2]       |
	00007FFAAB13E2DB        | 75 15                              | jne wechatwin.7FFAAB13E2F2             |
	00007FFAAB13E2DD        | 48:83C1 02                         | add rcx,2                              |
	00007FFAAB13E2E1        | 48:83F9 09                         | cmp rcx,9                              | 09:'\t'
	00007FFAAB13E2E5        | 74 1A                              | je wechatwin.7FFAAB13E301              |
	00007FFAAB13E2E7        | 41:0FB70449                        | movzx eax,word ptr ds:[r9+rcx*2]       |
	00007FFAAB13E2EC        | 66:3B044A                          | cmp ax,word ptr ds:[rdx+rcx*2]         |
	00007FFAAB13E2F0        | 74 DE                              | je wechatwin.7FFAAB13E2D0              |
	00007FFAAB13E2F2        | 4D:8B40 30                         | mov r8,qword ptr ds:[r8+30]            |
	00007FFAAB13E2F6        | 4D:85C0                            | test r8,r8                             |
	00007FFAAB13E2F9        | 0F84 38FDFFFF                      | je wechatwin.7FFAAB13E037              |
	00007FFAAB13E2FF        | EB BF                              | jmp wechatwin.7FFAAB13E2C0             | 这里NOP掉,撤回会有提示
	00007FFAAB13E301        | 41:8B00                            | mov eax,dword ptr ds:[r8]              |
	00007FFAAB13E304        | 24 0F                              | and al,F                               |
	00007FFAAB13E306        | 3C 02                              | cmp al,2                               |
	00007FFAAB13E308        | 75 09                              | jne wechatwin.7FFAAB13E313             |
	00007FFAAB13E30A        | 49:8B40 10                         | mov rax,qword ptr ds:[r8+10]           |
	00007FFAAB13E30E        | 48:85C0                            | test rax,rax                           |
	00007FFAAB13E311        | 75 33                              | jne wechatwin.7FFAAB13E346             |
	00007FFAAB13E313        | 49:8B48 20                         | mov rcx,qword ptr ds:[r8+20]           |
	00007FFAAB13E317        | 48:85C9                            | test rcx,rcx                           |
	00007FFAAB13E31A        | 74 23                              | je wechatwin.7FFAAB13E33F              |
	00007FFAAB13E31C        | 0F1F40 00                          | nop dword ptr ds:[rax],eax             |
	00007FFAAB13E320        | 8B01                               | mov eax,dword ptr ds:[rcx]             |
	00007FFAAB13E322        | 83E0 0F                            | and eax,F                              |
	00007FFAAB13E325        | 83E8 03                            | sub eax,3                              |
	00007FFAAB13E328        | 83F8 01                            | cmp eax,1                              |
	00007FFAAB13E32B        | 77 09                              | ja wechatwin.7FFAAB13E336              |
	00007FFAAB13E32D        | 48:8B41 10                         | mov rax,qword ptr ds:[rcx+10]          |
	00007FFAAB13E331        | 48:85C0                            | test rax,rax                           |
	00007FFAAB13E334        | 75 10                              | jne wechatwin.7FFAAB13E346             |
	00007FFAAB13E336        | 48:8B49 30                         | mov rcx,qword ptr ds:[rcx+30]          |
	00007FFAAB13E33A        | 48:85C9                            | test rcx,rcx                           |
	00007FFAAB13E33D        | 75 E1                              | jne wechatwin.7FFAAB13E320             |
	00007FFAAB13E33F        | 48:8D05 C6EA2003                   | lea rax,qword ptr ds:[7FFAAE34CE0C]    |
	00007FFAAB13E346        | 33DB                               | xor ebx,ebx                            |
	00007FFAAB13E348        | 48:895C24 60                       | mov qword ptr ss:[rsp+60],rbx          |
	00007FFAAB13E34D        | 44:8D43 0A                         | lea r8d,qword ptr ds:[rbx+A]           |
	00007FFAAB13E351        | 48:8D5424 60                       | lea rdx,qword ptr ss:[rsp+60]          |
	00007FFAAB13E356        | 48:8BC8                            | mov rcx,rax                            |
	00007FFAAB13E359        | E8 12B87602                        | call wechatwin.7FFAAD8A9B70            |
	4D????0F84????????EBBF41
	*/
	unsigned char szWXData[] = {0x4D, '?', '?', 0x0F, 0x84, '?', '?', '?', '?', 0xEB, 0xBF, 0x41};
	///////////////////////////////////微信数据///////////////////////////////////////
	
	
	
	///////////////////////QQ数据///////////////////////////////////////////////////
	/*
	// v9.6.6 (28796)
	5588BC8E  |.  51            PUSH ECX                                 ;  IM.<ModuleEntryPoint>
	5588BC8F  |.  68 9866C055   PUSH IM.55C06698                         ;  bytes_reserved
	5588BC94  |.  56            PUSH ESI
	5588BC95  |.  FF50 78       CALL DWORD PTR [EAX+0x78]
	5588BC98  |.  85C0          TEST EAX,EAX
	5588BC9A  |.  79 39         JNS SHORT IM.5588BCD5                    ;  nop 去掉撤回
	5588BC9C  |.  8D45 0C       LEA EAX,DWORD PTR [EBP+0xC]
	5588BC9F  |.  C745 0C A866C>MOV DWORD PTR [EBP+0xC],IM.55C066A8      ;  OnSysProtobufDataCome failed to get bytes_reserved
	5588BCA6  |.  50            PUSH EAX
	5588BCA7  |.  68 1853BE55   PUSH IM.55BE5318                         ;  %s
	5588BCAC  |.  FF35 C4A5D055 PUSH DWORD PTR [0x55D0A5C4]              ;  IM.55C05858
	5588BCB2  |.  6A 02         PUSH 0x2
	5588BCB4  |.  68 1055BE55   PUSH IM.55BE5510                         ;  func
	5588BCB9  |.  68 3D020000   PUSH 0x23D
	5588BCBE  |.  68 1C55BE55   PUSH IM.55BE551C                         ;  file
	5168??????????FF????????79??8D????C745????????????68????????FF??????????6A0268????????68????????68
	*/
	unsigned char szQQData_1[] = {
		0x51, 0x68, '?', '?', '?', '?', '?', 0xFF, '?', '?', '?', '?', 0x79, '?', 0x8D, '?', '?', 0xC7, 0x45, 
		'?', '?', '?', '?', '?', '?', 0x68, '?', '?', '?', '?', 0xFF, '?', '?', '?', '?', '?', 0x6A, 0x02, 
		0x68, '?', '?', '?', '?', 0x68, '?', '?', '?', '?', 0x68};


	/*
	559CB73F          | 8B75 08           | mov esi,dword ptr ss:[ebp+0x8]                      |
	559CB742          | 8D45 F4           | lea eax,dword ptr ss:[ebp-0xC]                      | [ebp-C]:__except_handler4
	559CB745          | 50                | push eax                                            |
	559CB746          | FF75 10           | push dword ptr ss:[ebp+0x10]                        |
	559CB749          | 33DB              | xor ebx,ebx                                         |
	559CB74B          | 56                | push esi                                            |
	559CB74C          | 895D F4           | mov dword ptr ss:[ebp-0xC],ebx                      | [ebp-C]:__except_handler4
	559CB74F          | E8 BFE2FFFF       | call im.559C9A13                                    |
	559CB754          | 83C4 28           | add esp,0x28                                        |
	559CB757          | 85C0              | test eax,eax                                        |
	559CB759          | 75 34             | jne im.559CB78F                                     |
	559CB75B          | 8D45 0C           | lea eax,dword ptr ss:[ebp+0xC]                      | [ebp+C]:@BaseThreadInitThunk@12
	8B75??8D45F450FF75??33DB56895DF4E8????????83C4??85C075348D45
	*/
	unsigned char szQQData_2[] = {
		0x8B, 0x75, '?', 0x8D, 0x45, 0xF4, 0x50, 0xFF, 0x75, '?', 0x33, 0xDB, 0x56, 0x89, 
		0x5D, 0xF4, 0xE8, '?', '?', '?', '?', 0x83, 0xC4, '?', 0x85, 0xC0, 0x75, 0x34, 
		0x8D, 0x45
	};
	///////////////////////////////////QQ数据///////////////////////////////////////
		
		
		// 判断是不是微信
		if(GetModuleHandle(WX_DLL_NAME))
		{
			_Log0("发现 微信  WeChatWin.dll");
			//进行第一次查找
			while(TRUE)
			{
				_try
				{
					if(!FindFirst(szWXData, sizeof(szWXData), (PUCHAR)dwModule, g_dwAddressWX))
					{
#ifdef LOG				
						_Log0("未找到 wx 防撤回数据, 休息5秒");
#endif
						Sleep(5*1000);
					}
					else
					{
						_Log1("找到 wx 防撤回数据 :) 111 %08X", g_dwAddressWX);
						break;
					}
				}
				_except(1)
				{
					_Log0("11111  异常了 ....");
				}
			}
			
			// 消息反撤回  2018.05.05 更新,可以点击查看消息
			m_tagWrite[ENUM_REVOKE_WX_ADDRESS].m_lpHookAddress = (PVOID)(g_dwAddressWX);

//			m_tagWrite[ENUM_REVOKE_WX_ADDRESS].m_szHookData[0] = 0xeb;
//			m_tagWrite[ENUM_REVOKE_WX_ADDRESS].m_dwHookSize = WX_RECOVER_OFFSET_SIZE;

//			m_tagWrite[ENUM_REVOKE_WX_ADDRESS].m_szHookData[0] = 0x90;
// 			m_tagWrite[ENUM_REVOKE_WX_ADDRESS].m_szHookData[1] = 0xe9;
// 			
			m_tagWrite[ENUM_REVOKE_WX_ADDRESS].m_lpHookAddress = (PVOID)(g_dwAddressWX+9);
			m_tagWrite[ENUM_REVOKE_WX_ADDRESS].m_szHookData[0] = 0x90;
			m_tagWrite[ENUM_REVOKE_WX_ADDRESS].m_szHookData[1] = 0x90;

			m_tagWrite[ENUM_REVOKE_WX_ADDRESS].m_dwHookSize = QQ_RECOVER_OFFSET_SIZE;
			
			_Log0("正常消息防撤回,带提示 开启!");
			MemWrite(m_tagWrite[ENUM_REVOKE_WX_ADDRESS].m_lpHookAddress, 
				m_tagWrite[ENUM_REVOKE_WX_ADDRESS].m_dwHookSize, 
				m_tagWrite[ENUM_REVOKE_WX_ADDRESS].m_szHookData, 
				m_tagWrite[ENUM_REVOKE_WX_ADDRESS].m_szHookRecoverData);
			m_bNormalRevoke = TRUE;
			//////////////////////////////////////////////////////////////////////////
			
		}
		// 判断是不是qq
		else if(GetModuleHandle(QQ_DLL_NAME))
		{
			//////////////////////////////////////////////////////////////////////////
			//进行第一次查找
			while(TRUE)
			{
				_try
				{
					if(!FindFirst(szQQData_1, sizeof(szQQData_1), (PUCHAR)dwModule, g_dwAddressQQ_1))
					{
						_Log0("未找到 qq 防撤回数据, 休息5秒");

						Sleep(5*1000);
					}
					else
					{
						g_dwAddressQQ_1 += 12;
						_Log1("找到 qq 个人 防撤回数据 :) 222 %08X", g_dwAddressQQ_1);

						
						_Log0("临时休息29秒");
				//		Sleep(29*1000);
						
						if(!FindFirst(szQQData_2, sizeof(szQQData_2), (PUCHAR)dwModule, g_dwAddressQQ_2))
						{
							_Log0("未找到 qq 防撤回数据, 休息5秒");
							
							Sleep(5*1000);
						}
						else
						{
							g_dwAddressQQ_2 += 0x1A;
							_Log1("找到 qq 群 防撤回数据 :) 222 %08X", g_dwAddressQQ_2);
							break;
						}
					}
				}
				_except(1)
				{
					_Log0("11111  异常了 ....");
				}
			}
			
			_Log0("临时休息30秒");
	//		Sleep(30*1000);

			m_tagWrite[ENUM_REVOKE_QQ_ADDRESS].m_lpHookAddress = (PVOID)(g_dwAddressQQ_1);
			m_tagWrite[ENUM_REVOKE_QQ_ADDRESS].m_dwHookSize = QQ_RECOVER_OFFSET_SIZE;
			m_tagWrite[ENUM_REVOKE_QQ_ADDRESS].m_szHookData[0] = 0x90;
			m_tagWrite[ENUM_REVOKE_QQ_ADDRESS].m_szHookData[1] = 0x90;
			
			_Log0("正常消息防撤回,带提示 开启!");
			MemWrite(m_tagWrite[ENUM_REVOKE_QQ_ADDRESS].m_lpHookAddress, 
				m_tagWrite[ENUM_REVOKE_QQ_ADDRESS].m_dwHookSize, 
				m_tagWrite[ENUM_REVOKE_QQ_ADDRESS].m_szHookData, 
				m_tagWrite[ENUM_REVOKE_QQ_ADDRESS].m_szHookRecoverData);

			
			m_tagWrite[ENUM_REVOKE_QQ_ADDRESS].m_lpHookAddress = (PVOID)(g_dwAddressQQ_2);
			_Log0("正常消息防撤回,带提示 开启!");
			MemWrite(m_tagWrite[ENUM_REVOKE_QQ_ADDRESS].m_lpHookAddress, 
				m_tagWrite[ENUM_REVOKE_QQ_ADDRESS].m_dwHookSize, 
				m_tagWrite[ENUM_REVOKE_QQ_ADDRESS].m_szHookData, 
				m_tagWrite[ENUM_REVOKE_QQ_ADDRESS].m_szHookRecoverData);


			m_bNormalRevoke = TRUE;
			//////////////////////////////////////////////////////////////////////////
			
			_Log0("发现 qq.exe  im.dll");
		}
		
		
		
		_Log0("初始化客户端多开数据");
		
		return true;
}

BOOL CWX::MemWrite(PVOID lpAddress, DWORD dwHookSize, PVOID lpHookValue, PVOID lpBackupHookValue)
{
	DWORD dwOldProetct = 0;
	
	VirtualProtect(lpAddress, dwHookSize, PAGE_EXECUTE_READWRITE, &dwOldProetct);
	
	if(lpBackupHookValue)
	{
		// 备份原始数据
		memcpy(lpBackupHookValue, lpAddress, dwHookSize);
	}
	
	// 修改数据
	memcpy(lpAddress, lpHookValue, dwHookSize);
	
	VirtualProtect(lpAddress, dwHookSize, dwOldProetct, &dwOldProetct);
	
	return TRUE;
}

BOOL CWX::MemRecover(PVOID lpAddress, DWORD dwHookSize, PVOID lpBackupHookValue)
{
	DWORD dwOldProetct = 0;
	
	VirtualProtect(lpAddress, dwHookSize, PAGE_EXECUTE_READWRITE, &dwOldProetct);
	
	// 修改数据
	memcpy(lpAddress, lpBackupHookValue, dwHookSize);
	
	VirtualProtect(lpAddress, dwHookSize, dwOldProetct, &dwOldProetct);
	
	return TRUE;
}

void CWX::InitData(HWND hWnd)
{
	m_hWnd = hWnd;
	//	m_Sql.GetParentHwnd(hWnd);
	
	//	LOG("InitData");
	//	LOGW(SQL_FILE_NAMEW);
	// 	if(access(m_szSqlPath, 0))
	// 	{
	// 		if(TRUE == m_Sql.IsRunSqlCmd("CREATE TABLE WX ([id] INTEGER PRIMARY KEY, [Type] varchar(32), [NickName] varchar(32), [Msg] varchar(32),[Comment] varchar(255))", SQL_CMD_INIT_CARD))
	// 		{
	// 			::MessageBox(m_hWnd, "数据库初始化成功!", WX_WG_TITLE, MB_ICONINFORMATION | MB_SYSTEMMODAL);
	// 		}
	// 	}
}

BOOL CWX::SaveSettings()
{
	char szValue[MAXBYTE] = {0};
	
	itoa(m_bLogSqlSave, szValue, 10);
	WritePrivateProfileString("记录存档", "保存数据库", szValue, m_szWxSettingsPath);
	itoa(m_bLogDisplay, szValue, 10);
	WritePrivateProfileString("记录存档", "查看历史消息", szValue, m_szWxSettingsPath);
	itoa(m_bLogDBG, szValue, 10);
	WritePrivateProfileString("记录存档", "输出DbgView", szValue, m_szWxSettingsPath);
	itoa(m_bLogImg, szValue, 10);
	WritePrivateProfileString("记录存档", "记录图片", szValue, m_szWxSettingsPath);
	itoa(m_bLogText, szValue, 10);
	WritePrivateProfileString("记录存档", "记录文本/经典表情", szValue, m_szWxSettingsPath);
	itoa(m_bLogVoice, szValue, 10);
	WritePrivateProfileString("记录存档", "记录语音", szValue, m_szWxSettingsPath);
	itoa(m_bLogEmoji, szValue, 10);
	WritePrivateProfileString("记录存档", "记录表情符号", szValue, m_szWxSettingsPath);
	itoa(m_bLogMoney, szValue, 10);
	WritePrivateProfileString("记录存档", "记录红包", szValue, m_szWxSettingsPath);
	itoa(m_bLogFile, szValue, 10);
	WritePrivateProfileString("记录存档", "记录文件/小程序/推文", szValue, m_szWxSettingsPath);
	itoa(m_bLogRevoke, szValue, 10);
	WritePrivateProfileString("记录存档", "记录撤回消息", szValue, m_szWxSettingsPath);
	itoa(m_bLogNew, szValue, 10);
	WritePrivateProfileString("记录存档", "记录公众号/新闻", szValue, m_szWxSettingsPath);
	itoa(m_bLogVideo, szValue, 10);
	WritePrivateProfileString("记录存档", "记录小视频", szValue, m_szWxSettingsPath);
	
	itoa(m_bNormalRevoke, szValue, 10);
	WritePrivateProfileString("通用功能", "消息防撤回", szValue, m_szWxSettingsPath);
	itoa(m_bNormalMoneyTip, szValue, 10);
	WritePrivateProfileString("通用功能", "红包提醒", szValue, m_szWxSettingsPath);
	itoa(m_bNormalVoiceTip, szValue, 10);
	WritePrivateProfileString("通用功能", "语音提醒", szValue, m_szWxSettingsPath);
	itoa(m_bNormalClientOpen, szValue, 10);
	WritePrivateProfileString("通用功能", "微信多开", szValue, m_szWxSettingsPath);
	
	
	
	itoa(m_dwHotKey1, szValue, 10);
	WritePrivateProfileString("老板热键", "左键值", szValue, m_szWxSettingsPath);
	itoa(m_dwHotKey2, szValue, 10);
	WritePrivateProfileString("老板热键", "右键值", szValue, m_szWxSettingsPath);
	return TRUE;
}

BOOL CWX::ReadSettings()
{
	char szValue[MAXBYTE] = {0};
	HWND hSubHwnd = NULL;
	
	_Log0("读取配置文件");
	
	m_bLogSqlSave = GetPrivateProfileInt("记录存档", "保存数据库", 0, m_szWxSettingsPath);
	m_bLogDisplay = GetPrivateProfileInt("记录存档", "查看历史消息", 0, m_szWxSettingsPath);
	m_bLogDBG = GetPrivateProfileInt("记录存档", "输出DbgView", 0, m_szWxSettingsPath);
	m_bLogImg = GetPrivateProfileInt("记录存档", "记录图片", 0, m_szWxSettingsPath);
	m_bLogText = GetPrivateProfileInt("记录存档", "记录文本/经典表情", 0, m_szWxSettingsPath);
	m_bLogVoice = GetPrivateProfileInt("记录存档", "记录语音", 0, m_szWxSettingsPath);
	m_bLogEmoji = GetPrivateProfileInt("记录存档", "记录表情符号", 0, m_szWxSettingsPath);
	m_bLogMoney = GetPrivateProfileInt("记录存档", "记录红包", 0, m_szWxSettingsPath);
	m_bLogFile = GetPrivateProfileInt("记录存档", "记录文件/小程序/推文", 0, m_szWxSettingsPath);
	m_bLogRevoke = GetPrivateProfileInt("记录存档", "记录撤回消息", 0, m_szWxSettingsPath);
	m_bLogNew = GetPrivateProfileInt("记录存档", "记录公众号/新闻", 0, m_szWxSettingsPath);
	m_bLogVideo = GetPrivateProfileInt("记录存档", "记录小视频", 0, m_szWxSettingsPath);
	
	m_bNormalRevoke = GetPrivateProfileInt("通用功能", "消息防撤回", 0, m_szWxSettingsPath);
	m_bNormalMoneyTip = GetPrivateProfileInt("通用功能", "红包提醒", 0, m_szWxSettingsPath);
	m_bNormalVoiceTip = GetPrivateProfileInt("通用功能", "语音提醒", 0, m_szWxSettingsPath);
	m_bNormalClientOpen = GetPrivateProfileInt("通用功能", "微信多开", 0, m_szWxSettingsPath);
	
	m_dwHotKey1 = GetPrivateProfileInt("老板热键", "左键值", 0, m_szWxSettingsPath);
	m_dwHotKey2 = GetPrivateProfileInt("老板热键", "右键值", 0, m_szWxSettingsPath);
	
	
	if(m_bLogSqlSave){
		hSubHwnd = GetDlgItem(m_hWnd, CHECK_LOG_SQL_SAVE);
		SendMessage(hSubHwnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
		SendMessage(hSubHwnd, WM_LBUTTONUP, MK_LBUTTON, 0);
		SendMessage(hSubHwnd, BM_SETCHECK, BST_CHECKED, 0);}
	
	if(m_bLogDisplay){
		hSubHwnd = GetDlgItem(m_hWnd, CHECK_LOG_DISPLAY);
		SendMessage(hSubHwnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
		SendMessage(hSubHwnd, WM_LBUTTONUP, MK_LBUTTON, 0);
		SendMessage(hSubHwnd, BM_SETCHECK, BST_CHECKED, 0);}
	
	if(m_bLogDBG){
		hSubHwnd = GetDlgItem(m_hWnd, CHECK_LOG_DBG);
		SendMessage(hSubHwnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
		SendMessage(hSubHwnd, WM_LBUTTONUP, MK_LBUTTON, 0);
		SendMessage(hSubHwnd, BM_SETCHECK, BST_CHECKED, 0);}
	
	if(m_bLogImg){
		hSubHwnd = GetDlgItem(m_hWnd, CHECK_LOG_IMG);
		SendMessage(hSubHwnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
		SendMessage(hSubHwnd, WM_LBUTTONUP, MK_LBUTTON, 0);
		SendMessage(hSubHwnd, BM_SETCHECK, BST_CHECKED, 0);}
	
	if(m_bLogText){
		hSubHwnd = GetDlgItem(m_hWnd, CHECK_LOG_TEXT);
		SendMessage(hSubHwnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
		SendMessage(hSubHwnd, WM_LBUTTONUP, MK_LBUTTON, 0);
		SendMessage(hSubHwnd, BM_SETCHECK, BST_CHECKED, 0);}
	
	if(m_bLogVoice){
		hSubHwnd = GetDlgItem(m_hWnd, CHECK_LOG_VOICE);
		SendMessage(hSubHwnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
		SendMessage(hSubHwnd, WM_LBUTTONUP, MK_LBUTTON, 0);
		SendMessage(hSubHwnd, BM_SETCHECK, BST_CHECKED, 0);}
	
	if(m_bLogEmoji){
		hSubHwnd = GetDlgItem(m_hWnd, CHECK_LOG_EMOJI);
		SendMessage(hSubHwnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
		SendMessage(hSubHwnd, WM_LBUTTONUP, MK_LBUTTON, 0);
		SendMessage(hSubHwnd, BM_SETCHECK, BST_CHECKED, 0);}
	
	if(m_bLogMoney){
		hSubHwnd = GetDlgItem(m_hWnd, CHECK_LOG_MONEY);
		SendMessage(hSubHwnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
		SendMessage(hSubHwnd, WM_LBUTTONUP, MK_LBUTTON, 0);
		SendMessage(hSubHwnd, BM_SETCHECK, BST_CHECKED, 0);}
	
	if(m_bLogFile){
		hSubHwnd = GetDlgItem(m_hWnd, CHECK_LOG_FILE);
		SendMessage(hSubHwnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
		SendMessage(hSubHwnd, WM_LBUTTONUP, MK_LBUTTON, 0);
		SendMessage(hSubHwnd, BM_SETCHECK, BST_CHECKED, 0);}
	
	if(m_bLogRevoke){
		hSubHwnd = GetDlgItem(m_hWnd, CHECK_LOG_REVOKE);
		SendMessage(hSubHwnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
		SendMessage(hSubHwnd, WM_LBUTTONUP, MK_LBUTTON, 0);
		SendMessage(hSubHwnd, BM_SETCHECK, BST_CHECKED, 0);}
	
	if(m_bLogNew){
		hSubHwnd = GetDlgItem(m_hWnd, CHECK_LOG_NEW);
		SendMessage(hSubHwnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
		SendMessage(hSubHwnd, WM_LBUTTONUP, MK_LBUTTON, 0);
		SendMessage(hSubHwnd, BM_SETCHECK, BST_CHECKED, 0);}
	
	if(m_bLogVideo){
		hSubHwnd = GetDlgItem(m_hWnd, CHECK_LOG_VIDEO);
		SendMessage(hSubHwnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
		SendMessage(hSubHwnd, WM_LBUTTONUP, MK_LBUTTON, 0);
		SendMessage(hSubHwnd, BM_SETCHECK, BST_CHECKED, 0);}
	
	
	if(m_bNormalRevoke){
		hSubHwnd = GetDlgItem(m_hWnd, CHECK_REVOKE);
		SendMessage(hSubHwnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
		SendMessage(hSubHwnd, WM_LBUTTONUP, MK_LBUTTON, 0);
		SendMessage(hSubHwnd, BM_SETCHECK, BST_CHECKED, 0);}
	
	if(m_bNormalMoneyTip){
		hSubHwnd = GetDlgItem(m_hWnd, CHECK_MONEY_TIP);
		SendMessage(hSubHwnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
		SendMessage(hSubHwnd, WM_LBUTTONUP, MK_LBUTTON, 0);
		SendMessage(hSubHwnd, BM_SETCHECK, BST_CHECKED, 0);}
	
	if(m_bNormalVoiceTip){
		hSubHwnd = GetDlgItem(m_hWnd, CHECK_VOICE_TIP);
		SendMessage(hSubHwnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
		SendMessage(hSubHwnd, WM_LBUTTONUP, MK_LBUTTON, 0);
		SendMessage(hSubHwnd, BM_SETCHECK, BST_CHECKED, 0);}
	
	if(m_bNormalClientOpen){
		hSubHwnd = GetDlgItem(m_hWnd, CHECK_WXCLIENT_OPEN);
		SendMessage(hSubHwnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
		SendMessage(hSubHwnd, WM_LBUTTONUP, MK_LBUTTON, 0);
		SendMessage(hSubHwnd, BM_SETCHECK, BST_CHECKED, 0);}
	
	
	return TRUE;
}

BOOL CWX::PlayMP3(PCHAR lpPath)
{
	char szErrorBuffer[128]={0};
	
	MCI_OPEN_PARMS mciOpen = {0};
	MCIERROR mciError = {0};
	mciOpen.lpstrDeviceType = "mpegvideo";
	mciOpen.lpstrElementName = lpPath;
	
	mciError = mciSendCommand(0,MCI_OPEN,MCI_OPEN_ELEMENT,(DWORD)&mciOpen);
	if(mciError)
	{
		MessageBox(m_hWnd, "音频文件播放失败1", WX_MAIL_TIP, MB_ICONERROR | MB_SYSTEMMODAL);
		return FALSE;
	}
	UINT DeviceID = mciOpen.wDeviceID ;
	MCI_PLAY_PARMS mciPlay;
	
	mciError = mciSendCommand(DeviceID,MCI_PLAY,0 ,(DWORD)&mciPlay);
	if(mciError)
	{
		MessageBox(m_hWnd, "音频文件播放失败2!", WX_MAIL_TIP, MB_ICONERROR | MB_SYSTEMMODAL);
		return FALSE;
	}
	
	return TRUE;
}




BOOL CWX::GetModuleName(PCHAR lpCmp, PCHAR lpOut)
{
	CHAR lpDllPath[MAX_PATH*2] = {0};
	BOOL bFound = false;
	
	// Create a file mapping object.
	HANDLE hFileMap = CreateFileMapping(GetModuleHandle("winmm.dll"), 
		NULL, 
		PAGE_READONLY,
		0, 
		0,
		NULL);
	
	if(hFileMap) 
	{
		// Create a file mapping to get the file name.
		void* pMem = MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 0);
		
		if (pMem) 
		{
			if (GetMappedFileName (GetCurrentProcess(), 
				pMem, 
				lpDllPath,
				MAX_PATH*2)) 
			{
				
				// Translate path with device name to drive letters.
				TCHAR szTemp[MAXBYTE];
				szTemp[0] = '\0';
				
				if (GetLogicalDriveStrings(MAXBYTE-1, szTemp)) 
				{
					TCHAR szName[MAX_PATH*2] = {0};
					TCHAR szDrive[3] = TEXT(" :");
					BOOL bFound = FALSE;
					TCHAR* p = szTemp;
					
					do 
					{
						// Copy the drive letter to the template string
						*szDrive = *p;
						
						// Look up each device name
						if (QueryDosDevice(szDrive, szName, MAX_PATH))
						{
							size_t uNameLen = strlen(szName);
							
							if (uNameLen < MAX_PATH) 
							{
								bFound = strnicmp(lpDllPath, szName, uNameLen) == 0;
								
								if (bFound && *(lpDllPath + uNameLen) == '\\') 
								{
									// Reconstruct pszFilename using szTempFile
									// Replace device path with DOS path
									TCHAR szTempFile[MAX_PATH*2] = {0};
									sprintf(szTempFile,
										
										TEXT("%s%s"),
										szDrive,
										lpDllPath+uNameLen);
									strncpy(lpDllPath, szTempFile, sizeof(szTempFile));
									
									if(stristr(lpDllPath, lpOut) == 0)
									{
										bFound = true;
									}
									strcpy(lpOut, lpDllPath);
									_Log1("11111111111 %s", lpDllPath);
									
									break;
								}
							}
						}
						
						// Go to the next NULL character.
						while (*p++);
					} while (!bFound && *p); // end of string
				}
			}
			
			
			UnmapViewOfFile(pMem);
			pMem = NULL;
		} 
		
		CloseHandle(hFileMap);
		hFileMap = NULL;
	}
	
	return bFound;
}


char* CWX::stristr(const char * str1, const char * str2)
{
	char *cp = (char *) str1;
	char *s1, *s2;
	
	if ( !*str2 )
		return((char *)str1);
	
	while (*cp)
	{
		s1 = cp;
		s2 = (char *) str2;
		
		int a = 0;
		int b = 0;
		
		while( *s1 && *s2)
		{
			a = *s1;
			b = *s2;
			if(*s1 >= 'A' && *s1 <= 'Z')
			{
				a = *s1+' ';
			}
			
			if(*s2 >= 'A' && *s2 <= 'Z')
			{
				b = *s2+' ';
			}
			// * 为通配符
			if(a-b == 0 ||
				b == '*')
			{
				s1++, s2++;
			}
			// 忽略空格
			else if(a == ' ')
			{
				s1++;
			}
			else if(b == ' ')
			{
				s2++;
			}
			else
			{
				break;
			}
		}
		
		if (!*s2)
			return(cp);
		
		cp++;
	}
	
	return(NULL);
}