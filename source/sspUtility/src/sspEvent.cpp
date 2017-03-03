// sspEvent.cpp: implementation of the sspEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "sspEvent.h"
#include <assert.h>

#if !defined(WIN32)
  #include <sys/time.h>
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

sspEvent::sspEvent()
{
  #ifdef WIN32
    m_hEvent = ::CreateEvent(NULL,TRUE,FALSE,NULL);
  #else
    pthread_mutex_init(&m_mutex, NULL);
    pthread_cond_init(&m_cond, NULL);
    m_bSet = false;
  #endif
}

sspEvent::~sspEvent()
{
  #ifdef WIN32
    if (m_hEvent != NULL) ::CloseHandle(m_hEvent);
    m_hEvent = NULL;
  #else
    pthread_mutex_destroy(&m_mutex);
    pthread_cond_destroy(&m_cond);
  #endif
}

void sspEvent::set()
{
  #ifdef WIN32
    ::SetEvent(m_hEvent);    
  #else
    pthread_mutex_lock(&m_mutex);
    m_bSet = true;
    pthread_cond_broadcast(&m_cond);
    pthread_mutex_unlock(&m_mutex);
  #endif
}

void sspEvent::reset()
{
  #ifdef WIN32
    ::ResetEvent(m_hEvent);    
  #else
    pthread_mutex_lock(&m_mutex);
    m_bSet = false;
    pthread_mutex_unlock(&m_mutex);
  #endif
}

bool sspEvent::wait()
{
  bool bResult = false;
  #ifdef WIN32
    bResult = (::WaitForSingleObject(m_hEvent,INFINITE) == WAIT_OBJECT_0);
  #else
    pthread_mutex_lock(&m_mutex);
   while (!m_bSet) {
      pthread_cond_wait(&m_cond,&m_mutex);
    }
    pthread_mutex_unlock(&m_mutex);
  #endif
  return bResult;
}

bool sspEvent::isSet()
{
  return timedWait(0);
}

bool sspEvent::timedWait(int millisec)
{
  bool bResult = false;
  assert(millisec>=0);

  #ifdef WIN32
    bResult = (::WaitForSingleObject(m_hEvent,millisec) == WAIT_OBJECT_0);
  #else
    pthread_mutex_lock(&m_mutex);

    struct timeval time_now;
    struct timespec time_to_wait;

    gettimeofday(&time_now,NULL);

    long long tot_time_ms = ((long long)time_now.tv_sec) * 1000 + time_now.tv_usec / 1000;
    long long time_end_ms = tot_time_ms + millisec;

    time_to_wait.tv_sec = time_end_ms / 1000;
    time_to_wait.tv_nsec = (time_end_ms % 1000) * 1000000;

    if (!m_bSet) {
      pthread_cond_timedwait(&m_cond,&m_mutex,&time_to_wait);
    }
    bResult = m_bSet;
    pthread_mutex_unlock(&m_mutex);
  #endif

  return bResult;
}
