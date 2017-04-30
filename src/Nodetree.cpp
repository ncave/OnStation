// (C) Copyright 1994-1995  Taco van Ieperen
//
// 
#include "stdafx.h"
#include "nodetree.h"
#include "surveyleg.h"
#include "node.h"
#include "surfacedata.h"
#include "onstationdoc.h"
#include "onstation.h"
#include "waitforclosuredialog.h"
#include "jpclose.h"
#include "colortool.h"
#include "realfolder.h"

extern COnStationApp theApp;
extern CSettings * pSettings_G;
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern COnStationDoc * pDocument_G;

CNode * CNodeTree::MakeUpAStartingNode(CRealFolder *myroot,int iColorScheme)
{
	//No starting point, so try and find a default one.
	m_iBlunderMode = 0;
	CFolder *folder;
	myroot->GotoTop();
	folder=myroot->GetCurrent();
	while (folder!=NULL)
	{
		if (folder->IsFolder())
		{
			CNode *node=MakeUpAStartingNode((CRealFolder *)folder,iColorScheme);
			if (node!=NULL)
			{
				return node;
			}
		}
		else
		{
			CSurveyLeg *leg=(CSurveyLeg *)folder;
			if (leg->GetDrawColor(iColorScheme)!=COLOR_INACTIVE)
			{
				CSurveyShotArray *shots=leg->GetShotArray();
				if (shots->GetSize()!=0)
				{				
					CSurveyShot *shot=shots->GetAt(0);
					if (shot->GetFromStationName()!=NULL)
					{
						return m_MyDocument->GetNodeTree()->FindNode(shot->GetFromStationName());
					}
				}
			}
		}
		folder=myroot->GetNext();
	}
	return NULL;
}

void CNodeTree::AddShot(CSurveyShot *pShot)
{
    ASSERT(pShot->m_FromNode==NULL);
    pShot->m_FromNode=FindNode(pShot->m_szFromStationName,TRUE);	//Add if not found
    pShot->m_FromNode->AddShot(pShot);
	ASSERT(pShot->m_ToNode==NULL);
	if (pShot->m_szToStationName[0]!=0)		//no to name is set since this is a simple wall vector shot
	{
		pShot->m_ToNode=FindNode(pShot->m_szToStationName,TRUE);	//Add if it can't be found
		pShot->m_ToNode->AddShot(pShot);
	}
}



void CNodeTree::RemoveShot(CSurveyShot *pShot)
{
    if (pShot->m_FromNode!=NULL)
    {
    	BOOL bEmpty=pShot->m_FromNode->RemoveShot(pShot);
    	if (bEmpty)
    	{
    	    RemoveNode(pShot->m_FromNode);
			delete pShot->m_FromNode;
    	    pShot->m_FromNode=NULL;
    	}
    }
    if (pShot->m_ToNode!=NULL)	//Old one was emtpy.
    {
    	BOOL bEmpty=pShot->m_ToNode->RemoveShot(pShot);
    	if (bEmpty)
    	{
    	    RemoveNode(pShot->m_ToNode);
			delete pShot->m_ToNode;
    	    pShot->m_ToNode=NULL;
    	}
    }
}


CNodeTree::CNodeTree(COnStationDoc *document)
{
	m_MyDocument=document;
	for (int i=0;i<HASHSIZE;i++)
	{
		m_HashTable[i]=NULL;
	}
	for (int i=0;i<TOTAL_COLOR_SCHEMES;i++)
	{
		m_bIsClosureDataDirty[i]=TRUE;
	}
	m_iNumberOfNodes=0;
}

CNodeTree::~CNodeTree()
{
	DeleteContents();
}


WORD CNodeTree::HashName(LPCTSTR szName)
{
	static long iMultiplies[8]={7,11,19,23,31,57,83,101};
	
	long lReturn=0;
	int i=0;
	while (szName[i]!=NULL)
	{
		lReturn=lReturn+((long)szName[i])*iMultiplies[i];
		i++;
	}
	if (lReturn<0)
		lReturn=0;
	return (WORD)(lReturn%HASHSIZE);
}



//Finds a node in the node tree.  Creates one if requested if the node is not
//there already.
CNode * CNodeTree::FindNode(LPCTSTR szName,BOOL bCreateIfMissing)
{
	POSITION pos;
	
	//This code is also used below in AddNewNode.
	WORD w=HashName(szName);
	if (m_HashTable[w]==NULL)
	{
		if (bCreateIfMissing)
		{
			m_HashTable[w]=new CPtrList();
			goto MakeNode;
		}
		else
		{
			return NULL;
		}
	}	
	pos=m_HashTable[w]->GetHeadPosition();
	while (pos!=NULL)
	{
		CNode *node=(CNode *)(m_HashTable[w]->GetNext(pos));
		if (lstrcmpi(node->GetName(),szName)==0)
			return node;
	}
	if (!bCreateIfMissing)
	{
		return NULL;
	}
MakeNode:
	CNode *node=new CNode(szName);
	m_HashTable[w]->AddTail((CObject *)node);
	m_iNumberOfNodes++;
	return node;
}

void CNodeTree::RemoveNode(CNode *node)
{
//	ASSERT(AfxCheckMemory());
	WORD wHash=HashName(node->GetName());
    ASSERT(m_HashTable[wHash]!=NULL);
    
    POSITION pos=m_HashTable[wHash]->Find((CObject *)node);
    ASSERT(pos!=NULL);
    m_HashTable[wHash]->RemoveAt(pos);
    if (m_HashTable[wHash]->IsEmpty())
    {
    	delete m_HashTable[wHash];
    	m_HashTable[wHash]=NULL;
		m_iNumberOfNodes--;
    }
//	ASSERT(AfxCheckMemory());
}






void CNodeTree::CalculateRawPositions(CSurfaceData *SurfaceData,int iColorScheme)
{
	m_MyDocument->BeginWaitCursor();
	UnmarkAllNodes();
	m_bIsClosureDataDirty[iColorScheme]=TRUE;		
	SurfaceData->GlobalSetSurfacePositions();

	//First calculate all of the "known" stuff based on our FIXED POINTS.
	CalculateRawPositionsHelper(NULL,iColorScheme,NMT_MAINSURVEY);

	m_NodesNotOnSurvey.RemoveAll();


	
	
	//Now calculate all of the stuff that was not part of this survey.
	//This is so that we can list it in the "ORPHANS" dialog box.
	for (int i=0;i<HASHSIZE;i++)
	{
		if (m_HashTable[i]!=NULL)
		{
			POSITION pos=m_HashTable[i]->GetHeadPosition();
			while (pos!=NULL)
			{
				CNode *node=(CNode *)(m_HashTable[i]->GetNext(pos));
				if (node->GetMarkType()==NMT_NOTHING && node->GetLeg()->IsActive(iColorScheme))
				{
					node->SetConstrainedPosition(0.0f,0.0f,0.0f);

					m_NodesNotOnSurvey.AddTail((LPVOID)node);
					//calculate everything reachable from this node.
					//This stops us from repeating information by accident.
					CalculateRawPositionsHelper(node,iColorScheme,NMT_MISSINGSECTION);
				}
			}
		}
	}
	m_MyDocument->EndWaitCursor();
}



void CNodeTree::CalculateRawPositionsHelper(CNode *start,int iColorScheme,NODE_MARKTYPE NMT)
{
    int iRingHead=1;
    int iRingTail=0;
    CNode ** MyQueue=new CNode * [MAX_GWORLD_QDEPTH];
	if (start!=NULL)
	{
		//If a node is suggested we simply start with that in our queue and solve from
		//there.
		MyQueue[0]=start;
		start->SetMarkType(NMT);
	}
	else
	{
		//No node was suggested.  This happens only on the main closure loop for
		//the main system.  What we do is put all of the fixed points onto our
		//queue and solve from there.  This allows us to spread the errors out as far
		//from our fixed points as possible since we start at a series of known positions.
		int iIndex=0;
		CNode *node;
		while ((node=m_MyDocument->GetSurface()->GetSuggestedStartingNode(iIndex))!=NULL)
		{
			TRACE(_T("CalculateRawPositionHelper::Got a fixed point %s\n"),node->GetName());
			MyQueue[iIndex++]=node;
			node->SetMarkType(NMT);
		}
		
		if (iIndex==0)	//no fixed points, so make up one as best as we can and
						//set it at the center of the surface
		{   
			node=MakeUpAStartingNode(m_MyDocument->GetSurveyFolder(),iColorScheme);
			if (node==NULL)
			{
				delete[] MyQueue;
				return;		//nothing to solve
			}
			TRACE(_T("Made up a fake fixed point %s\n"),node->GetName());
			//Set starting position to middle of surface
			//if not, then set it to 0,0,0
			CSurfaceData *Surf=m_MyDocument->GetSurface();
			node->SetConstrainedPosition(0.0f,0.0f,Surf->GetMiddleAltitude());
			node->SetMarkType(NMT);
			MyQueue[0]=node;
			iRingHead=1;
		}
		else
		{
			iRingHead=iIndex;
		}
	}
	//Use a array to eliminate recursion
	while (iRingTail!=iRingHead)
	{
		CNode *node=MyQueue[iRingTail];
		INT_PTR iMax=node->GetNumberOfShots(NST_ANYSHOT);
		for (INT_PTR i=0;i<iMax;i++)
		{
			CSurveyShot *pShot=node->GetShotByIndex(i,NST_ANYSHOT);
			// We add every shot we get to to the queue.

			if (pShot->m_ToNode==node)		//backsight from here
			{
				//Have we visited that node before
				if (pShot->m_FromNode->GetMarkType()==NMT_NOTHING)
				{
					CPosMatrix *Pos=node->GetPosition(FALSE);
					CPosMatrix *PosFrom=pShot->m_FromNode->GetPosition(FALSE);
					Pos->Subtract(PosFrom,pShot->m_Delta);
					pShot->m_FromNode->SetMarkType(NMT);

					MyQueue[iRingHead]=pShot->m_FromNode;
					iRingHead=(iRingHead+1)%MAX_GWORLD_QDEPTH;
				}
			}
			else	//Front sight from here
			{
				ASSERT(pShot->m_FromNode==node);	//one has to match
				if (pShot->m_ToNode->GetMarkType()==NMT_NOTHING)		//TO a new node so add it and draw
				{
					CPosMatrix *Pos=node->GetPosition(FALSE);
					CPosMatrix *PosTo=pShot->m_ToNode->GetPosition(FALSE);
					Pos->Add(PosTo,pShot->m_Delta);
					pShot->m_ToNode->SetMarkType(NMT);

					MyQueue[iRingHead]=pShot->m_ToNode;
					iRingHead=(iRingHead+1)%MAX_GWORLD_QDEPTH;
				}
			}
		}
		iRingTail=(iRingTail+1)%MAX_GWORLD_QDEPTH;
	}    
	delete[] MyQueue;
}


//To fill the gworld we merely walk through the entire tree that we calculated
//and add all of the forward shots to the GWorld.
void CNodeTree::FillGWorld(CGWorld *GW)
{
	m_MyDocument->BeginWaitCursor();
	GW->Empty();
	int iColorScheme=pSettings_G->m_iColorSchemeIndex;
	for (int i=0;i<HASHSIZE;i++)
	{
		if (m_HashTable[i]!=NULL)
		{
			POSITION pos=m_HashTable[i]->GetHeadPosition();
			while (pos!=NULL)
			{
				CNode *node=(CNode *)(m_HashTable[i]->GetNext(pos));
				//Is the node marked?
				if (node->GetMarkType()==NMT_MAINSURVEY)
				{														 
					INT_PTR iMax=node->GetNumberOfShots(NST_ANYSHOT);
					for (INT_PTR i=0;i<iMax;i++)
					{
						CSurveyShot *pShot=node->GetShotByIndex(i,NST_ANYSHOT);
						//Shots with total exclusion should not be in the Node Tree
						if (pShot->m_FromNode==node)
						{
							//We can't just check the color scheme for the node because the shot
							//color scheme is more accurate since only shots are stored in a
							//given surveys where as nodes may be present in several surveys
							COLORREF crColor;
							if (pSettings_G->m_iColorSchemeIndex!=COLORSCHEME_BLUNDER)
							{
								crColor=pShot->GetLeg()->GetDrawColor(pSettings_G->m_iColorSchemeIndex);
							}
							else
							{
								crColor=pSettings_G->GetBlunderColor((int)pShot->GetStressDeviation());
							}
							if (crColor>=0)
							{
								//Discard hidden surveys as well as surface surveys if our view options are to not show them.
								if (pShot->GetLeg()->GetDrawColor(pSettings_G->m_iColorSchemeIndex)>=0 && (pSettings_G->m_bSurfaceSurveys[pSettings_G->m_iColorSchemeIndex] || !(pShot->GetShotFlags()&SHOT_SURFACE)))
								{
									RenderShot(GW,pShot);
								}
							}
						}
					}
					//Is the primary leg for this node visible?  If so, show it.
					//We couldn't filter this out outside the loop because the node
					//may be in more then one leg so we want to ensure that we fairly
					//represent all effected legs.
					if (node->GetLeg()->GetDrawColor(iColorScheme)>=0)
					{
						if (node->GetLeg()->GetDrawColor(pSettings_G->m_iColorSchemeIndex)>=0)
						{
							CPosMatrix *Pos=node->GetPosition(pSettings_G->m_bShowClosedLoops[pSettings_G->m_iColorSchemeIndex]);
							AddLabelToGWorld(GW,node,*Pos);
						}
					}
				}
			}
		}
	}
	m_MyDocument->EndWaitCursor();
}


//This also adds the flags for the constrained positions
void CNodeTree::AddLabelToGWorld(CGWorld *GW,CNode *node,CPosMatrix& Pos)
{
	CSurveyLeg *Leg=node->GetLeg();
	//Don't add surface nodes if surfaces are turned off
	if (!pSettings_G->m_bSurfaceSurveys[pSettings_G->m_iColorSchemeIndex] && node->IsSurfaceNode())
	{
		return;
	}
	BOOL bIntersect=node->NeedATextLabel(pSettings_G->m_iColorSchemeIndex);
	if (node->MatchesQuery())
	{
		GW->AddConstraint(Pos,pSettings_G->m_crQueryColor,node,TRUE);
	}
	if (pSettings_G->m_iColorSchemeIndex==COLORSCHEME_DEPTH)
	{
		int iToZone=-1*(int)((Pos.GetZ()-pSettings_G->m_fStartDepth)/pSettings_G->m_fDepthIncrements);
		GW->AddLabel(node,Pos,node->GetName(),bIntersect,pSettings_G->GetDepthColor(iToZone));
	}
	else
	{
		COLORREF crColor=Leg->GetDrawColor(pSettings_G->m_iColorSchemeIndex);
		if (crColor!=COLOR_INVISIBLE)
		{
			GW->AddLabel(node,Pos,node->GetName(),bIntersect,crColor);
		}
	}
}		


/*
void CNodeTree::AddRectangleToGWorld(CGWorld *GW,CSurveyLeg *Leg,CPosMatrix& topLeft,CPosMatrix& topRight,CPosMatrix& bottomRight,CPosMatrix& bottomLeft,BOOL bMatchesQuery,LIGHTINGMODIFIER LM)
{
	ASSERT(Leg!=NULL);
	COLORREF crColor;
	if (!bMatchesQuery)
	{
		crColor=Leg->GetDrawColor(ppSettings_G->m_iColorSchemeIndex);
	}
	else
	{
		crColor=ppSettings_G->m_iQueryColor;
	}
	
	//We fake nice lighting if the system is not doing
	//the lighting calculations itself.  Otherwise we rely on
	//the system's lighting.
	if (!ppSettings_G->m_bMultipleLights)
	{
		switch (LM)
		{
		case TOP_LIGHTING:
			crColor=CGlobalColorManager::GetLighterShade(crColor);
			break;
		case SIDE_LIGHTING:
			crColor=CGlobalColorManager::GetNeutralShade(crColor);
			break;
		case BOTTOM_LIGHTING:
			crColor=CGlobalColorManager::GetDarkerShade(crColor);
			break;
		case NATURAL_LIGHTING:
			break;	//don't mess with the color.
		};
	}
	GW->AddRectangle(topLeft,topRight,bottomRight,bottomLeft,crColor);
}
*/


void CNodeTree::AddLineToGWorld(CGWorld *GW,COLORREF crColor,CPosMatrix& From,CPosMatrix& To,BOOL bMatchesQuery,LINE_TYPE lineType)
{
	if (From.IsEqual(To))
	{
		return;		//Empty line.  This check saves us some work
					//in the cross section generating code.
	}
	if (!bMatchesQuery)
	{
		if (pSettings_G->m_iColorSchemeIndex!=COLORSCHEME_DEPTH)
		{
			GW->AddLineSegment(From,To,crColor,lineType);
		}
		else
		{
			AddDepthLineToGWorld(GW,From,To,lineType);
		}
	}
	else
	{
		GW->AddLineSegment(From,To,pSettings_G->m_crQueryColor,lineType);
	}
}


//We assume that any discarding of this shot would already have been
//done were it not to be displayed in the current view.
void CNodeTree::AddDepthLineToGWorld(CGWorld *GW,CPosMatrix& From,CPosMatrix& To,LINE_TYPE lineType)
{
	CPosMatrix newFrom,newTo;
	//Make sure that shots always go down.  newTo Z must be less then newFrom Z
	if (To.GetZ()>From.GetZ())
	{
		newTo=From;
		newFrom=To;
	}
	else
	{
		newTo=To;
		newFrom=From;
	}

	//If it is outside of the depth color arena, we don't need to segment it.
	if (newFrom.GetZ()>=(pSettings_G->m_fStartDepth-pSettings_G->m_fDepthIncrements*MAX_DEPTH_COLORS)
		&& newTo.GetZ()<=pSettings_G->m_fStartDepth)
	{
		//Start outside of the depth area.  Draw the area first and then update the from position
		//to be right on the boundary where the depth area starts.  
		if (newFrom.GetZ()>pSettings_G->m_fStartDepth)
		{
			CPosMatrix Temp;
			newFrom.CalculateIntermediatePosFromZ(pSettings_G->m_fStartDepth,newTo,Temp);
			if (pSettings_G->GetDepthColor(0)>=RGB(0,0,0))
			{
				GW->AddLineSegment(newFrom,Temp,pSettings_G->GetDepthColor(0),lineType);
			}
			newFrom=Temp;
		}
		if (newTo.GetZ()<pSettings_G->m_fStartDepth-pSettings_G->m_fDepthIncrements*MAX_DEPTH_COLORS)
		{
			CPosMatrix Temp;
			newFrom.CalculateIntermediatePosFromZ(pSettings_G->m_fStartDepth-pSettings_G->m_fDepthIncrements*MAX_DEPTH_COLORS,newTo,Temp);
			if (pSettings_G->GetDepthColor(MAX_DEPTH_COLORS-1)>=RGB(0,0,0))
			{
				GW->AddLineSegment(Temp,newTo,pSettings_G->GetDepthColor(MAX_DEPTH_COLORS-1),lineType);
			}
			newTo=Temp;
		}

		int iFromIndex=(int)(-1.0*(newFrom.GetZ()-pSettings_G->m_fStartDepth)/pSettings_G->m_fDepthIncrements);
		int iToIndex=(int)(-1.0*(newTo.GetZ()-pSettings_G->m_fStartDepth)/pSettings_G->m_fDepthIncrements);
		ASSERT(iFromIndex<=iToIndex);
		while (iFromIndex!=iToIndex)
		{
			CPosMatrix Temp;
			newFrom.CalculateIntermediatePosFromZ(pSettings_G->m_fStartDepth-pSettings_G->m_fDepthIncrements*(iFromIndex+1),newTo,Temp);
			if (pSettings_G->GetDepthColor(iFromIndex)>=RGB(0,0,0))
			{
				GW->AddLineSegment(newFrom,Temp,pSettings_G->GetDepthColor(iFromIndex),lineType);
			}
			newFrom=Temp;
			iFromIndex++;
		}
		if (pSettings_G->GetDepthColor(iFromIndex)>=RGB(0,0,0))
		{
			GW->AddLineSegment(newFrom,newTo,pSettings_G->GetDepthColor(iFromIndex),lineType);
		}
	}
	else
	{
		//Set "above" or "below" color based on position
		int iZone=MAX_DEPTH_COLORS-1;
		if (newTo.GetZ()>=pSettings_G->m_fStartDepth)
		{
			iZone=0;
		}
		if (pSettings_G->GetDepthColor(iZone)>=0)
		{
			GW->AddLineSegment(newFrom,newTo,pSettings_G->GetDepthColor(iZone),lineType);
		}
	}
}

/*
void CNodeTree::AddDepthRectangleToGWorld(CGWorld *GW,CSurveyLeg *Leg,CPosMatrix& topLeft,CPosMatrix& topRight,CPosMatrix& bottomRight,CPosMatrix& bottomLeft)
{
	float fAverageZ=(topLeft.GetZ()+bottomRight.GetZ())/2.0f;

	CDocumentSettings *set=m_MyDocument->GetDocumentSettings();

	int iColor=-1;
	if (fAverageZ<=pSettings_G->m_fStartDepth)
	{
		iColor=pSettings_G->GetDepthColor(0);
	}
	else if (fAverageZ>=pSettings_G->m_fStartDepth-pSettings_G->m_fDepthIncrements*pSettings_G->m_iNumDepthColors)
	{
		iColor=pSettings_G->GetDepthColor(pSettings_G->m_iNumDepthColors);
	}
	else
	{
		int iIndex=(int)((fAverageZ-pSettings_G->m_fStartDepth)/pSettings_G->m_fDepthIncrements);
		iColor=pSettings_G->GetDepthColor(iIndex);
	}
	if (iColor>=0)
	{
		GW->AddRectangle(topLeft,topRight,bottomRight,bottomLeft,iColor);
	}
}
*/

//Make sure that every node in the system is unmarked.
void CNodeTree::UnmarkAllNodes()
{
	for (int i=0;i<HASHSIZE;i++)
	{
		if (m_HashTable[i]!=NULL)
		{
			POSITION pos=m_HashTable[i]->GetHeadPosition();
			while (pos!=NULL)
			{
				CNode *node=(CNode *)(m_HashTable[i]->GetNext(pos));
				node->SetMarkType(NMT_NOTHING);
			}
		}
	}
}
BOOL CNodeTree::UnmarkQueryAllNodes()
{
	BOOL bReturn=FALSE;
	for (int i=0;i<HASHSIZE;i++)
	{
		if (m_HashTable[i]!=NULL)
		{
			POSITION pos=m_HashTable[i]->GetHeadPosition();
			while (pos!=NULL)
			{
				CNode *node=(CNode *)(m_HashTable[i]->GetNext(pos));
				if (node->MatchesQuery())
				{
					node->SetMatchesQuery(FALSE);
					bReturn=TRUE;
				}
			}
		}
	}
	return bReturn;
}

CNode * CNodeTree::GetMissingSectionNode(int iIndex)
{
	ASSERT(iIndex<GetNumberOfMissingSections());
	POSITION P=m_NodesNotOnSurvey.FindIndex(iIndex);
	return (CNode *)m_NodesNotOnSurvey.GetAt(P);
}

INT_PTR CNodeTree::GetNumberOfMissingSections() const
{
	return m_NodesNotOnSurvey.GetCount();	
}







//This function draws a passage from one point to an other.  It does
//this by calculating cross sections for the two points and then linking
//them.  Cross sections are cubes if we are at a passage intersection or
//they are simple rectangles if we are in a linear passage.  This means
//that linear caves like castleguard tend to look a lot better then caves
//such as jewel cave.  
void CNodeTree::RenderShot(CGWorld *GW,CSurveyShot *pShot)
{
	BOOL bMatchesQuery= ((pShot->m_FromNode->MatchesQuery() && pSettings_G->m_bQueryFrom) ||
					    (pShot->m_ToNode->MatchesQuery() && pSettings_G->m_bQueryTo));

	PASSAGE_TYPE ptDrawType=pSettings_G->m_PassageType[pSettings_G->m_iColorSchemeIndex];
	//Surface surveys are always draw as line plots
	if (pShot->GetShotFlags()&SHOT_SURFACE)
	{
		ptDrawType=PT_LINE_PLOT;
	}
	CSurveyLeg *Leg=pShot->GetLeg();
	int iColorScheme=pSettings_G->m_iColorSchemeIndex;
	if (Leg->GetAssignedColor(iColorScheme)==COLOR_INVISIBLE)
	{
		return;
	}
	CNodePositionInfo * PosFrom=pShot->m_FromNode->GetNodePositionInfo();
	CNodePositionInfo * PosTo=pShot->m_ToNode->GetNodePositionInfo();

	//Calculate the to position again if we are not closing loops.  The reason is that
	//if we have a closure error simply drawing from node to node is going to in effect
	//connect the loop as opposed to showing the error.  Worst of all, this connection
	//will happen no matter how bad the loop closure error is.  Better to have a little
	//extra processing time and do it right.
	//
	CPosMatrix FakeTo;
	CPosMatrix * DrawTo=NULL;
	CPosMatrix *DrawFrom=NULL;
	if (pSettings_G->m_bShowClosedLoops[iColorScheme])
	{
		DrawTo=&PosTo->m_ClosedPosition;
		DrawFrom=&PosFrom->m_ClosedPosition;
	}
	else
	{
		DrawFrom=&PosFrom->m_RawPosition;
		DrawFrom->Add(&FakeTo,pShot->m_Delta);
		DrawTo=&FakeTo;
		if (!FakeTo.IsEqual(PosTo->m_RawPosition))
		{
//Actually, it is better to always add the closure errors
//or we will need to recalculate everything when we toggle
			GW->AddClosureError(PosTo->m_RawPosition,FakeTo);
		}

	}

	//Step 1: Do the From and To junctions
	CJunctionBox junctionFrom;
	CJunctionBox junctionTo;
	//This line is a special line which is used when we are rotating the system to provide
	//a quick wireframe.
	COLORREF crShot;
	if (pSettings_G->m_iColorSchemeIndex!=COLORSCHEME_BLUNDER)
	{
		crShot=Leg->GetDrawColor(pSettings_G->m_iColorSchemeIndex);
	}
	else
	{
		crShot=pSettings_G->GetBlunderColor((int)pShot->GetStressDeviation());
	}
	AddLineToGWorld(GW,crShot,*DrawFrom,*DrawTo,bMatchesQuery,LT_WIREFRAME);
	switch (ptDrawType)
	{
	case PT_LINE_PLOT:
		AddLineToGWorld(GW,crShot,*DrawFrom,*DrawTo,bMatchesQuery,(pShot->GetShotFlags()&SHOT_SURFACE)? LT_SURFACESHOT: LT_SHOT);
		break;
	case PT_WIDTHS:
	case PT_HEIGHTS:
	case PT_CROSS_SECTIONS:
	case PT_FULL_PASSAGES:
	case PT_FULL_PASSAGES_SKINNY:
		{
			//Step one, calculate junction geometry at the from and to positions.
			//In the case of complex junctions the cross section is actually al little bit
			//away from the junction so that we can draw volume around the intersection
			//of the passages by joining cross sections in a clockwise direction.
			if (pShot->m_FromNode->HasSimpleJunction())
			{
				pShot->m_FromNode->CreateSimpleJunctionBoxOutgoing(pShot,&junctionFrom);
				junctionFrom.SetPosition(*DrawFrom);
			}
			else
			{
				pShot->m_FromNode->CreateComplexJunctionBoxOutgoing(pShot,&junctionFrom);

				CPosMatrix newPos;
				newPos.Set(	(DrawTo->GetX()*OUTSIDE_WEIGHT+DrawFrom->GetX()*CENTER_WEIGHT),
							(DrawTo->GetY()*OUTSIDE_WEIGHT+DrawFrom->GetY()*CENTER_WEIGHT),
							(DrawTo->GetZ()*OUTSIDE_WEIGHT+DrawFrom->GetZ()*CENTER_WEIGHT));
				junctionFrom.SetPosition(newPos);
			}

			//TO NODE
			if (pShot->m_ToNode->HasSimpleJunction())
			{
				pShot->m_ToNode->CreateSimpleJunctionBoxIncoming(pShot,&junctionTo);
				junctionTo.SetPosition(*DrawTo);
			}
			else
			{
				pShot->m_ToNode->CreateComplexJunctionBoxIncoming(pShot,&junctionTo);

//fix closure bug
				CPosMatrix newPos;
				newPos.Set(	(DrawTo->GetX()*CENTER_WEIGHT+DrawFrom->GetX()*OUTSIDE_WEIGHT),
							(DrawTo->GetY()*CENTER_WEIGHT+DrawFrom->GetY()*OUTSIDE_WEIGHT),
							(DrawTo->GetZ()*CENTER_WEIGHT+DrawFrom->GetZ()*OUTSIDE_WEIGHT));
				junctionTo.SetPosition(newPos);
			}
		}
		break;
	default:
		ASSERT(FALSE);
	}

	//Step 2:  Draw connections between the junctions, as well as the junctions
	//themselves at the From Station
	switch (ptDrawType)
	{
	case PT_LINE_PLOT:	//already finished in the From section
		break;
	case PT_WIDTHS:
		AddLineToGWorld(GW,crShot,junctionTo.m_Left,junctionFrom.m_Left,bMatchesQuery,LT_PASSAGEWIDTH);
		AddLineToGWorld(GW,crShot,junctionTo.m_Right,junctionFrom.m_Right,bMatchesQuery,LT_PASSAGEWIDTH);
		AddLineToGWorld(GW,crShot,junctionFrom.m_Left,junctionFrom.m_Right,bMatchesQuery,LT_PASSAGEWIDTH);
		AddLineToGWorld(GW,crShot,junctionFrom.m_Middle,junctionTo.m_Middle,bMatchesQuery,LT_SHOT);
		break;
	case PT_HEIGHTS:
		AddLineToGWorld(GW,crShot,junctionTo.m_Top,junctionFrom.m_Top,bMatchesQuery,LT_PASSAGEHEIGHT);
		AddLineToGWorld(GW,crShot,junctionTo.m_Bottom,junctionFrom.m_Bottom,bMatchesQuery,LT_PASSAGEHEIGHT);
		AddLineToGWorld(GW,crShot,junctionFrom.m_Top,junctionFrom.m_Bottom,bMatchesQuery,LT_PASSAGEHEIGHT);
		AddLineToGWorld(GW,crShot,junctionFrom.m_Middle,junctionTo.m_Middle,bMatchesQuery,LT_SHOT);
		break;
	case PT_CROSS_SECTIONS:
		AddLineToGWorld(GW,crShot,junctionFrom.m_TopLeft,junctionFrom.m_TopRight,bMatchesQuery,LT_CROSSSECTION);
		AddLineToGWorld(GW,crShot,junctionFrom.m_TopRight,junctionFrom.m_BottomRight,bMatchesQuery,LT_CROSSSECTION);
		AddLineToGWorld(GW,crShot,junctionFrom.m_BottomRight,junctionFrom.m_BottomLeft,bMatchesQuery,LT_CROSSSECTION);
		AddLineToGWorld(GW,crShot,junctionFrom.m_BottomLeft,junctionFrom.m_TopLeft,bMatchesQuery,LT_CROSSSECTION);
		AddLineToGWorld(GW,crShot,junctionFrom.m_Middle,junctionTo.m_Middle,bMatchesQuery,LT_SHOT);
		break;
	case PT_FULL_PASSAGES:
	case PT_FULL_PASSAGES_SKINNY:
	{
		if (!bMatchesQuery)
		{
			if (pSettings_G->m_iColorSchemeIndex!=COLORSCHEME_DEPTH)
			{
				crShot=Leg->GetDrawColor(pSettings_G->m_iColorSchemeIndex);
			}
			else
			{
				crShot=pSettings_G->GetDepthColor((junctionFrom.m_Middle.GetZ()+junctionTo.m_Middle.GetZ())/2.0f);
			}
		}
		else
		{
			crShot=pSettings_G->m_crQueryColor;
		}
		CGWorldTriangleStrip * pTriangleStrip=new CGWorldTriangleStrip(10,crShot);
		pTriangleStrip->SetVertex(0,junctionFrom.m_TopLeft.GetDirectPointer());
		pTriangleStrip->SetVertex(1,junctionTo.m_TopLeft.GetDirectPointer());
		pTriangleStrip->SetVertex(2,junctionFrom.m_TopRight.GetDirectPointer());
		pTriangleStrip->SetVertex(3,junctionTo.m_TopRight.GetDirectPointer());
		pTriangleStrip->SetVertex(4,junctionFrom.m_BottomRight.GetDirectPointer());
		pTriangleStrip->SetVertex(5,junctionTo.m_BottomRight.GetDirectPointer());
		pTriangleStrip->SetVertex(6,junctionFrom.m_BottomLeft.GetDirectPointer());
		pTriangleStrip->SetVertex(7,junctionTo.m_BottomLeft.GetDirectPointer());
		pTriangleStrip->SetVertex(8,junctionFrom.m_TopLeft.GetDirectPointer());
		pTriangleStrip->SetVertex(9,junctionTo.m_TopLeft.GetDirectPointer());
		pTriangleStrip->SetRotationStripNormals();
		GW->AddTriangleGroup(pTriangleStrip);
	}
		break;
	default:
		ASSERT(FALSE);
	}

//Draw the junction at the other end if it is a complex junction or if
//there are no outgoing shots on the other end.
	if ((!pShot->m_ToNode->HasSimpleJunction()) || 
		(pShot->m_ToNode->GetNumberOfShots(NST_OUTGOINGSHOT)==0 && pShot->m_ToNode->GetShotByIndex(0,NST_INCOMINGSHOT)==pShot))
	{
		switch (pSettings_G->m_PassageType[pSettings_G->m_iColorSchemeIndex])
		{
		case PT_LINE_PLOT:	//already finished in the From section
			break;
		case PT_WIDTHS:
			AddLineToGWorld(GW,crShot,junctionTo.m_Left,junctionTo.m_Right,bMatchesQuery,LT_PASSAGEWIDTH);
			break;
		case PT_HEIGHTS:
			AddLineToGWorld(GW,crShot,junctionTo.m_Top,junctionTo.m_Bottom,bMatchesQuery,LT_PASSAGEHEIGHT);
			break;
		case PT_CROSS_SECTIONS:
			AddLineToGWorld(GW,crShot,junctionTo.m_TopLeft,junctionTo.m_TopRight,bMatchesQuery,LT_CROSSSECTION);
			AddLineToGWorld(GW,crShot,junctionTo.m_TopRight,junctionTo.m_BottomRight,bMatchesQuery,LT_CROSSSECTION);
			AddLineToGWorld(GW,crShot,junctionTo.m_BottomRight,junctionTo.m_BottomLeft,bMatchesQuery,LT_CROSSSECTION);
			AddLineToGWorld(GW,crShot,junctionTo.m_BottomLeft,junctionTo.m_TopLeft,bMatchesQuery,LT_CROSSSECTION);
			break;
		case PT_FULL_PASSAGES:
		case PT_FULL_PASSAGES_SKINNY:
			//No need for anything extra here
			break;
		default:
			ASSERT(FALSE);
		}
	}

	//Finally, draw the junctions.  This is the hardest part of the code.
	//Since multiple shots could be responsible for calling the junction drawing code we
	//adopt the rule that the first outgoing shot for the junction draws the junction,
	//or, if there are no outgoing shots, the first incoming shot does it.
	
	if ((pShot->GetShotFlags()&SHOT_SURFACE)==0)
	{
		//First outgoing shot
		if (!pShot->m_FromNode->HasSimpleJunction() && pShot->m_FromNode->GetShotByIndex(0,NST_OUTGOINGSHOT)==pShot)
		{
			GenerateNodeJunctionGeometry(pShot->m_FromNode,GW,Leg,bMatchesQuery);
		}
		
		//First incoming shot of the other junction
		if (!pShot->m_ToNode->HasSimpleJunction() && pShot->m_ToNode->GetNumberOfShots(NST_OUTGOINGSHOT)==0 && pShot->m_ToNode->GetShotByIndex(0,NST_INCOMINGSHOT)==pShot)
		{
			GenerateNodeJunctionGeometry(pShot->m_ToNode,GW,Leg,bMatchesQuery);
		}
	}
}

void CNodeTree::GenerateNodeJunctionGeometry(CNode *pNode,CGWorld *GW,CSurveyLeg *pLeg,BOOL bMatchesQuery)
{
	if (pSettings_G->m_PassageType[pSettings_G->m_iColorSchemeIndex]==PT_LINE_PLOT)
	{
		return;
	}

	CJunctionGeometryBox junctionBox;

	pNode->CreateComplexJunctionGeometry(&junctionBox,pSettings_G->m_bShowClosedLoops[pSettings_G->m_iColorSchemeIndex]);
	INT_PTR iNumRects=junctionBox.m_junctionBoxArray.GetSize();
	COLORREF crShot;
	if (pSettings_G->m_iColorSchemeIndex!=COLORSCHEME_BLUNDER)
	{
		crShot=pLeg->GetDrawColor(pSettings_G->m_iColorSchemeIndex);
	}
	else
	{
		pSettings_G->GetBlunderColor(0);
	}
	switch (pSettings_G->m_PassageType[pSettings_G->m_iColorSchemeIndex])
	{
	case PT_HEIGHTS:
		{
			for (int i=0;i<iNumRects;i++)
			{
				AddLineToGWorld(GW,crShot,junctionBox.m_junctionBoxArray[i]->m_Top,junctionBox.m_pCentralHeightOnlyJunctionBox->m_Top,bMatchesQuery,LT_PASSAGEHEIGHT);
				AddLineToGWorld(GW,crShot,junctionBox.m_junctionBoxArray[i]->m_Bottom,junctionBox.m_pCentralHeightOnlyJunctionBox->m_Bottom,bMatchesQuery,LT_PASSAGEHEIGHT);
				AddLineToGWorld(GW,crShot,junctionBox.m_junctionBoxArray[i]->m_Middle,junctionBox.m_pCentralHeightOnlyJunctionBox->m_Middle,bMatchesQuery,LT_PASSAGEHEIGHT);
				//Draw the central junction, but do it only once
				if (i==0)
				{
					AddLineToGWorld(GW,crShot,junctionBox.m_pCentralHeightOnlyJunctionBox->m_Top,junctionBox.m_pCentralHeightOnlyJunctionBox->m_Bottom,bMatchesQuery,LT_PASSAGEHEIGHT);
				}
			}
		}
		break;
	case PT_WIDTHS:
		{
			for (int i=0;i<iNumRects;i++)
			{
				AddLineToGWorld(GW,crShot,junctionBox.m_junctionBoxArray[i]->m_Right,junctionBox.m_junctionBoxArray[(i+1)%iNumRects]->m_Left,bMatchesQuery,LT_PASSAGEWIDTH);
				AddLineToGWorld(GW,crShot,junctionBox.m_junctionBoxArray[i]->m_Middle,junctionBox.m_pCentralHeightOnlyJunctionBox->m_Middle,bMatchesQuery,LT_PASSAGEWIDTH);
			}
		}
		break;
	case PT_CROSS_SECTIONS:
		{
			for (int i=0;i<iNumRects;i++)
			{
				AddLineToGWorld(GW,crShot,junctionBox.m_junctionBoxArray[i]->m_Middle,junctionBox.m_pCentralHeightOnlyJunctionBox->m_Middle,bMatchesQuery,LT_CROSSSECTION);
			}
		}
		break;

	case PT_FULL_PASSAGES:
	case PT_FULL_PASSAGES_SKINNY:
		{
			if (!bMatchesQuery)
			{
				if (pSettings_G->m_iColorSchemeIndex!=COLORSCHEME_DEPTH)
				{
					crShot=pLeg->GetDrawColor(pSettings_G->m_iColorSchemeIndex);
				}
				else
				{
					crShot=pSettings_G->GetDepthColor(junctionBox.m_pCentralHeightOnlyJunctionBox->m_Middle.GetZ());
				}
			}
			else
			{
				crShot=pSettings_G->m_crQueryColor;
			}
			CGWorldTriangleFan * pFanTop=new CGWorldTriangleFan(iNumRects*2+2,crShot);
			CGWorldTriangleFan * pFanBottom=new CGWorldTriangleFan(iNumRects*2+2,crShot);
			pFanTop->SetVertex(0,junctionBox.m_pCentralHeightOnlyJunctionBox->m_Top.GetDirectPointer());
			pFanBottom->SetVertex(0,junctionBox.m_pCentralHeightOnlyJunctionBox->m_Bottom.GetDirectPointer());
			float fUpNormal[3]={0.0f,0.0f,1.0f};
			float fDownNormal[3]={-1.0f,0.0f,0.0f};
			//We calculate the top backwards from the bottom to give them both the same
			//winding.  Otherwise our normals will all be backfacing for one set of these and that
			//results in ugly drawing of vertices
			int i;
			for (i=0;i<iNumRects;i++)
			{
				pFanTop->SetVertex(i*2+1,junctionBox.m_junctionBoxArray[iNumRects-1-i]->m_TopRight.GetDirectPointer(),fUpNormal);
				pFanTop->SetVertex(i*2+2,junctionBox.m_junctionBoxArray[iNumRects-1-i]->m_TopLeft.GetDirectPointer(),fUpNormal);
				pFanBottom->SetVertex(i*2+1,junctionBox.m_junctionBoxArray[i]->m_BottomLeft.GetDirectPointer(),fDownNormal);
				pFanBottom->SetVertex(i*2+2,junctionBox.m_junctionBoxArray[i]->m_BottomRight.GetDirectPointer(),fDownNormal);

				//TODO: these things still draw facing the wrong way.
				//I don't know wether this is a vertex problem or a normal problem.
				//Set CULLING on and see what happens to test this.
				CGWorldTriangleStrip * pStrip=new CGWorldTriangleStrip(4,crShot);
				pStrip->SetVertex(0,junctionBox.m_junctionBoxArray[i]->m_TopRight.GetDirectPointer());
				pStrip->SetVertex(1,junctionBox.m_junctionBoxArray[i]->m_BottomRight.GetDirectPointer());
				pStrip->SetVertex(2,junctionBox.m_junctionBoxArray[(i+1)%iNumRects]->m_TopLeft.GetDirectPointer());
				pStrip->SetVertex(3,junctionBox.m_junctionBoxArray[(i+1)%iNumRects]->m_BottomLeft.GetDirectPointer());
				pStrip->SetRectNormals();
				GW->AddTriangleGroup(pStrip);
			}
			pFanTop->SetVertex(i*2+1,junctionBox.m_junctionBoxArray[iNumRects-1]->m_TopRight.GetDirectPointer(),fUpNormal);
			pFanBottom->SetVertex(i*2+1,junctionBox.m_junctionBoxArray[0]->m_BottomLeft.GetDirectPointer(),fDownNormal);

			GW->AddTriangleGroup(pFanTop);
			GW->AddTriangleGroup(pFanBottom);
		}
		break;
	}

}

void CNodeTree::DeleteContents()
{
	for (int i=0;i<HASHSIZE;i++)
	{
		if (m_HashTable[i]!=NULL)
		{
			ASSERT(FALSE);
			//We have already deleted all of the shots, so there should be nothing left in the
			//node tree.
			m_HashTable[i]=NULL;	//don't dare delete it because something is corrupt, but can't leave it here either!
		}
	}
}



BOOL CNodeTree::CloseLoops(int iColorScheme)
{
	if (m_bIsClosureDataDirty[iColorScheme]==FALSE)
		return TRUE;   //THis is too time consuming to do spuriously.

	INT_PTR iShotCount=0;
	for (int i=0;i<HASHSIZE;i++)
	{
		if (m_HashTable[i]!=NULL)
		{
			POSITION pos=m_HashTable[i]->GetHeadPosition();
			while (pos!=NULL)
			{
				CNode *node=(CNode *)(m_HashTable[i]->GetNext(pos));
				//Set constraint bit
				node->GetNodePositionInfo()->m_iNodeType=0;
				if (node->GetMarkType()==NMT_MAINSURVEY)
				{
					iShotCount=iShotCount+node->GetNumberOfShots(NST_OUTGOINGSHOT);
				}
			}
		}
	}

	//The loop closure uses our native structures.  All it requires is an array
	//of pointers to all of our survey shots and a count telling it how many shots there
	//are.  While we are setting up this array, we also mark our fixed positions.
	CSurveyShot ** SI=new CSurveyShot * [iShotCount];
	int iShotIndex=0;
	for (int i=0;i<HASHSIZE;i++)
	{
		if (m_HashTable[i]!=NULL)
		{
			POSITION pos=m_HashTable[i]->GetHeadPosition();
			while (pos!=NULL)
			{
				CNode *node=(CNode *)(m_HashTable[i]->GetNext(pos));
				//Set constraint bit
				node->GetNodePositionInfo()->m_iNodeType=0;
				if (node->GetMarkType()==NMT_MAINSURVEY)
				{
					INT_PTR iNum=node->GetNumberOfShots(NST_OUTGOINGSHOT);
					for (INT_PTR i=0;i<iNum;i++)
					{
						SI[iShotIndex]=node->GetShotByIndex(i,NST_OUTGOINGSHOT);
						iShotIndex++;
					}
				}
			}
		}
	}


	//Set all of the constrained nodes based on the
	//surface data.  The system closes MUCH faster if there are a bunch of
	//constraints set up.
	if (m_MyDocument->GetSurface()->GetSuggestedStartingNode(0)!=NULL)
	{
		m_MyDocument->GetSurface()->GlobalSetSurfacePositions();
	}
	else	//no fixed points, so make up one as best as we can and
			//set it at the center of the surface
	{   
		CNode * node=MakeUpAStartingNode(m_MyDocument->GetSurveyFolder(),iColorScheme);
		if (node==NULL)
		{
			delete[] SI;
			return FALSE;		//nothing to solve
		}
		node->SetConstrainedPosition(0.0f,0.0f,m_MyDocument->GetSurface()->GetMiddleAltitude());
	}
	//END OF CONSTRAINED NODES STUFF

	CWaitForClosureDialog WFCD(iColorScheme,SI,iShotIndex);
	if (WFCD.DoModal()==IDCANCEL)
	{
		delete[] SI;
		return FALSE;
	}
	else
	{
		delete[] SI;
		m_bIsClosureDataDirty[iColorScheme]=FALSE;
		return TRUE;
	}
}




void CNodeTree::GetViewInfo(CNodeTreeSummary *NTS)
{
	int iColorScheme=pSettings_G->m_iColorSchemeIndex;
	BOOL bCloseLoops=pSettings_G->m_bShowClosedLoops[iColorScheme];
	for (int i=0;i<HASHSIZE;i++)
	{
		if (m_HashTable[i]!=NULL)
		{
			POSITION pos=m_HashTable[i]->GetHeadPosition();
			while (pos!=NULL)
			{
				CNode *node=(CNode *)(m_HashTable[i]->GetNext(pos));
				if (node->GetMarkType()==NMT_MAINSURVEY)
				{
					INT_PTR iShots=node->GetNumberOfShots(NST_OUTGOINGSHOT);
					for (INT_PTR i=0;i<iShots;i++)
					{
						CSurveyShot *pShot=node->GetShotByIndex(i,NST_OUTGOINGSHOT);
						if (pShot->GetShotFlags()&SHOT_SURFACE)
						{
							NTS->m_iSurfaceShots++;
							NTS->m_fSurfaceLength+=pShot->GetCalculationLength();
						}
						else
						{
							NTS->m_iCaveShots++;
							NTS->m_fCaveLength+=pShot->GetCalculationLength();
							NTS->m_fHorizontalLength+=pShot->GetHorizontalLength();
						}
					}
					if (!node->IsSurfaceNode())
					{
						CPosMatrix *Pos=node->GetPosition(bCloseLoops);
						if (Pos->GetZ()<NTS->m_fDeepest)
						{
							NTS->m_fDeepest=Pos->GetZ();
							lstrcpyn(NTS->m_szDeepest,node->GetName(),MAX_STATION_NAME_LENGTH-1);
						}
						if (Pos->GetZ()>NTS->m_fHighest)
						{
							NTS->m_fHighest=Pos->GetZ();
							lstrcpyn(NTS->m_szHighest,node->GetName(),MAX_STATION_NAME_LENGTH-1);
						}
						if (Pos->GetX()<NTS->m_fLeft)
						{
							NTS->m_fLeft=Pos->GetX();
						}
						if (Pos->GetX()>NTS->m_fRight)
						{
							NTS->m_fRight=Pos->GetX();
						}
						if (Pos->GetY()<NTS->m_fTop)
						{
							NTS->m_fTop=Pos->GetY();
						}
						if (Pos->GetY()>NTS->m_fBottom)
						{
							NTS->m_fBottom=Pos->GetY();
						}
					}
				}				
			}
		}
	}
	if (NTS->m_fHighest<NTS->m_fDeepest)
	{
		NTS->m_fHighest=0.0f;
		NTS->m_fDeepest=0.0f;
	}
}


CNodeTreeSummary::~CNodeTreeSummary()
{
};

CNodeTreeSummary::CNodeTreeSummary()
{
	m_fHighest=-10000.0f;
	m_fDeepest=10000.0f;
	m_szDeepest[0]=_T('\0');
	m_szHighest[0]=_T('\0');
	m_fCaveLength=0.0f;
	m_fHorizontalLength=0.0f;
	m_fSurfaceLength=0.0f;
	m_iCaveShots=0;
	m_iSurfaceShots=0;
	m_fLeft=100000.0f;
	m_fRight=-100000.0f;
	m_fTop=100000.0f;
	m_fBottom=-100000.0f;
	m_fViewDeepest=10000.0f;
	m_fViewHighest=-10000.0f;
};


void CNodeTree::AddBlundersToListBox(CListBox * pList,int iDeviation)
{
	float fDeviation = (float) iDeviation;
	for (int i=0;i<HASHSIZE;i++)
	{
		if (m_HashTable[i]!=NULL)
		{
			POSITION pos=m_HashTable[i]->GetHeadPosition();
			while (pos!=NULL)
			{
				CNode *node=(CNode *)(m_HashTable[i]->GetNext(pos));
				if (node->GetMarkType()==NMT_MAINSURVEY)
				{
					INT_PTR iShots=node->GetNumberOfShots(NST_OUTGOINGSHOT);
					for (INT_PTR i=0;i<iShots;i++)
					{
						CSurveyShot *pShot=node->GetShotByIndex(i,NST_OUTGOINGSHOT);
						float fDevThisShot=pShot->GetStressDeviation();
						if (fDevThisShot>99.0f) {fDevThisShot = 99.0f;}
						if (fDevThisShot>fDeviation)
						{
							CString csAdd;
							csAdd.Format(_T("% 5.1f devs.   %s -  %s")
								          ,fDevThisShot
													,pShot->GetFromStationName()
													,pShot->GetToStationName());
							int iIndex=pList->AddString(csAdd);
							pList->SetItemData(iIndex,(long)(void *)(pShot));
						}
					}
				}
			}
		}
	}
}

void CNodeTree::SetBlunderMode(int iMode)
{
	m_iBlunderMode = iMode;
}

int CNodeTree::GetBlunderMode()
{
	return m_iBlunderMode;
}
