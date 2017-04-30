#ifndef I_CMATHSTUFF__
#define I_CMATHSTUFF__

// Easier way to specify color for Windows bigots
#define glRGB(x, y, z)	glColor3ub((GLubyte)x, (GLubyte)y, (GLubyte)z)


const double PI=3.1415927;
#undef NAN
const float NAN=100000000000.0f;		//Numbers bigger then this are considered
									//invalid
const float MINIMUMZOOM=0.025f;		//2.5 meters per cm
const float ZOOMINCREMENTS=1.02f;
const float MAXIMUMZOOM=50.0f; 	//5000 meters per cm

#define CMPERINCH		2.54f

BOOL FloatFromString(LPCTSTR szString,float& flt);

void FloatToString(CString& szString,float flt);
CString FloatToString(float flt);
CString IntToString(int i);

void DWordToDate(DWORD dwDate,LPTSTR szInfo, int bufferLength);
DWORD DWordFromDateString(LPCTSTR szData);
DWORD DWordFromSEFDateString(LPCTSTR szData);
DWORD MakeMyDate(int iYear,int iMonth,int iDay);
WORD GetYear(DWORD dwDate);
WORD GetMonth(DWORD dwDate);
WORD GetDay(DWORD dwDate);

//Returns TRUE if the two readins are within Tolerance degrees of each other
BOOL WithinNDegrees(float fOne,float fTwo,float fTolerance);

inline float FeetToMeters(float fFeet) {return fFeet*0.3048f;}
inline float MetersToFeet(float fMeters) {return fMeters/0.3048f;}
inline float MetersToMiles(float fMeters) {return fMeters/1609.0f;}
//This function returns a pointer to a string consisting of the
//length in the prefered units as well as the units themselves.
//ie  2.3 m   7.83 km  1.43 miles   2.74 ft
LPCTSTR GetPreferedUnitString(float f);
LPCTSTR GetPreferedScaleString(float f);  //10.34 m/cm  1.37 ft/inch
float GetAngleBetween(float,float);

class CViewMatrix;
//MATRIX ALGEBRA CLASSES.
class CPosMatrix
{ 
	friend class CPosMatrix;
	friend class CClose;
	private:
		float m_Items[4];
	public:
		CPosMatrix();
		inline float const * GetDirectPointer() const {return &m_Items[0];}
//		void Dump() const;
		void Set(float f1,float f2,float f3);
		
		void CalculateIntermediatePosFromZ(float fTargetDepth,CPosMatrix & To,CPosMatrix &ret);
        
		void Normalize();
		BOOL IsNormal() {return m_Items[3]==1.0f;}
		float GetX() const {return m_Items[0];}
		float GetY() const {return m_Items[1];}
		float GetZ() const {return m_Items[2];}

		void Multiply(CPosMatrix * result,const CViewMatrix& r) const;
   		void Add(CPosMatrix * result,const CPosMatrix & r) const;
   		void Subtract(CPosMatrix * result,const CPosMatrix & r) const;
   		
		CPosMatrix & operator=(const CPosMatrix & r);
		
		BOOL IsEqual(CPosMatrix & );
};

void CalcNormal(float const * v1,float const *v2,float const * v3,float * out);
void ReduceToUnit(float vector[3]);

int PixelsToTwips(int iTwips);

class CViewMatrix
{
	friend class CPosMatrix;
	friend class CViewMatrix;
	private:
		float m_Items[4][4];
    public:
    	CViewMatrix();
    
//		void Dump() const;
    	void XPan(float distance);
    	void YPan(float distance);
		void ZPan(float distance);
    	void ChangeMetersPerCM(float fMetersPerCM);
    	void RotateX(float degrees);
    	void RotateY(float degrees);
    	void RotateZ(float degrees);

		void ResetRotations();
		    	
    	void Multiply(CViewMatrix *result,const CViewMatrix& r) const;

		CViewMatrix & operator=(const CViewMatrix & r);

		void SetPerspective();
//		void UnsetPerspective();
		
		void CenterOn(CPosMatrix *);	//move so that this spot is in the center
};

#endif

