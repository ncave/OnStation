// agecolors.cpp : implementation file
//

#include "stdafx.h"
#include "onstation.h"
#include "agecolorpage.h"
#include "settings.h"
#include "colortool.h"
#include "onstationview.h"
#include "onstationdoc.h"
#include "mainfrm.h"
#include "agecolorhelper.h"
#include "colortool.h"
#include "colorsheet.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CGlobalColorManager Colors_G;
extern CSettings * pSettings_G;
/////////////////////////////////////////////////////////////////////////////
// CAgeColorPage dialog


CAgeColorPage::CAgeColorPage(CColorSheet * pColorSheet)
	: CPropertyPage(CAgeColorPage::IDD),m_ColorWidget(FALSE,3,5)
{
	//{{AFX_DATA_INIT(CAgeColorPage)
	//}}AFX_DATA_INIT
	m_bDirty=FALSE;
	m_pColorSheet=pColorSheet;
}

BOOL CAgeColorPage::IsDirty()
{
	return m_bDirty;
}

void CAgeColorPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAgeColorPage)
	DDX_Control(pDX, IDC_AGELIST, m_AgeList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAgeColorPage, CPropertyPage)
	//{{AFX_MSG_MAP(CAgeColorPage)
	ON_BN_CLICKED(IDC_BUTTONDEFAULTS, OnButtondefaults)
	ON_WM_DRAWITEM()
	ON_MESSAGE(WM_COLORCHANGE, OnColorChange)
	ON_BN_CLICKED(IDC_WIZARD, OnWizard)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAgeColorPage message handlers

BOOL CAgeColorPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	RECT rect;
	GetDlgItem(IDC_COLORSLOT)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	m_ColorWidget.Create(this,rect,IDC_NEWCOLORSLOT);
	
	InitializeAll();
	m_pColorSheet->m_iRecommendedColorSchemeIndex=COLORSCHEME_AGE;
	m_AgeList.SetFocus();
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAgeColorPage::InitializeAll()
{
	m_AgeList.ResetContent();
	for (int i=0;i<pSettings_G->m_iNumAgeColors;i++)
	{
		TCHAR szTemp[20];
		CString szString;
		DWORD dwAge=pSettings_G->m_dwAgeDates[i];
		DWordToDate(dwAge,szTemp, _countof(szTemp));
		if (i==0)
		{
			szString.Format(IDS_BEFORE,szTemp);
		}
		else if (i==pSettings_G->m_iNumAgeColors-1)
		{
			DWordToDate(pSettings_G->m_dwAgeDates[i-1],szTemp, _countof(szTemp));
			szString.Format(IDS_ANDLATER,szTemp);
		}
		else
		{
			TCHAR szTemp1[20];
			DWordToDate(pSettings_G->m_dwAgeDates[i-1],szTemp1, _countof(szTemp1));
			szString.Format(IDS_STOS,szTemp1,szTemp);
		}
		m_AgeList.AddString(szString);
	}
}


void CAgeColorPage::OnButtondefaults() 
{
	pSettings_G->SetDefaultAgeColors();
	m_bDirty=TRUE;
	InitializeAll();
}

void CAgeColorPage::OnOK() 
{
	CPropertyPage::OnOK();
}


LONG_PTR CAgeColorPage::OnColorChange(UINT_PTR, LONG_PTR lParam)
{
	m_bDirty=TRUE;
	int iIndex=m_AgeList.GetCurSel();
	if (iIndex==LB_ERR)
	{
		return 0l;
	}
	else
	{
		CancelToClose();
		pSettings_G->m_crAgeColors[iIndex]=(COLORREF)lParam;

		m_AgeList.InvalidateRect(NULL,TRUE);
	}
	return 0l;	
}

void CAgeColorPage::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (lpDrawItemStruct->itemID!=LB_ERR)
	{
		DWORD dwColor=pSettings_G->m_crAgeColors[lpDrawItemStruct->itemID];
		if (dwColor==RGB(255,255,255))
		{
			dwColor=RGB(0,0,0);	//white on white is hard to see, so display it as black
		}
		SetTextColor(lpDrawItemStruct->hDC,dwColor);
		if (lpDrawItemStruct->itemState&ODS_FOCUS)
		{
			SetBkMode(lpDrawItemStruct->hDC,OPAQUE);
			SetBkColor(lpDrawItemStruct->hDC,RGB(0,0,255));
		}
		TCHAR szTemp[128];
		m_AgeList.GetText(lpDrawItemStruct->itemID,szTemp);
		TabbedTextOut(lpDrawItemStruct->hDC,lpDrawItemStruct->rcItem.left,lpDrawItemStruct->rcItem.top,szTemp,lstrlen(szTemp),0,NULL,0);
	}	
	else	
	{
		CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
	}
}




void CAgeColorPage::OnWizard() 
{
	WORD wYearDiff=GetYear(pSettings_G->m_dwAgeDates[1])-
				   GetYear(pSettings_G->m_dwAgeDates[0]);
	if (wYearDiff==0)
		wYearDiff=1;
	if (wYearDiff>50)
		wYearDiff=50;
	CAgeColorHelper ACH(pSettings_G->m_dwAgeDates[0],pSettings_G->m_iNumAgeColors,wYearDiff);
	if (ACH.DoModal()==IDOK)
	{
		m_bDirty=TRUE;
		CancelToClose();
		int iYear=GetYear(ACH.m_dwStartDate);
		int iDay=GetDay(ACH.m_dwStartDate);
		pSettings_G->SetAgeColors(ACH.m_dwStartDate,ACH.m_iNumIntervals,ACH.m_iYears);
		InitializeAll();	//reset the list box
	}
}
