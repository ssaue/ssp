//////////////////////////////////////////
// sspExecutiveManager.h

#include "stdafx.h"
#include "sspExecutiveManager.h"
#include "sspSetting.h"
#include "sspDeviceManager.h"
#include "sspDateTimeManager.h"
#include "sspInputManager.h"
#include "sspResetManager.h"
#include <sspTimer.h>

#include <algorithm>
using namespace std;

sspExecutiveManager* sspExecutiveManager::m_pInstance = NULL;
bool sspExecutiveManager::m_bDestroyed = false;

sspExecutiveManager::sspExecutiveManager(void)
: m_pTimer(NULL)
, m_nStopProc(SSP_EMPTY_STOP)
, m_nStartProc(SSP_START_NOTHING)
{
	m_bRunning = false;
}

sspExecutiveManager::sspExecutiveManager(const sspExecutiveManager&)
{
}

sspExecutiveManager& sspExecutiveManager::operator=(const sspExecutiveManager&)
{
	return *this;
}


sspExecutiveManager::~sspExecutiveManager(void)
{
	terminate();
	m_pInstance = NULL;
	m_bDestroyed = true;
}

sspExecutiveManager& sspExecutiveManager::Instance()
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

void sspExecutiveManager::Serialize(CArchive& ar)
{
  if (ar.IsStoring()) {
		ar << m_nStartProc << m_nStopProc;
  }
	else {
    int nD1, nD2;
		ar >> nD1 >> nD2;
    m_nStartProc = (tStartupProc) nD1;
    m_nStopProc = (tShutdownProc) nD2;
	}
}

void sspExecutiveManager::printASCII(sspOutStream& outStr)
{
	outStr << endl << "sspExecutiveManager";
	outStr << endl << "   - m_nStartProc: " << m_nStartProc << ", m_nStopProc: " << m_nStopProc;
}

bool sspExecutiveManager::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;
  // Nothing to verify so far
	return bRet;
}

void sspExecutiveManager::clearContents()
{
}

bool sspExecutiveManager::initialize(LPVOID hWnd)
{
  delete m_pTimer;
  m_pTimer = new sspPeriodicTimer(sspSetting::res.event);
  return m_pTimer->isInitialized();
}

bool sspExecutiveManager::start()
{
  srand( (unsigned)time( NULL ) );
  return m_pTimer && m_pTimer->run(this);
}

bool sspExecutiveManager::stop()
{
  if (m_pTimer) m_pTimer->stop();
  return true;
}

bool sspExecutiveManager::terminate()
{
  stop();
  delete m_pTimer;
  m_pTimer = NULL;
  return true;
}

bool sspExecutiveManager::begin(sspTimerType& nMSecs)
{
  // Make sure that the execution loop has a necessary short update interval
  float fUpdate = min(sspSetting::update.event, sspSetting::update.clock);
  fUpdate = min(fUpdate, sspInputManager::Instance().getMinUpdateInterval());

  nMSecs = sspTimeConvert::secs2timer(fUpdate);
  if (sspDateTimeManager::Instance().start()) {
		sspPlayManager::Instance().start();
    sspPlayManager::Instance().update();
  }
  sspResetManager::Instance().begin();  // Check return value ??
  m_bRunning = true;
  return true;
}

bool sspExecutiveManager::update(sspTimerType& nMSecs)
{
  bool bTimeChange = sspDateTimeManager::Instance().update();
  bool bStateChange = sspInputManager::Instance().update() || bTimeChange;

#ifdef NDEBUG

  //Verify that everything is playing as expected - if not exit the application
  if (!sspPlayManager::Instance().verifyRunning() || (bStateChange && !sspResetManager::Instance().update())) {
    m_nStopProc = SSP_EMPTY_EXIT;
    stop();
    return false;
  }
#endif

	if (bTimeChange) {
		if (sspDateTimeManager::Instance().isInsidePlayInterval()) {
			sspPlayManager::Instance().start();   // Does nothing if already playing
		}
    else {
			sspPlayManager::Instance().stop();    // Does nothing if not playing
		}
	}
  if (!sspPlayManager::Instance().update() && m_nStopProc != SSP_EMPTY_RUN)
    return false;
  else
    return true;
}

void sspExecutiveManager::end()
{
  sspPlayManager::Instance().stop();
  sspResetManager::Instance().end();
  m_bRunning = false;
}

void sspExecutiveManager::create()
{
	static sspExecutiveManager theInstance;
	m_pInstance = &theInstance;
}

void sspExecutiveManager::onDeadReference()
{
	throw std::runtime_error("Dead reference to executive manager detected");
}
