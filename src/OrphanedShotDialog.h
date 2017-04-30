// OrphanedShotDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COrphanedShotDialog dialog
class COnStationDoc;
class COrphanedShotDialog : public CDialog
{
private:
	COnStationDoc *m_MyDocument;
	int m_iColorScheme;
	// Construction
public:
	COrphanedShotDialog(COnStationDoc *document,int iColorScheme,CWnd* pParent = NULL);   // standard constructor
	BOOL m_bDirty;


// Dialog Data
	//{{AFX_DATA(COrphanedShotDialog)
	enum { IDD = IDD_ORPHANDIALOG };
	CListBox	m_OrphanList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COrphanedShotDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COrphanedShotDialog)
	afx_msg void OnSurfaceTieIn();
	afx_msg void OnEditSurvey();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
