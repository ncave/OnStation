#include "stdafx.h"
#include "junctionbox.h"

CJunctionBox::CJunctionBox()
{
}


void CJunctionBox::GenerateCrossSection(float fCompassAngle,float fClinoAngle,float fUp,float fDown,float fLeft,float fRight)
{
	//convert to radians
	float fCompassAngleRadians=fCompassAngle*(float)PI/180.0f;
	float fClinoAngleRadians=fClinoAngle*(float)PI/180.0f;

	//first calculate the X and Y proportions of a vector to the left.
	//CompassAngle-90 gives the angle to the left.
	float fYMultiplier=(float)cos(fCompassAngleRadians-(float)PI/2.0f);
	float fXMultiplier=(float)sin(fCompassAngleRadians-(float)PI/2.0f);

	//next calculate the height difference.
	float fHeightMultiplier=(float)cos(fClinoAngleRadians);
	float fAdjustedUp=fUp*fHeightMultiplier;
	float fAdjustedDown=fDown*fHeightMultiplier;

	//Finally, since the height vectors end up being "tilted", calculate the X and Y
	//impact of this tilt
	float fDistanceXYMultiplier;
	float fYMultiplierForZ;
	float fXMultiplierForZ;
	//Start to tilt the readings only at sharp angles
	if (fClinoAngle<65.0 && fClinoAngle>-65.0)
	{
		fDistanceXYMultiplier=0.0f;
		fXMultiplierForZ=0.0f;
		fYMultiplierForZ=0.0f;
	}
	else
	{
		fDistanceXYMultiplier=(float)sin(fClinoAngleRadians);
		fYMultiplierForZ=(float)cos(fCompassAngleRadians)*fDistanceXYMultiplier;
		fXMultiplierForZ=(float)sin(fCompassAngleRadians)*fDistanceXYMultiplier;
	}
	
	m_Left.Set(fXMultiplier*fLeft,fYMultiplier*fLeft,0.0f);
	m_Right.Set(-fXMultiplier*fRight,-fYMultiplier*fRight,0.0f);

	m_TopLeft.Set(fXMultiplier*fLeft-fUp*fXMultiplierForZ,fYMultiplier*fLeft-fUp*fYMultiplierForZ,fAdjustedUp);
	m_TopRight.Set(-fXMultiplier*fRight-fUp*fXMultiplierForZ,-fYMultiplier*fRight-fUp*fYMultiplierForZ,fAdjustedUp);
	m_BottomLeft.Set(fXMultiplier*fLeft+fDown*fXMultiplierForZ,fYMultiplier*fLeft+fDown*fYMultiplierForZ,-fAdjustedDown);
	m_BottomRight.Set(-fXMultiplier*fRight+fDown*fXMultiplierForZ,-fYMultiplier*fRight+fDown*fYMultiplierForZ,-fAdjustedDown);

	m_Top.Set(-fUp*fXMultiplierForZ,-fUp*fYMultiplierForZ,fAdjustedUp);
	m_Bottom.Set(fDown*fXMultiplierForZ,fDown*fYMultiplierForZ,-fAdjustedDown);
	m_Middle.Set(0.0f,0.0f,0.0f);

}

void CJunctionBox::SwapSides()
{
	CPosMatrix temp=m_TopLeft;
	m_TopLeft=m_TopRight;
	m_TopRight=temp;
	temp=m_BottomLeft;
	m_BottomLeft=m_BottomRight;
	m_BottomRight=temp;
	temp=m_Left;
	m_Left=m_Right;
	m_Right=temp;
}
#ifdef _DEBUG
void CJunctionBox::Test()
{
//This test became invalid when we changed junction box  behaviour
//to not tilt except for steep angles.

	/*
	CJunctionBox testBox;
	testBox.GenerateCrossSection(0.0f,0.0f,1.0f,2.0f,3.0f,4.0f);
	ASSERT(testBox.m_TopLeft.GetX()==-3.0f);
	ASSERT(testBox.m_TopRight.GetX()==4.0f);
	ASSERT(((int)testBox.m_TopLeft.GetY())==0);
	ASSERT(((int)testBox.m_TopRight.GetY())==0);
	ASSERT(testBox.m_TopRight.GetZ()==1.0f);
	ASSERT(testBox.m_TopLeft.GetZ()==1.0f);
	ASSERT(testBox.m_BottomLeft.GetZ()==-2.0f);
	ASSERT(testBox.m_BottomRight.GetX()==4.0f);
	testBox.SwapSides();
	ASSERT(testBox.m_BottomLeft.GetX()==4.0f);
	ASSERT(testBox.m_TopRight.GetX()==-3.0f);

	testBox.GenerateCrossSection(180.0f,0.0f,1.0f,2.0f,3.0f,4.0f);
	ASSERT(testBox.m_TopLeft.GetX()==3.0f);
	ASSERT(testBox.m_TopRight.GetX()==-4.0f);
	ASSERT(((int)testBox.m_TopLeft.GetY())==0);
	ASSERT(((int)testBox.m_TopRight.GetY())==0);
	ASSERT(testBox.m_TopRight.GetZ()==1.0f);
	ASSERT(testBox.m_TopLeft.GetZ()==1.0f);
	ASSERT(testBox.m_BottomLeft.GetZ()==-2.0f);
	ASSERT(testBox.m_BottomRight.GetX()==-4.0f);

	testBox.GenerateCrossSection(90.0f,0.0f,1.0f,2.0f,3.0f,4.0f);
	ASSERT(((int)testBox.m_TopLeft.GetX())==0.0f);
	ASSERT(((int)testBox.m_TopRight.GetX())==0.0f);
	ASSERT(testBox.m_TopLeft.GetY()==3.0f);
	ASSERT(testBox.m_TopRight.GetY()==-4.0f);
	ASSERT(testBox.m_TopRight.GetZ()==1.0f);
	ASSERT(testBox.m_TopLeft.GetZ()==1.0f);
	ASSERT(testBox.m_BottomLeft.GetZ()==-2.0f);
	ASSERT(((int)testBox.m_BottomRight.GetX())==0);

	float fCompare=(float)(sqrt(2.0)/2.0);
	testBox.GenerateCrossSection(0.0f,45.0f,1.0f,2.0f,3.0f,4.0f);
	ASSERT(testBox.m_TopLeft.GetX()==-3.0f);
	ASSERT(testBox.m_TopRight.GetX()==4.0f);
	ASSERT((int)(testBox.m_TopLeft.GetY()*10.0f)==((int)(-1.0f*fCompare*10.0f)));
	ASSERT((int)(testBox.m_TopRight.GetY()*10.0f)==((int)(-1.0f*fCompare*10.0f)));
	ASSERT((int)(testBox.m_TopRight.GetZ()*10.0f)==((int)(1.0f*fCompare*10.0f)));
	ASSERT((int)(testBox.m_TopLeft.GetZ()*10.0f)==((int)(1.0f*fCompare*10.0f)));
	ASSERT(testBox.m_BottomLeft.GetZ()==-2.0f*fCompare);
	ASSERT(testBox.m_BottomRight.GetZ()==-2.0f*fCompare);
	ASSERT(((int)(testBox.m_BottomLeft.GetY()*10.0f))==((int)(2.0f*fCompare*10.0f)));
	ASSERT(((int)(testBox.m_BottomRight.GetY()*10.0f))==((int)(2.0f*fCompare*10.0f)));
	ASSERT(testBox.m_BottomLeft.GetX()==-3.0f);
	ASSERT(testBox.m_BottomRight.GetX()==4.0f);

	testBox.GenerateCrossSection(90.0f,-45.0f,1.0f,2.0f,3.0f,4.0f);
	ASSERT(((int)(testBox.m_TopLeft.GetX()*10.0f))==((int)(1.0f*fCompare*10.0f)));
	ASSERT(((int)(testBox.m_TopRight.GetX()*10.0f))==((int)(1.0f*fCompare*10.0f)));
	ASSERT(testBox.m_TopLeft.GetY()==3.0f);
	ASSERT(testBox.m_TopRight.GetY()==-4.0f);
	ASSERT(testBox.m_TopRight.GetZ()==1.0f*fCompare);
	ASSERT(testBox.m_TopLeft.GetZ()==1.0f*fCompare);
	ASSERT(testBox.m_BottomLeft.GetZ()==-2.0f*fCompare);
	ASSERT(testBox.m_BottomRight.GetZ()==-2.0f*fCompare);
	ASSERT(testBox.m_BottomLeft.GetY()==3.0f);
	ASSERT(testBox.m_BottomRight.GetY()==-4.0f);
	ASSERT(((int)(testBox.m_BottomLeft.GetX()*10.0f))==((int)(-2.0f*fCompare*10.0f)));
	ASSERT(((int)(testBox.m_BottomRight.GetX()*10.0f))==((int)(-2.0f*fCompare*10.0f)));
	testBox.GenerateCrossSection(90.0f,90.0f,1.0f,2.0f,3.0f,4.0f);
	testBox.GenerateCrossSection(145.0f,45.0f,1.0f,2.0f,3.0f,4.0f);
	testBox.GenerateCrossSection(90.0f,90.0f,1.0f,2.0f,3.0f,4.0f);
*/
}
#endif

void CJunctionBox::SetPosition(CPosMatrix& pPosition)
{
	m_TopLeft.Add(&m_TopLeft,pPosition);
	m_TopRight.Add(&m_TopRight,pPosition);
	m_BottomLeft.Add(&m_BottomLeft,pPosition);
	m_BottomRight.Add(&m_BottomRight,pPosition);
	m_Left.Add(&m_Left,pPosition);
	m_Right.Add(&m_Right,pPosition);
	m_Top.Add(&m_Top,pPosition);
	m_Bottom.Add(&m_Bottom,pPosition);
	m_Middle.Add(&m_Middle,pPosition);
}




CJunctionGeometryBox::CJunctionGeometryBox()
{
	m_pCentralHeightOnlyJunctionBox=NULL;
}

CJunctionGeometryBox::~CJunctionGeometryBox()
{
	delete m_pCentralHeightOnlyJunctionBox;
	m_pCentralHeightOnlyJunctionBox=NULL;
	for (int i=0;i<m_junctionBoxArray.GetSize();i++)
	{
		delete m_junctionBoxArray[i];
		m_junctionBoxArray[i]=NULL;
	}
}

