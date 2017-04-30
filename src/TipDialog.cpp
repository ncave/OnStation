#include "stdafx.h"
#include "OnStation.h"
#include "TipDialog.h"

#include <winreg.h>
#include <sys\stat.h>
#include <sys\types.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTipDialog dialog

#define MAX_BUFLEN 1000

static const TCHAR szSection[] = _T("Tip");
static const TCHAR szTimeStamp[] = _T("TimeStamp");
static const TCHAR szIntLinePos[] = _T("LinePos");
static const TCHAR szIntStartup[] = _T("StartUp");

CTipDialog::CTipDialog(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_TIP, pParent)
{
	//{{AFX_DATA_INIT(CTipDialog)
	m_bStartup = TRUE;
	//}}AFX_DATA_INIT

	m_iLinePos = 0;

	CWinApp* pApp = AfxGetApp();
	m_bStartup = !pApp->GetProfileInt(szSection, szIntStartup, 0);
}

void CTipDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTipDialog)
	DDX_Check(pDX, IDC_STARTUP, m_bStartup);
	DDX_Text(pDX, IDC_TIPSTRING, m_strTip);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTipDialog, CDialog)
	//{{AFX_MSG_MAP(CTipDialog)
	ON_BN_CLICKED(IDC_NEXTTIP, OnNextTip)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTipDialog message handlers

BOOL CTipDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	LoadTips();
	GetNextTipString();
	UpdateData(FALSE);

	// If Tips file does not exist then disable NextTip
	if (m_tips.GetCount() == 0)
		GetDlgItem(IDC_NEXTTIP)->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CTipDialog::OnNextTip()
{
	GetNextTipString();
	UpdateData(FALSE);
}

HBRUSH CTipDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (pWnd->GetDlgCtrlID() == IDC_TIPSTRING)
		return (HBRUSH)GetStockObject(WHITE_BRUSH);

	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CTipDialog::OnOK()
{
	CDialog::OnOK();
	
    // Update the startup information stored in the INI file
	CWinApp* pApp = AfxGetApp();
	pApp->WriteProfileInt(szSection, szIntStartup, !m_bStartup);
}

void CTipDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// Get paint area for the big static control
	CWnd* pStatic = GetDlgItem(IDC_BULB);
	CRect rect;
	pStatic->GetWindowRect(&rect);
	ScreenToClient(&rect);

	// Paint the background white.
	CBrush brush;
	brush.CreateStockObject(WHITE_BRUSH);
	dc.FillRect(rect, &brush);

	// Load bitmap and get dimensions of the bitmap
	CBitmap bmp;
	bmp.LoadBitmap(IDB_LIGHTBULB);
	BITMAP bmpInfo;
	bmp.GetBitmap(&bmpInfo);

	// Draw bitmap in top corner and validate only top portion of window
	CDC dcTmp;
	dcTmp.CreateCompatibleDC(&dc);
	dcTmp.SelectObject(&bmp);
	rect.bottom = bmpInfo.bmHeight + rect.top;
	dc.BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), 
		&dcTmp, 0, 0, SRCCOPY);

	// Draw out "Did you know..." message next to the bitmap
	CString strMessage;
	strMessage.LoadString(CG_IDS_DIDYOUKNOW);
	rect.left += bmpInfo.bmWidth;
	dc.DrawText(strMessage, rect, DT_VCENTER | DT_SINGLELINE);

	// Do not call CDialog::OnPaint() for painting messages
}

void CTipDialog::LoadTips()
{
	m_tips.RemoveAll();
	m_strTip.LoadString(CG_IDS_FILE_ABSENT);

	// We need to find out what the startup and file position parameters are
	// If startup does not exist, we assume that the Tips on startup is checked TRUE.
	CWinApp* pApp = AfxGetApp();
	m_iLinePos = pApp->GetProfileInt(szSection, szIntLinePos, 0);

	// Now try to open the tips file
	FILE *pStream;
	errno_t err;
#ifdef _UNICODE
	err = _tfopen_s(&pStream, _T("tips.txt"), _T("rt, ccs=UTF-8"));
#else
	err = _tfopen_s(&pStream, _T("tips.txt"), _T("rt"));
#endif
	if (err != 0) 
	{
		//AfxMessageBox(CG_IDP_FILE_CORRUPT);
		return;
	} 

	// If the timestamp in the INI file is different from the timestamp of
	// the tips file, then we know that the tips file has been modified
	// Reset the file position to 0 and write the latest timestamp to the
	// ini file
	TCHAR timeBuf[26];
	struct _stat buf;
	_fstat(_fileno(pStream), &buf);
	_tctime_s(timeBuf, _countof(timeBuf), &buf.st_ctime);
	CString strCurrentTime = timeBuf;
	strCurrentTime.TrimRight();
	CString strStoredTime = pApp->GetProfileString(szSection, szTimeStamp, NULL);
	if (strCurrentTime != strStoredTime) 
	{
		m_iLinePos = 0;
		pApp->WriteProfileString(szSection, szTimeStamp, strCurrentTime);
	}

	// real all tips
	CStdioFile f(pStream);
	CString line;
	while (f.ReadString(line))
	{
		// There should be no space at the beginning of the tip
		// This behavior is same as VC++ Tips file
		// Comment lines are ignored and they start with a semicolon
		if (line.GetLength() > 0 &&
			line[0] != _T(' ') &&
			line[0] != _T('\t') &&
			line[0] != _T('\n') &&
			line[0] != _T(';'))
		{
			m_tips.Add(line);
		}
	}
	f.Close();
}

void CTipDialog::GetNextTipString()
{
	// This routine identifies the next string that needs to be
	// read from the tips file
	if (m_iLinePos < 0 || m_iLinePos >= m_tips.GetCount()) 
	{
		// We have either reached EOF or enocuntered some problem
		// In both cases reset the pointer to the beginning of the file
		// This behavior is same as VC++ Tips file
		m_iLinePos = 0;
	} 

	// get next tip
	m_strTip = m_tips[m_iLinePos];
	m_iLinePos++;

	// store the line pos
	CWinApp* pApp = AfxGetApp();
	pApp->WriteProfileInt(szSection, szIntLinePos, m_iLinePos);
}
