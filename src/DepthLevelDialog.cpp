// DepthLevelDialog.cpp : implementation file
//

#include "stdafx.h"
#include "onstation.h"
#include "DepthLevelDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDepthLevelDialog dialog


CDepthLevelDialog::CDepthLevelDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDepthLevelDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDepthLevelDialog)
	m_DepthLevel = 0;
	//}}AFX_DATA_INIT
}


void CDepthLevelDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDepthLevelDialog)
	DDX_Text(pDX, IDC_EDIT2, m_DepthLevel);
	DDV_MinMaxInt(pDX, m_DepthLevel, 2, 15);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDepthLevelDialog, CDialog)
	//{{AFX_MSG_MAP(CDepthLevelDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDepthLevelDialog message handlers
