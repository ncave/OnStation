#include "stdafx.h"
#include "folder.h"
#include "realfolder.h"
#include "filehelper.h"
#include "surveyleg.h"
#include "legquery.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CRealFolder::CRealFolder(COnStationDoc *doc):CFolder(doc)
{
	m_bExpanded=TRUE;
};

CRealFolder::~CRealFolder()
{
	DeleteContents();
};


void CRealFolder::SaveFormattedText(CFileDumpHelper *PDH,int iColorScheme,BOOL bClosedLoops,BOOL bCoordinatesOnly)
{
    GotoTop();
    CFolder *data=GetCurrent();
    while (data!=NULL)
    {
		data->SaveFormattedText(PDH,iColorScheme,bClosedLoops,bCoordinatesOnly);
		data=GetNext();
    }
}


void CRealFolder::Read(CFileHelper *h)
{
	h->ReadTokenBegin(_T("Folder"));
	SetName(h->ReadTokenString(_T("FolderName")));
	while (TRUE)
	{
		LPCTSTR szData=h->GetTokenLookaheadString(TRUE);
		if (lstrcmpi(szData, _T("End"))==0)
		{
			h->ReadTokenEnd(_T("Folder"));
			return;
		}
		LPCTSTR szBeginType=h->ReadTokenString(_T("Begin"));
		if (lstrcmpi(szBeginType, _T("Folder"))==0)
		{
			h->UndoLastRead();
			CRealFolder *RF=new CRealFolder(GetDocument());
			RF->Read(h);
			RF->SetParentFolder(this,NULL);
		}
		else
		{
			if (lstrcmpi(szBeginType, _T("Survey"))==0)
			{
				h->UndoLastRead();
				CSurveyLeg *Leg=new CSurveyLeg(GetDocument());
				Leg->Read(h);
				Leg->SetParentFolder(this,NULL);
				Leg->GetShotArray()->DoDeferredOffsetCalculation();
			}
			else
			{
				h->UndoLastRead();
				h->ReadTokenBegin(_T("SurveyLeg"));	//cause an error to happen
			}
		}
	}
}


void CRealFolder::Write(CFileHelper * h)
{
	h->WriteToken(_T("Begin"), _T("Folder"));
	h->WriteToken(_T("FolderName"),GetName());
	GotoTop();
    CFolder *data=GetCurrent();
    while (data!=NULL)
    {
		if (data->IsFolder())
		{
			((CRealFolder*)data)->Write(h);
		}
		else
		{
			((CSurveyLeg *)data)->Write(h);
		}
		data=GetNext();
    }
	h->WriteToken(_T("End"), _T("Folder"));
}

#ifdef _DEBUG
void CRealFolder::Dump()
{
	TRACE(_T("Dumping folder %s\n"),GetName());
	GotoTop();
	CFolder *data=GetCurrent();
	while (data!=NULL)
	{
		TRACE(_T("  %s\n"),data->GetName());
		data=GetNext();
 	}
}
#endif

void CRealFolder::Add(CFolder *pData,CFolder * pInsertAfter)
{
#ifdef _DEBUG
	Dump();
#endif
    ASSERT(m_folderList.Find(pData,NULL)==NULL);		//make sure we aren't reputting it.
	if (pInsertAfter==NULL)
	{
		m_folderList.AddTail(pData);
	}
	else
	{
		GotoTop();
		CFolder *data=GetCurrent();
		while (data!=NULL && data!=pInsertAfter)
		{
			data=GetNext();
 		}
		if (m_MyPos==NULL)
		{
			m_folderList.AddTail(pData);
		}
		else
		{
			TRACE(_T("Inserting item %s after %s\n"),pData->GetName(),data->GetName());
			m_folderList.InsertAfter(m_MyPos,pData);
		}
	}
#ifdef _DEBUG	
	Dump();
#endif
}

void CRealFolder::GotoTop()
{
    m_MyPos=m_folderList.GetHeadPosition();
}

CFolder * CRealFolder::GetCurrent()
{
    if (m_MyPos==NULL)
	{
        return NULL;
	}
    else
	{
		return (m_folderList.GetAt(m_MyPos));
	}
}


BOOL CRealFolder::DoesAnythingMatchTheQuery(CLegQuery * pQuery)
{
    ASSERT(pQuery!=NULL);
	GotoTop();
    CFolder *data=GetCurrent();
    while (data!=NULL)
    {
		if (data->DoesAnythingMatchTheQuery(pQuery))
		{
			return TRUE;
		}
		data=GetNext();
    }
	return FALSE;
}

//Recursively set the color for all contained folders
COLORREF CRealFolder::SetColor(COLORREF crColor,int iColorSchemeIndex,BOOL bRainbow,BOOL *bDisabled,CLegQuery * pLegQuery)
{
    GotoTop();
    CFolder *data=GetCurrent();
    while (data!=NULL)
    {
		CSurveyLeg * pLeg=dynamic_cast<CSurveyLeg *>(data);
		if (pLegQuery==NULL || pLeg==NULL || pLegQuery->MatchesQuery(pLeg))
		{
			crColor=data->SetColor(crColor,iColorSchemeIndex,bRainbow,bDisabled,pLegQuery);
		}
		data=GetNext();
    }
	return crColor;
}

//Note.  Unlike that standard List GetNext() function, this
//function actually increments the iterator and the gets the
//element.  This allows us to use the interator on a search.
CFolder * CRealFolder::GetNext()
{
    if (m_MyPos==NULL)
        return NULL;

    m_folderList.GetNext(m_MyPos);
   	return GetCurrent();
}



INT_PTR CRealFolder::GetSize()
{
    return m_folderList.GetCount();
}

CRealFolder * CRealFolder::FindSubFolderByName(LPCTSTR szName)
{
    GotoTop();
    CFolder *data=GetCurrent();
    while (data!=NULL)
    {
		if (data->IsFolder() && lstrcmpi(szName,data->GetName())==0)
		    return (CRealFolder *)data;
		data=GetNext();
    }
    return NULL;
}

CSurveyLeg * CRealFolder::FindSurveyByName(LPCTSTR szName)
{
    GotoTop();
    CFolder *data=GetCurrent();
    while (data!=NULL)
    {
		if (data->IsFolder()==FALSE && lstrcmpi(szName,data->GetName())==0)
		    return (CSurveyLeg *)data;
		data=GetNext();
    }
    return NULL;
}

BOOL CRealFolder::IsColorSchemeDataSimilar(int iOne,int iTwo)
{
	GotoTop();
	CFolder *data=GetCurrent();
    while (data!=NULL)
    {
		if (!data->IsColorSchemeDataSimilar(iOne,iTwo))
		{
			TRACE(_T("Color Scheme data is not similar\n"));
			return FALSE;
		}
		data=GetNext();
    }
	return TRUE;
}


void CRealFolder::GatherFolderData(CFolderData *FD,int iColorScheme,BOOL bClosedLoops,CLegQuery * pLegQuery)
{
    GotoTop();
    CFolder *data=GetCurrent();
    while (data!=NULL)
    {
		data->GatherFolderData(FD,iColorScheme,bClosedLoops,pLegQuery);
		data=GetNext();
    }
}

BOOL CRealFolder::QuerySurveyComments(CPtrArray * result,CQuery * pTypeQuery,CQuery * pCommentQuery)
{
    BOOL bMatches=FALSE;
	GotoTop();
    CFolder *data=GetCurrent();
    while (data!=NULL)
    {
		if (data->QuerySurveyComments(result,pTypeQuery,pCommentQuery))
		{
			bMatches=TRUE;
		}
		data=GetNext();
    }
	return bMatches;
}

BOOL CRealFolder::HasDisabledLegs(int iColorScheme)
{
	GotoTop();
    CFolder *data=GetCurrent();
    while (data!=NULL)
    {
		if (data->HasDisabledLegs(iColorScheme))
		{
			return TRUE;
		}
		data=GetNext();
    }
    return FALSE;
}


void CRealFolder::DeleteContents()
{
	GotoTop();
    CFolder *data=GetCurrent();
    while (data!=NULL)
    {
		data->DeleteContents();
		delete data;
		data=GetNext();
    }
    m_folderList.RemoveAll();	
}


//Remove, but don't delete the object.
BOOL CRealFolder::Remove(CFolder *folder)
{
	GotoTop();
    CFolder *data=GetCurrent();
    while (data!=NULL)
    {                  
		if (data==folder)
		{
			m_folderList.RemoveAt(m_MyPos);
			return TRUE;
		}
		data=GetNext();
    }
    return FALSE;
}
