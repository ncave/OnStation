// (C) Copyright 1994- 1996    Taco van Ieperen  (tacovan@smarttech.com)

#ifndef I_JUNCTIONBOX__
#define I_JUNCTIONBOX__
#include "afxtempl.h"

class CJunctionBox:public CObject
{
//All of the positions below are "0 based" meaning that they
//are offsets from the actual node position rather then the
//cross section itself.  This provides greater generality.
public:
	CJunctionBox();

#ifdef _DEBUG
	static void Test();
#endif
	CPosMatrix m_TopRight;
	CPosMatrix m_TopLeft;
	CPosMatrix m_BottomLeft;
	CPosMatrix m_BottomRight;

	//For width;
	CPosMatrix m_Left;
	CPosMatrix m_Right;

	//For height;
	CPosMatrix m_Top;
	CPosMatrix m_Bottom;

	//For general interest
	CPosMatrix m_Middle;

	void GenerateCrossSection(float fCompassAngle,float fClinoAngle,float fUp,float fDown,float fLeft,float fRight);
	void SetPosition(CPosMatrix& pPosition);	//adds the current position since the junction is calculated
						//as a series of deltas
	void SwapSides();
};

class CJunctionGeometryBox:public CObject
{
public:
	CJunctionGeometryBox();
	~CJunctionGeometryBox();

	//This is the central junction of the intersection
	CJunctionBox * m_pCentralHeightOnlyJunctionBox;
	//This array holds all the other junctions.  They are all calculated as
	//incoming junctions.
	CArray<CJunctionBox* ,CJunctionBox* > m_junctionBoxArray;
};

#endif



