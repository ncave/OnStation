// PageLayoutPreferences.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageLayoutPreferences dialog

class COnStationView;
class CPageLayoutPreferences : public CDialog
{
// Construction
public:
	CPageLayoutPreferences(COnStationView *,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPageLayoutPreferences)
	enum { IDD = IDD_PAGELAYOUT };
	BOOL	m_bThickLines;
	BOOL	m_bIncludeArrow;
	BOOL	m_bIncludeScale;
	BOOL	m_bIncludeSideScale;
	int		m_iPageOverlap;
	BOOL	m_bShowTitleBox;
	BOOL	m_bNameByYear;
	//}}AFX_DATA

	COnStationView * m_View;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageLayoutPreferences)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPageLayoutPreferences)
	afx_msg void OnEditTitlePage();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
