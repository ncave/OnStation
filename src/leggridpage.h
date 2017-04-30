#ifndef I_GRIDPAGE__
#define I_GRIDPAGE__
// leggridpage.h : header file
//
#include "EditableGrid.h"

//Dimensions of the spreadsheet
const int CELL_TEXT_BORDER=GetSystemMetrics(SM_CYDLGFRAME);
const int UPPERLEFT_X=GetSystemMetrics(SM_CYCAPTION);
const int UPPERLEFT_Y=GetSystemMetrics(SM_CYCAPTION)*3;

const int CELL_THICKNESS=GetSystemMetrics(SM_CYCAPTION)+CELL_TEXT_BORDER*2;
const int EDIT_THICKNESS=GetSystemMetrics(SM_CYCAPTION)+GetSystemMetrics(SM_CYDLGFRAME);
const int BORDER_WIDTH=30;
const int BORDER_THICKNESS=GetSystemMetrics(SM_CYCAPTION)+CELL_TEXT_BORDER*2;
const int MINIMUM_CELL_WIDTH=GetSystemMetrics(SM_CYCAPTION);



const int GRIDCONTROLID=106;

class CSurveyLeg;
class COnStationDoc;
class CSurveyShot;
/////////////////////////////////////////////////////////////////////////////
// CLegGridPage dialog
class CLegGridPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CLegGridPage)

protected:
	BOOL OnKillActive();
	BOOL OnSetActive();

	//Number of columns of data excluding the surface/length/comment columns
	int m_iNumStandardCols;
	void GetFlags(CSurveyShot *);
	void SetFlags(CSurveyShot *);
	int SetComments(int iRow,CSurveyShot *pShot);
	void GetComments(CSurveyShot *pShot);
	void LegGridMessageBox(int iMessageId,int iRow,int iCol);
	BOOL GetStandardRow(int iLine,CSurveyShot **ppShot,CSurveyShot * pLastShot);
	BOOL GetDiveRow(int iLine,CSurveyShot **ppShot,CSurveyShot* pLastShot);

	CSurveyLeg *m_Leg;

// Attributes
public:
	BOOL NeedRecalculate() const;
// Operations
public:
    void OnInsertRow(int);
    void GotoLine(int);

	void SetLeg(CSurveyLeg *Leg);

// Construction
public:
	CLegGridPage();
	~CLegGridPage();

// Dialog Data
	//{{AFX_DATA(CLegGridPage)
	enum { IDD = IDD_GRIDPAGE };
	CEditableGrid m_myGridControl;
	//}}AFX_DATA

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CLegGridPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CLegGridPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnInsertLine();
	afx_msg void OnAutoNumber();
	afx_msg void OnAutoSize();
	afx_msg void OnBnClickedUndo();
	afx_msg void OnBnClickedCut();
	afx_msg void OnBnClickedCopy();
	afx_msg void OnBnClickedPaste();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif

