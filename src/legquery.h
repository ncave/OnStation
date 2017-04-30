#ifndef I_LEGQUERY__
#define I_LEGQUERY__
#include "query.h"

class CSurveyLeg;
class CLegQuery:public CObject
{
public:
	CLegQuery();
	~CLegQuery();

	CQuery m_nameQuery;
	CQuery m_descriptionQuery;
	CQuery m_personelQuery;
	CQuery m_stationQuery;
	DWORD m_dwStartDate;
	DWORD m_dwEndDate;
	BOOL m_bCombineUsingOr;

	BOOL MatchesQuery(CSurveyLeg * pLeg);
	void GetDisplayText(CString * pString);
private:
	
};



#endif


