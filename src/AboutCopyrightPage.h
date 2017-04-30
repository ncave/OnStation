#ifndef I_ABOUTCOPYRIGHT__
#define I_ABOUTCOPYRIGHT__

class CAboutDlg : public CPropertyPage
{
public:
	CAboutDlg();
	CFont m_MyFontTitle;
	CFont m_MyFontAuthor;
	CFont m_MyFontWarning;

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//{{AFX_MSG(CAboutDlg)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif
