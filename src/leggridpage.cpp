// gridpage.cpp : implementation file
//
// This is the control which holds the editing "spreadsheet".  Due to the way
// messages are handled, most of the commands come throught here and are
// then passed into the grid control.
#include "stdafx.h"
#include "onstation.h"
#include "leggridpage.h"
#include "surveyleg.h"
#include "GridCtrl\GridCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLegGridPage property page

IMPLEMENT_DYNCREATE(CLegGridPage, CPropertyPage)

#define new DEBUG_NEW

static LPCTSTR szFlagTypes[] = { _T("S"), _T("W"), _T("L") };

CLegGridPage::CLegGridPage() : CPropertyPage(CLegGridPage::IDD)
{
	//{{AFX_DATA_INIT(CLegGridPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CLegGridPage::~CLegGridPage()
{
}

void CLegGridPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLegGridPage)
	DDX_GridControl(pDX, IDC_GRID1, m_myGridControl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLegGridPage, CPropertyPage)
	//{{AFX_MSG_MAP(CLegGridPage)
	ON_BN_CLICKED(IDC_AUTONUMBER, OnAutoNumber)
	ON_BN_CLICKED(IDC_INSERTLINE, OnInsertLine)
	ON_BN_CLICKED(IDC_AUTOSIZE, OnAutoSize)
	ON_BN_CLICKED(IDC_UNDO, OnBnClickedUndo)
	ON_BN_CLICKED(IDC_CUT, OnBnClickedCut)
	ON_BN_CLICKED(IDC_COPY, OnBnClickedCopy)
	ON_BN_CLICKED(IDC_PASTE, OnBnClickedPaste)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CLegGridPage message handlers

BOOL CLegGridPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLegGridPage::SetLeg(CSurveyLeg *Leg)
{
	m_Leg=Leg;
}


BOOL CLegGridPage::OnKillActive()
{	
	//Save column widths
	for (int i=0;i<m_iNumStandardCols+4;i++)
	{
		m_Leg->m_iColWidth[i]=(short)m_myGridControl.GetColumnWidth(i);
	}

	SetCursor(::LoadCursor(NULL,IDC_WAIT));
	m_Leg->DeleteShots();
	CSurveyShot * pLastShot=NULL;
	for (int i=0;i<MAX_GRID_ENTRIES;i++)
	{
		CSurveyShot *pShot;
		if (m_Leg->GetCaveDive())
		{
			if (!GetDiveRow(i,&pShot,pLastShot))
			{
				SetCursor(::LoadCursor(NULL,IDC_ARROW));
				return FALSE;
			}
		}
		else
		{
			if (!GetStandardRow(i,&pShot,pLastShot))
			{
				SetCursor(::LoadCursor(NULL,IDC_ARROW));
				return FALSE;
			}
		}
		if (pShot!=NULL)
		{
			m_Leg->GetShotArray()->Add(pShot);
			pLastShot=pShot;
		}
	}
	m_Leg->GetShotArray()->DoDeferredOffsetCalculation();
	SetCursor(::LoadCursor(NULL,IDC_ARROW));
	return CPropertyPage::OnKillActive();
}

BOOL CLegGridPage::OnSetActive()
{
	m_iNumStandardCols=9;
	if (m_Leg->HasBacksights())
	{
		m_iNumStandardCols+=2;
	}
	if (m_myGridControl.m_hWnd==NULL)
	{
		AfxMessageBox(IDS_NOGRIDCONTROL);
		return FALSE;
	}
	m_myGridControl.SetRowCount(MAX_GRID_ENTRIES+1);
	m_myGridControl.SetColumnCount(m_iNumStandardCols+4);
	m_myGridControl.SetFixedRowCount(1);
	m_myGridControl.SetFixedColumnCount(0);

	int i,j;
	BOOL bSetDefaultColWidths=(m_Leg->m_iColWidth[0]==-1);
	CSurveyShotArray *List=m_Leg->GetShotArray();
	int iRowIndex=1;
	float f, f2;
	for (i=0;i<List->GetSize();i++)
	{
		int iCol=2;
		if (m_Leg->GetCaveDive())
		{
	 		CCaveDiveSurveyShot *shot=dynamic_cast<CCaveDiveSurveyShot *>(List->GetAt(i));
			ASSERT(shot!=NULL);
			if (shot==NULL)
				return FALSE;	//major problem
			m_myGridControl.SetItemText(iRowIndex, 0, shot->GetFromStationName());
			m_myGridControl.SetItemText(iRowIndex, 1, shot->GetToStationName());
			for (j=0;j<7;j++)
			{
				switch (m_Leg->m_szFieldOrder[j])
				{
					case _T('T'):
						f = shot->GetLength();
						break;
					case _T('A'):
						f=shot->GetCompass();
						if (f!=NAN && m_Leg->GetEuroDegrees())
						{
							f=f*400.0f/360.0f;
						}
						break;
					case _T('I'):
						f=shot->GetDepthAtFromStation();
						break;
					case _T('U'):
						f = shot->GetRawUp();
						break;
					case _T('D'):
						f = shot->GetRawDown();
						break;
					case _T('L'):
						f = shot->GetRawLeft();
						break;
					case _T('R'):
						f = shot->GetRawRight();
						break;
					default:
						ASSERT(FALSE); //some sort of problem
				}
				CString CS;
				FloatToString(CS,f);
				m_myGridControl.SetItemText(iRowIndex, iCol++, CS);
			}
			SetFlags(shot);
			iRowIndex=SetComments(iRowIndex,shot);
		}
		else
		{
	 		CStandardSurveyShot *shot=dynamic_cast<CStandardSurveyShot *>(List->GetAt(i));

			m_myGridControl.SetItemText(iRowIndex, 0, shot->GetFromStationName());
			m_myGridControl.SetItemText(iRowIndex, 1, shot->GetToStationName());
			for (j=0;j<m_iNumStandardCols;j++)
			{
				switch (m_Leg->m_szFieldOrder[j])
				{
					case _T('T'):
						f = shot->GetLength();
						break;
					case _T('A'):
						f = shot->GetCompass();
						if (f != NAN && m_Leg->GetEuroDegrees())
						{
							f = f*400.0f/360.0f;
						}
						if (m_Leg->HasBacksights())
						{
							f2 = shot->GetCompass2();
							if (f2 != NAN && m_Leg->GetEuroDegrees())
							{
								f2 = f2*400.0f/360.0f;
							}
						}
						break;
					case _T('I'):
						f = shot->GetClino();
						if (f!=NAN && m_Leg->GetEuroDegrees())
						{
							f = f*100.0f/90.0f;
						}
						if (m_Leg->HasBacksights())
						{
							f2 = shot->GetClino2();
							if (f2!=NAN && m_Leg->GetEuroDegrees())
							{
								f2=f2*100.0f/90.0f;
							}
						}
						break;
					case _T('U'):
						f = shot->GetRawUp();
						break;
					case _T('D'):
						f = shot->GetRawDown();
						break;
					case _T('L'):
						f = shot->GetRawLeft();
						break;
					case _T('R'):
						f = shot->GetRawRight();
						break;
				}
				CString CS;
				FloatToString(CS,f);
				m_myGridControl.SetItemText(iRowIndex, iCol++, CS);
				if (m_Leg->HasBacksights())
				{
					CString CS2;
					FloatToString(CS2,f2);
					m_myGridControl.SetItemText(iRowIndex, iCol++, CS2);
				}
			}
			SetFlags(shot);
			iRowIndex=SetComments(iRowIndex,shot);
		}
	}

	CString CS, CS2;
	CS.LoadString(IDS_FROMSTATION);
	m_myGridControl.SetItemText(0, 0, CS);
	CS.LoadString(IDS_TOSTATION);
	m_myGridControl.SetItemText(0, 1, CS);
	int iIndex=2;
	for (j=0;j<7;j++)
	{
		switch (m_Leg->m_szFieldOrder[j])
		{
		case _T('T'):
			CS.LoadString(IDS_LENGTH);
			break;
		case _T('I'):
			if (m_Leg->GetCaveDive())
			{
				CS.LoadString(IDS_DEPTH);
			}
			else
			{
				CS.LoadString(IDS_CLINOMETER);
				if (m_Leg->HasBacksights())
				{
					CS2.LoadString(IDS_CLINOMETERBACK);
				}
			}
			break;
		case _T('A'):
			CS.LoadString(IDS_COMPASS);
			if (m_Leg->HasBacksights())
			{
				CS2.LoadString(IDS_COMPASSBACK);
			}
			break;
		case _T('U'):
			CS.LoadString(IDS_UP);
			break;
		case _T('D'):
			CS.LoadString(IDS_DOWN);
			break;
		case _T('L'):
			CS.LoadString(IDS_LEFT);
			break;
		case _T('R'):
			CS.LoadString(IDS_RIGHT);
			break;
		default:
			ASSERT(!_T("BAD STRING VALUE"));
			break;
		}
		m_myGridControl.SetItemText(0, iIndex++, CS);
		if (bSetDefaultColWidths)
		{
			m_myGridControl.SetColumnWidth(iIndex-1,PixelsToTwips(REGULAR_COL_WIDTH));
		}
		if (m_Leg->HasBacksights())
		{
			m_myGridControl.SetItemText(0, iIndex++, CS2);
			if (bSetDefaultColWidths)
			{
				m_myGridControl.SetColumnWidth(iIndex-1, PixelsToTwips(REGULAR_COL_WIDTH));
			}
		}
	}
	
	for (j=iIndex;j<iIndex+3;j++)
	{
		m_myGridControl.SetItemText(0, j, szFlagTypes[j-iIndex]);	//surface;
		if (bSetDefaultColWidths)
		{
			m_myGridControl.SetColumnWidth(j,PixelsToTwips(CHECK_COL_WIDTH));
		}
	}	
	CString csComment;
	csComment.LoadString(IDS_COMMENTS);
	m_myGridControl.SetItemText(0, j, csComment);

	// set column widths
	if (bSetDefaultColWidths)
	{
		m_myGridControl.SetColumnWidth(j,PixelsToTwips(COMMENTS_COL_WIDTH));
		m_myGridControl.SetColumnWidth(0,PixelsToTwips(NAME_COLUMN_WIDTH));
		m_myGridControl.SetColumnWidth(1,PixelsToTwips(NAME_COLUMN_WIDTH));
		m_myGridControl.AutoSizeColumns();
	}
	else
	{
		for (i=0;i<m_iNumStandardCols+4;i++)
		{
			m_myGridControl.SetColumnWidth(i, m_Leg->m_iColWidth[i]);
		}
	}

	// set row heights
	m_myGridControl.AutoSizeRow(0); // autosize column headers

	// set cell format
	UINT nFormatHeader = (DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX|DT_END_ELLIPSIS);
	UINT nFormatString = (DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX|DT_END_ELLIPSIS);
	UINT nFormatNumber = (DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX|DT_END_ELLIPSIS);
	UINT nFormatCenter = (DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX|DT_END_ELLIPSIS);
	for (j = 0; j < m_iNumStandardCols + 4; j++)
	{
		UINT nFormat;
		if (j >= 2 && j < m_iNumStandardCols) // numbers
			nFormat = nFormatNumber;
		else if (j >= m_iNumStandardCols && j < m_iNumStandardCols + 3) // checks
			nFormat = nFormatCenter;
		else // station names, comments
			nFormat = nFormatString;

		m_myGridControl.SetItemFormat(0, j, nFormatHeader); // column headers
		for (i = 1; i < MAX_GRID_ENTRIES; i++)
		{
			m_myGridControl.SetItemFormat(i, j, nFormat); // other cells
		}
	}
	
	//set initial focus
	m_myGridControl.SetFocusCell(1, 0);

	CPropertyPage::OnSetActive();

	return TRUE;
}

void CLegGridPage::SetFlags(CSurveyShot * pShot)
{
	int row = 0;
	int col = m_iNumStandardCols;
	if (m_myGridControl.m_hWnd==NULL)
	{
		return;
	}
	if ((pShot->GetShotFlags()&SHOT_SURFACE)!=0)
	{
		m_myGridControl.SetItemText(row, col, _T("X"));
	}
	if ((pShot->GetShotFlags()&SHOT_SPLAY_SHOT)!=0)
	{
		m_myGridControl.SetItemText(row, col+1, _T("X"));
	}
	if ((pShot->GetShotFlags()&SHOT_EXCLUDE_FROM_LENGTH_TOTALS)!=0)
	{
		m_myGridControl.SetItemText(row, col+2, _T("X"));
	}
}

int CLegGridPage::SetComments(int iRow, CSurveyShot * pShot)
{
	INT_PTR iNumShots=pShot->m_Comments.GetSize();
	for (INT_PTR i=0;i<iNumShots;i++)
	{
		CComment * pComment=pShot->m_Comments.GetAt(i);
		CString CS;
		if (!pComment->GetType()[0]==0)
		{
			CS=pComment->GetType();
			CS+= _T(":");
		}
		CS+=pComment->GetBody();
		m_myGridControl.SetItemText(iRow, m_iNumStandardCols+3, CS);
		iRow++;
	}
	if (iNumShots==0)
	{
		iRow++;
	}
	return iRow;
}


BOOL CLegGridPage::NeedRecalculate() const
{
	return TRUE;
}

BOOL CLegGridPage::GetDiveRow(int iLine,CSurveyShot ** ppShot,CSurveyShot * pPrevious)
{
	*ppShot=NULL;

	CString csLength;
	CString csCompass;
	CString csDepth;
	CString csUp;
	CString csDown;
	CString csLeft;
	CString csRight;
	CString csFrom;
	CString csTo;

	csLength=m_myGridControl.GetItemText(iLine+1, m_Leg->FindEntry(_T('T')));
	csCompass=m_myGridControl.GetItemText(iLine+1, m_Leg->FindEntry(_T('A')));
	csDepth=m_myGridControl.GetItemText(iLine+1, m_Leg->FindEntry(_T('I')));
	csFrom=m_myGridControl.GetItemText(iLine+1, 0);
	csTo=m_myGridControl.GetItemText(iLine+1, 1);
	csLeft=m_myGridControl.GetItemText(iLine+1, m_Leg->FindEntry(_T('L')));
	csRight=m_myGridControl.GetItemText(iLine+1, m_Leg->FindEntry(_T('R')));
	csUp=m_myGridControl.GetItemText(iLine+1, m_Leg->FindEntry(_T('U')));
	csDown=m_myGridControl.GetItemText(iLine+1, m_Leg->FindEntry(_T('D')));
	if (csCompass.IsEmpty() &&
		csDepth.IsEmpty() &&
		csLength.IsEmpty() && 
		csUp.IsEmpty() &&
		csDown.IsEmpty() &&
		csLeft.IsEmpty() &&
		csRight.IsEmpty() &&
		csFrom.IsEmpty() &&
		csTo.IsEmpty())
	{
		if (pPrevious!=NULL)
		{
			GetComments(pPrevious);
		}
		return TRUE;	//no problem.  Just an empty shot
	}

	if (csCompass.IsEmpty() ||
		csDepth.IsEmpty() ||
		csFrom.IsEmpty() ||
		csTo.IsEmpty() ||
		csLength.IsEmpty())
	{
		//From without other readings could be a wall dimension defining shot.
		if (csFrom.IsEmpty() || !csTo.IsEmpty() || !csCompass.IsEmpty() || csDepth.IsEmpty())
		{
			LegGridMessageBox(IDS_INCOMPLETESHOTDEPTH,iLine+1,1);
			return FALSE;
		}
    }

	if (_tcsstr(csFrom, _T(" "))!=NULL)
	{
		LegGridMessageBox(IDS_BADSTATIONNAME,iLine+1,0);
		return FALSE;
	}
	if (_tcsstr(csTo, _T(" "))!=NULL)
	{
		LegGridMessageBox(IDS_BADSTATIONNAME,iLine+1,1);
		return FALSE;
	}								

	float fUp,fDown,fLeft,fRight;		//validity check of field formats
	float fLength,fCompass,fDepth;
	if (!FloatFromString(csUp,fUp))
	{
		LegGridMessageBox(IDS_ENTERVALIDNUMBER,iLine+1,m_Leg->FindEntry(_T('U')));
		return FALSE;
	}
	if (!FloatFromString(csDown,fDown))
	{
		LegGridMessageBox(IDS_ENTERVALIDNUMBER,iLine+1,m_Leg->FindEntry(_T('D')));
		return FALSE;
	}
	if (!FloatFromString(csLeft,fLeft))
	{
		LegGridMessageBox(IDS_ENTERVALIDNUMBER,iLine+1,m_Leg->FindEntry(_T('L')));
		return FALSE;
	}
	if (!FloatFromString(csRight,fRight))
	{
		LegGridMessageBox(IDS_ENTERVALIDNUMBER,iLine+1,m_Leg->FindEntry(_T('R')));
		return FALSE;
	}
	if (!FloatFromString(csLength,fLength))
	{
		LegGridMessageBox(IDS_ENTERVALIDNUMBER,iLine+1,m_Leg->FindEntry(_T('T')));
		return FALSE;
	}
	if (!FloatFromString(csCompass,fCompass))
	{
		LegGridMessageBox(IDS_ENTERVALIDNUMBER,iLine+1,m_Leg->FindEntry(_T('A')));
		return FALSE;
	}
	if (m_Leg->GetEuroDegrees())
	{
		if (fCompass!=NAN && (fCompass<-400.0f || fCompass>400.0f) )
		{
			LegGridMessageBox(IDS_COMPASSRANGEEURO,iLine+1,m_Leg->FindEntry(_T('A')));
			return FALSE;
		}
		if (fCompass!=NAN)
		{
			fCompass=fCompass*360.0f/400.0f;	//convert.  Always store in standard units
		}
	}
	else
	{
		if (fCompass!=NAN && (fCompass<-360.0f || fCompass>360.0) )
		{
			LegGridMessageBox(IDS_COMPASSRANGE,iLine+1,m_Leg->FindEntry(_T('A')));
			return FALSE;
		}
	}
	if (!FloatFromString(csDepth,fDepth))
	{
		LegGridMessageBox(IDS_ENTERVALIDNUMBER,iLine+1,m_Leg->FindEntry(_T('I')));
		return FALSE;
	}

	CCaveDiveSurveyShot *pShot=new CCaveDiveSurveyShot(m_Leg);
	pShot->SetStationNames(csFrom,csTo);
	pShot->SetReadings(fLength,fCompass,fDepth);
	pShot->SetUDLR(fUp,fDown,fLeft,fRight);

	GetFlags(pShot);
	GetComments(pShot);
	*ppShot=pShot;

	return TRUE;
}

void CLegGridPage::GetComments(CSurveyShot * pShot)
{
	int row = m_myGridControl.GetFocusCell().row;
	int col = m_iNumStandardCols+3;
	CString CS=m_myGridControl.GetItemText(row, col);
	if (!CS.IsEmpty())
	{
		pShot->AddComment(CS);
	}
}

void CLegGridPage::GetFlags(CSurveyShot * pShot)
{
	CString CS;
	int row = m_myGridControl.GetFocusCell().row;
	int col = m_iNumStandardCols;
	CS=m_myGridControl.GetItemText(row, col);
	if (!CS.IsEmpty())
	{
		pShot->SetShotFlags(pShot->GetShotFlags()|SHOT_SURFACE);
	}
	CS=m_myGridControl.GetItemText(row, col+1);
	if (!CS.IsEmpty())
	{
		pShot->SetShotFlags(pShot->GetShotFlags()|SHOT_SPLAY_SHOT);
	}
	CS=m_myGridControl.GetItemText(row, col+2);
	if (!CS.IsEmpty())
	{
		pShot->SetShotFlags(pShot->GetShotFlags()|SHOT_EXCLUDE_FROM_LENGTH_TOTALS);
	}
}

BOOL CLegGridPage::GetStandardRow(int iLine,CSurveyShot **ppShot,CSurveyShot * pPrevious)
{
	*ppShot=NULL;

	CString csLength;
	CString csCompass;
	CString csCompass2;
	CString csClino;
	CString csClino2;
	CString csUp;
	CString csDown;
	CString csLeft;
	CString csRight;
	CString csFrom;
	CString csTo;

	csLength=m_myGridControl.GetItemText(iLine+1, m_Leg->FindEntry(_T('T')));
	csCompass=m_myGridControl.GetItemText(iLine+1, m_Leg->FindEntry(_T('A')));
	csClino=m_myGridControl.GetItemText(iLine+1, m_Leg->FindEntry(_T('I')));
	csFrom=m_myGridControl.GetItemText(iLine+1, 0);
	csTo=m_myGridControl.GetItemText(iLine+1, 1);
	csLeft=m_myGridControl.GetItemText(iLine+1, m_Leg->FindEntry(_T('L')));
	csRight=m_myGridControl.GetItemText(iLine+1, m_Leg->FindEntry(_T('R')));
	csUp=m_myGridControl.GetItemText(iLine+1, m_Leg->FindEntry(_T('U')));
	csDown=m_myGridControl.GetItemText(iLine+1, m_Leg->FindEntry(_T('D')));
	if (m_Leg->HasBacksights())
	{
		csCompass2=m_myGridControl.GetItemText(iLine+1, m_Leg->FindEntry(_T('A'))+1);
		csClino2=m_myGridControl.GetItemText(iLine+1, m_Leg->FindEntry(_T('I'))+1);
	}

	if (csCompass.IsEmpty() &&
		csCompass2.IsEmpty() &&
		csClino.IsEmpty() &&
		csClino2.IsEmpty() &&
		csUp.IsEmpty() &&
		csDown.IsEmpty() &&
		csLeft.IsEmpty() &&
		csRight.IsEmpty() &&
		csFrom.IsEmpty() &&
		csTo.IsEmpty())
	{
		if (pPrevious!=NULL)
		{
			GetComments(pPrevious);
		}
		return TRUE;	//no problem.  Just an empty shot
	}

	if ((csCompass.IsEmpty() && csCompass2.IsEmpty()) ||
		(csClino.IsEmpty() && csClino2.IsEmpty()) ||
		csFrom.IsEmpty() ||
		csTo.IsEmpty() ||
		csLength.IsEmpty())
	{
		//From without other readings could be a wall dimension defining shot.
		if (csFrom.IsEmpty() || !csTo.IsEmpty() || !csCompass.IsEmpty() || !csClino.IsEmpty())
		{
			LegGridMessageBox(IDS_INCOMPLETESHOT,iLine+1,1);
			return FALSE;
		}
    }

	if (_tcsstr(csFrom, _T(" "))!=NULL)
	{
		LegGridMessageBox(IDS_BADSTATIONNAME,iLine+1,0);
		return FALSE;
	}
	if (_tcsstr(csTo, _T(" "))!=NULL)
	{
		LegGridMessageBox(IDS_BADSTATIONNAME,iLine+1,1);
		return FALSE;
	}								

	float fUp,fDown,fLeft,fRight;		//validity check of field formats
	float fLength,fCompass,fClino,fCompass2,fClino2;
	if (!FloatFromString(csUp,fUp))
	{
		LegGridMessageBox(IDS_ENTERVALIDNUMBER,iLine+1,m_Leg->FindEntry(_T('U')));
		return FALSE;
	}
	if (!FloatFromString(csDown,fDown))
	{
		LegGridMessageBox(IDS_ENTERVALIDNUMBER,iLine+1,m_Leg->FindEntry(_T('D')));
		return FALSE;
	}
	if (!FloatFromString(csLeft,fLeft))
	{
		LegGridMessageBox(IDS_ENTERVALIDNUMBER,iLine+1,m_Leg->FindEntry(_T('L')));
		return FALSE;
	}
	if (!FloatFromString(csRight,fRight))
	{
		LegGridMessageBox(IDS_ENTERVALIDNUMBER,iLine+1,m_Leg->FindEntry(_T('R')));
	}
	if (!FloatFromString(csLength,fLength))
	{
		LegGridMessageBox(IDS_ENTERVALIDNUMBER,iLine+1,m_Leg->FindEntry(_T('T')));
		return FALSE;
	}
	if (!FloatFromString(csCompass,fCompass))
	{
		LegGridMessageBox(IDS_ENTERVALIDNUMBER,iLine+1,m_Leg->FindEntry(_T('A'))+1);
		return FALSE;
	}
	if (m_Leg->GetEuroDegrees())
	{
		if (fCompass!=NAN && (fCompass<-400.0f || fCompass>400.0f) )
		{
			LegGridMessageBox(IDS_COMPASSRANGEEURO,iLine+1,m_Leg->FindEntry(_T('A')));
			return FALSE;
		}
		if (fCompass!=NAN)
		{
			fCompass=fCompass*360.0f/400.0f;	//convert.  Always store in standard units
		}
	}
	else
	{
		if (fCompass!=NAN && (fCompass<-360.0f || fCompass>360.0) )
		{
			LegGridMessageBox(IDS_COMPASSRANGE,iLine+1,m_Leg->FindEntry(_T('A')));
			return FALSE;
		}
	}
	if (!FloatFromString(csCompass2,fCompass2))
	{
		LegGridMessageBox(IDS_ENTERVALIDNUMBER,iLine+1,m_Leg->FindEntry(_T('A'))+1);
		return FALSE;
	}
	if (m_Leg->GetEuroDegrees())
	{
		if (fCompass2!=NAN && (fCompass2<-400.0f || fCompass2>400.0f) )
		{
			LegGridMessageBox(IDS_COMPASSRANGEEURO,iLine+1,m_Leg->FindEntry(_T('A'))+1);
			return FALSE;
		}
		if (fCompass2!=NAN)
		{
			fCompass2=fCompass2*360.0f/400.0f;	//convert.  Always store in standard units
		}
	}
	else
	{
		if (fCompass2!=NAN && (fCompass2<-360.0f || fCompass2>360.0) )
		{
			LegGridMessageBox(IDS_COMPASSRANGE,iLine+1,m_Leg->FindEntry(_T('A'))+1);
			return FALSE;
		}
	}
	if (fCompass!=NAN && fCompass2!=NAN)
	{
		float fDiff;
		if (fCompass>fCompass2)
		{
			fDiff=fCompass-fCompass2;
		}
		else
		{
			fDiff=fCompass2-fCompass;
		}
		//Difference should ideally be 180.
		if (fDiff<160.0f || fDiff>200.0f)
		{
			LegGridMessageBox(IDS_COMPASSMISMATCH,iLine+1,m_Leg->FindEntry(_T('A')));
			return FALSE;
		}
	}
	if (!FloatFromString(csClino,fClino))
	{
		LegGridMessageBox(IDS_ENTERVALIDNUMBER,iLine+1,m_Leg->FindEntry(_T('I')));
		return FALSE;
	}
	if (m_Leg->GetEuroDegrees())
	{
		if (fClino!=NAN && (fClino<-100.0f || fClino>100.0f))
		{
			LegGridMessageBox(IDS_CLINORANGEEURO,iLine+1,m_Leg->FindEntry(_T('I')));
			return FALSE;
		}
		if (fClino!=NAN)
		{
			fClino=fClino*90.0f/100.0f;
		}
	}
	else
	{
		if (fClino!=NAN && (fClino<-90.0f || fClino>90.0f))
		{
			LegGridMessageBox(IDS_CLINORANGE,iLine+1,m_Leg->FindEntry(_T('I')));
			return FALSE;
		}
	}
	if (!FloatFromString(csClino2,fClino2))
	{
		LegGridMessageBox(IDS_ENTERVALIDNUMBER,iLine+1,m_Leg->FindEntry(_T('I'))+1);
		return FALSE;
	}
	if (m_Leg->GetEuroDegrees())
	{
		if (fClino2!=NAN && (fClino2<-100.0f || fClino2>100.0f))
		{
			LegGridMessageBox(IDS_CLINORANGEEURO,iLine+1,m_Leg->FindEntry(_T('I'))+1);
			return FALSE;
		}
		if (fClino2!=NAN)
		{
			fClino2=fClino2*90.0f/100.0f;
		}
	}
	else
	{
		if (fClino2!=NAN && (fClino2<-90.0f || fClino2>90.0f))
		{
			LegGridMessageBox(IDS_CLINORANGE,iLine+1,m_Leg->FindEntry(_T('I'))+1);
			return FALSE;
		}
	}
	if (fClino!=NAN && fClino2!=NAN && fClino+fClino2>10.0f)
	{
		LegGridMessageBox(IDS_BACKSIGHTMISMATCH,iLine+1,m_Leg->FindEntry(_T('I')));
		return FALSE;
	}

	CStandardSurveyShot *pShot=new CStandardSurveyShot(m_Leg);
	pShot->SetStationNames(csFrom,csTo);
	pShot->SetReadings(fLength,fCompass,fClino,fCompass2,fClino2);
	pShot->SetUDLR(fUp,fDown,fLeft,fRight);

	GetFlags(pShot);
	GetComments(pShot);
	*ppShot=pShot;
	return TRUE;
}

void CLegGridPage::OnInsertLine()
{
	int row = m_myGridControl.GetFocusCell().row;

	m_myGridControl.InsertRow(_T(""), row);
	m_myGridControl.Refresh();
}

void CLegGridPage::OnAutoNumber() 
{
	CCellRange selRange = m_myGridControl.GetSelectedCellRange();
	int iTopY = selRange.GetMinRow();
	int iBottomY = selRange.GetMaxRow();
	int iLeftX = selRange.GetMinCol();
	int iRightX = selRange.GetMaxCol();
	TCHAR szFromBase[MAX_STATION_NAME_LENGTH];
	TCHAR szToBase[MAX_STATION_NAME_LENGTH];
	szFromBase[0]=_T('\0');
	szToBase[0]=_T('\0');
	if (iTopY==iBottomY)
	{
		AfxMessageBox(IDS_SELECTFILLAREA);
		return;
	}

	int iFromColumn=0;
	int iToColumn=1;
	BOOL bFrom=iFromColumn>=iLeftX&& iFromColumn<=iRightX;
	BOOL bTo=iToColumn>=iLeftX && iToColumn<=iRightX;
	if (bFrom==FALSE && bTo==FALSE)
	{
		AfxMessageBox(IDS_FILLNEEDSFROMANDTO);
		return;
	}
	lstrcpyn(szFromBase,m_myGridControl.GetItemText(iTopY, iFromColumn),MAX_STATION_NAME_LENGTH);
	lstrcpyn(szToBase,m_myGridControl.GetItemText(iTopY, iToColumn),MAX_STATION_NAME_LENGTH);

	//Split the From station name to BASE+NUMBER
	int iStrLen=lstrlen(szFromBase);	
	LPTSTR szFromDigits=szFromBase;
	for (int k=iStrLen-1;k>=0;k--)
	{
		if (_tcschr(_T("0123456789"),(int)szFromBase[k])==NULL)
		{
			szFromDigits=szFromBase+k+1;
			break;
		}
	}
	int iFromDigit=_tstoi(szFromDigits);
	szFromDigits[0]=_T('\0');

	//Split the To station name to BASE+NUMBER
	iStrLen=lstrlen(szToBase);	
	LPTSTR szToDigits=szToBase;
	for (int k=iStrLen-1;k>=0;k--)
	{
		if (_tcschr(_T("0123456789"),(int)szToBase[k])==NULL)
		{
			szToDigits=szToBase+k+1;
			break;
		}
	}
	int iToDigit=_tstoi(szToDigits);
	szToDigits[0]=_T('\0');

	if (bFrom && bTo)
	{
		if ((iToDigit!=iFromDigit-1 && iToDigit!=iFromDigit+1) || (lstrcmpi(szToBase,szFromBase)!=0))
		{
			AfxMessageBox(IDS_BADFILLPATTERN);
			return;
		}
	}

	TCHAR szWorking[MAX_STATION_NAME_LENGTH+4];
	//Now do all the numbering.
	for (int i=iTopY+1;i<=iBottomY;i++)
	{
		if (bFrom)
		{
			iFromDigit++;
			_stprintf_s(szWorking, _T("%s%i"),szFromBase,iFromDigit);
			m_myGridControl.SetItemText(i, iFromColumn, szWorking);
		}
		if (bTo)
		{
			iToDigit++;
			_stprintf_s(szWorking, _T("%s%i"),szToBase,iToDigit);
			m_myGridControl.SetItemText(i, iToColumn, szWorking);
		}
	}

	m_myGridControl.Refresh();
}


void CLegGridPage::LegGridMessageBox(int iMessageId,int iRow,int iCol)
{
	m_myGridControl.SetSelectedRange(iRow, iCol, iRow, iCol);
	AfxMessageBox(iMessageId);
	m_myGridControl.SetFocus();
}

void CLegGridPage::OnBnClickedUndo() 
{
	m_myGridControl.OnEditUndo();
}

void CLegGridPage::OnBnClickedCut()
{
	m_myGridControl.OnEditCut();
}

void CLegGridPage::OnBnClickedCopy()
{
	m_myGridControl.OnEditCopy();
}

void CLegGridPage::OnBnClickedPaste()
{
	m_myGridControl.OnEditPaste();
}

void CLegGridPage::OnAutoSize()
{
	m_myGridControl.AutoSizeColumns();
	m_myGridControl.Refresh();
}
