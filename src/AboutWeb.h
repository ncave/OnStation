// AboutWeb.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAboutWeb dialog

class CAboutWeb : public CPropertyPage
{
	DECLARE_DYNCREATE(CAboutWeb)

// Construction
public:
	CAboutWeb();
	~CAboutWeb();

	CString m_szBrowser;
// Dialog Data
	//{{AFX_DATA(CAboutWeb)
	enum { IDD = IDD_ABOUTBOXWEB };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CAboutWeb)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAboutWeb)
	afx_msg void OnHome();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
