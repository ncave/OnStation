// CenterStationNameDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCenterStationNameDialog dialog

class CCenterStationNameDialog : public CDialog
{
// Construction
public:
	CCenterStationNameDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCenterStationNameDialog)
	enum { IDD = IDD_CENTERSTATION };
	CString	m_szEditStation;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCenterStationNameDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCenterStationNameDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
