///////////////////////////////
// sspTimer.h : header file
///////////////////////////////

#ifndef SSPTIMER_H
#define SSPTIMER_H

#include "sspTime.h"
#include "sspSynch.h"

//#if defined (WIN32)
//// The following definition assumes NT Service Pack 3 or later
//// (if earlier versions are needed, delete the definition).
//#define _WIN32_WINNT 0x0500
//#include "windows.h"
//
//#elif defined (sgi)
//#include <time.h>
//#include <signal.h>
//#include <pthread.h>
//#include <mutex.h>
//#endif

///////////////////////////////////////////////

/*
** PLEASE NOTE: The timer must be verified for non-Windows !!!!!!!!!!!
** And in particular for Linux
*/

class sspTimerObject;

class sspTimer
{
protected:
	sspTimerObject* m_pTimerObject;
	bool			      m_bInitialized;
	sspTimerType    m_nMinTime;
	sspTimerType    m_nMaxTime;

  sspSynch::atomicType	m_bReentrant;
	sspSynch::atomicType	m_bLoop;
	sspSynch::atomicType	m_bPlaying;
	sspSynch::atomicType	m_bPaused;
	sspSynch::atomicType	m_bStopped;

#if defined (WIN32)
	unsigned int m_nTimerID;
	unsigned long m_nTimeBuffer;

	sspTimerType newTime();
	sspTimerType newTime(sspTimerType nMsecs);

#else
	timer_t				    m_timer;
	itimerspec_t		  m_interSpec;
#endif

	virtual void next() = 0;
	void	exit();

public:
	sspTimer(sspTimerType nResolution);
	virtual ~sspTimer();

	virtual bool run(sspTimerObject* pObject) = 0;

	void	pause();
	void	stop();
	void	loop();

  int	  getTimeResolution() const {return m_nMinTime;}

	bool	isLoaded() const {return m_pTimerObject != NULL;}
	bool	isPlaying() const {return m_bPlaying == sspSynch::atomicTrue;}
	bool	isPausing() const {return m_bPaused == sspSynch::atomicTrue;}
	bool	isLooping() const {return m_bLoop == sspSynch::atomicTrue;}
  bool  isInitialized() const {return m_bInitialized;}
};

class sspOneShotTimer : public sspTimer
{
public:
	sspOneShotTimer(sspTimerType nResolution);
	bool run(sspTimerObject* pObject);

protected:
	void next();
#if defined (WIN32)
	static void CALLBACK timerCB(unsigned int wID, unsigned int msg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);
#else
  static void	signalHandler(int nSig, siginfo_t* pSigInfo, void* pContext);
#endif
};

class sspPeriodicTimer : public sspTimer
{
public:
	sspPeriodicTimer(sspTimerType nResolution);
	bool run(sspTimerObject* pObject);

protected:
	void next();
#if defined (WIN32)
	static void CALLBACK timerCB(unsigned int wID, unsigned int msg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);
#else
  static void	signalHandler(int nSig, siginfo_t* pSigInfo, void* pContext);
#endif
};

#endif
