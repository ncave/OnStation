// InfoByViewDialog.cpp : implementation file
//

#include "stdafx.h"
#include "onstation.h"
#include "InfoByViewDialog.h"
#include "nodetree.h"
#include "settings.h"

extern COnStationApp theApp;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInfoByViewDialog dialog

/*	float m_fDeepest;
	TCHAR m_szDeepest[MAX_STATION_NAME_LENGTH];
	float m_fHighest;
	TCHAR m_szHighest[MAX_STATION_NAME_LENGTH];
	float m_fSurfaceLength;
	float m_fCaveLength;
	int m_iCaveShots;
	int m_iSurfaceShots;
*/

CInfoByViewDialog::CInfoByViewDialog(CNodeTreeSummary *NTS,CWnd* pParent /*=NULL*/)
	: CDialog(CInfoByViewDialog::IDD, pParent)
{
	m_NTS=NTS;
	const int BUFFER_SIZE = 64;
	TCHAR szBuffer[BUFFER_SIZE];
	TCHAR szBuffer2[BUFFER_SIZE];
	TCHAR szBuffer3[BUFFER_SIZE];
	TCHAR szBuffer4[BUFFER_SIZE];
	TCHAR szBuffer5[BUFFER_SIZE];
	TCHAR szBuffer6[BUFFER_SIZE];
	TCHAR szBuffer7[BUFFER_SIZE];
	_stprintf_s(szBuffer, _T("%i"),NTS->m_iCaveShots);
	_stprintf_s(szBuffer2, _T("%i"),NTS->m_iSurfaceShots);
	_stprintf_s(szBuffer3, _T("%s\t%s"),NTS->m_szDeepest,GetPreferedUnitString(NTS->m_fDeepest));
	_stprintf_s(szBuffer4, _T("%s\t%s"),NTS->m_szHighest,GetPreferedUnitString(NTS->m_fHighest));
	_stprintf_s(szBuffer5, _T("%s"),GetPreferedUnitString(NTS->m_fCaveLength));
	_stprintf_s(szBuffer6, _T("%s"),GetPreferedUnitString(NTS->m_fSurfaceLength));
	_stprintf_s(szBuffer7, _T("%s"),GetPreferedUnitString(NTS->m_fHorizontalLength));
	//{{AFX_DATA_INIT(CInfoByViewDialog)
	m_NumShots = szBuffer;
	m_NumSurfaceShots = szBuffer2;
	m_szDeepestPoint = szBuffer3;
	m_szHighestPoint = szBuffer4;
	m_TotalLength = szBuffer5;
	m_TotalSurfaceLength = szBuffer6;
	m_HorizontalLength=szBuffer7;
	//}}AFX_DATA_INIT
}


void CInfoByViewDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInfoByViewDialog)
	DDX_Text(pDX, IDC_DEEPESTPOINT, m_szDeepestPoint);
	DDX_Text(pDX, IDC_HIGHESTPOINT, m_szHighestPoint);
	DDX_Text(pDX, IDC_NUMSHOTS, m_NumShots);
	DDX_Text(pDX, IDC_NUMSURFACESHOTS, m_NumSurfaceShots);
	DDX_Text(pDX, IDC_TOTALLENGTH, m_TotalLength);
	DDX_Text(pDX, IDC_HORIZONTALLENGTH,m_HorizontalLength);
	DDX_Text(pDX, IDC_TOTALSURFACELENGTH, m_TotalSurfaceLength);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInfoByViewDialog, CDialog)
	//{{AFX_MSG_MAP(CInfoByViewDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInfoByViewDialog message handlers
