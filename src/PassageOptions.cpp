 // PassageOptions.cpp : implementation file
//

#include "stdafx.h"
#include "onstation.h"
#include "PassageOptions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPassageOptions dialog


CPassageOptions::CPassageOptions(CWnd* pParent /*=NULL*/)
	: CDialog(CPassageOptions::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPassageOptions)
	m_bMultipleLights = FALSE;
	m_bDisableLightRotate = FALSE;
	m_bWireFrameRotate = FALSE;
	//}}AFX_DATA_INIT
}


void CPassageOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPassageOptions)
	DDX_Control(pDX, IDC_COMBO2, m_ComboBox);
	DDX_Check(pDX, IDC_MULTIPLELIGHTS, m_bMultipleLights);
	DDX_Check(pDX, IDC_DISABLELIGHTROTATE, m_bDisableLightRotate);
	DDX_Check(pDX, IDC_CHECK2, m_bWireFrameRotate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPassageOptions, CDialog)
	//{{AFX_MSG_MAP(CPassageOptions)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPassageOptions message handlers

BOOL CPassageOptions::OnInitDialog() 
{
	CDialog::OnInitDialog();
//	m_ComboBox.ResetContent();
//	CString CS;
//	CS.LoadString(IDS_PT_LINE_PLOT);
//	m_ComboBox.AddString(CS);
//	CS.LoadString(IDS_PT_HEIGHTS);
//	m_ComboBox.AddString(CS);
//	CS.LoadString(IDS_PT_WIDTHS);
//	m_ComboBox.AddString(CS);
//	CS.LoadString(IDS_PT_CROSS_SECTIONS);
//	m_ComboBox.AddString(CS);
//	CS.LoadString(IDS_PT_FULL_PASSAGES_SKINNY);
//	m_ComboBox.AddString(CS);
//	CS.LoadString(IDS_PT_FULL_PASSAGES);
//	m_ComboBox.AddString(CS);


	int iCurComboSel=0;
	switch (m_PassageType)
	{
		case PT_LINE_PLOT:
			iCurComboSel=0;
			break;
		case PT_HEIGHTS:
			iCurComboSel=1;
			break;
		case PT_WIDTHS:
			iCurComboSel=2;
			break;
		case PT_CROSS_SECTIONS:
			iCurComboSel=3;
			break;
		case PT_FULL_PASSAGES_SKINNY:
			iCurComboSel=4;
			break;
		case PT_FULL_PASSAGES:
			iCurComboSel=5;
			break;
		default:
			ASSERT(FALSE);
	}
	m_ComboBox.SetCurSel(iCurComboSel);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPassageOptions::OnOK() 
{
	switch (m_ComboBox.GetCurSel())
	{
	case 0:
		m_PassageType=PT_LINE_PLOT;
		break;
	case 1:
		m_PassageType=PT_HEIGHTS;
		break;
	case 2:
		m_PassageType=PT_WIDTHS;
		break;
	case 3:
		m_PassageType=PT_CROSS_SECTIONS;
		break;
	case 4:
		m_PassageType=PT_FULL_PASSAGES_SKINNY;
		break;
	case 5:
		m_PassageType=PT_FULL_PASSAGES;
		break;
	}
	CDialog::OnOK();
}
