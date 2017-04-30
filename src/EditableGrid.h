#ifndef I_EDITABLEGRID__
#define I_EDITABLEGRID__

#include "GridCtrl\GridCtrl.h"
#include "afxtempl.h"

/////////////////////////////////////////////////////////////////////////////
// Undo classes

class CUndoItem
{
public:
	CString m_szOldContents;
	int m_iXpos;
	int m_iYpos;
};

class CUndoStructure
{
public:
	CArray<CUndoItem *,CUndoItem *> m_ItemArray;

	~CUndoStructure();
};

/////////////////////////////////////////////////////////////////////////////
// CEditableGrid control

class CEditableGrid : public CGridCtrl
{
// Construction
public:
	CEditableGrid();
	virtual ~CEditableGrid();

// Attributes
public:
	BOOL IsModified() const { return m_bIsModified; }

protected:
	BOOL m_bIsModified;
	CArray<CUndoStructure *,CUndoStructure*> m_UndoArray;

	void PushUndoEvent();
	void AddUndoItem(int iRow, int iCol, CString itemText);

    virtual void OnEndEditCell(int nRow, int nCol, CString str);
	virtual void ValidateAndModifyCellContents(int nRow, int nCol, LPCTSTR strText);

protected:
	//{{AFX_MSG(CEditableGrid)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Operations
public:
	void OnEditUndo();
    virtual void CutSelectedText();
    virtual BOOL PasteTextToGrid(CCellID cell, COleDataObject* pDataObject, BOOL bSelectPastedCells=TRUE);
};

/////////////////////////////////////////////////////////////////////////////
#endif

