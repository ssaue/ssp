// sspSynch.cpp: implementation of the sspSynch classes.
//////////////////////////////////////////////////////////////////////

#include "compiler.h"
#include "sspSynch.h"

//////////////////////////////////////////////////////////////////////
// sspSynchObject
//////////////////////////////////////////////////////////////////////

sspSynchObject::sspSynchObject()
{
#ifdef WIN32
  #if (_WIN32_WINNT >= 0x0403)
    InitializeCriticalSectionAndSpinCount(&m_cs, SPIN_COUNT);
  #else
    InitializeCriticalSection(&m_cs);
  #endif
#else
  pthread_mutex_init(&m_mutex, NULL);
#endif
}

sspSynchObject::~sspSynchObject()
{
#ifdef WIN32
 	DeleteCriticalSection(&m_cs);
#else
  pthread_mutex_destroy(&m_mutex);
#endif
}
