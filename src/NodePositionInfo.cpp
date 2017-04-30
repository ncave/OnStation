#include "stdafx.h"
#include "nodepositioninfo.h"
#include "mathstuff.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CNodePositionInfo::CNodePositionInfo()
{
	m_iTotalOfAllShots=0;
	m_iOutgoingShots=0;
	m_iIncomingShots=0;
	m_iWallOnlyShots=0;
}

CNodePositionInfo::~CNodePositionInfo()
{

}

//Use this to set the position if it is constrained (ie, it is at a known
//location which should not be moved).
void CNodePositionInfo::SetConstrainedPosition(float fX,float fY,float fZ)
{
	m_RawPosition.Set(fX,fY,fZ);
	m_ClosedPosition.Set(fX,fY,fZ);
	//don't calculate through this point as it will be used as a starting
	//point later in the calculation.
	m_Marked=NMT_MAINSURVEY;
	m_iNodeType=1;
}

void CNodePositionInfo::Copy(CNodePositionInfo * NPI)
{
	memcpy((LPVOID)this,(LPVOID)NPI,sizeof(CNodePositionInfo)-1);
}

