// folderinfo.cpp : implementation file
//

#include "stdafx.h"
#include "onstation.h"
#include "folderinfodlg.h"
#include "folder.h"
#include "settings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern COnStationApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CFolderInfoDlg dialog


CFolderInfoDlg::CFolderInfoDlg(CFolderData *FD,LPCTSTR szName,CWnd* pParent /*=NULL*/)
	: CDialog(CFolderInfoDlg::IDD, pParent)
{
	TCHAR szNumShots[10];
	TCHAR szNumSurfaceShots[10];
	TCHAR szTotalAllShots[10];
	TCHAR szTotalSurveys[10];
	_stprintf_s(szNumShots, _T("%i"),FD->iNumCaveShots);
	_stprintf_s(szNumSurfaceShots, _T("%i"),FD->iNumSurfaceShots);
	_stprintf_s(szTotalAllShots, _T("%i"),FD->iTotalShots);
	_stprintf_s(szTotalSurveys, _T("%i"),FD->iNumSurveys);					
	CString szLen=GetPreferedUnitString(FD->fTotalShotLengthCave);
	CString szSurfaceLen=GetPreferedUnitString(FD->fTotalShotLengthSurface);

	//{{AFX_DATA_INIT(CFolderInfoDlg)
	m_szFolderName = szName;
	m_szNumShots = szNumShots;
	m_szNumSurfaceShots = szNumSurfaceShots;
	m_szTotalAllShots=szTotalAllShots;
	m_szTotalSurveys=szTotalSurveys;
	m_szTotalLength = szLen;
	m_szTotalSurfaceLength = szSurfaceLen;
	//}}AFX_DATA_INIT
	m_FD=FD;
}


void CFolderInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFolderInfoDlg)
	DDX_Text(pDX, IDC_FOLDERNAME, m_szFolderName);
	DDX_Text(pDX, IDC_NUMSHOTS, m_szNumShots);
	DDX_Text(pDX, IDC_NUMSURFACESHOTS, m_szNumSurfaceShots);
	DDX_Text(pDX, IDC_TOTALNOTEXCLUDED, m_szTotalAllShots);
	DDX_Text(pDX, IDC_TOTALALLSURVEYS,m_szTotalSurveys);
	DDX_Text(pDX, IDC_TOTALLENGTH, m_szTotalLength);
	DDX_Text(pDX, IDC_TOTALSURFACELENGTH, m_szTotalSurfaceLength);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFolderInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CFolderInfoDlg)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFolderInfoDlg message handlers

BOOL CFolderInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_MyFontTitle.CreateFont(15,0,0,0,FW_BOLD,FALSE,FALSE,FALSE,ANSI_CHARSET,
						OUT_DEFAULT_PRECIS,CLIP_CHARACTER_PRECIS,
						DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,
						_T("Arial"));
	GetDlgItem(IDC_FOLDERNAME)->SetFont(&m_MyFontTitle,TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CFolderInfoDlg::OnDestroy() 
{
	m_MyFontTitle.DeleteObject();
}
