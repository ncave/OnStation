// TipOfTheDay.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTipOfTheDay dialog

class CTipOfTheDay : public CDialog
{

	// Construction
public:
	CTipOfTheDay(CWnd* pParent = NULL);   // standard constructor
	int m_iTipIndex;
	CString m_szText;
// Dialog Data
	//{{AFX_DATA(CTipOfTheDay)
	enum { IDD = IDD_TIPOFTHEDAY };
	CStatic	m_szTip;
	BOOL	m_ShowOnStartup;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTipOfTheDay)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTipOfTheDay)
	virtual BOOL OnInitDialog();
	afx_msg void OnNexttip();
	virtual void OnOK();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
