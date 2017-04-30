// BookmarkDialog.cpp : implementation file
//

#include "stdafx.h"
#include "onstation.h"
#include "BookmarkDialog.h"
#include "bookmark.h"
#include "settings.h"
#include "stdio.h"

extern CSettings * pSettings_G;
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern COnStationApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CBookmarkDialog dialog


CBookmarkDialog::CBookmarkDialog(float fX,float fY,float fZ,float fRot,float fTilt,float fZoom,int iColorSchemeIndex,CWnd* pParent /*=NULL*/)
	: CDialog(CBookmarkDialog::IDD, pParent)
{
	m_fX=fX;
	m_fY=fY;
	m_fZ=fZ;
	m_fRot=fRot;
	m_fTilt=fTilt;
	m_fZoom=fZoom;
	m_GotoMe=NULL;
	m_iColorSchemeIndex=iColorSchemeIndex;
	m_bChanged=FALSE;
	//{{AFX_DATA_INIT(CBookmarkDialog)
	//}}AFX_DATA_INIT
}


void CBookmarkDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBookmarkDialog)
	DDX_Control(pDX, IDCANCEL, m_CloseButton);
	DDX_Control(pDX, IDC_GOTO, m_GotoButton);
	DDX_Control(pDX, IDC_DELETE, m_DeleteButton);
	DDX_Control(pDX, IDC_ADD, m_AddButton);
	DDX_Control(pDX, IDC_BOOKMARKLIST, m_ListBox);
	DDX_Control(pDX, IDC_STATICROTATION, m_RotateText);
	DDX_Control(pDX, IDC_STATICPANNING, m_PanText);
	DDX_Control(pDX, IDC_BOOKMARKCOMBO, m_ComboBox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBookmarkDialog, CDialog)
	//{{AFX_MSG_MAP(CBookmarkDialog)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_GOTO, OnGoto)
	ON_LBN_DBLCLK(IDC_BOOKMARKLIST, OnDblclkBookmarklist)
	ON_LBN_SELCHANGE(IDC_BOOKMARKLIST, OnSelchangeBookmarklist)
	ON_CBN_EDITCHANGE(IDC_BOOKMARKCOMBO, OnEditchangeBookmarkcombo)
	ON_CBN_SELCHANGE(IDC_BOOKMARKCOMBO, OnSelchangeBookmarkcombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBookmarkDialog message handlers

void CBookmarkDialog::OnAdd() 
{
	CString CS;
	m_ComboBox.GetWindowText(CS);
	if (m_ListBox.FindStringExact(-1,CS)==LB_ERR)
	{
		CBookMark * BM=new CBookMark(CS,m_fX,m_fY,m_fZ,m_fRot,m_fTilt,m_fZoom,m_iColorSchemeIndex);
		pSettings_G->AddBookMark(BM);
		ResetControls();
		m_bChanged=TRUE;
	}
}

void CBookmarkDialog::OnDelete() 
{
	int iIndex=m_ListBox.GetCurSel();
	if (iIndex!=LB_ERR)
	{
		DWORD_PTR iBookMarkIndex=m_ListBox.GetItemData(iIndex);
		CBookMark * BM=pSettings_G->GetBookMark(iBookMarkIndex);
		ASSERT(BM!=NULL);
		if (BM!=NULL)
		{
			delete BM;
			pSettings_G->RemoveBookMark(iBookMarkIndex);
			m_bChanged=TRUE;
		}
		ResetControls();
	}
}

void CBookmarkDialog::OnGoto() 
{
	int iIndex=m_ListBox.GetCurSel();
	if (iIndex!=LB_ERR)
	{
		DWORD_PTR iBookMarkIndex=m_ListBox.GetItemData(iIndex);
		m_GotoMe=pSettings_G->GetBookMark(iBookMarkIndex);
		CDialog::OnOK();
	}
}

void CBookmarkDialog::ResetControls()
{
	m_ComboBox.ResetContent();
	m_ListBox.ResetContent();
	for (int i=0;i<pSettings_G->GetNumberOfBookMarks();i++)
	{
		CBookMark * BM=pSettings_G->GetBookMark(i);
		int iIndex=m_ComboBox.AddString(BM->m_szName);
		m_ComboBox.SetItemData(iIndex,(LONG)i);
		iIndex=m_ListBox.AddString(BM->m_szName);
		m_ListBox.SetItemData(iIndex,(LONG)i);
	}
}

BOOL CBookmarkDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	ResetControls();

	m_ComboBox.SetFocus();
	m_AddButton.EnableWindow(FALSE);
	m_DeleteButton.EnableWindow(FALSE);
	m_GotoButton.EnableWindow(FALSE);
	UpdateStaticControls(NULL);

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBookmarkDialog::OnDblclkBookmarklist() 
{
	OnGoto();
}

void CBookmarkDialog::OnSelchangeBookmarklist() 
{
	m_AddButton.EnableWindow(FALSE);
	m_DeleteButton.EnableWindow(TRUE);
	m_GotoButton.EnableWindow(TRUE);
	int iIndex=m_ListBox.GetCurSel();
	if (iIndex!=LB_ERR)
	{
		CString CS;
		m_ListBox.GetText(iIndex,CS);
		int iFind=m_ComboBox.FindStringExact(-1,CS);
		ASSERT(iFind!=LB_ERR);
		m_ComboBox.SetCurSel(iFind);
		DWORD_PTR iItem=m_ComboBox.GetItemData(iFind);
		CBookMark * BM=pSettings_G->GetBookMark(iItem);
		UpdateStaticControls(BM);
	}
}

void CBookmarkDialog::UpdateStaticControls(CBookMark * BM)
{
	if (BM==NULL)
	{
		CString CS;
		CS.Format(IDS_XYZ,m_fX,m_fY,m_fZ);
		m_PanText.SetWindowText(CS);
		CS.Format(IDS_TILTSPINZOOM,m_fTilt,m_fRot,GetPreferedScaleString(m_fZoom));
		m_RotateText.SetWindowText(CS);
	}
	else
	{
		CString CS;
		CS.Format(IDS_XYZ,BM->m_fPanX,BM->m_fPanY,BM->m_fPanZ);
		m_PanText.SetWindowText(CS);

		CS.Format(IDS_TILTSPINZOOM,BM->m_fTilt,BM->m_fRot,GetPreferedScaleString(BM->m_fZoom));
		m_RotateText.SetWindowText(CS);
	}
}

void CBookmarkDialog::OnEditchangeBookmarkcombo() 
{
	m_AddButton.EnableWindow(TRUE);
	m_DeleteButton.EnableWindow(FALSE);
	m_GotoButton.EnableWindow(FALSE);
	m_ListBox.SetCurSel(-1);
	UpdateStaticControls(NULL);
}

void CBookmarkDialog::OnSelchangeBookmarkcombo() 
{
	m_AddButton.EnableWindow(FALSE);
	m_DeleteButton.EnableWindow(TRUE);
	m_GotoButton.EnableWindow(TRUE);
	int iIndex=m_ComboBox.GetCurSel();
	if (iIndex!=LB_ERR)
	{
		CString CS;
		m_ComboBox.GetLBText(iIndex,CS);
		int iFind=m_ListBox.FindStringExact(-1,CS);
		ASSERT(iFind!=LB_ERR);
		m_ListBox.SetCurSel(iFind);

		DWORD_PTR iItem=m_ListBox.GetItemData(iFind);
		CBookMark * BM=pSettings_G->GetBookMark(iItem);
		UpdateStaticControls(BM);
	}
}
