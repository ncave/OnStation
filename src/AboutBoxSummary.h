// AboutBoxSummary.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAboutBoxSummary dialog

class CAboutBoxSummary : public CPropertyPage
{
//	DECLARE_DYNCREATE(CAboutBoxSummary)

// Construction
public:
	CAboutBoxSummary(CWnd *pParent=NULL);

	CFont m_MyFontTitle;
	CFont m_MyFontAuthor;
// Dialog Data
	//{{AFX_DATA(CAboutBoxSummary)
	enum { IDD = IDD_ABOUTBOXSUMMARY };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CAboutBoxSummary)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAboutBoxSummary)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
