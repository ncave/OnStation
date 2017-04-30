// viewdial.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewAnglePageog dialog
class CViewAnglePage : public CPropertyPage
{
// Construction
public:
	DECLARE_DYNCREATE(CViewAnglePage)

	CViewAnglePage();
	CViewAnglePage(float fRotate,float fElevate);	// standard constructor
	
// Dialog Data
	//{{AFX_DATA(CViewAnglePage)
	enum { IDD = IDD_VIEWANGLE };
	float	m_fRotate;
	float	m_fElevate;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	// Generated message map functions
	//{{AFX_MSG(CViewAnglePage)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
