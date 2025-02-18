// SettingsDlg.cpp: implementation of the CWXSettingsDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "res/resource.h"
#include "WXSettingsDlg.h"
#include "WX.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CWXSettingsDlg *lpThisSettings;
extern CWX wx;

CWXSettingsDlg::CWXSettingsDlg()
{
	lpThisSettings = this;
	m_hInstance = (HINSTANCE)GetModuleHandle(NULL);
}

CWXSettingsDlg::~CWXSettingsDlg()
{
	
}



bool g_bWxDialogShow = FALSE;
WINDOWPLACEMENT g_wp = {0};
BOOL CALLBACK DlgSettingProc(HWND hwnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{
	switch(Msg)
	{
	case WM_LBUTTONDOWN:
		{
			PostMessage(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(wParam, lParam));
			break;
		}
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		{
			int x = 0;
			break;
		}
	case WM_CLOSE:
		{
			//EndDialog(hwnd, 0);
			SendMessage(hwnd, WM_SHOWWINDOW, FALSE, SW_OTHERUNZOOM);
			break;
		}
	case WM_INITDIALOG:
		{
			_Log0("WM_INITDIALOG");
			
			// 让窗口居中
			wx.CenterWindowOnScreen(hwnd, 1);
			
			wx.InitData(hwnd);
			
			_Log0("WM_INITDIALOG 中读取配置文件一次!");
			wx.ReadSettings();
			
			
			// 设置static版本提示
			HWND hStatic = GetDlgItem(hwnd, STATIC_TIP);
			SendMessage(hStatic, WM_SETTEXT, NULL, (LPARAM)WX_MAIL_TIP);
			
			
			//			CreateThread(NULL, 0, test, NULL, 0, 0);
			
			
			return FALSE;
		}
		
	case WM_COMMAND:
		{
			
			WORD wmId = LOWORD(wParam);
			// 这里是拦截 VK_ESCAPE 回头得研究下
			if(wmId == 2)
			{
				// 获取窗口大小和位置
				EndDialog(hwnd, wParam);
				break;
			}
			
			switch(wParam)
			{
			case BTN_WX_MAIN_EXIT:
				{
					SendMessage(hwnd, WM_SHOWWINDOW, FALSE, SW_OTHERZOOM);
					g_bWxDialogShow = false;
					break;
				}
				
				// 		case BTN_DISPLAY:
				// 			{
				// 				DialogBox(wx.m_hInstance, MAKEINTRESOURCE(DIALOG_HOTKEY), hwnd, (DLGPROC)HotKeySettingProc);
				// 				break;
				// 			}
				
			case BTN_SETTING_SAVE:
				{
					wx.SaveSettings();
					break;
				}
				
			case BTN_SETTING_READ:
				{
					_Log0("BTN_SETTING_READ 中读取配置文件一次!");
					wx.ReadSettings();
					break;
				}
				// 红包提醒
			case CHECK_MONEY_TIP:
				{
					if(SendMessage((HWND)lParam, BM_GETCHECK, 0, 0) == BST_CHECKED)
					{
						wx.m_bNormalMoneyTip = TRUE;
					}
					else
					{
						wx.m_bNormalMoneyTip = FALSE;
					}
					
					break;
				}
				// 语音提醒
			case CHECK_VOICE_TIP:
				{
					if(SendMessage((HWND)lParam, BM_GETCHECK, 0, 0) == BST_CHECKED)
					{
						wx.m_bNormalVoiceTip = TRUE;
					}
					else
					{
						wx.m_bNormalVoiceTip = FALSE;
					}
					
					break;
				}
				// WX多开
			case CHECK_WXCLIENT_OPEN:
				{
					if(SendMessage((HWND)lParam, BM_GETCHECK, 0, 0) == BST_CHECKED)
					{
						//					WXSubClass();
						wx.m_bNormalClientOpen = TRUE;
					}
					else
					{
						wx.m_bNormalClientOpen = FALSE;
					}
					break;
				}
		}
		}
		break;
		default:
			int x = 0;
	}
	
	return FALSE;
}



WINDOWPLACEMENT g_wpSettings = {0};
BOOL CALLBACK SettingsProc(HWND hwnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{
	switch(Msg)
	{
	case WM_LBUTTONDOWN:
		{
			_Log0("SettingsProc 设置 鼠标按下");
			SendDlgItemMessage(lpThisSettings->m_lpPublicData->m_hWndSetting, STATIC_BITMAP_SETTINGS, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)lpThisSettings->m_lpPublicData->m_hBitMapSetting3);
			SetCapture(lpThisSettings->m_lpPublicData->m_hWndSetting);
			break;
		}
	case WM_LBUTTONUP:
		{
			_Log0("SettingsProc 设置 鼠标弹起");			
			
			lpThisSettings->m_lpPublicData->m_bToolsClick = FALSE;
			lpThisSettings->m_lpPublicData->m_bSettingClick = TRUE;
			lpThisSettings->m_lpPublicData->m_bAboutClick = FALSE;
			SendDlgItemMessage(lpThisSettings->m_lpPublicData->m_hWndTools, STATIC_BITMAP_TOOLS, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)lpThisSettings->m_lpPublicData->m_hBitMapTools1);
			SendDlgItemMessage(lpThisSettings->m_lpPublicData->m_hWndSetting, STATIC_BITMAP_SETTINGS, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)lpThisSettings->m_lpPublicData->m_hBitMapSetting2);
			SendDlgItemMessage(lpThisSettings->m_lpPublicData->m_hWndAbout, STATIC_BITMAP_ABOUT, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)lpThisSettings->m_lpPublicData->m_hBitMapAbout1);
			
			lpThisSettings->m_lpPublicData->m_bToolsMouseLeave = FALSE;
			lpThisSettings->m_lpPublicData->m_bSettingMouseLeave = TRUE;
			lpThisSettings->m_lpPublicData->m_bAboutMouseLeave = FALSE;
			
			
			ReleaseCapture();
			if(lpThisSettings->m_lpPublicData->m_hWndToolsProc != false)
			{
				// 窗口位置
				WINDOWPLACEMENT tagGoToWP = {0};
				GetWindowPlacement(lpThisSettings->m_lpPublicData->m_hWndToolsProc, &tagGoToWP);
				


				if(IsWindowVisible(lpThisSettings->m_lpPublicData->m_hWndToolsProc))
				{
					_Log0("显示");
					ShowWindow(lpThisSettings->m_lpPublicData->m_hWndToolsProc, SW_HIDE);


				}
				else
				{
					_Log0("不显示");
					
					// 让窗口居中
					wx.CenterWindowOnScreen(lpThisSettings->m_lpPublicData->m_hWndToolsProc, 1);
					
					ShowWindow(lpThisSettings->m_lpPublicData->m_hWndToolsProc, SW_SHOW);
				}

				_Log4("消息: left: %08x right: %08x top: %08x bottom: %08x", 
					tagGoToWP.rcNormalPosition.left,
					tagGoToWP.rcNormalPosition.right,
					tagGoToWP.rcNormalPosition.top,
					tagGoToWP.rcNormalPosition.bottom);
				
				
// 				if(tagGoToWP.showCmd == SW_HIDE || tagGoToWP.showCmd == SW_SHOWNORMAL)
// 					ShowWindow(hToolsHwnd, SW_SHOW);
// 				else if(tagGoToWP.showCmd == SW_SHOW)
// 					ShowWindow(hToolsHwnd, SW_HIDE);


				break;
			}
			
			//	DialogBox(wx.m_hInstance, MAKEINTRESOURCE(DIALOG_WX), NULL, (DLGPROC)DlgSettingProc);
			lpThisSettings->m_lpPublicData->m_hWndToolsProc = CreateDialog(wx.m_hInstance, MAKEINTRESOURCE(DIALOG_WX), NULL, (DLGPROC)DlgSettingProc);
			ShowWindow(lpThisSettings->m_lpPublicData->m_hWndToolsProc, SW_SHOW);
			
			
			MSG msg = {0};
			while (GetMessage (&msg, NULL, 0, 0))
			{
				if(lpThisSettings->m_lpPublicData->m_hWndToolsProc == 0 || !IsDialogMessage(lpThisSettings->m_lpPublicData->m_hWndToolsProc, &msg))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			
			break;
		}
	case WM_MOUSELEAVE:
		{
		//	if(!lpThisSettings->m_lpPublicData->m_bSettingClick)
			{
				_Log0("SettingsProc 设置 鼠标离开");
				SendDlgItemMessage(lpThisSettings->m_lpPublicData->m_hWndSetting, STATIC_BITMAP_SETTINGS, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)lpThisSettings->m_lpPublicData->m_hBitMapSetting1);
				lpThisSettings->m_lpPublicData->m_bSettingMouseLeave = FALSE;
			}
			
			break;
		}
	case WM_MOUSEMOVE:
		{
			if(!lpThisSettings->m_lpPublicData->m_bSettingMouseLeave)
			{
				_Log0("SettingsProc 设置 鼠标移动");
				TRACKMOUSEEVENT tme;
				tme.cbSize = sizeof (tme);
				tme.dwFlags = TME_LEAVE;
				tme.dwHoverTime = HOVER_DEFAULT;
				tme.hwndTrack = hwnd;
				lpThisSettings->m_lpPublicData->m_bSettingMouseLeave = TrackMouseEvent(&tme);
				
				SendDlgItemMessage(lpThisSettings->m_lpPublicData->m_hWndSetting, STATIC_BITMAP_SETTINGS, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)lpThisSettings->m_lpPublicData->m_hBitMapSetting2);
			}
			
			break;
		}
	case WM_INITDIALOG:
		{
			_Log0("设置窗口初始化完毕");
			
			lpThisSettings->m_lpPublicData->m_hWndSetting = hwnd;
			
			lpThisSettings->UpdateWindowPosition();
			
			lpThisSettings->m_lpPublicData->m_hBitMapSetting1 = LoadBitmap((HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), MAKEINTRESOURCE(BITMAP_SETTING1));
			lpThisSettings->m_lpPublicData->m_hBitMapSetting2 = LoadBitmap((HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), MAKEINTRESOURCE(BITMAP_SETTING2));
			lpThisSettings->m_lpPublicData->m_hBitMapSetting3 = LoadBitmap((HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), MAKEINTRESOURCE(BITMAP_SETTING3));
			
			//	SendMessage(hStaticBitMapHwnd, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)lpThisSettings->m_lpPublicData->m_hBitMapSetting1);
			SendDlgItemMessage(hwnd, STATIC_BITMAP_SETTINGS, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)lpThisSettings->m_lpPublicData->m_hBitMapSetting1);
			
			//////////////////////////////////////////////////////////////////////////
// 			WINDOWPLACEMENT wp = {0};
// 			wp.length = sizeof(WINDOWPLACEMENT);
// 			GetWindowPlacement(hwnd, &g_wpSettings);
// 			wp.flags=WPF_RESTORETOMAXIMIZED;
// 			wp.showCmd=SW_HIDE;
// 			SetWindowPlacement(hwnd, &wp);
// 			lpThisSettings->m_lpPublicData->ModifyStyle(hwnd, GWL_EXSTYLE, WS_EX_APPWINDOW,WS_EX_TOOLWINDOW, 1);
// 			
// 			wp.flags=WPF_RESTORETOMAXIMIZED;
// 			wp.showCmd=SW_SHOW;
// 			SetWindowPlacement(hwnd, &g_wpSettings);
			//////////////////////////////////////////////////////////////////////////
			
			SetParent(hwnd, lpThisSettings->m_lpPublicData->m_WxMailHwnd);
			
			break;
		}
	case WM_CLOSE:
		{
			//EndDialog(hwnd, 0);
			SendMessage(hwnd, WM_SHOWWINDOW, FALSE, SW_OTHERUNZOOM);
			break;
		}
		// 移动窗口
	case WM_USER+100:
		{
			lpThisSettings->UpdateWindowPosition();
			break;
		}
		// 失去焦点
	case WM_USER+101:
		{
			lpThisSettings->UpdateWindowPosition(FALSE);
			break;
		}
		// 获取焦点
	case WM_USER+102:
		{
			lpThisSettings->UpdateWindowPosition();
			break;
		}
	}
	
	return FALSE;
}

void CWXSettingsDlg::GetSafeData(PVOID lpSafeThis)
{
	m_lpPublicData = (CWXPublicData*)lpSafeThis;
}

void CWXSettingsDlg::UpdateWindowPosition(int nTop)
{
	::SetWindowPos(lpThisSettings->m_lpPublicData->m_hWndSetting, HWND_TOPMOST, CWXPublicData::DPI(WX_POSITION_LEFT), CWXPublicData::DPI(WX_POSITION_TOP+WX_POSITION_SPACE), -1, -1, SWP_NOSIZE | SWP_NOZORDER);
}