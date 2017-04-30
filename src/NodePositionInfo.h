#ifndef I_NODEPOSITIONINFO__
#define I_NODEPOSITIONINFO__

enum NODE_MARKTYPE {NMT_NOTHING,NMT_MAINSURVEY,NMT_MISSINGSECTION};
//This class encapsulates the position of a node.  I have factored it out
//because each node can have a different position depending on what view
//you are currently looking at.  That is to say that each view has it's
//own position for each node.  It eats a ton of memory, but boy is
//it slick when you are turning things on and off.
class CNode;
class CNodePositionInfo
{
public:
	CNodePositionInfo();
	~CNodePositionInfo();

	void SetConstrainedPosition(float fX,float fY,float fZ);
	void Copy(CNodePositionInfo * NPI);

	CPosMatrix m_RawPosition;   			//Original 3 space position
    CPosMatrix m_ClosedPosition;			//Position after secondary processing (loop closure)

	short m_iTotalOfAllShots;		//total number off shots that come in or out of this node.
	short m_iIncomingShots;		//number of shots to this node.
	short m_iOutgoingShots;
	short m_iWallOnlyShots;		//number of survey shots that do nothing but define wall data
								//(ie, they have not TO station).

	//We mark when we have hit this spot.  We start by marking everything as NMT_NOTHING
	//and then later mark things as NMT_SURVEY (indicating that it is part of the main map)
	//or as NMT_MISSINGSECTION indicating that it was not hit during calculations.  The
	//NMT_MISSINGSECTION flag is used because we do some calculations on the missing sections
	//to figure out how many of them there might be.
	NODE_MARKTYPE m_Marked;
	
	//used by the loop closure code only.  This marks a node as constrained or unconstrained
	//I believe
	short m_iNodeType;
};


#endif

