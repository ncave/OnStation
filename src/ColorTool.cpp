
// (C) Copyright 1995, SMART Technologies Inc.
// Author: Taco van Ieperen

#include "stdafx.h"
#include "colortool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


COLORREF CGlobalColorManager::StandardVGAColors[]={RGB(255,255,255),RGB(255,0,0),RGB(0,255,0),RGB(0,0,255),
						RGB(192,192,192),RGB(128,0,0),RGB(0,128,0),RGB(0,0,128),
						RGB(128,128,128),RGB(255,255,0),RGB(255,0,255),RGB(0,255,255),
						RGB(128,128,0),RGB(128,0,128),RGB(0,128,128),RGB(0,0,0)};

CGlobalColorManager Colors_G(FALSE,TRUE,1);

CGlobalColorManager::CGlobalColorManager(BOOL bPrinting,BOOL bColor,int iThickness)
{
	m_bPrinting=bPrinting;
	m_bColor=bColor;
	for (int i=0;i<16;i++)
	{
		COLORREF crColor;
		if (bColor)
		{
			crColor=StandardVGAColors[i];
		}
		else
		{
			crColor=GetGrayColor(StandardVGAColors[i]);
		}
		m_pCachedPens[i]=new CPen(PS_SOLID,iThickness,crColor);;
	}
}

CGlobalColorManager::~CGlobalColorManager()
{
	for (int i=0;i<16;i++)
	{
		delete m_pCachedPens[i];
		m_pCachedPens[i]=NULL;
	}
}

void CGlobalColorManager::MakeGLColor(COLORREF crColor)
{
	float r=(float)GetRValue(crColor);
	float g=(float)GetGValue(crColor);
	float b=(float)GetBValue(crColor);
	glColor3f(r/255.0f,g/255.0f,b/255.0f);
}

CPen * CGlobalColorManager::GetPen(COLORREF crColor)
{
	for (int i=0;i<16;i++)
	{
		if (StandardVGAColors[i]==crColor)
		{
			return m_pCachedPens[i];
		}
	}
	TRACE(_T("CGlobalColorManager:: Couldn't find Color %li\n"),crColor);
	return m_pCachedPens[0];
}

COLORREF CGlobalColorManager::CycleColor(COLORREF crColor)
{
	for (int i=0;i<14;i++)
	{
		if (StandardVGAColors[i]==crColor)
		{
			return StandardVGAColors[i+1];
		}
	}
	return StandardVGAColors[0];
}

COLORREF CGlobalColorManager::GetGrayColor(COLORREF crColor)
{
	int iGreyLevel=GetRValue(crColor)*30/100+
				   GetGValue(crColor)*59/100+
				   GetBValue(crColor)*11/100;
	return RGB(iGreyLevel,iGreyLevel,iGreyLevel);
}

COLORREF CGlobalColorManager::GetLighterShade(COLORREF crColor)
{
	int iRed=GetRValue(crColor);
	int iGreen=GetGValue(crColor);
	int iBlue=GetBValue(crColor);
	if (iRed==255 || iGreen==255 || iBlue==255)
	{
		return crColor;
	}
	else
	{
		iRed=iRed*5/4;
		iRed=min(iRed,255);
		iGreen=iGreen*5/4;
		iGreen=min(iGreen,255);
		iBlue=iBlue*5/4;
		iBlue=min(iBlue,255);
		return RGB(iRed,iGreen,iBlue);
	}
}

COLORREF CGlobalColorManager::GetDarkerShade(COLORREF crColor)
{
	int iRed=GetRValue(crColor);
	int iGreen=GetGValue(crColor);
	int iBlue=GetBValue(crColor);
	iRed=iRed*3/4;
	iGreen=iGreen*3/4;
	iBlue=iBlue*3/4;
	return RGB(iRed,iGreen,iBlue);
}

COLORREF CGlobalColorManager::GetNeutralShade(COLORREF crColor)
{
	int iRed=GetRValue(crColor);
	int iGreen=GetGValue(crColor);
	int iBlue=GetBValue(crColor);
	if (iRed==255 || iGreen==255 || iBlue==255)
	{
		return RGB(iRed*4/5,iGreen*4/5,iBlue*4/5);	
	}
	else
	{
		return crColor;
	}
}

