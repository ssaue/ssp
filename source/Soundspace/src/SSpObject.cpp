// sspObject.cpp: implementation of the SSpValue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "sspObject.h"

using namespace std;

//////////////////////////////////////////////////////////////////////
// sspIObase
//////////////////////////////////////////////////////////////////////

sspIObase::sspIObase(const sspIObase& iob)
{
	m_strName = iob.m_strName;
}

sspIObase& sspIObase::operator= (const sspIObase& iob)
{
	m_strName = iob.m_strName;
	return *this;
}

void sspIObase::printError(sspOutStream& outStr, const sspString& txt, int& nCount)
{
	outStr << endl << "ERROR: " << m_strName << txt;
	nCount++;
}

void sspIObase::printWarning(sspOutStream& outStr, const sspString& txt, int& nCount)
{
	outStr << endl << "WARNING: " << m_strName << txt;
	nCount++;
}

//////////////////////////////////////////////////////////////////////
// sspObject
//////////////////////////////////////////////////////////////////////

sspObject::sspObject(const sspObject& obj) : sspIObase(obj)
{
	m_nClass = obj.m_nClass;
	m_nType = obj.m_nType;
	m_nIndex = obj.m_nIndex;
}

sspObject& sspObject::operator= (const sspObject& obj)
{
	sspIObase::operator=(obj);
	m_nClass = obj.m_nClass;
	m_nType = obj.m_nType;
	m_nIndex = obj.m_nIndex;
	return *this;
}

