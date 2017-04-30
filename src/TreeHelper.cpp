#include "stdafx.h"
#include "resource.h"
#include "folder.h"
#include "realfolder.h"
#include "colortool.h"
#include "legquery.h"

static CImageList ImageList;
static int idxFolder;
static int idxSurvey;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void InitializeTreeImageList()
{
	ImageList.Create(16,15,FALSE,20,5);	//allow up to 5 images if it needs to grow (Drag maybe)
	CBitmap hBmp;
	CBitmap hBmp2;
	hBmp2.LoadBitmap(IDB_BMSURVEYS);
	idxSurvey=ImageList.Add(&hBmp2,(COLORREF)0);
	hBmp.LoadBitmap(IDB_BMFOLDERS);
	idxFolder=ImageList.Add(&hBmp,(COLORREF)0);
}

void TreeHelperSetImageList(CTreeCtrl *TV)
{
	TV->SetImageList(&ImageList,TVSIL_NORMAL);	
}
//Fills a CTreeCtrl with the current heirarchy of folders.
void RecursiveTreeViewFill(CTreeCtrl *TV,CFolder *folder,HTREEITEM hParent,CLegQuery * pLegQuery)
{
	TCHAR szBuffer[MAX_FOLDER_NAME_LENGTH];

	TV_INSERTSTRUCT TVIS;
	TVIS.hParent=hParent;
	TVIS.hInsertAfter=TVI_LAST;

	lstrcpyn(szBuffer,folder->GetName(),MAX_FOLDER_NAME_LENGTH);
	TVIS.item.mask=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	TVIS.item.state=0;
	TVIS.item.stateMask=0;
	TVIS.item.cchTextMax=MAX_FOLDER_NAME_LENGTH-1;
	TVIS.item.pszText=szBuffer;
	TVIS.item.iImage=I_IMAGECALLBACK;
	TVIS.item.iSelectedImage=I_IMAGECALLBACK;
	TVIS.item.lParam=(LONG)folder;

	hParent=TV->InsertItem(&TVIS);
	folder->m_hTempTreeItem=hParent;
	if (folder->IsFolder())
	{
		CRealFolder *RF=(CRealFolder *)folder;
		RF->GotoTop();
		CFolder *data=RF->GetCurrent();
		while (data!=NULL)
		{
			//Do folders first
			if (data->IsFolder())
			{
				RecursiveTreeViewFill(TV,data,hParent,pLegQuery);
			}
			data=RF->GetNext();
		}

		RF->GotoTop();
		data=RF->GetCurrent();
		while (data!=NULL)
		{
			if (!data->IsFolder() && (pLegQuery==NULL || pLegQuery->MatchesQuery((CSurveyLeg *)data)))	//Next do the parents.
			{
				RecursiveTreeViewFill(TV,data,hParent,pLegQuery);
			}
			data=RF->GetNext();
		}
		TV->Expand(hParent,RF->IsExpanded()?TVE_EXPAND:TVE_COLLAPSE);	//expand here since earlier it had no items.
	}
}


void TreeHelperGetDisplayInfo(TV_DISPINFO *Disp,int iColorScheme)
{
	CFolder *folder=(CFolder*)Disp->item.lParam;
	if (folder->IsFolder())
	{
		if ((Disp->item.state&TVIS_EXPANDED)!=0)
		{
			Disp->item.iImage=idxFolder;
			Disp->item.iSelectedImage=idxFolder;
			((CRealFolder *)folder)->SetExpanded(TRUE);
		}
		else
		{
			Disp->item.iImage=idxFolder+1;
			Disp->item.iSelectedImage=idxFolder+1;
			((CRealFolder *)folder)->SetExpanded(FALSE);
		}
	}
	else
	{
		if (iColorScheme!=-1)
		{
			//Todo: convert the color to a folder index
			COLORREF crColor=folder->GetDrawColor(iColorScheme);
			//We have 16 colored folder bitmaps corresponding to the items in
			for (int i=0;i<16;i++)
			{
				if (CGlobalColorManager::StandardVGAColors[i]==crColor)
				{
					Disp->item.iImage=i;
					Disp->item.iSelectedImage=i;
				}
			}
		}
		else
		{
			Disp->item.iImage=0;
			Disp->item.iSelectedImage=0;
		}
	}
}
