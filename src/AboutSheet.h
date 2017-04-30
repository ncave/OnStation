// AboutSheet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAboutSheet
#include "aboutcopyrightpage.h"
#include "aboutboxsummary.h"
#include "aboutcreditsdialog.h"
#include "aboutweb.h"

class CAboutSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CAboutSheet)
private:
	CAboutDlg m_CopyrightPage;
	CAboutCreditsDialog m_CreditsPage;
	CAboutBoxSummary m_SummaryPage;
	CAboutWeb	m_AboutWebPage;	
// Construction
public:
	CAboutSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CAboutSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutSheet)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAboutSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAboutSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
