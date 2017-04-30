// depthcolors.cpp : implementation file
//

#include "stdafx.h"
#include "onstation.h"
#include "depthcolorpage.h"
#include "settings.h"
#include "onstationview.h"
#include "onstationdoc.h"
#include "colortool.h"
#include "depthcolorhelperdialog.h"
#include "depthleveldialog.h"
#include "colorsheet.h"
extern CGlobalColorManager Colors_G;
extern COnStationView * pView_G;
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDepthColorPage dialog
extern CSettings * pSettings_G;
extern COnStationDoc * pDocument_G;

CDepthColorPage::CDepthColorPage(CColorSheet * pColorSheet)
	: CPropertyPage(CDepthColorPage::IDD),m_ColorWidget(FALSE,3,5)
{
	//{{AFX_DATA_INIT(CDepthColorPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_iColorFocus=0;
	m_pColorSheet=pColorSheet;
	m_bDirty=FALSE;
}


void CDepthColorPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDepthColorPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDepthColorPage, CPropertyPage)
	//{{AFX_MSG_MAP(CDepthColorPage)
	ON_WM_PAINT()
	ON_MESSAGE(WM_COLORCHANGE, OnColorChange)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_WIZARD, OnWizard)
	ON_BN_CLICKED(IDC_EVENSPLIT, OnEvensplit)
	ON_BN_CLICKED(IDC_BUTTONDEFAULTS, OnButtondefaults)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDepthColorPage message handlers

void CDepthColorPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect rect;
	GetDlgItem(IDC_DEPTHDRAW)->GetWindowRect(&rect);
	ScreenToClient(rect);
	for (int i=0;i<pSettings_G->m_iNumDepthColors;i++)
	{
		CRect r=rect;
		r.top=rect.top+i*rect.Height()/pSettings_G->m_iNumDepthColors;
		r.bottom=r.top+rect.Height()/pSettings_G->m_iNumDepthColors;

		//Make these things into sunken wells
		CPen pen(PS_SOLID,1,m_iColorFocus!=i?RGB(192,192,192):RGB(0,0,0));
		dc.SelectStockObject(NULL_BRUSH);	//avoid flicker.
		CPen *oldPen=dc.SelectObject(&pen);
		dc.Rectangle(r);
		dc.SelectObject(oldPen);
		
		r.InflateRect(-1,-1);
		DrawClientBox(dc,r,TRUE);
		r.InflateRect(-1,-1);
		TRACE(_T("At %i color:%i\n"),i,pSettings_G->m_crDepthColors[i]);		
		CBrush *Br=new CBrush(pSettings_G->m_crDepthColors[i]);
		dc.FillRect(&r,Br);
		Br->DeleteObject();
		delete Br;
	}
}

LONG_PTR CDepthColorPage::OnColorChange(UINT_PTR, LONG_PTR lParam)
{
	m_bDirty=TRUE;
   	pSettings_G->m_crDepthColors[m_iColorFocus]=(int)lParam;
	CRect rect;
	GetDlgItem(IDC_DEPTHDRAW)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	InvalidateRect(rect,FALSE);
	CancelToClose();
	return 0l;	
}

void CDepthColorPage::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rect;
	GetDlgItem(IDC_DEPTHDRAW)->GetWindowRect(&rect);
	ScreenToClient(rect);

	if (rect.PtInRect(point)==TRUE)
	{
		point.y-=rect.top;
		m_iColorFocus=point.y/(rect.Height()/pSettings_G->m_iNumDepthColors);
		InvalidateRect(rect,FALSE);
	}
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnLButtonDown(nFlags, point);
}

BOOL CDepthColorPage::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_pColorSheet->m_iRecommendedColorSchemeIndex=COLORSCHEME_DEPTH;
	RECT rect;
	GetDlgItem(IDC_NEWCOLORSLOT)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	m_ColorWidget.Create(this,rect,IDC_NEWCOLORSLOT+1);

	
	// TODO: Add your specialized creation code here
	CString CS;
	FloatToString(CS,pSettings_G->m_fDepthIncrements);
	SetDlgItemText(IDC_EDITINCREMENTS,CS);
	FloatToString(CS,pSettings_G->m_fStartDepth);
	SetDlgItemText(IDC_EDITSTARTDEPTH,CS);
	// TODO: Add extra initialization here

	GetDlgItem(IDC_DEPTHDRAW)->GetWindowRect(&rect);
	m_iMaxY=rect.bottom-rect.top;
	ResizeColorWidget();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDepthColorPage::OnWizard() 
{
	CDepthColorHelperDialog ACH(pSettings_G->m_fStartDepth,pSettings_G->m_fDepthIncrements,pSettings_G->m_iNumDepthColors);
	if (ACH.DoModal()==IDOK)
	{
		m_bDirty=TRUE;
		CancelToClose();
		pSettings_G->m_fStartDepth=ACH.m_fStartDepth;
		pSettings_G->m_fDepthIncrements=ACH.m_fDepth;
		pSettings_G->m_iNumDepthColors=ACH.m_iNumIntervals;
		m_iColorFocus=0;
		ResizeColorWidget();
	}
}

void CDepthColorPage::ResizeColorWidget()
{		//Resize widget slightly so that colors fit evenly
	CRect rect;

	GetDlgItem(IDC_DEPTHDRAW)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	GetDlgItem(IDC_DEPTHDRAW)->MoveWindow(rect.left,rect.top,rect.right-rect.left,(m_iMaxY/pSettings_G->m_iNumDepthColors)*pSettings_G->m_iNumDepthColors);
	InvalidateRect(NULL,TRUE);
}

void CDepthColorPage::OnEvensplit() 
{
	CDepthLevelDialog DLD;
	DLD.m_DepthLevel=pSettings_G->m_iNumDepthColors;
	if (DLD.DoModal()==IDOK)
	{
		m_bDirty=TRUE;
		CancelToClose();
		pSettings_G->SetDocumentDepthGrid(pDocument_G,pView_G,DLD.m_DepthLevel);
		m_iColorFocus=0;
		ResizeColorWidget();
	}
}

void CDepthColorPage::OnButtondefaults() 
{
	m_bDirty=TRUE;
	pSettings_G->SetDocumentDepthGrid(pDocument_G);
	m_iColorFocus=0;
	ResizeColorWidget();
}
