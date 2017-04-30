#include "stdafx.h"
#include "resource.h"

#include "aboutcopyrightpage.h"

/////////////////////////////////////////////////////////////////////////////
// CC3dApp commands

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAboutDlg::CAboutDlg() : CPropertyPage(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CAboutDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CAboutDlg::OnInitDialog() 
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

void CAboutDlg::OnDestroy() 
{
	m_MyFontTitle.DeleteObject();
	m_MyFontWarning.DeleteObject();
	m_MyFontAuthor.DeleteObject();
	CPropertyPage::OnDestroy();

	// TODO: Add your message handler code here
	
}
