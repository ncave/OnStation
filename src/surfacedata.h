#ifndef I_SURFACEDATA__
#define I_SURFACEDATA__

#include "gworld.h"
//This class holds surface information.
//
// Currently we define the surface to be some rectangular area bounded by
// extremes in each direction.  It is broken into squares of a set size,
// and for each square, altitude information is provided.  Altitudes are
// initialized to NAN, and NAN squares are not plotted.
//
// This class maintains old data whenever possible if the surface is
// resized.  The IsDataLost() method will tell you if a resize will loose
// any data.
//
// Data is stored in an array of floats.
//
// TODO:  Add texture data to allow surface to be colored etc...
// TODO:  Add support for Lat/Long and satelite data.
class CFixedPoint;
class CFileHelper;
#define DEF_SLEFT	-500.0f
#define DEF_SRIGHT	500.0f
#define DEF_STOP	500.0f
#define DEF_SBOTTOM	-500.0f
#define DEF_SSIZE	100.0f

class CNode;
class CNodeTree;
class CGWorld;
class CRealFolder;
class COnStationDoc;
class CNodeTreeSummary;
class CSurfaceData:public CObject
{
	private:
		float m_fLeft;
		float m_fRight;
		float m_fTop;
		float m_fBottom;
		float m_fSquareSize;
		float m_fGridNorth;

		int m_iGridWidth;
		int m_iGridHeight;

		float * m_Altitudes;
		CPtrList m_FixedPointList;
		COnStationDoc * m_MyDocument;

	public:
		CSurfaceData(COnStationDoc  * Doc,float fGridNorth=0.0f,float left=DEF_SLEFT,float top=DEF_STOP,float right=DEF_SRIGHT,float bottom=DEF_SBOTTOM,float size=DEF_SSIZE);
		~CSurfaceData();

		BOOL IsEmpty();
		void ResetAll();
		void RemoveAllFixedPoints();
		float ConvertXToUTM(float X);
		float ConvertYToUTM(float Y);

		float ConvertXToLocal(float X);
		float ConvertYToLocal(float Y);

		//Surface tie in management
		CFixedPoint * GetFixedPoint(INT_PTR i);
		INT_PTR GetFixedPointCount();
		void RemoveFixedPoint(CFixedPoint *);
		
		//returns FALSE and deletes point if it is a duplicate
		BOOL AddFixedPoint(CFixedPoint *);

		void DeleteContents();

		CNode * GetSuggestedStartingNode(int iIndex);
		//These locations are in UMT units.
		void SetDimensions(float left,float top,float right,float bottom,float size);

		void GetViewInfo(CNodeTreeSummary *NTS);
		
		BOOL ReadRawSurfaceData(CFileHelper * h);
		void GlobalSetSurfacePositions();
		BOOL IsConstrainedNode(LPCTSTR szData,int iColorScheme);

//Information functions
		float GetGridNorth() const {return m_fGridNorth;}
		void SetGridNorth(float f);
//		float GetAltitude(float x,float y);
		void SetAltitude(float x,float y,float altitude);
		float GetAltitude(int x,int y);
		void SetAltitude(int x,int y,float altitude);
		int GetGridWidth() {return m_iGridWidth;}
		int GetGridHeight() {return m_iGridHeight;}
		float GetSquareSize() {return m_fSquareSize;}
		float GetLeft() {return m_fLeft;}
		float GetRight() {return m_fRight;}
		float GetTop() {return m_fTop;}
		float GetBottom() {return m_fBottom;}
		
		void CalculateGWorld(CGWorld *GW);

		void Write(CFileHelper * h);
		void WriteFixedPoints(CFileHelper *h);

		void ReadSurface(CFileHelper * h);
		void ReadFixedPoints(CFileHelper *h);	//only does one at a time.
		BOOL  CalculateAltitude(float x,float y,float * result);

		float GetMiddleAltitude();
};

#endif


