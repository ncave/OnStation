// WaitForClosureDialog.cpp : implementation file
//

#include "stdafx.h"
#include "OnStation.h"
#include "WaitForClosureDialog.h"

#include "jpclose.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWaitForClosureDialog dialog


CWaitForClosureDialog::CWaitForClosureDialog(int iColorScheme,CSurveyShot ** ShotPtrArray,int iNumShots,CWnd* pParent /*=NULL*/)
	: CDialog(CWaitForClosureDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWaitForClosureDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_MyClose=new CClose(iColorScheme);
	m_iPhase=0;
	m_ShotArray=ShotPtrArray;
	m_iShotCount=iNumShots;
}

CWaitForClosureDialog::~CWaitForClosureDialog()
{
	delete m_MyClose;
	m_MyClose=NULL;
}
void CWaitForClosureDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWaitForClosureDialog)
	DDX_Control(pDX, IDC_PROGRESS2, m_ProgressControl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWaitForClosureDialog, CDialog)
	//{{AFX_MSG_MAP(CWaitForClosureDialog)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWaitForClosureDialog message handlers

BOOL CWaitForClosureDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_ProgressControl.SetStep(20);
	SetTimer(1,100,NULL);
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWaitForClosureDialog::OnTimer(UINT_PTR nIDEvent) 
{
	//We get timer every 100 milliseconds.  This allows us to pause for
	//cancels as well as allowing us to redraw the progress control
	static BOOL bReenter=FALSE;
	if (!bReenter)
	{
		bReenter=TRUE;
		DWORD dwEndCalc=GetTickCount()+250;
		if (!m_MyClose->surv_loop_close(m_ShotArray,m_iShotCount,m_iPhase))
		{
			AfxMessageBox(IDS_INTERNALERRORINLOOPCLOSURE);
			CDialog::OnCancel();
			KillTimer(1);
			bReenter=FALSE;
			return;
		}
		m_iPhase++;
		m_ProgressControl.StepIt();
		if (m_iPhase==5)
		{
			KillTimer(1);
			CDialog::OnOK();
		}
		bReenter=FALSE;
	}
}

void CWaitForClosureDialog::OnCancel() 
{
	KillTimer(1);
	CDialog::OnCancel();
}
