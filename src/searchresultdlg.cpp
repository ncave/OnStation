// srchres.cpp : implementation file
//

#include "stdafx.h"
#include "onstation.h"
#include "searchresultdlg.h"
#include "surveyshot.h"
#include "node.h"
#include "nodetree.h"
#include "mainfrm.h"
#include "onstationview.h"
#include "surveyleg.h"
#include "legeditsheet.h"
#include "surfacedata.h"
#include "editfixedpointdialog.h"
#include "onstationdoc.h"
#include "settings.h"
#include "fixedpoint.h"
#include "legQuery.h"
#include "overalldataview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern COnStationApp theApp;
extern CSettings * pSettings_G;
extern COnStationDoc * pDocument_G;

/////////////////////////////////////////////////////////////////////////////
// CSearchResultDialog dialog


CSearchResultDialog::CSearchResultDialog(COnStationView * view,CPtrArray* find,CWnd* pParent /*=NULL*/)
	: CDialog(CSearchResultDialog::IDD, pParent)
{

	//{{AFX_DATA_INIT(CSearchResultDialog)
	//}}AFX_DATA_INIT
	m_MyView=view;
	m_pPointArray=find;
	m_bDirty=FALSE;
}

void CSearchResultDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSearchResultDialog)
	DDX_Control(pDX, IDC_STATIONLIST, m_StationList);
	DDX_Control(pDX, IDC_STATIC_Z, m_szZ);
	DDX_Control(pDX, IDC_STATIC_Y, m_szY);
	DDX_Control(pDX, IDC_STATIC_X, m_szX);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSearchResultDialog, CDialog)
	//{{AFX_MSG_MAP(CSearchResultDialog)
	ON_BN_CLICKED(IDC_BUTTONCENTER, OnButtoncenter)
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_FIXTHISPOINT, OnFixthispoint)
	ON_BN_CLICKED(IDSHOWSURVEYS, OnShowsurveys)
	ON_BN_CLICKED(IDC_BUTTON1, OnOK)
	ON_LBN_DBLCLK(IDC_STATIONLIST, OnDblclkStationlist)
	ON_LBN_SELCHANGE(IDC_STATIONLIST, OnSelchangeStationlist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSearchResultDialog message handlers

BOOL CSearchResultDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_StationList.ResetContent();
	int iTabStops[]={300,500};
	m_StationList.SetTabStops(2,iTabStops);
	for (int i=0;i<m_pPointArray->GetSize();i++)
	{
		CNode *pNode=(CNode *)m_pPointArray->GetAt(i);
		CSurveyLeg * pLeg=pNode->GetLeg();
		float z=pNode->GetPosition(pSettings_G->m_bShowClosedLoops[pSettings_G->m_iColorSchemeIndex])->GetZ();
		CString CS;
		CS.Format(IDS_NAMETABDEPTH,pNode->GetName(),GetPreferedUnitString(z),pLeg->GetName());
		int iIndex=m_StationList.AddString(CS);
		m_StationList.SetItemData(iIndex,(LONG)pNode);
	}	
	m_StationList.SetCurSel(0);
	UpdateNodePosText();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

CNode * CSearchResultDialog::GetSelectedNode()
{
	int iSel=m_StationList.GetCurSel();
	if (iSel!=LB_ERR)
	{
		return (CNode *)m_StationList.GetItemData(iSel);
	}
	return NULL;
}

void CSearchResultDialog::OnButtoncenter()
{
	CNode * pNode=GetSelectedNode();
	if (pNode!=NULL)
	{
		CPosMatrix *Pos=pNode->GetPosition(pSettings_G->m_bShowClosedLoops[pSettings_G->m_iColorSchemeIndex]);
		if (Pos!=NULL && Pos->GetX()!=NAN && Pos->GetY()!=NAN && Pos->GetZ()!=NAN)
		{
			m_MyView->SetExactView(NAN,NAN,NAN,Pos->GetX(),Pos->GetY(),Pos->GetZ());
		}
	}
	else
	{
		MessageBeep(0);
	}
}

void CSearchResultDialog::OnOK()
{
	EndDialog(IDOK);
}


void CSearchResultDialog::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (lpDrawItemStruct->itemID!=LB_ERR)
	{
		CNode * pNode=(CNode *)lpDrawItemStruct->itemData;
		int iColorScheme=pSettings_G->m_iColorSchemeIndex;
		BOOL bConstrained=pDocument_G->GetSurface()->IsConstrainedNode(pNode->GetName(),iColorScheme);
		CSurveyLeg * pLeg=pNode->GetLeg();

		if (pLeg->GetDrawColor(0)!=RGB(255,255,255))
		{
			SetTextColor(lpDrawItemStruct->hDC,pLeg->GetDrawColor(0));
		}
		else
		{
			SetTextColor(lpDrawItemStruct->hDC,RGB(0,0,0));
		}
		if (lpDrawItemStruct->itemState&ODS_SELECTED)
		{
			SetBkMode(lpDrawItemStruct->hDC,OPAQUE);
			SetBkColor(lpDrawItemStruct->hDC,RGB(0,0,128));
			SetTextColor(lpDrawItemStruct->hDC,RGB(255,255,255));
		}
		else
		{
			SetBkColor(lpDrawItemStruct->hDC,RGB(255,255,255));
		}
		//draw a pin if this is a constrained node
		if (pNode->MatchesQuery() || bConstrained)
		{
			HBITMAP bitmap;
			if (pNode->MatchesQuery() && bConstrained)
			{
				bitmap=::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAPPINSEARCH));
			}
			else
			{
				bitmap=::LoadBitmap(AfxGetInstanceHandle(),pNode->MatchesQuery()?MAKEINTRESOURCE(IDB_BITMAPSEARCHFOUND):MAKEINTRESOURCE(IDB_BITMAPPIN));
			}
			HDC dc2=CreateCompatibleDC(lpDrawItemStruct->hDC);
			HBITMAP hOldBitmap=(HBITMAP)SelectObject(dc2,bitmap);
			BitBlt(lpDrawItemStruct->hDC,lpDrawItemStruct->rcItem.left,lpDrawItemStruct->rcItem.top,16,16,dc2,0,0,SRCCOPY);
			SelectObject(dc2,hOldBitmap);
			DeleteDC(dc2);
		}
		int iLen=lstrlen(pLeg->GetName());
		CString CStr;
		float z=pNode->GetPosition(pSettings_G->m_bShowClosedLoops[pSettings_G->m_iColorSchemeIndex])->GetZ();
		CStr.Format(IDS_NAMETABDEPTH,pNode->GetName(),GetPreferedUnitString(z));
		TabbedTextOut(lpDrawItemStruct->hDC,lpDrawItemStruct->rcItem.left+20,lpDrawItemStruct->rcItem.top,CStr,CStr.GetLength(),0,NULL,0);
	}	
	else	
	{
//		CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
	}
}


void CSearchResultDialog::UpdateNodePosText()
{
	int iColorScheme=pSettings_G->m_iColorSchemeIndex;
	CSurfaceData *Surface=pDocument_G->GetSurface();

	CPosMatrix *nodePos=NULL;
	//When we delete the shot we searched for we still get an update here but the
	//node is now invalid and we GPF.  Hence we do the m_Node!=NULL check
	CNode * pNode=GetSelectedNode();
	if (pNode!=NULL)	
	{
		nodePos=pNode->GetPosition(pSettings_G->m_bShowClosedLoops[pSettings_G->m_iColorSchemeIndex]);
	}
	if (nodePos!=NULL)
	{
		CString CS;
		CS.Format(IDS_XPOS,GetPreferedUnitString(Surface->ConvertXToUTM(nodePos->GetX())));
		m_szX.SetWindowText(CS);
		CS.Format(IDS_YPOS,GetPreferedUnitString(Surface->ConvertYToUTM(nodePos->GetY())));
		m_szY.SetWindowText(CS);
		CS.Format(IDS_ZPOS,GetPreferedUnitString(nodePos->GetZ()));
		m_szZ.SetWindowText(CS);
	}
	else
	{
		m_szX.SetWindowText(_T("?"));	//It has been edited, so we don't know the position any more
		m_szY.SetWindowText(_T("?"));
		m_szZ.SetWindowText(_T("?"));
	}
}

void CSearchResultDialog::OnFixthispoint() 
{
	CSurfaceData *Surface=pDocument_G->GetSurface();
	CNode * pNode=GetSelectedNode();
	if (pNode==NULL)
	{
		MessageBeep(0);
		return;
	}
	for (int i=0;i<Surface->GetFixedPointCount();i++)
	{
		CFixedPoint * T=Surface->GetFixedPoint(i);
		//Does it exist already?
		if (lstrcmpi(pNode->GetName(),T->GetName())==0)
		{
			CEditFixedPointDialog TI(pDocument_G,T);
			if (TI.DoModal()==IDOK)
			{
				m_bDirty=TRUE;
				pDocument_G->SetModifiedFlag(TRUE);
			}
			return;			
		}
	}		
	CFixedPoint *T=new CFixedPoint(pNode->GetName());
	CPosMatrix *Pos=pNode->GetPosition(pSettings_G->m_bShowClosedLoops[pSettings_G->m_iColorSchemeIndex]);
	if (Pos!=NULL)
	{
		T->SetAllCoordinates(Surface->ConvertXToUTM(Pos->GetX()),Surface->ConvertYToUTM(Pos->GetY()),Pos->GetZ());

		CEditFixedPointDialog TI(pDocument_G,T);
		if (TI.DoModal()==IDOK)
		{
			if (Surface->AddFixedPoint(T))
			{
				m_bDirty=TRUE;
				pDocument_G->SetModifiedFlag(TRUE);
			}
			else
			{
				AfxMessageBox(IDS_DUPLICATEFIXEDPOINT);
			}

		}
		else
		{
			delete T;
		}
	}
	GetDlgItem(IDC_STATIONLIST)->InvalidateRect(NULL,FALSE);
}

void CSearchResultDialog::OnShowsurveys() 
{
	CNode * pNode=GetSelectedNode();
	if (pNode!=NULL)
	{
		CString CS=pNode->GetName();
		CLegQuery * pLegQuery=new CLegQuery();
		pLegQuery->m_stationQuery.SetQueryText(CS);
		CMainFrame * pFrame=dynamic_cast<CMainFrame*>(AfxGetMainWnd());
		if (pFrame!=NULL)
		{
			pFrame->m_dataView.SetQuery(pLegQuery);
		}
		else
		{
			delete pLegQuery;
		}
	}
}

void CSearchResultDialog::OnDblclkStationlist() 
{
	OnShowsurveys();
}

void CSearchResultDialog::OnSelchangeStationlist() 
{
	UpdateNodePosText();
}
