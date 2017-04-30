// DebugStationLinksDialog.cpp : implementation file
//

#include "stdafx.h"
#ifdef _DEBUG
#include "onstation.h"
#include "DebugStationLinksDialog.h"
#include "nodetree.h"
#include "onstationdoc.h"

extern COnStationDoc * pDocument_G;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDebugStationLinksDialog dialog


CDebugStationLinksDialog::CDebugStationLinksDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDebugStationLinksDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDebugStationLinksDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDebugStationLinksDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDebugStationLinksDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDebugStationLinksDialog, CDialog)
	//{{AFX_MSG_MAP(CDebugStationLinksDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDebugStationLinksDialog message handlers

void CDebugStationLinksDialog::OnOK() 
{
	CString cs;
	GetDlgItemText(IDC_EDIT1,cs);
	CNodeTree * pNodeTree=pDocument_G->GetNodeTree();	
	CNode * pNode=pNodeTree->FindNode(cs);
	if (pNode!=NULL)
	{
		pNode->Dump();
	}
}
#endif
