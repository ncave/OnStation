// (C) Copyright 1995, Taco van Ieperen

#include "stdafx.h"
#include "onstation.h"
#include "surfacedata.h"
#include "nodetree.h"
#include "node.h"
#include "OnStationDoc.h"
#include "folder.h"
#include "surveyleg.h"
#include "filehelper.h"
#include "fixedpoint.h"
#include "realfolder.h"
#include "settings.h"
#include "colortool.h"	//for COLOR_GRAY definition

extern CSettings * pSettings_G;
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSurfaceData::CSurfaceData(COnStationDoc * document,float fGridNorth,float left,float top,float right,float bottom,float size)
{
	m_MyDocument=document;
	m_Altitudes=NULL;
	m_fLeft=NAN;	//To make sure old size doesn't match in SetDimensions
	SetGridNorth(fGridNorth);
	SetDimensions(left,top,right,bottom,size);
}

CSurfaceData::~CSurfaceData()
{
	delete[] m_Altitudes;
	m_Altitudes=NULL;
	RemoveAllFixedPoints();
}

void CSurfaceData::RemoveAllFixedPoints()
{
	while (GetFixedPointCount()!=0)
	{
		CFixedPoint *TI=GetFixedPoint(0);
		RemoveFixedPoint(TI);
		delete TI;
	}
}

void CSurfaceData::ResetAll()
{
	DeleteContents();
	SetGridNorth(0.0f);
	SetDimensions(DEF_SLEFT,DEF_STOP,DEF_SRIGHT,DEF_SBOTTOM,DEF_SSIZE);
}

void CSurfaceData::DeleteContents()
{
	delete[] m_Altitudes;
	m_Altitudes=NULL;
	while (GetFixedPointCount()!=0)
	{
		CFixedPoint *TI=GetFixedPoint(0);
		RemoveFixedPoint(TI);
		delete TI;
	}
	//Make sure we don't actually match sizes on the SetDimensions member call
	m_fLeft=NAN;
	SetDimensions(DEF_SLEFT,DEF_STOP,DEF_SRIGHT,DEF_SBOTTOM,DEF_SSIZE);
	SetGridNorth(0.0f);
}

void CSurfaceData::SetGridNorth(float fGridNorth)
{
	m_fGridNorth=fGridNorth;
}

void CSurfaceData::SetDimensions(float left,float top,float right,float bottom,float size)
{
	ASSERT(AfxCheckMemory());

	if (left==m_fLeft && right==m_fRight && top==m_fTop && bottom==m_fBottom && size==m_fSquareSize)
		return;

	ASSERT(left<right);
	ASSERT(bottom<top);
	ASSERT(size!=0);
	int iX=(int)((right-left)/size)+1;
	ASSERT(iX<5000 && iX>0);
	int iY=(int)((top-bottom)/size)+1;
	ASSERT(iY<5000 && iY>0);

	//Make sure grid is divided evenly
	ASSERT( ((right-left)-((float)(iX-1)*size) ) ==0.0f );
	ASSERT( ((top-bottom)-((float)(iY-1)*size) ) ==0.0f );

	delete[] m_Altitudes;
	m_Altitudes=new float[iX*iY];

	ASSERT(AfxCheckMemory());

	for (int x=0;x<iX;x++)
	{
		//Calculated all the intermediate altitudes if it is possible
		for (int y=0;y<iY;y++)
		{
			m_Altitudes[x+y*iX]=NAN;
		}
	}

	m_iGridWidth=iX;
	m_iGridHeight=iY;
	m_fLeft=left;
	m_fRight=right;
	m_fBottom=bottom;
	m_fTop=top;
	m_fSquareSize=size;

	ASSERT(AfxCheckMemory());
}

BOOL CSurfaceData::IsEmpty()
{
	for (int x=0;x<m_iGridWidth;x++)
	{
		//Calculated all the intermediate altitudes if it is possible
		for (int y=0;y<m_iGridHeight;y++)
		{
			if (m_Altitudes[x+y*m_iGridWidth]!=NAN)
				return FALSE;
		}
	}
	return TRUE;
}

float CSurfaceData::ConvertXToUTM(float x)
{
	ASSERT(x!=NAN && m_fLeft!=NAN && m_fRight!=NAN);
	return (m_fRight+m_fLeft)/2+x;
}
float CSurfaceData::ConvertYToUTM(float y)
{
	ASSERT(y!=NAN && m_fBottom!=NAN && m_fTop!=NAN);
	return (m_fTop+m_fBottom)/2+y;
}
float CSurfaceData::GetMiddleAltitude()
{
	float f;
	f=GetAltitude(m_iGridWidth/2,m_iGridHeight/2);
	if (f==NAN)
	{
		f=0.0f;
	}
	return f;
}


float CSurfaceData::ConvertXToLocal(float x)
{
	ASSERT(x!=NAN && m_fLeft!=NAN && m_fRight!=NAN);
	return x-(m_fRight+m_fLeft)/2;
}
float CSurfaceData::ConvertYToLocal(float y)
{
	ASSERT(y!=NAN && m_fBottom!=NAN && m_fTop!=NAN);
	return y-(m_fTop+m_fBottom)/2;
}
/*
float CSurfaceData::GetAltitude(float x,float y)
{
	ASSERT(x>=m_fLeft && x<=m_fRight);
	ASSERT(y<=m_fTop && y>=m_fBottom);

	int iX=(int)((x-m_fLeft)/m_fSquareSize);
	int iY=(int)((y-m_fBottom)/m_fSquareSize);
	return GetAltitude(iX,iY);
}
*/

float CSurfaceData::GetAltitude(int iX,int iY)
{
//	ASSERT(iX>=0 && iX<m_iGridWidth);
//	ASSERT(iY>=0 && iY<m_iGridHeight);
	if (m_iGridWidth==0 || m_iGridHeight==0)
	{
		return 0.0f;
	}
	int iIndex=iX+iY*m_iGridWidth;
	return m_Altitudes[iIndex];
}

void CSurfaceData::SetAltitude(int iXIndex,int iYIndex,float z)
{
	ASSERT(iXIndex>=0 && iXIndex<m_iGridWidth);
	ASSERT(iYIndex>=0 && iYIndex<m_iGridHeight);
	int iIndex=iXIndex+iYIndex*m_iGridWidth;
	m_Altitudes[iIndex]=z;
}
void CSurfaceData::SetAltitude(float x,float y,float z)
{
	ASSERT(x>=m_fLeft && x<=m_fRight);
	ASSERT(y<=m_fTop && y>=m_fBottom);

	int iX=(int)((x-m_fLeft)/m_fSquareSize);
	int iY=(int)((y-m_fBottom)/m_fSquareSize);
	SetAltitude(iX,iY,z);
}


void CSurfaceData::CalculateGWorld(CGWorld *World)
{
	float fXMultiple=(float)sin(m_fGridNorth*PI/180.0);
	float fYMultiple=(float)cos(m_fGridNorth*PI/180.0);
	int x,y;
	for (x=0;x<m_iGridWidth;x++)
	{
		for (y=0;y<m_iGridHeight;y++)
		{
			if (GetAltitude(x,y)!=NAN)
			{
				CPosMatrix Mid;
				float fYtemp=((float)y*m_fSquareSize);
				float fXtemp=((float)x*m_fSquareSize);
				float fY=m_fTop-(fYtemp*fYMultiple-fXtemp*fXMultiple);
				float fX=m_fLeft+(fYtemp*fXMultiple+fXtemp*fYMultiple);
				Mid.Set(ConvertXToLocal(fX),ConvertYToLocal(fY),GetAltitude(x,y));
				if ((y<m_iGridHeight-1) && (GetAltitude(x,y+1)!=NAN))
				{
					float fYtemp=(float)(y+1)*m_fSquareSize;
					float fXtemp=(float)x*m_fSquareSize;
					float fY=m_fTop-(fYtemp*fYMultiple-fXtemp*fXMultiple);
					float fX=m_fLeft+(fYtemp*fXMultiple+fXtemp*fYMultiple);
					CPosMatrix Down;
					Down.Set(ConvertXToLocal(fX),ConvertYToLocal(fY),GetAltitude(x,y+1));
//					TRACE(_T("North %f = Drawing %f,%f to %f,%f\n"),m_fGridNorth,Mid.GetX(),Mid.GetY(),Down.GetX(),Down.GetY());
					COLORREF crColor=pSettings_G->m_cSurfaceColors[pSettings_G->m_iColorSchemeIndex];
					if (crColor!=COLOR_INVISIBLE && crColor!=COLOR_INACTIVE)
					{
						World->AddLineSegment(Mid,Down,crColor,LT_SURFACE);
					}
				}
				if ((x<m_iGridWidth-1) && (GetAltitude(x+1,y)!=NAN))
				{
					float fYtemp=(float)(y)*m_fSquareSize;
					float fXtemp=(float)(x+1)*m_fSquareSize;
					float fY=m_fTop-(fYtemp*fYMultiple-fXtemp*fXMultiple);
					float fX=m_fLeft+(fYtemp*fXMultiple+fXtemp*fYMultiple);
					CPosMatrix Right;
					Right.Set(ConvertXToLocal(fX),ConvertYToLocal(fY),GetAltitude(x+1,y));
					COLORREF crColor=pSettings_G->m_cSurfaceColors[pSettings_G->m_iColorSchemeIndex];
					if (crColor!=COLOR_INVISIBLE && crColor!=COLOR_INACTIVE)
					{
						World->AddLineSegment(Mid,Right,crColor,LT_SURFACE);
					}
				}				
			}
		}
	}

	//This is here for the constrained node boxes that we draw.  Since we
	//must know where they are we add them here.
	//Put all the constrained nodes into the GWorld
	int i;
	for (i=0;i<GetFixedPointCount();i++)
	{
		CFixedPoint * STI=GetFixedPoint(i);
		CNode *node=m_MyDocument->GetNodeTree()->FindNode(STI->GetName(),FALSE);
		//The second test is so that we don't draw nodes which are
		//marked as disabled!
		if (node!=NULL && node->GetNumberOfShots(NST_ANYSHOT))
		{
			TRACE(_T("Add Constraint %s\n"),node->GetName());

			CPosMatrix *Pos=node->GetPosition(pSettings_G->m_bShowClosedLoops[pSettings_G->m_iColorSchemeIndex]);

			TRACE(_T("        %i %i %i\n"),(int)Pos->GetX(),(int)Pos->GetY(),(int)Pos->GetZ());

			World->AddConstraint(*Pos,0,node,FALSE);
		}
	}
	//Add this guy as a constraint if there are no others.  This
	//is feedback that the system does have a starting point and
	//and that maybe one should be set if this is not correct.
	if (i==0)
	{
		CNode *node=m_MyDocument->GetNodeTree()->MakeUpAStartingNode(m_MyDocument->GetSurveyFolder(),pSettings_G->m_iColorSchemeIndex);
		if (node!=NULL)
		{
			CPosMatrix *Pos=node->GetPosition(pSettings_G->m_bShowClosedLoops[pSettings_G->m_iColorSchemeIndex]);

			TRACE(_T("        %i %i %i\n"),(int)Pos->GetX(),(int)Pos->GetY(),(int)Pos->GetZ());

			World->AddConstraint(*Pos,0,node,FALSE);
		}
	}
}


void CSurfaceData::Write(CFileHelper * h)
{
	CString sz;

	h->WriteToken(_T("Begin"), _T("SurfaceData"));

	h->WriteToken(_T("SurfaceSouthCorner"),m_fBottom);
	h->WriteToken(_T("SurfaceWestCorner"),m_fLeft);
	h->WriteToken(_T("SurfaceGridSquareSize"),m_fSquareSize);
	h->WriteToken(_T("SurfaceGridEntriesNS"),m_iGridHeight);
	h->WriteToken(_T("SurfaceGridEntriesEW"),m_iGridWidth);

	h->WriteToken(_T("Declination"),m_fGridNorth);

	h->WriteToken(_T("Begin"), _T("SurfaceHeights"));
	for (int y=0;y<m_iGridHeight;y++)
	{
		h->WritePartialToken(_T("SurfaceHeights"));
		for (int x=0;x<m_iGridWidth;x++)
		{
			h->AppendFloat(GetAltitude(x,y));
		}
		h->WriteEndOfLine();
	}
	h->WriteToken(_T("End"), _T("SurfaceHeights"));
	h->WriteToken(_T("End"), _T("SurfaceData"));
}

void CSurfaceData::WriteFixedPoints(CFileHelper *h)
{
	h->WriteToken(_T("Begin"), _T("Constrained Stations"));
	for (int i=0;i<GetFixedPointCount();i++)
	{
		CFixedPoint *T=GetFixedPoint(i);
		T->Write(h);
	}
	h->WriteToken(_T("End"), _T("Constrained Stations"));
}


void CSurfaceData::ReadSurface(CFileHelper * h)
{
	h->ReadTokenBegin(_T("SurfaceData"));

	m_fBottom=h->ReadTokenFloat(_T("SurfaceSouthCorner"));
	m_fLeft=h->ReadTokenFloat(_T("SurfaceWestCorner"));
	m_fSquareSize=h->ReadTokenFloat(_T("SurfaceGridSquareSize"));
	m_iGridHeight=h->ReadTokenInt(_T("SurfaceGridEntriesNS"));
	m_iGridWidth=h->ReadTokenInt(_T("SurfaceGridEntriesEW"));
	m_fGridNorth=h->ReadTokenFloat(_T("Declination"));
	m_fTop=m_fBottom+m_fSquareSize*(float)(m_iGridHeight-1);
	m_fRight=m_fLeft+m_fSquareSize*(float)(m_iGridWidth-1);
	delete[] m_Altitudes;
	m_Altitudes=new float[m_iGridWidth*m_iGridHeight];

	h->ReadTokenBegin(_T("SurfaceHeights"));
	for (int y=0;y<m_iGridHeight;y++)
	{
		h->ReadPartialToken(_T("SurfaceHeights"));
		for (int x=0;x<m_iGridWidth;x++)
		{
			SetAltitude(x,y,h->ReadAppendedFloat());
		}
 	}
	h->ReadTokenEnd(_T("SurfaceHeights"));
	h->ReadTokenEnd(_T("SurfaceData"));
}

void CSurfaceData::ReadFixedPoints(CFileHelper *h)
{
	h->ReadTokenBegin(_T("Constrained Stations"));
	LPCTSTR szData=h->GetTokenLookaheadString(TRUE);
	while (lstrcmpi(szData, _T("StationName"))==0)
	{
		CFixedPoint *T=new CFixedPoint(_T(""));
		T->Read(h);
		AddFixedPoint(T);
		szData=h->GetTokenLookaheadString(TRUE);
	}
	h->ReadTokenEnd(_T("Constrained Stations"));
}

//Calculates the alitutude at a given point.  This function currently
//only does this for points which are "in-line" with the existing data.
//It does not interpolate into the center of a square.  Returns FALSE
//if the square was right out to lunch.
BOOL CSurfaceData::CalculateAltitude(float x,float y,float * result)
{
	if (x<m_fLeft || x>m_fRight || y>m_fTop || y<m_fBottom)
	{
		*result=NAN;
		return FALSE;
	}
	//iX and iY are grid locations surrounding this point
	int iX=(int)((x-m_fLeft)/m_fSquareSize);
	int iY=(int)((m_fTop-y)/m_fSquareSize);
	int iDeltaX=((int)(x-m_fLeft)-((int)m_fSquareSize*iX));
	int iDeltaY=((int)(m_fTop-y)-((int)m_fSquareSize*iY));
	float fTopCorner=GetAltitude(iX,iY);
	if (fTopCorner==NAN)
	{
		*result=NAN;
		return TRUE;
	}
	if (iDeltaX==0 && iDeltaY==0)	//right on a corner
	{
		*result=fTopCorner;
		return TRUE;
	}
	if (iDeltaX==0)
	{
		float fOtherCorner=GetAltitude(iX,iY+1);
		if (fOtherCorner!=NAN)
		{
			*result=fTopCorner-(fTopCorner-fOtherCorner)*((float)iDeltaY/m_fSquareSize);
		}
		else
		{
			*result=NAN;
		}
		return TRUE;
	}
	if (iDeltaY==0)
	{
		float fOtherCorner=GetAltitude(iX+1,iY);
		if (fOtherCorner!=NAN)
		{
			*result=fTopCorner-(fTopCorner-fOtherCorner)*((float)iDeltaX/m_fSquareSize);
		}
		else
		{
			*result=NAN;
		}
		return TRUE;
	}
	*result=NAN;
	return FALSE;
}



CFixedPoint * CSurfaceData::GetFixedPoint(INT_PTR i)
{	
	POSITION Pos=m_FixedPointList.FindIndex(i);
	if (Pos!=NULL)
		return (CFixedPoint *)m_FixedPointList.GetAt(Pos);
	else
		return NULL;
}

INT_PTR CSurfaceData::GetFixedPointCount()
{
	return m_FixedPointList.GetCount();
}

void CSurfaceData::RemoveFixedPoint(CFixedPoint *ptr)
{
	POSITION Pos=NULL;
	POSITION Pos2=m_FixedPointList.Find((LPVOID)ptr,Pos);
	if (Pos2!=NULL)
	{
		m_FixedPointList.RemoveAt(Pos2);
	}
}

BOOL CSurfaceData::AddFixedPoint(CFixedPoint *ptr)
{
	for (int i=0;i<GetFixedPointCount();i++)
	{
		CFixedPoint * T=GetFixedPoint(i);
		//Does it exist already?
		if (lstrcmpi(ptr->GetName(),T->GetName())==0)
		{
			delete ptr;
			return FALSE;
		}
	}		
	m_FixedPointList.AddTail((LPVOID)ptr);
	return TRUE;
}


BOOL CSurfaceData::IsConstrainedNode(LPCTSTR szName,int iColorScheme)
{
	if (m_FixedPointList.GetCount()==0)
	{
		CNode *node=m_MyDocument->GetNodeTree()->MakeUpAStartingNode(m_MyDocument->GetSurveyFolder(),iColorScheme);
		if (node!=NULL)
		{
			return (lstrcmpi(node->GetName(),szName)==0);
		}
		else
		{
			return FALSE;
		}
	}

	for (int i=0;i<m_FixedPointList.GetCount();i++)
	{
		CFixedPoint * TI=GetFixedPoint(i);
		if (lstrcmpi(TI->GetName(),szName)==0)
			return TRUE;
	}
	return FALSE;
}


//Sets the positions for all of the stations in the tie in list.
//Also marks those shots as being fixed points.
void CSurfaceData::GlobalSetSurfacePositions()
{
 	for (int i=0;i<GetFixedPointCount();i++)
	{
		CFixedPoint * STI=GetFixedPoint(i);
		CNode *node=m_MyDocument->GetNodeTree()->FindNode(STI->GetName(),FALSE);
		if (node!=NULL)
		{
			CPosMatrix m;
			node->SetConstrainedPosition(ConvertXToLocal(STI->GetX()),ConvertYToLocal(STI->GetY()),STI->GetZ());
		}
		else
		{
			ASSERT(AfxCheckMemory());
			if (!STI->GetWarnedMissing())
			{
				CString CS;
				AfxFormatString1(CS,IDS_TIEINWASREMOVED,STI->GetName());
				AfxMessageBox(CS,MB_OK);
			}
		}
		STI->SetWarnedMissing(node==NULL);
	}
}

CNode *CSurfaceData::GetSuggestedStartingNode(int iIndex)
{
	INT_PTR iCount=GetFixedPointCount();
	for (INT_PTR i=0;i<iCount;i++)
	{
		CFixedPoint * STI=GetFixedPoint(i);
		CNode *node=m_MyDocument->GetNodeTree()->FindNode(STI->GetName(),FALSE);
		if (node!=NULL && node->GetNumberOfShots(NST_ANYSHOT))
		{
			iIndex--;
			if (iIndex==-1)
			{
				node->SetConstrainedPosition(ConvertXToLocal(STI->GetX()),ConvertYToLocal(STI->GetY()),STI->GetZ());
				return node;
			}
		}
	}
	return NULL;
}

void CSurfaceData::GetViewInfo(CNodeTreeSummary *NTS)
{
	if (m_fLeft<NTS->m_fLeft)
	{
		NTS->m_fLeft=m_fLeft;
	}
	if (m_fRight>NTS->m_fRight)
	{
		NTS->m_fRight=m_fRight;
	}
	if (m_fTop>NTS->m_fTop)
	{
		NTS->m_fTop=m_fTop;
	}
	if (m_fBottom<NTS->m_fBottom)
	{
		NTS->m_fBottom=m_fBottom;
	}

	int iX=(int)((m_fRight-m_fLeft)/m_fSquareSize)+1;
	int iY=(int)((m_fTop-m_fBottom)/m_fSquareSize)+1;
	for (int x=0;x<iX;x++)
	{
		for (int y=0;y<iY;y++)
		{
			float fZ=m_Altitudes[x+y*iY];
			if (fZ!=NAN)
			{
				if (fZ>NTS->m_fHighest)
				{
					NTS->m_fHighest=fZ;
				}
				if (fZ<NTS->m_fDeepest)
				{
					NTS->m_fDeepest=fZ;
				}
			}
		}
	}
}

						