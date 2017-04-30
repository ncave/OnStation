#ifndef I_SURFACETIEIN__
#define I_SURFACETIEIN__

class CSurfaceData;
class CFileHelper;
class CSEFHelper;
class CFixedPoint:public CObject
{
	private:
		//These things are in UTM coordinates.  Make sure you
		//convert them to local coordinates before using.
		float m_fXpos;
		float m_fYpos;
		float m_fZpos;
		CString m_szName;
		CString m_szComment;
//Have we popped up a dialog warning about invalid fixed points?
		BOOL m_bWarnedMissing;
	public:
		CFixedPoint(LPCTSTR szStationName);
		~CFixedPoint();

		BOOL GetWarnedMissing() const {return m_bWarnedMissing;}
		void SetWarnedMissing(BOOL b) {m_bWarnedMissing=b;}
		//Serialization
		void Read(CFileHelper *h);
		void Write(CFileHelper *h);

		//Property changing methods.
		void SetAllCoordinates(float fx,float fy,float fz);
		void SetComment(LPCTSTR szComment);
		void SetStationName(LPCTSTR szStationName) {m_szName=szStationName;}
		//Information
		inline float GetX() const {return m_fXpos;}
		inline float GetY() const {return m_fYpos;}
		inline float GetZ() const {return m_fZpos;}
		inline LPCTSTR GetName() const {return m_szName;}
		inline LPCTSTR GetComment() const {return m_szComment;}
};


#endif


