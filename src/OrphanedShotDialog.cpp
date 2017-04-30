// OrphanedShotDialog.cpp : implementation file
//

#include "stdafx.h"
#include "OnStation.h"
#include "OrphanedShotDialog.h"
#include "onstationdoc.h"
#include "surfacedata.h"
#include "fixedpoint.h"
#include "legeditsheet.h"
#include "node.h"
#include "nodetree.h"
#include "editfixedpointdialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COrphanedShotDialog dialog


COrphanedShotDialog::COrphanedShotDialog(COnStationDoc *document,int iColorScheme,CWnd* pParent)
	: CDialog(COrphanedShotDialog::IDD, pParent)
{
	m_MyDocument=document;
	m_iColorScheme=iColorScheme;
	m_bDirty=FALSE;
	//{{AFX_DATA_INIT(COrphanedShotDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void COrphanedShotDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COrphanedShotDialog)
	DDX_Control(pDX, IDC_LIST1, m_OrphanList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COrphanedShotDialog, CDialog)
	//{{AFX_MSG_MAP(COrphanedShotDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnSurfaceTieIn)
	ON_BN_CLICKED(IDC_BUTTON2, OnEditSurvey)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COrphanedShotDialog message handlers
void COrphanedShotDialog::OnSurfaceTieIn() 
{
	CNode *node;
	int iSel=m_OrphanList.GetCurSel();
	if (iSel==-1)
		return;
	node=(CNode *)m_OrphanList.GetItemData(iSel);
	if (node==NULL)
		return;

	CSurfaceData *Surface=m_MyDocument->GetSurface();
	for (int i=0;i<Surface->GetFixedPointCount();i++)
	{
		CFixedPoint * T=Surface->GetFixedPoint(i);
		//Does it exist already?
		if (lstrcmpi(node->GetName(),T->GetName())==0)
		{
			CEditFixedPointDialog TI(m_MyDocument,T);
			if (TI.DoModal()==IDOK)
			{
				m_bDirty=TRUE;
			}
			return;			
		}
	}		
	CFixedPoint *T=new CFixedPoint(node->GetName());
	CEditFixedPointDialog TI(m_MyDocument,T);
	if (TI.DoModal()==IDOK)
	{
		Surface->AddFixedPoint(T);
		m_bDirty=TRUE;
	}
	else
	{
		delete T;
	}
}

void COrphanedShotDialog::OnEditSurvey() 
{
	CNode *node;
	int iSel=m_OrphanList.GetCurSel();
	if (iSel==-1)
		return;
	node=(CNode *)m_OrphanList.GetItemData(iSel);
	if (node==NULL)
		return;

	CSurveyLeg *leg=node->GetLeg();
   	CEditSheet editdata(leg,IDS_SURVEYEDITSHEETTITLE,this,1);
	TRACE(_T("Editing COlor Scheme in Search Dialog\n"));
	if (editdata.DoModal()==IDOK)
	{
		if (editdata.NeedRecalculation())
		{
			TRACE(_T("Setting Leg Dirty\n"));
			AfxMessageBox(IDS_ORPHANEDLISTINVALID);
			EndDialog(IDOK);
			m_bDirty=TRUE;
			return;
		}
		else
		{
			TRACE(_T("NOT Setting Leg Dirty\n"));
		}
	}
}

BOOL COrphanedShotDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CNodeTree *NT=m_MyDocument->GetNodeTree();

	for (int i=0;i<NT->GetNumberOfMissingSections();i++)
	{
		CNode *node=NT->GetMissingSectionNode(i);
		if (node!=NULL)
		{
			CString CS;
			CS.Format(IDS_ORPHANFORMAT,node->GetName(),node->GetLeg()->GetName());
			int iIndex=m_OrphanList.AddString(CS);
			m_OrphanList.SetItemData(iIndex,(LONG)node);
		}
	}
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
