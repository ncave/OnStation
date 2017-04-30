// surfacesheet.cpp : implementation file
//

#include "stdafx.h"
#include "onstation.h"
#include "surfacepropertysheet.h"
#include "onstationdoc.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSurfacePropertySheet

IMPLEMENT_DYNAMIC(CSurfacePropertySheet, CPropertySheet)

CSurfacePropertySheet::CSurfacePropertySheet(COnStationDoc *document,UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	m_MyDocument=document;
	SetupPages();
}

CSurfacePropertySheet::CSurfacePropertySheet(COnStationDoc *document,LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	m_MyDocument=document;
	SetupPages();
}

void CSurfacePropertySheet::SetupPages()
{
	m_SurfaceSizePage.Initialize(m_MyDocument->GetSurface());
	m_SurfaceEditPage.Initialize(m_MyDocument->GetSurface());
	AddPage(&m_SurfaceSizePage);
	AddPage(&m_SurfaceEditPage);
}

BOOL CSurfacePropertySheet::NeedRecalculation()
{
	if (m_SurfaceSizePage.m_bDirty)
		return TRUE;
	if (m_SurfaceEditPage.m_bDirty)
		return TRUE;
	return FALSE;
}

CSurfacePropertySheet::~CSurfacePropertySheet()
{
}


BEGIN_MESSAGE_MAP(CSurfacePropertySheet, CPropertySheet)
	//{{AFX_MSG_MAP(CSurfacePropertySheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSurfacePropertySheet message handlers
