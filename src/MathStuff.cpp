#include "stdafx.h"
#include "stdio.h"
#include "mathstuff.h"
#include "onstation.h"
#include "settings.h"

//EXPLANATION OF UNITS
//	X goes from left to right
//	Y goes from bottom to top
//  Z goes from under the screen to out of the screen.
//
// Rotate X turns the image around the horizontal line through the
//		middle of the screen.
// Rotate Y turns the image round the vertical line through the
//		middle of the screen.
// Rotate Z turns the image clockwise and counterclockwise around
//		the point in the middle of the screen.

extern COnStationApp theApp;
const float PERSPECTIVEWARP=-0.001f;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BOOL FloatFromString(LPCTSTR sz,float &f)
{
    if (sz[0]==0)
    {
      	f=NAN;
        return TRUE;			//Empty
    }
    double df;
    if (_stscanf_s(sz, _T("%lf"),&df)!=1)
    {
    	f=NAN;
		return FALSE;
    }
    f=(float)df;
    return TRUE;
}


void FloatToString(CString & szReturn,float f)
{
    TCHAR szBuffer[15];
    if (f==NAN)
    {
    	szBuffer[0]=_T('\0');
		szReturn=szBuffer;
    	return;
    }
    _stprintf_s(szBuffer, _T("%.2f"),f);
    szReturn=szBuffer;
}

CString FloatToString(float f)
{
	CString value;
	FloatToString(value, f);
	return value;
}

CString IntToString(int i)
{
	TCHAR buff[10];
	
	_stprintf_s(buff, _T("%d"), i);
	
	return CString(buff);
}

// =========================================================
// =========================================================
// =========================================================
//Position Matrix
// =========================================================
// =========================================================
// =========================================================
CPosMatrix::CPosMatrix()
{
	m_Items[0]=0.0f;
	m_Items[1]=0.0f;
	m_Items[2]=0.0f;
	m_Items[3]=0.0f;
}

BOOL CPosMatrix::IsEqual(CPosMatrix & p2)
{
	if (m_Items[0]!=p2.GetX() || 
		m_Items[1]!=p2.GetY() || 
		m_Items[2]!=p2.GetZ())
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

//Calculates the position of a point on the line between this matrix and the matrix
//"To" given that position's Z value.  Returns the result in ret;
void CPosMatrix::CalculateIntermediatePosFromZ(float fTargetDepth,CPosMatrix & To,CPosMatrix &ret)
{
	ASSERT(GetZ()>=fTargetDepth && To.GetZ()<=fTargetDepth);	//should be between and from higher then to
	float fDeltaZ=fTargetDepth-GetZ();
	if (fDeltaZ==0.0)	//No Z position change
	{
		ret=*this;
		return;
	}
	float fTotalDeltaZ=GetZ()-To.GetZ();
	float fTotalDeltaX=GetX()-To.GetX();
	float fTotalDeltaY=GetY()-To.GetY();
	float fPercent=fDeltaZ/fTotalDeltaZ;
	ret.Set(GetX()+fTotalDeltaX*fPercent,GetY()+fTotalDeltaY*fPercent,fTargetDepth);
}

void CPosMatrix::Set(float f1,float f2,float f3)
{
	//It is nearly impossible to catch bad data once it is in.
	//it simply fucks up everything else.
	ASSERT(f1!=NAN && f1<50000000.0f && f1>-50000000.0f);
	ASSERT(f2!=NAN && f2<50000000.0f && f2>-50000000.0f);
	ASSERT(f3!=NAN && f3<50000000.0f && f3>-50000000.0f);

	m_Items[0]=f1;
	m_Items[1]=f2;
	m_Items[2]=f3;
	m_Items[3]=1.0f;
}

// Normalize the vector.  This is for scaling transformations.
//
void CPosMatrix::Normalize()
{
    ASSERT(m_Items[3]!=0.0);
 
    m_Items[0]=m_Items[0]/m_Items[3];
    m_Items[1]=m_Items[1]/m_Items[3];
    m_Items[2]=m_Items[2]/m_Items[3];
    m_Items[3]=1.0f;
}
void CPosMatrix::Add(CPosMatrix *p,const CPosMatrix & r) const
{
	ASSERT(r.m_Items[3]==1.0);		//must both be normalized!
	ASSERT(m_Items[3]==1.0);
	p->m_Items[0]=m_Items[0]+r.m_Items[0];
	p->m_Items[1]=m_Items[1]+r.m_Items[1];
	p->m_Items[2]=m_Items[2]+r.m_Items[2];
	p->m_Items[3]=1.0f;		//Keep it normalizeds
}
void CPosMatrix::Subtract(CPosMatrix *p,const CPosMatrix & r) const
{
	ASSERT(r.m_Items[3]==1.0);		//must both be normalized!
	ASSERT(m_Items[3]==1.0);
	p->m_Items[0]=m_Items[0]-r.m_Items[0];
	p->m_Items[1]=m_Items[1]-r.m_Items[1];
	p->m_Items[2]=m_Items[2]-r.m_Items[2];
	p->m_Items[3]=1.0f;		//Keep it normalized
}

void CPosMatrix::Multiply(CPosMatrix *p,const CViewMatrix &r) const
{
	//Use temporary so we can assign results to ourselves.
	//NOTE:  ON A P90 this routine benchmarks at about .5
	//million matrixes and normalizes per second in the release
	//build.  Unrolling the loop below gives a 6% speed
	//improvement
	float fTemp[4];
	ASSERT(m_Items[3]==1.0);

	for (int i=0;i<4;i++)
	{
		fTemp[i]=m_Items[0]*r.m_Items[0][i]+
					 m_Items[1]*r.m_Items[1][i]+
					 m_Items[2]*r.m_Items[2][i]+
					 m_Items[3]*r.m_Items[3][i];							
	}
	for (int i=0;i<4;i++)
	{
		p->m_Items[i]=fTemp[i];
	}
//	p->Dump();
}

CPosMatrix & CPosMatrix::operator=(const CPosMatrix & r)
{
	m_Items[0]=r.m_Items[0];
	m_Items[1]=r.m_Items[1];
	m_Items[2]=r.m_Items[2];
	m_Items[3]=r.m_Items[3];
	return *this;
}		
/*
void CPosMatrix::Dump() const
{
	TCHAR szTemp[100];
	_stprintf_s(szTemp, _T("%.2f %.2f %.2f %.2f\n"),m_Items[0],m_Items[1],m_Items[2],m_Items[3]);
	TRACE(szTemp);
	TRAC(_T("---------------------------------\n"));
}
*/

// =========================================================
// =========================================================
// =========================================================
// =========================================================
// VIEW MATRIX
// =========================================================
// =========================================================
// =========================================================
// =========================================================

CViewMatrix::CViewMatrix()
{
	for (int i=0;i<4;i++)
	{
		for (int j=0;j<4;j++)
		{
			if (i==j)
			{
				m_Items[i][j]=1.0f;
			}
			else
			{
				m_Items[i][j]=0.0f;
			}
		}
	}
}

/*
void CViewMatrix::Dump() const
{
	TCHAR szTemp[100];
	for (int i=0;i<4;i++)
	{
		_stprintf_s(szTemp, _T("%.2f %.2f %.2f %.2f\n"),m_Items[i][0],m_Items[i][1],m_Items[i][2],m_Items[i][3]);
		OutputDebugString(szTemp);
	}
	OutputDebugStrin(_T("---------------------------------\n"));
}
*/

CViewMatrix & CViewMatrix::operator=(const CViewMatrix &r)
{
	for (int i=0;i<4;i++)
	{
		for (int j=0;j<4;j++)
		{
			m_Items[i][j]=r.m_Items[i][j];
		}
	}
	return *this;
}

void CViewMatrix::Multiply(CViewMatrix *result,const CViewMatrix &r) const
{
	ASSERT(result!=this);	//can't multiply to self for this one.
	for (int i=0;i<4;i++)
	{
		for (int j=0;j<4;j++)
		{
			//Cell i,j = row i * column j
			result->m_Items[i][j]=m_Items[i][0]*r.m_Items[0][j]+
						  m_Items[i][1]*r.m_Items[1][j]+
						  m_Items[i][2]*r.m_Items[2][j]+
						  m_Items[i][3]*r.m_Items[3][j];
		}
	}
//	result->Dump();
}

void CViewMatrix::XPan(float distance)
{
	CViewMatrix m;
	m.m_Items[0][0]=1.0f;
	m.m_Items[1][1]=1.0f;
	m.m_Items[2][2]=1.0f;
	m.m_Items[3][3]=1.0f;
	m.m_Items[3][0]=distance;

	CViewMatrix Result;
	Multiply(&Result,m);
	*this=Result;
}

void CViewMatrix::YPan(float distance)
{
	CViewMatrix m;
	m.m_Items[0][0]=1.0f;
	m.m_Items[1][1]=1.0f;
	m.m_Items[2][2]=1.0f;
	m.m_Items[3][3]=1.0f;
	m.m_Items[3][1]=distance;

	CViewMatrix Result;
	Multiply(&Result,m);
	*this=Result;
}

void CViewMatrix::ZPan(float distance)
{
	CViewMatrix m;
	m.m_Items[0][0]=1.0f;
	m.m_Items[1][1]=1.0f;
	m.m_Items[2][2]=1.0f;
	m.m_Items[3][3]=1.0f;
	m.m_Items[3][2]=distance;

	CViewMatrix Result;
	Multiply(&Result,m);
	*this=Result;
}

void CViewMatrix::ChangeMetersPerCM(float fMetersPerCM)
{
	float zoomfactor=1.0f/fMetersPerCM;
	float fSize=m_Items[3][3]=m_Items[3][3]*zoomfactor;
	if (m_Items[3][3]<MINIMUMZOOM)
	{
		m_Items[3][3]=MINIMUMZOOM;
	}
	if (m_Items[3][3]>MAXIMUMZOOM)
	{
		m_Items[3][3]=MAXIMUMZOOM;
	}
}
    	
void CViewMatrix::RotateX(float deg)
{
    double degree=(double)(deg);
    degree=degree*PI/180.0;
	double mycos=cos(degree);
	double mysin=sin(degree);

	CViewMatrix m;
	m.m_Items[0][0]=1.0f;
	m.m_Items[3][3]=1.0f;
	
	m.m_Items[1][1]=(float)mycos;
	m.m_Items[2][2]=(float)mycos;
	m.m_Items[2][1]=(float)(-1.0*mysin);
	m.m_Items[1][2]=(float)mysin;

	CViewMatrix Result;
	Multiply(&Result,m);
	*this=Result;
}

void CViewMatrix::RotateZ(float deg)
{
    double degree=(double)(deg);
    degree=degree*PI/180.0;
	double mycos=cos(degree);
	double mysin=sin(degree);


	CViewMatrix m;

	m.m_Items[0][0]=(float)mycos;
	m.m_Items[1][1]=(float)mycos;
	m.m_Items[1][0]=(float)(-1.0*mysin);
	m.m_Items[0][1]=(float)mysin;
	m.m_Items[2][2]=1.0f;
	m.m_Items[3][3]=1.0f;

	CViewMatrix Result;
	Multiply(&Result,m);
	*this=Result;
}


//Right now we just go back to the identity matrix.
void CViewMatrix::ResetRotations()
{
	for (int i=0;i<4;i++)
	{
		for (int j=0;j<4;j++)
		{		
			if (i==j)
			{
				m_Items[i][j]=1.0f;
			}
			else
			{
				m_Items[i][j]=0.0f;
			}
		}
	}
}


void CViewMatrix::SetPerspective()
{
	CViewMatrix m;
	m.m_Items[0][0]=1.0f;
	m.m_Items[1][1]=1.0f;
	m.m_Items[2][2]=1.0f;
	m.m_Items[3][3]=1.0f;
	m.m_Items[2][3]=PERSPECTIVEWARP;		//Z position
//	m.m_Items[1][3]=PERSPECTIVEWARP;

	CViewMatrix Result;
	Multiply(&Result,m);
//	Multiply(&m,Result);
	*this=Result;
//	*this=m;
}


void CViewMatrix::RotateY(float deg)
{
    double degree=(double)(deg);
    degree=degree*PI/180.0;
	double mycos=cos(degree);
	double mysin=sin(degree);

	CViewMatrix m;

	m.m_Items[0][0]=(float)mycos;
	m.m_Items[2][2]=(float)mycos;
	
	m.m_Items[0][2]=(float)(mysin*-1.0);
	m.m_Items[2][0]=(float)mysin;
	m.m_Items[1][1]=1.0f;
	m.m_Items[3][3]=1.0f;

	CViewMatrix Result;
	Multiply(&Result,m);
	*this=Result;
}


//Change scaling parameters on the matrix so that the given position
//will be at 0,0,0 on the screen.  This is all very simple math.
//
//This is centered on the z axis in order that we rotate around this point.
void CViewMatrix::CenterOn(CPosMatrix *p)
{
	float f1=m_Items[0][0]*p->GetX()+m_Items[1][0]*p->GetY()+m_Items[2][0]*p->GetZ();
	float f2=m_Items[0][1]*p->GetX()+m_Items[1][1]*p->GetY()+m_Items[2][1]*p->GetZ();
	float f3=m_Items[0][2]*p->GetX()+m_Items[1][2]*p->GetY()+m_Items[2][2]*p->GetZ();
	m_Items[3][0]=-1.0f*f1;
	m_Items[3][1]=-1.0f*f2;
}


//Go ahead...
DWORD MakeMyDate(int iYear,int iMonth,int iDay)
{
	if (iYear>60 && iYear<100)
		iYear=iYear+1900;

	if (iYear<1800 || iYear>2200 || iMonth<=0 || iMonth>12 || iDay<=0 || iDay>31)
		return 0xffffffff;
	return MAKELONG((WORD)iDay+(WORD)(iMonth*256),(WORD)iYear);
}
	

void DWordToDate(DWORD dwDate,LPTSTR szBuffer, int bufferLength)
{
	if (dwDate==0)
	{
		szBuffer[0]=_T('\0');
	}
	else
	{
		WORD wYear=GetYear(dwDate);
		WORD wMonth=GetMonth(dwDate);
		WORD wDay=GetDay(dwDate);
		_stprintf_s(szBuffer, bufferLength, _T("%.4i,%.2i,%.2i"),(int)wYear,(int)wMonth,(int)wDay);
	}
}

WORD GetYear(DWORD dwDate)
{
	return HIWORD(dwDate);
}
WORD GetMonth(DWORD dwDate)
{
	return LOWORD(dwDate)/256;
}
WORD GetDay(DWORD dwDate)
{
	return LOWORD(dwDate)%256;
}
DWORD DWordFromSEFDateString(LPCTSTR szDateString)
{
	int iYear;
	int iMonth;
	int iDay;
	if (szDateString[0]==0)
		return 0;				//empty string.
	//Wind cave has a lot of stuff in Month Day year format.  This ASSERT
	//below will go off if SMAPS is fairly free form in what it accepts
	TCHAR szDate[128];
	lstrcpyn(szDate,szDateString,_countof(szDate)-1);
	//Scanf doesn't like the - signs in the original date nor does it like the
	//leading zeros.
	if (szDate[0]==_T('0'))
		szDate[0]=_T(' ');
	if (szDate[3]==_T('0'))
		szDate[3]=_T(' ');
	szDate[2]=_T(' ');
	szDate[5]=_T(' ');
	_stscanf_s(szDate, _T("%i %i %i"),&iMonth,&iDay,&iYear);
	ASSERT(iMonth>=1 && iMonth<=12);
	ASSERT(iDay>=1 && iDay<=31);
	DWORD dw=MakeMyDate(iYear,iMonth,iDay);
	ASSERT(dw!=0xffffffff);
	return dw;
}

DWORD DWordFromDateString(LPCTSTR szDateString)
{
	int iYear;
	int iMonth;
	int iDay;
	if (szDateString[0]==0)
		return 0;				//empty string.

	
	TCHAR szBuffer[128];
	lstrcpyn(szBuffer,szDateString,_countof(szBuffer));
	int iLen=lstrlen(szBuffer);
	BOOL bDoneOne=FALSE;
	int i;
	for (i=0;i<iLen && !bDoneOne;i++)
	{
		if (szBuffer[i]==_T('.') || szBuffer[i]==_T(' ') || szBuffer[i]==_T(',') || szBuffer[i]==_T('\\') || szBuffer[i]==_T('/'))
		{
			szBuffer[i]=_T('\0');
			iYear=_tstoi(szBuffer);
			bDoneOne=TRUE;
		}
	}
	if (bDoneOne==FALSE)
		return 0xffffffff;
	bDoneOne=FALSE;
	int j;
	for (j=i;j<iLen && !bDoneOne;j++)
	{
		if (szBuffer[j]==_T('.') || szBuffer[j]==_T(' ') || szBuffer[j]==_T(',') || szBuffer[j]==_T('\\') || szBuffer[j]==_T('/'))
		{
			szBuffer[j]=_T('\0');
			iMonth=_tstoi(szBuffer+i);
			bDoneOne=TRUE;
		}
	}
	if (bDoneOne==FALSE)
		return 0xffffffff;
	iDay=_tstoi(szBuffer+j);	//Remainder

	DWORD dw=MakeMyDate(iYear,iMonth,iDay);
	return dw;
}

float GetAngleBetween(float fIncoming,float fOutgoing)
{
	float fDifference=fOutgoing-fIncoming;
	if (fDifference>180.0f)
	{
		fDifference=fDifference-360.0f;
	}
	if (fDifference<-180.0f)
	{
		fDifference=fDifference+360.0f;
	}
	float fReturn=fIncoming+fDifference/2.0f;
	if (fReturn<0.0f)
		fReturn=fReturn+360.0f;
	if (fReturn>360.0f)
		fReturn=fReturn-360.0f;
	return fReturn;
}


LPCTSTR GetPreferedUnitString(float f)
{
	//WE rotate through a series of static buffers thereby
	//allowing us to return a pointer to a string instead of
	//requiring a pointer to a buffer on startup
	static int iStringIndex=0;
	static TCHAR szBuffer[4][25];

	iStringIndex++;
	if (iStringIndex==4)
		iStringIndex=0;

	CString CS;

	switch (theApp.GetSettings()->m_PreferedUnits)
	{
	case METERSANDKM:
		if (f>=5000.0f || f<=-5000.0f)
		{
			CS.Format(IDS_2FKM,f/1000.0f);
			break;	//DON'T DROP THROUGH
		}
		//Drop through and do the meters one
	case METERS:
		CS.Format(IDS_1FM,f);
		break;

	case FEETANDMILES:
		if (MetersToMiles(f)>5.0 || MetersToMiles(f)<-5.0)
		{
			CS.Format(IDS_2FMILES,MetersToMiles(f));
			break;	//DON'T DROP THROUGH
		}
		//Drop through and use feet instead
	case FEET:
		CS.Format(IDS_1FFT,MetersToFeet(f));
		break;

	default:
		ASSERT(FALSE);
	}
	lstrcpy(szBuffer[iStringIndex],CS);
	return szBuffer[iStringIndex];
}


LPCTSTR GetPreferedScaleString(float f)
{
	static CString CS;
	if (theApp.GetSettings()->IsMetric())
	{
		CS.Format(IDS_2FMCM,f);
	}
	else
	{
		//m/cm*ft/m=ft/cm*cm/inch=ft
		CS.Format(IDS_2FFTINCH,MetersToFeet(f)*CMPERINCH);
	}
	return CS;
}


BOOL WithinNDegrees(float fOne,float fTwo,float fTolerance)
{
	float fDifference=fOne-fTwo;
	if (fDifference>360.0f-fTolerance)
	{
		return TRUE;
	}
	if (fDifference<fTolerance && fDifference>-fTolerance)
	{
		return TRUE;
	}
	if (fDifference<-360.0f+fTolerance)
	{
		return TRUE;
	}
	return FALSE;
}

int PixelsToTwips(int iTwips)
{
	HDC dc=::GetDC(NULL);
	int iOldMode=::SetMapMode(dc,MM_TWIPS);
	POINT ptBogus;
	ptBogus.x=iTwips;
	::DPtoLP(dc,&ptBogus,1);
	SetMapMode(dc,iOldMode);
	::ReleaseDC(NULL,dc);
	return ptBogus.x;
}

//From OpenGL superbible
void CalcNormal(float const *v1,float const *v2,float const *v3,float * out)
{
	float vt1[3],vt2[3];
	static const int x=0;
	static const int y=1;
	static const int z=2;

	//calculate 2 vectors from 3 points
	vt1[x]=v1[x]-v2[x];
	vt1[y]=v1[y]-v2[y];
	vt1[z]=v1[z]-v2[z];

	vt2[x]=v2[x]-v3[x];
	vt2[y]=v2[y]-v3[y];
	vt2[z]=v2[z]-v3[z];

	//Take the cross product
	out[x]=vt1[y]*vt2[z]-vt1[z]*vt2[y];
	out[y]=vt1[z]*vt2[x]-vt1[x]*vt2[z];
	out[z]=vt1[x]*vt2[y]-vt1[y]*vt2[x];

	ReduceToUnit(out);
}

void ReduceToUnit(float vector[3])
{
	float length;
	length=(float)sqrt(vector[0]*vector[0]+vector[1]*vector[1]+vector[2]*vector[2]);
	if (length==0.0f)
		length=1.0f;	//prevent divide by zero
	vector[0]/=length;
	vector[1]/=length;
	vector[2]/=length;
}
