//////////////////////////////////////////
// sspResetManager.h

#include "stdafx.h"
#include "sspResetManager.h"
#include "sspDateTimeManager.h"
#include "sspwatchdog.h"
#include "sspWatchdogDecision.h"
#include "sspSetting.h"
#include "sspPools.h"
#include "SSpBasic.h"

using namespace std;

/***********************************************
** Static factory method for creating watchdogs
************************************************/

sspWatchdog* sspWatchdog::create(int nType)
{
  switch (nType) {
    case SSP_WATCHDOG_BBISA: 
      return new sspWatchdogBB_ISA;
      break;
    case SSP_WATCHDOG_BBPCI:
      return new sspWatchdogBB_PCI;
      break;
    case SSP_WATCHDOG_DECISION:
      return new sspWatchdogDecision;
      break;      
    case SSP_WATCHDOG_IB780: 
      return new sspWatchdogIB780;
      break;
    default:
      return NULL;
      break;
  }
}


sspResetManager* sspResetManager::m_pInstance = NULL;
bool sspResetManager::m_bDestroyed = false;

sspResetManager::sspResetManager(void)
: m_bWatchdog(false)
, m_nWatchdog(-1)
, m_nTimeOut(5)
, m_nUnit(TUNIT_HOURS)
, m_nInterval(0)
, m_nCondition(0)
, m_pDog(NULL)
, m_bTimeReset(false)
, m_timeOfDay(6, 0, 0)
{
}

sspResetManager::sspResetManager(const sspResetManager&)
{
}

sspResetManager& sspResetManager::operator=(const sspResetManager&)
{
	return *this;
}


sspResetManager::~sspResetManager(void)
{
	m_pInstance = NULL;
	m_bDestroyed = true;
  delete m_pDog;
}

sspResetManager& sspResetManager::Instance()
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

void sspResetManager::Serialize(CArchive& ar)
{
	m_timeOfDay.Serialize(ar);
  if (ar.IsStoring()) {
		ar << m_bWatchdog << m_nWatchdog << m_nTimeOut;
		ar << m_nUnit << m_nInterval << m_nCondition;
  }
	else {
    int nUnit;
		ar >> m_bWatchdog >> m_nWatchdog >> m_nTimeOut;
		ar >> nUnit >> m_nInterval >> m_nCondition;
    m_nUnit = (tTimeUnit) nUnit;
	}
}

void sspResetManager::printASCII(sspOutStream& outStr)
{
	outStr << endl << "sspResetManager";
  outStr << endl << m_bWatchdog ? "   - enabled" : "   - disabled";
	outStr << endl << "   - m_nWatchdog: " << m_nWatchdog << ", m_nTimeOut: " << m_nTimeOut;
  outStr << endl << "   - reset (timeunit: " << m_nUnit << "), m_nInterval: " << m_nInterval;
	outStr << endl << "   - m_timeOfDay: " << m_timeOfDay.hour() << ":" << m_timeOfDay.min() << ":" << m_timeOfDay.sec();
  outStr << endl << "   - reset condition: " << sspPool::Instance().conditionals.GetName(m_nCondition);
}

bool sspResetManager::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	if (m_bWatchdog && m_nTimeOut <= 1) {
		printError(outStr, _T("(sspResetManager): m_nTimeOut is too small"), nErrors);
		bRet = false;
	}
	if (!m_timeOfDay.isValid()) {
		printError(outStr, _T("(sspResetManager): m_timeOfDay is not valid"), nErrors);
		bRet = false;
	}
	if (m_nCondition < 0 || m_nCondition > (int) sspPool::Instance().conditionals.GetSize()) {
		printError(outStr, _T("(sspResetManager): reset condition index is not valid"), nErrors);
		bRet = false;
	}
	return bRet;
}

void sspResetManager::clearContents()
{
  m_bWatchdog = false;
  m_nWatchdog = -1;
  m_nTimeOut = 5;
  m_nUnit = TUNIT_HOURS;
  m_nInterval = 0;
  m_nCondition = 0;
  delete m_pDog;
  m_pDog = NULL;
  m_bTimeReset = false;
  m_timeOfDay.setHour(7);
  m_timeOfDay.setMinute(0);
  m_timeOfDay.setSecond(0);
}

bool sspResetManager::initialize()
{
  m_bTimeReset = (m_nInterval > 0);

  if (!m_bWatchdog) return true;    // No watchdog selected - nothing more to initialize

  m_pDog = sspWatchdog::create(m_nWatchdog);
  m_bWatchdog = m_pDog != NULL ? m_pDog->initialize(m_nTimeOut) : false;

  if (!m_bWatchdog) return false;   // The watchdog failed initialization

  return true;
}

bool sspResetManager::begin()
{
  if (m_bTimeReset) {
    m_resetTime.getCurrentTime();
    m_resetTime += (m_nUnit == TUNIT_DAYS) 
      ? sspTimeConvert::days2time(m_nInterval) 
      : sspTimeConvert::hours2time(m_nInterval);
  }

  if (m_bWatchdog && !m_pDog->enable()) 
    return false;   // The watchdog failed to enable

  return true;
}

bool sspResetManager::update()
{
  // Check reset conditions
  bool bResetCondition = sspPool::Instance().conditionals.GetConditional(m_nCondition)->isTrue();
  if (!bResetCondition && m_bTimeReset) {
    const sspLocalTime& now = sspDateTimeManager::Instance().current();
    bResetCondition = (m_nUnit == TUNIT_DAYS) 
      ? ((sspTime) now > m_resetTime && abs(m_timeOfDay - (sspClock)now) < TIMEOFDAY_ROUNDOFF)
      : ((sspTime) now > m_resetTime);
  }

  if (bResetCondition) {
    if (softReboot()) {
      end();
    }
    else if (m_bWatchdog) {// Soft reset failed, try a hard reset using the watchdog card instead
      m_bWatchdog = false;
    }
    return false;
  }
  if (m_bWatchdog) m_pDog->refresh();
  return true;
}

bool sspResetManager::end()
{
  if (!m_bWatchdog)
    return true;    // No watchdog - always succeed
  else
    return m_pDog->disable();
}

bool sspResetManager::terminate()
{
  if (!m_bWatchdog)
    return true;    // No watchdog - always succeed
  else if (m_pDog)
    return m_pDog->terminate();
  else
    return false;
}

void sspResetManager::create()
{
	static sspResetManager theInstance;
	m_pInstance = &theInstance;
}

void sspResetManager::onDeadReference()
{
	throw std::runtime_error("Dead reference to reset manager detected");
}

bool sspResetManager::softReboot()
{
  bool bSuccess = false;
  HANDLE hToken; 
  TOKEN_PRIVILEGES tkp; 
    
  if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
  
    LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);     
    tkp.PrivilegeCount = 1;  // one privilege to set    
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0); 
    
    if (GetLastError() == ERROR_SUCCESS) {
      bSuccess = ExitWindowsEx(EWX_REBOOT | EWX_FORCE, SHTDN_REASON_MAJOR_APPLICATION) == TRUE;
    }
  }
  return bSuccess;
}
