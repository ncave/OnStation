// DepthLevelDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDepthLevelDialog dialog

class CDepthLevelDialog : public CDialog
{
// Construction
public:
	CDepthLevelDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDepthLevelDialog)
	enum { IDD = IDD_HOWMANYDEPTHINTERVALS };
	int		m_DepthLevel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDepthLevelDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDepthLevelDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
