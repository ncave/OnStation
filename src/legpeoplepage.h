#ifndef I_PEOPLEPAGE__
#define I_PEOPLEPAGE__
// peoplepage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLegPeoplePage dialog
class CSurveyLeg;
class CLegPeoplePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CLegPeoplePage)

// Construction
public:
	CLegPeoplePage();
	~CLegPeoplePage();

	void RealDataInit(CSurveyLeg *);
// Dialog Data
	//{{AFX_DATA(CLegPeoplePage)
	enum { IDD = IDD_PEOPLEPAGE };
	//}}AFX_DATA
	CSurveyLeg * m_Leg;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CLegPeoplePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	BOOL OnKillActive();
// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CLegPeoplePage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


#endif


