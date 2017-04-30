#ifndef I_VIEWTAB__
#define I_VIEWTAB__
// viewtab.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewTab window

class CViewTab : public CWnd
{
private:
	CStringArray m_szStrings;
	CWnd * m_CommandTarget;
	CDWordArray  m_dwWidths;
	CDWordArray  m_iCommands;
	CFont * GetFont(int iWeight);
	void DrawTab(CDC& dc,int iLeft,int iWidth,CRect& r,HBRUSH hMiddle,LPCTSTR szText);
	INT_PTR m_iSelectedTab;
// Construction
public:
	CViewTab();
	BOOL Create(CWnd *cmdTarget,CWnd* pParentWnd, DWORD dwStyle, UINT nID);
	
// Attributes
public:
	void AddString(int iStringID,WPARAM Command,BOOL bActive);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewTab)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CViewTab();
	void SetActiveTab(int iTab);
	// Generated message map functions
protected:
	//{{AFX_MSG(CViewTab)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LONG_PTR OnHelpHitTest(UINT_PTR, LONG_PTR);
	afx_msg LRESULT OnSizeParent(WPARAM wParam,LPARAM lParam);
	afx_msg BOOL OnToolTipText(UINT,NMHDR * pNMHDR,LRESULT *pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual INT_PTR OnToolHitTest(CPoint point,TOOLINFO *pTI) const;
};

/////////////////////////////////////////////////////////////////////////////
#endif

