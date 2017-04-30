// PageLayoutPreferences.cpp : implementation file
//

#include "stdafx.h"
#include "onstation.h"
#include "PageLayoutPreferences.h"
#include "titleboxsetup.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageLayoutPreferences dialog


CPageLayoutPreferences::CPageLayoutPreferences(COnStationView *Vw,CWnd* pParent /*=NULL*/)
	: CDialog(CPageLayoutPreferences::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPageLayoutPreferences)
	m_bThickLines = FALSE;
	m_bIncludeArrow = FALSE;
	m_bIncludeScale = FALSE;
	m_bIncludeSideScale = FALSE;
	m_iPageOverlap = -1;
	m_bShowTitleBox = FALSE;
	m_bNameByYear = FALSE;
	//}}AFX_DATA_INIT
	m_View=Vw;
}


void CPageLayoutPreferences::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageLayoutPreferences)
	DDX_Check(pDX, IDC_CHECK_THICKLINES, m_bThickLines);
	DDX_Check(pDX, IDC_CHECKINCLUDENORTHARROW, m_bIncludeArrow);
	DDX_Check(pDX, IDC_CHECKINCLUDESCALE, m_bIncludeScale);
	DDX_Check(pDX, IDC_CHECKSIDESCALE, m_bIncludeSideScale);
	DDX_CBIndex(pDX, IDC_COMBOOVERLAP, m_iPageOverlap);
	DDX_Check(pDX, IDC_CHECK5, m_bShowTitleBox);
	DDX_Check(pDX, IDC_CHECK3, m_bNameByYear);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageLayoutPreferences, CDialog)
	//{{AFX_MSG_MAP(CPageLayoutPreferences)
	ON_BN_CLICKED(IDC_BUTTON1, OnEditTitlePage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageLayoutPreferences message handlers

void CPageLayoutPreferences::OnEditTitlePage() 
{
	CTitleBoxSetup TPSD(m_View);;
	if (TPSD.DoModal()==IDOK)
	{

	}
}
