#ifndef I_WIDGET__
#define I_WIDGET__
#define WELL_SIZE		20

//Sent when a new color is selected
#define WM_COLORCHANGE		WM_USER+5


/////////////////////////////////////////////////////////////////////////////
// CColorWidget window

//generic helper function
void DrawClientBox(CDC& dc,CRect& rect,BOOL bSunken);

class CColorWidget : public CWnd
{
private:
	BOOL m_bWidgetRightBoxes;
	BOOL m_bEnableDisableOnly;
	int m_iWidgetRows;
	int m_iWidgetCols;
// Construction
public:
	CColorWidget(BOOL bAllowNoUse,int m_iWidgetRows,int m_iWidgetCols);
// Attributes
public:
    int m_iSelectedCell;
	void EnableDisableOnly(BOOL bEnableDisableOnly);
// Operations
public:
	BOOL Create(CWnd *parent,RECT & rect,int ID);
// Implementation
public:
	virtual ~CColorWidget();

protected:
	BOOL m_bAllowNoUse;
	// Generated message map functions
	//{{AFX_MSG(CColorWidget)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif

