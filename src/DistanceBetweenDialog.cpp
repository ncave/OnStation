// DistanceBetweenDialog.cpp : implementation file
//

#include "stdafx.h"
#include "onstation.h"
#include "DistanceBetweenDialog.h"
#include "onstationdoc.h"
#include "node.h"
#include "nodetree.h"
#include "onstationview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern COnStationDoc * pDocument_G;
extern COnStationView * pView_G;
extern CSettings * pSettings_G;
/////////////////////////////////////////////////////////////////////////////
// CDistanceBetweenDialog dialog


CDistanceBetweenDialog::CDistanceBetweenDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDistanceBetweenDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDistanceBetweenDialog)
	m_szStartingStation = _T("");
	m_szEndingStation = _T("");
	//}}AFX_DATA_INIT
}


void CDistanceBetweenDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDistanceBetweenDialog)
	DDX_Text(pDX, IDC_EDIT1, m_szStartingStation);
	DDX_Text(pDX, IDC_EDIT2, m_szEndingStation);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDistanceBetweenDialog, CDialog)
	//{{AFX_MSG_MAP(CDistanceBetweenDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDistanceBetweenDialog message handlers

void CDistanceBetweenDialog::OnOK() 
{
	UpdateData();

	CNode * pFrom=pDocument_G->GetNodeTree()->FindNode(m_szStartingStation);
	if (pFrom==NULL)
	{
		AfxMessageBox(IDS_ENTERVALIDSTATIONNAME);
		GetDlgItem(IDC_EDIT1)->SetFocus();
		return;
	}
	CNode *pTo=pDocument_G->GetNodeTree()->FindNode(m_szEndingStation);
	if (pTo==NULL)
	{
		AfxMessageBox(IDS_ENTERVALIDSTATIONNAME);
		GetDlgItem(IDC_EDIT2)->SetFocus();
		return;
	}
	BOOL bClosed=pSettings_G->m_bShowClosedLoops[pSettings_G->m_iColorSchemeIndex];
	CPosMatrix * p1=pFrom->GetPosition(bClosed);
	CPosMatrix * p2=pTo->GetPosition(bClosed);
	double fDx=p2->GetX()-p1->GetX();
	double fDy=p2->GetY()-p1->GetY();
	double fDz=p2->GetZ()-p1->GetZ();
	double fDxy=sqrt(fDx*fDx+fDy*fDy);
	double fDxyz=sqrt(fDx*fDx+fDy*fDy+fDz*fDz);
	double fCompass=asin(fDx/fDxy);
	double fClino=asin(fDz/fDxyz);
	fClino=fClino*180.0/PI;
	fCompass=fCompass*180.0/PI;

	CString cs;
	CString csDirect1;
	CString csDirect2;
	CString csDirect3;
	if (fDx>0)
	{
		csDirect1.LoadString(IDS_EAST);
	}
	else
	{
		fDx=fDx*-1.0f;
		csDirect1.LoadString(IDS_WEST);
	}
	if (fDy>0)
	{
		csDirect2.LoadString(IDS_NORTH);
	}
	else
	{
		fDy=fDy*-1.0f;
		csDirect2.LoadString(IDS_SOUTH);
	}
	if (fDz>0)
	{
		csDirect3.LoadString(IDS_UP);
	}
	else
	{
		fDz=fDz*-1.0f;
		csDirect3.LoadString(IDS_DOWN);
	}
	cs.GetBuffer(512);
	cs.Format(IDS_DISTANCERESULT,
		m_szStartingStation,m_szEndingStation,
		(float)fDx,csDirect1,(float)fDy,csDirect2,(float)fDz,csDirect3,
		(float)fDxy,(float)fClino,(float)fCompass,(float)fDxyz);
	AfxMessageBox(cs);
}
