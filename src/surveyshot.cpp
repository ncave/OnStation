#include "stdafx.h"
#include "onstation.h"
#include "surveyshot.h"
#include "nodetree.h"
#include "surveyleg.h"
#include "onstationdoc.h"
#include "filehelper.h"
#include "comment.h"
#include "query.h"
#include "settings.h"

extern COnStationDoc * pDocument_G;
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSettings * pSettings_G;
CSurveyShot::CSurveyShot(CSurveyLeg *Leg)
{
	ASSERT(Leg!=NULL);	 //we depend on this value all over the place
	m_Leg=Leg;
    m_ToNode=NULL;
    m_FromNode=NULL;
    m_fUp=NAN;
    m_fDown=NAN;
    m_fLeft=NAN;
    m_fRight=NAN;
	m_szFromStationName[0]=_T('\0');
	m_szToStationName[0]=_T('\0');
	m_wShotFlags=0;
}

void CSurveyShot::AddComment(LPCTSTR szText)
{
	TCHAR szType[512];
	//truncate the buffer
	lstrcpyn(szType,szText,_countof(szType));
	for (int i=0;i<_countof(szType);i++)
	{
		if (szType[i]==0)
		{
			CComment * pComment=new CComment(m_szFromStationName, _T(""),szType);
			m_Comments.Add(pComment);
			return;
		}
		if (szText[i]==_T(':'))
		{
			szType[i]=_T('\0');
			CComment * pComment=new CComment(m_szFromStationName,szType,szType+i+1);
			m_Comments.Add(pComment);
			return;
		}
	}
}


BOOL CSurveyShot::QuerySurveyComments(CPtrArray * result,CQuery * pTypeQuery,CQuery * pTextQuery)
{
	BOOL bFoundAny=FALSE;
	for (int i=0;i<m_Comments.GetSize();i++)
	{
		CComment * pComment=m_Comments.GetAt(i);
		//TODO: Do the query here.
		if (pTypeQuery->MatchesQuery(pComment->GetType()) && pTextQuery->MatchesQuery(pComment->GetBody()))
		{
			CNode *node=pDocument_G->GetNodeTree()->FindNode(pComment->GetStationName());
			if (node!=NULL)
			{
				node->SetMatchesQuery(TRUE);
				bFoundAny=TRUE;
				if (result!=NULL)
				{
					//don't allow duplicate entries
					INT_PTR iCount=result->GetSize();
					BOOL bFound=FALSE;
					for (INT_PTR j=0;j<iCount;j++)
					{
						if (node==((CNode *)(result->GetAt(j))))
						{
							bFound=TRUE;
						}
					}
					if (!bFound)
					{
						result->Add((LPVOID)node);
					}
				}
			}
		}
	}
	return bFoundAny;
}

float CSurveyShot::GetStressDeviation()
{
	//.
	float fMax;
	CNodeTree * pNodes=pDocument_G->GetNodeTree();
	switch(pNodes->GetBlunderMode()){
		case 0:
			fMax=m_fStressDev;
			break;
		case 1:
			fMax=max(m_fStressCompass,m_fStressTape);
			fMax=max(fMax,m_fStressClino);
			break;
		case 2:
			fMax=m_fStressCompass;
			break;
		case 3:
			fMax=m_fStressClino;
			break;
		case 4:
			fMax=m_fStressTape;
			break;
	}

	return fMax;
}

void CSurveyShot::SetUDLR(float up,float dn,float l,float r)
{
 	m_fUp=up;
	m_fDown=dn;
	m_fLeft=l;
	m_fRight=r;
}


//Setting the names automatically adds us to the node tree
void CSurveyShot::SetStationNames(LPCTSTR szFrom,LPCTSTR szTo)
{
	ASSERT(lstrcmpi(szFrom,szTo)!=0);
    lstrcpyn(m_szFromStationName,szFrom,_countof(m_szFromStationName));
    lstrcpyn(m_szToStationName,szTo,_countof(m_szToStationName));
    GetLeg()->GetDocument()->GetNodeTree()->AddShot(this);
}

CSurveyShot::~CSurveyShot()
{
    GetLeg()->GetDocument()->GetNodeTree()->RemoveShot(this);
    m_ToNode=NULL;
    m_FromNode=NULL;
	for (int i=0;i<m_Comments.GetSize();i++)
	{
		CComment * pComment=m_Comments.GetAt(i);
		delete pComment;
	}
}


//Storage functions
void CSurveyShotArray::Write(CFileHelper * h)
{
	h->WriteToken(_T("Begin"), _T("Shots"));
    WORD iCount=(WORD)GetSize();
	for (WORD i=0;i<iCount;i++)	    
    {
		CSurveyShot *shot=GetAt(i);
		if (!shot->IsEmpty())
			shot->Write(h);
    }
	h->WriteToken(_T("End"), _T("Shots"));
	h->WriteToken(_T("Begin"), _T("Comments"));
	for (WORD i=0;i<iCount;i++)
	{
		CSurveyShot *shot=GetAt(i);
		shot->WriteComment(h);
	}
	h->WriteToken(_T("End"), _T("Comments"));
	h->WriteToken(_T("Begin"), _T("Walls"));
	for (WORD i=0;i<iCount;i++)
	{
		CSurveyShot *shot=GetAt(i);
		CStandardSurveyShot * pStandardShot=dynamic_cast<CStandardSurveyShot *>(shot);
		CCaveDiveSurveyShot * pDiveShot=dynamic_cast<CCaveDiveSurveyShot *>(shot);
		if (pStandardShot!=NULL)
		{
			h->WriteWalls(pStandardShot->GetFromStationName(),pStandardShot->GetCompass(),pStandardShot->GetClino(),pStandardShot->GetRawUp(),pStandardShot->GetRawDown(),pStandardShot->GetRawLeft(),pStandardShot->GetRawRight());
		}
		else
		{
			h->WriteWalls(pDiveShot->GetFromStationName(),pDiveShot->GetCompass(),0.0f,pDiveShot->GetRawUp(),pDiveShot->GetRawDown(),pDiveShot->GetRawLeft(),pDiveShot->GetRawRight());
		}
	}
	h->WriteToken(_T("End"), _T("Walls"));
}

//Send parent leg in as well
BOOL CSurveyShotArray::Read(CFileHelper *h,CSurveyLeg *leg,BOOL * pBacksights)
{
	BOOL bHasDiveShots=FALSE;
	BOOL bHasStandardShots=FALSE;
	CSurveyShot *pShot=NULL;
	try
	{
		h->ReadTokenBegin(_T("Shots"));
		while (TRUE)
		{
			pShot=NULL;
			LPCTSTR szData=h->GetTokenLookaheadString(TRUE);
			if (lstrcmpi(szData, _T("DiveShot"))==0)
			{
				pShot=new CCaveDiveSurveyShot(leg);
				bHasDiveShots=TRUE;
			}
			if (lstrcmpi(szData, _T("Shot"))==0)
			{
				pShot=new CStandardSurveyShot(leg);
				bHasStandardShots=TRUE;
			}
			if (pShot!=NULL)
			{
				if (h->m_fVersion<2.0)
				{
					pShot->ReadVersion1(h);
				}
				else
				{
					pShot->ReadVersion2(h);
				}
				Add(pShot);
				CStandardSurveyShot * pShotTemp=dynamic_cast<CStandardSurveyShot *>(pShot);
				if (pShotTemp && (pShotTemp->GetCompass2()!=NAN || pShotTemp->GetClino2()!=NAN))
				{
					*pBacksights=TRUE;
				}
			}
			else
			{
				h->ReadTokenEnd(_T("Shots"));
				if (bHasDiveShots && bHasStandardShots)
				{
					CString CS;
					CS.GetBuffer(200);
					CS.Format(IDS_MIXEDDIVEERROR,h->GetCurrentLine());
					AfxMessageBox(CS);
					throw(_T("mixed dive and regular data"));
				}
				if (h->m_fVersion>=2.0f)
				{
					ReadComments(h);
					ReadWalls(h);
				}
				return bHasDiveShots;
			}
		}
	}
	catch (...)
	{
		delete pShot;
		throw;
	}
	return FALSE;
}

void CSurveyShotArray::Remove(CSurveyShot *shot)
{
 	INT_PTR iCount=GetSize();
 	for (INT_PTR i=0;i<iCount;i++)
 	{
 		CSurveyShot *data=GetAt(i);
 		if (data==shot)
 		{
    		RemoveAt(i);
    		return;
    	}
    }
}


void CSurveyShotArray::DeleteContents()
{
	INT_PTR iCount=GetSize();
 	for (INT_PTR i=0;i<iCount;i++)
 	{
 		CSurveyShot *shot=GetAt(i);
 		delete shot;
    }
    RemoveAll();
}





void CStandardSurveyShot::ReadVersion1(CFileHelper *h)
{
	float fLength,fCompass,fClinometer,fUp,fDown,fLeft,fRight,fCompass2,fClinometer2;
	WORD wFlags;
	TCHAR szFromStationName[MAX_STATION_NAME_LENGTH];
	TCHAR szToStationName[MAX_STATION_NAME_LENGTH];
	h->ReadStandardShotVersion1(szFromStationName,szToStationName,&fLength,&fCompass,&fClinometer,
				 &fUp,&fDown,&fLeft,&fRight,&fCompass2,&fClinometer2,&wFlags);
	SetShotFlags(wFlags);
	SetReadings(fLength,fCompass,fClinometer,fCompass2,fClinometer2);
	SetUDLR(fUp,fDown,fLeft,fRight);
	SetStationNames(szFromStationName,szToStationName);		//This adds us to the global data structure

	LPCTSTR szData=h->GetTokenLookaheadString(TRUE);
	while (lstrcmpi(szData, _T("ShotComment"))==0)
	{
		LPCTSTR szComment=h->ReadTokenString(_T("ShotComment"));
		if (szComment[0]!=0)
		{
			AddComment(szComment);
		}
		szData=h->GetTokenLookaheadString(TRUE);
	}
}
void CStandardSurveyShot::ReadVersion2(CFileHelper *h)
{
	float fLength,fCompass,fClinometer,fCompass2,fClinometer2;
	WORD wFlags;
	TCHAR szFromStationName[MAX_STATION_NAME_LENGTH];
	TCHAR szToStationName[MAX_STATION_NAME_LENGTH];
	h->ReadStandardShotVersion2(szFromStationName,szToStationName,&fLength,&fCompass,&fClinometer,
				 &fCompass2,&fClinometer2,&wFlags);
	SetShotFlags(wFlags);
	SetReadings(fLength,fCompass,fClinometer,fCompass2,fClinometer2);
	SetStationNames(szFromStationName,szToStationName);		//This adds us to the global data structure
}

void CStandardSurveyShot::Write(CFileHelper * h)
{
	h->WriteStandardShot(m_szFromStationName,m_szToStationName,m_fLength,m_fCompass,m_fClinometer,
				 m_fCompass2,m_fClinometer2,m_wShotFlags);
}

CStandardSurveyShot::CStandardSurveyShot(CSurveyLeg *pLeg):CSurveyShot(pLeg)
{
    m_fCompass=NAN;
	m_fCompass2=NAN;
    m_fClinometer=NAN;
	m_fClinometer2=NAN;				    
    m_fLength=NAN;
}
CStandardSurveyShot::~CStandardSurveyShot()
{
}

void CStandardSurveyShot::SetReadings(float Length,float Compass,float Clino,float Compass2,float Clino2)
{
	ASSERT(Clino2==NAN || (Clino2<100.0f && Clino2>-100.0f));
	//There must be a clino reading for this to be valid.
	ASSERT(Length==NAN || (Clino!=NAN || Clino2!=NAN));
	if (Length!=NAN && Clino==NAN && Clino2==NAN)
	{
		Clino=0.0f;
		TRACE(_T("Had to fake a clino reading for missing shots\n"));
	}
    m_fLength=Length;
    m_fCompass=Compass;
    m_fClinometer=Clino;
	m_fClinometer2=Clino2;
	m_fCompass2=Compass2;
	//On Station assumes a compass even for vertical shots
	if (m_fCompass2==NAN && m_fCompass==NAN && (m_fClinometer!=NAN || m_fClinometer2!=NAN))
	{
		TRACE(_T("Found clino with no compass.  Correcting!\n"));
		m_fCompass=0.0f;
	}
	if (m_fCompass2!=NAN && m_fCompass!=NAN)
	{
		//Sometimes backsights are put in as front sights.  Catch this mistake here
		if (WithinNDegrees(m_fCompass2,m_fCompass,20.0f))
		{
			m_fCompass2=m_fCompass2+180.0f;
			if (m_fCompass2>=360.0f)
			{
				m_fCompass2=m_fCompass2-360.0f;
			}
		}
	}
}
float CStandardSurveyShot::GetCalculationCompass() const
{
	//This is possible if we have a wall shot at the end of a leg
	if (m_fCompass==NAN && m_fCompass2==NAN)
	{
		return NAN;
	}
	float compass;
	if (m_fCompass2==NAN)
	{
    	compass=(m_fCompass+m_Leg->GetCompass1Error());
	}
	else
	{
		if (m_fCompass!=NAN)
		{
			compass=GetAngleBetween(m_fCompass+m_Leg->GetCompass1Error(),m_fCompass2+m_Leg->GetCompass2Error()+180.0f);
		}
		else
		{
			compass=m_fCompass2+m_Leg->GetCompass2Error()+180.0f;
		}
	}
	compass=compass+m_Leg->GetDeclination();
	if (compass>360.0f)
	{
		compass=compass-360.0f;
	}
	if (compass<0.0f)
	{
		compass=compass+360.0f;
	}
	return compass;
}

float CStandardSurveyShot::GetCalculationClino() const
{
	//This is possible if we have a wall shot at the end of a leg
	if (m_fClinometer==NAN && m_fClinometer2==NAN)
	{
		return NAN;
	}
	float clino;
	if (m_fClinometer2==NAN)
	{
    	clino=(m_fClinometer+m_Leg->GetClino1Error());
	}
	else
	{
		if (m_fClinometer!=NAN)
		{
			clino=(m_fClinometer+m_Leg->GetClino1Error()-m_fClinometer2-m_Leg->GetClino2Error())/2.0f;
		}
		else
		{
			clino=-m_fClinometer2-m_Leg->GetClino2Error();
		}
	}
	if (clino>90.0f)
	{
		TRACE(_T("Corrected out of range clino\n"));
		clino=90.0f;
	}
	if (clino<-90.0f)
	{
		TRACE(_T("Corrected out of range clino\n"));
		clino=-90.0f;
	}
	return clino;
}

float CCaveDiveSurveyShot::GetCalculationDepth() const
{
	float fDepth=m_fDepthAtFromStation;
	if (fDepth==NAN)
	{
		return 0.0f;
	}
	if (m_Leg->GetDiveMeters()==FALSE)
	{
		fDepth=FeetToMeters(fDepth);
	}
	ASSERT(fDepth>-1000.0f && fDepth<1000.0f);
	return fDepth;
}

float CSurveyShot::GetHorizontalLength() const
{
	double clino=(double)GetCalculationClino();
    double len=(double)GetCalculationLength();
	if (clino!=NAN && len!=NAN)
	{
		clino=clino*PI/180.0;
		float retval=(float)(len*cos(clino));
		if (retval<0.0f)
		{
			retval=-retval;
		}
		return retval;
	}
	return 0.0f;
}

float CStandardSurveyShot::GetCalculationLength() const
{
	float len=m_fLength;
	if (len==NAN)
	{
		//this could happen when we have shots defineing the walls only
		//at the end of a survey.  We set it to zero so that when we
		//get summary information the totals are correct.
		return 0.0f;	
	}
    if (m_Leg->GetMeters()==FALSE)
    {
    	len=FeetToMeters((float)len);
    }
	len=len+m_Leg->GetTapeError();
	ASSERT(len<50000.0f);
	return len;
}

void CSurveyShot::CalculateDeltas()
{
    double len=(double)GetCalculationLength();
	double compass=(double)GetCalculationCompass();
	double clino=(double)GetCalculationClino();

	//Clino and compass may be NAN if this is simply
	//the terminating point of a leg.
	if (compass!=NAN && clino!=NAN)
	{
		compass=compass*PI/180.0;
		clino=clino*PI/180.0;
    
		double lenxy=len*cos(clino);
		double lenz=len*sin(clino);
		double leny=lenxy*cos(compass);
		double lenx=lenxy*sin(compass);

		float f1,f2,f3;
		f1=(float)lenx;    
		f2=(float)leny;
		f3=(float)lenz;
		m_Delta.Set(f1,f2,f3);
	}
}

CCaveDiveSurveyShot::CCaveDiveSurveyShot(CSurveyLeg * pLeg):CSurveyShot(pLeg)
{
    m_fLength=NAN;
    m_fCompass=NAN;
	m_fDepthAtFromStation=NAN;
}

CCaveDiveSurveyShot::~CCaveDiveSurveyShot()
{
}

float CCaveDiveSurveyShot::GetCalculationLength() const
{
	float len=m_fLength;
	if (len==NAN)
		return 0.0f;

    if (m_Leg->GetMeters()==FALSE)
    {
    	len=FeetToMeters((float)len);
    }
	len=len+m_Leg->GetTapeError();
	return len;
}

float CCaveDiveSurveyShot::GetCalculationCompass() const
{
	float compass;
   	compass=(m_fCompass+m_Leg->GetCompass1Error());
	compass=compass+m_Leg->GetDeclination();
	if (compass>360.0f)
	{
		compass=compass-360.0f;
	}
	if (compass<0.0f)
	{
		compass=compass+360.0f;
	}
	return compass;
}


void CCaveDiveSurveyShot::SetReadings(float fLength,float fCompass,float fDepthAtFromStation)
{
	m_fLength=fLength;
	m_fCompass=fCompass;
	//Allow the user to simply type in the depth
	//gauge reading.  This is due to a request from
	//a cave diver who is an On Station user.
	if (fDepthAtFromStation>0.0f)
	{
		fDepthAtFromStation=-fDepthAtFromStation;
	}
	m_fDepthAtFromStation=fDepthAtFromStation;
}

void CCaveDiveSurveyShot::ReadVersion1(CFileHelper *h)
{
	float fLength,fCompass,fDepth,fUp,fDown,fLeft,fRight;
	TCHAR szFromStationName[MAX_STATION_NAME_LENGTH];
	TCHAR szToStationName[MAX_STATION_NAME_LENGTH];
	WORD wFlags;
	h->ReadCaveDiveShotVersion1(szFromStationName,szToStationName,&fLength,&fCompass,&fDepth,&fUp,&fDown,&fLeft,&fRight,&wFlags);
	SetShotFlags(wFlags);
	SetReadings(fLength,fCompass,fDepth);
	SetUDLR(fUp,fDown,fLeft,fRight);
	SetStationNames(szFromStationName,szToStationName);		//This adds us to the global data structure

	LPCTSTR szData=h->GetTokenLookaheadString(TRUE);
	while (lstrcmpi(szData, _T("ShotComment"))==0)
	{
		LPCTSTR szComment=h->ReadTokenString(_T("ShotComment"));
		if (szComment[0]!=0)
		{
			AddComment(szComment);
		}
		szData=h->GetTokenLookaheadString(TRUE);
	}
}
void CCaveDiveSurveyShot::ReadVersion2(CFileHelper *h)
{
	float fLength,fCompass,fDepth;
	TCHAR szFromStationName[MAX_STATION_NAME_LENGTH];
	TCHAR szToStationName[MAX_STATION_NAME_LENGTH];
	WORD wFlags;
	h->ReadCaveDiveShotVersion2(szFromStationName,szToStationName,&fLength,&fCompass,&fDepth,&wFlags);
	SetShotFlags(wFlags);
	SetReadings(fLength,fCompass,fDepth);
	SetStationNames(szFromStationName,szToStationName);		//This adds us to the global data structure
}

void CCaveDiveSurveyShot::Write(CFileHelper * h)
{
	h->WriteDiveShot(m_szFromStationName,m_szToStationName,m_fLength,m_fCompass,m_fDepthAtFromStation,m_wShotFlags);
}


void CSurveyShot::WriteComment(CFileHelper *h)
{
	for (int i=0;i<m_Comments.GetSize();i++)
	{
		CComment * pComment=m_Comments.GetAt(i);
		CString CS=GetFromStationName();
		CS+= _T(",");
		CS+=pComment->GetType();
		CS+= _T(":");
		CS+=pComment->GetBody();
		h->WriteToken(_T("Comment"),CS);
	}
}


#ifdef _DEBUG
void CCaveDiveSurveyShot::SelfTest()
{
}

#endif

float CCaveDiveSurveyShot::GetCalculationClino() const
{
	float fOtherDepth=m_Leg->GetShotArray()->FindCalculationDepth(GetToStationName());
	if (fOtherDepth==NAN)
	{
		fOtherDepth=GetCalculationDepth();
	}
	double fOH=(double)((fOtherDepth-GetCalculationDepth())/GetCalculationLength());
	//Check if length was shorter then depth difference.
	if (fOH>1.0)
	{
		fOH=1.0;
	}
	if (fOH<-1.0)
	{
		fOH=-1.0;
	}
	float fAngle=(float)(asin(fOH)*180.0/PI);
	return fAngle;
}

float CSurveyShotArray::FindCalculationDepth(LPCTSTR szStationName)
{
    WORD iCount=(WORD)GetSize();
	for (WORD i=0;i<iCount;i++)	    
    {
		CCaveDiveSurveyShot *pShot=dynamic_cast<CCaveDiveSurveyShot *>(GetAt(i));
		if (pShot!=NULL && lstrcmpi(pShot->GetFromStationName(),szStationName)==0)
		{
			return pShot->GetCalculationDepth();
		}
    }
	return NAN;
}

void CSurveyShotArray::DoDeferredOffsetCalculation()
{
    WORD iCount=(WORD)GetSize();
	for (WORD i=0;i<iCount;i++)	    
    {
		CSurveyShot *pShot=GetAt(i);
		pShot->CalculateDeltas();
	}
}

BOOL CSurveyShotArray::QuerySurveyComments(CPtrArray * result,CQuery * pCommentQuery,CQuery * pTextQuery)
{
    BOOL bMatches=FALSE;
	WORD iCount=(WORD)GetSize();
	for (WORD i=0;i<iCount;i++)	    
    {
		CSurveyShot *pShot=GetAt(i);
		if (pShot->QuerySurveyComments(result,pCommentQuery,pTextQuery))
		{
			bMatches=TRUE;
		}
	}
	return bMatches;
}

//An alternate and elegant approach would have been to add all the station
//names into one big string and to a regular query on that.  The
//problem is that string concatenation is slow because it involves constant
//reallocs.  It took 5 minutes to do a 20km system.
BOOL CSurveyShotArray::QueryStationName(LPCTSTR szStationName)
{
    WORD iCount=(WORD)GetSize();
	for (WORD i=0;i<iCount;i++)	    
    {
		CSurveyShot *pShot=GetAt(i);
		if (lstrcmpi(pShot->GetFromStationName(),szStationName)==0)
		{
			return TRUE;
		}
		if (lstrcmpi(pShot->GetToStationName(),szStationName)==0)
		{
			return TRUE;
		}
	}
	return FALSE;
}

float CSurveyShot::GetUp() const
{
	if (pSettings_G->m_PassageType[pSettings_G->m_iColorSchemeIndex]==PT_FULL_PASSAGES_SKINNY)
	{
		return 1.0f;
	}
	else
	{
		return (m_fUp!=NAN && !EmptyCrossSection())?m_fUp:1.0f;
	}
}

float CSurveyShot::GetDown() const
{
	if (pSettings_G->m_PassageType[pSettings_G->m_iColorSchemeIndex]==PT_FULL_PASSAGES_SKINNY)
	{
		return 1.0f;
	}
	else
	{
		return (m_fDown!=NAN && !EmptyCrossSection())?m_fDown:1.0f;
	}
}

float CSurveyShot::GetLeft() const
{
	if (pSettings_G->m_PassageType[pSettings_G->m_iColorSchemeIndex]==PT_FULL_PASSAGES_SKINNY)
	{
		return 1.0f;
	}
	else
	{
		return (m_fLeft!=NAN&& !EmptyCrossSection())?m_fLeft:1.0f;
	}
}

float CSurveyShot::GetRight() const
{
	if (pSettings_G->m_PassageType[pSettings_G->m_iColorSchemeIndex]==PT_FULL_PASSAGES_SKINNY)
	{
		return 1.0f;
	}
	else
	{
		return (m_fRight!=NAN && !EmptyCrossSection())?m_fRight:1.0f;
	}
}
#ifdef _DEBUG
void CSurveyShot::Dump()
{
	TRACE(_T("Shot from %s to %s\n"),GetFromStationName(),GetToStationName());
}
#endif

CSurveyShot * CSurveyShotArray::FindOrAddShot(LPCTSTR szStation,float fCompass)
{
	int iBestFromMatch=-1;
	float fToMatchCompass=NAN;
	int iBestToMatch=-1;
	float fFromMatchCompass=NAN;
    WORD iCount=(WORD)GetSize();
	if (iCount==0)
	{
		return NULL;
	}
	for (WORD i=0;i<iCount;i++)	    
    {
		CSurveyShot *shot=GetAt(i);
		if (lstrcmpi(shot->GetFromStationName(),szStation)==0)
		{
			iBestFromMatch=i;
		}
		if (lstrcmpi(shot->GetToStationName(),szStation)==0)
		{
			iBestToMatch=i;	
		}
    }
	if (iBestFromMatch!=-1)
	{
		return GetAt(iBestFromMatch);
	}
	CSurveyShot * pShot;
	if (dynamic_cast<CCaveDiveSurveyShot *>(GetAt(0))!=NULL)
	{
		pShot=new CCaveDiveSurveyShot(GetAt(0)->GetLeg());
		pShot->SetStationNames(szStation, _T(""));
	}
	else
	{
		pShot=new CStandardSurveyShot(GetAt(0)->GetLeg());
		pShot->SetStationNames(szStation, _T(""));
	}
	if (iBestToMatch!=-1)
	{
		InsertAt(iBestToMatch+1,pShot);
		return pShot;
	}
	else
	{
		TRACE(_T("Shot %s was not in this survey!\n"),szStation);
		ASSERT(FALSE);	//might be an error of some sort
		return NULL;
	}
}


void CSurveyShotArray::ReadWalls(CFileHelper * h)
{
	h->ReadTokenBegin(_T("Walls"));
	while (TRUE)
	{
		LPCTSTR szData=h->GetTokenLookaheadString(TRUE);
		if (lstrcmpi(szData, _T("Wall"))==0)
		{
			TCHAR szStation[128];
			float fUp,fDown,fLeft,fRight,fCompass,fClino;
			h->ReadWalls(szStation,&fCompass,&fClino,&fUp,&fDown,&fLeft,&fRight);
			CSurveyShot * pShot=FindOrAddShot(szStation,fCompass);
			if (pShot!=NULL)
			{
				pShot->SetUDLR(fUp,fDown,fLeft,fRight);
			}
		}
		else
		{
			h->ReadTokenEnd(_T("Walls"));
			return;
		}
	}
}

void CSurveyShotArray::ReadComments(CFileHelper *h)
{
	h->ReadTokenBegin(_T("Comments"));
	while (TRUE)
	{
		LPCTSTR szData=h->GetTokenLookaheadString(TRUE);
		if (lstrcmpi(szData, _T("Comment"))==0)
		{
			TCHAR szComment[1024];
			TCHAR szShot[1024];
			szComment[0]=_T('\0');
			LPCTSTR szToken=h->ReadTokenString(_T("Comment"));
			int i=0;
			while (szToken[i]!=0 && szToken[i]!=_T(','))
			{
				szShot[i]=szToken[i];
				i++;
			}
			szShot[i]=_T('\0');
			if (szToken[i]!=0)
			{
				lstrcpyn(szComment,szToken+i+1,_countof(szComment)-1);
			}
			CSurveyShot * pShot=FindOrAddShot(szShot,NAN);
			if (pShot!=NULL)
			{
				pShot->AddComment(szComment);		
			}
		}
		else
		{
			h->ReadTokenEnd(_T("Comments"));
			return;
		}
	}
}

