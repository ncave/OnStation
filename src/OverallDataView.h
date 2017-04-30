#ifndef I_OVERALLDATAVIEW__
#define I_OVERALLDATAVIEW__
// OverallDataView.h : header file
//
#include "colorschemecombobox.h"
#include "onstationsurveytree.h"

/////////////////////////////////////////////////////////////////////////////
// COverallDataView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include "colorwidget.h"
#define IDC_NEWCOLORSHOT   321   //just picked a number and hope it works.
class CLegQuery;
class COnStationView;
class COverallDataView : public CDialogBar//Wnd  //CMDIChildWnd
{
private:
	//Client area controls

public:
	COverallDataView();           // protected constructor used by dynamic creation
// Dialog Data
	//{{AFX_DATA(COverallDataView)
	enum { IDD = IDD_OVERALLDATAVIEW };
	//}}AFX_DATA
	CColorWidget m_colorWidget;
	CColorSchemeComboBox	m_colorSchemeComboBox;
	COnStationSurveyTree	m_masterSurveyList;
	void InitialUpdate();
	void UpdateColorControl(int iColorScheme);
// Form Data
public:
// Attributes
public:

// Operations
public:
	void OnChangedDocumentData();
	void RemoveAll();
	virtual ~COverallDataView();

	void EnableColorWidget(BOOL bEnable);
	void SetColorSchemeIndex(int iIndex);
	void OnCut();
	void OnCopy();
	void OnPaste();
	void OnDelete();
	void OnEdit();
	void OnUpdateEditEdit(CCmdUI *);
	void OnUpdateEditDelete(CCmdUI *);
	void OnUpdateEditCopy(CCmdUI *);
	void OnAddSurvey();
	void OnAddFolder();
	void OnAddFixedPoint();
	void OnShowProperties();
	void SetQuery(CLegQuery *);
// Overrides
	// ClassWizard generated virtual function overrides
protected:
#ifdef _DEBUG
//	virtual void AssertValid() const;
//	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(COverallDataView)
	afx_msg void OnDrawmodeColorscheme1();
	afx_msg void OnDrawmodeColorscheme2();
	afx_msg void OnDrawmodeColorscheme3();
	afx_msg void OnDrawmodeColorscheme4();
	afx_msg void OnRMenuEdit();
	afx_msg void OnRMenuDelete();
	afx_msg void OnRMenuAddFolder();
	afx_msg void OnRMenuAddSurvey();
	afx_msg void OnRMenuAddFixedPoint();
	afx_msg void OnDrawmodeColorscheme5();
	afx_msg void OnDrawmodeColorscheme6();
	afx_msg void OnDrawmodeColorscheme7();
	afx_msg void OnDrawmodeColorscheme8();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LONG_PTR OnColorChange(UINT_PTR, LONG_PTR lParam);
	afx_msg void OnEditProperties();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDrawmodeDepthcoding();
	afx_msg void OnDrawmodeAgecoloring();
	afx_msg void OnDrawmodeBlunderColoring();
	afx_msg void OnConfigurecolors();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSurveytreeRainbowcolorsurveys();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif

