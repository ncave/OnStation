// EditorSettings.cpp : implementation file
//

#include "stdafx.h"
#include "onstation.h"
#include "legEditorSettingspage.h"
#include "surveyleg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLegEditorSettingsPage property page

IMPLEMENT_DYNCREATE(CLegEditorSettingsPage, CPropertyPage)

CLegEditorSettingsPage::CLegEditorSettingsPage() : CPropertyPage(CLegEditorSettingsPage::IDD)
{
	//{{AFX_DATA_INIT(CLegEditorSettingsPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CLegEditorSettingsPage::~CLegEditorSettingsPage()
{
}

void CLegEditorSettingsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLegEditorSettingsPage)
	DDX_Control(pDX, IDC_LISTCOLUMNS, m_ColumnList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLegEditorSettingsPage, CPropertyPage)
	//{{AFX_MSG_MAP(CLegEditorSettingsPage)
	ON_BN_CLICKED(IDC_MOVEDOWN, OnMovedown)
	ON_BN_CLICKED(IDC_MOVEUP, OnMoveup)
	ON_BN_CLICKED(IDC_DEFAULTS, OnDefaults)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLegEditorSettingsPage message handlers

void CLegEditorSettingsPage::Initialize(CSurveyLeg *Leg)
{
	m_SurveyLeg=Leg;
}

BOOL CLegEditorSettingsPage::OnSetActive()
{
	SetupFromLegFields();
	return TRUE;
}

BOOL CLegEditorSettingsPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	SetupFromLegFields();
	return FALSE;
}

void CLegEditorSettingsPage::SetupFromLegFields()
{
	m_ColumnList.ResetContent();
	for (int i=0;i<7;i++)
	{
		int iIndex;
		CString CS;
		TCHAR c=m_SurveyLeg->m_szFieldOrder[i];
		switch (c)
		{
		case _T('A'):
			CS.LoadString(IDS_COMPASS);
			break;
		case _T('I'):
			if (m_SurveyLeg->GetCaveDive())
			{
				CS.LoadString(IDS_DEPTH);
			}
			else
			{
				CS.LoadString(IDS_CLINOMETER);
			}
			break;
		case _T('U'):
			CS.LoadString(IDS_UP);
			break;
		case _T('D'):
			CS.LoadString(IDS_DOWN);
			break;
		case _T('L'):
			CS.LoadString(IDS_LEFT);
			break;
		case _T('R'):
			CS.LoadString(IDS_RIGHT);
			break;
		case _T('T'):
			CS.LoadString(IDS_LENGTH);
			break;
		default:
			ASSERT(FALSE);
		}
		iIndex=m_ColumnList.AddString(CS);
		m_ColumnList.SetItemData(iIndex,(LONG)c);
	}	
	m_ColumnList.SetCurSel(0);
}

void CLegEditorSettingsPage::OnMovedown() 
{
	TCHAR szBuffer[64];
	int iIndex=m_ColumnList.GetCurSel();
	if (iIndex!=LB_ERR && iIndex!=6)
	{
		m_ColumnList.GetText(iIndex,szBuffer);
		DWORD_PTR iData=m_ColumnList.GetItemData(iIndex);
		m_ColumnList.DeleteString(iIndex);
		int iNewPos=m_ColumnList.InsertString(iIndex+1,szBuffer);
		m_ColumnList.SetItemData(iNewPos,iData);
		m_ColumnList.SetCurSel(iNewPos);
	}
}

void CLegEditorSettingsPage::OnMoveup() 
{
	TCHAR szBuffer[64];
	int iIndex=m_ColumnList.GetCurSel();
	if (iIndex!=LB_ERR && iIndex!=0)
	{
		m_ColumnList.GetText(iIndex,szBuffer);
		DWORD_PTR iData=m_ColumnList.GetItemData(iIndex);
		m_ColumnList.DeleteString(iIndex);
		int iNewPos=m_ColumnList.InsertString(iIndex-1,szBuffer);
		m_ColumnList.SetItemData(iNewPos,iData);
		m_ColumnList.SetCurSel(iNewPos);
	}
}

void CLegEditorSettingsPage::OnDefaults() 
{
	m_SurveyLeg->GenerateDefaultFieldOrder();
	SetupFromLegFields();
}

BOOL CLegEditorSettingsPage::OnKillActive() 
{
	//Save the field order.  We simply use the item data for
	//each element of the list box.
	int iIndex=m_ColumnList.GetCount();
	for (int i=0;i<iIndex;i++)
	{
		m_SurveyLeg->m_szFieldOrder[i]=(TCHAR)m_ColumnList.GetItemData(i);
	}

	return CPropertyPage::OnKillActive();
}
