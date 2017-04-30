// SearchCommentsDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSearchCommentsDialog dialog
#include "colorwidget.h"
class CRealFolder;
class COnStationView;
class CSearchCommentsDialog : public CDialog
{
// Construction
public:
	CSearchCommentsDialog(COnStationView *view,CRealFolder *folder,CWnd* pParent = NULL);   // standard constructor
	CRealFolder * m_Folder;
	COnStationView * m_pView;
// Dialog Data
	//{{AFX_DATA(CSearchCommentsDialog)
	enum { IDD = IDD_SEARCHCOMMENTSDIALOG };
	CString	m_szCommentText;
	CString	m_szTypeString;
	BOOL	m_bColorToShot;
	BOOL	m_bColorFromShot;
	BOOL	m_bPlaceTag;
	//}}AFX_DATA
	int m_iColorIndex;
	CColorWidget m_ColorWidget;	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSearchCommentsDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSearchCommentsDialog)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDisplayresults();
	afx_msg LONG_PTR OnColorChange(UINT_PTR, LONG_PTR lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
