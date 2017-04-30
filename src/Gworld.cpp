#include "stdafx.h"
#include "gworld.h"

CGWorld::~CGWorld()
{
};

CGWorld::CGWorld()
{
};


CGWorldTriangleGroup::CGWorldTriangleGroup(int iNumVertices,COLORREF crColor)
{
	ASSERT(iNumVertices>=3);
	m_iNumVertices=iNumVertices;
	m_pVertices=new VERTEX[iNumVertices];
	m_crColor=crColor;
}

CGWorldTriangleGroup::~CGWorldTriangleGroup()
{
	delete[] m_pVertices;
	m_pVertices=NULL;
	m_iNumVertices=-1;
}

void CGWorldTriangleGroup::SetVertex(int iIndex,VERTEX& vertex)
{
	ASSERT(iIndex>=0 && iIndex<m_iNumVertices);	//don't overflow buffer
	m_pVertices[iIndex]=vertex;
}

void CGWorldTriangleGroup::SetVertex(int iIndex,const float *pPos)
{
	ASSERT(iIndex>=0 && iIndex<m_iNumVertices);	//don't overflow buffer
	m_pVertices[iIndex].m_fPos[0]=pPos[0];
	m_pVertices[iIndex].m_fPos[1]=pPos[1];
	m_pVertices[iIndex].m_fPos[2]=pPos[2];
}

void CGWorldTriangleGroup::SetVertex(int iIndex,const float *pPos,const float * pNormal)
{
	ASSERT(iIndex>=0 && iIndex<m_iNumVertices);	//don't overflow buffer
	m_pVertices[iIndex].m_fPos[0]=pPos[0];
	m_pVertices[iIndex].m_fPos[1]=pPos[1];
	m_pVertices[iIndex].m_fPos[2]=pPos[2];
	m_pVertices[iIndex].m_fNormal[0]=pNormal[0];
	m_pVertices[iIndex].m_fNormal[1]=pNormal[1];
	m_pVertices[iIndex].m_fNormal[2]=pNormal[2];
}

