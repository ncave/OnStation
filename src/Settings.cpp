#include "stdafx.h"
#include "onstation.h"
#include "settings.h"
#include "onstationdoc.h"
#include "folder.h"
#include "realfolder.h"
#include "colorwidget.h"
#include "filehelper.h"
#include "nodetree.h"
#include "onstationview.h"
#include "colortool.h"
#include "bookmark.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern COnStationApp theApp;
CSettings * pSettings_G=NULL;

void CAppSettings::WriteToIniFile()
{
//	theApp.WriteProfileInt(_T("Client"), _T("normleft"),m_wndPlacement.rcNormalPosition.left);
///	theApp.WriteProfileInt(_T("Client"), _T("normtop"),m_wndPlacement.rcNormalPosition.top);
//	theApp.WriteProfileInt(_T("Client"), _T("normright"),m_wndPlacement.rcNormalPosition.right);
//	theApp.WriteProfileInt(_T("Client"), _T("normbottom"),m_wndPlacement.rcNormalPosition.bottom);
//	theApp.WriteProfileInt(_T("Client"), _T("flags"),m_wndPlacement.flags);
//	theApp.WriteProfileInt(_T("Client"), _T("showcmd"),m_wndPlacement.showCmd);
//	theApp.WriteProfileInt(_T("Client"), _T("minposx"),m_wndPlacement.ptMinPosition.x);
//	theApp.WriteProfileInt(_T("Client"), _T("minposy"),m_wndPlacement.ptMinPosition.y);
//	theApp.WriteProfileInt(_T("Client"), _T("maxposx"),m_wndPlacement.ptMaxPosition.x);
//	theApp.WriteProfileInt(_T("Client"), _T("maxposy"),m_wndPlacement.ptMaxPosition.y);

	theApp.WriteProfileString(_T("Font"), _T("FontName"),m_szFontName);
	theApp.WriteProfileInt(_T("Font"), _T("FontSize"),m_iFontSize);
	theApp.WriteProfileInt(_T("Font"), _T("FontWeight"),m_iFontWeight);
	
	theApp.WriteProfileInt(_T("Bars"), _T("StatusBars"),m_bStatusBar);
	theApp.WriteProfileInt(_T("Bars"), _T("Toolbars"),m_bToolBar);
	theApp.WriteProfileInt(_T("Bars"), _T("Ruler"),m_bRuler);
	theApp.WriteProfileInt(_T("Bars"), _T("IndexTabs"),m_bBottomTabs);
	theApp.WriteProfileInt(_T("Bars"), _T("Compass"),m_bCompass);

	theApp.WriteProfileInt(_T("Units"), _T("Prefered"),m_PreferedUnits);
	theApp.WriteProfileInt(_T("Printing"), _T("Scale"),m_bPrintShowScale);
	theApp.WriteProfileInt(_T("Printing"), _T("North"),m_bPrintShowNorth);
	theApp.WriteProfileInt(_T("Printing"), _T("Overlap"),m_iPrintOverlap);
	theApp.WriteProfileInt(_T("Printing"), _T("ThickLines"),m_bPrintThickLines);
	theApp.WriteProfileInt(_T("Printing"), _T("SideScale"),m_bShowSideScale);
	theApp.WriteProfileInt(_T("Printing"), _T("TitlePage"),m_bPrintTitlePage);
	theApp.WriteProfileInt(_T("Printing"), _T("PeopleByYear"),m_bPrintPeopleByYear);

	pSettings_G->WriteToIniFile();
}

CAppSettings::CAppSettings()
{
}

CAppSettings::~CAppSettings()
{
}

BOOL CAppSettings::IsMetric() const
{
	return (m_PreferedUnits==METERS || m_PreferedUnits==METERSANDKM);
}

void CAppSettings::ReadFromIniFile()
{
//	m_wndPlacement.length=sizeof(WINDOWPLACEMENT);
//	m_wndPlacement.rcNormalPosition.left=theApp.GetProfileInt(_T("Client"), _T("normleft"),10);
//	m_wndPlacement.rcNormalPosition.top=theApp.GetProfileInt(_T("Client"), _T("normtop"),10);
//	m_wndPlacement.rcNormalPosition.right=theApp.GetProfileInt(_T("Client"), _T("normright"),630);
//	m_wndPlacement.rcNormalPosition.bottom=theApp.GetProfileInt(_T("Client"), _T("normbottom"),450);
//	m_wndPlacement.flags=theApp.GetProfileInt(_T("Client"), _T("flags"),WPF_RESTORETOMAXIMIZED);
//	m_wndPlacement.showCmd=theApp.GetProfileInt(_T("Client"), _T("showcmd"),SW_SHOWNORMAL);
//	m_wndPlacement.ptMinPosition.x=theApp.GetProfileInt(_T("Client"), _T("minposx"),50);
//	m_wndPlacement.ptMinPosition.y=theApp.GetProfileInt(_T("Client"), _T("minposy"),50);
//	m_wndPlacement.ptMaxPosition.x=theApp.GetProfileInt(_T("Client"), _T("maxposx"),0);
//	m_wndPlacement.ptMaxPosition.y=theApp.GetProfileInt(_T("Client"), _T("maxposy"),0);

	m_bStatusBar=theApp.GetProfileInt(_T("Bars"), _T("StatusBars"),TRUE);
	m_bToolBar=theApp.GetProfileInt(_T("Bars"), _T("Toolbars"),TRUE);
	m_bRuler=theApp.GetProfileInt(_T("Bars"), _T("Ruler"),TRUE);
	m_bBottomTabs=theApp.GetProfileInt(_T("Bars"), _T("IndexTabs"),TRUE);
	m_bCompass=theApp.GetProfileInt(_T("Bars"), _T("Compass"),TRUE);

	m_PreferedUnits=(PREFEREDUNITS)theApp.GetProfileInt(_T("Units"), _T("Prefered"),METERS);

	m_bPrintShowScale=theApp.GetProfileInt(_T("Printing"), _T("Scale"),TRUE);
	m_bPrintShowNorth=theApp.GetProfileInt(_T("Printing"), _T("North"),TRUE);
	m_iPrintOverlap=theApp.GetProfileInt(_T("Printing"), _T("Overlap"),20);
	m_bPrintThickLines=theApp.GetProfileInt(_T("Printing"), _T("ThickLines"),FALSE);
	m_bShowSideScale=theApp.GetProfileInt(_T("Printing"), _T("SideScale"),TRUE);
	m_bPrintTitlePage=theApp.GetProfileInt(_T("Printing"), _T("TitlePage"),FALSE);
	m_bPrintPeopleByYear=theApp.GetProfileInt(_T("Printing"), _T("PeopleByYear"),FALSE);

	m_szFontName=theApp.GetProfileString(_T("Font"), _T("FontName"), _T("Arial"));
	m_iFontSize=theApp.GetProfileInt(_T("Font"), _T("FontSize"),8);
	m_iFontWeight=theApp.GetProfileInt(_T("Font"), _T("FontWeight"),100);

	pSettings_G->ReadFromIniFile();
}



void CSettings::WriteToIniFile()
{
	theApp.WriteProfileInt(_T("ViewSettings"), _T("ShowQueryFromShot"),m_bQueryFrom);
	theApp.WriteProfileInt(_T("ViewSettings"), _T("ShowQueryToShot"),m_bQueryTo);
	theApp.WriteProfileInt(_T("ViewSettings"), _T("ShowQueryBox"),m_bQueryBox);
	theApp.WriteProfileInt(_T("ViewSettings"), _T("QueryColor"),m_crQueryColor);
}

void CSettings::ReadFromIniFile()
{
	m_bQueryFrom=theApp.GetProfileInt(_T("ViewSettings"), _T("ShowQueryFromShot"),m_bQueryFrom);
	m_bQueryTo=theApp.GetProfileInt(_T("ViewSettings"), _T("ShowQueryToShot"),m_bQueryTo);
	m_bQueryBox=theApp.GetProfileInt(_T("ViewSettings"), _T("ShowQueryBox"),m_bQueryBox);
	m_crQueryColor=theApp.GetProfileInt(_T("ViewSettings"), _T("QueryColor"),m_crQueryColor);
}

CSettings::~CSettings()
{
	int iIndex=GetNumberOfBookMarks();
	for (int i=0;i<iIndex;i++)
	{
		CBookMark * BM=GetBookMark(i);
		delete BM;
	}
	m_BookMarkList.RemoveAll();
}


CSettings::CSettings()
{
	ResetContents();

	m_iColorSchemeIndex=0;

	m_crQueryColor=RGB(255,0,0);
	m_bQueryFrom=TRUE;
	m_bQueryTo=TRUE;
	m_bQueryBox=TRUE;
}

void CSettings::Write(CFileHelper *file)
{
	file->WriteToken(_T("ProprietaryStart"), _T("On Station Editor Settings"));
	file->WriteToken(_T("Begin"), _T("ColorSchemes"));
	for (int i=0;i<TOTAL_COLOR_SCHEMES;i++)
	{
		file->WriteToken(_T("Name"),m_szColorSchemeNames[i]);
		file->WriteTokenColor(_T("SurfaceColor"),m_cSurfaceColors[i]);
		file->WriteToken(_T("SurfaceSurveys"),m_bSurfaceSurveys[i]);
		file->WriteToken(_T("ShowClosureErrors"),m_bShowClosureErrors[i]);
		file->WriteToken(_T("ShowClosedLoops"),m_bShowClosedLoops[i]);
		file->WriteToken(_T("ShowFixedPoints"),m_bBoxFixedPoints[i]);
		file->WriteToken(_T("ShowStationNames"),(int)m_ShowStationNames[i]);
		file->WriteToken(_T("Perspective"),m_bPerspective[i]);
		file->WriteToken(_T("ViewType"),(int)m_ViewTypes[i]);
		//Passage walls stuff
		file->WriteToken(_T("PassageType"),(int)m_PassageType[i]);
		file->WriteToken(_T("OpaquePassages"),m_bOpaquePassages[i]);
		file->WriteToken(_T("DepthFog"),m_bDepthFogging[i]);
		file->WriteToken(_T("Lights"),m_bMultipleLights[i]);
		file->WriteToken(_T("NoLightRotate"),m_bRotateNoLighting[i]);
		file->WriteToken(_T("NoSolidRotate"),m_bRotateNoSolids[i]);
	}
	file->WriteToken(_T("End"), _T("ColorSchemes"));
	file->WriteToken(_T("NumBookmarks"),GetNumberOfBookMarks());
	for (int i=0;i<GetNumberOfBookMarks();i++)
	{
		CBookMark *BM=GetBookMark(i);
		BM->Write(file);
	}
	file->WriteToken(_T("Begin"), _T("DepthColors"));
	file->WriteToken(_T("NumberOfDepthColors"),m_iNumDepthColors);
	file->WriteToken(_T("StartingDepth"),m_fStartDepth);
	file->WriteToken(_T("DepthIncrements"),m_fDepthIncrements);
	for (int i=0;i<m_iNumDepthColors;i++)
	{
		file->WriteTokenColor(_T("Color"),m_crDepthColors[i]);
	}
	file->WriteToken(_T("End"), _T("DepthColors"));

	file->WriteToken(_T("Begin"), _T("AgeColors"));
	file->WriteToken(_T("NumberOfAgeColors"),m_iNumAgeColors);
	for (int i=0;i<m_iNumAgeColors;i++)
	{
		file->WriteTokenDate(_T("Date"),m_dwAgeDates[i]);
		file->WriteTokenColor(_T("Color"),m_crAgeColors[i]);
	}
	file->WriteToken(_T("End"), _T("AgeColors"));

	file->WriteToken(_T("ProprietaryEnd"), _T("On Station Editor Settings"));
}

void CSettings::Read(CFileHelper *file)
{
	if (file->CheckProprietaryStart(_T("On Station Editor Settings"),TRUE))
	{
		//The last call just did a lookahead.  Now get it
		file->ReadTokenString(_T("ProprietaryStart"));
		file->ReadTokenBegin(_T("ColorSchemes"));
		for (int i=0;i<TOTAL_COLOR_SCHEMES;i++)
		{
			if (i<MAX_REGULAR_COLOR_SCHEMES)
			{
				m_szColorSchemeNames[i]=file->ReadTokenString(_T("Name"));
			}
			else
			{
				file->ReadTokenString(_T("Name"));	//ignore the name
			}
			m_cSurfaceColors[i]=file->ReadTokenColor(_T("SurfaceColor"));
			m_bSurfaceSurveys[i]=file->ReadTokenInt(_T("SurfaceSurveys"));
			m_bShowClosureErrors[i]=file->ReadTokenInt(_T("ShowClosureErrors"));
			m_bShowClosedLoops[i]=file->ReadTokenInt(_T("ShowClosedLoops"));
			m_bShowClosedLoops[i]=FALSE;	//bug if we show them on startup
			m_bShowReferenceGrid[i]=FALSE;
			m_bBoxFixedPoints[i]=file->ReadTokenInt(_T("ShowFixedPoints"));
			m_ShowStationNames[i]=(STATIONSHOW)(file->ReadTokenInt(_T("ShowStationNames")));
			m_bPerspective[i]=(int)file->ReadTokenInt(_T("Perspective"));
			m_ViewTypes[i]=(VIEWTYPE)file->ReadTokenInt(_T("ViewType"));

			//Passage walls stuff
			m_PassageType[i]=(PASSAGE_TYPE)file->ReadTokenInt(_T("PassageType"));
			m_bOpaquePassages[i]=file->ReadTokenInt(_T("OpaquePassages"));
			m_bDepthFogging[i]=file->ReadTokenInt(_T("DepthFog"));
			m_bMultipleLights[i]=file->ReadTokenInt(_T("Lights"));
			m_bRotateNoLighting[i]=file->ReadTokenInt(_T("NoLightRotate"));
			m_bRotateNoSolids[i]=file->ReadTokenInt(_T("NoSolidRotate"));
		}
		file->ReadTokenEnd(_T("ColorSchemes"));
		m_bShowClosedLoops[COLORSCHEME_BLUNDER]=TRUE;
		int iNumBookMarks=file->ReadTokenInt(_T("NumBookmarks"));
		for (int i=0;i<iNumBookMarks;i++)
		{
			CBookMark *pBM=new CBookMark(_T(""),0.0f,0.0f,0.0f,0.0f,0.0f,10.0f,0);
			pBM->Read(file);
			AddBookMark(pBM);
		}
		file->ReadTokenBegin(_T("DepthColors"));
		m_iNumDepthColors=file->ReadTokenInt(_T("NumberOfDepthColors"));
		m_fStartDepth=file->ReadTokenFloat(_T("StartingDepth"));
		m_fDepthIncrements=file->ReadTokenFloat(_T("DepthIncrements"));
		for (int i=0;i<m_iNumDepthColors;i++)
		{
			m_crDepthColors[i]=file->ReadTokenColor(_T("Color"));
		}
		file->ReadTokenEnd(_T("DepthColors"));

		file->ReadTokenBegin(_T("AgeColors"));
		m_iNumAgeColors=file->ReadTokenInt(_T("NumberOfAgeColors"));
		for (int i=0;i<m_iNumAgeColors;i++)
		{
			m_dwAgeDates[i]=file->ReadTokenDate(_T("Date"));
			m_crAgeColors[i]=file->ReadTokenColor(_T("Color"));
		}
		file->ReadTokenEnd(_T("AgeColors"));

		file->CheckProprietaryEnd(_T("On Station Editor Settings"));
	}
}

CBookMark * CSettings::GetBookMark(INT_PTR iIndex)
{
	POSITION P;
	P=m_BookMarkList.FindIndex(iIndex);
	if (P!=NULL)
	{
		return (CBookMark *)m_BookMarkList.GetAt(P);
	}
	else
	{
		return NULL;
	}
}

void CSettings::RemoveBookMark(INT_PTR iIndex)
{
	POSITION P;
	P=m_BookMarkList.FindIndex(iIndex);
	if (P!=NULL)
	{
		m_BookMarkList.RemoveAt(P);
	}
}

int CSettings::GetNumberOfBookMarks() const
{
	return m_BookMarkList.GetCount();
}

void CSettings::AddBookMark(CBookMark *Mark)
{
	m_BookMarkList.AddHead((CObject *)Mark);
}

void CSettings::SetDefaultAgeColors()
{
	for (int i=0;i<MAX_AGE_COLORS;i++)
	{
		m_crAgeColors[i]=CGlobalColorManager::StandardVGAColors[i];
	}
	CTime time=CTime::GetCurrentTime();
	SetAgeColors(MakeMyDate(time.GetYear()-20,1,1),10,2);
}

COLORREF CSettings::GetColorByAge(DWORD dwAge)
{
	for (int i=0;i<m_iNumAgeColors;i++)
	{
		if (dwAge<=m_dwAgeDates[i])
		{
			return m_crAgeColors[i];
		}
	}
	return m_crAgeColors[m_iNumAgeColors-1];
}

void CSettings::SetAgeColors(DWORD dwDate,UINT iNumIntervals,UINT iYears)
{
	m_iNumAgeColors=iNumIntervals;
	for (UINT i=0;i<iNumIntervals;i++)
	{
		m_dwAgeDates[i]=dwDate;
		int iYear=GetYear(dwDate);
		int iDay=GetDay(dwDate);
		int iMonth=GetMonth(dwDate);
		dwDate=MakeMyDate(iYear+iYears,iMonth,iDay);
	}
}

void CSettings::ResetContents()
{
//Remove all of the bookmarks
	int iIndex=GetNumberOfBookMarks();
	for (int i=0;i<iIndex;i++)
	{
		CBookMark * BM=GetBookMark(i);
		delete BM;
	}
	m_BookMarkList.RemoveAll();

	m_fDepthIncrements=NAN;	//Default could be generated from the data
	m_fStartDepth=NAN;		//or can be set manually.  Set to NAN to identify the non-initialized case
	m_iNumAgeColors=0;

	for (int i=0;i<MAX_REGULAR_COLOR_SCHEMES;i++)
	{
		CString CS;
		CS.Format(IDS_COLORSCHEMENAME,i+1);
		m_szColorSchemeNames[i]=CS;
	}
	m_szColorSchemeNames[COLORSCHEME_AGE].LoadString(IDS_COLORSCHEMEAGE);
	m_szColorSchemeNames[COLORSCHEME_DEPTH].LoadString(IDS_COLORSCHEMEDEPTH);
	m_szColorSchemeNames[COLORSCHEME_BLUNDER].LoadString(IDS_COLORSCHEMEBLUNDER);

	for (int i=0;i<TOTAL_COLOR_SCHEMES;i++)
	{
		m_cSurfaceColors[i]=RGB(128,128,128);
		m_bSurfaceSurveys[i]=TRUE;
		m_bShowClosureErrors[i]=TRUE;
		m_bShowClosedLoops[i]=FALSE;
		m_bBoxFixedPoints[i]=TRUE;
		m_ShowStationNames[i]=SHOW_INTERSECT;
		m_bPerspective[i]=FALSE;
		m_ViewTypes[i]=VT_FREEFORM;

		//Passage walls stuff
		m_PassageType[i]=PT_LINE_PLOT;
		m_bOpaquePassages[i]=FALSE;
		m_bDepthFogging[i]=FALSE;
		m_bMultipleLights[i]=TRUE;
		m_bRotateNoLighting[i]=TRUE;
		m_bRotateNoSolids[i]=TRUE;
	}

	for (int i=0;i<MAX_AGE_COLORS;i++)
	{
		m_crAgeColors[i]=CGlobalColorManager::StandardVGAColors[i];
	}
	for (int i=0;i<MAX_DEPTH_COLORS;i++)
	{
		m_crDepthColors[i]=CGlobalColorManager::StandardVGAColors[i];
	}
	SetDocumentDepthGrid();
	SetDefaultAgeColors();
	m_iColorSchemeIndex=0;
	m_bShowClosedLoops[COLORSCHEME_BLUNDER]=TRUE;
}

void CSettings::SetDocumentDepthGrid(COnStationDoc *document,COnStationView *View,int iNumDepthColors)
{
	//We will crash if this variable is out of bounds, so do a validity
	//check on it.
	ASSERT(iNumDepthColors>1 && iNumDepthColors<=MAX_DEPTH_COLORS);
	iNumDepthColors=max(1,iNumDepthColors);
	iNumDepthColors=min(MAX_DEPTH_COLORS,iNumDepthColors);

	int iDepthDelta=100;
	m_fStartDepth=0.0f;
	if (View!=NULL)
	{
		CNodeTreeSummary NTS;
		CNodeTree *NT=document->GetNodeTree();
		NT->GetViewInfo(&NTS);

		iDepthDelta=(int)(NTS.m_fHighest-NTS.m_fDeepest);
		m_fStartDepth=(float)((int)NTS.m_fHighest);
	}
	//	CFolderData Data;
	m_iNumDepthColors=iNumDepthColors;
	iDepthDelta=(iDepthDelta/DEPTH_ROUNDING+1)*DEPTH_ROUNDING;	//Round up to nearest 50 meters
	m_fDepthIncrements=(float)iDepthDelta/(float)m_iNumDepthColors;
	for (int i=0;i<m_iNumDepthColors;i++)
	{
		m_crDepthColors[i]=CGlobalColorManager::StandardVGAColors[i];		//index in order
	}
}

COLORREF CSettings::GetDepthColor(float fDepth)
{
	int iZone=(int)((m_fStartDepth-fDepth)/m_fDepthIncrements);
	return GetDepthColor(iZone);
}

COLORREF CSettings::GetDepthColor(int iDepthInterval)
{
	if (iDepthInterval<0)
		iDepthInterval=0;
	if (iDepthInterval>=m_iNumDepthColors)
		iDepthInterval=m_iNumDepthColors-1;
	return m_crDepthColors[iDepthInterval];
}

CFont* CAppSettings::GetFont()
{
	CFont * pFont=new CFont();
	pFont->CreateFont(m_iFontSize,0,0,0,m_iFontWeight,FALSE,FALSE,0,DEFAULT_CHARSET,
		OUT_CHARACTER_PRECIS,CLIP_CHARACTER_PRECIS,
		DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,m_szFontName);
	return pFont;
}

CFont* CAppSettings::GetPrinterFont()
{
	CFont * pFont=new CFont();
	pFont->CreateFont(m_iFontSize*2,0,0,0,m_iFontWeight,FALSE,FALSE,0,DEFAULT_CHARSET,
		OUT_CHARACTER_PRECIS,CLIP_CHARACTER_PRECIS,
		DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,m_szFontName);
	return pFont;
}

COLORREF CSettings::GetBlunderColor(int iDeviation)
{
	switch (iDeviation)
	{
	case 0:
		return RGB(128,128,128);
	case 1:
		return RGB(192,192,129);
	case 2:
		return RGB(255,255,255);
	case 3:
		return RGB(128,0,0);
	default:
		return RGB(255,0,0);
	}
}
