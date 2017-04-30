#ifndef I_ONSTATIONSURVEYTREE__
#define I_ONSTATIONSURVEYTREE__
// OnStationSurveyTree.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COnStationSurveyTree window
class CFolder;
class CLegQuery;
class COnStationDoc;
class CLegQuery;
class CFixedPoint;
class CSurfaceData;
class CRealFolder;
class CSurveyLeg;
class COverallDataView;
typedef enum SURVEYTREESELECTED {ST_NONE,ST_REALFOLDER,ST_SURVEY,ST_FIXEDPOINT,ST_SURFACE,ST_FIXEDPOINTFOLDER};
class COnStationSurveyTree : public CTreeCtrl
{
private:
	static CImageList m_ImageList;
	COverallDataView * m_pOverallDataView;
	int m_iColorScheme;
	BOOL m_bDragging;
	BOOL m_bFixedPointsFolderIsOpen;
	HTREEITEM m_hDragItem;
	CLegQuery * m_pLegQuery;
	static int m_idxFolder;
	static int m_idxSurvey;
	static int m_idxPin;
	static int m_idxSurfaceFolder;
	static int m_idxSurfaceRoot;
	static int m_idxPinRoot;
	static void InitializeTreeImageList();
	void RecursiveTreeViewFill(CFolder *folder,HTREEITEM hParent,CLegQuery * pLegQuery,HTREEITEM insertAfter=TVI_LAST);
	void SetNewName(HTREEITEM hItem,LPCTSTR szName);
	void AddNewOne(CFolder * newone,BOOL bRecalculate,CFolder * pInsertAfter);
// Construction
public:
	COnStationSurveyTree();

	SURVEYTREESELECTED GetSelectedType();
	CSurveyLeg * GetSelectedSurveyLeg();
	CRealFolder * GetSelectedRealFolder();
	CFixedPoint * GetSelectedFixedPoint();
	CSurfaceData * GetSelectedSurface();
	void InitialUpdate(COverallDataView * pDataView);

	void SetQuery(CLegQuery * pLegQuery);
	CLegQuery * GetLegQuery() {return m_pLegQuery;}

	void SetColorScheme(int iColorScheme);
	void UpdateItems();
	void OnUpdateEditEdit(CCmdUI *);
	void OnUpdateEditCopy(CCmdUI *);
	void OnUpdateEditDelete(CCmdUI *);
	void SetIncludeFixedPoints(BOOL bIncludeFixedPoints);
	void OnDelete();
	void OnEditSelectedItem();
	void OnCopy();
	void OnShowProperties();
	void OnCut();
	void OnPaste(); 
	void OnNewfolder();
	void OnNewsurvey();
	void OnNewFixedPoint();

// Attributes
public:
	BOOL IsDragging() const {return m_bDragging;}
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COnStationSurveyTree)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COnStationSurveyTree();

	void OnLButtonUp(UINT nFlags, CPoint point);
	void OnMouseMove(UINT nFlags, CPoint point);
	//Workaround for Bug number 10000 in this fucking piece of shit OS.
	void FuckYouMicrosoftMorons();
	// Generated message map functions
protected:
	//{{AFX_MSG(COnStationSurveyTree)
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelChanged(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif
