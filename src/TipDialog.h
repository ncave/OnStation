// CG: This file added by 'Tip of the Day' component.

/////////////////////////////////////////////////////////////////////////////
// CTipDialog dialog

class CTipDialog : public CDialog
{
// Construction
public:
	CTipDialog(CWnd* pParent = NULL);	 // standard constructor

// Dialog Data
	//{{AFX_DATA(CTipDialog)
	enum { IDD = IDD_TIP };
	BOOL	m_bStartup;
	CString	m_strTip;
	//}}AFX_DATA

protected:
	int m_iLinePos;
	CStringArray m_tips;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTipDialog)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTipDialog)
	afx_msg void OnNextTip();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void LoadTips();
	void GetNextTipString();
};

