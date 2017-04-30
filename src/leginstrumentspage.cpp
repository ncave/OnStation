// instruments.cpp : implementation file
//

#include "stdafx.h"
#include "onstation.h"
#include "leginstrumentspage.h"
#include "surveyleg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CLegInstrumentsPage, CPropertyPage)
	//{{AFX_MSG_MAP(CLegInstrumentsPage)
	ON_EN_CHANGE(IDC_CLINO_STD_ERROR, OnBigChange)
	ON_EN_CHANGE(IDC_CLINO1FIX, OnBigChange)
	ON_EN_CHANGE(IDC_CLINO2FIX, OnBigChange)
	ON_EN_CHANGE(IDC_COMPASS1FIX, OnBigChange)
	ON_EN_CHANGE(IDC_COMPASS2FIX, OnBigChange)
	ON_EN_CHANGE(IDC_DECLINATION, OnBigChange)
	ON_EN_CHANGE(IDC_TAPE_STD_ERROR, OnBigChange)
	ON_EN_CHANGE(IDC_TAPEFIX, OnBigChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLegInstrumentsPage property page

#define new DEBUG_NEW


CLegInstrumentsPage::CLegInstrumentsPage(CSurveyLeg *Leg) : CPropertyPage(CLegInstrumentsPage::IDD)
{
	m_Leg=Leg;
	//{{AFX_DATA_INIT(CLegInstrumentsPage)
	m_fTapeName = m_Leg->GetTapeName();
	m_fTapeFix = m_Leg->GetTapeError();
	m_szClino1Name = m_Leg->GetClino1Name();
	m_fClino1Fix = Leg->GetClino1Error();
	m_szClino2Name = m_Leg->GetClino2Name();
	m_fClino2Fix = m_Leg->GetClino2Error();
	m_szCompass1Name = m_Leg->GetCompass1Name();
	m_fCompass1Fix = m_Leg->GetCompass1Error();
	m_szCompass2Name = m_Leg->GetCompass2Name();
	m_fCompass2Fix = m_Leg->GetCompass2Error();
	m_fDeclination = m_Leg->GetDeclination();
	m_fStandardTapeError = m_Leg->GetStandardTapeError();
	m_fStandardCompassError = m_Leg->GetStandardCompassError();
	m_fStandardClinoError = m_Leg->GetStandardClinoError();
	//}}AFX_DATA_INIT
	m_bDirty=FALSE;
}


BOOL CLegInstrumentsPage::OnKillActive()
{
	if (UpdateData(TRUE))
	{
		m_Leg->SetClino1Error(m_fClino1Fix);
		m_Leg->SetClino1Name(m_szClino1Name);
		m_Leg->SetClino2Error(m_fClino2Fix);
		m_Leg->SetClino2Name(m_szClino2Name);
		m_Leg->SetStandardCompassError(m_fStandardCompassError);
		m_Leg->SetCompass1Error(m_fCompass1Fix);
		m_Leg->SetCompass1Name(m_szCompass1Name);
		m_Leg->SetCompass2Error(m_fCompass2Fix);
		m_Leg->SetCompass2Name(m_szCompass2Name);
		m_Leg->SetDeclination(m_fDeclination);
		m_Leg->SetStandardTapeError(m_fStandardTapeError);
		m_Leg->SetTapeError(m_fTapeFix);
		m_Leg->SetStandardClinoError(m_fStandardClinoError);
		m_Leg->SetTapeName(m_fTapeName);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CLegInstrumentsPage message handlers

BOOL CLegInstrumentsPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


CLegInstrumentsPage::~CLegInstrumentsPage()
{
}

void CLegInstrumentsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLegInstrumentsPage)
	DDX_Text(pDX, IDC_CLINO1FIX, m_fClino1Fix);
	DDV_MinMaxFloat(pDX, m_fClino1Fix, -20.f, 20.f);
	DDX_Text(pDX, IDC_CLINO1NAME, m_szClino1Name);
	DDX_Text(pDX, IDC_CLINO2FIX, m_fClino2Fix);
	DDV_MinMaxFloat(pDX, m_fClino2Fix, -20.f, 20.f);
	DDX_Text(pDX, IDC_CLINO2NAME, m_szClino2Name);
	DDX_Text(pDX, IDC_COMPASS_STD_ERROR, m_fStandardCompassError);
	DDV_MinMaxFloat(pDX, m_fStandardCompassError, 0.1f, 30.f);
	DDX_Text(pDX, IDC_COMPASS1FIX, m_fCompass1Fix);
	DDX_Text(pDX, IDC_COMPASS1NAME, m_szCompass1Name);
	DDX_Text(pDX, IDC_COMPASS2FIX, m_fCompass2Fix);
	DDX_Text(pDX, IDC_COMPASS2NAME, m_szCompass2Name);
	DDX_Text(pDX, IDC_DECLINATION, m_fDeclination);
	DDX_Text(pDX, IDC_TAPE_STD_ERROR, m_fStandardTapeError);
	DDV_MinMaxFloat(pDX, m_fStandardTapeError, 5.e-003f, 5.f);
	DDX_Text(pDX, IDC_TAPEFIX, m_fTapeFix);
	DDV_MinMaxFloat(pDX, m_fTapeFix, -10.f, 10.f);
	DDX_Text(pDX, IDC_TAPENAME, m_fTapeName);
	DDX_Text(pDX, IDC_CLINO_STD_ERROR, m_fStandardClinoError);
	DDV_MinMaxFloat(pDX, m_fStandardClinoError, 1.e-002f, 20.f);
	//}}AFX_DATA_MAP
}



//This is called when something has changed which will require
//a system recalculation
void CLegInstrumentsPage::OnBigChange() 
{
	m_bDirty=TRUE;
}

BOOL CLegInstrumentsPage::OnSetActive() 
{
	int iShow=m_Leg->GetCaveDive()?SW_HIDE:SW_SHOW;
	GetDlgItem(IDC_COMPASS2FIX)->ShowWindow(iShow);
	GetDlgItem(IDC_COMPASS2NAME)->ShowWindow(iShow);
	GetDlgItem(IDC_CLINO2FIX)->ShowWindow(iShow);
	GetDlgItem(IDC_CLINO2NAME)->ShowWindow(iShow);
	GetDlgItem(IDC_COMPASS2TEXT)->ShowWindow(iShow);
	GetDlgItem(IDC_CLINO2TEXT)->ShowWindow(iShow);
	CString CS;
	if (m_Leg->GetCaveDive())
	{
		CS.LoadString(IDS_DEPTHTAG);
	}
	else
	{
		CS.LoadString(IDS_CLINOTAG);
	}
	GetDlgItem(IDC_CLINO1TEXT)->SetWindowText(CS);
	GetDlgItem(IDC_CLINO1TEXTA)->SetWindowText(CS);
	m_bDirty=FALSE;
	
	return CPropertyPage::OnSetActive();
}
