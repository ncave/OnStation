// OnStationView.h : interface of the COnStationView class
//
/////////////////////////////////////////////////////////////////////////////
#include "settings.h"
#include "gworld.h"
#include "compasswidget.h"
#include "nodetree.h"
enum TRACKTYPE {T_LEFT,T_LEFTSHIFT,T_LEFTCONTROL,T_RIGHT,T_RIGHTSHIFT,T_RIGHTCONTROL};

class COnStationView : public CView
{
private:
	void SetColorTable(HDC hdcMemory);
	//What is our window on this system?	
	float m_fRotateClockwise;
	float m_fRotateTilt;
	float m_fXPan;
	float m_fYPan;
	float m_fZPan;
	float m_fMetersPerCM;

	void HandlePrintOfView(CDC * pDC,CRect rPrintedPage,float fLeft,float fTop,float fRight,float fBottom/*,int iYpos*/);
	void TrackMouse(TRACKTYPE T, CPoint oldPoint);
	BOOL m_bRotatingSystem;
	BOOL m_bTopHalf;
	float m_fMouseConvertX;
	float m_fMouseConvertY;
	//Clips to the appropriate half of the screen
	//depending on the view mode and so forth
	BOOL ClipCursor(CPoint oldPoint);
	void OnRotateDelta(int iX,int iY);
	void OnMoveDelta(int iX,int iY,int iZ);
	void OnZoomDelta(int iZoom);
	CRect GetMyClientArea();
	CRect GetMyBottomClientArea();
#ifdef _DEBUG
	void SetupTestGeometry();
#endif
protected: // create from serialization only
	HGLRC m_hRC;	// Rendering Context
	HDC m_hDC;		// Device Context

	CPalette m_GLPalette;	// Logical Palette

	COnStationView();
	DECLARE_DYNCREATE(COnStationView)
	CGWorld * m_MyGWorld;
	CCompassWidget m_Compass;
#ifdef _DEBUG
	void PerformanceMetrics();
#endif
	//printer support
	void GetPrintedPageBoundaries(CRect * rPage,int iPageNumber,float * fLeft,float *fTop,float *fRight,float *fBottom);
	void GetPrinterInformation(CDC * pDC);
	int m_nPageWidth;
	int m_nPageHeight;
	int m_iWidthInPages;
	int m_iHeightInPages;
	int m_iTopFudge;	//The fudge variables show how much extra was added to 
	int m_iBottomFudge;	//make room for rulers and the like
	int m_iLeftFudge;
	int m_iRightFudge;
	CQuery * m_pType;
	CQuery * m_pComment;
	CRect m_PrintedAreaRectangle;
	BOOL m_bAnimate;	//Auto rotate the cave if this is set.
	int m_iAnimateDeltaX;
	int m_iAnimateDeltaY;
	void GLRenderScene(int iHalf);
	void GLSetupRC(void *pData);

	void InitializePalette(void);
	void DisableAero();
	void DrawGDIStuff(CDC* pDC);
	void DrawGDIOverlay(CDC * pDC,int iHalfOfScreen);
	// Attributes
public:
	COnStationDoc* GetDocument();
	void DeleteContents();
	void SetCommentQuery(CQuery *pType,CQuery * pComment);
	void ShowCompass(BOOL bShowCompass);
	
	void SetExactView(float fClockwise,float fTilt,float fScale, float fDx,float fDy,float fDz);
	void RedrawView();

// Operations
public:
	BOOL OnQueryNewPalette();
	void OnPaletteChanged(CWnd* pFocusWnd);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COnStationView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual void OnPrint(CDC *,CPrintInfo *);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView);
	//}}AFX_VIRTUAL
	void SetScalingAndLighting(int iHalf);

	BOOL GetViewportAndExtents(int iHalf,CRect * pViewPort,float * fLeft,float *fRight,float *fTop,float *fBottom);
	void OnSize(UINT nType, int cx, int cy);
// Implementation
public:
	virtual ~COnStationView();
	void RecalculateGeometry(BOOL bGeometry);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
protected:
	void UpdateColorScheme(CCmdUI *pCmdUI,int iIndex);
public:
	void RefreshStatusIndicators(CMainFrame *frm);
	void PickColorScheme(int iIndex);
// Generated message map functions
protected:
	//{{AFX_MSG(COnStationView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdateDrawmodeBlunderColoring(CCmdUI* pCmdUI);
	afx_msg void OnDrawmodeBlunderColoring();
	afx_msg void OnUpdateDrawmodeAgecoloring(CCmdUI* pCmdUI);
	afx_msg void OnDrawmodeAgecoloring();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnExportDxf();
	afx_msg void OnUpdateDrawmodeDepthcoding(CCmdUI* pCmdUI);
	afx_msg void OnDrawmodeDepthcoding();
	afx_msg void OnUpdateDrawmodePerspective(CCmdUI* pCmdUI);
	afx_msg void OnDrawmodePerspective();
	afx_msg void OnUpdateViewNamenone(CCmdUI* pCmdUI);
	afx_msg void OnViewNamenone();
	afx_msg void OnQueryBlunders();
	afx_msg void OnUpdateViewNamejunctions(CCmdUI* pCmdUI);
	afx_msg void OnViewNamejunctions();
	afx_msg void OnUpdateViewNameall(CCmdUI* pCmdUI);
	afx_msg void OnViewNameall();
	afx_msg void OnViewSetexactview();
	afx_msg void OnMovementProfileview();
	afx_msg void OnUpdateMovementProfileview(CCmdUI* pCmdUI);
	afx_msg void OnMovementPlanview();
	afx_msg void OnUpdateMovementPlanview(CCmdUI* pCmdUI);
	afx_msg void OnMovementMixedview();
	afx_msg void OnUpdateMovementMixedview(CCmdUI* pCmdUI);
	afx_msg void OnMovementFreeformview();
	afx_msg void OnUpdateMovementFreeformview(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewSurfaceoverlay(CCmdUI* pCmdUI);
	afx_msg void OnViewSurfaceoverlay();
	afx_msg void OnViewSurfaceandlocationsurveys();
	afx_msg void OnUpdateViewSurfaceandlocationsurveys(CCmdUI* pCmdUI);
	afx_msg void OnViewCloseloops();
	afx_msg void OnUpdateViewCloseloops(CCmdUI* pCmdUI);
	afx_msg void OnEditConfigurecolors();
	afx_msg void OnViewPassageoptions();
	afx_msg void OnDrawmodeColorscheme1();
	afx_msg void OnUpdateDrawmodeColorscheme1(CCmdUI* pCmdUI);
	afx_msg void OnDrawmodeColorscheme2();
	afx_msg void OnUpdateDrawmodeColorscheme2(CCmdUI* pCmdUI);
	afx_msg void OnDrawmodeColorscheme3();
	afx_msg void OnUpdateDrawmodeColorscheme3(CCmdUI* pCmdUI);
	afx_msg void OnDrawmodeColorscheme4();
	afx_msg void OnUpdateDrawmodeColorscheme4(CCmdUI* pCmdUI);
	afx_msg void OnDrawmodeColorscheme5();
	afx_msg void OnUpdateDrawmodeColorscheme5(CCmdUI* pCmdUI);
	afx_msg void OnDrawmodeColorscheme6();
	afx_msg void OnUpdateDrawmodeColorscheme6(CCmdUI* pCmdUI);
	afx_msg void OnDrawmodeColorscheme7();
	afx_msg void OnUpdateDrawmodeColorscheme7(CCmdUI* pCmdUI);
	afx_msg void OnDrawmodeColorscheme8();
	afx_msg void OnUpdateDrawmodeColorscheme8(CCmdUI* pCmdUI);
	afx_msg void OnEditOrphanedshots();
	afx_msg void OnUpdateEditOrphanedshots(CCmdUI* pCmdUI);
	afx_msg void OnViewInformation();
	afx_msg void OnEditBookmarks();
	afx_msg void OnOptionsMarkfixedpoints();
	afx_msg void OnUpdateOptionsMarkfixedpoints(CCmdUI* pCmdUI);
	afx_msg void OnOptionsClosureerrors();
	afx_msg void OnShowReferenceGrid();
	afx_msg void OnUpdateShowReferenceGrid(CCmdUI * pCmdUI);
	afx_msg void OnZoomIn();
	afx_msg void OnZoomOut();
	afx_msg void OnMenuNorth();
	afx_msg void OnMenuSouth();
	afx_msg void OnMenuEast();
	afx_msg void OnMenuWest();
	afx_msg void OnMenuRotateLeft();
	afx_msg void OnMenuRotateRight();
	afx_msg void OnMenuTiltForward();
	afx_msg void OnMenuTiltBack();
	afx_msg void OnUpdateOptionsClosureerrors(CCmdUI* pCmdUI);
	afx_msg void OnFilePrintingpreferences();
	afx_msg void OnFileExportvrmlfile();
	afx_msg void OnEditSearchstationcomments();
	afx_msg void OnViewFullscreen();
	afx_msg void OnViewFullscreenEsckey();
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnViewportStartingposition();
	afx_msg void OnSearchShowstation();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnViewAnimate();
	afx_msg void OnUpdateViewAnimate(CCmdUI* pCmdUI);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnViewDistancebetweenstations();
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in OnStationView.cpp
inline COnStationDoc* COnStationView::GetDocument()
   { return (COnStationDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
