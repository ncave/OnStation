// AboutSheet.cpp : implementation file
//

#include "stdafx.h"
#include "OnStation.h"
#include "AboutSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutSheet

IMPLEMENT_DYNAMIC(CAboutSheet, CPropertySheet)

CAboutSheet::CAboutSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_SummaryPage);
	AddPage(&m_CreditsPage);
	AddPage(&m_CopyrightPage);	//copyright.
	AddPage(&m_AboutWebPage);
}

CAboutSheet::CAboutSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_SummaryPage);
	AddPage(&m_CreditsPage);
	AddPage(&m_CopyrightPage);	//copyright.
	AddPage(&m_AboutWebPage);
}

CAboutSheet::~CAboutSheet()
{
}


BEGIN_MESSAGE_MAP(CAboutSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CAboutSheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutSheet message handlers
