#include "stdafx.h"
#include "clipboard.h"
#include "surveyshot.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern UINT uiCustomClipboardFormat_G;

BOOL CClipboard::StringToClipboard(CString& string,BOOL bCustomFormat)
{
	OpenClipboard(AfxGetMainWnd()->m_hWnd);
	if (EmptyClipboard()==FALSE)
		return FALSE;
	HGLOBAL hMem=GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE,(string.GetLength()+1) * sizeof(TCHAR));
	LPBYTE byMem=(LPBYTE)GlobalLock(hMem);
	lstrcpy((LPTSTR)byMem,(LPCTSTR)string);
	GlobalUnlock(hMem);
	if (bCustomFormat)
	{
		if (SetClipboardData(uiCustomClipboardFormat_G,hMem)==NULL)
		{
			return FALSE;
		}
	}
	else
	{
#ifdef _UNICODE
		if (SetClipboardData(CF_UNICODETEXT,hMem)==NULL)
#else
		if (SetClipboardData(CF_TEXT,hMem)==NULL)
#endif
		{
			return FALSE;
		}
	}
	CloseClipboard();
	return TRUE;
}

BOOL CClipboard::IsClipboardEmpty()
{
#ifdef _UNICODE
	return (!IsClipboardFormatAvailable(CF_UNICODETEXT));
#else
	return (!IsClipboardFormatAvailable(CF_TEXT));
#endif
}


BOOL CClipboard::StringFromClipboard(CString& string,BOOL bCustomFormat)
{
	OpenClipboard(AfxGetMainWnd()->m_hWnd);
	HANDLE hMem;
	if (bCustomFormat)
	{
		hMem=GetClipboardData(uiCustomClipboardFormat_G);
	}
	else
	{
#ifdef _UNICODE
		hMem=GetClipboardData(CF_UNICODETEXT);
#else
		hMem=GetClipboardData(CF_TEXT);
#endif
	}
	if (hMem==NULL)
		return FALSE;
	LPBYTE byMem=(LPBYTE)GlobalLock(hMem);
	string=(LPCTSTR)byMem;
	GlobalUnlock(hMem);
	CloseClipboard();
	return TRUE;
}
