#ifndef I_EDITSHEET__
#define I_EDITSHEET__
// editsheet.h : header file
//
#include "surveyleg.h"
#include "legsummarypage.h"
#include "leggridpage.h"
#include "legpeoplepage.h"
#include "leginstrumentspage.h"
#include "legEditorSettingspage.h"

/////////////////////////////////////////////////////////////////////////////
// CEditSheet

class CEditSheet : public CPropertySheet
{
private:
// Construction
public:
	CEditSheet(CSurveyLeg *Leg,UINT uiCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:
	CSurveyLeg *m_Leg;
	BOOL NeedRecalculation();
private:
	CLegGridPage m_GridPage;
	CLegSummaryPage m_SummaryPage;
	CLegInstrumentsPage m_InstrumentPage;
	CLegPeoplePage m_PeoplePage;
	CLegEditorSettingsPage m_EditorSettingsPage;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditSheet)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEditSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEditSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif



