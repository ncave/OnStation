// AboutBoxSummary.cpp : implementation file
//

#include "stdafx.h"
#include "OnStation.h"
#include "AboutBoxSummary.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutBoxSummary property page

//IMPLEMENT_DYNCREATE(CAboutBoxSummary, CPropertyPage)

CAboutBoxSummary::CAboutBoxSummary(CWnd *pParent) : CPropertyPage(CAboutBoxSummary::IDD)
{
	//{{AFX_DATA_INIT(CAboutBoxSummary)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CAboutBoxSummary::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutBoxSummary)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAboutBoxSummary, CPropertyPage)
	//{{AFX_MSG_MAP(CAboutBoxSummary)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutBoxSummary message handlers
void CAboutBoxSummary::OnDestroy() 
{
	CPropertyPage::OnDestroy();
	
	m_MyFontTitle.DeleteObject();
	m_MyFontAuthor.DeleteObject();
	
}

BOOL CAboutBoxSummary::OnInitDialog() 
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
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

