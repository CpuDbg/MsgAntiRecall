// WXPublicData.cpp: implementation of the CWXPublicData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WXPublicData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWXPublicData::CWXPublicData()
{
// 	m_WxMailHwnd = ::FindWindow(WX_CLASSNAME, NULL);
// 	while(!m_WxMailHwnd)
// 	{
// 		LOG("没找到微信窗口 111");
// 		Sleep(3000);
// 	}
// 	LOG("已经找到微信窗口 111");

	m_bToolsMouseLeave = FALSE;
	m_bSettingMouseLeave = FALSE;
	m_bAboutMouseLeave = FALSE;
}

CWXPublicData::~CWXPublicData()
{

}


BOOL CWXPublicData::ModifyStyle(HWND hWnd, int nStyleOffset, DWORD dwRemove, DWORD dwAdd, UINT nFlags)
{
	DWORD dwStyle = ::GetWindowLongPtr(hWnd, nStyleOffset);
	DWORD dwNewStyle = (dwStyle & ~dwRemove) | dwAdd;
	if (dwStyle == dwNewStyle)
		return FALSE;
	
	::SetWindowLong(hWnd, nStyleOffset, dwNewStyle);
	if (nFlags != 0)
	{
		::SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | nFlags);
	}
	return TRUE;
}


// 获取系统DPI
int CWXPublicData::GetSystemDpi()
{
	HDC screen = GetDC(NULL);
	int dpiX = GetDeviceCaps(screen, LOGPIXELSX);
	//	int dpiY = GetDeviceCaps(screen, LOGPIXELSY);
	ReleaseDC(NULL, screen);
	return dpiX;
}

// 逻辑坐标和物理坐标之间的转换
int CWXPublicData::DPI(int logical)
{
	return MulDiv(logical, GetSystemDpi(), 96);
}