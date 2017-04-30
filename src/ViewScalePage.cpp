// ViewScalePage.cpp : implementation file
//

#include "stdafx.h"
#include "OnStation.h"
#include "ViewScalePage.h"
#include "settings.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern COnStationApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CViewScalePage dialog


CViewScalePage::CViewScalePage(float fScale,CWnd* pParent /*=NULL*/)
	: CPropertyPage(CViewScalePage::IDD)
{
	//{{AFX_DATA_INIT(CViewScalePage)
	//}}AFX_DATA_INIT
	m_fMetricScale=fScale;
	if (!theApp.GetSettings()->IsMetric())
	{
		m_fScale=MetersToFeet(fScale)*CMPERINCH;
	}
	else
	{
		m_fScale=fScale;
	}
}


void CViewScalePage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewScalePage)
	DDX_Control(pDX, IDC_SCALESIZE, m_SizeControl);
	DDX_Control(pDX, IDC_STATIC3, m_szLimits);
	DDX_Control(pDX, IDC_STATIC2, m_szDescribeText);
	DDX_Text(pDX, IDC_SCALESIZE, m_fScale);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewScalePage, CPropertyPage)
	//{{AFX_MSG_MAP(CViewScalePage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewScalePage message handlers

BOOL CViewScalePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	if (!theApp.GetSettings()->IsMetric())
	{
		CString CS;
		CS.LoadString(IDS_VALUELIMITSFT);
		m_szLimits.SetWindowText(CS);
		CS.LoadString(IDS_SIZEFTIN);
		m_szDescribeText.SetWindowText(CS);
	}

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CViewScalePage::OnKillActive()
{
	if (UpdateData(TRUE))
	{
		if (theApp.GetSettings()->IsMetric() && (m_fScale<1.0f || m_fScale>5000.0f))
		{
			AfxMessageBox(IDS_METERLIMIT);
			m_SizeControl.SetFocus();
			return FALSE;
		}
		else if (!theApp.GetSettings()->IsMetric() && (m_fScale<10.0f || m_fScale>20000.0f))
		{
			AfxMessageBox(IDS_FEETLIMIT);
			m_SizeControl.SetFocus();
			return FALSE;
		}
		m_fMetricScale=m_fScale;
		if (!theApp.GetSettings()->IsMetric())
		{
			m_fMetricScale=FeetToMeters(m_fMetricScale)/CMPERINCH;
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
