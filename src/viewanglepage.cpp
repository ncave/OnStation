// viewdial.cpp : implementation file
//

#include "stdafx.h"
#include "onstation.h"
#include "viewanglepage.h"

#define ROTATE_SIZE_PELS	40	
#define UP_X_PELS			49
#define UP_Y_PELS			27


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewAnglePage dialog

IMPLEMENT_DYNCREATE(CViewAnglePage, CPropertyPage)

CViewAnglePage::CViewAnglePage():CPropertyPage(CViewAnglePage::IDD)
{
}

CViewAnglePage::CViewAnglePage(float fRot,float fElev)
	: CPropertyPage(CViewAnglePage::IDD)
{
	//{{AFX_DATA_INIT(CViewAnglePage)
	m_fRotate = fRot;
	m_fElevate = fElev;
	//}}AFX_DATA_INIT
}

void CViewAnglePage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewAnglePage)
	DDX_Text(pDX, IDC_EDITROTATE, m_fRotate);
	DDV_MinMaxFloat(pDX, m_fRotate, 0.f, 360.f);
	DDX_Text(pDX, IDC_EDITELEVATE, m_fElevate);
	DDV_MinMaxFloat(pDX, m_fElevate, 0.f, 360.f);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CViewAnglePage, CPropertyPage)
	//{{AFX_MSG_MAP(CViewAnglePage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CViewAnglePage message handlers


