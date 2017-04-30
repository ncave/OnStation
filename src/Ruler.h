// ruler.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRuler window
#ifndef I_RULER__
#define I_RULER__

#define RULER_WIDTH	25

class COnStationView;
class CRuler : public CWnd
{
public:
	CRuler();

	COnStationView * m_View;

	//given a scale this function returns
	//fUnits which is the number of meters or feet change, and
	//iRulerSpacing which is the delta in pixels in LOMETRIC mode
	static void GetReasonableIncrements(float TempScale,float *fUnits);
// Attributes
public:
	float m_Scale;
// Implementation
public:
	virtual ~CRuler();
	void UpdateScale(float fScale);
	inline void SetView(COnStationView *pView) {m_View=pView;}

	BOOL Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID);
protected:
	// Generated message map functions
	//{{AFX_MSG(CRuler)
	afx_msg void OnPaint();
	afx_msg LRESULT OnSizeParent(WPARAM wParam,LPARAM lParam);
	afx_msg BOOL OnToolTipText(UINT,NMHDR * pNMHDR,LRESULT *pResult);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnShrink50();
	afx_msg void OnExpand50();
	afx_msg void OnShrinkMaximum();
	afx_msg void OnZoomMaximum();
	afx_msg void OnZoom5();
	afx_msg void OnZoom10();
	afx_msg void OnZoom25();
	afx_msg void OnZoom50();
	afx_msg void OnZoom100();
	afx_msg void OnZoom500();
	afx_msg void OnZoom1000();
	afx_msg void OnZoom10I();
	afx_msg void OnZoom20I();
	afx_msg void OnZoom50I();
	afx_msg void OnZoom100I();
	afx_msg void OnZoom500I();
	afx_msg void OnZoom1000I();
	afx_msg void OnZoom5000I();
	//}}AFX_MSG			
	DECLARE_MESSAGE_MAP()
	virtual INT_PTR OnToolHitTest(CPoint point,TOOLINFO *pTI) const;
};

/////////////////////////////////////////////////////////////////////////////


#endif
