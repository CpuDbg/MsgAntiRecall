// CWXToolsDlg.h: interface for the CToolsDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLSDLG_H__1ACE2FB8_5AC5_4C31_B0AF_BE1CA5E8AE1E__INCLUDED_)
#define AFX_TOOLSDLG_H__1ACE2FB8_5AC5_4C31_B0AF_BE1CA5E8AE1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WXPublicData.h"


BOOL CALLBACK ToolsProc(HWND hwnd,UINT Msg,WPARAM wParam,LPARAM lParam);


class CWXToolsDlg  
{
public:
	void UpdateWindowPosition(int nTop = TRUE);
	void GetSafeData(PVOID lpSafeDataThis);
	CWXPublicData *m_lpPublicData;
	


	CWXToolsDlg();
	virtual ~CWXToolsDlg();

};

#endif // !defined(AFX_TOOLSDLG_H__1ACE2FB8_5AC5_4C31_B0AF_BE1CA5E8AE1E__INCLUDED_)
