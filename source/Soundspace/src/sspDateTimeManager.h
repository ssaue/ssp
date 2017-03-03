//////////////////////////////////////////
// sspDateTimeManager.h

#ifndef SSP_DATETIMEMANAGER_H
#define SSP_DATETIMEMANAGER_H

#include "SSpObject.h"
#include <sspTime.h>
#include "sspConditionals.h"

class sspDateTimeManager : public sspIObase
{
protected:
  // Configurable member variables
	SSpClock 	      m_startTime;
	SSpClock 	      m_endTime;
  bool            m_bInterval;

  // State variables
  SSpTimeInterval	m_playInterval;
	sspLocalTime    m_current;
  sspTimeUpdate   m_update;
  bool            m_bIsInside;
  bool            m_bUpdated;

private:
	static sspDateTimeManager* m_pInstance;
	static bool m_bDestroyed;

public:
	static sspDateTimeManager& Instance();
   	
  void Serialize(CArchive& ar);
	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);

  const sspLocalTime& current() const {return m_current;}

	bool start();
	bool update();

  bool isUpdated() const {return m_bUpdated;}
  bool isInsidePlayInterval() const {return m_bIsInside;}

  void enablePlayInterval(bool bEnable=true) {m_bInterval = bEnable;}
  void setStartTime(const SSpClock& clock) {m_startTime = clock; m_playInterval.setStartClock(m_startTime);}
  void setEndTime(const SSpClock& clock) {m_endTime = clock; m_playInterval.setEndClock(m_endTime);}

  bool  hasPlayInterval() const {return m_bInterval;}
  const SSpClock& getStartTime() const {return m_startTime;}
  const SSpClock& getEndTime() const {return m_endTime;}

private:
	sspDateTimeManager();
	sspDateTimeManager(const sspDateTimeManager&);
	sspDateTimeManager& operator=(const sspDateTimeManager&);
	virtual ~sspDateTimeManager();

	bool updateTime();

	static void create();
	static void onDeadReference();
};

#endif // !defined(AFX_CLOCKDATE_H__9576A663_9118_11D3_901C_B11BC2A3FAB6__INCLUDED_)
