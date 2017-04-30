#ifndef I_WAITFORCLOSUREDLG__
#define I_WAITFORCLOSUREDLG__

// WaitForClosureDialog.h : header file
//
//CChange to take CSurveyShot * SS;
class CSurveyShot;
class CClose;
/////////////////////////////////////////////////////////////////////////////
// CWaitForClosureDialog dialog
class CWaitForClosureDialog : public CDialog
{
private:
	CClose * m_MyClose;
	int m_iPhase;
	int m_iShotCount;
	CSurveyShot ** m_ShotArray;
	// Construction
public:
	CWaitForClosureDialog(int iColorScheme,CSurveyShot ** ShotPtrArra,int iShots,CWnd* pParent = NULL);   // standard constructor
	~CWaitForClosureDialog();

// Dialog Data
	//{{AFX_DATA(CWaitForClosureDialog)
	enum { IDD = IDD_LOOPCLOSUREWAIT };
	CProgressCtrl	m_ProgressControl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWaitForClosureDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWaitForClosureDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif


