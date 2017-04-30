// colortl.h : header file
//
#ifndef I_COLORTOOL_
#define I_COLORTOOL_

/////////////////////////////////////////////////////////////////////////////
// CColorTool window

//These defines are used for the perspective transforms for 3d glasses
//BLUE
#define COLOR_LEFTEYE	RGB(0,0,255)
//RED
#define COLOR_RIGHTEYE	RGB(255,0,0)

#define COLOR_GRAY		RGB(128,128,128)

class CGlobalColorManager
{
public:
	static COLORREF StandardVGAColors[16];
	private:
		BOOL m_bPrinting;
		BOOL m_bColor;

		CPen * m_pCachedPens[16];
	public:
		CGlobalColorManager(BOOL bPrinting,BOOL bColor,int iThickness);
		~CGlobalColorManager();
	    
 		static void MakeGLColor(COLORREF crColor);
		static COLORREF GetGrayColor(COLORREF crColor);
		CPen * GetPen(COLORREF crColor);
		//Cycle the colors for the striped mode where colors change
		//with every survey
		static COLORREF CycleColor(COLORREF crColor);

		static COLORREF GetLighterShade(COLORREF crColor);
		static COLORREF GetDarkerShade(COLORREF crColor);
		static COLORREF GetNeutralShade(COLORREF crColor);
};

#endif
