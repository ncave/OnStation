#ifndef I_ALTITUDESHEET__
#define I_ALTITUDESHEET__

// altitudesheet.h : header file
//
#include "surfaceeditpage.h"
#include "surfacesizepage.h"

/////////////////////////////////////////////////////////////////////////////
// CAltitudesSheet
class COnStationDoc;
class CSurfacePropertySheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CSurfacePropertySheet)
private:
	CSurfaceEditPage m_SurfaceEditPage;
	CSurfaceSizePage m_SurfaceSizePage;
	COnStationDoc *m_MyDocument;

	void SetupPages();
// Construction
public:
	CSurfacePropertySheet(COnStationDoc *,UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CSurfacePropertySheet(COnStationDoc *,LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

	BOOL NeedRecalculation();
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSurfacePropertySheet)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSurfacePropertySheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSurfacePropertySheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif

