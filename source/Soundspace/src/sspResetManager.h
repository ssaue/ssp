//////////////////////////////////////////
// sspResetManager.h

#ifndef SSP_RESETMANAGER_H
#define SSP_RESETMANAGER_H

#include "ClockDate.h"
#include "SSpObject.h"

class sspWatchdog
{
public:
  sspWatchdog(void) {}
  virtual ~sspWatchdog(void) {}

	virtual bool initialize(int nSecs) = 0;
	virtual bool enable() = 0;
	virtual void refresh() = 0;
	virtual bool disable() = 0;
	virtual bool terminate() = 0;

  virtual int counter() {return 0;}
  static sspWatchdog* create(int nType);
};

// Class sspResetManager is implemented as a Singleton pattern
class sspResetManager : public sspIObase
{
public:
  enum tTimeUnit {TUNIT_HOURS, TUNIT_DAYS};

protected:
  bool      m_bWatchdog;
  int       m_nWatchdog;
  int       m_nTimeOut;
  tTimeUnit m_nUnit;
  int       m_nInterval;
  int       m_nCondition;
  SSpClock  m_timeOfDay;

private:
	static sspResetManager* m_pInstance;
	static bool m_bDestroyed;

  sspWatchdog*  m_pDog;
  bool          m_bTimeReset;
  sspTime       m_resetTime;
  enum {TIMEOFDAY_ROUNDOFF = 600};  // Within 10 minutes is OK

public:
	static sspResetManager& Instance();

 	void Serialize(CArchive& ar);
	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);
  void clearContents();

	bool initialize();
	bool begin();
	bool update();
	bool end();
	bool terminate();

  void enableWatchdog(bool bEnable) {m_bWatchdog = bEnable;}
  void setWathdog(int nType) {m_nWatchdog = nType;}
  void setTimeOut(int nSecs) {m_nTimeOut = nSecs;}
  void setTimeUnit(tTimeUnit nUnit) {m_nUnit = nUnit;}
  void setResetInterval(int nInterval) {m_nInterval = nInterval;}
  void setTimeOfDay(const SSpClock& clock) {m_timeOfDay = clock;}
  void setResetCondition(int nIndex) {m_nCondition = nIndex;}

  bool hasWatchdog() const {return m_bWatchdog;}
  int getWathdog() const {return m_nWatchdog;}
  int getTimeOut() const {return m_nTimeOut;}
  tTimeUnit getTimeUnit() const {return m_nUnit;}
  int getResetInterval() const {return m_nInterval;}
  const SSpClock& getTimeOfDay() const {return m_timeOfDay;}
  int getResetCondition() const {return m_nCondition;}

  static bool softReboot();

private:
	sspResetManager();
	sspResetManager(const sspResetManager&);
	sspResetManager& operator=(const sspResetManager&);
	~sspResetManager();

	static void create();
	static void onDeadReference();
};

#endif