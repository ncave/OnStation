// SurveySearchDialog.cpp : implementation file
//

#include "stdafx.h"
#include "onstation.h"
#include "SurveySearchDialog.h"
#include "legquery.h"
#include "onstationdoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSettings * pSettings_G;
/////////////////////////////////////////////////////////////////////////////
// SurveySearchDialog dialog


SurveySearchDialog::SurveySearchDialog(COnStationDoc * pDocument,CWnd* pParent /*=NULL*/)
	: CDialog(SurveySearchDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(SurveySearchDialog)
	m_szSurveyName = pSettings_G->m_szSurveyNameSearch;
	m_szSurveyDescription = pSettings_G->m_szSurveyDescriptionSearch;
	m_szSurveyPersonnel = pSettings_G->m_szSurveyPersonnelSearch;
	m_szSurveyStations = pSettings_G->m_szSurveyStationsSearch;
	m_CheckedORSearch = pSettings_G->m_bOrSearch;
	//}}AFX_DATA_INIT
	m_pDocument=pDocument;
	m_pLegQuery=NULL;
}


void SurveySearchDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SurveySearchDialog)
	DDX_Control(pDX, IDC_EDIT6, m_NameControl);
	DDX_Control(pDX, IDC_EDIT3, m_StationControl);
	DDX_Control(pDX, IDC_EDIT2, m_DescriptionControl);
	DDX_Control(pDX, IDC_EDIT1, m_PersonelControl);
	DDX_Control(pDX, IDC_COMBO1, m_ComboDateType);
	DDX_Text(pDX, IDC_EDIT6, m_szSurveyName);
	DDX_Text(pDX, IDC_EDIT2, m_szSurveyDescription);
	DDX_Text(pDX, IDC_EDIT1, m_szSurveyPersonnel);
	DDX_Text(pDX, IDC_EDIT3, m_szSurveyStations);
	DDX_Check(pDX, IDC_CHECK2, m_CheckedORSearch);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(SurveySearchDialog, CDialog)
	//{{AFX_MSG_MAP(SurveySearchDialog)
	ON_BN_CLICKED(IDC_DISPLAYRESULTS, OnDisplayresults)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SurveySearchDialog message handlers

void SurveySearchDialog::OnDisplayresults() 
{
	delete m_pLegQuery;
	m_pLegQuery=new CLegQuery();
	if (!UpdateData(TRUE))
		return;
	if (!m_pLegQuery->m_nameQuery.SetQueryText(m_szSurveyName))
	{
		m_NameControl.SetFocus();
		return;
	}
	if (!m_pLegQuery->m_descriptionQuery.SetQueryText(m_szSurveyDescription))
	{
		m_DescriptionControl.SetFocus();
		return;
	}
	if (!m_pLegQuery->m_personelQuery.SetQueryText(m_szSurveyPersonnel))
	{
		m_PersonelControl.SetFocus();
		return;
	}
	if (!m_pLegQuery->m_stationQuery.SetQueryText(m_szSurveyStations))
	{
		m_StationControl.SetFocus();
		return;
	}
	CDateTimeCtrl * pDate=(CDateTimeCtrl *)GetDlgItem(IDC_DATEEDIT1);
	ASSERT(pDate != NULL);

	COleDateTime date;
	pDate->GetTime(date);
	m_pLegQuery->m_dwStartDate=MakeMyDate(date.GetYear(),date.GetMonth(),date.GetDay());
	pDate=(CDateTimeCtrl *)GetDlgItem(IDC_DATEEDIT2);
	pDate->GetTime(date);
	m_pLegQuery->m_dwEndDate=MakeMyDate(date.GetYear(),date.GetMonth(),date.GetDay());
	
	switch (m_ComboDateType.GetCurSel())
	{
	case 0:
		m_pLegQuery->m_dwEndDate=0xffffffff;
		m_pLegQuery->m_dwStartDate=0xffffffff;
		break;
	case 1:	    //between
		break;
	case 2:		//before
		m_pLegQuery->m_dwStartDate=0xffffffff;
		break;
	case 3:		//after
		m_pLegQuery->m_dwEndDate=0xffffffff;
		break;
	}

	pSettings_G->m_szSurveyNameSearch=m_szSurveyName;
	pSettings_G->m_szSurveyDescriptionSearch=m_szSurveyDescription;
	pSettings_G->m_szSurveyPersonnelSearch=m_szSurveyPersonnel;
	pSettings_G->m_szSurveyStationsSearch=m_szSurveyStations;
	pSettings_G->m_bOrSearch=m_CheckedORSearch;

	EndDialog(IDOK);
}

BOOL SurveySearchDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ComboDateType.ResetContent();
	CString CS;
	CS.LoadString(IDS_CDT_IGNORE_DATES);
	m_ComboDateType.AddString(CS);
	CS.LoadString(IDS_CDT_BETWEEN);
	m_ComboDateType.AddString(CS);
	CS.LoadString(IDS_CDT_BEFORE);
	m_ComboDateType.AddString(CS);
	CS.LoadString(IDS_CDT_AFTER);
	m_ComboDateType.AddString(CS);
	m_ComboDateType.SetCurSel(0);
	GetDlgItem(IDC_DATEEDIT1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DATEEDIT2)->ShowWindow(SW_HIDE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void SurveySearchDialog::OnSelchangeCombo1() 
{
	int iIndex=m_ComboDateType.GetCurSel();	
	switch (iIndex)
	{
	case 0:
		GetDlgItem(IDC_DATEEDIT1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DATEEDIT2)->ShowWindow(SW_HIDE);
		break;
	case 1:
		GetDlgItem(IDC_DATEEDIT1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DATEEDIT2)->ShowWindow(SW_SHOW);
		break;
	case 2:
		GetDlgItem(IDC_DATEEDIT1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DATEEDIT2)->ShowWindow(SW_SHOW);
		break;
	case 3:
		GetDlgItem(IDC_DATEEDIT1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DATEEDIT2)->ShowWindow(SW_HIDE);
		break;
	}
}
