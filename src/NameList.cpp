#include "stdafx.h"
#include "namelist.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CNameList::CNameList()
{
	m_iLowestYear=0;
	m_iHighestYear=0;
}


CNameList::~CNameList()
{
	m_ListByNameOnly.RemoveAll();
	m_ListByYear.RemoveAll();
}


void CNameList::AddName(LPCTSTR szName,int iYear)
{						  
	ASSERT(iYear!=-1);  //the year should be 0 to indicate not known.
	POSITION Pos=m_ListByYear.GetHeadPosition();
	BOOL bFoundMatch=FALSE;
	while (Pos!=NULL && !bFoundMatch)
	{
		CNameHolderWithDate& NHWD=m_ListByYear.GetNext(Pos);
		//name is recurring so update the entry
		if (NHWD.m_iYear==iYear && lstrcmpi(szName,NHWD.m_szName)==0)
		{
			NHWD.m_iNumberOfTrips++;
			bFoundMatch=TRUE;
		}
	}
	if (!bFoundMatch)
	{
		CNameHolderWithDate NHWD;
		NHWD.m_szName=szName;
		NHWD.m_iNumberOfTrips=1;
		NHWD.m_iYear=iYear;
		m_ListByYear.AddTail(NHWD);
	}

	Pos=m_ListByNameOnly.GetHeadPosition();
	bFoundMatch=FALSE;
	while (Pos!=NULL && !bFoundMatch)
	{
		CNameHolderWithIncidents& NHWI=m_ListByNameOnly.GetNext(Pos);
		//name is recurring so update the entry
		if (lstrcmpi(szName,NHWI.m_szName)==0)
		{
			NHWI.m_iNumberOfTrips++;
			bFoundMatch=TRUE;
		}
	}
	if (!bFoundMatch)
	{
		CNameHolderWithIncidents NHWI;
		NHWI.m_szName=szName;
		NHWI.m_iNumberOfTrips=1;
		m_ListByNameOnly.AddTail(NHWI);
	}
}


INT_PTR CNameList::GetNumberOfNamesForYear(int iYear) const
{
	if (iYear==-1)
	{
		return m_ListByNameOnly.GetCount();
	}
	else
	{
		int iMatches=0;
		POSITION Pos=m_ListByYear.GetHeadPosition();
		while (Pos!=NULL)
		{
			const CNameHolderWithDate& NHWD=m_ListByYear.GetNext(Pos);
			//name is recurring so update the entry
			if (NHWD.m_iYear==iYear)
			{
				iMatches++;
			}
		}
		return iMatches;
	}
}

void CNameList::SetYear(int iYear)
{
	if (iYear>2)
	{
		if (m_iLowestYear==0)
			m_iLowestYear=iYear;
		if (m_iHighestYear==0)
			m_iHighestYear=iYear;
		m_iLowestYear=min(iYear,m_iLowestYear);
		m_iHighestYear=max(iYear,m_iHighestYear);
	}
}


LPCTSTR CNameList::GetName(int iYear,int iIndex,int *iUsed)
{
	if (iYear==-1)
	{
		POSITION Pos=m_ListByNameOnly.FindIndex(iIndex);
		if (Pos==NULL)
			return NULL;
		CNameHolderWithIncidents& NHWI=m_ListByNameOnly.GetAt(Pos);
		*iUsed=NHWI.m_iNumberOfTrips;
		return NHWI.m_szName;
	}
	else
	{
		POSITION Pos=m_ListByYear.GetHeadPosition();
		while (Pos!=NULL)
		{
			CNameHolderWithDate& NHWD=m_ListByYear.GetNext(Pos);
			//name is recurring so update the entry
			if (NHWD.m_iYear==iYear)
			{
				iIndex--;
				if (iIndex==0)
				{
					*iUsed=NHWD.m_iNumberOfTrips;
					return NHWD.m_szName;
				}
			}
		}
		return NULL;
	}
}

