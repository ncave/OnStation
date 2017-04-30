// DepthColorHelperDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDepthColorHelperDialog dialog

class CDepthColorHelperDialog : public CDialog
{
// Construction
public:
	CDepthColorHelperDialog(float fStartDepth,float fIncr,int iNum,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDepthColorHelperDialog)
	enum { IDD = IDD_DEPTHCOLORHELPERDIALOG };
	CStatic	m_szStartingText;
	CStatic	m_szIntervalText;
	UINT	m_iNumIntervals;
	float	m_fDepth;
	float	m_fStartDepth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDepthColorHelperDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDepthColorHelperDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
