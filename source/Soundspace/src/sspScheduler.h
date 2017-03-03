///////////////////////////////
// sspScheduler.h : header file
///////////////////////////////

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <sspSynch.h>
#include <sspEvent.h>
#include <sspTimerInterface.h>
#include <list>

class sspTimer;

class sspScheduleObject
{
public:
	int		m_nNextTime;
	bool	m_bPlaying;

	sspScheduleObject();

	virtual	bool begin() = 0;
	virtual bool update() = 0;
	virtual void end() = 0;

	bool isPlaying() {return m_bPlaying;}
	void setPlaying(bool bPlay=true) {m_bPlaying = bPlay;}
};

class sspScheduler : public sspTimerObject
{
private:
  typedef std::list<sspScheduleObject*>	tList;
  typedef tList::iterator tElem;

  enum {SSP_DEFAULT_TIME=50};
	tList         	m_list;
  sspEvent        m_event;
  sspTimer*       m_pTimer;
	sspSynchObject	m_synch;
	bool			      m_bCountDown;

public:
	sspScheduler();
	virtual ~sspScheduler();

	bool addObject(sspScheduleObject* pObj);
  bool verifyRunning(int nWaitSecs);
	void clear();
  bool isEmpty() const {return m_list.empty();}

  // Resolution is specified in milliseconds
  bool initialize(unsigned int nResolution);
  bool start();
  void stop();
  void terminate();

  // Virtual methods from sspTimerObject
	bool begin(sspTimerType& nMSecs);
	bool update(sspTimerType& nMSecs);
	void end(void);
};

#endif