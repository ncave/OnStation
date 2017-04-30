#include "stdafx.h"
#include "onstation.h"
#include "gworldopengl.h"
#include "colortool.h"
#include "onstationdoc.h"
#include "settings.h"	//used for print line thickness

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern COnStationApp theApp;
extern CGlobalColorManager Colors_G;


CGWorldOpenGL::CGWorldOpenGL()
{
}

CGWorldOpenGL::~CGWorldOpenGL()
{
	Empty();
}

void CGWorldOpenGL::Empty()
{
	m_WireframeLineArray.RemoveAll();
	m_LineArray.RemoveAll();
	m_LabelArray.RemoveAll();
	m_ConstraintArray.RemoveAll();
	INT_PTR iTriangleGroupCount=m_TriangleGroupArray.GetSize();
	for (INT_PTR i=0;i<iTriangleGroupCount;i++)
	{
		CGWorldTriangleGroup * pGroup=m_TriangleGroupArray.GetAt(i);
		delete pGroup;
	}
	m_TriangleGroupArray.RemoveAll();
}


void CGWorldOpenGL::AddTriangleGroup(CGWorldTriangleGroup * pTriangleGroup)
{
	m_TriangleGroupArray.Add(pTriangleGroup);
}

void CGWorldOpenGL::AddLineSegment(CPosMatrix & From,CPosMatrix & To,COLORREF crColor,LINE_TYPE lineType)
{
	ASSERT(From.GetX()<50000.0f && From.GetY()<50000.0f && From.GetZ()<50000.0f);
	ASSERT(To.GetX()<50000.0f && To.GetY()<50000.0f && To.GetZ()<50000.0f);
	ASSERT(From.GetX()>-50000.0f && From.GetY()>-50000.0f && From.GetZ()>-50000.0f);
	ASSERT(To.GetX()>-50000.0f && To.GetY()>-50000.0f && To.GetZ()>-50000.0f);

	CGWorldOpenGLLine line;
	line.crColor=crColor;
	line.lineType = lineType;

	memcpy(&line.pt1,From.GetDirectPointer(),sizeof(float)*3);
	memcpy(&line.pt2,To.GetDirectPointer(),sizeof(float)*3);

	if (lineType != LT_WIREFRAME)
	{
		m_LineArray.Add(line);
	}
	else
	{
		m_WireframeLineArray.Add(line);
	}
}

void CGWorldOpenGL::AddClosureError(CPosMatrix & From,CPosMatrix & To)
{
	AddLineSegment(From,To,CLOSURE_RGB,LT_CLOSUREERROR);
}


void CGWorldOpenGL::AddConstraint(CPosMatrix & Tag,COLORREF crColor,CNode * pNode,BOOL bMatchedSearch)
{
	CGWorldOpenGLConstraint constraint;
	memcpy(&constraint.position,Tag.GetDirectPointer(),sizeof(float)*3);
	constraint.crColor=crColor;
	constraint.node=pNode;
	constraint.bSearchResultTag=bMatchedSearch;
	m_ConstraintArray.Add(constraint);
}

void CGWorldOpenGL::AddLabel(CNode *node,CPosMatrix & Tag,LPCTSTR szName,BOOL bJunction,COLORREF crColor)
{
	CGWorldOpenGLLabel label;
	label.node=node;
	label.crColor=crColor;
	lstrcpyn(label.szName,szName,MAX_STATION_NAME_LENGTH-1);
	memcpy(&label.position,Tag.GetDirectPointer(),sizeof(float)*3);
	label.bJunction=bJunction;
	m_LabelArray.Add(label);
}

void CGWorldOpenGL::WriteGWorldToVRML(CArchive& file,STATIONSHOW SS)
{
	TCHAR szBuffer[128];
	file.WriteString(_T("#VRML V1.0 ascii\n"));
	//We should probably just use the defaults and make things run faster instead of setting
	//a light source.
	file.WriteString(_T("Separator {\n\tDEF SceneInfo Info {\n\t\tstring "));
	file<<_T('"');
	file.WriteString(_T("Modeled using On Station 3.1"));
	file<<_T('"');
	file.WriteString(_T("\n\t}\n}\n"));

	INT_PTR iTriangleGroupCount=m_TriangleGroupArray.GetSize();
	for (INT_PTR i=0;i<iTriangleGroupCount;i++)
	{
		_stprintf_s(szBuffer, _T("DEF TriangleStrip%i Separator {\n"),i+1);
		file.WriteString(szBuffer);
		CGWorldTriangleGroup const * pGroup=m_TriangleGroupArray.GetAt(i);
		pGroup->DumpToVRML(file);
		file.WriteString(_T("}\n"));
	}
	
	//Next, do the fixed points if appropriate
	INT_PTR iFixedPointCount=m_ConstraintArray.GetSize();
	CPosMatrix ptPos;
	CPosMatrix transformedPos;
	for (INT_PTR i=0;i<iFixedPointCount;i++)
	{
		_stprintf_s(szBuffer, _T("DEF FixedPoint%i Separator {\n"),i+1);
		file.WriteString(szBuffer);
		
		const CGWorldOpenGLConstraint & fixedPoint=m_ConstraintArray.GetAt(i);
		file.WriteString(_T("\tMaterial {\n\t\tdiffuseColor 1.0 0.0 0.0\n\t}\n"));
		_stprintf_s(szBuffer, _T("\tTranslation {\n\t\ttranslation %f %f %f\n\t}\n"),fixedPoint.position[0],fixedPoint.position[1],fixedPoint.position[2]);
		file.WriteString(szBuffer);
		file.WriteString(_T("\tSphere {\n\t\tradius 3\n\t}\n"));
		file.WriteString(_T("}\n"));
	}
}

void CGWorldOpenGL::WriteGWorldToDXF(CStdioFile *pFile, BOOL bShowClosureError, STATIONSHOW bShowStationNames, BOOL bBoxFixedPoints)
{
	// blocks
	CString stationBlockName = _T("STATION");
	CString fixedPointBlockName = _T("FIXEDPOINT");
	// layers
	CString stationLayerName1 = _T("S_STATION100");
	CString stationLayerName2 = _T("S_STATION500");
	CString shotLayerName = _T("S_SHOT");
	CString wallsLayerName = _T("S_PASSAGEWIDTH");
	CString ceilingLayerName = _T("S_PASSAGEHEIGHT");
	CString errorLayerName = _T("S_ERRORS");
	CString heightLayerName = _T("S_STATIONHEIGHT");
	//CString heightLayerName2 = _T("S_STATIONHEIGHT500");
	CString namesLayerName1 = _T("S_STATIONNAMES100");
	CString namesLayerName2 = _T("S_STATIONNAMES500");
	CString fixedPointLayerName = _T("S_FIXEDPOINT");
	CString wireFrameLayerName = _T("S_SURFACEDATA");
	CString crossSectionLayerName = _T("S_CROSSSECTION");
	CString surfaceShotLayerName = _T("S_SURFACESHOT");

	// show hourglass mouse pointer
	CWinApp *pApp = AfxGetApp();
	pApp -> BeginWaitCursor();

	// write DXF header
	///////////////////
	pFile-> WriteString(_T("0\nSECTION\n2\nHEADER\n"));	// (note: a header section must exist (even if empty) to support LAYER tables)

	// read value off one item to set drawing limits (otherwise the cave may end-up in one corner of the autocad screen)
	if(m_ConstraintArray.GetSize())	// test just in case the user tries to export an empty file
	{
		const CGWorldOpenGLConstraint & fixedPoint=m_ConstraintArray.GetAt(0);
		pFile-> WriteString(_T("9\n$LIMMIN\n10\n")+FloatToString(fixedPoint.position[0])+ _T("\n20\n")+FloatToString(fixedPoint.position[1])+
							"\n9\n$LIMMAX\n10\n"+FloatToString(fixedPoint.position[0])+ _T("\n20\n")+FloatToString(fixedPoint.position[1])+ _T("\n"));
	}

	pFile-> WriteString(_T("0\nENDSEC\n"));

	// write tables
	///////////////
	pFile-> WriteString(_T("0\nSECTION\n2\nTABLES\n"));
	// write layers
	const int countLayers = 14; //15;
	CString layerNames[countLayers] = {_T("0"), stationLayerName1, stationLayerName2, shotLayerName, wallsLayerName, ceilingLayerName, 
										errorLayerName, heightLayerName, /*heightLayerName2,*/ namesLayerName1, namesLayerName2, 
										fixedPointLayerName, wireFrameLayerName, crossSectionLayerName, surfaceShotLayerName};
	int layerColor[countLayers] = {7, 1, 1, 3, 5, 5, 1, 6, /*6,*/ 4, 4, 2, 8, 5, 9};

	pFile-> WriteString(_T("0\nTABLE\n2\nLAYER\n70\n")+IntToString(countLayers)+ _T("\n"));	// change last item (after 70) to max number of layers

	for (int i=0; i<countLayers; ++i)		// write each layer
		pFile-> WriteString(_T("0\nLAYER\n2\n")+layerNames[i]+ _T("\n70\n0\n62\n")+IntToString(layerColor[i])+ _T("\n6\nCONTINUOUS\n"));

	pFile-> WriteString(_T("0\nENDTAB\n"));
	pFile-> WriteString(_T("0\nENDSEC\n"));

	// define blocks (to be included later in the drawing)
	////////////////
	pFile-> WriteString(_T("0\nSECTION\n2\nBLOCKS\n"));

	// define station block
	pFile-> WriteString(_T("0\nBLOCK\n8\n")+ stationLayerName1 + _T("\n2\n") + stationBlockName + _T("\n70\n0\n10\n0\n20\n0\n30\n0\n"));
	// small triangle
	pFile-> WriteString(_T("0\nSOLID\n8\n") + stationLayerName1 + _T("\n10\n0\n20\n0.15\n30\n0\n11\n-0.1299\n21\n-0.075\n31\n0\n12\n0.1299\n22\n-0.075\n32\n0\n13\n0.1299\n23\n-0.075\n33\n0\n"));
	// large triangle
	pFile-> WriteString(_T("0\nPOLYLINE\n8\n") + stationLayerName2 + _T("\n66\n1\n10\n0.0\n20\n0.0\n30\n0.0\n70\n1\n"));
	pFile-> WriteString(_T("0\nVERTEX\n8\n") + stationLayerName2 + _T("\n10\n0\n20\n0.5\n30\n0\n"));
	pFile-> WriteString(_T("0\nVERTEX\n8\n") + stationLayerName2 + _T("\n10\n-0.433\n20\n-0.25\n30\n0\n"));
	pFile-> WriteString(_T("0\nVERTEX\n8\n") + stationLayerName2 + _T("\n10\n0.433\n20\n-0.25\n30\n0\n"));
	pFile-> WriteString(_T("0\nSEQEND\n8\n") + stationLayerName2 + _T("\n"));

	pFile-> WriteString(_T("0\nENDBLK\n8\n") + stationLayerName1+ _T("\n"));

	// define fixedPoint block
	pFile-> WriteString(_T("0\nBLOCK\n8\n") + fixedPointLayerName + _T("\n2\n") + fixedPointBlockName + _T("\n70\n0\n10\n0\n20\n0\n30\n0\n"));
	pFile-> WriteString(_T("0\nSOLID\n8\n") + fixedPointLayerName + _T("\n10\n0.5\n20\n0.5\n30\n0\n11\n-0.5\n21\n0.5\n31\n0\n12\n0.5\n22\n-0.5\n32\n0\n13\n-0.5\n23\n-0.5\n33\n0\n"));
	pFile-> WriteString(_T("0\nENDBLK\n8\n") + fixedPointLayerName+ _T("\n"));

	pFile-> WriteString(_T("0\nENDSEC\n"));

	// write DXF entities
	/////////////////////
	pFile-> WriteString(_T("0\nSECTION\n2\nENTITIES\n"));

	// write lines
	INT_PTR iCount=m_LineArray.GetSize();
	for (INT_PTR i=0;i<iCount;i++)
	{
		const CGWorldOpenGLLine& Line=m_LineArray.GetAt(i);

		BOOL bShowThisLine=TRUE;
		CString layer = _T("0");	// lines of undefined types go to layer 0

		switch (Line.lineType)
		{	// diferent types of lines are output to different layers
		case LT_SHOT:
			layer = shotLayerName;
			break;
		case LT_SURFACESHOT:
			layer = surfaceShotLayerName;
			break;
		case LT_CLOSUREERROR:
			layer = errorLayerName;
			bShowThisLine=bShowClosureError;
			break;
		case LT_PASSAGEWIDTH:
			layer = wallsLayerName;
			break;
		case LT_PASSAGEHEIGHT:
			layer = ceilingLayerName;
			break;
		case LT_SURFACE:
			layer = wireFrameLayerName;
			break;
		case LT_CROSSSECTION:
			layer = crossSectionLayerName;
			break;
//		case LT_WIREFRAME:	// those should not be seen since they only appear on screen when rotating
//		default:	// (there should be no other types)
//			break;
		}

		if (bShowThisLine)
			pFile-> WriteString(_T("0\nLINE\n8\n") + layer + 
						"\n10\n" + FloatToString(Line.pt1[0]) + _T("\n20\n") + FloatToString(Line.pt1[1]) + _T("\n30\n") + FloatToString(Line.pt1[2]) + 
						"\n11\n" + FloatToString(Line.pt2[0]) + _T("\n21\n") + FloatToString(Line.pt2[1]) + _T("\n31\n") + FloatToString(Line.pt2[2]) + _T("\n"));
	}

	// write stations and station's name and height
	INT_PTR iLabelCount=m_LabelArray.GetSize();
	for (INT_PTR i=0;i<iLabelCount;i++)
	{
		const CGWorldOpenGLLabel & Label=m_LabelArray.GetAt(i);

		// write station name
		if (bShowStationNames==SHOW_ALL || (Label.bJunction && bShowStationNames==SHOW_INTERSECT))
		{
			// small text
			pFile-> WriteString(_T("0\nTEXT\n8\n") + namesLayerName1 + _T("\n72\n2\n") +
						"10\n" + FloatToString(Label.position[0]) + _T("\n20\n") + FloatToString(Label.position[1]) + _T("\n30\n") + FloatToString(Label.position[2]) + 
						"\n11\n" + FloatToString(Label.position[0] - (float)0.1299) + _T("\n21\n") + FloatToString(Label.position[1]) + _T("\n31\n") + FloatToString(Label.position[2]) + 
						"\n40\n0.4\n1\n" + Label.szName + _T("\n"));
			// large text
			pFile-> WriteString(_T("0\nTEXT\n8\n") + namesLayerName2 + _T("\n72\n2\n") +
						"10\n" + FloatToString(Label.position[0]) + _T("\n20\n") + FloatToString(Label.position[1]) + _T("\n30\n") + FloatToString(Label.position[2]) + 
						"\n11\n" + FloatToString(Label.position[0] - (float)0.433) + _T("\n21\n") + FloatToString(Label.position[1]) + _T("\n31\n") + FloatToString(Label.position[2]) + 
						"\n40\n1.0\n1\n" + Label.szName + _T("\n"));
		}
		// write station height
		// small text
		pFile-> WriteString(_T("0\nTEXT\n8\n") + heightLayerName + _T("\n") + //"73\n3\n" +	// tag 73 is not recognized by AutoCAD v.10: we substract the font size from the y value
					"10\n" + FloatToString(Label.position[0] + (float)0.1299) + _T("\n20\n") + FloatToString(Label.position[1]-(float)0.4) + _T("\n30\n") + FloatToString(Label.position[2]) + 
					"\n11\n" + FloatToString(Label.position[0] + (float)0.1299) + _T("\n21\n") + FloatToString(Label.position[1]-(float)0.4) + _T("\n31\n") + FloatToString(Label.position[2]) + 
					"\n40\n0.4\n1\n" + FloatToString(Label.position[2]) + _T("\n"));
/*		// large text
		pFile-> WriteString(_T("0\nTEXT\n8\n") + heightLayerName2 + _T("\n73\n3\n") +
					"10\n" + FloatToString(Label.position[0]) + _T("\n20\n") + FloatToString(Label.position[1]) + _T("\n30\n") + FloatToString(Label.position[2]) + 
					"\n11\n" + FloatToString(Label.position[0] + (float)0.433) + _T("\n21\n") + FloatToString(Label.position[1]) + _T("\n31\n") + FloatToString(Label.position[2]) + 
					"\n40\n1.0\n1\n" + FloatToString(Label.position[2]) + _T("\n"));
*/
		// draw station
		pFile-> WriteString(_T("0\nINSERT\n2\n") + stationBlockName + _T("\n8\n") + stationLayerName1 + 
					"\n10\n" + FloatToString(Label.position[0])+ _T("\n20\n") + FloatToString(Label.position[1]) + _T("\n30\n") + FloatToString(Label.position[2]) + _T("\n"));
	}

	// write fixed points and fixed points' text
	INT_PTR iFixedPointCount=m_ConstraintArray.GetSize();
	for (INT_PTR i=0;i<iFixedPointCount;i++)
	{
		const CGWorldOpenGLConstraint & fixedPoint=m_ConstraintArray.GetAt(i);
		
		if (!fixedPoint.bSearchResultTag)	// only show fixed points: not search results _T('?') tags
		{
			// write text
			pFile-> WriteString(_T("0\nTEXT\n8\n") + fixedPointLayerName +
						"\n10\n" + FloatToString(fixedPoint.position[0] + (float)0.6) + _T("\n20\n") + FloatToString(fixedPoint.position[1]) + _T("\n30\n") + FloatToString(fixedPoint.position[2]) +
						"\n40\n1.0\n1\n" + fixedPoint.node->GetName() + _T("\n"));
					
			// draw fixed point
			pFile-> WriteString(_T("0\nINSERT\n2\n") + fixedPointBlockName + _T("\n8\n") + fixedPointLayerName + 
						"\n10\n" + FloatToString(fixedPoint.position[0])+ _T("\n20\n") + FloatToString(fixedPoint.position[1]) + _T("\n30\n") + FloatToString(fixedPoint.position[2]) + _T("\n"));

		}
	}

	// end of 'entities' section and end of file
	pFile-> WriteString(_T("0\nENDSEC\n0\nEOF\n"));

	// hide hourglass mouse pointer
	pApp -> EndWaitCursor();
}

void CGWorldOpenGL::FindGeometryLimits(CViewMatrix * pViewMatrix,CRect * pRect,BOOL bStereoscopic,BOOL bShowFixedPoints,BOOL bShowClosureErrors)
{
	CPosMatrix ptPos;
	CPosMatrix transformedPos;

	INT_PTR iLineCount=m_LineArray.GetSize();
	for (INT_PTR i=0;i<iLineCount;i++)
	{
		const CGWorldOpenGLLine& line=m_LineArray.GetAt(i);
		if (line.crColor!=CLOSURE_RGB)
		{
			line.ExpandRectangle(pViewMatrix,pRect);
		}
	}
	INT_PTR iTriangleGroupCount=m_TriangleGroupArray.GetSize();
	for (INT_PTR i=0;i<iTriangleGroupCount;i++)
	{
		CGWorldTriangleGroup const * pGroup=m_TriangleGroupArray.GetAt(i);
		pGroup->ExpandRectangle(pViewMatrix,pRect);
	}

	iLineCount=m_LineArray.GetSize();
	for (INT_PTR i=0;i<iLineCount;i++)
	{
		const CGWorldOpenGLLine& line=m_LineArray.GetAt(i);
		line.ExpandRectangle(pViewMatrix,pRect);
	}
	if (bShowClosureErrors)
	{
		for (INT_PTR i=0;i<iLineCount;i++)
		{
			const CGWorldOpenGLLine& line=m_LineArray.GetAt(i);
			line.ExpandRectangle(pViewMatrix,pRect);
		}
	}
}

// **************************************
// PRODUCE THE THING FUNCTIONS
void CGWorldOpenGL::GenerateGeometry(BOOL bFast,BOOL bStereoscopic,BOOL bShowFixedPoints,BOOL bShowClosureErrors)
{
	if (bFast)
	{
		glCallList(MYLIST_WIREFRAMEMODE);
	}
	else
	{
		glCallList(MYLIST_CAVEGEOMETRY);
	}
	if (bShowClosureErrors)
	{
		glCallList(MYLIST_CLOSUREERRORS);
	}
}

CPtrArray * CGWorldOpenGL::GetHitList(CViewMatrix* pViewMatrix,CPoint point,STATIONSHOW show,BOOL bFixedPoints,CRect rViewPort,float fLeft,float fRight,float fTop,float fBottom)
{
	CPtrArray * pPtrArray=new CPtrArray;

	if (bFixedPoints)
	{
		INT_PTR iFixedPointCount=m_ConstraintArray.GetSize();
		CPosMatrix ptPos;
		CPosMatrix transformedPos;
		for (INT_PTR i=0;i<iFixedPointCount;i++)
		{
			const CGWorldOpenGLConstraint & fixedPoint=m_ConstraintArray.GetAt(i);
			ptPos.Set(fixedPoint.position[0],fixedPoint.position[1],fixedPoint.position[2]);
			ptPos.Multiply(&transformedPos,*pViewMatrix);
			transformedPos.Normalize();
			CRect rect;
			float fTextLeft=((transformedPos.GetX()-fLeft)/(fRight-fLeft))*((float)rViewPort.Width())+(float)rViewPort.left;
			float fTextTop=(float)rViewPort.bottom-((transformedPos.GetY()-fTop)/(fBottom-fTop))*((float)rViewPort.Height());
			rect.left=(int)fTextLeft-18;
			rect.top=(int)fTextTop;
			rect.right=rect.left+16;
			rect.bottom=rect.top+16;
			if (rect.PtInRect(point))
			{
				pPtrArray->Add((LPVOID)fixedPoint.node);
			}
		}
	}

	if (show==SHOW_NONE)
	{
		return pPtrArray;
	}
	INT_PTR iLabelCount=m_LabelArray.GetSize();
	CPosMatrix labelPos;
	CPosMatrix transformedPos;
	for (INT_PTR i=0;i<iLabelCount;i++)
	{
		const CGWorldOpenGLLabel & Label=m_LabelArray.GetAt(i);
		if (show==SHOW_ALL || Label.bJunction)
		{
			labelPos.Set(Label.position[0],Label.position[1],Label.position[2]);
			labelPos.Multiply(&transformedPos,*pViewMatrix);
			CRect rect;
			transformedPos.Normalize();
			float fTextLeft=((transformedPos.GetX()-fLeft)/(fRight-fLeft))*((float)rViewPort.Width())+(float)rViewPort.left;
			float fTextTop=(float)rViewPort.bottom-((transformedPos.GetY()-fTop)/(fBottom-fTop))*((float)rViewPort.Height());
			rect.left=(int)fTextLeft;
			rect.top=(int)fTextTop;
			rect.right=rect.left+50;
			rect.bottom=rect.top+15;
			if (rect.PtInRect(point))
			{
				pPtrArray->Add((LPVOID)Label.node);
			}
		}
	}
	return pPtrArray;
}

void CGWorldOpenGL::DrawGDILineSegments(CDC * pDC,CViewMatrix * pViewMatrix, CRect rViewPort,float fLeft, float fRight,float fTop,float fBottom,int iLineSize,BOOL bShowClosureErrors)
{
	CPosMatrix fromPos;
	CPosMatrix transformedFromPos;
	CPosMatrix toPos;
	CPosMatrix transformedToPos;
	INT_PTR iCount=m_WireframeLineArray.GetSize();
	for (INT_PTR i=0;i<iCount;i++)
	{
		const CGWorldOpenGLLine& Line=m_WireframeLineArray.GetAt(i);
		fromPos.Set(Line.pt1[0],Line.pt1[1],Line.pt1[2]);
		fromPos.Multiply(&transformedFromPos,*pViewMatrix);
		toPos.Set(Line.pt2[0],Line.pt2[1],Line.pt2[2]);
		toPos.Multiply(&transformedToPos,*pViewMatrix);

		//Reverse black and white when printing
		COLORREF crColor=Line.crColor;
		if (pDC->IsPrinting() && crColor==RGB(255,255,255))
		{
			crColor=RGB(0,0,0);
		}
		toPos.Normalize();
		fromPos.Normalize();
		CRect rect;
		transformedFromPos.Normalize();
		float fFromLeft=((transformedFromPos.GetX()-fLeft)/(fRight-fLeft))*((float)rViewPort.Width())+(float)rViewPort.left;
		float fFromTop=(float)rViewPort.bottom-((transformedFromPos.GetY()-fTop)/(fBottom-fTop))*((float)rViewPort.Height());
		float fToLeft=((transformedToPos.GetX()-fLeft)/(fRight-fLeft))*((float)rViewPort.Width())+(float)rViewPort.left;
		float fToTop=(float)rViewPort.bottom-((transformedToPos.GetY()-fTop)/(fBottom-fTop))*((float)rViewPort.Height());
		CPen Pen(PS_SOLID,iLineSize,crColor);
		CPen * pPenOld=pDC->SelectObject(&Pen);
		pDC->MoveTo((int)fFromLeft,(int)fFromTop);
		pDC->LineTo((int)fToLeft,(int)fToTop);
		pDC->SelectObject(pPenOld);
	}

	iCount=m_LineArray.GetSize();
	for (INT_PTR i=0;i<iCount;i++)
	{
		const CGWorldOpenGLLine& Line=m_LineArray.GetAt(i);
		//crColor==CLOSURE_RGB indicates loop closure.
		BOOL bShowThisLine=TRUE;
		if (Line.crColor==CLOSURE_RGB)
		{
			bShowThisLine=bShowClosureErrors;
		}

		if (bShowThisLine)
		{
			fromPos.Set(Line.pt1[0],Line.pt1[1],Line.pt1[2]);
			fromPos.Multiply(&transformedFromPos,*pViewMatrix);
			toPos.Set(Line.pt2[0],Line.pt2[1],Line.pt2[2]);
			toPos.Multiply(&transformedToPos,*pViewMatrix);
			//Reverse black and white when printing
			COLORREF crColor=Line.crColor;
			if (pDC->IsPrinting() && crColor==RGB(255,255,255))
			{
				crColor=RGB(0,0,0);
			}
			toPos.Normalize();
			fromPos.Normalize();
			CRect rect;
			transformedFromPos.Normalize();
			float fFromLeft=((transformedFromPos.GetX()-fLeft)/(fRight-fLeft))*((float)rViewPort.Width())+(float)rViewPort.left;
			float fFromTop=(float)rViewPort.bottom-((transformedFromPos.GetY()-fTop)/(fBottom-fTop))*((float)rViewPort.Height());
			float fToLeft=((transformedToPos.GetX()-fLeft)/(fRight-fLeft))*((float)rViewPort.Width())+(float)rViewPort.left;
			float fToTop=(float)rViewPort.bottom-((transformedToPos.GetY()-fTop)/(fBottom-fTop))*((float)rViewPort.Height());
			
			CPen Pen(crColor==CLOSURE_RGB?PS_DASH:PS_SOLID,iLineSize,crColor==CLOSURE_RGB?RGB(255,0,0):crColor);
			CPen * pPenOld=pDC->SelectObject(&Pen);
			pDC->MoveTo((int)fFromLeft,(int)fFromTop);
			pDC->LineTo((int)fToLeft,(int)fToTop);
			pDC->SelectObject(pPenOld);
		}
	}
	INT_PTR iTriangleGroupCount=m_TriangleGroupArray.GetSize();
	for (INT_PTR i=0;i<iTriangleGroupCount;i++)
	{
		CGWorldTriangleGroup const * pGroup=m_TriangleGroupArray.GetAt(i);
		pGroup->DumpToGL();
	}
	glEndList();

}

void CGWorldOpenGL::DrawTaggedPoints(CDC * pDC,CViewMatrix * pViewMatrix,CRect rViewPort,float fLeft,float fRight,float fTop,float fBottom,BOOL bEliminateFixed)
{
	INT_PTR iFixedPointCount=m_ConstraintArray.GetSize();
	if (iFixedPointCount==0)
		return;

	CDC dc2;
	dc2.CreateCompatibleDC(pDC);
	CBitmap pFixedPointBitmap;
	pFixedPointBitmap.LoadBitmap(IDB_BITMAPPINONSCREEN);
	CBitmap pSearchFoundBitmap;
	pSearchFoundBitmap.LoadBitmap(IDB_BITMAPSEARCHFOUND);
	CPosMatrix ptPos;
	CPosMatrix transformedPos;
	for (INT_PTR i=0;i<iFixedPointCount;i++)
	{
		const CGWorldOpenGLConstraint & fixedPoint=m_ConstraintArray.GetAt(i);
		ptPos.Set(fixedPoint.position[0],fixedPoint.position[1],fixedPoint.position[2]);
		ptPos.Multiply(&transformedPos,*pViewMatrix);
		transformedPos.Normalize();
		float fTextLeft=((transformedPos.GetX()-fLeft)/(fRight-fLeft))*((float)rViewPort.Width())+(float)rViewPort.left;
		float fTextTop=(float)rViewPort.bottom-((transformedPos.GetY()-fTop)/(fBottom-fTop))*((float)rViewPort.Height());
		CBitmap * pBm=NULL;
		if (fixedPoint.bSearchResultTag || !bEliminateFixed)
		{
			CBitmap * pOldBitmap=dc2.SelectObject(fixedPoint.bSearchResultTag?&pSearchFoundBitmap:&pFixedPointBitmap);
			if (!pDC->IsPrinting())
			{
				pDC->BitBlt((int)fTextLeft-18,(int)fTextTop,16,16,&dc2,0,0,SRCCOPY);
			}
			else
			{
				pDC->FillSolidRect((int)fTextLeft-10,(int)fTextTop-10,20,20,RGB(0,0,0));
			}
			dc2.SelectObject(pOldBitmap);
		}
	}
}

void CGWorldOpenGL::DrawText(CDC * pDC, CViewMatrix * pViewMatrix,CFont * pFont, STATIONSHOW show,CRect rViewPort,float fLeft,float fRight,float fTop,float fBottom)
{
	if (show==SHOW_NONE)
		return;

	CFont * pOldFont=pDC->SelectObject(pFont);
	CBrush * pOldBrush = (CBrush *)pDC->SelectStockObject(NULL_BRUSH);
	int nOldBkMode = pDC->SetBkMode(OPAQUE);
	COLORREF oldColor = pDC->SetTextColor(RGB(255,255,255));
	COLORREF oldBkColor = pDC->SetBkColor(RGB(0, 0, 0));

	INT_PTR iLabelCount=m_LabelArray.GetSize();
	CPosMatrix labelPos;
	CPosMatrix transformedPos;
	for (INT_PTR i=0;i<iLabelCount;i++)
	{
		const CGWorldOpenGLLabel & Label=m_LabelArray.GetAt(i);
		if (show==SHOW_ALL || Label.bJunction)
		{
			labelPos.Set(Label.position[0],Label.position[1],Label.position[2]);
			labelPos.Multiply(&transformedPos,*pViewMatrix);
			COLORREF newColor = Label.crColor;
			//Reverse black and white when printing
			if (pDC->IsPrinting() && newColor==RGB(255,255,255))
			{
				newColor = RGB(0,0,0);
			}
			pDC->SetTextColor(newColor);

			CRect rect;
			transformedPos.Normalize();
			float fTextLeft=((transformedPos.GetX()-fLeft)/(fRight-fLeft))*((float)rViewPort.Width())+(float)rViewPort.left;
			float fTextTop=(float)rViewPort.bottom-((transformedPos.GetY()-fTop)/(fBottom-fTop))*((float)rViewPort.Height());
			rect.left=(int)fTextLeft;
			rect.top=(int)fTextTop;
			rect.right=rect.left+50;
			rect.bottom=rect.top+15;
			pDC->DrawText(Label.szName,rect,DT_NOCLIP);
			//pDC->TextOut(rect.left, rect.top, Label.szName, lstrlen(Label.szName));
			//TRACE(_T("GworldOpenGL:: Outputting %s at %i,%i %i,%i\n"),Label.szName,rect.left,rect.top,rect.right,rect.bottom);
		}
	}
	pDC->SetBkMode(nOldBkMode);
	pDC->SetBkColor(oldBkColor);
	pDC->SetTextColor(oldColor);
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldFont);
}



void CGWorldOpenGL::CompileLists()
{
	//Our compile strategy is to split the geometry into different
	//pieces so that we can turn on and off sections by simply rendering
	//the pieces that we want.

	/*TEST GEOMETRY
	glRGB(255,0,0);
	glBegin(GL_LINES);
	glVertex3f(0.0f,0.0f,0.0f);
	glVertex3f(0.0f,100.0f,-100.0f);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(0.0f,100.0f,-100.0f);
	glVertex3f(50.0f,150.0f,-200.0f);
	glEnd();
	glRGB(0,0,255);
	glBegin(GL_TRIANGLES);
	glVertex3f(30.f,30.f,300.f);
	glVertex3f(30.f,40.f,300.f);
	glVertex3f(40.f,40.f,300.f);
	glEnd();
	glRGB(0,0,255);
	glBegin(GL_TRIANGLES);
	glVertex3f(60.f,60.f,-1000.f);
	glVertex3f(60.f,70.f,-1000.f);
	glVertex3f(70.f,70.f,-1000.f);
	glEnd();
	glEndList();
*/

	//Start with the lines and rectangles which define the underground stuff
	glNewList(MYLIST_CAVEGEOMETRY,GL_COMPILE);
	INT_PTR iLineCount=m_LineArray.GetSize();
	for (INT_PTR i=0;i<iLineCount;i++)
	{
		const CGWorldOpenGLLine& line=m_LineArray.GetAt(i);
		if (line.crColor!=CLOSURE_RGB)
		{
			line.DumpToGL();
		}
	}
	INT_PTR iTriangleGroupCount=m_TriangleGroupArray.GetSize();
	for (INT_PTR i=0;i<iTriangleGroupCount;i++)
	{
		CGWorldTriangleGroup const * pGroup=m_TriangleGroupArray.GetAt(i);
		pGroup->DumpToGL();
	}
	glEndList();
	
	glNewList(MYLIST_CLOSUREERRORS,GL_COMPILE);
	for (INT_PTR i=0;i<iLineCount;i++)
	{
		const CGWorldOpenGLLine& line=m_LineArray.GetAt(i);
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(5,0x5555);	//dashed lines 5 pixels long
		glRGB(255,0,0);
		if (line.crColor==CLOSURE_RGB)
		{
			line.DumpToGL();
		}
		glDisable(GL_LINE_STIPPLE);
	}
	glEndList();

	glNewList(MYLIST_WIREFRAMEMODE,GL_COMPILE);
	iLineCount=m_WireframeLineArray.GetSize();
	for (INT_PTR i=0;i<iLineCount;i++)
	{
		const CGWorldOpenGLLine& line=m_WireframeLineArray.GetAt(i);
		line.DumpToGL();
	}
	glEndList();

}

#ifdef _DEBUG
void glDumpError()
{
	switch (glGetError())
	{
	case GL_NO_ERROR:
		break;
	case GL_INVALID_ENUM:
		AfxMessageBox(_T("Invalid Enum"));	//internationalization OK
		ASSERT(FALSE);
		break;
	case GL_INVALID_VALUE:
		AfxMessageBox(_T("Invalid Value"));//internationalization OK
		ASSERT(FALSE);
		break;
	case GL_INVALID_OPERATION:
		AfxMessageBox(_T("Invalid Operation"));//internationalization OK
		ASSERT(FALSE);
		break;
	case GL_STACK_OVERFLOW:
		AfxMessageBox(_T("Stack overflow"));//internationalization OK
		ASSERT(FALSE);
		break;
	case GL_STACK_UNDERFLOW:
		AfxMessageBox(_T("Stack underflow"));//internationalization OK
		ASSERT(FALSE);
		break;
	case GL_OUT_OF_MEMORY:
		AfxMessageBox(_T("Out of Memory"));//internationalization OK
		ASSERT(FALSE);
		break;
	default:
		ASSERT(FALSE);
	}
}


#endif




void CGWorldOpenGLLine::DumpToGL() const
{
	if (crColor!=CLOSURE_RGB)
	{
		Colors_G.MakeGLColor(crColor);
	}
 	glBegin(GL_LINES);
	glVertex3f(pt1[0],pt1[1],pt1[2]);
	glVertex3f(pt2[0],pt2[1],pt2[2]);
	glEnd();
}

void CGWorldOpenGLLine::ExpandRectangle(CViewMatrix * pView,CRect * pRect) const
{
	CPosMatrix ptPos;
	CPosMatrix transformedPos1;
	CPosMatrix transformedPos2;
	ptPos.Set(pt1[0],pt1[1],pt1[2]);
	ptPos.Multiply(&transformedPos1,*pView);
	transformedPos1.Normalize();

	ptPos.Set(pt2[0],pt2[1],pt2[2]);
	ptPos.Multiply(&transformedPos2,*pView);
	transformedPos2.Normalize();

	float fLeft=min(transformedPos1.GetX(),transformedPos2.GetX());
	float fRight=max(transformedPos1.GetX(),transformedPos2.GetX());
	float fTop=min(transformedPos1.GetY(),transformedPos2.GetY());
	float fBottom=max(transformedPos1.GetY(),transformedPos2.GetY());

	pRect->left=min((int)fLeft,pRect->left);
	pRect->right=max((int)fRight,pRect->right);
	pRect->top=min((int)fTop,pRect->top);
	pRect->bottom=max((int)fBottom,pRect->bottom);
}

CGWorldTriangleStrip::CGWorldTriangleStrip(int iNumVertices,COLORREF cr):CGWorldTriangleGroup(iNumVertices,cr)
{
}

CGWorldTriangleStrip::~CGWorldTriangleStrip()
{
}

void CGWorldTriangleStrip::DumpToGL() const
{
	glBegin(GL_TRIANGLE_STRIP);
	glRGB(GetRValue(m_crColor),GetGValue(m_crColor),GetBValue(m_crColor));
	for (int i=0;i<m_iNumVertices;i++)
	{
		glNormal3f(m_pVertices[i].m_fNormal[0],m_pVertices[i].m_fNormal[1],m_pVertices[i].m_fNormal[2]);
		glVertex3f(m_pVertices[i].m_fPos[0],m_pVertices[i].m_fPos[1],m_pVertices[i].m_fPos[2]);
	}
	glEnd();
};
void CGWorldTriangleStrip::DumpToVRML(CArchive& file) const
{
	TCHAR szBuffer[256];
	//write material properties
	_stprintf_s(szBuffer, _T("\tMaterial {\n\t\tdiffuseColor %f %f %f\n\t\tambientColor %f %f %f\n\t}\n"),	((float)GetRValue(m_crColor))/255.0f,((float)GetGValue(m_crColor))/255.0f,((float)GetBValue(m_crColor))/255.0f,	((float)GetRValue(m_crColor))/255.0f,((float)GetGValue(m_crColor))/255.0f,((float)GetBValue(m_crColor))/255.0f);
	file.WriteString(szBuffer);

	file.WriteString(_T("\tCoordinate3	{\n\t\tpoint [\n"));

	for (int i=0;i<m_iNumVertices;i++)
	{
		_stprintf_s(szBuffer, _T("\t\t\t%f %f %f"),m_pVertices[i].m_fPos[0],m_pVertices[i].m_fPos[1],m_pVertices[i].m_fPos[2]);
		file.WriteString(szBuffer);
		if (i!=m_iNumVertices-1)
		{
			file.WriteString(_T(",\n"));
		}
		else
		{
			file.WriteString(_T("\n"));
		}
	}
	file.WriteString(_T("\t\t]\n\t}\n"));
	file.WriteString(_T("\tIndexedFaceSet {\n\t\tcoordIndex [\n"));
	for (int i=0;i<m_iNumVertices;i++)
	{
		if ((i%2)==0)
		{
			_stprintf_s(szBuffer, _T("\t\t\t%i,%i,%i,-1"),i,(i+1)%m_iNumVertices,(i+2)%m_iNumVertices);
		}
		else
		{
			_stprintf_s(szBuffer, _T("\t\t\t%i,%i,%i,-1"),i,(i+2)%m_iNumVertices,(i+1)%m_iNumVertices);
		}
		file.WriteString(szBuffer);
		if (i!=m_iNumVertices-1)
		{
			file.WriteString(_T(",\n"));
		}
		else
		{
			file.WriteString(_T("\n"));
		}
	}
	file.WriteString(_T("\t\t]\n\t}\n"));
}

void CGWorldTriangleStrip::SetRectNormals()
{
	//Sets normal vectors for all the triangles assuming that
	//the represent a flat rectangle
	ASSERT(m_iNumVertices==4);	//if not,this is some other shape

	float fNormal[3];
	CalcNormal(m_pVertices[0].m_fPos,m_pVertices[1].m_fPos,m_pVertices[2].m_fPos,fNormal);
	for (int i=0;i<m_iNumVertices;i++)
	{
		m_pVertices[i].m_fNormal[0]=fNormal[0];
		m_pVertices[i].m_fNormal[1]=fNormal[1];
		m_pVertices[i].m_fNormal[2]=fNormal[2];
	}
}

void CGWorldTriangleStrip::SetRotationStripNormals()
{
	//Sets normal vectors for all the triangles assuming that
	//the represent a rectangular solid between two stations.
	ASSERT(m_iNumVertices==10);	//if not,this is some other shape
	//Could go to 11, but if the user fucked up the call into here
	//at least this won't crash

	//The pattern we used is aimed at given normals to the
	//vertices which are between the two flat surfaces.  Best is to
	//simply draw the pattern and figure out how this works for
	//yourself.
	for (int i=0;i<m_iNumVertices;i++)
	{
		int iOne,iThree;
		int iTwo=i;
		if (i%2==0)	//even
		{
			iOne=i-3;
			iThree=i+1;
		}
		else
		{
			iOne=i-1;
			iThree=i+3;
		}
		if (iOne<0)
			iOne+=10;
		if (iThree>=10)
			iThree-=10;
		CalcNormal(m_pVertices[iOne].m_fPos,m_pVertices[iTwo].m_fPos,m_pVertices[iThree].m_fPos,m_pVertices[i].m_fNormal);
	}
}


CGWorldTriangleFan::CGWorldTriangleFan(int iNumVertices,COLORREF cr):CGWorldTriangleGroup(iNumVertices,cr)
{
}

CGWorldTriangleFan::~CGWorldTriangleFan()
{
}

void CGWorldTriangleFan::DumpToGL() const
{
	glBegin(GL_TRIANGLE_FAN);
	glRGB(GetRValue(m_crColor),GetGValue(m_crColor),GetBValue(m_crColor));
	for (int i=0;i<m_iNumVertices;i++)
	{
		glNormal3f(m_pVertices[i].m_fNormal[0],m_pVertices[i].m_fNormal[1],m_pVertices[i].m_fNormal[2]);
		glVertex3f(m_pVertices[i].m_fPos[0],m_pVertices[i].m_fPos[1],m_pVertices[i].m_fPos[2]);
	}
	glEnd();
}

void CGWorldTriangleGroup::ExpandRectangle(CViewMatrix * pViewPort,CRect * pRect) const
{
	CPosMatrix ptPos;
	CPosMatrix transformedPos;
	for (int i=0;i<m_iNumVertices;i++)
	{
		ptPos.Set(m_pVertices[i].m_fPos[0],m_pVertices[i].m_fPos[1],m_pVertices[i].m_fPos[2]);
		ptPos.Multiply(&transformedPos,*pViewPort);
		transformedPos.Normalize();
		pRect->left=min(pRect->left,(int)transformedPos.GetX());
		pRect->right=max(pRect->right,(int)transformedPos.GetX());
		pRect->top=min(pRect->top,(int)transformedPos.GetY());
		pRect->bottom=max(pRect->bottom,(int)transformedPos.GetY());
	}
}

void CGWorldTriangleFan::DumpToVRML(CArchive& file) const
{
	TCHAR szBuffer[256];
	//write material properties
	_stprintf_s(szBuffer, _T("\tMaterial {\n\t\tdiffuseColor %f %f %f\n\t\tambientColor %f %f %f\n\t}\n"),	((float)GetRValue(m_crColor))/255.0f,((float)GetGValue(m_crColor))/255.0f,((float)GetBValue(m_crColor))/255.0f,	((float)GetRValue(m_crColor))/255.0f,((float)GetGValue(m_crColor))/255.0f,((float)GetBValue(m_crColor))/255.0f);
	file.WriteString(szBuffer);

	file.WriteString(_T("\tCoordinate3	{\n\t\tpoint [\n"));

	for (int i=0;i<m_iNumVertices;i++)
	{
		_stprintf_s(szBuffer, _T("\t\t\t%f %f %f"),m_pVertices[i].m_fPos[0],m_pVertices[i].m_fPos[1],m_pVertices[i].m_fPos[2]);
		file.WriteString(szBuffer);
		if (i!=m_iNumVertices-1)
		{
			file.WriteString(_T(",\n"));
		}
		else
		{
			file.WriteString(_T("\n"));
		}
	}
	file.WriteString(_T("\t\t]\n\t}\n"));
	file.WriteString(_T("\tIndexedFaceSet {\n\t\tcoordIndex [\n"));
	for (int i=0;i<m_iNumVertices;i++)
	{
		_stprintf_s(szBuffer, _T("\t\t\t%i,%i,%i,-1"),0,(i+1)%m_iNumVertices,(i+2)%m_iNumVertices);
		file.WriteString(szBuffer);
		if (i!=m_iNumVertices-1)
		{
			file.WriteString(_T(",\n"));
		}
		else
		{
			file.WriteString(_T("\n"));
		}
	}
	file.WriteString(_T("\t\t]\n\t}\n"));
}

