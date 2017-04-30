// AgeColorHelper.cpp : implementation file
//

#include "stdafx.h"
#include "OnStation.h"
#include "AgeColorHelper.h"
#include "mathstuff.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAgeColorHelper dialog


CAgeColorHelper::CAgeColorHelper(DWORD dwStartDate,int iNum,int iYears,CWnd* pParent /*=NULL*/)
	: CDialog(CAgeColorHelper::IDD, pParent)
{
	m_dwStartDate=dwStartDate;

	//{{AFX_DATA_INIT(CAgeColorHelper)
	m_iYears = iYears;
	m_iNumIntervals = iNum;
	//}}AFX_DATA_INIT
}


void CAgeColorHelper::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAgeColorHelper)
	DDX_Text(pDX, IDC_YEARS, m_iYears);
	DDV_MinMaxUInt(pDX, m_iYears, 1, 15);
	DDX_Text(pDX, IDC_NUMINTERVALS, m_iNumIntervals);
	DDV_MinMaxUInt(pDX, m_iNumIntervals, 2, 15);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAgeColorHelper, CDialog)
	//{{AFX_MSG_MAP(CAgeColorHelper)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAgeColorHelper message handlers

void CAgeColorHelper::OnOK() 
{
	//WE don't use update data here because it causes spurious bounds error
	//messages.  That is to say, if there is a problem with one of the edit
	//fields the update for the text causes it to be mentioned twice!
	CDateTimeCtrl * pDate=(CDateTimeCtrl *)GetDlgItem(IDC_DATEEDIT1);
	COleDateTime date;
	pDate->GetTime(date);
	m_dwStartDate=MakeMyDate(date.GetYear(),date.GetMonth(),date.GetDay());
	CDialog::OnOK();
}

BOOL CAgeColorHelper::OnInitDialog() 
{
	CDialog::OnInitDialog();
//Move to OnShow since it doesn't work here for some reason	
//	CDateTimeCtrl * pDate=(CDateTimeCtrl *)GetDlgItem(IDC_DATEEDIT1);
//	COleDateTime date(GetYear(m_dwStartDate),GetMonth(m_dwStartDate),GetDay(m_dwStartDate),12,0,0);
//	pDate->SetValue(date);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAgeColorHelper::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	if (bShow)
	{
		CDateTimeCtrl * pDate=(CDateTimeCtrl *)GetDlgItem(IDC_DATEEDIT1);
		COleDateTime date(GetYear(m_dwStartDate),GetMonth(m_dwStartDate),GetDay(m_dwStartDate),12,0,0);
		pDate->SetTime(date);
	}
	CDialog::OnShowWindow(bShow, nStatus);
}
