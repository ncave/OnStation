#ifndef I_COMPASSWIDGET__
#define I_COMPASSWIDGET__
// widget2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCompassWidget window
class COnStationView;
class CCompassWidget : public CWnd
{
private:
	COnStationView * m_pView;
	double m_fClino;
	double m_fCompass;
	
	double GetMousePosAngle(POINT,BOOL bCompass);
	
	BOOL ClickedOnCompass(POINT pt);
	BOOL ClickedOnTilter(POINT pt);
	void DrawClinoLine(CDC& dc,double dAngle,BOOL bFill);
	void DrawCompassLine(CDC& dc,double dAngle);	
	void TrackDrag(POINT pt,BOOL bCompass);
	HCURSOR GetAngleCursor(CPoint const& pt);
// Construction
public:
	CCompassWidget();

	inline void SetView(COnStationView * View) {m_pView=View;}
    inline BOOL IsAngleBetween(double dLow,double dHigh,double dAngle)
    	{ return  (dLow<dHigh) ?(dAngle>=dLow && dAngle<=dHigh):(dAngle>=dLow || dAngle<=dHigh); }

	void SetClinoAndCompass(float fClino,float fCompass);   
// Attributes
public:

// Operations
public:

// Implementation
public:
	virtual ~CCompassWidget();
	BOOL Create(CWnd *parent,int ID);

protected:
	// Generated message map functions
	//{{AFX_MSG(CCompassWidget)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnToolTipText(UINT,NMHDR * pNMHDR,LRESULT *pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNorth();
	afx_msg void OnEast();
	afx_msg void OnSouth();
	afx_msg void OnNorthEast();
	afx_msg void OnSouthEast();
	afx_msg void OnWest();
	afx_msg void OnNorthWest();
	afx_msg void OnSouthWest();
	afx_msg void OnTilt0();
	afx_msg void OnTilt45();
	afx_msg void OnTilt90();
	afx_msg void OnTilt135();
	afx_msg void OnTilt180();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual INT_PTR OnToolHitTest(CPoint point,TOOLINFO *pTI) const;
};

/////////////////////////////////////////////////////////////////////////////
#endif



