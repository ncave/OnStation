#ifndef I_GWORLDOPENGL__
#define I_GWORLDOPENGL__

#include "gworld.h"
#include "hitlist.h"	//searchable objects (for mouse hit testing in global drawing)
#include "afxtempl.h"

class CViewPort;
//Our dimensions are surface/cave/labels
#define MYLIST_CAVEGEOMETRY	1
#define MYLIST_JUNCTION_LABELS	2
#define MYLIST_LABELS	3
#define MYLIST_SURFACEDATA			4
#define MYLIST_WIREFRAMEMODE	5
#define MYLIST_CLOSUREERRORS	6

struct CVertex
{
	float fPos[3];
	float fNormal[3];
};

struct CGWorldOpenGLLabel
{
public:
	COLORREF crColor;
	CNode *node;
	float position[3];
	BOOL bJunction;
	TCHAR szName[MAX_STATION_NAME_LENGTH];
};

class CGWorldOpenGLConstraint
{
public:
	COLORREF crColor;
	CNode *node;
	float position[3];
	BOOL bSearchResultTag;
};

class CGWorldOpenGLLine
{
public:
	COLORREF crColor;
	float pt1[3];
	float pt2[3];
	LINE_TYPE lineType;

	void DumpToGL() const;
	void ExpandRectangle(CViewMatrix *,CRect *) const;
};

class CGlobalColorManager;
class CGWorldOpenGL:public CGWorld
{
	private:
		CArray<CGWorldOpenGLLine,CGWorldOpenGLLine&> m_LineArray;
		CArray<CGWorldOpenGLLine,CGWorldOpenGLLine&> m_WireframeLineArray;
		CArray<CGWorldOpenGLConstraint,CGWorldOpenGLConstraint&>  m_ConstraintArray;
		CArray<CGWorldOpenGLLabel,CGWorldOpenGLLabel&> m_LabelArray;
		//Because these triangle groups are variable size, we store pointers.  We
		//are responsible for cleanup
		CArray<CGWorldTriangleGroup *,CGWorldTriangleGroup *> m_TriangleGroupArray;

		void CompileLists();

	public:
		CGWorldOpenGL();
		~CGWorldOpenGL();

		void AddTriangleGroup(CGWorldTriangleGroup * pTriangleGroup);
		void AddLineSegment(CPosMatrix& From,CPosMatrix& To,COLORREF crColor,LINE_TYPE lineType);
		void AddLabel(CNode *node,CPosMatrix & Pos,LPCTSTR szText,BOOL bJunction,COLORREF crColor);
		void AddConstraint(CPosMatrix &Pos,COLORREF crColor,CNode * pNode,BOOL bMatchedSearch);
		void AddClosureError(CPosMatrix& From,CPosMatrix& To);

		void GenerateGeometry(BOOL bFast,BOOL bStereoscopic,BOOL bShowFixedPoints,BOOL bShowClosureErrors);
		//Finds the maximum extents of the current geometry
		void FindGeometryLimits(CViewMatrix * pViewMatrix,CRect *,BOOL bSterorscopic,BOOL bShowFixedPoints,BOOL bShowClosureErrors);

		void DrawText(CDC * pDC, CViewMatrix * pViewPort,CFont * pFont, STATIONSHOW show,CRect rViewPort,float fLeft,float fRight,float fTop,float fBottom);
		void DrawTaggedPoints(CDC * pDC, CViewMatrix * pViewPort,CRect rViewPort,float fLeft,float fRight,float fTop,float fBottom,BOOL bEliminateFixed);
		void DrawGDILineSegments(CDC * pDC, CViewMatrix * pViewMatrix, CRect rViewPort,float fLeft, float fRight,float fTop,float fBottom,int iLineSize,BOOL bClosureErrors);

		CPtrArray * GetHitList(CViewMatrix * pView,CPoint point,STATIONSHOW show,BOOL bFixedPoints,CRect rViewPort,float fLeft,float fRight,float fTop,float fBottom);
		void Empty();

		//VRML Support functions
		void WriteGWorldToVRML(CArchive& file,STATIONSHOW SS);

		// DXF Support functions
		void WriteGWorldToDXF(CStdioFile *pFile, BOOL bShowClosureError, STATIONSHOW bShowStationNames, BOOL bBoxFixedPoints);
};

#ifdef _DEBUG
void glDumpError();
#endif

#endif

