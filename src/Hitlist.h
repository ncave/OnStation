#ifndef I_HITLIST__
#define I_HITLIST__
#include "node.h"
// (C) Copyright 1995, Taco van Ieperen
//


class CSearchableObject:public CRect
{
private:
	CNode * m_Node;
public:
    CSearchableObject(CRect& rect,CNode *node);
    ~CSearchableObject();
    CNode *GetNode() {return m_Node;}
};


class CSearchableObjectList
{
private:
	CArray <CSearchableObject *,CSearchableObject *> m_myArray;
public:
	CSearchableObjectList();
	~CSearchableObjectList();
	
	void AddObject(CRect& rect,CNode *node);
	CPtrArray *HitTest(CPoint pt);
	void RemoveAll();
};

#endif

