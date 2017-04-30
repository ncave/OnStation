#ifndef I_PRINTDUMP__
#define I_PRINTDUMP__
//(C) Copyright 1995, Taco van Ieperen
//
// Class to help dump text output to the print spooler

class CFileDumpHelper
{
private:
	CFile * m_pFile;
public:
	CFileDumpHelper();
	~CFileDumpHelper();

	void OutputText(LPCTSTR szData);
	BOOL BeginDump();
	void EndDump();
};

#endif


