#include "stdafx.h"
#include "onstation.h"
#include "colorwidget.h"
#include "colortool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CColorWidget::CColorWidget(BOOL bAllowNoUse,int iWidgetRows,int iWidgetCols)
{   
	m_iSelectedCell=0;
	m_bAllowNoUse=bAllowNoUse;
	m_iWidgetRows=iWidgetRows;
	m_iWidgetCols=iWidgetCols;
	m_bWidgetRightBoxes=iWidgetRows==2;
	m_bEnableDisableOnly=FALSE;
}

CColorWidget::~CColorWidget()
{
}

BEGIN_MESSAGE_MAP(CColorWidget, CWnd)
	//{{AFX_MSG_MAP(CColorWidget)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorWidget message handlers

void CColorWidget::OnPaint()
{
	BOOL bEnabled=IsWindowEnabled();
	CPaintDC dc(this); // device context for painting
	CFont * pFont=CFont::FromHandle((HFONT)::GetStockObject(DEFAULT_GUI_FONT));
	dc.SelectObject(pFont);

	CPen PenGray(PS_SOLID,1,GetSysColor(COLOR_BTNFACE));
	CPen PenGray2(PS_SOLID,1,GetSysColor(COLOR_BTNSHADOW));
	CPen PenWhite(PS_SOLID,1,GetSysColor(COLOR_BTNHILIGHT));	
	CRect r;

	CPen *oldPen=dc.SelectObject(&PenGray);//so we can restore the DC later
	if (!m_bEnableDisableOnly)
	{
		for (int x=0;x<m_iWidgetCols;x++)
		{
			for (int y=0;y<m_iWidgetRows;y++)
			{
				if (x*m_iWidgetRows+y<15)
				{
					r.left=x*WELL_SIZE;
					r.right=r.left+WELL_SIZE;
					r.top=y*WELL_SIZE;
					r.bottom=r.top+WELL_SIZE;
					CBrush *oldBrush=(CBrush *)dc.SelectStockObject(NULL_BRUSH);
					if (m_iSelectedCell!=x+y*m_iWidgetCols || !bEnabled)
					{
						dc.SelectObject(&PenGray);
					}
					else
					{
						dc.SelectStockObject(BLACK_PEN);
					}
					dc.Rectangle((LPCRECT)r);
					dc.SelectStockObject(NULL_PEN);
					dc.SelectObject(&PenWhite);
					r.InflateRect(-1,-1);
					dc.MoveTo(r.left,r.bottom-1);
					dc.LineTo(r.right-1,r.bottom-1);
					dc.LineTo(r.right-1,r.top);
					dc.SelectObject(&PenGray2);
					dc.LineTo(r.left,r.top);
					dc.LineTo(r.left,r.bottom);
					r.InflateRect(-1,-1);
					if (bEnabled)
					{
						dc.FillSolidRect(r,CGlobalColorManager::StandardVGAColors[x+y*m_iWidgetCols]);
					}
					else
					{
						dc.FillSolidRect(r,GetSysColor(COLOR_BTNFACE));
					}
				}
			}
		}
	}
	if (!m_bWidgetRightBoxes)
	{
		r.top=WELL_SIZE*m_iWidgetRows;
		r.bottom=WELL_SIZE*(m_iWidgetRows+1);
		r.left=0;
		r.right=WELL_SIZE*m_iWidgetCols;
	}
	else
	{
		if (m_bEnableDisableOnly)
		{
			//Draw the rectangle for white so we can bring back deleted
			//items
			r.left=0;
			r.right=WELL_SIZE;
			r.top=0;
			r.bottom=WELL_SIZE*2;
			CBrush *oldBrush=(CBrush *)dc.SelectStockObject(NULL_BRUSH);
			if (m_iSelectedCell!=0)
			{
				dc.SelectObject(&PenGray);
			}
			else
			{
				dc.SelectStockObject(BLACK_PEN);
			}
			dc.Rectangle((LPCRECT)r);
			dc.SelectStockObject(NULL_PEN);
			dc.SelectObject(&PenWhite);
			r.InflateRect(-1,-1);
			dc.MoveTo(r.left,r.bottom-1);
			dc.LineTo(r.right-1,r.bottom-1);
			dc.LineTo(r.right-1,r.top);
			dc.SelectObject(&PenGray2);
			dc.LineTo(r.left,r.top);
			dc.LineTo(r.left,r.bottom);
			r.left++;
			r.top++;
			if (bEnabled)
			{
				dc.FillSolidRect(r,CGlobalColorManager::StandardVGAColors[0]);	
			}
			else
			{
				dc.FillSolidRect(r,GetSysColor(COLOR_BTNFACE));
			}
			
			//Setup the regular drawing rectangle
			r.top=0;
			r.bottom=WELL_SIZE*1;
			r.left=WELL_SIZE;
			r.right=WELL_SIZE*4;
		}
		else
		{
			r.top=0;
			r.bottom=WELL_SIZE*1;
			r.left=WELL_SIZE*m_iWidgetCols;
			r.right=WELL_SIZE*(m_iWidgetCols+3);
		}
	}
	CBrush *oldBrush=(CBrush *)dc.SelectStockObject(NULL_BRUSH);
	dc.SetBkColor(RGB(192,192,192));
	if (m_iSelectedCell!=-1)
	{
		dc.SelectObject(&PenGray);
		dc.SetTextColor(RGB(128,128,128));
	}
	else
	{
		dc.SelectStockObject(BLACK_PEN);
		dc.SetTextColor(RGB(0,0,0));
	}
	dc.Rectangle((LPCRECT)r);
	dc.SelectObject(oldBrush);
	
	r.InflateRect(-1,-1);
	dc.SelectObject(&PenWhite);
	dc.MoveTo(r.left,r.bottom-1);
	dc.LineTo(r.right-1,r.bottom-1);
	dc.LineTo(r.right-1,r.top);
	dc.SelectObject(&PenGray2);
	dc.LineTo(r.left,r.top);
	dc.LineTo(r.left,r.bottom);

	r.InflateRect(-1,-1);
	CString CS;
	CS.LoadString(IDS_HIDE);
	dc.DrawText(CS,r,DT_CENTER);

	r.InflateRect(2,2);	//undo past injustices!
	r.top+=WELL_SIZE;
	r.bottom+=WELL_SIZE;

	if (m_bAllowNoUse)
	{
		oldBrush=(CBrush *)dc.SelectStockObject(NULL_BRUSH);
		if (m_iSelectedCell!=-2)
		{
			dc.SelectObject(&PenGray);
			dc.SetTextColor(RGB(128,128,128));
		}
		else
		{
			dc.SelectStockObject(BLACK_PEN);
			dc.SetTextColor(RGB(0,0,0));
		}
		dc.Rectangle((LPCRECT)r);
		dc.SelectObject(oldBrush);
		
		r.InflateRect(-1,-1);
		dc.SelectObject(&PenWhite);
		dc.MoveTo(r.left,r.bottom-1);
		dc.LineTo(r.right-1,r.bottom-1);
		dc.LineTo(r.right-1,r.top);
		dc.SelectObject(&PenGray2);
		dc.LineTo(r.left,r.top);
		dc.LineTo(r.left,r.bottom);

		r.InflateRect(-1,-1);
		CS.LoadString(IDS_DISABLE);
		dc.DrawText(CS,-1,r,DT_CENTER);
	}
	dc.SelectObject(oldPen);
}

void CColorWidget::OnLButtonDown(UINT nFlags, CPoint point)
{
	int iXpos=point.x/WELL_SIZE;
	int iYpos=point.y/WELL_SIZE;
	if (!m_bWidgetRightBoxes)
	{
		if (iYpos>(m_iWidgetRows+1))
		{
			m_iSelectedCell=-2;
			InvalidateRect(NULL,FALSE);
			GetParent()->PostMessage(WM_COLORCHANGE,0,(LPARAM)COLOR_INACTIVE);
			return;
		}
		if (iYpos>m_iWidgetRows)
		{
			m_iSelectedCell=-1;
			InvalidateRect(NULL,FALSE);
			GetParent()->PostMessage(WM_COLORCHANGE,0,(LPARAM)COLOR_INVISIBLE);
			return;
		}	
	}
	else
	{
		if (m_bEnableDisableOnly)
		{
			//Check to see if the while cell was selected.
			if (iXpos==0)
			{
				InvalidateRect(NULL,FALSE);
				m_iSelectedCell=0;
				GetParent()->PostMessage(WM_COLORCHANGE,0,0);
				return;
			}
			//We are only showing disabled stuff so we fudge
			//things a little here
			iXpos=iXpos+m_iWidgetCols+1;
		}
		if (iXpos>=m_iWidgetCols)
		{
			if (iYpos==0)
			{
				m_iSelectedCell=-1;
			}
			else
			{
				m_iSelectedCell=-2;
			}
			InvalidateRect(NULL,FALSE);
			GetParent()->PostMessage(WM_COLORCHANGE,0,m_iSelectedCell==-2?(LPARAM)COLOR_INACTIVE:(LPARAM)COLOR_INVISIBLE);
			return;
		}
	}
	int iCheckValidCell=iXpos+iYpos*m_iWidgetCols;
	if (iCheckValidCell<15)
	{
		m_iSelectedCell=iCheckValidCell;
		InvalidateRect(NULL,FALSE);
		GetParent()->PostMessage(WM_COLORCHANGE,0,(LPARAM)CGlobalColorManager::StandardVGAColors[m_iSelectedCell]);
	}
	return;
}

BOOL CColorWidget::Create(CWnd *parent,RECT & rect,int ID)
{
	if (m_bWidgetRightBoxes)
	{
		rect.right=rect.left+WELL_SIZE*(m_iWidgetCols+3);
		rect.bottom=rect.top+WELL_SIZE*m_iWidgetRows;
	}
	else
	{
		rect.right=rect.left+WELL_SIZE*m_iWidgetCols;	//make sure it is the correct size
		rect.bottom=rect.top+WELL_SIZE*(m_iWidgetRows+2);
	}
	return CWnd::Create(NULL, _T(""),WS_VISIBLE|WS_CHILD|WS_TABSTOP,rect,parent,ID);
}



void DrawClientBox(CDC& dc,CRect& rect,BOOL bSunken)
{
	CPen DarkPen(PS_SOLID,1,GetSysColor(COLOR_BTNSHADOW));
	CPen LightPen(PS_SOLID,1,GetSysColor(COLOR_BTNHILIGHT));
	CPen *oldpen;
	//Bottom right border
	if (bSunken)
	{
		oldpen=(CPen*)dc.SelectObject(&LightPen);
	}
	else
	{
		oldpen=(CPen *)dc.SelectObject(&DarkPen);	
	}
	dc.MoveTo(rect.left,rect.bottom-1);
	dc.LineTo(rect.right-1,rect.bottom-1);
	dc.LineTo(rect.right-1,rect.top);		//Remember that LineTo does not include endpoint
	
	//top left border	
	if (!bSunken)
	{
		dc.SelectObject(&LightPen);
	}
	else
	{
		dc.SelectObject(&DarkPen);	
	}
	dc.MoveTo(rect.left,rect.bottom-2);
	dc.LineTo(rect.left,rect.top);
	dc.LineTo(rect.right,rect.top);	//fudge X
	
	dc.SelectObject(oldpen);
	DarkPen.DeleteObject();		//destroy 1
	LightPen.DeleteObject();	//destroy 2
}

void CColorWidget::EnableDisableOnly(BOOL bEnableDisableOnly)
{
	if (m_bEnableDisableOnly!=bEnableDisableOnly)
	{
		m_bEnableDisableOnly=bEnableDisableOnly;
		RECT rect;
		GetWindowRect(&rect);
		GetParent()->ScreenToClient(&rect);
		if (!m_bEnableDisableOnly)
		{
			rect.right=rect.left+WELL_SIZE*(m_iWidgetCols+3);
			rect.bottom=rect.top+WELL_SIZE*m_iWidgetRows;
		}
		else
		{
			rect.right=rect.left+WELL_SIZE*4;
			rect.bottom=rect.top+WELL_SIZE*m_iWidgetRows;
		}
		MoveWindow(rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,TRUE);
		InvalidateRect(NULL,TRUE);
	}
}
