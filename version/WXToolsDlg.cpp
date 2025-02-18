// ToolsDlg.cpp: implementation of the CWXToolsDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "res/resource.h"
#include "WXToolsDlg.h"
#include "WX.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CWXToolsDlg *lpThisTools = NULL;

CWXToolsDlg::CWXToolsDlg()
{
	lpThisTools = this;
}

CWXToolsDlg::~CWXToolsDlg()
{

}



WNDPROC wpOrigWXBitMapProc;

LRESULT APIENTRY WXSubBitMapProc(
								HWND hwnd, 
								UINT uMsg, 
								WPARAM wParam, 
								LPARAM lParam) 
{
    switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		{
			_Log0("���໯ WXSubBitMapProc ���� ��갴��");
			SendDlgItemMessage(lpThisTools->m_lpPublicData->m_hWndTools, STATIC_BITMAP_TOOLS, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)lpThisTools->m_lpPublicData->m_hBitMapTools3);
			SetCapture(lpThisTools->m_lpPublicData->m_hWndTools);
			break;
		}
	case WM_LBUTTONUP:
		{
			_Log0("���໯ WXSubBitMapProc ���� ��굯��");

			ReleaseCapture();

			lpThisTools->m_lpPublicData->m_bToolsClick = TRUE;
			lpThisTools->m_lpPublicData->m_bSettingClick = FALSE;
			lpThisTools->m_lpPublicData->m_bAboutClick = FALSE;
			SendDlgItemMessage(lpThisTools->m_lpPublicData->m_hWndTools, STATIC_BITMAP_TOOLS, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)lpThisTools->m_lpPublicData->m_hBitMapTools2);
			SendDlgItemMessage(lpThisTools->m_lpPublicData->m_hWndSetting, STATIC_BITMAP_SETTINGS, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)lpThisTools->m_lpPublicData->m_hBitMapSetting1);
			SendDlgItemMessage(lpThisTools->m_lpPublicData->m_hWndAbout, STATIC_BITMAP_ABOUT, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)lpThisTools->m_lpPublicData->m_hBitMapAbout1);
			
			lpThisTools->m_lpPublicData->m_bToolsMouseLeave = TRUE;
			lpThisTools->m_lpPublicData->m_bSettingMouseLeave = FALSE;
			lpThisTools->m_lpPublicData->m_bAboutMouseLeave = FALSE;

			// bjbl
			MsgBox(lpThisTools->m_lpPublicData->m_hWndTools, "�����ǹ��߰�ť, ������...", WX_MAIL_TIP, MB_ICONINFORMATION);

			break;
		}
	case WM_MOUSELEAVE:
		{
		//	if(!lpThisTools->m_lpPublicData->m_bToolsClick)
			{
				_Log0("���໯ WXSubBitMapProc ���� ����뿪");
				SendDlgItemMessage(lpThisTools->m_lpPublicData->m_hWndTools, STATIC_BITMAP_TOOLS, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)lpThisTools->m_lpPublicData->m_hBitMapTools1);
				lpThisTools->m_lpPublicData->m_bToolsMouseLeave = FALSE;
			}
			
			break;
		}
	case WM_MOUSEMOVE:
		{
			if(!lpThisTools->m_lpPublicData->m_bToolsMouseLeave)
			{
				_Log0("���໯ WXSubBitMapProc ���� ����ƶ�");
				TRACKMOUSEEVENT tme;
				tme.cbSize = sizeof (tme);
				tme.dwFlags = TME_LEAVE;
				tme.dwHoverTime = HOVER_DEFAULT;
				tme.hwndTrack = hwnd;
				lpThisTools->m_lpPublicData->m_bToolsMouseLeave = TrackMouseEvent(&tme);
				
//				SendMessage(hwnd, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)lpThisTools->m_lpPublicData->m_hBitMapTools2);
				SendDlgItemMessage(lpThisTools->m_lpPublicData->m_hWndTools, STATIC_BITMAP_TOOLS, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)lpThisTools->m_lpPublicData->m_hBitMapTools2);
			}
			


//			SelectObject()


			break;
		}
	}

	return CallWindowProc(wpOrigWXBitMapProc, hwnd, uMsg, wParam, lParam); 
} 

WINDOWPLACEMENT g_wpTools = {0};
BOOL CALLBACK ToolsProc(HWND hwnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{
	HWND hSubHwnd = GetDlgItem(lpThisTools->m_lpPublicData->m_hWndTools, IMAGE_BITMAP);

	switch(Msg)
	{
	case WM_LBUTTONDOWN:
		{
			_Log0("ToolsProc ���� ��갴��");
			SendMessage(hSubHwnd, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)lpThisTools->m_lpPublicData->m_hBitMapTools3);
			SendDlgItemMessage(lpThisTools->m_lpPublicData->m_hWndTools, STATIC_BITMAP_TOOLS, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)lpThisTools->m_lpPublicData->m_hBitMapTools3);
			SetCapture(lpThisTools->m_lpPublicData->m_hWndTools);
			break;
		}
	case WM_LBUTTONUP:
		{
			_Log0("ToolsProc ���� ��굯��");
			lpThisTools->m_lpPublicData->m_bToolsClick = TRUE;
			lpThisTools->m_lpPublicData->m_bSettingClick = FALSE;
			lpThisTools->m_lpPublicData->m_bAboutClick = FALSE;
			SendDlgItemMessage(lpThisTools->m_lpPublicData->m_hWndTools, STATIC_BITMAP_TOOLS, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)lpThisTools->m_lpPublicData->m_hBitMapTools3);
			SendDlgItemMessage(lpThisTools->m_lpPublicData->m_hWndSetting, STATIC_BITMAP_SETTINGS, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)lpThisTools->m_lpPublicData->m_hBitMapSetting1);
			SendDlgItemMessage(lpThisTools->m_lpPublicData->m_hWndAbout, STATIC_BITMAP_ABOUT, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)lpThisTools->m_lpPublicData->m_hBitMapAbout1);
			
			lpThisTools->m_lpPublicData->m_bToolsMouseLeave = TRUE;
			lpThisTools->m_lpPublicData->m_bSettingMouseLeave = FALSE;
			lpThisTools->m_lpPublicData->m_bAboutMouseLeave = FALSE;
			
			// bjbl
			MsgBox(lpThisTools->m_lpPublicData->m_hWndTools, "�����ǹ��߰�ť, ������...", WX_MAIL_TIP, MB_ICONINFORMATION);

			ReleaseCapture();
			break;
		}
	case WM_MOUSELEAVE:
		{
		//	if(!lpThisTools->m_lpPublicData->m_bToolsClick)
			{
				_Log0("ToolsProc ���� ����뿪");
				SendDlgItemMessage(lpThisTools->m_lpPublicData->m_hWndTools, STATIC_BITMAP_TOOLS, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)lpThisTools->m_lpPublicData->m_hBitMapTools1);
				lpThisTools->m_lpPublicData->m_bToolsMouseLeave = FALSE;
			}
			
			break;
		}
	case WM_MOUSEMOVE:
		{
			if(!lpThisTools->m_lpPublicData->m_bToolsMouseLeave)
			{
				_Log0("ToolsProc ���� ����ƶ�");
				TRACKMOUSEEVENT tme;
				tme.cbSize = sizeof (tme);
				tme.dwFlags = TME_LEAVE;
				tme.dwHoverTime = HOVER_DEFAULT;
				tme.hwndTrack = hwnd;
				lpThisTools->m_lpPublicData->m_bToolsMouseLeave = TrackMouseEvent(&tme);
				
				SendMessage(hSubHwnd, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)lpThisTools->m_lpPublicData->m_hBitMapTools2);
				SendDlgItemMessage(lpThisTools->m_lpPublicData->m_hWndTools, STATIC_BITMAP_TOOLS, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)lpThisTools->m_lpPublicData->m_hBitMapTools2);
			}
			
			break;
		}
	case WM_INITDIALOG:
		{
			_Log0("���ߴ��ڳ�ʼ�����");

			lpThisTools->m_lpPublicData->m_hWndTools = hwnd;
			
			lpThisTools->UpdateWindowPosition();
			
			lpThisTools->m_lpPublicData->m_hBitMapTools1 = LoadBitmap((HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), MAKEINTRESOURCE(BITMAP_TOOLS1));
			lpThisTools->m_lpPublicData->m_hBitMapTools2 = LoadBitmap((HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), MAKEINTRESOURCE(BITMAP_TOOLS2));
			lpThisTools->m_lpPublicData->m_hBitMapTools3 = LoadBitmap((HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), MAKEINTRESOURCE(BITMAP_TOOLS3));
			
			SendDlgItemMessage(hwnd, STATIC_BITMAP_TOOLS, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)lpThisTools->m_lpPublicData->m_hBitMapTools1);

			//////////////////////////////////////////////////////////////////////////
			WINDOWPLACEMENT wp = {0};
			wp.length = sizeof(WINDOWPLACEMENT);
			GetWindowPlacement(hwnd, &g_wpTools);
			wp.flags=WPF_RESTORETOMAXIMIZED;
			wp.showCmd=SW_HIDE;

			wp.ptMaxPosition.x = CWXPublicData::DPI(wp.ptMaxPosition.x);
			wp.ptMaxPosition.y = CWXPublicData::DPI(wp.ptMaxPosition.y);
			wp.ptMinPosition.x = CWXPublicData::DPI(wp.ptMinPosition.x);
			wp.ptMinPosition.y = CWXPublicData::DPI(wp.ptMinPosition.y);

			SetWindowPlacement(hwnd, &wp);
			lpThisTools->m_lpPublicData->ModifyStyle(hwnd, GWL_EXSTYLE, WS_EX_APPWINDOW,WS_EX_TOOLWINDOW, 1);
			
			wp.flags=WPF_RESTORETOMAXIMIZED;
			wp.showCmd=SW_SHOW;

			g_wpTools.ptMaxPosition.x = CWXPublicData::DPI(g_wpTools.ptMaxPosition.x);
			g_wpTools.ptMaxPosition.y = CWXPublicData::DPI(g_wpTools.ptMaxPosition.y);
			g_wpTools.ptMinPosition.x = CWXPublicData::DPI(g_wpTools.ptMinPosition.x);
			g_wpTools.ptMinPosition.y = CWXPublicData::DPI(g_wpTools.ptMinPosition.y);

			SetWindowPlacement(hwnd, &g_wpTools);
			//////////////////////////////////////////////////////////////////////////

			SetParent(hwnd, lpThisTools->m_lpPublicData->m_WxMailHwnd);
			

			
			// ���໯һ��
// 			wpOrigWXBitMapProc = (WNDPROC)GetWindowLongPtr(hwnd, GWL_WNDPROC);
// 			SetWindowLong(hwnd, GWL_WNDPROC, (LONG)WXSubBitMapProc);


			break;
		}
	case WM_CLOSE:
		{
			//EndDialog(hwnd, 0);
			SendMessage(hwnd, WM_SHOWWINDOW, FALSE, SW_OTHERUNZOOM);
			break;
		}
		// �ƶ�����
	case WM_USER+100:
		{
//			lpThisTools->UpdateWindowPosition();
			break;
		}
		// ʧȥ����
	case WM_USER+101:
		{
//			lpThisTools->UpdateWindowPosition(FALSE);
			break;
		}
		// ��ȡ����
	case WM_USER+102:
		{
//			lpThisTools->UpdateWindowPosition();
			break;
		}
	}
	
	return FALSE;
}

void CWXToolsDlg::GetSafeData(PVOID lpSafeDataThis)
{
	m_lpPublicData = (CWXPublicData*)lpSafeDataThis;
}

void CWXToolsDlg::UpdateWindowPosition(int nTop)
{
	::SetWindowPos(lpThisTools->m_lpPublicData->m_hWndTools, HWND_TOPMOST, CWXPublicData::DPI(WX_POSITION_LEFT), CWXPublicData::DPI(WX_POSITION_TOP), -1, -1, SWP_NOSIZE | SWP_NOZORDER);
}
