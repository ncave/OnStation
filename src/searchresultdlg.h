// srchres.h : header file
//
#include "colorwidget.h"

/////////////////////////////////////////////////////////////////////////////
// CSearchResultDialog dialog
class CNode;
class COnStationView;
class CSearchResultDialog : public CDialog
{
private:
	CPtrArray * m_pPointArray;
	COnStationView * m_MyView;

	void UpdateNodePosText();
	CNode * GetSelectedNode();
// Construction
public:
	CSearchResultDialog(COnStationView *View,CPtrArray * pPointArray,CWnd* pParent = NULL);	// standard constructor

	BOOL m_bDirty;
// Dialog Data
	//{{AFX_DATA(CSearchResultDialog)
	enum { IDD = IDD_SEARCHRESULT };
	CListBox	m_StationList;
	CStatic	m_szZ;
	CStatic	m_szY;
	CStatic	m_szX;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CSearchResultDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtoncenter();
	virtual void OnOK();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnSurface();
	afx_msg void OnFixthispoint();
	afx_msg void OnShowsurveys();
	afx_msg void OnDblclkStationlist();
	afx_msg void OnSelchangeStationlist();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
