// (C) Copyright 1994-1995  Taco van Ieperen

// This is the primary data structure containing the cave.  Each station
// creates a "Node" in this data structures.  For each station a list of
// all the shots to and from that station is maintained.  The end result
// is that we have a richly interconnected web where we can navigate along
// survey shots with ease.
//
// We calculate positions in two manners.  For a preview of a local survey
// we recurse from the first station in the local survey.  Positions are
// generated on the fly.  Recursion is ended by a marker indicating that a
// station has already been reached, and by a counter which only draws two
// shots outside of the active survey.
//
// For drawing the whole system, we calculate the each node position using
// an iterative method.  This prevents stack overflow in the recursion.
// After this, we make a call to a clipping routine that calculates the
// visibility of all nodes in the system.  The actual rendering is done
// by walking through all of the nodes top to bottom.  All shots leaving a
// node are drawn.


#ifndef I_NODETREE__
#define I_NODETREE__
#include "node.h"

const int HASHSIZE=731;

enum LINE_TYPE;

class CNodeTreeSummary
{
public:
	CNodeTreeSummary();
	~CNodeTreeSummary();

	//This will eventually be used by the printing code
	//to figure out how big the cave will be when printed
	//It is not being used right now and can be safely deleted.
	float m_fRight;
	float m_fLeft;
	float m_fTop;
	float m_fBottom;
	float m_fViewDeepest;
	float m_fViewHighest;

	float m_fDeepest;
	TCHAR m_szDeepest[MAX_STATION_NAME_LENGTH];
	float m_fHighest;
	TCHAR m_szHighest[MAX_STATION_NAME_LENGTH];
	float m_fSurfaceLength;
	float m_fCaveLength;
	float m_fHorizontalLength;
	int m_iCaveShots;
	int m_iSurfaceShots;
};

class CSurveyLeg;
class CSurfaceData;
class CGWorld;
class COnStationDoc;
class CNode;
class CSurveyShot;
class CNodePositionInfo;
class CRealFolder;
class CNodeTree
{
    private:
		COnStationDoc * m_MyDocument;

		//Array of all of the nodes.  We hash them to do quick finds.
		CPtrList * m_HashTable[HASHSIZE];

	   	//Adds a new named node or returns current one if there is one already
    	void RemoveNode(CNode *);

		// Removes calculated flag from all nodes in the system.
		void UnmarkAllNodes();
		//Time of last draw.  This was stamped on stations last time we refreshed,
		//so we want to make sure we don't use it again in case of quick updates
        long m_lLastTime;

		int  m_iNumberOfNodes;

		//The only time the Loop Closure data is considered accurate is when
		//it has been calculated.  Adding or removing nodes makes it dirty again.
		BOOL m_bIsClosureDataDirty[TOTAL_COLOR_SCHEMES];

		//returns hash value for a node name
		WORD HashName(LPCTSTR szName);
		
		//Helper functions
		void AddLineToGWorld(CGWorld *GW,COLORREF crColor,CPosMatrix & From,CPosMatrix & To,BOOL bMatchesQuery,LINE_TYPE lineType);
//		void AddRectangleToGWorld(CGWorld *GW,CSurveyLeg *Leg,CPosMatrix& topLeft,CPosMatrix& topRight,CPosMatrix& bottomRight,CPosMatrix& bottomLeft,BOOL bMatchesQuery,LIGHTINGMODIFIER LM);
		void AddLabelToGWorld(CGWorld *GW,CNode *node,CPosMatrix& Pos);
		void AddDepthLineToGWorld(CGWorld *GW,CPosMatrix& From,CPosMatrix& To,LINE_TYPE lineType);
//		void AddDepthRectangleToGWorld(CGWorld *GW,CSurveyLeg *Leg,CPosMatrix& topLeft,CPosMatrix& topRight,CPosMatrix& bottomRight,CPosMatrix& bottomLeft);
		void ConvertToPassageCrossSection(CNodePositionInfo * NPI,CPosMatrix& Pos,CPosMatrix& pTopLeft,CPosMatrix& pTopRight,CPosMatrix& pBottomRight,CPosMatrix& pBottomLeft);
		void DrawVolumeCrossSection(CGWorld *GW,CNode *node,CPosMatrix& Pos,BOOL bSolid);
		void CalculateDockingStation(CSurveyShot *shot,BOOL bIncoming,CPosMatrix& pTopLeft,CPosMatrix& pTopRight,CPosMatrix& pBottomRight,CPosMatrix& pBottomLeft);
		void CalculateLeftRightWalls(CPosMatrix& pos,CPosMatrix * pLeft,CPosMatrix * pRight,CNodePositionInfo * NPI);
		void GenerateNodeJunctionGeometry(CNode *pNode,CGWorld *GW,CSurveyLeg *pLeg,BOOL bMatchesQuery);

		CPtrList m_NodesNotOnSurvey;

	public:
		CNodeTree(COnStationDoc *document);
		~CNodeTree();

		// Stores the current state of the blunder stress calculator
		int  m_iBlunderMode;

		//Remove the cached results of the last search from the node list.
		//Returns TRUE if anything changed
		BOOL UnmarkQueryAllNodes();

		//Finds a node.  Optionally adds it if it doesn't exist
		CNode * FindNode(LPCTSTR szName,BOOL bAdd=FALSE);
	
		void GetCurrentSizeExtremes(LPRECT r);			//Get extreme X and Y ranges in meters.  Used
		CNode * MakeUpAStartingNode(CRealFolder *myroot,int iColorScheme);
													//for printing size determinations
		
		//Removes shots from the system.  Removes nodes if they are no
		//longer referenced.
		void RemoveShot(CSurveyShot *);

		//Adds shot to the system.  Adds the endpoint nodes if necessary.
		void AddShot(CSurveyShot *);

		// This calculates system locations without loop closures in the
		// standard coordinate system.
		void CalculateRawPositions(CSurfaceData *data,int iColorScheme);
		void CalculateRawPositionsHelper(CNode *node,int iColorScheme,NODE_MARKTYPE);
		BOOL CloseLoops(int iColorScheme);
		void FillGWorld(CGWorld *World);

		//Removes all nodes from system.  Clean up memory.  Helper function for
		//destructor.
		void DeleteContents();

		void RenderShot(CGWorld* GW,CSurveyShot *shot);

		int GetNumberOfNodes() const {return m_iNumberOfNodes;}
		BOOL IsClosureDataDirty(int iColorScheme) const {return m_bIsClosureDataDirty[iColorScheme];}

		CNode * GetMissingSectionNode(int iIndex);
		INT_PTR GetNumberOfMissingSections() const;

		void GetViewInfo(CNodeTreeSummary *NTS);

		void AddBlundersToListBox(CListBox * pList,int iDeviation);

		void SetBlunderMode(int iMode);

		int GetBlunderMode();
};
    	


#endif


