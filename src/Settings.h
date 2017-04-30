#ifndef I_SETTINGS__
#define I_SETTINGS__



class COnStationDoc;
class CBookMark;
class COnStationView;
class CFileHelper;
class CSettings
{
public:
	CSettings();
	~CSettings();

	CString m_szColorSchemeNames[TOTAL_COLOR_SCHEMES];
	//Hide and disable mean the same thing.  Don't show surface
	COLORREF m_cSurfaceColors[TOTAL_COLOR_SCHEMES];
	BOOL m_bSurfaceSurveys[TOTAL_COLOR_SCHEMES];
	BOOL m_bShowClosureErrors[TOTAL_COLOR_SCHEMES];
	BOOL m_bShowClosedLoops[TOTAL_COLOR_SCHEMES];
	BOOL m_bBoxFixedPoints[TOTAL_COLOR_SCHEMES];
	BOOL m_bShowReferenceGrid[TOTAL_COLOR_SCHEMES];
	STATIONSHOW m_ShowStationNames[TOTAL_COLOR_SCHEMES];
	BOOL m_bPerspective[TOTAL_COLOR_SCHEMES];
	VIEWTYPE m_ViewTypes[TOTAL_COLOR_SCHEMES];

	//Passage walls stuff
	PASSAGE_TYPE	m_PassageType[TOTAL_COLOR_SCHEMES];
	BOOL m_bOpaquePassages[TOTAL_COLOR_SCHEMES];
	BOOL m_bDepthFogging[TOTAL_COLOR_SCHEMES];
	BOOL m_bMultipleLights[TOTAL_COLOR_SCHEMES];
	BOOL m_bRotateNoLighting[TOTAL_COLOR_SCHEMES];
	BOOL m_bRotateNoSolids[TOTAL_COLOR_SCHEMES];

	void ResetContents();	//called on a new document

	COLORREF GetBlunderColor(int iDeviation);
	//Age color stuff
	DWORD m_dwAgeDates[MAX_AGE_COLORS];
	COLORREF m_crAgeColors[MAX_AGE_COLORS];
	int m_iNumAgeColors;
	COLORREF GetColorByAge(DWORD dwAge);
	void SetAgeColors(DWORD dwAgeStart,UINT iNumAgeColors,UINT m_iNumYears);
	void SetDefaultAgeColors();

	COLORREF m_crDepthColors[MAX_DEPTH_COLORS];
	int m_iNumDepthColors;
	float m_fDepthIncrements;
	float m_fStartDepth;
	COLORREF GetDepthColor(int iDepthInterval);
	COLORREF GetDepthColor(float fDepth);

	//Bookmark related stuff.
	//This is an ordered list of the book marks from most recently
	//used to least recently used.
	CObList m_BookMarkList;
	CBookMark * GetBookMark(INT_PTR iIndex);
	void RemoveBookMark(INT_PTR iIndex);
	int GetNumberOfBookMarks() const;
	void AddBookMark(CBookMark *);

	//Default settings arrange for 5 elements, 100 meters apart starting at a depth of 0.  We save the
	//depth settings with the document, so if this is overridden we are laughing.
	void SetDocumentDepthGrid(COnStationDoc *document=NULL,COnStationView *View=NULL,int iColors=5);

	void Write(CFileHelper *h);
	void Read(CFileHelper *h);

		//Query results stuff
	COLORREF    m_crQueryColor;
	BOOL		m_bQueryFrom;
	BOOL		m_bQueryTo;
	BOOL		m_bQueryBox;

	//Search surveys results stuff
	CString m_szSurveyNameSearch;
	CString m_szSurveyStationsSearch;
	CString m_szSurveyPersonnelSearch;
	CString m_szSurveyDescriptionSearch;
	BOOL m_bOrSearch;


	//This is the color scheme associated with this view.  It is
	//changeable.
	int m_iColorSchemeIndex;

	void ReadFromIniFile();
	void WriteToIniFile();

};


class CFileHelper;
class COnStationView;
class CAppSettings
{
public:
	CAppSettings();
	~CAppSettings();

	CString m_szFontName;
	int	m_iFontSize;
	int m_iFontWeight;

	BOOL m_bStatusBar;
	BOOL m_bToolBar;
	BOOL m_bRuler;
	BOOL m_bBottomTabs;
	BOOL m_bCompass;

	PREFEREDUNITS m_PreferedUnits;
	BOOL IsMetric() const;

	//retain position between instances
//	WINDOWPLACEMENT m_wndPlacement;

//Printing
	BOOL m_bPrintShowScale;
	BOOL m_bShowSideScale;
	BOOL m_bPrintShowNorth;
	BOOL m_bPrintThickLines;
	int m_iPrintOverlap;		//how much to overlap by (mm)
	BOOL m_bPrintTitlePage;
	BOOL m_bPrintPeopleByYear;

	void WriteToIniFile();
	void ReadFromIniFile();
	CFont * GetFont();
	CFont * GetPrinterFont();
};


#endif

