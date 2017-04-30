// ViewScalePage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewScalePage dialog

class CViewScalePage : public CPropertyPage
{
// Construction
public:
	CViewScalePage(float fScale,CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(CViewScalePage)
	enum { IDD = IDD_VIEWSCALE };
	CEdit	m_SizeControl;
	CStatic	m_szLimits;
	CStatic	m_szDescribeText;
	float m_fMetricScale;
	float	m_fScale;
	//}}AFX_DATA
	BOOL m_bMetric;

// Overrides
	virtual BOOL OnKillActive();
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewScalePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CViewScalePage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
