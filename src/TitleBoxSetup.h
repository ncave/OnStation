// TitleBoxSetup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTitleBoxSetup dialog
class COnStationView;
class CTitleBoxSetup : public CDialog
{
// Construction
public:
	CTitleBoxSetup(COnStationView *View,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTitleBoxSetup)
	enum { IDD = IDD_TITLEPAGESETUP };
	CString	m_szTitle;
	CString	m_szComments;
	CString	m_szLocation;
	CString	m_szSurveyedBy;
	CString	m_szLength;
	//}}AFX_DATA
	COnStationView *m_MyView;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTitleBoxSetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTitleBoxSetup)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
