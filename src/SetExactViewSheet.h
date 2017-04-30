// SetExactViewSheet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetExactViewSheet
class COnStationView;
#include "viewanglepage.h"
#include "centerpage.h"
#include "viewscalePage.h"
class COnStationDoc;
class CSetExactViewSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CSetExactViewSheet)

private:
	// Construction
	float m_fClockwise;
	float m_fTilt;
	float m_fDx;
	float m_fDy;
	float m_fDz;
	float m_fScaling;
	
//	CSizePage		m_SizePage;
//	CMovementPage	m_MovementPage;

// Construction
public:
	//Don't use this constructor
	CSetExactViewSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	//Use this one instead
	CSetExactViewSheet(COnStationView *view,LPCTSTR pszCaption,float fClock,float fTilt,float fScale,
											float fDx,float fDy,float fDz, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

	//Put these here since the will contain the final data.
	CViewAnglePage m_ViewPage;
	CCenterPage    m_ViewCenter;
	CViewScalePage m_ViewScale;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetExactViewSheet)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSetExactViewSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSetExactViewSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
