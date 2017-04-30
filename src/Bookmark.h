#ifndef I_BOOKMARKS__
#define I_BOOKMARKS__

class CFileHelper;
class CBookMark:public CObject
{
public:
	CBookMark(LPCTSTR m_szName,float X,float Y,float Z,float fRot,float fTilt,float Zoom,INT_PTR iColorSchemeIndex);
	~CBookMark();

	void Write(CFileHelper *);
	void Read(CFileHelper *);

	CString m_szName;
	float m_fPanX;
	float m_fPanY;
	float m_fPanZ;
	float m_fRot;
	float m_fTilt;
	float m_fZoom;

	int m_iColorSchemeIndex;
};

#endif
