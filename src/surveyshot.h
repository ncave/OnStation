#ifndef I_SHOTDATA
#define I_SHOTDATA
#include "afxtempl.h"
#include "comment.h"

#define SHOT_SURFACE   0x01  //S
#define SHOT_EXCLUDE_FROM_CLOSING	0x02	//C
#define SHOT_EXCLUDE_FROM_LENGTH_TOTALS	0x04	//L
#define SHOT_EXCLUDE_FROM_ALL_PROCESSING	0x08   //X
#define SHOT_EXCLUDE_FROM_PLOTTING	0x10	//P
#define SHOT_SPLAY_SHOT				0x20	//Y

class CNode;	//forward reference.
class CNodeTree;
class CSurveyLeg;
class CFileHelper;
class CComment;
class CQuery;
class CSurveyShot:public CObject
{
    friend class CNodeTree;
protected:
    TCHAR  m_szFromStationName[MAX_STATION_NAME_LENGTH];
	TCHAR  m_szToStationName[MAX_STATION_NAME_LENGTH];
    //Leg containing this shot
    CSurveyLeg * m_Leg;

	WORD m_wShotFlags;

	float m_fUp;	//walls...
	float m_fDown;
	float m_fLeft;
	float m_fRight;
public:
	virtual ~CSurveyShot();
	CSurveyShot(CSurveyLeg *);

#ifdef _DEBUG
	void Dump();
#endif
	//only the leg grid page can play with this guy
	CArray <CComment *,CComment *> m_Comments;

	//These flags use the masks defined at the top of this file
	WORD GetShotFlags() const {return m_wShotFlags;}
	void SetShotFlags(WORD w) {m_wShotFlags=w;}

	BOOL QuerySurveyComments(CPtrArray * result,CQuery * pCommentQuery,CQuery * pTextQuery);
	void AddComment(LPCTSTR szText);
	virtual void CalculateDeltas();
	//From and to nodes in the node tree.  We cache them here since
	//figuring them out is expensive.  This might be cleaner if we
	//didn't do this, since I don't believe the global calculation
	//uses these 
	CNode * m_ToNode;				
	CNode * m_FromNode;

	//Stress calculation results.  Since stress colors only appear on a single
	//colors scheme we don't need an array of these, we can just use one.
	float m_fStressCompass;
	float m_fStressDev;
	float m_fStressTape;
	float m_fStressClino;
	void SetStressCompass(float sig_thd) {m_fStressCompass=sig_thd;}
	void SetStressClino(float sig_inc) {m_fStressClino=sig_inc;}
	void SetStressTape(float sig_len) {m_fStressTape=sig_len;}
	void SetStressDev(float sig_dev) {m_fStressDev=sig_dev;}      // Measure of max pure error
	float GetStressDeviation();

	//we calculate this early so that we don't spend a lot of time
	//doing sines and cosines when we need quick results later on.
	CPosMatrix m_Delta;

	//Members to get at data
	inline CSurveyLeg * GetLeg() const {return m_Leg;}		

	//We fudge the dimensions a bit here in order to get decent
	//passage volume rendering.  To change this back so that passages
	//with no walls draw as simple lines, you need only modify the
	//return value to be as in the GetRaw functions below
	float GetUp() const; 
	float GetDown() const;
	float GetLeft() const;
	float GetRight() const;
	inline BOOL EmptyCrossSection() const {return (m_fUp==0.0f && m_fDown==0.0f && m_fLeft==0.0f && m_fRight==0.0f);}
	virtual float GetCalculationClino() const=0;
	virtual float GetCalculationCompass() const=0;
	virtual float GetCalculationLength() const=0;
	//Gets the portion of the length which is horizontal (lenght*
	virtual float GetHorizontalLength() const;
	//WE still use the raw data here for the editor and stuff
	inline float GetRawUp() const {return m_fUp;}
	inline float GetRawDown() const {return m_fDown;}
	inline float GetRawLeft() const {return m_fLeft;}
	inline float GetRawRight() const {return m_fRight;}

	inline BOOL IsEmpty() const {return m_szFromStationName[0]==0;}
    inline LPCTSTR GetFromStationName() const {return m_szFromStationName;}
    inline LPCTSTR GetToStationName() const {return m_szToStationName;}

	//members to set data.  Setting the station name has added
	//semantics in that it automatically adds the shot to the NodeTree.
	//This is because as soon as the name is known, we know where we
	//fit.  SetReadings() should be called before SetStationNames()
    void SetStationNames(LPCTSTR szFrom,LPCTSTR szTo);
	void SetUDLR(float,float,float,float);		

	//save and restore from file
    virtual void Write(CFileHelper * )=0;
    virtual void ReadVersion1(CFileHelper *)=0;
	virtual void ReadVersion2(CFileHelper *)=0;
	void WriteComment(CFileHelper *);
};

class CStandardSurveyShot:public CSurveyShot
{
private:
    float m_fLength;	//length (in meters)
    float m_fCompass;
	float m_fCompass2;	//backsight
    float m_fClinometer;
	float m_fClinometer2;  //backsight

public:
	CStandardSurveyShot(CSurveyLeg *);
	virtual ~CStandardSurveyShot();
	//Used by the survey leg whenever something such as the metric
	//flag changes.  This is not an excuse to not use it locally as well.
	float GetLength() const { return m_fLength;}
	float GetCompass() const {return m_fCompass;}
	float GetCompass2() const {return m_fCompass2;}
	float GetClino() const {return m_fClinometer;}
	float GetClino2() const {return m_fClinometer2;}
	float GetCalculationLength() const;
	float GetCalculationClino() const;
	float GetCalculationCompass() const;
	float GetCalculationDepth() const {ASSERT(FALSE);return 0.0f;}

	void SetReadings(float fLength,float fCompass,float fClino,float fCompass2,float fClino2);

	//save and restore from file
    virtual void Write(CFileHelper * );
    virtual void ReadVersion1(CFileHelper *);
    virtual void ReadVersion2(CFileHelper *);
#ifdef _DEBUG
	static void SelfTest();
#endif
};

class CCaveDiveSurveyShot:public CSurveyShot
{
private:
    float m_fLength;	//length (in meters)
    float m_fCompass;
	float m_fDepthAtFromStation;

public:
	CCaveDiveSurveyShot(CSurveyLeg *);
	virtual ~CCaveDiveSurveyShot();
	//Used by the survey leg whenever something such as the metric
	//flag changes.  This is not an excuse to not use it locally as well.
	float GetLength() const { return m_fLength;}
	float GetCompass() const {return m_fCompass;}
	float GetDepthAtFromStation() const {return m_fDepthAtFromStation;}
	float GetCalculationLength() const;
	float GetCalculationCompass() const;
	float GetCalculationClino() const;
	float GetCalculationDepth() const;

	void SetReadings(float fLength,float fCompass,float fDepth);

	//save and restore from file
    virtual void Write(CFileHelper * );
    virtual void ReadVersion1(CFileHelper *);
    virtual void ReadVersion2(CFileHelper *);

#ifdef _DEBUG
	static void SelfTest();
#endif
};



//This contains a list of survey shots.  It is used in several places.
class CSurveyShotArray:public CArray<CSurveyShot *,CSurveyShot *>
{
public:
// General access functions
//	  	int GetSize() const;
	void Remove(CSurveyShot *shot);
	
//helpers for saving and reading to files	
	void Write(CFileHelper *);
	//Return TRUE if this is a list of dive shots.
	//Throws an exception if the two are mixed.
	BOOL Read(CFileHelper *,CSurveyLeg *,BOOL * bBacksights);
//helper for cleanup
	void DeleteContents();

	//Searches through all of the stations in the
	//leg to find out what the depth of a station is.
	//Valid only if the Survey is a Dive Survey
	float FindCalculationDepth(LPCTSTR szStationName);
	
	void DoDeferredOffsetCalculation();

	//We allow extensive annotations of survey stations.
	//Comments allow you to association a type and a remark
	//with a station.  They can be used for GIS information, pitch
	//lengths, etc...  The plan is to expand the functionality to allow
	//widespread searching across these comments.
	BOOL QuerySurveyComments(CPtrArray * result,CQuery * pCommentQuery,CQuery * pTextQuery);
	BOOL QueryStationName(LPCTSTR szStationName);

	void ReadWalls(CFileHelper * h);
	void ReadComments(CFileHelper * h);
	CSurveyShot * FindOrAddShot(LPCTSTR szStation,float fCompass);
};



#endif

