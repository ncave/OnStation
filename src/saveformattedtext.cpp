// saveformattedtext.cpp : implementation file
//

#include "stdafx.h"
#include "onstation.h"
#include "saveformattedtext.h"
#include "filedump.h"
#include "onstationdoc.h"
#include "treehelper.h"
#include "realfolder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern COnStationApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSaveFormattedText dialog

CSaveFormattedText::~CSaveFormattedText()
{
}
CSaveFormattedText::CSaveFormattedText(COnStationDoc *document,CWnd* pParent /*=NULL*/)
	: CDialog(CSaveFormattedText::IDD, pParent)
{
	m_MyDocument=document;
	m_ImageList.Create(16,15,FALSE,2,0);
	CBitmap hBmp;
	hBmp.LoadBitmap(IDB_BMFOLDERS);
	m_idxFolder=m_ImageList.Add(&hBmp,(COLORREF)0);
	CBitmap hBmp2;
	hBmp2.LoadBitmap(IDB_BMSURVEYS);
	m_idxSurvey=m_ImageList.Add(&hBmp2,(COLORREF)0);
}


void CSaveFormattedText::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSaveFormattedText)
	DDX_Control(pDX, IDC_SURVEYLIST, m_SurveyList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSaveFormattedText, CDialog)
	//{{AFX_MSG_MAP(CSaveFormattedText)
	ON_NOTIFY(TVN_GETDISPINFO, IDC_SURVEYLIST, OnGetdispinfoSurveylist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSaveFormattedText message handlers

BOOL CSaveFormattedText::OnInitDialog() 
{
	CDialog::OnInitDialog();
	TreeHelperSetImageList(&m_SurveyList);
	CRealFolder *folder=m_MyDocument->GetSurveyFolder();
	RecursiveTreeViewFill(&m_SurveyList,(CFolder *)folder,TVI_ROOT,NULL);

	CComboBox * pBox=((CComboBox *)GetDlgItem(IDC_COMBO1));
	pBox->ResetContent();
	CString CS;
	CS.LoadString(IDS_FT_SURVEYDATA);
	pBox->AddString(CS);
	CS.LoadString(IDS_FT_RAWCOORDINATES);
	pBox->AddString(CS);
	pBox->SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CSaveFormattedText::OnOK() 
{
	BOOL bCoordinateSets=((CComboBox *)GetDlgItem(IDC_COMBO1))->GetCurSel()==1;
	CFolder *folder=NULL;
	TV_ITEM Item;
	
	HTREEITEM hItem=m_SurveyList.GetSelectedItem();
	if (hItem!=NULL)
	{
		Item.hItem=hItem;
		m_SurveyList.GetItem(&Item);
		folder=(CFolder *)Item.lParam;
	}
	if (folder==NULL)
	{
	    AfxMessageBox(IDS_NOSURVEYORFOLDERSELECTED);
	}
	else
	{
		try
		{
			CFileDumpHelper FDH;
			if (FDH.BeginDump())
			{
				PrintGuts(folder,&FDH,bCoordinateSets);
				FDH.EndDump();
				CDialog::OnOK();
			}
		}
		catch (CException *)
		{
			AfxMessageBox(IDS_SAVEFORMATTEDTEXTFAILEDUNKNOWN);
		}
	}
}

void CSaveFormattedText::PrintGuts(CFolder *folder,CFileDumpHelper *FDH,BOOL bCoordinateSets)
{
	folder->SaveFormattedText(FDH,0,FALSE,bCoordinateSets);
	CFolderData FD;
	if (!bCoordinateSets)
	{
		folder->GatherFolderData(&FD,0,FALSE,NULL);
		CString CS;
		if (folder->IsFolder())
		{
			CS.Format(IDS_FOLDERTOTALS,(LPCTSTR)folder->GetName());
		}
		else
		{
			CS.Format(IDS_SURVEYTOTALS,(LPCTSTR)folder->GetName());
		}
		FDH->OutputText(CS);
		if (FD.iNumCaveShots==0 && FD.iNumSurfaceShots==0)
		{
			CS.LoadString(IDS_CONTAINSNOSHOTS);
			FDH->OutputText(CS);
		}
		else
		{
			CString CS1;
			CString CS2;
			CS.Format(IDS_SUMMARYHEADER,FD.iNumSurfaceShots,FD.iNumCaveShots);
			FDH->OutputText(CS);
			FloatToString(CS1,FD.fTotalShotLengthSurface);
			FloatToString(CS2,FD.fTotalShotLengthCave);
			CS.Format(IDS_LENGTHTOTAL,CS1,CS2);
			FDH->OutputText(CS);
		}
	}
}

void CSaveFormattedText::OnCancel() 
{
	CDialog::OnOK();
}

void CSaveFormattedText::OnGetdispinfoSurveylist(NMHDR* pNMHDR, LRESULT* pResult) 
{
//The class wizard in 4.0 generates incorrect code here.
//	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	TV_DISPINFO *FuckMS=(TV_DISPINFO *)pNMHDR;

	TreeHelperGetDisplayInfo(FuckMS,-1);	
	
	*pResult = 0;
}

