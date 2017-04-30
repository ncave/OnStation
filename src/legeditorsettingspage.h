#ifndef I_EDITORSETTINGS__
#define I_EDITORSETTINGS__
// EditorSettings.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLegEditorSettingsPage dialog
class CSurveyLeg;
class CLegEditorSettingsPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CLegEditorSettingsPage)
private:
	CSurveyLeg * m_SurveyLeg;
	void SetupFromLegFields();
// Construction
public:
	CLegEditorSettingsPage();
	~CLegEditorSettingsPage();

	void Initialize(CSurveyLeg * Leg);
// Dialog Data
	//{{AFX_DATA(CLegEditorSettingsPage)
	enum { IDD = IDD_EDITSETTINGSPAGE };
	CListBox	m_ColumnList;
	//}}AFX_DATA
	BOOL OnSetActive();


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CLegEditorSettingsPage)
	public:
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CLegEditorSettingsPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnMovedown();
	afx_msg void OnMoveup();
	afx_msg void OnDefaults();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
#endif




