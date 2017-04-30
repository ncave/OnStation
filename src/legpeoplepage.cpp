// peoplepage.cpp : implementation file
//

#include "stdafx.h"
#include "onstation.h"
#include "legpeoplepage.h"
#include "surveyleg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLegPeoplePage property page

IMPLEMENT_DYNCREATE(CLegPeoplePage, CPropertyPage)

CLegPeoplePage::CLegPeoplePage() : CPropertyPage(CLegPeoplePage::IDD)
{
	//{{AFX_DATA_INIT(CLegPeoplePage)
	//}}AFX_DATA_INIT
}

CLegPeoplePage::~CLegPeoplePage()
{
}

void CLegPeoplePage::RealDataInit(CSurveyLeg *Leg)
{
	m_Leg=Leg;
}

BOOL CLegPeoplePage::OnKillActive()
{
	TCHAR szBuffer[128];
	GetDlgItemText(IDC_JOB1,szBuffer,_countof(szBuffer)-1);
	m_Leg->SetPersonDuty(0,szBuffer);
	GetDlgItemText(IDC_JOB2,szBuffer,_countof(szBuffer)-1);
	m_Leg->SetPersonDuty(1,szBuffer);
	GetDlgItemText(IDC_JOB3,szBuffer,_countof(szBuffer)-1);
	m_Leg->SetPersonDuty(2,szBuffer);
	GetDlgItemText(IDC_JOB4,szBuffer,_countof(szBuffer)-1);
	m_Leg->SetPersonDuty(3,szBuffer);
	GetDlgItemText(IDC_JOB5,szBuffer,_countof(szBuffer)-1);
	m_Leg->SetPersonDuty(4,szBuffer);
	GetDlgItemText(IDC_JOB6,szBuffer,_countof(szBuffer)-1);
	m_Leg->SetPersonDuty(5,szBuffer);
	GetDlgItemText(IDC_NAME1,szBuffer,_countof(szBuffer)-1);
	m_Leg->SetPersonName(0,szBuffer);
	GetDlgItemText(IDC_NAME2,szBuffer,_countof(szBuffer)-1);
	m_Leg->SetPersonName(1,szBuffer);
	GetDlgItemText(IDC_NAME3,szBuffer,_countof(szBuffer)-1);
	m_Leg->SetPersonName(2,szBuffer);
	GetDlgItemText(IDC_NAME4,szBuffer,_countof(szBuffer)-1);
	m_Leg->SetPersonName(3,szBuffer);
	GetDlgItemText(IDC_NAME5,szBuffer,_countof(szBuffer)-1);
	m_Leg->SetPersonName(4,szBuffer);
	GetDlgItemText(IDC_NAME6,szBuffer,_countof(szBuffer)-1);
	m_Leg->SetPersonName(5,szBuffer);
	return TRUE;
}

void CLegPeoplePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLegPeoplePage)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLegPeoplePage, CPropertyPage)
	//{{AFX_MSG_MAP(CLegPeoplePage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CLegPeoplePage message handlers

BOOL CLegPeoplePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	UINT idcJobs[]={IDC_JOB1,IDC_JOB2,IDC_JOB3,IDC_JOB4,IDC_JOB5,IDC_JOB6};	
	SetDlgItemText(IDC_NAME1,m_Leg->GetPersonName(0));
	SetDlgItemText(IDC_NAME2,m_Leg->GetPersonName(1));
	SetDlgItemText(IDC_NAME3,m_Leg->GetPersonName(2));
	SetDlgItemText(IDC_NAME4,m_Leg->GetPersonName(3));
	SetDlgItemText(IDC_NAME5,m_Leg->GetPersonName(4));
	SetDlgItemText(IDC_NAME6,m_Leg->GetPersonName(5));

	CString CS1;
	CS1.LoadString(IDS_JOBBOOK);
	CString CS2;
	CS2.LoadString(IDS_JOBTAPE);
	CString CS3;
	CS3.LoadString(IDS_JOBINSTRUMENTS);
	CString CS4;
	CS4.LoadString(IDS_JOBRIGGING);
	CString CS5;
	CS5.LoadString(IDS_JOBEXPLORATION);
	for (int i=0;i<6;i++)
	{
		CComboBox * pJob=(CComboBox *)GetDlgItem(idcJobs[i]);
		pJob->ResetContent();
		pJob->AddString(CS1);
		pJob->AddString(CS2);
		pJob->AddString(CS3);
		pJob->AddString(CS4);
		pJob->AddString(CS5);
		pJob->SetWindowText(m_Leg->GetPersonDuty(i));
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
