//{{AFX_INCLUDES()
//#include "editablegrid.h"
#include "EditableGrid.h"
//}}AFX_INCLUDES
#ifndef I_SURFACEDITPAGE__
#define I_SURFACEDITPAGE__
// surfaceeditpage.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CSurfaceEditPage dialog

class CSurfaceDataPump;
class CGridControl;
class CSurfaceData;
class CSurfaceEditPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CSurfaceEditPage)
private:
    CSurfaceData * m_SurfaceData;
	
// Construction
public:
	CSurfaceEditPage();
	~CSurfaceEditPage();

	BOOL m_bDirty;
	void Initialize(CSurfaceData *);
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
// Dialog Data
	//{{AFX_DATA(CSurfaceEditPage)
	enum { IDD = IDD_SURFACEALTITUDES };
	CEditableGrid m_myGridControl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSurfaceEditPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSurfaceEditPage)
	virtual BOOL OnInitDialog();
//	afx_msg void OnChangedEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
#endif

