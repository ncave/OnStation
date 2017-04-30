#ifndef I_SURFACESIZEPAGE__
#define I_SURFACESIZEPAGE__// surfacesizepage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSurfaceSizePage dialog
class CSurfaceData;
class CSurfaceSizePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CSurfaceSizePage)
private:
	CSurfaceData * m_SurfaceData;
// Construction
public:
	CSurfaceSizePage();
	~CSurfaceSizePage();

	BOOL m_bDirty;

	void Initialize(CSurfaceData * m_SurfaceData);
	virtual BOOL OnKillActive();	//Do data validation here
// Dialog Data
	int m_iSurfaceGridElementSize;
	//{{AFX_DATA(CSurfaceSizePage)
	enum { IDD = IDD_SURFACESIZEPAGE };
	float	m_fGridNorth;
	int		m_iWest;
	int		m_iSouth;
	int		m_iHeight;
	int		m_iWidth;
	UINT	m_iSurfaceCellSize;
	//}}AFX_DATA

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSurfaceSizePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSurfaceSizePage)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
#endif


