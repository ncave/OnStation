#include "stdafx.h"
#include "onstation.h"
#include "filehelper.h"
#include "stdio.h"
#include "string.h"
#include "surveyshot.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//This function reads the current line assuming a format of TOKEN=VALUE.
//The Token portion is stored in the string m_szToken.
//The Value portion is stored in the string m_szValue.
//The input is read until a \n is encountered or until our raw
//input buffer overflows.  If the last character of the input line
//is a \ we assume a continuation of the input onto the next line.
//in this case, we read the next line as well and add it to the m_szValue
//portion of the input.
void CFileHelper::ReadCurrentLine(BOOL bEndOfFileError)
{
start:
//	TRACE(_T("Reading line %i\n"),m_iCurrentLineNumber);
	if (m_bBufferedLine)
	{
		m_bBufferedLine=FALSE;
		return;
	}
	m_iCurrentLineNumber++;
	if (!ReadString(m_szBuffer,MAX_LINE_SIZE))
	{
		if (bEndOfFileError)
		{
			AfxMessageBox(IDS_ENDOFFILEERROR);
		}
		throw _T("end of file error");
	}
	if (m_szBuffer[0]==_T('\0'))	//empty line.  just skip it
	{
		goto start;
	}
	
	//Find the = to part of the string.
	for (int i=0;i<MAX_LINE_SIZE;i++)
	{
		if (m_szBuffer[i]==_T('\0'))
		{
			CString CS;
			CS.GetBuffer(512);
			m_szBuffer[450]=_T('\0');	//stop overflow
			CS.Format(IDS_BADLINEFORMAT,m_iCurrentLineNumber,m_szBuffer);
			AfxMessageBox(CS);
			throw _T("BadFormatError");
		}
		if (m_szBuffer[i]==_T('='))
		{
			m_szBuffer[i]=_T('\0');
			m_szValue=m_szBuffer+i+1;
			for (int j=i+1;j<MAX_LINE_SIZE+1;j++)
			{
				if (m_szBuffer[j]==_T('\0'))
				{
					if (m_szBuffer[j-1]==_T('\\'))
					{
						//line continues.  Read next line and append it.
						AppendLines(j);
						return;
					}
					return;
				}
			}
			return;
		}
	}
	CString CS;
	CS.GetBuffer(512);
	m_szBuffer[450]=_T('\0');	//stop overflow
	CS.Format(IDS_LINETOOLONG,m_iCurrentLineNumber);
	AfxMessageBox(CS);
	throw _T("Overrun exception");
}

void CFileHelper::AppendLines(int iPosition)
{
	m_iCurrentLineNumber++;
	if (iPosition>=MAX_LINE_SIZE-10)
	{
		CString CS;
		CS.Format(IDS_LINETOOLONG,m_iCurrentLineNumber);
		ReportError(CS);
		throw _T("Overrun exception");
		return;
	}
	if (ReadString(m_szBuffer+iPosition,MAX_LINE_SIZE-iPosition))
	{
		for (int j=iPosition;j<MAX_LINE_SIZE+1;j++)
		{
			if (m_szBuffer[j]==_T('\0'))
			{
				if (m_szBuffer[j-1]==_T('\\'))
				{
					AppendLines(j);
				}
				return;
			}
		}
		CString CS;
		CS.Format(IDS_LINETOOLONG,m_iCurrentLineNumber);
		ReportError(CS);
		throw _T("Overrun exception");
	}
	return;
}


//ASSUMES THAT THE CURRENT LINE IS THE BEGIN BLOCK.
//This is a little bit harder then it looks because these
//things can be nested.  We use this function recursively
//to take care of that.
BOOL CFileHelper::ReadToEndOfScope()
{
	CString szBlockToken=m_szBuffer;
	CString szBlockValue=m_szValue;

restart:	
	ReadCurrentLine();
	if (lstrcmpi(m_szBuffer, _T("Begin"))==0 || lstrcmpi(m_szBuffer, _T("ProprietaryExtension"))==0)
	{
		if (!ReadToEndOfScope())
		{
			return FALSE;
		}
		goto restart;
	}
	if (lstrcmpi(m_szBuffer, _T("End"))==0)
	{
		if (lstrcmpi(szBlockToken, _T("Begin"))!=0)
		{
			CString CS;
			CS.Format(IDS_MISMATCHEDEND,m_iCurrentLineNumber,m_szBuffer,m_szValue);
			ReportError(CS);
			return FALSE;
		}
		if (lstrcmpi(szBlockValue,m_szValue)==0)
		{
			return TRUE;
		}
		else
		{
			CString CS;
			CS.Format(IDS_WRONGEND,szBlockToken,szBlockValue,m_szBuffer,m_szValue);
			ReportError(CS);
			return FALSE;
		}
	}
	if (lstrcmpi(m_szBuffer, _T("ProprietaryEnd"))==0)
	{
		if (lstrcmpi(szBlockToken, _T("ProprietaryStart"))!=0)
		{
			CString CS;
			CS.Format(IDS_MISMATCHEDEND,m_iCurrentLineNumber,m_szBuffer,m_szValue);
			ReportError(CS);
			return FALSE;
		}
		if (lstrcmpi(szBlockValue,m_szValue)==0)
		{
			return TRUE;
		}
		else
		{
			CString CS;
			CS.Format(IDS_WRONGEND,szBlockToken,szBlockValue,m_szBuffer,m_szValue);
			ReportError(CS);
			return FALSE;
		}
	}
	goto restart; 
}

void CFileHelper::ReportError(LPCTSTR szError)
{
	AfxMessageBox(szError);
}

//Reads a token which may have a very large line following it.
//Internally, we use the standard read.
void CFileHelper::ReadPartialToken(LPCTSTR szExpectedToken)
{
	ReadTokenString(szExpectedToken);
}

float CFileHelper::ReadAppendedFloat()
{
	//skip whitespace
	while (m_szValue[0]==_T(' ') || m_szValue[0]==_T('\t'))
	{
		m_szValue++;
	}
	if (m_szValue[0]==_T('\0'))	//empty string:
	{
		return NAN;
	}
	//Look for trailing whitespace
	TCHAR szFloat[128];
	int i=0;
	while (m_szValue[0]!=_T('\t') && m_szValue[0]!=_T('\r') && m_szValue[0]!=_T('\n') && m_szValue[0]!=_T(' ') && m_szValue[0]!=0 && i<128)
	{
		szFloat[i]=m_szValue[0];
		m_szValue++;
		i++;
	}
	szFloat[i]=_T('\0');
	if (szFloat[0]==_T('\0'))
	{
		return NAN;
	}
	if (lstrcmpi(szFloat, _T("NAN"))==0)
	{
		return NAN;
	}
	float f;
	if (!FloatFromString(szFloat,f))
	{
		return NAN;
	}
	return f;
}

int CFileHelper::ReadTokenInt(LPCTSTR szExpectedToken)
{
restart:
	ReadCurrentLine();
	if (lstrcmpi(szExpectedToken, _T("ProprietaryExtension"))!=0 && lstrcmpi(m_szBuffer, _T("ProprietaryExtension"))==0)
	{
		if (!ReadToEndOfScope())
		{
			throw _T("extension error");
		}
		goto restart;
	}
	if (!lstrcmpi(szExpectedToken,m_szBuffer)==0)
	{
		CString CS;
		CS.Format(IDS_UNEXPECTEDTOKEN,m_iCurrentLineNumber,szExpectedToken,m_szBuffer,m_szValue);
		ReportError(CS);
		throw _T("unexpected token error");
	}
	return _tstoi(m_szValue);
}

float CFileHelper::ReadTokenFloat(LPCTSTR szExpectedToken)
{
restart:
	ReadCurrentLine();
	if (lstrcmpi(szExpectedToken, _T("ProprietaryExtension"))!=0 && lstrcmpi(m_szBuffer, _T("ProprietaryExtension"))==0)
	{
		if (!ReadToEndOfScope())
		{
			throw _T("extension error");
		}
		goto restart;
	}
	if (!lstrcmpi(szExpectedToken,m_szBuffer)==0)
	{
		CString CS;
		CS.Format(IDS_UNEXPECTEDTOKEN,m_iCurrentLineNumber,szExpectedToken,m_szBuffer,m_szValue);
		ReportError(CS);
		throw _T("unexpected token error");
	}
	//a little bit lazy here.
	if ((m_szValue[0]==_T('N') || m_szValue[0]==_T('n')) && (m_szValue[1]==_T('A') || m_szValue[1]==_T('a')))
	{
		return NAN;
	}
	return (float)_tstof(m_szValue);
}

DWORD CFileHelper::ReadTokenDate(LPCTSTR szExpectedToken)
{
restart:
	ReadCurrentLine();
	if (lstrcmpi(szExpectedToken, _T("ProprietaryExtension"))!=0 && lstrcmpi(m_szBuffer, _T("ProprietaryExtension"))==0)
	{
		if (!ReadToEndOfScope())
		{
			throw _T("extension error");
		}
		goto restart;
	}
	if (!lstrcmpi(szExpectedToken,m_szBuffer)==0)
	{
		CString CS;
		CS.Format(IDS_UNEXPECTEDTOKEN,m_iCurrentLineNumber,szExpectedToken,m_szBuffer,m_szValue);
		ReportError(CS);
		throw _T("unexpected token error");
	}
	int iYear,iMonth,iDay;
	if (_stscanf_s(m_szValue, _T("%i/%i/%i"),&iYear,&iMonth,&iDay)!=3)
	{
		return 0xffffffff;
	}
	if (iYear<1000 || iYear>3000 || iMonth<1 || iMonth>12 || iDay<1 || iDay>31)
	{
		return 0xffffffff;
	}
	return MakeMyDate(iYear,iMonth,iDay);
}

void CFileHelper::ReadTokenBegin(LPCTSTR szValue)
{
	LPCTSTR szData=ReadTokenString(_T("Begin"));
	int diff = lstrcmpi(szValue,szData);
	if (lstrcmpi(szValue,szData)!=0)
	{
		CString CS;
		CS.Format(IDS_UNEXPECTEDTOKENBEGIN,m_iCurrentLineNumber,szValue,m_szBuffer);
		ReportError(CS);
		throw _T("unexpected token error");
	}
}

void CFileHelper::WritePartialToken(LPCTSTR szToken)
{
	WriteString(szToken);
	WriteString(_T("="));
	m_bAppended=FALSE;;
}

void CFileHelper::AppendFloat(float fValue)
{
	if (m_bAppended)
	{
		WriteString(_T(" "));
	}
	WriteRawFloat(fValue);
	m_bAppended=TRUE;
}
void CFileHelper::WriteEndOfLine()
{
	WriteString(_T("\n"));
}

void CFileHelper::ReadTokenEnd(LPCTSTR szValue)
{
	LPCTSTR szData=ReadTokenString(_T("End"));
	if (lstrcmpi(szValue,szData)!=0)
	{
		CString CS;
		CS.Format(IDS_UNEXPECTEDTOKENEND,m_iCurrentLineNumber,szValue,m_szBuffer);
		ReportError(CS);
		throw _T("unexpected token error");
	}
}

BOOL CFileHelper::CheckProprietaryStart(LPCTSTR szValue,BOOL bEndOfFileAllowed)
{
start:
	try {
		LPCTSTR szData=GetTokenLookaheadString(FALSE,!bEndOfFileAllowed);
		if (lstrcmpi(szData, _T("ProprietaryStart"))!=0)
		{
			return FALSE;
		}
		if (lstrcmpi(m_szValue,szValue)!=0)
		{
			ReadToEndOfScope();
			//somebody else's proprietary data
			goto start;
		}
	}
	catch (LPCTSTR szData)
	{
		if (bEndOfFileAllowed && lstrcmpi(szData, _T("end of file error"))==0)
		{
			return FALSE;
		}
		else
		{
			throw;
		}
	}
	return TRUE;
}

void CFileHelper::CheckProprietaryEnd(LPCTSTR szValue)
{
	LPCTSTR szData=ReadTokenString(_T("ProprietaryEnd"));
	if (lstrcmpi(szValue,szData)!=0)
	{
		CString CS;
		CS.Format(IDS_UNEXPECTEDTOKEN,m_iCurrentLineNumber,szValue,m_szBuffer,m_szValue);
		ReportError(CS);
		throw _T("unexpected token error");
	}
}


LPCTSTR CFileHelper::ReadTokenString(LPCTSTR szExpectedToken)
{
restart:
	ReadCurrentLine();
	if (lstrcmpi(szExpectedToken, _T("ProprietaryExtension"))!=0 && lstrcmpi(m_szBuffer, _T("ProprietaryExtension"))==0)
	{
		if (!ReadToEndOfScope())
		{
			throw _T("extension error");
		}
		goto restart;
	}

	if (!lstrcmpi(szExpectedToken,m_szBuffer)==0)
	{
		CString CS;
		CS.Format(IDS_UNEXPECTEDTOKEN,m_iCurrentLineNumber,szExpectedToken,m_szBuffer,m_szValue);
		ReportError(CS);
		throw _T("unexpected token error");
	}
	return m_szValue;
}

LPCTSTR CFileHelper::GetTokenLookaheadString(BOOL bSkipProprietaryStart,BOOL bEndOfFileError)
{
start:
	ReadCurrentLine(bEndOfFileError);
	m_bBufferedLine=TRUE;
	if (bSkipProprietaryStart && (lstrcmpi(m_szBuffer, _T("ProprietaryStart"))==0))
	{
		ReadToEndOfScope();
		goto start;
	}
	return m_szBuffer;
}

LPCTSTR CFileHelper::GetTokenLookaheadValue()
{
	ReadCurrentLine();
	m_bBufferedLine=TRUE;
	return m_szValue;
}


void CFileHelper::ReadStandardShotVersion1(LPTSTR szFrom,LPTSTR szTo,float* length,float* compass,float* clino,float* up,float* down,float* left,float* right,float* compassback,float* clinoback,WORD * wAttributes)
{
	TCHAR stringfrom[128];
	TCHAR stringto[128];
	TCHAR stringlen[128];
	TCHAR stringcom[128];
	TCHAR stringclino[128];
	TCHAR stringup[128];
	TCHAR stringdn[128];
	TCHAR stringleft[128];
	TCHAR stringright[128];
	TCHAR stringcomback[128];
	TCHAR stringclinoback[128];
	TCHAR stringattributes[128];
	ReadCurrentLine();
	if (lstrcmpi(m_szBuffer, _T("Shot"))!=0)
	{
		CString CS;
		CS.Format(IDS_BADSHOTFORMAT,m_iCurrentLineNumber,m_szValue);
		AfxMessageBox(CS);
		throw (_T("Bad shot format"));
	}

	int iNum = _stscanf_s(m_szValue, _T("%s %s %s %s %s %s %s %s %s %s %s %s"),
		stringfrom,       _countof(stringfrom),
		stringto,         _countof(stringto),
		stringlen,        _countof(stringlen),
		stringcom,        _countof(stringcom),
		stringclino,      _countof(stringclino),
		stringcomback,    _countof(stringcomback),
		stringclinoback,  _countof(stringclinoback),
		stringup,         _countof(stringup),
		stringdn,         _countof(stringdn),
		stringleft,       _countof(stringleft),
		stringright,      _countof(stringright),
		stringattributes, _countof(stringattributes));
	if (iNum<12)
	{		   
		CString CS;
		CS.Format(IDS_BADSHOTFORMAT,m_iCurrentLineNumber,m_szValue);
		AfxMessageBox(CS);
		throw (_T("Bad shot format"));
	}
	
	WORD wShotType=ParseAttributes(stringattributes);
	if (wShotType==0xffff)
	{
		CString CS;
		CS.Format(IDS_BADFORMATSTRING,m_iCurrentLineNumber,m_szValue);
		AfxMessageBox(CS);
		throw (_T("Bad shot format"));
	}
	*wAttributes=wShotType;

	//Parse all the information
	if (!ParseFloat(length,stringlen) || !ParseFloat(compass,stringcom) || !(ParseFloat(clino,stringclino)))
	{
		CString CS;
		CS.Format(IDS_BADSHOTFORMAT,m_iCurrentLineNumber,m_szValue);
		ReportError(CS);
		throw (_T("Bad shot format"));
	}

	if (*length!=NAN)
	{
		if (*clino==NAN && *clinoback==NAN)
		{
			TRACE(_T("CLINO ERROR"));
			*clino=0.0f;
		}
		if (*compass==NAN && *compassback==NAN)
		{
			TRACE(_T("COMPASS ERROR"));
			*compass=0.0f;
		}
	}
	

	ParseFloat(up,stringup);
	ParseFloat(down,stringdn);
	ParseFloat(left,stringleft);
	ParseFloat(right,stringright);
	ParseFloat(compassback,stringcomback);
	ParseFloat(clinoback,stringclinoback);
	ASSERT(lstrlen(szFrom)<MAX_STATION_NAME_LENGTH);
	lstrcpyn(szFrom,stringfrom,MAX_STATION_NAME_LENGTH);
	//From and To equal means that we have a wall only
	//shot
	if (lstrcmpi(stringfrom,stringto)==0)
	{
		lstrcpy(szTo, _T(""));
	}
	else
	{
		ASSERT(lstrlen(szTo)<MAX_STATION_NAME_LENGTH);
		lstrcpyn(szTo,stringto,MAX_STATION_NAME_LENGTH);
	}
}

void CFileHelper::ReadCaveDiveShotVersion1(LPTSTR szFrom,LPTSTR szTo,float* length,float* compass,float* depth,float* up,float* down,float* left,float* right,WORD * wAttributes)
{
	TCHAR szFromBuffer[128];
	TCHAR szToBuffer[128];
	TCHAR szLength[128];
	TCHAR szCompass[128];
	TCHAR szDepth[128];
	TCHAR szUp[128];
	TCHAR szDown[128];
	TCHAR szLeft[128];
	TCHAR szRight[128];
	TCHAR szAttributes[128];

	ReadCurrentLine();
	if (lstrcmpi(m_szBuffer, _T("DiveShot"))!=0)
	{
		CString CS;
		CS.Format(IDS_BADSHOTFORMAT,m_iCurrentLineNumber,m_szValue);
		AfxMessageBox(CS);
		throw (_T("Bad shot format"));
	}

	int iNum = _stscanf_s(m_szValue, _T("%s %s %s %s %s %s %s %s %s %s"),
		szFromBuffer, _countof(szFromBuffer),
		szToBuffer,	  _countof(szToBuffer),
		szLength,	  _countof(szLength),
		szCompass,	  _countof(szCompass),
		szDepth,	  _countof(szDepth),
		szUp,		  _countof(szUp),
		szDown,		  _countof(szDown),
		szLeft,		  _countof(szLeft),
		szRight,	  _countof(szRight),
		szAttributes, _countof(szAttributes));
	if (iNum<10)
	{		   
		CString CS;
		CS.Format(IDS_BADSHOTFORMAT,m_iCurrentLineNumber,m_szBuffer);
		AfxMessageBox(CS);
		throw (_T("Bad shot format"));
	}
	
	WORD wShotType=ParseAttributes(szAttributes);
	if (wShotType==0xffff)
	{
		CString CS;
		CS.Format(IDS_BADFORMATSTRING,m_iCurrentLineNumber,m_szBuffer);
		AfxMessageBox(CS);
		throw (_T("Bad shot format"));
	}
	*wAttributes=wShotType;
	//Parse all the information
	if (!ParseFloat(length,szLength) || !ParseFloat(compass,szCompass) || !(ParseFloat(depth,szDepth)))
	{
		CString CS;
		CS.Format(IDS_BADSHOTFORMAT,m_iCurrentLineNumber,m_szBuffer);
		AfxMessageBox(CS);
		throw (_T("Bad shot format"));
	}
	ParseFloat(up,szUp);
	ParseFloat(down,szDown);
	ParseFloat(left,szLeft);
	ParseFloat(right,szRight);
	ASSERT(lstrlen(szFromBuffer)<MAX_STATION_NAME_LENGTH);
	lstrcpyn(szFrom,szFromBuffer,MAX_STATION_NAME_LENGTH);
	//From and To equal means that we have a wall only
	//shot
	ASSERT(lstrlen(szToBuffer)<MAX_STATION_NAME_LENGTH);
	if (lstrcmpi(szFromBuffer,szToBuffer)==0)
	{
		lstrcpy(szTo, _T(""));
	}
	else
	{
		lstrcpyn(szTo,szToBuffer,MAX_STATION_NAME_LENGTH);
	}
}

void CFileHelper::ReadWalls(LPTSTR szStation,float* fCompass,float* fClino,float* fUp,float* fDown,float * fLeft,float *fRight)
{
	TCHAR stringfrom[128];
	TCHAR stringcom[128];
	TCHAR stringclino[128];
	TCHAR stringup[128];
	TCHAR stringdown[128];
	TCHAR stringleft[128];
	TCHAR stringright[128];
	ReadCurrentLine();

	ASSERT(lstrcmpi(m_szBuffer, _T("Wall"))==0);

	int iNum = _stscanf_s(m_szValue, _T("%s %s %s %s %s %s %s"),
		stringfrom,  _countof(stringfrom),
		stringcom,	 _countof(stringcom),
		stringclino, _countof(stringclino),
		stringup,	 _countof(stringup),
		stringdown,	 _countof(stringdown),
		stringleft,	 _countof(stringleft),
		stringright, _countof(stringright));
	if (iNum<7)
	{		   
		CString CS;
		CS.Format(IDS_BADWALLFORMAT,m_iCurrentLineNumber,m_szValue);
		AfxMessageBox(CS);
		throw (_T("Bad shot format"));
	}
	
	//Parse all the information
	ParseFloat(fCompass,stringcom);
	ParseFloat(fClino,stringclino);
	ParseFloat(fUp,stringup);
	ParseFloat(fDown,stringdown);
	ParseFloat(fLeft,stringleft);
	ParseFloat(fRight,stringright);
	ASSERT(lstrlen(stringfrom)<MAX_STATION_NAME_LENGTH);
	lstrcpyn(szStation,stringfrom,MAX_STATION_NAME_LENGTH);
}

void CFileHelper::ReadStandardShotVersion2(LPTSTR szFrom,LPTSTR szTo,float* length,float* compass,float* clino,float* compassback,float* clinoback,WORD * wAttributes)
{
	TCHAR stringfrom[128];
	TCHAR stringto[128];
	TCHAR stringlen[128];
	TCHAR stringcom[128];
	TCHAR stringclino[128];
	TCHAR stringcomback[128];
	TCHAR stringclinoback[128];
	TCHAR stringattributes[128];
	ReadCurrentLine();
	if (lstrcmpi(m_szBuffer, _T("Shot"))!=0)
	{
		CString CS;
		CS.Format(IDS_BADSHOTFORMAT,m_iCurrentLineNumber,m_szValue);
		AfxMessageBox(CS);
		throw (_T("Bad shot format"));
	}

	int iNum = _stscanf_s(m_szValue, _T("%s %s %s %s %s %s %s %s"),
		stringfrom,       _countof(stringfrom),
		stringto,		  _countof(stringto),
		stringlen,		  _countof(stringlen),
		stringcom,		  _countof(stringcom),
		stringclino,	  _countof(stringclino),
		stringcomback,	  _countof(stringcomback),
		stringclinoback,  _countof(stringclinoback),
		stringattributes, _countof(stringattributes));
	if (iNum<8)
	{		   
		CString CS;
		CS.Format(IDS_BADSHOTFORMAT,m_iCurrentLineNumber,m_szValue);
		AfxMessageBox(CS);
		throw (_T("Bad shot format"));
	}
	
	WORD wShotType=ParseAttributes(stringattributes);
	if (wShotType==0xffff)
	{
		CString CS;
		CS.Format(IDS_BADFORMATSTRING,m_iCurrentLineNumber,m_szValue);
		AfxMessageBox(CS);
		throw (_T("Bad shot format"));
	}
	*wAttributes=wShotType;

	//Parse all the information
	if (!ParseFloat(length,stringlen) || !ParseFloat(compass,stringcom) || !(ParseFloat(clino,stringclino)))
	{
		CString CS;
		CS.Format(IDS_BADSHOTFORMAT,m_iCurrentLineNumber,m_szValue);
		ReportError(CS);
		throw (_T("Bad shot format"));
	}

	if (*length!=NAN)
	{
		if (*clino==NAN && *clinoback==NAN)
		{
			TRACE(_T("CLINO ERROR"));
			*clino=0.0f;
		}
		if (*compass==NAN && *compassback==NAN)
		{
			TRACE(_T("COMPASS ERROR"));
			*compass=0.0f;
		}
	}
	
	ParseFloat(compassback,stringcomback);
	ParseFloat(clinoback,stringclinoback);
	ASSERT(lstrlen(stringfrom)<MAX_STATION_NAME_LENGTH);
	lstrcpyn(szFrom,stringfrom,MAX_STATION_NAME_LENGTH);
	//From and To equal means that we have a wall only
	//shot
	if (lstrcmpi(stringfrom,stringto)==0)
	{
		lstrcpy(szTo, _T(""));
	}
	else
	{
		ASSERT(lstrlen(stringto)<MAX_STATION_NAME_LENGTH);
		lstrcpyn(szTo,stringto,MAX_STATION_NAME_LENGTH);
	}
}

void CFileHelper::ReadCaveDiveShotVersion2(LPTSTR szFrom,LPTSTR szTo,float* length,float* compass,float* depth,WORD * wAttributes)
{
	TCHAR szFromBuffer[128];
	TCHAR szToBuffer[128];
	TCHAR szLength[128];
	TCHAR szCompass[128];
	TCHAR szDepth[128];
	TCHAR szAttributes[128];

	ReadCurrentLine();
	if (lstrcmpi(m_szBuffer, _T("DiveShot"))!=0)
	{
		CString CS;
		CS.Format(IDS_BADSHOTFORMAT,m_iCurrentLineNumber,m_szValue);
		AfxMessageBox(CS);
		throw (_T("Bad shot format"));
	}

	int iNum = _stscanf_s(m_szValue, _T("%s %s %s %s %s %s"),
		szFromBuffer, _countof(szFromBuffer),
		szToBuffer,	  _countof(szToBuffer),
		szLength,	  _countof(szLength),
		szCompass,	  _countof(szCompass),
		szDepth,	  _countof(szDepth),
		szAttributes, _countof(szAttributes));
	if (iNum<6)
	{		   
		CString CS;
		CS.Format(IDS_BADSHOTFORMAT,m_iCurrentLineNumber,m_szBuffer);
		AfxMessageBox(CS);
		throw (_T("Bad shot format"));
	}
	
	WORD wShotType=ParseAttributes(szAttributes);
	if (wShotType==0xffff)
	{
		CString CS;
		CS.Format(IDS_BADFORMATSTRING,m_iCurrentLineNumber,m_szBuffer);
		AfxMessageBox(CS);
		throw (_T("Bad shot format"));
	}
	*wAttributes=wShotType;
	//Parse all the information
	if (!ParseFloat(length,szLength) || !ParseFloat(compass,szCompass) || !(ParseFloat(depth,szDepth)))
	{
		CString CS;
		CS.Format(IDS_BADSHOTFORMAT,m_iCurrentLineNumber,m_szBuffer);
		AfxMessageBox(CS);
		throw (_T("Bad shot format"));
	}
	ASSERT(lstrlen(szFromBuffer)<MAX_STATION_NAME_LENGTH);
	lstrcpyn(szFrom,szFromBuffer,MAX_STATION_NAME_LENGTH);
	//From and To equal means that we have a wall only
	//shot
	ASSERT(lstrlen(szToBuffer)<MAX_STATION_NAME_LENGTH);
	if (lstrcmpi(szFromBuffer,szToBuffer)==0)
	{
		lstrcpy(szTo, _T(""));
	}
	else
	{
		lstrcpyn(szTo,szToBuffer,MAX_STATION_NAME_LENGTH);
	}
}
  
WORD CFileHelper::ParseAttributes(LPCTSTR szData)
{
	if (szData[0]!=_T('('))
	{
		return 0xffff;	//bad format
	}
	szData++;
	WORD wReturn=0;
	while (szData[0]!=0)
	{
		switch (szData[0])
		{
		case _T('X'):
			wReturn|=SHOT_EXCLUDE_FROM_ALL_PROCESSING;
			break;
		case _T('S'):
			wReturn|=SHOT_SURFACE;
			break;
		case _T('C'):
			wReturn|=SHOT_EXCLUDE_FROM_CLOSING;
			break;
		case _T('L'):
			wReturn|=SHOT_EXCLUDE_FROM_LENGTH_TOTALS;
			break;
		case _T('P'):
			wReturn|=SHOT_EXCLUDE_FROM_PLOTTING;
			break;
		case _T('Y'):
			wReturn|=SHOT_SPLAY_SHOT;
			break;
		default:
			break;	//ignore anything else
		}
		szData++;
	}
	return wReturn;
}

#ifdef _UNICODE
CFileHelper::CFileHelper(CStdioFile * pFile)
{
	m_pFile=pFile;
	m_iCurrentLineNumber=0;
	m_bBufferedLine=FALSE;
	m_fVersion=2.0;
}
#else
CFileHelper::CFileHelper(CArchive * ar)
{
	m_Archive=ar;
	m_iCurrentLineNumber=0;
	m_bBufferedLine=FALSE;
	m_fVersion=2.0;
}
#endif

CFileHelper::~CFileHelper()
{
}

BOOL CFileHelper::ReadString(LPTSTR szBuffer,int iSize)
{
#ifdef _UNICODE
	LPTSTR szBuf = m_pFile->ReadString(szBuffer,iSize);
	if (szBuf != NULL && szBuf[lstrlen(szBuf)-1] == _T('\n'))
		szBuf[lstrlen(szBuf)-1] = _T('\0');
	return (szBuf != NULL);
#else
	return (m_Archive->ReadString(szBuffer,iSize)!=NULL);
#endif
}

void CFileHelper::WriteString(LPCTSTR szBuffer)
{
#ifdef _UNICODE
	m_pFile->WriteString(szBuffer);
#else
	CString str(szBuffer);
	str.Replace(_T("\r\n"), _T("\n"));
	str.Replace(_T("\n"), _T("\r\n"));
	m_Archive->WriteString(str);
#endif
}

void CFileHelper::WriteRawFloat(float f)
{
	TCHAR szBuffer[16];
	if (f!=NAN)
	{
		_stprintf_s(szBuffer, _T("%.2f"),f);
		WriteString(szBuffer);
	}
	else
	{
		WriteString(_T("NAN"));
	}
}

void CFileHelper::WriteToken(LPCTSTR szToken,float f)
{
	WriteString(szToken);
	WriteString(_T("="));
	WriteRawFloat(f);
	WriteString(_T("\n"));
}
		

void CFileHelper::WriteToken(LPCTSTR szToken,LPCTSTR szValue)
{
	WriteString(szToken);
	WriteString(_T("="));
	WriteString(szValue);
	WriteString(_T("\n"));
}

void CFileHelper::WriteToken(LPCTSTR szToken,int iValue)
{
	WriteString(szToken);
	WriteString(_T("="));

	TCHAR szBuffer[16];
	_stprintf_s(szBuffer, _T("%i"),iValue);
	WriteString(szBuffer);
	WriteString(_T("\n"));
}

void CFileHelper::WriteTokenDate(LPCTSTR szToken,DWORD dwDate)
{
	WriteString(szToken);
	WriteString(_T("="));

	if (dwDate!=0xffffffff)
	{
		TCHAR szBuffer[32];
		_stprintf_s(szBuffer, _T("%i/%i/%i"),GetYear(dwDate),GetMonth(dwDate),GetDay(dwDate));
		WriteString(szBuffer);
	}
	WriteString(_T("\n"));
}

void CFileHelper::WriteWalls(LPCTSTR szFrom,float fCompass,float fClino,float up,float down,float left,float right)
{
	if (up==NAN && down==NAN && left==NAN && right==NAN)
	{
		return;
	}
	WriteString(_T("Wall="));
	WriteString(szFrom);
	WriteString(_T(" "));
	WriteRawFloat(fCompass);
	WriteString(_T(" "));
	WriteRawFloat(fClino);
	WriteString(_T(" "));
	WriteRawFloat(up);
	WriteString(_T(" "));
	WriteRawFloat(down);
	WriteString(_T(" "));
	WriteRawFloat(left);
	WriteString(_T(" "));
	WriteRawFloat(right);
	WriteString(_T("\n"));
}
void CFileHelper::WriteStandardShot(LPCTSTR szFrom,LPCTSTR szTo,float length,float compass,float clino,float clinoback,float compassback,WORD wAttributes)
{
	//An endpoint of the leg
	if (szTo[0]==_T('\0'))
	{
		return;
	}
	WriteString(_T("Shot="));
	WriteString(szFrom);
	WriteString(_T(" "));
	WriteString(szTo);
	WriteString(_T(" "));
	WriteRawFloat(length);
	WriteString(_T(" "));
	WriteRawFloat(compass);
	WriteString(_T(" "));
	WriteRawFloat(clino);
	WriteString(_T(" "));
	WriteRawFloat(clinoback);
	WriteString(_T(" "));
	WriteRawFloat(compassback);
	WriteString(_T(" ("));
	if (wAttributes&SHOT_SURFACE)
	{
		WriteString(_T("S"));
	}
	if (wAttributes&SHOT_EXCLUDE_FROM_CLOSING)
	{
		WriteString(_T("C"));
	}
	if (wAttributes&SHOT_EXCLUDE_FROM_LENGTH_TOTALS)
	{
		WriteString(_T("L"));
	}
	if (wAttributes&SHOT_EXCLUDE_FROM_ALL_PROCESSING)
	{
		WriteString(_T("X"));
	}
	if (wAttributes&SHOT_EXCLUDE_FROM_PLOTTING)
	{
		WriteString(_T("P"));
	}
	if (wAttributes&SHOT_SPLAY_SHOT)
	{
		WriteString(_T("Y"));
	}
	WriteString(_T(")\n"));
}

void CFileHelper::WriteTokenColor(LPCTSTR szToken,COLORREF crColor)
{
	WriteString(szToken);
	WriteString(_T("="));
	if (crColor==COLOR_INACTIVE)
	{
		WriteString(_T("-2"));
	}
	else if (crColor==COLOR_INVISIBLE)
	{
		WriteString(_T("-1"));
	}
	else
	{
		TCHAR szBuffer[20];
		_stprintf_s(szBuffer, _T("%i %i %i"),GetRValue(crColor),GetGValue(crColor),GetBValue(crColor));
		WriteString(szBuffer);
	}
	WriteString(_T("\n"));
}

COLORREF CFileHelper::ReadTokenColor(LPCTSTR szExpectedToken)
{
restart:
	ReadCurrentLine();
	if (lstrcmpi(szExpectedToken, _T("ProprietaryExtension"))!=0 && lstrcmpi(m_szBuffer, _T("ProprietaryExtension"))==0)
	{
		if (!ReadToEndOfScope())
		{
			throw _T("extension error");
		}
		goto restart;
	}

	if (!lstrcmpi(szExpectedToken,m_szBuffer)==0)
	{
		CString CS;
		CS.Format(IDS_UNEXPECTEDTOKEN,m_iCurrentLineNumber,szExpectedToken,m_szBuffer,m_szValue);
		ReportError(CS);
		throw _T("unexpected token error");
	}
	int iRed,iGreen,iBlue;
	int iArgs = _stscanf_s(m_szValue, _T("%i %i %i"),&iRed,&iGreen,&iBlue);
	if (iArgs==1)
	{
		if (iRed==-1)
		{
			return COLOR_INVISIBLE;
		}
		if (iRed==-2)
		{
			return COLOR_INACTIVE;
		}
	}
	if (iRed<0 || iGreen<0 || iBlue<0 || iRed>255 || iGreen>255 || iBlue>255)
	{
		return RGB(255,255,255);	//white;
	}
	return (RGB(iRed,iGreen,iBlue));
}

void CFileHelper::WriteDiveShot(LPCTSTR szFrom,LPCTSTR szTo,float length,float compass,float fDepth,WORD wAttributes)
{
	WriteString(_T("DiveShot="));
	WriteString(szFrom);
	WriteString(_T(" "));
	if (szTo[0]==_T('\0'))
	{
		WriteString(szFrom);
	}
	else
	{
		WriteString(szTo);
	}
	WriteString(_T(" "));
	WriteRawFloat(length);
	WriteString(_T(" "));
	WriteRawFloat(compass);
	WriteString(_T(" "));
	WriteRawFloat(fDepth);
	WriteString(_T(" ("));
	if (wAttributes&SHOT_SURFACE)
	{
		WriteString(_T("S"));
	}
	if (wAttributes&SHOT_EXCLUDE_FROM_CLOSING)
	{
		WriteString(_T("C"));
	}
	if (wAttributes&SHOT_EXCLUDE_FROM_LENGTH_TOTALS)
	{
		WriteString(_T("L"));
	}
	if (wAttributes&SHOT_EXCLUDE_FROM_ALL_PROCESSING)
	{
		WriteString(_T("X"));
	}
	if (wAttributes&SHOT_EXCLUDE_FROM_PLOTTING)
	{
		WriteString(_T("P"));
	}
	if (wAttributes&SHOT_SPLAY_SHOT)
	{
		WriteString(_T("Y"));
	}
	WriteString(_T(")\n"));
}


BOOL CFileHelper::ParseFloat(float* test,LPCTSTR szTest)
{
	if (lstrcmpi(szTest, _T("NAN"))==0)
	{
		*test=NAN;
		return TRUE;
	}
	else
	{
		return FloatFromString(szTest,*test);
	}
}

CBufferFileHelper::CBufferFileHelper(LPCTSTR szData):CFileHelper(NULL)
{
	m_szReadPointer=szData;
	m_szTextStoring=NULL;
	m_iStoringLength=0;
	m_iStoringCurrent=0;
}
CBufferFileHelper::CBufferFileHelper():CFileHelper(NULL)
{
	//I used to concatenate CStrings for this but it
	//was too slow because the CString would have to
	//realloc every time it grew which could be millions
	//of times in some cases.
	m_szReadPointer=NULL;
	m_iStoringLength=10000 * sizeof(TCHAR);
	m_szTextStoring=(LPTSTR)malloc(m_iStoringLength);
	m_iStoringCurrent=0;
}

CBufferFileHelper::~CBufferFileHelper()
{
	if (m_szTextStoring!=NULL)
	{
		free(m_szTextStoring);
	}
}

BOOL CBufferFileHelper::ReadString(LPTSTR szData,int iSize)
{
	if (m_szReadPointer==NULL)
	{
		return FALSE;
	}
	int iCount=0;
	while ((m_szReadPointer[iCount]!=_T('\n')) && (m_szReadPointer[iCount]!=_T('\0')))
	{
		iCount++;
	}
	lstrcpyn(szData,m_szReadPointer,min(iSize,iCount));
	if (m_szReadPointer[iCount]==_T('\0'))
	{
		m_szReadPointer=NULL;
	}
	else
	{
		m_szReadPointer=m_szReadPointer+iCount+1;
	}

	return TRUE;
}

void CBufferFileHelper::WriteString(LPCTSTR szData)
{
	int iExtra=lstrlen(szData);
	if (m_iStoringCurrent+iExtra>=m_iStoringLength)
	{
		m_iStoringLength += 10000 * sizeof(TCHAR);
		m_szTextStoring=(LPTSTR)realloc((LPVOID)m_szTextStoring,m_iStoringLength);
	}
	lstrcpy(m_szTextStoring+m_iStoringCurrent,szData);
	m_iStoringCurrent+=iExtra;
}
