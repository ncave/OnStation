// ColorSchemeComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "onstation.h"
#include "ColorSchemeComboBox.h"
#include "onstationdoc.h"
#include "settings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern COnStationDoc * pDocument_G;	
extern CSettings * pSettings_G;
/////////////////////////////////////////////////////////////////////////////
// CColorSchemeComboBox

CColorSchemeComboBox::CColorSchemeComboBox()
{
	m_iCurSel=0;
	m_bEdited=FALSE;
}

CColorSchemeComboBox::~CColorSchemeComboBox()
{
}

void CColorSchemeComboBox::InitialUpdate()
{
	UpdateEntries();
}

void CColorSchemeComboBox::UpdateEntries()
{
	if (pDocument_G==NULL)
	{
		return;
	}
	m_iCurSel=0;
	ResetContent();
	for (int i=0;i<TOTAL_COLOR_SCHEMES;i++)
	{
		TRACE(_T("COMBO: Setting entry %i to %s\n"),i+1,(LPCTSTR)pSettings_G->m_szColorSchemeNames[i]);
		AddString(pSettings_G->m_szColorSchemeNames[i]);
	}	
	SetCurSel(m_iCurSel);
	ASSERT(GetCurSel()!=-1);
}

BEGIN_MESSAGE_MAP(CColorSchemeComboBox, CComboBox)
	//{{AFX_MSG_MAP(CColorSchemeComboBox)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
	ON_CONTROL_REFLECT(CBN_KILLFOCUS, OnKillfocus)
	ON_CONTROL_REFLECT(CBN_EDITCHANGE, OnEditchange)
	ON_CONTROL_REFLECT(CBN_DROPDOWN, OnDropdown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorSchemeComboBox message handlers

void CColorSchemeComboBox::OnSelchange() 
{
	static int cmds[]={ID_DRAWMODE_COLORSCHEME1,
						ID_DRAWMODE_COLORSCHEME2, 
						ID_DRAWMODE_COLORSCHEME3, 
						ID_DRAWMODE_COLORSCHEME4, 
						ID_DRAWMODE_COLORSCHEME5, 
						ID_DRAWMODE_COLORSCHEME6, 
						ID_DRAWMODE_COLORSCHEME7, 
						ID_DRAWMODE_COLORSCHEME8,
						ID_DRAWMODE_DEPTHCODING,
						ID_DRAWMODE_AGECOLORING,
						ID_DRAWMODE_BLUNDERCOLOR};
	if (m_bEdited && m_iCurSel<MAX_REGULAR_COLOR_SCHEMES)
	{
		GetWindowText(pSettings_G->m_szColorSchemeNames[m_iCurSel]);
		DeleteString(m_iCurSel);
		InsertString(m_iCurSel,pSettings_G->m_szColorSchemeNames[m_iCurSel]);
		SetCurSel(m_iCurSel);
	}
	m_iCurSel=GetCurSel();
	//let parent know that the color scheme is change
	GetParent()->PostMessage(WM_COMMAND,cmds[m_iCurSel]);
	m_bEdited=FALSE;
	ASSERT(GetCurSel()!=-1);
}

void CColorSchemeComboBox::OnKillfocus() 
{
	if (m_iCurSel==-1)
	{
		return;
	}
	if (m_bEdited && m_iCurSel<MAX_REGULAR_COLOR_SCHEMES)
	{
		GetWindowText(pSettings_G->m_szColorSchemeNames[m_iCurSel]);
		DeleteString(m_iCurSel);
		InsertString(m_iCurSel,pSettings_G->m_szColorSchemeNames[m_iCurSel]);
		SetCurSel(m_iCurSel);
	}
	ASSERT(GetCurSel()!=-1);
}

void CColorSchemeComboBox::OnEditchange() 
{
	if (m_iCurSel>=MAX_REGULAR_COLOR_SCHEMES)
	{
		MessageBeep(0);	//not allowed to edit these names
	}
	m_bEdited=TRUE;
}

void CColorSchemeComboBox::OnDropdown() 
{
	TRACE(_T("CB:Dropdown\n"));
	ASSERT(m_iCurSel!=-1);
	if (m_bEdited && m_iCurSel<MAX_REGULAR_COLOR_SCHEMES)
	{
		GetWindowText(pSettings_G->m_szColorSchemeNames[m_iCurSel]);
		DeleteString(m_iCurSel);
		InsertString(m_iCurSel,pSettings_G->m_szColorSchemeNames[m_iCurSel]);
		SetCurSel(m_iCurSel);
	}
	ASSERT(GetCurSel()!=-1);
}
