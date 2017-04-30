// (C) Copyright 1995,  Taco van Ieperen
//
// Code for a ruler along the left side of the screen.


/////////////////////////////////////////////////////////////////////////////
// CRuler
#include "stdafx.h"
#include "onstation.h"
#include "ruler.h"
#include "afxpriv.h"
#include "settings.h"
#include "onstationview.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern COnStationApp theApp;

CRuler::CRuler()
{                      											 
	m_Scale=100.0f;
	m_View=NULL;
}

CRuler::~CRuler()
{
}

BOOL CRuler::Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID)
{
	ASSERT_VALID(pParentWnd);   // must have a parent

	// create the HWND
	CRect rect;
	rect.SetRectEmpty();
	BOOL bRet=CWnd::Create(NULL, _T(""), dwStyle, rect, pParentWnd, nID);
	EnableToolTips(TRUE);
	return bRet;
}

BEGIN_MESSAGE_MAP(CRuler, CWnd)
	//{{AFX_MSG_MAP(CRuler)
	ON_WM_PAINT()
	ON_MESSAGE(WM_SIZEPARENT,OnSizeParent)
//	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXT,0,0xffff,OnToolTipText)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_RULER_EXPAND50,OnExpand50)
	ON_COMMAND(ID_RULER_SHRINK50,OnShrink50)
	ON_COMMAND(ID_RULER_MAXIMUMSHRINK,OnShrinkMaximum)
	ON_COMMAND(ID_RULER_MAXIMUMZOOM,OnZoomMaximum)
	ON_COMMAND(ID_RULER_5MCM,OnZoom5)
	ON_COMMAND(ID_RULER_10MCM,OnZoom10)
	ON_COMMAND(ID_RULER_25MCM,OnZoom25)
	ON_COMMAND(ID_RULER_50MCM,OnZoom50)
	ON_COMMAND(ID_RULER_100MCM,OnZoom100)
	ON_COMMAND(ID_RULER_500MCM,OnZoom500)
	ON_COMMAND(ID_RULER_1000MCM,OnZoom1000)
	ON_COMMAND(ID_RULER_10FTINCH,OnZoom10I)
	ON_COMMAND(ID_RULER_20FTINCH,OnZoom20I)
	ON_COMMAND(ID_RULER_50FTINCH,OnZoom50I)
	ON_COMMAND(ID_RULER_100FTINCH,OnZoom100I)
	ON_COMMAND(ID_RULER_500FTINCH,OnZoom500I)
	ON_COMMAND(ID_RULER_1000FTINCH,OnZoom1000I)
	ON_COMMAND(ID_RULER_5000FTINCH,OnZoom5000I)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CRuler::OnExpand50()
{
	if (m_View!=NULL)
		m_View->SetExactView(NAN,NAN,m_Scale*0.5f,NAN,NAN,NAN);
}

void CRuler::OnShrink50()
{
	if (m_View!=NULL)
		m_View->SetExactView(NAN,NAN,m_Scale*1.5f,NAN,NAN,NAN);
}
void CRuler::OnZoom5()
{
	if (m_View!=NULL)
		m_View->SetExactView(NAN,NAN,5.0f,NAN,NAN,NAN);
}
void CRuler::OnZoom10()
{
	if (m_View!=NULL)
		m_View->SetExactView(NAN,NAN,10.0f,NAN,NAN,NAN);
}
void CRuler::OnZoom25()
{
	if (m_View!=NULL)
		m_View->SetExactView(NAN,NAN,25.0f,NAN,NAN,NAN);
}
void CRuler::OnZoom50()
{
	if (m_View!=NULL)
		m_View->SetExactView(NAN,NAN,50.0f,NAN,NAN,NAN);
}
void CRuler::OnZoom100()
{
	if (m_View!=NULL)
		m_View->SetExactView(NAN,NAN,100.0f,NAN,NAN,NAN);
}
void CRuler::OnZoom500()
{
	if (m_View!=NULL)
		m_View->SetExactView(NAN,NAN,500.0f,NAN,NAN,NAN);
}
void CRuler::OnZoom1000()
{
	if (m_View!=NULL)
		m_View->SetExactView(NAN,NAN,1000.0f,NAN,NAN,NAN);
}
void CRuler::OnZoom10I()
{
	if (m_View!=NULL)
		m_View->SetExactView(NAN,NAN,FeetToMeters(10.0f)/CMPERINCH,NAN,NAN,NAN);
}
void CRuler::OnZoom20I()
{
	if (m_View!=NULL)
		m_View->SetExactView(NAN,NAN,FeetToMeters(20.0f)/CMPERINCH,NAN,NAN,NAN);
}
void CRuler::OnZoom50I()
{
	if (m_View!=NULL)
		m_View->SetExactView(NAN,NAN,FeetToMeters(50.0f)/CMPERINCH,NAN,NAN,NAN);
}
void CRuler::OnZoom100I()
{
	if (m_View!=NULL)
		m_View->SetExactView(NAN,NAN,FeetToMeters(100.0f)/CMPERINCH,NAN,NAN,NAN);
}
void CRuler::OnZoom500I()
{
	if (m_View!=NULL)
		m_View->SetExactView(NAN,NAN,FeetToMeters(500.0f)/CMPERINCH,NAN,NAN,NAN);
}
void CRuler::OnZoom1000I()
{
	if (m_View!=NULL)
		m_View->SetExactView(NAN,NAN,FeetToMeters(1000.0f)/CMPERINCH,NAN,NAN,NAN);
}
void CRuler::OnZoom5000I()
{
	if (m_View!=NULL)
		m_View->SetExactView(NAN,NAN,FeetToMeters(5000.0f)/CMPERINCH,NAN,NAN,NAN);
}

void CRuler::OnZoomMaximum()
{
	if (m_View!=NULL)
		m_View->SetExactView(NAN,NAN,MIN_ZOOM,NAN,NAN,NAN);
}

void CRuler::OnShrinkMaximum()
{
	if (m_View!=NULL)
		m_View->SetExactView(NAN,NAN,MAX_ZOOM,NAN,NAN,NAN);
}


void CRuler::UpdateScale(float fScale)
{
	m_Scale=fScale;
    InvalidateRect(NULL,FALSE);
}
void CRuler::OnPaint()
{
	const int INSIDE_FRAME=2;

	CFont MyFont;
	CPaintDC dc(this); // device context for painting

//  Select a font of exceptional beauty.
	MyFont.CreateFont(9,0,0,0,FW_THIN,FALSE,FALSE,FALSE,ANSI_CHARSET,
						OUT_DEFAULT_PRECIS,CLIP_CHARACTER_PRECIS,
						DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,
						_T("Arial"));
	CFont *oldFont=dc.SelectObject(&MyFont);

	CRect rBoundry;
	GetClientRect(&rBoundry);

	//Fill background
	::FillRect(dc.m_hDC,&rBoundry,(HBRUSH)GetStockObject(LTGRAY_BRUSH));

	//Draw inside edge in black
	dc.SelectStockObject(BLACK_PEN);

	rBoundry.InflateRect(-INSIDE_FRAME,-INSIDE_FRAME);
	dc.MoveTo(rBoundry.left,rBoundry.bottom-1);	//-1 due to rect thing
	dc.LineTo(rBoundry.left,rBoundry.top);
	dc.LineTo(rBoundry.right-1,rBoundry.top);
	//Draw other edge in Dark Grey
	CPen penDK(PS_SOLID,1,RGB(128,128,128));
	CPen *oldPen=dc.SelectObject(&penDK);
	dc.LineTo(rBoundry.right-1,rBoundry.bottom-1);
	dc.LineTo(rBoundry.left,rBoundry.bottom-1);
	dc.SelectObject(oldPen);
	penDK.DeleteObject();	

	//Fill inside with white.
	rBoundry.InflateRect(-1,-1);
	FillRect(dc.m_hDC,rBoundry,(HBRUSH)GetStockObject(WHITE_BRUSH));
  

	//Don't DRAW GUTS in perspective mode!
//	if (!m_bDisabled)
//	{
		dc.SetMapMode(MM_LOMETRIC);

		float fUnits;
		GetReasonableIncrements(m_Scale,&fUnits);
		//How many .1 mm apart will these have to be.  To do this, we simply
		//take the scale 10m/cm and units (every 5 m) to get 5m/10m/cm=.5cm=
		float fRulerSpacing;
		fRulerSpacing=fUnits/m_Scale*100.0f;

		rBoundry.InflateRect(-1,-1);	//inside white frame	
		//Draw ruler lines and text.
		dc.SelectStockObject(BLACK_PEN);
		int iPosY=(int)fRulerSpacing;
		float fLength=fUnits;
		dc.SetMapMode(MM_LOMETRIC);
		dc.DPtoLP(&rBoundry);

		//Height is negative in a LOMETRIC rectangle.  This is due to the
		//fact that the Y axis is UP, and height is a stupid call which
		//does not account for this.
		while (iPosY<-rBoundry.Height())	//Bit of a safety
		{
		 	if (iPosY/(int)(fRulerSpacing)%5!=0)
		 	{
				dc.MoveTo(rBoundry.left,rBoundry.top-iPosY);
				dc.LineTo(rBoundry.left+rBoundry.Width()/3,rBoundry.top-iPosY);
		 		//Small Tick
		 	}
		 	else
		 	{
		 		//every 5 units do a tick
				TCHAR szText[10];
				int iLength;
				if (theApp.GetSettings()->IsMetric())
				{
					iLength=_stprintf_s(szText, _T("%.0f"),fLength);
				}
				else
				{
					iLength=_stprintf_s(szText, _T("%.0f"),MetersToFeet(fLength));
				}
				CPoint ptMMText;
				ptMMText.x=rBoundry.left;
				ptMMText.y=rBoundry.top-iPosY+5;
				dc.LPtoDP(&ptMMText);
				dc.SetMapMode(MM_TEXT);
				dc.ExtTextOut(ptMMText.x,ptMMText.y,0,NULL,szText,iLength,NULL);
				dc.SetMapMode(MM_LOMETRIC);
		 	}
		 	iPosY=iPosY+(int)fRulerSpacing;
			fLength+=fUnits;
		}
		dc.SetMapMode(MM_TEXT);
		dc.SelectObject(oldFont);
		MyFont.DeleteObject();
//	}		//END OF PERSPECTIVE CHECK
}


//Figure out what interval of tick to use.  TempScale is in m/cm
//We try to calculate
//a scale for the ruler to the nearest multiple of 5.  The idea is that
//if you have 15.0 meters/cm then your scale would be 1 cm exactly.
//if you have 15.7 meters/cm your scale would be slightly larger then
//one cm and your gradations would still be 15 meters.
void CRuler::GetReasonableIncrements(float fTempScale,float *pfUnits)
{
	if (!theApp.GetSettings()->IsMetric())
	{
		fTempScale=MetersToFeet(fTempScale)*CMPERINCH;
	}

	//First see if we have some sort of reasonable scale (ie, divisible by 5)
	//in which case we return that unit.
	int iRounded;
	iRounded=(int)((fTempScale+0.499f)/5.0f);	//round a little
	*pfUnits=(float)(iRounded*5);
	if (!theApp.GetSettings()->IsMetric())
	{
		*pfUnits=FeetToMeters(*pfUnits)/2;
	}
	if (*pfUnits<5.0f)
	{
		*pfUnits=5.0f;
	}
}

LRESULT CRuler::OnSizeParent(WPARAM wParam,LPARAM lParam)
{
	if (GetStyle()&WS_VISIBLE)
	{
		AFX_SIZEPARENTPARAMS * SizeParams=(AFX_SIZEPARENTPARAMS *)lParam;
		DeferWindowPos(SizeParams->hDWP,m_hWnd,NULL,SizeParams->rect.left,SizeParams->rect.top,
								RULER_WIDTH,SizeParams->rect.bottom-SizeParams->rect.top,SWP_NOZORDER);
		SizeParams->rect.left=SizeParams->rect.left+RULER_WIDTH;
		SizeParams->sizeTotal.cx+=RULER_WIDTH;
	
		//TOP and BOTTOM docking are not supported right now
	}
	return 0l;
}

void CRuler::OnRButtonDown(UINT nFlags, CPoint point) 
{
	//These things all feed back to the view port, so don't
	//even bother unless a view port exists
	if (m_View!=NULL)
	{
		CMenu Menu;
		ClientToScreen(&point);
		if (theApp.GetSettings()->IsMetric())
		{
			VERIFY(Menu.LoadMenu(IDR_RULERMENU));
		}
		else
		{
			VERIFY(Menu.LoadMenu(IDR_RULERMENUINCHES));
		}
		CMenu* pPopup = Menu.GetSubMenu(0);
		if (pPopup!=NULL)
		{
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x,point.y,this);
		}
		Menu.DestroyMenu();
	}
}




BOOL CRuler::OnToolTipText(UINT uId,NMHDR * pNMHDR,LRESULT *pResult)
{
	TOOLTIPTEXT * Text=(TOOLTIPTEXT *)pNMHDR;
	if (!theApp.GetSettings()->IsMetric())
	{
		lstrcpy(Text->szText, _T("Ruler - Shows scale in ft/inch"));
	}
	else
	{
		lstrcpy(Text->szText, _T("Ruler - Shows scale in m/cm"));
	}
	*pResult=0;
	return TRUE;
}

INT_PTR CRuler::OnToolHitTest(CPoint point,TOOLINFO *pTI) const
{
	RECT r;
	GetClientRect(&r);
	pTI->lpszText=LPSTR_TEXTCALLBACK;
	pTI->hwnd=m_hWnd;
	pTI->uId=100;
	pTI->rect=r;
	return 1;
}
