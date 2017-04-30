// DepthColorHelperDialog.cpp : implementation file
//

#include "stdafx.h"
#include "OnStation.h"
#include "DepthColorHelperDialog.h"
#include "settings.h"
extern COnStationApp theApp;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDepthColorHelperDialog dialog


CDepthColorHelperDialog::CDepthColorHelperDialog(float fStartDepth,float fIncr,int iNum,CWnd* pParent /*=NULL*/)
	: CDialog(CDepthColorHelperDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDepthColorHelperDialog)
	m_iNumIntervals = iNum;
	m_fDepth = fIncr;
	m_fStartDepth = fStartDepth;
	//}}AFX_DATA_INIT
	if (!theApp.GetSettings()->IsMetric())
	{
		m_fStartDepth=MetersToFeet(m_fStartDepth);
		m_fDepth=MetersToFeet(m_fDepth);
	}
}


void CDepthColorHelperDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDepthColorHelperDialog)
	DDX_Control(pDX, IDC_STATICSTARTDEPTH, m_szStartingText);
	DDX_Control(pDX, IDC_STATICINTERVAL, m_szIntervalText);
	DDX_Text(pDX, IDC_NUMINTERVALS, m_iNumIntervals);
	DDV_MinMaxUInt(pDX, m_iNumIntervals, 2, 15);
	DDX_Text(pDX, IDC_DEPTH, m_fDepth);
	DDV_MinMaxFloat(pDX, m_fDepth, 5.f, 5000.f);
	DDX_Text(pDX, IDC_STARTINGDEPTH, m_fStartDepth);
	//}}AFX_DATA_MAP
	if (!theApp.GetSettings()->IsMetric() && pDX->m_bSaveAndValidate)
	{
		m_fStartDepth=FeetToMeters(m_fStartDepth);
		m_fDepth=FeetToMeters(m_fDepth);
	}
}


BEGIN_MESSAGE_MAP(CDepthColorHelperDialog, CDialog)
	//{{AFX_MSG_MAP(CDepthColorHelperDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDepthColorHelperDialog message handlers
BOOL CDepthColorHelperDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString CS;
	if (theApp.GetSettings()->IsMetric())
	{
		CS.LoadString(IDS_STARTDEPTHM);
		m_szStartingText.SetWindowText(CS);
		CS.LoadString(IDS_INTERVALM);
		m_szIntervalText.SetWindowText(CS);
	}
	else
	{
		CS.LoadString(IDS_STARTDEPTHFT);
		m_szStartingText.SetWindowText(CS);
		CS.LoadString(IDS_INTERVALFT);
		m_szIntervalText.SetWindowText(CS);
	}	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
