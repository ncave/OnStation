#include "stdafx.h"
#include "onstation.h"
#include "editablegrid.h"
//#include "clipboard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditableGrid

CEditableGrid::CEditableGrid()
{
	m_bIsModified=FALSE;
}

CEditableGrid::~CEditableGrid()
{
	for (int i=0;i<m_UndoArray.GetSize();i++)
	{
		CUndoStructure * pUndo=m_UndoArray.GetAt(i);
		delete pUndo;
	}
	m_UndoArray.RemoveAll();
}

BEGIN_MESSAGE_MAP(CEditableGrid, CGridCtrl)
	//{{AFX_MSG_MAP(CEditableGrid)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditableGrid message handlers

void CEditableGrid::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (IsCTRLpressed() && (nChar == _T('Z')))
		OnEditUndo();

	CGridCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CEditableGrid::PushUndoEvent()
{
	m_bIsModified=TRUE;
	CUndoStructure * pUndo=new CUndoStructure();
	m_UndoArray.Add(pUndo);
	if (m_UndoArray.GetSize()>GRID_UNDO_DEPTH)
	{
		TRACE("Undo buffer full, removing oldest item\n");
		delete m_UndoArray[0];
		m_UndoArray.RemoveAt(0);
	}
}

void CEditableGrid::AddUndoItem(int iRow, int iCol, CString itemText)
{
	CUndoItem * pItem=new CUndoItem();
	pItem->m_iXpos=iCol;
	pItem->m_iYpos=iRow;
	pItem->m_szOldContents=itemText;
	TRACE("Adding Row %i Col %i text %s to Undo\n",iRow,iCol,(LPCTSTR)pItem->m_szOldContents);
	//This fails if we forgot to call PushUndoEvent before calling AddUndoItem
	ASSERT(m_UndoArray.GetSize()>=1);
	CUndoStructure * pUndoStruct=m_UndoArray[m_UndoArray.GetSize()-1];
	pUndoStruct->m_ItemArray.Add(pItem);
}

void CEditableGrid::OnEndEditCell(int nRow, int nCol, CString str)
{
    CString oldText = GetItemText(nRow, nCol);
    if (str != oldText)
    {
		PushUndoEvent();
		AddUndoItem(nRow, nCol, oldText);
	}
	CGridCtrl::OnEndEditCell(nRow, nCol, str);
}

void CEditableGrid::ValidateAndModifyCellContents(int nRow, int nCol, LPCTSTR strText)
{
    if (!IsCellEditable(nRow, nCol))
        return;
	CString newText = strText;
	CString oldText = GetItemText(nRow, nCol);
	if (newText != oldText)
	{
		AddUndoItem(nRow, nCol, oldText);
	}
	CGridCtrl::ValidateAndModifyCellContents(nRow, nCol, strText);
}

void CEditableGrid::CutSelectedText()
{
    if (!IsEditable())
        return;

	PushUndoEvent();
	CGridCtrl::CutSelectedText();
}

BOOL CEditableGrid::PasteTextToGrid(CCellID cell, COleDataObject* pDataObject, 
								BOOL bSelectPastedCells /*=TRUE*/)
{
    if (!IsValid(cell) || !IsCellEditable(cell) || !pDataObject->IsDataAvailable(CF_TEXT))
        return FALSE;

	PushUndoEvent();
	return CGridCtrl::PasteTextToGrid(cell, pDataObject, bSelectPastedCells);
}

void CEditableGrid::OnEditUndo()
{
	if (m_UndoArray.GetSize()==0)
	{
		MessageBeep(0);
		return;	//nothing to UNDO
	}
	CUndoStructure * pUndoStruct=m_UndoArray[m_UndoArray.GetSize()-1];
	for (int i=0;i<pUndoStruct->m_ItemArray.GetSize();i++)
	{
		CUndoItem * pItem=pUndoStruct->m_ItemArray[i];
		//SetCol(pItem->m_iXpos);
		//SetRow(pItem->m_iYpos);
		//Don't need to set dirty on undo
		SetItemText(pItem->m_iYpos, pItem->m_iXpos, pItem->m_szOldContents);
	}
	delete pUndoStruct;
	m_UndoArray.RemoveAt(m_UndoArray.GetSize()-1);
	Refresh();
}

CUndoStructure::~CUndoStructure()
{
	for (int i=0;i<m_ItemArray.GetSize();i++)
	{
		delete m_ItemArray[i];
		m_ItemArray[i]=0;
	}
}

