// Preferences.cpp : implementation file
//

#include "stdafx.h"
#include "OnStation.h"
#include "Preferences.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPreferences dialog


CPreferences::CPreferences(BOOL bStatusBar,BOOL bToolBar,BOOL bRuler,BOOL bIndexTabs,BOOL bCompass,PREFEREDUNITS Units,CWnd* pParent /*=NULL*/)
	: CDialog(CPreferences::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPreferences)
	m_bStatusBar = bStatusBar;
	m_bToolBar = bToolBar;
	m_bRuler = bRuler;
	m_bIndexTabs = bIndexTabs;
	m_bCompass = bCompass;
	//}}AFX_DATA_INIT
	m_PreferedUnits = Units;
}


void CPreferences::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPreferences)
	DDX_Check(pDX, IDC_CHECK1, m_bStatusBar);
	DDX_Check(pDX, IDC_CHECK2, m_bToolBar);
	DDX_Check(pDX, IDC_CHECK3, m_bRuler);
	DDX_Check(pDX, IDC_CHECK4, m_bIndexTabs);
	DDX_Check(pDX, IDC_CHECK5, m_bCompass);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPreferences, CDialog)
	//{{AFX_MSG_MAP(CPreferences)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreferences message handlers

BOOL CPreferences::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CheckRadioButton(IDC_RADIO1,IDC_RADIO4,IDC_RADIO1+m_PreferedUnits);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPreferences::OnOK() 
{
	m_PreferedUnits=(PREFEREDUNITS)(GetCheckedRadioButton(IDC_RADIO1,IDC_RADIO4)-IDC_RADIO1);
	
	CDialog::OnOK();
}
