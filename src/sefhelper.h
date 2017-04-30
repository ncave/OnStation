OBSOLETE
#ifndef I_SEFHELPER__
#define I_SEFHELPER__
// (C) Copyright 1995, Taco van Ieperen

// Helper function for reading SMAPS SEF files.
#include "filehelper.h"

enum SEFTOKEN {SEF_DIR,SEF_UP,
			   SEF_CPOINT,SEF_UNITS,SEF_DEC,SEF_ELEVUNITS,SEF_ELEVDEC,SEF_DATA,SEF_ENDCPOINT,
			   SEF_CTSURVEY,SEF_DATE,
			   SEF_FCID,SEF_FCDEC,SEF_BCID,SEF_BCDEC,SEF_FIID,SEF_FIDEC,SEF_BIID,SEF_BIDEC,
			   SEF_MTUNITS,SEF_MTDEC,SEF_ATUNITS,SEF_ATDEC,SEF_DECL,
			   SEF_PERSON,SEF_DUTY,SEF_ENDCTSURVEY,SEF_COM,
			   SEF_MTID,SEF_MTC,SEF_BCUNITS,SEF_FCUNITS,SEF_ROOT,SEF_ATID,
			   SEF_TOKEN_LAST,SEF_ERROR};



class CSEFHelper:public CFileHelper
{
	private:
		int m_iFrom;	//from
		int m_iFrom2;	//from
		int m_iTo;		//to
		int m_iTo2;		//to
		int m_iDist;	//dist
		int m_iDist2;	//dist
		int m_iFazi;	  //fazi
		int m_iFazi2;	  //fazi
		int m_iFinc;		//finc
		int m_iFinc2;		//finc
		int m_iBazi;	  //bazi
		int m_iBazi2;	  //bazi
		int m_iBinc;	//binc
		int m_iBinc2;	//binc
		int m_iLeft;	//left
		int m_iLeft2;	//left
		int m_iRight;	//right
		int m_iRight2;	//right
		int m_iCeil;	//ceil
		int m_iCeil2;	//ceil
		int m_iFloor;	//floor
		int m_iFloor2;	//floor
		int m_iType;	//L=Land?
		int m_iType2;

		int m_iStation;	//Tie in data
		int m_iStation2;
		int m_iEast;
		int m_iEast2;
		int m_iNorth;
		int m_iNorth2;
		int m_iElev;
		int m_iElev2;

		BOOL m_bNonLand;	//Whenever a type field is set, we
							//assume LAND until otherwise told

		BOOL GetSubField(LPSTR,int,int);
	public:
		CSEFHelper(CArchive * ar);
		~CSEFHelper();

		SEFTOKEN ParseToken();

		BOOL FormatHasUpDown();
		BOOL FormatHasLeftRight();
		BOOL FormatHasBacksights();

		BOOL ParseSEFFormatSpecifier();
		BOOL ReadSEFTieIn(LPTSTR szName,float *east,float *north,float *elev);
		BOOL ReadSEFSurveyShot(LPTSTR szFrom,LPTSTR szTo,float* length,float* compass,float* clino,float* up,float* down,float* left,float* right,float* compassback,float* clinoback);
		BOOL AppearsToBeLand();
};



#endif

