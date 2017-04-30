// DistanceBetweenDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDistanceBetweenDialog dialog

class CDistanceBetweenDialog : public CDialog
{
// Construction
public:
	CDistanceBetweenDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDistanceBetweenDialog)
	enum { IDD = IDD_DISTANCEBETWEENSTATIONS };
	CString	m_szStartingStation;
	CString	m_szEndingStation;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDistanceBetweenDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDistanceBetweenDialog)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
