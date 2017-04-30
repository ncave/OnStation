#ifndef I_COLORSCHEMEBOX__
#define I_COLORSCHEMEBOX__
// ColorSchemeComboBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorSchemeComboBox window
class COnStationDoc;
class CColorSchemeComboBox : public CComboBox
{
// Construction
protected:
	int m_iCurSel;
	BOOL m_bEdited;
public:
	CColorSchemeComboBox();

	int GetColorScheme() {SetCurSel(m_iCurSel);return m_iCurSel;}
	void InitialUpdate();
	void UpdateEntries();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorSchemeComboBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColorSchemeComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorSchemeComboBox)
	afx_msg void OnSelchange();
	afx_msg void OnKillfocus();
	afx_msg void OnEditchange();
	afx_msg void OnDropdown();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif

