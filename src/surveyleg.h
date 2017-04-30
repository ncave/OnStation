#ifndef I_LEGDATA__
#define I_LEGDATA__

#include "surveyshot.h"
#include "folder.h"

class CSurveyLeg;
class CFileDumpHelper;
class COnStationDoc;
class CFileHelper;
class CComment;

class CSurveyLeg:public CFolder
{
	private:
		CSurveyShotArray m_MyShots;

		//Private since we need to update stuff if this changes
	    COLORREF m_iMyGlobalColorReferences[TOTAL_COLOR_SCHEMES];

		//Optional items.
		BOOL m_bBacksights;

		//These variables must be private because changing them could effect
		//shot data even though the shots themselves are not changed.
		float m_fDeclination;
		TCHAR m_szCompass1Name[64];
		float m_fCompass1Error;
		TCHAR m_szClino1Name[64];
		float m_fClino1Error;
		TCHAR m_szCompass2Name[64];
		float m_fCompass2Error;
		TCHAR m_szClino2Name[64];
		float m_fClino2Error;
		TCHAR m_szTapeName[64];
		float m_fTapeError;
		TCHAR m_szDepthGaugeName[64];
		float m_fDepthGaugeError;

		//Standard errors used for loop closures
		float m_fStandardTapeError;
		float m_fStandardClinoError;
		float m_fStandardCompassError;
		float m_fDepthGaugeStandardError;
		TCHAR m_szPersonName[6][64];
		TCHAR m_szDuty[6][64];
		BOOL m_bMeters;
		BOOL m_bEuroDegrees; //100 degrees in a right angle.  Used in some areas of europe.
		BOOL m_bCaveDive;	//shots are all underwater.
		BOOL m_bDiveMeters;

		//This is called when some higher parameter of this leg has changed
		//which can cause the actual data in the shots to change even if the
		//shots themselves do not.  An example is a switch to METRIC
		void RefreshLegNumbers();
//Writes all the field orders to a string with the format
//1,7,3,2,4 etc.. where the numbers correspond to the enum
//values.
		void FieldOrderToString(CString &CS);

		void ReadProprietaryStuff(CFileHelper * f);
		void WriteProprietaryStuff(CFileHelper *f);
    public:
    	CSurveyLeg(COnStationDoc *doc);
    	~CSurveyLeg();

		//Interrogation functions
		inline float GetDeclination() const {return m_fDeclination;}
		inline LPCTSTR GetCompass1Name() const {return m_szCompass1Name;} 
		inline LPCTSTR GetCompass2Name() const {return m_szCompass2Name;} 
		inline LPCTSTR GetClino1Name() const {return m_szClino1Name;} 
		inline LPCTSTR GetClino2Name() const {return m_szClino2Name;} 
		inline LPCTSTR GetTapeName() const  {return m_szTapeName;}
		inline BOOL GetCaveDive() const {return m_bCaveDive;}
		inline float GetClino1Error() const {return m_fClino1Error;}
		inline float GetClino2Error() const {return m_fClino2Error;}
		inline float GetCompass1Error() const {return m_fCompass1Error;}
		inline float GetCompass2Error() const {return m_fCompass2Error;}
		inline float GetTapeError() const {return m_fTapeError;}
		inline float GetStandardTapeError() const {return m_fStandardTapeError;}
		inline float GetStandardClinoError() const {return m_fStandardClinoError;}
		inline float GetStandardCompassError() const {return m_fStandardCompassError;}
	    inline LPCTSTR GetPersonName(int iPerson) const {return m_szPersonName[iPerson];}
		inline LPCTSTR GetPersonDuty(int iPerson) const {return m_szDuty[iPerson];}
		inline BOOL GetDiveMeters() const {return m_bDiveMeters;}
		inline BOOL GetMeters() const {return m_bMeters;}
		inline BOOL GetEuroDegrees() const {return m_bEuroDegrees;}

		BOOL QuerySurveyComments(CPtrArray * result,CQuery * pCommentQuery,CQuery * pTextQuery);
		
		//Value assignment functions
		void SetDeclination(float fDeclination);
		inline void SetCompass1Name(LPCTSTR szCompass1Name) {lstrcpyn(m_szCompass1Name,szCompass1Name,_countof(m_szCompass1Name));}
		inline void SetCompass2Name(LPCTSTR szCompass2Name) {lstrcpyn(m_szCompass2Name,szCompass2Name,_countof(m_szCompass2Name));}
		inline void SetClino1Name(LPCTSTR szClino1Name) {lstrcpyn(m_szClino1Name,szClino1Name,_countof(m_szClino1Name));}
		inline void SetClino2Name(LPCTSTR szClino2Name) {lstrcpyn(m_szClino2Name,szClino2Name,_countof(m_szClino2Name));}
		inline void SetTapeName(LPCTSTR szTapeName) {lstrcpyn(m_szTapeName,szTapeName,_countof(m_szTapeName));}
		void SetCompass1Error(float);
		void SetCompass2Error(float);
		void SetClino1Error(float);
		void SetClino2Error(float);
		void SetTapeError(float);
		void SetStandardTapeError(float);
		void SetStandardCompassError(float);
		void SetStandardClinoError(float);
		inline void SetPersonName(int iPerson,LPCTSTR szName) {lstrcpyn(m_szPersonName[iPerson],szName,64);}
		inline void SetPersonDuty(int iPerson,LPCTSTR szDuty) {lstrcpyn(m_szDuty[iPerson],szDuty,64);}
		void SetMeters(BOOL bMeters);
		void SetDiveMeters(BOOL bDiveMeters);
		inline void SetCaveDive(BOOL b) {m_bCaveDive=b;}

		BOOL HasDisabledLegs(int iColorScheme);
		
		COLORREF GetDrawColor(int iColorScheme);
		COLORREF GetAssignedColor(int iColorScheme);
		COLORREF SetColor(COLORREF crColor,int iColorSchemeIndex,BOOL bRainbow,BOOL *,CLegQuery * pLegQuery);
		void SaveFormattedText(CFileDumpHelper *,int iColorSchemeIndex,BOOL bCloseLoops,BOOL bCoordinatesOnly);
		
		BOOL DoesAnythingMatchTheQuery(CLegQuery * pQuery);
		inline BOOL HasBacksights() const {return m_bBacksights;}
		inline BOOL IsActive(int iScheme) const {return m_iMyGlobalColorReferences[iScheme]!=COLOR_INACTIVE;}
		DWORD m_dwDate;		//00YYMMDD


		CSurveyShotArray *GetShotArray() {return &m_MyShots;}
	    TCHAR m_szDescription[512];
		TCHAR m_szSystem[64];	    

		//saveguts and restore methods.
	    void Write(CFileHelper * h);
		void Read(CFileHelper * h);
	    void DeleteContents();
		void DeleteShots();
		
	    BOOL m_bRenderThis; 
	    virtual BOOL IsFolder() {return FALSE;}
		virtual BOOL IsColorSchemeDataSimilar(int First,int Second);
		void GatherFolderData(CFolderData *,int iColorScheme,BOOL bClosedView,CLegQuery * pLegQuery);

		//Turn optional items on and off.  This also updates the
		//field list.
		void SetBacksights(BOOL bBacksights);
		void SetEuroDegrees(BOOL bEuroDegrees) {m_bEuroDegrees=bEuroDegrees;}

		TCHAR m_szFieldOrder[8];	//LAIUDRL are characters
		short m_iColWidth[15];	//maximum that we can have
		void GenerateDefaultFieldOrder();

		int FindEntry(TCHAR c) const;
};

#endif

