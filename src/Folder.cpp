#include "stdafx.h"
#include "folder.h"
#include "surveyleg.h"
#include "realfolder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CFolderData::CFolderData()
{
	iNumSurveys=0;
	iTotalShots=0;
	iNumSurfaceShots=0;
	iNumCaveShots=0;
	iNumSplayShots=0;
	iNumExcludedShots=0;
	iNumLengthExcludedShots=0;
	fTotalShotLengthSurface=0.0f;
	fTotalShotLengthCave=0.0f;
}

CFolderData::~CFolderData()
{
}

CFolder::CFolder(COnStationDoc * document)
{
	m_ParentFolder=NULL;
	m_MyDocument=document;
	m_szName[0]=_T('\0');
}
CFolder::~CFolder()
{
}

//Returns TRUE if a folder can be dropped onto us.  If the folder is
//a parent of us, then we return false.
BOOL CFolder::IsDropTarget(CFolder *fold)
{
	if (fold==this)
		return FALSE;	
	if (!IsFolder())		//muse be a real folder
		return FALSE;
	if (!fold->IsFolder())
		return TRUE;

	CRealFolder *RF=GetParentFolder();
	while (RF!=NULL && RF!=(CRealFolder *)fold)
	{
		RF=RF->GetParentFolder();
	}
	return (RF==NULL);
}

void CFolder::SetParentFolder(CRealFolder *fold,CFolder * pInsertAfter)
{
	if (fold==this)
		return;			//safety check

	if (m_ParentFolder!=NULL)
	{
		m_ParentFolder->Remove(this);
	}
	m_ParentFolder=fold;
	m_ParentFolder->Add(this,pInsertAfter);
}


void CFolder::SetName(LPCTSTR szName)
{
 	lstrcpyn(m_szName,szName,_countof(m_szName));
}

LPCTSTR CFolder::GetName()
{
 	return m_szName;
}

int CFolder::GetDepth()
{
	if (m_ParentFolder==NULL)
	{
		return 0;
	}
	else
	{
		return m_ParentFolder->GetDepth()+1;
	}
}
