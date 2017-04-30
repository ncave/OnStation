#ifndef I_COLORSHEET__
#define I_COLORSHEET__
// ColorSheet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorSheet
#include "agecolorpage.h"
#include "depthcolorpage.h"

class CColorSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CColorSheet)

	CAgeColorPage m_AgePage;
	CDepthColorPage m_DepthPage;

// Construction
public:
//	CColorSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	//StartScheme 0=age, 1=depth
	CColorSheet(UINT nIDCaption, int iStartColorScheme,CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:
	BOOL IsDirty();
	int m_iRecommendedColorSchemeIndex;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorSheet)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColorSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif

