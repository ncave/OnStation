//
// This code is used to Track the position of various Nodes in the rendered
// view of the cave.  These positions can be used by search engines and
// click detection code to find out where a node is.
#include "stdafx.h"
#include "hitlist.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CSearchableObjectList::AddObject(CRect& rect,CNode *pNode)
{
	CSearchableObject *obj=new CSearchableObject(rect,pNode);
	m_myArray.Add(obj);
}

CSearchableObject::CSearchableObject(CRect& rect,CNode *node):CRect(rect)
{
	m_Node=node;
	NormalizeRect();		// Windows does not handle PtInRect for unnormalized rects properly
}

CSearchableObject::~CSearchableObject()
{
	m_Node=NULL;
}


CSearchableObjectList::CSearchableObjectList()
{
}

CSearchableObjectList::~CSearchableObjectList()
{
	RemoveAll();
}

//Returns an array of all the points under the current cursor position
CPtrArray *CSearchableObjectList::HitTest(CPoint pt)
{
	CPtrArray *ret=new CPtrArray;
	for (int i=0;i<m_myArray.GetSize();i++)
	{
		if (m_myArray[i]->PtInRect(pt)) 
		{
			ret->Add((LPVOID)m_myArray[i]->GetNode());
		}
	}
	if (ret->GetSize()==0)
	{	
		delete ret;
		return NULL;
	}
	else
	{
		return ret;
	}
}


void CSearchableObjectList::RemoveAll()
{
	for (int i=0;i<m_myArray.GetSize();i++)
	{
		delete m_myArray[i];
	}
	m_myArray.RemoveAll();
}
