//
#ifndef I_FOLDER__
#define I_FOLDER__
#include "namelist.h"

class CRealFolder;
class CFileDumpHelper;
class COnStationDoc;
class CFolder;
class CQuery;
class CLegQuery;
//This class is used to gather information on the contents of a folder
class CFolderData:public CObject
{
	public:
		CFolderData();
		~CFolderData();

		int iTotalShots;
		int iNumSurveys;
		int iNumSurfaceShots;
		int iNumCaveShots;
		int iNumSplayShots;
		int iNumExcludedShots;
		int iNumLengthExcludedShots;
		float fTotalShotLengthSurface;
		float fTotalShotLengthCave;
		CNameList m_NameList;
};

class CFolder:public CObject //"public CObject (double derivation in realfolder causes problems with this) 
{
	private:
		TCHAR m_szName[MAX_FOLDER_NAME_LENGTH];
		CRealFolder *m_ParentFolder;
		COnStationDoc *m_MyDocument;
	public:
		CFolder(COnStationDoc *doc);
		virtual	~CFolder();
		HTREEITEM m_hTempTreeItem;		//used to keep track of where this item is in the image list
	
		virtual BOOL IsColorSchemeDataSimilar(int First,int Second)=0;
		virtual BOOL HasDisabledLegs(int iColorScheme)=0;
		COnStationDoc * GetDocument() {return m_MyDocument;}		
		//Recursive dumpguts type of things
		virtual void SaveFormattedText(CFileDumpHelper* PDH,int iColorScheme,BOOL bClosedLoops,BOOL bCoordinatesOnly)=0;

		BOOL IsDropTarget(CFolder *Folder);
		void SetName(LPCTSTR);
		LPCTSTR GetName();
		virtual BOOL IsFolder()=0;
		virtual void DeleteContents()=0;
		virtual COLORREF GetAssignedColor(int iIndex)=0;
		virtual COLORREF GetDrawColor(int iIndex)=0;

		//Perform a query through all the comments in each survey and return an
		//array holding the aggregate result.  Return TRUE if there were matches
		virtual BOOL QuerySurveyComments(CPtrArray * result,CQuery *pTypeQuery,CQuery * pCommentQuery)=0;

		//bDisabled is used to determine if any legs with a color of -2 have
		//been newly created or destroyed.  WE need to know this so that we know
		//wether the whole system needs to be recalculated or not.
		virtual COLORREF SetColor(COLORREF crColor,int iColorSchemeIndex,BOOL bRainbow,BOOL *bDisabled,CLegQuery * pLegQuery)=0;
	    int GetDepth();
	    void SetParentFolder(CRealFolder *,CFolder * pInsertAfter);
	    CRealFolder * GetParentFolder() {return m_ParentFolder;}

		virtual void GatherFolderData(CFolderData * FD,int iColorScheme,BOOL bClosedLoops,CLegQuery * pLegQuery)=0;

		virtual BOOL DoesAnythingMatchTheQuery(CLegQuery* pLegQuery)=0;
};




#endif






