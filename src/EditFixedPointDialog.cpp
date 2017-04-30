// edittiein.cpp : implementation file
//

#include "stdafx.h"
#include "onstation.h"
#include "editfixedpointdialog.h"
#include "surfacedata.h"
#include "onstationdoc.h"
#include "fixedpoint.h"

extern COnStationApp theApp;
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditFixedPointDialog dialog


CEditFixedPointDialog::CEditFixedPointDialog(COnStationDoc * document,CFixedPoint *Tie,CWnd* pParent /*=NULL*/)
	: CDialog(CEditFixedPointDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditFixedPointDialog)
	//}}AFX_DATA_INIT
	m_TieIn=Tie;
	m_MyDocument=document;

	m_szComment=Tie->GetComment();
	m_szStationText=Tie->GetName();
	if (theApp.GetSettings()->IsMetric())
	{
		m_fXpan=Tie->GetX();
		m_fYpan=Tie->GetY();
		m_fZpan=Tie->GetZ();
		m_StaticZ = _T("&Z Position (m):");
	}
	else
	{
		m_fXpan=Tie->GetX();
		m_fYpan=Tie->GetY();
		m_fZpan=MetersToFeet(Tie->GetZ());
		m_StaticZ = _T("&Z Position (ft):");
	}
}


void CEditFixedPointDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditFixedPointDialog)
	DDX_Control(pDX, IDC_STATICWARNING, m_WarningText);
	DDX_Text(pDX, IDC_STATICZ, m_StaticZ);
	DDX_Text(pDX, IDC_EDITXPOS, m_fXpan);
	DDX_Text(pDX, IDC_EDITYPOS, m_fYpan);
	DDX_Text(pDX, IDC_EDITZPOS, m_fZpan);
	DDX_Text(pDX, IDC_EDIT1, m_szComment);
	DDX_Text(pDX, IDC_STATIONTEXT, m_szStationText);
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate)
	{
		if (!theApp.GetSettings()->IsMetric())
		{
			m_fZpan=FeetToMeters(m_fZpan);
		}
	}
}


BEGIN_MESSAGE_MAP(CEditFixedPointDialog, CDialog)
	//{{AFX_MSG_MAP(CEditFixedPointDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CEditFixedPointDialog message handlers

void CEditFixedPointDialog::OnOK() 
{
	UpdateData(TRUE);
	m_TieIn->SetAllCoordinates(m_fXpan,m_fYpan,m_fZpan);
	m_TieIn->SetComment(m_szComment);
	m_TieIn->SetStationName(m_szStationText);
	CDialog::OnOK();
}



BOOL CEditFixedPointDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_WarningText.ShowWindow(theApp.GetSettings()->IsMetric()?SW_HIDE:SW_SHOW);

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
