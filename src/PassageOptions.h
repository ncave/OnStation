// PassageOptions.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPassageOptions dialog

class CPassageOptions : public CDialog
{
// Construction
public:
	CPassageOptions(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(CPassageOptions)
	enum { IDD = IDD_PASSAGE_OPTIONS };
	CComboBox	m_ComboBox;
	BOOL	m_bMultipleLights;
	BOOL	m_bDisableLightRotate;
	BOOL	m_bWireFrameRotate;
	//}}AFX_DATA
	int m_iCurComboSel;
	PASSAGE_TYPE m_PassageType;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPassageOptions)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPassageOptions)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
