// SearchCommentsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "onstation.h"
#include "SearchCommentsDialog.h"
#include "realfolder.h"
#include "query.h"
#include "onstationview.h"
#include "settings.h"
#include "searchresultdlg.h"
#include "colortool.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString szSavedText;
CString szSavedType;
/////////////////////////////////////////////////////////////////////////////
// CSearchCommentsDialog dialog
extern CSettings * pSettings_G;
//Save these two global to maintain consitency between invokations
CSearchCommentsDialog::CSearchCommentsDialog(COnStationView *view,CRealFolder *folder,CWnd* pParent /*=NULL*/)
: CDialog(CSearchCommentsDialog::IDD, pParent),m_ColorWidget(FALSE,3,5)
{
	m_Folder=folder;
	m_pView=view;
	//{{AFX_DATA_INIT(CSearchCommentsDialog)
	m_szCommentText = szSavedText;
	m_szTypeString = szSavedType;
	m_bColorToShot = pSettings_G->m_bQueryTo;
	m_bColorFromShot = pSettings_G->m_bQueryFrom;
	m_bPlaceTag = pSettings_G->m_bQueryBox;
	//}}AFX_DATA_INIT
	m_iColorIndex=1;	//Default to Red but black or white
}


void CSearchCommentsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSearchCommentsDialog)
	DDX_Text(pDX, IDC_EDIT2, m_szCommentText);
	DDX_CBString(pDX, IDC_COMBO1, m_szTypeString);
	DDX_Check(pDX, IDC_COLORTOSHOT, m_bColorToShot);
	DDX_Check(pDX, IDC_COLORFROMSHOT, m_bColorFromShot);
	DDX_Check(pDX, IDC_PLACETAG, m_bPlaceTag);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSearchCommentsDialog, CDialog)
	//{{AFX_MSG_MAP(CSearchCommentsDialog)
	ON_BN_CLICKED(IDC_DISPLAYRESULTS, OnDisplayresults)
	ON_MESSAGE(WM_COLORCHANGE, OnColorChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSearchCommentsDialog message handlers

void CSearchCommentsDialog::OnOK() 
{
	CDialog::OnOK();
	szSavedText=m_szCommentText;
	szSavedType=m_szTypeString;
	pSettings_G->m_bQueryTo=m_bColorToShot;
	pSettings_G->m_bQueryFrom=m_bColorFromShot;
	pSettings_G->m_crQueryColor=CGlobalColorManager::StandardVGAColors[m_iColorIndex];
	pSettings_G->m_bQueryBox=m_bPlaceTag;
}

BOOL CSearchCommentsDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	RECT rect;
	GetDlgItem(IDC_COLORSTATIC)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	m_ColorWidget.m_iSelectedCell=1;	//red
	m_ColorWidget.Create(this,rect,IDC_NEWCOLORSLOT);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LONG_PTR CSearchCommentsDialog::OnColorChange(UINT_PTR, LONG_PTR lParam)
{
	m_iColorIndex=lParam;
	return 0l;	
}


void CSearchCommentsDialog::OnDisplayresults() 
{
	UpdateData(TRUE);
	CQuery typeQuery;
	CQuery bodyQuery;
	if (!typeQuery.SetQueryText(m_szTypeString))
	{
		return;
	}
	if (!bodyQuery.SetQueryText(m_szCommentText))
	{
		return;
	}
	CPtrArray resultArray;
	m_Folder->QuerySurveyComments(&resultArray,&typeQuery,&bodyQuery);
	if (resultArray.GetSize()==0)
	{
		AfxMessageBox(IDS_NOMATCHONSEARCH);
	}
	else
	{
		CSearchResultDialog SRD(m_pView,&resultArray);
		SRD.DoModal();
	}
}
