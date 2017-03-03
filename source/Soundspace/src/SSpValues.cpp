// SSpValues.cpp: implementation of the SSpFloat class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SoundSpace.h"
#include "SSpValues.h"
#include "sspPools.h"
#include "sspDateTimeManager.h"
#include "sspPlayManager.h"

#include <algorithm>
using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL( SSpBasicValue, CObject, 1 )
IMPLEMENT_SERIAL( SSpInputValueRange, CObject, 1 )
IMPLEMENT_SERIAL( SSpTimeMap, CObject, 1 )
IMPLEMENT_SERIAL( SSpDateMap, CObject, 1 )
IMPLEMENT_SERIAL( SSpLinearMap, CObject, 1 )
IMPLEMENT_SERIAL( SSpAddValue, CObject, 1 )
IMPLEMENT_SERIAL( SSpProductValue, CObject, 1 )
IMPLEMENT_SERIAL( SSpConditionalValue, CObject, 1 )
IMPLEMENT_SERIAL( SSpRandomValue, CObject, 1 )
IMPLEMENT_SERIAL( sspSecondsRun, CObject, 1 )

//////////////////////////////////////////////////////////////////////
// SSpBasicValue
//////////////////////////////////////////////////////////////////////

SSpBasicValue::SSpBasicValue() : sspValue(SSP_VALUE_SINGLE)
{
	m_fVal = 0.0f;
}

SSpBasicValue::SSpBasicValue(const SSpBasicValue& val) : sspValue(val)
{
	m_fVal = val.m_fVal;
}

SSpBasicValue& SSpBasicValue::operator= (const SSpBasicValue& val)
{
	sspValue::operator=(val);
	m_fVal = val.m_fVal;
	return *this;
}

SSpBasicValue::~SSpBasicValue()
{
}

void SSpBasicValue::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring())
		ar << m_fVal;
	else
		ar >> m_fVal;
}

void SSpBasicValue::printASCII(sspOutStream& outStr)
{
	outStr << endl << m_nIndex << ": SSpBasicValue " << m_strName;
	outStr << endl << "   - m_fVal: " << m_fVal;
}

//////////////////////////////////////////////////////////////////////
// SSpInputValueRange
//////////////////////////////////////////////////////////////////////

SSpInputValueRange::SSpInputValueRange() : sspValue(SSP_VALUE_INPUT_RANGE)
{
	m_fMin = 0.0f;
	m_fMax = 1.0f;
	m_fVal = (m_fMin + m_fMax ) * 0.5f;
}

SSpInputValueRange::SSpInputValueRange(const SSpInputValueRange& val) : sspValue(val)
{
	m_fVal = val.m_fVal;
	m_fMin = val.m_fMin;
	m_fMax = val.m_fMax;
}

SSpInputValueRange& SSpInputValueRange::operator= (const SSpInputValueRange& val)
{
	sspValue::operator=(val);
	m_fVal = val.m_fVal;
	m_fMin = val.m_fMin;
	m_fMax = val.m_fMax;
	return *this;
}

SSpInputValueRange::~SSpInputValueRange()
{
}

void SSpInputValueRange::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring())
		ar << m_fVal << m_fMin << m_fMax;
	else
		ar >> m_fVal >> m_fMin >> m_fMax;;
}

void SSpInputValueRange::printASCII(sspOutStream& outStr)
{
	outStr << endl << m_nIndex << ": SSpInputValueRange " << m_strName;
	outStr << endl << "   - m_fMin: " << m_fMin << ", m_fMax: " << m_fMax;
	outStr << endl << "   - (default m_fVal: " << m_fVal << ")";
}

bool SSpInputValueRange::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	if (m_fMin > m_fMax) {
		printError(outStr, _T("(SSpInputValueRange): m_fMin is larger than m_fMax"), nErrors);
		bRet = false;
	}
	if (m_fMin == m_fMax) {
		printWarning(outStr, _T("(SSpInputValueRange): m_fMin is equal to m_fMax"), nWarnings);
		bRet = false;
	}
	if (m_fVal < m_fMin || m_fVal > m_fMax) {
		printError(outStr, _T("(SSpInputValueRange): m_fVal is out of range"), nErrors);
		bRet = false;
	}
	return bRet;
}

void SSpInputValueRange::setValue(float fVal) {
	if (fVal < m_fMin)
		m_fVal = m_fMin;
	else if (fVal > m_fMax)
		m_fVal = m_fMax;
	else
		m_fVal = fVal;
}

void SSpInputValueRange::setValueRange(float fMin, float fMax)
{
	if (fMin < fMax) {
		m_fMin = fMin;
		m_fMax = fMax;
	}
	else {
		m_fMin = fMax;
		m_fMax = fMin;
	}
	setValue(m_fVal);
}

void SSpInputValueRange::setRelativeValue(float fVal)
{
	if (fVal < 0.0)
		m_fVal = m_fMin;
	else if (fVal > 1.0)
		m_fVal = m_fMax;
	else
		m_fVal = m_fMin + (m_fMax - m_fMin) * fVal;
}

float SSpInputValueRange::getRelativeValue() const
{
  return (m_fVal - m_fMin) / (m_fMax - m_fMin);
}

//////////////////////////////////////////////////////////////////////
// SSpTimeMap
//////////////////////////////////////////////////////////////////////

SSpTimeMap::SSpTimeMap() : sspValue(SSP_VALUE_CLOCK)
{
	m_fValStart = 0.0f;
	m_fValEnd = 1.0f;
	m_fSeconds = (float) (m_clockEnd - m_clockStart);
}

SSpTimeMap::SSpTimeMap(const SSpTimeMap& val) : sspValue(val)
{
	m_fValStart = val.m_fValStart;
	m_fValEnd = val.m_fValEnd;
	m_clockStart = val.m_clockStart;
	m_clockEnd = val.m_clockEnd;
	m_fSeconds = (float) (m_clockEnd - m_clockStart);
}

SSpTimeMap& SSpTimeMap::operator= (const SSpTimeMap& val)
{
	sspValue::operator=(val);
	m_fValStart = val.m_fValStart;
	m_fValEnd = val.m_fValEnd;
	m_clockStart = val.m_clockStart;
	m_clockEnd = val.m_clockEnd;
	m_fSeconds = (float) (m_clockEnd - m_clockStart);
	return *this;
}

SSpTimeMap::~SSpTimeMap()
{
}

void SSpTimeMap::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	m_clockStart.Serialize(ar);
	m_clockEnd.Serialize(ar);
	if (ar.IsStoring())
		ar << m_fValStart << m_fValEnd;
	else {
		ar >> m_fValStart >> m_fValEnd;
  	m_fSeconds = (float) (m_clockEnd - m_clockStart);
	}
}

void SSpTimeMap::printASCII(sspOutStream& outStr)
{
	outStr << endl << m_nIndex << ": SSpTimeMap " << m_strName;
	outStr << endl << "   - m_clockStart: " << m_clockStart.hour() << ":" << m_clockStart.min() << ":" << m_clockStart.sec();
	outStr << endl << "   - m_clockEnd: " << m_clockEnd.hour() << ":" << m_clockEnd.min() << ":" << m_clockEnd.sec();
	outStr << endl << "   - m_fValStart: " << m_fValStart << ", m_fValEnd: " << m_fValEnd;
}

bool SSpTimeMap::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	if (m_fSeconds < 0.0f) {
		printError(outStr, _T("(SSpTimeMap): m_clockStart is after m_clockEnd"), nErrors);
		bRet = false;
	}
	if (m_fSeconds == 0.0f) {
		printWarning(outStr, _T("(SSpTimeMap): m_clockStart is equal to m_clockEnd"), nWarnings);
		bRet = false;
	}
	if (!m_clockStart.isValid()) {
		printError(outStr, _T("(SSpTimeMap): m_clockStart is not valid"), nErrors);
		bRet = false;
	}
	if (!m_clockEnd.isValid()) {
		printError(outStr, _T("(SSpTimeMap): m_clockEnd is not valid"), nErrors);
		bRet = false;
	}
	return bRet;
}

float SSpTimeMap::getValue()
{
	if (m_fSeconds == 0.0f) return 0.0f;

  sspClock now = (sspClock) sspDateTimeManager::Instance().current();
	float fVal = m_fValStart + (m_fValEnd - m_fValStart) * ((now - m_clockStart) / m_fSeconds);
	if (m_fValStart < m_fValEnd) {
		if (fVal < m_fValStart) 
      return m_fValStart;
		else if (fVal > m_fValEnd) 
      return m_fValEnd;
		else
      return fVal;
	}
	else {
		if (fVal > m_fValStart) 
      return m_fValStart;
		else if (fVal < m_fValEnd) 
      return m_fValEnd;
    else
		  return fVal;
	}
}

void SSpTimeMap::setStartClock(SSpClock& clock)
{
	m_clockStart = clock;
 	m_fSeconds = (float) (m_clockEnd - m_clockStart);
}

void SSpTimeMap::setEndClock(SSpClock& clock)
{
	m_clockEnd = clock;
 	m_fSeconds = (float) (m_clockEnd - m_clockStart);
}

//////////////////////////////////////////////////////////////////////
// SSpDateMap
//////////////////////////////////////////////////////////////////////

SSpDateMap::SSpDateMap() : sspValue(SSP_VALUE_DATE)
{
	m_fValStart = 0.0f;
	m_fValEnd = 1.0f;
	m_fDays = (float) (m_dateEnd - m_dateStart);
}

SSpDateMap::SSpDateMap(const SSpDateMap& val) : sspValue(val)
{
	m_fValStart = val.m_fValStart;
	m_fValEnd = val.m_fValEnd;
	m_dateStart = val.m_dateStart;
	m_dateEnd = val.m_dateEnd;
	m_fDays = (float) (m_dateEnd - m_dateStart);
}

SSpDateMap& SSpDateMap::operator= (const SSpDateMap& val)
{
	sspValue::operator=(val);
	m_fValStart = val.m_fValStart;
	m_fValEnd = val.m_fValEnd;
	m_dateStart = val.m_dateStart;
	m_dateEnd = val.m_dateEnd;
	m_fDays = (float) (m_dateEnd - m_dateStart);
	return *this;
}

SSpDateMap::~SSpDateMap()
{
}

void SSpDateMap::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	m_dateStart.Serialize(ar);
	m_dateEnd.Serialize(ar);
	if (ar.IsStoring())
		ar << m_fValStart << m_fValEnd;
	else {
		ar >> m_fValStart >> m_fValEnd;
  	m_fDays = (float) (m_dateEnd - m_dateStart);
	}
}

void SSpDateMap::printASCII(sspOutStream& outStr)
{
	outStr << endl << m_nIndex << ": SSpDateMap " << m_strName;
  outStr << endl << "   - m_dateStart: " << m_dateStart.month() << "//" << m_dateStart.day();
	outStr << endl << "   - m_dateEnd: " << m_dateEnd.month() << "//" << m_dateEnd.day();
	outStr << endl << "   - m_fValStart: " << m_fValStart << ", m_fValEnd: " << m_fValEnd;
}

bool SSpDateMap::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	if (m_fDays < 0.0f) {
		printError(outStr, _T("(SSpDateMap): m_dateStart is after m_dateEnd"), nErrors);
		bRet = false;
	}
	if (m_fDays == 0.0f) {
		printWarning(outStr, _T("(SSpDateMap): m_dateStart is equal to m_dateEnd"), nWarnings);
		bRet = false;
	}
	if (!m_dateStart.isValid()) {
		printError(outStr, _T("(SSpDateMap): m_dateStart is not valid"), nErrors);
		bRet = false;
	}
	if (!m_dateEnd.isValid()) {
		printError(outStr, _T("(SSpDateMap): m_dateEnd is not valid"), nErrors);
		bRet = false;
	}
	return bRet;
}

float SSpDateMap::getValue()
{
  sspDate now = (sspDate) sspDateTimeManager::Instance().current();
	float fVal = m_fValStart + (m_fValEnd - m_fValStart) * ((now - m_dateStart) / m_fDays);
	if (m_fValStart < m_fValEnd) {
		if (fVal < m_fValStart) return m_fValStart;
		if (fVal > m_fValEnd) return m_fValEnd;
		return fVal;
	}
	else {
		if (fVal > m_fValStart) return m_fValStart;
		if (fVal < m_fValEnd) return m_fValEnd;
		return fVal;
	}
}

void SSpDateMap::setStartDate(SSpDate& date)
{
	m_dateStart = date;
	m_fDays = (float) (m_dateEnd - m_dateStart);
}

void SSpDateMap::setEndDate(SSpDate& date)
{
	m_dateEnd = date;
	m_fDays = (float) (m_dateEnd - m_dateStart);
}

//////////////////////////////////////////////////////////////////////
// SSpLinearMap
//////////////////////////////////////////////////////////////////////

SSpLinearMap::SSpLinearMap() : sspValue(SSP_VALUE_MAP)
{
	m_nValueIndex = -1;
	m_fInputStart = 0.0f;
	m_fInputEnd = 1.0f;
	m_fOutputStart = 0.0f;
	m_fOutputEnd = 1.0f;
}

SSpLinearMap::SSpLinearMap(const SSpLinearMap& val) : sspValue(val)
{
	m_nValueIndex = val.m_nValueIndex;
	m_fInputStart = val.m_fInputStart;
	m_fInputEnd = val.m_fInputEnd;
	m_fOutputStart = val.m_fOutputStart;
	m_fOutputEnd = val.m_fOutputEnd;
}

SSpLinearMap& SSpLinearMap::operator= (const SSpLinearMap& val)
{
	sspValue::operator=(val);
	m_nValueIndex = val.m_nValueIndex;
	m_fInputStart = val.m_fInputStart;
	m_fInputEnd = val.m_fInputEnd;
	m_fOutputStart = val.m_fOutputStart;
	m_fOutputEnd = val.m_fOutputEnd;
	return *this;
}

SSpLinearMap::~SSpLinearMap()
{
}

void SSpLinearMap::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring())
		ar << m_nValueIndex << m_fInputStart << m_fInputEnd << m_fOutputStart << m_fOutputEnd;
	else {
		ar >> m_nValueIndex >> m_fInputStart >> m_fInputEnd >> m_fOutputStart >> m_fOutputEnd;
	}
}

void SSpLinearMap::printASCII(sspOutStream& outStr)
{
	outStr << endl << m_nIndex << ": SSpLinearMap " << m_strName;
	outStr << endl << "   - m_fInputStart: " << m_fInputStart << ", m_fInputEnd: " << m_fInputEnd;
	outStr << endl << "   - m_fOutputStart: " << m_fOutputStart << ", m_fOutputEnd: " << m_fOutputEnd;
	outStr << endl << "   - m_nValueIndex: " << sspPool::Instance().values.GetName(m_nValueIndex);
}

bool SSpLinearMap::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	if (m_nValueIndex == m_nIndex || m_nValueIndex < 0 
		|| m_nValueIndex > (int) sspPool::Instance().values.GetSize()) {
		printError(outStr, _T("(SSpLinearMap): m_nValueIndex is not valid"), nErrors);
		bRet = false;
	}
	if (m_fInputStart == m_fInputEnd) {
		printError(outStr, _T("(SSpLinearMap): m_fInputStart is equal to m_fInputEnd"), nErrors);
		bRet = false;
	}
	if (m_fOutputStart == m_fOutputEnd) {
		printWarning(outStr, _T("(SSpLinearMap): m_fInputStart is equal to m_fInputEnd"), nWarnings);
		bRet = false;
	}
	return bRet;
}

float SSpLinearMap::getValue()
{
	float fVal = sspPool::Instance().values.getValue(m_nValueIndex)->getValue();
	fVal = m_fOutputStart + (m_fOutputEnd - m_fOutputStart) * 
		((fVal - m_fInputStart) / (m_fInputEnd - m_fInputStart));
	if (m_fOutputStart < m_fOutputEnd) {
		if (fVal < m_fOutputStart) return m_fOutputStart;
		if (fVal > m_fOutputEnd) return m_fOutputEnd;
		return fVal;
	}
	else {
		if (fVal > m_fOutputStart) return m_fOutputStart;
		if (fVal < m_fOutputEnd) return m_fOutputEnd;
		return fVal;
	}
}

//////////////////////////////////////////////////////////////////////
// SSpAddValue
//////////////////////////////////////////////////////////////////////

SSpAddValue::SSpAddValue() : sspValue(SSP_VALUE_SUM)
{
}

SSpAddValue::SSpAddValue(const SSpAddValue& val) : sspValue(val)
{
	m_nValues = val.m_nValues;
}

SSpAddValue& SSpAddValue::operator= (const SSpAddValue& val)
{
	sspValue::operator=(val);
	m_nValues = val.m_nValues;
	return *this;
}

SSpAddValue::~SSpAddValue()
{
}

void SSpAddValue::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring()) {
		ar << m_nValues.size();
		for (unsigned int i=0; i<m_nValues.size(); ++i) {
			ar << m_nValues[i];
		}
	}
	else {
		unsigned int nBuf;
		ar >> nBuf;
		m_nValues.resize(nBuf);
		for (unsigned int i=0; i<nBuf; i++)
			ar >> m_nValues[i];
	}
}

void SSpAddValue::printASCII(sspOutStream& outStr)
{
	outStr << endl << m_nIndex << ": SSpAddValue " << m_strName;
	for (unsigned int i=0; i<m_nValues.size(); ++i) {
		outStr << endl << "   - value " << i << ": " << sspPool::Instance().values.GetName(m_nValues[i]);
	}
}

bool SSpAddValue::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	for (unsigned int i=0; i<m_nValues.size(); ++i) {
		if (m_nValues[i] == m_nIndex || m_nValues[i] < 0 
			|| m_nValues[i] > (int) sspPool::Instance().values.GetSize()) {
			printError(outStr, _T("(SSpAddValue): a value index is not valid"), nErrors);
			bRet = false;
		}
	}
	if (m_nValues.size() == 0) {
		printError(outStr, _T("(SSpAddValue): there are no values"), nErrors);
		bRet = false;
	}
	if (m_nValues.size() == 1) {
		printWarning(outStr, _T("(SSpAddValue): there is only a single value"), nWarnings);
		bRet = false;
	}
	return bRet;
}

float SSpAddValue::getValue()
{
	float fVal = sspPool::Instance().values.getValue(m_nValues[0])->getValue();
	for (unsigned int i=1; i<m_nValues.size(); i++)
		fVal += sspPool::Instance().values.getValue(m_nValues[i])->getValue();
	return fVal;
}

//////////////////////////////////////////////////////////////////////
// SSpProductValue
//////////////////////////////////////////////////////////////////////

SSpProductValue::SSpProductValue() : sspValue(SSP_VALUE_PRODUCT)
{
}

SSpProductValue::SSpProductValue(const SSpProductValue& val) : sspValue(val)
{
	m_nValues = val.m_nValues;
}

SSpProductValue& SSpProductValue::operator= (const SSpProductValue& val)
{
	sspValue::operator=(val);
	m_nValues = val.m_nValues;
	return *this;
}

SSpProductValue::~SSpProductValue()
{
}

void SSpProductValue::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring()) {
		ar << m_nValues.size();
		for (unsigned int i=0; i<m_nValues.size(); ++i) {
			ar << m_nValues[i];
		}
	}
	else {
		unsigned int nBuf;
		ar >> nBuf;
		m_nValues.resize(nBuf);
		for (unsigned int i=0; i<nBuf; i++)
			ar >> m_nValues[i];
	}
}

void SSpProductValue::printASCII(sspOutStream& outStr)
{
	outStr << endl << m_nIndex << ": SSpProductValue " << m_strName;
	for (unsigned int i=0; i<m_nValues.size(); ++i) {
		outStr << endl << "   - value " << i << ": " << sspPool::Instance().values.GetName(m_nValues[i]);
	}
}

bool SSpProductValue::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	for (unsigned int i=0; i<m_nValues.size(); ++i) {
		if (m_nValues[i] == m_nIndex || m_nValues[i] < 0 
			|| m_nValues[i] > (int) sspPool::Instance().values.GetSize()) {
			printError(outStr, _T("(SSpProductValue): a value index is not valid"), nErrors);
			bRet = false;
		}
	}
	if (m_nValues.size() == 0) {
		printError(outStr, _T("(SSpProductValue): there are no values"), nErrors);
		bRet = false;
	}
	if (m_nValues.size() == 1) {
		printWarning(outStr, _T("(SSpProductValue): there is only a single value"), nWarnings);
		bRet = false;
	}
	return bRet;
}

float SSpProductValue::getValue()
{
	float fVal = sspPool::Instance().values.getValue(m_nValues[0])->getValue();
	for (unsigned int i=1; i<m_nValues.size(); i++)
		fVal *= sspPool::Instance().values.getValue(m_nValues[i])->getValue();
	return fVal;
}

//////////////////////////////////////////////////////////////////////
// SSpConditionalValue
//////////////////////////////////////////////////////////////////////

SSpConditionalValue::SSpConditionalValue() : sspValue(SSP_VALUE_CONDITIONAL)
{
	m_nDefaultValue = -1;
}

SSpConditionalValue::SSpConditionalValue(const SSpConditionalValue& val) : sspValue(val)
{
	m_nConditionals = val.m_nConditionals;
	m_nValues = val.m_nValues;
	m_nDefaultValue = val.m_nDefaultValue;
}

SSpConditionalValue& SSpConditionalValue::operator= (const SSpConditionalValue& val)
{
	sspValue::operator=(val);
	m_nConditionals = val.m_nConditionals;
	m_nValues = val.m_nValues;
	m_nDefaultValue = val.m_nDefaultValue;
	return *this;
}

SSpConditionalValue::~SSpConditionalValue()
{
}

void SSpConditionalValue::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring()) {
		int nSize = min(m_nValues.size(), m_nConditionals.size());
		ar << nSize;
		for (int i=0; i<nSize; ++i) {
			ar << m_nValues[i] << m_nConditionals[i];
		}
		ar << m_nDefaultValue;
	}
	else {
		int nBuf;
		ar >> nBuf;
		m_nValues.resize(nBuf);
		m_nConditionals.resize(nBuf);
		for (int i=0; i<nBuf; i++)
			ar >> m_nValues[i] >> m_nConditionals[i];
		ar >> m_nDefaultValue;
	}
}

void SSpConditionalValue::printASCII(sspOutStream& outStr)
{
	outStr << endl << m_nIndex << ": SSpConditionalValue " << m_strName;
	for (unsigned int i=0; i<m_nValues.size(); ++i) {
		outStr << endl << "   - value " << i << ": " << sspPool::Instance().values.GetName(m_nValues[i]);
		outStr << "   with conditional: " << sspPool::Instance().conditionals.GetName(m_nConditionals[i]);
	}
	outStr << endl << "   - default value: " << sspPool::Instance().values.GetName(m_nDefaultValue);
}

bool SSpConditionalValue::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	for (unsigned int i=0; i<m_nValues.size(); ++i) {
		if (m_nValues[i] == m_nIndex || m_nValues[i] < 0 
			|| m_nValues[i] > (int) sspPool::Instance().values.GetSize()) {
			printError(outStr, _T("(SSpConditionalValue): a value index is not valid"), nErrors);
			bRet = false;
		}
		if (m_nConditionals[i] < 0 || m_nConditionals[i] > (int) sspPool::Instance().conditionals.GetSize()) {
			printError(outStr, _T("(SSpConditionalValue): a conditional index is not valid"), nErrors);
			bRet = false;
		}
	}
	if (m_nValues.size() != m_nConditionals.size()) {
		printError(outStr, _T("(SSpConditionalValue): the number of values and conditionals do not match"), nErrors);
		bRet = false;
	}
	if (m_nValues.size() == 0) {
		printError(outStr, _T("(SSpConditionalValue): there are no values"), nErrors);
		bRet = false;
	}
	if (m_nValues.size() == 1) {
		printWarning(outStr, _T("(SSpConditionalValue): there is only a single value"), nWarnings);
		bRet = false;
	}
	if (m_nDefaultValue == m_nIndex || m_nDefaultValue < 0 
		|| m_nDefaultValue > (int) sspPool::Instance().values.GetSize()) {
		printError(outStr, _T("(SSpConditionalValue): m_nDefaultValue is not valid"), nErrors);
		bRet = false;
	}
	return bRet;
}

float SSpConditionalValue::getValue()
{
	int nSize = min(m_nValues.size(), m_nConditionals.size());
	for (int i=0; i<nSize; i++) {
		if (sspPool::Instance().conditionals.GetConditional(m_nConditionals[i])->isTrue())
			return sspPool::Instance().values.getValue(m_nValues[i])->getValue();
	}
	return sspPool::Instance().values.getValue(m_nDefaultValue)->getValue();
}

//////////////////////////////////////////////////////////////////////
// SSpRandomValue
//////////////////////////////////////////////////////////////////////

SSpRandomValue::SSpRandomValue() : sspValue(SSP_VALUE_RANDOM)
{
	m_nValueLow = -1;
	m_nValueHigh = -1;
}

SSpRandomValue::SSpRandomValue(const SSpRandomValue& val) : sspValue(val)
{
	m_nValueLow = val.m_nValueLow;
	m_nValueHigh = val.m_nValueHigh;
}

SSpRandomValue& SSpRandomValue::operator= (const SSpRandomValue& val)
{
	sspValue::operator=(val);
	m_nValueLow = val.m_nValueLow;
	m_nValueHigh = val.m_nValueHigh;
	return *this;
}

SSpRandomValue::~SSpRandomValue()
{
}

void SSpRandomValue::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring())
		ar << m_nValueLow << m_nValueHigh;
	else
		ar >> m_nValueLow >> m_nValueHigh;
}

void SSpRandomValue::printASCII(sspOutStream& outStr)
{
	outStr << endl << m_nIndex << ": SSpRandomValue " << m_strName;
	outStr << endl << "   - m_nValueLow: " << m_nValueLow << ", m_nValueHigh: " << m_nValueHigh;
}

bool SSpRandomValue::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	if (sspPool::Instance().values.getValue(m_nValueLow)->getValue() > sspPool::Instance().values.getValue(m_nValueHigh)->getValue()) {
		printError(outStr, _T("(SSpRandomValue): m_nValueLow is larger than m_nValueHigh"), nErrors);
		bRet = false;
	}
	if (sspPool::Instance().values.getValue(m_nValueLow)->getValue() == sspPool::Instance().values.getValue(m_nValueHigh)->getValue()) {
		printWarning(outStr, _T("(SSpRandomValue): m_nValueLow is equal to m_nValueHigh"), nWarnings);
		bRet = false;
	}
	return bRet;
}

float SSpRandomValue::getValue()
{
	float fVal1 = sspPool::Instance().values.getValue(m_nValueLow)->getValue();
	float fVal2 = sspPool::Instance().values.getValue(m_nValueHigh)->getValue();
  float fRet = fVal1 + (fVal2 - fVal1) * ((float) rand()) / RAND_MAX;
  return fRet;
}

//////////////////////////////////////////////////////////////////////
// sspSecondsRun
//////////////////////////////////////////////////////////////////////

sspSecondsRun::sspSecondsRun() : sspValue(SSP_VALUE_SECONDSRUN)
{
}

sspSecondsRun::sspSecondsRun(const sspSecondsRun& val) : sspValue(val)
{
}

sspSecondsRun& sspSecondsRun::operator= (const sspSecondsRun& val)
{
	sspValue::operator=(val);
	return *this;
}

sspSecondsRun::~sspSecondsRun()
{
}

void sspSecondsRun::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
}

float sspSecondsRun::getValue()
{
  return (float) sspPlayManager::Instance().secondsRunning();
}

void sspSecondsRun::printASCII(sspOutStream& outStr)
{
	outStr << endl << m_nIndex << ": sspSecondsRun " << m_strName;
}
