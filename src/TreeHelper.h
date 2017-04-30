#ifndef I_TREEHELPER__
#define I_TREEHELPER__

class CFolder;
class CLegQuery;


void InitializeTreeImageList();
void RecursiveTreeViewFill(CTreeCtrl *TV,CFolder *folder,HTREEITEM hParent,CLegQuery * pLegQuery);
void TreeHelperGetDisplayInfo(TV_DISPINFO *,int iColorScheme);
void TreeHelperSetImageList(CTreeCtrl *TV);

#endif
