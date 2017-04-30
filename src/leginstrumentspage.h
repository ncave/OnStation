#ifndef I_INSTRUMENTS__
#define I_INSTRUMENTS__
// instruments.h : header file
//
class CSurveyLeg;
/////////////////////////////////////////////////////////////////////////////
// CLegInstrumentsPage dialog

class CLegInstrumentsPage : public CPropertyPage
{
private:
	CSurveyLeg *m_Leg;
// Construction
public:
	CLegInstrumentsPage(CSurveyLeg *Leg);
	~CLegInstrumentsPage();

	BOOL m_bDirty;
	BOOL OnKillActive();
// Dialog Data
	//{{AFX_DATA(CLegInstrumentsPage)
	enum { IDD = IDD_INSTRUMENTPAGE };
	float	m_fClino1Fix;
	CString	m_szClino1Name;
	float	m_fClino2Fix;
	CString	m_szClino2Name;
	float	m_fStandardCompassError;
	float	m_fCompass1Fix;
	CString	m_szCompass1Name;
	float	m_fCompass2Fix;
	CString	m_szCompass2Name;
	float	m_fDeclination;
	float	m_fStandardTapeError;
	float	m_fTapeFix;
	CString	m_fTapeName;
	float	m_fStandardClinoError;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CLegInstrumentsPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CLegInstrumentsPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnBigChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


#endif


