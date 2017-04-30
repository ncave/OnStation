// TitleBoxSetup.cpp : implementation file
//

#include "stdafx.h"
#include "onstation.h"
#include "TitleBoxSetup.h"
#include "realfolder.h"
#include "onstationdoc.h"
#include "nodetree.h"
#include "stdio.h"
#include "mainfrm.h"
#include "onstationview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTitleBoxSetup dialog
extern COnStationApp theApp;

CTitleBoxSetup::CTitleBoxSetup(COnStationView *View,CWnd* pParent /*=NULL*/)
	: CDialog(CTitleBoxSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTitleBoxSetup)
	m_szTitle = _T("");
	m_szComments = _T("");
	m_szLocation = _T("");
	m_szSurveyedBy = _T("");
	m_szLength = _T("");
	//}}AFX_DATA_INIT
	COnStationDoc *Document=View->GetDocument();
	m_MyView=View;
	CFolderData FData;
	CRealFolder *F=Document->GetSurveyFolder();
	F->GatherFolderData(&FData,0,FALSE,NULL);
	CNodeTreeSummary NTS;
	Document->GetNodeTree()->GetViewInfo(&NTS);
	TCHAR szBuffer[256];
	_stprintf_s(szBuffer, _T("Total Surveyed Length: %s   Total Surveyed Depth: %s"),GetPreferedUnitString(NTS.m_fCaveLength),GetPreferedUnitString(NTS.m_fHighest-NTS.m_fDeepest));
	m_szLength=szBuffer;
	CString szNames;
	szNames= _T("");
	if (FData.m_NameList.GetLowestYear()!=0)
	{
		for (int i=FData.m_NameList.GetLowestYear();i<=FData.m_NameList.GetHighestYear();i++)
		{
			if (FData.m_NameList.GetNumberOfNamesForYear(i)!=0)
			{
				_stprintf_s(szBuffer, _T("%i:\t"),i);
				szNames+=szBuffer;
			}
			for (int j=0;j<FData.m_NameList.GetNumberOfNamesForYear(i)-1;j++)
			{
				int iRefs;
				LPCTSTR szTempName=FData.m_NameList.GetName(i,j,&iRefs);
				if (iRefs>1)
				{
					_stprintf_s(szBuffer, _T("%s (%i), "),szTempName,iRefs);
				}
				else
				{
					_stprintf_s(szBuffer, _T("%s,"),szTempName,iRefs);
				}
				szNames+=szBuffer;
			}
			if (FData.m_NameList.GetNumberOfNamesForYear(i)!=0)
			{
				int iRefs;
				//And the final one without the comma but with a line feed
				LPCTSTR szTempName=FData.m_NameList.GetName(i,FData.m_NameList.GetNumberOfNamesForYear(i)-1,&iRefs);
				if (iRefs>1)
				{
					_stprintf_s(szBuffer, _T("%s (%i)\n"),szTempName,iRefs);
				}
				else
				{
				    _stprintf_s(szBuffer, _T("%s\n"),szTempName);
				}
				szNames+=szBuffer;
			}
		}
	}
	if (FData.m_NameList.GetNumberOfNamesForYear(0)!=0)
	{
		szNames+= _T("Unknown Year:\t");
		int j;
		for (j=0;j<FData.m_NameList.GetNumberOfNamesForYear(0)-1;j++)
		{
			int iRefs;
			LPCTSTR szTempName=FData.m_NameList.GetName(0,j,&iRefs);
			_stprintf_s(szBuffer, _T("%s (%i),"),szTempName,iRefs);
			szNames+=szBuffer;
		}
		//And the final one without the comma but with a line feed
		if (FData.m_NameList.GetNumberOfNamesForYear(0)!=0)
		{
			int iRefs;
			LPCTSTR szTempName=FData.m_NameList.GetName(0,j,&iRefs);
			_stprintf_s(szBuffer, _T("%s (%i)\n"),szTempName,iRefs);
			szNames+=szBuffer;
		}
	}
	m_szSurveyedBy=szNames;
}


void CTitleBoxSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTitleBoxSetup)
	DDX_Text(pDX, IDC_EDITTITLE, m_szTitle);
	DDV_MaxChars(pDX, m_szTitle, 128);
	DDX_Text(pDX, IDC_EDIT5, m_szComments);
	DDX_Text(pDX, IDC_EDIT2, m_szLocation);
	DDX_Text(pDX, IDC_EDIT3, m_szSurveyedBy);
	DDX_Text(pDX, IDC_EDIT4, m_szLength);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTitleBoxSetup, CDialog)
	//{{AFX_MSG_MAP(CTitleBoxSetup)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTitleBoxSetup message handlers
