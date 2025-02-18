// AboutMeDlg.cpp: implementation of the CWXAboutDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "res/resource.h"
#include "WXAboutMeDlg.h"
#include "WX.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWXAboutDlg *lpThisAbout = NULL;

CWXAboutDlg::CWXAboutDlg()
{
	lpThisAbout = this;
}

CWXAboutDlg::~CWXAboutDlg()
{

}



WINDOWPLACEMENT g_wpAbout = {0};
BOOL CALLBACK AboutProc(HWND hwnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{
	switch(Msg)
	{
	case WM_LBUTTONDOWN:
		{	
			_Log0("AboutProc 关于 鼠标按下");
			SendDlgItemMessage(lpThisAbout->m_lpPublicData->m_hWndAbout, STATIC_BITMAP_ABOUT, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)lpThisAbout->m_lpPublicData->m_hBitMapAbout3);		
			SetCapture(lpThisAbout->m_lpPublicData->m_hWndAbout);
			break;
		}
	case WM_LBUTTONUP:
		{
			_Log0("AboutProc 关于 鼠标弹起");

			ReleaseCapture();
			
			lpThisAbout->m_lpPublicData->m_bToolsClick = FALSE;
			lpThisAbout->m_lpPublicData->m_bSettingClick = FALSE;
			lpThisAbout->m_lpPublicData->m_bAboutClick = TRUE;
 			SendDlgItemMessage(lpThisAbout->m_lpPublicData->m_hWndTools, STATIC_BITMAP_TOOLS, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)lpThisAbout->m_lpPublicData->m_hBitMapTools1);
 			SendDlgItemMessage(lpThisAbout->m_lpPublicData->m_hWndSetting, STATIC_BITMAP_SETTINGS, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)lpThisAbout->m_lpPublicData->m_hBitMapSetting1);
 			SendDlgItemMessage(lpThisAbout->m_lpPublicData->m_hWndAbout, STATIC_BITMAP_ABOUT, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)lpThisAbout->m_lpPublicData->m_hBitMapAbout2);
			
			lpThisAbout->m_lpPublicData->m_bToolsMouseLeave = FALSE;
			lpThisAbout->m_lpPublicData->m_bSettingMouseLeave = FALSE;
			lpThisAbout->m_lpPublicData->m_bAboutMouseLeave = TRUE;

			MsgBox(lpThisAbout->m_lpPublicData->m_hWndAbout, "感谢叶一设计的皮肤", WX_MAIL_TIP, MB_ICONINFORMATION);
			
			break;
		}
	case WM_MOUSELEAVE:
		{
		//	if(!lpThisAbout->m_lpPublicData->m_bAboutClick)
			{
				_Log0("AboutProc 关于 鼠标离开");
				SendDlgItemMessage(lpThisAbout->m_lpPublicData->m_hWndAbout, STATIC_BITMAP_ABOUT, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)lpThisAbout->m_lpPublicData->m_hBitMapAbout1);
				lpThisAbout->m_lpPublicData->m_bAboutMouseLeave = FALSE;
			}
			
			break;
		}
	case WM_MOUSEMOVE:
		{
			if(!lpThisAbout->m_lpPublicData->m_bAboutMouseLeave)
			{
				_Log0("AboutProc 关于 鼠标移动");
				TRACKMOUSEEVENT tme;
				tme.cbSize = sizeof (tme);
				tme.dwFlags = TME_LEAVE;
				tme.dwHoverTime = HOVER_DEFAULT;
				tme.hwndTrack = hwnd;
				lpThisAbout->m_lpPublicData->m_bAboutMouseLeave = TrackMouseEvent(&tme);
				
				
				SendDlgItemMessage(lpThisAbout->m_lpPublicData->m_hWndAbout, STATIC_BITMAP_ABOUT, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)lpThisAbout->m_lpPublicData->m_hBitMapAbout2);
			}
			
			break;
		}
	case WM_INITDIALOG:
		{
			_Log0("关于窗口初始化完毕");

			lpThisAbout->m_lpPublicData->m_hWndAbout = hwnd;

			lpThisAbout->UpdateWindowPosition();
			
			
//			HWND hStaticBitMapHwnd = GetDlgItem(hwnd, STATIC_BITMAP_ABOUT);
			//			DWORD dwNewStype = ModifyStyle(hStaticBitMapHwnd, GWL_STYLE, 0, BS_BITMAP, 0);
			
			
			lpThisAbout->m_lpPublicData->m_hBitMapAbout1 = LoadBitmap((HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), MAKEINTRESOURCE(BITMAP_ABOUT1));
			lpThisAbout->m_lpPublicData->m_hBitMapAbout2 = LoadBitmap((HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), MAKEINTRESOURCE(BITMAP_ABOUT2));
			lpThisAbout->m_lpPublicData->m_hBitMapAbout3 = LoadBitmap((HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), MAKEINTRESOURCE(BITMAP_ABOUT3));
			
//			SendMessage(hStaticBitMapHwnd, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)lpThisAbout->m_lpPublicData->m_hBitMapAbout1);
			SendDlgItemMessage(hwnd, STATIC_BITMAP_ABOUT, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)lpThisAbout->m_lpPublicData->m_hBitMapAbout1);
			


			//////////////////////////////////////////////////////////////////////////
			WINDOWPLACEMENT wp = {0};
			wp.length = sizeof(WINDOWPLACEMENT);
			GetWindowPlacement(hwnd, &g_wpAbout);
			wp.flags=WPF_RESTORETOMAXIMIZED;
			wp.showCmd=SW_HIDE;
			SetWindowPlacement(hwnd, &wp);
			lpThisAbout->m_lpPublicData->ModifyStyle(hwnd, GWL_EXSTYLE, WS_EX_APPWINDOW,WS_EX_TOOLWINDOW, 1);
			
			wp.flags=WPF_RESTORETOMAXIMIZED;
			wp.showCmd=SW_SHOW;
			SetWindowPlacement(hwnd, &g_wpAbout);
			//////////////////////////////////////////////////////////////////////////

			char szTemp[MAXBYTE] = {0};
			sprintf(szTemp, "%08x  -  %08x", hwnd, lpThisAbout->m_lpPublicData->m_WxMailHwnd);
			_Log0(szTemp);
			if(SetParent(hwnd, lpThisAbout->m_lpPublicData->m_WxMailHwnd))
			{
				_Log0("父窗口设置成功!");
			}
			else
			{
				_Log0("父窗口设置失败!");
			}

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
			lpThisAbout->UpdateWindowPosition();
			break;
		}
		// 失去焦点
	case WM_USER+101:
		{
			lpThisAbout->UpdateWindowPosition(FALSE);
			break;
		}
		// 获取焦点
	case WM_USER+102:
		{
			lpThisAbout->UpdateWindowPosition();
			break;
		}
	}
	
	return FALSE;
}

void CWXAboutDlg::GetSafeData(PVOID lpSafeThis)
{
	m_lpPublicData = (CWXPublicData*)lpSafeThis;
}



void CWXAboutDlg::UpdateWindowPosition(int nTop)
{
	::SetWindowPos(lpThisAbout->m_lpPublicData->m_hWndAbout, HWND_TOPMOST, CWXPublicData::DPI(WX_POSITION_LEFT), CWXPublicData::DPI(WX_POSITION_TOP+WX_POSITION_SPACE*2),-1,-1, SWP_NOSIZE | SWP_NOZORDER);
}