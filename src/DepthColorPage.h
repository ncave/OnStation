#ifndef I_DEPTHCOLORS__
#define I_DEPTHCOLORS__
#include "colorwidget.h"
// depthcolors.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDepthColorPage dialog

class CColorSheet;
class CDepthColorPage : public CPropertyPage
{
private:
	CColorWidget m_ColorWidget;	
	BOOL m_bDirty;

	//Keep track of the maximum height of the color widget
	//Since we resize it all the time we want to stop if from
	//being weirdly resized to be bigger then it started at.
	//The height of the original control is stored here as a starting
	//point for each resize.
	int m_iMaxY;

	int m_iColorFocus;

	void ResizeColorWidget();

	CColorSheet * m_pColorSheet;
// Construction
public:
	CDepthColorPage(CColorSheet * pColorSheet);   // standard constructor

	BOOL IsDirty() {return m_bDirty;}
// Dialog Data
	//{{AFX_DATA(CDepthColorPage)
	enum { IDD = IDD_DEPTHCOLORS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDepthColorPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDepthColorPage)
	afx_msg void OnPaint();
	afx_msg LONG_PTR OnColorChange(UINT_PTR wParam, LONG_PTR lParam);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnWizard();
	virtual BOOL OnInitDialog();
	afx_msg void OnEvensplit();
	afx_msg void OnButtondefaults();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif
