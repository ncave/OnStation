#if !defined(AFX_BLUNDERDIALOG_H__5D0E9CE0_79A1_11D2_97E7_0000B4560C83__INCLUDED_)
#define AFX_BLUNDERDIALOG_H__5D0E9CE0_79A1_11D2_97E7_0000B4560C83__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// BlunderDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBlunderDialog dialog

class CBlunderDialog : public CDialog
{
// Construction
public:
	CBlunderDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBlunderDialog)
	enum { IDD = IDD_BLUNDERS };
	CButton	m_Display_button;
	CComboBox	m_cbDeviations;
	CListBox	m_shotList;
	CButton	m_RadioDev;
	CButton	m_RadioMax;
	CButton	m_RadioCom;
	CButton	m_RadioCli;
	CButton	m_RadioTap;
	//}}AFX_DATA

	void	LoadListBox();
	BOOL m_bDirty;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBlunderDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBlunderDialog)
	afx_msg void OnSelchangeDeviations();
	virtual BOOL OnInitDialog();
	afx_msg void OnEditSurvey();
	afx_msg void OnDblclkList1();
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnBluDev();
	afx_msg void OnBluMax();
	afx_msg void OnBluCom();
	afx_msg void OnBluCli();
	afx_msg void OnBluTap();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BLUNDERDIALOG_H__5D0E9CE0_79A1_11D2_97E7_0000B4560C83__INCLUDED_)
