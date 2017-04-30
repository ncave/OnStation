#ifndef I_GWORLD__
#define I_GWORLD__

#include "hitlist.h"	//searchable objects (for mouse hit testing in global drawing)
#include "afxtempl.h"

enum LINE_TYPE {LT_SHOT,LT_SURFACESHOT, LT_CLOSUREERROR,LT_PASSAGEWIDTH,LT_PASSAGEHEIGHT,LT_WIREFRAME,LT_SURFACE,LT_CROSSSECTION};

typedef struct tagVERTEX
{
	float m_fNormal[3];
	float m_fPos[3];
} VERTEX;

class CGWorldTriangleGroup
{
protected:
	VERTEX * m_pVertices;
	COLORREF m_crColor;
	int m_iNumVertices;

	CGWorldTriangleGroup(int iNumVertices,COLORREF cr);
public:
	virtual void DumpToGL() const=0;
	void ExpandRectangle(CViewMatrix * pViewPort,CRect * pRect) const;
	void SetVertex(int iIndex,VERTEX& vertex);
	void SetVertex(int iIndex,const float *pPos);
	void SetVertex(int iIndex,const float *pPos,const float * pNormal);
	virtual ~CGWorldTriangleGroup();
	virtual void DumpToVRML(CArchive&) const=0;
};

class CGWorldTriangleStrip:public CGWorldTriangleGroup
{
public:
	CGWorldTriangleStrip(int iNumVertices,COLORREF);
	~CGWorldTriangleStrip();

	void SetRotationStripNormals();
	void SetRectNormals();
	void DumpToGL() const;
	void DumpToVRML(CArchive&) const;
};
class CGWorldTriangleFan:public CGWorldTriangleGroup
{
public:
	CGWorldTriangleFan(int iNumVertices,COLORREF);
	~CGWorldTriangleFan();

	void DumpToGL() const;
	void DumpToVRML(CArchive&) const;
};

// GWorld
//
// THis is a virtual base class for rendering.  Rendering is done in two
// derived worlds: GWorldGDI and GWorldGL.
class CGWorld:public CObject
{
public:
	CGWorld();
	virtual ~CGWorld();

	virtual	void AddTriangleGroup(CGWorldTriangleGroup * pTriangleGroup)=0;
	virtual void AddLineSegment(CPosMatrix& From,CPosMatrix& To,COLORREF crColor,LINE_TYPE lineType)=0;
	virtual void AddLabel(CNode *node,CPosMatrix & Pos,LPCTSTR szText,BOOL bJunction,COLORREF crColor)=0;
	virtual void AddConstraint(CPosMatrix &Pos,COLORREF crColor,CNode * pNode,BOOL bMatchedSearch)=0;
	virtual void AddClosureError(CPosMatrix& From,CPosMatrix& To)=0;

	virtual void GenerateGeometry(BOOL bFast,BOOL bStereoscopic,BOOL bShowFixedPoints,BOOL bShowClosureErrors)=0;

	//Finds the maximum extents of the current geometry
	virtual void FindGeometryLimits(CViewMatrix * pViewMatrix,CRect *,BOOL bStereoscopic,BOOL bShowFixedPoints,BOOL bShowClosureErrors)=0;

	virtual CPtrArray * GetHitList(CViewMatrix * pViewMatrix,CPoint point,STATIONSHOW show,BOOL bFixedPoints,CRect rViewPort,float fLeft,float fRight,float fTop,float fBottom)=0;
	virtual void Empty()=0;

	virtual void CompileLists() {};	//Calculate openGL display lists from the geometry added to the GWorld

	//VRML Support functions
	virtual void WriteGWorldToVRML(CArchive& file,STATIONSHOW SS)=0;

	//DXF Support functions

	virtual void WriteGWorldToDXF(CStdioFile *pFile, BOOL bShowClosureError, STATIONSHOW bShowStationNames, BOOL bBoxFixedPoints) {};

	virtual void DrawText(CDC * dc,CViewMatrix * pViewPort,CFont* pFont, STATIONSHOW show,CRect rViewPort,float fLeft,float fRight,float fTop,float fBottom)=0;
	virtual void DrawTaggedPoints(CDC * dc,CViewMatrix * pViewPort,CRect rViewPort,float fLeft,float fRight,float fTop,float fBottom,BOOL bShowConstraints)=0;
	virtual void DrawGDILineSegments(CDC * dc,CViewMatrix * pViewMatrix, CRect rViewPort,float fLeft, float fRight,float fTop,float fBottom,int iLineSize,BOOL bClosureErrors)=0;
	//This method is used to dump the internal output of the gworld into some more appropriate data
	//structures if that seems appropriate.  This is where VRML output is done and where the OpenGL
	//world geometry is created.  Empty() is the other half of this equation.
	virtual void AttachNewDC(CDC * dc) {};
	virtual void DetachDC() {};
};


#endif

