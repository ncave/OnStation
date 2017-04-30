#ifndef I_EDITTIEIN__
#define I_EDITTIEIN__

/////////////////////////////////////////////////////////////////////////////
// CEditFixedPointDialog dialog
class CFixedPoint;
class COnStationDoc;
class CEditFixedPointDialog : public CDialog
{
private:
	CFixedPoint *m_TieIn;
	COnStationDoc * m_MyDocument;
// Construction
public:
	CEditFixedPointDialog(COnStationDoc *,CFixedPoint *,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEditFixedPointDialog)
	enum { IDD = IDD_EDITFIXEDPOINT };
	CStatic	m_WarningText;
	CString	m_StaticZ;
	float	m_fXpan;
	float	m_fYpan;
	float	m_fZpan;
	CString	m_szComment;
	CString	m_szStationText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditFixedPointDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditFixedPointDialog)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


#endif






