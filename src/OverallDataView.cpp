// OverallDataView.cpp : implementation file
//

#include "stdafx.h"
#include "onstation.h"
#include "OverallDataView.h"
#include "onstationdoc.h"
#include "realfolder.h"
#include "treehelper.h"
#include "mainfrm.h"
#include "surfacepropertysheet.h"
#include "legquery.h"
#include "onstationview.h"
#include "surveyleg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSettings * pSettings_G;
extern COnStationDoc * pDocument_G;
extern COnStationView * pView_G;
/////////////////////////////////////////////////////////////////////////////
// COverallDataView

COverallDataView::COverallDataView()
	: CDialogBar(),m_colorWidget(TRUE,2,8)
{
	//{{AFX_DATA_INIT(COverallDataView)
	//}}AFX_DATA_INIT
}	



COverallDataView::~COverallDataView()
{
}

BEGIN_MESSAGE_MAP(COverallDataView, CDialogBar)
	//{{AFX_MSG_MAP(COverallDataView)
	ON_COMMAND(ID_DRAWMODE_COLORSCHEME1, OnDrawmodeColorscheme1)
	ON_COMMAND(ID_DRAWMODE_COLORSCHEME2, OnDrawmodeColorscheme2)
	ON_COMMAND(ID_DRAWMODE_COLORSCHEME3, OnDrawmodeColorscheme3)
	ON_COMMAND(ID_DRAWMODE_COLORSCHEME4, OnDrawmodeColorscheme4)
	ON_COMMAND(ID_SURVEYTREE_EDITCURRENTITEM,OnRMenuEdit)
	ON_COMMAND(ID_SURVEYTREE_DELETECURRENTITEM,OnRMenuDelete)
	ON_COMMAND(ID_SURVEYTREE_NEWFOLDER,OnRMenuAddFolder)
	ON_COMMAND(ID_SURVEYTREE_NEWSURVEY,OnRMenuAddSurvey)
	ON_COMMAND(ID_SURVEYTREE_NEWFIXEDPOINT,OnRMenuAddFixedPoint)
	ON_COMMAND(ID_DRAWMODE_COLORSCHEME5, OnDrawmodeColorscheme5)
	ON_COMMAND(ID_DRAWMODE_COLORSCHEME6, OnDrawmodeColorscheme6)
	ON_COMMAND(ID_DRAWMODE_COLORSCHEME7, OnDrawmodeColorscheme7)
	ON_COMMAND(ID_DRAWMODE_COLORSCHEME8, OnDrawmodeColorscheme8)
	ON_WM_CREATE()
	ON_MESSAGE(WM_COLORCHANGE, OnColorChange)
	ON_COMMAND(ID_EDIT_PROPERTIES, OnEditProperties)
	ON_WM_DESTROY()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_DRAWMODE_DEPTHCODING, OnDrawmodeDepthcoding)
	ON_COMMAND(ID_DRAWMODE_AGECOLORING, OnDrawmodeAgecoloring)
	ON_COMMAND(ID_DRAWMODE_BLUNDERCOLOR, OnDrawmodeBlunderColoring)
	ON_BN_CLICKED(ID_EDIT_CONFIGURECOLORS, OnConfigurecolors)
	ON_WM_SIZE()
	ON_COMMAND(ID_SURVEYTREE_RAINBOWCOLORSURVEYS, OnSurveytreeRainbowcolorsurveys)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COverallDataView diagnostics
/////////////////////////////////////////////////////////////////////////////
// COverallDataView message handlers
int COverallDataView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	return TRUE;
}

void COverallDataView::InitialUpdate()
{
	static BOOL m_bUpdatedBefore=FALSE;
	if (!m_bUpdatedBefore)
	{
		m_bUpdatedBefore=TRUE;
		m_masterSurveyList.Attach(::GetDlgItem(m_hWnd,IDC_SURVEYLIST));
		m_colorSchemeComboBox.Attach(::GetDlgItem(m_hWnd,IDC_COMBO1));
		CRect r;
		GetDlgItem(IDC_COLORSLOT)->GetWindowRect(r);
		ScreenToClient(r);
		m_colorWidget.Create(this,r,IDC_COLORSLOT+1);
		CFont * pFont=CFont::FromHandle((HFONT)::GetStockObject(DEFAULT_GUI_FONT));
		m_colorWidget.SetFont(pFont);
		m_masterSurveyList.SetFont(pFont);
		m_masterSurveyList.InitialUpdate(this);
		m_colorSchemeComboBox.InitialUpdate();
	}
	else
	{										  
		m_masterSurveyList.DeleteAllItems();
		m_masterSurveyList.UpdateItems();
		//This will rename all the color schemes
		m_colorSchemeComboBox.InitialUpdate();
	}
}

void COverallDataView::SetColorSchemeIndex(int iColorSchemeIndex)
{
	m_masterSurveyList.SetColorScheme(iColorSchemeIndex);
	m_colorSchemeComboBox.SetCurSel(iColorSchemeIndex);
	UpdateColorControl(iColorSchemeIndex);
}

void COverallDataView::OnChangedDocumentData()
{
	m_masterSurveyList.UpdateItems();
}

void COverallDataView::RemoveAll()
{
	m_masterSurveyList.DeleteAllItems();
	m_colorSchemeComboBox.UpdateEntries();
	SetQuery(NULL);
}

void COverallDataView::OnDrawmodeColorscheme1() 
{
	m_masterSurveyList.SetColorScheme(0);
	//GetActiveView() for CMainFrame returns NULL when
	//this dialog is active because this dialog is considered to
	//be the active view in some weird way.  For this reason we
	//remember the last active view becuase we need to know it
	//for some operations.
	pView_G->PickColorScheme(0);
	UpdateColorControl(0);
}

void COverallDataView::OnDrawmodeColorscheme2() 
{
	m_masterSurveyList.SetColorScheme(1);
	pView_G->PickColorScheme(1);
	UpdateColorControl(1);
}

void COverallDataView::OnDrawmodeColorscheme3() 
{
	m_masterSurveyList.SetColorScheme(2);
	pView_G->PickColorScheme(2);
	UpdateColorControl(2);
}

void COverallDataView::OnDrawmodeColorscheme4() 
{
	m_masterSurveyList.SetColorScheme(3);
	pView_G->PickColorScheme(3);
	UpdateColorControl(3);
}

void COverallDataView::OnDrawmodeColorscheme5() 
{
	m_masterSurveyList.SetColorScheme(4);
	pView_G->PickColorScheme(4);
	UpdateColorControl(4);
}

void COverallDataView::OnDrawmodeColorscheme6() 
{
	m_masterSurveyList.SetColorScheme(5);
	pView_G->PickColorScheme(5);
	UpdateColorControl(5);
}

void COverallDataView::OnDrawmodeColorscheme7() 
{
	m_masterSurveyList.SetColorScheme(6);
	pView_G->PickColorScheme(6);
	UpdateColorControl(6);
}

void COverallDataView::OnDrawmodeColorscheme8() 
{
	m_masterSurveyList.SetColorScheme(7);
	pView_G->PickColorScheme(7);
	UpdateColorControl(7);
}

void COverallDataView::UpdateColorControl(int iColorSchemeIndex)
{
	if (iColorSchemeIndex==COLORSCHEME_AGE || iColorSchemeIndex==COLORSCHEME_DEPTH || iColorSchemeIndex==COLORSCHEME_BLUNDER)
	{
		GetDlgItem(ID_EDIT_CONFIGURECOLORS)->ShowWindow(SW_SHOW);
		m_colorWidget.EnableDisableOnly(TRUE);
	}
	else
	{
		GetDlgItem(ID_EDIT_CONFIGURECOLORS)->ShowWindow(SW_HIDE);
		m_colorWidget.EnableDisableOnly(FALSE);
	}
}

void COverallDataView::EnableColorWidget(BOOL bEnable)
{
	if (m_colorWidget.IsWindowEnabled()!=bEnable)
	{
		m_colorWidget.EnableWindow(bEnable);
		m_colorWidget.InvalidateRect(NULL,FALSE);
	}
}


void COverallDataView::OnCut() 
{
	m_masterSurveyList.OnCut();
}

void COverallDataView::OnCopy() 
{
	m_masterSurveyList.OnCopy();
}

void COverallDataView::OnPaste() 
{
	m_masterSurveyList.OnPaste();
}

void COverallDataView::OnDelete() 
{
	m_masterSurveyList.OnDelete();
}

void COverallDataView::OnEdit() 
{
	m_masterSurveyList.OnEditSelectedItem();
}
void COverallDataView::OnAddFixedPoint()
{
	m_masterSurveyList.OnNewFixedPoint();
}
void COverallDataView::OnUpdateEditEdit(CCmdUI * pCmdUI)
{
	m_masterSurveyList.OnUpdateEditEdit(pCmdUI);
}
void COverallDataView::OnUpdateEditDelete(CCmdUI * pCmdUI)
{
	m_masterSurveyList.OnUpdateEditDelete(pCmdUI);
}
void COverallDataView::OnUpdateEditCopy(CCmdUI * pCmdUI)
{
	m_masterSurveyList.OnUpdateEditCopy(pCmdUI);
}

void COverallDataView::OnAddFolder()
{
	m_masterSurveyList.OnNewfolder();
}

void COverallDataView::OnAddSurvey()
{
	m_masterSurveyList.OnNewsurvey();
}

LONG_PTR COverallDataView::OnColorChange(UINT_PTR, LONG_PTR lParam)
{
	CFolder * pFolder=m_masterSurveyList.GetSelectedRealFolder();
	if (pFolder==NULL)
	{
		pFolder=m_masterSurveyList.GetSelectedSurveyLeg();
	}
	if (pFolder!=NULL)
	{
		pDocument_G->SetModifiedFlag();
		BOOL bDisabled=FALSE;	//do we need to recalculate any views
		int iColorSchemeIndex=m_colorSchemeComboBox.GetColorScheme();
		if (iColorSchemeIndex==-1)
		{
			m_colorSchemeComboBox.SetCurSel(0);
			iColorSchemeIndex=0;
		}
		pFolder->SetColor((int)lParam,iColorSchemeIndex,FALSE,&bDisabled,m_masterSurveyList.GetLegQuery());
		pView_G->RecalculateGeometry(bDisabled);
		m_masterSurveyList.InvalidateRect(NULL,FALSE);
	}
	else
	{
		CSurfaceData * pSurface=m_masterSurveyList.GetSelectedSurface();
		if (pSurface!=NULL)
		{
			pDocument_G->SetModifiedFlag();
			pSettings_G->m_cSurfaceColors[pSettings_G->m_iColorSchemeIndex]=(COLORREF)lParam;
			pView_G->RecalculateGeometry(FALSE);
		}
	}
	return 0l;	
}
void COverallDataView::OnRMenuEdit()
{
	OnEdit();
}

void COverallDataView::OnRMenuDelete()
{
	m_masterSurveyList.OnDelete();
}

void COverallDataView::OnRMenuAddFolder()
{
	m_masterSurveyList.OnNewfolder();
}

void COverallDataView::OnRMenuAddSurvey()
{
	m_masterSurveyList.OnNewsurvey();
}

void COverallDataView::OnRMenuAddFixedPoint()
{
	m_masterSurveyList.OnNewFixedPoint();
}

void COverallDataView::OnEditProperties() 
{
	OnShowProperties();
}

void COverallDataView::OnShowProperties()
{
	m_masterSurveyList.OnShowProperties();
}

void COverallDataView::SetQuery(CLegQuery * pLegQuery)
{
	//Quick sanity check to save on ugly and slow redraws
	if (m_masterSurveyList.GetLegQuery()==NULL && pLegQuery==NULL)
 	{
		return;
	}

	m_masterSurveyList.SetQuery(pLegQuery);
	//WE get the query from the master survey list.  This is
	//because the master survey list may have ignored our
	//query and set it to NULL instead.
	if (m_masterSurveyList.GetLegQuery()==NULL)
	{
		//Hide the static query text
		GetDlgItem(IDC_STATICQUERYTEXT)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_STATICQUERYTEXT)->ShowWindow(SW_SHOW);
		CString CS;
		pLegQuery->GetDisplayText(&CS);
		GetDlgItem(IDC_STATICQUERYTEXT)->SetWindowText(CS);
	}
	CRect rDialog;
	GetWindowRect(rDialog);
	CRect r;
	m_masterSurveyList.GetWindowRect(r);
	ScreenToClient(&r);
	if (pLegQuery==NULL)
	{
		r.bottom=rDialog.Height()-10;
	}
	else
	{
		r.bottom=rDialog.Height()-30;	//leave room for leg query text
	}
	m_masterSurveyList.MoveWindow(r.left,r.top,r.Width(),r.Height(),TRUE);
	InvalidateRect(NULL,TRUE);
}


void COverallDataView::OnDestroy() 
{
	m_masterSurveyList.Detach();
	m_colorSchemeComboBox.Detach();
	CDialogBar::OnDestroy();
}

void COverallDataView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_masterSurveyList.IsDragging())
	{
		ClientToScreen(&point);
		m_masterSurveyList.OnLButtonUp(nFlags,point);
	}
	else
	{
		CDialogBar::OnLButtonUp(nFlags, point);
	}
}

void COverallDataView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_masterSurveyList.IsDragging())
	{
		ClientToScreen(&point);
		m_masterSurveyList.OnMouseMove(nFlags, point);
	}
	else
	{
		CDialogBar::OnMouseMove(nFlags, point);
	}
}

void COverallDataView::OnDrawmodeDepthcoding() 
{
	m_masterSurveyList.SetColorScheme(COLORSCHEME_DEPTH);
	pView_G->PickColorScheme(COLORSCHEME_DEPTH);
	UpdateColorControl(COLORSCHEME_DEPTH);
}

void COverallDataView::OnDrawmodeAgecoloring() 
{
	m_masterSurveyList.SetColorScheme(COLORSCHEME_AGE);
	pView_G->PickColorScheme(COLORSCHEME_AGE);
	UpdateColorControl(COLORSCHEME_AGE);
}

void COverallDataView::OnDrawmodeBlunderColoring() 
{
	m_masterSurveyList.SetColorScheme(COLORSCHEME_BLUNDER);
	pView_G->PickColorScheme(COLORSCHEME_BLUNDER);
	UpdateColorControl(COLORSCHEME_BLUNDER);
}

void COverallDataView::OnConfigurecolors() 
{
	pView_G->PostMessage(WM_COMMAND,ID_EDIT_CONFIGURECOLORS,0l);
}

void COverallDataView::OnSize(UINT nType, int cx, int cy) 
{
	CDialogBar::OnSize(nType, cx, cy);
	if (cy>100 && ::IsWindow(m_masterSurveyList.m_hWnd))
	{
		CRect r;
		m_masterSurveyList.GetWindowRect(r);
		ScreenToClient(&r);
		r.bottom=cy-10;
		m_masterSurveyList.MoveWindow(r.left,r.top,r.Width(),r.Height(),TRUE);
		r.top=r.bottom-15;
		GetDlgItem(IDC_STATICQUERYTEXT)->MoveWindow(r.left,r.top,r.Width(),r.Height(),TRUE);
	}
}

void COverallDataView::OnSurveytreeRainbowcolorsurveys() 
{
	int iColorSchemeIndex=m_colorSchemeComboBox.GetColorScheme();
	if (iColorSchemeIndex>=MAX_REGULAR_COLOR_SCHEMES)
	{
		SetColorSchemeIndex(0);
		iColorSchemeIndex=0;
	}
	SetQuery(NULL);
	CFolder * pFolder=pDocument_G->GetSurveyFolder();
	if (pFolder!=NULL)
	{
		BOOL bDisabled=FALSE;	//do we need to recalculate any views
		pFolder->SetColor(RGB(255,255,255),iColorSchemeIndex,TRUE,&bDisabled,NULL);
		pView_G->RecalculateGeometry(bDisabled);
		m_masterSurveyList.InvalidateRect(NULL,FALSE);
	}
}

void COverallDataView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if (m_masterSurveyList.GetLegQuery()!=NULL)
	{
		CRect r;
		CBrush brush;
		CBitmap bitmap;
		bitmap.LoadBitmap(IDB_QUESTIONBRUSH);
		brush.CreatePatternBrush(&bitmap);
		GetClientRect(r);
		dc.FillRect(r,&brush);
	}
}
