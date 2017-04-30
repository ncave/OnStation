// widget2.cpp : implementation file
//

#include "stdafx.h"
#include "onstation.h"
#include "compasswidget.h"
#include "afxpriv.h"
#include "onstationview.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern COnStationApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CCompassWidget

CCompassWidget::CCompassWidget()
{
	m_fClino=0.0f;
	m_fCompass=0.0f;
	m_pView=NULL;
}

void CCompassWidget::SetClinoAndCompass(float fClino,float fCompass)
{
	m_fClino=(double)fClino;
	m_fCompass=(double)fCompass;
	if (m_hWnd!=NULL)
	{
		InvalidateRect(NULL,FALSE);
	}
}

CCompassWidget::~CCompassWidget()
{
}

BEGIN_MESSAGE_MAP(CCompassWidget, CWnd)
	//{{AFX_MSG_MAP(CCompassWidget)
//	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXT,0,0xffff,OnToolTipText)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_NORTH,OnNorth)
	ON_COMMAND(ID_SOUTH,OnSouth)
	ON_COMMAND(ID_EAST,OnEast)
	ON_COMMAND(ID_WEST,OnWest)
	ON_COMMAND(ID_NORTHEAST,OnNorthEast)
	ON_COMMAND(ID_NORTHWEST,OnNorthWest)
	ON_COMMAND(ID_SOUTHEAST,OnSouthEast)
	ON_COMMAND(ID_SOUTHWEST,OnSouthWest)
	ON_COMMAND(ID_TILT0,OnTilt0)
	ON_COMMAND(ID_TILT45,OnTilt45)
	ON_COMMAND(ID_TILT90,OnTilt90)
	ON_COMMAND(ID_TILT135,OnTilt135)
	ON_COMMAND(ID_TILT180,OnTilt180)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCompassWidget message handlers


void CCompassWidget::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CDC dc2;
	dc2.CreateCompatibleDC(&dc);
	CBitmap Bm2;
	Bm2.LoadBitmap(IDB_BITMAPGLASS);
	CBitmap *bmOld=dc2.SelectObject(&Bm2);
	dc.BitBlt(0,0,64,64,&dc2,0,0,SRCCOPY);
	
	dc.SelectStockObject(BLACK_PEN);
	DrawClinoLine(dc,m_fClino,TRUE);
		
	CPen Pen(PS_SOLID,3,RGB(255,0,0));
	CPen *oldPen=dc.SelectObject(&Pen);
	DrawCompassLine(dc,m_fCompass);
	
	dc.SelectObject(oldPen);
 	
	dc2.SelectObject(bmOld);
}

BOOL CCompassWidget::Create(CWnd *parent,int ID)
{
	//This code is complete crap.  It just adds fudge factors
	//until it looks good.
	CRect rect;
	rect.left=0;
	rect.right=64+5;
	rect.top=0;
	rect.bottom=4+64+GetSystemMetrics(SM_CYCAPTION)+GetSystemMetrics(SM_CYBORDER);
	EnableToolTips(TRUE);
	//SetRectEmpty();
	return CWnd::Create(NULL, _T("Compass"),WS_SYSMENU|WS_VISIBLE|WS_CAPTION|WS_CHILD|WS_BORDER,rect,parent,ID);
}

void CCompassWidget::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (ClickedOnCompass(point))
	{
		TrackDrag(point,TRUE);
	}
	else if (ClickedOnTilter(point))
	{
		TrackDrag(point,FALSE);
	}
	CWnd::OnLButtonDown(nFlags, point);
}

void CCompassWidget::TrackDrag(POINT ptStart,BOOL bCompass)
{
	SetCapture();
	SetCursor(GetAngleCursor(ptStart));
	
	RECT r={0,0,63,63};
	ClientToScreen(&r);
	ClipCursor(&r);
	double dAngle;
	
	//Set up for drawing
	CDC *dcDraw=GetDC();	
	CPen pen(PS_SOLID,bCompass?3:1,RGB(255,255,255));
	CPen *oldPen=dcDraw->SelectObject(&pen);
	int iOldRop=dcDraw->SetROP2(R2_XORPEN);
	if (bCompass)
	{
		dAngle=m_fCompass;
		DrawCompassLine(*dcDraw,dAngle);
	}
	else
	{
		dAngle=m_fClino;
		DrawClinoLine(*dcDraw,dAngle,FALSE);
	}



	// get messages until capture lost or cancelled/accepted
	for (;;)
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		switch (msg.message)
		{
		// handle movement/accept messages
		case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x=(int)(short)LOWORD(msg.lParam);
			pt.y=(int)(short)HIWORD(msg.lParam);
			double dAngle2=GetMousePosAngle(pt,bCompass);
			if (dAngle2!=-1000.0)
			{
				dAngle=dAngle2;
				if (bCompass)
				{
					m_fCompass=dAngle;
				}
				else
				{
					m_fClino=dAngle;
				}
			}
			if (m_pView!=NULL)
			{
				m_pView->SetExactView((float)m_fCompass,(float)m_fClino,NAN,NAN,NAN,NAN);
			}
			goto ExitLoop;
		}
			break;
			
		case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x=(int)(short)LOWORD(msg.lParam);
			pt.y=(int)(short)HIWORD(msg.lParam);
			SetCursor(GetAngleCursor(pt));
			double dAngle2=GetMousePosAngle(pt,bCompass);
			if (dAngle2!=-1000.0)
			{
				//Erase old
				if (bCompass)
				{
					DrawCompassLine(*dcDraw,dAngle);
				}
				else
				{
					DrawClinoLine(*dcDraw,dAngle,FALSE);
				}
				//Draw new
				dAngle=dAngle2;
				if (bCompass)
				{
					DrawCompassLine(*dcDraw,dAngle);
				}
				else
				{
					DrawClinoLine(*dcDraw,dAngle,FALSE);
				}
			}
		}
			break;

		// handle cancel messages
		case WM_KEYDOWN:
			if (msg.wParam != VK_ESCAPE)
				break;
		case WM_RBUTTONDOWN:
			goto ExitLoop;

		// just dispatch rest of the messages
		default:
			DispatchMessage(&msg);
			break;
		}
	}

ExitLoop:
	dcDraw->SetROP2(iOldRop);
	dcDraw->SelectObject(oldPen);	
	ReleaseDC(dcDraw);
	ClipCursor(NULL);
	ReleaseCapture();
	InvalidateRect(NULL,FALSE);
}

BOOL CCompassWidget::ClickedOnCompass(POINT pt)
{
	//get the angle from the click point to the center.
	double d=GetMousePosAngle(pt,TRUE);		
	d=d-m_fCompass;
	if (d<-190.0)
		d=d+360.0;
	if (d>190.0)
		d=d-360;
	return (-5.0<d && d<5.0);
}

//Reuse the compass code, except check for +90 and -90 both.
//Otherwise it is hard to stop getting quadrants confused
//with the clinometer line.
BOOL CCompassWidget::ClickedOnTilter(POINT pt)
{
	double d=GetMousePosAngle(pt,TRUE);		
	d=d-m_fClino+90.0f;
	if (d<-190.0)
		d=d+360.0;
	if (d>190.0)
		d=d-360;
	if (-5.0<d && d<5.0)
		return TRUE;
	d=GetMousePosAngle(pt,TRUE);		
	d=d-m_fClino-90.0f;
	if (d<-190.0)
		d=d+360.0;
	if (d>190.0)
		d=d-360;
	return (-5.0<d && d<5.0);
}

double CCompassWidget::GetMousePosAngle(POINT point,BOOL bCompass)
{
	int dx=point.x-31;
	int dy=31-point.y;
	if (dx>5 || dx<-5 || dy>5 || dy<-5)
	{
		double dAngle=atan2((double)dx,(double)dy);
		dAngle=dAngle*180.0/3.1415926535;
		if (dAngle<0.0)
			dAngle=dAngle+360.0;
		if (!bCompass)
		{
			dAngle=dAngle-90.0;
			if (dAngle>90.0)
				dAngle=dAngle-180.0;
		}
		return dAngle;
	}
	else
	{
		return -1000.0;		//error code
	}
}

void CCompassWidget::DrawClinoLine(CDC &dc,double fAngle,BOOL bFill)
{
	fAngle=fAngle+90.0;
	
	fAngle=fAngle*3.14159/180.0;
	double sine=sin(fAngle);
	double cosine=cos(fAngle);
	double dx=28.0*sine;
	double dy=28.0*cosine;
	if (bFill)
	{
		CBrush Brush(RGB(0,0,255));
		CBrush *oldBrush=dc.SelectObject(&Brush);
		dc.Pie(5,4,58,59,31+(int)dx,31-(int)dy,31-(int)dx,31+(int)dy);
		CBrush Brush2(RGB(0,128,0));
		dc.SelectObject(&Brush2);
		dc.Pie(5,4,58,59,31-(int)dx,31+(int)dy,31+(int)dx,31-(int)dy);
		dc.SelectObject(oldBrush);
	}
	else
	{
		dc.MoveTo(31+(int)dx,31-(int)dy);
		dc.LineTo(31-(int)dx,31+(int)dy);
	}
}

void CCompassWidget::DrawCompassLine(CDC &dc,double fAngle)
{
	fAngle=fAngle*3.14159/180.0;
	double sine=sin(fAngle);
	double cosine=cos(fAngle);
	double dx=25.0*sine;
	double dy=25.0*cosine;	
	dc.MoveTo(31,31);
	dc.LineTo(31+(int)dx,31-(int)dy);
}


void CCompassWidget::OnMouseMove(UINT nFlags, CPoint point)
{
	if (ClickedOnCompass(point) || ClickedOnTilter(point))
	{
		SetCursor(GetAngleCursor(point));
	}
	CWnd::OnMouseMove(nFlags, point);
}

HCURSOR CCompassWidget::GetAngleCursor(CPoint const& pt)
{
	double dAngle=GetMousePosAngle(pt,TRUE);
	if ((dAngle>20.0 && dAngle<65.0) || (dAngle>200.0 && dAngle<245.0))
		return LoadCursor(NULL,IDC_SIZENWSE);
	if ((dAngle>=65.0 && dAngle<=125.0) || (dAngle>=245.0 && dAngle<305.0))
		return LoadCursor(NULL,IDC_SIZENS);
	if ((dAngle>125.0 && dAngle<160.0) || (dAngle>=305.0 && dAngle<335.0))
		return LoadCursor(NULL,IDC_SIZENESW);
	return LoadCursor(NULL,IDC_SIZEWE);
	
}

/*
LRESULT CCompassWidget::OnSizeParent(WPARAM wParam,LPARAM lParam)
{
	if (GetStyle()&WS_VISIBLE)
	{
		AFX_SIZEPARENTPARAMS * SizeParams=(AFX_SIZEPARENTPARAMS *)lParam;
		DeferWindowPos(SizeParams->hDWP,m_hWnd,NULL,SizeParams->rect.right-64,SizeParams->rect.bottom-64,
							64,64,SWP_NOZORDER);
	}
	return 0l;
}
*/

void CCompassWidget::OnRButtonDown(UINT nFlags, CPoint point) 
{
	//These things all feed back to the view port, so don't
	//even bother unless a view port exists
	if (m_pView!=NULL)
	{
		CMenu Menu;
		ClientToScreen(&point);
		VERIFY(Menu.LoadMenu(IDR_COMPASSMENU));
		CMenu* pPopup = Menu.GetSubMenu(0);
		if (pPopup!=NULL)
		{
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x,point.y,this);
		}
		Menu.DestroyMenu();
	}
}

void CCompassWidget::OnWest()
{
	if (m_pView!=NULL)
	{
		m_pView->SetExactView(270.0f,NAN,NAN,NAN,NAN,NAN);
	}
}
void CCompassWidget::OnEast()
{
	if (m_pView!=NULL)
	{
		m_pView->SetExactView(90.0f,NAN,NAN,NAN,NAN,NAN);
	}
}
void CCompassWidget::OnNorthWest()
{
	if (m_pView!=NULL)
	{
		m_pView->SetExactView(315.0f,NAN,NAN,NAN,NAN,NAN);
	}
}
void CCompassWidget::OnNorthEast()
{
	if (m_pView!=NULL)
	{
		m_pView->SetExactView(45.0f,NAN,NAN,NAN,NAN,NAN);
	}
}
void CCompassWidget::OnSouthWest()
{
	if (m_pView!=NULL)
	{
		m_pView->SetExactView(225.0f,NAN,NAN,NAN,NAN,NAN);
	}
}
void CCompassWidget::OnSouthEast()
{
	if (m_pView!=NULL)
	{
		m_pView->SetExactView(135.0f,NAN,NAN,NAN,NAN,NAN);
	}
}
void CCompassWidget::OnNorth()
{
	if (m_pView!=NULL)
	{
		m_pView->SetExactView(0.0f,NAN,NAN,NAN,NAN,NAN);
	}
}
void CCompassWidget::OnSouth()
{
	if (m_pView!=NULL)
	{
		m_pView->SetExactView(180.0f,NAN,NAN,NAN,NAN,NAN);
	}
}


void CCompassWidget::OnTilt0()
{
	if (m_pView!=NULL)
	{
		m_pView->SetExactView(NAN,0.0f,NAN,NAN,NAN,NAN);
	}
}

void CCompassWidget::OnTilt45()
{
	if (m_pView!=NULL)
	{
		m_pView->SetExactView(NAN,45.0f,NAN,NAN,NAN,NAN);
	}
}

void CCompassWidget::OnTilt90()
{
	if (m_pView!=NULL)
	{
		m_pView->SetExactView(NAN,90.0f,NAN,NAN,NAN,NAN);
	}
}

void CCompassWidget::OnTilt135()
{
	if (m_pView!=NULL)
	{
		m_pView->SetExactView(NAN,135.0f,NAN,NAN,NAN,NAN);
	}
}

void CCompassWidget::OnTilt180()
{
	if (m_pView!=NULL)
	{
		m_pView->SetExactView(NAN,180.0f,NAN,NAN,NAN,NAN);
	}
}



BOOL CCompassWidget::OnToolTipText(UINT uId,NMHDR * pNMHDR,LRESULT *pResult)
{
	TOOLTIPTEXT * Text=(TOOLTIPTEXT *)pNMHDR;
	lstrcpy(Text->szText, _T("Compass - Shows rotation. Needles can be dragged"));
	*pResult=0;
	return TRUE;
}

INT_PTR CCompassWidget::OnToolHitTest(CPoint point,TOOLINFO *pTI) const
{
	RECT r;
	GetClientRect(&r);
	pTI->lpszText=LPSTR_TEXTCALLBACK;
	pTI->hwnd=m_hWnd;
	pTI->uId=100;
	pTI->rect=r;
	return 1;
}


