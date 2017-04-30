// AboutCreditsDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAboutCreditsDialog dialog

class CAboutCreditsDialog : public CPropertyPage
{
// Construction
public:
	CAboutCreditsDialog(CWnd* pParent = NULL);   // standard constructor

	CFont m_MyFontTitle;
	CFont m_MyFontAuthor;
// Dialog Data
	//{{AFX_DATA(CAboutCreditsDialog)
	enum { IDD = IDD_ABOUTBOXCREDITS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutCreditsDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAboutCreditsDialog)
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
