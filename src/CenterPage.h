// CenterPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCenterPage dialog
class COnStationView;
class CCenterPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CCenterPage)

// Construction
public:
	CCenterPage();
	CCenterPage(float fXpos,float fYpos,float fZpos);
	~CCenterPage();

	void Initialize(COnStationView *View);
	COnStationView * m_MyView;
// Dialog Data
	//{{AFX_DATA(CCenterPage)
	enum { IDD = IDD_VIEWCENTER };
	CString	m_StaticX;
	CString	m_StaticY;
	CString	m_StaticZ;
	float	m_fXpos;
	float	m_fYpos;
	float	m_fZpos;
	float	m_fMetricX;
	float	m_fMetricY;
	float	m_fMetricZ;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCenterPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCenterPage)
	afx_msg void OnSetCenterStation();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
