#ifndef I_ONSTATIONDOC__
#define I_ONSTATIONDOC__
// OnStationDoc.h : interface of the COnStationDoc class
//
/////////////////////////////////////////////////////////////////////////////

#include "settings.h"

class CRealFolder;
class CFileHelper;
class CNodeTree;
class CSurfaceData;
class CSurveyLeg;
class COverallDataView;

class COnStationDoc : public CDocument
{
private:
	void CreateSpecialDataView();
	CRealFolder * m_SurveyDataFolder;
	CNodeTree * m_NodeTree;
	CSurfaceData * m_SurfaceData;

	void Write(CFileHelper * h);
	void Read(CFileHelper * h,BOOL bInsertFile);

protected: // create from serialization only
	COnStationDoc();
	DECLARE_DYNCREATE(COnStationDoc)
	void DeleteContents();

// Attributes
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COnStationDoc)
	public:
	virtual BOOL OnNewDocument();
#ifdef _UNICODE
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
#else
	virtual void Serialize(CArchive& ar);
#endif
	//}}AFX_VIRTUAL

//Information functions
	inline CRealFolder * GetSurveyFolder() {return m_SurveyDataFolder;}
	inline CNodeTree * GetNodeTree() {return m_NodeTree;}
	inline CSurfaceData * GetSurface() {return m_SurfaceData;}

	public:
	virtual ~COnStationDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(COnStationDoc)
	afx_msg void OnFileImport();
	afx_msg void OnFileSaveFormattedText();
	afx_msg void OnInsertFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif


