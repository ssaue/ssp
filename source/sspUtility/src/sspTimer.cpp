///////////////////////////////////////////
// sspTimer.cpp : implementation file
///////////////////////////////////////////

#include "sspTimer.h"
#include "sspTimerInterface.h"
#include "sspTimeResources.h"

using namespace sspSynch;

//////////////////////////////////////////////////////////////////////
// sspTimer Class
//////////////////////////////////////////////////////////////////////

#if defined (WIN32)

inline sspTimerType sspTimer::newTime(sspTimerType nMsecs)
{
	if (nMsecs > m_nMaxTime) {
		m_nTimeBuffer = nMsecs - m_nMaxTime;
		return m_nMaxTime;
	}
	else
		return nMsecs;
}

inline sspTimerType sspTimer::newTime()
{
	if (m_nTimeBuffer > m_nMaxTime) {
		m_nTimeBuffer -= m_nMaxTime;
		return m_nMaxTime;
	}
	else {
		sspTimerType nMsecs = m_nTimeBuffer;
		m_nTimeBuffer = 0;
		return nMsecs;
	}
}

#endif

sspTimer::sspTimer(sspTimerType nResolution)
{
	m_pTimerObject = NULL;
	m_bReentrant = m_bLoop = m_bPlaying = m_bStopped = m_bPaused = atomicFalse;
  m_bInitialized = sspTimeResources::Instance().initialize();
  if (m_bInitialized) {
    m_nMinTime = max(sspTimeResources::Instance().getMinResolution(), nResolution);
    m_nMaxTime = sspTimeResources::Instance().getMaxResolution();

#if defined (WIN32)

	  m_nTimerID = 0;
	  m_nTimeBuffer = 0;

#else

	  struct sigevent sigEvt;
	  sigEvt.sigev_value.sival_ptr = (void*) this;
	  m_timer = 0;
    m_bInitialized = sspTimeResources::Instance().createTimer(m_timer, sigEvt, nResolution);
    if (m_bInitialized) {
	    struct sigaction sigAct;
	    sigAct.sa_flags = SA_SIGINFO;
	    sigAct.sa_handler = NULL;
	    sigemptyset(&sigAct.sa_mask);
	    sigAct.sa_sigaction = signalHandler;
	    m_bInitialized = (sigaction(sigEvt.sigev_signo, &sigAct, NULL) == 0);
    }

#endif
  
  }
}

sspTimer::~sspTimer()
{

#if defined (WIN32)

	if (m_nTimerID)	timeKillEvent(m_nTimerID);

#else

  sspTimeResources::Instance().deleteTimer(m_timer);

#endif

}

void sspTimer::exit()
{
	m_pTimerObject->end();
	assignAtomic(m_bPlaying, atomicFalse);

#if defined (WIN32)
	if (m_nTimerID) {
		timeKillEvent(m_nTimerID);
		m_nTimerID = 0;
	}

#else
  sspTimeConvert::timer2timespec(0, m_interSpec.it_value);
	timer_settime(m_timer, 0, &m_interSpec, NULL);
#endif
}

void sspTimer::pause()
{
	if (m_bPaused == atomicTrue)
		run(m_pTimerObject);
	else
		assignAtomic(m_bPaused, atomicFalse);
}

void sspTimer::stop()
{
  if (m_bPlaying == atomicTrue) {
	  assignAtomic(m_bStopped, atomicTrue);
	  if (m_bPaused == atomicTrue)
		  assignAtomic(m_bPaused, atomicFalse);
	  next();
  }
}

void sspTimer::loop()
{
	if (m_bLoop == atomicTrue)
		assignAtomic(m_bLoop, atomicTrue);
	else
		assignAtomic(m_bLoop, atomicFalse);
}

//////////////////////////////////////////////////////////////////////
// sspOneShotTimer Class
//////////////////////////////////////////////////////////////////////

sspOneShotTimer::sspOneShotTimer(sspTimerType nResolution) : sspTimer(nResolution)
{
#if !defined (WIN32)
  sspTimeConvert::timer2timespec(0, m_interSpec.it_interval);
#endif
}

bool sspOneShotTimer::run(sspTimerObject* pObject)
{
	if ((pObject == NULL) || !m_bInitialized)
		return false;

	m_pTimerObject = pObject;
	if (m_bPaused == atomicTrue) {
		assignAtomic(m_bPaused, atomicFalse);
		next();
		return true;
	}
	if (m_bPlaying == atomicTrue)
		return true;

#if defined (WIN32)
	m_nTimeBuffer = 0;
#endif

	sspTimerType nTime;
	if (!m_pTimerObject->begin(nTime)) {
		m_pTimerObject->end();
		return true;
	}
	while (nTime < m_nMinTime) {
		if (!m_pTimerObject->update(nTime)) {
			m_pTimerObject->end();
			return true;
		}
	}
	assignAtomic(m_bPlaying, atomicTrue);
	assignAtomic(m_bStopped, atomicFalse);

#if defined (WIN32)
	m_nTimerID = timeSetEvent(newTime(nTime), m_nMinTime, (LPTIMECALLBACK) timerCB, (DWORD_PTR) this, TIME_ONESHOT);
	if (!m_nTimerID) {
		exit();
		return false;
	}

#else
  sspTimeConvert::timer2timespec(nTime, m_interSpec.it_value);
	if (timer_settime(m_timer, 0, &m_interSpec, NULL) != NULL) {
		exit();
		return false;
	}
#endif

	return true;
}

void sspOneShotTimer::next()
{
	sspTimerType nTime;
  if (m_bReentrant == atomicTrue) return;
	assignAtomic(m_bReentrant, atomicTrue);
	if (m_bStopped == atomicTrue)
		exit();
	else if (m_bPaused == atomicFalse) {

#if defined (WIN32)
		if (m_nTimeBuffer > m_nMinTime) {
			m_nTimerID = timeSetEvent(newTime(), m_nMinTime, (LPTIMECALLBACK) timerCB, (DWORD_PTR) this, TIME_ONESHOT);
			if (!m_nTimerID)
				exit();
		}
		else {
			do {
				if (!m_pTimerObject->update(nTime)) {
					if (m_bLoop == atomicTrue) {
						if (!m_pTimerObject->begin(nTime)) {
							exit();
							break;
						}
					}
					else {
						exit();
						break;
					}
				}
			}
			while (nTime < m_nMinTime);
			if (m_bPlaying == atomicTrue) {
				m_nTimerID = timeSetEvent(newTime(nTime), m_nMinTime, (LPTIMECALLBACK) timerCB, (DWORD_PTR) this, TIME_ONESHOT);
				if (!m_nTimerID)
					exit();
			}
		}

#else
		do {
			if (!m_pTimerObject->update(nTime)) {
				if (m_bLoop == atomicTrue) {
					if (!m_pTimerObject->begin(nTime)) {
						exit();
						break;
					}
				}
				else {
					exit();
					break;
				}
			}
		}
		while (nTime < m_nMinTime);
		if (m_bPlaying == atomicTrue) {
      sspTimeConvert::timer2timespec(nTime, m_interSpec.it_value);
			if (timer_settime(m_timer, 0, &m_interSpec, NULL) != NULL)
				exit();
		}
#endif

	}
	assignAtomic(m_bReentrant, atomicFalse);
	return;
}

#if defined (WIN32)
  void CALLBACK sspOneShotTimer::timerCB(unsigned int wID, unsigned int msg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
  {
	  ((sspOneShotTimer*) dwUser)->next();
  }

#else
  void sspOneShotTimer::signalHandler(int nSig, siginfo_t* pSigInfo, void* pContext)
  {
	  if (nSig == SON_SIGNAL && pSigInfo->si_errno == 0)
		  ((sspOneShotTimer*) pSigInfo->si_value.sival_ptr)->next();
  }

#endif

//////////////////////////////////////////////////////////////////////
// sspPeriodicTimer Class
//////////////////////////////////////////////////////////////////////

sspPeriodicTimer::sspPeriodicTimer(sspTimerType nResolution) : sspTimer(nResolution)
{
}

bool sspPeriodicTimer::run(sspTimerObject* pObject)
{
	if ((pObject == NULL) || !m_bInitialized)
		return false;

	m_pTimerObject = pObject;
	if (m_bPaused == atomicTrue) {
		assignAtomic(m_bPaused, atomicFalse);
		next();
		return true;
	}
	if (m_bPlaying == atomicTrue)
		return true;

#if defined (WIN32)
	m_nTimeBuffer = 0;
#endif

	sspTimerType nTime;
  // Periodic timers must be within the resolution range
	if (!m_pTimerObject->begin(nTime) || nTime < m_nMinTime || nTime > m_nMaxTime) {
		m_pTimerObject->end();
		return true;
	}
	assignAtomic(m_bPlaying, atomicTrue);
	assignAtomic(m_bStopped, atomicFalse);

#if defined (WIN32)
	m_nTimerID = timeSetEvent(nTime, m_nMinTime, (LPTIMECALLBACK) timerCB, (DWORD_PTR) this, TIME_PERIODIC);
	if (!m_nTimerID) {
		exit();
		return false;
	}

#else
  sspTimeConvert::timer2timespec(nTime, m_interSpec.it_value);
  sspTimeConvert::timer2timespec(nTime, m_interSpec.it_interval);
	if (timer_settime(m_timer, 0, &m_interSpec, NULL) != NULL) {
		exit();
		return false;
	}
#endif

	return true;
}

void sspPeriodicTimer::next()
{
	sspTimerType nTime;
  if (m_bReentrant == atomicTrue) return;
	assignAtomic(m_bReentrant, atomicTrue);
	if (m_bStopped == atomicTrue)
		exit();
	else if (m_bPaused == atomicFalse) {
		if (!m_pTimerObject->update(nTime)) {
			if (m_bLoop == atomicTrue) {
				if (!m_pTimerObject->begin(nTime)) {
					exit();
				}
			}
			else {
				exit();
			}
		}
	}
	assignAtomic(m_bReentrant, atomicFalse);
	return;
}

#if defined (WIN32)
  void CALLBACK sspPeriodicTimer::timerCB(unsigned int wID, unsigned int msg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
  {
	  ((sspPeriodicTimer*) dwUser)->next();
  }

#else
  void sspPeriodicTimer::signalHandler(int nSig, siginfo_t* pSigInfo, void* pContext)
  {
	  if (nSig == SON_SIGNAL && pSigInfo->si_errno == 0)
		  ((sspPeriodicTimer*) pSigInfo->si_value.sival_ptr)->next();
  }

#endif

