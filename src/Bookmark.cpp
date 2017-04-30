#include "stdafx.h"
#include "bookmark.h"
#include "filehelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CBookMark::CBookMark(LPCTSTR szName,float X,float Y,float Z,float fRot,float fTilt,float fZoom,INT_PTR iColorSchemeIndex)
{
	m_szName=szName;
	m_fPanX=X;
	m_fPanY=Y;
	m_fPanZ=Z;
	m_fRot=fRot;
	m_fTilt=fTilt;
	m_fZoom=fZoom;
	m_iColorSchemeIndex=iColorSchemeIndex;
}

void CBookMark::Write(CFileHelper * FH)
{
	FH->WriteToken(_T("Begin"), _T("BookMark"));
	FH->WriteToken(_T("BookmarkName"),m_szName);
	FH->WriteToken(_T("PanX"),m_fPanX);
	FH->WriteToken(_T("PanY"),m_fPanY);
	FH->WriteToken(_T("PanZ"),m_fPanZ);
	FH->WriteToken(_T("Rotate"),m_fRot);
	FH->WriteToken(_T("Tilt"),m_fTilt);
	FH->WriteToken(_T("Scale"),m_fZoom);
	FH->WriteToken(_T("ColorSchemeIndex"),(int)m_iColorSchemeIndex);
	FH->WriteToken(_T("End"), _T("BookMark"));
}

void CBookMark::Read(CFileHelper *FH)
{
	FH->ReadTokenBegin(_T("BookMark"));
	m_szName=FH->ReadTokenString(_T("BookmarkName"));
	m_fPanX=FH->ReadTokenFloat(_T("PanX"));
	m_fPanY=FH->ReadTokenFloat(_T("PanY"));
	m_fPanZ=FH->ReadTokenFloat(_T("PanZ"));
	m_fRot=FH->ReadTokenFloat(_T("Rotate"));
	m_fTilt=FH->ReadTokenFloat(_T("Tilt"));
	m_fZoom=FH->ReadTokenFloat(_T("Scale"));
	m_iColorSchemeIndex=FH->ReadTokenInt(_T("ColorSchemeIndex"));
	FH->ReadTokenEnd(_T("BookMark"));
}

CBookMark::~CBookMark()
{
}

