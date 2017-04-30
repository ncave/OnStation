// SurveySearchDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// SurveySearchDialog dialog
class COnStationDoc;
class CLegQuery;
class SurveySearchDialog : public CDialog
{
// Construction
public:
	SurveySearchDialog(COnStationDoc * pDocument,CWnd* pParent = NULL);   // standard constructor
	CLegQuery * m_pLegQuery;
// Dialog Data
	//{{AFX_DATA(SurveySearchDialog)
	enum { IDD = IDD_SEARCHSURVEYS };
	CEdit	m_NameControl;
	CEdit	m_StationControl;
	CEdit	m_DescriptionControl;
	CEdit	m_PersonelControl;
	CComboBox	m_ComboDateType;
	CString	m_szSurveyName;
	CString	m_szSurveyDescription;
	CString	m_szSurveyPersonnel;
	CString	m_szSurveyStations;
	BOOL	m_CheckedORSearch;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SurveySearchDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	COnStationDoc * m_pDocument;
	// Generated message map functions
	//{{AFX_MSG(SurveySearchDialog)
	afx_msg void OnDisplayresults();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCombo1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
