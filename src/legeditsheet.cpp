// editsheet.cpp : implementation file
//

#include "stdafx.h"
#include "onstation.h"
#include "legeditsheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditSheet

BEGIN_MESSAGE_MAP(CEditSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CEditSheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#define new DEBUG_NEW


CEditSheet::CEditSheet(CSurveyLeg *Leg,UINT uiCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(uiCaption, pParentWnd, iSelectPage),m_InstrumentPage(Leg)
{
	m_Leg=Leg;
	AddPage(&m_SummaryPage);
	m_SummaryPage.SetLeg(Leg);
	AddPage(&m_GridPage);
	m_GridPage.SetLeg(Leg);
	AddPage(&m_InstrumentPage);
	m_PeoplePage.RealDataInit(Leg);
	AddPage(&m_PeoplePage);	
	m_EditorSettingsPage.Initialize(Leg);
	AddPage(&m_EditorSettingsPage);
}

CEditSheet::~CEditSheet()
{
}

BOOL CEditSheet::NeedRecalculation()
{
	if (m_InstrumentPage.m_bDirty)
		return TRUE;
	if (m_GridPage.NeedRecalculate())
		return TRUE;
	if (m_SummaryPage.m_bDirty)
		return TRUE;
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CEditSheet message handlers
