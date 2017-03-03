//////////////////////////////////////////
// sspTimeResources.h
//
// This class retrieves the timer resolution available
// It is implemented using a Singleton pattern
//

#ifndef SSP_TIMERRESOLUTION_H
#define SSP_TIMERRESOLUTION_H

#include "sspTime.h"
#include <map>
#include <deque>

class sspTimeResources
{
private:
	static sspTimeResources* m_pInstance;
	static bool m_bDestroyed;

  bool          m_bResolution;
  sspTimerType  m_nMinResolution;
  sspTimerType  m_nMaxResolution;

  // Structures for maintaining the timer resources
  #if !defined (WIN32)
  enum {HIGH_PRIORITY_LIMIT = 10};
    std::deque<int> m_signals;
    std::map<timer_t, int> m_timers;
  #endif

public:
	static sspTimeResources& Instance();

  bool initialize();
  void terminate();

#if !defined (WIN32)
  bool createTimer(timer_t nID, struct sigevent& event, sspTimerType nResolution);
  void deleteTimer(timer_t nID);
#endif

  sspTimerType getMinResolution() {return m_nMinResolution;}
  sspTimerType getMaxResolution() {return m_nMaxResolution;}

private:
	sspTimeResources();
	sspTimeResources(const sspTimeResources&);
	sspTimeResources& operator=(const sspTimeResources&);
	~sspTimeResources();

	static void create();
	static void onDeadReference();
};

#endif
