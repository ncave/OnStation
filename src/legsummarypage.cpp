// summarypage.cpp : implementation file
//

#include "stdafx.h"
#include "onstation.h"
#include "legsummarypage.h"
#include "surveyleg.h"
#include "surveyshot.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLegSummaryPage property page

IMPLEMENT_DYNCREATE(CLegSummaryPage, CPropertyPage)
BEGIN_MESSAGE_MAP(CLegSummaryPage, CPropertyPage)
	//{{AFX_MSG_MAP(CLegSummaryPage)
	ON_BN_CLICKED(IDC_CHECKMETRIC, OnBigChange)
	ON_BN_CLICKED(IDC_CHECK_SURFACE_SURVEY, OnBigChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


#define new DEBUG_NEW

CLegSummaryPage::CLegSummaryPage() : CPropertyPage(CLegSummaryPage::IDD)
{
	//{{AFX_DATA_INIT(CLegSummaryPage)
	//}}AFX_DATA_INIT
	m_bDirty=FALSE;
}

void CLegSummaryPage::SetLeg(CSurveyLeg *Leg)
{
	m_Leg=Leg;
}

CLegSummaryPage::~CLegSummaryPage()
{
}


BOOL CLegSummaryPage::OnKillActive()
{
	CancelToClose();

	BOOL bMeters=((CButton*)GetDlgItem(IDC_CHECKMETRIC))->GetCheck();
	m_Leg->SetMeters(bMeters);
	BOOL bDiveMeters=((CButton *)GetDlgItem(IDC_DIVEMETERS))->GetCheck();
	m_Leg->SetDiveMeters(bDiveMeters);
	
	BOOL bBacksights=((CButton*)GetDlgItem(IDC_CHECKBACKSIGHTS))->GetCheck();
	if ((!bBacksights && m_Leg->HasBacksights()))
	{
		if (AfxMessageBox(IDS_BACKSIGHTLOSS,MB_YESNO)==IDNO)
		{
			return FALSE;
		}
	}
	CSurveyShotArray * pShotArray=m_Leg->GetShotArray();
	BOOL bEuroDegrees=((CButton*)GetDlgItem(IDC_EURODEGREESCHECK))->GetCheck();
	//only warn if it is changing and we have shots
	if (bEuroDegrees!=m_Leg->GetEuroDegrees() && pShotArray->GetSize()!=0)
	{
		if (AfxMessageBox(IDS_EURODEGREESCHANGED,MB_YESNO)==IDNO)
		{
			return FALSE;
		}
	}
	m_Leg->SetEuroDegrees(bEuroDegrees);

	m_Leg->SetBacksights(bBacksights);
	GetDlgItemText(IDC_COMMENTS,m_Leg->m_szDescription,_countof(m_Leg->m_szDescription)-1);
	TCHAR szBuffer[256];
	CDateTimeCtrl * pDate=(CDateTimeCtrl *)GetDlgItem(IDC_DATEEDIT1);
	if (pDate!=NULL)
	{
		COleDateTime date;
		pDate->GetTime(date);
		m_Leg->m_dwDate=MakeMyDate(date.GetYear(),date.GetMonth(),date.GetDay());
	}

	GetDlgItemText(IDC_SURVEYNAME,szBuffer,256);
	m_Leg->SetName(szBuffer);
	GetDlgItemText(IDC_SYSTEM,m_Leg->m_szSystem,_countof(m_Leg->m_szSystem)-1);
	
	BOOL bNewCaveDive=((CButton*)GetDlgItem(IDC_CAVEDIVECHECK))->GetCheck();
	if (pShotArray->GetSize()!=0 && m_Leg->GetCaveDive()!=bNewCaveDive)
	{
		if (AfxMessageBox(IDS_CAVEDIVEFLAGCHANGING,MB_YESNO)==IDNO)
		{
			return FALSE;
		}
		else
		{
			INT_PTR iCount=pShotArray->GetSize();
			if (!bNewCaveDive)
			{
				for (INT_PTR i=0;i<iCount;i++)
				{
					CCaveDiveSurveyShot * pShotDive=dynamic_cast<CCaveDiveSurveyShot *>(pShotArray->GetAt(i));
					CStandardSurveyShot * pNewShot=new CStandardSurveyShot(m_Leg);
					pNewShot->SetStationNames(pShotDive->GetFromStationName(),pShotDive->GetToStationName());
					pNewShot->SetUDLR(pShotDive->GetRawUp(),pShotDive->GetRawDown(),pShotDive->GetRawLeft(),pShotDive->GetRawRight());
					pNewShot->SetReadings(pShotDive->GetLength(),pShotDive->GetCompass(),pShotDive->GetDepthAtFromStation(),NAN,NAN);
					pShotArray->SetAt(i,pNewShot);
				}
			}
			else
			{
				for (int i=0;i<iCount;i++)
				{
					CStandardSurveyShot * pShotStandard=dynamic_cast<CStandardSurveyShot *>(pShotArray->GetAt(i));
					CCaveDiveSurveyShot * pNewShot=new CCaveDiveSurveyShot(m_Leg);
					pNewShot->SetStationNames(pShotStandard->GetFromStationName(),pShotStandard->GetToStationName());
					pNewShot->SetUDLR(pShotStandard->GetRawUp(),pShotStandard->GetRawDown(),pShotStandard->GetRawLeft(),pShotStandard->GetRawRight());
					pNewShot->SetReadings(pShotStandard->GetLength(),pShotStandard->GetCompass(),pShotStandard->GetClino());
					delete pShotStandard;
					pShotArray->SetAt(i,pNewShot);
				}
			}
			pShotArray->DoDeferredOffsetCalculation();
		}
	}
	m_Leg->SetCaveDive(bNewCaveDive);
	if (bNewCaveDive)
	{
		m_Leg->SetBacksights(FALSE);
		((CButton*)GetDlgItem(IDC_CHECKBACKSIGHTS))->SetCheck(FALSE);
	}

	if (m_Leg->GetName()[0]==0)
	{
		AfxMessageBox(IDS_NEEDSURVEYNAME);
		GetDlgItem(IDC_SURVEYNAME)->SetFocus();
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

void CLegSummaryPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLegSummaryPage)
	//}}AFX_DATA_MAP
}



/////////////////////////////////////////////////////////////////////////////
// CLegSummaryPage message handlers

BOOL CLegSummaryPage::OnInitDialog() 
{
	m_bDirty=FALSE;
	CPropertyPage::OnInitDialog();
	
	((CButton*)GetDlgItem(IDC_DIVEMETERS))->SetCheck(m_Leg->GetDiveMeters());
	((CButton*)GetDlgItem(IDC_CAVEDIVECHECK))->SetCheck(m_Leg->GetCaveDive());
	((CButton*)GetDlgItem(IDC_CHECKMETRIC))->SetCheck( m_Leg->GetMeters());
	((CButton*)GetDlgItem(IDC_CHECKBACKSIGHTS))->SetCheck( m_Leg->HasBacksights());
	((CButton*)GetDlgItem(IDC_EURODEGREESCHECK))->SetCheck(m_Leg->GetEuroDegrees());
	SetDlgItemText(IDC_COMMENTS,m_Leg->m_szDescription);

	SetDlgItemText(IDC_SURVEYNAME,m_Leg->GetName());
	SetDlgItemText(IDC_SYSTEM,m_Leg->m_szSystem);

	GetDlgItem(IDC_SURVEYNAME)->SetFocus();
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLegSummaryPage::OnBigChange() 
{
	m_bDirty=TRUE;
	
}

BOOL CLegSummaryPage::OnSetActive() 
{
	//We have to do this here because it doesn't hold if we do
	//it in the InitDialog.  I think this is a bug in the pDate control.
	//if the date is nonse then don't initialize the controls.
	if (GetYear(m_Leg->m_dwDate)>1000 && GetYear(m_Leg->m_dwDate)<2500)
	{
		CDateTimeCtrl * pDate=(CDateTimeCtrl *)GetDlgItem(IDC_DATEEDIT1);
		if (pDate!=NULL)
		{
			COleDateTime date(GetYear(m_Leg->m_dwDate),GetMonth(m_Leg->m_dwDate),GetDay(m_Leg->m_dwDate),12,0,0);
			pDate->SetTime(date);
		}
		else
		{
			AfxMessageBox(IDS_MISSINGDATECONTROL);
			return FALSE;
		}
	}
	
	return CPropertyPage::OnSetActive();
}
