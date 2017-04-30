
// OnStationSurveyTree.cpp : implementation file
//

#include "stdafx.h"
#include "onstation.h"
#include "OnStationSurveyTree.h"
#include "onstationdoc.h"
#include "folder.h"
#include "surveyleg.h"
#include "realfolder.h"
#include "legquery.h"
#include "colortool.h"
#include "newfolderdlg.h"
#include "legeditsheet.h"
#include "filehelper.h"
#include "fixedpoint.h"
#include "editfixedpointdialog.h"
#include "surfacedata.h"
#include "folderinfodlg.h"
#include "surfacepropertysheet.h"
#include "nodetree.h"
#include "onstationview.h"
#include "overalldataview.h"
#include "clipboard.h"

extern COnStationView * pView_G;
extern COnStationDoc * pDocument_G;
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
int COnStationSurveyTree::m_idxFolder=-1;
int COnStationSurveyTree::m_idxSurvey=-1;
int COnStationSurveyTree::m_idxPin=-1;
int COnStationSurveyTree::m_idxPinRoot=-1;
int COnStationSurveyTree::m_idxSurfaceFolder=-1;
int COnStationSurveyTree::m_idxSurfaceRoot=-1;

CImageList COnStationSurveyTree::m_ImageList;

COnStationSurveyTree * pSurveyTree_M;

//Timers don't work for this type of window.  God know why?
void CALLBACK EXPORT TimerProc(HWND hWnd,
   UINT nMsg,
   UINT_PTR nIDEvent,
   DWORD dwTime)
{
	//This window is fucked up.  We must do it through a function
	pSurveyTree_M->FuckYouMicrosoftMorons();
}


void COnStationSurveyTree::InitializeTreeImageList()
{
	if (m_idxFolder==-1)
	{
		m_ImageList.Create(16,15,FALSE,20,5);	//allow up to 5 images if it needs to grow (Drag maybe)
		CBitmap hBmp;
		CBitmap hBmp2;
		CBitmap hBmp3;
		CBitmap hBmp4;
		CBitmap hBmp5;
		CBitmap hBmp6;
		hBmp.LoadBitmap(IDB_BMFOLDERS);
		m_idxFolder=m_ImageList.Add(&hBmp,(COLORREF)0);
		hBmp2.LoadBitmap(IDB_BMSURVEYS);
		m_idxSurvey=m_ImageList.Add(&hBmp2,(COLORREF)0);
		hBmp3.LoadBitmap(IDB_BITMAPPIN);
		m_idxPin=m_ImageList.Add(&hBmp3,(COLORREF)0);
		hBmp4.LoadBitmap(IDB_BITMAPPINROOT);
		m_idxPinRoot =m_ImageList.Add(&hBmp4,(COLORREF)0);
		hBmp5.LoadBitmap(IDB_SURFACEFOLDERROOT);
		m_idxSurfaceRoot =m_ImageList.Add(&hBmp5,(COLORREF)0);
		hBmp6.LoadBitmap(IDB_SURFACEFOLDER);
		m_idxSurfaceFolder =m_ImageList.Add(&hBmp6,(COLORREF)0);
	}
}

COnStationSurveyTree::COnStationSurveyTree()
{
	pSurveyTree_M=this;
	InitializeTreeImageList();  //do static initialization
	m_pLegQuery=NULL;
	m_iColorScheme=0;
	m_bDragging=FALSE;
	m_bFixedPointsFolderIsOpen=FALSE;
	m_pOverallDataView=NULL;
}

COnStationSurveyTree::~COnStationSurveyTree()
{
	m_pLegQuery=NULL;
}

void COnStationSurveyTree::InitialUpdate(COverallDataView * pOverallDataView)
{
	SetImageList(&m_ImageList,TVSIL_NORMAL);	
	UpdateItems();
	//Select the first item so that things are more
	//consistent
	HTREEITEM hItem=GetNextItem(NULL,TVGN_CHILD);
	Select(hItem,TVGN_FIRSTVISIBLE);
	m_pOverallDataView=pOverallDataView;
}

void COnStationSurveyTree::SetQuery(CLegQuery * pLegQuery)
{
	delete m_pLegQuery;
	if (pLegQuery!=NULL && !pDocument_G->GetSurveyFolder()->DoesAnythingMatchTheQuery(pLegQuery))
	{
		AfxMessageBox(IDS_QUERYNOMATCHES);
		delete pLegQuery;
		m_pLegQuery=NULL;
		UpdateItems();
	}
	else
	{
		m_pLegQuery=pLegQuery;
		UpdateItems();
	}
}
void COnStationSurveyTree::SetColorScheme(int iColorScheme)
{
	m_iColorScheme=iColorScheme;
	InvalidateRect(NULL,FALSE);
}

void COnStationSurveyTree::UpdateItems()
{
	TCHAR szBuffer[128];

	DeleteAllItems();
	RecursiveTreeViewFill(pDocument_G->GetSurveyFolder(),TVI_ROOT,m_pLegQuery);
	if (m_pLegQuery==NULL)
	{
		TV_INSERTSTRUCT TVIS;
		TVIS.hParent=TVI_ROOT;
		TVIS.hInsertAfter=TVI_LAST;
		LoadString(AfxGetResourceHandle(),IDS_FIXEDPOINTROOTNAME,szBuffer,_countof(szBuffer)-1);	//the TI_ITEM won't take CStrings
		TVIS.item.mask=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
		TVIS.item.state=0;
		TVIS.item.stateMask=0;
		TVIS.item.cchTextMax=MAX_FOLDER_NAME_LENGTH-1;
		TVIS.item.pszText=szBuffer;
		TVIS.item.iImage=I_IMAGECALLBACK;
		TVIS.item.iSelectedImage=I_IMAGECALLBACK;
		TVIS.item.lParam=NULL;
		HTREEITEM hParent=InsertItem(&TVIS);
		ASSERT(GetLastError()==ERROR_SUCCESS);

		CSurfaceData * pSurface=pDocument_G->GetSurface();
		if (pSurface!=NULL)
		{
			int iCount=pSurface->GetFixedPointCount();
			for (int i=0;i<iCount;i++)
			{
				CFixedPoint *pFixedPoint=pSurface->GetFixedPoint(i);

				TV_INSERTSTRUCT TVIS;
				TVIS.hParent=hParent;
				TVIS.hInsertAfter=TVI_LAST;
				TVIS.item.mask=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
				TVIS.item.state=0;
				TVIS.item.stateMask=0;
				TVIS.item.cchTextMax=MAX_FOLDER_NAME_LENGTH-1;
				lstrcpyn(szBuffer,pFixedPoint->GetName(),_countof(szBuffer)-1);
				TVIS.item.pszText=szBuffer;
				TVIS.item.iImage=I_IMAGECALLBACK;
				TVIS.item.iSelectedImage=I_IMAGECALLBACK;
				TVIS.item.lParam=(LONG)((CObject *)pFixedPoint);
				InsertItem(&TVIS);
				TRACE(_T("Fixed point param=%li\n"),TVIS.item.lParam);
#ifdef _DEBUG
				CObject * pObject=(CObject *)(TVIS.item.lParam);
				ASSERT(dynamic_cast<CSurfaceData *>(pObject)==NULL);
				ASSERT(dynamic_cast<CFixedPoint *>(pObject)!=NULL);
				ASSERT(dynamic_cast<CFolder *>(pObject)==NULL);
#endif
			}
		}
		Expand(hParent,m_bFixedPointsFolderIsOpen?TVE_EXPAND:TVE_COLLAPSE);	//expand here since earlier it had no items.
		ASSERT(GetLastError()==ERROR_SUCCESS);
		
		TVIS.hParent=TVI_ROOT;
		TVIS.hInsertAfter=TVI_LAST;
		LoadString(AfxGetResourceHandle(),IDS_SURFACEFOLDERTITLE,szBuffer,_countof(szBuffer)-1);	//the TI_ITEM won't take CStrings
		TVIS.item.mask=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
		TVIS.item.state=0;
		TVIS.item.stateMask=0;
		TVIS.item.cchTextMax=MAX_FOLDER_NAME_LENGTH-1;
		TVIS.item.pszText=szBuffer;
		TVIS.item.iImage=I_IMAGECALLBACK;
		TVIS.item.iSelectedImage=I_IMAGECALLBACK;
		TVIS.item.lParam=(LONG)((CObject *)pSurface);
		hParent=InsertItem(&TVIS);
		ASSERT(GetLastError()==ERROR_SUCCESS);
#ifdef _DEBUG
		CObject * pObject=(CObject *)(TVIS.item.lParam);
		ASSERT((dynamic_cast<CSurfaceData *>(pObject))!=NULL);
		ASSERT((dynamic_cast<CFixedPoint *>(pObject))==NULL);
		ASSERT((dynamic_cast<CFolder *>(pObject))==NULL);
#endif
	}
}

BEGIN_MESSAGE_MAP(COnStationSurveyTree, CTreeCtrl)
	//{{AFX_MSG_MAP(COnStationSurveyTree)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, OnEndlabeledit)
	ON_NOTIFY_REFLECT(TVN_GETDISPINFO, OnGetdispinfo)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_NOTIFY_REFLECT(TVN_BEGINLABELEDIT, OnBeginlabeledit)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED,OnSelChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//Fills a CTreeCtrl with the current heirarchy of folders.
//Uses query criterion to filter out unacceptable data.
void COnStationSurveyTree::RecursiveTreeViewFill(CFolder *folder,HTREEITEM hParent,CLegQuery * pLegQuery,HTREEITEM insertAfter)
{
	TCHAR szBuffer[MAX_FOLDER_NAME_LENGTH];

	TV_INSERTSTRUCT TVIS;
	TVIS.hParent=hParent;
	TVIS.hInsertAfter=insertAfter;

	if (hParent==TVI_ROOT && pLegQuery!=NULL)
	{
		LoadString(AfxGetResourceHandle(),IDS_QUERYRESULTS,szBuffer,_countof(szBuffer)-1);
	}
	else
	{
		lstrcpyn(szBuffer,folder->GetName(),MAX_FOLDER_NAME_LENGTH-1);
	}
	TVIS.item.mask=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	TVIS.item.state=0;
	TVIS.item.stateMask=0;
	TVIS.item.cchTextMax=MAX_FOLDER_NAME_LENGTH-1;
	TVIS.item.pszText=szBuffer;
	TVIS.item.iImage=I_IMAGECALLBACK;
	TVIS.item.iSelectedImage=I_IMAGECALLBACK;
	TVIS.item.lParam=(LONG)((CObject *)folder);
	TRACE(_T("Adding %s\n"),szBuffer);
	hParent=InsertItem(&TVIS);
	ASSERT(GetLastError()==ERROR_SUCCESS);
	folder->m_hTempTreeItem=hParent;
	if (folder->IsFolder())
	{
		CRealFolder *RF=(CRealFolder *)folder;
		RF->GotoTop();
		CFolder *data=RF->GetCurrent();
		while (data!=NULL)
		{
			if (data->IsFolder() && (pLegQuery==NULL || data->DoesAnythingMatchTheQuery(pLegQuery)))
			{
				RecursiveTreeViewFill(data,hParent,pLegQuery);
			}
			if (!data->IsFolder() && (pLegQuery==NULL || pLegQuery->MatchesQuery((CSurveyLeg *)data)))
			{
				RecursiveTreeViewFill(data,hParent,pLegQuery);
			}
			data=RF->GetNext();
		}
		Expand(hParent,pLegQuery!=NULL || (RF->IsExpanded())?TVE_EXPAND:TVE_COLLAPSE);	//expand here since earlier it had no items.
		ASSERT(GetLastError()==ERROR_SUCCESS);
	}
}


CFixedPoint * COnStationSurveyTree::GetSelectedFixedPoint()
{
	CFixedPoint *pFixedPoint=NULL;
	TV_ITEM Item;
	
	HTREEITEM hItem=GetSelectedItem();
	if (hItem!=NULL)
	{
		Item.hItem=hItem;
		Item.mask=TVIF_HANDLE|TVIF_PARAM;
		GetItem(&Item);
		CObject * pObject=(CObject *)(Item.lParam);
		pFixedPoint=dynamic_cast<CFixedPoint *>(pObject);
	}
	return pFixedPoint;
}

CSurfaceData * COnStationSurveyTree::GetSelectedSurface()
{
	CSurfaceData *pSurface=NULL;
	TV_ITEM Item;
	
	HTREEITEM hItem=GetSelectedItem();
	if (hItem!=NULL)
	{
		Item.hItem=hItem;
		Item.mask=TVIF_HANDLE|TVIF_PARAM;
		GetItem(&Item);
		CObject * pObject=(CObject *)(Item.lParam);
		pSurface=dynamic_cast<CSurfaceData *>(pObject);
	}
	return pSurface;
}

CRealFolder * COnStationSurveyTree::GetSelectedRealFolder()
{
	CRealFolder *folder=NULL;
	TV_ITEM Item;
	
	HTREEITEM hItem=GetSelectedItem();
	if (hItem!=NULL)
	{
		Item.hItem=hItem;
		Item.mask=TVIF_HANDLE|TVIF_PARAM;
		GetItem(&Item);
		CObject * pObject=(CObject *)(Item.lParam);
		folder=dynamic_cast<CRealFolder *>(pObject);
	}
	return folder;
}
CSurveyLeg * COnStationSurveyTree::GetSelectedSurveyLeg()
{
	CSurveyLeg *pLeg=NULL;
	TV_ITEM Item;
	
	HTREEITEM hItem=GetSelectedItem();
	if (hItem!=NULL)
	{
		Item.hItem=hItem;
		Item.mask=TVIF_HANDLE|TVIF_PARAM;
		GetItem(&Item);
		CObject * pObject=(CObject *)(Item.lParam);
		pLeg=dynamic_cast<CSurveyLeg *>(pObject);
	}
	return pLeg;
}

void COnStationSurveyTree::OnDelete()
{
	BOOL bRecalculateGeometry=TRUE;
	switch (GetSelectedType())
	{
	case ST_NONE:
		AfxMessageBox(IDS_NOSURVEYORFOLDERSELECTED);
		break;
	case ST_REALFOLDER:
		{
			CString CS;
			CRealFolder  * pFolder=GetSelectedRealFolder();
			CS.FormatMessage(IDS_DELETEWARNING,pFolder->GetName());
			if (pFolder->GetSize()!=0 && AfxMessageBox(CS,MB_YESNO)!=IDYES)
			{
				return;
			}
			bRecalculateGeometry=pFolder->GetSize()!=0;
			if (pFolder!=pDocument_G->GetSurveyFolder())
			{
			    pFolder->GetParentFolder()->Remove(pFolder);
				delete pFolder;
			}
			else
			{
				pFolder->DeleteContents();
				CString CS;
				CS.LoadString(IDS_SURVEYROOT);
				pFolder->SetName(CS);
			}
			UpdateItems();
		}
		break;
	case ST_SURVEY:
		{
			CSurveyLeg * pLeg=GetSelectedSurveyLeg();
			pLeg->DeleteContents();
			DeleteItem(pLeg->m_hTempTreeItem);
			pLeg->GetParentFolder()->Remove(pLeg);
			delete pLeg;
		}
		break;
	case ST_FIXEDPOINT:
		{
			CFixedPoint * pFixedPoint=GetSelectedFixedPoint();
			pDocument_G->GetSurface()->RemoveFixedPoint(pFixedPoint);
			DeleteItem(GetSelectedItem());
			delete pFixedPoint;
		}
		break;
	case ST_SURFACE:
		{
			CSurfaceData * pSurface=GetSelectedSurface();
			if (AfxMessageBox(IDS_RESETALLSURFACEDATA,MB_YESNO)==IDYES)
			{
				pSurface->ResetAll();
			}
			else
			{
				return;
			}
		}
		break;
	case ST_FIXEDPOINTFOLDER:
		if (AfxMessageBox(IDS_REMOVEALLFIXEDPOINTS,MB_YESNO)==IDYES)
		{
			pDocument_G->GetSurface()->RemoveAllFixedPoints();
			UpdateItems();
		}
		else
		{
			return;
		}
		break;
	}
	if (bRecalculateGeometry)
	{
		pView_G->RecalculateGeometry(TRUE);
	}
}

void COnStationSurveyTree::OnSelChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	//TVN_SELCHANGED
	switch (GetSelectedType())
	{
	case ST_SURVEY:
	case ST_REALFOLDER:
	case ST_SURFACE:
		m_pOverallDataView->EnableColorWidget(TRUE);
		break;
	default:
		m_pOverallDataView->EnableColorWidget(FALSE);
		break;
	}
	*pResult=0;
}


void COnStationSurveyTree::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	CObject *pObject=(CObject*)(pNMTreeView->itemNew.lParam);
	CFolder * pFolder=dynamic_cast<CFolder *>(pObject);

	if (pFolder!=NULL)
	{
		HCURSOR hCursor=LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_DRAGDROP));
		SetCursor(hCursor);
		//We don't get mouse messages for some reason, so
		//we capture to the parent window instead
		GetParent()->SetCapture();
		m_bDragging=TRUE;
		VERIFY(SetTimer(5,75,TimerProc)!=0);
		m_hDragItem=pNMTreeView->itemNew.hItem;
		*pResult = 0;
	}
	else
	{
		//Don't support dragging of fixed points
		*pResult=1;
	}
}								  

void COnStationSurveyTree::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	//Check for cancelation
	if (pTVDispInfo->item.pszText!=NULL)
	{
		// TODO: Add your control notification handler code here
		CObject *pObject=(CObject*)pTVDispInfo->item.lParam;
		CFolder * pFolder=dynamic_cast<CFolder *>(pObject);
		if (pFolder!=NULL)
		{
			BOOL res = SetItemText(pTVDispInfo->item.hItem,pTVDispInfo->item.pszText);
			pFolder->SetName(pTVDispInfo->item.pszText);
		}
	}
	*pResult = 0;
}

void COnStationSurveyTree::OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* Disp = (TV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	CObject * pData=(CObject *)Disp->item.lParam;

	CFolder *folder=dynamic_cast<CFolder*>(pData);

	//START OF SECTION ASSIGNING FOLDER IMAGE.  These images are
	//based on the color
	if (folder!=NULL)
	{
		//A real folder is either open or closed
		if (folder->IsFolder())
		{
			if ((Disp->item.state&TVIS_EXPANDED)!=0)
			{
				Disp->item.iImage=m_idxFolder;
				Disp->item.iSelectedImage=m_idxFolder;
				((CRealFolder *)folder)->SetExpanded(TRUE);
			}
			else
			{
				Disp->item.iImage=m_idxFolder+1;
				Disp->item.iSelectedImage=m_idxFolder+1;
				((CRealFolder *)folder)->SetExpanded(FALSE);
			}
		}
		else
		{
			//A survey folder has a color based on the color scheme
			if (m_iColorScheme>=0)
			{
				//Convert the color to a folder index
				COLORREF crColor=folder->GetDrawColor(m_iColorScheme);
				//First check to see if the survey is turned off or hidden.
				if (crColor==COLOR_INACTIVE)
				{
					Disp->item.iImage=m_idxSurvey+16;
					Disp->item.iSelectedImage=m_idxSurvey+16;
					*pResult=0;
					return;
				}
				if (crColor==COLOR_INVISIBLE)
				{
					Disp->item.iImage=m_idxSurvey+15;
					Disp->item.iSelectedImage=m_idxSurvey+15;
					*pResult=0;
					return;
				}
				//Next check to see if we have a depth, age or 3d
				//scheme since that overrides the normal colors
				if (m_iColorScheme==COLORSCHEME_DEPTH)
				{
					Disp->item.iImage=m_idxSurvey+17;
					Disp->item.iSelectedImage=m_idxSurvey+17;
					*pResult=0;
					return;
				}
				if (m_iColorScheme==COLORSCHEME_BLUNDER)
				{
					Disp->item.iImage=m_idxSurvey+18;
					Disp->item.iSelectedImage=m_idxSurvey+18;
					*pResult=0;
					return;
				}
				//We have 15 colored folder bitmaps corresponding to the items in
				for (int i=0;i<15;i++)
				{
					if (CGlobalColorManager::StandardVGAColors[i]==crColor)
					{
						Disp->item.iImage=m_idxSurvey+i+((m_iColorScheme==COLORSCHEME_AGE)?19:0);
						Disp->item.iSelectedImage=m_idxSurvey+i+((m_iColorScheme==COLORSCHEME_AGE)?19:0);
					}
				}
			}
			else
			{
				Disp->item.iImage=m_idxSurvey+17;
				Disp->item.iSelectedImage=m_idxSurvey+17;
			}
		}
	}
	else
	{
		//The selected item is not a folder.  It must be a fixed point or
		//the surface data.  The fixed point root has NULL for it's item data.
		if (pData==NULL)
		{
			//Item data is NULL, so we are the fixed point root.  Show whether this
			//is open or closed.
			if ((Disp->item.state&TVIS_EXPANDED)!=0)
			{
				Disp->item.iImage=m_idxPinRoot;
				Disp->item.iSelectedImage=m_idxPinRoot;
				m_bFixedPointsFolderIsOpen=FALSE;
			}
			else
			{
				Disp->item.iImage=m_idxPinRoot+1;
				Disp->item.iSelectedImage=m_idxPinRoot+1;
				m_bFixedPointsFolderIsOpen=TRUE;
			}
		}
		else
		{
			//See if this thing is surface data instead.  We use
			//the dynamic cast macro to convert this class.
			CSurfaceData * pSurfaceData=dynamic_cast<CSurfaceData *>(pData);
			TRACE(_T("Trying to test point param=%li\n"),Disp->item.lParam);
			if (pSurfaceData==NULL)
			{
				CFixedPoint * pFixedPoint=dynamic_cast<CFixedPoint *>(pData);
				BOOL bSuspectPin=TRUE;
				if (pFixedPoint!=NULL)
				{
					bSuspectPin=pDocument_G->GetNodeTree()->FindNode(pFixedPoint->GetName(),FALSE)==NULL;
				}
				Disp->item.iImage=m_idxPin+(bSuspectPin?1:0);
				Disp->item.iSelectedImage=m_idxPin+(bSuspectPin?1:0);
			}
			else
			{
				Disp->item.iImage=m_idxSurfaceFolder;
				Disp->item.iSelectedImage=m_idxSurfaceFolder;
			}
		}
	}
	*pResult = 0;
}

void COnStationSurveyTree::OnMouseMove(UINT nFlags, CPoint pt) 
{
	ScreenToClient(&pt);	//We got this from our parent window
	POINT point=pt;
	if (m_bDragging)
	{
		UINT flags;

		flags=TVHT_ONITEM;
		
		HTREEITEM hTarget=HitTest(point,&flags);
		if (hTarget!=NULL)
		{
			TV_ITEM Item;
			
			Item.hItem=hTarget;
			Item.mask=TVIF_HANDLE|TVIF_PARAM;
			GetItem(&Item);
			CObject * pObject=(CObject *)(Item.lParam);
			CFolder * pFolder;
			pFolder=dynamic_cast<CFolder *>(pObject);
			if (pFolder!=NULL)
			{
				if (!pFolder->IsFolder())	//Only drop onto a folder.
				{
					pFolder=pFolder->GetParentFolder();
				}
				SetCursor(LoadCursor(AfxGetResourceHandle(),MAKEINTRESOURCE(IDC_DRAGDROP)));
				SelectDropTarget((HTREEITEM)pFolder->m_hTempTreeItem);
				return;
			}
		}
		
		//You can't drag and drop to here
		SetCursor(LoadCursor(AfxGetResourceHandle(),MAKEINTRESOURCE(IDC_DRAGDROPBAD)));
	}
}


void COnStationSurveyTree::OnLButtonUp(UINT nFlags, CPoint pt) 
{
	VERIFY(KillTimer(5)!=0);
	ScreenToClient(&pt);	//We got this from our parent window
	POINT point=pt;
	if (m_bDragging)
	{
		HTREEITEM hTarget;
		TV_ITEM tvTarget;
		TV_ITEM tvSource;

		SetCursor(LoadCursor(NULL,IDC_ARROW));
		hTarget=HitTest(point,&nFlags);
		CFolder * pRealHit=NULL;
		if (hTarget!=NULL)
		{
			tvTarget.hItem=hTarget;
			tvTarget.mask=TVIF_HANDLE|TVIF_PARAM;
			GetItem(&tvTarget);
			tvSource.hItem=m_hDragItem;
			tvSource.mask=TVIF_HANDLE|TVIF_PARAM;
			GetItem(&tvSource);
			//Set everything here
			CObject *targetObj=(CObject *)tvTarget.lParam;
			CObject *sourceObj=(CObject *)tvSource.lParam;
			CFolder * target=dynamic_cast<CFolder *>(targetObj);
			CFolder * source=dynamic_cast<CFolder *>(sourceObj);
			if (target==NULL || source==NULL)
			{
				AfxMessageBox(IDS_ONLYDRAGSURVEYSANDFOLDER);
				return;
			}
			if (!target->IsFolder())	//Only drop onto a folder.
			{
				pRealHit=target;
				target=target->GetParentFolder();
			}
			if (target->IsDropTarget(source))
			{
			 	if (source!=pRealHit)
				{
					source->SetParentFolder((CRealFolder *)target,pRealHit);
					DeleteItem(source->m_hTempTreeItem);
					Select(target->m_hTempTreeItem,TVGN_CARET);	//Select it
					AddNewOne(source,FALSE,pRealHit);
				}
			}
//I don't see why this is necessary
//			pView_G->RecalculateGeometry(TRUE);
		}
		SelectDropTarget((HTREEITEM)NULL);
		ReleaseCapture();
		SetCursor(LoadCursor(NULL,IDC_ARROW));
		m_bDragging=FALSE;
	}
}
void COnStationSurveyTree::AddNewOne(CFolder * newone,BOOL bRecalculate,CFolder * pInsertAfter)
{
	CRealFolder *pFolder=GetSelectedRealFolder();
	if (pFolder==NULL)
	{
		CSurveyLeg * pLeg=GetSelectedSurveyLeg();
		if (pLeg!=NULL)
		{
			pInsertAfter=pLeg;
			pFolder=pLeg->GetParentFolder();
		}
	}
	if (pFolder==NULL)
	{
		pFolder=pDocument_G->GetSurveyFolder();
	}
	pDocument_G->SetModifiedFlag();
	newone->SetParentFolder(pFolder,pInsertAfter);
	RecursiveTreeViewFill(newone,pFolder->m_hTempTreeItem,m_pLegQuery,pInsertAfter?pInsertAfter->m_hTempTreeItem:TVI_LAST);
	Expand(pFolder->m_hTempTreeItem,TVE_EXPAND);	//open the folder if appropriate
	EnsureVisible(newone->m_hTempTreeItem);
	Select(newone->m_hTempTreeItem,TVGN_CARET);	//Select it
	if (bRecalculate)
	{
		pView_G->RecalculateGeometry(TRUE);
	}
}

void COnStationSurveyTree::OnNewfolder()
{
	CNewFolderDialog nfDlg(NULL);
	if (nfDlg.DoModal()==IDOK)
	{
		CRealFolder *RF=new CRealFolder(pDocument_G);
		RF->SetName(nfDlg.GetName());
		AddNewOne(RF,FALSE,NULL);
	}
}

void COnStationSurveyTree::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//Delay processing.  Otherwise we crash on exit becuause the code
	//on the other side of this expects the control to be unchanged.
	//Ie: the tree view called us and then we reset everything before
	//returning hence KABOOOM!!!
	AfxGetMainWnd()->PostMessage(WM_COMMAND,ID_EDIT_EDIT,0l);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void COnStationSurveyTree::OnUpdateEditCopy(CCmdUI * pCmdUI)
{
	switch (GetSelectedType())
	{
	case ST_NONE:
		pCmdUI->Enable(FALSE);
		break;
	default:
		pCmdUI->Enable(TRUE);
		break;
	}
}

void COnStationSurveyTree::OnUpdateEditDelete(CCmdUI * pCmdUI)
{
	switch (GetSelectedType())
	{
	case ST_NONE:
		pCmdUI->Enable(FALSE);
		break;
	default:
		pCmdUI->Enable(TRUE);
		break;
	}
}

void COnStationSurveyTree::OnUpdateEditEdit(CCmdUI * pCmdUI)
{
	switch (GetSelectedType())
	{
	case ST_NONE:
	case ST_FIXEDPOINTFOLDER:
	case ST_REALFOLDER:
		pCmdUI->Enable(FALSE);
		break;
	default:
		pCmdUI->Enable(TRUE);
		break;
	}
}

void COnStationSurveyTree::OnEditSelectedItem()
{
	switch (GetSelectedType())
	{
	case ST_NONE:
		AfxMessageBox(IDS_NOSURVEYORFOLDERSELECTED);
		break;
	case ST_REALFOLDER:
		{

		}
		break;
	case ST_SURVEY:
		{
			CSurveyLeg * pLeg=GetSelectedSurveyLeg();
			ASSERT(AfxCheckMemory());
	   	   	CEditSheet editdata(pLeg,IDS_SURVEYEDITSHEETTITLE,this,1);
		   	if (editdata.DoModal()==IDOK)
			{
//				ASSERT(AfxCheckMemory());
				if (editdata.NeedRecalculation())
				{
					pView_G->RecalculateGeometry(TRUE);
				}
				SetNewName(pLeg->m_hTempTreeItem,pLeg->GetName());
				pDocument_G->SetModifiedFlag();
			}
		}
		break;
	case ST_FIXEDPOINT:
		{
			CFixedPoint * pFixedPoint=GetSelectedFixedPoint();
			CEditFixedPointDialog dlgFixedPointEditor(pDocument_G,pFixedPoint);
			if (dlgFixedPointEditor.DoModal()==IDOK)
			{
				pView_G->RecalculateGeometry(TRUE);
				CSurfaceData * surf=pView_G->GetDocument()->GetSurface();
				if (surf!=NULL)
				{
					pView_G->SetExactView(NAN,NAN,NAN,surf->ConvertXToLocal(pFixedPoint->GetX()),surf->ConvertYToLocal(pFixedPoint->GetY()),pFixedPoint->GetZ());
				}
				UpdateItems();
				pDocument_G->SetModifiedFlag();
			}
			ASSERT(AfxCheckMemory());
		}
		break;
	case ST_SURFACE:
		{
			CSurfacePropertySheet CSPS(pDocument_G,IDS_SURFACEPROPERTYSHEETTITLE);
			if (CSPS.DoModal()==IDOK && CSPS.NeedRecalculation())
			{
				pView_G->RecalculateGeometry(TRUE);
				pDocument_G->SetModifiedFlag();
			}

			ASSERT(AfxCheckMemory());
		}
		break;
	case ST_FIXEDPOINTFOLDER:
		//This must be the root of the fixed point list.
		HTREEITEM hItem=GetSelectedItem();
		Expand(hItem,TVE_TOGGLE);
		pDocument_G->SetModifiedFlag();
		break;
	}
}

void COnStationSurveyTree::SetNewName(HTREEITEM hItem,LPCTSTR szName)
{
	TV_ITEM Item;
	Item.hItem=hItem;
	Item.mask=TVIF_TEXT;
	TCHAR szBuffer[64];
	lstrcpyn(szBuffer,szName,MAX_FOLDER_NAME_LENGTH-1);
	Item.cchTextMax=MAX_FOLDER_NAME_LENGTH-1;
	Item.pszText=szBuffer;
  	SetItem(&Item);
}



void COnStationSurveyTree::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CMenu Menu;
	CPoint point;
	GetCursorPos(&point);
	VERIFY(Menu.LoadMenu(IDR_SURVEYTREEMENU));
	CMenu* pPopup = Menu.GetSubMenu(0);
	switch (GetSelectedType())
	{
	case ST_NONE:
		pPopup->EnableMenuItem(ID_SURVEYTREE_DELETECURRENTITEM,MF_GRAYED|MF_DISABLED);
		//deliberately fall through
	case ST_FIXEDPOINTFOLDER:
	case ST_REALFOLDER:
		pPopup->EnableMenuItem(ID_SURVEYTREE_EDITCURRENTITEM,MF_GRAYED|MF_DISABLED);
		break;
	}
	//We popup the menu as if it belongs to the parent since this control
	//does not handle WM_COMMAND messages properly
	if (pPopup!=NULL)
	{
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x,point.y,GetParent());
	}
	Menu.DestroyMenu();
}

void COnStationSurveyTree::OnCut()
{
	OnCopy();
	OnDelete();
}

void COnStationSurveyTree::OnCopy()
{
	CWaitCursor w;
	try
	{
		SURVEYTREESELECTED st=GetSelectedType();
		if (st==ST_REALFOLDER || st==ST_SURVEY)
		{
			CBufferFileHelper BF;
			CRealFolder *pFolder=GetSelectedRealFolder();
			if (pFolder!=NULL)
			{
				pFolder->Write(&BF);
			}
			CSurveyLeg * pLeg=GetSelectedSurveyLeg();
			if (pLeg!=NULL)
			{
				pLeg->Write(&BF);
			}
			CString CS;
			CS=BF.GetTextStoring();
			CClipboard::StringToClipboard(CS,TRUE);
		}
	}
	catch (CMemoryException*)
	{
		AfxMessageBox(IDS_GENERALMEMORYERROR);
	}
}

void COnStationSurveyTree::OnPaste()
{
	CWaitCursor w;
	CString string;
	if (!CClipboard::StringFromClipboard(string,TRUE))
	{
		AfxMessageBox(IDS_NOFORMATONCLIPBOARD);
		return;
	}
	try
	{
		CBufferFileHelper BF(string);
		LPCTSTR szData=BF.ReadTokenString(_T("Begin"));
		if (lstrcmpi(szData, _T("Folder"))==0)
		{
			try
			{
				BF.UndoLastRead();
				CRealFolder *RF=new CRealFolder(pDocument_G);
				RF->Read(&BF);
				AddNewOne(RF,FALSE,NULL);
			}
			catch (...)
			{
				AfxMessageBox(IDS_NOFORMATONCLIPBOARD);
			}
		} else if (lstrcmpi(szData, _T("Survey"))==0)
		{
			try
			{
				BF.UndoLastRead();
				CSurveyLeg *Leg=new CSurveyLeg(pDocument_G);
				Leg->Read(&BF);
				Leg->GetShotArray()->DoDeferredOffsetCalculation();
				AddNewOne(Leg,FALSE,NULL);
			}
			catch (...)
			{
				AfxMessageBox(IDS_NOFORMATONCLIPBOARD);
			}
		}
		else
		{
			AfxMessageBox(IDS_NOFORMATONCLIPBOARD);
		}
		pView_G->RecalculateGeometry(TRUE);
	}
	catch (...)
	{
		AfxMessageBox(IDS_NOFORMATONCLIPBOARD);
	}
}

void COnStationSurveyTree::OnNewsurvey()
{
    CSurveyLeg *leg=new CSurveyLeg(pDocument_G);

   	CEditSheet editdata(leg,IDS_SURVEYEDITSHEETTITLE,this,0);
   	if (editdata.DoModal()==IDOK)
   	{
		AddNewOne(leg,TRUE,NULL);
	}
	else
	{
		delete leg;
	}
}


void COnStationSurveyTree::OnNewFixedPoint()
{
	CFixedPoint *pFixedPoint=new CFixedPoint(_T(""));
	CEditFixedPointDialog dlgFixedPointEditor(pDocument_G,pFixedPoint);
	if (dlgFixedPointEditor.DoModal()==IDOK)
	{
		if (pDocument_G->GetSurface()->AddFixedPoint(pFixedPoint))
		{
			UpdateItems();
		}
		else
		{
			AfxMessageBox(IDS_DUPLICATEFIXEDPOINT);
		}
		pView_G->RecalculateGeometry(TRUE);
		//Center on the first constrained point where appropriate
		//This stops us from being way the hell over in some unknown
		//location if there is a surface in play or something.
		CSurfaceData * surf=pView_G->GetDocument()->GetSurface();
		if (surf!=NULL)
		{
			pView_G->SetExactView(NAN,NAN,NAN,surf->ConvertXToLocal(pFixedPoint->GetX()),surf->ConvertYToLocal(pFixedPoint->GetY()),pFixedPoint->GetZ());
		}
	}
}


//Only allow label editing for the folders and surveys, not for fixed points or surface
void COnStationSurveyTree::OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here

	CObject *pObject=(CObject*)pTVDispInfo->item.lParam;
	CFolder * pFolder=dynamic_cast<CFolder *>(pObject);
	if (pFolder!=NULL)
	{
		*pResult = 0;
	}
	else
	{
		*pResult=1;
	}
}


SURVEYTREESELECTED COnStationSurveyTree::GetSelectedType()
{
	HTREEITEM hItem=GetSelectedItem();
	if (hItem==NULL)
	{
		return ST_NONE;
	}
	TV_ITEM Item;
	Item.hItem=hItem;
	Item.mask=TVIF_HANDLE|TVIF_PARAM;
	VERIFY(GetItem(&Item));
	//This is the only thing with a NULL lParam
	if (Item.lParam==NULL)
	{
		return ST_FIXEDPOINTFOLDER;
	}
	CObject * pObject=(CObject *)(Item.lParam);
	CRealFolder * pFolder=dynamic_cast<CRealFolder *>(pObject);
	if (pFolder!=NULL)
	{
		return ST_REALFOLDER;
	}
	CSurveyLeg * pLeg=dynamic_cast<CSurveyLeg *>(pObject);
	if (pLeg!=NULL)
	{
		return ST_SURVEY;
	}
	CSurfaceData * pSurfacedata=dynamic_cast<CSurfaceData *>(pObject);
	if (pSurfacedata!=NULL)
	{
		return ST_SURFACE;
	}
	CFixedPoint * pFixedPoint=dynamic_cast<CFixedPoint *>(pObject);
	if (pFixedPoint!=NULL)
	{
		return ST_FIXEDPOINT;
	}
	//We have already check all possible cases.  This is
	//obviously some sort of error.
	ASSERT(FALSE);
	return ST_FIXEDPOINTFOLDER;
}


void COnStationSurveyTree::OnShowProperties()
{
	CFolder *folder=GetSelectedRealFolder();
	if (folder==NULL)
	{
		folder=GetSelectedSurveyLeg();
	}
	if (folder==NULL)
	{
		//If the surface or a fixed point is selected, just show the overall data for now.
		folder=pDocument_G->GetSurveyFolder();
	}
	CFolderData FD;
	folder->GatherFolderData(&FD,m_iColorScheme,FALSE,m_pLegQuery);
	CFolderInfoDlg FI(&FD,folder->GetName());
	FI.DoModal();
}


void COnStationSurveyTree::FuckYouMicrosoftMorons()
{
	CPoint pt;
	GetCursorPos(&pt);
	CRect r;
	GetWindowRect(&r);
	HTREEITEM hTreeItem=GetFirstVisibleItem();
	if (r.top>pt.y)
	{
		hTreeItem=GetPrevVisibleItem(hTreeItem);
		if (hTreeItem!=NULL)
		{
			SelectSetFirstVisible(hTreeItem);
		}
	}
	if (r.bottom<pt.y)
	{
		hTreeItem=GetNextVisibleItem(hTreeItem);
		if (hTreeItem!=NULL)
		{
			SelectSetFirstVisible(hTreeItem);
		}
		
	}
}

