#include "stdafx.h"
#include "node.h"
#include "nodetree.h"
#include "onstationdoc.h"
#include "surfacedata.h"
#include "fixedpoint.h"
#include "surveyleg.h"
#include "onstationview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern COnStationDoc * pDocument_G;
extern CSettings * pSettings_G;

CNode::CNode(LPCTSTR szName)
{
    lstrcpyn(m_szNodeName,szName,MAX_STATION_NAME_LENGTH);
	m_byMatchesQuery=FALSE;
}


CNode::~CNode()
{
}
#ifdef _DEBUG
void CNode::Dump()
{
	TRACE(_T("===================\nNode %s\n"),GetName());
	INT_PTR iCount=m_SurveyShotList.GetSize();
	for (INT_PTR j=0;j<iCount;j++)
	{
		CSurveyShot *shot=m_SurveyShotList.GetAt(j);
		shot->Dump();
	}
}
#endif
void CNode::AddShot(CSurveyShot *shot)
{
	//This shot must have been initialized somehow before adding.
	//If not, why not?  One likely cause is calling SetStationNames
	//on a shot before setting the leg data.  The shot data is calculated
	//when the leg is data is set, but is added when station names are set.
	//
	//Only legal case is where the shot is a defining shot only
	ASSERT(shot->GetToStationName()[0]==0 || (shot->m_Delta.GetX()!=NAN));
		
	
	if (shot->GetToStationName()[0]!=0)	//not a wall only shot
	{
	    m_SurveyShotList.Add(shot);
	}
	else
	{
		m_WallsOnlyShots.Add(shot);
	}
}


//Returns TRUE if this is empty!
BOOL CNode::RemoveShot(CSurveyShot *shot)
{
	if (shot->GetToStationName()[0]!=0)
	{
	    m_SurveyShotList.Remove(shot);
	}
	else
	{
		m_WallsOnlyShots.Remove(shot);
	}
    if (!IsEmpty())
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}



//A node needs a label under the following circumstances:
//
// 1)  It is an intersection of more then 1 passage
// 2)  It is the terminus of some survey
// 3)  It is a junction between surveys
// 4)  It is a tie-in to the surface
// 5)  2 incoming or 2 outgoing shots only
BOOL CNode::NeedATextLabel(int iColorScheme)
{
	//Condition 1 or 3 above
	if (GetNumberOfShots(NST_ANYSHOT)!=2)
	{
		return TRUE;
	}

	//2 incoming or 2 outgoing shots is also weird
	if (GetNumberOfShots(NST_INCOMINGSHOT)!=1)
	{
		return TRUE;
	}
	//Size is 2.  Next likeliest option is that we are on a boundary between surveys.
	CSurveyShot *shot1;
	CSurveyShot *shot2;
	shot1=GetShotByIndex(0,NST_ANYSHOT);
	shot2=GetShotByIndex(1,NST_ANYSHOT);
	if (shot1->GetLeg()!=shot2->GetLeg())
		return TRUE;

	//The final possibility is that we are a tie in on the surface.  Check
	//the surface list to verify this.  TODO: This is extremely slow
	return pDocument_G->GetSurface()->IsConstrainedNode(GetName(),iColorScheme);
}


void CNode::SetConstrainedPosition(float fX,float fY,float fZ)
{
	m_Position.SetConstrainedPosition(fX,fY,fZ);
}

//This function sets a flag indicating what the status of this node is.
//The NMT_NOTHING flag resets the node (and redoes some fundamental calculations)
//The NMT_MAINSURVEY type indicates that the node is part of the main (shown) survey.
//The NMT_MISSINGSECTION type indicates that the node is part of some
//			orphaned part of the system (not connected to the main survey).
void CNode::SetMarkType(NODE_MARKTYPE NMT)
{
	//Reset a bunch of information if appropriate
	if (NMT==NMT_NOTHING)
	{
		//Calculate the number of incoming and outgoing shots.  This
		//is easier then doing it on the fly all of the time.
		DoProcessingOfWallData();
	}

	m_Position.m_Marked=NMT;
}



NODE_MARKTYPE CNode::GetMarkType() const
{
	return m_Position.m_Marked;
}


CPosMatrix * CNode::GetPosition(BOOL bClosedLoops)
{
	if (bClosedLoops)
	{
		return &(m_Position.m_ClosedPosition);
	}
	else
	{
		return &(m_Position.m_RawPosition);
	}
}

BOOL CNode::IsSurfaceNode()
{
	for (int i=0;i<GetNumberOfShots(NST_ANYSHOT);i++)
	{
		CSurveyShot *pShot=GetShotByIndex(i,NST_ANYSHOT);
		if (pShot->GetShotFlags()&SHOT_SURFACE)
		{
			return TRUE;
		}
	}
	return FALSE;
}

void CNode::CreateSimpleJunctionBoxIncoming(CSurveyShot *pIncomingShot,CJunctionBox * pJunction)
{
	//This only makes sense for incoming node.
	ASSERT(pIncomingShot->m_ToNode==this);

	//We use this one for our calculations just to keep it consistent
	CSurveyShot *pIncoming=GetShotByIndex(0,NST_INCOMINGSHOT);

	//Must be a simple junction to create a simple junction box.
	ASSERT(HasSimpleJunction());

	//If we have 1 outgoing shots we handle the junction here.
	ASSERT(m_Position.m_iOutgoingShots<2);	//otherwise not a simple junction as we have one incoming
	if (m_Position.m_iOutgoingShots==1)
	{
		//Use the outgoing shot U/D/L/R data and average with the
		//incoming shot angle if appropriate.  If we have 2 outgoing
		//shots we simply take the first one.
		CSurveyShot *pOutgoing=GetShotByIndex(0,NST_OUTGOINGSHOT);
		float fCompassAngle=pOutgoing->GetCalculationCompass();
		float fClinoAngle=pOutgoing->GetCalculationClino();
		fCompassAngle=GetAngleBetween(fCompassAngle,pIncoming->GetCalculationCompass());
		fClinoAngle=GetAngleBetween(fClinoAngle,pIncoming->GetCalculationClino());
		pJunction->GenerateCrossSection(fCompassAngle,fClinoAngle,pOutgoing->GetUp(),pOutgoing->GetDown(),pOutgoing->GetLeft(),pOutgoing->GetRight());
		//An incoming and an outgoing shot makes a nice save junction so we don't
		//have to worry about it getting twisted.
		return;
	}

	//No outgoing shots.  We must have incoming shots otherwise why even bother.
	ASSERT(m_Position.m_iIncomingShots!=0);
	//We use the incoming angle to set the compass and clino angles.
	//NOTE: We do not have to reverse this angle because we are
	//continuing the same trend
	float fCompassAngle=pIncoming->GetCalculationCompass();
	float fClinoAngle=pIncoming->GetCalculationClino();
	BOOL bNeedToReverseIntersection=FALSE;
	if (m_Position.m_iIncomingShots==2)
	{
		CSurveyShot *pSecondIncoming=GetShotByIndex(1,NST_INCOMINGSHOT);
		if (pSecondIncoming==pIncomingShot)
		{
			//in this case we actually need to flip the shot because we want the L/R
			//with respect to the second shot and we are calculating it with respect to
			//the first shot
			bNeedToReverseIntersection=TRUE;
		}
		float fCompass2=pSecondIncoming->GetCalculationCompass();
		fCompass2=fCompass2+180.0f;
		if (fCompass2>360.0f)
		{
			fCompass2=fCompass2-360.0f;
		}
		float fClino2=-pSecondIncoming->GetCalculationClino();
		fCompassAngle=GetAngleBetween(fCompassAngle,fCompass2);
		fClinoAngle=GetAngleBetween(fClinoAngle,fClino2);
	}
	if (GetNumberOfWallOnlyShots()>=1)
	{
		CSurveyShot * pWallShot=GetWallOnlyByIndex(0);
		//This is the obvious case where a wall only shot is used.
		pJunction->GenerateCrossSection(fCompassAngle,fClinoAngle,pWallShot->GetUp(),pWallShot->GetDown(),pWallShot->GetLeft(),pWallShot->GetRight());
	}
	else
	{
		//just project the last shot's dimension onto this one
		pJunction->GenerateCrossSection(fCompassAngle,fClinoAngle,pIncoming->GetUp(),pIncoming->GetDown(),pIncoming->GetLeft(),pIncoming->GetRight());
	}
	if (bNeedToReverseIntersection)
	{
		pJunction->SwapSides();
	}
}

void CNode::CreateSimpleJunctionBoxOutgoing(CSurveyShot *pOutgoingShot,CJunctionBox * pJunction)
{
	//This only makes sense for incoming node.
	ASSERT(pOutgoingShot->m_FromNode==this);
	//Must be a simple junction to create a simple junction box.
	ASSERT(HasSimpleJunction());

	CSurveyShot *pOutgoing=GetShotByIndex(0,NST_OUTGOINGSHOT);

	ASSERT(m_Position.m_iOutgoingShots>0);
	//Use the outgoing shot U/D/L/R data and average with the
	//incoming shot angle if appropriate.  If we have 2 outgoing
	//shots we simply take the first one.
	float fCompassAngle=pOutgoing->GetCalculationCompass();
	float fClinoAngle=pOutgoing->GetCalculationClino();
	BOOL bNeedToReverseIntersection=FALSE;
	//If we have 1 outgoing shots we handle the junction here.
	if (m_Position.m_iIncomingShots==1)
	{
		CSurveyShot *pIncoming=GetShotByIndex(0,NST_INCOMINGSHOT);
		fCompassAngle=GetAngleBetween(fCompassAngle,pIncoming->GetCalculationCompass());
		fClinoAngle=GetAngleBetween(fClinoAngle,pIncoming->GetCalculationClino());
	}
	if (m_Position.m_iOutgoingShots==2)
	{
		CSurveyShot *pSecondOutgoing=GetShotByIndex(1,NST_OUTGOINGSHOT);
		//Our left right up down is calculated with respect to the first
		//incoming shot, so we need to reverse the sides for the second
		//incoming shot.
		if (pSecondOutgoing==pOutgoingShot)
		{
			bNeedToReverseIntersection=TRUE;
		}
		float fCompass2=pSecondOutgoing->GetCalculationCompass();
		fCompass2=fCompass2+180.0f;
		if (fCompass2>360.0f)
		{
			fCompass2=fCompass2-360.0f;
		}
		float fClino2=-pSecondOutgoing->GetCalculationClino();
		fCompassAngle=GetAngleBetween(fCompassAngle,fCompass2);
		fClinoAngle=GetAngleBetween(fClinoAngle,fClino2);
	}
	pJunction->GenerateCrossSection(fCompassAngle,fClinoAngle,pOutgoing->GetUp(),pOutgoing->GetDown(),pOutgoing->GetLeft(),pOutgoing->GetRight());
	if (bNeedToReverseIntersection)
	{
		pJunction->SwapSides();
	}
	return;
}

void CNode::CreateHeightJunctionBox(CJunctionBox *pJunction)
{
	ASSERT(!HasSimpleJunction());
	CSurveyShot *pShot=GetShotByIndex(0,NST_OUTGOINGSHOT);
	if (pShot==NULL)
	{
		pShot=GetWallOnlyByIndex(0);
		if (pShot==NULL)
		{
			pShot=GetShotByIndex(0,NST_INCOMINGSHOT);
		}
	}
	pJunction->GenerateCrossSection(0.0f,0.0f,pShot->GetUp(),pShot->GetDown(),0.0f,0.0f);
}


//if our shot has a simple junction then it is fairly obvious what the junction
//box should look like.  However, if we have a complex junction (ie, multiple
//incoming or outgoing shots) then there is no one single place to join a shot
//to and we need to have the context of the incoming shot as well.
//
//We have an experimental scheme for doing junctions where we actually don't shoot
//to the junction station but instead go around it and shoot to the left and
//right surrounding shot.  We calculate the junction boxes for the left and
//right surrounding shots as at about the half way point between the junction station
//and the incoming shot station.
//
//Hence, incoming junction boxes just take the incoming vector and go half way
//between there and the destination station and apply the incoming U/D/L/R.
void CNode::CreateComplexJunctionBoxIncoming(CSurveyShot * pIncomingShot,CJunctionBox * pJunction)
{
	//This only makes sense for incoming node.
	ASSERT(pIncomingShot->m_ToNode==this);

	//Must be a complex junction to create a simple junction box.
	ASSERT(!HasSimpleJunction());

	float fCompassAngle=pIncomingShot->GetCalculationCompass();
	float fClinoAngle=pIncomingShot->GetCalculationClino();
	if (GetNumberOfWallOnlyShots()>=1)
	{
		CSurveyShot * pWallShot=GetWallOnlyByIndex(0);
		//This is the obvious case where a wall only shot is used.
		pJunction->GenerateCrossSection(fCompassAngle,fClinoAngle,pWallShot->GetUp(),pWallShot->GetDown(),pWallShot->GetLeft(),pWallShot->GetRight());
	}
	else
	{
		//just project the last shot's dimension onto this one
		pJunction->GenerateCrossSection(fCompassAngle,fClinoAngle,pIncomingShot->GetUp(),pIncomingShot->GetDown(),pIncomingShot->GetLeft(),pIncomingShot->GetRight());
	}
}

void CNode::CreateComplexJunctionBoxOutgoing(CSurveyShot * pOutgoingShot,CJunctionBox * pJunction)
{
	//This only makes sense for incoming node.
	ASSERT(pOutgoingShot->m_FromNode==this);

	//Must be a simple junction to create a simple junction box.
	ASSERT(!HasSimpleJunction());

	float fCompassAngle=pOutgoingShot->GetCalculationCompass();
	float fClinoAngle=pOutgoingShot->GetCalculationClino();
	pJunction->GenerateCrossSection(fCompassAngle,fClinoAngle,pOutgoingShot->GetUp(),pOutgoingShot->GetDown(),pOutgoingShot->GetLeft(),pOutgoingShot->GetRight());
//	pJunction->SwapSides();
}


//Any given junction will probably be incomplete.  This thing generates some additional
//geometry which should help make the junction much better looking.
void CNode::CreateComplexJunctionGeometry(CJunctionGeometryBox * pGeometry,BOOL bClosedLoops)
{
	//We need to calculate up and down for the central point.  These are
	//used to join all of the outlying shots into the center
	CJunctionBox * pHeightBox=new CJunctionBox();	//this is for the middle
	CreateHeightJunctionBox(pHeightBox);
	pHeightBox->SetPosition(*GetPosition(bClosedLoops));
	pGeometry->m_pCentralHeightOnlyJunctionBox=pHeightBox;
	//Now that we have calculated the central postion, we are ready to start
	//on all of the side vectors.

	ASSERT(!HasSimpleJunction());

	INT_PTR iNumShots=GetNumberOfShots(NST_ANYSHOT);
	if (iNumShots>50)	//set some sort of tolerance
	{
		iNumShots=50;
	}
	//Create an array of all the shots in sorted order by incoming angle
	CSurveyShot * shot[50];
	for (int i=0;i<_countof(shot);i++)
	{
		shot[i]=NULL;
	}
	for (int i=0;i<iNumShots;i++)
	{
		CSurveyShot *pShot=GetShotByIndex(i,NST_ANYSHOT);
		float fCurrentCompassAngle=pShot->GetCalculationCompass();
		if (pShot->m_ToNode==this)
		{
			fCurrentCompassAngle=fCurrentCompassAngle+180.0f;
			if (fCurrentCompassAngle>360.0f)
			{
				fCurrentCompassAngle-=360.0f;
			}
		}
		int j;
		for (j=0;j<i;j++)
		{
			float fSmallerAngle=shot[j]->GetCalculationCompass();
			if (shot[j]->m_ToNode==this)
			{
				fSmallerAngle=fSmallerAngle+180.0f;
				if (fSmallerAngle>360.0f)
				{
					fSmallerAngle-=360.0f;
				}
			}
			if (fSmallerAngle<fCurrentCompassAngle)
			{
				memmove((void *)(shot+j+1),(void *)(shot+j),(i-j)*sizeof(CSurveyShot *));
				break;
			}
		}
		shot[j]=pShot;
	}
	TRACE(_T("Done sorting the array\n"));

	//Next calculate all of the junctions since these are the raw building blocks that we use to generate the volume from.
	CJunctionBox * junc[50];
	for (int i=0;i<iNumShots;i++)
	{
		junc[i]=new CJunctionBox();
		if (shot[i]->m_ToNode==this)
		{
			CreateComplexJunctionBoxIncoming(shot[i],junc[i]);
			CPosMatrix newPos;
			newPos.Set(	(shot[i]->m_ToNode->GetPosition(bClosedLoops)->GetX()*CENTER_WEIGHT+shot[i]->m_FromNode->GetPosition(bClosedLoops)->GetX()*OUTSIDE_WEIGHT),
						(shot[i]->m_ToNode->GetPosition(bClosedLoops)->GetY()*CENTER_WEIGHT+shot[i]->m_FromNode->GetPosition(bClosedLoops)->GetY()*OUTSIDE_WEIGHT),
						(shot[i]->m_ToNode->GetPosition(bClosedLoops)->GetZ()*CENTER_WEIGHT+shot[i]->m_FromNode->GetPosition(bClosedLoops)->GetZ()*OUTSIDE_WEIGHT));
			junc[i]->SetPosition(newPos);
		}
		else
		{
			CreateComplexJunctionBoxOutgoing(shot[i],junc[i]);
			CPosMatrix newPos;
			newPos.Set(	(shot[i]->m_ToNode->GetPosition(bClosedLoops)->GetX()*OUTSIDE_WEIGHT+shot[i]->m_FromNode->GetPosition(bClosedLoops)->GetX()*CENTER_WEIGHT),
						(shot[i]->m_ToNode->GetPosition(bClosedLoops)->GetY()*OUTSIDE_WEIGHT+shot[i]->m_FromNode->GetPosition(bClosedLoops)->GetY()*CENTER_WEIGHT),
						(shot[i]->m_ToNode->GetPosition(bClosedLoops)->GetZ()*OUTSIDE_WEIGHT+shot[i]->m_FromNode->GetPosition(bClosedLoops)->GetZ()*CENTER_WEIGHT));
			junc[i]->SetPosition(newPos);
			junc[i]->SwapSides();
/*			//Survey junctions are always in the direction of the shot.  Since we are
			//connecting incoming and outgoing junctions, we need to reverse one
			//and we arbitrarily decided to make them all look as if they were incoming.
			CPosMatrix swap=junc[i].m_TopLeft;
			junc[i].m_TopLeft=junc[i].m_TopRight;
			junc[i].m_TopRight=swap;
			swap=junc[i].m_BottomLeft;
			junc[i].m_BottomLeft=junc[i].m_BottomRight;
			junc[i].m_BottomRight=swap;
*/
		}
	}
	for (int i=0;i<iNumShots;i++)
	{
		pGeometry->m_junctionBoxArray.Add(junc[i]);
	}
}


BOOL CNode::HasSimpleJunction()
{
	return ((m_Position.m_iIncomingShots+m_Position.m_iOutgoingShots)<=2);
}

void CNode::DoProcessingOfWallData()
{
	m_Position.m_iTotalOfAllShots=0;
	m_Position.m_iOutgoingShots=0;
	m_Position.m_iIncomingShots=0;
	m_Position.m_iWallOnlyShots=0;
	INT_PTR iCount=m_SurveyShotList.GetSize();
	for (INT_PTR j=0;j<iCount;j++)
	{
		CSurveyShot *shot=m_SurveyShotList.GetAt(j);
		if (shot->GetLeg()->IsActive(pSettings_G->m_iColorSchemeIndex))
		{
			m_Position.m_iTotalOfAllShots++;
			if (shot->m_ToNode==this)
			{
				m_Position.m_iIncomingShots++;
			}
			else
			{
				m_Position.m_iOutgoingShots++;
			}
		}
	}
	iCount=m_WallsOnlyShots.GetSize();
	for (int j=0;j<iCount;j++)
	{
		CSurveyShot *shot=m_WallsOnlyShots.GetAt(j);
		if (shot->GetLeg()->IsActive(pSettings_G->m_iColorSchemeIndex))
		{
			m_Position.m_iWallOnlyShots++;
		}
	}
}



INT_PTR CNode::GetNumberOfShots(NODE_SHOTTYPE type)
{
	switch (type)
	{
	case NST_ANYSHOT:
		return m_Position.m_iTotalOfAllShots;
	case NST_OUTGOINGSHOT:
		return m_Position.m_iOutgoingShots;
	case NST_INCOMINGSHOT:
		return m_Position.m_iIncomingShots;
	case NST_GLOBALSHOT_IGNORE_COLOR_SCHEME:
		return m_SurveyShotList.GetSize();
	default:
		ASSERT(FALSE);
	}
	return 0;
}

CSurveyShot * CNode::GetShotByIndex(INT_PTR iIndex, NODE_SHOTTYPE type)
{
	INT_PTR iCount=m_SurveyShotList.GetSize();
	for (INT_PTR j=0;j<iCount;j++)
	{
		CSurveyShot *shot=m_SurveyShotList.GetAt(j);
		if (type==NST_GLOBALSHOT_IGNORE_COLOR_SCHEME || (shot->GetLeg()->IsActive(pSettings_G->m_iColorSchemeIndex)))
		{
			switch (type)
			{
			case NST_GLOBALSHOT_IGNORE_COLOR_SCHEME:
			case NST_ANYSHOT:
				iIndex--;
				break;
			case NST_INCOMINGSHOT:
				if (shot->m_ToNode==this)
				{
					iIndex--;
				}
				break;
			case NST_OUTGOINGSHOT:
				if (shot->m_ToNode!=this)
				{
					iIndex--;
				}
				break;
			default:
				ASSERT(FALSE);
			}
		}
		if (iIndex==-1)
			return shot;
	}
return NULL;
}

INT_PTR CNode::GetNumberOfWallOnlyShots()
{
	return m_Position.m_iWallOnlyShots;
}

CSurveyLeg * CNode::GetLeg()
{
	if (m_SurveyShotList.GetSize()!=0)
	{
		return m_SurveyShotList.GetAt(0)->GetLeg();
	}
	else
	{
		return m_WallsOnlyShots.GetAt(0)->GetLeg();
	}
}

CSurveyShot * CNode::GetWallOnlyByIndex(INT_PTR iIndex)
{
	INT_PTR iCount=m_WallsOnlyShots.GetSize();
	for (INT_PTR j=0;j<iCount;j++)
	{
		CSurveyShot *shot=m_WallsOnlyShots.GetAt(j);
		if (shot->GetLeg()->IsActive(pSettings_G->m_iColorSchemeIndex))
		{
			iIndex--;
		}
		if (iIndex==-1)
			return shot;
	}
	return NULL;
}


