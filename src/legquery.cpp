#include "stdafx.h"
#include "onstation.h"
#include "legquery.h"
#include "surveyleg.h"
#include "surveyshot.h"


CLegQuery::CLegQuery()
{
	m_dwStartDate=0xffffffff;
	m_dwEndDate=0xffffffff;
}

CLegQuery::~CLegQuery()
{
}


BOOL CLegQuery::MatchesQuery(CSurveyLeg * pLeg)
{
	if (!m_nameQuery.IsEmpty())
	{
		if (!m_nameQuery.MatchesQuery(pLeg->GetName()))
		{
			if (!m_bCombineUsingOr)
			{
				return FALSE;
			}
		}
		else
		{
			if (m_bCombineUsingOr)
			{
				return TRUE;
			}
		}
	}
	if (!m_descriptionQuery.IsEmpty())
	{
		if (!m_descriptionQuery.MatchesQuery(pLeg->m_szDescription))
		{
			if (!m_bCombineUsingOr)
			{
				return FALSE;
			}
		}
		else
		{
			if (m_bCombineUsingOr)
			{
				return TRUE;
			}
		}
	}

	if (!m_personelQuery.IsEmpty())
	{
		CString szPersonelTest;
		for (int i=0;i<6;i++)
		{
			szPersonelTest+=pLeg->GetPersonName(i);
			szPersonelTest+= _T(" ");
			szPersonelTest+=pLeg->GetPersonDuty(i);
			szPersonelTest+= _T(" ");
		}
		if (!m_personelQuery.MatchesQuery(szPersonelTest))
		{
			if (!m_bCombineUsingOr)
			{
				return FALSE;
			}
		}
		else
		{
			if (m_bCombineUsingOr)
			{
				return TRUE;
			}
		}
	}
	if (!m_stationQuery.IsEmpty())
	{
		CSurveyShotArray * pShots=pLeg->GetShotArray();
		if (!m_stationQuery.MatchesQuery(pShots))
		{
			if (!m_bCombineUsingOr)
			{
				return FALSE;
			}
		}
		else
		{
			if (m_bCombineUsingOr)
			{
				return TRUE;
			}
		}
	}

	if (m_dwStartDate!=0xffffffff || m_dwEndDate!=0xffffffff)
	{
		if (m_dwEndDate!=0xffffffff && m_dwStartDate!=0xffffffff)
		{
			//Between
			return pLeg->m_dwDate>=m_dwStartDate && pLeg->m_dwDate<=m_dwEndDate;
		}
		if (m_dwStartDate!=0xffffffff)
		{
			return pLeg->m_dwDate>=m_dwStartDate;
		}
		if (m_dwEndDate!=0xffffffff)
		{
			return pLeg->m_dwDate<=m_dwEndDate;
		}
	}
	return !m_bCombineUsingOr;
}

void CLegQuery::GetDisplayText(CString * pString)
{
	BOOL bHasContent=FALSE;
	*pString= _T("");
	CString csTemp;
	CString csConnector;
	csConnector.LoadString(m_bCombineUsingOr?IDS_QUERYOR:IDS_QUERYAND);
	if (!m_nameQuery.IsEmpty())
	{
		csTemp.LoadString(IDS_SURVEYNAME);
		*pString+=csTemp;
		*pString+=m_nameQuery.GetQueryText();
		bHasContent=TRUE;
	}
	if (!m_descriptionQuery.IsEmpty())
	{
		if (bHasContent)
		{
			*pString+=csConnector;
		}
		csTemp.LoadString(IDS_DESCRIPTION);
		*pString+=csTemp;
		*pString+=m_descriptionQuery.GetQueryText();
		bHasContent=TRUE;
	}

	if (!m_personelQuery.IsEmpty())
	{
		if (bHasContent)
		{
			*pString+=csConnector;
		}
		csTemp.LoadString(IDS_PERSONNEL);
		*pString+=csTemp;
		*pString+=m_personelQuery.GetQueryText();
		bHasContent=TRUE;
	}
	if (!m_stationQuery.IsEmpty())
	{
		if (bHasContent)
		{
			*pString+=csConnector;
		}
		csTemp.LoadString(IDS_STATIONS);
		*pString+=csTemp;
		*pString+=m_stationQuery.GetQueryText();
		bHasContent=TRUE;
	}

	if (m_dwStartDate!=0xffffffff || m_dwEndDate!=0xffffffff)
	{
		if (bHasContent)
		{
			*pString+=csConnector;
		}
		csTemp.LoadString(IDS_DATE);
		*pString+=csTemp;
		if (m_dwEndDate!=0xffffffff && m_dwStartDate!=0xffffffff)
		{
			csTemp.LoadString(IDS_BETWEEN);
			*pString+=csTemp;
		}
		else
		{
			if (m_dwEndDate!=0xffffffff)
			{
				csTemp.LoadString(IDS_BEFORE2);
				*pString+=csTemp;
			}
			else
			{
				csTemp.LoadString(IDS_AFTER);
				*pString+=csTemp;
			}
		}
		if (m_dwStartDate!=0xffffffff)
		{
			csTemp.Format(_T(" %i.%i.%i"),GetYear(m_dwStartDate),GetMonth(m_dwStartDate),GetDay(m_dwStartDate));
			*pString+=csTemp;
		}
		if (m_dwEndDate!=0xffffffff)
		{
			csTemp.Format(_T(" %i.%i.%i"),GetYear(m_dwEndDate),GetMonth(m_dwEndDate),GetDay(m_dwEndDate));
			*pString+=csTemp;
		}

	}
}
