// surfacesize.cpp : implementation file
//

#include "stdafx.h"
#include "onstation.h"
#include "surfacesizepage.h"
#include "surfacedata.h"
#include "settings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern COnStationApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSurfaceSizePage property page

IMPLEMENT_DYNCREATE(CSurfaceSizePage,CPropertyPage)

CSurfaceSizePage::CSurfaceSizePage() : CPropertyPage(CSurfaceSizePage::IDD)
{
	//{{AFX_DATA_INIT(CSurfaceSizePage)
	m_fGridNorth = 0.0f;
	m_iWest = 0;
	m_iSouth = 0;
	m_iHeight=0;
	m_iWidth=0;
	m_iSurfaceCellSize = 250;
	//}}AFX_DATA_INIT
	m_SurfaceData=NULL;
}
void CSurfaceSizePage::Initialize(CSurfaceData *data)
{
	m_SurfaceData=data;
	m_fGridNorth=data->GetGridNorth();
	m_iSurfaceCellSize=(int)data->GetSquareSize();
	if (m_iSurfaceCellSize<5)
	{
		m_iSurfaceCellSize=5;	//prevent divide by zeros
	}
	m_iWest=(int)data->GetLeft();
	m_iSouth=(int)data->GetBottom();
	m_iWidth=(int)(data->GetRight()-data->GetLeft());
	m_iHeight=(int)(data->GetTop()-data->GetBottom());
	m_iWidth=m_iWidth/m_iSurfaceCellSize;
	m_iHeight=m_iHeight/m_iSurfaceCellSize;
	m_bDirty=FALSE;
}

CSurfaceSizePage::~CSurfaceSizePage()
{
}

void CSurfaceSizePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSurfaceSizePage)
	DDX_Text(pDX, IDC_GRIDNORTH, m_fGridNorth);
	DDV_MinMaxFloat(pDX, m_fGridNorth, -150.f, 150.f);
	DDX_Text(pDX, IDC_LEFT, m_iWest);
	DDX_Text(pDX, IDC_BOTTOM, m_iSouth);
	DDX_Text(pDX, IDC_GRIDHEIGHT,m_iHeight);
	DDV_MinMaxInt(pDX,m_iHeight,1,500);
	DDX_Text(pDX, IDC_GRIDWIDTH,m_iWidth);
	DDV_MinMaxInt(pDX,m_iWidth,1,500);
	DDX_Text(pDX, IDC_SURFACECELLSIZE, m_iSurfaceCellSize);
	DDV_MinMaxUInt(pDX, m_iSurfaceCellSize, 5, 5000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSurfaceSizePage, CPropertyPage)
	//{{AFX_MSG_MAP(CSurfaceSizePage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



//Do data validation here
BOOL CSurfaceSizePage::OnKillActive()
{
	if (!UpdateData(TRUE))
		return FALSE;

	if (m_fGridNorth!=m_SurfaceData->GetGridNorth())
	{
		m_bDirty=TRUE;
	}

	BOOL bGridSizeChanged=(m_iSurfaceCellSize!=(unsigned int)m_SurfaceData->GetSquareSize());
	BOOL bOriginChanged=(m_iSouth!=(int)m_SurfaceData->GetBottom() || m_iWest!=(int)m_SurfaceData->GetLeft());
	BOOL bSurfaceSizeChanged=(m_iHeight!=(int)(m_SurfaceData->GetTop()-m_SurfaceData->GetBottom())/m_SurfaceData->GetSquareSize() || m_iWidth!=(int)(m_SurfaceData->GetRight()-m_SurfaceData->GetLeft())/m_SurfaceData->GetSquareSize());
	//Setting these to floats causes a code generation error.
	int iTop=(m_iSouth+m_iSurfaceCellSize*m_iHeight);
	int iEast=(m_iWest+m_iSurfaceCellSize*m_iWidth);
	if (!bSurfaceSizeChanged && !bOriginChanged && !bGridSizeChanged)
	{
		m_SurfaceData->SetGridNorth(m_fGridNorth);
		return TRUE;	//nothing changed.  No problem
	}
	else
	{
		m_bDirty=TRUE;
	}
	if (m_SurfaceData->IsEmpty() || AfxMessageBox(IDS_SURFACECHANGEDSUGGESTPASTE,MB_YESNO)==IDYES)
	{
		m_SurfaceData->SetDimensions((float)m_iWest,(float)iTop,(float)iEast,(float)(m_iSouth),(float)m_iSurfaceCellSize);
		m_SurfaceData->SetGridNorth(m_fGridNorth);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
