// BookmarkDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBookmarkDialog dialog
class CBookMark;
class CBookmarkDialog : public CDialog
{
private:
	void ResetControls();
	void UpdateStaticControls(CBookMark *BM);
	// Construction
public:
	CBookmarkDialog(float fX,float fY,float fZ,float fRot,float fTilt,float fZoom,int iColorSchemeIndex,CWnd* pParent = NULL);   // standard constructor

	BOOL m_bChanged;
// Dialog Data
	//{{AFX_DATA(CBookmarkDialog)
	enum { IDD = IDD_BOOKMARKS };
	CButton	m_CloseButton;
	CButton	m_GotoButton;
	CButton	m_DeleteButton;
	CButton	m_AddButton;
	CListBox	m_ListBox;
	CStatic	m_RotateText;
	CStatic	m_PanText;
	CComboBox	m_ComboBox;
	//}}AFX_DATA
	CBookMark * m_GotoMe;
	float m_fX;
	float m_fY;
	float m_fZ;
	float m_fRot;
	float m_fTilt;
	float m_fZoom;
	int m_iColorSchemeIndex;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBookmarkDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBookmarkDialog)
	afx_msg void OnAdd();
	afx_msg void OnDelete();
	afx_msg void OnGoto();
 	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkBookmarklist();
	afx_msg void OnSelchangeBookmarklist();
	afx_msg void OnEditchangeBookmarkcombo();
	afx_msg void OnSelchangeBookmarkcombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
