// sspEvent.h: interface for the sspEvent class.
//
//////////////////////////////////////////////////////////////////////

#ifndef SSPEVENT_H
#define SSPEVENT_H

#ifdef WIN32
  #ifndef _WINDOWS_
    #include <windows.h>
  #endif
#else
  #include <pthread.h>
#endif

class sspEvent
{
public:
	bool timedWait(int millisec);
	bool isSet();
	bool wait();
	void reset();
	void set();
	sspEvent();
	virtual ~sspEvent();

private:
  // Do not copy this object
  sspEvent(sspEvent const &);
  sspEvent& operator=(sspEvent const &);

#ifdef WIN32
  HANDLE  m_hEvent;
#else
	pthread_mutex_t 	m_mutex;
  pthread_cond_t    m_cond;
  bool              m_bSet;
#endif
};

#endif
