// SettingsDlg.h: interface for the CWXSettingsDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SETTINGSDLG_H__257D5639_4B35_433B_8A7E_9856848A6A60__INCLUDED_)
#define AFX_SETTINGSDLG_H__257D5639_4B35_433B_8A7E_9856848A6A60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "WXPublicData.h"



BOOL CALLBACK SettingsProc(HWND hwnd,UINT Msg,WPARAM wParam,LPARAM lParam);

class CWXSettingsDlg  
{
public:
	void UpdateWindowPosition(int nTop = TRUE);
	void GetSafeData(PVOID lpSafeThis);
	CWXPublicData *m_lpPublicData;
	HINSTANCE m_hInstance;
	

	CWXSettingsDlg();
	virtual ~CWXSettingsDlg();

};

#endif // !defined(AFX_SETTINGSDLG_H__257D5639_4B35_433B_8A7E_9856848A6A60__INCLUDED_)
