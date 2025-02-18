// WXPublicData.h: interface for the CWXPublicData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WXPUBLICDATA_H__0AE81887_0F8D_4D57_9440_4A9F13134588__INCLUDED_)
#define AFX_WXPUBLICDATA_H__0AE81887_0F8D_4D57_9440_4A9F13134588__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//#include "Sql/Sql.h"
#include "public.h"


#include <WinGDI.h>
#pragma comment(lib, "gdi32")



class CWXPublicData  
{
public:
	HWND m_hWndTools;
	HWND m_hWndSetting;
	HWND m_hWndAbout;
	HWND m_hWndToolsProc;
	
	HBITMAP m_hBitMapSetting1;
	HBITMAP m_hBitMapSetting2;
	HBITMAP m_hBitMapSetting3;

	HBITMAP m_hBitMapAbout1;
	HBITMAP m_hBitMapAbout2;
	HBITMAP m_hBitMapAbout3;

	HBITMAP m_hBitMapTools1;
	HBITMAP m_hBitMapTools2;
	HBITMAP m_hBitMapTools3;

	
	BOOL m_bToolsClick;
	BOOL m_bSettingClick;
	BOOL m_bAboutClick;
	
	HWND m_WxMailHwnd;
	
	BOOL m_bToolsMouseLeave;
	BOOL m_bSettingMouseLeave;
	BOOL m_bAboutMouseLeave;

	BOOL ModifyStyle(HWND hWnd, int nStyleOffset, DWORD dwRemove, DWORD dwAdd, UINT nFlags);


	// 获取系统DPI
	static int CWXPublicData::GetSystemDpi();
	// 逻辑坐标和物理坐标之间的转换
	static int CWXPublicData::DPI(int logical);

	CWXPublicData();
	virtual ~CWXPublicData();

};

#endif // !defined(AFX_WXPUBLICDATA_H__0AE81887_0F8D_4D57_9440_4A9F13134588__INCLUDED_)
