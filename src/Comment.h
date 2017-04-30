#ifndef COMMENT_H_
#define COMMENT_H_

class CComment:public CObject
{
public:
	CComment::CComment(LPCTSTR szStationName,LPCTSTR szType,LPCTSTR szBody);
	CComment::~CComment();

	LPCTSTR GetType() const {return m_szType;}
	LPCTSTR GetBody() const {return m_szBody;}
	LPCTSTR GetStationName() const {return m_szStationName;}
	void SetValues(LPCTSTR szStationName,LPCTSTR szType,LPCTSTR szBody);
private:
	CString m_szType;
	CString m_szBody;
	TCHAR m_szStationName[MAX_STATION_NAME_LENGTH];
};

#endif

