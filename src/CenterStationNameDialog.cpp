// CenterStationNameDialog.cpp : implementation file
//

#include "stdafx.h"
#include "OnStation.h"
#include "CenterStationNameDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCenterStationNameDialog dialog


CCenterStationNameDialog::CCenterStationNameDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CCenterStationNameDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCenterStationNameDialog)
	m_szEditStation = _T("");
	//}}AFX_DATA_INIT
}


void CCenterStationNameDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCenterStationNameDialog)
	DDX_Text(pDX, IDC_EDITSTATION, m_szEditStation);
	DDV_MaxChars(pDX, m_szEditStation, 16);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCenterStationNameDialog, CDialog)
	//{{AFX_MSG_MAP(CCenterStationNameDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCenterStationNameDialog message handlers
