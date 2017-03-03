///////////////////////////////////////
// sspScheduleObjects.h : header file
///////////////////////////////////////

#ifndef sspScheduleObjects_H
#define sspScheduleObjects_H

#include "sspScheduler.h"
#include "SSpMessage.h"
#include "SSpBasic.h"
#include <time.h>
#include <sspMidi.h>

class sspSilenceObject : public sspScheduleObject
{
private:
	sspValue*		m_pValue;
	float			m_fValue;
	sspMsgHandler*	m_pOwner;

public:
	sspSilenceObject(sspValue* pValue);
	~sspSilenceObject();

	bool begin();
	bool update();
	void end();

	void setDuration(float fVal) {m_fValue = fVal; m_pValue = NULL;}
	void setDuration(sspValue* pValue) {m_pValue = pValue;}
	void setOwner(sspMsgHandler* pOwner);
};

#endif