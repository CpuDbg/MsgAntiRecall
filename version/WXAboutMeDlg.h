// AboutMeDlg.h: interface for the CWXAboutDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ABOUTMEDLG_H__CB74CC4F_18E9_4677_A12C_168A0AA04D16__INCLUDED_)
#define AFX_ABOUTMEDLG_H__CB74CC4F_18E9_4677_A12C_168A0AA04D16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "WXPublicData.h"
#include <stdio.h>



BOOL CALLBACK AboutProc(HWND hwnd,UINT Msg,WPARAM wParam,LPARAM lParam);




class CWXAboutDlg  
{
public:
	void UpdateWindowPosition(int nTop = TRUE);
	void GetSafeData(PVOID lpSafeThis);
	CWXPublicData *m_lpPublicData;


	CWXAboutDlg();
	virtual ~CWXAboutDlg();

};

#endif // !defined(AFX_ABOUTMEDLG_H__CB74CC4F_18E9_4677_A12C_168A0AA04D16__INCLUDED_)
