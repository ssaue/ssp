// sspSynch.h: interface for the sspSynch classes
//
//////////////////////////////////////////////////////////////////////

#ifndef SSPSYNCH_H
#define SSPSYNCH_H

#ifdef WIN32
  #if !defined(_WINDOWS_)
    #include <windows.h>
  #endif
#else
  #include <pthread.h>
#endif

namespace sspSynch
{
  #if defined(WIN32)
	  typedef long atomicType;
  #else
	  typedef sig_atomic_t atomicType;
  #endif

  const atomicType atomicFalse = 0;
  const atomicType atomicTrue = 1;

  inline void assignAtomic(atomicType& nOld, atomicType nVal)
  {
    #if defined (WIN32)
	    InterlockedExchange(&nOld, nVal);
    #else
	    nOld = nVal;
    #endif
  }
}

class sspSynchObject  
{
private:
#ifdef WIN32
	enum {SPIN_COUNT=4000};
	CRITICAL_SECTION m_cs;
#else
	pthread_mutex_t m_mutex;
#endif

public:
	sspSynchObject();
	virtual ~sspSynchObject();

	void lock();
	bool tryLock();
	void unLock();
};

inline void sspSynchObject::lock()
{
#ifdef WIN32
	EnterCriticalSection(&m_cs);
#else
  pthread_mutex_lock(&m_mutex);
#endif
}

inline void sspSynchObject::unLock()
{
#ifdef WIN32
	LeaveCriticalSection(&m_cs);
#else
  pthread_mutex_unlock(&m_mutex);
#endif
}

inline bool sspSynchObject::tryLock()
{
#ifdef WIN32
  #if (_WIN32_WINNT >= 0x0400)
  	return TryEnterCriticalSection(&m_cs) == TRUE;
  #else
    // Only available on WinNT and later (not Win95 or Win98)
    return false;
  #endif
#else
  return (pthread_mutex_trylock(&m_mutex) == 0);
#endif
}

// Class to automate semaphore operations.
// Whenever an object of this type is created, the semaphore will be locked.
// When it goes out of scope, it will automatically be unlocked, thus
// it is completely exception safe and programmer safe.

class sspAutoLock
{
public: 
  sspAutoLock(sspSynchObject& lock) : m_lock(lock) {m_lock.lock();}
  virtual ~sspAutoLock() {m_lock.unLock();}

private:
  // Do not copy this object
  sspAutoLock(sspAutoLock const &);
  sspAutoLock& operator=(sspAutoLock const &);
  sspSynchObject& m_lock;
};

#endif 
