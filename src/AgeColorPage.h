#ifndef I_AGECOLORS__
#define I_AGECOLORS__
// agecolors.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAgeColorPage dialog
#include "colorwidget.h"
class CColorSheet;
class CAgeColorPage : public CPropertyPage
{
private:
	BOOL m_bDirty;
	// Construction
	CColorSheet * m_pColorSheet;
public:
	CAgeColorPage(CColorSheet * pColorSheet);   // standard constructor
	BOOL IsDirty();
// Dialog Data
	//{{AFX_DATA(CAgeColorPage)
	enum { IDD = IDD_AGECOLORS };
	CListBox	m_AgeList;
	//}}AFX_DATA

	CColorWidget m_ColorWidget;

	void InitializeAll();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAgeColorPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAgeColorPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtondefaults();
	afx_msg void OnDrawItem(int, LPDRAWITEMSTRUCT);
	afx_msg LONG_PTR OnColorChange(UINT_PTR wParam, LONG_PTR lParam);
	virtual void OnOK();
	afx_msg void OnSelchangeAgelist();
	afx_msg void OnWizard();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif


