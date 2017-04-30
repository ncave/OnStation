#ifndef I_NAMELIST__
#define I_NAMELIST__
// (C) Copyright 1996, Taco van Ieperen
//
#include "afxtempl.h"

//This class is used to generate a list of who surveyed in a given
//cave in a certain period of time and how much they surveyed.  The
//class is used for printing out headers for the system which can list
//surveyors by year or by number of surveys, or by alphabetical order
class CNameHolderWithDate
{
public:
	CString m_szName;
	int m_iYear;
	int m_iNumberOfTrips;
};

class CNameHolderWithIncidents
{
public:
	CString m_szName;
	int m_iNumberOfTrips;
};

class CNameList
{
private:
	//We maintain two lists to keep things simpler.  One list
	//is simply a list of all the names and how often they
	//have occurred.  The other also uses the date to identify
	//things hence it will be larger then the first list since
	//names can recur across years.
	CList<CNameHolderWithDate,CNameHolderWithDate&> m_ListByYear;
	CList<CNameHolderWithIncidents,CNameHolderWithIncidents&> m_ListByNameOnly;
	int m_iLowestYear;
	int m_iHighestYear;

public:
	CNameList();
	~CNameList();

	void AddName(LPCTSTR szName,int iYear);
	int GetLowestYear() {return m_iLowestYear;}
	int GetHighestYear() {return m_iHighestYear;}

	void SetYear(int iYear);	//update lowest and highest thresholds
	//returns the number of distinct names for a given year.
	//Setting the Year to -1 means that you don't care about
	//the year and just want to list all names.  Year 0
	//means that you want all of the names which don't have a
	//year assigned to them.
	INT_PTR GetNumberOfNamesForYear(int iYear) const;
	LPCTSTR GetName(int iYear,int iIndex,int *iUsed);
};

#endif


