#ifndef I_FOLDERINFO__
#define I_FOLDERINFO__
// folderinfo.h : header file
//
#include "folder.h"	//for FOLDERDATA definition
/////////////////////////////////////////////////////////////////////////////
// CFolderInfoDlg dialog

class CFolderInfoDlg : public CDialog
{
private:
	CFolderData *m_FD;
	CFont m_MyFontTitle;
// Construction
public:
	CFolderInfoDlg(CFolderData *,LPCTSTR,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFolderInfoDlg)
	enum { IDD = IDD_FOLDERINFO };
	CString	m_szFolderName;
	CString	m_szNumShots;
	CString	m_szNumSurfaceShots;
	CString	m_szTotalSurveys;
	CString m_szTotalAllShots;
	CString	m_szTotalDepth;
	CString	m_szTotalLength;
	CString	m_szTotalSurfaceLength;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFolderInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFolderInfoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


#endif

