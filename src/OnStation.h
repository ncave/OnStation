// OnStation.h : main header file for the ONSTATION application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// COnStationApp:
// See OnStation.cpp for the implementation of this class
//

class CAppSettings;
class CViewSettings;
class COnStationApp : public CWinApp
{
private:
	void ShowTipAtStartup(void);
	void ShowTipOfTheDay(void);
	CAppSettings * m_MySettings;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	COnStationApp();

	inline CAppSettings * GetSettings() const {return m_MySettings;};
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COnStationApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(COnStationApp)
	afx_msg void OnAboutOnstation();
	afx_msg void OnHelpTechnicalsupport();
	afx_msg void OnHelpSummaryofshortcuts();
	afx_msg void OnHelpContents();
	afx_msg void OnHelpHowdoi();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
