// sspStringObjects.cpp: implementation of the SSpWavFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "sspStringObjects.h"
#include "sspPools.h"

using namespace std;

IMPLEMENT_SERIAL( sspSimpleString, CObject, 1 )
IMPLEMENT_SERIAL( sspAlphabetString, CObject, 1 )
IMPLEMENT_SERIAL( sspConcatString, CObject, 1 )

//////////////////////////////////////////////////////////////////////
// sspSimpleString
//////////////////////////////////////////////////////////////////////

sspSimpleString::sspSimpleString() 
: sspStringObject(SSP_SIMPLE_STRING), m_str(_T(""))
{
}

sspSimpleString::sspSimpleString(const sspSimpleString& other) 
: sspStringObject(other), m_str(other.m_str)
{
}

sspSimpleString& sspSimpleString::operator= (const sspSimpleString& other)
{
	sspStringObject::operator=(other);
	m_str = other.m_str;
	return *this;
}

sspSimpleString::~sspSimpleString()
{
}

void sspSimpleString::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring())
    ar << CString(m_str.c_str());
  else {
    CString dummy;
    ar >> dummy;
    m_str = dummy.GetBuffer();
  }
}

void sspSimpleString::printASCII(sspOutStream& outStr)
{
	outStr << endl << m_nIndex << ": sspSimpleString " << m_strName;
	outStr << endl << "   - " << m_str;
}

//////////////////////////////////////////////////////////////////////
// sspAlphabetString
//////////////////////////////////////////////////////////////////////

sspAlphabetString::sspAlphabetString() 
: sspStringObject(SSP_ALPHABET_STRING), m_nValueIndex(0)
{
}

sspAlphabetString::sspAlphabetString(const sspAlphabetString& other) 
: sspStringObject(other), m_strVec(other.m_strVec), m_nValueIndex(other.m_nValueIndex)
{
}

sspAlphabetString& sspAlphabetString::operator= (const sspAlphabetString& other)
{
	sspStringObject::operator=(other);
	m_strVec = other.m_strVec;
  m_nValueIndex = other.m_nValueIndex;
	return *this;
}

sspAlphabetString::~sspAlphabetString()
{
}

void sspAlphabetString::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
  if (ar.IsStoring()) {
    ar << m_strVec.size() << m_nValueIndex;
    for (size_t i=0; i<m_strVec.size(); ++i)
      ar << CString(m_strVec[i].c_str());
  }
  else {
    int nBuf;
    CString dummy;
    ar >> nBuf >> m_nValueIndex;
    m_strVec.resize(nBuf);
    for (size_t i=0; i<m_strVec.size(); ++i) {
      ar >> dummy;
      m_strVec[i] = dummy.GetBuffer();
    }
  }
}

void sspAlphabetString::printASCII(sspOutStream& outStr)
{
	outStr << endl << m_nIndex << ": sspAlphabetString " << m_strName;
  outStr << endl << "   - value index: " << sspPool::Instance().values.GetName(m_nValueIndex);
  for (size_t i=0; i<m_strVec.size(); ++i)
    outStr << endl << "   - string ( " << i << "): " << m_strVec[i]; 
}

bool sspAlphabetString::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	for (size_t i=0; i<m_strVec.size(); ++i) {
    if (m_strVec[i].empty()) {
			printWarning(outStr, _T("(sspAlphabetString): these is an empty string"), nWarnings);
			bRet = false;
		}
	}
	if (m_strVec.size() == 0) {
		printError(outStr, _T("(sspAlphabetString): there are no strings"), nErrors);
		bRet = false;
	}
	if (m_strVec.size() == 1) {
		printWarning(outStr, _T("(sspAlphabetString): there is only one string"), nWarnings);
		bRet = false;
	}
	if (m_nValueIndex < 0 || m_nValueIndex >= (int) sspPool::Instance().values.GetSize()) {
		printError(outStr, _T("(sspAlphabetString): m_nValueIndex is not valid"), nErrors);
		bRet = false;
	}
	return bRet;
}

sspString	sspAlphabetString::getString()
{
	float fVal = sspPool::Instance().values.getValue(m_nValueIndex)->getValue();
	size_t nIndex = fVal < 0 ? 0 : static_cast<size_t>(fVal + 0.5f);  	// rounding
	if (nIndex >= m_strVec.size()) nIndex = m_strVec.size() - 1;
  return m_strVec[nIndex];
}

//////////////////////////////////////////////////////////////////////
// sspConcatString
//////////////////////////////////////////////////////////////////////

sspConcatString::sspConcatString() 
: sspStringObject(SSP_CONCAT_STRING)
{
}

sspConcatString::sspConcatString(const sspConcatString& other) 
: sspStringObject(other), m_strIndices(other.m_strIndices)
{
}

sspConcatString& sspConcatString::operator= (const sspConcatString& other)
{
	sspStringObject::operator=(other);
	m_strIndices = other.m_strIndices;
	return *this;
}

sspConcatString::~sspConcatString()
{
}

void sspConcatString::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
  if (ar.IsStoring()) {
    ar << m_strIndices.size();
    for (size_t i=0; i<m_strIndices.size(); ++i)
      ar << m_strIndices[i];
  }
  else {
    int nBuf;
    ar >> nBuf;
    m_strIndices.resize(nBuf);
    for (size_t i=0; i<m_strIndices.size(); ++i) {
      ar >> m_strIndices[i];
    }
  }
}

void sspConcatString::printASCII(sspOutStream& outStr)
{
	outStr << endl << m_nIndex << ": sspConcatString " << m_strName;
  for (size_t i=0; i<m_strIndices.size(); ++i)
    outStr << endl << "   - string ( " << i << "): " << sspPool::Instance().strings.GetName(m_strIndices[i]); 
}

bool sspConcatString::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	for (unsigned int i=0; i<m_strIndices.size(); ++i) {
		if (m_strIndices[i] < 0 || m_strIndices[i] >= (int) sspPool::Instance().strings.GetSize()) {
			printError(outStr, _T("(sspConcatString): a string object index is not valid"), nErrors);
			bRet = false;
		}
	}
	if (m_strIndices.size() == 0) {
		printError(outStr, _T("(sspConcatString): there are no string objects"), nErrors);
		bRet = false;
	}
	if (m_strIndices.size() == 1) {
		printWarning(outStr, _T("(sspConcatString): there is only a single string object"), nWarnings);
		bRet = false;
	}
	return bRet;
}

sspString	sspConcatString::getString()
{
	sspString strCat(_T(""));
  for (size_t i=0; i<m_strIndices.size(); ++i)
    strCat += sspPool::Instance().strings.getString(m_strIndices[i])->getString();
  return strCat;
}

