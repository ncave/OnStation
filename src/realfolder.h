#ifndef I_REALFOLDER__
#define I_REALFOLDER__

#include "folder.h"
class CFileHelper;
class CSurveyLeg;
class CQuery;
class CRealFolder:public CFolder
{
    private:
		CList<CFolder *,CFolder *> m_folderList;
    	POSITION m_MyPos;
    	BOOL m_bExpanded;
    public:
#ifdef _DEBUG
		void Dump();
#endif
		BOOL IsExpanded() {return m_bExpanded;}
		virtual BOOL IsColorSchemeDataSimilar(int First,int Second);
		void SetExpanded(BOOL b) {m_bExpanded=b;}
    	CRealFolder(COnStationDoc *);
    	virtual ~CRealFolder();
		void Write(CFileHelper * h);
		void Read(CFileHelper * h);
    	void Add(CFolder *,CFolder * pInsertAfter);
    	void GotoTop();
    	CFolder * GetCurrent();
    	CFolder * GetNext();
    	INT_PTR GetSize();
    	
		void SaveFormattedText(CFileDumpHelper *,int iColorScheme,BOOL bClosedLoops,BOOL bCoordinatesOnly);
    	CSurveyLeg * FindSurveyByName(LPCTSTR szName);
		CRealFolder * FindSubFolderByName(LPCTSTR szName);

		int GetDrawColorIndex(int iIndex) {return 0;}
		COLORREF GetDrawColor(int iIndex) {return RGB(0,0,0);}
		COLORREF GetAssignedColor(int iIndex) {return RGB(0,0,0);}

		COLORREF SetColor(COLORREF crColor,int iColorSchemeIndex,BOOL bRainbow,BOOL *bDisabled,CLegQuery * pLegQuery);	//set children's color

		BOOL HasDisabledLegs(int iColorScheme);
		BOOL DoesAnythingMatchTheQuery(CLegQuery* pLegQuery);
		void DeleteContents();
		BOOL Remove(CFolder *);	//Remove leg but don't delete it
	    virtual BOOL IsFolder() {return TRUE;}

		void GatherFolderData(CFolderData *FD,int iColorScheme,BOOL bClosedLoops,CLegQuery * pLegQuery);

		virtual BOOL QuerySurveyComments(CPtrArray * result,CQuery * pTypeQuery,CQuery * pCommentQuery);
};



#endif


