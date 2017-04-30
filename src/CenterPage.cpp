// CenterPage.cpp : implementation file
//

#include "stdafx.h"
#include "OnStation.h"
#include "CenterPage.h"
#include "centerstationnamedialog.h"
#include "node.h"
#include "onstationview.h"
#include "onstationdoc.h"
#include "nodetree.h"
#include "mainfrm.h"
#include "settings.h"
#include "surfacedata.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern CSettings *pSettings_G;
extern COnStationApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CCenterPage property page

IMPLEMENT_DYNCREATE(CCenterPage, CPropertyPage)

CCenterPage::CCenterPage(): CPropertyPage(CCenterPage::IDD)
{
	m_MyView=NULL;
};

CCenterPage::CCenterPage(float fXpos,float fYpos,float fZpos) : CPropertyPage(CCenterPage::IDD)
{
	//{{AFX_DATA_INIT(CCenterPage)
	m_fXpos = 0.0f;
	m_fYpos = 0.0f;
	m_fZpos = 0.0f;
	//}}AFX_DATA_INIT
	m_fMetricX=fXpos;
	m_fMetricY=fYpos;
	m_fMetricZ=fZpos;
	if (theApp.GetSettings()->IsMetric())
	{
		m_fXpos = fXpos;
		m_fYpos = fYpos;
		m_fZpos = fZpos;
		m_StaticX = _T("&X pos (m):");
		m_StaticY = _T("&Y pos (m):");
		m_StaticZ = _T("&Z pos (m):");
	}
	else
	{
		m_fXpos = MetersToFeet(fXpos);
		m_fYpos = MetersToFeet(fYpos);
		m_fZpos = MetersToFeet(fZpos);
		m_StaticX = _T("&X pos (ft):");
		m_StaticY = _T("&Y pos (ft):");
		m_StaticZ = _T("&Z pos (ft):");
	}
	m_MyView=NULL;
}

void CCenterPage::Initialize(COnStationView * view)
{
	m_MyView=view;
}

CCenterPage::~CCenterPage()
{
}

void CCenterPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCenterPage)
	DDX_Text(pDX, IDC_STATICX, m_StaticX);
	DDX_Text(pDX, IDC_STATICY, m_StaticY);
	DDX_Text(pDX, IDC_STATICZ, m_StaticZ);
	DDX_Text(pDX, IDC_XPAN, m_fXpos);
	DDX_Text(pDX, IDC_YPAN, m_fYpos);
	DDX_Text(pDX, IDC_ZPAN, m_fZpos);
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate)
	{
		if (!theApp.GetSettings()->IsMetric())
		{
			m_fMetricX=FeetToMeters(m_fXpos);
			m_fMetricY=FeetToMeters(m_fYpos);
			m_fMetricZ=FeetToMeters(m_fZpos);
		}
		else
		{
			m_fMetricX=m_fXpos;
			m_fMetricY=m_fYpos;
			m_fMetricZ=m_fZpos;
		}
	}
}


BEGIN_MESSAGE_MAP(CCenterPage, CPropertyPage)
	//{{AFX_MSG_MAP(CCenterPage)
	ON_BN_CLICKED(IDC_BUTTON1, OnSetCenterStation)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCenterPage message handlers

void CCenterPage::OnSetCenterStation() 
{
	CCenterStationNameDialog CSND;
	if (CSND.DoModal()==IDOK)
	{
		CNode * Node=m_MyView->GetDocument()->GetNodeTree()->FindNode(CSND.m_szEditStation);
		if (Node!=NULL)
		{
			CPosMatrix *Pos=Node->GetPosition(pSettings_G->m_bShowClosedLoops[pSettings_G->m_iColorSchemeIndex]);

			if (Pos!=NULL && Pos->GetX()!=NAN && Pos->GetY()!=NAN && Pos->GetZ()!=NAN)
			{
				CSurfaceData * Surf=m_MyView->GetDocument()->GetSurface();
				m_fXpos=Surf->ConvertXToUTM(Pos->GetX());
				m_fYpos=Surf->ConvertYToUTM(Pos->GetY());
				m_fZpos=Pos->GetZ();
				if (!theApp.GetSettings()->IsMetric())
				{
					m_fXpos = MetersToFeet(m_fXpos);
					m_fYpos = MetersToFeet(m_fYpos);
					m_fZpos = MetersToFeet(m_fZpos);
				}
				UpdateData(FALSE);	//refresh all the edit fields.
			}
		}
		else
		{
			AfxMessageBox(IDS_INVALIDSTATIONNAME);
		}
	}
}
