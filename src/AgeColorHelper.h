// AgeColorHelper.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAgeColorHelper dialog

class CAgeColorHelper : public CDialog
{
// Construction
public:
	CAgeColorHelper(DWORD dwStartDate,int iNum,int iYears,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAgeColorHelper)
	enum { IDD = IDD_AGECOLORHELPERDIALOG };
	UINT	m_iYears;
	UINT	m_iNumIntervals;
	//}}AFX_DATA

	DWORD m_dwStartDate;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAgeColorHelper)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAgeColorHelper)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
