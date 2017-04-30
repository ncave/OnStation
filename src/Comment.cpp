#include "stdafx.h"
#include "comment.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CComment::CComment(LPCTSTR szStationName,LPCTSTR szType,LPCTSTR szBody)
{
	SetValues(szStationName,szType,szBody);
}
CComment::~CComment()
{
};

void CComment::SetValues(LPCTSTR szStationName,LPCTSTR szType,LPCTSTR szBody)
{
	if (szType!=NULL)
		m_szType=szType;
	if (szBody!=NULL)
		m_szBody=szBody;
	if (szStationName!=NULL)
		lstrcpyn(m_szStationName,szStationName,MAX_STATION_NAME_LENGTH);
}


