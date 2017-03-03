// SSpConditionals.cpp: implementation of the SSpBoolean class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SoundSpace.h"
#include "SSpConditionals.h"
#include "sspDateTimeManager.h"
#include "SSpTasks.h"
#include "sspPools.h"
using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL( SSpBoolean, CObject, 1 )
IMPLEMENT_SERIAL( SSpValueRelation, CObject, 1 )
IMPLEMENT_SERIAL( SSpInRange, CObject, 1 )
IMPLEMENT_SERIAL( SSpTimeInterval, CObject, 1 )
IMPLEMENT_SERIAL( SSpTimeRelation, CObject, 1 )
IMPLEMENT_SERIAL( SSpMinuteRelation, CObject, 1 )
IMPLEMENT_SERIAL( SSpDateInterval, CObject, 1 )
IMPLEMENT_SERIAL( SSpIsPlaying, CObject, 1 )
IMPLEMENT_SERIAL( SSpAnd, CObject, 1 )
IMPLEMENT_SERIAL( SSpOr, CObject, 1 )
IMPLEMENT_SERIAL( SSpNot, CObject, 1 )
IMPLEMENT_SERIAL( SSpTrigger, CObject, 1 )
IMPLEMENT_SERIAL( SSpDayOfWeek, CObject, 1 )

//////////////////////////////////////////////////////////////////////
// SSpBoolean
//////////////////////////////////////////////////////////////////////

SSpBoolean::SSpBoolean() : sspConditional(SSP_COND_BOOLEAN)
{
	m_bValue = true;
}

SSpBoolean::SSpBoolean(const SSpBoolean& cond) : sspConditional(cond)
{
	m_bValue = cond.m_bValue;
}

SSpBoolean& SSpBoolean::operator= (const SSpBoolean& cond)
{
	sspConditional::operator=(cond);
	m_bValue = cond.m_bValue;
	return *this;
}

SSpBoolean::~SSpBoolean()
{
}

void SSpBoolean::Serialize(CArchive& ar)
{
	BOOL bTrue; 

	CObject::Serialize(ar);
	if (ar.IsStoring()) {
		bTrue = m_bValue;
		ar << bTrue;
	}
	else {
		ar >> bTrue;
		m_bValue = (bTrue == TRUE);
	}
}

void SSpBoolean::printASCII(sspOutStream& outStr)
{
	outStr << endl << m_nIndex << ": SSpBoolean " << m_strName;
	outStr << endl << boolalpha << "   - m_bValue: " << m_bValue << noboolalpha;
}

bool SSpBoolean::isTrue()
{
	return m_bValue;
}

//////////////////////////////////////////////////////////////////////
// SSpValueRelation
//////////////////////////////////////////////////////////////////////

SSpValueRelation::SSpValueRelation() : sspConditional(SSP_COND_RELATION)
{
	m_nRelation = EQUAL;
	m_nTestIndex = m_nThresholdIndex = -1;
}

SSpValueRelation::SSpValueRelation(const SSpValueRelation& cond) : sspConditional(cond)
{
	m_nRelation = cond.m_nRelation;
	m_nTestIndex = cond.m_nTestIndex;
	m_nThresholdIndex = cond.m_nThresholdIndex;
}

SSpValueRelation& SSpValueRelation::operator= (const SSpValueRelation& cond)
{
	sspConditional::operator=(cond);
	m_nRelation = cond.m_nRelation;
	m_nTestIndex = cond.m_nTestIndex;
	m_nThresholdIndex = cond.m_nThresholdIndex;
	return *this;
}

SSpValueRelation::~SSpValueRelation()
{
}

void SSpValueRelation::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring())
		ar << m_nTestIndex << m_nThresholdIndex << m_nRelation;
	else
		ar >> m_nTestIndex >> m_nThresholdIndex >> m_nRelation;
}

void SSpValueRelation::printASCII(sspOutStream& outStr)
{
	outStr << endl << m_nIndex << ": SSpValueRelation " << m_strName;
	switch (m_nRelation) {
	case EQUAL:
		outStr << endl << "   - m_nRelation: equal";
		break;
	case UNEQUAL:
		outStr << endl << "   - m_nRelation: unequal";
		break;
	case SMALLER:
		outStr << endl << "   - m_nRelation: less than";
		break;
	case LARGER:
		outStr << endl << "   - m_nRelation: larger than";
		break;
	case SMALL_EQ:
		outStr << endl << "   - m_nRelation: less than or equal";
		break;
	case LARGE_EQ:
		outStr << endl << "   - m_nRelation: larger than or equal";
		break;
	default:
		outStr << endl << "   - m_nRelation: unknown";
		break;
	}
	outStr << endl << "   - m_nTestIndex: " << sspPool::Instance().values.GetName(m_nTestIndex);
	outStr << endl << "   - m_nThresholdIndex: " << sspPool::Instance().values.GetName(m_nThresholdIndex);
}

bool SSpValueRelation::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	if (m_nTestIndex < 0 || m_nTestIndex >= (int) sspPool::Instance().values.GetSize()) {
		printError(outStr, _T("(SSpValueRelation): m_nTestIndex is not valid"), nErrors);
		bRet = false;
	}
	if (m_nThresholdIndex < 0 || m_nThresholdIndex >= (int) sspPool::Instance().values.GetSize()) {
		printError(outStr, _T("(SSpValueRelation): m_nThresholdIndex is not valid"), nErrors);
		bRet = false;
	}
	if (m_nRelation < 0 || m_nRelation > LARGE_EQ) {
		printError(outStr, _T("(SSpValueRelation): m_nRelation is not valid"), nErrors);
		bRet = false;
	}
	return bRet;
}

bool SSpValueRelation::isTrue()
{
	float fTest = sspPool::Instance().values.getValue(m_nTestIndex)->getValue();
	float fThreshold = sspPool::Instance().values.getValue(m_nThresholdIndex)->getValue();
	switch (m_nRelation) {
	case EQUAL:
		return fTest == fThreshold;
	case UNEQUAL:
		return fTest != fThreshold;
	case SMALLER:
		return fTest < fThreshold;
	case LARGER:
		return fTest > fThreshold;
	case SMALL_EQ:
		return fTest <= fThreshold;
	case LARGE_EQ:
		return fTest >= fThreshold;
	default:
		return false;
	}
}

//////////////////////////////////////////////////////////////////////
// SSpInRange
//////////////////////////////////////////////////////////////////////

SSpInRange::SSpInRange() : sspConditional(SSP_COND_RANGE)
{
	m_nRangeMin = m_nRangeMax = -1;
	m_nValueIndex = -1;
}

SSpInRange::SSpInRange(const SSpInRange& cond) : sspConditional(cond)
{
	m_nRangeMin = cond.m_nRangeMin;
	m_nRangeMax = cond.m_nRangeMax;
	m_nValueIndex = cond.m_nValueIndex;
}

SSpInRange& SSpInRange::operator= (const SSpInRange& cond)
{
	sspConditional::operator=(cond);
	m_nRangeMin = cond.m_nRangeMin;
	m_nRangeMax = cond.m_nRangeMax;
	m_nValueIndex = cond.m_nValueIndex;
	return *this;
}

SSpInRange::~SSpInRange()
{
}

void SSpInRange::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring())
		ar << m_nRangeMin << m_nRangeMax << m_nValueIndex;
	else
		ar >> m_nRangeMin >> m_nRangeMax >> m_nValueIndex;
}

void SSpInRange::printASCII(sspOutStream& outStr)
{
	outStr << endl << m_nIndex << ": SSpInRange " << m_strName;
	outStr << endl << "   - m_nRangeMin: " << m_nRangeMin << ", m_nRangeMax: " << m_nRangeMax;
	outStr << endl << "   - m_nValueIndex: " << sspPool::Instance().values.GetName(m_nValueIndex);
}

bool SSpInRange::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	if (m_nValueIndex < 0 || m_nValueIndex >= (int) sspPool::Instance().values.GetSize()) {
		printError(outStr, _T("(SSpInRange): m_nValueIndex is not valid"), nErrors);
		bRet = false;
	}
	if (m_nRangeMin > m_nRangeMax) {
		printError(outStr, _T("(SSpInRange): m_nRangeMin is larger than m_nRangeMax"), nErrors);
		bRet = false;
	}
	if (m_nRangeMin == m_nRangeMax) {
		printWarning(outStr, _T("(SSpInRange): m_nRangeMin is larger than m_nRangeMax"), nWarnings);
		bRet = false;
	}
	return bRet;
}

bool SSpInRange::isTrue()
{
	float fRange1 = sspPool::Instance().values.getValue(m_nRangeMin)->getValue();
	float fRange2 = sspPool::Instance().values.getValue(m_nRangeMax)->getValue();
	float fTestVal = sspPool::Instance().values.getValue(m_nValueIndex)->getValue();
	return ((fTestVal >= fRange1 && fTestVal <= fRange2) ||
		(fTestVal <= fRange1 && fTestVal >= fRange2));
}

//////////////////////////////////////////////////////////////////////
// SSpTimeInterval
//////////////////////////////////////////////////////////////////////

SSpTimeInterval::SSpTimeInterval() : sspConditional(SSP_COND_TIME_INTERVAL)
{
}

SSpTimeInterval::SSpTimeInterval(const SSpTimeInterval& cond) : sspConditional(cond)
{
	m_clockStart = cond.m_clockStart;
	m_clockEnd = cond.m_clockEnd;
}

SSpTimeInterval& SSpTimeInterval::operator= (const SSpTimeInterval& cond)
{
	sspConditional::operator=(cond);
	m_clockStart = cond.m_clockStart;
	m_clockEnd = cond.m_clockEnd;
	return *this;
}

SSpTimeInterval::~SSpTimeInterval()
{
}

void SSpTimeInterval::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	m_clockStart.Serialize(ar);
	m_clockEnd.Serialize(ar);
}

void SSpTimeInterval::printASCII(sspOutStream& outStr)
{
	outStr << endl << m_nIndex << ": SSpTimeInterval " << m_strName;
	outStr << endl << "   - m_clockStart: " << m_clockStart.hour() << ":" << m_clockStart.min() << ":" << m_clockStart.sec();
	outStr << endl << "   - m_clockEnd: " << m_clockEnd.hour() << ":" << m_clockEnd.min() << ":" << m_clockEnd.sec();
}

bool SSpTimeInterval::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	if (!m_clockStart.isValid()) {
		printError(outStr, _T("(SSpTimeInterval): m_clockStart is not valid"), nErrors);
		bRet = false;
	}
	if (!m_clockEnd.isValid()) {
		printError(outStr, _T("(SSpTimeInterval): m_clockEnd is not valid"), nErrors);
		bRet = false;
	}
	if (m_clockStart == m_clockEnd) {
		printWarning(outStr, _T("(SSpTimeInterval): m_clockStart is equal to m_clockEnd"), nWarnings);
		bRet = false;
	}
	return bRet;
}

bool SSpTimeInterval::isTrue()
{
  sspClock now = (sspClock) sspDateTimeManager::Instance().current();
	return ((m_clockStart <= now) && (now <= m_clockEnd)
		|| (m_clockStart >= now) && (now >= m_clockEnd));
}

//////////////////////////////////////////////////////////////////////
// SSpTimeRelation
//////////////////////////////////////////////////////////////////////

SSpTimeRelation::SSpTimeRelation() : sspConditional(SSP_COND_TIME_RELATION)
{
	m_nRelation = AFTER;
}


SSpTimeRelation::SSpTimeRelation(const SSpTimeRelation& cond) : sspConditional(cond)
{
	m_clock = cond.m_clock;
	m_nRelation = cond.m_nRelation;
}

SSpTimeRelation& SSpTimeRelation::operator= (const SSpTimeRelation& cond)
{
	sspConditional::operator=(cond);
	m_clock = cond.m_clock;
	m_nRelation = cond.m_nRelation;
	return *this;
}

SSpTimeRelation::~SSpTimeRelation()
{
}

void SSpTimeRelation::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	m_clock.Serialize(ar);
	if (ar.IsStoring())
		ar << m_nRelation;
	else
		ar >> m_nRelation;
}

void SSpTimeRelation::printASCII(sspOutStream& outStr)
{
	outStr << endl << m_nIndex << ": SSpTimeRelation " << m_strName;
	outStr << endl << "   - m_clock: " << m_clock.hour() << ":" << m_clock.min() << ":" << m_clock.sec();
	switch (m_nRelation) {
	case BEFORE:
		outStr << endl << "   - m_nRelation: before";
		break;
	case AFTER:
		outStr << endl << "   - m_nRelation: after";
		break;
	default:
		outStr << endl << "   - m_nRelation: unknown";
		break;
	}
}

bool SSpTimeRelation::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	if (!m_clock.isValid()) {
		printError(outStr, _T("(SSpTimeRelation): m_clock is not valid"), nErrors);
		bRet = false;
	}
	if (m_nRelation != BEFORE && m_nRelation != AFTER) {
		printError(outStr, _T("(SSpTimeRelation): m_nRelation is not valid"), nErrors);
		bRet = false;
	}
	return bRet;
}

inline bool SSpTimeRelation::isTrue()
{
	switch (m_nRelation) {
	case BEFORE:
		return (sspClock) sspDateTimeManager::Instance().current() < m_clock;
	case AFTER:
		return (sspClock) sspDateTimeManager::Instance().current() >= m_clock;
	default:
		return false;
	}
}

//////////////////////////////////////////////////////////////////////
// SSpMinuteRelation
//////////////////////////////////////////////////////////////////////

SSpMinuteRelation::SSpMinuteRelation() : sspConditional(SSP_COND_MINUTE_RELATION)
{
	m_nMinute = 0;
	m_nRelation = AFTER;
}

SSpMinuteRelation::~SSpMinuteRelation()
{
}

SSpMinuteRelation::SSpMinuteRelation(const SSpMinuteRelation& cond) : sspConditional(cond)
{
	m_nMinute = cond.m_nMinute;
	m_nRelation = cond.m_nRelation;
}

SSpMinuteRelation& SSpMinuteRelation::operator= (const SSpMinuteRelation& cond)
{
	sspConditional::operator=(cond);
	m_nMinute = cond.m_nMinute;
	m_nRelation = cond.m_nRelation;
	return *this;
}

void SSpMinuteRelation::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring())
		ar << m_nMinute << m_nRelation;
	else
		ar >> m_nMinute >> m_nRelation;
}

void SSpMinuteRelation::printASCII(sspOutStream& outStr)
{
	outStr << endl << m_nIndex << ": SSpMinuteRelation " << m_strName;
	outStr << endl << "   - m_nMinute: " << m_nMinute;
	switch (m_nRelation) {
	case BEFORE:
		outStr << ", m_nRelation: before";
		break;
	case EQUAL:
		outStr << ", m_nRelation: equal";
		break;
	case AFTER:
		outStr << ", m_nRelation: after";
		break;
	default:
		outStr << ", m_nRelation: unknown";
		break;
	}
}

bool SSpMinuteRelation::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	if (m_nMinute < 0 || m_nMinute >= 60) {
		printError(outStr, _T("(SSpMinuteRelation): m_nMinute is not valid"), nErrors);
		bRet = false;
	}
	if (m_nRelation != BEFORE && m_nRelation != AFTER && m_nRelation != EQUAL) {
		printError(outStr, _T("(SSpMinuteRelation): m_nRelation is not valid"), nErrors);
		bRet = false;
	}
	return bRet;
}

inline bool SSpMinuteRelation::isTrue()
{
	switch (m_nRelation) {
	case EQUAL:
		return (sspDateTimeManager::Instance().current().min() == m_nMinute);
	case BEFORE:
		return (sspDateTimeManager::Instance().current().min() < m_nMinute);
	case AFTER:
		return (sspDateTimeManager::Instance().current().min() >= m_nMinute);
	default:
		return false;
	}
}

//////////////////////////////////////////////////////////////////////
// SSpDateInterval
//////////////////////////////////////////////////////////////////////

SSpDateInterval::SSpDateInterval() : sspConditional(SSP_COND_DATE_INTERVAL)
{
}

SSpDateInterval::SSpDateInterval(const SSpDateInterval& cond) : sspConditional(cond)
{
	m_dateStart = cond.m_dateStart;
	m_dateEnd = cond.m_dateEnd;
}

SSpDateInterval& SSpDateInterval::operator= (const SSpDateInterval& cond)
{
	sspConditional::operator=(cond);
	m_dateStart = cond.m_dateStart;
	m_dateEnd = cond.m_dateEnd;
	return *this;
}

SSpDateInterval::~SSpDateInterval()
{
}

void SSpDateInterval::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	m_dateStart.Serialize(ar);
	m_dateEnd.Serialize(ar);
}

void SSpDateInterval::printASCII(sspOutStream& outStr)
{
	outStr << endl << m_nIndex << ": SSpDateInterval " << m_strName;
	outStr << endl << "   - m_dateStart: " << m_dateStart.month() << "//" << m_dateStart.day();
	outStr << endl << "   - m_dateEnd: " << m_dateEnd.month() << "//" << m_dateEnd.day();
}

bool SSpDateInterval::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	if (!m_dateStart.isValid()) {
		printError(outStr, _T("(SSpDateInterval): m_dateStart is not valid"), nErrors);
		bRet = false;
	}
	if (!m_dateEnd.isValid()) {
		printError(outStr, _T("(SSpDateInterval): m_dateEnd is not valid"), nErrors);
		bRet = false;
	}
  if (m_dateStart == m_dateEnd) {
		printWarning(outStr, _T("(SSpDateInterval): m_dateStart is equal to m_dateEnd"), nWarnings);
		bRet = false;
	}
	return bRet;
}

inline bool SSpDateInterval::isTrue()
{
  sspDate now = (sspDate) sspDateTimeManager::Instance().current();
	return ((m_dateStart <= now) && (now <= m_dateEnd)
		|| (m_dateStart >= now) && (now >= m_dateEnd));	// Spanning over new year
}

//////////////////////////////////////////////////////////////////////
// SSpDayOfWeek
//////////////////////////////////////////////////////////////////////

SSpDayOfWeek::SSpDayOfWeek() : sspConditional(SSP_COND_DAYOFWEEK)
{
  for (int i=0; i < sspLocalTime::WEEKDAYS; ++i)
		m_bDayCheck[i] = false;
}

SSpDayOfWeek::SSpDayOfWeek(const SSpDayOfWeek& cond) : sspConditional(cond)
{
	for (int i=0; i < sspLocalTime::WEEKDAYS; ++i)
		m_bDayCheck[i] = cond.m_bDayCheck[i];
}

SSpDayOfWeek& SSpDayOfWeek::operator= (const SSpDayOfWeek& cond)
{
	sspConditional::operator=(cond);
	for (int i=0; i < sspLocalTime::WEEKDAYS; ++i)
		m_bDayCheck[i] = cond.m_bDayCheck[i];
	return *this;
}

SSpDayOfWeek::~SSpDayOfWeek()
{
}

void SSpDayOfWeek::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring()) {
		for (int i=0; i < sspLocalTime::WEEKDAYS; ++i)
			ar << m_bDayCheck[i];
	}
	else {
		for (int i=0; i < sspLocalTime::WEEKDAYS; ++i)
			ar >> m_bDayCheck[i];
	}
}

void SSpDayOfWeek::printASCII(sspOutStream& outStr)
{
	outStr << endl << m_nIndex << ": SSpDayOfWeek " << m_strName;
	for (int i=0; i < sspLocalTime::WEEKDAYS; ++i)
		if (m_bDayCheck[i])	outStr << endl << "   - " << sspLocalTime::s_dayNames[i];
}

bool SSpDayOfWeek::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = false;
	for (int i=0; i < sspLocalTime::WEEKDAYS; ++i)
		if (m_bDayCheck[i]) bRet = true;
		if (!bRet) printError(outStr, _T("(SSpDayOfWeek): no days are selected"), nErrors);
	return bRet;
}

inline bool SSpDayOfWeek::isTrue()
{
	return m_bDayCheck[sspDateTimeManager::Instance().current().dayOfWeek()];
}

//////////////////////////////////////////////////////////////////////
// SSpIsPlaying
//////////////////////////////////////////////////////////////////////

SSpIsPlaying::SSpIsPlaying() : sspConditional(SSP_COND_PLAYING)
{
}

SSpIsPlaying::SSpIsPlaying(const SSpIsPlaying& cond) : sspConditional(cond)
{
	m_nObjects = cond.m_nObjects;
}

SSpIsPlaying& SSpIsPlaying::operator= (const SSpIsPlaying& cond)
{
	sspConditional::operator=(cond);
	m_nObjects = cond.m_nObjects;
	return *this;
}

SSpIsPlaying::~SSpIsPlaying()
{
}

void SSpIsPlaying::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring()) {
		ar << m_nObjects.size();
		for (unsigned int i=0; i<m_nObjects.size(); ++i) {
			ar << m_nObjects[i];
		}
	}
	else {
		unsigned int nBuf;
		ar >> nBuf;
		m_nObjects.resize(nBuf);
		for (unsigned int i=0; i<nBuf; i++)
			ar >> m_nObjects[i];
	}
}

void SSpIsPlaying::printASCII(sspOutStream& outStr)
{
	outStr << endl << m_nIndex << ": SSpIsPlaying " << m_strName;
	for (unsigned int i=0; i<m_nObjects.size(); ++i) {
		outStr << endl << "   - play object " << i << ": " << sspPool::Instance().objects.GetName(m_nObjects[i]);
	}
}

bool SSpIsPlaying::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	for (unsigned int i=0; i<m_nObjects.size(); ++i) {
		if (m_nObjects[i] < 0 || m_nObjects[i] >= (int) sspPool::Instance().objects.GetSize()) {
			printError(outStr, _T("(SSpIsPlaying): a play object index is not valid"), nErrors);
			bRet = false;
		}
	}
	if (m_nObjects.size() == 0) {
		printError(outStr, _T("(SSpIsPlaying): there are no play objects"), nErrors);
		bRet = false;
	}
	return bRet;
}

bool SSpIsPlaying::isTrue()
{
	for (unsigned int i=0; i<m_nObjects.size(); ++i) {
		if (sspPool::Instance().objects.GetPlayObject(m_nObjects[i])->isPlaying()) 
			return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
// SSpAnd
//////////////////////////////////////////////////////////////////////

SSpAnd::SSpAnd() : sspConditional(SSP_COND_AND)
{
}

SSpAnd::SSpAnd(const SSpAnd& cond) : sspConditional(cond)
{
	m_nConditionals = cond.m_nConditionals;
}

SSpAnd& SSpAnd::operator= (const SSpAnd& cond)
{
	sspConditional::operator=(cond);
	m_nConditionals = cond.m_nConditionals;
	return *this;
}

SSpAnd::~SSpAnd()
{
}

void SSpAnd::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring()) {
		ar << m_nConditionals.size();
		for (unsigned int i=0; i<m_nConditionals.size(); ++i) {
			ar << m_nConditionals[i];
		}
	}
	else {
		unsigned int nBuf;
		ar >> nBuf;
		m_nConditionals.resize(nBuf);
		for (unsigned int i=0; i<nBuf; i++)
			ar >> m_nConditionals[i];
	}
}

void SSpAnd::printASCII(sspOutStream& outStr)
{
	outStr << endl << m_nIndex << ": SSpAnd " << m_strName;
	for (unsigned int i=0; i<m_nConditionals.size(); ++i) {
		outStr << endl << "   - conditional " << i << ": " << sspPool::Instance().conditionals.GetName(m_nConditionals[i]);
	}
}

bool SSpAnd::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	for (unsigned int i=0; i<m_nConditionals.size(); ++i) {
		if (m_nConditionals[i] == m_nIndex || m_nConditionals[i] < 0 
			|| m_nConditionals[i] >= (int) sspPool::Instance().conditionals.GetSize()) {
			printError(outStr, _T("(SSpAnd): a conditional index is not valid"), nErrors);
			bRet = false;
		}
	}
	if (m_nConditionals.size() == 0) {
		printError(outStr, _T("(SSpAnd): there are no conditionals"), nErrors);
		bRet = false;
	}
	if (m_nConditionals.size() == 1) {
		printWarning(outStr, _T("(SSpAnd): there is only a single conditional"), nWarnings);
		bRet = false;
	}
	return bRet;
}

bool SSpAnd::isTrue()
{
	for (unsigned int i=0; i<m_nConditionals.size(); ++i) {
		if (!sspPool::Instance().conditionals.GetConditional(m_nConditionals[i])->isTrue()) 
			return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
// SSpOr
//////////////////////////////////////////////////////////////////////

SSpOr::SSpOr() : sspConditional(SSP_COND_OR)
{
}

SSpOr::SSpOr(const SSpOr& cond) : sspConditional(cond)
{
	m_nConditionals = cond.m_nConditionals;
}

SSpOr& SSpOr::operator= (const SSpOr& cond)
{
	sspConditional::operator=(cond);
	m_nConditionals = cond.m_nConditionals;
	return *this;
}

SSpOr::~SSpOr()
{
}

void SSpOr::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring()) {
		ar << m_nConditionals.size();
		for (unsigned int i=0; i<m_nConditionals.size(); ++i) {
			ar << m_nConditionals[i];
		}
	}
	else {
		int nBuf;
		ar >> nBuf;
		m_nConditionals.resize(nBuf);
		for (int i=0; i<nBuf; i++)
			ar >> m_nConditionals[i];
	}
}

void SSpOr::printASCII(sspOutStream& outStr)
{
	outStr << endl << m_nIndex << ": SSpOr " << m_strName;
	for (unsigned int i=0; i<m_nConditionals.size(); ++i) {
		outStr << endl << "   - conditional " << i << ": " << sspPool::Instance().conditionals.GetName(m_nConditionals[i]);
	}
}

bool SSpOr::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	for (unsigned int i=0; i<m_nConditionals.size(); ++i) {
		if (m_nConditionals[i] == m_nIndex || m_nConditionals[i] < 0 
			|| m_nConditionals[i] >= (int) sspPool::Instance().conditionals.GetSize()) {
			printError(outStr, _T("(SSpOr): a conditional index is not valid"), nErrors);
			bRet = false;
		}
	}
	if (m_nConditionals.size() == 0) {
		printError(outStr, _T("(SSpOr): there are no conditionals"), nErrors);
		bRet = false;
	}
	if (m_nConditionals.size() == 1) {
		printWarning(outStr, _T("(SSpOr): there is only a single conditional"), nWarnings);
		bRet = false;
	}
	return bRet;
}

bool SSpOr::isTrue()
{
	for (unsigned int i=0; i<m_nConditionals.size(); ++i) {
		if (sspPool::Instance().conditionals.GetConditional(m_nConditionals[i])->isTrue()) 
			return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
// SSpNot
//////////////////////////////////////////////////////////////////////

SSpNot::SSpNot() : sspConditional(SSP_COND_NOT)
{
	m_nCondIndex = -1;
}

SSpNot::SSpNot(const SSpNot& cond) : sspConditional(cond)
{
	m_nCondIndex = cond.m_nCondIndex;
}

SSpNot& SSpNot::operator= (const SSpNot& cond)
{
	sspConditional::operator=(cond);
	m_nCondIndex = cond.m_nCondIndex;
	return *this;
}

SSpNot::~SSpNot()
{
}

void SSpNot::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring())
		ar << m_nCondIndex;
	else
		ar >> m_nCondIndex;
}

void SSpNot::printASCII(sspOutStream& outStr)
{
	outStr << endl << m_nIndex << ": SSpNot " << m_strName;
	outStr << endl << "   - m_nCondIndex: " << sspPool::Instance().conditionals.GetName(m_nCondIndex);
}

bool SSpNot::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	if (m_nCondIndex == m_nIndex || m_nCondIndex < 0 
		|| m_nCondIndex >= (int) sspPool::Instance().conditionals.GetSize()) {
		printError(outStr, _T("(SSpNot): m_nCondIndex is not valid"), nErrors);
		bRet = false;
	}
	return bRet;
}

bool SSpNot::isTrue()
{
	return !(sspPool::Instance().conditionals.GetConditional(m_nCondIndex)->isTrue());
}

//////////////////////////////////////////////////////////////////////
// SSpTrigger
//////////////////////////////////////////////////////////////////////

SSpTrigger::SSpTrigger() : sspConditional(SSP_COND_TRIGGER)
{
  m_nMode = SSP_TRIG_TRUE;
	m_nCondIndex = 0;
	m_bPrevious = false;
}

SSpTrigger::SSpTrigger(const SSpTrigger& trigger) : sspConditional(trigger)
{
  m_nMode = trigger.m_nMode;
	m_nCondIndex = trigger.m_nCondIndex;
	m_bPrevious = false;
}

SSpTrigger& SSpTrigger::operator= (const SSpTrigger& trigger)
{
	sspConditional::operator=(trigger);
  m_nMode = trigger.m_nMode;
	m_nCondIndex = trigger.m_nCondIndex;
	m_bPrevious = false;
	return *this;
}

SSpTrigger::~SSpTrigger()
{
}

void SSpTrigger::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring())
		ar << m_nCondIndex << m_nMode;
	else
		ar >> m_nCondIndex >> m_nMode;
}

void SSpTrigger::printASCII(sspOutStream& outStr)
{
	outStr << endl << "  - trigger " << sspPool::Instance().conditionals.GetName(m_nCondIndex);
  switch (m_nMode) {
    case SSP_TRIG_TRUE:
    	outStr << ", trig on true only";
      break;
    case SSP_TRIG_FALSE:
    	outStr << ", trig on false only";
      break;
    case SSP_TRIG_BOTH:
    	outStr << ", trig on true and false";
      break;
  }
}

bool SSpTrigger::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	if (m_nCondIndex < 0 || m_nCondIndex > (int) sspPool::Instance().conditionals.GetSize()) {
		printError(outStr, _T("(SSpTrigger): m_nCondIndex is not valid"), nErrors);
		bRet = false;
	}
	return bRet;
}

bool SSpTrigger::isTrue()
{
	bool bRtn;
	bool bNew = sspPool::Instance().conditionals.GetConditional(m_nCondIndex)->isTrue();
  switch (m_nMode) {
    case SSP_TRIG_TRUE:
      bRtn = bNew & !m_bPrevious;
      break;
    case SSP_TRIG_FALSE:
      bRtn = !bNew & m_bPrevious;
      break;
    case SSP_TRIG_BOTH:
    default:
      bRtn = bNew ^ m_bPrevious;
      break;
  }
	m_bPrevious = bNew;
	return bRtn;
}

void SSpTrigger::reset()
{
	m_bPrevious = sspPool::Instance().conditionals.GetConditional(m_nCondIndex)->isTrue();
}

