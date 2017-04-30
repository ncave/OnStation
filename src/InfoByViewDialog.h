// InfoByViewDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInfoByViewDialog dialog
class CNodeTreeSummary;
class CInfoByViewDialog : public CDialog
{
// Construction
public:
	CInfoByViewDialog(CNodeTreeSummary *NTS,CWnd* pParent = NULL);   // standard constructor
	CNodeTreeSummary *m_NTS;
// Dialog Data
	//{{AFX_DATA(CInfoByViewDialog)
	enum { IDD = IDD_FOLDERINFOBYVIEW };
	CString	m_szDeepestPoint;
	CString	m_szHighestPoint;
	CString	m_NumShots;
	CString	m_NumSurfaceShots;
	CString	m_TotalLength;
	CString	m_TotalSurfaceLength;
	CString m_HorizontalLength;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInfoByViewDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInfoByViewDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
