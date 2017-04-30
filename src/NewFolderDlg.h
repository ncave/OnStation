// newfold.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewFolderDialog dialog

class CNewFolderDialog : public CDialog
{
private:
	CString m_szName;
//	BOOL m_bRename;
// Construction
public:
	LPCTSTR GetName();
	CNewFolderDialog(LPCTSTR szValue,CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CNewFolderDialog)
	enum { IDD = IDD_NEWFOLDER };
	CEdit	m_FolderControl;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CNewFolderDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
