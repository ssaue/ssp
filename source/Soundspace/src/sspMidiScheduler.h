///////////////////////////////////////
// sspMidiScheduler.h : header file
///////////////////////////////////////

#ifndef sspMidiScheduler_H
#define sspMidiScheduler_H

#include "sspScheduler.h"
#include <sspMidi.h>

class sspValue;
class sspMsgHandler;
class sspMidiOutDevice;

class sspMidiScheduler : public sspScheduleObject
{
private:
	sspMidiSequence				    m_sequence;
	sspMidiSequence::iterator	m_iterator;

	double				    m_dDuration;
	sspValue*			    m_pTimeFactor;
	sspMidiOutDevice*	m_pDevice;
	sspMsgHandler*		m_pOwner;

public:
	sspMidiScheduler();
	virtual ~sspMidiScheduler();

  void setSequence(const sspMidiSequence& sequence);
  void swapSequence(sspMidiSequence& sequence); 

	void setTimeFactor(sspValue* pFactor);
	void setMidiDevice(int nDevice);
	void setOwner(sspMsgHandler* pOwner);
  long getLength() const {return m_sequence.size();}
	unsigned int getDuration() const;

	bool begin();
	bool update();
	void end();

private:
  void setDuration();
};

#endif