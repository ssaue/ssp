//////////////////////////////////////////
// sspTimeResources.cpp

#include "sspTimeResources.h"

#ifdef WIN32
  #include <windows.h>
  #undef min  // The min and max macro cause trouble for STL
  #undef max
#else
//  #include <pthread.h>  MUST BE DECIDED LATER!
#endif

#include <iostream>
#include <limits>
#include <algorithm>

using namespace std;

sspTimeResources* sspTimeResources::m_pInstance = NULL;
bool sspTimeResources::m_bDestroyed = false;

sspTimeResources::sspTimeResources(void)
{
  m_bResolution = false;
  m_nMinResolution = 1;
  m_nMaxResolution = numeric_limits<sspTimerType>::max();
  #if !defined (WIN32)
    for (int i=SIGRTMIN; i<=SIGRTMAX; ++i)
      m_signals.push_back(i);    
  #endif
}

sspTimeResources::sspTimeResources(const sspTimeResources&)
{
}

sspTimeResources& sspTimeResources::operator=(const sspTimeResources&)
{
	return *this;
}


sspTimeResources::~sspTimeResources(void)
{
	terminate();
	m_pInstance = NULL;
	m_bDestroyed = true;
}

bool sspTimeResources::initialize()
{
  if (m_bResolution)
    return true;

  #if defined (WIN32)
	  TIMECAPS timeCaps;
    m_bResolution = (timeGetDevCaps(&timeCaps, sizeof(TIMECAPS)) == TIMERR_NOERROR);
    if (!m_bResolution)
      return false;
    m_nMinResolution = std::max(timeCaps.wPeriodMin, m_nMinResolution);
	m_nMaxResolution = timeCaps.wPeriodMax;
  	timeBeginPeriod(m_nMinResolution);

  #else
	  timespec_t res;
	  m_bResolution = (clock_getres(CLOCK_REALTIME, &res) == 0);
    if (!m_bResolution)
      return false;
	  m_nMinResolution = timespec2timer(res);
    m_nMaxResolution = numeric_limits<sspTimerType>::max();

  #endif

  return true;
}

void sspTimeResources::terminate()
{
  timeEndPeriod(m_nMinResolution);
}

#if !defined (WIN32)

bool sspTimeResources::createTimer(timer_t nID, struct sigevent& event, sspTimerType nResolution)
{
  bool bOK = false;

  if (!m_signals.empty()) {
  	event.sigev_notify = SIGEV_SIGNAL;
    event.sigev_signo = (nResolution > HIGH_PRIORITY_LIMIT) ? m_signals.back() : m_signals.front();
  	bOK = (timer_create(CLOCK_REALTIME, &event, &nID) >= 0);
  }

  if (bOK) {
    m_timers[nID] = event.sigev_signo;
    if (nResolution > HIGH_PRIORITY_LIMIT)
      m_signals.pop_back();
    else
      m_signals.pop_front();
  }

  return bOK;
}

void sspTimeResources::deleteTimer(timer_t nID)
{
  timer_delete(nID);
  map<timer_t, int>::iterator finder = m_timers.find(nID);
  if (finder != m_timers.end()) {
    int nSignal = finder->second;
    if (nSignal < m_signals.front())
      m_signals.push_front(nSignal);
    else
      m_signals.push_back(nSignal);
    m_timers.erase(finder);
  }
}

#endif

sspTimeResources& sspTimeResources::Instance()
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

void sspTimeResources::create()
{
	static sspTimeResources theInstance;
	m_pInstance = &theInstance;
}

void sspTimeResources::onDeadReference()
{
	throw std::runtime_error("Dead reference to timer resolution detected");
}
