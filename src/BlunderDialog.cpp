// BlunderDialog.cpp : implementation file
//

#include "stdafx.h"
#include "onstation.h"
#include "BlunderDialog.h"
#include "onstationdoc.h"
#include "nodetree.h"
#include "surveyshot.h"
#include "legeditsheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static int m_iStandardDeviations={4};

extern COnStationDoc * pDocument_G;
/////////////////////////////////////////////////////////////////////////////
// CBlunderDialog dialog


CBlunderDialog::CBlunderDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CBlunderDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBlunderDialog)
	//}}AFX_DATA_INIT
	// m_iStandardDeviations=2;
	m_bDirty=FALSE;
}


void CBlunderDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBlunderDialog)
	DDX_Control(pDX, IDC_BLU_DISPLAY, m_Display_button);
	DDX_Control(pDX, IDC_DEVIATIONS, m_cbDeviations);
	DDX_Control(pDX, IDC_LIST1, m_shotList);
	DDX_Control(pDX, IDC_BLU_DEV, m_RadioDev);
	DDX_Control(pDX, IDC_BLU_MAX, m_RadioMax);
	DDX_Control(pDX, IDC_BLU_COM, m_RadioCom);
	DDX_Control(pDX, IDC_BLU_CLI, m_RadioCli);
	DDX_Control(pDX, IDC_BLU_TAP, m_RadioTap);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBlunderDialog, CDialog)
	//{{AFX_MSG_MAP(CBlunderDialog)
	ON_CBN_SELCHANGE(IDC_DEVIATIONS, OnSelchangeDeviations)
	ON_BN_CLICKED(IDC_BUTTON2, OnEditSurvey)
	ON_LBN_DBLCLK(IDC_LIST1, OnDblclkList1)
	ON_WM_RBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_BLU_DEV, OnBluDev)
	ON_BN_CLICKED(IDC_BLU_MAX, OnBluMax)
	ON_BN_CLICKED(IDC_BLU_COM, OnBluCom)
	ON_BN_CLICKED(IDC_BLU_CLI, OnBluCli)
	ON_BN_CLICKED(IDC_BLU_TAP, OnBluTap)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBlunderDialog message handlers

void CBlunderDialog::OnSelchangeDeviations() 
{
	// TODO: Add your control notification handler code here
	m_iStandardDeviations= m_cbDeviations.GetCurSel()+1;
	LoadListBox();
}

BOOL CBlunderDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_cbDeviations.SetCurSel(m_iStandardDeviations-1);
	CNodeTree * pNodes=pDocument_G->GetNodeTree();
	int iMode = pNodes->GetBlunderMode();
	switch(iMode){

		case 0:
		m_RadioDev.SetCheck(1);
		break;

		case 1:
		m_RadioMax.SetCheck(1);
		break;

		case 2:
		m_RadioCom.SetCheck(1);
		break;

		case 3:
		m_RadioCli.SetCheck(1);
		break;

		case 4:
		m_RadioTap.SetCheck(1);
		break;
	}

	LoadListBox();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CBlunderDialog::OnEditSurvey() 
{
	int iSel=m_shotList.GetCurSel();
	if (iSel!=LB_ERR)
	{
		CSurveyShot * pShot=(CSurveyShot *)m_shotList.GetItemData(iSel);
		if (pShot!=NULL)
		{
			CSurveyLeg * pLeg=pShot->GetLeg();
   			CEditSheet editdata(pLeg,IDS_SURVEYEDITSHEETTITLE,this,1);
			if (editdata.DoModal()==IDOK)
			{
				if (editdata.NeedRecalculation())
				{
					TRACE(_T("Setting Leg Dirty\n"));
					AfxMessageBox(IDS_BLUNDERLISTINVALID);
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
	}
}

void CBlunderDialog::LoadListBox()
{
	m_shotList.ResetContent();
	CNodeTree * pNodes=pDocument_G->GetNodeTree();
	pNodes->AddBlundersToListBox(&m_shotList,m_iStandardDeviations);
}

void CBlunderDialog::OnDblclkList1() 
{
	OnEditSurvey();	
}

void CBlunderDialog::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnRButtonDblClk(nFlags, point);
}

void CBlunderDialog::OnBluDev() 
{
	// TODO: Add your control notification handler code here
	CNodeTree * pNodes=pDocument_G->GetNodeTree();
	pNodes->SetBlunderMode(0);
	LoadListBox();
}

void CBlunderDialog::OnBluMax() 
{
	// TODO: Add your control notification handler code here
	CNodeTree * pNodes=pDocument_G->GetNodeTree();
	pNodes->SetBlunderMode(1);
	LoadListBox();
	
}

void CBlunderDialog::OnBluCom() 
{
	// TODO: Add your control notification handler code here
	CNodeTree * pNodes=pDocument_G->GetNodeTree();
	pNodes->SetBlunderMode(2);
	LoadListBox();
	
}

void CBlunderDialog::OnBluCli() 
{
	// TODO: Add your control notification handler code here
	CNodeTree * pNodes=pDocument_G->GetNodeTree();
	pNodes->SetBlunderMode(3);
	LoadListBox();
	
}

void CBlunderDialog::OnBluTap() 
{
	// TODO: Add your control notification handler code here
	CNodeTree * pNodes=pDocument_G->GetNodeTree();
	pNodes->SetBlunderMode(4);
	LoadListBox();
	
}
