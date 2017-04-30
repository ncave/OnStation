// OnStationView.cpp : implementation of the COnStationView class
//

#include "stdafx.h"
#include "OnStation.h"
#include "OnStationDoc.h"
#include "OnStationView.h"
#include "nodetree.h"
#include "mainfrm.h"
#include "surfacedata.h"
#include "colorsheet.h"
#include "passageoptions.h"
#include "orphanedshotdialog.h"
#include "infobyviewdialog.h"
#include "bookmark.h"
#include "bookmarkdialog.h"
#include "ruler.h"
#include "stdio.h"
#include "blunderdialog.h"
#include "pagelayoutpreferences.h"
#include "fixedpoint.h"
#include "searchcommentsdialog.h"
#include "query.h"
#include "realfolder.h"
#include "gworldopengl.h"
#include "overalldataview.h"
#include "setexactviewsheet.h"
#include "CenterStationNameDialog.h"
#include "searchresultdlg.h"
#include "distancebetweendialog.h"

#include <dwmapi.h>
typedef HRESULT (STDAPICALLTYPE *PFNDWMENABLECOMPOSITION)(UINT);

#define NUM_BANDS	10

COnStationView * pView_G=NULL;
extern CSettings * pSettings_G;
extern COnStationApp theApp;
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern COnStationDoc * pDocument_G;
/////////////////////////////////////////////////////////////////////////////
// COnStationView
IMPLEMENT_DYNCREATE(COnStationView, CView)


 

BEGIN_MESSAGE_MAP(COnStationView, CView)
	//{{AFX_MSG_MAP(COnStationView)
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI(ID_DRAWMODE_BLUNDERCOLOR, OnUpdateDrawmodeBlunderColoring)
	ON_COMMAND(ID_DRAWMODE_BLUNDERCOLOR, OnDrawmodeBlunderColoring)
	ON_UPDATE_COMMAND_UI(ID_DRAWMODE_AGECOLORING, OnUpdateDrawmodeAgecoloring)
	ON_COMMAND(ID_DRAWMODE_AGECOLORING, OnDrawmodeAgecoloring)
	ON_UPDATE_COMMAND_UI(ID_DRAWMODE_DEPTHCODING, OnUpdateDrawmodeDepthcoding)
	ON_COMMAND(ID_DRAWMODE_DEPTHCODING, OnDrawmodeDepthcoding)
	ON_UPDATE_COMMAND_UI(ID_DRAWMODE_PERSPECTIVE, OnUpdateDrawmodePerspective)
	ON_COMMAND(ID_DRAWMODE_PERSPECTIVE, OnDrawmodePerspective)
	ON_UPDATE_COMMAND_UI(ID_VIEW_NAMENONE, OnUpdateViewNamenone)
	ON_COMMAND(ID_VIEW_NAMENONE, OnViewNamenone)
	ON_UPDATE_COMMAND_UI(ID_VIEW_NAMEJUNCTIONS, OnUpdateViewNamejunctions)
	ON_COMMAND(ID_VIEW_NAMEJUNCTIONS, OnViewNamejunctions)
	ON_UPDATE_COMMAND_UI(ID_VIEW_NAMEALL, OnUpdateViewNameall)
	ON_COMMAND(ID_VIEW_NAMEALL, OnViewNameall)
	ON_COMMAND(ID_VIEW_SETEXACTVIEW, OnViewSetexactview)
	ON_COMMAND(ID_MOVEMENT_PROFILEVIEW, OnMovementProfileview)
	ON_UPDATE_COMMAND_UI(ID_MOVEMENT_PROFILEVIEW, OnUpdateMovementProfileview)
	ON_COMMAND(ID_MOVEMENT_PLANVIEW, OnMovementPlanview)
	ON_UPDATE_COMMAND_UI(ID_MOVEMENT_PLANVIEW, OnUpdateMovementPlanview)
	ON_COMMAND(ID_MOVEMENT_MIXEDVIEW, OnMovementMixedview)
	ON_UPDATE_COMMAND_UI(ID_MOVEMENT_MIXEDVIEW, OnUpdateMovementMixedview)
	ON_COMMAND(ID_MOVEMENT_FREEFORMVIEW, OnMovementFreeformview)
	ON_UPDATE_COMMAND_UI(ID_MOVEMENT_FREEFORMVIEW, OnUpdateMovementFreeformview)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SURFACEOVERLAY, OnUpdateViewSurfaceoverlay)
	ON_UPDATE_COMMAND_UI(ID_SHOWREFERENCE_GRID,OnUpdateShowReferenceGrid)
	ON_COMMAND(ID_SHOWREFERENCE_GRID,OnShowReferenceGrid)
	ON_COMMAND(ID_VIEW_SURFACEOVERLAY, OnViewSurfaceoverlay)
	ON_COMMAND(ID_VIEW_SURFACEANDLOCATIONSURVEYS, OnViewSurfaceandlocationsurveys)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SURFACEANDLOCATIONSURVEYS, OnUpdateViewSurfaceandlocationsurveys)
	ON_COMMAND(ID_VIEW_CLOSELOOPS, OnViewCloseloops)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CLOSELOOPS, OnUpdateViewCloseloops)
	ON_COMMAND(ID_EDIT_CONFIGURECOLORS, OnEditConfigurecolors)
	ON_COMMAND(ID_VIEW_PASSAGEOPTIONS, OnViewPassageoptions)
	ON_COMMAND(ID_DRAWMODE_COLORSCHEME1, OnDrawmodeColorscheme1)
	ON_UPDATE_COMMAND_UI(ID_DRAWMODE_COLORSCHEME1, OnUpdateDrawmodeColorscheme1)
	ON_COMMAND(ID_DRAWMODE_COLORSCHEME2, OnDrawmodeColorscheme2)
	ON_UPDATE_COMMAND_UI(ID_DRAWMODE_COLORSCHEME2, OnUpdateDrawmodeColorscheme2)
	ON_COMMAND(ID_DRAWMODE_COLORSCHEME3, OnDrawmodeColorscheme3)
	ON_UPDATE_COMMAND_UI(ID_DRAWMODE_COLORSCHEME3, OnUpdateDrawmodeColorscheme3)
	ON_COMMAND(ID_DRAWMODE_COLORSCHEME4, OnDrawmodeColorscheme4)
	ON_UPDATE_COMMAND_UI(ID_DRAWMODE_COLORSCHEME4, OnUpdateDrawmodeColorscheme4)
	ON_COMMAND(ID_DRAWMODE_COLORSCHEME5, OnDrawmodeColorscheme5)
	ON_UPDATE_COMMAND_UI(ID_DRAWMODE_COLORSCHEME5, OnUpdateDrawmodeColorscheme5)
	ON_COMMAND(ID_DRAWMODE_COLORSCHEME6, OnDrawmodeColorscheme6)
	ON_UPDATE_COMMAND_UI(ID_DRAWMODE_COLORSCHEME6, OnUpdateDrawmodeColorscheme6)
	ON_COMMAND(ID_DRAWMODE_COLORSCHEME7, OnDrawmodeColorscheme7)
	ON_UPDATE_COMMAND_UI(ID_DRAWMODE_COLORSCHEME7, OnUpdateDrawmodeColorscheme7)
	ON_COMMAND(ID_DRAWMODE_COLORSCHEME8, OnDrawmodeColorscheme8)
	ON_UPDATE_COMMAND_UI(ID_DRAWMODE_COLORSCHEME8, OnUpdateDrawmodeColorscheme8)
	ON_COMMAND(ID_EDIT_ORPHANEDSHOTS, OnEditOrphanedshots)
	ON_COMMAND(ID_QUERY_BLUNDERS,OnQueryBlunders)
	ON_UPDATE_COMMAND_UI(ID_EDIT_ORPHANEDSHOTS, OnUpdateEditOrphanedshots)
	ON_COMMAND(ID_VIEW_INFORMATION, OnViewInformation)
	ON_COMMAND(ID_VIEWPORT_EDITBOOKMARKS,OnEditBookmarks)
	ON_COMMAND(ID_VIEWPORT_ZOOMIN,OnZoomIn)
	ON_COMMAND(ID_VIEWPORT_ZOOMOUT,OnZoomOut)
	ON_COMMAND(ID_OPTIONS_MARKFIXEDPOINTS, OnOptionsMarkfixedpoints)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_MARKFIXEDPOINTS, OnUpdateOptionsMarkfixedpoints)
	ON_COMMAND(ID_OPTIONS_CLOSUREERRORS, OnOptionsClosureerrors)
	ON_COMMAND(ID_VIEW_ZOOMIN, OnZoomIn)
	ON_COMMAND(ID_VIEW_ZOOMOut, OnZoomOut)
	ON_COMMAND(ID_VIEWPORT_NORTH,OnMenuNorth)
	ON_COMMAND(ID_VIEWPORT_SOUTH,OnMenuSouth)
	ON_COMMAND(ID_VIEWPORT_EAST,OnMenuEast)
	ON_COMMAND(ID_VIEWPORT_WEST,OnMenuWest)
	ON_COMMAND(ID_VIEWPORT_ROTATELEFT,OnMenuRotateLeft)
	ON_COMMAND(ID_VIEWPORT_ROTATERIGHT,OnMenuRotateRight)
	ON_COMMAND(ID_VIEWPORT_TILTFORWARD,OnMenuTiltForward)
	ON_COMMAND(ID_VIEWPORT_TILTBACK,OnMenuTiltBack)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_CLOSUREERRORS, OnUpdateOptionsClosureerrors)
	ON_COMMAND(ID_FILE_PRINTINGPREFERENCES, OnFilePrintingpreferences)
	ON_COMMAND(ID_FILE_EXPORTVRMLFILE, OnFileExportvrmlfile)
	ON_COMMAND(ID_EDIT_SEARCHSTATIONCOMMENTS, OnEditSearchstationcomments)
	ON_COMMAND(ID_VIEW_FULLSCREEN, OnViewFullscreen)
	ON_COMMAND(ID_VIEW_FULLSCREEN_ESCKEY, OnViewFullscreenEsckey)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_VIEWPORT_STARTINGPOSITION, OnViewportStartingposition)
	ON_COMMAND(ID_SEARCH_SHOWSTATION, OnSearchShowstation)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_VIEW_ANIMATE, OnViewAnimate)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ANIMATE, OnUpdateViewAnimate)
	ON_WM_TIMER()
	ON_COMMAND(ID_VIEWPORT_SETEXACTVIEW,OnViewSetexactview)
	ON_COMMAND(ID_EDIT_BOOKMARKS, OnEditBookmarks)
	ON_WM_SIZE()
	ON_COMMAND(ID_VIEW_DISTANCEBETWEENSTATIONS, OnViewDistancebetweenstations)
    ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_COMMAND(ID_FILE_EXPORTDXF, OnExportDxf)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COnStationView construction/destruction

COnStationView::COnStationView()
{
	m_pType=NULL;
	m_pComment=NULL;
	m_bAnimate=FALSE;
	m_iAnimateDeltaX=0;
	m_iAnimateDeltaY=0;
	CGWorldOpenGL * pWorld=new CGWorldOpenGL();
	m_MyGWorld=(CGWorld *)pWorld;

	m_bRotatingSystem=FALSE;

	m_fRotateClockwise=0.0f;
	m_fRotateTilt=0.0f;

	m_fXPan=0.0f;
	m_fYPan=0.0f;
	m_fZPan=0.0f;
	//The document is NULL if we are doing a preview
	//for the surface or a survey
	m_fMetersPerCM=100.0f;
	pView_G=this;
}

COnStationView::~COnStationView()
{
	delete m_MyGWorld;
	m_MyGWorld=NULL;
	delete m_pType;
	m_pType=NULL;
	delete m_pComment;
	m_pComment=NULL;
	pView_G=NULL;
}

BOOL COnStationView::PreCreateWindow(CREATESTRUCT& cs)
{
	// Add Window styles required for OpenGL before window is created
	cs.style |= (WS_CLIPCHILDREN | WS_CLIPSIBLINGS | CS_OWNDC);

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// COnStationView drawing

void COnStationView::OnDraw(CDC* pDC)
{
	//OpenGL isn't particular about this, so we can't be either
	//otherwise text and so forth dissappear as soon as we repaint.
	//This doesn't work
//	InvalidateRect(NULL,FALSE);

	//	TRACE(_T("Entering OnDraw()\n"));
	SetScalingAndLighting(0);
	// Make the rendering context current
	wglMakeCurrent(m_hDC,m_hRC);

	// Call our external OpenGL code
	GLRenderScene(0);
	
	if (pSettings_G->m_ViewTypes[pSettings_G->m_iColorSchemeIndex]==VT_MIXED)
	{
		SetScalingAndLighting(1);
		// Call our external OpenGL code
		GLRenderScene(1);
	}
	SwapBuffers(m_hDC);
	wglMakeCurrent(m_hDC,NULL);

	//Attempt to fix redraw problem with text

	CDC * pDC2 = GetDC();
	DrawGDIStuff(pDC2);
	ReleaseDC(pDC2);
//	TRACE(_T("Leaving OnDraw()\n"));
}

void COnStationView::DrawGDIStuff(CDC* pDC2)
{
	DrawGDIOverlay(pDC2,0);
	
	if (pSettings_G->m_ViewTypes[pSettings_G->m_iColorSchemeIndex]==VT_MIXED)
	{
		DrawGDIOverlay(pDC2,1);
		CRect rect;
		rect=GetMyClientArea();
		CRect r2;
		r2=GetMyBottomClientArea();

		CPen *oldPen=(CPen *)pDC2->SelectStockObject(WHITE_PEN);
		pDC2->MoveTo(rect.left,rect.bottom);
		pDC2->LineTo(rect.right+1,rect.bottom);
		CPen penDK(PS_SOLID,1,RGB(128,128,128));
		pDC2->SelectObject(&penDK);
		pDC2->MoveTo(r2.left,r2.top-1);
		pDC2->LineTo(r2.right+1,r2.top-1);
		pDC2->SelectObject(oldPen);
		penDK.DeleteObject();

		CRect rFill(rect.left,rect.bottom+1,rect.right+1,r2.top-1);
		FillRect(pDC2->m_hDC,&rFill,(HBRUSH)GetStockObject(LTGRAY_BRUSH));
	}
}

/////////////////////////////////////////////////////////////////////////////
// COnStationView printing

BOOL COnStationView::OnPreparePrinting(CPrintInfo* pInfo)
{
	//Assuming the user does not change the current printer defaults
	//we can take a reasonable guess at the number of pages that are
	//going to be printed.
	CPrintDialog dlg(FALSE);
	if (dlg.GetDefaults())
	{
		CDC dc;
		dc.Attach(dlg.GetPrinterDC());

		GetPrinterInformation(&dc);
		pInfo->SetMaxPage(m_iHeightInPages*m_iWidthInPages);
	}
	return DoPreparePrinting(pInfo);
}

void COnStationView::GetPrinterInformation(CDC * pDC)
{
//	TRACE(_T("GetPrinterInformation\n"));
	CViewMatrix view;
	view.XPan(-m_fXPan);
	view.YPan(-m_fYPan);
	view.ZPan(-m_fZPan);
	view.RotateZ(-m_fRotateClockwise);
	view.RotateX(-m_fRotateTilt);
	m_PrintedAreaRectangle.left=10000;
	m_PrintedAreaRectangle.right=-10000;
	m_PrintedAreaRectangle.top=10000;
	m_PrintedAreaRectangle.bottom=-10000;
	int iColorScheme=pSettings_G->m_iColorSchemeIndex;
	m_MyGWorld->FindGeometryLimits(&view,&m_PrintedAreaRectangle,FALSE,
		pSettings_G->m_bBoxFixedPoints[iColorScheme],
		pSettings_G->m_bShowClosureErrors[iColorScheme]);
	if (m_PrintedAreaRectangle.left==10000 && m_PrintedAreaRectangle.right==-10000)
	{
//		TRACE(_T("No geometry to print!\n"));
		m_PrintedAreaRectangle.left=-5;
		m_PrintedAreaRectangle.right=5;
		m_PrintedAreaRectangle.top=-5;
		m_PrintedAreaRectangle.bottom=5;
	}
//	TRACE(_T("Geometry limits in meters are %i,%i %i,%i\n"),m_PrintedAreaRectangle.left,m_PrintedAreaRectangle.top,m_PrintedAreaRectangle.right,m_PrintedAreaRectangle.bottom);
	//Convert the rectangle to a width in LOMETRIC units
	m_PrintedAreaRectangle.left*=100;
	m_PrintedAreaRectangle.left=(int)(((float)m_PrintedAreaRectangle.left)/m_fMetersPerCM);
	m_PrintedAreaRectangle.right*=100;
	m_PrintedAreaRectangle.right=(int)(((float)m_PrintedAreaRectangle.right)/m_fMetersPerCM);
	m_PrintedAreaRectangle.top*=100;
	m_PrintedAreaRectangle.top=(int)(((float)m_PrintedAreaRectangle.top)/m_fMetersPerCM);
	m_PrintedAreaRectangle.bottom*=100;
	m_PrintedAreaRectangle.bottom=(int)(((float)m_PrintedAreaRectangle.bottom)/m_fMetersPerCM);
//	TRACE(_T("LOMETRIC limits without borders are %i,%i %i,%i\n"),m_PrintedAreaRectangle.left,m_PrintedAreaRectangle.top,m_PrintedAreaRectangle.right,m_PrintedAreaRectangle.bottom);

//Convert page width and height into LOMETRIC coordinates
	m_nPageHeight=pDC->GetDeviceCaps(VERTSIZE)*10;
	m_nPageWidth=pDC->GetDeviceCaps(HORZSIZE)*10;

	CAppSettings* Set=theApp.GetSettings();
	if (Set->m_bShowSideScale)
	{
		m_PrintedAreaRectangle.InflateRect(SIDE_RULER_BORDER,SIDE_RULER_BORDER);
	}
	if (Set->m_bPrintShowScale || Set->m_bPrintShowNorth)
	{
	//Because the bounding rect is in an upside down coordinate system
		m_PrintedAreaRectangle.top-=BOTTOM_WIDGETS_BORDER;
		//Make a reasonable width for the page so that we don't have
		//to try and cram the ruler or the compass into a narrow little
		//area.
		int iReasonableWidth=max(1500,m_nPageWidth-300);
		if (m_PrintedAreaRectangle.Width()<iReasonableWidth)
		{
			m_PrintedAreaRectangle.InflateRect((iReasonableWidth-m_PrintedAreaRectangle.Width())/2,0);
		}
	}

//	TRACE(_T("Final limits are %i,%i %i,%i\n"),m_PrintedAreaRectangle.left,m_PrintedAreaRectangle.top,m_PrintedAreaRectangle.right,m_PrintedAreaRectangle.bottom);
	//Widen if necessary so we can 
	long lTotalWidth=m_PrintedAreaRectangle.right-m_PrintedAreaRectangle.left;
	long lTotalHeight=m_PrintedAreaRectangle.bottom-m_PrintedAreaRectangle.top;
	m_iWidthInPages=1;
	if (lTotalWidth>m_nPageWidth)
	{
		lTotalWidth=lTotalWidth-m_nPageWidth;
		m_iWidthInPages=m_iWidthInPages+(lTotalWidth/(m_nPageWidth+theApp.GetSettings()->m_iPrintOverlap*10))+1;
	}
	m_iHeightInPages=1;
	if (lTotalHeight>m_nPageHeight)
	{
		lTotalHeight=lTotalHeight-m_nPageHeight;
		m_iHeightInPages=m_iHeightInPages+(lTotalHeight/(m_nPageHeight+theApp.GetSettings()->m_iPrintOverlap*10))+1;
	}

#ifdef _DEBUG
//	CRect r;
//	float fTop,fLeft,fBottom,fRight;
//	GetPrintedPageBoundaries(&r,0,&fLeft,&fTop,&fRight,&fBottom);
//	TRACE(_T("Page 1 is %i,%i %i,%i\n"),r.left,r.top,r.right,r.bottom);
#endif
}

void COnStationView::OnPrint(CDC *pDC,CPrintInfo *pInfo)
{
	TRACE(_T("entering OnPrint()\n"));
	CAppSettings *Set=theApp.GetSettings();
	BOOL bHasTrimmings=(Set->m_bPrintShowScale || Set->m_bPrintShowNorth);
	
	//Calculate the coordinates of this page in the current
	//in terms of our internal coordinate system.
	CRect rPrintedPage;
	float fLeft,fRight,fTop,fBottom;
	GetPrintedPageBoundaries(&rPrintedPage,pInfo->m_nCurPage-1,&fLeft,&fTop,&fRight,&fBottom);
/*
	for (int i=0;i<NUM_BANDS;i++)
	{
		float fStripSize=(fBottom-fTop)/NUM_BANDS;
		CRect rPrintedPageTemp=rPrintedPage;
		int iHeight=rPrintedPageTemp.Height();
		rPrintedPageTemp.top=rPrintedPage.top+(iHeight*i)/NUM_BANDS;
		rPrintedPageTemp.bottom=rPrintedPage.top+(iHeight*(i+1))/NUM_BANDS;
		HandlePrintOfView(pDC,rPrintedPageTemp,fLeft,fTop+fStripSize*i,fRight,fTop+fStripSize*(i+1),iHeight*i/NUM_BANDS);
	}
	*/
	HandlePrintOfView(pDC,rPrintedPage,fLeft,fTop,fRight,fBottom);
	pDC->SetViewportOrg(0,0);
	UINT mm=pDC->SetMapMode(MM_LOMETRIC);

	//Print the scale and so forth along the bottom
	//Calculate rectangle which really represents the screen
	CRect rTemp=m_PrintedAreaRectangle;
	rTemp.OffsetRect(-rPrintedPage.left,-rPrintedPage.top);
	float fIncrement;
	CRuler::GetReasonableIncrements(m_fMetersPerCM,&fIncrement);
	float fSpacing=fIncrement/m_fMetersPerCM*100.0f;
	
	if (Set->m_bShowSideScale)
	{
		pDC->SelectStockObject(BLACK_PEN);
		pDC->SelectStockObject(NULL_BRUSH);
		pDC->Rectangle(rTemp);
		int iOffset=1;
		int iLineX=(int)fSpacing;	//small errors accumulate, so keep ruler as a float
		while (rTemp.left+iLineX<rTemp.right)
		{
			int iSize=SIDE_RULER_SMALLTICK;
			if (iOffset%5==0)
				iSize=SIDE_RULER_MEDIUMTICK;
			if (iOffset%10==0)
				iSize=SIDE_RULER_LARGESTTICK;
			pDC->MoveTo(rTemp.left+iLineX,rTemp.top+1);
			pDC->LineTo(rTemp.left+iLineX,rTemp.top+1+iSize);
			pDC->MoveTo(rTemp.left+iLineX,rTemp.bottom);
			pDC->LineTo(rTemp.left+iLineX,rTemp.bottom-iSize);
			iOffset++;
			iLineX=(int)(fSpacing*(float)iOffset);
		}
		iOffset=1;
		int iLineY=(int)fSpacing;	//small errors accumulate so keep ruler as a float
		while (rTemp.bottom-iLineY>rTemp.top)
		{
			int iSize=SIDE_RULER_SMALLTICK;
			if (iOffset%5==0)
				iSize=SIDE_RULER_MEDIUMTICK;
			if (iOffset%10==0)
				iSize=SIDE_RULER_LARGESTTICK;
			pDC->MoveTo(rTemp.left,rTemp.bottom-iLineY);
			pDC->LineTo(rTemp.left+iSize,rTemp.bottom-iLineY);
			//-1 on right due to inside drawing of rectangles
			pDC->MoveTo(rTemp.right-1,rTemp.bottom-iLineY);
			pDC->LineTo(rTemp.right-iSize-1,rTemp.bottom-iLineY);
			iOffset++;
			iLineY=(int)(fSpacing*(float)iOffset);
		}
		//Shrink rectangle at bottom so that the ruler
		//and compass show up at the right spot instead of in
		//the middle of our border
		rTemp.top+=SIDE_RULER_LARGESTTICK;
	}

	int iOrnamentsWidth=0;
	if (Set->m_bPrintShowScale)
		iOrnamentsWidth+=(int)(fSpacing*12.0f);
	if (Set->m_bPrintShowNorth)
		iOrnamentsWidth+=COMPASS_WIDTH;
	if (Set->m_bPrintShowNorth && Set->m_bPrintShowScale)
	{
		iOrnamentsWidth+=COMPASS_RULER_SPACING;
	}
	int iLeftOffset=(rTemp.Width()-iOrnamentsWidth)/2;
	BOOL bShort=FALSE;
	if (iLeftOffset<0)	//if it doesn't all fit, shorten the ruler by 50%
	{
		bShort=TRUE;
		iLeftOffset=iLeftOffset+(int)(fSpacing*2.5f);
	}
	pDC->SetTextColor(RGB(0,0,0));
	if (Set->m_bPrintShowScale)
	{
		CPen *oldPen;
		CPen NewPen(PS_SOLID,Set->m_bPrintThickLines?3:1,RGB(0,0,0));
		oldPen=pDC->SelectObject(&NewPen);

		TCHAR szBuffer[128];

	//Draw the scale bar
		pDC->MoveTo(rTemp.left+iLeftOffset,rTemp.top+SCALEBAR_YSTART+SIDE_RULER_LARGESTTICK);
		pDC->LineTo(rTemp.left+iLeftOffset,rTemp.top+SCALEBAR_YSTART);
		pDC->LineTo(rTemp.left+iLeftOffset+(int)(fSpacing*(bShort?5.0f:10.0f)),rTemp.top+SCALEBAR_YSTART);
		pDC->LineTo(rTemp.left+iLeftOffset+(int)(fSpacing*(bShort?5.0f:10.0f)),rTemp.top+SCALEBAR_YSTART+SIDE_RULER_LARGESTTICK+1);	//1 extra since ends are not printed
		if (!bShort)
		{
			pDC->MoveTo(rTemp.left+iLeftOffset+(int)(fSpacing*5.0f),rTemp.top+SCALEBAR_YSTART+SIDE_RULER_LARGESTTICK);
			pDC->LineTo(rTemp.left+iLeftOffset+(int)(fSpacing*5.0f),rTemp.top+SCALEBAR_YSTART);
		}

		RECT rText;
		rText.left=rTemp.left+iLeftOffset-(int)fSpacing;
		rText.right=rTemp.left+iLeftOffset+(int)(fSpacing*2.0f);
		rText.top=rTemp.top+SCALEBARLRTEXT_YSTART+50;
		rText.bottom=rTemp.top+SCALEBARLRTEXT_YSTART;
		_stprintf_s(szBuffer, _T("%s"),GetPreferedUnitString(0.0f));
		pDC->DrawText(szBuffer,-1,&rText,DT_CENTER|DT_NOCLIP);
		rText.left=rTemp.left+iLeftOffset+(int)(fSpacing*(bShort?3.0f:8.0f));
		rText.right=rTemp.left+iLeftOffset+(int)(fSpacing*(bShort?6.0f:11.0f));
		_stprintf_s(szBuffer, _T("%s"),GetPreferedUnitString(fIncrement*(bShort?5.0f:10.0f)));
		pDC->DrawText(szBuffer,-1,&rText,DT_CENTER|DT_NOCLIP);

		rText.left=rTemp.left+iLeftOffset;
		rText.right=rTemp.left+iLeftOffset+(int)(fSpacing*(bShort?5.0f:10.0f));
		rText.top=rTemp.top+SCALEBARTITLE_YSTART+50;
		rText.bottom=rTemp.top+SCALEBARTITLE_YSTART;
		_stprintf_s(szBuffer, _T("%s"),GetPreferedScaleString(m_fMetersPerCM));
		pDC->DrawText(szBuffer,-1,&rText,DT_CENTER|DT_NOCLIP);

		pDC->SelectObject(oldPen);
		iLeftOffset+=COMPASS_RULER_SPACING;
		iLeftOffset+=(int)(bShort?fSpacing*7.0f:fSpacing*12.0f);
	}
	if (Set->m_bPrintShowNorth)
	{
		CPen NewPen(PS_SOLID,Set->m_bPrintThickLines?3:1,RGB(0,0,0));
		CPen * oldPen=pDC->SelectObject(&NewPen);

		pDC->Ellipse(rTemp.left+iLeftOffset,rTemp.top+COMPASS_YSTART+COMPASS_WIDTH,rTemp.left+iLeftOffset+COMPASS_WIDTH,rTemp.top+COMPASS_YSTART);
		RECT rText;
		rText.left=rTemp.left+iLeftOffset;
		rText.right=rTemp.left+iLeftOffset+COMPASS_WIDTH;
		rText.top=rTemp.top+SCALEBARTITLE_YSTART+50;
		rText.bottom=rTemp.top+SCALEBARTITLE_YSTART;
		CString CS;
		CS.LoadString(IDS_NORTH);
		pDC->DrawText(CS,&rText,DT_CENTER|DT_NOCLIP);

		double fAngle=m_fRotateClockwise*3.14159/180.0;
		double sine=sin(fAngle);
		double cosine=cos(fAngle);
		double dx=COMPASS_WIDTH*9.0/20.0*sine;
		double dy=COMPASS_WIDTH*9.0/20.0*cosine;
		CPoint pt1(rTemp.left+iLeftOffset+COMPASS_WIDTH/2,rTemp.top+COMPASS_YSTART+COMPASS_WIDTH/2);
		CPoint pt2(pt1.x+(int)dx,pt1.y+(int)dy);
		//Sides of the arrow
		//80% along
		CPoint pt3(pt1.x+(int)(dx*0.8),pt1.y+(int)(dy*0.8) );
		CPoint pt4(pt3.x+(int)(dy*.2),pt3.y-(int)(dx*.2));
		CPoint pt5(pt3.x-(int)(dy*.2),pt3.y+(int)(dx*.2));
		pDC->MoveTo(pt1);
		pDC->LineTo(pt2);
		pDC->LineTo(pt4);
		pDC->MoveTo(pt2);
		pDC->LineTo(pt5);
		pDC->SelectObject(oldPen);
	}	
	pDC->SetMapMode(mm);
	TRACE(_T("leaving OnPrint()\n"));
}

void COnStationView::HandlePrintOfView(CDC * pDCOriginal,CRect rPrintedPage,float fLeft,float fTop,float fRight,float fBottom/*,int iYposition*/)
{
/*	//Create a memory device context
	HDC hDCSample=::GetDC(NULL);
	HDC hDCMemory=::CreateCompatibleDC(hDCSample);
	::ReleaseDC(NULL,hDCSample);

	BITMAPINFO bmInfo;
	bmInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	bmInfo.bmiHeader.biWidth=rPrintedPage.Width();
	//Coordinates are upside down for height because they
	//are in lometric
	bmInfo.bmiHeader.biHeight=-rPrintedPage.Height();
	bmInfo.bmiHeader.biPlanes=1;
	bmInfo.bmiHeader.biBitCount=8;
	bmInfo.bmiHeader.biCompression=BI_RGB;
	bmInfo.bmiHeader.biSizeImage=0;
	bmInfo.bmiHeader.biXPelsPerMeter=0;
	bmInfo.bmiHeader.biYPelsPerMeter=0;
	bmInfo.bmiHeader.biClrUsed=0;
	bmInfo.bmiHeader.biClrImportant=0;

	LPVOID pbmDataPointer;
	// Selects the palette into the current device context
	HPALETTE hPalOld=SelectPalette(hDCMemory, (HPALETTE)m_GLPalette, FALSE);
	RealizePalette(hDCMemory);
	HBITMAP hBitmapDIB=CreateDIBSection(hDCMemory,&bmInfo,DIB_RGB_COLORS,&pbmDataPointer,NULL,NULL);
	HBITMAP hBitmapOld=(HBITMAP)::SelectObject(hDCMemory,hBitmapDIB);
	SetColorTable(hDCMemory);
	
	HPALETTE hPalOldOriginal=SelectPalette(pDCOriginal->m_hDC,(HPALETTE)m_GLPalette,FALSE);
	pDCOriginal->RealizePalette();

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),	// Size of this structure
		1,								// Version of this structure	
		PFD_DRAW_TO_BITMAP |			
		PFD_SUPPORT_OPENGL |			// Support OpenGL calls in window
		PFD_TYPE_RGBA,					// RGBA Color mode
		8,								// Want 24bit color 
		0,0,0,0,0,0,					// Not used to select mode
		0,0,							// Not used to select mode
		0,0,0,0,0,						// Not used to select mode
		32,								// Size of depth buffer
		0,								// Not used to select mode
		0,								// Not used to select mode
		PFD_MAIN_PLANE,					// Draw in main plane
		0,								// Not used to select mode
		0,0,0 };						// Not used to select mode

	// Choose a pixel format that best matches that described in pfd
	int nPixelFormat = ChoosePixelFormat(hDCMemory, &pfd);
	// Set the pixel format for the device context
	VERIFY(SetPixelFormat(hDCMemory, nPixelFormat, &pfd));
	int iError=GetLastError();	

	HGLRC hrc=wglCreateContext(hDCMemory);
	ASSERT(hrc!=NULL);
	VERIFY(wglMakeCurrent(hDCMemory,hrc));
	m_MyGWorld->CompileLists();
	iError=GetLastError();

	// Set Viewport to window dimensions
	TRACE(_T("glViewPortPrint %i,%i %i,%i\n"),0,0,rPrintedPage.Width(), -rPrintedPage.Height());
    glViewport(0,0, rPrintedPage.Width(), -rPrintedPage.Height());
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (pSettings_G->m_bPerspective)
	{
		//This does not work.  I don't have any idea why
		//		gluPerspective(180.0f, fSizeX/fSizeY, 10.0f,1000.0f);
		glFrustum(-50.0f,50.0f,-50.0f,50.0f,-10.0f,-1000.0f);
	}
	else
	{
		TRACE(_T("glOrthoPrint %f %f %f %f\n"),fLeft,fRight,fBottom,fTop),
		glOrtho(fLeft,fRight,fBottom,fTop,-1000.0f,2000.0f);
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CW);
	if (pSettings_G->m_bMultipleLights)
	{
		// Light values and coordinates
		GLfloat  whiteLight[] = {0.7f, 0.7f, 0.7f, 1.0f };
		GLfloat  specular[] = { 0.7f, 0.7f, 0.7f, 1.0f};
		GLfloat  shine[] = { 100.0f };


		// Enable lighting
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		// Enable color tracking
		glEnable(GL_COLOR_MATERIAL);
		
		// Set Material properties to follow glColor values
		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, shine);
	}
	else
	{
		// Enable lighting
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
	}

	// Draw in background in black
//	glRGB(0,0,220);

	// Clear the window with white since we are printing.
	glRGB(0,255,255);
	glClearColor(1.0f,1.0f,1.0f,0.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//desperation
//	PatBlt(hDCMemory,0,0,rPrintedPage.Width(),rPrintedPage.Height(),WHITENESS);

	// Save the modelview matrix stack
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glRotated(-m_fRotateTilt,1.0,0.0,0.0);
	glRotated(-m_fRotateClockwise,0.0,0.0,1.0);
	TRACE(_T("OpenGL Pan *10=%i %i %i\n"),(int)(-m_fXPan*10.0f),(int)(-m_fYPan*10.0f),(int)(-m_fZPan*10.0f));
	glTranslated(-m_fXPan,-m_fYPan,-m_fZPan);

//	SetupTestGeometry();
 	m_MyGWorld->GenerateGeometry(FALSE,
		pSettings_G->m_bSurfaceOverlay,
		pSettings_G->m_bBoxFixedPoints,
		pSettings_G->m_bShowClosureErrors);
	glPopMatrix();
	glFlush();	

	wglMakeCurrent(hDCMemory,NULL);
	wglDeleteContext(hrc);

	//Start debug code
//	HDC dcScreen=::GetDC(NULL);
//	SetMapMode(dcScreen,MM_LOMETRIC);
//	::StretchBlt(dcScreen,200,200,200,200,hDCMemory,0,0,rPrintedPage.Width(),-rPrintedPage.Height(),SRCCOPY);
//	MessageBeep(0);
//	Sleep(2000);
//	::ReleaseDC(NULL,dcScreen);
	//end debug code
	
	UINT mm=pDCOriginal->SetMapMode(MM_LOMETRIC);
//	::BitBlt(pDCOriginal->m_hDC,0,rPrintedPage.Height(),rPrintedPage.Width(),-rPrintedPage.Height(),hDCMemory,0,0,SRCCOPY);
	::StretchBlt(pDCOriginal->m_hDC,0,iYposition,rPrintedPage.Width(),rPrintedPage.Height(),hDCMemory,0,0,rPrintedPage.Width(),-rPrintedPage.Height(),SRCCOPY);

	CViewMatrix view;

	//if center is at X,Y,Z we must pan to -X,-Y,-Z
	view.XPan(-m_fXPan);
	view.YPan(-m_fYPan);
	view.ZPan(-m_fZPan);
	view.RotateZ(-m_fRotateClockwise);
	view.RotateX(-m_fRotateTilt);
	//Start the rectangle at 0,0 because that is the viewport origin
	rPrintedPage.OffsetRect(-rPrintedPage.left,-rPrintedPage.top+iYposition);
	int iTemp=rPrintedPage.bottom;
	rPrintedPage.bottom=rPrintedPage.top;
	rPrintedPage.top=iTemp;
	m_MyGWorld->DrawText(pDCOriginal,&view,pSettings_G->m_ShowStations,rPrintedPage,fLeft,fRight,fTop,fBottom);
	m_MyGWorld->DrawTaggedPoints(pDCOriginal,&view,rPrintedPage,fLeft,fRight,fTop,fBottom,!pSettings_G->m_bBoxFixedPoints);
	pDCOriginal->SetMapMode(mm);

//	pDCOriginal->FillSolidRect(0,rPrintedPage.Height(),rPrintedPage.Width(),-rPrintedPage.Height(),RGB(255,0,0));
//	pDCOriginal->FillSolidRect(10,rPrintedPage.Height()+10,rPrintedPage.Width()-20,-rPrintedPage.Height()-20,RGB(0,255,0));

	::SelectObject(hDCMemory,hBitmapOld);
	SelectPalette(hDCMemory, hPalOld, FALSE);
	SelectPalette(pDCOriginal->m_hDC,hPalOldOriginal,FALSE);
	DeleteObject(hBitmapDIB);
	DeleteDC(hDCMemory);
	*/

	UINT mm=pDCOriginal->SetMapMode(MM_LOMETRIC);

	CViewMatrix view;

	//if center is at X,Y,Z we must pan to -X,-Y,-Z
	view.XPan(-m_fXPan);
	view.YPan(-m_fYPan);
	view.ZPan(-m_fZPan);
	view.RotateZ(-m_fRotateClockwise);
	view.RotateX(-m_fRotateTilt);
	//Start the rectangle at 0,0 because that is the viewport origin
	rPrintedPage.OffsetRect(-rPrintedPage.left,-rPrintedPage.top/*+iYposition*/);
	int iTemp=rPrintedPage.bottom;
	rPrintedPage.bottom=rPrintedPage.top;
	rPrintedPage.top=iTemp;
	int iColorScheme=pSettings_G->m_iColorSchemeIndex;
	COLORREF crSurface=pSettings_G->m_cSurfaceColors[iColorScheme];
	m_MyGWorld->DrawGDILineSegments(pDCOriginal,&view,rPrintedPage,fLeft,fRight,fTop,fBottom,theApp.GetSettings()->m_bPrintThickLines?THICK_PRINTER_LINES:1,pSettings_G->m_bShowClosureErrors[iColorScheme]);
	CFont * pFont=theApp.GetSettings()->GetPrinterFont();
	m_MyGWorld->DrawText(pDCOriginal,&view,pFont,pSettings_G->m_ShowStationNames[iColorScheme],rPrintedPage,fLeft,fRight,fTop,fBottom);
	delete pFont;
	m_MyGWorld->DrawTaggedPoints(pDCOriginal,&view,rPrintedPage,fLeft,fRight,fTop,fBottom,!pSettings_G->m_bBoxFixedPoints[iColorScheme]);
	pDCOriginal->SetMapMode(mm);
}

void COnStationView::GetPrintedPageBoundaries(CRect * pPage,int iPage,float *fLeft,float *fTop,float *fRight,float *fBottom)
{
	CAppSettings *Set=theApp.GetSettings();
	int iOverlap=Set->m_iPrintOverlap;
	int iXPage=(iPage%m_iWidthInPages);
	int iYPage=(iPage/m_iWidthInPages);
	pPage->left=m_PrintedAreaRectangle.left+(m_nPageWidth-iOverlap)*iXPage;
	pPage->top=m_PrintedAreaRectangle.bottom-(m_nPageHeight-iOverlap)*iYPage;
	pPage->right=pPage->left+m_nPageWidth;
	pPage->bottom=pPage->top-m_nPageHeight;
	
	if (m_iWidthInPages==1)	//if only one page, then center it.
	{
		//Note, since the rectangle is upside down due to the map mode, we must
		//reverse the Y offset.
		pPage->OffsetRect(-(pPage->Width()-m_PrintedAreaRectangle.Width())/2,0);
	}
	if (m_iHeightInPages==1)
	{
		pPage->OffsetRect(0,(-pPage->Height()-m_PrintedAreaRectangle.Height())/2);
	}

	//Convert the LOMETRIC coordinates back to meters
	//so that we can use the appropriate section of the
	//GWORLD to print onto the canvas.
	*fLeft=(float)pPage->left;
	*fLeft=*fLeft/100.0f*m_fMetersPerCM;
	*fRight=(float)pPage->right;
	*fRight=*fRight/100.0f*m_fMetersPerCM;
	*fTop=(float)pPage->top;
	*fTop=*fTop/100.0f*m_fMetersPerCM;
	*fBottom=(float)pPage->bottom;
	*fBottom=*fBottom/100.0f*m_fMetersPerCM;

	TRACE(_T("GetPrintedPageBoundaries(): Printed page area is %i,%i to %i,%i\n"),pPage->left,pPage->top,pPage->right,pPage->bottom);
	TRACE(_T("GetPrintedPageBoundaries(): GL World Area is %i,%i to %i,%i\n"),(int)*fLeft,(int)*fTop,(int)*fRight,(int)*fBottom);
}

void COnStationView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	GetPrinterInformation(pDC);
}

void COnStationView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

/////////////////////////////////////////////////////////////////////////////
// COnStationView diagnostics

#ifdef _DEBUG
void COnStationView::AssertValid() const
{
	CView::AssertValid();
}

void COnStationView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COnStationDoc* COnStationView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COnStationDoc)));
	return (COnStationDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COnStationView message handlers

void COnStationView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	SetScalingAndLighting(0);
}

int COnStationView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// disable DWM Composition in Vista/Windows7, so we can draw OpenGL + GDI (3D + 2D)
	// (an alternative can be to draw everything with OpenGL, or draw OpenGL to FBO)
	DisableAero();

	if (!m_Compass.Create(this,106))
	{
		TRACE(_T("Failed to create compass\n"));
	}

	int nPixelFormat;					// Pixel format index
	m_hDC = ::GetDC(m_hWnd);			// Get the Device context

	//Convert mouse move in pixels to size in meters
	m_fMouseConvertX=((float)GetDeviceCaps(m_hDC,HORZSIZE)/10.0f)/(float)GetDeviceCaps(m_hDC,HORZRES);
	m_fMouseConvertY=((float)GetDeviceCaps(m_hDC,VERTSIZE)/10.0f)/(float)GetDeviceCaps(m_hDC,VERTRES);

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),	// Size of this structure
		1,								// Version of this structure	
		PFD_DRAW_TO_WINDOW |			// Draw to Window (not to bitmap)
		PFD_SUPPORT_OPENGL |			// Support OpenGL calls in window
		PFD_DOUBLEBUFFER,				// Double buffered mode
		PFD_TYPE_RGBA,					// RGBA Color mode
		8,								// Want 24bit color 
		0,0,0,0,0,0,					// Not used to select mode
		0,0,							// Not used to select mode
		0,0,0,0,0,						// Not used to select mode
		32,								// Size of depth buffer
		0,								// Not used to select mode
		0,								// Not used to select mode
		PFD_MAIN_PLANE,					// Draw in main plane
		0,								// Not used to select mode
		0,0,0 };						// Not used to select mode

	// Choose a pixel format that best matches that described in pfd
	nPixelFormat = ChoosePixelFormat(m_hDC, &pfd);

	// Set the pixel format for the device context
	VERIFY(SetPixelFormat(m_hDC, nPixelFormat, &pfd));

	// Create the rendering context
	m_hRC = wglCreateContext(m_hDC);

	// Make the rendering context current, perform initialization, then
	// deselect it
	//Commented out by taco
//	VERIFY(wglMakeCurrent(m_hDC,m_hRC));
//	GLSetupRC(m_hDC);
//	wglMakeCurrent(NULL,NULL);

	// Create the palette if needed
	InitializePalette();

	return 0;
}

void COnStationView::DisableAero()
{
	HMODULE hDWMAPI = ::GetModuleHandle(_T("DWMAPI"));
	if (hDWMAPI != NULL)
	{
		PFNDWMENABLECOMPOSITION pfnDwmEnableComposition = (PFNDWMENABLECOMPOSITION)::GetProcAddress(hDWMAPI, "DwmEnableComposition");
		if (pfnDwmEnableComposition)
		{
			pfnDwmEnableComposition(DWM_EC_DISABLECOMPOSITION);
		}
	}
}

void COnStationView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
//	RecalculateGeometryIfNeeded(FALSE);
}

void COnStationView::RecalculateGeometry(BOOL bRegenerateGeometry)
{
	BOOL bDataChanged=FALSE;
	if (bRegenerateGeometry)
	{
//		TRACE(_T("Recalculating Dirty data\n"));
		bDataChanged=TRUE;
		try
		{
			GetDocument()->GetNodeTree()->CalculateRawPositions(GetDocument()->GetSurface(),pSettings_G->m_iColorSchemeIndex);
		}
		catch(CException*)
		{
			AfxMessageBox(IDS_CALCULATESYSTEMINTERNALERROR);
			return;
		}
	}
	int iColorScheme=pSettings_G->m_iColorSchemeIndex;
	if (pSettings_G->m_bShowClosedLoops[iColorScheme])
	{
		if (!GetDocument()->GetNodeTree()->CloseLoops(iColorScheme))
		{
			pSettings_G->m_bShowClosedLoops[iColorScheme]=FALSE;
		}
	}

	ASSERT(AfxCheckMemory());
//	TRACE(_T("Updating Geometry\n"));
	m_MyGWorld->Empty();	//Important safety hint.
	GetDocument()->GetNodeTree()->FillGWorld(m_MyGWorld);
	GetDocument()->GetSurface()->CalculateGWorld(m_MyGWorld);

	if (pSettings_G->m_bShowReferenceGrid[iColorScheme])
	{
		CNodeTreeSummary nodeTreeSummary;
		GetDocument()->GetNodeTree()->GetViewInfo(&nodeTreeSummary);
		float fLeft=(float)((((int)nodeTreeSummary.m_fLeft)/100-1)*100);
		float fRight=(float)((((int)nodeTreeSummary.m_fRight)/100+1)*100);
		float fTop=(float)((((int)nodeTreeSummary.m_fTop)/100-1)*100);
		float fBottom=(float)((((int)nodeTreeSummary.m_fBottom)/100+1)*100);
		float fZMin=(float)((((int)nodeTreeSummary.m_fDeepest)/100-1)*100);
		float fZMax=(float)((((int)nodeTreeSummary.m_fHighest)/100+1)*100);
		if (fLeft<fRight && fTop<fBottom)
		{
			float fRightTemp=fRight;
			while (fRightTemp>=fLeft)
			{
				CPosMatrix From;
				CPosMatrix To;
				From.Set(fRightTemp,fBottom,fZMin);
				To.Set(fRightTemp,fTop,fZMin);
				m_MyGWorld->AddLineSegment(From,To,RGB(128,128,128),LT_SURFACE);
				m_MyGWorld->AddLineSegment(From,To,RGB(128,128,128),LT_WIREFRAME);
				To.Set(fRightTemp,fBottom,fZMax);
				m_MyGWorld->AddLineSegment(From,To,RGB(128,128,128),LT_SURFACE);
				m_MyGWorld->AddLineSegment(From,To,RGB(128,128,128),LT_WIREFRAME);
				fRightTemp-=100.0f;
			}
			float fTopTemp=fTop;
			while (fTopTemp<=fBottom)
			{
				CPosMatrix From;
				CPosMatrix To;
				From.Set(fLeft,fTopTemp,fZMin);
				To.Set(fRight,fTopTemp,fZMin);
				m_MyGWorld->AddLineSegment(From,To,RGB(128,128,128),LT_WIREFRAME);
				m_MyGWorld->AddLineSegment(From,To,RGB(128,128,128),LT_SURFACE);
				To.Set(fLeft,fTopTemp,fZMax);
				m_MyGWorld->AddLineSegment(From,To,RGB(128,128,128),LT_SURFACE);
				m_MyGWorld->AddLineSegment(From,To,RGB(128,128,128),LT_WIREFRAME);
				fTopTemp+=100.0f;
			}
			float fZTemp=fZMin;
			while (fZTemp<=fZMax)
			{
				CPosMatrix From;
				CPosMatrix To;
				From.Set(fLeft,fBottom,fZTemp);
				To.Set(fRight,fBottom,fZTemp);
				m_MyGWorld->AddLineSegment(From,To,RGB(128,128,128),LT_WIREFRAME);
				m_MyGWorld->AddLineSegment(From,To,RGB(128,128,128),LT_SURFACE);
				To.Set(fLeft,fTop,fZTemp);
				m_MyGWorld->AddLineSegment(From,To,RGB(128,128,128),LT_WIREFRAME);
				m_MyGWorld->AddLineSegment(From,To,RGB(128,128,128),LT_SURFACE);
				fZTemp+=100.0f;
			}
		}
	}
	// Make the rendering context current
	wglMakeCurrent(m_hDC,m_hRC);
	m_MyGWorld->CompileLists();
	wglMakeCurrent(m_hDC,NULL);

	InvalidateRect(NULL,FALSE);
}


void COnStationView::OnUpdateDrawmodeBlunderColoring(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(pSettings_G->m_iColorSchemeIndex==COLORSCHEME_BLUNDER);
}

void COnStationView::OnDrawmodeBlunderColoring() 
{
	CMainFrame * pMainFrame=dynamic_cast<CMainFrame *>(AfxGetMainWnd());
	pMainFrame->m_dataView.SetColorSchemeIndex(COLORSCHEME_BLUNDER);
	PickColorScheme(COLORSCHEME_BLUNDER);
}

void COnStationView::OnUpdateDrawmodeAgecoloring(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(pSettings_G->m_iColorSchemeIndex==COLORSCHEME_AGE);
}

void COnStationView::OnDrawmodeAgecoloring() 
{
	CMainFrame * pMainFrame=dynamic_cast<CMainFrame *>(AfxGetMainWnd());
	pMainFrame->m_dataView.SetColorSchemeIndex(COLORSCHEME_AGE);
	PickColorScheme(COLORSCHEME_AGE);
}

void COnStationView::OnUpdateDrawmodeDepthcoding(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(pSettings_G->m_iColorSchemeIndex==COLORSCHEME_DEPTH);
}

void COnStationView::OnDrawmodeDepthcoding() 
{
	CMainFrame * pMainFrame=dynamic_cast<CMainFrame *>(AfxGetMainWnd());
	pMainFrame->m_dataView.SetColorSchemeIndex(COLORSCHEME_DEPTH);
	PickColorScheme(COLORSCHEME_DEPTH);
}

void COnStationView::OnUpdateDrawmodePerspective(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(pSettings_G->m_bPerspective[pSettings_G->m_iColorSchemeIndex]);
}

void COnStationView::OnDrawmodePerspective() 
{
	AfxMessageBox(_T("Warning: Perspective is not working in this version.  Please do not report this as a bug"));
	pSettings_G->m_bPerspective[pSettings_G->m_iColorSchemeIndex]=!pSettings_G->m_bPerspective[pSettings_G->m_iColorSchemeIndex];;
	SetScalingAndLighting(0);
	RedrawView();
}

void COnStationView::OnUpdateViewNamenone(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(pSettings_G->m_ShowStationNames[pSettings_G->m_iColorSchemeIndex]==SHOW_NONE);
}

void COnStationView::OnViewNamenone() 
{
	pSettings_G->m_ShowStationNames[pSettings_G->m_iColorSchemeIndex]=SHOW_NONE;
	RedrawView();
}

void COnStationView::OnUpdateViewNamejunctions(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(pSettings_G->m_ShowStationNames[pSettings_G->m_iColorSchemeIndex]==SHOW_INTERSECT);
}

void COnStationView::OnViewNamejunctions() 
{
	pSettings_G->m_ShowStationNames[pSettings_G->m_iColorSchemeIndex]=SHOW_INTERSECT;
	RedrawView();
}

void COnStationView::OnUpdateViewNameall(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(pSettings_G->m_ShowStationNames[pSettings_G->m_iColorSchemeIndex]==SHOW_ALL);
}

void COnStationView::OnViewNameall() 
{
	pSettings_G->m_ShowStationNames[pSettings_G->m_iColorSchemeIndex]=SHOW_ALL;
	RedrawView();
}


void COnStationView::OnViewSetexactview() 
{
	CSurfaceData * Surf=pDocument_G->GetSurface();
	CSetExactViewSheet SEVS(this, _T("Set View Point"),m_fRotateClockwise,m_fRotateTilt,
			m_fMetersPerCM,Surf->ConvertXToUTM(m_fXPan),Surf->ConvertYToUTM(m_fYPan),m_fZPan);
	if (SEVS.DoModal()==IDOK)
	{
		SetExactView(SEVS.m_ViewPage.m_fRotate,SEVS.m_ViewPage.m_fElevate,SEVS.m_ViewScale.m_fMetricScale,Surf->ConvertXToLocal(SEVS.m_ViewCenter.m_fMetricX),Surf->ConvertYToLocal(SEVS.m_ViewCenter.m_fMetricY),SEVS.m_ViewCenter.m_fMetricZ);
	}
}

void COnStationView::OnMovementProfileview() 
{
	pSettings_G->m_ViewTypes[pSettings_G->m_iColorSchemeIndex]=VT_PROFILE;
	CMainFrame * pMainFrame=dynamic_cast<CMainFrame *>(AfxGetMainWnd());
	pMainFrame->SetActiveViewTab(VT_PROFILE);
	SetExactView(NAN,90.0f,NAN,NAN,NAN,NAN);
	SetScalingAndLighting(0);
	RedrawView();
}

void COnStationView::OnUpdateMovementProfileview(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(pSettings_G->m_ViewTypes[pSettings_G->m_iColorSchemeIndex]==VT_PROFILE);
}

void COnStationView::OnMovementPlanview() 
{
	pSettings_G->m_ViewTypes[pSettings_G->m_iColorSchemeIndex]=VT_TOP;
	CMainFrame * pMainFrame=dynamic_cast<CMainFrame *>(AfxGetMainWnd());
	pMainFrame->SetActiveViewTab(VT_TOP);
	SetExactView(NAN,0.0f,NAN,NAN,NAN,NAN);
	SetScalingAndLighting(0);
	RedrawView();
}

void COnStationView::OnUpdateMovementPlanview(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(pSettings_G->m_ViewTypes[pSettings_G->m_iColorSchemeIndex]==VT_TOP);
}

void COnStationView::OnMovementMixedview() 
{
	pSettings_G->m_ViewTypes[pSettings_G->m_iColorSchemeIndex]=VT_MIXED;
	CMainFrame * pMainFrame=dynamic_cast<CMainFrame *>(AfxGetMainWnd());
	pMainFrame->SetActiveViewTab(VT_MIXED);
	SetExactView(NAN,0.0f,NAN,NAN,NAN,NAN);
	SetScalingAndLighting(0);
	RedrawView();
}

void COnStationView::OnUpdateMovementMixedview(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(pSettings_G->m_ViewTypes[pSettings_G->m_iColorSchemeIndex]==VT_MIXED);
}

void COnStationView::OnMovementFreeformview() 
{
	pSettings_G->m_ViewTypes[pSettings_G->m_iColorSchemeIndex]=VT_FREEFORM;
	CMainFrame * pMainFrame=dynamic_cast<CMainFrame *>(AfxGetMainWnd());
	pMainFrame->SetActiveViewTab(VT_FREEFORM);
	SetScalingAndLighting(0);
	RedrawView();
}

void COnStationView::OnUpdateMovementFreeformview(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(pSettings_G->m_ViewTypes[pSettings_G->m_iColorSchemeIndex]==VT_FREEFORM);
}

void COnStationView::OnShowReferenceGrid()
{
	int iColorScheme=pSettings_G->m_iColorSchemeIndex;
	pSettings_G->m_bShowReferenceGrid[iColorScheme]=!pSettings_G->m_bShowReferenceGrid[iColorScheme];
	RecalculateGeometry(FALSE);
}

void COnStationView::OnUpdateShowReferenceGrid(CCmdUI * pCmdUI)
{
	int iColorScheme=pSettings_G->m_iColorSchemeIndex;
	pCmdUI->SetCheck(pSettings_G->m_bShowReferenceGrid[iColorScheme]);
}

void COnStationView::OnUpdateViewSurfaceoverlay(CCmdUI* pCmdUI) 
{
	int iColorScheme=pSettings_G->m_iColorSchemeIndex;
	COLORREF crSurface=pSettings_G->m_cSurfaceColors[iColorScheme];
	BOOL bShowSurfaceTopography=crSurface!=COLOR_INACTIVE && crSurface!=COLOR_INVISIBLE;
	pCmdUI->SetCheck(bShowSurfaceTopography);
}

void COnStationView::OnViewSurfaceoverlay() 
{
	int iColorScheme=pSettings_G->m_iColorSchemeIndex;
	COLORREF crSurface=pSettings_G->m_cSurfaceColors[iColorScheme];
	BOOL bShowSurfaceTopography=crSurface!=COLOR_INACTIVE && crSurface!=COLOR_INVISIBLE;
	pSettings_G->m_cSurfaceColors[iColorScheme]=bShowSurfaceTopography?COLOR_INVISIBLE:RGB(128,128,128);
	RecalculateGeometry(FALSE);
}

void COnStationView::OnViewSurfaceandlocationsurveys() 
{
	pSettings_G->m_bSurfaceSurveys[pSettings_G->m_iColorSchemeIndex]=!pSettings_G->m_bSurfaceSurveys[pSettings_G->m_iColorSchemeIndex];
//	InvalidateRect(NULL,FALSE);
	RecalculateGeometry(FALSE);
}

void COnStationView::OnUpdateViewSurfaceandlocationsurveys(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(pSettings_G->m_bSurfaceSurveys[pSettings_G->m_iColorSchemeIndex]);
}

void COnStationView::OnViewCloseloops() 
{
	if (pSettings_G->m_iColorSchemeIndex==COLORSCHEME_BLUNDER)
	{
		AfxMessageBox(_T("You must keep loop closure turned on in blunder color scheme since the loop closure code calculates the accuracy of shots."));
	}
	else
	{
		pSettings_G->m_bShowClosedLoops[pSettings_G->m_iColorSchemeIndex]=!pSettings_G->m_bShowClosedLoops[pSettings_G->m_iColorSchemeIndex];
		RecalculateGeometry(FALSE);
	}
}

void COnStationView::OnUpdateViewCloseloops(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(pSettings_G->m_bShowClosedLoops[pSettings_G->m_iColorSchemeIndex]);
}

void COnStationView::OnEditConfigurecolors() 
{
	CColorSheet CS(IDS_CONFIGURECOLORS,pSettings_G->m_iColorSchemeIndex==COLORSCHEME_DEPTH?1:0,this);
	if (CS.DoModal()==IDOK)
	{
		if (CS.IsDirty())
		{
			pDocument_G->SetModifiedFlag();
		}
		if (pSettings_G->m_iColorSchemeIndex==CS.m_iRecommendedColorSchemeIndex)
		{
			if (CS.IsDirty())
			{
				RecalculateGeometry(FALSE);
			}
		}
		else
		{
			CMainFrame * pMainFrame=dynamic_cast<CMainFrame *>(AfxGetMainWnd());
			pMainFrame->m_dataView.SetColorSchemeIndex(CS.m_iRecommendedColorSchemeIndex);
			PickColorScheme(CS.m_iRecommendedColorSchemeIndex);
		}
	}
}


void COnStationView::OnViewPassageoptions() 
{
	CPassageOptions Options;
	int iScheme=pSettings_G->m_iColorSchemeIndex;

	Options.m_bMultipleLights=pSettings_G->m_bMultipleLights[iScheme];
	Options.m_PassageType=pSettings_G->m_PassageType[iScheme];
	Options.m_bDisableLightRotate=pSettings_G->m_bRotateNoLighting[iScheme];
	Options.m_bWireFrameRotate=pSettings_G->m_bRotateNoSolids[iScheme];

	if (Options.DoModal()==IDOK)
	{
		pSettings_G->m_bMultipleLights[iScheme]=Options.m_bMultipleLights;
		pSettings_G->m_PassageType[iScheme]=Options.m_PassageType;
		pSettings_G->m_bRotateNoLighting[iScheme]=Options.m_bDisableLightRotate;
		pSettings_G->m_bRotateNoSolids[iScheme]=Options.m_bWireFrameRotate;
		// Make the rendering context current
		RecalculateGeometry(FALSE);
		SetScalingAndLighting(0);
		RedrawView();
	}
}

void COnStationView::OnDrawmodeColorscheme1() 
{
	CMainFrame * pMainFrame=dynamic_cast<CMainFrame *>(AfxGetMainWnd());
	pMainFrame->m_dataView.SetColorSchemeIndex(0);
	PickColorScheme(0);
}

void COnStationView::OnUpdateDrawmodeColorscheme1(CCmdUI* pCmdUI) 
{
	UpdateColorScheme(pCmdUI,0);
}

void COnStationView::UpdateColorScheme(CCmdUI *pCmdUI,int iIndex)
{
	pCmdUI->SetRadio(pSettings_G->m_iColorSchemeIndex==iIndex);
	COnStationDoc *document=GetDocument();
	if (document!=NULL)
	{
		CString cs;
		cs.Format(IDS_COLORSCHEMEHOTKEY,pSettings_G->m_szColorSchemeNames[iIndex],iIndex+1);
		pCmdUI->SetText(cs);
	}
}

void COnStationView::OnDrawmodeColorscheme2() 
{
	CMainFrame * pMainFrame=dynamic_cast<CMainFrame *>(AfxGetMainWnd());
	pMainFrame->m_dataView.SetColorSchemeIndex(1);
	PickColorScheme(1);
}

void COnStationView::OnUpdateDrawmodeColorscheme2(CCmdUI* pCmdUI) 
{
	UpdateColorScheme(pCmdUI,1);
}

void COnStationView::OnDrawmodeColorscheme3() 
{
	CMainFrame * pMainFrame=dynamic_cast<CMainFrame *>(AfxGetMainWnd());
	pMainFrame->m_dataView.SetColorSchemeIndex(2);
	PickColorScheme(2);
}

void COnStationView::OnUpdateDrawmodeColorscheme3(CCmdUI* pCmdUI) 
{
	UpdateColorScheme(pCmdUI,2);
}

void COnStationView::OnDrawmodeColorscheme4() 
{
	CMainFrame * pMainFrame=dynamic_cast<CMainFrame *>(AfxGetMainWnd());
	pMainFrame->m_dataView.SetColorSchemeIndex(3);
	PickColorScheme(3);
}

void COnStationView::OnUpdateDrawmodeColorscheme4(CCmdUI* pCmdUI) 
{
	UpdateColorScheme(pCmdUI,3);
}

void COnStationView::OnDrawmodeColorscheme5() 
{
	CMainFrame * pMainFrame=dynamic_cast<CMainFrame *>(AfxGetMainWnd());
	pMainFrame->m_dataView.SetColorSchemeIndex(4);
	PickColorScheme(4);
}

void COnStationView::OnUpdateDrawmodeColorscheme5(CCmdUI* pCmdUI) 
{
	UpdateColorScheme(pCmdUI,4);
}

void COnStationView::OnDrawmodeColorscheme6() 
{
	CMainFrame * pMainFrame=dynamic_cast<CMainFrame *>(AfxGetMainWnd());
	pMainFrame->m_dataView.SetColorSchemeIndex(5);
	PickColorScheme(5);
}

void COnStationView::OnUpdateDrawmodeColorscheme6(CCmdUI* pCmdUI) 
{
	UpdateColorScheme(pCmdUI,5);
}

void COnStationView::OnDrawmodeColorscheme7() 
{
	CMainFrame * pMainFrame=dynamic_cast<CMainFrame *>(AfxGetMainWnd());
	pMainFrame->m_dataView.SetColorSchemeIndex(6);
	PickColorScheme(6);
}

void COnStationView::OnUpdateDrawmodeColorscheme7(CCmdUI* pCmdUI) 
{
	UpdateColorScheme(pCmdUI,6);
}

void COnStationView::OnDrawmodeColorscheme8() 
{
	CMainFrame * pMainFrame=dynamic_cast<CMainFrame *>(AfxGetMainWnd());
	pMainFrame->m_dataView.SetColorSchemeIndex(7);
	PickColorScheme(7);
}

void COnStationView::PickColorScheme(int iIndex)
{
	BOOL bNeedNewPositions=!(GetDocument()->GetSurveyFolder()->IsColorSchemeDataSimilar(iIndex,pSettings_G->m_iColorSchemeIndex));
	pSettings_G->m_iColorSchemeIndex=iIndex;
	if (bNeedNewPositions)
	{
		RecalculateGeometry(TRUE);
	}
	else
	{
		RecalculateGeometry(FALSE);
	}
	CMainFrame * pMainFrame=dynamic_cast<CMainFrame *>(AfxGetMainWnd());
	pMainFrame->SetActiveViewTab(pSettings_G->m_ViewTypes[iIndex]);
	RedrawView();
}

void COnStationView::OnUpdateDrawmodeColorscheme8(CCmdUI* pCmdUI) 
{
	UpdateColorScheme(pCmdUI,7);
}

void COnStationView::OnEditOrphanedshots() 
{
	if (GetDocument()->GetSurveyFolder()->HasDisabledLegs(pSettings_G->m_iColorSchemeIndex))
	{
		if (AfxMessageBox(IDS_DISABLEDLEGSCREWTHISUP,MB_OKCANCEL)==IDCANCEL)
		{
			return;
		}
	}
	
	COrphanedShotDialog OSD(GetDocument(),pSettings_G->m_iColorSchemeIndex,this);
	OSD.DoModal();
	if (OSD.m_bDirty)
	{
		//Update the fixed point list
		CMainFrame * pMainFrame=dynamic_cast<CMainFrame *>(AfxGetMainWnd());
		pMainFrame->m_dataView.OnChangedDocumentData();
		//and the cave itself
		RecalculateGeometry(TRUE);
	}
}

void COnStationView::OnUpdateEditOrphanedshots(CCmdUI* pCmdUI) 
{
	CNodeTree *NT=GetDocument()->GetNodeTree();
	pCmdUI->Enable(NT->GetNumberOfMissingSections()!=0);
}


void COnStationView::OnViewInformation() 
{
	CNodeTreeSummary NTS;

	CNodeTree *NT=GetDocument()->GetNodeTree();
	NT->GetViewInfo(&NTS);
//These values will be used to set the back and front clipping planes
	CInfoByViewDialog IBVD(&NTS);
	IBVD.DoModal();
}

void COnStationView::OnEditBookmarks() 
{
//	PerformanceMetrics();
	CSurfaceData * pSurface=GetDocument()->GetSurface();
	CBookmarkDialog BMD(pSurface->ConvertXToUTM(m_fXPan),pSurface->ConvertYToUTM(m_fYPan),m_fZPan,m_fRotateClockwise,m_fRotateTilt,m_fMetersPerCM,pSettings_G->m_iColorSchemeIndex);
	if (BMD.DoModal()==IDOK)
	{
		CMainFrame * pMainFrame=dynamic_cast<CMainFrame *>(AfxGetMainWnd());
		pMainFrame->m_dataView.SetColorSchemeIndex(BMD.m_GotoMe->m_iColorSchemeIndex);
		PickColorScheme(BMD.m_GotoMe->m_iColorSchemeIndex);
		SetExactView(BMD.m_GotoMe->m_fRot,BMD.m_GotoMe->m_fTilt,BMD.m_GotoMe->m_fZoom,pSurface->ConvertXToLocal(BMD.m_GotoMe->m_fPanX),pSurface->ConvertYToLocal(BMD.m_GotoMe->m_fPanY),BMD.m_GotoMe->m_fPanZ);
	}
	if (BMD.m_bChanged)
	{
		GetDocument()->SetModifiedFlag(TRUE);
	}
}

void COnStationView::OnOptionsMarkfixedpoints() 
{
	pSettings_G->m_bBoxFixedPoints[pSettings_G->m_iColorSchemeIndex]=!pSettings_G->m_bBoxFixedPoints[pSettings_G->m_iColorSchemeIndex];
	RedrawView();
}

void COnStationView::OnUpdateOptionsMarkfixedpoints(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(pSettings_G->m_bBoxFixedPoints[pSettings_G->m_iColorSchemeIndex]);
}
#ifdef _DEBUG
void COnStationView::PerformanceMetrics()
{
	TCHAR szResult[256];
	CDC dc;
	dc.CreateCompatibleDC(NULL);
	CBitmap bm;
	BITMAPINFO bmh;
	bmh.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	bmh.bmiHeader.biWidth=1024;
	bmh.bmiHeader.biHeight=768;
	bmh.bmiHeader.biPlanes=1;
	bmh.bmiHeader.biBitCount=8;
	bmh.bmiHeader.biCompression=BI_RGB;
	bmh.bmiHeader.biSizeImage=0;
	bmh.bmiHeader.biXPelsPerMeter=0;
	bmh.bmiHeader.biYPelsPerMeter=0;
	bmh.bmiHeader.biClrUsed=0;
	bmh.bmiHeader.biClrImportant=0;
	LPVOID TempPtr;
	HBITMAP hbm=CreateDIBSection(dc.m_hDC,&bmh,DIB_RGB_COLORS,&TempPtr,NULL,0);
//	bm.Attach(hbm)
	bm.CreateCompatibleBitmap(&dc,1024,768);
	CBitmap *oldBitmap=dc.SelectObject(&bm);
	CPen pen(PS_SOLID,1,RGB(255,255,255));
	CPen *oldPen=dc.SelectObject(&pen);

	//MOVETO/LINETO LOOP
	long lTick=GetTickCount();
	for (int i=0;i<10000;i++)
	{
		dc.MoveTo(17,23);
		dc.LineTo(400,700);
//		dc.Rectangle(32,75,187,333);
		RECT r;
		r.left=32;
		r.right=150;
		r.top=32;
		r.bottom=17;
		dc.DrawText(_T("Hello"),-1,&r,DT_LEFT);	//internationalization OK
	}
	lTick=GetTickCount()-lTick;
	_stprintf_s(szResult, _T("In Bounds 1 pen 10000 reps %i"),lTick);	//internationalization OK
	CPen pen2(PS_SOLID,1,RGB(0,0,0));
	MessageBox(szResult, _T("Time"),MB_OK);	//internationalization OK

	//SELECTING 2 PENS 10000 TIMES
	lTick=GetTickCount();
	for (int i=0;i<10000;i++)
	{
		if (i%2)
			dc.SelectObject(&pen);
		else
			dc.SelectObject(&pen2);
	}
	lTick=GetTickCount()-lTick;
	_stprintf_s(szResult, _T("Selecting 2 pens 10000 reps %i"),lTick);	//internationaliztion OK
	MessageBox(szResult, _T("Time"),MB_OK);	//internationalization OK

	//MULTIPLY MATRICES TEST
	CViewMatrix VM;
	CPosMatrix PM;
	PM.Set(1.0f,2.0f,3.0f);
	lTick=GetTickCount();
	for (int i=0;i<200000;i++)
	{
		PM.Multiply(&PM,VM);
		PM.Normalize();
	}
	lTick=GetTickCount()-lTick;
	_stprintf_s(szResult, _T("multiply test 200000 reps %i"),lTick);	//internationalization OK
	MessageBox(szResult, _T("Time"),MB_OK);	//internationalization OK


	dc.SelectObject(oldPen);
	dc.SelectObject(oldBitmap);
	dc.DeleteDC();
}
#endif


void COnStationView::OnOptionsClosureerrors() 
{
	pSettings_G->m_bShowClosureErrors[pSettings_G->m_iColorSchemeIndex]=!pSettings_G->m_bShowClosureErrors[pSettings_G->m_iColorSchemeIndex];
	RedrawView();
}

void COnStationView::OnUpdateOptionsClosureerrors(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(pSettings_G->m_bShowClosureErrors[pSettings_G->m_iColorSchemeIndex]);
	pCmdUI->Enable(!pSettings_G->m_bShowClosedLoops[pSettings_G->m_iColorSchemeIndex]);
}

void COnStationView::OnFilePrintingpreferences() 
{
	CPageLayoutPreferences PLP(this);
	CAppSettings *Set=theApp.GetSettings();

	PLP.m_bThickLines=Set->m_bPrintThickLines;
	PLP.m_bIncludeArrow=Set->m_bPrintShowNorth;
	PLP.m_bIncludeScale=Set->m_bPrintShowScale;
	PLP.m_iPageOverlap=Set->m_iPrintOverlap;
	PLP.m_bIncludeSideScale=Set->m_bShowSideScale;
	PLP.m_bShowTitleBox = Set->m_bPrintTitlePage;
	PLP.m_bNameByYear = Set->m_bPrintPeopleByYear;
	if (PLP.DoModal()==IDOK)
	{
		Set->m_bPrintThickLines=PLP.m_bThickLines;
		Set->m_bPrintShowNorth=PLP.m_bIncludeArrow;
		Set->m_bPrintShowScale=PLP.m_bIncludeScale;
		Set->m_iPrintOverlap=PLP.m_iPageOverlap;
		Set->m_bShowSideScale=PLP.m_bIncludeSideScale;
		Set->m_bPrintTitlePage=PLP.m_bShowTitleBox;
		Set->m_bPrintPeopleByYear=PLP.m_bNameByYear ;
	}
}



void COnStationView::OnInitialUpdate() 
{
	CMainFrame * pMainFrame=dynamic_cast<CMainFrame *>(AfxGetMainWnd());
	if (pMainFrame!=NULL)
	{
		pMainFrame->m_dataView.InitialUpdate();
	}

	CView::OnInitialUpdate();
	RecalculateGeometry(TRUE);
	SetExactView(0.0f,0.0f,100.0f,0.0f,0.0f,0.0f);

	//Center on the first constrained point where appropriate
	//This stops us from being way the hell over in some unknown
	//location if there is a surface in play or something.
	CSurfaceData * surf=GetDocument()->GetSurface();
	if (surf!=NULL)
	{
		CFixedPoint *pt=surf->GetFixedPoint(0);
		if (pt!=NULL)
		{
			SetExactView(NAN,NAN,NAN,surf->ConvertXToLocal(pt->GetX()),surf->ConvertYToLocal(pt->GetY()),pt->GetZ());
		}
	}
	CMainFrame * pFrame=dynamic_cast<CMainFrame *>(AfxGetMainWnd());
	if (pFrame==NULL)
		return;
	pFrame->m_wndRuler.SetView(this);
	m_Compass.SetView(this);
}

void COnStationView::OnFileExportvrmlfile() 
{
//	try
//	{
		TCHAR szReturn[512];
		szReturn[0]=_T('\0');

		OPENFILENAME of;
		of.lStructSize=sizeof(OPENFILENAME);
		of.hwndOwner=m_hWnd;
		of.hInstance=AfxGetInstanceHandle();
		of.lpstrFilter=_T("(VRML Files)\0*.wrl\0*.wrld\0\0");
		of.lpstrCustomFilter=NULL;
		of.nFilterIndex=0;
		of.lpstrFile=szReturn;
		of.nMaxFile=_countof(szReturn);
		of.lpstrFileTitle=NULL;
		of.lpstrInitialDir=NULL;
		of.lpstrTitle=_T("Export to VRML File");
		of.Flags=OFN_HIDEREADONLY;
		of.lpstrDefExt= _T("wrl");

		if (GetOpenFileName(&of)==TRUE)
		{
			CFile File(szReturn,CFile::modeCreate|CFile::modeWrite);
			CArchive Ar(&File,CArchive::store);
			int iColorScheme=pSettings_G->m_iColorSchemeIndex;
			m_MyGWorld->WriteGWorldToVRML(Ar,pSettings_G->m_ShowStationNames[iColorScheme]);
		}	
//	}
//	catch
//	{
//		AfxMessageBox(_T("Error during export.  Unable to complete operation."));
//	}
}

void COnStationView::OnEditSearchstationcomments() 
{
	CSearchCommentsDialog SCD(this,GetDocument()->GetSurveyFolder());
	if (SCD.DoModal()==IDOK)
	{
		CQuery * pTypeQuery=new CQuery();
		CQuery * pBodyQuery;
		if (!pTypeQuery->SetQueryText(SCD.m_szTypeString))
		{
			delete pTypeQuery;
			return;
		}
		pBodyQuery=new CQuery();
		if (!pBodyQuery->SetQueryText(SCD.m_szCommentText))
		{
			delete pBodyQuery;
			delete pTypeQuery;
			return;
		}
		SetCommentQuery(pTypeQuery,pBodyQuery);
	}
	else
	{
 		SetCommentQuery(NULL,NULL);
	}
}



void COnStationView::OnViewFullscreen() 
{
	CMainFrame * pMainFrame=dynamic_cast<CMainFrame *>(AfxGetMainWnd());
	if (pMainFrame!=NULL)
	{
		pMainFrame->ViewFullScreen(this);
	}
}




void COnStationView::OnViewFullscreenEsckey() 
{
	//Only use the ESC key to go OUT of full screen mode.
	CMainFrame * pMainFrame=dynamic_cast<CMainFrame *>(AfxGetMainWnd());
	if (pMainFrame!=NULL && pMainFrame->IsFullScreen())
	{
		pMainFrame->ViewFullScreen(this);
	}
	if (pMainFrame!=NULL)
	{
		pMainFrame->m_dataView.SetQuery(NULL);
	}
	SetCommentQuery(NULL,NULL);
}

void COnStationView::OnDestroy() 
{
	CView::OnDestroy();

	// Clean up rendering context stuff
	wglDeleteContext(m_hRC);
	::ReleaseDC(m_hWnd,m_hDC);
}

BOOL COnStationView::OnEraseBkgnd(CDC* pDC) 
{
	return FALSE;
}

BOOL COnStationView::OnQueryNewPalette() 
{
	// If the palette was created.
	if((HPALETTE)m_GLPalette)
	{
		int nRet;

		// Selects the palette into the current device context
		SelectPalette(m_hDC, (HPALETTE)m_GLPalette, FALSE);

		// Map entries from the currently selected palette to
		// the system palette.  The return value is the number 
		// of palette entries modified.
		nRet = RealizePalette(m_hDC);

		// Repaint, forces remap of palette in current window
		InvalidateRect(NULL,FALSE);

		return nRet;
		}

	return CView::OnQueryNewPalette();
}

void COnStationView::OnPaletteChanged(CWnd* pFocusWnd) 
{
	if(((HPALETTE)m_GLPalette != NULL) && (pFocusWnd != this))
	{
		// Select the palette into the device context
		SelectPalette(m_hDC,(HPALETTE)m_GLPalette,FALSE);

		// Map entries to system palette
		RealizePalette(m_hDC);
				
		// Remap the current colors to the newly realized palette
		UpdateColors(m_hDC);
		return;
	}

	CView::OnPaletteChanged(pFocusWnd);
}



// Initializes the CPalette object
void COnStationView::InitializePalette(void)
{
	PIXELFORMATDESCRIPTOR pfd;	// Pixel Format Descriptor
	LOGPALETTE *pPal;			// Pointer to memory for logical palette
	int nPixelFormat;			// Pixel format index
	int nColors;				// Number of entries in palette
	int i;						// Counting variable
	BYTE RedRange,GreenRange,BlueRange;
								// Range for each color entry (7,7,and 3)

	// Get the pixel format index and retrieve the pixel format description
	nPixelFormat = GetPixelFormat(m_hDC);
	DescribePixelFormat(m_hDC, nPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	// Does this pixel format require a palette?  If not, do not create a
	// palette and just return NULL
	if(!(pfd.dwFlags & PFD_NEED_PALETTE))
		return;

	// Number of entries in palette.  8 bits yeilds 256 entries
	nColors = 1 << pfd.cColorBits;	

	// Allocate space for a logical palette structure plus all the palette entries
	pPal = (LOGPALETTE*)malloc(sizeof(LOGPALETTE) +nColors*sizeof(PALETTEENTRY));

	// Fill in palette header 
	pPal->palVersion = 0x300;		// Windows 3.0
	pPal->palNumEntries = nColors; // table size

	// Build mask of all 1's.  This creates a number represented by having
	// the low order x bits set, where x = pfd.cRedBits, pfd.cGreenBits, and
	// pfd.cBlueBits.  
	RedRange = (1 << pfd.cRedBits) -1;
	GreenRange = (1 << pfd.cGreenBits) - 1;
	BlueRange = (1 << pfd.cBlueBits) -1;

	// Loop through all the palette entries
	for(i = 0; i < nColors; i++)
		{
		// Fill in the 8-bit equivalents for each component
		pPal->palPalEntry[i].peRed = (i >> pfd.cRedShift) & RedRange;
		pPal->palPalEntry[i].peRed = (unsigned char)(
			(double) pPal->palPalEntry[i].peRed * 255.0 / RedRange);

		pPal->palPalEntry[i].peGreen = (i >> pfd.cGreenShift) & GreenRange;
		pPal->palPalEntry[i].peGreen = (unsigned char)(
			(double)pPal->palPalEntry[i].peGreen * 255.0 / GreenRange);

		pPal->palPalEntry[i].peBlue = (i >> pfd.cBlueShift) & BlueRange;
		pPal->palPalEntry[i].peBlue = (unsigned char)(
			(double)pPal->palPalEntry[i].peBlue * 255.0 / BlueRange);

		pPal->palPalEntry[i].peFlags = (unsigned char) NULL;
		}
		
	// Create the palette
	m_GLPalette.CreatePalette(pPal);

	// Go ahead and select and realize the palette for this device context
	SelectPalette(m_hDC,(HPALETTE)m_GLPalette,FALSE);
	RealizePalette(m_hDC);

	// Free the memory used for the logical palette structure
	free(pPal);
}



void COnStationView::RefreshStatusIndicators(CMainFrame *frm)
{
	if (pDocument_G==NULL)
		return;
	CString szBuffer;
	CSurfaceData *surf=GetDocument()->GetSurface();
	szBuffer.Format(IDS_X,GetPreferedUnitString(surf->ConvertXToUTM(m_fXPan)));
	frm->SetPaneText(ID_INDICATOR_XPAN,szBuffer);
	szBuffer.Format(IDS_Y,GetPreferedUnitString(surf->ConvertYToUTM(m_fYPan)));
	frm->SetPaneText(ID_INDICATOR_YPAN,szBuffer);
	szBuffer.Format(IDS_Z,GetPreferedUnitString(m_fZPan));
	frm->SetPaneText(ID_INDICATOR_ZPAN,szBuffer);
	szBuffer.Format(IDS_TILT,m_fRotateTilt);
	frm->SetPaneText(ID_INDICATOR_TILT,szBuffer);
	szBuffer.Format(IDS_SPIN,m_fRotateClockwise);
	frm->SetPaneText(ID_INDICATOR_CLOCKWISE,szBuffer);
	szBuffer.Format(_T("%s"),GetPreferedScaleString(m_fMetersPerCM));
	frm->SetPaneText(ID_INDICATOR_SCALE,szBuffer);
}

void COnStationView::OnMenuNorth()
{
	SetExactView(0.0f,NAN,NAN,NAN,NAN,NAN);
}
void COnStationView::OnMenuSouth()
{
	SetExactView(180.0f,NAN,NAN,NAN,NAN,NAN);
}
void COnStationView::OnMenuEast()
{
	SetExactView(90.0f,NAN,NAN,NAN,NAN,NAN);
}
void COnStationView::OnMenuWest()
{
	SetExactView(270.0f,NAN,NAN,NAN,NAN,NAN);
}

void COnStationView::OnZoomIn()
{
	SetExactView(NAN,NAN,m_fMetersPerCM*0.75f,NAN,NAN,NAN);
}

void COnStationView::OnMenuRotateLeft()
{
	SetExactView(m_fRotateClockwise-15.0f,NAN,NAN,NAN,NAN,NAN);
}

void COnStationView::OnMenuRotateRight()
{
	SetExactView(m_fRotateClockwise+15.0f,NAN,NAN,NAN,NAN,NAN);
}

void COnStationView::OnMenuTiltForward()
{
	SetExactView(NAN,m_fRotateTilt+15.0f,NAN,NAN,NAN,NAN);
}

void COnStationView::OnMenuTiltBack()
{
	SetExactView(NAN,m_fRotateTilt-15.0f,NAN,NAN,NAN,NAN);
}

void COnStationView::OnZoomOut()
{
	SetExactView(NAN,NAN,m_fMetersPerCM*1.25f,NAN,NAN,NAN);
}

void COnStationView::SetExactView(float fClockwise,float fTilt,float fScale, float fDx,float fDy,float fDz)
{
	//Don't allow the thing to tilt of axis if we are in a restricted
	//viewing mode.  Since we get these calls from a variety of sources
	//it is safest to do the checks here.
	VIEWTYPE viewType=pSettings_G->m_ViewTypes[pSettings_G->m_iColorSchemeIndex];
	if (viewType==VT_PROFILE)
	{
		if (fTilt!=90.0f)
		{
			fTilt=NAN;	//Bad user.  Tried to tilt off axis.
		}
	}
	if (viewType==VT_MIXED || viewType==VT_TOP)
	{
		if (fTilt!=0.0f)
		{
			fTilt=NAN;
		}
	}
	//Check for NAN.  This allows people to call this function without
//knowing some of the values.
	if (fClockwise!=NAN)
	{
		m_fRotateClockwise=fClockwise;
	}
	if (fTilt!=NAN)
	{
		m_fRotateTilt=fTilt;
	}
	if (fScale!=NAN &&  m_fMetersPerCM!=fScale)
	{
		m_fMetersPerCM=fScale;
		SetScalingAndLighting(0);
	}

	//Do range checking for various variables.  They should both
	//be in the 0 to 360 degree range.
	if (m_fRotateTilt>360.0f)
	{
		m_fRotateTilt=m_fRotateTilt-360.0f;
	}
	if (m_fRotateTilt<0.0f)
	{
		m_fRotateTilt=m_fRotateTilt+360.0f;
	}
	if (m_fRotateClockwise<0.0f)
	{
		m_fRotateClockwise=m_fRotateClockwise+360.0f;
	}
	if (m_fRotateClockwise>360.0f)
	{
		m_fRotateClockwise=m_fRotateClockwise-360.0f;
	}	
	
	//Within reasonable bounds on the scaling thing
	if (m_fMetersPerCM<MIN_ZOOM)
		m_fMetersPerCM=MIN_ZOOM;
	if (m_fMetersPerCM>MAX_ZOOM)
		m_fMetersPerCM=MAX_ZOOM;

	//restrict pan values to area occupied by the cave
	if (fDx!=NAN)
	{
		m_fXPan=fDx;
	}
	if (fDy!=NAN)
	{
		m_fYPan=fDy;
	}
	if (fDz!=NAN)
	{
		m_fZPan=fDz;
	}

	RedrawView();
	CMainFrame * pFrame=dynamic_cast<CMainFrame *>(AfxGetMainWnd());
	if (pFrame==NULL)
		return;

	if (fScale!=NAN)
	{
		pFrame->m_wndRuler.UpdateScale(m_fMetersPerCM);
	}
	RefreshStatusIndicators(pFrame);
	if (fTilt!=NAN || fClockwise!=NAN)
	{
		m_Compass.SetClinoAndCompass(m_fRotateTilt,m_fRotateClockwise);
	}
}

//To get realistic movements, we must project our movement commands through
//the current transformation.
void COnStationView::OnMoveDelta(int iX,int iY,int iZ)
{
//	TRACE(_T("Move Delta %i %i %i\n"),iX,iY,iZ);
	//pixels*cm/pixels *m/cm=screen meters
	float fX,fY,fZ;
	fX=((float)iX)*m_fMouseConvertX*m_fMetersPerCM;
	fY=((float)iY)*m_fMouseConvertY*m_fMetersPerCM;
	fZ=((float)iZ)*m_fMouseConvertY*m_fMetersPerCM;
	CPosMatrix P;

//	TRACE(_T("Converted Delta %i %i %i\n"),(int)fX,(int)fY,(int)fZ);
	P.Set(fX,fY,fZ);

   	//Calculate a backwards transform from the view to us
	CViewMatrix VM;
	VM.RotateX(m_fRotateTilt);
	VM.RotateZ(m_fRotateClockwise);
	P.Multiply(&P,VM);
	P.Normalize();
//	TRACE(_T("Transformed Delta %i %i %i\n"),(int)P.GetX(),(int)P.GetY(),(int)P.GetZ());

	SetExactView(NAN,NAN,NAN,m_fXPan-P.GetX(),m_fYPan-P.GetY(),m_fZPan-P.GetZ());
}		  


void COnStationView::OnRotateDelta(int iX,int iY)
{
	m_iAnimateDeltaX=iX;
	m_iAnimateDeltaY=iY;
	switch (pSettings_G->m_ViewTypes[pSettings_G->m_iColorSchemeIndex])
	{
		case VT_TOP:
		case VT_PROFILE:
		case VT_MIXED:
			{
			float lx=(float)iX;
			SetExactView(m_fRotateClockwise+lx*ROTATION_DAMPER,NAN,NAN,NAN,NAN,NAN);
			}
			break;
		case VT_FREEFORM:
			{
			float lx=(float)iX;
			float ly=(float)-iY;
		    SetExactView(m_fRotateClockwise+lx*ROTATION_DAMPER,m_fRotateTilt+ly*ROTATION_DAMPER,NAN,NAN,NAN,NAN);
			}
			break;
		default:
			ASSERT(FALSE);	//missed one.
	}
}

void COnStationView::TrackMouse(TRACKTYPE T, CPoint oldPoint) 
{
	m_iAnimateDeltaX=0;
	m_iAnimateDeltaY=0;
	CPoint SavedPoint=oldPoint;
	if (!ClipCursor(oldPoint))
		return;

	SetCapture();
	BOOL m_bMoved=FALSE;
	m_bRotatingSystem=TRUE;
	while(1)
	{
		MSG msg;
#ifndef _MAC
		if (!::GetMessage(&msg, NULL, 0, 0))
#else
		// don't allow yielding while tracking since we don't have LockWindowUpdate
		if (!PeekMessage(&msg, NULL, 0, 0, PM_REMOVE|PM_NOYIELD))
			continue;
		if (msg.message == WM_QUIT)
#endif
		{
			::PostQuitMessage(msg.wParam);
			break;
		}

		switch (msg.message)
		{
		case WM_RBUTTONUP:
			m_bRotatingSystem=FALSE;
			if (m_bMoved)
			{
				RedrawView();
			}
			::ClipCursor(NULL);
			ReleaseCapture();
			//Single right click is our indicator for a context sensitive popup
			if (!m_bMoved)
			{
				CMenu Menu;
				ClientToScreen(&SavedPoint);
				VERIFY(Menu.LoadMenu(IDR_VIEWPORTMENU));
				CMenu* pPopup = Menu.GetSubMenu(0);
				if (pPopup!=NULL)
				{
					pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, SavedPoint.x,SavedPoint.y,this);
				}
				Menu.DestroyMenu();
			}
			return;		//done processing!

		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_LBUTTONDOWN:
			{
			//Do the final move before the button up or down.  This is very
			//important because of our timeout constraints below.  We want to
			//avoid the situation where somebody does a quick zoom and has
			//all but the first point discarded because of the 400ms timeout.
			m_bRotatingSystem=FALSE;
			int dx=LOWORD(msg.lParam)-oldPoint.x;
			int dy=HIWORD(msg.lParam)-oldPoint.y;
			if (dx!=0 || dy!=0)
			{
				//WARNING:  THis code is repeated IN WM_MOUSEMOVE
				m_bMoved=TRUE;
				switch (T)
				{
					case T_LEFT:
						OnMoveDelta(dx,-dy,0);
						break;
					case T_LEFTSHIFT:
					case T_LEFTCONTROL:
						OnZoomDelta(dx);
						break;
					case T_RIGHT:
						OnRotateDelta(dx,dy);
						break;
					case T_RIGHTSHIFT:
					case T_RIGHTCONTROL:
						OnMoveDelta(0,0,dx);
						break;
					default:
						ASSERT(FALSE);
				}
			}
			if (m_bMoved)
			{
				RedrawView();
			}
			::ClipCursor(NULL);
			ReleaseCapture();
			}
			return;
		case WM_MOUSEMOVE:
		{
			//if more then 400 milliseconds have passed since the
			//message was posted we assume that the message is
			//stale and no longer process it.  This should help on Win95
			//where several mouse events seem to get posted causing
			//use to be VERY sluggish on slow systems.   -TVI
			if (GetTickCount()-msg.time>400)
			{
				break;
			}
			int dx=LOWORD(msg.lParam)-oldPoint.x;
			int dy=HIWORD(msg.lParam)-oldPoint.y;
			if (dx!=0 || dy!=0)
			{
				//WARNING:  THis code is repeated IN WM_LBUTTONUP
				m_bMoved=TRUE;
				switch (T)
				{
					case T_LEFT:
						if (m_bTopHalf)
						{
							OnMoveDelta(dx,-dy,0);
						}
						else
						{
							OnMoveDelta(dx,0,-dy);
						}
						break;
					case T_LEFTSHIFT:
					case T_LEFTCONTROL:
						OnZoomDelta(dx);
						break;
					case T_RIGHT:
						OnRotateDelta(dx,dy);
						break;
					case T_RIGHTSHIFT:
					case T_RIGHTCONTROL:
						OnMoveDelta(0,0,dx);
						break;
					default:
						ASSERT(FALSE);
				}
			}

			oldPoint.x=LOWORD(msg.lParam);
			oldPoint.y=HIWORD(msg.lParam);
			
		}
			break;
		case WM_KEYDOWN:
			if (msg.wParam == VK_ESCAPE)
			{
				m_bRotatingSystem=FALSE;
				RedrawView();
				::ClipCursor(NULL);
				ReleaseCapture();
				return;
			}
			break;

		// just dispatch rest of the messages
		default:
			DispatchMessage(&msg);
			break;
		}
	}
	m_bRotatingSystem=FALSE;		//just in case
}


void COnStationView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (nFlags&MK_SHIFT)
		TrackMouse(T_LEFTSHIFT,point);
	else if (nFlags&MK_CONTROL)
		TrackMouse(T_LEFTCONTROL,point);
	else
		TrackMouse(T_LEFT,point);
}

void COnStationView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if (nFlags&MK_SHIFT)
	{
		TrackMouse(T_RIGHTSHIFT,point);
	}
	else if (nFlags&MK_CONTROL)
	{
		TrackMouse(T_RIGHTCONTROL,point);
	}
	else
	{
		TrackMouse(T_RIGHT,point);
	}
}

BOOL COnStationView::ClipCursor(CPoint oldPoint)
{
	CRect rect=GetMyClientArea();
	if (rect.PtInRect(oldPoint))
	{
		ClientToScreen(&rect);
		::ClipCursor(&rect);
		m_bTopHalf=TRUE;
		return TRUE;
	}
	else
	{
		m_bTopHalf=FALSE;
		CRect rect=GetMyBottomClientArea();
		if (rect.PtInRect(oldPoint))
		{
			ClientToScreen(&rect);
			::ClipCursor(&rect);
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
}

void COnStationView::RedrawView()
{
	InvalidateRect(NULL,FALSE);
	UpdateWindow();
}

void COnStationView::OnZoomDelta(int iZoom)
{
	iZoom=iZoom/3;	//We were just going too fast
	double power=(double)iZoom;
	double zoom=pow((double)ZOOMINCREMENTS,power);
	SetExactView(NAN,NAN,m_fMetersPerCM*(float)zoom,NAN,NAN,NAN);
}

CRect COnStationView::GetMyClientArea()
{
	CRect r;
	GetClientRect(&r);
	if (pSettings_G->m_ViewTypes[pSettings_G->m_iColorSchemeIndex]==VT_MIXED)
	{
		r.bottom=r.bottom/2-3;
	}
	return r;
}

CRect COnStationView::GetMyBottomClientArea()
{
	ASSERT(pSettings_G->m_ViewTypes[pSettings_G->m_iColorSchemeIndex]==VT_MIXED);
	CRect r;
	GetClientRect(&r);
	r.top=r.bottom/2+3;
	return r;
}

void COnStationView::GLRenderScene(int iHalf)
{
	// Draw in background in black
	glRGB(0,0,220);

	if (iHalf==0)
	{
		// Clear the window with current clearing color
		glClearColor(0.0f,0.0f,0.0f,0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	// Save the modelview matrix stack
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	if (pSettings_G->m_ViewTypes[pSettings_G->m_iColorSchemeIndex]==VT_MIXED)
	{
		if (iHalf==1)
		{
			glRotated(-m_fRotateTilt,1.0,0.0,0.0);
		}
		else
		{
			glRotated(-m_fRotateTilt-90.0f,1.0,0.0,0.0);
		}
	}
	else
	{
		glRotated(-m_fRotateTilt,1.0,0.0,0.0);
	}
	glRotated(-m_fRotateClockwise,0.0,0.0,1.0);
//	TRACE(_T("OpenGL Pan *10=%i %i %i\n"),(int)(-m_fXPan*10.0f),(int)(-m_fYPan*10.0f),(int)(-m_fZPan*10.0f));
	glTranslated(-m_fXPan,-m_fYPan,-m_fZPan);

	//Do the drawing here
//	SetupTestGeometry();
	int iColorScheme=pSettings_G->m_iColorSchemeIndex;
	m_MyGWorld->GenerateGeometry(
		(m_bAnimate||m_bRotatingSystem)?pSettings_G->m_bRotateNoSolids[iColorScheme]:FALSE,FALSE,
		pSettings_G->m_bBoxFixedPoints[iColorScheme],
		pSettings_G->m_bShowClosureErrors[iColorScheme]);

	// Restore the modelview matrix
	glPopMatrix();

	// Flush drawing commands
	glFlush();
}


BOOL COnStationView::GetViewportAndExtents(int iHalf,CRect * rViewPort,float * fLeft,float *fRight,float *fTop,float *fBottom)
{
	if (pSettings_G->m_ViewTypes[pSettings_G->m_iColorSchemeIndex]==VT_MIXED)
	{
		if (iHalf==1)
		{
			*rViewPort=GetMyBottomClientArea();
		}
		else
		{
			*rViewPort=GetMyClientArea();
		}
	}
	else
	{
		*rViewPort=GetMyClientArea();
	}
	if (rViewPort->Width()<=0 || rViewPort->Height()<=0)
	{
		return FALSE;
	}
	CPoint cs(rViewPort->Width(),rViewPort->Height());

	int iOldMapMode=SetMapMode(m_hDC,MM_HIMETRIC);
	//figure out how many mm wide the screen is
	DPtoLP(m_hDC,&cs,1);
	cs.y=-cs.y;	//Normal coordinates are upside down
	SetMapMode(m_hDC,iOldMapMode);

	//now cs is in .01 of a mm.  scale is in m/cm
	//so we multiply by 1000 to get cm.  The result
	//show be the width of the viewport in CM
	float fSizeX=((float)cs.x)/1000.0f;
	float fSizeY=((float)cs.y)/1000.0f;
	float fMetersX=fSizeX*m_fMetersPerCM;
	float fMetersY=fSizeY*m_fMetersPerCM;
	*fLeft=-fMetersX/2;
	*fRight=fMetersX/2;
	*fTop=-fMetersY/2;
	*fBottom=fMetersY/2;
//	TRACE(_T("GetViewportandExtents %i returning rect %i,%i %i,%i\n"),iHalf,rViewPort->left,rViewPort->top,rViewPort->right,rViewPort->bottom);
	return TRUE;
}

void COnStationView::SetScalingAndLighting(int iHalf)
{
	CRect r;
	float fLeft,fRight,fTop,fBottom;
	if (!GetViewportAndExtents(iHalf,&r,&fLeft,&fRight,&fTop,&fBottom))
	{
		return;
	}

	if (iHalf==0)
	{
		VERIFY(wglMakeCurrent(m_hDC,m_hRC));
#ifdef _DEBUG
		//We were getting exceptions in this call for some
		//unknown reason.  This may help track it.
//		GLenum enumt=glGetError();
//		ASSERT(enumt==GL_NO_ERROR);
		int iError=GetLastError();
		if (iError!=0)
		{
			TRACE(_T("ERROR %i IN CView::DRAW LOOP\n"),iError);
		}
		//returns GetLastError=120 (stub API call for wglMakeCurrent);
#endif
	}
	// Set Viewport to window dimensions
//    TRACE(_T("glViewportScreen %i,%i,%i,%i\n"),r.left, r.top, r.Width(), r.Height());
    glViewport(r.left, r.top, r.Width(), r.Height());
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

//	TRACE(_T("glOrthoScreen %f %f %f %f\n"),fLeft,fRight,fTop,fBottom);

	//Set depth of clipping volume dependent on scale.  if
	//we set it too small we have trouble clipping small stuff
	//otherwise we have trouble clipping deep stuff.
	float fBack=2000.0f;
	if (m_fMetersPerCM>100.0f)
	{
		fBack=5000.0f;
	}
	if (m_fMetersPerCM>1000.0f)
	{
		fBack=30000.0f;
	}
	glOrtho(fLeft,fRight,fTop,fBottom,-fBack,fBack*2.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (!m_bRotatingSystem)
	{
		glEnable(GL_DEPTH_TEST);
		glFrontFace(GL_CW);
//		glEnable(GL_CULL_FACE);
	}
	int iColorScheme=pSettings_G->m_iColorSchemeIndex;
	if ((pSettings_G->m_PassageType[iColorScheme]==PT_FULL_PASSAGES || pSettings_G->m_PassageType[iColorScheme]==PT_FULL_PASSAGES_SKINNY) && pSettings_G->m_bMultipleLights[iColorScheme] && (!m_bRotatingSystem || !pSettings_G->m_bRotateNoLighting[iColorScheme]))
	{
		// Light values and coordinates
		GLfloat  whiteLight[] = {0.7f, 0.7f, 0.7f, 1.0f };
		GLfloat  specular[] = { 0.7f, 0.7f, 0.7f, 1.0f};
		GLfloat  shine[] = { 100.0f };


		// Enable lighting
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		// Enable color tracking
		glEnable(GL_COLOR_MATERIAL);
		
		// Set Material properties to follow glColor values
		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, shine);
	}
	else
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
	}
/*	if ((pSet->m_PassageType==PT_FULL_PASSAGES || pSet->m_PassageType==PT_FULL_PASSAGES_SKINNY) && pSet->m_bDepthFogging && !m_bRotatingSystem)
	{
		float fog_color[4]={0.0f,0.0f,0.0f,0.0f};
		glEnable(GL_FOG);
		glFogi(GL_FOG_MODE,GL_LINEAR);
//		glFogf(GL_FOG_DENSITY,0.0025);
		glFogfv(GL_FOG_COLOR,fog_color);
	}
	else
	{
		glDisable(GL_FOG);
	}
	*/
	glDisable(GL_FOG);
	if (iHalf==0)
	{
		VERIFY(wglMakeCurrent(NULL,NULL));
	}
}

#ifdef _DEBUG
void COnStationView::SetupTestGeometry()
{
	glRGB(255,0,0);
	glBegin(GL_LINES);
	glVertex3f(0.0f,0.0f,0.0f);
	glVertex3f(0.0f,100.0f,-100.0f);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(0.0f,100.0f,-100.0f);
	glVertex3f(50.0f,150.0f,-200.0f);
	glEnd();
	glRGB(0,255,0);
	glBegin(GL_TRIANGLES);
	glVertex3f(30.f,30.f,300.f);
	glVertex3f(30.f,40.f,300.f);
	glVertex3f(40.f,40.f,300.f);
	glEnd();
	glRGB(255,0,0);
	glBegin(GL_TRIANGLES);
	glVertex3f(60.f,60.f,-1000.f);
	glVertex3f(60.f,70.f,-1000.f);
	glVertex3f(70.f,70.f,-1000.f);
	glEnd();
	glRGB(0,0,255);	 //Blue one in the  middle
	glBegin(GL_TRIANGLES);
	glVertex3f(-10.0f,-10.0f,0.0f);
	glVertex3f(-10.0f,10.f,0.0f);
	glVertex3f(10.0f,0.0f,0.0f);
	glEnd();
}

#endif

void COnStationView::OnViewportStartingposition() 
{
	SetExactView(0.0f,0.0f,100.0f,0.0f,0.0f,0.0f);
}

void COnStationView::OnSearchShowstation() 
{
	CCenterStationNameDialog dlg;
	if (dlg.DoModal()==IDOK)
	{
		CNode * pNode=GetDocument()->GetNodeTree()->FindNode(dlg.m_szEditStation,FALSE);
		if (pNode!=NULL)
		{
			CPosMatrix pPos;
			if (pSettings_G->m_bShowClosedLoops[pSettings_G->m_iColorSchemeIndex])
			{
				pPos=pNode->GetNodePositionInfo()->m_ClosedPosition;
			}
			else
			{
				pPos=pNode->GetNodePositionInfo()->m_RawPosition;
			}
			SetExactView(NAN,NAN,NAN,pPos.GetX(),pPos.GetY(),pPos.GetZ());
		}
		else
		{
			AfxMessageBox(IDS_INVALIDSTATIONNAME);
		}
	}
}

void COnStationView::DrawGDIOverlay(CDC * pDC,int iHalfOfScreen)
{
	if (!m_bRotatingSystem || !pSettings_G->m_bRotateNoSolids[pSettings_G->m_iColorSchemeIndex])
	{
		CViewMatrix view;

		CRect r;
		float fLeft,fRight,fTop,fBottom;
		if (!GetViewportAndExtents(iHalfOfScreen,&r,&fLeft,&fRight,&fTop,&fBottom))
		{
			return;
		}
		pDC->IntersectClipRect(r);

		//if center is at X,Y,Z we must pan to -X,-Y,-Z
		view.XPan(-m_fXPan);
		view.YPan(-m_fYPan);
		view.ZPan(-m_fZPan);
		view.RotateZ(-m_fRotateClockwise);
		view.RotateX(-m_fRotateTilt+(iHalfOfScreen==0?0.0f:-90.0f));
		int iColorScheme=pSettings_G->m_iColorSchemeIndex;
		CFont * pFont=theApp.GetSettings()->GetFont();
		m_MyGWorld->DrawText(pDC,&view,pFont,pSettings_G->m_ShowStationNames[iColorScheme],r,fLeft,fRight,fTop,fBottom);
		delete pFont;
		m_MyGWorld->DrawTaggedPoints(pDC,&view,r,fLeft,fRight,fTop,fBottom,!pSettings_G->m_bBoxFixedPoints[iColorScheme]);
		pDC->SetViewportOrg(0,0);

		if (m_pType!=NULL)
		{
//			CRect rect;
//			GetClientRect(&rect);
			r.bottom=20;
			CString CS;
			CS.GetBuffer(128);
			CS.LoadString(IDS_SEARCH);
			CS+=m_pType->GetQueryText();
			CS+= _T(" ");
			CS+=m_pComment->GetQueryText();
			pDC->SetTextColor(RGB(255,0,0));
			pDC->DrawText(CS,r,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		}
		pDC->SelectClipRgn(NULL);
	}
}

void COnStationView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CViewMatrix view;

	CRect r;
	float fLeft,fRight,fTop,fBottom;
	int iHalfOfScreen=0;
	if (pSettings_G->m_ViewTypes[pSettings_G->m_iColorSchemeIndex]==VT_MIXED)
	{
		r=GetMyBottomClientArea();
		if (r.PtInRect(point))
		{
			iHalfOfScreen=1;
		}
	}
	if (!GetViewportAndExtents(iHalfOfScreen,&r,&fLeft,&fRight,&fTop,&fBottom))
	{
		return;
	}
	//if center is at X,Y,Z we must pan to -X,-Y,-Z
	view.XPan(-m_fXPan);
	view.YPan(-m_fYPan);
	view.ZPan(-m_fZPan);
	view.RotateZ(-m_fRotateClockwise);
	view.RotateX(-m_fRotateTilt+(iHalfOfScreen==0?0.0f:-90.0f));
	int iColorScheme=pSettings_G->m_iColorSchemeIndex;
	CPtrArray * pMyArray=m_MyGWorld->GetHitList(&view,point,pSettings_G->m_ShowStationNames[iColorScheme],pSettings_G->m_bBoxFixedPoints[iColorScheme],r,fLeft,fRight,fTop,fBottom);
	if (pMyArray->GetSize()!=0)
	{
		CSearchResultDialog mDlg(this,pMyArray);
		mDlg.DoModal();
		//Where any fixed points changed
		if (mDlg.m_bDirty)
		{
			//and fix the toolbar on the right
			CMainFrame * pMainFrame=dynamic_cast<CMainFrame *>(AfxGetMainWnd());
			pMainFrame->m_dataView.OnChangedDocumentData();

			//redraw the system
			RecalculateGeometry(TRUE);
		}
	}
	delete pMyArray;

}

void COnStationView::OnViewAnimate() 
{
	m_iAnimateDeltaX=0;
	m_iAnimateDeltaY=0;
	m_bAnimate=!m_bAnimate;
	if (m_bAnimate)
	{
		SetTimer(0,100,NULL);
	}
	else
	{
		KillTimer(0);
		RedrawView();
	}
}

void COnStationView::OnUpdateViewAnimate(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bAnimate);
}

void COnStationView::OnTimer(UINT_PTR nIDEvent) 
{
	if (m_bAnimate && (m_iAnimateDeltaX!=0 || m_iAnimateDeltaY!=0))
	{
		BOOL bOldRotate=m_bRotatingSystem;
		m_bRotatingSystem=TRUE;
		OnRotateDelta(m_iAnimateDeltaX,m_iAnimateDeltaY);
		RedrawView();
		m_bRotatingSystem=bOldRotate;
	}
	
	CView::OnTimer(nIDEvent);
}

void COnStationView::OnViewDistancebetweenstations() 
{
	CDistanceBetweenDialog dlg;
	dlg.DoModal();
	
}

void COnStationView::OnFilePrintPreview()
{
 	CMainFrame * pFrame=dynamic_cast<CMainFrame *>(AfxGetMainWnd());
	if (pFrame!=NULL)
	{
		pFrame->SetPrintPreview(TRUE);
	}
	CView::OnFilePrintPreview();
}

void COnStationView::OnExportDxf()
{
	TCHAR szReturn[512];
	szReturn[0]=_T('\0');

	OPENFILENAME of;
	of.lStructSize=sizeof(OPENFILENAME);
	of.hwndOwner=m_hWnd;
	of.hInstance=AfxGetInstanceHandle();
	of.lpstrFilter=_T("(DXF Files)\0*.dxf\0\0");
	of.lpstrCustomFilter=NULL;
	of.nFilterIndex=0;
	of.lpstrFile=szReturn;
	of.nMaxFile=_countof(szReturn);
	of.lpstrFileTitle=NULL;
	of.lpstrInitialDir=NULL;
	of.lpstrTitle=_T("Export to DXF File");
	of.Flags=OFN_HIDEREADONLY;
	of.lpstrDefExt=_T("dxf");

	if (GetOpenFileName(&of)==TRUE)
	{
		CStdioFile file(szReturn,CFile::modeCreate|CFile::modeWrite);
		int iColorScheme=pSettings_G->m_iColorSchemeIndex;
		m_MyGWorld->WriteGWorldToDXF(&file, pSettings_G->m_bShowClosureErrors[iColorScheme], 
											pSettings_G->m_ShowStationNames[iColorScheme], 
											pSettings_G->m_bBoxFixedPoints[iColorScheme]);
		file.Close();
	}	
}

void COnStationView::SetColorTable(HDC hdcMemory)
{
	RGBQUAD pColors[256];
	// Build mask of all 1's.  This creates a number represented by having
	// the low order x bits set, where x = pfd.cRedBits, pfd.cGreenBits, and
	// pfd.cBlueBits.  
	BYTE RedRange = 7;
	BYTE GreenRange = 7;
	BYTE BlueRange = 3;

	TCHAR cRedShift=0;
	TCHAR cGreenShift=3;
	TCHAR cBlueShift=6;

	// Loop through all the palette entries
	for (int i = 0; i < 256; i++)
	{
		pColors[i].rgbReserved=0;
		// Fill in the 8-bit equivalents for each component
		pColors[i].rgbRed= (i >> cRedShift) & RedRange;
		pColors[i].rgbRed = (unsigned char)(
			(double) pColors[i].rgbRed * 255.0 / RedRange);

		pColors[i].rgbGreen = (i >> cGreenShift) & GreenRange;
		pColors[i].rgbGreen = (unsigned char)(
			(double)pColors[i].rgbGreen * 255.0 / GreenRange);

		pColors[i].rgbBlue = (i >> cBlueShift) & BlueRange;
		pColors[i].rgbBlue = (unsigned char)(
			(double)pColors[i].rgbBlue * 255.0 / BlueRange);
	}

	SetDIBColorTable(hdcMemory,0,255,pColors);
}


void COnStationView::OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView) 
{
	CView::OnEndPrintPreview(pDC, pInfo, point, pView);
 	CMainFrame * pFrame=dynamic_cast<CMainFrame *>(AfxGetMainWnd());
	if (pFrame!=NULL)
	{
		pFrame->SetPrintPreview(FALSE);
	}
}

void COnStationView::DeleteContents()
{
	SetCommentQuery(NULL,NULL);
//	delete m_pType;
//	m_pType=NULL;
//	delete m_pComment;
//	m_pComment=NULL;
}

void COnStationView::SetCommentQuery(CQuery *pTypeQuery,CQuery * pCommentQuery)
{
	delete m_pType;
	delete m_pComment;
	m_pType=pTypeQuery;
	m_pComment=pCommentQuery;
	if (m_pType!=NULL)
	{
		//Reset the old one first
		GetDocument()->GetNodeTree()->UnmarkQueryAllNodes();
		
		if (!GetDocument()->GetSurveyFolder()->QuerySurveyComments(NULL,pTypeQuery,pCommentQuery))
		{
			AfxMessageBox(IDS_NOMATCHONSEARCH);
			delete m_pComment;
			delete m_pType;
			m_pComment=NULL;
			m_pType=NULL;
		}
		RecalculateGeometry(FALSE);
	}
	//Only redraw if needed
	else if (GetDocument()->GetNodeTree()->UnmarkQueryAllNodes())
	{
		RecalculateGeometry(FALSE);
	}
}


void COnStationView::ShowCompass(BOOL bShow)
{
	if (IsWindow(m_Compass.m_hWnd))
	{
		m_Compass.ShowWindow(bShow?SW_SHOW:SW_HIDE);
	}
	else
	{
		if (bShow)
		{
			if (!m_Compass.Create(this,106))
			{
				TRACE(_T("Failed to create compass in showcompass\n"));
			}
		}
	}
}

void COnStationView::OnQueryBlunders()
{
	if (pSettings_G->m_iColorSchemeIndex!=COLORSCHEME_BLUNDER)
	{
		if ((AfxMessageBox(_T("Would you like your color scheme to automatically be set to blunder coloring.  This will color shots based on the likely amount of error.")),MB_YESNO)==IDYES)
		{
			OnDrawmodeBlunderColoring();
		}
	}
	CBlunderDialog dlg;
	dlg.DoModal();
	if (dlg.m_bDirty)
	{
		//Update the fixed point list
		CMainFrame * pMainFrame=dynamic_cast<CMainFrame *>(AfxGetMainWnd());
		pMainFrame->m_dataView.OnChangedDocumentData();
		//and the cave itself
		RecalculateGeometry(TRUE);
	}
}

// Handles mouse wheel notifications
BOOL COnStationView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// slower zoom if Ctrl or Shift key are pressed
    int iZoom = zDelta / ((nFlags & (MK_CONTROL|MK_SHIFT)) > 0 ? 6 : 3);
	OnZoomDelta(iZoom);

    return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}
