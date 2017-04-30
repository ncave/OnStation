// (C) Copyright 1994-1995  Taco van Ieperen
//
// This class encapsulates the Node class.  A survey consists of a set of nodes
// connected by a set of shots.  A node could be considered equivalent to a survey
// station.
//
// The node class contains its name and a list of all the survey shots
// that reference the given node.  Both incoming and outgoing shots are tracked,
// giving us a doubly linked tree of nodes.
//
// Each node has two positions.  The m_RawPos is the position after rough
// calculation of the node's spot in 3 space.  The m_CookedPos is the final
// position after secondary processing.  This value is filled in the the
// loop closure routines.
//
// There is also an array of 4 positions and visibility flags.  While the m_RawPos
// and the m_CookedPos represent absolute positions, the array of positions
// represent the position in screen coordinates.  These values are recalculated
// every time we move, whereas the other values are calculated only when editing
// has been done to the system.
//
//
#ifndef I_NODE__
#define I_NODE__


#include "surveyshot.h"
#include "nodepositioninfo.h"
#include "junctionbox.h"
//Even nodes with no wall data are assumed to have a 1x1m wall.  This allows us to
//simplify our code TREMENDOUSLY!

enum NODE_SHOTTYPE {NST_ANYSHOT,NST_INCOMINGSHOT,NST_OUTGOINGSHOT,NST_GLOBALSHOT_IGNORE_COLOR_SCHEME};



class CNodeTree;
class CSurfaceTieIn;
class COnStationDoc;
class CSurveyShot;

class CNode
{
	friend class CClose;	//CClose is a C thing which doesn't understand information hiding
    private:	//DON'T EVER TOUCH THESE
    	TCHAR m_szNodeName[MAX_STATION_NAME_LENGTH];			//name of the station for this node

    	CSurveyShotArray m_SurveyShotList;		//List of survey shots
		
		CSurveyShotArray m_WallsOnlyShots;		//list of shots with no TO nodes

	private:
		//TRUE if this node matched the search conditions of the previous
		//query (normally when a comment for the node matches).
		BYTE  m_byMatchesQuery;	

		//closed and unclosed position for each seperate color scheme
		CNodePositionInfo m_Position;

	public:	//NODE TREE ACCESS MEMBERS.  Nobody outside of CNodeTree
			//should need to use these
    	CNode(LPCTSTR szName);
		~CNode();

#ifdef _DEBUG
		void Dump();
#endif

		inline CNodePositionInfo * GetNodePositionInfo()
			{ return &m_Position;}

		//Add and remove survey shots
        void  AddShot(CSurveyShot *shot);
        BOOL  RemoveShot(CSurveyShot *shot);	//return TRUE if not empty

		//Set and retrieve how far we have gotten in processing this node
		NODE_MARKTYPE GetMarkType() const;
		void SetMarkType(NODE_MARKTYPE);
		void DoProcessingOfWallData();

        inline BOOL  IsEmpty() const {return (m_SurveyShotList.GetSize()==0 && m_WallsOnlyShots.GetSize()==0);}

	public:
	    inline LPCTSTR GetName() const {return m_szNodeName;}
		CSurveyLeg * GetLeg();

		inline BOOL MatchesQuery() const {return m_byMatchesQuery;}
		inline void SetMatchesQuery(BOOL bMatch) {m_byMatchesQuery=bMatch;}

		BOOL NeedATextLabel(int iColorScheme);

		//Simply junctions exist at nodes which have incoming and outgoing shots
		//such that it is "obvious" what the cross section should be.  The rule
		//we use is as follows:


		// 1 incoming and 1 outgoing shot.  Use the outgoing U/D/L/R and use the
		//average of the incoming and outgoing compass and clino readings to set
		//the direction of the cross section.
		//1 incoming shot, 0 outgoing.  Use the U/D/L/R for the standalone shot (if exists)
		//and the direction is the incoming direction
		//1 outgoing shot, 0 incoming.  Use the U/D/L/R for the outgoing shot and the
		//direction is the outgoing direction.
		//2 outgoing shots, 0 incoming.  Average the U/D/L/R for the two and take the
		//directional average.
		//2 incoming shots, 0 outgoing.  Use the U/D/L/R for the standalone shot (if exists)
		//and the directions is the average of the two incoming directions.
		BOOL HasSimpleJunction();
		//If the node has a simple junction, this function returns the coordinates
		//of the four corners.
		void CreateSimpleJunctionBoxIncoming(CSurveyShot *shot,CJunctionBox *);
		void CreateSimpleJunctionBoxOutgoing(CSurveyShot *shot,CJunctionBox *);

		void CreateComplexJunctionBoxIncoming(CSurveyShot * pIncomingShot,CJunctionBox * pBox);
		void CreateComplexJunctionBoxOutgoing(CSurveyShot * pOutgoingShot,CJunctionBox * pBox);
		void CreateHeightJunctionBox(CJunctionBox *);

		void CreateComplexJunctionGeometry(CJunctionGeometryBox * pGeometry,BOOL bClosedLoops);

		void SetConstrainedPosition(float fX,float fY,float fZ);
		CPosMatrix * GetPosition(BOOL bClosedLoop);

		//New information functions
		INT_PTR GetNumberOfShots(NODE_SHOTTYPE);
		CSurveyShot * GetShotByIndex(INT_PTR iIndex, NODE_SHOTTYPE);
		INT_PTR GetNumberOfWallOnlyShots();
		CSurveyShot * GetWallOnlyByIndex(INT_PTR iIndex);

		BOOL IsSurfaceNode();
};


#endif
