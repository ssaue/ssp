//////////////////////////////////////////
// sspPlayManager.h

#ifndef SSP_PLAYMANAGER_H
#define SSP_PLAYMANAGER_H

#include "SSpObject.h"
#include "sspScheduler.h"
#include "sspMsgList.h"
#include <sspTime.h>

class SSpStream;

// Class sspPlayManager is implemented as a Singleton pattern
class sspPlayManager : public sspIObase
{
protected:
  sspCondMsgList    m_listStart;
  sspTriggerMsgList m_listTrigger;
  sspClockMsgList   m_listClock;

  SSpStream*    m_pRootStream;
  sspScheduler  m_scheduler;
	bool				  m_bPlaying;
  sspTimeUpdate m_update;   // Update intervals for play thread verification

  // Time references
	sspTime m_startRun;
  sspTime m_previous;

private:
	static sspPlayManager* m_pInstance;
	static bool m_bDestroyed;

public:
	static sspPlayManager& Instance();

 	void Serialize(CArchive& ar);
	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);

	bool initialize();
	bool start();
	bool update();
	void stop();
	void terminate();
	void clearContents();

  bool isPlaying() const {return m_bPlaying;}
  bool verifyRunning();
	void adjustVolume();

	sspCondMsgList&	    getStartList() {return m_listStart;}
	sspTriggerMsgList&  getTriggerList() {return m_listTrigger;}
	sspClockMsgList&    getClockList() {return m_listClock;}
  sspScheduler&       getScheduler() {return m_scheduler;}

  double secondsRunning() {return sspTimeConvert::time2secs(sspTime() - m_startRun);}


private:
	sspPlayManager();
	sspPlayManager(const sspPlayManager&);
	sspPlayManager& operator=(const sspPlayManager&);
	~sspPlayManager();

	static void create();
	static void onDeadReference();
};

#endif