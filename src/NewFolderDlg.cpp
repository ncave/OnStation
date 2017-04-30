// newfold.cpp : implementation file
//

#include "stdafx.h"
#include "onstation.h"
#include "newfolderdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewFolderDialog dialog


CNewFolderDialog::CNewFolderDialog(LPCTSTR szValue,CWnd* pParent /*=NULL*/)
	: CDialog(CNewFolderDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewFolderDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	if (szValue!=NULL)
	{
		m_szName=szValue;
//	 	m_bRename=TRUE;
	}
	else
	{
		m_szName= _T("");
//		m_bRename=FALSE;
	}
}

void CNewFolderDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewFolderDialog)
	DDX_Control(pDX, IDC_EDIT1, m_FolderControl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNewFolderDialog, CDialog)
	//{{AFX_MSG_MAP(CNewFolderDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CNewFolderDialog message handlers

BOOL CNewFolderDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	m_FolderControl.SetFocus();
//	if (m_bRename)
//	{
///		m_FolderControl.SetWindowText(m_szName);
//		SetWindowTex(_T("Rename Folder"));
//	}
	return FALSE;  // return TRUE  unless you set the focus to a control
}


LPCTSTR CNewFolderDialog::GetName()
{
    return m_szName;
}

void CNewFolderDialog::OnOK()
{
	TCHAR szData[128];
	m_FolderControl.GetWindowText(szData,_countof(szData)-1);
	m_szName=szData;
	
	CDialog::OnOK();
}
