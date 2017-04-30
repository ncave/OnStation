// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////
#include "overalldataview.h"
#include "ruler.h"
#include "viewtab.h"
#include "compasswidget.h"

class COnStationView;
class CMainFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CMainFrame)
private:
	BOOL m_bCreated;
	BOOL m_bFullScreen;
	WINDOWPLACEMENT m_wplNormal;
public:
	CMainFrame();

	BOOL IsFullScreen() {return m_bFullScreen;}
	// Attributes
public:
	void SetActiveViewTab(VIEWTYPE vt);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	void SetPaneText(int iPane,LPCTSTR szText);
public:		//Let the child frames play with these guys
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CToolBar	m_wndToolBarFullScreen;
	CRuler		m_wndRuler;
	CViewTab	m_viewTab;
	COverallDataView m_dataView;
	void ViewFullScreen(COnStationView * pView);
	//used when going into print preview mode
	void SetPrintPreview(BOOL bEnter);
// Generated message map functions
protected:
	afx_msg void OnInitMenu(CMenu* pMenu);
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnHelp();
	afx_msg void OnEditDelete();
	afx_msg void OnEditAddfolder();
	afx_msg void OnEditAddsurvey();
	afx_msg void OnEditAddfixedpoint();
	afx_msg void OnEditPaste();
	afx_msg void OnEditProperties();
	afx_msg void OnEditFindssurvey();
	afx_msg void OnEditEndquery();
	afx_msg void OnEditEdit();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnViewPreferences();
	afx_msg BOOL OnQueryNewPalette();
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	afx_msg void OnUpdateEditEdit(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditDelete(CCmdUI* pCmdUI);
	afx_msg void OnDebugStationlinks();
	afx_msg void OnDrawingFont();
	afx_msg void OnErrorsloopsLoopclosureoptions();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
