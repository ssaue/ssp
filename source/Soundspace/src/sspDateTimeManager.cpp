//////////////////////////////////////////////////////////////////////
// sspDateTimeManager
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "sspDateTimeManager.h"
#include "sspSetting.h"
#include <stdexcept>

using namespace std;

sspDateTimeManager* sspDateTimeManager::m_pInstance = NULL;
bool sspDateTimeManager::m_bDestroyed = false;

sspDateTimeManager::sspDateTimeManager()
: m_startTime(8, 0, 0)
, m_endTime(20, 0, 0)
, m_bInterval(false)
, m_update(sspTimeConvert::secs2time(sspSetting::update.clock))
{
	m_playInterval.setStartClock(m_startTime);
	m_playInterval.setEndClock(m_endTime);
	updateTime();
}

sspDateTimeManager::sspDateTimeManager(const sspDateTimeManager&)
{
}

sspDateTimeManager& sspDateTimeManager::operator=(const sspDateTimeManager&)
{
	return *this;
}

sspDateTimeManager::~sspDateTimeManager()
{
	m_pInstance = NULL;
	m_bDestroyed = true;
}

sspDateTimeManager& sspDateTimeManager::Instance()
{
	if (!m_pInstance) {
		if (m_bDestroyed) {
			onDeadReference();
		}
		else {
			create();
		}
	}
	return *m_pInstance;
}

void sspDateTimeManager::Serialize(CArchive& ar)
{
	m_startTime.Serialize(ar);
	m_endTime.Serialize(ar);
  if (ar.IsStoring()) {
		ar << m_bInterval;
  }
	else {
		ar >> m_bInterval;
  	m_playInterval.setStartClock(m_startTime);
	  m_playInterval.setEndClock(m_endTime);
	}
}

void sspDateTimeManager::printASCII(sspOutStream& outStr)
{
	outStr << endl << "sspDateTimeManager";
  outStr << endl << m_bInterval ? "   - using play interval" : "   - not using play interval";
	outStr << endl << "   - m_startTime: " << m_startTime.hour() << ":" << m_startTime.min() << ":" << m_startTime.sec();
	outStr << endl << "   - m_endTime: " << m_endTime.hour() << ":" << m_endTime.min() << ":" << m_endTime.sec();
}

bool sspDateTimeManager::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	if (m_bInterval && !m_startTime.isValid()) {
		printError(outStr, _T("(sspDateTimeManager): m_startTime is not valid"), nErrors);
		bRet = false;
	}
	if (m_bInterval && !m_endTime.isValid()) {
		printError(outStr, _T("(sspDateTimeManager): m_endTime is not valid"), nErrors);
		bRet = false;
	}
	return bRet;
}

bool sspDateTimeManager::start()
{
  m_bUpdated = true;
  m_update.setInterval(sspTimeConvert::secs2time(sspSetting::update.clock));
  m_update.initialize();
	return updateTime();
}

bool sspDateTimeManager::update()
{
  m_bUpdated = m_update.update();
  if (m_bUpdated) updateTime();
  return m_bUpdated;
}

bool sspDateTimeManager::updateTime()
{
  m_current.getCurrentTime();
  m_bIsInside = !m_bInterval || m_playInterval.isTrue();
  return m_bIsInside;
}

void sspDateTimeManager::create()
{
	static sspDateTimeManager theInstance;
	m_pInstance = &theInstance;
}

void sspDateTimeManager::onDeadReference()
{
	throw std::runtime_error("Dead reference to current time detected");
}
