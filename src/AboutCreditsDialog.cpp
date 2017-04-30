// AboutCreditsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "OnStation.h"
#include "AboutCreditsDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutCreditsDialog dialog


CAboutCreditsDialog::CAboutCreditsDialog(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CAboutCreditsDialog::IDD)
{
	//{{AFX_DATA_INIT(CAboutCreditsDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAboutCreditsDialog::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutCreditsDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAboutCreditsDialog, CPropertyPage)
	//{{AFX_MSG_MAP(CAboutCreditsDialog)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutCreditsDialog message handlers

void CAboutCreditsDialog::OnDestroy() 
{
	CPropertyPage::OnDestroy();
	
	m_MyFontTitle.DeleteObject();
	m_MyFontAuthor.DeleteObject();
}

BOOL CAboutCreditsDialog::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_MyFontTitle.CreateFont(24,0,0,0,FW_BOLD,FALSE,FALSE,FALSE,ANSI_CHARSET,
						OUT_DEFAULT_PRECIS,CLIP_CHARACTER_PRECIS,
						DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,
						_T("Arial"));
	GetDlgItem(IDC_STATICTITLE)->SetFont(&m_MyFontTitle,TRUE);

	m_MyFontAuthor.CreateFont(18,0,0,0,FW_BOLD,FALSE,FALSE,FALSE,ANSI_CHARSET,
						OUT_DEFAULT_PRECIS,CLIP_CHARACTER_PRECIS,
						DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,
						_T("Arial"));
	GetDlgItem(IDC_STATICAUTHOR)->SetFont(&m_MyFontAuthor,TRUE);
	
	GetDlgItem(IDC_STATICLOOPS)->SetFont(&m_MyFontAuthor,TRUE);
	GetDlgItem(IDC_STATICDEVELOPER)->SetFont(&m_MyFontAuthor,TRUE);
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
