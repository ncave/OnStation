// surfaceeditpage.cpp : implementation file
//

#include "stdafx.h"
#include "onstation.h"
#include "surfaceeditpage.h"
#include "surfacedata.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define GRIDCONTROLID	731


/////////////////////////////////////////////////////////////////////////////
// CSurfaceEditPage property page

IMPLEMENT_DYNCREATE(CSurfaceEditPage, CPropertyPage)
BEGIN_MESSAGE_MAP(CSurfaceEditPage, CPropertyPage)
	//{{AFX_MSG_MAP(CSurfaceEditPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#define new DEBUG_NEW


CSurfaceEditPage::CSurfaceEditPage() : CPropertyPage(CSurfaceEditPage::IDD)
{
	//{{AFX_DATA_INIT(CSurfaceEditPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_SurfaceData=NULL;
	m_bDirty=FALSE;
}

void CSurfaceEditPage::Initialize(CSurfaceData * data)
{
	m_SurfaceData=data;
}

CSurfaceEditPage::~CSurfaceEditPage()
{
}

void CSurfaceEditPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSurfaceEditPage)
	DDX_GridControl(pDX, IDC_SURFACEALTITUDEGRID, m_myGridControl);
	//}}AFX_DATA_MAP
}


BOOL CSurfaceEditPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CSurfaceEditPage::OnKillActive()
{
	if (m_myGridControl.m_hWnd==NULL)
	{
		return CPropertyPage::OnKillActive();
	}
	m_bDirty=m_myGridControl.GetModified();
	for (int i=0;i<m_SurfaceData->GetGridHeight();i++)
	{
		for (int j=0;j<m_SurfaceData->GetGridWidth();j++)
		{
			//m_myGridControl.SetRow(i+1);
			//m_myGridControl.SetCol(j+1);
			CString CS=m_myGridControl.GetItemText(i+1, j+1);
			float f;
			if (!FloatFromString(CS,f))
			{
				//m_myGridControl.SetSelStartCol(j+1);
				//m_myGridControl.SetSelEndCol(j+1);
				//m_myGridControl.SetSelStartRow(i+1);
				//m_myGridControl.SetSelEndRow(i+1);
				m_myGridControl.SetSelectedRange(i+1, j+1, i+1, j+1);
				AfxMessageBox(IDS_ENTERVALIDNUMBER);
				m_myGridControl.SetFocus();
				return FALSE;
			}
			m_SurfaceData->SetAltitude(j,i,f);
		}
	}
	return CPropertyPage::OnKillActive();
}

BOOL CSurfaceEditPage::OnSetActive()
{
	if (m_myGridControl.m_hWnd==NULL)
	{
		AfxMessageBox(IDS_NOGRIDCONTROL);
		return FALSE;
	}
	m_myGridControl.SetRowCount(m_SurfaceData->GetGridHeight()+1);
	m_myGridControl.SetColumnCount(m_SurfaceData->GetGridWidth()+1);
	m_myGridControl.SetFixedRowCount(1);
	m_myGridControl.SetFixedColumnCount(1);

	CString cs;
	for (int i=0;i<m_SurfaceData->GetGridHeight();i++)
	{
		//m_myGridControl.SetRow(i+1);
		//m_myGridControl.SetCol(0);
		float f=m_SurfaceData->GetTop()-m_SurfaceData->GetSquareSize()*i;
		FloatToString(cs,f);
		m_myGridControl.SetItemText(i+1, 0, cs);
		for (int j=0;j<m_SurfaceData->GetGridWidth();j++)
		{
			FloatToString(cs,m_SurfaceData->GetAltitude(j,i));
			//m_myGridControl.SetRow(i+1);
			//m_myGridControl.SetCol(j+1);
			m_myGridControl.SetItemText(i+1, j+1, cs);
		}
	}
	for (int j=0;j<m_SurfaceData->GetGridWidth();j++)
	{
		//m_myGridControl.SetRow(0);
		//m_myGridControl.SetCol(j+1);
		float f=m_SurfaceData->GetLeft()+m_SurfaceData->GetSquareSize()*j;
		FloatToString(cs,f);
		m_myGridControl.SetItemText(0, j+1, cs);
	}
	//m_myGridControl.SetCol(1);
	//m_myGridControl.SetRow(1);
	m_myGridControl.AutoSizeColumns();
	m_myGridControl.SetFocusCell(1,1);
	return TRUE;
}
