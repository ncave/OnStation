#if !defined(AFX_DEBUGSTATIONLINKSDIALOG_H__F84ADD21_351B_11D1_97E7_444553540000__INCLUDED_)
#define AFX_DEBUGSTATIONLINKSDIALOG_H__F84ADD21_351B_11D1_97E7_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DebugStationLinksDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDebugStationLinksDialog dialog

class CDebugStationLinksDialog : public CDialog
{
// Construction
public:
	CDebugStationLinksDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDebugStationLinksDialog)
	enum { IDD = IDD_DEBUGSTATIONLINKS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDebugStationLinksDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDebugStationLinksDialog)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEBUGSTATIONLINKSDIALOG_H__F84ADD21_351B_11D1_97E7_444553540000__INCLUDED_)
