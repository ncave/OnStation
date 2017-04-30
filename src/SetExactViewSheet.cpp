// SetExactViewSheet.cpp : implementation file
//

#include "stdafx.h"
#include "OnStation.h"
#include "SetExactViewSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetExactViewSheet

IMPLEMENT_DYNAMIC(CSetExactViewSheet, CPropertySheet)

CSetExactViewSheet::CSetExactViewSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage),m_ViewPage(0.0f,0.0f),m_ViewScale(0.0f)
{
	ASSERT(FALSE);		//do not use this one

}

CSetExactViewSheet::CSetExactViewSheet(COnStationView *view,LPCTSTR pszCaption,float fClock,float fTilt,float fScale,
											float fDx,float fDy,float fDz, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage),m_ViewPage(fClock,fTilt),m_ViewCenter(fDx,fDy,fDz),m_ViewScale(fScale)
{
	m_fClockwise=fClock;
	m_fTilt=fTilt;
	m_fScaling=fScale;
	m_fDx=fDx;
	m_fDy=fDy;
	m_fDz=fDz;
	AddPage(&m_ViewPage);
	AddPage(&m_ViewCenter);
	AddPage(&m_ViewScale);
//	This is how we handle the APPLY NOW case
	m_ViewCenter.Initialize(view);
}

CSetExactViewSheet::~CSetExactViewSheet()
{
}


BEGIN_MESSAGE_MAP(CSetExactViewSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CSetExactViewSheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetExactViewSheet message handlers
