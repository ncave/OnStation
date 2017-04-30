// OnStation.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "OnStation.h"

#include "MainFrm.h"
#include "OnStationDoc.h"
#include "OnStationView.h"
#include "settings.h"
#include "aboutsheet.h"
#include "gworldopengl.h"
#include "node.h"
#include "junctionbox.h"
#include "query.h"
#include "Splash.h"
#include "TipDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT uiCustomClipboardFormat_G=0;
extern CSettings * pSettings_G;
/////////////////////////////////////////////////////////////////////////////
// COnStationApp

BEGIN_MESSAGE_MAP(COnStationApp, CWinApp)
	//{{AFX_MSG_MAP(COnStationApp)
	ON_COMMAND(ID_ABOUT_ONSTATION, OnAboutOnstation)
	ON_COMMAND(ID_HELP_TIPOFTHEDAY, ShowTipOfTheDay)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COnStationApp construction
COnStationApp::COnStationApp()
{
#ifdef _DEBUG
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance

	//Some static verifications of routines.  Check that all systems are
	//working.
	CJunctionBox::Test();
	ASSERT(GetAngleBetween(10.0f,70.0f)==40.0f);
	ASSERT(GetAngleBetween(350.0f,30.0f)==10.0f);
	ASSERT(GetAngleBetween(180.0f,40.0f)==110.0f);
	ASSERT(GetAngleBetween(40.0f,280.0f)==340.0f);
	ASSERT(GetAngleBetween(280.0f,40.0f)==340.0f);
	ASSERT(GetAngleBetween(190.0f,30.0f)==110.0f);
	ASSERT(WithinNDegrees(10.0f,20.0f,15.0f));
	ASSERT(WithinNDegrees(355.0f,5.0f,15.0f));
	ASSERT(WithinNDegrees(5.0f,355.0f,15.0f));
	ASSERT(!WithinNDegrees(5.0f,355.0f,9.0f));
	ASSERT(WithinNDegrees(90.0f,110.0f,21.0f));
	ASSERT(WithinNDegrees(110.0f,90.0f,21.0f));
	ASSERT(!WithinNDegrees(110.0f,90.0f,19.0f));
	ASSERT(!WithinNDegrees(90.0f,110.0f,19.0f));
	CQuery Q;
	Q.Verify();
#endif
}

/////////////////////////////////////////////////////////////////////////////
// The one and only COnStationApp object

COnStationApp theApp;

/////////////////////////////////////////////////////////////////////////////
// COnStationApp initialization

BOOL COnStationApp::InitInstance()
{
	pSettings_G=new CSettings();
	// CG: This line was added by the OLE Control Containment component
	AfxEnableControlContainer();

	// CG: The following block was added by the Splash Screen component.
	{
		CCommandLineInfo cmdInfo;
		ParseCommandLine(cmdInfo);

		CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);
	}
	SetRegistryKey(_T("On Station Cave Mapping"));
	m_MySettings=new CAppSettings();
	m_MySettings->ReadFromIniFile();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

//#ifdef _AFXDLL
//	Enable3dControls();			// Call this when using MFC in a shared DLL
//#else
//	Enable3dControlsStatic();	// Call this when linking to MFC statically
//#endif

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	uiCustomClipboardFormat_G=RegisterClipboardFormat(_T("OnStationCustomFormat"));
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(COnStationDoc),
		RUNTIME_CLASS(CMainFrame),
		RUNTIME_CLASS(COnStationView));
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
//	CMainFrame* pMainFrame = new CMainFrame;
//	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
//		return FALSE;
//	m_pMainWnd = pMainFrame;

	//Make us nice and Win95 compliant
	EnableShellOpen();
	RegisterShellFileTypes();

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
//	pMainFrame->ShowWindow(m_nCmdShow);
//	pMainFrame->UpdateWindow();

	// CG: This line inserted by 'Tip of the Day' component.
	Sleep(2500);
	ShowTipAtStartup();
	return TRUE;
}

int COnStationApp::ExitInstance()
{
	m_MySettings->WriteToIniFile();
	delete m_MySettings;
	m_MySettings=NULL;
	delete pSettings_G;
	pSettings_G=NULL;
	return CWinApp::ExitInstance();
}



void COnStationApp::OnAboutOnstation() 
{
	CAboutSheet aboutDlg(IDS_ABOUTONSTATION);
	aboutDlg.DoModal();
}


void COnStationApp::ShowTipAtStartup(void)
{
	// CG: This function added by 'Tip of the Day' component.

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	if (cmdInfo.m_bShowSplash)
	{
		CTipDialog dlg;
		if (dlg.m_bStartup)
			dlg.DoModal();
	}

}

void COnStationApp::ShowTipOfTheDay(void)
{
	// CG: This function added by 'Tip of the Day' component.

	CTipDialog dlg;
	dlg.DoModal();

}

BOOL COnStationApp::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following line was added by the Splash Screen component.
	CSplashWnd::PreTranslateAppMessage(pMsg);

	return CWinApp::PreTranslateMessage(pMsg);
}


