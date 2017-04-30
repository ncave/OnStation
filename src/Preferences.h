// Preferences.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPreferences dialog

class CPreferences : public CDialog
{
// Construction
public:
	CPreferences(BOOL bStatusBar,BOOL bToolBar,BOOL bRuler,BOOL bIndexTabs,BOOL bCompass,PREFEREDUNITS Units,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPreferences)
	enum { IDD = IDD_PREFERENCES };
	BOOL	m_bStatusBar;
	BOOL	m_bToolBar;
	BOOL	m_bRuler;
	BOOL	m_bIndexTabs;
	BOOL	m_bCompass;
	//}}AFX_DATA
	PREFEREDUNITS m_PreferedUnits;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPreferences)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPreferences)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
