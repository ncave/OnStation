// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "OnStation.h"
#include "MainFrm.h"
#include "settings.h"
#include "treehelper.h"
#include "Splash.h"
#include "overalldataview.h"
#include "surveysearchdialog.h"
#include "onstationview.h"
#include "preferences.h"
#include "onstationdoc.h"
#include "debugstationlinksdialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern COnStationDoc * pDocument_G;
extern COnStationApp theApp;
extern COnStationView * pView_G;
extern UINT uiCustomClipboardFormat_G;
extern CSettings * pSettings_G;
CToolTipCtrl GlobalToolTips_G;
/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_INITMENU()
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_HELP_CONTENTS, OnHelp)
	ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
	ON_COMMAND(ID_EDIT_ADDFOLDER, OnEditAddfolder)
	ON_COMMAND(ID_EDIT_ADDSURVEY, OnEditAddsurvey)
	ON_COMMAND(ID_EDIT_ADDFIXEDPOINT, OnEditAddfixedpoint)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_PROPERTIES, OnEditProperties)
	ON_COMMAND(ID_EDIT_FINDSSURVEY, OnEditFindssurvey)
	ON_COMMAND(ID_EDIT_ENDQUERY, OnEditEndquery)
	ON_COMMAND(ID_EDIT_EDIT, OnEditEdit)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_COMMAND(ID_VIEW_PREFERENCES, OnViewPreferences)
	ON_WM_QUERYNEWPALETTE()
	ON_WM_PALETTECHANGED()
	ON_UPDATE_COMMAND_UI(ID_EDIT_EDIT, OnUpdateEditEdit)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY,OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE,OnUpdateEditDelete)
	ON_COMMAND(ID_DEBUG_STATIONLINKS, OnDebugStationlinks)
	ON_COMMAND(ID_DRAWING_FONT, OnDrawingFont)
	ON_COMMAND(ID_ERRORSLOOPS_LOOPCLOSUREOPTIONS, OnErrorsloopsLoopclosureoptions)
	//}}AFX_MSG_MAP
	// Global help commands
	ON_COMMAND(ID_HELP, OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, OnHelp)
	ON_COMMAND(ID_DEFAULT_HELP, OnHelp)
END_MESSAGE_MAP()


static UINT BASED_CODE indicators[] =
{
	ID_SEPARATOR,			// status line indicator
	ID_INDICATOR_XPAN,
	ID_INDICATOR_YPAN,
	ID_INDICATOR_ZPAN,
	ID_INDICATOR_CLOCKWISE,
	ID_INDICATOR_TILT,
	ID_INDICATOR_SCALE,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_bCreated=FALSE;	
	m_bFullScreen=FALSE;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	GlobalToolTips_G.Create(this);

	m_bCreated=TRUE;

	if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE(_T("Failed to create toolbar\n"));
		return -1;      // fail to create
	}
	if (!m_wndToolBarFullScreen.Create(this) ||
		!m_wndToolBarFullScreen.LoadToolBar(IDR_FULLSCREENTOOLBAR))
	{
		TRACE(_T("Failed to create toolbar\n"));
		return -1;      // fail to create
	}
	CString CS;
	CS.LoadString(IDS_STANDARD);
	m_wndToolBar.SetWindowText(CS);
	CS.LoadString(IDS_FULLSCREEN);
	m_wndToolBarFullScreen.SetWindowText(CS);
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators, _countof(indicators)))
	{
		TRACE(_T("Failed to create status bar\n"));
		return -1;      // fail to create
	}
	m_dataView.Create(this,IDD_OVERALLDATAVIEW,CBRS_LEFT,0xe004);


	// TODO: Remove this if you don't want tool tips or a resizeable toolbar
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBarFullScreen.EnableDocking(0);
	CPoint pt(10,10);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	FloatControlBar(&m_wndToolBarFullScreen,pt);
	ShowControlBar(&m_wndToolBarFullScreen,FALSE,FALSE);
	int i;
	for (i=1;i<4;i++) //This assumes that the separators do not count as indicators
	{
		m_wndStatusBar.SetPaneInfo(i,indicators[i],SBPS_NORMAL,STATUS_POS_PANE_WIDTH);
	}
	m_wndStatusBar.SetPaneInfo(4,indicators[4],SBPS_NORMAL,STATUS_SPIN_PANE_WIDTH);
	m_wndStatusBar.SetPaneInfo(5,indicators[5],SBPS_NORMAL,STATUS_TILT_PANE_WIDTH);
	m_wndStatusBar.SetPaneInfo(6,indicators[6],SBPS_NORMAL,STATUS_SIZE_PANE_WIDTH);

	InitializeTreeImageList();	//used by all the tree controls


	CRect rect2;
	rect2.left=0;
	rect2.right=0;
	rect2.bottom=64;
	rect2.right=64;
	if (!m_wndRuler.Create(this,WS_CHILD|WS_VISIBLE,AFX_IDW_CONTROLBAR_FIRST+4))
	{
		TRACE(_T("Failed to create ruler\n"));
	}
	CAppSettings *Set=theApp.GetSettings();
	if (!Set->m_bRuler)
	{
		m_wndRuler.ShowWindow(SW_HIDE);
	}
	m_viewTab.AddString(IDS_FREEFORM,ID_MOVEMENT_FREEFORMVIEW,FALSE);
	m_viewTab.AddString(IDS_PLAN,ID_MOVEMENT_PLANVIEW,FALSE);
	m_viewTab.AddString(IDS_PROFILE,ID_MOVEMENT_PROFILEVIEW,FALSE);
	m_viewTab.AddString(IDS_MIXED,ID_MOVEMENT_MIXEDVIEW,FALSE);
	if (!m_viewTab.Create(this,this,WS_CHILD|WS_VISIBLE,AFX_IDW_CONTROLBAR_FIRST+5))
	{
		TRACE(_T("Failed to create view tab\n"));
	}
	if (!Set->m_bBottomTabs)
	{
		m_viewTab.ShowWindow(SW_HIDE);
	}
	SetActiveViewTab(VT_FREEFORM);

	m_wndStatusBar.ShowWindow(Set->m_bStatusBar?SW_SHOW:SW_HIDE);
	m_wndToolBar.ShowWindow(Set->m_bToolBar?SW_SHOW:SW_HIDE);

	// CG: The following line was added by the Splash Screen component.
	CSplashWnd::ShowSplashScreen(this);
	return 0;
}


BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style|=WS_MAXIMIZE;
	return CFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
void CMainFrame::SetPaneText(int iPane,LPCTSTR szText)
{
	if (m_bCreated)
	{
		m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(iPane),szText,TRUE);
	}
}

void CMainFrame::OnClose() 
{
//	if (m_bFullScreen)
//	{
//		GetWindowPlacement(&(theApp.GetSettings()->m_wndPlacement));
//	}
	
	CFrameWnd::OnClose();
}

void CMainFrame::OnInitMenu(CMenu* pMenu)
{
   CFrameWnd::OnInitMenu(pMenu);
  
}




void CMainFrame::OnEditCopy() 
{
	m_dataView.OnCopy();
}

void CMainFrame::OnEditCut() 
{
	m_dataView.OnCut();
}

void CMainFrame::OnEditDelete() 
{
	m_dataView.OnDelete();
}

void CMainFrame::OnEditAddfolder() 
{
	m_dataView.OnAddFolder();
}

void CMainFrame::OnEditAddsurvey() 
{
	m_dataView.OnAddSurvey();
}

void CMainFrame::OnEditAddfixedpoint() 
{
	m_dataView.OnAddFixedPoint();
}

void CMainFrame::OnEditPaste() 
{
	m_dataView.OnPaste();
	
}

void CMainFrame::SetPrintPreview(BOOL bEnter)
{
	if (theApp.GetSettings()->m_bRuler)
	{
		m_wndRuler.ShowWindow(bEnter?SW_HIDE:SW_SHOW);
	}
	if (!m_bFullScreen)
	{
		m_dataView.ShowWindow(bEnter?SW_HIDE:SW_SHOW);
		if (!bEnter)
		{
			//For Some reason exiting print preview activates this thing
			ShowControlBar(&m_wndToolBarFullScreen,FALSE,FALSE);
			RecalcLayout();
		}
	}
}
void CMainFrame::ViewFullScreen(COnStationView *pView)
{
	if (!m_bFullScreen)
	{
		GetWindowPlacement(&m_wplNormal);
		if (!(m_wplNormal.flags&SW_SHOWMAXIMIZED))
		{
			ShowWindow(SW_SHOWMAXIMIZED);
		}
		m_dataView.ShowWindow(SW_HIDE);
		int iMaxX=GetSystemMetrics(SM_CXSCREEN);
		int iMaxY=GetSystemMetrics(SM_CYSCREEN);
		int iTitleBarFudge=GetSystemMetrics(SM_CYMENU)+GetSystemMetrics(SM_CYCAPTION);
		int iBorder=GetSystemMetrics(SM_CYFRAME);
		MoveWindow(-iBorder,-iTitleBarFudge-5,iMaxX+iBorder*2+3,iMaxY+iTitleBarFudge+60,TRUE);
		m_wndToolBar.ShowWindow(SW_HIDE);
		m_wndStatusBar.ShowWindow(SW_HIDE);
		m_wndRuler.ShowWindow(SW_HIDE);
		ShowControlBar(&m_wndToolBarFullScreen,TRUE,TRUE);
		RecalcLayout();
		if (pView!=NULL)
		{
			pView->GetParent()->ShowWindow(SW_SHOWMAXIMIZED);
		}
	}
	else
	{
		CAppSettings *Set=theApp.GetSettings();
		m_dataView.ShowWindow(SW_SHOW);
		m_wndStatusBar.ShowWindow(Set->m_bStatusBar);
		m_wndToolBar.ShowWindow(Set->m_bToolBar);
		ShowControlBar(&m_wndToolBarFullScreen,FALSE,TRUE);
		m_wndRuler.ShowWindow(Set->m_bRuler?SW_SHOW:SW_HIDE);
		//The following code fixes a problem where some stuff
		//didn't redraw properly coming out of full screen mode
		//if we were  maximized.  In particular, the status bar
		//would show up off the bottom of the screen.
		if (m_wplNormal.flags&SW_SHOWMAXIMIZED)
		{
			ShowWindow(SW_SHOWNORMAL);
			RecalcLayout();
			SetWindowPlacement(&m_wplNormal);
		}
		SetWindowPlacement(&m_wplNormal);
		RecalcLayout();
	}
	m_bFullScreen=!m_bFullScreen;
}

void CMainFrame::OnEditProperties() 
{
	m_dataView.OnShowProperties();
}

void CMainFrame::OnEditFindssurvey() 
{
	SurveySearchDialog SurveySearch((COnStationDoc *)GetActiveDocument());
	if (SurveySearch.DoModal()==IDOK)
	{
		m_dataView.SetQuery(SurveySearch.m_pLegQuery);
	}
	else
	{
		m_dataView.SetQuery(NULL);
	}
}

void CMainFrame::OnEditEndquery() 
{
	m_dataView.SetQuery(NULL);
	pView_G->SetCommentQuery(NULL,NULL);
}

void CMainFrame::OnEditEdit() 
{
	m_dataView.OnEdit();
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

void CMainFrame::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	CFrameWnd::OnGetMinMaxInfo(lpMMI);
//	lpMMI->ptMaxSize.x=GetSystemMetrics(SM_CXSCREEN)+100;
//	lpMMI->ptMaxSize.y=GetSystemMetrics(SM_CYSCREEN)+100;
	lpMMI->ptMaxTrackSize.x=GetSystemMetrics(SM_CXSCREEN)+100;
	lpMMI->ptMaxTrackSize.y=GetSystemMetrics(SM_CYSCREEN)+100;
//	lpMMI->ptMaxPosition.x-=40;
//	lpMMI->ptMaxPosition.y-=40;
}

void CMainFrame::SetActiveViewTab(VIEWTYPE vt)
{
	switch (vt)
	{
	case VT_FREEFORM:
		m_viewTab.SetActiveTab(0);
		break;
	case VT_TOP:
		m_viewTab.SetActiveTab(1);
		break;
	case VT_PROFILE:
		m_viewTab.SetActiveTab(2);
		break;
	case VT_MIXED:
		m_viewTab.SetActiveTab(3);
		break;
	}
}



void CMainFrame::OnViewPreferences() 
{
	CAppSettings *Set=theApp.GetSettings();
	CPreferences prefs(Set->m_bStatusBar,Set->m_bToolBar,Set->m_bRuler,Set->m_bBottomTabs,Set->m_bCompass,Set->m_PreferedUnits,this);
	if (prefs.DoModal())
	{
		Set->m_bStatusBar=prefs.m_bStatusBar;
		Set->m_bToolBar=prefs.m_bToolBar;
		Set->m_PreferedUnits=prefs.m_PreferedUnits;
		Set->m_bRuler=prefs.m_bRuler;
		Set->m_bBottomTabs=prefs.m_bIndexTabs;
		Set->m_bCompass=prefs.m_bCompass;
		//Recalculate the layout for the main frame
		m_wndStatusBar.ShowWindow(Set->m_bStatusBar?SW_SHOW:SW_HIDE);
		m_wndToolBar.ShowWindow(Set->m_bToolBar?SW_SHOW:SW_HIDE);
		RecalcLayout();

		//And for our personal child frame
		m_wndRuler.ShowWindow(Set->m_bRuler?SW_SHOW:SW_HIDE);
		//If the units changed to or from metric, instant feedback
		//is nice.
		m_wndRuler.InvalidateRect(NULL,FALSE);
		COnStationView *View=(COnStationView *)GetActiveView();
		if (View!=NULL)
		{
			View->ShowCompass(Set->m_bCompass);
			//if the view changed to or from metric, we need to update our units in the status bar
			View->RefreshStatusIndicators(this);
		}
		m_viewTab.ShowWindow(Set->m_bBottomTabs?SW_SHOW:SW_HIDE);
		RecalcLayout();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

BOOL CMainFrame::OnQueryNewPalette() 
{
	COnStationView* pView = dynamic_cast<COnStationView *>(GetActiveView());
    if (pView)
	{
        // OnQueryNewPalette is not public, so send a message.
        return pView->OnQueryNewPalette();
    }
    return FALSE; 
}

void CMainFrame::OnPaletteChanged(CWnd* pFocusWnd) 
{
    if (pView_G) 
	{
        // OnPaletteChanged is not public, so send a message.
        pView_G->OnPaletteChanged(pFocusWnd);
	}
}



void CMainFrame::OnUpdateEditEdit(CCmdUI* pCmdUI) 
{
	m_dataView.OnUpdateEditEdit(pCmdUI);
}
void CMainFrame::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	m_dataView.OnUpdateEditCopy(pCmdUI);
}
void CMainFrame::OnUpdateEditDelete(CCmdUI* pCmdUI) 
{
	m_dataView.OnUpdateEditDelete(pCmdUI);
}

void CMainFrame::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsClipboardFormatAvailable(uiCustomClipboardFormat_G));
}

void CMainFrame::OnHelp()
{
	HKEY hKey;
	LONG lValue=RegOpenKeyEx(HKEY_CLASSES_ROOT, _T("htmlfile\\shell\\open\\command"),0,KEY_READ,&hKey);
	CString csBrowser;
	csBrowser= _T("");
	if (lValue == ERROR_SUCCESS)
	{
		DWORD dwType;
		DWORD dwDataSize=256;
		TCHAR byData[512];
		if (RegQueryValueEx(hKey, _T(""),NULL,&dwType,(LPBYTE)byData,&dwDataSize)==ERROR_SUCCESS)
		{
			csBrowser=byData;
		}
	}
	if (csBrowser[0] != 0)
	{
		TCHAR szBuffer[2048];
		//lstrcpyn(szBuffer, csBrowser, _countof(szBuffer)-1);
		//lstrcat(szBuffer, _T(" "));
		lstrcat(szBuffer, theApp.m_pszHelpFilePath);
		int iLen=lstrlen(szBuffer)-3;
		lstrcpy(szBuffer+iLen, _T("htm"));
		//WinExec(szBuffer,SW_SHOWNORMAL);
		ShellExecute(NULL, _T("open"), szBuffer, NULL, NULL, SW_SHOWNORMAL);
	}
	else
	{
		AfxMessageBox(IDS_HELPTHROUGHBROWSER);
	}
}

void CMainFrame::OnDebugStationlinks() 
{
#ifdef _DEBUG
	CDebugStationLinksDialog dlg;
	dlg.DoModal();
#endif
}

void CMainFrame::OnDrawingFont() 
{
	LOGFONT lf;
	memset((LPVOID)&lf,0,sizeof(lf));
	lf.lfHeight=theApp.GetSettings()->m_iFontSize;
	lf.lfWeight=theApp.GetSettings()->m_iFontWeight;
	lstrcpyn(lf.lfFaceName,theApp.GetSettings()->m_szFontName,32);
	CFontDialog fd(&lf,CF_SCREENFONTS,NULL,this);
	if (fd.DoModal()==IDOK)
	{
		fd.GetCurrentFont(&lf);
		theApp.GetSettings()->m_iFontWeight=lf.lfWeight;
		theApp.GetSettings()->m_iFontSize=lf.lfHeight;
		theApp.GetSettings()->m_szFontName=lf.lfFaceName;
		pView_G->RedrawView();
	}
}


void CMainFrame::OnErrorsloopsLoopclosureoptions() 
{
	AfxMessageBox(_T("Hello Pierre: You can put all of the options in a dialog here.  This is about line 554 of MainFrm.cpp"));
}
