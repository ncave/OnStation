#ifndef I_SAVEFORMATTEDTEXT__
#define I_SAVEFORMATTEDTEXT__
// printsurveydata.h : header file
//
#include "surveyleg.h"

/////////////////////////////////////////////////////////////////////////////
// CSaveFormattedText dialog

class CFileDumpHelper;
class CFolder;
class COnStationDoc;
class CSaveFormattedText : public CDialog
{
private:
	void PrintGuts(CFolder *folder,CFileDumpHelper * FDH,BOOL bCoordinates);
	COnStationDoc * m_MyDocument;
// Construction
public:
	CSaveFormattedText(COnStationDoc *,CWnd* pParent = NULL);   // standard constructor
	~CSaveFormattedText();
// Dialog Data
	//{{AFX_DATA(CSaveFormattedText)
	enum { IDD = IDD_SAVEFORMATTEDTEXTFILE };
	CTreeCtrl	m_SurveyList;
	//}}AFX_DATA
	int m_idxSurvey;
	int m_idxFolder;
	CImageList m_ImageList;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSaveFormattedText)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSaveFormattedText)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnGetdispinfoSurveylist(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif



