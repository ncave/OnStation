// viewtab.cpp : implementation file
//

#include "stdafx.h"
#include "onstation.h"
#include "viewtab.h"
#include "afxpriv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewTab

CViewTab::CViewTab()
{
	m_iSelectedTab=0;
	m_CommandTarget=NULL;
}

CViewTab::~CViewTab()
{
}


BEGIN_MESSAGE_MAP(CViewTab, CWnd)
	//{{AFX_MSG_MAP(CViewTab)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
//	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXT,0,0xffff,OnToolTipText)
	ON_MESSAGE(WM_HELPHITTEST, OnHelpHitTest)
	ON_MESSAGE(WM_SIZEPARENT, OnSizeParent)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CViewTab message handlers

void CViewTab::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect r;
	GetClientRect(&r);
	FillRect(dc.m_hDC,r,(HBRUSH)GetStockObject(LTGRAY_BRUSH));
	if (m_dwWidths.GetSize()==0)
		return;	//don't paint anything if there is nothing to paint
	CFont * oldFont=dc.SelectObject(GetFont(FW_THIN));
	int iLeft=5;
	int iSelectedLeft=0;
	dc.SetBkColor(RGB(192,192,192));
	for (int i=0;i<m_szStrings.GetSize();i++)
	{
		if (i!=m_iSelectedTab)
		{
			DrawTab(dc,iLeft,(int)m_dwWidths.GetAt(i),r,(HBRUSH)GetStockObject(LTGRAY_BRUSH),m_szStrings.GetAt(i));
		}
		else
		{
			iSelectedLeft=iLeft;
		}
		iLeft=iLeft+(int)m_dwWidths.GetAt(i);
	}	
	delete dc.SelectObject(oldFont);
	oldFont=dc.SelectObject(GetFont(FW_BOLD));
	dc.SetBkColor(RGB(255,255,255));
	DrawTab(dc,iSelectedLeft,(int)m_dwWidths.GetAt(m_iSelectedTab),r,(HBRUSH)GetStockObject(WHITE_BRUSH),m_szStrings.GetAt(m_iSelectedTab));
	delete dc.SelectObject(oldFont);
}

BOOL CViewTab::Create(CWnd * wndMessages,CWnd* pParentWnd, DWORD dwStyle, UINT nID)
{
	//Warning: The tab has already been set before this thing
	//is created so be careful.
	ASSERT_VALID(pParentWnd);   // must have a parent
	m_CommandTarget=wndMessages;
	EnableToolTips(TRUE);

	// create the HWND
	CRect rect;
	rect.SetRectEmpty();
	return CWnd::Create(NULL, _T(""), dwStyle, rect, pParentWnd, nID);
}

void CViewTab::DrawTab(CDC& dc,int iLeft,int iWidth,CRect& r,HBRUSH hMiddle,LPCTSTR szText)
{
	POINT pts[4];
	pts[0].x=iLeft-5;
	pts[1].x=iLeft+3;
	pts[2].x=iLeft+iWidth-3;
	pts[3].x=iLeft+iWidth+5;
	pts[0].y=r.top;
	pts[1].y=r.bottom-1;
	pts[2].y=r.bottom-1;
	pts[3].y=r.top;
	CRgn rgn;
	rgn.CreatePolygonRgn(pts,4,ALTERNATE);
	FillRgn(dc.m_hDC,(HRGN)rgn.m_hObject,hMiddle);

	CRect rText;
	rText.top=r.top;
	rText.bottom=r.bottom;
	rText.left=iLeft;
	rText.right=iLeft+iWidth;
	dc.DrawText(szText,lstrlen(szText),rText,DT_CENTER);

	dc.SelectStockObject(BLACK_PEN);
	dc.Polyline(pts,4);	  
}


void CViewTab::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int iLeft=5;
	for (int i=0;i<m_szStrings.GetSize();i++)
	{
		if (point.x>=iLeft && point.x<(iLeft+(int)m_dwWidths.GetAt(i)))
		{
			m_iSelectedTab=i;
			InvalidateRect(NULL,FALSE);
			m_CommandTarget->PostMessage(WM_COMMAND,(WPARAM)m_iCommands.GetAt(i),0l);
			return;		
		}
		iLeft=iLeft+(int)m_dwWidths.GetAt(i);
	}
	CWnd::OnLButtonDown(nFlags, point);
}

int CViewTab::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void CViewTab::AddString(int iStringID,WPARAM iCommands,BOOL bActive)
{
	CString CStr;
	CStr.LoadString(iStringID);
	m_szStrings.Add(CStr);
	CDC dc;
	dc.CreateCompatibleDC(NULL);
	CFont * oldFont=dc.SelectObject(GetFont(FW_THIN));
	CSize CS=dc.GetTextExtent(CStr,lstrlen(CStr));
	delete dc.SelectObject(oldFont);	//get rid of font
	dc.DeleteDC();
	DWORD dw=CS.cx+30;	//30 pixel buffer
	m_dwWidths.Add(dw);
	m_iCommands.Add((DWORD)iCommands);
	if (bActive==TRUE)
	{
		m_iSelectedTab=m_dwWidths.GetSize()-1;
	}
}

CFont * CViewTab::GetFont(int iWeight)
{
	CFont *font=new CFont();
	font->CreateFont(14,0,0,0,iWeight,FALSE,FALSE,FALSE,ANSI_CHARSET,
						OUT_DEFAULT_PRECIS,CLIP_CHARACTER_PRECIS,
						DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,
						_T("Arial"));
	return font;
}

LONG_PTR CViewTab::OnHelpHitTest(UINT_PTR, LONG_PTR)
{
	return 0l;
}


LRESULT CViewTab::OnSizeParent(WPARAM wParam,LPARAM lParam)
{
	if (GetStyle()&WS_VISIBLE)
	{
		AFX_SIZEPARENTPARAMS * SizeParams=(AFX_SIZEPARENTPARAMS *)lParam;
		DeferWindowPos(SizeParams->hDWP,m_hWnd,NULL,SizeParams->rect.left,SizeParams->rect.bottom-VIEWTAB_HEIGHT,
							SizeParams->rect.right-SizeParams->rect.left,VIEWTAB_HEIGHT,SWP_NOZORDER);
		SizeParams->rect.bottom=SizeParams->rect.bottom-VIEWTAB_HEIGHT;
		SizeParams->sizeTotal.cy+=VIEWTAB_HEIGHT;
	}
	return 0l;
}


BOOL CViewTab::OnToolTipText(UINT uId,NMHDR * pNMHDR,LRESULT *pResult)
{
	TOOLTIPTEXT * Text=(TOOLTIPTEXT *)pNMHDR;
	lstrcpy(Text->szText, _T("Switches View Types"));
	*pResult=0;
	return TRUE;
}

void CViewTab::SetActiveTab(int iTab)
{
	if (m_iSelectedTab!=iTab)
	{
		m_iSelectedTab=iTab;	
		if (m_hWnd!=NULL)
		{
			InvalidateRect(NULL);
		}
	}
}

INT_PTR CViewTab::OnToolHitTest(CPoint point,TOOLINFO *pTI) const
{
	RECT r;
	GetClientRect(&r);
	pTI->lpszText=LPSTR_TEXTCALLBACK;
	pTI->hwnd=m_hWnd;
	pTI->uId=100;
	pTI->rect=r;
	return 1;
}
