#ifndef I_SUMMARYPAGE__
#define I_SUMMARYPAGE__
// summarypage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLegSummaryPage dialog
class CSurveyLeg;
class CLegSummaryPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CLegSummaryPage)
private:
	CSurveyLeg *m_Leg;	
// Construction
public:
	CLegSummaryPage();
	~CLegSummaryPage();

	BOOL OnKillActive();
	BOOL m_bDirty;

	void SetLeg(CSurveyLeg *Leg);
// Dialog Data
	//{{AFX_DATA(CLegSummaryPage)
	enum { IDD = IDD_SUMMARYPAGE };
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CLegSummaryPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CLegSummaryPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnBigChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
#endif

